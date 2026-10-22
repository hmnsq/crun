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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "linux.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "status.h"
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <sched.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <sys/mount.h>
#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#যদি defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#endif

#অন্তর্ভাৱ <sys/prctl.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#অন্তর্ভাৱ <sys/time.h>
#অন্তর্ভাৱ <sys/resource.h>
#অন্তর্ভাৱ <sys/sysmacros.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <grp.h>
#অন্তর্ভাৱ <signal.h>
#অন্তর্ভাৱ "terminal.h"
#অন্তর্ভাৱ "cgroup.h"
#অন্তর্ভাৱ "cgroup-utils.h"
#অন্তর্ভাৱ "status.h"
#অন্তর্ভাৱ "criu.h"
#অন্তর্ভাৱ "scheduler.h"
#অন্তর্ভাৱ "intelrdt.h"
#অন্তর্ভাৱ "io_priority.h"
#অন্তর্ভাৱ "net_device.h"

#অন্তর্ভাৱ <sys/socket.h>
#অন্তর্ভাৱ <libgen.h>
#অন্তর্ভাৱ <sys/wait.h>
#অন্তর্ভাৱ <sys/vfs.h>
#অন্তর্ভাৱ <sys/statvfs.h>
#অন্তর্ভাৱ <limits.h>
#অন্তর্ভাৱ <inttypes.h>
#অন্তর্ভাৱ <sys/personality.h>
#অন্তর্ভাৱ <net/if.h>
#অন্তর্ভাৱ <sys/xattr.h>
#অন্তর্ভাৱ <linux/netlink.h>
#অন্তর্ভাৱ <linux/rtnetlink.h>
#অন্তর্ভাৱ <sched.h>
#অন্তর্ভাৱ <linux/sched.h>
#অন্তর্ভাৱ <linux/magic.h>
#অন্তর্ভাৱ <linux/nsfs.h>

#অন্তর্ভাৱ <json-c/json.h>
#অন্তর্ভাৱ <ocispec/json_common.h>

#অন্তর্ভাৱ "mount_flags.h"
#অন্তর্ভাৱ "syscalls.h"

#ifndef RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#endif

#ifndef OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#endif

#ifndef OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#endif

#ifndef OPEN_বৃক্ষ_NAMESPACE
#  define OPEN_বৃক্ষ_NAMESPACE 2
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
স্থির unsigned long
mount_flags_from_statfs (unsigned long f_flags)
{
  unsigned long flags = 0;

  যদি (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  যদি (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  যদি (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  যদি (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  যদি (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  যদি (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  যদি (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  যদি (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  যদি (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  ঘূৰাই flags;
}

সংরচনা remount_s
{
  সংরচনা remount_s *next;
  ৱর্ণ *target;
  পূর্ণাঙ্ক targetfd;
  unsigned long flags;
  ৱর্ণ *data;
};

সংরচনা private_data_s
{
  সংরচনা remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  পূর্ণাঙ্ক unshare_flags;
  পূর্ণাঙ্ক unshare_cgroupns;

  ৱর্ণ *host_notify_socket_path;
  ৱর্ণ *container_notify_socket_path;
  ৱর্ণ *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  পূর্ণাঙ্ক procfd;

  ধ্রুৱ ৱর্ণ *rootfs;
  পূর্ণাঙ্ক rootfsfd;

  পূর্ণাঙ্ক notify_socket_বৃক্ষ_fd;
  পূর্ণাঙ্ক old_মূল_fd;

  সংরচনা libcrun_fd_map *mount_fds;
  সংরচনা libcrun_fd_map *dev_fds;
  সংরচনা libcrun_fd_map *needed_devs_fds;

  ৱর্ণ **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  ৱর্ণ *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  পূর্ণাঙ্ক maskdir_fd;
  ৱর্ণ *maskdir_proc_path;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_pivot;
  bool no_pivot;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_মূল_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  ৱর্ণ *host_rootfs;
};

সংরচনা linux_namespace_s
{
  ধ্রুৱ ৱর্ণ *name;
  ধ্রুৱ ৱর্ণ *ns_file;
  পূর্ণাঙ্ক মান;
};

স্থির শূন্য
cleanup_private_data (শূন্য *private_data)
{
  সংরচনা private_data_s *p = private_data;

  যদি (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  যদি (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  যদি (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  যদি (p->old_মূল_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_মূল_fd));
  যদি (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  যদি (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  যদি (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  যদি (p->copy_symlink_targets)
    {
      size_t i;
      বাবে (i = 0; i < p->n_copy_symlink_targets; i++)
        মুক্ত (p->copy_symlink_targets[i]);
      মুক্ত (p->copy_symlink_targets);
    }
  মুক্ত (p->unified_cgroup_path);
  মুক্ত (p->host_notify_socket_path);
  মুক্ত (p->container_notify_socket_path);
  মুক্ত (p->external_descriptors);
  মুক্ত (p->maskdir_proc_path);
  মুক্ত (p->host_rootfs);
  মুক্ত (p);
}

স্থির সংরচনা private_data_s *
get_private_data (সংরচনা libcrun_container_s *container)
{
  যদি (container->private_data == রিক্ত)
    {
      সংরচনা private_data_s *p = xmalloc0 (পরিমাণ (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_বৃক্ষ_fd = -1;
      p->old_মূল_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  ঘূৰাই container->private_data;
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

স্থির সংরচনা linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { রিক্ত, রিক্ত, 0 } };

স্থির পূর্ণাঙ্ক
get_and_reset (পূর্ণাঙ্ক *old)
{
  পূর্ণাঙ্ক tmp = *old;
  *old = -1;
  ঘূৰাই tmp;
}

পূর্ণাঙ্ক
libcrun_find_namespace (ধ্রুৱ ৱর্ণ *name)
{
  সংরচনা linux_namespace_s *it;
  বাবে (it = namespaces; it->name; it++)
    যদি (strcmp (it->name, name) == 0)
      ঘূৰাই it->মান;
  ঘূৰাই -1;
}

স্থির পূর্ণাঙ্ক
syscall_keyctl_join (ধ্রুৱ ৱর্ণ *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  ঘূৰাই (পূর্ণাঙ্ক) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

স্থির পূর্ণাঙ্ক
syscall_pidfd_open (pid_t pid, unsigned পূর্ণাঙ্ক flags)
{
#যদি defined __NR_pidfd_open
  ঘূৰাই (পূর্ণাঙ্ক) syscall (__NR_pidfd_open, pid, flags);
#অন্যথা
  (শূন্য) pid;
  (শূন্য) flags;
  errno = ENOSYS;
  ঘূৰাই -1;
#endif
}

স্থির পূর্ণাঙ্ক
syscall_pidfd_send_signal (পূর্ণাঙ্ক pidfd, পূর্ণাঙ্ক sig, siginfo_t *info, unsigned পূর্ণাঙ্ক flags)
{
#যদি defined __NR_pidfd_send_signal
  ঘূৰাই (পূর্ণাঙ্ক) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#অন্যথা
  (শূন্য) pidfd;
  (শূন্য) sig;
  (শূন্য) info;
  (শূন্য) flags;
  errno = ENOSYS;
  ঘূৰাই -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
স্থির uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  যদি (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  যদি (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  যদি (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  যদি (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  যদি (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  যদি (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  যদি (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  যদি (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  ঘূৰাই attr;
}

স্থির পূর্ণাঙ্ক
do_mount_setattr (bool recursive, ধ্রুৱ ৱর্ণ *target, পূর্ণাঙ্ক targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  সংরচনা mount_attr_s attr = {
    0,
  };
  পূর্ণাঙ্ক ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  যদি (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      যদি (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      অন্যথা যদি (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      অন্যথা
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      যদি (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  অন্যথা যদি (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "mount_setattr `/%s`", target);

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
get_bind_mount (পূর্ণাঙ্ক dirfd, ধ্রুৱ ৱর্ণ *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক open_বৃক্ষ_fd = -1;
  সংরচনা mount_attr_s attr = {
    0,
  };
  পূর্ণাঙ্ক recursive_flag = (recursive ? AT_RECURSIVE : 0);
  পূর্ণাঙ্ক ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  যদি (UNLIKELY (src == রিক্ত))
    ঘূৰাই crun_make_error (err, EINVAL, "mount source is not specified");

  যদি (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  যদি (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_বৃক্ষ_fd = syscall_open_বৃক্ষ (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  যদি (UNLIKELY (open_বৃক্ষ_fd < 0))
    ঘূৰাই crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_বৃক্ষ_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "mount_setattr `%s`", src);

  ঘূৰাই get_and_reset (&open_বৃক্ষ_fd);
}

পূর্ণাঙ্ক
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, ৱর্ণ *option, ৱর্ণ **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free ৱর্ণ *mappings = রিক্ত;
  ধ্রুৱ ৱর্ণ *it;
  পূর্ণাঙ্ক ret;

  mappings = xmalloc (allocated);

  বাবে (it = option; *it;)
    {
      bool relative = false;
      long মান[3];
      size_t i;

      যদি (*it == '\0')
        ভঙ্গ;

      যদি (*it == '#')
        it++;

      যদি (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      বাবে (i = 0; i < 3; i++)
        {
          ৱর্ণ *endptr = রিক্ত;

          যদি (i > 0 && *it == '-')
            it++;

          যদি (*it == '\0')
            ঘূৰাই crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          মান[i] = strtol (it, &endptr, 10);
          যদি (errno || endptr == it)
            ঘূৰাই crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      যদি (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          যদি (def == রিক্ত
              || def->linux == রিক্ত
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            ঘূৰাই crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          বাবে (i = 0; i < mappings_len; i++)
            যদি (মান[1] >= (int64_t) mappings[i]->container_id
                && মান[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              ভঙ্গ;

          যদি (i == mappings_len)
            ঘূৰাই crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          মান[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      যদি (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", মান[0], মান[1], মান[2]);
      যদি (UNLIKELY (ret >= (পূর্ণাঙ্ক) (allocated - written)))
        ঘূৰাই crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = রিক্ত;

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
format_mount_mappings (ৱর্ণ **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  ধ্রুৱ size_t max_len_mapping = 64;
  cleanup_free ৱর্ণ *ret = রিক্ত;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  বাবে (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      যদি (UNLIKELY (len >= max_len_mapping))
        ঘূৰাই crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = রিক্ত;
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
format_mount_mapping (ৱর্ণ **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t size, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .size = size,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    রিক্ত,
  };

  ঘূৰাই format_mount_mappings (ret, mappings, 1, written, err);
}

স্থির bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  যদি (def->linux == রিক্ত)
    ঘূৰাই mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  যদি (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    ঘূৰাই false;

  যদি (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    ঘূৰাই false;

  বাবে (s = 0; s < mnt->uid_mappings_len; s++)
    {
      যদি (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        ঘূৰাই false;
      যদি (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        ঘূৰাই false;
      যদি (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        ঘূৰাই false;
    }

  বাবে (s = 0; s < mnt->gid_mappings_len; s++)
    {
      যদি (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        ঘূৰাই false;
      যদি (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        ঘূৰাই false;
      যদি (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        ঘূৰাই false;
    }

  ঘূৰাই true;
}

স্থির pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        ধ্রুৱ ৱর্ণ *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != রিক্ত;

  যদি (! need_new_userns)
    ঘূৰাই 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, রিক্ত);
  যদি (UNLIKELY (pid < 0))
    ঘূৰাই crun_make_error (err, errno, "clone");

  যদি (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      যেতিয়ালৈ (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  যদি (mnt->uid_mappings_len)
    {
      cleanup_free ৱর্ণ *uid_map = রিক্ত;
      cleanup_free ৱর্ণ *gid_map = রিক্ত;
      size_t written = 0;
      পূর্ণাঙ্ক ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      cleanup_close পূর্ণাঙ্ক fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      যদি (UNLIKELY (fd < 0))
        ঘূৰাই fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      যদি (UNLIKELY (fd < 0))
        ঘূৰাই fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  অন্যথা
    {
      cleanup_free ৱর্ণ *dup_options = রিক্ত;
      ৱর্ণ *option, *saveptr = রিক্ত;

      যদি (! options)
        ঘূৰাই crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      বাবে (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (রিক্ত, ";", &saveptr))
        {
          cleanup_free ৱর্ণ *mappings = রিক্ত;
          cleanup_close পূর্ণাঙ্ক map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          পূর্ণাঙ্ক ret;

          যদি (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              যদি (UNLIKELY (map_fd < 0))
                ঘূৰাই map_fd;
            }
          অন্যথা যদি (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              যদি (UNLIKELY (map_fd < 0))
                ঘূৰাই map_fd;
            }
          অন্যথা
            ঘূৰাই crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_create_keyring (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *label, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক labelfd = -1;
  bool label_set = false;
  পূর্ণাঙ্ক ret;

  যদি (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      যদি (UNLIKELY (labelfd < 0))
        {
          যদি (crun_error_get_errno (err) != ENOENT)
            ঘূৰাই labelfd;

          crun_error_release (err);
        }

      যদি (labelfd >= 0)
        {
          ret = লিখ (labelfd, label, strlen (label));
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          গচ্ছ out;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      গচ্ছ out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  যদি (label_set && লিখ (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  ঘূৰাই ret;
}

স্থির শূন্য
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  যদি (def->process && def->process->user)
    {
      যদি (def->process->user->uid)
        *uid = def->process->user->uid;
      যদি (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

স্থির unsigned long
get_mount_flags (ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক current_flags, পূর্ণাঙ্ক *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  ধ্রুৱ সংরচনা propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  যদি (found)
    *found = prop ? 1 : 0;

  যদি (prop == রিক্ত)
    ঘূৰাই 0;

  যদি (prop->extra_flags & OPTION_RECURSIVE)
    {
      যদি (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      যদি (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  যদি (extra_flags)
    *extra_flags |= prop->extra_flags;

  যদি (prop->clear)
    ঘূৰাই current_flags & ~prop->flags;

  ঘূৰাই current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
স্থির bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  বাবে (i = 0; i < mount->options_len; i++)
    {
      ধ্রুৱ সংরচনা propagation_flags_s *prop;

      যদি (mount->options[i] == রিক্ত)
        অনুৱর্ত;

      prop = libcrun_str2mount_flags (mount->options[i]);
      যদি (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        ঘূৰাই true;
    }

  ঘূৰাই false;
}

স্থির unsigned long
get_mount_flags_or_option (ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক current_flags, unsigned long *extra_flags, ৱর্ণ **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  পূর্ণাঙ্ক found;
  __attribute__ ((unused)) cleanup_free ৱর্ণ *prev = রিক্ত;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  যদি (name == রিক্ত)
    ঘূৰাই 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  যদি (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      ধ্রুৱ ৱর্ণ *data_flag = রিক্ত;
      যদি ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      অন্যথা যদি ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      যদি (data_flag)
        {
          prev = *option;
          যদি (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          অন্যথা
            *option = xstrdup (data_flag);
        }
      ঘূৰাই flags;
    }

  prev = *option;
  যদি (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  অন্যথা
    *option = xstrdup (name);

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
pivot_মূল (ধ্রুৱ ৱর্ণ *new_মূল, ধ্রুৱ ৱর্ণ *put_old)
{
  ঘূৰাই syscall (__NR_pivot_root, new_মূল, put_old);
}

স্থির শূন্য
free_remount (সংরচনা remount_s *r)
{
  যদি (r == রিক্ত)
    ঘূৰাই;
  যদি (r->targetfd >= 0)
    close (r->targetfd);
  মুক্ত (r->data);
  মুক্ত (r->target);
  মুক্ত (r);
}

স্থির সংরচনা remount_s *
make_remount (পূর্ণাঙ্ক targetfd, ধ্রুৱ ৱর্ণ *target, unsigned long flags, ধ্রুৱ ৱর্ণ *data, সংরচনা remount_s *next)
{
  সংরচনা remount_s *ret = xmalloc (পরিমাণ (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : রিক্ত;
  ret->next = next;
  ret->targetfd = targetfd;
  ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক
do_remount (পূর্ণাঙ্ক targetfd, ধ্রুৱ ৱর্ণ *target, unsigned long flags, ধ্রুৱ ৱর্ণ *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  পূর্ণাঙ্ক ret;
  ধ্রুৱ ৱর্ণ *real_target = target;

  যদি (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      যদি (LIKELY (ret == 0))
        ঘূৰাই 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  যদি (flags & (MS_REMOUNT | MS_RDONLY))
    data = রিক্ত;

  ret = mount (রিক্ত, real_target, রিক্ত, flags, data);
  যদি (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      সংরচনা statfs sfs;

      ret = statfs (real_target, &sfs);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      যদি (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      যদি ((flags | remount_flags) != flags)
        {
          ret = mount (রিক্ত, real_target, রিক্ত, flags | remount_flags, data);
          যদি (LIKELY (ret == 0))
            ঘূৰাই 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          যদি (current_flags & MS_RDONLY)
            ret = mount (রিক্ত, real_target, রিক্ত, flags | remount_flags | MS_RDONLY, data);
        }
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "remount `%s`", target);
    }
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret = 0;
  সংরচনা remount_s *r = get_private_data (container)->remounts;
  যেতিয়ালৈ (r)
    {
      সংরচনা remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      যদি (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          যদি (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              অনুৱর্ত;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  যেতিয়ালৈ (r)
    {
      সংরচনা remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = রিক্ত;
  ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক
open_mount_target (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *target_rel, libcrun_error_t *err)
{
  ধ্রুৱ ৱর্ণ *rootfs = get_private_data (container)->rootfs;
  পূর্ণাঙ্ক rootfsfd = get_private_data (container)->rootfsfd;

  যদি (rootfsfd < 0)
    ঘূৰাই crun_make_error (err, 0, "invalid rootfs state");

  ঘূৰাই safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
স্থির পূর্ণাঙ্ক
fsopen_mount (ধ্রুৱ ৱর্ণ *type, ধ্রুৱ ৱর্ণ *source_name, ধ্রুৱ ৱর্ণ *labeltype, ধ্রুৱ ৱর্ণ *label, ধ্রুৱ ৱর্ণ *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close পূর্ণাঙ্ক fsfd = -1;
  পূর্ণাঙ্ক ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  যদি (UNLIKELY (fsfd < 0))
    ঘূৰাই fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (শূন্য) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  যদি (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  যদি (data && data[0])
    {
      cleanup_free ৱর্ণ *data_copy = xstrdup (data);
      ৱর্ণ *saveptr = রিক্ত;
      ৱর্ণ *token;

      বাবে (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (রিক্ত, ",", &saveptr))
        {
          ৱর্ণ *eq = strchr (token, '=');
          যদি (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          অন্যথা
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, রিক্ত, 0);

          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, রিক্ত, রিক্ত, 0);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#অন্যথা
  (শূন্য) type;
  (শূন্য) source_name;
  (শূন্য) data;
  errno = ENOSYS;
  ঘূৰাই -1;
#endif
}

স্থির পূর্ণাঙ্ক
get_procfd (সংরচনা private_data_s *data, libcrun_error_t *err)
{
  পূর্ণাঙ্ক fd;

  যদি (data->procfd >= 0)
    ঘূৰাই data->procfd;

  fd = fsopen_mount ("proc", রিক্ত, রিক্ত, রিক্ত, রিক্ত);
  যদি (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  ঘূৰাই fd;
}

স্থির পূর্ণাঙ্ক
get_old_মূল_fd (সংরচনা private_data_s *data)
{
  যদি (data->old_মূল_fd >= 0)
    ঘূৰাই data->old_মূল_fd;

  data->old_মূল_fd = open ("/", O_PATH | O_CLOEXEC);
  ঘূৰাই data->old_মূল_fd;
}

স্থির পূর্ণাঙ্ক
fs_move_mount_to (পূর্ণাঙ্ক fd, পূর্ণাঙ্ক dirfd, ধ্রুৱ ৱর্ণ *name)
{
#ifdef HAVE_NEW_MOUNT_API
  যদি (name)
    ঘূৰাই syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  ঘূৰাই syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#অন্যথা
  (শূন্য) fd;
  (শূন্য) dirfd;
  (শূন্য) name;
  (শূন্য) syscall_move_mount;
  errno = ENOSYS;
  ঘূৰাই -1;
#endif
}

গণনা
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

স্থির পূর্ণাঙ্ক do_mount (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *source, পূর্ণাঙ্ক targetfd,
                     ধ্রুৱ ৱর্ণ *target, ধ্রুৱ ৱর্ণ *fstype, unsigned long mountflags,
                     ধ্রুৱ শূন্য *data, পূর্ণাঙ্ক extra_flags, libcrun_error_t *err);

স্থির bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, ধ্রুৱ ৱর্ণ *option)
{
  size_t i;
  বাবে (i = 0; i < mnt->options_len; i++)
    যদি (mnt->options[i] != রিক্ত && strcmp (mnt->options[i], option) == 0)
      ঘূৰাই true;
  ঘূৰাই false;
}

স্থির runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  বাবে (i = 0; i < def->mounts_len; i++)
    {
      যদি (strcmp (def->mounts[i]->destination, destination) == 0)
        ঘূৰাই def->mounts[i];
    }
  ঘূৰাই রিক্ত;
}

স্থির শূন্য
warn_tmpfs_fallback_once (সংরচনা private_data_s *private_data, ধ্রুৱ ৱর্ণ *reason)
{
  যদি (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
স্থির পূর্ণাঙ্ক
get_shared_empty_dir_cached (libcrun_container_t *container, ৱর্ণ **proc_fd_path, libcrun_error_t *err)
{
  সংরচনা private_data_s *private_data = get_private_data (container);
  cleanup_close পূর্ণাঙ্ক fd = -1;
  cleanup_free ৱর্ণ *empty_dir_path = রিক্ত;
  proc_fd_path_t fd_path;
  পূর্ণাঙ্ক ret;

  /* Fast path: return cached proc fd path if already set up */
  যদি (private_data->maskdir_proc_path != রিক্ত)
    {
      যদি (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      ঘূৰাই 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_মূল, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  যদি (fd < 0)
    ঘূৰাই fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  যদি (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  ঘূৰাই 0;
}

স্থির bool
is_dev_null (ধ্রুৱ সংরচনা stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  ঘূৰাই S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
স্থির পূর্ণাঙ্ক
ensure_dev_null (পূর্ণাঙ্ক fd, libcrun_error_t *err)
{
  সংরচনা stat st;

  যদি (UNLIKELY (fstat (fd, &st) < 0))
    ঘূৰাই crun_make_error (err, errno, "stat `/dev/null`");

  যদি (UNLIKELY (! is_dev_null (&st)))
    ঘূৰাই crun_make_error (err, 0, "`/dev/null` is not the null device");

  ঘূৰাই 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
স্থির bool
is_empty_ro_tmpfs (পূর্ণাঙ্ক fd)
{
  সংরচনা statfs sfs;

  যদি (fstatfs (fd, &sfs) < 0)
    ঘূৰাই false;

  ঘূৰাই sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

স্থির পূর্ণাঙ্ক
mount_masked_dir (libcrun_container_t *container, পূর্ণাঙ্ক pathfd, ধ্রুৱ ৱর্ণ *rel_path, libcrun_error_t *err)
{
  সংরচনা private_data_s *private_data = get_private_data (container);
  cleanup_close পূর্ণাঙ্ক newfd = -1;
  libcrun_error_t tmp_err = রিক্ত;
  পূর্ণাঙ্ক ret;

  যদি (private_data->maskdir_fd < 0)
    {
      যদি (private_data->maskdir_bind_failed)
        গচ্ছ fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, রিক্ত, &tmp_err);
      যদি (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->বাৰ্তা);
          crun_error_release (&tmp_err);
          গচ্ছ fallback_to_tmpfs;
        }
    }

  যদি (private_data->maskdir_fd >= 0)
    {
      পূর্ণাঙ্ক rootfsfd = private_data->rootfsfd;
      সংরচনা stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, রিক্ত);
      যদি (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          যদি (UNLIKELY (newfd < 0))
            ঘূৰাই newfd;

          ret = fstat (newfd, &after);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          যদি (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            ঘূৰাই crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          ঘূৰাই 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_path);

      TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
      private_data->maskdir_fd = -1;
      private_data->maskdir_move_failed = true;
    }

  private_data->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_data, tmp_err ? tmp_err->বাৰ্তা : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  যদি (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      যদি (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      অন্যথা
        {
          যদি (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
do_masked_or_readonly_path (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  ধ্রুৱ ৱর্ণ *rootfs = get_private_data (container)->rootfs;
  cleanup_close পূর্ণাঙ্ক pathfd = -1;
  সংরচনা statfs sfs;
  পূর্ণাঙ্ক ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  যদি (UNLIKELY (rel_path == রিক্ত))
    ঘূৰাই crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  যদি (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  যদি (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      যদি (errno != ENOENT && errno != EACCES)
        ঘূৰাই pathfd;

      যদি (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      ঘূৰাই 0;
    }

  যদি (readonly)
    {
      পূর্ণাঙ্ক procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close পূর্ণাঙ্ক mountfd = -1;

      যদি (UNLIKELY (procfd < 0))
        ঘূৰাই procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      যদি (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          যদি (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, রিক্ত);
          অন্যথা
            ret = -1;

          যদি (ret < 0)
            crun_error_release (err);
        }

      যদি (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          যদি (keep_flags)
            {
              cleanup_close পূর্ণাঙ্ক sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              যদি (UNLIKELY (sfd < 0))
                ঘূৰাই crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              যদি (UNLIKELY (ret < 0))
                ঘূৰাই crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          যদি (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              যদি (UNLIKELY (ret < 0))
                ঘূৰাই ret;

              ret = fs_move_mount_to (mountfd, pathfd, রিক্ত);
              যদি (UNLIKELY (ret < 0))
                ঘূৰাই crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          অন্যথা
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, রিক্ত, mount_flags, রিক্ত,
                              LABEL_NONE, err);
              যদি (UNLIKELY (ret < 0))
                ঘূৰাই ret;
            }
        }
    }
  অন্যথা
    {
      ret = get_file_type_fd (pathfd, &mode);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      যদি ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      অন্যথা
        {
          cleanup_close পূর্ণাঙ্ক nullfd = -1;
          cleanup_close পূর্ণাঙ্ক mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          যদি (UNLIKELY (nullfd < 0))
            ঘূৰাই crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          যদি (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, রিক্ত);

          যদি (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, রিক্ত, MS_BIND | MS_RDONLY, রিক্ত,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  ঘূৰাই 0;
}

স্থির ধ্রুৱ ৱর্ণ *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  ধ্রুৱ ৱর্ণ *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  যদি (context_type)
    {
      যদি (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          ঘূৰাই রিক্ত;
        }
      ঘূৰাই context_type;
    }

  ঘূৰাই "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
স্থির পূর্ণাঙ্ক
diagnose_mount_failure (পূর্ণাঙ্ক ret, libcrun_error_t *err, libcrun_container_t *container,
                        ধ্রুৱ ৱর্ণ *fstype)
{
  যদি (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      যদি (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          ঘূৰাই ret;
        }
    }
  ঘূৰাই ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
স্থির পূর্ণাঙ্ক
sysfs_userns_fallback (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *target,
                       ধ্রুৱ ৱর্ণ *real_target, পূর্ণাঙ্ক targetfd,
                       unsigned long mountflags, ধ্রুৱ ৱর্ণ *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close পূর্ণাঙ্ক mountfd = -1;
  cleanup_close পূর্ণাঙ্ক sysfd = -1;
  পূর্ণাঙ্ক sys_old_মূল_fd = -1;
  পূর্ণাঙ্ক ret;

  যদি (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    ঘূৰাই 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (ret == 0)
    ঘূৰাই 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_মূল_fd = get_old_মূল_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  যদি (sys_old_মূল_fd >= 0)
    mountfd = get_bind_mount (sys_old_মূল_fd, "sys", true, true, false, MS_PRIVATE, err);
  যদি (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  যদি (cgroup_mount == রিক্ত)
    {
      যদি (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, রিক্ত);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      অন্যথা
        {
          crun_error_release (err);
          যদি (sys_old_মূল_fd >= 0)
            {
              cleanup_free ৱর্ণ *sys_path = রিক্ত;
              proc_fd_path_t sys_মূল_path;

              get_proc_self_fd_path (sys_মূল_path, sys_old_মূল_fd);
              xasprintf (&sys_path, "%s/sys", sys_মূল_path);
              ret = mount (sys_path, real_target, রিক্ত, MS_BIND | MS_REC, রিক্ত);
            }
          অন্যথা
            ret = mount ("/sys", real_target, রিক্ত, MS_BIND | MS_REC, রিক্ত);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      যদি (UNLIKELY (sysfd < 0))
        ঘূৰাই sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        রিক্ত, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ঘূৰাই do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  যদি (UNLIKELY (mountfd < 0))
    ঘূৰাই mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  যদি (UNLIKELY (sysfd < 0))
    ঘূৰাই sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ঘূৰাই 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
স্থির শূন্য
apply_selinux_xattr (libcrun_container_t *container, পূর্ণাঙ্ক fd, পূর্ণাঙ্ক extra_flags, ধ্রুৱ ৱর্ণ *label)
{
  libcrun_error_t tmp_err = রিক্ত;
  পূর্ণাঙ্ক procfd;

  যদি ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    ঘূৰাই;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  যদি (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close পূর্ণাঙ্ক xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      যদি (xfd >= 0)
        (শূন্য) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
স্থির পূর্ণাঙ্ক
do_mount_replace_rootfs (libcrun_container_t *container, পূর্ণাঙ্ক *fd, libcrun_error_t *err)
{
  পূর্ণাঙ্ক procfd = get_procfd (get_private_data (container), err);
  পূর্ণাঙ্ক tmp;
  পূর্ণাঙ্ক ret;

  যদি (UNLIKELY (procfd < 0))
    ঘূৰাই procfd;

  {
    cleanup_close পূর্ণাঙ্ক nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    যদি (UNLIKELY (nsfd < 0))
      ঘূৰাই crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    যদি (UNLIKELY (ret < 0))
      ঘূৰাই crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (*fd < 0))
    ঘূৰাই crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  যদি (UNLIKELY (tmp < 0))
    ঘূৰাই crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  ঘূৰাই 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
স্থির পূর্ণাঙ্ক
apply_propagation_flags (ধ্রুৱ ৱর্ণ *target, পূর্ণাঙ্ক targetfd, ধ্রুৱ ৱর্ণ *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  পূর্ণাঙ্ক ret;

  যদি (targetfd >= 0)
    {
      libcrun_error_t tmp_err = রিক্ত;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      যদি (LIKELY (ret == 0))
        propagation_done = true;
      অন্যথা
        crun_error_release (&tmp_err);
    }

  যদি (! propagation_done)
    {
      ret = mount (রিক্ত, real_target, রিক্ত, mountflags & ALL_PROPAGATIONS, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  ঘূৰাই 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
স্থির পূর্ণাঙ্ক
schedule_or_do_remount (libcrun_container_t *container, পূর্ণাঙ্ক *fd, পূর্ণাঙ্ক targetfd,
                        ধ্রুৱ ৱর্ণ *target, ধ্রুৱ ৱর্ণ *real_target,
                        bool single_instance, unsigned long mountflags,
                        ধ্রুৱ শূন্য *data, পূর্ণাঙ্ক extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  সংরচনা remount_s *r;

  যদি ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    ঘূৰাই do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  যদি (*fd < 0)
    {
      *fd = dup (targetfd);
      যদি (UNLIKELY (*fd < 0))
        ঘূৰাই crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
do_mount (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *source, পূর্ণাঙ্ক targetfd,
          ধ্রুৱ ৱর্ণ *target, ধ্রুৱ ৱর্ণ *fstype, unsigned long mountflags, ধ্রুৱ শূন্য *data,
          পূর্ণাঙ্ক extra_flags, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *data_with_label = রিক্ত;
  ধ্রুৱ ৱর্ণ *data_without_label = data;
  proc_fd_path_t target_buffer;
  ধ্রুৱ ৱর্ণ *context_type = রিক্ত;
  cleanup_close পূর্ণাঙ্ক ms_move_fd = -1;
  ধ্রুৱ ৱর্ণ *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close পূর্ণাঙ্ক fd = -1;
  ধ্রুৱ ৱর্ণ *label = রিক্ত;
  পূর্ণাঙ্ক ret = 0;

  যদি (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  অন্যথা
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  যদি (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  যদি ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      যদি (UNLIKELY (context_type == রিক্ত))
        ঘূৰাই -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      যদি (ret < 0)
        ঘূৰাই ret;
      data = data_with_label;
    }

  যদি (mountflags & MS_MOVE)
    {
      যদি ((mountflags & MS_BIND) || fstype)
        ঘূৰাই crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, রিক্ত, MS_MOVE, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      যদি (UNLIKELY (ms_move_fd < 0))
        ঘূৰাই ms_move_fd;
      targetfd = ms_move_fd;
    }

  যদি ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      যদি (targetfd >= 0)
        {
          যদি (mountflags & MS_BIND)
            {
              cleanup_close পূর্ণাঙ্ক bindfd = -1;
              libcrun_error_t tmp_err = রিক্ত;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              যদি (bindfd < 0)
                {
                  পূর্ণাঙ্ক old_মূল_fd = get_old_মূল_fd (get_private_data (container));

                  যদি (old_মূল_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_মূল_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              যদি (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, রিক্ত);
              যদি (ret < 0)
                crun_error_release (&tmp_err);
            }
          অন্যথা
            {
              cleanup_close পূর্ণাঙ্ক newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              যদি (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, রিক্ত);
            }
        }

      যদি (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          যদি (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              পূর্ণাঙ্ক old_মূল_fd = get_old_মূল_fd (get_private_data (container));

              যদি (old_মূল_fd >= 0)
                {
                  cleanup_free ৱর্ণ *proc_source_buf = রিক্ত;
                  proc_fd_path_t old_মূল_path;

                  get_proc_self_fd_path (old_মূল_path, old_মূল_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_মূল_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      যদি (UNLIKELY (ret < 0))
        {
          পূর্ণাঙ্ক saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          যদি (ret == 0)
            ঘূৰাই 0;
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          ঘূৰাই diagnose_mount_failure (ret, err, container, fstype);
        }

      যদি (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          যদি (UNLIKELY (fd < 0))
            ঘূৰাই fd;

          /* We are replacing the rootfs, reopen it.  */
          যদি (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              যদি (UNLIKELY (ret < 0))
                ঘূৰাই ret;
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

  যদি (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  যদি (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  যদি (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  যদি (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ঘূৰাই ret;
}

স্থির শূন্য
try_umount (পূর্ণাঙ্ক targetfd, ধ্রুৱ ৱর্ণ *target)
{
  proc_fd_path_t target_buffer;
  ধ্রুৱ ৱর্ণ *real_target = target;

  যদি (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

স্থির bool
container_has_cgroupns (libcrun_container_t *container)
{
  ঘূৰাই get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

স্থির পূর্ণাঙ্ক
do_mount_cgroup_v2 (libcrun_container_t *container, পূর্ণাঙ্ক targetfd, ধ্রুৱ ৱর্ণ *target,
                    unsigned long mountflags, ধ্রুৱ ৱর্ণ *data, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ঘূৰাই cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  যদি (UNLIKELY (ret < 0))
    {
      পূর্ণাঙ্ক saved_errno = crun_error_get_errno (err);
      যদি (saved_errno == EPERM || saved_errno == EBUSY)
        {
          ধ্রুৱ ৱর্ণ *unified_cgroup_path;
          ধ্রুৱ ৱর্ণ *src_cgroup;

          crun_error_release (err);

          যদি (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              যদি (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  যদি (LIKELY (ret == 0))
                    ঘূৰাই ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_মূল;
          ঘূৰাই do_mount (container, src_cgroup, targetfd, target, রিক্ত,
                           MS_BIND | mountflags, রিক্ত, LABEL_NONE, err);
        }

      ঘূৰাই ret;
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *source, পূর্ণাঙ্ক targetfd, ধ্রুৱ ৱর্ণ *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_close পূর্ণাঙ্ক fd = -1;
  ধ্রুৱ ৱর্ণ *subsystem = "systemd";
  cleanup_free ৱর্ণ *subsystem_path = রিক্ত;
  cleanup_close পূর্ণাঙ্ক tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  যদি (UNLIKELY (tmpfsdirfd < 0))
    ঘূৰাই tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  যদি (UNLIKELY (fd < 0))
    ঘূৰাই crun_make_error (err, errno, "openat `%s`", subsystem_path);

  ঘূৰাই do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

স্থির পূর্ণাঙ্ক
do_mount_cgroup_v1 (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *source, পূর্ণাঙ্ক targetfd, ধ্রুৱ ৱর্ণ *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_free ৱর্ণ *content = রিক্ত;
  ৱর্ণ *from;
  cleanup_close পূর্ণাঙ্ক tmpfsdirfd = -1;
  ৱর্ণ *saveptr = রিক্ত;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  যদি (UNLIKELY (tmpfsdirfd < 0))
    ঘূৰাই tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    পূর্ণাঙ্ক procfd = get_procfd (get_private_data (container), err);
    যদি (UNLIKELY (procfd < 0))
      ঘূৰাই procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, রিক্ত, err);
  }
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (UNLIKELY (content == রিক্ত || content[0] == '\0'))
    ঘূৰাই crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  বাবে (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (রিক্ত, "\n", &saveptr))
    {
      cleanup_free ৱর্ণ *source_path = রিক্ত;
      cleanup_free ৱর্ণ *source_subsystem = রিক্ত;
      cleanup_free ৱর্ণ *subsystem_path = রিক্ত;
      ৱর্ণ *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close পূর্ণাঙ্ক subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      যদি (subsystem[0] == '\0')
        অনুৱর্ত;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      যদি (it)
        subsystem = it + 5;

      যদি (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      যদি (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_মূল, subsystem, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      /* if there is already a mount specified, do not add a default one.  */
      যদি (find_mount_for (container, source_subsystem) != রিক্ত)
        অনুৱর্ত;

      ret = append_paths (&source_path, err, source_subsystem, subpath, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      যদি (UNLIKELY (subsystemfd < 0))
        ঘূৰাই crun_make_error (err, errno, "open `%s`", subsystem_path);

      যদি (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          যদি (UNLIKELY (ret < 0))
            {
              যদি (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  অনুৱর্ত;
                }
              ঘূৰাই ret;
            }
        }
      অন্যথা
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, রিক্ত, MS_BIND | mountflags, রিক্ত,
                          LABEL_NONE, err);
          যদি (UNLIKELY (ret < 0))
            {
              যদি (crun_error_get_errno (err) != ENOENT)
                ঘূৰাই ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, রিক্ত, MS_BIND | mountflags,
                              রিক্ত, LABEL_NONE, err);
              যদি (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  যদি (crun_error_get_errno (err) != ENOENT)
                    ঘূৰাই ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
do_mount_cgroup (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *source, পূর্ণাঙ্ক targetfd, ধ্রুৱ ৱর্ণ *target,
                 unsigned long mountflags, ধ্রুৱ ৱর্ণ *data, libcrun_error_t *err)
{
  পূর্ণাঙ্ক cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ঘূৰাই cgroup_mode;

  ৱিকল্প (cgroup_mode)
    {
    পক্ষ CGROUP_MODE_UNIFIED:
      ঘূৰাই do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    পক্ষ CGROUP_MODE_LEGACY:
    পক্ষ CGROUP_MODE_HYBRID:
      ঘূৰাই do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  ঘূৰাই crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

সংরচনা device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
স্থির শূন্য
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  সংরচনা private_data_s *private_data = get_private_data (container);
  size_t i;

  যদি (private_data->needed_devs_fds)
    {
      সংরচনা libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      বাবে (i = 0; needed_devs[i].path; i++)
        {
          যদি (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            অনুৱর্ত;

          পূর্ণাঙ্ক fd = syscall_open_বৃক্ষ (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          যদি (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          অন্যথা যদি (fd >= 0)
            close (fd);
        }
    }

  যদি (private_data->notify_socket_বৃক্ষ_fd < 0
      && private_data->host_notify_socket_path)
    {
      পূর্ণাঙ্ক fd = syscall_open_বৃক্ষ (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      যদি (fd >= 0)
        private_data->notify_socket_বৃক্ষ_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
স্থির ধ্রুৱ ৱর্ণ *
relative_path_under_dev (ধ্রুৱ ৱর্ণ *path)
{
  যদি (path[0] != '/')
    ঘূৰাই path;
  যদি (strncmp (path, "/dev/", 5) == 0)
    {
      যদি (strchr (path + 5, '/'))
        ঘূৰাই রিক্ত;
      ঘূৰাই path + 5;
    }
  ঘূৰাই রিক্ত;
}

স্থির পূর্ণাঙ্ক
mknod_and_set_attrs (libcrun_container_t *container, পূর্ণাঙ্ক dirfd, ধ্রুৱ ৱর্ণ *name, সংরচনা device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক fd = -1;
  proc_fd_path_t fd_path;
  ধ্রুৱ ৱর্ণ *what = "new";
  bool exists = false;
  সংরচনা stat st;
  পূর্ণাঙ্ক procfd;
  পূর্ণাঙ্ক ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno != EEXIST)
        ঘূৰাই crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    ঘূৰাই crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  যদি (UNLIKELY ((st.st_mode & S_IFMT) != type))
    ঘূৰাই crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  যদি (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    ঘূৰাই crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  যদি (exists)
    ঘূৰাই 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  যদি (UNLIKELY (procfd < 0))
    ঘূৰাই procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "fchown `%s`", device->path);

  ঘূৰাই 1;
}

পূর্ণাঙ্ক
libcrun_create_dev (libcrun_container_t *container, পূর্ণাঙ্ক devfd, পূর্ণাঙ্ক srcfd,
                    সংরচনা device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  ধ্রুৱ ৱর্ণ *fullname = device->path;
  ধ্রুৱ ৱর্ণ *rootfs = get_private_data (container)->rootfs;
  যদি (is_empty_string (fullname))
    ঘূৰাই crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free ৱর্ণ *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  যদি (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  ধ্রুৱ ৱর্ণ *rel_dev = relative_path_under_dev (normalized_path);

  যদি (binds)
    {
      cleanup_close পূর্ণাঙ্ক fd = -1;

      যদি (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          যদি (UNLIKELY (fd < 0))
            {
              যদি (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              যদি (UNLIKELY (fd < 0))
                ঘূৰাই crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      অন্যথা
        {
          ধ্রুৱ ৱর্ণ *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          যদি (UNLIKELY (fd < 0))
            ঘূৰাই fd;
        }

      যদি (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          যদি (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              যদি (LIKELY (ret >= 0))
                ঘূৰাই 0;
            }
          অন্যথা
            crun_error_release (err);
        }

      {
        cleanup_close পূর্ণাঙ্ক mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        যদি (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            যদি (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, রিক্ত);
                যদি (LIKELY (ret == 0))
                  ঘূৰাই 0;
              }
            অন্যথা
              crun_error_release (err);
          }
        অন্যথা
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, রিক্ত, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, রিক্ত, LABEL_MOUNT, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  অন্যথা
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      যদি (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          যদি (ret <= 0)
            ঘূৰাই ret;
        }
      অন্যথা
        {
          cleanup_close পূর্ণাঙ্ক dirfd = -1;
          cleanup_free ৱর্ণ *dirname = রিক্ত;
          ৱর্ণ *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          যদি (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          যদি (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              যদি (UNLIKELY (dirfd < 0))
                ঘূৰাই crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          অন্যথা
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              যদি (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              যদি (UNLIKELY (dirfd < 0))
                ঘূৰাই dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          যদি (ret <= 0)
            ঘূৰাই ret;
        }
    }
  ঘূৰাই 0;
}

সংরচনা symlink_s
{
  ধ্রুৱ ৱর্ণ *path;
  ধ্রুৱ ৱর্ণ *target;
  bool force;
};

স্থির সংরচনা symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { রিক্ত, রিক্ত, false } };

স্থির পূর্ণাঙ্ক
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  size_t i;
  সংরচনা device_s *it;
  cleanup_close পূর্ণাঙ্ক devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  ধ্রুৱ ৱর্ণ *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map সংরচনা libcrun_fd_map *dev_fds = রিক্ত;
  cleanup_close_map সংরচনা libcrun_fd_map *needed_fds = রিক্ত;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = রিক্ত;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = রিক্ত;

  যদি (! def || ! def->linux)
    ঘূৰাই 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  যদি (UNLIKELY (devfd < 0))
    ঘূৰাই devfd;

  বাবে (i = 0; i < def->linux->devices_len; i++)
    {
      সংরচনা device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      যদি (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  বাবে (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close পূর্ণাঙ্ক srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
      যদি (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  বাবে (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      যদি (UNLIKELY (ret < 0))
        {
          পূর্ণাঙ্ক saved_errno = errno;

          যদি (errno == EEXIST && ! symlinks[i].force)
            অনুৱর্ত;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          যদি (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              যদি (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              যদি (ret < 0 && errno == EBUSY)
                {
                  cleanup_close পূর্ণাঙ্ক tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  যদি (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      যদি (umount2 (procpath, MNT_DETACH) == 0)
                        গচ্ছ retry_unlink;
                    }
                }
              যদি (ret == 0)
                গচ্ছ retry_symlink;
            }
          ঘূৰাই crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  যদি (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ঘূৰাই 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
স্থির ধ্রুৱ ৱর্ণ *
next_path_component (ধ্রুৱ ৱর্ণ *p, size_t *len)
{
  বাবে (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      যদি (*len != 1 || p[0] != '.')
        ঘূৰাই p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
স্থির bool
same_path (ধ্রুৱ ৱর্ণ *a, ধ্রুৱ ৱর্ণ *b)
{
  size_t la, lb;

  বাবে (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      যদি (la != lb || memcmp (a, b, la) != 0)
        ঘূৰাই false;
      যদি (la == 0)
        ঘূৰাই true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
স্থির bool
is_duplicate_path (ৱর্ণ **paths, size_t i)
{
  size_t j;

  যদি (paths[i] == রিক্ত)
    ঘূৰাই false;

  বাবে (j = 0; j < i; j++)
    যদি (paths[j] && same_path (paths[j], paths[i]))
      ঘূৰাই true;

  ঘূৰাই false;
}

স্থির পূর্ণাঙ্ক
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  পূর্ণাঙ্ক ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  যদি (! def || ! def->linux)
    ঘূৰাই 0;

  বাবে (i = 0; i < def->linux->masked_paths_len; i++)
    {
      যদি (is_duplicate_path (def->linux->masked_paths, i))
        অনুৱর্ত;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  বাবে (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
do_pivot (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *rootfs, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক oldrootfd = -1;
  cleanup_close পূর্ণাঙ্ক newrootfd = -1;
  পূর্ণাঙ্ক ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (oldrootfd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (newrootfd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_মূল (".", ".");
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, রিক্ত, -1, ".", রিক্ত, MS_REC | MS_PRIVATE, রিক্ত, LABEL_MOUNT, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = umount2 (".", MNT_DETACH);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "umount2 oldroot");

  কৰা
    {
      ret = umount2 (".", MNT_DETACH);
      যদি (ret < 0 && errno == EINVAL)
        ভঙ্গ;
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "umount2 oldroot");
  } যেতিয়ালৈ (ret == 0);

  ret = chdir ("/");
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "chdir to newroot");

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, ৱর্ণ **data, libcrun_error_t *err)
{
  ধ্রুৱ ৱর্ণ *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close পূর্ণাঙ্ক fd = -1;
  সংরচনা stat st;
  পূর্ণাঙ্ক ret;

  যদি (*data != রিক্ত && strstr (*data, "mode="))
    ঘূৰাই 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  যদি (fd < 0)
    {
      যদি (crun_error_get_errno (err) != ENOENT)
        ঘূৰাই fd;

      crun_error_release (err);
      ঘূৰাই 0;
    }
  ret = fstat (fd, &st);
  যদি (ret < 0)
    ঘূৰাই crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
get_default_flags (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *destination, ৱর্ণ **data)
{
  যদি (strcmp (destination, "/proc") == 0)
    ঘূৰাই 0;
  যদি (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      ঘূৰাই MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  যদি (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      ঘূৰাই MS_NOEXEC | MS_STRICTATIME;
    }
  যদি (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      ঘূৰাই MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  যদি (strcmp (destination, "/dev/mqueue") == 0)
    ঘূৰাই MS_NOEXEC | MS_NOSUID | MS_NODEV;
  যদি (strcmp (destination, "/dev/pts") == 0)
    {
      যদি (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      অন্যথা
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      ঘূৰাই MS_NOEXEC | MS_NOSUID;
    }
  যদি (strcmp (destination, "/sys") == 0)
    ঘূৰাই MS_NOEXEC | MS_NOSUID | MS_NODEV;

  ঘূৰাই 0;
}

স্থির ধ্রুৱ ৱর্ণ *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  ঘূৰাই find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

স্থির পূর্ণাঙ্ক
check_valid_no_follow_file_system (পূর্ণাঙ্ক fd, ধ্রুৱ ৱর্ণ *destination, libcrun_error_t *err)
{
  সংরচনা statfs sfs;

  পূর্ণাঙ্ক ret = fstatfs (fd, &sfs);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "statfs `%s`", destination);

  ৱিকল্প (sfs.f_type)
    {
    পক্ষ PROC_SUPER_MAGIC:
    পক্ষ SYSFS_MAGIC:
    পক্ষ DEVPTS_SUPER_MAGIC:
      ঘূৰাই crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
safe_create_symlink (পূর্ণাঙ্ক rootfsfd, ধ্রুৱ ৱর্ণ *rootfs, ধ্রুৱ ৱর্ণ *target, ধ্রুৱ ৱর্ণ *destination, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক parent_dir_fd = -1;
  cleanup_free ৱর্ণ *buffer = রিক্ত;
  ৱর্ণ *part;
  পূর্ণাঙ্ক ret;

  যদি (is_empty_string (destination))
    ঘূৰাই crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  যদি (UNLIKELY (parent_dir_fd < 0))
    ঘূৰাই parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  যদি (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      যদি (errno == EEXIST)
        {
          cleanup_free ৱর্ণ *link = রিক্ত;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          যদি (UNLIKELY (len < 0))
            ঘূৰাই len;

          যদি ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            ঘূৰাই 0;

          ঘূৰাই crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      ঘূৰাই crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
handle_copy_symlink (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     ধ্রুৱ ৱর্ণ *link_target, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *target = রিক্ত;

  যদি (link_target == রিক্ত)
    {
      পূর্ণাঙ্ক old_মূল_fd = get_old_মূল_fd (get_private_data (container));
      ধ্রুৱ ৱর্ণ *source = mount->source;
      ssize_t len;

      যদি (old_মূল_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_মূল_fd, source + 1, &target, 0, err);
      অন্যথা
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      যদি (UNLIKELY (len < 0))
        ঘূৰাই len;
      link_target = target;
    }

  ঘূৰাই safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

স্থির পূর্ণাঙ্ক
prepare_sysfs_or_proc_mount (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *target,
                             পূর্ণাঙ্ক *targetfd, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOENT)
        {
          যদি (strchr (target, '/'))
            ঘূৰাই crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      অন্যথা যদি (errno == ENOTDIR)
        ঘূৰাই crun_make_error (err, errno, "the target `/%s` is invalid", target);

      যদি (ret < 0)
        ঘূৰাই crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
handle_tmpcopyup (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *rootfs, ধ্রুৱ ৱর্ণ *target,
                  পূর্ণাঙ্ক copy_from_fd, libcrun_error_t *err)
{
  পূর্ণাঙ্ক destfd, ret;
  cleanup_close পূর্ণাঙ্ক tmpfd = copy_from_fd;

  target = consume_slashes (target);

  যদি (is_empty_string (target))
    ঘূৰাই crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  যদি (UNLIKELY (destfd < 0))
    ঘূৰাই crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক
get_mount_label_how (ধ্রুৱ ৱর্ণ *type, bool is_sysfs_or_proc)
{
  যদি (is_sysfs_or_proc)
    ঘূৰাই LABEL_NONE;

  যদি (strcmp (type, "mqueue") == 0)
    ঘূৰাই LABEL_XATTR;

  ঘূৰাই LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
স্থির unsigned long
get_bind_mount_locked_flags (পূর্ণাঙ্ক source_mountfd, ধ্রুৱ ৱর্ণ *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  সংরচনা statfs sfs;
  size_t i;
  পূর্ণাঙ্ক ret;

  যদি (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  অন্যথা
    ret = statfs (source, &sfs);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই 0;

  বাবে (i = 0; i < mount->options_len; i++)
    {
      ধ্রুৱ সংরচনা propagation_flags_s *prop;

      যদি (mount->options[i] == রিক্ত)
        অনুৱর্ত;

      prop = libcrun_str2mount_flags (mount->options[i]);
      যদি (prop == রিক্ত || (prop->extra_flags & OPTION_RECURSIVE))
        অনুৱর্ত;

      যদি (prop->clear)
        cleared |= prop->flags;
      অন্যথা
        cleared &= ~prop->flags;
    }

  ঘূৰাই sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
স্থির ধ্রুৱ ৱর্ণ *
get_path_in_rootfs (ধ্রুৱ ৱর্ণ *rootfs, ধ্রুৱ ৱর্ণ *path)
{
  size_t len;

  যদি (rootfs == রিক্ত || path == রিক্ত)
    ঘূৰাই রিক্ত;

  len = strlen (rootfs);
  যেতিয়ালৈ (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  যদি (len == 0)
    ঘূৰাই রিক্ত;

  যদি (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    ঘূৰাই রিক্ত;

  ঘূৰাই consume_slashes (path + len);
}

স্থির পূর্ণাঙ্ক
process_single_mount (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      সংরচনা libcrun_fd_map *mount_fds, size_t mount_index,
                      ধ্রুৱ ৱর্ণ *systemd_cgroup_v1, libcrun_error_t *err)
{
  ধ্রুৱ ৱর্ণ *target = consume_slashes (mount->destination);
  cleanup_close পূর্ণাঙ্ক source_mountfd = -1;
  cleanup_free ৱর্ণ *data = রিক্ত;
  ৱর্ণ *type;
  ৱর্ণ *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close পূর্ণাঙ্ক copy_from_fd = -1;
  cleanup_close পূর্ণাঙ্ক targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  পূর্ণাঙ্ক no_remount = 0;
  পূর্ণাঙ্ক ret;

  যদি (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  যদি (mount->options == রিক্ত)
    flags = get_default_flags (container, mount->destination, &data);
  অন্যথা
    {
      size_t j;

      বাবে (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  যদি (type == রিক্ত && (flags & MS_BIND) == 0)
    ঘূৰাই crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  যদি ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      ধ্রুৱ ৱর্ণ *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      যদি (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          যদি (UNLIKELY (source_mountfd < 0))
            ঘূৰাই crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  যদি ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  যদি (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      যদি (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      যদি (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  যদি (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  যদি (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      ধ্রুৱ ৱর্ণ *path = mount->source;

      যদি ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        ঘূৰাই crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      যদি (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, রিক্ত);
        }
      অন্যথা যদি (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      অন্যথা যদি (path[0] == '/')
        {
          পূর্ণাঙ্ক old_মূল_fd = get_old_মূল_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          যদি (old_মূল_fd >= 0)
            ret = get_file_type_at (old_মূল_fd, &src_mode, nofollow, (path[1] == '\0') ? রিক্ত : path + 1);
          অন্যথা
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      অন্যথা
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "cannot stat `%s`", path);

      যদি (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          পূর্ণাঙ্ক bind_dirfd = AT_FDCWD;
          ধ্রুৱ ৱর্ণ *bind_source = mount->source;
          পূর্ণাঙ্ক old_মূল_fd = get_old_মূল_fd (get_private_data (container));

          যদি (old_মূল_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_মূল_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          source_mountfd = ret;
        }
    }

  যদি (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      ধ্রুৱ ৱর্ণ *cached_target = রিক্ত;

      যদি (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      mounted = true;
    }
  অন্যথা যদি (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  অন্যথা
    {
      bool is_dir = S_ISDIR (src_mode);

      যদি (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
      অন্যথা
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
          targetfd = ret;
        }
    }

  যদি (extra_flags & OPTION_TMPCOPYUP)
    {
      যদি (strcmp (type, "tmpfs") != 0)
        ঘূৰাই crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      যদি (UNLIKELY (copy_from_fd < 0))
        {
          যদি (errno != ENOTDIR)
            ঘূৰাই crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  যদি (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, রিক্ত);
      যদি (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          যদি (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, রিক্ত, source_mountfd, target, রিক্ত, remaining_flags, data, LABEL_NONE | no_remount, err);
              যদি (UNLIKELY (ret < 0))
                ঘূৰাই ret;
            }

          mounted = true;

          যদি (is_empty_string (target))
            {
              পূর্ণাঙ্ক new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              যদি (UNLIKELY (new_rootfsfd < 0))
                ঘূৰাই crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      অন্যথা যদি (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          ঘূৰাই crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      অন্যথা যদি (get_private_data (container)->host_মূল_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          ঘূৰাই crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      অন্যথা যদি (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  যদি (! mounted)
    {
      যদি (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
      অন্যথা যদি (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
      অন্যথা
        {
          পূর্ণাঙ্ক extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
    }

  যদি (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  যদি (rec_clear || rec_set)
    {
      ধ্রুৱ bool is_dir = S_ISDIR (src_mode);
      cleanup_close পূর্ণাঙ্ক dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      যদি (UNLIKELY (dfd < 0))
        ঘূৰাই crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
do_mounts (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ধ্রুৱ ৱর্ণ *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map সংরচনা libcrun_fd_map *mount_fds = রিক্ত;
  size_t i;
  পূর্ণাঙ্ক ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = রিক্ত;

  বাবে (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  ঘূৰাই 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
পূর্ণাঙ্ক
libcrun_container_do_bind_mount (libcrun_container_t *container, ৱর্ণ *mount_source, ৱর্ণ *mount_destination, ৱর্ণ **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  ধ্রুৱ ৱর্ণ *target = consume_slashes (mount_destination);
  cleanup_free ৱর্ণ *data = রিক্ত;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close পূর্ণাঙ্ক targetfd = -1;
  পূর্ণাঙ্ক is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  ধ্রুৱ ৱর্ণ *rootfs = get_private_data (container)->rootfs;
  পূর্ণাঙ্ক ret;

  যদি ((get_private_data (container)->rootfsfd < 0) || (rootfs == রিক্ত))
    ঘূৰাই crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  যদি (mount_options == রিক্ত)
    flags = get_default_flags (container, mount_destination, &data);
  অন্যথা
    {
      size_t j;

      বাবে (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  যদি (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  যদি (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      যদি (UNLIKELY (is_dir < 0))
        ঘূৰাই is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
পূর্ণাঙ্ক
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, পূর্ণাঙ্ক *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close পূর্ণাঙ্ক notify_fd = -1;
  cleanup_free ৱর্ণ *host_notify_socket_path = রিক্ত;
  cleanup_free ৱর্ণ *state_dir = রিক্ত;
  ৱর্ণ *host_path = রিক্ত;
  পূর্ণাঙ্ক ret;

  যদি (container && get_private_data (container)->host_notify_socket_path)
    {
      ধ্রুৱ ৱর্ণ *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  যদি (host_path == রিক্ত)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_মূল, context->id, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  যদি (UNLIKELY (notify_fd < 0))
    ঘূৰাই notify_fd;

  যদি (UNLIKELY (chmod (host_path, 0777) < 0))
    ঘূৰাই crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  যদি (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (শূন্য) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  ঘূৰাই 1;
#অন্যথা
  (শূন্য) context;
  (শূন্য) container;
  (শূন্য) err;
  *notify_socket_out = -1;
  ঘূৰাই 0;
#endif
}

#ifdef HAVE_SYSTEMD
স্থির পূর্ণাঙ্ক
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  ধ্রুৱ ৱর্ণ *notify_socket = container->context->notify_socket;
  cleanup_free ৱর্ণ *host_notify_socket_path = রিক্ত;
  cleanup_free ৱর্ণ *container_notify_socket_path = রিক্ত;
  cleanup_free ৱর্ণ *state_dir = রিক্ত;
  uid_t container_মূল_uid = -1;
  gid_t container_মূল_gid = -1;
  পূর্ণাঙ্ক notify_socket_বৃক্ষ_fd;

  যদি (notify_socket == রিক্ত)
    ঘূৰাই 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_মূল : রিক্ত),
                                     (container->context ? container->context->id : রিক্ত), err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = mkdir (host_notify_socket_path, 0700);
  যদি (ret < 0)
    ঘূৰাই crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  যদি (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_মূল_in_the_userns (container->container_def, 0, 0, &container_মূল_uid, &container_মূল_gid);
      যদি (container_মূল_uid != ((uid_t) -1) && container_মূল_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_মূল_uid, container_মূল_gid);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_মূল_uid, container_মূল_gid);
        }
    }

  notify_socket_বৃক্ষ_fd = syscall_open_বৃক্ষ (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  যদি (notify_socket_বৃক্ষ_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_বৃক্ষ_fd = notify_socket_বৃক্ষ_fd;
  অন্যথা যদি (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  অন্যথা যদি (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  অন্যথা
    /* some other error */
    ঘূৰাই crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = রিক্ত;
  ঘূৰাই 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
স্থির পূর্ণাঙ্ক
do_finalize_notify_socket (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *rootfs, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_free ৱর্ণ *host_notify_socket_path = রিক্ত;
  cleanup_free ৱর্ণ *container_notify_socket_path = রিক্ত;
  cleanup_free ৱর্ণ *container_notify_socket_path_dir_alloc = রিক্ত;
  ধ্রুৱ ৱর্ণ *container_notify_socket_path_dir = রিক্ত;
  cleanup_close পূর্ণাঙ্ক notify_socket_বৃক্ষ_fd = -1;
  cleanup_close পূর্ণাঙ্ক targetfd = -1;
  পূর্ণাঙ্ক did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = রিক্ত;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = রিক্ত;

  যদি (host_notify_socket_path == রিক্ত || container_notify_socket_path == রিক্ত)
    ঘূৰাই 0;

  যদি (rootfs == রিক্ত)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  যদি (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  targetfd = ret;

  notify_socket_বৃক্ষ_fd = get_private_data (container)->notify_socket_বৃক্ষ_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_বৃক্ষ_fd = -1;

  যদি (notify_socket_বৃক্ষ_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_বৃক্ষ_fd, targetfd, রিক্ত);
      যদি (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          যদি (UNLIKELY (targetfd < 0))
            ঘূৰাই targetfd;

          ret = do_mount (container, রিক্ত, targetfd, container_notify_socket_path_dir, রিক্ত,
                          MS_REC | MS_PRIVATE, রিক্ত, LABEL_NONE, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
      অন্যথা যদি (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      অন্যথা
        ঘূৰাই crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_বৃক্ষ_fd,
                                container_notify_socket_path_dir);
    }

  যদি (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, রিক্ত,
                      MS_BIND | MS_REC | MS_PRIVATE, রিক্ত, LABEL_MOUNT, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
make_parent_mount_private (ধ্রুৱ ৱর্ণ *rootfs, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক rootfsfd = -1;
  সংরচনা stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (rootfsfd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s`", rootfs);

  যদি (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    ঘূৰাই crun_make_error (err, errno, "fstat `%s`", rootfs);

  বাবে (;;)
    {
      সংরচনা stat cur_st;
      libcrun_error_t tmp_err = রিক্ত;
      পূর্ণাঙ্ক ret;
      cleanup_close পূর্ণাঙ্ক parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      যদি (ret == 0)
        ঘূৰাই 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (রিক্ত, proc_path, রিক্ত, MS_PRIVATE, রিক্ত);
      যদি (ret == 0)
        ঘূৰাই 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      যদি (parentfd < 0)
        {
          পূর্ণাঙ্ক saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          যদি (ret != 0)
            ঘূৰাই crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          ঘূৰাই crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      যদি (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        ঘূৰাই crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      যদি (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        ভঙ্গ;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  ঘূৰাই crun_make_error (err, 0, "make `%s` private", rootfs);
}

পূর্ণাঙ্ক
libcrun_set_mounts (সংরচনা container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ধ্রুৱ ৱর্ণ *rootfs, set_mounts_cb_t cb, শূন্য *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  পূর্ণাঙ্ক is_user_ns = 0;
  পূর্ণাঙ্ক ret = 0;

  যদি (rootfs == রিক্ত || def->mounts == রিক্ত)
    ঘূৰাই 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  যদি (def->মূল->readonly)
    {
      সংরচনা remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      পূর্ণাঙ্ক fd;

      fd = dup (get_private_data (container)->rootfsfd);
      যদি (UNLIKELY (fd < 0))
        ঘূৰাই crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, রিক্ত, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  যদি (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      যদি (UNLIKELY (is_user_ns < 0))
        ঘূৰাই is_user_ns;
    }

  যদি (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  যদি (cb)
    {
      ret = cb (cb_data, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  যদি (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = রিক্ত;
      ধ্রুৱ ৱর্ণ *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (শূন্য) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
umount_or_hide (ধ্রুৱ ৱর্ণ *target, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = umount2 (target, MNT_DETACH);
  যদি (UNLIKELY (ret < 0))
    {
      পূর্ণাঙ্ক saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      যদি (errno == EINVAL)
        {
          ret = mount (রিক্ত, target, "tmpfs", 0, "size=0k");
          যদি (LIKELY (ret == 0))
            ঘূৰাই 0;
        }

      ঘূৰাই crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক
move_মূল (ধ্রুৱ ৱর্ণ *rootfs, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = chdir (rootfs);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = umount_or_hide ("/proc", err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_finalize_mounts (সংরচনা container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ধ্রুৱ ৱর্ণ *rootfs,
                         set_mounts_cb_t after_pivot_cb, শূন্য *cb_data, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_মূল_fd));

  যদি (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      যদি (get_private_data (container)->no_pivot)
        {
          ret = move_মূল (rootfs, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
      অন্যথা
        {
          ret = do_pivot (container, rootfs, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }

      ret = do_mount (container, রিক্ত, -1, "/", রিক্ত,
                      get_private_data (container)->rootfs_propagation,
                      রিক্ত, LABEL_MOUNT, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = chdir ("/");
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "chdir to `/`");
    }

  যদি (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ret = finalize_mounts (container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
maybe_open_বৃক্ষ_namespace (ধ্রুৱ ৱর্ণ *rootfs, পূর্ণাঙ্ক *out_fd, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক rootfs_fd = -1;
  পূর্ণাঙ্ক বৃক্ষ_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (rootfs_fd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s`", rootfs);

  বৃক্ষ_fd = syscall_open_বৃক্ষ (rootfs_fd, "",
                               OPEN_বৃক্ষ_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  যদি (বৃক্ষ_fd < 0)
    {
      যদি (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        ঘূৰাই 0;
      ঘূৰাই crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = বৃক্ষ_fd;
  ঘূৰাই 0;
}

স্থির সংরচনা libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  সংরচনা libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  যদি (mount_fds == রিক্ত)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  ঘূৰাই mount_fds;
}

স্থির পূর্ণাঙ্ক
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ৱর্ণ *mnt_data = রিক্ত;
  unsigned long mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  ধ্রুৱ ৱর্ণ *label_type = রিক্ত;
  ধ্রুৱ ৱর্ণ *label_val = রিক্ত;
  ধ্রুৱ ৱর্ণ *fstype;
  পূর্ণাঙ্ক mnt_fd;
  size_t j;

  যদি (mount->type == রিক্ত)
    ঘূৰাই -1;

  fstype = mount->type;

  বাবে (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  যদি (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      পূর্ণাঙ্ক extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      যদি (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          যদি (label_type == রিক্ত)
            crun_error_release (err);
          অন্যথা
            label_val = def->linux->mount_label;
        }
    }

  যদি (strcmp (fstype, "cgroup") == 0)
    {
      পূর্ণাঙ্ক cgroup_mode = libcrun_get_cgroup_mode (err);
      যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  যদি (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, রিক্ত, রিক্ত, mnt_data);

  ঘূৰাই mnt_fd;
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
সংরচনা mnt_id_req_s
{
  uint32_t size;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

সংরচনা statmount_s
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
  ৱর্ণ str[];
};

স্থির পূর্ণাঙ্ক
syscall_listmount (সংরচনা mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned পূর্ণাঙ্ক flags)
{
#ifdef __NR_listmount
  ঘূৰাই (পূর্ণাঙ্ক) syscall (__NR_listmount, req, ids, nr, flags);
#অন্যথা
  (শূন্য) req;
  (শূন্য) ids;
  (শূন্য) nr;
  (শূন্য) flags;
  errno = ENOSYS;
  ঘূৰাই -1;
#endif
}

স্থির পূর্ণাঙ্ক
syscall_statmount (সংরচনা mnt_id_req_s *req, সংরচনা statmount_s *buf, size_t size, unsigned পূর্ণাঙ্ক flags)
{
#ifdef __NR_statmount
  ঘূৰাই (পূর্ণাঙ্ক) syscall (__NR_statmount, req, buf, size, flags);
#অন্যথা
  (শূন্য) req;
  (শূন্য) buf;
  (শূন্য) size;
  (শূন্য) flags;
  errno = ENOSYS;
  ঘূৰাই -1;
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
স্থির bool
mounts_hit_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = রিক্ত;
  size_t i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  বাবে (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      যদি (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != রিক্ত && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          ভঙ্গ;
        }
    }
  যদি (! has_absolute_bind)
    ঘূৰাই false;

  ids = xmalloc (capacity * পরিমাণ (uint64_t));
  বাবে (;;)
    {
      সংরচনা mnt_id_req_s req = {
        .size = পরিমাণ (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      পূর্ণাঙ্ক ret;

      যদি (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * পরিমাণ (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      যদি (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        ঘূৰাই true;
      যদি (ret == 0)
        ভঙ্গ;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  বাবে (i = 0; i < n_ids; i++)
    {
      ৱর্ণ buffer[4096] __attribute__ ((aligned (8)));
      সংরচনা statmount_s *sm = (সংরচনা statmount_s *) buffer;
      সংরচনা mnt_id_req_s req = {
        .size = পরিমাণ (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      ধ্রুৱ ৱর্ণ *মূল, *point;
      size_t j;

      যদি (syscall_statmount (&req, sm, পরিমাণ (buffer), 0) < 0)
        অনুৱর্ত;
      যদি ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        অনুৱর্ত;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      মূল = sm->str + sm->mnt_root;
      যদি (! has_prefix (মূল, "mnt:["))
        অনুৱর্ত;

      point = sm->str + sm->mnt_point;

      বাবে (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          ধ্রুৱ ৱর্ণ *source = def->mounts[j]->source;
          size_t source_len;

          যদি (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            অনুৱর্ত;
          যদি (source == রিক্ত || source[0] != '/')
            অনুৱর্ত;

          source_len = strlen (source);
          যেতিয়ালৈ (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          যদি (strncmp (point, source, source_len) != 0)
            অনুৱর্ত;

          /* The source itself is a pinned mount namespace.  */
          যদি (point[source_len] == '\0')
            ঘূৰাই true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          যদি (recursive && (source_len == 1 || point[source_len] == '/'))
            ঘূৰাই true;
        }
    }

  ঘূৰাই false;
}

স্থির bool
can_use_open_বৃক্ষ_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  সংরচনা libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  যদি (has_hooks || has_userns)
    ঘূৰাই false;

  যদি (mounts_hit_pinned_mount_namespace (def))
    ঘূৰাই false;

  যদি (container->context->no_open_বৃক্ষ_namespace)
    ঘূৰাই false;

  {
    libcrun_error_t tmp_err = রিক্ত;
    পূর্ণাঙ্ক in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    যদি (in_userns != 0)
      ঘূৰাই false;
  }

  mount_fds = get_fd_map (container);
  বাবে (i = 0; i < def->mounts_len; i++)
    {
      যদি (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          যদি (! has_cached_target)
            ঘূৰাই false;
        }
    }

  ঘূৰাই true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
স্থির bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  যদি (mount->type == রিক্ত || strcmp (mount->type, "tmpfs") != 0)
    ঘূৰাই false;

  বাবে (i = 0; i < mount->options_len; i++)
    যদি (mount->options[i] != রিক্ত && has_prefix (mount->options[i], "mode="))
      ঘূৰাই false;

  ঘূৰাই true;
}

স্থির পূর্ণাঙ্ক
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, ৱর্ণ **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close পূর্ণাঙ্ক rootfsfd = -1;
  cleanup_close পূর্ণাঙ্ক বৃক্ষ_fd = -1;
  libcrun_error_t tmp_err = রিক্ত;
  bool use_open_বৃক্ষ = false;
  size_t i;
  পূর্ণাঙ্ক ret;

  যদি (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, রিক্ত, রিক্ত, রিক্ত, রিক্ত);

  যদি ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  মুক্ত (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  বাবে (i = 0; i < def->mounts_len; i++)
    {
      সংরচনা libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close পূর্ণাঙ্ক mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      যদি (mount_fds && mount_fds->fds[i] >= 0)
        অনুৱর্ত;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      যদি (def->mounts[i]->source != রিক্ত
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free ৱর্ণ *mnt_data = রিক্ত;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          বাবে (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          যদি (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free ৱর্ণ *target = রিক্ত;
              libcrun_error_t tmp_err = রিক্ত;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              যদি (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              যদি (get_private_data (container)->copy_symlink_targets == রিক্ত)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * পরিমাণ (ৱর্ণ *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = রিক্ত;
            }
        }

      যদি (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          যদি (mount_fds && def->mounts[i]->source != রিক্ত && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == রিক্ত)
            {
              libcrun_error_t tmp_err = রিক্ত;
              unsigned long propagation = MS_PRIVATE;

              যদি (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              যদি (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          অনুৱর্ত;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      যদি (mnt_fd < 0)
        {
          crun_error_release (err);
          অনুৱর্ত;
        }

      যদি (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close পূর্ণাঙ্ক destfd = -1;
          সংরচনা stat st;

          যদি (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              যদি (UNLIKELY (rootfsfd < 0))
                ঘূৰাই crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          যদি (destfd < 0)
            crun_error_release (&tmp_err);
          অন্যথা যদি (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              পূর্ণাঙ্ক procfd = get_procfd (get_private_data (container), err);
              যদি (UNLIKELY (procfd < 0))
                ঘূৰাই procfd;

              get_self_fd_path (fd_path, mnt_fd);
              ret = fchmodat (procfd, fd_path, st.st_mode & 07777, 0);
              (শূন্য) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_empty_dir_cached (container, রিক্ত, &tmp_err);
  যদি (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  যদি (! get_private_data (container)->joined_mount_ns
      && can_use_open_বৃক্ষ_namespace (container))
    use_open_বৃক্ষ = true;

  যদি (use_open_বৃক্ষ)
    {
      ret = maybe_open_বৃক্ষ_namespace (*rootfs, &বৃক্ষ_fd, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  যদি (বৃক্ষ_fd >= 0)
    preopen_needed_devs_and_notify (container);

  যদি (বৃক্ষ_fd >= 0)
    {
      ret = setns (বৃক্ষ_fd, CLONE_NEWNS);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (রিক্ত, "/", রিক্ত, MS_REMOUNT | MS_BIND, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, রিক্ত, -1, "/", রিক্ত, MS_REC | MS_PRIVATE, রিক্ত, LABEL_MOUNT, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = do_mount (container, রিক্ত, -1, "/", রিক্ত, rootfs_propagation, রিক্ত, LABEL_MOUNT, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_মূল_switched = true;
      মুক্ত (*rootfs);
      *rootfs = xstrdup ("/");
    }
  অন্যথা
    {
      যদি (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, রিক্ত, -1, "/", রিক্ত, rootfs_propagation, রিক্ত, LABEL_MOUNT, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = make_parent_mount_private (*rootfs, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, রিক্ত, MS_BIND | MS_REC | MS_PRIVATE, রিক্ত, LABEL_MOUNT, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  যদি (! container->context->force_no_cgroup)
    {
      পূর্ণাঙ্ক cgroup_mode = libcrun_get_cgroup_mode (err);
      যদি (UNLIKELY (cgroup_mode < 0))
        ঘূৰাই cgroup_mode;

      যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          ৱর্ণ *unified_cgroup_path = রিক্ত;
          পূর্ণাঙ্ক procfd = get_procfd (get_private_data (container), err);
          যদি (UNLIKELY (procfd < 0))
            ঘূৰাই procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  যদি (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = রিক্ত;
      ঘূৰাই ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = রিক্ত;

  যদি (বৃক্ষ_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  যদি (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_do_pivot_মূল (libcrun_container_t *container, bool no_pivot, ৱর্ণ **rootfs, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  যদি (*rootfs == রিক্ত)
    ঘূৰাই 0;

  যদি (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      যদি (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "chdir to `/`");
        }
    }
  অন্যথা
    {
      ret = chroot (*rootfs);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "chdir to `/`");

      মুক্ত (*rootfs);
      *rootfs = xstrdup ("/");
    }

  ঘূৰাই 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
পূর্ণাঙ্ক
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  সংরচনা stat statbuf;
  cleanup_close পূর্ণাঙ্ক fd;
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  যদি (UNLIKELY (fd == -1))
    ঘূৰাই crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  বাবে (i = 0; i <= 2; i++)
    {
      যদি (UNLIKELY (fstat (i, &statbuf) == -1))
        ঘূৰাই crun_make_error (err, errno, "stat fd `%d`", i);
      যদি (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          যদি (UNLIKELY (dup2 (fd, i) == -1))
            ঘূৰাই crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
uidgidmap_helper (ৱর্ণ *helper, pid_t pid, ধ্রুৱ ৱর্ণ *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  ৱর্ণ pid_fmt[16];
  ৱর্ণ *args[MAX_ARGS + 1];
  ৱর্ণ *next;
  cleanup_free ৱর্ণ *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  পূর্ণাঙ্ক ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, পরিমাণ (pid_fmt), "%d", pid);
  যদি (UNLIKELY (ret >= (পূর্ণাঙ্ক) পরিমাণ (pid_fmt)))
    ঘূৰাই crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  যেতিয়ালৈ (nargs < MAX_ARGS)
    {
      ৱর্ণ *p = strsep (&next, " \n");
      যদি (next == রিক্ত)
        ভঙ্গ;
      args[nargs++] = p;
    }
  args[nargs++] = রিক্ত;

  ret = run_process (args, err);
  যদি (ret < 0)
    ঘূৰাই ret;
  যদি (ret > 0)
    ঘূৰাই crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
newgidmap (pid_t pid, ধ্রুৱ ৱর্ণ *map_file, libcrun_error_t *err)
{
  ঘূৰাই uidgidmap_helper ("newgidmap", pid, map_file, err);
}

স্থির পূর্ণাঙ্ক
newuidmap (pid_t pid, ধ্রুৱ ৱর্ণ *map_file, libcrun_error_t *err)
{
  ঘূৰাই uidgidmap_helper ("newuidmap", pid, map_file, err);
}

স্থির পূর্ণাঙ্ক
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_close পূর্ণাঙ্ক fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  যদি (UNLIKELY (fd < 0))
    ঘূৰাই fd;

  ret = TEMP_FAILURE_RETRY (লিখ (fd, "deny", 4));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_free ৱর্ণ *content = রিক্ত;

  যদি (get_private_data (container)->deny_setgroups)
    ঘূৰাই 0;

  যদি (container->container_def->annotations)
    {
      ধ্রুৱ ৱর্ণ *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      যদি (annotation)
        ঘূৰাই strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close পূর্ণাঙ্ক fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    যদি (fd < 0)
      ret = fd;
    অন্যথা
      ret = read_all_fd (fd, "self/setgroups", &content, রিক্ত, err);
  }
  যদি (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      যদি (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ঘূৰাই 1;
        }
      ঘূৰাই ret;
    }

  ঘূৰাই strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

পূর্ণাঙ্ক
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = রিক্ত;
  size_t additional_gids_len = 0;
  পূর্ণাঙ্ক can_do_setgroups;
  পূর্ণাঙ্ক ret;

  যদি (process != রিক্ত && process->user != রিক্ত)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  যদি (UNLIKELY (can_do_setgroups < 0))
    ঘূৰাই can_do_setgroups;

  যদি (can_do_setgroups == 0)
    {
      ধ্রুৱ ৱর্ণ *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      যদি (annotation && strcmp (annotation, "0") != 0)
        ঘূৰাই 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      বাবে (i = 0; i < additional_gids_len; i++)
        যদি (additional_gids[i] != process->user->gid)
          ঘূৰাই crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (পূর্ণাঙ্ক) additional_gids[i]);
      ঘূৰাই 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "setgroups");

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  যদি (get_private_data (container)->unshare_cgroupns)
    {
      পূর্ণাঙ্ক ret = unshare (CLONE_NEWCGROUP);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (errno != EINVAL)
            ঘূৰাই crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  ঘূৰাই 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
পূর্ণাঙ্ক
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  যদি (len != 1)
    ঘূৰাই 0;

  যদি (mappings[0]->size != 1)
    ঘূৰাই 0;

  যদি (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    ঘূৰাই 0;

  ঘূৰাই 1;
}

স্থির পূর্ণাঙ্ক
write_id_map (libcrun_container_t *container, pid_t pid, ধ্রুৱ ৱর্ণ *map_name,
              পূর্ণাঙ্ক (*idmap_helper) (pid_t, ধ্রুৱ ৱর্ণ *, libcrun_error_t *), ধ্রুৱ ৱর্ণ *helper_name,
              bool warn_on_failure, ধ্রুৱ ৱর্ণ *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret = 0;

  যদি (container->host_uid)
    ret = idmap_helper (pid, map, err);
  যদি (container->host_uid == 0 || ret < 0)
    {
      cleanup_close পূর্ণাঙ্ক fd = -1;

      যদি (ret < 0)
        {
          যদি (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      যদি (UNLIKELY (fd < 0))
        ঘূৰাই fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      যদি (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free ৱর্ণ *single_mapping = রিক্ত;
          crun_error_release (err);

          যদি (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              যদি (UNLIKELY (ret < 0))
                ঘূৰাই ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          যদি (UNLIKELY (fd < 0))
            ঘূৰাই fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  ঘূৰাই ret;
}

পূর্ণাঙ্ক
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *uid_map = রিক্ত;
  cleanup_free ৱর্ণ *gid_map = রিক্ত;
  size_t uid_map_len = 0, gid_map_len = 0;
  পূর্ণাঙ্ক ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  যদি ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    ঘূৰাই 0;

  যদি (! def || ! def->linux)
    ঘূৰাই 0;

  যদি (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  অন্যথা
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      uid_map_len = (size_t) ret;
      যদি (uid_map == রিক্ত)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
    }

  যদি (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  অন্যথা
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      gid_map_len = (size_t) ret;
      যদি (gid_map == রিক্ত)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

সংরচনা all_caps_s
{
  unsigned long effective[2];
  unsigned long permitted[2];
  unsigned long inheritable[2];
  unsigned long ambient[2];
  unsigned long bounding[2];
};

স্থির পূর্ণাঙ্ক
has_cap_on (পূর্ণাঙ্ক cap, long unsigned *caps)
{
  যদি (cap < 32)
    ঘূৰাই CAP_TO_MASK_0 (cap) & caps[0];
  ঘূৰাই (CAP_TO_MASK_1 (cap) & caps[1]);
}

স্থির unsigned long cap_last_cap;

পূর্ণাঙ্ক
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক fd = -1;
  পূর্ণাঙ্ক ret;
  ৱর্ণ buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  যদি (fd < 0)
    ঘূৰাই fd;
  ret = TEMP_FAILURE_RETRY (পঠ (fd, buffer, পরিমাণ (buffer) - 1));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, রিক্ত, 10);
  যদি (errno != 0)
    ঘূৰাই crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
set_required_caps (সংরচনা all_caps_s *caps, uid_t uid, gid_t gid, পূর্ণাঙ্ক no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  পূর্ণাঙ্ক ret;
  সংরচনা __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  সংরচনা __user_cap_data_struct data[2] = { { 0 } };

  যদি (cap_last_cap == 0)
    ঘূৰাই crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  বাবে (cap = 0; cap <= cap_last_cap; cap++)
    যদি (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        যদি (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          ঘূৰাই crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  যদি (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ঘূৰাই crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  যদি (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    ঘূৰাই crun_make_error (err, errno, "prctl reset ambient");

  বাবে (cap = 0; cap <= cap_last_cap; cap++)
    যদি (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        যদি (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          ঘূৰাই crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  যদি (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "prctl reset KEEPCAPS");

      যদি (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          বাবে (cap = 0; cap <= cap_last_cap; cap++)
            যদি (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "capset");
        }
    }
#endif

  যদি (no_new_privs)
    যদি (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      ঘূৰাই crun_make_error (err, errno, "prctl set no new privs");

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
read_caps (unsigned long caps[2], ৱর্ণ **মানসমূহ, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  বাবে (i = 0; i < len; i++)
    {
      cap_মান_t cap;
      যদি (cap_from_name (মানসমূহ[i], &cap) < 0)
        ঘূৰাই crun_make_error (err, 0, "unknown capability `%s`", মানসমূহ[i]);

      যদি (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      অন্যথা
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#অন্যথা
  caps[0] = 0;
  caps[1] = 0;
#endif
  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  যদি (proc->selinux_label)
    ঘূৰাই set_selinux_label (container, proc->selinux_label, now, err);

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  যদি (proc->apparmor_profile)
    ঘূৰাই set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  পূর্ণাঙ্ক no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  সংরচনা all_caps_s caps = {};
  পূর্ণাঙ্ক ret;

  যদি (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  ঘূৰাই set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

সংরচনা rlimit_s
{
  ধ্রুৱ ৱর্ণ *name;
  পূর্ণাঙ্ক মান;
};

সংরচনা rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
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
                              { "RLIMIT_STACK", RLIMIT_স্তূপ },
                              { রিক্ত, 0 } };

স্থির পূর্ণাঙ্ক
get_rlimit_resource (ধ্রুৱ ৱর্ণ *name)
{
  সংরচনা rlimit_s *it;
  বাবে (it = rlimits; it->name; it++)
    যদি (strcmp (it->name, name) == 0)
      ঘূৰাই it->মান;
  ঘূৰাই -1;
}

পূর্ণাঙ্ক
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  বাবে (i = 0; i < len; i++)
    {
      বাবে (j = i + 1; j < len; j++)
        {
          যদি (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            ঘূৰাই crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  বাবে (i = 0; i < len; i++)
    {
      সংরচনা rlimit limit;
      ৱর্ণ *type = new_rlimits[i]->type;
      পূর্ণাঙ্ক resource = get_rlimit_resource (type);
      যদি (UNLIKELY (resource < 0))
        ঘূৰাই crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      যদি (UNLIKELY (setrlimit (resource, &limit) < 0))
        ঘূৰাই crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  পূর্ণাঙ্ক has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  পূর্ণাঙ্ক ret;
  যদি (def->hostname == রিক্ত || def->hostname[0] == '\0')
    ঘূৰাই 0;
  যদি (! has_uts)
    ঘূৰাই crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "sethostname");
  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  পূর্ণাঙ্ক has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  পূর্ণাঙ্ক ret;
  যদি (is_empty_string (def->domainname))
    ঘূৰাই 0;
  যদি (! has_uts)
    ঘূৰাই crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "setdomainname");
  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close পূর্ণাঙ্ক fd = -1;
  পূর্ণাঙ্ক ret;
  ৱর্ণ oom_buffer[16];
  যদি (def->process == রিক্ত || ! def->process->oom_score_adj_present)
    ঘূৰাই 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, পরিমাণ (oom_buffer), "%i", def->process->oom_score_adj);
  যদি (UNLIKELY (ret >= (পূর্ণাঙ্ক) পরিমাণ (oom_buffer)))
    ঘূৰাই crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  যদি (fd < 0)
    ঘূৰাই fd;
  ret = TEMP_FAILURE_RETRY (লিখ (fd, oom_buffer, strlen (oom_buffer)));
  যদি (ret < 0)
    ঘূৰাই crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  ঘূৰাই 0;
}

ধ্রুৱ ৱর্ণ *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  রিক্ত
};

স্থির পূর্ণাঙ্ক
validate_sysctl (ধ্রুৱ ৱর্ণ *original_চাবি, ধ্রুৱ ৱর্ণ *original_মান, ধ্রুৱ ৱর্ণ *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ধ্রুৱ ৱর্ণ *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  যদি (name[0] == '/' || path_has_dot_dot_component (name))
    ঘূৰাই crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_চাবি);

  যদি (has_prefix (name, "fs/mqueue/"))
    {
      যদি (namespaces_created & CLONE_NEWIPC)
        ঘূৰাই 0;

      namespace = "IPC";
      গচ্ছ fail;
    }

  যদি (has_prefix (name, "kernel/"))
    {
      size_t i;

      বাবে (i = 0; sysctlRequiringIPC[i]; i++)
        যদি (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            যদি (namespaces_created & CLONE_NEWIPC)
              ঘূৰাই 0;

            namespace = "IPC";
            গচ্ছ fail;
          }

      যদি (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          যদি (! is_empty_string (def->domainname) && (strcmp (original_মান, def->domainname) != 0))
            ঘূৰাই crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_চাবি);

          যদি (namespaces_created & CLONE_NEWUTS)
            ঘূৰাই 0;

          namespace = "UTS";
          গচ্ছ fail;
        }

      যদি (strcmp (name, "kernel/hostname") == 0)
        ঘূৰাই crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_চাবি);
    }
  যদি (has_prefix (name, "net/"))
    {
      যদি (namespaces_created & CLONE_NEWNET)
        ঘূৰাই 0;

      namespace = "network";
      গচ্ছ fail;
    }

  ঘূৰাই crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_চাবি);

fail:
  ঘূৰাই crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_চাবি, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
স্থির ৱর্ণ *
sysctl_error_reason (ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক namespaces_created, পূর্ণাঙ্ক errno_)
{

  যদি (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    ঘূৰাই xstrdup ("are all the IDs mapped in the user namespace?");

  ঘূৰাই রিক্ত;
}

পূর্ণাঙ্ক
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close পূর্ণাঙ্ক dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  যদি (def->linux == রিক্ত || def->linux->sysctl == রিক্ত || def->linux->sysctl->len == 0)
    ঘূৰাই 0;

  বাবে (i = 0; i < def->linux->namespaces_len; i++)
    {
      পূর্ণাঙ্ক মান;

      মান = libcrun_find_namespace (def->linux->namespaces[i]->type);
      যদি (UNLIKELY (মান < 0))
        ঘূৰাই crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= মান;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  যদি (UNLIKELY (dirfd < 0))
    ঘূৰাই dirfd;

  বাবে (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free ৱর্ণ *name = রিক্ত;
      cleanup_close পূর্ণাঙ্ক fd = -1;
      পূর্ণাঙ্ক ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      যদি (def->linux->sysctl->চাবিগুলি[i] == রিক্ত || def->linux->sysctl->মানসমূহ[i] == রিক্ত)
        ঘূৰাই crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_চাবি_to_proc_path (def->linux->sysctl->চাবিগুলি[i]);

      ret = validate_sysctl (def->linux->sysctl->চাবিগুলি[i], def->linux->sysctl->মানসমূহ[i], name, namespaces_created, def, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      যদি (UNLIKELY (fd < 0))
        ঘূৰাই crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (লিখ (fd, def->linux->sysctl->মানসমূহ[i], strlen (def->linux->sysctl->মানসমূহ[i])));
      যদি (UNLIKELY (ret < 0))
        {
          পূর্ণাঙ্ক saved_errno = errno;
          cleanup_free ৱর্ণ *reason = রিক্ত;

          reason = sysctl_error_reason (def->linux->sysctl->চাবিগুলি[i], namespaces_created, saved_errno);
          ঘূৰাই crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
open_terminal (ৱর্ণ **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক fd = -1;
  uid_t uid = 0;
  পূর্ণাঙ্ক ret;

  যদি (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  যদি (UNLIKELY (fd < 0))
    ঘূৰাই fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  ঘূৰাই ret;
}

ৱর্ণ *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  ঘূৰাই get_private_data (container)->external_descriptors;
}

পূর্ণাঙ্ক
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  ধ্রুৱ ৱর্ণ *buf = রিক্ত;
  json_gen_ctx *gen = রিক্ত;
  size_t buf_len;
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক i;

  যদি (! json_gen_init (&gen, রিক্ত))
    ঘূৰাই crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  যদি (UNLIKELY (ret != json_gen_status_ok))
    গচ্ছ json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  বাবে (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      ৱর্ণ link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      যদি (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          যদি (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          অন্যথা
            {
              json_gen_free (gen);
              ঘূৰাই crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      যদি (UNLIKELY (ret != json_gen_status_ok))
        গচ্ছ json_error;
    }

  ret = json_gen_array_close (gen);
  যদি (UNLIKELY (ret != json_gen_status_ok))
    গচ্ছ json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  যদি (UNLIKELY (ret != json_gen_status_ok))
    গচ্ছ json_error;

  যদি (buf)
    {
      ৱর্ণ *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  ঘূৰাই 0;

json_error:
  যদি (gen)
    json_gen_free (gen);
  ঘূৰাই json_gen_error_to_crun_error (ret, err);
}

পূর্ণাঙ্ক
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ৱর্ণ *pty = রিক্ত;
  cleanup_close পূর্ণাঙ্ক fd = -1;
  পূর্ণাঙ্ক ret;

  যদি (def->process == রিক্ত || ! def->process->terminal)
    ঘূৰাই 0;

  fd = open_terminal (&pty, def->process, err);
  যদি (UNLIKELY (fd < 0))
    ঘূৰাই fd;

  যদি (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  যদি (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", রিক্ত, MS_BIND, রিক্ত, LABEL_MOUNT, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  অন্যথা
    {
      ret = unlink ("/dev/console");
      যদি (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          যদি (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", রিক্ত, MS_BIND, রিক্ত, LABEL_MOUNT, err);
              যদি (UNLIKELY (ret < 0))
                ঘূৰাই ret;

              ঘূৰাই get_and_reset (&fd);
            }
          ঘূৰাই crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  ঘূৰাই get_and_reset (&fd);
}

স্থির bool
read_error_from_sync_socket (পূর্ণাঙ্ক sync_socket_fd, পূর্ণাঙ্ক *error, ৱর্ণ **str)
{
  cleanup_free ৱর্ণ *b = রিক্ত;
  size_t size;
  পূর্ণাঙ্ক code = 0;
  পূর্ণাঙ্ক ret;

  যদি (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (পঠ (sync_socket_fd, &code, পরিমাণ (code)));
      যদি (UNLIKELY (ret != (পূর্ণাঙ্ক) পরিমাণ (code)))
        ঘূৰাই false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (পঠ (sync_socket_fd, &size, পরিমাণ (size)));
  যদি (UNLIKELY (ret != (পূর্ণাঙ্ক) পরিমাণ (size)))
    ঘূৰাই false;

  যদি (size == 0)
    ঘূৰাই false;

  যদি (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (পঠ (sync_socket_fd, b, size));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই false;

  b[ret] = '\0';

  *str = b;
  b = রিক্ত;
  ঘূৰাই true;
}

স্থির bool
send_error_to_sync_socket (পূর্ণাঙ্ক sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক code;
  ৱর্ণ *বাৰ্তা;

  যদি (err == রিক্ত || *err == রিক্ত)
    ঘূৰাই false;

  code = crun_error_get_errno (err);

  যদি (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (লিখ (sync_socket_fd, "1", 1));
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই false;
    }

  ret = TEMP_FAILURE_RETRY (লিখ (sync_socket_fd, &code, পরিমাণ (code)));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই false;

  বাৰ্তা = (*err)->বাৰ্তা;
  size = strlen (বাৰ্তা) + 1;
  ret = TEMP_FAILURE_RETRY (লিখ (sync_socket_fd, &size, পরিমাণ (size)));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই false;

  ret = TEMP_FAILURE_RETRY (লিখ (sync_socket_fd, বাৰ্তা, size));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই false;

  ঘূৰাই true;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
send_success_to_sync_socket (পূর্ণাঙ্ক sync_socket, libcrun_error_t *err)
{
  ধ্রুৱ পূর্ণাঙ্ক success = 0;
  পূর্ণাঙ্ক ret;

  ret = TEMP_FAILURE_RETRY (লিখ (sync_socket, &success, পরিমাণ (success)));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "write to sync socket");

  ঘূৰাই 0;
}

স্থির __attribute__ ((noreturn)) শূন্য
send_error_to_sync_socket_and_die (পূর্ণাঙ্ক sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  ৱর্ণ *বাৰ্তা;

  যদি (err == রিক্ত || *err == রিক্ত)
    _safe_exit (EXIT_FAILURE);

  যদি (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      বাৰ্তা = (*err)->বাৰ্তা;
      libcrun_fail_with_error (errno, "%s", বাৰ্তা);
    }
  _safe_exit (EXIT_FAILURE);
}

স্থির পূর্ণাঙ্ক
expect_success_from_sync_socket (পূর্ণাঙ্ক sync_fd, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *err_str = রিক্ত;
  পূর্ণাঙ্ক res = 1;
  পূর্ণাঙ্ক ret;

  ret = TEMP_FAILURE_RETRY (পঠ (sync_fd, &res, পরিমাণ (res)));
  যদি (UNLIKELY (ret != পরিমাণ (res)))
    ঘূৰাই crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  যদি (res == 0)
    ঘূৰাই 0;

  যদি (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      যদি (! is_empty_string (err_str))
        ঘূৰাই crun_make_error (err, res, "%s", err_str);
    }

  ঘূৰাই crun_make_error (err, res, "read from sync socket");
}

স্থির পূর্ণাঙ্ক
join_namespaces (runtime_spec_schema_config_schema *def, পূর্ণাঙ্ক *namespaces_to_join, পূর্ণাঙ্ক n_namespaces_to_join,
                 পূর্ণাঙ্ক *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক i;

  বাবে (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free ৱর্ণ *cwd = রিক্ত;
      পূর্ণাঙ্ক orig_index = namespaces_to_join_index[i];
      পূর্ণাঙ্ক মান;

      যদি (namespaces_to_join[i] < 0)
        অনুৱর্ত;

      /* Skip the user namespace.  */
      মান = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      যদি (মান == CLONE_NEWUSER)
        অনুৱর্ত;

      যদি (মান == CLONE_NEWNS)
        {
          cwd = getcwd (রিক্ত, 0);
          যদি (UNLIKELY (cwd == রিক্ত))
            ঘূৰাই crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], মান);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (ignore_join_errors)
            অনুৱর্ত;
          ঘূৰাই crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      যদি (মান == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  ঘূৰাই 0;
}

#define MAX_NAMESPACES 10

সংরচনা init_status_s
{
  /* fd to the namespace to join.  */
  পূর্ণাঙ্ক fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  পূর্ণাঙ্ক index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  পূর্ণাঙ্ক মান[MAX_NAMESPACES];
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
  পূর্ণাঙ্ক userns_index;
  /* def->linux->namespaces userns.  */
  পূর্ণাঙ্ক userns_index_origin;

  /* All namespaces created/joined by the container.  */
  পূর্ণাঙ্ক all_namespaces;

  /* What namespaces are still missing to be created.  */
  পূর্ণাঙ্ক namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  পূর্ণাঙ্ক idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  পূর্ণাঙ্ক idx_timens_to_join_immediately;
};

শূন্য
cleanup_free_init_statusp (সংরচনা init_status_s *ns)
{
  size_t i;

  যদি (ns == রিক্ত)
    ঘূৰাই;

  বাবে (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

স্থির পূর্ণাঙ্ক
configure_init_status (সংরচনা init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  বাবে (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  বাবে (i = 0; i < def->linux->namespaces_len; i++)
    {
      পূর্ণাঙ্ক মান = libcrun_find_namespace (def->linux->namespaces[i]->type);
      যদি (UNLIKELY (মান < 0))
        ঘূৰাই crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      যদি (ns->all_namespaces & মান)
        ঘূৰাই crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= মান;

      যদি (def->linux->namespaces[i]->path == রিক্ত)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= মান;
        }
      অন্যথা
        {
          পূর্ণাঙ্ক fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          যদি (ns->fd_len >= MAX_NAMESPACES)
            ঘূৰাই crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          যদি (UNLIKELY (fd < 0))
            ঘূৰাই crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          যদি (মান == CLONE_NEWUSER)
            {
              ns->userns_index = ns->fd_len;
              ns->userns_index_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->index[ns->fd_len] = i;
          ns->মান[ns->fd_len] = মান;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  যদি (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  ঘূৰাই 0;
}

/* Detect if root is available in the container.  */
স্থির bool
মূল_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  বাবে (i = 0; i < len; i++)
    যদি (mappings[i]->container_id == 0)
      ঘূৰাই true;

  ঘূৰাই false;
}

স্থির সংরচনা libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  সংরচনা libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  যদি (dev_fds == রিক্ত)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  ঘূৰাই dev_fds;
}

#define অঙ্ক_NEEDED_DEVS (পরিমাণ (needed_devs) / পরিমাণ (needed_devs[0]) - 1)

স্থির সংরচনা libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  সংরচনা libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  যদি (fds == রিক্ত)
    {
      fds = make_libcrun_fd_map (অঙ্ক_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  ঘূৰাই fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  যদি (recursive)
    *recursive = false;

  যদি (src_nofollow != রিক্ত)
    *src_nofollow = false;

  বাবে (i = 0; i < mnt->options_len; i++)
    {
      যদি (mnt->options[i] == রিক্ত)
        অনুৱর্ত;

      যদি (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          যদি (src_nofollow == রিক্ত || *src_nofollow)
            ভঙ্গ;
        }
      যদি (strcmp (mnt->options[i], "rbind") == 0)
        {
          যদি (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          যদি (src_nofollow == রিক্ত || *src_nofollow)
            ভঙ্গ;
        }
      যদি (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  ঘূৰাই ret;
}

স্থির ৱর্ণ *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  বাবে (i = 0; i < mnt->options_len; i++)
    {
      যদি (mnt->options[i] == রিক্ত)
        অনুৱর্ত;

      যদি (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          ঘূৰাই mnt->options[i];
        }
      যদি (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          ঘূৰাই mnt->options[i];
        }
    }
  ঘূৰাই রিক্ত;
}

স্থির পূর্ণাঙ্ক
open_mount_type (ধ্রুৱ ৱর্ণ *type, পূর্ণাঙ্ক *out_fd, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক fsopen_fd = -1;
  cleanup_close পূর্ণাঙ্ক newfs_fd = -1;
  পূর্ণাঙ্ক ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  যদি (UNLIKELY (fsopen_fd < 0))
    ঘূৰাই crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, রিক্ত, রিক্ত, 0);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  যদি (UNLIKELY (newfs_fd < 0))
    ঘূৰাই crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, পূর্ণাঙ্ক *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  সংরচনা mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close পূর্ণাঙ্ক fd = -1;
  ধ্রুৱ ৱর্ণ *idmap_option;
  bool recursive = false;
  ধ্রুৱ ৱর্ণ *options = রিক্ত;
  bool has_mappings;
  পূর্ণাঙ্ক ret;
  ৱর্ণ *extra_বাৰ্তা = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != রিক্ত);
  যদি (has_mappings_out)
    *has_mappings_out = has_mappings;
  যদি (! has_mappings)
    ঘূৰাই 0;

  যদি ((mnt->uid_mappings == রিক্ত) != (mnt->gid_mappings == রিক্ত))
    ঘূৰাই crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  যদি (idmap_option)
    {
      options = strchr (idmap_option, '=');
      যদি (options)
        {
          /* Skip the '=' itself.  */
          options++;
          যদি (options[0] == '\0')
            options = রিক্ত;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;
  যদি (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  যদি (UNLIKELY (fd < 0))
    ঘূৰাই fd;

  যদি (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_বৃক্ষ (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      যদি (UNLIKELY (newfs_fd < 0))
        ঘূৰাই crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  অন্যথা
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == EINVAL)
        {
          extra_বাৰ্তা = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      ঘূৰাই crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_বাৰ্তা);
    }

  *out_fd = get_and_reset (&newfs_fd);
  ঘূৰাই 0;
}

স্থির uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  যদি (! def || ! def->linux)
    গচ্ছ exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  যদি (! mappings)
    গচ্ছ exit;

  বাবে (i = 0; i < len; i++)
    {
      যদি (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        ঘূৰাই id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  ঘূৰাই is_uid ? get_overflow_uid () : get_overflow_gid ();
}

স্থির পূর্ণাঙ্ক
precreate_device (libcrun_container_t *container, পূর্ণাঙ্ক devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  ৱর্ণ name[64];
  mode_t type;
  dev_t dev;
  পূর্ণাঙ্ক ret;

  ret = snprintf (name, পরিমাণ (name), "%zu", i);
  যদি (UNLIKELY (ret >= (পূর্ণাঙ্ক) পরিমাণ (name)))
    ঘূৰাই crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "chown `%s`", device->path);

  ঘূৰাই get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

স্থির পূর্ণাঙ্ক
send_mounts (পূর্ণাঙ্ক sync_socket_host, সংরচনা libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  পূর্ণাঙ্ক ret;

  ret = TEMP_FAILURE_RETRY (লিখ (sync_socket_host, &how_many, পরিমাণ (how_many)));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "write to sync socket");

  বাবে (i = 0; i < total; i++)
    {
      যদি (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (ৱর্ণ *) &i, পরিমাণ (i), err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
    }
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, পূর্ণাঙ্ক sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map সংরচনা libcrun_fd_map *mount_fds = রিক্ত;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free ৱর্ণ *rootfs = রিক্ত;
  size_t how_many = 0;
  size_t i;
  পূর্ণাঙ্ক ret;

  যদি (def->mounts_len == 0)
    ঘূৰাই 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  যদি (def->মূল && def->মূল->path)
    rootfs = realpath (def->মূল->path, রিক্ত);

  যদি (! has_userns)
    {
      পূর্ণাঙ্ক is_in_userns = check_running_in_user_namespace (err);
      যদি (UNLIKELY (is_in_userns < 0))
        ঘূৰাই is_in_userns;

      has_userns = is_in_userns > 0;
    }

  বাবে (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      পূর্ণাঙ্ক mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      যদি (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == রিক্ত)
        {
          unsigned long propagation = 0;

          যদি (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, রিক্ত, রিক্ত, রিক্ত, রিক্ত) & ALL_PROPAGATIONS_NO_REC;
          যদি (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          যদি (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      যদি (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  ঘূৰাই send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

স্থির পূর্ণাঙ্ক
prepare_and_send_dev_mounts (libcrun_container_t *container, পূর্ণাঙ্ক sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map সংরচনা libcrun_fd_map *dev_fds = রিক্ত;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close পূর্ণাঙ্ক current_mountns = -1;
  cleanup_free ৱর্ণ *state_dir = রিক্ত;
  cleanup_free ৱর্ণ *devs_path = রিক্ত;
  cleanup_close পূর্ণাঙ্ক devs_mountfd = -1;
  cleanup_close পূর্ণাঙ্ক targetfd = -1;
  ধ্রুৱ ৱর্ণ *context_type = রিক্ত;
  ধ্রুৱ ৱর্ণ *label = রিক্ত;
  size_t how_many = 0;
  size_t i;
  পূর্ণাঙ্ক ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  যদি (def->linux == রিক্ত || def->linux->devices_len == 0)
    ঘূৰাই 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  যদি (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    ঘূৰাই send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_মূল : রিক্ত),
                                     (container->context ? container->context->id : রিক্ত), err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = mkdir (devs_path, 0700);
  যদি (UNLIKELY (ret < 0 && errno != EEXIST))
    ঘূৰাই crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  যদি (UNLIKELY (current_mountns < 0))
    ঘূৰাই current_mountns;

  ret = unshare (CLONE_NEWNS);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (রিক্ত, "/", রিক্ত, MS_REC | MS_PRIVATE, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      গচ্ছ restore_mountns;
    }

  যদি (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      যদি (UNLIKELY (context_type == রিক্ত))
        {
          ret = -1;
          গচ্ছ restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", রিক্ত, context_type, label, রিক্ত);
  যদি (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      গচ্ছ restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      গচ্ছ restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      গচ্ছ restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      গচ্ছ restore_mountns;
    }

  বাবে (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      যদি (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          অনুৱর্ত;
        }

      dev_fds->fds[i] = ret;

      যদি (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  যদি (ns_changed && current_mountns >= 0)
    {
      পূর্ণাঙ্ক setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      যদি (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          ঘূৰাই crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, পূর্ণাঙ্ক sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map সংরচনা libcrun_fd_map *fds = রিক্ত;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (অঙ্ক_NEEDED_DEVS);

  যদি (! has_userns || geteuid () > 0)
    ঘূৰাই send_mounts (sync_socket_host, fds, 0, অঙ্ক_NEEDED_DEVS, err);

  বাবে (i = 0; i < অঙ্ক_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      পূর্ণাঙ্ক fd = syscall_open_বৃক্ষ (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      যদি (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  ঘূৰাই send_mounts (sync_socket_host, fds, how_many, অঙ্ক_NEEDED_DEVS, err);
}

স্থির পূর্ণাঙ্ক
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, পূর্ণাঙ্ক sync_socket_host, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
receive_mounts (সংরচনা libcrun_fd_map *fds, পূর্ণাঙ্ক sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  পূর্ণাঙ্ক ret;

  যদি (fds->nfds == 0)
    ঘূৰাই 0;

  ret = TEMP_FAILURE_RETRY (পঠ (sync_socket_container, &how_many, পরিমাণ (how_many)));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "read from sync socket");

  বাবে (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (ৱর্ণ *) &index, পরিমাণ (index), err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
      যদি (index >= fds->nfds)
        ঘূৰাই crun_make_error (err, 0, "invalid mount data received");

      যদি (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  ঘূৰাই 0;
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
স্থির পূর্ণাঙ্ক
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  যদি (! container->host_has_cap_sys_ptrace)
    ঘূৰাই 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  পূর্ণাঙ্ক ret;

  যদি (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool মূল_mapped = false;

      যদি (def->linux->uid_mappings_len != 0)
        {
          মূল_mapped = মূল_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          যদি (! মূল_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      যদি (def->linux->gid_mappings_len != 0)
        {
          মূল_mapped = মূল_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          যদি (! মূল_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  যদি (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ঘূৰাই crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "setresgid to `%d`", gid);

  ঘূৰাই 0;
}

স্থির bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  ঘূৰাই mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, রিক্ত, রিক্ত);
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
স্থির পূর্ণাঙ্ক
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  সংরচনা libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close পূর্ণাঙ্ক socket_parent = -1;
  cleanup_close পূর্ণাঙ্ক socket_child = -1;
  cleanup_close পূর্ণাঙ্ক মূল_fd = -1;
  cleanup_close পূর্ণাঙ্ক cwd_fd = -1;
  size_t i, missing = 0;
  পূর্ণাঙ্ক saved_errno;
  পূর্ণাঙ্ক sockets[2];
  pid_t pid;
  পূর্ণাঙ্ক ret;

  যদি (def->মূল == রিক্ত || def->মূল->path == রিক্ত)
    ঘূৰাই 0;

  বাবে (i = 0; i < def->mounts_len; i++)
    যদি (is_proc_mount (def->mounts[i]))
      missing++;
  যদি (missing == 0)
    ঘূৰাই 1;

  মূল_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (মূল_fd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (cwd_fd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->মূল->path);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই 0;

  pid = fork ();
  saved_errno = errno;
  যদি (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      যদি (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (শূন্য) unshare (CLONE_NEWNS);

      বাবে (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close পূর্ণাঙ্ক fd = -1;

          যদি (! is_proc_mount (def->mounts[i]))
            অনুৱর্ত;

          fd = open_mount_of_type (container, def->mounts[i], err);
          যদি (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (ৱর্ণ *) &i, পরিমাণ (i), err);
          যদি (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  যদি (UNLIKELY (fchdir (মূল_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    ঘূৰাই crun_make_error (err, errno, "restore the root directory");

  যদি (UNLIKELY (pid < 0))
    ঘূৰাই crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  যেতিয়ালৈ (missing > 0)
    {
      libcrun_error_t tmp_err = রিক্ত;

      ret = receive_fd_from_socket_with_payload (socket_parent, (ৱর্ণ *) &i, পরিমাণ (i), &tmp_err);
      যদি (ret < 0)
        {
          crun_error_release (&tmp_err);
          ভঙ্গ;
        }
      যদি (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          ভঙ্গ;
        }

      যদি (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, রিক্ত, 0);

  ঘূৰাই missing == 0 ? 1 : 0;
}

স্থির পূর্ণাঙ্ক
init_container (libcrun_container_t *container, পূর্ণাঙ্ক sync_socket_container, সংরচনা init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  সংরচনা libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close পূর্ণাঙ্ক timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  পূর্ণাঙ্ক ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      যদি (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      যদি (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      যদি (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          delayed_pidns_fork = ret > 0;
        }

      যদি (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          ret = TEMP_FAILURE_RETRY (লিখ (sync_socket_container, &new_pid, পরিমাণ (new_pid)));
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "write to sync socket");
        }
      অন্যথা
        {
          new_pid = fork ();
          যদি (UNLIKELY (new_pid < 0))
            ঘূৰাই crun_make_error (err, errno, "fork");
        }

      যদি (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          যদি (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (লিখ (sync_socket_container, &new_pid, পরিমাণ (new_pid)));
          যদি (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ret = libcrun_set_oom (container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  যদি ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      বাবে (i = 0; i < def->mounts_len; i++)
        {
          পূর্ণাঙ্ক fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          যদি (def->mounts[i]->type == রিক্ত)
            অনুৱর্ত;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          যদি (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, রিক্ত, রিক্ত, রিক্ত);
          যদি (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, রিক্ত, রিক্ত, রিক্ত);

          যদি (fd >= 0)
            {
              যদি (mount_fds->fds[i] >= 0)
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
  যদি (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      যদি (UNLIKELY (timens_offsets_fd < 0))
        ঘূৰাই timens_offsets_fd;
    }

  যদি (init_status->all_namespaces & CLONE_NEWUSER)
    {
      যদি (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }

      যদি (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
      অন্যথা
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "unshare");
    }

  যদি (def->linux->time_offsets)
    {
      ৱর্ণ fmt_buffer[128];
      পূর্ণাঙ্ক fd = timens_offsets_fd;

      যদি (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, পরিমাণ (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          যদি (UNLIKELY (ret >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buffer)))
            ঘূৰাই crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long পূর্ণাঙ্ক) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = লিখ (fd, fmt_buffer, strlen (fmt_buffer));
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      যদি (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long পূর্ণাঙ্ক) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, পরিমাণ (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          যদি (UNLIKELY (ret >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buffer)))
            ঘূৰাই crun_make_error (err, 0, "internal error: static buffer too small");

          ret = লিখ (fd, fmt_buffer, strlen (fmt_buffer));
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  যদি (init_status->must_fork)
    {
      যদি (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          delayed_pidns_fork = ret > 0;
        }

      যদি (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          যদি (UNLIKELY (pid_container < 0))
            ঘূৰাই crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      যদি (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          ret = TEMP_FAILURE_RETRY (লিখ (sync_socket_container, &pid_container, পরিমাণ (pid_container)));
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "write to sync socket");

          যদি (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  ঘূৰাই get_private_data (container)->delayed_pidns_fork;
}

পূর্ণাঙ্ক
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক client_fd = -1;
  cleanup_close পূর্ণাঙ্ক pidfd = -1;
  ধ্রুৱ ৱর্ণ *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  যদি (v == রিক্ত)
    ঘূৰাই 0;

  pidfd = syscall_pidfd_open (pid, 0);
  যদি (UNLIKELY (pidfd < 0))
    ঘূৰাই crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  যদি (UNLIKELY (client_fd < 0))
    ঘূৰাই client_fd;

  ঘূৰাই send_fd_to_socket (client_fd, pidfd, err);
}

স্থির bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  যদি (process == রিক্ত || process->exec_cpu_affinity == রিক্ত)
    ঘূৰাই false;
  ঘূৰাই (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, শূন্য *args,
                             পূর্ণাঙ্ক *sync_socket_out, সংরচনা libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) সংরচনা init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close পূর্ণাঙ্ক sync_socket_container = -1;
  ৱর্ণ *notify_socket_env = রিক্ত;
  cleanup_close পূর্ণাঙ্ক sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close পূর্ণাঙ্ক restore_pidns = -1;
  পূর্ণাঙ্ক first_clone_args = 0;
  পূর্ণাঙ্ক sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  পূর্ণাঙ্ক ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  যদি (def->মূল)
    {
      ret = do_notify_socket (container, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  যদি (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ret = libcrun_set_oom (container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->মূল ? container->container_def->মূল->path : রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  বাবে (i = 0; i < init_status.fd_len; i++)
    {
      ৱিকল্প (init_status.মান[i])
        {
        পক্ষ CLONE_NEWIPC:
          যদি (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          ভঙ্গ;

        পক্ষ CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          যদি (! new_userns)
            init_status.must_fork = true;
          অন্যথা
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          ভঙ্গ;

        পক্ষ CLONE_NEWTIME:
          যদি (! new_userns)
            init_status.must_fork = true;
          অন্যথা
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          ভঙ্গ;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  যদি ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  অন্যথা যদি ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  যদি (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  যদি (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      সংরচনা _clone3_args clone3_args;
      memset (&clone3_args, 0, পরিমাণ (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      যদি (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  যদি (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, রিক্ত);
      যদি (UNLIKELY (pid < 0))
        ঘূৰাই crun_make_error (err, errno, "clone");
    }

  যদি (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = close_and_reset (&sync_socket_container);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_বৃক্ষ_fd));

      যদি (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          ret = TEMP_FAILURE_RETRY (পঠ (sync_socket_host, &new_pid, পরিমাণ (new_pid)));
          যদি (UNLIKELY (ret != পরিমাণ (new_pid)))
            ঘূৰাই crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          যদি (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          অন্যথা
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, রিক্ত, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }

      যদি (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }

      যদি ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }

      যদি (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          ret = TEMP_FAILURE_RETRY (পঠ (sync_socket_host, &grandchild, পরিমাণ (grandchild)));
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;

          যদি (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          অন্যথা
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, রিক্ত, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      যদি (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }

      pid_to_clean = 0;
      ঘূৰাই pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  যদি (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  যদি (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  অন্যথা
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      যদি (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->বাৰ্তা);
    }

  /* Jump into the specified entrypoint.  */
  যদি (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  যদি (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->বাৰ্তা);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  যদি (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

স্থির পূর্ণাঙ্ক
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, পূর্ণাঙ্ক sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, ধ্রুৱ ৱর্ণ *sub_cgroup,
                            পূর্ণাঙ্ক *terminal_fd, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  ৱর্ণ res;
  pid_t pid;
  cleanup_close পূর্ণাঙ্ক sync_fd = sync_socket_fd;

  যদি (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (পঠ (sync_fd, &res, 1));
  যদি (UNLIKELY (ret != 1))
    ঘূৰাই crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  যদি (res != '0')
    ঘূৰাই crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (পঠ (sync_fd, &pid, পরিমাণ (pid)));
  যদি (UNLIKELY (ret != পরিমাণ (pid)))
    ঘূৰাই crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, রিক্ত, 0);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "waitpid for exec child pid");

  যদি (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  যদি (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  যদি (need_move_to_cgroup)
    {
      cleanup_cgroup_status সংরচনা libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free ৱর্ণ *final_cgroup = রিক্ত;

      যদি (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, রিক্ত);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = libcrun_set_io_priority (pid, process, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (লিখ (sync_fd, &ret, পরিমাণ (ret)));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "write to sync socket");

  যদি (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      যদি (UNLIKELY (ret < 0))
        {
          পূর্ণাঙ্ক err_code = 0;
          cleanup_free ৱর্ণ *err_str = রিক্ত;

          যদি (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              যদি (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  ঘূৰাই crun_make_error (err, err_code, "%s", err_str);
                }
            }

          ঘূৰাই crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  ঘূৰাই pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
স্থির পূর্ণাঙ্ক
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close পূর্ণাঙ্ক pidfd_pid_to_join = -1;
  পূর্ণাঙ্ক all_flags = 0;
  size_t i;
  পূর্ণাঙ্ক ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  যদি (def->linux && def->linux->namespaces)
    {
      বাবে (i = 0; i < def->linux->namespaces_len; i++)
        যদি (! is_empty_string (def->linux->namespaces[i]->path))
          ঘূৰাই 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  যদি (UNLIKELY (pidfd_pid_to_join < 0))
    ঘূৰাই 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;
  যদি (ret == 0)
    ঘূৰাই crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  বাবে (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].মান;

  ret = setns (pidfd_pid_to_join, all_flags);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই 0;

  ঘূৰাই 1;
}

স্থির পূর্ণাঙ্ক
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  পূর্ণাঙ্ক fds_joined[MAX_NAMESPACES] = {
    0,
  };
  পূর্ণাঙ্ক fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  পূর্ণাঙ্ক ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;
  /* Nothing left to do if the namespaces were joined.  */
  যদি (LIKELY (ret > 0))
    ঘূৰাই 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  যদি (def->linux->namespaces_len >= MAX_NAMESPACES)
    ঘূৰাই crun_make_error (err, 0, "invalid configuration");

  বাবে (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free ৱর্ণ *ns_path = রিক্ত;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      যদি (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          যদি (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              অনুৱর্ত;
            }

          ret = fds[i];
          গচ্ছ exit;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ exit;
  যদি (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      গচ্ছ exit;
    }

  বাবে (i = 0; namespaces[i].ns_file; i++)
    {
      যদি (namespaces[i].মান == CLONE_NEWUSER)
        অনুৱর্ত;

      ret = setns (fds[i], 0);
      যদি (ret == 0)
        fds_joined[i] = 1;
    }
  বাবে (i = 0; namespaces[i].ns_file; i++)
    {
      যদি (fds_joined[i])
        অনুৱর্ত;
      ret = setns (fds[i], 0);
      যদি (ret == 0)
        fds_joined[i] = 1;
    }
  বাবে (i = 0; namespaces[i].ns_file; i++)
    {
      যদি (fds_joined[i])
        অনুৱর্ত;
      ret = setns (fds[i], 0);
      যদি (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          বাবে (j = 0; j < def->linux->namespaces_len; j++)
            {
              যদি (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  ভঙ্গ;
                }
            }
          যদি (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              অনুৱর্ত;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          গচ্ছ exit;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

exit:
  বাবে (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  ঘূৰাই ret;
}

পূর্ণাঙ্ক
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      ধ্রুৱ ৱর্ণ *sub_cgroup,
                      পূর্ণাঙ্ক detach,
                      runtime_spec_schema_config_schema_process *process,
                      পূর্ণাঙ্ক *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক sync_socket_fd[2];
  cleanup_close পূর্ণাঙ্ক cgroup_dirfd = -1;
  cleanup_close পূর্ণাঙ্ক sync_fd = -1;
  সংরচনা _clone3_args clone3_args;
  bool need_move_to_cgroup;

  যদি (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "set child subreaper");
    }

  যদি (path_has_dot_dot_component (sub_cgroup))
    ঘূৰাই crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  যদি (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free ৱর্ণ *final_cgroup = রিক্ত;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status সংরচনা libcrun_cgroup_status *cgroup_status = রিক্ত;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    যদি (get_force_cgroup_v1_annotation (container) == রিক্ত)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        যদি (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, পরিমাণ (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  যদি (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  অন্যথা
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  যদি (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  /* On errors, fall back to fork().  */
  যদি (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      যদি (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          গচ্ছ exit;
        }
    }

  যদি (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      ঘূৰাই join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  যদি (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (লিখ (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->বাৰ্তা);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  যদি (UNLIKELY (pid < 0))
    {
      পূর্ণাঙ্ক saved_errno = errno;
      TEMP_FAILURE_RETRY (লিখ (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  যদি (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (লিখ (sync_fd, "0", 1));
      যদি (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (লিখ (sync_fd, &pid, পরিমাণ (pid)));
      যদি (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  অন্যথা
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free ৱর্ণ *pty = রিক্ত;
      পূর্ণাঙ্ক r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (পঠ (sync_fd, &r, পরিমাণ (r)));
      যদি (UNLIKELY (ret != পরিমাণ (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      যদি (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      যদি (terminal_fd)
        {
          cleanup_close পূর্ণাঙ্ক ptmx_fd = -1;

          ret = set_id_init (container, err);
          যদি (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          যদি (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          যদি (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  ঘূৰাই 0;

exit:
  যদি (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  যদি (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  ঘূৰাই ret;
}

পূর্ণাঙ্ক
libcrun_linux_container_update (libcrun_container_status_t *status, ধ্রুৱ ৱর্ণ *state_মূল, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status সংরচনা libcrun_cgroup_status *cgroup_status = রিক্ত;

  cgroup_status = libcrun_cgroup_make_status (status);

  ঘূৰাই libcrun_update_cgroup_resources (cgroup_status, state_মূল, resources, err);
}

স্থির পূর্ণাঙ্ক
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, ধ্রুৱ bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status সংরচনা libcrun_cgroup_status *cgroup_status = রিক্ত;

  cgroup_status = libcrun_cgroup_make_status (status);

  ঘূৰাই libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

পূর্ণাঙ্ক
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ঘূৰাই libcrun_container_pause_unpause_linux (status, true, err);
}

পূর্ণাঙ্ক
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ঘূৰাই libcrun_container_pause_unpause_linux (status, false, err);
}

পূর্ণাঙ্ক
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  পূর্ণাঙ্ক ret;

  যদি (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  অন্যথা যদি (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  অন্যথা
    ঘূৰাই crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, 0, "personality to `%s`", p->domain);

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close পূর্ণাঙ্ক sockfd = -1;

  বাবে (i = 0; i < def->linux->namespaces_len; i++)
    {
      পূর্ণাঙ্ক মান = libcrun_find_namespace (def->linux->namespaces[i]->type);
      যদি (UNLIKELY (মান < 0))
        ঘূৰাই crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      যদি (মান == CLONE_NEWNET && def->linux->namespaces[i]->path == রিক্ত)
        {
          configure_network = true;
          ভঙ্গ;
        }
    }

  যদি (! configure_network)
    ঘূৰাই 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  যদি (LIKELY (sockfd >= 0))
    {
      সংরচনা ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  অন্যথা
    {
      সংরচনা nlmsghdr *hdr_recv;
      ৱর্ণ buf[পরিমাণ (সংরচনা nlmsghdr) + পরিমাণ (সংরচনা nlmsgerr)];
      সংরচনা sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      সংরচনা sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      সংরচনা nlmsghdr hdr = {
        .nlmsg_len = পরিমাণ (সংরচনা nlmsghdr) + পরিমাণ (সংরচনা ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      সংরচনা ifinfomsg বাৰ্তা = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((সংরচনা nlmsghdr *) buf) = hdr;
      *((সংরচনা ifinfomsg *) (buf + পরিমাণ (সংরচনা nlmsghdr))) = বাৰ্তা;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      যদি (UNLIKELY (sockfd < 0))
        ঘূৰাই crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (সংরচনা sockaddr *) &addr, পরিমাণ (addr));
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, পরিমাণ (buf), 0, (সংরচনা sockaddr *) &addr_to, পরিমাণ (addr_to));
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, পরিমাণ (buf), 0, রিক্ত, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (সংরচনা nlmsghdr *) buf;

      যদি (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          সংরচনা nlmsgerr *err_data = (সংরচনা nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          যদি (err_data->error < 0)
            ঘূৰাই crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  ঘূৰাই 0;
}

/* Protection for attacks like CVE-2019-5736.  */
পূর্ণাঙ্ক ensure_cloned_দ্বিমিক ();
__attribute__ ((constructor)) স্থির শূন্য
libcrun_rexec (শূন্য)
{
  যদি (ensure_cloned_দ্বিমিক () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

পূর্ণাঙ্ক
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ঘূৰাই libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

পূর্ণাঙ্ক
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
স্থির পূর্ণাঙ্ক
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, পূর্ণাঙ্ক সঙ্কেত, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  যদি (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      /* The pid is not valid anymore, return an error.  */
      যদি (ret == 0)
        {
          errno = ESRCH;
          ঘূৰাই crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, সঙ্কেত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "kill container");
  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_kill_linux (libcrun_container_status_t *status, পূর্ণাঙ্ক সঙ্কেত, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_close পূর্ণাঙ্ক pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  যদি (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      যদি (errno == ENOSYS)
        ঘূৰাই libcrun_kill_linux_no_pidfd (status, true, সঙ্কেত, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      যদি (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        ঘূৰাই crun_make_error (err, ESRCH, "process not running");
      ঘূৰাই crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  /* The pid is not valid anymore, return an error.  */
  যদি (ret == 0)
    {
      errno = ESRCH;
      ঘূৰাই crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, সঙ্কেত, রিক্ত, 0);
  যদি (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      যদি (errno == ENOSYS)
        ঘূৰাই libcrun_kill_linux_no_pidfd (status, false, সঙ্কেত, err);
      ঘূৰাই crun_make_error (err, errno, "send signal to pidfd");
    }

  ঘূৰাই 0;
}

স্থির ধ্রুৱ ৱর্ণ *
libcrun_get_intelrdt_name (ধ্রুৱ ৱর্ণ *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  যদি (def == রিক্ত || def->linux == রিক্ত || def->linux->intel_rdt == রিক্ত || def->linux->intel_rdt->clos_id == রিক্ত)
    {
      যদি (explicit)
        *explicit = false;
      ঘূৰাই ctr_name;
    }

  যদি (explicit)
    *explicit = true;
  ঘূৰাই def->linux->intel_rdt->clos_id;
}

পূর্ণাঙ্ক
libcrun_apply_intelrdt (ধ্রুৱ ৱর্ণ *ctr_name, libcrun_container_t *container, pid_t pid, পূর্ণাঙ্ক actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = রিক্ত;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  ধ্রুৱ ৱর্ণ *name;
  পূর্ণাঙ্ক ret;

  যদি (container)
    def = container->container_def;

  যদি (def == রিক্ত || def->linux == রিক্ত || def->linux->intel_rdt == রিক্ত)
    ঘূৰাই 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  যদি (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      যদি (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          যদি (UNLIKELY (ret < 0))
            গচ্ছ fail;

          has_monitoring = true;
        }
    }

  যদি (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ fail;
    }

  যদি (actions & LIBCRUN_INTELRDT_MOVE)
    {
      ধ্রুৱ ৱর্ণ *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : রিক্ত;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ fail;
    }

  ঘূৰাই 0;

fail:
  যদি (has_monitoring)
    {
      libcrun_error_t tmp_err = রিক্ত;
      পূর্ণাঙ্ক tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      যদি (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  যদি (created)
    {
      libcrun_error_t tmp_err = রিক্ত;
      পূর্ণাঙ্ক tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      যদি (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  ঘূৰাই ret;
}

পূর্ণাঙ্ক
libcrun_destroy_intelrdt (ধ্রুৱ ৱর্ণ *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  ধ্রুৱ ৱর্ণ *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  যদি (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      পূর্ণাঙ্ক ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      যদি (ret < 0)
        ঘূৰাই ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  যদি (explicit)
    ঘূৰাই 0;

  ঘূৰাই resctl_destroy (clos_id, err);
}

পূর্ণাঙ্ক
libcrun_update_intel_rdt (ধ্রুৱ ৱর্ণ *ctr_name, libcrun_container_t *container, ধ্রুৱ ৱর্ণ *l3_cache_schema, ধ্রুৱ ৱর্ণ *mem_bw_schema, ৱর্ণ *ধ্রুৱ *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = রিক্ত;
  ধ্রুৱ ৱর্ণ *name;

  যদি (container)
    def = container->container_def;

  যদি (def == রিক্ত || def->linux == রিক্ত || def->linux->intel_rdt == রিক্ত)
    ঘূৰাই 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, রিক্ত);

  ঘূৰাই resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
পূর্ণাঙ্ক
libcrun_safe_chdir (ধ্রুৱ ৱর্ণ *path, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *buffer = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = chdir (path);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  যদি (ret == 0 || buffer[0] != '/')
    {
      যদি (chdir ("/") < 0)
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
#define UNREACHABLE_LEN ((পূর্ণাঙ্ক) পরিমাণ (UNREACHABLE) - 1)

      যদি ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        ঘূৰাই crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      ঘূৰাই crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  ঘূৰাই 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
স্থির পূর্ণাঙ্ক
run_callback_in_namespace (পূর্ণাঙ্ক mntns_fd, পূর্ণাঙ্ক (*callback) (শূন্য *, libcrun_error_t *), শূন্য *arg, libcrun_error_t *err)
{
  পূর্ণাঙ্ক wait_status = 0;
  pid_t pid;
  পূর্ণাঙ্ক ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  যদি (UNLIKELY (pid < 0))
    ঘূৰাই crun_make_error (err, errno, "vfork");

  যদি (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      যদি (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      যদি (UNLIKELY (ret < 0))
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
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  ঘূৰাই get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
স্থির পূর্ণাঙ্ক
check_container_userns (libcrun_container_t *container, পূর্ণাঙ্ক userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close পূর্ণাঙ্ক expected_fd = -1;
  cleanup_close পূর্ণাঙ্ক parent_fd = -1;
  সংরচনা stat st, expected_st;
  ধ্রুৱ ৱর্ণ *path = রিক্ত;
  size_t i;
  পূর্ণাঙ্ক ret;

  যদি (def->linux)
    বাবে (i = 0; i < def->linux->namespaces_len; i++)
      যদি (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == রিক্ত;
          ভঙ্গ;
        }

  যদি (path == রিক্ত)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      যদি (UNLIKELY (expected_fd < 0))
        ঘূৰাই expected_fd;
    }
  অন্যথা
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      যদি (UNLIKELY (expected_fd < 0))
        ঘূৰাই crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  যদি (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      যদি (UNLIKELY (parent_fd < 0))
        ঘূৰাই crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "fstat user namespace");

  যদি (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    ঘূৰাই crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  ঘূৰাই 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
স্থির পূর্ণাঙ্ক
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, পূর্ণাঙ্ক *mntns_fd, পূর্ণাঙ্ক *owner_fd, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক mntns = -1;
  cleanup_close পূর্ণাঙ্ক owner = -1;
  পূর্ণাঙ্ক ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  যদি (UNLIKELY (mntns < 0))
    ঘূৰাই mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (ret == 0)
    ঘূৰাই crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  যদি (UNLIKELY (owner < 0))
    ঘূৰাই crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, পূর্ণাঙ্ক (*callback) (শূন্য *, libcrun_error_t *), শূন্য *arg, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক mntns_fd = -1;
  cleanup_close পূর্ণাঙ্ক owner_fd = -1;
  পূর্ণাঙ্ক ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই run_callback_in_namespace (mntns_fd, callback, arg, err);
}

সংরচনা umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
স্থির পূর্ণাঙ্ক
umount_nofollow (ধ্রুৱ ৱর্ণ *destination, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক parentfd = -1;
  cleanup_close পূর্ণাঙ্ক cwdfd = -1;
  ৱর্ণ buffer[PATH_MAX];
  ধ্রুৱ ৱর্ণ *parent, *name;
  ৱর্ণ *last_slash;
  size_t len;
  পূর্ণাঙ্ক saved_errno;
  পূর্ণাঙ্ক ret;

  যদি (is_empty_string (destination))
    ঘূৰাই crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  যদি (UNLIKELY (len >= পরিমাণ (buffer)))
    ঘূৰাই crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  যেতিয়ালৈ (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  যদি (last_slash == রিক্ত)
    {
      parent = ".";
      name = buffer;
    }
  অন্যথা যদি (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  অন্যথা
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  যদি (UNLIKELY (parentfd < 0))
    ঘূৰাই crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  যদি (UNLIKELY (cwdfd < 0))
    ঘূৰাই crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  যদি (UNLIKELY (fchdir (cwdfd) < 0))
    ঘূৰাই crun_make_error (err, errno, "fchdir to the previous working directory");

  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, saved_errno, "umount `%s`", destination);

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
do_umount_in_a_container (শূন্য *arg, libcrun_error_t *err)
{
  সংরচনা umount_in_a_container_args *args = arg;
  size_t i;
  পূর্ণাঙ্ক ret;

  বাবে (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ঘূৰাই 0;
}

সংরচনা mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  সংরচনা libcrun_fd_map *fds;
  পূর্ণাঙ্ক pidfd;
  পূর্ণাঙ্ক owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
স্থির পূর্ণাঙ্ক
open_destination_nofollow (ধ্রুৱ ৱর্ণ *destination, bool dir, libcrun_error_t *err)
{
  ধ্রুৱ uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  পূর্ণাঙ্ক fd;
  পূর্ণাঙ্ক ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  যদি (LIKELY (fd >= 0))
    ঘূৰাই fd;

  যদি (errno != ENOENT)
    ঘূৰাই crun_make_error (err, errno, "open `%s`", destination);

  যদি (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  অন্যথা
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  যদি (UNLIKELY (fd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s`", destination);

  ঘূৰাই fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
স্থির পূর্ণাঙ্ক
create_destinations_in_a_container (শূন্য *arg, libcrun_error_t *err)
{
  সংরচনা mount_in_a_container_args *args = arg;
  size_t i;
  পূর্ণাঙ্ক ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  যদি (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "setresuid to `0`");
    }
  অন্যথা যদি (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ঘূৰাই crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  বাবে (i = 0; i < args->len; i++)
    {
      cleanup_close পূর্ণাঙ্ক dest_fd = -1;
      সংরচনা stat st;

      ret = fstat (args->fds->fds[i], &st);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      যদি (UNLIKELY (dest_fd < 0))
        ঘূৰাই dest_fd;
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
do_mount_in_a_container (শূন্য *arg, libcrun_error_t *err)
{
  সংরচনা mount_in_a_container_args *args = arg;
  size_t i;
  পূর্ণাঙ্ক ret;

  বাবে (i = 0; i < args->len; i++)
    {
      cleanup_close পূর্ণাঙ্ক dest_fd = -1;
      সংরচনা stat st;

      ret = fstat (args->fds->fds[i], &st);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      যদি (UNLIKELY (dest_fd < 0))
        ঘূৰাই dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  ঘূৰাই 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
স্থির পূর্ণাঙ্ক
send_mounts_from_container_namespaces (libcrun_container_t *container, পূর্ণাঙ্ক pidfd, runtime_spec_schema_defs_mount **mounts,
                                       সংরচনা libcrun_fd_map *fds, size_t len, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক owner_fd, পূর্ণাঙ্ক sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  পূর্ণাঙ্ক ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  যদি (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  অন্যথা যদি (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ঘূৰাই crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  যদি (UNLIKELY (pid < 0))
    ঘূৰাই crun_make_error (err, errno, "fork");

  যদি (pid)
    {
      পূর্ণাঙ্ক wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      যদি (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, রিক্ত, 0));
          ঘূৰাই crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  বাবে (i = 0; i < len; i++)
    {
      cleanup_close পূর্ণাঙ্ক fd = -1;

      যদি (fds->fds[i] >= 0)
        অনুৱর্ত;

      fd = open_mount_of_type (container, mounts[i], err);
      যদি (UNLIKELY (fd < 0))
        ঘূৰাই crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ঘূৰাই 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
স্থির পূর্ণাঙ্ক
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, পূর্ণাঙ্ক *flags, libcrun_error_t *err)
{
  স্থির ধ্রুৱ সংরচনা
  {
    ধ্রুৱ ৱর্ণ *name;
    পূর্ণাঙ্ক flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  বাবে (i = 0; i < পরিমাণ (namespaces) / পরিমাণ (namespaces[0]); i++)
    {
      cleanup_close পূর্ণাঙ্ক self_fd = -1;
      cleanup_close পূর্ণাঙ্ক target_fd = -1;
      সংরচনা stat self_st, target_st;
      cleanup_free ৱর্ণ *self_path = রিক্ত;
      পূর্ণাঙ্ক ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      যদি (UNLIKELY (self_fd < 0))
        ঘূৰাই self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      যদি (UNLIKELY (target_fd < 0))
        ঘূৰাই target_fd;

      ret = fstat (self_fd, &self_st);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      যদি (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  ঘূৰাই 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
স্থির পূর্ণাঙ্ক
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, পূর্ণাঙ্ক owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, সংরচনা libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক sync_fd = -1;
  cleanup_close পূর্ণাঙ্ক pidfd = -1;
  পূর্ণাঙ্ক sync_socket_fd[2];
  bool needed = false;
  পূর্ণাঙ্ক flags;
  pid_t pid;
  size_t i;
  পূর্ণাঙ্ক ret;

  বাবে (i = 0; i < len; i++)
    {
      যদি (fds->fds[i] >= 0)
        অনুৱর্ত;

      যদি (mounts[i]->type == রিক্ত)
        ঘূৰাই crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  যদি (! needed)
    ঘূৰাই 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  যদি (UNLIKELY (pidfd < 0))
    ঘূৰাই crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (ret == 0)
    ঘূৰাই crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  যদি (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      ঘূৰাই ret;
    }

  যদি (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      যদি (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  বাবে (i = 0; i < len; i++)
    {
      যদি (fds->fds[i] >= 0)
        অনুৱর্ত;

      ret = receive_fd_from_socket (sync_fd, err);
      যদি (UNLIKELY (ret < 0))
        {
          cleanup_free ৱর্ণ *err_str = রিক্ত;
          পূর্ণাঙ্ক err_code = 0;

          যদি (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          অন্যথা
            ret = crun_error_wrap (err, "receive mount fd");
          ভঙ্গ;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, রিক্ত, 0));

  ঘূৰাই ret;
}

স্থির unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free ৱর্ণ *data = রিক্ত;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  যদি (mount->options == রিক্ত)
    ঘূৰাই get_default_flags (container, mount->destination, &data);

  বাবে (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  ঘূৰাই flags;
}

পূর্ণাঙ্ক
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  সংরচনা mount_in_a_container_args args;
  cleanup_close_map সংরচনা libcrun_fd_map *fds = রিক্ত;
  cleanup_close পূর্ণাঙ্ক pidfd = -1;
  cleanup_close পূর্ণাঙ্ক mntns_fd = -1;
  cleanup_close পূর্ণাঙ্ক owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  পূর্ণাঙ্ক ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  fds = make_libcrun_fd_map (len);

  বাবে (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), রিক্ত, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      যদি (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          যদি (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          যদি (UNLIKELY (fds->fds[i] < 0))
            ঘূৰাই fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  বাবে (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  যদি (UNLIKELY (ret > 0))
    ঘূৰাই *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

পূর্ণাঙ্ক
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  সংরচনা umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  ঘূৰাই run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

সংরচনা restrict_mounts_in_a_container_args
{
  সংরচনা libcrun_mount_flags_s *mounts;
  size_t len;
  পূর্ণাঙ্ক rootfd;
};

স্থির পূর্ণাঙ্ক
do_restrict_mounts_in_a_container (শূন্য *arg, libcrun_error_t *err)
{
  সংরচনা restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  পূর্ণাঙ্ক ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  যদি (LIKELY (ret == 0))
    ret = chroot (".");
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "chroot to the container root");

  বাবে (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      ধ্রুৱ ৱর্ণ *target = consume_slashes (args->mounts[i].target);
      cleanup_close পূর্ণাঙ্ক fd = -1;
      সংরচনা statfs sfs;
      unsigned long current_flags;

      যদি (flags == 0)
        অনুৱর্ত;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      যদি (UNLIKELY (fd < 0))
        {
          যদি (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              অনুৱর্ত;
            }
          ঘূৰাই fd;
        }

      ret = fstatfs (fd, &sfs);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      যদি ((current_flags & flags) == flags)
        অনুৱর্ত;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      যদি (LIKELY (ret == 0))
        অনুৱর্ত;

      যদি (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        ঘূৰাই ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (রিক্ত, target, রিক্ত, MS_REMOUNT | MS_BIND | flags | current_flags, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, errno, "remount `/%s`", target);
    }

  ঘূৰাই 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
পূর্ণাঙ্ক
libcrun_restrict_mounts_in_container (pid_t pid, সংরচনা libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  সংরচনা restrict_mounts_in_a_container_args args;
  cleanup_close পূর্ণাঙ্ক mntns_fd = -1;
  cleanup_close পূর্ণাঙ্ক rootfd = -1;
  ৱর্ণ path[64];
  পূর্ণাঙ্ক ret;

  snprintf (path, পরিমাণ (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  যদি (UNLIKELY (mntns_fd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, পরিমাণ (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  যদি (UNLIKELY (rootfd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  যদি (UNLIKELY (ret > 0))
    ঘূৰাই *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  ঘূৰাই ret;
}

পূর্ণাঙ্ক
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close পূর্ণাঙ্ক netns_fd = -1;
  size_t i;
  পূর্ণাঙ্ক ret;

  যদি (def == রিক্ত || def->linux == রিক্ত || def->linux->net_devices == রিক্ত)
    ঘূৰাই 0;

  cleanup_free ৱর্ণ *ns_path = রিক্ত;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  যদি (UNLIKELY (netns_fd < 0))
    ঘূৰাই netns_fd;

  বাবে (i = 0; i < def->linux->net_devices->len; i++)
    {
      ধ্রুৱ ৱর্ণ *new_name = def->linux->net_devices->মানসমূহ[i]->name ?: def->linux->net_devices->চাবিগুলি[i];

      ret = move_network_device (def->linux->net_devices->চাবিগুলি[i], new_name, netns_fd, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ঘূৰাই 0;
}
