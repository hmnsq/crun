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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mount.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#ਜੇ_ਅੰਤ
#ਜੇ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/prctl.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAP
#  include <sys/capability.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/resource.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysmacros.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <grp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "terminal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "criu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "scheduler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "intelrdt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "io_priority.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "net_device.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/socket.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <libgen.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/wait.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/vfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/statvfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/personality.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <net/if.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/xattr.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/netlink.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/rtnetlink.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/magic.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/nsfs.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <json-c/json.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/json_common.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "mount_flags.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "syscalls.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPEN_ਰੁੱਖ_NAMESPACE
#  define OPEN_ਰੁੱਖ_NAMESPACE 2
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOVE_MOUNT_F_EMPTY_PATH
#  define MOVE_MOUNT_F_EMPTY_PATH 0x00000004
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOVE_MOUNT_T_EMPTY_PATH
#  define MOVE_MOUNT_T_EMPTY_PATH 0x00000040
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALL_PROPAGATIONS_NO_REC (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALL_PROPAGATIONS (MS_REC | ALL_PROPAGATIONS_NO_REC)

/* The per-mount flags a remount clears unless they are explicitly set.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REMOUNT_CLEAR_FLAGS (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW)

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ST_NOSYMFOLLOW
#  define ST_NOSYMFOLLOW 0x2000
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MOUNT_ATIME_FLAGS (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME)

/* Convert the ST_ flags reported by statfs(2) to the MS_ flags of mount(2).  */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
mount_flags_from_statfs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ f_flags)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags = 0;

  ਜੇ (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  ਜੇ (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  ਜੇ (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  ਜੇ (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  ਜੇ (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  ਜੇ (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  ਜੇ (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  ਜੇ (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  ਜੇ (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  ਵਾਪਸ flags;
}

ਬਣਤਰ remount_s
{
  ਬਣਤਰ remount_s *ਅਗਲਾ;
  ਅੱਖਰ *ਨਿਸ਼ਾਨਾ;
  ਪੂਰਨ targetfd;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
  ਅੱਖਰ *ਅੰਕੜੇ;
};

ਬਣਤਰ ਨਿੱਜੀ_ਅੰਕੜੇ_s
{
  ਬਣਤਰ remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  ਪੂਰਨ unshare_flags;
  ਪੂਰਨ unshare_cgroupns;

  ਅੱਖਰ *host_notify_socket_ਰਸਤਾ;
  ਅੱਖਰ *container_notify_socket_ਰਸਤਾ;
  ਅੱਖਰ *unified_cgroup_ਰਸਤਾ;

  bool mount_dev_from_host;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ rootfs_ਪ੍ਰਸਾਰ;
  bool deny_setgroups;

  ਪੂਰਨ procfd;

  ਅਚਲ ਅੱਖਰ *rootfs;
  ਪੂਰਨ rootfsfd;

  ਪੂਰਨ notify_socket_ਰੁੱਖ_fd;
  ਪੂਰਨ old_ਜੜ੍ਹ_fd;

  ਬਣਤਰ libcrun_fd_map *mount_fds;
  ਬਣਤਰ libcrun_fd_map *dev_fds;
  ਬਣਤਰ libcrun_fd_map *needed_devs_fds;

  ਅੱਖਰ **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  ਅੱਖਰ *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  ਪੂਰਨ maskdir_fd;
  ਅੱਖਰ *maskdir_proc_ਰਸਤਾ;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_ਧੁਰਾ;
  bool no_ਧੁਰਾ;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_ਜੜ੍ਹ_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  ਅੱਖਰ *host_rootfs;
};

ਬਣਤਰ linux_namespace_s
{
  ਅਚਲ ਅੱਖਰ *name;
  ਅਚਲ ਅੱਖਰ *ns_file;
  ਪੂਰਨ ਮੁੱਲ;
};

ਸਥਿਰ ਖਾਲੀ
cleanup_ਨਿੱਜੀ_ਅੰਕੜੇ (ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ)
{
  ਬਣਤਰ ਨਿੱਜੀ_ਅੰਕੜੇ_s *p = ਨਿੱਜੀ_ਅੰਕੜੇ;

  ਜੇ (p->rootfsfd >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (p->rootfsfd));
  ਜੇ (p->procfd >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (p->procfd));
  ਜੇ (p->maskdir_fd >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (p->maskdir_fd));
  ਜੇ (p->old_ਜੜ੍ਹ_fd >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (p->old_ਜੜ੍ਹ_fd));
  ਜੇ (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  ਜੇ (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  ਜੇ (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  ਜੇ (p->copy_symlink_targets)
    {
      size_t i;
      ਲਈ (i = 0; i < p->n_copy_symlink_targets; i++)
        ਮੁਕਤ (p->copy_symlink_targets[i]);
      ਮੁਕਤ (p->copy_symlink_targets);
    }
  ਮੁਕਤ (p->unified_cgroup_ਰਸਤਾ);
  ਮੁਕਤ (p->host_notify_socket_ਰਸਤਾ);
  ਮੁਕਤ (p->container_notify_socket_ਰਸਤਾ);
  ਮੁਕਤ (p->external_descriptors);
  ਮੁਕਤ (p->maskdir_proc_ਰਸਤਾ);
  ਮੁਕਤ (p->host_rootfs);
  ਮੁਕਤ (p);
}

ਸਥਿਰ ਬਣਤਰ ਨਿੱਜੀ_ਅੰਕੜੇ_s *
get_ਨਿੱਜੀ_ਅੰਕੜੇ (ਬਣਤਰ libcrun_container_s *container)
{
  ਜੇ (container->ਨਿੱਜੀ_ਅੰਕੜੇ == ਨਿਰਾ)
    {
      ਬਣਤਰ ਨਿੱਜੀ_ਅੰਕੜੇ_s *p = xmalloc0 (ਆਕਾਰ (*p));
      container->ਨਿੱਜੀ_ਅੰਕੜੇ = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_ਰੁੱਖ_fd = -1;
      p->old_ਜੜ੍ਹ_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_ਨਿੱਜੀ_ਅੰਕੜੇ = cleanup_ਨਿੱਜੀ_ਅੰਕੜੇ;
    }
  ਵਾਪਸ container->ਨਿੱਜੀ_ਅੰਕੜੇ;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CLONE_NEWTIME
#  define CLONE_NEWTIME 0x00000080
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CLONE_NEWCGROUP
#  define CLONE_NEWCGROUP 0x02000000
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ AT_RECURSIVE
#  define AT_RECURSIVE 0x8000
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਬਣਤਰ linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { ਨਿਰਾ, ਨਿਰਾ, 0 } };

ਸਥਿਰ ਪੂਰਨ
get_and_reset (ਪੂਰਨ *old)
{
  ਪੂਰਨ tmp = *old;
  *old = -1;
  ਵਾਪਸ tmp;
}

ਪੂਰਨ
libcrun_ਲੱਭੋ_namespace (ਅਚਲ ਅੱਖਰ *name)
{
  ਬਣਤਰ linux_namespace_s *it;
  ਲਈ (it = namespaces; it->name; it++)
    ਜੇ (strcmp (it->name, name) == 0)
      ਵਾਪਸ it->ਮੁੱਲ;
  ਵਾਪਸ -1;
}

ਸਥਿਰ ਪੂਰਨ
syscall_keyctl_join (ਅਚਲ ਅੱਖਰ *name)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KEYCTL_JOIN_SESSION_KEYRING 0x1
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

ਸਥਿਰ ਪੂਰਨ
syscall_pidfd_open (pid_t pid, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
#ਜੇ defined __NR_pidfd_open
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_pidfd_open, pid, flags);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) pid;
  (ਖਾਲੀ) flags;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਪੂਰਨ
syscall_pidfd_send_signal (ਪੂਰਨ pidfd, ਪੂਰਨ sig, siginfo_t *info, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
#ਜੇ defined __NR_pidfd_send_signal
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) pidfd;
  (ਖਾਲੀ) sig;
  (ਖਾਲੀ) info;
  (ਖਾਲੀ) flags;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
ਸਥਿਰ uint64_t
ms_flags_ਮੰਜ਼ਿਲ_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  ਜੇ (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  ਜੇ (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  ਜੇ (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  ਜੇ (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  ਜੇ (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  ਜੇ (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  ਜੇ (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  ਜੇ (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  ਵਾਪਸ attr;
}

ਸਥਿਰ ਪੂਰਨ
do_mount_setattr (bool recursive, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ, ਪੂਰਨ targetfd, uint64_t clear, uint64_t ਸੈੱਟ, libcrun_error_t *err)
{
  ਬਣਤਰ mount_attr_s attr = {
    0,
  };
  ਪੂਰਨ ret;

  ਸੈੱਟ &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.ਪ੍ਰਸਾਰ = ਸੈੱਟ & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_ਮੰਜ਼ਿਲ_mount_attr (ਸੈੱਟ & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_ਮੰਜ਼ਿਲ_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  ਜੇ (ਸੈੱਟ & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      ਜੇ (ਸੈੱਟ & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      ਨਹੀਂਤਾਂ ਜੇ (ਸੈੱਟ & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      ਨਹੀਂਤਾਂ
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      ਜੇ (ਸੈੱਟ & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  ਨਹੀਂਤਾਂ ਜੇ (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ, &attr);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "mount_setattr `/%s`", ਨਿਸ਼ਾਨਾ);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
get_bind_mount (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *src, bool recursive, bool rdonly, bool nofollow, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪ੍ਰਸਾਰ, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ open_ਰੁੱਖ_fd = -1;
  ਬਣਤਰ mount_attr_s attr = {
    0,
  };
  ਪੂਰਨ recursive_flag = (recursive ? AT_RECURSIVE : 0);
  ਪੂਰਨ ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  ਜੇ (UNLIKELY (src == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, EINVAL, "mount source is not specified");

  ਜੇ (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.ਪ੍ਰਸਾਰ = ਪ੍ਰਸਾਰ;

  ਜੇ (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_ਰੁੱਖ_fd = syscall_open_ਰੁੱਖ (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  ਜੇ (UNLIKELY (open_ਰੁੱਖ_fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_ਰੁੱਖ_fd, "", AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ | recursive_flag, &attr);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "mount_setattr `%s`", src);

  ਵਾਪਸ get_and_reset (&open_ਰੁੱਖ_fd);
}

ਪੂਰਨ
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, ਅੱਖਰ *option, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free ਅੱਖਰ *mappings = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *it;
  ਪੂਰਨ ret;

  mappings = xmalloc (allocated);

  ਲਈ (it = option; *it;)
    {
      bool relative = false;
      ਲੰਮਾ ਮੁੱਲ[3];
      size_t i;

      ਜੇ (*it == '\0')
        ਰੋਕੋ;

      ਜੇ (*it == '#')
        it++;

      ਜੇ (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      ਲਈ (i = 0; i < 3; i++)
        {
          ਅੱਖਰ *endptr = ਨਿਰਾ;

          ਜੇ (i > 0 && *it == '-')
            it++;

          ਜੇ (*it == '\0')
            ਵਾਪਸ crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          ਮੁੱਲ[i] = strtol (it, &endptr, 10);
          ਜੇ (errno || endptr == it)
            ਵਾਪਸ crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      ਜੇ (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          ਜੇ (def == ਨਿਰਾ
              || def->linux == ਨਿਰਾ
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            ਵਾਪਸ crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          ਲਈ (i = 0; i < mappings_len; i++)
            ਜੇ (ਮੁੱਲ[1] >= (int64_t) mappings[i]->container_id
                && ਮੁੱਲ[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->ਮਾਪ)
              ਰੋਕੋ;

          ਜੇ (i == mappings_len)
            ਵਾਪਸ crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          ਮੁੱਲ[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      ਜੇ (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", ਮੁੱਲ[0], ਮੁੱਲ[1], ਮੁੱਲ[2]);
      ਜੇ (UNLIKELY (ret >= (ਪੂਰਨ) (allocated - written)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *ਬਾਹਰਲਾ = mappings;
  mappings = ਨਿਰਾ;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
format_mount_mappings (ਅੱਖਰ **ਬਾਹਰਲਾ,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  ਅਚਲ size_t max_len_mapping = 64;
  cleanup_free ਅੱਖਰ *ret = ਨਿਰਾ;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  ਲਈ (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->ਮਾਪ);
      ਜੇ (UNLIKELY (len >= max_len_mapping))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *ਬਾਹਰਲਾ = ret;
  ret = ਨਿਰਾ;
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
format_mount_mapping (ਅੱਖਰ **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t ਮਾਪ, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .ਮਾਪ = ਮਾਪ,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    ਨਿਰਾ,
  };

  ਵਾਪਸ format_mount_mappings (ret, mappings, 1, written, err);
}

ਸਥਿਰ bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  ਜੇ (def->linux == ਨਿਰਾ)
    ਵਾਪਸ mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  ਜੇ (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    ਵਾਪਸ false;

  ਜੇ (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    ਵਾਪਸ false;

  ਲਈ (s = 0; s < mnt->uid_mappings_len; s++)
    {
      ਜੇ (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        ਵਾਪਸ false;
      ਜੇ (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        ਵਾਪਸ false;
      ਜੇ (mnt->uid_mappings[s]->ਮਾਪ != def->linux->uid_mappings[s]->ਮਾਪ)
        ਵਾਪਸ false;
    }

  ਲਈ (s = 0; s < mnt->gid_mappings_len; s++)
    {
      ਜੇ (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        ਵਾਪਸ false;
      ਜੇ (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        ਵਾਪਸ false;
      ਜੇ (mnt->gid_mappings[s]->ਮਾਪ != def->linux->gid_mappings[s]->ਮਾਪ)
        ਵਾਪਸ false;
    }

  ਵਾਪਸ true;
}

ਸਥਿਰ pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        ਅਚਲ ਅੱਖਰ *options, pid_t *pid_ਬਾਹਰਲਾ,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != ਨਿਰਾ;

  ਜੇ (! need_new_userns)
    ਵਾਪਸ 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, ਨਿਰਾ);
  ਜੇ (UNLIKELY (pid < 0))
    ਵਾਪਸ crun_make_error (err, errno, "clone");

  ਜੇ (pid == 0)
    {
      prctl (PR_ਸੈੱਟ_PDEATHSIG, SIGKILL);
      ਜਦੋਂ (1)
        pause ();
      ਸੁਰੱਖਿਅਤ_exit (EXIT_SUCCESS);
    }

  ਜੇ (mnt->uid_mappings_len)
    {
      cleanup_free ਅੱਖਰ *uid_map = ਨਿਰਾ;
      cleanup_free ਅੱਖਰ *gid_map = ਨਿਰਾ;
      size_t written = 0;
      ਪੂਰਨ ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      cleanup_close ਪੂਰਨ fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      ਜੇ (UNLIKELY (fd < 0))
        ਵਾਪਸ fd;

      ret = ਸੁਰੱਖਿਅਤ_write (fd, "uid_map", uid_map, written, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      ਜੇ (UNLIKELY (fd < 0))
        ਵਾਪਸ fd;

      ret = ਸੁਰੱਖਿਅਤ_write (fd, "gid_map", gid_map, written, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      cleanup_free ਅੱਖਰ *dup_options = ਨਿਰਾ;
      ਅੱਖਰ *option, *saveptr = ਨਿਰਾ;

      ਜੇ (! options)
        ਵਾਪਸ crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      ਲਈ (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (ਨਿਰਾ, ";", &saveptr))
        {
          cleanup_free ਅੱਖਰ *mappings = ਨਿਰਾ;
          cleanup_close ਪੂਰਨ map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          ਪੂਰਨ ret;

          ਜੇ (has_ਅਗੇਤਰ (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              ਜੇ (UNLIKELY (map_fd < 0))
                ਵਾਪਸ map_fd;
            }
          ਨਹੀਂਤਾਂ ਜੇ (has_ਅਗੇਤਰ (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              ਜੇ (UNLIKELY (map_fd < 0))
                ਵਾਪਸ map_fd;
            }
          ਨਹੀਂਤਾਂ
            ਵਾਪਸ crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          ret = ਸੁਰੱਖਿਅਤ_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }

  *pid_ਬਾਹਰਲਾ = pid;
  pid = -1;
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_create_keyring (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *label, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ labelfd = -1;
  bool label_ਸੈੱਟ = false;
  ਪੂਰਨ ret;

  ਜੇ (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      ਜੇ (UNLIKELY (labelfd < 0))
        {
          ਜੇ (crun_error_get_errno (err) != ENOENT)
            ਵਾਪਸ labelfd;

          crun_error_release (err);
        }

      ਜੇ (labelfd >= 0)
        {
          ret = ਲਿਖ (labelfd, label, ਲੜੀ_ਲੰਬਾਈ (label));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_ਸੈੱਟ = true;
        }
    }

  ret = syscall_keyctl_join (name);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          ਜਾਓ ਬਾਹਰਲਾ;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      ਜਾਓ ਬਾਹਰਲਾ;
    }

ਬਾਹਰਲਾ:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  ਜੇ (label_ਸੈੱਟ && ਲਿਖ (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  ਜੇ (def->process && def->process->user)
    {
      ਜੇ (def->process->user->uid)
        *uid = def->process->user->uid;
      ਜੇ (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
get_mount_flags (ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ current_flags, ਪੂਰਨ *ਮਿਲਿਆ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *extra_flags, uint64_t *rec_clear, uint64_t *rec_ਸੈੱਟ)
{
  ਅਚਲ ਬਣਤਰ ਪ੍ਰਸਾਰ_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  ਜੇ (ਮਿਲਿਆ)
    *ਮਿਲਿਆ = prop ? 1 : 0;

  ਜੇ (prop == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਜੇ (prop->extra_flags & OPTION_RECURSIVE)
    {
      ਜੇ (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      ਜੇ (rec_ਸੈੱਟ && ! prop->clear)
        *rec_ਸੈੱਟ |= prop->flags;
    }

  ਜੇ (extra_flags)
    *extra_flags |= prop->extra_flags;

  ਜੇ (prop->clear)
    ਵਾਪਸ current_flags & ~prop->flags;

  ਵਾਪਸ current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
ਸਥਿਰ bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ਲਈ (i = 0; i < mount->options_len; i++)
    {
      ਅਚਲ ਬਣਤਰ ਪ੍ਰਸਾਰ_flags_s *prop;

      ਜੇ (mount->options[i] == ਨਿਰਾ)
        ਜਾਰੀ;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ਜੇ (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        ਵਾਪਸ true;
    }

  ਵਾਪਸ false;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
get_mount_flags_or_option (ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ current_flags, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *extra_flags, ਅੱਖਰ **option, uint64_t *rec_clear, uint64_t *rec_ਸੈੱਟ)
{
  ਪੂਰਨ ਮਿਲਿਆ;
  __attribute__ ((ਅਣਵਰਤਿਆ)) cleanup_free ਅੱਖਰ *prev = ਨਿਰਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  ਜੇ (name == ਨਿਰਾ)
    ਵਾਪਸ 0;

  flags = get_mount_flags (name, current_flags, &ਮਿਲਿਆ, extra_flags, rec_clear, rec_ਸੈੱਟ);
  ਜੇ (ਮਿਲਿਆ)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ_flag = ਨਿਰਾ;
      ਜੇ ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        ਅੰਕੜੇ_flag = "sync";
      ਨਹੀਂਤਾਂ ਜੇ ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        ਅੰਕੜੇ_flag = "dirsync";

      ਜੇ (ਅੰਕੜੇ_flag)
        {
          prev = *option;
          ਜੇ (*option && **option)
            xasprintf (option, "%s,%s", *option, ਅੰਕੜੇ_flag);
          ਨਹੀਂਤਾਂ
            *option = xstrdup (ਅੰਕੜੇ_flag);
        }
      ਵਾਪਸ flags;
    }

  prev = *option;
  ਜੇ (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  ਨਹੀਂਤਾਂ
    *option = xstrdup (name);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
ਧੁਰਾ_ਜੜ੍ਹ (ਅਚਲ ਅੱਖਰ *new_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *put_old)
{
  ਵਾਪਸ syscall (__NR_pivot_root, new_ਜੜ੍ਹ, put_old);
}

ਸਥਿਰ ਖਾਲੀ
free_remount (ਬਣਤਰ remount_s *r)
{
  ਜੇ (r == ਨਿਰਾ)
    ਵਾਪਸ;
  ਜੇ (r->targetfd >= 0)
    close (r->targetfd);
  ਮੁਕਤ (r->ਅੰਕੜੇ);
  ਮੁਕਤ (r->ਨਿਸ਼ਾਨਾ);
  ਮੁਕਤ (r);
}

ਸਥਿਰ ਬਣਤਰ remount_s *
make_remount (ਪੂਰਨ targetfd, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags, ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ, ਬਣਤਰ remount_s *ਅਗਲਾ)
{
  ਬਣਤਰ remount_s *ret = xmalloc (ਆਕਾਰ (*ret));
  ret->ਨਿਸ਼ਾਨਾ = xstrdup (ਨਿਸ਼ਾਨਾ);
  ret->flags = flags;
  ret->ਅੰਕੜੇ = ਅੰਕੜੇ ? xstrdup (ਅੰਕੜੇ) : ਨਿਰਾ;
  ret->ਅਗਲਾ = ਅਗਲਾ;
  ret->targetfd = targetfd;
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
do_remount (ਪੂਰਨ targetfd, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags, ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ, libcrun_error_t *err)
{
  proc_fd_ਰਸਤਾ_t ਨਿਸ਼ਾਨਾ_ਬਫਰ;
  ਪੂਰਨ ret;
  ਅਚਲ ਅੱਖਰ *real_ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ;

  ਜੇ (targetfd >= 0)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸੈੱਟ_flags = flags & ~(MS_REMOUNT | MS_BIND);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ clear_flags = REMOUNT_CLEAR_FLAGS & ~ਸੈੱਟ_flags;
      ret = do_mount_setattr (false, ਨਿਸ਼ਾਨਾ, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      ਜੇ (LIKELY (ret == 0))
        ਵਾਪਸ 0;
      crun_error_release (err);

      get_proc_self_fd_ਰਸਤਾ (ਨਿਸ਼ਾਨਾ_ਬਫਰ, targetfd);
      real_ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ_ਬਫਰ;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  ਜੇ (flags & (MS_REMOUNT | MS_RDONLY))
    ਅੰਕੜੇ = ਨਿਰਾ;

  ret = mount (ਨਿਰਾ, real_ਨਿਸ਼ਾਨਾ, ਨਿਰਾ, flags, ਅੰਕੜੇ);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ current_flags, remount_flags;
      ਬਣਤਰ statfs sfs;

      ret = statfs (real_ਨਿਸ਼ਾਨਾ, &sfs);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "statfs `%s`", real_ਨਿਸ਼ਾਨਾ);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      ਜੇ (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      ਜੇ ((flags | remount_flags) != flags)
        {
          ret = mount (ਨਿਰਾ, real_ਨਿਸ਼ਾਨਾ, ਨਿਰਾ, flags | remount_flags, ਅੰਕੜੇ);
          ਜੇ (LIKELY (ret == 0))
            ਵਾਪਸ 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          ਜੇ (current_flags & MS_RDONLY)
            ret = mount (ਨਿਰਾ, real_ਨਿਸ਼ਾਨਾ, ਨਿਰਾ, flags | remount_flags | MS_RDONLY, ਅੰਕੜੇ);
        }
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "remount `%s`", ਨਿਸ਼ਾਨਾ);
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  ਪੂਰਨ ret = 0;
  ਬਣਤਰ remount_s *r = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->remounts;
  ਜਦੋਂ (r)
    {
      ਬਣਤਰ remount_s *ਅਗਲਾ = r->ਅਗਲਾ;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      ਜੇ (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸੈੱਟ_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ clear_flags = REMOUNT_CLEAR_FLAGS & ~ਸੈੱਟ_flags;

          ret = do_mount_setattr (false, r->ਨਿਸ਼ਾਨਾ, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          ਜੇ (LIKELY (ret == 0))
            {
              free_remount (r);
              r = ਅਗਲਾ;
              ਜਾਰੀ;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->ਨਿਸ਼ਾਨਾ, r->flags, r->ਅੰਕੜੇ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ cleanup;

      free_remount (r);
      r = ਅਗਲਾ;
    }

cleanup:
  ਜਦੋਂ (r)
    {
      ਬਣਤਰ remount_s *ਅਗਲਾ = r->ਅਗਲਾ;
      free_remount (r);
      r = ਅਗਲਾ;
    }

  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->remounts = ਨਿਰਾ;
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
open_mount_ਨਿਸ਼ਾਨਾ (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ_rel, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *rootfs = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs;
  ਪੂਰਨ rootfsfd = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd;

  ਜੇ (rootfsfd < 0)
    ਵਾਪਸ crun_make_error (err, 0, "invalid rootfs state");

  ਵਾਪਸ ਸੁਰੱਖਿਅਤ_openat (rootfsfd, rootfs, ਨਿਸ਼ਾਨਾ_rel, O_ਰਸਤਾ | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
ਸਥਿਰ ਪੂਰਨ
fsopen_mount (ਅਚਲ ਅੱਖਰ *type, ਅਚਲ ਅੱਖਰ *ਸਰੋਤ_name, ਅਚਲ ਅੱਖਰ *labeltype, ਅਚਲ ਅੱਖਰ *label, ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_NEW_MOUNT_API
  cleanup_close ਪੂਰਨ fsfd = -1;
  ਪੂਰਨ ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ਜੇ (UNLIKELY (fsfd < 0))
    ਵਾਪਸ fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (ਖਾਲੀ) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", ਸਰੋਤ_name ? ਸਰੋਤ_name : type, 0);

  ਜੇ (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (ਅੰਕੜੇ && ਅੰਕੜੇ[0])
    {
      cleanup_free ਅੱਖਰ *ਅੰਕੜੇ_copy = xstrdup (ਅੰਕੜੇ);
      ਅੱਖਰ *saveptr = ਨਿਰਾ;
      ਅੱਖਰ *token;

      ਲਈ (token = strtok_r (ਅੰਕੜੇ_copy, ",", &saveptr); token; token = strtok_r (ਨਿਰਾ, ",", &saveptr))
        {
          ਅੱਖਰ *eq = strchr (token, '=');
          ਜੇ (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          ਨਹੀਂਤਾਂ
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, ਨਿਰਾ, 0);

          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ਨਿਰਾ, ਨਿਰਾ, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) type;
  (ਖਾਲੀ) ਸਰੋਤ_name;
  (ਖਾਲੀ) ਅੰਕੜੇ;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਪੂਰਨ
get_procfd (ਬਣਤਰ ਨਿੱਜੀ_ਅੰਕੜੇ_s *ਅੰਕੜੇ, libcrun_error_t *err)
{
  ਪੂਰਨ fd;

  ਜੇ (ਅੰਕੜੇ->procfd >= 0)
    ਵਾਪਸ ਅੰਕੜੇ->procfd;

  fd = fsopen_mount ("proc", ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
  ਜੇ (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `/proc`");

  ਅੰਕੜੇ->procfd = fd;
  ਵਾਪਸ fd;
}

ਸਥਿਰ ਪੂਰਨ
get_old_ਜੜ੍ਹ_fd (ਬਣਤਰ ਨਿੱਜੀ_ਅੰਕੜੇ_s *ਅੰਕੜੇ)
{
  ਜੇ (ਅੰਕੜੇ->old_ਜੜ੍ਹ_fd >= 0)
    ਵਾਪਸ ਅੰਕੜੇ->old_ਜੜ੍ਹ_fd;

  ਅੰਕੜੇ->old_ਜੜ੍ਹ_fd = open ("/", O_ਰਸਤਾ | O_CLOEXEC);
  ਵਾਪਸ ਅੰਕੜੇ->old_ਜੜ੍ਹ_fd;
}

ਸਥਿਰ ਪੂਰਨ
fs_move_mount_ਮੰਜ਼ਿਲ (ਪੂਰਨ fd, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *name)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_NEW_MOUNT_API
  ਜੇ (name)
    ਵਾਪਸ syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  ਵਾਪਸ syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) fd;
  (ਖਾਲੀ) dirfd;
  (ਖਾਲੀ) name;
  (ਖਾਲੀ) syscall_move_mount;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

ਗਿਣਤੀ
{
  /* Do not apply any label to the mount.  */
  LABEL_NONE = 0,
  /* Apply the label as a mount option.  */
  LABEL_MOUNT = 1,
  /* Apply the label using setxattr.  */
  LABEL_XATTR = 2,

  LABEL_ਨਕਾਬ = 0x0f,

  /* Do not defer a readonly remount — apply MS_RDONLY directly.  */
  MOUNT_NO_DEFERRED_REMOUNT = 0x10,

  /* Do not remount — keep the flags the mount already has.  */
  MOUNT_NO_REMOUNT = 0x20,
};

ਸਥਿਰ ਪੂਰਨ do_mount (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਸਰੋਤ, ਪੂਰਨ targetfd,
                     ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ, ਅਚਲ ਅੱਖਰ *fstype, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mountflags,
                     ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਪੂਰਨ extra_flags, libcrun_error_t *err);

ਸਥਿਰ bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, ਅਚਲ ਅੱਖਰ *option)
{
  size_t i;
  ਲਈ (i = 0; i < mnt->options_len; i++)
    ਜੇ (mnt->options[i] != ਨਿਰਾ && strcmp (mnt->options[i], option) == 0)
      ਵਾਪਸ true;
  ਵਾਪਸ false;
}

ਸਥਿਰ runtime_spec_schema_defs_mount *
ਲੱਭੋ_mount_for (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  ਲਈ (i = 0; i < def->mounts_len; i++)
    {
      ਜੇ (strcmp (def->mounts[i]->destination, destination) == 0)
        ਵਾਪਸ def->mounts[i];
    }
  ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ
warn_tmpfs_fallback_once (ਬਣਤਰ ਨਿੱਜੀ_ਅੰਕੜੇ_s *ਨਿੱਜੀ_ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *reason)
{
  ਜੇ (! ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
ਸਥਿਰ ਪੂਰਨ
get_shared_ਖਾਲੀ_ਥਾਵਾਂ_dir_cached (libcrun_container_t *container, ਅੱਖਰ **proc_fd_ਰਸਤਾ, libcrun_error_t *err)
{
  ਬਣਤਰ ਨਿੱਜੀ_ਅੰਕੜੇ_s *ਨਿੱਜੀ_ਅੰਕੜੇ = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container);
  cleanup_close ਪੂਰਨ fd = -1;
  cleanup_free ਅੱਖਰ *ਖਾਲੀ_ਥਾਵਾਂ_dir_ਰਸਤਾ = ਨਿਰਾ;
  proc_fd_ਰਸਤਾ_t fd_ਰਸਤਾ;
  ਪੂਰਨ ret;

  /* Fast path: return cached proc fd path if already set up */
  ਜੇ (ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_proc_ਰਸਤਾ != ਨਿਰਾ)
    {
      ਜੇ (proc_fd_ਰਸਤਾ)
        *proc_fd_ਰਸਤਾ = ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_proc_ਰਸਤਾ;
      ਵਾਪਸ 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_ਖਾਲੀ_ਥਾਵਾਂ_directory_ਰਸਤਾ (&ਖਾਲੀ_ਥਾਵਾਂ_dir_ਰਸਤਾ, container->context->state_ਜੜ੍ਹ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, ਖਾਲੀ_ਥਾਵਾਂ_dir_ਰਸਤਾ, true, true, false, MS_PRIVATE, err);
  ਜੇ (fd < 0)
    ਵਾਪਸ fd;

  get_self_fd_ਰਸਤਾ (fd_ਰਸਤਾ, fd);
  ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_proc_ਰਸਤਾ = xstrdup (fd_ਰਸਤਾ);

  ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  ਜੇ (proc_fd_ਰਸਤਾ)
    *proc_fd_ਰਸਤਾ = ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_proc_ਰਸਤਾ;
  ਵਾਪਸ 0;
}

ਸਥਿਰ bool
is_dev_null (ਅਚਲ ਬਣਤਰ stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  ਵਾਪਸ S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
ਸਥਿਰ ਪੂਰਨ
ensure_dev_null (ਪੂਰਨ fd, libcrun_error_t *err)
{
  ਬਣਤਰ stat st;

  ਜੇ (UNLIKELY (fstat (fd, &st) < 0))
    ਵਾਪਸ crun_make_error (err, errno, "stat `/dev/null`");

  ਜੇ (UNLIKELY (! is_dev_null (&st)))
    ਵਾਪਸ crun_make_error (err, 0, "`/dev/null` is not the null device");

  ਵਾਪਸ 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
ਸਥਿਰ bool
is_ਖਾਲੀ_ਥਾਵਾਂ_ro_tmpfs (ਪੂਰਨ fd)
{
  ਬਣਤਰ statfs sfs;

  ਜੇ (fstatfs (fd, &sfs) < 0)
    ਵਾਪਸ false;

  ਵਾਪਸ sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

ਸਥਿਰ ਪੂਰਨ
mount_masked_dir (libcrun_container_t *container, ਪੂਰਨ pathfd, ਅਚਲ ਅੱਖਰ *rel_ਰਸਤਾ, libcrun_error_t *err)
{
  ਬਣਤਰ ਨਿੱਜੀ_ਅੰਕੜੇ_s *ਨਿੱਜੀ_ਅੰਕੜੇ = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container);
  cleanup_close ਪੂਰਨ newfd = -1;
  libcrun_error_t tmp_err = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ਜੇ (ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_fd < 0)
    {
      ਜੇ (ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_bind_failed)
        ਜਾਓ fallback_ਮੰਜ਼ਿਲ_tmpfs;

      ret = get_shared_ਖਾਲੀ_ਥਾਵਾਂ_dir_cached (container, ਨਿਰਾ, &tmp_err);
      ਜੇ (ret < 0)
        {
          ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (ਨਿੱਜੀ_ਅੰਕੜੇ, tmp_err->ਸੁਨੇਹਾ);
          crun_error_release (&tmp_err);
          ਜਾਓ fallback_ਮੰਜ਼ਿਲ_tmpfs;
        }
    }

  ਜੇ (ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_fd >= 0)
    {
      ਪੂਰਨ rootfsfd = ਨਿੱਜੀ_ਅੰਕੜੇ->rootfsfd;
      ਬਣਤਰ stat before, after;

      ret = fstat (ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_fd, &before);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "fstat masked dir `%s`", rel_ਰਸਤਾ);

      ret = fs_move_mount_ਮੰਜ਼ਿਲ (ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_fd, pathfd, ਨਿਰਾ);
      ਜੇ (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_ਰਸਤਾ, true, true, false, MS_PRIVATE, err);
          ਜੇ (UNLIKELY (newfd < 0))
            ਵਾਪਸ newfd;

          ret = fstat (newfd, &after);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "fstat masked dir `%s`", rel_ਰਸਤਾ);

          ਜੇ (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            ਵਾਪਸ crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_ਰਸਤਾ);

          ਆਰਜ਼ੀ_FAILURE_RETRY (close (ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_fd));
          ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_fd = newfd;
          newfd = -1;

          ਵਾਪਸ 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_ਰਸਤਾ);

      ਆਰਜ਼ੀ_FAILURE_RETRY (close (ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_fd));
      ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_fd = -1;
      ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_move_failed = true;
    }

  ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (ਨਿੱਜੀ_ਅੰਕੜੇ, tmp_err ? tmp_err->ਸੁਨੇਹਾ : "unknown error");
  crun_error_release (&tmp_err);

fallback_ਮੰਜ਼ਿਲ_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_ਰਸਤਾ);
  ret = do_mount (container, "tmpfs", pathfd, rel_ਰਸਤਾ, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  ਜੇ (! ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_move_failed)
    {
      ret = get_bind_mount (ਨਿੱਜੀ_ਅੰਕੜੇ->rootfsfd, rel_ਰਸਤਾ, true, true, false, MS_PRIVATE, &tmp_err);
      ਜੇ (ret >= 0 && is_ਖਾਲੀ_ਥਾਵਾਂ_ro_tmpfs (ret))
        ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_fd = ret;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (ret >= 0)
            ਆਰਜ਼ੀ_FAILURE_RETRY (close (ret));
          ਨਿੱਜੀ_ਅੰਕੜੇ->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
do_masked_or_readonly_ਰਸਤਾ (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rel_ਰਸਤਾ, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mount_flags = 0;
  ਅਚਲ ਅੱਖਰ *rootfs = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs;
  cleanup_close ਪੂਰਨ pathfd = -1;
  ਬਣਤਰ statfs sfs;
  ਪੂਰਨ ret;
  mode_t ਬਹੁਲਕ;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  ਜੇ (UNLIKELY (rel_ਰਸਤਾ == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  ਜੇ (rel_ਰਸਤਾ[0] == '/')
    rel_ਰਸਤਾ++;

  pathfd = ਸੁਰੱਖਿਅਤ_openat (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, rel_ਰਸਤਾ, O_ਰਸਤਾ | O_CLOEXEC, 0, err);
  ਜੇ (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      ਜੇ (errno != ENOENT && errno != EACCES)
        ਵਾਪਸ pathfd;

      ਜੇ (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_ਰਸਤਾ);
      crun_error_release (err);
      ਵਾਪਸ 0;
    }

  ਜੇ (readonly)
    {
      ਪੂਰਨ procfd = get_procfd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container), err);
      proc_fd_ਰਸਤਾ_t ਸਰੋਤ_ਬਫਰ;
      cleanup_close ਪੂਰਨ mountfd = -1;

      ਜੇ (UNLIKELY (procfd < 0))
        ਵਾਪਸ procfd;

      get_self_fd_ਰਸਤਾ (ਸਰੋਤ_ਬਫਰ, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      ਜੇ (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, ਸਰੋਤ_ਬਫਰ, true, true, false, MS_PRIVATE, err);
          ਜੇ (mountfd >= 0)
            ret = fs_move_mount_ਮੰਜ਼ਿਲ (mountfd, pathfd, ਨਿਰਾ);
          ਨਹੀਂਤਾਂ
            ret = -1;

          ਜੇ (ret < 0)
            crun_error_release (err);
        }

      ਜੇ (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          ਜੇ (keep_flags)
            {
              cleanup_close ਪੂਰਨ sfd = openat (procfd, ਸਰੋਤ_ਬਫਰ, O_ਰਸਤਾ | O_CLOEXEC);
              ਜੇ (UNLIKELY (sfd < 0))
                ਵਾਪਸ crun_make_error (err, errno, "openat procfd for `%s`", rel_ਰਸਤਾ);

              ret = fstatfs (sfd, &sfs);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ crun_make_error (err, errno, "fstatfs `%s`", rel_ਰਸਤਾ);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, ਸਰੋਤ_ਬਫਰ, true, true, false, MS_PRIVATE, err);
          ਜੇ (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_ਰਸਤਾ, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;

              ret = fs_move_mount_ਮੰਜ਼ਿਲ (mountfd, pathfd, ਨਿਰਾ);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ crun_make_error (err, errno, "move mount for readonly path `%s`", rel_ਰਸਤਾ);
            }
          ਨਹੀਂਤਾਂ
            {
              proc_fd_ਰਸਤਾ_t abs_ਸਰੋਤ;

              crun_error_release (err);
              get_proc_self_fd_ਰਸਤਾ (abs_ਸਰੋਤ, pathfd);
              ret = do_mount (container, abs_ਸਰੋਤ, pathfd, rel_ਰਸਤਾ, ਨਿਰਾ, mount_flags, ਨਿਰਾ,
                              LABEL_NONE, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;
            }
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ret = get_file_type_fd (pathfd, &ਬਹੁਲਕ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "cannot stat `%s`", rel_ਰਸਤਾ);

      ਜੇ ((ਬਹੁਲਕ & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_ਰਸਤਾ, err);
      ਨਹੀਂਤਾਂ
        {
          cleanup_close ਪੂਰਨ nullfd = -1;
          cleanup_close ਪੂਰਨ mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_ਰਸਤਾ | O_CLOEXEC);
          ਜੇ (UNLIKELY (nullfd < 0))
            ਵਾਪਸ crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          ਜੇ (mountfd >= 0)
            ret = fs_move_mount_ਮੰਜ਼ਿਲ (mountfd, pathfd, ਨਿਰਾ);

          ਜੇ (mountfd < 0 || ret < 0)
            {
              proc_fd_ਰਸਤਾ_t null_ਰਸਤਾ;

              crun_error_release (err);
              get_proc_self_fd_ਰਸਤਾ (null_ਰਸਤਾ, nullfd);
              ret = do_mount (container, null_ਰਸਤਾ, pathfd, rel_ਰਸਤਾ, ਨਿਰਾ, MS_BIND | MS_RDONLY, ਨਿਰਾ,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *context_type;

  context_type = ਲੱਭੋ_annotation (container, "run.oci.mount_context_type");
  ਜੇ (context_type)
    {
      ਜੇ (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          ਵਾਪਸ ਨਿਰਾ;
        }
      ਵਾਪਸ context_type;
    }

  ਵਾਪਸ "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
ਸਥਿਰ ਪੂਰਨ
diagnose_mount_failure (ਪੂਰਨ ret, libcrun_error_t *err, libcrun_container_t *container,
                        ਅਚਲ ਅੱਖਰ *fstype)
{
  ਜੇ (fstype && strcmp (fstype, "cgroup2") == 0 && get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unified_cgroup_ਰਸਤਾ)
    {
      ਜੇ (access (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unified_cgroup_ਰਸਤਾ, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          ਵਾਪਸ ret;
        }
    }
  ਵਾਪਸ ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
ਸਥਿਰ ਪੂਰਨ
sysfs_userns_fallback (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ,
                       ਅਚਲ ਅੱਖਰ *real_ਨਿਸ਼ਾਨਾ, ਪੂਰਨ targetfd,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mountflags, ਅਚਲ ਅੱਖਰ *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close ਪੂਰਨ mountfd = -1;
  cleanup_close ਪੂਰਨ sysfd = -1;
  ਪੂਰਨ sys_old_ਜੜ੍ਹ_fd = -1;
  ਪੂਰਨ ret;

  ਜੇ (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    ਵਾਪਸ 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_ਵਿੱਚ_user_namespace (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ret == 0)
    ਵਾਪਸ 1;

  cgroup_mount = ਲੱਭੋ_mount_for (container, "/sys/fs/cgroup");
  sys_old_ਜੜ੍ਹ_fd = get_old_ਜੜ੍ਹ_fd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  ਜੇ (sys_old_ਜੜ੍ਹ_fd >= 0)
    mountfd = get_bind_mount (sys_old_ਜੜ੍ਹ_fd, "sys", true, true, false, MS_PRIVATE, err);
  ਜੇ (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  ਜੇ (cgroup_mount == ਨਿਰਾ)
    {
      ਜੇ (mountfd >= 0)
        {
          ret = fs_move_mount_ਮੰਜ਼ਿਲ (mountfd, targetfd, ਨਿਰਾ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "move mount `/sys` to `%s`", real_ਨਿਸ਼ਾਨਾ);
        }
      ਨਹੀਂਤਾਂ
        {
          crun_error_release (err);
          ਜੇ (sys_old_ਜੜ੍ਹ_fd >= 0)
            {
              cleanup_free ਅੱਖਰ *sys_ਰਸਤਾ = ਨਿਰਾ;
              proc_fd_ਰਸਤਾ_t sys_ਜੜ੍ਹ_ਰਸਤਾ;

              get_proc_self_fd_ਰਸਤਾ (sys_ਜੜ੍ਹ_ਰਸਤਾ, sys_old_ਜੜ੍ਹ_fd);
              xasprintf (&sys_ਰਸਤਾ, "%s/sys", sys_ਜੜ੍ਹ_ਰਸਤਾ);
              ret = mount (sys_ਰਸਤਾ, real_ਨਿਸ਼ਾਨਾ, ਨਿਰਾ, MS_BIND | MS_REC, ਨਿਰਾ);
            }
          ਨਹੀਂਤਾਂ
            ret = mount ("/sys", real_ਨਿਸ਼ਾਨਾ, ਨਿਰਾ, MS_BIND | MS_REC, ਨਿਰਾ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_ਨਿਸ਼ਾਨਾ (container, ਨਿਸ਼ਾਨਾ, err);
      ਜੇ (UNLIKELY (sysfd < 0))
        ਵਾਪਸ sysfd;

      ret = do_remount (sysfd, ਨਿਸ਼ਾਨਾ,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        ਨਿਰਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਵਾਪਸ do_masked_or_readonly_ਰਸਤਾ (container, "/sys/fs/cgroup", true, false, err);
    }

  ਜੇ (UNLIKELY (mountfd < 0))
    ਵਾਪਸ mountfd;

  ret = fs_move_mount_ਮੰਜ਼ਿਲ (mountfd, targetfd, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "move mount to `%s`", real_ਨਿਸ਼ਾਨਾ);

  sysfd = open_mount_ਨਿਸ਼ਾਨਾ (container, ਨਿਸ਼ਾਨਾ, err);
  ਜੇ (UNLIKELY (sysfd < 0))
    ਵਾਪਸ sysfd;

  ret = do_remount (sysfd, ਨਿਸ਼ਾਨਾ,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_ਰਸਤਾ (container, "/sys/fs/cgroup", true, false, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
ਸਥਿਰ ਖਾਲੀ
apply_selinux_xattr (libcrun_container_t *container, ਪੂਰਨ fd, ਪੂਰਨ extra_flags, ਅਚਲ ਅੱਖਰ *label)
{
  libcrun_error_t tmp_err = ਨਿਰਾ;
  ਪੂਰਨ procfd;

  ਜੇ ((extra_flags & LABEL_ਨਕਾਬ) != LABEL_XATTR)
    ਵਾਪਸ;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container), &tmp_err);
  ਜੇ (procfd >= 0)
    {
      proc_fd_ਰਸਤਾ_t proc_self_ਰਸਤਾ;
      cleanup_close ਪੂਰਨ xfd = -1;

      get_self_fd_ਰਸਤਾ (proc_self_ਰਸਤਾ, fd);
      xfd = openat (procfd, proc_self_ਰਸਤਾ, O_RDONLY | O_CLOEXEC);
      ਜੇ (xfd >= 0)
        (ਖਾਲੀ) fsetxattr (xfd, "security.selinux", label, ਲੜੀ_ਲੰਬਾਈ (label), 0);
    }
  crun_error_release (&tmp_err);
}
#ਜੇ_ਅੰਤ

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
ਸਥਿਰ ਪੂਰਨ
do_mount_replace_rootfs (libcrun_container_t *container, ਪੂਰਨ *fd, libcrun_error_t *err)
{
  ਪੂਰਨ procfd = get_procfd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container), err);
  ਪੂਰਨ tmp;
  ਪੂਰਨ ret;

  ਜੇ (UNLIKELY (procfd < 0))
    ਵਾਪਸ procfd;

  {
    cleanup_close ਪੂਰਨ nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    ਜੇ (UNLIKELY (nsfd < 0))
      ਵਾਪਸ crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    ਜੇ (UNLIKELY (ret < 0))
      ਵਾਪਸ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs, O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (*fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  ਜੇ (UNLIKELY (tmp < 0))
    ਵਾਪਸ crun_make_error (err, errno, "dup");

  ਆਰਜ਼ੀ_FAILURE_RETRY (close (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd));
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd = tmp;

  ਵਾਪਸ 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
ਸਥਿਰ ਪੂਰਨ
apply_ਪ੍ਰਸਾਰ_flags (ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ, ਪੂਰਨ targetfd, ਅਚਲ ਅੱਖਰ *real_ਨਿਸ਼ਾਨਾ,
                         ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mountflags, libcrun_error_t *err)
{
  bool ਪ੍ਰਸਾਰ_ਪੂਰਾ = false;
  ਪੂਰਨ ret;

  ਜੇ (targetfd >= 0)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, ਨਿਸ਼ਾਨਾ, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      ਜੇ (LIKELY (ret == 0))
        ਪ੍ਰਸਾਰ_ਪੂਰਾ = true;
      ਨਹੀਂਤਾਂ
        crun_error_release (&tmp_err);
    }

  ਜੇ (! ਪ੍ਰਸਾਰ_ਪੂਰਾ)
    {
      ret = mount (ਨਿਰਾ, real_ਨਿਸ਼ਾਨਾ, ਨਿਰਾ, mountflags & ALL_PROPAGATIONS, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "set propagation for `%s`", ਨਿਸ਼ਾਨਾ);
    }

  ਵਾਪਸ 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
ਸਥਿਰ ਪੂਰਨ
ਸਮਾਂ_ਸਾਰਣੀ_or_do_remount (libcrun_container_t *container, ਪੂਰਨ *fd, ਪੂਰਨ targetfd,
                        ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ, ਅਚਲ ਅੱਖਰ *real_ਨਿਸ਼ਾਨਾ,
                        bool single_instance, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mountflags,
                        ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਪੂਰਨ extra_flags, libcrun_error_t *err)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  ਬਣਤਰ remount_s *r;

  ਜੇ ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    ਵਾਪਸ do_remount (*fd >= 0 ? *fd : targetfd, real_ਨਿਸ਼ਾਨਾ, remount_flags, ਅੰਕੜੇ, err);

  ਜੇ (*fd < 0)
    {
      *fd = dup (targetfd);
      ਜੇ (UNLIKELY (*fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), ਨਿਸ਼ਾਨਾ, remount_flags, ਅੰਕੜੇ, get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->remounts);
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->remounts = r;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
do_mount (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਸਰੋਤ, ਪੂਰਨ targetfd,
          ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ, ਅਚਲ ਅੱਖਰ *fstype, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mountflags, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ,
          ਪੂਰਨ extra_flags, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਅੰਕੜੇ_with_label = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ_without_label = ਅੰਕੜੇ;
  proc_fd_ਰਸਤਾ_t ਨਿਸ਼ਾਨਾ_ਬਫਰ;
  ਅਚਲ ਅੱਖਰ *context_type = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ ms_move_fd = -1;
  ਅਚਲ ਅੱਖਰ *real_ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close ਪੂਰਨ fd = -1;
  ਅਚਲ ਅੱਖਰ *label = ਨਿਰਾ;
  ਪੂਰਨ ret = 0;

  ਜੇ (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  ਨਹੀਂਤਾਂ
    extra_flags = (extra_flags & ~LABEL_ਨਕਾਬ) | LABEL_NONE;

  ਜੇ (targetfd >= 0)
    {
      get_proc_self_fd_ਰਸਤਾ (ਨਿਸ਼ਾਨਾ_ਬਫਰ, targetfd);
      real_ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ_ਬਫਰ;
      needs_remount = true;
    }

  ਜੇ ((extra_flags & LABEL_ਨਕਾਬ) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      ਜੇ (UNLIKELY (context_type == ਨਿਰਾ))
        ਵਾਪਸ -1;

      ret = ਜੋੜੋ_selinux_mount_label (&ਅੰਕੜੇ_with_label, ਅੰਕੜੇ, label, context_type, err);
      ਜੇ (ret < 0)
        ਵਾਪਸ ret;
      ਅੰਕੜੇ = ਅੰਕੜੇ_with_label;
    }

  ਜੇ (mountflags & MS_MOVE)
    {
      ਜੇ ((mountflags & MS_BIND) || fstype)
        ਵਾਪਸ crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (ਸਰੋਤ, real_ਨਿਸ਼ਾਨਾ, ਨਿਰਾ, MS_MOVE, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "move mount `%s` to `%s`", ਸਰੋਤ, ਨਿਸ਼ਾਨਾ);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_ਨਿਸ਼ਾਨਾ (container, ਨਿਸ਼ਾਨਾ, err);
      ਜੇ (UNLIKELY (ms_move_fd < 0))
        ਵਾਪਸ ms_move_fd;
      targetfd = ms_move_fd;
    }

  ਜੇ ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      ਜੇ (targetfd >= 0)
        {
          ਜੇ (mountflags & MS_BIND)
            {
              cleanup_close ਪੂਰਨ bindfd = -1;
              libcrun_error_t tmp_err = ਨਿਰਾ;

              bindfd = get_bind_mount (AT_FDCWD, ਸਰੋਤ,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              ਜੇ (bindfd < 0)
                {
                  ਪੂਰਨ old_ਜੜ੍ਹ_fd = get_old_ਜੜ੍ਹ_fd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container));

                  ਜੇ (old_ਜੜ੍ਹ_fd >= 0 && ਸਰੋਤ && ਸਰੋਤ[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_ਜੜ੍ਹ_fd, (ਸਰੋਤ[1] == '\0') ? "." : ਸਰੋਤ + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              ਜੇ (bindfd >= 0)
                ret = fs_move_mount_ਮੰਜ਼ਿਲ (bindfd, targetfd, ਨਿਰਾ);
              ਜੇ (ret < 0)
                crun_error_release (&tmp_err);
            }
          ਨਹੀਂਤਾਂ
            {
              cleanup_close ਪੂਰਨ newfs = fsopen_mount (fstype, ਸਰੋਤ, context_type, label, ਅੰਕੜੇ_without_label);
              ਜੇ (newfs >= 0)
                ret = fs_move_mount_ਮੰਜ਼ਿਲ (newfs, targetfd, ਨਿਰਾ);
            }
        }

      ਜੇ (ret < 0)
        {
          ret = mount (ਸਰੋਤ, real_ਨਿਸ਼ਾਨਾ, fstype, flags, ਅੰਕੜੇ);
          ਜੇ (ret < 0 && (mountflags & MS_BIND) && ਸਰੋਤ && ਸਰੋਤ[0] == '/')
            {
              ਪੂਰਨ old_ਜੜ੍ਹ_fd = get_old_ਜੜ੍ਹ_fd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container));

              ਜੇ (old_ਜੜ੍ਹ_fd >= 0)
                {
                  cleanup_free ਅੱਖਰ *proc_ਸਰੋਤ_buf = ਨਿਰਾ;
                  proc_fd_ਰਸਤਾ_t old_ਜੜ੍ਹ_ਰਸਤਾ;

                  get_proc_self_fd_ਰਸਤਾ (old_ਜੜ੍ਹ_ਰਸਤਾ, old_ਜੜ੍ਹ_fd);
                  xasprintf (&proc_ਸਰੋਤ_buf, "%s/%s", old_ਜੜ੍ਹ_ਰਸਤਾ, ਸਰੋਤ + 1);
                  ret = mount (proc_ਸਰੋਤ_buf, real_ਨਿਸ਼ਾਨਾ, fstype, flags, ਅੰਕੜੇ);
                }
            }
        }
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਪੂਰਨ saved_errno = errno;

          ret = sysfs_userns_fallback (container, ਨਿਸ਼ਾਨਾ, real_ਨਿਸ਼ਾਨਾ, targetfd,
                                       mountflags, fstype, err);
          ਜੇ (ret == 0)
            ਵਾਪਸ 0;
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", ਸਰੋਤ, ਨਿਸ਼ਾਨਾ);

          ਵਾਪਸ diagnose_mount_failure (ret, err, container, fstype);
        }

      ਜੇ (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_ਨਿਸ਼ਾਨਾ (container, ਨਿਸ਼ਾਨਾ, err);
          ਜੇ (UNLIKELY (fd < 0))
            ਵਾਪਸ fd;

          /* We are replacing the rootfs, reopen it.  */
          ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਨਿਸ਼ਾਨਾ))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;
            }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FGETXATTR
          apply_selinux_xattr (container, fd, extra_flags, label);
#ਜੇ_ਅੰਤ

          targetfd = fd;

          /* The bind mount shadowed the original targetfd, so refresh
             real_target to point to the reopened mountpoint.  Otherwise the
             classic mount(2) fallbacks below (used when mount_setattr is not
             available, e.g. kernels < 5.12) operate on the shadowed path and
             fail with EINVAL.  */
          get_proc_self_fd_ਰਸਤਾ (ਨਿਸ਼ਾਨਾ_ਬਫਰ, targetfd);
          real_ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ_ਬਫਰ;
        }
    }

  ਜੇ (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_ਪ੍ਰਸਾਰ_flags (ਨਿਸ਼ਾਨਾ, targetfd, real_ਨਿਸ਼ਾਨਾ, mountflags, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  ਜੇ (ਅੰਕੜੇ && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  ਜੇ (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = ਸਮਾਂ_ਸਾਰਣੀ_or_do_remount (container, &fd, targetfd, ਨਿਸ਼ਾਨਾ, real_ਨਿਸ਼ਾਨਾ,
                                    single_instance, mountflags, ਅੰਕੜੇ, extra_flags, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ
try_umount (ਪੂਰਨ targetfd, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ)
{
  proc_fd_ਰਸਤਾ_t ਨਿਸ਼ਾਨਾ_ਬਫਰ;
  ਅਚਲ ਅੱਖਰ *real_ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ;

  ਜੇ (targetfd >= 0)
    {
      get_proc_self_fd_ਰਸਤਾ (ਨਿਸ਼ਾਨਾ_ਬਫਰ, targetfd);
      real_ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ_ਬਫਰ;
    }

  umount2 (real_ਨਿਸ਼ਾਨਾ, MNT_DETACH);
}

ਸਥਿਰ bool
container_has_cgroupns (libcrun_container_t *container)
{
  ਵਾਪਸ get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWCGROUP;
}

ਸਥਿਰ ਪੂਰਨ
do_mount_cgroup_v2 (libcrun_container_t *container, ਪੂਰਨ targetfd, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ,
                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mountflags, ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ret = do_mount (container, "cgroup2", targetfd, ਨਿਸ਼ਾਨਾ, "cgroup2", mountflags, ਅੰਕੜੇ, LABEL_NONE, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਪੂਰਨ saved_errno = crun_error_get_errno (err);
      ਜੇ (saved_errno == EPERM || saved_errno == EBUSY)
        {
          ਅਚਲ ਅੱਖਰ *unified_cgroup_ਰਸਤਾ;
          ਅਚਲ ਅੱਖਰ *src_cgroup;

          crun_error_release (err);

          ਜੇ (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, ਨਿਸ਼ਾਨਾ, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              ਜੇ (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, ਨਿਸ਼ਾਨਾ, "cgroup2", mountflags, ਅੰਕੜੇ, LABEL_NONE, err);
                  ਜੇ (LIKELY (ret == 0))
                    ਵਾਪਸ ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, ਨਿਸ਼ਾਨਾ);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_ਰਸਤਾ = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unified_cgroup_ਰਸਤਾ;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_ਰਸਤਾ && container_has_cgroupns (container) ? unified_cgroup_ਰਸਤਾ : CGROUP_ਜੜ੍ਹ;
          ਵਾਪਸ do_mount (container, src_cgroup, targetfd, ਨਿਸ਼ਾਨਾ, ਨਿਰਾ,
                           MS_BIND | mountflags, ਨਿਰਾ, LABEL_NONE, err);
        }

      ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਸਰੋਤ, ਪੂਰਨ targetfd, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mountflags, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_close ਪੂਰਨ fd = -1;
  ਅਚਲ ਅੱਖਰ *subsystem = "systemd";
  cleanup_free ਅੱਖਰ *subsystem_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, ਸਰੋਤ, targetfd, ਨਿਸ਼ਾਨਾ, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_ਨਿਸ਼ਾਨਾ (container, ਨਿਸ਼ਾਨਾ, err);
  ਜੇ (UNLIKELY (tmpfsdirfd < 0))
    ਵਾਪਸ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_ਰਸਤਾ, err, ਨਿਸ਼ਾਨਾ, subsystem, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_ਰਸਤਾ | O_DIRECTORY | O_NOFOLLOW);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "openat `%s`", subsystem_ਰਸਤਾ);

  ਵਾਪਸ do_mount (container, "cgroup", fd, subsystem_ਰਸਤਾ, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

ਸਥਿਰ ਪੂਰਨ
do_mount_cgroup_v1 (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਸਰੋਤ, ਪੂਰਨ targetfd, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ,
                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mountflags, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  ਅੱਖਰ *from;
  cleanup_close ਪੂਰਨ tmpfsdirfd = -1;
  ਅੱਖਰ *saveptr = ਨਿਰਾ;

  ret = do_mount (container, ਸਰੋਤ, targetfd, ਨਿਸ਼ਾਨਾ, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  tmpfsdirfd = open_mount_ਨਿਸ਼ਾਨਾ (container, ਨਿਸ਼ਾਨਾ, err);
  ਜੇ (UNLIKELY (tmpfsdirfd < 0))
    ਵਾਪਸ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    ਪੂਰਨ procfd = get_procfd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container), err);
    ਜੇ (UNLIKELY (procfd < 0))
      ਵਾਪਸ procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, ਨਿਰਾ, err);
  }
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (UNLIKELY (content == ਨਿਰਾ || content[0] == '\0'))
    ਵਾਪਸ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ਲਈ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ਨਿਰਾ, "\n", &saveptr))
    {
      cleanup_free ਅੱਖਰ *ਸਰੋਤ_ਰਸਤਾ = ਨਿਰਾ;
      cleanup_free ਅੱਖਰ *ਸਰੋਤ_subsystem = ਨਿਰਾ;
      cleanup_free ਅੱਖਰ *subsystem_ਰਸਤਾ = ਨਿਰਾ;
      ਅੱਖਰ *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close ਪੂਰਨ subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      ਜੇ (subsystem[0] == '\0')
        ਜਾਰੀ;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = ਲੜੀ_ਖੋਜ (subsystem, "name=");
      ਜੇ (it)
        subsystem = it + 5;

      ਜੇ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ਜੇ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&ਸਰੋਤ_subsystem, err, CGROUP_ਜੜ੍ਹ, subsystem, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      /* if there is already a mount specified, do not add a default one.  */
      ਜੇ (ਲੱਭੋ_mount_for (container, ਸਰੋਤ_subsystem) != ਨਿਰਾ)
        ਜਾਰੀ;

      ret = append_paths (&ਸਰੋਤ_ਰਸਤਾ, err, ਸਰੋਤ_subsystem, subpath, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = append_paths (&subsystem_ਰਸਤਾ, err, ਨਿਸ਼ਾਨਾ, subsystem, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "mkdirat `%s`", subsystem_ਰਸਤਾ);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_ਰਸਤਾ | O_DIRECTORY | O_NOFOLLOW);
      ਜੇ (UNLIKELY (subsystemfd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", subsystem_ਰਸਤਾ);

      ਜੇ (container_has_cgroupns (container))
        {
          ret = do_mount (container, ਸਰੋਤ_ਰਸਤਾ, subsystemfd, subsystem_ਰਸਤਾ, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ਜੇ (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  ਜਾਰੀ;
                }
              ਵਾਪਸ ret;
            }
        }
      ਨਹੀਂਤਾਂ
        {
          ret = do_mount (container, ਸਰੋਤ_ਰਸਤਾ, subsystemfd, subsystem_ਰਸਤਾ, ਨਿਰਾ, MS_BIND | mountflags, ਨਿਰਾ,
                          LABEL_NONE, err);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ਜੇ (crun_error_get_errno (err) != ENOENT)
                ਵਾਪਸ ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, ਸਰੋਤ_subsystem, subsystemfd, subsystem_ਰਸਤਾ, ਨਿਰਾ, MS_BIND | mountflags,
                              ਨਿਰਾ, LABEL_NONE, err);
              ਜੇ (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  ਜੇ (crun_error_get_errno (err) != ENOENT)
                    ਵਾਪਸ ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
do_mount_cgroup (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਸਰੋਤ, ਪੂਰਨ targetfd, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mountflags, ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ, libcrun_error_t *err)
{
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਚੋਣ (cgroup_ਬਹੁਲਕ)
    {
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_UNIFIED:
      ਵਾਪਸ do_mount_cgroup_v2 (container, targetfd, ਨਿਸ਼ਾਨਾ, mountflags, ਅੰਕੜੇ, err);
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_LEGACY:
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_HYBRID:
      ਵਾਪਸ do_mount_cgroup_v1 (container, ਸਰੋਤ, targetfd, ਨਿਸ਼ਾਨਾ, mountflags, err);
    }

  ਵਾਪਸ crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_ਬਹੁਲਕ);
}

ਬਣਤਰ device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
ਸਥਿਰ ਖਾਲੀ
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  ਬਣਤਰ ਨਿੱਜੀ_ਅੰਕੜੇ_s *ਨਿੱਜੀ_ਅੰਕੜੇ = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container);
  size_t i;

  ਜੇ (ਨਿੱਜੀ_ਅੰਕੜੇ->needed_devs_fds)
    {
      ਬਣਤਰ libcrun_fd_map *dev_fds = ਨਿੱਜੀ_ਅੰਕੜੇ->needed_devs_fds;

      ਲਈ (i = 0; needed_devs[i].ਰਸਤਾ; i++)
        {
          ਜੇ (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            ਜਾਰੀ;

          ਪੂਰਨ fd = syscall_open_ਰੁੱਖ (AT_FDCWD, needed_devs[i].ਰਸਤਾ,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          ਜੇ (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          ਨਹੀਂਤਾਂ ਜੇ (fd >= 0)
            close (fd);
        }
    }

  ਜੇ (ਨਿੱਜੀ_ਅੰਕੜੇ->notify_socket_ਰੁੱਖ_fd < 0
      && ਨਿੱਜੀ_ਅੰਕੜੇ->host_notify_socket_ਰਸਤਾ)
    {
      ਪੂਰਨ fd = syscall_open_ਰੁੱਖ (AT_FDCWD, ਨਿੱਜੀ_ਅੰਕੜੇ->host_notify_socket_ਰਸਤਾ,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      ਜੇ (fd >= 0)
        ਨਿੱਜੀ_ਅੰਕੜੇ->notify_socket_ਰੁੱਖ_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
relative_ਰਸਤਾ_under_dev (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
  ਜੇ (ਰਸਤਾ[0] != '/')
    ਵਾਪਸ ਰਸਤਾ;
  ਜੇ (ਲੜੀ_ਤੁਲਨਾ (ਰਸਤਾ, "/dev/", 5) == 0)
    {
      ਜੇ (strchr (ਰਸਤਾ + 5, '/'))
        ਵਾਪਸ ਨਿਰਾ;
      ਵਾਪਸ ਰਸਤਾ + 5;
    }
  ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ
mknod_and_ਸੈੱਟ_attrs (libcrun_container_t *container, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *name, ਬਣਤਰ device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ fd = -1;
  proc_fd_ਰਸਤਾ_t fd_ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *what = "new";
  bool exists = false;
  ਬਣਤਰ stat st;
  ਪੂਰਨ procfd;
  ਪੂਰਨ ret;

  ret = mknodat (dirfd, name, device->ਬਹੁਲਕ | type, dev);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno != EEXIST)
        ਵਾਪਸ crun_make_error (err, errno, "mknodat `%s`", device->ਰਸਤਾ);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_ਰਸਤਾ | O_NOFOLLOW | O_CLOEXEC);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open %s device `%s`", what, device->ਰਸਤਾ);

  ret = fstat (fd, &st);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fstat %s device `%s`", what, device->ਰਸਤਾ);

  ਜੇ (UNLIKELY ((st.st_mode & S_IFMT) != type))
    ਵਾਪਸ crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->ਰਸਤਾ, st.st_mode & S_IFMT, type);

  ਜੇ (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    ਵਾਪਸ crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->ਰਸਤਾ, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  ਜੇ (exists)
    ਵਾਪਸ 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container), err);
  ਜੇ (UNLIKELY (procfd < 0))
    ਵਾਪਸ procfd;

  get_self_fd_ਰਸਤਾ (fd_ਰਸਤਾ, fd);
  ret = fchmodat (procfd, fd_ਰਸਤਾ, device->ਬਹੁਲਕ, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fchmod `%s`", device->ਰਸਤਾ);

  ret = fchownat (fd, "", device->uid, device->gid, AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fchown `%s`", device->ਰਸਤਾ);

  ਵਾਪਸ 1;
}

ਪੂਰਨ
libcrun_create_dev (libcrun_container_t *container, ਪੂਰਨ devfd, ਪੂਰਨ srcfd,
                    ਬਣਤਰ device_s *device, bool binds, bool ensure_ਮਾਪਾ_dir,
                    libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  ਅਚਲ ਅੱਖਰ *fullname = device->ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *rootfs = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs;
  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (fullname))
    ਵਾਪਸ crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free ਅੱਖਰ *normalized_ਰਸਤਾ = xstrdup (fullname);
  consume_trailing_slashes (normalized_ਰਸਤਾ);
  ਜੇ (normalized_ਰਸਤਾ[0] == '\0')
    strcpy (normalized_ਰਸਤਾ, "/");
  ਅਚਲ ਅੱਖਰ *rel_dev = relative_ਰਸਤਾ_under_dev (normalized_ਰਸਤਾ);

  ਜੇ (binds)
    {
      cleanup_close ਪੂਰਨ fd = -1;

      ਜੇ (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_ਰਸਤਾ | O_NONBLOCK);
          ਜੇ (UNLIKELY (fd < 0))
            {
              ਜੇ (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              ਜੇ (UNLIKELY (fd < 0))
                ਵਾਪਸ crun_make_error (err, errno, "openat `%s`", device->ਰਸਤਾ);
            }
        }
      ਨਹੀਂਤਾਂ
        {
          ਅਚਲ ਅੱਖਰ *rel_ਰਸਤਾ = consume_slashes (normalized_ਰਸਤਾ);

          fd = crun_ਸੁਰੱਖਿਅਤ_create_and_open_ref_at (false, get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, rel_ਰਸਤਾ, 0755, err);
          ਜੇ (UNLIKELY (fd < 0))
            ਵਾਪਸ fd;
        }

      ਜੇ (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_ਰਸਤਾ, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          ਜੇ (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              ਜੇ (LIKELY (ret >= 0))
                ਵਾਪਸ 0;
            }
          ਨਹੀਂਤਾਂ
            crun_error_release (err);
        }

      {
        cleanup_close ਪੂਰਨ mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        ਜੇ (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_ਰਸਤਾ, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            ਜੇ (LIKELY (ret == 0))
              {
                ret = fs_move_mount_ਮੰਜ਼ਿਲ (mountfd, fd, ਨਿਰਾ);
                ਜੇ (LIKELY (ret == 0))
                  ਵਾਪਸ 0;
              }
            ਨਹੀਂਤਾਂ
              crun_error_release (err);
          }
        ਨਹੀਂਤਾਂ
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_ਰਸਤਾ, ਨਿਰਾ, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, ਨਿਰਾ, LABEL_MOUNT, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      ਜੇ (rel_dev)
        {
          ret = mknod_and_ਸੈੱਟ_attrs (container, devfd, rel_dev, device, type, dev, err);
          ਜੇ (ret <= 0)
            ਵਾਪਸ ret;
        }
      ਨਹੀਂਤਾਂ
        {
          cleanup_close ਪੂਰਨ dirfd = -1;
          cleanup_free ਅੱਖਰ *dirname = ਨਿਰਾ;
          ਅੱਖਰ *basename, *ਮਿਲਿਆ;

          dirname = xstrdup (normalized_ਰਸਤਾ);

          ਮਿਲਿਆ = strrchr (dirname, '/');
          ਜੇ (ਮਿਲਿਆ)
            *ਮਿਲਿਆ = '\0';

          basename = ਮਿਲਿਆ ? ਮਿਲਿਆ + 1 : dirname;

          ਜੇ (dirname[0] == '\0')
            {
              dirfd = dup (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd);
              ਜੇ (UNLIKELY (dirfd < 0))
                ਵਾਪਸ crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          ਨਹੀਂਤਾਂ
            {
              dirfd = ਸੁਰੱਖਿਅਤ_openat (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC, 0, err);
              ਜੇ (dirfd < 0 && ensure_ਮਾਪਾ_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_ਸੁਰੱਖਿਅਤ_create_and_open_ref_at (true, get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              ਜੇ (UNLIKELY (dirfd < 0))
                ਵਾਪਸ dirfd;
            }

          ret = mknod_and_ਸੈੱਟ_attrs (container, dirfd, basename, device, type, dev, err);
          ਜੇ (ret <= 0)
            ਵਾਪਸ ret;
        }
    }
  ਵਾਪਸ 0;
}

ਬਣਤਰ symlink_s
{
  ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ;
  bool force;
};

ਸਥਿਰ ਬਣਤਰ symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { ਨਿਰਾ, ਨਿਰਾ, false } };

ਸਥਿਰ ਪੂਰਨ
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  size_t i;
  ਬਣਤਰ device_s *it;
  cleanup_close ਪੂਰਨ devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  ਅਚਲ ਅੱਖਰ *rootfs = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs;
  cleanup_close_map ਬਣਤਰ libcrun_fd_map *dev_fds = ਨਿਰਾ;
  cleanup_close_map ਬਣਤਰ libcrun_fd_map *needed_fds = ਨਿਰਾ;

  dev_fds = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->dev_fds;
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->dev_fds = ਨਿਰਾ;

  needed_fds = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->needed_devs_fds;
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->needed_devs_fds = ਨਿਰਾ;

  ਜੇ (! def || ! def->linux)
    ਵਾਪਸ 0;

  devfd = ਸੁਰੱਖਿਅਤ_openat (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_ਰਸਤਾ | O_DIRECTORY, 0, err);
  ਜੇ (UNLIKELY (devfd < 0))
    ਵਾਪਸ devfd;

  ਲਈ (i = 0; i < def->linux->devices_len; i++)
    {
      ਬਣਤਰ device_s device = {
        def->linux->devices[i]->ਰਸਤਾ,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_ਬਹੁਲਕ,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      ਜੇ (! def->linux->devices[i]->file_ਬਹੁਲਕ_present)
        device.ਬਹੁਲਕ = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਲਈ (it = needed_devs, i = 0; it->ਰਸਤਾ; it++, i++)
    {
      cleanup_close ਪੂਰਨ srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      ਜੇ (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  ਲਈ (i = 0; symlinks[i].ਨਿਸ਼ਾਨਾ; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].ਰਸਤਾ, devfd, symlinks[i].ਨਿਸ਼ਾਨਾ);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਪੂਰਨ saved_errno = errno;

          ਜੇ (errno == EEXIST && ! symlinks[i].force)
            ਜਾਰੀ;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          ਜੇ (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].ਨਿਸ਼ਾਨਾ, 0);
              ਜੇ (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].ਨਿਸ਼ਾਨਾ, AT_REMOVEDIR);
              ਜੇ (ret < 0 && errno == EBUSY)
                {
                  cleanup_close ਪੂਰਨ tfd = openat (devfd, symlinks[i].ਨਿਸ਼ਾਨਾ, O_CLOEXEC | O_ਰਸਤਾ | O_NOFOLLOW);
                  ਜੇ (tfd >= 0)
                    {
                      proc_fd_ਰਸਤਾ_t procpath;

                      get_proc_self_fd_ਰਸਤਾ (procpath, tfd);
                      ਜੇ (umount2 (procpath, MNT_DETACH) == 0)
                        ਜਾਓ retry_unlink;
                    }
                }
              ਜੇ (ret == 0)
                ਜਾਓ retry_symlink;
            }
          ਵਾਪਸ crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].ਨਿਸ਼ਾਨਾ);
        }
    }

  ਜੇ (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
ਅਗਲਾ_ਰਸਤਾ_component (ਅਚਲ ਅੱਖਰ *p, size_t *len)
{
  ਲਈ (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      ਜੇ (*len != 1 || p[0] != '.')
        ਵਾਪਸ p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
ਸਥਿਰ bool
same_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *a, ਅਚਲ ਅੱਖਰ *b)
{
  size_t la, lb;

  ਲਈ (;;)
    {
      a = ਅਗਲਾ_ਰਸਤਾ_component (a, &la);
      b = ਅਗਲਾ_ਰਸਤਾ_component (b, &lb);
      ਜੇ (la != lb || memcmp (a, b, la) != 0)
        ਵਾਪਸ false;
      ਜੇ (la == 0)
        ਵਾਪਸ true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
ਸਥਿਰ bool
is_duplicate_ਰਸਤਾ (ਅੱਖਰ **paths, size_t i)
{
  size_t j;

  ਜੇ (paths[i] == ਨਿਰਾ)
    ਵਾਪਸ false;

  ਲਈ (j = 0; j < i; j++)
    ਜੇ (paths[j] && same_ਰਸਤਾ (paths[j], paths[i]))
      ਵਾਪਸ true;

  ਵਾਪਸ false;
}

ਸਥਿਰ ਪੂਰਨ
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  ਪੂਰਨ ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  ਜੇ (! def || ! def->linux)
    ਵਾਪਸ 0;

  ਲਈ (i = 0; i < def->linux->masked_paths_len; i++)
    {
      ਜੇ (is_duplicate_ਰਸਤਾ (def->linux->masked_paths, i))
        ਜਾਰੀ;

      ret = do_masked_or_readonly_ਰਸਤਾ (container, def->linux->masked_paths[i], false, false, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਲਈ (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_ਰਸਤਾ (container, def->linux->readonly_paths[i], true, true, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
do_ਧੁਰਾ (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rootfs, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ oldrootfd = -1;
  cleanup_close ਪੂਰਨ newrootfd = -1;
  ਪੂਰਨ ret;

  oldrootfd = open ("/", O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (oldrootfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (newrootfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = ਧੁਰਾ_ਜੜ੍ਹ (".", ".");
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, ਨਿਰਾ, -1, ".", ਨਿਰਾ, MS_REC | MS_PRIVATE, ਨਿਰਾ, LABEL_MOUNT, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = umount2 (".", MNT_DETACH);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "umount2 oldroot");

  ਕਰੋ
    {
      ret = umount2 (".", MNT_DETACH);
      ਜੇ (ret < 0 && errno == EINVAL)
        ਰੋਕੋ;
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "umount2 oldroot");
  } ਜਦੋਂ (ret == 0);

  ret = chdir ("/");
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "chdir to newroot");

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
append_tmpfs_ਬਹੁਲਕ_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, ਅੱਖਰ **ਅੰਕੜੇ, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *rootfs = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs;
  bool ਖਾਲੀ_ਥਾਵਾਂ_ਅੰਕੜੇ = is_ਖਾਲੀ_ਥਾਵਾਂ_string (*ਅੰਕੜੇ);
  cleanup_close ਪੂਰਨ fd = -1;
  ਬਣਤਰ stat st;
  ਪੂਰਨ ret;

  ਜੇ (*ਅੰਕੜੇ != ਨਿਰਾ && ਲੜੀ_ਖੋਜ (*ਅੰਕੜੇ, "mode="))
    ਵਾਪਸ 0;

  fd = ਸੁਰੱਖਿਅਤ_openat (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_ਰਸਤਾ, 0, err);
  ਜੇ (fd < 0)
    {
      ਜੇ (crun_error_get_errno (err) != ENOENT)
        ਵਾਪਸ fd;

      crun_error_release (err);
      ਵਾਪਸ 0;
    }
  ret = fstat (fd, &st);
  ਜੇ (ret < 0)
    ਵਾਪਸ crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (ਅੰਕੜੇ, "%s%smode=%o", ਖਾਲੀ_ਥਾਵਾਂ_ਅੰਕੜੇ ? "" : *ਅੰਕੜੇ, ਖਾਲੀ_ਥਾਵਾਂ_ਅੰਕੜੇ ? "" : ",", st.st_mode & 07777);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
get_default_flags (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *destination, ਅੱਖਰ **ਅੰਕੜੇ)
{
  ਜੇ (strcmp (destination, "/proc") == 0)
    ਵਾਪਸ 0;
  ਜੇ (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *ਅੰਕੜੇ = xstrdup ("none,name=");
      ਵਾਪਸ MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  ਜੇ (strcmp (destination, "/dev") == 0)
    {
      *ਅੰਕੜੇ = xstrdup ("mode=755");
      ਵਾਪਸ MS_NOEXEC | MS_STRICTATIME;
    }
  ਜੇ (strcmp (destination, "/dev/shm") == 0)
    {
      *ਅੰਕੜੇ = xstrdup ("mode=1777,size=65536k");
      ਵਾਪਸ MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  ਜੇ (strcmp (destination, "/dev/mqueue") == 0)
    ਵਾਪਸ MS_NOEXEC | MS_NOSUID | MS_NODEV;
  ਜੇ (strcmp (destination, "/dev/pts") == 0)
    {
      ਜੇ (container->host_uid == 0)
        *ਅੰਕੜੇ = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      ਨਹੀਂਤਾਂ
        *ਅੰਕੜੇ = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      ਵਾਪਸ MS_NOEXEC | MS_NOSUID;
    }
  ਜੇ (strcmp (destination, "/sys") == 0)
    ਵਾਪਸ MS_NOEXEC | MS_NOSUID | MS_NODEV;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  ਵਾਪਸ ਲੱਭੋ_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

ਸਥਿਰ ਪੂਰਨ
check_valid_no_follow_file_system (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *destination, libcrun_error_t *err)
{
  ਬਣਤਰ statfs sfs;

  ਪੂਰਨ ret = fstatfs (fd, &sfs);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "statfs `%s`", destination);

  ਚੋਣ (sfs.f_type)
    {
    ਹਾਲਤ PROC_SUPER_MAGIC:
    ਹਾਲਤ SYSFS_MAGIC:
    ਹਾਲਤ DEVPTS_SUPER_MAGIC:
      ਵਾਪਸ crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਸੁਰੱਖਿਅਤ_create_symlink (ਪੂਰਨ rootfsfd, ਅਚਲ ਅੱਖਰ *rootfs, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ, ਅਚਲ ਅੱਖਰ *destination, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ ਮਾਪਾ_dir_fd = -1;
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  ਅੱਖਰ *part;
  ਪੂਰਨ ret;

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (destination))
    ਵਾਪਸ crun_make_error (err, 0, "empty destination for symlink `%s`", ਨਿਸ਼ਾਨਾ);

  ਬਫਰ = xstrdup (destination);
  part = dirname (ਬਫਰ);

  ਮਾਪਾ_dir_fd = crun_ਸੁਰੱਖਿਅਤ_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  ਜੇ (UNLIKELY (ਮਾਪਾ_dir_fd < 0))
    ਵਾਪਸ ਮਾਪਾ_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (ਬਫਰ, destination);
  part = basename (ਬਫਰ);

  ret = symlinkat (ਨਿਸ਼ਾਨਾ, ਮਾਪਾ_dir_fd, part);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      ਜੇ (errno == EEXIST)
        {
          cleanup_free ਅੱਖਰ *link = ਨਿਰਾ;
          ssize_t len;

          len = ਸੁਰੱਖਿਅਤ_readlinkat (ਮਾਪਾ_dir_fd, part, &link, 0, err);
          ਜੇ (UNLIKELY (len < 0))
            ਵਾਪਸ len;

          ਜੇ ((((size_t) len) == ਲੜੀ_ਲੰਬਾਈ (ਨਿਸ਼ਾਨਾ)) && ਲੜੀ_ਤੁਲਨਾ (link, ਨਿਸ਼ਾਨਾ, len) == 0)
            ਵਾਪਸ 0;

          ਵਾਪਸ crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      ਵਾਪਸ crun_make_error (err, errno, "symlinkat `%s`", ਨਿਸ਼ਾਨਾ);
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
handle_copy_symlink (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     ਅਚਲ ਅੱਖਰ *link_ਨਿਸ਼ਾਨਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਨਿਸ਼ਾਨਾ = ਨਿਰਾ;

  ਜੇ (link_ਨਿਸ਼ਾਨਾ == ਨਿਰਾ)
    {
      ਪੂਰਨ old_ਜੜ੍ਹ_fd = get_old_ਜੜ੍ਹ_fd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container));
      ਅਚਲ ਅੱਖਰ *ਸਰੋਤ = mount->ਸਰੋਤ;
      ssize_t len;

      ਜੇ (old_ਜੜ੍ਹ_fd >= 0 && ਸਰੋਤ[0] == '/')
        len = ਸੁਰੱਖਿਅਤ_readlinkat (old_ਜੜ੍ਹ_fd, ਸਰੋਤ + 1, &ਨਿਸ਼ਾਨਾ, 0, err);
      ਨਹੀਂਤਾਂ
        len = ਸੁਰੱਖਿਅਤ_readlinkat (AT_FDCWD, ਸਰੋਤ, &ਨਿਸ਼ਾਨਾ, 0, err);
      ਜੇ (UNLIKELY (len < 0))
        ਵਾਪਸ len;
      link_ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ;
    }

  ਵਾਪਸ ਸੁਰੱਖਿਅਤ_create_symlink (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs,
                              link_ਨਿਸ਼ਾਨਾ, mount->destination, err);
}

ਸਥਿਰ ਪੂਰਨ
prepare_sysfs_or_proc_mount (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ,
                             ਪੂਰਨ *targetfd, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, ਨਿਸ਼ਾਨਾ,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno == ENOENT)
        {
          ਜੇ (strchr (ਨਿਸ਼ਾਨਾ, '/'))
            ਵਾਪਸ crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", ਨਿਸ਼ਾਨਾ);

          ret = mkdirat (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, ਨਿਸ਼ਾਨਾ, 0755);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "mkdirat `%s`", ਨਿਸ਼ਾਨਾ);

          /* Try opening it again.  */
          ret = openat (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, ਨਿਸ਼ਾਨਾ,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      ਨਹੀਂਤਾਂ ਜੇ (errno == ENOTDIR)
        ਵਾਪਸ crun_make_error (err, errno, "the target `/%s` is invalid", ਨਿਸ਼ਾਨਾ);

      ਜੇ (ret < 0)
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਨਿਸ਼ਾਨਾ);
    }

  *targetfd = ret;
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
handle_tmpcopyup (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rootfs, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ,
                  ਪੂਰਨ copy_from_fd, libcrun_error_t *err)
{
  ਪੂਰਨ destfd, ret;
  cleanup_close ਪੂਰਨ tmpfd = copy_from_fd;

  ਨਿਸ਼ਾਨਾ = consume_slashes (ਨਿਸ਼ਾਨਾ);

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਨਿਸ਼ਾਨਾ))
    ਵਾਪਸ crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = ਸੁਰੱਖਿਅਤ_openat (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, ਨਿਸ਼ਾਨਾ,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  ਜੇ (UNLIKELY (destfd < 0))
    ਵਾਪਸ crun_error_wrap (err, "open `%s` to write for tmpcopyup", ਨਿਸ਼ਾਨਾ);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_ਮੰਜ਼ਿਲ_fd (tmpfd, destfd, ਨਿਸ਼ਾਨਾ, ਨਿਸ਼ਾਨਾ, err);
  tmpfd = -1;
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
get_mount_label_how (ਅਚਲ ਅੱਖਰ *type, bool is_sysfs_or_proc)
{
  ਜੇ (is_sysfs_or_proc)
    ਵਾਪਸ LABEL_NONE;

  ਜੇ (strcmp (type, "mqueue") == 0)
    ਵਾਪਸ LABEL_XATTR;

  ਵਾਪਸ LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
get_bind_mount_locked_flags (ਪੂਰਨ ਸਰੋਤ_mountfd, ਅਚਲ ਅੱਖਰ *ਸਰੋਤ, runtime_spec_schema_defs_mount *mount)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cleared = 0;
  ਬਣਤਰ statfs sfs;
  size_t i;
  ਪੂਰਨ ret;

  ਜੇ (ਸਰੋਤ_mountfd >= 0)
    ret = fstatfs (ਸਰੋਤ_mountfd, &sfs);
  ਨਹੀਂਤਾਂ
    ret = statfs (ਸਰੋਤ, &sfs);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ 0;

  ਲਈ (i = 0; i < mount->options_len; i++)
    {
      ਅਚਲ ਬਣਤਰ ਪ੍ਰਸਾਰ_flags_s *prop;

      ਜੇ (mount->options[i] == ਨਿਰਾ)
        ਜਾਰੀ;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ਜੇ (prop == ਨਿਰਾ || (prop->extra_flags & OPTION_RECURSIVE))
        ਜਾਰੀ;

      ਜੇ (prop->clear)
        cleared |= prop->flags;
      ਨਹੀਂਤਾਂ
        cleared &= ~prop->flags;
    }

  ਵਾਪਸ sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
get_ਰਸਤਾ_ਵਿੱਚ_rootfs (ਅਚਲ ਅੱਖਰ *rootfs, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
  size_t len;

  ਜੇ (rootfs == ਨਿਰਾ || ਰਸਤਾ == ਨਿਰਾ)
    ਵਾਪਸ ਨਿਰਾ;

  len = ਲੜੀ_ਲੰਬਾਈ (rootfs);
  ਜਦੋਂ (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  ਜੇ (len == 0)
    ਵਾਪਸ ਨਿਰਾ;

  ਜੇ (ਲੜੀ_ਤੁਲਨਾ (ਰਸਤਾ, rootfs, len) != 0 || (ਰਸਤਾ[len] != '/' && ਰਸਤਾ[len] != '\0'))
    ਵਾਪਸ ਨਿਰਾ;

  ਵਾਪਸ consume_slashes (ਰਸਤਾ + len);
}

ਸਥਿਰ ਪੂਰਨ
process_single_mount (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      ਬਣਤਰ libcrun_fd_map *mount_fds, size_t mount_ਸੂਚਕਾਂਕ,
                      ਅਚਲ ਅੱਖਰ *systemd_cgroup_v1, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ = consume_slashes (mount->destination);
  cleanup_close ਪੂਰਨ ਸਰੋਤ_mountfd = -1;
  cleanup_free ਅੱਖਰ *ਅੰਕੜੇ = ਨਿਰਾ;
  ਅੱਖਰ *type;
  ਅੱਖਰ *ਸਰੋਤ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ extra_flags = 0;
  mode_t src_ਬਹੁਲਕ = S_IFDIR;
  cleanup_close ਪੂਰਨ copy_from_fd = -1;
  cleanup_close ਪੂਰਨ targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_ਸੈੱਟ = 0;
  ਪੂਰਨ no_remount = 0;
  ਪੂਰਨ ret;

  ਜੇ (mount_fds)
    ਸਰੋਤ_mountfd = get_and_reset (&(mount_fds->fds[mount_ਸੂਚਕਾਂਕ]));

  type = mount->type;

  ਜੇ (mount->options == ਨਿਰਾ)
    flags = get_default_flags (container, mount->destination, &ਅੰਕੜੇ);
  ਨਹੀਂਤਾਂ
    {
      size_t j;

      ਲਈ (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &ਅੰਕੜੇ, &rec_clear, &rec_ਸੈੱਟ);
    }

  ਜੇ (type == ਨਿਰਾ && (flags & MS_BIND) == 0)
    ਵਾਪਸ crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  ਜੇ ((flags & MS_BIND) && ਸਰੋਤ_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      ਅਚਲ ਅੱਖਰ *rel_ਸਰੋਤ = get_ਰਸਤਾ_ਵਿੱਚ_rootfs (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->host_rootfs, mount->ਸਰੋਤ);

      ਜੇ (rel_ਸਰੋਤ)
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪ੍ਰਸਾਰ = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          ਸਰੋਤ_mountfd = get_bind_mount (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rel_ਸਰੋਤ, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, ਪ੍ਰਸਾਰ, err);
          ਜੇ (UNLIKELY (ਸਰੋਤ_mountfd < 0))
            ਵਾਪਸ crun_error_wrap (err, "open mount source `%s`", mount->ਸਰੋਤ);
        }
    }

  ਜੇ ((flags & MS_BIND) && mount->ਸਰੋਤ && has_mount_flag_options (mount)
      && (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (ਸਰੋਤ_mountfd, mount->ਸਰੋਤ, mount);

  ਜੇ (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      ਜੇ (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      ਜੇ (ਰਸਤਾ_is_slash_dev (mount->destination))
        get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  ਜੇ (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_ਬਹੁਲਕ_if_missing (container, mount, &ਅੰਕੜੇ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (mount->ਸਰੋਤ && (flags & MS_BIND))
    {
      proc_fd_ਰਸਤਾ_t proc_buf;
      ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = mount->ਸਰੋਤ;

      ਜੇ ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        ਵਾਪਸ crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->copy_symlink_targets
                               && mount_ਸੂਚਕਾਂਕ < get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->n_copy_symlink_targets
                               && get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->copy_symlink_targets[mount_ਸੂਚਕਾਂਕ];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      ਜੇ (ਸਰੋਤ_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_ਰਸਤਾ (proc_buf, ਸਰੋਤ_mountfd);
          ਰਸਤਾ = proc_buf;

          ret = get_file_type_at (ਸਰੋਤ_mountfd, &src_ਬਹੁਲਕ, true, ਨਿਰਾ);
        }
      ਨਹੀਂਤਾਂ ਜੇ (is_cached_symlink)
        {
          src_ਬਹੁਲਕ = S_IFLNK;
          ret = 0;
        }
      ਨਹੀਂਤਾਂ ਜੇ (ਰਸਤਾ[0] == '/')
        {
          ਪੂਰਨ old_ਜੜ੍ਹ_fd = get_old_ਜੜ੍ਹ_fd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          ਜੇ (old_ਜੜ੍ਹ_fd >= 0)
            ret = get_file_type_at (old_ਜੜ੍ਹ_fd, &src_ਬਹੁਲਕ, nofollow, (ਰਸਤਾ[1] == '\0') ? ਨਿਰਾ : ਰਸਤਾ + 1);
          ਨਹੀਂਤਾਂ
            ret = get_file_type_at (AT_FDCWD, &src_ਬਹੁਲਕ, nofollow, ਰਸਤਾ);
        }
      ਨਹੀਂਤਾਂ
        ret = get_file_type (&src_ਬਹੁਲਕ, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, ਰਸਤਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "cannot stat `%s`", ਰਸਤਾ);

      ਜੇ (S_ISLNK (src_ਬਹੁਲਕ) && (extra_flags & OPTION_DEST_NOFOLLOW) && ਸਰੋਤ_mountfd < 0)
        {
          ਪੂਰਨ bind_dirfd = AT_FDCWD;
          ਅਚਲ ਅੱਖਰ *bind_ਸਰੋਤ = mount->ਸਰੋਤ;
          ਪੂਰਨ old_ਜੜ੍ਹ_fd = get_old_ਜੜ੍ਹ_fd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container));

          ਜੇ (old_ਜੜ੍ਹ_fd >= 0 && mount->ਸਰੋਤ && mount->ਸਰੋਤ[0] == '/')
            {
              bind_dirfd = old_ਜੜ੍ਹ_fd;
              bind_ਸਰੋਤ = (mount->ਸਰੋਤ[1] == '\0') ? "." : mount->ਸਰੋਤ + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_ਸਰੋਤ, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          ਸਰੋਤ_mountfd = ret;
        }
    }

  ਜੇ (S_ISLNK (src_ਬਹੁਲਕ) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      ਅਚਲ ਅੱਖਰ *cached_ਨਿਸ਼ਾਨਾ = ਨਿਰਾ;

      ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->copy_symlink_targets
          && mount_ਸੂਚਕਾਂਕ < get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->n_copy_symlink_targets)
        cached_ਨਿਸ਼ਾਨਾ = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->copy_symlink_targets[mount_ਸੂਚਕਾਂਕ];

      ret = handle_copy_symlink (container, rootfs, mount, cached_ਨਿਸ਼ਾਨਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      mounted = true;
    }
  ਨਹੀਂਤਾਂ ਜੇ (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, ਨਿਸ਼ਾਨਾ, &targetfd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      bool is_dir = S_ISDIR (src_ਬਹੁਲਕ);

      ਜੇ (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = ਸੁਰੱਖਿਅਤ_openat (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, ਨਿਸ਼ਾਨਾ, O_ਰਸਤਾ | O_NOFOLLOW | O_CLOEXEC, 0, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, ਨਿਸ਼ਾਨਾ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
      ਨਹੀਂਤਾਂ
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_ਸੁਰੱਖਿਅਤ_create_and_open_ref_at (is_dir, get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, ਨਿਸ਼ਾਨਾ, 0755, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          targetfd = ret;
        }
    }

  ਜੇ (extra_flags & OPTION_TMPCOPYUP)
    {
      ਜੇ (strcmp (type, "tmpfs") != 0)
        ਵਾਪਸ crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      ਜੇ (UNLIKELY (copy_from_fd < 0))
        {
          ਜੇ (errno != ENOTDIR)
            ਵਾਪਸ crun_make_error (err, errno, "cannot reopen `%s`", ਨਿਸ਼ਾਨਾ);
        }
    }

  ਸਰੋਤ = mount->ਸਰੋਤ ? mount->ਸਰੋਤ : type;

  /* Check if there is already a mount for the requested file system.  */
  ਜੇ (! mounted && ਸਰੋਤ_mountfd >= 0)
    {
      ret = fs_move_mount_ਮੰਜ਼ਿਲ (ਸਰੋਤ_mountfd, targetfd, ਨਿਰਾ);
      ਜੇ (LIKELY (ret == 0))
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਬਚਿਆ_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          ਜੇ (ਬਚਿਆ_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, ਨਿਰਾ, ਸਰੋਤ_mountfd, ਨਿਸ਼ਾਨਾ, ਨਿਰਾ, ਬਚਿਆ_flags, ਅੰਕੜੇ, LABEL_NONE | no_remount, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;
            }

          mounted = true;

          ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਨਿਸ਼ਾਨਾ))
            {
              ਪੂਰਨ new_rootfsfd = open (rootfs, O_ਰਸਤਾ | O_CLOEXEC);
              ਜੇ (UNLIKELY (new_rootfsfd < 0))
                ਵਾਪਸ crun_make_error (err, errno, "reopen rootfs after mount on /");

              ਆਰਜ਼ੀ_FAILURE_RETRY (close (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd));
              get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd = new_rootfsfd;
            }
        }
      ਨਹੀਂਤਾਂ ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          ਵਾਪਸ crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", ਨਿਸ਼ਾਨਾ);
        }
      ਨਹੀਂਤਾਂ ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->host_ਜੜ੍ਹ_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          ਵਾਪਸ crun_make_error (err, errno, "move_mount `%s` to `%s`", ਸਰੋਤ, ਨਿਸ਼ਾਨਾ);
        }
      ਨਹੀਂਤਾਂ ਜੇ (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           ਸਰੋਤ, ਨਿਸ਼ਾਨਾ, strerror (errno));
        }
    }

  ਜੇ (! mounted)
    {
      ਜੇ (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, ਸਰੋਤ, targetfd, ਨਿਸ਼ਾਨਾ, flags, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, ਸਰੋਤ, targetfd, ਨਿਸ਼ਾਨਾ, flags, ਅੰਕੜੇ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
      ਨਹੀਂਤਾਂ
        {
          ਪੂਰਨ extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, ਸਰੋਤ, targetfd, ਨਿਸ਼ਾਨਾ, type, flags, ਅੰਕੜੇ, extra_flags, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }

  ਜੇ (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, ਨਿਸ਼ਾਨਾ, copy_from_fd, err);
      copy_from_fd = -1;
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (rec_clear || rec_ਸੈੱਟ)
    {
      ਅਚਲ bool is_dir = S_ISDIR (src_ਬਹੁਲਕ);
      cleanup_close ਪੂਰਨ dfd = -1;

      dfd = ਸੁਰੱਖਿਅਤ_openat (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, ਨਿਸ਼ਾਨਾ, O_RDONLY | O_ਰਸਤਾ | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      ਜੇ (UNLIKELY (dfd < 0))
        ਵਾਪਸ crun_error_wrap (err, "open mount target `/%s`", ਨਿਸ਼ਾਨਾ);

      ret = do_mount_setattr (true, ਨਿਸ਼ਾਨਾ, dfd, rec_clear, rec_ਸੈੱਟ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
do_mounts (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਅਚਲ ਅੱਖਰ *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map ਬਣਤਰ libcrun_fd_map *mount_fds = ਨਿਰਾ;
  size_t i;
  ਪੂਰਨ ret;

  mount_fds = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->mount_fds;
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->mount_fds = ਨਿਰਾ;

  ਲਈ (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
ਪੂਰਨ
libcrun_container_do_bind_mount (libcrun_container_t *container, ਅੱਖਰ *mount_ਸਰੋਤ, ਅੱਖਰ *mount_destination, ਅੱਖਰ **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ = consume_slashes (mount_destination);
  cleanup_free ਅੱਖਰ *ਅੰਕੜੇ = ਨਿਰਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ extra_flags = 0;
  cleanup_close ਪੂਰਨ targetfd = -1;
  ਪੂਰਨ is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_ਸੈੱਟ = 0;
  ਅਚਲ ਅੱਖਰ *rootfs = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs;
  ਪੂਰਨ ret;

  ਜੇ ((get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd < 0) || (rootfs == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  ਜੇ (mount_options == ਨਿਰਾ)
    flags = get_default_flags (container, mount_destination, &ਅੰਕੜੇ);
  ਨਹੀਂਤਾਂ
    {
      size_t j;

      ਲਈ (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &ਅੰਕੜੇ, &rec_clear, &rec_ਸੈੱਟ);
    }

  ਜੇ (ਰਸਤਾ_is_slash_dev (mount_destination))
    get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->mount_dev_from_host = true;

  ਜੇ (mount_ਸਰੋਤ && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_ਸਰੋਤ, false, err);
      ਜੇ (UNLIKELY (is_dir < 0))
        ਵਾਪਸ is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_ਸੁਰੱਖਿਅਤ_create_and_open_ref_at (is_dir, get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, ਨਿਸ਼ਾਨਾ, 0755, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  targetfd = ret;

  ret = do_mount (container, mount_ਸਰੋਤ, targetfd, ਨਿਸ਼ਾਨਾ, "bind", flags, ਅੰਕੜੇ, LABEL_MOUNT, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
ਪੂਰਨ
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, ਪੂਰਨ *notify_socket_ਬਾਹਰਲਾ, libcrun_error_t *err)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
  cleanup_close ਪੂਰਨ notify_fd = -1;
  cleanup_free ਅੱਖਰ *host_notify_socket_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *state_dir = ਨਿਰਾ;
  ਅੱਖਰ *host_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ਜੇ (container && get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->host_notify_socket_ਰਸਤਾ)
    {
      ਅਚਲ ਅੱਖਰ *ਮਾਪਾ_dir;

      ਮਾਪਾ_dir = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->host_notify_socket_ਰਸਤਾ;

      ret = append_paths (&host_notify_socket_ਰਸਤਾ, err, ਮਾਪਾ_dir, "notify", ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      host_ਰਸਤਾ = host_notify_socket_ਰਸਤਾ;
    }

  *notify_socket_ਬਾਹਰਲਾ = -1;

  ਜੇ (host_ਰਸਤਾ == ਨਿਰਾ)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_ਜੜ੍ਹ, context->id, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = append_paths (&host_notify_socket_ਰਸਤਾ, err, state_dir, "notify/notify", ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      host_ਰਸਤਾ = host_notify_socket_ਰਸਤਾ;
    }

  notify_fd = open_unix_domain_socket (host_ਰਸਤਾ, 1, err);
  ਜੇ (UNLIKELY (notify_fd < 0))
    ਵਾਪਸ notify_fd;

  ਜੇ (UNLIKELY (chmod (host_ਰਸਤਾ, 0777) < 0))
    ਵਾਪਸ crun_make_error (err, errno, "chmod `%s`", host_ਰਸਤਾ);

#  ifdef HAVE_FGETXATTR
  ਜੇ (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (ਖਾਲੀ) setxattr (host_ਰਸਤਾ, "security.selinux", container->container_def->linux->mount_label,
                       ਲੜੀ_ਲੰਬਾਈ (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_ਬਾਹਰਲਾ = get_and_reset (&notify_fd);
  ਵਾਪਸ 1;
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) context;
  (ਖਾਲੀ) container;
  (ਖਾਲੀ) err;
  *notify_socket_ਬਾਹਰਲਾ = -1;
  ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
ਸਥਿਰ ਪੂਰਨ
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਅਚਲ ਅੱਖਰ *notify_socket = container->context->notify_socket;
  cleanup_free ਅੱਖਰ *host_notify_socket_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *container_notify_socket_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *state_dir = ਨਿਰਾ;
  uid_t container_ਜੜ੍ਹ_uid = -1;
  gid_t container_ਜੜ੍ਹ_gid = -1;
  ਪੂਰਨ notify_socket_ਰੁੱਖ_fd;

  ਜੇ (notify_socket == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_ਜੜ੍ਹ : ਨਿਰਾ),
                                     (container->context ? container->context->id : ਨਿਰਾ), err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_paths (&container_notify_socket_ਰਸਤਾ, err, "/", notify_socket, "notify", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_paths (&host_notify_socket_ਰਸਤਾ, err, state_dir, "notify", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = mkdir (host_notify_socket_ਰਸਤਾ, 0700);
  ਜੇ (ret < 0)
    ਵਾਪਸ crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_ਰਸਤਾ);

  ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_ਜੜ੍ਹ_ਵਿੱਚ_the_userns (container->container_def, 0, 0, &container_ਜੜ੍ਹ_uid, &container_ਜੜ੍ਹ_gid);
      ਜੇ (container_ਜੜ੍ਹ_uid != ((uid_t) -1) && container_ਜੜ੍ਹ_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_ਰਸਤਾ, container_ਜੜ੍ਹ_uid, container_ਜੜ੍ਹ_gid);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_ਰਸਤਾ, container_ਜੜ੍ਹ_uid, container_ਜੜ੍ਹ_gid);
        }
    }

  notify_socket_ਰੁੱਖ_fd = syscall_open_ਰੁੱਖ (AT_FDCWD, host_notify_socket_ਰਸਤਾ, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  ਜੇ (notify_socket_ਰੁੱਖ_fd >= 0)
    /* open_tree worked */
    get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->notify_socket_ਰੁੱਖ_fd = notify_socket_ਰੁੱਖ_fd;
  ਨਹੀਂਤਾਂ ਜੇ (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  ਨਹੀਂਤਾਂ ਜੇ (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  ਨਹੀਂਤਾਂ
    /* some other error */
    ਵਾਪਸ crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_ਰਸਤਾ);

  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->host_notify_socket_ਰਸਤਾ = host_notify_socket_ਰਸਤਾ;
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->container_notify_socket_ਰਸਤਾ = container_notify_socket_ਰਸਤਾ;
  host_notify_socket_ਰਸਤਾ = container_notify_socket_ਰਸਤਾ = ਨਿਰਾ;
  ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
ਸਥਿਰ ਪੂਰਨ
do_finalize_notify_socket (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rootfs, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_free ਅੱਖਰ *host_notify_socket_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *container_notify_socket_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *container_notify_socket_ਰਸਤਾ_dir_alloc = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *container_notify_socket_ਰਸਤਾ_dir = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ notify_socket_ਰੁੱਖ_fd = -1;
  cleanup_close ਪੂਰਨ targetfd = -1;
  ਪੂਰਨ did_mount_with_move_mount = 0;

  host_notify_socket_ਰਸਤਾ = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->host_notify_socket_ਰਸਤਾ;
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->host_notify_socket_ਰਸਤਾ = ਨਿਰਾ;

  container_notify_socket_ਰਸਤਾ = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->container_notify_socket_ਰਸਤਾ;
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->container_notify_socket_ਰਸਤਾ = ਨਿਰਾ;

  ਜੇ (host_notify_socket_ਰਸਤਾ == ਨਿਰਾ || container_notify_socket_ਰਸਤਾ == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਜੇ (rootfs == ਨਿਰਾ)
    rootfs = "/";

  container_notify_socket_ਰਸਤਾ_dir_alloc = xstrdup (container_notify_socket_ਰਸਤਾ);
  container_notify_socket_ਰਸਤਾ_dir = consume_slashes (dirname (container_notify_socket_ਰਸਤਾ_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_ਰਸਤਾ | O_CLOEXEC);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", rootfs);

      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd = ret;
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs = rootfs;
    }

  ret = crun_ਸੁਰੱਖਿਅਤ_create_and_open_ref_at (true, get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs,
                                          container_notify_socket_ਰਸਤਾ_dir, 0755, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  targetfd = ret;

  notify_socket_ਰੁੱਖ_fd = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->notify_socket_ਰੁੱਖ_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->notify_socket_ਰੁੱਖ_fd = -1;

  ਜੇ (notify_socket_ਰੁੱਖ_fd >= 0)
    {
      ret = fs_move_mount_ਮੰਜ਼ਿਲ (notify_socket_ਰੁੱਖ_fd, targetfd, ਨਿਰਾ);
      ਜੇ (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_ਨਿਸ਼ਾਨਾ (container, container_notify_socket_ਰਸਤਾ_dir, err);
          ਜੇ (UNLIKELY (targetfd < 0))
            ਵਾਪਸ targetfd;

          ret = do_mount (container, ਨਿਰਾ, targetfd, container_notify_socket_ਰਸਤਾ_dir, ਨਿਰਾ,
                          MS_REC | MS_PRIVATE, ਨਿਰਾ, LABEL_NONE, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
      ਨਹੀਂਤਾਂ ਜੇ (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_ਰੁੱਖ_fd,
                                container_notify_socket_ਰਸਤਾ_dir);
    }

  ਜੇ (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_ਰਸਤਾ, targetfd, container_notify_socket_ਰਸਤਾ_dir, ਨਿਰਾ,
                      MS_BIND | MS_REC | MS_PRIVATE, ਨਿਰਾ, LABEL_MOUNT, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
make_ਮਾਪਾ_mount_ਨਿੱਜੀ (ਅਚਲ ਅੱਖਰ *rootfs, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ rootfsfd = -1;
  ਬਣਤਰ stat prev_st;
  proc_fd_ਰਸਤਾ_t proc_ਰਸਤਾ;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (rootfsfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", rootfs);

  ਜੇ (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fstat `%s`", rootfs);

  ਲਈ (;;)
    {
      ਬਣਤਰ stat cur_st;
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਪੂਰਨ ret;
      cleanup_close ਪੂਰਨ parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      ਜੇ (ret == 0)
        ਵਾਪਸ 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_ਰਸਤਾ (proc_ਰਸਤਾ, rootfsfd);
      ret = mount (ਨਿਰਾ, proc_ਰਸਤਾ, ਨਿਰਾ, MS_PRIVATE, ਨਿਰਾ);
      ਜੇ (ret == 0)
        ਵਾਪਸ 0;

      parentfd = openat (rootfsfd, "..", O_ਰਸਤਾ | O_CLOEXEC);
      ਜੇ (parentfd < 0)
        {
          ਪੂਰਨ saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          ਜੇ (ret != 0)
            ਵਾਪਸ crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          ਵਾਪਸ crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      ਜੇ (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        ਵਾਪਸ crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      ਜੇ (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        ਰੋਕੋ;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  ਵਾਪਸ crun_make_error (err, 0, "make `%s` private", rootfs);
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_mounts (ਬਣਤਰ container_entrypoint_s *entrypoint_ਦਲੀਲਾਂ, libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rootfs, ਸੈੱਟ_mounts_cb_t cb, ਖਾਲੀ *cb_ਅੰਕੜੇ, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਪੂਰਨ is_user_ns = 0;
  ਪੂਰਨ ret = 0;

  ਜੇ (rootfs == ਨਿਰਾ || def->mounts == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ret = open (rootfs, O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", rootfs);

  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd = ret;
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_ਦਲੀਲਾਂ, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  ਜੇ (def->ਜੜ੍ਹ->readonly)
    {
      ਬਣਤਰ remount_s *r;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      ਪੂਰਨ fd;

      fd = dup (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd);
      ਜੇ (UNLIKELY (fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, ਨਿਰਾ, get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->remounts);
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWUSER);
  ਜੇ (! is_user_ns)
    {
      is_user_ns = check_running_ਵਿੱਚ_user_namespace (err);
      ਜੇ (UNLIKELY (is_user_ns < 0))
        ਵਾਪਸ is_user_ns;
    }

  ਜੇ (! get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  ਜੇ (cb)
    {
      ret = cb (cb_ਅੰਕੜੇ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਅਚਲ ਅੱਖਰ *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (ਖਾਲੀ) crun_ਸੁਰੱਖਿਅਤ_ensure_directory_at (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
umount_or_hide (ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = umount2 (ਨਿਸ਼ਾਨਾ, MNT_DETACH);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਪੂਰਨ saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      ਜੇ (errno == EINVAL)
        {
          ret = mount (ਨਿਰਾ, ਨਿਸ਼ਾਨਾ, "tmpfs", 0, "size=0k");
          ਜੇ (LIKELY (ret == 0))
            ਵਾਪਸ 0;
        }

      ਵਾਪਸ crun_make_error (err, saved_errno, "umount `%s`", ਨਿਸ਼ਾਨਾ);
    }

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
move_ਜੜ੍ਹ (ਅਚਲ ਅੱਖਰ *rootfs, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = chdir (rootfs);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = umount_or_hide ("/proc", err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_finalize_mounts (ਬਣਤਰ container_entrypoint_s *entrypoint_ਦਲੀਲਾਂ, libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rootfs,
                         ਸੈੱਟ_mounts_cb_t after_ਧੁਰਾ_cb, ਖਾਲੀ *cb_ਅੰਕੜੇ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_ਦਲੀਲਾਂ, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  close_and_reset (&(get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->old_ਜੜ੍ਹ_fd));

  ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->needs_ਧੁਰਾ)
    {
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->needs_ਧੁਰਾ = false;

      ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->no_ਧੁਰਾ)
        {
          ret = move_ਜੜ੍ਹ (rootfs, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
      ਨਹੀਂਤਾਂ
        {
          ret = do_ਧੁਰਾ (container, rootfs, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ret = do_mount (container, ਨਿਰਾ, -1, "/", ਨਿਰਾ,
                      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs_ਪ੍ਰਸਾਰ,
                      ਨਿਰਾ, LABEL_MOUNT, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = chdir ("/");
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "chdir to `/`");
    }

  ਜੇ (after_ਧੁਰਾ_cb)
    {
      ret = after_ਧੁਰਾ_cb (cb_ਅੰਕੜੇ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = finalize_mounts (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
maybe_open_ਰੁੱਖ_namespace (ਅਚਲ ਅੱਖਰ *rootfs, ਪੂਰਨ *ਬਾਹਰਲਾ_fd, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ rootfs_fd = -1;
  ਪੂਰਨ ਰੁੱਖ_fd;

  *ਬਾਹਰਲਾ_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (rootfs_fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", rootfs);

  ਰੁੱਖ_fd = syscall_open_ਰੁੱਖ (rootfs_fd, "",
                               OPEN_ਰੁੱਖ_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ | AT_RECURSIVE);
  ਜੇ (ਰੁੱਖ_fd < 0)
    {
      ਜੇ (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        ਵਾਪਸ 0;
      ਵਾਪਸ crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *ਬਾਹਰਲਾ_fd = ਰੁੱਖ_fd;
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  ਬਣਤਰ libcrun_fd_map *mount_fds = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->mount_fds;

  ਜੇ (mount_fds == ਨਿਰਾ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->mount_fds = mount_fds;
    }
  ਵਾਪਸ mount_fds;
}

ਸਥਿਰ ਪੂਰਨ
open_mount_ਦਾ_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ਅੱਖਰ *mnt_ਅੰਕੜੇ = ਨਿਰਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_ਸੈੱਟ = 0;
  ਅਚਲ ਅੱਖਰ *label_type = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *label_val = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *fstype;
  ਪੂਰਨ mnt_fd;
  size_t j;

  ਜੇ (mount->type == ਨਿਰਾ)
    ਵਾਪਸ -1;

  fstype = mount->type;

  ਲਈ (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_ਅੰਕੜੇ,
                                            &mnt_rec_clear, &mnt_rec_ਸੈੱਟ);

  ਜੇ (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      ਪੂਰਨ extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      ਜੇ (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          ਜੇ (label_type == ਨਿਰਾ)
            crun_error_release (err);
          ਨਹੀਂਤਾਂ
            label_val = def->linux->mount_label;
        }
    }

  ਜੇ (strcmp (fstype, "cgroup") == 0)
    {
      ਪੂਰਨ cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
      ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->ਸਰੋਤ, label_type, label_val, mnt_ਅੰਕੜੇ);
  ਜੇ (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->ਸਰੋਤ, ਨਿਰਾ, ਨਿਰਾ, mnt_ਅੰਕੜੇ);

  ਵਾਪਸ mnt_fd;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LSMT_ROOT
#  define LSMT_ROOT 0xffffffffffffffffULL
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STATMOUNT_FS_TYPE
#  define STATMOUNT_FS_TYPE 0x00000020U
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STATMOUNT_MNT_ROOT
#  define STATMOUNT_MNT_ROOT 0x00000040U
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STATMOUNT_MNT_POINT
#  define STATMOUNT_MNT_POINT 0x00000080U
#ਜੇ_ਅੰਤ

/* Declared here so that the build does not depend on the UAPI headers being
   recent enough; the layout is stable.  */
ਬਣਤਰ mnt_id_req_s
{
  uint32_t ਮਾਪ;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

ਬਣਤਰ statmount_s
{
  uint32_t ਮਾਪ;
  uint32_t mnt_opts;
  uint64_t ਨਕਾਬ;
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
  ਅੱਖਰ str[];
};

ਸਥਿਰ ਪੂਰਨ
syscall_listmount (ਬਣਤਰ mnt_id_req_s *req, uint64_t *ids, size_t nr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __NR_listmount
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_listmount, req, ids, nr, flags);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) req;
  (ਖਾਲੀ) ids;
  (ਖਾਲੀ) nr;
  (ਖਾਲੀ) flags;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਪੂਰਨ
syscall_statmount (ਬਣਤਰ mnt_id_req_s *req, ਬਣਤਰ statmount_s *buf, size_t ਮਾਪ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __NR_statmount
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_statmount, req, buf, ਮਾਪ, flags);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) req;
  (ਖਾਲੀ) buf;
  (ਖਾਲੀ) ਮਾਪ;
  (ਖਾਲੀ) flags;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
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
ਸਥਿਰ bool
mounts_ਪ੍ਰਾਪਤੀ_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = ਨਿਰਾ;
  size_t i, n_ids = 0, ਸਮਰੱਥਾ = 128;
  bool has_absolute_bind = false;
  uint64_t ਆਖਰੀ = 0;

  ਲਈ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      ਜੇ (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->ਸਰੋਤ != ਨਿਰਾ && def->mounts[i]->ਸਰੋਤ[0] == '/')
        {
          has_absolute_bind = true;
          ਰੋਕੋ;
        }
    }
  ਜੇ (! has_absolute_bind)
    ਵਾਪਸ false;

  ids = xmalloc (ਸਮਰੱਥਾ * ਆਕਾਰ (uint64_t));
  ਲਈ (;;)
    {
      ਬਣਤਰ mnt_id_req_s req = {
        .ਮਾਪ = ਆਕਾਰ (req),
        .mnt_id = LSMT_ROOT,
        .param = ਆਖਰੀ,
      };
      ਪੂਰਨ ret;

      ਜੇ (n_ids == ਸਮਰੱਥਾ)
        {
          ਸਮਰੱਥਾ *= 2;
          ids = xrealloc (ids, ਸਮਰੱਥਾ * ਆਕਾਰ (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, ਸਮਰੱਥਾ - n_ids, 0);
      ਜੇ (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        ਵਾਪਸ true;
      ਜੇ (ret == 0)
        ਰੋਕੋ;

      n_ids += (size_t) ret;
      ਆਖਰੀ = ids[n_ids - 1];
    }

  ਲਈ (i = 0; i < n_ids; i++)
    {
      ਅੱਖਰ ਬਫਰ[4096] __attribute__ ((aligned (8)));
      ਬਣਤਰ statmount_s *sm = (ਬਣਤਰ statmount_s *) ਬਫਰ;
      ਬਣਤਰ mnt_id_req_s req = {
        .ਮਾਪ = ਆਕਾਰ (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      ਅਚਲ ਅੱਖਰ *ਜੜ੍ਹ, *ਬਿੰਦੂ;
      size_t j;

      ਜੇ (syscall_statmount (&req, sm, ਆਕਾਰ (ਬਫਰ), 0) < 0)
        ਜਾਰੀ;
      ਜੇ ((sm->ਨਕਾਬ & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        ਜਾਰੀ;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      ਜੜ੍ਹ = sm->str + sm->mnt_root;
      ਜੇ (! has_ਅਗੇਤਰ (ਜੜ੍ਹ, "mnt:["))
        ਜਾਰੀ;

      ਬਿੰਦੂ = sm->str + sm->mnt_point;

      ਲਈ (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          ਅਚਲ ਅੱਖਰ *ਸਰੋਤ = def->mounts[j]->ਸਰੋਤ;
          size_t ਸਰੋਤ_len;

          ਜੇ (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            ਜਾਰੀ;
          ਜੇ (ਸਰੋਤ == ਨਿਰਾ || ਸਰੋਤ[0] != '/')
            ਜਾਰੀ;

          ਸਰੋਤ_len = ਲੜੀ_ਲੰਬਾਈ (ਸਰੋਤ);
          ਜਦੋਂ (ਸਰੋਤ_len > 1 && ਸਰੋਤ[ਸਰੋਤ_len - 1] == '/')
            ਸਰੋਤ_len--;

          ਜੇ (ਲੜੀ_ਤੁਲਨਾ (ਬਿੰਦੂ, ਸਰੋਤ, ਸਰੋਤ_len) != 0)
            ਜਾਰੀ;

          /* The source itself is a pinned mount namespace.  */
          ਜੇ (ਬਿੰਦੂ[ਸਰੋਤ_len] == '\0')
            ਵਾਪਸ true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          ਜੇ (recursive && (ਸਰੋਤ_len == 1 || ਬਿੰਦੂ[ਸਰੋਤ_len] == '/'))
            ਵਾਪਸ true;
        }
    }

  ਵਾਪਸ false;
}

ਸਥਿਰ bool
can_use_open_ਰੁੱਖ_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਬਣਤਰ libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  ਜੇ (has_hooks || has_userns)
    ਵਾਪਸ false;

  ਜੇ (mounts_ਪ੍ਰਾਪਤੀ_pinned_mount_namespace (def))
    ਵਾਪਸ false;

  ਜੇ (container->context->no_open_ਰੁੱਖ_namespace)
    ਵਾਪਸ false;

  {
    libcrun_error_t tmp_err = ਨਿਰਾ;
    ਪੂਰਨ ਵਿੱਚ_userns = check_running_ਵਿੱਚ_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    ਜੇ (ਵਿੱਚ_userns != 0)
      ਵਾਪਸ false;
  }

  mount_fds = get_fd_map (container);
  ਲਈ (i = 0; i < def->mounts_len; i++)
    {
      ਜੇ (mount_fds->fds[i] < 0)
        {
          bool has_cached_ਨਿਸ਼ਾਨਾ = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->copy_symlink_targets
                                   && i < get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->n_copy_symlink_targets
                                   && get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->copy_symlink_targets[i];
          ਜੇ (! has_cached_ਨਿਸ਼ਾਨਾ)
            ਵਾਪਸ false;
        }
    }

  ਵਾਪਸ true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
ਸਥਿਰ bool
mount_inherits_mountpoint_ਬਹੁਲਕ (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ਜੇ (mount->type == ਨਿਰਾ || strcmp (mount->type, "tmpfs") != 0)
    ਵਾਪਸ false;

  ਲਈ (i = 0; i < mount->options_len; i++)
    ਜੇ (mount->options[i] != ਨਿਰਾ && has_ਅਗੇਤਰ (mount->options[i], "mode="))
      ਵਾਪਸ false;

  ਵਾਪਸ true;
}

ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_mount_namespace (libcrun_container_t *container, bool no_ਧੁਰਾ, ਅੱਖਰ **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ rootfs_ਪ੍ਰਸਾਰ = 0;
  cleanup_close ਪੂਰਨ rootfsfd = -1;
  cleanup_close ਪੂਰਨ ਰੁੱਖ_fd = -1;
  libcrun_error_t tmp_err = ਨਿਰਾ;
  bool use_open_ਰੁੱਖ = false;
  size_t i;
  ਪੂਰਨ ret;

  ਜੇ (def->linux && def->linux->rootfs_ਪ੍ਰਸਾਰ)
    rootfs_ਪ੍ਰਸਾਰ = get_mount_flags (def->linux->rootfs_ਪ੍ਰਸਾਰ, 0, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);

  ਜੇ ((rootfs_ਪ੍ਰਸਾਰ & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_ਪ੍ਰਸਾਰ = MS_REC | MS_PRIVATE;

  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs_ਪ੍ਰਸਾਰ = rootfs_ਪ੍ਰਸਾਰ;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਮੁਕਤ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->host_rootfs);
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  ਲਈ (i = 0; i < def->mounts_len; i++)
    {
      ਬਣਤਰ libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close ਪੂਰਨ mnt_fd = -1;
      bool recursive_bind, nofollow;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mnt_flags = 0;

      ਜੇ (mount_fds && mount_fds->fds[i] >= 0)
        ਜਾਰੀ;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      ਜੇ (def->mounts[i]->ਸਰੋਤ != ਨਿਰਾ
          && def->mounts[i]->ਸਰੋਤ[0] == '/')
        {
          cleanup_free ਅੱਖਰ *mnt_ਅੰਕੜੇ = ਨਿਰਾ;
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_ਸੈੱਟ = 0;
          size_t j;

          ਲਈ (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_ਅੰਕੜੇ,
                                                    &mnt_rec_clear, &mnt_rec_ਸੈੱਟ);

          ਜੇ (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free ਅੱਖਰ *ਨਿਸ਼ਾਨਾ = ਨਿਰਾ;
              libcrun_error_t tmp_err = ਨਿਰਾ;
              ssize_t len;

              len = ਸੁਰੱਖਿਅਤ_readlinkat (AT_FDCWD, def->mounts[i]->ਸਰੋਤ, &ਨਿਸ਼ਾਨਾ, 0, &tmp_err);
              ਜੇ (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->copy_symlink_targets == ਨਿਰਾ)
                {
                  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * ਆਕਾਰ (ਅੱਖਰ *));
                  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->copy_symlink_targets[i] = ਨਿਸ਼ਾਨਾ;
              ਨਿਸ਼ਾਨਾ = ਨਿਰਾ;
            }
        }

      ਜੇ (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_ਨਿਸ਼ਾਨਾ = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->copy_symlink_targets
                                   && i < get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->n_copy_symlink_targets
                                   && get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->copy_symlink_targets[i];

          ਜੇ (mount_fds && def->mounts[i]->ਸਰੋਤ != ਨਿਰਾ && ! has_cached_ਨਿਸ਼ਾਨਾ
              && get_ਰਸਤਾ_ਵਿੱਚ_rootfs (*rootfs, def->mounts[i]->ਸਰੋਤ) == ਨਿਰਾ)
            {
              libcrun_error_t tmp_err = ਨਿਰਾ;
              ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪ੍ਰਸਾਰ = MS_PRIVATE;

              ਜੇ (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                ਪ੍ਰਸਾਰ = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->ਸਰੋਤ,
                                                  recursive_bind, false, nofollow,
                                                  ਪ੍ਰਸਾਰ, &tmp_err);
              ਜੇ (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          ਜਾਰੀ;
        }

      mnt_fd = open_mount_ਦਾ_type (container, def->mounts[i], err);
      ਜੇ (mnt_fd < 0)
        {
          crun_error_release (err);
          ਜਾਰੀ;
        }

      ਜੇ (mount_inherits_mountpoint_ਬਹੁਲਕ (def->mounts[i]))
        {
          cleanup_close ਪੂਰਨ destfd = -1;
          ਬਣਤਰ stat st;

          ਜੇ (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_ਰਸਤਾ | O_CLOEXEC);
              ਜੇ (UNLIKELY (rootfsfd < 0))
                ਵਾਪਸ crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = ਸੁਰੱਖਿਅਤ_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_ਰਸਤਾ | O_CLOEXEC, 0, &tmp_err);
          ਜੇ (destfd < 0)
            crun_error_release (&tmp_err);
          ਨਹੀਂਤਾਂ ਜੇ (fstat (destfd, &st) == 0)
            {
              proc_fd_ਰਸਤਾ_t fd_ਰਸਤਾ;
              ਪੂਰਨ procfd = get_procfd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container), err);
              ਜੇ (UNLIKELY (procfd < 0))
                ਵਾਪਸ procfd;

              get_self_fd_ਰਸਤਾ (fd_ਰਸਤਾ, mnt_fd);
              ret = fchmodat (procfd, fd_ਰਸਤਾ, st.st_mode & 07777, 0);
              (ਖਾਲੀ) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_ਖਾਲੀ_ਥਾਵਾਂ_dir_cached (container, ਨਿਰਾ, &tmp_err);
  ਜੇ (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->maskdir_bind_failed = true;
    }

  ਜੇ (! get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->joined_mount_ns
      && can_use_open_ਰੁੱਖ_namespace (container))
    use_open_ਰੁੱਖ = true;

  ਜੇ (use_open_ਰੁੱਖ)
    {
      ret = maybe_open_ਰੁੱਖ_namespace (*rootfs, &ਰੁੱਖ_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  ਜੇ (ਰੁੱਖ_fd >= 0)
    preopen_needed_devs_and_notify (container);

  ਜੇ (ਰੁੱਖ_fd >= 0)
    {
      ret = setns (ਰੁੱਖ_fd, CLONE_NEWNS);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (ਨਿਰਾ, "/", ਨਿਰਾ, MS_REMOUNT | MS_BIND, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, ਨਿਰਾ, -1, "/", ਨਿਰਾ, MS_REC | MS_PRIVATE, ਨਿਰਾ, LABEL_MOUNT, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = do_mount (container, ਨਿਰਾ, -1, "/", ਨਿਰਾ, rootfs_ਪ੍ਰਸਾਰ, ਨਿਰਾ, LABEL_MOUNT, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->needs_ਧੁਰਾ = false;
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->no_ਧੁਰਾ = no_ਧੁਰਾ;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->host_ਜੜ੍ਹ_switched = true;
      ਮੁਕਤ (*rootfs);
      *rootfs = xstrdup ("/");
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (! get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, ਨਿਰਾ, -1, "/", ਨਿਰਾ, rootfs_ਪ੍ਰਸਾਰ, ਨਿਰਾ, LABEL_MOUNT, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = make_ਮਾਪਾ_mount_ਨਿੱਜੀ (*rootfs, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, ਨਿਰਾ, MS_BIND | MS_REC | MS_PRIVATE, ਨਿਰਾ, LABEL_MOUNT, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->needs_ਧੁਰਾ = true;
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->no_ਧੁਰਾ = no_ਧੁਰਾ;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", *rootfs);
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd = ret;
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs = *rootfs;

  ਜੇ (! container->context->force_no_cgroup)
    {
      ਪੂਰਨ cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
      ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
        ਵਾਪਸ cgroup_ਬਹੁਲਕ;

      ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
        {
          ਅੱਖਰ *unified_cgroup_ਰਸਤਾ = ਨਿਰਾ;
          ਪੂਰਨ procfd = get_procfd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container), err);
          ਜੇ (UNLIKELY (procfd < 0))
            ਵਾਪਸ procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_ਰਸਤਾ, true, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unified_cgroup_ਰਸਤਾ = unified_cgroup_ਰਸਤਾ;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd);
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs = ਨਿਰਾ;
      ਵਾਪਸ ret;
    }

  close_and_reset (&get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfsfd);
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->rootfs = ਨਿਰਾ;

  ਜੇ (ਰੁੱਖ_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container), &tmp_err);
  ਜੇ (ret < 0)
    crun_error_release (&tmp_err);

  check_running_ਵਿੱਚ_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_do_ਧੁਰਾ_ਜੜ੍ਹ (libcrun_container_t *container, bool no_ਧੁਰਾ, ਅੱਖਰ **rootfs, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ਜੇ (*rootfs == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = ਤਿਆਰੀ_mount_namespace (container, no_ਧੁਰਾ, rootfs, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      ਜੇ (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "chdir to `/`");
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ret = chroot (*rootfs);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "chdir to `/`");

      ਮੁਕਤ (*rootfs);
      *rootfs = xstrdup ("/");
    }

  ਵਾਪਸ 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
ਪੂਰਨ
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  ਬਣਤਰ stat statbuf;
  cleanup_close ਪੂਰਨ fd;
  ਪੂਰਨ ret;
  ਪੂਰਨ i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  ਜੇ (UNLIKELY (fd == -1))
    ਵਾਪਸ crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਲਈ (i = 0; i <= 2; i++)
    {
      ਜੇ (UNLIKELY (fstat (i, &statbuf) == -1))
        ਵਾਪਸ crun_make_error (err, errno, "stat fd `%d`", i);
      ਜੇ (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          ਜੇ (UNLIKELY (dup2 (fd, i) == -1))
            ਵਾਪਸ crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
uidgidmap_helper (ਅੱਖਰ *helper, pid_t pid, ਅਚਲ ਅੱਖਰ *map_file, libcrun_error_t *err)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_ਦਲੀਲਾਂ 20
  ਅੱਖਰ pid_fmt[16];
  ਅੱਖਰ *ਦਲੀਲਾਂ[MAX_ਦਲੀਲਾਂ + 1];
  ਅੱਖਰ *ਅਗਲਾ;
  cleanup_free ਅੱਖਰ *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  ਪੂਰਨ ret;

  ਦਲੀਲਾਂ[nargs++] = helper;

  ret = snprintf (pid_fmt, ਆਕਾਰ (pid_fmt), "%d", pid);
  ਜੇ (UNLIKELY (ret >= (ਪੂਰਨ) ਆਕਾਰ (pid_fmt)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

  ਦਲੀਲਾਂ[nargs++] = pid_fmt;
  ਅਗਲਾ = map_file_copy;
  ਜਦੋਂ (nargs < MAX_ਦਲੀਲਾਂ)
    {
      ਅੱਖਰ *p = strsep (&ਅਗਲਾ, " \n");
      ਜੇ (ਅਗਲਾ == ਨਿਰਾ)
        ਰੋਕੋ;
      ਦਲੀਲਾਂ[nargs++] = p;
    }
  ਦਲੀਲਾਂ[nargs++] = ਨਿਰਾ;

  ret = ਲੜੀਵਾਰ_process (ਦਲੀਲਾਂ, err);
  ਜੇ (ret < 0)
    ਵਾਪਸ ret;
  ਜੇ (ret > 0)
    ਵਾਪਸ crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
newgidmap (pid_t pid, ਅਚਲ ਅੱਖਰ *map_file, libcrun_error_t *err)
{
  ਵਾਪਸ uidgidmap_helper ("newgidmap", pid, map_file, err);
}

ਸਥਿਰ ਪੂਰਨ
newuidmap (pid_t pid, ਅਚਲ ਅੱਖਰ *map_file, libcrun_error_t *err)
{
  ਵਾਪਸ uidgidmap_helper ("newuidmap", pid, map_file, err);
}

ਸਥਿਰ ਪੂਰਨ
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_close ਪੂਰਨ fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ fd;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, "deny", 4));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->deny_setgroups = true;
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;

  ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->deny_setgroups)
    ਵਾਪਸ 0;

  ਜੇ (container->container_def->annotations)
    {
      ਅਚਲ ਅੱਖਰ *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = ਲੱਭੋ_annotation (container, "run.oci.keep_original_groups");
      ਜੇ (annotation)
        ਵਾਪਸ strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close ਪੂਰਨ fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    ਜੇ (fd < 0)
      ret = fd;
    ਨਹੀਂਤਾਂ
      ret = read_all_fd (fd, "self/setgroups", &content, ਨਿਰਾ, err);
  }
  ਜੇ (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      ਜੇ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ਵਾਪਸ 1;
        }
      ਵਾਪਸ ret;
    }

  ਵਾਪਸ ਲੜੀ_ਤੁਲਨਾ (content, "deny", 4) == 0 ? 0 : 1;
}

ਪੂਰਨ
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = ਨਿਰਾ;
  size_t additional_gids_len = 0;
  ਪੂਰਨ can_do_setgroups;
  ਪੂਰਨ ret;

  ਜੇ (process != ਨਿਰਾ && process->user != ਨਿਰਾ)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  ਜੇ (UNLIKELY (can_do_setgroups < 0))
    ਵਾਪਸ can_do_setgroups;

  ਜੇ (can_do_setgroups == 0)
    {
      ਅਚਲ ਅੱਖਰ *annotation;
      size_t i;

      annotation = ਲੱਭੋ_annotation (container, "run.oci.keep_original_groups");
      ਜੇ (annotation && strcmp (annotation, "0") != 0)
        ਵਾਪਸ 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      ਲਈ (i = 0; i < additional_gids_len; i++)
        ਜੇ (additional_gids[i] != process->user->gid)
          ਵਾਪਸ crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (ਪੂਰਨ) additional_gids[i]);
      ਵਾਪਸ 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "setgroups");

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_cgroupns)
    {
      ਪੂਰਨ ret = unshare (CLONE_NEWCGROUP);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (errno != EINVAL)
            ਵਾਪਸ crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  ਵਾਪਸ 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
ਪੂਰਨ
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  ਜੇ (len != 1)
    ਵਾਪਸ 0;

  ਜੇ (mappings[0]->ਮਾਪ != 1)
    ਵਾਪਸ 0;

  ਜੇ (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    ਵਾਪਸ 0;

  ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ
write_id_map (libcrun_container_t *container, pid_t pid, ਅਚਲ ਅੱਖਰ *map_name,
              ਪੂਰਨ (*idmap_helper) (pid_t, ਅਚਲ ਅੱਖਰ *, libcrun_error_t *), ਅਚਲ ਅੱਖਰ *helper_name,
              bool warn_on_failure, ਅਚਲ ਅੱਖਰ *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  ਪੂਰਨ ret = 0;

  ਜੇ (container->host_uid)
    ret = idmap_helper (pid, map, err);
  ਜੇ (container->host_uid == 0 || ret < 0)
    {
      cleanup_close ਪੂਰਨ fd = -1;

      ਜੇ (ret < 0)
        {
          ਜੇ (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      ਜੇ (UNLIKELY (fd < 0))
        ਵਾਪਸ fd;

      ret = ਸੁਰੱਖਿਅਤ_write (fd, map_name, map, map_len, err);
      ਜੇ (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free ਅੱਖਰ *single_mapping = ਨਿਰਾ;
          crun_error_release (err);

          ਜੇ (! skip_if_setgroups_denied || ! get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          ਜੇ (UNLIKELY (fd < 0))
            ਵਾਪਸ fd;

          ret = ਸੁਰੱਖਿਅਤ_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *uid_map = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *gid_map = ਨਿਰਾ;
  size_t uid_map_len = 0, gid_map_len = 0;
  ਪੂਰਨ ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ਜੇ ((get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWUSER) == 0)
    ਵਾਪਸ 0;

  ਜੇ (! def || ! def->linux)
    ਵਾਪਸ 0;

  ਜੇ (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      uid_map_len = (size_t) ret;
      ਜੇ (uid_map == ਨਿਰਾ)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }

  ਜੇ (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      gid_map_len = (size_t) ret;
      ਜੇ (gid_map == ਨਿਰਾ)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CAP_ਮੰਜ਼ਿਲ_ਨਕਾਬ_0(x) (1L << ((x) & 31))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CAP_ਮੰਜ਼ਿਲ_ਨਕਾਬ_1(x) CAP_ਮੰਜ਼ਿਲ_ਨਕਾਬ_0 (x - 32)

ਬਣਤਰ all_caps_s
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ effective[2];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ permitted[2];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ inheritable[2];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ambient[2];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ bounding[2];
};

ਸਥਿਰ ਪੂਰਨ
has_cap_on (ਪੂਰਨ cap, ਲੰਮਾ ਬਿਨਾਨਿਸ਼ਾਨੀ *caps)
{
  ਜੇ (cap < 32)
    ਵਾਪਸ CAP_ਮੰਜ਼ਿਲ_ਨਕਾਬ_0 (cap) & caps[0];
  ਵਾਪਸ (CAP_ਮੰਜ਼ਿਲ_ਨਕਾਬ_1 (cap) & caps[1]);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cap_ਆਖਰੀ_cap;

ਪੂਰਨ
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ fd = -1;
  ਪੂਰਨ ret;
  ਅੱਖਰ ਬਫਰ[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  ਜੇ (fd < 0)
    ਵਾਪਸ fd;
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (fd, ਬਫਰ, ਆਕਾਰ (ਬਫਰ) - 1));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  ਬਫਰ[ret] = '\0';

  errno = 0;
  cap_ਆਖਰੀ_cap = strtoul (ਬਫਰ, ਨਿਰਾ, 10);
  ਜੇ (errno != 0)
    ਵਾਪਸ crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", ਬਫਰ);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਸੈੱਟ_required_caps (ਬਣਤਰ all_caps_s *caps, uid_t uid, gid_t gid, ਪੂਰਨ no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAP
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cap;
  ਪੂਰਨ ret;
  ਬਣਤਰ __user_cap_header_struct hdr = { LINUX_CAPABILITY_ਸੰਸਕਰਣ_3, 0 };
  ਬਣਤਰ __user_cap_data_struct ਅੰਕੜੇ[2] = { { 0 } };

  ਜੇ (cap_ਆਖਰੀ_cap == 0)
    ਵਾਪਸ crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  ਲਈ (cap = 0; cap <= cap_ਆਖਰੀ_cap; cap++)
    ਜੇ (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        ਜੇ (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          ਵਾਪਸ crun_make_error (err, errno, "prctl drop bounding");
      }

  ਅੰਕੜੇ[0].effective = caps->effective[0];
  ਅੰਕੜੇ[1].effective = caps->effective[1];
  ਅੰਕੜੇ[0].inheritable = caps->inheritable[0];
  ਅੰਕੜੇ[1].inheritable = caps->inheritable[1];
  ਅੰਕੜੇ[0].permitted = caps->permitted[0];
  ਅੰਕੜੇ[1].permitted = caps->permitted[1];

  ret = prctl (PR_ਸੈੱਟ_KEEPCAPS, 1, 0, 0, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  ਜੇ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ਵਾਪਸ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, ਅੰਕੜੇ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  ਜੇ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    ਵਾਪਸ crun_make_error (err, errno, "prctl reset ambient");

  ਲਈ (cap = 0; cap <= cap_ਆਖਰੀ_cap; cap++)
    ਜੇ (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        ਜੇ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          ਵਾਪਸ crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  ਜੇ (no_exec)
    {
      ret = prctl (PR_ਸੈੱਟ_KEEPCAPS, 0, 0, 0, 0);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "prctl reset KEEPCAPS");

      ਜੇ (uid != 0)
        {
          ਅੰਕੜੇ[0].permitted = ਅੰਕੜੇ[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          ਲਈ (cap = 0; cap <= cap_ਆਖਰੀ_cap; cap++)
            ਜੇ (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_ਸੈੱਟ, cap, 0, 0) > 0)
              ਅੰਕੜੇ[cap / 32].permitted |= CAP_ਮੰਜ਼ਿਲ_ਨਕਾਬ_0 (cap);
#  endif
          ਅੰਕੜੇ[0].effective = ਅੰਕੜੇ[0].permitted;
          ਅੰਕੜੇ[1].effective = ਅੰਕੜੇ[1].permitted;

          ret = capset (&hdr, ਅੰਕੜੇ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "capset");
        }
    }
#ਜੇ_ਅੰਤ

  ਜੇ (no_new_privs)
    ਜੇ (UNLIKELY (prctl (PR_ਸੈੱਟ_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      ਵਾਪਸ crun_make_error (err, errno, "prctl set no new privs");

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
read_caps (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ caps[2], ਅੱਖਰ **ਮੁੱਲਾਂ, size_t len, libcrun_error_t *err)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAP
  size_t i;
  ਲਈ (i = 0; i < len; i++)
    {
      cap_ਮੁੱਲ_t cap;
      ਜੇ (cap_from_name (ਮੁੱਲਾਂ[i], &cap) < 0)
        ਵਾਪਸ crun_make_error (err, 0, "unknown capability `%s`", ਮੁੱਲਾਂ[i]);

      ਜੇ (cap < 32)
        caps[0] |= CAP_ਮੰਜ਼ਿਲ_ਨਕਾਬ_0 (cap);
      ਨਹੀਂਤਾਂ
        caps[1] |= CAP_ਮੰਜ਼ਿਲ_ਨਕਾਬ_1 (cap);
    }
#ਨਹੀਂਤਾਂ
  caps[0] = 0;
  caps[1] = 0;
#ਜੇ_ਅੰਤ
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ਜੇ (proc->selinux_label)
    ਵਾਪਸ ਸੈੱਟ_selinux_label (container, proc->selinux_label, now, err);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ਜੇ (proc->apparmor_profile)
    ਵਾਪਸ ਸੈੱਟ_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  ਪੂਰਨ no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  ਬਣਤਰ all_caps_s caps = {};
  ਪੂਰਨ ret;

  ਜੇ (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ ਸੈੱਟ_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

ਬਣਤਰ rlimit_s
{
  ਅਚਲ ਅੱਖਰ *name;
  ਪੂਰਨ ਮੁੱਲ;
};

ਬਣਤਰ rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
                              { "RLIMIT_CORE", RLIMIT_CORE },
                              { "RLIMIT_CPU", RLIMIT_CPU },
                              { "RLIMIT_DATA", RLIMIT_ਅੰਕੜੇ },
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
                              { "RLIMIT_STACK", RLIMIT_ਢੇਰ },
                              { ਨਿਰਾ, 0 } };

ਸਥਿਰ ਪੂਰਨ
get_rlimit_resource (ਅਚਲ ਅੱਖਰ *name)
{
  ਬਣਤਰ rlimit_s *it;
  ਲਈ (it = rlimits; it->name; it++)
    ਜੇ (strcmp (it->name, name) == 0)
      ਵਾਪਸ it->ਮੁੱਲ;
  ਵਾਪਸ -1;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  ਲਈ (i = 0; i < len; i++)
    {
      ਲਈ (j = i + 1; j < len; j++)
        {
          ਜੇ (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            ਵਾਪਸ crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  ਲਈ (i = 0; i < len; i++)
    {
      ਬਣਤਰ rlimit ਹੱਦਬੰਦੀ;
      ਅੱਖਰ *type = new_rlimits[i]->type;
      ਪੂਰਨ resource = get_rlimit_resource (type);
      ਜੇ (UNLIKELY (resource < 0))
        ਵਾਪਸ crun_make_error (err, 0, "invalid rlimit `%s`", type);
      ਹੱਦਬੰਦੀ.rlim_cur = new_rlimits[i]->soft;
      ਹੱਦਬੰਦੀ.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਹੱਦਬੰਦੀ.rlim_cur,
                     (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਹੱਦਬੰਦੀ.rlim_max);
      ਜੇ (UNLIKELY (setrlimit (resource, &ਹੱਦਬੰਦੀ) < 0))
        ਵਾਪਸ crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਪੂਰਨ has_uts = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWUTS;
  ਪੂਰਨ ret;
  ਜੇ (def->hostname == ਨਿਰਾ || def->hostname[0] == '\0')
    ਵਾਪਸ 0;
  ਜੇ (! has_uts)
    ਵਾਪਸ crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, ਲੜੀ_ਲੰਬਾਈ (def->hostname));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "sethostname");
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਪੂਰਨ has_uts = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWUTS;
  ਪੂਰਨ ret;
  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (def->domainname))
    ਵਾਪਸ 0;
  ਜੇ (! has_uts)
    ਵਾਪਸ crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, ਲੜੀ_ਲੰਬਾਈ (def->domainname));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "setdomainname");
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ਪੂਰਨ fd = -1;
  ਪੂਰਨ ret;
  ਅੱਖਰ oom_ਬਫਰ[16];
  ਜੇ (def->process == ਨਿਰਾ || ! def->process->oom_score_adj_present)
    ਵਾਪਸ 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_ਬਫਰ, ਆਕਾਰ (oom_ਬਫਰ), "%i", def->process->oom_score_adj);
  ਜੇ (UNLIKELY (ret >= (ਪੂਰਨ) ਆਕਾਰ (oom_ਬਫਰ)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  ਜੇ (fd < 0)
    ਵਾਪਸ fd;
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, oom_ਬਫਰ, ਲੜੀ_ਲੰਬਾਈ (oom_ਬਫਰ)));
  ਜੇ (ret < 0)
    ਵਾਪਸ crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  ਵਾਪਸ 0;
}

ਅਚਲ ਅੱਖਰ *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ
validate_sysctl (ਅਚਲ ਅੱਖਰ *original_ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ *original_ਮੁੱਲ, ਅਚਲ ਅੱਖਰ *name, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  ਜੇ (name[0] == '/' || ਰਸਤਾ_has_dot_dot_component (name))
    ਵਾਪਸ crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_ਕੁੰਜੀ);

  ਜੇ (has_ਅਗੇਤਰ (name, "fs/mqueue/"))
    {
      ਜੇ (namespaces_created & CLONE_NEWIPC)
        ਵਾਪਸ 0;

      namespace = "IPC";
      ਜਾਓ fail;
    }

  ਜੇ (has_ਅਗੇਤਰ (name, "kernel/"))
    {
      size_t i;

      ਲਈ (i = 0; sysctlRequiringIPC[i]; i++)
        ਜੇ (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            ਜੇ (namespaces_created & CLONE_NEWIPC)
              ਵਾਪਸ 0;

            namespace = "IPC";
            ਜਾਓ fail;
          }

      ਜੇ (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          ਜੇ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (def->domainname) && (strcmp (original_ਮੁੱਲ, def->domainname) != 0))
            ਵਾਪਸ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_ਕੁੰਜੀ);

          ਜੇ (namespaces_created & CLONE_NEWUTS)
            ਵਾਪਸ 0;

          namespace = "UTS";
          ਜਾਓ fail;
        }

      ਜੇ (strcmp (name, "kernel/hostname") == 0)
        ਵਾਪਸ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_ਕੁੰਜੀ);
    }
  ਜੇ (has_ਅਗੇਤਰ (name, "net/"))
    {
      ਜੇ (namespaces_created & CLONE_NEWNET)
        ਵਾਪਸ 0;

      namespace = "network";
      ਜਾਓ fail;
    }

  ਵਾਪਸ crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_ਕੁੰਜੀ);

fail:
  ਵਾਪਸ crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_ਕੁੰਜੀ, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
ਸਥਿਰ ਅੱਖਰ *
sysctl_error_reason (ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ namespaces_created, ਪੂਰਨ errno_)
{

  ਜੇ (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    ਵਾਪਸ xstrdup ("are all the IDs mapped in the user namespace?");

  ਵਾਪਸ ਨਿਰਾ;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close ਪੂਰਨ dirfd = -1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ਜੇ (def->linux == ਨਿਰਾ || def->linux->sysctl == ਨਿਰਾ || def->linux->sysctl->len == 0)
    ਵਾਪਸ 0;

  ਲਈ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ਪੂਰਨ ਮੁੱਲ;

      ਮੁੱਲ = libcrun_ਲੱਭੋ_namespace (def->linux->namespaces[i]->type);
      ਜੇ (UNLIKELY (ਮੁੱਲ < 0))
        ਵਾਪਸ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= ਮੁੱਲ;
    }

  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (dirfd < 0))
    ਵਾਪਸ dirfd;

  ਲਈ (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free ਅੱਖਰ *name = ਨਿਰਾ;
      cleanup_close ਪੂਰਨ fd = -1;
      ਪੂਰਨ ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      ਜੇ (def->linux->sysctl->ਕੁੰਜੀਆਂ[i] == ਨਿਰਾ || def->linux->sysctl->ਮੁੱਲਾਂ[i] == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_ਕੁੰਜੀ_ਮੰਜ਼ਿਲ_proc_ਰਸਤਾ (def->linux->sysctl->ਕੁੰਜੀਆਂ[i]);

      ret = validate_sysctl (def->linux->sysctl->ਕੁੰਜੀਆਂ[i], def->linux->sysctl->ਮੁੱਲਾਂ[i], name, namespaces_created, def, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ਜੇ (UNLIKELY (fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, def->linux->sysctl->ਮੁੱਲਾਂ[i], ਲੜੀ_ਲੰਬਾਈ (def->linux->sysctl->ਮੁੱਲਾਂ[i])));
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਪੂਰਨ saved_errno = errno;
          cleanup_free ਅੱਖਰ *reason = ਨਿਰਾ;

          reason = sysctl_error_reason (def->linux->sysctl->ਕੁੰਜੀਆਂ[i], namespaces_created, saved_errno);
          ਵਾਪਸ crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
open_terminal (ਅੱਖਰ **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ fd = -1;
  uid_t uid = 0;
  ਪੂਰਨ ret;

  ਜੇ (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ fd;

  ret = libcrun_ਸੈੱਟ_stdio (fd, *pty, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  ਵਾਪਸ ret;
}

ਅੱਖਰ *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  ਵਾਪਸ get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->external_descriptors;
}

ਪੂਰਨ
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *buf = ਨਿਰਾ;
  json_gen_ctx *gen = ਨਿਰਾ;
  size_t buf_len;
  ਪੂਰਨ ret;
  ਪੂਰਨ i;

  ਜੇ (! json_gen_init (&gen, ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  ਜੇ (UNLIKELY (ret != json_gen_ਸਥਿਤੀ_ok))
    ਜਾਓ json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  ਲਈ (i = 0; i < 3; i++)
    {
      proc_fd_ਰਸਤਾ_t fd_ਰਸਤਾ;
      ਅੱਖਰ link_ਰਸਤਾ[PATH_MAX];

      get_proc_self_fd_ਰਸਤਾ (fd_ਰਸਤਾ, i);

      ret = readlink (fd_ਰਸਤਾ, link_ਰਸਤਾ, PATH_MAX - 1);
      ਜੇ (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          ਜੇ (errno == ENOENT)
            {
              strcpy (link_ਰਸਤਾ, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          ਨਹੀਂਤਾਂ
            {
              json_gen_free (gen);
              ਵਾਪਸ crun_make_error (err, errno, "readlink `%s`", fd_ਰਸਤਾ);
            }
        }
      link_ਰਸਤਾ[ret] = 0;

      ret = json_gen_string (gen, link_ਰਸਤਾ, ret);
      ਜੇ (UNLIKELY (ret != json_gen_ਸਥਿਤੀ_ok))
        ਜਾਓ json_error;
    }

  ret = json_gen_array_close (gen);
  ਜੇ (UNLIKELY (ret != json_gen_ਸਥਿਤੀ_ok))
    ਜਾਓ json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  ਜੇ (UNLIKELY (ret != json_gen_ਸਥਿਤੀ_ok))
    ਜਾਓ json_error;

  ਜੇ (buf)
    {
      ਅੱਖਰ *b = xmalloc (buf_len + 1);
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (b, buf, buf_len);
      b[buf_len] = '\0';
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  ਵਾਪਸ 0;

json_error:
  ਜੇ (gen)
    json_gen_free (gen);
  ਵਾਪਸ json_gen_error_ਮੰਜ਼ਿਲ_crun_error (ret, err);
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ਅੱਖਰ *pty = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ fd = -1;
  ਪੂਰਨ ret;

  ਜੇ (def->process == ਨਿਰਾ || ! def->process->terminal)
    ਵਾਪਸ 0;

  fd = open_terminal (&pty, def->process, err);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ fd;

  ਜੇ (def->process->console_ਮਾਪ)
    {
      ret = libcrun_terminal_ਤਿਆਰੀ_ਮਾਪ (0, def->process->console_ਮਾਪ->ਉਚਾਈ, def->process->console_ਮਾਪ->ਚੌੜਾਈ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", ਨਿਰਾ, MS_BIND, ਨਿਰਾ, LABEL_MOUNT, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      ret = unlink ("/dev/console");
      ਜੇ (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          ਜੇ (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", ਨਿਰਾ, MS_BIND, ਨਿਰਾ, LABEL_MOUNT, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;

              ਵਾਪਸ get_and_reset (&fd);
            }
          ਵਾਪਸ crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  ਵਾਪਸ get_and_reset (&fd);
}

ਸਥਿਰ bool
read_error_from_sync_socket (ਪੂਰਨ sync_socket_fd, ਪੂਰਨ *error, ਅੱਖਰ **str)
{
  cleanup_free ਅੱਖਰ *b = ਨਿਰਾ;
  size_t ਮਾਪ;
  ਪੂਰਨ ਕੋਡ = 0;
  ਪੂਰਨ ret;

  ਜੇ (*error == 0)
    {
      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_socket_fd, &ਕੋਡ, ਆਕਾਰ (ਕੋਡ)));
      ਜੇ (UNLIKELY (ret != (ਪੂਰਨ) ਆਕਾਰ (ਕੋਡ)))
        ਵਾਪਸ false;
      *error = ਕੋਡ;
    }

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_socket_fd, &ਮਾਪ, ਆਕਾਰ (ਮਾਪ)));
  ਜੇ (UNLIKELY (ret != (ਪੂਰਨ) ਆਕਾਰ (ਮਾਪ)))
    ਵਾਪਸ false;

  ਜੇ (ਮਾਪ == 0)
    ਵਾਪਸ false;

  ਜੇ (ਮਾਪ > 1024)
    ਮਾਪ = 1024;

  b = xmalloc (ਮਾਪ + 1);
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_socket_fd, b, ਮਾਪ));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ false;

  b[ret] = '\0';

  *str = b;
  b = ਨਿਰਾ;
  ਵਾਪਸ true;
}

ਸਥਿਰ bool
send_error_ਮੰਜ਼ਿਲ_sync_socket (ਪੂਰਨ sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t ਮਾਪ;
  ਪੂਰਨ ret;
  ਪੂਰਨ ਕੋਡ;
  ਅੱਖਰ *ਸੁਨੇਹਾ;

  ਜੇ (err == ਨਿਰਾ || *err == ਨਿਰਾ)
    ਵਾਪਸ false;

  ਕੋਡ = crun_error_get_errno (err);

  ਜੇ (has_fd)
    {
      /* dummy terminal fd.  */
      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket_fd, "1", 1));
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ false;
    }

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket_fd, &ਕੋਡ, ਆਕਾਰ (ਕੋਡ)));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ false;

  ਸੁਨੇਹਾ = (*err)->ਸੁਨੇਹਾ;
  ਮਾਪ = ਲੜੀ_ਲੰਬਾਈ (ਸੁਨੇਹਾ) + 1;
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket_fd, &ਮਾਪ, ਆਕਾਰ (ਮਾਪ)));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ false;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket_fd, ਸੁਨੇਹਾ, ਮਾਪ));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ false;

  ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
send_success_ਮੰਜ਼ਿਲ_sync_socket (ਪੂਰਨ sync_socket, libcrun_error_t *err)
{
  ਅਚਲ ਪੂਰਨ success = 0;
  ਪੂਰਨ ret;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket, &success, ਆਕਾਰ (success)));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "write to sync socket");

  ਵਾਪਸ 0;
}

ਸਥਿਰ __attribute__ ((noreturn)) ਖਾਲੀ
send_error_ਮੰਜ਼ਿਲ_sync_socket_and_die (ਪੂਰਨ sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  ਅੱਖਰ *ਸੁਨੇਹਾ;

  ਜੇ (err == ਨਿਰਾ || *err == ਨਿਰਾ)
    ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);

  ਜੇ (! send_error_ਮੰਜ਼ਿਲ_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      ਸੁਨੇਹਾ = (*err)->ਸੁਨੇਹਾ;
      libcrun_fail_with_error (errno, "%s", ਸੁਨੇਹਾ);
    }
  ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
}

ਸਥਿਰ ਪੂਰਨ
expect_success_from_sync_socket (ਪੂਰਨ sync_fd, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *err_str = ਨਿਰਾ;
  ਪੂਰਨ res = 1;
  ਪੂਰਨ ret;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_fd, &res, ਆਕਾਰ (res)));
  ਜੇ (UNLIKELY (ret != ਆਕਾਰ (res)))
    ਵਾਪਸ crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  ਜੇ (res == 0)
    ਵਾਪਸ 0;

  ਜੇ (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      ਜੇ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (err_str))
        ਵਾਪਸ crun_make_error (err, res, "%s", err_str);
    }

  ਵਾਪਸ crun_make_error (err, res, "read from sync socket");
}

ਸਥਿਰ ਪੂਰਨ
join_namespaces (runtime_spec_schema_config_schema *def, ਪੂਰਨ *namespaces_ਮੰਜ਼ਿਲ_join, ਪੂਰਨ n_namespaces_ਮੰਜ਼ਿਲ_join,
                 ਪੂਰਨ *namespaces_ਮੰਜ਼ਿਲ_join_ਸੂਚਕਾਂਕ, bool ignore_join_errors, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਪੂਰਨ i;

  ਲਈ (i = 0; i < n_namespaces_ਮੰਜ਼ਿਲ_join; i++)
    {
      cleanup_free ਅੱਖਰ *cwd = ਨਿਰਾ;
      ਪੂਰਨ orig_ਸੂਚਕਾਂਕ = namespaces_ਮੰਜ਼ਿਲ_join_ਸੂਚਕਾਂਕ[i];
      ਪੂਰਨ ਮੁੱਲ;

      ਜੇ (namespaces_ਮੰਜ਼ਿਲ_join[i] < 0)
        ਜਾਰੀ;

      /* Skip the user namespace.  */
      ਮੁੱਲ = libcrun_ਲੱਭੋ_namespace (def->linux->namespaces[orig_ਸੂਚਕਾਂਕ]->type);
      ਜੇ (ਮੁੱਲ == CLONE_NEWUSER)
        ਜਾਰੀ;

      ਜੇ (ਮੁੱਲ == CLONE_NEWNS)
        {
          cwd = getcwd (ਨਿਰਾ, 0);
          ਜੇ (UNLIKELY (cwd == ਨਿਰਾ))
            ਵਾਪਸ crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_ਸੂਚਕਾਂਕ]->type, def->linux->namespaces[orig_ਸੂਚਕਾਂਕ]->ਰਸਤਾ);
      ret = setns (namespaces_ਮੰਜ਼ਿਲ_join[i], ਮੁੱਲ);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (ignore_join_errors)
            ਜਾਰੀ;
          ਵਾਪਸ crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_ਸੂਚਕਾਂਕ]->ਰਸਤਾ);
        }

      close_and_reset (&namespaces_ਮੰਜ਼ਿਲ_join[i]);

      ਜੇ (ਮੁੱਲ == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  ਵਾਪਸ 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_NAMESPACES 10

ਬਣਤਰ init_ਸਥਿਤੀ_s
{
  /* fd to the namespace to join.  */
  ਪੂਰਨ fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  ਪੂਰਨ ਸੂਚਕਾਂਕ[MAX_NAMESPACES];
  /* CLONE_* value.  */
  ਪੂਰਨ ਮੁੱਲ[MAX_NAMESPACES];
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
  ਪੂਰਨ userns_ਸੂਚਕਾਂਕ;
  /* def->linux->namespaces userns.  */
  ਪੂਰਨ userns_ਸੂਚਕਾਂਕ_origin;

  /* All namespaces created/joined by the container.  */
  ਪੂਰਨ all_namespaces;

  /* What namespaces are still missing to be created.  */
  ਪੂਰਨ namespaces_ਮੰਜ਼ਿਲ_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  ਪੂਰਨ idx_pidns_ਮੰਜ਼ਿਲ_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  ਪੂਰਨ idx_timens_ਮੰਜ਼ਿਲ_join_immediately;
};

ਖਾਲੀ
cleanup_free_init_statusp (ਬਣਤਰ init_ਸਥਿਤੀ_s *ns)
{
  size_t i;

  ਜੇ (ns == ਨਿਰਾ)
    ਵਾਪਸ;

  ਲਈ (i = 0; i < ns->fd_len; i++)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (ns->fd[i]));
}

ਸਥਿਰ ਪੂਰਨ
configure_init_ਸਥਿਤੀ (ਬਣਤਰ init_ਸਥਿਤੀ_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  ਲਈ (i = 0; i < MAX_NAMESPACES + 1; i++)
    ns->fd[i] = -1;

  ns->fd_len = 0;
  ns->all_namespaces = 0;
  ns->namespaces_ਮੰਜ਼ਿਲ_unshare = 0;
  ns->join_pidns = false;
  ns->join_ipcns = false;
  ns->must_fork = false;
  ns->can_delay_pidns_fork = false;
  ns->delayed_userns_create = false;
  ns->userns_ਸੂਚਕਾਂਕ = -1;
  ns->userns_ਸੂਚਕਾਂਕ_origin = -1;
  ns->idx_pidns_ਮੰਜ਼ਿਲ_join_immediately = -1;
  ns->idx_timens_ਮੰਜ਼ਿਲ_join_immediately = -1;

  ਲਈ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ਪੂਰਨ ਮੁੱਲ = libcrun_ਲੱਭੋ_namespace (def->linux->namespaces[i]->type);
      ਜੇ (UNLIKELY (ਮੁੱਲ < 0))
        ਵਾਪਸ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ਜੇ (ns->all_namespaces & ਮੁੱਲ)
        ਵਾਪਸ crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= ਮੁੱਲ;

      ਜੇ (def->linux->namespaces[i]->ਰਸਤਾ == ਨਿਰਾ)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_ਮੰਜ਼ਿਲ_unshare |= ਮੁੱਲ;
        }
      ਨਹੀਂਤਾਂ
        {
          ਪੂਰਨ fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->ਰਸਤਾ);

          ਜੇ (ns->fd_len >= MAX_NAMESPACES)
            ਵਾਪਸ crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->ਰਸਤਾ, O_RDONLY | O_CLOEXEC);
          ਜੇ (UNLIKELY (fd < 0))
            ਵਾਪਸ crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->ਰਸਤਾ);

          ਜੇ (ਮੁੱਲ == CLONE_NEWUSER)
            {
              ns->userns_ਸੂਚਕਾਂਕ = ns->fd_len;
              ns->userns_ਸੂਚਕਾਂਕ_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->ਸੂਚਕਾਂਕ[ns->fd_len] = i;
          ns->ਮੁੱਲ[ns->fd_len] = ਮੁੱਲ;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  ਜੇ (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_ਮੰਜ਼ਿਲ_unshare |= CLONE_NEWUSER;
    }

  ਵਾਪਸ 0;
}

/* Detect if root is available in the container.  */
ਸਥਿਰ bool
ਜੜ੍ਹ_mapped_ਵਿੱਚ_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  ਲਈ (i = 0; i < len; i++)
    ਜੇ (mappings[i]->container_id == 0)
      ਵਾਪਸ true;

  ਵਾਪਸ false;
}

ਸਥਿਰ ਬਣਤਰ libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  ਬਣਤਰ libcrun_fd_map *dev_fds = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->dev_fds;

  ਜੇ (dev_fds == ਨਿਰਾ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->dev_fds = dev_fds;
    }
  ਵਾਪਸ dev_fds;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅੰਕ_NEEDED_DEVS (ਆਕਾਰ (needed_devs) / ਆਕਾਰ (needed_devs[0]) - 1)

ਸਥਿਰ ਬਣਤਰ libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  ਬਣਤਰ libcrun_fd_map *fds = get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->needed_devs_fds;

  ਜੇ (fds == ਨਿਰਾ)
    {
      fds = make_libcrun_fd_map (ਅੰਕ_NEEDED_DEVS);
      get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->needed_devs_fds = fds;
    }
  ਵਾਪਸ fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  ਜੇ (recursive)
    *recursive = false;

  ਜੇ (src_nofollow != ਨਿਰਾ)
    *src_nofollow = false;

  ਲਈ (i = 0; i < mnt->options_len; i++)
    {
      ਜੇ (mnt->options[i] == ਨਿਰਾ)
        ਜਾਰੀ;

      ਜੇ (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ਜੇ (src_nofollow == ਨਿਰਾ || *src_nofollow)
            ਰੋਕੋ;
        }
      ਜੇ (strcmp (mnt->options[i], "rbind") == 0)
        {
          ਜੇ (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ਜੇ (src_nofollow == ਨਿਰਾ || *src_nofollow)
            ਰੋਕੋ;
        }
      ਜੇ (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਅੱਖਰ *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  ਲਈ (i = 0; i < mnt->options_len; i++)
    {
      ਜੇ (mnt->options[i] == ਨਿਰਾ)
        ਜਾਰੀ;

      ਜੇ (has_ਅਗੇਤਰ (mnt->options[i], "idmap"))
        {
          *recursive = false;
          ਵਾਪਸ mnt->options[i];
        }
      ਜੇ (has_ਅਗੇਤਰ (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          ਵਾਪਸ mnt->options[i];
        }
    }
  ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ
open_mount_type (ਅਚਲ ਅੱਖਰ *type, ਪੂਰਨ *ਬਾਹਰਲਾ_fd, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ fsopen_fd = -1;
  cleanup_close ਪੂਰਨ newfs_fd = -1;
  ਪੂਰਨ ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ਜੇ (UNLIKELY (fsopen_fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, ਨਿਰਾ, ਨਿਰਾ, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  ਜੇ (UNLIKELY (newfs_fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fsmount `%s`", type);

  *ਬਾਹਰਲਾ_fd = get_and_reset (&newfs_fd);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, ਪੂਰਨ *ਬਾਹਰਲਾ_fd, bool *has_mappings_ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  ਬਣਤਰ mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close ਪੂਰਨ fd = -1;
  ਅਚਲ ਅੱਖਰ *idmap_option;
  bool recursive = false;
  ਅਚਲ ਅੱਖਰ *options = ਨਿਰਾ;
  bool has_mappings;
  ਪੂਰਨ ret;
  ਅੱਖਰ *extra_ਸੁਨੇਹਾ = "";
  bool nofollow = false;

  *ਬਾਹਰਲਾ_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != ਨਿਰਾ);
  ਜੇ (has_mappings_ਬਾਹਰਲਾ)
    *has_mappings_ਬਾਹਰਲਾ = has_mappings;
  ਜੇ (! has_mappings)
    ਵਾਪਸ 0;

  ਜੇ ((mnt->uid_mappings == ਨਿਰਾ) != (mnt->gid_mappings == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  ਜੇ (idmap_option)
    {
      options = strchr (idmap_option, '=');
      ਜੇ (options)
        {
          /* Skip the '=' itself.  */
          options++;
          ਜੇ (options[0] == '\0')
            options = ਨਿਰਾ;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  ਜੇ (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ fd;

  ਜੇ (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_ਰੁੱਖ (AT_FDCWD, mnt->ਸਰੋਤ, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      ਜੇ (UNLIKELY (newfs_fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", mnt->ਸਰੋਤ);
    }
  ਨਹੀਂਤਾਂ
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_ਖਾਲੀ_ਥਾਵਾਂ_ਰਸਤਾ | (recursive ? AT_RECURSIVE : 0), &attr);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno == EINVAL)
        {
          extra_ਸੁਨੇਹਾ = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      ਵਾਪਸ crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_ਸੁਨੇਹਾ);
    }

  *ਬਾਹਰਲਾ_fd = get_and_reset (&newfs_fd);
  ਵਾਪਸ 0;
}

ਸਥਿਰ uid_t
get_id_ਵਿੱਚ_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  ਜੇ (! def || ! def->linux)
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  ਜੇ (! mappings)
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  ਲਈ (i = 0; i < len; i++)
    {
      ਜੇ (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->ਮਾਪ)
        ਵਾਪਸ id - mappings[i]->container_id + mappings[i]->host_id;
    }
ਬਾਹਰ_ਨਿਕਲੋ:
  ਵਾਪਸ is_uid ? get_ਉਛਾਲ_uid () : get_ਉਛਾਲ_gid ();
}

ਸਥਿਰ ਪੂਰਨ
precreate_device (libcrun_container_t *container, ਪੂਰਨ devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  ਅੱਖਰ name[64];
  mode_t type;
  dev_t dev;
  ਪੂਰਨ ret;

  ret = snprintf (name, ਆਕਾਰ (name), "%zu", i);
  ਜੇ (UNLIKELY (ret >= (ਪੂਰਨ) ਆਕਾਰ (name)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_ਬਹੁਲਕ | type, dev);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "mknod `%s`", device->ਰਸਤਾ);

  uid = get_id_ਵਿੱਚ_user_namespace (device->uid, true, def);
  gid = get_id_ਵਿੱਚ_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "chown `%s`", device->ਰਸਤਾ);

  ਵਾਪਸ get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

ਸਥਿਰ ਪੂਰਨ
send_mounts (ਪੂਰਨ sync_socket_host, ਬਣਤਰ libcrun_fd_map *fds, size_t how_many, size_t ਕੁੱਲ, libcrun_error_t *err)
{
  size_t i;
  ਪੂਰਨ ret;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket_host, &how_many, ਆਕਾਰ (how_many)));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "write to sync socket");

  ਲਈ (i = 0; i < ਕੁੱਲ; i++)
    {
      ਜੇ (fds->fds[i] >= 0)
        {
          ret = send_fd_ਮੰਜ਼ਿਲ_socket_with_payload (sync_socket_host, fds->fds[i], (ਅੱਖਰ *) &i, ਆਕਾਰ (i), err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, ਪੂਰਨ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ਬਣਤਰ libcrun_fd_map *mount_fds = ਨਿਰਾ;
  bool has_userns = (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free ਅੱਖਰ *rootfs = ਨਿਰਾ;
  size_t how_many = 0;
  size_t i;
  ਪੂਰਨ ret;

  ਜੇ (def->mounts_len == 0)
    ਵਾਪਸ 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  ਜੇ (def->ਜੜ੍ਹ && def->ਜੜ੍ਹ->ਰਸਤਾ)
    rootfs = realpath (def->ਜੜ੍ਹ->ਰਸਤਾ, ਨਿਰਾ);

  ਜੇ (! has_userns)
    {
      ਪੂਰਨ is_ਵਿੱਚ_userns = check_running_ਵਿੱਚ_user_namespace (err);
      ਜੇ (UNLIKELY (is_ਵਿੱਚ_userns < 0))
        ਵਾਪਸ is_ਵਿੱਚ_userns;

      has_userns = is_ਵਿੱਚ_userns > 0;
    }

  ਲਈ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      ਪੂਰਨ mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      ਜੇ (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_ਰਸਤਾ_ਵਿੱਚ_rootfs (rootfs, def->mounts[i]->ਸਰੋਤ) == ਨਿਰਾ)
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪ੍ਰਸਾਰ = 0;

          ਜੇ (def->linux && def->linux->rootfs_ਪ੍ਰਸਾਰ)
            ਪ੍ਰਸਾਰ = get_mount_flags (def->linux->rootfs_ਪ੍ਰਸਾਰ, 0, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ) & ALL_PROPAGATIONS_NO_REC;
          ਜੇ (ਪ੍ਰਸਾਰ == 0)
            ਪ੍ਰਸਾਰ = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->ਸਰੋਤ, recursive, false, nofollow, ਪ੍ਰਸਾਰ, err);
          ਜੇ (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      ਜੇ (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  ਵਾਪਸ send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

ਸਥਿਰ ਪੂਰਨ
prepare_and_send_dev_mounts (libcrun_container_t *container, ਪੂਰਨ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ਬਣਤਰ libcrun_fd_map *dev_fds = ਨਿਰਾ;
  bool has_userns = (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close ਪੂਰਨ current_mountns = -1;
  cleanup_free ਅੱਖਰ *state_dir = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *devs_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ devs_mountfd = -1;
  cleanup_close ਪੂਰਨ targetfd = -1;
  ਅਚਲ ਅੱਖਰ *context_type = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *label = ਨਿਰਾ;
  size_t how_many = 0;
  size_t i;
  ਪੂਰਨ ret;
  // To track whether the namespace has been changed.
  bool ns_ਬਦਲਿਆ = false;

  ਜੇ (def->linux == ਨਿਰਾ || def->linux->devices_len == 0)
    ਵਾਪਸ 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  ਜੇ (! has_userns || is_ਖਾਲੀ_ਥਾਵਾਂ_string (container->context->id) || geteuid () > 0)
    ਵਾਪਸ send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_ਜੜ੍ਹ : ਨਿਰਾ),
                                     (container->context ? container->context->id : ਨਿਰਾ), err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_paths (&devs_ਰਸਤਾ, err, state_dir, "devs", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = mkdir (devs_ਰਸਤਾ, 0700);
  ਜੇ (UNLIKELY (ret < 0 && errno != EEXIST))
    ਵਾਪਸ crun_make_error (err, errno, "mkdir `%s`", devs_ਰਸਤਾ);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  ਜੇ (UNLIKELY (current_mountns < 0))
    ਵਾਪਸ current_mountns;

  ret = unshare (CLONE_NEWNS);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_ਬਦਲਿਆ = true;

  ret = mount (ਨਿਰਾ, "/", ਨਿਰਾ, MS_REC | MS_PRIVATE, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      ਜਾਓ restore_mountns;
    }

  ਜੇ (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      ਜੇ (UNLIKELY (context_type == ਨਿਰਾ))
        {
          ret = -1;
          ਜਾਓ restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", ਨਿਰਾ, context_type, label, ਨਿਰਾ);
  ਜੇ (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      ਜਾਓ restore_mountns;
    }

  targetfd = open (devs_ਰਸਤਾ, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_ਰਸਤਾ);
      ਜਾਓ restore_mountns;
    }

  ret = fs_move_mount_ਮੰਜ਼ਿਲ (devs_mountfd, targetfd, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_ਰਸਤਾ);
      ਜਾਓ restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_ਰਸਤਾ);
      ਜਾਓ restore_mountns;
    }

  ਲਈ (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ਜਾਰੀ;
        }

      dev_fds->fds[i] = ret;

      ਜੇ (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  ਜੇ (ns_ਬਦਲਿਆ && current_mountns >= 0)
    {
      ਪੂਰਨ setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      ਜੇ (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          ਵਾਪਸ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, ਪੂਰਨ sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map ਬਣਤਰ libcrun_fd_map *fds = ਨਿਰਾ;
  bool has_userns = (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (ਅੰਕ_NEEDED_DEVS);

  ਜੇ (! has_userns || geteuid () > 0)
    ਵਾਪਸ send_mounts (sync_socket_host, fds, 0, ਅੰਕ_NEEDED_DEVS, err);

  ਲਈ (i = 0; i < ਅੰਕ_NEEDED_DEVS && needed_devs[i].ਰਸਤਾ; i++)
    {
      ਪੂਰਨ fd = syscall_open_ਰੁੱਖ (AT_FDCWD, needed_devs[i].ਰਸਤਾ,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      ਜੇ (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  ਵਾਪਸ send_mounts (sync_socket_host, fds, how_many, ਅੰਕ_NEEDED_DEVS, err);
}

ਸਥਿਰ ਪੂਰਨ
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, ਪੂਰਨ sync_socket_host, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
receive_mounts (ਬਣਤਰ libcrun_fd_map *fds, ਪੂਰਨ sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  ਪੂਰਨ ret;

  ਜੇ (fds->nfds == 0)
    ਵਾਪਸ 0;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_socket_container, &how_many, ਆਕਾਰ (how_many)));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "read from sync socket");

  ਲਈ (i = 0; i < how_many; i++)
    {
      size_t ਸੂਚਕਾਂਕ;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (ਅੱਖਰ *) &ਸੂਚਕਾਂਕ, ਆਕਾਰ (ਸੂਚਕਾਂਕ), err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      ਜੇ (ਸੂਚਕਾਂਕ >= fds->nfds)
        ਵਾਪਸ crun_make_error (err, 0, "invalid mount data received");

      ਜੇ (fds->fds[ਸੂਚਕਾਂਕ] >= 0)
        ਆਰਜ਼ੀ_FAILURE_RETRY (close (fds->fds[ਸੂਚਕਾਂਕ]));

      fds->fds[ਸੂਚਕਾਂਕ] = ret;
    }

  ਵਾਪਸ 0;
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
ਸਥਿਰ ਪੂਰਨ
ਸੈੱਟ_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ਜੇ (! container->host_has_cap_sys_ptrace)
    ਵਾਪਸ 0;

  ret = prctl (PR_ਸੈੱਟ_DUMPABLE, 0, 0, 0, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਸੈੱਟ_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  ਪੂਰਨ ret;

  ਜੇ (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool ਜੜ੍ਹ_mapped = false;

      ਜੇ (def->linux->uid_mappings_len != 0)
        {
          ਜੜ੍ਹ_mapped = ਜੜ੍ਹ_mapped_ਵਿੱਚ_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          ਜੇ (! ਜੜ੍ਹ_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      ਜੇ (def->linux->gid_mappings_len != 0)
        {
          ਜੜ੍ਹ_mapped = ਜੜ੍ਹ_mapped_ਵਿੱਚ_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          ਜੇ (! ਜੜ੍ਹ_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  ਜੇ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ਵਾਪਸ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "setresgid to `%d`", gid);

  ਵਾਪਸ 0;
}

ਸਥਿਰ bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  ਵਾਪਸ mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, ਨਿਰਾ, ਨਿਰਾ);
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
ਸਥਿਰ ਪੂਰਨ
open_proc_mounts_ਵਿੱਚ_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਬਣਤਰ libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ਪੂਰਨ socket_ਮਾਪਾ = -1;
  cleanup_close ਪੂਰਨ socket_ਬੱਚਾ = -1;
  cleanup_close ਪੂਰਨ ਜੜ੍ਹ_fd = -1;
  cleanup_close ਪੂਰਨ cwd_fd = -1;
  size_t i, missing = 0;
  ਪੂਰਨ saved_errno;
  ਪੂਰਨ sockets[2];
  pid_t pid;
  ਪੂਰਨ ret;

  ਜੇ (def->ਜੜ੍ਹ == ਨਿਰਾ || def->ਜੜ੍ਹ->ਰਸਤਾ == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਲਈ (i = 0; i < def->mounts_len; i++)
    ਜੇ (is_proc_mount (def->mounts[i]))
      missing++;
  ਜੇ (missing == 0)
    ਵਾਪਸ 1;

  ਜੜ੍ਹ_fd = open ("/", O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (ਜੜ੍ਹ_fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (cwd_fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "socketpair");
  socket_ਮਾਪਾ = sockets[0];
  socket_ਬੱਚਾ = sockets[1];

  ret = chroot (def->ਜੜ੍ਹ->ਰਸਤਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ 0;

  pid = fork ();
  saved_errno = errno;
  ਜੇ (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      ਜੇ (get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags & CLONE_NEWUSER)
        (ਖਾਲੀ) unshare (CLONE_NEWNS);

      ਲਈ (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close ਪੂਰਨ fd = -1;

          ਜੇ (! is_proc_mount (def->mounts[i]))
            ਜਾਰੀ;

          fd = open_mount_ਦਾ_type (container, def->mounts[i], err);
          ਜੇ (fd < 0)
            ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);

          ret = send_fd_ਮੰਜ਼ਿਲ_socket_with_payload (socket_ਬੱਚਾ, fd, (ਅੱਖਰ *) &i, ਆਕਾਰ (i), err);
          ਜੇ (UNLIKELY (ret < 0))
            ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
        }
      ਸੁਰੱਖਿਅਤ_exit (EXIT_SUCCESS);
    }

  ਜੇ (UNLIKELY (fchdir (ਜੜ੍ਹ_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    ਵਾਪਸ crun_make_error (err, errno, "restore the root directory");

  ਜੇ (UNLIKELY (pid < 0))
    ਵਾਪਸ crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_ਬੱਚਾ);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  ਜਦੋਂ (missing > 0)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;

      ret = receive_fd_from_socket_with_payload (socket_ਮਾਪਾ, (ਅੱਖਰ *) &i, ਆਕਾਰ (i), &tmp_err);
      ਜੇ (ret < 0)
        {
          crun_error_release (&tmp_err);
          ਰੋਕੋ;
        }
      ਜੇ (i >= mount_fds->nfds)
        {
          ਆਰਜ਼ੀ_FAILURE_RETRY (close (ret));
          ਰੋਕੋ;
        }

      ਜੇ (mount_fds->fds[i] >= 0)
        ਆਰਜ਼ੀ_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, ਨਿਰਾ, 0);

  ਵਾਪਸ missing == 0 ? 1 : 0;
}

ਸਥਿਰ ਪੂਰਨ
init_container (libcrun_container_t *container, ਪੂਰਨ sync_socket_container, ਬਣਤਰ init_ਸਥਿਤੀ_s *init_ਸਥਿਤੀ,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਬਣਤਰ libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ਪੂਰਨ timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  ਪੂਰਨ ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = ਸੈੱਟ_init_non_dumpable (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (init_ਸਥਿਤੀ->idx_pidns_ਮੰਜ਼ਿਲ_join_immediately >= 0 || init_ਸਥਿਤੀ->idx_timens_ਮੰਜ਼ਿਲ_join_immediately >= 0)
    {
      pid_t new_pid;

      ਜੇ (init_ਸਥਿਤੀ->idx_pidns_ਮੰਜ਼ਿਲ_join_immediately >= 0)
        {
          ret = setns (init_ਸਥਿਤੀ->fd[init_ਸਥਿਤੀ->idx_pidns_ਮੰਜ਼ਿਲ_join_immediately], CLONE_NEWPID);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_ਸਥਿਤੀ->fd[init_ਸਥਿਤੀ->idx_pidns_ਮੰਜ਼ਿਲ_join_immediately]);
        }

      ਜੇ (init_ਸਥਿਤੀ->idx_timens_ਮੰਜ਼ਿਲ_join_immediately >= 0)
        {
          ret = setns (init_ਸਥਿਤੀ->fd[init_ਸਥਿਤੀ->idx_timens_ਮੰਜ਼ਿਲ_join_immediately], CLONE_NEWTIME);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_ਸਥਿਤੀ->fd[init_ਸਥਿਤੀ->idx_timens_ਮੰਜ਼ਿਲ_join_immediately]);
        }

      ਜੇ (init_ਸਥਿਤੀ->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_ਵਿੱਚ_pidns (container, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          delayed_pidns_fork = ret > 0;
        }

      ਜੇ (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_ਮੰਜ਼ਿਲ_sync_socket (sync_socket_container, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket_container, &new_pid, ਆਕਾਰ (new_pid)));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "write to sync socket");
        }
      ਨਹੀਂਤਾਂ
        {
          new_pid = fork ();
          ਜੇ (UNLIKELY (new_pid < 0))
            ਵਾਪਸ crun_make_error (err, errno, "fork");
        }

      ਜੇ (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_ਮੰਜ਼ਿਲ_sync_socket (sync_socket_container, err);
          ਜੇ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket_container, &new_pid, ਆਕਾਰ (new_pid)));
          ਜੇ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ਸੁਰੱਖਿਅਤ_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = libcrun_ਸੈੱਟ_oom (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (init_ਸਥਿਤੀ->fd_len > 0)
    {
      ret = join_namespaces (def, init_ਸਥਿਤੀ->fd, init_ਸਥਿਤੀ->fd_len, init_ਸਥਿਤੀ->ਸੂਚਕਾਂਕ, true, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  ਜੇ ((init_ਸਥਿਤੀ->all_namespaces & CLONE_NEWUSER) && (init_ਸਥਿਤੀ->join_pidns || init_ਸਥਿਤੀ->join_ipcns))
    {
      ਲਈ (i = 0; i < def->mounts_len; i++)
        {
          ਪੂਰਨ fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          ਜੇ (def->mounts[i]->type == ਨਿਰਾ)
            ਜਾਰੀ;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          ਜੇ (init_ਸਥਿਤੀ->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->ਸਰੋਤ, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
          ਜੇ (init_ਸਥਿਤੀ->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->ਸਰੋਤ, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);

          ਜੇ (fd >= 0)
            {
              ਜੇ (mount_fds->fds[i] >= 0)
                ਆਰਜ਼ੀ_FAILURE_RETRY (close (mount_fds->fds[i]));
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
  ਜੇ (def->linux->ਸਮਾਂ_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      ਜੇ (UNLIKELY (timens_offsets_fd < 0))
        ਵਾਪਸ timens_offsets_fd;
    }

  ਜੇ (init_ਸਥਿਤੀ->all_namespaces & CLONE_NEWUSER)
    {
      ਜੇ (init_ਸਥਿਤੀ->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "unshare user namespace");

          init_ਸਥਿਤੀ->namespaces_ਮੰਜ਼ਿਲ_unshare &= ~CLONE_NEWUSER;

          ret = send_success_ਮੰਜ਼ਿਲ_sync_socket (sync_socket_container, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ਜੇ (init_ਸਥਿਤੀ->userns_ਸੂਚਕਾਂਕ < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
      ਨਹੀਂਤਾਂ
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_ਸਥਿਤੀ->fd[init_ਸਥਿਤੀ->userns_ਸੂਚਕਾਂਕ], CLONE_NEWUSER);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_ਸਥਿਤੀ->userns_ਸੂਚਕਾਂਕ_origin]->ਰਸਤਾ);
        }

      ret = ਸੈੱਟ_id_init (container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = ਸੈੱਟ_init_non_dumpable (container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = join_namespaces (def, init_ਸਥਿਤੀ->fd, init_ਸਥਿਤੀ->fd_len, init_ਸਥਿਤੀ->ਸੂਚਕਾਂਕ, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (init_ਸਥਿਤੀ->namespaces_ਮੰਜ਼ਿਲ_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_ਸਥਿਤੀ->namespaces_ਮੰਜ਼ਿਲ_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "unshare");
    }

  ਜੇ (def->linux->ਸਮਾਂ_offsets)
    {
      ਅੱਖਰ fmt_ਬਫਰ[128];
      ਪੂਰਨ fd = timens_offsets_fd;

      ਜੇ (def->linux->ਸਮਾਂ_offsets->boottime)
        {
          ret = snprintf (fmt_ਬਫਰ, ਆਕਾਰ (fmt_ਬਫਰ), "boottime %" PRIi64 " %" PRIu32, def->linux->ਸਮਾਂ_offsets->boottime->secs, def->linux->ਸਮਾਂ_offsets->boottime->nanosecs);
          ਜੇ (UNLIKELY (ret >= (ਪੂਰਨ) ਆਕਾਰ (fmt_ਬਫਰ)))
            ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ) def->linux->ਸਮਾਂ_offsets->boottime->secs, def->linux->ਸਮਾਂ_offsets->boottime->nanosecs);
          ret = ਲਿਖ (fd, fmt_ਬਫਰ, ਲੜੀ_ਲੰਬਾਈ (fmt_ਬਫਰ));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      ਜੇ (def->linux->ਸਮਾਂ_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ) def->linux->ਸਮਾਂ_offsets->monotonic->secs, def->linux->ਸਮਾਂ_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_ਬਫਰ, ਆਕਾਰ (fmt_ਬਫਰ), "monotonic %" PRIi64 " %" PRIu32, def->linux->ਸਮਾਂ_offsets->monotonic->secs, def->linux->ਸਮਾਂ_offsets->monotonic->nanosecs);
          ਜੇ (UNLIKELY (ret >= (ਪੂਰਨ) ਆਕਾਰ (fmt_ਬਫਰ)))
            ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = ਲਿਖ (fd, fmt_ਬਫਰ, ਲੜੀ_ਲੰਬਾਈ (fmt_ਬਫਰ));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  ਜੇ (init_ਸਥਿਤੀ->must_fork)
    {
      ਜੇ (init_ਸਥਿਤੀ->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_ਵਿੱਚ_pidns (container, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          delayed_pidns_fork = ret > 0;
        }

      ਜੇ (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          ਜੇ (UNLIKELY (pid_container < 0))
            ਵਾਪਸ crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      ਜੇ (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_ਮੰਜ਼ਿਲ_sync_socket (sync_socket_container, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket_container, &pid_container, ਆਕਾਰ (pid_container)));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "write to sync socket");

          ਜੇ (pid_container)
            ਸੁਰੱਖਿਅਤ_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_ਮੰਜ਼ਿਲ_sync_socket (sync_socket_container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  ਵਾਪਸ get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->delayed_pidns_fork;
}

ਪੂਰਨ
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ client_fd = -1;
  cleanup_close ਪੂਰਨ pidfd = -1;
  ਅਚਲ ਅੱਖਰ *v;

  v = ਲੱਭੋ_annotation (container, "run.oci.pidfd_receiver");
  ਜੇ (v == ਨਿਰਾ)
    ਵਾਪਸ 0;

  pidfd = syscall_pidfd_open (pid, 0);
  ਜੇ (UNLIKELY (pidfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  ਜੇ (UNLIKELY (client_fd < 0))
    ਵਾਪਸ client_fd;

  ਵਾਪਸ send_fd_ਮੰਜ਼ਿਲ_socket (client_fd, pidfd, err);
}

ਸਥਿਰ bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  ਜੇ (process == ਨਿਰਾ || process->exec_cpu_affinity == ਨਿਰਾ)
    ਵਾਪਸ false;
  ਵਾਪਸ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (process->exec_cpu_affinity->initial))
         || (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_ਲੜੀਵਾਰ_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, ਖਾਲੀ *ਦਲੀਲਾਂ,
                             ਪੂਰਨ *sync_socket_ਬਾਹਰਲਾ, ਬਣਤਰ libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) ਬਣਤਰ init_ਸਥਿਤੀ_s init_ਸਥਿਤੀ;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ਪੂਰਨ sync_socket_container = -1;
  ਅੱਖਰ *notify_socket_env = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ sync_socket_host = -1;
  __attribute__ ((ਅਣਵਰਤਿਆ)) cleanup_close ਪੂਰਨ restore_pidns = -1;
  ਪੂਰਨ ਪਹਿਲਾ_clone_ਦਲੀਲਾਂ = 0;
  ਪੂਰਨ sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  ਪੂਰਨ ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_ਸਥਿਤੀ (&init_ਸਥਿਤੀ, container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_flags = init_ਸਥਿਤੀ.all_namespaces;
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->joined_mount_ns
      = (init_ਸਥਿਤੀ.all_namespaces & CLONE_NEWNS)
        && ! (init_ਸਥਿਤੀ.namespaces_ਮੰਜ਼ਿਲ_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_ਸਥਿਤੀ.all_namespaces &= ~CLONE_NEWCGROUP;
  get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->unshare_cgroupns = init_ਸਥਿਤੀ.namespaces_ਮੰਜ਼ਿਲ_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
  ਜੇ (def->ਜੜ੍ਹ)
    {
      ret = do_notify_socket (container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
#ਜੇ_ਅੰਤ

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  ਜੇ (def->process)
    {
      ret = libcrun_ਸੈੱਟ_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = libcrun_ਸੈੱਟ_oom (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_container_notify_handler (ਦਲੀਲਾਂ, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->ਜੜ੍ਹ ? container->container_def->ਜੜ੍ਹ->ਰਸਤਾ : ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_ਸਥਿਤੀ.delayed_userns_create
      = (init_ਸਥਿਤੀ.all_namespaces & CLONE_NEWUSER) && init_ਸਥਿਤੀ.userns_ਸੂਚਕਾਂਕ < 0 && init_ਸਥਿਤੀ.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_ਸਥਿਤੀ.all_namespaces & CLONE_NEWUSER) && init_ਸਥਿਤੀ.userns_ਸੂਚਕਾਂਕ < 0;
  ਲਈ (i = 0; i < init_ਸਥਿਤੀ.fd_len; i++)
    {
      ਚੋਣ (init_ਸਥਿਤੀ.ਮੁੱਲ[i])
        {
        ਹਾਲਤ CLONE_NEWIPC:
          ਜੇ (init_ਸਥਿਤੀ.all_namespaces & CLONE_NEWUSER)
            init_ਸਥਿਤੀ.join_ipcns = true;
          ਰੋਕੋ;

        ਹਾਲਤ CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_ਸਥਿਤੀ.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          ਜੇ (! new_userns)
            init_ਸਥਿਤੀ.must_fork = true;
          ਨਹੀਂਤਾਂ
            {
              init_ਸਥਿਤੀ.join_pidns = true;
              init_ਸਥਿਤੀ.idx_pidns_ਮੰਜ਼ਿਲ_join_immediately = i;
              init_ਸਥਿਤੀ.namespaces_ਮੰਜ਼ਿਲ_unshare &= ~CLONE_NEWPID;
            }
          ਰੋਕੋ;

        ਹਾਲਤ CLONE_NEWTIME:
          ਜੇ (! new_userns)
            init_ਸਥਿਤੀ.must_fork = true;
          ਨਹੀਂਤਾਂ
            {
              init_ਸਥਿਤੀ.idx_timens_ਮੰਜ਼ਿਲ_join_immediately = i;
              init_ਸਥਿਤੀ.namespaces_ਮੰਜ਼ਿਲ_unshare &= ~CLONE_NEWTIME;
            }
          ਰੋਕੋ;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  ਜੇ ((init_ਸਥਿਤੀ.namespaces_ਮੰਜ਼ਿਲ_unshare & CLONE_NEWUSER) && init_ਸਥਿਤੀ.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      ਪਹਿਲਾ_clone_ਦਲੀਲਾਂ = CLONE_NEWUSER;
    }
  ਨਹੀਂਤਾਂ ਜੇ ((init_ਸਥਿਤੀ.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      ਪਹਿਲਾ_clone_ਦਲੀਲਾਂ = init_ਸਥਿਤੀ.namespaces_ਮੰਜ਼ਿਲ_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_ਸਥਿਤੀ.namespaces_ਮੰਜ਼ਿਲ_unshare &= ~ਪਹਿਲਾ_clone_ਦਲੀਲਾਂ;

  /* Check if there are still namespaces that require a fork().  */
  ਜੇ (init_ਸਥਿਤੀ.namespaces_ਮੰਜ਼ਿਲ_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      ਪਹਿਲਾ_clone_ਦਲੀਲਾਂ &= ~CLONE_NEWPID;
      init_ਸਥਿਤੀ.namespaces_ਮੰਜ਼ਿਲ_unshare |= CLONE_NEWPID;
      init_ਸਥਿਤੀ.must_fork = true;
      init_ਸਥਿਤੀ.can_delay_pidns_fork = false;
    }

  pid = -1;
  ਜੇ (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      ਬਣਤਰ clone3_ਦਲੀਲਾਂ clone3_ਦਲੀਲਾਂ_2;
      memset (&clone3_ਦਲੀਲਾਂ_2, 0, ਆਕਾਰ (clone3_ਦਲੀਲਾਂ_2));
      clone3_ਦਲੀਲਾਂ_2.exit_signal = SIGCHLD;
      clone3_ਦਲੀਲਾਂ_2.flags = ਪਹਿਲਾ_clone_ਦਲੀਲਾਂ;

      clone3_ਦਲੀਲਾਂ_2.flags |= CLONE_INTO_CGROUP;
      clone3_ਦਲੀਲਾਂ_2.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_ਦਲੀਲਾਂ_2);
      ਜੇ (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  ਜੇ (pid < 0)
    {
      pid = syscall_clone (ਪਹਿਲਾ_clone_ਦਲੀਲਾਂ | SIGCHLD, ਨਿਰਾ);
      ਜੇ (UNLIKELY (pid < 0))
        ਵਾਪਸ crun_make_error (err, errno, "clone");
    }

  ਜੇ (pid)
    {
      __attribute__ ((ਅਣਵਰਤਿਆ)) cleanup_pid pid_t pid_ਮੰਜ਼ਿਲ_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = close_and_reset (&sync_socket_container);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->notify_socket_ਰੁੱਖ_fd));

      ਜੇ (init_ਸਥਿਤੀ.idx_pidns_ਮੰਜ਼ਿਲ_join_immediately >= 0 || init_ਸਥਿਤੀ.idx_timens_ਮੰਜ਼ਿਲ_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_socket_host, &new_pid, ਆਕਾਰ (new_pid)));
          ਜੇ (UNLIKELY (ret != ਆਕਾਰ (new_pid)))
            ਵਾਪਸ crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          ਜੇ (new_pid == 0)
            get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->delayed_pidns_fork = true;
          ਨਹੀਂਤਾਂ
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, ਨਿਰਾ, 0);

              pid_ਮੰਜ਼ਿਲ_clean = pid = new_pid;
            }

          ret = send_success_ਮੰਜ਼ਿਲ_sync_socket (sync_socket_host, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ਜੇ (init_ਸਥਿਤੀ.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ਜੇ ((init_ਸਥਿਤੀ.all_namespaces & CLONE_NEWUSER) && init_ਸਥਿਤੀ.userns_ਸੂਚਕਾਂਕ < 0)
        {
          ret = libcrun_ਸੈੱਟ_usernamespace (container, pid, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          ret = send_success_ਮੰਜ਼ਿਲ_sync_socket (sync_socket_host, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ਜੇ (init_ਸਥਿਤੀ.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_socket_host, &grandchild, ਆਕਾਰ (grandchild)));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_ਮੰਜ਼ਿਲ_sync_socket (sync_socket_host, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          ਜੇ (grandchild == 0)
            get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->delayed_pidns_fork = true;
          ਨਹੀਂਤਾਂ
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, ਨਿਰਾ, 0);

              pid_ਮੰਜ਼ਿਲ_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      *sync_socket_ਬਾਹਰਲਾ = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      ਜੇ (! get_ਨਿੱਜੀ_ਅੰਕੜੇ (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      pid_ਮੰਜ਼ਿਲ_clean = 0;
      ਵਾਪਸ pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  ਜੇ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    send_error_ਮੰਜ਼ਿਲ_sync_socket_and_die (sync_socket_container, false, err);
  ਨਹੀਂਤਾਂ
    {
      ret = send_success_ਮੰਜ਼ਿਲ_sync_socket (sync_socket_container, err);
      ਜੇ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->ਸੁਨੇਹਾ);
    }

  /* Jump into the specified entrypoint.  */
  ਜੇ (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (ਦਲੀਲਾਂ, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  ਜੇ (*err)
    libcrun_fail_with_error ((*err)->ਸਥਿਤੀ, "%s", (*err)->ਸੁਨੇਹਾ);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  ਜੇ (ret == 0)
    ਸੁਰੱਖਿਅਤ_exit (EXIT_SUCCESS);

  ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
}

ਸਥਿਰ ਪੂਰਨ
join_process_ਮਾਪਾ_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t ਬੱਚਾ_pid, ਪੂਰਨ sync_socket_fd,
                            libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ,
                            bool need_move_ਮੰਜ਼ਿਲ_cgroup, ਅਚਲ ਅੱਖਰ *sub_cgroup,
                            ਪੂਰਨ *terminal_fd, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਅੱਖਰ res;
  pid_t pid;
  cleanup_close ਪੂਰਨ sync_fd = sync_socket_fd;

  ਜੇ (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_fd, &res, 1));
  ਜੇ (UNLIKELY (ret != 1))
    ਵਾਪਸ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  ਜੇ (res != '0')
    ਵਾਪਸ crun_make_error (err, 0, "fail startup");

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_fd, &pid, ਆਕਾਰ (pid)));
  ਜੇ (UNLIKELY (ret != ਆਕਾਰ (pid)))
    ਵਾਪਸ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (ਬੱਚਾ_pid, ਨਿਰਾ, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "waitpid for exec child pid");

  ਜੇ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_ਸੈੱਟ_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_ਨਕਾਬ (pid, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (need_move_ਮੰਜ਼ਿਲ_cgroup)
    {
      cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ = libcrun_cgroup_make_ਸਥਿਤੀ (ਸਥਿਤੀ);
      cleanup_free ਅੱਖਰ *final_cgroup = ਨਿਰਾ;

      ਜੇ (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, ਸਥਿਤੀ->cgroup_ਰਸਤਾ, sub_cgroup, ਨਿਰਾ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_ਸਥਿਤੀ, final_cgroup ?: ਸਥਿਤੀ->cgroup_ਰਸਤਾ, pid, ਸਥਿਤੀ->pid, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_ਸੈੱਟ_scheduler (pid, process, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_ਸੈੱਟ_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_ਸੈੱਟ_io_ਤਰਜੀਹ (pid, process, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_fd, &ret, ਆਕਾਰ (ret)));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "write to sync socket");

  ਜੇ (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਪੂਰਨ err_ਕੋਡ = 0;
          cleanup_free ਅੱਖਰ *err_str = ਨਿਰਾ;

          ਜੇ (read_error_from_sync_socket (sync_fd, &err_ਕੋਡ, &err_str))
            {
              ਜੇ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (err_str))
                {
                  crun_error_release (err);
                  ਵਾਪਸ crun_make_error (err, err_ਕੋਡ, "%s", err_str);
                }
            }

          ਵਾਪਸ crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  ਵਾਪਸ pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
ਸਥਿਰ ਪੂਰਨ
try_setns_with_pidfd (pid_t pid_ਮੰਜ਼ਿਲ_join, libcrun_container_t *container, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ਪੂਰਨ pidfd_pid_ਮੰਜ਼ਿਲ_join = -1;
  ਪੂਰਨ all_flags = 0;
  size_t i;
  ਪੂਰਨ ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  ਜੇ (def->linux && def->linux->namespaces)
    {
      ਲਈ (i = 0; i < def->linux->namespaces_len; i++)
        ਜੇ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (def->linux->namespaces[i]->ਰਸਤਾ))
          ਵਾਪਸ 0;
    }

  pidfd_pid_ਮੰਜ਼ਿਲ_join = syscall_pidfd_open (pid_ਮੰਜ਼ਿਲ_join, 0);
  ਜੇ (UNLIKELY (pidfd_pid_ਮੰਜ਼ਿਲ_join < 0))
    ਵਾਪਸ 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  ਜੇ (ret == 0)
    ਵਾਪਸ crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  ਲਈ (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].ਮੁੱਲ;

  ret = setns (pidfd_pid_ਮੰਜ਼ਿਲ_join, all_flags);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ 0;

  ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ
join_process_namespaces (libcrun_container_t *container, pid_t pid_ਮੰਜ਼ਿਲ_join, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਪੂਰਨ fds_joined[MAX_NAMESPACES] = {
    0,
  };
  ਪੂਰਨ fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  ਪੂਰਨ ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_ਮੰਜ਼ਿਲ_join, container, ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  /* Nothing left to do if the namespaces were joined.  */
  ਜੇ (LIKELY (ret > 0))
    ਵਾਪਸ 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  ਜੇ (def->linux->namespaces_len >= MAX_NAMESPACES)
    ਵਾਪਸ crun_make_error (err, 0, "invalid configuration");

  ਲਈ (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free ਅੱਖਰ *ns_ਰਸਤਾ = ਨਿਰਾ;

      xasprintf (&ns_ਰਸਤਾ, "%d/ns/%s", pid_ਮੰਜ਼ਿਲ_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_ਰਸਤਾ, O_RDONLY, err);
      ਜੇ (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          ਜੇ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ਜਾਰੀ;
            }

          ret = fds[i];
          ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
  ਜੇ (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ਲਈ (i = 0; namespaces[i].ns_file; i++)
    {
      ਜੇ (namespaces[i].ਮੁੱਲ == CLONE_NEWUSER)
        ਜਾਰੀ;

      ret = setns (fds[i], 0);
      ਜੇ (ret == 0)
        fds_joined[i] = 1;
    }
  ਲਈ (i = 0; namespaces[i].ns_file; i++)
    {
      ਜੇ (fds_joined[i])
        ਜਾਰੀ;
      ret = setns (fds[i], 0);
      ਜੇ (ret == 0)
        fds_joined[i] = 1;
    }
  ਲਈ (i = 0; namespaces[i].ns_file; i++)
    {
      ਜੇ (fds_joined[i])
        ਜਾਰੀ;
      ret = setns (fds[i], 0);
      ਜੇ (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool ਮਿਲਿਆ = false;

          ਲਈ (j = 0; j < def->linux->namespaces_len; j++)
            {
              ਜੇ (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  ਮਿਲਿਆ = true;
                  ਰੋਕੋ;
                }
            }
          ਜੇ (! ਮਿਲਿਆ)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              ਜਾਰੀ;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

ਬਾਹਰ_ਨਿਕਲੋ:
  ਲਈ (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_ਮੰਜ਼ਿਲ_join,
                      libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ,
                      ਅਚਲ ਅੱਖਰ *sub_cgroup,
                      ਪੂਰਨ detach,
                      runtime_spec_schema_config_schema_process *process,
                      ਪੂਰਨ *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  ਪੂਰਨ ret;
  ਪੂਰਨ sync_socket_fd[2];
  cleanup_close ਪੂਰਨ cgroup_dirfd = -1;
  cleanup_close ਪੂਰਨ sync_fd = -1;
  ਬਣਤਰ clone3_ਦਲੀਲਾਂ clone3_ਦਲੀਲਾਂ_2;
  bool need_move_ਮੰਜ਼ਿਲ_cgroup;

  ਜੇ (! detach)
    {
      ret = prctl (PR_ਸੈੱਟ_ਬੱਚਾ_SUBREAPER, 1, 0, 0, 0);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "set child subreaper");
    }

  ਜੇ (ਰਸਤਾ_has_dot_dot_component (sub_cgroup))
    ਵਾਪਸ crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  ਜੇ (sub_cgroup && ! is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਸਥਿਤੀ->cgroup_ਰਸਤਾ))
    {
      cleanup_free ਅੱਖਰ *final_cgroup = ਨਿਰਾ;

      ret = append_paths (&final_cgroup, err, ਸਥਿਤੀ->cgroup_ਰਸਤਾ, sub_cgroup, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ = ਨਿਰਾ;

    cgroup_ਸਥਿਤੀ = libcrun_cgroup_make_ਸਥਿਤੀ (ਸਥਿਤੀ);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    ਜੇ (get_force_cgroup_v1_annotation (container) == ਨਿਰਾ)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_ਸਥਿਤੀ, sub_cgroup, err);
        ਜੇ (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_ਦਲੀਲਾਂ_2, 0, ਆਕਾਰ (clone3_ਦਲੀਲਾਂ_2));
  clone3_ਦਲੀਲਾਂ_2.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  ਜੇ (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_ਮੰਜ਼ਿਲ_cgroup = true;
  ਨਹੀਂਤਾਂ
    {
      need_move_ਮੰਜ਼ਿਲ_cgroup = false;
      clone3_ਦਲੀਲਾਂ_2.flags |= CLONE_INTO_CGROUP;
      clone3_ਦਲੀਲਾਂ_2.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_ਦਲੀਲਾਂ_2);

  ਜੇ (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_ਸੈੱਟ_scheduler (pid, process, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  /* On errors, fall back to fork().  */
  ਜੇ (pid < 0)
    {
      need_move_ਮੰਜ਼ਿਲ_cgroup = true;

      pid = fork ();
      ਜੇ (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
        }
    }

  ਜੇ (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      ਵਾਪਸ join_process_ਮਾਪਾ_helper (context, container,
                                         process, pid, sync_fd,
                                         ਸਥਿਤੀ, need_move_ਮੰਜ਼ਿਲ_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_ਮੰਜ਼ਿਲ_join, ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->ਸਥਿਤੀ, "%s", (*err)->ਸੁਨੇਹਾ);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ਜੇ (UNLIKELY (pid < 0))
    {
      ਪੂਰਨ saved_errno = errno;
      ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  ਜੇ (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_fd, "0", 1));
      ਜੇ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
        }

      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_fd, &pid, ਆਕਾਰ (pid)));
      ਜੇ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
        }

      ਸੁਰੱਖਿਅਤ_exit (EXIT_SUCCESS);
    }
  ਨਹੀਂਤਾਂ
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free ਅੱਖਰ *pty = ਨਿਰਾ;
      ਪੂਰਨ r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_fd, &r, ਆਕਾਰ (r)));
      ਜੇ (UNLIKELY (ret != ਆਕਾਰ (r) || r < 0))
        ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);

      ret = setsid ();
      ਜੇ (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_ਮੰਜ਼ਿਲ_sync_socket_and_die (sync_fd, true, err);
        }

      ਜੇ (terminal_fd)
        {
          cleanup_close ਪੂਰਨ ptmx_fd = -1;

          ret = ਸੈੱਟ_id_init (container, err);
          ਜੇ (UNLIKELY (ret < 0))
            send_error_ਮੰਜ਼ਿਲ_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          ਜੇ (UNLIKELY (ptmx_fd < 0))
            send_error_ਮੰਜ਼ਿਲ_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_ਮੰਜ਼ਿਲ_socket (sync_fd, ptmx_fd, err);
          ਜੇ (UNLIKELY (ret < 0))
            send_error_ਮੰਜ਼ਿਲ_sync_socket_and_die (sync_fd, true, err);
        }
    }

  ਵਾਪਸ 0;

ਬਾਹਰ_ਨਿਕਲੋ:
  ਜੇ (sync_socket_fd[0] >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (sync_socket_fd[0]));
  ਜੇ (sync_socket_fd[1] >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (close (sync_socket_fd[1]));
  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_linux_container_update (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ = ਨਿਰਾ;

  cgroup_ਸਥਿਤੀ = libcrun_cgroup_make_ਸਥਿਤੀ (ਸਥਿਤੀ);

  ਵਾਪਸ libcrun_update_cgroup_resources (cgroup_ਸਥਿਤੀ, state_ਜੜ੍ਹ, resources, err);
}

ਸਥਿਰ ਪੂਰਨ
libcrun_container_pause_unpause_linux (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਅਚਲ bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ = ਨਿਰਾ;

  cgroup_ਸਥਿਤੀ = libcrun_cgroup_make_ਸਥਿਤੀ (ਸਥਿਤੀ);

  ਵਾਪਸ libcrun_cgroup_pause_unpause (cgroup_ਸਥਿਤੀ, pause, err);
}

ਪੂਰਨ
libcrun_container_pause_linux (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err)
{
  ਵਾਪਸ libcrun_container_pause_unpause_linux (ਸਥਿਤੀ, true, err);
}

ਪੂਰਨ
libcrun_container_unpause_linux (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err)
{
  ਵਾਪਸ libcrun_container_pause_unpause_linux (ਸਥਿਤੀ, false, err);
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ persona = 0;
  ਪੂਰਨ ret;

  ਜੇ (strcmp (p->domain, "LINUX") == 0)
    persona = ਪ੍ਰਤੀ_LINUX;
  ਨਹੀਂਤਾਂ ਜੇ (strcmp (p->domain, "LINUX32") == 0)
    persona = ਪ੍ਰਤੀ_LINUX32;
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, 0, "personality to `%s`", p->domain);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_configure_ਜਾਲ (libcrun_container_t *container, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  size_t i;
  bool configure_ਜਾਲ = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ਪੂਰਨ sockfd = -1;

  ਲਈ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ਪੂਰਨ ਮੁੱਲ = libcrun_ਲੱਭੋ_namespace (def->linux->namespaces[i]->type);
      ਜੇ (UNLIKELY (ਮੁੱਲ < 0))
        ਵਾਪਸ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ਜੇ (ਮੁੱਲ == CLONE_NEWNET && def->linux->namespaces[i]->ਰਸਤਾ == ਨਿਰਾ)
        {
          configure_ਜਾਲ = true;
          ਰੋਕੋ;
        }
    }

  ਜੇ (! configure_ਜਾਲ)
    ਵਾਪਸ 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  ਜੇ (LIKELY (sockfd >= 0))
    {
      ਬਣਤਰ ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_ਉੱਪਰ | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  ਨਹੀਂਤਾਂ
    {
      ਬਣਤਰ nlmsghdr *hdr_recv;
      ਅੱਖਰ buf[ਆਕਾਰ (ਬਣਤਰ nlmsghdr) + ਆਕਾਰ (ਬਣਤਰ nlmsgerr)];
      ਬਣਤਰ sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      ਬਣਤਰ sockaddr_nl addr_ਮੰਜ਼ਿਲ = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      ਬਣਤਰ nlmsghdr hdr = {
        .nlmsg_len = ਆਕਾਰ (ਬਣਤਰ nlmsghdr) + ਆਕਾਰ (ਬਣਤਰ ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      ਬਣਤਰ ifinfomsg ਸੁਨੇਹਾ = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_ਸੂਚਕਾਂਕ = 1,
        .ifi_flags = IFF_ਉੱਪਰ,
        .ifi_change = IFF_ਉੱਪਰ,
      };

      *((ਬਣਤਰ nlmsghdr *) buf) = hdr;
      *((ਬਣਤਰ ifinfomsg *) (buf + ਆਕਾਰ (ਬਣਤਰ nlmsghdr))) = ਸੁਨੇਹਾ;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      ਜੇ (UNLIKELY (sockfd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (ਬਣਤਰ sockaddr *) &addr, ਆਕਾਰ (addr));
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, ਆਕਾਰ (buf), 0, (ਬਣਤਰ sockaddr *) &addr_ਮੰਜ਼ਿਲ, ਆਕਾਰ (addr_ਮੰਜ਼ਿਲ));
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, ਆਕਾਰ (buf), 0, ਨਿਰਾ, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (ਬਣਤਰ nlmsghdr *) buf;

      ਜੇ (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          ਬਣਤਰ nlmsgerr *err_ਅੰਕੜੇ = (ਬਣਤਰ nlmsgerr *) NLMSG_ਅੰਕੜੇ (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          ਜੇ (err_ਅੰਕੜੇ->error < 0)
            ਵਾਪਸ crun_make_error (err, -err_ਅੰਕੜੇ->error, "netlink error while configuring network");
        }
    }

  ਵਾਪਸ 0;
}

/* Protection for attacks like CVE-2019-5736.  */
ਪੂਰਨ ensure_cloned_ਦੁਆਧਾਰੀ ();
__attribute__ ((constructor)) ਸਥਿਰ ਖਾਲੀ
libcrun_rexec (ਖਾਲੀ)
{
  ਜੇ (ensure_cloned_ਦੁਆਧਾਰੀ () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
    }
}

ਪੂਰਨ
libcrun_container_checkpoint_linux (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ਵਾਪਸ libcrun_container_checkpoint_linux_criu (ਸਥਿਤੀ, container, cr_options, err);
}

ਪੂਰਨ
libcrun_container_restore_linux (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ret = libcrun_container_restore_linux_criu (ਸਥਿਤੀ, container, cr_options, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
ਸਥਿਰ ਪੂਰਨ
libcrun_kill_linux_no_pidfd (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, bool check_pid, ਪੂਰਨ ਸੰਕੇਤ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  ਜੇ (check_pid)
    {
      ret = libcrun_check_pid_valid (ਸਥਿਤੀ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      /* The pid is not valid anymore, return an error.  */
      ਜੇ (ret == 0)
        {
          errno = ESRCH;
          ਵਾਪਸ crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (ਸਥਿਤੀ->pid, ਸੰਕੇਤ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "kill container");
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_kill_linux (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਪੂਰਨ ਸੰਕੇਤ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_close ਪੂਰਨ pidfd = -1;

  pidfd = syscall_pidfd_open (ਸਥਿਤੀ->pid, 0);
  ਜੇ (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      ਜੇ (errno == ENOSYS)
        ਵਾਪਸ libcrun_kill_linux_no_pidfd (ਸਥਿਤੀ, true, ਸੰਕੇਤ, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      ਜੇ (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        ਵਾਪਸ crun_make_error (err, ESRCH, "process not running");
      ਵਾਪਸ crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* The pid is not valid anymore, return an error.  */
  ਜੇ (ret == 0)
    {
      errno = ESRCH;
      ਵਾਪਸ crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, ਸੰਕੇਤ, ਨਿਰਾ, 0);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      ਜੇ (errno == ENOSYS)
        ਵਾਪਸ libcrun_kill_linux_no_pidfd (ਸਥਿਤੀ, false, ਸੰਕੇਤ, err);
      ਵਾਪਸ crun_make_error (err, errno, "send signal to pidfd");
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
libcrun_get_intelrdt_name (ਅਚਲ ਅੱਖਰ *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  ਜੇ (def == ਨਿਰਾ || def->linux == ਨਿਰਾ || def->linux->intel_rdt == ਨਿਰਾ || def->linux->intel_rdt->clos_id == ਨਿਰਾ)
    {
      ਜੇ (explicit)
        *explicit = false;
      ਵਾਪਸ ctr_name;
    }

  ਜੇ (explicit)
    *explicit = true;
  ਵਾਪਸ def->linux->intel_rdt->clos_id;
}

ਪੂਰਨ
libcrun_apply_intelrdt (ਅਚਲ ਅੱਖਰ *ctr_name, libcrun_container_t *container, pid_t pid, ਪੂਰਨ actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ਨਿਰਾ;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  ਅਚਲ ਅੱਖਰ *name;
  ਪੂਰਨ ret;

  ਜੇ (container)
    def = container->container_def;

  ਜੇ (def == ਨਿਰਾ || def->linux == ਨਿਰਾ || def->linux->intel_rdt == ਨਿਰਾ)
    ਵਾਪਸ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  ਜੇ (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_ਸਮੂਹ (name, ctr_name, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਜਾਓ fail;

          has_monitoring = true;
        }
    }

  ਜੇ (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ fail;
    }

  ਜੇ (actions & LIBCRUN_INTELRDT_MOVE)
    {
      ਅਚਲ ਅੱਖਰ *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : ਨਿਰਾ;
      ret = resctl_move_task_ਮੰਜ਼ਿਲ (name, monitoring_name, pid, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ fail;
    }

  ਵਾਪਸ 0;

fail:
  ਜੇ (has_monitoring)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਪੂਰਨ tmp_ret;

      tmp_ret = resctl_destroy_monitoring_ਸਮੂਹ (name, ctr_name, &tmp_err);
      ਜੇ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  ਜੇ (created)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਪੂਰਨ tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      ਜੇ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_destroy_intelrdt (ਅਚਲ ਅੱਖਰ *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  ਅਚਲ ਅੱਖਰ *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  ਜੇ (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      ਪੂਰਨ ret;

      ret = resctl_destroy_monitoring_ਸਮੂਹ (clos_id, container_id, err);
      ਜੇ (ret < 0)
        ਵਾਪਸ ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  ਜੇ (explicit)
    ਵਾਪਸ 0;

  ਵਾਪਸ resctl_destroy (clos_id, err);
}

ਪੂਰਨ
libcrun_update_intel_rdt (ਅਚਲ ਅੱਖਰ *ctr_name, libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *l3_cache_schema, ਅਚਲ ਅੱਖਰ *mem_bw_schema, ਅੱਖਰ *ਅਚਲ *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *name;

  ਜੇ (container)
    def = container->container_def;

  ਜੇ (def == ਨਿਰਾ || def->linux == ਨਿਰਾ || def->linux->intel_rdt == ਨਿਰਾ)
    ਵਾਪਸ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, ਨਿਰਾ);

  ਵਾਪਸ resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
ਪੂਰਨ
libcrun_ਸੁਰੱਖਿਅਤ_chdir (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = chdir (ਰਸਤਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "chdir to `%s`", ਰਸਤਾ);

  ਬਫਰ = xmalloc (PATH_MAX);
  ret = syscall_getcwd (ਬਫਰ, PATH_MAX);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  ਜੇ (ret == 0 || ਬਫਰ[0] != '/')
    {
      ਜੇ (chdir ("/") < 0)
        {
          /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
        }
      errno = ENOENT;

      /*
        The kernel prepends the string "(unreachable)" to the path
        when it is not reachable from the current mount namespace.
        Use it to give a better error message.
      */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNREACHABLE "(unreachable)"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNREACHABLE_LEN ((ਪੂਰਨ) ਆਕਾਰ (UNREACHABLE) - 1)

      ਜੇ ((ret >= UNREACHABLE_LEN) && (memcmp (ਬਫਰ, UNREACHABLE, UNREACHABLE_LEN) == 0))
        ਵਾਪਸ crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      ਵਾਪਸ crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  ਵਾਪਸ 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
ਸਥਿਰ ਪੂਰਨ
ਲੜੀਵਾਰ_callback_ਵਿੱਚ_namespace (ਪੂਰਨ mntns_fd, ਪੂਰਨ (*callback) (ਖਾਲੀ *, libcrun_error_t *), ਖਾਲੀ *arg, libcrun_error_t *err)
{
  ਪੂਰਨ wait_ਸਥਿਤੀ = 0;
  pid_t pid;
  ਪੂਰਨ ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ਜੇ (UNLIKELY (pid < 0))
    ਵਾਪਸ crun_make_error (err, errno, "vfork");

  ਜੇ (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      ਜੇ (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          ਸੁਰੱਖਿਅਤ_exit (ret);
        }
      ret = chdir ("/");
      ਜੇ (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, errno, "chdir to `/`");
          ਸੁਰੱਖਿਅਤ_exit (ret);
        }

      ret = callback (arg, err);
      /* On failure (ret < 0) the created error will be
         used by the parent proc */
      ਸੁਰੱਖਿਅਤ_exit (ret);
    }

  ret = waitpid_ignore_stopped (pid, &wait_ਸਥਿਤੀ, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  ਵਾਪਸ get_process_exit_ਸਥਿਤੀ (wait_ਸਥਿਤੀ);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
ਸਥਿਰ ਪੂਰਨ
check_container_userns (libcrun_container_t *container, ਪੂਰਨ userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close ਪੂਰਨ expected_fd = -1;
  cleanup_close ਪੂਰਨ ਮਾਪਾ_fd = -1;
  ਬਣਤਰ stat st, expected_st;
  ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  size_t i;
  ਪੂਰਨ ret;

  ਜੇ (def->linux)
    ਲਈ (i = 0; i < def->linux->namespaces_len; i++)
      ਜੇ (libcrun_ਲੱਭੋ_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          ਰਸਤਾ = def->linux->namespaces[i]->ਰਸਤਾ;
          new_userns = ਰਸਤਾ == ਨਿਰਾ;
          ਰੋਕੋ;
        }

  ਜੇ (ਰਸਤਾ == ਨਿਰਾ)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      ਜੇ (UNLIKELY (expected_fd < 0))
        ਵਾਪਸ expected_fd;
    }
  ਨਹੀਂਤਾਂ
    {
      expected_fd = open (ਰਸਤਾ, O_RDONLY | O_CLOEXEC);
      ਜੇ (UNLIKELY (expected_fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਰਸਤਾ);
    }

  /* A user namespace created for the container is a child of the current one.  */
  ਜੇ (new_userns)
    {
      ਮਾਪਾ_fd = ioctl (userns_fd, NS_GET_ਮਾਪਾ);
      ਜੇ (UNLIKELY (ਮਾਪਾ_fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = ਮਾਪਾ_fd;
    }

  ret = fstat (userns_fd, &st);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fstat user namespace");

  ਜੇ (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    ਵਾਪਸ crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  ਵਾਪਸ 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
ਸਥਿਰ ਪੂਰਨ
open_container_mntns (libcrun_container_t *container, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਪੂਰਨ *mntns_fd, ਪੂਰਨ *owner_fd, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ mntns = -1;
  cleanup_close ਪੂਰਨ owner = -1;
  ਪੂਰਨ ret;

  mntns = libcrun_open_proc_pid_file (container, ਸਥਿਤੀ->pid, "ns/mnt", O_RDONLY, err);
  ਜੇ (UNLIKELY (mntns < 0))
    ਵਾਪਸ mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ret == 0)
    ਵਾਪਸ crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  ਜੇ (UNLIKELY (owner < 0))
    ਵਾਪਸ crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਲੜੀਵਾਰ_ਵਿੱਚ_container_namespace (libcrun_container_t *container, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਪੂਰਨ (*callback) (ਖਾਲੀ *, libcrun_error_t *), ਖਾਲੀ *arg, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ mntns_fd = -1;
  cleanup_close ਪੂਰਨ owner_fd = -1;
  ਪੂਰਨ ret;

  ret = open_container_mntns (container, ਸਥਿਤੀ, &mntns_fd, &owner_fd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ ਲੜੀਵਾਰ_callback_ਵਿੱਚ_namespace (mntns_fd, callback, arg, err);
}

ਬਣਤਰ umount_ਵਿੱਚ_a_container_ਦਲੀਲਾਂ
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
ਸਥਿਰ ਪੂਰਨ
umount_nofollow (ਅਚਲ ਅੱਖਰ *destination, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ parentfd = -1;
  cleanup_close ਪੂਰਨ cwdfd = -1;
  ਅੱਖਰ ਬਫਰ[PATH_MAX];
  ਅਚਲ ਅੱਖਰ *ਮਾਪਾ_2, *name;
  ਅੱਖਰ *ਆਖਰੀ_slash;
  size_t len;
  ਪੂਰਨ saved_errno;
  ਪੂਰਨ ret;

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (destination))
    ਵਾਪਸ crun_make_error (err, 0, "empty destination for mount");

  len = ਲੜੀ_ਲੰਬਾਈ (destination);
  ਜੇ (UNLIKELY (len >= ਆਕਾਰ (ਬਫਰ)))
    ਵਾਪਸ crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (ਬਫਰ, destination, len + 1);

  ਜਦੋਂ (len > 1 && ਬਫਰ[len - 1] == '/')
    ਬਫਰ[--len] = '\0';

  ਆਖਰੀ_slash = strrchr (ਬਫਰ, '/');
  ਜੇ (ਆਖਰੀ_slash == ਨਿਰਾ)
    {
      ਮਾਪਾ_2 = ".";
      name = ਬਫਰ;
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਆਖਰੀ_slash == ਬਫਰ)
    {
      ਮਾਪਾ_2 = "/";
      name = ਬਫਰ[1] ? ਬਫਰ + 1 : ".";
    }
  ਨਹੀਂਤਾਂ
    {
      *ਆਖਰੀ_slash = '\0';
      ਮਾਪਾ_2 = ਬਫਰ;
      name = ਆਖਰੀ_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, ਮਾਪਾ_2, O_ਰਸਤਾ | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  ਜੇ (UNLIKELY (parentfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_ਰਸਤਾ | O_DIRECTORY | O_CLOEXEC);
  ਜੇ (UNLIKELY (cwdfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  ਜੇ (UNLIKELY (fchdir (cwdfd) < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fchdir to the previous working directory");

  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, saved_errno, "umount `%s`", destination);

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
do_umount_ਵਿੱਚ_a_container (ਖਾਲੀ *arg, libcrun_error_t *err)
{
  ਬਣਤਰ umount_ਵਿੱਚ_a_container_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ = arg;
  size_t i;
  ਪੂਰਨ ret;

  ਲਈ (i = 0; i < ਦਲੀਲਾਂ->len; i++)
    {
      ret = umount_nofollow (ਦਲੀਲਾਂ->mounts[i]->destination, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਬਣਤਰ mount_ਵਿੱਚ_a_container_ਦਲੀਲਾਂ
{
  runtime_spec_schema_defs_mount **mounts;
  ਬਣਤਰ libcrun_fd_map *fds;
  ਪੂਰਨ pidfd;
  ਪੂਰਨ owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
ਸਥਿਰ ਪੂਰਨ
open_destination_nofollow (ਅਚਲ ਅੱਖਰ *destination, bool dir, libcrun_error_t *err)
{
  ਅਚਲ uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  ਪੂਰਨ fd;
  ਪੂਰਨ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_ਰਸਤਾ | O_CLOEXEC, 0, resolve);
  ਜੇ (LIKELY (fd >= 0))
    ਵਾਪਸ fd;

  ਜੇ (errno != ENOENT)
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", destination);

  ਜੇ (dir)
    ret = crun_ਸੁਰੱਖਿਅਤ_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  ਨਹੀਂਤਾਂ
    ret = crun_ਸੁਰੱਖਿਅਤ_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_ਰਸਤਾ | O_CLOEXEC, 0, resolve);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", destination);

  ਵਾਪਸ fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
ਸਥਿਰ ਪੂਰਨ
create_destinations_ਵਿੱਚ_a_container (ਖਾਲੀ *arg, libcrun_error_t *err)
{
  ਬਣਤਰ mount_ਵਿੱਚ_a_container_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ = arg;
  size_t i;
  ਪੂਰਨ ret;

  ret = setns (ਦਲੀਲਾਂ->owner_fd, CLONE_NEWUSER);
  ਜੇ (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "setresuid to `0`");
    }
  ਨਹੀਂਤਾਂ ਜੇ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ਵਾਪਸ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  ਲਈ (i = 0; i < ਦਲੀਲਾਂ->len; i++)
    {
      cleanup_close ਪੂਰਨ dest_fd = -1;
      ਬਣਤਰ stat st;

      ret = fstat (ਦਲੀਲਾਂ->fds->fds[i], &st);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (ਦਲੀਲਾਂ->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ਜੇ (UNLIKELY (dest_fd < 0))
        ਵਾਪਸ dest_fd;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
do_mount_ਵਿੱਚ_a_container (ਖਾਲੀ *arg, libcrun_error_t *err)
{
  ਬਣਤਰ mount_ਵਿੱਚ_a_container_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ = arg;
  size_t i;
  ਪੂਰਨ ret;

  ਲਈ (i = 0; i < ਦਲੀਲਾਂ->len; i++)
    {
      cleanup_close ਪੂਰਨ dest_fd = -1;
      ਬਣਤਰ stat st;

      ret = fstat (ਦਲੀਲਾਂ->fds->fds[i], &st);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (ਦਲੀਲਾਂ->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ਜੇ (UNLIKELY (dest_fd < 0))
        ਵਾਪਸ dest_fd;

      ret = fs_move_mount_ਮੰਜ਼ਿਲ (ਦਲੀਲਾਂ->fds->fds[i], dest_fd, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "move mount to `%s`", ਦਲੀਲਾਂ->mounts[i]->destination);
    }

  ਵਾਪਸ 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
ਸਥਿਰ ਪੂਰਨ
send_mounts_from_container_namespaces (libcrun_container_t *container, ਪੂਰਨ pidfd, runtime_spec_schema_defs_mount **mounts,
                                       ਬਣਤਰ libcrun_fd_map *fds, size_t len, ਪੂਰਨ flags, ਪੂਰਨ owner_fd, ਪੂਰਨ sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  ਪੂਰਨ ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_ਸੈੱਟ_DUMPABLE, 0, 0, 0, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  ਜੇ (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  ਨਹੀਂਤਾਂ ਜੇ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ਵਾਪਸ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ਜੇ (UNLIKELY (pid < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fork");

  ਜੇ (pid)
    {
      ਪੂਰਨ wait_ਸਥਿਤੀ = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (waitpid (pid, &wait_ਸਥਿਤੀ, WUNTRACED));
      ਜੇ (ret > 0 && WIFSTOPPED (wait_ਸਥਿਤੀ))
        {
          kill (pid, SIGKILL);
          ਆਰਜ਼ੀ_FAILURE_RETRY (waitpid (pid, ਨਿਰਾ, 0));
          ਵਾਪਸ crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      ਸੁਰੱਖਿਅਤ_exit (EXIT_SUCCESS);
    }

  ret = ਸੈੱਟ_id_init (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = prctl (PR_ਸੈੱਟ_DUMPABLE, 0, 0, 0, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ਲਈ (i = 0; i < len; i++)
    {
      cleanup_close ਪੂਰਨ fd = -1;

      ਜੇ (fds->fds[i] >= 0)
        ਜਾਰੀ;

      fd = open_mount_ਦਾ_type (container, mounts[i], err);
      ਜੇ (UNLIKELY (fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_ਮੰਜ਼ਿਲ_socket (sync_fd, fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
ਸਥਿਰ ਪੂਰਨ
get_namespaces_ਮੰਜ਼ਿਲ_join (libcrun_container_t *container, pid_t pid, ਪੂਰਨ *flags, libcrun_error_t *err)
{
  ਸਥਿਰ ਅਚਲ ਬਣਤਰ
  {
    ਅਚਲ ਅੱਖਰ *name;
    ਪੂਰਨ flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  ਲਈ (i = 0; i < ਆਕਾਰ (namespaces) / ਆਕਾਰ (namespaces[0]); i++)
    {
      cleanup_close ਪੂਰਨ self_fd = -1;
      cleanup_close ਪੂਰਨ ਨਿਸ਼ਾਨਾ_fd = -1;
      ਬਣਤਰ stat self_st, ਨਿਸ਼ਾਨਾ_st;
      cleanup_free ਅੱਖਰ *self_ਰਸਤਾ = ਨਿਰਾ;
      ਪੂਰਨ ret;

      xasprintf (&self_ਰਸਤਾ, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_ਰਸਤਾ, O_RDONLY, err);
      ਜੇ (UNLIKELY (self_fd < 0))
        ਵਾਪਸ self_fd;

      ਨਿਸ਼ਾਨਾ_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      ਜੇ (UNLIKELY (ਨਿਸ਼ਾਨਾ_fd < 0))
        ਵਾਪਸ ਨਿਸ਼ਾਨਾ_fd;

      ret = fstat (self_fd, &self_st);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (ਨਿਸ਼ਾਨਾ_fd, &ਨਿਸ਼ਾਨਾ_st);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ਜੇ (self_st.st_dev != ਨਿਸ਼ਾਨਾ_st.st_dev || self_st.st_ino != ਨਿਸ਼ਾਨਾ_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  ਵਾਪਸ 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
ਸਥਿਰ ਪੂਰਨ
open_mounts_ਵਿੱਚ_container_namespaces (libcrun_container_t *container, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਪੂਰਨ owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, ਬਣਤਰ libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ sync_fd = -1;
  cleanup_close ਪੂਰਨ pidfd = -1;
  ਪੂਰਨ sync_socket_fd[2];
  bool needed = false;
  ਪੂਰਨ flags;
  pid_t pid;
  size_t i;
  ਪੂਰਨ ret;

  ਲਈ (i = 0; i < len; i++)
    {
      ਜੇ (fds->fds[i] >= 0)
        ਜਾਰੀ;

      ਜੇ (mounts[i]->type == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  ਜੇ (! needed)
    ਵਾਪਸ 0;

  pidfd = syscall_pidfd_open (ਸਥਿਤੀ->pid, 0);
  ਜੇ (UNLIKELY (pidfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ret == 0)
    ਵਾਪਸ crun_make_error (err, 0, "container not running");

  ret = get_namespaces_ਮੰਜ਼ਿਲ_join (container, ਸਥਿਤੀ->pid, &flags, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  ਜੇ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      ਆਰਜ਼ੀ_FAILURE_RETRY (close (sync_socket_fd[1]));
      ਵਾਪਸ ret;
    }

  ਜੇ (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      ਜੇ (UNLIKELY (ret < 0))
        send_error_ਮੰਜ਼ਿਲ_sync_socket_and_die (sync_socket_fd[1], true, err);

      ਸੁਰੱਖਿਅਤ_exit (EXIT_SUCCESS);
    }

  ਆਰਜ਼ੀ_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  ਲਈ (i = 0; i < len; i++)
    {
      ਜੇ (fds->fds[i] >= 0)
        ਜਾਰੀ;

      ret = receive_fd_from_socket (sync_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          cleanup_free ਅੱਖਰ *err_str = ਨਿਰਾ;
          ਪੂਰਨ err_ਕੋਡ = 0;

          ਜੇ (read_error_from_sync_socket (sync_fd, &err_ਕੋਡ, &err_str) && ! is_ਖਾਲੀ_ਥਾਵਾਂ_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_ਕੋਡ, "%s", err_str);
            }
          ਨਹੀਂਤਾਂ
            ret = crun_error_wrap (err, "receive mount fd");
          ਰੋਕੋ;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  ਆਰਜ਼ੀ_FAILURE_RETRY (waitpid (pid, ਨਿਰਾ, 0));

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_ਸੈੱਟ)
{
  cleanup_free ਅੱਖਰ *ਅੰਕੜੇ = ਨਿਰਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ extra_flags = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags = 0;
  size_t j;

  ਜੇ (mount->options == ਨਿਰਾ)
    ਵਾਪਸ get_default_flags (container, mount->destination, &ਅੰਕੜੇ);

  ਲਈ (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &ਅੰਕੜੇ, rec_clear, rec_ਸੈੱਟ);

  ਵਾਪਸ flags;
}

ਪੂਰਨ
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਬਣਤਰ mount_ਵਿੱਚ_a_container_ਦਲੀਲਾਂ ਦਲੀਲਾਂ;
  cleanup_close_map ਬਣਤਰ libcrun_fd_map *fds = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ pidfd = -1;
  cleanup_close ਪੂਰਨ mntns_fd = -1;
  cleanup_close ਪੂਰਨ owner_fd = -1;
  pid_t pid = ਸਥਿਤੀ->pid;
  size_t i;
  ਪੂਰਨ ret;

  ret = open_container_mntns (container, ਸਥਿਤੀ, &mntns_fd, &owner_fd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  fds = make_libcrun_fd_map (len);

  ਲਈ (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), ਨਿਰਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_ਸੈੱਟ = 0;
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪ੍ਰਸਾਰ;

          ਪ੍ਰਸਾਰ = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_ਸੈੱਟ) & ALL_PROPAGATIONS_NO_REC;
          ਜੇ (ਪ੍ਰਸਾਰ == 0)
            ਪ੍ਰਸਾਰ = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->ਸਰੋਤ, recursive, false, nofollow, ਪ੍ਰਸਾਰ, err);
          ਜੇ (UNLIKELY (fds->fds[i] < 0))
            ਵਾਪਸ fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_ਵਿੱਚ_container_namespaces (container, ਸਥਿਤੀ, owner_fd, mounts, fds, len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਲਈ (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_ਸੈੱਟ = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_ਸੈੱਟ);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_ਸੈੱਟ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਦਲੀਲਾਂ.mounts = mounts;
  ਦਲੀਲਾਂ.fds = fds;
  ਦਲੀਲਾਂ.pidfd = pidfd;
  ਦਲੀਲਾਂ.owner_fd = owner_fd;
  ਦਲੀਲਾਂ.len = len;

  ret = ਲੜੀਵਾਰ_callback_ਵਿੱਚ_namespace (mntns_fd, create_destinations_ਵਿੱਚ_a_container, &ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret > 0))
    ਵਾਪਸ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ ਲੜੀਵਾਰ_callback_ਵਿੱਚ_namespace (mntns_fd, do_mount_ਵਿੱਚ_a_container, &ਦਲੀਲਾਂ, err);
}

ਪੂਰਨ
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  ਬਣਤਰ umount_ਵਿੱਚ_a_container_ਦਲੀਲਾਂ ਦਲੀਲਾਂ;

  ਦਲੀਲਾਂ.mounts = mounts;
  ਦਲੀਲਾਂ.len = len;

  ਵਾਪਸ ਲੜੀਵਾਰ_ਵਿੱਚ_container_namespace (container, ਸਥਿਤੀ, do_umount_ਵਿੱਚ_a_container, &ਦਲੀਲਾਂ, err);
}

ਬਣਤਰ restrict_mounts_ਵਿੱਚ_a_container_ਦਲੀਲਾਂ
{
  ਬਣਤਰ libcrun_mount_flags_s *mounts;
  size_t len;
  ਪੂਰਨ rootfd;
};

ਸਥਿਰ ਪੂਰਨ
do_restrict_mounts_ਵਿੱਚ_a_container (ਖਾਲੀ *arg, libcrun_error_t *err)
{
  ਬਣਤਰ restrict_mounts_ਵਿੱਚ_a_container_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ = arg;
  size_t i;
  ਪੂਰਨ ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (ਦਲੀਲਾਂ->rootfd);
  ਜੇ (LIKELY (ret == 0))
    ret = chroot (".");
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "chroot to the container root");

  ਲਈ (i = 0; i < ਦਲੀਲਾਂ->len; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags = ਦਲੀਲਾਂ->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ = consume_slashes (ਦਲੀਲਾਂ->mounts[i].ਨਿਸ਼ਾਨਾ);
      cleanup_close ਪੂਰਨ fd = -1;
      ਬਣਤਰ statfs sfs;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ current_flags;

      ਜੇ (flags == 0)
        ਜਾਰੀ;

      fd = ਸੁਰੱਖਿਅਤ_openat (AT_FDCWD, "/", ਨਿਸ਼ਾਨਾ, O_ਰਸਤਾ | O_CLOEXEC, 0, err);
      ਜੇ (UNLIKELY (fd < 0))
        {
          ਜੇ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ਜਾਰੀ;
            }
          ਵਾਪਸ fd;
        }

      ret = fstatfs (fd, &sfs);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "statfs `/%s`", ਨਿਸ਼ਾਨਾ);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      ਜੇ ((current_flags & flags) == flags)
        ਜਾਰੀ;

      ret = do_mount_setattr (false, ਨਿਸ਼ਾਨਾ, fd, 0, flags, err);
      ਜੇ (LIKELY (ret == 0))
        ਜਾਰੀ;

      ਜੇ (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        ਵਾਪਸ ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (ਨਿਰਾ, ਨਿਸ਼ਾਨਾ, ਨਿਰਾ, MS_REMOUNT | MS_BIND | flags | current_flags, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "remount `/%s`", ਨਿਸ਼ਾਨਾ);
    }

  ਵਾਪਸ 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
ਪੂਰਨ
libcrun_restrict_mounts_ਵਿੱਚ_container (pid_t pid, ਬਣਤਰ libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  ਬਣਤਰ restrict_mounts_ਵਿੱਚ_a_container_ਦਲੀਲਾਂ ਦਲੀਲਾਂ;
  cleanup_close ਪੂਰਨ mntns_fd = -1;
  cleanup_close ਪੂਰਨ rootfd = -1;
  ਅੱਖਰ ਰਸਤਾ[64];
  ਪੂਰਨ ret;

  snprintf (ਰਸਤਾ, ਆਕਾਰ (ਰਸਤਾ), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (ਰਸਤਾ, O_RDONLY | O_CLOEXEC);
  ਜੇ (UNLIKELY (mntns_fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਰਸਤਾ);

  snprintf (ਰਸਤਾ, ਆਕਾਰ (ਰਸਤਾ), "/proc/%d/root", pid);
  rootfd = open (ਰਸਤਾ, O_ਰਸਤਾ | O_DIRECTORY | O_CLOEXEC);
  ਜੇ (UNLIKELY (rootfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਰਸਤਾ);

  ਦਲੀਲਾਂ.mounts = mounts;
  ਦਲੀਲਾਂ.len = len;
  ਦਲੀਲਾਂ.rootfd = rootfd;

  ret = ਲੜੀਵਾਰ_callback_ਵਿੱਚ_namespace (mntns_fd, do_restrict_mounts_ਵਿੱਚ_a_container, &ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret > 0))
    ਵਾਪਸ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_move_ਜਾਲ_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ਪੂਰਨ netns_fd = -1;
  size_t i;
  ਪੂਰਨ ret;

  ਜੇ (def == ਨਿਰਾ || def->linux == ਨਿਰਾ || def->linux->net_devices == ਨਿਰਾ)
    ਵਾਪਸ 0;

  cleanup_free ਅੱਖਰ *ns_ਰਸਤਾ = ਨਿਰਾ;

  xasprintf (&ns_ਰਸਤਾ, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_ਰਸਤਾ, O_RDONLY, err);
  ਜੇ (UNLIKELY (netns_fd < 0))
    ਵਾਪਸ netns_fd;

  ਲਈ (i = 0; i < def->linux->net_devices->len; i++)
    {
      ਅਚਲ ਅੱਖਰ *new_name = def->linux->net_devices->ਮੁੱਲਾਂ[i]->name ?: def->linux->net_devices->ਕੁੰਜੀਆਂ[i];

      ret = move_ਜਾਲ_device (def->linux->net_devices->ਕੁੰਜੀਆਂ[i], new_name, netns_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}
