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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "linux.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "status.h"
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/mount.h>
#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#ଯଦି defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#endif

#ଅନ୍ତର୍ଭାଵ <sys/prctl.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#ଅନ୍ତର୍ଭାଵ <sys/time.h>
#ଅନ୍ତର୍ଭାଵ <sys/resource.h>
#ଅନ୍ତର୍ଭାଵ <sys/sysmacros.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <grp.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ "terminal.h"
#ଅନ୍ତର୍ଭାଵ "cgroup.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-utils.h"
#ଅନ୍ତର୍ଭାଵ "status.h"
#ଅନ୍ତର୍ଭାଵ "criu.h"
#ଅନ୍ତର୍ଭାଵ "scheduler.h"
#ଅନ୍ତର୍ଭାଵ "intelrdt.h"
#ଅନ୍ତର୍ଭାଵ "io_priority.h"
#ଅନ୍ତର୍ଭାଵ "net_device.h"

#ଅନ୍ତର୍ଭାଵ <sys/socket.h>
#ଅନ୍ତର୍ଭାଵ <libgen.h>
#ଅନ୍ତର୍ଭାଵ <sys/wait.h>
#ଅନ୍ତର୍ଭାଵ <sys/vfs.h>
#ଅନ୍ତର୍ଭାଵ <sys/statvfs.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ <sys/personality.h>
#ଅନ୍ତର୍ଭାଵ <net/if.h>
#ଅନ୍ତର୍ଭାଵ <sys/xattr.h>
#ଅନ୍ତର୍ଭାଵ <linux/netlink.h>
#ଅନ୍ତର୍ଭାଵ <linux/rtnetlink.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched.h>
#ଅନ୍ତର୍ଭାଵ <linux/magic.h>
#ଅନ୍ତର୍ଭାଵ <linux/nsfs.h>

#ଅନ୍ତର୍ଭାଵ <json-c/json.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/json_common.h>

#ଅନ୍ତର୍ଭାଵ "mount_flags.h"
#ଅନ୍ତର୍ଭାଵ "syscalls.h"

#ifndef RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#endif

#ifndef OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#endif

#ifndef OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#endif

#ifndef OPEN_ବୃକ୍ଷ_NAMESPACE
#  define OPEN_ବୃକ୍ଷ_NAMESPACE 2
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
ସ୍ଥିର unsigned long
mount_flags_from_statfs (unsigned long f_flags)
{
  unsigned long flags = 0;

  ଯଦି (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  ଯଦି (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  ଯଦି (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  ଯଦି (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  ଯଦି (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  ଯଦି (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  ଯଦି (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  ଯଦି (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  ଯଦି (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  ଫେରନ୍ତୁ flags;
}

ସଂରଚନା remount_s
{
  ସଂରଚନା remount_s *next;
  ଅକ୍ଷର *target;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd;
  unsigned long flags;
  ଅକ୍ଷର *data;
};

ସଂରଚନା private_data_s
{
  ସଂରଚନା remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା unshare_flags;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା unshare_cgroupns;

  ଅକ୍ଷର *host_notify_socket_path;
  ଅକ୍ଷର *container_notify_socket_path;
  ଅକ୍ଷର *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା procfd;

  ଅଚଳ ଅକ୍ଷର *rootfs;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootfsfd;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା notify_socket_ବୃକ୍ଷ_fd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_ମୂଳ_fd;

  ସଂରଚନା libcrun_fd_map *mount_fds;
  ସଂରଚନା libcrun_fd_map *dev_fds;
  ସଂରଚନା libcrun_fd_map *needed_devs_fds;

  ଅକ୍ଷର **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  ଅକ୍ଷର *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା maskdir_fd;
  ଅକ୍ଷର *maskdir_proc_path;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_pivot;
  bool no_pivot;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_ମୂଳ_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  ଅକ୍ଷର *host_rootfs;
};

ସଂରଚନା linux_namespace_s
{
  ଅଚଳ ଅକ୍ଷର *name;
  ଅଚଳ ଅକ୍ଷର *ns_file;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;
};

ସ୍ଥିର ଶୂନ୍ୟ
cleanup_private_data (ଶୂନ୍ୟ *private_data)
{
  ସଂରଚନା private_data_s *p = private_data;

  ଯଦି (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  ଯଦି (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  ଯଦି (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  ଯଦି (p->old_ମୂଳ_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_ମୂଳ_fd));
  ଯଦି (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  ଯଦି (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  ଯଦି (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  ଯଦି (p->copy_symlink_targets)
    {
      size_t i;
      ପାଇଁ (i = 0; i < p->n_copy_symlink_targets; i++)
        ମୁକ୍ତ (p->copy_symlink_targets[i]);
      ମୁକ୍ତ (p->copy_symlink_targets);
    }
  ମୁକ୍ତ (p->unified_cgroup_path);
  ମୁକ୍ତ (p->host_notify_socket_path);
  ମୁକ୍ତ (p->container_notify_socket_path);
  ମୁକ୍ତ (p->external_descriptors);
  ମୁକ୍ତ (p->maskdir_proc_path);
  ମୁକ୍ତ (p->host_rootfs);
  ମୁକ୍ତ (p);
}

ସ୍ଥିର ସଂରଚନା private_data_s *
get_private_data (ସଂରଚନା libcrun_container_s *container)
{
  ଯଦି (container->private_data == ଖାଲି)
    {
      ସଂରଚନା private_data_s *p = xmalloc0 (ଆକାର (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_ବୃକ୍ଷ_fd = -1;
      p->old_ମୂଳ_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  ଫେରନ୍ତୁ container->private_data;
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

ସ୍ଥିର ସଂରଚନା linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { ଖାଲି, ଖାଲି, 0 } };

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_and_reset (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *old)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmp = *old;
  *old = -1;
  ଫେରନ୍ତୁ tmp;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_find_namespace (ଅଚଳ ଅକ୍ଷର *name)
{
  ସଂରଚନା linux_namespace_s *it;
  ପାଇଁ (it = namespaces; it->name; it++)
    ଯଦି (strcmp (it->name, name) == 0)
      ଫେରନ୍ତୁ it->ମୂଲ୍ୟ;
  ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_keyctl_join (ଅଚଳ ଅକ୍ଷର *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_pidfd_open (pid_t pid, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
#ଯଦି defined __NR_pidfd_open
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_pidfd_open, pid, flags);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) pid;
  (ଶୂନ୍ୟ) flags;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_pidfd_send_signal (ପୂର୍ଣ୍ଣସଂଖ୍ୟା pidfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig, siginfo_t *info, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
#ଯଦି defined __NR_pidfd_send_signal
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) pidfd;
  (ଶୂନ୍ୟ) sig;
  (ଶୂନ୍ୟ) info;
  (ଶୂନ୍ୟ) flags;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
ସ୍ଥିର uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  ଯଦି (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  ଯଦି (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  ଯଦି (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  ଯଦି (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  ଯଦି (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  ଯଦି (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  ଯଦି (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  ଯଦି (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  ଫେରନ୍ତୁ attr;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_mount_setattr (bool recursive, ଅଚଳ ଅକ୍ଷର *target, ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  ସଂରଚନା mount_attr_s attr = {
    0,
  };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  ଯଦି (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      ଯଦି (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      ଅନ୍ୟଥା ଯଦି (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      ଅନ୍ୟଥା
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      ଯଦି (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  ଅନ୍ୟଥା ଯଦି (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "mount_setattr `/%s`", target);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_bind_mount (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_ବୃକ୍ଷ_fd = -1;
  ସଂରଚନା mount_attr_s attr = {
    0,
  };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା recursive_flag = (recursive ? AT_RECURSIVE : 0);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  ଯଦି (UNLIKELY (src == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "mount source is not specified");

  ଯଦି (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  ଯଦି (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_ବୃକ୍ଷ_fd = syscall_open_ବୃକ୍ଷ (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  ଯଦି (UNLIKELY (open_ବୃକ୍ଷ_fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_ବୃକ୍ଷ_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "mount_setattr `%s`", src);

  ଫେରନ୍ତୁ get_and_reset (&open_ବୃକ୍ଷ_fd);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, ଅକ୍ଷର *option, ଅକ୍ଷର **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free ଅକ୍ଷର *mappings = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *it;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  mappings = xmalloc (allocated);

  ପାଇଁ (it = option; *it;)
    {
      bool relative = false;
      long ମୂଲ୍ୟ[3];
      size_t i;

      ଯଦି (*it == '\0')
        ଭାଙ୍ଗ;

      ଯଦି (*it == '#')
        it++;

      ଯଦି (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      ପାଇଁ (i = 0; i < 3; i++)
        {
          ଅକ୍ଷର *endptr = ଖାଲି;

          ଯଦି (i > 0 && *it == '-')
            it++;

          ଯଦି (*it == '\0')
            ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          ମୂଲ୍ୟ[i] = strtol (it, &endptr, 10);
          ଯଦି (errno || endptr == it)
            ଫେରନ୍ତୁ crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      ଯଦି (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          ଯଦି (def == ଖାଲି
              || def->linux == ଖାଲି
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          ପାଇଁ (i = 0; i < mappings_len; i++)
            ଯଦି (ମୂଲ୍ୟ[1] >= (int64_t) mappings[i]->container_id
                && ମୂଲ୍ୟ[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              ଭାଙ୍ଗ;

          ଯଦି (i == mappings_len)
            ଫେରନ୍ତୁ crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          ମୂଲ୍ୟ[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      ଯଦି (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", ମୂଲ୍ୟ[0], ମୂଲ୍ୟ[1], ମୂଲ୍ୟ[2]);
      ଯଦି (UNLIKELY (ret >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (allocated - written)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = ଖାଲି;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
format_mount_mappings (ଅକ୍ଷର **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  ଅଚଳ size_t max_len_mapping = 64;
  cleanup_free ଅକ୍ଷର *ret = ଖାଲି;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  ପାଇଁ (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      ଯଦି (UNLIKELY (len >= max_len_mapping))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = ଖାଲି;
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
format_mount_mapping (ଅକ୍ଷର **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t size, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .size = size,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    ଖାଲି,
  };

  ଫେରନ୍ତୁ format_mount_mappings (ret, mappings, 1, written, err);
}

ସ୍ଥିର bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  ଯଦି (def->linux == ଖାଲି)
    ଫେରନ୍ତୁ mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  ଯଦି (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    ଫେରନ୍ତୁ false;

  ଯଦି (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    ଫେରନ୍ତୁ false;

  ପାଇଁ (s = 0; s < mnt->uid_mappings_len; s++)
    {
      ଯଦି (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        ଫେରନ୍ତୁ false;
      ଯଦି (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        ଫେରନ୍ତୁ false;
      ଯଦି (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        ଫେରନ୍ତୁ false;
    }

  ପାଇଁ (s = 0; s < mnt->gid_mappings_len; s++)
    {
      ଯଦି (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        ଫେରନ୍ତୁ false;
      ଯଦି (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        ଫେରନ୍ତୁ false;
      ଯଦି (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        ଫେରନ୍ତୁ false;
    }

  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        ଅଚଳ ଅକ୍ଷର *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != ଖାଲି;

  ଯଦି (! need_new_userns)
    ଫେରନ୍ତୁ 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, ଖାଲି);
  ଯଦି (UNLIKELY (pid < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "clone");

  ଯଦି (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      ଯେତେବେଳେ (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  ଯଦି (mnt->uid_mappings_len)
    {
      cleanup_free ଅକ୍ଷର *uid_map = ଖାଲି;
      cleanup_free ଅକ୍ଷର *gid_map = ଖାଲି;
      size_t written = 0;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      ଯଦି (UNLIKELY (fd < 0))
        ଫେରନ୍ତୁ fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      ଯଦି (UNLIKELY (fd < 0))
        ଫେରନ୍ତୁ fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      cleanup_free ଅକ୍ଷର *dup_options = ଖାଲି;
      ଅକ୍ଷର *option, *saveptr = ଖାଲି;

      ଯଦି (! options)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      ପାଇଁ (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (ଖାଲି, ";", &saveptr))
        {
          cleanup_free ଅକ୍ଷର *mappings = ଖାଲି;
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

          ଯଦି (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              ଯଦି (UNLIKELY (map_fd < 0))
                ଫେରନ୍ତୁ map_fd;
            }
          ଅନ୍ୟଥା ଯଦି (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              ଯଦି (UNLIKELY (map_fd < 0))
                ଫେରନ୍ତୁ map_fd;
            }
          ଅନ୍ୟଥା
            ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_create_keyring (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *label, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା labelfd = -1;
  bool label_set = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      ଯଦି (UNLIKELY (labelfd < 0))
        {
          ଯଦି (crun_error_get_errno (err) != ENOENT)
            ଫେରନ୍ତୁ labelfd;

          crun_error_release (err);
        }

      ଯଦି (labelfd >= 0)
        {
          ret = ଲିଖ (labelfd, label, strlen (label));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          ଯାଅ out;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      ଯାଅ out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  ଯଦି (label_set && ଲିଖ (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  ଯଦି (def->process && def->process->user)
    {
      ଯଦି (def->process->user->uid)
        *uid = def->process->user->uid;
      ଯଦି (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

ସ୍ଥିର unsigned long
get_mount_flags (ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  ଅଚଳ ସଂରଚନା propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  ଯଦି (found)
    *found = prop ? 1 : 0;

  ଯଦି (prop == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ଯଦି (prop->extra_flags & OPTION_RECURSIVE)
    {
      ଯଦି (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      ଯଦି (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  ଯଦି (extra_flags)
    *extra_flags |= prop->extra_flags;

  ଯଦି (prop->clear)
    ଫେରନ୍ତୁ current_flags & ~prop->flags;

  ଫେରନ୍ତୁ current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
ସ୍ଥିର bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ପାଇଁ (i = 0; i < mount->options_len; i++)
    {
      ଅଚଳ ସଂରଚନା propagation_flags_s *prop;

      ଯଦି (mount->options[i] == ଖାଲି)
        ଜାରି;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ଯଦି (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        ଫେରନ୍ତୁ true;
    }

  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର unsigned long
get_mount_flags_or_option (ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_flags, unsigned long *extra_flags, ଅକ୍ଷର **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା found;
  __attribute__ ((unused)) cleanup_free ଅକ୍ଷର *prev = ଖାଲି;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  ଯଦି (name == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  ଯଦି (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      ଅଚଳ ଅକ୍ଷର *data_flag = ଖାଲି;
      ଯଦି ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      ଅନ୍ୟଥା ଯଦି ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      ଯଦି (data_flag)
        {
          prev = *option;
          ଯଦି (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          ଅନ୍ୟଥା
            *option = xstrdup (data_flag);
        }
      ଫେରନ୍ତୁ flags;
    }

  prev = *option;
  ଯଦି (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  ଅନ୍ୟଥା
    *option = xstrdup (name);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
pivot_ମୂଳ (ଅଚଳ ଅକ୍ଷର *new_ମୂଳ, ଅଚଳ ଅକ୍ଷର *put_old)
{
  ଫେରନ୍ତୁ syscall (__NR_pivot_root, new_ମୂଳ, put_old);
}

ସ୍ଥିର ଶୂନ୍ୟ
free_remount (ସଂରଚନା remount_s *r)
{
  ଯଦି (r == ଖାଲି)
    ଫେରନ୍ତୁ;
  ଯଦି (r->targetfd >= 0)
    close (r->targetfd);
  ମୁକ୍ତ (r->data);
  ମୁକ୍ତ (r->target);
  ମୁକ୍ତ (r);
}

ସ୍ଥିର ସଂରଚନା remount_s *
make_remount (ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd, ଅଚଳ ଅକ୍ଷର *target, unsigned long flags, ଅଚଳ ଅକ୍ଷର *data, ସଂରଚନା remount_s *next)
{
  ସଂରଚନା remount_s *ret = xmalloc (ଆକାର (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : ଖାଲି;
  ret->next = next;
  ret->targetfd = targetfd;
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_remount (ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd, ଅଚଳ ଅକ୍ଷର *target, unsigned long flags, ଅଚଳ ଅକ୍ଷର *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅଚଳ ଅକ୍ଷର *real_target = target;

  ଯଦି (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      ଯଦି (LIKELY (ret == 0))
        ଫେରନ୍ତୁ 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  ଯଦି (flags & (MS_REMOUNT | MS_RDONLY))
    data = ଖାଲି;

  ret = mount (ଖାଲି, real_target, ଖାଲି, flags, data);
  ଯଦି (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      ସଂରଚନା statfs sfs;

      ret = statfs (real_target, &sfs);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      ଯଦି (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      ଯଦି ((flags | remount_flags) != flags)
        {
          ret = mount (ଖାଲି, real_target, ଖାଲି, flags | remount_flags, data);
          ଯଦି (LIKELY (ret == 0))
            ଫେରନ୍ତୁ 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          ଯଦି (current_flags & MS_RDONLY)
            ret = mount (ଖାଲି, real_target, ଖାଲି, flags | remount_flags | MS_RDONLY, data);
        }
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "remount `%s`", target);
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
  ସଂରଚନା remount_s *r = get_private_data (container)->remounts;
  ଯେତେବେଳେ (r)
    {
      ସଂରଚନା remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      ଯଦି (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          ଯଦି (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              ଜାରି;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  ଯେତେବେଳେ (r)
    {
      ସଂରଚନା remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = ଖାଲି;
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_mount_target (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *target_rel, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *rootfs = get_private_data (container)->rootfs;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootfsfd = get_private_data (container)->rootfsfd;

  ଯଦି (rootfsfd < 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid rootfs state");

  ଫେରନ୍ତୁ safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
fsopen_mount (ଅଚଳ ଅକ୍ଷର *type, ଅଚଳ ଅକ୍ଷର *source_name, ଅଚଳ ଅକ୍ଷର *labeltype, ଅଚଳ ଅକ୍ଷର *label, ଅଚଳ ଅକ୍ଷର *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsfd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ଯଦି (UNLIKELY (fsfd < 0))
    ଫେରନ୍ତୁ fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (ଶୂନ୍ୟ) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  ଯଦି (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (data && data[0])
    {
      cleanup_free ଅକ୍ଷର *data_copy = xstrdup (data);
      ଅକ୍ଷର *saveptr = ଖାଲି;
      ଅକ୍ଷର *token;

      ପାଇଁ (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (ଖାଲି, ",", &saveptr))
        {
          ଅକ୍ଷର *eq = strchr (token, '=');
          ଯଦି (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          ଅନ୍ୟଥା
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, ଖାଲି, 0);

          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ଖାଲି, ଖାଲି, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) type;
  (ଶୂନ୍ୟ) source_name;
  (ଶୂନ୍ୟ) data;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_procfd (ସଂରଚନା private_data_s *data, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

  ଯଦି (data->procfd >= 0)
    ଫେରନ୍ତୁ data->procfd;

  fd = fsopen_mount ("proc", ଖାଲି, ଖାଲି, ଖାଲି, ଖାଲି);
  ଯଦି (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  ଫେରନ୍ତୁ fd;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_old_ମୂଳ_fd (ସଂରଚନା private_data_s *data)
{
  ଯଦି (data->old_ମୂଳ_fd >= 0)
    ଫେରନ୍ତୁ data->old_ମୂଳ_fd;

  data->old_ମୂଳ_fd = open ("/", O_PATH | O_CLOEXEC);
  ଫେରନ୍ତୁ data->old_ମୂଳ_fd;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
fs_move_mount_to (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *name)
{
#ifdef HAVE_NEW_MOUNT_API
  ଯଦି (name)
    ଫେରନ୍ତୁ syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  ଫେରନ୍ତୁ syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) fd;
  (ଶୂନ୍ୟ) dirfd;
  (ଶୂନ୍ୟ) name;
  (ଶୂନ୍ୟ) syscall_move_mount;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

ଗଣନା
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_mount (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *source, ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd,
                     ଅଚଳ ଅକ୍ଷର *target, ଅଚଳ ଅକ୍ଷର *fstype, unsigned long mountflags,
                     ଅଚଳ ଶୂନ୍ୟ *data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra_flags, libcrun_error_t *err);

ସ୍ଥିର bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, ଅଚଳ ଅକ୍ଷର *option)
{
  size_t i;
  ପାଇଁ (i = 0; i < mnt->options_len; i++)
    ଯଦି (mnt->options[i] != ଖାଲି && strcmp (mnt->options[i], option) == 0)
      ଫେରନ୍ତୁ true;
  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    {
      ଯଦି (strcmp (def->mounts[i]->destination, destination) == 0)
        ଫେରନ୍ତୁ def->mounts[i];
    }
  ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଶୂନ୍ୟ
warn_tmpfs_fallback_once (ସଂରଚନା private_data_s *private_data, ଅଚଳ ଅକ୍ଷର *reason)
{
  ଯଦି (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_shared_empty_dir_cached (libcrun_container_t *container, ଅକ୍ଷର **proc_fd_path, libcrun_error_t *err)
{
  ସଂରଚନା private_data_s *private_data = get_private_data (container);
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  cleanup_free ଅକ୍ଷର *empty_dir_path = ଖାଲି;
  proc_fd_path_t fd_path;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* Fast path: return cached proc fd path if already set up */
  ଯଦି (private_data->maskdir_proc_path != ଖାଲି)
    {
      ଯଦି (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      ଫେରନ୍ତୁ 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_ମୂଳ, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  ଯଦି (fd < 0)
    ଫେରନ୍ତୁ fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  ଯଦି (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର bool
is_dev_null (ଅଚଳ ସଂରଚନା stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  ଫେରନ୍ତୁ S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ensure_dev_null (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, libcrun_error_t *err)
{
  ସଂରଚନା stat st;

  ଯଦି (UNLIKELY (fstat (fd, &st) < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "stat `/dev/null`");

  ଯଦି (UNLIKELY (! is_dev_null (&st)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "`/dev/null` is not the null device");

  ଫେରନ୍ତୁ 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
ସ୍ଥିର bool
is_empty_ro_tmpfs (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
  ସଂରଚନା statfs sfs;

  ଯଦି (fstatfs (fd, &sfs) < 0)
    ଫେରନ୍ତୁ false;

  ଫେରନ୍ତୁ sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
mount_masked_dir (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pathfd, ଅଚଳ ଅକ୍ଷର *rel_path, libcrun_error_t *err)
{
  ସଂରଚନା private_data_s *private_data = get_private_data (container);
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା newfd = -1;
  libcrun_error_t tmp_err = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (private_data->maskdir_fd < 0)
    {
      ଯଦି (private_data->maskdir_bind_failed)
        ଯାଅ fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, ଖାଲି, &tmp_err);
      ଯଦି (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->ସନ୍ଦେଶ);
          crun_error_release (&tmp_err);
          ଯାଅ fallback_to_tmpfs;
        }
    }

  ଯଦି (private_data->maskdir_fd >= 0)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootfsfd = private_data->rootfsfd;
      ସଂରଚନା stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, ଖାଲି);
      ଯଦି (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          ଯଦି (UNLIKELY (newfd < 0))
            ଫେରନ୍ତୁ newfd;

          ret = fstat (newfd, &after);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          ଯଦି (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            ଫେରନ୍ତୁ crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          ଫେରନ୍ତୁ 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_path);

      TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
      private_data->maskdir_fd = -1;
      private_data->maskdir_move_failed = true;
    }

  private_data->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_data, tmp_err ? tmp_err->ସନ୍ଦେଶ : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  ଯଦି (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      ଯଦି (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      ଅନ୍ୟଥା
        {
          ଯଦି (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_masked_or_readonly_path (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  ଅଚଳ ଅକ୍ଷର *rootfs = get_private_data (container)->rootfs;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pathfd = -1;
  ସଂରଚନା statfs sfs;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  ଯଦି (UNLIKELY (rel_path == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  ଯଦି (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  ଯଦି (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      ଯଦି (errno != ENOENT && errno != EACCES)
        ଫେରନ୍ତୁ pathfd;

      ଯଦି (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      ଫେରନ୍ତୁ 0;
    }

  ଯଦି (readonly)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା mountfd = -1;

      ଯଦି (UNLIKELY (procfd < 0))
        ଫେରନ୍ତୁ procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      ଯଦି (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          ଯଦି (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, ଖାଲି);
          ଅନ୍ୟଥା
            ret = -1;

          ଯଦି (ret < 0)
            crun_error_release (err);
        }

      ଯଦି (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          ଯଦି (keep_flags)
            {
              cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              ଯଦି (UNLIKELY (sfd < 0))
                ଫେରନ୍ତୁ crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          ଯଦି (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;

              ret = fs_move_mount_to (mountfd, pathfd, ଖାଲି);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          ଅନ୍ୟଥା
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, ଖାଲି, mount_flags, ଖାଲି,
                              LABEL_NONE, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;
            }
        }
    }
  ଅନ୍ୟଥା
    {
      ret = get_file_type_fd (pathfd, &mode);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      ଯଦି ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      ଅନ୍ୟଥା
        {
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା nullfd = -1;
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          ଯଦି (UNLIKELY (nullfd < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          ଯଦି (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, ଖାଲି);

          ଯଦି (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, ଖାଲି, MS_BIND | MS_RDONLY, ଖାଲି,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  ଯଦି (context_type)
    {
      ଯଦି (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          ଫେରନ୍ତୁ ଖାଲି;
        }
      ଫେରନ୍ତୁ context_type;
    }

  ଫେରନ୍ତୁ "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
diagnose_mount_failure (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, libcrun_error_t *err, libcrun_container_t *container,
                        ଅଚଳ ଅକ୍ଷର *fstype)
{
  ଯଦି (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      ଯଦି (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          ଫେରନ୍ତୁ ret;
        }
    }
  ଫେରନ୍ତୁ ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sysfs_userns_fallback (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *target,
                       ଅଚଳ ଅକ୍ଷର *real_target, ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd,
                       unsigned long mountflags, ଅଚଳ ଅକ୍ଷର *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା mountfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysfd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sys_old_ମୂଳ_fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    ଫେରନ୍ତୁ 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (ret == 0)
    ଫେରନ୍ତୁ 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_ମୂଳ_fd = get_old_ମୂଳ_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  ଯଦି (sys_old_ମୂଳ_fd >= 0)
    mountfd = get_bind_mount (sys_old_ମୂଳ_fd, "sys", true, true, false, MS_PRIVATE, err);
  ଯଦି (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  ଯଦି (cgroup_mount == ଖାଲି)
    {
      ଯଦି (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, ଖାଲି);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      ଅନ୍ୟଥା
        {
          crun_error_release (err);
          ଯଦି (sys_old_ମୂଳ_fd >= 0)
            {
              cleanup_free ଅକ୍ଷର *sys_path = ଖାଲି;
              proc_fd_path_t sys_ମୂଳ_path;

              get_proc_self_fd_path (sys_ମୂଳ_path, sys_old_ମୂଳ_fd);
              xasprintf (&sys_path, "%s/sys", sys_ମୂଳ_path);
              ret = mount (sys_path, real_target, ଖାଲି, MS_BIND | MS_REC, ଖାଲି);
            }
          ଅନ୍ୟଥା
            ret = mount ("/sys", real_target, ଖାଲି, MS_BIND | MS_REC, ଖାଲି);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      ଯଦି (UNLIKELY (sysfd < 0))
        ଫେରନ୍ତୁ sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        ଖାଲି, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଫେରନ୍ତୁ do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  ଯଦି (UNLIKELY (mountfd < 0))
    ଫେରନ୍ତୁ mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  ଯଦି (UNLIKELY (sysfd < 0))
    ଫେରନ୍ତୁ sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
ସ୍ଥିର ଶୂନ୍ୟ
apply_selinux_xattr (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra_flags, ଅଚଳ ଅକ୍ଷର *label)
{
  libcrun_error_t tmp_err = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା procfd;

  ଯଦି ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    ଫେରନ୍ତୁ;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  ଯଦି (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      ଯଦି (xfd >= 0)
        (ଶୂନ୍ୟ) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_mount_replace_rootfs (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *fd, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା procfd = get_procfd (get_private_data (container), err);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmp;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (UNLIKELY (procfd < 0))
    ଫେରନ୍ତୁ procfd;

  {
    cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    ଯଦି (UNLIKELY (nsfd < 0))
      ଫେରନ୍ତୁ crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    ଯଦି (UNLIKELY (ret < 0))
      ଫେରନ୍ତୁ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (*fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  ଯଦି (UNLIKELY (tmp < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  ଫେରନ୍ତୁ 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
apply_propagation_flags (ଅଚଳ ଅକ୍ଷର *target, ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd, ଅଚଳ ଅକ୍ଷର *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (targetfd >= 0)
    {
      libcrun_error_t tmp_err = ଖାଲି;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      ଯଦି (LIKELY (ret == 0))
        propagation_done = true;
      ଅନ୍ୟଥା
        crun_error_release (&tmp_err);
    }

  ଯଦି (! propagation_done)
    {
      ret = mount (ଖାଲି, real_target, ଖାଲି, mountflags & ALL_PROPAGATIONS, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  ଫେରନ୍ତୁ 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
schedule_or_do_remount (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd,
                        ଅଚଳ ଅକ୍ଷର *target, ଅଚଳ ଅକ୍ଷର *real_target,
                        bool single_instance, unsigned long mountflags,
                        ଅଚଳ ଶୂନ୍ୟ *data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  ସଂରଚନା remount_s *r;

  ଯଦି ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    ଫେରନ୍ତୁ do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  ଯଦି (*fd < 0)
    {
      *fd = dup (targetfd);
      ଯଦି (UNLIKELY (*fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_mount (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *source, ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd,
          ଅଚଳ ଅକ୍ଷର *target, ଅଚଳ ଅକ୍ଷର *fstype, unsigned long mountflags, ଅଚଳ ଶୂନ୍ୟ *data,
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra_flags, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *data_with_label = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *data_without_label = data;
  proc_fd_path_t target_buffer;
  ଅଚଳ ଅକ୍ଷର *context_type = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା ms_move_fd = -1;
  ଅଚଳ ଅକ୍ଷର *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ଅଚଳ ଅକ୍ଷର *label = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

  ଯଦି (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  ଅନ୍ୟଥା
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  ଯଦି (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  ଯଦି ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      ଯଦି (UNLIKELY (context_type == ଖାଲି))
        ଫେରନ୍ତୁ -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      ଯଦି (ret < 0)
        ଫେରନ୍ତୁ ret;
      data = data_with_label;
    }

  ଯଦି (mountflags & MS_MOVE)
    {
      ଯଦି ((mountflags & MS_BIND) || fstype)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, ଖାଲି, MS_MOVE, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      ଯଦି (UNLIKELY (ms_move_fd < 0))
        ଫେରନ୍ତୁ ms_move_fd;
      targetfd = ms_move_fd;
    }

  ଯଦି ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      ଯଦି (targetfd >= 0)
        {
          ଯଦି (mountflags & MS_BIND)
            {
              cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା bindfd = -1;
              libcrun_error_t tmp_err = ଖାଲି;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              ଯଦି (bindfd < 0)
                {
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_ମୂଳ_fd = get_old_ମୂଳ_fd (get_private_data (container));

                  ଯଦି (old_ମୂଳ_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_ମୂଳ_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              ଯଦି (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, ଖାଲି);
              ଯଦି (ret < 0)
                crun_error_release (&tmp_err);
            }
          ଅନ୍ୟଥା
            {
              cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              ଯଦି (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, ଖାଲି);
            }
        }

      ଯଦି (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          ଯଦି (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_ମୂଳ_fd = get_old_ମୂଳ_fd (get_private_data (container));

              ଯଦି (old_ମୂଳ_fd >= 0)
                {
                  cleanup_free ଅକ୍ଷର *proc_source_buf = ଖାଲି;
                  proc_fd_path_t old_ମୂଳ_path;

                  get_proc_self_fd_path (old_ମୂଳ_path, old_ମୂଳ_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_ମୂଳ_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      ଯଦି (UNLIKELY (ret < 0))
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          ଯଦି (ret == 0)
            ଫେରନ୍ତୁ 0;
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          ଫେରନ୍ତୁ diagnose_mount_failure (ret, err, container, fstype);
        }

      ଯଦି (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          ଯଦି (UNLIKELY (fd < 0))
            ଫେରନ୍ତୁ fd;

          /* We are replacing the rootfs, reopen it.  */
          ଯଦି (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;
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

  ଯଦି (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  ଯଦି (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  ଯଦି (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ
try_umount (ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd, ଅଚଳ ଅକ୍ଷର *target)
{
  proc_fd_path_t target_buffer;
  ଅଚଳ ଅକ୍ଷର *real_target = target;

  ଯଦି (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

ସ୍ଥିର bool
container_has_cgroupns (libcrun_container_t *container)
{
  ଫେରନ୍ତୁ get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_mount_cgroup_v2 (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd, ଅଚଳ ଅକ୍ଷର *target,
                    unsigned long mountflags, ଅଚଳ ଅକ୍ଷର *data, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = crun_error_get_errno (err);
      ଯଦି (saved_errno == EPERM || saved_errno == EBUSY)
        {
          ଅଚଳ ଅକ୍ଷର *unified_cgroup_path;
          ଅଚଳ ଅକ୍ଷର *src_cgroup;

          crun_error_release (err);

          ଯଦି (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              ଯଦି (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  ଯଦି (LIKELY (ret == 0))
                    ଫେରନ୍ତୁ ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_ମୂଳ;
          ଫେରନ୍ତୁ do_mount (container, src_cgroup, targetfd, target, ଖାଲି,
                           MS_BIND | mountflags, ଖାଲି, LABEL_NONE, err);
        }

      ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *source, ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd, ଅଚଳ ଅକ୍ଷର *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ଅଚଳ ଅକ୍ଷର *subsystem = "systemd";
  cleanup_free ଅକ୍ଷର *subsystem_path = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  ଯଦି (UNLIKELY (tmpfsdirfd < 0))
    ଫେରନ୍ତୁ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "openat `%s`", subsystem_path);

  ଫେରନ୍ତୁ do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_mount_cgroup_v1 (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *source, ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd, ଅଚଳ ଅକ୍ଷର *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  ଅକ୍ଷର *from;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmpfsdirfd = -1;
  ଅକ୍ଷର *saveptr = ଖାଲି;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  ଯଦି (UNLIKELY (tmpfsdirfd < 0))
    ଫେରନ୍ତୁ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା procfd = get_procfd (get_private_data (container), err);
    ଯଦି (UNLIKELY (procfd < 0))
      ଫେରନ୍ତୁ procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, ଖାଲି, err);
  }
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (UNLIKELY (content == ଖାଲି || content[0] == '\0'))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ପାଇଁ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ଖାଲି, "\n", &saveptr))
    {
      cleanup_free ଅକ୍ଷର *source_path = ଖାଲି;
      cleanup_free ଅକ୍ଷର *source_subsystem = ଖାଲି;
      cleanup_free ଅକ୍ଷର *subsystem_path = ଖାଲି;
      ଅକ୍ଷର *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      ଯଦି (subsystem[0] == '\0')
        ଜାରି;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      ଯଦି (it)
        subsystem = it + 5;

      ଯଦି (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ଯଦି (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_ମୂଳ, subsystem, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      /* if there is already a mount specified, do not add a default one.  */
      ଯଦି (find_mount_for (container, source_subsystem) != ଖାଲି)
        ଜାରି;

      ret = append_paths (&source_path, err, source_subsystem, subpath, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      ଯଦି (UNLIKELY (subsystemfd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", subsystem_path);

      ଯଦି (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ଯଦି (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  ଜାରି;
                }
              ଫେରନ୍ତୁ ret;
            }
        }
      ଅନ୍ୟଥା
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, ଖାଲି, MS_BIND | mountflags, ଖାଲି,
                          LABEL_NONE, err);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ଯଦି (crun_error_get_errno (err) != ENOENT)
                ଫେରନ୍ତୁ ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, ଖାଲି, MS_BIND | mountflags,
                              ଖାଲି, LABEL_NONE, err);
              ଯଦି (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  ଯଦି (crun_error_get_errno (err) != ENOENT)
                    ଫେରନ୍ତୁ ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_mount_cgroup (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *source, ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd, ଅଚଳ ଅକ୍ଷର *target,
                 unsigned long mountflags, ଅଚଳ ଅକ୍ଷର *data, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ବାଛ (cgroup_mode)
    {
    ସ୍ଥିତି CGROUP_MODE_UNIFIED:
      ଫେରନ୍ତୁ do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    ସ୍ଥିତି CGROUP_MODE_LEGACY:
    ସ୍ଥିତି CGROUP_MODE_HYBRID:
      ଫେରନ୍ତୁ do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  ଫେରନ୍ତୁ crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

ସଂରଚନା device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
ସ୍ଥିର ଶୂନ୍ୟ
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  ସଂରଚନା private_data_s *private_data = get_private_data (container);
  size_t i;

  ଯଦି (private_data->needed_devs_fds)
    {
      ସଂରଚନା libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      ପାଇଁ (i = 0; needed_devs[i].path; i++)
        {
          ଯଦି (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            ଜାରି;

          ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = syscall_open_ବୃକ୍ଷ (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          ଯଦି (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          ଅନ୍ୟଥା ଯଦି (fd >= 0)
            close (fd);
        }
    }

  ଯଦି (private_data->notify_socket_ବୃକ୍ଷ_fd < 0
      && private_data->host_notify_socket_path)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = syscall_open_ବୃକ୍ଷ (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      ଯଦି (fd >= 0)
        private_data->notify_socket_ବୃକ୍ଷ_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
relative_path_under_dev (ଅଚଳ ଅକ୍ଷର *path)
{
  ଯଦି (path[0] != '/')
    ଫେରନ୍ତୁ path;
  ଯଦି (strncmp (path, "/dev/", 5) == 0)
    {
      ଯଦି (strchr (path + 5, '/'))
        ଫେରନ୍ତୁ ଖାଲି;
      ଫେରନ୍ତୁ path + 5;
    }
  ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
mknod_and_set_attrs (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *name, ସଂରଚନା device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  proc_fd_path_t fd_path;
  ଅଚଳ ଅକ୍ଷର *what = "new";
  bool exists = false;
  ସଂରଚନା stat st;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା procfd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (errno != EEXIST)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  ଯଦି (UNLIKELY ((st.st_mode & S_IFMT) != type))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  ଯଦି (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  ଯଦି (exists)
    ଫେରନ୍ତୁ 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  ଯଦି (UNLIKELY (procfd < 0))
    ଫେରନ୍ତୁ procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fchown `%s`", device->path);

  ଫେରନ୍ତୁ 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_create_dev (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା devfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcfd,
                    ସଂରଚନା device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  ଅଚଳ ଅକ୍ଷର *fullname = device->path;
  ଅଚଳ ଅକ୍ଷର *rootfs = get_private_data (container)->rootfs;
  ଯଦି (is_empty_string (fullname))
    ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free ଅକ୍ଷର *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  ଯଦି (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  ଅଚଳ ଅକ୍ଷର *rel_dev = relative_path_under_dev (normalized_path);

  ଯଦି (binds)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

      ଯଦି (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          ଯଦି (UNLIKELY (fd < 0))
            {
              ଯଦି (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              ଯଦି (UNLIKELY (fd < 0))
                ଫେରନ୍ତୁ crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      ଅନ୍ୟଥା
        {
          ଅଚଳ ଅକ୍ଷର *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          ଯଦି (UNLIKELY (fd < 0))
            ଫେରନ୍ତୁ fd;
        }

      ଯଦି (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          ଯଦି (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              ଯଦି (LIKELY (ret >= 0))
                ଫେରନ୍ତୁ 0;
            }
          ଅନ୍ୟଥା
            crun_error_release (err);
        }

      {
        cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        ଯଦି (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            ଯଦି (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, ଖାଲି);
                ଯଦି (LIKELY (ret == 0))
                  ଫେରନ୍ତୁ 0;
              }
            ଅନ୍ୟଥା
              crun_error_release (err);
          }
        ଅନ୍ୟଥା
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, ଖାଲି, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, ଖାଲି, LABEL_MOUNT, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      ଯଦି (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          ଯଦି (ret <= 0)
            ଫେରନ୍ତୁ ret;
        }
      ଅନ୍ୟଥା
        {
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd = -1;
          cleanup_free ଅକ୍ଷର *dirname = ଖାଲି;
          ଅକ୍ଷର *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          ଯଦି (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          ଯଦି (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              ଯଦି (UNLIKELY (dirfd < 0))
                ଫେରନ୍ତୁ crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          ଅନ୍ୟଥା
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              ଯଦି (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              ଯଦି (UNLIKELY (dirfd < 0))
                ଫେରନ୍ତୁ dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          ଯଦି (ret <= 0)
            ଫେରନ୍ତୁ ret;
        }
    }
  ଫେରନ୍ତୁ 0;
}

ସଂରଚନା symlink_s
{
  ଅଚଳ ଅକ୍ଷର *path;
  ଅଚଳ ଅକ୍ଷର *target;
  bool force;
};

ସ୍ଥିର ସଂରଚନା symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { ଖାଲି, ଖାଲି, false } };

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  size_t i;
  ସଂରଚନା device_s *it;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  ଅଚଳ ଅକ୍ଷର *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map ସଂରଚନା libcrun_fd_map *dev_fds = ଖାଲି;
  cleanup_close_map ସଂରଚନା libcrun_fd_map *needed_fds = ଖାଲି;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = ଖାଲି;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = ଖାଲି;

  ଯଦି (! def || ! def->linux)
    ଫେରନ୍ତୁ 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  ଯଦି (UNLIKELY (devfd < 0))
    ଫେରନ୍ତୁ devfd;

  ପାଇଁ (i = 0; i < def->linux->devices_len; i++)
    {
      ସଂରଚନା device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      ଯଦି (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ପାଇଁ (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      ଯଦି (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  ପାଇଁ (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;

          ଯଦି (errno == EEXIST && ! symlinks[i].force)
            ଜାରି;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          ଯଦି (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              ଯଦି (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              ଯଦି (ret < 0 && errno == EBUSY)
                {
                  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  ଯଦି (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      ଯଦି (umount2 (procpath, MNT_DETACH) == 0)
                        ଯାଅ retry_unlink;
                    }
                }
              ଯଦି (ret == 0)
                ଯାଅ retry_symlink;
            }
          ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  ଯଦି (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
next_path_component (ଅଚଳ ଅକ୍ଷର *p, size_t *len)
{
  ପାଇଁ (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      ଯଦି (*len != 1 || p[0] != '.')
        ଫେରନ୍ତୁ p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
ସ୍ଥିର bool
same_path (ଅଚଳ ଅକ୍ଷର *a, ଅଚଳ ଅକ୍ଷର *b)
{
  size_t la, lb;

  ପାଇଁ (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      ଯଦି (la != lb || memcmp (a, b, la) != 0)
        ଫେରନ୍ତୁ false;
      ଯଦି (la == 0)
        ଫେରନ୍ତୁ true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
ସ୍ଥିର bool
is_duplicate_path (ଅକ୍ଷର **paths, size_t i)
{
  size_t j;

  ଯଦି (paths[i] == ଖାଲି)
    ଫେରନ୍ତୁ false;

  ପାଇଁ (j = 0; j < i; j++)
    ଯଦି (paths[j] && same_path (paths[j], paths[i]))
      ଫେରନ୍ତୁ true;

  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  ଯଦି (! def || ! def->linux)
    ଫେରନ୍ତୁ 0;

  ପାଇଁ (i = 0; i < def->linux->masked_paths_len; i++)
    {
      ଯଦି (is_duplicate_path (def->linux->masked_paths, i))
        ଜାରି;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ପାଇଁ (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_pivot (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rootfs, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା oldrootfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା newrootfd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (oldrootfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (newrootfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_ମୂଳ (".", ".");
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, ଖାଲି, -1, ".", ଖାଲି, MS_REC | MS_PRIVATE, ଖାଲି, LABEL_MOUNT, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = umount2 (".", MNT_DETACH);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "umount2 oldroot");

  କର
    {
      ret = umount2 (".", MNT_DETACH);
      ଯଦି (ret < 0 && errno == EINVAL)
        ଭାଙ୍ଗ;
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "umount2 oldroot");
  } ଯେତେବେଳେ (ret == 0);

  ret = chdir ("/");
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "chdir to newroot");

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, ଅକ୍ଷର **data, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ସଂରଚନା stat st;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (*data != ଖାଲି && strstr (*data, "mode="))
    ଫେରନ୍ତୁ 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  ଯଦି (fd < 0)
    {
      ଯଦି (crun_error_get_errno (err) != ENOENT)
        ଫେରନ୍ତୁ fd;

      crun_error_release (err);
      ଫେରନ୍ତୁ 0;
    }
  ret = fstat (fd, &st);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_default_flags (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *destination, ଅକ୍ଷର **data)
{
  ଯଦି (strcmp (destination, "/proc") == 0)
    ଫେରନ୍ତୁ 0;
  ଯଦି (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      ଫେରନ୍ତୁ MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  ଯଦି (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      ଫେରନ୍ତୁ MS_NOEXEC | MS_STRICTATIME;
    }
  ଯଦି (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      ଫେରନ୍ତୁ MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  ଯଦି (strcmp (destination, "/dev/mqueue") == 0)
    ଫେରନ୍ତୁ MS_NOEXEC | MS_NOSUID | MS_NODEV;
  ଯଦି (strcmp (destination, "/dev/pts") == 0)
    {
      ଯଦି (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      ଅନ୍ୟଥା
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      ଫେରନ୍ତୁ MS_NOEXEC | MS_NOSUID;
    }
  ଯଦି (strcmp (destination, "/sys") == 0)
    ଫେରନ୍ତୁ MS_NOEXEC | MS_NOSUID | MS_NODEV;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  ଫେରନ୍ତୁ find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
check_valid_no_follow_file_system (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *destination, libcrun_error_t *err)
{
  ସଂରଚନା statfs sfs;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = fstatfs (fd, &sfs);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "statfs `%s`", destination);

  ବାଛ (sfs.f_type)
    {
    ସ୍ଥିତି PROC_SUPER_MAGIC:
    ସ୍ଥିତି SYSFS_MAGIC:
    ସ୍ଥିତି DEVPTS_SUPER_MAGIC:
      ଫେରନ୍ତୁ crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
safe_create_symlink (ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootfsfd, ଅଚଳ ଅକ୍ଷର *rootfs, ଅଚଳ ଅକ୍ଷର *target, ଅଚଳ ଅକ୍ଷର *destination, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent_dir_fd = -1;
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  ଅକ୍ଷର *part;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (is_empty_string (destination))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  ଯଦି (UNLIKELY (parent_dir_fd < 0))
    ଫେରନ୍ତୁ parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      ଯଦି (errno == EEXIST)
        {
          cleanup_free ଅକ୍ଷର *link = ଖାଲି;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          ଯଦି (UNLIKELY (len < 0))
            ଫେରନ୍ତୁ len;

          ଯଦି ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            ଫେରନ୍ତୁ 0;

          ଫେରନ୍ତୁ crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      ଫେରନ୍ତୁ crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
handle_copy_symlink (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     ଅଚଳ ଅକ୍ଷର *link_target, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *target = ଖାଲି;

  ଯଦି (link_target == ଖାଲି)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_ମୂଳ_fd = get_old_ମୂଳ_fd (get_private_data (container));
      ଅଚଳ ଅକ୍ଷର *source = mount->source;
      ssize_t len;

      ଯଦି (old_ମୂଳ_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_ମୂଳ_fd, source + 1, &target, 0, err);
      ଅନ୍ୟଥା
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      ଯଦି (UNLIKELY (len < 0))
        ଫେରନ୍ତୁ len;
      link_target = target;
    }

  ଫେରନ୍ତୁ safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
prepare_sysfs_or_proc_mount (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *target,
                             ପୂର୍ଣ୍ଣସଂଖ୍ୟା *targetfd, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (errno == ENOENT)
        {
          ଯଦି (strchr (target, '/'))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      ଅନ୍ୟଥା ଯଦି (errno == ENOTDIR)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "the target `/%s` is invalid", target);

      ଯଦି (ret < 0)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
handle_tmpcopyup (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rootfs, ଅଚଳ ଅକ୍ଷର *target,
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy_from_fd, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା destfd, ret;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmpfd = copy_from_fd;

  target = consume_slashes (target);

  ଯଦି (is_empty_string (target))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  ଯଦି (UNLIKELY (destfd < 0))
    ଫେରନ୍ତୁ crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_mount_label_how (ଅଚଳ ଅକ୍ଷର *type, bool is_sysfs_or_proc)
{
  ଯଦି (is_sysfs_or_proc)
    ଫେରନ୍ତୁ LABEL_NONE;

  ଯଦି (strcmp (type, "mqueue") == 0)
    ଫେରନ୍ତୁ LABEL_XATTR;

  ଫେରନ୍ତୁ LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
ସ୍ଥିର unsigned long
get_bind_mount_locked_flags (ପୂର୍ଣ୍ଣସଂଖ୍ୟା source_mountfd, ଅଚଳ ଅକ୍ଷର *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  ସଂରଚନା statfs sfs;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  ଅନ୍ୟଥା
    ret = statfs (source, &sfs);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ 0;

  ପାଇଁ (i = 0; i < mount->options_len; i++)
    {
      ଅଚଳ ସଂରଚନା propagation_flags_s *prop;

      ଯଦି (mount->options[i] == ଖାଲି)
        ଜାରି;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ଯଦି (prop == ଖାଲି || (prop->extra_flags & OPTION_RECURSIVE))
        ଜାରି;

      ଯଦି (prop->clear)
        cleared |= prop->flags;
      ଅନ୍ୟଥା
        cleared &= ~prop->flags;
    }

  ଫେରନ୍ତୁ sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
get_path_in_rootfs (ଅଚଳ ଅକ୍ଷର *rootfs, ଅଚଳ ଅକ୍ଷର *path)
{
  size_t len;

  ଯଦି (rootfs == ଖାଲି || path == ଖାଲି)
    ଫେରନ୍ତୁ ଖାଲି;

  len = strlen (rootfs);
  ଯେତେବେଳେ (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  ଯଦି (len == 0)
    ଫେରନ୍ତୁ ଖାଲି;

  ଯଦି (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    ଫେରନ୍ତୁ ଖାଲି;

  ଫେରନ୍ତୁ consume_slashes (path + len);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
process_single_mount (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      ସଂରଚନା libcrun_fd_map *mount_fds, size_t mount_index,
                      ଅଚଳ ଅକ୍ଷର *systemd_cgroup_v1, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *target = consume_slashes (mount->destination);
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା source_mountfd = -1;
  cleanup_free ଅକ୍ଷର *data = ଖାଲି;
  ଅକ୍ଷର *type;
  ଅକ୍ଷର *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy_from_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_remount = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  ଯଦି (mount->options == ଖାଲି)
    flags = get_default_flags (container, mount->destination, &data);
  ଅନ୍ୟଥା
    {
      size_t j;

      ପାଇଁ (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  ଯଦି (type == ଖାଲି && (flags & MS_BIND) == 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  ଯଦି ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      ଅଚଳ ଅକ୍ଷର *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      ଯଦି (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          ଯଦି (UNLIKELY (source_mountfd < 0))
            ଫେରନ୍ତୁ crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  ଯଦି ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  ଯଦି (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      ଯଦି (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      ଯଦି (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  ଯଦି (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      ଅଚଳ ଅକ୍ଷର *path = mount->source;

      ଯଦି ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      ଯଦି (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, ଖାଲି);
        }
      ଅନ୍ୟଥା ଯଦି (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      ଅନ୍ୟଥା ଯଦି (path[0] == '/')
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_ମୂଳ_fd = get_old_ମୂଳ_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          ଯଦି (old_ମୂଳ_fd >= 0)
            ret = get_file_type_at (old_ମୂଳ_fd, &src_mode, nofollow, (path[1] == '\0') ? ଖାଲି : path + 1);
          ଅନ୍ୟଥା
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      ଅନ୍ୟଥା
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot stat `%s`", path);

      ଯଦି (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା bind_dirfd = AT_FDCWD;
          ଅଚଳ ଅକ୍ଷର *bind_source = mount->source;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_ମୂଳ_fd = get_old_ମୂଳ_fd (get_private_data (container));

          ଯଦି (old_ମୂଳ_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_ମୂଳ_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          source_mountfd = ret;
        }
    }

  ଯଦି (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      ଅଚଳ ଅକ୍ଷର *cached_target = ଖାଲି;

      ଯଦି (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      mounted = true;
    }
  ଅନ୍ୟଥା ଯଦି (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      bool is_dir = S_ISDIR (src_mode);

      ଯଦି (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
      ଅନ୍ୟଥା
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          targetfd = ret;
        }
    }

  ଯଦି (extra_flags & OPTION_TMPCOPYUP)
    {
      ଯଦି (strcmp (type, "tmpfs") != 0)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      ଯଦି (UNLIKELY (copy_from_fd < 0))
        {
          ଯଦି (errno != ENOTDIR)
            ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  ଯଦି (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, ଖାଲି);
      ଯଦି (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          ଯଦି (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, ଖାଲି, source_mountfd, target, ଖାଲି, remaining_flags, data, LABEL_NONE | no_remount, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;
            }

          mounted = true;

          ଯଦି (is_empty_string (target))
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              ଯଦି (UNLIKELY (new_rootfsfd < 0))
                ଫେରନ୍ତୁ crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      ଅନ୍ୟଥା ଯଦି (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          ଫେରନ୍ତୁ crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      ଅନ୍ୟଥା ଯଦି (get_private_data (container)->host_ମୂଳ_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          ଫେରନ୍ତୁ crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      ଅନ୍ୟଥା ଯଦି (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  ଯଦି (! mounted)
    {
      ଯଦି (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
      ଅନ୍ୟଥା ଯଦି (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
      ଅନ୍ୟଥା
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }

  ଯଦି (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (rec_clear || rec_set)
    {
      ଅଚଳ bool is_dir = S_ISDIR (src_mode);
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      ଯଦି (UNLIKELY (dfd < 0))
        ଫେରନ୍ତୁ crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_mounts (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ଅଚଳ ଅକ୍ଷର *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map ସଂରଚନା libcrun_fd_map *mount_fds = ଖାଲି;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = ଖାଲି;

  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_do_bind_mount (libcrun_container_t *container, ଅକ୍ଷର *mount_source, ଅକ୍ଷର *mount_destination, ଅକ୍ଷର **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *target = consume_slashes (mount_destination);
  cleanup_free ଅକ୍ଷର *data = ଖାଲି;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  ଅଚଳ ଅକ୍ଷର *rootfs = get_private_data (container)->rootfs;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି ((get_private_data (container)->rootfsfd < 0) || (rootfs == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  ଯଦି (mount_options == ଖାଲି)
    flags = get_default_flags (container, mount_destination, &data);
  ଅନ୍ୟଥା
    {
      size_t j;

      ପାଇଁ (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  ଯଦି (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  ଯଦି (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      ଯଦି (UNLIKELY (is_dir < 0))
        ଫେରନ୍ତୁ is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା notify_fd = -1;
  cleanup_free ଅକ୍ଷର *host_notify_socket_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *state_dir = ଖାଲି;
  ଅକ୍ଷର *host_path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (container && get_private_data (container)->host_notify_socket_path)
    {
      ଅଚଳ ଅକ୍ଷର *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  ଯଦି (host_path == ଖାଲି)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_ମୂଳ, context->id, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  ଯଦି (UNLIKELY (notify_fd < 0))
    ଫେରନ୍ତୁ notify_fd;

  ଯଦି (UNLIKELY (chmod (host_path, 0777) < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  ଯଦି (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (ଶୂନ୍ୟ) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  ଫେରନ୍ତୁ 1;
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) context;
  (ଶୂନ୍ୟ) container;
  (ଶୂନ୍ୟ) err;
  *notify_socket_out = -1;
  ଫେରନ୍ତୁ 0;
#endif
}

#ifdef HAVE_SYSTEMD
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅଚଳ ଅକ୍ଷର *notify_socket = container->context->notify_socket;
  cleanup_free ଅକ୍ଷର *host_notify_socket_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *container_notify_socket_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *state_dir = ଖାଲି;
  uid_t container_ମୂଳ_uid = -1;
  gid_t container_ମୂଳ_gid = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା notify_socket_ବୃକ୍ଷ_fd;

  ଯଦି (notify_socket == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_ମୂଳ : ଖାଲି),
                                     (container->context ? container->context->id : ଖାଲି), err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = mkdir (host_notify_socket_path, 0700);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  ଯଦି (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_ମୂଳ_in_the_userns (container->container_def, 0, 0, &container_ମୂଳ_uid, &container_ମୂଳ_gid);
      ଯଦି (container_ମୂଳ_uid != ((uid_t) -1) && container_ମୂଳ_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_ମୂଳ_uid, container_ମୂଳ_gid);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_ମୂଳ_uid, container_ମୂଳ_gid);
        }
    }

  notify_socket_ବୃକ୍ଷ_fd = syscall_open_ବୃକ୍ଷ (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  ଯଦି (notify_socket_ବୃକ୍ଷ_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_ବୃକ୍ଷ_fd = notify_socket_ବୃକ୍ଷ_fd;
  ଅନ୍ୟଥା ଯଦି (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  ଅନ୍ୟଥା ଯଦି (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  ଅନ୍ୟଥା
    /* some other error */
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = ଖାଲି;
  ଫେରନ୍ତୁ 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_finalize_notify_socket (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rootfs, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_free ଅକ୍ଷର *host_notify_socket_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *container_notify_socket_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *container_notify_socket_path_dir_alloc = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *container_notify_socket_path_dir = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା notify_socket_ବୃକ୍ଷ_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = ଖାଲି;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = ଖାଲି;

  ଯଦି (host_notify_socket_path == ଖାଲି || container_notify_socket_path == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ଯଦି (rootfs == ଖାଲି)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  ଯଦି (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  targetfd = ret;

  notify_socket_ବୃକ୍ଷ_fd = get_private_data (container)->notify_socket_ବୃକ୍ଷ_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_ବୃକ୍ଷ_fd = -1;

  ଯଦି (notify_socket_ବୃକ୍ଷ_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_ବୃକ୍ଷ_fd, targetfd, ଖାଲି);
      ଯଦି (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          ଯଦି (UNLIKELY (targetfd < 0))
            ଫେରନ୍ତୁ targetfd;

          ret = do_mount (container, ଖାଲି, targetfd, container_notify_socket_path_dir, ଖାଲି,
                          MS_REC | MS_PRIVATE, ଖାଲି, LABEL_NONE, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
      ଅନ୍ୟଥା ଯଦି (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_ବୃକ୍ଷ_fd,
                                container_notify_socket_path_dir);
    }

  ଯଦି (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, ଖାଲି,
                      MS_BIND | MS_REC | MS_PRIVATE, ଖାଲି, LABEL_MOUNT, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
make_parent_mount_private (ଅଚଳ ଅକ୍ଷର *rootfs, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootfsfd = -1;
  ସଂରଚନା stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (rootfsfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", rootfs);

  ଯଦି (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat `%s`", rootfs);

  ପାଇଁ (;;)
    {
      ସଂରଚନା stat cur_st;
      libcrun_error_t tmp_err = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      ଯଦି (ret == 0)
        ଫେରନ୍ତୁ 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (ଖାଲି, proc_path, ଖାଲି, MS_PRIVATE, ଖାଲି);
      ଯଦି (ret == 0)
        ଫେରନ୍ତୁ 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      ଯଦି (parentfd < 0)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          ଯଦି (ret != 0)
            ଫେରନ୍ତୁ crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      ଯଦି (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      ଯଦି (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        ଭାଙ୍ଗ;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  ଫେରନ୍ତୁ crun_make_error (err, 0, "make `%s` private", rootfs);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_mounts (ସଂରଚନା container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rootfs, set_mounts_cb_t cb, ଶୂନ୍ୟ *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_user_ns = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

  ଯଦି (rootfs == ଖାଲି || def->mounts == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  ଯଦି (def->ମୂଳ->readonly)
    {
      ସଂରଚନା remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

      fd = dup (get_private_data (container)->rootfsfd);
      ଯଦି (UNLIKELY (fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, ଖାଲି, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  ଯଦି (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      ଯଦି (UNLIKELY (is_user_ns < 0))
        ଫେରନ୍ତୁ is_user_ns;
    }

  ଯଦି (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  ଯଦି (cb)
    {
      ret = cb (cb_data, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = ଖାଲି;
      ଅଚଳ ଅକ୍ଷର *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (ଶୂନ୍ୟ) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
umount_or_hide (ଅଚଳ ଅକ୍ଷର *target, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = umount2 (target, MNT_DETACH);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      ଯଦି (errno == EINVAL)
        {
          ret = mount (ଖାଲି, target, "tmpfs", 0, "size=0k");
          ଯଦି (LIKELY (ret == 0))
            ଫେରନ୍ତୁ 0;
        }

      ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
move_ମୂଳ (ଅଚଳ ଅକ୍ଷର *rootfs, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = chdir (rootfs);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = umount_or_hide ("/proc", err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_finalize_mounts (ସଂରଚନା container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rootfs,
                         set_mounts_cb_t after_pivot_cb, ଶୂନ୍ୟ *cb_data, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_ମୂଳ_fd));

  ଯଦି (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      ଯଦି (get_private_data (container)->no_pivot)
        {
          ret = move_ମୂଳ (rootfs, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
      ଅନ୍ୟଥା
        {
          ret = do_pivot (container, rootfs, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ret = do_mount (container, ଖାଲି, -1, "/", ଖାଲି,
                      get_private_data (container)->rootfs_propagation,
                      ଖାଲି, LABEL_MOUNT, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = chdir ("/");
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "chdir to `/`");
    }

  ଯଦି (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = finalize_mounts (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
maybe_open_ବୃକ୍ଷ_namespace (ଅଚଳ ଅକ୍ଷର *rootfs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out_fd, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootfs_fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବୃକ୍ଷ_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (rootfs_fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", rootfs);

  ବୃକ୍ଷ_fd = syscall_open_ବୃକ୍ଷ (rootfs_fd, "",
                               OPEN_ବୃକ୍ଷ_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  ଯଦି (ବୃକ୍ଷ_fd < 0)
    {
      ଯଦି (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = ବୃକ୍ଷ_fd;
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  ସଂରଚନା libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  ଯଦି (mount_fds == ଖାଲି)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  ଫେରନ୍ତୁ mount_fds;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ଅକ୍ଷର *mnt_data = ଖାଲି;
  unsigned long mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  ଅଚଳ ଅକ୍ଷର *label_type = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *label_val = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *fstype;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା mnt_fd;
  size_t j;

  ଯଦି (mount->type == ଖାଲି)
    ଫେରନ୍ତୁ -1;

  fstype = mount->type;

  ପାଇଁ (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  ଯଦି (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      ଯଦି (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          ଯଦି (label_type == ଖାଲି)
            crun_error_release (err);
          ଅନ୍ୟଥା
            label_val = def->linux->mount_label;
        }
    }

  ଯଦି (strcmp (fstype, "cgroup") == 0)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode = libcrun_get_cgroup_mode (err);
      ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  ଯଦି (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, ଖାଲି, ଖାଲି, mnt_data);

  ଫେରନ୍ତୁ mnt_fd;
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
ସଂରଚନା mnt_id_req_s
{
  uint32_t size;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

ସଂରଚନା statmount_s
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
  ଅକ୍ଷର str[];
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_listmount (ସଂରଚନା mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
#ifdef __NR_listmount
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_listmount, req, ids, nr, flags);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) req;
  (ଶୂନ୍ୟ) ids;
  (ଶୂନ୍ୟ) nr;
  (ଶୂନ୍ୟ) flags;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_statmount (ସଂରଚନା mnt_id_req_s *req, ସଂରଚନା statmount_s *buf, size_t size, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
#ifdef __NR_statmount
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_statmount, req, buf, size, flags);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) req;
  (ଶୂନ୍ୟ) buf;
  (ଶୂନ୍ୟ) size;
  (ଶୂନ୍ୟ) flags;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
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
ସ୍ଥିର bool
mounts_hit_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = ଖାଲି;
  size_t i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      ଯଦି (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != ଖାଲି && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          ଭାଙ୍ଗ;
        }
    }
  ଯଦି (! has_absolute_bind)
    ଫେରନ୍ତୁ false;

  ids = xmalloc (capacity * ଆକାର (uint64_t));
  ପାଇଁ (;;)
    {
      ସଂରଚନା mnt_id_req_s req = {
        .size = ଆକାର (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ଯଦି (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * ଆକାର (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      ଯଦି (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        ଫେରନ୍ତୁ true;
      ଯଦି (ret == 0)
        ଭାଙ୍ଗ;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  ପାଇଁ (i = 0; i < n_ids; i++)
    {
      ଅକ୍ଷର buffer[4096] __attribute__ ((aligned (8)));
      ସଂରଚନା statmount_s *sm = (ସଂରଚନା statmount_s *) buffer;
      ସଂରଚନା mnt_id_req_s req = {
        .size = ଆକାର (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      ଅଚଳ ଅକ୍ଷର *ମୂଳ, *point;
      size_t j;

      ଯଦି (syscall_statmount (&req, sm, ଆକାର (buffer), 0) < 0)
        ଜାରି;
      ଯଦି ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        ଜାରି;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      ମୂଳ = sm->str + sm->mnt_root;
      ଯଦି (! has_prefix (ମୂଳ, "mnt:["))
        ଜାରି;

      point = sm->str + sm->mnt_point;

      ପାଇଁ (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          ଅଚଳ ଅକ୍ଷର *source = def->mounts[j]->source;
          size_t source_len;

          ଯଦି (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            ଜାରି;
          ଯଦି (source == ଖାଲି || source[0] != '/')
            ଜାରି;

          source_len = strlen (source);
          ଯେତେବେଳେ (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          ଯଦି (strncmp (point, source, source_len) != 0)
            ଜାରି;

          /* The source itself is a pinned mount namespace.  */
          ଯଦି (point[source_len] == '\0')
            ଫେରନ୍ତୁ true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          ଯଦି (recursive && (source_len == 1 || point[source_len] == '/'))
            ଫେରନ୍ତୁ true;
        }
    }

  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
can_use_open_ବୃକ୍ଷ_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ସଂରଚନା libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  ଯଦି (has_hooks || has_userns)
    ଫେରନ୍ତୁ false;

  ଯଦି (mounts_hit_pinned_mount_namespace (def))
    ଫେରନ୍ତୁ false;

  ଯଦି (container->context->no_open_ବୃକ୍ଷ_namespace)
    ଫେରନ୍ତୁ false;

  {
    libcrun_error_t tmp_err = ଖାଲି;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    ଯଦି (in_userns != 0)
      ଫେରନ୍ତୁ false;
  }

  mount_fds = get_fd_map (container);
  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    {
      ଯଦି (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          ଯଦି (! has_cached_target)
            ଫେରନ୍ତୁ false;
        }
    }

  ଫେରନ୍ତୁ true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
ସ୍ଥିର bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ଯଦି (mount->type == ଖାଲି || strcmp (mount->type, "tmpfs") != 0)
    ଫେରନ୍ତୁ false;

  ପାଇଁ (i = 0; i < mount->options_len; i++)
    ଯଦି (mount->options[i] != ଖାଲି && has_prefix (mount->options[i], "mode="))
      ଫେରନ୍ତୁ false;

  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, ଅକ୍ଷର **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootfsfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବୃକ୍ଷ_fd = -1;
  libcrun_error_t tmp_err = ଖାଲି;
  bool use_open_ବୃକ୍ଷ = false;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, ଖାଲି, ଖାଲି, ଖାଲି, ଖାଲି);

  ଯଦି ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ମୁକ୍ତ (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    {
      ସଂରଚନା libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      ଯଦି (mount_fds && mount_fds->fds[i] >= 0)
        ଜାରି;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      ଯଦି (def->mounts[i]->source != ଖାଲି
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free ଅକ୍ଷର *mnt_data = ଖାଲି;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          ପାଇଁ (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          ଯଦି (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free ଅକ୍ଷର *target = ଖାଲି;
              libcrun_error_t tmp_err = ଖାଲି;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              ଯଦି (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              ଯଦି (get_private_data (container)->copy_symlink_targets == ଖାଲି)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * ଆକାର (ଅକ୍ଷର *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = ଖାଲି;
            }
        }

      ଯଦି (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          ଯଦି (mount_fds && def->mounts[i]->source != ଖାଲି && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == ଖାଲି)
            {
              libcrun_error_t tmp_err = ଖାଲି;
              unsigned long propagation = MS_PRIVATE;

              ଯଦି (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              ଯଦି (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          ଜାରି;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      ଯଦି (mnt_fd < 0)
        {
          crun_error_release (err);
          ଜାରି;
        }

      ଯଦି (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା destfd = -1;
          ସଂରଚନା stat st;

          ଯଦି (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              ଯଦି (UNLIKELY (rootfsfd < 0))
                ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          ଯଦି (destfd < 0)
            crun_error_release (&tmp_err);
          ଅନ୍ୟଥା ଯଦି (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା procfd = get_procfd (get_private_data (container), err);
              ଯଦି (UNLIKELY (procfd < 0))
                ଫେରନ୍ତୁ procfd;

              get_self_fd_path (fd_path, mnt_fd);
              ret = fchmodat (procfd, fd_path, st.st_mode & 07777, 0);
              (ଶୂନ୍ୟ) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_empty_dir_cached (container, ଖାଲି, &tmp_err);
  ଯଦି (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  ଯଦି (! get_private_data (container)->joined_mount_ns
      && can_use_open_ବୃକ୍ଷ_namespace (container))
    use_open_ବୃକ୍ଷ = true;

  ଯଦି (use_open_ବୃକ୍ଷ)
    {
      ret = maybe_open_ବୃକ୍ଷ_namespace (*rootfs, &ବୃକ୍ଷ_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  ଯଦି (ବୃକ୍ଷ_fd >= 0)
    preopen_needed_devs_and_notify (container);

  ଯଦି (ବୃକ୍ଷ_fd >= 0)
    {
      ret = setns (ବୃକ୍ଷ_fd, CLONE_NEWNS);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (ଖାଲି, "/", ଖାଲି, MS_REMOUNT | MS_BIND, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, ଖାଲି, -1, "/", ଖାଲି, MS_REC | MS_PRIVATE, ଖାଲି, LABEL_MOUNT, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = do_mount (container, ଖାଲି, -1, "/", ଖାଲି, rootfs_propagation, ଖାଲି, LABEL_MOUNT, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_ମୂଳ_switched = true;
      ମୁକ୍ତ (*rootfs);
      *rootfs = xstrdup ("/");
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, ଖାଲି, -1, "/", ଖାଲି, rootfs_propagation, ଖାଲି, LABEL_MOUNT, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = make_parent_mount_private (*rootfs, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, ଖାଲି, MS_BIND | MS_REC | MS_PRIVATE, ଖାଲି, LABEL_MOUNT, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  ଯଦି (! container->context->force_no_cgroup)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode = libcrun_get_cgroup_mode (err);
      ଯଦି (UNLIKELY (cgroup_mode < 0))
        ଫେରନ୍ତୁ cgroup_mode;

      ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          ଅକ୍ଷର *unified_cgroup_path = ଖାଲି;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା procfd = get_procfd (get_private_data (container), err);
          ଯଦି (UNLIKELY (procfd < 0))
            ଫେରନ୍ତୁ procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = ଖାଲି;
      ଫେରନ୍ତୁ ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = ଖାଲି;

  ଯଦି (ବୃକ୍ଷ_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  ଯଦି (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_do_pivot_ମୂଳ (libcrun_container_t *container, bool no_pivot, ଅକ୍ଷର **rootfs, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (*rootfs == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ଯଦି (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      ଯଦି (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "chdir to `/`");
        }
    }
  ଅନ୍ୟଥା
    {
      ret = chroot (*rootfs);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "chdir to `/`");

      ମୁକ୍ତ (*rootfs);
      *rootfs = xstrdup ("/");
    }

  ଫେରନ୍ତୁ 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  ସଂରଚନା stat statbuf;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  ଯଦି (UNLIKELY (fd == -1))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ପାଇଁ (i = 0; i <= 2; i++)
    {
      ଯଦି (UNLIKELY (fstat (i, &statbuf) == -1))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "stat fd `%d`", i);
      ଯଦି (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          ଯଦି (UNLIKELY (dup2 (fd, i) == -1))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
uidgidmap_helper (ଅକ୍ଷର *helper, pid_t pid, ଅଚଳ ଅକ୍ଷର *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  ଅକ୍ଷର pid_fmt[16];
  ଅକ୍ଷର *args[MAX_ARGS + 1];
  ଅକ୍ଷର *next;
  cleanup_free ଅକ୍ଷର *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, ଆକାର (pid_fmt), "%d", pid);
  ଯଦି (UNLIKELY (ret >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (pid_fmt)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  ଯେତେବେଳେ (nargs < MAX_ARGS)
    {
      ଅକ୍ଷର *p = strsep (&next, " \n");
      ଯଦି (next == ଖାଲି)
        ଭାଙ୍ଗ;
      args[nargs++] = p;
    }
  args[nargs++] = ଖାଲି;

  ret = run_process (args, err);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ ret;
  ଯଦି (ret > 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
newgidmap (pid_t pid, ଅଚଳ ଅକ୍ଷର *map_file, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ uidgidmap_helper ("newgidmap", pid, map_file, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
newuidmap (pid_t pid, ଅଚଳ ଅକ୍ଷର *map_file, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ uidgidmap_helper ("newuidmap", pid, map_file, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ fd;

  ret = TEMP_FAILURE_RETRY (ଲିଖ (fd, "deny", 4));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;

  ଯଦି (get_private_data (container)->deny_setgroups)
    ଫେରନ୍ତୁ 0;

  ଯଦି (container->container_def->annotations)
    {
      ଅଚଳ ଅକ୍ଷର *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      ଯଦି (annotation)
        ଫେରନ୍ତୁ strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    ଯଦି (fd < 0)
      ret = fd;
    ଅନ୍ୟଥା
      ret = read_all_fd (fd, "self/setgroups", &content, ଖାଲି, err);
  }
  ଯଦି (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      ଯଦି (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ଫେରନ୍ତୁ 1;
        }
      ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = ଖାଲି;
  size_t additional_gids_len = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା can_do_setgroups;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (process != ଖାଲି && process->user != ଖାଲି)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  ଯଦି (UNLIKELY (can_do_setgroups < 0))
    ଫେରନ୍ତୁ can_do_setgroups;

  ଯଦି (can_do_setgroups == 0)
    {
      ଅଚଳ ଅକ୍ଷର *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      ଯଦି (annotation && strcmp (annotation, "0") != 0)
        ଫେରନ୍ତୁ 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      ପାଇଁ (i = 0; i < additional_gids_len; i++)
        ଯଦି (additional_gids[i] != process->user->gid)
          ଫେରନ୍ତୁ crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) additional_gids[i]);
      ଫେରନ୍ତୁ 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "setgroups");

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  ଯଦି (get_private_data (container)->unshare_cgroupns)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = unshare (CLONE_NEWCGROUP);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (errno != EINVAL)
            ଫେରନ୍ତୁ crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  ଫେରନ୍ତୁ 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  ଯଦି (len != 1)
    ଫେରନ୍ତୁ 0;

  ଯଦି (mappings[0]->size != 1)
    ଫେରନ୍ତୁ 0;

  ଯଦି (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_id_map (libcrun_container_t *container, pid_t pid, ଅଚଳ ଅକ୍ଷର *map_name,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*idmap_helper) (pid_t, ଅଚଳ ଅକ୍ଷର *, libcrun_error_t *), ଅଚଳ ଅକ୍ଷର *helper_name,
              bool warn_on_failure, ଅଚଳ ଅକ୍ଷର *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

  ଯଦି (container->host_uid)
    ret = idmap_helper (pid, map, err);
  ଯଦି (container->host_uid == 0 || ret < 0)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

      ଯଦି (ret < 0)
        {
          ଯଦି (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      ଯଦି (UNLIKELY (fd < 0))
        ଫେରନ୍ତୁ fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      ଯଦି (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free ଅକ୍ଷର *single_mapping = ଖାଲି;
          crun_error_release (err);

          ଯଦି (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          ଯଦି (UNLIKELY (fd < 0))
            ଫେରନ୍ତୁ fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *uid_map = ଖାଲି;
  cleanup_free ଅକ୍ଷର *gid_map = ଖାଲି;
  size_t uid_map_len = 0, gid_map_len = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ଯଦି ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    ଫେରନ୍ତୁ 0;

  ଯଦି (! def || ! def->linux)
    ଫେରନ୍ତୁ 0;

  ଯଦି (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      uid_map_len = (size_t) ret;
      ଯଦି (uid_map == ଖାଲି)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }

  ଯଦି (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      gid_map_len = (size_t) ret;
      ଯଦି (gid_map == ଖାଲି)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

ସଂରଚନା all_caps_s
{
  unsigned long effective[2];
  unsigned long permitted[2];
  unsigned long inheritable[2];
  unsigned long ambient[2];
  unsigned long bounding[2];
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
has_cap_on (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cap, long unsigned *caps)
{
  ଯଦି (cap < 32)
    ଫେରନ୍ତୁ CAP_TO_MASK_0 (cap) & caps[0];
  ଫେରନ୍ତୁ (CAP_TO_MASK_1 (cap) & caps[1]);
}

ସ୍ଥିର unsigned long cap_last_cap;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅକ୍ଷର buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  ଯଦି (fd < 0)
    ଫେରନ୍ତୁ fd;
  ret = TEMP_FAILURE_RETRY (ପଠ (fd, buffer, ଆକାର (buffer) - 1));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, ଖାଲି, 10);
  ଯଦି (errno != 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
set_required_caps (ସଂରଚନା all_caps_s *caps, uid_t uid, gid_t gid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ସଂରଚନା __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  ସଂରଚନା __user_cap_data_struct data[2] = { { 0 } };

  ଯଦି (cap_last_cap == 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  ପାଇଁ (cap = 0; cap <= cap_last_cap; cap++)
    ଯଦି (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        ଯଦି (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          ଫେରନ୍ତୁ crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  ଯଦି (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  ଯଦି (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "prctl reset ambient");

  ପାଇଁ (cap = 0; cap <= cap_last_cap; cap++)
    ଯଦି (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        ଯଦି (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          ଫେରନ୍ତୁ crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  ଯଦି (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "prctl reset KEEPCAPS");

      ଯଦି (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          ପାଇଁ (cap = 0; cap <= cap_last_cap; cap++)
            ଯଦି (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "capset");
        }
    }
#endif

  ଯଦି (no_new_privs)
    ଯଦି (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      ଫେରନ୍ତୁ crun_make_error (err, errno, "prctl set no new privs");

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_caps (unsigned long caps[2], ଅକ୍ଷର **ମୂଲ୍ୟଗୁଡ଼ିକ, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  ପାଇଁ (i = 0; i < len; i++)
    {
      cap_ମୂଲ୍ୟ_t cap;
      ଯଦି (cap_from_name (ମୂଲ୍ୟଗୁଡ଼ିକ[i], &cap) < 0)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "unknown capability `%s`", ମୂଲ୍ୟଗୁଡ଼ିକ[i]);

      ଯଦି (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      ଅନ୍ୟଥା
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#ଅନ୍ୟଥା
  caps[0] = 0;
  caps[1] = 0;
#endif
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ଯଦି (proc->selinux_label)
    ଫେରନ୍ତୁ set_selinux_label (container, proc->selinux_label, now, err);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ଯଦି (proc->apparmor_profile)
    ଫେରନ୍ତୁ set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  ସଂରଚନା all_caps_s caps = {};
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

ସଂରଚନା rlimit_s
{
  ଅଚଳ ଅକ୍ଷର *name;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;
};

ସଂରଚନା rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
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
                              { "RLIMIT_STACK", RLIMIT_ଗଦା },
                              { ଖାଲି, 0 } };

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_rlimit_resource (ଅଚଳ ଅକ୍ଷର *name)
{
  ସଂରଚନା rlimit_s *it;
  ପାଇଁ (it = rlimits; it->name; it++)
    ଯଦି (strcmp (it->name, name) == 0)
      ଫେରନ୍ତୁ it->ମୂଲ୍ୟ;
  ଫେରନ୍ତୁ -1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  ପାଇଁ (i = 0; i < len; i++)
    {
      ପାଇଁ (j = i + 1; j < len; j++)
        {
          ଯଦି (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            ଫେରନ୍ତୁ crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  ପାଇଁ (i = 0; i < len; i++)
    {
      ସଂରଚନା rlimit limit;
      ଅକ୍ଷର *type = new_rlimits[i]->type;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା resource = get_rlimit_resource (type);
      ଯଦି (UNLIKELY (resource < 0))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      ଯଦି (UNLIKELY (setrlimit (resource, &limit) < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଯଦି (def->hostname == ଖାଲି || def->hostname[0] == '\0')
    ଫେରନ୍ତୁ 0;
  ଯଦି (! has_uts)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "sethostname");
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଯଦି (is_empty_string (def->domainname))
    ଫେରନ୍ତୁ 0;
  ଯଦି (! has_uts)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "setdomainname");
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅକ୍ଷର oom_buffer[16];
  ଯଦି (def->process == ଖାଲି || ! def->process->oom_score_adj_present)
    ଫେରନ୍ତୁ 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, ଆକାର (oom_buffer), "%i", def->process->oom_score_adj);
  ଯଦି (UNLIKELY (ret >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (oom_buffer)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  ଯଦି (fd < 0)
    ଫେରନ୍ତୁ fd;
  ret = TEMP_FAILURE_RETRY (ଲିଖ (fd, oom_buffer, strlen (oom_buffer)));
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  ଫେରନ୍ତୁ 0;
}

ଅଚଳ ଅକ୍ଷର *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
validate_sysctl (ଅଚଳ ଅକ୍ଷର *original_ଚାବି, ଅଚଳ ଅକ୍ଷର *original_ମୂଲ୍ୟ, ଅଚଳ ଅକ୍ଷର *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  ଯଦି (name[0] == '/' || path_has_dot_dot_component (name))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_ଚାବି);

  ଯଦି (has_prefix (name, "fs/mqueue/"))
    {
      ଯଦି (namespaces_created & CLONE_NEWIPC)
        ଫେରନ୍ତୁ 0;

      namespace = "IPC";
      ଯାଅ fail;
    }

  ଯଦି (has_prefix (name, "kernel/"))
    {
      size_t i;

      ପାଇଁ (i = 0; sysctlRequiringIPC[i]; i++)
        ଯଦି (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            ଯଦି (namespaces_created & CLONE_NEWIPC)
              ଫେରନ୍ତୁ 0;

            namespace = "IPC";
            ଯାଅ fail;
          }

      ଯଦି (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          ଯଦି (! is_empty_string (def->domainname) && (strcmp (original_ମୂଲ୍ୟ, def->domainname) != 0))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_ଚାବି);

          ଯଦି (namespaces_created & CLONE_NEWUTS)
            ଫେରନ୍ତୁ 0;

          namespace = "UTS";
          ଯାଅ fail;
        }

      ଯଦି (strcmp (name, "kernel/hostname") == 0)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_ଚାବି);
    }
  ଯଦି (has_prefix (name, "net/"))
    {
      ଯଦି (namespaces_created & CLONE_NEWNET)
        ଫେରନ୍ତୁ 0;

      namespace = "network";
      ଯାଅ fail;
    }

  ଫେରନ୍ତୁ crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_ଚାବି);

fail:
  ଫେରନ୍ତୁ crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_ଚାବି, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
ସ୍ଥିର ଅକ୍ଷର *
sysctl_error_reason (ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା namespaces_created, ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_)
{

  ଯଦି (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    ଫେରନ୍ତୁ xstrdup ("are all the IDs mapped in the user namespace?");

  ଫେରନ୍ତୁ ଖାଲି;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ଯଦି (def->linux == ଖାଲି || def->linux->sysctl == ଖାଲି || def->linux->sysctl->len == 0)
    ଫେରନ୍ତୁ 0;

  ପାଇଁ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;

      ମୂଲ୍ୟ = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ଯଦି (UNLIKELY (ମୂଲ୍ୟ < 0))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= ମୂଲ୍ୟ;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  ଯଦି (UNLIKELY (dirfd < 0))
    ଫେରନ୍ତୁ dirfd;

  ପାଇଁ (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free ଅକ୍ଷର *name = ଖାଲି;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      ଯଦି (def->linux->sysctl->ଚାବିଗୁଡ଼ିକ[i] == ଖାଲି || def->linux->sysctl->ମୂଲ୍ୟଗୁଡ଼ିକ[i] == ଖାଲି)
        ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_ଚାବି_to_proc_path (def->linux->sysctl->ଚାବିଗୁଡ଼ିକ[i]);

      ret = validate_sysctl (def->linux->sysctl->ଚାବିଗୁଡ଼ିକ[i], def->linux->sysctl->ମୂଲ୍ୟଗୁଡ଼ିକ[i], name, namespaces_created, def, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ଯଦି (UNLIKELY (fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (ଲିଖ (fd, def->linux->sysctl->ମୂଲ୍ୟଗୁଡ଼ିକ[i], strlen (def->linux->sysctl->ମୂଲ୍ୟଗୁଡ଼ିକ[i])));
      ଯଦି (UNLIKELY (ret < 0))
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
          cleanup_free ଅକ୍ଷର *reason = ଖାଲି;

          reason = sysctl_error_reason (def->linux->sysctl->ଚାବିଗୁଡ଼ିକ[i], namespaces_created, saved_errno);
          ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_terminal (ଅକ୍ଷର **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  uid_t uid = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  ଫେରନ୍ତୁ ret;
}

ଅକ୍ଷର *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  ଫେରନ୍ତୁ get_private_data (container)->external_descriptors;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *buf = ଖାଲି;
  json_gen_ctx *gen = ଖାଲି;
  size_t buf_len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ଯଦି (! json_gen_init (&gen, ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  ଯଦି (UNLIKELY (ret != json_gen_status_ok))
    ଯାଅ json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  ପାଇଁ (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      ଅକ୍ଷର link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      ଯଦି (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          ଯଦି (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          ଅନ୍ୟଥା
            {
              json_gen_free (gen);
              ଫେରନ୍ତୁ crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      ଯଦି (UNLIKELY (ret != json_gen_status_ok))
        ଯାଅ json_error;
    }

  ret = json_gen_array_close (gen);
  ଯଦି (UNLIKELY (ret != json_gen_status_ok))
    ଯାଅ json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  ଯଦି (UNLIKELY (ret != json_gen_status_ok))
    ଯାଅ json_error;

  ଯଦି (buf)
    {
      ଅକ୍ଷର *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  ଫେରନ୍ତୁ 0;

json_error:
  ଯଦି (gen)
    json_gen_free (gen);
  ଫେରନ୍ତୁ json_gen_error_to_crun_error (ret, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ଅକ୍ଷର *pty = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (def->process == ଖାଲି || ! def->process->terminal)
    ଫେରନ୍ତୁ 0;

  fd = open_terminal (&pty, def->process, err);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ fd;

  ଯଦି (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", ଖାଲି, MS_BIND, ଖାଲି, LABEL_MOUNT, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      ret = unlink ("/dev/console");
      ଯଦି (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          ଯଦି (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", ଖାଲି, MS_BIND, ଖାଲି, LABEL_MOUNT, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;

              ଫେରନ୍ତୁ get_and_reset (&fd);
            }
          ଫେରନ୍ତୁ crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  ଫେରନ୍ତୁ get_and_reset (&fd);
}

ସ୍ଥିର bool
read_error_from_sync_socket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *error, ଅକ୍ଷର **str)
{
  cleanup_free ଅକ୍ଷର *b = ଖାଲି;
  size_t size;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା code = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (ପଠ (sync_socket_fd, &code, ଆକାର (code)));
      ଯଦି (UNLIKELY (ret != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (code)))
        ଫେରନ୍ତୁ false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (ପଠ (sync_socket_fd, &size, ଆକାର (size)));
  ଯଦି (UNLIKELY (ret != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (size)))
    ଫେରନ୍ତୁ false;

  ଯଦି (size == 0)
    ଫେରନ୍ତୁ false;

  ଯଦି (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (ପଠ (sync_socket_fd, b, size));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ false;

  b[ret] = '\0';

  *str = b;
  b = ଖାଲି;
  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
send_error_to_sync_socket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;
  ଅକ୍ଷର *ସନ୍ଦେଶ;

  ଯଦି (err == ଖାଲି || *err == ଖାଲି)
    ଫେରନ୍ତୁ false;

  code = crun_error_get_errno (err);

  ଯଦି (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket_fd, "1", 1));
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ false;
    }

  ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket_fd, &code, ଆକାର (code)));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ false;

  ସନ୍ଦେଶ = (*err)->ସନ୍ଦେଶ;
  size = strlen (ସନ୍ଦେଶ) + 1;
  ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket_fd, &size, ଆକାର (size)));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ false;

  ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket_fd, ସନ୍ଦେଶ, size));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ false;

  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
send_success_to_sync_socket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket, libcrun_error_t *err)
{
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା success = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket, &success, ଆକାର (success)));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "write to sync socket");

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର __attribute__ ((noreturn)) ଶୂନ୍ୟ
send_error_to_sync_socket_and_die (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  ଅକ୍ଷର *ସନ୍ଦେଶ;

  ଯଦି (err == ଖାଲି || *err == ଖାଲି)
    _safe_exit (EXIT_FAILURE);

  ଯଦି (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      ସନ୍ଦେଶ = (*err)->ସନ୍ଦେଶ;
      libcrun_fail_with_error (errno, "%s", ସନ୍ଦେଶ);
    }
  _safe_exit (EXIT_FAILURE);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
expect_success_from_sync_socket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_fd, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *err_str = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = TEMP_FAILURE_RETRY (ପଠ (sync_fd, &res, ଆକାର (res)));
  ଯଦି (UNLIKELY (ret != ଆକାର (res)))
    ଫେରନ୍ତୁ crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  ଯଦି (res == 0)
    ଫେରନ୍ତୁ 0;

  ଯଦି (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      ଯଦି (! is_empty_string (err_str))
        ଫେରନ୍ତୁ crun_make_error (err, res, "%s", err_str);
    }

  ଫେରନ୍ତୁ crun_make_error (err, res, "read from sync socket");
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
join_namespaces (runtime_spec_schema_config_schema *def, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *namespaces_to_join, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_namespaces_to_join,
                 ପୂର୍ଣ୍ଣସଂଖ୍ୟା *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ପାଇଁ (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free ଅକ୍ଷର *cwd = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା orig_index = namespaces_to_join_index[i];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;

      ଯଦି (namespaces_to_join[i] < 0)
        ଜାରି;

      /* Skip the user namespace.  */
      ମୂଲ୍ୟ = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      ଯଦି (ମୂଲ୍ୟ == CLONE_NEWUSER)
        ଜାରି;

      ଯଦି (ମୂଲ୍ୟ == CLONE_NEWNS)
        {
          cwd = getcwd (ଖାଲି, 0);
          ଯଦି (UNLIKELY (cwd == ଖାଲି))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], ମୂଲ୍ୟ);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (ignore_join_errors)
            ଜାରି;
          ଫେରନ୍ତୁ crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      ଯଦି (ମୂଲ୍ୟ == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  ଫେରନ୍ତୁ 0;
}

#define MAX_NAMESPACES 10

ସଂରଚନା init_status_s
{
  /* fd to the namespace to join.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ[MAX_NAMESPACES];
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
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା userns_index;
  /* def->linux->namespaces userns.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା userns_index_origin;

  /* All namespaces created/joined by the container.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_namespaces;

  /* What namespaces are still missing to be created.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx_timens_to_join_immediately;
};

ଶୂନ୍ୟ
cleanup_free_init_statusp (ସଂରଚନା init_status_s *ns)
{
  size_t i;

  ଯଦି (ns == ଖାଲି)
    ଫେରନ୍ତୁ;

  ପାଇଁ (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
configure_init_status (ସଂରଚନା init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  ପାଇଁ (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  ପାଇଁ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ଯଦି (UNLIKELY (ମୂଲ୍ୟ < 0))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ଯଦି (ns->all_namespaces & ମୂଲ୍ୟ)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= ମୂଲ୍ୟ;

      ଯଦି (def->linux->namespaces[i]->path == ଖାଲି)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= ମୂଲ୍ୟ;
        }
      ଅନ୍ୟଥା
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          ଯଦି (ns->fd_len >= MAX_NAMESPACES)
            ଫେରନ୍ତୁ crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          ଯଦି (UNLIKELY (fd < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          ଯଦି (ମୂଲ୍ୟ == CLONE_NEWUSER)
            {
              ns->userns_index = ns->fd_len;
              ns->userns_index_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->index[ns->fd_len] = i;
          ns->ମୂଲ୍ୟ[ns->fd_len] = ମୂଲ୍ୟ;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  ଯଦି (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  ଫେରନ୍ତୁ 0;
}

/* Detect if root is available in the container.  */
ସ୍ଥିର bool
ମୂଳ_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  ପାଇଁ (i = 0; i < len; i++)
    ଯଦି (mappings[i]->container_id == 0)
      ଫେରନ୍ତୁ true;

  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ସଂରଚନା libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  ସଂରଚନା libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  ଯଦି (dev_fds == ଖାଲି)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  ଫେରନ୍ତୁ dev_fds;
}

#define ଅଙ୍କ_NEEDED_DEVS (ଆକାର (needed_devs) / ଆକାର (needed_devs[0]) - 1)

ସ୍ଥିର ସଂରଚନା libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  ସଂରଚନା libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  ଯଦି (fds == ଖାଲି)
    {
      fds = make_libcrun_fd_map (ଅଙ୍କ_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  ଫେରନ୍ତୁ fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  ଯଦି (recursive)
    *recursive = false;

  ଯଦି (src_nofollow != ଖାଲି)
    *src_nofollow = false;

  ପାଇଁ (i = 0; i < mnt->options_len; i++)
    {
      ଯଦି (mnt->options[i] == ଖାଲି)
        ଜାରି;

      ଯଦି (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ଯଦି (src_nofollow == ଖାଲି || *src_nofollow)
            ଭାଙ୍ଗ;
        }
      ଯଦି (strcmp (mnt->options[i], "rbind") == 0)
        {
          ଯଦି (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ଯଦି (src_nofollow == ଖାଲି || *src_nofollow)
            ଭାଙ୍ଗ;
        }
      ଯଦି (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଅକ୍ଷର *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  ପାଇଁ (i = 0; i < mnt->options_len; i++)
    {
      ଯଦି (mnt->options[i] == ଖାଲି)
        ଜାରି;

      ଯଦି (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          ଫେରନ୍ତୁ mnt->options[i];
        }
      ଯଦି (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          ଫେରନ୍ତୁ mnt->options[i];
        }
    }
  ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_mount_type (ଅଚଳ ଅକ୍ଷର *type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out_fd, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsopen_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା newfs_fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ଯଦି (UNLIKELY (fsopen_fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, ଖାଲି, ଖାଲି, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  ଯଦି (UNLIKELY (newfs_fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  ସଂରଚନା mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ଅଚଳ ଅକ୍ଷର *idmap_option;
  bool recursive = false;
  ଅଚଳ ଅକ୍ଷର *options = ଖାଲି;
  bool has_mappings;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅକ୍ଷର *extra_ସନ୍ଦେଶ = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != ଖାଲି);
  ଯଦି (has_mappings_out)
    *has_mappings_out = has_mappings;
  ଯଦି (! has_mappings)
    ଫେରନ୍ତୁ 0;

  ଯଦି ((mnt->uid_mappings == ଖାଲି) != (mnt->gid_mappings == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  ଯଦି (idmap_option)
    {
      options = strchr (idmap_option, '=');
      ଯଦି (options)
        {
          /* Skip the '=' itself.  */
          options++;
          ଯଦି (options[0] == '\0')
            options = ଖାଲି;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  ଯଦି (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ fd;

  ଯଦି (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_ବୃକ୍ଷ (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      ଯଦି (UNLIKELY (newfs_fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  ଅନ୍ୟଥା
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (errno == EINVAL)
        {
          extra_ସନ୍ଦେଶ = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      ଫେରନ୍ତୁ crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_ସନ୍ଦେଶ);
    }

  *out_fd = get_and_reset (&newfs_fd);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  ଯଦି (! def || ! def->linux)
    ଯାଅ exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  ଯଦି (! mappings)
    ଯାଅ exit;

  ପାଇଁ (i = 0; i < len; i++)
    {
      ଯଦି (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        ଫେରନ୍ତୁ id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  ଫେରନ୍ତୁ is_uid ? get_overflow_uid () : get_overflow_gid ();
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
precreate_device (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  ଅକ୍ଷର name[64];
  mode_t type;
  dev_t dev;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = snprintf (name, ଆକାର (name), "%zu", i);
  ଯଦି (UNLIKELY (ret >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (name)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "chown `%s`", device->path);

  ଫେରନ୍ତୁ get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
send_mounts (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_host, ସଂରଚନା libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket_host, &how_many, ଆକାର (how_many)));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "write to sync socket");

  ପାଇଁ (i = 0; i < total; i++)
    {
      ଯଦି (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (ଅକ୍ଷର *) &i, ଆକାର (i), err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ସଂରଚନା libcrun_fd_map *mount_fds = ଖାଲି;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free ଅକ୍ଷର *rootfs = ଖାଲି;
  size_t how_many = 0;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (def->mounts_len == 0)
    ଫେରନ୍ତୁ 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  ଯଦି (def->ମୂଳ && def->ମୂଳ->path)
    rootfs = realpath (def->ମୂଳ->path, ଖାଲି);

  ଯଦି (! has_userns)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_in_userns = check_running_in_user_namespace (err);
      ଯଦି (UNLIKELY (is_in_userns < 0))
        ଫେରନ୍ତୁ is_in_userns;

      has_userns = is_in_userns > 0;
    }

  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      ଯଦି (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == ଖାଲି)
        {
          unsigned long propagation = 0;

          ଯଦି (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, ଖାଲି, ଖାଲି, ଖାଲି, ଖାଲି) & ALL_PROPAGATIONS_NO_REC;
          ଯଦି (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          ଯଦି (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      ଯଦି (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  ଫେରନ୍ତୁ send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
prepare_and_send_dev_mounts (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ସଂରଚନା libcrun_fd_map *dev_fds = ଖାଲି;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_mountns = -1;
  cleanup_free ଅକ୍ଷର *state_dir = ଖାଲି;
  cleanup_free ଅକ୍ଷର *devs_path = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା devs_mountfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା targetfd = -1;
  ଅଚଳ ଅକ୍ଷର *context_type = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *label = ଖାଲି;
  size_t how_many = 0;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  ଯଦି (def->linux == ଖାଲି || def->linux->devices_len == 0)
    ଫେରନ୍ତୁ 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  ଯଦି (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    ଫେରନ୍ତୁ send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_ମୂଳ : ଖାଲି),
                                     (container->context ? container->context->id : ଖାଲି), err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = mkdir (devs_path, 0700);
  ଯଦି (UNLIKELY (ret < 0 && errno != EEXIST))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  ଯଦି (UNLIKELY (current_mountns < 0))
    ଫେରନ୍ତୁ current_mountns;

  ret = unshare (CLONE_NEWNS);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (ଖାଲି, "/", ଖାଲି, MS_REC | MS_PRIVATE, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      ଯାଅ restore_mountns;
    }

  ଯଦି (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      ଯଦି (UNLIKELY (context_type == ଖାଲି))
        {
          ret = -1;
          ଯାଅ restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", ଖାଲି, context_type, label, ଖାଲି);
  ଯଦି (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      ଯାଅ restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      ଯାଅ restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      ଯାଅ restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      ଯାଅ restore_mountns;
    }

  ପାଇଁ (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ଜାରି;
        }

      dev_fds->fds[i] = ret;

      ଯଦି (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  ଯଦି (ns_changed && current_mountns >= 0)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      ଯଦି (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          ଫେରନ୍ତୁ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map ସଂରଚନା libcrun_fd_map *fds = ଖାଲି;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (ଅଙ୍କ_NEEDED_DEVS);

  ଯଦି (! has_userns || geteuid () > 0)
    ଫେରନ୍ତୁ send_mounts (sync_socket_host, fds, 0, ଅଙ୍କ_NEEDED_DEVS, err);

  ପାଇଁ (i = 0; i < ଅଙ୍କ_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = syscall_open_ବୃକ୍ଷ (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      ଯଦି (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  ଫେରନ୍ତୁ send_mounts (sync_socket_host, fds, how_many, ଅଙ୍କ_NEEDED_DEVS, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_host, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
receive_mounts (ସଂରଚନା libcrun_fd_map *fds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (fds->nfds == 0)
    ଫେରନ୍ତୁ 0;

  ret = TEMP_FAILURE_RETRY (ପଠ (sync_socket_container, &how_many, ଆକାର (how_many)));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "read from sync socket");

  ପାଇଁ (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (ଅକ୍ଷର *) &index, ଆକାର (index), err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      ଯଦି (index >= fds->nfds)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid mount data received");

      ଯଦି (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (! container->host_has_cap_sys_ptrace)
    ଫେରନ୍ତୁ 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool ମୂଳ_mapped = false;

      ଯଦି (def->linux->uid_mappings_len != 0)
        {
          ମୂଳ_mapped = ମୂଳ_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          ଯଦି (! ମୂଳ_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      ଯଦି (def->linux->gid_mappings_len != 0)
        {
          ମୂଳ_mapped = ମୂଳ_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          ଯଦି (! ମୂଳ_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  ଯଦି (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "setresgid to `%d`", gid);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  ଫେରନ୍ତୁ mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, ଖାଲି, ଖାଲି);
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ସଂରଚନା libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା socket_parent = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା socket_child = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଳ_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା cwd_fd = -1;
  size_t i, missing = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockets[2];
  pid_t pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (def->ମୂଳ == ଖାଲି || def->ମୂଳ->path == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    ଯଦି (is_proc_mount (def->mounts[i]))
      missing++;
  ଯଦି (missing == 0)
    ଫେରନ୍ତୁ 1;

  ମୂଳ_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (ମୂଳ_fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (cwd_fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->ମୂଳ->path);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ 0;

  pid = fork ();
  saved_errno = errno;
  ଯଦି (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      ଯଦି (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (ଶୂନ୍ୟ) unshare (CLONE_NEWNS);

      ପାଇଁ (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

          ଯଦି (! is_proc_mount (def->mounts[i]))
            ଜାରି;

          fd = open_mount_of_type (container, def->mounts[i], err);
          ଯଦି (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (ଅକ୍ଷର *) &i, ଆକାର (i), err);
          ଯଦି (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ଯଦି (UNLIKELY (fchdir (ମୂଳ_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "restore the root directory");

  ଯଦି (UNLIKELY (pid < 0))
    ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  ଯେତେବେଳେ (missing > 0)
    {
      libcrun_error_t tmp_err = ଖାଲି;

      ret = receive_fd_from_socket_with_payload (socket_parent, (ଅକ୍ଷର *) &i, ଆକାର (i), &tmp_err);
      ଯଦି (ret < 0)
        {
          crun_error_release (&tmp_err);
          ଭାଙ୍ଗ;
        }
      ଯଦି (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          ଭାଙ୍ଗ;
        }

      ଯଦି (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, ଖାଲି, 0);

  ଫେରନ୍ତୁ missing == 0 ? 1 : 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
init_container (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_container, ସଂରଚନା init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ସଂରଚନା libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      ଯଦି (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      ଯଦି (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      ଯଦି (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          delayed_pidns_fork = ret > 0;
        }

      ଯଦି (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket_container, &new_pid, ଆକାର (new_pid)));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "write to sync socket");
        }
      ଅନ୍ୟଥା
        {
          new_pid = fork ();
          ଯଦି (UNLIKELY (new_pid < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "fork");
        }

      ଯଦି (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          ଯଦି (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket_container, &new_pid, ଆକାର (new_pid)));
          ଯଦି (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = libcrun_set_oom (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  ଯଦି ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      ପାଇଁ (i = 0; i < def->mounts_len; i++)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          ଯଦି (def->mounts[i]->type == ଖାଲି)
            ଜାରି;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          ଯଦି (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, ଖାଲି, ଖାଲି, ଖାଲି);
          ଯଦି (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, ଖାଲି, ଖାଲି, ଖାଲି);

          ଯଦି (fd >= 0)
            {
              ଯଦି (mount_fds->fds[i] >= 0)
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
  ଯଦି (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      ଯଦି (UNLIKELY (timens_offsets_fd < 0))
        ଫେରନ୍ତୁ timens_offsets_fd;
    }

  ଯଦି (init_status->all_namespaces & CLONE_NEWUSER)
    {
      ଯଦି (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ଯଦି (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
      ଅନ୍ୟଥା
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "unshare");
    }

  ଯଦି (def->linux->time_offsets)
    {
      ଅକ୍ଷର fmt_buffer[128];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = timens_offsets_fd;

      ଯଦି (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, ଆକାର (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ଯଦି (UNLIKELY (ret >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buffer)))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = ଲିଖ (fd, fmt_buffer, strlen (fmt_buffer));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      ଯଦି (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, ଆକାର (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          ଯଦି (UNLIKELY (ret >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buffer)))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = ଲିଖ (fd, fmt_buffer, strlen (fmt_buffer));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  ଯଦି (init_status->must_fork)
    {
      ଯଦି (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          delayed_pidns_fork = ret > 0;
        }

      ଯଦି (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          ଯଦି (UNLIKELY (pid_container < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      ଯଦି (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket_container, &pid_container, ଆକାର (pid_container)));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "write to sync socket");

          ଯଦି (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  ଫେରନ୍ତୁ get_private_data (container)->delayed_pidns_fork;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା client_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pidfd = -1;
  ଅଚଳ ଅକ୍ଷର *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  ଯଦି (v == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  pidfd = syscall_pidfd_open (pid, 0);
  ଯଦି (UNLIKELY (pidfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  ଯଦି (UNLIKELY (client_fd < 0))
    ଫେରନ୍ତୁ client_fd;

  ଫେରନ୍ତୁ send_fd_to_socket (client_fd, pidfd, err);
}

ସ୍ଥିର bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  ଯଦି (process == ଖାଲି || process->exec_cpu_affinity == ଖାଲି)
    ଫେରନ୍ତୁ false;
  ଫେରନ୍ତୁ (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, ଶୂନ୍ୟ *args,
                             ପୂର୍ଣ୍ଣସଂଖ୍ୟା *sync_socket_out, ସଂରଚନା libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) ସଂରଚନା init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_container = -1;
  ଅକ୍ଷର *notify_socket_env = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା restore_pidns = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_clone_args = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  ଯଦି (def->ମୂଳ)
    {
      ret = do_notify_socket (container, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  ଯଦି (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = libcrun_set_oom (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->ମୂଳ ? container->container_def->ମୂଳ->path : ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  ପାଇଁ (i = 0; i < init_status.fd_len; i++)
    {
      ବାଛ (init_status.ମୂଲ୍ୟ[i])
        {
        ସ୍ଥିତି CLONE_NEWIPC:
          ଯଦି (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          ଯଦି (! new_userns)
            init_status.must_fork = true;
          ଅନ୍ୟଥା
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି CLONE_NEWTIME:
          ଯଦି (! new_userns)
            init_status.must_fork = true;
          ଅନ୍ୟଥା
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          ଭାଙ୍ଗ;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  ଯଦି ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  ଅନ୍ୟଥା ଯଦି ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  ଯଦି (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  ଯଦି (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      ସଂରଚନା _clone3_args clone3_args;
      memset (&clone3_args, 0, ଆକାର (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      ଯଦି (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  ଯଦି (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, ଖାଲି);
      ଯଦି (UNLIKELY (pid < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "clone");
    }

  ଯଦି (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = close_and_reset (&sync_socket_container);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_ବୃକ୍ଷ_fd));

      ଯଦି (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          ret = TEMP_FAILURE_RETRY (ପଠ (sync_socket_host, &new_pid, ଆକାର (new_pid)));
          ଯଦି (UNLIKELY (ret != ଆକାର (new_pid)))
            ଫେରନ୍ତୁ crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          ଯଦି (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          ଅନ୍ୟଥା
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, ଖାଲି, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ଯଦି (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ଯଦି ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ଯଦି (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          ret = TEMP_FAILURE_RETRY (ପଠ (sync_socket_host, &grandchild, ଆକାର (grandchild)));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          ଯଦି (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          ଅନ୍ୟଥା
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, ଖାଲି, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      ଯଦି (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      pid_to_clean = 0;
      ଫେରନ୍ତୁ pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  ଯଦି (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  ଯଦି (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  ଅନ୍ୟଥା
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      ଯଦି (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->ସନ୍ଦେଶ);
    }

  /* Jump into the specified entrypoint.  */
  ଯଦି (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  ଯଦି (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->ସନ୍ଦେଶ);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  ଯଦି (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, ଅଚଳ ଅକ୍ଷର *sub_cgroup,
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା *terminal_fd, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅକ୍ଷର res;
  pid_t pid;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_fd = sync_socket_fd;

  ଯଦି (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (ପଠ (sync_fd, &res, 1));
  ଯଦି (UNLIKELY (ret != 1))
    ଫେରନ୍ତୁ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  ଯଦି (res != '0')
    ଫେରନ୍ତୁ crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (ପଠ (sync_fd, &pid, ଆକାର (pid)));
  ଯଦି (UNLIKELY (ret != ଆକାର (pid)))
    ଫେରନ୍ତୁ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, ଖାଲି, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "waitpid for exec child pid");

  ଯଦି (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (need_move_to_cgroup)
    {
      cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free ଅକ୍ଷର *final_cgroup = ଖାଲି;

      ଯଦି (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, ଖାଲି);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_set_io_priority (pid, process, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_fd, &ret, ଆକାର (ret)));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "write to sync socket");

  ଯଦି (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା err_code = 0;
          cleanup_free ଅକ୍ଷର *err_str = ଖାଲି;

          ଯଦି (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              ଯଦି (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  ଫେରନ୍ତୁ crun_make_error (err, err_code, "%s", err_str);
                }
            }

          ଫେରନ୍ତୁ crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  ଫେରନ୍ତୁ pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pidfd_pid_to_join = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_flags = 0;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  ଯଦି (def->linux && def->linux->namespaces)
    {
      ପାଇଁ (i = 0; i < def->linux->namespaces_len; i++)
        ଯଦି (! is_empty_string (def->linux->namespaces[i]->path))
          ଫେରନ୍ତୁ 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  ଯଦି (UNLIKELY (pidfd_pid_to_join < 0))
    ଫେରନ୍ତୁ 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  ଯଦି (ret == 0)
    ଫେରନ୍ତୁ crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  ପାଇଁ (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].ମୂଲ୍ୟ;

  ret = setns (pidfd_pid_to_join, all_flags);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fds_joined[MAX_NAMESPACES] = {
    0,
  };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  /* Nothing left to do if the namespaces were joined.  */
  ଯଦି (LIKELY (ret > 0))
    ଫେରନ୍ତୁ 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  ଯଦି (def->linux->namespaces_len >= MAX_NAMESPACES)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid configuration");

  ପାଇଁ (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free ଅକ୍ଷର *ns_path = ଖାଲି;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      ଯଦି (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          ଯଦି (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ଜାରି;
            }

          ret = fds[i];
          ଯାଅ exit;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ exit;
  ଯଦି (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      ଯାଅ exit;
    }

  ପାଇଁ (i = 0; namespaces[i].ns_file; i++)
    {
      ଯଦି (namespaces[i].ମୂଲ୍ୟ == CLONE_NEWUSER)
        ଜାରି;

      ret = setns (fds[i], 0);
      ଯଦି (ret == 0)
        fds_joined[i] = 1;
    }
  ପାଇଁ (i = 0; namespaces[i].ns_file; i++)
    {
      ଯଦି (fds_joined[i])
        ଜାରି;
      ret = setns (fds[i], 0);
      ଯଦି (ret == 0)
        fds_joined[i] = 1;
    }
  ପାଇଁ (i = 0; namespaces[i].ns_file; i++)
    {
      ଯଦି (fds_joined[i])
        ଜାରି;
      ret = setns (fds[i], 0);
      ଯଦି (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          ପାଇଁ (j = 0; j < def->linux->namespaces_len; j++)
            {
              ଯଦି (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  ଭାଙ୍ଗ;
                }
            }
          ଯଦି (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              ଜାରି;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          ଯାଅ exit;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

exit:
  ପାଇଁ (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      ଅଚଳ ଅକ୍ଷର *sub_cgroup,
                      ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach,
                      runtime_spec_schema_config_schema_process *process,
                      ପୂର୍ଣ୍ଣସଂଖ୍ୟା *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_fd[2];
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_dirfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_fd = -1;
  ସଂରଚନା _clone3_args clone3_args;
  bool need_move_to_cgroup;

  ଯଦି (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "set child subreaper");
    }

  ଯଦି (path_has_dot_dot_component (sub_cgroup))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  ଯଦି (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free ଅକ୍ଷର *final_cgroup = ଖାଲି;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *cgroup_status = ଖାଲି;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    ଯଦି (get_force_cgroup_v1_annotation (container) == ଖାଲି)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        ଯଦି (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, ଆକାର (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  ଯଦି (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  ଅନ୍ୟଥା
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  ଯଦି (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  /* On errors, fall back to fork().  */
  ଯଦି (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      ଯଦି (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          ଯାଅ exit;
        }
    }

  ଯଦି (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      ଫେରନ୍ତୁ join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (ଲିଖ (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->ସନ୍ଦେଶ);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ଯଦି (UNLIKELY (pid < 0))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
      TEMP_FAILURE_RETRY (ଲିଖ (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  ଯଦି (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_fd, "0", 1));
      ଯଦି (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_fd, &pid, ଆକାର (pid)));
      ଯଦି (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  ଅନ୍ୟଥା
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free ଅକ୍ଷର *pty = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (ପଠ (sync_fd, &r, ଆକାର (r)));
      ଯଦି (UNLIKELY (ret != ଆକାର (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      ଯଦି (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      ଯଦି (terminal_fd)
        {
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା ptmx_fd = -1;

          ret = set_id_init (container, err);
          ଯଦି (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          ଯଦି (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          ଯଦି (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  ଫେରନ୍ତୁ 0;

exit:
  ଯଦି (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  ଯଦି (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_linux_container_update (libcrun_container_status_t *status, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *cgroup_status = ଖାଲି;

  cgroup_status = libcrun_cgroup_make_status (status);

  ଫେରନ୍ତୁ libcrun_update_cgroup_resources (cgroup_status, state_ମୂଳ, resources, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, ଅଚଳ bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *cgroup_status = ଖାଲି;

  cgroup_status = libcrun_cgroup_make_status (status);

  ଫେରନ୍ତୁ libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ libcrun_container_pause_unpause_linux (status, true, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ libcrun_container_pause_unpause_linux (status, false, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  ଅନ୍ୟଥା ଯଦି (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "personality to `%s`", p->domain);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sockfd = -1;

  ପାଇଁ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ଯଦି (UNLIKELY (ମୂଲ୍ୟ < 0))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ଯଦି (ମୂଲ୍ୟ == CLONE_NEWNET && def->linux->namespaces[i]->path == ଖାଲି)
        {
          configure_network = true;
          ଭାଙ୍ଗ;
        }
    }

  ଯଦି (! configure_network)
    ଫେରନ୍ତୁ 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  ଯଦି (LIKELY (sockfd >= 0))
    {
      ସଂରଚନା ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  ଅନ୍ୟଥା
    {
      ସଂରଚନା nlmsghdr *hdr_recv;
      ଅକ୍ଷର buf[ଆକାର (ସଂରଚନା nlmsghdr) + ଆକାର (ସଂରଚନା nlmsgerr)];
      ସଂରଚନା sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      ସଂରଚନା sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      ସଂରଚନା nlmsghdr hdr = {
        .nlmsg_len = ଆକାର (ସଂରଚନା nlmsghdr) + ଆକାର (ସଂରଚନା ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      ସଂରଚନା ifinfomsg ସନ୍ଦେଶ = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((ସଂରଚନା nlmsghdr *) buf) = hdr;
      *((ସଂରଚନା ifinfomsg *) (buf + ଆକାର (ସଂରଚନା nlmsghdr))) = ସନ୍ଦେଶ;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      ଯଦି (UNLIKELY (sockfd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (ସଂରଚନା sockaddr *) &addr, ଆକାର (addr));
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, ଆକାର (buf), 0, (ସଂରଚନା sockaddr *) &addr_to, ଆକାର (addr_to));
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, ଆକାର (buf), 0, ଖାଲି, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (ସଂରଚନା nlmsghdr *) buf;

      ଯଦି (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          ସଂରଚନା nlmsgerr *err_data = (ସଂରଚନା nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          ଯଦି (err_data->error < 0)
            ଫେରନ୍ତୁ crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  ଫେରନ୍ତୁ 0;
}

/* Protection for attacks like CVE-2019-5736.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ensure_cloned_ଦ୍ୱିମିକ ();
__attribute__ ((constructor)) ସ୍ଥିର ଶୂନ୍ୟ
libcrun_rexec (ଶୂନ୍ୟ)
{
  ଯଦି (ensure_cloned_ଦ୍ୱିମିକ () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଙ୍କେତ, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  ଯଦି (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      /* The pid is not valid anymore, return an error.  */
      ଯଦି (ret == 0)
        {
          errno = ESRCH;
          ଫେରନ୍ତୁ crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, ସଙ୍କେତ);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "kill container");
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_kill_linux (libcrun_container_status_t *status, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଙ୍କେତ, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  ଯଦି (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      ଯଦି (errno == ENOSYS)
        ଫେରନ୍ତୁ libcrun_kill_linux_no_pidfd (status, true, ସଙ୍କେତ, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      ଯଦି (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        ଫେରନ୍ତୁ crun_make_error (err, ESRCH, "process not running");
      ଫେରନ୍ତୁ crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* The pid is not valid anymore, return an error.  */
  ଯଦି (ret == 0)
    {
      errno = ESRCH;
      ଫେରନ୍ତୁ crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, ସଙ୍କେତ, ଖାଲି, 0);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      ଯଦି (errno == ENOSYS)
        ଫେରନ୍ତୁ libcrun_kill_linux_no_pidfd (status, false, ସଙ୍କେତ, err);
      ଫେରନ୍ତୁ crun_make_error (err, errno, "send signal to pidfd");
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
libcrun_get_intelrdt_name (ଅଚଳ ଅକ୍ଷର *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  ଯଦି (def == ଖାଲି || def->linux == ଖାଲି || def->linux->intel_rdt == ଖାଲି || def->linux->intel_rdt->clos_id == ଖାଲି)
    {
      ଯଦି (explicit)
        *explicit = false;
      ଫେରନ୍ତୁ ctr_name;
    }

  ଯଦି (explicit)
    *explicit = true;
  ଫେରନ୍ତୁ def->linux->intel_rdt->clos_id;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_apply_intelrdt (ଅଚଳ ଅକ୍ଷର *ctr_name, libcrun_container_t *container, pid_t pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ଖାଲି;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  ଅଚଳ ଅକ୍ଷର *name;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (container)
    def = container->container_def;

  ଯଦି (def == ଖାଲି || def->linux == ଖାଲି || def->linux->intel_rdt == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  ଯଦି (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଯଦି (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଯାଅ fail;

          has_monitoring = true;
        }
    }

  ଯଦି (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ fail;
    }

  ଯଦି (actions & LIBCRUN_INTELRDT_MOVE)
    {
      ଅଚଳ ଅକ୍ଷର *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : ଖାଲି;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ fail;
    }

  ଫେରନ୍ତୁ 0;

fail:
  ଯଦି (has_monitoring)
    {
      libcrun_error_t tmp_err = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      ଯଦି (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  ଯଦି (created)
    {
      libcrun_error_t tmp_err = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      ଯଦି (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_destroy_intelrdt (ଅଚଳ ଅକ୍ଷର *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  ଅଚଳ ଅକ୍ଷର *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  ଯଦି (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      ଯଦି (ret < 0)
        ଫେରନ୍ତୁ ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  ଯଦି (explicit)
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ resctl_destroy (clos_id, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_update_intel_rdt (ଅଚଳ ଅକ୍ଷର *ctr_name, libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *l3_cache_schema, ଅଚଳ ଅକ୍ଷର *mem_bw_schema, ଅକ୍ଷର *ଅଚଳ *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *name;

  ଯଦି (container)
    def = container->container_def;

  ଯଦି (def == ଖାଲି || def->linux == ଖାଲି || def->linux->intel_rdt == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, ଖାଲି);

  ଫେରନ୍ତୁ resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_safe_chdir (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = chdir (path);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  ଯଦି (ret == 0 || buffer[0] != '/')
    {
      ଯଦି (chdir ("/") < 0)
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
#define UNREACHABLE_LEN ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (UNREACHABLE) - 1)

      ଯଦି ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      ଫେରନ୍ତୁ crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  ଫେରନ୍ତୁ 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
run_callback_in_namespace (ପୂର୍ଣ୍ଣସଂଖ୍ୟା mntns_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*callback) (ଶୂନ୍ୟ *, libcrun_error_t *), ଶୂନ୍ୟ *arg, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା wait_status = 0;
  pid_t pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ଯଦି (UNLIKELY (pid < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "vfork");

  ଯଦି (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      ଯଦି (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      ଯଦି (UNLIKELY (ret < 0))
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
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  ଫେରନ୍ତୁ get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
check_container_userns (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent_fd = -1;
  ସଂରଚନା stat st, expected_st;
  ଅଚଳ ଅକ୍ଷର *path = ଖାଲି;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (def->linux)
    ପାଇଁ (i = 0; i < def->linux->namespaces_len; i++)
      ଯଦି (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == ଖାଲି;
          ଭାଙ୍ଗ;
        }

  ଯଦି (path == ଖାଲି)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      ଯଦି (UNLIKELY (expected_fd < 0))
        ଫେରନ୍ତୁ expected_fd;
    }
  ଅନ୍ୟଥା
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      ଯଦି (UNLIKELY (expected_fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  ଯଦି (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      ଯଦି (UNLIKELY (parent_fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat user namespace");

  ଯଦି (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  ଫେରନ୍ତୁ 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mntns_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *owner_fd, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା mntns = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା owner = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  ଯଦି (UNLIKELY (mntns < 0))
    ଫେରନ୍ତୁ mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (ret == 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  ଯଦି (UNLIKELY (owner < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*callback) (ଶୂନ୍ୟ *, libcrun_error_t *), ଶୂନ୍ୟ *arg, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା mntns_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା owner_fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ run_callback_in_namespace (mntns_fd, callback, arg, err);
}

ସଂରଚନା umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
umount_nofollow (ଅଚଳ ଅକ୍ଷର *destination, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା parentfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା cwdfd = -1;
  ଅକ୍ଷର buffer[PATH_MAX];
  ଅଚଳ ଅକ୍ଷର *parent, *name;
  ଅକ୍ଷର *last_slash;
  size_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (is_empty_string (destination))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  ଯଦି (UNLIKELY (len >= ଆକାର (buffer)))
    ଫେରନ୍ତୁ crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  ଯେତେବେଳେ (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  ଯଦି (last_slash == ଖାଲି)
    {
      parent = ".";
      name = buffer;
    }
  ଅନ୍ୟଥା ଯଦି (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  ଅନ୍ୟଥା
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  ଯଦି (UNLIKELY (parentfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  ଯଦି (UNLIKELY (cwdfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  ଯଦି (UNLIKELY (fchdir (cwdfd) < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fchdir to the previous working directory");

  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "umount `%s`", destination);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_umount_in_a_container (ଶୂନ୍ୟ *arg, libcrun_error_t *err)
{
  ସଂରଚନା umount_in_a_container_args *args = arg;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ପାଇଁ (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ସଂରଚନା mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  ସଂରଚନା libcrun_fd_map *fds;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା pidfd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_destination_nofollow (ଅଚଳ ଅକ୍ଷର *destination, bool dir, libcrun_error_t *err)
{
  ଅଚଳ uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  ଯଦି (LIKELY (fd >= 0))
    ଫେରନ୍ତୁ fd;

  ଯଦି (errno != ENOENT)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", destination);

  ଯଦି (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  ଅନ୍ୟଥା
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", destination);

  ଫେରନ୍ତୁ fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
create_destinations_in_a_container (ଶୂନ୍ୟ *arg, libcrun_error_t *err)
{
  ସଂରଚନା mount_in_a_container_args *args = arg;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  ଯଦି (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "setresuid to `0`");
    }
  ଅନ୍ୟଥା ଯଦି (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ଫେରନ୍ତୁ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  ପାଇଁ (i = 0; i < args->len; i++)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_fd = -1;
      ସଂରଚନା stat st;

      ret = fstat (args->fds->fds[i], &st);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ଯଦି (UNLIKELY (dest_fd < 0))
        ଫେରନ୍ତୁ dest_fd;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_mount_in_a_container (ଶୂନ୍ୟ *arg, libcrun_error_t *err)
{
  ସଂରଚନା mount_in_a_container_args *args = arg;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ପାଇଁ (i = 0; i < args->len; i++)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_fd = -1;
      ସଂରଚନା stat st;

      ret = fstat (args->fds->fds[i], &st);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ଯଦି (UNLIKELY (dest_fd < 0))
        ଫେରନ୍ତୁ dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  ଫେରନ୍ତୁ 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
send_mounts_from_container_namespaces (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pidfd, runtime_spec_schema_defs_mount **mounts,
                                       ସଂରଚନା libcrun_fd_map *fds, size_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା owner_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  ଯଦି (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  ଅନ୍ୟଥା ଯଦି (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ଫେରନ୍ତୁ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ଯଦି (UNLIKELY (pid < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fork");

  ଯଦି (pid)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      ଯଦି (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, ଖାଲି, 0));
          ଫେରନ୍ତୁ crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ପାଇଁ (i = 0; i < len; i++)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

      ଯଦି (fds->fds[i] >= 0)
        ଜାରି;

      fd = open_mount_of_type (container, mounts[i], err);
      ଯଦି (UNLIKELY (fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *flags, libcrun_error_t *err)
{
  ସ୍ଥିର ଅଚଳ ସଂରଚନା
  {
    ଅଚଳ ଅକ୍ଷର *name;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  ପାଇଁ (i = 0; i < ଆକାର (namespaces) / ଆକାର (namespaces[0]); i++)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା self_fd = -1;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା target_fd = -1;
      ସଂରଚନା stat self_st, target_st;
      cleanup_free ଅକ୍ଷର *self_path = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      ଯଦି (UNLIKELY (self_fd < 0))
        ଫେରନ୍ତୁ self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      ଯଦି (UNLIKELY (target_fd < 0))
        ଫେରନ୍ତୁ target_fd;

      ret = fstat (self_fd, &self_st);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ଯଦି (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  ଫେରନ୍ତୁ 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, ପୂର୍ଣ୍ଣସଂଖ୍ୟା owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, ସଂରଚନା libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pidfd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_fd[2];
  bool needed = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
  pid_t pid;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ପାଇଁ (i = 0; i < len; i++)
    {
      ଯଦି (fds->fds[i] >= 0)
        ଜାରି;

      ଯଦି (mounts[i]->type == ଖାଲି)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  ଯଦି (! needed)
    ଫେରନ୍ତୁ 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  ଯଦି (UNLIKELY (pidfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (ret == 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  ଯଦି (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      ଫେରନ୍ତୁ ret;
    }

  ଯଦି (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      ଯଦି (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  ପାଇଁ (i = 0; i < len; i++)
    {
      ଯଦି (fds->fds[i] >= 0)
        ଜାରି;

      ret = receive_fd_from_socket (sync_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          cleanup_free ଅକ୍ଷର *err_str = ଖାଲି;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା err_code = 0;

          ଯଦି (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          ଅନ୍ୟଥା
            ret = crun_error_wrap (err, "receive mount fd");
          ଭାଙ୍ଗ;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, ଖାଲି, 0));

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free ଅକ୍ଷର *data = ଖାଲି;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  ଯଦି (mount->options == ଖାଲି)
    ଫେରନ୍ତୁ get_default_flags (container, mount->destination, &data);

  ପାଇଁ (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  ଫେରନ୍ତୁ flags;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ସଂରଚନା mount_in_a_container_args args;
  cleanup_close_map ସଂରଚନା libcrun_fd_map *fds = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pidfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା mntns_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  fds = make_libcrun_fd_map (len);

  ପାଇଁ (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), ଖାଲି, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଯଦି (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          ଯଦି (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          ଯଦି (UNLIKELY (fds->fds[i] < 0))
            ଫେରନ୍ତୁ fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ପାଇଁ (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  ଯଦି (UNLIKELY (ret > 0))
    ଫେରନ୍ତୁ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  ସଂରଚନା umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  ଫେରନ୍ତୁ run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

ସଂରଚନା restrict_mounts_in_a_container_args
{
  ସଂରଚନା libcrun_mount_flags_s *mounts;
  size_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootfd;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_restrict_mounts_in_a_container (ଶୂନ୍ୟ *arg, libcrun_error_t *err)
{
  ସଂରଚନା restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  ଯଦି (LIKELY (ret == 0))
    ret = chroot (".");
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "chroot to the container root");

  ପାଇଁ (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      ଅଚଳ ଅକ୍ଷର *target = consume_slashes (args->mounts[i].target);
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
      ସଂରଚନା statfs sfs;
      unsigned long current_flags;

      ଯଦି (flags == 0)
        ଜାରି;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      ଯଦି (UNLIKELY (fd < 0))
        {
          ଯଦି (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ଜାରି;
            }
          ଫେରନ୍ତୁ fd;
        }

      ret = fstatfs (fd, &sfs);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      ଯଦି ((current_flags & flags) == flags)
        ଜାରି;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      ଯଦି (LIKELY (ret == 0))
        ଜାରି;

      ଯଦି (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        ଫେରନ୍ତୁ ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (ଖାଲି, target, ଖାଲି, MS_REMOUNT | MS_BIND | flags | current_flags, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "remount `/%s`", target);
    }

  ଫେରନ୍ତୁ 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_restrict_mounts_in_container (pid_t pid, ସଂରଚନା libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  ସଂରଚନା restrict_mounts_in_a_container_args args;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା mntns_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootfd = -1;
  ଅକ୍ଷର path[64];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  snprintf (path, ଆକାର (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  ଯଦି (UNLIKELY (mntns_fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, ଆକାର (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  ଯଦି (UNLIKELY (rootfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  ଯଦି (UNLIKELY (ret > 0))
    ଫେରନ୍ତୁ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା netns_fd = -1;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (def == ଖାଲି || def->linux == ଖାଲି || def->linux->net_devices == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  cleanup_free ଅକ୍ଷର *ns_path = ଖାଲି;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  ଯଦି (UNLIKELY (netns_fd < 0))
    ଫେରନ୍ତୁ netns_fd;

  ପାଇଁ (i = 0; i < def->linux->net_devices->len; i++)
    {
      ଅଚଳ ଅକ୍ଷର *new_name = def->linux->net_devices->ମୂଲ୍ୟଗୁଡ଼ିକ[i]->name ?: def->linux->net_devices->ଚାବିଗୁଡ଼ିକ[i];

      ret = move_network_device (def->linux->net_devices->ଚାବିଗୁଡ଼ିକ[i], new_name, netns_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}
