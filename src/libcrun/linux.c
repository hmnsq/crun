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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "linux.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "status.h"
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/mount.h>
#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#எனில் defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#endif

#அந்தர்பாவ <sys/prctl.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#அந்தர்பாவ <sys/time.h>
#அந்தர்பாவ <sys/resource.h>
#அந்தர்பாவ <sys/sysmacros.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <grp.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ "terminal.h"
#அந்தர்பாவ "cgroup.h"
#அந்தர்பாவ "cgroup-utils.h"
#அந்தர்பாவ "status.h"
#அந்தர்பாவ "criu.h"
#அந்தர்பாவ "scheduler.h"
#அந்தர்பாவ "intelrdt.h"
#அந்தர்பாவ "io_priority.h"
#அந்தர்பாவ "net_device.h"

#அந்தர்பாவ <sys/socket.h>
#அந்தர்பாவ <libgen.h>
#அந்தர்பாவ <sys/wait.h>
#அந்தர்பாவ <sys/vfs.h>
#அந்தர்பாவ <sys/statvfs.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ <sys/personality.h>
#அந்தர்பாவ <net/if.h>
#அந்தர்பாவ <sys/xattr.h>
#அந்தர்பாவ <linux/netlink.h>
#அந்தர்பாவ <linux/rtnetlink.h>
#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <linux/sched.h>
#அந்தர்பாவ <linux/magic.h>
#அந்தர்பாவ <linux/nsfs.h>

#அந்தர்பாவ <json-c/json.h>
#அந்தர்பாவ <ocispec/json_common.h>

#அந்தர்பாவ "mount_flags.h"
#அந்தர்பாவ "syscalls.h"

#ifndef RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#endif

#ifndef OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#endif

#ifndef OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#endif

#ifndef OPEN_மரம்_NAMESPACE
#  define OPEN_மரம்_NAMESPACE 2
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
நிலையான unsigned long
mount_flags_from_statfs (unsigned long f_flags)
{
  unsigned long flags = 0;

  எனில் (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  எனில் (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  எனில் (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  எனில் (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  எனில் (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  எனில் (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  எனில் (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  எனில் (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  எனில் (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  திரும்பு flags;
}

கட்டமைப்பு remount_s
{
  கட்டமைப்பு remount_s *next;
  எழுத்து *target;
  முழு targetfd;
  unsigned long flags;
  எழுத்து *data;
};

கட்டமைப்பு private_data_s
{
  கட்டமைப்பு remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  முழு unshare_flags;
  முழு unshare_cgroupns;

  எழுத்து *host_notify_socket_path;
  எழுத்து *container_notify_socket_path;
  எழுத்து *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  முழு procfd;

  மாறிலி எழுத்து *rootfs;
  முழு rootfsfd;

  முழு notify_socket_மரம்_fd;
  முழு old_வேர்_fd;

  கட்டமைப்பு libcrun_fd_map *mount_fds;
  கட்டமைப்பு libcrun_fd_map *dev_fds;
  கட்டமைப்பு libcrun_fd_map *needed_devs_fds;

  எழுத்து **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  எழுத்து *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  முழு maskdir_fd;
  எழுத்து *maskdir_proc_path;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_pivot;
  bool no_pivot;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_வேர்_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  எழுத்து *host_rootfs;
};

கட்டமைப்பு linux_namespace_s
{
  மாறிலி எழுத்து *name;
  மாறிலி எழுத்து *ns_file;
  முழு மதிப்பு;
};

நிலையான வெறுமை
cleanup_private_data (வெறுமை *private_data)
{
  கட்டமைப்பு private_data_s *p = private_data;

  எனில் (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  எனில் (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  எனில் (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  எனில் (p->old_வேர்_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_வேர்_fd));
  எனில் (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  எனில் (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  எனில் (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  எனில் (p->copy_symlink_targets)
    {
      size_t i;
      க்கு (i = 0; i < p->n_copy_symlink_targets; i++)
        விடுவி (p->copy_symlink_targets[i]);
      விடுவி (p->copy_symlink_targets);
    }
  விடுவி (p->unified_cgroup_path);
  விடுவி (p->host_notify_socket_path);
  விடுவி (p->container_notify_socket_path);
  விடுவி (p->external_descriptors);
  விடுவி (p->maskdir_proc_path);
  விடுவி (p->host_rootfs);
  விடுவி (p);
}

நிலையான கட்டமைப்பு private_data_s *
get_private_data (கட்டமைப்பு libcrun_container_s *container)
{
  எனில் (container->private_data == வெற்று)
    {
      கட்டமைப்பு private_data_s *p = xmalloc0 (அளவு (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_மரம்_fd = -1;
      p->old_வேர்_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  திரும்பு container->private_data;
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

நிலையான கட்டமைப்பு linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { வெற்று, வெற்று, 0 } };

நிலையான முழு
get_and_reset (முழு *old)
{
  முழு tmp = *old;
  *old = -1;
  திரும்பு tmp;
}

முழு
libcrun_find_namespace (மாறிலி எழுத்து *name)
{
  கட்டமைப்பு linux_namespace_s *it;
  க்கு (it = namespaces; it->name; it++)
    எனில் (strcmp (it->name, name) == 0)
      திரும்பு it->மதிப்பு;
  திரும்பு -1;
}

நிலையான முழு
syscall_keyctl_join (மாறிலி எழுத்து *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  திரும்பு (முழு) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

நிலையான முழு
syscall_pidfd_open (pid_t pid, unsigned முழு flags)
{
#எனில் defined __NR_pidfd_open
  திரும்பு (முழு) syscall (__NR_pidfd_open, pid, flags);
#இல்லையெனில்
  (வெறுமை) pid;
  (வெறுமை) flags;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

நிலையான முழு
syscall_pidfd_send_signal (முழு pidfd, முழு sig, siginfo_t *info, unsigned முழு flags)
{
#எனில் defined __NR_pidfd_send_signal
  திரும்பு (முழு) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#இல்லையெனில்
  (வெறுமை) pidfd;
  (வெறுமை) sig;
  (வெறுமை) info;
  (வெறுமை) flags;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
நிலையான uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  எனில் (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  எனில் (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  எனில் (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  எனில் (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  எனில் (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  எனில் (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  எனில் (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  எனில் (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  திரும்பு attr;
}

நிலையான முழு
do_mount_setattr (bool recursive, மாறிலி எழுத்து *target, முழு targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  கட்டமைப்பு mount_attr_s attr = {
    0,
  };
  முழு ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  எனில் (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      எனில் (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      இல்லையெனில் எனில் (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      இல்லையெனில்
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      எனில் (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  இல்லையெனில் எனில் (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "mount_setattr `/%s`", target);

  திரும்பு 0;
}

முழு
get_bind_mount (முழு dirfd, மாறிலி எழுத்து *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close முழு open_மரம்_fd = -1;
  கட்டமைப்பு mount_attr_s attr = {
    0,
  };
  முழு recursive_flag = (recursive ? AT_RECURSIVE : 0);
  முழு ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  எனில் (UNLIKELY (src == வெற்று))
    திரும்பு crun_make_error (err, EINVAL, "mount source is not specified");

  எனில் (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  எனில் (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_மரம்_fd = syscall_open_மரம் (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  எனில் (UNLIKELY (open_மரம்_fd < 0))
    திரும்பு crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_மரம்_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "mount_setattr `%s`", src);

  திரும்பு get_and_reset (&open_மரம்_fd);
}

முழு
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, எழுத்து *option, எழுத்து **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free எழுத்து *mappings = வெற்று;
  மாறிலி எழுத்து *it;
  முழு ret;

  mappings = xmalloc (allocated);

  க்கு (it = option; *it;)
    {
      bool relative = false;
      long மதிப்பு[3];
      size_t i;

      எனில் (*it == '\0')
        நிறுத்து;

      எனில் (*it == '#')
        it++;

      எனில் (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      க்கு (i = 0; i < 3; i++)
        {
          எழுத்து *endptr = வெற்று;

          எனில் (i > 0 && *it == '-')
            it++;

          எனில் (*it == '\0')
            திரும்பு crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          மதிப்பு[i] = strtol (it, &endptr, 10);
          எனில் (errno || endptr == it)
            திரும்பு crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      எனில் (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          எனில் (def == வெற்று
              || def->linux == வெற்று
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            திரும்பு crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          க்கு (i = 0; i < mappings_len; i++)
            எனில் (மதிப்பு[1] >= (int64_t) mappings[i]->container_id
                && மதிப்பு[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              நிறுத்து;

          எனில் (i == mappings_len)
            திரும்பு crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          மதிப்பு[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      எனில் (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", மதிப்பு[0], மதிப்பு[1], மதிப்பு[2]);
      எனில் (UNLIKELY (ret >= (முழு) (allocated - written)))
        திரும்பு crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = வெற்று;

  திரும்பு 0;
}

நிலையான முழு
format_mount_mappings (எழுத்து **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  மாறிலி size_t max_len_mapping = 64;
  cleanup_free எழுத்து *ret = வெற்று;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  க்கு (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      எனில் (UNLIKELY (len >= max_len_mapping))
        திரும்பு crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = வெற்று;
  திரும்பு 0;
}

நிலையான முழு
format_mount_mapping (எழுத்து **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t size, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .size = size,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    வெற்று,
  };

  திரும்பு format_mount_mappings (ret, mappings, 1, written, err);
}

நிலையான bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  எனில் (def->linux == வெற்று)
    திரும்பு mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  எனில் (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    திரும்பு false;

  எனில் (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    திரும்பு false;

  க்கு (s = 0; s < mnt->uid_mappings_len; s++)
    {
      எனில் (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        திரும்பு false;
      எனில் (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        திரும்பு false;
      எனில் (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        திரும்பு false;
    }

  க்கு (s = 0; s < mnt->gid_mappings_len; s++)
    {
      எனில் (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        திரும்பு false;
      எனில் (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        திரும்பு false;
      எனில் (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        திரும்பு false;
    }

  திரும்பு true;
}

நிலையான pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        மாறிலி எழுத்து *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != வெற்று;

  எனில் (! need_new_userns)
    திரும்பு 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, வெற்று);
  எனில் (UNLIKELY (pid < 0))
    திரும்பு crun_make_error (err, errno, "clone");

  எனில் (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      போது (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  எனில் (mnt->uid_mappings_len)
    {
      cleanup_free எழுத்து *uid_map = வெற்று;
      cleanup_free எழுத்து *gid_map = வெற்று;
      size_t written = 0;
      முழு ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      cleanup_close முழு fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      எனில் (UNLIKELY (fd < 0))
        திரும்பு fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      எனில் (UNLIKELY (fd < 0))
        திரும்பு fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      cleanup_free எழுத்து *dup_options = வெற்று;
      எழுத்து *option, *saveptr = வெற்று;

      எனில் (! options)
        திரும்பு crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      க்கு (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (வெற்று, ";", &saveptr))
        {
          cleanup_free எழுத்து *mappings = வெற்று;
          cleanup_close முழு map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          முழு ret;

          எனில் (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              எனில் (UNLIKELY (map_fd < 0))
                திரும்பு map_fd;
            }
          இல்லையெனில் எனில் (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              எனில் (UNLIKELY (map_fd < 0))
                திரும்பு map_fd;
            }
          இல்லையெனில்
            திரும்பு crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  திரும்பு 0;
}

முழு
libcrun_create_keyring (libcrun_container_t *container, மாறிலி எழுத்து *name, மாறிலி எழுத்து *label, libcrun_error_t *err)
{
  cleanup_close முழு labelfd = -1;
  bool label_set = false;
  முழு ret;

  எனில் (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      எனில் (UNLIKELY (labelfd < 0))
        {
          எனில் (crun_error_get_errno (err) != ENOENT)
            திரும்பு labelfd;

          crun_error_release (err);
        }

      எனில் (labelfd >= 0)
        {
          ret = லிக (labelfd, label, strlen (label));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          செல் out;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      செல் out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  எனில் (label_set && லிக (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  திரும்பு ret;
}

நிலையான வெறுமை
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  எனில் (def->process && def->process->user)
    {
      எனில் (def->process->user->uid)
        *uid = def->process->user->uid;
      எனில் (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

நிலையான unsigned long
get_mount_flags (மாறிலி எழுத்து *name, முழு current_flags, முழு *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  மாறிலி கட்டமைப்பு propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  எனில் (found)
    *found = prop ? 1 : 0;

  எனில் (prop == வெற்று)
    திரும்பு 0;

  எனில் (prop->extra_flags & OPTION_RECURSIVE)
    {
      எனில் (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      எனில் (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  எனில் (extra_flags)
    *extra_flags |= prop->extra_flags;

  எனில் (prop->clear)
    திரும்பு current_flags & ~prop->flags;

  திரும்பு current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
நிலையான bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  க்கு (i = 0; i < mount->options_len; i++)
    {
      மாறிலி கட்டமைப்பு propagation_flags_s *prop;

      எனில் (mount->options[i] == வெற்று)
        தொடர்;

      prop = libcrun_str2mount_flags (mount->options[i]);
      எனில் (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        திரும்பு true;
    }

  திரும்பு false;
}

நிலையான unsigned long
get_mount_flags_or_option (மாறிலி எழுத்து *name, முழு current_flags, unsigned long *extra_flags, எழுத்து **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  முழு found;
  __attribute__ ((unused)) cleanup_free எழுத்து *prev = வெற்று;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  எனில் (name == வெற்று)
    திரும்பு 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  எனில் (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      மாறிலி எழுத்து *data_flag = வெற்று;
      எனில் ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      இல்லையெனில் எனில் ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      எனில் (data_flag)
        {
          prev = *option;
          எனில் (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          இல்லையெனில்
            *option = xstrdup (data_flag);
        }
      திரும்பு flags;
    }

  prev = *option;
  எனில் (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  இல்லையெனில்
    *option = xstrdup (name);

  திரும்பு 0;
}

முழு
pivot_வேர் (மாறிலி எழுத்து *new_வேர், மாறிலி எழுத்து *put_old)
{
  திரும்பு syscall (__NR_pivot_root, new_வேர், put_old);
}

நிலையான வெறுமை
free_remount (கட்டமைப்பு remount_s *r)
{
  எனில் (r == வெற்று)
    திரும்பு;
  எனில் (r->targetfd >= 0)
    close (r->targetfd);
  விடுவி (r->data);
  விடுவி (r->target);
  விடுவி (r);
}

நிலையான கட்டமைப்பு remount_s *
make_remount (முழு targetfd, மாறிலி எழுத்து *target, unsigned long flags, மாறிலி எழுத்து *data, கட்டமைப்பு remount_s *next)
{
  கட்டமைப்பு remount_s *ret = xmalloc (அளவு (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : வெற்று;
  ret->next = next;
  ret->targetfd = targetfd;
  திரும்பு ret;
}

நிலையான முழு
do_remount (முழு targetfd, மாறிலி எழுத்து *target, unsigned long flags, மாறிலி எழுத்து *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  முழு ret;
  மாறிலி எழுத்து *real_target = target;

  எனில் (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      எனில் (LIKELY (ret == 0))
        திரும்பு 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  எனில் (flags & (MS_REMOUNT | MS_RDONLY))
    data = வெற்று;

  ret = mount (வெற்று, real_target, வெற்று, flags, data);
  எனில் (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      கட்டமைப்பு statfs sfs;

      ret = statfs (real_target, &sfs);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      எனில் (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      எனில் ((flags | remount_flags) != flags)
        {
          ret = mount (வெற்று, real_target, வெற்று, flags | remount_flags, data);
          எனில் (LIKELY (ret == 0))
            திரும்பு 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          எனில் (current_flags & MS_RDONLY)
            ret = mount (வெற்று, real_target, வெற்று, flags | remount_flags | MS_RDONLY, data);
        }
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "remount `%s`", target);
    }
  திரும்பு 0;
}

நிலையான முழு
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  முழு ret = 0;
  கட்டமைப்பு remount_s *r = get_private_data (container)->remounts;
  போது (r)
    {
      கட்டமைப்பு remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      எனில் (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          எனில் (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              தொடர்;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      எனில் (UNLIKELY (ret < 0))
        செல் cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  போது (r)
    {
      கட்டமைப்பு remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = வெற்று;
  திரும்பு ret;
}

நிலையான முழு
open_mount_target (libcrun_container_t *container, மாறிலி எழுத்து *target_rel, libcrun_error_t *err)
{
  மாறிலி எழுத்து *rootfs = get_private_data (container)->rootfs;
  முழு rootfsfd = get_private_data (container)->rootfsfd;

  எனில் (rootfsfd < 0)
    திரும்பு crun_make_error (err, 0, "invalid rootfs state");

  திரும்பு safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
நிலையான முழு
fsopen_mount (மாறிலி எழுத்து *type, மாறிலி எழுத்து *source_name, மாறிலி எழுத்து *labeltype, மாறிலி எழுத்து *label, மாறிலி எழுத்து *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close முழு fsfd = -1;
  முழு ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  எனில் (UNLIKELY (fsfd < 0))
    திரும்பு fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (வெறுமை) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  எனில் (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (data && data[0])
    {
      cleanup_free எழுத்து *data_copy = xstrdup (data);
      எழுத்து *saveptr = வெற்று;
      எழுத்து *token;

      க்கு (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (வெற்று, ",", &saveptr))
        {
          எழுத்து *eq = strchr (token, '=');
          எனில் (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          இல்லையெனில்
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, வெற்று, 0);

          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, வெற்று, வெற்று, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#இல்லையெனில்
  (வெறுமை) type;
  (வெறுமை) source_name;
  (வெறுமை) data;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

நிலையான முழு
get_procfd (கட்டமைப்பு private_data_s *data, libcrun_error_t *err)
{
  முழு fd;

  எனில் (data->procfd >= 0)
    திரும்பு data->procfd;

  fd = fsopen_mount ("proc", வெற்று, வெற்று, வெற்று, வெற்று);
  எனில் (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  திரும்பு fd;
}

நிலையான முழு
get_old_வேர்_fd (கட்டமைப்பு private_data_s *data)
{
  எனில் (data->old_வேர்_fd >= 0)
    திரும்பு data->old_வேர்_fd;

  data->old_வேர்_fd = open ("/", O_PATH | O_CLOEXEC);
  திரும்பு data->old_வேர்_fd;
}

நிலையான முழு
fs_move_mount_to (முழு fd, முழு dirfd, மாறிலி எழுத்து *name)
{
#ifdef HAVE_NEW_MOUNT_API
  எனில் (name)
    திரும்பு syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  திரும்பு syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#இல்லையெனில்
  (வெறுமை) fd;
  (வெறுமை) dirfd;
  (வெறுமை) name;
  (வெறுமை) syscall_move_mount;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

எண்ணிக்கை
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

நிலையான முழு do_mount (libcrun_container_t *container, மாறிலி எழுத்து *source, முழு targetfd,
                     மாறிலி எழுத்து *target, மாறிலி எழுத்து *fstype, unsigned long mountflags,
                     மாறிலி வெறுமை *data, முழு extra_flags, libcrun_error_t *err);

நிலையான bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, மாறிலி எழுத்து *option)
{
  size_t i;
  க்கு (i = 0; i < mnt->options_len; i++)
    எனில் (mnt->options[i] != வெற்று && strcmp (mnt->options[i], option) == 0)
      திரும்பு true;
  திரும்பு false;
}

நிலையான runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, மாறிலி எழுத்து *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  க்கு (i = 0; i < def->mounts_len; i++)
    {
      எனில் (strcmp (def->mounts[i]->destination, destination) == 0)
        திரும்பு def->mounts[i];
    }
  திரும்பு வெற்று;
}

நிலையான வெறுமை
warn_tmpfs_fallback_once (கட்டமைப்பு private_data_s *private_data, மாறிலி எழுத்து *reason)
{
  எனில் (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
நிலையான முழு
get_shared_empty_dir_cached (libcrun_container_t *container, எழுத்து **proc_fd_path, libcrun_error_t *err)
{
  கட்டமைப்பு private_data_s *private_data = get_private_data (container);
  cleanup_close முழு fd = -1;
  cleanup_free எழுத்து *empty_dir_path = வெற்று;
  proc_fd_path_t fd_path;
  முழு ret;

  /* Fast path: return cached proc fd path if already set up */
  எனில் (private_data->maskdir_proc_path != வெற்று)
    {
      எனில் (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      திரும்பு 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_வேர், err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  எனில் (fd < 0)
    திரும்பு fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  எனில் (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  திரும்பு 0;
}

நிலையான bool
is_dev_null (மாறிலி கட்டமைப்பு stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  திரும்பு S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
நிலையான முழு
ensure_dev_null (முழு fd, libcrun_error_t *err)
{
  கட்டமைப்பு stat st;

  எனில் (UNLIKELY (fstat (fd, &st) < 0))
    திரும்பு crun_make_error (err, errno, "stat `/dev/null`");

  எனில் (UNLIKELY (! is_dev_null (&st)))
    திரும்பு crun_make_error (err, 0, "`/dev/null` is not the null device");

  திரும்பு 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
நிலையான bool
is_empty_ro_tmpfs (முழு fd)
{
  கட்டமைப்பு statfs sfs;

  எனில் (fstatfs (fd, &sfs) < 0)
    திரும்பு false;

  திரும்பு sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

நிலையான முழு
mount_masked_dir (libcrun_container_t *container, முழு pathfd, மாறிலி எழுத்து *rel_path, libcrun_error_t *err)
{
  கட்டமைப்பு private_data_s *private_data = get_private_data (container);
  cleanup_close முழு newfd = -1;
  libcrun_error_t tmp_err = வெற்று;
  முழு ret;

  எனில் (private_data->maskdir_fd < 0)
    {
      எனில் (private_data->maskdir_bind_failed)
        செல் fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, வெற்று, &tmp_err);
      எனில் (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->செய்தி);
          crun_error_release (&tmp_err);
          செல் fallback_to_tmpfs;
        }
    }

  எனில் (private_data->maskdir_fd >= 0)
    {
      முழு rootfsfd = private_data->rootfsfd;
      கட்டமைப்பு stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, வெற்று);
      எனில் (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          எனில் (UNLIKELY (newfd < 0))
            திரும்பு newfd;

          ret = fstat (newfd, &after);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          எனில் (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            திரும்பு crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          திரும்பு 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_path);

      TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
      private_data->maskdir_fd = -1;
      private_data->maskdir_move_failed = true;
    }

  private_data->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_data, tmp_err ? tmp_err->செய்தி : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  எனில் (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      எனில் (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      இல்லையெனில்
        {
          எனில் (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  திரும்பு 0;
}

நிலையான முழு
do_masked_or_readonly_path (libcrun_container_t *container, மாறிலி எழுத்து *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  மாறிலி எழுத்து *rootfs = get_private_data (container)->rootfs;
  cleanup_close முழு pathfd = -1;
  கட்டமைப்பு statfs sfs;
  முழு ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  எனில் (UNLIKELY (rel_path == வெற்று))
    திரும்பு crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  எனில் (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  எனில் (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      எனில் (errno != ENOENT && errno != EACCES)
        திரும்பு pathfd;

      எனில் (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      திரும்பு 0;
    }

  எனில் (readonly)
    {
      முழு procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close முழு mountfd = -1;

      எனில் (UNLIKELY (procfd < 0))
        திரும்பு procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      எனில் (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          எனில் (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, வெற்று);
          இல்லையெனில்
            ret = -1;

          எனில் (ret < 0)
            crun_error_release (err);
        }

      எனில் (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          எனில் (keep_flags)
            {
              cleanup_close முழு sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              எனில் (UNLIKELY (sfd < 0))
                திரும்பு crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          எனில் (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;

              ret = fs_move_mount_to (mountfd, pathfd, வெற்று);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          இல்லையெனில்
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, வெற்று, mount_flags, வெற்று,
                              LABEL_NONE, err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;
            }
        }
    }
  இல்லையெனில்
    {
      ret = get_file_type_fd (pathfd, &mode);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      எனில் ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      இல்லையெனில்
        {
          cleanup_close முழு nullfd = -1;
          cleanup_close முழு mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          எனில் (UNLIKELY (nullfd < 0))
            திரும்பு crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          எனில் (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, வெற்று);

          எனில் (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, வெற்று, MS_BIND | MS_RDONLY, வெற்று,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  மாறிலி எழுத்து *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  எனில் (context_type)
    {
      எனில் (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          திரும்பு வெற்று;
        }
      திரும்பு context_type;
    }

  திரும்பு "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
நிலையான முழு
diagnose_mount_failure (முழு ret, libcrun_error_t *err, libcrun_container_t *container,
                        மாறிலி எழுத்து *fstype)
{
  எனில் (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      எனில் (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          திரும்பு ret;
        }
    }
  திரும்பு ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
நிலையான முழு
sysfs_userns_fallback (libcrun_container_t *container, மாறிலி எழுத்து *target,
                       மாறிலி எழுத்து *real_target, முழு targetfd,
                       unsigned long mountflags, மாறிலி எழுத்து *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close முழு mountfd = -1;
  cleanup_close முழு sysfd = -1;
  முழு sys_old_வேர்_fd = -1;
  முழு ret;

  எனில் (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    திரும்பு 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (ret == 0)
    திரும்பு 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_வேர்_fd = get_old_வேர்_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  எனில் (sys_old_வேர்_fd >= 0)
    mountfd = get_bind_mount (sys_old_வேர்_fd, "sys", true, true, false, MS_PRIVATE, err);
  எனில் (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  எனில் (cgroup_mount == வெற்று)
    {
      எனில் (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, வெற்று);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      இல்லையெனில்
        {
          crun_error_release (err);
          எனில் (sys_old_வேர்_fd >= 0)
            {
              cleanup_free எழுத்து *sys_path = வெற்று;
              proc_fd_path_t sys_வேர்_path;

              get_proc_self_fd_path (sys_வேர்_path, sys_old_வேர்_fd);
              xasprintf (&sys_path, "%s/sys", sys_வேர்_path);
              ret = mount (sys_path, real_target, வெற்று, MS_BIND | MS_REC, வெற்று);
            }
          இல்லையெனில்
            ret = mount ("/sys", real_target, வெற்று, MS_BIND | MS_REC, வெற்று);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      எனில் (UNLIKELY (sysfd < 0))
        திரும்பு sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        வெற்று, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      திரும்பு do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  எனில் (UNLIKELY (mountfd < 0))
    திரும்பு mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  எனில் (UNLIKELY (sysfd < 0))
    திரும்பு sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
நிலையான வெறுமை
apply_selinux_xattr (libcrun_container_t *container, முழு fd, முழு extra_flags, மாறிலி எழுத்து *label)
{
  libcrun_error_t tmp_err = வெற்று;
  முழு procfd;

  எனில் ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    திரும்பு;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  எனில் (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close முழு xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      எனில் (xfd >= 0)
        (வெறுமை) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
நிலையான முழு
do_mount_replace_rootfs (libcrun_container_t *container, முழு *fd, libcrun_error_t *err)
{
  முழு procfd = get_procfd (get_private_data (container), err);
  முழு tmp;
  முழு ret;

  எனில் (UNLIKELY (procfd < 0))
    திரும்பு procfd;

  {
    cleanup_close முழு nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    எனில் (UNLIKELY (nsfd < 0))
      திரும்பு crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    எனில் (UNLIKELY (ret < 0))
      திரும்பு crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (*fd < 0))
    திரும்பு crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  எனில் (UNLIKELY (tmp < 0))
    திரும்பு crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  திரும்பு 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
நிலையான முழு
apply_propagation_flags (மாறிலி எழுத்து *target, முழு targetfd, மாறிலி எழுத்து *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  முழு ret;

  எனில் (targetfd >= 0)
    {
      libcrun_error_t tmp_err = வெற்று;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      எனில் (LIKELY (ret == 0))
        propagation_done = true;
      இல்லையெனில்
        crun_error_release (&tmp_err);
    }

  எனில் (! propagation_done)
    {
      ret = mount (வெற்று, real_target, வெற்று, mountflags & ALL_PROPAGATIONS, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  திரும்பு 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
நிலையான முழு
schedule_or_do_remount (libcrun_container_t *container, முழு *fd, முழு targetfd,
                        மாறிலி எழுத்து *target, மாறிலி எழுத்து *real_target,
                        bool single_instance, unsigned long mountflags,
                        மாறிலி வெறுமை *data, முழு extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  கட்டமைப்பு remount_s *r;

  எனில் ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    திரும்பு do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  எனில் (*fd < 0)
    {
      *fd = dup (targetfd);
      எனில் (UNLIKELY (*fd < 0))
        திரும்பு crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  திரும்பு 0;
}

நிலையான முழு
do_mount (libcrun_container_t *container, மாறிலி எழுத்து *source, முழு targetfd,
          மாறிலி எழுத்து *target, மாறிலி எழுத்து *fstype, unsigned long mountflags, மாறிலி வெறுமை *data,
          முழு extra_flags, libcrun_error_t *err)
{
  cleanup_free எழுத்து *data_with_label = வெற்று;
  மாறிலி எழுத்து *data_without_label = data;
  proc_fd_path_t target_buffer;
  மாறிலி எழுத்து *context_type = வெற்று;
  cleanup_close முழு ms_move_fd = -1;
  மாறிலி எழுத்து *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close முழு fd = -1;
  மாறிலி எழுத்து *label = வெற்று;
  முழு ret = 0;

  எனில் (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  இல்லையெனில்
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  எனில் (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  எனில் ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      எனில் (UNLIKELY (context_type == வெற்று))
        திரும்பு -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      எனில் (ret < 0)
        திரும்பு ret;
      data = data_with_label;
    }

  எனில் (mountflags & MS_MOVE)
    {
      எனில் ((mountflags & MS_BIND) || fstype)
        திரும்பு crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, வெற்று, MS_MOVE, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      எனில் (UNLIKELY (ms_move_fd < 0))
        திரும்பு ms_move_fd;
      targetfd = ms_move_fd;
    }

  எனில் ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      எனில் (targetfd >= 0)
        {
          எனில் (mountflags & MS_BIND)
            {
              cleanup_close முழு bindfd = -1;
              libcrun_error_t tmp_err = வெற்று;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              எனில் (bindfd < 0)
                {
                  முழு old_வேர்_fd = get_old_வேர்_fd (get_private_data (container));

                  எனில் (old_வேர்_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_வேர்_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              எனில் (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, வெற்று);
              எனில் (ret < 0)
                crun_error_release (&tmp_err);
            }
          இல்லையெனில்
            {
              cleanup_close முழு newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              எனில் (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, வெற்று);
            }
        }

      எனில் (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          எனில் (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              முழு old_வேர்_fd = get_old_வேர்_fd (get_private_data (container));

              எனில் (old_வேர்_fd >= 0)
                {
                  cleanup_free எழுத்து *proc_source_buf = வெற்று;
                  proc_fd_path_t old_வேர்_path;

                  get_proc_self_fd_path (old_வேர்_path, old_வேர்_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_வேர்_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      எனில் (UNLIKELY (ret < 0))
        {
          முழு saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          எனில் (ret == 0)
            திரும்பு 0;
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          திரும்பு diagnose_mount_failure (ret, err, container, fstype);
        }

      எனில் (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          எனில் (UNLIKELY (fd < 0))
            திரும்பு fd;

          /* We are replacing the rootfs, reopen it.  */
          எனில் (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;
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

  எனில் (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  எனில் (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  எனில் (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு ret;
}

நிலையான வெறுமை
try_umount (முழு targetfd, மாறிலி எழுத்து *target)
{
  proc_fd_path_t target_buffer;
  மாறிலி எழுத்து *real_target = target;

  எனில் (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

நிலையான bool
container_has_cgroupns (libcrun_container_t *container)
{
  திரும்பு get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

நிலையான முழு
do_mount_cgroup_v2 (libcrun_container_t *container, முழு targetfd, மாறிலி எழுத்து *target,
                    unsigned long mountflags, மாறிலி எழுத்து *data, libcrun_error_t *err)
{
  முழு ret;
  முழு cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  எனில் (UNLIKELY (ret < 0))
    {
      முழு saved_errno = crun_error_get_errno (err);
      எனில் (saved_errno == EPERM || saved_errno == EBUSY)
        {
          மாறிலி எழுத்து *unified_cgroup_path;
          மாறிலி எழுத்து *src_cgroup;

          crun_error_release (err);

          எனில் (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              எனில் (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  எனில் (LIKELY (ret == 0))
                    திரும்பு ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_வேர்;
          திரும்பு do_mount (container, src_cgroup, targetfd, target, வெற்று,
                           MS_BIND | mountflags, வெற்று, LABEL_NONE, err);
        }

      திரும்பு ret;
    }

  திரும்பு 0;
}

நிலையான முழு
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, மாறிலி எழுத்து *source, முழு targetfd, மாறிலி எழுத்து *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  முழு ret;
  cleanup_close முழு fd = -1;
  மாறிலி எழுத்து *subsystem = "systemd";
  cleanup_free எழுத்து *subsystem_path = வெற்று;
  cleanup_close முழு tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  எனில் (UNLIKELY (tmpfsdirfd < 0))
    திரும்பு tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "openat `%s`", subsystem_path);

  திரும்பு do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

நிலையான முழு
do_mount_cgroup_v1 (libcrun_container_t *container, மாறிலி எழுத்து *source, முழு targetfd, மாறிலி எழுத்து *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  முழு ret;
  cleanup_free எழுத்து *content = வெற்று;
  எழுத்து *from;
  cleanup_close முழு tmpfsdirfd = -1;
  எழுத்து *saveptr = வெற்று;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  எனில் (UNLIKELY (tmpfsdirfd < 0))
    திரும்பு tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    முழு procfd = get_procfd (get_private_data (container), err);
    எனில் (UNLIKELY (procfd < 0))
      திரும்பு procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, வெற்று, err);
  }
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (UNLIKELY (content == வெற்று || content[0] == '\0'))
    திரும்பு crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  க்கு (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (வெற்று, "\n", &saveptr))
    {
      cleanup_free எழுத்து *source_path = வெற்று;
      cleanup_free எழுத்து *source_subsystem = வெற்று;
      cleanup_free எழுத்து *subsystem_path = வெற்று;
      எழுத்து *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close முழு subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      எனில் (subsystem[0] == '\0')
        தொடர்;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      எனில் (it)
        subsystem = it + 5;

      எனில் (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      எனில் (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_வேர், subsystem, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      /* if there is already a mount specified, do not add a default one.  */
      எனில் (find_mount_for (container, source_subsystem) != வெற்று)
        தொடர்;

      ret = append_paths (&source_path, err, source_subsystem, subpath, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      எனில் (UNLIKELY (subsystemfd < 0))
        திரும்பு crun_make_error (err, errno, "open `%s`", subsystem_path);

      எனில் (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          எனில் (UNLIKELY (ret < 0))
            {
              எனில் (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  தொடர்;
                }
              திரும்பு ret;
            }
        }
      இல்லையெனில்
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, வெற்று, MS_BIND | mountflags, வெற்று,
                          LABEL_NONE, err);
          எனில் (UNLIKELY (ret < 0))
            {
              எனில் (crun_error_get_errno (err) != ENOENT)
                திரும்பு ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, வெற்று, MS_BIND | mountflags,
                              வெற்று, LABEL_NONE, err);
              எனில் (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  எனில் (crun_error_get_errno (err) != ENOENT)
                    திரும்பு ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

நிலையான முழு
do_mount_cgroup (libcrun_container_t *container, மாறிலி எழுத்து *source, முழு targetfd, மாறிலி எழுத்து *target,
                 unsigned long mountflags, மாறிலி எழுத்து *data, libcrun_error_t *err)
{
  முழு cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  தேர்வு (cgroup_mode)
    {
    நிலை CGROUP_MODE_UNIFIED:
      திரும்பு do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    நிலை CGROUP_MODE_LEGACY:
    நிலை CGROUP_MODE_HYBRID:
      திரும்பு do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  திரும்பு crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

கட்டமைப்பு device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
நிலையான வெறுமை
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  கட்டமைப்பு private_data_s *private_data = get_private_data (container);
  size_t i;

  எனில் (private_data->needed_devs_fds)
    {
      கட்டமைப்பு libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      க்கு (i = 0; needed_devs[i].path; i++)
        {
          எனில் (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            தொடர்;

          முழு fd = syscall_open_மரம் (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          எனில் (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          இல்லையெனில் எனில் (fd >= 0)
            close (fd);
        }
    }

  எனில் (private_data->notify_socket_மரம்_fd < 0
      && private_data->host_notify_socket_path)
    {
      முழு fd = syscall_open_மரம் (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      எனில் (fd >= 0)
        private_data->notify_socket_மரம்_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
நிலையான மாறிலி எழுத்து *
relative_path_under_dev (மாறிலி எழுத்து *path)
{
  எனில் (path[0] != '/')
    திரும்பு path;
  எனில் (strncmp (path, "/dev/", 5) == 0)
    {
      எனில் (strchr (path + 5, '/'))
        திரும்பு வெற்று;
      திரும்பு path + 5;
    }
  திரும்பு வெற்று;
}

நிலையான முழு
mknod_and_set_attrs (libcrun_container_t *container, முழு dirfd, மாறிலி எழுத்து *name, கட்டமைப்பு device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close முழு fd = -1;
  proc_fd_path_t fd_path;
  மாறிலி எழுத்து *what = "new";
  bool exists = false;
  கட்டமைப்பு stat st;
  முழு procfd;
  முழு ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno != EEXIST)
        திரும்பு crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  எனில் (UNLIKELY ((st.st_mode & S_IFMT) != type))
    திரும்பு crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  எனில் (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    திரும்பு crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  எனில் (exists)
    திரும்பு 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  எனில் (UNLIKELY (procfd < 0))
    திரும்பு procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fchown `%s`", device->path);

  திரும்பு 1;
}

முழு
libcrun_create_dev (libcrun_container_t *container, முழு devfd, முழு srcfd,
                    கட்டமைப்பு device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  முழு ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  மாறிலி எழுத்து *fullname = device->path;
  மாறிலி எழுத்து *rootfs = get_private_data (container)->rootfs;
  எனில் (is_empty_string (fullname))
    திரும்பு crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free எழுத்து *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  எனில் (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  மாறிலி எழுத்து *rel_dev = relative_path_under_dev (normalized_path);

  எனில் (binds)
    {
      cleanup_close முழு fd = -1;

      எனில் (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          எனில் (UNLIKELY (fd < 0))
            {
              எனில் (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              எனில் (UNLIKELY (fd < 0))
                திரும்பு crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      இல்லையெனில்
        {
          மாறிலி எழுத்து *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          எனில் (UNLIKELY (fd < 0))
            திரும்பு fd;
        }

      எனில் (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          எனில் (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              எனில் (LIKELY (ret >= 0))
                திரும்பு 0;
            }
          இல்லையெனில்
            crun_error_release (err);
        }

      {
        cleanup_close முழு mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        எனில் (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            எனில் (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, வெற்று);
                எனில் (LIKELY (ret == 0))
                  திரும்பு 0;
              }
            இல்லையெனில்
              crun_error_release (err);
          }
        இல்லையெனில்
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, வெற்று, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, வெற்று, LABEL_MOUNT, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      எனில் (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          எனில் (ret <= 0)
            திரும்பு ret;
        }
      இல்லையெனில்
        {
          cleanup_close முழு dirfd = -1;
          cleanup_free எழுத்து *dirname = வெற்று;
          எழுத்து *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          எனில் (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          எனில் (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              எனில் (UNLIKELY (dirfd < 0))
                திரும்பு crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          இல்லையெனில்
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              எனில் (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              எனில் (UNLIKELY (dirfd < 0))
                திரும்பு dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          எனில் (ret <= 0)
            திரும்பு ret;
        }
    }
  திரும்பு 0;
}

கட்டமைப்பு symlink_s
{
  மாறிலி எழுத்து *path;
  மாறிலி எழுத்து *target;
  bool force;
};

நிலையான கட்டமைப்பு symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { வெற்று, வெற்று, false } };

நிலையான முழு
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  முழு ret;
  size_t i;
  கட்டமைப்பு device_s *it;
  cleanup_close முழு devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  மாறிலி எழுத்து *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map கட்டமைப்பு libcrun_fd_map *dev_fds = வெற்று;
  cleanup_close_map கட்டமைப்பு libcrun_fd_map *needed_fds = வெற்று;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = வெற்று;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = வெற்று;

  எனில் (! def || ! def->linux)
    திரும்பு 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  எனில் (UNLIKELY (devfd < 0))
    திரும்பு devfd;

  க்கு (i = 0; i < def->linux->devices_len; i++)
    {
      கட்டமைப்பு device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      எனில் (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  க்கு (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close முழு srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      எனில் (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  க்கு (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      எனில் (UNLIKELY (ret < 0))
        {
          முழு saved_errno = errno;

          எனில் (errno == EEXIST && ! symlinks[i].force)
            தொடர்;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          எனில் (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              எனில் (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              எனில் (ret < 0 && errno == EBUSY)
                {
                  cleanup_close முழு tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  எனில் (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      எனில் (umount2 (procpath, MNT_DETACH) == 0)
                        செல் retry_unlink;
                    }
                }
              எனில் (ret == 0)
                செல் retry_symlink;
            }
          திரும்பு crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  எனில் (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
நிலையான மாறிலி எழுத்து *
next_path_component (மாறிலி எழுத்து *p, size_t *len)
{
  க்கு (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      எனில் (*len != 1 || p[0] != '.')
        திரும்பு p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
நிலையான bool
same_path (மாறிலி எழுத்து *a, மாறிலி எழுத்து *b)
{
  size_t la, lb;

  க்கு (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      எனில் (la != lb || memcmp (a, b, la) != 0)
        திரும்பு false;
      எனில் (la == 0)
        திரும்பு true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
நிலையான bool
is_duplicate_path (எழுத்து **paths, size_t i)
{
  size_t j;

  எனில் (paths[i] == வெற்று)
    திரும்பு false;

  க்கு (j = 0; j < i; j++)
    எனில் (paths[j] && same_path (paths[j], paths[i]))
      திரும்பு true;

  திரும்பு false;
}

நிலையான முழு
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  முழு ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  எனில் (! def || ! def->linux)
    திரும்பு 0;

  க்கு (i = 0; i < def->linux->masked_paths_len; i++)
    {
      எனில் (is_duplicate_path (def->linux->masked_paths, i))
        தொடர்;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  க்கு (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு 0;
}

நிலையான முழு
do_pivot (libcrun_container_t *container, மாறிலி எழுத்து *rootfs, libcrun_error_t *err)
{
  cleanup_close முழு oldrootfd = -1;
  cleanup_close முழு newrootfd = -1;
  முழு ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (oldrootfd < 0))
    திரும்பு crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (newrootfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_வேர் (".", ".");
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, வெற்று, -1, ".", வெற்று, MS_REC | MS_PRIVATE, வெற்று, LABEL_MOUNT, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = umount2 (".", MNT_DETACH);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "umount2 oldroot");

  செய்
    {
      ret = umount2 (".", MNT_DETACH);
      எனில் (ret < 0 && errno == EINVAL)
        நிறுத்து;
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "umount2 oldroot");
  } போது (ret == 0);

  ret = chdir ("/");
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "chdir to newroot");

  திரும்பு 0;
}

நிலையான முழு
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, எழுத்து **data, libcrun_error_t *err)
{
  மாறிலி எழுத்து *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close முழு fd = -1;
  கட்டமைப்பு stat st;
  முழு ret;

  எனில் (*data != வெற்று && strstr (*data, "mode="))
    திரும்பு 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  எனில் (fd < 0)
    {
      எனில் (crun_error_get_errno (err) != ENOENT)
        திரும்பு fd;

      crun_error_release (err);
      திரும்பு 0;
    }
  ret = fstat (fd, &st);
  எனில் (ret < 0)
    திரும்பு crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  திரும்பு 0;
}

நிலையான முழு
get_default_flags (libcrun_container_t *container, மாறிலி எழுத்து *destination, எழுத்து **data)
{
  எனில் (strcmp (destination, "/proc") == 0)
    திரும்பு 0;
  எனில் (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      திரும்பு MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  எனில் (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      திரும்பு MS_NOEXEC | MS_STRICTATIME;
    }
  எனில் (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      திரும்பு MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  எனில் (strcmp (destination, "/dev/mqueue") == 0)
    திரும்பு MS_NOEXEC | MS_NOSUID | MS_NODEV;
  எனில் (strcmp (destination, "/dev/pts") == 0)
    {
      எனில் (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      இல்லையெனில்
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      திரும்பு MS_NOEXEC | MS_NOSUID;
    }
  எனில் (strcmp (destination, "/sys") == 0)
    திரும்பு MS_NOEXEC | MS_NOSUID | MS_NODEV;

  திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  திரும்பு find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

நிலையான முழு
check_valid_no_follow_file_system (முழு fd, மாறிலி எழுத்து *destination, libcrun_error_t *err)
{
  கட்டமைப்பு statfs sfs;

  முழு ret = fstatfs (fd, &sfs);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "statfs `%s`", destination);

  தேர்வு (sfs.f_type)
    {
    நிலை PROC_SUPER_MAGIC:
    நிலை SYSFS_MAGIC:
    நிலை DEVPTS_SUPER_MAGIC:
      திரும்பு crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  திரும்பு 0;
}

நிலையான முழு
safe_create_symlink (முழு rootfsfd, மாறிலி எழுத்து *rootfs, மாறிலி எழுத்து *target, மாறிலி எழுத்து *destination, libcrun_error_t *err)
{
  cleanup_close முழு parent_dir_fd = -1;
  cleanup_free எழுத்து *buffer = வெற்று;
  எழுத்து *part;
  முழு ret;

  எனில் (is_empty_string (destination))
    திரும்பு crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  எனில் (UNLIKELY (parent_dir_fd < 0))
    திரும்பு parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  எனில் (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      எனில் (errno == EEXIST)
        {
          cleanup_free எழுத்து *link = வெற்று;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          எனில் (UNLIKELY (len < 0))
            திரும்பு len;

          எனில் ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            திரும்பு 0;

          திரும்பு crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      திரும்பு crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  திரும்பு 0;
}

நிலையான முழு
handle_copy_symlink (libcrun_container_t *container, மாறிலி எழுத்து *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     மாறிலி எழுத்து *link_target, libcrun_error_t *err)
{
  cleanup_free எழுத்து *target = வெற்று;

  எனில் (link_target == வெற்று)
    {
      முழு old_வேர்_fd = get_old_வேர்_fd (get_private_data (container));
      மாறிலி எழுத்து *source = mount->source;
      ssize_t len;

      எனில் (old_வேர்_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_வேர்_fd, source + 1, &target, 0, err);
      இல்லையெனில்
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      எனில் (UNLIKELY (len < 0))
        திரும்பு len;
      link_target = target;
    }

  திரும்பு safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

நிலையான முழு
prepare_sysfs_or_proc_mount (libcrun_container_t *container, மாறிலி எழுத்து *target,
                             முழு *targetfd, libcrun_error_t *err)
{
  முழு ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno == ENOENT)
        {
          எனில் (strchr (target, '/'))
            திரும்பு crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      இல்லையெனில் எனில் (errno == ENOTDIR)
        திரும்பு crun_make_error (err, errno, "the target `/%s` is invalid", target);

      எனில் (ret < 0)
        திரும்பு crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  திரும்பு 0;
}

நிலையான முழு
handle_tmpcopyup (libcrun_container_t *container, மாறிலி எழுத்து *rootfs, மாறிலி எழுத்து *target,
                  முழு copy_from_fd, libcrun_error_t *err)
{
  முழு destfd, ret;
  cleanup_close முழு tmpfd = copy_from_fd;

  target = consume_slashes (target);

  எனில் (is_empty_string (target))
    திரும்பு crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  எனில் (UNLIKELY (destfd < 0))
    திரும்பு crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  திரும்பு ret;
}

நிலையான முழு
get_mount_label_how (மாறிலி எழுத்து *type, bool is_sysfs_or_proc)
{
  எனில் (is_sysfs_or_proc)
    திரும்பு LABEL_NONE;

  எனில் (strcmp (type, "mqueue") == 0)
    திரும்பு LABEL_XATTR;

  திரும்பு LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
நிலையான unsigned long
get_bind_mount_locked_flags (முழு source_mountfd, மாறிலி எழுத்து *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  கட்டமைப்பு statfs sfs;
  size_t i;
  முழு ret;

  எனில் (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  இல்லையெனில்
    ret = statfs (source, &sfs);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு 0;

  க்கு (i = 0; i < mount->options_len; i++)
    {
      மாறிலி கட்டமைப்பு propagation_flags_s *prop;

      எனில் (mount->options[i] == வெற்று)
        தொடர்;

      prop = libcrun_str2mount_flags (mount->options[i]);
      எனில் (prop == வெற்று || (prop->extra_flags & OPTION_RECURSIVE))
        தொடர்;

      எனில் (prop->clear)
        cleared |= prop->flags;
      இல்லையெனில்
        cleared &= ~prop->flags;
    }

  திரும்பு sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
நிலையான மாறிலி எழுத்து *
get_path_in_rootfs (மாறிலி எழுத்து *rootfs, மாறிலி எழுத்து *path)
{
  size_t len;

  எனில் (rootfs == வெற்று || path == வெற்று)
    திரும்பு வெற்று;

  len = strlen (rootfs);
  போது (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  எனில் (len == 0)
    திரும்பு வெற்று;

  எனில் (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    திரும்பு வெற்று;

  திரும்பு consume_slashes (path + len);
}

நிலையான முழு
process_single_mount (libcrun_container_t *container, மாறிலி எழுத்து *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      கட்டமைப்பு libcrun_fd_map *mount_fds, size_t mount_index,
                      மாறிலி எழுத்து *systemd_cgroup_v1, libcrun_error_t *err)
{
  மாறிலி எழுத்து *target = consume_slashes (mount->destination);
  cleanup_close முழு source_mountfd = -1;
  cleanup_free எழுத்து *data = வெற்று;
  எழுத்து *type;
  எழுத்து *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close முழு copy_from_fd = -1;
  cleanup_close முழு targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  முழு no_remount = 0;
  முழு ret;

  எனில் (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  எனில் (mount->options == வெற்று)
    flags = get_default_flags (container, mount->destination, &data);
  இல்லையெனில்
    {
      size_t j;

      க்கு (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  எனில் (type == வெற்று && (flags & MS_BIND) == 0)
    திரும்பு crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  எனில் ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      மாறிலி எழுத்து *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      எனில் (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          எனில் (UNLIKELY (source_mountfd < 0))
            திரும்பு crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  எனில் ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  எனில் (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      எனில் (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      எனில் (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  எனில் (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      மாறிலி எழுத்து *path = mount->source;

      எனில் ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        திரும்பு crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      எனில் (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, வெற்று);
        }
      இல்லையெனில் எனில் (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      இல்லையெனில் எனில் (path[0] == '/')
        {
          முழு old_வேர்_fd = get_old_வேர்_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          எனில் (old_வேர்_fd >= 0)
            ret = get_file_type_at (old_வேர்_fd, &src_mode, nofollow, (path[1] == '\0') ? வெற்று : path + 1);
          இல்லையெனில்
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      இல்லையெனில்
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "cannot stat `%s`", path);

      எனில் (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          முழு bind_dirfd = AT_FDCWD;
          மாறிலி எழுத்து *bind_source = mount->source;
          முழு old_வேர்_fd = get_old_வேர்_fd (get_private_data (container));

          எனில் (old_வேர்_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_வேர்_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          source_mountfd = ret;
        }
    }

  எனில் (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      மாறிலி எழுத்து *cached_target = வெற்று;

      எனில் (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      mounted = true;
    }
  இல்லையெனில் எனில் (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      bool is_dir = S_ISDIR (src_mode);

      எனில் (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
      இல்லையெனில்
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          targetfd = ret;
        }
    }

  எனில் (extra_flags & OPTION_TMPCOPYUP)
    {
      எனில் (strcmp (type, "tmpfs") != 0)
        திரும்பு crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      எனில் (UNLIKELY (copy_from_fd < 0))
        {
          எனில் (errno != ENOTDIR)
            திரும்பு crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  எனில் (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, வெற்று);
      எனில் (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          எனில் (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, வெற்று, source_mountfd, target, வெற்று, remaining_flags, data, LABEL_NONE | no_remount, err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;
            }

          mounted = true;

          எனில் (is_empty_string (target))
            {
              முழு new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              எனில் (UNLIKELY (new_rootfsfd < 0))
                திரும்பு crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      இல்லையெனில் எனில் (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          திரும்பு crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      இல்லையெனில் எனில் (get_private_data (container)->host_வேர்_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          திரும்பு crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      இல்லையெனில் எனில் (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  எனில் (! mounted)
    {
      எனில் (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
      இல்லையெனில் எனில் (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
      இல்லையெனில்
        {
          முழு extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }

  எனில் (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (rec_clear || rec_set)
    {
      மாறிலி bool is_dir = S_ISDIR (src_mode);
      cleanup_close முழு dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      எனில் (UNLIKELY (dfd < 0))
        திரும்பு crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

நிலையான முழு
do_mounts (libcrun_container_t *container, மாறிலி எழுத்து *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  மாறிலி எழுத்து *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map கட்டமைப்பு libcrun_fd_map *mount_fds = வெற்று;
  size_t i;
  முழு ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = வெற்று;

  க்கு (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
முழு
libcrun_container_do_bind_mount (libcrun_container_t *container, எழுத்து *mount_source, எழுத்து *mount_destination, எழுத்து **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  மாறிலி எழுத்து *target = consume_slashes (mount_destination);
  cleanup_free எழுத்து *data = வெற்று;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close முழு targetfd = -1;
  முழு is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  மாறிலி எழுத்து *rootfs = get_private_data (container)->rootfs;
  முழு ret;

  எனில் ((get_private_data (container)->rootfsfd < 0) || (rootfs == வெற்று))
    திரும்பு crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  எனில் (mount_options == வெற்று)
    flags = get_default_flags (container, mount_destination, &data);
  இல்லையெனில்
    {
      size_t j;

      க்கு (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  எனில் (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  எனில் (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      எனில் (UNLIKELY (is_dir < 0))
        திரும்பு is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
முழு
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, முழு *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close முழு notify_fd = -1;
  cleanup_free எழுத்து *host_notify_socket_path = வெற்று;
  cleanup_free எழுத்து *state_dir = வெற்று;
  எழுத்து *host_path = வெற்று;
  முழு ret;

  எனில் (container && get_private_data (container)->host_notify_socket_path)
    {
      மாறிலி எழுத்து *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  எனில் (host_path == வெற்று)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_வேர், context->id, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  எனில் (UNLIKELY (notify_fd < 0))
    திரும்பு notify_fd;

  எனில் (UNLIKELY (chmod (host_path, 0777) < 0))
    திரும்பு crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  எனில் (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (வெறுமை) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  திரும்பு 1;
#இல்லையெனில்
  (வெறுமை) context;
  (வெறுமை) container;
  (வெறுமை) err;
  *notify_socket_out = -1;
  திரும்பு 0;
#endif
}

#ifdef HAVE_SYSTEMD
நிலையான முழு
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  முழு ret;
  மாறிலி எழுத்து *notify_socket = container->context->notify_socket;
  cleanup_free எழுத்து *host_notify_socket_path = வெற்று;
  cleanup_free எழுத்து *container_notify_socket_path = வெற்று;
  cleanup_free எழுத்து *state_dir = வெற்று;
  uid_t container_வேர்_uid = -1;
  gid_t container_வேர்_gid = -1;
  முழு notify_socket_மரம்_fd;

  எனில் (notify_socket == வெற்று)
    திரும்பு 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_வேர் : வெற்று),
                                     (container->context ? container->context->id : வெற்று), err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = mkdir (host_notify_socket_path, 0700);
  எனில் (ret < 0)
    திரும்பு crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  எனில் (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_வேர்_in_the_userns (container->container_def, 0, 0, &container_வேர்_uid, &container_வேர்_gid);
      எனில் (container_வேர்_uid != ((uid_t) -1) && container_வேர்_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_வேர்_uid, container_வேர்_gid);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_வேர்_uid, container_வேர்_gid);
        }
    }

  notify_socket_மரம்_fd = syscall_open_மரம் (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  எனில் (notify_socket_மரம்_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_மரம்_fd = notify_socket_மரம்_fd;
  இல்லையெனில் எனில் (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  இல்லையெனில் எனில் (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  இல்லையெனில்
    /* some other error */
    திரும்பு crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = வெற்று;
  திரும்பு 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
நிலையான முழு
do_finalize_notify_socket (libcrun_container_t *container, மாறிலி எழுத்து *rootfs, libcrun_error_t *err)
{
  முழு ret;
  cleanup_free எழுத்து *host_notify_socket_path = வெற்று;
  cleanup_free எழுத்து *container_notify_socket_path = வெற்று;
  cleanup_free எழுத்து *container_notify_socket_path_dir_alloc = வெற்று;
  மாறிலி எழுத்து *container_notify_socket_path_dir = வெற்று;
  cleanup_close முழு notify_socket_மரம்_fd = -1;
  cleanup_close முழு targetfd = -1;
  முழு did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = வெற்று;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = வெற்று;

  எனில் (host_notify_socket_path == வெற்று || container_notify_socket_path == வெற்று)
    திரும்பு 0;

  எனில் (rootfs == வெற்று)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  எனில் (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  targetfd = ret;

  notify_socket_மரம்_fd = get_private_data (container)->notify_socket_மரம்_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_மரம்_fd = -1;

  எனில் (notify_socket_மரம்_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_மரம்_fd, targetfd, வெற்று);
      எனில் (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          எனில் (UNLIKELY (targetfd < 0))
            திரும்பு targetfd;

          ret = do_mount (container, வெற்று, targetfd, container_notify_socket_path_dir, வெற்று,
                          MS_REC | MS_PRIVATE, வெற்று, LABEL_NONE, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
      இல்லையெனில் எனில் (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      இல்லையெனில்
        திரும்பு crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_மரம்_fd,
                                container_notify_socket_path_dir);
    }

  எனில் (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, வெற்று,
                      MS_BIND | MS_REC | MS_PRIVATE, வெற்று, LABEL_MOUNT, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

நிலையான முழு
make_parent_mount_private (மாறிலி எழுத்து *rootfs, libcrun_error_t *err)
{
  cleanup_close முழு rootfsfd = -1;
  கட்டமைப்பு stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (rootfsfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", rootfs);

  எனில் (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    திரும்பு crun_make_error (err, errno, "fstat `%s`", rootfs);

  க்கு (;;)
    {
      கட்டமைப்பு stat cur_st;
      libcrun_error_t tmp_err = வெற்று;
      முழு ret;
      cleanup_close முழு parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      எனில் (ret == 0)
        திரும்பு 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (வெற்று, proc_path, வெற்று, MS_PRIVATE, வெற்று);
      எனில் (ret == 0)
        திரும்பு 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      எனில் (parentfd < 0)
        {
          முழு saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          எனில் (ret != 0)
            திரும்பு crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          திரும்பு crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      எனில் (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        திரும்பு crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      எனில் (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        நிறுத்து;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  திரும்பு crun_make_error (err, 0, "make `%s` private", rootfs);
}

முழு
libcrun_set_mounts (கட்டமைப்பு container_entrypoint_s *entrypoint_args, libcrun_container_t *container, மாறிலி எழுத்து *rootfs, set_mounts_cb_t cb, வெறுமை *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  முழு is_user_ns = 0;
  முழு ret = 0;

  எனில் (rootfs == வெற்று || def->mounts == வெற்று)
    திரும்பு 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  எனில் (def->வேர்->readonly)
    {
      கட்டமைப்பு remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      முழு fd;

      fd = dup (get_private_data (container)->rootfsfd);
      எனில் (UNLIKELY (fd < 0))
        திரும்பு crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, வெற்று, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  எனில் (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      எனில் (UNLIKELY (is_user_ns < 0))
        திரும்பு is_user_ns;
    }

  எனில் (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  எனில் (cb)
    {
      ret = cb (cb_data, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = வெற்று;
      மாறிலி எழுத்து *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (வெறுமை) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

நிலையான முழு
umount_or_hide (மாறிலி எழுத்து *target, libcrun_error_t *err)
{
  முழு ret;

  ret = umount2 (target, MNT_DETACH);
  எனில் (UNLIKELY (ret < 0))
    {
      முழு saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      எனில் (errno == EINVAL)
        {
          ret = mount (வெற்று, target, "tmpfs", 0, "size=0k");
          எனில் (LIKELY (ret == 0))
            திரும்பு 0;
        }

      திரும்பு crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  திரும்பு ret;
}

நிலையான முழு
move_வேர் (மாறிலி எழுத்து *rootfs, libcrun_error_t *err)
{
  முழு ret;

  ret = chdir (rootfs);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = umount_or_hide ("/proc", err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "chdir to `%s`", rootfs);

  திரும்பு 0;
}

முழு
libcrun_finalize_mounts (கட்டமைப்பு container_entrypoint_s *entrypoint_args, libcrun_container_t *container, மாறிலி எழுத்து *rootfs,
                         set_mounts_cb_t after_pivot_cb, வெறுமை *cb_data, libcrun_error_t *err)
{
  முழு ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_வேர்_fd));

  எனில் (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      எனில் (get_private_data (container)->no_pivot)
        {
          ret = move_வேர் (rootfs, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
      இல்லையெனில்
        {
          ret = do_pivot (container, rootfs, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      ret = do_mount (container, வெற்று, -1, "/", வெற்று,
                      get_private_data (container)->rootfs_propagation,
                      வெற்று, LABEL_MOUNT, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = chdir ("/");
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "chdir to `/`");
    }

  எனில் (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = finalize_mounts (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

நிலையான முழு
maybe_open_மரம்_namespace (மாறிலி எழுத்து *rootfs, முழு *out_fd, libcrun_error_t *err)
{
  cleanup_close முழு rootfs_fd = -1;
  முழு மரம்_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (rootfs_fd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", rootfs);

  மரம்_fd = syscall_open_மரம் (rootfs_fd, "",
                               OPEN_மரம்_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  எனில் (மரம்_fd < 0)
    {
      எனில் (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        திரும்பு 0;
      திரும்பு crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = மரம்_fd;
  திரும்பு 0;
}

நிலையான கட்டமைப்பு libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  கட்டமைப்பு libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  எனில் (mount_fds == வெற்று)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  திரும்பு mount_fds;
}

நிலையான முழு
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free எழுத்து *mnt_data = வெற்று;
  unsigned long mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  மாறிலி எழுத்து *label_type = வெற்று;
  மாறிலி எழுத்து *label_val = வெற்று;
  மாறிலி எழுத்து *fstype;
  முழு mnt_fd;
  size_t j;

  எனில் (mount->type == வெற்று)
    திரும்பு -1;

  fstype = mount->type;

  க்கு (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  எனில் (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      முழு extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      எனில் (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          எனில் (label_type == வெற்று)
            crun_error_release (err);
          இல்லையெனில்
            label_val = def->linux->mount_label;
        }
    }

  எனில் (strcmp (fstype, "cgroup") == 0)
    {
      முழு cgroup_mode = libcrun_get_cgroup_mode (err);
      எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  எனில் (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, வெற்று, வெற்று, mnt_data);

  திரும்பு mnt_fd;
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
கட்டமைப்பு mnt_id_req_s
{
  uint32_t size;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

கட்டமைப்பு statmount_s
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
  எழுத்து str[];
};

நிலையான முழு
syscall_listmount (கட்டமைப்பு mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned முழு flags)
{
#ifdef __NR_listmount
  திரும்பு (முழு) syscall (__NR_listmount, req, ids, nr, flags);
#இல்லையெனில்
  (வெறுமை) req;
  (வெறுமை) ids;
  (வெறுமை) nr;
  (வெறுமை) flags;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

நிலையான முழு
syscall_statmount (கட்டமைப்பு mnt_id_req_s *req, கட்டமைப்பு statmount_s *buf, size_t size, unsigned முழு flags)
{
#ifdef __NR_statmount
  திரும்பு (முழு) syscall (__NR_statmount, req, buf, size, flags);
#இல்லையெனில்
  (வெறுமை) req;
  (வெறுமை) buf;
  (வெறுமை) size;
  (வெறுமை) flags;
  errno = ENOSYS;
  திரும்பு -1;
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
நிலையான bool
mounts_hit_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = வெற்று;
  size_t i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  க்கு (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      எனில் (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != வெற்று && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          நிறுத்து;
        }
    }
  எனில் (! has_absolute_bind)
    திரும்பு false;

  ids = xmalloc (capacity * அளவு (uint64_t));
  க்கு (;;)
    {
      கட்டமைப்பு mnt_id_req_s req = {
        .size = அளவு (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      முழு ret;

      எனில் (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * அளவு (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      எனில் (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        திரும்பு true;
      எனில் (ret == 0)
        நிறுத்து;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  க்கு (i = 0; i < n_ids; i++)
    {
      எழுத்து buffer[4096] __attribute__ ((aligned (8)));
      கட்டமைப்பு statmount_s *sm = (கட்டமைப்பு statmount_s *) buffer;
      கட்டமைப்பு mnt_id_req_s req = {
        .size = அளவு (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      மாறிலி எழுத்து *வேர், *point;
      size_t j;

      எனில் (syscall_statmount (&req, sm, அளவு (buffer), 0) < 0)
        தொடர்;
      எனில் ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        தொடர்;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      வேர் = sm->str + sm->mnt_root;
      எனில் (! has_prefix (வேர், "mnt:["))
        தொடர்;

      point = sm->str + sm->mnt_point;

      க்கு (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          மாறிலி எழுத்து *source = def->mounts[j]->source;
          size_t source_len;

          எனில் (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            தொடர்;
          எனில் (source == வெற்று || source[0] != '/')
            தொடர்;

          source_len = strlen (source);
          போது (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          எனில் (strncmp (point, source, source_len) != 0)
            தொடர்;

          /* The source itself is a pinned mount namespace.  */
          எனில் (point[source_len] == '\0')
            திரும்பு true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          எனில் (recursive && (source_len == 1 || point[source_len] == '/'))
            திரும்பு true;
        }
    }

  திரும்பு false;
}

நிலையான bool
can_use_open_மரம்_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  கட்டமைப்பு libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  எனில் (has_hooks || has_userns)
    திரும்பு false;

  எனில் (mounts_hit_pinned_mount_namespace (def))
    திரும்பு false;

  எனில் (container->context->no_open_மரம்_namespace)
    திரும்பு false;

  {
    libcrun_error_t tmp_err = வெற்று;
    முழு in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    எனில் (in_userns != 0)
      திரும்பு false;
  }

  mount_fds = get_fd_map (container);
  க்கு (i = 0; i < def->mounts_len; i++)
    {
      எனில் (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          எனில் (! has_cached_target)
            திரும்பு false;
        }
    }

  திரும்பு true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
நிலையான bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  எனில் (mount->type == வெற்று || strcmp (mount->type, "tmpfs") != 0)
    திரும்பு false;

  க்கு (i = 0; i < mount->options_len; i++)
    எனில் (mount->options[i] != வெற்று && has_prefix (mount->options[i], "mode="))
      திரும்பு false;

  திரும்பு true;
}

நிலையான முழு
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, எழுத்து **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close முழு rootfsfd = -1;
  cleanup_close முழு மரம்_fd = -1;
  libcrun_error_t tmp_err = வெற்று;
  bool use_open_மரம் = false;
  size_t i;
  முழு ret;

  எனில் (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, வெற்று, வெற்று, வெற்று, வெற்று);

  எனில் ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  விடுவி (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  க்கு (i = 0; i < def->mounts_len; i++)
    {
      கட்டமைப்பு libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close முழு mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      எனில் (mount_fds && mount_fds->fds[i] >= 0)
        தொடர்;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      எனில் (def->mounts[i]->source != வெற்று
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free எழுத்து *mnt_data = வெற்று;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          க்கு (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          எனில் (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free எழுத்து *target = வெற்று;
              libcrun_error_t tmp_err = வெற்று;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              எனில் (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              எனில் (get_private_data (container)->copy_symlink_targets == வெற்று)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * அளவு (எழுத்து *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = வெற்று;
            }
        }

      எனில் (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          எனில் (mount_fds && def->mounts[i]->source != வெற்று && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == வெற்று)
            {
              libcrun_error_t tmp_err = வெற்று;
              unsigned long propagation = MS_PRIVATE;

              எனில் (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              எனில் (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          தொடர்;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      எனில் (mnt_fd < 0)
        {
          crun_error_release (err);
          தொடர்;
        }

      எனில் (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close முழு destfd = -1;
          கட்டமைப்பு stat st;

          எனில் (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              எனில் (UNLIKELY (rootfsfd < 0))
                திரும்பு crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          எனில் (destfd < 0)
            crun_error_release (&tmp_err);
          இல்லையெனில் எனில் (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              முழு procfd = get_procfd (get_private_data (container), err);
              எனில் (UNLIKELY (procfd < 0))
                திரும்பு procfd;

              get_self_fd_path (fd_path, mnt_fd);
              ret = fchmodat (procfd, fd_path, st.st_mode & 07777, 0);
              (வெறுமை) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_empty_dir_cached (container, வெற்று, &tmp_err);
  எனில் (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  எனில் (! get_private_data (container)->joined_mount_ns
      && can_use_open_மரம்_namespace (container))
    use_open_மரம் = true;

  எனில் (use_open_மரம்)
    {
      ret = maybe_open_மரம்_namespace (*rootfs, &மரம்_fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  எனில் (மரம்_fd >= 0)
    preopen_needed_devs_and_notify (container);

  எனில் (மரம்_fd >= 0)
    {
      ret = setns (மரம்_fd, CLONE_NEWNS);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (வெற்று, "/", வெற்று, MS_REMOUNT | MS_BIND, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, வெற்று, -1, "/", வெற்று, MS_REC | MS_PRIVATE, வெற்று, LABEL_MOUNT, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = do_mount (container, வெற்று, -1, "/", வெற்று, rootfs_propagation, வெற்று, LABEL_MOUNT, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_வேர்_switched = true;
      விடுவி (*rootfs);
      *rootfs = xstrdup ("/");
    }
  இல்லையெனில்
    {
      எனில் (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, வெற்று, -1, "/", வெற்று, rootfs_propagation, வெற்று, LABEL_MOUNT, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = make_parent_mount_private (*rootfs, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, வெற்று, MS_BIND | MS_REC | MS_PRIVATE, வெற்று, LABEL_MOUNT, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  எனில் (! container->context->force_no_cgroup)
    {
      முழு cgroup_mode = libcrun_get_cgroup_mode (err);
      எனில் (UNLIKELY (cgroup_mode < 0))
        திரும்பு cgroup_mode;

      எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          எழுத்து *unified_cgroup_path = வெற்று;
          முழு procfd = get_procfd (get_private_data (container), err);
          எனில் (UNLIKELY (procfd < 0))
            திரும்பு procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  எனில் (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = வெற்று;
      திரும்பு ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = வெற்று;

  எனில் (மரம்_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  எனில் (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  திரும்பு 0;
}

முழு
libcrun_do_pivot_வேர் (libcrun_container_t *container, bool no_pivot, எழுத்து **rootfs, libcrun_error_t *err)
{
  முழு ret;

  எனில் (*rootfs == வெற்று)
    திரும்பு 0;

  எனில் (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      எனில் (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "chdir to `/`");
        }
    }
  இல்லையெனில்
    {
      ret = chroot (*rootfs);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "chdir to `/`");

      விடுவி (*rootfs);
      *rootfs = xstrdup ("/");
    }

  திரும்பு 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
முழு
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  கட்டமைப்பு stat statbuf;
  cleanup_close முழு fd;
  முழு ret;
  முழு i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  எனில் (UNLIKELY (fd == -1))
    திரும்பு crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  க்கு (i = 0; i <= 2; i++)
    {
      எனில் (UNLIKELY (fstat (i, &statbuf) == -1))
        திரும்பு crun_make_error (err, errno, "stat fd `%d`", i);
      எனில் (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          எனில் (UNLIKELY (dup2 (fd, i) == -1))
            திரும்பு crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  திரும்பு 0;
}

நிலையான முழு
uidgidmap_helper (எழுத்து *helper, pid_t pid, மாறிலி எழுத்து *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  எழுத்து pid_fmt[16];
  எழுத்து *args[MAX_ARGS + 1];
  எழுத்து *next;
  cleanup_free எழுத்து *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  முழு ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, அளவு (pid_fmt), "%d", pid);
  எனில் (UNLIKELY (ret >= (முழு) அளவு (pid_fmt)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  போது (nargs < MAX_ARGS)
    {
      எழுத்து *p = strsep (&next, " \n");
      எனில் (next == வெற்று)
        நிறுத்து;
      args[nargs++] = p;
    }
  args[nargs++] = வெற்று;

  ret = run_process (args, err);
  எனில் (ret < 0)
    திரும்பு ret;
  எனில் (ret > 0)
    திரும்பு crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  திரும்பு 0;
}

நிலையான முழு
newgidmap (pid_t pid, மாறிலி எழுத்து *map_file, libcrun_error_t *err)
{
  திரும்பு uidgidmap_helper ("newgidmap", pid, map_file, err);
}

நிலையான முழு
newuidmap (pid_t pid, மாறிலி எழுத்து *map_file, libcrun_error_t *err)
{
  திரும்பு uidgidmap_helper ("newuidmap", pid, map_file, err);
}

நிலையான முழு
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  முழு ret;
  cleanup_close முழு fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு fd;

  ret = TEMP_FAILURE_RETRY (லிக (fd, "deny", 4));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  திரும்பு 0;
}

நிலையான முழு
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  முழு ret;
  cleanup_free எழுத்து *content = வெற்று;

  எனில் (get_private_data (container)->deny_setgroups)
    திரும்பு 0;

  எனில் (container->container_def->annotations)
    {
      மாறிலி எழுத்து *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      எனில் (annotation)
        திரும்பு strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close முழு fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    எனில் (fd < 0)
      ret = fd;
    இல்லையெனில்
      ret = read_all_fd (fd, "self/setgroups", &content, வெற்று, err);
  }
  எனில் (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      எனில் (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          திரும்பு 1;
        }
      திரும்பு ret;
    }

  திரும்பு strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

முழு
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = வெற்று;
  size_t additional_gids_len = 0;
  முழு can_do_setgroups;
  முழு ret;

  எனில் (process != வெற்று && process->user != வெற்று)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  எனில் (UNLIKELY (can_do_setgroups < 0))
    திரும்பு can_do_setgroups;

  எனில் (can_do_setgroups == 0)
    {
      மாறிலி எழுத்து *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      எனில் (annotation && strcmp (annotation, "0") != 0)
        திரும்பு 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      க்கு (i = 0; i < additional_gids_len; i++)
        எனில் (additional_gids[i] != process->user->gid)
          திரும்பு crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (முழு) additional_gids[i]);
      திரும்பு 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "setgroups");

  திரும்பு 0;
}

முழு
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  எனில் (get_private_data (container)->unshare_cgroupns)
    {
      முழு ret = unshare (CLONE_NEWCGROUP);
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (errno != EINVAL)
            திரும்பு crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  திரும்பு 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
முழு
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  எனில் (len != 1)
    திரும்பு 0;

  எனில் (mappings[0]->size != 1)
    திரும்பு 0;

  எனில் (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    திரும்பு 0;

  திரும்பு 1;
}

நிலையான முழு
write_id_map (libcrun_container_t *container, pid_t pid, மாறிலி எழுத்து *map_name,
              முழு (*idmap_helper) (pid_t, மாறிலி எழுத்து *, libcrun_error_t *), மாறிலி எழுத்து *helper_name,
              bool warn_on_failure, மாறிலி எழுத்து *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  முழு ret = 0;

  எனில் (container->host_uid)
    ret = idmap_helper (pid, map, err);
  எனில் (container->host_uid == 0 || ret < 0)
    {
      cleanup_close முழு fd = -1;

      எனில் (ret < 0)
        {
          எனில் (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      எனில் (UNLIKELY (fd < 0))
        திரும்பு fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      எனில் (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free எழுத்து *single_mapping = வெற்று;
          crun_error_release (err);

          எனில் (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          எனில் (UNLIKELY (fd < 0))
            திரும்பு fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  திரும்பு ret;
}

முழு
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free எழுத்து *uid_map = வெற்று;
  cleanup_free எழுத்து *gid_map = வெற்று;
  size_t uid_map_len = 0, gid_map_len = 0;
  முழு ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  எனில் ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    திரும்பு 0;

  எனில் (! def || ! def->linux)
    திரும்பு 0;

  எனில் (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      uid_map_len = (size_t) ret;
      எனில் (uid_map == வெற்று)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }

  எனில் (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      gid_map_len = (size_t) ret;
      எனில் (gid_map == வெற்று)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

கட்டமைப்பு all_caps_s
{
  unsigned long effective[2];
  unsigned long permitted[2];
  unsigned long inheritable[2];
  unsigned long ambient[2];
  unsigned long bounding[2];
};

நிலையான முழு
has_cap_on (முழு cap, long unsigned *caps)
{
  எனில் (cap < 32)
    திரும்பு CAP_TO_MASK_0 (cap) & caps[0];
  திரும்பு (CAP_TO_MASK_1 (cap) & caps[1]);
}

நிலையான unsigned long cap_last_cap;

முழு
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close முழு fd = -1;
  முழு ret;
  எழுத்து buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  எனில் (fd < 0)
    திரும்பு fd;
  ret = TEMP_FAILURE_RETRY (பட (fd, buffer, அளவு (buffer) - 1));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, வெற்று, 10);
  எனில் (errno != 0)
    திரும்பு crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  திரும்பு 0;
}

நிலையான முழு
set_required_caps (கட்டமைப்பு all_caps_s *caps, uid_t uid, gid_t gid, முழு no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  முழு ret;
  கட்டமைப்பு __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  கட்டமைப்பு __user_cap_data_struct data[2] = { { 0 } };

  எனில் (cap_last_cap == 0)
    திரும்பு crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  க்கு (cap = 0; cap <= cap_last_cap; cap++)
    எனில் (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        எனில் (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          திரும்பு crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  எனில் (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    திரும்பு crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  எனில் (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    திரும்பு crun_make_error (err, errno, "prctl reset ambient");

  க்கு (cap = 0; cap <= cap_last_cap; cap++)
    எனில் (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        எனில் (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          திரும்பு crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  எனில் (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "prctl reset KEEPCAPS");

      எனில் (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          க்கு (cap = 0; cap <= cap_last_cap; cap++)
            எனில் (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "capset");
        }
    }
#endif

  எனில் (no_new_privs)
    எனில் (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      திரும்பு crun_make_error (err, errno, "prctl set no new privs");

  திரும்பு 0;
}

நிலையான முழு
read_caps (unsigned long caps[2], எழுத்து **மதிப்புகள், size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  க்கு (i = 0; i < len; i++)
    {
      cap_மதிப்பு_t cap;
      எனில் (cap_from_name (மதிப்புகள்[i], &cap) < 0)
        திரும்பு crun_make_error (err, 0, "unknown capability `%s`", மதிப்புகள்[i]);

      எனில் (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      இல்லையெனில்
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#இல்லையெனில்
  caps[0] = 0;
  caps[1] = 0;
#endif
  திரும்பு 0;
}

முழு
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  எனில் (proc->selinux_label)
    திரும்பு set_selinux_label (container, proc->selinux_label, now, err);

  திரும்பு 0;
}

முழு
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  எனில் (proc->apparmor_profile)
    திரும்பு set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  திரும்பு 0;
}

முழு
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  முழு no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  கட்டமைப்பு all_caps_s caps = {};
  முழு ret;

  எனில் (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

கட்டமைப்பு rlimit_s
{
  மாறிலி எழுத்து *name;
  முழு மதிப்பு;
};

கட்டமைப்பு rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
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
                              { "RLIMIT_STACK", RLIMIT_அடுக்கு },
                              { வெற்று, 0 } };

நிலையான முழு
get_rlimit_resource (மாறிலி எழுத்து *name)
{
  கட்டமைப்பு rlimit_s *it;
  க்கு (it = rlimits; it->name; it++)
    எனில் (strcmp (it->name, name) == 0)
      திரும்பு it->மதிப்பு;
  திரும்பு -1;
}

முழு
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  க்கு (i = 0; i < len; i++)
    {
      க்கு (j = i + 1; j < len; j++)
        {
          எனில் (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            திரும்பு crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  க்கு (i = 0; i < len; i++)
    {
      கட்டமைப்பு rlimit limit;
      எழுத்து *type = new_rlimits[i]->type;
      முழு resource = get_rlimit_resource (type);
      எனில் (UNLIKELY (resource < 0))
        திரும்பு crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      எனில் (UNLIKELY (setrlimit (resource, &limit) < 0))
        திரும்பு crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  திரும்பு 0;
}

முழு
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  முழு has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  முழு ret;
  எனில் (def->hostname == வெற்று || def->hostname[0] == '\0')
    திரும்பு 0;
  எனில் (! has_uts)
    திரும்பு crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "sethostname");
  திரும்பு 0;
}

முழு
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  முழு has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  முழு ret;
  எனில் (is_empty_string (def->domainname))
    திரும்பு 0;
  எனில் (! has_uts)
    திரும்பு crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "setdomainname");
  திரும்பு 0;
}

முழு
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close முழு fd = -1;
  முழு ret;
  எழுத்து oom_buffer[16];
  எனில் (def->process == வெற்று || ! def->process->oom_score_adj_present)
    திரும்பு 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, அளவு (oom_buffer), "%i", def->process->oom_score_adj);
  எனில் (UNLIKELY (ret >= (முழு) அளவு (oom_buffer)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  எனில் (fd < 0)
    திரும்பு fd;
  ret = TEMP_FAILURE_RETRY (லிக (fd, oom_buffer, strlen (oom_buffer)));
  எனில் (ret < 0)
    திரும்பு crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  திரும்பு 0;
}

மாறிலி எழுத்து *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  வெற்று
};

நிலையான முழு
validate_sysctl (மாறிலி எழுத்து *original_சாவி, மாறிலி எழுத்து *original_மதிப்பு, மாறிலி எழுத்து *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  மாறிலி எழுத்து *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  எனில் (name[0] == '/' || path_has_dot_dot_component (name))
    திரும்பு crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_சாவி);

  எனில் (has_prefix (name, "fs/mqueue/"))
    {
      எனில் (namespaces_created & CLONE_NEWIPC)
        திரும்பு 0;

      namespace = "IPC";
      செல் fail;
    }

  எனில் (has_prefix (name, "kernel/"))
    {
      size_t i;

      க்கு (i = 0; sysctlRequiringIPC[i]; i++)
        எனில் (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            எனில் (namespaces_created & CLONE_NEWIPC)
              திரும்பு 0;

            namespace = "IPC";
            செல் fail;
          }

      எனில் (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          எனில் (! is_empty_string (def->domainname) && (strcmp (original_மதிப்பு, def->domainname) != 0))
            திரும்பு crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_சாவி);

          எனில் (namespaces_created & CLONE_NEWUTS)
            திரும்பு 0;

          namespace = "UTS";
          செல் fail;
        }

      எனில் (strcmp (name, "kernel/hostname") == 0)
        திரும்பு crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_சாவி);
    }
  எனில் (has_prefix (name, "net/"))
    {
      எனில் (namespaces_created & CLONE_NEWNET)
        திரும்பு 0;

      namespace = "network";
      செல் fail;
    }

  திரும்பு crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_சாவி);

fail:
  திரும்பு crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_சாவி, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
நிலையான எழுத்து *
sysctl_error_reason (மாறிலி எழுத்து *name, முழு namespaces_created, முழு errno_)
{

  எனில் (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    திரும்பு xstrdup ("are all the IDs mapped in the user namespace?");

  திரும்பு வெற்று;
}

முழு
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close முழு dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  எனில் (def->linux == வெற்று || def->linux->sysctl == வெற்று || def->linux->sysctl->len == 0)
    திரும்பு 0;

  க்கு (i = 0; i < def->linux->namespaces_len; i++)
    {
      முழு மதிப்பு;

      மதிப்பு = libcrun_find_namespace (def->linux->namespaces[i]->type);
      எனில் (UNLIKELY (மதிப்பு < 0))
        திரும்பு crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= மதிப்பு;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  எனில் (UNLIKELY (dirfd < 0))
    திரும்பு dirfd;

  க்கு (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free எழுத்து *name = வெற்று;
      cleanup_close முழு fd = -1;
      முழு ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      எனில் (def->linux->sysctl->சாவிகள்[i] == வெற்று || def->linux->sysctl->மதிப்புகள்[i] == வெற்று)
        திரும்பு crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_சாவி_to_proc_path (def->linux->sysctl->சாவிகள்[i]);

      ret = validate_sysctl (def->linux->sysctl->சாவிகள்[i], def->linux->sysctl->மதிப்புகள்[i], name, namespaces_created, def, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      எனில் (UNLIKELY (fd < 0))
        திரும்பு crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (லிக (fd, def->linux->sysctl->மதிப்புகள்[i], strlen (def->linux->sysctl->மதிப்புகள்[i])));
      எனில் (UNLIKELY (ret < 0))
        {
          முழு saved_errno = errno;
          cleanup_free எழுத்து *reason = வெற்று;

          reason = sysctl_error_reason (def->linux->sysctl->சாவிகள்[i], namespaces_created, saved_errno);
          திரும்பு crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  திரும்பு 0;
}

நிலையான முழு
open_terminal (எழுத்து **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close முழு fd = -1;
  uid_t uid = 0;
  முழு ret;

  எனில் (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  திரும்பு ret;
}

எழுத்து *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  திரும்பு get_private_data (container)->external_descriptors;
}

முழு
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  மாறிலி எழுத்து *buf = வெற்று;
  json_gen_ctx *gen = வெற்று;
  size_t buf_len;
  முழு ret;
  முழு i;

  எனில் (! json_gen_init (&gen, வெற்று))
    திரும்பு crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  எனில் (UNLIKELY (ret != json_gen_status_ok))
    செல் json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  க்கு (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      எழுத்து link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      எனில் (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          எனில் (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          இல்லையெனில்
            {
              json_gen_free (gen);
              திரும்பு crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      எனில் (UNLIKELY (ret != json_gen_status_ok))
        செல் json_error;
    }

  ret = json_gen_array_close (gen);
  எனில் (UNLIKELY (ret != json_gen_status_ok))
    செல் json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  எனில் (UNLIKELY (ret != json_gen_status_ok))
    செல் json_error;

  எனில் (buf)
    {
      எழுத்து *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  திரும்பு 0;

json_error:
  எனில் (gen)
    json_gen_free (gen);
  திரும்பு json_gen_error_to_crun_error (ret, err);
}

முழு
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free எழுத்து *pty = வெற்று;
  cleanup_close முழு fd = -1;
  முழு ret;

  எனில் (def->process == வெற்று || ! def->process->terminal)
    திரும்பு 0;

  fd = open_terminal (&pty, def->process, err);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு fd;

  எனில் (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", வெற்று, MS_BIND, வெற்று, LABEL_MOUNT, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      ret = unlink ("/dev/console");
      எனில் (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          எனில் (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", வெற்று, MS_BIND, வெற்று, LABEL_MOUNT, err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;

              திரும்பு get_and_reset (&fd);
            }
          திரும்பு crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  திரும்பு get_and_reset (&fd);
}

நிலையான bool
read_error_from_sync_socket (முழு sync_socket_fd, முழு *error, எழுத்து **str)
{
  cleanup_free எழுத்து *b = வெற்று;
  size_t size;
  முழு code = 0;
  முழு ret;

  எனில் (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (பட (sync_socket_fd, &code, அளவு (code)));
      எனில் (UNLIKELY (ret != (முழு) அளவு (code)))
        திரும்பு false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (பட (sync_socket_fd, &size, அளவு (size)));
  எனில் (UNLIKELY (ret != (முழு) அளவு (size)))
    திரும்பு false;

  எனில் (size == 0)
    திரும்பு false;

  எனில் (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (பட (sync_socket_fd, b, size));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு false;

  b[ret] = '\0';

  *str = b;
  b = வெற்று;
  திரும்பு true;
}

நிலையான bool
send_error_to_sync_socket (முழு sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  முழு ret;
  முழு code;
  எழுத்து *செய்தி;

  எனில் (err == வெற்று || *err == வெற்று)
    திரும்பு false;

  code = crun_error_get_errno (err);

  எனில் (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (லிக (sync_socket_fd, "1", 1));
      எனில் (UNLIKELY (ret < 0))
        திரும்பு false;
    }

  ret = TEMP_FAILURE_RETRY (லிக (sync_socket_fd, &code, அளவு (code)));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு false;

  செய்தி = (*err)->செய்தி;
  size = strlen (செய்தி) + 1;
  ret = TEMP_FAILURE_RETRY (லிக (sync_socket_fd, &size, அளவு (size)));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு false;

  ret = TEMP_FAILURE_RETRY (லிக (sync_socket_fd, செய்தி, size));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு false;

  திரும்பு true;
}

நிலையான உள்வரி முழு
send_success_to_sync_socket (முழு sync_socket, libcrun_error_t *err)
{
  மாறிலி முழு success = 0;
  முழு ret;

  ret = TEMP_FAILURE_RETRY (லிக (sync_socket, &success, அளவு (success)));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "write to sync socket");

  திரும்பு 0;
}

நிலையான __attribute__ ((noreturn)) வெறுமை
send_error_to_sync_socket_and_die (முழு sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  எழுத்து *செய்தி;

  எனில் (err == வெற்று || *err == வெற்று)
    _safe_exit (EXIT_FAILURE);

  எனில் (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      செய்தி = (*err)->செய்தி;
      libcrun_fail_with_error (errno, "%s", செய்தி);
    }
  _safe_exit (EXIT_FAILURE);
}

நிலையான முழு
expect_success_from_sync_socket (முழு sync_fd, libcrun_error_t *err)
{
  cleanup_free எழுத்து *err_str = வெற்று;
  முழு res = 1;
  முழு ret;

  ret = TEMP_FAILURE_RETRY (பட (sync_fd, &res, அளவு (res)));
  எனில் (UNLIKELY (ret != அளவு (res)))
    திரும்பு crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  எனில் (res == 0)
    திரும்பு 0;

  எனில் (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      எனில் (! is_empty_string (err_str))
        திரும்பு crun_make_error (err, res, "%s", err_str);
    }

  திரும்பு crun_make_error (err, res, "read from sync socket");
}

நிலையான முழு
join_namespaces (runtime_spec_schema_config_schema *def, முழு *namespaces_to_join, முழு n_namespaces_to_join,
                 முழு *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  முழு ret;
  முழு i;

  க்கு (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free எழுத்து *cwd = வெற்று;
      முழு orig_index = namespaces_to_join_index[i];
      முழு மதிப்பு;

      எனில் (namespaces_to_join[i] < 0)
        தொடர்;

      /* Skip the user namespace.  */
      மதிப்பு = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      எனில் (மதிப்பு == CLONE_NEWUSER)
        தொடர்;

      எனில் (மதிப்பு == CLONE_NEWNS)
        {
          cwd = getcwd (வெற்று, 0);
          எனில் (UNLIKELY (cwd == வெற்று))
            திரும்பு crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], மதிப்பு);
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (ignore_join_errors)
            தொடர்;
          திரும்பு crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      எனில் (மதிப்பு == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  திரும்பு 0;
}

#define MAX_NAMESPACES 10

கட்டமைப்பு init_status_s
{
  /* fd to the namespace to join.  */
  முழு fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  முழு index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  முழு மதிப்பு[MAX_NAMESPACES];
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
  முழு userns_index;
  /* def->linux->namespaces userns.  */
  முழு userns_index_origin;

  /* All namespaces created/joined by the container.  */
  முழு all_namespaces;

  /* What namespaces are still missing to be created.  */
  முழு namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  முழு idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  முழு idx_timens_to_join_immediately;
};

வெறுமை
cleanup_free_init_statusp (கட்டமைப்பு init_status_s *ns)
{
  size_t i;

  எனில் (ns == வெற்று)
    திரும்பு;

  க்கு (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

நிலையான முழு
configure_init_status (கட்டமைப்பு init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  க்கு (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  க்கு (i = 0; i < def->linux->namespaces_len; i++)
    {
      முழு மதிப்பு = libcrun_find_namespace (def->linux->namespaces[i]->type);
      எனில் (UNLIKELY (மதிப்பு < 0))
        திரும்பு crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      எனில் (ns->all_namespaces & மதிப்பு)
        திரும்பு crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= மதிப்பு;

      எனில் (def->linux->namespaces[i]->path == வெற்று)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= மதிப்பு;
        }
      இல்லையெனில்
        {
          முழு fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          எனில் (ns->fd_len >= MAX_NAMESPACES)
            திரும்பு crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          எனில் (UNLIKELY (fd < 0))
            திரும்பு crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          எனில் (மதிப்பு == CLONE_NEWUSER)
            {
              ns->userns_index = ns->fd_len;
              ns->userns_index_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->index[ns->fd_len] = i;
          ns->மதிப்பு[ns->fd_len] = மதிப்பு;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  எனில் (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  திரும்பு 0;
}

/* Detect if root is available in the container.  */
நிலையான bool
வேர்_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  க்கு (i = 0; i < len; i++)
    எனில் (mappings[i]->container_id == 0)
      திரும்பு true;

  திரும்பு false;
}

நிலையான கட்டமைப்பு libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  கட்டமைப்பு libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  எனில் (dev_fds == வெற்று)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  திரும்பு dev_fds;
}

#define இலக்கம்_NEEDED_DEVS (அளவு (needed_devs) / அளவு (needed_devs[0]) - 1)

நிலையான கட்டமைப்பு libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  கட்டமைப்பு libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  எனில் (fds == வெற்று)
    {
      fds = make_libcrun_fd_map (இலக்கம்_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  திரும்பு fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  எனில் (recursive)
    *recursive = false;

  எனில் (src_nofollow != வெற்று)
    *src_nofollow = false;

  க்கு (i = 0; i < mnt->options_len; i++)
    {
      எனில் (mnt->options[i] == வெற்று)
        தொடர்;

      எனில் (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          எனில் (src_nofollow == வெற்று || *src_nofollow)
            நிறுத்து;
        }
      எனில் (strcmp (mnt->options[i], "rbind") == 0)
        {
          எனில் (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          எனில் (src_nofollow == வெற்று || *src_nofollow)
            நிறுத்து;
        }
      எனில் (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  திரும்பு ret;
}

நிலையான எழுத்து *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  க்கு (i = 0; i < mnt->options_len; i++)
    {
      எனில் (mnt->options[i] == வெற்று)
        தொடர்;

      எனில் (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          திரும்பு mnt->options[i];
        }
      எனில் (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          திரும்பு mnt->options[i];
        }
    }
  திரும்பு வெற்று;
}

நிலையான முழு
open_mount_type (மாறிலி எழுத்து *type, முழு *out_fd, libcrun_error_t *err)
{
  cleanup_close முழு fsopen_fd = -1;
  cleanup_close முழு newfs_fd = -1;
  முழு ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  எனில் (UNLIKELY (fsopen_fd < 0))
    திரும்பு crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, வெற்று, வெற்று, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  எனில் (UNLIKELY (newfs_fd < 0))
    திரும்பு crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  திரும்பு 0;
}

நிலையான முழு
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, முழு *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close முழு newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  கட்டமைப்பு mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close முழு fd = -1;
  மாறிலி எழுத்து *idmap_option;
  bool recursive = false;
  மாறிலி எழுத்து *options = வெற்று;
  bool has_mappings;
  முழு ret;
  எழுத்து *extra_செய்தி = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != வெற்று);
  எனில் (has_mappings_out)
    *has_mappings_out = has_mappings;
  எனில் (! has_mappings)
    திரும்பு 0;

  எனில் ((mnt->uid_mappings == வெற்று) != (mnt->gid_mappings == வெற்று))
    திரும்பு crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  எனில் (idmap_option)
    {
      options = strchr (idmap_option, '=');
      எனில் (options)
        {
          /* Skip the '=' itself.  */
          options++;
          எனில் (options[0] == '\0')
            options = வெற்று;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  எனில் (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு fd;

  எனில் (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_மரம் (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      எனில் (UNLIKELY (newfs_fd < 0))
        திரும்பு crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  இல்லையெனில்
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno == EINVAL)
        {
          extra_செய்தி = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      திரும்பு crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_செய்தி);
    }

  *out_fd = get_and_reset (&newfs_fd);
  திரும்பு 0;
}

நிலையான uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  எனில் (! def || ! def->linux)
    செல் exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  எனில் (! mappings)
    செல் exit;

  க்கு (i = 0; i < len; i++)
    {
      எனில் (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        திரும்பு id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  திரும்பு is_uid ? get_overflow_uid () : get_overflow_gid ();
}

நிலையான முழு
precreate_device (libcrun_container_t *container, முழு devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  எழுத்து name[64];
  mode_t type;
  dev_t dev;
  முழு ret;

  ret = snprintf (name, அளவு (name), "%zu", i);
  எனில் (UNLIKELY (ret >= (முழு) அளவு (name)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "chown `%s`", device->path);

  திரும்பு get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

நிலையான முழு
send_mounts (முழு sync_socket_host, கட்டமைப்பு libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  முழு ret;

  ret = TEMP_FAILURE_RETRY (லிக (sync_socket_host, &how_many, அளவு (how_many)));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "write to sync socket");

  க்கு (i = 0; i < total; i++)
    {
      எனில் (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (எழுத்து *) &i, அளவு (i), err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }
  திரும்பு 0;
}

நிலையான முழு
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, முழு sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map கட்டமைப்பு libcrun_fd_map *mount_fds = வெற்று;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free எழுத்து *rootfs = வெற்று;
  size_t how_many = 0;
  size_t i;
  முழு ret;

  எனில் (def->mounts_len == 0)
    திரும்பு 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  எனில் (def->வேர் && def->வேர்->path)
    rootfs = realpath (def->வேர்->path, வெற்று);

  எனில் (! has_userns)
    {
      முழு is_in_userns = check_running_in_user_namespace (err);
      எனில் (UNLIKELY (is_in_userns < 0))
        திரும்பு is_in_userns;

      has_userns = is_in_userns > 0;
    }

  க்கு (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      முழு mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      எனில் (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == வெற்று)
        {
          unsigned long propagation = 0;

          எனில் (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, வெற்று, வெற்று, வெற்று, வெற்று) & ALL_PROPAGATIONS_NO_REC;
          எனில் (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          எனில் (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      எனில் (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  திரும்பு send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

நிலையான முழு
prepare_and_send_dev_mounts (libcrun_container_t *container, முழு sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map கட்டமைப்பு libcrun_fd_map *dev_fds = வெற்று;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close முழு current_mountns = -1;
  cleanup_free எழுத்து *state_dir = வெற்று;
  cleanup_free எழுத்து *devs_path = வெற்று;
  cleanup_close முழு devs_mountfd = -1;
  cleanup_close முழு targetfd = -1;
  மாறிலி எழுத்து *context_type = வெற்று;
  மாறிலி எழுத்து *label = வெற்று;
  size_t how_many = 0;
  size_t i;
  முழு ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  எனில் (def->linux == வெற்று || def->linux->devices_len == 0)
    திரும்பு 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  எனில் (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    திரும்பு send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_வேர் : வெற்று),
                                     (container->context ? container->context->id : வெற்று), err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = mkdir (devs_path, 0700);
  எனில் (UNLIKELY (ret < 0 && errno != EEXIST))
    திரும்பு crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  எனில் (UNLIKELY (current_mountns < 0))
    திரும்பு current_mountns;

  ret = unshare (CLONE_NEWNS);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (வெற்று, "/", வெற்று, MS_REC | MS_PRIVATE, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      செல் restore_mountns;
    }

  எனில் (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      எனில் (UNLIKELY (context_type == வெற்று))
        {
          ret = -1;
          செல் restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", வெற்று, context_type, label, வெற்று);
  எனில் (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      செல் restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      செல் restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      செல் restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      செல் restore_mountns;
    }

  க்கு (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      எனில் (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          தொடர்;
        }

      dev_fds->fds[i] = ret;

      எனில் (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  எனில் (ns_changed && current_mountns >= 0)
    {
      முழு setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      எனில் (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          திரும்பு crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  திரும்பு ret;
}

நிலையான முழு
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, முழு sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map கட்டமைப்பு libcrun_fd_map *fds = வெற்று;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (இலக்கம்_NEEDED_DEVS);

  எனில் (! has_userns || geteuid () > 0)
    திரும்பு send_mounts (sync_socket_host, fds, 0, இலக்கம்_NEEDED_DEVS, err);

  க்கு (i = 0; i < இலக்கம்_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      முழு fd = syscall_open_மரம் (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      எனில் (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  திரும்பு send_mounts (sync_socket_host, fds, how_many, இலக்கம்_NEEDED_DEVS, err);
}

நிலையான முழு
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, முழு sync_socket_host, libcrun_error_t *err)
{
  முழு ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

நிலையான முழு
receive_mounts (கட்டமைப்பு libcrun_fd_map *fds, முழு sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  முழு ret;

  எனில் (fds->nfds == 0)
    திரும்பு 0;

  ret = TEMP_FAILURE_RETRY (பட (sync_socket_container, &how_many, அளவு (how_many)));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "read from sync socket");

  க்கு (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (எழுத்து *) &index, அளவு (index), err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      எனில் (index >= fds->nfds)
        திரும்பு crun_make_error (err, 0, "invalid mount data received");

      எனில் (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  திரும்பு 0;
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
நிலையான முழு
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  முழு ret;

  எனில் (! container->host_has_cap_sys_ptrace)
    திரும்பு 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  திரும்பு 0;
}

நிலையான முழு
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  முழு ret;

  எனில் (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool வேர்_mapped = false;

      எனில் (def->linux->uid_mappings_len != 0)
        {
          வேர்_mapped = வேர்_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          எனில் (! வேர்_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      எனில் (def->linux->gid_mappings_len != 0)
        {
          வேர்_mapped = வேர்_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          எனில் (! வேர்_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  எனில் (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    திரும்பு crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "setresgid to `%d`", gid);

  திரும்பு 0;
}

நிலையான bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  திரும்பு mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, வெற்று, வெற்று);
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
நிலையான முழு
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  கட்டமைப்பு libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close முழு socket_parent = -1;
  cleanup_close முழு socket_child = -1;
  cleanup_close முழு வேர்_fd = -1;
  cleanup_close முழு cwd_fd = -1;
  size_t i, missing = 0;
  முழு saved_errno;
  முழு sockets[2];
  pid_t pid;
  முழு ret;

  எனில் (def->வேர் == வெற்று || def->வேர்->path == வெற்று)
    திரும்பு 0;

  க்கு (i = 0; i < def->mounts_len; i++)
    எனில் (is_proc_mount (def->mounts[i]))
      missing++;
  எனில் (missing == 0)
    திரும்பு 1;

  வேர்_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (வேர்_fd < 0))
    திரும்பு crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (cwd_fd < 0))
    திரும்பு crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->வேர்->path);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு 0;

  pid = fork ();
  saved_errno = errno;
  எனில் (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      எனில் (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (வெறுமை) unshare (CLONE_NEWNS);

      க்கு (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close முழு fd = -1;

          எனில் (! is_proc_mount (def->mounts[i]))
            தொடர்;

          fd = open_mount_of_type (container, def->mounts[i], err);
          எனில் (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (எழுத்து *) &i, அளவு (i), err);
          எனில் (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  எனில் (UNLIKELY (fchdir (வேர்_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    திரும்பு crun_make_error (err, errno, "restore the root directory");

  எனில் (UNLIKELY (pid < 0))
    திரும்பு crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  போது (missing > 0)
    {
      libcrun_error_t tmp_err = வெற்று;

      ret = receive_fd_from_socket_with_payload (socket_parent, (எழுத்து *) &i, அளவு (i), &tmp_err);
      எனில் (ret < 0)
        {
          crun_error_release (&tmp_err);
          நிறுத்து;
        }
      எனில் (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          நிறுத்து;
        }

      எனில் (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, வெற்று, 0);

  திரும்பு missing == 0 ? 1 : 0;
}

நிலையான முழு
init_container (libcrun_container_t *container, முழு sync_socket_container, கட்டமைப்பு init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  கட்டமைப்பு libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close முழு timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  முழு ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      எனில் (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      எனில் (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      எனில் (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          delayed_pidns_fork = ret > 0;
        }

      எனில் (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          ret = TEMP_FAILURE_RETRY (லிக (sync_socket_container, &new_pid, அளவு (new_pid)));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "write to sync socket");
        }
      இல்லையெனில்
        {
          new_pid = fork ();
          எனில் (UNLIKELY (new_pid < 0))
            திரும்பு crun_make_error (err, errno, "fork");
        }

      எனில் (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          எனில் (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (லிக (sync_socket_container, &new_pid, அளவு (new_pid)));
          எனில் (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = libcrun_set_oom (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  எனில் ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      க்கு (i = 0; i < def->mounts_len; i++)
        {
          முழு fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          எனில் (def->mounts[i]->type == வெற்று)
            தொடர்;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          எனில் (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, வெற்று, வெற்று, வெற்று);
          எனில் (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, வெற்று, வெற்று, வெற்று);

          எனில் (fd >= 0)
            {
              எனில் (mount_fds->fds[i] >= 0)
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
  எனில் (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      எனில் (UNLIKELY (timens_offsets_fd < 0))
        திரும்பு timens_offsets_fd;
    }

  எனில் (init_status->all_namespaces & CLONE_NEWUSER)
    {
      எனில் (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      எனில் (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
      இல்லையெனில்
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "unshare");
    }

  எனில் (def->linux->time_offsets)
    {
      எழுத்து fmt_buffer[128];
      முழு fd = timens_offsets_fd;

      எனில் (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, அளவு (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          எனில் (UNLIKELY (ret >= (முழு) அளவு (fmt_buffer)))
            திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long முழு) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = லிக (fd, fmt_buffer, strlen (fmt_buffer));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      எனில் (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long முழு) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, அளவு (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          எனில் (UNLIKELY (ret >= (முழு) அளவு (fmt_buffer)))
            திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

          ret = லிக (fd, fmt_buffer, strlen (fmt_buffer));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  எனில் (init_status->must_fork)
    {
      எனில் (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          delayed_pidns_fork = ret > 0;
        }

      எனில் (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          எனில் (UNLIKELY (pid_container < 0))
            திரும்பு crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      எனில் (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          ret = TEMP_FAILURE_RETRY (லிக (sync_socket_container, &pid_container, அளவு (pid_container)));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "write to sync socket");

          எனில் (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  திரும்பு get_private_data (container)->delayed_pidns_fork;
}

முழு
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close முழு client_fd = -1;
  cleanup_close முழு pidfd = -1;
  மாறிலி எழுத்து *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  எனில் (v == வெற்று)
    திரும்பு 0;

  pidfd = syscall_pidfd_open (pid, 0);
  எனில் (UNLIKELY (pidfd < 0))
    திரும்பு crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  எனில் (UNLIKELY (client_fd < 0))
    திரும்பு client_fd;

  திரும்பு send_fd_to_socket (client_fd, pidfd, err);
}

நிலையான bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  எனில் (process == வெற்று || process->exec_cpu_affinity == வெற்று)
    திரும்பு false;
  திரும்பு (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, வெறுமை *args,
                             முழு *sync_socket_out, கட்டமைப்பு libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) கட்டமைப்பு init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close முழு sync_socket_container = -1;
  எழுத்து *notify_socket_env = வெற்று;
  cleanup_close முழு sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close முழு restore_pidns = -1;
  முழு first_clone_args = 0;
  முழு sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  முழு ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  எனில் (def->வேர்)
    {
      ret = do_notify_socket (container, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  எனில் (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = libcrun_set_oom (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->வேர் ? container->container_def->வேர்->path : வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  க்கு (i = 0; i < init_status.fd_len; i++)
    {
      தேர்வு (init_status.மதிப்பு[i])
        {
        நிலை CLONE_NEWIPC:
          எனில் (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          நிறுத்து;

        நிலை CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          எனில் (! new_userns)
            init_status.must_fork = true;
          இல்லையெனில்
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          நிறுத்து;

        நிலை CLONE_NEWTIME:
          எனில் (! new_userns)
            init_status.must_fork = true;
          இல்லையெனில்
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          நிறுத்து;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  எனில் ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  இல்லையெனில் எனில் ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  எனில் (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  எனில் (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      கட்டமைப்பு _clone3_args clone3_args;
      memset (&clone3_args, 0, அளவு (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      எனில் (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  எனில் (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, வெற்று);
      எனில் (UNLIKELY (pid < 0))
        திரும்பு crun_make_error (err, errno, "clone");
    }

  எனில் (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = close_and_reset (&sync_socket_container);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_மரம்_fd));

      எனில் (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          ret = TEMP_FAILURE_RETRY (பட (sync_socket_host, &new_pid, அளவு (new_pid)));
          எனில் (UNLIKELY (ret != அளவு (new_pid)))
            திரும்பு crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          எனில் (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          இல்லையெனில்
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, வெற்று, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      எனில் (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      எனில் ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      எனில் (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          ret = TEMP_FAILURE_RETRY (பட (sync_socket_host, &grandchild, அளவு (grandchild)));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          எனில் (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          இல்லையெனில்
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, வெற்று, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      எனில் (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      pid_to_clean = 0;
      திரும்பு pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  எனில் (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  எனில் (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  இல்லையெனில்
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      எனில் (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->செய்தி);
    }

  /* Jump into the specified entrypoint.  */
  எனில் (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  எனில் (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->செய்தி);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  எனில் (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

நிலையான முழு
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, முழு sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, மாறிலி எழுத்து *sub_cgroup,
                            முழு *terminal_fd, libcrun_error_t *err)
{
  முழு ret;
  எழுத்து res;
  pid_t pid;
  cleanup_close முழு sync_fd = sync_socket_fd;

  எனில் (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (பட (sync_fd, &res, 1));
  எனில் (UNLIKELY (ret != 1))
    திரும்பு crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  எனில் (res != '0')
    திரும்பு crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (பட (sync_fd, &pid, அளவு (pid)));
  எனில் (UNLIKELY (ret != அளவு (pid)))
    திரும்பு crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, வெற்று, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "waitpid for exec child pid");

  எனில் (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (need_move_to_cgroup)
    {
      cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free எழுத்து *final_cgroup = வெற்று;

      எனில் (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, வெற்று);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_set_io_priority (pid, process, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (லிக (sync_fd, &ret, அளவு (ret)));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "write to sync socket");

  எனில் (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      எனில் (UNLIKELY (ret < 0))
        {
          முழு err_code = 0;
          cleanup_free எழுத்து *err_str = வெற்று;

          எனில் (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              எனில் (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  திரும்பு crun_make_error (err, err_code, "%s", err_str);
                }
            }

          திரும்பு crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  திரும்பு pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
நிலையான முழு
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close முழு pidfd_pid_to_join = -1;
  முழு all_flags = 0;
  size_t i;
  முழு ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  எனில் (def->linux && def->linux->namespaces)
    {
      க்கு (i = 0; i < def->linux->namespaces_len; i++)
        எனில் (! is_empty_string (def->linux->namespaces[i]->path))
          திரும்பு 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  எனில் (UNLIKELY (pidfd_pid_to_join < 0))
    திரும்பு 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  எனில் (ret == 0)
    திரும்பு crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  க்கு (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].மதிப்பு;

  ret = setns (pidfd_pid_to_join, all_flags);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு 0;

  திரும்பு 1;
}

நிலையான முழு
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  முழு fds_joined[MAX_NAMESPACES] = {
    0,
  };
  முழு fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  முழு ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  /* Nothing left to do if the namespaces were joined.  */
  எனில் (LIKELY (ret > 0))
    திரும்பு 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  எனில் (def->linux->namespaces_len >= MAX_NAMESPACES)
    திரும்பு crun_make_error (err, 0, "invalid configuration");

  க்கு (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free எழுத்து *ns_path = வெற்று;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      எனில் (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          எனில் (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              தொடர்;
            }

          ret = fds[i];
          செல் exit;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  எனில் (UNLIKELY (ret < 0))
    செல் exit;
  எனில் (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      செல் exit;
    }

  க்கு (i = 0; namespaces[i].ns_file; i++)
    {
      எனில் (namespaces[i].மதிப்பு == CLONE_NEWUSER)
        தொடர்;

      ret = setns (fds[i], 0);
      எனில் (ret == 0)
        fds_joined[i] = 1;
    }
  க்கு (i = 0; namespaces[i].ns_file; i++)
    {
      எனில் (fds_joined[i])
        தொடர்;
      ret = setns (fds[i], 0);
      எனில் (ret == 0)
        fds_joined[i] = 1;
    }
  க்கு (i = 0; namespaces[i].ns_file; i++)
    {
      எனில் (fds_joined[i])
        தொடர்;
      ret = setns (fds[i], 0);
      எனில் (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          க்கு (j = 0; j < def->linux->namespaces_len; j++)
            {
              எனில் (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  நிறுத்து;
                }
            }
          எனில் (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              தொடர்;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          செல் exit;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

exit:
  க்கு (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  திரும்பு ret;
}

முழு
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      மாறிலி எழுத்து *sub_cgroup,
                      முழு detach,
                      runtime_spec_schema_config_schema_process *process,
                      முழு *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  முழு ret;
  முழு sync_socket_fd[2];
  cleanup_close முழு cgroup_dirfd = -1;
  cleanup_close முழு sync_fd = -1;
  கட்டமைப்பு _clone3_args clone3_args;
  bool need_move_to_cgroup;

  எனில் (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "set child subreaper");
    }

  எனில் (path_has_dot_dot_component (sub_cgroup))
    திரும்பு crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  எனில் (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free எழுத்து *final_cgroup = வெற்று;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *cgroup_status = வெற்று;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    எனில் (get_force_cgroup_v1_annotation (container) == வெற்று)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        எனில் (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, அளவு (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  எனில் (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  இல்லையெனில்
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  எனில் (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  /* On errors, fall back to fork().  */
  எனில் (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      எனில் (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          செல் exit;
        }
    }

  எனில் (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      திரும்பு join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  எனில் (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (லிக (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->செய்தி);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  எனில் (UNLIKELY (pid < 0))
    {
      முழு saved_errno = errno;
      TEMP_FAILURE_RETRY (லிக (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  எனில் (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (லிக (sync_fd, "0", 1));
      எனில் (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (லிக (sync_fd, &pid, அளவு (pid)));
      எனில் (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  இல்லையெனில்
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free எழுத்து *pty = வெற்று;
      முழு r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (பட (sync_fd, &r, அளவு (r)));
      எனில் (UNLIKELY (ret != அளவு (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      எனில் (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      எனில் (terminal_fd)
        {
          cleanup_close முழு ptmx_fd = -1;

          ret = set_id_init (container, err);
          எனில் (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          எனில் (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          எனில் (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  திரும்பு 0;

exit:
  எனில் (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  எனில் (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  திரும்பு ret;
}

முழு
libcrun_linux_container_update (libcrun_container_status_t *status, மாறிலி எழுத்து *state_வேர், runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *cgroup_status = வெற்று;

  cgroup_status = libcrun_cgroup_make_status (status);

  திரும்பு libcrun_update_cgroup_resources (cgroup_status, state_வேர், resources, err);
}

நிலையான முழு
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, மாறிலி bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *cgroup_status = வெற்று;

  cgroup_status = libcrun_cgroup_make_status (status);

  திரும்பு libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

முழு
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  திரும்பு libcrun_container_pause_unpause_linux (status, true, err);
}

முழு
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  திரும்பு libcrun_container_pause_unpause_linux (status, false, err);
}

முழு
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  முழு ret;

  எனில் (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  இல்லையெனில் எனில் (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  இல்லையெனில்
    திரும்பு crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, 0, "personality to `%s`", p->domain);

  திரும்பு 0;
}

முழு
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  முழு ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close முழு sockfd = -1;

  க்கு (i = 0; i < def->linux->namespaces_len; i++)
    {
      முழு மதிப்பு = libcrun_find_namespace (def->linux->namespaces[i]->type);
      எனில் (UNLIKELY (மதிப்பு < 0))
        திரும்பு crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      எனில் (மதிப்பு == CLONE_NEWNET && def->linux->namespaces[i]->path == வெற்று)
        {
          configure_network = true;
          நிறுத்து;
        }
    }

  எனில் (! configure_network)
    திரும்பு 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  எனில் (LIKELY (sockfd >= 0))
    {
      கட்டமைப்பு ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  இல்லையெனில்
    {
      கட்டமைப்பு nlmsghdr *hdr_recv;
      எழுத்து buf[அளவு (கட்டமைப்பு nlmsghdr) + அளவு (கட்டமைப்பு nlmsgerr)];
      கட்டமைப்பு sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      கட்டமைப்பு sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      கட்டமைப்பு nlmsghdr hdr = {
        .nlmsg_len = அளவு (கட்டமைப்பு nlmsghdr) + அளவு (கட்டமைப்பு ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      கட்டமைப்பு ifinfomsg செய்தி = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((கட்டமைப்பு nlmsghdr *) buf) = hdr;
      *((கட்டமைப்பு ifinfomsg *) (buf + அளவு (கட்டமைப்பு nlmsghdr))) = செய்தி;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      எனில் (UNLIKELY (sockfd < 0))
        திரும்பு crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (கட்டமைப்பு sockaddr *) &addr, அளவு (addr));
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, அளவு (buf), 0, (கட்டமைப்பு sockaddr *) &addr_to, அளவு (addr_to));
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, அளவு (buf), 0, வெற்று, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (கட்டமைப்பு nlmsghdr *) buf;

      எனில் (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          கட்டமைப்பு nlmsgerr *err_data = (கட்டமைப்பு nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          எனில் (err_data->error < 0)
            திரும்பு crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  திரும்பு 0;
}

/* Protection for attacks like CVE-2019-5736.  */
முழு ensure_cloned_இருமம் ();
__attribute__ ((constructor)) நிலையான வெறுமை
libcrun_rexec (வெறுமை)
{
  எனில் (ensure_cloned_இருமம் () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

முழு
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  திரும்பு libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

முழு
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  முழு ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
நிலையான முழு
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, முழு சமிக்ஞை, libcrun_error_t *err)
{
  முழு ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  எனில் (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      /* The pid is not valid anymore, return an error.  */
      எனில் (ret == 0)
        {
          errno = ESRCH;
          திரும்பு crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, சமிக்ஞை);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "kill container");
  திரும்பு 0;
}

முழு
libcrun_kill_linux (libcrun_container_status_t *status, முழு சமிக்ஞை, libcrun_error_t *err)
{
  முழு ret;
  cleanup_close முழு pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  எனில் (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      எனில் (errno == ENOSYS)
        திரும்பு libcrun_kill_linux_no_pidfd (status, true, சமிக்ஞை, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      எனில் (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        திரும்பு crun_make_error (err, ESRCH, "process not running");
      திரும்பு crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* The pid is not valid anymore, return an error.  */
  எனில் (ret == 0)
    {
      errno = ESRCH;
      திரும்பு crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, சமிக்ஞை, வெற்று, 0);
  எனில் (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      எனில் (errno == ENOSYS)
        திரும்பு libcrun_kill_linux_no_pidfd (status, false, சமிக்ஞை, err);
      திரும்பு crun_make_error (err, errno, "send signal to pidfd");
    }

  திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *
libcrun_get_intelrdt_name (மாறிலி எழுத்து *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  எனில் (def == வெற்று || def->linux == வெற்று || def->linux->intel_rdt == வெற்று || def->linux->intel_rdt->clos_id == வெற்று)
    {
      எனில் (explicit)
        *explicit = false;
      திரும்பு ctr_name;
    }

  எனில் (explicit)
    *explicit = true;
  திரும்பு def->linux->intel_rdt->clos_id;
}

முழு
libcrun_apply_intelrdt (மாறிலி எழுத்து *ctr_name, libcrun_container_t *container, pid_t pid, முழு actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = வெற்று;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  மாறிலி எழுத்து *name;
  முழு ret;

  எனில் (container)
    def = container->container_def;

  எனில் (def == வெற்று || def->linux == வெற்று || def->linux->intel_rdt == வெற்று)
    திரும்பு 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  எனில் (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      எனில் (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          எனில் (UNLIKELY (ret < 0))
            செல் fail;

          has_monitoring = true;
        }
    }

  எனில் (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      எனில் (UNLIKELY (ret < 0))
        செல் fail;
    }

  எனில் (actions & LIBCRUN_INTELRDT_MOVE)
    {
      மாறிலி எழுத்து *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : வெற்று;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      எனில் (UNLIKELY (ret < 0))
        செல் fail;
    }

  திரும்பு 0;

fail:
  எனில் (has_monitoring)
    {
      libcrun_error_t tmp_err = வெற்று;
      முழு tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      எனில் (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  எனில் (created)
    {
      libcrun_error_t tmp_err = வெற்று;
      முழு tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      எனில் (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  திரும்பு ret;
}

முழு
libcrun_destroy_intelrdt (மாறிலி எழுத்து *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  மாறிலி எழுத்து *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  எனில் (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      முழு ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      எனில் (ret < 0)
        திரும்பு ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  எனில் (explicit)
    திரும்பு 0;

  திரும்பு resctl_destroy (clos_id, err);
}

முழு
libcrun_update_intel_rdt (மாறிலி எழுத்து *ctr_name, libcrun_container_t *container, மாறிலி எழுத்து *l3_cache_schema, மாறிலி எழுத்து *mem_bw_schema, எழுத்து *மாறிலி *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = வெற்று;
  மாறிலி எழுத்து *name;

  எனில் (container)
    def = container->container_def;

  எனில் (def == வெற்று || def->linux == வெற்று || def->linux->intel_rdt == வெற்று)
    திரும்பு 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, வெற்று);

  திரும்பு resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
முழு
libcrun_safe_chdir (மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *buffer = வெற்று;
  முழு ret;

  ret = chdir (path);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  எனில் (ret == 0 || buffer[0] != '/')
    {
      எனில் (chdir ("/") < 0)
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
#define UNREACHABLE_LEN ((முழு) அளவு (UNREACHABLE) - 1)

      எனில் ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        திரும்பு crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      திரும்பு crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  திரும்பு 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
நிலையான முழு
run_callback_in_namespace (முழு mntns_fd, முழு (*callback) (வெறுமை *, libcrun_error_t *), வெறுமை *arg, libcrun_error_t *err)
{
  முழு wait_status = 0;
  pid_t pid;
  முழு ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  எனில் (UNLIKELY (pid < 0))
    திரும்பு crun_make_error (err, errno, "vfork");

  எனில் (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      எனில் (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      எனில் (UNLIKELY (ret < 0))
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
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  திரும்பு get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
நிலையான முழு
check_container_userns (libcrun_container_t *container, முழு userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close முழு expected_fd = -1;
  cleanup_close முழு parent_fd = -1;
  கட்டமைப்பு stat st, expected_st;
  மாறிலி எழுத்து *path = வெற்று;
  size_t i;
  முழு ret;

  எனில் (def->linux)
    க்கு (i = 0; i < def->linux->namespaces_len; i++)
      எனில் (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == வெற்று;
          நிறுத்து;
        }

  எனில் (path == வெற்று)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      எனில் (UNLIKELY (expected_fd < 0))
        திரும்பு expected_fd;
    }
  இல்லையெனில்
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      எனில் (UNLIKELY (expected_fd < 0))
        திரும்பு crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  எனில் (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      எனில் (UNLIKELY (parent_fd < 0))
        திரும்பு crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fstat user namespace");

  எனில் (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    திரும்பு crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  திரும்பு 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
நிலையான முழு
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, முழு *mntns_fd, முழு *owner_fd, libcrun_error_t *err)
{
  cleanup_close முழு mntns = -1;
  cleanup_close முழு owner = -1;
  முழு ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  எனில் (UNLIKELY (mntns < 0))
    திரும்பு mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (ret == 0)
    திரும்பு crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  எனில் (UNLIKELY (owner < 0))
    திரும்பு crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  திரும்பு 0;
}

நிலையான முழு
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, முழு (*callback) (வெறுமை *, libcrun_error_t *), வெறுமை *arg, libcrun_error_t *err)
{
  cleanup_close முழு mntns_fd = -1;
  cleanup_close முழு owner_fd = -1;
  முழு ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு run_callback_in_namespace (mntns_fd, callback, arg, err);
}

கட்டமைப்பு umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
நிலையான முழு
umount_nofollow (மாறிலி எழுத்து *destination, libcrun_error_t *err)
{
  cleanup_close முழு parentfd = -1;
  cleanup_close முழு cwdfd = -1;
  எழுத்து buffer[PATH_MAX];
  மாறிலி எழுத்து *parent, *name;
  எழுத்து *last_slash;
  size_t len;
  முழு saved_errno;
  முழு ret;

  எனில் (is_empty_string (destination))
    திரும்பு crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  எனில் (UNLIKELY (len >= அளவு (buffer)))
    திரும்பு crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  போது (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  எனில் (last_slash == வெற்று)
    {
      parent = ".";
      name = buffer;
    }
  இல்லையெனில் எனில் (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  இல்லையெனில்
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  எனில் (UNLIKELY (parentfd < 0))
    திரும்பு crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  எனில் (UNLIKELY (cwdfd < 0))
    திரும்பு crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  எனில் (UNLIKELY (fchdir (cwdfd) < 0))
    திரும்பு crun_make_error (err, errno, "fchdir to the previous working directory");

  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, saved_errno, "umount `%s`", destination);

  திரும்பு 0;
}

நிலையான முழு
do_umount_in_a_container (வெறுமை *arg, libcrun_error_t *err)
{
  கட்டமைப்பு umount_in_a_container_args *args = arg;
  size_t i;
  முழு ret;

  க்கு (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

கட்டமைப்பு mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  கட்டமைப்பு libcrun_fd_map *fds;
  முழு pidfd;
  முழு owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
நிலையான முழு
open_destination_nofollow (மாறிலி எழுத்து *destination, bool dir, libcrun_error_t *err)
{
  மாறிலி uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  முழு fd;
  முழு ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  எனில் (LIKELY (fd >= 0))
    திரும்பு fd;

  எனில் (errno != ENOENT)
    திரும்பு crun_make_error (err, errno, "open `%s`", destination);

  எனில் (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  இல்லையெனில்
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", destination);

  திரும்பு fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
நிலையான முழு
create_destinations_in_a_container (வெறுமை *arg, libcrun_error_t *err)
{
  கட்டமைப்பு mount_in_a_container_args *args = arg;
  size_t i;
  முழு ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  எனில் (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "setresuid to `0`");
    }
  இல்லையெனில் எனில் (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      திரும்பு crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  க்கு (i = 0; i < args->len; i++)
    {
      cleanup_close முழு dest_fd = -1;
      கட்டமைப்பு stat st;

      ret = fstat (args->fds->fds[i], &st);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      எனில் (UNLIKELY (dest_fd < 0))
        திரும்பு dest_fd;
    }

  திரும்பு 0;
}

நிலையான முழு
do_mount_in_a_container (வெறுமை *arg, libcrun_error_t *err)
{
  கட்டமைப்பு mount_in_a_container_args *args = arg;
  size_t i;
  முழு ret;

  க்கு (i = 0; i < args->len; i++)
    {
      cleanup_close முழு dest_fd = -1;
      கட்டமைப்பு stat st;

      ret = fstat (args->fds->fds[i], &st);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      எனில் (UNLIKELY (dest_fd < 0))
        திரும்பு dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  திரும்பு 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
நிலையான முழு
send_mounts_from_container_namespaces (libcrun_container_t *container, முழு pidfd, runtime_spec_schema_defs_mount **mounts,
                                       கட்டமைப்பு libcrun_fd_map *fds, size_t len, முழு flags, முழு owner_fd, முழு sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  முழு ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  எனில் (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  இல்லையெனில் எனில் (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      திரும்பு crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  எனில் (UNLIKELY (pid < 0))
    திரும்பு crun_make_error (err, errno, "fork");

  எனில் (pid)
    {
      முழு wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      எனில் (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, வெற்று, 0));
          திரும்பு crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  க்கு (i = 0; i < len; i++)
    {
      cleanup_close முழு fd = -1;

      எனில் (fds->fds[i] >= 0)
        தொடர்;

      fd = open_mount_of_type (container, mounts[i], err);
      எனில் (UNLIKELY (fd < 0))
        திரும்பு crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
நிலையான முழு
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, முழு *flags, libcrun_error_t *err)
{
  நிலையான மாறிலி கட்டமைப்பு
  {
    மாறிலி எழுத்து *name;
    முழு flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  க்கு (i = 0; i < அளவு (namespaces) / அளவு (namespaces[0]); i++)
    {
      cleanup_close முழு self_fd = -1;
      cleanup_close முழு target_fd = -1;
      கட்டமைப்பு stat self_st, target_st;
      cleanup_free எழுத்து *self_path = வெற்று;
      முழு ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      எனில் (UNLIKELY (self_fd < 0))
        திரும்பு self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      எனில் (UNLIKELY (target_fd < 0))
        திரும்பு target_fd;

      ret = fstat (self_fd, &self_st);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      எனில் (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  திரும்பு 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
நிலையான முழு
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, முழு owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, கட்டமைப்பு libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close முழு sync_fd = -1;
  cleanup_close முழு pidfd = -1;
  முழு sync_socket_fd[2];
  bool needed = false;
  முழு flags;
  pid_t pid;
  size_t i;
  முழு ret;

  க்கு (i = 0; i < len; i++)
    {
      எனில் (fds->fds[i] >= 0)
        தொடர்;

      எனில் (mounts[i]->type == வெற்று)
        திரும்பு crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  எனில் (! needed)
    திரும்பு 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  எனில் (UNLIKELY (pidfd < 0))
    திரும்பு crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (ret == 0)
    திரும்பு crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  எனில் (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      திரும்பு ret;
    }

  எனில் (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      எனில் (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  க்கு (i = 0; i < len; i++)
    {
      எனில் (fds->fds[i] >= 0)
        தொடர்;

      ret = receive_fd_from_socket (sync_fd, err);
      எனில் (UNLIKELY (ret < 0))
        {
          cleanup_free எழுத்து *err_str = வெற்று;
          முழு err_code = 0;

          எனில் (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          இல்லையெனில்
            ret = crun_error_wrap (err, "receive mount fd");
          நிறுத்து;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, வெற்று, 0));

  திரும்பு ret;
}

நிலையான unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free எழுத்து *data = வெற்று;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  எனில் (mount->options == வெற்று)
    திரும்பு get_default_flags (container, mount->destination, &data);

  க்கு (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  திரும்பு flags;
}

முழு
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  கட்டமைப்பு mount_in_a_container_args args;
  cleanup_close_map கட்டமைப்பு libcrun_fd_map *fds = வெற்று;
  cleanup_close முழு pidfd = -1;
  cleanup_close முழு mntns_fd = -1;
  cleanup_close முழு owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  முழு ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  fds = make_libcrun_fd_map (len);

  க்கு (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), வெற்று, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      எனில் (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          எனில் (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          எனில் (UNLIKELY (fds->fds[i] < 0))
            திரும்பு fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  க்கு (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  எனில் (UNLIKELY (ret > 0))
    திரும்பு *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

முழு
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  கட்டமைப்பு umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  திரும்பு run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

கட்டமைப்பு restrict_mounts_in_a_container_args
{
  கட்டமைப்பு libcrun_mount_flags_s *mounts;
  size_t len;
  முழு rootfd;
};

நிலையான முழு
do_restrict_mounts_in_a_container (வெறுமை *arg, libcrun_error_t *err)
{
  கட்டமைப்பு restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  முழு ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  எனில் (LIKELY (ret == 0))
    ret = chroot (".");
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "chroot to the container root");

  க்கு (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      மாறிலி எழுத்து *target = consume_slashes (args->mounts[i].target);
      cleanup_close முழு fd = -1;
      கட்டமைப்பு statfs sfs;
      unsigned long current_flags;

      எனில் (flags == 0)
        தொடர்;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      எனில் (UNLIKELY (fd < 0))
        {
          எனில் (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              தொடர்;
            }
          திரும்பு fd;
        }

      ret = fstatfs (fd, &sfs);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      எனில் ((current_flags & flags) == flags)
        தொடர்;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      எனில் (LIKELY (ret == 0))
        தொடர்;

      எனில் (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        திரும்பு ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (வெற்று, target, வெற்று, MS_REMOUNT | MS_BIND | flags | current_flags, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "remount `/%s`", target);
    }

  திரும்பு 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
முழு
libcrun_restrict_mounts_in_container (pid_t pid, கட்டமைப்பு libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  கட்டமைப்பு restrict_mounts_in_a_container_args args;
  cleanup_close முழு mntns_fd = -1;
  cleanup_close முழு rootfd = -1;
  எழுத்து path[64];
  முழு ret;

  snprintf (path, அளவு (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  எனில் (UNLIKELY (mntns_fd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, அளவு (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  எனில் (UNLIKELY (rootfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  எனில் (UNLIKELY (ret > 0))
    திரும்பு *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  திரும்பு ret;
}

முழு
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close முழு netns_fd = -1;
  size_t i;
  முழு ret;

  எனில் (def == வெற்று || def->linux == வெற்று || def->linux->net_devices == வெற்று)
    திரும்பு 0;

  cleanup_free எழுத்து *ns_path = வெற்று;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  எனில் (UNLIKELY (netns_fd < 0))
    திரும்பு netns_fd;

  க்கு (i = 0; i < def->linux->net_devices->len; i++)
    {
      மாறிலி எழுத்து *new_name = def->linux->net_devices->மதிப்புகள்[i]->name ?: def->linux->net_devices->சாவிகள்[i];

      ret = move_network_device (def->linux->net_devices->சாவிகள்[i], new_name, netns_fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}
