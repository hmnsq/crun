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
#もし defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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

  もし (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  もし (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  もし (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  もし (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  もし (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  もし (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  もし (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  もし (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  もし (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  戻る flags;
}

struct remount_s
{
  struct remount_s *next;
  char *target;
  整数 targetfd;
  unsigned long flags;
  char *data;
};

struct private_data_s
{
  struct remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  整数 unshare_flags;
  整数 unshare_cgroupns;

  char *host_notify_socket_path;
  char *container_notify_socket_path;
  char *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  整数 procfd;

  const char *rootfs;
  整数 rootfsfd;

  整数 notify_socket_tree_fd;
  整数 old_root_fd;

  struct libcrun_fd_map *mount_fds;
  struct libcrun_fd_map *dev_fds;
  struct libcrun_fd_map *needed_devs_fds;

  char **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  char *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  整数 maskdir_fd;
  char *maskdir_proc_path;
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
  char *host_rootfs;
};

struct linux_namespace_s
{
  const char *name;
  const char *ns_file;
  整数 value;
};

static void
cleanup_private_data (void *private_data)
{
  struct private_data_s *p = private_data;

  もし (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  もし (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  もし (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  もし (p->old_root_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_root_fd));
  もし (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  もし (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  もし (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  もし (p->copy_symlink_targets)
    {
      size_t i;
      ために (i = 0; i < p->n_copy_symlink_targets; i++)
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

static struct private_data_s *
get_private_data (struct libcrun_container_s *container)
{
  もし (container->private_data == NULL)
    {
      struct private_data_s *p = xmalloc0 (sizeof (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_tree_fd = -1;
      p->old_root_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  戻る container->private_data;
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

static struct linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { NULL, NULL, 0 } };

static 整数
get_and_reset (整数 *old)
{
  整数 tmp = *old;
  *old = -1;
  戻る tmp;
}

整数
libcrun_find_namespace (const char *name)
{
  struct linux_namespace_s *it;
  ために (it = namespaces; it->name; it++)
    もし (strcmp (it->name, name) == 0)
      戻る it->value;
  戻る -1;
}

static 整数
syscall_keyctl_join (const char *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  戻る (整数) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

static 整数
syscall_pidfd_open (pid_t pid, unsigned 整数 flags)
{
#もし defined __NR_pidfd_open
  戻る (整数) syscall (__NR_pidfd_open, pid, flags);
#否则
  (void) pid;
  (void) flags;
  errno = ENOSYS;
  戻る -1;
#endif
}

static 整数
syscall_pidfd_send_signal (整数 pidfd, 整数 sig, siginfo_t *info, unsigned 整数 flags)
{
#もし defined __NR_pidfd_send_signal
  戻る (整数) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#否则
  (void) pidfd;
  (void) sig;
  (void) info;
  (void) flags;
  errno = ENOSYS;
  戻る -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
static uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  もし (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  もし (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  もし (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  もし (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  もし (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  もし (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  もし (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  もし (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  戻る attr;
}

static 整数
do_mount_setattr (bool recursive, const char *target, 整数 targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  struct mount_attr_s attr = {
    0,
  };
  整数 ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  もし (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      もし (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      否则 もし (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      否则
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      もし (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  否则 もし (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "mount_setattr `/%s`", target);

  戻る 0;
}

整数
get_bind_mount (整数 dirfd, const char *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close 整数 open_tree_fd = -1;
  struct mount_attr_s attr = {
    0,
  };
  整数 recursive_flag = (recursive ? AT_RECURSIVE : 0);
  整数 ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  もし (UNLIKELY (src == NULL))
    戻る crun_make_error (err, EINVAL, "mount source is not specified");

  もし (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  もし (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_tree_fd = syscall_open_tree (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  もし (UNLIKELY (open_tree_fd < 0))
    戻る crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_tree_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "mount_setattr `%s`", src);

  戻る get_and_reset (&open_tree_fd);
}

整数
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, char *option, char **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free char *mappings = NULL;
  const char *it;
  整数 ret;

  mappings = xmalloc (allocated);

  ために (it = option; *it;)
    {
      bool relative = false;
      long value[3];
      size_t i;

      もし (*it == '\0')
        break;

      もし (*it == '#')
        it++;

      もし (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      ために (i = 0; i < 3; i++)
        {
          char *endptr = NULL;

          もし (i > 0 && *it == '-')
            it++;

          もし (*it == '\0')
            戻る crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          value[i] = strtol (it, &endptr, 10);
          もし (errno || endptr == it)
            戻る crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      もし (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          もし (def == NULL
              || def->linux == NULL
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            戻る crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          ために (i = 0; i < mappings_len; i++)
            もし (value[1] >= (int64_t) mappings[i]->container_id
                && value[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              break;

          もし (i == mappings_len)
            戻る crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          value[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      もし (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", value[0], value[1], value[2]);
      もし (UNLIKELY (ret >= (整数) (allocated - written)))
        戻る crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = NULL;

  戻る 0;
}

static 整数
format_mount_mappings (char **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  const size_t max_len_mapping = 64;
  cleanup_free char *ret = NULL;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  ために (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      もし (UNLIKELY (len >= max_len_mapping))
        戻る crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = NULL;
  戻る 0;
}

static 整数
format_mount_mapping (char **ret, uint32_t container_id, uint32_t host_id,
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

  戻る format_mount_mappings (ret, mappings, 1, written, err);
}

static bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  もし (def->linux == NULL)
    戻る mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  もし (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    戻る false;

  もし (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    戻る false;

  ために (s = 0; s < mnt->uid_mappings_len; s++)
    {
      もし (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        戻る false;
      もし (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        戻る false;
      もし (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        戻る false;
    }

  ために (s = 0; s < mnt->gid_mappings_len; s++)
    {
      もし (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        戻る false;
      もし (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        戻る false;
      もし (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        戻る false;
    }

  戻る true;
}

static pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        const char *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != NULL;

  もし (! need_new_userns)
    戻る 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, NULL);
  もし (UNLIKELY (pid < 0))
    戻る crun_make_error (err, errno, "clone");

  もし (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      ながら (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  もし (mnt->uid_mappings_len)
    {
      cleanup_free char *uid_map = NULL;
      cleanup_free char *gid_map = NULL;
      size_t written = 0;
      整数 ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      cleanup_close 整数 fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      もし (UNLIKELY (fd < 0))
        戻る fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      もし (UNLIKELY (fd < 0))
        戻る fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  否则
    {
      cleanup_free char *dup_options = NULL;
      char *option, *saveptr = NULL;

      もし (! options)
        戻る crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      ために (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (NULL, ";", &saveptr))
        {
          cleanup_free char *mappings = NULL;
          cleanup_close 整数 map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          整数 ret;

          もし (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              もし (UNLIKELY (map_fd < 0))
                戻る map_fd;
            }
          否则 もし (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              もし (UNLIKELY (map_fd < 0))
                戻る map_fd;
            }
          否则
            戻る crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  戻る 0;
}

整数
libcrun_create_keyring (libcrun_container_t *container, const char *name, const char *label, libcrun_error_t *err)
{
  cleanup_close 整数 labelfd = -1;
  bool label_set = false;
  整数 ret;

  もし (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      もし (UNLIKELY (labelfd < 0))
        {
          もし (crun_error_get_errno (err) != ENOENT)
            戻る labelfd;

          crun_error_release (err);
        }

      もし (labelfd >= 0)
        {
          ret = write (labelfd, label, strlen (label));
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  もし (UNLIKELY (ret < 0))
    {
      もし (errno == ENOSYS)
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
  もし (label_set && write (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  戻る ret;
}

static void
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  もし (def->process && def->process->user)
    {
      もし (def->process->user->uid)
        *uid = def->process->user->uid;
      もし (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

static unsigned long
get_mount_flags (const char *name, 整数 current_flags, 整数 *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  const struct propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  もし (found)
    *found = prop ? 1 : 0;

  もし (prop == NULL)
    戻る 0;

  もし (prop->extra_flags & OPTION_RECURSIVE)
    {
      もし (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      もし (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  もし (extra_flags)
    *extra_flags |= prop->extra_flags;

  もし (prop->clear)
    戻る current_flags & ~prop->flags;

  戻る current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
static bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ために (i = 0; i < mount->options_len; i++)
    {
      const struct propagation_flags_s *prop;

      もし (mount->options[i] == NULL)
        continue;

      prop = libcrun_str2mount_flags (mount->options[i]);
      もし (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        戻る true;
    }

  戻る false;
}

static unsigned long
get_mount_flags_or_option (const char *name, 整数 current_flags, unsigned long *extra_flags, char **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  整数 found;
  __attribute__ ((unused)) cleanup_free char *prev = NULL;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  もし (name == NULL)
    戻る 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  もし (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      const char *data_flag = NULL;
      もし ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      否则 もし ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      もし (data_flag)
        {
          prev = *option;
          もし (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          否则
            *option = xstrdup (data_flag);
        }
      戻る flags;
    }

  prev = *option;
  もし (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  否则
    *option = xstrdup (name);

  戻る 0;
}

整数
pivot_root (const char *new_root, const char *put_old)
{
  戻る syscall (__NR_pivot_root, new_root, put_old);
}

static void
free_remount (struct remount_s *r)
{
  もし (r == NULL)
    戻る;
  もし (r->targetfd >= 0)
    close (r->targetfd);
  free (r->data);
  free (r->target);
  free (r);
}

static struct remount_s *
make_remount (整数 targetfd, const char *target, unsigned long flags, const char *data, struct remount_s *next)
{
  struct remount_s *ret = xmalloc (sizeof (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : NULL;
  ret->next = next;
  ret->targetfd = targetfd;
  戻る ret;
}

static 整数
do_remount (整数 targetfd, const char *target, unsigned long flags, const char *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  整数 ret;
  const char *real_target = target;

  もし (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      もし (LIKELY (ret == 0))
        戻る 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  もし (flags & (MS_REMOUNT | MS_RDONLY))
    data = NULL;

  ret = mount (NULL, real_target, NULL, flags, data);
  もし (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      struct statfs sfs;

      ret = statfs (real_target, &sfs);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      もし (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      もし ((flags | remount_flags) != flags)
        {
          ret = mount (NULL, real_target, NULL, flags | remount_flags, data);
          もし (LIKELY (ret == 0))
            戻る 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          もし (current_flags & MS_RDONLY)
            ret = mount (NULL, real_target, NULL, flags | remount_flags | MS_RDONLY, data);
        }
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "remount `%s`", target);
    }
  戻る 0;
}

static 整数
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  整数 ret = 0;
  struct remount_s *r = get_private_data (container)->remounts;
  ながら (r)
    {
      struct remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      もし (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          もし (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              continue;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      もし (UNLIKELY (ret < 0))
        goto cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  ながら (r)
    {
      struct remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = NULL;
  戻る ret;
}

static 整数
open_mount_target (libcrun_container_t *container, const char *target_rel, libcrun_error_t *err)
{
  const char *rootfs = get_private_data (container)->rootfs;
  整数 rootfsfd = get_private_data (container)->rootfsfd;

  もし (rootfsfd < 0)
    戻る crun_make_error (err, 0, "invalid rootfs state");

  戻る safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
static 整数
fsopen_mount (const char *type, const char *source_name, const char *labeltype, const char *label, const char *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close 整数 fsfd = -1;
  整数 ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  もし (UNLIKELY (fsfd < 0))
    戻る fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (void) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  もし (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (data && data[0])
    {
      cleanup_free char *data_copy = xstrdup (data);
      char *saveptr = NULL;
      char *token;

      ために (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (NULL, ",", &saveptr))
        {
          char *eq = strchr (token, '=');
          もし (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          否则
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, NULL, 0);

          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#否则
  (void) type;
  (void) source_name;
  (void) data;
  errno = ENOSYS;
  戻る -1;
#endif
}

static 整数
get_procfd (struct private_data_s *data, libcrun_error_t *err)
{
  整数 fd;

  もし (data->procfd >= 0)
    戻る data->procfd;

  fd = fsopen_mount ("proc", NULL, NULL, NULL, NULL);
  もし (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  もし (UNLIKELY (fd < 0))
    戻る crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  戻る fd;
}

static 整数
get_old_root_fd (struct private_data_s *data)
{
  もし (data->old_root_fd >= 0)
    戻る data->old_root_fd;

  data->old_root_fd = open ("/", O_PATH | O_CLOEXEC);
  戻る data->old_root_fd;
}

static 整数
fs_move_mount_to (整数 fd, 整数 dirfd, const char *name)
{
#ifdef HAVE_NEW_MOUNT_API
  もし (name)
    戻る syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  戻る syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#否则
  (void) fd;
  (void) dirfd;
  (void) name;
  (void) syscall_move_mount;
  errno = ENOSYS;
  戻る -1;
#endif
}

enum
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

static 整数 do_mount (libcrun_container_t *container, const char *source, 整数 targetfd,
                     const char *target, const char *fstype, unsigned long mountflags,
                     const void *data, 整数 extra_flags, libcrun_error_t *err);

static bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, const char *option)
{
  size_t i;
  ために (i = 0; i < mnt->options_len; i++)
    もし (mnt->options[i] != NULL && strcmp (mnt->options[i], option) == 0)
      戻る true;
  戻る false;
}

static runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, const char *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  ために (i = 0; i < def->mounts_len; i++)
    {
      もし (strcmp (def->mounts[i]->destination, destination) == 0)
        戻る def->mounts[i];
    }
  戻る NULL;
}

static void
warn_tmpfs_fallback_once (struct private_data_s *private_data, const char *reason)
{
  もし (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
static 整数
get_shared_empty_dir_cached (libcrun_container_t *container, char **proc_fd_path, libcrun_error_t *err)
{
  struct private_data_s *private_data = get_private_data (container);
  cleanup_close 整数 fd = -1;
  cleanup_free char *empty_dir_path = NULL;
  proc_fd_path_t fd_path;
  整数 ret;

  /* Fast path: return cached proc fd path if already set up */
  もし (private_data->maskdir_proc_path != NULL)
    {
      もし (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      戻る 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_root, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  もし (fd < 0)
    戻る fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  もし (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  戻る 0;
}

static bool
is_dev_null (const struct stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  戻る S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
static 整数
ensure_dev_null (整数 fd, libcrun_error_t *err)
{
  struct stat st;

  もし (UNLIKELY (fstat (fd, &st) < 0))
    戻る crun_make_error (err, errno, "stat `/dev/null`");

  もし (UNLIKELY (! is_dev_null (&st)))
    戻る crun_make_error (err, 0, "`/dev/null` is not the null device");

  戻る 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
static bool
is_empty_ro_tmpfs (整数 fd)
{
  struct statfs sfs;

  もし (fstatfs (fd, &sfs) < 0)
    戻る false;

  戻る sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

static 整数
mount_masked_dir (libcrun_container_t *container, 整数 pathfd, const char *rel_path, libcrun_error_t *err)
{
  struct private_data_s *private_data = get_private_data (container);
  cleanup_close 整数 newfd = -1;
  libcrun_error_t tmp_err = NULL;
  整数 ret;

  もし (private_data->maskdir_fd < 0)
    {
      もし (private_data->maskdir_bind_failed)
        goto fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, NULL, &tmp_err);
      もし (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->msg);
          crun_error_release (&tmp_err);
          goto fallback_to_tmpfs;
        }
    }

  もし (private_data->maskdir_fd >= 0)
    {
      整数 rootfsfd = private_data->rootfsfd;
      struct stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, NULL);
      もし (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          もし (UNLIKELY (newfd < 0))
            戻る newfd;

          ret = fstat (newfd, &after);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          もし (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            戻る crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          戻る 0;
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
  もし (UNLIKELY (ret < 0))
    戻る ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  もし (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      もし (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      否则
        {
          もし (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  戻る 0;
}

static 整数
do_masked_or_readonly_path (libcrun_container_t *container, const char *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  const char *rootfs = get_private_data (container)->rootfs;
  cleanup_close 整数 pathfd = -1;
  struct statfs sfs;
  整数 ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  もし (UNLIKELY (rel_path == NULL))
    戻る crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  もし (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  もし (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      もし (errno != ENOENT && errno != EACCES)
        戻る pathfd;

      もし (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      戻る 0;
    }

  もし (readonly)
    {
      整数 procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close 整数 mountfd = -1;

      もし (UNLIKELY (procfd < 0))
        戻る procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      もし (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          もし (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, NULL);
          否则
            ret = -1;

          もし (ret < 0)
            crun_error_release (err);
        }

      もし (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          もし (keep_flags)
            {
              cleanup_close 整数 sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              もし (UNLIKELY (sfd < 0))
                戻る crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              もし (UNLIKELY (ret < 0))
                戻る crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          もし (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              もし (UNLIKELY (ret < 0))
                戻る ret;

              ret = fs_move_mount_to (mountfd, pathfd, NULL);
              もし (UNLIKELY (ret < 0))
                戻る crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          否则
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, NULL, mount_flags, NULL,
                              LABEL_NONE, err);
              もし (UNLIKELY (ret < 0))
                戻る ret;
            }
        }
    }
  否则
    {
      ret = get_file_type_fd (pathfd, &mode);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      もし ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      否则
        {
          cleanup_close 整数 nullfd = -1;
          cleanup_close 整数 mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          もし (UNLIKELY (nullfd < 0))
            戻る crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          もし (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, NULL);

          もし (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, NULL, MS_BIND | MS_RDONLY, NULL,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  戻る 0;
}

static const char *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  const char *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  もし (context_type)
    {
      もし (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          戻る NULL;
        }
      戻る context_type;
    }

  戻る "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
static 整数
diagnose_mount_failure (整数 ret, libcrun_error_t *err, libcrun_container_t *container,
                        const char *fstype)
{
  もし (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      もし (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          戻る ret;
        }
    }
  戻る ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
static 整数
sysfs_userns_fallback (libcrun_container_t *container, const char *target,
                       const char *real_target, 整数 targetfd,
                       unsigned long mountflags, const char *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close 整数 mountfd = -1;
  cleanup_close 整数 sysfd = -1;
  整数 sys_old_root_fd = -1;
  整数 ret;

  もし (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    戻る 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (ret == 0)
    戻る 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_root_fd = get_old_root_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  もし (sys_old_root_fd >= 0)
    mountfd = get_bind_mount (sys_old_root_fd, "sys", true, true, false, MS_PRIVATE, err);
  もし (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  もし (cgroup_mount == NULL)
    {
      もし (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, NULL);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      否则
        {
          crun_error_release (err);
          もし (sys_old_root_fd >= 0)
            {
              cleanup_free char *sys_path = NULL;
              proc_fd_path_t sys_root_path;

              get_proc_self_fd_path (sys_root_path, sys_old_root_fd);
              xasprintf (&sys_path, "%s/sys", sys_root_path);
              ret = mount (sys_path, real_target, NULL, MS_BIND | MS_REC, NULL);
            }
          否则
            ret = mount ("/sys", real_target, NULL, MS_BIND | MS_REC, NULL);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      もし (UNLIKELY (sysfd < 0))
        戻る sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        NULL, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      戻る do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  もし (UNLIKELY (mountfd < 0))
    戻る mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, NULL);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  もし (UNLIKELY (sysfd < 0))
    戻る sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    NULL, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
static void
apply_selinux_xattr (libcrun_container_t *container, 整数 fd, 整数 extra_flags, const char *label)
{
  libcrun_error_t tmp_err = NULL;
  整数 procfd;

  もし ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    戻る;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  もし (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close 整数 xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      もし (xfd >= 0)
        (void) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
static 整数
do_mount_replace_rootfs (libcrun_container_t *container, 整数 *fd, libcrun_error_t *err)
{
  整数 procfd = get_procfd (get_private_data (container), err);
  整数 tmp;
  整数 ret;

  もし (UNLIKELY (procfd < 0))
    戻る procfd;

  {
    cleanup_close 整数 nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    もし (UNLIKELY (nsfd < 0))
      戻る crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    もし (UNLIKELY (ret < 0))
      戻る crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  もし (UNLIKELY (*fd < 0))
    戻る crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  もし (UNLIKELY (tmp < 0))
    戻る crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  戻る 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
static 整数
apply_propagation_flags (const char *target, 整数 targetfd, const char *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  整数 ret;

  もし (targetfd >= 0)
    {
      libcrun_error_t tmp_err = NULL;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      もし (LIKELY (ret == 0))
        propagation_done = true;
      否则
        crun_error_release (&tmp_err);
    }

  もし (! propagation_done)
    {
      ret = mount (NULL, real_target, NULL, mountflags & ALL_PROPAGATIONS, NULL);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  戻る 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
static 整数
schedule_or_do_remount (libcrun_container_t *container, 整数 *fd, 整数 targetfd,
                        const char *target, const char *real_target,
                        bool single_instance, unsigned long mountflags,
                        const void *data, 整数 extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  struct remount_s *r;

  もし ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    戻る do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  もし (*fd < 0)
    {
      *fd = dup (targetfd);
      もし (UNLIKELY (*fd < 0))
        戻る crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  戻る 0;
}

static 整数
do_mount (libcrun_container_t *container, const char *source, 整数 targetfd,
          const char *target, const char *fstype, unsigned long mountflags, const void *data,
          整数 extra_flags, libcrun_error_t *err)
{
  cleanup_free char *data_with_label = NULL;
  const char *data_without_label = data;
  proc_fd_path_t target_buffer;
  const char *context_type = NULL;
  cleanup_close 整数 ms_move_fd = -1;
  const char *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close 整数 fd = -1;
  const char *label = NULL;
  整数 ret = 0;

  もし (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  否则
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  もし (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  もし ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      もし (UNLIKELY (context_type == NULL))
        戻る -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      もし (ret < 0)
        戻る ret;
      data = data_with_label;
    }

  もし (mountflags & MS_MOVE)
    {
      もし ((mountflags & MS_BIND) || fstype)
        戻る crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, NULL, MS_MOVE, NULL);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      もし (UNLIKELY (ms_move_fd < 0))
        戻る ms_move_fd;
      targetfd = ms_move_fd;
    }

  もし ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      もし (targetfd >= 0)
        {
          もし (mountflags & MS_BIND)
            {
              cleanup_close 整数 bindfd = -1;
              libcrun_error_t tmp_err = NULL;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              もし (bindfd < 0)
                {
                  整数 old_root_fd = get_old_root_fd (get_private_data (container));

                  もし (old_root_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_root_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              もし (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, NULL);
              もし (ret < 0)
                crun_error_release (&tmp_err);
            }
          否则
            {
              cleanup_close 整数 newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              もし (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, NULL);
            }
        }

      もし (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          もし (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              整数 old_root_fd = get_old_root_fd (get_private_data (container));

              もし (old_root_fd >= 0)
                {
                  cleanup_free char *proc_source_buf = NULL;
                  proc_fd_path_t old_root_path;

                  get_proc_self_fd_path (old_root_path, old_root_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_root_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      もし (UNLIKELY (ret < 0))
        {
          整数 saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          もし (ret == 0)
            戻る 0;
          もし (UNLIKELY (ret < 0))
            戻る ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          戻る diagnose_mount_failure (ret, err, container, fstype);
        }

      もし (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          もし (UNLIKELY (fd < 0))
            戻る fd;

          /* We are replacing the rootfs, reopen it.  */
          もし (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              もし (UNLIKELY (ret < 0))
                戻る ret;
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

  もし (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  もし (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  もし (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る ret;
}

static void
try_umount (整数 targetfd, const char *target)
{
  proc_fd_path_t target_buffer;
  const char *real_target = target;

  もし (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

static bool
container_has_cgroupns (libcrun_container_t *container)
{
  戻る get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

static 整数
do_mount_cgroup_v2 (libcrun_container_t *container, 整数 targetfd, const char *target,
                    unsigned long mountflags, const char *data, libcrun_error_t *err)
{
  整数 ret;
  整数 cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  もし (UNLIKELY (cgroup_mode < 0))
    戻る cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  もし (UNLIKELY (ret < 0))
    {
      整数 saved_errno = crun_error_get_errno (err);
      もし (saved_errno == EPERM || saved_errno == EBUSY)
        {
          const char *unified_cgroup_path;
          const char *src_cgroup;

          crun_error_release (err);

          もし (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              もし (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  もし (LIKELY (ret == 0))
                    戻る ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_ROOT;
          戻る do_mount (container, src_cgroup, targetfd, target, NULL,
                           MS_BIND | mountflags, NULL, LABEL_NONE, err);
        }

      戻る ret;
    }

  戻る 0;
}

static 整数
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, const char *source, 整数 targetfd, const char *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  整数 ret;
  cleanup_close 整数 fd = -1;
  const char *subsystem = "systemd";
  cleanup_free char *subsystem_path = NULL;
  cleanup_close 整数 tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  もし (UNLIKELY (tmpfsdirfd < 0))
    戻る tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, NULL);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  もし (UNLIKELY (fd < 0))
    戻る crun_make_error (err, errno, "openat `%s`", subsystem_path);

  戻る do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

static 整数
do_mount_cgroup_v1 (libcrun_container_t *container, const char *source, 整数 targetfd, const char *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  整数 ret;
  cleanup_free char *content = NULL;
  char *from;
  cleanup_close 整数 tmpfsdirfd = -1;
  char *saveptr = NULL;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  もし (UNLIKELY (tmpfsdirfd < 0))
    戻る tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    整数 procfd = get_procfd (get_private_data (container), err);
    もし (UNLIKELY (procfd < 0))
      戻る procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, NULL, err);
  }
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (UNLIKELY (content == NULL || content[0] == '\0'))
    戻る crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ために (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (NULL, "\n", &saveptr))
    {
      cleanup_free char *source_path = NULL;
      cleanup_free char *source_subsystem = NULL;
      cleanup_free char *subsystem_path = NULL;
      char *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close 整数 subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      もし (subsystem[0] == '\0')
        continue;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      もし (it)
        subsystem = it + 5;

      もし (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      もし (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_ROOT, subsystem, NULL);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      /* if there is already a mount specified, do not add a default one.  */
      もし (find_mount_for (container, source_subsystem) != NULL)
        continue;

      ret = append_paths (&source_path, err, source_subsystem, subpath, NULL);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, NULL);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      もし (UNLIKELY (subsystemfd < 0))
        戻る crun_make_error (err, errno, "open `%s`", subsystem_path);

      もし (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          もし (UNLIKELY (ret < 0))
            {
              もし (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  continue;
                }
              戻る ret;
            }
        }
      否则
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, NULL, MS_BIND | mountflags, NULL,
                          LABEL_NONE, err);
          もし (UNLIKELY (ret < 0))
            {
              もし (crun_error_get_errno (err) != ENOENT)
                戻る ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, NULL, MS_BIND | mountflags,
                              NULL, LABEL_NONE, err);
              もし (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  もし (crun_error_get_errno (err) != ENOENT)
                    戻る ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る 0;
}

static 整数
do_mount_cgroup (libcrun_container_t *container, const char *source, 整数 targetfd, const char *target,
                 unsigned long mountflags, const char *data, libcrun_error_t *err)
{
  整数 cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  もし (UNLIKELY (cgroup_mode < 0))
    戻る cgroup_mode;

  switch (cgroup_mode)
    {
    case CGROUP_MODE_UNIFIED:
      戻る do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    case CGROUP_MODE_LEGACY:
    case CGROUP_MODE_HYBRID:
      戻る do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  戻る crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

struct device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
  struct private_data_s *private_data = get_private_data (container);
  size_t i;

  もし (private_data->needed_devs_fds)
    {
      struct libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      ために (i = 0; needed_devs[i].path; i++)
        {
          もし (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            continue;

          整数 fd = syscall_open_tree (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          もし (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          否则 もし (fd >= 0)
            close (fd);
        }
    }

  もし (private_data->notify_socket_tree_fd < 0
      && private_data->host_notify_socket_path)
    {
      整数 fd = syscall_open_tree (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      もし (fd >= 0)
        private_data->notify_socket_tree_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
static const char *
relative_path_under_dev (const char *path)
{
  もし (path[0] != '/')
    戻る path;
  もし (strncmp (path, "/dev/", 5) == 0)
    {
      もし (strchr (path + 5, '/'))
        戻る NULL;
      戻る path + 5;
    }
  戻る NULL;
}

static 整数
mknod_and_set_attrs (libcrun_container_t *container, 整数 dirfd, const char *name, struct device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close 整数 fd = -1;
  proc_fd_path_t fd_path;
  const char *what = "new";
  bool exists = false;
  struct stat st;
  整数 procfd;
  整数 ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  もし (UNLIKELY (ret < 0))
    {
      もし (errno != EEXIST)
        戻る crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  もし (UNLIKELY (fd < 0))
    戻る crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  もし (UNLIKELY ((st.st_mode & S_IFMT) != type))
    戻る crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  もし (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    戻る crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  もし (exists)
    戻る 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  もし (UNLIKELY (procfd < 0))
    戻る procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "fchown `%s`", device->path);

  戻る 1;
}

整数
libcrun_create_dev (libcrun_container_t *container, 整数 devfd, 整数 srcfd,
                    struct device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  整数 ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  const char *fullname = device->path;
  const char *rootfs = get_private_data (container)->rootfs;
  もし (is_empty_string (fullname))
    戻る crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free char *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  もし (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  const char *rel_dev = relative_path_under_dev (normalized_path);

  もし (binds)
    {
      cleanup_close 整数 fd = -1;

      もし (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          もし (UNLIKELY (fd < 0))
            {
              もし (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              もし (UNLIKELY (fd < 0))
                戻る crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      否则
        {
          const char *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          もし (UNLIKELY (fd < 0))
            戻る fd;
        }

      もし (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          もし (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              もし (LIKELY (ret >= 0))
                戻る 0;
            }
          否则
            crun_error_release (err);
        }

      {
        cleanup_close 整数 mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        もし (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            もし (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, NULL);
                もし (LIKELY (ret == 0))
                  戻る 0;
              }
            否则
              crun_error_release (err);
          }
        否则
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, NULL, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, NULL, LABEL_MOUNT, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  否则
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      もし (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          もし (ret <= 0)
            戻る ret;
        }
      否则
        {
          cleanup_close 整数 dirfd = -1;
          cleanup_free char *dirname = NULL;
          char *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          もし (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          もし (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              もし (UNLIKELY (dirfd < 0))
                戻る crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          否则
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              もし (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              もし (UNLIKELY (dirfd < 0))
                戻る dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          もし (ret <= 0)
            戻る ret;
        }
    }
  戻る 0;
}

struct symlink_s
{
  const char *path;
  const char *target;
  bool force;
};

static struct symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { NULL, NULL, false } };

static 整数
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  整数 ret;
  size_t i;
  struct device_s *it;
  cleanup_close 整数 devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  const char *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map struct libcrun_fd_map *dev_fds = NULL;
  cleanup_close_map struct libcrun_fd_map *needed_fds = NULL;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = NULL;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = NULL;

  もし (! def || ! def->linux)
    戻る 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  もし (UNLIKELY (devfd < 0))
    戻る devfd;

  ために (i = 0; i < def->linux->devices_len; i++)
    {
      struct device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      もし (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  ために (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close 整数 srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
      もし (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  ために (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      もし (UNLIKELY (ret < 0))
        {
          整数 saved_errno = errno;

          もし (errno == EEXIST && ! symlinks[i].force)
            continue;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          もし (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              もし (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              もし (ret < 0 && errno == EBUSY)
                {
                  cleanup_close 整数 tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  もし (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      もし (umount2 (procpath, MNT_DETACH) == 0)
                        goto retry_unlink;
                    }
                }
              もし (ret == 0)
                goto retry_symlink;
            }
          戻る crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  もし (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
static const char *
next_path_component (const char *p, size_t *len)
{
  ために (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      もし (*len != 1 || p[0] != '.')
        戻る p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
static bool
same_path (const char *a, const char *b)
{
  size_t la, lb;

  ために (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      もし (la != lb || memcmp (a, b, la) != 0)
        戻る false;
      もし (la == 0)
        戻る true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
static bool
is_duplicate_path (char **paths, size_t i)
{
  size_t j;

  もし (paths[i] == NULL)
    戻る false;

  ために (j = 0; j < i; j++)
    もし (paths[j] && same_path (paths[j], paths[i]))
      戻る true;

  戻る false;
}

static 整数
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  整数 ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  もし (! def || ! def->linux)
    戻る 0;

  ために (i = 0; i < def->linux->masked_paths_len; i++)
    {
      もし (is_duplicate_path (def->linux->masked_paths, i))
        continue;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  ために (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  戻る 0;
}

static 整数
do_pivot (libcrun_container_t *container, const char *rootfs, libcrun_error_t *err)
{
  cleanup_close 整数 oldrootfd = -1;
  cleanup_close 整数 newrootfd = -1;
  整数 ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  もし (UNLIKELY (oldrootfd < 0))
    戻る crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  もし (UNLIKELY (newrootfd < 0))
    戻る crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_root (".", ".");
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, NULL, -1, ".", NULL, MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = umount2 (".", MNT_DETACH);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "umount2 oldroot");

  do
    {
      ret = umount2 (".", MNT_DETACH);
      もし (ret < 0 && errno == EINVAL)
        break;
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "umount2 oldroot");
  } ながら (ret == 0);

  ret = chdir ("/");
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "chdir to newroot");

  戻る 0;
}

static 整数
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, char **data, libcrun_error_t *err)
{
  const char *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close 整数 fd = -1;
  struct stat st;
  整数 ret;

  もし (*data != NULL && strstr (*data, "mode="))
    戻る 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  もし (fd < 0)
    {
      もし (crun_error_get_errno (err) != ENOENT)
        戻る fd;

      crun_error_release (err);
      戻る 0;
    }
  ret = fstat (fd, &st);
  もし (ret < 0)
    戻る crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  戻る 0;
}

static 整数
get_default_flags (libcrun_container_t *container, const char *destination, char **data)
{
  もし (strcmp (destination, "/proc") == 0)
    戻る 0;
  もし (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      戻る MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  もし (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      戻る MS_NOEXEC | MS_STRICTATIME;
    }
  もし (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      戻る MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  もし (strcmp (destination, "/dev/mqueue") == 0)
    戻る MS_NOEXEC | MS_NOSUID | MS_NODEV;
  もし (strcmp (destination, "/dev/pts") == 0)
    {
      もし (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      否则
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      戻る MS_NOEXEC | MS_NOSUID;
    }
  もし (strcmp (destination, "/sys") == 0)
    戻る MS_NOEXEC | MS_NOSUID | MS_NODEV;

  戻る 0;
}

static const char *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  戻る find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

static 整数
check_valid_no_follow_file_system (整数 fd, const char *destination, libcrun_error_t *err)
{
  struct statfs sfs;

  整数 ret = fstatfs (fd, &sfs);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "statfs `%s`", destination);

  switch (sfs.f_type)
    {
    case PROC_SUPER_MAGIC:
    case SYSFS_MAGIC:
    case DEVPTS_SUPER_MAGIC:
      戻る crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  戻る 0;
}

static 整数
safe_create_symlink (整数 rootfsfd, const char *rootfs, const char *target, const char *destination, libcrun_error_t *err)
{
  cleanup_close 整数 parent_dir_fd = -1;
  cleanup_free char *buffer = NULL;
  char *part;
  整数 ret;

  もし (is_empty_string (destination))
    戻る crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  もし (UNLIKELY (parent_dir_fd < 0))
    戻る parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  もし (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      もし (errno == EEXIST)
        {
          cleanup_free char *link = NULL;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          もし (UNLIKELY (len < 0))
            戻る len;

          もし ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            戻る 0;

          戻る crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      戻る crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  戻る 0;
}

static 整数
handle_copy_symlink (libcrun_container_t *container, const char *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     const char *link_target, libcrun_error_t *err)
{
  cleanup_free char *target = NULL;

  もし (link_target == NULL)
    {
      整数 old_root_fd = get_old_root_fd (get_private_data (container));
      const char *source = mount->source;
      ssize_t len;

      もし (old_root_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_root_fd, source + 1, &target, 0, err);
      否则
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      もし (UNLIKELY (len < 0))
        戻る len;
      link_target = target;
    }

  戻る safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

static 整数
prepare_sysfs_or_proc_mount (libcrun_container_t *container, const char *target,
                             整数 *targetfd, libcrun_error_t *err)
{
  整数 ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  もし (UNLIKELY (ret < 0))
    {
      もし (errno == ENOENT)
        {
          もし (strchr (target, '/'))
            戻る crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      否则 もし (errno == ENOTDIR)
        戻る crun_make_error (err, errno, "the target `/%s` is invalid", target);

      もし (ret < 0)
        戻る crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  戻る 0;
}

static 整数
handle_tmpcopyup (libcrun_container_t *container, const char *rootfs, const char *target,
                  整数 copy_from_fd, libcrun_error_t *err)
{
  整数 destfd, ret;
  cleanup_close 整数 tmpfd = copy_from_fd;

  target = consume_slashes (target);

  もし (is_empty_string (target))
    戻る crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  もし (UNLIKELY (destfd < 0))
    戻る crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  戻る ret;
}

static 整数
get_mount_label_how (const char *type, bool is_sysfs_or_proc)
{
  もし (is_sysfs_or_proc)
    戻る LABEL_NONE;

  もし (strcmp (type, "mqueue") == 0)
    戻る LABEL_XATTR;

  戻る LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
static unsigned long
get_bind_mount_locked_flags (整数 source_mountfd, const char *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  struct statfs sfs;
  size_t i;
  整数 ret;

  もし (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  否则
    ret = statfs (source, &sfs);
  もし (UNLIKELY (ret < 0))
    戻る 0;

  ために (i = 0; i < mount->options_len; i++)
    {
      const struct propagation_flags_s *prop;

      もし (mount->options[i] == NULL)
        continue;

      prop = libcrun_str2mount_flags (mount->options[i]);
      もし (prop == NULL || (prop->extra_flags & OPTION_RECURSIVE))
        continue;

      もし (prop->clear)
        cleared |= prop->flags;
      否则
        cleared &= ~prop->flags;
    }

  戻る sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
static const char *
get_path_in_rootfs (const char *rootfs, const char *path)
{
  size_t len;

  もし (rootfs == NULL || path == NULL)
    戻る NULL;

  len = strlen (rootfs);
  ながら (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  もし (len == 0)
    戻る NULL;

  もし (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    戻る NULL;

  戻る consume_slashes (path + len);
}

static 整数
process_single_mount (libcrun_container_t *container, const char *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      struct libcrun_fd_map *mount_fds, size_t mount_index,
                      const char *systemd_cgroup_v1, libcrun_error_t *err)
{
  const char *target = consume_slashes (mount->destination);
  cleanup_close 整数 source_mountfd = -1;
  cleanup_free char *data = NULL;
  char *type;
  char *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close 整数 copy_from_fd = -1;
  cleanup_close 整数 targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  整数 no_remount = 0;
  整数 ret;

  もし (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  もし (mount->options == NULL)
    flags = get_default_flags (container, mount->destination, &data);
  否则
    {
      size_t j;

      ために (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  もし (type == NULL && (flags & MS_BIND) == 0)
    戻る crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  もし ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      const char *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      もし (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          もし (UNLIKELY (source_mountfd < 0))
            戻る crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  もし ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  もし (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      もし (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      もし (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  もし (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      const char *path = mount->source;

      もし ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        戻る crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      もし (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, NULL);
        }
      否则 もし (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      否则 もし (path[0] == '/')
        {
          整数 old_root_fd = get_old_root_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          もし (old_root_fd >= 0)
            ret = get_file_type_at (old_root_fd, &src_mode, nofollow, (path[1] == '\0') ? NULL : path + 1);
          否则
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      否则
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "cannot stat `%s`", path);

      もし (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          整数 bind_dirfd = AT_FDCWD;
          const char *bind_source = mount->source;
          整数 old_root_fd = get_old_root_fd (get_private_data (container));

          もし (old_root_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_root_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          source_mountfd = ret;
        }
    }

  もし (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      const char *cached_target = NULL;

      もし (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      mounted = true;
    }
  否则 もし (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  否则
    {
      bool is_dir = S_ISDIR (src_mode);

      もし (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
      否则
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
          targetfd = ret;
        }
    }

  もし (extra_flags & OPTION_TMPCOPYUP)
    {
      もし (strcmp (type, "tmpfs") != 0)
        戻る crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      もし (UNLIKELY (copy_from_fd < 0))
        {
          もし (errno != ENOTDIR)
            戻る crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  もし (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, NULL);
      もし (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          もし (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, NULL, source_mountfd, target, NULL, remaining_flags, data, LABEL_NONE | no_remount, err);
              もし (UNLIKELY (ret < 0))
                戻る ret;
            }

          mounted = true;

          もし (is_empty_string (target))
            {
              整数 new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              もし (UNLIKELY (new_rootfsfd < 0))
                戻る crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      否则 もし (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          戻る crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      否则 もし (get_private_data (container)->host_root_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          戻る crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      否则 もし (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  もし (! mounted)
    {
      もし (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
      否则 もし (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
      否则
        {
          整数 extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
    }

  もし (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (rec_clear || rec_set)
    {
      const bool is_dir = S_ISDIR (src_mode);
      cleanup_close 整数 dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      もし (UNLIKELY (dfd < 0))
        戻る crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}

static 整数
do_mounts (libcrun_container_t *container, const char *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  const char *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map struct libcrun_fd_map *mount_fds = NULL;
  size_t i;
  整数 ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = NULL;

  ために (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  戻る 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
整数
libcrun_container_do_bind_mount (libcrun_container_t *container, char *mount_source, char *mount_destination, char **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  const char *target = consume_slashes (mount_destination);
  cleanup_free char *data = NULL;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close 整数 targetfd = -1;
  整数 is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  const char *rootfs = get_private_data (container)->rootfs;
  整数 ret;

  もし ((get_private_data (container)->rootfsfd < 0) || (rootfs == NULL))
    戻る crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  もし (mount_options == NULL)
    flags = get_default_flags (container, mount_destination, &data);
  否则
    {
      size_t j;

      ために (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  もし (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  もし (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      もし (UNLIKELY (is_dir < 0))
        戻る is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
整数
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, 整数 *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close 整数 notify_fd = -1;
  cleanup_free char *host_notify_socket_path = NULL;
  cleanup_free char *state_dir = NULL;
  char *host_path = NULL;
  整数 ret;

  もし (container && get_private_data (container)->host_notify_socket_path)
    {
      const char *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", NULL);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  もし (host_path == NULL)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_root, context->id, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", NULL);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  もし (UNLIKELY (notify_fd < 0))
    戻る notify_fd;

  もし (UNLIKELY (chmod (host_path, 0777) < 0))
    戻る crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  もし (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (void) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  戻る 1;
#否则
  (void) context;
  (void) container;
  (void) err;
  *notify_socket_out = -1;
  戻る 0;
#endif
}

#ifdef HAVE_SYSTEMD
static 整数
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  整数 ret;
  const char *notify_socket = container->context->notify_socket;
  cleanup_free char *host_notify_socket_path = NULL;
  cleanup_free char *container_notify_socket_path = NULL;
  cleanup_free char *state_dir = NULL;
  uid_t container_root_uid = -1;
  gid_t container_root_gid = -1;
  整数 notify_socket_tree_fd;

  もし (notify_socket == NULL)
    戻る 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_root : NULL),
                                     (container->context ? container->context->id : NULL), err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", NULL);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", NULL);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = mkdir (host_notify_socket_path, 0700);
  もし (ret < 0)
    戻る crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  もし (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_root_in_the_userns (container->container_def, 0, 0, &container_root_uid, &container_root_gid);
      もし (container_root_uid != ((uid_t) -1) && container_root_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_root_uid, container_root_gid);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_root_uid, container_root_gid);
        }
    }

  notify_socket_tree_fd = syscall_open_tree (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  もし (notify_socket_tree_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_tree_fd = notify_socket_tree_fd;
  否则 もし (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  否则 もし (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  否则
    /* some other error */
    戻る crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = NULL;
  戻る 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
static 整数
do_finalize_notify_socket (libcrun_container_t *container, const char *rootfs, libcrun_error_t *err)
{
  整数 ret;
  cleanup_free char *host_notify_socket_path = NULL;
  cleanup_free char *container_notify_socket_path = NULL;
  cleanup_free char *container_notify_socket_path_dir_alloc = NULL;
  const char *container_notify_socket_path_dir = NULL;
  cleanup_close 整数 notify_socket_tree_fd = -1;
  cleanup_close 整数 targetfd = -1;
  整数 did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = NULL;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = NULL;

  もし (host_notify_socket_path == NULL || container_notify_socket_path == NULL)
    戻る 0;

  もし (rootfs == NULL)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  もし (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  targetfd = ret;

  notify_socket_tree_fd = get_private_data (container)->notify_socket_tree_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_tree_fd = -1;

  もし (notify_socket_tree_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_tree_fd, targetfd, NULL);
      もし (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          もし (UNLIKELY (targetfd < 0))
            戻る targetfd;

          ret = do_mount (container, NULL, targetfd, container_notify_socket_path_dir, NULL,
                          MS_REC | MS_PRIVATE, NULL, LABEL_NONE, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
      否则 もし (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      否则
        戻る crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_tree_fd,
                                container_notify_socket_path_dir);
    }

  もし (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, NULL,
                      MS_BIND | MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}

static 整数
make_parent_mount_private (const char *rootfs, libcrun_error_t *err)
{
  cleanup_close 整数 rootfsfd = -1;
  struct stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  もし (UNLIKELY (rootfsfd < 0))
    戻る crun_make_error (err, errno, "open `%s`", rootfs);

  もし (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    戻る crun_make_error (err, errno, "fstat `%s`", rootfs);

  ために (;;)
    {
      struct stat cur_st;
      libcrun_error_t tmp_err = NULL;
      整数 ret;
      cleanup_close 整数 parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      もし (ret == 0)
        戻る 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (NULL, proc_path, NULL, MS_PRIVATE, NULL);
      もし (ret == 0)
        戻る 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      もし (parentfd < 0)
        {
          整数 saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          もし (ret != 0)
            戻る crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          戻る crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      もし (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        戻る crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      もし (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        break;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  戻る crun_make_error (err, 0, "make `%s` private", rootfs);
}

整数
libcrun_set_mounts (struct container_entrypoint_s *entrypoint_args, libcrun_container_t *container, const char *rootfs, set_mounts_cb_t cb, void *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  整数 is_user_ns = 0;
  整数 ret = 0;

  もし (rootfs == NULL || def->mounts == NULL)
    戻る 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  もし (UNLIKELY (ret < 0))
    戻る crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  もし (def->root->readonly)
    {
      struct remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      整数 fd;

      fd = dup (get_private_data (container)->rootfsfd);
      もし (UNLIKELY (fd < 0))
        戻る crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, NULL, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  もし (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      もし (UNLIKELY (is_user_ns < 0))
        戻る is_user_ns;
    }

  もし (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  もし (cb)
    {
      ret = cb (cb_data, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = NULL;
      const char *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (void) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る 0;
}

static 整数
umount_or_hide (const char *target, libcrun_error_t *err)
{
  整数 ret;

  ret = umount2 (target, MNT_DETACH);
  もし (UNLIKELY (ret < 0))
    {
      整数 saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      もし (errno == EINVAL)
        {
          ret = mount (NULL, target, "tmpfs", 0, "size=0k");
          もし (LIKELY (ret == 0))
            戻る 0;
        }

      戻る crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  戻る ret;
}

static 整数
move_root (const char *rootfs, libcrun_error_t *err)
{
  整数 ret;

  ret = chdir (rootfs);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = umount_or_hide ("/proc", err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "chdir to `%s`", rootfs);

  戻る 0;
}

整数
libcrun_finalize_mounts (struct container_entrypoint_s *entrypoint_args, libcrun_container_t *container, const char *rootfs,
                         set_mounts_cb_t after_pivot_cb, void *cb_data, libcrun_error_t *err)
{
  整数 ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  もし (UNLIKELY (ret < 0))
    戻る crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_root_fd));

  もし (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      もし (get_private_data (container)->no_pivot)
        {
          ret = move_root (rootfs, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
      否则
        {
          ret = do_pivot (container, rootfs, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }

      ret = do_mount (container, NULL, -1, "/", NULL,
                      get_private_data (container)->rootfs_propagation,
                      NULL, LABEL_MOUNT, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = chdir ("/");
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "chdir to `/`");
    }

  もし (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  ret = finalize_mounts (container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る 0;
}

static 整数
maybe_open_tree_namespace (const char *rootfs, 整数 *out_fd, libcrun_error_t *err)
{
  cleanup_close 整数 rootfs_fd = -1;
  整数 tree_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  もし (UNLIKELY (rootfs_fd < 0))
    戻る crun_make_error (err, errno, "open `%s`", rootfs);

  tree_fd = syscall_open_tree (rootfs_fd, "",
                               OPEN_TREE_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  もし (tree_fd < 0)
    {
      もし (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        戻る 0;
      戻る crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = tree_fd;
  戻る 0;
}

static struct libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  struct libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  もし (mount_fds == NULL)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  戻る mount_fds;
}

static 整数
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free char *mnt_data = NULL;
  unsigned long mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  const char *label_type = NULL;
  const char *label_val = NULL;
  const char *fstype;
  整数 mnt_fd;
  size_t j;

  もし (mount->type == NULL)
    戻る -1;

  fstype = mount->type;

  ために (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  もし (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      整数 extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      もし (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          もし (label_type == NULL)
            crun_error_release (err);
          否则
            label_val = def->linux->mount_label;
        }
    }

  もし (strcmp (fstype, "cgroup") == 0)
    {
      整数 cgroup_mode = libcrun_get_cgroup_mode (err);
      もし (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  もし (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, NULL, NULL, mnt_data);

  戻る mnt_fd;
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
struct mnt_id_req_s
{
  uint32_t size;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

struct statmount_s
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
  char str[];
};

static 整数
syscall_listmount (struct mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned 整数 flags)
{
#ifdef __NR_listmount
  戻る (整数) syscall (__NR_listmount, req, ids, nr, flags);
#否则
  (void) req;
  (void) ids;
  (void) nr;
  (void) flags;
  errno = ENOSYS;
  戻る -1;
#endif
}

static 整数
syscall_statmount (struct mnt_id_req_s *req, struct statmount_s *buf, size_t size, unsigned 整数 flags)
{
#ifdef __NR_statmount
  戻る (整数) syscall (__NR_statmount, req, buf, size, flags);
#否则
  (void) req;
  (void) buf;
  (void) size;
  (void) flags;
  errno = ENOSYS;
  戻る -1;
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

  ために (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      もし (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != NULL && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          break;
        }
    }
  もし (! has_absolute_bind)
    戻る false;

  ids = xmalloc (capacity * sizeof (uint64_t));
  ために (;;)
    {
      struct mnt_id_req_s req = {
        .size = sizeof (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      整数 ret;

      もし (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * sizeof (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      もし (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        戻る true;
      もし (ret == 0)
        break;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  ために (i = 0; i < n_ids; i++)
    {
      char buffer[4096] __attribute__ ((aligned (8)));
      struct statmount_s *sm = (struct statmount_s *) buffer;
      struct mnt_id_req_s req = {
        .size = sizeof (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      const char *root, *point;
      size_t j;

      もし (syscall_statmount (&req, sm, sizeof (buffer), 0) < 0)
        continue;
      もし ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        continue;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      root = sm->str + sm->mnt_root;
      もし (! has_prefix (root, "mnt:["))
        continue;

      point = sm->str + sm->mnt_point;

      ために (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          const char *source = def->mounts[j]->source;
          size_t source_len;

          もし (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            continue;
          もし (source == NULL || source[0] != '/')
            continue;

          source_len = strlen (source);
          ながら (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          もし (strncmp (point, source, source_len) != 0)
            continue;

          /* The source itself is a pinned mount namespace.  */
          もし (point[source_len] == '\0')
            戻る true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          もし (recursive && (source_len == 1 || point[source_len] == '/'))
            戻る true;
        }
    }

  戻る false;
}

static bool
can_use_open_tree_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  struct libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  もし (has_hooks || has_userns)
    戻る false;

  もし (mounts_hit_pinned_mount_namespace (def))
    戻る false;

  もし (container->context->no_open_tree_namespace)
    戻る false;

  {
    libcrun_error_t tmp_err = NULL;
    整数 in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    もし (in_userns != 0)
      戻る false;
  }

  mount_fds = get_fd_map (container);
  ために (i = 0; i < def->mounts_len; i++)
    {
      もし (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          もし (! has_cached_target)
            戻る false;
        }
    }

  戻る true;
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

  もし (mount->type == NULL || strcmp (mount->type, "tmpfs") != 0)
    戻る false;

  ために (i = 0; i < mount->options_len; i++)
    もし (mount->options[i] != NULL && has_prefix (mount->options[i], "mode="))
      戻る false;

  戻る true;
}

static 整数
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, char **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close 整数 rootfsfd = -1;
  cleanup_close 整数 tree_fd = -1;
  libcrun_error_t tmp_err = NULL;
  bool use_open_tree = false;
  size_t i;
  整数 ret;

  もし (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, NULL, NULL, NULL, NULL);

  もし ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  free (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  ために (i = 0; i < def->mounts_len; i++)
    {
      struct libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close 整数 mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      もし (mount_fds && mount_fds->fds[i] >= 0)
        continue;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      もし (def->mounts[i]->source != NULL
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free char *mnt_data = NULL;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          ために (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          もし (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free char *target = NULL;
              libcrun_error_t tmp_err = NULL;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              もし (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              もし (get_private_data (container)->copy_symlink_targets == NULL)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * sizeof (char *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = NULL;
            }
        }

      もし (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          もし (mount_fds && def->mounts[i]->source != NULL && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == NULL)
            {
              libcrun_error_t tmp_err = NULL;
              unsigned long propagation = MS_PRIVATE;

              もし (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              もし (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          continue;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      もし (mnt_fd < 0)
        {
          crun_error_release (err);
          continue;
        }

      もし (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close 整数 destfd = -1;
          struct stat st;

          もし (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              もし (UNLIKELY (rootfsfd < 0))
                戻る crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          もし (destfd < 0)
            crun_error_release (&tmp_err);
          否则 もし (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              整数 procfd = get_procfd (get_private_data (container), err);
              もし (UNLIKELY (procfd < 0))
                戻る procfd;

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
  もし (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  もし (! get_private_data (container)->joined_mount_ns
      && can_use_open_tree_namespace (container))
    use_open_tree = true;

  もし (use_open_tree)
    {
      ret = maybe_open_tree_namespace (*rootfs, &tree_fd, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  もし (tree_fd >= 0)
    preopen_needed_devs_and_notify (container);

  もし (tree_fd >= 0)
    {
      ret = setns (tree_fd, CLONE_NEWNS);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (NULL, "/", NULL, MS_REMOUNT | MS_BIND, NULL);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, NULL, -1, "/", NULL, MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = do_mount (container, NULL, -1, "/", NULL, rootfs_propagation, NULL, LABEL_MOUNT, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_root_switched = true;
      free (*rootfs);
      *rootfs = xstrdup ("/");
    }
  否则
    {
      もし (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, NULL, -1, "/", NULL, rootfs_propagation, NULL, LABEL_MOUNT, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = make_parent_mount_private (*rootfs, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, NULL, MS_BIND | MS_REC | MS_PRIVATE, NULL, LABEL_MOUNT, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  もし (! container->context->force_no_cgroup)
    {
      整数 cgroup_mode = libcrun_get_cgroup_mode (err);
      もし (UNLIKELY (cgroup_mode < 0))
        戻る cgroup_mode;

      もし (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          char *unified_cgroup_path = NULL;
          整数 procfd = get_procfd (get_private_data (container), err);
          もし (UNLIKELY (procfd < 0))
            戻る procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  もし (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = NULL;
      戻る ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = NULL;

  もし (tree_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  もし (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  戻る 0;
}

整数
libcrun_do_pivot_root (libcrun_container_t *container, bool no_pivot, char **rootfs, libcrun_error_t *err)
{
  整数 ret;

  もし (*rootfs == NULL)
    戻る 0;

  もし (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      もし (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "chdir to `/`");
        }
    }
  否则
    {
      ret = chroot (*rootfs);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "chdir to `/`");

      free (*rootfs);
      *rootfs = xstrdup ("/");
    }

  戻る 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
整数
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  struct stat statbuf;
  cleanup_close 整数 fd;
  整数 ret;
  整数 i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  もし (UNLIKELY (fd == -1))
    戻る crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ために (i = 0; i <= 2; i++)
    {
      もし (UNLIKELY (fstat (i, &statbuf) == -1))
        戻る crun_make_error (err, errno, "stat fd `%d`", i);
      もし (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          もし (UNLIKELY (dup2 (fd, i) == -1))
            戻る crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  戻る 0;
}

static 整数
uidgidmap_helper (char *helper, pid_t pid, const char *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  char pid_fmt[16];
  char *args[MAX_ARGS + 1];
  char *next;
  cleanup_free char *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  整数 ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, sizeof (pid_fmt), "%d", pid);
  もし (UNLIKELY (ret >= (整数) sizeof (pid_fmt)))
    戻る crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  ながら (nargs < MAX_ARGS)
    {
      char *p = strsep (&next, " \n");
      もし (next == NULL)
        break;
      args[nargs++] = p;
    }
  args[nargs++] = NULL;

  ret = run_process (args, err);
  もし (ret < 0)
    戻る ret;
  もし (ret > 0)
    戻る crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  戻る 0;
}

static 整数
newgidmap (pid_t pid, const char *map_file, libcrun_error_t *err)
{
  戻る uidgidmap_helper ("newgidmap", pid, map_file, err);
}

static 整数
newuidmap (pid_t pid, const char *map_file, libcrun_error_t *err)
{
  戻る uidgidmap_helper ("newuidmap", pid, map_file, err);
}

static 整数
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  整数 ret;
  cleanup_close 整数 fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  もし (UNLIKELY (fd < 0))
    戻る fd;

  ret = TEMP_FAILURE_RETRY (write (fd, "deny", 4));
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  戻る 0;
}

static 整数
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  整数 ret;
  cleanup_free char *content = NULL;

  もし (get_private_data (container)->deny_setgroups)
    戻る 0;

  もし (container->container_def->annotations)
    {
      const char *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      もし (annotation)
        戻る strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close 整数 fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    もし (fd < 0)
      ret = fd;
    否则
      ret = read_all_fd (fd, "self/setgroups", &content, NULL, err);
  }
  もし (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      もし (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          戻る 1;
        }
      戻る ret;
    }

  戻る strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

整数
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = NULL;
  size_t additional_gids_len = 0;
  整数 can_do_setgroups;
  整数 ret;

  もし (process != NULL && process->user != NULL)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  もし (UNLIKELY (can_do_setgroups < 0))
    戻る can_do_setgroups;

  もし (can_do_setgroups == 0)
    {
      const char *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      もし (annotation && strcmp (annotation, "0") != 0)
        戻る 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      ために (i = 0; i < additional_gids_len; i++)
        もし (additional_gids[i] != process->user->gid)
          戻る crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (整数) additional_gids[i]);
      戻る 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "setgroups");

  戻る 0;
}

整数
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  もし (get_private_data (container)->unshare_cgroupns)
    {
      整数 ret = unshare (CLONE_NEWCGROUP);
      もし (UNLIKELY (ret < 0))
        {
          もし (errno != EINVAL)
            戻る crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  戻る 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
整数
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  もし (len != 1)
    戻る 0;

  もし (mappings[0]->size != 1)
    戻る 0;

  もし (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    戻る 0;

  戻る 1;
}

static 整数
write_id_map (libcrun_container_t *container, pid_t pid, const char *map_name,
              整数 (*idmap_helper) (pid_t, const char *, libcrun_error_t *), const char *helper_name,
              bool warn_on_failure, const char *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  整数 ret = 0;

  もし (container->host_uid)
    ret = idmap_helper (pid, map, err);
  もし (container->host_uid == 0 || ret < 0)
    {
      cleanup_close 整数 fd = -1;

      もし (ret < 0)
        {
          もし (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      もし (UNLIKELY (fd < 0))
        戻る fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      もし (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free char *single_mapping = NULL;
          crun_error_release (err);

          もし (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              もし (UNLIKELY (ret < 0))
                戻る ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          もし (UNLIKELY (fd < 0))
            戻る fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  戻る ret;
}

整数
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free char *uid_map = NULL;
  cleanup_free char *gid_map = NULL;
  size_t uid_map_len = 0, gid_map_len = 0;
  整数 ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  もし ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    戻る 0;

  もし (! def || ! def->linux)
    戻る 0;

  もし (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  否则
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      uid_map_len = (size_t) ret;
      もし (uid_map == NULL)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
    }

  もし (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  否则
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      gid_map_len = (size_t) ret;
      もし (gid_map == NULL)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

struct all_caps_s
{
  unsigned long effective[2];
  unsigned long permitted[2];
  unsigned long inheritable[2];
  unsigned long ambient[2];
  unsigned long bounding[2];
};

static 整数
has_cap_on (整数 cap, long unsigned *caps)
{
  もし (cap < 32)
    戻る CAP_TO_MASK_0 (cap) & caps[0];
  戻る (CAP_TO_MASK_1 (cap) & caps[1]);
}

static unsigned long cap_last_cap;

整数
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close 整数 fd = -1;
  整数 ret;
  char buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  もし (fd < 0)
    戻る fd;
  ret = TEMP_FAILURE_RETRY (read (fd, buffer, sizeof (buffer) - 1));
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, NULL, 10);
  もし (errno != 0)
    戻る crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  戻る 0;
}

static 整数
set_required_caps (struct all_caps_s *caps, uid_t uid, gid_t gid, 整数 no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  整数 ret;
  struct __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  struct __user_cap_data_struct data[2] = { { 0 } };

  もし (cap_last_cap == 0)
    戻る crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  ために (cap = 0; cap <= cap_last_cap; cap++)
    もし (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        もし (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          戻る crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  もし (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    戻る crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  もし (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    戻る crun_make_error (err, errno, "prctl reset ambient");

  ために (cap = 0; cap <= cap_last_cap; cap++)
    もし (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        もし (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          戻る crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  もし (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "prctl reset KEEPCAPS");

      もし (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          ために (cap = 0; cap <= cap_last_cap; cap++)
            もし (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "capset");
        }
    }
#endif

  もし (no_new_privs)
    もし (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      戻る crun_make_error (err, errno, "prctl set no new privs");

  戻る 0;
}

static 整数
read_caps (unsigned long caps[2], char **values, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  ために (i = 0; i < len; i++)
    {
      cap_value_t cap;
      もし (cap_from_name (values[i], &cap) < 0)
        戻る crun_make_error (err, 0, "unknown capability `%s`", values[i]);

      もし (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      否则
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#否则
  caps[0] = 0;
  caps[1] = 0;
#endif
  戻る 0;
}

整数
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  もし (proc->selinux_label)
    戻る set_selinux_label (container, proc->selinux_label, now, err);

  戻る 0;
}

整数
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  もし (proc->apparmor_profile)
    戻る set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  戻る 0;
}

整数
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  整数 no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  struct all_caps_s caps = {};
  整数 ret;

  もし (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  戻る set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

struct rlimit_s
{
  const char *name;
  整数 value;
};

struct rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
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

static 整数
get_rlimit_resource (const char *name)
{
  struct rlimit_s *it;
  ために (it = rlimits; it->name; it++)
    もし (strcmp (it->name, name) == 0)
      戻る it->value;
  戻る -1;
}

整数
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  ために (i = 0; i < len; i++)
    {
      ために (j = i + 1; j < len; j++)
        {
          もし (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            戻る crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  ために (i = 0; i < len; i++)
    {
      struct rlimit limit;
      char *type = new_rlimits[i]->type;
      整数 resource = get_rlimit_resource (type);
      もし (UNLIKELY (resource < 0))
        戻る crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      もし (UNLIKELY (setrlimit (resource, &limit) < 0))
        戻る crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  戻る 0;
}

整数
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  整数 has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  整数 ret;
  もし (def->hostname == NULL || def->hostname[0] == '\0')
    戻る 0;
  もし (! has_uts)
    戻る crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "sethostname");
  戻る 0;
}

整数
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  整数 has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  整数 ret;
  もし (is_empty_string (def->domainname))
    戻る 0;
  もし (! has_uts)
    戻る crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "setdomainname");
  戻る 0;
}

整数
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close 整数 fd = -1;
  整数 ret;
  char oom_buffer[16];
  もし (def->process == NULL || ! def->process->oom_score_adj_present)
    戻る 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, sizeof (oom_buffer), "%i", def->process->oom_score_adj);
  もし (UNLIKELY (ret >= (整数) sizeof (oom_buffer)))
    戻る crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  もし (fd < 0)
    戻る fd;
  ret = TEMP_FAILURE_RETRY (write (fd, oom_buffer, strlen (oom_buffer)));
  もし (ret < 0)
    戻る crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  戻る 0;
}

const char *sysctlRequiringIPC[] = {
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

static 整数
validate_sysctl (const char *original_key, const char *original_value, const char *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  const char *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  もし (name[0] == '/' || path_has_dot_dot_component (name))
    戻る crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_key);

  もし (has_prefix (name, "fs/mqueue/"))
    {
      もし (namespaces_created & CLONE_NEWIPC)
        戻る 0;

      namespace = "IPC";
      goto fail;
    }

  もし (has_prefix (name, "kernel/"))
    {
      size_t i;

      ために (i = 0; sysctlRequiringIPC[i]; i++)
        もし (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            もし (namespaces_created & CLONE_NEWIPC)
              戻る 0;

            namespace = "IPC";
            goto fail;
          }

      もし (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          もし (! is_empty_string (def->domainname) && (strcmp (original_value, def->domainname) != 0))
            戻る crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_key);

          もし (namespaces_created & CLONE_NEWUTS)
            戻る 0;

          namespace = "UTS";
          goto fail;
        }

      もし (strcmp (name, "kernel/hostname") == 0)
        戻る crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_key);
    }
  もし (has_prefix (name, "net/"))
    {
      もし (namespaces_created & CLONE_NEWNET)
        戻る 0;

      namespace = "network";
      goto fail;
    }

  戻る crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_key);

fail:
  戻る crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_key, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
static char *
sysctl_error_reason (const char *name, 整数 namespaces_created, 整数 errno_)
{

  もし (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    戻る xstrdup ("are all the IDs mapped in the user namespace?");

  戻る NULL;
}

整数
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close 整数 dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  もし (def->linux == NULL || def->linux->sysctl == NULL || def->linux->sysctl->len == 0)
    戻る 0;

  ために (i = 0; i < def->linux->namespaces_len; i++)
    {
      整数 value;

      value = libcrun_find_namespace (def->linux->namespaces[i]->type);
      もし (UNLIKELY (value < 0))
        戻る crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= value;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  もし (UNLIKELY (dirfd < 0))
    戻る dirfd;

  ために (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free char *name = NULL;
      cleanup_close 整数 fd = -1;
      整数 ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      もし (def->linux->sysctl->keys[i] == NULL || def->linux->sysctl->values[i] == NULL)
        戻る crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_key_to_proc_path (def->linux->sysctl->keys[i]);

      ret = validate_sysctl (def->linux->sysctl->keys[i], def->linux->sysctl->values[i], name, namespaces_created, def, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      もし (UNLIKELY (fd < 0))
        戻る crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (write (fd, def->linux->sysctl->values[i], strlen (def->linux->sysctl->values[i])));
      もし (UNLIKELY (ret < 0))
        {
          整数 saved_errno = errno;
          cleanup_free char *reason = NULL;

          reason = sysctl_error_reason (def->linux->sysctl->keys[i], namespaces_created, saved_errno);
          戻る crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  戻る 0;
}

static 整数
open_terminal (char **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close 整数 fd = -1;
  uid_t uid = 0;
  整数 ret;

  もし (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  もし (UNLIKELY (fd < 0))
    戻る fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  戻る ret;
}

char *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  戻る get_private_data (container)->external_descriptors;
}

整数
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  const char *buf = NULL;
  json_gen_ctx *gen = NULL;
  size_t buf_len;
  整数 ret;
  整数 i;

  もし (! json_gen_init (&gen, NULL))
    戻る crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  もし (UNLIKELY (ret != json_gen_status_ok))
    goto json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  ために (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      char link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      もし (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          もし (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          否则
            {
              json_gen_free (gen);
              戻る crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      もし (UNLIKELY (ret != json_gen_status_ok))
        goto json_error;
    }

  ret = json_gen_array_close (gen);
  もし (UNLIKELY (ret != json_gen_status_ok))
    goto json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  もし (UNLIKELY (ret != json_gen_status_ok))
    goto json_error;

  もし (buf)
    {
      char *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  戻る 0;

json_error:
  もし (gen)
    json_gen_free (gen);
  戻る json_gen_error_to_crun_error (ret, err);
}

整数
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free char *pty = NULL;
  cleanup_close 整数 fd = -1;
  整数 ret;

  もし (def->process == NULL || ! def->process->terminal)
    戻る 0;

  fd = open_terminal (&pty, def->process, err);
  もし (UNLIKELY (fd < 0))
    戻る fd;

  もし (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", NULL, MS_BIND, NULL, LABEL_MOUNT, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  否则
    {
      ret = unlink ("/dev/console");
      もし (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          もし (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", NULL, MS_BIND, NULL, LABEL_MOUNT, err);
              もし (UNLIKELY (ret < 0))
                戻る ret;

              戻る get_and_reset (&fd);
            }
          戻る crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  戻る get_and_reset (&fd);
}

static bool
read_error_from_sync_socket (整数 sync_socket_fd, 整数 *error, char **str)
{
  cleanup_free char *b = NULL;
  size_t size;
  整数 code = 0;
  整数 ret;

  もし (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (read (sync_socket_fd, &code, sizeof (code)));
      もし (UNLIKELY (ret != (整数) sizeof (code)))
        戻る false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (read (sync_socket_fd, &size, sizeof (size)));
  もし (UNLIKELY (ret != (整数) sizeof (size)))
    戻る false;

  もし (size == 0)
    戻る false;

  もし (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (read (sync_socket_fd, b, size));
  もし (UNLIKELY (ret < 0))
    戻る false;

  b[ret] = '\0';

  *str = b;
  b = NULL;
  戻る true;
}

static bool
send_error_to_sync_socket (整数 sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  整数 ret;
  整数 code;
  char *msg;

  もし (err == NULL || *err == NULL)
    戻る false;

  code = crun_error_get_errno (err);

  もし (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, "1", 1));
      もし (UNLIKELY (ret < 0))
        戻る false;
    }

  ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, &code, sizeof (code)));
  もし (UNLIKELY (ret < 0))
    戻る false;

  msg = (*err)->msg;
  size = strlen (msg) + 1;
  ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, &size, sizeof (size)));
  もし (UNLIKELY (ret < 0))
    戻る false;

  ret = TEMP_FAILURE_RETRY (write (sync_socket_fd, msg, size));
  もし (UNLIKELY (ret < 0))
    戻る false;

  戻る true;
}

static inline 整数
send_success_to_sync_socket (整数 sync_socket, libcrun_error_t *err)
{
  const 整数 success = 0;
  整数 ret;

  ret = TEMP_FAILURE_RETRY (write (sync_socket, &success, sizeof (success)));
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "write to sync socket");

  戻る 0;
}

static __attribute__ ((noreturn)) void
send_error_to_sync_socket_and_die (整数 sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  char *msg;

  もし (err == NULL || *err == NULL)
    _safe_exit (EXIT_FAILURE);

  もし (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      msg = (*err)->msg;
      libcrun_fail_with_error (errno, "%s", msg);
    }
  _safe_exit (EXIT_FAILURE);
}

static 整数
expect_success_from_sync_socket (整数 sync_fd, libcrun_error_t *err)
{
  cleanup_free char *err_str = NULL;
  整数 res = 1;
  整数 ret;

  ret = TEMP_FAILURE_RETRY (read (sync_fd, &res, sizeof (res)));
  もし (UNLIKELY (ret != sizeof (res)))
    戻る crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  もし (res == 0)
    戻る 0;

  もし (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      もし (! is_empty_string (err_str))
        戻る crun_make_error (err, res, "%s", err_str);
    }

  戻る crun_make_error (err, res, "read from sync socket");
}

static 整数
join_namespaces (runtime_spec_schema_config_schema *def, 整数 *namespaces_to_join, 整数 n_namespaces_to_join,
                 整数 *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  整数 ret;
  整数 i;

  ために (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free char *cwd = NULL;
      整数 orig_index = namespaces_to_join_index[i];
      整数 value;

      もし (namespaces_to_join[i] < 0)
        continue;

      /* Skip the user namespace.  */
      value = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      もし (value == CLONE_NEWUSER)
        continue;

      もし (value == CLONE_NEWNS)
        {
          cwd = getcwd (NULL, 0);
          もし (UNLIKELY (cwd == NULL))
            戻る crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], value);
      もし (UNLIKELY (ret < 0))
        {
          もし (ignore_join_errors)
            continue;
          戻る crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      もし (value == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  戻る 0;
}

#define MAX_NAMESPACES 10

struct init_status_s
{
  /* fd to the namespace to join.  */
  整数 fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  整数 index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  整数 value[MAX_NAMESPACES];
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
  整数 userns_index;
  /* def->linux->namespaces userns.  */
  整数 userns_index_origin;

  /* All namespaces created/joined by the container.  */
  整数 all_namespaces;

  /* What namespaces are still missing to be created.  */
  整数 namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  整数 idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  整数 idx_timens_to_join_immediately;
};

void
cleanup_free_init_statusp (struct init_status_s *ns)
{
  size_t i;

  もし (ns == NULL)
    戻る;

  ために (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

static 整数
configure_init_status (struct init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  ために (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  ために (i = 0; i < def->linux->namespaces_len; i++)
    {
      整数 value = libcrun_find_namespace (def->linux->namespaces[i]->type);
      もし (UNLIKELY (value < 0))
        戻る crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      もし (ns->all_namespaces & value)
        戻る crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= value;

      もし (def->linux->namespaces[i]->path == NULL)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= value;
        }
      否则
        {
          整数 fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          もし (ns->fd_len >= MAX_NAMESPACES)
            戻る crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          もし (UNLIKELY (fd < 0))
            戻る crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          もし (value == CLONE_NEWUSER)
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

  もし (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  戻る 0;
}

/* Detect if root is available in the container.  */
static bool
root_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  ために (i = 0; i < len; i++)
    もし (mappings[i]->container_id == 0)
      戻る true;

  戻る false;
}

static struct libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  struct libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  もし (dev_fds == NULL)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  戻る dev_fds;
}

#define NUM_NEEDED_DEVS (sizeof (needed_devs) / sizeof (needed_devs[0]) - 1)

static struct libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  struct libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  もし (fds == NULL)
    {
      fds = make_libcrun_fd_map (NUM_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  戻る fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  もし (recursive)
    *recursive = false;

  もし (src_nofollow != NULL)
    *src_nofollow = false;

  ために (i = 0; i < mnt->options_len; i++)
    {
      もし (mnt->options[i] == NULL)
        continue;

      もし (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          もし (src_nofollow == NULL || *src_nofollow)
            break;
        }
      もし (strcmp (mnt->options[i], "rbind") == 0)
        {
          もし (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          もし (src_nofollow == NULL || *src_nofollow)
            break;
        }
      もし (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  戻る ret;
}

static char *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  ために (i = 0; i < mnt->options_len; i++)
    {
      もし (mnt->options[i] == NULL)
        continue;

      もし (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          戻る mnt->options[i];
        }
      もし (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          戻る mnt->options[i];
        }
    }
  戻る NULL;
}

static 整数
open_mount_type (const char *type, 整数 *out_fd, libcrun_error_t *err)
{
  cleanup_close 整数 fsopen_fd = -1;
  cleanup_close 整数 newfs_fd = -1;
  整数 ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  もし (UNLIKELY (fsopen_fd < 0))
    戻る crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, NULL, NULL, 0);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  もし (UNLIKELY (newfs_fd < 0))
    戻る crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  戻る 0;
}

static 整数
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, 整数 *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close 整数 newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  struct mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close 整数 fd = -1;
  const char *idmap_option;
  bool recursive = false;
  const char *options = NULL;
  bool has_mappings;
  整数 ret;
  char *extra_msg = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != NULL);
  もし (has_mappings_out)
    *has_mappings_out = has_mappings;
  もし (! has_mappings)
    戻る 0;

  もし ((mnt->uid_mappings == NULL) != (mnt->gid_mappings == NULL))
    戻る crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  もし (idmap_option)
    {
      options = strchr (idmap_option, '=');
      もし (options)
        {
          /* Skip the '=' itself.  */
          options++;
          もし (options[0] == '\0')
            options = NULL;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;
  もし (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  もし (UNLIKELY (fd < 0))
    戻る fd;

  もし (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_tree (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      もし (UNLIKELY (newfs_fd < 0))
        戻る crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  否则
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  もし (UNLIKELY (ret < 0))
    {
      もし (errno == EINVAL)
        {
          extra_msg = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      戻る crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_msg);
    }

  *out_fd = get_and_reset (&newfs_fd);
  戻る 0;
}

static uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  もし (! def || ! def->linux)
    goto exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  もし (! mappings)
    goto exit;

  ために (i = 0; i < len; i++)
    {
      もし (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        戻る id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  戻る is_uid ? get_overflow_uid () : get_overflow_gid ();
}

static 整数
precreate_device (libcrun_container_t *container, 整数 devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  char name[64];
  mode_t type;
  dev_t dev;
  整数 ret;

  ret = snprintf (name, sizeof (name), "%zu", i);
  もし (UNLIKELY (ret >= (整数) sizeof (name)))
    戻る crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "chown `%s`", device->path);

  戻る get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

static 整数
send_mounts (整数 sync_socket_host, struct libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  整数 ret;

  ret = TEMP_FAILURE_RETRY (write (sync_socket_host, &how_many, sizeof (how_many)));
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "write to sync socket");

  ために (i = 0; i < total; i++)
    {
      もし (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (char *) &i, sizeof (i), err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
    }
  戻る 0;
}

static 整数
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, 整数 sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map struct libcrun_fd_map *mount_fds = NULL;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free char *rootfs = NULL;
  size_t how_many = 0;
  size_t i;
  整数 ret;

  もし (def->mounts_len == 0)
    戻る 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  もし (def->root && def->root->path)
    rootfs = realpath (def->root->path, NULL);

  もし (! has_userns)
    {
      整数 is_in_userns = check_running_in_user_namespace (err);
      もし (UNLIKELY (is_in_userns < 0))
        戻る is_in_userns;

      has_userns = is_in_userns > 0;
    }

  ために (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      整数 mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      もし (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == NULL)
        {
          unsigned long propagation = 0;

          もし (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, NULL, NULL, NULL, NULL) & ALL_PROPAGATIONS_NO_REC;
          もし (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          もし (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      もし (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  戻る send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

static 整数
prepare_and_send_dev_mounts (libcrun_container_t *container, 整数 sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map struct libcrun_fd_map *dev_fds = NULL;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close 整数 current_mountns = -1;
  cleanup_free char *state_dir = NULL;
  cleanup_free char *devs_path = NULL;
  cleanup_close 整数 devs_mountfd = -1;
  cleanup_close 整数 targetfd = -1;
  const char *context_type = NULL;
  const char *label = NULL;
  size_t how_many = 0;
  size_t i;
  整数 ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  もし (def->linux == NULL || def->linux->devices_len == 0)
    戻る 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  もし (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    戻る send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_root : NULL),
                                     (container->context ? container->context->id : NULL), err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", NULL);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = mkdir (devs_path, 0700);
  もし (UNLIKELY (ret < 0 && errno != EEXIST))
    戻る crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  もし (UNLIKELY (current_mountns < 0))
    戻る current_mountns;

  ret = unshare (CLONE_NEWNS);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL);
  もし (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      goto restore_mountns;
    }

  もし (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      もし (UNLIKELY (context_type == NULL))
        {
          ret = -1;
          goto restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", NULL, context_type, label, NULL);
  もし (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      goto restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  もし (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      goto restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, NULL);
  もし (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      goto restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  もし (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      goto restore_mountns;
    }

  ために (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      もし (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          continue;
        }

      dev_fds->fds[i] = ret;

      もし (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  もし (ns_changed && current_mountns >= 0)
    {
      整数 setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      もし (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          戻る crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  戻る ret;
}

static 整数
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, 整数 sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map struct libcrun_fd_map *fds = NULL;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (NUM_NEEDED_DEVS);

  もし (! has_userns || geteuid () > 0)
    戻る send_mounts (sync_socket_host, fds, 0, NUM_NEEDED_DEVS, err);

  ために (i = 0; i < NUM_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      整数 fd = syscall_open_tree (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      もし (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  戻る send_mounts (sync_socket_host, fds, how_many, NUM_NEEDED_DEVS, err);
}

static 整数
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, 整数 sync_socket_host, libcrun_error_t *err)
{
  整数 ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る 0;
}

static 整数
receive_mounts (struct libcrun_fd_map *fds, 整数 sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  整数 ret;

  もし (fds->nfds == 0)
    戻る 0;

  ret = TEMP_FAILURE_RETRY (read (sync_socket_container, &how_many, sizeof (how_many)));
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "read from sync socket");

  ために (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (char *) &index, sizeof (index), err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
      もし (index >= fds->nfds)
        戻る crun_make_error (err, 0, "invalid mount data received");

      もし (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  戻る 0;
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
static 整数
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  整数 ret;

  もし (! container->host_has_cap_sys_ptrace)
    戻る 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  戻る 0;
}

static 整数
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  整数 ret;

  もし (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool root_mapped = false;

      もし (def->linux->uid_mappings_len != 0)
        {
          root_mapped = root_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          もし (! root_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      もし (def->linux->gid_mappings_len != 0)
        {
          root_mapped = root_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          もし (! root_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  もし (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    戻る crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "setresgid to `%d`", gid);

  戻る 0;
}

static bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  戻る mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, NULL, NULL);
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
static 整数
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  struct libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close 整数 socket_parent = -1;
  cleanup_close 整数 socket_child = -1;
  cleanup_close 整数 root_fd = -1;
  cleanup_close 整数 cwd_fd = -1;
  size_t i, missing = 0;
  整数 saved_errno;
  整数 sockets[2];
  pid_t pid;
  整数 ret;

  もし (def->root == NULL || def->root->path == NULL)
    戻る 0;

  ために (i = 0; i < def->mounts_len; i++)
    もし (is_proc_mount (def->mounts[i]))
      missing++;
  もし (missing == 0)
    戻る 1;

  root_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  もし (UNLIKELY (root_fd < 0))
    戻る crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  もし (UNLIKELY (cwd_fd < 0))
    戻る crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->root->path);
  もし (UNLIKELY (ret < 0))
    戻る 0;

  pid = fork ();
  saved_errno = errno;
  もし (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      もし (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (void) unshare (CLONE_NEWNS);

      ために (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close 整数 fd = -1;

          もし (! is_proc_mount (def->mounts[i]))
            continue;

          fd = open_mount_of_type (container, def->mounts[i], err);
          もし (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (char *) &i, sizeof (i), err);
          もし (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  もし (UNLIKELY (fchdir (root_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    戻る crun_make_error (err, errno, "restore the root directory");

  もし (UNLIKELY (pid < 0))
    戻る crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  ながら (missing > 0)
    {
      libcrun_error_t tmp_err = NULL;

      ret = receive_fd_from_socket_with_payload (socket_parent, (char *) &i, sizeof (i), &tmp_err);
      もし (ret < 0)
        {
          crun_error_release (&tmp_err);
          break;
        }
      もし (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          break;
        }

      もし (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, NULL, 0);

  戻る missing == 0 ? 1 : 0;
}

static 整数
init_container (libcrun_container_t *container, 整数 sync_socket_container, struct init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  struct libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close 整数 timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  整数 ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      もし (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      もし (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      もし (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          delayed_pidns_fork = ret > 0;
        }

      もし (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          ret = TEMP_FAILURE_RETRY (write (sync_socket_container, &new_pid, sizeof (new_pid)));
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "write to sync socket");
        }
      否则
        {
          new_pid = fork ();
          もし (UNLIKELY (new_pid < 0))
            戻る crun_make_error (err, errno, "fork");
        }

      もし (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          もし (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (write (sync_socket_container, &new_pid, sizeof (new_pid)));
          もし (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  ret = libcrun_set_oom (container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  もし ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      ために (i = 0; i < def->mounts_len; i++)
        {
          整数 fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          もし (def->mounts[i]->type == NULL)
            continue;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          もし (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, NULL, NULL, NULL);
          もし (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, NULL, NULL, NULL);

          もし (fd >= 0)
            {
              もし (mount_fds->fds[i] >= 0)
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
  もし (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      もし (UNLIKELY (timens_offsets_fd < 0))
        戻る timens_offsets_fd;
    }

  もし (init_status->all_namespaces & CLONE_NEWUSER)
    {
      もし (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }

      もし (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
      否则
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "unshare");
    }

  もし (def->linux->time_offsets)
    {
      char fmt_buffer[128];
      整数 fd = timens_offsets_fd;

      もし (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, sizeof (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          もし (UNLIKELY (ret >= (整数) sizeof (fmt_buffer)))
            戻る crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long 整数) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = write (fd, fmt_buffer, strlen (fmt_buffer));
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      もし (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long 整数) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, sizeof (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          もし (UNLIKELY (ret >= (整数) sizeof (fmt_buffer)))
            戻る crun_make_error (err, 0, "internal error: static buffer too small");

          ret = write (fd, fmt_buffer, strlen (fmt_buffer));
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  もし (init_status->must_fork)
    {
      もし (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          delayed_pidns_fork = ret > 0;
        }

      もし (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          もし (UNLIKELY (pid_container < 0))
            戻る crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      もし (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          ret = TEMP_FAILURE_RETRY (write (sync_socket_container, &pid_container, sizeof (pid_container)));
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "write to sync socket");

          もし (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  戻る get_private_data (container)->delayed_pidns_fork;
}

整数
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close 整数 client_fd = -1;
  cleanup_close 整数 pidfd = -1;
  const char *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  もし (v == NULL)
    戻る 0;

  pidfd = syscall_pidfd_open (pid, 0);
  もし (UNLIKELY (pidfd < 0))
    戻る crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  もし (UNLIKELY (client_fd < 0))
    戻る client_fd;

  戻る send_fd_to_socket (client_fd, pidfd, err);
}

static bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  もし (process == NULL || process->exec_cpu_affinity == NULL)
    戻る false;
  戻る (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, void *args,
                             整数 *sync_socket_out, struct libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) struct init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close 整数 sync_socket_container = -1;
  char *notify_socket_env = NULL;
  cleanup_close 整数 sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close 整数 restore_pidns = -1;
  整数 first_clone_args = 0;
  整数 sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  整数 ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  もし (def->root)
    {
      ret = do_notify_socket (container, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  もし (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  ret = libcrun_set_oom (container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->root ? container->container_def->root->path : NULL, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  ために (i = 0; i < init_status.fd_len; i++)
    {
      switch (init_status.value[i])
        {
        case CLONE_NEWIPC:
          もし (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          break;

        case CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          もし (! new_userns)
            init_status.must_fork = true;
          否则
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          break;

        case CLONE_NEWTIME:
          もし (! new_userns)
            init_status.must_fork = true;
          否则
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          break;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  もし ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  否则 もし ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  もし (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  もし (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      struct _clone3_args clone3_args;
      memset (&clone3_args, 0, sizeof (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      もし (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  もし (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, NULL);
      もし (UNLIKELY (pid < 0))
        戻る crun_make_error (err, errno, "clone");
    }

  もし (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = close_and_reset (&sync_socket_container);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_tree_fd));

      もし (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          ret = TEMP_FAILURE_RETRY (read (sync_socket_host, &new_pid, sizeof (new_pid)));
          もし (UNLIKELY (ret != sizeof (new_pid)))
            戻る crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          もし (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          否则
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, NULL, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }

      もし (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }

      もし ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }

      もし (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          ret = TEMP_FAILURE_RETRY (read (sync_socket_host, &grandchild, sizeof (grandchild)));
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;

          もし (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          否则
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, NULL, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      もし (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }

      pid_to_clean = 0;
      戻る pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  もし (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  もし (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  否则
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      もし (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->msg);
    }

  /* Jump into the specified entrypoint.  */
  もし (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  もし (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->msg);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  もし (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

static 整数
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, 整数 sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, const char *sub_cgroup,
                            整数 *terminal_fd, libcrun_error_t *err)
{
  整数 ret;
  char res;
  pid_t pid;
  cleanup_close 整数 sync_fd = sync_socket_fd;

  もし (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (read (sync_fd, &res, 1));
  もし (UNLIKELY (ret != 1))
    戻る crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  もし (res != '0')
    戻る crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (read (sync_fd, &pid, sizeof (pid)));
  もし (UNLIKELY (ret != sizeof (pid)))
    戻る crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, NULL, 0);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "waitpid for exec child pid");

  もし (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (need_move_to_cgroup)
    {
      cleanup_cgroup_status struct libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free char *final_cgroup = NULL;

      もし (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, NULL);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = libcrun_set_io_priority (pid, process, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (write (sync_fd, &ret, sizeof (ret)));
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "write to sync socket");

  もし (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      もし (UNLIKELY (ret < 0))
        {
          整数 err_code = 0;
          cleanup_free char *err_str = NULL;

          もし (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              もし (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  戻る crun_make_error (err, err_code, "%s", err_str);
                }
            }

          戻る crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  戻る pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
static 整数
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close 整数 pidfd_pid_to_join = -1;
  整数 all_flags = 0;
  size_t i;
  整数 ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  もし (def->linux && def->linux->namespaces)
    {
      ために (i = 0; i < def->linux->namespaces_len; i++)
        もし (! is_empty_string (def->linux->namespaces[i]->path))
          戻る 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  もし (UNLIKELY (pidfd_pid_to_join < 0))
    戻る 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;
  もし (ret == 0)
    戻る crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  ために (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].value;

  ret = setns (pidfd_pid_to_join, all_flags);
  もし (UNLIKELY (ret < 0))
    戻る 0;

  戻る 1;
}

static 整数
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  整数 fds_joined[MAX_NAMESPACES] = {
    0,
  };
  整数 fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  整数 ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;
  /* Nothing left to do if the namespaces were joined.  */
  もし (LIKELY (ret > 0))
    戻る 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  もし (def->linux->namespaces_len >= MAX_NAMESPACES)
    戻る crun_make_error (err, 0, "invalid configuration");

  ために (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free char *ns_path = NULL;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      もし (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          もし (crun_error_get_errno (err) == ENOENT)
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
  もし (UNLIKELY (ret < 0))
    goto exit;
  もし (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      goto exit;
    }

  ために (i = 0; namespaces[i].ns_file; i++)
    {
      もし (namespaces[i].value == CLONE_NEWUSER)
        continue;

      ret = setns (fds[i], 0);
      もし (ret == 0)
        fds_joined[i] = 1;
    }
  ために (i = 0; namespaces[i].ns_file; i++)
    {
      もし (fds_joined[i])
        continue;
      ret = setns (fds[i], 0);
      もし (ret == 0)
        fds_joined[i] = 1;
    }
  ために (i = 0; namespaces[i].ns_file; i++)
    {
      もし (fds_joined[i])
        continue;
      ret = setns (fds[i], 0);
      もし (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          ために (j = 0; j < def->linux->namespaces_len; j++)
            {
              もし (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  break;
                }
            }
          もし (! found)
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
  ために (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  戻る ret;
}

整数
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      const char *sub_cgroup,
                      整数 detach,
                      runtime_spec_schema_config_schema_process *process,
                      整数 *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  整数 ret;
  整数 sync_socket_fd[2];
  cleanup_close 整数 cgroup_dirfd = -1;
  cleanup_close 整数 sync_fd = -1;
  struct _clone3_args clone3_args;
  bool need_move_to_cgroup;

  もし (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "set child subreaper");
    }

  もし (path_has_dot_dot_component (sub_cgroup))
    戻る crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  もし (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free char *final_cgroup = NULL;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, NULL);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status struct libcrun_cgroup_status *cgroup_status = NULL;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    もし (get_force_cgroup_v1_annotation (container) == NULL)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        もし (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, sizeof (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  もし (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  否则
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  もし (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  /* On errors, fall back to fork().  */
  もし (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      もし (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          goto exit;
        }
    }

  もし (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      戻る join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  もし (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (write (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->msg);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  もし (UNLIKELY (pid < 0))
    {
      整数 saved_errno = errno;
      TEMP_FAILURE_RETRY (write (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  もし (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (write (sync_fd, "0", 1));
      もし (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (write (sync_fd, &pid, sizeof (pid)));
      もし (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  否则
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free char *pty = NULL;
      整数 r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (read (sync_fd, &r, sizeof (r)));
      もし (UNLIKELY (ret != sizeof (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      もし (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      もし (terminal_fd)
        {
          cleanup_close 整数 ptmx_fd = -1;

          ret = set_id_init (container, err);
          もし (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          もし (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          もし (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  戻る 0;

exit:
  もし (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  もし (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  戻る ret;
}

整数
libcrun_linux_container_update (libcrun_container_status_t *status, const char *state_root, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status struct libcrun_cgroup_status *cgroup_status = NULL;

  cgroup_status = libcrun_cgroup_make_status (status);

  戻る libcrun_update_cgroup_resources (cgroup_status, state_root, resources, err);
}

static 整数
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, const bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status struct libcrun_cgroup_status *cgroup_status = NULL;

  cgroup_status = libcrun_cgroup_make_status (status);

  戻る libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

整数
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  戻る libcrun_container_pause_unpause_linux (status, true, err);
}

整数
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  戻る libcrun_container_pause_unpause_linux (status, false, err);
}

整数
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  整数 ret;

  もし (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  否则 もし (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  否则
    戻る crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, 0, "personality to `%s`", p->domain);

  戻る 0;
}

整数
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  整数 ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close 整数 sockfd = -1;

  ために (i = 0; i < def->linux->namespaces_len; i++)
    {
      整数 value = libcrun_find_namespace (def->linux->namespaces[i]->type);
      もし (UNLIKELY (value < 0))
        戻る crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      もし (value == CLONE_NEWNET && def->linux->namespaces[i]->path == NULL)
        {
          configure_network = true;
          break;
        }
    }

  もし (! configure_network)
    戻る 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  もし (LIKELY (sockfd >= 0))
    {
      struct ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  否则
    {
      struct nlmsghdr *hdr_recv;
      char buf[sizeof (struct nlmsghdr) + sizeof (struct nlmsgerr)];
      struct sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      struct sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      struct nlmsghdr hdr = {
        .nlmsg_len = sizeof (struct nlmsghdr) + sizeof (struct ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      struct ifinfomsg msg = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((struct nlmsghdr *) buf) = hdr;
      *((struct ifinfomsg *) (buf + sizeof (struct nlmsghdr))) = msg;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      もし (UNLIKELY (sockfd < 0))
        戻る crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (struct sockaddr *) &addr, sizeof (addr));
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, sizeof (buf), 0, (struct sockaddr *) &addr_to, sizeof (addr_to));
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, sizeof (buf), 0, NULL, NULL);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (struct nlmsghdr *) buf;

      もし (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          struct nlmsgerr *err_data = (struct nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          もし (err_data->error < 0)
            戻る crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  戻る 0;
}

/* Protection for attacks like CVE-2019-5736.  */
整数 ensure_cloned_binary ();
__attribute__ ((constructor)) static void
libcrun_rexec (void)
{
  もし (ensure_cloned_binary () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

整数
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  戻る libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

整数
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  整数 ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
static 整数
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, 整数 signal, libcrun_error_t *err)
{
  整数 ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  もし (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      /* The pid is not valid anymore, return an error.  */
      もし (ret == 0)
        {
          errno = ESRCH;
          戻る crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, signal);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "kill container");
  戻る 0;
}

整数
libcrun_kill_linux (libcrun_container_status_t *status, 整数 signal, libcrun_error_t *err)
{
  整数 ret;
  cleanup_close 整数 pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  もし (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      もし (errno == ENOSYS)
        戻る libcrun_kill_linux_no_pidfd (status, true, signal, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      もし (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        戻る crun_make_error (err, ESRCH, "process not running");
      戻る crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  /* The pid is not valid anymore, return an error.  */
  もし (ret == 0)
    {
      errno = ESRCH;
      戻る crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, signal, NULL, 0);
  もし (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      もし (errno == ENOSYS)
        戻る libcrun_kill_linux_no_pidfd (status, false, signal, err);
      戻る crun_make_error (err, errno, "send signal to pidfd");
    }

  戻る 0;
}

static const char *
libcrun_get_intelrdt_name (const char *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  もし (def == NULL || def->linux == NULL || def->linux->intel_rdt == NULL || def->linux->intel_rdt->clos_id == NULL)
    {
      もし (explicit)
        *explicit = false;
      戻る ctr_name;
    }

  もし (explicit)
    *explicit = true;
  戻る def->linux->intel_rdt->clos_id;
}

整数
libcrun_apply_intelrdt (const char *ctr_name, libcrun_container_t *container, pid_t pid, 整数 actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = NULL;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  const char *name;
  整数 ret;

  もし (container)
    def = container->container_def;

  もし (def == NULL || def->linux == NULL || def->linux->intel_rdt == NULL)
    戻る 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  もし (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      もし (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          もし (UNLIKELY (ret < 0))
            goto fail;

          has_monitoring = true;
        }
    }

  もし (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      もし (UNLIKELY (ret < 0))
        goto fail;
    }

  もし (actions & LIBCRUN_INTELRDT_MOVE)
    {
      const char *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : NULL;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      もし (UNLIKELY (ret < 0))
        goto fail;
    }

  戻る 0;

fail:
  もし (has_monitoring)
    {
      libcrun_error_t tmp_err = NULL;
      整数 tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      もし (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  もし (created)
    {
      libcrun_error_t tmp_err = NULL;
      整数 tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      もし (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  戻る ret;
}

整数
libcrun_destroy_intelrdt (const char *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  const char *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  もし (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      整数 ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      もし (ret < 0)
        戻る ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  もし (explicit)
    戻る 0;

  戻る resctl_destroy (clos_id, err);
}

整数
libcrun_update_intel_rdt (const char *ctr_name, libcrun_container_t *container, const char *l3_cache_schema, const char *mem_bw_schema, char *const *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = NULL;
  const char *name;

  もし (container)
    def = container->container_def;

  もし (def == NULL || def->linux == NULL || def->linux->intel_rdt == NULL)
    戻る 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, NULL);

  戻る resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
整数
libcrun_safe_chdir (const char *path, libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  整数 ret;

  ret = chdir (path);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  もし (ret == 0 || buffer[0] != '/')
    {
      もし (chdir ("/") < 0)
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
#define UNREACHABLE_LEN ((整数) sizeof (UNREACHABLE) - 1)

      もし ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        戻る crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      戻る crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  戻る 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
static 整数
run_callback_in_namespace (整数 mntns_fd, 整数 (*callback) (void *, libcrun_error_t *), void *arg, libcrun_error_t *err)
{
  整数 wait_status = 0;
  pid_t pid;
  整数 ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  もし (UNLIKELY (pid < 0))
    戻る crun_make_error (err, errno, "vfork");

  もし (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      もし (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      もし (UNLIKELY (ret < 0))
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
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  戻る get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
static 整数
check_container_userns (libcrun_container_t *container, 整数 userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close 整数 expected_fd = -1;
  cleanup_close 整数 parent_fd = -1;
  struct stat st, expected_st;
  const char *path = NULL;
  size_t i;
  整数 ret;

  もし (def->linux)
    ために (i = 0; i < def->linux->namespaces_len; i++)
      もし (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == NULL;
          break;
        }

  もし (path == NULL)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      もし (UNLIKELY (expected_fd < 0))
        戻る expected_fd;
    }
  否则
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      もし (UNLIKELY (expected_fd < 0))
        戻る crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  もし (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      もし (UNLIKELY (parent_fd < 0))
        戻る crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "fstat user namespace");

  もし (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    戻る crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  戻る 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
static 整数
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, 整数 *mntns_fd, 整数 *owner_fd, libcrun_error_t *err)
{
  cleanup_close 整数 mntns = -1;
  cleanup_close 整数 owner = -1;
  整数 ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  もし (UNLIKELY (mntns < 0))
    戻る mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (ret == 0)
    戻る crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  もし (UNLIKELY (owner < 0))
    戻る crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  戻る 0;
}

static 整数
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, 整数 (*callback) (void *, libcrun_error_t *), void *arg, libcrun_error_t *err)
{
  cleanup_close 整数 mntns_fd = -1;
  cleanup_close 整数 owner_fd = -1;
  整数 ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る run_callback_in_namespace (mntns_fd, callback, arg, err);
}

struct umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
static 整数
umount_nofollow (const char *destination, libcrun_error_t *err)
{
  cleanup_close 整数 parentfd = -1;
  cleanup_close 整数 cwdfd = -1;
  char buffer[PATH_MAX];
  const char *parent, *name;
  char *last_slash;
  size_t len;
  整数 saved_errno;
  整数 ret;

  もし (is_empty_string (destination))
    戻る crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  もし (UNLIKELY (len >= sizeof (buffer)))
    戻る crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  ながら (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  もし (last_slash == NULL)
    {
      parent = ".";
      name = buffer;
    }
  否则 もし (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  否则
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  もし (UNLIKELY (parentfd < 0))
    戻る crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  もし (UNLIKELY (cwdfd < 0))
    戻る crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  もし (UNLIKELY (fchdir (cwdfd) < 0))
    戻る crun_make_error (err, errno, "fchdir to the previous working directory");

  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, saved_errno, "umount `%s`", destination);

  戻る 0;
}

static 整数
do_umount_in_a_container (void *arg, libcrun_error_t *err)
{
  struct umount_in_a_container_args *args = arg;
  size_t i;
  整数 ret;

  ために (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}

struct mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  struct libcrun_fd_map *fds;
  整数 pidfd;
  整数 owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
static 整数
open_destination_nofollow (const char *destination, bool dir, libcrun_error_t *err)
{
  const uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  整数 fd;
  整数 ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  もし (LIKELY (fd >= 0))
    戻る fd;

  もし (errno != ENOENT)
    戻る crun_make_error (err, errno, "open `%s`", destination);

  もし (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  否则
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  もし (UNLIKELY (fd < 0))
    戻る crun_make_error (err, errno, "open `%s`", destination);

  戻る fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
static 整数
create_destinations_in_a_container (void *arg, libcrun_error_t *err)
{
  struct mount_in_a_container_args *args = arg;
  size_t i;
  整数 ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  もし (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "setresuid to `0`");
    }
  否则 もし (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      戻る crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  ために (i = 0; i < args->len; i++)
    {
      cleanup_close 整数 dest_fd = -1;
      struct stat st;

      ret = fstat (args->fds->fds[i], &st);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      もし (UNLIKELY (dest_fd < 0))
        戻る dest_fd;
    }

  戻る 0;
}

static 整数
do_mount_in_a_container (void *arg, libcrun_error_t *err)
{
  struct mount_in_a_container_args *args = arg;
  size_t i;
  整数 ret;

  ために (i = 0; i < args->len; i++)
    {
      cleanup_close 整数 dest_fd = -1;
      struct stat st;

      ret = fstat (args->fds->fds[i], &st);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      もし (UNLIKELY (dest_fd < 0))
        戻る dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, NULL);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  戻る 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
static 整数
send_mounts_from_container_namespaces (libcrun_container_t *container, 整数 pidfd, runtime_spec_schema_defs_mount **mounts,
                                       struct libcrun_fd_map *fds, size_t len, 整数 flags, 整数 owner_fd, 整数 sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  整数 ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  もし (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  否则 もし (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      戻る crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  もし (UNLIKELY (pid < 0))
    戻る crun_make_error (err, errno, "fork");

  もし (pid)
    {
      整数 wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      もし (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, NULL, 0));
          戻る crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ために (i = 0; i < len; i++)
    {
      cleanup_close 整数 fd = -1;

      もし (fds->fds[i] >= 0)
        continue;

      fd = open_mount_of_type (container, mounts[i], err);
      もし (UNLIKELY (fd < 0))
        戻る crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
static 整数
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, 整数 *flags, libcrun_error_t *err)
{
  static const struct
  {
    const char *name;
    整数 flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  ために (i = 0; i < sizeof (namespaces) / sizeof (namespaces[0]); i++)
    {
      cleanup_close 整数 self_fd = -1;
      cleanup_close 整数 target_fd = -1;
      struct stat self_st, target_st;
      cleanup_free char *self_path = NULL;
      整数 ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      もし (UNLIKELY (self_fd < 0))
        戻る self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      もし (UNLIKELY (target_fd < 0))
        戻る target_fd;

      ret = fstat (self_fd, &self_st);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      もし (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  戻る 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
static 整数
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, 整数 owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, struct libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close 整数 sync_fd = -1;
  cleanup_close 整数 pidfd = -1;
  整数 sync_socket_fd[2];
  bool needed = false;
  整数 flags;
  pid_t pid;
  size_t i;
  整数 ret;

  ために (i = 0; i < len; i++)
    {
      もし (fds->fds[i] >= 0)
        continue;

      もし (mounts[i]->type == NULL)
        戻る crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  もし (! needed)
    戻る 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  もし (UNLIKELY (pidfd < 0))
    戻る crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (ret == 0)
    戻る crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  もし (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      戻る ret;
    }

  もし (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      もし (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  ために (i = 0; i < len; i++)
    {
      もし (fds->fds[i] >= 0)
        continue;

      ret = receive_fd_from_socket (sync_fd, err);
      もし (UNLIKELY (ret < 0))
        {
          cleanup_free char *err_str = NULL;
          整数 err_code = 0;

          もし (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          否则
            ret = crun_error_wrap (err, "receive mount fd");
          break;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, NULL, 0));

  戻る ret;
}

static unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free char *data = NULL;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  もし (mount->options == NULL)
    戻る get_default_flags (container, mount->destination, &data);

  ために (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  戻る flags;
}

整数
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  struct mount_in_a_container_args args;
  cleanup_close_map struct libcrun_fd_map *fds = NULL;
  cleanup_close 整数 pidfd = -1;
  cleanup_close 整数 mntns_fd = -1;
  cleanup_close 整数 owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  整数 ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  fds = make_libcrun_fd_map (len);

  ために (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), NULL, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      もし (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          もし (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          もし (UNLIKELY (fds->fds[i] < 0))
            戻る fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ために (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  もし (UNLIKELY (ret > 0))
    戻る *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

整数
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  struct umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  戻る run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

struct restrict_mounts_in_a_container_args
{
  struct libcrun_mount_flags_s *mounts;
  size_t len;
  整数 rootfd;
};

static 整数
do_restrict_mounts_in_a_container (void *arg, libcrun_error_t *err)
{
  struct restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  整数 ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  もし (LIKELY (ret == 0))
    ret = chroot (".");
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "chroot to the container root");

  ために (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      const char *target = consume_slashes (args->mounts[i].target);
      cleanup_close 整数 fd = -1;
      struct statfs sfs;
      unsigned long current_flags;

      もし (flags == 0)
        continue;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      もし (UNLIKELY (fd < 0))
        {
          もし (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              continue;
            }
          戻る fd;
        }

      ret = fstatfs (fd, &sfs);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      もし ((current_flags & flags) == flags)
        continue;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      もし (LIKELY (ret == 0))
        continue;

      もし (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        戻る ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (NULL, target, NULL, MS_REMOUNT | MS_BIND | flags | current_flags, NULL);
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "remount `/%s`", target);
    }

  戻る 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
整数
libcrun_restrict_mounts_in_container (pid_t pid, struct libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  struct restrict_mounts_in_a_container_args args;
  cleanup_close 整数 mntns_fd = -1;
  cleanup_close 整数 rootfd = -1;
  char path[64];
  整数 ret;

  snprintf (path, sizeof (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  もし (UNLIKELY (mntns_fd < 0))
    戻る crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, sizeof (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  もし (UNLIKELY (rootfd < 0))
    戻る crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  もし (UNLIKELY (ret > 0))
    戻る *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  戻る ret;
}

整数
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close 整数 netns_fd = -1;
  size_t i;
  整数 ret;

  もし (def == NULL || def->linux == NULL || def->linux->net_devices == NULL)
    戻る 0;

  cleanup_free char *ns_path = NULL;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  もし (UNLIKELY (netns_fd < 0))
    戻る netns_fd;

  ために (i = 0; i < def->linux->net_devices->len; i++)
    {
      const char *new_name = def->linux->net_devices->values[i]->name ?: def->linux->net_devices->keys[i];

      ret = move_network_device (def->linux->net_devices->keys[i], new_name, netns_fd, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}
