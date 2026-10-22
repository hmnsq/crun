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

#شامل <config.h>
#شامل "linux.h"
#شامل "utils.h"
#شامل "status.h"
#شامل <string.h>
#شامل <sched.h>
#شامل <fcntl.h>
#شامل <unistd.h>
#شامل <sys/mount.h>
#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#اگر defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#endif

#شامل <sys/prctl.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#شامل <sys/time.h>
#شامل <sys/resource.h>
#شامل <sys/sysmacros.h>
#شامل <sys/types.h>
#شامل <sys/stat.h>
#شامل <grp.h>
#شامل <signal.h>
#شامل "terminal.h"
#شامل "cgroup.h"
#شامل "cgroup-utils.h"
#شامل "status.h"
#شامل "criu.h"
#شامل "scheduler.h"
#شامل "intelrdt.h"
#شامل "io_priority.h"
#شامل "net_device.h"

#شامل <sys/socket.h>
#شامل <libgen.h>
#شامل <sys/wait.h>
#شامل <sys/vfs.h>
#شامل <sys/statvfs.h>
#شامل <limits.h>
#شامل <inttypes.h>
#شامل <sys/personality.h>
#شامل <net/if.h>
#شامل <sys/xattr.h>
#شامل <linux/netlink.h>
#شامل <linux/rtnetlink.h>
#شامل <sched.h>
#شامل <linux/sched.h>
#شامل <linux/magic.h>
#شامل <linux/nsfs.h>

#شامل <json-c/json.h>
#شامل <ocispec/json_common.h>

#شامل "mount_flags.h"
#شامل "syscalls.h"

#ifndef RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#endif

#ifndef OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#endif

#ifndef OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#endif

#ifndef OPEN_درخت_NAMESPACE
#  define OPEN_درخت_NAMESPACE 2
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
ایستا unsigned long
mount_flags_from_statfs (unsigned long f_flags)
{
  unsigned long flags = 0;

  اگر (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  اگر (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  اگر (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  اگر (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  اگر (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  اگر (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  اگر (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  اگر (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  اگر (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  بازگشت flags;
}

ساختار remount_s
{
  ساختار remount_s *next;
  نویسه *target;
  صحیح targetfd;
  unsigned long flags;
  نویسه *data;
};

ساختار private_data_s
{
  ساختار remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  صحیح unshare_flags;
  صحیح unshare_cgroupns;

  نویسه *host_notify_socket_path;
  نویسه *container_notify_socket_path;
  نویسه *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  صحیح procfd;

  ثابت نویسه *rootfs;
  صحیح rootfsfd;

  صحیح notify_socket_درخت_fd;
  صحیح old_ریشه_fd;

  ساختار libcrun_fd_map *mount_fds;
  ساختار libcrun_fd_map *dev_fds;
  ساختار libcrun_fd_map *needed_devs_fds;

  نویسه **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  نویسه *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  صحیح maskdir_fd;
  نویسه *maskdir_proc_path;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_pivot;
  bool no_pivot;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_ریشه_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  نویسه *host_rootfs;
};

ساختار linux_namespace_s
{
  ثابت نویسه *name;
  ثابت نویسه *ns_file;
  صحیح مقدار;
};

ایستا تهی
cleanup_private_data (تهی *private_data)
{
  ساختار private_data_s *p = private_data;

  اگر (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  اگر (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  اگر (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  اگر (p->old_ریشه_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_ریشه_fd));
  اگر (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  اگر (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  اگر (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  اگر (p->copy_symlink_targets)
    {
      size_t i;
      برای (i = 0; i < p->n_copy_symlink_targets; i++)
        آزاد (p->copy_symlink_targets[i]);
      آزاد (p->copy_symlink_targets);
    }
  آزاد (p->unified_cgroup_path);
  آزاد (p->host_notify_socket_path);
  آزاد (p->container_notify_socket_path);
  آزاد (p->external_descriptors);
  آزاد (p->maskdir_proc_path);
  آزاد (p->host_rootfs);
  آزاد (p);
}

ایستا ساختار private_data_s *
get_private_data (ساختار libcrun_container_s *container)
{
  اگر (container->private_data == پوچ)
    {
      ساختار private_data_s *p = xmalloc0 (اندازه (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_درخت_fd = -1;
      p->old_ریشه_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  بازگشت container->private_data;
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

ایستا ساختار linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { پوچ, پوچ, 0 } };

ایستا صحیح
get_and_reset (صحیح *old)
{
  صحیح tmp = *old;
  *old = -1;
  بازگشت tmp;
}

صحیح
libcrun_find_namespace (ثابت نویسه *name)
{
  ساختار linux_namespace_s *it;
  برای (it = namespaces; it->name; it++)
    اگر (strcmp (it->name, name) == 0)
      بازگشت it->مقدار;
  بازگشت -1;
}

ایستا صحیح
syscall_keyctl_join (ثابت نویسه *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  بازگشت (صحیح) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

ایستا صحیح
syscall_pidfd_open (pid_t pid, unsigned صحیح flags)
{
#اگر defined __NR_pidfd_open
  بازگشت (صحیح) syscall (__NR_pidfd_open, pid, flags);
#وگرنه
  (تهی) pid;
  (تهی) flags;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

ایستا صحیح
syscall_pidfd_send_signal (صحیح pidfd, صحیح sig, siginfo_t *info, unsigned صحیح flags)
{
#اگر defined __NR_pidfd_send_signal
  بازگشت (صحیح) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#وگرنه
  (تهی) pidfd;
  (تهی) sig;
  (تهی) info;
  (تهی) flags;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
ایستا uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  اگر (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  اگر (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  اگر (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  اگر (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  اگر (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  اگر (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  اگر (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  اگر (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  بازگشت attr;
}

ایستا صحیح
do_mount_setattr (bool recursive, ثابت نویسه *target, صحیح targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  ساختار mount_attr_s attr = {
    0,
  };
  صحیح ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  اگر (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      اگر (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      وگرنه اگر (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      وگرنه
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      اگر (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  وگرنه اگر (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "mount_setattr `/%s`", target);

  بازگشت 0;
}

صحیح
get_bind_mount (صحیح dirfd, ثابت نویسه *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close صحیح open_درخت_fd = -1;
  ساختار mount_attr_s attr = {
    0,
  };
  صحیح recursive_flag = (recursive ? AT_RECURSIVE : 0);
  صحیح ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  اگر (UNLIKELY (src == پوچ))
    بازگشت crun_make_error (err, EINVAL, "mount source is not specified");

  اگر (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  اگر (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_درخت_fd = syscall_open_درخت (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  اگر (UNLIKELY (open_درخت_fd < 0))
    بازگشت crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_درخت_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "mount_setattr `%s`", src);

  بازگشت get_and_reset (&open_درخت_fd);
}

صحیح
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, نویسه *option, نویسه **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free نویسه *mappings = پوچ;
  ثابت نویسه *it;
  صحیح ret;

  mappings = xmalloc (allocated);

  برای (it = option; *it;)
    {
      bool relative = false;
      long مقدار[3];
      size_t i;

      اگر (*it == '\0')
        بشکن;

      اگر (*it == '#')
        it++;

      اگر (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      برای (i = 0; i < 3; i++)
        {
          نویسه *endptr = پوچ;

          اگر (i > 0 && *it == '-')
            it++;

          اگر (*it == '\0')
            بازگشت crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          مقدار[i] = strtol (it, &endptr, 10);
          اگر (errno || endptr == it)
            بازگشت crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      اگر (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          اگر (def == پوچ
              || def->linux == پوچ
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            بازگشت crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          برای (i = 0; i < mappings_len; i++)
            اگر (مقدار[1] >= (int64_t) mappings[i]->container_id
                && مقدار[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              بشکن;

          اگر (i == mappings_len)
            بازگشت crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          مقدار[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      اگر (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", مقدار[0], مقدار[1], مقدار[2]);
      اگر (UNLIKELY (ret >= (صحیح) (allocated - written)))
        بازگشت crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = پوچ;

  بازگشت 0;
}

ایستا صحیح
format_mount_mappings (نویسه **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  ثابت size_t max_len_mapping = 64;
  cleanup_free نویسه *ret = پوچ;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  برای (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      اگر (UNLIKELY (len >= max_len_mapping))
        بازگشت crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = پوچ;
  بازگشت 0;
}

ایستا صحیح
format_mount_mapping (نویسه **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t size, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .size = size,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    پوچ,
  };

  بازگشت format_mount_mappings (ret, mappings, 1, written, err);
}

ایستا bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  اگر (def->linux == پوچ)
    بازگشت mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  اگر (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    بازگشت false;

  اگر (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    بازگشت false;

  برای (s = 0; s < mnt->uid_mappings_len; s++)
    {
      اگر (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        بازگشت false;
      اگر (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        بازگشت false;
      اگر (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        بازگشت false;
    }

  برای (s = 0; s < mnt->gid_mappings_len; s++)
    {
      اگر (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        بازگشت false;
      اگر (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        بازگشت false;
      اگر (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        بازگشت false;
    }

  بازگشت true;
}

ایستا pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        ثابت نویسه *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != پوچ;

  اگر (! need_new_userns)
    بازگشت 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, پوچ);
  اگر (UNLIKELY (pid < 0))
    بازگشت crun_make_error (err, errno, "clone");

  اگر (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      وقتی (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  اگر (mnt->uid_mappings_len)
    {
      cleanup_free نویسه *uid_map = پوچ;
      cleanup_free نویسه *gid_map = پوچ;
      size_t written = 0;
      صحیح ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      cleanup_close صحیح fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      اگر (UNLIKELY (fd < 0))
        بازگشت fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      اگر (UNLIKELY (fd < 0))
        بازگشت fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه
    {
      cleanup_free نویسه *dup_options = پوچ;
      نویسه *option, *saveptr = پوچ;

      اگر (! options)
        بازگشت crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      برای (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (پوچ, ";", &saveptr))
        {
          cleanup_free نویسه *mappings = پوچ;
          cleanup_close صحیح map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          صحیح ret;

          اگر (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              اگر (UNLIKELY (map_fd < 0))
                بازگشت map_fd;
            }
          وگرنه اگر (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              اگر (UNLIKELY (map_fd < 0))
                بازگشت map_fd;
            }
          وگرنه
            بازگشت crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  بازگشت 0;
}

صحیح
libcrun_create_keyring (libcrun_container_t *container, ثابت نویسه *name, ثابت نویسه *label, libcrun_error_t *err)
{
  cleanup_close صحیح labelfd = -1;
  bool label_set = false;
  صحیح ret;

  اگر (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      اگر (UNLIKELY (labelfd < 0))
        {
          اگر (crun_error_get_errno (err) != ENOENT)
            بازگشت labelfd;

          crun_error_release (err);
        }

      اگر (labelfd >= 0)
        {
          ret = اكتب (labelfd, label, strlen (label));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          برو out;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      برو out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  اگر (label_set && اكتب (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  بازگشت ret;
}

ایستا تهی
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  اگر (def->process && def->process->user)
    {
      اگر (def->process->user->uid)
        *uid = def->process->user->uid;
      اگر (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

ایستا unsigned long
get_mount_flags (ثابت نویسه *name, صحیح current_flags, صحیح *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  ثابت ساختار propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  اگر (found)
    *found = prop ? 1 : 0;

  اگر (prop == پوچ)
    بازگشت 0;

  اگر (prop->extra_flags & OPTION_RECURSIVE)
    {
      اگر (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      اگر (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  اگر (extra_flags)
    *extra_flags |= prop->extra_flags;

  اگر (prop->clear)
    بازگشت current_flags & ~prop->flags;

  بازگشت current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
ایستا bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  برای (i = 0; i < mount->options_len; i++)
    {
      ثابت ساختار propagation_flags_s *prop;

      اگر (mount->options[i] == پوچ)
        ادامه;

      prop = libcrun_str2mount_flags (mount->options[i]);
      اگر (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        بازگشت true;
    }

  بازگشت false;
}

ایستا unsigned long
get_mount_flags_or_option (ثابت نویسه *name, صحیح current_flags, unsigned long *extra_flags, نویسه **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  صحیح found;
  __attribute__ ((unused)) cleanup_free نویسه *prev = پوچ;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  اگر (name == پوچ)
    بازگشت 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  اگر (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      ثابت نویسه *data_flag = پوچ;
      اگر ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      وگرنه اگر ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      اگر (data_flag)
        {
          prev = *option;
          اگر (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          وگرنه
            *option = xstrdup (data_flag);
        }
      بازگشت flags;
    }

  prev = *option;
  اگر (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  وگرنه
    *option = xstrdup (name);

  بازگشت 0;
}

صحیح
pivot_ریشه (ثابت نویسه *new_ریشه, ثابت نویسه *put_old)
{
  بازگشت syscall (__NR_pivot_root, new_ریشه, put_old);
}

ایستا تهی
free_remount (ساختار remount_s *r)
{
  اگر (r == پوچ)
    بازگشت;
  اگر (r->targetfd >= 0)
    close (r->targetfd);
  آزاد (r->data);
  آزاد (r->target);
  آزاد (r);
}

ایستا ساختار remount_s *
make_remount (صحیح targetfd, ثابت نویسه *target, unsigned long flags, ثابت نویسه *data, ساختار remount_s *next)
{
  ساختار remount_s *ret = xmalloc (اندازه (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : پوچ;
  ret->next = next;
  ret->targetfd = targetfd;
  بازگشت ret;
}

ایستا صحیح
do_remount (صحیح targetfd, ثابت نویسه *target, unsigned long flags, ثابت نویسه *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  صحیح ret;
  ثابت نویسه *real_target = target;

  اگر (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      اگر (LIKELY (ret == 0))
        بازگشت 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  اگر (flags & (MS_REMOUNT | MS_RDONLY))
    data = پوچ;

  ret = mount (پوچ, real_target, پوچ, flags, data);
  اگر (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      ساختار statfs sfs;

      ret = statfs (real_target, &sfs);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      اگر (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      اگر ((flags | remount_flags) != flags)
        {
          ret = mount (پوچ, real_target, پوچ, flags | remount_flags, data);
          اگر (LIKELY (ret == 0))
            بازگشت 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          اگر (current_flags & MS_RDONLY)
            ret = mount (پوچ, real_target, پوچ, flags | remount_flags | MS_RDONLY, data);
        }
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "remount `%s`", target);
    }
  بازگشت 0;
}

ایستا صحیح
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret = 0;
  ساختار remount_s *r = get_private_data (container)->remounts;
  وقتی (r)
    {
      ساختار remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      اگر (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          اگر (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              ادامه;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      اگر (UNLIKELY (ret < 0))
        برو cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  وقتی (r)
    {
      ساختار remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = پوچ;
  بازگشت ret;
}

ایستا صحیح
open_mount_target (libcrun_container_t *container, ثابت نویسه *target_rel, libcrun_error_t *err)
{
  ثابت نویسه *rootfs = get_private_data (container)->rootfs;
  صحیح rootfsfd = get_private_data (container)->rootfsfd;

  اگر (rootfsfd < 0)
    بازگشت crun_make_error (err, 0, "invalid rootfs state");

  بازگشت safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
ایستا صحیح
fsopen_mount (ثابت نویسه *type, ثابت نویسه *source_name, ثابت نویسه *labeltype, ثابت نویسه *label, ثابت نویسه *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close صحیح fsfd = -1;
  صحیح ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  اگر (UNLIKELY (fsfd < 0))
    بازگشت fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (تهی) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  اگر (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (data && data[0])
    {
      cleanup_free نویسه *data_copy = xstrdup (data);
      نویسه *saveptr = پوچ;
      نویسه *token;

      برای (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (پوچ, ",", &saveptr))
        {
          نویسه *eq = strchr (token, '=');
          اگر (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          وگرنه
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, پوچ, 0);

          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, پوچ, پوچ, 0);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#وگرنه
  (تهی) type;
  (تهی) source_name;
  (تهی) data;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

ایستا صحیح
get_procfd (ساختار private_data_s *data, libcrun_error_t *err)
{
  صحیح fd;

  اگر (data->procfd >= 0)
    بازگشت data->procfd;

  fd = fsopen_mount ("proc", پوچ, پوچ, پوچ, پوچ);
  اگر (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  بازگشت fd;
}

ایستا صحیح
get_old_ریشه_fd (ساختار private_data_s *data)
{
  اگر (data->old_ریشه_fd >= 0)
    بازگشت data->old_ریشه_fd;

  data->old_ریشه_fd = open ("/", O_PATH | O_CLOEXEC);
  بازگشت data->old_ریشه_fd;
}

ایستا صحیح
fs_move_mount_to (صحیح fd, صحیح dirfd, ثابت نویسه *name)
{
#ifdef HAVE_NEW_MOUNT_API
  اگر (name)
    بازگشت syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  بازگشت syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#وگرنه
  (تهی) fd;
  (تهی) dirfd;
  (تهی) name;
  (تهی) syscall_move_mount;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

شمارش
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

ایستا صحیح do_mount (libcrun_container_t *container, ثابت نویسه *source, صحیح targetfd,
                     ثابت نویسه *target, ثابت نویسه *fstype, unsigned long mountflags,
                     ثابت تهی *data, صحیح extra_flags, libcrun_error_t *err);

ایستا bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, ثابت نویسه *option)
{
  size_t i;
  برای (i = 0; i < mnt->options_len; i++)
    اگر (mnt->options[i] != پوچ && strcmp (mnt->options[i], option) == 0)
      بازگشت true;
  بازگشت false;
}

ایستا runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, ثابت نویسه *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  برای (i = 0; i < def->mounts_len; i++)
    {
      اگر (strcmp (def->mounts[i]->destination, destination) == 0)
        بازگشت def->mounts[i];
    }
  بازگشت پوچ;
}

ایستا تهی
warn_tmpfs_fallback_once (ساختار private_data_s *private_data, ثابت نویسه *reason)
{
  اگر (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
ایستا صحیح
get_shared_empty_dir_cached (libcrun_container_t *container, نویسه **proc_fd_path, libcrun_error_t *err)
{
  ساختار private_data_s *private_data = get_private_data (container);
  cleanup_close صحیح fd = -1;
  cleanup_free نویسه *empty_dir_path = پوچ;
  proc_fd_path_t fd_path;
  صحیح ret;

  /* Fast path: return cached proc fd path if already set up */
  اگر (private_data->maskdir_proc_path != پوچ)
    {
      اگر (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      بازگشت 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_ریشه, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  اگر (fd < 0)
    بازگشت fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  اگر (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  بازگشت 0;
}

ایستا bool
is_dev_null (ثابت ساختار stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  بازگشت S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
ایستا صحیح
ensure_dev_null (صحیح fd, libcrun_error_t *err)
{
  ساختار stat st;

  اگر (UNLIKELY (fstat (fd, &st) < 0))
    بازگشت crun_make_error (err, errno, "stat `/dev/null`");

  اگر (UNLIKELY (! is_dev_null (&st)))
    بازگشت crun_make_error (err, 0, "`/dev/null` is not the null device");

  بازگشت 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
ایستا bool
is_empty_ro_tmpfs (صحیح fd)
{
  ساختار statfs sfs;

  اگر (fstatfs (fd, &sfs) < 0)
    بازگشت false;

  بازگشت sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

ایستا صحیح
mount_masked_dir (libcrun_container_t *container, صحیح pathfd, ثابت نویسه *rel_path, libcrun_error_t *err)
{
  ساختار private_data_s *private_data = get_private_data (container);
  cleanup_close صحیح newfd = -1;
  libcrun_error_t tmp_err = پوچ;
  صحیح ret;

  اگر (private_data->maskdir_fd < 0)
    {
      اگر (private_data->maskdir_bind_failed)
        برو fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, پوچ, &tmp_err);
      اگر (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->پیام);
          crun_error_release (&tmp_err);
          برو fallback_to_tmpfs;
        }
    }

  اگر (private_data->maskdir_fd >= 0)
    {
      صحیح rootfsfd = private_data->rootfsfd;
      ساختار stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, پوچ);
      اگر (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          اگر (UNLIKELY (newfd < 0))
            بازگشت newfd;

          ret = fstat (newfd, &after);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          اگر (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            بازگشت crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          بازگشت 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_path);

      TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
      private_data->maskdir_fd = -1;
      private_data->maskdir_move_failed = true;
    }

  private_data->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_data, tmp_err ? tmp_err->پیام : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  اگر (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      اگر (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      وگرنه
        {
          اگر (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  بازگشت 0;
}

ایستا صحیح
do_masked_or_readonly_path (libcrun_container_t *container, ثابت نویسه *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  ثابت نویسه *rootfs = get_private_data (container)->rootfs;
  cleanup_close صحیح pathfd = -1;
  ساختار statfs sfs;
  صحیح ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  اگر (UNLIKELY (rel_path == پوچ))
    بازگشت crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  اگر (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  اگر (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      اگر (errno != ENOENT && errno != EACCES)
        بازگشت pathfd;

      اگر (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      بازگشت 0;
    }

  اگر (readonly)
    {
      صحیح procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close صحیح mountfd = -1;

      اگر (UNLIKELY (procfd < 0))
        بازگشت procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      اگر (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          اگر (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, پوچ);
          وگرنه
            ret = -1;

          اگر (ret < 0)
            crun_error_release (err);
        }

      اگر (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          اگر (keep_flags)
            {
              cleanup_close صحیح sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              اگر (UNLIKELY (sfd < 0))
                بازگشت crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              اگر (UNLIKELY (ret < 0))
                بازگشت crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          اگر (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              اگر (UNLIKELY (ret < 0))
                بازگشت ret;

              ret = fs_move_mount_to (mountfd, pathfd, پوچ);
              اگر (UNLIKELY (ret < 0))
                بازگشت crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          وگرنه
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, پوچ, mount_flags, پوچ,
                              LABEL_NONE, err);
              اگر (UNLIKELY (ret < 0))
                بازگشت ret;
            }
        }
    }
  وگرنه
    {
      ret = get_file_type_fd (pathfd, &mode);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      اگر ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      وگرنه
        {
          cleanup_close صحیح nullfd = -1;
          cleanup_close صحیح mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          اگر (UNLIKELY (nullfd < 0))
            بازگشت crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          اگر (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, پوچ);

          اگر (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, پوچ, MS_BIND | MS_RDONLY, پوچ,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت 0;
}

ایستا ثابت نویسه *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  ثابت نویسه *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  اگر (context_type)
    {
      اگر (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          بازگشت پوچ;
        }
      بازگشت context_type;
    }

  بازگشت "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
ایستا صحیح
diagnose_mount_failure (صحیح ret, libcrun_error_t *err, libcrun_container_t *container,
                        ثابت نویسه *fstype)
{
  اگر (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      اگر (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          بازگشت ret;
        }
    }
  بازگشت ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
ایستا صحیح
sysfs_userns_fallback (libcrun_container_t *container, ثابت نویسه *target,
                       ثابت نویسه *real_target, صحیح targetfd,
                       unsigned long mountflags, ثابت نویسه *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close صحیح mountfd = -1;
  cleanup_close صحیح sysfd = -1;
  صحیح sys_old_ریشه_fd = -1;
  صحیح ret;

  اگر (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    بازگشت 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (ret == 0)
    بازگشت 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_ریشه_fd = get_old_ریشه_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  اگر (sys_old_ریشه_fd >= 0)
    mountfd = get_bind_mount (sys_old_ریشه_fd, "sys", true, true, false, MS_PRIVATE, err);
  اگر (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  اگر (cgroup_mount == پوچ)
    {
      اگر (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, پوچ);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      وگرنه
        {
          crun_error_release (err);
          اگر (sys_old_ریشه_fd >= 0)
            {
              cleanup_free نویسه *sys_path = پوچ;
              proc_fd_path_t sys_ریشه_path;

              get_proc_self_fd_path (sys_ریشه_path, sys_old_ریشه_fd);
              xasprintf (&sys_path, "%s/sys", sys_ریشه_path);
              ret = mount (sys_path, real_target, پوچ, MS_BIND | MS_REC, پوچ);
            }
          وگرنه
            ret = mount ("/sys", real_target, پوچ, MS_BIND | MS_REC, پوچ);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      اگر (UNLIKELY (sysfd < 0))
        بازگشت sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        پوچ, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      بازگشت do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  اگر (UNLIKELY (mountfd < 0))
    بازگشت mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  اگر (UNLIKELY (sysfd < 0))
    بازگشت sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
ایستا تهی
apply_selinux_xattr (libcrun_container_t *container, صحیح fd, صحیح extra_flags, ثابت نویسه *label)
{
  libcrun_error_t tmp_err = پوچ;
  صحیح procfd;

  اگر ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    بازگشت;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  اگر (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close صحیح xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      اگر (xfd >= 0)
        (تهی) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
ایستا صحیح
do_mount_replace_rootfs (libcrun_container_t *container, صحیح *fd, libcrun_error_t *err)
{
  صحیح procfd = get_procfd (get_private_data (container), err);
  صحیح tmp;
  صحیح ret;

  اگر (UNLIKELY (procfd < 0))
    بازگشت procfd;

  {
    cleanup_close صحیح nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    اگر (UNLIKELY (nsfd < 0))
      بازگشت crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    اگر (UNLIKELY (ret < 0))
      بازگشت crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (*fd < 0))
    بازگشت crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  اگر (UNLIKELY (tmp < 0))
    بازگشت crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  بازگشت 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
ایستا صحیح
apply_propagation_flags (ثابت نویسه *target, صحیح targetfd, ثابت نویسه *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  صحیح ret;

  اگر (targetfd >= 0)
    {
      libcrun_error_t tmp_err = پوچ;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      اگر (LIKELY (ret == 0))
        propagation_done = true;
      وگرنه
        crun_error_release (&tmp_err);
    }

  اگر (! propagation_done)
    {
      ret = mount (پوچ, real_target, پوچ, mountflags & ALL_PROPAGATIONS, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  بازگشت 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
ایستا صحیح
schedule_or_do_remount (libcrun_container_t *container, صحیح *fd, صحیح targetfd,
                        ثابت نویسه *target, ثابت نویسه *real_target,
                        bool single_instance, unsigned long mountflags,
                        ثابت تهی *data, صحیح extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  ساختار remount_s *r;

  اگر ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    بازگشت do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  اگر (*fd < 0)
    {
      *fd = dup (targetfd);
      اگر (UNLIKELY (*fd < 0))
        بازگشت crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  بازگشت 0;
}

ایستا صحیح
do_mount (libcrun_container_t *container, ثابت نویسه *source, صحیح targetfd,
          ثابت نویسه *target, ثابت نویسه *fstype, unsigned long mountflags, ثابت تهی *data,
          صحیح extra_flags, libcrun_error_t *err)
{
  cleanup_free نویسه *data_with_label = پوچ;
  ثابت نویسه *data_without_label = data;
  proc_fd_path_t target_buffer;
  ثابت نویسه *context_type = پوچ;
  cleanup_close صحیح ms_move_fd = -1;
  ثابت نویسه *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close صحیح fd = -1;
  ثابت نویسه *label = پوچ;
  صحیح ret = 0;

  اگر (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  وگرنه
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  اگر (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  اگر ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      اگر (UNLIKELY (context_type == پوچ))
        بازگشت -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      اگر (ret < 0)
        بازگشت ret;
      data = data_with_label;
    }

  اگر (mountflags & MS_MOVE)
    {
      اگر ((mountflags & MS_BIND) || fstype)
        بازگشت crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, پوچ, MS_MOVE, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      اگر (UNLIKELY (ms_move_fd < 0))
        بازگشت ms_move_fd;
      targetfd = ms_move_fd;
    }

  اگر ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      اگر (targetfd >= 0)
        {
          اگر (mountflags & MS_BIND)
            {
              cleanup_close صحیح bindfd = -1;
              libcrun_error_t tmp_err = پوچ;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              اگر (bindfd < 0)
                {
                  صحیح old_ریشه_fd = get_old_ریشه_fd (get_private_data (container));

                  اگر (old_ریشه_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_ریشه_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              اگر (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, پوچ);
              اگر (ret < 0)
                crun_error_release (&tmp_err);
            }
          وگرنه
            {
              cleanup_close صحیح newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              اگر (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, پوچ);
            }
        }

      اگر (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          اگر (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              صحیح old_ریشه_fd = get_old_ریشه_fd (get_private_data (container));

              اگر (old_ریشه_fd >= 0)
                {
                  cleanup_free نویسه *proc_source_buf = پوچ;
                  proc_fd_path_t old_ریشه_path;

                  get_proc_self_fd_path (old_ریشه_path, old_ریشه_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_ریشه_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      اگر (UNLIKELY (ret < 0))
        {
          صحیح saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          اگر (ret == 0)
            بازگشت 0;
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          بازگشت diagnose_mount_failure (ret, err, container, fstype);
        }

      اگر (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          اگر (UNLIKELY (fd < 0))
            بازگشت fd;

          /* We are replacing the rootfs, reopen it.  */
          اگر (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              اگر (UNLIKELY (ret < 0))
                بازگشت ret;
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

  اگر (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  اگر (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  اگر (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت ret;
}

ایستا تهی
try_umount (صحیح targetfd, ثابت نویسه *target)
{
  proc_fd_path_t target_buffer;
  ثابت نویسه *real_target = target;

  اگر (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

ایستا bool
container_has_cgroupns (libcrun_container_t *container)
{
  بازگشت get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

ایستا صحیح
do_mount_cgroup_v2 (libcrun_container_t *container, صحیح targetfd, ثابت نویسه *target,
                    unsigned long mountflags, ثابت نویسه *data, libcrun_error_t *err)
{
  صحیح ret;
  صحیح cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  اگر (UNLIKELY (ret < 0))
    {
      صحیح saved_errno = crun_error_get_errno (err);
      اگر (saved_errno == EPERM || saved_errno == EBUSY)
        {
          ثابت نویسه *unified_cgroup_path;
          ثابت نویسه *src_cgroup;

          crun_error_release (err);

          اگر (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              اگر (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  اگر (LIKELY (ret == 0))
                    بازگشت ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_ریشه;
          بازگشت do_mount (container, src_cgroup, targetfd, target, پوچ,
                           MS_BIND | mountflags, پوچ, LABEL_NONE, err);
        }

      بازگشت ret;
    }

  بازگشت 0;
}

ایستا صحیح
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, ثابت نویسه *source, صحیح targetfd, ثابت نویسه *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_close صحیح fd = -1;
  ثابت نویسه *subsystem = "systemd";
  cleanup_free نویسه *subsystem_path = پوچ;
  cleanup_close صحیح tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  اگر (UNLIKELY (tmpfsdirfd < 0))
    بازگشت tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "openat `%s`", subsystem_path);

  بازگشت do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

ایستا صحیح
do_mount_cgroup_v1 (libcrun_container_t *container, ثابت نویسه *source, صحیح targetfd, ثابت نویسه *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free نویسه *content = پوچ;
  نویسه *from;
  cleanup_close صحیح tmpfsdirfd = -1;
  نویسه *saveptr = پوچ;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  اگر (UNLIKELY (tmpfsdirfd < 0))
    بازگشت tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    صحیح procfd = get_procfd (get_private_data (container), err);
    اگر (UNLIKELY (procfd < 0))
      بازگشت procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, پوچ, err);
  }
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (UNLIKELY (content == پوچ || content[0] == '\0'))
    بازگشت crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  برای (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (پوچ, "\n", &saveptr))
    {
      cleanup_free نویسه *source_path = پوچ;
      cleanup_free نویسه *source_subsystem = پوچ;
      cleanup_free نویسه *subsystem_path = پوچ;
      نویسه *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close صحیح subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      اگر (subsystem[0] == '\0')
        ادامه;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      اگر (it)
        subsystem = it + 5;

      اگر (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      اگر (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_ریشه, subsystem, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      /* if there is already a mount specified, do not add a default one.  */
      اگر (find_mount_for (container, source_subsystem) != پوچ)
        ادامه;

      ret = append_paths (&source_path, err, source_subsystem, subpath, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      اگر (UNLIKELY (subsystemfd < 0))
        بازگشت crun_make_error (err, errno, "open `%s`", subsystem_path);

      اگر (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  ادامه;
                }
              بازگشت ret;
            }
        }
      وگرنه
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, پوچ, MS_BIND | mountflags, پوچ,
                          LABEL_NONE, err);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (crun_error_get_errno (err) != ENOENT)
                بازگشت ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, پوچ, MS_BIND | mountflags,
                              پوچ, LABEL_NONE, err);
              اگر (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  اگر (crun_error_get_errno (err) != ENOENT)
                    بازگشت ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

ایستا صحیح
do_mount_cgroup (libcrun_container_t *container, ثابت نویسه *source, صحیح targetfd, ثابت نویسه *target,
                 unsigned long mountflags, ثابت نویسه *data, libcrun_error_t *err)
{
  صحیح cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  انتخاب (cgroup_mode)
    {
    حالت CGROUP_MODE_UNIFIED:
      بازگشت do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    حالت CGROUP_MODE_LEGACY:
    حالت CGROUP_MODE_HYBRID:
      بازگشت do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  بازگشت crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

ساختار device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
ایستا تهی
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  ساختار private_data_s *private_data = get_private_data (container);
  size_t i;

  اگر (private_data->needed_devs_fds)
    {
      ساختار libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      برای (i = 0; needed_devs[i].path; i++)
        {
          اگر (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            ادامه;

          صحیح fd = syscall_open_درخت (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          اگر (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          وگرنه اگر (fd >= 0)
            close (fd);
        }
    }

  اگر (private_data->notify_socket_درخت_fd < 0
      && private_data->host_notify_socket_path)
    {
      صحیح fd = syscall_open_درخت (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      اگر (fd >= 0)
        private_data->notify_socket_درخت_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
ایستا ثابت نویسه *
relative_path_under_dev (ثابت نویسه *path)
{
  اگر (path[0] != '/')
    بازگشت path;
  اگر (strncmp (path, "/dev/", 5) == 0)
    {
      اگر (strchr (path + 5, '/'))
        بازگشت پوچ;
      بازگشت path + 5;
    }
  بازگشت پوچ;
}

ایستا صحیح
mknod_and_set_attrs (libcrun_container_t *container, صحیح dirfd, ثابت نویسه *name, ساختار device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  proc_fd_path_t fd_path;
  ثابت نویسه *what = "new";
  bool exists = false;
  ساختار stat st;
  صحیح procfd;
  صحیح ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno != EEXIST)
        بازگشت crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  اگر (UNLIKELY ((st.st_mode & S_IFMT) != type))
    بازگشت crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  اگر (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    بازگشت crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  اگر (exists)
    بازگشت 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  اگر (UNLIKELY (procfd < 0))
    بازگشت procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fchown `%s`", device->path);

  بازگشت 1;
}

صحیح
libcrun_create_dev (libcrun_container_t *container, صحیح devfd, صحیح srcfd,
                    ساختار device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  صحیح ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  ثابت نویسه *fullname = device->path;
  ثابت نویسه *rootfs = get_private_data (container)->rootfs;
  اگر (is_empty_string (fullname))
    بازگشت crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free نویسه *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  اگر (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  ثابت نویسه *rel_dev = relative_path_under_dev (normalized_path);

  اگر (binds)
    {
      cleanup_close صحیح fd = -1;

      اگر (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          اگر (UNLIKELY (fd < 0))
            {
              اگر (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              اگر (UNLIKELY (fd < 0))
                بازگشت crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      وگرنه
        {
          ثابت نویسه *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          اگر (UNLIKELY (fd < 0))
            بازگشت fd;
        }

      اگر (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          اگر (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              اگر (LIKELY (ret >= 0))
                بازگشت 0;
            }
          وگرنه
            crun_error_release (err);
        }

      {
        cleanup_close صحیح mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        اگر (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            اگر (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, پوچ);
                اگر (LIKELY (ret == 0))
                  بازگشت 0;
              }
            وگرنه
              crun_error_release (err);
          }
        وگرنه
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, پوچ, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, پوچ, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      اگر (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          اگر (ret <= 0)
            بازگشت ret;
        }
      وگرنه
        {
          cleanup_close صحیح dirfd = -1;
          cleanup_free نویسه *dirname = پوچ;
          نویسه *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          اگر (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          اگر (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              اگر (UNLIKELY (dirfd < 0))
                بازگشت crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          وگرنه
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              اگر (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              اگر (UNLIKELY (dirfd < 0))
                بازگشت dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          اگر (ret <= 0)
            بازگشت ret;
        }
    }
  بازگشت 0;
}

ساختار symlink_s
{
  ثابت نویسه *path;
  ثابت نویسه *target;
  bool force;
};

ایستا ساختار symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { پوچ, پوچ, false } };

ایستا صحیح
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  صحیح ret;
  size_t i;
  ساختار device_s *it;
  cleanup_close صحیح devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت نویسه *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map ساختار libcrun_fd_map *dev_fds = پوچ;
  cleanup_close_map ساختار libcrun_fd_map *needed_fds = پوچ;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = پوچ;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = پوچ;

  اگر (! def || ! def->linux)
    بازگشت 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  اگر (UNLIKELY (devfd < 0))
    بازگشت devfd;

  برای (i = 0; i < def->linux->devices_len; i++)
    {
      ساختار device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      اگر (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  برای (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close صحیح srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      اگر (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  برای (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      اگر (UNLIKELY (ret < 0))
        {
          صحیح saved_errno = errno;

          اگر (errno == EEXIST && ! symlinks[i].force)
            ادامه;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          اگر (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              اگر (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              اگر (ret < 0 && errno == EBUSY)
                {
                  cleanup_close صحیح tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  اگر (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      اگر (umount2 (procpath, MNT_DETACH) == 0)
                        برو retry_unlink;
                    }
                }
              اگر (ret == 0)
                برو retry_symlink;
            }
          بازگشت crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  اگر (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
ایستا ثابت نویسه *
next_path_component (ثابت نویسه *p, size_t *len)
{
  برای (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      اگر (*len != 1 || p[0] != '.')
        بازگشت p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
ایستا bool
same_path (ثابت نویسه *a, ثابت نویسه *b)
{
  size_t la, lb;

  برای (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      اگر (la != lb || memcmp (a, b, la) != 0)
        بازگشت false;
      اگر (la == 0)
        بازگشت true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
ایستا bool
is_duplicate_path (نویسه **paths, size_t i)
{
  size_t j;

  اگر (paths[i] == پوچ)
    بازگشت false;

  برای (j = 0; j < i; j++)
    اگر (paths[j] && same_path (paths[j], paths[i]))
      بازگشت true;

  بازگشت false;
}

ایستا صحیح
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  صحیح ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  اگر (! def || ! def->linux)
    بازگشت 0;

  برای (i = 0; i < def->linux->masked_paths_len; i++)
    {
      اگر (is_duplicate_path (def->linux->masked_paths, i))
        ادامه;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  برای (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت 0;
}

ایستا صحیح
do_pivot (libcrun_container_t *container, ثابت نویسه *rootfs, libcrun_error_t *err)
{
  cleanup_close صحیح oldrootfd = -1;
  cleanup_close صحیح newrootfd = -1;
  صحیح ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (oldrootfd < 0))
    بازگشت crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (newrootfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_ریشه (".", ".");
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, پوچ, -1, ".", پوچ, MS_REC | MS_PRIVATE, پوچ, LABEL_MOUNT, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = umount2 (".", MNT_DETACH);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "umount2 oldroot");

  انجامبده
    {
      ret = umount2 (".", MNT_DETACH);
      اگر (ret < 0 && errno == EINVAL)
        بشکن;
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "umount2 oldroot");
  } وقتی (ret == 0);

  ret = chdir ("/");
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "chdir to newroot");

  بازگشت 0;
}

ایستا صحیح
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, نویسه **data, libcrun_error_t *err)
{
  ثابت نویسه *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close صحیح fd = -1;
  ساختار stat st;
  صحیح ret;

  اگر (*data != پوچ && strstr (*data, "mode="))
    بازگشت 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  اگر (fd < 0)
    {
      اگر (crun_error_get_errno (err) != ENOENT)
        بازگشت fd;

      crun_error_release (err);
      بازگشت 0;
    }
  ret = fstat (fd, &st);
  اگر (ret < 0)
    بازگشت crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  بازگشت 0;
}

ایستا صحیح
get_default_flags (libcrun_container_t *container, ثابت نویسه *destination, نویسه **data)
{
  اگر (strcmp (destination, "/proc") == 0)
    بازگشت 0;
  اگر (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      بازگشت MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  اگر (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      بازگشت MS_NOEXEC | MS_STRICTATIME;
    }
  اگر (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      بازگشت MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  اگر (strcmp (destination, "/dev/mqueue") == 0)
    بازگشت MS_NOEXEC | MS_NOSUID | MS_NODEV;
  اگر (strcmp (destination, "/dev/pts") == 0)
    {
      اگر (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      وگرنه
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      بازگشت MS_NOEXEC | MS_NOSUID;
    }
  اگر (strcmp (destination, "/sys") == 0)
    بازگشت MS_NOEXEC | MS_NOSUID | MS_NODEV;

  بازگشت 0;
}

ایستا ثابت نویسه *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  بازگشت find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

ایستا صحیح
check_valid_no_follow_file_system (صحیح fd, ثابت نویسه *destination, libcrun_error_t *err)
{
  ساختار statfs sfs;

  صحیح ret = fstatfs (fd, &sfs);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "statfs `%s`", destination);

  انتخاب (sfs.f_type)
    {
    حالت PROC_SUPER_MAGIC:
    حالت SYSFS_MAGIC:
    حالت DEVPTS_SUPER_MAGIC:
      بازگشت crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  بازگشت 0;
}

ایستا صحیح
safe_create_symlink (صحیح rootfsfd, ثابت نویسه *rootfs, ثابت نویسه *target, ثابت نویسه *destination, libcrun_error_t *err)
{
  cleanup_close صحیح parent_dir_fd = -1;
  cleanup_free نویسه *buffer = پوچ;
  نویسه *part;
  صحیح ret;

  اگر (is_empty_string (destination))
    بازگشت crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  اگر (UNLIKELY (parent_dir_fd < 0))
    بازگشت parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  اگر (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      اگر (errno == EEXIST)
        {
          cleanup_free نویسه *link = پوچ;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          اگر (UNLIKELY (len < 0))
            بازگشت len;

          اگر ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            بازگشت 0;

          بازگشت crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      بازگشت crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  بازگشت 0;
}

ایستا صحیح
handle_copy_symlink (libcrun_container_t *container, ثابت نویسه *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     ثابت نویسه *link_target, libcrun_error_t *err)
{
  cleanup_free نویسه *target = پوچ;

  اگر (link_target == پوچ)
    {
      صحیح old_ریشه_fd = get_old_ریشه_fd (get_private_data (container));
      ثابت نویسه *source = mount->source;
      ssize_t len;

      اگر (old_ریشه_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_ریشه_fd, source + 1, &target, 0, err);
      وگرنه
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      اگر (UNLIKELY (len < 0))
        بازگشت len;
      link_target = target;
    }

  بازگشت safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

ایستا صحیح
prepare_sysfs_or_proc_mount (libcrun_container_t *container, ثابت نویسه *target,
                             صحیح *targetfd, libcrun_error_t *err)
{
  صحیح ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOENT)
        {
          اگر (strchr (target, '/'))
            بازگشت crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      وگرنه اگر (errno == ENOTDIR)
        بازگشت crun_make_error (err, errno, "the target `/%s` is invalid", target);

      اگر (ret < 0)
        بازگشت crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  بازگشت 0;
}

ایستا صحیح
handle_tmpcopyup (libcrun_container_t *container, ثابت نویسه *rootfs, ثابت نویسه *target,
                  صحیح copy_from_fd, libcrun_error_t *err)
{
  صحیح destfd, ret;
  cleanup_close صحیح tmpfd = copy_from_fd;

  target = consume_slashes (target);

  اگر (is_empty_string (target))
    بازگشت crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  اگر (UNLIKELY (destfd < 0))
    بازگشت crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  بازگشت ret;
}

ایستا صحیح
get_mount_label_how (ثابت نویسه *type, bool is_sysfs_or_proc)
{
  اگر (is_sysfs_or_proc)
    بازگشت LABEL_NONE;

  اگر (strcmp (type, "mqueue") == 0)
    بازگشت LABEL_XATTR;

  بازگشت LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
ایستا unsigned long
get_bind_mount_locked_flags (صحیح source_mountfd, ثابت نویسه *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  ساختار statfs sfs;
  size_t i;
  صحیح ret;

  اگر (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  وگرنه
    ret = statfs (source, &sfs);
  اگر (UNLIKELY (ret < 0))
    بازگشت 0;

  برای (i = 0; i < mount->options_len; i++)
    {
      ثابت ساختار propagation_flags_s *prop;

      اگر (mount->options[i] == پوچ)
        ادامه;

      prop = libcrun_str2mount_flags (mount->options[i]);
      اگر (prop == پوچ || (prop->extra_flags & OPTION_RECURSIVE))
        ادامه;

      اگر (prop->clear)
        cleared |= prop->flags;
      وگرنه
        cleared &= ~prop->flags;
    }

  بازگشت sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
ایستا ثابت نویسه *
get_path_in_rootfs (ثابت نویسه *rootfs, ثابت نویسه *path)
{
  size_t len;

  اگر (rootfs == پوچ || path == پوچ)
    بازگشت پوچ;

  len = strlen (rootfs);
  وقتی (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  اگر (len == 0)
    بازگشت پوچ;

  اگر (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    بازگشت پوچ;

  بازگشت consume_slashes (path + len);
}

ایستا صحیح
process_single_mount (libcrun_container_t *container, ثابت نویسه *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      ساختار libcrun_fd_map *mount_fds, size_t mount_index,
                      ثابت نویسه *systemd_cgroup_v1, libcrun_error_t *err)
{
  ثابت نویسه *target = consume_slashes (mount->destination);
  cleanup_close صحیح source_mountfd = -1;
  cleanup_free نویسه *data = پوچ;
  نویسه *type;
  نویسه *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close صحیح copy_from_fd = -1;
  cleanup_close صحیح targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  صحیح no_remount = 0;
  صحیح ret;

  اگر (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  اگر (mount->options == پوچ)
    flags = get_default_flags (container, mount->destination, &data);
  وگرنه
    {
      size_t j;

      برای (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  اگر (type == پوچ && (flags & MS_BIND) == 0)
    بازگشت crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  اگر ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      ثابت نویسه *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      اگر (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          اگر (UNLIKELY (source_mountfd < 0))
            بازگشت crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  اگر ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  اگر (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      اگر (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      اگر (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  اگر (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      ثابت نویسه *path = mount->source;

      اگر ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        بازگشت crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      اگر (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, پوچ);
        }
      وگرنه اگر (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      وگرنه اگر (path[0] == '/')
        {
          صحیح old_ریشه_fd = get_old_ریشه_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          اگر (old_ریشه_fd >= 0)
            ret = get_file_type_at (old_ریشه_fd, &src_mode, nofollow, (path[1] == '\0') ? پوچ : path + 1);
          وگرنه
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      وگرنه
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "cannot stat `%s`", path);

      اگر (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          صحیح bind_dirfd = AT_FDCWD;
          ثابت نویسه *bind_source = mount->source;
          صحیح old_ریشه_fd = get_old_ریشه_fd (get_private_data (container));

          اگر (old_ریشه_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_ریشه_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          source_mountfd = ret;
        }
    }

  اگر (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      ثابت نویسه *cached_target = پوچ;

      اگر (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      mounted = true;
    }
  وگرنه اگر (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه
    {
      bool is_dir = S_ISDIR (src_mode);

      اگر (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
      وگرنه
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          targetfd = ret;
        }
    }

  اگر (extra_flags & OPTION_TMPCOPYUP)
    {
      اگر (strcmp (type, "tmpfs") != 0)
        بازگشت crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      اگر (UNLIKELY (copy_from_fd < 0))
        {
          اگر (errno != ENOTDIR)
            بازگشت crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  اگر (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, پوچ);
      اگر (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          اگر (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, پوچ, source_mountfd, target, پوچ, remaining_flags, data, LABEL_NONE | no_remount, err);
              اگر (UNLIKELY (ret < 0))
                بازگشت ret;
            }

          mounted = true;

          اگر (is_empty_string (target))
            {
              صحیح new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              اگر (UNLIKELY (new_rootfsfd < 0))
                بازگشت crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      وگرنه اگر (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          بازگشت crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      وگرنه اگر (get_private_data (container)->host_ریشه_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          بازگشت crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      وگرنه اگر (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  اگر (! mounted)
    {
      اگر (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
      وگرنه اگر (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
      وگرنه
        {
          صحیح extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }

  اگر (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (rec_clear || rec_set)
    {
      ثابت bool is_dir = S_ISDIR (src_mode);
      cleanup_close صحیح dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      اگر (UNLIKELY (dfd < 0))
        بازگشت crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

ایستا صحیح
do_mounts (libcrun_container_t *container, ثابت نویسه *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت نویسه *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map ساختار libcrun_fd_map *mount_fds = پوچ;
  size_t i;
  صحیح ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = پوچ;

  برای (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
صحیح
libcrun_container_do_bind_mount (libcrun_container_t *container, نویسه *mount_source, نویسه *mount_destination, نویسه **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  ثابت نویسه *target = consume_slashes (mount_destination);
  cleanup_free نویسه *data = پوچ;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close صحیح targetfd = -1;
  صحیح is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  ثابت نویسه *rootfs = get_private_data (container)->rootfs;
  صحیح ret;

  اگر ((get_private_data (container)->rootfsfd < 0) || (rootfs == پوچ))
    بازگشت crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  اگر (mount_options == پوچ)
    flags = get_default_flags (container, mount_destination, &data);
  وگرنه
    {
      size_t j;

      برای (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  اگر (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  اگر (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      اگر (UNLIKELY (is_dir < 0))
        بازگشت is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
صحیح
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, صحیح *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close صحیح notify_fd = -1;
  cleanup_free نویسه *host_notify_socket_path = پوچ;
  cleanup_free نویسه *state_dir = پوچ;
  نویسه *host_path = پوچ;
  صحیح ret;

  اگر (container && get_private_data (container)->host_notify_socket_path)
    {
      ثابت نویسه *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  اگر (host_path == پوچ)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_ریشه, context->id, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  اگر (UNLIKELY (notify_fd < 0))
    بازگشت notify_fd;

  اگر (UNLIKELY (chmod (host_path, 0777) < 0))
    بازگشت crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  اگر (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (تهی) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  بازگشت 1;
#وگرنه
  (تهی) context;
  (تهی) container;
  (تهی) err;
  *notify_socket_out = -1;
  بازگشت 0;
#endif
}

#ifdef HAVE_SYSTEMD
ایستا صحیح
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;
  ثابت نویسه *notify_socket = container->context->notify_socket;
  cleanup_free نویسه *host_notify_socket_path = پوچ;
  cleanup_free نویسه *container_notify_socket_path = پوچ;
  cleanup_free نویسه *state_dir = پوچ;
  uid_t container_ریشه_uid = -1;
  gid_t container_ریشه_gid = -1;
  صحیح notify_socket_درخت_fd;

  اگر (notify_socket == پوچ)
    بازگشت 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_ریشه : پوچ),
                                     (container->context ? container->context->id : پوچ), err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = mkdir (host_notify_socket_path, 0700);
  اگر (ret < 0)
    بازگشت crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  اگر (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_ریشه_in_the_userns (container->container_def, 0, 0, &container_ریشه_uid, &container_ریشه_gid);
      اگر (container_ریشه_uid != ((uid_t) -1) && container_ریشه_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_ریشه_uid, container_ریشه_gid);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_ریشه_uid, container_ریشه_gid);
        }
    }

  notify_socket_درخت_fd = syscall_open_درخت (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  اگر (notify_socket_درخت_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_درخت_fd = notify_socket_درخت_fd;
  وگرنه اگر (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  وگرنه اگر (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  وگرنه
    /* some other error */
    بازگشت crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = پوچ;
  بازگشت 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
ایستا صحیح
do_finalize_notify_socket (libcrun_container_t *container, ثابت نویسه *rootfs, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free نویسه *host_notify_socket_path = پوچ;
  cleanup_free نویسه *container_notify_socket_path = پوچ;
  cleanup_free نویسه *container_notify_socket_path_dir_alloc = پوچ;
  ثابت نویسه *container_notify_socket_path_dir = پوچ;
  cleanup_close صحیح notify_socket_درخت_fd = -1;
  cleanup_close صحیح targetfd = -1;
  صحیح did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = پوچ;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = پوچ;

  اگر (host_notify_socket_path == پوچ || container_notify_socket_path == پوچ)
    بازگشت 0;

  اگر (rootfs == پوچ)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  اگر (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  targetfd = ret;

  notify_socket_درخت_fd = get_private_data (container)->notify_socket_درخت_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_درخت_fd = -1;

  اگر (notify_socket_درخت_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_درخت_fd, targetfd, پوچ);
      اگر (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          اگر (UNLIKELY (targetfd < 0))
            بازگشت targetfd;

          ret = do_mount (container, پوچ, targetfd, container_notify_socket_path_dir, پوچ,
                          MS_REC | MS_PRIVATE, پوچ, LABEL_NONE, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
      وگرنه اگر (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      وگرنه
        بازگشت crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_درخت_fd,
                                container_notify_socket_path_dir);
    }

  اگر (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, پوچ,
                      MS_BIND | MS_REC | MS_PRIVATE, پوچ, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

ایستا صحیح
make_parent_mount_private (ثابت نویسه *rootfs, libcrun_error_t *err)
{
  cleanup_close صحیح rootfsfd = -1;
  ساختار stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (rootfsfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", rootfs);

  اگر (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    بازگشت crun_make_error (err, errno, "fstat `%s`", rootfs);

  برای (;;)
    {
      ساختار stat cur_st;
      libcrun_error_t tmp_err = پوچ;
      صحیح ret;
      cleanup_close صحیح parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      اگر (ret == 0)
        بازگشت 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (پوچ, proc_path, پوچ, MS_PRIVATE, پوچ);
      اگر (ret == 0)
        بازگشت 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      اگر (parentfd < 0)
        {
          صحیح saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          اگر (ret != 0)
            بازگشت crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          بازگشت crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      اگر (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        بازگشت crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      اگر (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        بشکن;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  بازگشت crun_make_error (err, 0, "make `%s` private", rootfs);
}

صحیح
libcrun_set_mounts (ساختار container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ثابت نویسه *rootfs, set_mounts_cb_t cb, تهی *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح is_user_ns = 0;
  صحیح ret = 0;

  اگر (rootfs == پوچ || def->mounts == پوچ)
    بازگشت 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  اگر (def->ریشه->readonly)
    {
      ساختار remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      صحیح fd;

      fd = dup (get_private_data (container)->rootfsfd);
      اگر (UNLIKELY (fd < 0))
        بازگشت crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, پوچ, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  اگر (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      اگر (UNLIKELY (is_user_ns < 0))
        بازگشت is_user_ns;
    }

  اگر (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  اگر (cb)
    {
      ret = cb (cb_data, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = پوچ;
      ثابت نویسه *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (تهی) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

ایستا صحیح
umount_or_hide (ثابت نویسه *target, libcrun_error_t *err)
{
  صحیح ret;

  ret = umount2 (target, MNT_DETACH);
  اگر (UNLIKELY (ret < 0))
    {
      صحیح saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      اگر (errno == EINVAL)
        {
          ret = mount (پوچ, target, "tmpfs", 0, "size=0k");
          اگر (LIKELY (ret == 0))
            بازگشت 0;
        }

      بازگشت crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  بازگشت ret;
}

ایستا صحیح
move_ریشه (ثابت نویسه *rootfs, libcrun_error_t *err)
{
  صحیح ret;

  ret = chdir (rootfs);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = umount_or_hide ("/proc", err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "chdir to `%s`", rootfs);

  بازگشت 0;
}

صحیح
libcrun_finalize_mounts (ساختار container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ثابت نویسه *rootfs,
                         set_mounts_cb_t after_pivot_cb, تهی *cb_data, libcrun_error_t *err)
{
  صحیح ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_ریشه_fd));

  اگر (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      اگر (get_private_data (container)->no_pivot)
        {
          ret = move_ریشه (rootfs, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
      وگرنه
        {
          ret = do_pivot (container, rootfs, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      ret = do_mount (container, پوچ, -1, "/", پوچ,
                      get_private_data (container)->rootfs_propagation,
                      پوچ, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = chdir ("/");
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "chdir to `/`");
    }

  اگر (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = finalize_mounts (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

ایستا صحیح
maybe_open_درخت_namespace (ثابت نویسه *rootfs, صحیح *out_fd, libcrun_error_t *err)
{
  cleanup_close صحیح rootfs_fd = -1;
  صحیح درخت_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (rootfs_fd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", rootfs);

  درخت_fd = syscall_open_درخت (rootfs_fd, "",
                               OPEN_درخت_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  اگر (درخت_fd < 0)
    {
      اگر (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        بازگشت 0;
      بازگشت crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = درخت_fd;
  بازگشت 0;
}

ایستا ساختار libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  ساختار libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  اگر (mount_fds == پوچ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  بازگشت mount_fds;
}

ایستا صحیح
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free نویسه *mnt_data = پوچ;
  unsigned long mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  ثابت نویسه *label_type = پوچ;
  ثابت نویسه *label_val = پوچ;
  ثابت نویسه *fstype;
  صحیح mnt_fd;
  size_t j;

  اگر (mount->type == پوچ)
    بازگشت -1;

  fstype = mount->type;

  برای (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  اگر (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      صحیح extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      اگر (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          اگر (label_type == پوچ)
            crun_error_release (err);
          وگرنه
            label_val = def->linux->mount_label;
        }
    }

  اگر (strcmp (fstype, "cgroup") == 0)
    {
      صحیح cgroup_mode = libcrun_get_cgroup_mode (err);
      اگر (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  اگر (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, پوچ, پوچ, mnt_data);

  بازگشت mnt_fd;
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
ساختار mnt_id_req_s
{
  uint32_t size;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

ساختار statmount_s
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
  نویسه str[];
};

ایستا صحیح
syscall_listmount (ساختار mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned صحیح flags)
{
#ifdef __NR_listmount
  بازگشت (صحیح) syscall (__NR_listmount, req, ids, nr, flags);
#وگرنه
  (تهی) req;
  (تهی) ids;
  (تهی) nr;
  (تهی) flags;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

ایستا صحیح
syscall_statmount (ساختار mnt_id_req_s *req, ساختار statmount_s *buf, size_t size, unsigned صحیح flags)
{
#ifdef __NR_statmount
  بازگشت (صحیح) syscall (__NR_statmount, req, buf, size, flags);
#وگرنه
  (تهی) req;
  (تهی) buf;
  (تهی) size;
  (تهی) flags;
  errno = ENOSYS;
  بازگشت -1;
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
ایستا bool
mounts_hit_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = پوچ;
  size_t i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  برای (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      اگر (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != پوچ && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          بشکن;
        }
    }
  اگر (! has_absolute_bind)
    بازگشت false;

  ids = xmalloc (capacity * اندازه (uint64_t));
  برای (;;)
    {
      ساختار mnt_id_req_s req = {
        .size = اندازه (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      صحیح ret;

      اگر (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * اندازه (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      اگر (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        بازگشت true;
      اگر (ret == 0)
        بشکن;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  برای (i = 0; i < n_ids; i++)
    {
      نویسه buffer[4096] __attribute__ ((aligned (8)));
      ساختار statmount_s *sm = (ساختار statmount_s *) buffer;
      ساختار mnt_id_req_s req = {
        .size = اندازه (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      ثابت نویسه *ریشه, *point;
      size_t j;

      اگر (syscall_statmount (&req, sm, اندازه (buffer), 0) < 0)
        ادامه;
      اگر ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        ادامه;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      ریشه = sm->str + sm->mnt_root;
      اگر (! has_prefix (ریشه, "mnt:["))
        ادامه;

      point = sm->str + sm->mnt_point;

      برای (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          ثابت نویسه *source = def->mounts[j]->source;
          size_t source_len;

          اگر (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            ادامه;
          اگر (source == پوچ || source[0] != '/')
            ادامه;

          source_len = strlen (source);
          وقتی (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          اگر (strncmp (point, source, source_len) != 0)
            ادامه;

          /* The source itself is a pinned mount namespace.  */
          اگر (point[source_len] == '\0')
            بازگشت true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          اگر (recursive && (source_len == 1 || point[source_len] == '/'))
            بازگشت true;
        }
    }

  بازگشت false;
}

ایستا bool
can_use_open_درخت_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ساختار libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  اگر (has_hooks || has_userns)
    بازگشت false;

  اگر (mounts_hit_pinned_mount_namespace (def))
    بازگشت false;

  اگر (container->context->no_open_درخت_namespace)
    بازگشت false;

  {
    libcrun_error_t tmp_err = پوچ;
    صحیح in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    اگر (in_userns != 0)
      بازگشت false;
  }

  mount_fds = get_fd_map (container);
  برای (i = 0; i < def->mounts_len; i++)
    {
      اگر (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          اگر (! has_cached_target)
            بازگشت false;
        }
    }

  بازگشت true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
ایستا bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  اگر (mount->type == پوچ || strcmp (mount->type, "tmpfs") != 0)
    بازگشت false;

  برای (i = 0; i < mount->options_len; i++)
    اگر (mount->options[i] != پوچ && has_prefix (mount->options[i], "mode="))
      بازگشت false;

  بازگشت true;
}

ایستا صحیح
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, نویسه **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close صحیح rootfsfd = -1;
  cleanup_close صحیح درخت_fd = -1;
  libcrun_error_t tmp_err = پوچ;
  bool use_open_درخت = false;
  size_t i;
  صحیح ret;

  اگر (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, پوچ, پوچ, پوچ, پوچ);

  اگر ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  آزاد (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  برای (i = 0; i < def->mounts_len; i++)
    {
      ساختار libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close صحیح mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      اگر (mount_fds && mount_fds->fds[i] >= 0)
        ادامه;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      اگر (def->mounts[i]->source != پوچ
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free نویسه *mnt_data = پوچ;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          برای (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          اگر (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free نویسه *target = پوچ;
              libcrun_error_t tmp_err = پوچ;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              اگر (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              اگر (get_private_data (container)->copy_symlink_targets == پوچ)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * اندازه (نویسه *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = پوچ;
            }
        }

      اگر (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          اگر (mount_fds && def->mounts[i]->source != پوچ && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == پوچ)
            {
              libcrun_error_t tmp_err = پوچ;
              unsigned long propagation = MS_PRIVATE;

              اگر (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              اگر (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          ادامه;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      اگر (mnt_fd < 0)
        {
          crun_error_release (err);
          ادامه;
        }

      اگر (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close صحیح destfd = -1;
          ساختار stat st;

          اگر (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              اگر (UNLIKELY (rootfsfd < 0))
                بازگشت crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          اگر (destfd < 0)
            crun_error_release (&tmp_err);
          وگرنه اگر (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              صحیح procfd = get_procfd (get_private_data (container), err);
              اگر (UNLIKELY (procfd < 0))
                بازگشت procfd;

              get_self_fd_path (fd_path, mnt_fd);
              ret = fchmodat (procfd, fd_path, st.st_mode & 07777, 0);
              (تهی) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_empty_dir_cached (container, پوچ, &tmp_err);
  اگر (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  اگر (! get_private_data (container)->joined_mount_ns
      && can_use_open_درخت_namespace (container))
    use_open_درخت = true;

  اگر (use_open_درخت)
    {
      ret = maybe_open_درخت_namespace (*rootfs, &درخت_fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  اگر (درخت_fd >= 0)
    preopen_needed_devs_and_notify (container);

  اگر (درخت_fd >= 0)
    {
      ret = setns (درخت_fd, CLONE_NEWNS);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (پوچ, "/", پوچ, MS_REMOUNT | MS_BIND, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, پوچ, -1, "/", پوچ, MS_REC | MS_PRIVATE, پوچ, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = do_mount (container, پوچ, -1, "/", پوچ, rootfs_propagation, پوچ, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_ریشه_switched = true;
      آزاد (*rootfs);
      *rootfs = xstrdup ("/");
    }
  وگرنه
    {
      اگر (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, پوچ, -1, "/", پوچ, rootfs_propagation, پوچ, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = make_parent_mount_private (*rootfs, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, پوچ, MS_BIND | MS_REC | MS_PRIVATE, پوچ, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  اگر (! container->context->force_no_cgroup)
    {
      صحیح cgroup_mode = libcrun_get_cgroup_mode (err);
      اگر (UNLIKELY (cgroup_mode < 0))
        بازگشت cgroup_mode;

      اگر (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          نویسه *unified_cgroup_path = پوچ;
          صحیح procfd = get_procfd (get_private_data (container), err);
          اگر (UNLIKELY (procfd < 0))
            بازگشت procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  اگر (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = پوچ;
      بازگشت ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = پوچ;

  اگر (درخت_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  اگر (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  بازگشت 0;
}

صحیح
libcrun_do_pivot_ریشه (libcrun_container_t *container, bool no_pivot, نویسه **rootfs, libcrun_error_t *err)
{
  صحیح ret;

  اگر (*rootfs == پوچ)
    بازگشت 0;

  اگر (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      اگر (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "chdir to `/`");
        }
    }
  وگرنه
    {
      ret = chroot (*rootfs);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "chdir to `/`");

      آزاد (*rootfs);
      *rootfs = xstrdup ("/");
    }

  بازگشت 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
صحیح
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  ساختار stat statbuf;
  cleanup_close صحیح fd;
  صحیح ret;
  صحیح i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  اگر (UNLIKELY (fd == -1))
    بازگشت crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  برای (i = 0; i <= 2; i++)
    {
      اگر (UNLIKELY (fstat (i, &statbuf) == -1))
        بازگشت crun_make_error (err, errno, "stat fd `%d`", i);
      اگر (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          اگر (UNLIKELY (dup2 (fd, i) == -1))
            بازگشت crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  بازگشت 0;
}

ایستا صحیح
uidgidmap_helper (نویسه *helper, pid_t pid, ثابت نویسه *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  نویسه pid_fmt[16];
  نویسه *args[MAX_ARGS + 1];
  نویسه *next;
  cleanup_free نویسه *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  صحیح ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, اندازه (pid_fmt), "%d", pid);
  اگر (UNLIKELY (ret >= (صحیح) اندازه (pid_fmt)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  وقتی (nargs < MAX_ARGS)
    {
      نویسه *p = strsep (&next, " \n");
      اگر (next == پوچ)
        بشکن;
      args[nargs++] = p;
    }
  args[nargs++] = پوچ;

  ret = run_process (args, err);
  اگر (ret < 0)
    بازگشت ret;
  اگر (ret > 0)
    بازگشت crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  بازگشت 0;
}

ایستا صحیح
newgidmap (pid_t pid, ثابت نویسه *map_file, libcrun_error_t *err)
{
  بازگشت uidgidmap_helper ("newgidmap", pid, map_file, err);
}

ایستا صحیح
newuidmap (pid_t pid, ثابت نویسه *map_file, libcrun_error_t *err)
{
  بازگشت uidgidmap_helper ("newuidmap", pid, map_file, err);
}

ایستا صحیح
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_close صحیح fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  اگر (UNLIKELY (fd < 0))
    بازگشت fd;

  ret = TEMP_FAILURE_RETRY (اكتب (fd, "deny", 4));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  بازگشت 0;
}

ایستا صحیح
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free نویسه *content = پوچ;

  اگر (get_private_data (container)->deny_setgroups)
    بازگشت 0;

  اگر (container->container_def->annotations)
    {
      ثابت نویسه *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      اگر (annotation)
        بازگشت strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close صحیح fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    اگر (fd < 0)
      ret = fd;
    وگرنه
      ret = read_all_fd (fd, "self/setgroups", &content, پوچ, err);
  }
  اگر (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          بازگشت 1;
        }
      بازگشت ret;
    }

  بازگشت strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

صحیح
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = پوچ;
  size_t additional_gids_len = 0;
  صحیح can_do_setgroups;
  صحیح ret;

  اگر (process != پوچ && process->user != پوچ)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  اگر (UNLIKELY (can_do_setgroups < 0))
    بازگشت can_do_setgroups;

  اگر (can_do_setgroups == 0)
    {
      ثابت نویسه *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      اگر (annotation && strcmp (annotation, "0") != 0)
        بازگشت 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      برای (i = 0; i < additional_gids_len; i++)
        اگر (additional_gids[i] != process->user->gid)
          بازگشت crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (صحیح) additional_gids[i]);
      بازگشت 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "setgroups");

  بازگشت 0;
}

صحیح
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  اگر (get_private_data (container)->unshare_cgroupns)
    {
      صحیح ret = unshare (CLONE_NEWCGROUP);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (errno != EINVAL)
            بازگشت crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  بازگشت 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
صحیح
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  اگر (len != 1)
    بازگشت 0;

  اگر (mappings[0]->size != 1)
    بازگشت 0;

  اگر (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    بازگشت 0;

  بازگشت 1;
}

ایستا صحیح
write_id_map (libcrun_container_t *container, pid_t pid, ثابت نویسه *map_name,
              صحیح (*idmap_helper) (pid_t, ثابت نویسه *, libcrun_error_t *), ثابت نویسه *helper_name,
              bool warn_on_failure, ثابت نویسه *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  صحیح ret = 0;

  اگر (container->host_uid)
    ret = idmap_helper (pid, map, err);
  اگر (container->host_uid == 0 || ret < 0)
    {
      cleanup_close صحیح fd = -1;

      اگر (ret < 0)
        {
          اگر (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      اگر (UNLIKELY (fd < 0))
        بازگشت fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      اگر (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free نویسه *single_mapping = پوچ;
          crun_error_release (err);

          اگر (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              اگر (UNLIKELY (ret < 0))
                بازگشت ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          اگر (UNLIKELY (fd < 0))
            بازگشت fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  بازگشت ret;
}

صحیح
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free نویسه *uid_map = پوچ;
  cleanup_free نویسه *gid_map = پوچ;
  size_t uid_map_len = 0, gid_map_len = 0;
  صحیح ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  اگر ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    بازگشت 0;

  اگر (! def || ! def->linux)
    بازگشت 0;

  اگر (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      uid_map_len = (size_t) ret;
      اگر (uid_map == پوچ)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }

  اگر (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      gid_map_len = (size_t) ret;
      اگر (gid_map == پوچ)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

ساختار all_caps_s
{
  unsigned long effective[2];
  unsigned long permitted[2];
  unsigned long inheritable[2];
  unsigned long ambient[2];
  unsigned long bounding[2];
};

ایستا صحیح
has_cap_on (صحیح cap, long unsigned *caps)
{
  اگر (cap < 32)
    بازگشت CAP_TO_MASK_0 (cap) & caps[0];
  بازگشت (CAP_TO_MASK_1 (cap) & caps[1]);
}

ایستا unsigned long cap_last_cap;

صحیح
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  صحیح ret;
  نویسه buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  اگر (fd < 0)
    بازگشت fd;
  ret = TEMP_FAILURE_RETRY (اقرأ (fd, buffer, اندازه (buffer) - 1));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, پوچ, 10);
  اگر (errno != 0)
    بازگشت crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  بازگشت 0;
}

ایستا صحیح
set_required_caps (ساختار all_caps_s *caps, uid_t uid, gid_t gid, صحیح no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  صحیح ret;
  ساختار __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  ساختار __user_cap_data_struct data[2] = { { 0 } };

  اگر (cap_last_cap == 0)
    بازگشت crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  برای (cap = 0; cap <= cap_last_cap; cap++)
    اگر (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        اگر (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          بازگشت crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  اگر (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    بازگشت crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  اگر (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    بازگشت crun_make_error (err, errno, "prctl reset ambient");

  برای (cap = 0; cap <= cap_last_cap; cap++)
    اگر (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        اگر (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          بازگشت crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  اگر (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "prctl reset KEEPCAPS");

      اگر (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          برای (cap = 0; cap <= cap_last_cap; cap++)
            اگر (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "capset");
        }
    }
#endif

  اگر (no_new_privs)
    اگر (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      بازگشت crun_make_error (err, errno, "prctl set no new privs");

  بازگشت 0;
}

ایستا صحیح
read_caps (unsigned long caps[2], نویسه **مقادیر, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  برای (i = 0; i < len; i++)
    {
      cap_مقدار_t cap;
      اگر (cap_from_name (مقادیر[i], &cap) < 0)
        بازگشت crun_make_error (err, 0, "unknown capability `%s`", مقادیر[i]);

      اگر (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      وگرنه
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#وگرنه
  caps[0] = 0;
  caps[1] = 0;
#endif
  بازگشت 0;
}

صحیح
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  اگر (proc->selinux_label)
    بازگشت set_selinux_label (container, proc->selinux_label, now, err);

  بازگشت 0;
}

صحیح
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  اگر (proc->apparmor_profile)
    بازگشت set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  بازگشت 0;
}

صحیح
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  صحیح no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  ساختار all_caps_s caps = {};
  صحیح ret;

  اگر (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

ساختار rlimit_s
{
  ثابت نویسه *name;
  صحیح مقدار;
};

ساختار rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
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
                              { "RLIMIT_STACK", RLIMIT_پشته },
                              { پوچ, 0 } };

ایستا صحیح
get_rlimit_resource (ثابت نویسه *name)
{
  ساختار rlimit_s *it;
  برای (it = rlimits; it->name; it++)
    اگر (strcmp (it->name, name) == 0)
      بازگشت it->مقدار;
  بازگشت -1;
}

صحیح
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  برای (i = 0; i < len; i++)
    {
      برای (j = i + 1; j < len; j++)
        {
          اگر (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            بازگشت crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  برای (i = 0; i < len; i++)
    {
      ساختار rlimit limit;
      نویسه *type = new_rlimits[i]->type;
      صحیح resource = get_rlimit_resource (type);
      اگر (UNLIKELY (resource < 0))
        بازگشت crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      اگر (UNLIKELY (setrlimit (resource, &limit) < 0))
        بازگشت crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  بازگشت 0;
}

صحیح
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  صحیح ret;
  اگر (def->hostname == پوچ || def->hostname[0] == '\0')
    بازگشت 0;
  اگر (! has_uts)
    بازگشت crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "sethostname");
  بازگشت 0;
}

صحیح
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  صحیح ret;
  اگر (is_empty_string (def->domainname))
    بازگشت 0;
  اگر (! has_uts)
    بازگشت crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "setdomainname");
  بازگشت 0;
}

صحیح
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحیح fd = -1;
  صحیح ret;
  نویسه oom_buffer[16];
  اگر (def->process == پوچ || ! def->process->oom_score_adj_present)
    بازگشت 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, اندازه (oom_buffer), "%i", def->process->oom_score_adj);
  اگر (UNLIKELY (ret >= (صحیح) اندازه (oom_buffer)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  اگر (fd < 0)
    بازگشت fd;
  ret = TEMP_FAILURE_RETRY (اكتب (fd, oom_buffer, strlen (oom_buffer)));
  اگر (ret < 0)
    بازگشت crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  بازگشت 0;
}

ثابت نویسه *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  پوچ
};

ایستا صحیح
validate_sysctl (ثابت نویسه *original_کلید, ثابت نویسه *original_مقدار, ثابت نویسه *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ثابت نویسه *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  اگر (name[0] == '/' || path_has_dot_dot_component (name))
    بازگشت crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_کلید);

  اگر (has_prefix (name, "fs/mqueue/"))
    {
      اگر (namespaces_created & CLONE_NEWIPC)
        بازگشت 0;

      namespace = "IPC";
      برو fail;
    }

  اگر (has_prefix (name, "kernel/"))
    {
      size_t i;

      برای (i = 0; sysctlRequiringIPC[i]; i++)
        اگر (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            اگر (namespaces_created & CLONE_NEWIPC)
              بازگشت 0;

            namespace = "IPC";
            برو fail;
          }

      اگر (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          اگر (! is_empty_string (def->domainname) && (strcmp (original_مقدار, def->domainname) != 0))
            بازگشت crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_کلید);

          اگر (namespaces_created & CLONE_NEWUTS)
            بازگشت 0;

          namespace = "UTS";
          برو fail;
        }

      اگر (strcmp (name, "kernel/hostname") == 0)
        بازگشت crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_کلید);
    }
  اگر (has_prefix (name, "net/"))
    {
      اگر (namespaces_created & CLONE_NEWNET)
        بازگشت 0;

      namespace = "network";
      برو fail;
    }

  بازگشت crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_کلید);

fail:
  بازگشت crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_کلید, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
ایستا نویسه *
sysctl_error_reason (ثابت نویسه *name, صحیح namespaces_created, صحیح errno_)
{

  اگر (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    بازگشت xstrdup ("are all the IDs mapped in the user namespace?");

  بازگشت پوچ;
}

صحیح
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close صحیح dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  اگر (def->linux == پوچ || def->linux->sysctl == پوچ || def->linux->sysctl->len == 0)
    بازگشت 0;

  برای (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحیح مقدار;

      مقدار = libcrun_find_namespace (def->linux->namespaces[i]->type);
      اگر (UNLIKELY (مقدار < 0))
        بازگشت crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= مقدار;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  اگر (UNLIKELY (dirfd < 0))
    بازگشت dirfd;

  برای (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free نویسه *name = پوچ;
      cleanup_close صحیح fd = -1;
      صحیح ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      اگر (def->linux->sysctl->کلیدها[i] == پوچ || def->linux->sysctl->مقادیر[i] == پوچ)
        بازگشت crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_کلید_to_proc_path (def->linux->sysctl->کلیدها[i]);

      ret = validate_sysctl (def->linux->sysctl->کلیدها[i], def->linux->sysctl->مقادیر[i], name, namespaces_created, def, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      اگر (UNLIKELY (fd < 0))
        بازگشت crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (اكتب (fd, def->linux->sysctl->مقادیر[i], strlen (def->linux->sysctl->مقادیر[i])));
      اگر (UNLIKELY (ret < 0))
        {
          صحیح saved_errno = errno;
          cleanup_free نویسه *reason = پوچ;

          reason = sysctl_error_reason (def->linux->sysctl->کلیدها[i], namespaces_created, saved_errno);
          بازگشت crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  بازگشت 0;
}

ایستا صحیح
open_terminal (نویسه **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  uid_t uid = 0;
  صحیح ret;

  اگر (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  اگر (UNLIKELY (fd < 0))
    بازگشت fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  بازگشت ret;
}

نویسه *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  بازگشت get_private_data (container)->external_descriptors;
}

صحیح
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  ثابت نویسه *buf = پوچ;
  json_gen_ctx *gen = پوچ;
  size_t buf_len;
  صحیح ret;
  صحیح i;

  اگر (! json_gen_init (&gen, پوچ))
    بازگشت crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  اگر (UNLIKELY (ret != json_gen_status_ok))
    برو json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  برای (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      نویسه link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      اگر (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          اگر (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          وگرنه
            {
              json_gen_free (gen);
              بازگشت crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      اگر (UNLIKELY (ret != json_gen_status_ok))
        برو json_error;
    }

  ret = json_gen_array_close (gen);
  اگر (UNLIKELY (ret != json_gen_status_ok))
    برو json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  اگر (UNLIKELY (ret != json_gen_status_ok))
    برو json_error;

  اگر (buf)
    {
      نویسه *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  بازگشت 0;

json_error:
  اگر (gen)
    json_gen_free (gen);
  بازگشت json_gen_error_to_crun_error (ret, err);
}

صحیح
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free نویسه *pty = پوچ;
  cleanup_close صحیح fd = -1;
  صحیح ret;

  اگر (def->process == پوچ || ! def->process->terminal)
    بازگشت 0;

  fd = open_terminal (&pty, def->process, err);
  اگر (UNLIKELY (fd < 0))
    بازگشت fd;

  اگر (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", پوچ, MS_BIND, پوچ, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه
    {
      ret = unlink ("/dev/console");
      اگر (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          اگر (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", پوچ, MS_BIND, پوچ, LABEL_MOUNT, err);
              اگر (UNLIKELY (ret < 0))
                بازگشت ret;

              بازگشت get_and_reset (&fd);
            }
          بازگشت crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  بازگشت get_and_reset (&fd);
}

ایستا bool
read_error_from_sync_socket (صحیح sync_socket_fd, صحیح *error, نویسه **str)
{
  cleanup_free نویسه *b = پوچ;
  size_t size;
  صحیح code = 0;
  صحیح ret;

  اگر (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_fd, &code, اندازه (code)));
      اگر (UNLIKELY (ret != (صحیح) اندازه (code)))
        بازگشت false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_fd, &size, اندازه (size)));
  اگر (UNLIKELY (ret != (صحیح) اندازه (size)))
    بازگشت false;

  اگر (size == 0)
    بازگشت false;

  اگر (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_fd, b, size));
  اگر (UNLIKELY (ret < 0))
    بازگشت false;

  b[ret] = '\0';

  *str = b;
  b = پوچ;
  بازگشت true;
}

ایستا bool
send_error_to_sync_socket (صحیح sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  صحیح ret;
  صحیح code;
  نویسه *پیام;

  اگر (err == پوچ || *err == پوچ)
    بازگشت false;

  code = crun_error_get_errno (err);

  اگر (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_fd, "1", 1));
      اگر (UNLIKELY (ret < 0))
        بازگشت false;
    }

  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_fd, &code, اندازه (code)));
  اگر (UNLIKELY (ret < 0))
    بازگشت false;

  پیام = (*err)->پیام;
  size = strlen (پیام) + 1;
  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_fd, &size, اندازه (size)));
  اگر (UNLIKELY (ret < 0))
    بازگشت false;

  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_fd, پیام, size));
  اگر (UNLIKELY (ret < 0))
    بازگشت false;

  بازگشت true;
}

ایستا درونخطی صحیح
send_success_to_sync_socket (صحیح sync_socket, libcrun_error_t *err)
{
  ثابت صحیح success = 0;
  صحیح ret;

  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket, &success, اندازه (success)));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "write to sync socket");

  بازگشت 0;
}

ایستا __attribute__ ((noreturn)) تهی
send_error_to_sync_socket_and_die (صحیح sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  نویسه *پیام;

  اگر (err == پوچ || *err == پوچ)
    _safe_exit (EXIT_FAILURE);

  اگر (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      پیام = (*err)->پیام;
      libcrun_fail_with_error (errno, "%s", پیام);
    }
  _safe_exit (EXIT_FAILURE);
}

ایستا صحیح
expect_success_from_sync_socket (صحیح sync_fd, libcrun_error_t *err)
{
  cleanup_free نویسه *err_str = پوچ;
  صحیح res = 1;
  صحیح ret;

  ret = TEMP_FAILURE_RETRY (اقرأ (sync_fd, &res, اندازه (res)));
  اگر (UNLIKELY (ret != اندازه (res)))
    بازگشت crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  اگر (res == 0)
    بازگشت 0;

  اگر (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      اگر (! is_empty_string (err_str))
        بازگشت crun_make_error (err, res, "%s", err_str);
    }

  بازگشت crun_make_error (err, res, "read from sync socket");
}

ایستا صحیح
join_namespaces (runtime_spec_schema_config_schema *def, صحیح *namespaces_to_join, صحیح n_namespaces_to_join,
                 صحیح *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  صحیح ret;
  صحیح i;

  برای (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free نویسه *cwd = پوچ;
      صحیح orig_index = namespaces_to_join_index[i];
      صحیح مقدار;

      اگر (namespaces_to_join[i] < 0)
        ادامه;

      /* Skip the user namespace.  */
      مقدار = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      اگر (مقدار == CLONE_NEWUSER)
        ادامه;

      اگر (مقدار == CLONE_NEWNS)
        {
          cwd = getcwd (پوچ, 0);
          اگر (UNLIKELY (cwd == پوچ))
            بازگشت crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], مقدار);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (ignore_join_errors)
            ادامه;
          بازگشت crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      اگر (مقدار == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  بازگشت 0;
}

#define MAX_NAMESPACES 10

ساختار init_status_s
{
  /* fd to the namespace to join.  */
  صحیح fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  صحیح index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  صحیح مقدار[MAX_NAMESPACES];
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
  صحیح userns_index;
  /* def->linux->namespaces userns.  */
  صحیح userns_index_origin;

  /* All namespaces created/joined by the container.  */
  صحیح all_namespaces;

  /* What namespaces are still missing to be created.  */
  صحیح namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  صحیح idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  صحیح idx_timens_to_join_immediately;
};

تهی
cleanup_free_init_statusp (ساختار init_status_s *ns)
{
  size_t i;

  اگر (ns == پوچ)
    بازگشت;

  برای (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

ایستا صحیح
configure_init_status (ساختار init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  برای (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  برای (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحیح مقدار = libcrun_find_namespace (def->linux->namespaces[i]->type);
      اگر (UNLIKELY (مقدار < 0))
        بازگشت crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      اگر (ns->all_namespaces & مقدار)
        بازگشت crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= مقدار;

      اگر (def->linux->namespaces[i]->path == پوچ)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= مقدار;
        }
      وگرنه
        {
          صحیح fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          اگر (ns->fd_len >= MAX_NAMESPACES)
            بازگشت crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          اگر (UNLIKELY (fd < 0))
            بازگشت crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          اگر (مقدار == CLONE_NEWUSER)
            {
              ns->userns_index = ns->fd_len;
              ns->userns_index_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->index[ns->fd_len] = i;
          ns->مقدار[ns->fd_len] = مقدار;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  اگر (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  بازگشت 0;
}

/* Detect if root is available in the container.  */
ایستا bool
ریشه_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  برای (i = 0; i < len; i++)
    اگر (mappings[i]->container_id == 0)
      بازگشت true;

  بازگشت false;
}

ایستا ساختار libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  ساختار libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  اگر (dev_fds == پوچ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  بازگشت dev_fds;
}

#define رقم_NEEDED_DEVS (اندازه (needed_devs) / اندازه (needed_devs[0]) - 1)

ایستا ساختار libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  ساختار libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  اگر (fds == پوچ)
    {
      fds = make_libcrun_fd_map (رقم_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  بازگشت fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  اگر (recursive)
    *recursive = false;

  اگر (src_nofollow != پوچ)
    *src_nofollow = false;

  برای (i = 0; i < mnt->options_len; i++)
    {
      اگر (mnt->options[i] == پوچ)
        ادامه;

      اگر (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          اگر (src_nofollow == پوچ || *src_nofollow)
            بشکن;
        }
      اگر (strcmp (mnt->options[i], "rbind") == 0)
        {
          اگر (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          اگر (src_nofollow == پوچ || *src_nofollow)
            بشکن;
        }
      اگر (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  بازگشت ret;
}

ایستا نویسه *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  برای (i = 0; i < mnt->options_len; i++)
    {
      اگر (mnt->options[i] == پوچ)
        ادامه;

      اگر (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          بازگشت mnt->options[i];
        }
      اگر (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          بازگشت mnt->options[i];
        }
    }
  بازگشت پوچ;
}

ایستا صحیح
open_mount_type (ثابت نویسه *type, صحیح *out_fd, libcrun_error_t *err)
{
  cleanup_close صحیح fsopen_fd = -1;
  cleanup_close صحیح newfs_fd = -1;
  صحیح ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  اگر (UNLIKELY (fsopen_fd < 0))
    بازگشت crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, پوچ, پوچ, 0);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  اگر (UNLIKELY (newfs_fd < 0))
    بازگشت crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  بازگشت 0;
}

ایستا صحیح
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, صحیح *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close صحیح newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  ساختار mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close صحیح fd = -1;
  ثابت نویسه *idmap_option;
  bool recursive = false;
  ثابت نویسه *options = پوچ;
  bool has_mappings;
  صحیح ret;
  نویسه *extra_پیام = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != پوچ);
  اگر (has_mappings_out)
    *has_mappings_out = has_mappings;
  اگر (! has_mappings)
    بازگشت 0;

  اگر ((mnt->uid_mappings == پوچ) != (mnt->gid_mappings == پوچ))
    بازگشت crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  اگر (idmap_option)
    {
      options = strchr (idmap_option, '=');
      اگر (options)
        {
          /* Skip the '=' itself.  */
          options++;
          اگر (options[0] == '\0')
            options = پوچ;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  اگر (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  اگر (UNLIKELY (fd < 0))
    بازگشت fd;

  اگر (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_درخت (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      اگر (UNLIKELY (newfs_fd < 0))
        بازگشت crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  وگرنه
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == EINVAL)
        {
          extra_پیام = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      بازگشت crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_پیام);
    }

  *out_fd = get_and_reset (&newfs_fd);
  بازگشت 0;
}

ایستا uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  اگر (! def || ! def->linux)
    برو exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  اگر (! mappings)
    برو exit;

  برای (i = 0; i < len; i++)
    {
      اگر (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        بازگشت id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  بازگشت is_uid ? get_overflow_uid () : get_overflow_gid ();
}

ایستا صحیح
precreate_device (libcrun_container_t *container, صحیح devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  نویسه name[64];
  mode_t type;
  dev_t dev;
  صحیح ret;

  ret = snprintf (name, اندازه (name), "%zu", i);
  اگر (UNLIKELY (ret >= (صحیح) اندازه (name)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "chown `%s`", device->path);

  بازگشت get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

ایستا صحیح
send_mounts (صحیح sync_socket_host, ساختار libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  صحیح ret;

  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_host, &how_many, اندازه (how_many)));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "write to sync socket");

  برای (i = 0; i < total; i++)
    {
      اگر (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (نویسه *) &i, اندازه (i), err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }
  بازگشت 0;
}

ایستا صحیح
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, صحیح sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ساختار libcrun_fd_map *mount_fds = پوچ;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free نویسه *rootfs = پوچ;
  size_t how_many = 0;
  size_t i;
  صحیح ret;

  اگر (def->mounts_len == 0)
    بازگشت 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  اگر (def->ریشه && def->ریشه->path)
    rootfs = realpath (def->ریشه->path, پوچ);

  اگر (! has_userns)
    {
      صحیح is_in_userns = check_running_in_user_namespace (err);
      اگر (UNLIKELY (is_in_userns < 0))
        بازگشت is_in_userns;

      has_userns = is_in_userns > 0;
    }

  برای (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      صحیح mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      اگر (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == پوچ)
        {
          unsigned long propagation = 0;

          اگر (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, پوچ, پوچ, پوچ, پوچ) & ALL_PROPAGATIONS_NO_REC;
          اگر (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          اگر (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      اگر (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  بازگشت send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

ایستا صحیح
prepare_and_send_dev_mounts (libcrun_container_t *container, صحیح sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ساختار libcrun_fd_map *dev_fds = پوچ;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close صحیح current_mountns = -1;
  cleanup_free نویسه *state_dir = پوچ;
  cleanup_free نویسه *devs_path = پوچ;
  cleanup_close صحیح devs_mountfd = -1;
  cleanup_close صحیح targetfd = -1;
  ثابت نویسه *context_type = پوچ;
  ثابت نویسه *label = پوچ;
  size_t how_many = 0;
  size_t i;
  صحیح ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  اگر (def->linux == پوچ || def->linux->devices_len == 0)
    بازگشت 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  اگر (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    بازگشت send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_ریشه : پوچ),
                                     (container->context ? container->context->id : پوچ), err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = mkdir (devs_path, 0700);
  اگر (UNLIKELY (ret < 0 && errno != EEXIST))
    بازگشت crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  اگر (UNLIKELY (current_mountns < 0))
    بازگشت current_mountns;

  ret = unshare (CLONE_NEWNS);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (پوچ, "/", پوچ, MS_REC | MS_PRIVATE, پوچ);
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      برو restore_mountns;
    }

  اگر (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      اگر (UNLIKELY (context_type == پوچ))
        {
          ret = -1;
          برو restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", پوچ, context_type, label, پوچ);
  اگر (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      برو restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      برو restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, پوچ);
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      برو restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      برو restore_mountns;
    }

  برای (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      اگر (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ادامه;
        }

      dev_fds->fds[i] = ret;

      اگر (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  اگر (ns_changed && current_mountns >= 0)
    {
      صحیح setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      اگر (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          بازگشت crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  بازگشت ret;
}

ایستا صحیح
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, صحیح sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map ساختار libcrun_fd_map *fds = پوچ;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (رقم_NEEDED_DEVS);

  اگر (! has_userns || geteuid () > 0)
    بازگشت send_mounts (sync_socket_host, fds, 0, رقم_NEEDED_DEVS, err);

  برای (i = 0; i < رقم_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      صحیح fd = syscall_open_درخت (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      اگر (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  بازگشت send_mounts (sync_socket_host, fds, how_many, رقم_NEEDED_DEVS, err);
}

ایستا صحیح
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, صحیح sync_socket_host, libcrun_error_t *err)
{
  صحیح ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

ایستا صحیح
receive_mounts (ساختار libcrun_fd_map *fds, صحیح sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  صحیح ret;

  اگر (fds->nfds == 0)
    بازگشت 0;

  ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_container, &how_many, اندازه (how_many)));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "read from sync socket");

  برای (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (نویسه *) &index, اندازه (index), err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      اگر (index >= fds->nfds)
        بازگشت crun_make_error (err, 0, "invalid mount data received");

      اگر (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  بازگشت 0;
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
ایستا صحیح
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;

  اگر (! container->host_has_cap_sys_ptrace)
    بازگشت 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  بازگشت 0;
}

ایستا صحیح
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  صحیح ret;

  اگر (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool ریشه_mapped = false;

      اگر (def->linux->uid_mappings_len != 0)
        {
          ریشه_mapped = ریشه_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          اگر (! ریشه_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      اگر (def->linux->gid_mappings_len != 0)
        {
          ریشه_mapped = ریشه_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          اگر (! ریشه_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  اگر (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    بازگشت crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "setresgid to `%d`", gid);

  بازگشت 0;
}

ایستا bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  بازگشت mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, پوچ, پوچ);
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
ایستا صحیح
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ساختار libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close صحیح socket_parent = -1;
  cleanup_close صحیح socket_child = -1;
  cleanup_close صحیح ریشه_fd = -1;
  cleanup_close صحیح cwd_fd = -1;
  size_t i, missing = 0;
  صحیح saved_errno;
  صحیح sockets[2];
  pid_t pid;
  صحیح ret;

  اگر (def->ریشه == پوچ || def->ریشه->path == پوچ)
    بازگشت 0;

  برای (i = 0; i < def->mounts_len; i++)
    اگر (is_proc_mount (def->mounts[i]))
      missing++;
  اگر (missing == 0)
    بازگشت 1;

  ریشه_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (ریشه_fd < 0))
    بازگشت crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (cwd_fd < 0))
    بازگشت crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->ریشه->path);
  اگر (UNLIKELY (ret < 0))
    بازگشت 0;

  pid = fork ();
  saved_errno = errno;
  اگر (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      اگر (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (تهی) unshare (CLONE_NEWNS);

      برای (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close صحیح fd = -1;

          اگر (! is_proc_mount (def->mounts[i]))
            ادامه;

          fd = open_mount_of_type (container, def->mounts[i], err);
          اگر (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (نویسه *) &i, اندازه (i), err);
          اگر (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  اگر (UNLIKELY (fchdir (ریشه_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    بازگشت crun_make_error (err, errno, "restore the root directory");

  اگر (UNLIKELY (pid < 0))
    بازگشت crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  وقتی (missing > 0)
    {
      libcrun_error_t tmp_err = پوچ;

      ret = receive_fd_from_socket_with_payload (socket_parent, (نویسه *) &i, اندازه (i), &tmp_err);
      اگر (ret < 0)
        {
          crun_error_release (&tmp_err);
          بشکن;
        }
      اگر (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          بشکن;
        }

      اگر (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, پوچ, 0);

  بازگشت missing == 0 ? 1 : 0;
}

ایستا صحیح
init_container (libcrun_container_t *container, صحیح sync_socket_container, ساختار init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ساختار libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close صحیح timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  صحیح ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      اگر (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      اگر (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      اگر (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          delayed_pidns_fork = ret > 0;
        }

      اگر (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_container, &new_pid, اندازه (new_pid)));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "write to sync socket");
        }
      وگرنه
        {
          new_pid = fork ();
          اگر (UNLIKELY (new_pid < 0))
            بازگشت crun_make_error (err, errno, "fork");
        }

      اگر (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          اگر (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_container, &new_pid, اندازه (new_pid)));
          اگر (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = libcrun_set_oom (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  اگر ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      برای (i = 0; i < def->mounts_len; i++)
        {
          صحیح fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          اگر (def->mounts[i]->type == پوچ)
            ادامه;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          اگر (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, پوچ, پوچ, پوچ);
          اگر (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, پوچ, پوچ, پوچ);

          اگر (fd >= 0)
            {
              اگر (mount_fds->fds[i] >= 0)
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
  اگر (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      اگر (UNLIKELY (timens_offsets_fd < 0))
        بازگشت timens_offsets_fd;
    }

  اگر (init_status->all_namespaces & CLONE_NEWUSER)
    {
      اگر (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      اگر (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
      وگرنه
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "unshare");
    }

  اگر (def->linux->time_offsets)
    {
      نویسه fmt_buffer[128];
      صحیح fd = timens_offsets_fd;

      اگر (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, اندازه (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          اگر (UNLIKELY (ret >= (صحیح) اندازه (fmt_buffer)))
            بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long صحیح) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = اكتب (fd, fmt_buffer, strlen (fmt_buffer));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      اگر (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long صحیح) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, اندازه (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          اگر (UNLIKELY (ret >= (صحیح) اندازه (fmt_buffer)))
            بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

          ret = اكتب (fd, fmt_buffer, strlen (fmt_buffer));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  اگر (init_status->must_fork)
    {
      اگر (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          delayed_pidns_fork = ret > 0;
        }

      اگر (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          اگر (UNLIKELY (pid_container < 0))
            بازگشت crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      اگر (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_container, &pid_container, اندازه (pid_container)));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "write to sync socket");

          اگر (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  بازگشت get_private_data (container)->delayed_pidns_fork;
}

صحیح
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close صحیح client_fd = -1;
  cleanup_close صحیح pidfd = -1;
  ثابت نویسه *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  اگر (v == پوچ)
    بازگشت 0;

  pidfd = syscall_pidfd_open (pid, 0);
  اگر (UNLIKELY (pidfd < 0))
    بازگشت crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  اگر (UNLIKELY (client_fd < 0))
    بازگشت client_fd;

  بازگشت send_fd_to_socket (client_fd, pidfd, err);
}

ایستا bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  اگر (process == پوچ || process->exec_cpu_affinity == پوچ)
    بازگشت false;
  بازگشت (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, تهی *args,
                             صحیح *sync_socket_out, ساختار libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) ساختار init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحیح sync_socket_container = -1;
  نویسه *notify_socket_env = پوچ;
  cleanup_close صحیح sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close صحیح restore_pidns = -1;
  صحیح first_clone_args = 0;
  صحیح sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  صحیح ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  اگر (def->ریشه)
    {
      ret = do_notify_socket (container, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  اگر (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = libcrun_set_oom (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->ریشه ? container->container_def->ریشه->path : پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  برای (i = 0; i < init_status.fd_len; i++)
    {
      انتخاب (init_status.مقدار[i])
        {
        حالت CLONE_NEWIPC:
          اگر (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          بشکن;

        حالت CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          اگر (! new_userns)
            init_status.must_fork = true;
          وگرنه
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          بشکن;

        حالت CLONE_NEWTIME:
          اگر (! new_userns)
            init_status.must_fork = true;
          وگرنه
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          بشکن;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  اگر ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  وگرنه اگر ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  اگر (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  اگر (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      ساختار _clone3_args clone3_args;
      memset (&clone3_args, 0, اندازه (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      اگر (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  اگر (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, پوچ);
      اگر (UNLIKELY (pid < 0))
        بازگشت crun_make_error (err, errno, "clone");
    }

  اگر (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = close_and_reset (&sync_socket_container);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_درخت_fd));

      اگر (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_host, &new_pid, اندازه (new_pid)));
          اگر (UNLIKELY (ret != اندازه (new_pid)))
            بازگشت crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          اگر (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          وگرنه
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, پوچ, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      اگر (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      اگر ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      اگر (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_host, &grandchild, اندازه (grandchild)));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          اگر (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          وگرنه
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, پوچ, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      اگر (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      pid_to_clean = 0;
      بازگشت pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  اگر (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  اگر (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  وگرنه
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      اگر (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->پیام);
    }

  /* Jump into the specified entrypoint.  */
  اگر (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  اگر (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->پیام);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  اگر (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

ایستا صحیح
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, صحیح sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, ثابت نویسه *sub_cgroup,
                            صحیح *terminal_fd, libcrun_error_t *err)
{
  صحیح ret;
  نویسه res;
  pid_t pid;
  cleanup_close صحیح sync_fd = sync_socket_fd;

  اگر (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (اقرأ (sync_fd, &res, 1));
  اگر (UNLIKELY (ret != 1))
    بازگشت crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  اگر (res != '0')
    بازگشت crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (اقرأ (sync_fd, &pid, اندازه (pid)));
  اگر (UNLIKELY (ret != اندازه (pid)))
    بازگشت crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, پوچ, 0);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "waitpid for exec child pid");

  اگر (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (need_move_to_cgroup)
    {
      cleanup_cgroup_status ساختار libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free نویسه *final_cgroup = پوچ;

      اگر (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, پوچ);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_set_io_priority (pid, process, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (اكتب (sync_fd, &ret, اندازه (ret)));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "write to sync socket");

  اگر (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      اگر (UNLIKELY (ret < 0))
        {
          صحیح err_code = 0;
          cleanup_free نویسه *err_str = پوچ;

          اگر (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              اگر (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  بازگشت crun_make_error (err, err_code, "%s", err_str);
                }
            }

          بازگشت crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  بازگشت pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
ایستا صحیح
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحیح pidfd_pid_to_join = -1;
  صحیح all_flags = 0;
  size_t i;
  صحیح ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  اگر (def->linux && def->linux->namespaces)
    {
      برای (i = 0; i < def->linux->namespaces_len; i++)
        اگر (! is_empty_string (def->linux->namespaces[i]->path))
          بازگشت 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  اگر (UNLIKELY (pidfd_pid_to_join < 0))
    بازگشت 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  اگر (ret == 0)
    بازگشت crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  برای (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].مقدار;

  ret = setns (pidfd_pid_to_join, all_flags);
  اگر (UNLIKELY (ret < 0))
    بازگشت 0;

  بازگشت 1;
}

ایستا صحیح
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح fds_joined[MAX_NAMESPACES] = {
    0,
  };
  صحیح fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  صحیح ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  /* Nothing left to do if the namespaces were joined.  */
  اگر (LIKELY (ret > 0))
    بازگشت 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  اگر (def->linux->namespaces_len >= MAX_NAMESPACES)
    بازگشت crun_make_error (err, 0, "invalid configuration");

  برای (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free نویسه *ns_path = پوچ;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      اگر (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          اگر (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ادامه;
            }

          ret = fds[i];
          برو exit;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  اگر (UNLIKELY (ret < 0))
    برو exit;
  اگر (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      برو exit;
    }

  برای (i = 0; namespaces[i].ns_file; i++)
    {
      اگر (namespaces[i].مقدار == CLONE_NEWUSER)
        ادامه;

      ret = setns (fds[i], 0);
      اگر (ret == 0)
        fds_joined[i] = 1;
    }
  برای (i = 0; namespaces[i].ns_file; i++)
    {
      اگر (fds_joined[i])
        ادامه;
      ret = setns (fds[i], 0);
      اگر (ret == 0)
        fds_joined[i] = 1;
    }
  برای (i = 0; namespaces[i].ns_file; i++)
    {
      اگر (fds_joined[i])
        ادامه;
      ret = setns (fds[i], 0);
      اگر (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          برای (j = 0; j < def->linux->namespaces_len; j++)
            {
              اگر (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  بشکن;
                }
            }
          اگر (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              ادامه;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          برو exit;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

exit:
  برای (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  بازگشت ret;
}

صحیح
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      ثابت نویسه *sub_cgroup,
                      صحیح detach,
                      runtime_spec_schema_config_schema_process *process,
                      صحیح *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  صحیح ret;
  صحیح sync_socket_fd[2];
  cleanup_close صحیح cgroup_dirfd = -1;
  cleanup_close صحیح sync_fd = -1;
  ساختار _clone3_args clone3_args;
  bool need_move_to_cgroup;

  اگر (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "set child subreaper");
    }

  اگر (path_has_dot_dot_component (sub_cgroup))
    بازگشت crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  اگر (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free نویسه *final_cgroup = پوچ;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status ساختار libcrun_cgroup_status *cgroup_status = پوچ;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    اگر (get_force_cgroup_v1_annotation (container) == پوچ)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        اگر (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, اندازه (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  اگر (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  وگرنه
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  اگر (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  /* On errors, fall back to fork().  */
  اگر (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      اگر (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          برو exit;
        }
    }

  اگر (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      بازگشت join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  اگر (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (اكتب (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->پیام);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  اگر (UNLIKELY (pid < 0))
    {
      صحیح saved_errno = errno;
      TEMP_FAILURE_RETRY (اكتب (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  اگر (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (اكتب (sync_fd, "0", 1));
      اگر (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (اكتب (sync_fd, &pid, اندازه (pid)));
      اگر (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  وگرنه
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free نویسه *pty = پوچ;
      صحیح r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (اقرأ (sync_fd, &r, اندازه (r)));
      اگر (UNLIKELY (ret != اندازه (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      اگر (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      اگر (terminal_fd)
        {
          cleanup_close صحیح ptmx_fd = -1;

          ret = set_id_init (container, err);
          اگر (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          اگر (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          اگر (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  بازگشت 0;

exit:
  اگر (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  اگر (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  بازگشت ret;
}

صحیح
libcrun_linux_container_update (libcrun_container_status_t *status, ثابت نویسه *state_ریشه, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status ساختار libcrun_cgroup_status *cgroup_status = پوچ;

  cgroup_status = libcrun_cgroup_make_status (status);

  بازگشت libcrun_update_cgroup_resources (cgroup_status, state_ریشه, resources, err);
}

ایستا صحیح
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, ثابت bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status ساختار libcrun_cgroup_status *cgroup_status = پوچ;

  cgroup_status = libcrun_cgroup_make_status (status);

  بازگشت libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

صحیح
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  بازگشت libcrun_container_pause_unpause_linux (status, true, err);
}

صحیح
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  بازگشت libcrun_container_pause_unpause_linux (status, false, err);
}

صحیح
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  صحیح ret;

  اگر (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  وگرنه اگر (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  وگرنه
    بازگشت crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, 0, "personality to `%s`", p->domain);

  بازگشت 0;
}

صحیح
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحیح sockfd = -1;

  برای (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحیح مقدار = libcrun_find_namespace (def->linux->namespaces[i]->type);
      اگر (UNLIKELY (مقدار < 0))
        بازگشت crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      اگر (مقدار == CLONE_NEWNET && def->linux->namespaces[i]->path == پوچ)
        {
          configure_network = true;
          بشکن;
        }
    }

  اگر (! configure_network)
    بازگشت 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  اگر (LIKELY (sockfd >= 0))
    {
      ساختار ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  وگرنه
    {
      ساختار nlmsghdr *hdr_recv;
      نویسه buf[اندازه (ساختار nlmsghdr) + اندازه (ساختار nlmsgerr)];
      ساختار sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      ساختار sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      ساختار nlmsghdr hdr = {
        .nlmsg_len = اندازه (ساختار nlmsghdr) + اندازه (ساختار ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      ساختار ifinfomsg پیام = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((ساختار nlmsghdr *) buf) = hdr;
      *((ساختار ifinfomsg *) (buf + اندازه (ساختار nlmsghdr))) = پیام;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      اگر (UNLIKELY (sockfd < 0))
        بازگشت crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (ساختار sockaddr *) &addr, اندازه (addr));
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, اندازه (buf), 0, (ساختار sockaddr *) &addr_to, اندازه (addr_to));
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, اندازه (buf), 0, پوچ, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (ساختار nlmsghdr *) buf;

      اگر (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          ساختار nlmsgerr *err_data = (ساختار nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          اگر (err_data->error < 0)
            بازگشت crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  بازگشت 0;
}

/* Protection for attacks like CVE-2019-5736.  */
صحیح ensure_cloned_دودویی ();
__attribute__ ((constructor)) ایستا تهی
libcrun_rexec (تهی)
{
  اگر (ensure_cloned_دودویی () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

صحیح
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  بازگشت libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

صحیح
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  صحیح ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
ایستا صحیح
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, صحیح سیگنال, libcrun_error_t *err)
{
  صحیح ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  اگر (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      /* The pid is not valid anymore, return an error.  */
      اگر (ret == 0)
        {
          errno = ESRCH;
          بازگشت crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, سیگنال);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "kill container");
  بازگشت 0;
}

صحیح
libcrun_kill_linux (libcrun_container_status_t *status, صحیح سیگنال, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_close صحیح pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  اگر (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      اگر (errno == ENOSYS)
        بازگشت libcrun_kill_linux_no_pidfd (status, true, سیگنال, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      اگر (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        بازگشت crun_make_error (err, ESRCH, "process not running");
      بازگشت crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* The pid is not valid anymore, return an error.  */
  اگر (ret == 0)
    {
      errno = ESRCH;
      بازگشت crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, سیگنال, پوچ, 0);
  اگر (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      اگر (errno == ENOSYS)
        بازگشت libcrun_kill_linux_no_pidfd (status, false, سیگنال, err);
      بازگشت crun_make_error (err, errno, "send signal to pidfd");
    }

  بازگشت 0;
}

ایستا ثابت نویسه *
libcrun_get_intelrdt_name (ثابت نویسه *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  اگر (def == پوچ || def->linux == پوچ || def->linux->intel_rdt == پوچ || def->linux->intel_rdt->clos_id == پوچ)
    {
      اگر (explicit)
        *explicit = false;
      بازگشت ctr_name;
    }

  اگر (explicit)
    *explicit = true;
  بازگشت def->linux->intel_rdt->clos_id;
}

صحیح
libcrun_apply_intelrdt (ثابت نویسه *ctr_name, libcrun_container_t *container, pid_t pid, صحیح actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = پوچ;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  ثابت نویسه *name;
  صحیح ret;

  اگر (container)
    def = container->container_def;

  اگر (def == پوچ || def->linux == پوچ || def->linux->intel_rdt == پوچ)
    بازگشت 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  اگر (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      اگر (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          اگر (UNLIKELY (ret < 0))
            برو fail;

          has_monitoring = true;
        }
    }

  اگر (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      اگر (UNLIKELY (ret < 0))
        برو fail;
    }

  اگر (actions & LIBCRUN_INTELRDT_MOVE)
    {
      ثابت نویسه *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : پوچ;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      اگر (UNLIKELY (ret < 0))
        برو fail;
    }

  بازگشت 0;

fail:
  اگر (has_monitoring)
    {
      libcrun_error_t tmp_err = پوچ;
      صحیح tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      اگر (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  اگر (created)
    {
      libcrun_error_t tmp_err = پوچ;
      صحیح tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      اگر (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  بازگشت ret;
}

صحیح
libcrun_destroy_intelrdt (ثابت نویسه *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  ثابت نویسه *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  اگر (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      صحیح ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      اگر (ret < 0)
        بازگشت ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  اگر (explicit)
    بازگشت 0;

  بازگشت resctl_destroy (clos_id, err);
}

صحیح
libcrun_update_intel_rdt (ثابت نویسه *ctr_name, libcrun_container_t *container, ثابت نویسه *l3_cache_schema, ثابت نویسه *mem_bw_schema, نویسه *ثابت *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = پوچ;
  ثابت نویسه *name;

  اگر (container)
    def = container->container_def;

  اگر (def == پوچ || def->linux == پوچ || def->linux->intel_rdt == پوچ)
    بازگشت 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, پوچ);

  بازگشت resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
صحیح
libcrun_safe_chdir (ثابت نویسه *path, libcrun_error_t *err)
{
  cleanup_free نویسه *buffer = پوچ;
  صحیح ret;

  ret = chdir (path);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  اگر (ret == 0 || buffer[0] != '/')
    {
      اگر (chdir ("/") < 0)
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
#define UNREACHABLE_LEN ((صحیح) اندازه (UNREACHABLE) - 1)

      اگر ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        بازگشت crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      بازگشت crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  بازگشت 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
ایستا صحیح
run_callback_in_namespace (صحیح mntns_fd, صحیح (*callback) (تهی *, libcrun_error_t *), تهی *arg, libcrun_error_t *err)
{
  صحیح wait_status = 0;
  pid_t pid;
  صحیح ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  اگر (UNLIKELY (pid < 0))
    بازگشت crun_make_error (err, errno, "vfork");

  اگر (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      اگر (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      اگر (UNLIKELY (ret < 0))
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
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  بازگشت get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
ایستا صحیح
check_container_userns (libcrun_container_t *container, صحیح userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close صحیح expected_fd = -1;
  cleanup_close صحیح parent_fd = -1;
  ساختار stat st, expected_st;
  ثابت نویسه *path = پوچ;
  size_t i;
  صحیح ret;

  اگر (def->linux)
    برای (i = 0; i < def->linux->namespaces_len; i++)
      اگر (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == پوچ;
          بشکن;
        }

  اگر (path == پوچ)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      اگر (UNLIKELY (expected_fd < 0))
        بازگشت expected_fd;
    }
  وگرنه
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      اگر (UNLIKELY (expected_fd < 0))
        بازگشت crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  اگر (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      اگر (UNLIKELY (parent_fd < 0))
        بازگشت crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fstat user namespace");

  اگر (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    بازگشت crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  بازگشت 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
ایستا صحیح
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, صحیح *mntns_fd, صحیح *owner_fd, libcrun_error_t *err)
{
  cleanup_close صحیح mntns = -1;
  cleanup_close صحیح owner = -1;
  صحیح ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  اگر (UNLIKELY (mntns < 0))
    بازگشت mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (ret == 0)
    بازگشت crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  اگر (UNLIKELY (owner < 0))
    بازگشت crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  بازگشت 0;
}

ایستا صحیح
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, صحیح (*callback) (تهی *, libcrun_error_t *), تهی *arg, libcrun_error_t *err)
{
  cleanup_close صحیح mntns_fd = -1;
  cleanup_close صحیح owner_fd = -1;
  صحیح ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت run_callback_in_namespace (mntns_fd, callback, arg, err);
}

ساختار umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
ایستا صحیح
umount_nofollow (ثابت نویسه *destination, libcrun_error_t *err)
{
  cleanup_close صحیح parentfd = -1;
  cleanup_close صحیح cwdfd = -1;
  نویسه buffer[PATH_MAX];
  ثابت نویسه *parent, *name;
  نویسه *last_slash;
  size_t len;
  صحیح saved_errno;
  صحیح ret;

  اگر (is_empty_string (destination))
    بازگشت crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  اگر (UNLIKELY (len >= اندازه (buffer)))
    بازگشت crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  وقتی (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  اگر (last_slash == پوچ)
    {
      parent = ".";
      name = buffer;
    }
  وگرنه اگر (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  وگرنه
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  اگر (UNLIKELY (parentfd < 0))
    بازگشت crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (cwdfd < 0))
    بازگشت crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  اگر (UNLIKELY (fchdir (cwdfd) < 0))
    بازگشت crun_make_error (err, errno, "fchdir to the previous working directory");

  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, saved_errno, "umount `%s`", destination);

  بازگشت 0;
}

ایستا صحیح
do_umount_in_a_container (تهی *arg, libcrun_error_t *err)
{
  ساختار umount_in_a_container_args *args = arg;
  size_t i;
  صحیح ret;

  برای (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

ساختار mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  ساختار libcrun_fd_map *fds;
  صحیح pidfd;
  صحیح owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
ایستا صحیح
open_destination_nofollow (ثابت نویسه *destination, bool dir, libcrun_error_t *err)
{
  ثابت uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  صحیح fd;
  صحیح ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  اگر (LIKELY (fd >= 0))
    بازگشت fd;

  اگر (errno != ENOENT)
    بازگشت crun_make_error (err, errno, "open `%s`", destination);

  اگر (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  وگرنه
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", destination);

  بازگشت fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
ایستا صحیح
create_destinations_in_a_container (تهی *arg, libcrun_error_t *err)
{
  ساختار mount_in_a_container_args *args = arg;
  size_t i;
  صحیح ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  اگر (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "setresuid to `0`");
    }
  وگرنه اگر (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      بازگشت crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  برای (i = 0; i < args->len; i++)
    {
      cleanup_close صحیح dest_fd = -1;
      ساختار stat st;

      ret = fstat (args->fds->fds[i], &st);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      اگر (UNLIKELY (dest_fd < 0))
        بازگشت dest_fd;
    }

  بازگشت 0;
}

ایستا صحیح
do_mount_in_a_container (تهی *arg, libcrun_error_t *err)
{
  ساختار mount_in_a_container_args *args = arg;
  size_t i;
  صحیح ret;

  برای (i = 0; i < args->len; i++)
    {
      cleanup_close صحیح dest_fd = -1;
      ساختار stat st;

      ret = fstat (args->fds->fds[i], &st);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      اگر (UNLIKELY (dest_fd < 0))
        بازگشت dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  بازگشت 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
ایستا صحیح
send_mounts_from_container_namespaces (libcrun_container_t *container, صحیح pidfd, runtime_spec_schema_defs_mount **mounts,
                                       ساختار libcrun_fd_map *fds, size_t len, صحیح flags, صحیح owner_fd, صحیح sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  صحیح ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  اگر (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  وگرنه اگر (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      بازگشت crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  اگر (UNLIKELY (pid < 0))
    بازگشت crun_make_error (err, errno, "fork");

  اگر (pid)
    {
      صحیح wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      اگر (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, پوچ, 0));
          بازگشت crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  برای (i = 0; i < len; i++)
    {
      cleanup_close صحیح fd = -1;

      اگر (fds->fds[i] >= 0)
        ادامه;

      fd = open_mount_of_type (container, mounts[i], err);
      اگر (UNLIKELY (fd < 0))
        بازگشت crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
ایستا صحیح
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, صحیح *flags, libcrun_error_t *err)
{
  ایستا ثابت ساختار
  {
    ثابت نویسه *name;
    صحیح flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  برای (i = 0; i < اندازه (namespaces) / اندازه (namespaces[0]); i++)
    {
      cleanup_close صحیح self_fd = -1;
      cleanup_close صحیح target_fd = -1;
      ساختار stat self_st, target_st;
      cleanup_free نویسه *self_path = پوچ;
      صحیح ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      اگر (UNLIKELY (self_fd < 0))
        بازگشت self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      اگر (UNLIKELY (target_fd < 0))
        بازگشت target_fd;

      ret = fstat (self_fd, &self_st);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      اگر (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  بازگشت 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
ایستا صحیح
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, صحیح owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, ساختار libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close صحیح sync_fd = -1;
  cleanup_close صحیح pidfd = -1;
  صحیح sync_socket_fd[2];
  bool needed = false;
  صحیح flags;
  pid_t pid;
  size_t i;
  صحیح ret;

  برای (i = 0; i < len; i++)
    {
      اگر (fds->fds[i] >= 0)
        ادامه;

      اگر (mounts[i]->type == پوچ)
        بازگشت crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  اگر (! needed)
    بازگشت 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  اگر (UNLIKELY (pidfd < 0))
    بازگشت crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (ret == 0)
    بازگشت crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  اگر (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      بازگشت ret;
    }

  اگر (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      اگر (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  برای (i = 0; i < len; i++)
    {
      اگر (fds->fds[i] >= 0)
        ادامه;

      ret = receive_fd_from_socket (sync_fd, err);
      اگر (UNLIKELY (ret < 0))
        {
          cleanup_free نویسه *err_str = پوچ;
          صحیح err_code = 0;

          اگر (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          وگرنه
            ret = crun_error_wrap (err, "receive mount fd");
          بشکن;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, پوچ, 0));

  بازگشت ret;
}

ایستا unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free نویسه *data = پوچ;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  اگر (mount->options == پوچ)
    بازگشت get_default_flags (container, mount->destination, &data);

  برای (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  بازگشت flags;
}

صحیح
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ساختار mount_in_a_container_args args;
  cleanup_close_map ساختار libcrun_fd_map *fds = پوچ;
  cleanup_close صحیح pidfd = -1;
  cleanup_close صحیح mntns_fd = -1;
  cleanup_close صحیح owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  صحیح ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  fds = make_libcrun_fd_map (len);

  برای (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), پوچ, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      اگر (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          اگر (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          اگر (UNLIKELY (fds->fds[i] < 0))
            بازگشت fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  برای (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  اگر (UNLIKELY (ret > 0))
    بازگشت *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

صحیح
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  ساختار umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  بازگشت run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

ساختار restrict_mounts_in_a_container_args
{
  ساختار libcrun_mount_flags_s *mounts;
  size_t len;
  صحیح rootfd;
};

ایستا صحیح
do_restrict_mounts_in_a_container (تهی *arg, libcrun_error_t *err)
{
  ساختار restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  صحیح ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  اگر (LIKELY (ret == 0))
    ret = chroot (".");
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "chroot to the container root");

  برای (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      ثابت نویسه *target = consume_slashes (args->mounts[i].target);
      cleanup_close صحیح fd = -1;
      ساختار statfs sfs;
      unsigned long current_flags;

      اگر (flags == 0)
        ادامه;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      اگر (UNLIKELY (fd < 0))
        {
          اگر (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ادامه;
            }
          بازگشت fd;
        }

      ret = fstatfs (fd, &sfs);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      اگر ((current_flags & flags) == flags)
        ادامه;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      اگر (LIKELY (ret == 0))
        ادامه;

      اگر (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        بازگشت ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (پوچ, target, پوچ, MS_REMOUNT | MS_BIND | flags | current_flags, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "remount `/%s`", target);
    }

  بازگشت 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
صحیح
libcrun_restrict_mounts_in_container (pid_t pid, ساختار libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  ساختار restrict_mounts_in_a_container_args args;
  cleanup_close صحیح mntns_fd = -1;
  cleanup_close صحیح rootfd = -1;
  نویسه path[64];
  صحیح ret;

  snprintf (path, اندازه (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  اگر (UNLIKELY (mntns_fd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, اندازه (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (rootfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  اگر (UNLIKELY (ret > 0))
    بازگشت *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  بازگشت ret;
}

صحیح
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحیح netns_fd = -1;
  size_t i;
  صحیح ret;

  اگر (def == پوچ || def->linux == پوچ || def->linux->net_devices == پوچ)
    بازگشت 0;

  cleanup_free نویسه *ns_path = پوچ;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  اگر (UNLIKELY (netns_fd < 0))
    بازگشت netns_fd;

  برای (i = 0; i < def->linux->net_devices->len; i++)
    {
      ثابت نویسه *new_name = def->linux->net_devices->مقادیر[i]->name ?: def->linux->net_devices->کلیدها[i];

      ret = move_network_device (def->linux->net_devices->کلیدها[i], new_name, netns_fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}
