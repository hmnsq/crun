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
#གལ་ཏེ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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

  གལ་ཏེ (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  གལ་ཏེ (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  གལ་ཏེ (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  གལ་ཏེ (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  གལ་ཏེ (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  གལ་ཏེ (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  གལ་ཏེ (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  གལ་ཏེ (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  གལ་ཏེ (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  ཕྱིར flags;
}

sa_nracanaa remount_s
{
  sa_nracanaa remount_s *next;
  var_na *target;
  ཧྲིལ targetfd;
  unsigned long flags;
  var_na *data;
};

sa_nracanaa private_data_s
{
  sa_nracanaa remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  ཧྲིལ unshare_flags;
  ཧྲིལ unshare_cgroupns;

  var_na *host_notify_socket_path;
  var_na *container_notify_socket_path;
  var_na *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  ཧྲིལ procfd;

  const var_na *rootfs;
  ཧྲིལ rootfsfd;

  ཧྲིལ notify_socket_tree_fd;
  ཧྲིལ old_root_fd;

  sa_nracanaa libcrun_fd_map *mount_fds;
  sa_nracanaa libcrun_fd_map *dev_fds;
  sa_nracanaa libcrun_fd_map *needed_devs_fds;

  var_na **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  var_na *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  ཧྲིལ maskdir_fd;
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
  ཧྲིལ value;
};

static void
cleanup_private_data (void *private_data)
{
  sa_nracanaa private_data_s *p = private_data;

  གལ་ཏེ (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  གལ་ཏེ (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  གལ་ཏེ (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  གལ་ཏེ (p->old_root_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_root_fd));
  གལ་ཏེ (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  གལ་ཏེ (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  གལ་ཏེ (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  གལ་ཏེ (p->copy_symlink_targets)
    {
      size_t i;
      ཆེད་དུ (i = 0; i < p->n_copy_symlink_targets; i++)
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
  གལ་ཏེ (container->private_data == NULL)
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
  ཕྱིར container->private_data;
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

static ཧྲིལ
get_and_reset (ཧྲིལ *old)
{
  ཧྲིལ tmp = *old;
  *old = -1;
  ཕྱིར tmp;
}

ཧྲིལ
libcrun_find_namespace (const var_na *name)
{
  sa_nracanaa linux_namespace_s *it;
  ཆེད་དུ (it = namespaces; it->name; it++)
    གལ་ཏེ (strcmp (it->name, name) == 0)
      ཕྱིར it->value;
  ཕྱིར -1;
}

static ཧྲིལ
syscall_keyctl_join (const var_na *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  ཕྱིར (ཧྲིལ) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

static ཧྲིལ
syscall_pidfd_open (pid_t pid, unsigned ཧྲིལ flags)
{
#གལ་ཏེ defined __NR_pidfd_open
  ཕྱིར (ཧྲིལ) syscall (__NR_pidfd_open, pid, flags);
#དེ་མིན
  (void) pid;
  (void) flags;
  errno = ENOSYS;
  ཕྱིར -1;
#endif
}

static ཧྲིལ
syscall_pidfd_send_signal (ཧྲིལ pidfd, ཧྲིལ sig, siginfo_t *info, unsigned ཧྲིལ flags)
{
#གལ་ཏེ defined __NR_pidfd_send_signal
  ཕྱིར (ཧྲིལ) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#དེ་མིན
  (void) pidfd;
  (void) sig;
  (void) info;
  (void) flags;
  errno = ENOSYS;
  ཕྱིར -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
static uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  གལ་ཏེ (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  གལ་ཏེ (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  གལ་ཏེ (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  གལ་ཏེ (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  གལ་ཏེ (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  གལ་ཏེ (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  གལ་ཏེ (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  གལ་ཏེ (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  ཕྱིར attr;
}

static ཧྲིལ
do_mount_setattr (bool recursive, const var_na *target, ཧྲིལ targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  sa_nracanaa mount_attr_s attr = {
    0,
  };
  ཧྲིལ ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  གལ་ཏེ (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      གལ་ཏེ (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      དེ་མིན གལ་ཏེ (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      དེ་མིན
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      གལ་ཏེ (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  དེ་མིན གལ་ཏེ (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "mount_setattr `/%s`", target);

  ཕྱིར 0;
}

ཧྲིལ
get_bind_mount (ཧྲིལ dirfd, const var_na *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ open_tree_fd = -1;
  sa_nracanaa mount_attr_s attr = {
    0,
  };
  ཧྲིལ recursive_flag = (recursive ? AT_RECURSIVE : 0);
  ཧྲིལ ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  གལ་ཏེ (UNLIKELY (src == NULL))
    ཕྱིར crun_make_error (err, EINVAL, "mount source is not specified");

  གལ་ཏེ (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  གལ་ཏེ (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_tree_fd = syscall_open_tree (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  གལ་ཏེ (UNLIKELY (open_tree_fd < 0))
    ཕྱིར crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_tree_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "mount_setattr `%s`", src);

  ཕྱིར get_and_reset (&open_tree_fd);
}

ཧྲིལ
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, var_na *option, var_na **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free var_na *mappings = NULL;
  const var_na *it;
  ཧྲིལ ret;

  mappings = xmalloc (allocated);

  ཆེད་དུ (it = option; *it;)
    {
      bool relative = false;
      long value[3];
      size_t i;

      གལ་ཏེ (*it == '\0')
        bha_nga;

      གལ་ཏེ (*it == '#')
        it++;

      གལ་ཏེ (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      ཆེད་དུ (i = 0; i < 3; i++)
        {
          var_na *endptr = NULL;

          གལ་ཏེ (i > 0 && *it == '-')
            it++;

          གལ་ཏེ (*it == '\0')
            ཕྱིར crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          value[i] = strtol (it, &endptr, 10);
          གལ་ཏེ (errno || endptr == it)
            ཕྱིར crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      གལ་ཏེ (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          གལ་ཏེ (def == NULL
              || def->linux == NULL
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            ཕྱིར crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          ཆེད་དུ (i = 0; i < mappings_len; i++)
            གལ་ཏེ (value[1] >= (int64_t) mappings[i]->container_id
                && value[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              bha_nga;

          གལ་ཏེ (i == mappings_len)
            ཕྱིར crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          value[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      གལ་ཏེ (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", value[0], value[1], value[2]);
      གལ་ཏེ (UNLIKELY (ret >= (ཧྲིལ) (allocated - written)))
        ཕྱིར crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = NULL;

  ཕྱིར 0;
}

static ཧྲིལ
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
  ཆེད་དུ (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      གལ་ཏེ (UNLIKELY (len >= max_len_mapping))
        ཕྱིར crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = NULL;
  ཕྱིར 0;
}

static ཧྲིལ
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

  ཕྱིར format_mount_mappings (ret, mappings, 1, written, err);
}

static bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  གལ་ཏེ (def->linux == NULL)
    ཕྱིར mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  གལ་ཏེ (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    ཕྱིར false;

  གལ་ཏེ (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    ཕྱིར false;

  ཆེད་དུ (s = 0; s < mnt->uid_mappings_len; s++)
    {
      གལ་ཏེ (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        ཕྱིར false;
      གལ་ཏེ (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        ཕྱིར false;
      གལ་ཏེ (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        ཕྱིར false;
    }

  ཆེད་དུ (s = 0; s < mnt->gid_mappings_len; s++)
    {
      གལ་ཏེ (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        ཕྱིར false;
      གལ་ཏེ (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        ཕྱིར false;
      གལ་ཏེ (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        ཕྱིར false;
    }

  ཕྱིར true;
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

  གལ་ཏེ (! need_new_userns)
    ཕྱིར 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, NULL);
  གལ་ཏེ (UNLIKELY (pid < 0))
    ཕྱིར crun_make_error (err, errno, "clone");

  གལ་ཏེ (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      སྐབས (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  གལ་ཏེ (mnt->uid_mappings_len)
    {
      cleanup_free var_na *uid_map = NULL;
      cleanup_free var_na *gid_map = NULL;
      size_t written = 0;
      ཧྲིལ ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      cleanup_close ཧྲིལ fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      གལ་ཏེ (UNLIKELY (fd < 0))
        ཕྱིར fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      གལ་ཏེ (UNLIKELY (fd < 0))
        ཕྱིར fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
  དེ་མིན
    {
      cleanup_free var_na *dup_options = NULL;
      var_na *option, *saveptr = NULL;

      གལ་ཏེ (! options)
        ཕྱིར crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      ཆེད་དུ (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (NULL, ";", &saveptr))
        {
          cleanup_free var_na *mappings = NULL;
          cleanup_close ཧྲིལ map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          ཧྲིལ ret;

          གལ་ཏེ (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              གལ་ཏེ (UNLIKELY (map_fd < 0))
                ཕྱིར map_fd;
            }
          དེ་མིན གལ་ཏེ (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              གལ་ཏེ (UNLIKELY (map_fd < 0))
                ཕྱིར map_fd;
            }
          དེ་མིན
            ཕྱིར crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  ཕྱིར 0;
}

ཧྲིལ
libcrun_create_keyring (libcrun_container_t *container, const var_na *name, const var_na *label, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ labelfd = -1;
  bool label_set = false;
  ཧྲིལ ret;

  གལ་ཏེ (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      གལ་ཏེ (UNLIKELY (labelfd < 0))
        {
          གལ་ཏེ (crun_error_get_errno (err) != ENOENT)
            ཕྱིར labelfd;

          crun_error_release (err);
        }

      གལ་ཏེ (labelfd >= 0)
        {
          ret = write (labelfd, label, strlen (label));
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      གལ་ཏེ (errno == ENOSYS)
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
  གལ་ཏེ (label_set && write (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  ཕྱིར ret;
}

static void
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  གལ་ཏེ (def->process && def->process->user)
    {
      གལ་ཏེ (def->process->user->uid)
        *uid = def->process->user->uid;
      གལ་ཏེ (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

static unsigned long
get_mount_flags (const var_na *name, ཧྲིལ current_flags, ཧྲིལ *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  const sa_nracanaa propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  གལ་ཏེ (found)
    *found = prop ? 1 : 0;

  གལ་ཏེ (prop == NULL)
    ཕྱིར 0;

  གལ་ཏེ (prop->extra_flags & OPTION_RECURSIVE)
    {
      གལ་ཏེ (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      གལ་ཏེ (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  གལ་ཏེ (extra_flags)
    *extra_flags |= prop->extra_flags;

  གལ་ཏེ (prop->clear)
    ཕྱིར current_flags & ~prop->flags;

  ཕྱིར current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
static bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ཆེད་དུ (i = 0; i < mount->options_len; i++)
    {
      const sa_nracanaa propagation_flags_s *prop;

      གལ་ཏེ (mount->options[i] == NULL)
        continue;

      prop = libcrun_str2mount_flags (mount->options[i]);
      གལ་ཏེ (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        ཕྱིར true;
    }

  ཕྱིར false;
}

static unsigned long
get_mount_flags_or_option (const var_na *name, ཧྲིལ current_flags, unsigned long *extra_flags, var_na **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  ཧྲིལ found;
  __attribute__ ((unused)) cleanup_free var_na *prev = NULL;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  གལ་ཏེ (name == NULL)
    ཕྱིར 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  གལ་ཏེ (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      const var_na *data_flag = NULL;
      གལ་ཏེ ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      དེ་མིན གལ་ཏེ ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      གལ་ཏེ (data_flag)
        {
          prev = *option;
          གལ་ཏེ (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          དེ་མིན
            *option = xstrdup (data_flag);
        }
      ཕྱིར flags;
    }

  prev = *option;
  གལ་ཏེ (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  དེ་མིན
    *option = xstrdup (name);

  ཕྱིར 0;
}

ཧྲིལ
pivot_root (const var_na *new_root, const var_na *put_old)
{
  ཕྱིར syscall (__NR_pivot_root, new_root, put_old);
}

static void
free_remount (sa_nracanaa remount_s *r)
{
  གལ་ཏེ (r == NULL)
    ཕྱིར;
  གལ་ཏེ (r->targetfd >= 0)
    close (r->targetfd);
  free (r->data);
  free (r->target);
  free (r);
}

static sa_nracanaa remount_s *
make_remount (ཧྲིལ targetfd, const var_na *target, unsigned long flags, const var_na *data, sa_nracanaa remount_s *next)
{
  sa_nracanaa remount_s *ret = xmalloc (parimaa_na (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : NULL;
  ret->next = next;
  ret->targetfd = targetfd;
  ཕྱིར ret;
}

static ཧྲིལ
do_remount (ཧྲིལ targetfd, const var_na *target, unsigned long flags, const var_na *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  ཧྲིལ ret;
  const var_na *real_target = target;

  གལ་ཏེ (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      གལ་ཏེ (LIKELY (ret == 0))
        ཕྱིར 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  གལ་ཏེ (flags & (MS_REMOUNT | MS_RDONLY))
    data = NULL;

  ret = mount (NULL, real_target, NULL, flags, data);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      sa_nracanaa statfs sfs;

      ret = statfs (real_target, &sfs);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      གལ་ཏེ (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      གལ་ཏེ ((flags | remount_flags) != flags)
        {
          ret = mount (NULL, real_target, NULL, flags | remount_flags, data);
          གལ་ཏེ (LIKELY (ret == 0))
            ཕྱིར 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          གལ་ཏེ (current_flags & MS_RDONLY)
            ret = mount (NULL, real_target, NULL, flags | remount_flags | MS_RDONLY, data);
        }
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "remount `%s`", target);
    }
  ཕྱིར 0;
}

static ཧྲིལ
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  ཧྲིལ ret = 0;
  sa_nracanaa remount_s *r = get_private_data (container)->remounts;
  སྐབས (r)
    {
      sa_nracanaa remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      གལ་ཏེ (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          གལ་ཏེ (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              continue;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        goto cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  སྐབས (r)
    {
      sa_nracanaa remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = NULL;
  ཕྱིར ret;
}

static ཧྲིལ
open_mount_target (libcrun_container_t *container, const var_na *target_rel, libcrun_error_t *err)
{
  const var_na *rootfs = get_private_data (container)->rootfs;
  ཧྲིལ rootfsfd = get_private_data (container)->rootfsfd;

  གལ་ཏེ (rootfsfd < 0)
    ཕྱིར crun_make_error (err, 0, "invalid rootfs state");

  ཕྱིར safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
static ཧྲིལ
fsopen_mount (const var_na *type, const var_na *source_name, const var_na *labeltype, const var_na *label, const var_na *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close ཧྲིལ fsfd = -1;
  ཧྲིལ ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  གལ་ཏེ (UNLIKELY (fsfd < 0))
    ཕྱིར fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (void) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  གལ་ཏེ (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  གལ་ཏེ (data && data[0])
    {
      cleanup_free var_na *data_copy = xstrdup (data);
      var_na *saveptr = NULL;
      var_na *token;

      ཆེད་དུ (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (NULL, ",", &saveptr))
        {
          var_na *eq = strchr (token, '=');
          གལ་ཏེ (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          དེ་མིན
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, NULL, 0);

          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཕྱིར syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#དེ་མིན
  (void) type;
  (void) source_name;
  (void) data;
  errno = ENOSYS;
  ཕྱིར -1;
#endif
}

static ཧྲིལ
get_procfd (sa_nracanaa private_data_s *data, libcrun_error_t *err)
{
  ཧྲིལ fd;

  གལ་ཏེ (data->procfd >= 0)
    ཕྱིར data->procfd;

  fd = fsopen_mount ("proc", NULL, NULL, NULL, NULL);
  གལ་ཏེ (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (fd < 0))
    ཕྱིར crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  ཕྱིར fd;
}

static ཧྲིལ
get_old_root_fd (sa_nracanaa private_data_s *data)
{
  གལ་ཏེ (data->old_root_fd >= 0)
    ཕྱིར data->old_root_fd;

  data->old_root_fd = open ("/", O_PATH | O_CLOEXEC);
  ཕྱིར data->old_root_fd;
}

static ཧྲིལ
fs_move_mount_to (ཧྲིལ fd, ཧྲིལ dirfd, const var_na *name)
{
#ifdef HAVE_NEW_MOUNT_API
  གལ་ཏེ (name)
    ཕྱིར syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  ཕྱིར syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#དེ་མིན
  (void) fd;
  (void) dirfd;
  (void) name;
  (void) syscall_move_mount;
  errno = ENOSYS;
  ཕྱིར -1;
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

static ཧྲིལ do_mount (libcrun_container_t *container, const var_na *source, ཧྲིལ targetfd,
                     const var_na *target, const var_na *fstype, unsigned long mountflags,
                     const void *data, ཧྲིལ extra_flags, libcrun_error_t *err);

static bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, const var_na *option)
{
  size_t i;
  ཆེད་དུ (i = 0; i < mnt->options_len; i++)
    གལ་ཏེ (mnt->options[i] != NULL && strcmp (mnt->options[i], option) == 0)
      ཕྱིར true;
  ཕྱིར false;
}

static runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, const var_na *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  ཆེད་དུ (i = 0; i < def->mounts_len; i++)
    {
      གལ་ཏེ (strcmp (def->mounts[i]->destination, destination) == 0)
        ཕྱིར def->mounts[i];
    }
  ཕྱིར NULL;
}

static void
warn_tmpfs_fallback_once (sa_nracanaa private_data_s *private_data, const var_na *reason)
{
  གལ་ཏེ (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
static ཧྲིལ
get_shared_empty_dir_cached (libcrun_container_t *container, var_na **proc_fd_path, libcrun_error_t *err)
{
  sa_nracanaa private_data_s *private_data = get_private_data (container);
  cleanup_close ཧྲིལ fd = -1;
  cleanup_free var_na *empty_dir_path = NULL;
  proc_fd_path_t fd_path;
  ཧྲིལ ret;

  /* Fast path: return cached proc fd path if already set up */
  གལ་ཏེ (private_data->maskdir_proc_path != NULL)
    {
      གལ་ཏེ (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      ཕྱིར 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_root, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  གལ་ཏེ (fd < 0)
    ཕྱིར fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  གལ་ཏེ (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  ཕྱིར 0;
}

static bool
is_dev_null (const sa_nracanaa stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  ཕྱིར S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
static ཧྲིལ
ensure_dev_null (ཧྲིལ fd, libcrun_error_t *err)
{
  sa_nracanaa stat st;

  གལ་ཏེ (UNLIKELY (fstat (fd, &st) < 0))
    ཕྱིར crun_make_error (err, errno, "stat `/dev/null`");

  གལ་ཏེ (UNLIKELY (! is_dev_null (&st)))
    ཕྱིར crun_make_error (err, 0, "`/dev/null` is not the null device");

  ཕྱིར 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
static bool
is_empty_ro_tmpfs (ཧྲིལ fd)
{
  sa_nracanaa statfs sfs;

  གལ་ཏེ (fstatfs (fd, &sfs) < 0)
    ཕྱིར false;

  ཕྱིར sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

static ཧྲིལ
mount_masked_dir (libcrun_container_t *container, ཧྲིལ pathfd, const var_na *rel_path, libcrun_error_t *err)
{
  sa_nracanaa private_data_s *private_data = get_private_data (container);
  cleanup_close ཧྲིལ newfd = -1;
  libcrun_error_t tmp_err = NULL;
  ཧྲིལ ret;

  གལ་ཏེ (private_data->maskdir_fd < 0)
    {
      གལ་ཏེ (private_data->maskdir_bind_failed)
        goto fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, NULL, &tmp_err);
      གལ་ཏེ (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->msg);
          crun_error_release (&tmp_err);
          goto fallback_to_tmpfs;
        }
    }

  གལ་ཏེ (private_data->maskdir_fd >= 0)
    {
      ཧྲིལ rootfsfd = private_data->rootfsfd;
      sa_nracanaa stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, NULL);
      གལ་ཏེ (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          གལ་ཏེ (UNLIKELY (newfd < 0))
            ཕྱིར newfd;

          ret = fstat (newfd, &after);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          གལ་ཏེ (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            ཕྱིར crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          ཕྱིར 0;
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
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  གལ་ཏེ (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      གལ་ཏེ (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      དེ་མིན
        {
          གལ་ཏེ (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  ཕྱིར 0;
}

static ཧྲིལ
do_masked_or_readonly_path (libcrun_container_t *container, const var_na *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  const var_na *rootfs = get_private_data (container)->rootfs;
  cleanup_close ཧྲིལ pathfd = -1;
  sa_nracanaa statfs sfs;
  ཧྲིལ ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  གལ་ཏེ (UNLIKELY (rel_path == NULL))
    ཕྱིར crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  གལ་ཏེ (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  གལ་ཏེ (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      གལ་ཏེ (errno != ENOENT && errno != EACCES)
        ཕྱིར pathfd;

      གལ་ཏེ (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      ཕྱིར 0;
    }

  གལ་ཏེ (readonly)
    {
      ཧྲིལ procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close ཧྲིལ mountfd = -1;

      གལ་ཏེ (UNLIKELY (procfd < 0))
        ཕྱིར procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      གལ་ཏེ (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          གལ་ཏེ (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, NULL);
          དེ་མིན
            ret = -1;

          གལ་ཏེ (ret < 0)
            crun_error_release (err);
        }

      གལ་ཏེ (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          གལ་ཏེ (keep_flags)
            {
              cleanup_close ཧྲིལ sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              གལ་ཏེ (UNLIKELY (sfd < 0))
                ཕྱིར crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              གལ་ཏེ (UNLIKELY (ret < 0))
                ཕྱིར crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          གལ་ཏེ (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              གལ་ཏེ (UNLIKELY (ret < 0))
                ཕྱིར ret;

              ret = fs_move_mount_to (mountfd, pathfd, NULL);
              གལ་ཏེ (UNLIKELY (ret < 0))
                ཕྱིར crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          དེ་མིན
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, NULL, mount_flags, NULL,
                              LABEL_NONE, err);
              གལ་ཏེ (UNLIKELY (ret < 0))
                ཕྱིར ret;
            }
        }
    }
  དེ་མིན
    {
      ret = get_file_type_fd (pathfd, &mode);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      གལ་ཏེ ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      དེ་མིན
        {
          cleanup_close ཧྲིལ nullfd = -1;
          cleanup_close ཧྲིལ mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          གལ་ཏེ (UNLIKELY (nullfd < 0))
            ཕྱིར crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          གལ་ཏེ (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, NULL);

          གལ་ཏེ (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, NULL, MS_BIND | MS_RDONLY, NULL,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
  ཕྱིར 0;
}

static const var_na *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  const var_na *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  གལ་ཏེ (context_type)
    {
      གལ་ཏེ (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          ཕྱིར NULL;
        }
      ཕྱིར context_type;
    }

  ཕྱིར "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
static ཧྲིལ
diagnose_mount_failure (ཧྲིལ ret, libcrun_error_t *err, libcrun_container_t *container,
                        const var_na *fstype)
{
  གལ་ཏེ (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      གལ་ཏེ (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          ཕྱིར ret;
        }
    }
  ཕྱིར ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
static ཧྲིལ
sysfs_userns_fallback (libcrun_container_t *container, const var_na *target,
                       const var_na *real_target, ཧྲིལ targetfd,
                       unsigned long mountflags, const var_na *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close ཧྲིལ mountfd = -1;
  cleanup_close ཧྲིལ sysfd = -1;
  ཧྲིལ sys_old_root_fd = -1;
  ཧྲིལ ret;

  གལ་ཏེ (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    ཕྱིར 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  གལ་ཏེ (ret == 0)
    ཕྱིར 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_root_fd = get_old_root_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  གལ་ཏེ (sys_old_root_fd >= 0)
    mountfd = get_bind_mount (sys_old_root_fd, "sys", true, true, false, MS_PRIVATE, err);
  གལ་ཏེ (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  གལ་ཏེ (cgroup_mount == NULL)
    {
      གལ་ཏེ (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, NULL);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      དེ་མིན
        {
          crun_error_release (err);
          གལ་ཏེ (sys_old_root_fd >= 0)
            {
              cleanup_free var_na *sys_path = NULL;
              proc_fd_path_t sys_root_path;

              get_proc_self_fd_path (sys_root_path, sys_old_root_fd);
              xasprintf (&sys_path, "%s/sys", sys_root_path);
              ret = mount (sys_path, real_target, NULL, MS_BIND | MS_REC, NULL);
            }
          དེ་མིན
            ret = mount ("/sys", real_target, NULL, MS_BIND | MS_REC, NULL);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      གལ་ཏེ (UNLIKELY (sysfd < 0))
        ཕྱིར sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        NULL, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ཕྱིར do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  གལ་ཏེ (UNLIKELY (mountfd < 0))
    ཕྱིར mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, NULL);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  གལ་ཏེ (UNLIKELY (sysfd < 0))
    ཕྱིར sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    NULL, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  གལ་ཏེ (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ཕྱིར 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
static void
apply_selinux_xattr (libcrun_container_t *container, ཧྲིལ fd, ཧྲིལ extra_flags, const var_na *label)
{
  libcrun_error_t tmp_err = NULL;
  ཧྲིལ procfd;

  གལ་ཏེ ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    ཕྱིར;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  གལ་ཏེ (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close ཧྲིལ xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      གལ་ཏེ (xfd >= 0)
        (void) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
static ཧྲིལ
do_mount_replace_rootfs (libcrun_container_t *container, ཧྲིལ *fd, libcrun_error_t *err)
{
  ཧྲིལ procfd = get_procfd (get_private_data (container), err);
  ཧྲིལ tmp;
  ཧྲིལ ret;

  གལ་ཏེ (UNLIKELY (procfd < 0))
    ཕྱིར procfd;

  {
    cleanup_close ཧྲིལ nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    གལ་ཏེ (UNLIKELY (nsfd < 0))
      ཕྱིར crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    གལ་ཏེ (UNLIKELY (ret < 0))
      ཕྱིར crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (*fd < 0))
    ཕྱིར crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  གལ་ཏེ (UNLIKELY (tmp < 0))
    ཕྱིར crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  ཕྱིར 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
static ཧྲིལ
apply_propagation_flags (const var_na *target, ཧྲིལ targetfd, const var_na *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  ཧྲིལ ret;

  གལ་ཏེ (targetfd >= 0)
    {
      libcrun_error_t tmp_err = NULL;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      གལ་ཏེ (LIKELY (ret == 0))
        propagation_done = true;
      དེ་མིན
        crun_error_release (&tmp_err);
    }

  གལ་ཏེ (! propagation_done)
    {
      ret = mount (NULL, real_target, NULL, mountflags & ALL_PROPAGATIONS, NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  ཕྱིར 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
static ཧྲིལ
schedule_or_do_remount (libcrun_container_t *container, ཧྲིལ *fd, ཧྲིལ targetfd,
                        const var_na *target, const var_na *real_target,
                        bool single_instance, unsigned long mountflags,
                        const void *data, ཧྲིལ extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  sa_nracanaa remount_s *r;

  གལ་ཏེ ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    ཕྱིར do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  གལ་ཏེ (*fd < 0)
    {
      *fd = dup (targetfd);
      གལ་ཏེ (UNLIKELY (*fd < 0))
        ཕྱིར crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  ཕྱིར 0;
}

static ཧྲིལ
do_mount (libcrun_container_t *container, const var_na *source, ཧྲིལ targetfd,
          const var_na *target, const var_na *fstype, unsigned long mountflags, const void *data,
          ཧྲིལ extra_flags, libcrun_error_t *err)
{
  cleanup_free var_na *data_with_label = NULL;
  const var_na *data_without_label = data;
  proc_fd_path_t target_buffer;
  const var_na *context_type = NULL;
  cleanup_close ཧྲིལ ms_move_fd = -1;
  const var_na *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close ཧྲིལ fd = -1;
  const var_na *label = NULL;
  ཧྲིལ ret = 0;

  གལ་ཏེ (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  དེ་མིན
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  གལ་ཏེ (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  གལ་ཏེ ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      གལ་ཏེ (UNLIKELY (context_type == NULL))
        ཕྱིར -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      གལ་ཏེ (ret < 0)
        ཕྱིར ret;
      data = data_with_label;
    }

  གལ་ཏེ (mountflags & MS_MOVE)
    {
      གལ་ཏེ ((mountflags & MS_BIND) || fstype)
        ཕྱིར crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, NULL, MS_MOVE, NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      གལ་ཏེ (UNLIKELY (ms_move_fd < 0))
        ཕྱིར ms_move_fd;
      targetfd = ms_move_fd;
    }

  གལ་ཏེ ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      གལ་ཏེ (targetfd >= 0)
        {
          གལ་ཏེ (mountflags & MS_BIND)
            {
              cleanup_close ཧྲིལ bindfd = -1;
              libcrun_error_t tmp_err = NULL;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              གལ་ཏེ (bindfd < 0)
                {
                  ཧྲིལ old_root_fd = get_old_root_fd (get_private_data (container));

                  གལ་ཏེ (old_root_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_root_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              གལ་ཏེ (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, NULL);
              གལ་ཏེ (ret < 0)
                crun_error_release (&tmp_err);
            }
          དེ་མིན
            {
              cleanup_close ཧྲིལ newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              གལ་ཏེ (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, NULL);
            }
        }

      གལ་ཏེ (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          གལ་ཏེ (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              ཧྲིལ old_root_fd = get_old_root_fd (get_private_data (container));

              གལ་ཏེ (old_root_fd >= 0)
                {
                  cleanup_free var_na *proc_source_buf = NULL;
                  proc_fd_path_t old_root_path;

                  get_proc_self_fd_path (old_root_path, old_root_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_root_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          ཧྲིལ saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          གལ་ཏེ (ret == 0)
            ཕྱིར 0;
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          ཕྱིར diagnose_mount_failure (ret, err, container, fstype);
        }

      གལ་ཏེ (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          གལ་ཏེ (UNLIKELY (fd < 0))
            ཕྱིར fd;

          /* We are replacing the rootfs, reopen it.  */
          གལ་ཏེ (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              གལ་ཏེ (UNLIKELY (ret < 0))
                ཕྱིར ret;
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

  གལ་ཏེ (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  གལ་ཏེ (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  གལ་ཏེ (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  གལ་ཏེ (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ཕྱིར ret;
}

static void
try_umount (ཧྲིལ targetfd, const var_na *target)
{
  proc_fd_path_t target_buffer;
  const var_na *real_target = target;

  གལ་ཏེ (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

static bool
container_has_cgroupns (libcrun_container_t *container)
{
  ཕྱིར get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

static ཧྲིལ
do_mount_cgroup_v2 (libcrun_container_t *container, ཧྲིལ targetfd, const var_na *target,
                    unsigned long mountflags, const var_na *data, libcrun_error_t *err)
{
  ཧྲིལ ret;
  ཧྲིལ cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  གལ་ཏེ (UNLIKELY (cgroup_mode < 0))
    ཕྱིར cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      ཧྲིལ saved_errno = crun_error_get_errno (err);
      གལ་ཏེ (saved_errno == EPERM || saved_errno == EBUSY)
        {
          const var_na *unified_cgroup_path;
          const var_na *src_cgroup;

          crun_error_release (err);

          གལ་ཏེ (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              གལ་ཏེ (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  གལ་ཏེ (LIKELY (ret == 0))
                    ཕྱིར ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_ROOT;
          ཕྱིར do_mount (container, src_cgroup, targetfd, target, NULL,
                           MS_BIND | mountflags, NULL, LABEL_NONE, err);
        }

      ཕྱིར ret;
    }

  ཕྱིར 0;
}

static ཧྲིལ
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, const var_na *source, ཧྲིལ targetfd, const var_na *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  ཧྲིལ ret;
  cleanup_close ཧྲིལ fd = -1;
  const var_na *subsystem = "systemd";
  cleanup_free var_na *subsystem_path = NULL;
  cleanup_close ཧྲིལ tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  གལ་ཏེ (UNLIKELY (tmpfsdirfd < 0))
    ཕྱིར tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, NULL);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  གལ་ཏེ (UNLIKELY (fd < 0))
    ཕྱིར crun_make_error (err, errno, "openat `%s`", subsystem_path);

  ཕྱིར do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

static ཧྲིལ
do_mount_cgroup_v1 (libcrun_container_t *container, const var_na *source, ཧྲིལ targetfd, const var_na *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  ཧྲིལ ret;
  cleanup_free var_na *content = NULL;
  var_na *from;
  cleanup_close ཧྲིལ tmpfsdirfd = -1;
  var_na *saveptr = NULL;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  གལ་ཏེ (UNLIKELY (tmpfsdirfd < 0))
    ཕྱིར tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    ཧྲིལ procfd = get_procfd (get_private_data (container), err);
    གལ་ཏེ (UNLIKELY (procfd < 0))
      ཕྱིར procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, NULL, err);
  }
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  གལ་ཏེ (UNLIKELY (content == NULL || content[0] == '\0'))
    ཕྱིར crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ཆེད་དུ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (NULL, "\n", &saveptr))
    {
      cleanup_free var_na *source_path = NULL;
      cleanup_free var_na *source_subsystem = NULL;
      cleanup_free var_na *subsystem_path = NULL;
      var_na *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close ཧྲིལ subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      གལ་ཏེ (subsystem[0] == '\0')
        continue;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      གལ་ཏེ (it)
        subsystem = it + 5;

      གལ་ཏེ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      གལ་ཏེ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_ROOT, subsystem, NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      /* if there is already a mount specified, do not add a default one.  */
      གལ་ཏེ (find_mount_for (container, source_subsystem) != NULL)
        continue;

      ret = append_paths (&source_path, err, source_subsystem, subpath, NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      གལ་ཏེ (UNLIKELY (subsystemfd < 0))
        ཕྱིར crun_make_error (err, errno, "open `%s`", subsystem_path);

      གལ་ཏེ (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            {
              གལ་ཏེ (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  continue;
                }
              ཕྱིར ret;
            }
        }
      དེ་མིན
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, NULL, MS_BIND | mountflags, NULL,
                          LABEL_NONE, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            {
              གལ་ཏེ (crun_error_get_errno (err) != ENOENT)
                ཕྱིར ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, NULL, MS_BIND | mountflags,
                              NULL, LABEL_NONE, err);
              གལ་ཏེ (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  གལ་ཏེ (crun_error_get_errno (err) != ENOENT)
                    ཕྱིར ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཕྱིར 0;
}

static ཧྲིལ
do_mount_cgroup (libcrun_container_t *container, const var_na *source, ཧྲིལ targetfd, const var_na *target,
                 unsigned long mountflags, const var_na *data, libcrun_error_t *err)
{
  ཧྲིལ cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  གལ་ཏེ (UNLIKELY (cgroup_mode < 0))
    ཕྱིར cgroup_mode;

  switch (cgroup_mode)
    {
    pak_sa CGROUP_MODE_UNIFIED:
      ཕྱིར do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    pak_sa CGROUP_MODE_LEGACY:
    pak_sa CGROUP_MODE_HYBRID:
      ཕྱིར do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  ཕྱིར crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
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

  གལ་ཏེ (private_data->needed_devs_fds)
    {
      sa_nracanaa libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      ཆེད་དུ (i = 0; needed_devs[i].path; i++)
        {
          གལ་ཏེ (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            continue;

          ཧྲིལ fd = syscall_open_tree (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          གལ་ཏེ (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          དེ་མིན གལ་ཏེ (fd >= 0)
            close (fd);
        }
    }

  གལ་ཏེ (private_data->notify_socket_tree_fd < 0
      && private_data->host_notify_socket_path)
    {
      ཧྲིལ fd = syscall_open_tree (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      གལ་ཏེ (fd >= 0)
        private_data->notify_socket_tree_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
static const var_na *
relative_path_under_dev (const var_na *path)
{
  གལ་ཏེ (path[0] != '/')
    ཕྱིར path;
  གལ་ཏེ (strncmp (path, "/dev/", 5) == 0)
    {
      གལ་ཏེ (strchr (path + 5, '/'))
        ཕྱིར NULL;
      ཕྱིར path + 5;
    }
  ཕྱིར NULL;
}

static ཧྲིལ
mknod_and_set_attrs (libcrun_container_t *container, ཧྲིལ dirfd, const var_na *name, sa_nracanaa device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ fd = -1;
  proc_fd_path_t fd_path;
  const var_na *what = "new";
  bool exists = false;
  sa_nracanaa stat st;
  ཧྲིལ procfd;
  ཧྲིལ ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      གལ་ཏེ (errno != EEXIST)
        ཕྱིར crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (fd < 0))
    ཕྱིར crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  གལ་ཏེ (UNLIKELY ((st.st_mode & S_IFMT) != type))
    ཕྱིར crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  གལ་ཏེ (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    ཕྱིར crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  གལ་ཏེ (exists)
    ཕྱིར 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  གལ་ཏེ (UNLIKELY (procfd < 0))
    ཕྱིར procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "fchown `%s`", device->path);

  ཕྱིར 1;
}

ཧྲིལ
libcrun_create_dev (libcrun_container_t *container, ཧྲིལ devfd, ཧྲིལ srcfd,
                    sa_nracanaa device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  ཧྲིལ ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  const var_na *fullname = device->path;
  const var_na *rootfs = get_private_data (container)->rootfs;
  གལ་ཏེ (is_empty_string (fullname))
    ཕྱིར crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free var_na *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  གལ་ཏེ (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  const var_na *rel_dev = relative_path_under_dev (normalized_path);

  གལ་ཏེ (binds)
    {
      cleanup_close ཧྲིལ fd = -1;

      གལ་ཏེ (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          གལ་ཏེ (UNLIKELY (fd < 0))
            {
              གལ་ཏེ (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              གལ་ཏེ (UNLIKELY (fd < 0))
                ཕྱིར crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      དེ་མིན
        {
          const var_na *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          གལ་ཏེ (UNLIKELY (fd < 0))
            ཕྱིར fd;
        }

      གལ་ཏེ (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          གལ་ཏེ (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              གལ་ཏེ (LIKELY (ret >= 0))
                ཕྱིར 0;
            }
          དེ་མིན
            crun_error_release (err);
        }

      {
        cleanup_close ཧྲིལ mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        གལ་ཏེ (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            གལ་ཏེ (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, NULL);
                གལ་ཏེ (LIKELY (ret == 0))
                  ཕྱིར 0;
              }
            དེ་མིན
              crun_error_release (err);
          }
        དེ་མིན
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, NULL, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, NULL, LABEL_MOUNT, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
  དེ་མིན
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      གལ་ཏེ (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          གལ་ཏེ (ret <= 0)
            ཕྱིར ret;
        }
      དེ་མིན
        {
          cleanup_close ཧྲིལ dirfd = -1;
          cleanup_free var_na *dirname = NULL;
          var_na *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          གལ་ཏེ (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          གལ་ཏེ (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              གལ་ཏེ (UNLIKELY (dirfd < 0))
                ཕྱིར crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          དེ་མིན
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              གལ་ཏེ (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              གལ་ཏེ (UNLIKELY (dirfd < 0))
                ཕྱིར dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          གལ་ཏེ (ret <= 0)
            ཕྱིར ret;
        }
    }
  ཕྱིར 0;
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

static ཧྲིལ
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  ཧྲིལ ret;
  size_t i;
  sa_nracanaa device_s *it;
  cleanup_close ཧྲིལ devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  const var_na *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map sa_nracanaa libcrun_fd_map *dev_fds = NULL;
  cleanup_close_map sa_nracanaa libcrun_fd_map *needed_fds = NULL;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = NULL;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = NULL;

  གལ་ཏེ (! def || ! def->linux)
    ཕྱིར 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  གལ་ཏེ (UNLIKELY (devfd < 0))
    ཕྱིར devfd;

  ཆེད་དུ (i = 0; i < def->linux->devices_len; i++)
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

      གལ་ཏེ (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ཆེད་དུ (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close ཧྲིལ srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
      གལ་ཏེ (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  ཆེད་དུ (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          ཧྲིལ saved_errno = errno;

          གལ་ཏེ (errno == EEXIST && ! symlinks[i].force)
            continue;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          གལ་ཏེ (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              གལ་ཏེ (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              གལ་ཏེ (ret < 0 && errno == EBUSY)
                {
                  cleanup_close ཧྲིལ tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  གལ་ཏེ (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      གལ་ཏེ (umount2 (procpath, MNT_DETACH) == 0)
                        goto retry_unlink;
                    }
                }
              གལ་ཏེ (ret == 0)
                goto retry_symlink;
            }
          ཕྱིར crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  གལ་ཏེ (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ཕྱིར 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
static const var_na *
next_path_component (const var_na *p, size_t *len)
{
  ཆེད་དུ (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      གལ་ཏེ (*len != 1 || p[0] != '.')
        ཕྱིར p;
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

  ཆེད་དུ (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      གལ་ཏེ (la != lb || memcmp (a, b, la) != 0)
        ཕྱིར false;
      གལ་ཏེ (la == 0)
        ཕྱིར true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
static bool
is_duplicate_path (var_na **paths, size_t i)
{
  size_t j;

  གལ་ཏེ (paths[i] == NULL)
    ཕྱིར false;

  ཆེད་དུ (j = 0; j < i; j++)
    གལ་ཏེ (paths[j] && same_path (paths[j], paths[i]))
      ཕྱིར true;

  ཕྱིར false;
}

static ཧྲིལ
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  ཧྲིལ ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  གལ་ཏེ (! def || ! def->linux)
    ཕྱིར 0;

  ཆེད་དུ (i = 0; i < def->linux->masked_paths_len; i++)
    {
      གལ་ཏེ (is_duplicate_path (def->linux->masked_paths, i))
        continue;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
  ཆེད་དུ (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
  ཕྱིར 0;
}

static ཧྲིལ
do_pivot (libcrun_container_t *container, const var_na *rootfs, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ oldrootfd = -1;
  cleanup_close ཧྲིལ newrootfd = -1;
  ཧྲིལ ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (oldrootfd < 0))
    ཕྱིར crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (newrootfd < 0))
    ཕྱིར crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_root (".", ".");
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, NULL, -1, ".", NULL, MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = umount2 (".", MNT_DETACH);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "umount2 oldroot");

  do
    {
      ret = umount2 (".", MNT_DETACH);
      གལ་ཏེ (ret < 0 && errno == EINVAL)
        bha_nga;
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "umount2 oldroot");
  } སྐབས (ret == 0);

  ret = chdir ("/");
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "chdir to newroot");

  ཕྱིར 0;
}

static ཧྲིལ
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, var_na **data, libcrun_error_t *err)
{
  const var_na *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close ཧྲིལ fd = -1;
  sa_nracanaa stat st;
  ཧྲིལ ret;

  གལ་ཏེ (*data != NULL && strstr (*data, "mode="))
    ཕྱིར 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  གལ་ཏེ (fd < 0)
    {
      གལ་ཏེ (crun_error_get_errno (err) != ENOENT)
        ཕྱིར fd;

      crun_error_release (err);
      ཕྱིར 0;
    }
  ret = fstat (fd, &st);
  གལ་ཏེ (ret < 0)
    ཕྱིར crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  ཕྱིར 0;
}

static ཧྲིལ
get_default_flags (libcrun_container_t *container, const var_na *destination, var_na **data)
{
  གལ་ཏེ (strcmp (destination, "/proc") == 0)
    ཕྱིར 0;
  གལ་ཏེ (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      ཕྱིར MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  གལ་ཏེ (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      ཕྱིར MS_NOEXEC | MS_STRICTATIME;
    }
  གལ་ཏེ (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      ཕྱིར MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  གལ་ཏེ (strcmp (destination, "/dev/mqueue") == 0)
    ཕྱིར MS_NOEXEC | MS_NOSUID | MS_NODEV;
  གལ་ཏེ (strcmp (destination, "/dev/pts") == 0)
    {
      གལ་ཏེ (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      དེ་མིན
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      ཕྱིར MS_NOEXEC | MS_NOSUID;
    }
  གལ་ཏེ (strcmp (destination, "/sys") == 0)
    ཕྱིར MS_NOEXEC | MS_NOSUID | MS_NODEV;

  ཕྱིར 0;
}

static const var_na *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  ཕྱིར find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

static ཧྲིལ
check_valid_no_follow_file_system (ཧྲིལ fd, const var_na *destination, libcrun_error_t *err)
{
  sa_nracanaa statfs sfs;

  ཧྲིལ ret = fstatfs (fd, &sfs);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "statfs `%s`", destination);

  switch (sfs.f_type)
    {
    pak_sa PROC_SUPER_MAGIC:
    pak_sa SYSFS_MAGIC:
    pak_sa DEVPTS_SUPER_MAGIC:
      ཕྱིར crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  ཕྱིར 0;
}

static ཧྲིལ
safe_create_symlink (ཧྲིལ rootfsfd, const var_na *rootfs, const var_na *target, const var_na *destination, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ parent_dir_fd = -1;
  cleanup_free var_na *buffer = NULL;
  var_na *part;
  ཧྲིལ ret;

  གལ་ཏེ (is_empty_string (destination))
    ཕྱིར crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  གལ་ཏེ (UNLIKELY (parent_dir_fd < 0))
    ཕྱིར parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      གལ་ཏེ (errno == EEXIST)
        {
          cleanup_free var_na *link = NULL;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          གལ་ཏེ (UNLIKELY (len < 0))
            ཕྱིར len;

          གལ་ཏེ ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            ཕྱིར 0;

          ཕྱིར crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      ཕྱིར crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  ཕྱིར 0;
}

static ཧྲིལ
handle_copy_symlink (libcrun_container_t *container, const var_na *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     const var_na *link_target, libcrun_error_t *err)
{
  cleanup_free var_na *target = NULL;

  གལ་ཏེ (link_target == NULL)
    {
      ཧྲིལ old_root_fd = get_old_root_fd (get_private_data (container));
      const var_na *source = mount->source;
      ssize_t len;

      གལ་ཏེ (old_root_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_root_fd, source + 1, &target, 0, err);
      དེ་མིན
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      གལ་ཏེ (UNLIKELY (len < 0))
        ཕྱིར len;
      link_target = target;
    }

  ཕྱིར safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

static ཧྲིལ
prepare_sysfs_or_proc_mount (libcrun_container_t *container, const var_na *target,
                             ཧྲིལ *targetfd, libcrun_error_t *err)
{
  ཧྲིལ ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      གལ་ཏེ (errno == ENOENT)
        {
          གལ་ཏེ (strchr (target, '/'))
            ཕྱིར crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      དེ་མིན གལ་ཏེ (errno == ENOTDIR)
        ཕྱིར crun_make_error (err, errno, "the target `/%s` is invalid", target);

      གལ་ཏེ (ret < 0)
        ཕྱིར crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  ཕྱིར 0;
}

static ཧྲིལ
handle_tmpcopyup (libcrun_container_t *container, const var_na *rootfs, const var_na *target,
                  ཧྲིལ copy_from_fd, libcrun_error_t *err)
{
  ཧྲིལ destfd, ret;
  cleanup_close ཧྲིལ tmpfd = copy_from_fd;

  target = consume_slashes (target);

  གལ་ཏེ (is_empty_string (target))
    ཕྱིར crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  གལ་ཏེ (UNLIKELY (destfd < 0))
    ཕྱིར crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  ཕྱིར ret;
}

static ཧྲིལ
get_mount_label_how (const var_na *type, bool is_sysfs_or_proc)
{
  གལ་ཏེ (is_sysfs_or_proc)
    ཕྱིར LABEL_NONE;

  གལ་ཏེ (strcmp (type, "mqueue") == 0)
    ཕྱིར LABEL_XATTR;

  ཕྱིར LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
static unsigned long
get_bind_mount_locked_flags (ཧྲིལ source_mountfd, const var_na *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  sa_nracanaa statfs sfs;
  size_t i;
  ཧྲིལ ret;

  གལ་ཏེ (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  དེ་མིན
    ret = statfs (source, &sfs);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར 0;

  ཆེད་དུ (i = 0; i < mount->options_len; i++)
    {
      const sa_nracanaa propagation_flags_s *prop;

      གལ་ཏེ (mount->options[i] == NULL)
        continue;

      prop = libcrun_str2mount_flags (mount->options[i]);
      གལ་ཏེ (prop == NULL || (prop->extra_flags & OPTION_RECURSIVE))
        continue;

      གལ་ཏེ (prop->clear)
        cleared |= prop->flags;
      དེ་མིན
        cleared &= ~prop->flags;
    }

  ཕྱིར sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
static const var_na *
get_path_in_rootfs (const var_na *rootfs, const var_na *path)
{
  size_t len;

  གལ་ཏེ (rootfs == NULL || path == NULL)
    ཕྱིར NULL;

  len = strlen (rootfs);
  སྐབས (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  གལ་ཏེ (len == 0)
    ཕྱིར NULL;

  གལ་ཏེ (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    ཕྱིར NULL;

  ཕྱིར consume_slashes (path + len);
}

static ཧྲིལ
process_single_mount (libcrun_container_t *container, const var_na *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      sa_nracanaa libcrun_fd_map *mount_fds, size_t mount_index,
                      const var_na *systemd_cgroup_v1, libcrun_error_t *err)
{
  const var_na *target = consume_slashes (mount->destination);
  cleanup_close ཧྲིལ source_mountfd = -1;
  cleanup_free var_na *data = NULL;
  var_na *type;
  var_na *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close ཧྲིལ copy_from_fd = -1;
  cleanup_close ཧྲིལ targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  ཧྲིལ no_remount = 0;
  ཧྲིལ ret;

  གལ་ཏེ (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  གལ་ཏེ (mount->options == NULL)
    flags = get_default_flags (container, mount->destination, &data);
  དེ་མིན
    {
      size_t j;

      ཆེད་དུ (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  གལ་ཏེ (type == NULL && (flags & MS_BIND) == 0)
    ཕྱིར crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  གལ་ཏེ ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      const var_na *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      གལ་ཏེ (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          གལ་ཏེ (UNLIKELY (source_mountfd < 0))
            ཕྱིར crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  གལ་ཏེ ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  གལ་ཏེ (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      གལ་ཏེ (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      གལ་ཏེ (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  གལ་ཏེ (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  གལ་ཏེ (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      const var_na *path = mount->source;

      གལ་ཏེ ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        ཕྱིར crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      གལ་ཏེ (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, NULL);
        }
      དེ་མིན གལ་ཏེ (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      དེ་མིན གལ་ཏེ (path[0] == '/')
        {
          ཧྲིལ old_root_fd = get_old_root_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          གལ་ཏེ (old_root_fd >= 0)
            ret = get_file_type_at (old_root_fd, &src_mode, nofollow, (path[1] == '\0') ? NULL : path + 1);
          དེ་མིན
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      དེ་མིན
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "cannot stat `%s`", path);

      གལ་ཏེ (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          ཧྲིལ bind_dirfd = AT_FDCWD;
          const var_na *bind_source = mount->source;
          ཧྲིལ old_root_fd = get_old_root_fd (get_private_data (container));

          གལ་ཏེ (old_root_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_root_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          source_mountfd = ret;
        }
    }

  གལ་ཏེ (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      const var_na *cached_target = NULL;

      གལ་ཏེ (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      mounted = true;
    }
  དེ་མིན གལ་ཏེ (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
  དེ་མིན
    {
      bool is_dir = S_ISDIR (src_mode);

      གལ་ཏེ (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
      དེ་མིན
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
          targetfd = ret;
        }
    }

  གལ་ཏེ (extra_flags & OPTION_TMPCOPYUP)
    {
      གལ་ཏེ (strcmp (type, "tmpfs") != 0)
        ཕྱིར crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      གལ་ཏེ (UNLIKELY (copy_from_fd < 0))
        {
          གལ་ཏེ (errno != ENOTDIR)
            ཕྱིར crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  གལ་ཏེ (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, NULL);
      གལ་ཏེ (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          གལ་ཏེ (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, NULL, source_mountfd, target, NULL, remaining_flags, data, LABEL_NONE | no_remount, err);
              གལ་ཏེ (UNLIKELY (ret < 0))
                ཕྱིར ret;
            }

          mounted = true;

          གལ་ཏེ (is_empty_string (target))
            {
              ཧྲིལ new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              གལ་ཏེ (UNLIKELY (new_rootfsfd < 0))
                ཕྱིར crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      དེ་མིན གལ་ཏེ (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          ཕྱིར crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      དེ་མིན གལ་ཏེ (get_private_data (container)->host_root_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          ཕྱིར crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      དེ་མིན གལ་ཏེ (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  གལ་ཏེ (! mounted)
    {
      གལ་ཏེ (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
      དེ་མིན གལ་ཏེ (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
      དེ་མིན
        {
          ཧྲིལ extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
    }

  གལ་ཏེ (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  གལ་ཏེ (rec_clear || rec_set)
    {
      const bool is_dir = S_ISDIR (src_mode);
      cleanup_close ཧྲིལ dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      གལ་ཏེ (UNLIKELY (dfd < 0))
        ཕྱིར crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ཕྱིར 0;
}

static ཧྲིལ
do_mounts (libcrun_container_t *container, const var_na *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  const var_na *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map sa_nracanaa libcrun_fd_map *mount_fds = NULL;
  size_t i;
  ཧྲིལ ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = NULL;

  ཆེད་དུ (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
  ཕྱིར 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
ཧྲིལ
libcrun_container_do_bind_mount (libcrun_container_t *container, var_na *mount_source, var_na *mount_destination, var_na **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  const var_na *target = consume_slashes (mount_destination);
  cleanup_free var_na *data = NULL;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close ཧྲིལ targetfd = -1;
  ཧྲིལ is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  const var_na *rootfs = get_private_data (container)->rootfs;
  ཧྲིལ ret;

  གལ་ཏེ ((get_private_data (container)->rootfsfd < 0) || (rootfs == NULL))
    ཕྱིར crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  གལ་ཏེ (mount_options == NULL)
    flags = get_default_flags (container, mount_destination, &data);
  དེ་མིན
    {
      size_t j;

      ཆེད་དུ (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  གལ་ཏེ (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  གལ་ཏེ (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      གལ་ཏེ (UNLIKELY (is_dir < 0))
        ཕྱིར is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཕྱིར 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
ཧྲིལ
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, ཧྲིལ *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close ཧྲིལ notify_fd = -1;
  cleanup_free var_na *host_notify_socket_path = NULL;
  cleanup_free var_na *state_dir = NULL;
  var_na *host_path = NULL;
  ཧྲིལ ret;

  གལ་ཏེ (container && get_private_data (container)->host_notify_socket_path)
    {
      const var_na *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  གལ་ཏེ (host_path == NULL)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_root, context->id, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  གལ་ཏེ (UNLIKELY (notify_fd < 0))
    ཕྱིར notify_fd;

  གལ་ཏེ (UNLIKELY (chmod (host_path, 0777) < 0))
    ཕྱིར crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  གལ་ཏེ (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (void) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  ཕྱིར 1;
#དེ་མིན
  (void) context;
  (void) container;
  (void) err;
  *notify_socket_out = -1;
  ཕྱིར 0;
#endif
}

#ifdef HAVE_SYSTEMD
static ཧྲིལ
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  ཧྲིལ ret;
  const var_na *notify_socket = container->context->notify_socket;
  cleanup_free var_na *host_notify_socket_path = NULL;
  cleanup_free var_na *container_notify_socket_path = NULL;
  cleanup_free var_na *state_dir = NULL;
  uid_t container_root_uid = -1;
  gid_t container_root_gid = -1;
  ཧྲིལ notify_socket_tree_fd;

  གལ་ཏེ (notify_socket == NULL)
    ཕྱིར 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_root : NULL),
                                     (container->context ? container->context->id : NULL), err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", NULL);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", NULL);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = mkdir (host_notify_socket_path, 0700);
  གལ་ཏེ (ret < 0)
    ཕྱིར crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  གལ་ཏེ (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_root_in_the_userns (container->container_def, 0, 0, &container_root_uid, &container_root_gid);
      གལ་ཏེ (container_root_uid != ((uid_t) -1) && container_root_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_root_uid, container_root_gid);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_root_uid, container_root_gid);
        }
    }

  notify_socket_tree_fd = syscall_open_tree (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  གལ་ཏེ (notify_socket_tree_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_tree_fd = notify_socket_tree_fd;
  དེ་མིན གལ་ཏེ (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  དེ་མིན གལ་ཏེ (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  དེ་མིན
    /* some other error */
    ཕྱིར crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = NULL;
  ཕྱིར 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
static ཧྲིལ
do_finalize_notify_socket (libcrun_container_t *container, const var_na *rootfs, libcrun_error_t *err)
{
  ཧྲིལ ret;
  cleanup_free var_na *host_notify_socket_path = NULL;
  cleanup_free var_na *container_notify_socket_path = NULL;
  cleanup_free var_na *container_notify_socket_path_dir_alloc = NULL;
  const var_na *container_notify_socket_path_dir = NULL;
  cleanup_close ཧྲིལ notify_socket_tree_fd = -1;
  cleanup_close ཧྲིལ targetfd = -1;
  ཧྲིལ did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = NULL;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = NULL;

  གལ་ཏེ (host_notify_socket_path == NULL || container_notify_socket_path == NULL)
    ཕྱིར 0;

  གལ་ཏེ (rootfs == NULL)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  གལ་ཏེ (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  targetfd = ret;

  notify_socket_tree_fd = get_private_data (container)->notify_socket_tree_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_tree_fd = -1;

  གལ་ཏེ (notify_socket_tree_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_tree_fd, targetfd, NULL);
      གལ་ཏེ (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          གལ་ཏེ (UNLIKELY (targetfd < 0))
            ཕྱིར targetfd;

          ret = do_mount (container, NULL, targetfd, container_notify_socket_path_dir, NULL,
                          MS_REC | MS_PRIVATE, NULL, LABEL_NONE, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
      དེ་མིན གལ་ཏེ (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      དེ་མིན
        ཕྱིར crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_tree_fd,
                                container_notify_socket_path_dir);
    }

  གལ་ཏེ (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, NULL,
                      MS_BIND | MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ཕྱིར 0;
}

static ཧྲིལ
make_parent_mount_private (const var_na *rootfs, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ rootfsfd = -1;
  sa_nracanaa stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (rootfsfd < 0))
    ཕྱིར crun_make_error (err, errno, "open `%s`", rootfs);

  གལ་ཏེ (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    ཕྱིར crun_make_error (err, errno, "fstat `%s`", rootfs);

  ཆེད་དུ (;;)
    {
      sa_nracanaa stat cur_st;
      libcrun_error_t tmp_err = NULL;
      ཧྲིལ ret;
      cleanup_close ཧྲིལ parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      གལ་ཏེ (ret == 0)
        ཕྱིར 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (NULL, proc_path, NULL, MS_PRIVATE, NULL);
      གལ་ཏེ (ret == 0)
        ཕྱིར 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      གལ་ཏེ (parentfd < 0)
        {
          ཧྲིལ saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          གལ་ཏེ (ret != 0)
            ཕྱིར crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          ཕྱིར crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      གལ་ཏེ (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        ཕྱིར crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      གལ་ཏེ (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        bha_nga;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  ཕྱིར crun_make_error (err, 0, "make `%s` private", rootfs);
}

ཧྲིལ
libcrun_set_mounts (sa_nracanaa container_entrypoint_s *entrypoint_args, libcrun_container_t *container, const var_na *rootfs, set_mounts_cb_t cb, void *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ཧྲིལ is_user_ns = 0;
  ཧྲིལ ret = 0;

  གལ་ཏེ (rootfs == NULL || def->mounts == NULL)
    ཕྱིར 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  གལ་ཏེ (def->root->readonly)
    {
      sa_nracanaa remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      ཧྲིལ fd;

      fd = dup (get_private_data (container)->rootfsfd);
      གལ་ཏེ (UNLIKELY (fd < 0))
        ཕྱིར crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, NULL, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  གལ་ཏེ (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      གལ་ཏེ (UNLIKELY (is_user_ns < 0))
        ཕྱིར is_user_ns;
    }

  གལ་ཏེ (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  གལ་ཏེ (cb)
    {
      ret = cb (cb_data, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  གལ་ཏེ (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = NULL;
      const var_na *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (void) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཕྱིར 0;
}

static ཧྲིལ
umount_or_hide (const var_na *target, libcrun_error_t *err)
{
  ཧྲིལ ret;

  ret = umount2 (target, MNT_DETACH);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      ཧྲིལ saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      གལ་ཏེ (errno == EINVAL)
        {
          ret = mount (NULL, target, "tmpfs", 0, "size=0k");
          གལ་ཏེ (LIKELY (ret == 0))
            ཕྱིར 0;
        }

      ཕྱིར crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  ཕྱིར ret;
}

static ཧྲིལ
move_root (const var_na *rootfs, libcrun_error_t *err)
{
  ཧྲིལ ret;

  ret = chdir (rootfs);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = umount_or_hide ("/proc", err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ཕྱིར 0;
}

ཧྲིལ
libcrun_finalize_mounts (sa_nracanaa container_entrypoint_s *entrypoint_args, libcrun_container_t *container, const var_na *rootfs,
                         set_mounts_cb_t after_pivot_cb, void *cb_data, libcrun_error_t *err)
{
  ཧྲིལ ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_root_fd));

  གལ་ཏེ (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      གལ་ཏེ (get_private_data (container)->no_pivot)
        {
          ret = move_root (rootfs, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
      དེ་མིན
        {
          ret = do_pivot (container, rootfs, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }

      ret = do_mount (container, NULL, -1, "/", NULL,
                      get_private_data (container)->rootfs_propagation,
                      NULL, LABEL_MOUNT, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = chdir ("/");
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "chdir to `/`");
    }

  གལ་ཏེ (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ret = finalize_mounts (container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཕྱིར 0;
}

static ཧྲིལ
maybe_open_tree_namespace (const var_na *rootfs, ཧྲིལ *out_fd, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ rootfs_fd = -1;
  ཧྲིལ tree_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (rootfs_fd < 0))
    ཕྱིར crun_make_error (err, errno, "open `%s`", rootfs);

  tree_fd = syscall_open_tree (rootfs_fd, "",
                               OPEN_TREE_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  གལ་ཏེ (tree_fd < 0)
    {
      གལ་ཏེ (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        ཕྱིར 0;
      ཕྱིར crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = tree_fd;
  ཕྱིར 0;
}

static sa_nracanaa libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  sa_nracanaa libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  གལ་ཏེ (mount_fds == NULL)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  ཕྱིར mount_fds;
}

static ཧྲིལ
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
  ཧྲིལ mnt_fd;
  size_t j;

  གལ་ཏེ (mount->type == NULL)
    ཕྱིར -1;

  fstype = mount->type;

  ཆེད་དུ (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  གལ་ཏེ (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      ཧྲིལ extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      གལ་ཏེ (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          གལ་ཏེ (label_type == NULL)
            crun_error_release (err);
          དེ་མིན
            label_val = def->linux->mount_label;
        }
    }

  གལ་ཏེ (strcmp (fstype, "cgroup") == 0)
    {
      ཧྲིལ cgroup_mode = libcrun_get_cgroup_mode (err);
      གལ་ཏེ (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  གལ་ཏེ (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, NULL, NULL, mnt_data);

  ཕྱིར mnt_fd;
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

static ཧྲིལ
syscall_listmount (sa_nracanaa mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned ཧྲིལ flags)
{
#ifdef __NR_listmount
  ཕྱིར (ཧྲིལ) syscall (__NR_listmount, req, ids, nr, flags);
#དེ་མིན
  (void) req;
  (void) ids;
  (void) nr;
  (void) flags;
  errno = ENOSYS;
  ཕྱིར -1;
#endif
}

static ཧྲིལ
syscall_statmount (sa_nracanaa mnt_id_req_s *req, sa_nracanaa statmount_s *buf, size_t size, unsigned ཧྲིལ flags)
{
#ifdef __NR_statmount
  ཕྱིར (ཧྲིལ) syscall (__NR_statmount, req, buf, size, flags);
#དེ་མིན
  (void) req;
  (void) buf;
  (void) size;
  (void) flags;
  errno = ENOSYS;
  ཕྱིར -1;
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

  ཆེད་དུ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      གལ་ཏེ (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != NULL && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          bha_nga;
        }
    }
  གལ་ཏེ (! has_absolute_bind)
    ཕྱིར false;

  ids = xmalloc (capacity * parimaa_na (uint64_t));
  ཆེད་དུ (;;)
    {
      sa_nracanaa mnt_id_req_s req = {
        .size = parimaa_na (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      ཧྲིལ ret;

      གལ་ཏེ (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * parimaa_na (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      གལ་ཏེ (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        ཕྱིར true;
      གལ་ཏེ (ret == 0)
        bha_nga;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  ཆེད་དུ (i = 0; i < n_ids; i++)
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

      གལ་ཏེ (syscall_statmount (&req, sm, parimaa_na (buffer), 0) < 0)
        continue;
      གལ་ཏེ ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        continue;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      root = sm->str + sm->mnt_root;
      གལ་ཏེ (! has_prefix (root, "mnt:["))
        continue;

      point = sm->str + sm->mnt_point;

      ཆེད་དུ (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          const var_na *source = def->mounts[j]->source;
          size_t source_len;

          གལ་ཏེ (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            continue;
          གལ་ཏེ (source == NULL || source[0] != '/')
            continue;

          source_len = strlen (source);
          སྐབས (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          གལ་ཏེ (strncmp (point, source, source_len) != 0)
            continue;

          /* The source itself is a pinned mount namespace.  */
          གལ་ཏེ (point[source_len] == '\0')
            ཕྱིར true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          གལ་ཏེ (recursive && (source_len == 1 || point[source_len] == '/'))
            ཕྱིར true;
        }
    }

  ཕྱིར false;
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

  གལ་ཏེ (has_hooks || has_userns)
    ཕྱིར false;

  གལ་ཏེ (mounts_hit_pinned_mount_namespace (def))
    ཕྱིར false;

  གལ་ཏེ (container->context->no_open_tree_namespace)
    ཕྱིར false;

  {
    libcrun_error_t tmp_err = NULL;
    ཧྲིལ in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    གལ་ཏེ (in_userns != 0)
      ཕྱིར false;
  }

  mount_fds = get_fd_map (container);
  ཆེད་དུ (i = 0; i < def->mounts_len; i++)
    {
      གལ་ཏེ (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          གལ་ཏེ (! has_cached_target)
            ཕྱིར false;
        }
    }

  ཕྱིར true;
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

  གལ་ཏེ (mount->type == NULL || strcmp (mount->type, "tmpfs") != 0)
    ཕྱིར false;

  ཆེད་དུ (i = 0; i < mount->options_len; i++)
    གལ་ཏེ (mount->options[i] != NULL && has_prefix (mount->options[i], "mode="))
      ཕྱིར false;

  ཕྱིར true;
}

static ཧྲིལ
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, var_na **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close ཧྲིལ rootfsfd = -1;
  cleanup_close ཧྲིལ tree_fd = -1;
  libcrun_error_t tmp_err = NULL;
  bool use_open_tree = false;
  size_t i;
  ཧྲིལ ret;

  གལ་ཏེ (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, NULL, NULL, NULL, NULL);

  གལ་ཏེ ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  free (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  ཆེད་དུ (i = 0; i < def->mounts_len; i++)
    {
      sa_nracanaa libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close ཧྲིལ mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      གལ་ཏེ (mount_fds && mount_fds->fds[i] >= 0)
        continue;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      གལ་ཏེ (def->mounts[i]->source != NULL
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free var_na *mnt_data = NULL;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          ཆེད་དུ (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          གལ་ཏེ (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free var_na *target = NULL;
              libcrun_error_t tmp_err = NULL;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              གལ་ཏེ (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              གལ་ཏེ (get_private_data (container)->copy_symlink_targets == NULL)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * parimaa_na (var_na *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = NULL;
            }
        }

      གལ་ཏེ (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          གལ་ཏེ (mount_fds && def->mounts[i]->source != NULL && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == NULL)
            {
              libcrun_error_t tmp_err = NULL;
              unsigned long propagation = MS_PRIVATE;

              གལ་ཏེ (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              གལ་ཏེ (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          continue;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      གལ་ཏེ (mnt_fd < 0)
        {
          crun_error_release (err);
          continue;
        }

      གལ་ཏེ (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close ཧྲིལ destfd = -1;
          sa_nracanaa stat st;

          གལ་ཏེ (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              གལ་ཏེ (UNLIKELY (rootfsfd < 0))
                ཕྱིར crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          གལ་ཏེ (destfd < 0)
            crun_error_release (&tmp_err);
          དེ་མིན གལ་ཏེ (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              ཧྲིལ procfd = get_procfd (get_private_data (container), err);
              གལ་ཏེ (UNLIKELY (procfd < 0))
                ཕྱིར procfd;

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
  གལ་ཏེ (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  གལ་ཏེ (! get_private_data (container)->joined_mount_ns
      && can_use_open_tree_namespace (container))
    use_open_tree = true;

  གལ་ཏེ (use_open_tree)
    {
      ret = maybe_open_tree_namespace (*rootfs, &tree_fd, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  གལ་ཏེ (tree_fd >= 0)
    preopen_needed_devs_and_notify (container);

  གལ་ཏེ (tree_fd >= 0)
    {
      ret = setns (tree_fd, CLONE_NEWNS);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (NULL, "/", NULL, MS_REMOUNT | MS_BIND, NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, NULL, -1, "/", NULL, MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = do_mount (container, NULL, -1, "/", NULL, rootfs_propagation, NULL, LABEL_MOUNT, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_root_switched = true;
      free (*rootfs);
      *rootfs = xstrdup ("/");
    }
  དེ་མིན
    {
      གལ་ཏེ (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, NULL, -1, "/", NULL, rootfs_propagation, NULL, LABEL_MOUNT, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = make_parent_mount_private (*rootfs, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, NULL, MS_BIND | MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  གལ་ཏེ (! container->context->force_no_cgroup)
    {
      ཧྲིལ cgroup_mode = libcrun_get_cgroup_mode (err);
      གལ་ཏེ (UNLIKELY (cgroup_mode < 0))
        ཕྱིར cgroup_mode;

      གལ་ཏེ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          var_na *unified_cgroup_path = NULL;
          ཧྲིལ procfd = get_procfd (get_private_data (container), err);
          གལ་ཏེ (UNLIKELY (procfd < 0))
            ཕྱིར procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = NULL;
      ཕྱིར ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = NULL;

  གལ་ཏེ (tree_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  གལ་ཏེ (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  ཕྱིར 0;
}

ཧྲིལ
libcrun_do_pivot_root (libcrun_container_t *container, bool no_pivot, var_na **rootfs, libcrun_error_t *err)
{
  ཧྲིལ ret;

  གལ་ཏེ (*rootfs == NULL)
    ཕྱིར 0;

  གལ་ཏེ (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      གལ་ཏེ (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "chdir to `/`");
        }
    }
  དེ་མིན
    {
      ret = chroot (*rootfs);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "chdir to `/`");

      free (*rootfs);
      *rootfs = xstrdup ("/");
    }

  ཕྱིར 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
ཧྲིལ
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  sa_nracanaa stat statbuf;
  cleanup_close ཧྲིལ fd;
  ཧྲིལ ret;
  ཧྲིལ i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  གལ་ཏེ (UNLIKELY (fd == -1))
    ཕྱིར crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཆེད་དུ (i = 0; i <= 2; i++)
    {
      གལ་ཏེ (UNLIKELY (fstat (i, &statbuf) == -1))
        ཕྱིར crun_make_error (err, errno, "stat fd `%d`", i);
      གལ་ཏེ (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          གལ་ཏེ (UNLIKELY (dup2 (fd, i) == -1))
            ཕྱིར crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  ཕྱིར 0;
}

static ཧྲིལ
uidgidmap_helper (var_na *helper, pid_t pid, const var_na *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  var_na pid_fmt[16];
  var_na *args[MAX_ARGS + 1];
  var_na *next;
  cleanup_free var_na *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  ཧྲིལ ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, parimaa_na (pid_fmt), "%d", pid);
  གལ་ཏེ (UNLIKELY (ret >= (ཧྲིལ) parimaa_na (pid_fmt)))
    ཕྱིར crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  སྐབས (nargs < MAX_ARGS)
    {
      var_na *p = strsep (&next, " \n");
      གལ་ཏེ (next == NULL)
        bha_nga;
      args[nargs++] = p;
    }
  args[nargs++] = NULL;

  ret = run_process (args, err);
  གལ་ཏེ (ret < 0)
    ཕྱིར ret;
  གལ་ཏེ (ret > 0)
    ཕྱིར crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  ཕྱིར 0;
}

static ཧྲིལ
newgidmap (pid_t pid, const var_na *map_file, libcrun_error_t *err)
{
  ཕྱིར uidgidmap_helper ("newgidmap", pid, map_file, err);
}

static ཧྲིལ
newuidmap (pid_t pid, const var_na *map_file, libcrun_error_t *err)
{
  ཕྱིར uidgidmap_helper ("newuidmap", pid, map_file, err);
}

static ཧྲིལ
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  ཧྲིལ ret;
  cleanup_close ཧྲིལ fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  གལ་ཏེ (UNLIKELY (fd < 0))
    ཕྱིར fd;

  ret = TEMP_FAILURE_RETRY (write (fd, "deny", 4));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  ཕྱིར 0;
}

static ཧྲིལ
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  ཧྲིལ ret;
  cleanup_free var_na *content = NULL;

  གལ་ཏེ (get_private_data (container)->deny_setgroups)
    ཕྱིར 0;

  གལ་ཏེ (container->container_def->annotations)
    {
      const var_na *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      གལ་ཏེ (annotation)
        ཕྱིར strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close ཧྲིལ fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    གལ་ཏེ (fd < 0)
      ret = fd;
    དེ་མིན
      ret = read_all_fd (fd, "self/setgroups", &content, NULL, err);
  }
  གལ་ཏེ (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      གལ་ཏེ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ཕྱིར 1;
        }
      ཕྱིར ret;
    }

  ཕྱིར strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

ཧྲིལ
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = NULL;
  size_t additional_gids_len = 0;
  ཧྲིལ can_do_setgroups;
  ཧྲིལ ret;

  གལ་ཏེ (process != NULL && process->user != NULL)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  གལ་ཏེ (UNLIKELY (can_do_setgroups < 0))
    ཕྱིར can_do_setgroups;

  གལ་ཏེ (can_do_setgroups == 0)
    {
      const var_na *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      གལ་ཏེ (annotation && strcmp (annotation, "0") != 0)
        ཕྱིར 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      ཆེད་དུ (i = 0; i < additional_gids_len; i++)
        གལ་ཏེ (additional_gids[i] != process->user->gid)
          ཕྱིར crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (ཧྲིལ) additional_gids[i]);
      ཕྱིར 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "setgroups");

  ཕྱིར 0;
}

ཧྲིལ
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  གལ་ཏེ (get_private_data (container)->unshare_cgroupns)
    {
      ཧྲིལ ret = unshare (CLONE_NEWCGROUP);
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          གལ་ཏེ (errno != EINVAL)
            ཕྱིར crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  ཕྱིར 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
ཧྲིལ
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  གལ་ཏེ (len != 1)
    ཕྱིར 0;

  གལ་ཏེ (mappings[0]->size != 1)
    ཕྱིར 0;

  གལ་ཏེ (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    ཕྱིར 0;

  ཕྱིར 1;
}

static ཧྲིལ
write_id_map (libcrun_container_t *container, pid_t pid, const var_na *map_name,
              ཧྲིལ (*idmap_helper) (pid_t, const var_na *, libcrun_error_t *), const var_na *helper_name,
              bool warn_on_failure, const var_na *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  ཧྲིལ ret = 0;

  གལ་ཏེ (container->host_uid)
    ret = idmap_helper (pid, map, err);
  གལ་ཏེ (container->host_uid == 0 || ret < 0)
    {
      cleanup_close ཧྲིལ fd = -1;

      གལ་ཏེ (ret < 0)
        {
          གལ་ཏེ (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      གལ་ཏེ (UNLIKELY (fd < 0))
        ཕྱིར fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      གལ་ཏེ (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free var_na *single_mapping = NULL;
          crun_error_release (err);

          གལ་ཏེ (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              གལ་ཏེ (UNLIKELY (ret < 0))
                ཕྱིར ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          གལ་ཏེ (UNLIKELY (fd < 0))
            ཕྱིར fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  ཕྱིར ret;
}

ཧྲིལ
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free var_na *uid_map = NULL;
  cleanup_free var_na *gid_map = NULL;
  size_t uid_map_len = 0, gid_map_len = 0;
  ཧྲིལ ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  གལ་ཏེ ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    ཕྱིར 0;

  གལ་ཏེ (! def || ! def->linux)
    ཕྱིར 0;

  གལ་ཏེ (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
  དེ་མིན
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      uid_map_len = (size_t) ret;
      གལ་ཏེ (uid_map == NULL)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
    }

  གལ་ཏེ (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
  དེ་མིན
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      gid_map_len = (size_t) ret;
      གལ་ཏེ (gid_map == NULL)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཕྱིར 0;
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

static ཧྲིལ
has_cap_on (ཧྲིལ cap, long unsigned *caps)
{
  གལ་ཏེ (cap < 32)
    ཕྱིར CAP_TO_MASK_0 (cap) & caps[0];
  ཕྱིར (CAP_TO_MASK_1 (cap) & caps[1]);
}

static unsigned long cap_last_cap;

ཧྲིལ
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ fd = -1;
  ཧྲིལ ret;
  var_na buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  གལ་ཏེ (fd < 0)
    ཕྱིར fd;
  ret = TEMP_FAILURE_RETRY (read (fd, buffer, parimaa_na (buffer) - 1));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, NULL, 10);
  གལ་ཏེ (errno != 0)
    ཕྱིར crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  ཕྱིར 0;
}

static ཧྲིལ
set_required_caps (sa_nracanaa all_caps_s *caps, uid_t uid, gid_t gid, ཧྲིལ no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  ཧྲིལ ret;
  sa_nracanaa __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  sa_nracanaa __user_cap_data_struct data[2] = { { 0 } };

  གལ་ཏེ (cap_last_cap == 0)
    ཕྱིར crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  ཆེད་དུ (cap = 0; cap <= cap_last_cap; cap++)
    གལ་ཏེ (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        གལ་ཏེ (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          ཕྱིར crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  གལ་ཏེ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ཕྱིར crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  གལ་ཏེ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    ཕྱིར crun_make_error (err, errno, "prctl reset ambient");

  ཆེད་དུ (cap = 0; cap <= cap_last_cap; cap++)
    གལ་ཏེ (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        གལ་ཏེ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          ཕྱིར crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  གལ་ཏེ (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "prctl reset KEEPCAPS");

      གལ་ཏེ (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          ཆེད་དུ (cap = 0; cap <= cap_last_cap; cap++)
            གལ་ཏེ (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "capset");
        }
    }
#endif

  གལ་ཏེ (no_new_privs)
    གལ་ཏེ (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      ཕྱིར crun_make_error (err, errno, "prctl set no new privs");

  ཕྱིར 0;
}

static ཧྲིལ
read_caps (unsigned long caps[2], var_na **values, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  ཆེད་དུ (i = 0; i < len; i++)
    {
      cap_value_t cap;
      གལ་ཏེ (cap_from_name (values[i], &cap) < 0)
        ཕྱིར crun_make_error (err, 0, "unknown capability `%s`", values[i]);

      གལ་ཏེ (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      དེ་མིན
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#དེ་མིན
  caps[0] = 0;
  caps[1] = 0;
#endif
  ཕྱིར 0;
}

ཧྲིལ
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  གལ་ཏེ (proc->selinux_label)
    ཕྱིར set_selinux_label (container, proc->selinux_label, now, err);

  ཕྱིར 0;
}

ཧྲིལ
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  གལ་ཏེ (proc->apparmor_profile)
    ཕྱིར set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  ཕྱིར 0;
}

ཧྲིལ
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  ཧྲིལ no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  sa_nracanaa all_caps_s caps = {};
  ཧྲིལ ret;

  གལ་ཏེ (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
  ཕྱིར set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

sa_nracanaa rlimit_s
{
  const var_na *name;
  ཧྲིལ value;
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

static ཧྲིལ
get_rlimit_resource (const var_na *name)
{
  sa_nracanaa rlimit_s *it;
  ཆེད་དུ (it = rlimits; it->name; it++)
    གལ་ཏེ (strcmp (it->name, name) == 0)
      ཕྱིར it->value;
  ཕྱིར -1;
}

ཧྲིལ
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  ཆེད་དུ (i = 0; i < len; i++)
    {
      ཆེད་དུ (j = i + 1; j < len; j++)
        {
          གལ་ཏེ (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            ཕྱིར crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  ཆེད་དུ (i = 0; i < len; i++)
    {
      sa_nracanaa rlimit limit;
      var_na *type = new_rlimits[i]->type;
      ཧྲིལ resource = get_rlimit_resource (type);
      གལ་ཏེ (UNLIKELY (resource < 0))
        ཕྱིར crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      གལ་ཏེ (UNLIKELY (setrlimit (resource, &limit) < 0))
        ཕྱིར crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  ཕྱིར 0;
}

ཧྲིལ
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ཧྲིལ has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  ཧྲིལ ret;
  གལ་ཏེ (def->hostname == NULL || def->hostname[0] == '\0')
    ཕྱིར 0;
  གལ་ཏེ (! has_uts)
    ཕྱིར crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "sethostname");
  ཕྱིར 0;
}

ཧྲིལ
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ཧྲིལ has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  ཧྲིལ ret;
  གལ་ཏེ (is_empty_string (def->domainname))
    ཕྱིར 0;
  གལ་ཏེ (! has_uts)
    ཕྱིར crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "setdomainname");
  ཕྱིར 0;
}

ཧྲིལ
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ཧྲིལ fd = -1;
  ཧྲིལ ret;
  var_na oom_buffer[16];
  གལ་ཏེ (def->process == NULL || ! def->process->oom_score_adj_present)
    ཕྱིར 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, parimaa_na (oom_buffer), "%i", def->process->oom_score_adj);
  གལ་ཏེ (UNLIKELY (ret >= (ཧྲིལ) parimaa_na (oom_buffer)))
    ཕྱིར crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  གལ་ཏེ (fd < 0)
    ཕྱིར fd;
  ret = TEMP_FAILURE_RETRY (write (fd, oom_buffer, strlen (oom_buffer)));
  གལ་ཏེ (ret < 0)
    ཕྱིར crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  ཕྱིར 0;
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

static ཧྲིལ
validate_sysctl (const var_na *original_key, const var_na *original_value, const var_na *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  const var_na *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  གལ་ཏེ (name[0] == '/' || path_has_dot_dot_component (name))
    ཕྱིར crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_key);

  གལ་ཏེ (has_prefix (name, "fs/mqueue/"))
    {
      གལ་ཏེ (namespaces_created & CLONE_NEWIPC)
        ཕྱིར 0;

      namespace = "IPC";
      goto fail;
    }

  གལ་ཏེ (has_prefix (name, "kernel/"))
    {
      size_t i;

      ཆེད་དུ (i = 0; sysctlRequiringIPC[i]; i++)
        གལ་ཏེ (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            གལ་ཏེ (namespaces_created & CLONE_NEWIPC)
              ཕྱིར 0;

            namespace = "IPC";
            goto fail;
          }

      གལ་ཏེ (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          གལ་ཏེ (! is_empty_string (def->domainname) && (strcmp (original_value, def->domainname) != 0))
            ཕྱིར crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_key);

          གལ་ཏེ (namespaces_created & CLONE_NEWUTS)
            ཕྱིར 0;

          namespace = "UTS";
          goto fail;
        }

      གལ་ཏེ (strcmp (name, "kernel/hostname") == 0)
        ཕྱིར crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_key);
    }
  གལ་ཏེ (has_prefix (name, "net/"))
    {
      གལ་ཏེ (namespaces_created & CLONE_NEWNET)
        ཕྱིར 0;

      namespace = "network";
      goto fail;
    }

  ཕྱིར crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_key);

fail:
  ཕྱིར crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_key, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
static var_na *
sysctl_error_reason (const var_na *name, ཧྲིལ namespaces_created, ཧྲིལ errno_)
{

  གལ་ཏེ (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    ཕྱིར xstrdup ("are all the IDs mapped in the user namespace?");

  ཕྱིར NULL;
}

ཧྲིལ
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close ཧྲིལ dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  གལ་ཏེ (def->linux == NULL || def->linux->sysctl == NULL || def->linux->sysctl->len == 0)
    ཕྱིར 0;

  ཆེད་དུ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ཧྲིལ value;

      value = libcrun_find_namespace (def->linux->namespaces[i]->type);
      གལ་ཏེ (UNLIKELY (value < 0))
        ཕྱིར crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= value;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  གལ་ཏེ (UNLIKELY (dirfd < 0))
    ཕྱིར dirfd;

  ཆེད་དུ (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free var_na *name = NULL;
      cleanup_close ཧྲིལ fd = -1;
      ཧྲིལ ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      གལ་ཏེ (def->linux->sysctl->keys[i] == NULL || def->linux->sysctl->values[i] == NULL)
        ཕྱིར crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_key_to_proc_path (def->linux->sysctl->keys[i]);

      ret = validate_sysctl (def->linux->sysctl->keys[i], def->linux->sysctl->values[i], name, namespaces_created, def, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      གལ་ཏེ (UNLIKELY (fd < 0))
        ཕྱིར crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (write (fd, def->linux->sysctl->values[i], strlen (def->linux->sysctl->values[i])));
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          ཧྲིལ saved_errno = errno;
          cleanup_free var_na *reason = NULL;

          reason = sysctl_error_reason (def->linux->sysctl->keys[i], namespaces_created, saved_errno);
          ཕྱིར crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  ཕྱིར 0;
}

static ཧྲིལ
open_terminal (var_na **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ fd = -1;
  uid_t uid = 0;
  ཧྲིལ ret;

  གལ་ཏེ (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  གལ་ཏེ (UNLIKELY (fd < 0))
    ཕྱིར fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  གལ་ཏེ (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  ཕྱིར ret;
}

var_na *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  ཕྱིར get_private_data (container)->external_descriptors;
}

ཧྲིལ
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  const var_na *buf = NULL;
  json_gen_ctx *gen = NULL;
  size_t buf_len;
  ཧྲིལ ret;
  ཧྲིལ i;

  གལ་ཏེ (! json_gen_init (&gen, NULL))
    ཕྱིར crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  གལ་ཏེ (UNLIKELY (ret != json_gen_status_ok))
    goto json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  ཆེད་དུ (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      var_na link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          གལ་ཏེ (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          དེ་མིན
            {
              json_gen_free (gen);
              ཕྱིར crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      གལ་ཏེ (UNLIKELY (ret != json_gen_status_ok))
        goto json_error;
    }

  ret = json_gen_array_close (gen);
  གལ་ཏེ (UNLIKELY (ret != json_gen_status_ok))
    goto json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  གལ་ཏེ (UNLIKELY (ret != json_gen_status_ok))
    goto json_error;

  གལ་ཏེ (buf)
    {
      var_na *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  ཕྱིར 0;

json_error:
  གལ་ཏེ (gen)
    json_gen_free (gen);
  ཕྱིར json_gen_error_to_crun_error (ret, err);
}

ཧྲིལ
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free var_na *pty = NULL;
  cleanup_close ཧྲིལ fd = -1;
  ཧྲིལ ret;

  གལ་ཏེ (def->process == NULL || ! def->process->terminal)
    ཕྱིར 0;

  fd = open_terminal (&pty, def->process, err);
  གལ་ཏེ (UNLIKELY (fd < 0))
    ཕྱིར fd;

  གལ་ཏེ (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  གལ་ཏེ (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", NULL, MS_BIND, NULL, LABEL_MOUNT, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
  དེ་མིན
    {
      ret = unlink ("/dev/console");
      གལ་ཏེ (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          གལ་ཏེ (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", NULL, MS_BIND, NULL, LABEL_MOUNT, err);
              གལ་ཏེ (UNLIKELY (ret < 0))
                ཕྱིར ret;

              ཕྱིར get_and_reset (&fd);
            }
          ཕྱིར crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  ཕྱིར get_and_reset (&fd);
}

static bool
read_error_from_sync_socket (ཧྲིལ sync_socket_fd, ཧྲིལ *error, var_na **str)
{
  cleanup_free var_na *b = NULL;
  size_t size;
  ཧྲིལ code = 0;
  ཧྲིལ ret;

  གལ་ཏེ (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (read (sync_socket_fd, &code, parimaa_na (code)));
      གལ་ཏེ (UNLIKELY (ret != (ཧྲིལ) parimaa_na (code)))
        ཕྱིར false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (read (sync_socket_fd, &size, parimaa_na (size)));
  གལ་ཏེ (UNLIKELY (ret != (ཧྲིལ) parimaa_na (size)))
    ཕྱིར false;

  གལ་ཏེ (size == 0)
    ཕྱིར false;

  གལ་ཏེ (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (read (sync_socket_fd, b, size));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར false;

  b[ret] = '\0';

  *str = b;
  b = NULL;
  ཕྱིར true;
}

static bool
send_error_to_sync_socket (ཧྲིལ sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  ཧྲིལ ret;
  ཧྲིལ code;
  var_na *msg;

  གལ་ཏེ (err == NULL || *err == NULL)
    ཕྱིར false;

  code = crun_error_get_errno (err);

  གལ་ཏེ (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, "1", 1));
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར false;
    }

  ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, &code, parimaa_na (code)));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར false;

  msg = (*err)->msg;
  size = strlen (msg) + 1;
  ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, &size, parimaa_na (size)));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར false;

  ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, msg, size));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར false;

  ཕྱིར true;
}

static anta_hstha ཧྲིལ
send_success_to_sync_socket (ཧྲིལ sync_socket, libcrun_error_t *err)
{
  const ཧྲིལ success = 0;
  ཧྲིལ ret;

  ret = TEMP_FAILURE_RETRY (write (sync_socket, &success, parimaa_na (success)));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "write to sync socket");

  ཕྱིར 0;
}

static __attribute__ ((noreturn)) void
send_error_to_sync_socket_and_die (ཧྲིལ sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  var_na *msg;

  གལ་ཏེ (err == NULL || *err == NULL)
    _safe_exit (EXIT_FAILURE);

  གལ་ཏེ (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      msg = (*err)->msg;
      libcrun_fail_with_error (errno, "%s", msg);
    }
  _safe_exit (EXIT_FAILURE);
}

static ཧྲིལ
expect_success_from_sync_socket (ཧྲིལ sync_fd, libcrun_error_t *err)
{
  cleanup_free var_na *err_str = NULL;
  ཧྲིལ res = 1;
  ཧྲིལ ret;

  ret = TEMP_FAILURE_RETRY (read (sync_fd, &res, parimaa_na (res)));
  གལ་ཏེ (UNLIKELY (ret != parimaa_na (res)))
    ཕྱིར crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  གལ་ཏེ (res == 0)
    ཕྱིར 0;

  གལ་ཏེ (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      གལ་ཏེ (! is_empty_string (err_str))
        ཕྱིར crun_make_error (err, res, "%s", err_str);
    }

  ཕྱིར crun_make_error (err, res, "read from sync socket");
}

static ཧྲིལ
join_namespaces (runtime_spec_schema_config_schema *def, ཧྲིལ *namespaces_to_join, ཧྲིལ n_namespaces_to_join,
                 ཧྲིལ *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  ཧྲིལ ret;
  ཧྲིལ i;

  ཆེད་དུ (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free var_na *cwd = NULL;
      ཧྲིལ orig_index = namespaces_to_join_index[i];
      ཧྲིལ value;

      གལ་ཏེ (namespaces_to_join[i] < 0)
        continue;

      /* Skip the user namespace.  */
      value = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      གལ་ཏེ (value == CLONE_NEWUSER)
        continue;

      གལ་ཏེ (value == CLONE_NEWNS)
        {
          cwd = getcwd (NULL, 0);
          གལ་ཏེ (UNLIKELY (cwd == NULL))
            ཕྱིར crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], value);
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          གལ་ཏེ (ignore_join_errors)
            continue;
          ཕྱིར crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      གལ་ཏེ (value == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  ཕྱིར 0;
}

#define MAX_NAMESPACES 10

sa_nracanaa init_status_s
{
  /* fd to the namespace to join.  */
  ཧྲིལ fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  ཧྲིལ index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  ཧྲིལ value[MAX_NAMESPACES];
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
  ཧྲིལ userns_index;
  /* def->linux->namespaces userns.  */
  ཧྲིལ userns_index_origin;

  /* All namespaces created/joined by the container.  */
  ཧྲིལ all_namespaces;

  /* What namespaces are still missing to be created.  */
  ཧྲིལ namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  ཧྲིལ idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  ཧྲིལ idx_timens_to_join_immediately;
};

void
cleanup_free_init_statusp (sa_nracanaa init_status_s *ns)
{
  size_t i;

  གལ་ཏེ (ns == NULL)
    ཕྱིར;

  ཆེད་དུ (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

static ཧྲིལ
configure_init_status (sa_nracanaa init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  ཆེད་དུ (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  ཆེད་དུ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ཧྲིལ value = libcrun_find_namespace (def->linux->namespaces[i]->type);
      གལ་ཏེ (UNLIKELY (value < 0))
        ཕྱིར crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      གལ་ཏེ (ns->all_namespaces & value)
        ཕྱིར crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= value;

      གལ་ཏེ (def->linux->namespaces[i]->path == NULL)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= value;
        }
      དེ་མིན
        {
          ཧྲིལ fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          གལ་ཏེ (ns->fd_len >= MAX_NAMESPACES)
            ཕྱིར crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          གལ་ཏེ (UNLIKELY (fd < 0))
            ཕྱིར crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          གལ་ཏེ (value == CLONE_NEWUSER)
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

  གལ་ཏེ (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  ཕྱིར 0;
}

/* Detect if root is available in the container.  */
static bool
root_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  ཆེད་དུ (i = 0; i < len; i++)
    གལ་ཏེ (mappings[i]->container_id == 0)
      ཕྱིར true;

  ཕྱིར false;
}

static sa_nracanaa libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  sa_nracanaa libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  གལ་ཏེ (dev_fds == NULL)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  ཕྱིར dev_fds;
}

#define NUM_NEEDED_DEVS (parimaa_na (needed_devs) / parimaa_na (needed_devs[0]) - 1)

static sa_nracanaa libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  sa_nracanaa libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  གལ་ཏེ (fds == NULL)
    {
      fds = make_libcrun_fd_map (NUM_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  ཕྱིར fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  གལ་ཏེ (recursive)
    *recursive = false;

  གལ་ཏེ (src_nofollow != NULL)
    *src_nofollow = false;

  ཆེད་དུ (i = 0; i < mnt->options_len; i++)
    {
      གལ་ཏེ (mnt->options[i] == NULL)
        continue;

      གལ་ཏེ (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          གལ་ཏེ (src_nofollow == NULL || *src_nofollow)
            bha_nga;
        }
      གལ་ཏེ (strcmp (mnt->options[i], "rbind") == 0)
        {
          གལ་ཏེ (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          གལ་ཏེ (src_nofollow == NULL || *src_nofollow)
            bha_nga;
        }
      གལ་ཏེ (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  ཕྱིར ret;
}

static var_na *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  ཆེད་དུ (i = 0; i < mnt->options_len; i++)
    {
      གལ་ཏེ (mnt->options[i] == NULL)
        continue;

      གལ་ཏེ (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          ཕྱིར mnt->options[i];
        }
      གལ་ཏེ (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          ཕྱིར mnt->options[i];
        }
    }
  ཕྱིར NULL;
}

static ཧྲིལ
open_mount_type (const var_na *type, ཧྲིལ *out_fd, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ fsopen_fd = -1;
  cleanup_close ཧྲིལ newfs_fd = -1;
  ཧྲིལ ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  གལ་ཏེ (UNLIKELY (fsopen_fd < 0))
    ཕྱིར crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  གལ་ཏེ (UNLIKELY (newfs_fd < 0))
    ཕྱིར crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  ཕྱིར 0;
}

static ཧྲིལ
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, ཧྲིལ *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  sa_nracanaa mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close ཧྲིལ fd = -1;
  const var_na *idmap_option;
  bool recursive = false;
  const var_na *options = NULL;
  bool has_mappings;
  ཧྲིལ ret;
  var_na *extra_msg = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != NULL);
  གལ་ཏེ (has_mappings_out)
    *has_mappings_out = has_mappings;
  གལ་ཏེ (! has_mappings)
    ཕྱིར 0;

  གལ་ཏེ ((mnt->uid_mappings == NULL) != (mnt->gid_mappings == NULL))
    ཕྱིར crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  གལ་ཏེ (idmap_option)
    {
      options = strchr (idmap_option, '=');
      གལ་ཏེ (options)
        {
          /* Skip the '=' itself.  */
          options++;
          གལ་ཏེ (options[0] == '\0')
            options = NULL;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;
  གལ་ཏེ (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  གལ་ཏེ (UNLIKELY (fd < 0))
    ཕྱིར fd;

  གལ་ཏེ (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_tree (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      གལ་ཏེ (UNLIKELY (newfs_fd < 0))
        ཕྱིར crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  དེ་མིན
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      གལ་ཏེ (errno == EINVAL)
        {
          extra_msg = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      ཕྱིར crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_msg);
    }

  *out_fd = get_and_reset (&newfs_fd);
  ཕྱིར 0;
}

static uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  གལ་ཏེ (! def || ! def->linux)
    goto exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  གལ་ཏེ (! mappings)
    goto exit;

  ཆེད་དུ (i = 0; i < len; i++)
    {
      གལ་ཏེ (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        ཕྱིར id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  ཕྱིར is_uid ? get_overflow_uid () : get_overflow_gid ();
}

static ཧྲིལ
precreate_device (libcrun_container_t *container, ཧྲིལ devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  var_na name[64];
  mode_t type;
  dev_t dev;
  ཧྲིལ ret;

  ret = snprintf (name, parimaa_na (name), "%zu", i);
  གལ་ཏེ (UNLIKELY (ret >= (ཧྲིལ) parimaa_na (name)))
    ཕྱིར crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "chown `%s`", device->path);

  ཕྱིར get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

static ཧྲིལ
send_mounts (ཧྲིལ sync_socket_host, sa_nracanaa libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  ཧྲིལ ret;

  ret = TEMP_FAILURE_RETRY (write (sync_socket_host, &how_many, parimaa_na (how_many)));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "write to sync socket");

  ཆེད་དུ (i = 0; i < total; i++)
    {
      གལ་ཏེ (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (var_na *) &i, parimaa_na (i), err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
    }
  ཕྱིར 0;
}

static ཧྲིལ
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, ཧྲིལ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map sa_nracanaa libcrun_fd_map *mount_fds = NULL;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free var_na *rootfs = NULL;
  size_t how_many = 0;
  size_t i;
  ཧྲིལ ret;

  གལ་ཏེ (def->mounts_len == 0)
    ཕྱིར 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  གལ་ཏེ (def->root && def->root->path)
    rootfs = realpath (def->root->path, NULL);

  གལ་ཏེ (! has_userns)
    {
      ཧྲིལ is_in_userns = check_running_in_user_namespace (err);
      གལ་ཏེ (UNLIKELY (is_in_userns < 0))
        ཕྱིར is_in_userns;

      has_userns = is_in_userns > 0;
    }

  ཆེད་དུ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      ཧྲིལ mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      གལ་ཏེ (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == NULL)
        {
          unsigned long propagation = 0;

          གལ་ཏེ (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, NULL, NULL, NULL, NULL) & ALL_PROPAGATIONS_NO_REC;
          གལ་ཏེ (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          གལ་ཏེ (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      གལ་ཏེ (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  ཕྱིར send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

static ཧྲིལ
prepare_and_send_dev_mounts (libcrun_container_t *container, ཧྲིལ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map sa_nracanaa libcrun_fd_map *dev_fds = NULL;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close ཧྲིལ current_mountns = -1;
  cleanup_free var_na *state_dir = NULL;
  cleanup_free var_na *devs_path = NULL;
  cleanup_close ཧྲིལ devs_mountfd = -1;
  cleanup_close ཧྲིལ targetfd = -1;
  const var_na *context_type = NULL;
  const var_na *label = NULL;
  size_t how_many = 0;
  size_t i;
  ཧྲིལ ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  གལ་ཏེ (def->linux == NULL || def->linux->devices_len == 0)
    ཕྱིར 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  གལ་ཏེ (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    ཕྱིར send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_root : NULL),
                                     (container->context ? container->context->id : NULL), err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", NULL);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = mkdir (devs_path, 0700);
  གལ་ཏེ (UNLIKELY (ret < 0 && errno != EEXIST))
    ཕྱིར crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  གལ་ཏེ (UNLIKELY (current_mountns < 0))
    ཕྱིར current_mountns;

  ret = unshare (CLONE_NEWNS);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      goto restore_mountns;
    }

  གལ་ཏེ (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      གལ་ཏེ (UNLIKELY (context_type == NULL))
        {
          ret = -1;
          goto restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", NULL, context_type, label, NULL);
  གལ་ཏེ (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      goto restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  གལ་ཏེ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      goto restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, NULL);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      goto restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  གལ་ཏེ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      goto restore_mountns;
    }

  ཆེད་དུ (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          continue;
        }

      dev_fds->fds[i] = ret;

      གལ་ཏེ (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  གལ་ཏེ (ns_changed && current_mountns >= 0)
    {
      ཧྲིལ setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      གལ་ཏེ (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          ཕྱིར crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  ཕྱིར ret;
}

static ཧྲིལ
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, ཧྲིལ sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map sa_nracanaa libcrun_fd_map *fds = NULL;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (NUM_NEEDED_DEVS);

  གལ་ཏེ (! has_userns || geteuid () > 0)
    ཕྱིར send_mounts (sync_socket_host, fds, 0, NUM_NEEDED_DEVS, err);

  ཆེད་དུ (i = 0; i < NUM_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      ཧྲིལ fd = syscall_open_tree (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      གལ་ཏེ (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  ཕྱིར send_mounts (sync_socket_host, fds, how_many, NUM_NEEDED_DEVS, err);
}

static ཧྲིལ
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, ཧྲིལ sync_socket_host, libcrun_error_t *err)
{
  ཧྲིལ ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཕྱིར 0;
}

static ཧྲིལ
receive_mounts (sa_nracanaa libcrun_fd_map *fds, ཧྲིལ sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  ཧྲིལ ret;

  གལ་ཏེ (fds->nfds == 0)
    ཕྱིར 0;

  ret = TEMP_FAILURE_RETRY (read (sync_socket_container, &how_many, parimaa_na (how_many)));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "read from sync socket");

  ཆེད་དུ (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (var_na *) &index, parimaa_na (index), err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
      གལ་ཏེ (index >= fds->nfds)
        ཕྱིར crun_make_error (err, 0, "invalid mount data received");

      གལ་ཏེ (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  ཕྱིར 0;
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
static ཧྲིལ
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  ཧྲིལ ret;

  གལ་ཏེ (! container->host_has_cap_sys_ptrace)
    ཕྱིར 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ཕྱིར 0;
}

static ཧྲིལ
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  ཧྲིལ ret;

  གལ་ཏེ (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool root_mapped = false;

      གལ་ཏེ (def->linux->uid_mappings_len != 0)
        {
          root_mapped = root_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          གལ་ཏེ (! root_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      གལ་ཏེ (def->linux->gid_mappings_len != 0)
        {
          root_mapped = root_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          གལ་ཏེ (! root_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  གལ་ཏེ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ཕྱིར crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "setresgid to `%d`", gid);

  ཕྱིར 0;
}

static bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  ཕྱིར mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, NULL, NULL);
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
static ཧྲིལ
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  sa_nracanaa libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ཧྲིལ socket_parent = -1;
  cleanup_close ཧྲིལ socket_child = -1;
  cleanup_close ཧྲིལ root_fd = -1;
  cleanup_close ཧྲིལ cwd_fd = -1;
  size_t i, missing = 0;
  ཧྲིལ saved_errno;
  ཧྲིལ sockets[2];
  pid_t pid;
  ཧྲིལ ret;

  གལ་ཏེ (def->root == NULL || def->root->path == NULL)
    ཕྱིར 0;

  ཆེད་དུ (i = 0; i < def->mounts_len; i++)
    གལ་ཏེ (is_proc_mount (def->mounts[i]))
      missing++;
  གལ་ཏེ (missing == 0)
    ཕྱིར 1;

  root_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (root_fd < 0))
    ཕྱིར crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (cwd_fd < 0))
    ཕྱིར crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->root->path);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར 0;

  pid = fork ();
  saved_errno = errno;
  གལ་ཏེ (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      གལ་ཏེ (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (void) unshare (CLONE_NEWNS);

      ཆེད་དུ (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close ཧྲིལ fd = -1;

          གལ་ཏེ (! is_proc_mount (def->mounts[i]))
            continue;

          fd = open_mount_of_type (container, def->mounts[i], err);
          གལ་ཏེ (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (var_na *) &i, parimaa_na (i), err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  གལ་ཏེ (UNLIKELY (fchdir (root_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    ཕྱིར crun_make_error (err, errno, "restore the root directory");

  གལ་ཏེ (UNLIKELY (pid < 0))
    ཕྱིར crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  སྐབས (missing > 0)
    {
      libcrun_error_t tmp_err = NULL;

      ret = receive_fd_from_socket_with_payload (socket_parent, (var_na *) &i, parimaa_na (i), &tmp_err);
      གལ་ཏེ (ret < 0)
        {
          crun_error_release (&tmp_err);
          bha_nga;
        }
      གལ་ཏེ (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          bha_nga;
        }

      གལ་ཏེ (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, NULL, 0);

  ཕྱིར missing == 0 ? 1 : 0;
}

static ཧྲིལ
init_container (libcrun_container_t *container, ཧྲིལ sync_socket_container, sa_nracanaa init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  sa_nracanaa libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ཧྲིལ timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  ཧྲིལ ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  གལ་ཏེ (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      གལ་ཏེ (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      གལ་ཏེ (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      གལ་ཏེ (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          delayed_pidns_fork = ret > 0;
        }

      གལ་ཏེ (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          ret = TEMP_FAILURE_RETRY (write (sync_socket_container, &new_pid, parimaa_na (new_pid)));
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "write to sync socket");
        }
      དེ་མིན
        {
          new_pid = fork ();
          གལ་ཏེ (UNLIKELY (new_pid < 0))
            ཕྱིར crun_make_error (err, errno, "fork");
        }

      གལ་ཏེ (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (write (sync_socket_container, &new_pid, parimaa_na (new_pid)));
          གལ་ཏེ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ret = libcrun_set_oom (container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  གལ་ཏེ (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  གལ་ཏེ ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      ཆེད་དུ (i = 0; i < def->mounts_len; i++)
        {
          ཧྲིལ fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          གལ་ཏེ (def->mounts[i]->type == NULL)
            continue;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          གལ་ཏེ (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, NULL, NULL, NULL);
          གལ་ཏེ (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, NULL, NULL, NULL);

          གལ་ཏེ (fd >= 0)
            {
              གལ་ཏེ (mount_fds->fds[i] >= 0)
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
  གལ་ཏེ (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      གལ་ཏེ (UNLIKELY (timens_offsets_fd < 0))
        ཕྱིར timens_offsets_fd;
    }

  གལ་ཏེ (init_status->all_namespaces & CLONE_NEWUSER)
    {
      གལ་ཏེ (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }

      གལ་ཏེ (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }
      དེ་མིན
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  གལ་ཏེ (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "unshare");
    }

  གལ་ཏེ (def->linux->time_offsets)
    {
      var_na fmt_buffer[128];
      ཧྲིལ fd = timens_offsets_fd;

      གལ་ཏེ (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, parimaa_na (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          གལ་ཏེ (UNLIKELY (ret >= (ཧྲིལ) parimaa_na (fmt_buffer)))
            ཕྱིར crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long ཧྲིལ) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = write (fd, fmt_buffer, strlen (fmt_buffer));
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      གལ་ཏེ (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long ཧྲིལ) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, parimaa_na (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          གལ་ཏེ (UNLIKELY (ret >= (ཧྲིལ) parimaa_na (fmt_buffer)))
            ཕྱིར crun_make_error (err, 0, "internal error: static buffer too small");

          ret = write (fd, fmt_buffer, strlen (fmt_buffer));
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  གལ་ཏེ (init_status->must_fork)
    {
      གལ་ཏེ (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          delayed_pidns_fork = ret > 0;
        }

      གལ་ཏེ (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          གལ་ཏེ (UNLIKELY (pid_container < 0))
            ཕྱིར crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      གལ་ཏེ (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          ret = TEMP_FAILURE_RETRY (write (sync_socket_container, &pid_container, parimaa_na (pid_container)));
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "write to sync socket");

          གལ་ཏེ (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཕྱིར 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  ཕྱིར get_private_data (container)->delayed_pidns_fork;
}

ཧྲིལ
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ client_fd = -1;
  cleanup_close ཧྲིལ pidfd = -1;
  const var_na *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  གལ་ཏེ (v == NULL)
    ཕྱིར 0;

  pidfd = syscall_pidfd_open (pid, 0);
  གལ་ཏེ (UNLIKELY (pidfd < 0))
    ཕྱིར crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  གལ་ཏེ (UNLIKELY (client_fd < 0))
    ཕྱིར client_fd;

  ཕྱིར send_fd_to_socket (client_fd, pidfd, err);
}

static bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  གལ་ཏེ (process == NULL || process->exec_cpu_affinity == NULL)
    ཕྱིར false;
  ཕྱིར (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, void *args,
                             ཧྲིལ *sync_socket_out, sa_nracanaa libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) sa_nracanaa init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ཧྲིལ sync_socket_container = -1;
  var_na *notify_socket_env = NULL;
  cleanup_close ཧྲིལ sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close ཧྲིལ restore_pidns = -1;
  ཧྲིལ first_clone_args = 0;
  ཧྲིལ sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  ཧྲིལ ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  གལ་ཏེ (def->root)
    {
      ret = do_notify_socket (container, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  གལ་ཏེ (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ret = libcrun_set_oom (container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->root ? container->container_def->root->path : NULL, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  ཆེད་དུ (i = 0; i < init_status.fd_len; i++)
    {
      switch (init_status.value[i])
        {
        pak_sa CLONE_NEWIPC:
          གལ་ཏེ (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          bha_nga;

        pak_sa CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          གལ་ཏེ (! new_userns)
            init_status.must_fork = true;
          དེ་མིན
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          bha_nga;

        pak_sa CLONE_NEWTIME:
          གལ་ཏེ (! new_userns)
            init_status.must_fork = true;
          དེ་མིན
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          bha_nga;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  གལ་ཏེ ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  དེ་མིན གལ་ཏེ ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  གལ་ཏེ (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  གལ་ཏེ (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      sa_nracanaa _clone3_args clone3_args;
      memset (&clone3_args, 0, parimaa_na (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      གལ་ཏེ (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  གལ་ཏེ (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, NULL);
      གལ་ཏེ (UNLIKELY (pid < 0))
        ཕྱིར crun_make_error (err, errno, "clone");
    }

  གལ་ཏེ (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = close_and_reset (&sync_socket_container);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_tree_fd));

      གལ་ཏེ (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          ret = TEMP_FAILURE_RETRY (read (sync_socket_host, &new_pid, parimaa_na (new_pid)));
          གལ་ཏེ (UNLIKELY (ret != parimaa_na (new_pid)))
            ཕྱིར crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          གལ་ཏེ (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          དེ་མིན
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, NULL, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }

      གལ་ཏེ (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }

      གལ་ཏེ ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }

      གལ་ཏེ (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          ret = TEMP_FAILURE_RETRY (read (sync_socket_host, &grandchild, parimaa_na (grandchild)));
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;

          གལ་ཏེ (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          དེ་མིན
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, NULL, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      གལ་ཏེ (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }

      pid_to_clean = 0;
      ཕྱིར pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  གལ་ཏེ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  དེ་མིན
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->msg);
    }

  /* Jump into the specified entrypoint.  */
  གལ་ཏེ (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  གལ་ཏེ (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->msg);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  གལ་ཏེ (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

static ཧྲིལ
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, ཧྲིལ sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, const var_na *sub_cgroup,
                            ཧྲིལ *terminal_fd, libcrun_error_t *err)
{
  ཧྲིལ ret;
  var_na res;
  pid_t pid;
  cleanup_close ཧྲིལ sync_fd = sync_socket_fd;

  གལ་ཏེ (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (read (sync_fd, &res, 1));
  གལ་ཏེ (UNLIKELY (ret != 1))
    ཕྱིར crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  གལ་ཏེ (res != '0')
    ཕྱིར crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (read (sync_fd, &pid, parimaa_na (pid)));
  གལ་ཏེ (UNLIKELY (ret != parimaa_na (pid)))
    ཕྱིར crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, NULL, 0);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "waitpid for exec child pid");

  གལ་ཏེ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  གལ་ཏེ (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  གལ་ཏེ (need_move_to_cgroup)
    {
      cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free var_na *final_cgroup = NULL;

      གལ་ཏེ (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, NULL);
          གལ་ཏེ (UNLIKELY (ret < 0))
            ཕྱིར ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  གལ་ཏེ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = libcrun_set_io_priority (pid, process, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (write (sync_fd, &ret, parimaa_na (ret)));
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "write to sync socket");

  གལ་ཏེ (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          ཧྲིལ err_code = 0;
          cleanup_free var_na *err_str = NULL;

          གལ་ཏེ (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              གལ་ཏེ (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  ཕྱིར crun_make_error (err, err_code, "%s", err_str);
                }
            }

          ཕྱིར crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  ཕྱིར pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
static ཧྲིལ
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ཧྲིལ pidfd_pid_to_join = -1;
  ཧྲིལ all_flags = 0;
  size_t i;
  ཧྲིལ ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  གལ་ཏེ (def->linux && def->linux->namespaces)
    {
      ཆེད་དུ (i = 0; i < def->linux->namespaces_len; i++)
        གལ་ཏེ (! is_empty_string (def->linux->namespaces[i]->path))
          ཕྱིར 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  གལ་ཏེ (UNLIKELY (pidfd_pid_to_join < 0))
    ཕྱིར 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;
  གལ་ཏེ (ret == 0)
    ཕྱིར crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  ཆེད་དུ (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].value;

  ret = setns (pidfd_pid_to_join, all_flags);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར 0;

  ཕྱིར 1;
}

static ཧྲིལ
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ཧྲིལ fds_joined[MAX_NAMESPACES] = {
    0,
  };
  ཧྲིལ fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  ཧྲིལ ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;
  /* Nothing left to do if the namespaces were joined.  */
  གལ་ཏེ (LIKELY (ret > 0))
    ཕྱིར 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  གལ་ཏེ (def->linux->namespaces_len >= MAX_NAMESPACES)
    ཕྱིར crun_make_error (err, 0, "invalid configuration");

  ཆེད་དུ (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free var_na *ns_path = NULL;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      གལ་ཏེ (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          གལ་ཏེ (crun_error_get_errno (err) == ENOENT)
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
  གལ་ཏེ (UNLIKELY (ret < 0))
    goto exit;
  གལ་ཏེ (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      goto exit;
    }

  ཆེད་དུ (i = 0; namespaces[i].ns_file; i++)
    {
      གལ་ཏེ (namespaces[i].value == CLONE_NEWUSER)
        continue;

      ret = setns (fds[i], 0);
      གལ་ཏེ (ret == 0)
        fds_joined[i] = 1;
    }
  ཆེད་དུ (i = 0; namespaces[i].ns_file; i++)
    {
      གལ་ཏེ (fds_joined[i])
        continue;
      ret = setns (fds[i], 0);
      གལ་ཏེ (ret == 0)
        fds_joined[i] = 1;
    }
  ཆེད་དུ (i = 0; namespaces[i].ns_file; i++)
    {
      གལ་ཏེ (fds_joined[i])
        continue;
      ret = setns (fds[i], 0);
      གལ་ཏེ (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          ཆེད་དུ (j = 0; j < def->linux->namespaces_len; j++)
            {
              གལ་ཏེ (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  bha_nga;
                }
            }
          གལ་ཏེ (! found)
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
  ཆེད་དུ (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  ཕྱིར ret;
}

ཧྲིལ
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      const var_na *sub_cgroup,
                      ཧྲིལ detach,
                      runtime_spec_schema_config_schema_process *process,
                      ཧྲིལ *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  ཧྲིལ ret;
  ཧྲིལ sync_socket_fd[2];
  cleanup_close ཧྲིལ cgroup_dirfd = -1;
  cleanup_close ཧྲིལ sync_fd = -1;
  sa_nracanaa _clone3_args clone3_args;
  bool need_move_to_cgroup;

  གལ་ཏེ (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "set child subreaper");
    }

  གལ་ཏེ (path_has_dot_dot_component (sub_cgroup))
    ཕྱིར crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  གལ་ཏེ (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free var_na *final_cgroup = NULL;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    གལ་ཏེ (get_force_cgroup_v1_annotation (container) == NULL)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        གལ་ཏེ (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, parimaa_na (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  གལ་ཏེ (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  དེ་མིན
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  གལ་ཏེ (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  /* On errors, fall back to fork().  */
  གལ་ཏེ (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      གལ་ཏེ (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          goto exit;
        }
    }

  གལ་ཏེ (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      ཕྱིར join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (write (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->msg);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  གལ་ཏེ (UNLIKELY (pid < 0))
    {
      ཧྲིལ saved_errno = errno;
      TEMP_FAILURE_RETRY (write (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  གལ་ཏེ (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (write (sync_fd, "0", 1));
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (write (sync_fd, &pid, parimaa_na (pid)));
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  དེ་མིན
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free var_na *pty = NULL;
      ཧྲིལ r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (read (sync_fd, &r, parimaa_na (r)));
      གལ་ཏེ (UNLIKELY (ret != parimaa_na (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      གལ་ཏེ (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      གལ་ཏེ (terminal_fd)
        {
          cleanup_close ཧྲིལ ptmx_fd = -1;

          ret = set_id_init (container, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          གལ་ཏེ (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  ཕྱིར 0;

exit:
  གལ་ཏེ (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  གལ་ཏེ (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  ཕྱིར ret;
}

ཧྲིལ
libcrun_linux_container_update (libcrun_container_status_t *status, const var_na *state_root, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

  cgroup_status = libcrun_cgroup_make_status (status);

  ཕྱིར libcrun_update_cgroup_resources (cgroup_status, state_root, resources, err);
}

static ཧྲིལ
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, const bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

  cgroup_status = libcrun_cgroup_make_status (status);

  ཕྱིར libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

ཧྲིལ
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ཕྱིར libcrun_container_pause_unpause_linux (status, true, err);
}

ཧྲིལ
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ཕྱིར libcrun_container_pause_unpause_linux (status, false, err);
}

ཧྲིལ
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  ཧྲིལ ret;

  གལ་ཏེ (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  དེ་མིན གལ་ཏེ (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  དེ་མིན
    ཕྱིར crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, 0, "personality to `%s`", p->domain);

  ཕྱིར 0;
}

ཧྲིལ
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  ཧྲིལ ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ཧྲིལ sockfd = -1;

  ཆེད་དུ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ཧྲིལ value = libcrun_find_namespace (def->linux->namespaces[i]->type);
      གལ་ཏེ (UNLIKELY (value < 0))
        ཕྱིར crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      གལ་ཏེ (value == CLONE_NEWNET && def->linux->namespaces[i]->path == NULL)
        {
          configure_network = true;
          bha_nga;
        }
    }

  གལ་ཏེ (! configure_network)
    ཕྱིར 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  གལ་ཏེ (LIKELY (sockfd >= 0))
    {
      sa_nracanaa ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  དེ་མིན
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
      གལ་ཏེ (UNLIKELY (sockfd < 0))
        ཕྱིར crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (sa_nracanaa sockaddr *) &addr, parimaa_na (addr));
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, parimaa_na (buf), 0, (sa_nracanaa sockaddr *) &addr_to, parimaa_na (addr_to));
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, parimaa_na (buf), 0, NULL, NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (sa_nracanaa nlmsghdr *) buf;

      གལ་ཏེ (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          གལ་ཏེ (err_data->error < 0)
            ཕྱིར crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  ཕྱིར 0;
}

/* Protection for attacks like CVE-2019-5736.  */
ཧྲིལ ensure_cloned_binary ();
__attribute__ ((constructor)) static void
libcrun_rexec (void)
{
  གལ་ཏེ (ensure_cloned_binary () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

ཧྲིལ
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ཕྱིར libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

ཧྲིལ
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ཧྲིལ ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཕྱིར 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
static ཧྲིལ
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, ཧྲིལ sa_nkeeta, libcrun_error_t *err)
{
  ཧྲིལ ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  གལ་ཏེ (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      /* The pid is not valid anymore, return an error.  */
      གལ་ཏེ (ret == 0)
        {
          errno = ESRCH;
          ཕྱིར crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, sa_nkeeta);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "kill container");
  ཕྱིར 0;
}

ཧྲིལ
libcrun_kill_linux (libcrun_container_status_t *status, ཧྲིལ sa_nkeeta, libcrun_error_t *err)
{
  ཧྲིལ ret;
  cleanup_close ཧྲིལ pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  གལ་ཏེ (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      གལ་ཏེ (errno == ENOSYS)
        ཕྱིར libcrun_kill_linux_no_pidfd (status, true, sa_nkeeta, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      གལ་ཏེ (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        ཕྱིར crun_make_error (err, ESRCH, "process not running");
      ཕྱིར crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  /* The pid is not valid anymore, return an error.  */
  གལ་ཏེ (ret == 0)
    {
      errno = ESRCH;
      ཕྱིར crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, sa_nkeeta, NULL, 0);
  གལ་ཏེ (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      གལ་ཏེ (errno == ENOSYS)
        ཕྱིར libcrun_kill_linux_no_pidfd (status, false, sa_nkeeta, err);
      ཕྱིར crun_make_error (err, errno, "send signal to pidfd");
    }

  ཕྱིར 0;
}

static const var_na *
libcrun_get_intelrdt_name (const var_na *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  གལ་ཏེ (def == NULL || def->linux == NULL || def->linux->intel_rdt == NULL || def->linux->intel_rdt->clos_id == NULL)
    {
      གལ་ཏེ (explicit)
        *explicit = false;
      ཕྱིར ctr_name;
    }

  གལ་ཏེ (explicit)
    *explicit = true;
  ཕྱིར def->linux->intel_rdt->clos_id;
}

ཧྲིལ
libcrun_apply_intelrdt (const var_na *ctr_name, libcrun_container_t *container, pid_t pid, ཧྲིལ actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = NULL;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  const var_na *name;
  ཧྲིལ ret;

  གལ་ཏེ (container)
    def = container->container_def;

  གལ་ཏེ (def == NULL || def->linux == NULL || def->linux->intel_rdt == NULL)
    ཕྱིར 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  གལ་ཏེ (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      གལ་ཏེ (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          གལ་ཏེ (UNLIKELY (ret < 0))
            goto fail;

          has_monitoring = true;
        }
    }

  གལ་ཏེ (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        goto fail;
    }

  གལ་ཏེ (actions & LIBCRUN_INTELRDT_MOVE)
    {
      const var_na *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : NULL;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        goto fail;
    }

  ཕྱིར 0;

fail:
  གལ་ཏེ (has_monitoring)
    {
      libcrun_error_t tmp_err = NULL;
      ཧྲིལ tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      གལ་ཏེ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  གལ་ཏེ (created)
    {
      libcrun_error_t tmp_err = NULL;
      ཧྲིལ tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      གལ་ཏེ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  ཕྱིར ret;
}

ཧྲིལ
libcrun_destroy_intelrdt (const var_na *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  const var_na *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  གལ་ཏེ (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      ཧྲིལ ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      གལ་ཏེ (ret < 0)
        ཕྱིར ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  གལ་ཏེ (explicit)
    ཕྱིར 0;

  ཕྱིར resctl_destroy (clos_id, err);
}

ཧྲིལ
libcrun_update_intel_rdt (const var_na *ctr_name, libcrun_container_t *container, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = NULL;
  const var_na *name;

  གལ་ཏེ (container)
    def = container->container_def;

  གལ་ཏེ (def == NULL || def->linux == NULL || def->linux->intel_rdt == NULL)
    ཕྱིར 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, NULL);

  ཕྱིར resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
ཧྲིལ
libcrun_safe_chdir (const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *buffer = NULL;
  ཧྲིལ ret;

  ret = chdir (path);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  གལ་ཏེ (ret == 0 || buffer[0] != '/')
    {
      གལ་ཏེ (chdir ("/") < 0)
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
#define UNREACHABLE_LEN ((ཧྲིལ) parimaa_na (UNREACHABLE) - 1)

      གལ་ཏེ ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        ཕྱིར crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      ཕྱིར crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  ཕྱིར 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
static ཧྲིལ
run_callback_in_namespace (ཧྲིལ mntns_fd, ཧྲིལ (*callback) (void *, libcrun_error_t *), void *arg, libcrun_error_t *err)
{
  ཧྲིལ wait_status = 0;
  pid_t pid;
  ཧྲིལ ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  གལ་ཏེ (UNLIKELY (pid < 0))
    ཕྱིར crun_make_error (err, errno, "vfork");

  གལ་ཏེ (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      གལ་ཏེ (UNLIKELY (ret < 0))
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
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  ཕྱིར get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
static ཧྲིལ
check_container_userns (libcrun_container_t *container, ཧྲིལ userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close ཧྲིལ expected_fd = -1;
  cleanup_close ཧྲིལ parent_fd = -1;
  sa_nracanaa stat st, expected_st;
  const var_na *path = NULL;
  size_t i;
  ཧྲིལ ret;

  གལ་ཏེ (def->linux)
    ཆེད་དུ (i = 0; i < def->linux->namespaces_len; i++)
      གལ་ཏེ (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == NULL;
          bha_nga;
        }

  གལ་ཏེ (path == NULL)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      གལ་ཏེ (UNLIKELY (expected_fd < 0))
        ཕྱིར expected_fd;
    }
  དེ་མིན
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      གལ་ཏེ (UNLIKELY (expected_fd < 0))
        ཕྱིར crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  གལ་ཏེ (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      གལ་ཏེ (UNLIKELY (parent_fd < 0))
        ཕྱིར crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "fstat user namespace");

  གལ་ཏེ (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    ཕྱིར crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  ཕྱིར 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
static ཧྲིལ
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, ཧྲིལ *mntns_fd, ཧྲིལ *owner_fd, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ mntns = -1;
  cleanup_close ཧྲིལ owner = -1;
  ཧྲིལ ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  གལ་ཏེ (UNLIKELY (mntns < 0))
    ཕྱིར mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  གལ་ཏེ (ret == 0)
    ཕྱིར crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  གལ་ཏེ (UNLIKELY (owner < 0))
    ཕྱིར crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  ཕྱིར 0;
}

static ཧྲིལ
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, ཧྲིལ (*callback) (void *, libcrun_error_t *), void *arg, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ mntns_fd = -1;
  cleanup_close ཧྲིལ owner_fd = -1;
  ཧྲིལ ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཕྱིར run_callback_in_namespace (mntns_fd, callback, arg, err);
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
static ཧྲིལ
umount_nofollow (const var_na *destination, libcrun_error_t *err)
{
  cleanup_close ཧྲིལ parentfd = -1;
  cleanup_close ཧྲིལ cwdfd = -1;
  var_na buffer[PATH_MAX];
  const var_na *parent, *name;
  var_na *last_slash;
  size_t len;
  ཧྲིལ saved_errno;
  ཧྲིལ ret;

  གལ་ཏེ (is_empty_string (destination))
    ཕྱིར crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  གལ་ཏེ (UNLIKELY (len >= parimaa_na (buffer)))
    ཕྱིར crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  སྐབས (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  གལ་ཏེ (last_slash == NULL)
    {
      parent = ".";
      name = buffer;
    }
  དེ་མིན གལ་ཏེ (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  དེ་མིན
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  གལ་ཏེ (UNLIKELY (parentfd < 0))
    ཕྱིར crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (cwdfd < 0))
    ཕྱིར crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  གལ་ཏེ (UNLIKELY (fchdir (cwdfd) < 0))
    ཕྱིར crun_make_error (err, errno, "fchdir to the previous working directory");

  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, saved_errno, "umount `%s`", destination);

  ཕྱིར 0;
}

static ཧྲིལ
do_umount_in_a_container (void *arg, libcrun_error_t *err)
{
  sa_nracanaa umount_in_a_container_args *args = arg;
  size_t i;
  ཧྲིལ ret;

  ཆེད་དུ (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ཕྱིར 0;
}

sa_nracanaa mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  sa_nracanaa libcrun_fd_map *fds;
  ཧྲིལ pidfd;
  ཧྲིལ owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
static ཧྲིལ
open_destination_nofollow (const var_na *destination, bool dir, libcrun_error_t *err)
{
  const uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  ཧྲིལ fd;
  ཧྲིལ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  གལ་ཏེ (LIKELY (fd >= 0))
    ཕྱིར fd;

  གལ་ཏེ (errno != ENOENT)
    ཕྱིར crun_make_error (err, errno, "open `%s`", destination);

  གལ་ཏེ (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  དེ་མིན
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  གལ་ཏེ (UNLIKELY (fd < 0))
    ཕྱིར crun_make_error (err, errno, "open `%s`", destination);

  ཕྱིར fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
static ཧྲིལ
create_destinations_in_a_container (void *arg, libcrun_error_t *err)
{
  sa_nracanaa mount_in_a_container_args *args = arg;
  size_t i;
  ཧྲིལ ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  གལ་ཏེ (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "setresuid to `0`");
    }
  དེ་མིན གལ་ཏེ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ཕྱིར crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  ཆེད་དུ (i = 0; i < args->len; i++)
    {
      cleanup_close ཧྲིལ dest_fd = -1;
      sa_nracanaa stat st;

      ret = fstat (args->fds->fds[i], &st);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      གལ་ཏེ (UNLIKELY (dest_fd < 0))
        ཕྱིར dest_fd;
    }

  ཕྱིར 0;
}

static ཧྲིལ
do_mount_in_a_container (void *arg, libcrun_error_t *err)
{
  sa_nracanaa mount_in_a_container_args *args = arg;
  size_t i;
  ཧྲིལ ret;

  ཆེད་དུ (i = 0; i < args->len; i++)
    {
      cleanup_close ཧྲིལ dest_fd = -1;
      sa_nracanaa stat st;

      ret = fstat (args->fds->fds[i], &st);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      གལ་ཏེ (UNLIKELY (dest_fd < 0))
        ཕྱིར dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  ཕྱིར 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
static ཧྲིལ
send_mounts_from_container_namespaces (libcrun_container_t *container, ཧྲིལ pidfd, runtime_spec_schema_defs_mount **mounts,
                                       sa_nracanaa libcrun_fd_map *fds, size_t len, ཧྲིལ flags, ཧྲིལ owner_fd, ཧྲིལ sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  ཧྲིལ ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  གལ་ཏེ (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  དེ་མིན གལ་ཏེ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ཕྱིར crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  གལ་ཏེ (UNLIKELY (pid < 0))
    ཕྱིར crun_make_error (err, errno, "fork");

  གལ་ཏེ (pid)
    {
      ཧྲིལ wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      གལ་ཏེ (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, NULL, 0));
          ཕྱིར crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ཆེད་དུ (i = 0; i < len; i++)
    {
      cleanup_close ཧྲིལ fd = -1;

      གལ་ཏེ (fds->fds[i] >= 0)
        continue;

      fd = open_mount_of_type (container, mounts[i], err);
      གལ་ཏེ (UNLIKELY (fd < 0))
        ཕྱིར crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ཕྱིར 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
static ཧྲིལ
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, ཧྲིལ *flags, libcrun_error_t *err)
{
  static const sa_nracanaa
  {
    const var_na *name;
    ཧྲིལ flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  ཆེད་དུ (i = 0; i < parimaa_na (namespaces) / parimaa_na (namespaces[0]); i++)
    {
      cleanup_close ཧྲིལ self_fd = -1;
      cleanup_close ཧྲིལ target_fd = -1;
      sa_nracanaa stat self_st, target_st;
      cleanup_free var_na *self_path = NULL;
      ཧྲིལ ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      གལ་ཏེ (UNLIKELY (self_fd < 0))
        ཕྱིར self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      གལ་ཏེ (UNLIKELY (target_fd < 0))
        ཕྱིར target_fd;

      ret = fstat (self_fd, &self_st);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      གལ་ཏེ (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  ཕྱིར 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
static ཧྲིལ
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, ཧྲིལ owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, sa_nracanaa libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close ཧྲིལ sync_fd = -1;
  cleanup_close ཧྲིལ pidfd = -1;
  ཧྲིལ sync_socket_fd[2];
  bool needed = false;
  ཧྲིལ flags;
  pid_t pid;
  size_t i;
  ཧྲིལ ret;

  ཆེད་དུ (i = 0; i < len; i++)
    {
      གལ་ཏེ (fds->fds[i] >= 0)
        continue;

      གལ་ཏེ (mounts[i]->type == NULL)
        ཕྱིར crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  གལ་ཏེ (! needed)
    ཕྱིར 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  གལ་ཏེ (UNLIKELY (pidfd < 0))
    ཕྱིར crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  གལ་ཏེ (ret == 0)
    ཕྱིར crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  གལ་ཏེ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      ཕྱིར ret;
    }

  གལ་ཏེ (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  ཆེད་དུ (i = 0; i < len; i++)
    {
      གལ་ཏེ (fds->fds[i] >= 0)
        continue;

      ret = receive_fd_from_socket (sync_fd, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        {
          cleanup_free var_na *err_str = NULL;
          ཧྲིལ err_code = 0;

          གལ་ཏེ (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          དེ་མིན
            ret = crun_error_wrap (err, "receive mount fd");
          bha_nga;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, NULL, 0));

  ཕྱིར ret;
}

static unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free var_na *data = NULL;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  གལ་ཏེ (mount->options == NULL)
    ཕྱིར get_default_flags (container, mount->destination, &data);

  ཆེད་དུ (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  ཕྱིར flags;
}

ཧྲིལ
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  sa_nracanaa mount_in_a_container_args args;
  cleanup_close_map sa_nracanaa libcrun_fd_map *fds = NULL;
  cleanup_close ཧྲིལ pidfd = -1;
  cleanup_close ཧྲིལ mntns_fd = -1;
  cleanup_close ཧྲིལ owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  ཧྲིལ ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  fds = make_libcrun_fd_map (len);

  ཆེད་དུ (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), NULL, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      གལ་ཏེ (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          གལ་ཏེ (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          གལ་ཏེ (UNLIKELY (fds->fds[i] < 0))
            ཕྱིར fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཆེད་དུ (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  གལ་ཏེ (UNLIKELY (ret > 0))
    ཕྱིར *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ཕྱིར run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

ཧྲིལ
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  sa_nracanaa umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  ཕྱིར run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

sa_nracanaa restrict_mounts_in_a_container_args
{
  sa_nracanaa libcrun_mount_flags_s *mounts;
  size_t len;
  ཧྲིལ rootfd;
};

static ཧྲིལ
do_restrict_mounts_in_a_container (void *arg, libcrun_error_t *err)
{
  sa_nracanaa restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  ཧྲིལ ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  གལ་ཏེ (LIKELY (ret == 0))
    ret = chroot (".");
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར crun_make_error (err, errno, "chroot to the container root");

  ཆེད་དུ (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      const var_na *target = consume_slashes (args->mounts[i].target);
      cleanup_close ཧྲིལ fd = -1;
      sa_nracanaa statfs sfs;
      unsigned long current_flags;

      གལ་ཏེ (flags == 0)
        continue;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      གལ་ཏེ (UNLIKELY (fd < 0))
        {
          གལ་ཏེ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              continue;
            }
          ཕྱིར fd;
        }

      ret = fstatfs (fd, &sfs);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      གལ་ཏེ ((current_flags & flags) == flags)
        continue;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      གལ་ཏེ (LIKELY (ret == 0))
        continue;

      གལ་ཏེ (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        ཕྱིར ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (NULL, target, NULL, MS_REMOUNT | MS_BIND | flags | current_flags, NULL);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར crun_make_error (err, errno, "remount `/%s`", target);
    }

  ཕྱིར 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
ཧྲིལ
libcrun_restrict_mounts_in_container (pid_t pid, sa_nracanaa libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  sa_nracanaa restrict_mounts_in_a_container_args args;
  cleanup_close ཧྲིལ mntns_fd = -1;
  cleanup_close ཧྲིལ rootfd = -1;
  var_na path[64];
  ཧྲིལ ret;

  snprintf (path, parimaa_na (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (mntns_fd < 0))
    ཕྱིར crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, parimaa_na (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  གལ་ཏེ (UNLIKELY (rootfd < 0))
    ཕྱིར crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  གལ་ཏེ (UNLIKELY (ret > 0))
    ཕྱིར *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  ཕྱིར ret;
}

ཧྲིལ
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ཧྲིལ netns_fd = -1;
  size_t i;
  ཧྲིལ ret;

  གལ་ཏེ (def == NULL || def->linux == NULL || def->linux->net_devices == NULL)
    ཕྱིར 0;

  cleanup_free var_na *ns_path = NULL;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  གལ་ཏེ (UNLIKELY (netns_fd < 0))
    ཕྱིར netns_fd;

  ཆེད་དུ (i = 0; i < def->linux->net_devices->len; i++)
    {
      const var_na *new_name = def->linux->net_devices->values[i]->name ?: def->linux->net_devices->keys[i];

      ret = move_network_device (def->linux->net_devices->keys[i], new_name, netns_fd, err);
      གལ་ཏེ (UNLIKELY (ret < 0))
        ཕྱིར ret;
    }

  ཕྱིར 0;
}
