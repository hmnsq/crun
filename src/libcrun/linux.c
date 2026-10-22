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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "linux.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "status.h"
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sched.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/mount.h>
#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#എങ്കിൽ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#endif

#അന്തര്ഭാവ <sys/prctl.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#അന്തര്ഭാവ <sys/time.h>
#അന്തര്ഭാവ <sys/resource.h>
#അന്തര്ഭാവ <sys/sysmacros.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <grp.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ "terminal.h"
#അന്തര്ഭാവ "cgroup.h"
#അന്തര്ഭാവ "cgroup-utils.h"
#അന്തര്ഭാവ "status.h"
#അന്തര്ഭാവ "criu.h"
#അന്തര്ഭാവ "scheduler.h"
#അന്തര്ഭാവ "intelrdt.h"
#അന്തര്ഭാവ "io_priority.h"
#അന്തര്ഭാവ "net_device.h"

#അന്തര്ഭാവ <sys/socket.h>
#അന്തര്ഭാവ <libgen.h>
#അന്തര്ഭാവ <sys/wait.h>
#അന്തര്ഭാവ <sys/vfs.h>
#അന്തര്ഭാവ <sys/statvfs.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ <sys/personality.h>
#അന്തര്ഭാവ <net/if.h>
#അന്തര്ഭാവ <sys/xattr.h>
#അന്തര്ഭാവ <linux/netlink.h>
#അന്തര്ഭാവ <linux/rtnetlink.h>
#അന്തര്ഭാവ <sched.h>
#അന്തര്ഭാവ <linux/sched.h>
#അന്തര്ഭാവ <linux/magic.h>
#അന്തര്ഭാവ <linux/nsfs.h>

#അന്തര്ഭാവ <json-c/json.h>
#അന്തര്ഭാവ <ocispec/json_common.h>

#അന്തര്ഭാവ "mount_flags.h"
#അന്തര്ഭാവ "syscalls.h"

#ifndef RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#endif

#ifndef OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#endif

#ifndef OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#endif

#ifndef OPEN_വൃക്ഷം_NAMESPACE
#  define OPEN_വൃക്ഷം_NAMESPACE 2
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
സ്ഥിതം unsigned long
mount_flags_from_statfs (unsigned long f_flags)
{
  unsigned long flags = 0;

  എങ്കിൽ (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  എങ്കിൽ (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  എങ്കിൽ (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  എങ്കിൽ (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  എങ്കിൽ (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  എങ്കിൽ (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  എങ്കിൽ (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  എങ്കിൽ (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  എങ്കിൽ (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  തിരികെ flags;
}

ഘടന remount_s
{
  ഘടന remount_s *next;
  അക്ഷരം *target;
  പൂർണ്ണസംഖ്യ targetfd;
  unsigned long flags;
  അക്ഷരം *data;
};

ഘടന private_data_s
{
  ഘടന remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  പൂർണ്ണസംഖ്യ unshare_flags;
  പൂർണ്ണസംഖ്യ unshare_cgroupns;

  അക്ഷരം *host_notify_socket_path;
  അക്ഷരം *container_notify_socket_path;
  അക്ഷരം *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  പൂർണ്ണസംഖ്യ procfd;

  സ്ഥിരം അക്ഷരം *rootfs;
  പൂർണ്ണസംഖ്യ rootfsfd;

  പൂർണ്ണസംഖ്യ notify_socket_വൃക്ഷം_fd;
  പൂർണ്ണസംഖ്യ old_വേര്_fd;

  ഘടന libcrun_fd_map *mount_fds;
  ഘടന libcrun_fd_map *dev_fds;
  ഘടന libcrun_fd_map *needed_devs_fds;

  അക്ഷരം **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  അക്ഷരം *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  പൂർണ്ണസംഖ്യ maskdir_fd;
  അക്ഷരം *maskdir_proc_path;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_pivot;
  bool no_pivot;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_വേര്_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  അക്ഷരം *host_rootfs;
};

ഘടന linux_namespace_s
{
  സ്ഥിരം അക്ഷരം *name;
  സ്ഥിരം അക്ഷരം *ns_file;
  പൂർണ്ണസംഖ്യ മൂല്യം;
};

സ്ഥിതം ശൂന്യം
cleanup_private_data (ശൂന്യം *private_data)
{
  ഘടന private_data_s *p = private_data;

  എങ്കിൽ (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  എങ്കിൽ (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  എങ്കിൽ (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  എങ്കിൽ (p->old_വേര്_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_വേര്_fd));
  എങ്കിൽ (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  എങ്കിൽ (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  എങ്കിൽ (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  എങ്കിൽ (p->copy_symlink_targets)
    {
      size_t i;
      വേണ്ടി (i = 0; i < p->n_copy_symlink_targets; i++)
        സ്വതന്ത്രം (p->copy_symlink_targets[i]);
      സ്വതന്ത്രം (p->copy_symlink_targets);
    }
  സ്വതന്ത്രം (p->unified_cgroup_path);
  സ്വതന്ത്രം (p->host_notify_socket_path);
  സ്വതന്ത്രം (p->container_notify_socket_path);
  സ്വതന്ത്രം (p->external_descriptors);
  സ്വതന്ത്രം (p->maskdir_proc_path);
  സ്വതന്ത്രം (p->host_rootfs);
  സ്വതന്ത്രം (p);
}

സ്ഥിതം ഘടന private_data_s *
get_private_data (ഘടന libcrun_container_s *container)
{
  എങ്കിൽ (container->private_data == ശൂന്യ)
    {
      ഘടന private_data_s *p = xmalloc0 (വലുപ്പം (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_വൃക്ഷം_fd = -1;
      p->old_വേര്_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  തിരികെ container->private_data;
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

സ്ഥിതം ഘടന linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { ശൂന്യ, ശൂന്യ, 0 } };

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_and_reset (പൂർണ്ണസംഖ്യ *old)
{
  പൂർണ്ണസംഖ്യ tmp = *old;
  *old = -1;
  തിരികെ tmp;
}

പൂർണ്ണസംഖ്യ
libcrun_find_namespace (സ്ഥിരം അക്ഷരം *name)
{
  ഘടന linux_namespace_s *it;
  വേണ്ടി (it = namespaces; it->name; it++)
    എങ്കിൽ (strcmp (it->name, name) == 0)
      തിരികെ it->മൂല്യം;
  തിരികെ -1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
syscall_keyctl_join (സ്ഥിരം അക്ഷരം *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
syscall_pidfd_open (pid_t pid, unsigned പൂർണ്ണസംഖ്യ flags)
{
#എങ്കിൽ defined __NR_pidfd_open
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_pidfd_open, pid, flags);
#അല്ലെങ്കിൽ
  (ശൂന്യം) pid;
  (ശൂന്യം) flags;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
syscall_pidfd_send_signal (പൂർണ്ണസംഖ്യ pidfd, പൂർണ്ണസംഖ്യ sig, siginfo_t *info, unsigned പൂർണ്ണസംഖ്യ flags)
{
#എങ്കിൽ defined __NR_pidfd_send_signal
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#അല്ലെങ്കിൽ
  (ശൂന്യം) pidfd;
  (ശൂന്യം) sig;
  (ശൂന്യം) info;
  (ശൂന്യം) flags;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
സ്ഥിതം uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  എങ്കിൽ (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  എങ്കിൽ (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  എങ്കിൽ (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  എങ്കിൽ (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  എങ്കിൽ (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  എങ്കിൽ (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  എങ്കിൽ (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  എങ്കിൽ (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  തിരികെ attr;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_mount_setattr (bool recursive, സ്ഥിരം അക്ഷരം *target, പൂർണ്ണസംഖ്യ targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  ഘടന mount_attr_s attr = {
    0,
  };
  പൂർണ്ണസംഖ്യ ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  എങ്കിൽ (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      എങ്കിൽ (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      അല്ലെങ്കിൽ എങ്കിൽ (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      അല്ലെങ്കിൽ
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      എങ്കിൽ (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  അല്ലെങ്കിൽ എങ്കിൽ (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "mount_setattr `/%s`", target);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
get_bind_mount (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ open_വൃക്ഷം_fd = -1;
  ഘടന mount_attr_s attr = {
    0,
  };
  പൂർണ്ണസംഖ്യ recursive_flag = (recursive ? AT_RECURSIVE : 0);
  പൂർണ്ണസംഖ്യ ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  എങ്കിൽ (UNLIKELY (src == ശൂന്യ))
    തിരികെ crun_make_error (err, EINVAL, "mount source is not specified");

  എങ്കിൽ (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  എങ്കിൽ (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_വൃക്ഷം_fd = syscall_open_വൃക്ഷം (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  എങ്കിൽ (UNLIKELY (open_വൃക്ഷം_fd < 0))
    തിരികെ crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_വൃക്ഷം_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "mount_setattr `%s`", src);

  തിരികെ get_and_reset (&open_വൃക്ഷം_fd);
}

പൂർണ്ണസംഖ്യ
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, അക്ഷരം *option, അക്ഷരം **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free അക്ഷരം *mappings = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *it;
  പൂർണ്ണസംഖ്യ ret;

  mappings = xmalloc (allocated);

  വേണ്ടി (it = option; *it;)
    {
      bool relative = false;
      long മൂല്യം[3];
      size_t i;

      എങ്കിൽ (*it == '\0')
        നിർത്തുക;

      എങ്കിൽ (*it == '#')
        it++;

      എങ്കിൽ (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      വേണ്ടി (i = 0; i < 3; i++)
        {
          അക്ഷരം *endptr = ശൂന്യ;

          എങ്കിൽ (i > 0 && *it == '-')
            it++;

          എങ്കിൽ (*it == '\0')
            തിരികെ crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          മൂല്യം[i] = strtol (it, &endptr, 10);
          എങ്കിൽ (errno || endptr == it)
            തിരികെ crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      എങ്കിൽ (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          എങ്കിൽ (def == ശൂന്യ
              || def->linux == ശൂന്യ
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            തിരികെ crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          വേണ്ടി (i = 0; i < mappings_len; i++)
            എങ്കിൽ (മൂല്യം[1] >= (int64_t) mappings[i]->container_id
                && മൂല്യം[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              നിർത്തുക;

          എങ്കിൽ (i == mappings_len)
            തിരികെ crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          മൂല്യം[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      എങ്കിൽ (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", മൂല്യം[0], മൂല്യം[1], മൂല്യം[2]);
      എങ്കിൽ (UNLIKELY (ret >= (പൂർണ്ണസംഖ്യ) (allocated - written)))
        തിരികെ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = ശൂന്യ;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
format_mount_mappings (അക്ഷരം **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  സ്ഥിരം size_t max_len_mapping = 64;
  cleanup_free അക്ഷരം *ret = ശൂന്യ;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  വേണ്ടി (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      എങ്കിൽ (UNLIKELY (len >= max_len_mapping))
        തിരികെ crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = ശൂന്യ;
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
format_mount_mapping (അക്ഷരം **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t size, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .size = size,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    ശൂന്യ,
  };

  തിരികെ format_mount_mappings (ret, mappings, 1, written, err);
}

സ്ഥിതം bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  എങ്കിൽ (def->linux == ശൂന്യ)
    തിരികെ mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  എങ്കിൽ (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    തിരികെ false;

  എങ്കിൽ (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    തിരികെ false;

  വേണ്ടി (s = 0; s < mnt->uid_mappings_len; s++)
    {
      എങ്കിൽ (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        തിരികെ false;
      എങ്കിൽ (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        തിരികെ false;
      എങ്കിൽ (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        തിരികെ false;
    }

  വേണ്ടി (s = 0; s < mnt->gid_mappings_len; s++)
    {
      എങ്കിൽ (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        തിരികെ false;
      എങ്കിൽ (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        തിരികെ false;
      എങ്കിൽ (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        തിരികെ false;
    }

  തിരികെ true;
}

സ്ഥിതം pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        സ്ഥിരം അക്ഷരം *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != ശൂന്യ;

  എങ്കിൽ (! need_new_userns)
    തിരികെ 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (pid < 0))
    തിരികെ crun_make_error (err, errno, "clone");

  എങ്കിൽ (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      ആകുമ്പോൾ (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  എങ്കിൽ (mnt->uid_mappings_len)
    {
      cleanup_free അക്ഷരം *uid_map = ശൂന്യ;
      cleanup_free അക്ഷരം *gid_map = ശൂന്യ;
      size_t written = 0;
      പൂർണ്ണസംഖ്യ ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      എങ്കിൽ (UNLIKELY (fd < 0))
        തിരികെ fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      എങ്കിൽ (UNLIKELY (fd < 0))
        തിരികെ fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      cleanup_free അക്ഷരം *dup_options = ശൂന്യ;
      അക്ഷരം *option, *saveptr = ശൂന്യ;

      എങ്കിൽ (! options)
        തിരികെ crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      വേണ്ടി (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (ശൂന്യ, ";", &saveptr))
        {
          cleanup_free അക്ഷരം *mappings = ശൂന്യ;
          cleanup_close പൂർണ്ണസംഖ്യ map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          പൂർണ്ണസംഖ്യ ret;

          എങ്കിൽ (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              എങ്കിൽ (UNLIKELY (map_fd < 0))
                തിരികെ map_fd;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              എങ്കിൽ (UNLIKELY (map_fd < 0))
                തിരികെ map_fd;
            }
          അല്ലെങ്കിൽ
            തിരികെ crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_create_keyring (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *label, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ labelfd = -1;
  bool label_set = false;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      എങ്കിൽ (UNLIKELY (labelfd < 0))
        {
          എങ്കിൽ (crun_error_get_errno (err) != ENOENT)
            തിരികെ labelfd;

          crun_error_release (err);
        }

      എങ്കിൽ (labelfd >= 0)
        {
          ret = ലിഖ (labelfd, label, strlen (label));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          പോകുക out;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      പോകുക out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  എങ്കിൽ (label_set && ലിഖ (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  തിരികെ ret;
}

സ്ഥിതം ശൂന്യം
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  എങ്കിൽ (def->process && def->process->user)
    {
      എങ്കിൽ (def->process->user->uid)
        *uid = def->process->user->uid;
      എങ്കിൽ (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

സ്ഥിതം unsigned long
get_mount_flags (സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ current_flags, പൂർണ്ണസംഖ്യ *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  സ്ഥിരം ഘടന propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  എങ്കിൽ (found)
    *found = prop ? 1 : 0;

  എങ്കിൽ (prop == ശൂന്യ)
    തിരികെ 0;

  എങ്കിൽ (prop->extra_flags & OPTION_RECURSIVE)
    {
      എങ്കിൽ (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      എങ്കിൽ (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  എങ്കിൽ (extra_flags)
    *extra_flags |= prop->extra_flags;

  എങ്കിൽ (prop->clear)
    തിരികെ current_flags & ~prop->flags;

  തിരികെ current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
സ്ഥിതം bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  വേണ്ടി (i = 0; i < mount->options_len; i++)
    {
      സ്ഥിരം ഘടന propagation_flags_s *prop;

      എങ്കിൽ (mount->options[i] == ശൂന്യ)
        തുടരുക;

      prop = libcrun_str2mount_flags (mount->options[i]);
      എങ്കിൽ (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        തിരികെ true;
    }

  തിരികെ false;
}

സ്ഥിതം unsigned long
get_mount_flags_or_option (സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ current_flags, unsigned long *extra_flags, അക്ഷരം **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  പൂർണ്ണസംഖ്യ found;
  __attribute__ ((unused)) cleanup_free അക്ഷരം *prev = ശൂന്യ;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  എങ്കിൽ (name == ശൂന്യ)
    തിരികെ 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  എങ്കിൽ (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      സ്ഥിരം അക്ഷരം *data_flag = ശൂന്യ;
      എങ്കിൽ ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      അല്ലെങ്കിൽ എങ്കിൽ ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      എങ്കിൽ (data_flag)
        {
          prev = *option;
          എങ്കിൽ (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          അല്ലെങ്കിൽ
            *option = xstrdup (data_flag);
        }
      തിരികെ flags;
    }

  prev = *option;
  എങ്കിൽ (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  അല്ലെങ്കിൽ
    *option = xstrdup (name);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
pivot_വേര് (സ്ഥിരം അക്ഷരം *new_വേര്, സ്ഥിരം അക്ഷരം *put_old)
{
  തിരികെ syscall (__NR_pivot_root, new_വേര്, put_old);
}

സ്ഥിതം ശൂന്യം
free_remount (ഘടന remount_s *r)
{
  എങ്കിൽ (r == ശൂന്യ)
    തിരികെ;
  എങ്കിൽ (r->targetfd >= 0)
    close (r->targetfd);
  സ്വതന്ത്രം (r->data);
  സ്വതന്ത്രം (r->target);
  സ്വതന്ത്രം (r);
}

സ്ഥിതം ഘടന remount_s *
make_remount (പൂർണ്ണസംഖ്യ targetfd, സ്ഥിരം അക്ഷരം *target, unsigned long flags, സ്ഥിരം അക്ഷരം *data, ഘടന remount_s *next)
{
  ഘടന remount_s *ret = xmalloc (വലുപ്പം (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : ശൂന്യ;
  ret->next = next;
  ret->targetfd = targetfd;
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_remount (പൂർണ്ണസംഖ്യ targetfd, സ്ഥിരം അക്ഷരം *target, unsigned long flags, സ്ഥിരം അക്ഷരം *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  പൂർണ്ണസംഖ്യ ret;
  സ്ഥിരം അക്ഷരം *real_target = target;

  എങ്കിൽ (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      എങ്കിൽ (LIKELY (ret == 0))
        തിരികെ 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  എങ്കിൽ (flags & (MS_REMOUNT | MS_RDONLY))
    data = ശൂന്യ;

  ret = mount (ശൂന്യ, real_target, ശൂന്യ, flags, data);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      ഘടന statfs sfs;

      ret = statfs (real_target, &sfs);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      എങ്കിൽ (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      എങ്കിൽ ((flags | remount_flags) != flags)
        {
          ret = mount (ശൂന്യ, real_target, ശൂന്യ, flags | remount_flags, data);
          എങ്കിൽ (LIKELY (ret == 0))
            തിരികെ 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          എങ്കിൽ (current_flags & MS_RDONLY)
            ret = mount (ശൂന്യ, real_target, ശൂന്യ, flags | remount_flags | MS_RDONLY, data);
        }
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "remount `%s`", target);
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret = 0;
  ഘടന remount_s *r = get_private_data (container)->remounts;
  ആകുമ്പോൾ (r)
    {
      ഘടന remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      എങ്കിൽ (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          എങ്കിൽ (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              തുടരുക;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  ആകുമ്പോൾ (r)
    {
      ഘടന remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = ശൂന്യ;
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
open_mount_target (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *target_rel, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *rootfs = get_private_data (container)->rootfs;
  പൂർണ്ണസംഖ്യ rootfsfd = get_private_data (container)->rootfsfd;

  എങ്കിൽ (rootfsfd < 0)
    തിരികെ crun_make_error (err, 0, "invalid rootfs state");

  തിരികെ safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
fsopen_mount (സ്ഥിരം അക്ഷരം *type, സ്ഥിരം അക്ഷരം *source_name, സ്ഥിരം അക്ഷരം *labeltype, സ്ഥിരം അക്ഷരം *label, സ്ഥിരം അക്ഷരം *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close പൂർണ്ണസംഖ്യ fsfd = -1;
  പൂർണ്ണസംഖ്യ ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  എങ്കിൽ (UNLIKELY (fsfd < 0))
    തിരികെ fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (ശൂന്യം) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  എങ്കിൽ (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (data && data[0])
    {
      cleanup_free അക്ഷരം *data_copy = xstrdup (data);
      അക്ഷരം *saveptr = ശൂന്യ;
      അക്ഷരം *token;

      വേണ്ടി (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (ശൂന്യ, ",", &saveptr))
        {
          അക്ഷരം *eq = strchr (token, '=');
          എങ്കിൽ (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          അല്ലെങ്കിൽ
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, ശൂന്യ, 0);

          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ശൂന്യ, ശൂന്യ, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#അല്ലെങ്കിൽ
  (ശൂന്യം) type;
  (ശൂന്യം) source_name;
  (ശൂന്യം) data;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_procfd (ഘടന private_data_s *data, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ fd;

  എങ്കിൽ (data->procfd >= 0)
    തിരികെ data->procfd;

  fd = fsopen_mount ("proc", ശൂന്യ, ശൂന്യ, ശൂന്യ, ശൂന്യ);
  എങ്കിൽ (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  തിരികെ fd;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_old_വേര്_fd (ഘടന private_data_s *data)
{
  എങ്കിൽ (data->old_വേര്_fd >= 0)
    തിരികെ data->old_വേര്_fd;

  data->old_വേര്_fd = open ("/", O_PATH | O_CLOEXEC);
  തിരികെ data->old_വേര്_fd;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
fs_move_mount_to (പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *name)
{
#ifdef HAVE_NEW_MOUNT_API
  എങ്കിൽ (name)
    തിരികെ syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  തിരികെ syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#അല്ലെങ്കിൽ
  (ശൂന്യം) fd;
  (ശൂന്യം) dirfd;
  (ശൂന്യം) name;
  (ശൂന്യം) syscall_move_mount;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

എണ്ണൽ
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

സ്ഥിതം പൂർണ്ണസംഖ്യ do_mount (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *source, പൂർണ്ണസംഖ്യ targetfd,
                     സ്ഥിരം അക്ഷരം *target, സ്ഥിരം അക്ഷരം *fstype, unsigned long mountflags,
                     സ്ഥിരം ശൂന്യം *data, പൂർണ്ണസംഖ്യ extra_flags, libcrun_error_t *err);

സ്ഥിതം bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, സ്ഥിരം അക്ഷരം *option)
{
  size_t i;
  വേണ്ടി (i = 0; i < mnt->options_len; i++)
    എങ്കിൽ (mnt->options[i] != ശൂന്യ && strcmp (mnt->options[i], option) == 0)
      തിരികെ true;
  തിരികെ false;
}

സ്ഥിതം runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  വേണ്ടി (i = 0; i < def->mounts_len; i++)
    {
      എങ്കിൽ (strcmp (def->mounts[i]->destination, destination) == 0)
        തിരികെ def->mounts[i];
    }
  തിരികെ ശൂന്യ;
}

സ്ഥിതം ശൂന്യം
warn_tmpfs_fallback_once (ഘടന private_data_s *private_data, സ്ഥിരം അക്ഷരം *reason)
{
  എങ്കിൽ (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ
get_shared_empty_dir_cached (libcrun_container_t *container, അക്ഷരം **proc_fd_path, libcrun_error_t *err)
{
  ഘടന private_data_s *private_data = get_private_data (container);
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  cleanup_free അക്ഷരം *empty_dir_path = ശൂന്യ;
  proc_fd_path_t fd_path;
  പൂർണ്ണസംഖ്യ ret;

  /* Fast path: return cached proc fd path if already set up */
  എങ്കിൽ (private_data->maskdir_proc_path != ശൂന്യ)
    {
      എങ്കിൽ (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      തിരികെ 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_വേര്, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  എങ്കിൽ (fd < 0)
    തിരികെ fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  എങ്കിൽ (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  തിരികെ 0;
}

സ്ഥിതം bool
is_dev_null (സ്ഥിരം ഘടന stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  തിരികെ S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
ensure_dev_null (പൂർണ്ണസംഖ്യ fd, libcrun_error_t *err)
{
  ഘടന stat st;

  എങ്കിൽ (UNLIKELY (fstat (fd, &st) < 0))
    തിരികെ crun_make_error (err, errno, "stat `/dev/null`");

  എങ്കിൽ (UNLIKELY (! is_dev_null (&st)))
    തിരികെ crun_make_error (err, 0, "`/dev/null` is not the null device");

  തിരികെ 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
സ്ഥിതം bool
is_empty_ro_tmpfs (പൂർണ്ണസംഖ്യ fd)
{
  ഘടന statfs sfs;

  എങ്കിൽ (fstatfs (fd, &sfs) < 0)
    തിരികെ false;

  തിരികെ sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
mount_masked_dir (libcrun_container_t *container, പൂർണ്ണസംഖ്യ pathfd, സ്ഥിരം അക്ഷരം *rel_path, libcrun_error_t *err)
{
  ഘടന private_data_s *private_data = get_private_data (container);
  cleanup_close പൂർണ്ണസംഖ്യ newfd = -1;
  libcrun_error_t tmp_err = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (private_data->maskdir_fd < 0)
    {
      എങ്കിൽ (private_data->maskdir_bind_failed)
        പോകുക fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, ശൂന്യ, &tmp_err);
      എങ്കിൽ (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->സന്ദേശം);
          crun_error_release (&tmp_err);
          പോകുക fallback_to_tmpfs;
        }
    }

  എങ്കിൽ (private_data->maskdir_fd >= 0)
    {
      പൂർണ്ണസംഖ്യ rootfsfd = private_data->rootfsfd;
      ഘടന stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, ശൂന്യ);
      എങ്കിൽ (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          എങ്കിൽ (UNLIKELY (newfd < 0))
            തിരികെ newfd;

          ret = fstat (newfd, &after);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          എങ്കിൽ (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            തിരികെ crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          തിരികെ 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_path);

      TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
      private_data->maskdir_fd = -1;
      private_data->maskdir_move_failed = true;
    }

  private_data->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_data, tmp_err ? tmp_err->സന്ദേശം : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  എങ്കിൽ (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      എങ്കിൽ (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_masked_or_readonly_path (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  സ്ഥിരം അക്ഷരം *rootfs = get_private_data (container)->rootfs;
  cleanup_close പൂർണ്ണസംഖ്യ pathfd = -1;
  ഘടന statfs sfs;
  പൂർണ്ണസംഖ്യ ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  എങ്കിൽ (UNLIKELY (rel_path == ശൂന്യ))
    തിരികെ crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  എങ്കിൽ (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  എങ്കിൽ (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      എങ്കിൽ (errno != ENOENT && errno != EACCES)
        തിരികെ pathfd;

      എങ്കിൽ (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      തിരികെ 0;
    }

  എങ്കിൽ (readonly)
    {
      പൂർണ്ണസംഖ്യ procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close പൂർണ്ണസംഖ്യ mountfd = -1;

      എങ്കിൽ (UNLIKELY (procfd < 0))
        തിരികെ procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      എങ്കിൽ (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          എങ്കിൽ (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, ശൂന്യ);
          അല്ലെങ്കിൽ
            ret = -1;

          എങ്കിൽ (ret < 0)
            crun_error_release (err);
        }

      എങ്കിൽ (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          എങ്കിൽ (keep_flags)
            {
              cleanup_close പൂർണ്ണസംഖ്യ sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              എങ്കിൽ (UNLIKELY (sfd < 0))
                തിരികെ crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          എങ്കിൽ (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;

              ret = fs_move_mount_to (mountfd, pathfd, ശൂന്യ);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          അല്ലെങ്കിൽ
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, ശൂന്യ, mount_flags, ശൂന്യ,
                              LABEL_NONE, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;
            }
        }
    }
  അല്ലെങ്കിൽ
    {
      ret = get_file_type_fd (pathfd, &mode);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      എങ്കിൽ ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      അല്ലെങ്കിൽ
        {
          cleanup_close പൂർണ്ണസംഖ്യ nullfd = -1;
          cleanup_close പൂർണ്ണസംഖ്യ mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          എങ്കിൽ (UNLIKELY (nullfd < 0))
            തിരികെ crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          എങ്കിൽ (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, ശൂന്യ);

          എങ്കിൽ (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, ശൂന്യ, MS_BIND | MS_RDONLY, ശൂന്യ,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  എങ്കിൽ (context_type)
    {
      എങ്കിൽ (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          തിരികെ ശൂന്യ;
        }
      തിരികെ context_type;
    }

  തിരികെ "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
diagnose_mount_failure (പൂർണ്ണസംഖ്യ ret, libcrun_error_t *err, libcrun_container_t *container,
                        സ്ഥിരം അക്ഷരം *fstype)
{
  എങ്കിൽ (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      എങ്കിൽ (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          തിരികെ ret;
        }
    }
  തിരികെ ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
sysfs_userns_fallback (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *target,
                       സ്ഥിരം അക്ഷരം *real_target, പൂർണ്ണസംഖ്യ targetfd,
                       unsigned long mountflags, സ്ഥിരം അക്ഷരം *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close പൂർണ്ണസംഖ്യ mountfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ sysfd = -1;
  പൂർണ്ണസംഖ്യ sys_old_വേര്_fd = -1;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    തിരികെ 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (ret == 0)
    തിരികെ 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_വേര്_fd = get_old_വേര്_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  എങ്കിൽ (sys_old_വേര്_fd >= 0)
    mountfd = get_bind_mount (sys_old_വേര്_fd, "sys", true, true, false, MS_PRIVATE, err);
  എങ്കിൽ (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  എങ്കിൽ (cgroup_mount == ശൂന്യ)
    {
      എങ്കിൽ (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, ശൂന്യ);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      അല്ലെങ്കിൽ
        {
          crun_error_release (err);
          എങ്കിൽ (sys_old_വേര്_fd >= 0)
            {
              cleanup_free അക്ഷരം *sys_path = ശൂന്യ;
              proc_fd_path_t sys_വേര്_path;

              get_proc_self_fd_path (sys_വേര്_path, sys_old_വേര്_fd);
              xasprintf (&sys_path, "%s/sys", sys_വേര്_path);
              ret = mount (sys_path, real_target, ശൂന്യ, MS_BIND | MS_REC, ശൂന്യ);
            }
          അല്ലെങ്കിൽ
            ret = mount ("/sys", real_target, ശൂന്യ, MS_BIND | MS_REC, ശൂന്യ);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      എങ്കിൽ (UNLIKELY (sysfd < 0))
        തിരികെ sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        ശൂന്യ, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      തിരികെ do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  എങ്കിൽ (UNLIKELY (mountfd < 0))
    തിരികെ mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  എങ്കിൽ (UNLIKELY (sysfd < 0))
    തിരികെ sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
സ്ഥിതം ശൂന്യം
apply_selinux_xattr (libcrun_container_t *container, പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ extra_flags, സ്ഥിരം അക്ഷരം *label)
{
  libcrun_error_t tmp_err = ശൂന്യ;
  പൂർണ്ണസംഖ്യ procfd;

  എങ്കിൽ ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    തിരികെ;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  എങ്കിൽ (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close പൂർണ്ണസംഖ്യ xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      എങ്കിൽ (xfd >= 0)
        (ശൂന്യം) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
do_mount_replace_rootfs (libcrun_container_t *container, പൂർണ്ണസംഖ്യ *fd, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ procfd = get_procfd (get_private_data (container), err);
  പൂർണ്ണസംഖ്യ tmp;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (UNLIKELY (procfd < 0))
    തിരികെ procfd;

  {
    cleanup_close പൂർണ്ണസംഖ്യ nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    എങ്കിൽ (UNLIKELY (nsfd < 0))
      തിരികെ crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    എങ്കിൽ (UNLIKELY (ret < 0))
      തിരികെ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (*fd < 0))
    തിരികെ crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  എങ്കിൽ (UNLIKELY (tmp < 0))
    തിരികെ crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  തിരികെ 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
apply_propagation_flags (സ്ഥിരം അക്ഷരം *target, പൂർണ്ണസംഖ്യ targetfd, സ്ഥിരം അക്ഷരം *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (targetfd >= 0)
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      എങ്കിൽ (LIKELY (ret == 0))
        propagation_done = true;
      അല്ലെങ്കിൽ
        crun_error_release (&tmp_err);
    }

  എങ്കിൽ (! propagation_done)
    {
      ret = mount (ശൂന്യ, real_target, ശൂന്യ, mountflags & ALL_PROPAGATIONS, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  തിരികെ 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
schedule_or_do_remount (libcrun_container_t *container, പൂർണ്ണസംഖ്യ *fd, പൂർണ്ണസംഖ്യ targetfd,
                        സ്ഥിരം അക്ഷരം *target, സ്ഥിരം അക്ഷരം *real_target,
                        bool single_instance, unsigned long mountflags,
                        സ്ഥിരം ശൂന്യം *data, പൂർണ്ണസംഖ്യ extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  ഘടന remount_s *r;

  എങ്കിൽ ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    തിരികെ do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  എങ്കിൽ (*fd < 0)
    {
      *fd = dup (targetfd);
      എങ്കിൽ (UNLIKELY (*fd < 0))
        തിരികെ crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_mount (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *source, പൂർണ്ണസംഖ്യ targetfd,
          സ്ഥിരം അക്ഷരം *target, സ്ഥിരം അക്ഷരം *fstype, unsigned long mountflags, സ്ഥിരം ശൂന്യം *data,
          പൂർണ്ണസംഖ്യ extra_flags, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *data_with_label = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *data_without_label = data;
  proc_fd_path_t target_buffer;
  സ്ഥിരം അക്ഷരം *context_type = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ ms_move_fd = -1;
  സ്ഥിരം അക്ഷരം *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  സ്ഥിരം അക്ഷരം *label = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret = 0;

  എങ്കിൽ (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  അല്ലെങ്കിൽ
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  എങ്കിൽ (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  എങ്കിൽ ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      എങ്കിൽ (UNLIKELY (context_type == ശൂന്യ))
        തിരികെ -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      എങ്കിൽ (ret < 0)
        തിരികെ ret;
      data = data_with_label;
    }

  എങ്കിൽ (mountflags & MS_MOVE)
    {
      എങ്കിൽ ((mountflags & MS_BIND) || fstype)
        തിരികെ crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, ശൂന്യ, MS_MOVE, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      എങ്കിൽ (UNLIKELY (ms_move_fd < 0))
        തിരികെ ms_move_fd;
      targetfd = ms_move_fd;
    }

  എങ്കിൽ ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      എങ്കിൽ (targetfd >= 0)
        {
          എങ്കിൽ (mountflags & MS_BIND)
            {
              cleanup_close പൂർണ്ണസംഖ്യ bindfd = -1;
              libcrun_error_t tmp_err = ശൂന്യ;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              എങ്കിൽ (bindfd < 0)
                {
                  പൂർണ്ണസംഖ്യ old_വേര്_fd = get_old_വേര്_fd (get_private_data (container));

                  എങ്കിൽ (old_വേര്_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_വേര്_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              എങ്കിൽ (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, ശൂന്യ);
              എങ്കിൽ (ret < 0)
                crun_error_release (&tmp_err);
            }
          അല്ലെങ്കിൽ
            {
              cleanup_close പൂർണ്ണസംഖ്യ newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              എങ്കിൽ (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, ശൂന്യ);
            }
        }

      എങ്കിൽ (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          എങ്കിൽ (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              പൂർണ്ണസംഖ്യ old_വേര്_fd = get_old_വേര്_fd (get_private_data (container));

              എങ്കിൽ (old_വേര്_fd >= 0)
                {
                  cleanup_free അക്ഷരം *proc_source_buf = ശൂന്യ;
                  proc_fd_path_t old_വേര്_path;

                  get_proc_self_fd_path (old_വേര്_path, old_വേര്_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_വേര്_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          പൂർണ്ണസംഖ്യ saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          എങ്കിൽ (ret == 0)
            തിരികെ 0;
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          തിരികെ diagnose_mount_failure (ret, err, container, fstype);
        }

      എങ്കിൽ (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          എങ്കിൽ (UNLIKELY (fd < 0))
            തിരികെ fd;

          /* We are replacing the rootfs, reopen it.  */
          എങ്കിൽ (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;
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

  എങ്കിൽ (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  എങ്കിൽ (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  എങ്കിൽ (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ ret;
}

സ്ഥിതം ശൂന്യം
try_umount (പൂർണ്ണസംഖ്യ targetfd, സ്ഥിരം അക്ഷരം *target)
{
  proc_fd_path_t target_buffer;
  സ്ഥിരം അക്ഷരം *real_target = target;

  എങ്കിൽ (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

സ്ഥിതം bool
container_has_cgroupns (libcrun_container_t *container)
{
  തിരികെ get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_mount_cgroup_v2 (libcrun_container_t *container, പൂർണ്ണസംഖ്യ targetfd, സ്ഥിരം അക്ഷരം *target,
                    unsigned long mountflags, സ്ഥിരം അക്ഷരം *data, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      പൂർണ്ണസംഖ്യ saved_errno = crun_error_get_errno (err);
      എങ്കിൽ (saved_errno == EPERM || saved_errno == EBUSY)
        {
          സ്ഥിരം അക്ഷരം *unified_cgroup_path;
          സ്ഥിരം അക്ഷരം *src_cgroup;

          crun_error_release (err);

          എങ്കിൽ (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              എങ്കിൽ (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  എങ്കിൽ (LIKELY (ret == 0))
                    തിരികെ ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_വേര്;
          തിരികെ do_mount (container, src_cgroup, targetfd, target, ശൂന്യ,
                           MS_BIND | mountflags, ശൂന്യ, LABEL_NONE, err);
        }

      തിരികെ ret;
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *source, പൂർണ്ണസംഖ്യ targetfd, സ്ഥിരം അക്ഷരം *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  സ്ഥിരം അക്ഷരം *subsystem = "systemd";
  cleanup_free അക്ഷരം *subsystem_path = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  എങ്കിൽ (UNLIKELY (tmpfsdirfd < 0))
    തിരികെ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "openat `%s`", subsystem_path);

  തിരികെ do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_mount_cgroup_v1 (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *source, പൂർണ്ണസംഖ്യ targetfd, സ്ഥിരം അക്ഷരം *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  അക്ഷരം *from;
  cleanup_close പൂർണ്ണസംഖ്യ tmpfsdirfd = -1;
  അക്ഷരം *saveptr = ശൂന്യ;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  എങ്കിൽ (UNLIKELY (tmpfsdirfd < 0))
    തിരികെ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    പൂർണ്ണസംഖ്യ procfd = get_procfd (get_private_data (container), err);
    എങ്കിൽ (UNLIKELY (procfd < 0))
      തിരികെ procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, ശൂന്യ, err);
  }
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (UNLIKELY (content == ശൂന്യ || content[0] == '\0'))
    തിരികെ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  വേണ്ടി (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ശൂന്യ, "\n", &saveptr))
    {
      cleanup_free അക്ഷരം *source_path = ശൂന്യ;
      cleanup_free അക്ഷരം *source_subsystem = ശൂന്യ;
      cleanup_free അക്ഷരം *subsystem_path = ശൂന്യ;
      അക്ഷരം *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close പൂർണ്ണസംഖ്യ subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      എങ്കിൽ (subsystem[0] == '\0')
        തുടരുക;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      എങ്കിൽ (it)
        subsystem = it + 5;

      എങ്കിൽ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      എങ്കിൽ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_വേര്, subsystem, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      /* if there is already a mount specified, do not add a default one.  */
      എങ്കിൽ (find_mount_for (container, source_subsystem) != ശൂന്യ)
        തുടരുക;

      ret = append_paths (&source_path, err, source_subsystem, subpath, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      എങ്കിൽ (UNLIKELY (subsystemfd < 0))
        തിരികെ crun_make_error (err, errno, "open `%s`", subsystem_path);

      എങ്കിൽ (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              എങ്കിൽ (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  തുടരുക;
                }
              തിരികെ ret;
            }
        }
      അല്ലെങ്കിൽ
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, ശൂന്യ, MS_BIND | mountflags, ശൂന്യ,
                          LABEL_NONE, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              എങ്കിൽ (crun_error_get_errno (err) != ENOENT)
                തിരികെ ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, ശൂന്യ, MS_BIND | mountflags,
                              ശൂന്യ, LABEL_NONE, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  എങ്കിൽ (crun_error_get_errno (err) != ENOENT)
                    തിരികെ ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_mount_cgroup (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *source, പൂർണ്ണസംഖ്യ targetfd, സ്ഥിരം അക്ഷരം *target,
                 unsigned long mountflags, സ്ഥിരം അക്ഷരം *data, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  തിരഞ്ഞെടുപ്പ് (cgroup_mode)
    {
    അവസ്ഥ CGROUP_MODE_UNIFIED:
      തിരികെ do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    അവസ്ഥ CGROUP_MODE_LEGACY:
    അവസ്ഥ CGROUP_MODE_HYBRID:
      തിരികെ do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  തിരികെ crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

ഘടന device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
സ്ഥിതം ശൂന്യം
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  ഘടന private_data_s *private_data = get_private_data (container);
  size_t i;

  എങ്കിൽ (private_data->needed_devs_fds)
    {
      ഘടന libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      വേണ്ടി (i = 0; needed_devs[i].path; i++)
        {
          എങ്കിൽ (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            തുടരുക;

          പൂർണ്ണസംഖ്യ fd = syscall_open_വൃക്ഷം (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          എങ്കിൽ (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          അല്ലെങ്കിൽ എങ്കിൽ (fd >= 0)
            close (fd);
        }
    }

  എങ്കിൽ (private_data->notify_socket_വൃക്ഷം_fd < 0
      && private_data->host_notify_socket_path)
    {
      പൂർണ്ണസംഖ്യ fd = syscall_open_വൃക്ഷം (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      എങ്കിൽ (fd >= 0)
        private_data->notify_socket_വൃക്ഷം_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
സ്ഥിതം സ്ഥിരം അക്ഷരം *
relative_path_under_dev (സ്ഥിരം അക്ഷരം *path)
{
  എങ്കിൽ (path[0] != '/')
    തിരികെ path;
  എങ്കിൽ (strncmp (path, "/dev/", 5) == 0)
    {
      എങ്കിൽ (strchr (path + 5, '/'))
        തിരികെ ശൂന്യ;
      തിരികെ path + 5;
    }
  തിരികെ ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
mknod_and_set_attrs (libcrun_container_t *container, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *name, ഘടന device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  proc_fd_path_t fd_path;
  സ്ഥിരം അക്ഷരം *what = "new";
  bool exists = false;
  ഘടന stat st;
  പൂർണ്ണസംഖ്യ procfd;
  പൂർണ്ണസംഖ്യ ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno != EEXIST)
        തിരികെ crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  എങ്കിൽ (UNLIKELY ((st.st_mode & S_IFMT) != type))
    തിരികെ crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  എങ്കിൽ (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    തിരികെ crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  എങ്കിൽ (exists)
    തിരികെ 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  എങ്കിൽ (UNLIKELY (procfd < 0))
    തിരികെ procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fchown `%s`", device->path);

  തിരികെ 1;
}

പൂർണ്ണസംഖ്യ
libcrun_create_dev (libcrun_container_t *container, പൂർണ്ണസംഖ്യ devfd, പൂർണ്ണസംഖ്യ srcfd,
                    ഘടന device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  സ്ഥിരം അക്ഷരം *fullname = device->path;
  സ്ഥിരം അക്ഷരം *rootfs = get_private_data (container)->rootfs;
  എങ്കിൽ (is_empty_string (fullname))
    തിരികെ crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free അക്ഷരം *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  എങ്കിൽ (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  സ്ഥിരം അക്ഷരം *rel_dev = relative_path_under_dev (normalized_path);

  എങ്കിൽ (binds)
    {
      cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

      എങ്കിൽ (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          എങ്കിൽ (UNLIKELY (fd < 0))
            {
              എങ്കിൽ (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              എങ്കിൽ (UNLIKELY (fd < 0))
                തിരികെ crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      അല്ലെങ്കിൽ
        {
          സ്ഥിരം അക്ഷരം *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          എങ്കിൽ (UNLIKELY (fd < 0))
            തിരികെ fd;
        }

      എങ്കിൽ (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          എങ്കിൽ (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              എങ്കിൽ (LIKELY (ret >= 0))
                തിരികെ 0;
            }
          അല്ലെങ്കിൽ
            crun_error_release (err);
        }

      {
        cleanup_close പൂർണ്ണസംഖ്യ mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        എങ്കിൽ (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            എങ്കിൽ (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, ശൂന്യ);
                എങ്കിൽ (LIKELY (ret == 0))
                  തിരികെ 0;
              }
            അല്ലെങ്കിൽ
              crun_error_release (err);
          }
        അല്ലെങ്കിൽ
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, ശൂന്യ, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, ശൂന്യ, LABEL_MOUNT, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      എങ്കിൽ (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          എങ്കിൽ (ret <= 0)
            തിരികെ ret;
        }
      അല്ലെങ്കിൽ
        {
          cleanup_close പൂർണ്ണസംഖ്യ dirfd = -1;
          cleanup_free അക്ഷരം *dirname = ശൂന്യ;
          അക്ഷരം *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          എങ്കിൽ (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          എങ്കിൽ (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              എങ്കിൽ (UNLIKELY (dirfd < 0))
                തിരികെ crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          അല്ലെങ്കിൽ
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              എങ്കിൽ (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              എങ്കിൽ (UNLIKELY (dirfd < 0))
                തിരികെ dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          എങ്കിൽ (ret <= 0)
            തിരികെ ret;
        }
    }
  തിരികെ 0;
}

ഘടന symlink_s
{
  സ്ഥിരം അക്ഷരം *path;
  സ്ഥിരം അക്ഷരം *target;
  bool force;
};

സ്ഥിതം ഘടന symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { ശൂന്യ, ശൂന്യ, false } };

സ്ഥിതം പൂർണ്ണസംഖ്യ
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  size_t i;
  ഘടന device_s *it;
  cleanup_close പൂർണ്ണസംഖ്യ devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  സ്ഥിരം അക്ഷരം *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map ഘടന libcrun_fd_map *dev_fds = ശൂന്യ;
  cleanup_close_map ഘടന libcrun_fd_map *needed_fds = ശൂന്യ;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = ശൂന്യ;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = ശൂന്യ;

  എങ്കിൽ (! def || ! def->linux)
    തിരികെ 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  എങ്കിൽ (UNLIKELY (devfd < 0))
    തിരികെ devfd;

  വേണ്ടി (i = 0; i < def->linux->devices_len; i++)
    {
      ഘടന device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      എങ്കിൽ (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  വേണ്ടി (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close പൂർണ്ണസംഖ്യ srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      എങ്കിൽ (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  വേണ്ടി (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          പൂർണ്ണസംഖ്യ saved_errno = errno;

          എങ്കിൽ (errno == EEXIST && ! symlinks[i].force)
            തുടരുക;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          എങ്കിൽ (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              എങ്കിൽ (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              എങ്കിൽ (ret < 0 && errno == EBUSY)
                {
                  cleanup_close പൂർണ്ണസംഖ്യ tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  എങ്കിൽ (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      എങ്കിൽ (umount2 (procpath, MNT_DETACH) == 0)
                        പോകുക retry_unlink;
                    }
                }
              എങ്കിൽ (ret == 0)
                പോകുക retry_symlink;
            }
          തിരികെ crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  എങ്കിൽ (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
സ്ഥിതം സ്ഥിരം അക്ഷരം *
next_path_component (സ്ഥിരം അക്ഷരം *p, size_t *len)
{
  വേണ്ടി (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      എങ്കിൽ (*len != 1 || p[0] != '.')
        തിരികെ p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
സ്ഥിതം bool
same_path (സ്ഥിരം അക്ഷരം *a, സ്ഥിരം അക്ഷരം *b)
{
  size_t la, lb;

  വേണ്ടി (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      എങ്കിൽ (la != lb || memcmp (a, b, la) != 0)
        തിരികെ false;
      എങ്കിൽ (la == 0)
        തിരികെ true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
സ്ഥിതം bool
is_duplicate_path (അക്ഷരം **paths, size_t i)
{
  size_t j;

  എങ്കിൽ (paths[i] == ശൂന്യ)
    തിരികെ false;

  വേണ്ടി (j = 0; j < i; j++)
    എങ്കിൽ (paths[j] && same_path (paths[j], paths[i]))
      തിരികെ true;

  തിരികെ false;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  പൂർണ്ണസംഖ്യ ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  എങ്കിൽ (! def || ! def->linux)
    തിരികെ 0;

  വേണ്ടി (i = 0; i < def->linux->masked_paths_len; i++)
    {
      എങ്കിൽ (is_duplicate_path (def->linux->masked_paths, i))
        തുടരുക;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  വേണ്ടി (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_pivot (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rootfs, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ oldrootfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ newrootfd = -1;
  പൂർണ്ണസംഖ്യ ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (oldrootfd < 0))
    തിരികെ crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (newrootfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_വേര് (".", ".");
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, ശൂന്യ, -1, ".", ശൂന്യ, MS_REC | MS_PRIVATE, ശൂന്യ, LABEL_MOUNT, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = umount2 (".", MNT_DETACH);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "umount2 oldroot");

  ചെയ്യുക
    {
      ret = umount2 (".", MNT_DETACH);
      എങ്കിൽ (ret < 0 && errno == EINVAL)
        നിർത്തുക;
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "umount2 oldroot");
  } ആകുമ്പോൾ (ret == 0);

  ret = chdir ("/");
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "chdir to newroot");

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, അക്ഷരം **data, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  ഘടന stat st;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (*data != ശൂന്യ && strstr (*data, "mode="))
    തിരികെ 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  എങ്കിൽ (fd < 0)
    {
      എങ്കിൽ (crun_error_get_errno (err) != ENOENT)
        തിരികെ fd;

      crun_error_release (err);
      തിരികെ 0;
    }
  ret = fstat (fd, &st);
  എങ്കിൽ (ret < 0)
    തിരികെ crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_default_flags (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *destination, അക്ഷരം **data)
{
  എങ്കിൽ (strcmp (destination, "/proc") == 0)
    തിരികെ 0;
  എങ്കിൽ (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      തിരികെ MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  എങ്കിൽ (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      തിരികെ MS_NOEXEC | MS_STRICTATIME;
    }
  എങ്കിൽ (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      തിരികെ MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  എങ്കിൽ (strcmp (destination, "/dev/mqueue") == 0)
    തിരികെ MS_NOEXEC | MS_NOSUID | MS_NODEV;
  എങ്കിൽ (strcmp (destination, "/dev/pts") == 0)
    {
      എങ്കിൽ (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      അല്ലെങ്കിൽ
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      തിരികെ MS_NOEXEC | MS_NOSUID;
    }
  എങ്കിൽ (strcmp (destination, "/sys") == 0)
    തിരികെ MS_NOEXEC | MS_NOSUID | MS_NODEV;

  തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  തിരികെ find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
check_valid_no_follow_file_system (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *destination, libcrun_error_t *err)
{
  ഘടന statfs sfs;

  പൂർണ്ണസംഖ്യ ret = fstatfs (fd, &sfs);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "statfs `%s`", destination);

  തിരഞ്ഞെടുപ്പ് (sfs.f_type)
    {
    അവസ്ഥ PROC_SUPER_MAGIC:
    അവസ്ഥ SYSFS_MAGIC:
    അവസ്ഥ DEVPTS_SUPER_MAGIC:
      തിരികെ crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
safe_create_symlink (പൂർണ്ണസംഖ്യ rootfsfd, സ്ഥിരം അക്ഷരം *rootfs, സ്ഥിരം അക്ഷരം *target, സ്ഥിരം അക്ഷരം *destination, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ parent_dir_fd = -1;
  cleanup_free അക്ഷരം *buffer = ശൂന്യ;
  അക്ഷരം *part;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (is_empty_string (destination))
    തിരികെ crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  എങ്കിൽ (UNLIKELY (parent_dir_fd < 0))
    തിരികെ parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      എങ്കിൽ (errno == EEXIST)
        {
          cleanup_free അക്ഷരം *link = ശൂന്യ;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          എങ്കിൽ (UNLIKELY (len < 0))
            തിരികെ len;

          എങ്കിൽ ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            തിരികെ 0;

          തിരികെ crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      തിരികെ crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
handle_copy_symlink (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     സ്ഥിരം അക്ഷരം *link_target, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *target = ശൂന്യ;

  എങ്കിൽ (link_target == ശൂന്യ)
    {
      പൂർണ്ണസംഖ്യ old_വേര്_fd = get_old_വേര്_fd (get_private_data (container));
      സ്ഥിരം അക്ഷരം *source = mount->source;
      ssize_t len;

      എങ്കിൽ (old_വേര്_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_വേര്_fd, source + 1, &target, 0, err);
      അല്ലെങ്കിൽ
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      എങ്കിൽ (UNLIKELY (len < 0))
        തിരികെ len;
      link_target = target;
    }

  തിരികെ safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
prepare_sysfs_or_proc_mount (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *target,
                             പൂർണ്ണസംഖ്യ *targetfd, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno == ENOENT)
        {
          എങ്കിൽ (strchr (target, '/'))
            തിരികെ crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (errno == ENOTDIR)
        തിരികെ crun_make_error (err, errno, "the target `/%s` is invalid", target);

      എങ്കിൽ (ret < 0)
        തിരികെ crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
handle_tmpcopyup (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rootfs, സ്ഥിരം അക്ഷരം *target,
                  പൂർണ്ണസംഖ്യ copy_from_fd, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ destfd, ret;
  cleanup_close പൂർണ്ണസംഖ്യ tmpfd = copy_from_fd;

  target = consume_slashes (target);

  എങ്കിൽ (is_empty_string (target))
    തിരികെ crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  എങ്കിൽ (UNLIKELY (destfd < 0))
    തിരികെ crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_mount_label_how (സ്ഥിരം അക്ഷരം *type, bool is_sysfs_or_proc)
{
  എങ്കിൽ (is_sysfs_or_proc)
    തിരികെ LABEL_NONE;

  എങ്കിൽ (strcmp (type, "mqueue") == 0)
    തിരികെ LABEL_XATTR;

  തിരികെ LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
സ്ഥിതം unsigned long
get_bind_mount_locked_flags (പൂർണ്ണസംഖ്യ source_mountfd, സ്ഥിരം അക്ഷരം *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  ഘടന statfs sfs;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  അല്ലെങ്കിൽ
    ret = statfs (source, &sfs);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ 0;

  വേണ്ടി (i = 0; i < mount->options_len; i++)
    {
      സ്ഥിരം ഘടന propagation_flags_s *prop;

      എങ്കിൽ (mount->options[i] == ശൂന്യ)
        തുടരുക;

      prop = libcrun_str2mount_flags (mount->options[i]);
      എങ്കിൽ (prop == ശൂന്യ || (prop->extra_flags & OPTION_RECURSIVE))
        തുടരുക;

      എങ്കിൽ (prop->clear)
        cleared |= prop->flags;
      അല്ലെങ്കിൽ
        cleared &= ~prop->flags;
    }

  തിരികെ sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
സ്ഥിതം സ്ഥിരം അക്ഷരം *
get_path_in_rootfs (സ്ഥിരം അക്ഷരം *rootfs, സ്ഥിരം അക്ഷരം *path)
{
  size_t len;

  എങ്കിൽ (rootfs == ശൂന്യ || path == ശൂന്യ)
    തിരികെ ശൂന്യ;

  len = strlen (rootfs);
  ആകുമ്പോൾ (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  എങ്കിൽ (len == 0)
    തിരികെ ശൂന്യ;

  എങ്കിൽ (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    തിരികെ ശൂന്യ;

  തിരികെ consume_slashes (path + len);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
process_single_mount (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      ഘടന libcrun_fd_map *mount_fds, size_t mount_index,
                      സ്ഥിരം അക്ഷരം *systemd_cgroup_v1, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *target = consume_slashes (mount->destination);
  cleanup_close പൂർണ്ണസംഖ്യ source_mountfd = -1;
  cleanup_free അക്ഷരം *data = ശൂന്യ;
  അക്ഷരം *type;
  അക്ഷരം *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close പൂർണ്ണസംഖ്യ copy_from_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  പൂർണ്ണസംഖ്യ no_remount = 0;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  എങ്കിൽ (mount->options == ശൂന്യ)
    flags = get_default_flags (container, mount->destination, &data);
  അല്ലെങ്കിൽ
    {
      size_t j;

      വേണ്ടി (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  എങ്കിൽ (type == ശൂന്യ && (flags & MS_BIND) == 0)
    തിരികെ crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  എങ്കിൽ ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      സ്ഥിരം അക്ഷരം *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      എങ്കിൽ (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          എങ്കിൽ (UNLIKELY (source_mountfd < 0))
            തിരികെ crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  എങ്കിൽ ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  എങ്കിൽ (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      എങ്കിൽ (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      എങ്കിൽ (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  എങ്കിൽ (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      സ്ഥിരം അക്ഷരം *path = mount->source;

      എങ്കിൽ ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        തിരികെ crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      എങ്കിൽ (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, ശൂന്യ);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (path[0] == '/')
        {
          പൂർണ്ണസംഖ്യ old_വേര്_fd = get_old_വേര്_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          എങ്കിൽ (old_വേര്_fd >= 0)
            ret = get_file_type_at (old_വേര്_fd, &src_mode, nofollow, (path[1] == '\0') ? ശൂന്യ : path + 1);
          അല്ലെങ്കിൽ
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      അല്ലെങ്കിൽ
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "cannot stat `%s`", path);

      എങ്കിൽ (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          പൂർണ്ണസംഖ്യ bind_dirfd = AT_FDCWD;
          സ്ഥിരം അക്ഷരം *bind_source = mount->source;
          പൂർണ്ണസംഖ്യ old_വേര്_fd = get_old_വേര്_fd (get_private_data (container));

          എങ്കിൽ (old_വേര്_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_വേര്_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          source_mountfd = ret;
        }
    }

  എങ്കിൽ (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      സ്ഥിരം അക്ഷരം *cached_target = ശൂന്യ;

      എങ്കിൽ (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      mounted = true;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      bool is_dir = S_ISDIR (src_mode);

      എങ്കിൽ (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
      അല്ലെങ്കിൽ
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          targetfd = ret;
        }
    }

  എങ്കിൽ (extra_flags & OPTION_TMPCOPYUP)
    {
      എങ്കിൽ (strcmp (type, "tmpfs") != 0)
        തിരികെ crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      എങ്കിൽ (UNLIKELY (copy_from_fd < 0))
        {
          എങ്കിൽ (errno != ENOTDIR)
            തിരികെ crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  എങ്കിൽ (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, ശൂന്യ);
      എങ്കിൽ (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          എങ്കിൽ (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, ശൂന്യ, source_mountfd, target, ശൂന്യ, remaining_flags, data, LABEL_NONE | no_remount, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;
            }

          mounted = true;

          എങ്കിൽ (is_empty_string (target))
            {
              പൂർണ്ണസംഖ്യ new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              എങ്കിൽ (UNLIKELY (new_rootfsfd < 0))
                തിരികെ crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      അല്ലെങ്കിൽ എങ്കിൽ (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          തിരികെ crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (get_private_data (container)->host_വേര്_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          തിരികെ crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  എങ്കിൽ (! mounted)
    {
      എങ്കിൽ (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
      അല്ലെങ്കിൽ
        {
          പൂർണ്ണസംഖ്യ extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }

  എങ്കിൽ (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (rec_clear || rec_set)
    {
      സ്ഥിരം bool is_dir = S_ISDIR (src_mode);
      cleanup_close പൂർണ്ണസംഖ്യ dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      എങ്കിൽ (UNLIKELY (dfd < 0))
        തിരികെ crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_mounts (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  സ്ഥിരം അക്ഷരം *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map ഘടന libcrun_fd_map *mount_fds = ശൂന്യ;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = ശൂന്യ;

  വേണ്ടി (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
പൂർണ്ണസംഖ്യ
libcrun_container_do_bind_mount (libcrun_container_t *container, അക്ഷരം *mount_source, അക്ഷരം *mount_destination, അക്ഷരം **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *target = consume_slashes (mount_destination);
  cleanup_free അക്ഷരം *data = ശൂന്യ;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close പൂർണ്ണസംഖ്യ targetfd = -1;
  പൂർണ്ണസംഖ്യ is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  സ്ഥിരം അക്ഷരം *rootfs = get_private_data (container)->rootfs;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ ((get_private_data (container)->rootfsfd < 0) || (rootfs == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  എങ്കിൽ (mount_options == ശൂന്യ)
    flags = get_default_flags (container, mount_destination, &data);
  അല്ലെങ്കിൽ
    {
      size_t j;

      വേണ്ടി (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  എങ്കിൽ (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  എങ്കിൽ (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      എങ്കിൽ (UNLIKELY (is_dir < 0))
        തിരികെ is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
പൂർണ്ണസംഖ്യ
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, പൂർണ്ണസംഖ്യ *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close പൂർണ്ണസംഖ്യ notify_fd = -1;
  cleanup_free അക്ഷരം *host_notify_socket_path = ശൂന്യ;
  cleanup_free അക്ഷരം *state_dir = ശൂന്യ;
  അക്ഷരം *host_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (container && get_private_data (container)->host_notify_socket_path)
    {
      സ്ഥിരം അക്ഷരം *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  എങ്കിൽ (host_path == ശൂന്യ)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_വേര്, context->id, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  എങ്കിൽ (UNLIKELY (notify_fd < 0))
    തിരികെ notify_fd;

  എങ്കിൽ (UNLIKELY (chmod (host_path, 0777) < 0))
    തിരികെ crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  എങ്കിൽ (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (ശൂന്യം) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  തിരികെ 1;
#അല്ലെങ്കിൽ
  (ശൂന്യം) context;
  (ശൂന്യം) container;
  (ശൂന്യം) err;
  *notify_socket_out = -1;
  തിരികെ 0;
#endif
}

#ifdef HAVE_SYSTEMD
സ്ഥിതം പൂർണ്ണസംഖ്യ
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  സ്ഥിരം അക്ഷരം *notify_socket = container->context->notify_socket;
  cleanup_free അക്ഷരം *host_notify_socket_path = ശൂന്യ;
  cleanup_free അക്ഷരം *container_notify_socket_path = ശൂന്യ;
  cleanup_free അക്ഷരം *state_dir = ശൂന്യ;
  uid_t container_വേര്_uid = -1;
  gid_t container_വേര്_gid = -1;
  പൂർണ്ണസംഖ്യ notify_socket_വൃക്ഷം_fd;

  എങ്കിൽ (notify_socket == ശൂന്യ)
    തിരികെ 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_വേര് : ശൂന്യ),
                                     (container->context ? container->context->id : ശൂന്യ), err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = mkdir (host_notify_socket_path, 0700);
  എങ്കിൽ (ret < 0)
    തിരികെ crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  എങ്കിൽ (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_വേര്_in_the_userns (container->container_def, 0, 0, &container_വേര്_uid, &container_വേര്_gid);
      എങ്കിൽ (container_വേര്_uid != ((uid_t) -1) && container_വേര്_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_വേര്_uid, container_വേര്_gid);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_വേര്_uid, container_വേര്_gid);
        }
    }

  notify_socket_വൃക്ഷം_fd = syscall_open_വൃക്ഷം (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  എങ്കിൽ (notify_socket_വൃക്ഷം_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_വൃക്ഷം_fd = notify_socket_വൃക്ഷം_fd;
  അല്ലെങ്കിൽ എങ്കിൽ (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  അല്ലെങ്കിൽ എങ്കിൽ (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  അല്ലെങ്കിൽ
    /* some other error */
    തിരികെ crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = ശൂന്യ;
  തിരികെ 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
do_finalize_notify_socket (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rootfs, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free അക്ഷരം *host_notify_socket_path = ശൂന്യ;
  cleanup_free അക്ഷരം *container_notify_socket_path = ശൂന്യ;
  cleanup_free അക്ഷരം *container_notify_socket_path_dir_alloc = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *container_notify_socket_path_dir = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ notify_socket_വൃക്ഷം_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ targetfd = -1;
  പൂർണ്ണസംഖ്യ did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = ശൂന്യ;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = ശൂന്യ;

  എങ്കിൽ (host_notify_socket_path == ശൂന്യ || container_notify_socket_path == ശൂന്യ)
    തിരികെ 0;

  എങ്കിൽ (rootfs == ശൂന്യ)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  എങ്കിൽ (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  targetfd = ret;

  notify_socket_വൃക്ഷം_fd = get_private_data (container)->notify_socket_വൃക്ഷം_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_വൃക്ഷം_fd = -1;

  എങ്കിൽ (notify_socket_വൃക്ഷം_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_വൃക്ഷം_fd, targetfd, ശൂന്യ);
      എങ്കിൽ (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          എങ്കിൽ (UNLIKELY (targetfd < 0))
            തിരികെ targetfd;

          ret = do_mount (container, ശൂന്യ, targetfd, container_notify_socket_path_dir, ശൂന്യ,
                          MS_REC | MS_PRIVATE, ശൂന്യ, LABEL_NONE, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      അല്ലെങ്കിൽ
        തിരികെ crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_വൃക്ഷം_fd,
                                container_notify_socket_path_dir);
    }

  എങ്കിൽ (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, ശൂന്യ,
                      MS_BIND | MS_REC | MS_PRIVATE, ശൂന്യ, LABEL_MOUNT, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
make_parent_mount_private (സ്ഥിരം അക്ഷരം *rootfs, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ rootfsfd = -1;
  ഘടന stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (rootfsfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", rootfs);

  എങ്കിൽ (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    തിരികെ crun_make_error (err, errno, "fstat `%s`", rootfs);

  വേണ്ടി (;;)
    {
      ഘടന stat cur_st;
      libcrun_error_t tmp_err = ശൂന്യ;
      പൂർണ്ണസംഖ്യ ret;
      cleanup_close പൂർണ്ണസംഖ്യ parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      എങ്കിൽ (ret == 0)
        തിരികെ 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (ശൂന്യ, proc_path, ശൂന്യ, MS_PRIVATE, ശൂന്യ);
      എങ്കിൽ (ret == 0)
        തിരികെ 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      എങ്കിൽ (parentfd < 0)
        {
          പൂർണ്ണസംഖ്യ saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          എങ്കിൽ (ret != 0)
            തിരികെ crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          തിരികെ crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      എങ്കിൽ (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        തിരികെ crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      എങ്കിൽ (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        നിർത്തുക;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  തിരികെ crun_make_error (err, 0, "make `%s` private", rootfs);
}

പൂർണ്ണസംഖ്യ
libcrun_set_mounts (ഘടന container_entrypoint_s *entrypoint_args, libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rootfs, set_mounts_cb_t cb, ശൂന്യം *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  പൂർണ്ണസംഖ്യ is_user_ns = 0;
  പൂർണ്ണസംഖ്യ ret = 0;

  എങ്കിൽ (rootfs == ശൂന്യ || def->mounts == ശൂന്യ)
    തിരികെ 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  എങ്കിൽ (def->വേര്->readonly)
    {
      ഘടന remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      പൂർണ്ണസംഖ്യ fd;

      fd = dup (get_private_data (container)->rootfsfd);
      എങ്കിൽ (UNLIKELY (fd < 0))
        തിരികെ crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, ശൂന്യ, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  എങ്കിൽ (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      എങ്കിൽ (UNLIKELY (is_user_ns < 0))
        തിരികെ is_user_ns;
    }

  എങ്കിൽ (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  എങ്കിൽ (cb)
    {
      ret = cb (cb_data, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      സ്ഥിരം അക്ഷരം *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (ശൂന്യം) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
umount_or_hide (സ്ഥിരം അക്ഷരം *target, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = umount2 (target, MNT_DETACH);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      പൂർണ്ണസംഖ്യ saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      എങ്കിൽ (errno == EINVAL)
        {
          ret = mount (ശൂന്യ, target, "tmpfs", 0, "size=0k");
          എങ്കിൽ (LIKELY (ret == 0))
            തിരികെ 0;
        }

      തിരികെ crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
move_വേര് (സ്ഥിരം അക്ഷരം *rootfs, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = chdir (rootfs);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = umount_or_hide ("/proc", err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_finalize_mounts (ഘടന container_entrypoint_s *entrypoint_args, libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rootfs,
                         set_mounts_cb_t after_pivot_cb, ശൂന്യം *cb_data, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_വേര്_fd));

  എങ്കിൽ (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      എങ്കിൽ (get_private_data (container)->no_pivot)
        {
          ret = move_വേര് (rootfs, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
      അല്ലെങ്കിൽ
        {
          ret = do_pivot (container, rootfs, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      ret = do_mount (container, ശൂന്യ, -1, "/", ശൂന്യ,
                      get_private_data (container)->rootfs_propagation,
                      ശൂന്യ, LABEL_MOUNT, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = chdir ("/");
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "chdir to `/`");
    }

  എങ്കിൽ (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = finalize_mounts (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
maybe_open_വൃക്ഷം_namespace (സ്ഥിരം അക്ഷരം *rootfs, പൂർണ്ണസംഖ്യ *out_fd, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ rootfs_fd = -1;
  പൂർണ്ണസംഖ്യ വൃക്ഷം_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (rootfs_fd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", rootfs);

  വൃക്ഷം_fd = syscall_open_വൃക്ഷം (rootfs_fd, "",
                               OPEN_വൃക്ഷം_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  എങ്കിൽ (വൃക്ഷം_fd < 0)
    {
      എങ്കിൽ (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        തിരികെ 0;
      തിരികെ crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = വൃക്ഷം_fd;
  തിരികെ 0;
}

സ്ഥിതം ഘടന libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  ഘടന libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  എങ്കിൽ (mount_fds == ശൂന്യ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  തിരികെ mount_fds;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free അക്ഷരം *mnt_data = ശൂന്യ;
  unsigned long mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  സ്ഥിരം അക്ഷരം *label_type = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *label_val = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *fstype;
  പൂർണ്ണസംഖ്യ mnt_fd;
  size_t j;

  എങ്കിൽ (mount->type == ശൂന്യ)
    തിരികെ -1;

  fstype = mount->type;

  വേണ്ടി (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  എങ്കിൽ (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      പൂർണ്ണസംഖ്യ extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      എങ്കിൽ (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          എങ്കിൽ (label_type == ശൂന്യ)
            crun_error_release (err);
          അല്ലെങ്കിൽ
            label_val = def->linux->mount_label;
        }
    }

  എങ്കിൽ (strcmp (fstype, "cgroup") == 0)
    {
      പൂർണ്ണസംഖ്യ cgroup_mode = libcrun_get_cgroup_mode (err);
      എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  എങ്കിൽ (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, ശൂന്യ, ശൂന്യ, mnt_data);

  തിരികെ mnt_fd;
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
ഘടന mnt_id_req_s
{
  uint32_t size;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

ഘടന statmount_s
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
  അക്ഷരം str[];
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
syscall_listmount (ഘടന mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned പൂർണ്ണസംഖ്യ flags)
{
#ifdef __NR_listmount
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_listmount, req, ids, nr, flags);
#അല്ലെങ്കിൽ
  (ശൂന്യം) req;
  (ശൂന്യം) ids;
  (ശൂന്യം) nr;
  (ശൂന്യം) flags;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
syscall_statmount (ഘടന mnt_id_req_s *req, ഘടന statmount_s *buf, size_t size, unsigned പൂർണ്ണസംഖ്യ flags)
{
#ifdef __NR_statmount
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_statmount, req, buf, size, flags);
#അല്ലെങ്കിൽ
  (ശൂന്യം) req;
  (ശൂന്യം) buf;
  (ശൂന്യം) size;
  (ശൂന്യം) flags;
  errno = ENOSYS;
  തിരികെ -1;
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
സ്ഥിതം bool
mounts_hit_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = ശൂന്യ;
  size_t i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  വേണ്ടി (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      എങ്കിൽ (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != ശൂന്യ && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          നിർത്തുക;
        }
    }
  എങ്കിൽ (! has_absolute_bind)
    തിരികെ false;

  ids = xmalloc (capacity * വലുപ്പം (uint64_t));
  വേണ്ടി (;;)
    {
      ഘടന mnt_id_req_s req = {
        .size = വലുപ്പം (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      പൂർണ്ണസംഖ്യ ret;

      എങ്കിൽ (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * വലുപ്പം (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      എങ്കിൽ (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        തിരികെ true;
      എങ്കിൽ (ret == 0)
        നിർത്തുക;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  വേണ്ടി (i = 0; i < n_ids; i++)
    {
      അക്ഷരം buffer[4096] __attribute__ ((aligned (8)));
      ഘടന statmount_s *sm = (ഘടന statmount_s *) buffer;
      ഘടന mnt_id_req_s req = {
        .size = വലുപ്പം (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      സ്ഥിരം അക്ഷരം *വേര്, *point;
      size_t j;

      എങ്കിൽ (syscall_statmount (&req, sm, വലുപ്പം (buffer), 0) < 0)
        തുടരുക;
      എങ്കിൽ ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        തുടരുക;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      വേര് = sm->str + sm->mnt_root;
      എങ്കിൽ (! has_prefix (വേര്, "mnt:["))
        തുടരുക;

      point = sm->str + sm->mnt_point;

      വേണ്ടി (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          സ്ഥിരം അക്ഷരം *source = def->mounts[j]->source;
          size_t source_len;

          എങ്കിൽ (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            തുടരുക;
          എങ്കിൽ (source == ശൂന്യ || source[0] != '/')
            തുടരുക;

          source_len = strlen (source);
          ആകുമ്പോൾ (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          എങ്കിൽ (strncmp (point, source, source_len) != 0)
            തുടരുക;

          /* The source itself is a pinned mount namespace.  */
          എങ്കിൽ (point[source_len] == '\0')
            തിരികെ true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          എങ്കിൽ (recursive && (source_len == 1 || point[source_len] == '/'))
            തിരികെ true;
        }
    }

  തിരികെ false;
}

സ്ഥിതം bool
can_use_open_വൃക്ഷം_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ഘടന libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  എങ്കിൽ (has_hooks || has_userns)
    തിരികെ false;

  എങ്കിൽ (mounts_hit_pinned_mount_namespace (def))
    തിരികെ false;

  എങ്കിൽ (container->context->no_open_വൃക്ഷം_namespace)
    തിരികെ false;

  {
    libcrun_error_t tmp_err = ശൂന്യ;
    പൂർണ്ണസംഖ്യ in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    എങ്കിൽ (in_userns != 0)
      തിരികെ false;
  }

  mount_fds = get_fd_map (container);
  വേണ്ടി (i = 0; i < def->mounts_len; i++)
    {
      എങ്കിൽ (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          എങ്കിൽ (! has_cached_target)
            തിരികെ false;
        }
    }

  തിരികെ true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
സ്ഥിതം bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  എങ്കിൽ (mount->type == ശൂന്യ || strcmp (mount->type, "tmpfs") != 0)
    തിരികെ false;

  വേണ്ടി (i = 0; i < mount->options_len; i++)
    എങ്കിൽ (mount->options[i] != ശൂന്യ && has_prefix (mount->options[i], "mode="))
      തിരികെ false;

  തിരികെ true;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, അക്ഷരം **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close പൂർണ്ണസംഖ്യ rootfsfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ വൃക്ഷം_fd = -1;
  libcrun_error_t tmp_err = ശൂന്യ;
  bool use_open_വൃക്ഷം = false;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, ശൂന്യ, ശൂന്യ, ശൂന്യ, ശൂന്യ);

  എങ്കിൽ ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  സ്വതന്ത്രം (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  വേണ്ടി (i = 0; i < def->mounts_len; i++)
    {
      ഘടന libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close പൂർണ്ണസംഖ്യ mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      എങ്കിൽ (mount_fds && mount_fds->fds[i] >= 0)
        തുടരുക;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      എങ്കിൽ (def->mounts[i]->source != ശൂന്യ
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free അക്ഷരം *mnt_data = ശൂന്യ;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          വേണ്ടി (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          എങ്കിൽ (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free അക്ഷരം *target = ശൂന്യ;
              libcrun_error_t tmp_err = ശൂന്യ;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              എങ്കിൽ (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              എങ്കിൽ (get_private_data (container)->copy_symlink_targets == ശൂന്യ)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * വലുപ്പം (അക്ഷരം *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = ശൂന്യ;
            }
        }

      എങ്കിൽ (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          എങ്കിൽ (mount_fds && def->mounts[i]->source != ശൂന്യ && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == ശൂന്യ)
            {
              libcrun_error_t tmp_err = ശൂന്യ;
              unsigned long propagation = MS_PRIVATE;

              എങ്കിൽ (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              എങ്കിൽ (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          തുടരുക;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      എങ്കിൽ (mnt_fd < 0)
        {
          crun_error_release (err);
          തുടരുക;
        }

      എങ്കിൽ (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close പൂർണ്ണസംഖ്യ destfd = -1;
          ഘടന stat st;

          എങ്കിൽ (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              എങ്കിൽ (UNLIKELY (rootfsfd < 0))
                തിരികെ crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          എങ്കിൽ (destfd < 0)
            crun_error_release (&tmp_err);
          അല്ലെങ്കിൽ എങ്കിൽ (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              പൂർണ്ണസംഖ്യ procfd = get_procfd (get_private_data (container), err);
              എങ്കിൽ (UNLIKELY (procfd < 0))
                തിരികെ procfd;

              get_self_fd_path (fd_path, mnt_fd);
              ret = fchmodat (procfd, fd_path, st.st_mode & 07777, 0);
              (ശൂന്യം) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_empty_dir_cached (container, ശൂന്യ, &tmp_err);
  എങ്കിൽ (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  എങ്കിൽ (! get_private_data (container)->joined_mount_ns
      && can_use_open_വൃക്ഷം_namespace (container))
    use_open_വൃക്ഷം = true;

  എങ്കിൽ (use_open_വൃക്ഷം)
    {
      ret = maybe_open_വൃക്ഷം_namespace (*rootfs, &വൃക്ഷം_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  എങ്കിൽ (വൃക്ഷം_fd >= 0)
    preopen_needed_devs_and_notify (container);

  എങ്കിൽ (വൃക്ഷം_fd >= 0)
    {
      ret = setns (വൃക്ഷം_fd, CLONE_NEWNS);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (ശൂന്യ, "/", ശൂന്യ, MS_REMOUNT | MS_BIND, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, ശൂന്യ, -1, "/", ശൂന്യ, MS_REC | MS_PRIVATE, ശൂന്യ, LABEL_MOUNT, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = do_mount (container, ശൂന്യ, -1, "/", ശൂന്യ, rootfs_propagation, ശൂന്യ, LABEL_MOUNT, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_വേര്_switched = true;
      സ്വതന്ത്രം (*rootfs);
      *rootfs = xstrdup ("/");
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, ശൂന്യ, -1, "/", ശൂന്യ, rootfs_propagation, ശൂന്യ, LABEL_MOUNT, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = make_parent_mount_private (*rootfs, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, ശൂന്യ, MS_BIND | MS_REC | MS_PRIVATE, ശൂന്യ, LABEL_MOUNT, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  എങ്കിൽ (! container->context->force_no_cgroup)
    {
      പൂർണ്ണസംഖ്യ cgroup_mode = libcrun_get_cgroup_mode (err);
      എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
        തിരികെ cgroup_mode;

      എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          അക്ഷരം *unified_cgroup_path = ശൂന്യ;
          പൂർണ്ണസംഖ്യ procfd = get_procfd (get_private_data (container), err);
          എങ്കിൽ (UNLIKELY (procfd < 0))
            തിരികെ procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = ശൂന്യ;
      തിരികെ ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = ശൂന്യ;

  എങ്കിൽ (വൃക്ഷം_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  എങ്കിൽ (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_do_pivot_വേര് (libcrun_container_t *container, bool no_pivot, അക്ഷരം **rootfs, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (*rootfs == ശൂന്യ)
    തിരികെ 0;

  എങ്കിൽ (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      എങ്കിൽ (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "chdir to `/`");
        }
    }
  അല്ലെങ്കിൽ
    {
      ret = chroot (*rootfs);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "chdir to `/`");

      സ്വതന്ത്രം (*rootfs);
      *rootfs = xstrdup ("/");
    }

  തിരികെ 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
പൂർണ്ണസംഖ്യ
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  ഘടന stat statbuf;
  cleanup_close പൂർണ്ണസംഖ്യ fd;
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  എങ്കിൽ (UNLIKELY (fd == -1))
    തിരികെ crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  വേണ്ടി (i = 0; i <= 2; i++)
    {
      എങ്കിൽ (UNLIKELY (fstat (i, &statbuf) == -1))
        തിരികെ crun_make_error (err, errno, "stat fd `%d`", i);
      എങ്കിൽ (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          എങ്കിൽ (UNLIKELY (dup2 (fd, i) == -1))
            തിരികെ crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
uidgidmap_helper (അക്ഷരം *helper, pid_t pid, സ്ഥിരം അക്ഷരം *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  അക്ഷരം pid_fmt[16];
  അക്ഷരം *args[MAX_ARGS + 1];
  അക്ഷരം *next;
  cleanup_free അക്ഷരം *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  പൂർണ്ണസംഖ്യ ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, വലുപ്പം (pid_fmt), "%d", pid);
  എങ്കിൽ (UNLIKELY (ret >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (pid_fmt)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  ആകുമ്പോൾ (nargs < MAX_ARGS)
    {
      അക്ഷരം *p = strsep (&next, " \n");
      എങ്കിൽ (next == ശൂന്യ)
        നിർത്തുക;
      args[nargs++] = p;
    }
  args[nargs++] = ശൂന്യ;

  ret = run_process (args, err);
  എങ്കിൽ (ret < 0)
    തിരികെ ret;
  എങ്കിൽ (ret > 0)
    തിരികെ crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
newgidmap (pid_t pid, സ്ഥിരം അക്ഷരം *map_file, libcrun_error_t *err)
{
  തിരികെ uidgidmap_helper ("newgidmap", pid, map_file, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
newuidmap (pid_t pid, സ്ഥിരം അക്ഷരം *map_file, libcrun_error_t *err)
{
  തിരികെ uidgidmap_helper ("newuidmap", pid, map_file, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ fd;

  ret = TEMP_FAILURE_RETRY (ലിഖ (fd, "deny", 4));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free അക്ഷരം *content = ശൂന്യ;

  എങ്കിൽ (get_private_data (container)->deny_setgroups)
    തിരികെ 0;

  എങ്കിൽ (container->container_def->annotations)
    {
      സ്ഥിരം അക്ഷരം *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      എങ്കിൽ (annotation)
        തിരികെ strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    എങ്കിൽ (fd < 0)
      ret = fd;
    അല്ലെങ്കിൽ
      ret = read_all_fd (fd, "self/setgroups", &content, ശൂന്യ, err);
  }
  എങ്കിൽ (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          തിരികെ 1;
        }
      തിരികെ ret;
    }

  തിരികെ strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

പൂർണ്ണസംഖ്യ
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = ശൂന്യ;
  size_t additional_gids_len = 0;
  പൂർണ്ണസംഖ്യ can_do_setgroups;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (process != ശൂന്യ && process->user != ശൂന്യ)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  എങ്കിൽ (UNLIKELY (can_do_setgroups < 0))
    തിരികെ can_do_setgroups;

  എങ്കിൽ (can_do_setgroups == 0)
    {
      സ്ഥിരം അക്ഷരം *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      എങ്കിൽ (annotation && strcmp (annotation, "0") != 0)
        തിരികെ 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      വേണ്ടി (i = 0; i < additional_gids_len; i++)
        എങ്കിൽ (additional_gids[i] != process->user->gid)
          തിരികെ crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (പൂർണ്ണസംഖ്യ) additional_gids[i]);
      തിരികെ 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "setgroups");

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  എങ്കിൽ (get_private_data (container)->unshare_cgroupns)
    {
      പൂർണ്ണസംഖ്യ ret = unshare (CLONE_NEWCGROUP);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (errno != EINVAL)
            തിരികെ crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  തിരികെ 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
പൂർണ്ണസംഖ്യ
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  എങ്കിൽ (len != 1)
    തിരികെ 0;

  എങ്കിൽ (mappings[0]->size != 1)
    തിരികെ 0;

  എങ്കിൽ (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    തിരികെ 0;

  തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_id_map (libcrun_container_t *container, pid_t pid, സ്ഥിരം അക്ഷരം *map_name,
              പൂർണ്ണസംഖ്യ (*idmap_helper) (pid_t, സ്ഥിരം അക്ഷരം *, libcrun_error_t *), സ്ഥിരം അക്ഷരം *helper_name,
              bool warn_on_failure, സ്ഥിരം അക്ഷരം *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret = 0;

  എങ്കിൽ (container->host_uid)
    ret = idmap_helper (pid, map, err);
  എങ്കിൽ (container->host_uid == 0 || ret < 0)
    {
      cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

      എങ്കിൽ (ret < 0)
        {
          എങ്കിൽ (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      എങ്കിൽ (UNLIKELY (fd < 0))
        തിരികെ fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      എങ്കിൽ (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free അക്ഷരം *single_mapping = ശൂന്യ;
          crun_error_release (err);

          എങ്കിൽ (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          എങ്കിൽ (UNLIKELY (fd < 0))
            തിരികെ fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *uid_map = ശൂന്യ;
  cleanup_free അക്ഷരം *gid_map = ശൂന്യ;
  size_t uid_map_len = 0, gid_map_len = 0;
  പൂർണ്ണസംഖ്യ ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  എങ്കിൽ ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    തിരികെ 0;

  എങ്കിൽ (! def || ! def->linux)
    തിരികെ 0;

  എങ്കിൽ (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      uid_map_len = (size_t) ret;
      എങ്കിൽ (uid_map == ശൂന്യ)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }

  എങ്കിൽ (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      gid_map_len = (size_t) ret;
      എങ്കിൽ (gid_map == ശൂന്യ)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

ഘടന all_caps_s
{
  unsigned long effective[2];
  unsigned long permitted[2];
  unsigned long inheritable[2];
  unsigned long ambient[2];
  unsigned long bounding[2];
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
has_cap_on (പൂർണ്ണസംഖ്യ cap, long unsigned *caps)
{
  എങ്കിൽ (cap < 32)
    തിരികെ CAP_TO_MASK_0 (cap) & caps[0];
  തിരികെ (CAP_TO_MASK_1 (cap) & caps[1]);
}

സ്ഥിതം unsigned long cap_last_cap;

പൂർണ്ണസംഖ്യ
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  പൂർണ്ണസംഖ്യ ret;
  അക്ഷരം buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  എങ്കിൽ (fd < 0)
    തിരികെ fd;
  ret = TEMP_FAILURE_RETRY (പഠ (fd, buffer, വലുപ്പം (buffer) - 1));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, ശൂന്യ, 10);
  എങ്കിൽ (errno != 0)
    തിരികെ crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
set_required_caps (ഘടന all_caps_s *caps, uid_t uid, gid_t gid, പൂർണ്ണസംഖ്യ no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  പൂർണ്ണസംഖ്യ ret;
  ഘടന __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  ഘടന __user_cap_data_struct data[2] = { { 0 } };

  എങ്കിൽ (cap_last_cap == 0)
    തിരികെ crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  വേണ്ടി (cap = 0; cap <= cap_last_cap; cap++)
    എങ്കിൽ (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        എങ്കിൽ (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          തിരികെ crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  എങ്കിൽ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    തിരികെ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  എങ്കിൽ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    തിരികെ crun_make_error (err, errno, "prctl reset ambient");

  വേണ്ടി (cap = 0; cap <= cap_last_cap; cap++)
    എങ്കിൽ (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        എങ്കിൽ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          തിരികെ crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  എങ്കിൽ (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "prctl reset KEEPCAPS");

      എങ്കിൽ (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          വേണ്ടി (cap = 0; cap <= cap_last_cap; cap++)
            എങ്കിൽ (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "capset");
        }
    }
#endif

  എങ്കിൽ (no_new_privs)
    എങ്കിൽ (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      തിരികെ crun_make_error (err, errno, "prctl set no new privs");

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_caps (unsigned long caps[2], അക്ഷരം **മൂല്യങ്ങൾ, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  വേണ്ടി (i = 0; i < len; i++)
    {
      cap_മൂല്യം_t cap;
      എങ്കിൽ (cap_from_name (മൂല്യങ്ങൾ[i], &cap) < 0)
        തിരികെ crun_make_error (err, 0, "unknown capability `%s`", മൂല്യങ്ങൾ[i]);

      എങ്കിൽ (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      അല്ലെങ്കിൽ
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#അല്ലെങ്കിൽ
  caps[0] = 0;
  caps[1] = 0;
#endif
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  എങ്കിൽ (proc->selinux_label)
    തിരികെ set_selinux_label (container, proc->selinux_label, now, err);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  എങ്കിൽ (proc->apparmor_profile)
    തിരികെ set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  പൂർണ്ണസംഖ്യ no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  ഘടന all_caps_s caps = {};
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

ഘടന rlimit_s
{
  സ്ഥിരം അക്ഷരം *name;
  പൂർണ്ണസംഖ്യ മൂല്യം;
};

ഘടന rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
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
                              { "RLIMIT_STACK", RLIMIT_അട്ടി },
                              { ശൂന്യ, 0 } };

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_rlimit_resource (സ്ഥിരം അക്ഷരം *name)
{
  ഘടന rlimit_s *it;
  വേണ്ടി (it = rlimits; it->name; it++)
    എങ്കിൽ (strcmp (it->name, name) == 0)
      തിരികെ it->മൂല്യം;
  തിരികെ -1;
}

പൂർണ്ണസംഖ്യ
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  വേണ്ടി (i = 0; i < len; i++)
    {
      വേണ്ടി (j = i + 1; j < len; j++)
        {
          എങ്കിൽ (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            തിരികെ crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  വേണ്ടി (i = 0; i < len; i++)
    {
      ഘടന rlimit limit;
      അക്ഷരം *type = new_rlimits[i]->type;
      പൂർണ്ണസംഖ്യ resource = get_rlimit_resource (type);
      എങ്കിൽ (UNLIKELY (resource < 0))
        തിരികെ crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      എങ്കിൽ (UNLIKELY (setrlimit (resource, &limit) < 0))
        തിരികെ crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  പൂർണ്ണസംഖ്യ has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  പൂർണ്ണസംഖ്യ ret;
  എങ്കിൽ (def->hostname == ശൂന്യ || def->hostname[0] == '\0')
    തിരികെ 0;
  എങ്കിൽ (! has_uts)
    തിരികെ crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "sethostname");
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  പൂർണ്ണസംഖ്യ has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  പൂർണ്ണസംഖ്യ ret;
  എങ്കിൽ (is_empty_string (def->domainname))
    തിരികെ 0;
  എങ്കിൽ (! has_uts)
    തിരികെ crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "setdomainname");
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  പൂർണ്ണസംഖ്യ ret;
  അക്ഷരം oom_buffer[16];
  എങ്കിൽ (def->process == ശൂന്യ || ! def->process->oom_score_adj_present)
    തിരികെ 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, വലുപ്പം (oom_buffer), "%i", def->process->oom_score_adj);
  എങ്കിൽ (UNLIKELY (ret >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (oom_buffer)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  എങ്കിൽ (fd < 0)
    തിരികെ fd;
  ret = TEMP_FAILURE_RETRY (ലിഖ (fd, oom_buffer, strlen (oom_buffer)));
  എങ്കിൽ (ret < 0)
    തിരികെ crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  തിരികെ 0;
}

സ്ഥിരം അക്ഷരം *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
validate_sysctl (സ്ഥിരം അക്ഷരം *original_താക്കോൽ, സ്ഥിരം അക്ഷരം *original_മൂല്യം, സ്ഥിരം അക്ഷരം *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  എങ്കിൽ (name[0] == '/' || path_has_dot_dot_component (name))
    തിരികെ crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_താക്കോൽ);

  എങ്കിൽ (has_prefix (name, "fs/mqueue/"))
    {
      എങ്കിൽ (namespaces_created & CLONE_NEWIPC)
        തിരികെ 0;

      namespace = "IPC";
      പോകുക fail;
    }

  എങ്കിൽ (has_prefix (name, "kernel/"))
    {
      size_t i;

      വേണ്ടി (i = 0; sysctlRequiringIPC[i]; i++)
        എങ്കിൽ (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            എങ്കിൽ (namespaces_created & CLONE_NEWIPC)
              തിരികെ 0;

            namespace = "IPC";
            പോകുക fail;
          }

      എങ്കിൽ (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          എങ്കിൽ (! is_empty_string (def->domainname) && (strcmp (original_മൂല്യം, def->domainname) != 0))
            തിരികെ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_താക്കോൽ);

          എങ്കിൽ (namespaces_created & CLONE_NEWUTS)
            തിരികെ 0;

          namespace = "UTS";
          പോകുക fail;
        }

      എങ്കിൽ (strcmp (name, "kernel/hostname") == 0)
        തിരികെ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_താക്കോൽ);
    }
  എങ്കിൽ (has_prefix (name, "net/"))
    {
      എങ്കിൽ (namespaces_created & CLONE_NEWNET)
        തിരികെ 0;

      namespace = "network";
      പോകുക fail;
    }

  തിരികെ crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_താക്കോൽ);

fail:
  തിരികെ crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_താക്കോൽ, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
സ്ഥിതം അക്ഷരം *
sysctl_error_reason (സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ namespaces_created, പൂർണ്ണസംഖ്യ errno_)
{

  എങ്കിൽ (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    തിരികെ xstrdup ("are all the IDs mapped in the user namespace?");

  തിരികെ ശൂന്യ;
}

പൂർണ്ണസംഖ്യ
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close പൂർണ്ണസംഖ്യ dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  എങ്കിൽ (def->linux == ശൂന്യ || def->linux->sysctl == ശൂന്യ || def->linux->sysctl->len == 0)
    തിരികെ 0;

  വേണ്ടി (i = 0; i < def->linux->namespaces_len; i++)
    {
      പൂർണ്ണസംഖ്യ മൂല്യം;

      മൂല്യം = libcrun_find_namespace (def->linux->namespaces[i]->type);
      എങ്കിൽ (UNLIKELY (മൂല്യം < 0))
        തിരികെ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= മൂല്യം;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  എങ്കിൽ (UNLIKELY (dirfd < 0))
    തിരികെ dirfd;

  വേണ്ടി (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free അക്ഷരം *name = ശൂന്യ;
      cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
      പൂർണ്ണസംഖ്യ ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      എങ്കിൽ (def->linux->sysctl->താക്കോലുകൾ[i] == ശൂന്യ || def->linux->sysctl->മൂല്യങ്ങൾ[i] == ശൂന്യ)
        തിരികെ crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_താക്കോൽ_to_proc_path (def->linux->sysctl->താക്കോലുകൾ[i]);

      ret = validate_sysctl (def->linux->sysctl->താക്കോലുകൾ[i], def->linux->sysctl->മൂല്യങ്ങൾ[i], name, namespaces_created, def, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (fd < 0))
        തിരികെ crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (ലിഖ (fd, def->linux->sysctl->മൂല്യങ്ങൾ[i], strlen (def->linux->sysctl->മൂല്യങ്ങൾ[i])));
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          പൂർണ്ണസംഖ്യ saved_errno = errno;
          cleanup_free അക്ഷരം *reason = ശൂന്യ;

          reason = sysctl_error_reason (def->linux->sysctl->താക്കോലുകൾ[i], namespaces_created, saved_errno);
          തിരികെ crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
open_terminal (അക്ഷരം **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  uid_t uid = 0;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  തിരികെ ret;
}

അക്ഷരം *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  തിരികെ get_private_data (container)->external_descriptors;
}

പൂർണ്ണസംഖ്യ
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *buf = ശൂന്യ;
  json_gen_ctx *gen = ശൂന്യ;
  size_t buf_len;
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ i;

  എങ്കിൽ (! json_gen_init (&gen, ശൂന്യ))
    തിരികെ crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  എങ്കിൽ (UNLIKELY (ret != json_gen_status_ok))
    പോകുക json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  വേണ്ടി (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      അക്ഷരം link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          എങ്കിൽ (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          അല്ലെങ്കിൽ
            {
              json_gen_free (gen);
              തിരികെ crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      എങ്കിൽ (UNLIKELY (ret != json_gen_status_ok))
        പോകുക json_error;
    }

  ret = json_gen_array_close (gen);
  എങ്കിൽ (UNLIKELY (ret != json_gen_status_ok))
    പോകുക json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  എങ്കിൽ (UNLIKELY (ret != json_gen_status_ok))
    പോകുക json_error;

  എങ്കിൽ (buf)
    {
      അക്ഷരം *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  തിരികെ 0;

json_error:
  എങ്കിൽ (gen)
    json_gen_free (gen);
  തിരികെ json_gen_error_to_crun_error (ret, err);
}

പൂർണ്ണസംഖ്യ
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free അക്ഷരം *pty = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (def->process == ശൂന്യ || ! def->process->terminal)
    തിരികെ 0;

  fd = open_terminal (&pty, def->process, err);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ fd;

  എങ്കിൽ (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", ശൂന്യ, MS_BIND, ശൂന്യ, LABEL_MOUNT, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      ret = unlink ("/dev/console");
      എങ്കിൽ (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          എങ്കിൽ (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", ശൂന്യ, MS_BIND, ശൂന്യ, LABEL_MOUNT, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;

              തിരികെ get_and_reset (&fd);
            }
          തിരികെ crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  തിരികെ get_and_reset (&fd);
}

സ്ഥിതം bool
read_error_from_sync_socket (പൂർണ്ണസംഖ്യ sync_socket_fd, പൂർണ്ണസംഖ്യ *error, അക്ഷരം **str)
{
  cleanup_free അക്ഷരം *b = ശൂന്യ;
  size_t size;
  പൂർണ്ണസംഖ്യ code = 0;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (പഠ (sync_socket_fd, &code, വലുപ്പം (code)));
      എങ്കിൽ (UNLIKELY (ret != (പൂർണ്ണസംഖ്യ) വലുപ്പം (code)))
        തിരികെ false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (പഠ (sync_socket_fd, &size, വലുപ്പം (size)));
  എങ്കിൽ (UNLIKELY (ret != (പൂർണ്ണസംഖ്യ) വലുപ്പം (size)))
    തിരികെ false;

  എങ്കിൽ (size == 0)
    തിരികെ false;

  എങ്കിൽ (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (പഠ (sync_socket_fd, b, size));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ false;

  b[ret] = '\0';

  *str = b;
  b = ശൂന്യ;
  തിരികെ true;
}

സ്ഥിതം bool
send_error_to_sync_socket (പൂർണ്ണസംഖ്യ sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ code;
  അക്ഷരം *സന്ദേശം;

  എങ്കിൽ (err == ശൂന്യ || *err == ശൂന്യ)
    തിരികെ false;

  code = crun_error_get_errno (err);

  എങ്കിൽ (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket_fd, "1", 1));
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ false;
    }

  ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket_fd, &code, വലുപ്പം (code)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ false;

  സന്ദേശം = (*err)->സന്ദേശം;
  size = strlen (സന്ദേശം) + 1;
  ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket_fd, &size, വലുപ്പം (size)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ false;

  ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket_fd, സന്ദേശം, size));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ false;

  തിരികെ true;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
send_success_to_sync_socket (പൂർണ്ണസംഖ്യ sync_socket, libcrun_error_t *err)
{
  സ്ഥിരം പൂർണ്ണസംഖ്യ success = 0;
  പൂർണ്ണസംഖ്യ ret;

  ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket, &success, വലുപ്പം (success)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "write to sync socket");

  തിരികെ 0;
}

സ്ഥിതം __attribute__ ((noreturn)) ശൂന്യം
send_error_to_sync_socket_and_die (പൂർണ്ണസംഖ്യ sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  അക്ഷരം *സന്ദേശം;

  എങ്കിൽ (err == ശൂന്യ || *err == ശൂന്യ)
    _safe_exit (EXIT_FAILURE);

  എങ്കിൽ (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      സന്ദേശം = (*err)->സന്ദേശം;
      libcrun_fail_with_error (errno, "%s", സന്ദേശം);
    }
  _safe_exit (EXIT_FAILURE);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
expect_success_from_sync_socket (പൂർണ്ണസംഖ്യ sync_fd, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *err_str = ശൂന്യ;
  പൂർണ്ണസംഖ്യ res = 1;
  പൂർണ്ണസംഖ്യ ret;

  ret = TEMP_FAILURE_RETRY (പഠ (sync_fd, &res, വലുപ്പം (res)));
  എങ്കിൽ (UNLIKELY (ret != വലുപ്പം (res)))
    തിരികെ crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  എങ്കിൽ (res == 0)
    തിരികെ 0;

  എങ്കിൽ (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      എങ്കിൽ (! is_empty_string (err_str))
        തിരികെ crun_make_error (err, res, "%s", err_str);
    }

  തിരികെ crun_make_error (err, res, "read from sync socket");
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
join_namespaces (runtime_spec_schema_config_schema *def, പൂർണ്ണസംഖ്യ *namespaces_to_join, പൂർണ്ണസംഖ്യ n_namespaces_to_join,
                 പൂർണ്ണസംഖ്യ *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ i;

  വേണ്ടി (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free അക്ഷരം *cwd = ശൂന്യ;
      പൂർണ്ണസംഖ്യ orig_index = namespaces_to_join_index[i];
      പൂർണ്ണസംഖ്യ മൂല്യം;

      എങ്കിൽ (namespaces_to_join[i] < 0)
        തുടരുക;

      /* Skip the user namespace.  */
      മൂല്യം = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      എങ്കിൽ (മൂല്യം == CLONE_NEWUSER)
        തുടരുക;

      എങ്കിൽ (മൂല്യം == CLONE_NEWNS)
        {
          cwd = getcwd (ശൂന്യ, 0);
          എങ്കിൽ (UNLIKELY (cwd == ശൂന്യ))
            തിരികെ crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], മൂല്യം);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (ignore_join_errors)
            തുടരുക;
          തിരികെ crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      എങ്കിൽ (മൂല്യം == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  തിരികെ 0;
}

#define MAX_NAMESPACES 10

ഘടന init_status_s
{
  /* fd to the namespace to join.  */
  പൂർണ്ണസംഖ്യ fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  പൂർണ്ണസംഖ്യ index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  പൂർണ്ണസംഖ്യ മൂല്യം[MAX_NAMESPACES];
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
  പൂർണ്ണസംഖ്യ userns_index;
  /* def->linux->namespaces userns.  */
  പൂർണ്ണസംഖ്യ userns_index_origin;

  /* All namespaces created/joined by the container.  */
  പൂർണ്ണസംഖ്യ all_namespaces;

  /* What namespaces are still missing to be created.  */
  പൂർണ്ണസംഖ്യ namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  പൂർണ്ണസംഖ്യ idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  പൂർണ്ണസംഖ്യ idx_timens_to_join_immediately;
};

ശൂന്യം
cleanup_free_init_statusp (ഘടന init_status_s *ns)
{
  size_t i;

  എങ്കിൽ (ns == ശൂന്യ)
    തിരികെ;

  വേണ്ടി (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
configure_init_status (ഘടന init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  വേണ്ടി (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  വേണ്ടി (i = 0; i < def->linux->namespaces_len; i++)
    {
      പൂർണ്ണസംഖ്യ മൂല്യം = libcrun_find_namespace (def->linux->namespaces[i]->type);
      എങ്കിൽ (UNLIKELY (മൂല്യം < 0))
        തിരികെ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      എങ്കിൽ (ns->all_namespaces & മൂല്യം)
        തിരികെ crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= മൂല്യം;

      എങ്കിൽ (def->linux->namespaces[i]->path == ശൂന്യ)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= മൂല്യം;
        }
      അല്ലെങ്കിൽ
        {
          പൂർണ്ണസംഖ്യ fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          എങ്കിൽ (ns->fd_len >= MAX_NAMESPACES)
            തിരികെ crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          എങ്കിൽ (UNLIKELY (fd < 0))
            തിരികെ crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          എങ്കിൽ (മൂല്യം == CLONE_NEWUSER)
            {
              ns->userns_index = ns->fd_len;
              ns->userns_index_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->index[ns->fd_len] = i;
          ns->മൂല്യം[ns->fd_len] = മൂല്യം;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  എങ്കിൽ (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  തിരികെ 0;
}

/* Detect if root is available in the container.  */
സ്ഥിതം bool
വേര്_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  വേണ്ടി (i = 0; i < len; i++)
    എങ്കിൽ (mappings[i]->container_id == 0)
      തിരികെ true;

  തിരികെ false;
}

സ്ഥിതം ഘടന libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  ഘടന libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  എങ്കിൽ (dev_fds == ശൂന്യ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  തിരികെ dev_fds;
}

#define അക്കം_NEEDED_DEVS (വലുപ്പം (needed_devs) / വലുപ്പം (needed_devs[0]) - 1)

സ്ഥിതം ഘടന libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  ഘടന libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  എങ്കിൽ (fds == ശൂന്യ)
    {
      fds = make_libcrun_fd_map (അക്കം_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  തിരികെ fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  എങ്കിൽ (recursive)
    *recursive = false;

  എങ്കിൽ (src_nofollow != ശൂന്യ)
    *src_nofollow = false;

  വേണ്ടി (i = 0; i < mnt->options_len; i++)
    {
      എങ്കിൽ (mnt->options[i] == ശൂന്യ)
        തുടരുക;

      എങ്കിൽ (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          എങ്കിൽ (src_nofollow == ശൂന്യ || *src_nofollow)
            നിർത്തുക;
        }
      എങ്കിൽ (strcmp (mnt->options[i], "rbind") == 0)
        {
          എങ്കിൽ (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          എങ്കിൽ (src_nofollow == ശൂന്യ || *src_nofollow)
            നിർത്തുക;
        }
      എങ്കിൽ (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  തിരികെ ret;
}

സ്ഥിതം അക്ഷരം *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  വേണ്ടി (i = 0; i < mnt->options_len; i++)
    {
      എങ്കിൽ (mnt->options[i] == ശൂന്യ)
        തുടരുക;

      എങ്കിൽ (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          തിരികെ mnt->options[i];
        }
      എങ്കിൽ (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          തിരികെ mnt->options[i];
        }
    }
  തിരികെ ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
open_mount_type (സ്ഥിരം അക്ഷരം *type, പൂർണ്ണസംഖ്യ *out_fd, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ fsopen_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ newfs_fd = -1;
  പൂർണ്ണസംഖ്യ ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  എങ്കിൽ (UNLIKELY (fsopen_fd < 0))
    തിരികെ crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, ശൂന്യ, ശൂന്യ, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  എങ്കിൽ (UNLIKELY (newfs_fd < 0))
    തിരികെ crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, പൂർണ്ണസംഖ്യ *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  ഘടന mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  സ്ഥിരം അക്ഷരം *idmap_option;
  bool recursive = false;
  സ്ഥിരം അക്ഷരം *options = ശൂന്യ;
  bool has_mappings;
  പൂർണ്ണസംഖ്യ ret;
  അക്ഷരം *extra_സന്ദേശം = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != ശൂന്യ);
  എങ്കിൽ (has_mappings_out)
    *has_mappings_out = has_mappings;
  എങ്കിൽ (! has_mappings)
    തിരികെ 0;

  എങ്കിൽ ((mnt->uid_mappings == ശൂന്യ) != (mnt->gid_mappings == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  എങ്കിൽ (idmap_option)
    {
      options = strchr (idmap_option, '=');
      എങ്കിൽ (options)
        {
          /* Skip the '=' itself.  */
          options++;
          എങ്കിൽ (options[0] == '\0')
            options = ശൂന്യ;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  എങ്കിൽ (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ fd;

  എങ്കിൽ (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_വൃക്ഷം (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      എങ്കിൽ (UNLIKELY (newfs_fd < 0))
        തിരികെ crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  അല്ലെങ്കിൽ
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno == EINVAL)
        {
          extra_സന്ദേശം = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      തിരികെ crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_സന്ദേശം);
    }

  *out_fd = get_and_reset (&newfs_fd);
  തിരികെ 0;
}

സ്ഥിതം uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  എങ്കിൽ (! def || ! def->linux)
    പോകുക exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  എങ്കിൽ (! mappings)
    പോകുക exit;

  വേണ്ടി (i = 0; i < len; i++)
    {
      എങ്കിൽ (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        തിരികെ id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  തിരികെ is_uid ? get_overflow_uid () : get_overflow_gid ();
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
precreate_device (libcrun_container_t *container, പൂർണ്ണസംഖ്യ devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  അക്ഷരം name[64];
  mode_t type;
  dev_t dev;
  പൂർണ്ണസംഖ്യ ret;

  ret = snprintf (name, വലുപ്പം (name), "%zu", i);
  എങ്കിൽ (UNLIKELY (ret >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (name)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "chown `%s`", device->path);

  തിരികെ get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
send_mounts (പൂർണ്ണസംഖ്യ sync_socket_host, ഘടന libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket_host, &how_many, വലുപ്പം (how_many)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "write to sync socket");

  വേണ്ടി (i = 0; i < total; i++)
    {
      എങ്കിൽ (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (അക്ഷരം *) &i, വലുപ്പം (i), err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, പൂർണ്ണസംഖ്യ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ഘടന libcrun_fd_map *mount_fds = ശൂന്യ;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free അക്ഷരം *rootfs = ശൂന്യ;
  size_t how_many = 0;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (def->mounts_len == 0)
    തിരികെ 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  എങ്കിൽ (def->വേര് && def->വേര്->path)
    rootfs = realpath (def->വേര്->path, ശൂന്യ);

  എങ്കിൽ (! has_userns)
    {
      പൂർണ്ണസംഖ്യ is_in_userns = check_running_in_user_namespace (err);
      എങ്കിൽ (UNLIKELY (is_in_userns < 0))
        തിരികെ is_in_userns;

      has_userns = is_in_userns > 0;
    }

  വേണ്ടി (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      പൂർണ്ണസംഖ്യ mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      എങ്കിൽ (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == ശൂന്യ)
        {
          unsigned long propagation = 0;

          എങ്കിൽ (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, ശൂന്യ, ശൂന്യ, ശൂന്യ, ശൂന്യ) & ALL_PROPAGATIONS_NO_REC;
          എങ്കിൽ (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          എങ്കിൽ (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      എങ്കിൽ (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  തിരികെ send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
prepare_and_send_dev_mounts (libcrun_container_t *container, പൂർണ്ണസംഖ്യ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ഘടന libcrun_fd_map *dev_fds = ശൂന്യ;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close പൂർണ്ണസംഖ്യ current_mountns = -1;
  cleanup_free അക്ഷരം *state_dir = ശൂന്യ;
  cleanup_free അക്ഷരം *devs_path = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ devs_mountfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ targetfd = -1;
  സ്ഥിരം അക്ഷരം *context_type = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *label = ശൂന്യ;
  size_t how_many = 0;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  എങ്കിൽ (def->linux == ശൂന്യ || def->linux->devices_len == 0)
    തിരികെ 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  എങ്കിൽ (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    തിരികെ send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_വേര് : ശൂന്യ),
                                     (container->context ? container->context->id : ശൂന്യ), err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = mkdir (devs_path, 0700);
  എങ്കിൽ (UNLIKELY (ret < 0 && errno != EEXIST))
    തിരികെ crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  എങ്കിൽ (UNLIKELY (current_mountns < 0))
    തിരികെ current_mountns;

  ret = unshare (CLONE_NEWNS);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (ശൂന്യ, "/", ശൂന്യ, MS_REC | MS_PRIVATE, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      പോകുക restore_mountns;
    }

  എങ്കിൽ (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      എങ്കിൽ (UNLIKELY (context_type == ശൂന്യ))
        {
          ret = -1;
          പോകുക restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", ശൂന്യ, context_type, label, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      പോകുക restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      പോകുക restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      പോകുക restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      പോകുക restore_mountns;
    }

  വേണ്ടി (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          തുടരുക;
        }

      dev_fds->fds[i] = ret;

      എങ്കിൽ (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  എങ്കിൽ (ns_changed && current_mountns >= 0)
    {
      പൂർണ്ണസംഖ്യ setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      എങ്കിൽ (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          തിരികെ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, പൂർണ്ണസംഖ്യ sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map ഘടന libcrun_fd_map *fds = ശൂന്യ;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (അക്കം_NEEDED_DEVS);

  എങ്കിൽ (! has_userns || geteuid () > 0)
    തിരികെ send_mounts (sync_socket_host, fds, 0, അക്കം_NEEDED_DEVS, err);

  വേണ്ടി (i = 0; i < അക്കം_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      പൂർണ്ണസംഖ്യ fd = syscall_open_വൃക്ഷം (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      എങ്കിൽ (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  തിരികെ send_mounts (sync_socket_host, fds, how_many, അക്കം_NEEDED_DEVS, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, പൂർണ്ണസംഖ്യ sync_socket_host, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
receive_mounts (ഘടന libcrun_fd_map *fds, പൂർണ്ണസംഖ്യ sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (fds->nfds == 0)
    തിരികെ 0;

  ret = TEMP_FAILURE_RETRY (പഠ (sync_socket_container, &how_many, വലുപ്പം (how_many)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "read from sync socket");

  വേണ്ടി (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (അക്ഷരം *) &index, വലുപ്പം (index), err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      എങ്കിൽ (index >= fds->nfds)
        തിരികെ crun_make_error (err, 0, "invalid mount data received");

      എങ്കിൽ (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  തിരികെ 0;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (! container->host_has_cap_sys_ptrace)
    തിരികെ 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool വേര്_mapped = false;

      എങ്കിൽ (def->linux->uid_mappings_len != 0)
        {
          വേര്_mapped = വേര്_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          എങ്കിൽ (! വേര്_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      എങ്കിൽ (def->linux->gid_mappings_len != 0)
        {
          വേര്_mapped = വേര്_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          എങ്കിൽ (! വേര്_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  എങ്കിൽ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    തിരികെ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "setresgid to `%d`", gid);

  തിരികെ 0;
}

സ്ഥിതം bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  തിരികെ mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, ശൂന്യ, ശൂന്യ);
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ഘടന libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close പൂർണ്ണസംഖ്യ socket_parent = -1;
  cleanup_close പൂർണ്ണസംഖ്യ socket_child = -1;
  cleanup_close പൂർണ്ണസംഖ്യ വേര്_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ cwd_fd = -1;
  size_t i, missing = 0;
  പൂർണ്ണസംഖ്യ saved_errno;
  പൂർണ്ണസംഖ്യ sockets[2];
  pid_t pid;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (def->വേര് == ശൂന്യ || def->വേര്->path == ശൂന്യ)
    തിരികെ 0;

  വേണ്ടി (i = 0; i < def->mounts_len; i++)
    എങ്കിൽ (is_proc_mount (def->mounts[i]))
      missing++;
  എങ്കിൽ (missing == 0)
    തിരികെ 1;

  വേര്_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (വേര്_fd < 0))
    തിരികെ crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (cwd_fd < 0))
    തിരികെ crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->വേര്->path);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ 0;

  pid = fork ();
  saved_errno = errno;
  എങ്കിൽ (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      എങ്കിൽ (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (ശൂന്യം) unshare (CLONE_NEWNS);

      വേണ്ടി (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

          എങ്കിൽ (! is_proc_mount (def->mounts[i]))
            തുടരുക;

          fd = open_mount_of_type (container, def->mounts[i], err);
          എങ്കിൽ (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (അക്ഷരം *) &i, വലുപ്പം (i), err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  എങ്കിൽ (UNLIKELY (fchdir (വേര്_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    തിരികെ crun_make_error (err, errno, "restore the root directory");

  എങ്കിൽ (UNLIKELY (pid < 0))
    തിരികെ crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  ആകുമ്പോൾ (missing > 0)
    {
      libcrun_error_t tmp_err = ശൂന്യ;

      ret = receive_fd_from_socket_with_payload (socket_parent, (അക്ഷരം *) &i, വലുപ്പം (i), &tmp_err);
      എങ്കിൽ (ret < 0)
        {
          crun_error_release (&tmp_err);
          നിർത്തുക;
        }
      എങ്കിൽ (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          നിർത്തുക;
        }

      എങ്കിൽ (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, ശൂന്യ, 0);

  തിരികെ missing == 0 ? 1 : 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
init_container (libcrun_container_t *container, പൂർണ്ണസംഖ്യ sync_socket_container, ഘടന init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ഘടന libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close പൂർണ്ണസംഖ്യ timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      എങ്കിൽ (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      എങ്കിൽ (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      എങ്കിൽ (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          delayed_pidns_fork = ret > 0;
        }

      എങ്കിൽ (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket_container, &new_pid, വലുപ്പം (new_pid)));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "write to sync socket");
        }
      അല്ലെങ്കിൽ
        {
          new_pid = fork ();
          എങ്കിൽ (UNLIKELY (new_pid < 0))
            തിരികെ crun_make_error (err, errno, "fork");
        }

      എങ്കിൽ (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket_container, &new_pid, വലുപ്പം (new_pid)));
          എങ്കിൽ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = libcrun_set_oom (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  എങ്കിൽ ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      വേണ്ടി (i = 0; i < def->mounts_len; i++)
        {
          പൂർണ്ണസംഖ്യ fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          എങ്കിൽ (def->mounts[i]->type == ശൂന്യ)
            തുടരുക;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          എങ്കിൽ (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, ശൂന്യ, ശൂന്യ, ശൂന്യ);
          എങ്കിൽ (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, ശൂന്യ, ശൂന്യ, ശൂന്യ);

          എങ്കിൽ (fd >= 0)
            {
              എങ്കിൽ (mount_fds->fds[i] >= 0)
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
  എങ്കിൽ (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      എങ്കിൽ (UNLIKELY (timens_offsets_fd < 0))
        തിരികെ timens_offsets_fd;
    }

  എങ്കിൽ (init_status->all_namespaces & CLONE_NEWUSER)
    {
      എങ്കിൽ (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      എങ്കിൽ (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
      അല്ലെങ്കിൽ
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "unshare");
    }

  എങ്കിൽ (def->linux->time_offsets)
    {
      അക്ഷരം fmt_buffer[128];
      പൂർണ്ണസംഖ്യ fd = timens_offsets_fd;

      എങ്കിൽ (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, വലുപ്പം (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          എങ്കിൽ (UNLIKELY (ret >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buffer)))
            തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long പൂർണ്ണസംഖ്യ) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = ലിഖ (fd, fmt_buffer, strlen (fmt_buffer));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      എങ്കിൽ (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long പൂർണ്ണസംഖ്യ) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, വലുപ്പം (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          എങ്കിൽ (UNLIKELY (ret >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buffer)))
            തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = ലിഖ (fd, fmt_buffer, strlen (fmt_buffer));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  എങ്കിൽ (init_status->must_fork)
    {
      എങ്കിൽ (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          delayed_pidns_fork = ret > 0;
        }

      എങ്കിൽ (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          എങ്കിൽ (UNLIKELY (pid_container < 0))
            തിരികെ crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      എങ്കിൽ (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket_container, &pid_container, വലുപ്പം (pid_container)));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "write to sync socket");

          എങ്കിൽ (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  തിരികെ get_private_data (container)->delayed_pidns_fork;
}

പൂർണ്ണസംഖ്യ
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ client_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ pidfd = -1;
  സ്ഥിരം അക്ഷരം *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  എങ്കിൽ (v == ശൂന്യ)
    തിരികെ 0;

  pidfd = syscall_pidfd_open (pid, 0);
  എങ്കിൽ (UNLIKELY (pidfd < 0))
    തിരികെ crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  എങ്കിൽ (UNLIKELY (client_fd < 0))
    തിരികെ client_fd;

  തിരികെ send_fd_to_socket (client_fd, pidfd, err);
}

സ്ഥിതം bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  എങ്കിൽ (process == ശൂന്യ || process->exec_cpu_affinity == ശൂന്യ)
    തിരികെ false;
  തിരികെ (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, ശൂന്യം *args,
                             പൂർണ്ണസംഖ്യ *sync_socket_out, ഘടന libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) ഘടന init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close പൂർണ്ണസംഖ്യ sync_socket_container = -1;
  അക്ഷരം *notify_socket_env = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close പൂർണ്ണസംഖ്യ restore_pidns = -1;
  പൂർണ്ണസംഖ്യ first_clone_args = 0;
  പൂർണ്ണസംഖ്യ sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  എങ്കിൽ (def->വേര്)
    {
      ret = do_notify_socket (container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  എങ്കിൽ (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = libcrun_set_oom (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->വേര് ? container->container_def->വേര്->path : ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  വേണ്ടി (i = 0; i < init_status.fd_len; i++)
    {
      തിരഞ്ഞെടുപ്പ് (init_status.മൂല്യം[i])
        {
        അവസ്ഥ CLONE_NEWIPC:
          എങ്കിൽ (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          നിർത്തുക;

        അവസ്ഥ CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          എങ്കിൽ (! new_userns)
            init_status.must_fork = true;
          അല്ലെങ്കിൽ
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          നിർത്തുക;

        അവസ്ഥ CLONE_NEWTIME:
          എങ്കിൽ (! new_userns)
            init_status.must_fork = true;
          അല്ലെങ്കിൽ
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          നിർത്തുക;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  എങ്കിൽ ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  അല്ലെങ്കിൽ എങ്കിൽ ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  എങ്കിൽ (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  എങ്കിൽ (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      ഘടന _clone3_args clone3_args;
      memset (&clone3_args, 0, വലുപ്പം (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      എങ്കിൽ (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  എങ്കിൽ (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (pid < 0))
        തിരികെ crun_make_error (err, errno, "clone");
    }

  എങ്കിൽ (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = close_and_reset (&sync_socket_container);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_വൃക്ഷം_fd));

      എങ്കിൽ (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          ret = TEMP_FAILURE_RETRY (പഠ (sync_socket_host, &new_pid, വലുപ്പം (new_pid)));
          എങ്കിൽ (UNLIKELY (ret != വലുപ്പം (new_pid)))
            തിരികെ crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          എങ്കിൽ (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          അല്ലെങ്കിൽ
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, ശൂന്യ, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      എങ്കിൽ (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      എങ്കിൽ ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      എങ്കിൽ (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          ret = TEMP_FAILURE_RETRY (പഠ (sync_socket_host, &grandchild, വലുപ്പം (grandchild)));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          എങ്കിൽ (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          അല്ലെങ്കിൽ
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, ശൂന്യ, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      എങ്കിൽ (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      pid_to_clean = 0;
      തിരികെ pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  എങ്കിൽ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  അല്ലെങ്കിൽ
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->സന്ദേശം);
    }

  /* Jump into the specified entrypoint.  */
  എങ്കിൽ (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  എങ്കിൽ (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->സന്ദേശം);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  എങ്കിൽ (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, പൂർണ്ണസംഖ്യ sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, സ്ഥിരം അക്ഷരം *sub_cgroup,
                            പൂർണ്ണസംഖ്യ *terminal_fd, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  അക്ഷരം res;
  pid_t pid;
  cleanup_close പൂർണ്ണസംഖ്യ sync_fd = sync_socket_fd;

  എങ്കിൽ (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (പഠ (sync_fd, &res, 1));
  എങ്കിൽ (UNLIKELY (ret != 1))
    തിരികെ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  എങ്കിൽ (res != '0')
    തിരികെ crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (പഠ (sync_fd, &pid, വലുപ്പം (pid)));
  എങ്കിൽ (UNLIKELY (ret != വലുപ്പം (pid)))
    തിരികെ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, ശൂന്യ, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "waitpid for exec child pid");

  എങ്കിൽ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (need_move_to_cgroup)
    {
      cleanup_cgroup_status ഘടന libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free അക്ഷരം *final_cgroup = ശൂന്യ;

      എങ്കിൽ (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, ശൂന്യ);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_set_io_priority (pid, process, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (ലിഖ (sync_fd, &ret, വലുപ്പം (ret)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "write to sync socket");

  എങ്കിൽ (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          പൂർണ്ണസംഖ്യ err_code = 0;
          cleanup_free അക്ഷരം *err_str = ശൂന്യ;

          എങ്കിൽ (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              എങ്കിൽ (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  തിരികെ crun_make_error (err, err_code, "%s", err_str);
                }
            }

          തിരികെ crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  തിരികെ pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close പൂർണ്ണസംഖ്യ pidfd_pid_to_join = -1;
  പൂർണ്ണസംഖ്യ all_flags = 0;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  എങ്കിൽ (def->linux && def->linux->namespaces)
    {
      വേണ്ടി (i = 0; i < def->linux->namespaces_len; i++)
        എങ്കിൽ (! is_empty_string (def->linux->namespaces[i]->path))
          തിരികെ 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  എങ്കിൽ (UNLIKELY (pidfd_pid_to_join < 0))
    തിരികെ 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  എങ്കിൽ (ret == 0)
    തിരികെ crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  വേണ്ടി (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].മൂല്യം;

  ret = setns (pidfd_pid_to_join, all_flags);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ 0;

  തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  പൂർണ്ണസംഖ്യ fds_joined[MAX_NAMESPACES] = {
    0,
  };
  പൂർണ്ണസംഖ്യ fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  /* Nothing left to do if the namespaces were joined.  */
  എങ്കിൽ (LIKELY (ret > 0))
    തിരികെ 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  എങ്കിൽ (def->linux->namespaces_len >= MAX_NAMESPACES)
    തിരികെ crun_make_error (err, 0, "invalid configuration");

  വേണ്ടി (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free അക്ഷരം *ns_path = ശൂന്യ;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      എങ്കിൽ (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              തുടരുക;
            }

          ret = fds[i];
          പോകുക exit;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക exit;
  എങ്കിൽ (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      പോകുക exit;
    }

  വേണ്ടി (i = 0; namespaces[i].ns_file; i++)
    {
      എങ്കിൽ (namespaces[i].മൂല്യം == CLONE_NEWUSER)
        തുടരുക;

      ret = setns (fds[i], 0);
      എങ്കിൽ (ret == 0)
        fds_joined[i] = 1;
    }
  വേണ്ടി (i = 0; namespaces[i].ns_file; i++)
    {
      എങ്കിൽ (fds_joined[i])
        തുടരുക;
      ret = setns (fds[i], 0);
      എങ്കിൽ (ret == 0)
        fds_joined[i] = 1;
    }
  വേണ്ടി (i = 0; namespaces[i].ns_file; i++)
    {
      എങ്കിൽ (fds_joined[i])
        തുടരുക;
      ret = setns (fds[i], 0);
      എങ്കിൽ (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          വേണ്ടി (j = 0; j < def->linux->namespaces_len; j++)
            {
              എങ്കിൽ (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  നിർത്തുക;
                }
            }
          എങ്കിൽ (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              തുടരുക;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          പോകുക exit;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

exit:
  വേണ്ടി (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      സ്ഥിരം അക്ഷരം *sub_cgroup,
                      പൂർണ്ണസംഖ്യ detach,
                      runtime_spec_schema_config_schema_process *process,
                      പൂർണ്ണസംഖ്യ *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ sync_socket_fd[2];
  cleanup_close പൂർണ്ണസംഖ്യ cgroup_dirfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ sync_fd = -1;
  ഘടന _clone3_args clone3_args;
  bool need_move_to_cgroup;

  എങ്കിൽ (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "set child subreaper");
    }

  എങ്കിൽ (path_has_dot_dot_component (sub_cgroup))
    തിരികെ crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  എങ്കിൽ (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free അക്ഷരം *final_cgroup = ശൂന്യ;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status ഘടന libcrun_cgroup_status *cgroup_status = ശൂന്യ;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    എങ്കിൽ (get_force_cgroup_v1_annotation (container) == ശൂന്യ)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        എങ്കിൽ (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, വലുപ്പം (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  എങ്കിൽ (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  അല്ലെങ്കിൽ
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  എങ്കിൽ (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  /* On errors, fall back to fork().  */
  എങ്കിൽ (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      എങ്കിൽ (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          പോകുക exit;
        }
    }

  എങ്കിൽ (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      തിരികെ join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (ലിഖ (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->സന്ദേശം);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  എങ്കിൽ (UNLIKELY (pid < 0))
    {
      പൂർണ്ണസംഖ്യ saved_errno = errno;
      TEMP_FAILURE_RETRY (ലിഖ (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  എങ്കിൽ (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (ലിഖ (sync_fd, "0", 1));
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (ലിഖ (sync_fd, &pid, വലുപ്പം (pid)));
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  അല്ലെങ്കിൽ
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free അക്ഷരം *pty = ശൂന്യ;
      പൂർണ്ണസംഖ്യ r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (പഠ (sync_fd, &r, വലുപ്പം (r)));
      എങ്കിൽ (UNLIKELY (ret != വലുപ്പം (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      എങ്കിൽ (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      എങ്കിൽ (terminal_fd)
        {
          cleanup_close പൂർണ്ണസംഖ്യ ptmx_fd = -1;

          ret = set_id_init (container, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          എങ്കിൽ (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  തിരികെ 0;

exit:
  എങ്കിൽ (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  എങ്കിൽ (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_linux_container_update (libcrun_container_status_t *status, സ്ഥിരം അക്ഷരം *state_വേര്, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status ഘടന libcrun_cgroup_status *cgroup_status = ശൂന്യ;

  cgroup_status = libcrun_cgroup_make_status (status);

  തിരികെ libcrun_update_cgroup_resources (cgroup_status, state_വേര്, resources, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, സ്ഥിരം bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status ഘടന libcrun_cgroup_status *cgroup_status = ശൂന്യ;

  cgroup_status = libcrun_cgroup_make_status (status);

  തിരികെ libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  തിരികെ libcrun_container_pause_unpause_linux (status, true, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  തിരികെ libcrun_container_pause_unpause_linux (status, false, err);
}

പൂർണ്ണസംഖ്യ
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  അല്ലെങ്കിൽ എങ്കിൽ (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  അല്ലെങ്കിൽ
    തിരികെ crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, 0, "personality to `%s`", p->domain);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close പൂർണ്ണസംഖ്യ sockfd = -1;

  വേണ്ടി (i = 0; i < def->linux->namespaces_len; i++)
    {
      പൂർണ്ണസംഖ്യ മൂല്യം = libcrun_find_namespace (def->linux->namespaces[i]->type);
      എങ്കിൽ (UNLIKELY (മൂല്യം < 0))
        തിരികെ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      എങ്കിൽ (മൂല്യം == CLONE_NEWNET && def->linux->namespaces[i]->path == ശൂന്യ)
        {
          configure_network = true;
          നിർത്തുക;
        }
    }

  എങ്കിൽ (! configure_network)
    തിരികെ 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  എങ്കിൽ (LIKELY (sockfd >= 0))
    {
      ഘടന ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  അല്ലെങ്കിൽ
    {
      ഘടന nlmsghdr *hdr_recv;
      അക്ഷരം buf[വലുപ്പം (ഘടന nlmsghdr) + വലുപ്പം (ഘടന nlmsgerr)];
      ഘടന sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      ഘടന sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      ഘടന nlmsghdr hdr = {
        .nlmsg_len = വലുപ്പം (ഘടന nlmsghdr) + വലുപ്പം (ഘടന ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      ഘടന ifinfomsg സന്ദേശം = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((ഘടന nlmsghdr *) buf) = hdr;
      *((ഘടന ifinfomsg *) (buf + വലുപ്പം (ഘടന nlmsghdr))) = സന്ദേശം;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      എങ്കിൽ (UNLIKELY (sockfd < 0))
        തിരികെ crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (ഘടന sockaddr *) &addr, വലുപ്പം (addr));
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, വലുപ്പം (buf), 0, (ഘടന sockaddr *) &addr_to, വലുപ്പം (addr_to));
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, വലുപ്പം (buf), 0, ശൂന്യ, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (ഘടന nlmsghdr *) buf;

      എങ്കിൽ (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          ഘടന nlmsgerr *err_data = (ഘടന nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          എങ്കിൽ (err_data->error < 0)
            തിരികെ crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  തിരികെ 0;
}

/* Protection for attacks like CVE-2019-5736.  */
പൂർണ്ണസംഖ്യ ensure_cloned_ദ്വിമാന ();
__attribute__ ((constructor)) സ്ഥിതം ശൂന്യം
libcrun_rexec (ശൂന്യം)
{
  എങ്കിൽ (ensure_cloned_ദ്വിമാന () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

പൂർണ്ണസംഖ്യ
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  തിരികെ libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, പൂർണ്ണസംഖ്യ സൂചന, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  എങ്കിൽ (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      /* The pid is not valid anymore, return an error.  */
      എങ്കിൽ (ret == 0)
        {
          errno = ESRCH;
          തിരികെ crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, സൂചന);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "kill container");
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_kill_linux (libcrun_container_status_t *status, പൂർണ്ണസംഖ്യ സൂചന, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_close പൂർണ്ണസംഖ്യ pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  എങ്കിൽ (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      എങ്കിൽ (errno == ENOSYS)
        തിരികെ libcrun_kill_linux_no_pidfd (status, true, സൂചന, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      എങ്കിൽ (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        തിരികെ crun_make_error (err, ESRCH, "process not running");
      തിരികെ crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* The pid is not valid anymore, return an error.  */
  എങ്കിൽ (ret == 0)
    {
      errno = ESRCH;
      തിരികെ crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, സൂചന, ശൂന്യ, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      എങ്കിൽ (errno == ENOSYS)
        തിരികെ libcrun_kill_linux_no_pidfd (status, false, സൂചന, err);
      തിരികെ crun_make_error (err, errno, "send signal to pidfd");
    }

  തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
libcrun_get_intelrdt_name (സ്ഥിരം അക്ഷരം *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  എങ്കിൽ (def == ശൂന്യ || def->linux == ശൂന്യ || def->linux->intel_rdt == ശൂന്യ || def->linux->intel_rdt->clos_id == ശൂന്യ)
    {
      എങ്കിൽ (explicit)
        *explicit = false;
      തിരികെ ctr_name;
    }

  എങ്കിൽ (explicit)
    *explicit = true;
  തിരികെ def->linux->intel_rdt->clos_id;
}

പൂർണ്ണസംഖ്യ
libcrun_apply_intelrdt (സ്ഥിരം അക്ഷരം *ctr_name, libcrun_container_t *container, pid_t pid, പൂർണ്ണസംഖ്യ actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ശൂന്യ;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  സ്ഥിരം അക്ഷരം *name;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (container)
    def = container->container_def;

  എങ്കിൽ (def == ശൂന്യ || def->linux == ശൂന്യ || def->linux->intel_rdt == ശൂന്യ)
    തിരികെ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  എങ്കിൽ (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      എങ്കിൽ (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            പോകുക fail;

          has_monitoring = true;
        }
    }

  എങ്കിൽ (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക fail;
    }

  എങ്കിൽ (actions & LIBCRUN_INTELRDT_MOVE)
    {
      സ്ഥിരം അക്ഷരം *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : ശൂന്യ;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക fail;
    }

  തിരികെ 0;

fail:
  എങ്കിൽ (has_monitoring)
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      പൂർണ്ണസംഖ്യ tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      എങ്കിൽ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  എങ്കിൽ (created)
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      പൂർണ്ണസംഖ്യ tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      എങ്കിൽ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_destroy_intelrdt (സ്ഥിരം അക്ഷരം *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  സ്ഥിരം അക്ഷരം *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  എങ്കിൽ (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      പൂർണ്ണസംഖ്യ ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      എങ്കിൽ (ret < 0)
        തിരികെ ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  എങ്കിൽ (explicit)
    തിരികെ 0;

  തിരികെ resctl_destroy (clos_id, err);
}

പൂർണ്ണസംഖ്യ
libcrun_update_intel_rdt (സ്ഥിരം അക്ഷരം *ctr_name, libcrun_container_t *container, സ്ഥിരം അക്ഷരം *l3_cache_schema, സ്ഥിരം അക്ഷരം *mem_bw_schema, അക്ഷരം *സ്ഥിരം *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *name;

  എങ്കിൽ (container)
    def = container->container_def;

  എങ്കിൽ (def == ശൂന്യ || def->linux == ശൂന്യ || def->linux->intel_rdt == ശൂന്യ)
    തിരികെ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, ശൂന്യ);

  തിരികെ resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
പൂർണ്ണസംഖ്യ
libcrun_safe_chdir (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *buffer = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = chdir (path);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  എങ്കിൽ (ret == 0 || buffer[0] != '/')
    {
      എങ്കിൽ (chdir ("/") < 0)
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
#define UNREACHABLE_LEN ((പൂർണ്ണസംഖ്യ) വലുപ്പം (UNREACHABLE) - 1)

      എങ്കിൽ ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        തിരികെ crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      തിരികെ crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  തിരികെ 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
run_callback_in_namespace (പൂർണ്ണസംഖ്യ mntns_fd, പൂർണ്ണസംഖ്യ (*callback) (ശൂന്യം *, libcrun_error_t *), ശൂന്യം *arg, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ wait_status = 0;
  pid_t pid;
  പൂർണ്ണസംഖ്യ ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  എങ്കിൽ (UNLIKELY (pid < 0))
    തിരികെ crun_make_error (err, errno, "vfork");

  എങ്കിൽ (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      എങ്കിൽ (UNLIKELY (ret < 0))
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
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  തിരികെ get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
check_container_userns (libcrun_container_t *container, പൂർണ്ണസംഖ്യ userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close പൂർണ്ണസംഖ്യ expected_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ parent_fd = -1;
  ഘടന stat st, expected_st;
  സ്ഥിരം അക്ഷരം *path = ശൂന്യ;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (def->linux)
    വേണ്ടി (i = 0; i < def->linux->namespaces_len; i++)
      എങ്കിൽ (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == ശൂന്യ;
          നിർത്തുക;
        }

  എങ്കിൽ (path == ശൂന്യ)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      എങ്കിൽ (UNLIKELY (expected_fd < 0))
        തിരികെ expected_fd;
    }
  അല്ലെങ്കിൽ
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (expected_fd < 0))
        തിരികെ crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  എങ്കിൽ (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      എങ്കിൽ (UNLIKELY (parent_fd < 0))
        തിരികെ crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fstat user namespace");

  എങ്കിൽ (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    തിരികെ crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  തിരികെ 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, പൂർണ്ണസംഖ്യ *mntns_fd, പൂർണ്ണസംഖ്യ *owner_fd, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ mntns = -1;
  cleanup_close പൂർണ്ണസംഖ്യ owner = -1;
  പൂർണ്ണസംഖ്യ ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  എങ്കിൽ (UNLIKELY (mntns < 0))
    തിരികെ mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (ret == 0)
    തിരികെ crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  എങ്കിൽ (UNLIKELY (owner < 0))
    തിരികെ crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, പൂർണ്ണസംഖ്യ (*callback) (ശൂന്യം *, libcrun_error_t *), ശൂന്യം *arg, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ mntns_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ owner_fd = -1;
  പൂർണ്ണസംഖ്യ ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ run_callback_in_namespace (mntns_fd, callback, arg, err);
}

ഘടന umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
umount_nofollow (സ്ഥിരം അക്ഷരം *destination, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ parentfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ cwdfd = -1;
  അക്ഷരം buffer[PATH_MAX];
  സ്ഥിരം അക്ഷരം *parent, *name;
  അക്ഷരം *last_slash;
  size_t len;
  പൂർണ്ണസംഖ്യ saved_errno;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (is_empty_string (destination))
    തിരികെ crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  എങ്കിൽ (UNLIKELY (len >= വലുപ്പം (buffer)))
    തിരികെ crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  ആകുമ്പോൾ (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  എങ്കിൽ (last_slash == ശൂന്യ)
    {
      parent = ".";
      name = buffer;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  അല്ലെങ്കിൽ
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  എങ്കിൽ (UNLIKELY (parentfd < 0))
    തിരികെ crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (cwdfd < 0))
    തിരികെ crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  എങ്കിൽ (UNLIKELY (fchdir (cwdfd) < 0))
    തിരികെ crun_make_error (err, errno, "fchdir to the previous working directory");

  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, saved_errno, "umount `%s`", destination);

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_umount_in_a_container (ശൂന്യം *arg, libcrun_error_t *err)
{
  ഘടന umount_in_a_container_args *args = arg;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  വേണ്ടി (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

ഘടന mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  ഘടന libcrun_fd_map *fds;
  പൂർണ്ണസംഖ്യ pidfd;
  പൂർണ്ണസംഖ്യ owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
open_destination_nofollow (സ്ഥിരം അക്ഷരം *destination, bool dir, libcrun_error_t *err)
{
  സ്ഥിരം uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  പൂർണ്ണസംഖ്യ fd;
  പൂർണ്ണസംഖ്യ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  എങ്കിൽ (LIKELY (fd >= 0))
    തിരികെ fd;

  എങ്കിൽ (errno != ENOENT)
    തിരികെ crun_make_error (err, errno, "open `%s`", destination);

  എങ്കിൽ (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  അല്ലെങ്കിൽ
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", destination);

  തിരികെ fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
create_destinations_in_a_container (ശൂന്യം *arg, libcrun_error_t *err)
{
  ഘടന mount_in_a_container_args *args = arg;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  എങ്കിൽ (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "setresuid to `0`");
    }
  അല്ലെങ്കിൽ എങ്കിൽ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      തിരികെ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  വേണ്ടി (i = 0; i < args->len; i++)
    {
      cleanup_close പൂർണ്ണസംഖ്യ dest_fd = -1;
      ഘടന stat st;

      ret = fstat (args->fds->fds[i], &st);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      എങ്കിൽ (UNLIKELY (dest_fd < 0))
        തിരികെ dest_fd;
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_mount_in_a_container (ശൂന്യം *arg, libcrun_error_t *err)
{
  ഘടന mount_in_a_container_args *args = arg;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  വേണ്ടി (i = 0; i < args->len; i++)
    {
      cleanup_close പൂർണ്ണസംഖ്യ dest_fd = -1;
      ഘടന stat st;

      ret = fstat (args->fds->fds[i], &st);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      എങ്കിൽ (UNLIKELY (dest_fd < 0))
        തിരികെ dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  തിരികെ 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
send_mounts_from_container_namespaces (libcrun_container_t *container, പൂർണ്ണസംഖ്യ pidfd, runtime_spec_schema_defs_mount **mounts,
                                       ഘടന libcrun_fd_map *fds, size_t len, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ owner_fd, പൂർണ്ണസംഖ്യ sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  എങ്കിൽ (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  അല്ലെങ്കിൽ എങ്കിൽ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      തിരികെ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  എങ്കിൽ (UNLIKELY (pid < 0))
    തിരികെ crun_make_error (err, errno, "fork");

  എങ്കിൽ (pid)
    {
      പൂർണ്ണസംഖ്യ wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      എങ്കിൽ (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, ശൂന്യ, 0));
          തിരികെ crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  വേണ്ടി (i = 0; i < len; i++)
    {
      cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

      എങ്കിൽ (fds->fds[i] >= 0)
        തുടരുക;

      fd = open_mount_of_type (container, mounts[i], err);
      എങ്കിൽ (UNLIKELY (fd < 0))
        തിരികെ crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, പൂർണ്ണസംഖ്യ *flags, libcrun_error_t *err)
{
  സ്ഥിതം സ്ഥിരം ഘടന
  {
    സ്ഥിരം അക്ഷരം *name;
    പൂർണ്ണസംഖ്യ flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  വേണ്ടി (i = 0; i < വലുപ്പം (namespaces) / വലുപ്പം (namespaces[0]); i++)
    {
      cleanup_close പൂർണ്ണസംഖ്യ self_fd = -1;
      cleanup_close പൂർണ്ണസംഖ്യ target_fd = -1;
      ഘടന stat self_st, target_st;
      cleanup_free അക്ഷരം *self_path = ശൂന്യ;
      പൂർണ്ണസംഖ്യ ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      എങ്കിൽ (UNLIKELY (self_fd < 0))
        തിരികെ self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      എങ്കിൽ (UNLIKELY (target_fd < 0))
        തിരികെ target_fd;

      ret = fstat (self_fd, &self_st);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      എങ്കിൽ (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  തിരികെ 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, പൂർണ്ണസംഖ്യ owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, ഘടന libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ sync_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ pidfd = -1;
  പൂർണ്ണസംഖ്യ sync_socket_fd[2];
  bool needed = false;
  പൂർണ്ണസംഖ്യ flags;
  pid_t pid;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  വേണ്ടി (i = 0; i < len; i++)
    {
      എങ്കിൽ (fds->fds[i] >= 0)
        തുടരുക;

      എങ്കിൽ (mounts[i]->type == ശൂന്യ)
        തിരികെ crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  എങ്കിൽ (! needed)
    തിരികെ 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  എങ്കിൽ (UNLIKELY (pidfd < 0))
    തിരികെ crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (ret == 0)
    തിരികെ crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  എങ്കിൽ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      തിരികെ ret;
    }

  എങ്കിൽ (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  വേണ്ടി (i = 0; i < len; i++)
    {
      എങ്കിൽ (fds->fds[i] >= 0)
        തുടരുക;

      ret = receive_fd_from_socket (sync_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          cleanup_free അക്ഷരം *err_str = ശൂന്യ;
          പൂർണ്ണസംഖ്യ err_code = 0;

          എങ്കിൽ (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          അല്ലെങ്കിൽ
            ret = crun_error_wrap (err, "receive mount fd");
          നിർത്തുക;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, ശൂന്യ, 0));

  തിരികെ ret;
}

സ്ഥിതം unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free അക്ഷരം *data = ശൂന്യ;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  എങ്കിൽ (mount->options == ശൂന്യ)
    തിരികെ get_default_flags (container, mount->destination, &data);

  വേണ്ടി (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  തിരികെ flags;
}

പൂർണ്ണസംഖ്യ
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ഘടന mount_in_a_container_args args;
  cleanup_close_map ഘടന libcrun_fd_map *fds = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ pidfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ mntns_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  fds = make_libcrun_fd_map (len);

  വേണ്ടി (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), ശൂന്യ, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      എങ്കിൽ (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          എങ്കിൽ (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          എങ്കിൽ (UNLIKELY (fds->fds[i] < 0))
            തിരികെ fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  വേണ്ടി (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  എങ്കിൽ (UNLIKELY (ret > 0))
    തിരികെ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

പൂർണ്ണസംഖ്യ
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  ഘടന umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  തിരികെ run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

ഘടന restrict_mounts_in_a_container_args
{
  ഘടന libcrun_mount_flags_s *mounts;
  size_t len;
  പൂർണ്ണസംഖ്യ rootfd;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_restrict_mounts_in_a_container (ശൂന്യം *arg, libcrun_error_t *err)
{
  ഘടന restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  എങ്കിൽ (LIKELY (ret == 0))
    ret = chroot (".");
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "chroot to the container root");

  വേണ്ടി (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      സ്ഥിരം അക്ഷരം *target = consume_slashes (args->mounts[i].target);
      cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
      ഘടന statfs sfs;
      unsigned long current_flags;

      എങ്കിൽ (flags == 0)
        തുടരുക;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      എങ്കിൽ (UNLIKELY (fd < 0))
        {
          എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              തുടരുക;
            }
          തിരികെ fd;
        }

      ret = fstatfs (fd, &sfs);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      എങ്കിൽ ((current_flags & flags) == flags)
        തുടരുക;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      എങ്കിൽ (LIKELY (ret == 0))
        തുടരുക;

      എങ്കിൽ (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        തിരികെ ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (ശൂന്യ, target, ശൂന്യ, MS_REMOUNT | MS_BIND | flags | current_flags, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "remount `/%s`", target);
    }

  തിരികെ 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
പൂർണ്ണസംഖ്യ
libcrun_restrict_mounts_in_container (pid_t pid, ഘടന libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  ഘടന restrict_mounts_in_a_container_args args;
  cleanup_close പൂർണ്ണസംഖ്യ mntns_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ rootfd = -1;
  അക്ഷരം path[64];
  പൂർണ്ണസംഖ്യ ret;

  snprintf (path, വലുപ്പം (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (mntns_fd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, വലുപ്പം (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (rootfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  എങ്കിൽ (UNLIKELY (ret > 0))
    തിരികെ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close പൂർണ്ണസംഖ്യ netns_fd = -1;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (def == ശൂന്യ || def->linux == ശൂന്യ || def->linux->net_devices == ശൂന്യ)
    തിരികെ 0;

  cleanup_free അക്ഷരം *ns_path = ശൂന്യ;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  എങ്കിൽ (UNLIKELY (netns_fd < 0))
    തിരികെ netns_fd;

  വേണ്ടി (i = 0; i < def->linux->net_devices->len; i++)
    {
      സ്ഥിരം അക്ഷരം *new_name = def->linux->net_devices->മൂല്യങ്ങൾ[i]->name ?: def->linux->net_devices->താക്കോലുകൾ[i];

      ret = move_network_device (def->linux->net_devices->താക്കോലുകൾ[i], new_name, netns_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}
