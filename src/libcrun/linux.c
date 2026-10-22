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

#अन्तर्भाव <config.h>
#अन्तर्भाव "linux.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "status.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/mount.h>
#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#सचे defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#endif

#अन्तर्भाव <sys/prctl.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <sys/resource.h>
#अन्तर्भाव <sys/sysmacros.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <grp.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव "terminal.h"
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव "cgroup-utils.h"
#अन्तर्भाव "status.h"
#अन्तर्भाव "criu.h"
#अन्तर्भाव "scheduler.h"
#अन्तर्भाव "intelrdt.h"
#अन्तर्भाव "io_priority.h"
#अन्तर्भाव "net_device.h"

#अन्तर्भाव <sys/socket.h>
#अन्तर्भाव <libgen.h>
#अन्तर्भाव <sys/wait.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <sys/statvfs.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <sys/personality.h>
#अन्तर्भाव <net/if.h>
#अन्तर्भाव <sys/xattr.h>
#अन्तर्भाव <linux/netlink.h>
#अन्तर्भाव <linux/rtnetlink.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <linux/sched.h>
#अन्तर्भाव <linux/magic.h>
#अन्तर्भाव <linux/nsfs.h>

#अन्तर्भाव <json-c/json.h>
#अन्तर्भाव <ocispec/json_common.h>

#अन्तर्भाव "mount_flags.h"
#अन्तर्भाव "syscalls.h"

#ifndef RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#endif

#ifndef OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#endif

#ifndef OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#endif

#ifndef OPEN_वृक्षः_NAMESPACE
#  define OPEN_वृक्षः_NAMESPACE 2
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
स्थिर unsigned long
mount_flags_from_statfs (unsigned long f_flags)
{
  unsigned long flags = 0;

  सचे (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  सचे (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  सचे (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  सचे (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  सचे (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  सचे (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  सचे (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  सचे (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  सचे (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  पटिनिवत्त flags;
}

संरचना remount_s
{
  संरचना remount_s *next;
  वर्ण *target;
  पूर्णाङ्क targetfd;
  unsigned long flags;
  वर्ण *data;
};

संरचना private_data_s
{
  संरचना remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  पूर्णाङ्क unshare_flags;
  पूर्णाङ्क unshare_cgroupns;

  वर्ण *host_notify_socket_path;
  वर्ण *container_notify_socket_path;
  वर्ण *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  पूर्णाङ्क procfd;

  ध्रुव वर्ण *rootfs;
  पूर्णाङ्क rootfsfd;

  पूर्णाङ्क notify_socket_वृक्षः_fd;
  पूर्णाङ्क old_मूलम्_fd;

  संरचना libcrun_fd_map *mount_fds;
  संरचना libcrun_fd_map *dev_fds;
  संरचना libcrun_fd_map *needed_devs_fds;

  वर्ण **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  वर्ण *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  पूर्णाङ्क maskdir_fd;
  वर्ण *maskdir_proc_path;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_pivot;
  bool no_pivot;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_मूलम्_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  वर्ण *host_rootfs;
};

संरचना linux_namespace_s
{
  ध्रुव वर्ण *name;
  ध्रुव वर्ण *ns_file;
  पूर्णाङ्क मानम्;
};

स्थिर शून्य
cleanup_private_data (शून्य *private_data)
{
  संरचना private_data_s *p = private_data;

  सचे (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  सचे (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  सचे (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  सचे (p->old_मूलम्_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_मूलम्_fd));
  सचे (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  सचे (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  सचे (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  सचे (p->copy_symlink_targets)
    {
      size_t i;
      अत्थाय (i = 0; i < p->n_copy_symlink_targets; i++)
        मुक्त (p->copy_symlink_targets[i]);
      मुक्त (p->copy_symlink_targets);
    }
  मुक्त (p->unified_cgroup_path);
  मुक्त (p->host_notify_socket_path);
  मुक्त (p->container_notify_socket_path);
  मुक्त (p->external_descriptors);
  मुक्त (p->maskdir_proc_path);
  मुक्त (p->host_rootfs);
  मुक्त (p);
}

स्थिर संरचना private_data_s *
get_private_data (संरचना libcrun_container_s *container)
{
  सचे (container->private_data == रिक्त)
    {
      संरचना private_data_s *p = xmalloc0 (परिमाण (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_वृक्षः_fd = -1;
      p->old_मूलम्_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  पटिनिवत्त container->private_data;
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

स्थिर संरचना linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { रिक्त, रिक्त, 0 } };

स्थिर पूर्णाङ्क
get_and_reset (पूर्णाङ्क *old)
{
  पूर्णाङ्क tmp = *old;
  *old = -1;
  पटिनिवत्त tmp;
}

पूर्णाङ्क
libcrun_find_namespace (ध्रुव वर्ण *name)
{
  संरचना linux_namespace_s *it;
  अत्थाय (it = namespaces; it->name; it++)
    सचे (strcmp (it->name, name) == 0)
      पटिनिवत्त it->मानम्;
  पटिनिवत्त -1;
}

स्थिर पूर्णाङ्क
syscall_keyctl_join (ध्रुव वर्ण *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  पटिनिवत्त (पूर्णाङ्क) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

स्थिर पूर्णाङ्क
syscall_pidfd_open (pid_t pid, unsigned पूर्णाङ्क flags)
{
#सचे defined __NR_pidfd_open
  पटिनिवत्त (पूर्णाङ्क) syscall (__NR_pidfd_open, pid, flags);
#अञ्ञथा
  (शून्य) pid;
  (शून्य) flags;
  errno = ENOSYS;
  पटिनिवत्त -1;
#endif
}

स्थिर पूर्णाङ्क
syscall_pidfd_send_signal (पूर्णाङ्क pidfd, पूर्णाङ्क sig, siginfo_t *info, unsigned पूर्णाङ्क flags)
{
#सचे defined __NR_pidfd_send_signal
  पटिनिवत्त (पूर्णाङ्क) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#अञ्ञथा
  (शून्य) pidfd;
  (शून्य) sig;
  (शून्य) info;
  (शून्य) flags;
  errno = ENOSYS;
  पटिनिवत्त -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
स्थिर uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  सचे (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  सचे (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  सचे (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  सचे (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  सचे (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  सचे (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  सचे (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  सचे (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  पटिनिवत्त attr;
}

स्थिर पूर्णाङ्क
do_mount_setattr (bool recursive, ध्रुव वर्ण *target, पूर्णाङ्क targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  संरचना mount_attr_s attr = {
    0,
  };
  पूर्णाङ्क ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  सचे (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      सचे (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      अञ्ञथा सचे (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      अञ्ञथा
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      सचे (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  अञ्ञथा सचे (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "mount_setattr `/%s`", target);

  पटिनिवत्त 0;
}

पूर्णाङ्क
get_bind_mount (पूर्णाङ्क dirfd, ध्रुव वर्ण *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क open_वृक्षः_fd = -1;
  संरचना mount_attr_s attr = {
    0,
  };
  पूर्णाङ्क recursive_flag = (recursive ? AT_RECURSIVE : 0);
  पूर्णाङ्क ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  सचे (UNLIKELY (src == रिक्त))
    पटिनिवत्त crun_make_error (err, EINVAL, "mount source is not specified");

  सचे (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  सचे (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_वृक्षः_fd = syscall_open_वृक्षः (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  सचे (UNLIKELY (open_वृक्षः_fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_वृक्षः_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "mount_setattr `%s`", src);

  पटिनिवत्त get_and_reset (&open_वृक्षः_fd);
}

पूर्णाङ्क
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, वर्ण *option, वर्ण **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free वर्ण *mappings = रिक्त;
  ध्रुव वर्ण *it;
  पूर्णाङ्क ret;

  mappings = xmalloc (allocated);

  अत्थाय (it = option; *it;)
    {
      bool relative = false;
      long मानम्[3];
      size_t i;

      सचे (*it == '\0')
        भङ्ग;

      सचे (*it == '#')
        it++;

      सचे (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      अत्थाय (i = 0; i < 3; i++)
        {
          वर्ण *endptr = रिक्त;

          सचे (i > 0 && *it == '-')
            it++;

          सचे (*it == '\0')
            पटिनिवत्त crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          मानम्[i] = strtol (it, &endptr, 10);
          सचे (errno || endptr == it)
            पटिनिवत्त crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      सचे (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          सचे (def == रिक्त
              || def->linux == रिक्त
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            पटिनिवत्त crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          अत्थाय (i = 0; i < mappings_len; i++)
            सचे (मानम्[1] >= (int64_t) mappings[i]->container_id
                && मानम्[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              भङ्ग;

          सचे (i == mappings_len)
            पटिनिवत्त crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          मानम्[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      सचे (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", मानम्[0], मानम्[1], मानम्[2]);
      सचे (UNLIKELY (ret >= (पूर्णाङ्क) (allocated - written)))
        पटिनिवत्त crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = रिक्त;

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
format_mount_mappings (वर्ण **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  ध्रुव size_t max_len_mapping = 64;
  cleanup_free वर्ण *ret = रिक्त;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  अत्थाय (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      सचे (UNLIKELY (len >= max_len_mapping))
        पटिनिवत्त crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = रिक्त;
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
format_mount_mapping (वर्ण **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t size, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .size = size,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    रिक्त,
  };

  पटिनिवत्त format_mount_mappings (ret, mappings, 1, written, err);
}

स्थिर bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  सचे (def->linux == रिक्त)
    पटिनिवत्त mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  सचे (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    पटिनिवत्त false;

  सचे (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    पटिनिवत्त false;

  अत्थाय (s = 0; s < mnt->uid_mappings_len; s++)
    {
      सचे (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        पटिनिवत्त false;
      सचे (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        पटिनिवत्त false;
      सचे (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        पटिनिवत्त false;
    }

  अत्थाय (s = 0; s < mnt->gid_mappings_len; s++)
    {
      सचे (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        पटिनिवत्त false;
      सचे (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        पटिनिवत्त false;
      सचे (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        पटिनिवत्त false;
    }

  पटिनिवत्त true;
}

स्थिर pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        ध्रुव वर्ण *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != रिक्त;

  सचे (! need_new_userns)
    पटिनिवत्त 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, रिक्त);
  सचे (UNLIKELY (pid < 0))
    पटिनिवत्त crun_make_error (err, errno, "clone");

  सचे (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      याव (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  सचे (mnt->uid_mappings_len)
    {
      cleanup_free वर्ण *uid_map = रिक्त;
      cleanup_free वर्ण *gid_map = रिक्त;
      size_t written = 0;
      पूर्णाङ्क ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      cleanup_close पूर्णाङ्क fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      सचे (UNLIKELY (fd < 0))
        पटिनिवत्त fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      सचे (UNLIKELY (fd < 0))
        पटिनिवत्त fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  अञ्ञथा
    {
      cleanup_free वर्ण *dup_options = रिक्त;
      वर्ण *option, *saveptr = रिक्त;

      सचे (! options)
        पटिनिवत्त crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      अत्थाय (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (रिक्त, ";", &saveptr))
        {
          cleanup_free वर्ण *mappings = रिक्त;
          cleanup_close पूर्णाङ्क map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          पूर्णाङ्क ret;

          सचे (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              सचे (UNLIKELY (map_fd < 0))
                पटिनिवत्त map_fd;
            }
          अञ्ञथा सचे (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              सचे (UNLIKELY (map_fd < 0))
                पटिनिवत्त map_fd;
            }
          अञ्ञथा
            पटिनिवत्त crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_create_keyring (libcrun_container_t *container, ध्रुव वर्ण *name, ध्रुव वर्ण *label, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क labelfd = -1;
  bool label_set = false;
  पूर्णाङ्क ret;

  सचे (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      सचे (UNLIKELY (labelfd < 0))
        {
          सचे (crun_error_get_errno (err) != ENOENT)
            पटिनिवत्त labelfd;

          crun_error_release (err);
        }

      सचे (labelfd >= 0)
        {
          ret = लिख (labelfd, label, strlen (label));
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  सचे (UNLIKELY (ret < 0))
    {
      सचे (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          गच्छ out;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      गच्छ out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  सचे (label_set && लिख (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  पटिनिवत्त ret;
}

स्थिर शून्य
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  सचे (def->process && def->process->user)
    {
      सचे (def->process->user->uid)
        *uid = def->process->user->uid;
      सचे (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

स्थिर unsigned long
get_mount_flags (ध्रुव वर्ण *name, पूर्णाङ्क current_flags, पूर्णाङ्क *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  ध्रुव संरचना propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  सचे (found)
    *found = prop ? 1 : 0;

  सचे (prop == रिक्त)
    पटिनिवत्त 0;

  सचे (prop->extra_flags & OPTION_RECURSIVE)
    {
      सचे (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      सचे (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  सचे (extra_flags)
    *extra_flags |= prop->extra_flags;

  सचे (prop->clear)
    पटिनिवत्त current_flags & ~prop->flags;

  पटिनिवत्त current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
स्थिर bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  अत्थाय (i = 0; i < mount->options_len; i++)
    {
      ध्रुव संरचना propagation_flags_s *prop;

      सचे (mount->options[i] == रिक्त)
        अनुवर्त;

      prop = libcrun_str2mount_flags (mount->options[i]);
      सचे (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        पटिनिवत्त true;
    }

  पटिनिवत्त false;
}

स्थिर unsigned long
get_mount_flags_or_option (ध्रुव वर्ण *name, पूर्णाङ्क current_flags, unsigned long *extra_flags, वर्ण **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  पूर्णाङ्क found;
  __attribute__ ((unused)) cleanup_free वर्ण *prev = रिक्त;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  सचे (name == रिक्त)
    पटिनिवत्त 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  सचे (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      ध्रुव वर्ण *data_flag = रिक्त;
      सचे ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      अञ्ञथा सचे ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      सचे (data_flag)
        {
          prev = *option;
          सचे (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          अञ्ञथा
            *option = xstrdup (data_flag);
        }
      पटिनिवत्त flags;
    }

  prev = *option;
  सचे (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  अञ्ञथा
    *option = xstrdup (name);

  पटिनिवत्त 0;
}

पूर्णाङ्क
pivot_मूलम् (ध्रुव वर्ण *new_मूलम्, ध्रुव वर्ण *put_old)
{
  पटिनिवत्त syscall (__NR_pivot_root, new_मूलम्, put_old);
}

स्थिर शून्य
free_remount (संरचना remount_s *r)
{
  सचे (r == रिक्त)
    पटिनिवत्त;
  सचे (r->targetfd >= 0)
    close (r->targetfd);
  मुक्त (r->data);
  मुक्त (r->target);
  मुक्त (r);
}

स्थिर संरचना remount_s *
make_remount (पूर्णाङ्क targetfd, ध्रुव वर्ण *target, unsigned long flags, ध्रुव वर्ण *data, संरचना remount_s *next)
{
  संरचना remount_s *ret = xmalloc (परिमाण (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : रिक्त;
  ret->next = next;
  ret->targetfd = targetfd;
  पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क
do_remount (पूर्णाङ्क targetfd, ध्रुव वर्ण *target, unsigned long flags, ध्रुव वर्ण *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  पूर्णाङ्क ret;
  ध्रुव वर्ण *real_target = target;

  सचे (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      सचे (LIKELY (ret == 0))
        पटिनिवत्त 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  सचे (flags & (MS_REMOUNT | MS_RDONLY))
    data = रिक्त;

  ret = mount (रिक्त, real_target, रिक्त, flags, data);
  सचे (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      संरचना statfs sfs;

      ret = statfs (real_target, &sfs);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      सचे (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      सचे ((flags | remount_flags) != flags)
        {
          ret = mount (रिक्त, real_target, रिक्त, flags | remount_flags, data);
          सचे (LIKELY (ret == 0))
            पटिनिवत्त 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          सचे (current_flags & MS_RDONLY)
            ret = mount (रिक्त, real_target, रिक्त, flags | remount_flags | MS_RDONLY, data);
        }
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "remount `%s`", target);
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret = 0;
  संरचना remount_s *r = get_private_data (container)->remounts;
  याव (r)
    {
      संरचना remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      सचे (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          सचे (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              अनुवर्त;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  याव (r)
    {
      संरचना remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = रिक्त;
  पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क
open_mount_target (libcrun_container_t *container, ध्रुव वर्ण *target_rel, libcrun_error_t *err)
{
  ध्रुव वर्ण *rootfs = get_private_data (container)->rootfs;
  पूर्णाङ्क rootfsfd = get_private_data (container)->rootfsfd;

  सचे (rootfsfd < 0)
    पटिनिवत्त crun_make_error (err, 0, "invalid rootfs state");

  पटिनिवत्त safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
स्थिर पूर्णाङ्क
fsopen_mount (ध्रुव वर्ण *type, ध्रुव वर्ण *source_name, ध्रुव वर्ण *labeltype, ध्रुव वर्ण *label, ध्रुव वर्ण *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close पूर्णाङ्क fsfd = -1;
  पूर्णाङ्क ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  सचे (UNLIKELY (fsfd < 0))
    पटिनिवत्त fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (शून्य) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  सचे (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (data && data[0])
    {
      cleanup_free वर्ण *data_copy = xstrdup (data);
      वर्ण *saveptr = रिक्त;
      वर्ण *token;

      अत्थाय (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (रिक्त, ",", &saveptr))
        {
          वर्ण *eq = strchr (token, '=');
          सचे (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          अञ्ञथा
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, रिक्त, 0);

          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, रिक्त, रिक्त, 0);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#अञ्ञथा
  (शून्य) type;
  (शून्य) source_name;
  (शून्य) data;
  errno = ENOSYS;
  पटिनिवत्त -1;
#endif
}

स्थिर पूर्णाङ्क
get_procfd (संरचना private_data_s *data, libcrun_error_t *err)
{
  पूर्णाङ्क fd;

  सचे (data->procfd >= 0)
    पटिनिवत्त data->procfd;

  fd = fsopen_mount ("proc", रिक्त, रिक्त, रिक्त, रिक्त);
  सचे (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  पटिनिवत्त fd;
}

स्थिर पूर्णाङ्क
get_old_मूलम्_fd (संरचना private_data_s *data)
{
  सचे (data->old_मूलम्_fd >= 0)
    पटिनिवत्त data->old_मूलम्_fd;

  data->old_मूलम्_fd = open ("/", O_PATH | O_CLOEXEC);
  पटिनिवत्त data->old_मूलम्_fd;
}

स्थिर पूर्णाङ्क
fs_move_mount_to (पूर्णाङ्क fd, पूर्णाङ्क dirfd, ध्रुव वर्ण *name)
{
#ifdef HAVE_NEW_MOUNT_API
  सचे (name)
    पटिनिवत्त syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  पटिनिवत्त syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#अञ्ञथा
  (शून्य) fd;
  (शून्य) dirfd;
  (शून्य) name;
  (शून्य) syscall_move_mount;
  errno = ENOSYS;
  पटिनिवत्त -1;
#endif
}

गणना
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

स्थिर पूर्णाङ्क do_mount (libcrun_container_t *container, ध्रुव वर्ण *source, पूर्णाङ्क targetfd,
                     ध्रुव वर्ण *target, ध्रुव वर्ण *fstype, unsigned long mountflags,
                     ध्रुव शून्य *data, पूर्णाङ्क extra_flags, libcrun_error_t *err);

स्थिर bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, ध्रुव वर्ण *option)
{
  size_t i;
  अत्थाय (i = 0; i < mnt->options_len; i++)
    सचे (mnt->options[i] != रिक्त && strcmp (mnt->options[i], option) == 0)
      पटिनिवत्त true;
  पटिनिवत्त false;
}

स्थिर runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, ध्रुव वर्ण *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  अत्थाय (i = 0; i < def->mounts_len; i++)
    {
      सचे (strcmp (def->mounts[i]->destination, destination) == 0)
        पटिनिवत्त def->mounts[i];
    }
  पटिनिवत्त रिक्त;
}

स्थिर शून्य
warn_tmpfs_fallback_once (संरचना private_data_s *private_data, ध्रुव वर्ण *reason)
{
  सचे (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
स्थिर पूर्णाङ्क
get_shared_empty_dir_cached (libcrun_container_t *container, वर्ण **proc_fd_path, libcrun_error_t *err)
{
  संरचना private_data_s *private_data = get_private_data (container);
  cleanup_close पूर्णाङ्क fd = -1;
  cleanup_free वर्ण *empty_dir_path = रिक्त;
  proc_fd_path_t fd_path;
  पूर्णाङ्क ret;

  /* Fast path: return cached proc fd path if already set up */
  सचे (private_data->maskdir_proc_path != रिक्त)
    {
      सचे (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      पटिनिवत्त 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_मूलम्, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  सचे (fd < 0)
    पटिनिवत्त fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  सचे (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  पटिनिवत्त 0;
}

स्थिर bool
is_dev_null (ध्रुव संरचना stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  पटिनिवत्त S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
स्थिर पूर्णाङ्क
ensure_dev_null (पूर्णाङ्क fd, libcrun_error_t *err)
{
  संरचना stat st;

  सचे (UNLIKELY (fstat (fd, &st) < 0))
    पटिनिवत्त crun_make_error (err, errno, "stat `/dev/null`");

  सचे (UNLIKELY (! is_dev_null (&st)))
    पटिनिवत्त crun_make_error (err, 0, "`/dev/null` is not the null device");

  पटिनिवत्त 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
स्थिर bool
is_empty_ro_tmpfs (पूर्णाङ्क fd)
{
  संरचना statfs sfs;

  सचे (fstatfs (fd, &sfs) < 0)
    पटिनिवत्त false;

  पटिनिवत्त sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

स्थिर पूर्णाङ्क
mount_masked_dir (libcrun_container_t *container, पूर्णाङ्क pathfd, ध्रुव वर्ण *rel_path, libcrun_error_t *err)
{
  संरचना private_data_s *private_data = get_private_data (container);
  cleanup_close पूर्णाङ्क newfd = -1;
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क ret;

  सचे (private_data->maskdir_fd < 0)
    {
      सचे (private_data->maskdir_bind_failed)
        गच्छ fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, रिक्त, &tmp_err);
      सचे (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->सन्देशः);
          crun_error_release (&tmp_err);
          गच्छ fallback_to_tmpfs;
        }
    }

  सचे (private_data->maskdir_fd >= 0)
    {
      पूर्णाङ्क rootfsfd = private_data->rootfsfd;
      संरचना stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, रिक्त);
      सचे (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          सचे (UNLIKELY (newfd < 0))
            पटिनिवत्त newfd;

          ret = fstat (newfd, &after);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          सचे (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            पटिनिवत्त crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          पटिनिवत्त 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_path);

      TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
      private_data->maskdir_fd = -1;
      private_data->maskdir_move_failed = true;
    }

  private_data->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_data, tmp_err ? tmp_err->सन्देशः : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  सचे (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      सचे (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      अञ्ञथा
        {
          सचे (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
do_masked_or_readonly_path (libcrun_container_t *container, ध्रुव वर्ण *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  ध्रुव वर्ण *rootfs = get_private_data (container)->rootfs;
  cleanup_close पूर्णाङ्क pathfd = -1;
  संरचना statfs sfs;
  पूर्णाङ्क ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  सचे (UNLIKELY (rel_path == रिक्त))
    पटिनिवत्त crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  सचे (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  सचे (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      सचे (errno != ENOENT && errno != EACCES)
        पटिनिवत्त pathfd;

      सचे (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      पटिनिवत्त 0;
    }

  सचे (readonly)
    {
      पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close पूर्णाङ्क mountfd = -1;

      सचे (UNLIKELY (procfd < 0))
        पटिनिवत्त procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      सचे (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          सचे (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, रिक्त);
          अञ्ञथा
            ret = -1;

          सचे (ret < 0)
            crun_error_release (err);
        }

      सचे (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          सचे (keep_flags)
            {
              cleanup_close पूर्णाङ्क sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              सचे (UNLIKELY (sfd < 0))
                पटिनिवत्त crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          सचे (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त ret;

              ret = fs_move_mount_to (mountfd, pathfd, रिक्त);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          अञ्ञथा
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, रिक्त, mount_flags, रिक्त,
                              LABEL_NONE, err);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त ret;
            }
        }
    }
  अञ्ञथा
    {
      ret = get_file_type_fd (pathfd, &mode);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      सचे ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      अञ्ञथा
        {
          cleanup_close पूर्णाङ्क nullfd = -1;
          cleanup_close पूर्णाङ्क mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          सचे (UNLIKELY (nullfd < 0))
            पटिनिवत्त crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          सचे (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, रिक्त);

          सचे (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, रिक्त, MS_BIND | MS_RDONLY, रिक्त,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  ध्रुव वर्ण *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  सचे (context_type)
    {
      सचे (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          पटिनिवत्त रिक्त;
        }
      पटिनिवत्त context_type;
    }

  पटिनिवत्त "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
स्थिर पूर्णाङ्क
diagnose_mount_failure (पूर्णाङ्क ret, libcrun_error_t *err, libcrun_container_t *container,
                        ध्रुव वर्ण *fstype)
{
  सचे (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      सचे (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          पटिनिवत्त ret;
        }
    }
  पटिनिवत्त ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
स्थिर पूर्णाङ्क
sysfs_userns_fallback (libcrun_container_t *container, ध्रुव वर्ण *target,
                       ध्रुव वर्ण *real_target, पूर्णाङ्क targetfd,
                       unsigned long mountflags, ध्रुव वर्ण *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close पूर्णाङ्क mountfd = -1;
  cleanup_close पूर्णाङ्क sysfd = -1;
  पूर्णाङ्क sys_old_मूलम्_fd = -1;
  पूर्णाङ्क ret;

  सचे (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    पटिनिवत्त 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (ret == 0)
    पटिनिवत्त 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  सचे (sys_old_मूलम्_fd >= 0)
    mountfd = get_bind_mount (sys_old_मूलम्_fd, "sys", true, true, false, MS_PRIVATE, err);
  सचे (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  सचे (cgroup_mount == रिक्त)
    {
      सचे (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, रिक्त);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      अञ्ञथा
        {
          crun_error_release (err);
          सचे (sys_old_मूलम्_fd >= 0)
            {
              cleanup_free वर्ण *sys_path = रिक्त;
              proc_fd_path_t sys_मूलम्_path;

              get_proc_self_fd_path (sys_मूलम्_path, sys_old_मूलम्_fd);
              xasprintf (&sys_path, "%s/sys", sys_मूलम्_path);
              ret = mount (sys_path, real_target, रिक्त, MS_BIND | MS_REC, रिक्त);
            }
          अञ्ञथा
            ret = mount ("/sys", real_target, रिक्त, MS_BIND | MS_REC, रिक्त);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      सचे (UNLIKELY (sysfd < 0))
        पटिनिवत्त sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        रिक्त, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      पटिनिवत्त do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  सचे (UNLIKELY (mountfd < 0))
    पटिनिवत्त mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  सचे (UNLIKELY (sysfd < 0))
    पटिनिवत्त sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    रिक्त, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
स्थिर शून्य
apply_selinux_xattr (libcrun_container_t *container, पूर्णाङ्क fd, पूर्णाङ्क extra_flags, ध्रुव वर्ण *label)
{
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क procfd;

  सचे ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    पटिनिवत्त;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  सचे (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close पूर्णाङ्क xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      सचे (xfd >= 0)
        (शून्य) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
स्थिर पूर्णाङ्क
do_mount_replace_rootfs (libcrun_container_t *container, पूर्णाङ्क *fd, libcrun_error_t *err)
{
  पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
  पूर्णाङ्क tmp;
  पूर्णाङ्क ret;

  सचे (UNLIKELY (procfd < 0))
    पटिनिवत्त procfd;

  {
    cleanup_close पूर्णाङ्क nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    सचे (UNLIKELY (nsfd < 0))
      पटिनिवत्त crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    सचे (UNLIKELY (ret < 0))
      पटिनिवत्त crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  सचे (UNLIKELY (*fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  सचे (UNLIKELY (tmp < 0))
    पटिनिवत्त crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  पटिनिवत्त 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
स्थिर पूर्णाङ्क
apply_propagation_flags (ध्रुव वर्ण *target, पूर्णाङ्क targetfd, ध्रुव वर्ण *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  पूर्णाङ्क ret;

  सचे (targetfd >= 0)
    {
      libcrun_error_t tmp_err = रिक्त;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      सचे (LIKELY (ret == 0))
        propagation_done = true;
      अञ्ञथा
        crun_error_release (&tmp_err);
    }

  सचे (! propagation_done)
    {
      ret = mount (रिक्त, real_target, रिक्त, mountflags & ALL_PROPAGATIONS, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  पटिनिवत्त 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
स्थिर पूर्णाङ्क
schedule_or_do_remount (libcrun_container_t *container, पूर्णाङ्क *fd, पूर्णाङ्क targetfd,
                        ध्रुव वर्ण *target, ध्रुव वर्ण *real_target,
                        bool single_instance, unsigned long mountflags,
                        ध्रुव शून्य *data, पूर्णाङ्क extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  संरचना remount_s *r;

  सचे ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    पटिनिवत्त do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  सचे (*fd < 0)
    {
      *fd = dup (targetfd);
      सचे (UNLIKELY (*fd < 0))
        पटिनिवत्त crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
do_mount (libcrun_container_t *container, ध्रुव वर्ण *source, पूर्णाङ्क targetfd,
          ध्रुव वर्ण *target, ध्रुव वर्ण *fstype, unsigned long mountflags, ध्रुव शून्य *data,
          पूर्णाङ्क extra_flags, libcrun_error_t *err)
{
  cleanup_free वर्ण *data_with_label = रिक्त;
  ध्रुव वर्ण *data_without_label = data;
  proc_fd_path_t target_buffer;
  ध्रुव वर्ण *context_type = रिक्त;
  cleanup_close पूर्णाङ्क ms_move_fd = -1;
  ध्रुव वर्ण *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close पूर्णाङ्क fd = -1;
  ध्रुव वर्ण *label = रिक्त;
  पूर्णाङ्क ret = 0;

  सचे (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  अञ्ञथा
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  सचे (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  सचे ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      सचे (UNLIKELY (context_type == रिक्त))
        पटिनिवत्त -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      सचे (ret < 0)
        पटिनिवत्त ret;
      data = data_with_label;
    }

  सचे (mountflags & MS_MOVE)
    {
      सचे ((mountflags & MS_BIND) || fstype)
        पटिनिवत्त crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, रिक्त, MS_MOVE, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      सचे (UNLIKELY (ms_move_fd < 0))
        पटिनिवत्त ms_move_fd;
      targetfd = ms_move_fd;
    }

  सचे ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      सचे (targetfd >= 0)
        {
          सचे (mountflags & MS_BIND)
            {
              cleanup_close पूर्णाङ्क bindfd = -1;
              libcrun_error_t tmp_err = रिक्त;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              सचे (bindfd < 0)
                {
                  पूर्णाङ्क old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));

                  सचे (old_मूलम्_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_मूलम्_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              सचे (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, रिक्त);
              सचे (ret < 0)
                crun_error_release (&tmp_err);
            }
          अञ्ञथा
            {
              cleanup_close पूर्णाङ्क newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              सचे (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, रिक्त);
            }
        }

      सचे (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          सचे (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              पूर्णाङ्क old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));

              सचे (old_मूलम्_fd >= 0)
                {
                  cleanup_free वर्ण *proc_source_buf = रिक्त;
                  proc_fd_path_t old_मूलम्_path;

                  get_proc_self_fd_path (old_मूलम्_path, old_मूलम्_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_मूलम्_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      सचे (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          सचे (ret == 0)
            पटिनिवत्त 0;
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          पटिनिवत्त diagnose_mount_failure (ret, err, container, fstype);
        }

      सचे (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          सचे (UNLIKELY (fd < 0))
            पटिनिवत्त fd;

          /* We are replacing the rootfs, reopen it.  */
          सचे (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त ret;
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

  सचे (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  सचे (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  सचे (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  पटिनिवत्त ret;
}

स्थिर शून्य
try_umount (पूर्णाङ्क targetfd, ध्रुव वर्ण *target)
{
  proc_fd_path_t target_buffer;
  ध्रुव वर्ण *real_target = target;

  सचे (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

स्थिर bool
container_has_cgroupns (libcrun_container_t *container)
{
  पटिनिवत्त get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

स्थिर पूर्णाङ्क
do_mount_cgroup_v2 (libcrun_container_t *container, पूर्णाङ्क targetfd, ध्रुव वर्ण *target,
                    unsigned long mountflags, ध्रुव वर्ण *data, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  पूर्णाङ्क cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  सचे (UNLIKELY (cgroup_mode < 0))
    पटिनिवत्त cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  सचे (UNLIKELY (ret < 0))
    {
      पूर्णाङ्क saved_errno = crun_error_get_errno (err);
      सचे (saved_errno == EPERM || saved_errno == EBUSY)
        {
          ध्रुव वर्ण *unified_cgroup_path;
          ध्रुव वर्ण *src_cgroup;

          crun_error_release (err);

          सचे (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              सचे (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  सचे (LIKELY (ret == 0))
                    पटिनिवत्त ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_मूलम्;
          पटिनिवत्त do_mount (container, src_cgroup, targetfd, target, रिक्त,
                           MS_BIND | mountflags, रिक्त, LABEL_NONE, err);
        }

      पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, ध्रुव वर्ण *source, पूर्णाङ्क targetfd, ध्रुव वर्ण *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क fd = -1;
  ध्रुव वर्ण *subsystem = "systemd";
  cleanup_free वर्ण *subsystem_path = रिक्त;
  cleanup_close पूर्णाङ्क tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  सचे (UNLIKELY (tmpfsdirfd < 0))
    पटिनिवत्त tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "openat `%s`", subsystem_path);

  पटिनिवत्त do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

स्थिर पूर्णाङ्क
do_mount_cgroup_v1 (libcrun_container_t *container, ध्रुव वर्ण *source, पूर्णाङ्क targetfd, ध्रुव वर्ण *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *content = रिक्त;
  वर्ण *from;
  cleanup_close पूर्णाङ्क tmpfsdirfd = -1;
  वर्ण *saveptr = रिक्त;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  सचे (UNLIKELY (tmpfsdirfd < 0))
    पटिनिवत्त tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
    सचे (UNLIKELY (procfd < 0))
      पटिनिवत्त procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, रिक्त, err);
  }
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (UNLIKELY (content == रिक्त || content[0] == '\0'))
    पटिनिवत्त crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  अत्थाय (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (रिक्त, "\n", &saveptr))
    {
      cleanup_free वर्ण *source_path = रिक्त;
      cleanup_free वर्ण *source_subsystem = रिक्त;
      cleanup_free वर्ण *subsystem_path = रिक्त;
      वर्ण *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close पूर्णाङ्क subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      सचे (subsystem[0] == '\0')
        अनुवर्त;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      सचे (it)
        subsystem = it + 5;

      सचे (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      सचे (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_मूलम्, subsystem, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      /* if there is already a mount specified, do not add a default one.  */
      सचे (find_mount_for (container, source_subsystem) != रिक्त)
        अनुवर्त;

      ret = append_paths (&source_path, err, source_subsystem, subpath, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      सचे (UNLIKELY (subsystemfd < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `%s`", subsystem_path);

      सचे (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          सचे (UNLIKELY (ret < 0))
            {
              सचे (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  अनुवर्त;
                }
              पटिनिवत्त ret;
            }
        }
      अञ्ञथा
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, रिक्त, MS_BIND | mountflags, रिक्त,
                          LABEL_NONE, err);
          सचे (UNLIKELY (ret < 0))
            {
              सचे (crun_error_get_errno (err) != ENOENT)
                पटिनिवत्त ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, रिक्त, MS_BIND | mountflags,
                              रिक्त, LABEL_NONE, err);
              सचे (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  सचे (crun_error_get_errno (err) != ENOENT)
                    पटिनिवत्त ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
do_mount_cgroup (libcrun_container_t *container, ध्रुव वर्ण *source, पूर्णाङ्क targetfd, ध्रुव वर्ण *target,
                 unsigned long mountflags, ध्रुव वर्ण *data, libcrun_error_t *err)
{
  पूर्णाङ्क cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  सचे (UNLIKELY (cgroup_mode < 0))
    पटिनिवत्त cgroup_mode;

  विकल्प (cgroup_mode)
    {
    पक्ष CGROUP_MODE_UNIFIED:
      पटिनिवत्त do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    पक्ष CGROUP_MODE_LEGACY:
    पक्ष CGROUP_MODE_HYBRID:
      पटिनिवत्त do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  पटिनिवत्त crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

संरचना device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
स्थिर शून्य
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  संरचना private_data_s *private_data = get_private_data (container);
  size_t i;

  सचे (private_data->needed_devs_fds)
    {
      संरचना libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      अत्थाय (i = 0; needed_devs[i].path; i++)
        {
          सचे (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            अनुवर्त;

          पूर्णाङ्क fd = syscall_open_वृक्षः (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          सचे (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          अञ्ञथा सचे (fd >= 0)
            close (fd);
        }
    }

  सचे (private_data->notify_socket_वृक्षः_fd < 0
      && private_data->host_notify_socket_path)
    {
      पूर्णाङ्क fd = syscall_open_वृक्षः (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      सचे (fd >= 0)
        private_data->notify_socket_वृक्षः_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
स्थिर ध्रुव वर्ण *
relative_path_under_dev (ध्रुव वर्ण *path)
{
  सचे (path[0] != '/')
    पटिनिवत्त path;
  सचे (strncmp (path, "/dev/", 5) == 0)
    {
      सचे (strchr (path + 5, '/'))
        पटिनिवत्त रिक्त;
      पटिनिवत्त path + 5;
    }
  पटिनिवत्त रिक्त;
}

स्थिर पूर्णाङ्क
mknod_and_set_attrs (libcrun_container_t *container, पूर्णाङ्क dirfd, ध्रुव वर्ण *name, संरचना device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  proc_fd_path_t fd_path;
  ध्रुव वर्ण *what = "new";
  bool exists = false;
  संरचना stat st;
  पूर्णाङ्क procfd;
  पूर्णाङ्क ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  सचे (UNLIKELY (ret < 0))
    {
      सचे (errno != EEXIST)
        पटिनिवत्त crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  सचे (UNLIKELY ((st.st_mode & S_IFMT) != type))
    पटिनिवत्त crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  सचे (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    पटिनिवत्त crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  सचे (exists)
    पटिनिवत्त 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  सचे (UNLIKELY (procfd < 0))
    पटिनिवत्त procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fchown `%s`", device->path);

  पटिनिवत्त 1;
}

पूर्णाङ्क
libcrun_create_dev (libcrun_container_t *container, पूर्णाङ्क devfd, पूर्णाङ्क srcfd,
                    संरचना device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  ध्रुव वर्ण *fullname = device->path;
  ध्रुव वर्ण *rootfs = get_private_data (container)->rootfs;
  सचे (is_empty_string (fullname))
    पटिनिवत्त crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free वर्ण *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  सचे (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  ध्रुव वर्ण *rel_dev = relative_path_under_dev (normalized_path);

  सचे (binds)
    {
      cleanup_close पूर्णाङ्क fd = -1;

      सचे (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          सचे (UNLIKELY (fd < 0))
            {
              सचे (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              सचे (UNLIKELY (fd < 0))
                पटिनिवत्त crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      अञ्ञथा
        {
          ध्रुव वर्ण *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          सचे (UNLIKELY (fd < 0))
            पटिनिवत्त fd;
        }

      सचे (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          सचे (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              सचे (LIKELY (ret >= 0))
                पटिनिवत्त 0;
            }
          अञ्ञथा
            crun_error_release (err);
        }

      {
        cleanup_close पूर्णाङ्क mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        सचे (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            सचे (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, रिक्त);
                सचे (LIKELY (ret == 0))
                  पटिनिवत्त 0;
              }
            अञ्ञथा
              crun_error_release (err);
          }
        अञ्ञथा
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, रिक्त, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, रिक्त, LABEL_MOUNT, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  अञ्ञथा
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      सचे (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          सचे (ret <= 0)
            पटिनिवत्त ret;
        }
      अञ्ञथा
        {
          cleanup_close पूर्णाङ्क dirfd = -1;
          cleanup_free वर्ण *dirname = रिक्त;
          वर्ण *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          सचे (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          सचे (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              सचे (UNLIKELY (dirfd < 0))
                पटिनिवत्त crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          अञ्ञथा
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              सचे (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              सचे (UNLIKELY (dirfd < 0))
                पटिनिवत्त dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          सचे (ret <= 0)
            पटिनिवत्त ret;
        }
    }
  पटिनिवत्त 0;
}

संरचना symlink_s
{
  ध्रुव वर्ण *path;
  ध्रुव वर्ण *target;
  bool force;
};

स्थिर संरचना symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { रिक्त, रिक्त, false } };

स्थिर पूर्णाङ्क
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  size_t i;
  संरचना device_s *it;
  cleanup_close पूर्णाङ्क devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  ध्रुव वर्ण *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map संरचना libcrun_fd_map *dev_fds = रिक्त;
  cleanup_close_map संरचना libcrun_fd_map *needed_fds = रिक्त;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = रिक्त;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = रिक्त;

  सचे (! def || ! def->linux)
    पटिनिवत्त 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  सचे (UNLIKELY (devfd < 0))
    पटिनिवत्त devfd;

  अत्थाय (i = 0; i < def->linux->devices_len; i++)
    {
      संरचना device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      सचे (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  अत्थाय (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close पूर्णाङ्क srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
      सचे (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  अत्थाय (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      सचे (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क saved_errno = errno;

          सचे (errno == EEXIST && ! symlinks[i].force)
            अनुवर्त;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          सचे (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              सचे (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              सचे (ret < 0 && errno == EBUSY)
                {
                  cleanup_close पूर्णाङ्क tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  सचे (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      सचे (umount2 (procpath, MNT_DETACH) == 0)
                        गच्छ retry_unlink;
                    }
                }
              सचे (ret == 0)
                गच्छ retry_symlink;
            }
          पटिनिवत्त crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  सचे (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
स्थिर ध्रुव वर्ण *
next_path_component (ध्रुव वर्ण *p, size_t *len)
{
  अत्थाय (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      सचे (*len != 1 || p[0] != '.')
        पटिनिवत्त p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
स्थिर bool
same_path (ध्रुव वर्ण *a, ध्रुव वर्ण *b)
{
  size_t la, lb;

  अत्थाय (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      सचे (la != lb || memcmp (a, b, la) != 0)
        पटिनिवत्त false;
      सचे (la == 0)
        पटिनिवत्त true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
स्थिर bool
is_duplicate_path (वर्ण **paths, size_t i)
{
  size_t j;

  सचे (paths[i] == रिक्त)
    पटिनिवत्त false;

  अत्थाय (j = 0; j < i; j++)
    सचे (paths[j] && same_path (paths[j], paths[i]))
      पटिनिवत्त true;

  पटिनिवत्त false;
}

स्थिर पूर्णाङ्क
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  पूर्णाङ्क ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  सचे (! def || ! def->linux)
    पटिनिवत्त 0;

  अत्थाय (i = 0; i < def->linux->masked_paths_len; i++)
    {
      सचे (is_duplicate_path (def->linux->masked_paths, i))
        अनुवर्त;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  अत्थाय (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
do_pivot (libcrun_container_t *container, ध्रुव वर्ण *rootfs, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क oldrootfd = -1;
  cleanup_close पूर्णाङ्क newrootfd = -1;
  पूर्णाङ्क ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  सचे (UNLIKELY (oldrootfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  सचे (UNLIKELY (newrootfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_मूलम् (".", ".");
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, रिक्त, -1, ".", रिक्त, MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = umount2 (".", MNT_DETACH);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "umount2 oldroot");

  करोहि
    {
      ret = umount2 (".", MNT_DETACH);
      सचे (ret < 0 && errno == EINVAL)
        भङ्ग;
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "umount2 oldroot");
  } याव (ret == 0);

  ret = chdir ("/");
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "chdir to newroot");

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, वर्ण **data, libcrun_error_t *err)
{
  ध्रुव वर्ण *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close पूर्णाङ्क fd = -1;
  संरचना stat st;
  पूर्णाङ्क ret;

  सचे (*data != रिक्त && strstr (*data, "mode="))
    पटिनिवत्त 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  सचे (fd < 0)
    {
      सचे (crun_error_get_errno (err) != ENOENT)
        पटिनिवत्त fd;

      crun_error_release (err);
      पटिनिवत्त 0;
    }
  ret = fstat (fd, &st);
  सचे (ret < 0)
    पटिनिवत्त crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
get_default_flags (libcrun_container_t *container, ध्रुव वर्ण *destination, वर्ण **data)
{
  सचे (strcmp (destination, "/proc") == 0)
    पटिनिवत्त 0;
  सचे (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      पटिनिवत्त MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  सचे (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      पटिनिवत्त MS_NOEXEC | MS_STRICTATIME;
    }
  सचे (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      पटिनिवत्त MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  सचे (strcmp (destination, "/dev/mqueue") == 0)
    पटिनिवत्त MS_NOEXEC | MS_NOSUID | MS_NODEV;
  सचे (strcmp (destination, "/dev/pts") == 0)
    {
      सचे (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      अञ्ञथा
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      पटिनिवत्त MS_NOEXEC | MS_NOSUID;
    }
  सचे (strcmp (destination, "/sys") == 0)
    पटिनिवत्त MS_NOEXEC | MS_NOSUID | MS_NODEV;

  पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  पटिनिवत्त find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

स्थिर पूर्णाङ्क
check_valid_no_follow_file_system (पूर्णाङ्क fd, ध्रुव वर्ण *destination, libcrun_error_t *err)
{
  संरचना statfs sfs;

  पूर्णाङ्क ret = fstatfs (fd, &sfs);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "statfs `%s`", destination);

  विकल्प (sfs.f_type)
    {
    पक्ष PROC_SUPER_MAGIC:
    पक्ष SYSFS_MAGIC:
    पक्ष DEVPTS_SUPER_MAGIC:
      पटिनिवत्त crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
safe_create_symlink (पूर्णाङ्क rootfsfd, ध्रुव वर्ण *rootfs, ध्रुव वर्ण *target, ध्रुव वर्ण *destination, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क parent_dir_fd = -1;
  cleanup_free वर्ण *buffer = रिक्त;
  वर्ण *part;
  पूर्णाङ्क ret;

  सचे (is_empty_string (destination))
    पटिनिवत्त crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  सचे (UNLIKELY (parent_dir_fd < 0))
    पटिनिवत्त parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  सचे (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      सचे (errno == EEXIST)
        {
          cleanup_free वर्ण *link = रिक्त;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          सचे (UNLIKELY (len < 0))
            पटिनिवत्त len;

          सचे ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            पटिनिवत्त 0;

          पटिनिवत्त crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      पटिनिवत्त crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
handle_copy_symlink (libcrun_container_t *container, ध्रुव वर्ण *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     ध्रुव वर्ण *link_target, libcrun_error_t *err)
{
  cleanup_free वर्ण *target = रिक्त;

  सचे (link_target == रिक्त)
    {
      पूर्णाङ्क old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));
      ध्रुव वर्ण *source = mount->source;
      ssize_t len;

      सचे (old_मूलम्_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_मूलम्_fd, source + 1, &target, 0, err);
      अञ्ञथा
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      सचे (UNLIKELY (len < 0))
        पटिनिवत्त len;
      link_target = target;
    }

  पटिनिवत्त safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

स्थिर पूर्णाङ्क
prepare_sysfs_or_proc_mount (libcrun_container_t *container, ध्रुव वर्ण *target,
                             पूर्णाङ्क *targetfd, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  सचे (UNLIKELY (ret < 0))
    {
      सचे (errno == ENOENT)
        {
          सचे (strchr (target, '/'))
            पटिनिवत्त crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      अञ्ञथा सचे (errno == ENOTDIR)
        पटिनिवत्त crun_make_error (err, errno, "the target `/%s` is invalid", target);

      सचे (ret < 0)
        पटिनिवत्त crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
handle_tmpcopyup (libcrun_container_t *container, ध्रुव वर्ण *rootfs, ध्रुव वर्ण *target,
                  पूर्णाङ्क copy_from_fd, libcrun_error_t *err)
{
  पूर्णाङ्क destfd, ret;
  cleanup_close पूर्णाङ्क tmpfd = copy_from_fd;

  target = consume_slashes (target);

  सचे (is_empty_string (target))
    पटिनिवत्त crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  सचे (UNLIKELY (destfd < 0))
    पटिनिवत्त crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क
get_mount_label_how (ध्रुव वर्ण *type, bool is_sysfs_or_proc)
{
  सचे (is_sysfs_or_proc)
    पटिनिवत्त LABEL_NONE;

  सचे (strcmp (type, "mqueue") == 0)
    पटिनिवत्त LABEL_XATTR;

  पटिनिवत्त LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
स्थिर unsigned long
get_bind_mount_locked_flags (पूर्णाङ्क source_mountfd, ध्रुव वर्ण *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  संरचना statfs sfs;
  size_t i;
  पूर्णाङ्क ret;

  सचे (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  अञ्ञथा
    ret = statfs (source, &sfs);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त 0;

  अत्थाय (i = 0; i < mount->options_len; i++)
    {
      ध्रुव संरचना propagation_flags_s *prop;

      सचे (mount->options[i] == रिक्त)
        अनुवर्त;

      prop = libcrun_str2mount_flags (mount->options[i]);
      सचे (prop == रिक्त || (prop->extra_flags & OPTION_RECURSIVE))
        अनुवर्त;

      सचे (prop->clear)
        cleared |= prop->flags;
      अञ्ञथा
        cleared &= ~prop->flags;
    }

  पटिनिवत्त sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
स्थिर ध्रुव वर्ण *
get_path_in_rootfs (ध्रुव वर्ण *rootfs, ध्रुव वर्ण *path)
{
  size_t len;

  सचे (rootfs == रिक्त || path == रिक्त)
    पटिनिवत्त रिक्त;

  len = strlen (rootfs);
  याव (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  सचे (len == 0)
    पटिनिवत्त रिक्त;

  सचे (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    पटिनिवत्त रिक्त;

  पटिनिवत्त consume_slashes (path + len);
}

स्थिर पूर्णाङ्क
process_single_mount (libcrun_container_t *container, ध्रुव वर्ण *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      संरचना libcrun_fd_map *mount_fds, size_t mount_index,
                      ध्रुव वर्ण *systemd_cgroup_v1, libcrun_error_t *err)
{
  ध्रुव वर्ण *target = consume_slashes (mount->destination);
  cleanup_close पूर्णाङ्क source_mountfd = -1;
  cleanup_free वर्ण *data = रिक्त;
  वर्ण *type;
  वर्ण *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close पूर्णाङ्क copy_from_fd = -1;
  cleanup_close पूर्णाङ्क targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  पूर्णाङ्क no_remount = 0;
  पूर्णाङ्क ret;

  सचे (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  सचे (mount->options == रिक्त)
    flags = get_default_flags (container, mount->destination, &data);
  अञ्ञथा
    {
      size_t j;

      अत्थाय (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  सचे (type == रिक्त && (flags & MS_BIND) == 0)
    पटिनिवत्त crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  सचे ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      ध्रुव वर्ण *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      सचे (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          सचे (UNLIKELY (source_mountfd < 0))
            पटिनिवत्त crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  सचे ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  सचे (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      सचे (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      सचे (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  सचे (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      ध्रुव वर्ण *path = mount->source;

      सचे ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        पटिनिवत्त crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      सचे (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, रिक्त);
        }
      अञ्ञथा सचे (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      अञ्ञथा सचे (path[0] == '/')
        {
          पूर्णाङ्क old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          सचे (old_मूलम्_fd >= 0)
            ret = get_file_type_at (old_मूलम्_fd, &src_mode, nofollow, (path[1] == '\0') ? रिक्त : path + 1);
          अञ्ञथा
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      अञ्ञथा
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "cannot stat `%s`", path);

      सचे (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          पूर्णाङ्क bind_dirfd = AT_FDCWD;
          ध्रुव वर्ण *bind_source = mount->source;
          पूर्णाङ्क old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));

          सचे (old_मूलम्_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_मूलम्_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          source_mountfd = ret;
        }
    }

  सचे (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      ध्रुव वर्ण *cached_target = रिक्त;

      सचे (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      mounted = true;
    }
  अञ्ञथा सचे (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  अञ्ञथा
    {
      bool is_dir = S_ISDIR (src_mode);

      सचे (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
      अञ्ञथा
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          targetfd = ret;
        }
    }

  सचे (extra_flags & OPTION_TMPCOPYUP)
    {
      सचे (strcmp (type, "tmpfs") != 0)
        पटिनिवत्त crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      सचे (UNLIKELY (copy_from_fd < 0))
        {
          सचे (errno != ENOTDIR)
            पटिनिवत्त crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  सचे (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, रिक्त);
      सचे (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          सचे (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, रिक्त, source_mountfd, target, रिक्त, remaining_flags, data, LABEL_NONE | no_remount, err);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त ret;
            }

          mounted = true;

          सचे (is_empty_string (target))
            {
              पूर्णाङ्क new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              सचे (UNLIKELY (new_rootfsfd < 0))
                पटिनिवत्त crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      अञ्ञथा सचे (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          पटिनिवत्त crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      अञ्ञथा सचे (get_private_data (container)->host_मूलम्_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          पटिनिवत्त crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      अञ्ञथा सचे (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  सचे (! mounted)
    {
      सचे (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
      अञ्ञथा सचे (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
      अञ्ञथा
        {
          पूर्णाङ्क extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
    }

  सचे (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (rec_clear || rec_set)
    {
      ध्रुव bool is_dir = S_ISDIR (src_mode);
      cleanup_close पूर्णाङ्क dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      सचे (UNLIKELY (dfd < 0))
        पटिनिवत्त crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
do_mounts (libcrun_container_t *container, ध्रुव वर्ण *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ध्रुव वर्ण *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map संरचना libcrun_fd_map *mount_fds = रिक्त;
  size_t i;
  पूर्णाङ्क ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = रिक्त;

  अत्थाय (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  पटिनिवत्त 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
पूर्णाङ्क
libcrun_container_do_bind_mount (libcrun_container_t *container, वर्ण *mount_source, वर्ण *mount_destination, वर्ण **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  ध्रुव वर्ण *target = consume_slashes (mount_destination);
  cleanup_free वर्ण *data = रिक्त;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close पूर्णाङ्क targetfd = -1;
  पूर्णाङ्क is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  ध्रुव वर्ण *rootfs = get_private_data (container)->rootfs;
  पूर्णाङ्क ret;

  सचे ((get_private_data (container)->rootfsfd < 0) || (rootfs == रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  सचे (mount_options == रिक्त)
    flags = get_default_flags (container, mount_destination, &data);
  अञ्ञथा
    {
      size_t j;

      अत्थाय (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  सचे (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  सचे (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      सचे (UNLIKELY (is_dir < 0))
        पटिनिवत्त is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
पूर्णाङ्क
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, पूर्णाङ्क *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close पूर्णाङ्क notify_fd = -1;
  cleanup_free वर्ण *host_notify_socket_path = रिक्त;
  cleanup_free वर्ण *state_dir = रिक्त;
  वर्ण *host_path = रिक्त;
  पूर्णाङ्क ret;

  सचे (container && get_private_data (container)->host_notify_socket_path)
    {
      ध्रुव वर्ण *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  सचे (host_path == रिक्त)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_मूलम्, context->id, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  सचे (UNLIKELY (notify_fd < 0))
    पटिनिवत्त notify_fd;

  सचे (UNLIKELY (chmod (host_path, 0777) < 0))
    पटिनिवत्त crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  सचे (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (शून्य) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  पटिनिवत्त 1;
#अञ्ञथा
  (शून्य) context;
  (शून्य) container;
  (शून्य) err;
  *notify_socket_out = -1;
  पटिनिवत्त 0;
#endif
}

#ifdef HAVE_SYSTEMD
स्थिर पूर्णाङ्क
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  ध्रुव वर्ण *notify_socket = container->context->notify_socket;
  cleanup_free वर्ण *host_notify_socket_path = रिक्त;
  cleanup_free वर्ण *container_notify_socket_path = रिक्त;
  cleanup_free वर्ण *state_dir = रिक्त;
  uid_t container_मूलम्_uid = -1;
  gid_t container_मूलम्_gid = -1;
  पूर्णाङ्क notify_socket_वृक्षः_fd;

  सचे (notify_socket == रिक्त)
    पटिनिवत्त 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_मूलम् : रिक्त),
                                     (container->context ? container->context->id : रिक्त), err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = mkdir (host_notify_socket_path, 0700);
  सचे (ret < 0)
    पटिनिवत्त crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  सचे (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_मूलम्_in_the_userns (container->container_def, 0, 0, &container_मूलम्_uid, &container_मूलम्_gid);
      सचे (container_मूलम्_uid != ((uid_t) -1) && container_मूलम्_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_मूलम्_uid, container_मूलम्_gid);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_मूलम्_uid, container_मूलम्_gid);
        }
    }

  notify_socket_वृक्षः_fd = syscall_open_वृक्षः (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  सचे (notify_socket_वृक्षः_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_वृक्षः_fd = notify_socket_वृक्षः_fd;
  अञ्ञथा सचे (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  अञ्ञथा सचे (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  अञ्ञथा
    /* some other error */
    पटिनिवत्त crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = रिक्त;
  पटिनिवत्त 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
स्थिर पूर्णाङ्क
do_finalize_notify_socket (libcrun_container_t *container, ध्रुव वर्ण *rootfs, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *host_notify_socket_path = रिक्त;
  cleanup_free वर्ण *container_notify_socket_path = रिक्त;
  cleanup_free वर्ण *container_notify_socket_path_dir_alloc = रिक्त;
  ध्रुव वर्ण *container_notify_socket_path_dir = रिक्त;
  cleanup_close पूर्णाङ्क notify_socket_वृक्षः_fd = -1;
  cleanup_close पूर्णाङ्क targetfd = -1;
  पूर्णाङ्क did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = रिक्त;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = रिक्त;

  सचे (host_notify_socket_path == रिक्त || container_notify_socket_path == रिक्त)
    पटिनिवत्त 0;

  सचे (rootfs == रिक्त)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  सचे (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  targetfd = ret;

  notify_socket_वृक्षः_fd = get_private_data (container)->notify_socket_वृक्षः_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_वृक्षः_fd = -1;

  सचे (notify_socket_वृक्षः_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_वृक्षः_fd, targetfd, रिक्त);
      सचे (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          सचे (UNLIKELY (targetfd < 0))
            पटिनिवत्त targetfd;

          ret = do_mount (container, रिक्त, targetfd, container_notify_socket_path_dir, रिक्त,
                          MS_REC | MS_PRIVATE, रिक्त, LABEL_NONE, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
      अञ्ञथा सचे (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      अञ्ञथा
        पटिनिवत्त crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_वृक्षः_fd,
                                container_notify_socket_path_dir);
    }

  सचे (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, रिक्त,
                      MS_BIND | MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
make_parent_mount_private (ध्रुव वर्ण *rootfs, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क rootfsfd = -1;
  संरचना stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  सचे (UNLIKELY (rootfsfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", rootfs);

  सचे (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    पटिनिवत्त crun_make_error (err, errno, "fstat `%s`", rootfs);

  अत्थाय (;;)
    {
      संरचना stat cur_st;
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क ret;
      cleanup_close पूर्णाङ्क parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      सचे (ret == 0)
        पटिनिवत्त 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (रिक्त, proc_path, रिक्त, MS_PRIVATE, रिक्त);
      सचे (ret == 0)
        पटिनिवत्त 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      सचे (parentfd < 0)
        {
          पूर्णाङ्क saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          सचे (ret != 0)
            पटिनिवत्त crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          पटिनिवत्त crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      सचे (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        पटिनिवत्त crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      सचे (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        भङ्ग;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  पटिनिवत्त crun_make_error (err, 0, "make `%s` private", rootfs);
}

पूर्णाङ्क
libcrun_set_mounts (संरचना container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ध्रुव वर्ण *rootfs, set_mounts_cb_t cb, शून्य *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क is_user_ns = 0;
  पूर्णाङ्क ret = 0;

  सचे (rootfs == रिक्त || def->mounts == रिक्त)
    पटिनिवत्त 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  सचे (def->मूलम्->readonly)
    {
      संरचना remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      पूर्णाङ्क fd;

      fd = dup (get_private_data (container)->rootfsfd);
      सचे (UNLIKELY (fd < 0))
        पटिनिवत्त crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, रिक्त, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  सचे (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      सचे (UNLIKELY (is_user_ns < 0))
        पटिनिवत्त is_user_ns;
    }

  सचे (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  सचे (cb)
    {
      ret = cb (cb_data, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = रिक्त;
      ध्रुव वर्ण *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (शून्य) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
umount_or_hide (ध्रुव वर्ण *target, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = umount2 (target, MNT_DETACH);
  सचे (UNLIKELY (ret < 0))
    {
      पूर्णाङ्क saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      सचे (errno == EINVAL)
        {
          ret = mount (रिक्त, target, "tmpfs", 0, "size=0k");
          सचे (LIKELY (ret == 0))
            पटिनिवत्त 0;
        }

      पटिनिवत्त crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क
move_मूलम् (ध्रुव वर्ण *rootfs, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = chdir (rootfs);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = umount_or_hide ("/proc", err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "chdir to `%s`", rootfs);

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_finalize_mounts (संरचना container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ध्रुव वर्ण *rootfs,
                         set_mounts_cb_t after_pivot_cb, शून्य *cb_data, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_मूलम्_fd));

  सचे (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      सचे (get_private_data (container)->no_pivot)
        {
          ret = move_मूलम् (rootfs, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
      अञ्ञथा
        {
          ret = do_pivot (container, rootfs, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      ret = do_mount (container, रिक्त, -1, "/", रिक्त,
                      get_private_data (container)->rootfs_propagation,
                      रिक्त, LABEL_MOUNT, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = chdir ("/");
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "chdir to `/`");
    }

  सचे (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = finalize_mounts (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
maybe_open_वृक्षः_namespace (ध्रुव वर्ण *rootfs, पूर्णाङ्क *out_fd, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क rootfs_fd = -1;
  पूर्णाङ्क वृक्षः_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  सचे (UNLIKELY (rootfs_fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", rootfs);

  वृक्षः_fd = syscall_open_वृक्षः (rootfs_fd, "",
                               OPEN_वृक्षः_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  सचे (वृक्षः_fd < 0)
    {
      सचे (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        पटिनिवत्त 0;
      पटिनिवत्त crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = वृक्षः_fd;
  पटिनिवत्त 0;
}

स्थिर संरचना libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  संरचना libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  सचे (mount_fds == रिक्त)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  पटिनिवत्त mount_fds;
}

स्थिर पूर्णाङ्क
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free वर्ण *mnt_data = रिक्त;
  unsigned long mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  ध्रुव वर्ण *label_type = रिक्त;
  ध्रुव वर्ण *label_val = रिक्त;
  ध्रुव वर्ण *fstype;
  पूर्णाङ्क mnt_fd;
  size_t j;

  सचे (mount->type == रिक्त)
    पटिनिवत्त -1;

  fstype = mount->type;

  अत्थाय (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  सचे (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      पूर्णाङ्क extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      सचे (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          सचे (label_type == रिक्त)
            crun_error_release (err);
          अञ्ञथा
            label_val = def->linux->mount_label;
        }
    }

  सचे (strcmp (fstype, "cgroup") == 0)
    {
      पूर्णाङ्क cgroup_mode = libcrun_get_cgroup_mode (err);
      सचे (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  सचे (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, रिक्त, रिक्त, mnt_data);

  पटिनिवत्त mnt_fd;
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
संरचना mnt_id_req_s
{
  uint32_t size;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

संरचना statmount_s
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
  वर्ण str[];
};

स्थिर पूर्णाङ्क
syscall_listmount (संरचना mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned पूर्णाङ्क flags)
{
#ifdef __NR_listmount
  पटिनिवत्त (पूर्णाङ्क) syscall (__NR_listmount, req, ids, nr, flags);
#अञ्ञथा
  (शून्य) req;
  (शून्य) ids;
  (शून्य) nr;
  (शून्य) flags;
  errno = ENOSYS;
  पटिनिवत्त -1;
#endif
}

स्थिर पूर्णाङ्क
syscall_statmount (संरचना mnt_id_req_s *req, संरचना statmount_s *buf, size_t size, unsigned पूर्णाङ्क flags)
{
#ifdef __NR_statmount
  पटिनिवत्त (पूर्णाङ्क) syscall (__NR_statmount, req, buf, size, flags);
#अञ्ञथा
  (शून्य) req;
  (शून्य) buf;
  (शून्य) size;
  (शून्य) flags;
  errno = ENOSYS;
  पटिनिवत्त -1;
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
स्थिर bool
mounts_hit_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = रिक्त;
  size_t i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  अत्थाय (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      सचे (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != रिक्त && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          भङ्ग;
        }
    }
  सचे (! has_absolute_bind)
    पटिनिवत्त false;

  ids = xmalloc (capacity * परिमाण (uint64_t));
  अत्थाय (;;)
    {
      संरचना mnt_id_req_s req = {
        .size = परिमाण (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      पूर्णाङ्क ret;

      सचे (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * परिमाण (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      सचे (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        पटिनिवत्त true;
      सचे (ret == 0)
        भङ्ग;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  अत्थाय (i = 0; i < n_ids; i++)
    {
      वर्ण buffer[4096] __attribute__ ((aligned (8)));
      संरचना statmount_s *sm = (संरचना statmount_s *) buffer;
      संरचना mnt_id_req_s req = {
        .size = परिमाण (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      ध्रुव वर्ण *मूलम्, *point;
      size_t j;

      सचे (syscall_statmount (&req, sm, परिमाण (buffer), 0) < 0)
        अनुवर्त;
      सचे ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        अनुवर्त;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      मूलम् = sm->str + sm->mnt_root;
      सचे (! has_prefix (मूलम्, "mnt:["))
        अनुवर्त;

      point = sm->str + sm->mnt_point;

      अत्थाय (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          ध्रुव वर्ण *source = def->mounts[j]->source;
          size_t source_len;

          सचे (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            अनुवर्त;
          सचे (source == रिक्त || source[0] != '/')
            अनुवर्त;

          source_len = strlen (source);
          याव (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          सचे (strncmp (point, source, source_len) != 0)
            अनुवर्त;

          /* The source itself is a pinned mount namespace.  */
          सचे (point[source_len] == '\0')
            पटिनिवत्त true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          सचे (recursive && (source_len == 1 || point[source_len] == '/'))
            पटिनिवत्त true;
        }
    }

  पटिनिवत्त false;
}

स्थिर bool
can_use_open_वृक्षः_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  संरचना libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  सचे (has_hooks || has_userns)
    पटिनिवत्त false;

  सचे (mounts_hit_pinned_mount_namespace (def))
    पटिनिवत्त false;

  सचे (container->context->no_open_वृक्षः_namespace)
    पटिनिवत्त false;

  {
    libcrun_error_t tmp_err = रिक्त;
    पूर्णाङ्क in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    सचे (in_userns != 0)
      पटिनिवत्त false;
  }

  mount_fds = get_fd_map (container);
  अत्थाय (i = 0; i < def->mounts_len; i++)
    {
      सचे (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          सचे (! has_cached_target)
            पटिनिवत्त false;
        }
    }

  पटिनिवत्त true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
स्थिर bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  सचे (mount->type == रिक्त || strcmp (mount->type, "tmpfs") != 0)
    पटिनिवत्त false;

  अत्थाय (i = 0; i < mount->options_len; i++)
    सचे (mount->options[i] != रिक्त && has_prefix (mount->options[i], "mode="))
      पटिनिवत्त false;

  पटिनिवत्त true;
}

स्थिर पूर्णाङ्क
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, वर्ण **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close पूर्णाङ्क rootfsfd = -1;
  cleanup_close पूर्णाङ्क वृक्षः_fd = -1;
  libcrun_error_t tmp_err = रिक्त;
  bool use_open_वृक्षः = false;
  size_t i;
  पूर्णाङ्क ret;

  सचे (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, रिक्त, रिक्त, रिक्त, रिक्त);

  सचे ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  मुक्त (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  अत्थाय (i = 0; i < def->mounts_len; i++)
    {
      संरचना libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close पूर्णाङ्क mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      सचे (mount_fds && mount_fds->fds[i] >= 0)
        अनुवर्त;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      सचे (def->mounts[i]->source != रिक्त
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free वर्ण *mnt_data = रिक्त;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          अत्थाय (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          सचे (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free वर्ण *target = रिक्त;
              libcrun_error_t tmp_err = रिक्त;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              सचे (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              सचे (get_private_data (container)->copy_symlink_targets == रिक्त)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * परिमाण (वर्ण *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = रिक्त;
            }
        }

      सचे (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          सचे (mount_fds && def->mounts[i]->source != रिक्त && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == रिक्त)
            {
              libcrun_error_t tmp_err = रिक्त;
              unsigned long propagation = MS_PRIVATE;

              सचे (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              सचे (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          अनुवर्त;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      सचे (mnt_fd < 0)
        {
          crun_error_release (err);
          अनुवर्त;
        }

      सचे (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close पूर्णाङ्क destfd = -1;
          संरचना stat st;

          सचे (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              सचे (UNLIKELY (rootfsfd < 0))
                पटिनिवत्त crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          सचे (destfd < 0)
            crun_error_release (&tmp_err);
          अञ्ञथा सचे (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
              सचे (UNLIKELY (procfd < 0))
                पटिनिवत्त procfd;

              get_self_fd_path (fd_path, mnt_fd);
              ret = fchmodat (procfd, fd_path, st.st_mode & 07777, 0);
              (शून्य) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_empty_dir_cached (container, रिक्त, &tmp_err);
  सचे (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  सचे (! get_private_data (container)->joined_mount_ns
      && can_use_open_वृक्षः_namespace (container))
    use_open_वृक्षः = true;

  सचे (use_open_वृक्षः)
    {
      ret = maybe_open_वृक्षः_namespace (*rootfs, &वृक्षः_fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  सचे (वृक्षः_fd >= 0)
    preopen_needed_devs_and_notify (container);

  सचे (वृक्षः_fd >= 0)
    {
      ret = setns (वृक्षः_fd, CLONE_NEWNS);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (रिक्त, "/", रिक्त, MS_REMOUNT | MS_BIND, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, रिक्त, -1, "/", रिक्त, MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = do_mount (container, रिक्त, -1, "/", रिक्त, rootfs_propagation, रिक्त, LABEL_MOUNT, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_मूलम्_switched = true;
      मुक्त (*rootfs);
      *rootfs = xstrdup ("/");
    }
  अञ्ञथा
    {
      सचे (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, रिक्त, -1, "/", रिक्त, rootfs_propagation, रिक्त, LABEL_MOUNT, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = make_parent_mount_private (*rootfs, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, रिक्त, MS_BIND | MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  सचे (! container->context->force_no_cgroup)
    {
      पूर्णाङ्क cgroup_mode = libcrun_get_cgroup_mode (err);
      सचे (UNLIKELY (cgroup_mode < 0))
        पटिनिवत्त cgroup_mode;

      सचे (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          वर्ण *unified_cgroup_path = रिक्त;
          पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
          सचे (UNLIKELY (procfd < 0))
            पटिनिवत्त procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  सचे (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = रिक्त;
      पटिनिवत्त ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = रिक्त;

  सचे (वृक्षः_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  सचे (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_do_pivot_मूलम् (libcrun_container_t *container, bool no_pivot, वर्ण **rootfs, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  सचे (*rootfs == रिक्त)
    पटिनिवत्त 0;

  सचे (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      सचे (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "chdir to `/`");
        }
    }
  अञ्ञथा
    {
      ret = chroot (*rootfs);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "chdir to `/`");

      मुक्त (*rootfs);
      *rootfs = xstrdup ("/");
    }

  पटिनिवत्त 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
पूर्णाङ्क
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  संरचना stat statbuf;
  cleanup_close पूर्णाङ्क fd;
  पूर्णाङ्क ret;
  पूर्णाङ्क i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  सचे (UNLIKELY (fd == -1))
    पटिनिवत्त crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  अत्थाय (i = 0; i <= 2; i++)
    {
      सचे (UNLIKELY (fstat (i, &statbuf) == -1))
        पटिनिवत्त crun_make_error (err, errno, "stat fd `%d`", i);
      सचे (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          सचे (UNLIKELY (dup2 (fd, i) == -1))
            पटिनिवत्त crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
uidgidmap_helper (वर्ण *helper, pid_t pid, ध्रुव वर्ण *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  वर्ण pid_fmt[16];
  वर्ण *args[MAX_ARGS + 1];
  वर्ण *next;
  cleanup_free वर्ण *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  पूर्णाङ्क ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, परिमाण (pid_fmt), "%d", pid);
  सचे (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (pid_fmt)))
    पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  याव (nargs < MAX_ARGS)
    {
      वर्ण *p = strsep (&next, " \n");
      सचे (next == रिक्त)
        भङ्ग;
      args[nargs++] = p;
    }
  args[nargs++] = रिक्त;

  ret = run_process (args, err);
  सचे (ret < 0)
    पटिनिवत्त ret;
  सचे (ret > 0)
    पटिनिवत्त crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
newgidmap (pid_t pid, ध्रुव वर्ण *map_file, libcrun_error_t *err)
{
  पटिनिवत्त uidgidmap_helper ("newgidmap", pid, map_file, err);
}

स्थिर पूर्णाङ्क
newuidmap (pid_t pid, ध्रुव वर्ण *map_file, libcrun_error_t *err)
{
  पटिनिवत्त uidgidmap_helper ("newuidmap", pid, map_file, err);
}

स्थिर पूर्णाङ्क
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त fd;

  ret = TEMP_FAILURE_RETRY (लिख (fd, "deny", 4));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *content = रिक्त;

  सचे (get_private_data (container)->deny_setgroups)
    पटिनिवत्त 0;

  सचे (container->container_def->annotations)
    {
      ध्रुव वर्ण *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      सचे (annotation)
        पटिनिवत्त strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close पूर्णाङ्क fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    सचे (fd < 0)
      ret = fd;
    अञ्ञथा
      ret = read_all_fd (fd, "self/setgroups", &content, रिक्त, err);
  }
  सचे (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      सचे (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          पटिनिवत्त 1;
        }
      पटिनिवत्त ret;
    }

  पटिनिवत्त strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

पूर्णाङ्क
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = रिक्त;
  size_t additional_gids_len = 0;
  पूर्णाङ्क can_do_setgroups;
  पूर्णाङ्क ret;

  सचे (process != रिक्त && process->user != रिक्त)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  सचे (UNLIKELY (can_do_setgroups < 0))
    पटिनिवत्त can_do_setgroups;

  सचे (can_do_setgroups == 0)
    {
      ध्रुव वर्ण *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      सचे (annotation && strcmp (annotation, "0") != 0)
        पटिनिवत्त 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      अत्थाय (i = 0; i < additional_gids_len; i++)
        सचे (additional_gids[i] != process->user->gid)
          पटिनिवत्त crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (पूर्णाङ्क) additional_gids[i]);
      पटिनिवत्त 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "setgroups");

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  सचे (get_private_data (container)->unshare_cgroupns)
    {
      पूर्णाङ्क ret = unshare (CLONE_NEWCGROUP);
      सचे (UNLIKELY (ret < 0))
        {
          सचे (errno != EINVAL)
            पटिनिवत्त crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  पटिनिवत्त 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
पूर्णाङ्क
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  सचे (len != 1)
    पटिनिवत्त 0;

  सचे (mappings[0]->size != 1)
    पटिनिवत्त 0;

  सचे (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    पटिनिवत्त 0;

  पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क
write_id_map (libcrun_container_t *container, pid_t pid, ध्रुव वर्ण *map_name,
              पूर्णाङ्क (*idmap_helper) (pid_t, ध्रुव वर्ण *, libcrun_error_t *), ध्रुव वर्ण *helper_name,
              bool warn_on_failure, ध्रुव वर्ण *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  पूर्णाङ्क ret = 0;

  सचे (container->host_uid)
    ret = idmap_helper (pid, map, err);
  सचे (container->host_uid == 0 || ret < 0)
    {
      cleanup_close पूर्णाङ्क fd = -1;

      सचे (ret < 0)
        {
          सचे (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      सचे (UNLIKELY (fd < 0))
        पटिनिवत्त fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      सचे (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free वर्ण *single_mapping = रिक्त;
          crun_error_release (err);

          सचे (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          सचे (UNLIKELY (fd < 0))
            पटिनिवत्त fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  पटिनिवत्त ret;
}

पूर्णाङ्क
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free वर्ण *uid_map = रिक्त;
  cleanup_free वर्ण *gid_map = रिक्त;
  size_t uid_map_len = 0, gid_map_len = 0;
  पूर्णाङ्क ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  सचे ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    पटिनिवत्त 0;

  सचे (! def || ! def->linux)
    पटिनिवत्त 0;

  सचे (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  अञ्ञथा
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      uid_map_len = (size_t) ret;
      सचे (uid_map == रिक्त)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
    }

  सचे (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  अञ्ञथा
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      gid_map_len = (size_t) ret;
      सचे (gid_map == रिक्त)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

संरचना all_caps_s
{
  unsigned long effective[2];
  unsigned long permitted[2];
  unsigned long inheritable[2];
  unsigned long ambient[2];
  unsigned long bounding[2];
};

स्थिर पूर्णाङ्क
has_cap_on (पूर्णाङ्क cap, long unsigned *caps)
{
  सचे (cap < 32)
    पटिनिवत्त CAP_TO_MASK_0 (cap) & caps[0];
  पटिनिवत्त (CAP_TO_MASK_1 (cap) & caps[1]);
}

स्थिर unsigned long cap_last_cap;

पूर्णाङ्क
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;
  वर्ण buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  सचे (fd < 0)
    पटिनिवत्त fd;
  ret = TEMP_FAILURE_RETRY (पठ (fd, buffer, परिमाण (buffer) - 1));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, रिक्त, 10);
  सचे (errno != 0)
    पटिनिवत्त crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
set_required_caps (संरचना all_caps_s *caps, uid_t uid, gid_t gid, पूर्णाङ्क no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  पूर्णाङ्क ret;
  संरचना __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  संरचना __user_cap_data_struct data[2] = { { 0 } };

  सचे (cap_last_cap == 0)
    पटिनिवत्त crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  अत्थाय (cap = 0; cap <= cap_last_cap; cap++)
    सचे (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        सचे (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          पटिनिवत्त crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  सचे (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    पटिनिवत्त crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  सचे (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    पटिनिवत्त crun_make_error (err, errno, "prctl reset ambient");

  अत्थाय (cap = 0; cap <= cap_last_cap; cap++)
    सचे (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        सचे (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          पटिनिवत्त crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  सचे (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "prctl reset KEEPCAPS");

      सचे (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          अत्थाय (cap = 0; cap <= cap_last_cap; cap++)
            सचे (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "capset");
        }
    }
#endif

  सचे (no_new_privs)
    सचे (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      पटिनिवत्त crun_make_error (err, errno, "prctl set no new privs");

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
read_caps (unsigned long caps[2], वर्ण **मानानि, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  अत्थाय (i = 0; i < len; i++)
    {
      cap_मानम्_t cap;
      सचे (cap_from_name (मानानि[i], &cap) < 0)
        पटिनिवत्त crun_make_error (err, 0, "unknown capability `%s`", मानानि[i]);

      सचे (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      अञ्ञथा
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#अञ्ञथा
  caps[0] = 0;
  caps[1] = 0;
#endif
  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  सचे (proc->selinux_label)
    पटिनिवत्त set_selinux_label (container, proc->selinux_label, now, err);

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  सचे (proc->apparmor_profile)
    पटिनिवत्त set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  पूर्णाङ्क no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  संरचना all_caps_s caps = {};
  पूर्णाङ्क ret;

  सचे (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  पटिनिवत्त set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

संरचना rlimit_s
{
  ध्रुव वर्ण *name;
  पूर्णाङ्क मानम्;
};

संरचना rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
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
                              { "RLIMIT_STACK", RLIMIT_राशिः },
                              { रिक्त, 0 } };

स्थिर पूर्णाङ्क
get_rlimit_resource (ध्रुव वर्ण *name)
{
  संरचना rlimit_s *it;
  अत्थाय (it = rlimits; it->name; it++)
    सचे (strcmp (it->name, name) == 0)
      पटिनिवत्त it->मानम्;
  पटिनिवत्त -1;
}

पूर्णाङ्क
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  अत्थाय (i = 0; i < len; i++)
    {
      अत्थाय (j = i + 1; j < len; j++)
        {
          सचे (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            पटिनिवत्त crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  अत्थाय (i = 0; i < len; i++)
    {
      संरचना rlimit limit;
      वर्ण *type = new_rlimits[i]->type;
      पूर्णाङ्क resource = get_rlimit_resource (type);
      सचे (UNLIKELY (resource < 0))
        पटिनिवत्त crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      सचे (UNLIKELY (setrlimit (resource, &limit) < 0))
        पटिनिवत्त crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  पूर्णाङ्क ret;
  सचे (def->hostname == रिक्त || def->hostname[0] == '\0')
    पटिनिवत्त 0;
  सचे (! has_uts)
    पटिनिवत्त crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "sethostname");
  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  पूर्णाङ्क ret;
  सचे (is_empty_string (def->domainname))
    पटिनिवत्त 0;
  सचे (! has_uts)
    पटिनिवत्त crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "setdomainname");
  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;
  वर्ण oom_buffer[16];
  सचे (def->process == रिक्त || ! def->process->oom_score_adj_present)
    पटिनिवत्त 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, परिमाण (oom_buffer), "%i", def->process->oom_score_adj);
  सचे (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (oom_buffer)))
    पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  सचे (fd < 0)
    पटिनिवत्त fd;
  ret = TEMP_FAILURE_RETRY (लिख (fd, oom_buffer, strlen (oom_buffer)));
  सचे (ret < 0)
    पटिनिवत्त crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  पटिनिवत्त 0;
}

ध्रुव वर्ण *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  रिक्त
};

स्थिर पूर्णाङ्क
validate_sysctl (ध्रुव वर्ण *original_कुञ्चिका, ध्रुव वर्ण *original_मानम्, ध्रुव वर्ण *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ध्रुव वर्ण *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  सचे (name[0] == '/' || path_has_dot_dot_component (name))
    पटिनिवत्त crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_कुञ्चिका);

  सचे (has_prefix (name, "fs/mqueue/"))
    {
      सचे (namespaces_created & CLONE_NEWIPC)
        पटिनिवत्त 0;

      namespace = "IPC";
      गच्छ fail;
    }

  सचे (has_prefix (name, "kernel/"))
    {
      size_t i;

      अत्थाय (i = 0; sysctlRequiringIPC[i]; i++)
        सचे (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            सचे (namespaces_created & CLONE_NEWIPC)
              पटिनिवत्त 0;

            namespace = "IPC";
            गच्छ fail;
          }

      सचे (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          सचे (! is_empty_string (def->domainname) && (strcmp (original_मानम्, def->domainname) != 0))
            पटिनिवत्त crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_कुञ्चिका);

          सचे (namespaces_created & CLONE_NEWUTS)
            पटिनिवत्त 0;

          namespace = "UTS";
          गच्छ fail;
        }

      सचे (strcmp (name, "kernel/hostname") == 0)
        पटिनिवत्त crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_कुञ्चिका);
    }
  सचे (has_prefix (name, "net/"))
    {
      सचे (namespaces_created & CLONE_NEWNET)
        पटिनिवत्त 0;

      namespace = "network";
      गच्छ fail;
    }

  पटिनिवत्त crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_कुञ्चिका);

fail:
  पटिनिवत्त crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_कुञ्चिका, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
स्थिर वर्ण *
sysctl_error_reason (ध्रुव वर्ण *name, पूर्णाङ्क namespaces_created, पूर्णाङ्क errno_)
{

  सचे (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    पटिनिवत्त xstrdup ("are all the IDs mapped in the user namespace?");

  पटिनिवत्त रिक्त;
}

पूर्णाङ्क
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close पूर्णाङ्क dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  सचे (def->linux == रिक्त || def->linux->sysctl == रिक्त || def->linux->sysctl->len == 0)
    पटिनिवत्त 0;

  अत्थाय (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णाङ्क मानम्;

      मानम् = libcrun_find_namespace (def->linux->namespaces[i]->type);
      सचे (UNLIKELY (मानम् < 0))
        पटिनिवत्त crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= मानम्;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  सचे (UNLIKELY (dirfd < 0))
    पटिनिवत्त dirfd;

  अत्थाय (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free वर्ण *name = रिक्त;
      cleanup_close पूर्णाङ्क fd = -1;
      पूर्णाङ्क ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      सचे (def->linux->sysctl->कुञ्चिकाः[i] == रिक्त || def->linux->sysctl->मानानि[i] == रिक्त)
        पटिनिवत्त crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_कुञ्चिका_to_proc_path (def->linux->sysctl->कुञ्चिकाः[i]);

      ret = validate_sysctl (def->linux->sysctl->कुञ्चिकाः[i], def->linux->sysctl->मानानि[i], name, namespaces_created, def, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      सचे (UNLIKELY (fd < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (लिख (fd, def->linux->sysctl->मानानि[i], strlen (def->linux->sysctl->मानानि[i])));
      सचे (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क saved_errno = errno;
          cleanup_free वर्ण *reason = रिक्त;

          reason = sysctl_error_reason (def->linux->sysctl->कुञ्चिकाः[i], namespaces_created, saved_errno);
          पटिनिवत्त crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
open_terminal (वर्ण **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  uid_t uid = 0;
  पूर्णाङ्क ret;

  सचे (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  पटिनिवत्त ret;
}

वर्ण *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  पटिनिवत्त get_private_data (container)->external_descriptors;
}

पूर्णाङ्क
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  ध्रुव वर्ण *buf = रिक्त;
  json_gen_ctx *gen = रिक्त;
  size_t buf_len;
  पूर्णाङ्क ret;
  पूर्णाङ्क i;

  सचे (! json_gen_init (&gen, रिक्त))
    पटिनिवत्त crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  सचे (UNLIKELY (ret != json_gen_status_ok))
    गच्छ json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  अत्थाय (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      वर्ण link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      सचे (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          सचे (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          अञ्ञथा
            {
              json_gen_free (gen);
              पटिनिवत्त crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      सचे (UNLIKELY (ret != json_gen_status_ok))
        गच्छ json_error;
    }

  ret = json_gen_array_close (gen);
  सचे (UNLIKELY (ret != json_gen_status_ok))
    गच्छ json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  सचे (UNLIKELY (ret != json_gen_status_ok))
    गच्छ json_error;

  सचे (buf)
    {
      वर्ण *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  पटिनिवत्त 0;

json_error:
  सचे (gen)
    json_gen_free (gen);
  पटिनिवत्त json_gen_error_to_crun_error (ret, err);
}

पूर्णाङ्क
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free वर्ण *pty = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;

  सचे (def->process == रिक्त || ! def->process->terminal)
    पटिनिवत्त 0;

  fd = open_terminal (&pty, def->process, err);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त fd;

  सचे (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", रिक्त, MS_BIND, रिक्त, LABEL_MOUNT, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  अञ्ञथा
    {
      ret = unlink ("/dev/console");
      सचे (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          सचे (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", रिक्त, MS_BIND, रिक्त, LABEL_MOUNT, err);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त ret;

              पटिनिवत्त get_and_reset (&fd);
            }
          पटिनिवत्त crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  पटिनिवत्त get_and_reset (&fd);
}

स्थिर bool
read_error_from_sync_socket (पूर्णाङ्क sync_socket_fd, पूर्णाङ्क *error, वर्ण **str)
{
  cleanup_free वर्ण *b = रिक्त;
  size_t size;
  पूर्णाङ्क code = 0;
  पूर्णाङ्क ret;

  सचे (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (पठ (sync_socket_fd, &code, परिमाण (code)));
      सचे (UNLIKELY (ret != (पूर्णाङ्क) परिमाण (code)))
        पटिनिवत्त false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (पठ (sync_socket_fd, &size, परिमाण (size)));
  सचे (UNLIKELY (ret != (पूर्णाङ्क) परिमाण (size)))
    पटिनिवत्त false;

  सचे (size == 0)
    पटिनिवत्त false;

  सचे (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (पठ (sync_socket_fd, b, size));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त false;

  b[ret] = '\0';

  *str = b;
  b = रिक्त;
  पटिनिवत्त true;
}

स्थिर bool
send_error_to_sync_socket (पूर्णाङ्क sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  पूर्णाङ्क ret;
  पूर्णाङ्क code;
  वर्ण *सन्देशः;

  सचे (err == रिक्त || *err == रिक्त)
    पटिनिवत्त false;

  code = crun_error_get_errno (err);

  सचे (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, "1", 1));
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त false;
    }

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, &code, परिमाण (code)));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त false;

  सन्देशः = (*err)->सन्देशः;
  size = strlen (सन्देशः) + 1;
  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, &size, परिमाण (size)));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त false;

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, सन्देशः, size));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त false;

  पटिनिवत्त true;
}

स्थिर अन्तःस्थ पूर्णाङ्क
send_success_to_sync_socket (पूर्णाङ्क sync_socket, libcrun_error_t *err)
{
  ध्रुव पूर्णाङ्क success = 0;
  पूर्णाङ्क ret;

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket, &success, परिमाण (success)));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "write to sync socket");

  पटिनिवत्त 0;
}

स्थिर __attribute__ ((noreturn)) शून्य
send_error_to_sync_socket_and_die (पूर्णाङ्क sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  वर्ण *सन्देशः;

  सचे (err == रिक्त || *err == रिक्त)
    _safe_exit (EXIT_FAILURE);

  सचे (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      सन्देशः = (*err)->सन्देशः;
      libcrun_fail_with_error (errno, "%s", सन्देशः);
    }
  _safe_exit (EXIT_FAILURE);
}

स्थिर पूर्णाङ्क
expect_success_from_sync_socket (पूर्णाङ्क sync_fd, libcrun_error_t *err)
{
  cleanup_free वर्ण *err_str = रिक्त;
  पूर्णाङ्क res = 1;
  पूर्णाङ्क ret;

  ret = TEMP_FAILURE_RETRY (पठ (sync_fd, &res, परिमाण (res)));
  सचे (UNLIKELY (ret != परिमाण (res)))
    पटिनिवत्त crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  सचे (res == 0)
    पटिनिवत्त 0;

  सचे (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      सचे (! is_empty_string (err_str))
        पटिनिवत्त crun_make_error (err, res, "%s", err_str);
    }

  पटिनिवत्त crun_make_error (err, res, "read from sync socket");
}

स्थिर पूर्णाङ्क
join_namespaces (runtime_spec_schema_config_schema *def, पूर्णाङ्क *namespaces_to_join, पूर्णाङ्क n_namespaces_to_join,
                 पूर्णाङ्क *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  पूर्णाङ्क i;

  अत्थाय (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free वर्ण *cwd = रिक्त;
      पूर्णाङ्क orig_index = namespaces_to_join_index[i];
      पूर्णाङ्क मानम्;

      सचे (namespaces_to_join[i] < 0)
        अनुवर्त;

      /* Skip the user namespace.  */
      मानम् = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      सचे (मानम् == CLONE_NEWUSER)
        अनुवर्त;

      सचे (मानम् == CLONE_NEWNS)
        {
          cwd = getcwd (रिक्त, 0);
          सचे (UNLIKELY (cwd == रिक्त))
            पटिनिवत्त crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], मानम्);
      सचे (UNLIKELY (ret < 0))
        {
          सचे (ignore_join_errors)
            अनुवर्त;
          पटिनिवत्त crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      सचे (मानम् == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  पटिनिवत्त 0;
}

#define MAX_NAMESPACES 10

संरचना init_status_s
{
  /* fd to the namespace to join.  */
  पूर्णाङ्क fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  पूर्णाङ्क index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  पूर्णाङ्क मानम्[MAX_NAMESPACES];
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
  पूर्णाङ्क userns_index;
  /* def->linux->namespaces userns.  */
  पूर्णाङ्क userns_index_origin;

  /* All namespaces created/joined by the container.  */
  पूर्णाङ्क all_namespaces;

  /* What namespaces are still missing to be created.  */
  पूर्णाङ्क namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  पूर्णाङ्क idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  पूर्णाङ्क idx_timens_to_join_immediately;
};

शून्य
cleanup_free_init_statusp (संरचना init_status_s *ns)
{
  size_t i;

  सचे (ns == रिक्त)
    पटिनिवत्त;

  अत्थाय (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

स्थिर पूर्णाङ्क
configure_init_status (संरचना init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  अत्थाय (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  अत्थाय (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णाङ्क मानम् = libcrun_find_namespace (def->linux->namespaces[i]->type);
      सचे (UNLIKELY (मानम् < 0))
        पटिनिवत्त crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      सचे (ns->all_namespaces & मानम्)
        पटिनिवत्त crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= मानम्;

      सचे (def->linux->namespaces[i]->path == रिक्त)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= मानम्;
        }
      अञ्ञथा
        {
          पूर्णाङ्क fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          सचे (ns->fd_len >= MAX_NAMESPACES)
            पटिनिवत्त crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          सचे (UNLIKELY (fd < 0))
            पटिनिवत्त crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          सचे (मानम् == CLONE_NEWUSER)
            {
              ns->userns_index = ns->fd_len;
              ns->userns_index_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->index[ns->fd_len] = i;
          ns->मानम्[ns->fd_len] = मानम्;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  सचे (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  पटिनिवत्त 0;
}

/* Detect if root is available in the container.  */
स्थिर bool
मूलम्_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  अत्थाय (i = 0; i < len; i++)
    सचे (mappings[i]->container_id == 0)
      पटिनिवत्त true;

  पटिनिवत्त false;
}

स्थिर संरचना libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  संरचना libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  सचे (dev_fds == रिक्त)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  पटिनिवत्त dev_fds;
}

#define अङ्कः_NEEDED_DEVS (परिमाण (needed_devs) / परिमाण (needed_devs[0]) - 1)

स्थिर संरचना libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  संरचना libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  सचे (fds == रिक्त)
    {
      fds = make_libcrun_fd_map (अङ्कः_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  पटिनिवत्त fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  सचे (recursive)
    *recursive = false;

  सचे (src_nofollow != रिक्त)
    *src_nofollow = false;

  अत्थाय (i = 0; i < mnt->options_len; i++)
    {
      सचे (mnt->options[i] == रिक्त)
        अनुवर्त;

      सचे (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          सचे (src_nofollow == रिक्त || *src_nofollow)
            भङ्ग;
        }
      सचे (strcmp (mnt->options[i], "rbind") == 0)
        {
          सचे (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          सचे (src_nofollow == रिक्त || *src_nofollow)
            भङ्ग;
        }
      सचे (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  पटिनिवत्त ret;
}

स्थिर वर्ण *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  अत्थाय (i = 0; i < mnt->options_len; i++)
    {
      सचे (mnt->options[i] == रिक्त)
        अनुवर्त;

      सचे (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          पटिनिवत्त mnt->options[i];
        }
      सचे (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          पटिनिवत्त mnt->options[i];
        }
    }
  पटिनिवत्त रिक्त;
}

स्थिर पूर्णाङ्क
open_mount_type (ध्रुव वर्ण *type, पूर्णाङ्क *out_fd, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fsopen_fd = -1;
  cleanup_close पूर्णाङ्क newfs_fd = -1;
  पूर्णाङ्क ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  सचे (UNLIKELY (fsopen_fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, रिक्त, रिक्त, 0);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  सचे (UNLIKELY (newfs_fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, पूर्णाङ्क *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  संरचना mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close पूर्णाङ्क fd = -1;
  ध्रुव वर्ण *idmap_option;
  bool recursive = false;
  ध्रुव वर्ण *options = रिक्त;
  bool has_mappings;
  पूर्णाङ्क ret;
  वर्ण *extra_सन्देशः = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != रिक्त);
  सचे (has_mappings_out)
    *has_mappings_out = has_mappings;
  सचे (! has_mappings)
    पटिनिवत्त 0;

  सचे ((mnt->uid_mappings == रिक्त) != (mnt->gid_mappings == रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  सचे (idmap_option)
    {
      options = strchr (idmap_option, '=');
      सचे (options)
        {
          /* Skip the '=' itself.  */
          options++;
          सचे (options[0] == '\0')
            options = रिक्त;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;
  सचे (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त fd;

  सचे (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_वृक्षः (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      सचे (UNLIKELY (newfs_fd < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  अञ्ञथा
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  सचे (UNLIKELY (ret < 0))
    {
      सचे (errno == EINVAL)
        {
          extra_सन्देशः = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      पटिनिवत्त crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_सन्देशः);
    }

  *out_fd = get_and_reset (&newfs_fd);
  पटिनिवत्त 0;
}

स्थिर uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  सचे (! def || ! def->linux)
    गच्छ exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  सचे (! mappings)
    गच्छ exit;

  अत्थाय (i = 0; i < len; i++)
    {
      सचे (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        पटिनिवत्त id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  पटिनिवत्त is_uid ? get_overflow_uid () : get_overflow_gid ();
}

स्थिर पूर्णाङ्क
precreate_device (libcrun_container_t *container, पूर्णाङ्क devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  वर्ण name[64];
  mode_t type;
  dev_t dev;
  पूर्णाङ्क ret;

  ret = snprintf (name, परिमाण (name), "%zu", i);
  सचे (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (name)))
    पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "chown `%s`", device->path);

  पटिनिवत्त get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

स्थिर पूर्णाङ्क
send_mounts (पूर्णाङ्क sync_socket_host, संरचना libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  पूर्णाङ्क ret;

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_host, &how_many, परिमाण (how_many)));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "write to sync socket");

  अत्थाय (i = 0; i < total; i++)
    {
      सचे (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (वर्ण *) &i, परिमाण (i), err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, पूर्णाङ्क sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map संरचना libcrun_fd_map *mount_fds = रिक्त;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free वर्ण *rootfs = रिक्त;
  size_t how_many = 0;
  size_t i;
  पूर्णाङ्क ret;

  सचे (def->mounts_len == 0)
    पटिनिवत्त 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  सचे (def->मूलम् && def->मूलम्->path)
    rootfs = realpath (def->मूलम्->path, रिक्त);

  सचे (! has_userns)
    {
      पूर्णाङ्क is_in_userns = check_running_in_user_namespace (err);
      सचे (UNLIKELY (is_in_userns < 0))
        पटिनिवत्त is_in_userns;

      has_userns = is_in_userns > 0;
    }

  अत्थाय (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      पूर्णाङ्क mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      सचे (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == रिक्त)
        {
          unsigned long propagation = 0;

          सचे (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, रिक्त, रिक्त, रिक्त, रिक्त) & ALL_PROPAGATIONS_NO_REC;
          सचे (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          सचे (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      सचे (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  पटिनिवत्त send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

स्थिर पूर्णाङ्क
prepare_and_send_dev_mounts (libcrun_container_t *container, पूर्णाङ्क sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map संरचना libcrun_fd_map *dev_fds = रिक्त;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close पूर्णाङ्क current_mountns = -1;
  cleanup_free वर्ण *state_dir = रिक्त;
  cleanup_free वर्ण *devs_path = रिक्त;
  cleanup_close पूर्णाङ्क devs_mountfd = -1;
  cleanup_close पूर्णाङ्क targetfd = -1;
  ध्रुव वर्ण *context_type = रिक्त;
  ध्रुव वर्ण *label = रिक्त;
  size_t how_many = 0;
  size_t i;
  पूर्णाङ्क ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  सचे (def->linux == रिक्त || def->linux->devices_len == 0)
    पटिनिवत्त 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  सचे (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    पटिनिवत्त send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_मूलम् : रिक्त),
                                     (container->context ? container->context->id : रिक्त), err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = mkdir (devs_path, 0700);
  सचे (UNLIKELY (ret < 0 && errno != EEXIST))
    पटिनिवत्त crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  सचे (UNLIKELY (current_mountns < 0))
    पटिनिवत्त current_mountns;

  ret = unshare (CLONE_NEWNS);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (रिक्त, "/", रिक्त, MS_REC | MS_PRIVATE, रिक्त);
  सचे (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      गच्छ restore_mountns;
    }

  सचे (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      सचे (UNLIKELY (context_type == रिक्त))
        {
          ret = -1;
          गच्छ restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", रिक्त, context_type, label, रिक्त);
  सचे (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      गच्छ restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  सचे (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      गच्छ restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, रिक्त);
  सचे (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      गच्छ restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  सचे (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      गच्छ restore_mountns;
    }

  अत्थाय (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      सचे (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          अनुवर्त;
        }

      dev_fds->fds[i] = ret;

      सचे (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  सचे (ns_changed && current_mountns >= 0)
    {
      पूर्णाङ्क setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      सचे (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          पटिनिवत्त crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, पूर्णाङ्क sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map संरचना libcrun_fd_map *fds = रिक्त;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (अङ्कः_NEEDED_DEVS);

  सचे (! has_userns || geteuid () > 0)
    पटिनिवत्त send_mounts (sync_socket_host, fds, 0, अङ्कः_NEEDED_DEVS, err);

  अत्थाय (i = 0; i < अङ्कः_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      पूर्णाङ्क fd = syscall_open_वृक्षः (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      सचे (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  पटिनिवत्त send_mounts (sync_socket_host, fds, how_many, अङ्कः_NEEDED_DEVS, err);
}

स्थिर पूर्णाङ्क
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, पूर्णाङ्क sync_socket_host, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
receive_mounts (संरचना libcrun_fd_map *fds, पूर्णाङ्क sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  पूर्णाङ्क ret;

  सचे (fds->nfds == 0)
    पटिनिवत्त 0;

  ret = TEMP_FAILURE_RETRY (पठ (sync_socket_container, &how_many, परिमाण (how_many)));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "read from sync socket");

  अत्थाय (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (वर्ण *) &index, परिमाण (index), err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
      सचे (index >= fds->nfds)
        पटिनिवत्त crun_make_error (err, 0, "invalid mount data received");

      सचे (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  पटिनिवत्त 0;
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
स्थिर पूर्णाङ्क
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  सचे (! container->host_has_cap_sys_ptrace)
    पटिनिवत्त 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  पूर्णाङ्क ret;

  सचे (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool मूलम्_mapped = false;

      सचे (def->linux->uid_mappings_len != 0)
        {
          मूलम्_mapped = मूलम्_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          सचे (! मूलम्_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      सचे (def->linux->gid_mappings_len != 0)
        {
          मूलम्_mapped = मूलम्_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          सचे (! मूलम्_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  सचे (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    पटिनिवत्त crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "setresgid to `%d`", gid);

  पटिनिवत्त 0;
}

स्थिर bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  पटिनिवत्त mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, रिक्त, रिक्त);
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
स्थिर पूर्णाङ्क
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  संरचना libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close पूर्णाङ्क socket_parent = -1;
  cleanup_close पूर्णाङ्क socket_child = -1;
  cleanup_close पूर्णाङ्क मूलम्_fd = -1;
  cleanup_close पूर्णाङ्क cwd_fd = -1;
  size_t i, missing = 0;
  पूर्णाङ्क saved_errno;
  पूर्णाङ्क sockets[2];
  pid_t pid;
  पूर्णाङ्क ret;

  सचे (def->मूलम् == रिक्त || def->मूलम्->path == रिक्त)
    पटिनिवत्त 0;

  अत्थाय (i = 0; i < def->mounts_len; i++)
    सचे (is_proc_mount (def->mounts[i]))
      missing++;
  सचे (missing == 0)
    पटिनिवत्त 1;

  मूलम्_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  सचे (UNLIKELY (मूलम्_fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  सचे (UNLIKELY (cwd_fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->मूलम्->path);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त 0;

  pid = fork ();
  saved_errno = errno;
  सचे (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      सचे (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (शून्य) unshare (CLONE_NEWNS);

      अत्थाय (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close पूर्णाङ्क fd = -1;

          सचे (! is_proc_mount (def->mounts[i]))
            अनुवर्त;

          fd = open_mount_of_type (container, def->mounts[i], err);
          सचे (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (वर्ण *) &i, परिमाण (i), err);
          सचे (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  सचे (UNLIKELY (fchdir (मूलम्_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    पटिनिवत्त crun_make_error (err, errno, "restore the root directory");

  सचे (UNLIKELY (pid < 0))
    पटिनिवत्त crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  याव (missing > 0)
    {
      libcrun_error_t tmp_err = रिक्त;

      ret = receive_fd_from_socket_with_payload (socket_parent, (वर्ण *) &i, परिमाण (i), &tmp_err);
      सचे (ret < 0)
        {
          crun_error_release (&tmp_err);
          भङ्ग;
        }
      सचे (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          भङ्ग;
        }

      सचे (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, रिक्त, 0);

  पटिनिवत्त missing == 0 ? 1 : 0;
}

स्थिर पूर्णाङ्क
init_container (libcrun_container_t *container, पूर्णाङ्क sync_socket_container, संरचना init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  संरचना libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close पूर्णाङ्क timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  पूर्णाङ्क ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      सचे (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      सचे (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      सचे (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          delayed_pidns_fork = ret > 0;
        }

      सचे (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          ret = TEMP_FAILURE_RETRY (लिख (sync_socket_container, &new_pid, परिमाण (new_pid)));
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "write to sync socket");
        }
      अञ्ञथा
        {
          new_pid = fork ();
          सचे (UNLIKELY (new_pid < 0))
            पटिनिवत्त crun_make_error (err, errno, "fork");
        }

      सचे (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          सचे (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (लिख (sync_socket_container, &new_pid, परिमाण (new_pid)));
          सचे (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = libcrun_set_oom (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  सचे ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      अत्थाय (i = 0; i < def->mounts_len; i++)
        {
          पूर्णाङ्क fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          सचे (def->mounts[i]->type == रिक्त)
            अनुवर्त;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          सचे (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, रिक्त, रिक्त, रिक्त);
          सचे (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, रिक्त, रिक्त, रिक्त);

          सचे (fd >= 0)
            {
              सचे (mount_fds->fds[i] >= 0)
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
  सचे (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      सचे (UNLIKELY (timens_offsets_fd < 0))
        पटिनिवत्त timens_offsets_fd;
    }

  सचे (init_status->all_namespaces & CLONE_NEWUSER)
    {
      सचे (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      सचे (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
      अञ्ञथा
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "unshare");
    }

  सचे (def->linux->time_offsets)
    {
      वर्ण fmt_buffer[128];
      पूर्णाङ्क fd = timens_offsets_fd;

      सचे (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, परिमाण (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          सचे (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (fmt_buffer)))
            पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long पूर्णाङ्क) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = लिख (fd, fmt_buffer, strlen (fmt_buffer));
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      सचे (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long पूर्णाङ्क) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, परिमाण (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          सचे (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (fmt_buffer)))
            पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");

          ret = लिख (fd, fmt_buffer, strlen (fmt_buffer));
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  सचे (init_status->must_fork)
    {
      सचे (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          delayed_pidns_fork = ret > 0;
        }

      सचे (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          सचे (UNLIKELY (pid_container < 0))
            पटिनिवत्त crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      सचे (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          ret = TEMP_FAILURE_RETRY (लिख (sync_socket_container, &pid_container, परिमाण (pid_container)));
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "write to sync socket");

          सचे (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  पटिनिवत्त get_private_data (container)->delayed_pidns_fork;
}

पूर्णाङ्क
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क client_fd = -1;
  cleanup_close पूर्णाङ्क pidfd = -1;
  ध्रुव वर्ण *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  सचे (v == रिक्त)
    पटिनिवत्त 0;

  pidfd = syscall_pidfd_open (pid, 0);
  सचे (UNLIKELY (pidfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  सचे (UNLIKELY (client_fd < 0))
    पटिनिवत्त client_fd;

  पटिनिवत्त send_fd_to_socket (client_fd, pidfd, err);
}

स्थिर bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  सचे (process == रिक्त || process->exec_cpu_affinity == रिक्त)
    पटिनिवत्त false;
  पटिनिवत्त (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, शून्य *args,
                             पूर्णाङ्क *sync_socket_out, संरचना libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) संरचना init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णाङ्क sync_socket_container = -1;
  वर्ण *notify_socket_env = रिक्त;
  cleanup_close पूर्णाङ्क sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close पूर्णाङ्क restore_pidns = -1;
  पूर्णाङ्क first_clone_args = 0;
  पूर्णाङ्क sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  पूर्णाङ्क ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  सचे (def->मूलम्)
    {
      ret = do_notify_socket (container, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  सचे (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = libcrun_set_oom (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->मूलम् ? container->container_def->मूलम्->path : रिक्त, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  अत्थाय (i = 0; i < init_status.fd_len; i++)
    {
      विकल्प (init_status.मानम्[i])
        {
        पक्ष CLONE_NEWIPC:
          सचे (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          भङ्ग;

        पक्ष CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          सचे (! new_userns)
            init_status.must_fork = true;
          अञ्ञथा
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          भङ्ग;

        पक्ष CLONE_NEWTIME:
          सचे (! new_userns)
            init_status.must_fork = true;
          अञ्ञथा
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          भङ्ग;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  सचे ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  अञ्ञथा सचे ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  सचे (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  सचे (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      संरचना _clone3_args clone3_args;
      memset (&clone3_args, 0, परिमाण (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      सचे (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  सचे (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, रिक्त);
      सचे (UNLIKELY (pid < 0))
        पटिनिवत्त crun_make_error (err, errno, "clone");
    }

  सचे (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = close_and_reset (&sync_socket_container);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_वृक्षः_fd));

      सचे (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          ret = TEMP_FAILURE_RETRY (पठ (sync_socket_host, &new_pid, परिमाण (new_pid)));
          सचे (UNLIKELY (ret != परिमाण (new_pid)))
            पटिनिवत्त crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          सचे (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          अञ्ञथा
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, रिक्त, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      सचे (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      सचे ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      सचे (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          ret = TEMP_FAILURE_RETRY (पठ (sync_socket_host, &grandchild, परिमाण (grandchild)));
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          सचे (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          अञ्ञथा
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, रिक्त, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      सचे (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      pid_to_clean = 0;
      पटिनिवत्त pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  सचे (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  सचे (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  अञ्ञथा
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      सचे (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->सन्देशः);
    }

  /* Jump into the specified entrypoint.  */
  सचे (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  सचे (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->सन्देशः);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  सचे (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

स्थिर पूर्णाङ्क
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, पूर्णाङ्क sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, ध्रुव वर्ण *sub_cgroup,
                            पूर्णाङ्क *terminal_fd, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  वर्ण res;
  pid_t pid;
  cleanup_close पूर्णाङ्क sync_fd = sync_socket_fd;

  सचे (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (पठ (sync_fd, &res, 1));
  सचे (UNLIKELY (ret != 1))
    पटिनिवत्त crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  सचे (res != '0')
    पटिनिवत्त crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (पठ (sync_fd, &pid, परिमाण (pid)));
  सचे (UNLIKELY (ret != परिमाण (pid)))
    पटिनिवत्त crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, रिक्त, 0);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "waitpid for exec child pid");

  सचे (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (need_move_to_cgroup)
    {
      cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free वर्ण *final_cgroup = रिक्त;

      सचे (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, रिक्त);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_set_io_priority (pid, process, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (लिख (sync_fd, &ret, परिमाण (ret)));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "write to sync socket");

  सचे (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      सचे (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क err_code = 0;
          cleanup_free वर्ण *err_str = रिक्त;

          सचे (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              सचे (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  पटिनिवत्त crun_make_error (err, err_code, "%s", err_str);
                }
            }

          पटिनिवत्त crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  पटिनिवत्त pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
स्थिर पूर्णाङ्क
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णाङ्क pidfd_pid_to_join = -1;
  पूर्णाङ्क all_flags = 0;
  size_t i;
  पूर्णाङ्क ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  सचे (def->linux && def->linux->namespaces)
    {
      अत्थाय (i = 0; i < def->linux->namespaces_len; i++)
        सचे (! is_empty_string (def->linux->namespaces[i]->path))
          पटिनिवत्त 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  सचे (UNLIKELY (pidfd_pid_to_join < 0))
    पटिनिवत्त 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;
  सचे (ret == 0)
    पटिनिवत्त crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  अत्थाय (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].मानम्;

  ret = setns (pidfd_pid_to_join, all_flags);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त 0;

  पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क fds_joined[MAX_NAMESPACES] = {
    0,
  };
  पूर्णाङ्क fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  पूर्णाङ्क ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;
  /* Nothing left to do if the namespaces were joined.  */
  सचे (LIKELY (ret > 0))
    पटिनिवत्त 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  सचे (def->linux->namespaces_len >= MAX_NAMESPACES)
    पटिनिवत्त crun_make_error (err, 0, "invalid configuration");

  अत्थाय (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free वर्ण *ns_path = रिक्त;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      सचे (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          सचे (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              अनुवर्त;
            }

          ret = fds[i];
          गच्छ exit;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ exit;
  सचे (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      गच्छ exit;
    }

  अत्थाय (i = 0; namespaces[i].ns_file; i++)
    {
      सचे (namespaces[i].मानम् == CLONE_NEWUSER)
        अनुवर्त;

      ret = setns (fds[i], 0);
      सचे (ret == 0)
        fds_joined[i] = 1;
    }
  अत्थाय (i = 0; namespaces[i].ns_file; i++)
    {
      सचे (fds_joined[i])
        अनुवर्त;
      ret = setns (fds[i], 0);
      सचे (ret == 0)
        fds_joined[i] = 1;
    }
  अत्थाय (i = 0; namespaces[i].ns_file; i++)
    {
      सचे (fds_joined[i])
        अनुवर्त;
      ret = setns (fds[i], 0);
      सचे (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          अत्थाय (j = 0; j < def->linux->namespaces_len; j++)
            {
              सचे (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  भङ्ग;
                }
            }
          सचे (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              अनुवर्त;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          गच्छ exit;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

exit:
  अत्थाय (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  पटिनिवत्त ret;
}

पूर्णाङ्क
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      ध्रुव वर्ण *sub_cgroup,
                      पूर्णाङ्क detach,
                      runtime_spec_schema_config_schema_process *process,
                      पूर्णाङ्क *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  पूर्णाङ्क ret;
  पूर्णाङ्क sync_socket_fd[2];
  cleanup_close पूर्णाङ्क cgroup_dirfd = -1;
  cleanup_close पूर्णाङ्क sync_fd = -1;
  संरचना _clone3_args clone3_args;
  bool need_move_to_cgroup;

  सचे (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "set child subreaper");
    }

  सचे (path_has_dot_dot_component (sub_cgroup))
    पटिनिवत्त crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  सचे (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free वर्ण *final_cgroup = रिक्त;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    सचे (get_force_cgroup_v1_annotation (container) == रिक्त)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        सचे (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, परिमाण (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  सचे (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  अञ्ञथा
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  सचे (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  /* On errors, fall back to fork().  */
  सचे (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      सचे (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          गच्छ exit;
        }
    }

  सचे (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      पटिनिवत्त join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  सचे (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (लिख (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->सन्देशः);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  सचे (UNLIKELY (pid < 0))
    {
      पूर्णाङ्क saved_errno = errno;
      TEMP_FAILURE_RETRY (लिख (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  सचे (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (लिख (sync_fd, "0", 1));
      सचे (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (लिख (sync_fd, &pid, परिमाण (pid)));
      सचे (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  अञ्ञथा
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free वर्ण *pty = रिक्त;
      पूर्णाङ्क r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (पठ (sync_fd, &r, परिमाण (r)));
      सचे (UNLIKELY (ret != परिमाण (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      सचे (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      सचे (terminal_fd)
        {
          cleanup_close पूर्णाङ्क ptmx_fd = -1;

          ret = set_id_init (container, err);
          सचे (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          सचे (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          सचे (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  पटिनिवत्त 0;

exit:
  सचे (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  सचे (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  पटिनिवत्त ret;
}

पूर्णाङ्क
libcrun_linux_container_update (libcrun_container_status_t *status, ध्रुव वर्ण *state_मूलम्, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

  cgroup_status = libcrun_cgroup_make_status (status);

  पटिनिवत्त libcrun_update_cgroup_resources (cgroup_status, state_मूलम्, resources, err);
}

स्थिर पूर्णाङ्क
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, ध्रुव bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

  cgroup_status = libcrun_cgroup_make_status (status);

  पटिनिवत्त libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

पूर्णाङ्क
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  पटिनिवत्त libcrun_container_pause_unpause_linux (status, true, err);
}

पूर्णाङ्क
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  पटिनिवत्त libcrun_container_pause_unpause_linux (status, false, err);
}

पूर्णाङ्क
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  पूर्णाङ्क ret;

  सचे (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  अञ्ञथा सचे (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  अञ्ञथा
    पटिनिवत्त crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, 0, "personality to `%s`", p->domain);

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णाङ्क sockfd = -1;

  अत्थाय (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णाङ्क मानम् = libcrun_find_namespace (def->linux->namespaces[i]->type);
      सचे (UNLIKELY (मानम् < 0))
        पटिनिवत्त crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      सचे (मानम् == CLONE_NEWNET && def->linux->namespaces[i]->path == रिक्त)
        {
          configure_network = true;
          भङ्ग;
        }
    }

  सचे (! configure_network)
    पटिनिवत्त 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  सचे (LIKELY (sockfd >= 0))
    {
      संरचना ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  अञ्ञथा
    {
      संरचना nlmsghdr *hdr_recv;
      वर्ण buf[परिमाण (संरचना nlmsghdr) + परिमाण (संरचना nlmsgerr)];
      संरचना sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      संरचना sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      संरचना nlmsghdr hdr = {
        .nlmsg_len = परिमाण (संरचना nlmsghdr) + परिमाण (संरचना ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      संरचना ifinfomsg सन्देशः = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((संरचना nlmsghdr *) buf) = hdr;
      *((संरचना ifinfomsg *) (buf + परिमाण (संरचना nlmsghdr))) = सन्देशः;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      सचे (UNLIKELY (sockfd < 0))
        पटिनिवत्त crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (संरचना sockaddr *) &addr, परिमाण (addr));
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, परिमाण (buf), 0, (संरचना sockaddr *) &addr_to, परिमाण (addr_to));
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, परिमाण (buf), 0, रिक्त, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (संरचना nlmsghdr *) buf;

      सचे (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          संरचना nlmsgerr *err_data = (संरचना nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          सचे (err_data->error < 0)
            पटिनिवत्त crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  पटिनिवत्त 0;
}

/* Protection for attacks like CVE-2019-5736.  */
पूर्णाङ्क ensure_cloned_द्विमान ();
__attribute__ ((constructor)) स्थिर शून्य
libcrun_rexec (शून्य)
{
  सचे (ensure_cloned_द्विमान () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

पूर्णाङ्क
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  पटिनिवत्त libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

पूर्णाङ्क
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
स्थिर पूर्णाङ्क
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, पूर्णाङ्क सङ्केत, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  सचे (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      /* The pid is not valid anymore, return an error.  */
      सचे (ret == 0)
        {
          errno = ESRCH;
          पटिनिवत्त crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, सङ्केत);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "kill container");
  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_kill_linux (libcrun_container_status_t *status, पूर्णाङ्क सङ्केत, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  सचे (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      सचे (errno == ENOSYS)
        पटिनिवत्त libcrun_kill_linux_no_pidfd (status, true, सङ्केत, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      सचे (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        पटिनिवत्त crun_make_error (err, ESRCH, "process not running");
      पटिनिवत्त crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* The pid is not valid anymore, return an error.  */
  सचे (ret == 0)
    {
      errno = ESRCH;
      पटिनिवत्त crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, सङ्केत, रिक्त, 0);
  सचे (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      सचे (errno == ENOSYS)
        पटिनिवत्त libcrun_kill_linux_no_pidfd (status, false, सङ्केत, err);
      पटिनिवत्त crun_make_error (err, errno, "send signal to pidfd");
    }

  पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण *
libcrun_get_intelrdt_name (ध्रुव वर्ण *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  सचे (def == रिक्त || def->linux == रिक्त || def->linux->intel_rdt == रिक्त || def->linux->intel_rdt->clos_id == रिक्त)
    {
      सचे (explicit)
        *explicit = false;
      पटिनिवत्त ctr_name;
    }

  सचे (explicit)
    *explicit = true;
  पटिनिवत्त def->linux->intel_rdt->clos_id;
}

पूर्णाङ्क
libcrun_apply_intelrdt (ध्रुव वर्ण *ctr_name, libcrun_container_t *container, pid_t pid, पूर्णाङ्क actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = रिक्त;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  ध्रुव वर्ण *name;
  पूर्णाङ्क ret;

  सचे (container)
    def = container->container_def;

  सचे (def == रिक्त || def->linux == रिक्त || def->linux->intel_rdt == रिक्त)
    पटिनिवत्त 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  सचे (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      सचे (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          सचे (UNLIKELY (ret < 0))
            गच्छ fail;

          has_monitoring = true;
        }
    }

  सचे (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  सचे (actions & LIBCRUN_INTELRDT_MOVE)
    {
      ध्रुव वर्ण *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : रिक्त;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  पटिनिवत्त 0;

fail:
  सचे (has_monitoring)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      सचे (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  सचे (created)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      सचे (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  पटिनिवत्त ret;
}

पूर्णाङ्क
libcrun_destroy_intelrdt (ध्रुव वर्ण *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  ध्रुव वर्ण *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  सचे (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      पूर्णाङ्क ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      सचे (ret < 0)
        पटिनिवत्त ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  सचे (explicit)
    पटिनिवत्त 0;

  पटिनिवत्त resctl_destroy (clos_id, err);
}

पूर्णाङ्क
libcrun_update_intel_rdt (ध्रुव वर्ण *ctr_name, libcrun_container_t *container, ध्रुव वर्ण *l3_cache_schema, ध्रुव वर्ण *mem_bw_schema, वर्ण *ध्रुव *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = रिक्त;
  ध्रुव वर्ण *name;

  सचे (container)
    def = container->container_def;

  सचे (def == रिक्त || def->linux == रिक्त || def->linux->intel_rdt == रिक्त)
    पटिनिवत्त 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, रिक्त);

  पटिनिवत्त resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
पूर्णाङ्क
libcrun_safe_chdir (ध्रुव वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *buffer = रिक्त;
  पूर्णाङ्क ret;

  ret = chdir (path);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  सचे (ret == 0 || buffer[0] != '/')
    {
      सचे (chdir ("/") < 0)
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
#define UNREACHABLE_LEN ((पूर्णाङ्क) परिमाण (UNREACHABLE) - 1)

      सचे ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        पटिनिवत्त crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      पटिनिवत्त crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  पटिनिवत्त 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
स्थिर पूर्णाङ्क
run_callback_in_namespace (पूर्णाङ्क mntns_fd, पूर्णाङ्क (*callback) (शून्य *, libcrun_error_t *), शून्य *arg, libcrun_error_t *err)
{
  पूर्णाङ्क wait_status = 0;
  pid_t pid;
  पूर्णाङ्क ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  सचे (UNLIKELY (pid < 0))
    पटिनिवत्त crun_make_error (err, errno, "vfork");

  सचे (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      सचे (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      सचे (UNLIKELY (ret < 0))
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
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  पटिनिवत्त get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
स्थिर पूर्णाङ्क
check_container_userns (libcrun_container_t *container, पूर्णाङ्क userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close पूर्णाङ्क expected_fd = -1;
  cleanup_close पूर्णाङ्क parent_fd = -1;
  संरचना stat st, expected_st;
  ध्रुव वर्ण *path = रिक्त;
  size_t i;
  पूर्णाङ्क ret;

  सचे (def->linux)
    अत्थाय (i = 0; i < def->linux->namespaces_len; i++)
      सचे (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == रिक्त;
          भङ्ग;
        }

  सचे (path == रिक्त)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      सचे (UNLIKELY (expected_fd < 0))
        पटिनिवत्त expected_fd;
    }
  अञ्ञथा
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      सचे (UNLIKELY (expected_fd < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  सचे (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      सचे (UNLIKELY (parent_fd < 0))
        पटिनिवत्त crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fstat user namespace");

  सचे (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    पटिनिवत्त crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  पटिनिवत्त 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
स्थिर पूर्णाङ्क
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, पूर्णाङ्क *mntns_fd, पूर्णाङ्क *owner_fd, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क mntns = -1;
  cleanup_close पूर्णाङ्क owner = -1;
  पूर्णाङ्क ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  सचे (UNLIKELY (mntns < 0))
    पटिनिवत्त mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (ret == 0)
    पटिनिवत्त crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  सचे (UNLIKELY (owner < 0))
    पटिनिवत्त crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, पूर्णाङ्क (*callback) (शून्य *, libcrun_error_t *), शून्य *arg, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क mntns_fd = -1;
  cleanup_close पूर्णाङ्क owner_fd = -1;
  पूर्णाङ्क ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त run_callback_in_namespace (mntns_fd, callback, arg, err);
}

संरचना umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
स्थिर पूर्णाङ्क
umount_nofollow (ध्रुव वर्ण *destination, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क parentfd = -1;
  cleanup_close पूर्णाङ्क cwdfd = -1;
  वर्ण buffer[PATH_MAX];
  ध्रुव वर्ण *parent, *name;
  वर्ण *last_slash;
  size_t len;
  पूर्णाङ्क saved_errno;
  पूर्णाङ्क ret;

  सचे (is_empty_string (destination))
    पटिनिवत्त crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  सचे (UNLIKELY (len >= परिमाण (buffer)))
    पटिनिवत्त crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  याव (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  सचे (last_slash == रिक्त)
    {
      parent = ".";
      name = buffer;
    }
  अञ्ञथा सचे (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  अञ्ञथा
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  सचे (UNLIKELY (parentfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  सचे (UNLIKELY (cwdfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  सचे (UNLIKELY (fchdir (cwdfd) < 0))
    पटिनिवत्त crun_make_error (err, errno, "fchdir to the previous working directory");

  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, saved_errno, "umount `%s`", destination);

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
do_umount_in_a_container (शून्य *arg, libcrun_error_t *err)
{
  संरचना umount_in_a_container_args *args = arg;
  size_t i;
  पूर्णाङ्क ret;

  अत्थाय (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}

संरचना mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  संरचना libcrun_fd_map *fds;
  पूर्णाङ्क pidfd;
  पूर्णाङ्क owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
स्थिर पूर्णाङ्क
open_destination_nofollow (ध्रुव वर्ण *destination, bool dir, libcrun_error_t *err)
{
  ध्रुव uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  पूर्णाङ्क fd;
  पूर्णाङ्क ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  सचे (LIKELY (fd >= 0))
    पटिनिवत्त fd;

  सचे (errno != ENOENT)
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", destination);

  सचे (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  अञ्ञथा
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", destination);

  पटिनिवत्त fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
स्थिर पूर्णाङ्क
create_destinations_in_a_container (शून्य *arg, libcrun_error_t *err)
{
  संरचना mount_in_a_container_args *args = arg;
  size_t i;
  पूर्णाङ्क ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  सचे (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "setresuid to `0`");
    }
  अञ्ञथा सचे (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      पटिनिवत्त crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  अत्थाय (i = 0; i < args->len; i++)
    {
      cleanup_close पूर्णाङ्क dest_fd = -1;
      संरचना stat st;

      ret = fstat (args->fds->fds[i], &st);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      सचे (UNLIKELY (dest_fd < 0))
        पटिनिवत्त dest_fd;
    }

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
do_mount_in_a_container (शून्य *arg, libcrun_error_t *err)
{
  संरचना mount_in_a_container_args *args = arg;
  size_t i;
  पूर्णाङ्क ret;

  अत्थाय (i = 0; i < args->len; i++)
    {
      cleanup_close पूर्णाङ्क dest_fd = -1;
      संरचना stat st;

      ret = fstat (args->fds->fds[i], &st);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      सचे (UNLIKELY (dest_fd < 0))
        पटिनिवत्त dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  पटिनिवत्त 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
स्थिर पूर्णाङ्क
send_mounts_from_container_namespaces (libcrun_container_t *container, पूर्णाङ्क pidfd, runtime_spec_schema_defs_mount **mounts,
                                       संरचना libcrun_fd_map *fds, size_t len, पूर्णाङ्क flags, पूर्णाङ्क owner_fd, पूर्णाङ्क sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  पूर्णाङ्क ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  सचे (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  अञ्ञथा सचे (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      पटिनिवत्त crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  सचे (UNLIKELY (pid < 0))
    पटिनिवत्त crun_make_error (err, errno, "fork");

  सचे (pid)
    {
      पूर्णाङ्क wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      सचे (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, रिक्त, 0));
          पटिनिवत्त crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  अत्थाय (i = 0; i < len; i++)
    {
      cleanup_close पूर्णाङ्क fd = -1;

      सचे (fds->fds[i] >= 0)
        अनुवर्त;

      fd = open_mount_of_type (container, mounts[i], err);
      सचे (UNLIKELY (fd < 0))
        पटिनिवत्त crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
स्थिर पूर्णाङ्क
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, पूर्णाङ्क *flags, libcrun_error_t *err)
{
  स्थिर ध्रुव संरचना
  {
    ध्रुव वर्ण *name;
    पूर्णाङ्क flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  अत्थाय (i = 0; i < परिमाण (namespaces) / परिमाण (namespaces[0]); i++)
    {
      cleanup_close पूर्णाङ्क self_fd = -1;
      cleanup_close पूर्णाङ्क target_fd = -1;
      संरचना stat self_st, target_st;
      cleanup_free वर्ण *self_path = रिक्त;
      पूर्णाङ्क ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      सचे (UNLIKELY (self_fd < 0))
        पटिनिवत्त self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      सचे (UNLIKELY (target_fd < 0))
        पटिनिवत्त target_fd;

      ret = fstat (self_fd, &self_st);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      सचे (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  पटिनिवत्त 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
स्थिर पूर्णाङ्क
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, पूर्णाङ्क owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, संरचना libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क sync_fd = -1;
  cleanup_close पूर्णाङ्क pidfd = -1;
  पूर्णाङ्क sync_socket_fd[2];
  bool needed = false;
  पूर्णाङ्क flags;
  pid_t pid;
  size_t i;
  पूर्णाङ्क ret;

  अत्थाय (i = 0; i < len; i++)
    {
      सचे (fds->fds[i] >= 0)
        अनुवर्त;

      सचे (mounts[i]->type == रिक्त)
        पटिनिवत्त crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  सचे (! needed)
    पटिनिवत्त 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  सचे (UNLIKELY (pidfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (ret == 0)
    पटिनिवत्त crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  सचे (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      पटिनिवत्त ret;
    }

  सचे (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      सचे (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  अत्थाय (i = 0; i < len; i++)
    {
      सचे (fds->fds[i] >= 0)
        अनुवर्त;

      ret = receive_fd_from_socket (sync_fd, err);
      सचे (UNLIKELY (ret < 0))
        {
          cleanup_free वर्ण *err_str = रिक्त;
          पूर्णाङ्क err_code = 0;

          सचे (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          अञ्ञथा
            ret = crun_error_wrap (err, "receive mount fd");
          भङ्ग;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, रिक्त, 0));

  पटिनिवत्त ret;
}

स्थिर unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free वर्ण *data = रिक्त;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  सचे (mount->options == रिक्त)
    पटिनिवत्त get_default_flags (container, mount->destination, &data);

  अत्थाय (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  पटिनिवत्त flags;
}

पूर्णाङ्क
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  संरचना mount_in_a_container_args args;
  cleanup_close_map संरचना libcrun_fd_map *fds = रिक्त;
  cleanup_close पूर्णाङ्क pidfd = -1;
  cleanup_close पूर्णाङ्क mntns_fd = -1;
  cleanup_close पूर्णाङ्क owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  पूर्णाङ्क ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  fds = make_libcrun_fd_map (len);

  अत्थाय (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), रिक्त, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      सचे (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          सचे (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          सचे (UNLIKELY (fds->fds[i] < 0))
            पटिनिवत्त fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  अत्थाय (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  सचे (UNLIKELY (ret > 0))
    पटिनिवत्त *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

पूर्णाङ्क
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  संरचना umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  पटिनिवत्त run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

संरचना restrict_mounts_in_a_container_args
{
  संरचना libcrun_mount_flags_s *mounts;
  size_t len;
  पूर्णाङ्क rootfd;
};

स्थिर पूर्णाङ्क
do_restrict_mounts_in_a_container (शून्य *arg, libcrun_error_t *err)
{
  संरचना restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  पूर्णाङ्क ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  सचे (LIKELY (ret == 0))
    ret = chroot (".");
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "chroot to the container root");

  अत्थाय (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      ध्रुव वर्ण *target = consume_slashes (args->mounts[i].target);
      cleanup_close पूर्णाङ्क fd = -1;
      संरचना statfs sfs;
      unsigned long current_flags;

      सचे (flags == 0)
        अनुवर्त;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      सचे (UNLIKELY (fd < 0))
        {
          सचे (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              अनुवर्त;
            }
          पटिनिवत्त fd;
        }

      ret = fstatfs (fd, &sfs);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      सचे ((current_flags & flags) == flags)
        अनुवर्त;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      सचे (LIKELY (ret == 0))
        अनुवर्त;

      सचे (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        पटिनिवत्त ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (रिक्त, target, रिक्त, MS_REMOUNT | MS_BIND | flags | current_flags, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "remount `/%s`", target);
    }

  पटिनिवत्त 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
पूर्णाङ्क
libcrun_restrict_mounts_in_container (pid_t pid, संरचना libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  संरचना restrict_mounts_in_a_container_args args;
  cleanup_close पूर्णाङ्क mntns_fd = -1;
  cleanup_close पूर्णाङ्क rootfd = -1;
  वर्ण path[64];
  पूर्णाङ्क ret;

  snprintf (path, परिमाण (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  सचे (UNLIKELY (mntns_fd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, परिमाण (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  सचे (UNLIKELY (rootfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  सचे (UNLIKELY (ret > 0))
    पटिनिवत्त *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  पटिनिवत्त ret;
}

पूर्णाङ्क
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णाङ्क netns_fd = -1;
  size_t i;
  पूर्णाङ्क ret;

  सचे (def == रिक्त || def->linux == रिक्त || def->linux->net_devices == रिक्त)
    पटिनिवत्त 0;

  cleanup_free वर्ण *ns_path = रिक्त;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  सचे (UNLIKELY (netns_fd < 0))
    पटिनिवत्त netns_fd;

  अत्थाय (i = 0; i < def->linux->net_devices->len; i++)
    {
      ध्रुव वर्ण *new_name = def->linux->net_devices->मानानि[i]->name ?: def->linux->net_devices->कुञ्चिकाः[i];

      ret = move_network_device (def->linux->net_devices->कुञ्चिकाः[i], new_name, netns_fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}
