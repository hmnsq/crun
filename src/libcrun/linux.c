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

#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل "linux.h"
#شامل "utils.h"
#شامل "status.h"
#شامل <string.h>
#شامل <sched.h>
#شامل <fcntl.h>
#شامل <unistd.h>
#شامل <sys/mount.h>
#اگر_تعریف_شدہ HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#اگر_ختم
#اگر defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#اگر_ختم

#شامل <sys/prctl.h>
#اگر_تعریف_شدہ HAVE_CAP
#  include <sys/capability.h>
#اگر_ختم
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

#اگر_غیر_تعریف_شدہ RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#اگر_ختم

#اگر_غیر_تعریف_شدہ OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#اگر_ختم

#اگر_غیر_تعریف_شدہ OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#اگر_ختم

#اگر_غیر_تعریف_شدہ OPEN_درخت_NAMESPACE
#  define OPEN_درخت_NAMESPACE 2
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOVE_MOUNT_F_EMPTY_PATH
#  define MOVE_MOUNT_F_EMPTY_PATH 0x00000004
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOVE_MOUNT_T_EMPTY_PATH
#  define MOVE_MOUNT_T_EMPTY_PATH 0x00000040
#اگر_ختم

#اگر_غیر_تعریف_شدہ FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#اگر_ختم

#اگر_غیر_تعریف_شدہ FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#اگر_ختم

#اگر_غیر_تعریف_شدہ FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#اگر_ختم

#تعریف_کرو ALL_PROPAGATIONS_NO_REC (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE)
#تعریف_کرو ALL_PROPAGATIONS (MS_REC | ALL_PROPAGATIONS_NO_REC)

/* The per-mount flags a remount clears unless they are explicitly set.  */
#تعریف_کرو REMOUNT_CLEAR_FLAGS (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW)

#اگر_غیر_تعریف_شدہ ST_NOSYMFOLLOW
#  define ST_NOSYMFOLLOW 0x2000
#اگر_ختم

#تعریف_کرو MOUNT_ATIME_FLAGS (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME)

/* Convert the ST_ flags reported by statfs(2) to the MS_ flags of mount(2).  */
ساکن غیرعلامتی طویل
mount_flags_from_statfs (غیرعلامتی طویل f_flags)
{
  غیرعلامتی طویل flags = 0;

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

  واپس flags;
}

ساخت remount_s
{
  ساخت remount_s *اگلا;
  حرف *ہدف;
  صحیح targetfd;
  غیرعلامتی طویل flags;
  حرف *ڈیٹا;
};

ساخت نجی_ڈیٹا_s
{
  ساخت remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  صحیح unshare_flags;
  صحیح unshare_cgroupns;

  حرف *host_notify_socket_راستہ;
  حرف *container_notify_socket_راستہ;
  حرف *unified_cgroup_راستہ;

  bool mount_dev_from_host;
  غیرعلامتی طویل rootfs_اشاعت;
  bool deny_setgroups;

  صحیح procfd;

  ثابت حرف *rootfs;
  صحیح rootfsfd;

  صحیح notify_socket_درخت_fd;
  صحیح old_جڑ_fd;

  ساخت libcrun_fd_map *mount_fds;
  ساخت libcrun_fd_map *dev_fds;
  ساخت libcrun_fd_map *needed_devs_fds;

  حرف **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  حرف *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  صحیح maskdir_fd;
  حرف *maskdir_proc_راستہ;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_محور;
  bool no_محور;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_جڑ_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  حرف *host_rootfs;
};

ساخت linux_namespace_s
{
  ثابت حرف *name;
  ثابت حرف *ns_file;
  صحیح قدر;
};

ساکن خالی
cleanup_نجی_ڈیٹا (خالی *نجی_ڈیٹا)
{
  ساخت نجی_ڈیٹا_s *p = نجی_ڈیٹا;

  اگر (p->rootfsfd >= 0)
    عارضی_FAILURE_RETRY (close (p->rootfsfd));
  اگر (p->procfd >= 0)
    عارضی_FAILURE_RETRY (close (p->procfd));
  اگر (p->maskdir_fd >= 0)
    عارضی_FAILURE_RETRY (close (p->maskdir_fd));
  اگر (p->old_جڑ_fd >= 0)
    عارضی_FAILURE_RETRY (close (p->old_جڑ_fd));
  اگر (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  اگر (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  اگر (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  اگر (p->copy_symlink_targets)
    {
      size_t i;
      برائے (i = 0; i < p->n_copy_symlink_targets; i++)
        آزاد (p->copy_symlink_targets[i]);
      آزاد (p->copy_symlink_targets);
    }
  آزاد (p->unified_cgroup_راستہ);
  آزاد (p->host_notify_socket_راستہ);
  آزاد (p->container_notify_socket_راستہ);
  آزاد (p->external_descriptors);
  آزاد (p->maskdir_proc_راستہ);
  آزاد (p->host_rootfs);
  آزاد (p);
}

ساکن ساخت نجی_ڈیٹا_s *
get_نجی_ڈیٹا (ساخت libcrun_container_s *container)
{
  اگر (container->نجی_ڈیٹا == تہی)
    {
      ساخت نجی_ڈیٹا_s *p = xmalloc0 (حجم (*p));
      container->نجی_ڈیٹا = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_درخت_fd = -1;
      p->old_جڑ_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_نجی_ڈیٹا = cleanup_نجی_ڈیٹا;
    }
  واپس container->نجی_ڈیٹا;
}

#اگر_غیر_تعریف_شدہ CLONE_NEWTIME
#  define CLONE_NEWTIME 0x00000080
#اگر_ختم
#اگر_غیر_تعریف_شدہ CLONE_NEWCGROUP
#  define CLONE_NEWCGROUP 0x02000000
#اگر_ختم
#اگر_غیر_تعریف_شدہ AT_RECURSIVE
#  define AT_RECURSIVE 0x8000
#اگر_ختم

ساکن ساخت linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { تہی, تہی, 0 } };

ساکن صحیح
get_and_reset (صحیح *old)
{
  صحیح tmp = *old;
  *old = -1;
  واپس tmp;
}

صحیح
libcrun_ڈھونڈو_namespace (ثابت حرف *name)
{
  ساخت linux_namespace_s *it;
  برائے (it = namespaces; it->name; it++)
    اگر (strcmp (it->name, name) == 0)
      واپس it->قدر;
  واپس -1;
}

ساکن صحیح
syscall_keyctl_join (ثابت حرف *name)
{
#تعریف_کرو KEYCTL_JOIN_SESSION_KEYRING 0x1
  واپس (صحیح) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

ساکن صحیح
syscall_pidfd_open (pid_t pid, غیرعلامتی صحیح flags)
{
#اگر defined __NR_pidfd_open
  واپس (صحیح) syscall (__NR_pidfd_open, pid, flags);
#ورنہ
  (خالی) pid;
  (خالی) flags;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

ساکن صحیح
syscall_pidfd_send_signal (صحیح pidfd, صحیح sig, siginfo_t *info, غیرعلامتی صحیح flags)
{
#اگر defined __NR_pidfd_send_signal
  واپس (صحیح) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#ورنہ
  (خالی) pidfd;
  (خالی) sig;
  (خالی) info;
  (خالی) flags;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
ساکن uint64_t
ms_flags_مقصد_mount_attr (uint64_t ms_flags)
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

  واپس attr;
}

ساکن صحیح
do_mount_setattr (bool recursive, ثابت حرف *ہدف, صحیح targetfd, uint64_t clear, uint64_t سیٹ, libcrun_error_t *err)
{
  ساخت mount_attr_s attr = {
    0,
  };
  صحیح ret;

  سیٹ &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.اشاعت = سیٹ & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_مقصد_mount_attr (سیٹ & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_مقصد_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  اگر (سیٹ & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      اگر (سیٹ & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      ورنہ اگر (سیٹ & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      ورنہ
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      اگر (سیٹ & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  ورنہ اگر (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_خالی_خانے_راستہ, &attr);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "mount_setattr `/%s`", ہدف);

  واپس 0;
}

صحیح
get_bind_mount (صحیح dirfd, ثابت حرف *src, bool recursive, bool rdonly, bool nofollow, غیرعلامتی طویل اشاعت, libcrun_error_t *err)
{
  cleanup_close صحیح open_درخت_fd = -1;
  ساخت mount_attr_s attr = {
    0,
  };
  صحیح recursive_flag = (recursive ? AT_RECURSIVE : 0);
  صحیح ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  اگر (UNLIKELY (src == تہی))
    واپس crun_make_error (err, EINVAL, "mount source is not specified");

  اگر (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.اشاعت = اشاعت;

  اگر (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_درخت_fd = syscall_open_درخت (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_خالی_خانے_راستہ : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  اگر (UNLIKELY (open_درخت_fd < 0))
    واپس crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_درخت_fd, "", AT_خالی_خانے_راستہ | recursive_flag, &attr);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "mount_setattr `%s`", src);

  واپس get_and_reset (&open_درخت_fd);
}

صحیح
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, حرف *option, حرف **برآمد, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free حرف *mappings = تہی;
  ثابت حرف *it;
  صحیح ret;

  mappings = xmalloc (allocated);

  برائے (it = option; *it;)
    {
      bool relative = false;
      طویل قدر[3];
      size_t i;

      اگر (*it == '\0')
        توڑو;

      اگر (*it == '#')
        it++;

      اگر (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      برائے (i = 0; i < 3; i++)
        {
          حرف *endptr = تہی;

          اگر (i > 0 && *it == '-')
            it++;

          اگر (*it == '\0')
            واپس crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          قدر[i] = strtol (it, &endptr, 10);
          اگر (errno || endptr == it)
            واپس crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      اگر (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          اگر (def == تہی
              || def->linux == تہی
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            واپس crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          برائے (i = 0; i < mappings_len; i++)
            اگر (قدر[1] >= (int64_t) mappings[i]->container_id
                && قدر[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->سائز)
              توڑو;

          اگر (i == mappings_len)
            واپس crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          قدر[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      اگر (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", قدر[0], قدر[1], قدر[2]);
      اگر (UNLIKELY (ret >= (صحیح) (allocated - written)))
        واپس crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *برآمد = mappings;
  mappings = تہی;

  واپس 0;
}

ساکن صحیح
format_mount_mappings (حرف **برآمد,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  ثابت size_t max_len_mapping = 64;
  cleanup_free حرف *ret = تہی;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  برائے (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->سائز);
      اگر (UNLIKELY (len >= max_len_mapping))
        واپس crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *برآمد = ret;
  ret = تہی;
  واپس 0;
}

ساکن صحیح
format_mount_mapping (حرف **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t سائز, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .سائز = سائز,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    تہی,
  };

  واپس format_mount_mappings (ret, mappings, 1, written, err);
}

ساکن bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  اگر (def->linux == تہی)
    واپس mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  اگر (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    واپس false;

  اگر (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    واپس false;

  برائے (s = 0; s < mnt->uid_mappings_len; s++)
    {
      اگر (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        واپس false;
      اگر (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        واپس false;
      اگر (mnt->uid_mappings[s]->سائز != def->linux->uid_mappings[s]->سائز)
        واپس false;
    }

  برائے (s = 0; s < mnt->gid_mappings_len; s++)
    {
      اگر (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        واپس false;
      اگر (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        واپس false;
      اگر (mnt->gid_mappings[s]->سائز != def->linux->gid_mappings[s]->سائز)
        واپس false;
    }

  واپس true;
}

ساکن pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        ثابت حرف *options, pid_t *pid_برآمد,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != تہی;

  اگر (! need_new_userns)
    واپس 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, تہی);
  اگر (UNLIKELY (pid < 0))
    واپس crun_make_error (err, errno, "clone");

  اگر (pid == 0)
    {
      prctl (PR_سیٹ_PDEATHSIG, SIGKILL);
      جب (1)
        pause ();
      محفوظ_exit (EXIT_SUCCESS);
    }

  اگر (mnt->uid_mappings_len)
    {
      cleanup_free حرف *uid_map = تہی;
      cleanup_free حرف *gid_map = تہی;
      size_t written = 0;
      صحیح ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      cleanup_close صحیح fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      اگر (UNLIKELY (fd < 0))
        واپس fd;

      ret = محفوظ_write (fd, "uid_map", uid_map, written, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      اگر (UNLIKELY (fd < 0))
        واپس fd;

      ret = محفوظ_write (fd, "gid_map", gid_map, written, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      cleanup_free حرف *dup_options = تہی;
      حرف *option, *saveptr = تہی;

      اگر (! options)
        واپس crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      برائے (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (تہی, ";", &saveptr))
        {
          cleanup_free حرف *mappings = تہی;
          cleanup_close صحیح map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          صحیح ret;

          اگر (has_سابقہ (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              اگر (UNLIKELY (map_fd < 0))
                واپس map_fd;
            }
          ورنہ اگر (has_سابقہ (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              اگر (UNLIKELY (map_fd < 0))
                واپس map_fd;
            }
          ورنہ
            واپس crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          ret = محفوظ_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }

  *pid_برآمد = pid;
  pid = -1;
  واپس 0;
}

صحیح
libcrun_create_keyring (libcrun_container_t *container, ثابت حرف *name, ثابت حرف *label, libcrun_error_t *err)
{
  cleanup_close صحیح labelfd = -1;
  bool label_سیٹ = false;
  صحیح ret;

  اگر (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      اگر (UNLIKELY (labelfd < 0))
        {
          اگر (crun_error_get_errno (err) != ENOENT)
            واپس labelfd;

          crun_error_release (err);
        }

      اگر (labelfd >= 0)
        {
          ret = اكتب (labelfd, label, لڑی_لمبائی (label));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_سیٹ = true;
        }
    }

  ret = syscall_keyctl_join (name);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          جاؤ برآمد;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      جاؤ برآمد;
    }

برآمد:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  اگر (label_سیٹ && اكتب (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  واپس ret;
}

ساکن خالی
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

ساکن غیرعلامتی طویل
get_mount_flags (ثابت حرف *name, صحیح current_flags, صحیح *ملا, غیرعلامتی طویل *extra_flags, uint64_t *rec_clear, uint64_t *rec_سیٹ)
{
  ثابت ساخت اشاعت_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  اگر (ملا)
    *ملا = prop ? 1 : 0;

  اگر (prop == تہی)
    واپس 0;

  اگر (prop->extra_flags & OPTION_RECURSIVE)
    {
      اگر (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      اگر (rec_سیٹ && ! prop->clear)
        *rec_سیٹ |= prop->flags;
    }

  اگر (extra_flags)
    *extra_flags |= prop->extra_flags;

  اگر (prop->clear)
    واپس current_flags & ~prop->flags;

  واپس current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
ساکن bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  برائے (i = 0; i < mount->options_len; i++)
    {
      ثابت ساخت اشاعت_flags_s *prop;

      اگر (mount->options[i] == تہی)
        جاری;

      prop = libcrun_str2mount_flags (mount->options[i]);
      اگر (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        واپس true;
    }

  واپس false;
}

ساکن غیرعلامتی طویل
get_mount_flags_or_option (ثابت حرف *name, صحیح current_flags, غیرعلامتی طویل *extra_flags, حرف **option, uint64_t *rec_clear, uint64_t *rec_سیٹ)
{
  صحیح ملا;
  __attribute__ ((غیر_مستعمل)) cleanup_free حرف *پچھلا = تہی;
  غیرعلامتی طویل flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  اگر (name == تہی)
    واپس 0;

  flags = get_mount_flags (name, current_flags, &ملا, extra_flags, rec_clear, rec_سیٹ);
  اگر (ملا)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      ثابت حرف *ڈیٹا_flag = تہی;
      اگر ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        ڈیٹا_flag = "sync";
      ورنہ اگر ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        ڈیٹا_flag = "dirsync";

      اگر (ڈیٹا_flag)
        {
          پچھلا = *option;
          اگر (*option && **option)
            xasprintf (option, "%s,%s", *option, ڈیٹا_flag);
          ورنہ
            *option = xstrdup (ڈیٹا_flag);
        }
      واپس flags;
    }

  پچھلا = *option;
  اگر (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  ورنہ
    *option = xstrdup (name);

  واپس 0;
}

صحیح
محور_جڑ (ثابت حرف *new_جڑ, ثابت حرف *put_old)
{
  واپس syscall (__NR_pivot_root, new_جڑ, put_old);
}

ساکن خالی
free_remount (ساخت remount_s *r)
{
  اگر (r == تہی)
    واپس;
  اگر (r->targetfd >= 0)
    close (r->targetfd);
  آزاد (r->ڈیٹا);
  آزاد (r->ہدف);
  آزاد (r);
}

ساکن ساخت remount_s *
make_remount (صحیح targetfd, ثابت حرف *ہدف, غیرعلامتی طویل flags, ثابت حرف *ڈیٹا, ساخت remount_s *اگلا)
{
  ساخت remount_s *ret = xmalloc (حجم (*ret));
  ret->ہدف = xstrdup (ہدف);
  ret->flags = flags;
  ret->ڈیٹا = ڈیٹا ? xstrdup (ڈیٹا) : تہی;
  ret->اگلا = اگلا;
  ret->targetfd = targetfd;
  واپس ret;
}

ساکن صحیح
do_remount (صحیح targetfd, ثابت حرف *ہدف, غیرعلامتی طویل flags, ثابت حرف *ڈیٹا, libcrun_error_t *err)
{
  proc_fd_راستہ_t ہدف_بفر;
  صحیح ret;
  ثابت حرف *real_ہدف = ہدف;

  اگر (targetfd >= 0)
    {
      غیرعلامتی طویل سیٹ_flags = flags & ~(MS_REMOUNT | MS_BIND);
      غیرعلامتی طویل clear_flags = REMOUNT_CLEAR_FLAGS & ~سیٹ_flags;
      ret = do_mount_setattr (false, ہدف, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      اگر (LIKELY (ret == 0))
        واپس 0;
      crun_error_release (err);

      get_proc_self_fd_راستہ (ہدف_بفر, targetfd);
      real_ہدف = ہدف_بفر;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  اگر (flags & (MS_REMOUNT | MS_RDONLY))
    ڈیٹا = تہی;

  ret = mount (تہی, real_ہدف, تہی, flags, ڈیٹا);
  اگر (UNLIKELY (ret < 0))
    {
      غیرعلامتی طویل current_flags, remount_flags;
      ساخت statfs sfs;

      ret = statfs (real_ہدف, &sfs);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "statfs `%s`", real_ہدف);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      اگر (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      اگر ((flags | remount_flags) != flags)
        {
          ret = mount (تہی, real_ہدف, تہی, flags | remount_flags, ڈیٹا);
          اگر (LIKELY (ret == 0))
            واپس 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          اگر (current_flags & MS_RDONLY)
            ret = mount (تہی, real_ہدف, تہی, flags | remount_flags | MS_RDONLY, ڈیٹا);
        }
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "remount `%s`", ہدف);
    }
  واپس 0;
}

ساکن صحیح
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret = 0;
  ساخت remount_s *r = get_نجی_ڈیٹا (container)->remounts;
  جب (r)
    {
      ساخت remount_s *اگلا = r->اگلا;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      اگر (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          غیرعلامتی طویل سیٹ_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          غیرعلامتی طویل clear_flags = REMOUNT_CLEAR_FLAGS & ~سیٹ_flags;

          ret = do_mount_setattr (false, r->ہدف, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          اگر (LIKELY (ret == 0))
            {
              free_remount (r);
              r = اگلا;
              جاری;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->ہدف, r->flags, r->ڈیٹا, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ cleanup;

      free_remount (r);
      r = اگلا;
    }

cleanup:
  جب (r)
    {
      ساخت remount_s *اگلا = r->اگلا;
      free_remount (r);
      r = اگلا;
    }

  get_نجی_ڈیٹا (container)->remounts = تہی;
  واپس ret;
}

ساکن صحیح
open_mount_ہدف (libcrun_container_t *container, ثابت حرف *ہدف_rel, libcrun_error_t *err)
{
  ثابت حرف *rootfs = get_نجی_ڈیٹا (container)->rootfs;
  صحیح rootfsfd = get_نجی_ڈیٹا (container)->rootfsfd;

  اگر (rootfsfd < 0)
    واپس crun_make_error (err, 0, "invalid rootfs state");

  واپس محفوظ_openat (rootfsfd, rootfs, ہدف_rel, O_راستہ | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
ساکن صحیح
fsopen_mount (ثابت حرف *type, ثابت حرف *ماخذ_name, ثابت حرف *labeltype, ثابت حرف *label, ثابت حرف *ڈیٹا)
{
#اگر_تعریف_شدہ HAVE_NEW_MOUNT_API
  cleanup_close صحیح fsfd = -1;
  صحیح ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  اگر (UNLIKELY (fsfd < 0))
    واپس fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (خالی) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", ماخذ_name ? ماخذ_name : type, 0);

  اگر (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (ڈیٹا && ڈیٹا[0])
    {
      cleanup_free حرف *ڈیٹا_copy = xstrdup (ڈیٹا);
      حرف *saveptr = تہی;
      حرف *token;

      برائے (token = strtok_r (ڈیٹا_copy, ",", &saveptr); token; token = strtok_r (تہی, ",", &saveptr))
        {
          حرف *eq = strchr (token, '=');
          اگر (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          ورنہ
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, تہی, 0);

          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, تہی, تہی, 0);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#ورنہ
  (خالی) type;
  (خالی) ماخذ_name;
  (خالی) ڈیٹا;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

ساکن صحیح
get_procfd (ساخت نجی_ڈیٹا_s *ڈیٹا, libcrun_error_t *err)
{
  صحیح fd;

  اگر (ڈیٹا->procfd >= 0)
    واپس ڈیٹا->procfd;

  fd = fsopen_mount ("proc", تہی, تہی, تہی, تہی);
  اگر (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "open `/proc`");

  ڈیٹا->procfd = fd;
  واپس fd;
}

ساکن صحیح
get_old_جڑ_fd (ساخت نجی_ڈیٹا_s *ڈیٹا)
{
  اگر (ڈیٹا->old_جڑ_fd >= 0)
    واپس ڈیٹا->old_جڑ_fd;

  ڈیٹا->old_جڑ_fd = open ("/", O_راستہ | O_CLOEXEC);
  واپس ڈیٹا->old_جڑ_fd;
}

ساکن صحیح
fs_move_mount_مقصد (صحیح fd, صحیح dirfd, ثابت حرف *name)
{
#اگر_تعریف_شدہ HAVE_NEW_MOUNT_API
  اگر (name)
    واپس syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  واپس syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#ورنہ
  (خالی) fd;
  (خالی) dirfd;
  (خالی) name;
  (خالی) syscall_move_mount;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

شمار
{
  /* Do not apply any label to the mount.  */
  LABEL_NONE = 0,
  /* Apply the label as a mount option.  */
  LABEL_MOUNT = 1,
  /* Apply the label using setxattr.  */
  LABEL_XATTR = 2,

  LABEL_نقاب = 0x0f,

  /* Do not defer a readonly remount — apply MS_RDONLY directly.  */
  MOUNT_NO_DEFERRED_REMOUNT = 0x10,

  /* Do not remount — keep the flags the mount already has.  */
  MOUNT_NO_REMOUNT = 0x20,
};

ساکن صحیح do_mount (libcrun_container_t *container, ثابت حرف *ماخذ, صحیح targetfd,
                     ثابت حرف *ہدف, ثابت حرف *fstype, غیرعلامتی طویل mountflags,
                     ثابت خالی *ڈیٹا, صحیح extra_flags, libcrun_error_t *err);

ساکن bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, ثابت حرف *option)
{
  size_t i;
  برائے (i = 0; i < mnt->options_len; i++)
    اگر (mnt->options[i] != تہی && strcmp (mnt->options[i], option) == 0)
      واپس true;
  واپس false;
}

ساکن runtime_spec_schema_defs_mount *
ڈھونڈو_mount_for (libcrun_container_t *container, ثابت حرف *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  برائے (i = 0; i < def->mounts_len; i++)
    {
      اگر (strcmp (def->mounts[i]->destination, destination) == 0)
        واپس def->mounts[i];
    }
  واپس تہی;
}

ساکن خالی
warn_tmpfs_fallback_once (ساخت نجی_ڈیٹا_s *نجی_ڈیٹا, ثابت حرف *reason)
{
  اگر (! نجی_ڈیٹا->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      نجی_ڈیٹا->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
ساکن صحیح
get_shared_خالی_خانے_dir_cached (libcrun_container_t *container, حرف **proc_fd_راستہ, libcrun_error_t *err)
{
  ساخت نجی_ڈیٹا_s *نجی_ڈیٹا = get_نجی_ڈیٹا (container);
  cleanup_close صحیح fd = -1;
  cleanup_free حرف *خالی_خانے_dir_راستہ = تہی;
  proc_fd_راستہ_t fd_راستہ;
  صحیح ret;

  /* Fast path: return cached proc fd path if already set up */
  اگر (نجی_ڈیٹا->maskdir_proc_راستہ != تہی)
    {
      اگر (proc_fd_راستہ)
        *proc_fd_راستہ = نجی_ڈیٹا->maskdir_proc_راستہ;
      واپس 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_خالی_خانے_directory_راستہ (&خالی_خانے_dir_راستہ, container->context->state_جڑ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, خالی_خانے_dir_راستہ, true, true, false, MS_PRIVATE, err);
  اگر (fd < 0)
    واپس fd;

  get_self_fd_راستہ (fd_راستہ, fd);
  نجی_ڈیٹا->maskdir_proc_راستہ = xstrdup (fd_راستہ);

  نجی_ڈیٹا->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  اگر (proc_fd_راستہ)
    *proc_fd_راستہ = نجی_ڈیٹا->maskdir_proc_راستہ;
  واپس 0;
}

ساکن bool
is_dev_null (ثابت ساخت stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  واپس S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
ساکن صحیح
ensure_dev_null (صحیح fd, libcrun_error_t *err)
{
  ساخت stat st;

  اگر (UNLIKELY (fstat (fd, &st) < 0))
    واپس crun_make_error (err, errno, "stat `/dev/null`");

  اگر (UNLIKELY (! is_dev_null (&st)))
    واپس crun_make_error (err, 0, "`/dev/null` is not the null device");

  واپس 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
ساکن bool
is_خالی_خانے_ro_tmpfs (صحیح fd)
{
  ساخت statfs sfs;

  اگر (fstatfs (fd, &sfs) < 0)
    واپس false;

  واپس sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

ساکن صحیح
mount_masked_dir (libcrun_container_t *container, صحیح pathfd, ثابت حرف *rel_راستہ, libcrun_error_t *err)
{
  ساخت نجی_ڈیٹا_s *نجی_ڈیٹا = get_نجی_ڈیٹا (container);
  cleanup_close صحیح newfd = -1;
  libcrun_error_t tmp_err = تہی;
  صحیح ret;

  اگر (نجی_ڈیٹا->maskdir_fd < 0)
    {
      اگر (نجی_ڈیٹا->maskdir_bind_failed)
        جاؤ fallback_مقصد_tmpfs;

      ret = get_shared_خالی_خانے_dir_cached (container, تہی, &tmp_err);
      اگر (ret < 0)
        {
          نجی_ڈیٹا->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (نجی_ڈیٹا, tmp_err->پیغام);
          crun_error_release (&tmp_err);
          جاؤ fallback_مقصد_tmpfs;
        }
    }

  اگر (نجی_ڈیٹا->maskdir_fd >= 0)
    {
      صحیح rootfsfd = نجی_ڈیٹا->rootfsfd;
      ساخت stat before, after;

      ret = fstat (نجی_ڈیٹا->maskdir_fd, &before);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "fstat masked dir `%s`", rel_راستہ);

      ret = fs_move_mount_مقصد (نجی_ڈیٹا->maskdir_fd, pathfd, تہی);
      اگر (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_راستہ, true, true, false, MS_PRIVATE, err);
          اگر (UNLIKELY (newfd < 0))
            واپس newfd;

          ret = fstat (newfd, &after);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "fstat masked dir `%s`", rel_راستہ);

          اگر (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            واپس crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_راستہ);

          عارضی_FAILURE_RETRY (close (نجی_ڈیٹا->maskdir_fd));
          نجی_ڈیٹا->maskdir_fd = newfd;
          newfd = -1;

          واپس 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_راستہ);

      عارضی_FAILURE_RETRY (close (نجی_ڈیٹا->maskdir_fd));
      نجی_ڈیٹا->maskdir_fd = -1;
      نجی_ڈیٹا->maskdir_move_failed = true;
    }

  نجی_ڈیٹا->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (نجی_ڈیٹا, tmp_err ? tmp_err->پیغام : "unknown error");
  crun_error_release (&tmp_err);

fallback_مقصد_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_راستہ);
  ret = do_mount (container, "tmpfs", pathfd, rel_راستہ, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  اگر (! نجی_ڈیٹا->maskdir_move_failed)
    {
      ret = get_bind_mount (نجی_ڈیٹا->rootfsfd, rel_راستہ, true, true, false, MS_PRIVATE, &tmp_err);
      اگر (ret >= 0 && is_خالی_خانے_ro_tmpfs (ret))
        نجی_ڈیٹا->maskdir_fd = ret;
      ورنہ
        {
          اگر (ret >= 0)
            عارضی_FAILURE_RETRY (close (ret));
          نجی_ڈیٹا->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  واپس 0;
}

ساکن صحیح
do_masked_or_readonly_راستہ (libcrun_container_t *container, ثابت حرف *rel_راستہ, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  غیرعلامتی طویل mount_flags = 0;
  ثابت حرف *rootfs = get_نجی_ڈیٹا (container)->rootfs;
  cleanup_close صحیح pathfd = -1;
  ساخت statfs sfs;
  صحیح ret;
  mode_t منوال;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  اگر (UNLIKELY (rel_راستہ == تہی))
    واپس crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  اگر (rel_راستہ[0] == '/')
    rel_راستہ++;

  pathfd = محفوظ_openat (get_نجی_ڈیٹا (container)->rootfsfd, rootfs, rel_راستہ, O_راستہ | O_CLOEXEC, 0, err);
  اگر (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      اگر (errno != ENOENT && errno != EACCES)
        واپس pathfd;

      اگر (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_راستہ);
      crun_error_release (err);
      واپس 0;
    }

  اگر (readonly)
    {
      صحیح procfd = get_procfd (get_نجی_ڈیٹا (container), err);
      proc_fd_راستہ_t ماخذ_بفر;
      cleanup_close صحیح mountfd = -1;

      اگر (UNLIKELY (procfd < 0))
        واپس procfd;

      get_self_fd_راستہ (ماخذ_بفر, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      اگر (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, ماخذ_بفر, true, true, false, MS_PRIVATE, err);
          اگر (mountfd >= 0)
            ret = fs_move_mount_مقصد (mountfd, pathfd, تہی);
          ورنہ
            ret = -1;

          اگر (ret < 0)
            crun_error_release (err);
        }

      اگر (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          اگر (keep_flags)
            {
              cleanup_close صحیح sfd = openat (procfd, ماخذ_بفر, O_راستہ | O_CLOEXEC);
              اگر (UNLIKELY (sfd < 0))
                واپس crun_make_error (err, errno, "openat procfd for `%s`", rel_راستہ);

              ret = fstatfs (sfd, &sfs);
              اگر (UNLIKELY (ret < 0))
                واپس crun_make_error (err, errno, "fstatfs `%s`", rel_راستہ);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, ماخذ_بفر, true, true, false, MS_PRIVATE, err);
          اگر (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_راستہ, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              اگر (UNLIKELY (ret < 0))
                واپس ret;

              ret = fs_move_mount_مقصد (mountfd, pathfd, تہی);
              اگر (UNLIKELY (ret < 0))
                واپس crun_make_error (err, errno, "move mount for readonly path `%s`", rel_راستہ);
            }
          ورنہ
            {
              proc_fd_راستہ_t abs_ماخذ;

              crun_error_release (err);
              get_proc_self_fd_راستہ (abs_ماخذ, pathfd);
              ret = do_mount (container, abs_ماخذ, pathfd, rel_راستہ, تہی, mount_flags, تہی,
                              LABEL_NONE, err);
              اگر (UNLIKELY (ret < 0))
                واپس ret;
            }
        }
    }
  ورنہ
    {
      ret = get_file_type_fd (pathfd, &منوال);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "cannot stat `%s`", rel_راستہ);

      اگر ((منوال & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_راستہ, err);
      ورنہ
        {
          cleanup_close صحیح nullfd = -1;
          cleanup_close صحیح mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_راستہ | O_CLOEXEC);
          اگر (UNLIKELY (nullfd < 0))
            واپس crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          اگر (mountfd >= 0)
            ret = fs_move_mount_مقصد (mountfd, pathfd, تہی);

          اگر (mountfd < 0 || ret < 0)
            {
              proc_fd_راستہ_t null_راستہ;

              crun_error_release (err);
              get_proc_self_fd_راستہ (null_راستہ, nullfd);
              ret = do_mount (container, null_راستہ, pathfd, rel_راستہ, تہی, MS_BIND | MS_RDONLY, تہی,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس 0;
}

ساکن ثابت حرف *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  ثابت حرف *context_type;

  context_type = ڈھونڈو_annotation (container, "run.oci.mount_context_type");
  اگر (context_type)
    {
      اگر (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          واپس تہی;
        }
      واپس context_type;
    }

  واپس "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
ساکن صحیح
diagnose_mount_failure (صحیح ret, libcrun_error_t *err, libcrun_container_t *container,
                        ثابت حرف *fstype)
{
  اگر (fstype && strcmp (fstype, "cgroup2") == 0 && get_نجی_ڈیٹا (container)->unified_cgroup_راستہ)
    {
      اگر (access (get_نجی_ڈیٹا (container)->unified_cgroup_راستہ, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          واپس ret;
        }
    }
  واپس ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
ساکن صحیح
sysfs_userns_fallback (libcrun_container_t *container, ثابت حرف *ہدف,
                       ثابت حرف *real_ہدف, صحیح targetfd,
                       غیرعلامتی طویل mountflags, ثابت حرف *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close صحیح mountfd = -1;
  cleanup_close صحیح sysfd = -1;
  صحیح sys_old_جڑ_fd = -1;
  صحیح ret;

  اگر (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    واپس 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_اندر_user_namespace (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (ret == 0)
    واپس 1;

  cgroup_mount = ڈھونڈو_mount_for (container, "/sys/fs/cgroup");
  sys_old_جڑ_fd = get_old_جڑ_fd (get_نجی_ڈیٹا (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  اگر (sys_old_جڑ_fd >= 0)
    mountfd = get_bind_mount (sys_old_جڑ_fd, "sys", true, true, false, MS_PRIVATE, err);
  اگر (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  اگر (cgroup_mount == تہی)
    {
      اگر (mountfd >= 0)
        {
          ret = fs_move_mount_مقصد (mountfd, targetfd, تہی);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "move mount `/sys` to `%s`", real_ہدف);
        }
      ورنہ
        {
          crun_error_release (err);
          اگر (sys_old_جڑ_fd >= 0)
            {
              cleanup_free حرف *sys_راستہ = تہی;
              proc_fd_راستہ_t sys_جڑ_راستہ;

              get_proc_self_fd_راستہ (sys_جڑ_راستہ, sys_old_جڑ_fd);
              xasprintf (&sys_راستہ, "%s/sys", sys_جڑ_راستہ);
              ret = mount (sys_راستہ, real_ہدف, تہی, MS_BIND | MS_REC, تہی);
            }
          ورنہ
            ret = mount ("/sys", real_ہدف, تہی, MS_BIND | MS_REC, تہی);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_ہدف (container, ہدف, err);
      اگر (UNLIKELY (sysfd < 0))
        واپس sysfd;

      ret = do_remount (sysfd, ہدف,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        تہی, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      واپس do_masked_or_readonly_راستہ (container, "/sys/fs/cgroup", true, false, err);
    }

  اگر (UNLIKELY (mountfd < 0))
    واپس mountfd;

  ret = fs_move_mount_مقصد (mountfd, targetfd, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "move mount to `%s`", real_ہدف);

  sysfd = open_mount_ہدف (container, ہدف, err);
  اگر (UNLIKELY (sysfd < 0))
    واپس sysfd;

  ret = do_remount (sysfd, ہدف,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_راستہ (container, "/sys/fs/cgroup", true, false, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

#اگر_تعریف_شدہ HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
ساکن خالی
apply_selinux_xattr (libcrun_container_t *container, صحیح fd, صحیح extra_flags, ثابت حرف *label)
{
  libcrun_error_t tmp_err = تہی;
  صحیح procfd;

  اگر ((extra_flags & LABEL_نقاب) != LABEL_XATTR)
    واپس;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_نجی_ڈیٹا (container), &tmp_err);
  اگر (procfd >= 0)
    {
      proc_fd_راستہ_t proc_self_راستہ;
      cleanup_close صحیح xfd = -1;

      get_self_fd_راستہ (proc_self_راستہ, fd);
      xfd = openat (procfd, proc_self_راستہ, O_RDONLY | O_CLOEXEC);
      اگر (xfd >= 0)
        (خالی) fsetxattr (xfd, "security.selinux", label, لڑی_لمبائی (label), 0);
    }
  crun_error_release (&tmp_err);
}
#اگر_ختم

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
ساکن صحیح
do_mount_replace_rootfs (libcrun_container_t *container, صحیح *fd, libcrun_error_t *err)
{
  صحیح procfd = get_procfd (get_نجی_ڈیٹا (container), err);
  صحیح tmp;
  صحیح ret;

  اگر (UNLIKELY (procfd < 0))
    واپس procfd;

  {
    cleanup_close صحیح nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    اگر (UNLIKELY (nsfd < 0))
      واپس crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    اگر (UNLIKELY (ret < 0))
      واپس crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_نجی_ڈیٹا (container)->rootfs, O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (*fd < 0))
    واپس crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  اگر (UNLIKELY (tmp < 0))
    واپس crun_make_error (err, errno, "dup");

  عارضی_FAILURE_RETRY (close (get_نجی_ڈیٹا (container)->rootfsfd));
  get_نجی_ڈیٹا (container)->rootfsfd = tmp;

  واپس 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
ساکن صحیح
apply_اشاعت_flags (ثابت حرف *ہدف, صحیح targetfd, ثابت حرف *real_ہدف,
                         غیرعلامتی طویل mountflags, libcrun_error_t *err)
{
  bool اشاعت_مکمل = false;
  صحیح ret;

  اگر (targetfd >= 0)
    {
      libcrun_error_t tmp_err = تہی;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, ہدف, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      اگر (LIKELY (ret == 0))
        اشاعت_مکمل = true;
      ورنہ
        crun_error_release (&tmp_err);
    }

  اگر (! اشاعت_مکمل)
    {
      ret = mount (تہی, real_ہدف, تہی, mountflags & ALL_PROPAGATIONS, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "set propagation for `%s`", ہدف);
    }

  واپس 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
ساکن صحیح
جدول_وقت_or_do_remount (libcrun_container_t *container, صحیح *fd, صحیح targetfd,
                        ثابت حرف *ہدف, ثابت حرف *real_ہدف,
                        bool single_instance, غیرعلامتی طویل mountflags,
                        ثابت خالی *ڈیٹا, صحیح extra_flags, libcrun_error_t *err)
{
  غیرعلامتی طویل remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  ساخت remount_s *r;

  اگر ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    واپس do_remount (*fd >= 0 ? *fd : targetfd, real_ہدف, remount_flags, ڈیٹا, err);

  اگر (*fd < 0)
    {
      *fd = dup (targetfd);
      اگر (UNLIKELY (*fd < 0))
        واپس crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), ہدف, remount_flags, ڈیٹا, get_نجی_ڈیٹا (container)->remounts);
  get_نجی_ڈیٹا (container)->remounts = r;

  واپس 0;
}

ساکن صحیح
do_mount (libcrun_container_t *container, ثابت حرف *ماخذ, صحیح targetfd,
          ثابت حرف *ہدف, ثابت حرف *fstype, غیرعلامتی طویل mountflags, ثابت خالی *ڈیٹا,
          صحیح extra_flags, libcrun_error_t *err)
{
  cleanup_free حرف *ڈیٹا_with_label = تہی;
  ثابت حرف *ڈیٹا_without_label = ڈیٹا;
  proc_fd_راستہ_t ہدف_بفر;
  ثابت حرف *context_type = تہی;
  cleanup_close صحیح ms_move_fd = -1;
  ثابت حرف *real_ہدف = ہدف;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close صحیح fd = -1;
  ثابت حرف *label = تہی;
  صحیح ret = 0;

  اگر (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  ورنہ
    extra_flags = (extra_flags & ~LABEL_نقاب) | LABEL_NONE;

  اگر (targetfd >= 0)
    {
      get_proc_self_fd_راستہ (ہدف_بفر, targetfd);
      real_ہدف = ہدف_بفر;
      needs_remount = true;
    }

  اگر ((extra_flags & LABEL_نقاب) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      اگر (UNLIKELY (context_type == تہی))
        واپس -1;

      ret = جوڑو_selinux_mount_label (&ڈیٹا_with_label, ڈیٹا, label, context_type, err);
      اگر (ret < 0)
        واپس ret;
      ڈیٹا = ڈیٹا_with_label;
    }

  اگر (mountflags & MS_MOVE)
    {
      اگر ((mountflags & MS_BIND) || fstype)
        واپس crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (ماخذ, real_ہدف, تہی, MS_MOVE, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "move mount `%s` to `%s`", ماخذ, ہدف);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_ہدف (container, ہدف, err);
      اگر (UNLIKELY (ms_move_fd < 0))
        واپس ms_move_fd;
      targetfd = ms_move_fd;
    }

  اگر ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      غیرعلامتی طویل flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      اگر (targetfd >= 0)
        {
          اگر (mountflags & MS_BIND)
            {
              cleanup_close صحیح bindfd = -1;
              libcrun_error_t tmp_err = تہی;

              bindfd = get_bind_mount (AT_FDCWD, ماخذ,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              اگر (bindfd < 0)
                {
                  صحیح old_جڑ_fd = get_old_جڑ_fd (get_نجی_ڈیٹا (container));

                  اگر (old_جڑ_fd >= 0 && ماخذ && ماخذ[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_جڑ_fd, (ماخذ[1] == '\0') ? "." : ماخذ + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              اگر (bindfd >= 0)
                ret = fs_move_mount_مقصد (bindfd, targetfd, تہی);
              اگر (ret < 0)
                crun_error_release (&tmp_err);
            }
          ورنہ
            {
              cleanup_close صحیح newfs = fsopen_mount (fstype, ماخذ, context_type, label, ڈیٹا_without_label);
              اگر (newfs >= 0)
                ret = fs_move_mount_مقصد (newfs, targetfd, تہی);
            }
        }

      اگر (ret < 0)
        {
          ret = mount (ماخذ, real_ہدف, fstype, flags, ڈیٹا);
          اگر (ret < 0 && (mountflags & MS_BIND) && ماخذ && ماخذ[0] == '/')
            {
              صحیح old_جڑ_fd = get_old_جڑ_fd (get_نجی_ڈیٹا (container));

              اگر (old_جڑ_fd >= 0)
                {
                  cleanup_free حرف *proc_ماخذ_buf = تہی;
                  proc_fd_راستہ_t old_جڑ_راستہ;

                  get_proc_self_fd_راستہ (old_جڑ_راستہ, old_جڑ_fd);
                  xasprintf (&proc_ماخذ_buf, "%s/%s", old_جڑ_راستہ, ماخذ + 1);
                  ret = mount (proc_ماخذ_buf, real_ہدف, fstype, flags, ڈیٹا);
                }
            }
        }
      اگر (UNLIKELY (ret < 0))
        {
          صحیح saved_errno = errno;

          ret = sysfs_userns_fallback (container, ہدف, real_ہدف, targetfd,
                                       mountflags, fstype, err);
          اگر (ret == 0)
            واپس 0;
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", ماخذ, ہدف);

          واپس diagnose_mount_failure (ret, err, container, fstype);
        }

      اگر (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_ہدف (container, ہدف, err);
          اگر (UNLIKELY (fd < 0))
            واپس fd;

          /* We are replacing the rootfs, reopen it.  */
          اگر (is_خالی_خانے_string (ہدف))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              اگر (UNLIKELY (ret < 0))
                واپس ret;
            }

#اگر_تعریف_شدہ HAVE_FGETXATTR
          apply_selinux_xattr (container, fd, extra_flags, label);
#اگر_ختم

          targetfd = fd;

          /* The bind mount shadowed the original targetfd, so refresh
             real_target to point to the reopened mountpoint.  Otherwise the
             classic mount(2) fallbacks below (used when mount_setattr is not
             available, e.g. kernels < 5.12) operate on the shadowed path and
             fail with EINVAL.  */
          get_proc_self_fd_راستہ (ہدف_بفر, targetfd);
          real_ہدف = ہدف_بفر;
        }
    }

  اگر (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_اشاعت_flags (ہدف, targetfd, real_ہدف, mountflags, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  اگر (ڈیٹا && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  اگر (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = جدول_وقت_or_do_remount (container, &fd, targetfd, ہدف, real_ہدف,
                                    single_instance, mountflags, ڈیٹا, extra_flags, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس ret;
}

ساکن خالی
try_umount (صحیح targetfd, ثابت حرف *ہدف)
{
  proc_fd_راستہ_t ہدف_بفر;
  ثابت حرف *real_ہدف = ہدف;

  اگر (targetfd >= 0)
    {
      get_proc_self_fd_راستہ (ہدف_بفر, targetfd);
      real_ہدف = ہدف_بفر;
    }

  umount2 (real_ہدف, MNT_DETACH);
}

ساکن bool
container_has_cgroupns (libcrun_container_t *container)
{
  واپس get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWCGROUP;
}

ساکن صحیح
do_mount_cgroup_v2 (libcrun_container_t *container, صحیح targetfd, ثابت حرف *ہدف,
                    غیرعلامتی طویل mountflags, ثابت حرف *ڈیٹا, libcrun_error_t *err)
{
  صحیح ret;
  صحیح cgroup_منوال;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  ret = do_mount (container, "cgroup2", targetfd, ہدف, "cgroup2", mountflags, ڈیٹا, LABEL_NONE, err);
  اگر (UNLIKELY (ret < 0))
    {
      صحیح saved_errno = crun_error_get_errno (err);
      اگر (saved_errno == EPERM || saved_errno == EBUSY)
        {
          ثابت حرف *unified_cgroup_راستہ;
          ثابت حرف *src_cgroup;

          crun_error_release (err);

          اگر (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, ہدف, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              اگر (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, ہدف, "cgroup2", mountflags, ڈیٹا, LABEL_NONE, err);
                  اگر (LIKELY (ret == 0))
                    واپس ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, ہدف);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_راستہ = get_نجی_ڈیٹا (container)->unified_cgroup_راستہ;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_راستہ && container_has_cgroupns (container) ? unified_cgroup_راستہ : CGROUP_جڑ;
          واپس do_mount (container, src_cgroup, targetfd, ہدف, تہی,
                           MS_BIND | mountflags, تہی, LABEL_NONE, err);
        }

      واپس ret;
    }

  واپس 0;
}

ساکن صحیح
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, ثابت حرف *ماخذ, صحیح targetfd, ثابت حرف *ہدف,
                            غیرعلامتی طویل mountflags, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_close صحیح fd = -1;
  ثابت حرف *subsystem = "systemd";
  cleanup_free حرف *subsystem_راستہ = تہی;
  cleanup_close صحیح tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, ماخذ, targetfd, ہدف, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_ہدف (container, ہدف, err);
  اگر (UNLIKELY (tmpfsdirfd < 0))
    واپس tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_راستہ, err, ہدف, subsystem, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_راستہ | O_DIRECTORY | O_NOFOLLOW);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "openat `%s`", subsystem_راستہ);

  واپس do_mount (container, "cgroup", fd, subsystem_راستہ, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

ساکن صحیح
do_mount_cgroup_v1 (libcrun_container_t *container, ثابت حرف *ماخذ, صحیح targetfd, ثابت حرف *ہدف,
                    غیرعلامتی طویل mountflags, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free حرف *content = تہی;
  حرف *from;
  cleanup_close صحیح tmpfsdirfd = -1;
  حرف *saveptr = تہی;

  ret = do_mount (container, ماخذ, targetfd, ہدف, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  tmpfsdirfd = open_mount_ہدف (container, ہدف, err);
  اگر (UNLIKELY (tmpfsdirfd < 0))
    واپس tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    صحیح procfd = get_procfd (get_نجی_ڈیٹا (container), err);
    اگر (UNLIKELY (procfd < 0))
      واپس procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, تہی, err);
  }
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (UNLIKELY (content == تہی || content[0] == '\0'))
    واپس crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  برائے (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (تہی, "\n", &saveptr))
    {
      cleanup_free حرف *ماخذ_راستہ = تہی;
      cleanup_free حرف *ماخذ_subsystem = تہی;
      cleanup_free حرف *subsystem_راستہ = تہی;
      حرف *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close صحیح subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      اگر (subsystem[0] == '\0')
        جاری;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = لڑی_تلاش (subsystem, "name=");
      اگر (it)
        subsystem = it + 5;

      اگر (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      اگر (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&ماخذ_subsystem, err, CGROUP_جڑ, subsystem, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      /* if there is already a mount specified, do not add a default one.  */
      اگر (ڈھونڈو_mount_for (container, ماخذ_subsystem) != تہی)
        جاری;

      ret = append_paths (&ماخذ_راستہ, err, ماخذ_subsystem, subpath, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = append_paths (&subsystem_راستہ, err, ہدف, subsystem, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "mkdirat `%s`", subsystem_راستہ);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_راستہ | O_DIRECTORY | O_NOFOLLOW);
      اگر (UNLIKELY (subsystemfd < 0))
        واپس crun_make_error (err, errno, "open `%s`", subsystem_راستہ);

      اگر (container_has_cgroupns (container))
        {
          ret = do_mount (container, ماخذ_راستہ, subsystemfd, subsystem_راستہ, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  جاری;
                }
              واپس ret;
            }
        }
      ورنہ
        {
          ret = do_mount (container, ماخذ_راستہ, subsystemfd, subsystem_راستہ, تہی, MS_BIND | mountflags, تہی,
                          LABEL_NONE, err);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (crun_error_get_errno (err) != ENOENT)
                واپس ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, ماخذ_subsystem, subsystemfd, subsystem_راستہ, تہی, MS_BIND | mountflags,
                              تہی, LABEL_NONE, err);
              اگر (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  اگر (crun_error_get_errno (err) != ENOENT)
                    واپس ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

ساکن صحیح
do_mount_cgroup (libcrun_container_t *container, ثابت حرف *ماخذ, صحیح targetfd, ثابت حرف *ہدف,
                 غیرعلامتی طویل mountflags, ثابت حرف *ڈیٹا, libcrun_error_t *err)
{
  صحیح cgroup_منوال;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  انتخاب (cgroup_منوال)
    {
    حالت CGROUP_منوال_UNIFIED:
      واپس do_mount_cgroup_v2 (container, targetfd, ہدف, mountflags, ڈیٹا, err);
    حالت CGROUP_منوال_LEGACY:
    حالت CGROUP_منوال_HYBRID:
      واپس do_mount_cgroup_v1 (container, ماخذ, targetfd, ہدف, mountflags, err);
    }

  واپس crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_منوال);
}

ساخت device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
ساکن خالی
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  ساخت نجی_ڈیٹا_s *نجی_ڈیٹا = get_نجی_ڈیٹا (container);
  size_t i;

  اگر (نجی_ڈیٹا->needed_devs_fds)
    {
      ساخت libcrun_fd_map *dev_fds = نجی_ڈیٹا->needed_devs_fds;

      برائے (i = 0; needed_devs[i].راستہ; i++)
        {
          اگر (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            جاری;

          صحیح fd = syscall_open_درخت (AT_FDCWD, needed_devs[i].راستہ,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          اگر (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          ورنہ اگر (fd >= 0)
            close (fd);
        }
    }

  اگر (نجی_ڈیٹا->notify_socket_درخت_fd < 0
      && نجی_ڈیٹا->host_notify_socket_راستہ)
    {
      صحیح fd = syscall_open_درخت (AT_FDCWD, نجی_ڈیٹا->host_notify_socket_راستہ,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      اگر (fd >= 0)
        نجی_ڈیٹا->notify_socket_درخت_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
ساکن ثابت حرف *
relative_راستہ_under_dev (ثابت حرف *راستہ)
{
  اگر (راستہ[0] != '/')
    واپس راستہ;
  اگر (لڑی_موازنہ (راستہ, "/dev/", 5) == 0)
    {
      اگر (strchr (راستہ + 5, '/'))
        واپس تہی;
      واپس راستہ + 5;
    }
  واپس تہی;
}

ساکن صحیح
mknod_and_سیٹ_attrs (libcrun_container_t *container, صحیح dirfd, ثابت حرف *name, ساخت device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  proc_fd_راستہ_t fd_راستہ;
  ثابت حرف *what = "new";
  bool exists = false;
  ساخت stat st;
  صحیح procfd;
  صحیح ret;

  ret = mknodat (dirfd, name, device->منوال | type, dev);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno != EEXIST)
        واپس crun_make_error (err, errno, "mknodat `%s`", device->راستہ);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_راستہ | O_NOFOLLOW | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "open %s device `%s`", what, device->راستہ);

  ret = fstat (fd, &st);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fstat %s device `%s`", what, device->راستہ);

  اگر (UNLIKELY ((st.st_mode & S_IFMT) != type))
    واپس crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->راستہ, st.st_mode & S_IFMT, type);

  اگر (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    واپس crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->راستہ, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  اگر (exists)
    واپس 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_نجی_ڈیٹا (container), err);
  اگر (UNLIKELY (procfd < 0))
    واپس procfd;

  get_self_fd_راستہ (fd_راستہ, fd);
  ret = fchmodat (procfd, fd_راستہ, device->منوال, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fchmod `%s`", device->راستہ);

  ret = fchownat (fd, "", device->uid, device->gid, AT_خالی_خانے_راستہ);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fchown `%s`", device->راستہ);

  واپس 1;
}

صحیح
libcrun_create_dev (libcrun_container_t *container, صحیح devfd, صحیح srcfd,
                    ساخت device_s *device, bool binds, bool ensure_والد_dir,
                    libcrun_error_t *err)
{
  صحیح ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  ثابت حرف *fullname = device->راستہ;
  ثابت حرف *rootfs = get_نجی_ڈیٹا (container)->rootfs;
  اگر (is_خالی_خانے_string (fullname))
    واپس crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free حرف *normalized_راستہ = xstrdup (fullname);
  consume_trailing_slashes (normalized_راستہ);
  اگر (normalized_راستہ[0] == '\0')
    strcpy (normalized_راستہ, "/");
  ثابت حرف *rel_dev = relative_راستہ_under_dev (normalized_راستہ);

  اگر (binds)
    {
      cleanup_close صحیح fd = -1;

      اگر (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_راستہ | O_NONBLOCK);
          اگر (UNLIKELY (fd < 0))
            {
              اگر (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              اگر (UNLIKELY (fd < 0))
                واپس crun_make_error (err, errno, "openat `%s`", device->راستہ);
            }
        }
      ورنہ
        {
          ثابت حرف *rel_راستہ = consume_slashes (normalized_راستہ);

          fd = crun_محفوظ_create_and_open_ref_at (false, get_نجی_ڈیٹا (container)->rootfsfd, rootfs, rel_راستہ, 0755, err);
          اگر (UNLIKELY (fd < 0))
            واپس fd;
        }

      اگر (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_راستہ, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          اگر (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              اگر (LIKELY (ret >= 0))
                واپس 0;
            }
          ورنہ
            crun_error_release (err);
        }

      {
        cleanup_close صحیح mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        اگر (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_راستہ, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            اگر (LIKELY (ret == 0))
              {
                ret = fs_move_mount_مقصد (mountfd, fd, تہی);
                اگر (LIKELY (ret == 0))
                  واپس 0;
              }
            ورنہ
              crun_error_release (err);
          }
        ورنہ
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_راستہ, تہی, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, تہی, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      اگر (rel_dev)
        {
          ret = mknod_and_سیٹ_attrs (container, devfd, rel_dev, device, type, dev, err);
          اگر (ret <= 0)
            واپس ret;
        }
      ورنہ
        {
          cleanup_close صحیح dirfd = -1;
          cleanup_free حرف *dirname = تہی;
          حرف *basename, *ملا;

          dirname = xstrdup (normalized_راستہ);

          ملا = strrchr (dirname, '/');
          اگر (ملا)
            *ملا = '\0';

          basename = ملا ? ملا + 1 : dirname;

          اگر (dirname[0] == '\0')
            {
              dirfd = dup (get_نجی_ڈیٹا (container)->rootfsfd);
              اگر (UNLIKELY (dirfd < 0))
                واپس crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          ورنہ
            {
              dirfd = محفوظ_openat (get_نجی_ڈیٹا (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_راستہ | O_CLOEXEC, 0, err);
              اگر (dirfd < 0 && ensure_والد_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_محفوظ_create_and_open_ref_at (true, get_نجی_ڈیٹا (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              اگر (UNLIKELY (dirfd < 0))
                واپس dirfd;
            }

          ret = mknod_and_سیٹ_attrs (container, dirfd, basename, device, type, dev, err);
          اگر (ret <= 0)
            واپس ret;
        }
    }
  واپس 0;
}

ساخت symlink_s
{
  ثابت حرف *راستہ;
  ثابت حرف *ہدف;
  bool force;
};

ساکن ساخت symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { تہی, تہی, false } };

ساکن صحیح
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  صحیح ret;
  size_t i;
  ساخت device_s *it;
  cleanup_close صحیح devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت حرف *rootfs = get_نجی_ڈیٹا (container)->rootfs;
  cleanup_close_map ساخت libcrun_fd_map *dev_fds = تہی;
  cleanup_close_map ساخت libcrun_fd_map *needed_fds = تہی;

  dev_fds = get_نجی_ڈیٹا (container)->dev_fds;
  get_نجی_ڈیٹا (container)->dev_fds = تہی;

  needed_fds = get_نجی_ڈیٹا (container)->needed_devs_fds;
  get_نجی_ڈیٹا (container)->needed_devs_fds = تہی;

  اگر (! def || ! def->linux)
    واپس 0;

  devfd = محفوظ_openat (get_نجی_ڈیٹا (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_راستہ | O_DIRECTORY, 0, err);
  اگر (UNLIKELY (devfd < 0))
    واپس devfd;

  برائے (i = 0; i < def->linux->devices_len; i++)
    {
      ساخت device_s device = {
        def->linux->devices[i]->راستہ,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_منوال,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      اگر (! def->linux->devices[i]->file_منوال_present)
        device.منوال = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  برائے (it = needed_devs, i = 0; it->راستہ; it++, i++)
    {
      cleanup_close صحیح srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      اگر (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  برائے (i = 0; symlinks[i].ہدف; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].راستہ, devfd, symlinks[i].ہدف);
      اگر (UNLIKELY (ret < 0))
        {
          صحیح saved_errno = errno;

          اگر (errno == EEXIST && ! symlinks[i].force)
            جاری;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          اگر (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].ہدف, 0);
              اگر (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].ہدف, AT_REMOVEDIR);
              اگر (ret < 0 && errno == EBUSY)
                {
                  cleanup_close صحیح tfd = openat (devfd, symlinks[i].ہدف, O_CLOEXEC | O_راستہ | O_NOFOLLOW);
                  اگر (tfd >= 0)
                    {
                      proc_fd_راستہ_t procpath;

                      get_proc_self_fd_راستہ (procpath, tfd);
                      اگر (umount2 (procpath, MNT_DETACH) == 0)
                        جاؤ retry_unlink;
                    }
                }
              اگر (ret == 0)
                جاؤ retry_symlink;
            }
          واپس crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].ہدف);
        }
    }

  اگر (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
ساکن ثابت حرف *
اگلا_راستہ_component (ثابت حرف *p, size_t *len)
{
  برائے (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      اگر (*len != 1 || p[0] != '.')
        واپس p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
ساکن bool
same_راستہ (ثابت حرف *a, ثابت حرف *b)
{
  size_t la, lb;

  برائے (;;)
    {
      a = اگلا_راستہ_component (a, &la);
      b = اگلا_راستہ_component (b, &lb);
      اگر (la != lb || memcmp (a, b, la) != 0)
        واپس false;
      اگر (la == 0)
        واپس true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
ساکن bool
is_duplicate_راستہ (حرف **paths, size_t i)
{
  size_t j;

  اگر (paths[i] == تہی)
    واپس false;

  برائے (j = 0; j < i; j++)
    اگر (paths[j] && same_راستہ (paths[j], paths[i]))
      واپس true;

  واپس false;
}

ساکن صحیح
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  صحیح ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  اگر (! def || ! def->linux)
    واپس 0;

  برائے (i = 0; i < def->linux->masked_paths_len; i++)
    {
      اگر (is_duplicate_راستہ (def->linux->masked_paths, i))
        جاری;

      ret = do_masked_or_readonly_راستہ (container, def->linux->masked_paths[i], false, false, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  برائے (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_راستہ (container, def->linux->readonly_paths[i], true, true, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس 0;
}

ساکن صحیح
do_محور (libcrun_container_t *container, ثابت حرف *rootfs, libcrun_error_t *err)
{
  cleanup_close صحیح oldrootfd = -1;
  cleanup_close صحیح newrootfd = -1;
  صحیح ret;

  oldrootfd = open ("/", O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (oldrootfd < 0))
    واپس crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (newrootfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = محور_جڑ (".", ".");
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, تہی, -1, ".", تہی, MS_REC | MS_PRIVATE, تہی, LABEL_MOUNT, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = umount2 (".", MNT_DETACH);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "umount2 oldroot");

  کرو
    {
      ret = umount2 (".", MNT_DETACH);
      اگر (ret < 0 && errno == EINVAL)
        توڑو;
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "umount2 oldroot");
  } جب (ret == 0);

  ret = chdir ("/");
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "chdir to newroot");

  واپس 0;
}

ساکن صحیح
append_tmpfs_منوال_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, حرف **ڈیٹا, libcrun_error_t *err)
{
  ثابت حرف *rootfs = get_نجی_ڈیٹا (container)->rootfs;
  bool خالی_خانے_ڈیٹا = is_خالی_خانے_string (*ڈیٹا);
  cleanup_close صحیح fd = -1;
  ساخت stat st;
  صحیح ret;

  اگر (*ڈیٹا != تہی && لڑی_تلاش (*ڈیٹا, "mode="))
    واپس 0;

  fd = محفوظ_openat (get_نجی_ڈیٹا (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_راستہ, 0, err);
  اگر (fd < 0)
    {
      اگر (crun_error_get_errno (err) != ENOENT)
        واپس fd;

      crun_error_release (err);
      واپس 0;
    }
  ret = fstat (fd, &st);
  اگر (ret < 0)
    واپس crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (ڈیٹا, "%s%smode=%o", خالی_خانے_ڈیٹا ? "" : *ڈیٹا, خالی_خانے_ڈیٹا ? "" : ",", st.st_mode & 07777);
  واپس 0;
}

ساکن صحیح
get_default_flags (libcrun_container_t *container, ثابت حرف *destination, حرف **ڈیٹا)
{
  اگر (strcmp (destination, "/proc") == 0)
    واپس 0;
  اگر (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *ڈیٹا = xstrdup ("none,name=");
      واپس MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  اگر (strcmp (destination, "/dev") == 0)
    {
      *ڈیٹا = xstrdup ("mode=755");
      واپس MS_NOEXEC | MS_STRICTATIME;
    }
  اگر (strcmp (destination, "/dev/shm") == 0)
    {
      *ڈیٹا = xstrdup ("mode=1777,size=65536k");
      واپس MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  اگر (strcmp (destination, "/dev/mqueue") == 0)
    واپس MS_NOEXEC | MS_NOSUID | MS_NODEV;
  اگر (strcmp (destination, "/dev/pts") == 0)
    {
      اگر (container->host_uid == 0)
        *ڈیٹا = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      ورنہ
        *ڈیٹا = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      واپس MS_NOEXEC | MS_NOSUID;
    }
  اگر (strcmp (destination, "/sys") == 0)
    واپس MS_NOEXEC | MS_NOSUID | MS_NODEV;

  واپس 0;
}

ساکن ثابت حرف *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  واپس ڈھونڈو_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

ساکن صحیح
check_valid_no_follow_file_system (صحیح fd, ثابت حرف *destination, libcrun_error_t *err)
{
  ساخت statfs sfs;

  صحیح ret = fstatfs (fd, &sfs);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "statfs `%s`", destination);

  انتخاب (sfs.f_type)
    {
    حالت PROC_SUPER_MAGIC:
    حالت SYSFS_MAGIC:
    حالت DEVPTS_SUPER_MAGIC:
      واپس crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  واپس 0;
}

ساکن صحیح
محفوظ_create_symlink (صحیح rootfsfd, ثابت حرف *rootfs, ثابت حرف *ہدف, ثابت حرف *destination, libcrun_error_t *err)
{
  cleanup_close صحیح والد_dir_fd = -1;
  cleanup_free حرف *بفر = تہی;
  حرف *part;
  صحیح ret;

  اگر (is_خالی_خانے_string (destination))
    واپس crun_make_error (err, 0, "empty destination for symlink `%s`", ہدف);

  بفر = xstrdup (destination);
  part = dirname (بفر);

  والد_dir_fd = crun_محفوظ_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  اگر (UNLIKELY (والد_dir_fd < 0))
    واپس والد_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (بفر, destination);
  part = basename (بفر);

  ret = symlinkat (ہدف, والد_dir_fd, part);
  اگر (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      اگر (errno == EEXIST)
        {
          cleanup_free حرف *link = تہی;
          ssize_t len;

          len = محفوظ_readlinkat (والد_dir_fd, part, &link, 0, err);
          اگر (UNLIKELY (len < 0))
            واپس len;

          اگر ((((size_t) len) == لڑی_لمبائی (ہدف)) && لڑی_موازنہ (link, ہدف, len) == 0)
            واپس 0;

          واپس crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      واپس crun_make_error (err, errno, "symlinkat `%s`", ہدف);
    }

  واپس 0;
}

ساکن صحیح
handle_copy_symlink (libcrun_container_t *container, ثابت حرف *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     ثابت حرف *link_ہدف, libcrun_error_t *err)
{
  cleanup_free حرف *ہدف = تہی;

  اگر (link_ہدف == تہی)
    {
      صحیح old_جڑ_fd = get_old_جڑ_fd (get_نجی_ڈیٹا (container));
      ثابت حرف *ماخذ = mount->ماخذ;
      ssize_t len;

      اگر (old_جڑ_fd >= 0 && ماخذ[0] == '/')
        len = محفوظ_readlinkat (old_جڑ_fd, ماخذ + 1, &ہدف, 0, err);
      ورنہ
        len = محفوظ_readlinkat (AT_FDCWD, ماخذ, &ہدف, 0, err);
      اگر (UNLIKELY (len < 0))
        واپس len;
      link_ہدف = ہدف;
    }

  واپس محفوظ_create_symlink (get_نجی_ڈیٹا (container)->rootfsfd, rootfs,
                              link_ہدف, mount->destination, err);
}

ساکن صحیح
prepare_sysfs_or_proc_mount (libcrun_container_t *container, ثابت حرف *ہدف,
                             صحیح *targetfd, libcrun_error_t *err)
{
  صحیح ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_نجی_ڈیٹا (container)->rootfsfd, ہدف,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOENT)
        {
          اگر (strchr (ہدف, '/'))
            واپس crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", ہدف);

          ret = mkdirat (get_نجی_ڈیٹا (container)->rootfsfd, ہدف, 0755);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "mkdirat `%s`", ہدف);

          /* Try opening it again.  */
          ret = openat (get_نجی_ڈیٹا (container)->rootfsfd, ہدف,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      ورنہ اگر (errno == ENOTDIR)
        واپس crun_make_error (err, errno, "the target `/%s` is invalid", ہدف);

      اگر (ret < 0)
        واپس crun_make_error (err, errno, "open `%s`", ہدف);
    }

  *targetfd = ret;
  واپس 0;
}

ساکن صحیح
handle_tmpcopyup (libcrun_container_t *container, ثابت حرف *rootfs, ثابت حرف *ہدف,
                  صحیح copy_from_fd, libcrun_error_t *err)
{
  صحیح destfd, ret;
  cleanup_close صحیح tmpfd = copy_from_fd;

  ہدف = consume_slashes (ہدف);

  اگر (is_خالی_خانے_string (ہدف))
    واپس crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = محفوظ_openat (get_نجی_ڈیٹا (container)->rootfsfd, rootfs, ہدف,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  اگر (UNLIKELY (destfd < 0))
    واپس crun_error_wrap (err, "open `%s` to write for tmpcopyup", ہدف);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_مقصد_fd (tmpfd, destfd, ہدف, ہدف, err);
  tmpfd = -1;
  واپس ret;
}

ساکن صحیح
get_mount_label_how (ثابت حرف *type, bool is_sysfs_or_proc)
{
  اگر (is_sysfs_or_proc)
    واپس LABEL_NONE;

  اگر (strcmp (type, "mqueue") == 0)
    واپس LABEL_XATTR;

  واپس LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
ساکن غیرعلامتی طویل
get_bind_mount_locked_flags (صحیح ماخذ_mountfd, ثابت حرف *ماخذ, runtime_spec_schema_defs_mount *mount)
{
  غیرعلامتی طویل cleared = 0;
  ساخت statfs sfs;
  size_t i;
  صحیح ret;

  اگر (ماخذ_mountfd >= 0)
    ret = fstatfs (ماخذ_mountfd, &sfs);
  ورنہ
    ret = statfs (ماخذ, &sfs);
  اگر (UNLIKELY (ret < 0))
    واپس 0;

  برائے (i = 0; i < mount->options_len; i++)
    {
      ثابت ساخت اشاعت_flags_s *prop;

      اگر (mount->options[i] == تہی)
        جاری;

      prop = libcrun_str2mount_flags (mount->options[i]);
      اگر (prop == تہی || (prop->extra_flags & OPTION_RECURSIVE))
        جاری;

      اگر (prop->clear)
        cleared |= prop->flags;
      ورنہ
        cleared &= ~prop->flags;
    }

  واپس sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
ساکن ثابت حرف *
get_راستہ_اندر_rootfs (ثابت حرف *rootfs, ثابت حرف *راستہ)
{
  size_t len;

  اگر (rootfs == تہی || راستہ == تہی)
    واپس تہی;

  len = لڑی_لمبائی (rootfs);
  جب (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  اگر (len == 0)
    واپس تہی;

  اگر (لڑی_موازنہ (راستہ, rootfs, len) != 0 || (راستہ[len] != '/' && راستہ[len] != '\0'))
    واپس تہی;

  واپس consume_slashes (راستہ + len);
}

ساکن صحیح
process_single_mount (libcrun_container_t *container, ثابت حرف *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      ساخت libcrun_fd_map *mount_fds, size_t mount_اشاریہ,
                      ثابت حرف *systemd_cgroup_v1, libcrun_error_t *err)
{
  ثابت حرف *ہدف = consume_slashes (mount->destination);
  cleanup_close صحیح ماخذ_mountfd = -1;
  cleanup_free حرف *ڈیٹا = تہی;
  حرف *type;
  حرف *ماخذ;
  غیرعلامتی طویل flags = 0;
  غیرعلامتی طویل extra_flags = 0;
  mode_t src_منوال = S_IFDIR;
  cleanup_close صحیح copy_from_fd = -1;
  cleanup_close صحیح targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_سیٹ = 0;
  صحیح no_remount = 0;
  صحیح ret;

  اگر (mount_fds)
    ماخذ_mountfd = get_and_reset (&(mount_fds->fds[mount_اشاریہ]));

  type = mount->type;

  اگر (mount->options == تہی)
    flags = get_default_flags (container, mount->destination, &ڈیٹا);
  ورنہ
    {
      size_t j;

      برائے (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &ڈیٹا, &rec_clear, &rec_سیٹ);
    }

  اگر (type == تہی && (flags & MS_BIND) == 0)
    واپس crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  اگر ((flags & MS_BIND) && ماخذ_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      ثابت حرف *rel_ماخذ = get_راستہ_اندر_rootfs (get_نجی_ڈیٹا (container)->host_rootfs, mount->ماخذ);

      اگر (rel_ماخذ)
        {
          غیرعلامتی طویل اشاعت = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          ماخذ_mountfd = get_bind_mount (get_نجی_ڈیٹا (container)->rootfsfd, rel_ماخذ, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, اشاعت, err);
          اگر (UNLIKELY (ماخذ_mountfd < 0))
            واپس crun_error_wrap (err, "open mount source `%s`", mount->ماخذ);
        }
    }

  اگر ((flags & MS_BIND) && mount->ماخذ && has_mount_flag_options (mount)
      && (get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (ماخذ_mountfd, mount->ماخذ, mount);

  اگر (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      اگر (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      اگر (راستہ_is_slash_dev (mount->destination))
        get_نجی_ڈیٹا (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  اگر (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_منوال_if_missing (container, mount, &ڈیٹا, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (mount->ماخذ && (flags & MS_BIND))
    {
      proc_fd_راستہ_t proc_buf;
      ثابت حرف *راستہ = mount->ماخذ;

      اگر ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        واپس crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_نجی_ڈیٹا (container)->copy_symlink_targets
                               && mount_اشاریہ < get_نجی_ڈیٹا (container)->n_copy_symlink_targets
                               && get_نجی_ڈیٹا (container)->copy_symlink_targets[mount_اشاریہ];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      اگر (ماخذ_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_راستہ (proc_buf, ماخذ_mountfd);
          راستہ = proc_buf;

          ret = get_file_type_at (ماخذ_mountfd, &src_منوال, true, تہی);
        }
      ورنہ اگر (is_cached_symlink)
        {
          src_منوال = S_IFLNK;
          ret = 0;
        }
      ورنہ اگر (راستہ[0] == '/')
        {
          صحیح old_جڑ_fd = get_old_جڑ_fd (get_نجی_ڈیٹا (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          اگر (old_جڑ_fd >= 0)
            ret = get_file_type_at (old_جڑ_fd, &src_منوال, nofollow, (راستہ[1] == '\0') ? تہی : راستہ + 1);
          ورنہ
            ret = get_file_type_at (AT_FDCWD, &src_منوال, nofollow, راستہ);
        }
      ورنہ
        ret = get_file_type (&src_منوال, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, راستہ);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "cannot stat `%s`", راستہ);

      اگر (S_ISLNK (src_منوال) && (extra_flags & OPTION_DEST_NOFOLLOW) && ماخذ_mountfd < 0)
        {
          صحیح bind_dirfd = AT_FDCWD;
          ثابت حرف *bind_ماخذ = mount->ماخذ;
          صحیح old_جڑ_fd = get_old_جڑ_fd (get_نجی_ڈیٹا (container));

          اگر (old_جڑ_fd >= 0 && mount->ماخذ && mount->ماخذ[0] == '/')
            {
              bind_dirfd = old_جڑ_fd;
              bind_ماخذ = (mount->ماخذ[1] == '\0') ? "." : mount->ماخذ + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_ماخذ, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          ماخذ_mountfd = ret;
        }
    }

  اگر (S_ISLNK (src_منوال) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      ثابت حرف *cached_ہدف = تہی;

      اگر (get_نجی_ڈیٹا (container)->copy_symlink_targets
          && mount_اشاریہ < get_نجی_ڈیٹا (container)->n_copy_symlink_targets)
        cached_ہدف = get_نجی_ڈیٹا (container)->copy_symlink_targets[mount_اشاریہ];

      ret = handle_copy_symlink (container, rootfs, mount, cached_ہدف, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      mounted = true;
    }
  ورنہ اگر (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, ہدف, &targetfd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      bool is_dir = S_ISDIR (src_منوال);

      اگر (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = محفوظ_openat (get_نجی_ڈیٹا (container)->rootfsfd, rootfs, ہدف, O_راستہ | O_NOFOLLOW | O_CLOEXEC, 0, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, ہدف, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
      ورنہ
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_محفوظ_create_and_open_ref_at (is_dir, get_نجی_ڈیٹا (container)->rootfsfd, rootfs, ہدف, 0755, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          targetfd = ret;
        }
    }

  اگر (extra_flags & OPTION_TMPCOPYUP)
    {
      اگر (strcmp (type, "tmpfs") != 0)
        واپس crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      اگر (UNLIKELY (copy_from_fd < 0))
        {
          اگر (errno != ENOTDIR)
            واپس crun_make_error (err, errno, "cannot reopen `%s`", ہدف);
        }
    }

  ماخذ = mount->ماخذ ? mount->ماخذ : type;

  /* Check if there is already a mount for the requested file system.  */
  اگر (! mounted && ماخذ_mountfd >= 0)
    {
      ret = fs_move_mount_مقصد (ماخذ_mountfd, targetfd, تہی);
      اگر (LIKELY (ret == 0))
        {
          غیرعلامتی طویل بقایا_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          اگر (بقایا_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, تہی, ماخذ_mountfd, ہدف, تہی, بقایا_flags, ڈیٹا, LABEL_NONE | no_remount, err);
              اگر (UNLIKELY (ret < 0))
                واپس ret;
            }

          mounted = true;

          اگر (is_خالی_خانے_string (ہدف))
            {
              صحیح new_rootfsfd = open (rootfs, O_راستہ | O_CLOEXEC);
              اگر (UNLIKELY (new_rootfsfd < 0))
                واپس crun_make_error (err, errno, "reopen rootfs after mount on /");

              عارضی_FAILURE_RETRY (close (get_نجی_ڈیٹا (container)->rootfsfd));
              get_نجی_ڈیٹا (container)->rootfsfd = new_rootfsfd;
            }
        }
      ورنہ اگر (get_نجی_ڈیٹا (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          واپس crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", ہدف);
        }
      ورنہ اگر (get_نجی_ڈیٹا (container)->host_جڑ_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          واپس crun_make_error (err, errno, "move_mount `%s` to `%s`", ماخذ, ہدف);
        }
      ورنہ اگر (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           ماخذ, ہدف, strerror (errno));
        }
    }

  اگر (! mounted)
    {
      اگر (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, ماخذ, targetfd, ہدف, flags, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
      ورنہ اگر (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, ماخذ, targetfd, ہدف, flags, ڈیٹا, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
      ورنہ
        {
          صحیح extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, ماخذ, targetfd, ہدف, type, flags, ڈیٹا, extra_flags, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }

  اگر (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, ہدف, copy_from_fd, err);
      copy_from_fd = -1;
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (rec_clear || rec_سیٹ)
    {
      ثابت bool is_dir = S_ISDIR (src_منوال);
      cleanup_close صحیح dfd = -1;

      dfd = محفوظ_openat (get_نجی_ڈیٹا (container)->rootfsfd, rootfs, ہدف, O_RDONLY | O_راستہ | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      اگر (UNLIKELY (dfd < 0))
        واپس crun_error_wrap (err, "open mount target `/%s`", ہدف);

      ret = do_mount_setattr (true, ہدف, dfd, rec_clear, rec_سیٹ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

ساکن صحیح
do_mounts (libcrun_container_t *container, ثابت حرف *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت حرف *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map ساخت libcrun_fd_map *mount_fds = تہی;
  size_t i;
  صحیح ret;

  mount_fds = get_نجی_ڈیٹا (container)->mount_fds;
  get_نجی_ڈیٹا (container)->mount_fds = تہی;

  برائے (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
صحیح
libcrun_container_do_bind_mount (libcrun_container_t *container, حرف *mount_ماخذ, حرف *mount_destination, حرف **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  ثابت حرف *ہدف = consume_slashes (mount_destination);
  cleanup_free حرف *ڈیٹا = تہی;
  غیرعلامتی طویل flags = 0;
  غیرعلامتی طویل extra_flags = 0;
  cleanup_close صحیح targetfd = -1;
  صحیح is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_سیٹ = 0;
  ثابت حرف *rootfs = get_نجی_ڈیٹا (container)->rootfs;
  صحیح ret;

  اگر ((get_نجی_ڈیٹا (container)->rootfsfd < 0) || (rootfs == تہی))
    واپس crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  اگر (mount_options == تہی)
    flags = get_default_flags (container, mount_destination, &ڈیٹا);
  ورنہ
    {
      size_t j;

      برائے (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &ڈیٹا, &rec_clear, &rec_سیٹ);
    }

  اگر (راستہ_is_slash_dev (mount_destination))
    get_نجی_ڈیٹا (container)->mount_dev_from_host = true;

  اگر (mount_ماخذ && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_ماخذ, false, err);
      اگر (UNLIKELY (is_dir < 0))
        واپس is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_محفوظ_create_and_open_ref_at (is_dir, get_نجی_ڈیٹا (container)->rootfsfd, rootfs, ہدف, 0755, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  targetfd = ret;

  ret = do_mount (container, mount_ماخذ, targetfd, ہدف, "bind", flags, ڈیٹا, LABEL_MOUNT, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
صحیح
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, صحیح *notify_socket_برآمد, libcrun_error_t *err)
{
#اگر_تعریف_شدہ HAVE_SYSTEMD
  cleanup_close صحیح notify_fd = -1;
  cleanup_free حرف *host_notify_socket_راستہ = تہی;
  cleanup_free حرف *state_dir = تہی;
  حرف *host_راستہ = تہی;
  صحیح ret;

  اگر (container && get_نجی_ڈیٹا (container)->host_notify_socket_راستہ)
    {
      ثابت حرف *والد_dir;

      والد_dir = get_نجی_ڈیٹا (container)->host_notify_socket_راستہ;

      ret = append_paths (&host_notify_socket_راستہ, err, والد_dir, "notify", تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      host_راستہ = host_notify_socket_راستہ;
    }

  *notify_socket_برآمد = -1;

  اگر (host_راستہ == تہی)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_جڑ, context->id, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = append_paths (&host_notify_socket_راستہ, err, state_dir, "notify/notify", تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      host_راستہ = host_notify_socket_راستہ;
    }

  notify_fd = open_unix_domain_socket (host_راستہ, 1, err);
  اگر (UNLIKELY (notify_fd < 0))
    واپس notify_fd;

  اگر (UNLIKELY (chmod (host_راستہ, 0777) < 0))
    واپس crun_make_error (err, errno, "chmod `%s`", host_راستہ);

#  ifdef HAVE_FGETXATTR
  اگر (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (خالی) setxattr (host_راستہ, "security.selinux", container->container_def->linux->mount_label,
                       لڑی_لمبائی (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_برآمد = get_and_reset (&notify_fd);
  واپس 1;
#ورنہ
  (خالی) context;
  (خالی) container;
  (خالی) err;
  *notify_socket_برآمد = -1;
  واپس 0;
#اگر_ختم
}

#اگر_تعریف_شدہ HAVE_SYSTEMD
ساکن صحیح
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;
  ثابت حرف *notify_socket = container->context->notify_socket;
  cleanup_free حرف *host_notify_socket_راستہ = تہی;
  cleanup_free حرف *container_notify_socket_راستہ = تہی;
  cleanup_free حرف *state_dir = تہی;
  uid_t container_جڑ_uid = -1;
  gid_t container_جڑ_gid = -1;
  صحیح notify_socket_درخت_fd;

  اگر (notify_socket == تہی)
    واپس 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_جڑ : تہی),
                                     (container->context ? container->context->id : تہی), err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_paths (&container_notify_socket_راستہ, err, "/", notify_socket, "notify", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_paths (&host_notify_socket_راستہ, err, state_dir, "notify", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = mkdir (host_notify_socket_راستہ, 0700);
  اگر (ret < 0)
    واپس crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_راستہ);

  اگر (get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_جڑ_اندر_the_userns (container->container_def, 0, 0, &container_جڑ_uid, &container_جڑ_gid);
      اگر (container_جڑ_uid != ((uid_t) -1) && container_جڑ_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_راستہ, container_جڑ_uid, container_جڑ_gid);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_راستہ, container_جڑ_uid, container_جڑ_gid);
        }
    }

  notify_socket_درخت_fd = syscall_open_درخت (AT_FDCWD, host_notify_socket_راستہ, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  اگر (notify_socket_درخت_fd >= 0)
    /* open_tree worked */
    get_نجی_ڈیٹا (container)->notify_socket_درخت_fd = notify_socket_درخت_fd;
  ورنہ اگر (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  ورنہ اگر (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  ورنہ
    /* some other error */
    واپس crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_راستہ);

  get_نجی_ڈیٹا (container)->host_notify_socket_راستہ = host_notify_socket_راستہ;
  get_نجی_ڈیٹا (container)->container_notify_socket_راستہ = container_notify_socket_راستہ;
  host_notify_socket_راستہ = container_notify_socket_راستہ = تہی;
  واپس 0;
}
#اگر_ختم

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
ساکن صحیح
do_finalize_notify_socket (libcrun_container_t *container, ثابت حرف *rootfs, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free حرف *host_notify_socket_راستہ = تہی;
  cleanup_free حرف *container_notify_socket_راستہ = تہی;
  cleanup_free حرف *container_notify_socket_راستہ_dir_alloc = تہی;
  ثابت حرف *container_notify_socket_راستہ_dir = تہی;
  cleanup_close صحیح notify_socket_درخت_fd = -1;
  cleanup_close صحیح targetfd = -1;
  صحیح did_mount_with_move_mount = 0;

  host_notify_socket_راستہ = get_نجی_ڈیٹا (container)->host_notify_socket_راستہ;
  get_نجی_ڈیٹا (container)->host_notify_socket_راستہ = تہی;

  container_notify_socket_راستہ = get_نجی_ڈیٹا (container)->container_notify_socket_راستہ;
  get_نجی_ڈیٹا (container)->container_notify_socket_راستہ = تہی;

  اگر (host_notify_socket_راستہ == تہی || container_notify_socket_راستہ == تہی)
    واپس 0;

  اگر (rootfs == تہی)
    rootfs = "/";

  container_notify_socket_راستہ_dir_alloc = xstrdup (container_notify_socket_راستہ);
  container_notify_socket_راستہ_dir = consume_slashes (dirname (container_notify_socket_راستہ_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  اگر (get_نجی_ڈیٹا (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_راستہ | O_CLOEXEC);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "open `%s`", rootfs);

      get_نجی_ڈیٹا (container)->rootfsfd = ret;
      get_نجی_ڈیٹا (container)->rootfs = rootfs;
    }

  ret = crun_محفوظ_create_and_open_ref_at (true, get_نجی_ڈیٹا (container)->rootfsfd, rootfs,
                                          container_notify_socket_راستہ_dir, 0755, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  targetfd = ret;

  notify_socket_درخت_fd = get_نجی_ڈیٹا (container)->notify_socket_درخت_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_نجی_ڈیٹا (container)->notify_socket_درخت_fd = -1;

  اگر (notify_socket_درخت_fd >= 0)
    {
      ret = fs_move_mount_مقصد (notify_socket_درخت_fd, targetfd, تہی);
      اگر (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_ہدف (container, container_notify_socket_راستہ_dir, err);
          اگر (UNLIKELY (targetfd < 0))
            واپس targetfd;

          ret = do_mount (container, تہی, targetfd, container_notify_socket_راستہ_dir, تہی,
                          MS_REC | MS_PRIVATE, تہی, LABEL_NONE, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
      ورنہ اگر (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      ورنہ
        واپس crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_درخت_fd,
                                container_notify_socket_راستہ_dir);
    }

  اگر (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_راستہ, targetfd, container_notify_socket_راستہ_dir, تہی,
                      MS_BIND | MS_REC | MS_PRIVATE, تہی, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

ساکن صحیح
make_والد_mount_نجی (ثابت حرف *rootfs, libcrun_error_t *err)
{
  cleanup_close صحیح rootfsfd = -1;
  ساخت stat پچھلا_st;
  proc_fd_راستہ_t proc_راستہ;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (rootfsfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", rootfs);

  اگر (UNLIKELY (fstat (rootfsfd, &پچھلا_st) < 0))
    واپس crun_make_error (err, errno, "fstat `%s`", rootfs);

  برائے (;;)
    {
      ساخت stat cur_st;
      libcrun_error_t tmp_err = تہی;
      صحیح ret;
      cleanup_close صحیح parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      اگر (ret == 0)
        واپس 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_راستہ (proc_راستہ, rootfsfd);
      ret = mount (تہی, proc_راستہ, تہی, MS_PRIVATE, تہی);
      اگر (ret == 0)
        واپس 0;

      parentfd = openat (rootfsfd, "..", O_راستہ | O_CLOEXEC);
      اگر (parentfd < 0)
        {
          صحیح saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          اگر (ret != 0)
            واپس crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          واپس crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      اگر (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        واپس crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      اگر (cur_st.st_dev == پچھلا_st.st_dev && cur_st.st_ino == پچھلا_st.st_ino)
        توڑو;

      پچھلا_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  واپس crun_make_error (err, 0, "make `%s` private", rootfs);
}

صحیح
libcrun_سیٹ_mounts (ساخت container_entrypoint_s *entrypoint_دلائل, libcrun_container_t *container, ثابت حرف *rootfs, سیٹ_mounts_cb_t cb, خالی *cb_ڈیٹا, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح is_user_ns = 0;
  صحیح ret = 0;

  اگر (rootfs == تہی || def->mounts == تہی)
    واپس 0;

  ret = open (rootfs, O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "open `%s`", rootfs);

  get_نجی_ڈیٹا (container)->rootfsfd = ret;
  get_نجی_ڈیٹا (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_دلائل, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  اگر (UNLIKELY (ret < 0))
    واپس crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  اگر (def->جڑ->readonly)
    {
      ساخت remount_s *r;
      غیرعلامتی طویل remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      صحیح fd;

      fd = dup (get_نجی_ڈیٹا (container)->rootfsfd);
      اگر (UNLIKELY (fd < 0))
        واپس crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, تہی, get_نجی_ڈیٹا (container)->remounts);
      get_نجی_ڈیٹا (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWUSER);
  اگر (! is_user_ns)
    {
      is_user_ns = check_running_اندر_user_namespace (err);
      اگر (UNLIKELY (is_user_ns < 0))
        واپس is_user_ns;
    }

  اگر (! get_نجی_ڈیٹا (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  اگر (cb)
    {
      ret = cb (cb_ڈیٹا, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = تہی;
      ثابت حرف *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (خالی) crun_محفوظ_ensure_directory_at (get_نجی_ڈیٹا (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

ساکن صحیح
umount_or_hide (ثابت حرف *ہدف, libcrun_error_t *err)
{
  صحیح ret;

  ret = umount2 (ہدف, MNT_DETACH);
  اگر (UNLIKELY (ret < 0))
    {
      صحیح saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      اگر (errno == EINVAL)
        {
          ret = mount (تہی, ہدف, "tmpfs", 0, "size=0k");
          اگر (LIKELY (ret == 0))
            واپس 0;
        }

      واپس crun_make_error (err, saved_errno, "umount `%s`", ہدف);
    }

  واپس ret;
}

ساکن صحیح
move_جڑ (ثابت حرف *rootfs, libcrun_error_t *err)
{
  صحیح ret;

  ret = chdir (rootfs);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = umount_or_hide ("/proc", err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "chdir to `%s`", rootfs);

  واپس 0;
}

صحیح
libcrun_finalize_mounts (ساخت container_entrypoint_s *entrypoint_دلائل, libcrun_container_t *container, ثابت حرف *rootfs,
                         سیٹ_mounts_cb_t after_محور_cb, خالی *cb_ڈیٹا, libcrun_error_t *err)
{
  صحیح ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_دلائل, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  اگر (UNLIKELY (ret < 0))
    واپس crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  close_and_reset (&(get_نجی_ڈیٹا (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_نجی_ڈیٹا (container)->old_جڑ_fd));

  اگر (get_نجی_ڈیٹا (container)->needs_محور)
    {
      get_نجی_ڈیٹا (container)->needs_محور = false;

      اگر (get_نجی_ڈیٹا (container)->no_محور)
        {
          ret = move_جڑ (rootfs, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
      ورنہ
        {
          ret = do_محور (container, rootfs, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      ret = do_mount (container, تہی, -1, "/", تہی,
                      get_نجی_ڈیٹا (container)->rootfs_اشاعت,
                      تہی, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = chdir ("/");
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "chdir to `/`");
    }

  اگر (after_محور_cb)
    {
      ret = after_محور_cb (cb_ڈیٹا, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = finalize_mounts (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

ساکن صحیح
maybe_open_درخت_namespace (ثابت حرف *rootfs, صحیح *برآمد_fd, libcrun_error_t *err)
{
  cleanup_close صحیح rootfs_fd = -1;
  صحیح درخت_fd;

  *برآمد_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (rootfs_fd < 0))
    واپس crun_make_error (err, errno, "open `%s`", rootfs);

  درخت_fd = syscall_open_درخت (rootfs_fd, "",
                               OPEN_درخت_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_خالی_خانے_راستہ | AT_RECURSIVE);
  اگر (درخت_fd < 0)
    {
      اگر (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        واپس 0;
      واپس crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *برآمد_fd = درخت_fd;
  واپس 0;
}

ساکن ساخت libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  ساخت libcrun_fd_map *mount_fds = get_نجی_ڈیٹا (container)->mount_fds;

  اگر (mount_fds == تہی)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_نجی_ڈیٹا (container)->mount_fds = mount_fds;
    }
  واپس mount_fds;
}

ساکن صحیح
open_mount_کا_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free حرف *mnt_ڈیٹا = تہی;
  غیرعلامتی طویل mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_سیٹ = 0;
  ثابت حرف *label_type = تہی;
  ثابت حرف *label_val = تہی;
  ثابت حرف *fstype;
  صحیح mnt_fd;
  size_t j;

  اگر (mount->type == تہی)
    واپس -1;

  fstype = mount->type;

  برائے (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_ڈیٹا,
                                            &mnt_rec_clear, &mnt_rec_سیٹ);

  اگر (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      صحیح extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      اگر (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          اگر (label_type == تہی)
            crun_error_release (err);
          ورنہ
            label_val = def->linux->mount_label;
        }
    }

  اگر (strcmp (fstype, "cgroup") == 0)
    {
      صحیح cgroup_منوال = libcrun_get_cgroup_منوال (err);
      اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->ماخذ, label_type, label_val, mnt_ڈیٹا);
  اگر (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->ماخذ, تہی, تہی, mnt_ڈیٹا);

  واپس mnt_fd;
}

#اگر_غیر_تعریف_شدہ LSMT_ROOT
#  define LSMT_ROOT 0xffffffffffffffffULL
#اگر_ختم
#اگر_غیر_تعریف_شدہ STATMOUNT_FS_TYPE
#  define STATMOUNT_FS_TYPE 0x00000020U
#اگر_ختم
#اگر_غیر_تعریف_شدہ STATMOUNT_MNT_ROOT
#  define STATMOUNT_MNT_ROOT 0x00000040U
#اگر_ختم
#اگر_غیر_تعریف_شدہ STATMOUNT_MNT_POINT
#  define STATMOUNT_MNT_POINT 0x00000080U
#اگر_ختم

/* Declared here so that the build does not depend on the UAPI headers being
   recent enough; the layout is stable.  */
ساخت mnt_id_req_s
{
  uint32_t سائز;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

ساخت statmount_s
{
  uint32_t سائز;
  uint32_t mnt_opts;
  uint64_t نقاب;
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

ساکن صحیح
syscall_listmount (ساخت mnt_id_req_s *req, uint64_t *ids, size_t nr, غیرعلامتی صحیح flags)
{
#اگر_تعریف_شدہ __NR_listmount
  واپس (صحیح) syscall (__NR_listmount, req, ids, nr, flags);
#ورنہ
  (خالی) req;
  (خالی) ids;
  (خالی) nr;
  (خالی) flags;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

ساکن صحیح
syscall_statmount (ساخت mnt_id_req_s *req, ساخت statmount_s *buf, size_t سائز, غیرعلامتی صحیح flags)
{
#اگر_تعریف_شدہ __NR_statmount
  واپس (صحیح) syscall (__NR_statmount, req, buf, سائز, flags);
#ورنہ
  (خالی) req;
  (خالی) buf;
  (خالی) سائز;
  (خالی) flags;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
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
ساکن bool
mounts_کامیابی_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = تہی;
  size_t i, n_ids = 0, استعداد = 128;
  bool has_absolute_bind = false;
  uint64_t آخری = 0;

  برائے (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      اگر (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->ماخذ != تہی && def->mounts[i]->ماخذ[0] == '/')
        {
          has_absolute_bind = true;
          توڑو;
        }
    }
  اگر (! has_absolute_bind)
    واپس false;

  ids = xmalloc (استعداد * حجم (uint64_t));
  برائے (;;)
    {
      ساخت mnt_id_req_s req = {
        .سائز = حجم (req),
        .mnt_id = LSMT_ROOT,
        .param = آخری,
      };
      صحیح ret;

      اگر (n_ids == استعداد)
        {
          استعداد *= 2;
          ids = xrealloc (ids, استعداد * حجم (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, استعداد - n_ids, 0);
      اگر (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        واپس true;
      اگر (ret == 0)
        توڑو;

      n_ids += (size_t) ret;
      آخری = ids[n_ids - 1];
    }

  برائے (i = 0; i < n_ids; i++)
    {
      حرف بفر[4096] __attribute__ ((aligned (8)));
      ساخت statmount_s *sm = (ساخت statmount_s *) بفر;
      ساخت mnt_id_req_s req = {
        .سائز = حجم (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      ثابت حرف *جڑ, *نقطہ;
      size_t j;

      اگر (syscall_statmount (&req, sm, حجم (بفر), 0) < 0)
        جاری;
      اگر ((sm->نقاب & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        جاری;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      جڑ = sm->str + sm->mnt_root;
      اگر (! has_سابقہ (جڑ, "mnt:["))
        جاری;

      نقطہ = sm->str + sm->mnt_point;

      برائے (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          ثابت حرف *ماخذ = def->mounts[j]->ماخذ;
          size_t ماخذ_len;

          اگر (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            جاری;
          اگر (ماخذ == تہی || ماخذ[0] != '/')
            جاری;

          ماخذ_len = لڑی_لمبائی (ماخذ);
          جب (ماخذ_len > 1 && ماخذ[ماخذ_len - 1] == '/')
            ماخذ_len--;

          اگر (لڑی_موازنہ (نقطہ, ماخذ, ماخذ_len) != 0)
            جاری;

          /* The source itself is a pinned mount namespace.  */
          اگر (نقطہ[ماخذ_len] == '\0')
            واپس true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          اگر (recursive && (ماخذ_len == 1 || نقطہ[ماخذ_len] == '/'))
            واپس true;
        }
    }

  واپس false;
}

ساکن bool
can_use_open_درخت_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ساخت libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  اگر (has_hooks || has_userns)
    واپس false;

  اگر (mounts_کامیابی_pinned_mount_namespace (def))
    واپس false;

  اگر (container->context->no_open_درخت_namespace)
    واپس false;

  {
    libcrun_error_t tmp_err = تہی;
    صحیح اندر_userns = check_running_اندر_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    اگر (اندر_userns != 0)
      واپس false;
  }

  mount_fds = get_fd_map (container);
  برائے (i = 0; i < def->mounts_len; i++)
    {
      اگر (mount_fds->fds[i] < 0)
        {
          bool has_cached_ہدف = get_نجی_ڈیٹا (container)->copy_symlink_targets
                                   && i < get_نجی_ڈیٹا (container)->n_copy_symlink_targets
                                   && get_نجی_ڈیٹا (container)->copy_symlink_targets[i];
          اگر (! has_cached_ہدف)
            واپس false;
        }
    }

  واپس true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
ساکن bool
mount_inherits_mountpoint_منوال (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  اگر (mount->type == تہی || strcmp (mount->type, "tmpfs") != 0)
    واپس false;

  برائے (i = 0; i < mount->options_len; i++)
    اگر (mount->options[i] != تہی && has_سابقہ (mount->options[i], "mode="))
      واپس false;

  واپس true;
}

ساکن صحیح
تیاری_mount_namespace (libcrun_container_t *container, bool no_محور, حرف **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  غیرعلامتی طویل rootfs_اشاعت = 0;
  cleanup_close صحیح rootfsfd = -1;
  cleanup_close صحیح درخت_fd = -1;
  libcrun_error_t tmp_err = تہی;
  bool use_open_درخت = false;
  size_t i;
  صحیح ret;

  اگر (def->linux && def->linux->rootfs_اشاعت)
    rootfs_اشاعت = get_mount_flags (def->linux->rootfs_اشاعت, 0, تہی, تہی, تہی, تہی);

  اگر ((rootfs_اشاعت & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_اشاعت = MS_REC | MS_PRIVATE;

  get_نجی_ڈیٹا (container)->rootfs_اشاعت = rootfs_اشاعت;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  آزاد (get_نجی_ڈیٹا (container)->host_rootfs);
  get_نجی_ڈیٹا (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  برائے (i = 0; i < def->mounts_len; i++)
    {
      ساخت libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close صحیح mnt_fd = -1;
      bool recursive_bind, nofollow;
      غیرعلامتی طویل mnt_flags = 0;

      اگر (mount_fds && mount_fds->fds[i] >= 0)
        جاری;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      اگر (def->mounts[i]->ماخذ != تہی
          && def->mounts[i]->ماخذ[0] == '/')
        {
          cleanup_free حرف *mnt_ڈیٹا = تہی;
          غیرعلامتی طویل mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_سیٹ = 0;
          size_t j;

          برائے (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_ڈیٹا,
                                                    &mnt_rec_clear, &mnt_rec_سیٹ);

          اگر (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free حرف *ہدف = تہی;
              libcrun_error_t tmp_err = تہی;
              ssize_t len;

              len = محفوظ_readlinkat (AT_FDCWD, def->mounts[i]->ماخذ, &ہدف, 0, &tmp_err);
              اگر (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              اگر (get_نجی_ڈیٹا (container)->copy_symlink_targets == تہی)
                {
                  get_نجی_ڈیٹا (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * حجم (حرف *));
                  get_نجی_ڈیٹا (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_نجی_ڈیٹا (container)->copy_symlink_targets[i] = ہدف;
              ہدف = تہی;
            }
        }

      اگر (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_ہدف = get_نجی_ڈیٹا (container)->copy_symlink_targets
                                   && i < get_نجی_ڈیٹا (container)->n_copy_symlink_targets
                                   && get_نجی_ڈیٹا (container)->copy_symlink_targets[i];

          اگر (mount_fds && def->mounts[i]->ماخذ != تہی && ! has_cached_ہدف
              && get_راستہ_اندر_rootfs (*rootfs, def->mounts[i]->ماخذ) == تہی)
            {
              libcrun_error_t tmp_err = تہی;
              غیرعلامتی طویل اشاعت = MS_PRIVATE;

              اگر (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                اشاعت = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->ماخذ,
                                                  recursive_bind, false, nofollow,
                                                  اشاعت, &tmp_err);
              اگر (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          جاری;
        }

      mnt_fd = open_mount_کا_type (container, def->mounts[i], err);
      اگر (mnt_fd < 0)
        {
          crun_error_release (err);
          جاری;
        }

      اگر (mount_inherits_mountpoint_منوال (def->mounts[i]))
        {
          cleanup_close صحیح destfd = -1;
          ساخت stat st;

          اگر (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_راستہ | O_CLOEXEC);
              اگر (UNLIKELY (rootfsfd < 0))
                واپس crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = محفوظ_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_راستہ | O_CLOEXEC, 0, &tmp_err);
          اگر (destfd < 0)
            crun_error_release (&tmp_err);
          ورنہ اگر (fstat (destfd, &st) == 0)
            {
              proc_fd_راستہ_t fd_راستہ;
              صحیح procfd = get_procfd (get_نجی_ڈیٹا (container), err);
              اگر (UNLIKELY (procfd < 0))
                واپس procfd;

              get_self_fd_راستہ (fd_راستہ, mnt_fd);
              ret = fchmodat (procfd, fd_راستہ, st.st_mode & 07777, 0);
              (خالی) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_خالی_خانے_dir_cached (container, تہی, &tmp_err);
  اگر (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_نجی_ڈیٹا (container)->maskdir_bind_failed = true;
    }

  اگر (! get_نجی_ڈیٹا (container)->joined_mount_ns
      && can_use_open_درخت_namespace (container))
    use_open_درخت = true;

  اگر (use_open_درخت)
    {
      ret = maybe_open_درخت_namespace (*rootfs, &درخت_fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
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
        واپس crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (تہی, "/", تہی, MS_REMOUNT | MS_BIND, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, تہی, -1, "/", تہی, MS_REC | MS_PRIVATE, تہی, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = do_mount (container, تہی, -1, "/", تہی, rootfs_اشاعت, تہی, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      get_نجی_ڈیٹا (container)->needs_محور = false;
      get_نجی_ڈیٹا (container)->no_محور = no_محور;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_نجی_ڈیٹا (container)->host_جڑ_switched = true;
      آزاد (*rootfs);
      *rootfs = xstrdup ("/");
    }
  ورنہ
    {
      اگر (! get_نجی_ڈیٹا (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, تہی, -1, "/", تہی, rootfs_اشاعت, تہی, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = make_والد_mount_نجی (*rootfs, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, تہی, MS_BIND | MS_REC | MS_PRIVATE, تہی, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      get_نجی_ڈیٹا (container)->needs_محور = true;
      get_نجی_ڈیٹا (container)->no_محور = no_محور;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "open `%s`", *rootfs);
  get_نجی_ڈیٹا (container)->rootfsfd = ret;
  get_نجی_ڈیٹا (container)->rootfs = *rootfs;

  اگر (! container->context->force_no_cgroup)
    {
      صحیح cgroup_منوال = libcrun_get_cgroup_منوال (err);
      اگر (UNLIKELY (cgroup_منوال < 0))
        واپس cgroup_منوال;

      اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
        {
          حرف *unified_cgroup_راستہ = تہی;
          صحیح procfd = get_procfd (get_نجی_ڈیٹا (container), err);
          اگر (UNLIKELY (procfd < 0))
            واپس procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_راستہ, true, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          get_نجی_ڈیٹا (container)->unified_cgroup_راستہ = unified_cgroup_راستہ;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  اگر (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_نجی_ڈیٹا (container)->rootfsfd);
      get_نجی_ڈیٹا (container)->rootfs = تہی;
      واپس ret;
    }

  close_and_reset (&get_نجی_ڈیٹا (container)->rootfsfd);
  get_نجی_ڈیٹا (container)->rootfs = تہی;

  اگر (درخت_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_نجی_ڈیٹا (container), &tmp_err);
  اگر (ret < 0)
    crun_error_release (&tmp_err);

  check_running_اندر_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  واپس 0;
}

صحیح
libcrun_do_محور_جڑ (libcrun_container_t *container, bool no_محور, حرف **rootfs, libcrun_error_t *err)
{
  صحیح ret;

  اگر (*rootfs == تہی)
    واپس 0;

  اگر (get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = تیاری_mount_namespace (container, no_محور, rootfs, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      اگر (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "chdir to `/`");
        }
    }
  ورنہ
    {
      ret = chroot (*rootfs);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "chdir to `/`");

      آزاد (*rootfs);
      *rootfs = xstrdup ("/");
    }

  واپس 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
صحیح
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  ساخت stat statbuf;
  cleanup_close صحیح fd;
  صحیح ret;
  صحیح i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  اگر (UNLIKELY (fd == -1))
    واپس crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  برائے (i = 0; i <= 2; i++)
    {
      اگر (UNLIKELY (fstat (i, &statbuf) == -1))
        واپس crun_make_error (err, errno, "stat fd `%d`", i);
      اگر (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          اگر (UNLIKELY (dup2 (fd, i) == -1))
            واپس crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  واپس 0;
}

ساکن صحیح
uidgidmap_helper (حرف *helper, pid_t pid, ثابت حرف *map_file, libcrun_error_t *err)
{
#تعریف_کرو MAX_دلائل 20
  حرف pid_fmt[16];
  حرف *دلائل[MAX_دلائل + 1];
  حرف *اگلا;
  cleanup_free حرف *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  صحیح ret;

  دلائل[nargs++] = helper;

  ret = snprintf (pid_fmt, حجم (pid_fmt), "%d", pid);
  اگر (UNLIKELY (ret >= (صحیح) حجم (pid_fmt)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");

  دلائل[nargs++] = pid_fmt;
  اگلا = map_file_copy;
  جب (nargs < MAX_دلائل)
    {
      حرف *p = strsep (&اگلا, " \n");
      اگر (اگلا == تہی)
        توڑو;
      دلائل[nargs++] = p;
    }
  دلائل[nargs++] = تہی;

  ret = تسلسل_process (دلائل, err);
  اگر (ret < 0)
    واپس ret;
  اگر (ret > 0)
    واپس crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  واپس 0;
}

ساکن صحیح
newgidmap (pid_t pid, ثابت حرف *map_file, libcrun_error_t *err)
{
  واپس uidgidmap_helper ("newgidmap", pid, map_file, err);
}

ساکن صحیح
newuidmap (pid_t pid, ثابت حرف *map_file, libcrun_error_t *err)
{
  واپس uidgidmap_helper ("newuidmap", pid, map_file, err);
}

ساکن صحیح
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_close صحیح fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  اگر (UNLIKELY (fd < 0))
    واپس fd;

  ret = عارضی_FAILURE_RETRY (اكتب (fd, "deny", 4));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_نجی_ڈیٹا (container)->deny_setgroups = true;
  واپس 0;
}

ساکن صحیح
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free حرف *content = تہی;

  اگر (get_نجی_ڈیٹا (container)->deny_setgroups)
    واپس 0;

  اگر (container->container_def->annotations)
    {
      ثابت حرف *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = ڈھونڈو_annotation (container, "run.oci.keep_original_groups");
      اگر (annotation)
        واپس strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close صحیح fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    اگر (fd < 0)
      ret = fd;
    ورنہ
      ret = read_all_fd (fd, "self/setgroups", &content, تہی, err);
  }
  اگر (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          واپس 1;
        }
      واپس ret;
    }

  واپس لڑی_موازنہ (content, "deny", 4) == 0 ? 0 : 1;
}

صحیح
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = تہی;
  size_t additional_gids_len = 0;
  صحیح can_do_setgroups;
  صحیح ret;

  اگر (process != تہی && process->user != تہی)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  اگر (UNLIKELY (can_do_setgroups < 0))
    واپس can_do_setgroups;

  اگر (can_do_setgroups == 0)
    {
      ثابت حرف *annotation;
      size_t i;

      annotation = ڈھونڈو_annotation (container, "run.oci.keep_original_groups");
      اگر (annotation && strcmp (annotation, "0") != 0)
        واپس 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      برائے (i = 0; i < additional_gids_len; i++)
        اگر (additional_gids[i] != process->user->gid)
          واپس crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (صحیح) additional_gids[i]);
      واپس 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "setgroups");

  واپس 0;
}

صحیح
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  اگر (get_نجی_ڈیٹا (container)->unshare_cgroupns)
    {
      صحیح ret = unshare (CLONE_NEWCGROUP);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (errno != EINVAL)
            واپس crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  واپس 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
صحیح
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  اگر (len != 1)
    واپس 0;

  اگر (mappings[0]->سائز != 1)
    واپس 0;

  اگر (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    واپس 0;

  واپس 1;
}

ساکن صحیح
write_id_map (libcrun_container_t *container, pid_t pid, ثابت حرف *map_name,
              صحیح (*idmap_helper) (pid_t, ثابت حرف *, libcrun_error_t *), ثابت حرف *helper_name,
              bool warn_on_failure, ثابت حرف *map, size_t map_len,
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
        واپس fd;

      ret = محفوظ_write (fd, map_name, map, map_len, err);
      اگر (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free حرف *single_mapping = تہی;
          crun_error_release (err);

          اگر (! skip_if_setgroups_denied || ! get_نجی_ڈیٹا (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              اگر (UNLIKELY (ret < 0))
                واپس ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          اگر (UNLIKELY (fd < 0))
            واپس fd;

          ret = محفوظ_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  واپس ret;
}

صحیح
libcrun_سیٹ_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free حرف *uid_map = تہی;
  cleanup_free حرف *gid_map = تہی;
  size_t uid_map_len = 0, gid_map_len = 0;
  صحیح ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  اگر ((get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWUSER) == 0)
    واپس 0;

  اگر (! def || ! def->linux)
    واپس 0;

  اگر (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      uid_map_len = (size_t) ret;
      اگر (uid_map == تہی)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }

  اگر (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      gid_map_len = (size_t) ret;
      اگر (gid_map == تہی)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

#تعریف_کرو CAP_مقصد_نقاب_0(x) (1L << ((x) & 31))
#تعریف_کرو CAP_مقصد_نقاب_1(x) CAP_مقصد_نقاب_0 (x - 32)

ساخت all_caps_s
{
  غیرعلامتی طویل effective[2];
  غیرعلامتی طویل permitted[2];
  غیرعلامتی طویل inheritable[2];
  غیرعلامتی طویل ambient[2];
  غیرعلامتی طویل bounding[2];
};

ساکن صحیح
has_cap_on (صحیح cap, طویل غیرعلامتی *caps)
{
  اگر (cap < 32)
    واپس CAP_مقصد_نقاب_0 (cap) & caps[0];
  واپس (CAP_مقصد_نقاب_1 (cap) & caps[1]);
}

ساکن غیرعلامتی طویل cap_آخری_cap;

صحیح
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  صحیح ret;
  حرف بفر[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  اگر (fd < 0)
    واپس fd;
  ret = عارضی_FAILURE_RETRY (اقرأ (fd, بفر, حجم (بفر) - 1));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  بفر[ret] = '\0';

  errno = 0;
  cap_آخری_cap = strtoul (بفر, تہی, 10);
  اگر (errno != 0)
    واپس crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", بفر);
  واپس 0;
}

ساکن صحیح
سیٹ_required_caps (ساخت all_caps_s *caps, uid_t uid, gid_t gid, صحیح no_new_privs, bool no_exec, libcrun_error_t *err)
{
#اگر_تعریف_شدہ HAVE_CAP
  غیرعلامتی طویل cap;
  صحیح ret;
  ساخت __user_cap_header_struct hdr = { LINUX_CAPABILITY_نسخہ_3, 0 };
  ساخت __user_cap_data_struct ڈیٹا[2] = { { 0 } };

  اگر (cap_آخری_cap == 0)
    واپس crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  برائے (cap = 0; cap <= cap_آخری_cap; cap++)
    اگر (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        اگر (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          واپس crun_make_error (err, errno, "prctl drop bounding");
      }

  ڈیٹا[0].effective = caps->effective[0];
  ڈیٹا[1].effective = caps->effective[1];
  ڈیٹا[0].inheritable = caps->inheritable[0];
  ڈیٹا[1].inheritable = caps->inheritable[1];
  ڈیٹا[0].permitted = caps->permitted[0];
  ڈیٹا[1].permitted = caps->permitted[1];

  ret = prctl (PR_سیٹ_KEEPCAPS, 1, 0, 0, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  اگر (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    واپس crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, ڈیٹا);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  اگر (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    واپس crun_make_error (err, errno, "prctl reset ambient");

  برائے (cap = 0; cap <= cap_آخری_cap; cap++)
    اگر (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        اگر (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          واپس crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  اگر (no_exec)
    {
      ret = prctl (PR_سیٹ_KEEPCAPS, 0, 0, 0, 0);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "prctl reset KEEPCAPS");

      اگر (uid != 0)
        {
          ڈیٹا[0].permitted = ڈیٹا[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          برائے (cap = 0; cap <= cap_آخری_cap; cap++)
            اگر (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_سیٹ, cap, 0, 0) > 0)
              ڈیٹا[cap / 32].permitted |= CAP_مقصد_نقاب_0 (cap);
#  endif
          ڈیٹا[0].effective = ڈیٹا[0].permitted;
          ڈیٹا[1].effective = ڈیٹا[1].permitted;

          ret = capset (&hdr, ڈیٹا);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "capset");
        }
    }
#اگر_ختم

  اگر (no_new_privs)
    اگر (UNLIKELY (prctl (PR_سیٹ_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      واپس crun_make_error (err, errno, "prctl set no new privs");

  واپس 0;
}

ساکن صحیح
read_caps (غیرعلامتی طویل caps[2], حرف **اقدار, size_t len, libcrun_error_t *err)
{
#اگر_تعریف_شدہ HAVE_CAP
  size_t i;
  برائے (i = 0; i < len; i++)
    {
      cap_قدر_t cap;
      اگر (cap_from_name (اقدار[i], &cap) < 0)
        واپس crun_make_error (err, 0, "unknown capability `%s`", اقدار[i]);

      اگر (cap < 32)
        caps[0] |= CAP_مقصد_نقاب_0 (cap);
      ورنہ
        caps[1] |= CAP_مقصد_نقاب_1 (cap);
    }
#ورنہ
  caps[0] = 0;
  caps[1] = 0;
#اگر_ختم
  واپس 0;
}

صحیح
libcrun_سیٹ_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  اگر (proc->selinux_label)
    واپس سیٹ_selinux_label (container, proc->selinux_label, now, err);

  واپس 0;
}

صحیح
libcrun_سیٹ_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  اگر (proc->apparmor_profile)
    واپس سیٹ_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  واپس 0;
}

صحیح
libcrun_سیٹ_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  صحیح no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  ساخت all_caps_s caps = {};
  صحیح ret;

  اگر (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس سیٹ_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

ساخت rlimit_s
{
  ثابت حرف *name;
  صحیح قدر;
};

ساخت rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
                              { "RLIMIT_CORE", RLIMIT_CORE },
                              { "RLIMIT_CPU", RLIMIT_CPU },
                              { "RLIMIT_DATA", RLIMIT_ڈیٹا },
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
                              { "RLIMIT_STACK", RLIMIT_ڈھیر },
                              { تہی, 0 } };

ساکن صحیح
get_rlimit_resource (ثابت حرف *name)
{
  ساخت rlimit_s *it;
  برائے (it = rlimits; it->name; it++)
    اگر (strcmp (it->name, name) == 0)
      واپس it->قدر;
  واپس -1;
}

صحیح
libcrun_سیٹ_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  برائے (i = 0; i < len; i++)
    {
      برائے (j = i + 1; j < len; j++)
        {
          اگر (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            واپس crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  برائے (i = 0; i < len; i++)
    {
      ساخت rlimit حد_بندی;
      حرف *type = new_rlimits[i]->type;
      صحیح resource = get_rlimit_resource (type);
      اگر (UNLIKELY (resource < 0))
        واپس crun_make_error (err, 0, "invalid rlimit `%s`", type);
      حد_بندی.rlim_cur = new_rlimits[i]->soft;
      حد_بندی.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (غیرعلامتی طویل طویل) حد_بندی.rlim_cur,
                     (غیرعلامتی طویل طویل) حد_بندی.rlim_max);
      اگر (UNLIKELY (setrlimit (resource, &حد_بندی) < 0))
        واپس crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  واپس 0;
}

صحیح
libcrun_سیٹ_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح has_uts = get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWUTS;
  صحیح ret;
  اگر (def->hostname == تہی || def->hostname[0] == '\0')
    واپس 0;
  اگر (! has_uts)
    واپس crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, لڑی_لمبائی (def->hostname));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "sethostname");
  واپس 0;
}

صحیح
libcrun_سیٹ_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح has_uts = get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWUTS;
  صحیح ret;
  اگر (is_خالی_خانے_string (def->domainname))
    واپس 0;
  اگر (! has_uts)
    واپس crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, لڑی_لمبائی (def->domainname));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "setdomainname");
  واپس 0;
}

صحیح
libcrun_سیٹ_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحیح fd = -1;
  صحیح ret;
  حرف oom_بفر[16];
  اگر (def->process == تہی || ! def->process->oom_score_adj_present)
    واپس 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_بفر, حجم (oom_بفر), "%i", def->process->oom_score_adj);
  اگر (UNLIKELY (ret >= (صحیح) حجم (oom_بفر)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  اگر (fd < 0)
    واپس fd;
  ret = عارضی_FAILURE_RETRY (اكتب (fd, oom_بفر, لڑی_لمبائی (oom_بفر)));
  اگر (ret < 0)
    واپس crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  واپس 0;
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
  تہی
};

ساکن صحیح
validate_sysctl (ثابت حرف *original_کلید, ثابت حرف *original_قدر, ثابت حرف *name, غیرعلامتی طویل namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ثابت حرف *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  اگر (name[0] == '/' || راستہ_has_dot_dot_component (name))
    واپس crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_کلید);

  اگر (has_سابقہ (name, "fs/mqueue/"))
    {
      اگر (namespaces_created & CLONE_NEWIPC)
        واپس 0;

      namespace = "IPC";
      جاؤ fail;
    }

  اگر (has_سابقہ (name, "kernel/"))
    {
      size_t i;

      برائے (i = 0; sysctlRequiringIPC[i]; i++)
        اگر (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            اگر (namespaces_created & CLONE_NEWIPC)
              واپس 0;

            namespace = "IPC";
            جاؤ fail;
          }

      اگر (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          اگر (! is_خالی_خانے_string (def->domainname) && (strcmp (original_قدر, def->domainname) != 0))
            واپس crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_کلید);

          اگر (namespaces_created & CLONE_NEWUTS)
            واپس 0;

          namespace = "UTS";
          جاؤ fail;
        }

      اگر (strcmp (name, "kernel/hostname") == 0)
        واپس crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_کلید);
    }
  اگر (has_سابقہ (name, "net/"))
    {
      اگر (namespaces_created & CLONE_NEWNET)
        واپس 0;

      namespace = "network";
      جاؤ fail;
    }

  واپس crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_کلید);

fail:
  واپس crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_کلید, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
ساکن حرف *
sysctl_error_reason (ثابت حرف *name, صحیح namespaces_created, صحیح errno_)
{

  اگر (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    واپس xstrdup ("are all the IDs mapped in the user namespace?");

  واپس تہی;
}

صحیح
libcrun_سیٹ_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close صحیح dirfd = -1;
  غیرعلامتی طویل namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  اگر (def->linux == تہی || def->linux->sysctl == تہی || def->linux->sysctl->len == 0)
    واپس 0;

  برائے (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحیح قدر;

      قدر = libcrun_ڈھونڈو_namespace (def->linux->namespaces[i]->type);
      اگر (UNLIKELY (قدر < 0))
        واپس crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= قدر;
    }

  get_نجی_ڈیٹا (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_راستہ, err);
  اگر (UNLIKELY (dirfd < 0))
    واپس dirfd;

  برائے (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free حرف *name = تہی;
      cleanup_close صحیح fd = -1;
      صحیح ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      اگر (def->linux->sysctl->کلیدیں[i] == تہی || def->linux->sysctl->اقدار[i] == تہی)
        واپس crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_کلید_مقصد_proc_راستہ (def->linux->sysctl->کلیدیں[i]);

      ret = validate_sysctl (def->linux->sysctl->کلیدیں[i], def->linux->sysctl->اقدار[i], name, namespaces_created, def, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      اگر (UNLIKELY (fd < 0))
        واپس crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = عارضی_FAILURE_RETRY (اكتب (fd, def->linux->sysctl->اقدار[i], لڑی_لمبائی (def->linux->sysctl->اقدار[i])));
      اگر (UNLIKELY (ret < 0))
        {
          صحیح saved_errno = errno;
          cleanup_free حرف *reason = تہی;

          reason = sysctl_error_reason (def->linux->sysctl->کلیدیں[i], namespaces_created, saved_errno);
          واپس crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  واپس 0;
}

ساکن صحیح
open_terminal (حرف **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  uid_t uid = 0;
  صحیح ret;

  اگر (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  اگر (UNLIKELY (fd < 0))
    واپس fd;

  ret = libcrun_سیٹ_stdio (fd, *pty, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  واپس ret;
}

حرف *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  واپس get_نجی_ڈیٹا (container)->external_descriptors;
}

صحیح
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  ثابت حرف *buf = تہی;
  json_gen_ctx *gen = تہی;
  size_t buf_len;
  صحیح ret;
  صحیح i;

  اگر (! json_gen_init (&gen, تہی))
    واپس crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  اگر (UNLIKELY (ret != json_gen_حالت_ok))
    جاؤ json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  برائے (i = 0; i < 3; i++)
    {
      proc_fd_راستہ_t fd_راستہ;
      حرف link_راستہ[PATH_MAX];

      get_proc_self_fd_راستہ (fd_راستہ, i);

      ret = readlink (fd_راستہ, link_راستہ, PATH_MAX - 1);
      اگر (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          اگر (errno == ENOENT)
            {
              strcpy (link_راستہ, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          ورنہ
            {
              json_gen_free (gen);
              واپس crun_make_error (err, errno, "readlink `%s`", fd_راستہ);
            }
        }
      link_راستہ[ret] = 0;

      ret = json_gen_string (gen, link_راستہ, ret);
      اگر (UNLIKELY (ret != json_gen_حالت_ok))
        جاؤ json_error;
    }

  ret = json_gen_array_close (gen);
  اگر (UNLIKELY (ret != json_gen_حالت_ok))
    جاؤ json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  اگر (UNLIKELY (ret != json_gen_حالت_ok))
    جاؤ json_error;

  اگر (buf)
    {
      حرف *b = xmalloc (buf_len + 1);
      حافظہ_نقل (b, buf, buf_len);
      b[buf_len] = '\0';
      get_نجی_ڈیٹا (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  واپس 0;

json_error:
  اگر (gen)
    json_gen_free (gen);
  واپس json_gen_error_مقصد_crun_error (ret, err);
}

صحیح
libcrun_سیٹ_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free حرف *pty = تہی;
  cleanup_close صحیح fd = -1;
  صحیح ret;

  اگر (def->process == تہی || ! def->process->terminal)
    واپس 0;

  fd = open_terminal (&pty, def->process, err);
  اگر (UNLIKELY (fd < 0))
    واپس fd;

  اگر (def->process->console_سائز)
    {
      ret = libcrun_terminal_تیاری_سائز (0, def->process->console_سائز->اونچائی, def->process->console_سائز->چوڑائی, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (get_نجی_ڈیٹا (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", تہی, MS_BIND, تہی, LABEL_MOUNT, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      ret = unlink ("/dev/console");
      اگر (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          اگر (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", تہی, MS_BIND, تہی, LABEL_MOUNT, err);
              اگر (UNLIKELY (ret < 0))
                واپس ret;

              واپس get_and_reset (&fd);
            }
          واپس crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  واپس get_and_reset (&fd);
}

ساکن bool
read_error_from_sync_socket (صحیح sync_socket_fd, صحیح *error, حرف **str)
{
  cleanup_free حرف *b = تہی;
  size_t سائز;
  صحیح رمز = 0;
  صحیح ret;

  اگر (*error == 0)
    {
      ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_fd, &رمز, حجم (رمز)));
      اگر (UNLIKELY (ret != (صحیح) حجم (رمز)))
        واپس false;
      *error = رمز;
    }

  ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_fd, &سائز, حجم (سائز)));
  اگر (UNLIKELY (ret != (صحیح) حجم (سائز)))
    واپس false;

  اگر (سائز == 0)
    واپس false;

  اگر (سائز > 1024)
    سائز = 1024;

  b = xmalloc (سائز + 1);
  ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_fd, b, سائز));
  اگر (UNLIKELY (ret < 0))
    واپس false;

  b[ret] = '\0';

  *str = b;
  b = تہی;
  واپس true;
}

ساکن bool
send_error_مقصد_sync_socket (صحیح sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t سائز;
  صحیح ret;
  صحیح رمز;
  حرف *پیغام;

  اگر (err == تہی || *err == تہی)
    واپس false;

  رمز = crun_error_get_errno (err);

  اگر (has_fd)
    {
      /* dummy terminal fd.  */
      ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_fd, "1", 1));
      اگر (UNLIKELY (ret < 0))
        واپس false;
    }

  ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_fd, &رمز, حجم (رمز)));
  اگر (UNLIKELY (ret < 0))
    واپس false;

  پیغام = (*err)->پیغام;
  سائز = لڑی_لمبائی (پیغام) + 1;
  ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_fd, &سائز, حجم (سائز)));
  اگر (UNLIKELY (ret < 0))
    واپس false;

  ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_fd, پیغام, سائز));
  اگر (UNLIKELY (ret < 0))
    واپس false;

  واپس true;
}

ساکن اندرونی صحیح
send_success_مقصد_sync_socket (صحیح sync_socket, libcrun_error_t *err)
{
  ثابت صحیح success = 0;
  صحیح ret;

  ret = عارضی_FAILURE_RETRY (اكتب (sync_socket, &success, حجم (success)));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "write to sync socket");

  واپس 0;
}

ساکن __attribute__ ((noreturn)) خالی
send_error_مقصد_sync_socket_and_die (صحیح sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  حرف *پیغام;

  اگر (err == تہی || *err == تہی)
    محفوظ_exit (EXIT_FAILURE);

  اگر (! send_error_مقصد_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      پیغام = (*err)->پیغام;
      libcrun_fail_with_error (errno, "%s", پیغام);
    }
  محفوظ_exit (EXIT_FAILURE);
}

ساکن صحیح
expect_success_from_sync_socket (صحیح sync_fd, libcrun_error_t *err)
{
  cleanup_free حرف *err_str = تہی;
  صحیح res = 1;
  صحیح ret;

  ret = عارضی_FAILURE_RETRY (اقرأ (sync_fd, &res, حجم (res)));
  اگر (UNLIKELY (ret != حجم (res)))
    واپس crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  اگر (res == 0)
    واپس 0;

  اگر (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      اگر (! is_خالی_خانے_string (err_str))
        واپس crun_make_error (err, res, "%s", err_str);
    }

  واپس crun_make_error (err, res, "read from sync socket");
}

ساکن صحیح
join_namespaces (runtime_spec_schema_config_schema *def, صحیح *namespaces_مقصد_join, صحیح n_namespaces_مقصد_join,
                 صحیح *namespaces_مقصد_join_اشاریہ, bool ignore_join_errors, libcrun_error_t *err)
{
  صحیح ret;
  صحیح i;

  برائے (i = 0; i < n_namespaces_مقصد_join; i++)
    {
      cleanup_free حرف *cwd = تہی;
      صحیح orig_اشاریہ = namespaces_مقصد_join_اشاریہ[i];
      صحیح قدر;

      اگر (namespaces_مقصد_join[i] < 0)
        جاری;

      /* Skip the user namespace.  */
      قدر = libcrun_ڈھونڈو_namespace (def->linux->namespaces[orig_اشاریہ]->type);
      اگر (قدر == CLONE_NEWUSER)
        جاری;

      اگر (قدر == CLONE_NEWNS)
        {
          cwd = getcwd (تہی, 0);
          اگر (UNLIKELY (cwd == تہی))
            واپس crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_اشاریہ]->type, def->linux->namespaces[orig_اشاریہ]->راستہ);
      ret = setns (namespaces_مقصد_join[i], قدر);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (ignore_join_errors)
            جاری;
          واپس crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_اشاریہ]->راستہ);
        }

      close_and_reset (&namespaces_مقصد_join[i]);

      اگر (قدر == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  واپس 0;
}

#تعریف_کرو MAX_NAMESPACES 10

ساخت init_حالت_s
{
  /* fd to the namespace to join.  */
  صحیح fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  صحیح اشاریہ[MAX_NAMESPACES];
  /* CLONE_* value.  */
  صحیح قدر[MAX_NAMESPACES];
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
  صحیح userns_اشاریہ;
  /* def->linux->namespaces userns.  */
  صحیح userns_اشاریہ_origin;

  /* All namespaces created/joined by the container.  */
  صحیح all_namespaces;

  /* What namespaces are still missing to be created.  */
  صحیح namespaces_مقصد_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  صحیح idx_pidns_مقصد_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  صحیح idx_timens_مقصد_join_immediately;
};

خالی
cleanup_free_init_statusp (ساخت init_حالت_s *ns)
{
  size_t i;

  اگر (ns == تہی)
    واپس;

  برائے (i = 0; i < ns->fd_len; i++)
    عارضی_FAILURE_RETRY (close (ns->fd[i]));
}

ساکن صحیح
configure_init_حالت (ساخت init_حالت_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  برائے (i = 0; i < MAX_NAMESPACES + 1; i++)
    ns->fd[i] = -1;

  ns->fd_len = 0;
  ns->all_namespaces = 0;
  ns->namespaces_مقصد_unshare = 0;
  ns->join_pidns = false;
  ns->join_ipcns = false;
  ns->must_fork = false;
  ns->can_delay_pidns_fork = false;
  ns->delayed_userns_create = false;
  ns->userns_اشاریہ = -1;
  ns->userns_اشاریہ_origin = -1;
  ns->idx_pidns_مقصد_join_immediately = -1;
  ns->idx_timens_مقصد_join_immediately = -1;

  برائے (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحیح قدر = libcrun_ڈھونڈو_namespace (def->linux->namespaces[i]->type);
      اگر (UNLIKELY (قدر < 0))
        واپس crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      اگر (ns->all_namespaces & قدر)
        واپس crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= قدر;

      اگر (def->linux->namespaces[i]->راستہ == تہی)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_مقصد_unshare |= قدر;
        }
      ورنہ
        {
          صحیح fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->راستہ);

          اگر (ns->fd_len >= MAX_NAMESPACES)
            واپس crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->راستہ, O_RDONLY | O_CLOEXEC);
          اگر (UNLIKELY (fd < 0))
            واپس crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->راستہ);

          اگر (قدر == CLONE_NEWUSER)
            {
              ns->userns_اشاریہ = ns->fd_len;
              ns->userns_اشاریہ_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->اشاریہ[ns->fd_len] = i;
          ns->قدر[ns->fd_len] = قدر;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  اگر (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_مقصد_unshare |= CLONE_NEWUSER;
    }

  واپس 0;
}

/* Detect if root is available in the container.  */
ساکن bool
جڑ_mapped_اندر_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  برائے (i = 0; i < len; i++)
    اگر (mappings[i]->container_id == 0)
      واپس true;

  واپس false;
}

ساکن ساخت libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  ساخت libcrun_fd_map *dev_fds = get_نجی_ڈیٹا (container)->dev_fds;

  اگر (dev_fds == تہی)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_نجی_ڈیٹا (container)->dev_fds = dev_fds;
    }
  واپس dev_fds;
}

#تعریف_کرو عدد_NEEDED_DEVS (حجم (needed_devs) / حجم (needed_devs[0]) - 1)

ساکن ساخت libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  ساخت libcrun_fd_map *fds = get_نجی_ڈیٹا (container)->needed_devs_fds;

  اگر (fds == تہی)
    {
      fds = make_libcrun_fd_map (عدد_NEEDED_DEVS);
      get_نجی_ڈیٹا (container)->needed_devs_fds = fds;
    }
  واپس fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  اگر (recursive)
    *recursive = false;

  اگر (src_nofollow != تہی)
    *src_nofollow = false;

  برائے (i = 0; i < mnt->options_len; i++)
    {
      اگر (mnt->options[i] == تہی)
        جاری;

      اگر (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          اگر (src_nofollow == تہی || *src_nofollow)
            توڑو;
        }
      اگر (strcmp (mnt->options[i], "rbind") == 0)
        {
          اگر (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          اگر (src_nofollow == تہی || *src_nofollow)
            توڑو;
        }
      اگر (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  واپس ret;
}

ساکن حرف *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  برائے (i = 0; i < mnt->options_len; i++)
    {
      اگر (mnt->options[i] == تہی)
        جاری;

      اگر (has_سابقہ (mnt->options[i], "idmap"))
        {
          *recursive = false;
          واپس mnt->options[i];
        }
      اگر (has_سابقہ (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          واپس mnt->options[i];
        }
    }
  واپس تہی;
}

ساکن صحیح
open_mount_type (ثابت حرف *type, صحیح *برآمد_fd, libcrun_error_t *err)
{
  cleanup_close صحیح fsopen_fd = -1;
  cleanup_close صحیح newfs_fd = -1;
  صحیح ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  اگر (UNLIKELY (fsopen_fd < 0))
    واپس crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, تہی, تہی, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  اگر (UNLIKELY (newfs_fd < 0))
    واپس crun_make_error (err, errno, "fsmount `%s`", type);

  *برآمد_fd = get_and_reset (&newfs_fd);
  واپس 0;
}

ساکن صحیح
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, صحیح *برآمد_fd, bool *has_mappings_برآمد, libcrun_error_t *err)
{
  cleanup_close صحیح newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  ساخت mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close صحیح fd = -1;
  ثابت حرف *idmap_option;
  bool recursive = false;
  ثابت حرف *options = تہی;
  bool has_mappings;
  صحیح ret;
  حرف *extra_پیغام = "";
  bool nofollow = false;

  *برآمد_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != تہی);
  اگر (has_mappings_برآمد)
    *has_mappings_برآمد = has_mappings;
  اگر (! has_mappings)
    واپس 0;

  اگر ((mnt->uid_mappings == تہی) != (mnt->gid_mappings == تہی))
    واپس crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  اگر (idmap_option)
    {
      options = strchr (idmap_option, '=');
      اگر (options)
        {
          /* Skip the '=' itself.  */
          options++;
          اگر (options[0] == '\0')
            options = تہی;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  اگر (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  اگر (UNLIKELY (fd < 0))
    واپس fd;

  اگر (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_درخت (AT_FDCWD, mnt->ماخذ, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      اگر (UNLIKELY (newfs_fd < 0))
        واپس crun_make_error (err, errno, "open `%s`", mnt->ماخذ);
    }
  ورنہ
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_خالی_خانے_راستہ | (recursive ? AT_RECURSIVE : 0), &attr);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == EINVAL)
        {
          extra_پیغام = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      واپس crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_پیغام);
    }

  *برآمد_fd = get_and_reset (&newfs_fd);
  واپس 0;
}

ساکن uid_t
get_id_اندر_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  اگر (! def || ! def->linux)
    جاؤ باہر_نکلو;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  اگر (! mappings)
    جاؤ باہر_نکلو;

  برائے (i = 0; i < len; i++)
    {
      اگر (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->سائز)
        واپس id - mappings[i]->container_id + mappings[i]->host_id;
    }
باہر_نکلو:
  واپس is_uid ? get_حد_سے_تجاوز_uid () : get_حد_سے_تجاوز_gid ();
}

ساکن صحیح
precreate_device (libcrun_container_t *container, صحیح devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  حرف name[64];
  mode_t type;
  dev_t dev;
  صحیح ret;

  ret = snprintf (name, حجم (name), "%zu", i);
  اگر (UNLIKELY (ret >= (صحیح) حجم (name)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_منوال | type, dev);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "mknod `%s`", device->راستہ);

  uid = get_id_اندر_user_namespace (device->uid, true, def);
  gid = get_id_اندر_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "chown `%s`", device->راستہ);

  واپس get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

ساکن صحیح
send_mounts (صحیح sync_socket_host, ساخت libcrun_fd_map *fds, size_t how_many, size_t کل, libcrun_error_t *err)
{
  size_t i;
  صحیح ret;

  ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_host, &how_many, حجم (how_many)));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "write to sync socket");

  برائے (i = 0; i < کل; i++)
    {
      اگر (fds->fds[i] >= 0)
        {
          ret = send_fd_مقصد_socket_with_payload (sync_socket_host, fds->fds[i], (حرف *) &i, حجم (i), err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }
  واپس 0;
}

ساکن صحیح
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, صحیح sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ساخت libcrun_fd_map *mount_fds = تہی;
  bool has_userns = (get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free حرف *rootfs = تہی;
  size_t how_many = 0;
  size_t i;
  صحیح ret;

  اگر (def->mounts_len == 0)
    واپس 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  اگر (def->جڑ && def->جڑ->راستہ)
    rootfs = realpath (def->جڑ->راستہ, تہی);

  اگر (! has_userns)
    {
      صحیح is_اندر_userns = check_running_اندر_user_namespace (err);
      اگر (UNLIKELY (is_اندر_userns < 0))
        واپس is_اندر_userns;

      has_userns = is_اندر_userns > 0;
    }

  برائے (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      صحیح mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      اگر (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_راستہ_اندر_rootfs (rootfs, def->mounts[i]->ماخذ) == تہی)
        {
          غیرعلامتی طویل اشاعت = 0;

          اگر (def->linux && def->linux->rootfs_اشاعت)
            اشاعت = get_mount_flags (def->linux->rootfs_اشاعت, 0, تہی, تہی, تہی, تہی) & ALL_PROPAGATIONS_NO_REC;
          اگر (اشاعت == 0)
            اشاعت = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->ماخذ, recursive, false, nofollow, اشاعت, err);
          اگر (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      اگر (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  واپس send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

ساکن صحیح
prepare_and_send_dev_mounts (libcrun_container_t *container, صحیح sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ساخت libcrun_fd_map *dev_fds = تہی;
  bool has_userns = (get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close صحیح current_mountns = -1;
  cleanup_free حرف *state_dir = تہی;
  cleanup_free حرف *devs_راستہ = تہی;
  cleanup_close صحیح devs_mountfd = -1;
  cleanup_close صحیح targetfd = -1;
  ثابت حرف *context_type = تہی;
  ثابت حرف *label = تہی;
  size_t how_many = 0;
  size_t i;
  صحیح ret;
  // To track whether the namespace has been changed.
  bool ns_بدلا = false;

  اگر (def->linux == تہی || def->linux->devices_len == 0)
    واپس 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  اگر (! has_userns || is_خالی_خانے_string (container->context->id) || geteuid () > 0)
    واپس send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_جڑ : تہی),
                                     (container->context ? container->context->id : تہی), err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_paths (&devs_راستہ, err, state_dir, "devs", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = mkdir (devs_راستہ, 0700);
  اگر (UNLIKELY (ret < 0 && errno != EEXIST))
    واپس crun_make_error (err, errno, "mkdir `%s`", devs_راستہ);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  اگر (UNLIKELY (current_mountns < 0))
    واپس current_mountns;

  ret = unshare (CLONE_NEWNS);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_بدلا = true;

  ret = mount (تہی, "/", تہی, MS_REC | MS_PRIVATE, تہی);
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      جاؤ restore_mountns;
    }

  اگر (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      اگر (UNLIKELY (context_type == تہی))
        {
          ret = -1;
          جاؤ restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", تہی, context_type, label, تہی);
  اگر (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      جاؤ restore_mountns;
    }

  targetfd = open (devs_راستہ, O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_راستہ);
      جاؤ restore_mountns;
    }

  ret = fs_move_mount_مقصد (devs_mountfd, targetfd, تہی);
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_راستہ);
      جاؤ restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_راستہ);
      جاؤ restore_mountns;
    }

  برائے (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      اگر (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          جاری;
        }

      dev_fds->fds[i] = ret;

      اگر (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  اگر (ns_بدلا && current_mountns >= 0)
    {
      صحیح setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      اگر (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          واپس crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  واپس ret;
}

ساکن صحیح
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, صحیح sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map ساخت libcrun_fd_map *fds = تہی;
  bool has_userns = (get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (عدد_NEEDED_DEVS);

  اگر (! has_userns || geteuid () > 0)
    واپس send_mounts (sync_socket_host, fds, 0, عدد_NEEDED_DEVS, err);

  برائے (i = 0; i < عدد_NEEDED_DEVS && needed_devs[i].راستہ; i++)
    {
      صحیح fd = syscall_open_درخت (AT_FDCWD, needed_devs[i].راستہ,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      اگر (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  واپس send_mounts (sync_socket_host, fds, how_many, عدد_NEEDED_DEVS, err);
}

ساکن صحیح
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, صحیح sync_socket_host, libcrun_error_t *err)
{
  صحیح ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

ساکن صحیح
receive_mounts (ساخت libcrun_fd_map *fds, صحیح sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  صحیح ret;

  اگر (fds->nfds == 0)
    واپس 0;

  ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_container, &how_many, حجم (how_many)));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "read from sync socket");

  برائے (i = 0; i < how_many; i++)
    {
      size_t اشاریہ;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (حرف *) &اشاریہ, حجم (اشاریہ), err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      اگر (اشاریہ >= fds->nfds)
        واپس crun_make_error (err, 0, "invalid mount data received");

      اگر (fds->fds[اشاریہ] >= 0)
        عارضی_FAILURE_RETRY (close (fds->fds[اشاریہ]));

      fds->fds[اشاریہ] = ret;
    }

  واپس 0;
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
ساکن صحیح
سیٹ_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;

  اگر (! container->host_has_cap_sys_ptrace)
    واپس 0;

  ret = prctl (PR_سیٹ_DUMPABLE, 0, 0, 0, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  واپس 0;
}

ساکن صحیح
سیٹ_id_init (libcrun_container_t *container, libcrun_error_t *err)
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
      bool جڑ_mapped = false;

      اگر (def->linux->uid_mappings_len != 0)
        {
          جڑ_mapped = جڑ_mapped_اندر_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          اگر (! جڑ_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      اگر (def->linux->gid_mappings_len != 0)
        {
          جڑ_mapped = جڑ_mapped_اندر_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          اگر (! جڑ_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  اگر (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    واپس crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "setresgid to `%d`", gid);

  واپس 0;
}

ساکن bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  واپس mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, تہی, تہی);
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
ساکن صحیح
open_proc_mounts_اندر_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ساخت libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close صحیح socket_والد = -1;
  cleanup_close صحیح socket_بچہ = -1;
  cleanup_close صحیح جڑ_fd = -1;
  cleanup_close صحیح cwd_fd = -1;
  size_t i, missing = 0;
  صحیح saved_errno;
  صحیح sockets[2];
  pid_t pid;
  صحیح ret;

  اگر (def->جڑ == تہی || def->جڑ->راستہ == تہی)
    واپس 0;

  برائے (i = 0; i < def->mounts_len; i++)
    اگر (is_proc_mount (def->mounts[i]))
      missing++;
  اگر (missing == 0)
    واپس 1;

  جڑ_fd = open ("/", O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (جڑ_fd < 0))
    واپس crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (cwd_fd < 0))
    واپس crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "socketpair");
  socket_والد = sockets[0];
  socket_بچہ = sockets[1];

  ret = chroot (def->جڑ->راستہ);
  اگر (UNLIKELY (ret < 0))
    واپس 0;

  pid = fork ();
  saved_errno = errno;
  اگر (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      اگر (get_نجی_ڈیٹا (container)->unshare_flags & CLONE_NEWUSER)
        (خالی) unshare (CLONE_NEWNS);

      برائے (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close صحیح fd = -1;

          اگر (! is_proc_mount (def->mounts[i]))
            جاری;

          fd = open_mount_کا_type (container, def->mounts[i], err);
          اگر (fd < 0)
            محفوظ_exit (EXIT_FAILURE);

          ret = send_fd_مقصد_socket_with_payload (socket_بچہ, fd, (حرف *) &i, حجم (i), err);
          اگر (UNLIKELY (ret < 0))
            محفوظ_exit (EXIT_FAILURE);
        }
      محفوظ_exit (EXIT_SUCCESS);
    }

  اگر (UNLIKELY (fchdir (جڑ_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    واپس crun_make_error (err, errno, "restore the root directory");

  اگر (UNLIKELY (pid < 0))
    واپس crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_بچہ);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  جب (missing > 0)
    {
      libcrun_error_t tmp_err = تہی;

      ret = receive_fd_from_socket_with_payload (socket_والد, (حرف *) &i, حجم (i), &tmp_err);
      اگر (ret < 0)
        {
          crun_error_release (&tmp_err);
          توڑو;
        }
      اگر (i >= mount_fds->nfds)
        {
          عارضی_FAILURE_RETRY (close (ret));
          توڑو;
        }

      اگر (mount_fds->fds[i] >= 0)
        عارضی_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, تہی, 0);

  واپس missing == 0 ? 1 : 0;
}

ساکن صحیح
init_container (libcrun_container_t *container, صحیح sync_socket_container, ساخت init_حالت_s *init_حالت,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ساخت libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close صحیح timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  صحیح ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = سیٹ_init_non_dumpable (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (init_حالت->idx_pidns_مقصد_join_immediately >= 0 || init_حالت->idx_timens_مقصد_join_immediately >= 0)
    {
      pid_t new_pid;

      اگر (init_حالت->idx_pidns_مقصد_join_immediately >= 0)
        {
          ret = setns (init_حالت->fd[init_حالت->idx_pidns_مقصد_join_immediately], CLONE_NEWPID);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_حالت->fd[init_حالت->idx_pidns_مقصد_join_immediately]);
        }

      اگر (init_حالت->idx_timens_مقصد_join_immediately >= 0)
        {
          ret = setns (init_حالت->fd[init_حالت->idx_timens_مقصد_join_immediately], CLONE_NEWTIME);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_حالت->fd[init_حالت->idx_timens_مقصد_join_immediately]);
        }

      اگر (init_حالت->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_اندر_pidns (container, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          delayed_pidns_fork = ret > 0;
        }

      اگر (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_مقصد_sync_socket (sync_socket_container, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_container, &new_pid, حجم (new_pid)));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "write to sync socket");
        }
      ورنہ
        {
          new_pid = fork ();
          اگر (UNLIKELY (new_pid < 0))
            واپس crun_make_error (err, errno, "fork");
        }

      اگر (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_مقصد_sync_socket (sync_socket_container, err);
          اگر (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_container, &new_pid, حجم (new_pid)));
          اگر (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          محفوظ_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = libcrun_سیٹ_oom (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (init_حالت->fd_len > 0)
    {
      ret = join_namespaces (def, init_حالت->fd, init_حالت->fd_len, init_حالت->اشاریہ, true, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  اگر ((init_حالت->all_namespaces & CLONE_NEWUSER) && (init_حالت->join_pidns || init_حالت->join_ipcns))
    {
      برائے (i = 0; i < def->mounts_len; i++)
        {
          صحیح fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          اگر (def->mounts[i]->type == تہی)
            جاری;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          اگر (init_حالت->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->ماخذ, تہی, تہی, تہی);
          اگر (init_حالت->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->ماخذ, تہی, تہی, تہی);

          اگر (fd >= 0)
            {
              اگر (mount_fds->fds[i] >= 0)
                عارضی_FAILURE_RETRY (close (mount_fds->fds[i]));
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
  اگر (def->linux->وقت_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      اگر (UNLIKELY (timens_offsets_fd < 0))
        واپس timens_offsets_fd;
    }

  اگر (init_حالت->all_namespaces & CLONE_NEWUSER)
    {
      اگر (init_حالت->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "unshare user namespace");

          init_حالت->namespaces_مقصد_unshare &= ~CLONE_NEWUSER;

          ret = send_success_مقصد_sync_socket (sync_socket_container, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      اگر (init_حالت->userns_اشاریہ < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
      ورنہ
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_حالت->fd[init_حالت->userns_اشاریہ], CLONE_NEWUSER);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_حالت->userns_اشاریہ_origin]->راستہ);
        }

      ret = سیٹ_id_init (container, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = سیٹ_init_non_dumpable (container, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = join_namespaces (def, init_حالت->fd, init_حالت->fd_len, init_حالت->اشاریہ, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (init_حالت->namespaces_مقصد_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_حالت->namespaces_مقصد_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "unshare");
    }

  اگر (def->linux->وقت_offsets)
    {
      حرف fmt_بفر[128];
      صحیح fd = timens_offsets_fd;

      اگر (def->linux->وقت_offsets->boottime)
        {
          ret = snprintf (fmt_بفر, حجم (fmt_بفر), "boottime %" PRIi64 " %" PRIu32, def->linux->وقت_offsets->boottime->secs, def->linux->وقت_offsets->boottime->nanosecs);
          اگر (UNLIKELY (ret >= (صحیح) حجم (fmt_بفر)))
            واپس crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (طویل طویل صحیح) def->linux->وقت_offsets->boottime->secs, def->linux->وقت_offsets->boottime->nanosecs);
          ret = اكتب (fd, fmt_بفر, لڑی_لمبائی (fmt_بفر));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      اگر (def->linux->وقت_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (طویل طویل صحیح) def->linux->وقت_offsets->monotonic->secs, def->linux->وقت_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_بفر, حجم (fmt_بفر), "monotonic %" PRIi64 " %" PRIu32, def->linux->وقت_offsets->monotonic->secs, def->linux->وقت_offsets->monotonic->nanosecs);
          اگر (UNLIKELY (ret >= (صحیح) حجم (fmt_بفر)))
            واپس crun_make_error (err, 0, "internal error: static buffer too small");

          ret = اكتب (fd, fmt_بفر, لڑی_لمبائی (fmt_بفر));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  اگر (init_حالت->must_fork)
    {
      اگر (init_حالت->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_اندر_pidns (container, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          delayed_pidns_fork = ret > 0;
        }

      اگر (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          اگر (UNLIKELY (pid_container < 0))
            واپس crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      اگر (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_مقصد_sync_socket (sync_socket_container, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_container, &pid_container, حجم (pid_container)));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "write to sync socket");

          اگر (pid_container)
            محفوظ_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  get_نجی_ڈیٹا (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_مقصد_sync_socket (sync_socket_container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  واپس get_نجی_ڈیٹا (container)->delayed_pidns_fork;
}

صحیح
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close صحیح client_fd = -1;
  cleanup_close صحیح pidfd = -1;
  ثابت حرف *v;

  v = ڈھونڈو_annotation (container, "run.oci.pidfd_receiver");
  اگر (v == تہی)
    واپس 0;

  pidfd = syscall_pidfd_open (pid, 0);
  اگر (UNLIKELY (pidfd < 0))
    واپس crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  اگر (UNLIKELY (client_fd < 0))
    واپس client_fd;

  واپس send_fd_مقصد_socket (client_fd, pidfd, err);
}

ساکن bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  اگر (process == تہی || process->exec_cpu_affinity == تہی)
    واپس false;
  واپس (! is_خالی_خانے_string (process->exec_cpu_affinity->initial))
         || (! is_خالی_خانے_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_تسلسل_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, خالی *دلائل,
                             صحیح *sync_socket_برآمد, ساخت libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) ساخت init_حالت_s init_حالت;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحیح sync_socket_container = -1;
  حرف *notify_socket_env = تہی;
  cleanup_close صحیح sync_socket_host = -1;
  __attribute__ ((غیر_مستعمل)) cleanup_close صحیح restore_pidns = -1;
  صحیح پہلا_clone_دلائل = 0;
  صحیح sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  صحیح ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_حالت (&init_حالت, container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  get_نجی_ڈیٹا (container)->unshare_flags = init_حالت.all_namespaces;
  get_نجی_ڈیٹا (container)->joined_mount_ns
      = (init_حالت.all_namespaces & CLONE_NEWNS)
        && ! (init_حالت.namespaces_مقصد_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_حالت.all_namespaces &= ~CLONE_NEWCGROUP;
  get_نجی_ڈیٹا (container)->unshare_cgroupns = init_حالت.namespaces_مقصد_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#اگر_تعریف_شدہ HAVE_SYSTEMD
  اگر (def->جڑ)
    {
      ret = do_notify_socket (container, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
#اگر_ختم

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  اگر (def->process)
    {
      ret = libcrun_سیٹ_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = libcrun_سیٹ_oom (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_container_notify_handler (دلائل, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->جڑ ? container->container_def->جڑ->راستہ : تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_حالت.delayed_userns_create
      = (init_حالت.all_namespaces & CLONE_NEWUSER) && init_حالت.userns_اشاریہ < 0 && init_حالت.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_حالت.all_namespaces & CLONE_NEWUSER) && init_حالت.userns_اشاریہ < 0;
  برائے (i = 0; i < init_حالت.fd_len; i++)
    {
      انتخاب (init_حالت.قدر[i])
        {
        حالت CLONE_NEWIPC:
          اگر (init_حالت.all_namespaces & CLONE_NEWUSER)
            init_حالت.join_ipcns = true;
          توڑو;

        حالت CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_حالت.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          اگر (! new_userns)
            init_حالت.must_fork = true;
          ورنہ
            {
              init_حالت.join_pidns = true;
              init_حالت.idx_pidns_مقصد_join_immediately = i;
              init_حالت.namespaces_مقصد_unshare &= ~CLONE_NEWPID;
            }
          توڑو;

        حالت CLONE_NEWTIME:
          اگر (! new_userns)
            init_حالت.must_fork = true;
          ورنہ
            {
              init_حالت.idx_timens_مقصد_join_immediately = i;
              init_حالت.namespaces_مقصد_unshare &= ~CLONE_NEWTIME;
            }
          توڑو;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  اگر ((init_حالت.namespaces_مقصد_unshare & CLONE_NEWUSER) && init_حالت.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      پہلا_clone_دلائل = CLONE_NEWUSER;
    }
  ورنہ اگر ((init_حالت.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      پہلا_clone_دلائل = init_حالت.namespaces_مقصد_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_حالت.namespaces_مقصد_unshare &= ~پہلا_clone_دلائل;

  /* Check if there are still namespaces that require a fork().  */
  اگر (init_حالت.namespaces_مقصد_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      پہلا_clone_دلائل &= ~CLONE_NEWPID;
      init_حالت.namespaces_مقصد_unshare |= CLONE_NEWPID;
      init_حالت.must_fork = true;
      init_حالت.can_delay_pidns_fork = false;
    }

  pid = -1;
  اگر (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      ساخت clone3_دلائل clone3_دلائل_2;
      memset (&clone3_دلائل_2, 0, حجم (clone3_دلائل_2));
      clone3_دلائل_2.exit_signal = SIGCHLD;
      clone3_دلائل_2.flags = پہلا_clone_دلائل;

      clone3_دلائل_2.flags |= CLONE_INTO_CGROUP;
      clone3_دلائل_2.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_دلائل_2);
      اگر (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  اگر (pid < 0)
    {
      pid = syscall_clone (پہلا_clone_دلائل | SIGCHLD, تہی);
      اگر (UNLIKELY (pid < 0))
        واپس crun_make_error (err, errno, "clone");
    }

  اگر (pid)
    {
      __attribute__ ((غیر_مستعمل)) cleanup_pid pid_t pid_مقصد_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = close_and_reset (&sync_socket_container);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_نجی_ڈیٹا (container)->notify_socket_درخت_fd));

      اگر (init_حالت.idx_pidns_مقصد_join_immediately >= 0 || init_حالت.idx_timens_مقصد_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_host, &new_pid, حجم (new_pid)));
          اگر (UNLIKELY (ret != حجم (new_pid)))
            واپس crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          اگر (new_pid == 0)
            get_نجی_ڈیٹا (container)->delayed_pidns_fork = true;
          ورنہ
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, تہی, 0);

              pid_مقصد_clean = pid = new_pid;
            }

          ret = send_success_مقصد_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      اگر (init_حالت.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      اگر ((init_حالت.all_namespaces & CLONE_NEWUSER) && init_حالت.userns_اشاریہ < 0)
        {
          ret = libcrun_سیٹ_usernamespace (container, pid, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          ret = send_success_مقصد_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      اگر (init_حالت.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_host, &grandchild, حجم (grandchild)));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_مقصد_sync_socket (sync_socket_host, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          اگر (grandchild == 0)
            get_نجی_ڈیٹا (container)->delayed_pidns_fork = true;
          ورنہ
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, تہی, 0);

              pid_مقصد_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      *sync_socket_برآمد = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      اگر (! get_نجی_ڈیٹا (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      pid_مقصد_clean = 0;
      واپس pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  اگر (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_حالت, err);
  اگر (UNLIKELY (ret < 0))
    send_error_مقصد_sync_socket_and_die (sync_socket_container, false, err);
  ورنہ
    {
      ret = send_success_مقصد_sync_socket (sync_socket_container, err);
      اگر (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->پیغام);
    }

  /* Jump into the specified entrypoint.  */
  اگر (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (دلائل, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  اگر (*err)
    libcrun_fail_with_error ((*err)->حالت_2, "%s", (*err)->پیغام);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  اگر (ret == 0)
    محفوظ_exit (EXIT_SUCCESS);

  محفوظ_exit (EXIT_FAILURE);
}

ساکن صحیح
join_process_والد_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t بچہ_pid, صحیح sync_socket_fd,
                            libcrun_container_حالت_t *حالت_2,
                            bool need_move_مقصد_cgroup, ثابت حرف *sub_cgroup,
                            صحیح *terminal_fd, libcrun_error_t *err)
{
  صحیح ret;
  حرف res;
  pid_t pid;
  cleanup_close صحیح sync_fd = sync_socket_fd;

  اگر (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = عارضی_FAILURE_RETRY (اقرأ (sync_fd, &res, 1));
  اگر (UNLIKELY (ret != 1))
    واپس crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  اگر (res != '0')
    واپس crun_make_error (err, 0, "fail startup");

  ret = عارضی_FAILURE_RETRY (اقرأ (sync_fd, &pid, حجم (pid)));
  اگر (UNLIKELY (ret != حجم (pid)))
    واپس crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (بچہ_pid, تہی, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "waitpid for exec child pid");

  اگر (process && process->exec_cpu_affinity)
    {
      ret = libcrun_سیٹ_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_نقاب (pid, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (need_move_مقصد_cgroup)
    {
      cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *cgroup_حالت = libcrun_cgroup_make_حالت (حالت_2);
      cleanup_free حرف *final_cgroup = تہی;

      اگر (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, حالت_2->cgroup_راستہ, sub_cgroup, تہی);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_حالت, final_cgroup ?: حالت_2->cgroup_راستہ, pid, حالت_2->pid, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_سیٹ_scheduler (pid, process, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (process && process->exec_cpu_affinity)
    {
      ret = libcrun_سیٹ_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_سیٹ_io_ترجیح (pid, process, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = عارضی_FAILURE_RETRY (اكتب (sync_fd, &ret, حجم (ret)));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "write to sync socket");

  اگر (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      اگر (UNLIKELY (ret < 0))
        {
          صحیح err_رمز = 0;
          cleanup_free حرف *err_str = تہی;

          اگر (read_error_from_sync_socket (sync_fd, &err_رمز, &err_str))
            {
              اگر (! is_خالی_خانے_string (err_str))
                {
                  crun_error_release (err);
                  واپس crun_make_error (err, err_رمز, "%s", err_str);
                }
            }

          واپس crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  واپس pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
ساکن صحیح
try_setns_with_pidfd (pid_t pid_مقصد_join, libcrun_container_t *container, libcrun_container_حالت_t *حالت_2, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحیح pidfd_pid_مقصد_join = -1;
  صحیح all_flags = 0;
  size_t i;
  صحیح ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  اگر (def->linux && def->linux->namespaces)
    {
      برائے (i = 0; i < def->linux->namespaces_len; i++)
        اگر (! is_خالی_خانے_string (def->linux->namespaces[i]->راستہ))
          واپس 0;
    }

  pidfd_pid_مقصد_join = syscall_pidfd_open (pid_مقصد_join, 0);
  اگر (UNLIKELY (pidfd_pid_مقصد_join < 0))
    واپس 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  اگر (ret == 0)
    واپس crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  برائے (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].قدر;

  ret = setns (pidfd_pid_مقصد_join, all_flags);
  اگر (UNLIKELY (ret < 0))
    واپس 0;

  واپس 1;
}

ساکن صحیح
join_process_namespaces (libcrun_container_t *container, pid_t pid_مقصد_join, libcrun_container_حالت_t *حالت_2, libcrun_error_t *err)
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
  ret = try_setns_with_pidfd (pid_مقصد_join, container, حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  /* Nothing left to do if the namespaces were joined.  */
  اگر (LIKELY (ret > 0))
    واپس 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  اگر (def->linux->namespaces_len >= MAX_NAMESPACES)
    واپس crun_make_error (err, 0, "invalid configuration");

  برائے (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free حرف *ns_راستہ = تہی;

      xasprintf (&ns_راستہ, "%d/ns/%s", pid_مقصد_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_راستہ, O_RDONLY, err);
      اگر (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          اگر (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              جاری;
            }

          ret = fds[i];
          جاؤ باہر_نکلو;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ باہر_نکلو;
  اگر (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      جاؤ باہر_نکلو;
    }

  برائے (i = 0; namespaces[i].ns_file; i++)
    {
      اگر (namespaces[i].قدر == CLONE_NEWUSER)
        جاری;

      ret = setns (fds[i], 0);
      اگر (ret == 0)
        fds_joined[i] = 1;
    }
  برائے (i = 0; namespaces[i].ns_file; i++)
    {
      اگر (fds_joined[i])
        جاری;
      ret = setns (fds[i], 0);
      اگر (ret == 0)
        fds_joined[i] = 1;
    }
  برائے (i = 0; namespaces[i].ns_file; i++)
    {
      اگر (fds_joined[i])
        جاری;
      ret = setns (fds[i], 0);
      اگر (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool ملا = false;

          برائے (j = 0; j < def->linux->namespaces_len; j++)
            {
              اگر (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  ملا = true;
                  توڑو;
                }
            }
          اگر (! ملا)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              جاری;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          جاؤ باہر_نکلو;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

باہر_نکلو:
  برائے (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  واپس ret;
}

صحیح
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_مقصد_join,
                      libcrun_container_حالت_t *حالت_2,
                      ثابت حرف *sub_cgroup,
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
  ساخت clone3_دلائل clone3_دلائل_2;
  bool need_move_مقصد_cgroup;

  اگر (! detach)
    {
      ret = prctl (PR_سیٹ_بچہ_SUBREAPER, 1, 0, 0, 0);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "set child subreaper");
    }

  اگر (راستہ_has_dot_dot_component (sub_cgroup))
    واپس crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  اگر (sub_cgroup && ! is_خالی_خانے_string (حالت_2->cgroup_راستہ))
    {
      cleanup_free حرف *final_cgroup = تہی;

      ret = append_paths (&final_cgroup, err, حالت_2->cgroup_راستہ, sub_cgroup, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *cgroup_حالت = تہی;

    cgroup_حالت = libcrun_cgroup_make_حالت (حالت_2);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    اگر (get_force_cgroup_v1_annotation (container) == تہی)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_حالت, sub_cgroup, err);
        اگر (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_دلائل_2, 0, حجم (clone3_دلائل_2));
  clone3_دلائل_2.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  اگر (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_مقصد_cgroup = true;
  ورنہ
    {
      need_move_مقصد_cgroup = false;
      clone3_دلائل_2.flags |= CLONE_INTO_CGROUP;
      clone3_دلائل_2.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_دلائل_2);

  اگر (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_سیٹ_scheduler (pid, process, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  /* On errors, fall back to fork().  */
  اگر (pid < 0)
    {
      need_move_مقصد_cgroup = true;

      pid = fork ();
      اگر (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          جاؤ باہر_نکلو;
        }
    }

  اگر (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      واپس join_process_والد_helper (context, container,
                                         process, pid, sync_fd,
                                         حالت_2, need_move_مقصد_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_مقصد_join, حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    {
      عارضی_FAILURE_RETRY (اكتب (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->حالت_2, "%s", (*err)->پیغام);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  اگر (UNLIKELY (pid < 0))
    {
      صحیح saved_errno = errno;
      عارضی_FAILURE_RETRY (اكتب (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  اگر (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = عارضی_FAILURE_RETRY (اكتب (sync_fd, "0", 1));
      اگر (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          محفوظ_exit (EXIT_FAILURE);
        }

      ret = عارضی_FAILURE_RETRY (اكتب (sync_fd, &pid, حجم (pid)));
      اگر (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          محفوظ_exit (EXIT_FAILURE);
        }

      محفوظ_exit (EXIT_SUCCESS);
    }
  ورنہ
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free حرف *pty = تہی;
      صحیح r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = عارضی_FAILURE_RETRY (اقرأ (sync_fd, &r, حجم (r)));
      اگر (UNLIKELY (ret != حجم (r) || r < 0))
        محفوظ_exit (EXIT_FAILURE);

      ret = setsid ();
      اگر (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_مقصد_sync_socket_and_die (sync_fd, true, err);
        }

      اگر (terminal_fd)
        {
          cleanup_close صحیح ptmx_fd = -1;

          ret = سیٹ_id_init (container, err);
          اگر (UNLIKELY (ret < 0))
            send_error_مقصد_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          اگر (UNLIKELY (ptmx_fd < 0))
            send_error_مقصد_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_مقصد_socket (sync_fd, ptmx_fd, err);
          اگر (UNLIKELY (ret < 0))
            send_error_مقصد_sync_socket_and_die (sync_fd, true, err);
        }
    }

  واپس 0;

باہر_نکلو:
  اگر (sync_socket_fd[0] >= 0)
    عارضی_FAILURE_RETRY (close (sync_socket_fd[0]));
  اگر (sync_socket_fd[1] >= 0)
    عارضی_FAILURE_RETRY (close (sync_socket_fd[1]));
  واپس ret;
}

صحیح
libcrun_linux_container_update (libcrun_container_حالت_t *حالت_2, ثابت حرف *state_جڑ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *cgroup_حالت = تہی;

  cgroup_حالت = libcrun_cgroup_make_حالت (حالت_2);

  واپس libcrun_update_cgroup_resources (cgroup_حالت, state_جڑ, resources, err);
}

ساکن صحیح
libcrun_container_pause_unpause_linux (libcrun_container_حالت_t *حالت_2, ثابت bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *cgroup_حالت = تہی;

  cgroup_حالت = libcrun_cgroup_make_حالت (حالت_2);

  واپس libcrun_cgroup_pause_unpause (cgroup_حالت, pause, err);
}

صحیح
libcrun_container_pause_linux (libcrun_container_حالت_t *حالت_2, libcrun_error_t *err)
{
  واپس libcrun_container_pause_unpause_linux (حالت_2, true, err);
}

صحیح
libcrun_container_unpause_linux (libcrun_container_حالت_t *حالت_2, libcrun_error_t *err)
{
  واپس libcrun_container_pause_unpause_linux (حالت_2, false, err);
}

صحیح
libcrun_سیٹ_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  غیرعلامتی طویل persona = 0;
  صحیح ret;

  اگر (strcmp (p->domain, "LINUX") == 0)
    persona = فی_LINUX;
  ورنہ اگر (strcmp (p->domain, "LINUX32") == 0)
    persona = فی_LINUX32;
  ورنہ
    واپس crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, 0, "personality to `%s`", p->domain);

  واپس 0;
}

صحیح
libcrun_configure_جال (libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;
  size_t i;
  bool configure_جال = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحیح sockfd = -1;

  برائے (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحیح قدر = libcrun_ڈھونڈو_namespace (def->linux->namespaces[i]->type);
      اگر (UNLIKELY (قدر < 0))
        واپس crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      اگر (قدر == CLONE_NEWNET && def->linux->namespaces[i]->راستہ == تہی)
        {
          configure_جال = true;
          توڑو;
        }
    }

  اگر (! configure_جال)
    واپس 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  اگر (LIKELY (sockfd >= 0))
    {
      ساخت ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_اوپر | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  ورنہ
    {
      ساخت nlmsghdr *hdr_recv;
      حرف buf[حجم (ساخت nlmsghdr) + حجم (ساخت nlmsgerr)];
      ساخت sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      ساخت sockaddr_nl addr_مقصد = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      ساخت nlmsghdr hdr = {
        .nlmsg_len = حجم (ساخت nlmsghdr) + حجم (ساخت ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      ساخت ifinfomsg پیغام = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_اشاریہ = 1,
        .ifi_flags = IFF_اوپر,
        .ifi_change = IFF_اوپر,
      };

      *((ساخت nlmsghdr *) buf) = hdr;
      *((ساخت ifinfomsg *) (buf + حجم (ساخت nlmsghdr))) = پیغام;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      اگر (UNLIKELY (sockfd < 0))
        واپس crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (ساخت sockaddr *) &addr, حجم (addr));
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, حجم (buf), 0, (ساخت sockaddr *) &addr_مقصد, حجم (addr_مقصد));
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, حجم (buf), 0, تہی, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (ساخت nlmsghdr *) buf;

      اگر (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          ساخت nlmsgerr *err_ڈیٹا = (ساخت nlmsgerr *) NLMSG_ڈیٹا (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          اگر (err_ڈیٹا->error < 0)
            واپس crun_make_error (err, -err_ڈیٹا->error, "netlink error while configuring network");
        }
    }

  واپس 0;
}

/* Protection for attacks like CVE-2019-5736.  */
صحیح ensure_cloned_ثنائی ();
__attribute__ ((constructor)) ساکن خالی
libcrun_rexec (خالی)
{
  اگر (ensure_cloned_ثنائی () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      محفوظ_exit (EXIT_FAILURE);
    }
}

صحیح
libcrun_container_checkpoint_linux (libcrun_container_حالت_t *حالت_2, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  واپس libcrun_container_checkpoint_linux_criu (حالت_2, container, cr_options, err);
}

صحیح
libcrun_container_restore_linux (libcrun_container_حالت_t *حالت_2, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  صحیح ret;
  ret = libcrun_container_restore_linux_criu (حالت_2, container, cr_options, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
ساکن صحیح
libcrun_kill_linux_no_pidfd (libcrun_container_حالت_t *حالت_2, bool check_pid, صحیح اشارہ, libcrun_error_t *err)
{
  صحیح ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  اگر (check_pid)
    {
      ret = libcrun_check_pid_valid (حالت_2, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      /* The pid is not valid anymore, return an error.  */
      اگر (ret == 0)
        {
          errno = ESRCH;
          واپس crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (حالت_2->pid, اشارہ);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "kill container");
  واپس 0;
}

صحیح
libcrun_kill_linux (libcrun_container_حالت_t *حالت_2, صحیح اشارہ, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_close صحیح pidfd = -1;

  pidfd = syscall_pidfd_open (حالت_2->pid, 0);
  اگر (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      اگر (errno == ENOSYS)
        واپس libcrun_kill_linux_no_pidfd (حالت_2, true, اشارہ, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      اگر (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        واپس crun_make_error (err, ESRCH, "process not running");
      واپس crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* The pid is not valid anymore, return an error.  */
  اگر (ret == 0)
    {
      errno = ESRCH;
      واپس crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, اشارہ, تہی, 0);
  اگر (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      اگر (errno == ENOSYS)
        واپس libcrun_kill_linux_no_pidfd (حالت_2, false, اشارہ, err);
      واپس crun_make_error (err, errno, "send signal to pidfd");
    }

  واپس 0;
}

ساکن ثابت حرف *
libcrun_get_intelrdt_name (ثابت حرف *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  اگر (def == تہی || def->linux == تہی || def->linux->intel_rdt == تہی || def->linux->intel_rdt->clos_id == تہی)
    {
      اگر (explicit)
        *explicit = false;
      واپس ctr_name;
    }

  اگر (explicit)
    *explicit = true;
  واپس def->linux->intel_rdt->clos_id;
}

صحیح
libcrun_apply_intelrdt (ثابت حرف *ctr_name, libcrun_container_t *container, pid_t pid, صحیح actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = تہی;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  ثابت حرف *name;
  صحیح ret;

  اگر (container)
    def = container->container_def;

  اگر (def == تہی || def->linux == تہی || def->linux->intel_rdt == تہی)
    واپس 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  اگر (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_گروہ (name, ctr_name, err);
          اگر (UNLIKELY (ret < 0))
            جاؤ fail;

          has_monitoring = true;
        }
    }

  اگر (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ fail;
    }

  اگر (actions & LIBCRUN_INTELRDT_MOVE)
    {
      ثابت حرف *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : تہی;
      ret = resctl_move_task_مقصد (name, monitoring_name, pid, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ fail;
    }

  واپس 0;

fail:
  اگر (has_monitoring)
    {
      libcrun_error_t tmp_err = تہی;
      صحیح tmp_ret;

      tmp_ret = resctl_destroy_monitoring_گروہ (name, ctr_name, &tmp_err);
      اگر (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  اگر (created)
    {
      libcrun_error_t tmp_err = تہی;
      صحیح tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      اگر (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  واپس ret;
}

صحیح
libcrun_destroy_intelrdt (ثابت حرف *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  ثابت حرف *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  اگر (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      صحیح ret;

      ret = resctl_destroy_monitoring_گروہ (clos_id, container_id, err);
      اگر (ret < 0)
        واپس ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  اگر (explicit)
    واپس 0;

  واپس resctl_destroy (clos_id, err);
}

صحیح
libcrun_update_intel_rdt (ثابت حرف *ctr_name, libcrun_container_t *container, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = تہی;
  ثابت حرف *name;

  اگر (container)
    def = container->container_def;

  اگر (def == تہی || def->linux == تہی || def->linux->intel_rdt == تہی)
    واپس 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, تہی);

  واپس resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
صحیح
libcrun_محفوظ_chdir (ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *بفر = تہی;
  صحیح ret;

  ret = chdir (راستہ);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "chdir to `%s`", راستہ);

  بفر = xmalloc (PATH_MAX);
  ret = syscall_getcwd (بفر, PATH_MAX);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  اگر (ret == 0 || بفر[0] != '/')
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
#تعریف_کرو UNREACHABLE "(unreachable)"
#تعریف_کرو UNREACHABLE_LEN ((صحیح) حجم (UNREACHABLE) - 1)

      اگر ((ret >= UNREACHABLE_LEN) && (memcmp (بفر, UNREACHABLE, UNREACHABLE_LEN) == 0))
        واپس crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      واپس crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  واپس 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
ساکن صحیح
تسلسل_callback_اندر_namespace (صحیح mntns_fd, صحیح (*callback) (خالی *, libcrun_error_t *), خالی *arg, libcrun_error_t *err)
{
  صحیح wait_حالت = 0;
  pid_t pid;
  صحیح ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  اگر (UNLIKELY (pid < 0))
    واپس crun_make_error (err, errno, "vfork");

  اگر (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      اگر (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          محفوظ_exit (ret);
        }
      ret = chdir ("/");
      اگر (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, errno, "chdir to `/`");
          محفوظ_exit (ret);
        }

      ret = callback (arg, err);
      /* On failure (ret < 0) the created error will be
         used by the parent proc */
      محفوظ_exit (ret);
    }

  ret = waitpid_ignore_stopped (pid, &wait_حالت, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  واپس get_process_exit_حالت (wait_حالت);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
ساکن صحیح
check_container_userns (libcrun_container_t *container, صحیح userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close صحیح expected_fd = -1;
  cleanup_close صحیح والد_fd = -1;
  ساخت stat st, expected_st;
  ثابت حرف *راستہ = تہی;
  size_t i;
  صحیح ret;

  اگر (def->linux)
    برائے (i = 0; i < def->linux->namespaces_len; i++)
      اگر (libcrun_ڈھونڈو_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          راستہ = def->linux->namespaces[i]->راستہ;
          new_userns = راستہ == تہی;
          توڑو;
        }

  اگر (راستہ == تہی)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      اگر (UNLIKELY (expected_fd < 0))
        واپس expected_fd;
    }
  ورنہ
    {
      expected_fd = open (راستہ, O_RDONLY | O_CLOEXEC);
      اگر (UNLIKELY (expected_fd < 0))
        واپس crun_make_error (err, errno, "open `%s`", راستہ);
    }

  /* A user namespace created for the container is a child of the current one.  */
  اگر (new_userns)
    {
      والد_fd = ioctl (userns_fd, NS_GET_والد);
      اگر (UNLIKELY (والد_fd < 0))
        واپس crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = والد_fd;
    }

  ret = fstat (userns_fd, &st);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fstat user namespace");

  اگر (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    واپس crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  واپس 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
ساکن صحیح
open_container_mntns (libcrun_container_t *container, libcrun_container_حالت_t *حالت_2, صحیح *mntns_fd, صحیح *owner_fd, libcrun_error_t *err)
{
  cleanup_close صحیح mntns = -1;
  cleanup_close صحیح owner = -1;
  صحیح ret;

  mntns = libcrun_open_proc_pid_file (container, حالت_2->pid, "ns/mnt", O_RDONLY, err);
  اگر (UNLIKELY (mntns < 0))
    واپس mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (ret == 0)
    واپس crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  اگر (UNLIKELY (owner < 0))
    واپس crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  واپس 0;
}

ساکن صحیح
تسلسل_اندر_container_namespace (libcrun_container_t *container, libcrun_container_حالت_t *حالت_2, صحیح (*callback) (خالی *, libcrun_error_t *), خالی *arg, libcrun_error_t *err)
{
  cleanup_close صحیح mntns_fd = -1;
  cleanup_close صحیح owner_fd = -1;
  صحیح ret;

  ret = open_container_mntns (container, حالت_2, &mntns_fd, &owner_fd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس تسلسل_callback_اندر_namespace (mntns_fd, callback, arg, err);
}

ساخت umount_اندر_a_container_دلائل
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
ساکن صحیح
umount_nofollow (ثابت حرف *destination, libcrun_error_t *err)
{
  cleanup_close صحیح parentfd = -1;
  cleanup_close صحیح cwdfd = -1;
  حرف بفر[PATH_MAX];
  ثابت حرف *والد_2, *name;
  حرف *آخری_slash;
  size_t len;
  صحیح saved_errno;
  صحیح ret;

  اگر (is_خالی_خانے_string (destination))
    واپس crun_make_error (err, 0, "empty destination for mount");

  len = لڑی_لمبائی (destination);
  اگر (UNLIKELY (len >= حجم (بفر)))
    واپس crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  حافظہ_نقل (بفر, destination, len + 1);

  جب (len > 1 && بفر[len - 1] == '/')
    بفر[--len] = '\0';

  آخری_slash = strrchr (بفر, '/');
  اگر (آخری_slash == تہی)
    {
      والد_2 = ".";
      name = بفر;
    }
  ورنہ اگر (آخری_slash == بفر)
    {
      والد_2 = "/";
      name = بفر[1] ? بفر + 1 : ".";
    }
  ورنہ
    {
      *آخری_slash = '\0';
      والد_2 = بفر;
      name = آخری_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, والد_2, O_راستہ | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  اگر (UNLIKELY (parentfd < 0))
    واپس crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_راستہ | O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (cwdfd < 0))
    واپس crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  اگر (UNLIKELY (fchdir (cwdfd) < 0))
    واپس crun_make_error (err, errno, "fchdir to the previous working directory");

  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, saved_errno, "umount `%s`", destination);

  واپس 0;
}

ساکن صحیح
do_umount_اندر_a_container (خالی *arg, libcrun_error_t *err)
{
  ساخت umount_اندر_a_container_دلائل *دلائل = arg;
  size_t i;
  صحیح ret;

  برائے (i = 0; i < دلائل->len; i++)
    {
      ret = umount_nofollow (دلائل->mounts[i]->destination, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

ساخت mount_اندر_a_container_دلائل
{
  runtime_spec_schema_defs_mount **mounts;
  ساخت libcrun_fd_map *fds;
  صحیح pidfd;
  صحیح owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
ساکن صحیح
open_destination_nofollow (ثابت حرف *destination, bool dir, libcrun_error_t *err)
{
  ثابت uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  صحیح fd;
  صحیح ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_راستہ | O_CLOEXEC, 0, resolve);
  اگر (LIKELY (fd >= 0))
    واپس fd;

  اگر (errno != ENOENT)
    واپس crun_make_error (err, errno, "open `%s`", destination);

  اگر (dir)
    ret = crun_محفوظ_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  ورنہ
    ret = crun_محفوظ_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_راستہ | O_CLOEXEC, 0, resolve);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "open `%s`", destination);

  واپس fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
ساکن صحیح
create_destinations_اندر_a_container (خالی *arg, libcrun_error_t *err)
{
  ساخت mount_اندر_a_container_دلائل *دلائل = arg;
  size_t i;
  صحیح ret;

  ret = setns (دلائل->owner_fd, CLONE_NEWUSER);
  اگر (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "setresuid to `0`");
    }
  ورنہ اگر (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      واپس crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  برائے (i = 0; i < دلائل->len; i++)
    {
      cleanup_close صحیح dest_fd = -1;
      ساخت stat st;

      ret = fstat (دلائل->fds->fds[i], &st);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (دلائل->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      اگر (UNLIKELY (dest_fd < 0))
        واپس dest_fd;
    }

  واپس 0;
}

ساکن صحیح
do_mount_اندر_a_container (خالی *arg, libcrun_error_t *err)
{
  ساخت mount_اندر_a_container_دلائل *دلائل = arg;
  size_t i;
  صحیح ret;

  برائے (i = 0; i < دلائل->len; i++)
    {
      cleanup_close صحیح dest_fd = -1;
      ساخت stat st;

      ret = fstat (دلائل->fds->fds[i], &st);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (دلائل->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      اگر (UNLIKELY (dest_fd < 0))
        واپس dest_fd;

      ret = fs_move_mount_مقصد (دلائل->fds->fds[i], dest_fd, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "move mount to `%s`", دلائل->mounts[i]->destination);
    }

  واپس 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
ساکن صحیح
send_mounts_from_container_namespaces (libcrun_container_t *container, صحیح pidfd, runtime_spec_schema_defs_mount **mounts,
                                       ساخت libcrun_fd_map *fds, size_t len, صحیح flags, صحیح owner_fd, صحیح sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  صحیح ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_سیٹ_DUMPABLE, 0, 0, 0, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  اگر (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  ورنہ اگر (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      واپس crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  اگر (UNLIKELY (pid < 0))
    واپس crun_make_error (err, errno, "fork");

  اگر (pid)
    {
      صحیح wait_حالت = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = عارضی_FAILURE_RETRY (waitpid (pid, &wait_حالت, WUNTRACED));
      اگر (ret > 0 && WIFSTOPPED (wait_حالت))
        {
          kill (pid, SIGKILL);
          عارضی_FAILURE_RETRY (waitpid (pid, تہی, 0));
          واپس crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      محفوظ_exit (EXIT_SUCCESS);
    }

  ret = سیٹ_id_init (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = prctl (PR_سیٹ_DUMPABLE, 0, 0, 0, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  برائے (i = 0; i < len; i++)
    {
      cleanup_close صحیح fd = -1;

      اگر (fds->fds[i] >= 0)
        جاری;

      fd = open_mount_کا_type (container, mounts[i], err);
      اگر (UNLIKELY (fd < 0))
        واپس crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_مقصد_socket (sync_fd, fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
ساکن صحیح
get_namespaces_مقصد_join (libcrun_container_t *container, pid_t pid, صحیح *flags, libcrun_error_t *err)
{
  ساکن ثابت ساخت
  {
    ثابت حرف *name;
    صحیح flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  برائے (i = 0; i < حجم (namespaces) / حجم (namespaces[0]); i++)
    {
      cleanup_close صحیح self_fd = -1;
      cleanup_close صحیح ہدف_fd = -1;
      ساخت stat self_st, ہدف_st;
      cleanup_free حرف *self_راستہ = تہی;
      صحیح ret;

      xasprintf (&self_راستہ, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_راستہ, O_RDONLY, err);
      اگر (UNLIKELY (self_fd < 0))
        واپس self_fd;

      ہدف_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      اگر (UNLIKELY (ہدف_fd < 0))
        واپس ہدف_fd;

      ret = fstat (self_fd, &self_st);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (ہدف_fd, &ہدف_st);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      اگر (self_st.st_dev != ہدف_st.st_dev || self_st.st_ino != ہدف_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  واپس 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
ساکن صحیح
open_mounts_اندر_container_namespaces (libcrun_container_t *container, libcrun_container_حالت_t *حالت_2, صحیح owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, ساخت libcrun_fd_map *fds, size_t len,
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

  برائے (i = 0; i < len; i++)
    {
      اگر (fds->fds[i] >= 0)
        جاری;

      اگر (mounts[i]->type == تہی)
        واپس crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  اگر (! needed)
    واپس 0;

  pidfd = syscall_pidfd_open (حالت_2->pid, 0);
  اگر (UNLIKELY (pidfd < 0))
    واپس crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (ret == 0)
    واپس crun_make_error (err, 0, "container not running");

  ret = get_namespaces_مقصد_join (container, حالت_2->pid, &flags, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  اگر (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      عارضی_FAILURE_RETRY (close (sync_socket_fd[1]));
      واپس ret;
    }

  اگر (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      اگر (UNLIKELY (ret < 0))
        send_error_مقصد_sync_socket_and_die (sync_socket_fd[1], true, err);

      محفوظ_exit (EXIT_SUCCESS);
    }

  عارضی_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  برائے (i = 0; i < len; i++)
    {
      اگر (fds->fds[i] >= 0)
        جاری;

      ret = receive_fd_from_socket (sync_fd, err);
      اگر (UNLIKELY (ret < 0))
        {
          cleanup_free حرف *err_str = تہی;
          صحیح err_رمز = 0;

          اگر (read_error_from_sync_socket (sync_fd, &err_رمز, &err_str) && ! is_خالی_خانے_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_رمز, "%s", err_str);
            }
          ورنہ
            ret = crun_error_wrap (err, "receive mount fd");
          توڑو;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  عارضی_FAILURE_RETRY (waitpid (pid, تہی, 0));

  واپس ret;
}

ساکن غیرعلامتی طویل
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_سیٹ)
{
  cleanup_free حرف *ڈیٹا = تہی;
  غیرعلامتی طویل extra_flags = 0;
  غیرعلامتی طویل flags = 0;
  size_t j;

  اگر (mount->options == تہی)
    واپس get_default_flags (container, mount->destination, &ڈیٹا);

  برائے (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &ڈیٹا, rec_clear, rec_سیٹ);

  واپس flags;
}

صحیح
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_حالت_t *حالت_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ساخت mount_اندر_a_container_دلائل دلائل;
  cleanup_close_map ساخت libcrun_fd_map *fds = تہی;
  cleanup_close صحیح pidfd = -1;
  cleanup_close صحیح mntns_fd = -1;
  cleanup_close صحیح owner_fd = -1;
  pid_t pid = حالت_2->pid;
  size_t i;
  صحیح ret;

  ret = open_container_mntns (container, حالت_2, &mntns_fd, &owner_fd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  fds = make_libcrun_fd_map (len);

  برائے (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), تہی, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_سیٹ = 0;
          غیرعلامتی طویل اشاعت;

          اشاعت = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_سیٹ) & ALL_PROPAGATIONS_NO_REC;
          اگر (اشاعت == 0)
            اشاعت = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->ماخذ, recursive, false, nofollow, اشاعت, err);
          اگر (UNLIKELY (fds->fds[i] < 0))
            واپس fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_اندر_container_namespaces (container, حالت_2, owner_fd, mounts, fds, len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  برائے (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_سیٹ = 0;
      غیرعلامتی طویل flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_سیٹ);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_سیٹ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  دلائل.mounts = mounts;
  دلائل.fds = fds;
  دلائل.pidfd = pidfd;
  دلائل.owner_fd = owner_fd;
  دلائل.len = len;

  ret = تسلسل_callback_اندر_namespace (mntns_fd, create_destinations_اندر_a_container, &دلائل, err);
  اگر (UNLIKELY (ret > 0))
    واپس *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس تسلسل_callback_اندر_namespace (mntns_fd, do_mount_اندر_a_container, &دلائل, err);
}

صحیح
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_حالت_t *حالت_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  ساخت umount_اندر_a_container_دلائل دلائل;

  دلائل.mounts = mounts;
  دلائل.len = len;

  واپس تسلسل_اندر_container_namespace (container, حالت_2, do_umount_اندر_a_container, &دلائل, err);
}

ساخت restrict_mounts_اندر_a_container_دلائل
{
  ساخت libcrun_mount_flags_s *mounts;
  size_t len;
  صحیح rootfd;
};

ساکن صحیح
do_restrict_mounts_اندر_a_container (خالی *arg, libcrun_error_t *err)
{
  ساخت restrict_mounts_اندر_a_container_دلائل *دلائل = arg;
  size_t i;
  صحیح ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (دلائل->rootfd);
  اگر (LIKELY (ret == 0))
    ret = chroot (".");
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "chroot to the container root");

  برائے (i = 0; i < دلائل->len; i++)
    {
      غیرعلامتی طویل flags = دلائل->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      ثابت حرف *ہدف = consume_slashes (دلائل->mounts[i].ہدف);
      cleanup_close صحیح fd = -1;
      ساخت statfs sfs;
      غیرعلامتی طویل current_flags;

      اگر (flags == 0)
        جاری;

      fd = محفوظ_openat (AT_FDCWD, "/", ہدف, O_راستہ | O_CLOEXEC, 0, err);
      اگر (UNLIKELY (fd < 0))
        {
          اگر (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              جاری;
            }
          واپس fd;
        }

      ret = fstatfs (fd, &sfs);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "statfs `/%s`", ہدف);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      اگر ((current_flags & flags) == flags)
        جاری;

      ret = do_mount_setattr (false, ہدف, fd, 0, flags, err);
      اگر (LIKELY (ret == 0))
        جاری;

      اگر (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        واپس ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (تہی, ہدف, تہی, MS_REMOUNT | MS_BIND | flags | current_flags, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "remount `/%s`", ہدف);
    }

  واپس 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
صحیح
libcrun_restrict_mounts_اندر_container (pid_t pid, ساخت libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  ساخت restrict_mounts_اندر_a_container_دلائل دلائل;
  cleanup_close صحیح mntns_fd = -1;
  cleanup_close صحیح rootfd = -1;
  حرف راستہ[64];
  صحیح ret;

  snprintf (راستہ, حجم (راستہ), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (راستہ, O_RDONLY | O_CLOEXEC);
  اگر (UNLIKELY (mntns_fd < 0))
    واپس crun_make_error (err, errno, "open `%s`", راستہ);

  snprintf (راستہ, حجم (راستہ), "/proc/%d/root", pid);
  rootfd = open (راستہ, O_راستہ | O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (rootfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", راستہ);

  دلائل.mounts = mounts;
  دلائل.len = len;
  دلائل.rootfd = rootfd;

  ret = تسلسل_callback_اندر_namespace (mntns_fd, do_restrict_mounts_اندر_a_container, &دلائل, err);
  اگر (UNLIKELY (ret > 0))
    واپس *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  واپس ret;
}

صحیح
libcrun_move_جال_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحیح netns_fd = -1;
  size_t i;
  صحیح ret;

  اگر (def == تہی || def->linux == تہی || def->linux->net_devices == تہی)
    واپس 0;

  cleanup_free حرف *ns_راستہ = تہی;

  xasprintf (&ns_راستہ, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_راستہ, O_RDONLY, err);
  اگر (UNLIKELY (netns_fd < 0))
    واپس netns_fd;

  برائے (i = 0; i < def->linux->net_devices->len; i++)
    {
      ثابت حرف *new_name = def->linux->net_devices->اقدار[i]->name ?: def->linux->net_devices->کلیدیں[i];

      ret = move_جال_device (def->linux->net_devices->کلیدیں[i], new_name, netns_fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}
