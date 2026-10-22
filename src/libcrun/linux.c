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

#include <config.h>
#include "linux.h"
#include "utils.h"
#include "status.h"
#include <string.h>
#include <sched.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mount.h>
#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#ຖ້າ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#endif

#include <sys/prctl.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>
#include <signal.h>
#include "terminal.h"
#include "cgroup.h"
#include "cgroup-utils.h"
#include "status.h"
#include "criu.h"
#include "scheduler.h"
#include "intelrdt.h"
#include "io_priority.h"
#include "net_device.h"

#include <sys/socket.h>
#include <libgen.h>
#include <sys/wait.h>
#include <sys/vfs.h>
#include <sys/statvfs.h>
#include <limits.h>
#include <inttypes.h>
#include <sys/personality.h>
#include <net/if.h>
#include <sys/xattr.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sched.h>
#include <linux/sched.h>
#include <linux/magic.h>
#include <linux/nsfs.h>

#include <json-c/json.h>
#include <ocispec/json_common.h>

#include "mount_flags.h"
#include "syscalls.h"

#ifndef RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#endif

#ifndef OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#endif

#ifndef OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#endif

#ifndef OPEN_TREE_NAMESPACE
#  define OPEN_TREE_NAMESPACE 2
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
static unsigned long
mount_flags_from_statfs (unsigned long f_flags)
{
  unsigned long flags = 0;

  ຖ້າ (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  ຖ້າ (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  ຖ້າ (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  ຖ້າ (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  ຖ້າ (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  ຖ້າ (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  ຖ້າ (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  ຖ້າ (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  ຖ້າ (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  ສົ່ງຄືນ flags;
}

sa_nracanaa remount_s
{
  sa_nracanaa remount_s *next;
  var_na *target;
  ຈຳນວນເຕັມ targetfd;
  unsigned long flags;
  var_na *data;
};

sa_nracanaa private_data_s
{
  sa_nracanaa remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  ຈຳນວນເຕັມ unshare_flags;
  ຈຳນວນເຕັມ unshare_cgroupns;

  var_na *host_notify_socket_path;
  var_na *container_notify_socket_path;
  var_na *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  ຈຳນວນເຕັມ procfd;

  const var_na *rootfs;
  ຈຳນວນເຕັມ rootfsfd;

  ຈຳນວນເຕັມ notify_socket_tree_fd;
  ຈຳນວນເຕັມ old_root_fd;

  sa_nracanaa libcrun_fd_map *mount_fds;
  sa_nracanaa libcrun_fd_map *dev_fds;
  sa_nracanaa libcrun_fd_map *needed_devs_fds;

  var_na **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  var_na *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  ຈຳນວນເຕັມ maskdir_fd;
  var_na *maskdir_proc_path;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_pivot;
  bool no_pivot;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_root_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  var_na *host_rootfs;
};

sa_nracanaa linux_namespace_s
{
  const var_na *name;
  const var_na *ns_file;
  ຈຳນວນເຕັມ value;
};

static void
cleanup_private_data (void *private_data)
{
  sa_nracanaa private_data_s *p = private_data;

  ຖ້າ (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  ຖ້າ (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  ຖ້າ (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  ຖ້າ (p->old_root_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_root_fd));
  ຖ້າ (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  ຖ້າ (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  ຖ້າ (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  ຖ້າ (p->copy_symlink_targets)
    {
      size_t i;
      ສຳລັບ (i = 0; i < p->n_copy_symlink_targets; i++)
        free (p->copy_symlink_targets[i]);
      free (p->copy_symlink_targets);
    }
  free (p->unified_cgroup_path);
  free (p->host_notify_socket_path);
  free (p->container_notify_socket_path);
  free (p->external_descriptors);
  free (p->maskdir_proc_path);
  free (p->host_rootfs);
  free (p);
}

static sa_nracanaa private_data_s *
get_private_data (sa_nracanaa libcrun_container_s *container)
{
  ຖ້າ (container->private_data == NULL)
    {
      sa_nracanaa private_data_s *p = xmalloc0 (parimaa_na (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_tree_fd = -1;
      p->old_root_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  ສົ່ງຄືນ container->private_data;
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

static sa_nracanaa linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { NULL, NULL, 0 } };

static ຈຳນວນເຕັມ
get_and_reset (ຈຳນວນເຕັມ *old)
{
  ຈຳນວນເຕັມ tmp = *old;
  *old = -1;
  ສົ່ງຄືນ tmp;
}

ຈຳນວນເຕັມ
libcrun_find_namespace (const var_na *name)
{
  sa_nracanaa linux_namespace_s *it;
  ສຳລັບ (it = namespaces; it->name; it++)
    ຖ້າ (strcmp (it->name, name) == 0)
      ສົ່ງຄືນ it->value;
  ສົ່ງຄືນ -1;
}

static ຈຳນວນເຕັມ
syscall_keyctl_join (const var_na *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  ສົ່ງຄືນ (ຈຳນວນເຕັມ) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

static ຈຳນວນເຕັມ
syscall_pidfd_open (pid_t pid, unsigned ຈຳນວນເຕັມ flags)
{
#ຖ້າ defined __NR_pidfd_open
  ສົ່ງຄືນ (ຈຳນວນເຕັມ) syscall (__NR_pidfd_open, pid, flags);
#ບໍ່ດັ່ງນັ້ນ
  (void) pid;
  (void) flags;
  errno = ENOSYS;
  ສົ່ງຄືນ -1;
#endif
}

static ຈຳນວນເຕັມ
syscall_pidfd_send_signal (ຈຳນວນເຕັມ pidfd, ຈຳນວນເຕັມ sig, siginfo_t *info, unsigned ຈຳນວນເຕັມ flags)
{
#ຖ້າ defined __NR_pidfd_send_signal
  ສົ່ງຄືນ (ຈຳນວນເຕັມ) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#ບໍ່ດັ່ງນັ້ນ
  (void) pidfd;
  (void) sig;
  (void) info;
  (void) flags;
  errno = ENOSYS;
  ສົ່ງຄືນ -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
static uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  ຖ້າ (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  ຖ້າ (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  ຖ້າ (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  ຖ້າ (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  ຖ້າ (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  ຖ້າ (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  ຖ້າ (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  ຖ້າ (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  ສົ່ງຄືນ attr;
}

static ຈຳນວນເຕັມ
do_mount_setattr (bool recursive, const var_na *target, ຈຳນວນເຕັມ targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  sa_nracanaa mount_attr_s attr = {
    0,
  };
  ຈຳນວນເຕັມ ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  ຖ້າ (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      ຖ້າ (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      ບໍ່ດັ່ງນັ້ນ
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      ຖ້າ (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "mount_setattr `/%s`", target);

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
get_bind_mount (ຈຳນວນເຕັມ dirfd, const var_na *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ open_tree_fd = -1;
  sa_nracanaa mount_attr_s attr = {
    0,
  };
  ຈຳນວນເຕັມ recursive_flag = (recursive ? AT_RECURSIVE : 0);
  ຈຳນວນເຕັມ ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  ຖ້າ (UNLIKELY (src == NULL))
    ສົ່ງຄືນ crun_make_error (err, EINVAL, "mount source is not specified");

  ຖ້າ (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  ຖ້າ (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_tree_fd = syscall_open_tree (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  ຖ້າ (UNLIKELY (open_tree_fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_tree_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "mount_setattr `%s`", src);

  ສົ່ງຄືນ get_and_reset (&open_tree_fd);
}

ຈຳນວນເຕັມ
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, var_na *option, var_na **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free var_na *mappings = NULL;
  const var_na *it;
  ຈຳນວນເຕັມ ret;

  mappings = xmalloc (allocated);

  ສຳລັບ (it = option; *it;)
    {
      bool relative = false;
      long value[3];
      size_t i;

      ຖ້າ (*it == '\0')
        bha_nga;

      ຖ້າ (*it == '#')
        it++;

      ຖ້າ (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      ສຳລັບ (i = 0; i < 3; i++)
        {
          var_na *endptr = NULL;

          ຖ້າ (i > 0 && *it == '-')
            it++;

          ຖ້າ (*it == '\0')
            ສົ່ງຄືນ crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          value[i] = strtol (it, &endptr, 10);
          ຖ້າ (errno || endptr == it)
            ສົ່ງຄືນ crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      ຖ້າ (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          ຖ້າ (def == NULL
              || def->linux == NULL
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            ສົ່ງຄືນ crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          ສຳລັບ (i = 0; i < mappings_len; i++)
            ຖ້າ (value[1] >= (int64_t) mappings[i]->container_id
                && value[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              bha_nga;

          ຖ້າ (i == mappings_len)
            ສົ່ງຄືນ crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          value[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      ຖ້າ (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", value[0], value[1], value[2]);
      ຖ້າ (UNLIKELY (ret >= (ຈຳນວນເຕັມ) (allocated - written)))
        ສົ່ງຄືນ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = NULL;

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
format_mount_mappings (var_na **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  const size_t max_len_mapping = 64;
  cleanup_free var_na *ret = NULL;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  ສຳລັບ (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      ຖ້າ (UNLIKELY (len >= max_len_mapping))
        ສົ່ງຄືນ crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = NULL;
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
format_mount_mapping (var_na **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t size, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .size = size,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    NULL,
  };

  ສົ່ງຄືນ format_mount_mappings (ret, mappings, 1, written, err);
}

static bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  ຖ້າ (def->linux == NULL)
    ສົ່ງຄືນ mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  ຖ້າ (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    ສົ່ງຄືນ false;

  ຖ້າ (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    ສົ່ງຄືນ false;

  ສຳລັບ (s = 0; s < mnt->uid_mappings_len; s++)
    {
      ຖ້າ (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        ສົ່ງຄືນ false;
      ຖ້າ (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        ສົ່ງຄືນ false;
      ຖ້າ (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        ສົ່ງຄືນ false;
    }

  ສຳລັບ (s = 0; s < mnt->gid_mappings_len; s++)
    {
      ຖ້າ (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        ສົ່ງຄືນ false;
      ຖ້າ (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        ສົ່ງຄືນ false;
      ຖ້າ (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        ສົ່ງຄືນ false;
    }

  ສົ່ງຄືນ true;
}

static pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        const var_na *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != NULL;

  ຖ້າ (! need_new_userns)
    ສົ່ງຄືນ 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, NULL);
  ຖ້າ (UNLIKELY (pid < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "clone");

  ຖ້າ (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      ໃນຂະນະທີ່ (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  ຖ້າ (mnt->uid_mappings_len)
    {
      cleanup_free var_na *uid_map = NULL;
      cleanup_free var_na *gid_map = NULL;
      size_t written = 0;
      ຈຳນວນເຕັມ ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      cleanup_close ຈຳນວນເຕັມ fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      ຖ້າ (UNLIKELY (fd < 0))
        ສົ່ງຄືນ fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      ຖ້າ (UNLIKELY (fd < 0))
        ສົ່ງຄືນ fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      cleanup_free var_na *dup_options = NULL;
      var_na *option, *saveptr = NULL;

      ຖ້າ (! options)
        ສົ່ງຄືນ crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      ສຳລັບ (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (NULL, ";", &saveptr))
        {
          cleanup_free var_na *mappings = NULL;
          cleanup_close ຈຳນວນເຕັມ map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          ຈຳນວນເຕັມ ret;

          ຖ້າ (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              ຖ້າ (UNLIKELY (map_fd < 0))
                ສົ່ງຄືນ map_fd;
            }
          ບໍ່ດັ່ງນັ້ນ ຖ້າ (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              ຖ້າ (UNLIKELY (map_fd < 0))
                ສົ່ງຄືນ map_fd;
            }
          ບໍ່ດັ່ງນັ້ນ
            ສົ່ງຄືນ crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_create_keyring (libcrun_container_t *container, const var_na *name, const var_na *label, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ labelfd = -1;
  bool label_set = false;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      ຖ້າ (UNLIKELY (labelfd < 0))
        {
          ຖ້າ (crun_error_get_errno (err) != ENOENT)
            ສົ່ງຄືນ labelfd;

          crun_error_release (err);
        }

      ຖ້າ (labelfd >= 0)
        {
          ret = write (labelfd, label, strlen (label));
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      ຖ້າ (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          goto out;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      goto out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  ຖ້າ (label_set && write (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  ສົ່ງຄືນ ret;
}

static void
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  ຖ້າ (def->process && def->process->user)
    {
      ຖ້າ (def->process->user->uid)
        *uid = def->process->user->uid;
      ຖ້າ (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

static unsigned long
get_mount_flags (const var_na *name, ຈຳນວນເຕັມ current_flags, ຈຳນວນເຕັມ *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  const sa_nracanaa propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  ຖ້າ (found)
    *found = prop ? 1 : 0;

  ຖ້າ (prop == NULL)
    ສົ່ງຄືນ 0;

  ຖ້າ (prop->extra_flags & OPTION_RECURSIVE)
    {
      ຖ້າ (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      ຖ້າ (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  ຖ້າ (extra_flags)
    *extra_flags |= prop->extra_flags;

  ຖ້າ (prop->clear)
    ສົ່ງຄືນ current_flags & ~prop->flags;

  ສົ່ງຄືນ current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
static bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ສຳລັບ (i = 0; i < mount->options_len; i++)
    {
      const sa_nracanaa propagation_flags_s *prop;

      ຖ້າ (mount->options[i] == NULL)
        continue;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ຖ້າ (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        ສົ່ງຄືນ true;
    }

  ສົ່ງຄືນ false;
}

static unsigned long
get_mount_flags_or_option (const var_na *name, ຈຳນວນເຕັມ current_flags, unsigned long *extra_flags, var_na **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  ຈຳນວນເຕັມ found;
  __attribute__ ((unused)) cleanup_free var_na *prev = NULL;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  ຖ້າ (name == NULL)
    ສົ່ງຄືນ 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  ຖ້າ (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      const var_na *data_flag = NULL;
      ຖ້າ ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      ບໍ່ດັ່ງນັ້ນ ຖ້າ ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      ຖ້າ (data_flag)
        {
          prev = *option;
          ຖ້າ (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          ບໍ່ດັ່ງນັ້ນ
            *option = xstrdup (data_flag);
        }
      ສົ່ງຄືນ flags;
    }

  prev = *option;
  ຖ້າ (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  ບໍ່ດັ່ງນັ້ນ
    *option = xstrdup (name);

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
pivot_root (const var_na *new_root, const var_na *put_old)
{
  ສົ່ງຄືນ syscall (__NR_pivot_root, new_root, put_old);
}

static void
free_remount (sa_nracanaa remount_s *r)
{
  ຖ້າ (r == NULL)
    ສົ່ງຄືນ;
  ຖ້າ (r->targetfd >= 0)
    close (r->targetfd);
  free (r->data);
  free (r->target);
  free (r);
}

static sa_nracanaa remount_s *
make_remount (ຈຳນວນເຕັມ targetfd, const var_na *target, unsigned long flags, const var_na *data, sa_nracanaa remount_s *next)
{
  sa_nracanaa remount_s *ret = xmalloc (parimaa_na (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : NULL;
  ret->next = next;
  ret->targetfd = targetfd;
  ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ
do_remount (ຈຳນວນເຕັມ targetfd, const var_na *target, unsigned long flags, const var_na *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  ຈຳນວນເຕັມ ret;
  const var_na *real_target = target;

  ຖ້າ (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      ຖ້າ (LIKELY (ret == 0))
        ສົ່ງຄືນ 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  ຖ້າ (flags & (MS_REMOUNT | MS_RDONLY))
    data = NULL;

  ret = mount (NULL, real_target, NULL, flags, data);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      sa_nracanaa statfs sfs;

      ret = statfs (real_target, &sfs);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      ຖ້າ (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      ຖ້າ ((flags | remount_flags) != flags)
        {
          ret = mount (NULL, real_target, NULL, flags | remount_flags, data);
          ຖ້າ (LIKELY (ret == 0))
            ສົ່ງຄືນ 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          ຖ້າ (current_flags & MS_RDONLY)
            ret = mount (NULL, real_target, NULL, flags | remount_flags | MS_RDONLY, data);
        }
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "remount `%s`", target);
    }
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret = 0;
  sa_nracanaa remount_s *r = get_private_data (container)->remounts;
  ໃນຂະນະທີ່ (r)
    {
      sa_nracanaa remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      ຖ້າ (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          ຖ້າ (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              continue;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      ຖ້າ (UNLIKELY (ret < 0))
        goto cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  ໃນຂະນະທີ່ (r)
    {
      sa_nracanaa remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = NULL;
  ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ
open_mount_target (libcrun_container_t *container, const var_na *target_rel, libcrun_error_t *err)
{
  const var_na *rootfs = get_private_data (container)->rootfs;
  ຈຳນວນເຕັມ rootfsfd = get_private_data (container)->rootfsfd;

  ຖ້າ (rootfsfd < 0)
    ສົ່ງຄືນ crun_make_error (err, 0, "invalid rootfs state");

  ສົ່ງຄືນ safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
static ຈຳນວນເຕັມ
fsopen_mount (const var_na *type, const var_na *source_name, const var_na *labeltype, const var_na *label, const var_na *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close ຈຳນວນເຕັມ fsfd = -1;
  ຈຳນວນເຕັມ ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ຖ້າ (UNLIKELY (fsfd < 0))
    ສົ່ງຄືນ fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (void) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  ຖ້າ (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ຖ້າ (data && data[0])
    {
      cleanup_free var_na *data_copy = xstrdup (data);
      var_na *saveptr = NULL;
      var_na *token;

      ສຳລັບ (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (NULL, ",", &saveptr))
        {
          var_na *eq = strchr (token, '=');
          ຖ້າ (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          ບໍ່ດັ່ງນັ້ນ
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, NULL, 0);

          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#ບໍ່ດັ່ງນັ້ນ
  (void) type;
  (void) source_name;
  (void) data;
  errno = ENOSYS;
  ສົ່ງຄືນ -1;
#endif
}

static ຈຳນວນເຕັມ
get_procfd (sa_nracanaa private_data_s *data, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ fd;

  ຖ້າ (data->procfd >= 0)
    ສົ່ງຄືນ data->procfd;

  fd = fsopen_mount ("proc", NULL, NULL, NULL, NULL);
  ຖ້າ (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  ສົ່ງຄືນ fd;
}

static ຈຳນວນເຕັມ
get_old_root_fd (sa_nracanaa private_data_s *data)
{
  ຖ້າ (data->old_root_fd >= 0)
    ສົ່ງຄືນ data->old_root_fd;

  data->old_root_fd = open ("/", O_PATH | O_CLOEXEC);
  ສົ່ງຄືນ data->old_root_fd;
}

static ຈຳນວນເຕັມ
fs_move_mount_to (ຈຳນວນເຕັມ fd, ຈຳນວນເຕັມ dirfd, const var_na *name)
{
#ifdef HAVE_NEW_MOUNT_API
  ຖ້າ (name)
    ສົ່ງຄືນ syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  ສົ່ງຄືນ syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#ບໍ່ດັ່ງນັ້ນ
  (void) fd;
  (void) dirfd;
  (void) name;
  (void) syscall_move_mount;
  errno = ENOSYS;
  ສົ່ງຄືນ -1;
#endif
}

ga_nanaa
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

static ຈຳນວນເຕັມ do_mount (libcrun_container_t *container, const var_na *source, ຈຳນວນເຕັມ targetfd,
                     const var_na *target, const var_na *fstype, unsigned long mountflags,
                     const void *data, ຈຳນວນເຕັມ extra_flags, libcrun_error_t *err);

static bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, const var_na *option)
{
  size_t i;
  ສຳລັບ (i = 0; i < mnt->options_len; i++)
    ຖ້າ (mnt->options[i] != NULL && strcmp (mnt->options[i], option) == 0)
      ສົ່ງຄືນ true;
  ສົ່ງຄືນ false;
}

static runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, const var_na *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  ສຳລັບ (i = 0; i < def->mounts_len; i++)
    {
      ຖ້າ (strcmp (def->mounts[i]->destination, destination) == 0)
        ສົ່ງຄືນ def->mounts[i];
    }
  ສົ່ງຄືນ NULL;
}

static void
warn_tmpfs_fallback_once (sa_nracanaa private_data_s *private_data, const var_na *reason)
{
  ຖ້າ (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
static ຈຳນວນເຕັມ
get_shared_empty_dir_cached (libcrun_container_t *container, var_na **proc_fd_path, libcrun_error_t *err)
{
  sa_nracanaa private_data_s *private_data = get_private_data (container);
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  cleanup_free var_na *empty_dir_path = NULL;
  proc_fd_path_t fd_path;
  ຈຳນວນເຕັມ ret;

  /* Fast path: return cached proc fd path if already set up */
  ຖ້າ (private_data->maskdir_proc_path != NULL)
    {
      ຖ້າ (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      ສົ່ງຄືນ 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_root, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  ຖ້າ (fd < 0)
    ສົ່ງຄືນ fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  ຖ້າ (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  ສົ່ງຄືນ 0;
}

static bool
is_dev_null (const sa_nracanaa stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  ສົ່ງຄືນ S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
static ຈຳນວນເຕັມ
ensure_dev_null (ຈຳນວນເຕັມ fd, libcrun_error_t *err)
{
  sa_nracanaa stat st;

  ຖ້າ (UNLIKELY (fstat (fd, &st) < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "stat `/dev/null`");

  ຖ້າ (UNLIKELY (! is_dev_null (&st)))
    ສົ່ງຄືນ crun_make_error (err, 0, "`/dev/null` is not the null device");

  ສົ່ງຄືນ 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
static bool
is_empty_ro_tmpfs (ຈຳນວນເຕັມ fd)
{
  sa_nracanaa statfs sfs;

  ຖ້າ (fstatfs (fd, &sfs) < 0)
    ສົ່ງຄືນ false;

  ສົ່ງຄືນ sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

static ຈຳນວນເຕັມ
mount_masked_dir (libcrun_container_t *container, ຈຳນວນເຕັມ pathfd, const var_na *rel_path, libcrun_error_t *err)
{
  sa_nracanaa private_data_s *private_data = get_private_data (container);
  cleanup_close ຈຳນວນເຕັມ newfd = -1;
  libcrun_error_t tmp_err = NULL;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (private_data->maskdir_fd < 0)
    {
      ຖ້າ (private_data->maskdir_bind_failed)
        goto fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, NULL, &tmp_err);
      ຖ້າ (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->msg);
          crun_error_release (&tmp_err);
          goto fallback_to_tmpfs;
        }
    }

  ຖ້າ (private_data->maskdir_fd >= 0)
    {
      ຈຳນວນເຕັມ rootfsfd = private_data->rootfsfd;
      sa_nracanaa stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, NULL);
      ຖ້າ (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          ຖ້າ (UNLIKELY (newfd < 0))
            ສົ່ງຄືນ newfd;

          ret = fstat (newfd, &after);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          ຖ້າ (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            ສົ່ງຄືນ crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          ສົ່ງຄືນ 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_path);

      TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
      private_data->maskdir_fd = -1;
      private_data->maskdir_move_failed = true;
    }

  private_data->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_data, tmp_err ? tmp_err->msg : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  ຖ້າ (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      ຖ້າ (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      ບໍ່ດັ່ງນັ້ນ
        {
          ຖ້າ (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
do_masked_or_readonly_path (libcrun_container_t *container, const var_na *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  const var_na *rootfs = get_private_data (container)->rootfs;
  cleanup_close ຈຳນວນເຕັມ pathfd = -1;
  sa_nracanaa statfs sfs;
  ຈຳນວນເຕັມ ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  ຖ້າ (UNLIKELY (rel_path == NULL))
    ສົ່ງຄືນ crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  ຖ້າ (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  ຖ້າ (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      ຖ້າ (errno != ENOENT && errno != EACCES)
        ສົ່ງຄືນ pathfd;

      ຖ້າ (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      ສົ່ງຄືນ 0;
    }

  ຖ້າ (readonly)
    {
      ຈຳນວນເຕັມ procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close ຈຳນວນເຕັມ mountfd = -1;

      ຖ້າ (UNLIKELY (procfd < 0))
        ສົ່ງຄືນ procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      ຖ້າ (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          ຖ້າ (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, NULL);
          ບໍ່ດັ່ງນັ້ນ
            ret = -1;

          ຖ້າ (ret < 0)
            crun_error_release (err);
        }

      ຖ້າ (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          ຖ້າ (keep_flags)
            {
              cleanup_close ຈຳນວນເຕັມ sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              ຖ້າ (UNLIKELY (sfd < 0))
                ສົ່ງຄືນ crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              ຖ້າ (UNLIKELY (ret < 0))
                ສົ່ງຄືນ crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          ຖ້າ (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              ຖ້າ (UNLIKELY (ret < 0))
                ສົ່ງຄືນ ret;

              ret = fs_move_mount_to (mountfd, pathfd, NULL);
              ຖ້າ (UNLIKELY (ret < 0))
                ສົ່ງຄືນ crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          ບໍ່ດັ່ງນັ້ນ
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, NULL, mount_flags, NULL,
                              LABEL_NONE, err);
              ຖ້າ (UNLIKELY (ret < 0))
                ສົ່ງຄືນ ret;
            }
        }
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      ret = get_file_type_fd (pathfd, &mode);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      ຖ້າ ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      ບໍ່ດັ່ງນັ້ນ
        {
          cleanup_close ຈຳນວນເຕັມ nullfd = -1;
          cleanup_close ຈຳນວນເຕັມ mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          ຖ້າ (UNLIKELY (nullfd < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          ຖ້າ (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, NULL);

          ຖ້າ (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, NULL, MS_BIND | MS_RDONLY, NULL,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ສົ່ງຄືນ 0;
}

static const var_na *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  const var_na *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  ຖ້າ (context_type)
    {
      ຖ້າ (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          ສົ່ງຄືນ NULL;
        }
      ສົ່ງຄືນ context_type;
    }

  ສົ່ງຄືນ "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
static ຈຳນວນເຕັມ
diagnose_mount_failure (ຈຳນວນເຕັມ ret, libcrun_error_t *err, libcrun_container_t *container,
                        const var_na *fstype)
{
  ຖ້າ (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      ຖ້າ (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          ສົ່ງຄືນ ret;
        }
    }
  ສົ່ງຄືນ ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
static ຈຳນວນເຕັມ
sysfs_userns_fallback (libcrun_container_t *container, const var_na *target,
                       const var_na *real_target, ຈຳນວນເຕັມ targetfd,
                       unsigned long mountflags, const var_na *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close ຈຳນວນເຕັມ mountfd = -1;
  cleanup_close ຈຳນວນເຕັມ sysfd = -1;
  ຈຳນວນເຕັມ sys_old_root_fd = -1;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    ສົ່ງຄືນ 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (ret == 0)
    ສົ່ງຄືນ 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_root_fd = get_old_root_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  ຖ້າ (sys_old_root_fd >= 0)
    mountfd = get_bind_mount (sys_old_root_fd, "sys", true, true, false, MS_PRIVATE, err);
  ຖ້າ (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  ຖ້າ (cgroup_mount == NULL)
    {
      ຖ້າ (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, NULL);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          crun_error_release (err);
          ຖ້າ (sys_old_root_fd >= 0)
            {
              cleanup_free var_na *sys_path = NULL;
              proc_fd_path_t sys_root_path;

              get_proc_self_fd_path (sys_root_path, sys_old_root_fd);
              xasprintf (&sys_path, "%s/sys", sys_root_path);
              ret = mount (sys_path, real_target, NULL, MS_BIND | MS_REC, NULL);
            }
          ບໍ່ດັ່ງນັ້ນ
            ret = mount ("/sys", real_target, NULL, MS_BIND | MS_REC, NULL);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      ຖ້າ (UNLIKELY (sysfd < 0))
        ສົ່ງຄືນ sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        NULL, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ສົ່ງຄືນ do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  ຖ້າ (UNLIKELY (mountfd < 0))
    ສົ່ງຄືນ mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  ຖ້າ (UNLIKELY (sysfd < 0))
    ສົ່ງຄືນ sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    NULL, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
static void
apply_selinux_xattr (libcrun_container_t *container, ຈຳນວນເຕັມ fd, ຈຳນວນເຕັມ extra_flags, const var_na *label)
{
  libcrun_error_t tmp_err = NULL;
  ຈຳນວນເຕັມ procfd;

  ຖ້າ ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    ສົ່ງຄືນ;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  ຖ້າ (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close ຈຳນວນເຕັມ xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      ຖ້າ (xfd >= 0)
        (void) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
static ຈຳນວນເຕັມ
do_mount_replace_rootfs (libcrun_container_t *container, ຈຳນວນເຕັມ *fd, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ procfd = get_procfd (get_private_data (container), err);
  ຈຳນວນເຕັມ tmp;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (UNLIKELY (procfd < 0))
    ສົ່ງຄືນ procfd;

  {
    cleanup_close ຈຳນວນເຕັມ nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    ຖ້າ (UNLIKELY (nsfd < 0))
      ສົ່ງຄືນ crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    ຖ້າ (UNLIKELY (ret < 0))
      ສົ່ງຄືນ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  ຖ້າ (UNLIKELY (*fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  ຖ້າ (UNLIKELY (tmp < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  ສົ່ງຄືນ 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
static ຈຳນວນເຕັມ
apply_propagation_flags (const var_na *target, ຈຳນວນເຕັມ targetfd, const var_na *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (targetfd >= 0)
    {
      libcrun_error_t tmp_err = NULL;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      ຖ້າ (LIKELY (ret == 0))
        propagation_done = true;
      ບໍ່ດັ່ງນັ້ນ
        crun_error_release (&tmp_err);
    }

  ຖ້າ (! propagation_done)
    {
      ret = mount (NULL, real_target, NULL, mountflags & ALL_PROPAGATIONS, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  ສົ່ງຄືນ 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
static ຈຳນວນເຕັມ
schedule_or_do_remount (libcrun_container_t *container, ຈຳນວນເຕັມ *fd, ຈຳນວນເຕັມ targetfd,
                        const var_na *target, const var_na *real_target,
                        bool single_instance, unsigned long mountflags,
                        const void *data, ຈຳນວນເຕັມ extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  sa_nracanaa remount_s *r;

  ຖ້າ ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    ສົ່ງຄືນ do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  ຖ້າ (*fd < 0)
    {
      *fd = dup (targetfd);
      ຖ້າ (UNLIKELY (*fd < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
do_mount (libcrun_container_t *container, const var_na *source, ຈຳນວນເຕັມ targetfd,
          const var_na *target, const var_na *fstype, unsigned long mountflags, const void *data,
          ຈຳນວນເຕັມ extra_flags, libcrun_error_t *err)
{
  cleanup_free var_na *data_with_label = NULL;
  const var_na *data_without_label = data;
  proc_fd_path_t target_buffer;
  const var_na *context_type = NULL;
  cleanup_close ຈຳນວນເຕັມ ms_move_fd = -1;
  const var_na *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  const var_na *label = NULL;
  ຈຳນວນເຕັມ ret = 0;

  ຖ້າ (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  ບໍ່ດັ່ງນັ້ນ
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  ຖ້າ (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  ຖ້າ ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      ຖ້າ (UNLIKELY (context_type == NULL))
        ສົ່ງຄືນ -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      ຖ້າ (ret < 0)
        ສົ່ງຄືນ ret;
      data = data_with_label;
    }

  ຖ້າ (mountflags & MS_MOVE)
    {
      ຖ້າ ((mountflags & MS_BIND) || fstype)
        ສົ່ງຄືນ crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, NULL, MS_MOVE, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      ຖ້າ (UNLIKELY (ms_move_fd < 0))
        ສົ່ງຄືນ ms_move_fd;
      targetfd = ms_move_fd;
    }

  ຖ້າ ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      ຖ້າ (targetfd >= 0)
        {
          ຖ້າ (mountflags & MS_BIND)
            {
              cleanup_close ຈຳນວນເຕັມ bindfd = -1;
              libcrun_error_t tmp_err = NULL;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              ຖ້າ (bindfd < 0)
                {
                  ຈຳນວນເຕັມ old_root_fd = get_old_root_fd (get_private_data (container));

                  ຖ້າ (old_root_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_root_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              ຖ້າ (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, NULL);
              ຖ້າ (ret < 0)
                crun_error_release (&tmp_err);
            }
          ບໍ່ດັ່ງນັ້ນ
            {
              cleanup_close ຈຳນວນເຕັມ newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              ຖ້າ (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, NULL);
            }
        }

      ຖ້າ (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          ຖ້າ (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              ຈຳນວນເຕັມ old_root_fd = get_old_root_fd (get_private_data (container));

              ຖ້າ (old_root_fd >= 0)
                {
                  cleanup_free var_na *proc_source_buf = NULL;
                  proc_fd_path_t old_root_path;

                  get_proc_self_fd_path (old_root_path, old_root_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_root_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      ຖ້າ (UNLIKELY (ret < 0))
        {
          ຈຳນວນເຕັມ saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          ຖ້າ (ret == 0)
            ສົ່ງຄືນ 0;
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          ສົ່ງຄືນ diagnose_mount_failure (ret, err, container, fstype);
        }

      ຖ້າ (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          ຖ້າ (UNLIKELY (fd < 0))
            ສົ່ງຄືນ fd;

          /* We are replacing the rootfs, reopen it.  */
          ຖ້າ (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              ຖ້າ (UNLIKELY (ret < 0))
                ສົ່ງຄືນ ret;
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

  ຖ້າ (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ຖ້າ (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  ຖ້າ (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  ຖ້າ (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ ret;
}

static void
try_umount (ຈຳນວນເຕັມ targetfd, const var_na *target)
{
  proc_fd_path_t target_buffer;
  const var_na *real_target = target;

  ຖ້າ (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

static bool
container_has_cgroupns (libcrun_container_t *container)
{
  ສົ່ງຄືນ get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

static ຈຳນວນເຕັມ
do_mount_cgroup_v2 (libcrun_container_t *container, ຈຳນວນເຕັມ targetfd, const var_na *target,
                    unsigned long mountflags, const var_na *data, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  ຈຳນວນເຕັມ cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (UNLIKELY (cgroup_mode < 0))
    ສົ່ງຄືນ cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      ຈຳນວນເຕັມ saved_errno = crun_error_get_errno (err);
      ຖ້າ (saved_errno == EPERM || saved_errno == EBUSY)
        {
          const var_na *unified_cgroup_path;
          const var_na *src_cgroup;

          crun_error_release (err);

          ຖ້າ (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              ຖ້າ (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  ຖ້າ (LIKELY (ret == 0))
                    ສົ່ງຄືນ ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_ROOT;
          ສົ່ງຄືນ do_mount (container, src_cgroup, targetfd, target, NULL,
                           MS_BIND | mountflags, NULL, LABEL_NONE, err);
        }

      ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, const var_na *source, ຈຳນວນເຕັມ targetfd, const var_na *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  const var_na *subsystem = "systemd";
  cleanup_free var_na *subsystem_path = NULL;
  cleanup_close ຈຳນວນເຕັມ tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  ຖ້າ (UNLIKELY (tmpfsdirfd < 0))
    ສົ່ງຄືນ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "openat `%s`", subsystem_path);

  ສົ່ງຄືນ do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

static ຈຳນວນເຕັມ
do_mount_cgroup_v1 (libcrun_container_t *container, const var_na *source, ຈຳນວນເຕັມ targetfd, const var_na *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  cleanup_free var_na *content = NULL;
  var_na *from;
  cleanup_close ຈຳນວນເຕັມ tmpfsdirfd = -1;
  var_na *saveptr = NULL;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  ຖ້າ (UNLIKELY (tmpfsdirfd < 0))
    ສົ່ງຄືນ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    ຈຳນວນເຕັມ procfd = get_procfd (get_private_data (container), err);
    ຖ້າ (UNLIKELY (procfd < 0))
      ສົ່ງຄືນ procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, NULL, err);
  }
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (UNLIKELY (content == NULL || content[0] == '\0'))
    ສົ່ງຄືນ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ສຳລັບ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (NULL, "\n", &saveptr))
    {
      cleanup_free var_na *source_path = NULL;
      cleanup_free var_na *source_subsystem = NULL;
      cleanup_free var_na *subsystem_path = NULL;
      var_na *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close ຈຳນວນເຕັມ subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      ຖ້າ (subsystem[0] == '\0')
        continue;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      ຖ້າ (it)
        subsystem = it + 5;

      ຖ້າ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ຖ້າ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_ROOT, subsystem, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      /* if there is already a mount specified, do not add a default one.  */
      ຖ້າ (find_mount_for (container, source_subsystem) != NULL)
        continue;

      ret = append_paths (&source_path, err, source_subsystem, subpath, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      ຖ້າ (UNLIKELY (subsystemfd < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", subsystem_path);

      ຖ້າ (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          ຖ້າ (UNLIKELY (ret < 0))
            {
              ຖ້າ (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  continue;
                }
              ສົ່ງຄືນ ret;
            }
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, NULL, MS_BIND | mountflags, NULL,
                          LABEL_NONE, err);
          ຖ້າ (UNLIKELY (ret < 0))
            {
              ຖ້າ (crun_error_get_errno (err) != ENOENT)
                ສົ່ງຄືນ ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, NULL, MS_BIND | mountflags,
                              NULL, LABEL_NONE, err);
              ຖ້າ (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  ຖ້າ (crun_error_get_errno (err) != ENOENT)
                    ສົ່ງຄືນ ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
do_mount_cgroup (libcrun_container_t *container, const var_na *source, ຈຳນວນເຕັມ targetfd, const var_na *target,
                 unsigned long mountflags, const var_na *data, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ຖ້າ (UNLIKELY (cgroup_mode < 0))
    ສົ່ງຄືນ cgroup_mode;

  switch (cgroup_mode)
    {
    pak_sa CGROUP_MODE_UNIFIED:
      ສົ່ງຄືນ do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    pak_sa CGROUP_MODE_LEGACY:
    pak_sa CGROUP_MODE_HYBRID:
      ສົ່ງຄືນ do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  ສົ່ງຄືນ crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

sa_nracanaa device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
static void
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  sa_nracanaa private_data_s *private_data = get_private_data (container);
  size_t i;

  ຖ້າ (private_data->needed_devs_fds)
    {
      sa_nracanaa libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      ສຳລັບ (i = 0; needed_devs[i].path; i++)
        {
          ຖ້າ (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            continue;

          ຈຳນວນເຕັມ fd = syscall_open_tree (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          ຖ້າ (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          ບໍ່ດັ່ງນັ້ນ ຖ້າ (fd >= 0)
            close (fd);
        }
    }

  ຖ້າ (private_data->notify_socket_tree_fd < 0
      && private_data->host_notify_socket_path)
    {
      ຈຳນວນເຕັມ fd = syscall_open_tree (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      ຖ້າ (fd >= 0)
        private_data->notify_socket_tree_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
static const var_na *
relative_path_under_dev (const var_na *path)
{
  ຖ້າ (path[0] != '/')
    ສົ່ງຄືນ path;
  ຖ້າ (strncmp (path, "/dev/", 5) == 0)
    {
      ຖ້າ (strchr (path + 5, '/'))
        ສົ່ງຄືນ NULL;
      ສົ່ງຄືນ path + 5;
    }
  ສົ່ງຄືນ NULL;
}

static ຈຳນວນເຕັມ
mknod_and_set_attrs (libcrun_container_t *container, ຈຳນວນເຕັມ dirfd, const var_na *name, sa_nracanaa device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  proc_fd_path_t fd_path;
  const var_na *what = "new";
  bool exists = false;
  sa_nracanaa stat st;
  ຈຳນວນເຕັມ procfd;
  ຈຳນວນເຕັມ ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      ຖ້າ (errno != EEXIST)
        ສົ່ງຄືນ crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  ຖ້າ (UNLIKELY ((st.st_mode & S_IFMT) != type))
    ສົ່ງຄືນ crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  ຖ້າ (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    ສົ່ງຄືນ crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  ຖ້າ (exists)
    ສົ່ງຄືນ 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  ຖ້າ (UNLIKELY (procfd < 0))
    ສົ່ງຄືນ procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fchown `%s`", device->path);

  ສົ່ງຄືນ 1;
}

ຈຳນວນເຕັມ
libcrun_create_dev (libcrun_container_t *container, ຈຳນວນເຕັມ devfd, ຈຳນວນເຕັມ srcfd,
                    sa_nracanaa device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  const var_na *fullname = device->path;
  const var_na *rootfs = get_private_data (container)->rootfs;
  ຖ້າ (is_empty_string (fullname))
    ສົ່ງຄືນ crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free var_na *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  ຖ້າ (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  const var_na *rel_dev = relative_path_under_dev (normalized_path);

  ຖ້າ (binds)
    {
      cleanup_close ຈຳນວນເຕັມ fd = -1;

      ຖ້າ (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          ຖ້າ (UNLIKELY (fd < 0))
            {
              ຖ້າ (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              ຖ້າ (UNLIKELY (fd < 0))
                ສົ່ງຄືນ crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          const var_na *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          ຖ້າ (UNLIKELY (fd < 0))
            ສົ່ງຄືນ fd;
        }

      ຖ້າ (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          ຖ້າ (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              ຖ້າ (LIKELY (ret >= 0))
                ສົ່ງຄືນ 0;
            }
          ບໍ່ດັ່ງນັ້ນ
            crun_error_release (err);
        }

      {
        cleanup_close ຈຳນວນເຕັມ mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        ຖ້າ (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            ຖ້າ (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, NULL);
                ຖ້າ (LIKELY (ret == 0))
                  ສົ່ງຄືນ 0;
              }
            ບໍ່ດັ່ງນັ້ນ
              crun_error_release (err);
          }
        ບໍ່ດັ່ງນັ້ນ
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, NULL, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, NULL, LABEL_MOUNT, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      ຖ້າ (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          ຖ້າ (ret <= 0)
            ສົ່ງຄືນ ret;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          cleanup_close ຈຳນວນເຕັມ dirfd = -1;
          cleanup_free var_na *dirname = NULL;
          var_na *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          ຖ້າ (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          ຖ້າ (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              ຖ້າ (UNLIKELY (dirfd < 0))
                ສົ່ງຄືນ crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          ບໍ່ດັ່ງນັ້ນ
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              ຖ້າ (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              ຖ້າ (UNLIKELY (dirfd < 0))
                ສົ່ງຄືນ dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          ຖ້າ (ret <= 0)
            ສົ່ງຄືນ ret;
        }
    }
  ສົ່ງຄືນ 0;
}

sa_nracanaa symlink_s
{
  const var_na *path;
  const var_na *target;
  bool force;
};

static sa_nracanaa symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { NULL, NULL, false } };

static ຈຳນວນເຕັມ
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  size_t i;
  sa_nracanaa device_s *it;
  cleanup_close ຈຳນວນເຕັມ devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  const var_na *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map sa_nracanaa libcrun_fd_map *dev_fds = NULL;
  cleanup_close_map sa_nracanaa libcrun_fd_map *needed_fds = NULL;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = NULL;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = NULL;

  ຖ້າ (! def || ! def->linux)
    ສົ່ງຄືນ 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  ຖ້າ (UNLIKELY (devfd < 0))
    ສົ່ງຄືນ devfd;

  ສຳລັບ (i = 0; i < def->linux->devices_len; i++)
    {
      sa_nracanaa device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      ຖ້າ (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສຳລັບ (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close ຈຳນວນເຕັມ srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
      ຖ້າ (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  ສຳລັບ (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          ຈຳນວນເຕັມ saved_errno = errno;

          ຖ້າ (errno == EEXIST && ! symlinks[i].force)
            continue;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          ຖ້າ (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              ຖ້າ (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              ຖ້າ (ret < 0 && errno == EBUSY)
                {
                  cleanup_close ຈຳນວນເຕັມ tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  ຖ້າ (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      ຖ້າ (umount2 (procpath, MNT_DETACH) == 0)
                        goto retry_unlink;
                    }
                }
              ຖ້າ (ret == 0)
                goto retry_symlink;
            }
          ສົ່ງຄືນ crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  ຖ້າ (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
static const var_na *
next_path_component (const var_na *p, size_t *len)
{
  ສຳລັບ (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      ຖ້າ (*len != 1 || p[0] != '.')
        ສົ່ງຄືນ p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
static bool
same_path (const var_na *a, const var_na *b)
{
  size_t la, lb;

  ສຳລັບ (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      ຖ້າ (la != lb || memcmp (a, b, la) != 0)
        ສົ່ງຄືນ false;
      ຖ້າ (la == 0)
        ສົ່ງຄືນ true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
static bool
is_duplicate_path (var_na **paths, size_t i)
{
  size_t j;

  ຖ້າ (paths[i] == NULL)
    ສົ່ງຄືນ false;

  ສຳລັບ (j = 0; j < i; j++)
    ຖ້າ (paths[j] && same_path (paths[j], paths[i]))
      ສົ່ງຄືນ true;

  ສົ່ງຄືນ false;
}

static ຈຳນວນເຕັມ
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  ຈຳນວນເຕັມ ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  ຖ້າ (! def || ! def->linux)
    ສົ່ງຄືນ 0;

  ສຳລັບ (i = 0; i < def->linux->masked_paths_len; i++)
    {
      ຖ້າ (is_duplicate_path (def->linux->masked_paths, i))
        continue;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ສຳລັບ (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
do_pivot (libcrun_container_t *container, const var_na *rootfs, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ oldrootfd = -1;
  cleanup_close ຈຳນວນເຕັມ newrootfd = -1;
  ຈຳນວນເຕັມ ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ຖ້າ (UNLIKELY (oldrootfd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ຖ້າ (UNLIKELY (newrootfd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_root (".", ".");
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, NULL, -1, ".", NULL, MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = umount2 (".", MNT_DETACH);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "umount2 oldroot");

  do
    {
      ret = umount2 (".", MNT_DETACH);
      ຖ້າ (ret < 0 && errno == EINVAL)
        bha_nga;
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "umount2 oldroot");
  } ໃນຂະນະທີ່ (ret == 0);

  ret = chdir ("/");
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "chdir to newroot");

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, var_na **data, libcrun_error_t *err)
{
  const var_na *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  sa_nracanaa stat st;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (*data != NULL && strstr (*data, "mode="))
    ສົ່ງຄືນ 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  ຖ້າ (fd < 0)
    {
      ຖ້າ (crun_error_get_errno (err) != ENOENT)
        ສົ່ງຄືນ fd;

      crun_error_release (err);
      ສົ່ງຄືນ 0;
    }
  ret = fstat (fd, &st);
  ຖ້າ (ret < 0)
    ສົ່ງຄືນ crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
get_default_flags (libcrun_container_t *container, const var_na *destination, var_na **data)
{
  ຖ້າ (strcmp (destination, "/proc") == 0)
    ສົ່ງຄືນ 0;
  ຖ້າ (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      ສົ່ງຄືນ MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  ຖ້າ (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      ສົ່ງຄືນ MS_NOEXEC | MS_STRICTATIME;
    }
  ຖ້າ (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      ສົ່ງຄືນ MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  ຖ້າ (strcmp (destination, "/dev/mqueue") == 0)
    ສົ່ງຄືນ MS_NOEXEC | MS_NOSUID | MS_NODEV;
  ຖ້າ (strcmp (destination, "/dev/pts") == 0)
    {
      ຖ້າ (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      ບໍ່ດັ່ງນັ້ນ
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      ສົ່ງຄືນ MS_NOEXEC | MS_NOSUID;
    }
  ຖ້າ (strcmp (destination, "/sys") == 0)
    ສົ່ງຄືນ MS_NOEXEC | MS_NOSUID | MS_NODEV;

  ສົ່ງຄືນ 0;
}

static const var_na *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  ສົ່ງຄືນ find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

static ຈຳນວນເຕັມ
check_valid_no_follow_file_system (ຈຳນວນເຕັມ fd, const var_na *destination, libcrun_error_t *err)
{
  sa_nracanaa statfs sfs;

  ຈຳນວນເຕັມ ret = fstatfs (fd, &sfs);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "statfs `%s`", destination);

  switch (sfs.f_type)
    {
    pak_sa PROC_SUPER_MAGIC:
    pak_sa SYSFS_MAGIC:
    pak_sa DEVPTS_SUPER_MAGIC:
      ສົ່ງຄືນ crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
safe_create_symlink (ຈຳນວນເຕັມ rootfsfd, const var_na *rootfs, const var_na *target, const var_na *destination, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ parent_dir_fd = -1;
  cleanup_free var_na *buffer = NULL;
  var_na *part;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (is_empty_string (destination))
    ສົ່ງຄືນ crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  ຖ້າ (UNLIKELY (parent_dir_fd < 0))
    ສົ່ງຄືນ parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      ຖ້າ (errno == EEXIST)
        {
          cleanup_free var_na *link = NULL;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          ຖ້າ (UNLIKELY (len < 0))
            ສົ່ງຄືນ len;

          ຖ້າ ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            ສົ່ງຄືນ 0;

          ສົ່ງຄືນ crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      ສົ່ງຄືນ crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
handle_copy_symlink (libcrun_container_t *container, const var_na *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     const var_na *link_target, libcrun_error_t *err)
{
  cleanup_free var_na *target = NULL;

  ຖ້າ (link_target == NULL)
    {
      ຈຳນວນເຕັມ old_root_fd = get_old_root_fd (get_private_data (container));
      const var_na *source = mount->source;
      ssize_t len;

      ຖ້າ (old_root_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_root_fd, source + 1, &target, 0, err);
      ບໍ່ດັ່ງນັ້ນ
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      ຖ້າ (UNLIKELY (len < 0))
        ສົ່ງຄືນ len;
      link_target = target;
    }

  ສົ່ງຄືນ safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

static ຈຳນວນເຕັມ
prepare_sysfs_or_proc_mount (libcrun_container_t *container, const var_na *target,
                             ຈຳນວນເຕັມ *targetfd, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      ຖ້າ (errno == ENOENT)
        {
          ຖ້າ (strchr (target, '/'))
            ສົ່ງຄືນ crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (errno == ENOTDIR)
        ສົ່ງຄືນ crun_make_error (err, errno, "the target `/%s` is invalid", target);

      ຖ້າ (ret < 0)
        ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
handle_tmpcopyup (libcrun_container_t *container, const var_na *rootfs, const var_na *target,
                  ຈຳນວນເຕັມ copy_from_fd, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ destfd, ret;
  cleanup_close ຈຳນວນເຕັມ tmpfd = copy_from_fd;

  target = consume_slashes (target);

  ຖ້າ (is_empty_string (target))
    ສົ່ງຄືນ crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  ຖ້າ (UNLIKELY (destfd < 0))
    ສົ່ງຄືນ crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ
get_mount_label_how (const var_na *type, bool is_sysfs_or_proc)
{
  ຖ້າ (is_sysfs_or_proc)
    ສົ່ງຄືນ LABEL_NONE;

  ຖ້າ (strcmp (type, "mqueue") == 0)
    ສົ່ງຄືນ LABEL_XATTR;

  ສົ່ງຄືນ LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
static unsigned long
get_bind_mount_locked_flags (ຈຳນວນເຕັມ source_mountfd, const var_na *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  sa_nracanaa statfs sfs;
  size_t i;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  ບໍ່ດັ່ງນັ້ນ
    ret = statfs (source, &sfs);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ 0;

  ສຳລັບ (i = 0; i < mount->options_len; i++)
    {
      const sa_nracanaa propagation_flags_s *prop;

      ຖ້າ (mount->options[i] == NULL)
        continue;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ຖ້າ (prop == NULL || (prop->extra_flags & OPTION_RECURSIVE))
        continue;

      ຖ້າ (prop->clear)
        cleared |= prop->flags;
      ບໍ່ດັ່ງນັ້ນ
        cleared &= ~prop->flags;
    }

  ສົ່ງຄືນ sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
static const var_na *
get_path_in_rootfs (const var_na *rootfs, const var_na *path)
{
  size_t len;

  ຖ້າ (rootfs == NULL || path == NULL)
    ສົ່ງຄືນ NULL;

  len = strlen (rootfs);
  ໃນຂະນະທີ່ (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  ຖ້າ (len == 0)
    ສົ່ງຄືນ NULL;

  ຖ້າ (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    ສົ່ງຄືນ NULL;

  ສົ່ງຄືນ consume_slashes (path + len);
}

static ຈຳນວນເຕັມ
process_single_mount (libcrun_container_t *container, const var_na *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      sa_nracanaa libcrun_fd_map *mount_fds, size_t mount_index,
                      const var_na *systemd_cgroup_v1, libcrun_error_t *err)
{
  const var_na *target = consume_slashes (mount->destination);
  cleanup_close ຈຳນວນເຕັມ source_mountfd = -1;
  cleanup_free var_na *data = NULL;
  var_na *type;
  var_na *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close ຈຳນວນເຕັມ copy_from_fd = -1;
  cleanup_close ຈຳນວນເຕັມ targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  ຈຳນວນເຕັມ no_remount = 0;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  ຖ້າ (mount->options == NULL)
    flags = get_default_flags (container, mount->destination, &data);
  ບໍ່ດັ່ງນັ້ນ
    {
      size_t j;

      ສຳລັບ (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  ຖ້າ (type == NULL && (flags & MS_BIND) == 0)
    ສົ່ງຄືນ crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  ຖ້າ ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      const var_na *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      ຖ້າ (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          ຖ້າ (UNLIKELY (source_mountfd < 0))
            ສົ່ງຄືນ crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  ຖ້າ ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  ຖ້າ (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      ຖ້າ (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      ຖ້າ (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  ຖ້າ (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ຖ້າ (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      const var_na *path = mount->source;

      ຖ້າ ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        ສົ່ງຄືນ crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      ຖ້າ (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, NULL);
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (path[0] == '/')
        {
          ຈຳນວນເຕັມ old_root_fd = get_old_root_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          ຖ້າ (old_root_fd >= 0)
            ret = get_file_type_at (old_root_fd, &src_mode, nofollow, (path[1] == '\0') ? NULL : path + 1);
          ບໍ່ດັ່ງນັ້ນ
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      ບໍ່ດັ່ງນັ້ນ
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "cannot stat `%s`", path);

      ຖ້າ (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          ຈຳນວນເຕັມ bind_dirfd = AT_FDCWD;
          const var_na *bind_source = mount->source;
          ຈຳນວນເຕັມ old_root_fd = get_old_root_fd (get_private_data (container));

          ຖ້າ (old_root_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_root_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          source_mountfd = ret;
        }
    }

  ຖ້າ (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      const var_na *cached_target = NULL;

      ຖ້າ (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      mounted = true;
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      bool is_dir = S_ISDIR (src_mode);

      ຖ້າ (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
          targetfd = ret;
        }
    }

  ຖ້າ (extra_flags & OPTION_TMPCOPYUP)
    {
      ຖ້າ (strcmp (type, "tmpfs") != 0)
        ສົ່ງຄືນ crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      ຖ້າ (UNLIKELY (copy_from_fd < 0))
        {
          ຖ້າ (errno != ENOTDIR)
            ສົ່ງຄືນ crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  ຖ້າ (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, NULL);
      ຖ້າ (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          ຖ້າ (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, NULL, source_mountfd, target, NULL, remaining_flags, data, LABEL_NONE | no_remount, err);
              ຖ້າ (UNLIKELY (ret < 0))
                ສົ່ງຄືນ ret;
            }

          mounted = true;

          ຖ້າ (is_empty_string (target))
            {
              ຈຳນວນເຕັມ new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              ຖ້າ (UNLIKELY (new_rootfsfd < 0))
                ສົ່ງຄືນ crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          ສົ່ງຄືນ crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (get_private_data (container)->host_root_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          ສົ່ງຄືນ crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  ຖ້າ (! mounted)
    {
      ຖ້າ (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          ຈຳນວນເຕັມ extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
    }

  ຖ້າ (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ຖ້າ (rec_clear || rec_set)
    {
      const bool is_dir = S_ISDIR (src_mode);
      cleanup_close ຈຳນວນເຕັມ dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      ຖ້າ (UNLIKELY (dfd < 0))
        ສົ່ງຄືນ crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
do_mounts (libcrun_container_t *container, const var_na *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  const var_na *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map sa_nracanaa libcrun_fd_map *mount_fds = NULL;
  size_t i;
  ຈຳນວນເຕັມ ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = NULL;

  ສຳລັບ (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ສົ່ງຄືນ 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
ຈຳນວນເຕັມ
libcrun_container_do_bind_mount (libcrun_container_t *container, var_na *mount_source, var_na *mount_destination, var_na **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  const var_na *target = consume_slashes (mount_destination);
  cleanup_free var_na *data = NULL;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close ຈຳນວນເຕັມ targetfd = -1;
  ຈຳນວນເຕັມ is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  const var_na *rootfs = get_private_data (container)->rootfs;
  ຈຳນວນເຕັມ ret;

  ຖ້າ ((get_private_data (container)->rootfsfd < 0) || (rootfs == NULL))
    ສົ່ງຄືນ crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  ຖ້າ (mount_options == NULL)
    flags = get_default_flags (container, mount_destination, &data);
  ບໍ່ດັ່ງນັ້ນ
    {
      size_t j;

      ສຳລັບ (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  ຖ້າ (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  ຖ້າ (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      ຖ້າ (UNLIKELY (is_dir < 0))
        ສົ່ງຄືນ is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
ຈຳນວນເຕັມ
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, ຈຳນວນເຕັມ *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close ຈຳນວນເຕັມ notify_fd = -1;
  cleanup_free var_na *host_notify_socket_path = NULL;
  cleanup_free var_na *state_dir = NULL;
  var_na *host_path = NULL;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (container && get_private_data (container)->host_notify_socket_path)
    {
      const var_na *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  ຖ້າ (host_path == NULL)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_root, context->id, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  ຖ້າ (UNLIKELY (notify_fd < 0))
    ສົ່ງຄືນ notify_fd;

  ຖ້າ (UNLIKELY (chmod (host_path, 0777) < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  ຖ້າ (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (void) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  ສົ່ງຄືນ 1;
#ບໍ່ດັ່ງນັ້ນ
  (void) context;
  (void) container;
  (void) err;
  *notify_socket_out = -1;
  ສົ່ງຄືນ 0;
#endif
}

#ifdef HAVE_SYSTEMD
static ຈຳນວນເຕັມ
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  const var_na *notify_socket = container->context->notify_socket;
  cleanup_free var_na *host_notify_socket_path = NULL;
  cleanup_free var_na *container_notify_socket_path = NULL;
  cleanup_free var_na *state_dir = NULL;
  uid_t container_root_uid = -1;
  gid_t container_root_gid = -1;
  ຈຳນວນເຕັມ notify_socket_tree_fd;

  ຖ້າ (notify_socket == NULL)
    ສົ່ງຄືນ 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_root : NULL),
                                     (container->context ? container->context->id : NULL), err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = mkdir (host_notify_socket_path, 0700);
  ຖ້າ (ret < 0)
    ສົ່ງຄືນ crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  ຖ້າ (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_root_in_the_userns (container->container_def, 0, 0, &container_root_uid, &container_root_gid);
      ຖ້າ (container_root_uid != ((uid_t) -1) && container_root_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_root_uid, container_root_gid);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_root_uid, container_root_gid);
        }
    }

  notify_socket_tree_fd = syscall_open_tree (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  ຖ້າ (notify_socket_tree_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_tree_fd = notify_socket_tree_fd;
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  ບໍ່ດັ່ງນັ້ນ
    /* some other error */
    ສົ່ງຄືນ crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = NULL;
  ສົ່ງຄືນ 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
static ຈຳນວນເຕັມ
do_finalize_notify_socket (libcrun_container_t *container, const var_na *rootfs, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  cleanup_free var_na *host_notify_socket_path = NULL;
  cleanup_free var_na *container_notify_socket_path = NULL;
  cleanup_free var_na *container_notify_socket_path_dir_alloc = NULL;
  const var_na *container_notify_socket_path_dir = NULL;
  cleanup_close ຈຳນວນເຕັມ notify_socket_tree_fd = -1;
  cleanup_close ຈຳນວນເຕັມ targetfd = -1;
  ຈຳນວນເຕັມ did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = NULL;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = NULL;

  ຖ້າ (host_notify_socket_path == NULL || container_notify_socket_path == NULL)
    ສົ່ງຄືນ 0;

  ຖ້າ (rootfs == NULL)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  ຖ້າ (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  targetfd = ret;

  notify_socket_tree_fd = get_private_data (container)->notify_socket_tree_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_tree_fd = -1;

  ຖ້າ (notify_socket_tree_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_tree_fd, targetfd, NULL);
      ຖ້າ (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          ຖ້າ (UNLIKELY (targetfd < 0))
            ສົ່ງຄືນ targetfd;

          ret = do_mount (container, NULL, targetfd, container_notify_socket_path_dir, NULL,
                          MS_REC | MS_PRIVATE, NULL, LABEL_NONE, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      ບໍ່ດັ່ງນັ້ນ
        ສົ່ງຄືນ crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_tree_fd,
                                container_notify_socket_path_dir);
    }

  ຖ້າ (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, NULL,
                      MS_BIND | MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
make_parent_mount_private (const var_na *rootfs, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ rootfsfd = -1;
  sa_nracanaa stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  ຖ້າ (UNLIKELY (rootfsfd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", rootfs);

  ຖ້າ (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fstat `%s`", rootfs);

  ສຳລັບ (;;)
    {
      sa_nracanaa stat cur_st;
      libcrun_error_t tmp_err = NULL;
      ຈຳນວນເຕັມ ret;
      cleanup_close ຈຳນວນເຕັມ parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      ຖ້າ (ret == 0)
        ສົ່ງຄືນ 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (NULL, proc_path, NULL, MS_PRIVATE, NULL);
      ຖ້າ (ret == 0)
        ສົ່ງຄືນ 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      ຖ້າ (parentfd < 0)
        {
          ຈຳນວນເຕັມ saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          ຖ້າ (ret != 0)
            ສົ່ງຄືນ crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          ສົ່ງຄືນ crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      ຖ້າ (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      ຖ້າ (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        bha_nga;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  ສົ່ງຄືນ crun_make_error (err, 0, "make `%s` private", rootfs);
}

ຈຳນວນເຕັມ
libcrun_set_mounts (sa_nracanaa container_entrypoint_s *entrypoint_args, libcrun_container_t *container, const var_na *rootfs, set_mounts_cb_t cb, void *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ຈຳນວນເຕັມ is_user_ns = 0;
  ຈຳນວນເຕັມ ret = 0;

  ຖ້າ (rootfs == NULL || def->mounts == NULL)
    ສົ່ງຄືນ 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  ຖ້າ (def->root->readonly)
    {
      sa_nracanaa remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      ຈຳນວນເຕັມ fd;

      fd = dup (get_private_data (container)->rootfsfd);
      ຖ້າ (UNLIKELY (fd < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, NULL, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  ຖ້າ (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      ຖ້າ (UNLIKELY (is_user_ns < 0))
        ສົ່ງຄືນ is_user_ns;
    }

  ຖ້າ (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  ຖ້າ (cb)
    {
      ret = cb (cb_data, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ຖ້າ (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = NULL;
      const var_na *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (void) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
umount_or_hide (const var_na *target, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;

  ret = umount2 (target, MNT_DETACH);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      ຈຳນວນເຕັມ saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      ຖ້າ (errno == EINVAL)
        {
          ret = mount (NULL, target, "tmpfs", 0, "size=0k");
          ຖ້າ (LIKELY (ret == 0))
            ສົ່ງຄືນ 0;
        }

      ສົ່ງຄືນ crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ
move_root (const var_na *rootfs, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;

  ret = chdir (rootfs);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = umount_or_hide ("/proc", err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_finalize_mounts (sa_nracanaa container_entrypoint_s *entrypoint_args, libcrun_container_t *container, const var_na *rootfs,
                         set_mounts_cb_t after_pivot_cb, void *cb_data, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_root_fd));

  ຖ້າ (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      ຖ້າ (get_private_data (container)->no_pivot)
        {
          ret = move_root (rootfs, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          ret = do_pivot (container, rootfs, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }

      ret = do_mount (container, NULL, -1, "/", NULL,
                      get_private_data (container)->rootfs_propagation,
                      NULL, LABEL_MOUNT, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = chdir ("/");
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "chdir to `/`");
    }

  ຖ້າ (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ret = finalize_mounts (container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
maybe_open_tree_namespace (const var_na *rootfs, ຈຳນວນເຕັມ *out_fd, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ rootfs_fd = -1;
  ຈຳນວນເຕັມ tree_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ຖ້າ (UNLIKELY (rootfs_fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", rootfs);

  tree_fd = syscall_open_tree (rootfs_fd, "",
                               OPEN_TREE_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  ຖ້າ (tree_fd < 0)
    {
      ຖ້າ (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        ສົ່ງຄືນ 0;
      ສົ່ງຄືນ crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = tree_fd;
  ສົ່ງຄືນ 0;
}

static sa_nracanaa libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  sa_nracanaa libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  ຖ້າ (mount_fds == NULL)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  ສົ່ງຄືນ mount_fds;
}

static ຈຳນວນເຕັມ
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free var_na *mnt_data = NULL;
  unsigned long mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  const var_na *label_type = NULL;
  const var_na *label_val = NULL;
  const var_na *fstype;
  ຈຳນວນເຕັມ mnt_fd;
  size_t j;

  ຖ້າ (mount->type == NULL)
    ສົ່ງຄືນ -1;

  fstype = mount->type;

  ສຳລັບ (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  ຖ້າ (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      ຈຳນວນເຕັມ extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      ຖ້າ (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          ຖ້າ (label_type == NULL)
            crun_error_release (err);
          ບໍ່ດັ່ງນັ້ນ
            label_val = def->linux->mount_label;
        }
    }

  ຖ້າ (strcmp (fstype, "cgroup") == 0)
    {
      ຈຳນວນເຕັມ cgroup_mode = libcrun_get_cgroup_mode (err);
      ຖ້າ (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  ຖ້າ (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, NULL, NULL, mnt_data);

  ສົ່ງຄືນ mnt_fd;
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
sa_nracanaa mnt_id_req_s
{
  uint32_t size;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

sa_nracanaa statmount_s
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
  var_na str[];
};

static ຈຳນວນເຕັມ
syscall_listmount (sa_nracanaa mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned ຈຳນວນເຕັມ flags)
{
#ifdef __NR_listmount
  ສົ່ງຄືນ (ຈຳນວນເຕັມ) syscall (__NR_listmount, req, ids, nr, flags);
#ບໍ່ດັ່ງນັ້ນ
  (void) req;
  (void) ids;
  (void) nr;
  (void) flags;
  errno = ENOSYS;
  ສົ່ງຄືນ -1;
#endif
}

static ຈຳນວນເຕັມ
syscall_statmount (sa_nracanaa mnt_id_req_s *req, sa_nracanaa statmount_s *buf, size_t size, unsigned ຈຳນວນເຕັມ flags)
{
#ifdef __NR_statmount
  ສົ່ງຄືນ (ຈຳນວນເຕັມ) syscall (__NR_statmount, req, buf, size, flags);
#ບໍ່ດັ່ງນັ້ນ
  (void) req;
  (void) buf;
  (void) size;
  (void) flags;
  errno = ENOSYS;
  ສົ່ງຄືນ -1;
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
static bool
mounts_hit_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = NULL;
  size_t i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  ສຳລັບ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      ຖ້າ (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != NULL && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          bha_nga;
        }
    }
  ຖ້າ (! has_absolute_bind)
    ສົ່ງຄືນ false;

  ids = xmalloc (capacity * parimaa_na (uint64_t));
  ສຳລັບ (;;)
    {
      sa_nracanaa mnt_id_req_s req = {
        .size = parimaa_na (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      ຈຳນວນເຕັມ ret;

      ຖ້າ (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * parimaa_na (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      ຖ້າ (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        ສົ່ງຄືນ true;
      ຖ້າ (ret == 0)
        bha_nga;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  ສຳລັບ (i = 0; i < n_ids; i++)
    {
      var_na buffer[4096] __attribute__ ((aligned (8)));
      sa_nracanaa statmount_s *sm = (sa_nracanaa statmount_s *) buffer;
      sa_nracanaa mnt_id_req_s req = {
        .size = parimaa_na (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      const var_na *root, *point;
      size_t j;

      ຖ້າ (syscall_statmount (&req, sm, parimaa_na (buffer), 0) < 0)
        continue;
      ຖ້າ ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        continue;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      root = sm->str + sm->mnt_root;
      ຖ້າ (! has_prefix (root, "mnt:["))
        continue;

      point = sm->str + sm->mnt_point;

      ສຳລັບ (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          const var_na *source = def->mounts[j]->source;
          size_t source_len;

          ຖ້າ (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            continue;
          ຖ້າ (source == NULL || source[0] != '/')
            continue;

          source_len = strlen (source);
          ໃນຂະນະທີ່ (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          ຖ້າ (strncmp (point, source, source_len) != 0)
            continue;

          /* The source itself is a pinned mount namespace.  */
          ຖ້າ (point[source_len] == '\0')
            ສົ່ງຄືນ true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          ຖ້າ (recursive && (source_len == 1 || point[source_len] == '/'))
            ສົ່ງຄືນ true;
        }
    }

  ສົ່ງຄືນ false;
}

static bool
can_use_open_tree_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  sa_nracanaa libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  ຖ້າ (has_hooks || has_userns)
    ສົ່ງຄືນ false;

  ຖ້າ (mounts_hit_pinned_mount_namespace (def))
    ສົ່ງຄືນ false;

  ຖ້າ (container->context->no_open_tree_namespace)
    ສົ່ງຄືນ false;

  {
    libcrun_error_t tmp_err = NULL;
    ຈຳນວນເຕັມ in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    ຖ້າ (in_userns != 0)
      ສົ່ງຄືນ false;
  }

  mount_fds = get_fd_map (container);
  ສຳລັບ (i = 0; i < def->mounts_len; i++)
    {
      ຖ້າ (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          ຖ້າ (! has_cached_target)
            ສົ່ງຄືນ false;
        }
    }

  ສົ່ງຄືນ true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
static bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ຖ້າ (mount->type == NULL || strcmp (mount->type, "tmpfs") != 0)
    ສົ່ງຄືນ false;

  ສຳລັບ (i = 0; i < mount->options_len; i++)
    ຖ້າ (mount->options[i] != NULL && has_prefix (mount->options[i], "mode="))
      ສົ່ງຄືນ false;

  ສົ່ງຄືນ true;
}

static ຈຳນວນເຕັມ
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, var_na **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close ຈຳນວນເຕັມ rootfsfd = -1;
  cleanup_close ຈຳນວນເຕັມ tree_fd = -1;
  libcrun_error_t tmp_err = NULL;
  bool use_open_tree = false;
  size_t i;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, NULL, NULL, NULL, NULL);

  ຖ້າ ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  free (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  ສຳລັບ (i = 0; i < def->mounts_len; i++)
    {
      sa_nracanaa libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close ຈຳນວນເຕັມ mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      ຖ້າ (mount_fds && mount_fds->fds[i] >= 0)
        continue;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      ຖ້າ (def->mounts[i]->source != NULL
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free var_na *mnt_data = NULL;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          ສຳລັບ (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          ຖ້າ (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free var_na *target = NULL;
              libcrun_error_t tmp_err = NULL;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              ຖ້າ (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              ຖ້າ (get_private_data (container)->copy_symlink_targets == NULL)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * parimaa_na (var_na *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = NULL;
            }
        }

      ຖ້າ (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          ຖ້າ (mount_fds && def->mounts[i]->source != NULL && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == NULL)
            {
              libcrun_error_t tmp_err = NULL;
              unsigned long propagation = MS_PRIVATE;

              ຖ້າ (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              ຖ້າ (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          continue;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      ຖ້າ (mnt_fd < 0)
        {
          crun_error_release (err);
          continue;
        }

      ຖ້າ (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close ຈຳນວນເຕັມ destfd = -1;
          sa_nracanaa stat st;

          ຖ້າ (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              ຖ້າ (UNLIKELY (rootfsfd < 0))
                ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          ຖ້າ (destfd < 0)
            crun_error_release (&tmp_err);
          ບໍ່ດັ່ງນັ້ນ ຖ້າ (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              ຈຳນວນເຕັມ procfd = get_procfd (get_private_data (container), err);
              ຖ້າ (UNLIKELY (procfd < 0))
                ສົ່ງຄືນ procfd;

              get_self_fd_path (fd_path, mnt_fd);
              ret = fchmodat (procfd, fd_path, st.st_mode & 07777, 0);
              (void) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_empty_dir_cached (container, NULL, &tmp_err);
  ຖ້າ (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  ຖ້າ (! get_private_data (container)->joined_mount_ns
      && can_use_open_tree_namespace (container))
    use_open_tree = true;

  ຖ້າ (use_open_tree)
    {
      ret = maybe_open_tree_namespace (*rootfs, &tree_fd, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  ຖ້າ (tree_fd >= 0)
    preopen_needed_devs_and_notify (container);

  ຖ້າ (tree_fd >= 0)
    {
      ret = setns (tree_fd, CLONE_NEWNS);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (NULL, "/", NULL, MS_REMOUNT | MS_BIND, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, NULL, -1, "/", NULL, MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = do_mount (container, NULL, -1, "/", NULL, rootfs_propagation, NULL, LABEL_MOUNT, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_root_switched = true;
      free (*rootfs);
      *rootfs = xstrdup ("/");
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      ຖ້າ (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, NULL, -1, "/", NULL, rootfs_propagation, NULL, LABEL_MOUNT, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = make_parent_mount_private (*rootfs, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, NULL, MS_BIND | MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  ຖ້າ (! container->context->force_no_cgroup)
    {
      ຈຳນວນເຕັມ cgroup_mode = libcrun_get_cgroup_mode (err);
      ຖ້າ (UNLIKELY (cgroup_mode < 0))
        ສົ່ງຄືນ cgroup_mode;

      ຖ້າ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          var_na *unified_cgroup_path = NULL;
          ຈຳນວນເຕັມ procfd = get_procfd (get_private_data (container), err);
          ຖ້າ (UNLIKELY (procfd < 0))
            ສົ່ງຄືນ procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = NULL;
      ສົ່ງຄືນ ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = NULL;

  ຖ້າ (tree_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  ຖ້າ (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_do_pivot_root (libcrun_container_t *container, bool no_pivot, var_na **rootfs, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;

  ຖ້າ (*rootfs == NULL)
    ສົ່ງຄືນ 0;

  ຖ້າ (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      ຖ້າ (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "chdir to `/`");
        }
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      ret = chroot (*rootfs);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "chdir to `/`");

      free (*rootfs);
      *rootfs = xstrdup ("/");
    }

  ສົ່ງຄືນ 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
ຈຳນວນເຕັມ
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  sa_nracanaa stat statbuf;
  cleanup_close ຈຳນວນເຕັມ fd;
  ຈຳນວນເຕັມ ret;
  ຈຳນວນເຕັມ i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  ຖ້າ (UNLIKELY (fd == -1))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສຳລັບ (i = 0; i <= 2; i++)
    {
      ຖ້າ (UNLIKELY (fstat (i, &statbuf) == -1))
        ສົ່ງຄືນ crun_make_error (err, errno, "stat fd `%d`", i);
      ຖ້າ (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          ຖ້າ (UNLIKELY (dup2 (fd, i) == -1))
            ສົ່ງຄືນ crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
uidgidmap_helper (var_na *helper, pid_t pid, const var_na *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  var_na pid_fmt[16];
  var_na *args[MAX_ARGS + 1];
  var_na *next;
  cleanup_free var_na *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  ຈຳນວນເຕັມ ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, parimaa_na (pid_fmt), "%d", pid);
  ຖ້າ (UNLIKELY (ret >= (ຈຳນວນເຕັມ) parimaa_na (pid_fmt)))
    ສົ່ງຄືນ crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  ໃນຂະນະທີ່ (nargs < MAX_ARGS)
    {
      var_na *p = strsep (&next, " \n");
      ຖ້າ (next == NULL)
        bha_nga;
      args[nargs++] = p;
    }
  args[nargs++] = NULL;

  ret = run_process (args, err);
  ຖ້າ (ret < 0)
    ສົ່ງຄືນ ret;
  ຖ້າ (ret > 0)
    ສົ່ງຄືນ crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
newgidmap (pid_t pid, const var_na *map_file, libcrun_error_t *err)
{
  ສົ່ງຄືນ uidgidmap_helper ("newgidmap", pid, map_file, err);
}

static ຈຳນວນເຕັມ
newuidmap (pid_t pid, const var_na *map_file, libcrun_error_t *err)
{
  ສົ່ງຄືນ uidgidmap_helper ("newuidmap", pid, map_file, err);
}

static ຈຳນວນເຕັມ
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  cleanup_close ຈຳນວນເຕັມ fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ fd;

  ret = TEMP_FAILURE_RETRY (write (fd, "deny", 4));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  cleanup_free var_na *content = NULL;

  ຖ້າ (get_private_data (container)->deny_setgroups)
    ສົ່ງຄືນ 0;

  ຖ້າ (container->container_def->annotations)
    {
      const var_na *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      ຖ້າ (annotation)
        ສົ່ງຄືນ strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close ຈຳນວນເຕັມ fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    ຖ້າ (fd < 0)
      ret = fd;
    ບໍ່ດັ່ງນັ້ນ
      ret = read_all_fd (fd, "self/setgroups", &content, NULL, err);
  }
  ຖ້າ (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      ຖ້າ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ສົ່ງຄືນ 1;
        }
      ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

ຈຳນວນເຕັມ
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = NULL;
  size_t additional_gids_len = 0;
  ຈຳນວນເຕັມ can_do_setgroups;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (process != NULL && process->user != NULL)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  ຖ້າ (UNLIKELY (can_do_setgroups < 0))
    ສົ່ງຄືນ can_do_setgroups;

  ຖ້າ (can_do_setgroups == 0)
    {
      const var_na *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      ຖ້າ (annotation && strcmp (annotation, "0") != 0)
        ສົ່ງຄືນ 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      ສຳລັບ (i = 0; i < additional_gids_len; i++)
        ຖ້າ (additional_gids[i] != process->user->gid)
          ສົ່ງຄືນ crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (ຈຳນວນເຕັມ) additional_gids[i]);
      ສົ່ງຄືນ 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "setgroups");

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  ຖ້າ (get_private_data (container)->unshare_cgroupns)
    {
      ຈຳນວນເຕັມ ret = unshare (CLONE_NEWCGROUP);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          ຖ້າ (errno != EINVAL)
            ສົ່ງຄືນ crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  ສົ່ງຄືນ 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
ຈຳນວນເຕັມ
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  ຖ້າ (len != 1)
    ສົ່ງຄືນ 0;

  ຖ້າ (mappings[0]->size != 1)
    ສົ່ງຄືນ 0;

  ຖ້າ (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    ສົ່ງຄືນ 0;

  ສົ່ງຄືນ 1;
}

static ຈຳນວນເຕັມ
write_id_map (libcrun_container_t *container, pid_t pid, const var_na *map_name,
              ຈຳນວນເຕັມ (*idmap_helper) (pid_t, const var_na *, libcrun_error_t *), const var_na *helper_name,
              bool warn_on_failure, const var_na *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret = 0;

  ຖ້າ (container->host_uid)
    ret = idmap_helper (pid, map, err);
  ຖ້າ (container->host_uid == 0 || ret < 0)
    {
      cleanup_close ຈຳນວນເຕັມ fd = -1;

      ຖ້າ (ret < 0)
        {
          ຖ້າ (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      ຖ້າ (UNLIKELY (fd < 0))
        ສົ່ງຄືນ fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      ຖ້າ (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free var_na *single_mapping = NULL;
          crun_error_release (err);

          ຖ້າ (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              ຖ້າ (UNLIKELY (ret < 0))
                ສົ່ງຄືນ ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          ຖ້າ (UNLIKELY (fd < 0))
            ສົ່ງຄືນ fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free var_na *uid_map = NULL;
  cleanup_free var_na *gid_map = NULL;
  size_t uid_map_len = 0, gid_map_len = 0;
  ຈຳນວນເຕັມ ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ຖ້າ ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    ສົ່ງຄືນ 0;

  ຖ້າ (! def || ! def->linux)
    ສົ່ງຄືນ 0;

  ຖ້າ (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      uid_map_len = (size_t) ret;
      ຖ້າ (uid_map == NULL)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
    }

  ຖ້າ (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      gid_map_len = (size_t) ret;
      ຖ້າ (gid_map == NULL)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

sa_nracanaa all_caps_s
{
  unsigned long effective[2];
  unsigned long permitted[2];
  unsigned long inheritable[2];
  unsigned long ambient[2];
  unsigned long bounding[2];
};

static ຈຳນວນເຕັມ
has_cap_on (ຈຳນວນເຕັມ cap, long unsigned *caps)
{
  ຖ້າ (cap < 32)
    ສົ່ງຄືນ CAP_TO_MASK_0 (cap) & caps[0];
  ສົ່ງຄືນ (CAP_TO_MASK_1 (cap) & caps[1]);
}

static unsigned long cap_last_cap;

ຈຳນວນເຕັມ
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  ຈຳນວນເຕັມ ret;
  var_na buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  ຖ້າ (fd < 0)
    ສົ່ງຄືນ fd;
  ret = TEMP_FAILURE_RETRY (read (fd, buffer, parimaa_na (buffer) - 1));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, NULL, 10);
  ຖ້າ (errno != 0)
    ສົ່ງຄືນ crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
set_required_caps (sa_nracanaa all_caps_s *caps, uid_t uid, gid_t gid, ຈຳນວນເຕັມ no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  ຈຳນວນເຕັມ ret;
  sa_nracanaa __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  sa_nracanaa __user_cap_data_struct data[2] = { { 0 } };

  ຖ້າ (cap_last_cap == 0)
    ສົ່ງຄືນ crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  ສຳລັບ (cap = 0; cap <= cap_last_cap; cap++)
    ຖ້າ (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        ຖ້າ (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          ສົ່ງຄືນ crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  ຖ້າ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ສົ່ງຄືນ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  ຖ້າ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    ສົ່ງຄືນ crun_make_error (err, errno, "prctl reset ambient");

  ສຳລັບ (cap = 0; cap <= cap_last_cap; cap++)
    ຖ້າ (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        ຖ້າ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          ສົ່ງຄືນ crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  ຖ້າ (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "prctl reset KEEPCAPS");

      ຖ້າ (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          ສຳລັບ (cap = 0; cap <= cap_last_cap; cap++)
            ຖ້າ (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "capset");
        }
    }
#endif

  ຖ້າ (no_new_privs)
    ຖ້າ (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      ສົ່ງຄືນ crun_make_error (err, errno, "prctl set no new privs");

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
read_caps (unsigned long caps[2], var_na **values, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  ສຳລັບ (i = 0; i < len; i++)
    {
      cap_value_t cap;
      ຖ້າ (cap_from_name (values[i], &cap) < 0)
        ສົ່ງຄືນ crun_make_error (err, 0, "unknown capability `%s`", values[i]);

      ຖ້າ (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      ບໍ່ດັ່ງນັ້ນ
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#ບໍ່ດັ່ງນັ້ນ
  caps[0] = 0;
  caps[1] = 0;
#endif
  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ຖ້າ (proc->selinux_label)
    ສົ່ງຄືນ set_selinux_label (container, proc->selinux_label, now, err);

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ຖ້າ (proc->apparmor_profile)
    ສົ່ງຄືນ set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  ຈຳນວນເຕັມ no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  sa_nracanaa all_caps_s caps = {};
  ຈຳນວນເຕັມ ret;

  ຖ້າ (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ສົ່ງຄືນ set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

sa_nracanaa rlimit_s
{
  const var_na *name;
  ຈຳນວນເຕັມ value;
};

sa_nracanaa rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
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
                              { "RLIMIT_STACK", RLIMIT_STACK },
                              { NULL, 0 } };

static ຈຳນວນເຕັມ
get_rlimit_resource (const var_na *name)
{
  sa_nracanaa rlimit_s *it;
  ສຳລັບ (it = rlimits; it->name; it++)
    ຖ້າ (strcmp (it->name, name) == 0)
      ສົ່ງຄືນ it->value;
  ສົ່ງຄືນ -1;
}

ຈຳນວນເຕັມ
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  ສຳລັບ (i = 0; i < len; i++)
    {
      ສຳລັບ (j = i + 1; j < len; j++)
        {
          ຖ້າ (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            ສົ່ງຄືນ crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  ສຳລັບ (i = 0; i < len; i++)
    {
      sa_nracanaa rlimit limit;
      var_na *type = new_rlimits[i]->type;
      ຈຳນວນເຕັມ resource = get_rlimit_resource (type);
      ຖ້າ (UNLIKELY (resource < 0))
        ສົ່ງຄືນ crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      ຖ້າ (UNLIKELY (setrlimit (resource, &limit) < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ຈຳນວນເຕັມ has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  ຈຳນວນເຕັມ ret;
  ຖ້າ (def->hostname == NULL || def->hostname[0] == '\0')
    ສົ່ງຄືນ 0;
  ຖ້າ (! has_uts)
    ສົ່ງຄືນ crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "sethostname");
  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ຈຳນວນເຕັມ has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  ຈຳນວນເຕັມ ret;
  ຖ້າ (is_empty_string (def->domainname))
    ສົ່ງຄືນ 0;
  ຖ້າ (! has_uts)
    ສົ່ງຄືນ crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "setdomainname");
  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  ຈຳນວນເຕັມ ret;
  var_na oom_buffer[16];
  ຖ້າ (def->process == NULL || ! def->process->oom_score_adj_present)
    ສົ່ງຄືນ 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, parimaa_na (oom_buffer), "%i", def->process->oom_score_adj);
  ຖ້າ (UNLIKELY (ret >= (ຈຳນວນເຕັມ) parimaa_na (oom_buffer)))
    ສົ່ງຄືນ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  ຖ້າ (fd < 0)
    ສົ່ງຄືນ fd;
  ret = TEMP_FAILURE_RETRY (write (fd, oom_buffer, strlen (oom_buffer)));
  ຖ້າ (ret < 0)
    ສົ່ງຄືນ crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  ສົ່ງຄືນ 0;
}

const var_na *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  NULL
};

static ຈຳນວນເຕັມ
validate_sysctl (const var_na *original_key, const var_na *original_value, const var_na *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  const var_na *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  ຖ້າ (name[0] == '/' || path_has_dot_dot_component (name))
    ສົ່ງຄືນ crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_key);

  ຖ້າ (has_prefix (name, "fs/mqueue/"))
    {
      ຖ້າ (namespaces_created & CLONE_NEWIPC)
        ສົ່ງຄືນ 0;

      namespace = "IPC";
      goto fail;
    }

  ຖ້າ (has_prefix (name, "kernel/"))
    {
      size_t i;

      ສຳລັບ (i = 0; sysctlRequiringIPC[i]; i++)
        ຖ້າ (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            ຖ້າ (namespaces_created & CLONE_NEWIPC)
              ສົ່ງຄືນ 0;

            namespace = "IPC";
            goto fail;
          }

      ຖ້າ (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          ຖ້າ (! is_empty_string (def->domainname) && (strcmp (original_value, def->domainname) != 0))
            ສົ່ງຄືນ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_key);

          ຖ້າ (namespaces_created & CLONE_NEWUTS)
            ສົ່ງຄືນ 0;

          namespace = "UTS";
          goto fail;
        }

      ຖ້າ (strcmp (name, "kernel/hostname") == 0)
        ສົ່ງຄືນ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_key);
    }
  ຖ້າ (has_prefix (name, "net/"))
    {
      ຖ້າ (namespaces_created & CLONE_NEWNET)
        ສົ່ງຄືນ 0;

      namespace = "network";
      goto fail;
    }

  ສົ່ງຄືນ crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_key);

fail:
  ສົ່ງຄືນ crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_key, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
static var_na *
sysctl_error_reason (const var_na *name, ຈຳນວນເຕັມ namespaces_created, ຈຳນວນເຕັມ errno_)
{

  ຖ້າ (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    ສົ່ງຄືນ xstrdup ("are all the IDs mapped in the user namespace?");

  ສົ່ງຄືນ NULL;
}

ຈຳນວນເຕັມ
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close ຈຳນວນເຕັມ dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ຖ້າ (def->linux == NULL || def->linux->sysctl == NULL || def->linux->sysctl->len == 0)
    ສົ່ງຄືນ 0;

  ສຳລັບ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ຈຳນວນເຕັມ value;

      value = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ຖ້າ (UNLIKELY (value < 0))
        ສົ່ງຄືນ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= value;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  ຖ້າ (UNLIKELY (dirfd < 0))
    ສົ່ງຄືນ dirfd;

  ສຳລັບ (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free var_na *name = NULL;
      cleanup_close ຈຳນວນເຕັມ fd = -1;
      ຈຳນວນເຕັມ ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      ຖ້າ (def->linux->sysctl->keys[i] == NULL || def->linux->sysctl->values[i] == NULL)
        ສົ່ງຄືນ crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_key_to_proc_path (def->linux->sysctl->keys[i]);

      ret = validate_sysctl (def->linux->sysctl->keys[i], def->linux->sysctl->values[i], name, namespaces_created, def, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ຖ້າ (UNLIKELY (fd < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (write (fd, def->linux->sysctl->values[i], strlen (def->linux->sysctl->values[i])));
      ຖ້າ (UNLIKELY (ret < 0))
        {
          ຈຳນວນເຕັມ saved_errno = errno;
          cleanup_free var_na *reason = NULL;

          reason = sysctl_error_reason (def->linux->sysctl->keys[i], namespaces_created, saved_errno);
          ສົ່ງຄືນ crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
open_terminal (var_na **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  uid_t uid = 0;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  ສົ່ງຄືນ ret;
}

var_na *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  ສົ່ງຄືນ get_private_data (container)->external_descriptors;
}

ຈຳນວນເຕັມ
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  const var_na *buf = NULL;
  json_gen_ctx *gen = NULL;
  size_t buf_len;
  ຈຳນວນເຕັມ ret;
  ຈຳນວນເຕັມ i;

  ຖ້າ (! json_gen_init (&gen, NULL))
    ສົ່ງຄືນ crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  ຖ້າ (UNLIKELY (ret != json_gen_status_ok))
    goto json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  ສຳລັບ (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      var_na link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          ຖ້າ (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          ບໍ່ດັ່ງນັ້ນ
            {
              json_gen_free (gen);
              ສົ່ງຄືນ crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      ຖ້າ (UNLIKELY (ret != json_gen_status_ok))
        goto json_error;
    }

  ret = json_gen_array_close (gen);
  ຖ້າ (UNLIKELY (ret != json_gen_status_ok))
    goto json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  ຖ້າ (UNLIKELY (ret != json_gen_status_ok))
    goto json_error;

  ຖ້າ (buf)
    {
      var_na *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  ສົ່ງຄືນ 0;

json_error:
  ຖ້າ (gen)
    json_gen_free (gen);
  ສົ່ງຄືນ json_gen_error_to_crun_error (ret, err);
}

ຈຳນວນເຕັມ
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free var_na *pty = NULL;
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (def->process == NULL || ! def->process->terminal)
    ສົ່ງຄືນ 0;

  fd = open_terminal (&pty, def->process, err);
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ fd;

  ຖ້າ (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ຖ້າ (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", NULL, MS_BIND, NULL, LABEL_MOUNT, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      ret = unlink ("/dev/console");
      ຖ້າ (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          ຖ້າ (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", NULL, MS_BIND, NULL, LABEL_MOUNT, err);
              ຖ້າ (UNLIKELY (ret < 0))
                ສົ່ງຄືນ ret;

              ສົ່ງຄືນ get_and_reset (&fd);
            }
          ສົ່ງຄືນ crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  ສົ່ງຄືນ get_and_reset (&fd);
}

static bool
read_error_from_sync_socket (ຈຳນວນເຕັມ sync_socket_fd, ຈຳນວນເຕັມ *error, var_na **str)
{
  cleanup_free var_na *b = NULL;
  size_t size;
  ຈຳນວນເຕັມ code = 0;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (read (sync_socket_fd, &code, parimaa_na (code)));
      ຖ້າ (UNLIKELY (ret != (ຈຳນວນເຕັມ) parimaa_na (code)))
        ສົ່ງຄືນ false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (read (sync_socket_fd, &size, parimaa_na (size)));
  ຖ້າ (UNLIKELY (ret != (ຈຳນວນເຕັມ) parimaa_na (size)))
    ສົ່ງຄືນ false;

  ຖ້າ (size == 0)
    ສົ່ງຄືນ false;

  ຖ້າ (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (read (sync_socket_fd, b, size));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ false;

  b[ret] = '\0';

  *str = b;
  b = NULL;
  ສົ່ງຄືນ true;
}

static bool
send_error_to_sync_socket (ຈຳນວນເຕັມ sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  ຈຳນວນເຕັມ ret;
  ຈຳນວນເຕັມ code;
  var_na *msg;

  ຖ້າ (err == NULL || *err == NULL)
    ສົ່ງຄືນ false;

  code = crun_error_get_errno (err);

  ຖ້າ (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, "1", 1));
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ false;
    }

  ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, &code, parimaa_na (code)));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ false;

  msg = (*err)->msg;
  size = strlen (msg) + 1;
  ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, &size, parimaa_na (size)));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ false;

  ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, msg, size));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ false;

  ສົ່ງຄືນ true;
}

static anta_hstha ຈຳນວນເຕັມ
send_success_to_sync_socket (ຈຳນວນເຕັມ sync_socket, libcrun_error_t *err)
{
  const ຈຳນວນເຕັມ success = 0;
  ຈຳນວນເຕັມ ret;

  ret = TEMP_FAILURE_RETRY (write (sync_socket, &success, parimaa_na (success)));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "write to sync socket");

  ສົ່ງຄືນ 0;
}

static __attribute__ ((noreturn)) void
send_error_to_sync_socket_and_die (ຈຳນວນເຕັມ sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  var_na *msg;

  ຖ້າ (err == NULL || *err == NULL)
    _safe_exit (EXIT_FAILURE);

  ຖ້າ (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      msg = (*err)->msg;
      libcrun_fail_with_error (errno, "%s", msg);
    }
  _safe_exit (EXIT_FAILURE);
}

static ຈຳນວນເຕັມ
expect_success_from_sync_socket (ຈຳນວນເຕັມ sync_fd, libcrun_error_t *err)
{
  cleanup_free var_na *err_str = NULL;
  ຈຳນວນເຕັມ res = 1;
  ຈຳນວນເຕັມ ret;

  ret = TEMP_FAILURE_RETRY (read (sync_fd, &res, parimaa_na (res)));
  ຖ້າ (UNLIKELY (ret != parimaa_na (res)))
    ສົ່ງຄືນ crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  ຖ້າ (res == 0)
    ສົ່ງຄືນ 0;

  ຖ້າ (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      ຖ້າ (! is_empty_string (err_str))
        ສົ່ງຄືນ crun_make_error (err, res, "%s", err_str);
    }

  ສົ່ງຄືນ crun_make_error (err, res, "read from sync socket");
}

static ຈຳນວນເຕັມ
join_namespaces (runtime_spec_schema_config_schema *def, ຈຳນວນເຕັມ *namespaces_to_join, ຈຳນວນເຕັມ n_namespaces_to_join,
                 ຈຳນວນເຕັມ *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  ຈຳນວນເຕັມ i;

  ສຳລັບ (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free var_na *cwd = NULL;
      ຈຳນວນເຕັມ orig_index = namespaces_to_join_index[i];
      ຈຳນວນເຕັມ value;

      ຖ້າ (namespaces_to_join[i] < 0)
        continue;

      /* Skip the user namespace.  */
      value = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      ຖ້າ (value == CLONE_NEWUSER)
        continue;

      ຖ້າ (value == CLONE_NEWNS)
        {
          cwd = getcwd (NULL, 0);
          ຖ້າ (UNLIKELY (cwd == NULL))
            ສົ່ງຄືນ crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], value);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          ຖ້າ (ignore_join_errors)
            continue;
          ສົ່ງຄືນ crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      ຖ້າ (value == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  ສົ່ງຄືນ 0;
}

#define MAX_NAMESPACES 10

sa_nracanaa init_status_s
{
  /* fd to the namespace to join.  */
  ຈຳນວນເຕັມ fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  ຈຳນວນເຕັມ index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  ຈຳນວນເຕັມ value[MAX_NAMESPACES];
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
  ຈຳນວນເຕັມ userns_index;
  /* def->linux->namespaces userns.  */
  ຈຳນວນເຕັມ userns_index_origin;

  /* All namespaces created/joined by the container.  */
  ຈຳນວນເຕັມ all_namespaces;

  /* What namespaces are still missing to be created.  */
  ຈຳນວນເຕັມ namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  ຈຳນວນເຕັມ idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  ຈຳນວນເຕັມ idx_timens_to_join_immediately;
};

void
cleanup_free_init_statusp (sa_nracanaa init_status_s *ns)
{
  size_t i;

  ຖ້າ (ns == NULL)
    ສົ່ງຄືນ;

  ສຳລັບ (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

static ຈຳນວນເຕັມ
configure_init_status (sa_nracanaa init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  ສຳລັບ (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  ສຳລັບ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ຈຳນວນເຕັມ value = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ຖ້າ (UNLIKELY (value < 0))
        ສົ່ງຄືນ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ຖ້າ (ns->all_namespaces & value)
        ສົ່ງຄືນ crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= value;

      ຖ້າ (def->linux->namespaces[i]->path == NULL)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= value;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          ຈຳນວນເຕັມ fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          ຖ້າ (ns->fd_len >= MAX_NAMESPACES)
            ສົ່ງຄືນ crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          ຖ້າ (UNLIKELY (fd < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          ຖ້າ (value == CLONE_NEWUSER)
            {
              ns->userns_index = ns->fd_len;
              ns->userns_index_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->index[ns->fd_len] = i;
          ns->value[ns->fd_len] = value;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  ຖ້າ (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  ສົ່ງຄືນ 0;
}

/* Detect if root is available in the container.  */
static bool
root_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  ສຳລັບ (i = 0; i < len; i++)
    ຖ້າ (mappings[i]->container_id == 0)
      ສົ່ງຄືນ true;

  ສົ່ງຄືນ false;
}

static sa_nracanaa libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  sa_nracanaa libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  ຖ້າ (dev_fds == NULL)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  ສົ່ງຄືນ dev_fds;
}

#define NUM_NEEDED_DEVS (parimaa_na (needed_devs) / parimaa_na (needed_devs[0]) - 1)

static sa_nracanaa libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  sa_nracanaa libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  ຖ້າ (fds == NULL)
    {
      fds = make_libcrun_fd_map (NUM_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  ສົ່ງຄືນ fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  ຖ້າ (recursive)
    *recursive = false;

  ຖ້າ (src_nofollow != NULL)
    *src_nofollow = false;

  ສຳລັບ (i = 0; i < mnt->options_len; i++)
    {
      ຖ້າ (mnt->options[i] == NULL)
        continue;

      ຖ້າ (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ຖ້າ (src_nofollow == NULL || *src_nofollow)
            bha_nga;
        }
      ຖ້າ (strcmp (mnt->options[i], "rbind") == 0)
        {
          ຖ້າ (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ຖ້າ (src_nofollow == NULL || *src_nofollow)
            bha_nga;
        }
      ຖ້າ (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  ສົ່ງຄືນ ret;
}

static var_na *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  ສຳລັບ (i = 0; i < mnt->options_len; i++)
    {
      ຖ້າ (mnt->options[i] == NULL)
        continue;

      ຖ້າ (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          ສົ່ງຄືນ mnt->options[i];
        }
      ຖ້າ (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          ສົ່ງຄືນ mnt->options[i];
        }
    }
  ສົ່ງຄືນ NULL;
}

static ຈຳນວນເຕັມ
open_mount_type (const var_na *type, ຈຳນວນເຕັມ *out_fd, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ fsopen_fd = -1;
  cleanup_close ຈຳນວນເຕັມ newfs_fd = -1;
  ຈຳນວນເຕັມ ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ຖ້າ (UNLIKELY (fsopen_fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  ຖ້າ (UNLIKELY (newfs_fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, ຈຳນວນເຕັມ *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  sa_nracanaa mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  const var_na *idmap_option;
  bool recursive = false;
  const var_na *options = NULL;
  bool has_mappings;
  ຈຳນວນເຕັມ ret;
  var_na *extra_msg = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != NULL);
  ຖ້າ (has_mappings_out)
    *has_mappings_out = has_mappings;
  ຖ້າ (! has_mappings)
    ສົ່ງຄືນ 0;

  ຖ້າ ((mnt->uid_mappings == NULL) != (mnt->gid_mappings == NULL))
    ສົ່ງຄືນ crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  ຖ້າ (idmap_option)
    {
      options = strchr (idmap_option, '=');
      ຖ້າ (options)
        {
          /* Skip the '=' itself.  */
          options++;
          ຖ້າ (options[0] == '\0')
            options = NULL;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;
  ຖ້າ (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ fd;

  ຖ້າ (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_tree (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      ຖ້າ (UNLIKELY (newfs_fd < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      ຖ້າ (errno == EINVAL)
        {
          extra_msg = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      ສົ່ງຄືນ crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_msg);
    }

  *out_fd = get_and_reset (&newfs_fd);
  ສົ່ງຄືນ 0;
}

static uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  ຖ້າ (! def || ! def->linux)
    goto exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  ຖ້າ (! mappings)
    goto exit;

  ສຳລັບ (i = 0; i < len; i++)
    {
      ຖ້າ (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        ສົ່ງຄືນ id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  ສົ່ງຄືນ is_uid ? get_overflow_uid () : get_overflow_gid ();
}

static ຈຳນວນເຕັມ
precreate_device (libcrun_container_t *container, ຈຳນວນເຕັມ devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  var_na name[64];
  mode_t type;
  dev_t dev;
  ຈຳນວນເຕັມ ret;

  ret = snprintf (name, parimaa_na (name), "%zu", i);
  ຖ້າ (UNLIKELY (ret >= (ຈຳນວນເຕັມ) parimaa_na (name)))
    ສົ່ງຄືນ crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "chown `%s`", device->path);

  ສົ່ງຄືນ get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

static ຈຳນວນເຕັມ
send_mounts (ຈຳນວນເຕັມ sync_socket_host, sa_nracanaa libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  ຈຳນວນເຕັມ ret;

  ret = TEMP_FAILURE_RETRY (write (sync_socket_host, &how_many, parimaa_na (how_many)));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "write to sync socket");

  ສຳລັບ (i = 0; i < total; i++)
    {
      ຖ້າ (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (var_na *) &i, parimaa_na (i), err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
    }
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, ຈຳນວນເຕັມ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map sa_nracanaa libcrun_fd_map *mount_fds = NULL;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free var_na *rootfs = NULL;
  size_t how_many = 0;
  size_t i;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (def->mounts_len == 0)
    ສົ່ງຄືນ 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  ຖ້າ (def->root && def->root->path)
    rootfs = realpath (def->root->path, NULL);

  ຖ້າ (! has_userns)
    {
      ຈຳນວນເຕັມ is_in_userns = check_running_in_user_namespace (err);
      ຖ້າ (UNLIKELY (is_in_userns < 0))
        ສົ່ງຄືນ is_in_userns;

      has_userns = is_in_userns > 0;
    }

  ສຳລັບ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      ຈຳນວນເຕັມ mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      ຖ້າ (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == NULL)
        {
          unsigned long propagation = 0;

          ຖ້າ (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, NULL, NULL, NULL, NULL) & ALL_PROPAGATIONS_NO_REC;
          ຖ້າ (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          ຖ້າ (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      ຖ້າ (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  ສົ່ງຄືນ send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

static ຈຳນວນເຕັມ
prepare_and_send_dev_mounts (libcrun_container_t *container, ຈຳນວນເຕັມ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map sa_nracanaa libcrun_fd_map *dev_fds = NULL;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close ຈຳນວນເຕັມ current_mountns = -1;
  cleanup_free var_na *state_dir = NULL;
  cleanup_free var_na *devs_path = NULL;
  cleanup_close ຈຳນວນເຕັມ devs_mountfd = -1;
  cleanup_close ຈຳນວນເຕັມ targetfd = -1;
  const var_na *context_type = NULL;
  const var_na *label = NULL;
  size_t how_many = 0;
  size_t i;
  ຈຳນວນເຕັມ ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  ຖ້າ (def->linux == NULL || def->linux->devices_len == 0)
    ສົ່ງຄືນ 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  ຖ້າ (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    ສົ່ງຄືນ send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_root : NULL),
                                     (container->context ? container->context->id : NULL), err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = mkdir (devs_path, 0700);
  ຖ້າ (UNLIKELY (ret < 0 && errno != EEXIST))
    ສົ່ງຄືນ crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  ຖ້າ (UNLIKELY (current_mountns < 0))
    ສົ່ງຄືນ current_mountns;

  ret = unshare (CLONE_NEWNS);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      goto restore_mountns;
    }

  ຖ້າ (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      ຖ້າ (UNLIKELY (context_type == NULL))
        {
          ret = -1;
          goto restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", NULL, context_type, label, NULL);
  ຖ້າ (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      goto restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ຖ້າ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      goto restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      goto restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ຖ້າ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      goto restore_mountns;
    }

  ສຳລັບ (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          continue;
        }

      dev_fds->fds[i] = ret;

      ຖ້າ (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  ຖ້າ (ns_changed && current_mountns >= 0)
    {
      ຈຳນວນເຕັມ setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      ຖ້າ (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          ສົ່ງຄືນ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, ຈຳນວນເຕັມ sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map sa_nracanaa libcrun_fd_map *fds = NULL;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (NUM_NEEDED_DEVS);

  ຖ້າ (! has_userns || geteuid () > 0)
    ສົ່ງຄືນ send_mounts (sync_socket_host, fds, 0, NUM_NEEDED_DEVS, err);

  ສຳລັບ (i = 0; i < NUM_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      ຈຳນວນເຕັມ fd = syscall_open_tree (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      ຖ້າ (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  ສົ່ງຄືນ send_mounts (sync_socket_host, fds, how_many, NUM_NEEDED_DEVS, err);
}

static ຈຳນວນເຕັມ
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, ຈຳນວນເຕັມ sync_socket_host, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
receive_mounts (sa_nracanaa libcrun_fd_map *fds, ຈຳນວນເຕັມ sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (fds->nfds == 0)
    ສົ່ງຄືນ 0;

  ret = TEMP_FAILURE_RETRY (read (sync_socket_container, &how_many, parimaa_na (how_many)));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "read from sync socket");

  ສຳລັບ (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (var_na *) &index, parimaa_na (index), err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
      ຖ້າ (index >= fds->nfds)
        ສົ່ງຄືນ crun_make_error (err, 0, "invalid mount data received");

      ຖ້າ (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  ສົ່ງຄືນ 0;
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
static ຈຳນວນເຕັມ
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;

  ຖ້າ (! container->host_has_cap_sys_ptrace)
    ສົ່ງຄືນ 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool root_mapped = false;

      ຖ້າ (def->linux->uid_mappings_len != 0)
        {
          root_mapped = root_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          ຖ້າ (! root_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      ຖ້າ (def->linux->gid_mappings_len != 0)
        {
          root_mapped = root_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          ຖ້າ (! root_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  ຖ້າ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ສົ່ງຄືນ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "setresgid to `%d`", gid);

  ສົ່ງຄືນ 0;
}

static bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  ສົ່ງຄືນ mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, NULL, NULL);
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
static ຈຳນວນເຕັມ
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  sa_nracanaa libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ຈຳນວນເຕັມ socket_parent = -1;
  cleanup_close ຈຳນວນເຕັມ socket_child = -1;
  cleanup_close ຈຳນວນເຕັມ root_fd = -1;
  cleanup_close ຈຳນວນເຕັມ cwd_fd = -1;
  size_t i, missing = 0;
  ຈຳນວນເຕັມ saved_errno;
  ຈຳນວນເຕັມ sockets[2];
  pid_t pid;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (def->root == NULL || def->root->path == NULL)
    ສົ່ງຄືນ 0;

  ສຳລັບ (i = 0; i < def->mounts_len; i++)
    ຖ້າ (is_proc_mount (def->mounts[i]))
      missing++;
  ຖ້າ (missing == 0)
    ສົ່ງຄືນ 1;

  root_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ຖ້າ (UNLIKELY (root_fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ຖ້າ (UNLIKELY (cwd_fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->root->path);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ 0;

  pid = fork ();
  saved_errno = errno;
  ຖ້າ (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      ຖ້າ (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (void) unshare (CLONE_NEWNS);

      ສຳລັບ (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close ຈຳນວນເຕັມ fd = -1;

          ຖ້າ (! is_proc_mount (def->mounts[i]))
            continue;

          fd = open_mount_of_type (container, def->mounts[i], err);
          ຖ້າ (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (var_na *) &i, parimaa_na (i), err);
          ຖ້າ (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ຖ້າ (UNLIKELY (fchdir (root_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "restore the root directory");

  ຖ້າ (UNLIKELY (pid < 0))
    ສົ່ງຄືນ crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  ໃນຂະນະທີ່ (missing > 0)
    {
      libcrun_error_t tmp_err = NULL;

      ret = receive_fd_from_socket_with_payload (socket_parent, (var_na *) &i, parimaa_na (i), &tmp_err);
      ຖ້າ (ret < 0)
        {
          crun_error_release (&tmp_err);
          bha_nga;
        }
      ຖ້າ (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          bha_nga;
        }

      ຖ້າ (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, NULL, 0);

  ສົ່ງຄືນ missing == 0 ? 1 : 0;
}

static ຈຳນວນເຕັມ
init_container (libcrun_container_t *container, ຈຳນວນເຕັມ sync_socket_container, sa_nracanaa init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  sa_nracanaa libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ຈຳນວນເຕັມ timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  ຈຳນວນເຕັມ ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      ຖ້າ (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      ຖ້າ (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      ຖ້າ (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          delayed_pidns_fork = ret > 0;
        }

      ຖ້າ (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          ret = TEMP_FAILURE_RETRY (write (sync_socket_container, &new_pid, parimaa_na (new_pid)));
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "write to sync socket");
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          new_pid = fork ();
          ຖ້າ (UNLIKELY (new_pid < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "fork");
        }

      ຖ້າ (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          ຖ້າ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (write (sync_socket_container, &new_pid, parimaa_na (new_pid)));
          ຖ້າ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ret = libcrun_set_oom (container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  ຖ້າ ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      ສຳລັບ (i = 0; i < def->mounts_len; i++)
        {
          ຈຳນວນເຕັມ fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          ຖ້າ (def->mounts[i]->type == NULL)
            continue;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          ຖ້າ (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, NULL, NULL, NULL);
          ຖ້າ (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, NULL, NULL, NULL);

          ຖ້າ (fd >= 0)
            {
              ຖ້າ (mount_fds->fds[i] >= 0)
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
  ຖ້າ (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      ຖ້າ (UNLIKELY (timens_offsets_fd < 0))
        ສົ່ງຄືນ timens_offsets_fd;
    }

  ຖ້າ (init_status->all_namespaces & CLONE_NEWUSER)
    {
      ຖ້າ (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }

      ຖ້າ (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "unshare");
    }

  ຖ້າ (def->linux->time_offsets)
    {
      var_na fmt_buffer[128];
      ຈຳນວນເຕັມ fd = timens_offsets_fd;

      ຖ້າ (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, parimaa_na (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ຖ້າ (UNLIKELY (ret >= (ຈຳນວນເຕັມ) parimaa_na (fmt_buffer)))
            ສົ່ງຄືນ crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long ຈຳນວນເຕັມ) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = write (fd, fmt_buffer, strlen (fmt_buffer));
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      ຖ້າ (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long ຈຳນວນເຕັມ) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, parimaa_na (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          ຖ້າ (UNLIKELY (ret >= (ຈຳນວນເຕັມ) parimaa_na (fmt_buffer)))
            ສົ່ງຄືນ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = write (fd, fmt_buffer, strlen (fmt_buffer));
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  ຖ້າ (init_status->must_fork)
    {
      ຖ້າ (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          delayed_pidns_fork = ret > 0;
        }

      ຖ້າ (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          ຖ້າ (UNLIKELY (pid_container < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      ຖ້າ (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          ret = TEMP_FAILURE_RETRY (write (sync_socket_container, &pid_container, parimaa_na (pid_container)));
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "write to sync socket");

          ຖ້າ (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  ສົ່ງຄືນ get_private_data (container)->delayed_pidns_fork;
}

ຈຳນວນເຕັມ
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ client_fd = -1;
  cleanup_close ຈຳນວນເຕັມ pidfd = -1;
  const var_na *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  ຖ້າ (v == NULL)
    ສົ່ງຄືນ 0;

  pidfd = syscall_pidfd_open (pid, 0);
  ຖ້າ (UNLIKELY (pidfd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  ຖ້າ (UNLIKELY (client_fd < 0))
    ສົ່ງຄືນ client_fd;

  ສົ່ງຄືນ send_fd_to_socket (client_fd, pidfd, err);
}

static bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  ຖ້າ (process == NULL || process->exec_cpu_affinity == NULL)
    ສົ່ງຄືນ false;
  ສົ່ງຄືນ (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, void *args,
                             ຈຳນວນເຕັມ *sync_socket_out, sa_nracanaa libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) sa_nracanaa init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ຈຳນວນເຕັມ sync_socket_container = -1;
  var_na *notify_socket_env = NULL;
  cleanup_close ຈຳນວນເຕັມ sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close ຈຳນວນເຕັມ restore_pidns = -1;
  ຈຳນວນເຕັມ first_clone_args = 0;
  ຈຳນວນເຕັມ sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  ຈຳນວນເຕັມ ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  ຖ້າ (def->root)
    {
      ret = do_notify_socket (container, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  ຖ້າ (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ret = libcrun_set_oom (container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->root ? container->container_def->root->path : NULL, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  ສຳລັບ (i = 0; i < init_status.fd_len; i++)
    {
      switch (init_status.value[i])
        {
        pak_sa CLONE_NEWIPC:
          ຖ້າ (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          bha_nga;

        pak_sa CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          ຖ້າ (! new_userns)
            init_status.must_fork = true;
          ບໍ່ດັ່ງນັ້ນ
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          bha_nga;

        pak_sa CLONE_NEWTIME:
          ຖ້າ (! new_userns)
            init_status.must_fork = true;
          ບໍ່ດັ່ງນັ້ນ
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          bha_nga;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  ຖ້າ ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  ຖ້າ (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  ຖ້າ (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      sa_nracanaa _clone3_args clone3_args;
      memset (&clone3_args, 0, parimaa_na (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      ຖ້າ (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  ຖ້າ (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, NULL);
      ຖ້າ (UNLIKELY (pid < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "clone");
    }

  ຖ້າ (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = close_and_reset (&sync_socket_container);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_tree_fd));

      ຖ້າ (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          ret = TEMP_FAILURE_RETRY (read (sync_socket_host, &new_pid, parimaa_na (new_pid)));
          ຖ້າ (UNLIKELY (ret != parimaa_na (new_pid)))
            ສົ່ງຄືນ crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          ຖ້າ (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          ບໍ່ດັ່ງນັ້ນ
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, NULL, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }

      ຖ້າ (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }

      ຖ້າ ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }

      ຖ້າ (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          ret = TEMP_FAILURE_RETRY (read (sync_socket_host, &grandchild, parimaa_na (grandchild)));
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;

          ຖ້າ (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          ບໍ່ດັ່ງນັ້ນ
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, NULL, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      ຖ້າ (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }

      pid_to_clean = 0;
      ສົ່ງຄືນ pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  ຖ້າ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  ຖ້າ (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  ບໍ່ດັ່ງນັ້ນ
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      ຖ້າ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->msg);
    }

  /* Jump into the specified entrypoint.  */
  ຖ້າ (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  ຖ້າ (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->msg);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  ຖ້າ (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

static ຈຳນວນເຕັມ
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, ຈຳນວນເຕັມ sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, const var_na *sub_cgroup,
                            ຈຳນວນເຕັມ *terminal_fd, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  var_na res;
  pid_t pid;
  cleanup_close ຈຳນວນເຕັມ sync_fd = sync_socket_fd;

  ຖ້າ (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (read (sync_fd, &res, 1));
  ຖ້າ (UNLIKELY (ret != 1))
    ສົ່ງຄືນ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  ຖ້າ (res != '0')
    ສົ່ງຄືນ crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (read (sync_fd, &pid, parimaa_na (pid)));
  ຖ້າ (UNLIKELY (ret != parimaa_na (pid)))
    ສົ່ງຄືນ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, NULL, 0);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "waitpid for exec child pid");

  ຖ້າ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ຖ້າ (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ຖ້າ (need_move_to_cgroup)
    {
      cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free var_na *final_cgroup = NULL;

      ຖ້າ (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, NULL);
          ຖ້າ (UNLIKELY (ret < 0))
            ສົ່ງຄືນ ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = libcrun_set_io_priority (pid, process, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (write (sync_fd, &ret, parimaa_na (ret)));
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "write to sync socket");

  ຖ້າ (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          ຈຳນວນເຕັມ err_code = 0;
          cleanup_free var_na *err_str = NULL;

          ຖ້າ (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              ຖ້າ (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  ສົ່ງຄືນ crun_make_error (err, err_code, "%s", err_str);
                }
            }

          ສົ່ງຄືນ crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  ສົ່ງຄືນ pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
static ຈຳນວນເຕັມ
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ຈຳນວນເຕັມ pidfd_pid_to_join = -1;
  ຈຳນວນເຕັມ all_flags = 0;
  size_t i;
  ຈຳນວນເຕັມ ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  ຖ້າ (def->linux && def->linux->namespaces)
    {
      ສຳລັບ (i = 0; i < def->linux->namespaces_len; i++)
        ຖ້າ (! is_empty_string (def->linux->namespaces[i]->path))
          ສົ່ງຄືນ 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  ຖ້າ (UNLIKELY (pidfd_pid_to_join < 0))
    ສົ່ງຄືນ 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;
  ຖ້າ (ret == 0)
    ສົ່ງຄືນ crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  ສຳລັບ (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].value;

  ret = setns (pidfd_pid_to_join, all_flags);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ 0;

  ສົ່ງຄືນ 1;
}

static ຈຳນວນເຕັມ
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ຈຳນວນເຕັມ fds_joined[MAX_NAMESPACES] = {
    0,
  };
  ຈຳນວນເຕັມ fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  ຈຳນວນເຕັມ ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;
  /* Nothing left to do if the namespaces were joined.  */
  ຖ້າ (LIKELY (ret > 0))
    ສົ່ງຄືນ 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  ຖ້າ (def->linux->namespaces_len >= MAX_NAMESPACES)
    ສົ່ງຄືນ crun_make_error (err, 0, "invalid configuration");

  ສຳລັບ (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free var_na *ns_path = NULL;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      ຖ້າ (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          ຖ້າ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              continue;
            }

          ret = fds[i];
          goto exit;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  ຖ້າ (UNLIKELY (ret < 0))
    goto exit;
  ຖ້າ (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      goto exit;
    }

  ສຳລັບ (i = 0; namespaces[i].ns_file; i++)
    {
      ຖ້າ (namespaces[i].value == CLONE_NEWUSER)
        continue;

      ret = setns (fds[i], 0);
      ຖ້າ (ret == 0)
        fds_joined[i] = 1;
    }
  ສຳລັບ (i = 0; namespaces[i].ns_file; i++)
    {
      ຖ້າ (fds_joined[i])
        continue;
      ret = setns (fds[i], 0);
      ຖ້າ (ret == 0)
        fds_joined[i] = 1;
    }
  ສຳລັບ (i = 0; namespaces[i].ns_file; i++)
    {
      ຖ້າ (fds_joined[i])
        continue;
      ret = setns (fds[i], 0);
      ຖ້າ (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          ສຳລັບ (j = 0; j < def->linux->namespaces_len; j++)
            {
              ຖ້າ (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  bha_nga;
                }
            }
          ຖ້າ (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              continue;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          goto exit;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

exit:
  ສຳລັບ (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      const var_na *sub_cgroup,
                      ຈຳນວນເຕັມ detach,
                      runtime_spec_schema_config_schema_process *process,
                      ຈຳນວນເຕັມ *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  ຈຳນວນເຕັມ ret;
  ຈຳນວນເຕັມ sync_socket_fd[2];
  cleanup_close ຈຳນວນເຕັມ cgroup_dirfd = -1;
  cleanup_close ຈຳນວນເຕັມ sync_fd = -1;
  sa_nracanaa _clone3_args clone3_args;
  bool need_move_to_cgroup;

  ຖ້າ (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "set child subreaper");
    }

  ຖ້າ (path_has_dot_dot_component (sub_cgroup))
    ສົ່ງຄືນ crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  ຖ້າ (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free var_na *final_cgroup = NULL;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    ຖ້າ (get_force_cgroup_v1_annotation (container) == NULL)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        ຖ້າ (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, parimaa_na (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  ຖ້າ (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  ບໍ່ດັ່ງນັ້ນ
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  ຖ້າ (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  /* On errors, fall back to fork().  */
  ຖ້າ (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      ຖ້າ (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          goto exit;
        }
    }

  ຖ້າ (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      ສົ່ງຄືນ join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (write (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->msg);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ຖ້າ (UNLIKELY (pid < 0))
    {
      ຈຳນວນເຕັມ saved_errno = errno;
      TEMP_FAILURE_RETRY (write (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  ຖ້າ (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (write (sync_fd, "0", 1));
      ຖ້າ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (write (sync_fd, &pid, parimaa_na (pid)));
      ຖ້າ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free var_na *pty = NULL;
      ຈຳນວນເຕັມ r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (read (sync_fd, &r, parimaa_na (r)));
      ຖ້າ (UNLIKELY (ret != parimaa_na (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      ຖ້າ (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      ຖ້າ (terminal_fd)
        {
          cleanup_close ຈຳນວນເຕັມ ptmx_fd = -1;

          ret = set_id_init (container, err);
          ຖ້າ (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          ຖ້າ (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          ຖ້າ (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  ສົ່ງຄືນ 0;

exit:
  ຖ້າ (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  ຖ້າ (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ
libcrun_linux_container_update (libcrun_container_status_t *status, const var_na *state_root, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

  cgroup_status = libcrun_cgroup_make_status (status);

  ສົ່ງຄືນ libcrun_update_cgroup_resources (cgroup_status, state_root, resources, err);
}

static ຈຳນວນເຕັມ
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, const bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

  cgroup_status = libcrun_cgroup_make_status (status);

  ສົ່ງຄືນ libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

ຈຳນວນເຕັມ
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ສົ່ງຄືນ libcrun_container_pause_unpause_linux (status, true, err);
}

ຈຳນວນເຕັມ
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ສົ່ງຄືນ libcrun_container_pause_unpause_linux (status, false, err);
}

ຈຳນວນເຕັມ
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  ບໍ່ດັ່ງນັ້ນ
    ສົ່ງຄືນ crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, 0, "personality to `%s`", p->domain);

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ຈຳນວນເຕັມ sockfd = -1;

  ສຳລັບ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ຈຳນວນເຕັມ value = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ຖ້າ (UNLIKELY (value < 0))
        ສົ່ງຄືນ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ຖ້າ (value == CLONE_NEWNET && def->linux->namespaces[i]->path == NULL)
        {
          configure_network = true;
          bha_nga;
        }
    }

  ຖ້າ (! configure_network)
    ສົ່ງຄືນ 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  ຖ້າ (LIKELY (sockfd >= 0))
    {
      sa_nracanaa ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      sa_nracanaa nlmsghdr *hdr_recv;
      var_na buf[parimaa_na (sa_nracanaa nlmsghdr) + parimaa_na (sa_nracanaa nlmsgerr)];
      sa_nracanaa sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      sa_nracanaa sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      sa_nracanaa nlmsghdr hdr = {
        .nlmsg_len = parimaa_na (sa_nracanaa nlmsghdr) + parimaa_na (sa_nracanaa ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      sa_nracanaa ifinfomsg msg = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((sa_nracanaa nlmsghdr *) buf) = hdr;
      *((sa_nracanaa ifinfomsg *) (buf + parimaa_na (sa_nracanaa nlmsghdr))) = msg;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      ຖ້າ (UNLIKELY (sockfd < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (sa_nracanaa sockaddr *) &addr, parimaa_na (addr));
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, parimaa_na (buf), 0, (sa_nracanaa sockaddr *) &addr_to, parimaa_na (addr_to));
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, parimaa_na (buf), 0, NULL, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (sa_nracanaa nlmsghdr *) buf;

      ຖ້າ (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          ຖ້າ (err_data->error < 0)
            ສົ່ງຄືນ crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  ສົ່ງຄືນ 0;
}

/* Protection for attacks like CVE-2019-5736.  */
ຈຳນວນເຕັມ ensure_cloned_binary ();
__attribute__ ((constructor)) static void
libcrun_rexec (void)
{
  ຖ້າ (ensure_cloned_binary () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

ຈຳນວນເຕັມ
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ສົ່ງຄືນ libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

ຈຳນວນເຕັມ
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
static ຈຳນວນເຕັມ
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, ຈຳນວນເຕັມ sa_nkeeta, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  ຖ້າ (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      /* The pid is not valid anymore, return an error.  */
      ຖ້າ (ret == 0)
        {
          errno = ESRCH;
          ສົ່ງຄືນ crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, sa_nkeeta);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "kill container");
  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_kill_linux (libcrun_container_status_t *status, ຈຳນວນເຕັມ sa_nkeeta, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;
  cleanup_close ຈຳນວນເຕັມ pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  ຖ້າ (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      ຖ້າ (errno == ENOSYS)
        ສົ່ງຄືນ libcrun_kill_linux_no_pidfd (status, true, sa_nkeeta, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      ຖ້າ (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        ສົ່ງຄືນ crun_make_error (err, ESRCH, "process not running");
      ສົ່ງຄືນ crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  /* The pid is not valid anymore, return an error.  */
  ຖ້າ (ret == 0)
    {
      errno = ESRCH;
      ສົ່ງຄືນ crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, sa_nkeeta, NULL, 0);
  ຖ້າ (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      ຖ້າ (errno == ENOSYS)
        ສົ່ງຄືນ libcrun_kill_linux_no_pidfd (status, false, sa_nkeeta, err);
      ສົ່ງຄືນ crun_make_error (err, errno, "send signal to pidfd");
    }

  ສົ່ງຄືນ 0;
}

static const var_na *
libcrun_get_intelrdt_name (const var_na *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  ຖ້າ (def == NULL || def->linux == NULL || def->linux->intel_rdt == NULL || def->linux->intel_rdt->clos_id == NULL)
    {
      ຖ້າ (explicit)
        *explicit = false;
      ສົ່ງຄືນ ctr_name;
    }

  ຖ້າ (explicit)
    *explicit = true;
  ສົ່ງຄືນ def->linux->intel_rdt->clos_id;
}

ຈຳນວນເຕັມ
libcrun_apply_intelrdt (const var_na *ctr_name, libcrun_container_t *container, pid_t pid, ຈຳນວນເຕັມ actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = NULL;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  const var_na *name;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (container)
    def = container->container_def;

  ຖ້າ (def == NULL || def->linux == NULL || def->linux->intel_rdt == NULL)
    ສົ່ງຄືນ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  ຖ້າ (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ຖ້າ (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          ຖ້າ (UNLIKELY (ret < 0))
            goto fail;

          has_monitoring = true;
        }
    }

  ຖ້າ (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ຖ້າ (UNLIKELY (ret < 0))
        goto fail;
    }

  ຖ້າ (actions & LIBCRUN_INTELRDT_MOVE)
    {
      const var_na *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : NULL;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      ຖ້າ (UNLIKELY (ret < 0))
        goto fail;
    }

  ສົ່ງຄືນ 0;

fail:
  ຖ້າ (has_monitoring)
    {
      libcrun_error_t tmp_err = NULL;
      ຈຳນວນເຕັມ tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      ຖ້າ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  ຖ້າ (created)
    {
      libcrun_error_t tmp_err = NULL;
      ຈຳນວນເຕັມ tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      ຖ້າ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ
libcrun_destroy_intelrdt (const var_na *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  const var_na *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  ຖ້າ (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      ຈຳນວນເຕັມ ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      ຖ້າ (ret < 0)
        ສົ່ງຄືນ ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  ຖ້າ (explicit)
    ສົ່ງຄືນ 0;

  ສົ່ງຄືນ resctl_destroy (clos_id, err);
}

ຈຳນວນເຕັມ
libcrun_update_intel_rdt (const var_na *ctr_name, libcrun_container_t *container, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = NULL;
  const var_na *name;

  ຖ້າ (container)
    def = container->container_def;

  ຖ້າ (def == NULL || def->linux == NULL || def->linux->intel_rdt == NULL)
    ສົ່ງຄືນ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, NULL);

  ສົ່ງຄືນ resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
ຈຳນວນເຕັມ
libcrun_safe_chdir (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *buffer = NULL;
  ຈຳນວນເຕັມ ret;

  ret = chdir (path);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  ຖ້າ (ret == 0 || buffer[0] != '/')
    {
      ຖ້າ (chdir ("/") < 0)
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
#define UNREACHABLE_LEN ((ຈຳນວນເຕັມ) parimaa_na (UNREACHABLE) - 1)

      ຖ້າ ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      ສົ່ງຄືນ crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  ສົ່ງຄືນ 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
static ຈຳນວນເຕັມ
run_callback_in_namespace (ຈຳນວນເຕັມ mntns_fd, ຈຳນວນເຕັມ (*callback) (void *, libcrun_error_t *), void *arg, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ wait_status = 0;
  pid_t pid;
  ຈຳນວນເຕັມ ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ຖ້າ (UNLIKELY (pid < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "vfork");

  ຖ້າ (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      ຖ້າ (UNLIKELY (ret < 0))
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
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  ສົ່ງຄືນ get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
static ຈຳນວນເຕັມ
check_container_userns (libcrun_container_t *container, ຈຳນວນເຕັມ userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close ຈຳນວນເຕັມ expected_fd = -1;
  cleanup_close ຈຳນວນເຕັມ parent_fd = -1;
  sa_nracanaa stat st, expected_st;
  const var_na *path = NULL;
  size_t i;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (def->linux)
    ສຳລັບ (i = 0; i < def->linux->namespaces_len; i++)
      ຖ້າ (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == NULL;
          bha_nga;
        }

  ຖ້າ (path == NULL)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      ຖ້າ (UNLIKELY (expected_fd < 0))
        ສົ່ງຄືນ expected_fd;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      ຖ້າ (UNLIKELY (expected_fd < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  ຖ້າ (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      ຖ້າ (UNLIKELY (parent_fd < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fstat user namespace");

  ຖ້າ (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    ສົ່ງຄືນ crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  ສົ່ງຄືນ 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
static ຈຳນວນເຕັມ
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, ຈຳນວນເຕັມ *mntns_fd, ຈຳນວນເຕັມ *owner_fd, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ mntns = -1;
  cleanup_close ຈຳນວນເຕັມ owner = -1;
  ຈຳນວນເຕັມ ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  ຖ້າ (UNLIKELY (mntns < 0))
    ສົ່ງຄືນ mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (ret == 0)
    ສົ່ງຄືນ crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  ຖ້າ (UNLIKELY (owner < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, ຈຳນວນເຕັມ (*callback) (void *, libcrun_error_t *), void *arg, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ mntns_fd = -1;
  cleanup_close ຈຳນວນເຕັມ owner_fd = -1;
  ຈຳນວນເຕັມ ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ run_callback_in_namespace (mntns_fd, callback, arg, err);
}

sa_nracanaa umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
static ຈຳນວນເຕັມ
umount_nofollow (const var_na *destination, libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ parentfd = -1;
  cleanup_close ຈຳນວນເຕັມ cwdfd = -1;
  var_na buffer[PATH_MAX];
  const var_na *parent, *name;
  var_na *last_slash;
  size_t len;
  ຈຳນວນເຕັມ saved_errno;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (is_empty_string (destination))
    ສົ່ງຄືນ crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  ຖ້າ (UNLIKELY (len >= parimaa_na (buffer)))
    ສົ່ງຄືນ crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  ໃນຂະນະທີ່ (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  ຖ້າ (last_slash == NULL)
    {
      parent = ".";
      name = buffer;
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  ຖ້າ (UNLIKELY (parentfd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  ຖ້າ (UNLIKELY (cwdfd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  ຖ້າ (UNLIKELY (fchdir (cwdfd) < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fchdir to the previous working directory");

  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, saved_errno, "umount `%s`", destination);

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
do_umount_in_a_container (void *arg, libcrun_error_t *err)
{
  sa_nracanaa umount_in_a_container_args *args = arg;
  size_t i;
  ຈຳນວນເຕັມ ret;

  ສຳລັບ (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ 0;
}

sa_nracanaa mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  sa_nracanaa libcrun_fd_map *fds;
  ຈຳນວນເຕັມ pidfd;
  ຈຳນວນເຕັມ owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
static ຈຳນວນເຕັມ
open_destination_nofollow (const var_na *destination, bool dir, libcrun_error_t *err)
{
  const uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  ຈຳນວນເຕັມ fd;
  ຈຳນວນເຕັມ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  ຖ້າ (LIKELY (fd >= 0))
    ສົ່ງຄືນ fd;

  ຖ້າ (errno != ENOENT)
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", destination);

  ຖ້າ (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  ບໍ່ດັ່ງນັ້ນ
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", destination);

  ສົ່ງຄືນ fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
static ຈຳນວນເຕັມ
create_destinations_in_a_container (void *arg, libcrun_error_t *err)
{
  sa_nracanaa mount_in_a_container_args *args = arg;
  size_t i;
  ຈຳນວນເຕັມ ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  ຖ້າ (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "setresuid to `0`");
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ສົ່ງຄືນ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  ສຳລັບ (i = 0; i < args->len; i++)
    {
      cleanup_close ຈຳນວນເຕັມ dest_fd = -1;
      sa_nracanaa stat st;

      ret = fstat (args->fds->fds[i], &st);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ຖ້າ (UNLIKELY (dest_fd < 0))
        ສົ່ງຄືນ dest_fd;
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
do_mount_in_a_container (void *arg, libcrun_error_t *err)
{
  sa_nracanaa mount_in_a_container_args *args = arg;
  size_t i;
  ຈຳນວນເຕັມ ret;

  ສຳລັບ (i = 0; i < args->len; i++)
    {
      cleanup_close ຈຳນວນເຕັມ dest_fd = -1;
      sa_nracanaa stat st;

      ret = fstat (args->fds->fds[i], &st);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ຖ້າ (UNLIKELY (dest_fd < 0))
        ສົ່ງຄືນ dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  ສົ່ງຄືນ 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
static ຈຳນວນເຕັມ
send_mounts_from_container_namespaces (libcrun_container_t *container, ຈຳນວນເຕັມ pidfd, runtime_spec_schema_defs_mount **mounts,
                                       sa_nracanaa libcrun_fd_map *fds, size_t len, ຈຳນວນເຕັມ flags, ຈຳນວນເຕັມ owner_fd, ຈຳນວນເຕັມ sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  ຈຳນວນເຕັມ ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  ຖ້າ (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ສົ່ງຄືນ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ຖ້າ (UNLIKELY (pid < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "fork");

  ຖ້າ (pid)
    {
      ຈຳນວນເຕັມ wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      ຖ້າ (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, NULL, 0));
          ສົ່ງຄືນ crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ສຳລັບ (i = 0; i < len; i++)
    {
      cleanup_close ຈຳນວນເຕັມ fd = -1;

      ຖ້າ (fds->fds[i] >= 0)
        continue;

      fd = open_mount_of_type (container, mounts[i], err);
      ຖ້າ (UNLIKELY (fd < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
static ຈຳນວນເຕັມ
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, ຈຳນວນເຕັມ *flags, libcrun_error_t *err)
{
  static const sa_nracanaa
  {
    const var_na *name;
    ຈຳນວນເຕັມ flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  ສຳລັບ (i = 0; i < parimaa_na (namespaces) / parimaa_na (namespaces[0]); i++)
    {
      cleanup_close ຈຳນວນເຕັມ self_fd = -1;
      cleanup_close ຈຳນວນເຕັມ target_fd = -1;
      sa_nracanaa stat self_st, target_st;
      cleanup_free var_na *self_path = NULL;
      ຈຳນວນເຕັມ ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      ຖ້າ (UNLIKELY (self_fd < 0))
        ສົ່ງຄືນ self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      ຖ້າ (UNLIKELY (target_fd < 0))
        ສົ່ງຄືນ target_fd;

      ret = fstat (self_fd, &self_st);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ຖ້າ (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  ສົ່ງຄືນ 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
static ຈຳນວນເຕັມ
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, ຈຳນວນເຕັມ owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, sa_nracanaa libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close ຈຳນວນເຕັມ sync_fd = -1;
  cleanup_close ຈຳນວນເຕັມ pidfd = -1;
  ຈຳນວນເຕັມ sync_socket_fd[2];
  bool needed = false;
  ຈຳນວນເຕັມ flags;
  pid_t pid;
  size_t i;
  ຈຳນວນເຕັມ ret;

  ສຳລັບ (i = 0; i < len; i++)
    {
      ຖ້າ (fds->fds[i] >= 0)
        continue;

      ຖ້າ (mounts[i]->type == NULL)
        ສົ່ງຄືນ crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  ຖ້າ (! needed)
    ສົ່ງຄືນ 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  ຖ້າ (UNLIKELY (pidfd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (ret == 0)
    ສົ່ງຄືນ crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  ຖ້າ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      ສົ່ງຄືນ ret;
    }

  ຖ້າ (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      ຖ້າ (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  ສຳລັບ (i = 0; i < len; i++)
    {
      ຖ້າ (fds->fds[i] >= 0)
        continue;

      ret = receive_fd_from_socket (sync_fd, err);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          cleanup_free var_na *err_str = NULL;
          ຈຳນວນເຕັມ err_code = 0;

          ຖ້າ (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          ບໍ່ດັ່ງນັ້ນ
            ret = crun_error_wrap (err, "receive mount fd");
          bha_nga;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, NULL, 0));

  ສົ່ງຄືນ ret;
}

static unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free var_na *data = NULL;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  ຖ້າ (mount->options == NULL)
    ສົ່ງຄືນ get_default_flags (container, mount->destination, &data);

  ສຳລັບ (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  ສົ່ງຄືນ flags;
}

ຈຳນວນເຕັມ
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  sa_nracanaa mount_in_a_container_args args;
  cleanup_close_map sa_nracanaa libcrun_fd_map *fds = NULL;
  cleanup_close ຈຳນວນເຕັມ pidfd = -1;
  cleanup_close ຈຳນວນເຕັມ mntns_fd = -1;
  cleanup_close ຈຳນວນເຕັມ owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  ຈຳນວນເຕັມ ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  fds = make_libcrun_fd_map (len);

  ສຳລັບ (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), NULL, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ຖ້າ (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          ຖ້າ (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          ຖ້າ (UNLIKELY (fds->fds[i] < 0))
            ສົ່ງຄືນ fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສຳລັບ (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  ຖ້າ (UNLIKELY (ret > 0))
    ສົ່ງຄືນ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

ຈຳນວນເຕັມ
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  sa_nracanaa umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  ສົ່ງຄືນ run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

sa_nracanaa restrict_mounts_in_a_container_args
{
  sa_nracanaa libcrun_mount_flags_s *mounts;
  size_t len;
  ຈຳນວນເຕັມ rootfd;
};

static ຈຳນວນເຕັມ
do_restrict_mounts_in_a_container (void *arg, libcrun_error_t *err)
{
  sa_nracanaa restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  ຈຳນວນເຕັມ ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  ຖ້າ (LIKELY (ret == 0))
    ret = chroot (".");
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "chroot to the container root");

  ສຳລັບ (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      const var_na *target = consume_slashes (args->mounts[i].target);
      cleanup_close ຈຳນວນເຕັມ fd = -1;
      sa_nracanaa statfs sfs;
      unsigned long current_flags;

      ຖ້າ (flags == 0)
        continue;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      ຖ້າ (UNLIKELY (fd < 0))
        {
          ຖ້າ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              continue;
            }
          ສົ່ງຄືນ fd;
        }

      ret = fstatfs (fd, &sfs);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      ຖ້າ ((current_flags & flags) == flags)
        continue;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      ຖ້າ (LIKELY (ret == 0))
        continue;

      ຖ້າ (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        ສົ່ງຄືນ ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (NULL, target, NULL, MS_REMOUNT | MS_BIND | flags | current_flags, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ crun_make_error (err, errno, "remount `/%s`", target);
    }

  ສົ່ງຄືນ 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
ຈຳນວນເຕັມ
libcrun_restrict_mounts_in_container (pid_t pid, sa_nracanaa libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  sa_nracanaa restrict_mounts_in_a_container_args args;
  cleanup_close ຈຳນວນເຕັມ mntns_fd = -1;
  cleanup_close ຈຳນວນເຕັມ rootfd = -1;
  var_na path[64];
  ຈຳນວນເຕັມ ret;

  snprintf (path, parimaa_na (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  ຖ້າ (UNLIKELY (mntns_fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, parimaa_na (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  ຖ້າ (UNLIKELY (rootfd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  ຖ້າ (UNLIKELY (ret > 0))
    ສົ່ງຄືນ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ຈຳນວນເຕັມ netns_fd = -1;
  size_t i;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (def == NULL || def->linux == NULL || def->linux->net_devices == NULL)
    ສົ່ງຄືນ 0;

  cleanup_free var_na *ns_path = NULL;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  ຖ້າ (UNLIKELY (netns_fd < 0))
    ສົ່ງຄືນ netns_fd;

  ສຳລັບ (i = 0; i < def->linux->net_devices->len; i++)
    {
      const var_na *new_name = def->linux->net_devices->values[i]->name ?: def->linux->net_devices->keys[i];

      ret = move_network_device (def->linux->net_devices->keys[i], new_name, netns_fd, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ 0;
}
