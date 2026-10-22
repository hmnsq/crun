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

#تعریف_کرو GNU_سروت

#ضمن <config.h>
#ضمن "linux.h"
#ضمن "utils.h"
#ضمن "status.h"
#ضمن <string.h>
#ضمن <sched.h>
#ضمن <fcntl.h>
#ضمن <unistd.h>
#ضمن <sys/mount.h>
#جے_تعریف_شدہ HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#جے_انت
#إذا defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#جے_انت

#ضمن <sys/prctl.h>
#جے_تعریف_شدہ HAVE_CAP
#  include <sys/capability.h>
#جے_انت
#ضمن <sys/time.h>
#ضمن <sys/resource.h>
#ضمن <sys/sysmacros.h>
#ضمن <sys/types.h>
#ضمن <sys/stat.h>
#ضمن <grp.h>
#ضمن <signal.h>
#ضمن "terminal.h"
#ضمن "cgroup.h"
#ضمن "cgroup-utils.h"
#ضمن "status.h"
#ضمن "criu.h"
#ضمن "scheduler.h"
#ضمن "intelrdt.h"
#ضمن "io_priority.h"
#ضمن "net_device.h"

#ضمن <sys/socket.h>
#ضمن <libgen.h>
#ضمن <sys/wait.h>
#ضمن <sys/vfs.h>
#ضمن <sys/statvfs.h>
#ضمن <limits.h>
#ضمن <inttypes.h>
#ضمن <sys/personality.h>
#ضمن <net/if.h>
#ضمن <sys/xattr.h>
#ضمن <linux/netlink.h>
#ضمن <linux/rtnetlink.h>
#ضمن <sched.h>
#ضمن <linux/sched.h>
#ضمن <linux/magic.h>
#ضمن <linux/nsfs.h>

#ضمن <json-c/json.h>
#ضمن <ocispec/json_common.h>

#ضمن "mount_flags.h"
#ضمن "syscalls.h"

#جے_بنا_تعریف RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#جے_انت

#جے_بنا_تعریف OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#جے_انت

#جے_بنا_تعریف OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#جے_انت

#جے_بنا_تعریف OPEN_درخت_NAMESPACE
#  define OPEN_درخت_NAMESPACE 2
#جے_انت

#جے_بنا_تعریف MOVE_MOUNT_F_EMPTY_PATH
#  define MOVE_MOUNT_F_EMPTY_PATH 0x00000004
#جے_انت

#جے_بنا_تعریف MOVE_MOUNT_T_EMPTY_PATH
#  define MOVE_MOUNT_T_EMPTY_PATH 0x00000040
#جے_انت

#جے_بنا_تعریف FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#جے_انت

#جے_بنا_تعریف FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#جے_انت

#جے_بنا_تعریف FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#جے_انت

#تعریف_کرو ALL_PROPAGATIONS_NO_REC (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE)
#تعریف_کرو ALL_PROPAGATIONS (MS_REC | ALL_PROPAGATIONS_NO_REC)

/* The per-mount flags a remount clears unless they are explicitly set.  */
#تعریف_کرو REMOUNT_CLEAR_FLAGS (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW)

#جے_بنا_تعریف ST_NOSYMFOLLOW
#  define ST_NOSYMFOLLOW 0x2000
#جے_انت

#تعریف_کرو MOUNT_ATIME_FLAGS (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME)

/* Convert the ST_ flags reported by statfs(2) to the MS_ flags of mount(2).  */
ساكن بنانشانی لما
mount_flags_from_statfs (بنانشانی لما f_flags)
{
  بنانشانی لما flags = 0;

  إذا (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  إذا (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  إذا (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  إذا (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  إذا (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  إذا (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  إذا (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  إذا (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  إذا (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  أرجع flags;
}

بنية remount_s
{
  بنية remount_s *اگلا;
  حرف *نشانہ;
  صحيح targetfd;
  بنانشانی لما flags;
  حرف *انکڑے;
};

بنية نجی_انکڑے_s
{
  بنية remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  صحيح unshare_flags;
  صحيح unshare_cgroupns;

  حرف *host_notify_socket_رستہ;
  حرف *container_notify_socket_رستہ;
  حرف *unified_cgroup_رستہ;

  bool mount_dev_from_host;
  بنانشانی لما rootfs_پسار;
  bool deny_setgroups;

  صحيح procfd;

  ثابت حرف *rootfs;
  صحيح rootfsfd;

  صحيح notify_socket_درخت_fd;
  صحيح old_جڑ_fd;

  بنية libcrun_fd_map *mount_fds;
  بنية libcrun_fd_map *dev_fds;
  بنية libcrun_fd_map *needed_devs_fds;

  حرف **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  حرف *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  صحيح maskdir_fd;
  حرف *maskdir_proc_رستہ;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_دھرا;
  bool no_دھرا;
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

بنية linux_namespace_s
{
  ثابت حرف *name;
  ثابت حرف *ns_file;
  صحيح قدر;
};

ساكن فراغ
cleanup_نجی_انکڑے (فراغ *نجی_انکڑے)
{
  بنية نجی_انکڑے_s *p = نجی_انکڑے;

  إذا (p->rootfsfd >= 0)
    عارضی_FAILURE_RETRY (close (p->rootfsfd));
  إذا (p->procfd >= 0)
    عارضی_FAILURE_RETRY (close (p->procfd));
  إذا (p->maskdir_fd >= 0)
    عارضی_FAILURE_RETRY (close (p->maskdir_fd));
  إذا (p->old_جڑ_fd >= 0)
    عارضی_FAILURE_RETRY (close (p->old_جڑ_fd));
  إذا (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  إذا (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  إذا (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  إذا (p->copy_symlink_targets)
    {
      size_t i;
      لكل (i = 0; i < p->n_copy_symlink_targets; i++)
        حرر (p->copy_symlink_targets[i]);
      حرر (p->copy_symlink_targets);
    }
  حرر (p->unified_cgroup_رستہ);
  حرر (p->host_notify_socket_رستہ);
  حرر (p->container_notify_socket_رستہ);
  حرر (p->external_descriptors);
  حرر (p->maskdir_proc_رستہ);
  حرر (p->host_rootfs);
  حرر (p);
}

ساكن بنية نجی_انکڑے_s *
get_نجی_انکڑے (بنية libcrun_container_s *container)
{
  إذا (container->نجی_انکڑے == عدم)
    {
      بنية نجی_انکڑے_s *p = xmalloc0 (حجم (*p));
      container->نجی_انکڑے = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_درخت_fd = -1;
      p->old_جڑ_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_نجی_انکڑے = cleanup_نجی_انکڑے;
    }
  أرجع container->نجی_انکڑے;
}

#جے_بنا_تعریف CLONE_NEWTIME
#  define CLONE_NEWTIME 0x00000080
#جے_انت
#جے_بنا_تعریف CLONE_NEWCGROUP
#  define CLONE_NEWCGROUP 0x02000000
#جے_انت
#جے_بنا_تعریف AT_RECURSIVE
#  define AT_RECURSIVE 0x8000
#جے_انت

ساكن بنية linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { عدم, عدم, 0 } };

ساكن صحيح
get_and_reset (صحيح *old)
{
  صحيح tmp = *old;
  *old = -1;
  أرجع tmp;
}

صحيح
libcrun_لبھو_namespace (ثابت حرف *name)
{
  بنية linux_namespace_s *it;
  لكل (it = namespaces; it->name; it++)
    إذا (strcmp (it->name, name) == 0)
      أرجع it->قدر;
  أرجع -1;
}

ساكن صحيح
syscall_keyctl_join (ثابت حرف *name)
{
#تعریف_کرو KEYCTL_JOIN_SESSION_KEYRING 0x1
  أرجع (صحيح) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

ساكن صحيح
syscall_pidfd_open (pid_t pid, بنانشانی صحيح flags)
{
#إذا defined __NR_pidfd_open
  أرجع (صحيح) syscall (__NR_pidfd_open, pid, flags);
#وإلا
  (فراغ) pid;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#جے_انت
}

ساكن صحيح
syscall_pidfd_send_signal (صحيح pidfd, صحيح sig, siginfo_t *info, بنانشانی صحيح flags)
{
#إذا defined __NR_pidfd_send_signal
  أرجع (صحيح) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#وإلا
  (فراغ) pidfd;
  (فراغ) sig;
  (فراغ) info;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#جے_انت
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
ساكن uint64_t
ms_flags_منزل_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  إذا (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  إذا (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  إذا (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  إذا (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  إذا (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  إذا (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  إذا (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  إذا (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  أرجع attr;
}

ساكن صحيح
do_mount_setattr (bool recursive, ثابت حرف *نشانہ, صحيح targetfd, uint64_t clear, uint64_t سیٹ, libcrun_error_t *err)
{
  بنية mount_attr_s attr = {
    0,
  };
  صحيح ret;

  سیٹ &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.پسار = سیٹ & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_منزل_mount_attr (سیٹ & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_منزل_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  إذا (سیٹ & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      إذا (سیٹ & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      وإلا إذا (سیٹ & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      وإلا
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      إذا (سیٹ & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  وإلا إذا (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_خالی_تھاواں_رستہ, &attr);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "mount_setattr `/%s`", نشانہ);

  أرجع 0;
}

صحيح
get_bind_mount (صحيح dirfd, ثابت حرف *src, bool recursive, bool rdonly, bool nofollow, بنانشانی لما پسار, libcrun_error_t *err)
{
  cleanup_close صحيح open_درخت_fd = -1;
  بنية mount_attr_s attr = {
    0,
  };
  صحيح recursive_flag = (recursive ? AT_RECURSIVE : 0);
  صحيح ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  إذا (UNLIKELY (src == عدم))
    أرجع crun_make_error (err, EINVAL, "mount source is not specified");

  إذا (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.پسار = پسار;

  إذا (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_درخت_fd = syscall_open_درخت (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_خالی_تھاواں_رستہ : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  إذا (UNLIKELY (open_درخت_fd < 0))
    أرجع crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_درخت_fd, "", AT_خالی_تھاواں_رستہ | recursive_flag, &attr);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "mount_setattr `%s`", src);

  أرجع get_and_reset (&open_درخت_fd);
}

صحيح
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, حرف *option, حرف **باہرلا, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free حرف *mappings = عدم;
  ثابت حرف *it;
  صحيح ret;

  mappings = xmalloc (allocated);

  لكل (it = option; *it;)
    {
      bool relative = false;
      لما قدر[3];
      size_t i;

      إذا (*it == '\0')
        اكسر;

      إذا (*it == '#')
        it++;

      إذا (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      لكل (i = 0; i < 3; i++)
        {
          حرف *endptr = عدم;

          إذا (i > 0 && *it == '-')
            it++;

          إذا (*it == '\0')
            أرجع crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          قدر[i] = strtol (it, &endptr, 10);
          إذا (errno || endptr == it)
            أرجع crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      إذا (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          إذا (def == عدم
              || def->linux == عدم
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            أرجع crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          لكل (i = 0; i < mappings_len; i++)
            إذا (قدر[1] >= (int64_t) mappings[i]->container_id
                && قدر[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->ناپ)
              اكسر;

          إذا (i == mappings_len)
            أرجع crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          قدر[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      إذا (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", قدر[0], قدر[1], قدر[2]);
      إذا (UNLIKELY (ret >= (صحيح) (allocated - written)))
        أرجع crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *باہرلا = mappings;
  mappings = عدم;

  أرجع 0;
}

ساكن صحيح
format_mount_mappings (حرف **باہرلا,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  ثابت size_t max_len_mapping = 64;
  cleanup_free حرف *ret = عدم;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  لكل (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->ناپ);
      إذا (UNLIKELY (len >= max_len_mapping))
        أرجع crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *باہرلا = ret;
  ret = عدم;
  أرجع 0;
}

ساكن صحيح
format_mount_mapping (حرف **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t ناپ, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .ناپ = ناپ,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    عدم,
  };

  أرجع format_mount_mappings (ret, mappings, 1, written, err);
}

ساكن bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  إذا (def->linux == عدم)
    أرجع mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  إذا (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    أرجع false;

  إذا (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    أرجع false;

  لكل (s = 0; s < mnt->uid_mappings_len; s++)
    {
      إذا (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        أرجع false;
      إذا (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        أرجع false;
      إذا (mnt->uid_mappings[s]->ناپ != def->linux->uid_mappings[s]->ناپ)
        أرجع false;
    }

  لكل (s = 0; s < mnt->gid_mappings_len; s++)
    {
      إذا (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        أرجع false;
      إذا (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        أرجع false;
      إذا (mnt->gid_mappings[s]->ناپ != def->linux->gid_mappings[s]->ناپ)
        أرجع false;
    }

  أرجع true;
}

ساكن pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        ثابت حرف *options, pid_t *pid_باہرلا,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != عدم;

  إذا (! need_new_userns)
    أرجع 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, عدم);
  إذا (UNLIKELY (pid < 0))
    أرجع crun_make_error (err, errno, "clone");

  إذا (pid == 0)
    {
      prctl (PR_سیٹ_PDEATHSIG, SIGKILL);
      طالما (1)
        pause ();
      محفوظ_exit (EXIT_SUCCESS);
    }

  إذا (mnt->uid_mappings_len)
    {
      cleanup_free حرف *uid_map = عدم;
      cleanup_free حرف *gid_map = عدم;
      size_t written = 0;
      صحيح ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      cleanup_close صحيح fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      إذا (UNLIKELY (fd < 0))
        أرجع fd;

      ret = محفوظ_write (fd, "uid_map", uid_map, written, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      إذا (UNLIKELY (fd < 0))
        أرجع fd;

      ret = محفوظ_write (fd, "gid_map", gid_map, written, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      cleanup_free حرف *dup_options = عدم;
      حرف *option, *saveptr = عدم;

      إذا (! options)
        أرجع crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      لكل (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (عدم, ";", &saveptr))
        {
          cleanup_free حرف *mappings = عدم;
          cleanup_close صحيح map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          صحيح ret;

          إذا (has_اگیتر (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              إذا (UNLIKELY (map_fd < 0))
                أرجع map_fd;
            }
          وإلا إذا (has_اگیتر (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              إذا (UNLIKELY (map_fd < 0))
                أرجع map_fd;
            }
          وإلا
            أرجع crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          ret = محفوظ_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  *pid_باہرلا = pid;
  pid = -1;
  أرجع 0;
}

صحيح
libcrun_create_keyring (libcrun_container_t *container, ثابت حرف *name, ثابت حرف *label, libcrun_error_t *err)
{
  cleanup_close صحيح labelfd = -1;
  bool label_سیٹ = false;
  صحيح ret;

  إذا (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      إذا (UNLIKELY (labelfd < 0))
        {
          إذا (crun_error_get_errno (err) != ENOENT)
            أرجع labelfd;

          crun_error_release (err);
        }

      إذا (labelfd >= 0)
        {
          ret = اكتب (labelfd, label, لڑی_لمبائی (label));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_سیٹ = true;
        }
    }

  ret = syscall_keyctl_join (name);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          اذهب باہرلا;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      اذهب باہرلا;
    }

باہرلا:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  إذا (label_سیٹ && اكتب (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  أرجع ret;
}

ساكن فراغ
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  إذا (def->process && def->process->user)
    {
      إذا (def->process->user->uid)
        *uid = def->process->user->uid;
      إذا (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

ساكن بنانشانی لما
get_mount_flags (ثابت حرف *name, صحيح current_flags, صحيح *لبھیا, بنانشانی لما *extra_flags, uint64_t *rec_clear, uint64_t *rec_سیٹ)
{
  ثابت بنية پسار_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  إذا (لبھیا)
    *لبھیا = prop ? 1 : 0;

  إذا (prop == عدم)
    أرجع 0;

  إذا (prop->extra_flags & OPTION_RECURSIVE)
    {
      إذا (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      إذا (rec_سیٹ && ! prop->clear)
        *rec_سیٹ |= prop->flags;
    }

  إذا (extra_flags)
    *extra_flags |= prop->extra_flags;

  إذا (prop->clear)
    أرجع current_flags & ~prop->flags;

  أرجع current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
ساكن bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  لكل (i = 0; i < mount->options_len; i++)
    {
      ثابت بنية پسار_flags_s *prop;

      إذا (mount->options[i] == عدم)
        استمر;

      prop = libcrun_str2mount_flags (mount->options[i]);
      إذا (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        أرجع true;
    }

  أرجع false;
}

ساكن بنانشانی لما
get_mount_flags_or_option (ثابت حرف *name, صحيح current_flags, بنانشانی لما *extra_flags, حرف **option, uint64_t *rec_clear, uint64_t *rec_سیٹ)
{
  صحيح لبھیا;
  __attribute__ ((بن_ورتیا)) cleanup_free حرف *prev = عدم;
  بنانشانی لما flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  إذا (name == عدم)
    أرجع 0;

  flags = get_mount_flags (name, current_flags, &لبھیا, extra_flags, rec_clear, rec_سیٹ);
  إذا (لبھیا)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      ثابت حرف *انکڑے_flag = عدم;
      إذا ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        انکڑے_flag = "sync";
      وإلا إذا ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        انکڑے_flag = "dirsync";

      إذا (انکڑے_flag)
        {
          prev = *option;
          إذا (*option && **option)
            xasprintf (option, "%s,%s", *option, انکڑے_flag);
          وإلا
            *option = xstrdup (انکڑے_flag);
        }
      أرجع flags;
    }

  prev = *option;
  إذا (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  وإلا
    *option = xstrdup (name);

  أرجع 0;
}

صحيح
دھرا_جڑ (ثابت حرف *new_جڑ, ثابت حرف *put_old)
{
  أرجع syscall (__NR_pivot_root, new_جڑ, put_old);
}

ساكن فراغ
free_remount (بنية remount_s *r)
{
  إذا (r == عدم)
    أرجع;
  إذا (r->targetfd >= 0)
    close (r->targetfd);
  حرر (r->انکڑے);
  حرر (r->نشانہ);
  حرر (r);
}

ساكن بنية remount_s *
make_remount (صحيح targetfd, ثابت حرف *نشانہ, بنانشانی لما flags, ثابت حرف *انکڑے, بنية remount_s *اگلا)
{
  بنية remount_s *ret = xmalloc (حجم (*ret));
  ret->نشانہ = xstrdup (نشانہ);
  ret->flags = flags;
  ret->انکڑے = انکڑے ? xstrdup (انکڑے) : عدم;
  ret->اگلا = اگلا;
  ret->targetfd = targetfd;
  أرجع ret;
}

ساكن صحيح
do_remount (صحيح targetfd, ثابت حرف *نشانہ, بنانشانی لما flags, ثابت حرف *انکڑے, libcrun_error_t *err)
{
  proc_fd_رستہ_t نشانہ_بفر;
  صحيح ret;
  ثابت حرف *real_نشانہ = نشانہ;

  إذا (targetfd >= 0)
    {
      بنانشانی لما سیٹ_flags = flags & ~(MS_REMOUNT | MS_BIND);
      بنانشانی لما clear_flags = REMOUNT_CLEAR_FLAGS & ~سیٹ_flags;
      ret = do_mount_setattr (false, نشانہ, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      إذا (LIKELY (ret == 0))
        أرجع 0;
      crun_error_release (err);

      get_proc_self_fd_رستہ (نشانہ_بفر, targetfd);
      real_نشانہ = نشانہ_بفر;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  إذا (flags & (MS_REMOUNT | MS_RDONLY))
    انکڑے = عدم;

  ret = mount (عدم, real_نشانہ, عدم, flags, انکڑے);
  إذا (UNLIKELY (ret < 0))
    {
      بنانشانی لما current_flags, remount_flags;
      بنية statfs sfs;

      ret = statfs (real_نشانہ, &sfs);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "statfs `%s`", real_نشانہ);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      إذا (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      إذا ((flags | remount_flags) != flags)
        {
          ret = mount (عدم, real_نشانہ, عدم, flags | remount_flags, انکڑے);
          إذا (LIKELY (ret == 0))
            أرجع 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          إذا (current_flags & MS_RDONLY)
            ret = mount (عدم, real_نشانہ, عدم, flags | remount_flags | MS_RDONLY, انکڑے);
        }
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "remount `%s`", نشانہ);
    }
  أرجع 0;
}

ساكن صحيح
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret = 0;
  بنية remount_s *r = get_نجی_انکڑے (container)->remounts;
  طالما (r)
    {
      بنية remount_s *اگلا = r->اگلا;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      إذا (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          بنانشانی لما سیٹ_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          بنانشانی لما clear_flags = REMOUNT_CLEAR_FLAGS & ~سیٹ_flags;

          ret = do_mount_setattr (false, r->نشانہ, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          إذا (LIKELY (ret == 0))
            {
              free_remount (r);
              r = اگلا;
              استمر;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->نشانہ, r->flags, r->انکڑے, err);
      إذا (UNLIKELY (ret < 0))
        اذهب cleanup;

      free_remount (r);
      r = اگلا;
    }

cleanup:
  طالما (r)
    {
      بنية remount_s *اگلا = r->اگلا;
      free_remount (r);
      r = اگلا;
    }

  get_نجی_انکڑے (container)->remounts = عدم;
  أرجع ret;
}

ساكن صحيح
open_mount_نشانہ (libcrun_container_t *container, ثابت حرف *نشانہ_rel, libcrun_error_t *err)
{
  ثابت حرف *rootfs = get_نجی_انکڑے (container)->rootfs;
  صحيح rootfsfd = get_نجی_انکڑے (container)->rootfsfd;

  إذا (rootfsfd < 0)
    أرجع crun_make_error (err, 0, "invalid rootfs state");

  أرجع محفوظ_openat (rootfsfd, rootfs, نشانہ_rel, O_رستہ | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
ساكن صحيح
fsopen_mount (ثابت حرف *type, ثابت حرف *سروت_name, ثابت حرف *labeltype, ثابت حرف *label, ثابت حرف *انکڑے)
{
#جے_تعریف_شدہ HAVE_NEW_MOUNT_API
  cleanup_close صحيح fsfd = -1;
  صحيح ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  إذا (UNLIKELY (fsfd < 0))
    أرجع fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (فراغ) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", سروت_name ? سروت_name : type, 0);

  إذا (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (انکڑے && انکڑے[0])
    {
      cleanup_free حرف *انکڑے_copy = xstrdup (انکڑے);
      حرف *saveptr = عدم;
      حرف *token;

      لكل (token = strtok_r (انکڑے_copy, ",", &saveptr); token; token = strtok_r (عدم, ",", &saveptr))
        {
          حرف *eq = strchr (token, '=');
          إذا (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          وإلا
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, عدم, 0);

          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, عدم, عدم, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#وإلا
  (فراغ) type;
  (فراغ) سروت_name;
  (فراغ) انکڑے;
  errno = ENOSYS;
  أرجع -1;
#جے_انت
}

ساكن صحيح
get_procfd (بنية نجی_انکڑے_s *انکڑے, libcrun_error_t *err)
{
  صحيح fd;

  إذا (انکڑے->procfd >= 0)
    أرجع انکڑے->procfd;

  fd = fsopen_mount ("proc", عدم, عدم, عدم, عدم);
  إذا (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `/proc`");

  انکڑے->procfd = fd;
  أرجع fd;
}

ساكن صحيح
get_old_جڑ_fd (بنية نجی_انکڑے_s *انکڑے)
{
  إذا (انکڑے->old_جڑ_fd >= 0)
    أرجع انکڑے->old_جڑ_fd;

  انکڑے->old_جڑ_fd = open ("/", O_رستہ | O_CLOEXEC);
  أرجع انکڑے->old_جڑ_fd;
}

ساكن صحيح
fs_move_mount_منزل (صحيح fd, صحيح dirfd, ثابت حرف *name)
{
#جے_تعریف_شدہ HAVE_NEW_MOUNT_API
  إذا (name)
    أرجع syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  أرجع syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#وإلا
  (فراغ) fd;
  (فراغ) dirfd;
  (فراغ) name;
  (فراغ) syscall_move_mount;
  errno = ENOSYS;
  أرجع -1;
#جے_انت
}

تعداد
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

ساكن صحيح do_mount (libcrun_container_t *container, ثابت حرف *سروت, صحيح targetfd,
                     ثابت حرف *نشانہ, ثابت حرف *fstype, بنانشانی لما mountflags,
                     ثابت فراغ *انکڑے, صحيح extra_flags, libcrun_error_t *err);

ساكن bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, ثابت حرف *option)
{
  size_t i;
  لكل (i = 0; i < mnt->options_len; i++)
    إذا (mnt->options[i] != عدم && strcmp (mnt->options[i], option) == 0)
      أرجع true;
  أرجع false;
}

ساكن runtime_spec_schema_defs_mount *
لبھو_mount_for (libcrun_container_t *container, ثابت حرف *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  لكل (i = 0; i < def->mounts_len; i++)
    {
      إذا (strcmp (def->mounts[i]->destination, destination) == 0)
        أرجع def->mounts[i];
    }
  أرجع عدم;
}

ساكن فراغ
warn_tmpfs_fallback_once (بنية نجی_انکڑے_s *نجی_انکڑے, ثابت حرف *reason)
{
  إذا (! نجی_انکڑے->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      نجی_انکڑے->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
ساكن صحيح
get_shared_خالی_تھاواں_dir_cached (libcrun_container_t *container, حرف **proc_fd_رستہ, libcrun_error_t *err)
{
  بنية نجی_انکڑے_s *نجی_انکڑے = get_نجی_انکڑے (container);
  cleanup_close صحيح fd = -1;
  cleanup_free حرف *خالی_تھاواں_dir_رستہ = عدم;
  proc_fd_رستہ_t fd_رستہ;
  صحيح ret;

  /* Fast path: return cached proc fd path if already set up */
  إذا (نجی_انکڑے->maskdir_proc_رستہ != عدم)
    {
      إذا (proc_fd_رستہ)
        *proc_fd_رستہ = نجی_انکڑے->maskdir_proc_رستہ;
      أرجع 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_خالی_تھاواں_directory_رستہ (&خالی_تھاواں_dir_رستہ, container->context->state_جڑ, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, خالی_تھاواں_dir_رستہ, true, true, false, MS_PRIVATE, err);
  إذا (fd < 0)
    أرجع fd;

  get_self_fd_رستہ (fd_رستہ, fd);
  نجی_انکڑے->maskdir_proc_رستہ = xstrdup (fd_رستہ);

  نجی_انکڑے->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  إذا (proc_fd_رستہ)
    *proc_fd_رستہ = نجی_انکڑے->maskdir_proc_رستہ;
  أرجع 0;
}

ساكن bool
is_dev_null (ثابت بنية stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  أرجع S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
ساكن صحيح
ensure_dev_null (صحيح fd, libcrun_error_t *err)
{
  بنية stat st;

  إذا (UNLIKELY (fstat (fd, &st) < 0))
    أرجع crun_make_error (err, errno, "stat `/dev/null`");

  إذا (UNLIKELY (! is_dev_null (&st)))
    أرجع crun_make_error (err, 0, "`/dev/null` is not the null device");

  أرجع 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
ساكن bool
is_خالی_تھاواں_ro_tmpfs (صحيح fd)
{
  بنية statfs sfs;

  إذا (fstatfs (fd, &sfs) < 0)
    أرجع false;

  أرجع sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

ساكن صحيح
mount_masked_dir (libcrun_container_t *container, صحيح pathfd, ثابت حرف *rel_رستہ, libcrun_error_t *err)
{
  بنية نجی_انکڑے_s *نجی_انکڑے = get_نجی_انکڑے (container);
  cleanup_close صحيح newfd = -1;
  libcrun_error_t tmp_err = عدم;
  صحيح ret;

  إذا (نجی_انکڑے->maskdir_fd < 0)
    {
      إذا (نجی_انکڑے->maskdir_bind_failed)
        اذهب fallback_منزل_tmpfs;

      ret = get_shared_خالی_تھاواں_dir_cached (container, عدم, &tmp_err);
      إذا (ret < 0)
        {
          نجی_انکڑے->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (نجی_انکڑے, tmp_err->پیغام);
          crun_error_release (&tmp_err);
          اذهب fallback_منزل_tmpfs;
        }
    }

  إذا (نجی_انکڑے->maskdir_fd >= 0)
    {
      صحيح rootfsfd = نجی_انکڑے->rootfsfd;
      بنية stat before, after;

      ret = fstat (نجی_انکڑے->maskdir_fd, &before);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "fstat masked dir `%s`", rel_رستہ);

      ret = fs_move_mount_منزل (نجی_انکڑے->maskdir_fd, pathfd, عدم);
      إذا (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_رستہ, true, true, false, MS_PRIVATE, err);
          إذا (UNLIKELY (newfd < 0))
            أرجع newfd;

          ret = fstat (newfd, &after);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "fstat masked dir `%s`", rel_رستہ);

          إذا (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            أرجع crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_رستہ);

          عارضی_FAILURE_RETRY (close (نجی_انکڑے->maskdir_fd));
          نجی_انکڑے->maskdir_fd = newfd;
          newfd = -1;

          أرجع 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_رستہ);

      عارضی_FAILURE_RETRY (close (نجی_انکڑے->maskdir_fd));
      نجی_انکڑے->maskdir_fd = -1;
      نجی_انکڑے->maskdir_move_failed = true;
    }

  نجی_انکڑے->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (نجی_انکڑے, tmp_err ? tmp_err->پیغام : "unknown error");
  crun_error_release (&tmp_err);

fallback_منزل_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_رستہ);
  ret = do_mount (container, "tmpfs", pathfd, rel_رستہ, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  إذا (! نجی_انکڑے->maskdir_move_failed)
    {
      ret = get_bind_mount (نجی_انکڑے->rootfsfd, rel_رستہ, true, true, false, MS_PRIVATE, &tmp_err);
      إذا (ret >= 0 && is_خالی_تھاواں_ro_tmpfs (ret))
        نجی_انکڑے->maskdir_fd = ret;
      وإلا
        {
          إذا (ret >= 0)
            عارضی_FAILURE_RETRY (close (ret));
          نجی_انکڑے->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  أرجع 0;
}

ساكن صحيح
do_masked_or_readonly_رستہ (libcrun_container_t *container, ثابت حرف *rel_رستہ, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  بنانشانی لما mount_flags = 0;
  ثابت حرف *rootfs = get_نجی_انکڑے (container)->rootfs;
  cleanup_close صحيح pathfd = -1;
  بنية statfs sfs;
  صحيح ret;
  mode_t منوال;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  إذا (UNLIKELY (rel_رستہ == عدم))
    أرجع crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  إذا (rel_رستہ[0] == '/')
    rel_رستہ++;

  pathfd = محفوظ_openat (get_نجی_انکڑے (container)->rootfsfd, rootfs, rel_رستہ, O_رستہ | O_CLOEXEC, 0, err);
  إذا (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      إذا (errno != ENOENT && errno != EACCES)
        أرجع pathfd;

      إذا (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_رستہ);
      crun_error_release (err);
      أرجع 0;
    }

  إذا (readonly)
    {
      صحيح procfd = get_procfd (get_نجی_انکڑے (container), err);
      proc_fd_رستہ_t سروت_بفر;
      cleanup_close صحيح mountfd = -1;

      إذا (UNLIKELY (procfd < 0))
        أرجع procfd;

      get_self_fd_رستہ (سروت_بفر, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      إذا (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, سروت_بفر, true, true, false, MS_PRIVATE, err);
          إذا (mountfd >= 0)
            ret = fs_move_mount_منزل (mountfd, pathfd, عدم);
          وإلا
            ret = -1;

          إذا (ret < 0)
            crun_error_release (err);
        }

      إذا (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          إذا (keep_flags)
            {
              cleanup_close صحيح sfd = openat (procfd, سروت_بفر, O_رستہ | O_CLOEXEC);
              إذا (UNLIKELY (sfd < 0))
                أرجع crun_make_error (err, errno, "openat procfd for `%s`", rel_رستہ);

              ret = fstatfs (sfd, &sfs);
              إذا (UNLIKELY (ret < 0))
                أرجع crun_make_error (err, errno, "fstatfs `%s`", rel_رستہ);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, سروت_بفر, true, true, false, MS_PRIVATE, err);
          إذا (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_رستہ, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;

              ret = fs_move_mount_منزل (mountfd, pathfd, عدم);
              إذا (UNLIKELY (ret < 0))
                أرجع crun_make_error (err, errno, "move mount for readonly path `%s`", rel_رستہ);
            }
          وإلا
            {
              proc_fd_رستہ_t abs_سروت;

              crun_error_release (err);
              get_proc_self_fd_رستہ (abs_سروت, pathfd);
              ret = do_mount (container, abs_سروت, pathfd, rel_رستہ, عدم, mount_flags, عدم,
                              LABEL_NONE, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;
            }
        }
    }
  وإلا
    {
      ret = get_file_type_fd (pathfd, &منوال);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "cannot stat `%s`", rel_رستہ);

      إذا ((منوال & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_رستہ, err);
      وإلا
        {
          cleanup_close صحيح nullfd = -1;
          cleanup_close صحيح mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_رستہ | O_CLOEXEC);
          إذا (UNLIKELY (nullfd < 0))
            أرجع crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          إذا (mountfd >= 0)
            ret = fs_move_mount_منزل (mountfd, pathfd, عدم);

          إذا (mountfd < 0 || ret < 0)
            {
              proc_fd_رستہ_t null_رستہ;

              crun_error_release (err);
              get_proc_self_fd_رستہ (null_رستہ, nullfd);
              ret = do_mount (container, null_رستہ, pathfd, rel_رستہ, عدم, MS_BIND | MS_RDONLY, عدم,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن ثابت حرف *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  ثابت حرف *context_type;

  context_type = لبھو_annotation (container, "run.oci.mount_context_type");
  إذا (context_type)
    {
      إذا (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          أرجع عدم;
        }
      أرجع context_type;
    }

  أرجع "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
ساكن صحيح
diagnose_mount_failure (صحيح ret, libcrun_error_t *err, libcrun_container_t *container,
                        ثابت حرف *fstype)
{
  إذا (fstype && strcmp (fstype, "cgroup2") == 0 && get_نجی_انکڑے (container)->unified_cgroup_رستہ)
    {
      إذا (access (get_نجی_انکڑے (container)->unified_cgroup_رستہ, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          أرجع ret;
        }
    }
  أرجع ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
ساكن صحيح
sysfs_userns_fallback (libcrun_container_t *container, ثابت حرف *نشانہ,
                       ثابت حرف *real_نشانہ, صحيح targetfd,
                       بنانشانی لما mountflags, ثابت حرف *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close صحيح mountfd = -1;
  cleanup_close صحيح sysfd = -1;
  صحيح sys_old_جڑ_fd = -1;
  صحيح ret;

  إذا (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    أرجع 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_وچ_user_namespace (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret == 0)
    أرجع 1;

  cgroup_mount = لبھو_mount_for (container, "/sys/fs/cgroup");
  sys_old_جڑ_fd = get_old_جڑ_fd (get_نجی_انکڑے (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  إذا (sys_old_جڑ_fd >= 0)
    mountfd = get_bind_mount (sys_old_جڑ_fd, "sys", true, true, false, MS_PRIVATE, err);
  إذا (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  إذا (cgroup_mount == عدم)
    {
      إذا (mountfd >= 0)
        {
          ret = fs_move_mount_منزل (mountfd, targetfd, عدم);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "move mount `/sys` to `%s`", real_نشانہ);
        }
      وإلا
        {
          crun_error_release (err);
          إذا (sys_old_جڑ_fd >= 0)
            {
              cleanup_free حرف *sys_رستہ = عدم;
              proc_fd_رستہ_t sys_جڑ_رستہ;

              get_proc_self_fd_رستہ (sys_جڑ_رستہ, sys_old_جڑ_fd);
              xasprintf (&sys_رستہ, "%s/sys", sys_جڑ_رستہ);
              ret = mount (sys_رستہ, real_نشانہ, عدم, MS_BIND | MS_REC, عدم);
            }
          وإلا
            ret = mount ("/sys", real_نشانہ, عدم, MS_BIND | MS_REC, عدم);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_نشانہ (container, نشانہ, err);
      إذا (UNLIKELY (sysfd < 0))
        أرجع sysfd;

      ret = do_remount (sysfd, نشانہ,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        عدم, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      أرجع do_masked_or_readonly_رستہ (container, "/sys/fs/cgroup", true, false, err);
    }

  إذا (UNLIKELY (mountfd < 0))
    أرجع mountfd;

  ret = fs_move_mount_منزل (mountfd, targetfd, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "move mount to `%s`", real_نشانہ);

  sysfd = open_mount_نشانہ (container, نشانہ, err);
  إذا (UNLIKELY (sysfd < 0))
    أرجع sysfd;

  ret = do_remount (sysfd, نشانہ,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_رستہ (container, "/sys/fs/cgroup", true, false, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

#جے_تعریف_شدہ HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
ساكن فراغ
apply_selinux_xattr (libcrun_container_t *container, صحيح fd, صحيح extra_flags, ثابت حرف *label)
{
  libcrun_error_t tmp_err = عدم;
  صحيح procfd;

  إذا ((extra_flags & LABEL_نقاب) != LABEL_XATTR)
    أرجع;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_نجی_انکڑے (container), &tmp_err);
  إذا (procfd >= 0)
    {
      proc_fd_رستہ_t proc_self_رستہ;
      cleanup_close صحيح xfd = -1;

      get_self_fd_رستہ (proc_self_رستہ, fd);
      xfd = openat (procfd, proc_self_رستہ, O_RDONLY | O_CLOEXEC);
      إذا (xfd >= 0)
        (فراغ) fsetxattr (xfd, "security.selinux", label, لڑی_لمبائی (label), 0);
    }
  crun_error_release (&tmp_err);
}
#جے_انت

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
ساكن صحيح
do_mount_replace_rootfs (libcrun_container_t *container, صحيح *fd, libcrun_error_t *err)
{
  صحيح procfd = get_procfd (get_نجی_انکڑے (container), err);
  صحيح tmp;
  صحيح ret;

  إذا (UNLIKELY (procfd < 0))
    أرجع procfd;

  {
    cleanup_close صحيح nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    إذا (UNLIKELY (nsfd < 0))
      أرجع crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    إذا (UNLIKELY (ret < 0))
      أرجع crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_نجی_انکڑے (container)->rootfs, O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (*fd < 0))
    أرجع crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  إذا (UNLIKELY (tmp < 0))
    أرجع crun_make_error (err, errno, "dup");

  عارضی_FAILURE_RETRY (close (get_نجی_انکڑے (container)->rootfsfd));
  get_نجی_انکڑے (container)->rootfsfd = tmp;

  أرجع 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
ساكن صحيح
apply_پسار_flags (ثابت حرف *نشانہ, صحيح targetfd, ثابت حرف *real_نشانہ,
                         بنانشانی لما mountflags, libcrun_error_t *err)
{
  bool پسار_پورا = false;
  صحيح ret;

  إذا (targetfd >= 0)
    {
      libcrun_error_t tmp_err = عدم;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, نشانہ, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      إذا (LIKELY (ret == 0))
        پسار_پورا = true;
      وإلا
        crun_error_release (&tmp_err);
    }

  إذا (! پسار_پورا)
    {
      ret = mount (عدم, real_نشانہ, عدم, mountflags & ALL_PROPAGATIONS, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "set propagation for `%s`", نشانہ);
    }

  أرجع 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
ساكن صحيح
ویلا_سارنی_or_do_remount (libcrun_container_t *container, صحيح *fd, صحيح targetfd,
                        ثابت حرف *نشانہ, ثابت حرف *real_نشانہ,
                        bool single_instance, بنانشانی لما mountflags,
                        ثابت فراغ *انکڑے, صحيح extra_flags, libcrun_error_t *err)
{
  بنانشانی لما remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  بنية remount_s *r;

  إذا ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    أرجع do_remount (*fd >= 0 ? *fd : targetfd, real_نشانہ, remount_flags, انکڑے, err);

  إذا (*fd < 0)
    {
      *fd = dup (targetfd);
      إذا (UNLIKELY (*fd < 0))
        أرجع crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), نشانہ, remount_flags, انکڑے, get_نجی_انکڑے (container)->remounts);
  get_نجی_انکڑے (container)->remounts = r;

  أرجع 0;
}

ساكن صحيح
do_mount (libcrun_container_t *container, ثابت حرف *سروت, صحيح targetfd,
          ثابت حرف *نشانہ, ثابت حرف *fstype, بنانشانی لما mountflags, ثابت فراغ *انکڑے,
          صحيح extra_flags, libcrun_error_t *err)
{
  cleanup_free حرف *انکڑے_with_label = عدم;
  ثابت حرف *انکڑے_without_label = انکڑے;
  proc_fd_رستہ_t نشانہ_بفر;
  ثابت حرف *context_type = عدم;
  cleanup_close صحيح ms_move_fd = -1;
  ثابت حرف *real_نشانہ = نشانہ;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close صحيح fd = -1;
  ثابت حرف *label = عدم;
  صحيح ret = 0;

  إذا (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  وإلا
    extra_flags = (extra_flags & ~LABEL_نقاب) | LABEL_NONE;

  إذا (targetfd >= 0)
    {
      get_proc_self_fd_رستہ (نشانہ_بفر, targetfd);
      real_نشانہ = نشانہ_بفر;
      needs_remount = true;
    }

  إذا ((extra_flags & LABEL_نقاب) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      إذا (UNLIKELY (context_type == عدم))
        أرجع -1;

      ret = جوڑو_selinux_mount_label (&انکڑے_with_label, انکڑے, label, context_type, err);
      إذا (ret < 0)
        أرجع ret;
      انکڑے = انکڑے_with_label;
    }

  إذا (mountflags & MS_MOVE)
    {
      إذا ((mountflags & MS_BIND) || fstype)
        أرجع crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (سروت, real_نشانہ, عدم, MS_MOVE, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "move mount `%s` to `%s`", سروت, نشانہ);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_نشانہ (container, نشانہ, err);
      إذا (UNLIKELY (ms_move_fd < 0))
        أرجع ms_move_fd;
      targetfd = ms_move_fd;
    }

  إذا ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      بنانشانی لما flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      إذا (targetfd >= 0)
        {
          إذا (mountflags & MS_BIND)
            {
              cleanup_close صحيح bindfd = -1;
              libcrun_error_t tmp_err = عدم;

              bindfd = get_bind_mount (AT_FDCWD, سروت,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              إذا (bindfd < 0)
                {
                  صحيح old_جڑ_fd = get_old_جڑ_fd (get_نجی_انکڑے (container));

                  إذا (old_جڑ_fd >= 0 && سروت && سروت[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_جڑ_fd, (سروت[1] == '\0') ? "." : سروت + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              إذا (bindfd >= 0)
                ret = fs_move_mount_منزل (bindfd, targetfd, عدم);
              إذا (ret < 0)
                crun_error_release (&tmp_err);
            }
          وإلا
            {
              cleanup_close صحيح newfs = fsopen_mount (fstype, سروت, context_type, label, انکڑے_without_label);
              إذا (newfs >= 0)
                ret = fs_move_mount_منزل (newfs, targetfd, عدم);
            }
        }

      إذا (ret < 0)
        {
          ret = mount (سروت, real_نشانہ, fstype, flags, انکڑے);
          إذا (ret < 0 && (mountflags & MS_BIND) && سروت && سروت[0] == '/')
            {
              صحيح old_جڑ_fd = get_old_جڑ_fd (get_نجی_انکڑے (container));

              إذا (old_جڑ_fd >= 0)
                {
                  cleanup_free حرف *proc_سروت_buf = عدم;
                  proc_fd_رستہ_t old_جڑ_رستہ;

                  get_proc_self_fd_رستہ (old_جڑ_رستہ, old_جڑ_fd);
                  xasprintf (&proc_سروت_buf, "%s/%s", old_جڑ_رستہ, سروت + 1);
                  ret = mount (proc_سروت_buf, real_نشانہ, fstype, flags, انکڑے);
                }
            }
        }
      إذا (UNLIKELY (ret < 0))
        {
          صحيح saved_errno = errno;

          ret = sysfs_userns_fallback (container, نشانہ, real_نشانہ, targetfd,
                                       mountflags, fstype, err);
          إذا (ret == 0)
            أرجع 0;
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", سروت, نشانہ);

          أرجع diagnose_mount_failure (ret, err, container, fstype);
        }

      إذا (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_نشانہ (container, نشانہ, err);
          إذا (UNLIKELY (fd < 0))
            أرجع fd;

          /* We are replacing the rootfs, reopen it.  */
          إذا (is_خالی_تھاواں_string (نشانہ))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;
            }

#جے_تعریف_شدہ HAVE_FGETXATTR
          apply_selinux_xattr (container, fd, extra_flags, label);
#جے_انت

          targetfd = fd;

          /* The bind mount shadowed the original targetfd, so refresh
             real_target to point to the reopened mountpoint.  Otherwise the
             classic mount(2) fallbacks below (used when mount_setattr is not
             available, e.g. kernels < 5.12) operate on the shadowed path and
             fail with EINVAL.  */
          get_proc_self_fd_رستہ (نشانہ_بفر, targetfd);
          real_نشانہ = نشانہ_بفر;
        }
    }

  إذا (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_پسار_flags (نشانہ, targetfd, real_نشانہ, mountflags, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  إذا (انکڑے && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  إذا (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = ویلا_سارنی_or_do_remount (container, &fd, targetfd, نشانہ, real_نشانہ,
                                    single_instance, mountflags, انکڑے, extra_flags, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع ret;
}

ساكن فراغ
try_umount (صحيح targetfd, ثابت حرف *نشانہ)
{
  proc_fd_رستہ_t نشانہ_بفر;
  ثابت حرف *real_نشانہ = نشانہ;

  إذا (targetfd >= 0)
    {
      get_proc_self_fd_رستہ (نشانہ_بفر, targetfd);
      real_نشانہ = نشانہ_بفر;
    }

  umount2 (real_نشانہ, MNT_DETACH);
}

ساكن bool
container_has_cgroupns (libcrun_container_t *container)
{
  أرجع get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWCGROUP;
}

ساكن صحيح
do_mount_cgroup_v2 (libcrun_container_t *container, صحيح targetfd, ثابت حرف *نشانہ,
                    بنانشانی لما mountflags, ثابت حرف *انکڑے, libcrun_error_t *err)
{
  صحيح ret;
  صحيح cgroup_منوال;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  إذا (UNLIKELY (cgroup_منوال < 0))
    أرجع cgroup_منوال;

  ret = do_mount (container, "cgroup2", targetfd, نشانہ, "cgroup2", mountflags, انکڑے, LABEL_NONE, err);
  إذا (UNLIKELY (ret < 0))
    {
      صحيح saved_errno = crun_error_get_errno (err);
      إذا (saved_errno == EPERM || saved_errno == EBUSY)
        {
          ثابت حرف *unified_cgroup_رستہ;
          ثابت حرف *src_cgroup;

          crun_error_release (err);

          إذا (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, نشانہ, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              إذا (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, نشانہ, "cgroup2", mountflags, انکڑے, LABEL_NONE, err);
                  إذا (LIKELY (ret == 0))
                    أرجع ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, نشانہ);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_رستہ = get_نجی_انکڑے (container)->unified_cgroup_رستہ;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_رستہ && container_has_cgroupns (container) ? unified_cgroup_رستہ : CGROUP_جڑ;
          أرجع do_mount (container, src_cgroup, targetfd, نشانہ, عدم,
                           MS_BIND | mountflags, عدم, LABEL_NONE, err);
        }

      أرجع ret;
    }

  أرجع 0;
}

ساكن صحيح
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, ثابت حرف *سروت, صحيح targetfd, ثابت حرف *نشانہ,
                            بنانشانی لما mountflags, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_close صحيح fd = -1;
  ثابت حرف *subsystem = "systemd";
  cleanup_free حرف *subsystem_رستہ = عدم;
  cleanup_close صحيح tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, سروت, targetfd, نشانہ, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_نشانہ (container, نشانہ, err);
  إذا (UNLIKELY (tmpfsdirfd < 0))
    أرجع tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_رستہ, err, نشانہ, subsystem, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_رستہ | O_DIRECTORY | O_NOFOLLOW);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "openat `%s`", subsystem_رستہ);

  أرجع do_mount (container, "cgroup", fd, subsystem_رستہ, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

ساكن صحيح
do_mount_cgroup_v1 (libcrun_container_t *container, ثابت حرف *سروت, صحيح targetfd, ثابت حرف *نشانہ,
                    بنانشانی لما mountflags, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *content = عدم;
  حرف *from;
  cleanup_close صحيح tmpfsdirfd = -1;
  حرف *saveptr = عدم;

  ret = do_mount (container, سروت, targetfd, نشانہ, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  tmpfsdirfd = open_mount_نشانہ (container, نشانہ, err);
  إذا (UNLIKELY (tmpfsdirfd < 0))
    أرجع tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    صحيح procfd = get_procfd (get_نجی_انکڑے (container), err);
    إذا (UNLIKELY (procfd < 0))
      أرجع procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, عدم, err);
  }
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (UNLIKELY (content == عدم || content[0] == '\0'))
    أرجع crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  لكل (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (عدم, "\n", &saveptr))
    {
      cleanup_free حرف *سروت_رستہ = عدم;
      cleanup_free حرف *سروت_subsystem = عدم;
      cleanup_free حرف *subsystem_رستہ = عدم;
      حرف *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close صحيح subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      إذا (subsystem[0] == '\0')
        استمر;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = لڑی_کھوج (subsystem, "name=");
      إذا (it)
        subsystem = it + 5;

      إذا (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      إذا (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&سروت_subsystem, err, CGROUP_جڑ, subsystem, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      /* if there is already a mount specified, do not add a default one.  */
      إذا (لبھو_mount_for (container, سروت_subsystem) != عدم)
        استمر;

      ret = append_paths (&سروت_رستہ, err, سروت_subsystem, subpath, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = append_paths (&subsystem_رستہ, err, نشانہ, subsystem, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "mkdirat `%s`", subsystem_رستہ);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_رستہ | O_DIRECTORY | O_NOFOLLOW);
      إذا (UNLIKELY (subsystemfd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", subsystem_رستہ);

      إذا (container_has_cgroupns (container))
        {
          ret = do_mount (container, سروت_رستہ, subsystemfd, subsystem_رستہ, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          إذا (UNLIKELY (ret < 0))
            {
              إذا (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  استمر;
                }
              أرجع ret;
            }
        }
      وإلا
        {
          ret = do_mount (container, سروت_رستہ, subsystemfd, subsystem_رستہ, عدم, MS_BIND | mountflags, عدم,
                          LABEL_NONE, err);
          إذا (UNLIKELY (ret < 0))
            {
              إذا (crun_error_get_errno (err) != ENOENT)
                أرجع ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, سروت_subsystem, subsystemfd, subsystem_رستہ, عدم, MS_BIND | mountflags,
                              عدم, LABEL_NONE, err);
              إذا (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  إذا (crun_error_get_errno (err) != ENOENT)
                    أرجع ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

ساكن صحيح
do_mount_cgroup (libcrun_container_t *container, ثابت حرف *سروت, صحيح targetfd, ثابت حرف *نشانہ,
                 بنانشانی لما mountflags, ثابت حرف *انکڑے, libcrun_error_t *err)
{
  صحيح cgroup_منوال;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  إذا (UNLIKELY (cgroup_منوال < 0))
    أرجع cgroup_منوال;

  بدل (cgroup_منوال)
    {
    حالة CGROUP_منوال_UNIFIED:
      أرجع do_mount_cgroup_v2 (container, targetfd, نشانہ, mountflags, انکڑے, err);
    حالة CGROUP_منوال_LEGACY:
    حالة CGROUP_منوال_HYBRID:
      أرجع do_mount_cgroup_v1 (container, سروت, targetfd, نشانہ, mountflags, err);
    }

  أرجع crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_منوال);
}

بنية device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
ساكن فراغ
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  بنية نجی_انکڑے_s *نجی_انکڑے = get_نجی_انکڑے (container);
  size_t i;

  إذا (نجی_انکڑے->needed_devs_fds)
    {
      بنية libcrun_fd_map *dev_fds = نجی_انکڑے->needed_devs_fds;

      لكل (i = 0; needed_devs[i].رستہ; i++)
        {
          إذا (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            استمر;

          صحيح fd = syscall_open_درخت (AT_FDCWD, needed_devs[i].رستہ,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          إذا (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          وإلا إذا (fd >= 0)
            close (fd);
        }
    }

  إذا (نجی_انکڑے->notify_socket_درخت_fd < 0
      && نجی_انکڑے->host_notify_socket_رستہ)
    {
      صحيح fd = syscall_open_درخت (AT_FDCWD, نجی_انکڑے->host_notify_socket_رستہ,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      إذا (fd >= 0)
        نجی_انکڑے->notify_socket_درخت_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
ساكن ثابت حرف *
relative_رستہ_under_dev (ثابت حرف *رستہ)
{
  إذا (رستہ[0] != '/')
    أرجع رستہ;
  إذا (لڑی_مقابلہ (رستہ, "/dev/", 5) == 0)
    {
      إذا (strchr (رستہ + 5, '/'))
        أرجع عدم;
      أرجع رستہ + 5;
    }
  أرجع عدم;
}

ساكن صحيح
mknod_and_سیٹ_attrs (libcrun_container_t *container, صحيح dirfd, ثابت حرف *name, بنية device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  proc_fd_رستہ_t fd_رستہ;
  ثابت حرف *what = "new";
  bool exists = false;
  بنية stat st;
  صحيح procfd;
  صحيح ret;

  ret = mknodat (dirfd, name, device->منوال | type, dev);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno != EEXIST)
        أرجع crun_make_error (err, errno, "mknodat `%s`", device->رستہ);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_رستہ | O_NOFOLLOW | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open %s device `%s`", what, device->رستہ);

  ret = fstat (fd, &st);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fstat %s device `%s`", what, device->رستہ);

  إذا (UNLIKELY ((st.st_mode & S_IFMT) != type))
    أرجع crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->رستہ, st.st_mode & S_IFMT, type);

  إذا (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    أرجع crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->رستہ, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  إذا (exists)
    أرجع 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_نجی_انکڑے (container), err);
  إذا (UNLIKELY (procfd < 0))
    أرجع procfd;

  get_self_fd_رستہ (fd_رستہ, fd);
  ret = fchmodat (procfd, fd_رستہ, device->منوال, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fchmod `%s`", device->رستہ);

  ret = fchownat (fd, "", device->uid, device->gid, AT_خالی_تھاواں_رستہ);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fchown `%s`", device->رستہ);

  أرجع 1;
}

صحيح
libcrun_create_dev (libcrun_container_t *container, صحيح devfd, صحيح srcfd,
                    بنية device_s *device, bool binds, bool ensure_ماپا_dir,
                    libcrun_error_t *err)
{
  صحيح ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  ثابت حرف *fullname = device->رستہ;
  ثابت حرف *rootfs = get_نجی_انکڑے (container)->rootfs;
  إذا (is_خالی_تھاواں_string (fullname))
    أرجع crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free حرف *normalized_رستہ = xstrdup (fullname);
  consume_trailing_slashes (normalized_رستہ);
  إذا (normalized_رستہ[0] == '\0')
    strcpy (normalized_رستہ, "/");
  ثابت حرف *rel_dev = relative_رستہ_under_dev (normalized_رستہ);

  إذا (binds)
    {
      cleanup_close صحيح fd = -1;

      إذا (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_رستہ | O_NONBLOCK);
          إذا (UNLIKELY (fd < 0))
            {
              إذا (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              إذا (UNLIKELY (fd < 0))
                أرجع crun_make_error (err, errno, "openat `%s`", device->رستہ);
            }
        }
      وإلا
        {
          ثابت حرف *rel_رستہ = consume_slashes (normalized_رستہ);

          fd = crun_محفوظ_create_and_open_ref_at (false, get_نجی_انکڑے (container)->rootfsfd, rootfs, rel_رستہ, 0755, err);
          إذا (UNLIKELY (fd < 0))
            أرجع fd;
        }

      إذا (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_رستہ, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          إذا (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              إذا (LIKELY (ret >= 0))
                أرجع 0;
            }
          وإلا
            crun_error_release (err);
        }

      {
        cleanup_close صحيح mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        إذا (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_رستہ, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            إذا (LIKELY (ret == 0))
              {
                ret = fs_move_mount_منزل (mountfd, fd, عدم);
                إذا (LIKELY (ret == 0))
                  أرجع 0;
              }
            وإلا
              crun_error_release (err);
          }
        وإلا
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_رستہ, عدم, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      إذا (rel_dev)
        {
          ret = mknod_and_سیٹ_attrs (container, devfd, rel_dev, device, type, dev, err);
          إذا (ret <= 0)
            أرجع ret;
        }
      وإلا
        {
          cleanup_close صحيح dirfd = -1;
          cleanup_free حرف *dirname = عدم;
          حرف *basename, *لبھیا;

          dirname = xstrdup (normalized_رستہ);

          لبھیا = strrchr (dirname, '/');
          إذا (لبھیا)
            *لبھیا = '\0';

          basename = لبھیا ? لبھیا + 1 : dirname;

          إذا (dirname[0] == '\0')
            {
              dirfd = dup (get_نجی_انکڑے (container)->rootfsfd);
              إذا (UNLIKELY (dirfd < 0))
                أرجع crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          وإلا
            {
              dirfd = محفوظ_openat (get_نجی_انکڑے (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_رستہ | O_CLOEXEC, 0, err);
              إذا (dirfd < 0 && ensure_ماپا_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_محفوظ_create_and_open_ref_at (true, get_نجی_انکڑے (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              إذا (UNLIKELY (dirfd < 0))
                أرجع dirfd;
            }

          ret = mknod_and_سیٹ_attrs (container, dirfd, basename, device, type, dev, err);
          إذا (ret <= 0)
            أرجع ret;
        }
    }
  أرجع 0;
}

بنية symlink_s
{
  ثابت حرف *رستہ;
  ثابت حرف *نشانہ;
  bool force;
};

ساكن بنية symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { عدم, عدم, false } };

ساكن صحيح
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  صحيح ret;
  size_t i;
  بنية device_s *it;
  cleanup_close صحيح devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت حرف *rootfs = get_نجی_انکڑے (container)->rootfs;
  cleanup_close_map بنية libcrun_fd_map *dev_fds = عدم;
  cleanup_close_map بنية libcrun_fd_map *needed_fds = عدم;

  dev_fds = get_نجی_انکڑے (container)->dev_fds;
  get_نجی_انکڑے (container)->dev_fds = عدم;

  needed_fds = get_نجی_انکڑے (container)->needed_devs_fds;
  get_نجی_انکڑے (container)->needed_devs_fds = عدم;

  إذا (! def || ! def->linux)
    أرجع 0;

  devfd = محفوظ_openat (get_نجی_انکڑے (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_رستہ | O_DIRECTORY, 0, err);
  إذا (UNLIKELY (devfd < 0))
    أرجع devfd;

  لكل (i = 0; i < def->linux->devices_len; i++)
    {
      بنية device_s device = {
        def->linux->devices[i]->رستہ,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_منوال,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      إذا (! def->linux->devices[i]->file_منوال_present)
        device.منوال = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  لكل (it = needed_devs, i = 0; it->رستہ; it++, i++)
    {
      cleanup_close صحيح srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  لكل (i = 0; symlinks[i].نشانہ; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].رستہ, devfd, symlinks[i].نشانہ);
      إذا (UNLIKELY (ret < 0))
        {
          صحيح saved_errno = errno;

          إذا (errno == EEXIST && ! symlinks[i].force)
            استمر;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          إذا (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].نشانہ, 0);
              إذا (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].نشانہ, AT_REMOVEDIR);
              إذا (ret < 0 && errno == EBUSY)
                {
                  cleanup_close صحيح tfd = openat (devfd, symlinks[i].نشانہ, O_CLOEXEC | O_رستہ | O_NOFOLLOW);
                  إذا (tfd >= 0)
                    {
                      proc_fd_رستہ_t procpath;

                      get_proc_self_fd_رستہ (procpath, tfd);
                      إذا (umount2 (procpath, MNT_DETACH) == 0)
                        اذهب retry_unlink;
                    }
                }
              إذا (ret == 0)
                اذهب retry_symlink;
            }
          أرجع crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].نشانہ);
        }
    }

  إذا (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
ساكن ثابت حرف *
اگلا_رستہ_component (ثابت حرف *p, size_t *len)
{
  لكل (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      إذا (*len != 1 || p[0] != '.')
        أرجع p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
ساكن bool
same_رستہ (ثابت حرف *a, ثابت حرف *b)
{
  size_t la, lb;

  لكل (;;)
    {
      a = اگلا_رستہ_component (a, &la);
      b = اگلا_رستہ_component (b, &lb);
      إذا (la != lb || memcmp (a, b, la) != 0)
        أرجع false;
      إذا (la == 0)
        أرجع true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
ساكن bool
is_duplicate_رستہ (حرف **paths, size_t i)
{
  size_t j;

  إذا (paths[i] == عدم)
    أرجع false;

  لكل (j = 0; j < i; j++)
    إذا (paths[j] && same_رستہ (paths[j], paths[i]))
      أرجع true;

  أرجع false;
}

ساكن صحيح
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  صحيح ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  إذا (! def || ! def->linux)
    أرجع 0;

  لكل (i = 0; i < def->linux->masked_paths_len; i++)
    {
      إذا (is_duplicate_رستہ (def->linux->masked_paths, i))
        استمر;

      ret = do_masked_or_readonly_رستہ (container, def->linux->masked_paths[i], false, false, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  لكل (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_رستہ (container, def->linux->readonly_paths[i], true, true, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
do_دھرا (libcrun_container_t *container, ثابت حرف *rootfs, libcrun_error_t *err)
{
  cleanup_close صحيح oldrootfd = -1;
  cleanup_close صحيح newrootfd = -1;
  صحيح ret;

  oldrootfd = open ("/", O_DIRECTORY | O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (oldrootfd < 0))
    أرجع crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (newrootfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = دھرا_جڑ (".", ".");
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, عدم, -1, ".", عدم, MS_REC | MS_PRIVATE, عدم, LABEL_MOUNT, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = umount2 (".", MNT_DETACH);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "umount2 oldroot");

  افعل
    {
      ret = umount2 (".", MNT_DETACH);
      إذا (ret < 0 && errno == EINVAL)
        اكسر;
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "umount2 oldroot");
  } طالما (ret == 0);

  ret = chdir ("/");
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "chdir to newroot");

  أرجع 0;
}

ساكن صحيح
append_tmpfs_منوال_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, حرف **انکڑے, libcrun_error_t *err)
{
  ثابت حرف *rootfs = get_نجی_انکڑے (container)->rootfs;
  bool خالی_تھاواں_انکڑے = is_خالی_تھاواں_string (*انکڑے);
  cleanup_close صحيح fd = -1;
  بنية stat st;
  صحيح ret;

  إذا (*انکڑے != عدم && لڑی_کھوج (*انکڑے, "mode="))
    أرجع 0;

  fd = محفوظ_openat (get_نجی_انکڑے (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_رستہ, 0, err);
  إذا (fd < 0)
    {
      إذا (crun_error_get_errno (err) != ENOENT)
        أرجع fd;

      crun_error_release (err);
      أرجع 0;
    }
  ret = fstat (fd, &st);
  إذا (ret < 0)
    أرجع crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (انکڑے, "%s%smode=%o", خالی_تھاواں_انکڑے ? "" : *انکڑے, خالی_تھاواں_انکڑے ? "" : ",", st.st_mode & 07777);
  أرجع 0;
}

ساكن صحيح
get_default_flags (libcrun_container_t *container, ثابت حرف *destination, حرف **انکڑے)
{
  إذا (strcmp (destination, "/proc") == 0)
    أرجع 0;
  إذا (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *انکڑے = xstrdup ("none,name=");
      أرجع MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  إذا (strcmp (destination, "/dev") == 0)
    {
      *انکڑے = xstrdup ("mode=755");
      أرجع MS_NOEXEC | MS_STRICTATIME;
    }
  إذا (strcmp (destination, "/dev/shm") == 0)
    {
      *انکڑے = xstrdup ("mode=1777,size=65536k");
      أرجع MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  إذا (strcmp (destination, "/dev/mqueue") == 0)
    أرجع MS_NOEXEC | MS_NOSUID | MS_NODEV;
  إذا (strcmp (destination, "/dev/pts") == 0)
    {
      إذا (container->host_uid == 0)
        *انکڑے = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      وإلا
        *انکڑے = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      أرجع MS_NOEXEC | MS_NOSUID;
    }
  إذا (strcmp (destination, "/sys") == 0)
    أرجع MS_NOEXEC | MS_NOSUID | MS_NODEV;

  أرجع 0;
}

ساكن ثابت حرف *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  أرجع لبھو_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

ساكن صحيح
check_valid_no_follow_file_system (صحيح fd, ثابت حرف *destination, libcrun_error_t *err)
{
  بنية statfs sfs;

  صحيح ret = fstatfs (fd, &sfs);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "statfs `%s`", destination);

  بدل (sfs.f_type)
    {
    حالة PROC_SUPER_MAGIC:
    حالة SYSFS_MAGIC:
    حالة DEVPTS_SUPER_MAGIC:
      أرجع crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  أرجع 0;
}

ساكن صحيح
محفوظ_create_symlink (صحيح rootfsfd, ثابت حرف *rootfs, ثابت حرف *نشانہ, ثابت حرف *destination, libcrun_error_t *err)
{
  cleanup_close صحيح ماپا_dir_fd = -1;
  cleanup_free حرف *بفر = عدم;
  حرف *part;
  صحيح ret;

  إذا (is_خالی_تھاواں_string (destination))
    أرجع crun_make_error (err, 0, "empty destination for symlink `%s`", نشانہ);

  بفر = xstrdup (destination);
  part = dirname (بفر);

  ماپا_dir_fd = crun_محفوظ_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  إذا (UNLIKELY (ماپا_dir_fd < 0))
    أرجع ماپا_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (بفر, destination);
  part = basename (بفر);

  ret = symlinkat (نشانہ, ماپا_dir_fd, part);
  إذا (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      إذا (errno == EEXIST)
        {
          cleanup_free حرف *link = عدم;
          ssize_t len;

          len = محفوظ_readlinkat (ماپا_dir_fd, part, &link, 0, err);
          إذا (UNLIKELY (len < 0))
            أرجع len;

          إذا ((((size_t) len) == لڑی_لمبائی (نشانہ)) && لڑی_مقابلہ (link, نشانہ, len) == 0)
            أرجع 0;

          أرجع crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      أرجع crun_make_error (err, errno, "symlinkat `%s`", نشانہ);
    }

  أرجع 0;
}

ساكن صحيح
handle_copy_symlink (libcrun_container_t *container, ثابت حرف *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     ثابت حرف *link_نشانہ, libcrun_error_t *err)
{
  cleanup_free حرف *نشانہ = عدم;

  إذا (link_نشانہ == عدم)
    {
      صحيح old_جڑ_fd = get_old_جڑ_fd (get_نجی_انکڑے (container));
      ثابت حرف *سروت = mount->سروت;
      ssize_t len;

      إذا (old_جڑ_fd >= 0 && سروت[0] == '/')
        len = محفوظ_readlinkat (old_جڑ_fd, سروت + 1, &نشانہ, 0, err);
      وإلا
        len = محفوظ_readlinkat (AT_FDCWD, سروت, &نشانہ, 0, err);
      إذا (UNLIKELY (len < 0))
        أرجع len;
      link_نشانہ = نشانہ;
    }

  أرجع محفوظ_create_symlink (get_نجی_انکڑے (container)->rootfsfd, rootfs,
                              link_نشانہ, mount->destination, err);
}

ساكن صحيح
prepare_sysfs_or_proc_mount (libcrun_container_t *container, ثابت حرف *نشانہ,
                             صحيح *targetfd, libcrun_error_t *err)
{
  صحيح ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_نجی_انکڑے (container)->rootfsfd, نشانہ,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOENT)
        {
          إذا (strchr (نشانہ, '/'))
            أرجع crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", نشانہ);

          ret = mkdirat (get_نجی_انکڑے (container)->rootfsfd, نشانہ, 0755);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "mkdirat `%s`", نشانہ);

          /* Try opening it again.  */
          ret = openat (get_نجی_انکڑے (container)->rootfsfd, نشانہ,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      وإلا إذا (errno == ENOTDIR)
        أرجع crun_make_error (err, errno, "the target `/%s` is invalid", نشانہ);

      إذا (ret < 0)
        أرجع crun_make_error (err, errno, "open `%s`", نشانہ);
    }

  *targetfd = ret;
  أرجع 0;
}

ساكن صحيح
handle_tmpcopyup (libcrun_container_t *container, ثابت حرف *rootfs, ثابت حرف *نشانہ,
                  صحيح copy_from_fd, libcrun_error_t *err)
{
  صحيح destfd, ret;
  cleanup_close صحيح tmpfd = copy_from_fd;

  نشانہ = consume_slashes (نشانہ);

  إذا (is_خالی_تھاواں_string (نشانہ))
    أرجع crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = محفوظ_openat (get_نجی_انکڑے (container)->rootfsfd, rootfs, نشانہ,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  إذا (UNLIKELY (destfd < 0))
    أرجع crun_error_wrap (err, "open `%s` to write for tmpcopyup", نشانہ);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_منزل_fd (tmpfd, destfd, نشانہ, نشانہ, err);
  tmpfd = -1;
  أرجع ret;
}

ساكن صحيح
get_mount_label_how (ثابت حرف *type, bool is_sysfs_or_proc)
{
  إذا (is_sysfs_or_proc)
    أرجع LABEL_NONE;

  إذا (strcmp (type, "mqueue") == 0)
    أرجع LABEL_XATTR;

  أرجع LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
ساكن بنانشانی لما
get_bind_mount_locked_flags (صحيح سروت_mountfd, ثابت حرف *سروت, runtime_spec_schema_defs_mount *mount)
{
  بنانشانی لما cleared = 0;
  بنية statfs sfs;
  size_t i;
  صحيح ret;

  إذا (سروت_mountfd >= 0)
    ret = fstatfs (سروت_mountfd, &sfs);
  وإلا
    ret = statfs (سروت, &sfs);
  إذا (UNLIKELY (ret < 0))
    أرجع 0;

  لكل (i = 0; i < mount->options_len; i++)
    {
      ثابت بنية پسار_flags_s *prop;

      إذا (mount->options[i] == عدم)
        استمر;

      prop = libcrun_str2mount_flags (mount->options[i]);
      إذا (prop == عدم || (prop->extra_flags & OPTION_RECURSIVE))
        استمر;

      إذا (prop->clear)
        cleared |= prop->flags;
      وإلا
        cleared &= ~prop->flags;
    }

  أرجع sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
ساكن ثابت حرف *
get_رستہ_وچ_rootfs (ثابت حرف *rootfs, ثابت حرف *رستہ)
{
  size_t len;

  إذا (rootfs == عدم || رستہ == عدم)
    أرجع عدم;

  len = لڑی_لمبائی (rootfs);
  طالما (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  إذا (len == 0)
    أرجع عدم;

  إذا (لڑی_مقابلہ (رستہ, rootfs, len) != 0 || (رستہ[len] != '/' && رستہ[len] != '\0'))
    أرجع عدم;

  أرجع consume_slashes (رستہ + len);
}

ساكن صحيح
process_single_mount (libcrun_container_t *container, ثابت حرف *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      بنية libcrun_fd_map *mount_fds, size_t mount_اشاریہ,
                      ثابت حرف *systemd_cgroup_v1, libcrun_error_t *err)
{
  ثابت حرف *نشانہ = consume_slashes (mount->destination);
  cleanup_close صحيح سروت_mountfd = -1;
  cleanup_free حرف *انکڑے = عدم;
  حرف *type;
  حرف *سروت;
  بنانشانی لما flags = 0;
  بنانشانی لما extra_flags = 0;
  mode_t src_منوال = S_IFDIR;
  cleanup_close صحيح copy_from_fd = -1;
  cleanup_close صحيح targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_سیٹ = 0;
  صحيح no_remount = 0;
  صحيح ret;

  إذا (mount_fds)
    سروت_mountfd = get_and_reset (&(mount_fds->fds[mount_اشاریہ]));

  type = mount->type;

  إذا (mount->options == عدم)
    flags = get_default_flags (container, mount->destination, &انکڑے);
  وإلا
    {
      size_t j;

      لكل (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &انکڑے, &rec_clear, &rec_سیٹ);
    }

  إذا (type == عدم && (flags & MS_BIND) == 0)
    أرجع crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  إذا ((flags & MS_BIND) && سروت_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      ثابت حرف *rel_سروت = get_رستہ_وچ_rootfs (get_نجی_انکڑے (container)->host_rootfs, mount->سروت);

      إذا (rel_سروت)
        {
          بنانشانی لما پسار = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          سروت_mountfd = get_bind_mount (get_نجی_انکڑے (container)->rootfsfd, rel_سروت, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, پسار, err);
          إذا (UNLIKELY (سروت_mountfd < 0))
            أرجع crun_error_wrap (err, "open mount source `%s`", mount->سروت);
        }
    }

  إذا ((flags & MS_BIND) && mount->سروت && has_mount_flag_options (mount)
      && (get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (سروت_mountfd, mount->سروت, mount);

  إذا (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      إذا (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      إذا (رستہ_is_slash_dev (mount->destination))
        get_نجی_انکڑے (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  إذا (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_منوال_if_missing (container, mount, &انکڑے, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (mount->سروت && (flags & MS_BIND))
    {
      proc_fd_رستہ_t proc_buf;
      ثابت حرف *رستہ = mount->سروت;

      إذا ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        أرجع crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_نجی_انکڑے (container)->copy_symlink_targets
                               && mount_اشاریہ < get_نجی_انکڑے (container)->n_copy_symlink_targets
                               && get_نجی_انکڑے (container)->copy_symlink_targets[mount_اشاریہ];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      إذا (سروت_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_رستہ (proc_buf, سروت_mountfd);
          رستہ = proc_buf;

          ret = get_file_type_at (سروت_mountfd, &src_منوال, true, عدم);
        }
      وإلا إذا (is_cached_symlink)
        {
          src_منوال = S_IFLNK;
          ret = 0;
        }
      وإلا إذا (رستہ[0] == '/')
        {
          صحيح old_جڑ_fd = get_old_جڑ_fd (get_نجی_انکڑے (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          إذا (old_جڑ_fd >= 0)
            ret = get_file_type_at (old_جڑ_fd, &src_منوال, nofollow, (رستہ[1] == '\0') ? عدم : رستہ + 1);
          وإلا
            ret = get_file_type_at (AT_FDCWD, &src_منوال, nofollow, رستہ);
        }
      وإلا
        ret = get_file_type (&src_منوال, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, رستہ);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "cannot stat `%s`", رستہ);

      إذا (S_ISLNK (src_منوال) && (extra_flags & OPTION_DEST_NOFOLLOW) && سروت_mountfd < 0)
        {
          صحيح bind_dirfd = AT_FDCWD;
          ثابت حرف *bind_سروت = mount->سروت;
          صحيح old_جڑ_fd = get_old_جڑ_fd (get_نجی_انکڑے (container));

          إذا (old_جڑ_fd >= 0 && mount->سروت && mount->سروت[0] == '/')
            {
              bind_dirfd = old_جڑ_fd;
              bind_سروت = (mount->سروت[1] == '\0') ? "." : mount->سروت + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_سروت, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          سروت_mountfd = ret;
        }
    }

  إذا (S_ISLNK (src_منوال) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      ثابت حرف *cached_نشانہ = عدم;

      إذا (get_نجی_انکڑے (container)->copy_symlink_targets
          && mount_اشاریہ < get_نجی_انکڑے (container)->n_copy_symlink_targets)
        cached_نشانہ = get_نجی_انکڑے (container)->copy_symlink_targets[mount_اشاریہ];

      ret = handle_copy_symlink (container, rootfs, mount, cached_نشانہ, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      mounted = true;
    }
  وإلا إذا (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, نشانہ, &targetfd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      bool is_dir = S_ISDIR (src_منوال);

      إذا (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = محفوظ_openat (get_نجی_انکڑے (container)->rootfsfd, rootfs, نشانہ, O_رستہ | O_NOFOLLOW | O_CLOEXEC, 0, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, نشانہ, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_محفوظ_create_and_open_ref_at (is_dir, get_نجی_انکڑے (container)->rootfsfd, rootfs, نشانہ, 0755, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          targetfd = ret;
        }
    }

  إذا (extra_flags & OPTION_TMPCOPYUP)
    {
      إذا (strcmp (type, "tmpfs") != 0)
        أرجع crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      إذا (UNLIKELY (copy_from_fd < 0))
        {
          إذا (errno != ENOTDIR)
            أرجع crun_make_error (err, errno, "cannot reopen `%s`", نشانہ);
        }
    }

  سروت = mount->سروت ? mount->سروت : type;

  /* Check if there is already a mount for the requested file system.  */
  إذا (! mounted && سروت_mountfd >= 0)
    {
      ret = fs_move_mount_منزل (سروت_mountfd, targetfd, عدم);
      إذا (LIKELY (ret == 0))
        {
          بنانشانی لما بچیا_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          إذا (بچیا_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, عدم, سروت_mountfd, نشانہ, عدم, بچیا_flags, انکڑے, LABEL_NONE | no_remount, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;
            }

          mounted = true;

          إذا (is_خالی_تھاواں_string (نشانہ))
            {
              صحيح new_rootfsfd = open (rootfs, O_رستہ | O_CLOEXEC);
              إذا (UNLIKELY (new_rootfsfd < 0))
                أرجع crun_make_error (err, errno, "reopen rootfs after mount on /");

              عارضی_FAILURE_RETRY (close (get_نجی_انکڑے (container)->rootfsfd));
              get_نجی_انکڑے (container)->rootfsfd = new_rootfsfd;
            }
        }
      وإلا إذا (get_نجی_انکڑے (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          أرجع crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", نشانہ);
        }
      وإلا إذا (get_نجی_انکڑے (container)->host_جڑ_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          أرجع crun_make_error (err, errno, "move_mount `%s` to `%s`", سروت, نشانہ);
        }
      وإلا إذا (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           سروت, نشانہ, strerror (errno));
        }
    }

  إذا (! mounted)
    {
      إذا (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, سروت, targetfd, نشانہ, flags, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا إذا (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, سروت, targetfd, نشانہ, flags, انکڑے, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا
        {
          صحيح extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, سروت, targetfd, نشانہ, type, flags, انکڑے, extra_flags, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  إذا (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, نشانہ, copy_from_fd, err);
      copy_from_fd = -1;
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (rec_clear || rec_سیٹ)
    {
      ثابت bool is_dir = S_ISDIR (src_منوال);
      cleanup_close صحيح dfd = -1;

      dfd = محفوظ_openat (get_نجی_انکڑے (container)->rootfsfd, rootfs, نشانہ, O_RDONLY | O_رستہ | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      إذا (UNLIKELY (dfd < 0))
        أرجع crun_error_wrap (err, "open mount target `/%s`", نشانہ);

      ret = do_mount_setattr (true, نشانہ, dfd, rec_clear, rec_سیٹ, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

ساكن صحيح
do_mounts (libcrun_container_t *container, ثابت حرف *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت حرف *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map بنية libcrun_fd_map *mount_fds = عدم;
  size_t i;
  صحيح ret;

  mount_fds = get_نجی_انکڑے (container)->mount_fds;
  get_نجی_انکڑے (container)->mount_fds = عدم;

  لكل (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
صحيح
libcrun_container_do_bind_mount (libcrun_container_t *container, حرف *mount_سروت, حرف *mount_destination, حرف **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  ثابت حرف *نشانہ = consume_slashes (mount_destination);
  cleanup_free حرف *انکڑے = عدم;
  بنانشانی لما flags = 0;
  بنانشانی لما extra_flags = 0;
  cleanup_close صحيح targetfd = -1;
  صحيح is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_سیٹ = 0;
  ثابت حرف *rootfs = get_نجی_انکڑے (container)->rootfs;
  صحيح ret;

  إذا ((get_نجی_انکڑے (container)->rootfsfd < 0) || (rootfs == عدم))
    أرجع crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  إذا (mount_options == عدم)
    flags = get_default_flags (container, mount_destination, &انکڑے);
  وإلا
    {
      size_t j;

      لكل (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &انکڑے, &rec_clear, &rec_سیٹ);
    }

  إذا (رستہ_is_slash_dev (mount_destination))
    get_نجی_انکڑے (container)->mount_dev_from_host = true;

  إذا (mount_سروت && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_سروت, false, err);
      إذا (UNLIKELY (is_dir < 0))
        أرجع is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_محفوظ_create_and_open_ref_at (is_dir, get_نجی_انکڑے (container)->rootfsfd, rootfs, نشانہ, 0755, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  targetfd = ret;

  ret = do_mount (container, mount_سروت, targetfd, نشانہ, "bind", flags, انکڑے, LABEL_MOUNT, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
صحيح
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, صحيح *notify_socket_باہرلا, libcrun_error_t *err)
{
#جے_تعریف_شدہ HAVE_SYSTEMD
  cleanup_close صحيح notify_fd = -1;
  cleanup_free حرف *host_notify_socket_رستہ = عدم;
  cleanup_free حرف *state_dir = عدم;
  حرف *host_رستہ = عدم;
  صحيح ret;

  إذا (container && get_نجی_انکڑے (container)->host_notify_socket_رستہ)
    {
      ثابت حرف *ماپا_dir;

      ماپا_dir = get_نجی_انکڑے (container)->host_notify_socket_رستہ;

      ret = append_paths (&host_notify_socket_رستہ, err, ماپا_dir, "notify", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      host_رستہ = host_notify_socket_رستہ;
    }

  *notify_socket_باہرلا = -1;

  إذا (host_رستہ == عدم)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_جڑ, context->id, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = append_paths (&host_notify_socket_رستہ, err, state_dir, "notify/notify", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      host_رستہ = host_notify_socket_رستہ;
    }

  notify_fd = open_unix_domain_socket (host_رستہ, 1, err);
  إذا (UNLIKELY (notify_fd < 0))
    أرجع notify_fd;

  إذا (UNLIKELY (chmod (host_رستہ, 0777) < 0))
    أرجع crun_make_error (err, errno, "chmod `%s`", host_رستہ);

#  ifdef HAVE_FGETXATTR
  إذا (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (فراغ) setxattr (host_رستہ, "security.selinux", container->container_def->linux->mount_label,
                       لڑی_لمبائی (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_باہرلا = get_and_reset (&notify_fd);
  أرجع 1;
#وإلا
  (فراغ) context;
  (فراغ) container;
  (فراغ) err;
  *notify_socket_باہرلا = -1;
  أرجع 0;
#جے_انت
}

#جے_تعریف_شدہ HAVE_SYSTEMD
ساكن صحيح
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;
  ثابت حرف *notify_socket = container->context->notify_socket;
  cleanup_free حرف *host_notify_socket_رستہ = عدم;
  cleanup_free حرف *container_notify_socket_رستہ = عدم;
  cleanup_free حرف *state_dir = عدم;
  uid_t container_جڑ_uid = -1;
  gid_t container_جڑ_gid = -1;
  صحيح notify_socket_درخت_fd;

  إذا (notify_socket == عدم)
    أرجع 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_جڑ : عدم),
                                     (container->context ? container->context->id : عدم), err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&container_notify_socket_رستہ, err, "/", notify_socket, "notify", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&host_notify_socket_رستہ, err, state_dir, "notify", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = mkdir (host_notify_socket_رستہ, 0700);
  إذا (ret < 0)
    أرجع crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_رستہ);

  إذا (get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_جڑ_وچ_the_userns (container->container_def, 0, 0, &container_جڑ_uid, &container_جڑ_gid);
      إذا (container_جڑ_uid != ((uid_t) -1) && container_جڑ_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_رستہ, container_جڑ_uid, container_جڑ_gid);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_رستہ, container_جڑ_uid, container_جڑ_gid);
        }
    }

  notify_socket_درخت_fd = syscall_open_درخت (AT_FDCWD, host_notify_socket_رستہ, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  إذا (notify_socket_درخت_fd >= 0)
    /* open_tree worked */
    get_نجی_انکڑے (container)->notify_socket_درخت_fd = notify_socket_درخت_fd;
  وإلا إذا (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  وإلا إذا (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  وإلا
    /* some other error */
    أرجع crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_رستہ);

  get_نجی_انکڑے (container)->host_notify_socket_رستہ = host_notify_socket_رستہ;
  get_نجی_انکڑے (container)->container_notify_socket_رستہ = container_notify_socket_رستہ;
  host_notify_socket_رستہ = container_notify_socket_رستہ = عدم;
  أرجع 0;
}
#جے_انت

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
ساكن صحيح
do_finalize_notify_socket (libcrun_container_t *container, ثابت حرف *rootfs, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *host_notify_socket_رستہ = عدم;
  cleanup_free حرف *container_notify_socket_رستہ = عدم;
  cleanup_free حرف *container_notify_socket_رستہ_dir_alloc = عدم;
  ثابت حرف *container_notify_socket_رستہ_dir = عدم;
  cleanup_close صحيح notify_socket_درخت_fd = -1;
  cleanup_close صحيح targetfd = -1;
  صحيح did_mount_with_move_mount = 0;

  host_notify_socket_رستہ = get_نجی_انکڑے (container)->host_notify_socket_رستہ;
  get_نجی_انکڑے (container)->host_notify_socket_رستہ = عدم;

  container_notify_socket_رستہ = get_نجی_انکڑے (container)->container_notify_socket_رستہ;
  get_نجی_انکڑے (container)->container_notify_socket_رستہ = عدم;

  إذا (host_notify_socket_رستہ == عدم || container_notify_socket_رستہ == عدم)
    أرجع 0;

  إذا (rootfs == عدم)
    rootfs = "/";

  container_notify_socket_رستہ_dir_alloc = xstrdup (container_notify_socket_رستہ);
  container_notify_socket_رستہ_dir = consume_slashes (dirname (container_notify_socket_رستہ_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  إذا (get_نجی_انکڑے (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_رستہ | O_CLOEXEC);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "open `%s`", rootfs);

      get_نجی_انکڑے (container)->rootfsfd = ret;
      get_نجی_انکڑے (container)->rootfs = rootfs;
    }

  ret = crun_محفوظ_create_and_open_ref_at (true, get_نجی_انکڑے (container)->rootfsfd, rootfs,
                                          container_notify_socket_رستہ_dir, 0755, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  targetfd = ret;

  notify_socket_درخت_fd = get_نجی_انکڑے (container)->notify_socket_درخت_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_نجی_انکڑے (container)->notify_socket_درخت_fd = -1;

  إذا (notify_socket_درخت_fd >= 0)
    {
      ret = fs_move_mount_منزل (notify_socket_درخت_fd, targetfd, عدم);
      إذا (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_نشانہ (container, container_notify_socket_رستہ_dir, err);
          إذا (UNLIKELY (targetfd < 0))
            أرجع targetfd;

          ret = do_mount (container, عدم, targetfd, container_notify_socket_رستہ_dir, عدم,
                          MS_REC | MS_PRIVATE, عدم, LABEL_NONE, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا إذا (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      وإلا
        أرجع crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_درخت_fd,
                                container_notify_socket_رستہ_dir);
    }

  إذا (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_رستہ, targetfd, container_notify_socket_رستہ_dir, عدم,
                      MS_BIND | MS_REC | MS_PRIVATE, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

ساكن صحيح
make_ماپا_mount_نجی (ثابت حرف *rootfs, libcrun_error_t *err)
{
  cleanup_close صحيح rootfsfd = -1;
  بنية stat prev_st;
  proc_fd_رستہ_t proc_رستہ;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (rootfsfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", rootfs);

  إذا (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    أرجع crun_make_error (err, errno, "fstat `%s`", rootfs);

  لكل (;;)
    {
      بنية stat cur_st;
      libcrun_error_t tmp_err = عدم;
      صحيح ret;
      cleanup_close صحيح parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      إذا (ret == 0)
        أرجع 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_رستہ (proc_رستہ, rootfsfd);
      ret = mount (عدم, proc_رستہ, عدم, MS_PRIVATE, عدم);
      إذا (ret == 0)
        أرجع 0;

      parentfd = openat (rootfsfd, "..", O_رستہ | O_CLOEXEC);
      إذا (parentfd < 0)
        {
          صحيح saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          إذا (ret != 0)
            أرجع crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          أرجع crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      إذا (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        أرجع crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      إذا (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        اكسر;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  أرجع crun_make_error (err, 0, "make `%s` private", rootfs);
}

صحيح
libcrun_سیٹ_mounts (بنية container_entrypoint_s *entrypoint_دلیلاں, libcrun_container_t *container, ثابت حرف *rootfs, سیٹ_mounts_cb_t cb, فراغ *cb_انکڑے, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح is_user_ns = 0;
  صحيح ret = 0;

  إذا (rootfs == عدم || def->mounts == عدم)
    أرجع 0;

  ret = open (rootfs, O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "open `%s`", rootfs);

  get_نجی_انکڑے (container)->rootfsfd = ret;
  get_نجی_انکڑے (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_دلیلاں, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  إذا (def->جڑ->readonly)
    {
      بنية remount_s *r;
      بنانشانی لما remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      صحيح fd;

      fd = dup (get_نجی_انکڑے (container)->rootfsfd);
      إذا (UNLIKELY (fd < 0))
        أرجع crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, عدم, get_نجی_انکڑے (container)->remounts);
      get_نجی_انکڑے (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWUSER);
  إذا (! is_user_ns)
    {
      is_user_ns = check_running_وچ_user_namespace (err);
      إذا (UNLIKELY (is_user_ns < 0))
        أرجع is_user_ns;
    }

  إذا (! get_نجی_انکڑے (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  إذا (cb)
    {
      ret = cb (cb_انکڑے, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = عدم;
      ثابت حرف *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (فراغ) crun_محفوظ_ensure_directory_at (get_نجی_انکڑے (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

ساكن صحيح
umount_or_hide (ثابت حرف *نشانہ, libcrun_error_t *err)
{
  صحيح ret;

  ret = umount2 (نشانہ, MNT_DETACH);
  إذا (UNLIKELY (ret < 0))
    {
      صحيح saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      إذا (errno == EINVAL)
        {
          ret = mount (عدم, نشانہ, "tmpfs", 0, "size=0k");
          إذا (LIKELY (ret == 0))
            أرجع 0;
        }

      أرجع crun_make_error (err, saved_errno, "umount `%s`", نشانہ);
    }

  أرجع ret;
}

ساكن صحيح
move_جڑ (ثابت حرف *rootfs, libcrun_error_t *err)
{
  صحيح ret;

  ret = chdir (rootfs);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = umount_or_hide ("/proc", err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "chdir to `%s`", rootfs);

  أرجع 0;
}

صحيح
libcrun_finalize_mounts (بنية container_entrypoint_s *entrypoint_دلیلاں, libcrun_container_t *container, ثابت حرف *rootfs,
                         سیٹ_mounts_cb_t after_دھرا_cb, فراغ *cb_انکڑے, libcrun_error_t *err)
{
  صحيح ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_دلیلاں, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  close_and_reset (&(get_نجی_انکڑے (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_نجی_انکڑے (container)->old_جڑ_fd));

  إذا (get_نجی_انکڑے (container)->needs_دھرا)
    {
      get_نجی_انکڑے (container)->needs_دھرا = false;

      إذا (get_نجی_انکڑے (container)->no_دھرا)
        {
          ret = move_جڑ (rootfs, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا
        {
          ret = do_دھرا (container, rootfs, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = do_mount (container, عدم, -1, "/", عدم,
                      get_نجی_انکڑے (container)->rootfs_پسار,
                      عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = chdir ("/");
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "chdir to `/`");
    }

  إذا (after_دھرا_cb)
    {
      ret = after_دھرا_cb (cb_انکڑے, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = finalize_mounts (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

ساكن صحيح
maybe_open_درخت_namespace (ثابت حرف *rootfs, صحيح *باہرلا_fd, libcrun_error_t *err)
{
  cleanup_close صحيح rootfs_fd = -1;
  صحيح درخت_fd;

  *باہرلا_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (rootfs_fd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", rootfs);

  درخت_fd = syscall_open_درخت (rootfs_fd, "",
                               OPEN_درخت_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_خالی_تھاواں_رستہ | AT_RECURSIVE);
  إذا (درخت_fd < 0)
    {
      إذا (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        أرجع 0;
      أرجع crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *باہرلا_fd = درخت_fd;
  أرجع 0;
}

ساكن بنية libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  بنية libcrun_fd_map *mount_fds = get_نجی_انکڑے (container)->mount_fds;

  إذا (mount_fds == عدم)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_نجی_انکڑے (container)->mount_fds = mount_fds;
    }
  أرجع mount_fds;
}

ساكن صحيح
open_mount_دا_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free حرف *mnt_انکڑے = عدم;
  بنانشانی لما mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_سیٹ = 0;
  ثابت حرف *label_type = عدم;
  ثابت حرف *label_val = عدم;
  ثابت حرف *fstype;
  صحيح mnt_fd;
  size_t j;

  إذا (mount->type == عدم)
    أرجع -1;

  fstype = mount->type;

  لكل (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_انکڑے,
                                            &mnt_rec_clear, &mnt_rec_سیٹ);

  إذا (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      صحيح extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      إذا (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          إذا (label_type == عدم)
            crun_error_release (err);
          وإلا
            label_val = def->linux->mount_label;
        }
    }

  إذا (strcmp (fstype, "cgroup") == 0)
    {
      صحيح cgroup_منوال = libcrun_get_cgroup_منوال (err);
      إذا (cgroup_منوال == CGROUP_منوال_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->سروت, label_type, label_val, mnt_انکڑے);
  إذا (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->سروت, عدم, عدم, mnt_انکڑے);

  أرجع mnt_fd;
}

#جے_بنا_تعریف LSMT_ROOT
#  define LSMT_ROOT 0xffffffffffffffffULL
#جے_انت
#جے_بنا_تعریف STATMOUNT_FS_TYPE
#  define STATMOUNT_FS_TYPE 0x00000020U
#جے_انت
#جے_بنا_تعریف STATMOUNT_MNT_ROOT
#  define STATMOUNT_MNT_ROOT 0x00000040U
#جے_انت
#جے_بنا_تعریف STATMOUNT_MNT_POINT
#  define STATMOUNT_MNT_POINT 0x00000080U
#جے_انت

/* Declared here so that the build does not depend on the UAPI headers being
   recent enough; the layout is stable.  */
بنية mnt_id_req_s
{
  uint32_t ناپ;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

بنية statmount_s
{
  uint32_t ناپ;
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

ساكن صحيح
syscall_listmount (بنية mnt_id_req_s *req, uint64_t *ids, size_t nr, بنانشانی صحيح flags)
{
#جے_تعریف_شدہ __NR_listmount
  أرجع (صحيح) syscall (__NR_listmount, req, ids, nr, flags);
#وإلا
  (فراغ) req;
  (فراغ) ids;
  (فراغ) nr;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#جے_انت
}

ساكن صحيح
syscall_statmount (بنية mnt_id_req_s *req, بنية statmount_s *buf, size_t ناپ, بنانشانی صحيح flags)
{
#جے_تعریف_شدہ __NR_statmount
  أرجع (صحيح) syscall (__NR_statmount, req, buf, ناپ, flags);
#وإلا
  (فراغ) req;
  (فراغ) buf;
  (فراغ) ناپ;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#جے_انت
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
ساكن bool
mounts_کامیابی_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = عدم;
  size_t i, n_ids = 0, سمرتھا = 128;
  bool has_absolute_bind = false;
  uint64_t آخری = 0;

  لكل (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      إذا (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->سروت != عدم && def->mounts[i]->سروت[0] == '/')
        {
          has_absolute_bind = true;
          اكسر;
        }
    }
  إذا (! has_absolute_bind)
    أرجع false;

  ids = xmalloc (سمرتھا * حجم (uint64_t));
  لكل (;;)
    {
      بنية mnt_id_req_s req = {
        .ناپ = حجم (req),
        .mnt_id = LSMT_ROOT,
        .param = آخری,
      };
      صحيح ret;

      إذا (n_ids == سمرتھا)
        {
          سمرتھا *= 2;
          ids = xrealloc (ids, سمرتھا * حجم (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, سمرتھا - n_ids, 0);
      إذا (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        أرجع true;
      إذا (ret == 0)
        اكسر;

      n_ids += (size_t) ret;
      آخری = ids[n_ids - 1];
    }

  لكل (i = 0; i < n_ids; i++)
    {
      حرف بفر[4096] __attribute__ ((aligned (8)));
      بنية statmount_s *sm = (بنية statmount_s *) بفر;
      بنية mnt_id_req_s req = {
        .ناپ = حجم (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      ثابت حرف *جڑ, *نقطہ;
      size_t j;

      إذا (syscall_statmount (&req, sm, حجم (بفر), 0) < 0)
        استمر;
      إذا ((sm->نقاب & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        استمر;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      جڑ = sm->str + sm->mnt_root;
      إذا (! has_اگیتر (جڑ, "mnt:["))
        استمر;

      نقطہ = sm->str + sm->mnt_point;

      لكل (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          ثابت حرف *سروت = def->mounts[j]->سروت;
          size_t سروت_len;

          إذا (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            استمر;
          إذا (سروت == عدم || سروت[0] != '/')
            استمر;

          سروت_len = لڑی_لمبائی (سروت);
          طالما (سروت_len > 1 && سروت[سروت_len - 1] == '/')
            سروت_len--;

          إذا (لڑی_مقابلہ (نقطہ, سروت, سروت_len) != 0)
            استمر;

          /* The source itself is a pinned mount namespace.  */
          إذا (نقطہ[سروت_len] == '\0')
            أرجع true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          إذا (recursive && (سروت_len == 1 || نقطہ[سروت_len] == '/'))
            أرجع true;
        }
    }

  أرجع false;
}

ساكن bool
can_use_open_درخت_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  بنية libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  إذا (has_hooks || has_userns)
    أرجع false;

  إذا (mounts_کامیابی_pinned_mount_namespace (def))
    أرجع false;

  إذا (container->context->no_open_درخت_namespace)
    أرجع false;

  {
    libcrun_error_t tmp_err = عدم;
    صحيح وچ_userns = check_running_وچ_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    إذا (وچ_userns != 0)
      أرجع false;
  }

  mount_fds = get_fd_map (container);
  لكل (i = 0; i < def->mounts_len; i++)
    {
      إذا (mount_fds->fds[i] < 0)
        {
          bool has_cached_نشانہ = get_نجی_انکڑے (container)->copy_symlink_targets
                                   && i < get_نجی_انکڑے (container)->n_copy_symlink_targets
                                   && get_نجی_انکڑے (container)->copy_symlink_targets[i];
          إذا (! has_cached_نشانہ)
            أرجع false;
        }
    }

  أرجع true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
ساكن bool
mount_inherits_mountpoint_منوال (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  إذا (mount->type == عدم || strcmp (mount->type, "tmpfs") != 0)
    أرجع false;

  لكل (i = 0; i < mount->options_len; i++)
    إذا (mount->options[i] != عدم && has_اگیتر (mount->options[i], "mode="))
      أرجع false;

  أرجع true;
}

ساكن صحيح
تیاری_mount_namespace (libcrun_container_t *container, bool no_دھرا, حرف **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  بنانشانی لما rootfs_پسار = 0;
  cleanup_close صحيح rootfsfd = -1;
  cleanup_close صحيح درخت_fd = -1;
  libcrun_error_t tmp_err = عدم;
  bool use_open_درخت = false;
  size_t i;
  صحيح ret;

  إذا (def->linux && def->linux->rootfs_پسار)
    rootfs_پسار = get_mount_flags (def->linux->rootfs_پسار, 0, عدم, عدم, عدم, عدم);

  إذا ((rootfs_پسار & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_پسار = MS_REC | MS_PRIVATE;

  get_نجی_انکڑے (container)->rootfs_پسار = rootfs_پسار;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  حرر (get_نجی_انکڑے (container)->host_rootfs);
  get_نجی_انکڑے (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  لكل (i = 0; i < def->mounts_len; i++)
    {
      بنية libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close صحيح mnt_fd = -1;
      bool recursive_bind, nofollow;
      بنانشانی لما mnt_flags = 0;

      إذا (mount_fds && mount_fds->fds[i] >= 0)
        استمر;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      إذا (def->mounts[i]->سروت != عدم
          && def->mounts[i]->سروت[0] == '/')
        {
          cleanup_free حرف *mnt_انکڑے = عدم;
          بنانشانی لما mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_سیٹ = 0;
          size_t j;

          لكل (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_انکڑے,
                                                    &mnt_rec_clear, &mnt_rec_سیٹ);

          إذا (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free حرف *نشانہ = عدم;
              libcrun_error_t tmp_err = عدم;
              ssize_t len;

              len = محفوظ_readlinkat (AT_FDCWD, def->mounts[i]->سروت, &نشانہ, 0, &tmp_err);
              إذا (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              إذا (get_نجی_انکڑے (container)->copy_symlink_targets == عدم)
                {
                  get_نجی_انکڑے (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * حجم (حرف *));
                  get_نجی_انکڑے (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_نجی_انکڑے (container)->copy_symlink_targets[i] = نشانہ;
              نشانہ = عدم;
            }
        }

      إذا (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_نشانہ = get_نجی_انکڑے (container)->copy_symlink_targets
                                   && i < get_نجی_انکڑے (container)->n_copy_symlink_targets
                                   && get_نجی_انکڑے (container)->copy_symlink_targets[i];

          إذا (mount_fds && def->mounts[i]->سروت != عدم && ! has_cached_نشانہ
              && get_رستہ_وچ_rootfs (*rootfs, def->mounts[i]->سروت) == عدم)
            {
              libcrun_error_t tmp_err = عدم;
              بنانشانی لما پسار = MS_PRIVATE;

              إذا (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                پسار = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->سروت,
                                                  recursive_bind, false, nofollow,
                                                  پسار, &tmp_err);
              إذا (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          استمر;
        }

      mnt_fd = open_mount_دا_type (container, def->mounts[i], err);
      إذا (mnt_fd < 0)
        {
          crun_error_release (err);
          استمر;
        }

      إذا (mount_inherits_mountpoint_منوال (def->mounts[i]))
        {
          cleanup_close صحيح destfd = -1;
          بنية stat st;

          إذا (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_رستہ | O_CLOEXEC);
              إذا (UNLIKELY (rootfsfd < 0))
                أرجع crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = محفوظ_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_رستہ | O_CLOEXEC, 0, &tmp_err);
          إذا (destfd < 0)
            crun_error_release (&tmp_err);
          وإلا إذا (fstat (destfd, &st) == 0)
            {
              proc_fd_رستہ_t fd_رستہ;
              صحيح procfd = get_procfd (get_نجی_انکڑے (container), err);
              إذا (UNLIKELY (procfd < 0))
                أرجع procfd;

              get_self_fd_رستہ (fd_رستہ, mnt_fd);
              ret = fchmodat (procfd, fd_رستہ, st.st_mode & 07777, 0);
              (فراغ) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_خالی_تھاواں_dir_cached (container, عدم, &tmp_err);
  إذا (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_نجی_انکڑے (container)->maskdir_bind_failed = true;
    }

  إذا (! get_نجی_انکڑے (container)->joined_mount_ns
      && can_use_open_درخت_namespace (container))
    use_open_درخت = true;

  إذا (use_open_درخت)
    {
      ret = maybe_open_درخت_namespace (*rootfs, &درخت_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  إذا (درخت_fd >= 0)
    preopen_needed_devs_and_notify (container);

  إذا (درخت_fd >= 0)
    {
      ret = setns (درخت_fd, CLONE_NEWNS);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (عدم, "/", عدم, MS_REMOUNT | MS_BIND, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, عدم, -1, "/", عدم, MS_REC | MS_PRIVATE, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = do_mount (container, عدم, -1, "/", عدم, rootfs_پسار, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      get_نجی_انکڑے (container)->needs_دھرا = false;
      get_نجی_انکڑے (container)->no_دھرا = no_دھرا;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_نجی_انکڑے (container)->host_جڑ_switched = true;
      حرر (*rootfs);
      *rootfs = xstrdup ("/");
    }
  وإلا
    {
      إذا (! get_نجی_انکڑے (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, عدم, -1, "/", عدم, rootfs_پسار, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = make_ماپا_mount_نجی (*rootfs, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, عدم, MS_BIND | MS_REC | MS_PRIVATE, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      get_نجی_انکڑے (container)->needs_دھرا = true;
      get_نجی_انکڑے (container)->no_دھرا = no_دھرا;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "open `%s`", *rootfs);
  get_نجی_انکڑے (container)->rootfsfd = ret;
  get_نجی_انکڑے (container)->rootfs = *rootfs;

  إذا (! container->context->force_no_cgroup)
    {
      صحيح cgroup_منوال = libcrun_get_cgroup_منوال (err);
      إذا (UNLIKELY (cgroup_منوال < 0))
        أرجع cgroup_منوال;

      إذا (cgroup_منوال == CGROUP_منوال_UNIFIED)
        {
          حرف *unified_cgroup_رستہ = عدم;
          صحيح procfd = get_procfd (get_نجی_انکڑے (container), err);
          إذا (UNLIKELY (procfd < 0))
            أرجع procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_رستہ, true, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          get_نجی_انکڑے (container)->unified_cgroup_رستہ = unified_cgroup_رستہ;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  إذا (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_نجی_انکڑے (container)->rootfsfd);
      get_نجی_انکڑے (container)->rootfs = عدم;
      أرجع ret;
    }

  close_and_reset (&get_نجی_انکڑے (container)->rootfsfd);
  get_نجی_انکڑے (container)->rootfs = عدم;

  إذا (درخت_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_نجی_انکڑے (container), &tmp_err);
  إذا (ret < 0)
    crun_error_release (&tmp_err);

  check_running_وچ_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  أرجع 0;
}

صحيح
libcrun_do_دھرا_جڑ (libcrun_container_t *container, bool no_دھرا, حرف **rootfs, libcrun_error_t *err)
{
  صحيح ret;

  إذا (*rootfs == عدم)
    أرجع 0;

  إذا (get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = تیاری_mount_namespace (container, no_دھرا, rootfs, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      إذا (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "chdir to `/`");
        }
    }
  وإلا
    {
      ret = chroot (*rootfs);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "chdir to `/`");

      حرر (*rootfs);
      *rootfs = xstrdup ("/");
    }

  أرجع 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
صحيح
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  بنية stat statbuf;
  cleanup_close صحيح fd;
  صحيح ret;
  صحيح i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  إذا (UNLIKELY (fd == -1))
    أرجع crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  لكل (i = 0; i <= 2; i++)
    {
      إذا (UNLIKELY (fstat (i, &statbuf) == -1))
        أرجع crun_make_error (err, errno, "stat fd `%d`", i);
      إذا (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          إذا (UNLIKELY (dup2 (fd, i) == -1))
            أرجع crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  أرجع 0;
}

ساكن صحيح
uidgidmap_helper (حرف *helper, pid_t pid, ثابت حرف *map_file, libcrun_error_t *err)
{
#تعریف_کرو MAX_دلیلاں 20
  حرف pid_fmt[16];
  حرف *دلیلاں[MAX_دلیلاں + 1];
  حرف *اگلا;
  cleanup_free حرف *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  صحيح ret;

  دلیلاں[nargs++] = helper;

  ret = snprintf (pid_fmt, حجم (pid_fmt), "%d", pid);
  إذا (UNLIKELY (ret >= (صحيح) حجم (pid_fmt)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  دلیلاں[nargs++] = pid_fmt;
  اگلا = map_file_copy;
  طالما (nargs < MAX_دلیلاں)
    {
      حرف *p = strsep (&اگلا, " \n");
      إذا (اگلا == عدم)
        اكسر;
      دلیلاں[nargs++] = p;
    }
  دلیلاں[nargs++] = عدم;

  ret = لگاتار_process (دلیلاں, err);
  إذا (ret < 0)
    أرجع ret;
  إذا (ret > 0)
    أرجع crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  أرجع 0;
}

ساكن صحيح
newgidmap (pid_t pid, ثابت حرف *map_file, libcrun_error_t *err)
{
  أرجع uidgidmap_helper ("newgidmap", pid, map_file, err);
}

ساكن صحيح
newuidmap (pid_t pid, ثابت حرف *map_file, libcrun_error_t *err)
{
  أرجع uidgidmap_helper ("newuidmap", pid, map_file, err);
}

ساكن صحيح
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_close صحيح fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = عارضی_FAILURE_RETRY (اكتب (fd, "deny", 4));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_نجی_انکڑے (container)->deny_setgroups = true;
  أرجع 0;
}

ساكن صحيح
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *content = عدم;

  إذا (get_نجی_انکڑے (container)->deny_setgroups)
    أرجع 0;

  إذا (container->container_def->annotations)
    {
      ثابت حرف *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = لبھو_annotation (container, "run.oci.keep_original_groups");
      إذا (annotation)
        أرجع strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close صحيح fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    إذا (fd < 0)
      ret = fd;
    وإلا
      ret = read_all_fd (fd, "self/setgroups", &content, عدم, err);
  }
  إذا (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      إذا (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          أرجع 1;
        }
      أرجع ret;
    }

  أرجع لڑی_مقابلہ (content, "deny", 4) == 0 ? 0 : 1;
}

صحيح
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = عدم;
  size_t additional_gids_len = 0;
  صحيح can_do_setgroups;
  صحيح ret;

  إذا (process != عدم && process->user != عدم)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  إذا (UNLIKELY (can_do_setgroups < 0))
    أرجع can_do_setgroups;

  إذا (can_do_setgroups == 0)
    {
      ثابت حرف *annotation;
      size_t i;

      annotation = لبھو_annotation (container, "run.oci.keep_original_groups");
      إذا (annotation && strcmp (annotation, "0") != 0)
        أرجع 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      لكل (i = 0; i < additional_gids_len; i++)
        إذا (additional_gids[i] != process->user->gid)
          أرجع crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (صحيح) additional_gids[i]);
      أرجع 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "setgroups");

  أرجع 0;
}

صحيح
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  إذا (get_نجی_انکڑے (container)->unshare_cgroupns)
    {
      صحيح ret = unshare (CLONE_NEWCGROUP);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (errno != EINVAL)
            أرجع crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  أرجع 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
صحيح
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  إذا (len != 1)
    أرجع 0;

  إذا (mappings[0]->ناپ != 1)
    أرجع 0;

  إذا (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    أرجع 0;

  أرجع 1;
}

ساكن صحيح
write_id_map (libcrun_container_t *container, pid_t pid, ثابت حرف *map_name,
              صحيح (*idmap_helper) (pid_t, ثابت حرف *, libcrun_error_t *), ثابت حرف *helper_name,
              bool warn_on_failure, ثابت حرف *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  صحيح ret = 0;

  إذا (container->host_uid)
    ret = idmap_helper (pid, map, err);
  إذا (container->host_uid == 0 || ret < 0)
    {
      cleanup_close صحيح fd = -1;

      إذا (ret < 0)
        {
          إذا (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      إذا (UNLIKELY (fd < 0))
        أرجع fd;

      ret = محفوظ_write (fd, map_name, map, map_len, err);
      إذا (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free حرف *single_mapping = عدم;
          crun_error_release (err);

          إذا (! skip_if_setgroups_denied || ! get_نجی_انکڑے (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          إذا (UNLIKELY (fd < 0))
            أرجع fd;

          ret = محفوظ_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  أرجع ret;
}

صحيح
libcrun_سیٹ_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free حرف *uid_map = عدم;
  cleanup_free حرف *gid_map = عدم;
  size_t uid_map_len = 0, gid_map_len = 0;
  صحيح ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  إذا ((get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWUSER) == 0)
    أرجع 0;

  إذا (! def || ! def->linux)
    أرجع 0;

  إذا (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      uid_map_len = (size_t) ret;
      إذا (uid_map == عدم)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  إذا (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      gid_map_len = (size_t) ret;
      إذا (gid_map == عدم)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

#تعریف_کرو CAP_منزل_نقاب_0(x) (1L << ((x) & 31))
#تعریف_کرو CAP_منزل_نقاب_1(x) CAP_منزل_نقاب_0 (x - 32)

بنية all_caps_s
{
  بنانشانی لما effective[2];
  بنانشانی لما permitted[2];
  بنانشانی لما inheritable[2];
  بنانشانی لما ambient[2];
  بنانشانی لما bounding[2];
};

ساكن صحيح
has_cap_on (صحيح cap, لما بنانشانی *caps)
{
  إذا (cap < 32)
    أرجع CAP_منزل_نقاب_0 (cap) & caps[0];
  أرجع (CAP_منزل_نقاب_1 (cap) & caps[1]);
}

ساكن بنانشانی لما cap_آخری_cap;

صحيح
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  صحيح ret;
  حرف بفر[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  إذا (fd < 0)
    أرجع fd;
  ret = عارضی_FAILURE_RETRY (اقرأ (fd, بفر, حجم (بفر) - 1));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  بفر[ret] = '\0';

  errno = 0;
  cap_آخری_cap = strtoul (بفر, عدم, 10);
  إذا (errno != 0)
    أرجع crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", بفر);
  أرجع 0;
}

ساكن صحيح
سیٹ_required_caps (بنية all_caps_s *caps, uid_t uid, gid_t gid, صحيح no_new_privs, bool no_exec, libcrun_error_t *err)
{
#جے_تعریف_شدہ HAVE_CAP
  بنانشانی لما cap;
  صحيح ret;
  بنية __user_cap_header_struct hdr = { LINUX_CAPABILITY_نسخہ_3, 0 };
  بنية __user_cap_data_struct انکڑے[2] = { { 0 } };

  إذا (cap_آخری_cap == 0)
    أرجع crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  لكل (cap = 0; cap <= cap_آخری_cap; cap++)
    إذا (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        إذا (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          أرجع crun_make_error (err, errno, "prctl drop bounding");
      }

  انکڑے[0].effective = caps->effective[0];
  انکڑے[1].effective = caps->effective[1];
  انکڑے[0].inheritable = caps->inheritable[0];
  انکڑے[1].inheritable = caps->inheritable[1];
  انکڑے[0].permitted = caps->permitted[0];
  انکڑے[1].permitted = caps->permitted[1];

  ret = prctl (PR_سیٹ_KEEPCAPS, 1, 0, 0, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  إذا (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    أرجع crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, انکڑے);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  إذا (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    أرجع crun_make_error (err, errno, "prctl reset ambient");

  لكل (cap = 0; cap <= cap_آخری_cap; cap++)
    إذا (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        إذا (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          أرجع crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  إذا (no_exec)
    {
      ret = prctl (PR_سیٹ_KEEPCAPS, 0, 0, 0, 0);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "prctl reset KEEPCAPS");

      إذا (uid != 0)
        {
          انکڑے[0].permitted = انکڑے[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          لكل (cap = 0; cap <= cap_آخری_cap; cap++)
            إذا (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_سیٹ, cap, 0, 0) > 0)
              انکڑے[cap / 32].permitted |= CAP_منزل_نقاب_0 (cap);
#  endif
          انکڑے[0].effective = انکڑے[0].permitted;
          انکڑے[1].effective = انکڑے[1].permitted;

          ret = capset (&hdr, انکڑے);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "capset");
        }
    }
#جے_انت

  إذا (no_new_privs)
    إذا (UNLIKELY (prctl (PR_سیٹ_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      أرجع crun_make_error (err, errno, "prctl set no new privs");

  أرجع 0;
}

ساكن صحيح
read_caps (بنانشانی لما caps[2], حرف **اقدار, size_t len, libcrun_error_t *err)
{
#جے_تعریف_شدہ HAVE_CAP
  size_t i;
  لكل (i = 0; i < len; i++)
    {
      cap_قدر_t cap;
      إذا (cap_from_name (اقدار[i], &cap) < 0)
        أرجع crun_make_error (err, 0, "unknown capability `%s`", اقدار[i]);

      إذا (cap < 32)
        caps[0] |= CAP_منزل_نقاب_0 (cap);
      وإلا
        caps[1] |= CAP_منزل_نقاب_1 (cap);
    }
#وإلا
  caps[0] = 0;
  caps[1] = 0;
#جے_انت
  أرجع 0;
}

صحيح
libcrun_سیٹ_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  إذا (proc->selinux_label)
    أرجع سیٹ_selinux_label (container, proc->selinux_label, now, err);

  أرجع 0;
}

صحيح
libcrun_سیٹ_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  إذا (proc->apparmor_profile)
    أرجع سیٹ_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  أرجع 0;
}

صحيح
libcrun_سیٹ_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  صحيح no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  بنية all_caps_s caps = {};
  صحيح ret;

  إذا (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع سیٹ_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

بنية rlimit_s
{
  ثابت حرف *name;
  صحيح قدر;
};

بنية rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
                              { "RLIMIT_CORE", RLIMIT_CORE },
                              { "RLIMIT_CPU", RLIMIT_CPU },
                              { "RLIMIT_DATA", RLIMIT_انکڑے },
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
                              { عدم, 0 } };

ساكن صحيح
get_rlimit_resource (ثابت حرف *name)
{
  بنية rlimit_s *it;
  لكل (it = rlimits; it->name; it++)
    إذا (strcmp (it->name, name) == 0)
      أرجع it->قدر;
  أرجع -1;
}

صحيح
libcrun_سیٹ_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  لكل (i = 0; i < len; i++)
    {
      لكل (j = i + 1; j < len; j++)
        {
          إذا (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            أرجع crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  لكل (i = 0; i < len; i++)
    {
      بنية rlimit حد_بندی;
      حرف *type = new_rlimits[i]->type;
      صحيح resource = get_rlimit_resource (type);
      إذا (UNLIKELY (resource < 0))
        أرجع crun_make_error (err, 0, "invalid rlimit `%s`", type);
      حد_بندی.rlim_cur = new_rlimits[i]->soft;
      حد_بندی.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (بنانشانی لما لما) حد_بندی.rlim_cur,
                     (بنانشانی لما لما) حد_بندی.rlim_max);
      إذا (UNLIKELY (setrlimit (resource, &حد_بندی) < 0))
        أرجع crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  أرجع 0;
}

صحيح
libcrun_سیٹ_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح has_uts = get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWUTS;
  صحيح ret;
  إذا (def->hostname == عدم || def->hostname[0] == '\0')
    أرجع 0;
  إذا (! has_uts)
    أرجع crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, لڑی_لمبائی (def->hostname));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "sethostname");
  أرجع 0;
}

صحيح
libcrun_سیٹ_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح has_uts = get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWUTS;
  صحيح ret;
  إذا (is_خالی_تھاواں_string (def->domainname))
    أرجع 0;
  إذا (! has_uts)
    أرجع crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, لڑی_لمبائی (def->domainname));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "setdomainname");
  أرجع 0;
}

صحيح
libcrun_سیٹ_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحيح fd = -1;
  صحيح ret;
  حرف oom_بفر[16];
  إذا (def->process == عدم || ! def->process->oom_score_adj_present)
    أرجع 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_بفر, حجم (oom_بفر), "%i", def->process->oom_score_adj);
  إذا (UNLIKELY (ret >= (صحيح) حجم (oom_بفر)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  إذا (fd < 0)
    أرجع fd;
  ret = عارضی_FAILURE_RETRY (اكتب (fd, oom_بفر, لڑی_لمبائی (oom_بفر)));
  إذا (ret < 0)
    أرجع crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  أرجع 0;
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
  عدم
};

ساكن صحيح
validate_sysctl (ثابت حرف *original_کلید, ثابت حرف *original_قدر, ثابت حرف *name, بنانشانی لما namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ثابت حرف *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  إذا (name[0] == '/' || رستہ_has_dot_dot_component (name))
    أرجع crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_کلید);

  إذا (has_اگیتر (name, "fs/mqueue/"))
    {
      إذا (namespaces_created & CLONE_NEWIPC)
        أرجع 0;

      namespace = "IPC";
      اذهب fail;
    }

  إذا (has_اگیتر (name, "kernel/"))
    {
      size_t i;

      لكل (i = 0; sysctlRequiringIPC[i]; i++)
        إذا (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            إذا (namespaces_created & CLONE_NEWIPC)
              أرجع 0;

            namespace = "IPC";
            اذهب fail;
          }

      إذا (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          إذا (! is_خالی_تھاواں_string (def->domainname) && (strcmp (original_قدر, def->domainname) != 0))
            أرجع crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_کلید);

          إذا (namespaces_created & CLONE_NEWUTS)
            أرجع 0;

          namespace = "UTS";
          اذهب fail;
        }

      إذا (strcmp (name, "kernel/hostname") == 0)
        أرجع crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_کلید);
    }
  إذا (has_اگیتر (name, "net/"))
    {
      إذا (namespaces_created & CLONE_NEWNET)
        أرجع 0;

      namespace = "network";
      اذهب fail;
    }

  أرجع crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_کلید);

fail:
  أرجع crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_کلید, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
ساكن حرف *
sysctl_error_reason (ثابت حرف *name, صحيح namespaces_created, صحيح errno_)
{

  إذا (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    أرجع xstrdup ("are all the IDs mapped in the user namespace?");

  أرجع عدم;
}

صحيح
libcrun_سیٹ_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close صحيح dirfd = -1;
  بنانشانی لما namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  إذا (def->linux == عدم || def->linux->sysctl == عدم || def->linux->sysctl->len == 0)
    أرجع 0;

  لكل (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحيح قدر;

      قدر = libcrun_لبھو_namespace (def->linux->namespaces[i]->type);
      إذا (UNLIKELY (قدر < 0))
        أرجع crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= قدر;
    }

  get_نجی_انکڑے (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_رستہ, err);
  إذا (UNLIKELY (dirfd < 0))
    أرجع dirfd;

  لكل (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free حرف *name = عدم;
      cleanup_close صحيح fd = -1;
      صحيح ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      إذا (def->linux->sysctl->کلیدیں[i] == عدم || def->linux->sysctl->اقدار[i] == عدم)
        أرجع crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_کلید_منزل_proc_رستہ (def->linux->sysctl->کلیدیں[i]);

      ret = validate_sysctl (def->linux->sysctl->کلیدیں[i], def->linux->sysctl->اقدار[i], name, namespaces_created, def, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      إذا (UNLIKELY (fd < 0))
        أرجع crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = عارضی_FAILURE_RETRY (اكتب (fd, def->linux->sysctl->اقدار[i], لڑی_لمبائی (def->linux->sysctl->اقدار[i])));
      إذا (UNLIKELY (ret < 0))
        {
          صحيح saved_errno = errno;
          cleanup_free حرف *reason = عدم;

          reason = sysctl_error_reason (def->linux->sysctl->کلیدیں[i], namespaces_created, saved_errno);
          أرجع crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  أرجع 0;
}

ساكن صحيح
open_terminal (حرف **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  uid_t uid = 0;
  صحيح ret;

  إذا (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = libcrun_سیٹ_stdio (fd, *pty, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  أرجع ret;
}

حرف *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  أرجع get_نجی_انکڑے (container)->external_descriptors;
}

صحيح
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  ثابت حرف *buf = عدم;
  json_gen_ctx *gen = عدم;
  size_t buf_len;
  صحيح ret;
  صحيح i;

  إذا (! json_gen_init (&gen, عدم))
    أرجع crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  إذا (UNLIKELY (ret != json_gen_حالت_ok))
    اذهب json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  لكل (i = 0; i < 3; i++)
    {
      proc_fd_رستہ_t fd_رستہ;
      حرف link_رستہ[PATH_MAX];

      get_proc_self_fd_رستہ (fd_رستہ, i);

      ret = readlink (fd_رستہ, link_رستہ, PATH_MAX - 1);
      إذا (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          إذا (errno == ENOENT)
            {
              strcpy (link_رستہ, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          وإلا
            {
              json_gen_free (gen);
              أرجع crun_make_error (err, errno, "readlink `%s`", fd_رستہ);
            }
        }
      link_رستہ[ret] = 0;

      ret = json_gen_string (gen, link_رستہ, ret);
      إذا (UNLIKELY (ret != json_gen_حالت_ok))
        اذهب json_error;
    }

  ret = json_gen_array_close (gen);
  إذا (UNLIKELY (ret != json_gen_حالت_ok))
    اذهب json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  إذا (UNLIKELY (ret != json_gen_حالت_ok))
    اذهب json_error;

  إذا (buf)
    {
      حرف *b = xmalloc (buf_len + 1);
      حافظہ_نقل (b, buf, buf_len);
      b[buf_len] = '\0';
      get_نجی_انکڑے (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  أرجع 0;

json_error:
  إذا (gen)
    json_gen_free (gen);
  أرجع json_gen_error_منزل_crun_error (ret, err);
}

صحيح
libcrun_سیٹ_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free حرف *pty = عدم;
  cleanup_close صحيح fd = -1;
  صحيح ret;

  إذا (def->process == عدم || ! def->process->terminal)
    أرجع 0;

  fd = open_terminal (&pty, def->process, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  إذا (def->process->console_ناپ)
    {
      ret = libcrun_terminal_تیاری_ناپ (0, def->process->console_ناپ->اچائی, def->process->console_ناپ->چوڑائی, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (get_نجی_انکڑے (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", عدم, MS_BIND, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      ret = unlink ("/dev/console");
      إذا (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          إذا (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", عدم, MS_BIND, عدم, LABEL_MOUNT, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;

              أرجع get_and_reset (&fd);
            }
          أرجع crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  أرجع get_and_reset (&fd);
}

ساكن bool
read_error_from_sync_socket (صحيح sync_socket_fd, صحيح *error, حرف **str)
{
  cleanup_free حرف *b = عدم;
  size_t ناپ;
  صحيح کوڈ = 0;
  صحيح ret;

  إذا (*error == 0)
    {
      ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_fd, &کوڈ, حجم (کوڈ)));
      إذا (UNLIKELY (ret != (صحيح) حجم (کوڈ)))
        أرجع false;
      *error = کوڈ;
    }

  ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_fd, &ناپ, حجم (ناپ)));
  إذا (UNLIKELY (ret != (صحيح) حجم (ناپ)))
    أرجع false;

  إذا (ناپ == 0)
    أرجع false;

  إذا (ناپ > 1024)
    ناپ = 1024;

  b = xmalloc (ناپ + 1);
  ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_fd, b, ناپ));
  إذا (UNLIKELY (ret < 0))
    أرجع false;

  b[ret] = '\0';

  *str = b;
  b = عدم;
  أرجع true;
}

ساكن bool
send_error_منزل_sync_socket (صحيح sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t ناپ;
  صحيح ret;
  صحيح کوڈ;
  حرف *پیغام;

  إذا (err == عدم || *err == عدم)
    أرجع false;

  کوڈ = crun_error_get_errno (err);

  إذا (has_fd)
    {
      /* dummy terminal fd.  */
      ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_fd, "1", 1));
      إذا (UNLIKELY (ret < 0))
        أرجع false;
    }

  ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_fd, &کوڈ, حجم (کوڈ)));
  إذا (UNLIKELY (ret < 0))
    أرجع false;

  پیغام = (*err)->پیغام;
  ناپ = لڑی_لمبائی (پیغام) + 1;
  ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_fd, &ناپ, حجم (ناپ)));
  إذا (UNLIKELY (ret < 0))
    أرجع false;

  ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_fd, پیغام, ناپ));
  إذا (UNLIKELY (ret < 0))
    أرجع false;

  أرجع true;
}

ساكن مضمن صحيح
send_success_منزل_sync_socket (صحيح sync_socket, libcrun_error_t *err)
{
  ثابت صحيح success = 0;
  صحيح ret;

  ret = عارضی_FAILURE_RETRY (اكتب (sync_socket, &success, حجم (success)));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "write to sync socket");

  أرجع 0;
}

ساكن __attribute__ ((noreturn)) فراغ
send_error_منزل_sync_socket_and_die (صحيح sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  حرف *پیغام;

  إذا (err == عدم || *err == عدم)
    محفوظ_exit (EXIT_FAILURE);

  إذا (! send_error_منزل_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      پیغام = (*err)->پیغام;
      libcrun_fail_with_error (errno, "%s", پیغام);
    }
  محفوظ_exit (EXIT_FAILURE);
}

ساكن صحيح
expect_success_from_sync_socket (صحيح sync_fd, libcrun_error_t *err)
{
  cleanup_free حرف *err_str = عدم;
  صحيح res = 1;
  صحيح ret;

  ret = عارضی_FAILURE_RETRY (اقرأ (sync_fd, &res, حجم (res)));
  إذا (UNLIKELY (ret != حجم (res)))
    أرجع crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  إذا (res == 0)
    أرجع 0;

  إذا (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      إذا (! is_خالی_تھاواں_string (err_str))
        أرجع crun_make_error (err, res, "%s", err_str);
    }

  أرجع crun_make_error (err, res, "read from sync socket");
}

ساكن صحيح
join_namespaces (runtime_spec_schema_config_schema *def, صحيح *namespaces_منزل_join, صحيح n_namespaces_منزل_join,
                 صحيح *namespaces_منزل_join_اشاریہ, bool ignore_join_errors, libcrun_error_t *err)
{
  صحيح ret;
  صحيح i;

  لكل (i = 0; i < n_namespaces_منزل_join; i++)
    {
      cleanup_free حرف *cwd = عدم;
      صحيح orig_اشاریہ = namespaces_منزل_join_اشاریہ[i];
      صحيح قدر;

      إذا (namespaces_منزل_join[i] < 0)
        استمر;

      /* Skip the user namespace.  */
      قدر = libcrun_لبھو_namespace (def->linux->namespaces[orig_اشاریہ]->type);
      إذا (قدر == CLONE_NEWUSER)
        استمر;

      إذا (قدر == CLONE_NEWNS)
        {
          cwd = getcwd (عدم, 0);
          إذا (UNLIKELY (cwd == عدم))
            أرجع crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_اشاریہ]->type, def->linux->namespaces[orig_اشاریہ]->رستہ);
      ret = setns (namespaces_منزل_join[i], قدر);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (ignore_join_errors)
            استمر;
          أرجع crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_اشاریہ]->رستہ);
        }

      close_and_reset (&namespaces_منزل_join[i]);

      إذا (قدر == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  أرجع 0;
}

#تعریف_کرو MAX_NAMESPACES 10

بنية init_حالت_s
{
  /* fd to the namespace to join.  */
  صحيح fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  صحيح اشاریہ[MAX_NAMESPACES];
  /* CLONE_* value.  */
  صحيح قدر[MAX_NAMESPACES];
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
  صحيح userns_اشاریہ;
  /* def->linux->namespaces userns.  */
  صحيح userns_اشاریہ_origin;

  /* All namespaces created/joined by the container.  */
  صحيح all_namespaces;

  /* What namespaces are still missing to be created.  */
  صحيح namespaces_منزل_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  صحيح idx_pidns_منزل_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  صحيح idx_timens_منزل_join_immediately;
};

فراغ
cleanup_free_init_statusp (بنية init_حالت_s *ns)
{
  size_t i;

  إذا (ns == عدم)
    أرجع;

  لكل (i = 0; i < ns->fd_len; i++)
    عارضی_FAILURE_RETRY (close (ns->fd[i]));
}

ساكن صحيح
configure_init_حالت (بنية init_حالت_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  لكل (i = 0; i < MAX_NAMESPACES + 1; i++)
    ns->fd[i] = -1;

  ns->fd_len = 0;
  ns->all_namespaces = 0;
  ns->namespaces_منزل_unshare = 0;
  ns->join_pidns = false;
  ns->join_ipcns = false;
  ns->must_fork = false;
  ns->can_delay_pidns_fork = false;
  ns->delayed_userns_create = false;
  ns->userns_اشاریہ = -1;
  ns->userns_اشاریہ_origin = -1;
  ns->idx_pidns_منزل_join_immediately = -1;
  ns->idx_timens_منزل_join_immediately = -1;

  لكل (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحيح قدر = libcrun_لبھو_namespace (def->linux->namespaces[i]->type);
      إذا (UNLIKELY (قدر < 0))
        أرجع crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      إذا (ns->all_namespaces & قدر)
        أرجع crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= قدر;

      إذا (def->linux->namespaces[i]->رستہ == عدم)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_منزل_unshare |= قدر;
        }
      وإلا
        {
          صحيح fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->رستہ);

          إذا (ns->fd_len >= MAX_NAMESPACES)
            أرجع crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->رستہ, O_RDONLY | O_CLOEXEC);
          إذا (UNLIKELY (fd < 0))
            أرجع crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->رستہ);

          إذا (قدر == CLONE_NEWUSER)
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

  إذا (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_منزل_unshare |= CLONE_NEWUSER;
    }

  أرجع 0;
}

/* Detect if root is available in the container.  */
ساكن bool
جڑ_mapped_وچ_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  لكل (i = 0; i < len; i++)
    إذا (mappings[i]->container_id == 0)
      أرجع true;

  أرجع false;
}

ساكن بنية libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  بنية libcrun_fd_map *dev_fds = get_نجی_انکڑے (container)->dev_fds;

  إذا (dev_fds == عدم)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_نجی_انکڑے (container)->dev_fds = dev_fds;
    }
  أرجع dev_fds;
}

#تعریف_کرو عدد_NEEDED_DEVS (حجم (needed_devs) / حجم (needed_devs[0]) - 1)

ساكن بنية libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  بنية libcrun_fd_map *fds = get_نجی_انکڑے (container)->needed_devs_fds;

  إذا (fds == عدم)
    {
      fds = make_libcrun_fd_map (عدد_NEEDED_DEVS);
      get_نجی_انکڑے (container)->needed_devs_fds = fds;
    }
  أرجع fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  إذا (recursive)
    *recursive = false;

  إذا (src_nofollow != عدم)
    *src_nofollow = false;

  لكل (i = 0; i < mnt->options_len; i++)
    {
      إذا (mnt->options[i] == عدم)
        استمر;

      إذا (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          إذا (src_nofollow == عدم || *src_nofollow)
            اكسر;
        }
      إذا (strcmp (mnt->options[i], "rbind") == 0)
        {
          إذا (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          إذا (src_nofollow == عدم || *src_nofollow)
            اكسر;
        }
      إذا (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  أرجع ret;
}

ساكن حرف *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  لكل (i = 0; i < mnt->options_len; i++)
    {
      إذا (mnt->options[i] == عدم)
        استمر;

      إذا (has_اگیتر (mnt->options[i], "idmap"))
        {
          *recursive = false;
          أرجع mnt->options[i];
        }
      إذا (has_اگیتر (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          أرجع mnt->options[i];
        }
    }
  أرجع عدم;
}

ساكن صحيح
open_mount_type (ثابت حرف *type, صحيح *باہرلا_fd, libcrun_error_t *err)
{
  cleanup_close صحيح fsopen_fd = -1;
  cleanup_close صحيح newfs_fd = -1;
  صحيح ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  إذا (UNLIKELY (fsopen_fd < 0))
    أرجع crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, عدم, عدم, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  إذا (UNLIKELY (newfs_fd < 0))
    أرجع crun_make_error (err, errno, "fsmount `%s`", type);

  *باہرلا_fd = get_and_reset (&newfs_fd);
  أرجع 0;
}

ساكن صحيح
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, صحيح *باہرلا_fd, bool *has_mappings_باہرلا, libcrun_error_t *err)
{
  cleanup_close صحيح newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  بنية mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close صحيح fd = -1;
  ثابت حرف *idmap_option;
  bool recursive = false;
  ثابت حرف *options = عدم;
  bool has_mappings;
  صحيح ret;
  حرف *extra_پیغام = "";
  bool nofollow = false;

  *باہرلا_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != عدم);
  إذا (has_mappings_باہرلا)
    *has_mappings_باہرلا = has_mappings;
  إذا (! has_mappings)
    أرجع 0;

  إذا ((mnt->uid_mappings == عدم) != (mnt->gid_mappings == عدم))
    أرجع crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  إذا (idmap_option)
    {
      options = strchr (idmap_option, '=');
      إذا (options)
        {
          /* Skip the '=' itself.  */
          options++;
          إذا (options[0] == '\0')
            options = عدم;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  إذا (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  إذا (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_درخت (AT_FDCWD, mnt->سروت, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      إذا (UNLIKELY (newfs_fd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", mnt->سروت);
    }
  وإلا
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_خالی_تھاواں_رستہ | (recursive ? AT_RECURSIVE : 0), &attr);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == EINVAL)
        {
          extra_پیغام = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      أرجع crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_پیغام);
    }

  *باہرلا_fd = get_and_reset (&newfs_fd);
  أرجع 0;
}

ساكن uid_t
get_id_وچ_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  إذا (! def || ! def->linux)
    اذهب باہر_نکلو;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  إذا (! mappings)
    اذهب باہر_نکلو;

  لكل (i = 0; i < len; i++)
    {
      إذا (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->ناپ)
        أرجع id - mappings[i]->container_id + mappings[i]->host_id;
    }
باہر_نکلو:
  أرجع is_uid ? get_حد_توں_ودھ_uid () : get_حد_توں_ودھ_gid ();
}

ساكن صحيح
precreate_device (libcrun_container_t *container, صحيح devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  حرف name[64];
  mode_t type;
  dev_t dev;
  صحيح ret;

  ret = snprintf (name, حجم (name), "%zu", i);
  إذا (UNLIKELY (ret >= (صحيح) حجم (name)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_منوال | type, dev);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "mknod `%s`", device->رستہ);

  uid = get_id_وچ_user_namespace (device->uid, true, def);
  gid = get_id_وچ_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "chown `%s`", device->رستہ);

  أرجع get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

ساكن صحيح
send_mounts (صحيح sync_socket_host, بنية libcrun_fd_map *fds, size_t how_many, size_t کل, libcrun_error_t *err)
{
  size_t i;
  صحيح ret;

  ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_host, &how_many, حجم (how_many)));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "write to sync socket");

  لكل (i = 0; i < کل; i++)
    {
      إذا (fds->fds[i] >= 0)
        {
          ret = send_fd_منزل_socket_with_payload (sync_socket_host, fds->fds[i], (حرف *) &i, حجم (i), err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }
  أرجع 0;
}

ساكن صحيح
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, صحيح sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map بنية libcrun_fd_map *mount_fds = عدم;
  bool has_userns = (get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free حرف *rootfs = عدم;
  size_t how_many = 0;
  size_t i;
  صحيح ret;

  إذا (def->mounts_len == 0)
    أرجع 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  إذا (def->جڑ && def->جڑ->رستہ)
    rootfs = realpath (def->جڑ->رستہ, عدم);

  إذا (! has_userns)
    {
      صحيح is_وچ_userns = check_running_وچ_user_namespace (err);
      إذا (UNLIKELY (is_وچ_userns < 0))
        أرجع is_وچ_userns;

      has_userns = is_وچ_userns > 0;
    }

  لكل (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      صحيح mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      إذا (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_رستہ_وچ_rootfs (rootfs, def->mounts[i]->سروت) == عدم)
        {
          بنانشانی لما پسار = 0;

          إذا (def->linux && def->linux->rootfs_پسار)
            پسار = get_mount_flags (def->linux->rootfs_پسار, 0, عدم, عدم, عدم, عدم) & ALL_PROPAGATIONS_NO_REC;
          إذا (پسار == 0)
            پسار = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->سروت, recursive, false, nofollow, پسار, err);
          إذا (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      إذا (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  أرجع send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

ساكن صحيح
prepare_and_send_dev_mounts (libcrun_container_t *container, صحيح sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map بنية libcrun_fd_map *dev_fds = عدم;
  bool has_userns = (get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close صحيح current_mountns = -1;
  cleanup_free حرف *state_dir = عدم;
  cleanup_free حرف *devs_رستہ = عدم;
  cleanup_close صحيح devs_mountfd = -1;
  cleanup_close صحيح targetfd = -1;
  ثابت حرف *context_type = عدم;
  ثابت حرف *label = عدم;
  size_t how_many = 0;
  size_t i;
  صحيح ret;
  // To track whether the namespace has been changed.
  bool ns_بدلیا = false;

  إذا (def->linux == عدم || def->linux->devices_len == 0)
    أرجع 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  إذا (! has_userns || is_خالی_تھاواں_string (container->context->id) || geteuid () > 0)
    أرجع send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_جڑ : عدم),
                                     (container->context ? container->context->id : عدم), err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&devs_رستہ, err, state_dir, "devs", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = mkdir (devs_رستہ, 0700);
  إذا (UNLIKELY (ret < 0 && errno != EEXIST))
    أرجع crun_make_error (err, errno, "mkdir `%s`", devs_رستہ);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  إذا (UNLIKELY (current_mountns < 0))
    أرجع current_mountns;

  ret = unshare (CLONE_NEWNS);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_بدلیا = true;

  ret = mount (عدم, "/", عدم, MS_REC | MS_PRIVATE, عدم);
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      اذهب restore_mountns;
    }

  إذا (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      إذا (UNLIKELY (context_type == عدم))
        {
          ret = -1;
          اذهب restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", عدم, context_type, label, عدم);
  إذا (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      اذهب restore_mountns;
    }

  targetfd = open (devs_رستہ, O_DIRECTORY | O_رستہ | O_CLOEXEC);
  إذا (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_رستہ);
      اذهب restore_mountns;
    }

  ret = fs_move_mount_منزل (devs_mountfd, targetfd, عدم);
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_رستہ);
      اذهب restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_رستہ | O_CLOEXEC);
  إذا (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_رستہ);
      اذهب restore_mountns;
    }

  لكل (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      إذا (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          استمر;
        }

      dev_fds->fds[i] = ret;

      إذا (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  إذا (ns_بدلیا && current_mountns >= 0)
    {
      صحيح setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      إذا (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          أرجع crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  أرجع ret;
}

ساكن صحيح
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, صحيح sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map بنية libcrun_fd_map *fds = عدم;
  bool has_userns = (get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (عدد_NEEDED_DEVS);

  إذا (! has_userns || geteuid () > 0)
    أرجع send_mounts (sync_socket_host, fds, 0, عدد_NEEDED_DEVS, err);

  لكل (i = 0; i < عدد_NEEDED_DEVS && needed_devs[i].رستہ; i++)
    {
      صحيح fd = syscall_open_درخت (AT_FDCWD, needed_devs[i].رستہ,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      إذا (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  أرجع send_mounts (sync_socket_host, fds, how_many, عدد_NEEDED_DEVS, err);
}

ساكن صحيح
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, صحيح sync_socket_host, libcrun_error_t *err)
{
  صحيح ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

ساكن صحيح
receive_mounts (بنية libcrun_fd_map *fds, صحيح sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  صحيح ret;

  إذا (fds->nfds == 0)
    أرجع 0;

  ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_container, &how_many, حجم (how_many)));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "read from sync socket");

  لكل (i = 0; i < how_many; i++)
    {
      size_t اشاریہ;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (حرف *) &اشاریہ, حجم (اشاریہ), err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (اشاریہ >= fds->nfds)
        أرجع crun_make_error (err, 0, "invalid mount data received");

      إذا (fds->fds[اشاریہ] >= 0)
        عارضی_FAILURE_RETRY (close (fds->fds[اشاریہ]));

      fds->fds[اشاریہ] = ret;
    }

  أرجع 0;
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
ساكن صحيح
سیٹ_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;

  إذا (! container->host_has_cap_sys_ptrace)
    أرجع 0;

  ret = prctl (PR_سیٹ_DUMPABLE, 0, 0, 0, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  أرجع 0;
}

ساكن صحيح
سیٹ_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  صحيح ret;

  إذا (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool جڑ_mapped = false;

      إذا (def->linux->uid_mappings_len != 0)
        {
          جڑ_mapped = جڑ_mapped_وچ_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          إذا (! جڑ_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      إذا (def->linux->gid_mappings_len != 0)
        {
          جڑ_mapped = جڑ_mapped_وچ_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          إذا (! جڑ_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  إذا (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    أرجع crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "setresgid to `%d`", gid);

  أرجع 0;
}

ساكن bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  أرجع mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, عدم, عدم);
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
ساكن صحيح
open_proc_mounts_وچ_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  بنية libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close صحيح socket_ماپا = -1;
  cleanup_close صحيح socket_بچہ = -1;
  cleanup_close صحيح جڑ_fd = -1;
  cleanup_close صحيح cwd_fd = -1;
  size_t i, missing = 0;
  صحيح saved_errno;
  صحيح sockets[2];
  pid_t pid;
  صحيح ret;

  إذا (def->جڑ == عدم || def->جڑ->رستہ == عدم)
    أرجع 0;

  لكل (i = 0; i < def->mounts_len; i++)
    إذا (is_proc_mount (def->mounts[i]))
      missing++;
  إذا (missing == 0)
    أرجع 1;

  جڑ_fd = open ("/", O_DIRECTORY | O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (جڑ_fd < 0))
    أرجع crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_رستہ | O_CLOEXEC);
  إذا (UNLIKELY (cwd_fd < 0))
    أرجع crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "socketpair");
  socket_ماپا = sockets[0];
  socket_بچہ = sockets[1];

  ret = chroot (def->جڑ->رستہ);
  إذا (UNLIKELY (ret < 0))
    أرجع 0;

  pid = fork ();
  saved_errno = errno;
  إذا (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      إذا (get_نجی_انکڑے (container)->unshare_flags & CLONE_NEWUSER)
        (فراغ) unshare (CLONE_NEWNS);

      لكل (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close صحيح fd = -1;

          إذا (! is_proc_mount (def->mounts[i]))
            استمر;

          fd = open_mount_دا_type (container, def->mounts[i], err);
          إذا (fd < 0)
            محفوظ_exit (EXIT_FAILURE);

          ret = send_fd_منزل_socket_with_payload (socket_بچہ, fd, (حرف *) &i, حجم (i), err);
          إذا (UNLIKELY (ret < 0))
            محفوظ_exit (EXIT_FAILURE);
        }
      محفوظ_exit (EXIT_SUCCESS);
    }

  إذا (UNLIKELY (fchdir (جڑ_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    أرجع crun_make_error (err, errno, "restore the root directory");

  إذا (UNLIKELY (pid < 0))
    أرجع crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_بچہ);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  طالما (missing > 0)
    {
      libcrun_error_t tmp_err = عدم;

      ret = receive_fd_from_socket_with_payload (socket_ماپا, (حرف *) &i, حجم (i), &tmp_err);
      إذا (ret < 0)
        {
          crun_error_release (&tmp_err);
          اكسر;
        }
      إذا (i >= mount_fds->nfds)
        {
          عارضی_FAILURE_RETRY (close (ret));
          اكسر;
        }

      إذا (mount_fds->fds[i] >= 0)
        عارضی_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, عدم, 0);

  أرجع missing == 0 ? 1 : 0;
}

ساكن صحيح
init_container (libcrun_container_t *container, صحيح sync_socket_container, بنية init_حالت_s *init_حالت,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  بنية libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close صحيح timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  صحيح ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = سیٹ_init_non_dumpable (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (init_حالت->idx_pidns_منزل_join_immediately >= 0 || init_حالت->idx_timens_منزل_join_immediately >= 0)
    {
      pid_t new_pid;

      إذا (init_حالت->idx_pidns_منزل_join_immediately >= 0)
        {
          ret = setns (init_حالت->fd[init_حالت->idx_pidns_منزل_join_immediately], CLONE_NEWPID);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_حالت->fd[init_حالت->idx_pidns_منزل_join_immediately]);
        }

      إذا (init_حالت->idx_timens_منزل_join_immediately >= 0)
        {
          ret = setns (init_حالت->fd[init_حالت->idx_timens_منزل_join_immediately], CLONE_NEWTIME);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_حالت->fd[init_حالت->idx_timens_منزل_join_immediately]);
        }

      إذا (init_حالت->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_وچ_pidns (container, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          delayed_pidns_fork = ret > 0;
        }

      إذا (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_منزل_sync_socket (sync_socket_container, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_container, &new_pid, حجم (new_pid)));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "write to sync socket");
        }
      وإلا
        {
          new_pid = fork ();
          إذا (UNLIKELY (new_pid < 0))
            أرجع crun_make_error (err, errno, "fork");
        }

      إذا (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_منزل_sync_socket (sync_socket_container, err);
          إذا (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_container, &new_pid, حجم (new_pid)));
          إذا (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          محفوظ_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = libcrun_سیٹ_oom (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (init_حالت->fd_len > 0)
    {
      ret = join_namespaces (def, init_حالت->fd, init_حالت->fd_len, init_حالت->اشاریہ, true, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  إذا ((init_حالت->all_namespaces & CLONE_NEWUSER) && (init_حالت->join_pidns || init_حالت->join_ipcns))
    {
      لكل (i = 0; i < def->mounts_len; i++)
        {
          صحيح fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          إذا (def->mounts[i]->type == عدم)
            استمر;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          إذا (init_حالت->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->سروت, عدم, عدم, عدم);
          إذا (init_حالت->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->سروت, عدم, عدم, عدم);

          إذا (fd >= 0)
            {
              إذا (mount_fds->fds[i] >= 0)
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
  إذا (def->linux->ویلا_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      إذا (UNLIKELY (timens_offsets_fd < 0))
        أرجع timens_offsets_fd;
    }

  إذا (init_حالت->all_namespaces & CLONE_NEWUSER)
    {
      إذا (init_حالت->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "unshare user namespace");

          init_حالت->namespaces_منزل_unshare &= ~CLONE_NEWUSER;

          ret = send_success_منزل_sync_socket (sync_socket_container, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      إذا (init_حالت->userns_اشاریہ < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_حالت->fd[init_حالت->userns_اشاریہ], CLONE_NEWUSER);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_حالت->userns_اشاریہ_origin]->رستہ);
        }

      ret = سیٹ_id_init (container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = سیٹ_init_non_dumpable (container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = join_namespaces (def, init_حالت->fd, init_حالت->fd_len, init_حالت->اشاریہ, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (init_حالت->namespaces_منزل_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_حالت->namespaces_منزل_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "unshare");
    }

  إذا (def->linux->ویلا_offsets)
    {
      حرف fmt_بفر[128];
      صحيح fd = timens_offsets_fd;

      إذا (def->linux->ویلا_offsets->boottime)
        {
          ret = snprintf (fmt_بفر, حجم (fmt_بفر), "boottime %" PRIi64 " %" PRIu32, def->linux->ویلا_offsets->boottime->secs, def->linux->ویلا_offsets->boottime->nanosecs);
          إذا (UNLIKELY (ret >= (صحيح) حجم (fmt_بفر)))
            أرجع crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (لما لما صحيح) def->linux->ویلا_offsets->boottime->secs, def->linux->ویلا_offsets->boottime->nanosecs);
          ret = اكتب (fd, fmt_بفر, لڑی_لمبائی (fmt_بفر));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      إذا (def->linux->ویلا_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (لما لما صحيح) def->linux->ویلا_offsets->monotonic->secs, def->linux->ویلا_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_بفر, حجم (fmt_بفر), "monotonic %" PRIi64 " %" PRIu32, def->linux->ویلا_offsets->monotonic->secs, def->linux->ویلا_offsets->monotonic->nanosecs);
          إذا (UNLIKELY (ret >= (صحيح) حجم (fmt_بفر)))
            أرجع crun_make_error (err, 0, "internal error: static buffer too small");

          ret = اكتب (fd, fmt_بفر, لڑی_لمبائی (fmt_بفر));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  إذا (init_حالت->must_fork)
    {
      إذا (init_حالت->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_وچ_pidns (container, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          delayed_pidns_fork = ret > 0;
        }

      إذا (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          إذا (UNLIKELY (pid_container < 0))
            أرجع crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      إذا (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_منزل_sync_socket (sync_socket_container, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          ret = عارضی_FAILURE_RETRY (اكتب (sync_socket_container, &pid_container, حجم (pid_container)));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "write to sync socket");

          إذا (pid_container)
            محفوظ_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  get_نجی_انکڑے (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_منزل_sync_socket (sync_socket_container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  أرجع get_نجی_انکڑے (container)->delayed_pidns_fork;
}

صحيح
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close صحيح client_fd = -1;
  cleanup_close صحيح pidfd = -1;
  ثابت حرف *v;

  v = لبھو_annotation (container, "run.oci.pidfd_receiver");
  إذا (v == عدم)
    أرجع 0;

  pidfd = syscall_pidfd_open (pid, 0);
  إذا (UNLIKELY (pidfd < 0))
    أرجع crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  إذا (UNLIKELY (client_fd < 0))
    أرجع client_fd;

  أرجع send_fd_منزل_socket (client_fd, pidfd, err);
}

ساكن bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  إذا (process == عدم || process->exec_cpu_affinity == عدم)
    أرجع false;
  أرجع (! is_خالی_تھاواں_string (process->exec_cpu_affinity->initial))
         || (! is_خالی_تھاواں_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_لگاتار_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, فراغ *دلیلاں,
                             صحيح *sync_socket_باہرلا, بنية libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) بنية init_حالت_s init_حالت;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحيح sync_socket_container = -1;
  حرف *notify_socket_env = عدم;
  cleanup_close صحيح sync_socket_host = -1;
  __attribute__ ((بن_ورتیا)) cleanup_close صحيح restore_pidns = -1;
  صحيح پہلا_clone_دلیلاں = 0;
  صحيح sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  صحيح ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_حالت (&init_حالت, container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  get_نجی_انکڑے (container)->unshare_flags = init_حالت.all_namespaces;
  get_نجی_انکڑے (container)->joined_mount_ns
      = (init_حالت.all_namespaces & CLONE_NEWNS)
        && ! (init_حالت.namespaces_منزل_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_حالت.all_namespaces &= ~CLONE_NEWCGROUP;
  get_نجی_انکڑے (container)->unshare_cgroupns = init_حالت.namespaces_منزل_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#جے_تعریف_شدہ HAVE_SYSTEMD
  إذا (def->جڑ)
    {
      ret = do_notify_socket (container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
#جے_انت

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  إذا (def->process)
    {
      ret = libcrun_سیٹ_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = libcrun_سیٹ_oom (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_container_notify_handler (دلیلاں, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->جڑ ? container->container_def->جڑ->رستہ : عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_حالت.delayed_userns_create
      = (init_حالت.all_namespaces & CLONE_NEWUSER) && init_حالت.userns_اشاریہ < 0 && init_حالت.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_حالت.all_namespaces & CLONE_NEWUSER) && init_حالت.userns_اشاریہ < 0;
  لكل (i = 0; i < init_حالت.fd_len; i++)
    {
      بدل (init_حالت.قدر[i])
        {
        حالة CLONE_NEWIPC:
          إذا (init_حالت.all_namespaces & CLONE_NEWUSER)
            init_حالت.join_ipcns = true;
          اكسر;

        حالة CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_حالت.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          إذا (! new_userns)
            init_حالت.must_fork = true;
          وإلا
            {
              init_حالت.join_pidns = true;
              init_حالت.idx_pidns_منزل_join_immediately = i;
              init_حالت.namespaces_منزل_unshare &= ~CLONE_NEWPID;
            }
          اكسر;

        حالة CLONE_NEWTIME:
          إذا (! new_userns)
            init_حالت.must_fork = true;
          وإلا
            {
              init_حالت.idx_timens_منزل_join_immediately = i;
              init_حالت.namespaces_منزل_unshare &= ~CLONE_NEWTIME;
            }
          اكسر;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  إذا ((init_حالت.namespaces_منزل_unshare & CLONE_NEWUSER) && init_حالت.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      پہلا_clone_دلیلاں = CLONE_NEWUSER;
    }
  وإلا إذا ((init_حالت.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      پہلا_clone_دلیلاں = init_حالت.namespaces_منزل_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_حالت.namespaces_منزل_unshare &= ~پہلا_clone_دلیلاں;

  /* Check if there are still namespaces that require a fork().  */
  إذا (init_حالت.namespaces_منزل_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      پہلا_clone_دلیلاں &= ~CLONE_NEWPID;
      init_حالت.namespaces_منزل_unshare |= CLONE_NEWPID;
      init_حالت.must_fork = true;
      init_حالت.can_delay_pidns_fork = false;
    }

  pid = -1;
  إذا (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      بنية clone3_دلیلاں clone3_دلیلاں_2;
      memset (&clone3_دلیلاں_2, 0, حجم (clone3_دلیلاں_2));
      clone3_دلیلاں_2.exit_signal = SIGCHLD;
      clone3_دلیلاں_2.flags = پہلا_clone_دلیلاں;

      clone3_دلیلاں_2.flags |= CLONE_INTO_CGROUP;
      clone3_دلیلاں_2.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_دلیلاں_2);
      إذا (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  إذا (pid < 0)
    {
      pid = syscall_clone (پہلا_clone_دلیلاں | SIGCHLD, عدم);
      إذا (UNLIKELY (pid < 0))
        أرجع crun_make_error (err, errno, "clone");
    }

  إذا (pid)
    {
      __attribute__ ((بن_ورتیا)) cleanup_pid pid_t pid_منزل_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = close_and_reset (&sync_socket_container);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_نجی_انکڑے (container)->notify_socket_درخت_fd));

      إذا (init_حالت.idx_pidns_منزل_join_immediately >= 0 || init_حالت.idx_timens_منزل_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_host, &new_pid, حجم (new_pid)));
          إذا (UNLIKELY (ret != حجم (new_pid)))
            أرجع crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          إذا (new_pid == 0)
            get_نجی_انکڑے (container)->delayed_pidns_fork = true;
          وإلا
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, عدم, 0);

              pid_منزل_clean = pid = new_pid;
            }

          ret = send_success_منزل_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      إذا (init_حالت.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      إذا ((init_حالت.all_namespaces & CLONE_NEWUSER) && init_حالت.userns_اشاریہ < 0)
        {
          ret = libcrun_سیٹ_usernamespace (container, pid, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          ret = send_success_منزل_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      إذا (init_حالت.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket_host, &grandchild, حجم (grandchild)));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_منزل_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          إذا (grandchild == 0)
            get_نجی_انکڑے (container)->delayed_pidns_fork = true;
          وإلا
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, عدم, 0);

              pid_منزل_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      *sync_socket_باہرلا = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      إذا (! get_نجی_انکڑے (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      pid_منزل_clean = 0;
      أرجع pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  إذا (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_حالت, err);
  إذا (UNLIKELY (ret < 0))
    send_error_منزل_sync_socket_and_die (sync_socket_container, false, err);
  وإلا
    {
      ret = send_success_منزل_sync_socket (sync_socket_container, err);
      إذا (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->پیغام);
    }

  /* Jump into the specified entrypoint.  */
  إذا (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (دلیلاں, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  إذا (*err)
    libcrun_fail_with_error ((*err)->حالت, "%s", (*err)->پیغام);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  إذا (ret == 0)
    محفوظ_exit (EXIT_SUCCESS);

  محفوظ_exit (EXIT_FAILURE);
}

ساكن صحيح
join_process_ماپا_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t بچہ_pid, صحيح sync_socket_fd,
                            libcrun_container_حالت_t *حالت,
                            bool need_move_منزل_cgroup, ثابت حرف *sub_cgroup,
                            صحيح *terminal_fd, libcrun_error_t *err)
{
  صحيح ret;
  حرف res;
  pid_t pid;
  cleanup_close صحيح sync_fd = sync_socket_fd;

  إذا (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = عارضی_FAILURE_RETRY (اقرأ (sync_fd, &res, 1));
  إذا (UNLIKELY (ret != 1))
    أرجع crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  إذا (res != '0')
    أرجع crun_make_error (err, 0, "fail startup");

  ret = عارضی_FAILURE_RETRY (اقرأ (sync_fd, &pid, حجم (pid)));
  إذا (UNLIKELY (ret != حجم (pid)))
    أرجع crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (بچہ_pid, عدم, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "waitpid for exec child pid");

  إذا (process && process->exec_cpu_affinity)
    {
      ret = libcrun_سیٹ_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_نقاب (pid, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (need_move_منزل_cgroup)
    {
      cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *cgroup_حالت = libcrun_cgroup_make_حالت (حالت);
      cleanup_free حرف *final_cgroup = عدم;

      إذا (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, حالت->cgroup_رستہ, sub_cgroup, عدم);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_حالت, final_cgroup ?: حالت->cgroup_رستہ, pid, حالت->pid, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_سیٹ_scheduler (pid, process, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (process && process->exec_cpu_affinity)
    {
      ret = libcrun_سیٹ_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_سیٹ_io_ترجیح (pid, process, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = عارضی_FAILURE_RETRY (اكتب (sync_fd, &ret, حجم (ret)));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "write to sync socket");

  إذا (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      إذا (UNLIKELY (ret < 0))
        {
          صحيح err_کوڈ = 0;
          cleanup_free حرف *err_str = عدم;

          إذا (read_error_from_sync_socket (sync_fd, &err_کوڈ, &err_str))
            {
              إذا (! is_خالی_تھاواں_string (err_str))
                {
                  crun_error_release (err);
                  أرجع crun_make_error (err, err_کوڈ, "%s", err_str);
                }
            }

          أرجع crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  أرجع pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
ساكن صحيح
try_setns_with_pidfd (pid_t pid_منزل_join, libcrun_container_t *container, libcrun_container_حالت_t *حالت, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحيح pidfd_pid_منزل_join = -1;
  صحيح all_flags = 0;
  size_t i;
  صحيح ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  إذا (def->linux && def->linux->namespaces)
    {
      لكل (i = 0; i < def->linux->namespaces_len; i++)
        إذا (! is_خالی_تھاواں_string (def->linux->namespaces[i]->رستہ))
          أرجع 0;
    }

  pidfd_pid_منزل_join = syscall_pidfd_open (pid_منزل_join, 0);
  إذا (UNLIKELY (pidfd_pid_منزل_join < 0))
    أرجع 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (حالت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  إذا (ret == 0)
    أرجع crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  لكل (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].قدر;

  ret = setns (pidfd_pid_منزل_join, all_flags);
  إذا (UNLIKELY (ret < 0))
    أرجع 0;

  أرجع 1;
}

ساكن صحيح
join_process_namespaces (libcrun_container_t *container, pid_t pid_منزل_join, libcrun_container_حالت_t *حالت, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح fds_joined[MAX_NAMESPACES] = {
    0,
  };
  صحيح fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  صحيح ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_منزل_join, container, حالت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  /* Nothing left to do if the namespaces were joined.  */
  إذا (LIKELY (ret > 0))
    أرجع 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  إذا (def->linux->namespaces_len >= MAX_NAMESPACES)
    أرجع crun_make_error (err, 0, "invalid configuration");

  لكل (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free حرف *ns_رستہ = عدم;

      xasprintf (&ns_رستہ, "%d/ns/%s", pid_منزل_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_رستہ, O_RDONLY, err);
      إذا (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          إذا (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              استمر;
            }

          ret = fds[i];
          اذهب باہر_نکلو;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (حالت, err);
  إذا (UNLIKELY (ret < 0))
    اذهب باہر_نکلو;
  إذا (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      اذهب باہر_نکلو;
    }

  لكل (i = 0; namespaces[i].ns_file; i++)
    {
      إذا (namespaces[i].قدر == CLONE_NEWUSER)
        استمر;

      ret = setns (fds[i], 0);
      إذا (ret == 0)
        fds_joined[i] = 1;
    }
  لكل (i = 0; namespaces[i].ns_file; i++)
    {
      إذا (fds_joined[i])
        استمر;
      ret = setns (fds[i], 0);
      إذا (ret == 0)
        fds_joined[i] = 1;
    }
  لكل (i = 0; namespaces[i].ns_file; i++)
    {
      إذا (fds_joined[i])
        استمر;
      ret = setns (fds[i], 0);
      إذا (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool لبھیا = false;

          لكل (j = 0; j < def->linux->namespaces_len; j++)
            {
              إذا (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  لبھیا = true;
                  اكسر;
                }
            }
          إذا (! لبھیا)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              استمر;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          اذهب باہر_نکلو;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

باہر_نکلو:
  لكل (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  أرجع ret;
}

صحيح
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_منزل_join,
                      libcrun_container_حالت_t *حالت,
                      ثابت حرف *sub_cgroup,
                      صحيح detach,
                      runtime_spec_schema_config_schema_process *process,
                      صحيح *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  صحيح ret;
  صحيح sync_socket_fd[2];
  cleanup_close صحيح cgroup_dirfd = -1;
  cleanup_close صحيح sync_fd = -1;
  بنية clone3_دلیلاں clone3_دلیلاں_2;
  bool need_move_منزل_cgroup;

  إذا (! detach)
    {
      ret = prctl (PR_سیٹ_بچہ_SUBREAPER, 1, 0, 0, 0);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "set child subreaper");
    }

  إذا (رستہ_has_dot_dot_component (sub_cgroup))
    أرجع crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  إذا (sub_cgroup && ! is_خالی_تھاواں_string (حالت->cgroup_رستہ))
    {
      cleanup_free حرف *final_cgroup = عدم;

      ret = append_paths (&final_cgroup, err, حالت->cgroup_رستہ, sub_cgroup, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *cgroup_حالت = عدم;

    cgroup_حالت = libcrun_cgroup_make_حالت (حالت);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    إذا (get_force_cgroup_v1_annotation (container) == عدم)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_حالت, sub_cgroup, err);
        إذا (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_دلیلاں_2, 0, حجم (clone3_دلیلاں_2));
  clone3_دلیلاں_2.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  إذا (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_منزل_cgroup = true;
  وإلا
    {
      need_move_منزل_cgroup = false;
      clone3_دلیلاں_2.flags |= CLONE_INTO_CGROUP;
      clone3_دلیلاں_2.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_دلیلاں_2);

  إذا (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_سیٹ_scheduler (pid, process, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  /* On errors, fall back to fork().  */
  إذا (pid < 0)
    {
      need_move_منزل_cgroup = true;

      pid = fork ();
      إذا (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          اذهب باہر_نکلو;
        }
    }

  إذا (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      أرجع join_process_ماپا_helper (context, container,
                                         process, pid, sync_fd,
                                         حالت, need_move_منزل_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_منزل_join, حالت, err);
  إذا (UNLIKELY (ret < 0))
    {
      عارضی_FAILURE_RETRY (اكتب (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->حالت, "%s", (*err)->پیغام);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  إذا (UNLIKELY (pid < 0))
    {
      صحيح saved_errno = errno;
      عارضی_FAILURE_RETRY (اكتب (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  إذا (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = عارضی_FAILURE_RETRY (اكتب (sync_fd, "0", 1));
      إذا (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          محفوظ_exit (EXIT_FAILURE);
        }

      ret = عارضی_FAILURE_RETRY (اكتب (sync_fd, &pid, حجم (pid)));
      إذا (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          محفوظ_exit (EXIT_FAILURE);
        }

      محفوظ_exit (EXIT_SUCCESS);
    }
  وإلا
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free حرف *pty = عدم;
      صحيح r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = عارضی_FAILURE_RETRY (اقرأ (sync_fd, &r, حجم (r)));
      إذا (UNLIKELY (ret != حجم (r) || r < 0))
        محفوظ_exit (EXIT_FAILURE);

      ret = setsid ();
      إذا (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_منزل_sync_socket_and_die (sync_fd, true, err);
        }

      إذا (terminal_fd)
        {
          cleanup_close صحيح ptmx_fd = -1;

          ret = سیٹ_id_init (container, err);
          إذا (UNLIKELY (ret < 0))
            send_error_منزل_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          إذا (UNLIKELY (ptmx_fd < 0))
            send_error_منزل_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_منزل_socket (sync_fd, ptmx_fd, err);
          إذا (UNLIKELY (ret < 0))
            send_error_منزل_sync_socket_and_die (sync_fd, true, err);
        }
    }

  أرجع 0;

باہر_نکلو:
  إذا (sync_socket_fd[0] >= 0)
    عارضی_FAILURE_RETRY (close (sync_socket_fd[0]));
  إذا (sync_socket_fd[1] >= 0)
    عارضی_FAILURE_RETRY (close (sync_socket_fd[1]));
  أرجع ret;
}

صحيح
libcrun_linux_container_update (libcrun_container_حالت_t *حالت, ثابت حرف *state_جڑ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *cgroup_حالت = عدم;

  cgroup_حالت = libcrun_cgroup_make_حالت (حالت);

  أرجع libcrun_update_cgroup_resources (cgroup_حالت, state_جڑ, resources, err);
}

ساكن صحيح
libcrun_container_pause_unpause_linux (libcrun_container_حالت_t *حالت, ثابت bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *cgroup_حالت = عدم;

  cgroup_حالت = libcrun_cgroup_make_حالت (حالت);

  أرجع libcrun_cgroup_pause_unpause (cgroup_حالت, pause, err);
}

صحيح
libcrun_container_pause_linux (libcrun_container_حالت_t *حالت, libcrun_error_t *err)
{
  أرجع libcrun_container_pause_unpause_linux (حالت, true, err);
}

صحيح
libcrun_container_unpause_linux (libcrun_container_حالت_t *حالت, libcrun_error_t *err)
{
  أرجع libcrun_container_pause_unpause_linux (حالت, false, err);
}

صحيح
libcrun_سیٹ_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  بنانشانی لما persona = 0;
  صحيح ret;

  إذا (strcmp (p->domain, "LINUX") == 0)
    persona = فی_LINUX;
  وإلا إذا (strcmp (p->domain, "LINUX32") == 0)
    persona = فی_LINUX32;
  وإلا
    أرجع crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, 0, "personality to `%s`", p->domain);

  أرجع 0;
}

صحيح
libcrun_configure_جال (libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;
  size_t i;
  bool configure_جال = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحيح sockfd = -1;

  لكل (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحيح قدر = libcrun_لبھو_namespace (def->linux->namespaces[i]->type);
      إذا (UNLIKELY (قدر < 0))
        أرجع crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      إذا (قدر == CLONE_NEWNET && def->linux->namespaces[i]->رستہ == عدم)
        {
          configure_جال = true;
          اكسر;
        }
    }

  إذا (! configure_جال)
    أرجع 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  إذا (LIKELY (sockfd >= 0))
    {
      بنية ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_اتے | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  وإلا
    {
      بنية nlmsghdr *hdr_recv;
      حرف buf[حجم (بنية nlmsghdr) + حجم (بنية nlmsgerr)];
      بنية sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      بنية sockaddr_nl addr_منزل = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      بنية nlmsghdr hdr = {
        .nlmsg_len = حجم (بنية nlmsghdr) + حجم (بنية ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      بنية ifinfomsg پیغام = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_اشاریہ = 1,
        .ifi_flags = IFF_اتے,
        .ifi_change = IFF_اتے,
      };

      *((بنية nlmsghdr *) buf) = hdr;
      *((بنية ifinfomsg *) (buf + حجم (بنية nlmsghdr))) = پیغام;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      إذا (UNLIKELY (sockfd < 0))
        أرجع crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (بنية sockaddr *) &addr, حجم (addr));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, حجم (buf), 0, (بنية sockaddr *) &addr_منزل, حجم (addr_منزل));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, حجم (buf), 0, عدم, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (بنية nlmsghdr *) buf;

      إذا (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          بنية nlmsgerr *err_انکڑے = (بنية nlmsgerr *) NLMSG_انکڑے (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          إذا (err_انکڑے->error < 0)
            أرجع crun_make_error (err, -err_انکڑے->error, "netlink error while configuring network");
        }
    }

  أرجع 0;
}

/* Protection for attacks like CVE-2019-5736.  */
صحيح ensure_cloned_ثنائی ();
__attribute__ ((constructor)) ساكن فراغ
libcrun_rexec (فراغ)
{
  إذا (ensure_cloned_ثنائی () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      محفوظ_exit (EXIT_FAILURE);
    }
}

صحيح
libcrun_container_checkpoint_linux (libcrun_container_حالت_t *حالت, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  أرجع libcrun_container_checkpoint_linux_criu (حالت, container, cr_options, err);
}

صحيح
libcrun_container_restore_linux (libcrun_container_حالت_t *حالت, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  صحيح ret;
  ret = libcrun_container_restore_linux_criu (حالت, container, cr_options, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
ساكن صحيح
libcrun_kill_linux_no_pidfd (libcrun_container_حالت_t *حالت, bool check_pid, صحيح إشارة, libcrun_error_t *err)
{
  صحيح ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  إذا (check_pid)
    {
      ret = libcrun_check_pid_valid (حالت, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      /* The pid is not valid anymore, return an error.  */
      إذا (ret == 0)
        {
          errno = ESRCH;
          أرجع crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (حالت->pid, إشارة);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "kill container");
  أرجع 0;
}

صحيح
libcrun_kill_linux (libcrun_container_حالت_t *حالت, صحيح إشارة, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_close صحيح pidfd = -1;

  pidfd = syscall_pidfd_open (حالت->pid, 0);
  إذا (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      إذا (errno == ENOSYS)
        أرجع libcrun_kill_linux_no_pidfd (حالت, true, إشارة, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      إذا (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        أرجع crun_make_error (err, ESRCH, "process not running");
      أرجع crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (حالت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* The pid is not valid anymore, return an error.  */
  إذا (ret == 0)
    {
      errno = ESRCH;
      أرجع crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, إشارة, عدم, 0);
  إذا (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      إذا (errno == ENOSYS)
        أرجع libcrun_kill_linux_no_pidfd (حالت, false, إشارة, err);
      أرجع crun_make_error (err, errno, "send signal to pidfd");
    }

  أرجع 0;
}

ساكن ثابت حرف *
libcrun_get_intelrdt_name (ثابت حرف *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  إذا (def == عدم || def->linux == عدم || def->linux->intel_rdt == عدم || def->linux->intel_rdt->clos_id == عدم)
    {
      إذا (explicit)
        *explicit = false;
      أرجع ctr_name;
    }

  إذا (explicit)
    *explicit = true;
  أرجع def->linux->intel_rdt->clos_id;
}

صحيح
libcrun_apply_intelrdt (ثابت حرف *ctr_name, libcrun_container_t *container, pid_t pid, صحيح actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = عدم;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  ثابت حرف *name;
  صحيح ret;

  إذا (container)
    def = container->container_def;

  إذا (def == عدم || def->linux == عدم || def->linux->intel_rdt == عدم)
    أرجع 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  إذا (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_گروہ (name, ctr_name, err);
          إذا (UNLIKELY (ret < 0))
            اذهب fail;

          has_monitoring = true;
        }
    }

  إذا (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;
    }

  إذا (actions & LIBCRUN_INTELRDT_MOVE)
    {
      ثابت حرف *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : عدم;
      ret = resctl_move_task_منزل (name, monitoring_name, pid, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;
    }

  أرجع 0;

fail:
  إذا (has_monitoring)
    {
      libcrun_error_t tmp_err = عدم;
      صحيح tmp_ret;

      tmp_ret = resctl_destroy_monitoring_گروہ (name, ctr_name, &tmp_err);
      إذا (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  إذا (created)
    {
      libcrun_error_t tmp_err = عدم;
      صحيح tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      إذا (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  أرجع ret;
}

صحيح
libcrun_destroy_intelrdt (ثابت حرف *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  ثابت حرف *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  إذا (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      صحيح ret;

      ret = resctl_destroy_monitoring_گروہ (clos_id, container_id, err);
      إذا (ret < 0)
        أرجع ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  إذا (explicit)
    أرجع 0;

  أرجع resctl_destroy (clos_id, err);
}

صحيح
libcrun_update_intel_rdt (ثابت حرف *ctr_name, libcrun_container_t *container, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = عدم;
  ثابت حرف *name;

  إذا (container)
    def = container->container_def;

  إذا (def == عدم || def->linux == عدم || def->linux->intel_rdt == عدم)
    أرجع 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, عدم);

  أرجع resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
صحيح
libcrun_محفوظ_chdir (ثابت حرف *رستہ, libcrun_error_t *err)
{
  cleanup_free حرف *بفر = عدم;
  صحيح ret;

  ret = chdir (رستہ);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "chdir to `%s`", رستہ);

  بفر = xmalloc (PATH_MAX);
  ret = syscall_getcwd (بفر, PATH_MAX);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  إذا (ret == 0 || بفر[0] != '/')
    {
      إذا (chdir ("/") < 0)
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
#تعریف_کرو UNREACHABLE_LEN ((صحيح) حجم (UNREACHABLE) - 1)

      إذا ((ret >= UNREACHABLE_LEN) && (memcmp (بفر, UNREACHABLE, UNREACHABLE_LEN) == 0))
        أرجع crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      أرجع crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  أرجع 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
ساكن صحيح
لگاتار_callback_وچ_namespace (صحيح mntns_fd, صحيح (*callback) (فراغ *, libcrun_error_t *), فراغ *arg, libcrun_error_t *err)
{
  صحيح wait_حالت = 0;
  pid_t pid;
  صحيح ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  إذا (UNLIKELY (pid < 0))
    أرجع crun_make_error (err, errno, "vfork");

  إذا (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      إذا (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          محفوظ_exit (ret);
        }
      ret = chdir ("/");
      إذا (UNLIKELY (ret < 0))
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
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  أرجع get_process_exit_حالت (wait_حالت);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
ساكن صحيح
check_container_userns (libcrun_container_t *container, صحيح userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close صحيح expected_fd = -1;
  cleanup_close صحيح ماپا_fd = -1;
  بنية stat st, expected_st;
  ثابت حرف *رستہ = عدم;
  size_t i;
  صحيح ret;

  إذا (def->linux)
    لكل (i = 0; i < def->linux->namespaces_len; i++)
      إذا (libcrun_لبھو_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          رستہ = def->linux->namespaces[i]->رستہ;
          new_userns = رستہ == عدم;
          اكسر;
        }

  إذا (رستہ == عدم)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      إذا (UNLIKELY (expected_fd < 0))
        أرجع expected_fd;
    }
  وإلا
    {
      expected_fd = open (رستہ, O_RDONLY | O_CLOEXEC);
      إذا (UNLIKELY (expected_fd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", رستہ);
    }

  /* A user namespace created for the container is a child of the current one.  */
  إذا (new_userns)
    {
      ماپا_fd = ioctl (userns_fd, NS_GET_ماپا);
      إذا (UNLIKELY (ماپا_fd < 0))
        أرجع crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = ماپا_fd;
    }

  ret = fstat (userns_fd, &st);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fstat user namespace");

  إذا (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    أرجع crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  أرجع 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
ساكن صحيح
open_container_mntns (libcrun_container_t *container, libcrun_container_حالت_t *حالت, صحيح *mntns_fd, صحيح *owner_fd, libcrun_error_t *err)
{
  cleanup_close صحيح mntns = -1;
  cleanup_close صحيح owner = -1;
  صحيح ret;

  mntns = libcrun_open_proc_pid_file (container, حالت->pid, "ns/mnt", O_RDONLY, err);
  إذا (UNLIKELY (mntns < 0))
    أرجع mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (حالت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret == 0)
    أرجع crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  إذا (UNLIKELY (owner < 0))
    أرجع crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  أرجع 0;
}

ساكن صحيح
لگاتار_وچ_container_namespace (libcrun_container_t *container, libcrun_container_حالت_t *حالت, صحيح (*callback) (فراغ *, libcrun_error_t *), فراغ *arg, libcrun_error_t *err)
{
  cleanup_close صحيح mntns_fd = -1;
  cleanup_close صحيح owner_fd = -1;
  صحيح ret;

  ret = open_container_mntns (container, حالت, &mntns_fd, &owner_fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع لگاتار_callback_وچ_namespace (mntns_fd, callback, arg, err);
}

بنية umount_وچ_a_container_دلیلاں
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
ساكن صحيح
umount_nofollow (ثابت حرف *destination, libcrun_error_t *err)
{
  cleanup_close صحيح parentfd = -1;
  cleanup_close صحيح cwdfd = -1;
  حرف بفر[PATH_MAX];
  ثابت حرف *ماپا_2, *name;
  حرف *آخری_slash;
  size_t len;
  صحيح saved_errno;
  صحيح ret;

  إذا (is_خالی_تھاواں_string (destination))
    أرجع crun_make_error (err, 0, "empty destination for mount");

  len = لڑی_لمبائی (destination);
  إذا (UNLIKELY (len >= حجم (بفر)))
    أرجع crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  حافظہ_نقل (بفر, destination, len + 1);

  طالما (len > 1 && بفر[len - 1] == '/')
    بفر[--len] = '\0';

  آخری_slash = strrchr (بفر, '/');
  إذا (آخری_slash == عدم)
    {
      ماپا_2 = ".";
      name = بفر;
    }
  وإلا إذا (آخری_slash == بفر)
    {
      ماپا_2 = "/";
      name = بفر[1] ? بفر + 1 : ".";
    }
  وإلا
    {
      *آخری_slash = '\0';
      ماپا_2 = بفر;
      name = آخری_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, ماپا_2, O_رستہ | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  إذا (UNLIKELY (parentfd < 0))
    أرجع crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_رستہ | O_DIRECTORY | O_CLOEXEC);
  إذا (UNLIKELY (cwdfd < 0))
    أرجع crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  إذا (UNLIKELY (fchdir (cwdfd) < 0))
    أرجع crun_make_error (err, errno, "fchdir to the previous working directory");

  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, saved_errno, "umount `%s`", destination);

  أرجع 0;
}

ساكن صحيح
do_umount_وچ_a_container (فراغ *arg, libcrun_error_t *err)
{
  بنية umount_وچ_a_container_دلیلاں *دلیلاں = arg;
  size_t i;
  صحيح ret;

  لكل (i = 0; i < دلیلاں->len; i++)
    {
      ret = umount_nofollow (دلیلاں->mounts[i]->destination, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

بنية mount_وچ_a_container_دلیلاں
{
  runtime_spec_schema_defs_mount **mounts;
  بنية libcrun_fd_map *fds;
  صحيح pidfd;
  صحيح owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
ساكن صحيح
open_destination_nofollow (ثابت حرف *destination, bool dir, libcrun_error_t *err)
{
  ثابت uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  صحيح fd;
  صحيح ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_رستہ | O_CLOEXEC, 0, resolve);
  إذا (LIKELY (fd >= 0))
    أرجع fd;

  إذا (errno != ENOENT)
    أرجع crun_make_error (err, errno, "open `%s`", destination);

  إذا (dir)
    ret = crun_محفوظ_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  وإلا
    ret = crun_محفوظ_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_رستہ | O_CLOEXEC, 0, resolve);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", destination);

  أرجع fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
ساكن صحيح
create_destinations_وچ_a_container (فراغ *arg, libcrun_error_t *err)
{
  بنية mount_وچ_a_container_دلیلاں *دلیلاں = arg;
  size_t i;
  صحيح ret;

  ret = setns (دلیلاں->owner_fd, CLONE_NEWUSER);
  إذا (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "setresuid to `0`");
    }
  وإلا إذا (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      أرجع crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  لكل (i = 0; i < دلیلاں->len; i++)
    {
      cleanup_close صحيح dest_fd = -1;
      بنية stat st;

      ret = fstat (دلیلاں->fds->fds[i], &st);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (دلیلاں->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      إذا (UNLIKELY (dest_fd < 0))
        أرجع dest_fd;
    }

  أرجع 0;
}

ساكن صحيح
do_mount_وچ_a_container (فراغ *arg, libcrun_error_t *err)
{
  بنية mount_وچ_a_container_دلیلاں *دلیلاں = arg;
  size_t i;
  صحيح ret;

  لكل (i = 0; i < دلیلاں->len; i++)
    {
      cleanup_close صحيح dest_fd = -1;
      بنية stat st;

      ret = fstat (دلیلاں->fds->fds[i], &st);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (دلیلاں->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      إذا (UNLIKELY (dest_fd < 0))
        أرجع dest_fd;

      ret = fs_move_mount_منزل (دلیلاں->fds->fds[i], dest_fd, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "move mount to `%s`", دلیلاں->mounts[i]->destination);
    }

  أرجع 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
ساكن صحيح
send_mounts_from_container_namespaces (libcrun_container_t *container, صحيح pidfd, runtime_spec_schema_defs_mount **mounts,
                                       بنية libcrun_fd_map *fds, size_t len, صحيح flags, صحيح owner_fd, صحيح sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  صحيح ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_سیٹ_DUMPABLE, 0, 0, 0, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  إذا (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  وإلا إذا (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      أرجع crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  إذا (UNLIKELY (pid < 0))
    أرجع crun_make_error (err, errno, "fork");

  إذا (pid)
    {
      صحيح wait_حالت = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = عارضی_FAILURE_RETRY (waitpid (pid, &wait_حالت, WUNTRACED));
      إذا (ret > 0 && WIFSTOPPED (wait_حالت))
        {
          kill (pid, SIGKILL);
          عارضی_FAILURE_RETRY (waitpid (pid, عدم, 0));
          أرجع crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      محفوظ_exit (EXIT_SUCCESS);
    }

  ret = سیٹ_id_init (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = prctl (PR_سیٹ_DUMPABLE, 0, 0, 0, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  لكل (i = 0; i < len; i++)
    {
      cleanup_close صحيح fd = -1;

      إذا (fds->fds[i] >= 0)
        استمر;

      fd = open_mount_دا_type (container, mounts[i], err);
      إذا (UNLIKELY (fd < 0))
        أرجع crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_منزل_socket (sync_fd, fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
ساكن صحيح
get_namespaces_منزل_join (libcrun_container_t *container, pid_t pid, صحيح *flags, libcrun_error_t *err)
{
  ساكن ثابت بنية
  {
    ثابت حرف *name;
    صحيح flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  لكل (i = 0; i < حجم (namespaces) / حجم (namespaces[0]); i++)
    {
      cleanup_close صحيح self_fd = -1;
      cleanup_close صحيح نشانہ_fd = -1;
      بنية stat self_st, نشانہ_st;
      cleanup_free حرف *self_رستہ = عدم;
      صحيح ret;

      xasprintf (&self_رستہ, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_رستہ, O_RDONLY, err);
      إذا (UNLIKELY (self_fd < 0))
        أرجع self_fd;

      نشانہ_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      إذا (UNLIKELY (نشانہ_fd < 0))
        أرجع نشانہ_fd;

      ret = fstat (self_fd, &self_st);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (نشانہ_fd, &نشانہ_st);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      إذا (self_st.st_dev != نشانہ_st.st_dev || self_st.st_ino != نشانہ_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  أرجع 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
ساكن صحيح
open_mounts_وچ_container_namespaces (libcrun_container_t *container, libcrun_container_حالت_t *حالت, صحيح owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, بنية libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close صحيح sync_fd = -1;
  cleanup_close صحيح pidfd = -1;
  صحيح sync_socket_fd[2];
  bool needed = false;
  صحيح flags;
  pid_t pid;
  size_t i;
  صحيح ret;

  لكل (i = 0; i < len; i++)
    {
      إذا (fds->fds[i] >= 0)
        استمر;

      إذا (mounts[i]->type == عدم)
        أرجع crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  إذا (! needed)
    أرجع 0;

  pidfd = syscall_pidfd_open (حالت->pid, 0);
  إذا (UNLIKELY (pidfd < 0))
    أرجع crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (حالت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret == 0)
    أرجع crun_make_error (err, 0, "container not running");

  ret = get_namespaces_منزل_join (container, حالت->pid, &flags, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  إذا (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      عارضی_FAILURE_RETRY (close (sync_socket_fd[1]));
      أرجع ret;
    }

  إذا (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      إذا (UNLIKELY (ret < 0))
        send_error_منزل_sync_socket_and_die (sync_socket_fd[1], true, err);

      محفوظ_exit (EXIT_SUCCESS);
    }

  عارضی_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  لكل (i = 0; i < len; i++)
    {
      إذا (fds->fds[i] >= 0)
        استمر;

      ret = receive_fd_from_socket (sync_fd, err);
      إذا (UNLIKELY (ret < 0))
        {
          cleanup_free حرف *err_str = عدم;
          صحيح err_کوڈ = 0;

          إذا (read_error_from_sync_socket (sync_fd, &err_کوڈ, &err_str) && ! is_خالی_تھاواں_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_کوڈ, "%s", err_str);
            }
          وإلا
            ret = crun_error_wrap (err, "receive mount fd");
          اكسر;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  عارضی_FAILURE_RETRY (waitpid (pid, عدم, 0));

  أرجع ret;
}

ساكن بنانشانی لما
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_سیٹ)
{
  cleanup_free حرف *انکڑے = عدم;
  بنانشانی لما extra_flags = 0;
  بنانشانی لما flags = 0;
  size_t j;

  إذا (mount->options == عدم)
    أرجع get_default_flags (container, mount->destination, &انکڑے);

  لكل (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &انکڑے, rec_clear, rec_سیٹ);

  أرجع flags;
}

صحيح
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_حالت_t *حالت, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  بنية mount_وچ_a_container_دلیلاں دلیلاں;
  cleanup_close_map بنية libcrun_fd_map *fds = عدم;
  cleanup_close صحيح pidfd = -1;
  cleanup_close صحيح mntns_fd = -1;
  cleanup_close صحيح owner_fd = -1;
  pid_t pid = حالت->pid;
  size_t i;
  صحيح ret;

  ret = open_container_mntns (container, حالت, &mntns_fd, &owner_fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  fds = make_libcrun_fd_map (len);

  لكل (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), عدم, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_سیٹ = 0;
          بنانشانی لما پسار;

          پسار = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_سیٹ) & ALL_PROPAGATIONS_NO_REC;
          إذا (پسار == 0)
            پسار = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->سروت, recursive, false, nofollow, پسار, err);
          إذا (UNLIKELY (fds->fds[i] < 0))
            أرجع fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_وچ_container_namespaces (container, حالت, owner_fd, mounts, fds, len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  لكل (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_سیٹ = 0;
      بنانشانی لما flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_سیٹ);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_سیٹ, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  دلیلاں.mounts = mounts;
  دلیلاں.fds = fds;
  دلیلاں.pidfd = pidfd;
  دلیلاں.owner_fd = owner_fd;
  دلیلاں.len = len;

  ret = لگاتار_callback_وچ_namespace (mntns_fd, create_destinations_وچ_a_container, &دلیلاں, err);
  إذا (UNLIKELY (ret > 0))
    أرجع *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع لگاتار_callback_وچ_namespace (mntns_fd, do_mount_وچ_a_container, &دلیلاں, err);
}

صحيح
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_حالت_t *حالت, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  بنية umount_وچ_a_container_دلیلاں دلیلاں;

  دلیلاں.mounts = mounts;
  دلیلاں.len = len;

  أرجع لگاتار_وچ_container_namespace (container, حالت, do_umount_وچ_a_container, &دلیلاں, err);
}

بنية restrict_mounts_وچ_a_container_دلیلاں
{
  بنية libcrun_mount_flags_s *mounts;
  size_t len;
  صحيح rootfd;
};

ساكن صحيح
do_restrict_mounts_وچ_a_container (فراغ *arg, libcrun_error_t *err)
{
  بنية restrict_mounts_وچ_a_container_دلیلاں *دلیلاں = arg;
  size_t i;
  صحيح ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (دلیلاں->rootfd);
  إذا (LIKELY (ret == 0))
    ret = chroot (".");
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "chroot to the container root");

  لكل (i = 0; i < دلیلاں->len; i++)
    {
      بنانشانی لما flags = دلیلاں->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      ثابت حرف *نشانہ = consume_slashes (دلیلاں->mounts[i].نشانہ);
      cleanup_close صحيح fd = -1;
      بنية statfs sfs;
      بنانشانی لما current_flags;

      إذا (flags == 0)
        استمر;

      fd = محفوظ_openat (AT_FDCWD, "/", نشانہ, O_رستہ | O_CLOEXEC, 0, err);
      إذا (UNLIKELY (fd < 0))
        {
          إذا (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              استمر;
            }
          أرجع fd;
        }

      ret = fstatfs (fd, &sfs);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "statfs `/%s`", نشانہ);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      إذا ((current_flags & flags) == flags)
        استمر;

      ret = do_mount_setattr (false, نشانہ, fd, 0, flags, err);
      إذا (LIKELY (ret == 0))
        استمر;

      إذا (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        أرجع ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (عدم, نشانہ, عدم, MS_REMOUNT | MS_BIND | flags | current_flags, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "remount `/%s`", نشانہ);
    }

  أرجع 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
صحيح
libcrun_restrict_mounts_وچ_container (pid_t pid, بنية libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  بنية restrict_mounts_وچ_a_container_دلیلاں دلیلاں;
  cleanup_close صحيح mntns_fd = -1;
  cleanup_close صحيح rootfd = -1;
  حرف رستہ[64];
  صحيح ret;

  snprintf (رستہ, حجم (رستہ), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (رستہ, O_RDONLY | O_CLOEXEC);
  إذا (UNLIKELY (mntns_fd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", رستہ);

  snprintf (رستہ, حجم (رستہ), "/proc/%d/root", pid);
  rootfd = open (رستہ, O_رستہ | O_DIRECTORY | O_CLOEXEC);
  إذا (UNLIKELY (rootfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", رستہ);

  دلیلاں.mounts = mounts;
  دلیلاں.len = len;
  دلیلاں.rootfd = rootfd;

  ret = لگاتار_callback_وچ_namespace (mntns_fd, do_restrict_mounts_وچ_a_container, &دلیلاں, err);
  إذا (UNLIKELY (ret > 0))
    أرجع *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  أرجع ret;
}

صحيح
libcrun_move_جال_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحيح netns_fd = -1;
  size_t i;
  صحيح ret;

  إذا (def == عدم || def->linux == عدم || def->linux->net_devices == عدم)
    أرجع 0;

  cleanup_free حرف *ns_رستہ = عدم;

  xasprintf (&ns_رستہ, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_رستہ, O_RDONLY, err);
  إذا (UNLIKELY (netns_fd < 0))
    أرجع netns_fd;

  لكل (i = 0; i < def->linux->net_devices->len; i++)
    {
      ثابت حرف *new_name = def->linux->net_devices->اقدار[i]->name ?: def->linux->net_devices->کلیدیں[i];

      ret = move_جال_device (def->linux->net_devices->کلیدیں[i], new_name, netns_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}
