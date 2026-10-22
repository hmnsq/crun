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
#जइ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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

  जइ (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  जइ (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  जइ (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  जइ (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  जइ (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  जइ (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  जइ (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  जइ (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  जइ (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  प्रत्यागम flags;
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

  जइ (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  जइ (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  जइ (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  जइ (p->old_मूलम्_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_मूलम्_fd));
  जइ (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  जइ (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  जइ (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  जइ (p->copy_symlink_targets)
    {
      size_t i;
      प्रति (i = 0; i < p->n_copy_symlink_targets; i++)
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
  जइ (container->private_data == रिक्त)
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
  प्रत्यागम container->private_data;
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
  प्रत्यागम tmp;
}

पूर्णाङ्क
libcrun_find_namespace (ध्रुव वर्ण *name)
{
  संरचना linux_namespace_s *it;
  प्रति (it = namespaces; it->name; it++)
    जइ (strcmp (it->name, name) == 0)
      प्रत्यागम it->मानम्;
  प्रत्यागम -1;
}

स्थिर पूर्णाङ्क
syscall_keyctl_join (ध्रुव वर्ण *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

स्थिर पूर्णाङ्क
syscall_pidfd_open (pid_t pid, unsigned पूर्णाङ्क flags)
{
#जइ defined __NR_pidfd_open
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_pidfd_open, pid, flags);
#अण्णहा
  (शून्य) pid;
  (शून्य) flags;
  errno = ENOSYS;
  प्रत्यागम -1;
#endif
}

स्थिर पूर्णाङ्क
syscall_pidfd_send_signal (पूर्णाङ्क pidfd, पूर्णाङ्क sig, siginfo_t *info, unsigned पूर्णाङ्क flags)
{
#जइ defined __NR_pidfd_send_signal
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#अण्णहा
  (शून्य) pidfd;
  (शून्य) sig;
  (शून्य) info;
  (शून्य) flags;
  errno = ENOSYS;
  प्रत्यागम -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
स्थिर uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  जइ (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  जइ (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  जइ (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  जइ (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  जइ (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  जइ (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  जइ (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  जइ (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  प्रत्यागम attr;
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
  जइ (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      जइ (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      अण्णहा जइ (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      अण्णहा
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      जइ (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  अण्णहा जइ (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "mount_setattr `/%s`", target);

  प्रत्यागम 0;
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
  जइ (UNLIKELY (src == रिक्त))
    प्रत्यागम crun_make_error (err, EINVAL, "mount source is not specified");

  जइ (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  जइ (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_वृक्षः_fd = syscall_open_वृक्षः (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  जइ (UNLIKELY (open_वृक्षः_fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_वृक्षः_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "mount_setattr `%s`", src);

  प्रत्यागम get_and_reset (&open_वृक्षः_fd);
}

पूर्णाङ्क
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, वर्ण *option, वर्ण **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free वर्ण *mappings = रिक्त;
  ध्रुव वर्ण *it;
  पूर्णाङ्क ret;

  mappings = xmalloc (allocated);

  प्रति (it = option; *it;)
    {
      bool relative = false;
      long मानम्[3];
      size_t i;

      जइ (*it == '\0')
        भङ्ग;

      जइ (*it == '#')
        it++;

      जइ (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      प्रति (i = 0; i < 3; i++)
        {
          वर्ण *endptr = रिक्त;

          जइ (i > 0 && *it == '-')
            it++;

          जइ (*it == '\0')
            प्रत्यागम crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          मानम्[i] = strtol (it, &endptr, 10);
          जइ (errno || endptr == it)
            प्रत्यागम crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      जइ (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          जइ (def == रिक्त
              || def->linux == रिक्त
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            प्रत्यागम crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          प्रति (i = 0; i < mappings_len; i++)
            जइ (मानम्[1] >= (int64_t) mappings[i]->container_id
                && मानम्[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              भङ्ग;

          जइ (i == mappings_len)
            प्रत्यागम crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          मानम्[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      जइ (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", मानम्[0], मानम्[1], मानम्[2]);
      जइ (UNLIKELY (ret >= (पूर्णाङ्क) (allocated - written)))
        प्रत्यागम crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = रिक्त;

  प्रत्यागम 0;
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
  प्रति (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      जइ (UNLIKELY (len >= max_len_mapping))
        प्रत्यागम crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = रिक्त;
  प्रत्यागम 0;
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

  प्रत्यागम format_mount_mappings (ret, mappings, 1, written, err);
}

स्थिर bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  जइ (def->linux == रिक्त)
    प्रत्यागम mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  जइ (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    प्रत्यागम false;

  जइ (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    प्रत्यागम false;

  प्रति (s = 0; s < mnt->uid_mappings_len; s++)
    {
      जइ (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        प्रत्यागम false;
      जइ (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        प्रत्यागम false;
      जइ (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        प्रत्यागम false;
    }

  प्रति (s = 0; s < mnt->gid_mappings_len; s++)
    {
      जइ (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        प्रत्यागम false;
      जइ (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        प्रत्यागम false;
      जइ (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        प्रत्यागम false;
    }

  प्रत्यागम true;
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

  जइ (! need_new_userns)
    प्रत्यागम 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, रिक्त);
  जइ (UNLIKELY (pid < 0))
    प्रत्यागम crun_make_error (err, errno, "clone");

  जइ (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      जाव (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  जइ (mnt->uid_mappings_len)
    {
      cleanup_free वर्ण *uid_map = रिक्त;
      cleanup_free वर्ण *gid_map = रिक्त;
      size_t written = 0;
      पूर्णाङ्क ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      cleanup_close पूर्णाङ्क fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      जइ (UNLIKELY (fd < 0))
        प्रत्यागम fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      जइ (UNLIKELY (fd < 0))
        प्रत्यागम fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अण्णहा
    {
      cleanup_free वर्ण *dup_options = रिक्त;
      वर्ण *option, *saveptr = रिक्त;

      जइ (! options)
        प्रत्यागम crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      प्रति (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (रिक्त, ";", &saveptr))
        {
          cleanup_free वर्ण *mappings = रिक्त;
          cleanup_close पूर्णाङ्क map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          पूर्णाङ्क ret;

          जइ (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              जइ (UNLIKELY (map_fd < 0))
                प्रत्यागम map_fd;
            }
          अण्णहा जइ (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              जइ (UNLIKELY (map_fd < 0))
                प्रत्यागम map_fd;
            }
          अण्णहा
            प्रत्यागम crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_create_keyring (libcrun_container_t *container, ध्रुव वर्ण *name, ध्रुव वर्ण *label, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क labelfd = -1;
  bool label_set = false;
  पूर्णाङ्क ret;

  जइ (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      जइ (UNLIKELY (labelfd < 0))
        {
          जइ (crun_error_get_errno (err) != ENOENT)
            प्रत्यागम labelfd;

          crun_error_release (err);
        }

      जइ (labelfd >= 0)
        {
          ret = लिख (labelfd, label, strlen (label));
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  जइ (UNLIKELY (ret < 0))
    {
      जइ (errno == ENOSYS)
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
  जइ (label_set && लिख (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  प्रत्यागम ret;
}

स्थिर शून्य
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  जइ (def->process && def->process->user)
    {
      जइ (def->process->user->uid)
        *uid = def->process->user->uid;
      जइ (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

स्थिर unsigned long
get_mount_flags (ध्रुव वर्ण *name, पूर्णाङ्क current_flags, पूर्णाङ्क *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  ध्रुव संरचना propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  जइ (found)
    *found = prop ? 1 : 0;

  जइ (prop == रिक्त)
    प्रत्यागम 0;

  जइ (prop->extra_flags & OPTION_RECURSIVE)
    {
      जइ (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      जइ (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  जइ (extra_flags)
    *extra_flags |= prop->extra_flags;

  जइ (prop->clear)
    प्रत्यागम current_flags & ~prop->flags;

  प्रत्यागम current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
स्थिर bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  प्रति (i = 0; i < mount->options_len; i++)
    {
      ध्रुव संरचना propagation_flags_s *prop;

      जइ (mount->options[i] == रिक्त)
        अनुवर्त;

      prop = libcrun_str2mount_flags (mount->options[i]);
      जइ (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        प्रत्यागम true;
    }

  प्रत्यागम false;
}

स्थिर unsigned long
get_mount_flags_or_option (ध्रुव वर्ण *name, पूर्णाङ्क current_flags, unsigned long *extra_flags, वर्ण **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  पूर्णाङ्क found;
  __attribute__ ((unused)) cleanup_free वर्ण *prev = रिक्त;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  जइ (name == रिक्त)
    प्रत्यागम 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  जइ (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      ध्रुव वर्ण *data_flag = रिक्त;
      जइ ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      अण्णहा जइ ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      जइ (data_flag)
        {
          prev = *option;
          जइ (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          अण्णहा
            *option = xstrdup (data_flag);
        }
      प्रत्यागम flags;
    }

  prev = *option;
  जइ (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  अण्णहा
    *option = xstrdup (name);

  प्रत्यागम 0;
}

पूर्णाङ्क
pivot_मूलम् (ध्रुव वर्ण *new_मूलम्, ध्रुव वर्ण *put_old)
{
  प्रत्यागम syscall (__NR_pivot_root, new_मूलम्, put_old);
}

स्थिर शून्य
free_remount (संरचना remount_s *r)
{
  जइ (r == रिक्त)
    प्रत्यागम;
  जइ (r->targetfd >= 0)
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
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
do_remount (पूर्णाङ्क targetfd, ध्रुव वर्ण *target, unsigned long flags, ध्रुव वर्ण *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  पूर्णाङ्क ret;
  ध्रुव वर्ण *real_target = target;

  जइ (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      जइ (LIKELY (ret == 0))
        प्रत्यागम 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  जइ (flags & (MS_REMOUNT | MS_RDONLY))
    data = रिक्त;

  ret = mount (रिक्त, real_target, रिक्त, flags, data);
  जइ (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      संरचना statfs sfs;

      ret = statfs (real_target, &sfs);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      जइ (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      जइ ((flags | remount_flags) != flags)
        {
          ret = mount (रिक्त, real_target, रिक्त, flags | remount_flags, data);
          जइ (LIKELY (ret == 0))
            प्रत्यागम 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          जइ (current_flags & MS_RDONLY)
            ret = mount (रिक्त, real_target, रिक्त, flags | remount_flags | MS_RDONLY, data);
        }
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "remount `%s`", target);
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret = 0;
  संरचना remount_s *r = get_private_data (container)->remounts;
  जाव (r)
    {
      संरचना remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      जइ (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          जइ (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              अनुवर्त;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      जइ (UNLIKELY (ret < 0))
        गच्छ cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  जाव (r)
    {
      संरचना remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = रिक्त;
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
open_mount_target (libcrun_container_t *container, ध्रुव वर्ण *target_rel, libcrun_error_t *err)
{
  ध्रुव वर्ण *rootfs = get_private_data (container)->rootfs;
  पूर्णाङ्क rootfsfd = get_private_data (container)->rootfsfd;

  जइ (rootfsfd < 0)
    प्रत्यागम crun_make_error (err, 0, "invalid rootfs state");

  प्रत्यागम safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
स्थिर पूर्णाङ्क
fsopen_mount (ध्रुव वर्ण *type, ध्रुव वर्ण *source_name, ध्रुव वर्ण *labeltype, ध्रुव वर्ण *label, ध्रुव वर्ण *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close पूर्णाङ्क fsfd = -1;
  पूर्णाङ्क ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  जइ (UNLIKELY (fsfd < 0))
    प्रत्यागम fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (शून्य) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  जइ (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (data && data[0])
    {
      cleanup_free वर्ण *data_copy = xstrdup (data);
      वर्ण *saveptr = रिक्त;
      वर्ण *token;

      प्रति (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (रिक्त, ",", &saveptr))
        {
          वर्ण *eq = strchr (token, '=');
          जइ (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          अण्णहा
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, रिक्त, 0);

          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, रिक्त, रिक्त, 0);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#अण्णहा
  (शून्य) type;
  (शून्य) source_name;
  (शून्य) data;
  errno = ENOSYS;
  प्रत्यागम -1;
#endif
}

स्थिर पूर्णाङ्क
get_procfd (संरचना private_data_s *data, libcrun_error_t *err)
{
  पूर्णाङ्क fd;

  जइ (data->procfd >= 0)
    प्रत्यागम data->procfd;

  fd = fsopen_mount ("proc", रिक्त, रिक्त, रिक्त, रिक्त);
  जइ (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  जइ (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  प्रत्यागम fd;
}

स्थिर पूर्णाङ्क
get_old_मूलम्_fd (संरचना private_data_s *data)
{
  जइ (data->old_मूलम्_fd >= 0)
    प्रत्यागम data->old_मूलम्_fd;

  data->old_मूलम्_fd = open ("/", O_PATH | O_CLOEXEC);
  प्रत्यागम data->old_मूलम्_fd;
}

स्थिर पूर्णाङ्क
fs_move_mount_to (पूर्णाङ्क fd, पूर्णाङ्क dirfd, ध्रुव वर्ण *name)
{
#ifdef HAVE_NEW_MOUNT_API
  जइ (name)
    प्रत्यागम syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  प्रत्यागम syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#अण्णहा
  (शून्य) fd;
  (शून्य) dirfd;
  (शून्य) name;
  (शून्य) syscall_move_mount;
  errno = ENOSYS;
  प्रत्यागम -1;
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
  प्रति (i = 0; i < mnt->options_len; i++)
    जइ (mnt->options[i] != रिक्त && strcmp (mnt->options[i], option) == 0)
      प्रत्यागम true;
  प्रत्यागम false;
}

स्थिर runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, ध्रुव वर्ण *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  प्रति (i = 0; i < def->mounts_len; i++)
    {
      जइ (strcmp (def->mounts[i]->destination, destination) == 0)
        प्रत्यागम def->mounts[i];
    }
  प्रत्यागम रिक्त;
}

स्थिर शून्य
warn_tmpfs_fallback_once (संरचना private_data_s *private_data, ध्रुव वर्ण *reason)
{
  जइ (! private_data->maskdir_warned)
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
  जइ (private_data->maskdir_proc_path != रिक्त)
    {
      जइ (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      प्रत्यागम 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_मूलम्, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  जइ (fd < 0)
    प्रत्यागम fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  जइ (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  प्रत्यागम 0;
}

स्थिर bool
is_dev_null (ध्रुव संरचना stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  प्रत्यागम S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
स्थिर पूर्णाङ्क
ensure_dev_null (पूर्णाङ्क fd, libcrun_error_t *err)
{
  संरचना stat st;

  जइ (UNLIKELY (fstat (fd, &st) < 0))
    प्रत्यागम crun_make_error (err, errno, "stat `/dev/null`");

  जइ (UNLIKELY (! is_dev_null (&st)))
    प्रत्यागम crun_make_error (err, 0, "`/dev/null` is not the null device");

  प्रत्यागम 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
स्थिर bool
is_empty_ro_tmpfs (पूर्णाङ्क fd)
{
  संरचना statfs sfs;

  जइ (fstatfs (fd, &sfs) < 0)
    प्रत्यागम false;

  प्रत्यागम sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

स्थिर पूर्णाङ्क
mount_masked_dir (libcrun_container_t *container, पूर्णाङ्क pathfd, ध्रुव वर्ण *rel_path, libcrun_error_t *err)
{
  संरचना private_data_s *private_data = get_private_data (container);
  cleanup_close पूर्णाङ्क newfd = -1;
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क ret;

  जइ (private_data->maskdir_fd < 0)
    {
      जइ (private_data->maskdir_bind_failed)
        गच्छ fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, रिक्त, &tmp_err);
      जइ (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->सन्देशः);
          crun_error_release (&tmp_err);
          गच्छ fallback_to_tmpfs;
        }
    }

  जइ (private_data->maskdir_fd >= 0)
    {
      पूर्णाङ्क rootfsfd = private_data->rootfsfd;
      संरचना stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, रिक्त);
      जइ (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          जइ (UNLIKELY (newfd < 0))
            प्रत्यागम newfd;

          ret = fstat (newfd, &after);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          जइ (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            प्रत्यागम crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          प्रत्यागम 0;
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
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  जइ (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      जइ (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      अण्णहा
        {
          जइ (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  प्रत्यागम 0;
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
  जइ (UNLIKELY (rel_path == रिक्त))
    प्रत्यागम crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  जइ (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  जइ (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      जइ (errno != ENOENT && errno != EACCES)
        प्रत्यागम pathfd;

      जइ (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      प्रत्यागम 0;
    }

  जइ (readonly)
    {
      पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close पूर्णाङ्क mountfd = -1;

      जइ (UNLIKELY (procfd < 0))
        प्रत्यागम procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      जइ (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          जइ (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, रिक्त);
          अण्णहा
            ret = -1;

          जइ (ret < 0)
            crun_error_release (err);
        }

      जइ (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          जइ (keep_flags)
            {
              cleanup_close पूर्णाङ्क sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              जइ (UNLIKELY (sfd < 0))
                प्रत्यागम crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              जइ (UNLIKELY (ret < 0))
                प्रत्यागम crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          जइ (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              जइ (UNLIKELY (ret < 0))
                प्रत्यागम ret;

              ret = fs_move_mount_to (mountfd, pathfd, रिक्त);
              जइ (UNLIKELY (ret < 0))
                प्रत्यागम crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          अण्णहा
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, रिक्त, mount_flags, रिक्त,
                              LABEL_NONE, err);
              जइ (UNLIKELY (ret < 0))
                प्रत्यागम ret;
            }
        }
    }
  अण्णहा
    {
      ret = get_file_type_fd (pathfd, &mode);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      जइ ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      अण्णहा
        {
          cleanup_close पूर्णाङ्क nullfd = -1;
          cleanup_close पूर्णाङ्क mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          जइ (UNLIKELY (nullfd < 0))
            प्रत्यागम crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          जइ (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, रिक्त);

          जइ (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, रिक्त, MS_BIND | MS_RDONLY, रिक्त,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  ध्रुव वर्ण *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  जइ (context_type)
    {
      जइ (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          प्रत्यागम रिक्त;
        }
      प्रत्यागम context_type;
    }

  प्रत्यागम "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
स्थिर पूर्णाङ्क
diagnose_mount_failure (पूर्णाङ्क ret, libcrun_error_t *err, libcrun_container_t *container,
                        ध्रुव वर्ण *fstype)
{
  जइ (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      जइ (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          प्रत्यागम ret;
        }
    }
  प्रत्यागम ret;
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

  जइ (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    प्रत्यागम 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (ret == 0)
    प्रत्यागम 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  जइ (sys_old_मूलम्_fd >= 0)
    mountfd = get_bind_mount (sys_old_मूलम्_fd, "sys", true, true, false, MS_PRIVATE, err);
  जइ (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  जइ (cgroup_mount == रिक्त)
    {
      जइ (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, रिक्त);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      अण्णहा
        {
          crun_error_release (err);
          जइ (sys_old_मूलम्_fd >= 0)
            {
              cleanup_free वर्ण *sys_path = रिक्त;
              proc_fd_path_t sys_मूलम्_path;

              get_proc_self_fd_path (sys_मूलम्_path, sys_old_मूलम्_fd);
              xasprintf (&sys_path, "%s/sys", sys_मूलम्_path);
              ret = mount (sys_path, real_target, रिक्त, MS_BIND | MS_REC, रिक्त);
            }
          अण्णहा
            ret = mount ("/sys", real_target, रिक्त, MS_BIND | MS_REC, रिक्त);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      जइ (UNLIKELY (sysfd < 0))
        प्रत्यागम sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        रिक्त, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      प्रत्यागम do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  जइ (UNLIKELY (mountfd < 0))
    प्रत्यागम mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, रिक्त);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  जइ (UNLIKELY (sysfd < 0))
    प्रत्यागम sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    रिक्त, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
स्थिर शून्य
apply_selinux_xattr (libcrun_container_t *container, पूर्णाङ्क fd, पूर्णाङ्क extra_flags, ध्रुव वर्ण *label)
{
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क procfd;

  जइ ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    प्रत्यागम;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  जइ (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close पूर्णाङ्क xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      जइ (xfd >= 0)
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

  जइ (UNLIKELY (procfd < 0))
    प्रत्यागम procfd;

  {
    cleanup_close पूर्णाङ्क nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    जइ (UNLIKELY (nsfd < 0))
      प्रत्यागम crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    जइ (UNLIKELY (ret < 0))
      प्रत्यागम crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (*fd < 0))
    प्रत्यागम crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  जइ (UNLIKELY (tmp < 0))
    प्रत्यागम crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  प्रत्यागम 0;
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

  जइ (targetfd >= 0)
    {
      libcrun_error_t tmp_err = रिक्त;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      जइ (LIKELY (ret == 0))
        propagation_done = true;
      अण्णहा
        crun_error_release (&tmp_err);
    }

  जइ (! propagation_done)
    {
      ret = mount (रिक्त, real_target, रिक्त, mountflags & ALL_PROPAGATIONS, रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  प्रत्यागम 0;
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

  जइ ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    प्रत्यागम do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  जइ (*fd < 0)
    {
      *fd = dup (targetfd);
      जइ (UNLIKELY (*fd < 0))
        प्रत्यागम crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  प्रत्यागम 0;
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

  जइ (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  अण्णहा
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  जइ (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  जइ ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      जइ (UNLIKELY (context_type == रिक्त))
        प्रत्यागम -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      जइ (ret < 0)
        प्रत्यागम ret;
      data = data_with_label;
    }

  जइ (mountflags & MS_MOVE)
    {
      जइ ((mountflags & MS_BIND) || fstype)
        प्रत्यागम crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, रिक्त, MS_MOVE, रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      जइ (UNLIKELY (ms_move_fd < 0))
        प्रत्यागम ms_move_fd;
      targetfd = ms_move_fd;
    }

  जइ ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      जइ (targetfd >= 0)
        {
          जइ (mountflags & MS_BIND)
            {
              cleanup_close पूर्णाङ्क bindfd = -1;
              libcrun_error_t tmp_err = रिक्त;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              जइ (bindfd < 0)
                {
                  पूर्णाङ्क old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));

                  जइ (old_मूलम्_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_मूलम्_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              जइ (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, रिक्त);
              जइ (ret < 0)
                crun_error_release (&tmp_err);
            }
          अण्णहा
            {
              cleanup_close पूर्णाङ्क newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              जइ (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, रिक्त);
            }
        }

      जइ (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          जइ (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              पूर्णाङ्क old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));

              जइ (old_मूलम्_fd >= 0)
                {
                  cleanup_free वर्ण *proc_source_buf = रिक्त;
                  proc_fd_path_t old_मूलम्_path;

                  get_proc_self_fd_path (old_मूलम्_path, old_मूलम्_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_मूलम्_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      जइ (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          जइ (ret == 0)
            प्रत्यागम 0;
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          प्रत्यागम diagnose_mount_failure (ret, err, container, fstype);
        }

      जइ (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          जइ (UNLIKELY (fd < 0))
            प्रत्यागम fd;

          /* We are replacing the rootfs, reopen it.  */
          जइ (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              जइ (UNLIKELY (ret < 0))
                प्रत्यागम ret;
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

  जइ (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  जइ (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  जइ (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम ret;
}

स्थिर शून्य
try_umount (पूर्णाङ्क targetfd, ध्रुव वर्ण *target)
{
  proc_fd_path_t target_buffer;
  ध्रुव वर्ण *real_target = target;

  जइ (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

स्थिर bool
container_has_cgroupns (libcrun_container_t *container)
{
  प्रत्यागम get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

स्थिर पूर्णाङ्क
do_mount_cgroup_v2 (libcrun_container_t *container, पूर्णाङ्क targetfd, ध्रुव वर्ण *target,
                    unsigned long mountflags, ध्रुव वर्ण *data, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  पूर्णाङ्क cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जइ (UNLIKELY (cgroup_mode < 0))
    प्रत्यागम cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  जइ (UNLIKELY (ret < 0))
    {
      पूर्णाङ्क saved_errno = crun_error_get_errno (err);
      जइ (saved_errno == EPERM || saved_errno == EBUSY)
        {
          ध्रुव वर्ण *unified_cgroup_path;
          ध्रुव वर्ण *src_cgroup;

          crun_error_release (err);

          जइ (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              जइ (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  जइ (LIKELY (ret == 0))
                    प्रत्यागम ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_मूलम्;
          प्रत्यागम do_mount (container, src_cgroup, targetfd, target, रिक्त,
                           MS_BIND | mountflags, रिक्त, LABEL_NONE, err);
        }

      प्रत्यागम ret;
    }

  प्रत्यागम 0;
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
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  जइ (UNLIKELY (tmpfsdirfd < 0))
    प्रत्यागम tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, रिक्त);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  जइ (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "openat `%s`", subsystem_path);

  प्रत्यागम do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
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
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  जइ (UNLIKELY (tmpfsdirfd < 0))
    प्रत्यागम tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
    जइ (UNLIKELY (procfd < 0))
      प्रत्यागम procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, रिक्त, err);
  }
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (UNLIKELY (content == रिक्त || content[0] == '\0'))
    प्रत्यागम crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  प्रति (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (रिक्त, "\n", &saveptr))
    {
      cleanup_free वर्ण *source_path = रिक्त;
      cleanup_free वर्ण *source_subsystem = रिक्त;
      cleanup_free वर्ण *subsystem_path = रिक्त;
      वर्ण *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close पूर्णाङ्क subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      जइ (subsystem[0] == '\0')
        अनुवर्त;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      जइ (it)
        subsystem = it + 5;

      जइ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      जइ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_मूलम्, subsystem, रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      /* if there is already a mount specified, do not add a default one.  */
      जइ (find_mount_for (container, source_subsystem) != रिक्त)
        अनुवर्त;

      ret = append_paths (&source_path, err, source_subsystem, subpath, रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      जइ (UNLIKELY (subsystemfd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s`", subsystem_path);

      जइ (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          जइ (UNLIKELY (ret < 0))
            {
              जइ (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  अनुवर्त;
                }
              प्रत्यागम ret;
            }
        }
      अण्णहा
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, रिक्त, MS_BIND | mountflags, रिक्त,
                          LABEL_NONE, err);
          जइ (UNLIKELY (ret < 0))
            {
              जइ (crun_error_get_errno (err) != ENOENT)
                प्रत्यागम ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, रिक्त, MS_BIND | mountflags,
                              रिक्त, LABEL_NONE, err);
              जइ (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  जइ (crun_error_get_errno (err) != ENOENT)
                    प्रत्यागम ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
do_mount_cgroup (libcrun_container_t *container, ध्रुव वर्ण *source, पूर्णाङ्क targetfd, ध्रुव वर्ण *target,
                 unsigned long mountflags, ध्रुव वर्ण *data, libcrun_error_t *err)
{
  पूर्णाङ्क cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जइ (UNLIKELY (cgroup_mode < 0))
    प्रत्यागम cgroup_mode;

  विकल्प (cgroup_mode)
    {
    पक्ष CGROUP_MODE_UNIFIED:
      प्रत्यागम do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    पक्ष CGROUP_MODE_LEGACY:
    पक्ष CGROUP_MODE_HYBRID:
      प्रत्यागम do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  प्रत्यागम crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
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

  जइ (private_data->needed_devs_fds)
    {
      संरचना libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      प्रति (i = 0; needed_devs[i].path; i++)
        {
          जइ (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            अनुवर्त;

          पूर्णाङ्क fd = syscall_open_वृक्षः (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          जइ (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          अण्णहा जइ (fd >= 0)
            close (fd);
        }
    }

  जइ (private_data->notify_socket_वृक्षः_fd < 0
      && private_data->host_notify_socket_path)
    {
      पूर्णाङ्क fd = syscall_open_वृक्षः (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      जइ (fd >= 0)
        private_data->notify_socket_वृक्षः_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
स्थिर ध्रुव वर्ण *
relative_path_under_dev (ध्रुव वर्ण *path)
{
  जइ (path[0] != '/')
    प्रत्यागम path;
  जइ (strncmp (path, "/dev/", 5) == 0)
    {
      जइ (strchr (path + 5, '/'))
        प्रत्यागम रिक्त;
      प्रत्यागम path + 5;
    }
  प्रत्यागम रिक्त;
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
  जइ (UNLIKELY (ret < 0))
    {
      जइ (errno != EEXIST)
        प्रत्यागम crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  जइ (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  जइ (UNLIKELY ((st.st_mode & S_IFMT) != type))
    प्रत्यागम crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  जइ (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    प्रत्यागम crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  जइ (exists)
    प्रत्यागम 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  जइ (UNLIKELY (procfd < 0))
    प्रत्यागम procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fchown `%s`", device->path);

  प्रत्यागम 1;
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
  जइ (is_empty_string (fullname))
    प्रत्यागम crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free वर्ण *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  जइ (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  ध्रुव वर्ण *rel_dev = relative_path_under_dev (normalized_path);

  जइ (binds)
    {
      cleanup_close पूर्णाङ्क fd = -1;

      जइ (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          जइ (UNLIKELY (fd < 0))
            {
              जइ (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              जइ (UNLIKELY (fd < 0))
                प्रत्यागम crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      अण्णहा
        {
          ध्रुव वर्ण *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          जइ (UNLIKELY (fd < 0))
            प्रत्यागम fd;
        }

      जइ (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          जइ (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              जइ (LIKELY (ret >= 0))
                प्रत्यागम 0;
            }
          अण्णहा
            crun_error_release (err);
        }

      {
        cleanup_close पूर्णाङ्क mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        जइ (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            जइ (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, रिक्त);
                जइ (LIKELY (ret == 0))
                  प्रत्यागम 0;
              }
            अण्णहा
              crun_error_release (err);
          }
        अण्णहा
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, रिक्त, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, रिक्त, LABEL_MOUNT, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अण्णहा
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      जइ (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          जइ (ret <= 0)
            प्रत्यागम ret;
        }
      अण्णहा
        {
          cleanup_close पूर्णाङ्क dirfd = -1;
          cleanup_free वर्ण *dirname = रिक्त;
          वर्ण *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          जइ (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          जइ (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              जइ (UNLIKELY (dirfd < 0))
                प्रत्यागम crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          अण्णहा
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              जइ (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              जइ (UNLIKELY (dirfd < 0))
                प्रत्यागम dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          जइ (ret <= 0)
            प्रत्यागम ret;
        }
    }
  प्रत्यागम 0;
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

  जइ (! def || ! def->linux)
    प्रत्यागम 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  जइ (UNLIKELY (devfd < 0))
    प्रत्यागम devfd;

  प्रति (i = 0; i < def->linux->devices_len; i++)
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

      जइ (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रति (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close पूर्णाङ्क srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      जइ (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  प्रति (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      जइ (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क saved_errno = errno;

          जइ (errno == EEXIST && ! symlinks[i].force)
            अनुवर्त;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          जइ (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              जइ (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              जइ (ret < 0 && errno == EBUSY)
                {
                  cleanup_close पूर्णाङ्क tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  जइ (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      जइ (umount2 (procpath, MNT_DETACH) == 0)
                        गच्छ retry_unlink;
                    }
                }
              जइ (ret == 0)
                गच्छ retry_symlink;
            }
          प्रत्यागम crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  जइ (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
स्थिर ध्रुव वर्ण *
next_path_component (ध्रुव वर्ण *p, size_t *len)
{
  प्रति (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      जइ (*len != 1 || p[0] != '.')
        प्रत्यागम p;
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

  प्रति (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      जइ (la != lb || memcmp (a, b, la) != 0)
        प्रत्यागम false;
      जइ (la == 0)
        प्रत्यागम true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
स्थिर bool
is_duplicate_path (वर्ण **paths, size_t i)
{
  size_t j;

  जइ (paths[i] == रिक्त)
    प्रत्यागम false;

  प्रति (j = 0; j < i; j++)
    जइ (paths[j] && same_path (paths[j], paths[i]))
      प्रत्यागम true;

  प्रत्यागम false;
}

स्थिर पूर्णाङ्क
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  पूर्णाङ्क ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  जइ (! def || ! def->linux)
    प्रत्यागम 0;

  प्रति (i = 0; i < def->linux->masked_paths_len; i++)
    {
      जइ (is_duplicate_path (def->linux->masked_paths, i))
        अनुवर्त;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रति (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
do_pivot (libcrun_container_t *container, ध्रुव वर्ण *rootfs, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क oldrootfd = -1;
  cleanup_close पूर्णाङ्क newrootfd = -1;
  पूर्णाङ्क ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (oldrootfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (newrootfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_मूलम् (".", ".");
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, रिक्त, -1, ".", रिक्त, MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = umount2 (".", MNT_DETACH);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "umount2 oldroot");

  करेहि
    {
      ret = umount2 (".", MNT_DETACH);
      जइ (ret < 0 && errno == EINVAL)
        भङ्ग;
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "umount2 oldroot");
  } जाव (ret == 0);

  ret = chdir ("/");
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "chdir to newroot");

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, वर्ण **data, libcrun_error_t *err)
{
  ध्रुव वर्ण *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close पूर्णाङ्क fd = -1;
  संरचना stat st;
  पूर्णाङ्क ret;

  जइ (*data != रिक्त && strstr (*data, "mode="))
    प्रत्यागम 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  जइ (fd < 0)
    {
      जइ (crun_error_get_errno (err) != ENOENT)
        प्रत्यागम fd;

      crun_error_release (err);
      प्रत्यागम 0;
    }
  ret = fstat (fd, &st);
  जइ (ret < 0)
    प्रत्यागम crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
get_default_flags (libcrun_container_t *container, ध्रुव वर्ण *destination, वर्ण **data)
{
  जइ (strcmp (destination, "/proc") == 0)
    प्रत्यागम 0;
  जइ (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      प्रत्यागम MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  जइ (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      प्रत्यागम MS_NOEXEC | MS_STRICTATIME;
    }
  जइ (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      प्रत्यागम MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  जइ (strcmp (destination, "/dev/mqueue") == 0)
    प्रत्यागम MS_NOEXEC | MS_NOSUID | MS_NODEV;
  जइ (strcmp (destination, "/dev/pts") == 0)
    {
      जइ (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      अण्णहा
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      प्रत्यागम MS_NOEXEC | MS_NOSUID;
    }
  जइ (strcmp (destination, "/sys") == 0)
    प्रत्यागम MS_NOEXEC | MS_NOSUID | MS_NODEV;

  प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  प्रत्यागम find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

स्थिर पूर्णाङ्क
check_valid_no_follow_file_system (पूर्णाङ्क fd, ध्रुव वर्ण *destination, libcrun_error_t *err)
{
  संरचना statfs sfs;

  पूर्णाङ्क ret = fstatfs (fd, &sfs);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "statfs `%s`", destination);

  विकल्प (sfs.f_type)
    {
    पक्ष PROC_SUPER_MAGIC:
    पक्ष SYSFS_MAGIC:
    पक्ष DEVPTS_SUPER_MAGIC:
      प्रत्यागम crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
safe_create_symlink (पूर्णाङ्क rootfsfd, ध्रुव वर्ण *rootfs, ध्रुव वर्ण *target, ध्रुव वर्ण *destination, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क parent_dir_fd = -1;
  cleanup_free वर्ण *buffer = रिक्त;
  वर्ण *part;
  पूर्णाङ्क ret;

  जइ (is_empty_string (destination))
    प्रत्यागम crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  जइ (UNLIKELY (parent_dir_fd < 0))
    प्रत्यागम parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  जइ (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      जइ (errno == EEXIST)
        {
          cleanup_free वर्ण *link = रिक्त;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          जइ (UNLIKELY (len < 0))
            प्रत्यागम len;

          जइ ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            प्रत्यागम 0;

          प्रत्यागम crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      प्रत्यागम crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
handle_copy_symlink (libcrun_container_t *container, ध्रुव वर्ण *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     ध्रुव वर्ण *link_target, libcrun_error_t *err)
{
  cleanup_free वर्ण *target = रिक्त;

  जइ (link_target == रिक्त)
    {
      पूर्णाङ्क old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));
      ध्रुव वर्ण *source = mount->source;
      ssize_t len;

      जइ (old_मूलम्_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_मूलम्_fd, source + 1, &target, 0, err);
      अण्णहा
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      जइ (UNLIKELY (len < 0))
        प्रत्यागम len;
      link_target = target;
    }

  प्रत्यागम safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
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
  जइ (UNLIKELY (ret < 0))
    {
      जइ (errno == ENOENT)
        {
          जइ (strchr (target, '/'))
            प्रत्यागम crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      अण्णहा जइ (errno == ENOTDIR)
        प्रत्यागम crun_make_error (err, errno, "the target `/%s` is invalid", target);

      जइ (ret < 0)
        प्रत्यागम crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
handle_tmpcopyup (libcrun_container_t *container, ध्रुव वर्ण *rootfs, ध्रुव वर्ण *target,
                  पूर्णाङ्क copy_from_fd, libcrun_error_t *err)
{
  पूर्णाङ्क destfd, ret;
  cleanup_close पूर्णाङ्क tmpfd = copy_from_fd;

  target = consume_slashes (target);

  जइ (is_empty_string (target))
    प्रत्यागम crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  जइ (UNLIKELY (destfd < 0))
    प्रत्यागम crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
get_mount_label_how (ध्रुव वर्ण *type, bool is_sysfs_or_proc)
{
  जइ (is_sysfs_or_proc)
    प्रत्यागम LABEL_NONE;

  जइ (strcmp (type, "mqueue") == 0)
    प्रत्यागम LABEL_XATTR;

  प्रत्यागम LABEL_MOUNT;
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

  जइ (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  अण्णहा
    ret = statfs (source, &sfs);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम 0;

  प्रति (i = 0; i < mount->options_len; i++)
    {
      ध्रुव संरचना propagation_flags_s *prop;

      जइ (mount->options[i] == रिक्त)
        अनुवर्त;

      prop = libcrun_str2mount_flags (mount->options[i]);
      जइ (prop == रिक्त || (prop->extra_flags & OPTION_RECURSIVE))
        अनुवर्त;

      जइ (prop->clear)
        cleared |= prop->flags;
      अण्णहा
        cleared &= ~prop->flags;
    }

  प्रत्यागम sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
स्थिर ध्रुव वर्ण *
get_path_in_rootfs (ध्रुव वर्ण *rootfs, ध्रुव वर्ण *path)
{
  size_t len;

  जइ (rootfs == रिक्त || path == रिक्त)
    प्रत्यागम रिक्त;

  len = strlen (rootfs);
  जाव (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  जइ (len == 0)
    प्रत्यागम रिक्त;

  जइ (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    प्रत्यागम रिक्त;

  प्रत्यागम consume_slashes (path + len);
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

  जइ (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  जइ (mount->options == रिक्त)
    flags = get_default_flags (container, mount->destination, &data);
  अण्णहा
    {
      size_t j;

      प्रति (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  जइ (type == रिक्त && (flags & MS_BIND) == 0)
    प्रत्यागम crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  जइ ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      ध्रुव वर्ण *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      जइ (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          जइ (UNLIKELY (source_mountfd < 0))
            प्रत्यागम crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  जइ ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  जइ (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      जइ (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      जइ (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  जइ (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      ध्रुव वर्ण *path = mount->source;

      जइ ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        प्रत्यागम crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      जइ (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, रिक्त);
        }
      अण्णहा जइ (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      अण्णहा जइ (path[0] == '/')
        {
          पूर्णाङ्क old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          जइ (old_मूलम्_fd >= 0)
            ret = get_file_type_at (old_मूलम्_fd, &src_mode, nofollow, (path[1] == '\0') ? रिक्त : path + 1);
          अण्णहा
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      अण्णहा
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "cannot stat `%s`", path);

      जइ (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          पूर्णाङ्क bind_dirfd = AT_FDCWD;
          ध्रुव वर्ण *bind_source = mount->source;
          पूर्णाङ्क old_मूलम्_fd = get_old_मूलम्_fd (get_private_data (container));

          जइ (old_मूलम्_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_मूलम्_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          source_mountfd = ret;
        }
    }

  जइ (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      ध्रुव वर्ण *cached_target = रिक्त;

      जइ (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      mounted = true;
    }
  अण्णहा जइ (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अण्णहा
    {
      bool is_dir = S_ISDIR (src_mode);

      जइ (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      अण्णहा
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          targetfd = ret;
        }
    }

  जइ (extra_flags & OPTION_TMPCOPYUP)
    {
      जइ (strcmp (type, "tmpfs") != 0)
        प्रत्यागम crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      जइ (UNLIKELY (copy_from_fd < 0))
        {
          जइ (errno != ENOTDIR)
            प्रत्यागम crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  जइ (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, रिक्त);
      जइ (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          जइ (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, रिक्त, source_mountfd, target, रिक्त, remaining_flags, data, LABEL_NONE | no_remount, err);
              जइ (UNLIKELY (ret < 0))
                प्रत्यागम ret;
            }

          mounted = true;

          जइ (is_empty_string (target))
            {
              पूर्णाङ्क new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              जइ (UNLIKELY (new_rootfsfd < 0))
                प्रत्यागम crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      अण्णहा जइ (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          प्रत्यागम crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      अण्णहा जइ (get_private_data (container)->host_मूलम्_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          प्रत्यागम crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      अण्णहा जइ (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  जइ (! mounted)
    {
      जइ (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      अण्णहा जइ (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      अण्णहा
        {
          पूर्णाङ्क extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }

  जइ (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (rec_clear || rec_set)
    {
      ध्रुव bool is_dir = S_ISDIR (src_mode);
      cleanup_close पूर्णाङ्क dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      जइ (UNLIKELY (dfd < 0))
        प्रत्यागम crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
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

  प्रति (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
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

  जइ ((get_private_data (container)->rootfsfd < 0) || (rootfs == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  जइ (mount_options == रिक्त)
    flags = get_default_flags (container, mount_destination, &data);
  अण्णहा
    {
      size_t j;

      प्रति (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  जइ (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  जइ (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      जइ (UNLIKELY (is_dir < 0))
        प्रत्यागम is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
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

  जइ (container && get_private_data (container)->host_notify_socket_path)
    {
      ध्रुव वर्ण *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  जइ (host_path == रिक्त)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_मूलम्, context->id, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  जइ (UNLIKELY (notify_fd < 0))
    प्रत्यागम notify_fd;

  जइ (UNLIKELY (chmod (host_path, 0777) < 0))
    प्रत्यागम crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  जइ (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (शून्य) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  प्रत्यागम 1;
#अण्णहा
  (शून्य) context;
  (शून्य) container;
  (शून्य) err;
  *notify_socket_out = -1;
  प्रत्यागम 0;
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

  जइ (notify_socket == रिक्त)
    प्रत्यागम 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_मूलम् : रिक्त),
                                     (container->context ? container->context->id : रिक्त), err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", रिक्त);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", रिक्त);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = mkdir (host_notify_socket_path, 0700);
  जइ (ret < 0)
    प्रत्यागम crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  जइ (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_मूलम्_in_the_userns (container->container_def, 0, 0, &container_मूलम्_uid, &container_मूलम्_gid);
      जइ (container_मूलम्_uid != ((uid_t) -1) && container_मूलम्_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_मूलम्_uid, container_मूलम्_gid);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_मूलम्_uid, container_मूलम्_gid);
        }
    }

  notify_socket_वृक्षः_fd = syscall_open_वृक्षः (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  जइ (notify_socket_वृक्षः_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_वृक्षः_fd = notify_socket_वृक्षः_fd;
  अण्णहा जइ (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  अण्णहा जइ (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  अण्णहा
    /* some other error */
    प्रत्यागम crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = रिक्त;
  प्रत्यागम 0;
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

  जइ (host_notify_socket_path == रिक्त || container_notify_socket_path == रिक्त)
    प्रत्यागम 0;

  जइ (rootfs == रिक्त)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  जइ (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  targetfd = ret;

  notify_socket_वृक्षः_fd = get_private_data (container)->notify_socket_वृक्षः_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_वृक्षः_fd = -1;

  जइ (notify_socket_वृक्षः_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_वृक्षः_fd, targetfd, रिक्त);
      जइ (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          जइ (UNLIKELY (targetfd < 0))
            प्रत्यागम targetfd;

          ret = do_mount (container, रिक्त, targetfd, container_notify_socket_path_dir, रिक्त,
                          MS_REC | MS_PRIVATE, रिक्त, LABEL_NONE, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      अण्णहा जइ (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      अण्णहा
        प्रत्यागम crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_वृक्षः_fd,
                                container_notify_socket_path_dir);
    }

  जइ (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, रिक्त,
                      MS_BIND | MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
make_parent_mount_private (ध्रुव वर्ण *rootfs, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क rootfsfd = -1;
  संरचना stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (rootfsfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", rootfs);

  जइ (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    प्रत्यागम crun_make_error (err, errno, "fstat `%s`", rootfs);

  प्रति (;;)
    {
      संरचना stat cur_st;
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क ret;
      cleanup_close पूर्णाङ्क parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      जइ (ret == 0)
        प्रत्यागम 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (रिक्त, proc_path, रिक्त, MS_PRIVATE, रिक्त);
      जइ (ret == 0)
        प्रत्यागम 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      जइ (parentfd < 0)
        {
          पूर्णाङ्क saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          जइ (ret != 0)
            प्रत्यागम crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          प्रत्यागम crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      जइ (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        प्रत्यागम crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      जइ (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        भङ्ग;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  प्रत्यागम crun_make_error (err, 0, "make `%s` private", rootfs);
}

पूर्णाङ्क
libcrun_set_mounts (संरचना container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ध्रुव वर्ण *rootfs, set_mounts_cb_t cb, शून्य *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क is_user_ns = 0;
  पूर्णाङ्क ret = 0;

  जइ (rootfs == रिक्त || def->mounts == रिक्त)
    प्रत्यागम 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  जइ (def->मूलम्->readonly)
    {
      संरचना remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      पूर्णाङ्क fd;

      fd = dup (get_private_data (container)->rootfsfd);
      जइ (UNLIKELY (fd < 0))
        प्रत्यागम crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, रिक्त, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  जइ (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      जइ (UNLIKELY (is_user_ns < 0))
        प्रत्यागम is_user_ns;
    }

  जइ (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  जइ (cb)
    {
      ret = cb (cb_data, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = रिक्त;
      ध्रुव वर्ण *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (शून्य) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
umount_or_hide (ध्रुव वर्ण *target, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = umount2 (target, MNT_DETACH);
  जइ (UNLIKELY (ret < 0))
    {
      पूर्णाङ्क saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      जइ (errno == EINVAL)
        {
          ret = mount (रिक्त, target, "tmpfs", 0, "size=0k");
          जइ (LIKELY (ret == 0))
            प्रत्यागम 0;
        }

      प्रत्यागम crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
move_मूलम् (ध्रुव वर्ण *rootfs, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = chdir (rootfs);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = umount_or_hide ("/proc", err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "chdir to `%s`", rootfs);

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_finalize_mounts (संरचना container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ध्रुव वर्ण *rootfs,
                         set_mounts_cb_t after_pivot_cb, शून्य *cb_data, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_मूलम्_fd));

  जइ (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      जइ (get_private_data (container)->no_pivot)
        {
          ret = move_मूलम् (rootfs, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      अण्णहा
        {
          ret = do_pivot (container, rootfs, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      ret = do_mount (container, रिक्त, -1, "/", रिक्त,
                      get_private_data (container)->rootfs_propagation,
                      रिक्त, LABEL_MOUNT, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = chdir ("/");
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "chdir to `/`");
    }

  जइ (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = finalize_mounts (container, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
maybe_open_वृक्षः_namespace (ध्रुव वर्ण *rootfs, पूर्णाङ्क *out_fd, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क rootfs_fd = -1;
  पूर्णाङ्क वृक्षः_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (rootfs_fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", rootfs);

  वृक्षः_fd = syscall_open_वृक्षः (rootfs_fd, "",
                               OPEN_वृक्षः_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  जइ (वृक्षः_fd < 0)
    {
      जइ (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        प्रत्यागम 0;
      प्रत्यागम crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = वृक्षः_fd;
  प्रत्यागम 0;
}

स्थिर संरचना libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  संरचना libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  जइ (mount_fds == रिक्त)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  प्रत्यागम mount_fds;
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

  जइ (mount->type == रिक्त)
    प्रत्यागम -1;

  fstype = mount->type;

  प्रति (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  जइ (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      पूर्णाङ्क extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      जइ (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          जइ (label_type == रिक्त)
            crun_error_release (err);
          अण्णहा
            label_val = def->linux->mount_label;
        }
    }

  जइ (strcmp (fstype, "cgroup") == 0)
    {
      पूर्णाङ्क cgroup_mode = libcrun_get_cgroup_mode (err);
      जइ (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  जइ (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, रिक्त, रिक्त, mnt_data);

  प्रत्यागम mnt_fd;
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
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_listmount, req, ids, nr, flags);
#अण्णहा
  (शून्य) req;
  (शून्य) ids;
  (शून्य) nr;
  (शून्य) flags;
  errno = ENOSYS;
  प्रत्यागम -1;
#endif
}

स्थिर पूर्णाङ्क
syscall_statmount (संरचना mnt_id_req_s *req, संरचना statmount_s *buf, size_t size, unsigned पूर्णाङ्क flags)
{
#ifdef __NR_statmount
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_statmount, req, buf, size, flags);
#अण्णहा
  (शून्य) req;
  (शून्य) buf;
  (शून्य) size;
  (शून्य) flags;
  errno = ENOSYS;
  प्रत्यागम -1;
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

  प्रति (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      जइ (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != रिक्त && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          भङ्ग;
        }
    }
  जइ (! has_absolute_bind)
    प्रत्यागम false;

  ids = xmalloc (capacity * परिमाण (uint64_t));
  प्रति (;;)
    {
      संरचना mnt_id_req_s req = {
        .size = परिमाण (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      पूर्णाङ्क ret;

      जइ (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * परिमाण (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      जइ (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        प्रत्यागम true;
      जइ (ret == 0)
        भङ्ग;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  प्रति (i = 0; i < n_ids; i++)
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

      जइ (syscall_statmount (&req, sm, परिमाण (buffer), 0) < 0)
        अनुवर्त;
      जइ ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        अनुवर्त;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      मूलम् = sm->str + sm->mnt_root;
      जइ (! has_prefix (मूलम्, "mnt:["))
        अनुवर्त;

      point = sm->str + sm->mnt_point;

      प्रति (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          ध्रुव वर्ण *source = def->mounts[j]->source;
          size_t source_len;

          जइ (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            अनुवर्त;
          जइ (source == रिक्त || source[0] != '/')
            अनुवर्त;

          source_len = strlen (source);
          जाव (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          जइ (strncmp (point, source, source_len) != 0)
            अनुवर्त;

          /* The source itself is a pinned mount namespace.  */
          जइ (point[source_len] == '\0')
            प्रत्यागम true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          जइ (recursive && (source_len == 1 || point[source_len] == '/'))
            प्रत्यागम true;
        }
    }

  प्रत्यागम false;
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

  जइ (has_hooks || has_userns)
    प्रत्यागम false;

  जइ (mounts_hit_pinned_mount_namespace (def))
    प्रत्यागम false;

  जइ (container->context->no_open_वृक्षः_namespace)
    प्रत्यागम false;

  {
    libcrun_error_t tmp_err = रिक्त;
    पूर्णाङ्क in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    जइ (in_userns != 0)
      प्रत्यागम false;
  }

  mount_fds = get_fd_map (container);
  प्रति (i = 0; i < def->mounts_len; i++)
    {
      जइ (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          जइ (! has_cached_target)
            प्रत्यागम false;
        }
    }

  प्रत्यागम true;
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

  जइ (mount->type == रिक्त || strcmp (mount->type, "tmpfs") != 0)
    प्रत्यागम false;

  प्रति (i = 0; i < mount->options_len; i++)
    जइ (mount->options[i] != रिक्त && has_prefix (mount->options[i], "mode="))
      प्रत्यागम false;

  प्रत्यागम true;
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

  जइ (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, रिक्त, रिक्त, रिक्त, रिक्त);

  जइ ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  मुक्त (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  प्रति (i = 0; i < def->mounts_len; i++)
    {
      संरचना libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close पूर्णाङ्क mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      जइ (mount_fds && mount_fds->fds[i] >= 0)
        अनुवर्त;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      जइ (def->mounts[i]->source != रिक्त
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free वर्ण *mnt_data = रिक्त;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          प्रति (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          जइ (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free वर्ण *target = रिक्त;
              libcrun_error_t tmp_err = रिक्त;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              जइ (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              जइ (get_private_data (container)->copy_symlink_targets == रिक्त)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * परिमाण (वर्ण *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = रिक्त;
            }
        }

      जइ (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          जइ (mount_fds && def->mounts[i]->source != रिक्त && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == रिक्त)
            {
              libcrun_error_t tmp_err = रिक्त;
              unsigned long propagation = MS_PRIVATE;

              जइ (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              जइ (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          अनुवर्त;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      जइ (mnt_fd < 0)
        {
          crun_error_release (err);
          अनुवर्त;
        }

      जइ (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close पूर्णाङ्क destfd = -1;
          संरचना stat st;

          जइ (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              जइ (UNLIKELY (rootfsfd < 0))
                प्रत्यागम crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          जइ (destfd < 0)
            crun_error_release (&tmp_err);
          अण्णहा जइ (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
              जइ (UNLIKELY (procfd < 0))
                प्रत्यागम procfd;

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
  जइ (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  जइ (! get_private_data (container)->joined_mount_ns
      && can_use_open_वृक्षः_namespace (container))
    use_open_वृक्षः = true;

  जइ (use_open_वृक्षः)
    {
      ret = maybe_open_वृक्षः_namespace (*rootfs, &वृक्षः_fd, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  जइ (वृक्षः_fd >= 0)
    preopen_needed_devs_and_notify (container);

  जइ (वृक्षः_fd >= 0)
    {
      ret = setns (वृक्षः_fd, CLONE_NEWNS);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (रिक्त, "/", रिक्त, MS_REMOUNT | MS_BIND, रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, रिक्त, -1, "/", रिक्त, MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = do_mount (container, रिक्त, -1, "/", रिक्त, rootfs_propagation, रिक्त, LABEL_MOUNT, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_मूलम्_switched = true;
      मुक्त (*rootfs);
      *rootfs = xstrdup ("/");
    }
  अण्णहा
    {
      जइ (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, रिक्त, -1, "/", रिक्त, rootfs_propagation, रिक्त, LABEL_MOUNT, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = make_parent_mount_private (*rootfs, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, रिक्त, MS_BIND | MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  जइ (! container->context->force_no_cgroup)
    {
      पूर्णाङ्क cgroup_mode = libcrun_get_cgroup_mode (err);
      जइ (UNLIKELY (cgroup_mode < 0))
        प्रत्यागम cgroup_mode;

      जइ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          वर्ण *unified_cgroup_path = रिक्त;
          पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
          जइ (UNLIKELY (procfd < 0))
            प्रत्यागम procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  जइ (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = रिक्त;
      प्रत्यागम ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = रिक्त;

  जइ (वृक्षः_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  जइ (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_do_pivot_मूलम् (libcrun_container_t *container, bool no_pivot, वर्ण **rootfs, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  जइ (*rootfs == रिक्त)
    प्रत्यागम 0;

  जइ (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      जइ (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "chdir to `/`");
        }
    }
  अण्णहा
    {
      ret = chroot (*rootfs);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "chdir to `/`");

      मुक्त (*rootfs);
      *rootfs = xstrdup ("/");
    }

  प्रत्यागम 0;
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
  जइ (UNLIKELY (fd == -1))
    प्रत्यागम crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रति (i = 0; i <= 2; i++)
    {
      जइ (UNLIKELY (fstat (i, &statbuf) == -1))
        प्रत्यागम crun_make_error (err, errno, "stat fd `%d`", i);
      जइ (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          जइ (UNLIKELY (dup2 (fd, i) == -1))
            प्रत्यागम crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  प्रत्यागम 0;
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
  जइ (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (pid_fmt)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  जाव (nargs < MAX_ARGS)
    {
      वर्ण *p = strsep (&next, " \n");
      जइ (next == रिक्त)
        भङ्ग;
      args[nargs++] = p;
    }
  args[nargs++] = रिक्त;

  ret = run_process (args, err);
  जइ (ret < 0)
    प्रत्यागम ret;
  जइ (ret > 0)
    प्रत्यागम crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
newgidmap (pid_t pid, ध्रुव वर्ण *map_file, libcrun_error_t *err)
{
  प्रत्यागम uidgidmap_helper ("newgidmap", pid, map_file, err);
}

स्थिर पूर्णाङ्क
newuidmap (pid_t pid, ध्रुव वर्ण *map_file, libcrun_error_t *err)
{
  प्रत्यागम uidgidmap_helper ("newuidmap", pid, map_file, err);
}

स्थिर पूर्णाङ्क
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  जइ (UNLIKELY (fd < 0))
    प्रत्यागम fd;

  ret = TEMP_FAILURE_RETRY (लिख (fd, "deny", 4));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *content = रिक्त;

  जइ (get_private_data (container)->deny_setgroups)
    प्रत्यागम 0;

  जइ (container->container_def->annotations)
    {
      ध्रुव वर्ण *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      जइ (annotation)
        प्रत्यागम strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close पूर्णाङ्क fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    जइ (fd < 0)
      ret = fd;
    अण्णहा
      ret = read_all_fd (fd, "self/setgroups", &content, रिक्त, err);
  }
  जइ (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      जइ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          प्रत्यागम 1;
        }
      प्रत्यागम ret;
    }

  प्रत्यागम strncmp (content, "deny", 4) == 0 ? 0 : 1;
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

  जइ (process != रिक्त && process->user != रिक्त)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  जइ (UNLIKELY (can_do_setgroups < 0))
    प्रत्यागम can_do_setgroups;

  जइ (can_do_setgroups == 0)
    {
      ध्रुव वर्ण *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      जइ (annotation && strcmp (annotation, "0") != 0)
        प्रत्यागम 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      प्रति (i = 0; i < additional_gids_len; i++)
        जइ (additional_gids[i] != process->user->gid)
          प्रत्यागम crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (पूर्णाङ्क) additional_gids[i]);
      प्रत्यागम 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "setgroups");

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  जइ (get_private_data (container)->unshare_cgroupns)
    {
      पूर्णाङ्क ret = unshare (CLONE_NEWCGROUP);
      जइ (UNLIKELY (ret < 0))
        {
          जइ (errno != EINVAL)
            प्रत्यागम crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  प्रत्यागम 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
पूर्णाङ्क
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  जइ (len != 1)
    प्रत्यागम 0;

  जइ (mappings[0]->size != 1)
    प्रत्यागम 0;

  जइ (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    प्रत्यागम 0;

  प्रत्यागम 1;
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

  जइ (container->host_uid)
    ret = idmap_helper (pid, map, err);
  जइ (container->host_uid == 0 || ret < 0)
    {
      cleanup_close पूर्णाङ्क fd = -1;

      जइ (ret < 0)
        {
          जइ (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      जइ (UNLIKELY (fd < 0))
        प्रत्यागम fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      जइ (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free वर्ण *single_mapping = रिक्त;
          crun_error_release (err);

          जइ (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              जइ (UNLIKELY (ret < 0))
                प्रत्यागम ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          जइ (UNLIKELY (fd < 0))
            प्रत्यागम fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free वर्ण *uid_map = रिक्त;
  cleanup_free वर्ण *gid_map = रिक्त;
  size_t uid_map_len = 0, gid_map_len = 0;
  पूर्णाङ्क ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  जइ ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    प्रत्यागम 0;

  जइ (! def || ! def->linux)
    प्रत्यागम 0;

  जइ (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अण्णहा
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      uid_map_len = (size_t) ret;
      जइ (uid_map == रिक्त)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }

  जइ (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अण्णहा
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      gid_map_len = (size_t) ret;
      जइ (gid_map == रिक्त)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
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
  जइ (cap < 32)
    प्रत्यागम CAP_TO_MASK_0 (cap) & caps[0];
  प्रत्यागम (CAP_TO_MASK_1 (cap) & caps[1]);
}

स्थिर unsigned long cap_last_cap;

पूर्णाङ्क
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;
  वर्ण buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  जइ (fd < 0)
    प्रत्यागम fd;
  ret = TEMP_FAILURE_RETRY (पठ (fd, buffer, परिमाण (buffer) - 1));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, रिक्त, 10);
  जइ (errno != 0)
    प्रत्यागम crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
set_required_caps (संरचना all_caps_s *caps, uid_t uid, gid_t gid, पूर्णाङ्क no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  पूर्णाङ्क ret;
  संरचना __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  संरचना __user_cap_data_struct data[2] = { { 0 } };

  जइ (cap_last_cap == 0)
    प्रत्यागम crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  प्रति (cap = 0; cap <= cap_last_cap; cap++)
    जइ (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        जइ (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          प्रत्यागम crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  जइ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    प्रत्यागम crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  जइ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    प्रत्यागम crun_make_error (err, errno, "prctl reset ambient");

  प्रति (cap = 0; cap <= cap_last_cap; cap++)
    जइ (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        जइ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          प्रत्यागम crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  जइ (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "prctl reset KEEPCAPS");

      जइ (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          प्रति (cap = 0; cap <= cap_last_cap; cap++)
            जइ (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "capset");
        }
    }
#endif

  जइ (no_new_privs)
    जइ (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      प्रत्यागम crun_make_error (err, errno, "prctl set no new privs");

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
read_caps (unsigned long caps[2], वर्ण **मानानि, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  प्रति (i = 0; i < len; i++)
    {
      cap_मानम्_t cap;
      जइ (cap_from_name (मानानि[i], &cap) < 0)
        प्रत्यागम crun_make_error (err, 0, "unknown capability `%s`", मानानि[i]);

      जइ (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      अण्णहा
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#अण्णहा
  caps[0] = 0;
  caps[1] = 0;
#endif
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  जइ (proc->selinux_label)
    प्रत्यागम set_selinux_label (container, proc->selinux_label, now, err);

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  जइ (proc->apparmor_profile)
    प्रत्यागम set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  पूर्णाङ्क no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  संरचना all_caps_s caps = {};
  पूर्णाङ्क ret;

  जइ (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
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
  प्रति (it = rlimits; it->name; it++)
    जइ (strcmp (it->name, name) == 0)
      प्रत्यागम it->मानम्;
  प्रत्यागम -1;
}

पूर्णाङ्क
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  प्रति (i = 0; i < len; i++)
    {
      प्रति (j = i + 1; j < len; j++)
        {
          जइ (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            प्रत्यागम crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  प्रति (i = 0; i < len; i++)
    {
      संरचना rlimit limit;
      वर्ण *type = new_rlimits[i]->type;
      पूर्णाङ्क resource = get_rlimit_resource (type);
      जइ (UNLIKELY (resource < 0))
        प्रत्यागम crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      जइ (UNLIKELY (setrlimit (resource, &limit) < 0))
        प्रत्यागम crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  पूर्णाङ्क ret;
  जइ (def->hostname == रिक्त || def->hostname[0] == '\0')
    प्रत्यागम 0;
  जइ (! has_uts)
    प्रत्यागम crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "sethostname");
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  पूर्णाङ्क ret;
  जइ (is_empty_string (def->domainname))
    प्रत्यागम 0;
  जइ (! has_uts)
    प्रत्यागम crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "setdomainname");
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;
  वर्ण oom_buffer[16];
  जइ (def->process == रिक्त || ! def->process->oom_score_adj_present)
    प्रत्यागम 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, परिमाण (oom_buffer), "%i", def->process->oom_score_adj);
  जइ (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (oom_buffer)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  जइ (fd < 0)
    प्रत्यागम fd;
  ret = TEMP_FAILURE_RETRY (लिख (fd, oom_buffer, strlen (oom_buffer)));
  जइ (ret < 0)
    प्रत्यागम crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  प्रत्यागम 0;
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
  जइ (name[0] == '/' || path_has_dot_dot_component (name))
    प्रत्यागम crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_कुञ्चिका);

  जइ (has_prefix (name, "fs/mqueue/"))
    {
      जइ (namespaces_created & CLONE_NEWIPC)
        प्रत्यागम 0;

      namespace = "IPC";
      गच्छ fail;
    }

  जइ (has_prefix (name, "kernel/"))
    {
      size_t i;

      प्रति (i = 0; sysctlRequiringIPC[i]; i++)
        जइ (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            जइ (namespaces_created & CLONE_NEWIPC)
              प्रत्यागम 0;

            namespace = "IPC";
            गच्छ fail;
          }

      जइ (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          जइ (! is_empty_string (def->domainname) && (strcmp (original_मानम्, def->domainname) != 0))
            प्रत्यागम crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_कुञ्चिका);

          जइ (namespaces_created & CLONE_NEWUTS)
            प्रत्यागम 0;

          namespace = "UTS";
          गच्छ fail;
        }

      जइ (strcmp (name, "kernel/hostname") == 0)
        प्रत्यागम crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_कुञ्चिका);
    }
  जइ (has_prefix (name, "net/"))
    {
      जइ (namespaces_created & CLONE_NEWNET)
        प्रत्यागम 0;

      namespace = "network";
      गच्छ fail;
    }

  प्रत्यागम crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_कुञ्चिका);

fail:
  प्रत्यागम crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_कुञ्चिका, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
स्थिर वर्ण *
sysctl_error_reason (ध्रुव वर्ण *name, पूर्णाङ्क namespaces_created, पूर्णाङ्क errno_)
{

  जइ (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    प्रत्यागम xstrdup ("are all the IDs mapped in the user namespace?");

  प्रत्यागम रिक्त;
}

पूर्णाङ्क
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close पूर्णाङ्क dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  जइ (def->linux == रिक्त || def->linux->sysctl == रिक्त || def->linux->sysctl->len == 0)
    प्रत्यागम 0;

  प्रति (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णाङ्क मानम्;

      मानम् = libcrun_find_namespace (def->linux->namespaces[i]->type);
      जइ (UNLIKELY (मानम् < 0))
        प्रत्यागम crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= मानम्;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  जइ (UNLIKELY (dirfd < 0))
    प्रत्यागम dirfd;

  प्रति (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free वर्ण *name = रिक्त;
      cleanup_close पूर्णाङ्क fd = -1;
      पूर्णाङ्क ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      जइ (def->linux->sysctl->कुञ्चिकाः[i] == रिक्त || def->linux->sysctl->मानानि[i] == रिक्त)
        प्रत्यागम crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_कुञ्चिका_to_proc_path (def->linux->sysctl->कुञ्चिकाः[i]);

      ret = validate_sysctl (def->linux->sysctl->कुञ्चिकाः[i], def->linux->sysctl->मानानि[i], name, namespaces_created, def, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      जइ (UNLIKELY (fd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (लिख (fd, def->linux->sysctl->मानानि[i], strlen (def->linux->sysctl->मानानि[i])));
      जइ (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क saved_errno = errno;
          cleanup_free वर्ण *reason = रिक्त;

          reason = sysctl_error_reason (def->linux->sysctl->कुञ्चिकाः[i], namespaces_created, saved_errno);
          प्रत्यागम crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
open_terminal (वर्ण **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  uid_t uid = 0;
  पूर्णाङ्क ret;

  जइ (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  जइ (UNLIKELY (fd < 0))
    प्रत्यागम fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  प्रत्यागम ret;
}

वर्ण *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  प्रत्यागम get_private_data (container)->external_descriptors;
}

पूर्णाङ्क
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  ध्रुव वर्ण *buf = रिक्त;
  json_gen_ctx *gen = रिक्त;
  size_t buf_len;
  पूर्णाङ्क ret;
  पूर्णाङ्क i;

  जइ (! json_gen_init (&gen, रिक्त))
    प्रत्यागम crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  जइ (UNLIKELY (ret != json_gen_status_ok))
    गच्छ json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  प्रति (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      वर्ण link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      जइ (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          जइ (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          अण्णहा
            {
              json_gen_free (gen);
              प्रत्यागम crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      जइ (UNLIKELY (ret != json_gen_status_ok))
        गच्छ json_error;
    }

  ret = json_gen_array_close (gen);
  जइ (UNLIKELY (ret != json_gen_status_ok))
    गच्छ json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  जइ (UNLIKELY (ret != json_gen_status_ok))
    गच्छ json_error;

  जइ (buf)
    {
      वर्ण *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  प्रत्यागम 0;

json_error:
  जइ (gen)
    json_gen_free (gen);
  प्रत्यागम json_gen_error_to_crun_error (ret, err);
}

पूर्णाङ्क
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free वर्ण *pty = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;

  जइ (def->process == रिक्त || ! def->process->terminal)
    प्रत्यागम 0;

  fd = open_terminal (&pty, def->process, err);
  जइ (UNLIKELY (fd < 0))
    प्रत्यागम fd;

  जइ (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", रिक्त, MS_BIND, रिक्त, LABEL_MOUNT, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अण्णहा
    {
      ret = unlink ("/dev/console");
      जइ (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          जइ (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", रिक्त, MS_BIND, रिक्त, LABEL_MOUNT, err);
              जइ (UNLIKELY (ret < 0))
                प्रत्यागम ret;

              प्रत्यागम get_and_reset (&fd);
            }
          प्रत्यागम crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  प्रत्यागम get_and_reset (&fd);
}

स्थिर bool
read_error_from_sync_socket (पूर्णाङ्क sync_socket_fd, पूर्णाङ्क *error, वर्ण **str)
{
  cleanup_free वर्ण *b = रिक्त;
  size_t size;
  पूर्णाङ्क code = 0;
  पूर्णाङ्क ret;

  जइ (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (पठ (sync_socket_fd, &code, परिमाण (code)));
      जइ (UNLIKELY (ret != (पूर्णाङ्क) परिमाण (code)))
        प्रत्यागम false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (पठ (sync_socket_fd, &size, परिमाण (size)));
  जइ (UNLIKELY (ret != (पूर्णाङ्क) परिमाण (size)))
    प्रत्यागम false;

  जइ (size == 0)
    प्रत्यागम false;

  जइ (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (पठ (sync_socket_fd, b, size));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम false;

  b[ret] = '\0';

  *str = b;
  b = रिक्त;
  प्रत्यागम true;
}

स्थिर bool
send_error_to_sync_socket (पूर्णाङ्क sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  पूर्णाङ्क ret;
  पूर्णाङ्क code;
  वर्ण *सन्देशः;

  जइ (err == रिक्त || *err == रिक्त)
    प्रत्यागम false;

  code = crun_error_get_errno (err);

  जइ (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, "1", 1));
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम false;
    }

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, &code, परिमाण (code)));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम false;

  सन्देशः = (*err)->सन्देशः;
  size = strlen (सन्देशः) + 1;
  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, &size, परिमाण (size)));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम false;

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, सन्देशः, size));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम false;

  प्रत्यागम true;
}

स्थिर अन्तःस्थ पूर्णाङ्क
send_success_to_sync_socket (पूर्णाङ्क sync_socket, libcrun_error_t *err)
{
  ध्रुव पूर्णाङ्क success = 0;
  पूर्णाङ्क ret;

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket, &success, परिमाण (success)));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "write to sync socket");

  प्रत्यागम 0;
}

स्थिर __attribute__ ((noreturn)) शून्य
send_error_to_sync_socket_and_die (पूर्णाङ्क sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  वर्ण *सन्देशः;

  जइ (err == रिक्त || *err == रिक्त)
    _safe_exit (EXIT_FAILURE);

  जइ (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
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
  जइ (UNLIKELY (ret != परिमाण (res)))
    प्रत्यागम crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  जइ (res == 0)
    प्रत्यागम 0;

  जइ (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      जइ (! is_empty_string (err_str))
        प्रत्यागम crun_make_error (err, res, "%s", err_str);
    }

  प्रत्यागम crun_make_error (err, res, "read from sync socket");
}

स्थिर पूर्णाङ्क
join_namespaces (runtime_spec_schema_config_schema *def, पूर्णाङ्क *namespaces_to_join, पूर्णाङ्क n_namespaces_to_join,
                 पूर्णाङ्क *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  पूर्णाङ्क i;

  प्रति (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free वर्ण *cwd = रिक्त;
      पूर्णाङ्क orig_index = namespaces_to_join_index[i];
      पूर्णाङ्क मानम्;

      जइ (namespaces_to_join[i] < 0)
        अनुवर्त;

      /* Skip the user namespace.  */
      मानम् = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      जइ (मानम् == CLONE_NEWUSER)
        अनुवर्त;

      जइ (मानम् == CLONE_NEWNS)
        {
          cwd = getcwd (रिक्त, 0);
          जइ (UNLIKELY (cwd == रिक्त))
            प्रत्यागम crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], मानम्);
      जइ (UNLIKELY (ret < 0))
        {
          जइ (ignore_join_errors)
            अनुवर्त;
          प्रत्यागम crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      जइ (मानम् == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  प्रत्यागम 0;
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

  जइ (ns == रिक्त)
    प्रत्यागम;

  प्रति (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

स्थिर पूर्णाङ्क
configure_init_status (संरचना init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  प्रति (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  प्रति (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णाङ्क मानम् = libcrun_find_namespace (def->linux->namespaces[i]->type);
      जइ (UNLIKELY (मानम् < 0))
        प्रत्यागम crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      जइ (ns->all_namespaces & मानम्)
        प्रत्यागम crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= मानम्;

      जइ (def->linux->namespaces[i]->path == रिक्त)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= मानम्;
        }
      अण्णहा
        {
          पूर्णाङ्क fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          जइ (ns->fd_len >= MAX_NAMESPACES)
            प्रत्यागम crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          जइ (UNLIKELY (fd < 0))
            प्रत्यागम crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          जइ (मानम् == CLONE_NEWUSER)
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

  जइ (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  प्रत्यागम 0;
}

/* Detect if root is available in the container.  */
स्थिर bool
मूलम्_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  प्रति (i = 0; i < len; i++)
    जइ (mappings[i]->container_id == 0)
      प्रत्यागम true;

  प्रत्यागम false;
}

स्थिर संरचना libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  संरचना libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  जइ (dev_fds == रिक्त)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  प्रत्यागम dev_fds;
}

#define अङ्कः_NEEDED_DEVS (परिमाण (needed_devs) / परिमाण (needed_devs[0]) - 1)

स्थिर संरचना libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  संरचना libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  जइ (fds == रिक्त)
    {
      fds = make_libcrun_fd_map (अङ्कः_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  प्रत्यागम fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  जइ (recursive)
    *recursive = false;

  जइ (src_nofollow != रिक्त)
    *src_nofollow = false;

  प्रति (i = 0; i < mnt->options_len; i++)
    {
      जइ (mnt->options[i] == रिक्त)
        अनुवर्त;

      जइ (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          जइ (src_nofollow == रिक्त || *src_nofollow)
            भङ्ग;
        }
      जइ (strcmp (mnt->options[i], "rbind") == 0)
        {
          जइ (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          जइ (src_nofollow == रिक्त || *src_nofollow)
            भङ्ग;
        }
      जइ (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  प्रत्यागम ret;
}

स्थिर वर्ण *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  प्रति (i = 0; i < mnt->options_len; i++)
    {
      जइ (mnt->options[i] == रिक्त)
        अनुवर्त;

      जइ (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          प्रत्यागम mnt->options[i];
        }
      जइ (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          प्रत्यागम mnt->options[i];
        }
    }
  प्रत्यागम रिक्त;
}

स्थिर पूर्णाङ्क
open_mount_type (ध्रुव वर्ण *type, पूर्णाङ्क *out_fd, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fsopen_fd = -1;
  cleanup_close पूर्णाङ्क newfs_fd = -1;
  पूर्णाङ्क ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  जइ (UNLIKELY (fsopen_fd < 0))
    प्रत्यागम crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, रिक्त, रिक्त, 0);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  जइ (UNLIKELY (newfs_fd < 0))
    प्रत्यागम crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  प्रत्यागम 0;
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
  जइ (has_mappings_out)
    *has_mappings_out = has_mappings;
  जइ (! has_mappings)
    प्रत्यागम 0;

  जइ ((mnt->uid_mappings == रिक्त) != (mnt->gid_mappings == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  जइ (idmap_option)
    {
      options = strchr (idmap_option, '=');
      जइ (options)
        {
          /* Skip the '=' itself.  */
          options++;
          जइ (options[0] == '\0')
            options = रिक्त;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  जइ (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  जइ (UNLIKELY (fd < 0))
    प्रत्यागम fd;

  जइ (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_वृक्षः (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      जइ (UNLIKELY (newfs_fd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  अण्णहा
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  जइ (UNLIKELY (ret < 0))
    {
      जइ (errno == EINVAL)
        {
          extra_सन्देशः = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      प्रत्यागम crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_सन्देशः);
    }

  *out_fd = get_and_reset (&newfs_fd);
  प्रत्यागम 0;
}

स्थिर uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  जइ (! def || ! def->linux)
    गच्छ exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  जइ (! mappings)
    गच्छ exit;

  प्रति (i = 0; i < len; i++)
    {
      जइ (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        प्रत्यागम id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  प्रत्यागम is_uid ? get_overflow_uid () : get_overflow_gid ();
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
  जइ (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (name)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "chown `%s`", device->path);

  प्रत्यागम get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

स्थिर पूर्णाङ्क
send_mounts (पूर्णाङ्क sync_socket_host, संरचना libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  पूर्णाङ्क ret;

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_host, &how_many, परिमाण (how_many)));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "write to sync socket");

  प्रति (i = 0; i < total; i++)
    {
      जइ (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (वर्ण *) &i, परिमाण (i), err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }
  प्रत्यागम 0;
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

  जइ (def->mounts_len == 0)
    प्रत्यागम 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  जइ (def->मूलम् && def->मूलम्->path)
    rootfs = realpath (def->मूलम्->path, रिक्त);

  जइ (! has_userns)
    {
      पूर्णाङ्क is_in_userns = check_running_in_user_namespace (err);
      जइ (UNLIKELY (is_in_userns < 0))
        प्रत्यागम is_in_userns;

      has_userns = is_in_userns > 0;
    }

  प्रति (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      पूर्णाङ्क mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      जइ (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == रिक्त)
        {
          unsigned long propagation = 0;

          जइ (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, रिक्त, रिक्त, रिक्त, रिक्त) & ALL_PROPAGATIONS_NO_REC;
          जइ (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          जइ (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      जइ (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  प्रत्यागम send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
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

  जइ (def->linux == रिक्त || def->linux->devices_len == 0)
    प्रत्यागम 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  जइ (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    प्रत्यागम send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_मूलम् : रिक्त),
                                     (container->context ? container->context->id : रिक्त), err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", रिक्त);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = mkdir (devs_path, 0700);
  जइ (UNLIKELY (ret < 0 && errno != EEXIST))
    प्रत्यागम crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  जइ (UNLIKELY (current_mountns < 0))
    प्रत्यागम current_mountns;

  ret = unshare (CLONE_NEWNS);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (रिक्त, "/", रिक्त, MS_REC | MS_PRIVATE, रिक्त);
  जइ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      गच्छ restore_mountns;
    }

  जइ (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      जइ (UNLIKELY (context_type == रिक्त))
        {
          ret = -1;
          गच्छ restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", रिक्त, context_type, label, रिक्त);
  जइ (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      गच्छ restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  जइ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      गच्छ restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, रिक्त);
  जइ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      गच्छ restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  जइ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      गच्छ restore_mountns;
    }

  प्रति (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      जइ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          अनुवर्त;
        }

      dev_fds->fds[i] = ret;

      जइ (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  जइ (ns_changed && current_mountns >= 0)
    {
      पूर्णाङ्क setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      जइ (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          प्रत्यागम crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, पूर्णाङ्क sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map संरचना libcrun_fd_map *fds = रिक्त;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (अङ्कः_NEEDED_DEVS);

  जइ (! has_userns || geteuid () > 0)
    प्रत्यागम send_mounts (sync_socket_host, fds, 0, अङ्कः_NEEDED_DEVS, err);

  प्रति (i = 0; i < अङ्कः_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      पूर्णाङ्क fd = syscall_open_वृक्षः (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      जइ (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  प्रत्यागम send_mounts (sync_socket_host, fds, how_many, अङ्कः_NEEDED_DEVS, err);
}

स्थिर पूर्णाङ्क
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, पूर्णाङ्क sync_socket_host, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
receive_mounts (संरचना libcrun_fd_map *fds, पूर्णाङ्क sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  पूर्णाङ्क ret;

  जइ (fds->nfds == 0)
    प्रत्यागम 0;

  ret = TEMP_FAILURE_RETRY (पठ (sync_socket_container, &how_many, परिमाण (how_many)));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "read from sync socket");

  प्रति (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (वर्ण *) &index, परिमाण (index), err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      जइ (index >= fds->nfds)
        प्रत्यागम crun_make_error (err, 0, "invalid mount data received");

      जइ (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  प्रत्यागम 0;
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

  जइ (! container->host_has_cap_sys_ptrace)
    प्रत्यागम 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  पूर्णाङ्क ret;

  जइ (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool मूलम्_mapped = false;

      जइ (def->linux->uid_mappings_len != 0)
        {
          मूलम्_mapped = मूलम्_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          जइ (! मूलम्_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      जइ (def->linux->gid_mappings_len != 0)
        {
          मूलम्_mapped = मूलम्_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          जइ (! मूलम्_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  जइ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    प्रत्यागम crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "setresgid to `%d`", gid);

  प्रत्यागम 0;
}

स्थिर bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  प्रत्यागम mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, रिक्त, रिक्त);
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

  जइ (def->मूलम् == रिक्त || def->मूलम्->path == रिक्त)
    प्रत्यागम 0;

  प्रति (i = 0; i < def->mounts_len; i++)
    जइ (is_proc_mount (def->mounts[i]))
      missing++;
  जइ (missing == 0)
    प्रत्यागम 1;

  मूलम्_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (मूलम्_fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (cwd_fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->मूलम्->path);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम 0;

  pid = fork ();
  saved_errno = errno;
  जइ (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      जइ (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (शून्य) unshare (CLONE_NEWNS);

      प्रति (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close पूर्णाङ्क fd = -1;

          जइ (! is_proc_mount (def->mounts[i]))
            अनुवर्त;

          fd = open_mount_of_type (container, def->mounts[i], err);
          जइ (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (वर्ण *) &i, परिमाण (i), err);
          जइ (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  जइ (UNLIKELY (fchdir (मूलम्_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    प्रत्यागम crun_make_error (err, errno, "restore the root directory");

  जइ (UNLIKELY (pid < 0))
    प्रत्यागम crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  जाव (missing > 0)
    {
      libcrun_error_t tmp_err = रिक्त;

      ret = receive_fd_from_socket_with_payload (socket_parent, (वर्ण *) &i, परिमाण (i), &tmp_err);
      जइ (ret < 0)
        {
          crun_error_release (&tmp_err);
          भङ्ग;
        }
      जइ (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          भङ्ग;
        }

      जइ (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, रिक्त, 0);

  प्रत्यागम missing == 0 ? 1 : 0;
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
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      जइ (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      जइ (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      जइ (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          delayed_pidns_fork = ret > 0;
        }

      जइ (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          ret = TEMP_FAILURE_RETRY (लिख (sync_socket_container, &new_pid, परिमाण (new_pid)));
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "write to sync socket");
        }
      अण्णहा
        {
          new_pid = fork ();
          जइ (UNLIKELY (new_pid < 0))
            प्रत्यागम crun_make_error (err, errno, "fork");
        }

      जइ (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          जइ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (लिख (sync_socket_container, &new_pid, परिमाण (new_pid)));
          जइ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = libcrun_set_oom (container, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  जइ ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      प्रति (i = 0; i < def->mounts_len; i++)
        {
          पूर्णाङ्क fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          जइ (def->mounts[i]->type == रिक्त)
            अनुवर्त;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          जइ (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, रिक्त, रिक्त, रिक्त);
          जइ (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, रिक्त, रिक्त, रिक्त);

          जइ (fd >= 0)
            {
              जइ (mount_fds->fds[i] >= 0)
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
  जइ (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      जइ (UNLIKELY (timens_offsets_fd < 0))
        प्रत्यागम timens_offsets_fd;
    }

  जइ (init_status->all_namespaces & CLONE_NEWUSER)
    {
      जइ (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      जइ (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      अण्णहा
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "unshare");
    }

  जइ (def->linux->time_offsets)
    {
      वर्ण fmt_buffer[128];
      पूर्णाङ्क fd = timens_offsets_fd;

      जइ (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, परिमाण (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          जइ (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (fmt_buffer)))
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long पूर्णाङ्क) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = लिख (fd, fmt_buffer, strlen (fmt_buffer));
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      जइ (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long पूर्णाङ्क) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, परिमाण (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          जइ (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (fmt_buffer)))
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

          ret = लिख (fd, fmt_buffer, strlen (fmt_buffer));
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  जइ (init_status->must_fork)
    {
      जइ (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          delayed_pidns_fork = ret > 0;
        }

      जइ (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          जइ (UNLIKELY (pid_container < 0))
            प्रत्यागम crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      जइ (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          ret = TEMP_FAILURE_RETRY (लिख (sync_socket_container, &pid_container, परिमाण (pid_container)));
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "write to sync socket");

          जइ (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  प्रत्यागम get_private_data (container)->delayed_pidns_fork;
}

पूर्णाङ्क
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क client_fd = -1;
  cleanup_close पूर्णाङ्क pidfd = -1;
  ध्रुव वर्ण *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  जइ (v == रिक्त)
    प्रत्यागम 0;

  pidfd = syscall_pidfd_open (pid, 0);
  जइ (UNLIKELY (pidfd < 0))
    प्रत्यागम crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  जइ (UNLIKELY (client_fd < 0))
    प्रत्यागम client_fd;

  प्रत्यागम send_fd_to_socket (client_fd, pidfd, err);
}

स्थिर bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  जइ (process == रिक्त || process->exec_cpu_affinity == रिक्त)
    प्रत्यागम false;
  प्रत्यागम (! is_empty_string (process->exec_cpu_affinity->initial))
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
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  जइ (def->मूलम्)
    {
      ret = do_notify_socket (container, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  जइ (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = libcrun_set_oom (container, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->मूलम् ? container->container_def->मूलम्->path : रिक्त, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  प्रति (i = 0; i < init_status.fd_len; i++)
    {
      विकल्प (init_status.मानम्[i])
        {
        पक्ष CLONE_NEWIPC:
          जइ (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          भङ्ग;

        पक्ष CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          जइ (! new_userns)
            init_status.must_fork = true;
          अण्णहा
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          भङ्ग;

        पक्ष CLONE_NEWTIME:
          जइ (! new_userns)
            init_status.must_fork = true;
          अण्णहा
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          भङ्ग;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  जइ ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  अण्णहा जइ ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  जइ (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  जइ (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      संरचना _clone3_args clone3_args;
      memset (&clone3_args, 0, परिमाण (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      जइ (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  जइ (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, रिक्त);
      जइ (UNLIKELY (pid < 0))
        प्रत्यागम crun_make_error (err, errno, "clone");
    }

  जइ (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = close_and_reset (&sync_socket_container);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_वृक्षः_fd));

      जइ (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          ret = TEMP_FAILURE_RETRY (पठ (sync_socket_host, &new_pid, परिमाण (new_pid)));
          जइ (UNLIKELY (ret != परिमाण (new_pid)))
            प्रत्यागम crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          जइ (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          अण्णहा
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, रिक्त, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      जइ (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      जइ ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      जइ (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          ret = TEMP_FAILURE_RETRY (पठ (sync_socket_host, &grandchild, परिमाण (grandchild)));
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          जइ (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          अण्णहा
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, रिक्त, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      जइ (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      pid_to_clean = 0;
      प्रत्यागम pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  जइ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  जइ (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  अण्णहा
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      जइ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->सन्देशः);
    }

  /* Jump into the specified entrypoint.  */
  जइ (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  जइ (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->सन्देशः);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  जइ (ret == 0)
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

  जइ (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (पठ (sync_fd, &res, 1));
  जइ (UNLIKELY (ret != 1))
    प्रत्यागम crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  जइ (res != '0')
    प्रत्यागम crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (पठ (sync_fd, &pid, परिमाण (pid)));
  जइ (UNLIKELY (ret != परिमाण (pid)))
    प्रत्यागम crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, रिक्त, 0);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "waitpid for exec child pid");

  जइ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (need_move_to_cgroup)
    {
      cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free वर्ण *final_cgroup = रिक्त;

      जइ (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, रिक्त);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_set_io_priority (pid, process, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (लिख (sync_fd, &ret, परिमाण (ret)));
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "write to sync socket");

  जइ (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      जइ (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क err_code = 0;
          cleanup_free वर्ण *err_str = रिक्त;

          जइ (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              जइ (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  प्रत्यागम crun_make_error (err, err_code, "%s", err_str);
                }
            }

          प्रत्यागम crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  प्रत्यागम pid;
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
  जइ (def->linux && def->linux->namespaces)
    {
      प्रति (i = 0; i < def->linux->namespaces_len; i++)
        जइ (! is_empty_string (def->linux->namespaces[i]->path))
          प्रत्यागम 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  जइ (UNLIKELY (pidfd_pid_to_join < 0))
    प्रत्यागम 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  जइ (ret == 0)
    प्रत्यागम crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  प्रति (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].मानम्;

  ret = setns (pidfd_pid_to_join, all_flags);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम 0;

  प्रत्यागम 1;
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
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  /* Nothing left to do if the namespaces were joined.  */
  जइ (LIKELY (ret > 0))
    प्रत्यागम 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  जइ (def->linux->namespaces_len >= MAX_NAMESPACES)
    प्रत्यागम crun_make_error (err, 0, "invalid configuration");

  प्रति (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free वर्ण *ns_path = रिक्त;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      जइ (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          जइ (crun_error_get_errno (err) == ENOENT)
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
  जइ (UNLIKELY (ret < 0))
    गच्छ exit;
  जइ (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      गच्छ exit;
    }

  प्रति (i = 0; namespaces[i].ns_file; i++)
    {
      जइ (namespaces[i].मानम् == CLONE_NEWUSER)
        अनुवर्त;

      ret = setns (fds[i], 0);
      जइ (ret == 0)
        fds_joined[i] = 1;
    }
  प्रति (i = 0; namespaces[i].ns_file; i++)
    {
      जइ (fds_joined[i])
        अनुवर्त;
      ret = setns (fds[i], 0);
      जइ (ret == 0)
        fds_joined[i] = 1;
    }
  प्रति (i = 0; namespaces[i].ns_file; i++)
    {
      जइ (fds_joined[i])
        अनुवर्त;
      ret = setns (fds[i], 0);
      जइ (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          प्रति (j = 0; j < def->linux->namespaces_len; j++)
            {
              जइ (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  भङ्ग;
                }
            }
          जइ (! found)
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
  प्रति (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  प्रत्यागम ret;
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

  जइ (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "set child subreaper");
    }

  जइ (path_has_dot_dot_component (sub_cgroup))
    प्रत्यागम crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  जइ (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free वर्ण *final_cgroup = रिक्त;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    जइ (get_force_cgroup_v1_annotation (container) == रिक्त)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        जइ (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, परिमाण (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  जइ (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  अण्णहा
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  जइ (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  /* On errors, fall back to fork().  */
  जइ (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      जइ (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          गच्छ exit;
        }
    }

  जइ (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      प्रत्यागम join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  जइ (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (लिख (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->सन्देशः);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  जइ (UNLIKELY (pid < 0))
    {
      पूर्णाङ्क saved_errno = errno;
      TEMP_FAILURE_RETRY (लिख (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  जइ (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (लिख (sync_fd, "0", 1));
      जइ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (लिख (sync_fd, &pid, परिमाण (pid)));
      जइ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  अण्णहा
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free वर्ण *pty = रिक्त;
      पूर्णाङ्क r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (पठ (sync_fd, &r, परिमाण (r)));
      जइ (UNLIKELY (ret != परिमाण (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      जइ (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      जइ (terminal_fd)
        {
          cleanup_close पूर्णाङ्क ptmx_fd = -1;

          ret = set_id_init (container, err);
          जइ (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          जइ (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          जइ (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  प्रत्यागम 0;

exit:
  जइ (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  जइ (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_linux_container_update (libcrun_container_status_t *status, ध्रुव वर्ण *state_मूलम्, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

  cgroup_status = libcrun_cgroup_make_status (status);

  प्रत्यागम libcrun_update_cgroup_resources (cgroup_status, state_मूलम्, resources, err);
}

स्थिर पूर्णाङ्क
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, ध्रुव bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

  cgroup_status = libcrun_cgroup_make_status (status);

  प्रत्यागम libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

पूर्णाङ्क
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  प्रत्यागम libcrun_container_pause_unpause_linux (status, true, err);
}

पूर्णाङ्क
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  प्रत्यागम libcrun_container_pause_unpause_linux (status, false, err);
}

पूर्णाङ्क
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  पूर्णाङ्क ret;

  जइ (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  अण्णहा जइ (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  अण्णहा
    प्रत्यागम crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, 0, "personality to `%s`", p->domain);

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णाङ्क sockfd = -1;

  प्रति (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णाङ्क मानम् = libcrun_find_namespace (def->linux->namespaces[i]->type);
      जइ (UNLIKELY (मानम् < 0))
        प्रत्यागम crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      जइ (मानम् == CLONE_NEWNET && def->linux->namespaces[i]->path == रिक्त)
        {
          configure_network = true;
          भङ्ग;
        }
    }

  जइ (! configure_network)
    प्रत्यागम 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  जइ (LIKELY (sockfd >= 0))
    {
      संरचना ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  अण्णहा
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
      जइ (UNLIKELY (sockfd < 0))
        प्रत्यागम crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (संरचना sockaddr *) &addr, परिमाण (addr));
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, परिमाण (buf), 0, (संरचना sockaddr *) &addr_to, परिमाण (addr_to));
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, परिमाण (buf), 0, रिक्त, रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (संरचना nlmsghdr *) buf;

      जइ (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          संरचना nlmsgerr *err_data = (संरचना nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          जइ (err_data->error < 0)
            प्रत्यागम crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  प्रत्यागम 0;
}

/* Protection for attacks like CVE-2019-5736.  */
पूर्णाङ्क ensure_cloned_द्विमान ();
__attribute__ ((constructor)) स्थिर शून्य
libcrun_rexec (शून्य)
{
  जइ (ensure_cloned_द्विमान () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

पूर्णाङ्क
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  प्रत्यागम libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

पूर्णाङ्क
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
स्थिर पूर्णाङ्क
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, पूर्णाङ्क सङ्केत, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  जइ (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      /* The pid is not valid anymore, return an error.  */
      जइ (ret == 0)
        {
          errno = ESRCH;
          प्रत्यागम crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, सङ्केत);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "kill container");
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_kill_linux (libcrun_container_status_t *status, पूर्णाङ्क सङ्केत, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  जइ (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      जइ (errno == ENOSYS)
        प्रत्यागम libcrun_kill_linux_no_pidfd (status, true, सङ्केत, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      जइ (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        प्रत्यागम crun_make_error (err, ESRCH, "process not running");
      प्रत्यागम crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* The pid is not valid anymore, return an error.  */
  जइ (ret == 0)
    {
      errno = ESRCH;
      प्रत्यागम crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, सङ्केत, रिक्त, 0);
  जइ (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      जइ (errno == ENOSYS)
        प्रत्यागम libcrun_kill_linux_no_pidfd (status, false, सङ्केत, err);
      प्रत्यागम crun_make_error (err, errno, "send signal to pidfd");
    }

  प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *
libcrun_get_intelrdt_name (ध्रुव वर्ण *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  जइ (def == रिक्त || def->linux == रिक्त || def->linux->intel_rdt == रिक्त || def->linux->intel_rdt->clos_id == रिक्त)
    {
      जइ (explicit)
        *explicit = false;
      प्रत्यागम ctr_name;
    }

  जइ (explicit)
    *explicit = true;
  प्रत्यागम def->linux->intel_rdt->clos_id;
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

  जइ (container)
    def = container->container_def;

  जइ (def == रिक्त || def->linux == रिक्त || def->linux->intel_rdt == रिक्त)
    प्रत्यागम 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  जइ (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      जइ (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          जइ (UNLIKELY (ret < 0))
            गच्छ fail;

          has_monitoring = true;
        }
    }

  जइ (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      जइ (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  जइ (actions & LIBCRUN_INTELRDT_MOVE)
    {
      ध्रुव वर्ण *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : रिक्त;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      जइ (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  प्रत्यागम 0;

fail:
  जइ (has_monitoring)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      जइ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  जइ (created)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      जइ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_destroy_intelrdt (ध्रुव वर्ण *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  ध्रुव वर्ण *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  जइ (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      पूर्णाङ्क ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      जइ (ret < 0)
        प्रत्यागम ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  जइ (explicit)
    प्रत्यागम 0;

  प्रत्यागम resctl_destroy (clos_id, err);
}

पूर्णाङ्क
libcrun_update_intel_rdt (ध्रुव वर्ण *ctr_name, libcrun_container_t *container, ध्रुव वर्ण *l3_cache_schema, ध्रुव वर्ण *mem_bw_schema, वर्ण *ध्रुव *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = रिक्त;
  ध्रुव वर्ण *name;

  जइ (container)
    def = container->container_def;

  जइ (def == रिक्त || def->linux == रिक्त || def->linux->intel_rdt == रिक्त)
    प्रत्यागम 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, रिक्त);

  प्रत्यागम resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
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
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  जइ (ret == 0 || buffer[0] != '/')
    {
      जइ (chdir ("/") < 0)
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

      जइ ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        प्रत्यागम crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      प्रत्यागम crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  प्रत्यागम 0;
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
  जइ (UNLIKELY (pid < 0))
    प्रत्यागम crun_make_error (err, errno, "vfork");

  जइ (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      जइ (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      जइ (UNLIKELY (ret < 0))
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
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  प्रत्यागम get_process_exit_status (wait_status);
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

  जइ (def->linux)
    प्रति (i = 0; i < def->linux->namespaces_len; i++)
      जइ (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == रिक्त;
          भङ्ग;
        }

  जइ (path == रिक्त)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      जइ (UNLIKELY (expected_fd < 0))
        प्रत्यागम expected_fd;
    }
  अण्णहा
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      जइ (UNLIKELY (expected_fd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  जइ (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      जइ (UNLIKELY (parent_fd < 0))
        प्रत्यागम crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fstat user namespace");

  जइ (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    प्रत्यागम crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  प्रत्यागम 0;
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
  जइ (UNLIKELY (mntns < 0))
    प्रत्यागम mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (ret == 0)
    प्रत्यागम crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  जइ (UNLIKELY (owner < 0))
    प्रत्यागम crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, पूर्णाङ्क (*callback) (शून्य *, libcrun_error_t *), शून्य *arg, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क mntns_fd = -1;
  cleanup_close पूर्णाङ्क owner_fd = -1;
  पूर्णाङ्क ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम run_callback_in_namespace (mntns_fd, callback, arg, err);
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

  जइ (is_empty_string (destination))
    प्रत्यागम crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  जइ (UNLIKELY (len >= परिमाण (buffer)))
    प्रत्यागम crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  जाव (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  जइ (last_slash == रिक्त)
    {
      parent = ".";
      name = buffer;
    }
  अण्णहा जइ (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  अण्णहा
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  जइ (UNLIKELY (parentfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  जइ (UNLIKELY (cwdfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  जइ (UNLIKELY (fchdir (cwdfd) < 0))
    प्रत्यागम crun_make_error (err, errno, "fchdir to the previous working directory");

  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, saved_errno, "umount `%s`", destination);

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
do_umount_in_a_container (शून्य *arg, libcrun_error_t *err)
{
  संरचना umount_in_a_container_args *args = arg;
  size_t i;
  पूर्णाङ्क ret;

  प्रति (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
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
  जइ (LIKELY (fd >= 0))
    प्रत्यागम fd;

  जइ (errno != ENOENT)
    प्रत्यागम crun_make_error (err, errno, "open `%s`", destination);

  जइ (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  अण्णहा
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  जइ (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", destination);

  प्रत्यागम fd;
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
  जइ (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "setresuid to `0`");
    }
  अण्णहा जइ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      प्रत्यागम crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  प्रति (i = 0; i < args->len; i++)
    {
      cleanup_close पूर्णाङ्क dest_fd = -1;
      संरचना stat st;

      ret = fstat (args->fds->fds[i], &st);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      जइ (UNLIKELY (dest_fd < 0))
        प्रत्यागम dest_fd;
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
do_mount_in_a_container (शून्य *arg, libcrun_error_t *err)
{
  संरचना mount_in_a_container_args *args = arg;
  size_t i;
  पूर्णाङ्क ret;

  प्रति (i = 0; i < args->len; i++)
    {
      cleanup_close पूर्णाङ्क dest_fd = -1;
      संरचना stat st;

      ret = fstat (args->fds->fds[i], &st);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      जइ (UNLIKELY (dest_fd < 0))
        प्रत्यागम dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  प्रत्यागम 0;
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
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  जइ (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  अण्णहा जइ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      प्रत्यागम crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  जइ (UNLIKELY (pid < 0))
    प्रत्यागम crun_make_error (err, errno, "fork");

  जइ (pid)
    {
      पूर्णाङ्क wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      जइ (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, रिक्त, 0));
          प्रत्यागम crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  प्रति (i = 0; i < len; i++)
    {
      cleanup_close पूर्णाङ्क fd = -1;

      जइ (fds->fds[i] >= 0)
        अनुवर्त;

      fd = open_mount_of_type (container, mounts[i], err);
      जइ (UNLIKELY (fd < 0))
        प्रत्यागम crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
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

  प्रति (i = 0; i < परिमाण (namespaces) / परिमाण (namespaces[0]); i++)
    {
      cleanup_close पूर्णाङ्क self_fd = -1;
      cleanup_close पूर्णाङ्क target_fd = -1;
      संरचना stat self_st, target_st;
      cleanup_free वर्ण *self_path = रिक्त;
      पूर्णाङ्क ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      जइ (UNLIKELY (self_fd < 0))
        प्रत्यागम self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      जइ (UNLIKELY (target_fd < 0))
        प्रत्यागम target_fd;

      ret = fstat (self_fd, &self_st);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      जइ (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  प्रत्यागम 0;
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

  प्रति (i = 0; i < len; i++)
    {
      जइ (fds->fds[i] >= 0)
        अनुवर्त;

      जइ (mounts[i]->type == रिक्त)
        प्रत्यागम crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  जइ (! needed)
    प्रत्यागम 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  जइ (UNLIKELY (pidfd < 0))
    प्रत्यागम crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (ret == 0)
    प्रत्यागम crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  जइ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      प्रत्यागम ret;
    }

  जइ (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      जइ (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  प्रति (i = 0; i < len; i++)
    {
      जइ (fds->fds[i] >= 0)
        अनुवर्त;

      ret = receive_fd_from_socket (sync_fd, err);
      जइ (UNLIKELY (ret < 0))
        {
          cleanup_free वर्ण *err_str = रिक्त;
          पूर्णाङ्क err_code = 0;

          जइ (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          अण्णहा
            ret = crun_error_wrap (err, "receive mount fd");
          भङ्ग;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, रिक्त, 0));

  प्रत्यागम ret;
}

स्थिर unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free वर्ण *data = रिक्त;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  जइ (mount->options == रिक्त)
    प्रत्यागम get_default_flags (container, mount->destination, &data);

  प्रति (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  प्रत्यागम flags;
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
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  fds = make_libcrun_fd_map (len);

  प्रति (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), रिक्त, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      जइ (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          जइ (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          जइ (UNLIKELY (fds->fds[i] < 0))
            प्रत्यागम fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रति (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  जइ (UNLIKELY (ret > 0))
    प्रत्यागम *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

पूर्णाङ्क
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  संरचना umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  प्रत्यागम run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
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
  जइ (LIKELY (ret == 0))
    ret = chroot (".");
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "chroot to the container root");

  प्रति (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      ध्रुव वर्ण *target = consume_slashes (args->mounts[i].target);
      cleanup_close पूर्णाङ्क fd = -1;
      संरचना statfs sfs;
      unsigned long current_flags;

      जइ (flags == 0)
        अनुवर्त;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      जइ (UNLIKELY (fd < 0))
        {
          जइ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              अनुवर्त;
            }
          प्रत्यागम fd;
        }

      ret = fstatfs (fd, &sfs);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      जइ ((current_flags & flags) == flags)
        अनुवर्त;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      जइ (LIKELY (ret == 0))
        अनुवर्त;

      जइ (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        प्रत्यागम ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (रिक्त, target, रिक्त, MS_REMOUNT | MS_BIND | flags | current_flags, रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "remount `/%s`", target);
    }

  प्रत्यागम 0;
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
  जइ (UNLIKELY (mntns_fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, परिमाण (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  जइ (UNLIKELY (rootfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  जइ (UNLIKELY (ret > 0))
    प्रत्यागम *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णाङ्क netns_fd = -1;
  size_t i;
  पूर्णाङ्क ret;

  जइ (def == रिक्त || def->linux == रिक्त || def->linux->net_devices == रिक्त)
    प्रत्यागम 0;

  cleanup_free वर्ण *ns_path = रिक्त;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  जइ (UNLIKELY (netns_fd < 0))
    प्रत्यागम netns_fd;

  प्रति (i = 0; i < def->linux->net_devices->len; i++)
    {
      ध्रुव वर्ण *new_name = def->linux->net_devices->मानानि[i]->name ?: def->linux->net_devices->कुञ्चिकाः[i];

      ret = move_network_device (def->linux->net_devices->कुञ्चिकाः[i], new_name, netns_fd, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}
