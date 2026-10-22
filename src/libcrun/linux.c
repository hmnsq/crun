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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "linux.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "status.h"
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sched.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <sys/mount.h>
#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#જો defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#endif

#અન્તર્ભાવ <sys/prctl.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#અન્તર્ભાવ <sys/time.h>
#અન્તર્ભાવ <sys/resource.h>
#અન્તર્ભાવ <sys/sysmacros.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <grp.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ "terminal.h"
#અન્તર્ભાવ "cgroup.h"
#અન્તર્ભાવ "cgroup-utils.h"
#અન્તર્ભાવ "status.h"
#અન્તર્ભાવ "criu.h"
#અન્તર્ભાવ "scheduler.h"
#અન્તર્ભાવ "intelrdt.h"
#અન્તર્ભાવ "io_priority.h"
#અન્તર્ભાવ "net_device.h"

#અન્તર્ભાવ <sys/socket.h>
#અન્તર્ભાવ <libgen.h>
#અન્તર્ભાવ <sys/wait.h>
#અન્તર્ભાવ <sys/vfs.h>
#અન્તર્ભાવ <sys/statvfs.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ <sys/personality.h>
#અન્તર્ભાવ <net/if.h>
#અન્તર્ભાવ <sys/xattr.h>
#અન્તર્ભાવ <linux/netlink.h>
#અન્તર્ભાવ <linux/rtnetlink.h>
#અન્તર્ભાવ <sched.h>
#અન્તર્ભાવ <linux/sched.h>
#અન્તર્ભાવ <linux/magic.h>
#અન્તર્ભાવ <linux/nsfs.h>

#અન્તર્ભાવ <json-c/json.h>
#અન્તર્ભાવ <ocispec/json_common.h>

#અન્તર્ભાવ "mount_flags.h"
#અન્તર્ભાવ "syscalls.h"

#ifndef RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#endif

#ifndef OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#endif

#ifndef OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#endif

#ifndef OPEN_વૃક્ષ_NAMESPACE
#  define OPEN_વૃક્ષ_NAMESPACE 2
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
સ્થિર unsigned long
mount_flags_from_statfs (unsigned long f_flags)
{
  unsigned long flags = 0;

  જો (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  જો (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  જો (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  જો (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  જો (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  જો (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  જો (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  જો (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  જો (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  પાછા flags;
}

સંરચના remount_s
{
  સંરચના remount_s *next;
  અક્ષર *target;
  પૂર્ણાંક targetfd;
  unsigned long flags;
  અક્ષર *data;
};

સંરચના private_data_s
{
  સંરચના remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  પૂર્ણાંક unshare_flags;
  પૂર્ણાંક unshare_cgroupns;

  અક્ષર *host_notify_socket_path;
  અક્ષર *container_notify_socket_path;
  અક્ષર *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  પૂર્ણાંક procfd;

  અચલ અક્ષર *rootfs;
  પૂર્ણાંક rootfsfd;

  પૂર્ણાંક notify_socket_વૃક્ષ_fd;
  પૂર્ણાંક old_મૂળ_fd;

  સંરચના libcrun_fd_map *mount_fds;
  સંરચના libcrun_fd_map *dev_fds;
  સંરચના libcrun_fd_map *needed_devs_fds;

  અક્ષર **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  અક્ષર *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  પૂર્ણાંક maskdir_fd;
  અક્ષર *maskdir_proc_path;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_pivot;
  bool no_pivot;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_મૂળ_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  અક્ષર *host_rootfs;
};

સંરચના linux_namespace_s
{
  અચલ અક્ષર *name;
  અચલ અક્ષર *ns_file;
  પૂર્ણાંક કિંમત;
};

સ્થિર શૂન્ય
cleanup_private_data (શૂન્ય *private_data)
{
  સંરચના private_data_s *p = private_data;

  જો (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  જો (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  જો (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  જો (p->old_મૂળ_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_મૂળ_fd));
  જો (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  જો (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  જો (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  જો (p->copy_symlink_targets)
    {
      size_t i;
      માટે (i = 0; i < p->n_copy_symlink_targets; i++)
        મુક્ત (p->copy_symlink_targets[i]);
      મુક્ત (p->copy_symlink_targets);
    }
  મુક્ત (p->unified_cgroup_path);
  મુક્ત (p->host_notify_socket_path);
  મુક્ત (p->container_notify_socket_path);
  મુક્ત (p->external_descriptors);
  મુક્ત (p->maskdir_proc_path);
  મુક્ત (p->host_rootfs);
  મુક્ત (p);
}

સ્થિર સંરચના private_data_s *
get_private_data (સંરચના libcrun_container_s *container)
{
  જો (container->private_data == ખાલી)
    {
      સંરચના private_data_s *p = xmalloc0 (કદ (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_વૃક્ષ_fd = -1;
      p->old_મૂળ_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  પાછા container->private_data;
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

સ્થિર સંરચના linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { ખાલી, ખાલી, 0 } };

સ્થિર પૂર્ણાંક
get_and_reset (પૂર્ણાંક *old)
{
  પૂર્ણાંક tmp = *old;
  *old = -1;
  પાછા tmp;
}

પૂર્ણાંક
libcrun_find_namespace (અચલ અક્ષર *name)
{
  સંરચના linux_namespace_s *it;
  માટે (it = namespaces; it->name; it++)
    જો (strcmp (it->name, name) == 0)
      પાછા it->કિંમત;
  પાછા -1;
}

સ્થિર પૂર્ણાંક
syscall_keyctl_join (અચલ અક્ષર *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  પાછા (પૂર્ણાંક) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

સ્થિર પૂર્ણાંક
syscall_pidfd_open (pid_t pid, unsigned પૂર્ણાંક flags)
{
#જો defined __NR_pidfd_open
  પાછા (પૂર્ણાંક) syscall (__NR_pidfd_open, pid, flags);
#નહીંતર
  (શૂન્ય) pid;
  (શૂન્ય) flags;
  errno = ENOSYS;
  પાછા -1;
#endif
}

સ્થિર પૂર્ણાંક
syscall_pidfd_send_signal (પૂર્ણાંક pidfd, પૂર્ણાંક sig, siginfo_t *info, unsigned પૂર્ણાંક flags)
{
#જો defined __NR_pidfd_send_signal
  પાછા (પૂર્ણાંક) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#નહીંતર
  (શૂન્ય) pidfd;
  (શૂન્ય) sig;
  (શૂન્ય) info;
  (શૂન્ય) flags;
  errno = ENOSYS;
  પાછા -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
સ્થિર uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  જો (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  જો (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  જો (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  જો (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  જો (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  જો (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  જો (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  જો (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  પાછા attr;
}

સ્થિર પૂર્ણાંક
do_mount_setattr (bool recursive, અચલ અક્ષર *target, પૂર્ણાંક targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  સંરચના mount_attr_s attr = {
    0,
  };
  પૂર્ણાંક ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  જો (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      જો (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      નહીંતર જો (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      નહીંતર
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      જો (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  નહીંતર જો (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "mount_setattr `/%s`", target);

  પાછા 0;
}

પૂર્ણાંક
get_bind_mount (પૂર્ણાંક dirfd, અચલ અક્ષર *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક open_વૃક્ષ_fd = -1;
  સંરચના mount_attr_s attr = {
    0,
  };
  પૂર્ણાંક recursive_flag = (recursive ? AT_RECURSIVE : 0);
  પૂર્ણાંક ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  જો (UNLIKELY (src == ખાલી))
    પાછા crun_make_error (err, EINVAL, "mount source is not specified");

  જો (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  જો (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_વૃક્ષ_fd = syscall_open_વૃક્ષ (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  જો (UNLIKELY (open_વૃક્ષ_fd < 0))
    પાછા crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_વૃક્ષ_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "mount_setattr `%s`", src);

  પાછા get_and_reset (&open_વૃક્ષ_fd);
}

પૂર્ણાંક
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, અક્ષર *option, અક્ષર **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free અક્ષર *mappings = ખાલી;
  અચલ અક્ષર *it;
  પૂર્ણાંક ret;

  mappings = xmalloc (allocated);

  માટે (it = option; *it;)
    {
      bool relative = false;
      long કિંમત[3];
      size_t i;

      જો (*it == '\0')
        તોડો;

      જો (*it == '#')
        it++;

      જો (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      માટે (i = 0; i < 3; i++)
        {
          અક્ષર *endptr = ખાલી;

          જો (i > 0 && *it == '-')
            it++;

          જો (*it == '\0')
            પાછા crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          કિંમત[i] = strtol (it, &endptr, 10);
          જો (errno || endptr == it)
            પાછા crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      જો (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          જો (def == ખાલી
              || def->linux == ખાલી
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            પાછા crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          માટે (i = 0; i < mappings_len; i++)
            જો (કિંમત[1] >= (int64_t) mappings[i]->container_id
                && કિંમત[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              તોડો;

          જો (i == mappings_len)
            પાછા crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          કિંમત[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      જો (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", કિંમત[0], કિંમત[1], કિંમત[2]);
      જો (UNLIKELY (ret >= (પૂર્ણાંક) (allocated - written)))
        પાછા crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = ખાલી;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
format_mount_mappings (અક્ષર **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  અચલ size_t max_len_mapping = 64;
  cleanup_free અક્ષર *ret = ખાલી;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  માટે (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      જો (UNLIKELY (len >= max_len_mapping))
        પાછા crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = ખાલી;
  પાછા 0;
}

સ્થિર પૂર્ણાંક
format_mount_mapping (અક્ષર **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t size, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .size = size,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    ખાલી,
  };

  પાછા format_mount_mappings (ret, mappings, 1, written, err);
}

સ્થિર bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  જો (def->linux == ખાલી)
    પાછા mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  જો (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    પાછા false;

  જો (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    પાછા false;

  માટે (s = 0; s < mnt->uid_mappings_len; s++)
    {
      જો (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        પાછા false;
      જો (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        પાછા false;
      જો (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        પાછા false;
    }

  માટે (s = 0; s < mnt->gid_mappings_len; s++)
    {
      જો (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        પાછા false;
      જો (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        પાછા false;
      જો (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        પાછા false;
    }

  પાછા true;
}

સ્થિર pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        અચલ અક્ષર *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != ખાલી;

  જો (! need_new_userns)
    પાછા 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, ખાલી);
  જો (UNLIKELY (pid < 0))
    પાછા crun_make_error (err, errno, "clone");

  જો (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      જ્યારે (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  જો (mnt->uid_mappings_len)
    {
      cleanup_free અક્ષર *uid_map = ખાલી;
      cleanup_free અક્ષર *gid_map = ખાલી;
      size_t written = 0;
      પૂર્ણાંક ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      cleanup_close પૂર્ણાંક fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      જો (UNLIKELY (fd < 0))
        પાછા fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      જો (UNLIKELY (fd < 0))
        પાછા fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      cleanup_free અક્ષર *dup_options = ખાલી;
      અક્ષર *option, *saveptr = ખાલી;

      જો (! options)
        પાછા crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      માટે (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (ખાલી, ";", &saveptr))
        {
          cleanup_free અક્ષર *mappings = ખાલી;
          cleanup_close પૂર્ણાંક map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          પૂર્ણાંક ret;

          જો (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              જો (UNLIKELY (map_fd < 0))
                પાછા map_fd;
            }
          નહીંતર જો (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              જો (UNLIKELY (map_fd < 0))
                પાછા map_fd;
            }
          નહીંતર
            પાછા crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  પાછા 0;
}

પૂર્ણાંક
libcrun_create_keyring (libcrun_container_t *container, અચલ અક્ષર *name, અચલ અક્ષર *label, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક labelfd = -1;
  bool label_set = false;
  પૂર્ણાંક ret;

  જો (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      જો (UNLIKELY (labelfd < 0))
        {
          જો (crun_error_get_errno (err) != ENOENT)
            પાછા labelfd;

          crun_error_release (err);
        }

      જો (labelfd >= 0)
        {
          ret = લિખ (labelfd, label, strlen (label));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          જાઓ out;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      જાઓ out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  જો (label_set && લિખ (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  પાછા ret;
}

સ્થિર શૂન્ય
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  જો (def->process && def->process->user)
    {
      જો (def->process->user->uid)
        *uid = def->process->user->uid;
      જો (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

સ્થિર unsigned long
get_mount_flags (અચલ અક્ષર *name, પૂર્ણાંક current_flags, પૂર્ણાંક *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  અચલ સંરચના propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  જો (found)
    *found = prop ? 1 : 0;

  જો (prop == ખાલી)
    પાછા 0;

  જો (prop->extra_flags & OPTION_RECURSIVE)
    {
      જો (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      જો (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  જો (extra_flags)
    *extra_flags |= prop->extra_flags;

  જો (prop->clear)
    પાછા current_flags & ~prop->flags;

  પાછા current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
સ્થિર bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  માટે (i = 0; i < mount->options_len; i++)
    {
      અચલ સંરચના propagation_flags_s *prop;

      જો (mount->options[i] == ખાલી)
        ચાલુ;

      prop = libcrun_str2mount_flags (mount->options[i]);
      જો (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        પાછા true;
    }

  પાછા false;
}

સ્થિર unsigned long
get_mount_flags_or_option (અચલ અક્ષર *name, પૂર્ણાંક current_flags, unsigned long *extra_flags, અક્ષર **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  પૂર્ણાંક found;
  __attribute__ ((unused)) cleanup_free અક્ષર *prev = ખાલી;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  જો (name == ખાલી)
    પાછા 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  જો (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      અચલ અક્ષર *data_flag = ખાલી;
      જો ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      નહીંતર જો ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      જો (data_flag)
        {
          prev = *option;
          જો (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          નહીંતર
            *option = xstrdup (data_flag);
        }
      પાછા flags;
    }

  prev = *option;
  જો (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  નહીંતર
    *option = xstrdup (name);

  પાછા 0;
}

પૂર્ણાંક
pivot_મૂળ (અચલ અક્ષર *new_મૂળ, અચલ અક્ષર *put_old)
{
  પાછા syscall (__NR_pivot_root, new_મૂળ, put_old);
}

સ્થિર શૂન્ય
free_remount (સંરચના remount_s *r)
{
  જો (r == ખાલી)
    પાછા;
  જો (r->targetfd >= 0)
    close (r->targetfd);
  મુક્ત (r->data);
  મુક્ત (r->target);
  મુક્ત (r);
}

સ્થિર સંરચના remount_s *
make_remount (પૂર્ણાંક targetfd, અચલ અક્ષર *target, unsigned long flags, અચલ અક્ષર *data, સંરચના remount_s *next)
{
  સંરચના remount_s *ret = xmalloc (કદ (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : ખાલી;
  ret->next = next;
  ret->targetfd = targetfd;
  પાછા ret;
}

સ્થિર પૂર્ણાંક
do_remount (પૂર્ણાંક targetfd, અચલ અક્ષર *target, unsigned long flags, અચલ અક્ષર *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  પૂર્ણાંક ret;
  અચલ અક્ષર *real_target = target;

  જો (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      જો (LIKELY (ret == 0))
        પાછા 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  જો (flags & (MS_REMOUNT | MS_RDONLY))
    data = ખાલી;

  ret = mount (ખાલી, real_target, ખાલી, flags, data);
  જો (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      સંરચના statfs sfs;

      ret = statfs (real_target, &sfs);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      જો (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      જો ((flags | remount_flags) != flags)
        {
          ret = mount (ખાલી, real_target, ખાલી, flags | remount_flags, data);
          જો (LIKELY (ret == 0))
            પાછા 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          જો (current_flags & MS_RDONLY)
            ret = mount (ખાલી, real_target, ખાલી, flags | remount_flags | MS_RDONLY, data);
        }
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "remount `%s`", target);
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  પૂર્ણાંક ret = 0;
  સંરચના remount_s *r = get_private_data (container)->remounts;
  જ્યારે (r)
    {
      સંરચના remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      જો (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          જો (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              ચાલુ;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      જો (UNLIKELY (ret < 0))
        જાઓ cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  જ્યારે (r)
    {
      સંરચના remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = ખાલી;
  પાછા ret;
}

સ્થિર પૂર્ણાંક
open_mount_target (libcrun_container_t *container, અચલ અક્ષર *target_rel, libcrun_error_t *err)
{
  અચલ અક્ષર *rootfs = get_private_data (container)->rootfs;
  પૂર્ણાંક rootfsfd = get_private_data (container)->rootfsfd;

  જો (rootfsfd < 0)
    પાછા crun_make_error (err, 0, "invalid rootfs state");

  પાછા safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
સ્થિર પૂર્ણાંક
fsopen_mount (અચલ અક્ષર *type, અચલ અક્ષર *source_name, અચલ અક્ષર *labeltype, અચલ અક્ષર *label, અચલ અક્ષર *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close પૂર્ણાંક fsfd = -1;
  પૂર્ણાંક ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  જો (UNLIKELY (fsfd < 0))
    પાછા fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (શૂન્ય) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  જો (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (data && data[0])
    {
      cleanup_free અક્ષર *data_copy = xstrdup (data);
      અક્ષર *saveptr = ખાલી;
      અક્ષર *token;

      માટે (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (ખાલી, ",", &saveptr))
        {
          અક્ષર *eq = strchr (token, '=');
          જો (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          નહીંતર
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, ખાલી, 0);

          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ખાલી, ખાલી, 0);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#નહીંતર
  (શૂન્ય) type;
  (શૂન્ય) source_name;
  (શૂન્ય) data;
  errno = ENOSYS;
  પાછા -1;
#endif
}

સ્થિર પૂર્ણાંક
get_procfd (સંરચના private_data_s *data, libcrun_error_t *err)
{
  પૂર્ણાંક fd;

  જો (data->procfd >= 0)
    પાછા data->procfd;

  fd = fsopen_mount ("proc", ખાલી, ખાલી, ખાલી, ખાલી);
  જો (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  પાછા fd;
}

સ્થિર પૂર્ણાંક
get_old_મૂળ_fd (સંરચના private_data_s *data)
{
  જો (data->old_મૂળ_fd >= 0)
    પાછા data->old_મૂળ_fd;

  data->old_મૂળ_fd = open ("/", O_PATH | O_CLOEXEC);
  પાછા data->old_મૂળ_fd;
}

સ્થિર પૂર્ણાંક
fs_move_mount_to (પૂર્ણાંક fd, પૂર્ણાંક dirfd, અચલ અક્ષર *name)
{
#ifdef HAVE_NEW_MOUNT_API
  જો (name)
    પાછા syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  પાછા syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#નહીંતર
  (શૂન્ય) fd;
  (શૂન્ય) dirfd;
  (શૂન્ય) name;
  (શૂન્ય) syscall_move_mount;
  errno = ENOSYS;
  પાછા -1;
#endif
}

ગણના
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

સ્થિર પૂર્ણાંક do_mount (libcrun_container_t *container, અચલ અક્ષર *source, પૂર્ણાંક targetfd,
                     અચલ અક્ષર *target, અચલ અક્ષર *fstype, unsigned long mountflags,
                     અચલ શૂન્ય *data, પૂર્ણાંક extra_flags, libcrun_error_t *err);

સ્થિર bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, અચલ અક્ષર *option)
{
  size_t i;
  માટે (i = 0; i < mnt->options_len; i++)
    જો (mnt->options[i] != ખાલી && strcmp (mnt->options[i], option) == 0)
      પાછા true;
  પાછા false;
}

સ્થિર runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, અચલ અક્ષર *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  માટે (i = 0; i < def->mounts_len; i++)
    {
      જો (strcmp (def->mounts[i]->destination, destination) == 0)
        પાછા def->mounts[i];
    }
  પાછા ખાલી;
}

સ્થિર શૂન્ય
warn_tmpfs_fallback_once (સંરચના private_data_s *private_data, અચલ અક્ષર *reason)
{
  જો (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
સ્થિર પૂર્ણાંક
get_shared_empty_dir_cached (libcrun_container_t *container, અક્ષર **proc_fd_path, libcrun_error_t *err)
{
  સંરચના private_data_s *private_data = get_private_data (container);
  cleanup_close પૂર્ણાંક fd = -1;
  cleanup_free અક્ષર *empty_dir_path = ખાલી;
  proc_fd_path_t fd_path;
  પૂર્ણાંક ret;

  /* Fast path: return cached proc fd path if already set up */
  જો (private_data->maskdir_proc_path != ખાલી)
    {
      જો (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      પાછા 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_મૂળ, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  જો (fd < 0)
    પાછા fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  જો (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  પાછા 0;
}

સ્થિર bool
is_dev_null (અચલ સંરચના stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  પાછા S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
સ્થિર પૂર્ણાંક
ensure_dev_null (પૂર્ણાંક fd, libcrun_error_t *err)
{
  સંરચના stat st;

  જો (UNLIKELY (fstat (fd, &st) < 0))
    પાછા crun_make_error (err, errno, "stat `/dev/null`");

  જો (UNLIKELY (! is_dev_null (&st)))
    પાછા crun_make_error (err, 0, "`/dev/null` is not the null device");

  પાછા 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
સ્થિર bool
is_empty_ro_tmpfs (પૂર્ણાંક fd)
{
  સંરચના statfs sfs;

  જો (fstatfs (fd, &sfs) < 0)
    પાછા false;

  પાછા sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

સ્થિર પૂર્ણાંક
mount_masked_dir (libcrun_container_t *container, પૂર્ણાંક pathfd, અચલ અક્ષર *rel_path, libcrun_error_t *err)
{
  સંરચના private_data_s *private_data = get_private_data (container);
  cleanup_close પૂર્ણાંક newfd = -1;
  libcrun_error_t tmp_err = ખાલી;
  પૂર્ણાંક ret;

  જો (private_data->maskdir_fd < 0)
    {
      જો (private_data->maskdir_bind_failed)
        જાઓ fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, ખાલી, &tmp_err);
      જો (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->સંદેશ);
          crun_error_release (&tmp_err);
          જાઓ fallback_to_tmpfs;
        }
    }

  જો (private_data->maskdir_fd >= 0)
    {
      પૂર્ણાંક rootfsfd = private_data->rootfsfd;
      સંરચના stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, ખાલી);
      જો (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          જો (UNLIKELY (newfd < 0))
            પાછા newfd;

          ret = fstat (newfd, &after);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          જો (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            પાછા crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          પાછા 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_path);

      TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
      private_data->maskdir_fd = -1;
      private_data->maskdir_move_failed = true;
    }

  private_data->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_data, tmp_err ? tmp_err->સંદેશ : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  જો (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      જો (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      નહીંતર
        {
          જો (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
do_masked_or_readonly_path (libcrun_container_t *container, અચલ અક્ષર *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  અચલ અક્ષર *rootfs = get_private_data (container)->rootfs;
  cleanup_close પૂર્ણાંક pathfd = -1;
  સંરચના statfs sfs;
  પૂર્ણાંક ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  જો (UNLIKELY (rel_path == ખાલી))
    પાછા crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  જો (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  જો (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      જો (errno != ENOENT && errno != EACCES)
        પાછા pathfd;

      જો (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      પાછા 0;
    }

  જો (readonly)
    {
      પૂર્ણાંક procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close પૂર્ણાંક mountfd = -1;

      જો (UNLIKELY (procfd < 0))
        પાછા procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      જો (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          જો (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, ખાલી);
          નહીંતર
            ret = -1;

          જો (ret < 0)
            crun_error_release (err);
        }

      જો (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          જો (keep_flags)
            {
              cleanup_close પૂર્ણાંક sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              જો (UNLIKELY (sfd < 0))
                પાછા crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              જો (UNLIKELY (ret < 0))
                પાછા crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          જો (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              જો (UNLIKELY (ret < 0))
                પાછા ret;

              ret = fs_move_mount_to (mountfd, pathfd, ખાલી);
              જો (UNLIKELY (ret < 0))
                પાછા crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          નહીંતર
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, ખાલી, mount_flags, ખાલી,
                              LABEL_NONE, err);
              જો (UNLIKELY (ret < 0))
                પાછા ret;
            }
        }
    }
  નહીંતર
    {
      ret = get_file_type_fd (pathfd, &mode);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      જો ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      નહીંતર
        {
          cleanup_close પૂર્ણાંક nullfd = -1;
          cleanup_close પૂર્ણાંક mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          જો (UNLIKELY (nullfd < 0))
            પાછા crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          જો (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, ખાલી);

          જો (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, ખાલી, MS_BIND | MS_RDONLY, ખાલી,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા 0;
}

સ્થિર અચલ અક્ષર *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  અચલ અક્ષર *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  જો (context_type)
    {
      જો (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          પાછા ખાલી;
        }
      પાછા context_type;
    }

  પાછા "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
સ્થિર પૂર્ણાંક
diagnose_mount_failure (પૂર્ણાંક ret, libcrun_error_t *err, libcrun_container_t *container,
                        અચલ અક્ષર *fstype)
{
  જો (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      જો (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          પાછા ret;
        }
    }
  પાછા ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
સ્થિર પૂર્ણાંક
sysfs_userns_fallback (libcrun_container_t *container, અચલ અક્ષર *target,
                       અચલ અક્ષર *real_target, પૂર્ણાંક targetfd,
                       unsigned long mountflags, અચલ અક્ષર *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close પૂર્ણાંક mountfd = -1;
  cleanup_close પૂર્ણાંક sysfd = -1;
  પૂર્ણાંક sys_old_મૂળ_fd = -1;
  પૂર્ણાંક ret;

  જો (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    પાછા 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (ret == 0)
    પાછા 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_મૂળ_fd = get_old_મૂળ_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  જો (sys_old_મૂળ_fd >= 0)
    mountfd = get_bind_mount (sys_old_મૂળ_fd, "sys", true, true, false, MS_PRIVATE, err);
  જો (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  જો (cgroup_mount == ખાલી)
    {
      જો (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, ખાલી);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      નહીંતર
        {
          crun_error_release (err);
          જો (sys_old_મૂળ_fd >= 0)
            {
              cleanup_free અક્ષર *sys_path = ખાલી;
              proc_fd_path_t sys_મૂળ_path;

              get_proc_self_fd_path (sys_મૂળ_path, sys_old_મૂળ_fd);
              xasprintf (&sys_path, "%s/sys", sys_મૂળ_path);
              ret = mount (sys_path, real_target, ખાલી, MS_BIND | MS_REC, ખાલી);
            }
          નહીંતર
            ret = mount ("/sys", real_target, ખાલી, MS_BIND | MS_REC, ખાલી);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      જો (UNLIKELY (sysfd < 0))
        પાછા sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        ખાલી, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      પાછા do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  જો (UNLIKELY (mountfd < 0))
    પાછા mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  જો (UNLIKELY (sysfd < 0))
    પાછા sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
સ્થિર શૂન્ય
apply_selinux_xattr (libcrun_container_t *container, પૂર્ણાંક fd, પૂર્ણાંક extra_flags, અચલ અક્ષર *label)
{
  libcrun_error_t tmp_err = ખાલી;
  પૂર્ણાંક procfd;

  જો ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    પાછા;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  જો (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close પૂર્ણાંક xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      જો (xfd >= 0)
        (શૂન્ય) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
સ્થિર પૂર્ણાંક
do_mount_replace_rootfs (libcrun_container_t *container, પૂર્ણાંક *fd, libcrun_error_t *err)
{
  પૂર્ણાંક procfd = get_procfd (get_private_data (container), err);
  પૂર્ણાંક tmp;
  પૂર્ણાંક ret;

  જો (UNLIKELY (procfd < 0))
    પાછા procfd;

  {
    cleanup_close પૂર્ણાંક nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    જો (UNLIKELY (nsfd < 0))
      પાછા crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    જો (UNLIKELY (ret < 0))
      પાછા crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  જો (UNLIKELY (*fd < 0))
    પાછા crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  જો (UNLIKELY (tmp < 0))
    પાછા crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  પાછા 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
સ્થિર પૂર્ણાંક
apply_propagation_flags (અચલ અક્ષર *target, પૂર્ણાંક targetfd, અચલ અક્ષર *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  પૂર્ણાંક ret;

  જો (targetfd >= 0)
    {
      libcrun_error_t tmp_err = ખાલી;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      જો (LIKELY (ret == 0))
        propagation_done = true;
      નહીંતર
        crun_error_release (&tmp_err);
    }

  જો (! propagation_done)
    {
      ret = mount (ખાલી, real_target, ખાલી, mountflags & ALL_PROPAGATIONS, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  પાછા 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
સ્થિર પૂર્ણાંક
schedule_or_do_remount (libcrun_container_t *container, પૂર્ણાંક *fd, પૂર્ણાંક targetfd,
                        અચલ અક્ષર *target, અચલ અક્ષર *real_target,
                        bool single_instance, unsigned long mountflags,
                        અચલ શૂન્ય *data, પૂર્ણાંક extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  સંરચના remount_s *r;

  જો ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    પાછા do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  જો (*fd < 0)
    {
      *fd = dup (targetfd);
      જો (UNLIKELY (*fd < 0))
        પાછા crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
do_mount (libcrun_container_t *container, અચલ અક્ષર *source, પૂર્ણાંક targetfd,
          અચલ અક્ષર *target, અચલ અક્ષર *fstype, unsigned long mountflags, અચલ શૂન્ય *data,
          પૂર્ણાંક extra_flags, libcrun_error_t *err)
{
  cleanup_free અક્ષર *data_with_label = ખાલી;
  અચલ અક્ષર *data_without_label = data;
  proc_fd_path_t target_buffer;
  અચલ અક્ષર *context_type = ખાલી;
  cleanup_close પૂર્ણાંક ms_move_fd = -1;
  અચલ અક્ષર *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close પૂર્ણાંક fd = -1;
  અચલ અક્ષર *label = ખાલી;
  પૂર્ણાંક ret = 0;

  જો (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  નહીંતર
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  જો (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  જો ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      જો (UNLIKELY (context_type == ખાલી))
        પાછા -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      જો (ret < 0)
        પાછા ret;
      data = data_with_label;
    }

  જો (mountflags & MS_MOVE)
    {
      જો ((mountflags & MS_BIND) || fstype)
        પાછા crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, ખાલી, MS_MOVE, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      જો (UNLIKELY (ms_move_fd < 0))
        પાછા ms_move_fd;
      targetfd = ms_move_fd;
    }

  જો ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      જો (targetfd >= 0)
        {
          જો (mountflags & MS_BIND)
            {
              cleanup_close પૂર્ણાંક bindfd = -1;
              libcrun_error_t tmp_err = ખાલી;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              જો (bindfd < 0)
                {
                  પૂર્ણાંક old_મૂળ_fd = get_old_મૂળ_fd (get_private_data (container));

                  જો (old_મૂળ_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_મૂળ_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              જો (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, ખાલી);
              જો (ret < 0)
                crun_error_release (&tmp_err);
            }
          નહીંતર
            {
              cleanup_close પૂર્ણાંક newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              જો (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, ખાલી);
            }
        }

      જો (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          જો (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              પૂર્ણાંક old_મૂળ_fd = get_old_મૂળ_fd (get_private_data (container));

              જો (old_મૂળ_fd >= 0)
                {
                  cleanup_free અક્ષર *proc_source_buf = ખાલી;
                  proc_fd_path_t old_મૂળ_path;

                  get_proc_self_fd_path (old_મૂળ_path, old_મૂળ_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_મૂળ_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      જો (UNLIKELY (ret < 0))
        {
          પૂર્ણાંક saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          જો (ret == 0)
            પાછા 0;
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          પાછા diagnose_mount_failure (ret, err, container, fstype);
        }

      જો (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          જો (UNLIKELY (fd < 0))
            પાછા fd;

          /* We are replacing the rootfs, reopen it.  */
          જો (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              જો (UNLIKELY (ret < 0))
                પાછા ret;
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

  જો (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  જો (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  જો (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા ret;
}

સ્થિર શૂન્ય
try_umount (પૂર્ણાંક targetfd, અચલ અક્ષર *target)
{
  proc_fd_path_t target_buffer;
  અચલ અક્ષર *real_target = target;

  જો (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

સ્થિર bool
container_has_cgroupns (libcrun_container_t *container)
{
  પાછા get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

સ્થિર પૂર્ણાંક
do_mount_cgroup_v2 (libcrun_container_t *container, પૂર્ણાંક targetfd, અચલ અક્ષર *target,
                    unsigned long mountflags, અચલ અક્ષર *data, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  પૂર્ણાંક cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  જો (UNLIKELY (ret < 0))
    {
      પૂર્ણાંક saved_errno = crun_error_get_errno (err);
      જો (saved_errno == EPERM || saved_errno == EBUSY)
        {
          અચલ અક્ષર *unified_cgroup_path;
          અચલ અક્ષર *src_cgroup;

          crun_error_release (err);

          જો (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              જો (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  જો (LIKELY (ret == 0))
                    પાછા ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_મૂળ;
          પાછા do_mount (container, src_cgroup, targetfd, target, ખાલી,
                           MS_BIND | mountflags, ખાલી, LABEL_NONE, err);
        }

      પાછા ret;
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, અચલ અક્ષર *source, પૂર્ણાંક targetfd, અચલ અક્ષર *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_close પૂર્ણાંક fd = -1;
  અચલ અક્ષર *subsystem = "systemd";
  cleanup_free અક્ષર *subsystem_path = ખાલી;
  cleanup_close પૂર્ણાંક tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  જો (UNLIKELY (tmpfsdirfd < 0))
    પાછા tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "openat `%s`", subsystem_path);

  પાછા do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

સ્થિર પૂર્ણાંક
do_mount_cgroup_v1 (libcrun_container_t *container, અચલ અક્ષર *source, પૂર્ણાંક targetfd, અચલ અક્ષર *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_free અક્ષર *content = ખાલી;
  અક્ષર *from;
  cleanup_close પૂર્ણાંક tmpfsdirfd = -1;
  અક્ષર *saveptr = ખાલી;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  જો (UNLIKELY (tmpfsdirfd < 0))
    પાછા tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    પૂર્ણાંક procfd = get_procfd (get_private_data (container), err);
    જો (UNLIKELY (procfd < 0))
      પાછા procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, ખાલી, err);
  }
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (UNLIKELY (content == ખાલી || content[0] == '\0'))
    પાછા crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  માટે (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ખાલી, "\n", &saveptr))
    {
      cleanup_free અક્ષર *source_path = ખાલી;
      cleanup_free અક્ષર *source_subsystem = ખાલી;
      cleanup_free અક્ષર *subsystem_path = ખાલી;
      અક્ષર *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close પૂર્ણાંક subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      જો (subsystem[0] == '\0')
        ચાલુ;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      જો (it)
        subsystem = it + 5;

      જો (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      જો (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_મૂળ, subsystem, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      /* if there is already a mount specified, do not add a default one.  */
      જો (find_mount_for (container, source_subsystem) != ખાલી)
        ચાલુ;

      ret = append_paths (&source_path, err, source_subsystem, subpath, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      જો (UNLIKELY (subsystemfd < 0))
        પાછા crun_make_error (err, errno, "open `%s`", subsystem_path);

      જો (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          જો (UNLIKELY (ret < 0))
            {
              જો (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  ચાલુ;
                }
              પાછા ret;
            }
        }
      નહીંતર
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, ખાલી, MS_BIND | mountflags, ખાલી,
                          LABEL_NONE, err);
          જો (UNLIKELY (ret < 0))
            {
              જો (crun_error_get_errno (err) != ENOENT)
                પાછા ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, ખાલી, MS_BIND | mountflags,
                              ખાલી, LABEL_NONE, err);
              જો (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  જો (crun_error_get_errno (err) != ENOENT)
                    પાછા ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
do_mount_cgroup (libcrun_container_t *container, અચલ અક્ષર *source, પૂર્ણાંક targetfd, અચલ અક્ષર *target,
                 unsigned long mountflags, અચલ અક્ષર *data, libcrun_error_t *err)
{
  પૂર્ણાંક cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  પસંદગી (cgroup_mode)
    {
    સ્થિતિ CGROUP_MODE_UNIFIED:
      પાછા do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    સ્થિતિ CGROUP_MODE_LEGACY:
    સ્થિતિ CGROUP_MODE_HYBRID:
      પાછા do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  પાછા crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

સંરચના device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
સ્થિર શૂન્ય
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  સંરચના private_data_s *private_data = get_private_data (container);
  size_t i;

  જો (private_data->needed_devs_fds)
    {
      સંરચના libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      માટે (i = 0; needed_devs[i].path; i++)
        {
          જો (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            ચાલુ;

          પૂર્ણાંક fd = syscall_open_વૃક્ષ (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          જો (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          નહીંતર જો (fd >= 0)
            close (fd);
        }
    }

  જો (private_data->notify_socket_વૃક્ષ_fd < 0
      && private_data->host_notify_socket_path)
    {
      પૂર્ણાંક fd = syscall_open_વૃક્ષ (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      જો (fd >= 0)
        private_data->notify_socket_વૃક્ષ_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
સ્થિર અચલ અક્ષર *
relative_path_under_dev (અચલ અક્ષર *path)
{
  જો (path[0] != '/')
    પાછા path;
  જો (strncmp (path, "/dev/", 5) == 0)
    {
      જો (strchr (path + 5, '/'))
        પાછા ખાલી;
      પાછા path + 5;
    }
  પાછા ખાલી;
}

સ્થિર પૂર્ણાંક
mknod_and_set_attrs (libcrun_container_t *container, પૂર્ણાંક dirfd, અચલ અક્ષર *name, સંરચના device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક fd = -1;
  proc_fd_path_t fd_path;
  અચલ અક્ષર *what = "new";
  bool exists = false;
  સંરચના stat st;
  પૂર્ણાંક procfd;
  પૂર્ણાંક ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno != EEXIST)
        પાછા crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  જો (UNLIKELY ((st.st_mode & S_IFMT) != type))
    પાછા crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  જો (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    પાછા crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  જો (exists)
    પાછા 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  જો (UNLIKELY (procfd < 0))
    પાછા procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fchown `%s`", device->path);

  પાછા 1;
}

પૂર્ણાંક
libcrun_create_dev (libcrun_container_t *container, પૂર્ણાંક devfd, પૂર્ણાંક srcfd,
                    સંરચના device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  અચલ અક્ષર *fullname = device->path;
  અચલ અક્ષર *rootfs = get_private_data (container)->rootfs;
  જો (is_empty_string (fullname))
    પાછા crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free અક્ષર *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  જો (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  અચલ અક્ષર *rel_dev = relative_path_under_dev (normalized_path);

  જો (binds)
    {
      cleanup_close પૂર્ણાંક fd = -1;

      જો (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          જો (UNLIKELY (fd < 0))
            {
              જો (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              જો (UNLIKELY (fd < 0))
                પાછા crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      નહીંતર
        {
          અચલ અક્ષર *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          જો (UNLIKELY (fd < 0))
            પાછા fd;
        }

      જો (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          જો (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              જો (LIKELY (ret >= 0))
                પાછા 0;
            }
          નહીંતર
            crun_error_release (err);
        }

      {
        cleanup_close પૂર્ણાંક mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        જો (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            જો (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, ખાલી);
                જો (LIKELY (ret == 0))
                  પાછા 0;
              }
            નહીંતર
              crun_error_release (err);
          }
        નહીંતર
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, ખાલી, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, ખાલી, LABEL_MOUNT, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      જો (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          જો (ret <= 0)
            પાછા ret;
        }
      નહીંતર
        {
          cleanup_close પૂર્ણાંક dirfd = -1;
          cleanup_free અક્ષર *dirname = ખાલી;
          અક્ષર *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          જો (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          જો (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              જો (UNLIKELY (dirfd < 0))
                પાછા crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          નહીંતર
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              જો (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              જો (UNLIKELY (dirfd < 0))
                પાછા dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          જો (ret <= 0)
            પાછા ret;
        }
    }
  પાછા 0;
}

સંરચના symlink_s
{
  અચલ અક્ષર *path;
  અચલ અક્ષર *target;
  bool force;
};

સ્થિર સંરચના symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { ખાલી, ખાલી, false } };

સ્થિર પૂર્ણાંક
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  size_t i;
  સંરચના device_s *it;
  cleanup_close પૂર્ણાંક devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  અચલ અક્ષર *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map સંરચના libcrun_fd_map *dev_fds = ખાલી;
  cleanup_close_map સંરચના libcrun_fd_map *needed_fds = ખાલી;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = ખાલી;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = ખાલી;

  જો (! def || ! def->linux)
    પાછા 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  જો (UNLIKELY (devfd < 0))
    પાછા devfd;

  માટે (i = 0; i < def->linux->devices_len; i++)
    {
      સંરચના device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      જો (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  માટે (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close પૂર્ણાંક srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      જો (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  માટે (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      જો (UNLIKELY (ret < 0))
        {
          પૂર્ણાંક saved_errno = errno;

          જો (errno == EEXIST && ! symlinks[i].force)
            ચાલુ;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          જો (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              જો (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              જો (ret < 0 && errno == EBUSY)
                {
                  cleanup_close પૂર્ણાંક tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  જો (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      જો (umount2 (procpath, MNT_DETACH) == 0)
                        જાઓ retry_unlink;
                    }
                }
              જો (ret == 0)
                જાઓ retry_symlink;
            }
          પાછા crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  જો (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
સ્થિર અચલ અક્ષર *
next_path_component (અચલ અક્ષર *p, size_t *len)
{
  માટે (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      જો (*len != 1 || p[0] != '.')
        પાછા p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
સ્થિર bool
same_path (અચલ અક્ષર *a, અચલ અક્ષર *b)
{
  size_t la, lb;

  માટે (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      જો (la != lb || memcmp (a, b, la) != 0)
        પાછા false;
      જો (la == 0)
        પાછા true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
સ્થિર bool
is_duplicate_path (અક્ષર **paths, size_t i)
{
  size_t j;

  જો (paths[i] == ખાલી)
    પાછા false;

  માટે (j = 0; j < i; j++)
    જો (paths[j] && same_path (paths[j], paths[i]))
      પાછા true;

  પાછા false;
}

સ્થિર પૂર્ણાંક
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  પૂર્ણાંક ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  જો (! def || ! def->linux)
    પાછા 0;

  માટે (i = 0; i < def->linux->masked_paths_len; i++)
    {
      જો (is_duplicate_path (def->linux->masked_paths, i))
        ચાલુ;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  માટે (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
do_pivot (libcrun_container_t *container, અચલ અક્ષર *rootfs, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક oldrootfd = -1;
  cleanup_close પૂર્ણાંક newrootfd = -1;
  પૂર્ણાંક ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (oldrootfd < 0))
    પાછા crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (newrootfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_મૂળ (".", ".");
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, ખાલી, -1, ".", ખાલી, MS_REC | MS_PRIVATE, ખાલી, LABEL_MOUNT, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = umount2 (".", MNT_DETACH);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "umount2 oldroot");

  કરો
    {
      ret = umount2 (".", MNT_DETACH);
      જો (ret < 0 && errno == EINVAL)
        તોડો;
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "umount2 oldroot");
  } જ્યારે (ret == 0);

  ret = chdir ("/");
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "chdir to newroot");

  પાછા 0;
}

સ્થિર પૂર્ણાંક
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, અક્ષર **data, libcrun_error_t *err)
{
  અચલ અક્ષર *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close પૂર્ણાંક fd = -1;
  સંરચના stat st;
  પૂર્ણાંક ret;

  જો (*data != ખાલી && strstr (*data, "mode="))
    પાછા 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  જો (fd < 0)
    {
      જો (crun_error_get_errno (err) != ENOENT)
        પાછા fd;

      crun_error_release (err);
      પાછા 0;
    }
  ret = fstat (fd, &st);
  જો (ret < 0)
    પાછા crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  પાછા 0;
}

સ્થિર પૂર્ણાંક
get_default_flags (libcrun_container_t *container, અચલ અક્ષર *destination, અક્ષર **data)
{
  જો (strcmp (destination, "/proc") == 0)
    પાછા 0;
  જો (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      પાછા MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  જો (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      પાછા MS_NOEXEC | MS_STRICTATIME;
    }
  જો (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      પાછા MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  જો (strcmp (destination, "/dev/mqueue") == 0)
    પાછા MS_NOEXEC | MS_NOSUID | MS_NODEV;
  જો (strcmp (destination, "/dev/pts") == 0)
    {
      જો (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      નહીંતર
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      પાછા MS_NOEXEC | MS_NOSUID;
    }
  જો (strcmp (destination, "/sys") == 0)
    પાછા MS_NOEXEC | MS_NOSUID | MS_NODEV;

  પાછા 0;
}

સ્થિર અચલ અક્ષર *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  પાછા find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

સ્થિર પૂર્ણાંક
check_valid_no_follow_file_system (પૂર્ણાંક fd, અચલ અક્ષર *destination, libcrun_error_t *err)
{
  સંરચના statfs sfs;

  પૂર્ણાંક ret = fstatfs (fd, &sfs);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "statfs `%s`", destination);

  પસંદગી (sfs.f_type)
    {
    સ્થિતિ PROC_SUPER_MAGIC:
    સ્થિતિ SYSFS_MAGIC:
    સ્થિતિ DEVPTS_SUPER_MAGIC:
      પાછા crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
safe_create_symlink (પૂર્ણાંક rootfsfd, અચલ અક્ષર *rootfs, અચલ અક્ષર *target, અચલ અક્ષર *destination, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક parent_dir_fd = -1;
  cleanup_free અક્ષર *buffer = ખાલી;
  અક્ષર *part;
  પૂર્ણાંક ret;

  જો (is_empty_string (destination))
    પાછા crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  જો (UNLIKELY (parent_dir_fd < 0))
    પાછા parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  જો (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      જો (errno == EEXIST)
        {
          cleanup_free અક્ષર *link = ખાલી;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          જો (UNLIKELY (len < 0))
            પાછા len;

          જો ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            પાછા 0;

          પાછા crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      પાછા crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
handle_copy_symlink (libcrun_container_t *container, અચલ અક્ષર *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     અચલ અક્ષર *link_target, libcrun_error_t *err)
{
  cleanup_free અક્ષર *target = ખાલી;

  જો (link_target == ખાલી)
    {
      પૂર્ણાંક old_મૂળ_fd = get_old_મૂળ_fd (get_private_data (container));
      અચલ અક્ષર *source = mount->source;
      ssize_t len;

      જો (old_મૂળ_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_મૂળ_fd, source + 1, &target, 0, err);
      નહીંતર
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      જો (UNLIKELY (len < 0))
        પાછા len;
      link_target = target;
    }

  પાછા safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

સ્થિર પૂર્ણાંક
prepare_sysfs_or_proc_mount (libcrun_container_t *container, અચલ અક્ષર *target,
                             પૂર્ણાંક *targetfd, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno == ENOENT)
        {
          જો (strchr (target, '/'))
            પાછા crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      નહીંતર જો (errno == ENOTDIR)
        પાછા crun_make_error (err, errno, "the target `/%s` is invalid", target);

      જો (ret < 0)
        પાછા crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  પાછા 0;
}

સ્થિર પૂર્ણાંક
handle_tmpcopyup (libcrun_container_t *container, અચલ અક્ષર *rootfs, અચલ અક્ષર *target,
                  પૂર્ણાંક copy_from_fd, libcrun_error_t *err)
{
  પૂર્ણાંક destfd, ret;
  cleanup_close પૂર્ણાંક tmpfd = copy_from_fd;

  target = consume_slashes (target);

  જો (is_empty_string (target))
    પાછા crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  જો (UNLIKELY (destfd < 0))
    પાછા crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  પાછા ret;
}

સ્થિર પૂર્ણાંક
get_mount_label_how (અચલ અક્ષર *type, bool is_sysfs_or_proc)
{
  જો (is_sysfs_or_proc)
    પાછા LABEL_NONE;

  જો (strcmp (type, "mqueue") == 0)
    પાછા LABEL_XATTR;

  પાછા LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
સ્થિર unsigned long
get_bind_mount_locked_flags (પૂર્ણાંક source_mountfd, અચલ અક્ષર *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  સંરચના statfs sfs;
  size_t i;
  પૂર્ણાંક ret;

  જો (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  નહીંતર
    ret = statfs (source, &sfs);
  જો (UNLIKELY (ret < 0))
    પાછા 0;

  માટે (i = 0; i < mount->options_len; i++)
    {
      અચલ સંરચના propagation_flags_s *prop;

      જો (mount->options[i] == ખાલી)
        ચાલુ;

      prop = libcrun_str2mount_flags (mount->options[i]);
      જો (prop == ખાલી || (prop->extra_flags & OPTION_RECURSIVE))
        ચાલુ;

      જો (prop->clear)
        cleared |= prop->flags;
      નહીંતર
        cleared &= ~prop->flags;
    }

  પાછા sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
સ્થિર અચલ અક્ષર *
get_path_in_rootfs (અચલ અક્ષર *rootfs, અચલ અક્ષર *path)
{
  size_t len;

  જો (rootfs == ખાલી || path == ખાલી)
    પાછા ખાલી;

  len = strlen (rootfs);
  જ્યારે (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  જો (len == 0)
    પાછા ખાલી;

  જો (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    પાછા ખાલી;

  પાછા consume_slashes (path + len);
}

સ્થિર પૂર્ણાંક
process_single_mount (libcrun_container_t *container, અચલ અક્ષર *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      સંરચના libcrun_fd_map *mount_fds, size_t mount_index,
                      અચલ અક્ષર *systemd_cgroup_v1, libcrun_error_t *err)
{
  અચલ અક્ષર *target = consume_slashes (mount->destination);
  cleanup_close પૂર્ણાંક source_mountfd = -1;
  cleanup_free અક્ષર *data = ખાલી;
  અક્ષર *type;
  અક્ષર *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close પૂર્ણાંક copy_from_fd = -1;
  cleanup_close પૂર્ણાંક targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  પૂર્ણાંક no_remount = 0;
  પૂર્ણાંક ret;

  જો (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  જો (mount->options == ખાલી)
    flags = get_default_flags (container, mount->destination, &data);
  નહીંતર
    {
      size_t j;

      માટે (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  જો (type == ખાલી && (flags & MS_BIND) == 0)
    પાછા crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  જો ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      અચલ અક્ષર *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      જો (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          જો (UNLIKELY (source_mountfd < 0))
            પાછા crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  જો ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  જો (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      જો (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      જો (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  જો (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      અચલ અક્ષર *path = mount->source;

      જો ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        પાછા crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      જો (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, ખાલી);
        }
      નહીંતર જો (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      નહીંતર જો (path[0] == '/')
        {
          પૂર્ણાંક old_મૂળ_fd = get_old_મૂળ_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          જો (old_મૂળ_fd >= 0)
            ret = get_file_type_at (old_મૂળ_fd, &src_mode, nofollow, (path[1] == '\0') ? ખાલી : path + 1);
          નહીંતર
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      નહીંતર
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "cannot stat `%s`", path);

      જો (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          પૂર્ણાંક bind_dirfd = AT_FDCWD;
          અચલ અક્ષર *bind_source = mount->source;
          પૂર્ણાંક old_મૂળ_fd = get_old_મૂળ_fd (get_private_data (container));

          જો (old_મૂળ_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_મૂળ_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          source_mountfd = ret;
        }
    }

  જો (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      અચલ અક્ષર *cached_target = ખાલી;

      જો (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      mounted = true;
    }
  નહીંતર જો (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      bool is_dir = S_ISDIR (src_mode);

      જો (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
      નહીંતર
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          targetfd = ret;
        }
    }

  જો (extra_flags & OPTION_TMPCOPYUP)
    {
      જો (strcmp (type, "tmpfs") != 0)
        પાછા crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      જો (UNLIKELY (copy_from_fd < 0))
        {
          જો (errno != ENOTDIR)
            પાછા crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  જો (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, ખાલી);
      જો (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          જો (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, ખાલી, source_mountfd, target, ખાલી, remaining_flags, data, LABEL_NONE | no_remount, err);
              જો (UNLIKELY (ret < 0))
                પાછા ret;
            }

          mounted = true;

          જો (is_empty_string (target))
            {
              પૂર્ણાંક new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              જો (UNLIKELY (new_rootfsfd < 0))
                પાછા crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      નહીંતર જો (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          પાછા crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      નહીંતર જો (get_private_data (container)->host_મૂળ_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          પાછા crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      નહીંતર જો (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  જો (! mounted)
    {
      જો (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
      નહીંતર જો (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
      નહીંતર
        {
          પૂર્ણાંક extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }

  જો (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (rec_clear || rec_set)
    {
      અચલ bool is_dir = S_ISDIR (src_mode);
      cleanup_close પૂર્ણાંક dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      જો (UNLIKELY (dfd < 0))
        પાછા crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
do_mounts (libcrun_container_t *container, અચલ અક્ષર *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  અચલ અક્ષર *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map સંરચના libcrun_fd_map *mount_fds = ખાલી;
  size_t i;
  પૂર્ણાંક ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = ખાલી;

  માટે (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
પૂર્ણાંક
libcrun_container_do_bind_mount (libcrun_container_t *container, અક્ષર *mount_source, અક્ષર *mount_destination, અક્ષર **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  અચલ અક્ષર *target = consume_slashes (mount_destination);
  cleanup_free અક્ષર *data = ખાલી;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close પૂર્ણાંક targetfd = -1;
  પૂર્ણાંક is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  અચલ અક્ષર *rootfs = get_private_data (container)->rootfs;
  પૂર્ણાંક ret;

  જો ((get_private_data (container)->rootfsfd < 0) || (rootfs == ખાલી))
    પાછા crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  જો (mount_options == ખાલી)
    flags = get_default_flags (container, mount_destination, &data);
  નહીંતર
    {
      size_t j;

      માટે (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  જો (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  જો (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      જો (UNLIKELY (is_dir < 0))
        પાછા is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
પૂર્ણાંક
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, પૂર્ણાંક *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close પૂર્ણાંક notify_fd = -1;
  cleanup_free અક્ષર *host_notify_socket_path = ખાલી;
  cleanup_free અક્ષર *state_dir = ખાલી;
  અક્ષર *host_path = ખાલી;
  પૂર્ણાંક ret;

  જો (container && get_private_data (container)->host_notify_socket_path)
    {
      અચલ અક્ષર *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  જો (host_path == ખાલી)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_મૂળ, context->id, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  જો (UNLIKELY (notify_fd < 0))
    પાછા notify_fd;

  જો (UNLIKELY (chmod (host_path, 0777) < 0))
    પાછા crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  જો (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (શૂન્ય) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  પાછા 1;
#નહીંતર
  (શૂન્ય) context;
  (શૂન્ય) container;
  (શૂન્ય) err;
  *notify_socket_out = -1;
  પાછા 0;
#endif
}

#ifdef HAVE_SYSTEMD
સ્થિર પૂર્ણાંક
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  અચલ અક્ષર *notify_socket = container->context->notify_socket;
  cleanup_free અક્ષર *host_notify_socket_path = ખાલી;
  cleanup_free અક્ષર *container_notify_socket_path = ખાલી;
  cleanup_free અક્ષર *state_dir = ખાલી;
  uid_t container_મૂળ_uid = -1;
  gid_t container_મૂળ_gid = -1;
  પૂર્ણાંક notify_socket_વૃક્ષ_fd;

  જો (notify_socket == ખાલી)
    પાછા 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_મૂળ : ખાલી),
                                     (container->context ? container->context->id : ખાલી), err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = mkdir (host_notify_socket_path, 0700);
  જો (ret < 0)
    પાછા crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  જો (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_મૂળ_in_the_userns (container->container_def, 0, 0, &container_મૂળ_uid, &container_મૂળ_gid);
      જો (container_મૂળ_uid != ((uid_t) -1) && container_મૂળ_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_મૂળ_uid, container_મૂળ_gid);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_મૂળ_uid, container_મૂળ_gid);
        }
    }

  notify_socket_વૃક્ષ_fd = syscall_open_વૃક્ષ (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  જો (notify_socket_વૃક્ષ_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_વૃક્ષ_fd = notify_socket_વૃક્ષ_fd;
  નહીંતર જો (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  નહીંતર જો (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  નહીંતર
    /* some other error */
    પાછા crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = ખાલી;
  પાછા 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
સ્થિર પૂર્ણાંક
do_finalize_notify_socket (libcrun_container_t *container, અચલ અક્ષર *rootfs, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_free અક્ષર *host_notify_socket_path = ખાલી;
  cleanup_free અક્ષર *container_notify_socket_path = ખાલી;
  cleanup_free અક્ષર *container_notify_socket_path_dir_alloc = ખાલી;
  અચલ અક્ષર *container_notify_socket_path_dir = ખાલી;
  cleanup_close પૂર્ણાંક notify_socket_વૃક્ષ_fd = -1;
  cleanup_close પૂર્ણાંક targetfd = -1;
  પૂર્ણાંક did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = ખાલી;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = ખાલી;

  જો (host_notify_socket_path == ખાલી || container_notify_socket_path == ખાલી)
    પાછા 0;

  જો (rootfs == ખાલી)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  જો (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  targetfd = ret;

  notify_socket_વૃક્ષ_fd = get_private_data (container)->notify_socket_વૃક્ષ_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_વૃક્ષ_fd = -1;

  જો (notify_socket_વૃક્ષ_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_વૃક્ષ_fd, targetfd, ખાલી);
      જો (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          જો (UNLIKELY (targetfd < 0))
            પાછા targetfd;

          ret = do_mount (container, ખાલી, targetfd, container_notify_socket_path_dir, ખાલી,
                          MS_REC | MS_PRIVATE, ખાલી, LABEL_NONE, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
      નહીંતર જો (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      નહીંતર
        પાછા crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_વૃક્ષ_fd,
                                container_notify_socket_path_dir);
    }

  જો (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, ખાલી,
                      MS_BIND | MS_REC | MS_PRIVATE, ખાલી, LABEL_MOUNT, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
make_parent_mount_private (અચલ અક્ષર *rootfs, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક rootfsfd = -1;
  સંરચના stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  જો (UNLIKELY (rootfsfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", rootfs);

  જો (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    પાછા crun_make_error (err, errno, "fstat `%s`", rootfs);

  માટે (;;)
    {
      સંરચના stat cur_st;
      libcrun_error_t tmp_err = ખાલી;
      પૂર્ણાંક ret;
      cleanup_close પૂર્ણાંક parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      જો (ret == 0)
        પાછા 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (ખાલી, proc_path, ખાલી, MS_PRIVATE, ખાલી);
      જો (ret == 0)
        પાછા 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      જો (parentfd < 0)
        {
          પૂર્ણાંક saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          જો (ret != 0)
            પાછા crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          પાછા crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      જો (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        પાછા crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      જો (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        તોડો;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  પાછા crun_make_error (err, 0, "make `%s` private", rootfs);
}

પૂર્ણાંક
libcrun_set_mounts (સંરચના container_entrypoint_s *entrypoint_args, libcrun_container_t *container, અચલ અક્ષર *rootfs, set_mounts_cb_t cb, શૂન્ય *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  પૂર્ણાંક is_user_ns = 0;
  પૂર્ણાંક ret = 0;

  જો (rootfs == ખાલી || def->mounts == ખાલી)
    પાછા 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  જો (UNLIKELY (ret < 0))
    પાછા crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  જો (def->મૂળ->readonly)
    {
      સંરચના remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      પૂર્ણાંક fd;

      fd = dup (get_private_data (container)->rootfsfd);
      જો (UNLIKELY (fd < 0))
        પાછા crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, ખાલી, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  જો (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      જો (UNLIKELY (is_user_ns < 0))
        પાછા is_user_ns;
    }

  જો (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  જો (cb)
    {
      ret = cb (cb_data, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = ખાલી;
      અચલ અક્ષર *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (શૂન્ય) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
umount_or_hide (અચલ અક્ષર *target, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = umount2 (target, MNT_DETACH);
  જો (UNLIKELY (ret < 0))
    {
      પૂર્ણાંક saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      જો (errno == EINVAL)
        {
          ret = mount (ખાલી, target, "tmpfs", 0, "size=0k");
          જો (LIKELY (ret == 0))
            પાછા 0;
        }

      પાછા crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  પાછા ret;
}

સ્થિર પૂર્ણાંક
move_મૂળ (અચલ અક્ષર *rootfs, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = chdir (rootfs);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = umount_or_hide ("/proc", err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "chdir to `%s`", rootfs);

  પાછા 0;
}

પૂર્ણાંક
libcrun_finalize_mounts (સંરચના container_entrypoint_s *entrypoint_args, libcrun_container_t *container, અચલ અક્ષર *rootfs,
                         set_mounts_cb_t after_pivot_cb, શૂન્ય *cb_data, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  જો (UNLIKELY (ret < 0))
    પાછા crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_મૂળ_fd));

  જો (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      જો (get_private_data (container)->no_pivot)
        {
          ret = move_મૂળ (rootfs, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
      નહીંતર
        {
          ret = do_pivot (container, rootfs, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      ret = do_mount (container, ખાલી, -1, "/", ખાલી,
                      get_private_data (container)->rootfs_propagation,
                      ખાલી, LABEL_MOUNT, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = chdir ("/");
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "chdir to `/`");
    }

  જો (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = finalize_mounts (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
maybe_open_વૃક્ષ_namespace (અચલ અક્ષર *rootfs, પૂર્ણાંક *out_fd, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક rootfs_fd = -1;
  પૂર્ણાંક વૃક્ષ_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (rootfs_fd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", rootfs);

  વૃક્ષ_fd = syscall_open_વૃક્ષ (rootfs_fd, "",
                               OPEN_વૃક્ષ_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  જો (વૃક્ષ_fd < 0)
    {
      જો (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        પાછા 0;
      પાછા crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = વૃક્ષ_fd;
  પાછા 0;
}

સ્થિર સંરચના libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  સંરચના libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  જો (mount_fds == ખાલી)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  પાછા mount_fds;
}

સ્થિર પૂર્ણાંક
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free અક્ષર *mnt_data = ખાલી;
  unsigned long mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  અચલ અક્ષર *label_type = ખાલી;
  અચલ અક્ષર *label_val = ખાલી;
  અચલ અક્ષર *fstype;
  પૂર્ણાંક mnt_fd;
  size_t j;

  જો (mount->type == ખાલી)
    પાછા -1;

  fstype = mount->type;

  માટે (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  જો (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      પૂર્ણાંક extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      જો (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          જો (label_type == ખાલી)
            crun_error_release (err);
          નહીંતર
            label_val = def->linux->mount_label;
        }
    }

  જો (strcmp (fstype, "cgroup") == 0)
    {
      પૂર્ણાંક cgroup_mode = libcrun_get_cgroup_mode (err);
      જો (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  જો (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, ખાલી, ખાલી, mnt_data);

  પાછા mnt_fd;
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
સંરચના mnt_id_req_s
{
  uint32_t size;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

સંરચના statmount_s
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
  અક્ષર str[];
};

સ્થિર પૂર્ણાંક
syscall_listmount (સંરચના mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned પૂર્ણાંક flags)
{
#ifdef __NR_listmount
  પાછા (પૂર્ણાંક) syscall (__NR_listmount, req, ids, nr, flags);
#નહીંતર
  (શૂન્ય) req;
  (શૂન્ય) ids;
  (શૂન્ય) nr;
  (શૂન્ય) flags;
  errno = ENOSYS;
  પાછા -1;
#endif
}

સ્થિર પૂર્ણાંક
syscall_statmount (સંરચના mnt_id_req_s *req, સંરચના statmount_s *buf, size_t size, unsigned પૂર્ણાંક flags)
{
#ifdef __NR_statmount
  પાછા (પૂર્ણાંક) syscall (__NR_statmount, req, buf, size, flags);
#નહીંતર
  (શૂન્ય) req;
  (શૂન્ય) buf;
  (શૂન્ય) size;
  (શૂન્ય) flags;
  errno = ENOSYS;
  પાછા -1;
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
સ્થિર bool
mounts_hit_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = ખાલી;
  size_t i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  માટે (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      જો (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != ખાલી && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          તોડો;
        }
    }
  જો (! has_absolute_bind)
    પાછા false;

  ids = xmalloc (capacity * કદ (uint64_t));
  માટે (;;)
    {
      સંરચના mnt_id_req_s req = {
        .size = કદ (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      પૂર્ણાંક ret;

      જો (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * કદ (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      જો (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        પાછા true;
      જો (ret == 0)
        તોડો;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  માટે (i = 0; i < n_ids; i++)
    {
      અક્ષર buffer[4096] __attribute__ ((aligned (8)));
      સંરચના statmount_s *sm = (સંરચના statmount_s *) buffer;
      સંરચના mnt_id_req_s req = {
        .size = કદ (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      અચલ અક્ષર *મૂળ, *point;
      size_t j;

      જો (syscall_statmount (&req, sm, કદ (buffer), 0) < 0)
        ચાલુ;
      જો ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        ચાલુ;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      મૂળ = sm->str + sm->mnt_root;
      જો (! has_prefix (મૂળ, "mnt:["))
        ચાલુ;

      point = sm->str + sm->mnt_point;

      માટે (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          અચલ અક્ષર *source = def->mounts[j]->source;
          size_t source_len;

          જો (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            ચાલુ;
          જો (source == ખાલી || source[0] != '/')
            ચાલુ;

          source_len = strlen (source);
          જ્યારે (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          જો (strncmp (point, source, source_len) != 0)
            ચાલુ;

          /* The source itself is a pinned mount namespace.  */
          જો (point[source_len] == '\0')
            પાછા true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          જો (recursive && (source_len == 1 || point[source_len] == '/'))
            પાછા true;
        }
    }

  પાછા false;
}

સ્થિર bool
can_use_open_વૃક્ષ_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  સંરચના libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  જો (has_hooks || has_userns)
    પાછા false;

  જો (mounts_hit_pinned_mount_namespace (def))
    પાછા false;

  જો (container->context->no_open_વૃક્ષ_namespace)
    પાછા false;

  {
    libcrun_error_t tmp_err = ખાલી;
    પૂર્ણાંક in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    જો (in_userns != 0)
      પાછા false;
  }

  mount_fds = get_fd_map (container);
  માટે (i = 0; i < def->mounts_len; i++)
    {
      જો (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          જો (! has_cached_target)
            પાછા false;
        }
    }

  પાછા true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
સ્થિર bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  જો (mount->type == ખાલી || strcmp (mount->type, "tmpfs") != 0)
    પાછા false;

  માટે (i = 0; i < mount->options_len; i++)
    જો (mount->options[i] != ખાલી && has_prefix (mount->options[i], "mode="))
      પાછા false;

  પાછા true;
}

સ્થિર પૂર્ણાંક
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, અક્ષર **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close પૂર્ણાંક rootfsfd = -1;
  cleanup_close પૂર્ણાંક વૃક્ષ_fd = -1;
  libcrun_error_t tmp_err = ખાલી;
  bool use_open_વૃક્ષ = false;
  size_t i;
  પૂર્ણાંક ret;

  જો (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, ખાલી, ખાલી, ખાલી, ખાલી);

  જો ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  મુક્ત (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  માટે (i = 0; i < def->mounts_len; i++)
    {
      સંરચના libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close પૂર્ણાંક mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      જો (mount_fds && mount_fds->fds[i] >= 0)
        ચાલુ;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      જો (def->mounts[i]->source != ખાલી
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free અક્ષર *mnt_data = ખાલી;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          માટે (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          જો (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free અક્ષર *target = ખાલી;
              libcrun_error_t tmp_err = ખાલી;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              જો (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              જો (get_private_data (container)->copy_symlink_targets == ખાલી)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * કદ (અક્ષર *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = ખાલી;
            }
        }

      જો (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          જો (mount_fds && def->mounts[i]->source != ખાલી && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == ખાલી)
            {
              libcrun_error_t tmp_err = ખાલી;
              unsigned long propagation = MS_PRIVATE;

              જો (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              જો (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          ચાલુ;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      જો (mnt_fd < 0)
        {
          crun_error_release (err);
          ચાલુ;
        }

      જો (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close પૂર્ણાંક destfd = -1;
          સંરચના stat st;

          જો (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              જો (UNLIKELY (rootfsfd < 0))
                પાછા crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          જો (destfd < 0)
            crun_error_release (&tmp_err);
          નહીંતર જો (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              પૂર્ણાંક procfd = get_procfd (get_private_data (container), err);
              જો (UNLIKELY (procfd < 0))
                પાછા procfd;

              get_self_fd_path (fd_path, mnt_fd);
              ret = fchmodat (procfd, fd_path, st.st_mode & 07777, 0);
              (શૂન્ય) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_empty_dir_cached (container, ખાલી, &tmp_err);
  જો (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  જો (! get_private_data (container)->joined_mount_ns
      && can_use_open_વૃક્ષ_namespace (container))
    use_open_વૃક્ષ = true;

  જો (use_open_વૃક્ષ)
    {
      ret = maybe_open_વૃક્ષ_namespace (*rootfs, &વૃક્ષ_fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  જો (વૃક્ષ_fd >= 0)
    preopen_needed_devs_and_notify (container);

  જો (વૃક્ષ_fd >= 0)
    {
      ret = setns (વૃક્ષ_fd, CLONE_NEWNS);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (ખાલી, "/", ખાલી, MS_REMOUNT | MS_BIND, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, ખાલી, -1, "/", ખાલી, MS_REC | MS_PRIVATE, ખાલી, LABEL_MOUNT, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = do_mount (container, ખાલી, -1, "/", ખાલી, rootfs_propagation, ખાલી, LABEL_MOUNT, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_મૂળ_switched = true;
      મુક્ત (*rootfs);
      *rootfs = xstrdup ("/");
    }
  નહીંતર
    {
      જો (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, ખાલી, -1, "/", ખાલી, rootfs_propagation, ખાલી, LABEL_MOUNT, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = make_parent_mount_private (*rootfs, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, ખાલી, MS_BIND | MS_REC | MS_PRIVATE, ખાલી, LABEL_MOUNT, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  જો (! container->context->force_no_cgroup)
    {
      પૂર્ણાંક cgroup_mode = libcrun_get_cgroup_mode (err);
      જો (UNLIKELY (cgroup_mode < 0))
        પાછા cgroup_mode;

      જો (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          અક્ષર *unified_cgroup_path = ખાલી;
          પૂર્ણાંક procfd = get_procfd (get_private_data (container), err);
          જો (UNLIKELY (procfd < 0))
            પાછા procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  જો (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = ખાલી;
      પાછા ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = ખાલી;

  જો (વૃક્ષ_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  જો (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  પાછા 0;
}

પૂર્ણાંક
libcrun_do_pivot_મૂળ (libcrun_container_t *container, bool no_pivot, અક્ષર **rootfs, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  જો (*rootfs == ખાલી)
    પાછા 0;

  જો (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      જો (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "chdir to `/`");
        }
    }
  નહીંતર
    {
      ret = chroot (*rootfs);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "chdir to `/`");

      મુક્ત (*rootfs);
      *rootfs = xstrdup ("/");
    }

  પાછા 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
પૂર્ણાંક
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  સંરચના stat statbuf;
  cleanup_close પૂર્ણાંક fd;
  પૂર્ણાંક ret;
  પૂર્ણાંક i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  જો (UNLIKELY (fd == -1))
    પાછા crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  માટે (i = 0; i <= 2; i++)
    {
      જો (UNLIKELY (fstat (i, &statbuf) == -1))
        પાછા crun_make_error (err, errno, "stat fd `%d`", i);
      જો (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          જો (UNLIKELY (dup2 (fd, i) == -1))
            પાછા crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
uidgidmap_helper (અક્ષર *helper, pid_t pid, અચલ અક્ષર *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  અક્ષર pid_fmt[16];
  અક્ષર *args[MAX_ARGS + 1];
  અક્ષર *next;
  cleanup_free અક્ષર *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  પૂર્ણાંક ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, કદ (pid_fmt), "%d", pid);
  જો (UNLIKELY (ret >= (પૂર્ણાંક) કદ (pid_fmt)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  જ્યારે (nargs < MAX_ARGS)
    {
      અક્ષર *p = strsep (&next, " \n");
      જો (next == ખાલી)
        તોડો;
      args[nargs++] = p;
    }
  args[nargs++] = ખાલી;

  ret = run_process (args, err);
  જો (ret < 0)
    પાછા ret;
  જો (ret > 0)
    પાછા crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  પાછા 0;
}

સ્થિર પૂર્ણાંક
newgidmap (pid_t pid, અચલ અક્ષર *map_file, libcrun_error_t *err)
{
  પાછા uidgidmap_helper ("newgidmap", pid, map_file, err);
}

સ્થિર પૂર્ણાંક
newuidmap (pid_t pid, અચલ અક્ષર *map_file, libcrun_error_t *err)
{
  પાછા uidgidmap_helper ("newuidmap", pid, map_file, err);
}

સ્થિર પૂર્ણાંક
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_close પૂર્ણાંક fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  જો (UNLIKELY (fd < 0))
    પાછા fd;

  ret = TEMP_FAILURE_RETRY (લિખ (fd, "deny", 4));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  પાછા 0;
}

સ્થિર પૂર્ણાંક
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_free અક્ષર *content = ખાલી;

  જો (get_private_data (container)->deny_setgroups)
    પાછા 0;

  જો (container->container_def->annotations)
    {
      અચલ અક્ષર *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      જો (annotation)
        પાછા strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close પૂર્ણાંક fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    જો (fd < 0)
      ret = fd;
    નહીંતર
      ret = read_all_fd (fd, "self/setgroups", &content, ખાલી, err);
  }
  જો (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      જો (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          પાછા 1;
        }
      પાછા ret;
    }

  પાછા strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

પૂર્ણાંક
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = ખાલી;
  size_t additional_gids_len = 0;
  પૂર્ણાંક can_do_setgroups;
  પૂર્ણાંક ret;

  જો (process != ખાલી && process->user != ખાલી)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  જો (UNLIKELY (can_do_setgroups < 0))
    પાછા can_do_setgroups;

  જો (can_do_setgroups == 0)
    {
      અચલ અક્ષર *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      જો (annotation && strcmp (annotation, "0") != 0)
        પાછા 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      માટે (i = 0; i < additional_gids_len; i++)
        જો (additional_gids[i] != process->user->gid)
          પાછા crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (પૂર્ણાંક) additional_gids[i]);
      પાછા 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "setgroups");

  પાછા 0;
}

પૂર્ણાંક
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  જો (get_private_data (container)->unshare_cgroupns)
    {
      પૂર્ણાંક ret = unshare (CLONE_NEWCGROUP);
      જો (UNLIKELY (ret < 0))
        {
          જો (errno != EINVAL)
            પાછા crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  પાછા 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
પૂર્ણાંક
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  જો (len != 1)
    પાછા 0;

  જો (mappings[0]->size != 1)
    પાછા 0;

  જો (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    પાછા 0;

  પાછા 1;
}

સ્થિર પૂર્ણાંક
write_id_map (libcrun_container_t *container, pid_t pid, અચલ અક્ષર *map_name,
              પૂર્ણાંક (*idmap_helper) (pid_t, અચલ અક્ષર *, libcrun_error_t *), અચલ અક્ષર *helper_name,
              bool warn_on_failure, અચલ અક્ષર *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  પૂર્ણાંક ret = 0;

  જો (container->host_uid)
    ret = idmap_helper (pid, map, err);
  જો (container->host_uid == 0 || ret < 0)
    {
      cleanup_close પૂર્ણાંક fd = -1;

      જો (ret < 0)
        {
          જો (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      જો (UNLIKELY (fd < 0))
        પાછા fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      જો (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free અક્ષર *single_mapping = ખાલી;
          crun_error_release (err);

          જો (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              જો (UNLIKELY (ret < 0))
                પાછા ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          જો (UNLIKELY (fd < 0))
            પાછા fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  પાછા ret;
}

પૂર્ણાંક
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free અક્ષર *uid_map = ખાલી;
  cleanup_free અક્ષર *gid_map = ખાલી;
  size_t uid_map_len = 0, gid_map_len = 0;
  પૂર્ણાંક ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  જો ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    પાછા 0;

  જો (! def || ! def->linux)
    પાછા 0;

  જો (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      uid_map_len = (size_t) ret;
      જો (uid_map == ખાલી)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }

  જો (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      gid_map_len = (size_t) ret;
      જો (gid_map == ખાલી)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

સંરચના all_caps_s
{
  unsigned long effective[2];
  unsigned long permitted[2];
  unsigned long inheritable[2];
  unsigned long ambient[2];
  unsigned long bounding[2];
};

સ્થિર પૂર્ણાંક
has_cap_on (પૂર્ણાંક cap, long unsigned *caps)
{
  જો (cap < 32)
    પાછા CAP_TO_MASK_0 (cap) & caps[0];
  પાછા (CAP_TO_MASK_1 (cap) & caps[1]);
}

સ્થિર unsigned long cap_last_cap;

પૂર્ણાંક
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક fd = -1;
  પૂર્ણાંક ret;
  અક્ષર buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  જો (fd < 0)
    પાછા fd;
  ret = TEMP_FAILURE_RETRY (પઠ (fd, buffer, કદ (buffer) - 1));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, ખાલી, 10);
  જો (errno != 0)
    પાછા crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  પાછા 0;
}

સ્થિર પૂર્ણાંક
set_required_caps (સંરચના all_caps_s *caps, uid_t uid, gid_t gid, પૂર્ણાંક no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  પૂર્ણાંક ret;
  સંરચના __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  સંરચના __user_cap_data_struct data[2] = { { 0 } };

  જો (cap_last_cap == 0)
    પાછા crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  માટે (cap = 0; cap <= cap_last_cap; cap++)
    જો (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        જો (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          પાછા crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  જો (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    પાછા crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  જો (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    પાછા crun_make_error (err, errno, "prctl reset ambient");

  માટે (cap = 0; cap <= cap_last_cap; cap++)
    જો (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        જો (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          પાછા crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  જો (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "prctl reset KEEPCAPS");

      જો (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          માટે (cap = 0; cap <= cap_last_cap; cap++)
            જો (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "capset");
        }
    }
#endif

  જો (no_new_privs)
    જો (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      પાછા crun_make_error (err, errno, "prctl set no new privs");

  પાછા 0;
}

સ્થિર પૂર્ણાંક
read_caps (unsigned long caps[2], અક્ષર **કિંમતો, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  માટે (i = 0; i < len; i++)
    {
      cap_કિંમત_t cap;
      જો (cap_from_name (કિંમતો[i], &cap) < 0)
        પાછા crun_make_error (err, 0, "unknown capability `%s`", કિંમતો[i]);

      જો (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      નહીંતર
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#નહીંતર
  caps[0] = 0;
  caps[1] = 0;
#endif
  પાછા 0;
}

પૂર્ણાંક
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  જો (proc->selinux_label)
    પાછા set_selinux_label (container, proc->selinux_label, now, err);

  પાછા 0;
}

પૂર્ણાંક
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  જો (proc->apparmor_profile)
    પાછા set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  પાછા 0;
}

પૂર્ણાંક
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  પૂર્ણાંક no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  સંરચના all_caps_s caps = {};
  પૂર્ણાંક ret;

  જો (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

સંરચના rlimit_s
{
  અચલ અક્ષર *name;
  પૂર્ણાંક કિંમત;
};

સંરચના rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
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
                              { "RLIMIT_STACK", RLIMIT_ઢગલો },
                              { ખાલી, 0 } };

સ્થિર પૂર્ણાંક
get_rlimit_resource (અચલ અક્ષર *name)
{
  સંરચના rlimit_s *it;
  માટે (it = rlimits; it->name; it++)
    જો (strcmp (it->name, name) == 0)
      પાછા it->કિંમત;
  પાછા -1;
}

પૂર્ણાંક
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  માટે (i = 0; i < len; i++)
    {
      માટે (j = i + 1; j < len; j++)
        {
          જો (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            પાછા crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  માટે (i = 0; i < len; i++)
    {
      સંરચના rlimit limit;
      અક્ષર *type = new_rlimits[i]->type;
      પૂર્ણાંક resource = get_rlimit_resource (type);
      જો (UNLIKELY (resource < 0))
        પાછા crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      જો (UNLIKELY (setrlimit (resource, &limit) < 0))
        પાછા crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  પાછા 0;
}

પૂર્ણાંક
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  પૂર્ણાંક has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  પૂર્ણાંક ret;
  જો (def->hostname == ખાલી || def->hostname[0] == '\0')
    પાછા 0;
  જો (! has_uts)
    પાછા crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "sethostname");
  પાછા 0;
}

પૂર્ણાંક
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  પૂર્ણાંક has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  પૂર્ણાંક ret;
  જો (is_empty_string (def->domainname))
    પાછા 0;
  જો (! has_uts)
    પાછા crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "setdomainname");
  પાછા 0;
}

પૂર્ણાંક
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close પૂર્ણાંક fd = -1;
  પૂર્ણાંક ret;
  અક્ષર oom_buffer[16];
  જો (def->process == ખાલી || ! def->process->oom_score_adj_present)
    પાછા 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, કદ (oom_buffer), "%i", def->process->oom_score_adj);
  જો (UNLIKELY (ret >= (પૂર્ણાંક) કદ (oom_buffer)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  જો (fd < 0)
    પાછા fd;
  ret = TEMP_FAILURE_RETRY (લિખ (fd, oom_buffer, strlen (oom_buffer)));
  જો (ret < 0)
    પાછા crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  પાછા 0;
}

અચલ અક્ષર *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  ખાલી
};

સ્થિર પૂર્ણાંક
validate_sysctl (અચલ અક્ષર *original_ચાવી, અચલ અક્ષર *original_કિંમત, અચલ અક્ષર *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  અચલ અક્ષર *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  જો (name[0] == '/' || path_has_dot_dot_component (name))
    પાછા crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_ચાવી);

  જો (has_prefix (name, "fs/mqueue/"))
    {
      જો (namespaces_created & CLONE_NEWIPC)
        પાછા 0;

      namespace = "IPC";
      જાઓ fail;
    }

  જો (has_prefix (name, "kernel/"))
    {
      size_t i;

      માટે (i = 0; sysctlRequiringIPC[i]; i++)
        જો (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            જો (namespaces_created & CLONE_NEWIPC)
              પાછા 0;

            namespace = "IPC";
            જાઓ fail;
          }

      જો (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          જો (! is_empty_string (def->domainname) && (strcmp (original_કિંમત, def->domainname) != 0))
            પાછા crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_ચાવી);

          જો (namespaces_created & CLONE_NEWUTS)
            પાછા 0;

          namespace = "UTS";
          જાઓ fail;
        }

      જો (strcmp (name, "kernel/hostname") == 0)
        પાછા crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_ચાવી);
    }
  જો (has_prefix (name, "net/"))
    {
      જો (namespaces_created & CLONE_NEWNET)
        પાછા 0;

      namespace = "network";
      જાઓ fail;
    }

  પાછા crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_ચાવી);

fail:
  પાછા crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_ચાવી, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
સ્થિર અક્ષર *
sysctl_error_reason (અચલ અક્ષર *name, પૂર્ણાંક namespaces_created, પૂર્ણાંક errno_)
{

  જો (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    પાછા xstrdup ("are all the IDs mapped in the user namespace?");

  પાછા ખાલી;
}

પૂર્ણાંક
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close પૂર્ણાંક dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  જો (def->linux == ખાલી || def->linux->sysctl == ખાલી || def->linux->sysctl->len == 0)
    પાછા 0;

  માટે (i = 0; i < def->linux->namespaces_len; i++)
    {
      પૂર્ણાંક કિંમત;

      કિંમત = libcrun_find_namespace (def->linux->namespaces[i]->type);
      જો (UNLIKELY (કિંમત < 0))
        પાછા crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= કિંમત;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  જો (UNLIKELY (dirfd < 0))
    પાછા dirfd;

  માટે (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free અક્ષર *name = ખાલી;
      cleanup_close પૂર્ણાંક fd = -1;
      પૂર્ણાંક ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      જો (def->linux->sysctl->ચાવીઓ[i] == ખાલી || def->linux->sysctl->કિંમતો[i] == ખાલી)
        પાછા crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_ચાવી_to_proc_path (def->linux->sysctl->ચાવીઓ[i]);

      ret = validate_sysctl (def->linux->sysctl->ચાવીઓ[i], def->linux->sysctl->કિંમતો[i], name, namespaces_created, def, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      જો (UNLIKELY (fd < 0))
        પાછા crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (લિખ (fd, def->linux->sysctl->કિંમતો[i], strlen (def->linux->sysctl->કિંમતો[i])));
      જો (UNLIKELY (ret < 0))
        {
          પૂર્ણાંક saved_errno = errno;
          cleanup_free અક્ષર *reason = ખાલી;

          reason = sysctl_error_reason (def->linux->sysctl->ચાવીઓ[i], namespaces_created, saved_errno);
          પાછા crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
open_terminal (અક્ષર **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક fd = -1;
  uid_t uid = 0;
  પૂર્ણાંક ret;

  જો (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  જો (UNLIKELY (fd < 0))
    પાછા fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  પાછા ret;
}

અક્ષર *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  પાછા get_private_data (container)->external_descriptors;
}

પૂર્ણાંક
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  અચલ અક્ષર *buf = ખાલી;
  json_gen_ctx *gen = ખાલી;
  size_t buf_len;
  પૂર્ણાંક ret;
  પૂર્ણાંક i;

  જો (! json_gen_init (&gen, ખાલી))
    પાછા crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  જો (UNLIKELY (ret != json_gen_status_ok))
    જાઓ json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  માટે (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      અક્ષર link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      જો (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          જો (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          નહીંતર
            {
              json_gen_free (gen);
              પાછા crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      જો (UNLIKELY (ret != json_gen_status_ok))
        જાઓ json_error;
    }

  ret = json_gen_array_close (gen);
  જો (UNLIKELY (ret != json_gen_status_ok))
    જાઓ json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  જો (UNLIKELY (ret != json_gen_status_ok))
    જાઓ json_error;

  જો (buf)
    {
      અક્ષર *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  પાછા 0;

json_error:
  જો (gen)
    json_gen_free (gen);
  પાછા json_gen_error_to_crun_error (ret, err);
}

પૂર્ણાંક
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free અક્ષર *pty = ખાલી;
  cleanup_close પૂર્ણાંક fd = -1;
  પૂર્ણાંક ret;

  જો (def->process == ખાલી || ! def->process->terminal)
    પાછા 0;

  fd = open_terminal (&pty, def->process, err);
  જો (UNLIKELY (fd < 0))
    પાછા fd;

  જો (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", ખાલી, MS_BIND, ખાલી, LABEL_MOUNT, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      ret = unlink ("/dev/console");
      જો (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          જો (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", ખાલી, MS_BIND, ખાલી, LABEL_MOUNT, err);
              જો (UNLIKELY (ret < 0))
                પાછા ret;

              પાછા get_and_reset (&fd);
            }
          પાછા crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  પાછા get_and_reset (&fd);
}

સ્થિર bool
read_error_from_sync_socket (પૂર્ણાંક sync_socket_fd, પૂર્ણાંક *error, અક્ષર **str)
{
  cleanup_free અક્ષર *b = ખાલી;
  size_t size;
  પૂર્ણાંક code = 0;
  પૂર્ણાંક ret;

  જો (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (પઠ (sync_socket_fd, &code, કદ (code)));
      જો (UNLIKELY (ret != (પૂર્ણાંક) કદ (code)))
        પાછા false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (પઠ (sync_socket_fd, &size, કદ (size)));
  જો (UNLIKELY (ret != (પૂર્ણાંક) કદ (size)))
    પાછા false;

  જો (size == 0)
    પાછા false;

  જો (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (પઠ (sync_socket_fd, b, size));
  જો (UNLIKELY (ret < 0))
    પાછા false;

  b[ret] = '\0';

  *str = b;
  b = ખાલી;
  પાછા true;
}

સ્થિર bool
send_error_to_sync_socket (પૂર્ણાંક sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  પૂર્ણાંક ret;
  પૂર્ણાંક code;
  અક્ષર *સંદેશ;

  જો (err == ખાલી || *err == ખાલી)
    પાછા false;

  code = crun_error_get_errno (err);

  જો (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (લિખ (sync_socket_fd, "1", 1));
      જો (UNLIKELY (ret < 0))
        પાછા false;
    }

  ret = TEMP_FAILURE_RETRY (લિખ (sync_socket_fd, &code, કદ (code)));
  જો (UNLIKELY (ret < 0))
    પાછા false;

  સંદેશ = (*err)->સંદેશ;
  size = strlen (સંદેશ) + 1;
  ret = TEMP_FAILURE_RETRY (લિખ (sync_socket_fd, &size, કદ (size)));
  જો (UNLIKELY (ret < 0))
    પાછા false;

  ret = TEMP_FAILURE_RETRY (લિખ (sync_socket_fd, સંદેશ, size));
  જો (UNLIKELY (ret < 0))
    પાછા false;

  પાછા true;
}

સ્થિર અંતઃરેખ પૂર્ણાંક
send_success_to_sync_socket (પૂર્ણાંક sync_socket, libcrun_error_t *err)
{
  અચલ પૂર્ણાંક success = 0;
  પૂર્ણાંક ret;

  ret = TEMP_FAILURE_RETRY (લિખ (sync_socket, &success, કદ (success)));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "write to sync socket");

  પાછા 0;
}

સ્થિર __attribute__ ((noreturn)) શૂન્ય
send_error_to_sync_socket_and_die (પૂર્ણાંક sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  અક્ષર *સંદેશ;

  જો (err == ખાલી || *err == ખાલી)
    _safe_exit (EXIT_FAILURE);

  જો (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      સંદેશ = (*err)->સંદેશ;
      libcrun_fail_with_error (errno, "%s", સંદેશ);
    }
  _safe_exit (EXIT_FAILURE);
}

સ્થિર પૂર્ણાંક
expect_success_from_sync_socket (પૂર્ણાંક sync_fd, libcrun_error_t *err)
{
  cleanup_free અક્ષર *err_str = ખાલી;
  પૂર્ણાંક res = 1;
  પૂર્ણાંક ret;

  ret = TEMP_FAILURE_RETRY (પઠ (sync_fd, &res, કદ (res)));
  જો (UNLIKELY (ret != કદ (res)))
    પાછા crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  જો (res == 0)
    પાછા 0;

  જો (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      જો (! is_empty_string (err_str))
        પાછા crun_make_error (err, res, "%s", err_str);
    }

  પાછા crun_make_error (err, res, "read from sync socket");
}

સ્થિર પૂર્ણાંક
join_namespaces (runtime_spec_schema_config_schema *def, પૂર્ણાંક *namespaces_to_join, પૂર્ણાંક n_namespaces_to_join,
                 પૂર્ણાંક *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  પૂર્ણાંક i;

  માટે (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free અક્ષર *cwd = ખાલી;
      પૂર્ણાંક orig_index = namespaces_to_join_index[i];
      પૂર્ણાંક કિંમત;

      જો (namespaces_to_join[i] < 0)
        ચાલુ;

      /* Skip the user namespace.  */
      કિંમત = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      જો (કિંમત == CLONE_NEWUSER)
        ચાલુ;

      જો (કિંમત == CLONE_NEWNS)
        {
          cwd = getcwd (ખાલી, 0);
          જો (UNLIKELY (cwd == ખાલી))
            પાછા crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], કિંમત);
      જો (UNLIKELY (ret < 0))
        {
          જો (ignore_join_errors)
            ચાલુ;
          પાછા crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      જો (કિંમત == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  પાછા 0;
}

#define MAX_NAMESPACES 10

સંરચના init_status_s
{
  /* fd to the namespace to join.  */
  પૂર્ણાંક fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  પૂર્ણાંક index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  પૂર્ણાંક કિંમત[MAX_NAMESPACES];
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
  પૂર્ણાંક userns_index;
  /* def->linux->namespaces userns.  */
  પૂર્ણાંક userns_index_origin;

  /* All namespaces created/joined by the container.  */
  પૂર્ણાંક all_namespaces;

  /* What namespaces are still missing to be created.  */
  પૂર્ણાંક namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  પૂર્ણાંક idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  પૂર્ણાંક idx_timens_to_join_immediately;
};

શૂન્ય
cleanup_free_init_statusp (સંરચના init_status_s *ns)
{
  size_t i;

  જો (ns == ખાલી)
    પાછા;

  માટે (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

સ્થિર પૂર્ણાંક
configure_init_status (સંરચના init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  માટે (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  માટે (i = 0; i < def->linux->namespaces_len; i++)
    {
      પૂર્ણાંક કિંમત = libcrun_find_namespace (def->linux->namespaces[i]->type);
      જો (UNLIKELY (કિંમત < 0))
        પાછા crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      જો (ns->all_namespaces & કિંમત)
        પાછા crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= કિંમત;

      જો (def->linux->namespaces[i]->path == ખાલી)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= કિંમત;
        }
      નહીંતર
        {
          પૂર્ણાંક fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          જો (ns->fd_len >= MAX_NAMESPACES)
            પાછા crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          જો (UNLIKELY (fd < 0))
            પાછા crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          જો (કિંમત == CLONE_NEWUSER)
            {
              ns->userns_index = ns->fd_len;
              ns->userns_index_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->index[ns->fd_len] = i;
          ns->કિંમત[ns->fd_len] = કિંમત;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  જો (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  પાછા 0;
}

/* Detect if root is available in the container.  */
સ્થિર bool
મૂળ_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  માટે (i = 0; i < len; i++)
    જો (mappings[i]->container_id == 0)
      પાછા true;

  પાછા false;
}

સ્થિર સંરચના libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  સંરચના libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  જો (dev_fds == ખાલી)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  પાછા dev_fds;
}

#define અંક_NEEDED_DEVS (કદ (needed_devs) / કદ (needed_devs[0]) - 1)

સ્થિર સંરચના libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  સંરચના libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  જો (fds == ખાલી)
    {
      fds = make_libcrun_fd_map (અંક_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  પાછા fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  જો (recursive)
    *recursive = false;

  જો (src_nofollow != ખાલી)
    *src_nofollow = false;

  માટે (i = 0; i < mnt->options_len; i++)
    {
      જો (mnt->options[i] == ખાલી)
        ચાલુ;

      જો (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          જો (src_nofollow == ખાલી || *src_nofollow)
            તોડો;
        }
      જો (strcmp (mnt->options[i], "rbind") == 0)
        {
          જો (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          જો (src_nofollow == ખાલી || *src_nofollow)
            તોડો;
        }
      જો (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  પાછા ret;
}

સ્થિર અક્ષર *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  માટે (i = 0; i < mnt->options_len; i++)
    {
      જો (mnt->options[i] == ખાલી)
        ચાલુ;

      જો (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          પાછા mnt->options[i];
        }
      જો (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          પાછા mnt->options[i];
        }
    }
  પાછા ખાલી;
}

સ્થિર પૂર્ણાંક
open_mount_type (અચલ અક્ષર *type, પૂર્ણાંક *out_fd, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક fsopen_fd = -1;
  cleanup_close પૂર્ણાંક newfs_fd = -1;
  પૂર્ણાંક ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  જો (UNLIKELY (fsopen_fd < 0))
    પાછા crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, ખાલી, ખાલી, 0);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  જો (UNLIKELY (newfs_fd < 0))
    પાછા crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  પાછા 0;
}

સ્થિર પૂર્ણાંક
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, પૂર્ણાંક *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  સંરચના mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close પૂર્ણાંક fd = -1;
  અચલ અક્ષર *idmap_option;
  bool recursive = false;
  અચલ અક્ષર *options = ખાલી;
  bool has_mappings;
  પૂર્ણાંક ret;
  અક્ષર *extra_સંદેશ = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != ખાલી);
  જો (has_mappings_out)
    *has_mappings_out = has_mappings;
  જો (! has_mappings)
    પાછા 0;

  જો ((mnt->uid_mappings == ખાલી) != (mnt->gid_mappings == ખાલી))
    પાછા crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  જો (idmap_option)
    {
      options = strchr (idmap_option, '=');
      જો (options)
        {
          /* Skip the '=' itself.  */
          options++;
          જો (options[0] == '\0')
            options = ખાલી;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  જો (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  જો (UNLIKELY (fd < 0))
    પાછા fd;

  જો (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_વૃક્ષ (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      જો (UNLIKELY (newfs_fd < 0))
        પાછા crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  નહીંતર
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno == EINVAL)
        {
          extra_સંદેશ = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      પાછા crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_સંદેશ);
    }

  *out_fd = get_and_reset (&newfs_fd);
  પાછા 0;
}

સ્થિર uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  જો (! def || ! def->linux)
    જાઓ exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  જો (! mappings)
    જાઓ exit;

  માટે (i = 0; i < len; i++)
    {
      જો (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        પાછા id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  પાછા is_uid ? get_overflow_uid () : get_overflow_gid ();
}

સ્થિર પૂર્ણાંક
precreate_device (libcrun_container_t *container, પૂર્ણાંક devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  અક્ષર name[64];
  mode_t type;
  dev_t dev;
  પૂર્ણાંક ret;

  ret = snprintf (name, કદ (name), "%zu", i);
  જો (UNLIKELY (ret >= (પૂર્ણાંક) કદ (name)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "chown `%s`", device->path);

  પાછા get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

સ્થિર પૂર્ણાંક
send_mounts (પૂર્ણાંક sync_socket_host, સંરચના libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  પૂર્ણાંક ret;

  ret = TEMP_FAILURE_RETRY (લિખ (sync_socket_host, &how_many, કદ (how_many)));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "write to sync socket");

  માટે (i = 0; i < total; i++)
    {
      જો (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (અક્ષર *) &i, કદ (i), err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, પૂર્ણાંક sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map સંરચના libcrun_fd_map *mount_fds = ખાલી;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free અક્ષર *rootfs = ખાલી;
  size_t how_many = 0;
  size_t i;
  પૂર્ણાંક ret;

  જો (def->mounts_len == 0)
    પાછા 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  જો (def->મૂળ && def->મૂળ->path)
    rootfs = realpath (def->મૂળ->path, ખાલી);

  જો (! has_userns)
    {
      પૂર્ણાંક is_in_userns = check_running_in_user_namespace (err);
      જો (UNLIKELY (is_in_userns < 0))
        પાછા is_in_userns;

      has_userns = is_in_userns > 0;
    }

  માટે (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      પૂર્ણાંક mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      જો (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == ખાલી)
        {
          unsigned long propagation = 0;

          જો (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, ખાલી, ખાલી, ખાલી, ખાલી) & ALL_PROPAGATIONS_NO_REC;
          જો (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          જો (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      જો (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  પાછા send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

સ્થિર પૂર્ણાંક
prepare_and_send_dev_mounts (libcrun_container_t *container, પૂર્ણાંક sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map સંરચના libcrun_fd_map *dev_fds = ખાલી;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close પૂર્ણાંક current_mountns = -1;
  cleanup_free અક્ષર *state_dir = ખાલી;
  cleanup_free અક્ષર *devs_path = ખાલી;
  cleanup_close પૂર્ણાંક devs_mountfd = -1;
  cleanup_close પૂર્ણાંક targetfd = -1;
  અચલ અક્ષર *context_type = ખાલી;
  અચલ અક્ષર *label = ખાલી;
  size_t how_many = 0;
  size_t i;
  પૂર્ણાંક ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  જો (def->linux == ખાલી || def->linux->devices_len == 0)
    પાછા 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  જો (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    પાછા send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_મૂળ : ખાલી),
                                     (container->context ? container->context->id : ખાલી), err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = mkdir (devs_path, 0700);
  જો (UNLIKELY (ret < 0 && errno != EEXIST))
    પાછા crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  જો (UNLIKELY (current_mountns < 0))
    પાછા current_mountns;

  ret = unshare (CLONE_NEWNS);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (ખાલી, "/", ખાલી, MS_REC | MS_PRIVATE, ખાલી);
  જો (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      જાઓ restore_mountns;
    }

  જો (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      જો (UNLIKELY (context_type == ખાલી))
        {
          ret = -1;
          જાઓ restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", ખાલી, context_type, label, ખાલી);
  જો (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      જાઓ restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      જાઓ restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, ખાલી);
  જો (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      જાઓ restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      જાઓ restore_mountns;
    }

  માટે (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      જો (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ચાલુ;
        }

      dev_fds->fds[i] = ret;

      જો (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  જો (ns_changed && current_mountns >= 0)
    {
      પૂર્ણાંક setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      જો (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          પાછા crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  પાછા ret;
}

સ્થિર પૂર્ણાંક
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, પૂર્ણાંક sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map સંરચના libcrun_fd_map *fds = ખાલી;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (અંક_NEEDED_DEVS);

  જો (! has_userns || geteuid () > 0)
    પાછા send_mounts (sync_socket_host, fds, 0, અંક_NEEDED_DEVS, err);

  માટે (i = 0; i < અંક_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      પૂર્ણાંક fd = syscall_open_વૃક્ષ (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      જો (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  પાછા send_mounts (sync_socket_host, fds, how_many, અંક_NEEDED_DEVS, err);
}

સ્થિર પૂર્ણાંક
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, પૂર્ણાંક sync_socket_host, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
receive_mounts (સંરચના libcrun_fd_map *fds, પૂર્ણાંક sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  પૂર્ણાંક ret;

  જો (fds->nfds == 0)
    પાછા 0;

  ret = TEMP_FAILURE_RETRY (પઠ (sync_socket_container, &how_many, કદ (how_many)));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "read from sync socket");

  માટે (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (અક્ષર *) &index, કદ (index), err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      જો (index >= fds->nfds)
        પાછા crun_make_error (err, 0, "invalid mount data received");

      જો (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  પાછા 0;
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
સ્થિર પૂર્ણાંક
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  જો (! container->host_has_cap_sys_ptrace)
    પાછા 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  પાછા 0;
}

સ્થિર પૂર્ણાંક
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  પૂર્ણાંક ret;

  જો (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool મૂળ_mapped = false;

      જો (def->linux->uid_mappings_len != 0)
        {
          મૂળ_mapped = મૂળ_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          જો (! મૂળ_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      જો (def->linux->gid_mappings_len != 0)
        {
          મૂળ_mapped = મૂળ_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          જો (! મૂળ_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  જો (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    પાછા crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "setresgid to `%d`", gid);

  પાછા 0;
}

સ્થિર bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  પાછા mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, ખાલી, ખાલી);
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
સ્થિર પૂર્ણાંક
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  સંરચના libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close પૂર્ણાંક socket_parent = -1;
  cleanup_close પૂર્ણાંક socket_child = -1;
  cleanup_close પૂર્ણાંક મૂળ_fd = -1;
  cleanup_close પૂર્ણાંક cwd_fd = -1;
  size_t i, missing = 0;
  પૂર્ણાંક saved_errno;
  પૂર્ણાંક sockets[2];
  pid_t pid;
  પૂર્ણાંક ret;

  જો (def->મૂળ == ખાલી || def->મૂળ->path == ખાલી)
    પાછા 0;

  માટે (i = 0; i < def->mounts_len; i++)
    જો (is_proc_mount (def->mounts[i]))
      missing++;
  જો (missing == 0)
    પાછા 1;

  મૂળ_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (મૂળ_fd < 0))
    પાછા crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (cwd_fd < 0))
    પાછા crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->મૂળ->path);
  જો (UNLIKELY (ret < 0))
    પાછા 0;

  pid = fork ();
  saved_errno = errno;
  જો (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      જો (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (શૂન્ય) unshare (CLONE_NEWNS);

      માટે (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close પૂર્ણાંક fd = -1;

          જો (! is_proc_mount (def->mounts[i]))
            ચાલુ;

          fd = open_mount_of_type (container, def->mounts[i], err);
          જો (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (અક્ષર *) &i, કદ (i), err);
          જો (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  જો (UNLIKELY (fchdir (મૂળ_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    પાછા crun_make_error (err, errno, "restore the root directory");

  જો (UNLIKELY (pid < 0))
    પાછા crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  જ્યારે (missing > 0)
    {
      libcrun_error_t tmp_err = ખાલી;

      ret = receive_fd_from_socket_with_payload (socket_parent, (અક્ષર *) &i, કદ (i), &tmp_err);
      જો (ret < 0)
        {
          crun_error_release (&tmp_err);
          તોડો;
        }
      જો (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          તોડો;
        }

      જો (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, ખાલી, 0);

  પાછા missing == 0 ? 1 : 0;
}

સ્થિર પૂર્ણાંક
init_container (libcrun_container_t *container, પૂર્ણાંક sync_socket_container, સંરચના init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  સંરચના libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close પૂર્ણાંક timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  પૂર્ણાંક ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      જો (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      જો (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      જો (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          delayed_pidns_fork = ret > 0;
        }

      જો (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          ret = TEMP_FAILURE_RETRY (લિખ (sync_socket_container, &new_pid, કદ (new_pid)));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "write to sync socket");
        }
      નહીંતર
        {
          new_pid = fork ();
          જો (UNLIKELY (new_pid < 0))
            પાછા crun_make_error (err, errno, "fork");
        }

      જો (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          જો (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (લિખ (sync_socket_container, &new_pid, કદ (new_pid)));
          જો (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = libcrun_set_oom (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  જો ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      માટે (i = 0; i < def->mounts_len; i++)
        {
          પૂર્ણાંક fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          જો (def->mounts[i]->type == ખાલી)
            ચાલુ;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          જો (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, ખાલી, ખાલી, ખાલી);
          જો (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, ખાલી, ખાલી, ખાલી);

          જો (fd >= 0)
            {
              જો (mount_fds->fds[i] >= 0)
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
  જો (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      જો (UNLIKELY (timens_offsets_fd < 0))
        પાછા timens_offsets_fd;
    }

  જો (init_status->all_namespaces & CLONE_NEWUSER)
    {
      જો (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      જો (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
      નહીંતર
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "unshare");
    }

  જો (def->linux->time_offsets)
    {
      અક્ષર fmt_buffer[128];
      પૂર્ણાંક fd = timens_offsets_fd;

      જો (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, કદ (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          જો (UNLIKELY (ret >= (પૂર્ણાંક) કદ (fmt_buffer)))
            પાછા crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long પૂર્ણાંક) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = લિખ (fd, fmt_buffer, strlen (fmt_buffer));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      જો (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long પૂર્ણાંક) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, કદ (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          જો (UNLIKELY (ret >= (પૂર્ણાંક) કદ (fmt_buffer)))
            પાછા crun_make_error (err, 0, "internal error: static buffer too small");

          ret = લિખ (fd, fmt_buffer, strlen (fmt_buffer));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  જો (init_status->must_fork)
    {
      જો (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          delayed_pidns_fork = ret > 0;
        }

      જો (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          જો (UNLIKELY (pid_container < 0))
            પાછા crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      જો (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          ret = TEMP_FAILURE_RETRY (લિખ (sync_socket_container, &pid_container, કદ (pid_container)));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "write to sync socket");

          જો (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  પાછા get_private_data (container)->delayed_pidns_fork;
}

પૂર્ણાંક
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક client_fd = -1;
  cleanup_close પૂર્ણાંક pidfd = -1;
  અચલ અક્ષર *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  જો (v == ખાલી)
    પાછા 0;

  pidfd = syscall_pidfd_open (pid, 0);
  જો (UNLIKELY (pidfd < 0))
    પાછા crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  જો (UNLIKELY (client_fd < 0))
    પાછા client_fd;

  પાછા send_fd_to_socket (client_fd, pidfd, err);
}

સ્થિર bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  જો (process == ખાલી || process->exec_cpu_affinity == ખાલી)
    પાછા false;
  પાછા (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, શૂન્ય *args,
                             પૂર્ણાંક *sync_socket_out, સંરચના libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) સંરચના init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close પૂર્ણાંક sync_socket_container = -1;
  અક્ષર *notify_socket_env = ખાલી;
  cleanup_close પૂર્ણાંક sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close પૂર્ણાંક restore_pidns = -1;
  પૂર્ણાંક first_clone_args = 0;
  પૂર્ણાંક sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  પૂર્ણાંક ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  જો (def->મૂળ)
    {
      ret = do_notify_socket (container, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  જો (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = libcrun_set_oom (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->મૂળ ? container->container_def->મૂળ->path : ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  માટે (i = 0; i < init_status.fd_len; i++)
    {
      પસંદગી (init_status.કિંમત[i])
        {
        સ્થિતિ CLONE_NEWIPC:
          જો (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          તોડો;

        સ્થિતિ CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          જો (! new_userns)
            init_status.must_fork = true;
          નહીંતર
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          તોડો;

        સ્થિતિ CLONE_NEWTIME:
          જો (! new_userns)
            init_status.must_fork = true;
          નહીંતર
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          તોડો;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  જો ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  નહીંતર જો ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  જો (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  જો (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      સંરચના _clone3_args clone3_args;
      memset (&clone3_args, 0, કદ (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      જો (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  જો (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, ખાલી);
      જો (UNLIKELY (pid < 0))
        પાછા crun_make_error (err, errno, "clone");
    }

  જો (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = close_and_reset (&sync_socket_container);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_વૃક્ષ_fd));

      જો (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          ret = TEMP_FAILURE_RETRY (પઠ (sync_socket_host, &new_pid, કદ (new_pid)));
          જો (UNLIKELY (ret != કદ (new_pid)))
            પાછા crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          જો (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          નહીંતર
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, ખાલી, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      જો (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      જો ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      જો (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          ret = TEMP_FAILURE_RETRY (પઠ (sync_socket_host, &grandchild, કદ (grandchild)));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          જો (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          નહીંતર
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, ખાલી, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      જો (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      pid_to_clean = 0;
      પાછા pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  જો (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  જો (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  નહીંતર
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      જો (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->સંદેશ);
    }

  /* Jump into the specified entrypoint.  */
  જો (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  જો (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->સંદેશ);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  જો (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

સ્થિર પૂર્ણાંક
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, પૂર્ણાંક sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, અચલ અક્ષર *sub_cgroup,
                            પૂર્ણાંક *terminal_fd, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  અક્ષર res;
  pid_t pid;
  cleanup_close પૂર્ણાંક sync_fd = sync_socket_fd;

  જો (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (પઠ (sync_fd, &res, 1));
  જો (UNLIKELY (ret != 1))
    પાછા crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  જો (res != '0')
    પાછા crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (પઠ (sync_fd, &pid, કદ (pid)));
  જો (UNLIKELY (ret != કદ (pid)))
    પાછા crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, ખાલી, 0);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "waitpid for exec child pid");

  જો (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (need_move_to_cgroup)
    {
      cleanup_cgroup_status સંરચના libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free અક્ષર *final_cgroup = ખાલી;

      જો (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, ખાલી);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_set_io_priority (pid, process, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (લિખ (sync_fd, &ret, કદ (ret)));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "write to sync socket");

  જો (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      જો (UNLIKELY (ret < 0))
        {
          પૂર્ણાંક err_code = 0;
          cleanup_free અક્ષર *err_str = ખાલી;

          જો (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              જો (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  પાછા crun_make_error (err, err_code, "%s", err_str);
                }
            }

          પાછા crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  પાછા pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
સ્થિર પૂર્ણાંક
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close પૂર્ણાંક pidfd_pid_to_join = -1;
  પૂર્ણાંક all_flags = 0;
  size_t i;
  પૂર્ણાંક ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  જો (def->linux && def->linux->namespaces)
    {
      માટે (i = 0; i < def->linux->namespaces_len; i++)
        જો (! is_empty_string (def->linux->namespaces[i]->path))
          પાછા 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  જો (UNLIKELY (pidfd_pid_to_join < 0))
    પાછા 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  જો (ret == 0)
    પાછા crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  માટે (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].કિંમત;

  ret = setns (pidfd_pid_to_join, all_flags);
  જો (UNLIKELY (ret < 0))
    પાછા 0;

  પાછા 1;
}

સ્થિર પૂર્ણાંક
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  પૂર્ણાંક fds_joined[MAX_NAMESPACES] = {
    0,
  };
  પૂર્ણાંક fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  પૂર્ણાંક ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  /* Nothing left to do if the namespaces were joined.  */
  જો (LIKELY (ret > 0))
    પાછા 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  જો (def->linux->namespaces_len >= MAX_NAMESPACES)
    પાછા crun_make_error (err, 0, "invalid configuration");

  માટે (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free અક્ષર *ns_path = ખાલી;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      જો (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          જો (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ચાલુ;
            }

          ret = fds[i];
          જાઓ exit;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  જો (UNLIKELY (ret < 0))
    જાઓ exit;
  જો (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      જાઓ exit;
    }

  માટે (i = 0; namespaces[i].ns_file; i++)
    {
      જો (namespaces[i].કિંમત == CLONE_NEWUSER)
        ચાલુ;

      ret = setns (fds[i], 0);
      જો (ret == 0)
        fds_joined[i] = 1;
    }
  માટે (i = 0; namespaces[i].ns_file; i++)
    {
      જો (fds_joined[i])
        ચાલુ;
      ret = setns (fds[i], 0);
      જો (ret == 0)
        fds_joined[i] = 1;
    }
  માટે (i = 0; namespaces[i].ns_file; i++)
    {
      જો (fds_joined[i])
        ચાલુ;
      ret = setns (fds[i], 0);
      જો (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          માટે (j = 0; j < def->linux->namespaces_len; j++)
            {
              જો (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  તોડો;
                }
            }
          જો (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              ચાલુ;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          જાઓ exit;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

exit:
  માટે (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  પાછા ret;
}

પૂર્ણાંક
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      અચલ અક્ષર *sub_cgroup,
                      પૂર્ણાંક detach,
                      runtime_spec_schema_config_schema_process *process,
                      પૂર્ણાંક *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  પૂર્ણાંક ret;
  પૂર્ણાંક sync_socket_fd[2];
  cleanup_close પૂર્ણાંક cgroup_dirfd = -1;
  cleanup_close પૂર્ણાંક sync_fd = -1;
  સંરચના _clone3_args clone3_args;
  bool need_move_to_cgroup;

  જો (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "set child subreaper");
    }

  જો (path_has_dot_dot_component (sub_cgroup))
    પાછા crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  જો (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free અક્ષર *final_cgroup = ખાલી;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status સંરચના libcrun_cgroup_status *cgroup_status = ખાલી;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    જો (get_force_cgroup_v1_annotation (container) == ખાલી)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        જો (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, કદ (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  જો (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  નહીંતર
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  જો (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  /* On errors, fall back to fork().  */
  જો (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      જો (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          જાઓ exit;
        }
    }

  જો (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      પાછા join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  જો (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (લિખ (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->સંદેશ);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  જો (UNLIKELY (pid < 0))
    {
      પૂર્ણાંક saved_errno = errno;
      TEMP_FAILURE_RETRY (લિખ (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  જો (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (લિખ (sync_fd, "0", 1));
      જો (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (લિખ (sync_fd, &pid, કદ (pid)));
      જો (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  નહીંતર
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free અક્ષર *pty = ખાલી;
      પૂર્ણાંક r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (પઠ (sync_fd, &r, કદ (r)));
      જો (UNLIKELY (ret != કદ (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      જો (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      જો (terminal_fd)
        {
          cleanup_close પૂર્ણાંક ptmx_fd = -1;

          ret = set_id_init (container, err);
          જો (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          જો (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          જો (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  પાછા 0;

exit:
  જો (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  જો (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  પાછા ret;
}

પૂર્ણાંક
libcrun_linux_container_update (libcrun_container_status_t *status, અચલ અક્ષર *state_મૂળ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status સંરચના libcrun_cgroup_status *cgroup_status = ખાલી;

  cgroup_status = libcrun_cgroup_make_status (status);

  પાછા libcrun_update_cgroup_resources (cgroup_status, state_મૂળ, resources, err);
}

સ્થિર પૂર્ણાંક
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, અચલ bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status સંરચના libcrun_cgroup_status *cgroup_status = ખાલી;

  cgroup_status = libcrun_cgroup_make_status (status);

  પાછા libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

પૂર્ણાંક
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  પાછા libcrun_container_pause_unpause_linux (status, true, err);
}

પૂર્ણાંક
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  પાછા libcrun_container_pause_unpause_linux (status, false, err);
}

પૂર્ણાંક
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  પૂર્ણાંક ret;

  જો (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  નહીંતર જો (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  નહીંતર
    પાછા crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, 0, "personality to `%s`", p->domain);

  પાછા 0;
}

પૂર્ણાંક
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close પૂર્ણાંક sockfd = -1;

  માટે (i = 0; i < def->linux->namespaces_len; i++)
    {
      પૂર્ણાંક કિંમત = libcrun_find_namespace (def->linux->namespaces[i]->type);
      જો (UNLIKELY (કિંમત < 0))
        પાછા crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      જો (કિંમત == CLONE_NEWNET && def->linux->namespaces[i]->path == ખાલી)
        {
          configure_network = true;
          તોડો;
        }
    }

  જો (! configure_network)
    પાછા 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  જો (LIKELY (sockfd >= 0))
    {
      સંરચના ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  નહીંતર
    {
      સંરચના nlmsghdr *hdr_recv;
      અક્ષર buf[કદ (સંરચના nlmsghdr) + કદ (સંરચના nlmsgerr)];
      સંરચના sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      સંરચના sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      સંરચના nlmsghdr hdr = {
        .nlmsg_len = કદ (સંરચના nlmsghdr) + કદ (સંરચના ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      સંરચના ifinfomsg સંદેશ = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((સંરચના nlmsghdr *) buf) = hdr;
      *((સંરચના ifinfomsg *) (buf + કદ (સંરચના nlmsghdr))) = સંદેશ;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      જો (UNLIKELY (sockfd < 0))
        પાછા crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (સંરચના sockaddr *) &addr, કદ (addr));
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, કદ (buf), 0, (સંરચના sockaddr *) &addr_to, કદ (addr_to));
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, કદ (buf), 0, ખાલી, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (સંરચના nlmsghdr *) buf;

      જો (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          સંરચના nlmsgerr *err_data = (સંરચના nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          જો (err_data->error < 0)
            પાછા crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  પાછા 0;
}

/* Protection for attacks like CVE-2019-5736.  */
પૂર્ણાંક ensure_cloned_દ્વિઅંકી ();
__attribute__ ((constructor)) સ્થિર શૂન્ય
libcrun_rexec (શૂન્ય)
{
  જો (ensure_cloned_દ્વિઅંકી () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

પૂર્ણાંક
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  પાછા libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

પૂર્ણાંક
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
સ્થિર પૂર્ણાંક
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, પૂર્ણાંક સંકેત, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  જો (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      /* The pid is not valid anymore, return an error.  */
      જો (ret == 0)
        {
          errno = ESRCH;
          પાછા crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, સંકેત);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "kill container");
  પાછા 0;
}

પૂર્ણાંક
libcrun_kill_linux (libcrun_container_status_t *status, પૂર્ણાંક સંકેત, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_close પૂર્ણાંક pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  જો (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      જો (errno == ENOSYS)
        પાછા libcrun_kill_linux_no_pidfd (status, true, સંકેત, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      જો (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        પાછા crun_make_error (err, ESRCH, "process not running");
      પાછા crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* The pid is not valid anymore, return an error.  */
  જો (ret == 0)
    {
      errno = ESRCH;
      પાછા crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, સંકેત, ખાલી, 0);
  જો (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      જો (errno == ENOSYS)
        પાછા libcrun_kill_linux_no_pidfd (status, false, સંકેત, err);
      પાછા crun_make_error (err, errno, "send signal to pidfd");
    }

  પાછા 0;
}

સ્થિર અચલ અક્ષર *
libcrun_get_intelrdt_name (અચલ અક્ષર *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  જો (def == ખાલી || def->linux == ખાલી || def->linux->intel_rdt == ખાલી || def->linux->intel_rdt->clos_id == ખાલી)
    {
      જો (explicit)
        *explicit = false;
      પાછા ctr_name;
    }

  જો (explicit)
    *explicit = true;
  પાછા def->linux->intel_rdt->clos_id;
}

પૂર્ણાંક
libcrun_apply_intelrdt (અચલ અક્ષર *ctr_name, libcrun_container_t *container, pid_t pid, પૂર્ણાંક actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ખાલી;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  અચલ અક્ષર *name;
  પૂર્ણાંક ret;

  જો (container)
    def = container->container_def;

  જો (def == ખાલી || def->linux == ખાલી || def->linux->intel_rdt == ખાલી)
    પાછા 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  જો (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      જો (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          જો (UNLIKELY (ret < 0))
            જાઓ fail;

          has_monitoring = true;
        }
    }

  જો (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      જો (UNLIKELY (ret < 0))
        જાઓ fail;
    }

  જો (actions & LIBCRUN_INTELRDT_MOVE)
    {
      અચલ અક્ષર *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : ખાલી;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      જો (UNLIKELY (ret < 0))
        જાઓ fail;
    }

  પાછા 0;

fail:
  જો (has_monitoring)
    {
      libcrun_error_t tmp_err = ખાલી;
      પૂર્ણાંક tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      જો (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  જો (created)
    {
      libcrun_error_t tmp_err = ખાલી;
      પૂર્ણાંક tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      જો (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  પાછા ret;
}

પૂર્ણાંક
libcrun_destroy_intelrdt (અચલ અક્ષર *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  અચલ અક્ષર *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  જો (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      પૂર્ણાંક ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      જો (ret < 0)
        પાછા ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  જો (explicit)
    પાછા 0;

  પાછા resctl_destroy (clos_id, err);
}

પૂર્ણાંક
libcrun_update_intel_rdt (અચલ અક્ષર *ctr_name, libcrun_container_t *container, અચલ અક્ષર *l3_cache_schema, અચલ અક્ષર *mem_bw_schema, અક્ષર *અચલ *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ખાલી;
  અચલ અક્ષર *name;

  જો (container)
    def = container->container_def;

  જો (def == ખાલી || def->linux == ખાલી || def->linux->intel_rdt == ખાલી)
    પાછા 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, ખાલી);

  પાછા resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
પૂર્ણાંક
libcrun_safe_chdir (અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *buffer = ખાલી;
  પૂર્ણાંક ret;

  ret = chdir (path);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  જો (ret == 0 || buffer[0] != '/')
    {
      જો (chdir ("/") < 0)
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
#define UNREACHABLE_LEN ((પૂર્ણાંક) કદ (UNREACHABLE) - 1)

      જો ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        પાછા crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      પાછા crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  પાછા 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
સ્થિર પૂર્ણાંક
run_callback_in_namespace (પૂર્ણાંક mntns_fd, પૂર્ણાંક (*callback) (શૂન્ય *, libcrun_error_t *), શૂન્ય *arg, libcrun_error_t *err)
{
  પૂર્ણાંક wait_status = 0;
  pid_t pid;
  પૂર્ણાંક ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  જો (UNLIKELY (pid < 0))
    પાછા crun_make_error (err, errno, "vfork");

  જો (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      જો (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      જો (UNLIKELY (ret < 0))
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
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  પાછા get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
સ્થિર પૂર્ણાંક
check_container_userns (libcrun_container_t *container, પૂર્ણાંક userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close પૂર્ણાંક expected_fd = -1;
  cleanup_close પૂર્ણાંક parent_fd = -1;
  સંરચના stat st, expected_st;
  અચલ અક્ષર *path = ખાલી;
  size_t i;
  પૂર્ણાંક ret;

  જો (def->linux)
    માટે (i = 0; i < def->linux->namespaces_len; i++)
      જો (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == ખાલી;
          તોડો;
        }

  જો (path == ખાલી)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      જો (UNLIKELY (expected_fd < 0))
        પાછા expected_fd;
    }
  નહીંતર
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      જો (UNLIKELY (expected_fd < 0))
        પાછા crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  જો (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      જો (UNLIKELY (parent_fd < 0))
        પાછા crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fstat user namespace");

  જો (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    પાછા crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  પાછા 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
સ્થિર પૂર્ણાંક
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, પૂર્ણાંક *mntns_fd, પૂર્ણાંક *owner_fd, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક mntns = -1;
  cleanup_close પૂર્ણાંક owner = -1;
  પૂર્ણાંક ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  જો (UNLIKELY (mntns < 0))
    પાછા mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (ret == 0)
    પાછા crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  જો (UNLIKELY (owner < 0))
    પાછા crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  પાછા 0;
}

સ્થિર પૂર્ણાંક
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, પૂર્ણાંક (*callback) (શૂન્ય *, libcrun_error_t *), શૂન્ય *arg, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક mntns_fd = -1;
  cleanup_close પૂર્ણાંક owner_fd = -1;
  પૂર્ણાંક ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા run_callback_in_namespace (mntns_fd, callback, arg, err);
}

સંરચના umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
સ્થિર પૂર્ણાંક
umount_nofollow (અચલ અક્ષર *destination, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક parentfd = -1;
  cleanup_close પૂર્ણાંક cwdfd = -1;
  અક્ષર buffer[PATH_MAX];
  અચલ અક્ષર *parent, *name;
  અક્ષર *last_slash;
  size_t len;
  પૂર્ણાંક saved_errno;
  પૂર્ણાંક ret;

  જો (is_empty_string (destination))
    પાછા crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  જો (UNLIKELY (len >= કદ (buffer)))
    પાછા crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  જ્યારે (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  જો (last_slash == ખાલી)
    {
      parent = ".";
      name = buffer;
    }
  નહીંતર જો (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  નહીંતર
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  જો (UNLIKELY (parentfd < 0))
    પાછા crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  જો (UNLIKELY (cwdfd < 0))
    પાછા crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  જો (UNLIKELY (fchdir (cwdfd) < 0))
    પાછા crun_make_error (err, errno, "fchdir to the previous working directory");

  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, saved_errno, "umount `%s`", destination);

  પાછા 0;
}

સ્થિર પૂર્ણાંક
do_umount_in_a_container (શૂન્ય *arg, libcrun_error_t *err)
{
  સંરચના umount_in_a_container_args *args = arg;
  size_t i;
  પૂર્ણાંક ret;

  માટે (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

સંરચના mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  સંરચના libcrun_fd_map *fds;
  પૂર્ણાંક pidfd;
  પૂર્ણાંક owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
સ્થિર પૂર્ણાંક
open_destination_nofollow (અચલ અક્ષર *destination, bool dir, libcrun_error_t *err)
{
  અચલ uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  પૂર્ણાંક fd;
  પૂર્ણાંક ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  જો (LIKELY (fd >= 0))
    પાછા fd;

  જો (errno != ENOENT)
    પાછા crun_make_error (err, errno, "open `%s`", destination);

  જો (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  નહીંતર
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", destination);

  પાછા fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
સ્થિર પૂર્ણાંક
create_destinations_in_a_container (શૂન્ય *arg, libcrun_error_t *err)
{
  સંરચના mount_in_a_container_args *args = arg;
  size_t i;
  પૂર્ણાંક ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  જો (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "setresuid to `0`");
    }
  નહીંતર જો (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      પાછા crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  માટે (i = 0; i < args->len; i++)
    {
      cleanup_close પૂર્ણાંક dest_fd = -1;
      સંરચના stat st;

      ret = fstat (args->fds->fds[i], &st);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      જો (UNLIKELY (dest_fd < 0))
        પાછા dest_fd;
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
do_mount_in_a_container (શૂન્ય *arg, libcrun_error_t *err)
{
  સંરચના mount_in_a_container_args *args = arg;
  size_t i;
  પૂર્ણાંક ret;

  માટે (i = 0; i < args->len; i++)
    {
      cleanup_close પૂર્ણાંક dest_fd = -1;
      સંરચના stat st;

      ret = fstat (args->fds->fds[i], &st);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      જો (UNLIKELY (dest_fd < 0))
        પાછા dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  પાછા 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
સ્થિર પૂર્ણાંક
send_mounts_from_container_namespaces (libcrun_container_t *container, પૂર્ણાંક pidfd, runtime_spec_schema_defs_mount **mounts,
                                       સંરચના libcrun_fd_map *fds, size_t len, પૂર્ણાંક flags, પૂર્ણાંક owner_fd, પૂર્ણાંક sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  પૂર્ણાંક ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  જો (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  નહીંતર જો (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      પાછા crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  જો (UNLIKELY (pid < 0))
    પાછા crun_make_error (err, errno, "fork");

  જો (pid)
    {
      પૂર્ણાંક wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      જો (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, ખાલી, 0));
          પાછા crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  માટે (i = 0; i < len; i++)
    {
      cleanup_close પૂર્ણાંક fd = -1;

      જો (fds->fds[i] >= 0)
        ચાલુ;

      fd = open_mount_of_type (container, mounts[i], err);
      જો (UNLIKELY (fd < 0))
        પાછા crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
સ્થિર પૂર્ણાંક
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, પૂર્ણાંક *flags, libcrun_error_t *err)
{
  સ્થિર અચલ સંરચના
  {
    અચલ અક્ષર *name;
    પૂર્ણાંક flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  માટે (i = 0; i < કદ (namespaces) / કદ (namespaces[0]); i++)
    {
      cleanup_close પૂર્ણાંક self_fd = -1;
      cleanup_close પૂર્ણાંક target_fd = -1;
      સંરચના stat self_st, target_st;
      cleanup_free અક્ષર *self_path = ખાલી;
      પૂર્ણાંક ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      જો (UNLIKELY (self_fd < 0))
        પાછા self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      જો (UNLIKELY (target_fd < 0))
        પાછા target_fd;

      ret = fstat (self_fd, &self_st);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      જો (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  પાછા 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
સ્થિર પૂર્ણાંક
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, પૂર્ણાંક owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, સંરચના libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક sync_fd = -1;
  cleanup_close પૂર્ણાંક pidfd = -1;
  પૂર્ણાંક sync_socket_fd[2];
  bool needed = false;
  પૂર્ણાંક flags;
  pid_t pid;
  size_t i;
  પૂર્ણાંક ret;

  માટે (i = 0; i < len; i++)
    {
      જો (fds->fds[i] >= 0)
        ચાલુ;

      જો (mounts[i]->type == ખાલી)
        પાછા crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  જો (! needed)
    પાછા 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  જો (UNLIKELY (pidfd < 0))
    પાછા crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (ret == 0)
    પાછા crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  જો (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      પાછા ret;
    }

  જો (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      જો (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  માટે (i = 0; i < len; i++)
    {
      જો (fds->fds[i] >= 0)
        ચાલુ;

      ret = receive_fd_from_socket (sync_fd, err);
      જો (UNLIKELY (ret < 0))
        {
          cleanup_free અક્ષર *err_str = ખાલી;
          પૂર્ણાંક err_code = 0;

          જો (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          નહીંતર
            ret = crun_error_wrap (err, "receive mount fd");
          તોડો;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, ખાલી, 0));

  પાછા ret;
}

સ્થિર unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free અક્ષર *data = ખાલી;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  જો (mount->options == ખાલી)
    પાછા get_default_flags (container, mount->destination, &data);

  માટે (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  પાછા flags;
}

પૂર્ણાંક
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  સંરચના mount_in_a_container_args args;
  cleanup_close_map સંરચના libcrun_fd_map *fds = ખાલી;
  cleanup_close પૂર્ણાંક pidfd = -1;
  cleanup_close પૂર્ણાંક mntns_fd = -1;
  cleanup_close પૂર્ણાંક owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  પૂર્ણાંક ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  fds = make_libcrun_fd_map (len);

  માટે (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), ખાલી, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      જો (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          જો (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          જો (UNLIKELY (fds->fds[i] < 0))
            પાછા fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  માટે (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  જો (UNLIKELY (ret > 0))
    પાછા *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

પૂર્ણાંક
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  સંરચના umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  પાછા run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

સંરચના restrict_mounts_in_a_container_args
{
  સંરચના libcrun_mount_flags_s *mounts;
  size_t len;
  પૂર્ણાંક rootfd;
};

સ્થિર પૂર્ણાંક
do_restrict_mounts_in_a_container (શૂન્ય *arg, libcrun_error_t *err)
{
  સંરચના restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  પૂર્ણાંક ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  જો (LIKELY (ret == 0))
    ret = chroot (".");
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "chroot to the container root");

  માટે (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      અચલ અક્ષર *target = consume_slashes (args->mounts[i].target);
      cleanup_close પૂર્ણાંક fd = -1;
      સંરચના statfs sfs;
      unsigned long current_flags;

      જો (flags == 0)
        ચાલુ;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      જો (UNLIKELY (fd < 0))
        {
          જો (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ચાલુ;
            }
          પાછા fd;
        }

      ret = fstatfs (fd, &sfs);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      જો ((current_flags & flags) == flags)
        ચાલુ;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      જો (LIKELY (ret == 0))
        ચાલુ;

      જો (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        પાછા ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (ખાલી, target, ખાલી, MS_REMOUNT | MS_BIND | flags | current_flags, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "remount `/%s`", target);
    }

  પાછા 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
પૂર્ણાંક
libcrun_restrict_mounts_in_container (pid_t pid, સંરચના libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  સંરચના restrict_mounts_in_a_container_args args;
  cleanup_close પૂર્ણાંક mntns_fd = -1;
  cleanup_close પૂર્ણાંક rootfd = -1;
  અક્ષર path[64];
  પૂર્ણાંક ret;

  snprintf (path, કદ (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  જો (UNLIKELY (mntns_fd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, કદ (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  જો (UNLIKELY (rootfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  જો (UNLIKELY (ret > 0))
    પાછા *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  પાછા ret;
}

પૂર્ણાંક
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close પૂર્ણાંક netns_fd = -1;
  size_t i;
  પૂર્ણાંક ret;

  જો (def == ખાલી || def->linux == ખાલી || def->linux->net_devices == ખાલી)
    પાછા 0;

  cleanup_free અક્ષર *ns_path = ખાલી;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  જો (UNLIKELY (netns_fd < 0))
    પાછા netns_fd;

  માટે (i = 0; i < def->linux->net_devices->len; i++)
    {
      અચલ અક્ષર *new_name = def->linux->net_devices->કિંમતો[i]->name ?: def->linux->net_devices->ચાવીઓ[i];

      ret = move_network_device (def->linux->net_devices->ચાવીઓ[i], new_name, netns_fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}
