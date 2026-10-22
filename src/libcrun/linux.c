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
#यदि defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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

#ifndef OPEN_रूख_NAMESPACE
#  define OPEN_रूख_NAMESPACE 2
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
स्थिर अचिन्हित दीर्घ
mount_flags_from_statfs (अचिन्हित दीर्घ f_flags)
{
  अचिन्हित दीर्घ flags = 0;

  यदि (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  यदि (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  यदि (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  यदि (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  यदि (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  यदि (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  यदि (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  यदि (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  यदि (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  फर्काऊ flags;
}

संरचना remount_s
{
  संरचना remount_s *next;
  वर्ण *target;
  पूर्णाङ्क targetfd;
  अचिन्हित दीर्घ flags;
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
  अचिन्हित दीर्घ rootfs_propagation;
  bool deny_setgroups;

  पूर्णाङ्क procfd;

  अचर वर्ण *rootfs;
  पूर्णाङ्क rootfsfd;

  पूर्णाङ्क notify_socket_रूख_fd;
  पूर्णाङ्क old_जरा_fd;

  संरचना libcrun_fd_map *mount_fds;
  संरचना libcrun_fd_map *dev_fds;
  संरचना libcrun_fd_map *needed_devs_fds;

  वर्ण **copy_symlink_targets;
  माप_प्रकार n_copy_symlink_targets;

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
  bool host_जरा_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  वर्ण *host_rootfs;
};

संरचना linux_namespace_s
{
  अचर वर्ण *name;
  अचर वर्ण *ns_file;
  पूर्णाङ्क मान;
};

स्थिर शून्य
cleanup_private_data (शून्य *private_data)
{
  संरचना private_data_s *p = private_data;

  यदि (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (बंद (p->rootfsfd));
  यदि (p->procfd >= 0)
    TEMP_FAILURE_RETRY (बंद (p->procfd));
  यदि (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (बंद (p->maskdir_fd));
  यदि (p->old_जरा_fd >= 0)
    TEMP_FAILURE_RETRY (बंद (p->old_जरा_fd));
  यदि (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  यदि (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  यदि (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  यदि (p->copy_symlink_targets)
    {
      माप_प्रकार i;
      लागि (i = 0; i < p->n_copy_symlink_targets; i++)
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
  यदि (container->private_data == रिक्त)
    {
      संरचना private_data_s *p = xmalloc0 (परिमाण (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_रूख_fd = -1;
      p->old_जरा_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  फर्काऊ container->private_data;
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
  फर्काऊ tmp;
}

पूर्णाङ्क
libcrun_find_namespace (अचर वर्ण *name)
{
  संरचना linux_namespace_s *it;
  लागि (it = namespaces; it->name; it++)
    यदि (म_भेद (it->name, name) == 0)
      फर्काऊ it->मान;
  फर्काऊ -1;
}

स्थिर पूर्णाङ्क
syscall_keyctl_join (अचर वर्ण *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  फर्काऊ (पूर्णाङ्क) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

स्थिर पूर्णाङ्क
syscall_pidfd_open (pid_t pid, अचिन्हित पूर्णाङ्क flags)
{
#यदि defined __NR_pidfd_open
  फर्काऊ (पूर्णाङ्क) syscall (__NR_pidfd_open, pid, flags);
#अन्यथा
  (शून्य) pid;
  (शून्य) flags;
  त्रुटि_सं = ENOSYS;
  फर्काऊ -1;
#endif
}

स्थिर पूर्णाङ्क
syscall_pidfd_send_signal (पूर्णाङ्क pidfd, पूर्णाङ्क sig, siginfo_t *info, अचिन्हित पूर्णाङ्क flags)
{
#यदि defined __NR_pidfd_send_signal
  फर्काऊ (पूर्णाङ्क) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#अन्यथा
  (शून्य) pidfd;
  (शून्य) sig;
  (शून्य) info;
  (शून्य) flags;
  त्रुटि_सं = ENOSYS;
  फर्काऊ -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
स्थिर uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  यदि (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  यदि (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  यदि (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  यदि (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  यदि (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  यदि (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  यदि (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  यदि (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  फर्काऊ attr;
}

स्थिर पूर्णाङ्क
do_mount_setattr (bool recursive, अचर वर्ण *target, पूर्णाङ्क targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
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
  यदि (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      यदि (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      अन्यथा यदि (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      अन्यथा
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      यदि (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  अन्यथा यदि (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "mount_setattr `/%s`", target);

  फर्काऊ 0;
}

पूर्णाङ्क
get_bind_mount (पूर्णाङ्क dirfd, अचर वर्ण *src, bool recursive, bool rdonly, bool nofollow, अचिन्हित दीर्घ propagation, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क open_रूख_fd = -1;
  संरचना mount_attr_s attr = {
    0,
  };
  पूर्णाङ्क recursive_flag = (recursive ? AT_RECURSIVE : 0);
  पूर्णाङ्क ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  यदि (UNLIKELY (src == रिक्त))
    फर्काऊ crun_make_error (err, EINVAL, "mount source is not specified");

  यदि (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  यदि (dirfd < 0)
    dirfd = AT_FDCWD;

  त्रुटि_सं = 0;
  open_रूख_fd = syscall_open_रूख (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  यदि (UNLIKELY (open_रूख_fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_रूख_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "mount_setattr `%s`", src);

  फर्काऊ get_and_reset (&open_रूख_fd);
}

पूर्णाङ्क
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, वर्ण *option, वर्ण **out, माप_प्रकार *len, libcrun_error_t *err)
{
  माप_प्रकार written = 0, allocated = 256;
  cleanup_free वर्ण *mappings = रिक्त;
  अचर वर्ण *it;
  पूर्णाङ्क ret;

  mappings = xmalloc (allocated);

  लागि (it = option; *it;)
    {
      bool relative = false;
      दीर्घ मान[3];
      माप_प्रकार i;

      यदि (*it == '\0')
        रोक;

      यदि (*it == '#')
        it++;

      यदि (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      लागि (i = 0; i < 3; i++)
        {
          वर्ण *endptr = रिक्त;

          यदि (i > 0 && *it == '-')
            it++;

          यदि (*it == '\0')
            फर्काऊ crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          त्रुटि_सं = 0;
          मान[i] = म_से_दीर्घ (it, &endptr, 10);
          यदि (त्रुटि_सं || endptr == it)
            फर्काऊ crun_make_error (err, त्रुटि_सं, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      यदि (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          माप_प्रकार mappings_len;

          यदि (def == रिक्त
              || def->linux == रिक्त
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            फर्काऊ crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          लागि (i = 0; i < mappings_len; i++)
            यदि (मान[1] >= (int64_t) mappings[i]->container_id
                && मान[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              रोक;

          यदि (i == mappings_len)
            फर्काऊ crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          मान[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      यदि (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", मान[0], मान[1], मान[2]);
      यदि (UNLIKELY (ret >= (पूर्णाङ्क) (allocated - written)))
        फर्काऊ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = रिक्त;

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
format_mount_mappings (वर्ण **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       माप_प्रकार mappings_len,
                       माप_प्रकार *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  अचर माप_प्रकार max_len_mapping = 64;
  cleanup_free वर्ण *ret = रिक्त;
  माप_प्रकार s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  लागि (s = 0; s < mappings_len; s++)
    {
      माप_प्रकार len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      यदि (UNLIKELY (len >= max_len_mapping))
        फर्काऊ crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = रिक्त;
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
format_mount_mapping (वर्ण **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t size, माप_प्रकार *written, libcrun_error_t *err)
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

  फर्काऊ format_mount_mappings (ret, mappings, 1, written, err);
}

स्थिर bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  माप_प्रकार s;

  यदि (def->linux == रिक्त)
    फर्काऊ mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  यदि (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    फर्काऊ false;

  यदि (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    फर्काऊ false;

  लागि (s = 0; s < mnt->uid_mappings_len; s++)
    {
      यदि (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        फर्काऊ false;
      यदि (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        फर्काऊ false;
      यदि (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        फर्काऊ false;
    }

  लागि (s = 0; s < mnt->gid_mappings_len; s++)
    {
      यदि (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        फर्काऊ false;
      यदि (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        फर्काऊ false;
      यदि (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        फर्काऊ false;
    }

  फर्काऊ true;
}

स्थिर pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        अचर वर्ण *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != रिक्त;

  यदि (! need_new_userns)
    फर्काऊ 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, रिक्त);
  यदि (UNLIKELY (pid < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "clone");

  यदि (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      जबसम्म (1)
        छोड़ो ();
      _safe_exit (निकास_सफल);
    }

  यदि (mnt->uid_mappings_len)
    {
      cleanup_free वर्ण *uid_map = रिक्त;
      cleanup_free वर्ण *gid_map = रिक्त;
      माप_प्रकार written = 0;
      पूर्णाङ्क ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      cleanup_close पूर्णाङ्क fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      यदि (UNLIKELY (fd < 0))
        फर्काऊ fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      यदि (UNLIKELY (fd < 0))
        फर्काऊ fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  अन्यथा
    {
      cleanup_free वर्ण *dup_options = रिक्त;
      वर्ण *option, *saveptr = रिक्त;

      यदि (! options)
        फर्काऊ crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      लागि (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (रिक्त, ";", &saveptr))
        {
          cleanup_free वर्ण *mappings = रिक्त;
          cleanup_close पूर्णाङ्क map_fd = -1;
          bool is_uids = false;
          माप_प्रकार len = 0;
          पूर्णाङ्क ret;

          यदि (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              यदि (UNLIKELY (map_fd < 0))
                फर्काऊ map_fd;
            }
          अन्यथा यदि (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              यदि (UNLIKELY (map_fd < 0))
                फर्काऊ map_fd;
            }
          अन्यथा
            फर्काऊ crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_create_keyring (libcrun_container_t *container, अचर वर्ण *name, अचर वर्ण *label, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क labelfd = -1;
  bool label_set = false;
  पूर्णाङ्क ret;

  यदि (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      यदि (UNLIKELY (labelfd < 0))
        {
          यदि (crun_error_get_errno (err) != ENOENT)
            फर्काऊ labelfd;

          crun_error_release (err);
        }

      यदि (labelfd >= 0)
        {
          ret = लिख (labelfd, label, म_माप (label));
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (त्रुटि_सं == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          गच्छ out;
        }
      ret = crun_make_error (err, त्रुटि_सं, "join keyctl `%s`", name ? name : "");
      गच्छ out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  यदि (label_set && लिख (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  फर्काऊ ret;
}

स्थिर शून्य
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  यदि (def->process && def->process->user)
    {
      यदि (def->process->user->uid)
        *uid = def->process->user->uid;
      यदि (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

स्थिर अचिन्हित दीर्घ
get_mount_flags (अचर वर्ण *name, पूर्णाङ्क current_flags, पूर्णाङ्क *found, अचिन्हित दीर्घ *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  अचर संरचना propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  यदि (found)
    *found = prop ? 1 : 0;

  यदि (prop == रिक्त)
    फर्काऊ 0;

  यदि (prop->extra_flags & OPTION_RECURSIVE)
    {
      यदि (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      यदि (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  यदि (extra_flags)
    *extra_flags |= prop->extra_flags;

  यदि (prop->clear)
    फर्काऊ current_flags & ~prop->flags;

  फर्काऊ current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
स्थिर bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  माप_प्रकार i;

  लागि (i = 0; i < mount->options_len; i++)
    {
      अचर संरचना propagation_flags_s *prop;

      यदि (mount->options[i] == रिक्त)
        जारी;

      prop = libcrun_str2mount_flags (mount->options[i]);
      यदि (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        फर्काऊ true;
    }

  फर्काऊ false;
}

स्थिर अचिन्हित दीर्घ
get_mount_flags_or_option (अचर वर्ण *name, पूर्णाङ्क current_flags, अचिन्हित दीर्घ *extra_flags, वर्ण **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  पूर्णाङ्क found;
  __attribute__ ((unused)) cleanup_free वर्ण *prev = रिक्त;
  अचिन्हित दीर्घ flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  यदि (name == रिक्त)
    फर्काऊ 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  यदि (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      अचर वर्ण *data_flag = रिक्त;
      यदि ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      अन्यथा यदि ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      यदि (data_flag)
        {
          prev = *option;
          यदि (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          अन्यथा
            *option = xstrdup (data_flag);
        }
      फर्काऊ flags;
    }

  prev = *option;
  यदि (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  अन्यथा
    *option = xstrdup (name);

  फर्काऊ 0;
}

पूर्णाङ्क
pivot_जरा (अचर वर्ण *new_जरा, अचर वर्ण *put_old)
{
  फर्काऊ syscall (__NR_pivot_root, new_जरा, put_old);
}

स्थिर शून्य
free_remount (संरचना remount_s *r)
{
  यदि (r == रिक्त)
    फर्काऊ;
  यदि (r->targetfd >= 0)
    बंद (r->targetfd);
  मुक्त (r->data);
  मुक्त (r->target);
  मुक्त (r);
}

स्थिर संरचना remount_s *
make_remount (पूर्णाङ्क targetfd, अचर वर्ण *target, अचिन्हित दीर्घ flags, अचर वर्ण *data, संरचना remount_s *next)
{
  संरचना remount_s *ret = xmalloc (परिमाण (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : रिक्त;
  ret->next = next;
  ret->targetfd = targetfd;
  फर्काऊ ret;
}

स्थिर पूर्णाङ्क
do_remount (पूर्णाङ्क targetfd, अचर वर्ण *target, अचिन्हित दीर्घ flags, अचर वर्ण *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  पूर्णाङ्क ret;
  अचर वर्ण *real_target = target;

  यदि (targetfd >= 0)
    {
      अचिन्हित दीर्घ set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      अचिन्हित दीर्घ clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      यदि (LIKELY (ret == 0))
        फर्काऊ 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  यदि (flags & (MS_REMOUNT | MS_RDONLY))
    data = रिक्त;

  ret = mount (रिक्त, real_target, रिक्त, flags, data);
  यदि (UNLIKELY (ret < 0))
    {
      अचिन्हित दीर्घ current_flags, remount_flags;
      संरचना statfs sfs;

      ret = statfs (real_target, &sfs);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      यदि (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      यदि ((flags | remount_flags) != flags)
        {
          ret = mount (रिक्त, real_target, रिक्त, flags | remount_flags, data);
          यदि (LIKELY (ret == 0))
            फर्काऊ 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          यदि (current_flags & MS_RDONLY)
            ret = mount (रिक्त, real_target, रिक्त, flags | remount_flags | MS_RDONLY, data);
        }
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "remount `%s`", target);
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret = 0;
  संरचना remount_s *r = get_private_data (container)->remounts;
  जबसम्म (r)
    {
      संरचना remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      यदि (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          अचिन्हित दीर्घ set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          अचिन्हित दीर्घ clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          यदि (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              जारी;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  जबसम्म (r)
    {
      संरचना remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = रिक्त;
  फर्काऊ ret;
}

स्थिर पूर्णाङ्क
open_mount_target (libcrun_container_t *container, अचर वर्ण *target_rel, libcrun_error_t *err)
{
  अचर वर्ण *rootfs = get_private_data (container)->rootfs;
  पूर्णाङ्क rootfsfd = get_private_data (container)->rootfsfd;

  यदि (rootfsfd < 0)
    फर्काऊ crun_make_error (err, 0, "invalid rootfs state");

  फर्काऊ safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
स्थिर पूर्णाङ्क
fsopen_mount (अचर वर्ण *type, अचर वर्ण *source_name, अचर वर्ण *labeltype, अचर वर्ण *label, अचर वर्ण *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close पूर्णाङ्क fsfd = -1;
  पूर्णाङ्क ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  यदि (UNLIKELY (fsfd < 0))
    फर्काऊ fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (शून्य) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  यदि (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  यदि (data && data[0])
    {
      cleanup_free वर्ण *data_copy = xstrdup (data);
      वर्ण *saveptr = रिक्त;
      वर्ण *token;

      लागि (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (रिक्त, ",", &saveptr))
        {
          वर्ण *eq = म_अक्षर (token, '=');
          यदि (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          अन्यथा
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, रिक्त, 0);

          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, रिक्त, रिक्त, 0);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#अन्यथा
  (शून्य) type;
  (शून्य) source_name;
  (शून्य) data;
  त्रुटि_सं = ENOSYS;
  फर्काऊ -1;
#endif
}

स्थिर पूर्णाङ्क
get_procfd (संरचना private_data_s *data, libcrun_error_t *err)
{
  पूर्णाङ्क fd;

  यदि (data->procfd >= 0)
    फर्काऊ data->procfd;

  fd = fsopen_mount ("proc", रिक्त, रिक्त, रिक्त, रिक्त);
  यदि (fd < 0)
    fd = खोलो ("/proc", O_DIRECTORY | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `/proc`");

  data->procfd = fd;
  फर्काऊ fd;
}

स्थिर पूर्णाङ्क
get_old_जरा_fd (संरचना private_data_s *data)
{
  यदि (data->old_जरा_fd >= 0)
    फर्काऊ data->old_जरा_fd;

  data->old_जरा_fd = खोलो ("/", O_PATH | O_CLOEXEC);
  फर्काऊ data->old_जरा_fd;
}

स्थिर पूर्णाङ्क
fs_move_mount_to (पूर्णाङ्क fd, पूर्णाङ्क dirfd, अचर वर्ण *name)
{
#ifdef HAVE_NEW_MOUNT_API
  यदि (name)
    फर्काऊ syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  फर्काऊ syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#अन्यथा
  (शून्य) fd;
  (शून्य) dirfd;
  (शून्य) name;
  (शून्य) syscall_move_mount;
  त्रुटि_सं = ENOSYS;
  फर्काऊ -1;
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

स्थिर पूर्णाङ्क do_mount (libcrun_container_t *container, अचर वर्ण *source, पूर्णाङ्क targetfd,
                     अचर वर्ण *target, अचर वर्ण *fstype, अचिन्हित दीर्घ mountflags,
                     अचर शून्य *data, पूर्णाङ्क extra_flags, libcrun_error_t *err);

स्थिर bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, अचर वर्ण *option)
{
  माप_प्रकार i;
  लागि (i = 0; i < mnt->options_len; i++)
    यदि (mnt->options[i] != रिक्त && म_भेद (mnt->options[i], option) == 0)
      फर्काऊ true;
  फर्काऊ false;
}

स्थिर runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, अचर वर्ण *destination)
{
  माप_प्रकार i;
  runtime_spec_schema_config_schema *def = container->container_def;

  लागि (i = 0; i < def->mounts_len; i++)
    {
      यदि (म_भेद (def->mounts[i]->destination, destination) == 0)
        फर्काऊ def->mounts[i];
    }
  फर्काऊ रिक्त;
}

स्थिर शून्य
warn_tmpfs_fallback_once (संरचना private_data_s *private_data, अचर वर्ण *reason)
{
  यदि (! private_data->maskdir_warned)
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
  यदि (private_data->maskdir_proc_path != रिक्त)
    {
      यदि (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      फर्काऊ 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_जरा, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  यदि (fd < 0)
    फर्काऊ fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  यदि (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  फर्काऊ 0;
}

स्थिर bool
is_dev_null (अचर संरचना stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  फर्काऊ S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
स्थिर पूर्णाङ्क
ensure_dev_null (पूर्णाङ्क fd, libcrun_error_t *err)
{
  संरचना stat st;

  यदि (UNLIKELY (ख_स्थिति (fd, &st) < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "stat `/dev/null`");

  यदि (UNLIKELY (! is_dev_null (&st)))
    फर्काऊ crun_make_error (err, 0, "`/dev/null` is not the null device");

  फर्काऊ 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
स्थिर bool
is_empty_ro_tmpfs (पूर्णाङ्क fd)
{
  संरचना statfs sfs;

  यदि (fstatfs (fd, &sfs) < 0)
    फर्काऊ false;

  फर्काऊ sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

स्थिर पूर्णाङ्क
mount_masked_dir (libcrun_container_t *container, पूर्णाङ्क pathfd, अचर वर्ण *rel_path, libcrun_error_t *err)
{
  संरचना private_data_s *private_data = get_private_data (container);
  cleanup_close पूर्णाङ्क newfd = -1;
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क ret;

  यदि (private_data->maskdir_fd < 0)
    {
      यदि (private_data->maskdir_bind_failed)
        गच्छ fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, रिक्त, &tmp_err);
      यदि (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->सन्देश);
          crun_error_release (&tmp_err);
          गच्छ fallback_to_tmpfs;
        }
    }

  यदि (private_data->maskdir_fd >= 0)
    {
      पूर्णाङ्क rootfsfd = private_data->rootfsfd;
      संरचना stat before, after;

      ret = ख_स्थिति (private_data->maskdir_fd, &before);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, रिक्त);
      यदि (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          यदि (UNLIKELY (newfd < 0))
            फर्काऊ newfd;

          ret = ख_स्थिति (newfd, &after);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat masked dir `%s`", rel_path);

          यदि (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            फर्काऊ crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (बंद (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          फर्काऊ 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, त्रुटि_सं, "move mount masked dir `%s`", rel_path);

      TEMP_FAILURE_RETRY (बंद (private_data->maskdir_fd));
      private_data->maskdir_fd = -1;
      private_data->maskdir_move_failed = true;
    }

  private_data->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_data, tmp_err ? tmp_err->सन्देश : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  यदि (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      यदि (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      अन्यथा
        {
          यदि (ret >= 0)
            TEMP_FAILURE_RETRY (बंद (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
do_masked_or_readonly_path (libcrun_container_t *container, अचर वर्ण *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  अचिन्हित दीर्घ mount_flags = 0;
  अचर वर्ण *rootfs = get_private_data (container)->rootfs;
  cleanup_close पूर्णाङ्क pathfd = -1;
  संरचना statfs sfs;
  पूर्णाङ्क ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  यदि (UNLIKELY (rel_path == रिक्त))
    फर्काऊ crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  यदि (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  यदि (UNLIKELY (pathfd < 0))
    {
      त्रुटि_सं = crun_error_get_errno (err);
      यदि (त्रुटि_सं != ENOENT && त्रुटि_सं != EACCES)
        फर्काऊ pathfd;

      यदि (त्रुटि_सं == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      फर्काऊ 0;
    }

  यदि (readonly)
    {
      पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close पूर्णाङ्क mountfd = -1;

      यदि (UNLIKELY (procfd < 0))
        फर्काऊ procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      यदि (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          यदि (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, रिक्त);
          अन्यथा
            ret = -1;

          यदि (ret < 0)
            crun_error_release (err);
        }

      यदि (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          यदि (keep_flags)
            {
              cleanup_close पूर्णाङ्क sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              यदि (UNLIKELY (sfd < 0))
                फर्काऊ crun_make_error (err, त्रुटि_सं, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              यदि (UNLIKELY (ret < 0))
                फर्काऊ crun_make_error (err, त्रुटि_सं, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          यदि (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              यदि (UNLIKELY (ret < 0))
                फर्काऊ ret;

              ret = fs_move_mount_to (mountfd, pathfd, रिक्त);
              यदि (UNLIKELY (ret < 0))
                फर्काऊ crun_make_error (err, त्रुटि_सं, "move mount for readonly path `%s`", rel_path);
            }
          अन्यथा
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, रिक्त, mount_flags, रिक्त,
                              LABEL_NONE, err);
              यदि (UNLIKELY (ret < 0))
                फर्काऊ ret;
            }
        }
    }
  अन्यथा
    {
      ret = get_file_type_fd (pathfd, &mode);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot stat `%s`", rel_path);

      यदि ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      अन्यथा
        {
          cleanup_close पूर्णाङ्क nullfd = -1;
          cleanup_close पूर्णाङ्क mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = खोलो ("/dev/null", O_PATH | O_CLOEXEC);
          यदि (UNLIKELY (nullfd < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          यदि (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, रिक्त);

          यदि (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, रिक्त, MS_BIND | MS_RDONLY, रिक्त,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  फर्काऊ 0;
}

स्थिर अचर वर्ण *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  अचर वर्ण *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  यदि (context_type)
    {
      यदि (म_भेद (context_type, "context") != 0
          && म_भेद (context_type, "fscontext") != 0
          && म_भेद (context_type, "defcontext") != 0
          && म_भेद (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          फर्काऊ रिक्त;
        }
      फर्काऊ context_type;
    }

  फर्काऊ "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
स्थिर पूर्णाङ्क
diagnose_mount_failure (पूर्णाङ्क ret, libcrun_error_t *err, libcrun_container_t *container,
                        अचर वर्ण *fstype)
{
  यदि (fstype && म_भेद (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      यदि (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && त्रुटि_सं == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          फर्काऊ ret;
        }
    }
  फर्काऊ ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
स्थिर पूर्णाङ्क
sysfs_userns_fallback (libcrun_container_t *container, अचर वर्ण *target,
                       अचर वर्ण *real_target, पूर्णाङ्क targetfd,
                       अचिन्हित दीर्घ mountflags, अचर वर्ण *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close पूर्णाङ्क mountfd = -1;
  cleanup_close पूर्णाङ्क sysfd = -1;
  पूर्णाङ्क sys_old_जरा_fd = -1;
  पूर्णाङ्क ret;

  यदि (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && म_भेद (fstype, "sysfs") == 0))
    फर्काऊ 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (ret == 0)
    फर्काऊ 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_जरा_fd = get_old_जरा_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  यदि (sys_old_जरा_fd >= 0)
    mountfd = get_bind_mount (sys_old_जरा_fd, "sys", true, true, false, MS_PRIVATE, err);
  यदि (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  यदि (cgroup_mount == रिक्त)
    {
      यदि (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, रिक्त);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "move mount `/sys` to `%s`", real_target);
        }
      अन्यथा
        {
          crun_error_release (err);
          यदि (sys_old_जरा_fd >= 0)
            {
              cleanup_free वर्ण *sys_path = रिक्त;
              proc_fd_path_t sys_जरा_path;

              get_proc_self_fd_path (sys_जरा_path, sys_old_जरा_fd);
              xasprintf (&sys_path, "%s/sys", sys_जरा_path);
              ret = mount (sys_path, real_target, रिक्त, MS_BIND | MS_REC, रिक्त);
            }
          अन्यथा
            ret = mount ("/sys", real_target, रिक्त, MS_BIND | MS_REC, रिक्त);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      यदि (UNLIKELY (sysfd < 0))
        फर्काऊ sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        रिक्त, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      फर्काऊ do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  यदि (UNLIKELY (mountfd < 0))
    फर्काऊ mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  यदि (UNLIKELY (sysfd < 0))
    फर्काऊ sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  फर्काऊ 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
स्थिर शून्य
apply_selinux_xattr (libcrun_container_t *container, पूर्णाङ्क fd, पूर्णाङ्क extra_flags, अचर वर्ण *label)
{
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क procfd;

  यदि ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    फर्काऊ;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  यदि (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close पूर्णाङ्क xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      यदि (xfd >= 0)
        (शून्य) fsetxattr (xfd, "security.selinux", label, म_माप (label), 0);
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

  यदि (UNLIKELY (procfd < 0))
    फर्काऊ procfd;

  {
    cleanup_close पूर्णाङ्क nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    यदि (UNLIKELY (nsfd < 0))
      फर्काऊ crun_make_error (err, त्रुटि_सं, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    यदि (UNLIKELY (ret < 0))
      फर्काऊ crun_make_error (err, त्रुटि_सं, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = खोलो (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (*fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  यदि (UNLIKELY (tmp < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "dup");

  TEMP_FAILURE_RETRY (बंद (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  फर्काऊ 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
स्थिर पूर्णाङ्क
apply_propagation_flags (अचर वर्ण *target, पूर्णाङ्क targetfd, अचर वर्ण *real_target,
                         अचिन्हित दीर्घ mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  पूर्णाङ्क ret;

  यदि (targetfd >= 0)
    {
      libcrun_error_t tmp_err = रिक्त;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      यदि (LIKELY (ret == 0))
        propagation_done = true;
      अन्यथा
        crun_error_release (&tmp_err);
    }

  यदि (! propagation_done)
    {
      ret = mount (रिक्त, real_target, रिक्त, mountflags & ALL_PROPAGATIONS, रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "set propagation for `%s`", target);
    }

  फर्काऊ 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
स्थिर पूर्णाङ्क
schedule_or_do_remount (libcrun_container_t *container, पूर्णाङ्क *fd, पूर्णाङ्क targetfd,
                        अचर वर्ण *target, अचर वर्ण *real_target,
                        bool single_instance, अचिन्हित दीर्घ mountflags,
                        अचर शून्य *data, पूर्णाङ्क extra_flags, libcrun_error_t *err)
{
  अचिन्हित दीर्घ remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  संरचना remount_s *r;

  यदि ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    फर्काऊ do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  यदि (*fd < 0)
    {
      *fd = dup (targetfd);
      यदि (UNLIKELY (*fd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
do_mount (libcrun_container_t *container, अचर वर्ण *source, पूर्णाङ्क targetfd,
          अचर वर्ण *target, अचर वर्ण *fstype, अचिन्हित दीर्घ mountflags, अचर शून्य *data,
          पूर्णाङ्क extra_flags, libcrun_error_t *err)
{
  cleanup_free वर्ण *data_with_label = रिक्त;
  अचर वर्ण *data_without_label = data;
  proc_fd_path_t target_buffer;
  अचर वर्ण *context_type = रिक्त;
  cleanup_close पूर्णाङ्क ms_move_fd = -1;
  अचर वर्ण *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close पूर्णाङ्क fd = -1;
  अचर वर्ण *label = रिक्त;
  पूर्णाङ्क ret = 0;

  यदि (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  अन्यथा
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  यदि (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  यदि ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      यदि (UNLIKELY (context_type == रिक्त))
        फर्काऊ -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      यदि (ret < 0)
        फर्काऊ ret;
      data = data_with_label;
    }

  यदि (mountflags & MS_MOVE)
    {
      यदि ((mountflags & MS_BIND) || fstype)
        फर्काऊ crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, रिक्त, MS_MOVE, रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      यदि (UNLIKELY (ms_move_fd < 0))
        फर्काऊ ms_move_fd;
      targetfd = ms_move_fd;
    }

  यदि ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      अचिन्हित दीर्घ flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      यदि (targetfd >= 0)
        {
          यदि (mountflags & MS_BIND)
            {
              cleanup_close पूर्णाङ्क bindfd = -1;
              libcrun_error_t tmp_err = रिक्त;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              यदि (bindfd < 0)
                {
                  पूर्णाङ्क old_जरा_fd = get_old_जरा_fd (get_private_data (container));

                  यदि (old_जरा_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_जरा_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              यदि (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, रिक्त);
              यदि (ret < 0)
                crun_error_release (&tmp_err);
            }
          अन्यथा
            {
              cleanup_close पूर्णाङ्क newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              यदि (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, रिक्त);
            }
        }

      यदि (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          यदि (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              पूर्णाङ्क old_जरा_fd = get_old_जरा_fd (get_private_data (container));

              यदि (old_जरा_fd >= 0)
                {
                  cleanup_free वर्ण *proc_source_buf = रिक्त;
                  proc_fd_path_t old_जरा_path;

                  get_proc_self_fd_path (old_जरा_path, old_जरा_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_जरा_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      यदि (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क saved_errno = त्रुटि_सं;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          यदि (ret == 0)
            फर्काऊ 0;
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          फर्काऊ diagnose_mount_failure (ret, err, container, fstype);
        }

      यदि (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          यदि (UNLIKELY (fd < 0))
            फर्काऊ fd;

          /* We are replacing the rootfs, reopen it.  */
          यदि (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              यदि (UNLIKELY (ret < 0))
                फर्काऊ ret;
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

  यदि (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  यदि (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  यदि (data && fstype && म_भेद (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  यदि (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  फर्काऊ ret;
}

स्थिर शून्य
try_umount (पूर्णाङ्क targetfd, अचर वर्ण *target)
{
  proc_fd_path_t target_buffer;
  अचर वर्ण *real_target = target;

  यदि (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

स्थिर bool
container_has_cgroupns (libcrun_container_t *container)
{
  फर्काऊ get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

स्थिर पूर्णाङ्क
do_mount_cgroup_v2 (libcrun_container_t *container, पूर्णाङ्क targetfd, अचर वर्ण *target,
                    अचिन्हित दीर्घ mountflags, अचर वर्ण *data, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  पूर्णाङ्क cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    फर्काऊ cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  यदि (UNLIKELY (ret < 0))
    {
      पूर्णाङ्क saved_errno = crun_error_get_errno (err);
      यदि (saved_errno == EPERM || saved_errno == EBUSY)
        {
          अचर वर्ण *unified_cgroup_path;
          अचर वर्ण *src_cgroup;

          crun_error_release (err);

          यदि (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              यदि (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  यदि (LIKELY (ret == 0))
                    फर्काऊ ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_जरा;
          फर्काऊ do_mount (container, src_cgroup, targetfd, target, रिक्त,
                           MS_BIND | mountflags, रिक्त, LABEL_NONE, err);
        }

      फर्काऊ ret;
    }

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, अचर वर्ण *source, पूर्णाङ्क targetfd, अचर वर्ण *target,
                            अचिन्हित दीर्घ mountflags, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क fd = -1;
  अचर वर्ण *subsystem = "systemd";
  cleanup_free वर्ण *subsystem_path = रिक्त;
  cleanup_close पूर्णाङ्क tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  यदि (UNLIKELY (tmpfsdirfd < 0))
    फर्काऊ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "openat `%s`", subsystem_path);

  फर्काऊ do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

स्थिर पूर्णाङ्क
do_mount_cgroup_v1 (libcrun_container_t *container, अचर वर्ण *source, पूर्णाङ्क targetfd, अचर वर्ण *target,
                    अचिन्हित दीर्घ mountflags, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *content = रिक्त;
  वर्ण *from;
  cleanup_close पूर्णाङ्क tmpfsdirfd = -1;
  वर्ण *saveptr = रिक्त;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  यदि (UNLIKELY (tmpfsdirfd < 0))
    फर्काऊ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
    यदि (UNLIKELY (procfd < 0))
      फर्काऊ procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, रिक्त, err);
  }
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (UNLIKELY (content == रिक्त || content[0] == '\0'))
    फर्काऊ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  लागि (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (रिक्त, "\n", &saveptr))
    {
      cleanup_free वर्ण *source_path = रिक्त;
      cleanup_free वर्ण *source_subsystem = रिक्त;
      cleanup_free वर्ण *subsystem_path = रिक्त;
      वर्ण *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close पूर्णाङ्क subsystemfd = -1;
      subsystem = म_अक्षर (from, ':') + 1;
      subpath = म_अक्षर (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      यदि (subsystem[0] == '\0')
        जारी;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = म_माला (subsystem, "name=");
      यदि (it)
        subsystem = it + 5;

      यदि (म_भेद (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      यदि (म_भेद (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_जरा, subsystem, रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      /* if there is already a mount specified, do not add a default one.  */
      यदि (find_mount_for (container, source_subsystem) != रिक्त)
        जारी;

      ret = append_paths (&source_path, err, source_subsystem, subpath, रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      यदि (UNLIKELY (subsystemfd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", subsystem_path);

      यदि (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          यदि (UNLIKELY (ret < 0))
            {
              यदि (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  जारी;
                }
              फर्काऊ ret;
            }
        }
      अन्यथा
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, रिक्त, MS_BIND | mountflags, रिक्त,
                          LABEL_NONE, err);
          यदि (UNLIKELY (ret < 0))
            {
              यदि (crun_error_get_errno (err) != ENOENT)
                फर्काऊ ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, रिक्त, MS_BIND | mountflags,
                              रिक्त, LABEL_NONE, err);
              यदि (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  यदि (crun_error_get_errno (err) != ENOENT)
                    फर्काऊ ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
do_mount_cgroup (libcrun_container_t *container, अचर वर्ण *source, पूर्णाङ्क targetfd, अचर वर्ण *target,
                 अचिन्हित दीर्घ mountflags, अचर वर्ण *data, libcrun_error_t *err)
{
  पूर्णाङ्क cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    फर्काऊ cgroup_mode;

  विकल्प (cgroup_mode)
    {
    पक्ष CGROUP_MODE_UNIFIED:
      फर्काऊ do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    पक्ष CGROUP_MODE_LEGACY:
    पक्ष CGROUP_MODE_HYBRID:
      फर्काऊ do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  फर्काऊ crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
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
  माप_प्रकार i;

  यदि (private_data->needed_devs_fds)
    {
      संरचना libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      लागि (i = 0; needed_devs[i].path; i++)
        {
          यदि (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            जारी;

          पूर्णाङ्क fd = syscall_open_रूख (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          यदि (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          अन्यथा यदि (fd >= 0)
            बंद (fd);
        }
    }

  यदि (private_data->notify_socket_रूख_fd < 0
      && private_data->host_notify_socket_path)
    {
      पूर्णाङ्क fd = syscall_open_रूख (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      यदि (fd >= 0)
        private_data->notify_socket_रूख_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
स्थिर अचर वर्ण *
relative_path_under_dev (अचर वर्ण *path)
{
  यदि (path[0] != '/')
    फर्काऊ path;
  यदि (म_भेदन (path, "/dev/", 5) == 0)
    {
      यदि (म_अक्षर (path + 5, '/'))
        फर्काऊ रिक्त;
      फर्काऊ path + 5;
    }
  फर्काऊ रिक्त;
}

स्थिर पूर्णाङ्क
mknod_and_set_attrs (libcrun_container_t *container, पूर्णाङ्क dirfd, अचर वर्ण *name, संरचना device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  proc_fd_path_t fd_path;
  अचर वर्ण *what = "new";
  bool exists = false;
  संरचना stat st;
  पूर्णाङ्क procfd;
  पूर्णाङ्क ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (त्रुटि_सं != EEXIST)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open %s device `%s`", what, device->path);

  ret = ख_स्थिति (fd, &st);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat %s device `%s`", what, device->path);

  यदि (UNLIKELY ((st.st_mode & S_IFMT) != type))
    फर्काऊ crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  यदि (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    फर्काऊ crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  यदि (exists)
    फर्काऊ 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  यदि (UNLIKELY (procfd < 0))
    फर्काऊ procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fchown `%s`", device->path);

  फर्काऊ 1;
}

पूर्णाङ्क
libcrun_create_dev (libcrun_container_t *container, पूर्णाङ्क devfd, पूर्णाङ्क srcfd,
                    संरचना device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  अचर वर्ण *fullname = device->path;
  अचर वर्ण *rootfs = get_private_data (container)->rootfs;
  यदि (is_empty_string (fullname))
    फर्काऊ crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free वर्ण *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  यदि (normalized_path[0] == '\0')
    म_नकल (normalized_path, "/");
  अचर वर्ण *rel_dev = relative_path_under_dev (normalized_path);

  यदि (binds)
    {
      cleanup_close पूर्णाङ्क fd = -1;

      यदि (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          यदि (UNLIKELY (fd < 0))
            {
              यदि (त्रुटि_सं == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              यदि (UNLIKELY (fd < 0))
                फर्काऊ crun_make_error (err, त्रुटि_सं, "openat `%s`", device->path);
            }
        }
      अन्यथा
        {
          अचर वर्ण *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          यदि (UNLIKELY (fd < 0))
            फर्काऊ fd;
        }

      यदि (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          यदि (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              यदि (LIKELY (ret >= 0))
                फर्काऊ 0;
            }
          अन्यथा
            crun_error_release (err);
        }

      {
        cleanup_close पूर्णाङ्क mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        यदि (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            यदि (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, रिक्त);
                यदि (LIKELY (ret == 0))
                  फर्काऊ 0;
              }
            अन्यथा
              crun_error_release (err);
          }
        अन्यथा
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, रिक्त, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, रिक्त, LABEL_MOUNT, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  अन्यथा
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      यदि (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          यदि (ret <= 0)
            फर्काऊ ret;
        }
      अन्यथा
        {
          cleanup_close पूर्णाङ्क dirfd = -1;
          cleanup_free वर्ण *स_नाम = रिक्त;
          वर्ण *basename, *found;

          स_नाम = xstrdup (normalized_path);

          found = म_खोजप (स_नाम, '/');
          यदि (found)
            *found = '\0';

          basename = found ? found + 1 : स_नाम;

          यदि (स_नाम[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              यदि (UNLIKELY (dirfd < 0))
                फर्काऊ crun_make_error (err, त्रुटि_सं, "dup fd for `%s`", rootfs);
            }
          अन्यथा
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, स_नाम, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              यदि (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, स_नाम, 0755, err);
                }
              यदि (UNLIKELY (dirfd < 0))
                फर्काऊ dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          यदि (ret <= 0)
            फर्काऊ ret;
        }
    }
  फर्काऊ 0;
}

संरचना symlink_s
{
  अचर वर्ण *path;
  अचर वर्ण *target;
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
  माप_प्रकार i;
  संरचना device_s *it;
  cleanup_close पूर्णाङ्क devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  अचर वर्ण *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map संरचना libcrun_fd_map *dev_fds = रिक्त;
  cleanup_close_map संरचना libcrun_fd_map *needed_fds = रिक्त;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = रिक्त;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = रिक्त;

  यदि (! def || ! def->linux)
    फर्काऊ 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  यदि (UNLIKELY (devfd < 0))
    फर्काऊ devfd;

  लागि (i = 0; i < def->linux->devices_len; i++)
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

      यदि (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  लागि (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close पूर्णाङ्क srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
      यदि (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  लागि (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      यदि (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क saved_errno = त्रुटि_सं;

          यदि (त्रुटि_सं == EEXIST && ! symlinks[i].force)
            जारी;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          यदि (त्रुटि_सं == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              यदि (ret < 0 && त्रुटि_सं == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              यदि (ret < 0 && त्रुटि_सं == EBUSY)
                {
                  cleanup_close पूर्णाङ्क tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  यदि (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      यदि (umount2 (procpath, MNT_DETACH) == 0)
                        गच्छ retry_unlink;
                    }
                }
              यदि (ret == 0)
                गच्छ retry_symlink;
            }
          फर्काऊ crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  यदि (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  फर्काऊ 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
स्थिर अचर वर्ण *
next_path_component (अचर वर्ण *p, माप_प्रकार *len)
{
  लागि (;;)
    {
      p = consume_slashes (p);
      *len = म_खोज (p, "/");
      यदि (*len != 1 || p[0] != '.')
        फर्काऊ p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
स्थिर bool
same_path (अचर वर्ण *a, अचर वर्ण *b)
{
  माप_प्रकार la, lb;

  लागि (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      यदि (la != lb || स_भेद (a, b, la) != 0)
        फर्काऊ false;
      यदि (la == 0)
        फर्काऊ true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
स्थिर bool
is_duplicate_path (वर्ण **paths, माप_प्रकार i)
{
  माप_प्रकार j;

  यदि (paths[i] == रिक्त)
    फर्काऊ false;

  लागि (j = 0; j < i; j++)
    यदि (paths[j] && same_path (paths[j], paths[i]))
      फर्काऊ true;

  फर्काऊ false;
}

स्थिर पूर्णाङ्क
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  माप_प्रकार i;
  पूर्णाङ्क ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  यदि (! def || ! def->linux)
    फर्काऊ 0;

  लागि (i = 0; i < def->linux->masked_paths_len; i++)
    {
      यदि (is_duplicate_path (def->linux->masked_paths, i))
        जारी;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  लागि (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
do_pivot (libcrun_container_t *container, अचर वर्ण *rootfs, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क oldrootfd = -1;
  cleanup_close पूर्णाङ्क newrootfd = -1;
  पूर्णाङ्क ret;

  oldrootfd = खोलो ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (oldrootfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `/`");

  newrootfd = खोलो (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (newrootfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fchdir `%s`", rootfs);

  ret = pivot_जरा (".", ".");
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "pivot_root");

  ret = fchdir (oldrootfd);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fchdir `%s`", rootfs);

  ret = do_mount (container, रिक्त, -1, ".", रिक्त, MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = umount2 (".", MNT_DETACH);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "umount2 oldroot");

  गर
    {
      ret = umount2 (".", MNT_DETACH);
      यदि (ret < 0 && त्रुटि_सं == EINVAL)
        रोक;
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "umount2 oldroot");
  } जबसम्म (ret == 0);

  ret = स_बदलो ("/");
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "chdir to newroot");

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, वर्ण **data, libcrun_error_t *err)
{
  अचर वर्ण *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close पूर्णाङ्क fd = -1;
  संरचना stat st;
  पूर्णाङ्क ret;

  यदि (*data != रिक्त && म_माला (*data, "mode="))
    फर्काऊ 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  यदि (fd < 0)
    {
      यदि (crun_error_get_errno (err) != ENOENT)
        फर्काऊ fd;

      crun_error_release (err);
      फर्काऊ 0;
    }
  ret = ख_स्थिति (fd, &st);
  यदि (ret < 0)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
get_default_flags (libcrun_container_t *container, अचर वर्ण *destination, वर्ण **data)
{
  यदि (म_भेद (destination, "/proc") == 0)
    फर्काऊ 0;
  यदि (म_भेद (destination, "/dev/cgroup") == 0 || म_भेद (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      फर्काऊ MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  यदि (म_भेद (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      फर्काऊ MS_NOEXEC | MS_STRICTATIME;
    }
  यदि (म_भेद (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      फर्काऊ MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  यदि (म_भेद (destination, "/dev/mqueue") == 0)
    फर्काऊ MS_NOEXEC | MS_NOSUID | MS_NODEV;
  यदि (म_भेद (destination, "/dev/pts") == 0)
    {
      यदि (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      अन्यथा
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      फर्काऊ MS_NOEXEC | MS_NOSUID;
    }
  यदि (म_भेद (destination, "/sys") == 0)
    फर्काऊ MS_NOEXEC | MS_NOSUID | MS_NODEV;

  फर्काऊ 0;
}

स्थिर अचर वर्ण *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  फर्काऊ find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

स्थिर पूर्णाङ्क
check_valid_no_follow_file_system (पूर्णाङ्क fd, अचर वर्ण *destination, libcrun_error_t *err)
{
  संरचना statfs sfs;

  पूर्णाङ्क ret = fstatfs (fd, &sfs);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "statfs `%s`", destination);

  विकल्प (sfs.f_type)
    {
    पक्ष PROC_SUPER_MAGIC:
    पक्ष SYSFS_MAGIC:
    पक्ष DEVPTS_SUPER_MAGIC:
      फर्काऊ crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
safe_create_symlink (पूर्णाङ्क rootfsfd, अचर वर्ण *rootfs, अचर वर्ण *target, अचर वर्ण *destination, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क parent_dir_fd = -1;
  cleanup_free वर्ण *buffer = रिक्त;
  वर्ण *part;
  पूर्णाङ्क ret;

  यदि (is_empty_string (destination))
    फर्काऊ crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = स_नाम (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  यदि (UNLIKELY (parent_dir_fd < 0))
    फर्काऊ parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  म_नकल (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  यदि (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      यदि (त्रुटि_सं == EEXIST)
        {
          cleanup_free वर्ण *link = रिक्त;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          यदि (UNLIKELY (len < 0))
            फर्काऊ len;

          यदि ((((माप_प्रकार) len) == म_माप (target)) && म_भेदन (link, target, len) == 0)
            फर्काऊ 0;

          फर्काऊ crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      फर्काऊ crun_make_error (err, त्रुटि_सं, "symlinkat `%s`", target);
    }

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
handle_copy_symlink (libcrun_container_t *container, अचर वर्ण *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     अचर वर्ण *link_target, libcrun_error_t *err)
{
  cleanup_free वर्ण *target = रिक्त;

  यदि (link_target == रिक्त)
    {
      पूर्णाङ्क old_जरा_fd = get_old_जरा_fd (get_private_data (container));
      अचर वर्ण *source = mount->source;
      ssize_t len;

      यदि (old_जरा_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_जरा_fd, source + 1, &target, 0, err);
      अन्यथा
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      यदि (UNLIKELY (len < 0))
        फर्काऊ len;
      link_target = target;
    }

  फर्काऊ safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

स्थिर पूर्णाङ्क
prepare_sysfs_or_proc_mount (libcrun_container_t *container, अचर वर्ण *target,
                             पूर्णाङ्क *targetfd, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (त्रुटि_सं == ENOENT)
        {
          यदि (म_अक्षर (target, '/'))
            फर्काऊ crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      अन्यथा यदि (त्रुटि_सं == ENOTDIR)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "the target `/%s` is invalid", target);

      यदि (ret < 0)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", target);
    }

  *targetfd = ret;
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
handle_tmpcopyup (libcrun_container_t *container, अचर वर्ण *rootfs, अचर वर्ण *target,
                  पूर्णाङ्क copy_from_fd, libcrun_error_t *err)
{
  पूर्णाङ्क destfd, ret;
  cleanup_close पूर्णाङ्क tmpfd = copy_from_fd;

  target = consume_slashes (target);

  यदि (is_empty_string (target))
    फर्काऊ crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  यदि (UNLIKELY (destfd < 0))
    फर्काऊ crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  फर्काऊ ret;
}

स्थिर पूर्णाङ्क
get_mount_label_how (अचर वर्ण *type, bool is_sysfs_or_proc)
{
  यदि (is_sysfs_or_proc)
    फर्काऊ LABEL_NONE;

  यदि (म_भेद (type, "mqueue") == 0)
    फर्काऊ LABEL_XATTR;

  फर्काऊ LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
स्थिर अचिन्हित दीर्घ
get_bind_mount_locked_flags (पूर्णाङ्क source_mountfd, अचर वर्ण *source, runtime_spec_schema_defs_mount *mount)
{
  अचिन्हित दीर्घ cleared = 0;
  संरचना statfs sfs;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  यदि (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  अन्यथा
    ret = statfs (source, &sfs);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ 0;

  लागि (i = 0; i < mount->options_len; i++)
    {
      अचर संरचना propagation_flags_s *prop;

      यदि (mount->options[i] == रिक्त)
        जारी;

      prop = libcrun_str2mount_flags (mount->options[i]);
      यदि (prop == रिक्त || (prop->extra_flags & OPTION_RECURSIVE))
        जारी;

      यदि (prop->clear)
        cleared |= prop->flags;
      अन्यथा
        cleared &= ~prop->flags;
    }

  फर्काऊ sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
स्थिर अचर वर्ण *
get_path_in_rootfs (अचर वर्ण *rootfs, अचर वर्ण *path)
{
  माप_प्रकार len;

  यदि (rootfs == रिक्त || path == रिक्त)
    फर्काऊ रिक्त;

  len = म_माप (rootfs);
  जबसम्म (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  यदि (len == 0)
    फर्काऊ रिक्त;

  यदि (म_भेदन (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    फर्काऊ रिक्त;

  फर्काऊ consume_slashes (path + len);
}

स्थिर पूर्णाङ्क
process_single_mount (libcrun_container_t *container, अचर वर्ण *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      संरचना libcrun_fd_map *mount_fds, माप_प्रकार mount_index,
                      अचर वर्ण *systemd_cgroup_v1, libcrun_error_t *err)
{
  अचर वर्ण *target = consume_slashes (mount->destination);
  cleanup_close पूर्णाङ्क source_mountfd = -1;
  cleanup_free वर्ण *data = रिक्त;
  वर्ण *type;
  वर्ण *source;
  अचिन्हित दीर्घ flags = 0;
  अचिन्हित दीर्घ extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close पूर्णाङ्क copy_from_fd = -1;
  cleanup_close पूर्णाङ्क targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  पूर्णाङ्क no_remount = 0;
  पूर्णाङ्क ret;

  यदि (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  यदि (mount->options == रिक्त)
    flags = get_default_flags (container, mount->destination, &data);
  अन्यथा
    {
      माप_प्रकार j;

      लागि (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  यदि (type == रिक्त && (flags & MS_BIND) == 0)
    फर्काऊ crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  यदि ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      अचर वर्ण *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      यदि (rel_source)
        {
          अचिन्हित दीर्घ propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          यदि (UNLIKELY (source_mountfd < 0))
            फर्काऊ crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  यदि ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  यदि (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      यदि (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      यदि (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = म_भेद (type, "sysfs") == 0 || म_भेद (type, "proc") == 0;

  यदि (म_भेद (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  यदि (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      अचर वर्ण *path = mount->source;

      यदि ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        फर्काऊ crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      यदि (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, रिक्त);
        }
      अन्यथा यदि (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      अन्यथा यदि (path[0] == '/')
        {
          पूर्णाङ्क old_जरा_fd = get_old_जरा_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          यदि (old_जरा_fd >= 0)
            ret = get_file_type_at (old_जरा_fd, &src_mode, nofollow, (path[1] == '\0') ? रिक्त : path + 1);
          अन्यथा
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      अन्यथा
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot stat `%s`", path);

      यदि (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          पूर्णाङ्क bind_dirfd = AT_FDCWD;
          अचर वर्ण *bind_source = mount->source;
          पूर्णाङ्क old_जरा_fd = get_old_जरा_fd (get_private_data (container));

          यदि (old_जरा_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_जरा_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          source_mountfd = ret;
        }
    }

  यदि (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      अचर वर्ण *cached_target = रिक्त;

      यदि (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      mounted = true;
    }
  अन्यथा यदि (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  अन्यथा
    {
      bool is_dir = S_ISDIR (src_mode);

      यदि (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
      अन्यथा
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
          targetfd = ret;
        }
    }

  यदि (extra_flags & OPTION_TMPCOPYUP)
    {
      यदि (म_भेद (type, "tmpfs") != 0)
        फर्काऊ crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      यदि (UNLIKELY (copy_from_fd < 0))
        {
          यदि (त्रुटि_सं != ENOTDIR)
            फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  यदि (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, रिक्त);
      यदि (LIKELY (ret == 0))
        {
          अचिन्हित दीर्घ remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          यदि (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, रिक्त, source_mountfd, target, रिक्त, remaining_flags, data, LABEL_NONE | no_remount, err);
              यदि (UNLIKELY (ret < 0))
                फर्काऊ ret;
            }

          mounted = true;

          यदि (is_empty_string (target))
            {
              पूर्णाङ्क new_rootfsfd = खोलो (rootfs, O_PATH | O_CLOEXEC);
              यदि (UNLIKELY (new_rootfsfd < 0))
                फर्काऊ crun_make_error (err, त्रुटि_सं, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (बंद (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      अन्यथा यदि (get_private_data (container)->delayed_pidns_fork && म_भेद (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          फर्काऊ crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      अन्यथा यदि (get_private_data (container)->host_जरा_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          फर्काऊ crun_make_error (err, त्रुटि_सं, "move_mount `%s` to `%s`", source, target);
        }
      अन्यथा यदि (त्रुटि_सं != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, म_त्रुटि (त्रुटि_सं));
        }
    }

  यदि (! mounted)
    {
      यदि (systemd_cgroup_v1 && म_भेद (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
      अन्यथा यदि (म_भेद (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
      अन्यथा
        {
          पूर्णाङ्क extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
    }

  यदि (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  यदि (rec_clear || rec_set)
    {
      अचर bool is_dir = S_ISDIR (src_mode);
      cleanup_close पूर्णाङ्क dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      यदि (UNLIKELY (dfd < 0))
        फर्काऊ crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
do_mounts (libcrun_container_t *container, अचर वर्ण *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  अचर वर्ण *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map संरचना libcrun_fd_map *mount_fds = रिक्त;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = रिक्त;

  लागि (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  फर्काऊ 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
पूर्णाङ्क
libcrun_container_do_bind_mount (libcrun_container_t *container, वर्ण *mount_source, वर्ण *mount_destination, वर्ण **mount_options, माप_प्रकार mount_options_len, libcrun_error_t *err)
{
  अचर वर्ण *target = consume_slashes (mount_destination);
  cleanup_free वर्ण *data = रिक्त;
  अचिन्हित दीर्घ flags = 0;
  अचिन्हित दीर्घ extra_flags = 0;
  cleanup_close पूर्णाङ्क targetfd = -1;
  पूर्णाङ्क is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  अचर वर्ण *rootfs = get_private_data (container)->rootfs;
  पूर्णाङ्क ret;

  यदि ((get_private_data (container)->rootfsfd < 0) || (rootfs == रिक्त))
    फर्काऊ crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  यदि (mount_options == रिक्त)
    flags = get_default_flags (container, mount_destination, &data);
  अन्यथा
    {
      माप_प्रकार j;

      लागि (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  यदि (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  यदि (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      यदि (UNLIKELY (is_dir < 0))
        फर्काऊ is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ 0;
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

  यदि (container && get_private_data (container)->host_notify_socket_path)
    {
      अचर वर्ण *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  यदि (host_path == रिक्त)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_जरा, context->id, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  यदि (UNLIKELY (notify_fd < 0))
    फर्काऊ notify_fd;

  यदि (UNLIKELY (chmod (host_path, 0777) < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  यदि (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (शून्य) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       म_माप (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  फर्काऊ 1;
#अन्यथा
  (शून्य) context;
  (शून्य) container;
  (शून्य) err;
  *notify_socket_out = -1;
  फर्काऊ 0;
#endif
}

#ifdef HAVE_SYSTEMD
स्थिर पूर्णाङ्क
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  अचर वर्ण *notify_socket = container->context->notify_socket;
  cleanup_free वर्ण *host_notify_socket_path = रिक्त;
  cleanup_free वर्ण *container_notify_socket_path = रिक्त;
  cleanup_free वर्ण *state_dir = रिक्त;
  uid_t container_जरा_uid = -1;
  gid_t container_जरा_gid = -1;
  पूर्णाङ्क notify_socket_रूख_fd;

  यदि (notify_socket == रिक्त)
    फर्काऊ 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_जरा : रिक्त),
                                     (container->context ? container->context->id : रिक्त), err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = सूची_गढ़ो (host_notify_socket_path, 0700);
  यदि (ret < 0)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "mkdir `%s`", host_notify_socket_path);

  यदि (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_जरा_in_the_userns (container->container_def, 0, 0, &container_जरा_uid, &container_जरा_gid);
      यदि (container_जरा_uid != ((uid_t) -1) && container_जरा_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_जरा_uid, container_जरा_gid);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "chown `%s` to `%d:%d`", host_notify_socket_path, container_जरा_uid, container_जरा_gid);
        }
    }

  notify_socket_रूख_fd = syscall_open_रूख (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  यदि (notify_socket_रूख_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_रूख_fd = notify_socket_रूख_fd;
  अन्यथा यदि (त्रुटि_सं == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  अन्यथा यदि (त्रुटि_सं == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  अन्यथा
    /* some other error */
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = रिक्त;
  फर्काऊ 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
स्थिर पूर्णाङ्क
do_finalize_notify_socket (libcrun_container_t *container, अचर वर्ण *rootfs, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *host_notify_socket_path = रिक्त;
  cleanup_free वर्ण *container_notify_socket_path = रिक्त;
  cleanup_free वर्ण *container_notify_socket_path_dir_alloc = रिक्त;
  अचर वर्ण *container_notify_socket_path_dir = रिक्त;
  cleanup_close पूर्णाङ्क notify_socket_रूख_fd = -1;
  cleanup_close पूर्णाङ्क targetfd = -1;
  पूर्णाङ्क did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = रिक्त;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = रिक्त;

  यदि (host_notify_socket_path == रिक्त || container_notify_socket_path == रिक्त)
    फर्काऊ 0;

  यदि (rootfs == रिक्त)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (स_नाम (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  यदि (get_private_data (container)->rootfsfd < 0)
    {
      ret = खोलो (rootfs, O_PATH | O_CLOEXEC);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  targetfd = ret;

  notify_socket_रूख_fd = get_private_data (container)->notify_socket_रूख_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_रूख_fd = -1;

  यदि (notify_socket_रूख_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_रूख_fd, targetfd, रिक्त);
      यदि (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          यदि (UNLIKELY (targetfd < 0))
            फर्काऊ targetfd;

          ret = do_mount (container, रिक्त, targetfd, container_notify_socket_path_dir, रिक्त,
                          MS_REC | MS_PRIVATE, रिक्त, LABEL_NONE, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
      अन्यथा यदि (त्रुटि_सं == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      अन्यथा
        फर्काऊ crun_make_error (err, त्रुटि_सं, "move_mount `%d` to `%s`", notify_socket_रूख_fd,
                                container_notify_socket_path_dir);
    }

  यदि (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, रिक्त,
                      MS_BIND | MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
make_parent_mount_private (अचर वर्ण *rootfs, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क rootfsfd = -1;
  संरचना stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = खोलो (rootfs, O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (rootfsfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

  यदि (UNLIKELY (ख_स्थिति (rootfsfd, &prev_st) < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat `%s`", rootfs);

  लागि (;;)
    {
      संरचना stat cur_st;
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क ret;
      cleanup_close पूर्णाङ्क parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      यदि (ret == 0)
        फर्काऊ 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (रिक्त, proc_path, रिक्त, MS_PRIVATE, रिक्त);
      यदि (ret == 0)
        फर्काऊ 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      यदि (parentfd < 0)
        {
          पूर्णाङ्क saved_errno = त्रुटि_सं;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          यदि (ret != 0)
            फर्काऊ crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          फर्काऊ crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      यदि (UNLIKELY (ख_स्थिति (parentfd, &cur_st) < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      यदि (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        रोक;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  फर्काऊ crun_make_error (err, 0, "make `%s` private", rootfs);
}

पूर्णाङ्क
libcrun_set_mounts (संरचना container_entrypoint_s *entrypoint_args, libcrun_container_t *container, अचर वर्ण *rootfs, set_mounts_cb_t cb, शून्य *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क is_user_ns = 0;
  पूर्णाङ्क ret = 0;

  यदि (rootfs == रिक्त || def->mounts == रिक्त)
    फर्काऊ 0;

  ret = खोलो (rootfs, O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  यदि (def->जरा->readonly)
    {
      संरचना remount_s *r;
      अचिन्हित दीर्घ remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      पूर्णाङ्क fd;

      fd = dup (get_private_data (container)->rootfsfd);
      यदि (UNLIKELY (fd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, रिक्त, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  यदि (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      यदि (UNLIKELY (is_user_ns < 0))
        फर्काऊ is_user_ns;
    }

  यदि (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  यदि (cb)
    {
      ret = cb (cb_data, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  यदि (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = रिक्त;
      अचर वर्ण *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (शून्य) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
umount_or_hide (अचर वर्ण *target, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = umount2 (target, MNT_DETACH);
  यदि (UNLIKELY (ret < 0))
    {
      पूर्णाङ्क saved_errno = त्रुटि_सं;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      यदि (त्रुटि_सं == EINVAL)
        {
          ret = mount (रिक्त, target, "tmpfs", 0, "size=0k");
          यदि (LIKELY (ret == 0))
            फर्काऊ 0;
        }

      फर्काऊ crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  फर्काऊ ret;
}

स्थिर पूर्णाङ्क
move_जरा (अचर वर्ण *rootfs, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = स_बदलो (rootfs);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = umount_or_hide ("/proc", err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "mount MS_MOVE to `/`");

  ret = chroot (".");
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "chroot to `%s`", rootfs);

  ret = स_बदलो ("/");
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "chdir to `%s`", rootfs);

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_finalize_mounts (संरचना container_entrypoint_s *entrypoint_args, libcrun_container_t *container, अचर वर्ण *rootfs,
                         set_mounts_cb_t after_pivot_cb, शून्य *cb_data, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_जरा_fd));

  यदि (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      यदि (get_private_data (container)->no_pivot)
        {
          ret = move_जरा (rootfs, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
      अन्यथा
        {
          ret = do_pivot (container, rootfs, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }

      ret = do_mount (container, रिक्त, -1, "/", रिक्त,
                      get_private_data (container)->rootfs_propagation,
                      रिक्त, LABEL_MOUNT, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = स_बदलो ("/");
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "chdir to `/`");
    }

  यदि (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  ret = finalize_mounts (container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
maybe_open_रूख_namespace (अचर वर्ण *rootfs, पूर्णाङ्क *out_fd, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क rootfs_fd = -1;
  पूर्णाङ्क रूख_fd;

  *out_fd = -1;

  rootfs_fd = खोलो (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (rootfs_fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

  रूख_fd = syscall_open_रूख (rootfs_fd, "",
                               OPEN_रूख_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  यदि (रूख_fd < 0)
    {
      यदि (त्रुटि_सं == EINVAL || त्रुटि_सं == ENOSYS || त्रुटि_सं == EPERM)
        फर्काऊ 0;
      फर्काऊ crun_make_error (err, त्रुटि_सं, "open_tree `%s`", rootfs);
    }

  *out_fd = रूख_fd;
  फर्काऊ 0;
}

स्थिर संरचना libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  संरचना libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  यदि (mount_fds == रिक्त)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  फर्काऊ mount_fds;
}

स्थिर पूर्णाङ्क
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free वर्ण *mnt_data = रिक्त;
  अचिन्हित दीर्घ mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  अचर वर्ण *label_type = रिक्त;
  अचर वर्ण *label_val = रिक्त;
  अचर वर्ण *fstype;
  पूर्णाङ्क mnt_fd;
  माप_प्रकार j;

  यदि (mount->type == रिक्त)
    फर्काऊ -1;

  fstype = mount->type;

  लागि (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  यदि (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = म_भेद (fstype, "sysfs") == 0
                              || म_भेद (fstype, "proc") == 0;
      पूर्णाङ्क extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      यदि (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          यदि (label_type == रिक्त)
            crun_error_release (err);
          अन्यथा
            label_val = def->linux->mount_label;
        }
    }

  यदि (म_भेद (fstype, "cgroup") == 0)
    {
      पूर्णाङ्क cgroup_mode = libcrun_get_cgroup_mode (err);
      यदि (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  यदि (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, रिक्त, रिक्त, mnt_data);

  फर्काऊ mnt_fd;
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
syscall_listmount (संरचना mnt_id_req_s *req, uint64_t *ids, माप_प्रकार nr, अचिन्हित पूर्णाङ्क flags)
{
#ifdef __NR_listmount
  फर्काऊ (पूर्णाङ्क) syscall (__NR_listmount, req, ids, nr, flags);
#अन्यथा
  (शून्य) req;
  (शून्य) ids;
  (शून्य) nr;
  (शून्य) flags;
  त्रुटि_सं = ENOSYS;
  फर्काऊ -1;
#endif
}

स्थिर पूर्णाङ्क
syscall_statmount (संरचना mnt_id_req_s *req, संरचना statmount_s *buf, माप_प्रकार size, अचिन्हित पूर्णाङ्क flags)
{
#ifdef __NR_statmount
  फर्काऊ (पूर्णाङ्क) syscall (__NR_statmount, req, buf, size, flags);
#अन्यथा
  (शून्य) req;
  (शून्य) buf;
  (शून्य) size;
  (शून्य) flags;
  त्रुटि_सं = ENOSYS;
  फर्काऊ -1;
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
  माप_प्रकार i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  लागि (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      यदि (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != रिक्त && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          रोक;
        }
    }
  यदि (! has_absolute_bind)
    फर्काऊ false;

  ids = xmalloc (capacity * परिमाण (uint64_t));
  लागि (;;)
    {
      संरचना mnt_id_req_s req = {
        .size = परिमाण (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      पूर्णाङ्क ret;

      यदि (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * परिमाण (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      यदि (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        फर्काऊ true;
      यदि (ret == 0)
        रोक;

      n_ids += (माप_प्रकार) ret;
      last = ids[n_ids - 1];
    }

  लागि (i = 0; i < n_ids; i++)
    {
      वर्ण buffer[4096] __attribute__ ((aligned (8)));
      संरचना statmount_s *sm = (संरचना statmount_s *) buffer;
      संरचना mnt_id_req_s req = {
        .size = परिमाण (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      अचर वर्ण *जरा, *point;
      माप_प्रकार j;

      यदि (syscall_statmount (&req, sm, परिमाण (buffer), 0) < 0)
        जारी;
      यदि ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        जारी;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      जरा = sm->str + sm->mnt_root;
      यदि (! has_prefix (जरा, "mnt:["))
        जारी;

      point = sm->str + sm->mnt_point;

      लागि (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          अचर वर्ण *source = def->mounts[j]->source;
          माप_प्रकार source_len;

          यदि (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            जारी;
          यदि (source == रिक्त || source[0] != '/')
            जारी;

          source_len = म_माप (source);
          जबसम्म (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          यदि (म_भेदन (point, source, source_len) != 0)
            जारी;

          /* The source itself is a pinned mount namespace.  */
          यदि (point[source_len] == '\0')
            फर्काऊ true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          यदि (recursive && (source_len == 1 || point[source_len] == '/'))
            फर्काऊ true;
        }
    }

  फर्काऊ false;
}

स्थिर bool
can_use_open_रूख_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  संरचना libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  माप_प्रकार i;

  यदि (has_hooks || has_userns)
    फर्काऊ false;

  यदि (mounts_hit_pinned_mount_namespace (def))
    फर्काऊ false;

  यदि (container->context->no_open_रूख_namespace)
    फर्काऊ false;

  {
    libcrun_error_t tmp_err = रिक्त;
    पूर्णाङ्क in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    यदि (in_userns != 0)
      फर्काऊ false;
  }

  mount_fds = get_fd_map (container);
  लागि (i = 0; i < def->mounts_len; i++)
    {
      यदि (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          यदि (! has_cached_target)
            फर्काऊ false;
        }
    }

  फर्काऊ true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
स्थिर bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  माप_प्रकार i;

  यदि (mount->type == रिक्त || म_भेद (mount->type, "tmpfs") != 0)
    फर्काऊ false;

  लागि (i = 0; i < mount->options_len; i++)
    यदि (mount->options[i] != रिक्त && has_prefix (mount->options[i], "mode="))
      फर्काऊ false;

  फर्काऊ true;
}

स्थिर पूर्णाङ्क
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, वर्ण **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  अचिन्हित दीर्घ rootfs_propagation = 0;
  cleanup_close पूर्णाङ्क rootfsfd = -1;
  cleanup_close पूर्णाङ्क रूख_fd = -1;
  libcrun_error_t tmp_err = रिक्त;
  bool use_open_रूख = false;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  यदि (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, रिक्त, रिक्त, रिक्त, रिक्त);

  यदि ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  मुक्त (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  लागि (i = 0; i < def->mounts_len; i++)
    {
      संरचना libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close पूर्णाङ्क mnt_fd = -1;
      bool recursive_bind, nofollow;
      अचिन्हित दीर्घ mnt_flags = 0;

      यदि (mount_fds && mount_fds->fds[i] >= 0)
        जारी;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      यदि (def->mounts[i]->source != रिक्त
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free वर्ण *mnt_data = रिक्त;
          अचिन्हित दीर्घ mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          माप_प्रकार j;

          लागि (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          यदि (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free वर्ण *target = रिक्त;
              libcrun_error_t tmp_err = रिक्त;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              यदि (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              यदि (get_private_data (container)->copy_symlink_targets == रिक्त)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * परिमाण (वर्ण *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = रिक्त;
            }
        }

      यदि (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          यदि (mount_fds && def->mounts[i]->source != रिक्त && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == रिक्त)
            {
              libcrun_error_t tmp_err = रिक्त;
              अचिन्हित दीर्घ propagation = MS_PRIVATE;

              यदि (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              यदि (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          जारी;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      यदि (mnt_fd < 0)
        {
          crun_error_release (err);
          जारी;
        }

      यदि (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close पूर्णाङ्क destfd = -1;
          संरचना stat st;

          यदि (rootfsfd < 0)
            {
              rootfsfd = खोलो (*rootfs, O_PATH | O_CLOEXEC);
              यदि (UNLIKELY (rootfsfd < 0))
                फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          यदि (destfd < 0)
            crun_error_release (&tmp_err);
          अन्यथा यदि (ख_स्थिति (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
              यदि (UNLIKELY (procfd < 0))
                फर्काऊ procfd;

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
  यदि (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  यदि (! get_private_data (container)->joined_mount_ns
      && can_use_open_रूख_namespace (container))
    use_open_रूख = true;

  यदि (use_open_रूख)
    {
      ret = maybe_open_रूख_namespace (*rootfs, &रूख_fd, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  यदि (रूख_fd >= 0)
    preopen_needed_devs_and_notify (container);

  यदि (रूख_fd >= 0)
    {
      ret = setns (रूख_fd, CLONE_NEWNS);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "setns `CLONE_NEWNS`");

      ret = mount (रिक्त, "/", रिक्त, MS_REMOUNT | MS_BIND, रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "remount `/`");

      ret = do_mount (container, रिक्त, -1, "/", रिक्त, MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = do_mount (container, रिक्त, -1, "/", रिक्त, rootfs_propagation, रिक्त, LABEL_MOUNT, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_जरा_switched = true;
      मुक्त (*rootfs);
      *rootfs = xstrdup ("/");
    }
  अन्यथा
    {
      यदि (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, रिक्त, -1, "/", रिक्त, rootfs_propagation, रिक्त, LABEL_MOUNT, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = make_parent_mount_private (*rootfs, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, रिक्त, MS_BIND | MS_REC | MS_PRIVATE, रिक्त, LABEL_MOUNT, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = खोलो (*rootfs, O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  यदि (! container->context->force_no_cgroup)
    {
      पूर्णाङ्क cgroup_mode = libcrun_get_cgroup_mode (err);
      यदि (UNLIKELY (cgroup_mode < 0))
        फर्काऊ cgroup_mode;

      यदि (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          वर्ण *unified_cgroup_path = रिक्त;
          पूर्णाङ्क procfd = get_procfd (get_private_data (container), err);
          यदि (UNLIKELY (procfd < 0))
            फर्काऊ procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  यदि (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = रिक्त;
      फर्काऊ ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = रिक्त;

  यदि (रूख_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  यदि (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_do_pivot_जरा (libcrun_container_t *container, bool no_pivot, वर्ण **rootfs, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  यदि (*rootfs == रिक्त)
    फर्काऊ 0;

  यदि (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      यदि (म_भेद (*rootfs, "/") == 0)
        {
          ret = स_बदलो ("/");
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "chdir to `/`");
        }
    }
  अन्यथा
    {
      ret = chroot (*rootfs);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "chroot to `%s`", *rootfs);

      ret = स_बदलो ("/");
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "chdir to `/`");

      मुक्त (*rootfs);
      *rootfs = xstrdup ("/");
    }

  फर्काऊ 0;
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
  fd = खोलो ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  यदि (UNLIKELY (fd == -1))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  लागि (i = 0; i <= 2; i++)
    {
      यदि (UNLIKELY (ख_स्थिति (i, &statbuf) == -1))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "stat fd `%d`", i);
      यदि (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          यदि (UNLIKELY (dup2 (fd, i) == -1))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "dup2 `%d`", i);
        }
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
uidgidmap_helper (वर्ण *helper, pid_t pid, अचर वर्ण *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  वर्ण pid_fmt[16];
  वर्ण *args[MAX_ARGS + 1];
  वर्ण *next;
  cleanup_free वर्ण *map_file_copy = xstrdup (map_file);
  माप_प्रकार nargs = 0;
  पूर्णाङ्क ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, परिमाण (pid_fmt), "%d", pid);
  यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (pid_fmt)))
    फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  जबसम्म (nargs < MAX_ARGS)
    {
      वर्ण *p = strsep (&next, " \n");
      यदि (next == रिक्त)
        रोक;
      args[nargs++] = p;
    }
  args[nargs++] = रिक्त;

  ret = run_process (args, err);
  यदि (ret < 0)
    फर्काऊ ret;
  यदि (ret > 0)
    फर्काऊ crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
newgidmap (pid_t pid, अचर वर्ण *map_file, libcrun_error_t *err)
{
  फर्काऊ uidgidmap_helper ("newgidmap", pid, map_file, err);
}

स्थिर पूर्णाङ्क
newuidmap (pid_t pid, अचर वर्ण *map_file, libcrun_error_t *err)
{
  फर्काऊ uidgidmap_helper ("newuidmap", pid, map_file, err);
}

स्थिर पूर्णाङ्क
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ fd;

  ret = TEMP_FAILURE_RETRY (लिख (fd, "deny", 4));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *content = रिक्त;

  यदि (get_private_data (container)->deny_setgroups)
    फर्काऊ 0;

  यदि (container->container_def->annotations)
    {
      अचर वर्ण *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      यदि (annotation)
        फर्काऊ म_भेद (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close पूर्णाङ्क fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    यदि (fd < 0)
      ret = fd;
    अन्यथा
      ret = read_all_fd (fd, "self/setgroups", &content, रिक्त, err);
  }
  यदि (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      यदि (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          फर्काऊ 1;
        }
      फर्काऊ ret;
    }

  फर्काऊ म_भेदन (content, "deny", 4) == 0 ? 0 : 1;
}

पूर्णाङ्क
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = रिक्त;
  माप_प्रकार additional_gids_len = 0;
  पूर्णाङ्क can_do_setgroups;
  पूर्णाङ्क ret;

  यदि (process != रिक्त && process->user != रिक्त)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  यदि (UNLIKELY (can_do_setgroups < 0))
    फर्काऊ can_do_setgroups;

  यदि (can_do_setgroups == 0)
    {
      अचर वर्ण *annotation;
      माप_प्रकार i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      यदि (annotation && म_भेद (annotation, "0") != 0)
        फर्काऊ 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      लागि (i = 0; i < additional_gids_len; i++)
        यदि (additional_gids[i] != process->user->gid)
          फर्काऊ crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (पूर्णाङ्क) additional_gids[i]);
      फर्काऊ 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "setgroups");

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  यदि (get_private_data (container)->unshare_cgroupns)
    {
      पूर्णाङ्क ret = unshare (CLONE_NEWCGROUP);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (त्रुटि_सं != EINVAL)
            फर्काऊ crun_make_error (err, त्रुटि_सं, "unshare (CLONE_NEWCGROUP)");
        }
    }
  फर्काऊ 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
पूर्णाङ्क
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, माप_प्रकार len,
                   uint32_t host_id, uint32_t container_id)
{
  यदि (len != 1)
    फर्काऊ 0;

  यदि (mappings[0]->size != 1)
    फर्काऊ 0;

  यदि (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    फर्काऊ 0;

  फर्काऊ 1;
}

स्थिर पूर्णाङ्क
write_id_map (libcrun_container_t *container, pid_t pid, अचर वर्ण *map_name,
              पूर्णाङ्क (*idmap_helper) (pid_t, अचर वर्ण *, libcrun_error_t *), अचर वर्ण *helper_name,
              bool warn_on_failure, अचर वर्ण *map, माप_प्रकार map_len,
              runtime_spec_schema_defs_id_mapping **mappings, माप_प्रकार mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  पूर्णाङ्क ret = 0;

  यदि (container->host_uid)
    ret = idmap_helper (pid, map, err);
  यदि (container->host_uid == 0 || ret < 0)
    {
      cleanup_close पूर्णाङ्क fd = -1;

      यदि (ret < 0)
        {
          यदि (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      यदि (UNLIKELY (fd < 0))
        फर्काऊ fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      यदि (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          माप_प्रकार single_mapping_len;
          cleanup_free वर्ण *single_mapping = रिक्त;
          crun_error_release (err);

          यदि (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              यदि (UNLIKELY (ret < 0))
                फर्काऊ ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          यदि (UNLIKELY (fd < 0))
            फर्काऊ fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  फर्काऊ ret;
}

पूर्णाङ्क
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free वर्ण *uid_map = रिक्त;
  cleanup_free वर्ण *gid_map = रिक्त;
  माप_प्रकार uid_map_len = 0, gid_map_len = 0;
  पूर्णाङ्क ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  यदि ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    फर्काऊ 0;

  यदि (! def || ! def->linux)
    फर्काऊ 0;

  यदि (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  अन्यथा
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      uid_map_len = (माप_प्रकार) ret;
      यदि (uid_map == रिक्त)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
    }

  यदि (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  अन्यथा
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      gid_map_len = (माप_प्रकार) ret;
      यदि (gid_map == रिक्त)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

संरचना all_caps_s
{
  अचिन्हित दीर्घ effective[2];
  अचिन्हित दीर्घ permitted[2];
  अचिन्हित दीर्घ inheritable[2];
  अचिन्हित दीर्घ ambient[2];
  अचिन्हित दीर्घ bounding[2];
};

स्थिर पूर्णाङ्क
has_cap_on (पूर्णाङ्क cap, दीर्घ अचिन्हित *caps)
{
  यदि (cap < 32)
    फर्काऊ CAP_TO_MASK_0 (cap) & caps[0];
  फर्काऊ (CAP_TO_MASK_1 (cap) & caps[1]);
}

स्थिर अचिन्हित दीर्घ cap_last_cap;

पूर्णाङ्क
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;
  वर्ण buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  यदि (fd < 0)
    फर्काऊ fd;
  ret = TEMP_FAILURE_RETRY (पठ (fd, buffer, परिमाण (buffer) - 1));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  त्रुटि_सं = 0;
  cap_last_cap = म_से_अदीर्घ (buffer, रिक्त, 10);
  यदि (त्रुटि_सं != 0)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
set_required_caps (संरचना all_caps_s *caps, uid_t uid, gid_t gid, पूर्णाङ्क no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  अचिन्हित दीर्घ cap;
  पूर्णाङ्क ret;
  संरचना __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  संरचना __user_cap_data_struct data[2] = { { 0 } };

  यदि (cap_last_cap == 0)
    फर्काऊ crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  लागि (cap = 0; cap <= cap_last_cap; cap++)
    यदि (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        यदि (UNLIKELY (ret < 0 && ! (त्रुटि_सं == EINVAL)))
          फर्काऊ crun_make_error (err, त्रुटि_सं, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  यदि (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    फर्काऊ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  यदि (UNLIKELY (ret < 0 && ! (त्रुटि_सं == EINVAL || त्रुटि_सं == EPERM)))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "prctl reset ambient");

  लागि (cap = 0; cap <= cap_last_cap; cap++)
    यदि (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        यदि (UNLIKELY (ret < 0 && ! (त्रुटि_सं == EINVAL || त्रुटि_सं == EPERM)))
          फर्काऊ crun_make_error (err, त्रुटि_सं, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  यदि (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "prctl reset KEEPCAPS");

      यदि (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          लागि (cap = 0; cap <= cap_last_cap; cap++)
            यदि (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "capset");
        }
    }
#endif

  यदि (no_new_privs)
    यदि (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      फर्काऊ crun_make_error (err, त्रुटि_सं, "prctl set no new privs");

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
read_caps (अचिन्हित दीर्घ caps[2], वर्ण **मानहरू, माप_प्रकार len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  माप_प्रकार i;
  लागि (i = 0; i < len; i++)
    {
      cap_मान_t cap;
      यदि (cap_from_name (मानहरू[i], &cap) < 0)
        फर्काऊ crun_make_error (err, 0, "unknown capability `%s`", मानहरू[i]);

      यदि (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      अन्यथा
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#अन्यथा
  caps[0] = 0;
  caps[1] = 0;
#endif
  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  यदि (proc->selinux_label)
    फर्काऊ set_selinux_label (container, proc->selinux_label, now, err);

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  यदि (proc->apparmor_profile)
    फर्काऊ set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  पूर्णाङ्क no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  संरचना all_caps_s caps = {};
  पूर्णाङ्क ret;

  यदि (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  फर्काऊ set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

संरचना rlimit_s
{
  अचर वर्ण *name;
  पूर्णाङ्क मान;
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
                              { "RLIMIT_STACK", RLIMIT_थाक },
                              { रिक्त, 0 } };

स्थिर पूर्णाङ्क
get_rlimit_resource (अचर वर्ण *name)
{
  संरचना rlimit_s *it;
  लागि (it = rlimits; it->name; it++)
    यदि (म_भेद (it->name, name) == 0)
      फर्काऊ it->मान;
  फर्काऊ -1;
}

पूर्णाङ्क
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, माप_प्रकार len,
                     libcrun_error_t *err)
{
  माप_प्रकार i, j;
  /* Check for duplicate rlimit types.  */
  लागि (i = 0; i < len; i++)
    {
      लागि (j = i + 1; j < len; j++)
        {
          यदि (म_भेद (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            फर्काऊ crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  लागि (i = 0; i < len; i++)
    {
      संरचना rlimit limit;
      वर्ण *type = new_rlimits[i]->type;
      पूर्णाङ्क resource = get_rlimit_resource (type);
      यदि (UNLIKELY (resource < 0))
        फर्काऊ crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (अचिन्हित दीर्घ दीर्घ) limit.rlim_cur,
                     (अचिन्हित दीर्घ दीर्घ) limit.rlim_max);
      यदि (UNLIKELY (setrlimit (resource, &limit) < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "setrlimit `%s`", type);
    }
  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  पूर्णाङ्क ret;
  यदि (def->hostname == रिक्त || def->hostname[0] == '\0')
    फर्काऊ 0;
  यदि (! has_uts)
    फर्काऊ crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, म_माप (def->hostname));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "sethostname");
  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  पूर्णाङ्क ret;
  यदि (is_empty_string (def->domainname))
    फर्काऊ 0;
  यदि (! has_uts)
    फर्काऊ crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, म_माप (def->domainname));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "setdomainname");
  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;
  वर्ण oom_buffer[16];
  यदि (def->process == रिक्त || ! def->process->oom_score_adj_present)
    फर्काऊ 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, परिमाण (oom_buffer), "%i", def->process->oom_score_adj);
  यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (oom_buffer)))
    फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  यदि (fd < 0)
    फर्काऊ fd;
  ret = TEMP_FAILURE_RETRY (लिख (fd, oom_buffer, म_माप (oom_buffer)));
  यदि (ret < 0)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "write to `/proc/self/oom_score_adj`");
  फर्काऊ 0;
}

अचर वर्ण *sysctlRequiringIPC[] = {
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
validate_sysctl (अचर वर्ण *original_साँचो, अचर वर्ण *original_मान, अचर वर्ण *name, अचिन्हित दीर्घ namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  अचर वर्ण *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  यदि (name[0] == '/' || path_has_dot_dot_component (name))
    फर्काऊ crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_साँचो);

  यदि (has_prefix (name, "fs/mqueue/"))
    {
      यदि (namespaces_created & CLONE_NEWIPC)
        फर्काऊ 0;

      namespace = "IPC";
      गच्छ fail;
    }

  यदि (has_prefix (name, "kernel/"))
    {
      माप_प्रकार i;

      लागि (i = 0; sysctlRequiringIPC[i]; i++)
        यदि (म_भेद (sysctlRequiringIPC[i], name) == 0)
          {
            यदि (namespaces_created & CLONE_NEWIPC)
              फर्काऊ 0;

            namespace = "IPC";
            गच्छ fail;
          }

      यदि (म_भेद (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          यदि (! is_empty_string (def->domainname) && (म_भेद (original_मान, def->domainname) != 0))
            फर्काऊ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_साँचो);

          यदि (namespaces_created & CLONE_NEWUTS)
            फर्काऊ 0;

          namespace = "UTS";
          गच्छ fail;
        }

      यदि (म_भेद (name, "kernel/hostname") == 0)
        फर्काऊ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_साँचो);
    }
  यदि (has_prefix (name, "net/"))
    {
      यदि (namespaces_created & CLONE_NEWNET)
        फर्काऊ 0;

      namespace = "network";
      गच्छ fail;
    }

  फर्काऊ crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_साँचो);

fail:
  फर्काऊ crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_साँचो, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
स्थिर वर्ण *
sysctl_error_reason (अचर वर्ण *name, पूर्णाङ्क namespaces_created, पूर्णाङ्क errno_)
{

  यदि (म_भेद (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    फर्काऊ xstrdup ("are all the IDs mapped in the user namespace?");

  फर्काऊ रिक्त;
}

पूर्णाङ्क
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  माप_प्रकार i;
  cleanup_close पूर्णाङ्क dirfd = -1;
  अचिन्हित दीर्घ namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  यदि (def->linux == रिक्त || def->linux->sysctl == रिक्त || def->linux->sysctl->len == 0)
    फर्काऊ 0;

  लागि (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णाङ्क मान;

      मान = libcrun_find_namespace (def->linux->namespaces[i]->type);
      यदि (UNLIKELY (मान < 0))
        फर्काऊ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= मान;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  यदि (UNLIKELY (dirfd < 0))
    फर्काऊ dirfd;

  लागि (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free वर्ण *name = रिक्त;
      cleanup_close पूर्णाङ्क fd = -1;
      पूर्णाङ्क ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      यदि (def->linux->sysctl->साँचोहरू[i] == रिक्त || def->linux->sysctl->मानहरू[i] == रिक्त)
        फर्काऊ crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_साँचो_to_proc_path (def->linux->sysctl->साँचोहरू[i]);

      ret = validate_sysctl (def->linux->sysctl->साँचोहरू[i], def->linux->sysctl->मानहरू[i], name, namespaces_created, def, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      यदि (UNLIKELY (fd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (लिख (fd, def->linux->sysctl->मानहरू[i], म_माप (def->linux->sysctl->मानहरू[i])));
      यदि (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क saved_errno = त्रुटि_सं;
          cleanup_free वर्ण *reason = रिक्त;

          reason = sysctl_error_reason (def->linux->sysctl->साँचोहरू[i], namespaces_created, saved_errno);
          फर्काऊ crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
open_terminal (वर्ण **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  uid_t uid = 0;
  पूर्णाङ्क ret;

  यदि (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  फर्काऊ ret;
}

वर्ण *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  फर्काऊ get_private_data (container)->external_descriptors;
}

पूर्णाङ्क
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  अचर वर्ण *buf = रिक्त;
  json_gen_ctx *gen = रिक्त;
  माप_प्रकार buf_len;
  पूर्णाङ्क ret;
  पूर्णाङ्क i;

  यदि (! json_gen_init (&gen, रिक्त))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "json_gen_init");

  ret = json_gen_array_open (gen);
  यदि (UNLIKELY (ret != json_gen_status_ok))
    गच्छ json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  लागि (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      वर्ण link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      यदि (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          यदि (त्रुटि_सं == ENOENT)
            {
              म_नकल (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          अन्यथा
            {
              json_gen_free (gen);
              फर्काऊ crun_make_error (err, त्रुटि_सं, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      यदि (UNLIKELY (ret != json_gen_status_ok))
        गच्छ json_error;
    }

  ret = json_gen_array_close (gen);
  यदि (UNLIKELY (ret != json_gen_status_ok))
    गच्छ json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  यदि (UNLIKELY (ret != json_gen_status_ok))
    गच्छ json_error;

  यदि (buf)
    {
      वर्ण *b = xmalloc (buf_len + 1);
      स_नकल (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  फर्काऊ 0;

json_error:
  यदि (gen)
    json_gen_free (gen);
  फर्काऊ json_gen_error_to_crun_error (ret, err);
}

पूर्णाङ्क
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free वर्ण *pty = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;

  यदि (def->process == रिक्त || ! def->process->terminal)
    फर्काऊ 0;

  fd = open_terminal (&pty, def->process, err);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ fd;

  यदि (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  यदि (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", रिक्त, MS_BIND, रिक्त, LABEL_MOUNT, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  अन्यथा
    {
      ret = unlink ("/dev/console");
      यदि (UNLIKELY (ret < 0 && त्रुटि_सं != ENOENT))
        {
          यदि (त्रुटि_सं == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", रिक्त, MS_BIND, रिक्त, LABEL_MOUNT, err);
              यदि (UNLIKELY (ret < 0))
                फर्काऊ ret;

              फर्काऊ get_and_reset (&fd);
            }
          फर्काऊ crun_make_error (err, त्रुटि_सं, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "symlink `/dev/console` -> `%s`", pty);
    }

  फर्काऊ get_and_reset (&fd);
}

स्थिर bool
read_error_from_sync_socket (पूर्णाङ्क sync_socket_fd, पूर्णाङ्क *error, वर्ण **str)
{
  cleanup_free वर्ण *b = रिक्त;
  माप_प्रकार size;
  पूर्णाङ्क code = 0;
  पूर्णाङ्क ret;

  यदि (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (पठ (sync_socket_fd, &code, परिमाण (code)));
      यदि (UNLIKELY (ret != (पूर्णाङ्क) परिमाण (code)))
        फर्काऊ false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (पठ (sync_socket_fd, &size, परिमाण (size)));
  यदि (UNLIKELY (ret != (पूर्णाङ्क) परिमाण (size)))
    फर्काऊ false;

  यदि (size == 0)
    फर्काऊ false;

  यदि (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (पठ (sync_socket_fd, b, size));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ false;

  b[ret] = '\0';

  *str = b;
  b = रिक्त;
  फर्काऊ true;
}

स्थिर bool
send_error_to_sync_socket (पूर्णाङ्क sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  माप_प्रकार size;
  पूर्णाङ्क ret;
  पूर्णाङ्क code;
  वर्ण *सन्देश;

  यदि (err == रिक्त || *err == रिक्त)
    फर्काऊ false;

  code = crun_error_get_errno (err);

  यदि (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, "1", 1));
      यदि (UNLIKELY (ret < 0))
        फर्काऊ false;
    }

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, &code, परिमाण (code)));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ false;

  सन्देश = (*err)->सन्देश;
  size = म_माप (सन्देश) + 1;
  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, &size, परिमाण (size)));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ false;

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_fd, सन्देश, size));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ false;

  फर्काऊ true;
}

स्थिर अन्तःस्थ पूर्णाङ्क
send_success_to_sync_socket (पूर्णाङ्क sync_socket, libcrun_error_t *err)
{
  अचर पूर्णाङ्क success = 0;
  पूर्णाङ्क ret;

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket, &success, परिमाण (success)));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "write to sync socket");

  फर्काऊ 0;
}

स्थिर __attribute__ ((noreturn)) शून्य
send_error_to_sync_socket_and_die (पूर्णाङ्क sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  वर्ण *सन्देश;

  यदि (err == रिक्त || *err == रिक्त)
    _safe_exit (निकास_त्रुटि);

  यदि (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      त्रुटि_सं = crun_error_get_errno (err);
      सन्देश = (*err)->सन्देश;
      libcrun_fail_with_error (त्रुटि_सं, "%s", सन्देश);
    }
  _safe_exit (निकास_त्रुटि);
}

स्थिर पूर्णाङ्क
expect_success_from_sync_socket (पूर्णाङ्क sync_fd, libcrun_error_t *err)
{
  cleanup_free वर्ण *err_str = रिक्त;
  पूर्णाङ्क res = 1;
  पूर्णाङ्क ret;

  ret = TEMP_FAILURE_RETRY (पठ (sync_fd, &res, परिमाण (res)));
  यदि (UNLIKELY (ret != परिमाण (res)))
    फर्काऊ crun_make_error (err, ret < 0 ? त्रुटि_सं : 0, "read status from sync socket");

  यदि (res == 0)
    फर्काऊ 0;

  यदि (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      यदि (! is_empty_string (err_str))
        फर्काऊ crun_make_error (err, res, "%s", err_str);
    }

  फर्काऊ crun_make_error (err, res, "read from sync socket");
}

स्थिर पूर्णाङ्क
join_namespaces (runtime_spec_schema_config_schema *def, पूर्णाङ्क *namespaces_to_join, पूर्णाङ्क n_namespaces_to_join,
                 पूर्णाङ्क *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  पूर्णाङ्क i;

  लागि (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free वर्ण *cwd = रिक्त;
      पूर्णाङ्क orig_index = namespaces_to_join_index[i];
      पूर्णाङ्क मान;

      यदि (namespaces_to_join[i] < 0)
        जारी;

      /* Skip the user namespace.  */
      मान = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      यदि (मान == CLONE_NEWUSER)
        जारी;

      यदि (मान == CLONE_NEWNS)
        {
          cwd = getcwd (रिक्त, 0);
          यदि (UNLIKELY (cwd == रिक्त))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], मान);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (ignore_join_errors)
            जारी;
          फर्काऊ crun_make_error (err, त्रुटि_सं, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      यदि (मान == CLONE_NEWNS)
        {
          ret = स_बदलो (cwd);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "chdir `%s`", cwd);
        }
    }
  फर्काऊ 0;
}

#define MAX_NAMESPACES 10

संरचना init_status_s
{
  /* fd to the namespace to join.  */
  पूर्णाङ्क fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  पूर्णाङ्क index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  पूर्णाङ्क मान[MAX_NAMESPACES];
  /* How many namespaces to join.  */
  माप_प्रकार fd_len;

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
  माप_प्रकार i;

  यदि (ns == रिक्त)
    फर्काऊ;

  लागि (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (बंद (ns->fd[i]));
}

स्थिर पूर्णाङ्क
configure_init_status (संरचना init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  माप_प्रकार i;

  लागि (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  लागि (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णाङ्क मान = libcrun_find_namespace (def->linux->namespaces[i]->type);
      यदि (UNLIKELY (मान < 0))
        फर्काऊ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      यदि (ns->all_namespaces & मान)
        फर्काऊ crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= मान;

      यदि (def->linux->namespaces[i]->path == रिक्त)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= मान;
        }
      अन्यथा
        {
          पूर्णाङ्क fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          यदि (ns->fd_len >= MAX_NAMESPACES)
            फर्काऊ crun_make_error (err, 0, "too many namespaces to join");

          fd = खोलो (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          यदि (UNLIKELY (fd < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", def->linux->namespaces[i]->path);

          यदि (मान == CLONE_NEWUSER)
            {
              ns->userns_index = ns->fd_len;
              ns->userns_index_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->index[ns->fd_len] = i;
          ns->मान[ns->fd_len] = मान;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  यदि (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  फर्काऊ 0;
}

/* Detect if root is available in the container.  */
स्थिर bool
जरा_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, माप_प्रकार len)
{
  माप_प्रकार i;

  लागि (i = 0; i < len; i++)
    यदि (mappings[i]->container_id == 0)
      फर्काऊ true;

  फर्काऊ false;
}

स्थिर संरचना libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  संरचना libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  यदि (dev_fds == रिक्त)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      माप_प्रकार len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  फर्काऊ dev_fds;
}

#define अङ्क_NEEDED_DEVS (परिमाण (needed_devs) / परिमाण (needed_devs[0]) - 1)

स्थिर संरचना libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  संरचना libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  यदि (fds == रिक्त)
    {
      fds = make_libcrun_fd_map (अङ्क_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  फर्काऊ fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  माप_प्रकार i;

  यदि (recursive)
    *recursive = false;

  यदि (src_nofollow != रिक्त)
    *src_nofollow = false;

  लागि (i = 0; i < mnt->options_len; i++)
    {
      यदि (mnt->options[i] == रिक्त)
        जारी;

      यदि (म_भेद (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          यदि (src_nofollow == रिक्त || *src_nofollow)
            रोक;
        }
      यदि (म_भेद (mnt->options[i], "rbind") == 0)
        {
          यदि (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          यदि (src_nofollow == रिक्त || *src_nofollow)
            रोक;
        }
      यदि (src_nofollow && म_भेद (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  फर्काऊ ret;
}

स्थिर वर्ण *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  माप_प्रकार i;

  लागि (i = 0; i < mnt->options_len; i++)
    {
      यदि (mnt->options[i] == रिक्त)
        जारी;

      यदि (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          फर्काऊ mnt->options[i];
        }
      यदि (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          फर्काऊ mnt->options[i];
        }
    }
  फर्काऊ रिक्त;
}

स्थिर पूर्णाङ्क
open_mount_type (अचर वर्ण *type, पूर्णाङ्क *out_fd, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fsopen_fd = -1;
  cleanup_close पूर्णाङ्क newfs_fd = -1;
  पूर्णाङ्क ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  यदि (UNLIKELY (fsopen_fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, रिक्त, रिक्त, 0);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  यदि (UNLIKELY (newfs_fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  फर्काऊ 0;
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
  अचर वर्ण *idmap_option;
  bool recursive = false;
  अचर वर्ण *options = रिक्त;
  bool has_mappings;
  पूर्णाङ्क ret;
  वर्ण *extra_सन्देश = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != रिक्त);
  यदि (has_mappings_out)
    *has_mappings_out = has_mappings;
  यदि (! has_mappings)
    फर्काऊ 0;

  यदि ((mnt->uid_mappings == रिक्त) != (mnt->gid_mappings == रिक्त))
    फर्काऊ crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  यदि (idmap_option)
    {
      options = म_अक्षर (idmap_option, '=');
      यदि (options)
        {
          /* Skip the '=' itself.  */
          options++;
          यदि (options[0] == '\0')
            options = रिक्त;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;
  यदि (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ fd;

  यदि (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_रूख (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      यदि (UNLIKELY (newfs_fd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", mnt->source);
    }
  अन्यथा
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (त्रुटि_सं == EINVAL)
        {
          extra_सन्देश = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      फर्काऊ crun_make_error (err, त्रुटि_सं, "mount_setattr `%s`%s", mnt->destination, extra_सन्देश);
    }

  *out_fd = get_and_reset (&newfs_fd);
  फर्काऊ 0;
}

स्थिर uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  माप_प्रकार len;
  माप_प्रकार i;

  यदि (! def || ! def->linux)
    गच्छ निकास;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  यदि (! mappings)
    गच्छ निकास;

  लागि (i = 0; i < len; i++)
    {
      यदि (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        फर्काऊ id - mappings[i]->container_id + mappings[i]->host_id;
    }
निकास:
  फर्काऊ is_uid ? get_overflow_uid () : get_overflow_gid ();
}

स्थिर पूर्णाङ्क
precreate_device (libcrun_container_t *container, पूर्णाङ्क devs_dirfd, माप_प्रकार i, libcrun_error_t *err)
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
  यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (name)))
    फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "chown `%s`", device->path);

  फर्काऊ get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

स्थिर पूर्णाङ्क
send_mounts (पूर्णाङ्क sync_socket_host, संरचना libcrun_fd_map *fds, माप_प्रकार how_many, माप_प्रकार total, libcrun_error_t *err)
{
  माप_प्रकार i;
  पूर्णाङ्क ret;

  ret = TEMP_FAILURE_RETRY (लिख (sync_socket_host, &how_many, परिमाण (how_many)));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "write to sync socket");

  लागि (i = 0; i < total; i++)
    {
      यदि (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (वर्ण *) &i, परिमाण (i), err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, पूर्णाङ्क sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map संरचना libcrun_fd_map *mount_fds = रिक्त;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free वर्ण *rootfs = रिक्त;
  माप_प्रकार how_many = 0;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  यदि (def->mounts_len == 0)
    फर्काऊ 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  यदि (def->जरा && def->जरा->path)
    rootfs = realpath (def->जरा->path, रिक्त);

  यदि (! has_userns)
    {
      पूर्णाङ्क is_in_userns = check_running_in_user_namespace (err);
      यदि (UNLIKELY (is_in_userns < 0))
        फर्काऊ is_in_userns;

      has_userns = is_in_userns > 0;
    }

  लागि (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      पूर्णाङ्क mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      यदि (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == रिक्त)
        {
          अचिन्हित दीर्घ propagation = 0;

          यदि (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, रिक्त, रिक्त, रिक्त, रिक्त) & ALL_PROPAGATIONS_NO_REC;
          यदि (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          यदि (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      यदि (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  फर्काऊ send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
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
  अचर वर्ण *context_type = रिक्त;
  अचर वर्ण *label = रिक्त;
  माप_प्रकार how_many = 0;
  माप_प्रकार i;
  पूर्णाङ्क ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  यदि (def->linux == रिक्त || def->linux->devices_len == 0)
    फर्काऊ 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  यदि (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    फर्काऊ send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_जरा : रिक्त),
                                     (container->context ? container->context->id : रिक्त), err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = सूची_गढ़ो (devs_path, 0700);
  यदि (UNLIKELY (ret < 0 && त्रुटि_सं != EEXIST))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  यदि (UNLIKELY (current_mountns < 0))
    फर्काऊ current_mountns;

  ret = unshare (CLONE_NEWNS);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (रिक्त, "/", रिक्त, MS_REC | MS_PRIVATE, रिक्त);
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, त्रुटि_सं, "mount `MS_REC | MS_PRIVATE`");
      गच्छ restore_mountns;
    }

  यदि (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      यदि (UNLIKELY (context_type == रिक्त))
        {
          ret = -1;
          गच्छ restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", रिक्त, context_type, label, रिक्त);
  यदि (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      गच्छ restore_mountns;
    }

  targetfd = खोलो (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (targetfd < 0)
    {
      ret = crun_make_error (err, त्रुटि_सं, "open `%s`", devs_path);
      गच्छ restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, रिक्त);
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, त्रुटि_सं, "fs_move_mount_to `%s`", devs_path);
      गच्छ restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (targetfd < 0)
    {
      ret = crun_make_error (err, त्रुटि_सं, "open `%s`", devs_path);
      गच्छ restore_mountns;
    }

  लागि (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      यदि (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          जारी;
        }

      dev_fds->fds[i] = ret;

      यदि (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  यदि (ns_changed && current_mountns >= 0)
    {
      पूर्णाङ्क setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      यदि (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          फर्काऊ crun_make_error (err, त्रुटि_सं, "setns `CLONE_NEWNS`");
        }
    }

  फर्काऊ ret;
}

स्थिर पूर्णाङ्क
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, पूर्णाङ्क sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map संरचना libcrun_fd_map *fds = रिक्त;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  माप_प्रकार how_many = 0;
  माप_प्रकार i;

  fds = make_libcrun_fd_map (अङ्क_NEEDED_DEVS);

  यदि (! has_userns || geteuid () > 0)
    फर्काऊ send_mounts (sync_socket_host, fds, 0, अङ्क_NEEDED_DEVS, err);

  लागि (i = 0; i < अङ्क_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      पूर्णाङ्क fd = syscall_open_रूख (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      यदि (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  फर्काऊ send_mounts (sync_socket_host, fds, how_many, अङ्क_NEEDED_DEVS, err);
}

स्थिर पूर्णाङ्क
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, पूर्णाङ्क sync_socket_host, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
receive_mounts (संरचना libcrun_fd_map *fds, पूर्णाङ्क sync_socket_container, libcrun_error_t *err)
{
  माप_प्रकार i, how_many = 0;
  पूर्णाङ्क ret;

  यदि (fds->nfds == 0)
    फर्काऊ 0;

  ret = TEMP_FAILURE_RETRY (पठ (sync_socket_container, &how_many, परिमाण (how_many)));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "read from sync socket");

  लागि (i = 0; i < how_many; i++)
    {
      माप_प्रकार index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (वर्ण *) &index, परिमाण (index), err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
      यदि (index >= fds->nfds)
        फर्काऊ crun_make_error (err, 0, "invalid mount data received");

      यदि (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (बंद (fds->fds[index]));

      fds->fds[index] = ret;
    }

  फर्काऊ 0;
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

  यदि (! container->host_has_cap_sys_ptrace)
    फर्काऊ 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "prctl (PR_SET_DUMPABLE)");

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  पूर्णाङ्क ret;

  यदि (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool जरा_mapped = false;

      यदि (def->linux->uid_mappings_len != 0)
        {
          जरा_mapped = जरा_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          यदि (! जरा_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      यदि (def->linux->gid_mappings_len != 0)
        {
          जरा_mapped = जरा_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          यदि (! जरा_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  यदि (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    फर्काऊ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "setresgid to `%d`", gid);

  फर्काऊ 0;
}

स्थिर bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  फर्काऊ mnt->type && म_भेद (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, रिक्त, रिक्त);
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
  cleanup_close पूर्णाङ्क जरा_fd = -1;
  cleanup_close पूर्णाङ्क cwd_fd = -1;
  माप_प्रकार i, missing = 0;
  पूर्णाङ्क saved_errno;
  पूर्णाङ्क sockets[2];
  pid_t pid;
  पूर्णाङ्क ret;

  यदि (def->जरा == रिक्त || def->जरा->path == रिक्त)
    फर्काऊ 0;

  लागि (i = 0; i < def->mounts_len; i++)
    यदि (is_proc_mount (def->mounts[i]))
      missing++;
  यदि (missing == 0)
    फर्काऊ 1;

  जरा_fd = खोलो ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (जरा_fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `/`");

  cwd_fd = खोलो (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (cwd_fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->जरा->path);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ 0;

  pid = विभाजन ();
  saved_errno = त्रुटि_सं;
  यदि (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      यदि (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (शून्य) unshare (CLONE_NEWNS);

      लागि (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close पूर्णाङ्क fd = -1;

          यदि (! is_proc_mount (def->mounts[i]))
            जारी;

          fd = open_mount_of_type (container, def->mounts[i], err);
          यदि (fd < 0)
            _safe_exit (निकास_त्रुटि);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (वर्ण *) &i, परिमाण (i), err);
          यदि (UNLIKELY (ret < 0))
            _safe_exit (निकास_त्रुटि);
        }
      _safe_exit (निकास_सफल);
    }

  यदि (UNLIKELY (fchdir (जरा_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "restore the root directory");

  यदि (UNLIKELY (pid < 0))
    फर्काऊ crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  जबसम्म (missing > 0)
    {
      libcrun_error_t tmp_err = रिक्त;

      ret = receive_fd_from_socket_with_payload (socket_parent, (वर्ण *) &i, परिमाण (i), &tmp_err);
      यदि (ret < 0)
        {
          crun_error_release (&tmp_err);
          रोक;
        }
      यदि (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (बंद (ret));
          रोक;
        }

      यदि (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (बंद (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, रिक्त, 0);

  फर्काऊ missing == 0 ? 1 : 0;
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
  माप_प्रकार i;
  पूर्णाङ्क ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      यदि (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      यदि (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      यदि (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          delayed_pidns_fork = ret > 0;
        }

      यदि (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          ret = TEMP_FAILURE_RETRY (लिख (sync_socket_container, &new_pid, परिमाण (new_pid)));
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "write to sync socket");
        }
      अन्यथा
        {
          new_pid = विभाजन ();
          यदि (UNLIKELY (new_pid < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "fork");
        }

      यदि (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          यदि (UNLIKELY (ret < 0))
            समाप्त (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (लिख (sync_socket_container, &new_pid, परिमाण (new_pid)));
          यदि (UNLIKELY (ret < 0))
            समाप्त (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  ret = libcrun_set_oom (container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  यदि ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      लागि (i = 0; i < def->mounts_len; i++)
        {
          पूर्णाङ्क fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          यदि (def->mounts[i]->type == रिक्त)
            जारी;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          यदि (init_status->join_pidns && ! delayed_pidns_fork && म_भेद (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, रिक्त, रिक्त, रिक्त);
          यदि (init_status->join_ipcns && म_भेद (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, रिक्त, रिक्त, रिक्त);

          यदि (fd >= 0)
            {
              यदि (mount_fds->fds[i] >= 0)
                TEMP_FAILURE_RETRY (बंद (mount_fds->fds[i]));
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
  यदि (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      यदि (UNLIKELY (timens_offsets_fd < 0))
        फर्काऊ timens_offsets_fd;
    }

  यदि (init_status->all_namespaces & CLONE_NEWUSER)
    {
      यदि (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }

      यदि (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
      अन्यथा
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "unshare");
    }

  यदि (def->linux->time_offsets)
    {
      वर्ण fmt_buffer[128];
      पूर्णाङ्क fd = timens_offsets_fd;

      यदि (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, परिमाण (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (fmt_buffer)))
            फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (दीर्घ दीर्घ पूर्णाङ्क) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = लिख (fd, fmt_buffer, म_माप (fmt_buffer));
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "write `/proc/self/timens_offsets`");
        }
      यदि (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (दीर्घ दीर्घ पूर्णाङ्क) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, परिमाण (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (fmt_buffer)))
            फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = लिख (fd, fmt_buffer, म_माप (fmt_buffer));
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "write `/proc/self/timens_offsets`");
        }
    }

  यदि (init_status->must_fork)
    {
      यदि (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          delayed_pidns_fork = ret > 0;
        }

      यदि (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = विभाजन ();
          यदि (UNLIKELY (pid_container < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      यदि (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          ret = TEMP_FAILURE_RETRY (लिख (sync_socket_container, &pid_container, परिमाण (pid_container)));
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "write to sync socket");

          यदि (pid_container)
            _safe_exit (निकास_सफल);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  फर्काऊ get_private_data (container)->delayed_pidns_fork;
}

पूर्णाङ्क
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क client_fd = -1;
  cleanup_close पूर्णाङ्क pidfd = -1;
  अचर वर्ण *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  यदि (v == रिक्त)
    फर्काऊ 0;

  pidfd = syscall_pidfd_open (pid, 0);
  यदि (UNLIKELY (pidfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  यदि (UNLIKELY (client_fd < 0))
    फर्काऊ client_fd;

  फर्काऊ send_fd_to_socket (client_fd, pidfd, err);
}

स्थिर bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  यदि (process == रिक्त || process->exec_cpu_affinity == रिक्त)
    फर्काऊ false;
  फर्काऊ (! is_empty_string (process->exec_cpu_affinity->initial))
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
  माप_प्रकार i;
  पूर्णाङ्क ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  यदि (def->जरा)
    {
      ret = do_notify_socket (container, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  यदि (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  ret = libcrun_set_oom (container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->जरा ? container->container_def->जरा->path : रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  लागि (i = 0; i < init_status.fd_len; i++)
    {
      विकल्प (init_status.मान[i])
        {
        पक्ष CLONE_NEWIPC:
          यदि (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          रोक;

        पक्ष CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          यदि (! new_userns)
            init_status.must_fork = true;
          अन्यथा
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          रोक;

        पक्ष CLONE_NEWTIME:
          यदि (! new_userns)
            init_status.must_fork = true;
          अन्यथा
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          रोक;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  यदि ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  अन्यथा यदि ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  यदि (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  यदि (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      संरचना _clone3_args clone3_args;
      स_रखो (&clone3_args, 0, परिमाण (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      यदि (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  यदि (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, रिक्त);
      यदि (UNLIKELY (pid < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "clone");
    }

  यदि (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = close_and_reset (&sync_socket_container);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_रूख_fd));

      यदि (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          ret = TEMP_FAILURE_RETRY (पठ (sync_socket_host, &new_pid, परिमाण (new_pid)));
          यदि (UNLIKELY (ret != परिमाण (new_pid)))
            फर्काऊ crun_make_error (err, ret < 0 ? त्रुटि_सं : 0, "read pid from sync socket");

          यदि (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          अन्यथा
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, रिक्त, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }

      यदि (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }

      यदि ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }

      यदि (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          ret = TEMP_FAILURE_RETRY (पठ (sync_socket_host, &grandchild, परिमाण (grandchild)));
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          यदि (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          अन्यथा
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, रिक्त, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      यदि (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }

      pid_to_clean = 0;
      फर्काऊ pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  यदि (UNLIKELY (ret < 0))
    libcrun_fail_with_error (त्रुटि_सं, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  यदि (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  अन्यथा
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      यदि (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->सन्देश);
    }

  /* Jump into the specified entrypoint.  */
  यदि (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  यदि (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->सन्देश);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  यदि (ret == 0)
    _safe_exit (निकास_सफल);

  _safe_exit (निकास_त्रुटि);
}

स्थिर पूर्णाङ्क
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, पूर्णाङ्क sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, अचर वर्ण *sub_cgroup,
                            पूर्णाङ्क *terminal_fd, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  वर्ण res;
  pid_t pid;
  cleanup_close पूर्णाङ्क sync_fd = sync_socket_fd;

  यदि (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (पठ (sync_fd, &res, 1));
  यदि (UNLIKELY (ret != 1))
    फर्काऊ crun_make_error (err, ret < 0 ? त्रुटि_सं : 0, "read from sync socket");

  यदि (res != '0')
    फर्काऊ crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (पठ (sync_fd, &pid, परिमाण (pid)));
  यदि (UNLIKELY (ret != परिमाण (pid)))
    फर्काऊ crun_make_error (err, ret < 0 ? त्रुटि_सं : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, रिक्त, 0);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "waitpid for exec child pid");

  यदि (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  यदि (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  यदि (need_move_to_cgroup)
    {
      cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free वर्ण *final_cgroup = रिक्त;

      यदि (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, रिक्त);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = libcrun_set_io_priority (pid, process, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (लिख (sync_fd, &ret, परिमाण (ret)));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "write to sync socket");

  यदि (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      यदि (UNLIKELY (ret < 0))
        {
          पूर्णाङ्क err_code = 0;
          cleanup_free वर्ण *err_str = रिक्त;

          यदि (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              यदि (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  फर्काऊ crun_make_error (err, err_code, "%s", err_str);
                }
            }

          फर्काऊ crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  फर्काऊ pid;
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
  माप_प्रकार i;
  पूर्णाङ्क ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  यदि (def->linux && def->linux->namespaces)
    {
      लागि (i = 0; i < def->linux->namespaces_len; i++)
        यदि (! is_empty_string (def->linux->namespaces[i]->path))
          फर्काऊ 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  यदि (UNLIKELY (pidfd_pid_to_join < 0))
    फर्काऊ 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;
  यदि (ret == 0)
    फर्काऊ crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  लागि (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].मान;

  ret = setns (pidfd_pid_to_join, all_flags);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ 0;

  फर्काऊ 1;
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
  माप_प्रकार i;
  पूर्णाङ्क ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;
  /* Nothing left to do if the namespaces were joined.  */
  यदि (LIKELY (ret > 0))
    फर्काऊ 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  यदि (def->linux->namespaces_len >= MAX_NAMESPACES)
    फर्काऊ crun_make_error (err, 0, "invalid configuration");

  लागि (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free वर्ण *ns_path = रिक्त;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      यदि (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          यदि (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              जारी;
            }

          ret = fds[i];
          गच्छ निकास;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निकास;
  यदि (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      गच्छ निकास;
    }

  लागि (i = 0; namespaces[i].ns_file; i++)
    {
      यदि (namespaces[i].मान == CLONE_NEWUSER)
        जारी;

      ret = setns (fds[i], 0);
      यदि (ret == 0)
        fds_joined[i] = 1;
    }
  लागि (i = 0; namespaces[i].ns_file; i++)
    {
      यदि (fds_joined[i])
        जारी;
      ret = setns (fds[i], 0);
      यदि (ret == 0)
        fds_joined[i] = 1;
    }
  लागि (i = 0; namespaces[i].ns_file; i++)
    {
      यदि (fds_joined[i])
        जारी;
      ret = setns (fds[i], 0);
      यदि (UNLIKELY (ret < 0 && त्रुटि_सं != EINVAL))
        {
          माप_प्रकार j;
          bool found = false;

          लागि (j = 0; j < def->linux->namespaces_len; j++)
            {
              यदि (म_भेद (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  रोक;
                }
            }
          यदि (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              जारी;
            }

          ret = crun_make_error (err, त्रुटि_सं, "setns `%s`", namespaces[i].ns_file);
          गच्छ निकास;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

निकास:
  लागि (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  फर्काऊ ret;
}

पूर्णाङ्क
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      अचर वर्ण *sub_cgroup,
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

  यदि (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "set child subreaper");
    }

  यदि (path_has_dot_dot_component (sub_cgroup))
    फर्काऊ crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  यदि (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free वर्ण *final_cgroup = रिक्त;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "error creating socketpair");

  {
    cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    यदि (get_force_cgroup_v1_annotation (container) == रिक्त)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        यदि (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  स_रखो (&clone3_args, 0, परिमाण (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  यदि (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  अन्यथा
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  यदि (pid > 0 && ! (process->scheduler && process->scheduler->policy && म_भेद (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  /* On errors, fall back to fork().  */
  यदि (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = विभाजन ();
      यदि (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, त्रुटि_सं, "fork");
          गच्छ निकास;
        }
    }

  यदि (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      फर्काऊ join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  यदि (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (लिख (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->सन्देश);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = विभाजन ();
  यदि (UNLIKELY (pid < 0))
    {
      पूर्णाङ्क saved_errno = त्रुटि_सं;
      TEMP_FAILURE_RETRY (लिख (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  यदि (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (लिख (sync_fd, "0", 1));
      यदि (UNLIKELY (ret < 0))
        {
          समाप्त (pid, SIGKILL);
          _safe_exit (निकास_त्रुटि);
        }

      ret = TEMP_FAILURE_RETRY (लिख (sync_fd, &pid, परिमाण (pid)));
      यदि (UNLIKELY (ret < 0))
        {
          समाप्त (pid, SIGKILL);
          _safe_exit (निकास_त्रुटि);
        }

      _safe_exit (निकास_सफल);
    }
  अन्यथा
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free वर्ण *pty = रिक्त;
      पूर्णाङ्क r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (पठ (sync_fd, &r, परिमाण (r)));
      यदि (UNLIKELY (ret != परिमाण (r) || r < 0))
        _safe_exit (निकास_त्रुटि);

      ret = setsid ();
      यदि (ret < 0)
        {
          crun_make_error (err, त्रुटि_सं, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      यदि (terminal_fd)
        {
          cleanup_close पूर्णाङ्क ptmx_fd = -1;

          ret = set_id_init (container, err);
          यदि (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          यदि (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          यदि (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  फर्काऊ 0;

निकास:
  यदि (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (बंद (sync_socket_fd[0]));
  यदि (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (बंद (sync_socket_fd[1]));
  फर्काऊ ret;
}

पूर्णाङ्क
libcrun_linux_container_update (libcrun_container_status_t *status, अचर वर्ण *state_जरा, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

  cgroup_status = libcrun_cgroup_make_status (status);

  फर्काऊ libcrun_update_cgroup_resources (cgroup_status, state_जरा, resources, err);
}

स्थिर पूर्णाङ्क
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, अचर bool छोड़ो, libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

  cgroup_status = libcrun_cgroup_make_status (status);

  फर्काऊ libcrun_cgroup_pause_unpause (cgroup_status, छोड़ो, err);
}

पूर्णाङ्क
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  फर्काऊ libcrun_container_pause_unpause_linux (status, true, err);
}

पूर्णाङ्क
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  फर्काऊ libcrun_container_pause_unpause_linux (status, false, err);
}

पूर्णाङ्क
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  अचिन्हित दीर्घ persona = 0;
  पूर्णाङ्क ret;

  यदि (म_भेद (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  अन्यथा यदि (म_भेद (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  अन्यथा
    फर्काऊ crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, 0, "personality to `%s`", p->domain);

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  माप_प्रकार i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णाङ्क sockfd = -1;

  लागि (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णाङ्क मान = libcrun_find_namespace (def->linux->namespaces[i]->type);
      यदि (UNLIKELY (मान < 0))
        फर्काऊ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      यदि (मान == CLONE_NEWNET && def->linux->namespaces[i]->path == रिक्त)
        {
          configure_network = true;
          रोक;
        }
    }

  यदि (! configure_network)
    फर्काऊ 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  यदि (LIKELY (sockfd >= 0))
    {
      संरचना ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "ioctl SIOCSIFFLAGS");
    }
  अन्यथा
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
      संरचना ifinfomsg सन्देश = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((संरचना nlmsghdr *) buf) = hdr;
      *((संरचना ifinfomsg *) (buf + परिमाण (संरचना nlmsghdr))) = सन्देश;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      यदि (UNLIKELY (sockfd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "socket PF_NETLINK");

      ret = bind (sockfd, (संरचना sockaddr *) &addr, परिमाण (addr));
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, परिमाण (buf), 0, (संरचना sockaddr *) &addr_to, परिमाण (addr_to));
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, परिमाण (buf), 0, रिक्त, रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "recvfrom PF_NETLINK");

      hdr_recv = (संरचना nlmsghdr *) buf;

      यदि (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          संरचना nlmsgerr *err_data = (संरचना nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          यदि (err_data->error < 0)
            फर्काऊ crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  फर्काऊ 0;
}

/* Protection for attacks like CVE-2019-5736.  */
पूर्णाङ्क ensure_cloned_द्विआधारी ();
__attribute__ ((constructor)) स्थिर शून्य
libcrun_rexec (शून्य)
{
  यदि (ensure_cloned_द्विआधारी () < 0)
    {
      ख_लिखो (मानक_त्रुटि, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (निकास_त्रुटि);
    }
}

पूर्णाङ्क
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  फर्काऊ libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

पूर्णाङ्क
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
स्थिर पूर्णाङ्क
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, पूर्णाङ्क सङ्केत, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  यदि (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      /* The pid is not valid anymore, return an error.  */
      यदि (ret == 0)
        {
          त्रुटि_सं = ESRCH;
          फर्काऊ crun_make_error (err, त्रुटि_सं, "kill container");
        }
    }

  ret = समाप्त (status->pid, सङ्केत);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "kill container");
  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_kill_linux (libcrun_container_status_t *status, पूर्णाङ्क सङ्केत, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  यदि (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      यदि (त्रुटि_सं == ENOSYS)
        फर्काऊ libcrun_kill_linux_no_pidfd (status, true, सङ्केत, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      यदि (त्रुटि_सं == ESRCH || त्रुटि_सं == ENOENT || त्रुटि_सं == EINVAL)
        फर्काऊ crun_make_error (err, ESRCH, "process not running");
      फर्काऊ crun_make_error (err, त्रुटि_सं, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* The pid is not valid anymore, return an error.  */
  यदि (ret == 0)
    {
      त्रुटि_सं = ESRCH;
      फर्काऊ crun_make_error (err, त्रुटि_सं, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, सङ्केत, रिक्त, 0);
  यदि (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      यदि (त्रुटि_सं == ENOSYS)
        फर्काऊ libcrun_kill_linux_no_pidfd (status, false, सङ्केत, err);
      फर्काऊ crun_make_error (err, त्रुटि_सं, "send signal to pidfd");
    }

  फर्काऊ 0;
}

स्थिर अचर वर्ण *
libcrun_get_intelrdt_name (अचर वर्ण *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  यदि (def == रिक्त || def->linux == रिक्त || def->linux->intel_rdt == रिक्त || def->linux->intel_rdt->clos_id == रिक्त)
    {
      यदि (explicit)
        *explicit = false;
      फर्काऊ ctr_name;
    }

  यदि (explicit)
    *explicit = true;
  फर्काऊ def->linux->intel_rdt->clos_id;
}

पूर्णाङ्क
libcrun_apply_intelrdt (अचर वर्ण *ctr_name, libcrun_container_t *container, pid_t pid, पूर्णाङ्क actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = रिक्त;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  अचर वर्ण *name;
  पूर्णाङ्क ret;

  यदि (container)
    def = container->container_def;

  यदि (def == रिक्त || def->linux == रिक्त || def->linux->intel_rdt == रिक्त)
    फर्काऊ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  यदि (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      यदि (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          यदि (UNLIKELY (ret < 0))
            गच्छ fail;

          has_monitoring = true;
        }
    }

  यदि (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  यदि (actions & LIBCRUN_INTELRDT_MOVE)
    {
      अचर वर्ण *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : रिक्त;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  फर्काऊ 0;

fail:
  यदि (has_monitoring)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      यदि (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  यदि (created)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      यदि (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  फर्काऊ ret;
}

पूर्णाङ्क
libcrun_destroy_intelrdt (अचर वर्ण *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  अचर वर्ण *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  यदि (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      पूर्णाङ्क ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      यदि (ret < 0)
        फर्काऊ ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  यदि (explicit)
    फर्काऊ 0;

  फर्काऊ resctl_destroy (clos_id, err);
}

पूर्णाङ्क
libcrun_update_intel_rdt (अचर वर्ण *ctr_name, libcrun_container_t *container, अचर वर्ण *l3_cache_schema, अचर वर्ण *mem_bw_schema, वर्ण *अचर *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = रिक्त;
  अचर वर्ण *name;

  यदि (container)
    def = container->container_def;

  यदि (def == रिक्त || def->linux == रिक्त || def->linux->intel_rdt == रिक्त)
    फर्काऊ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, रिक्त);

  फर्काऊ resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
पूर्णाङ्क
libcrun_safe_chdir (अचर वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *buffer = रिक्त;
  पूर्णाङ्क ret;

  ret = स_बदलो (path);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  यदि (ret == 0 || buffer[0] != '/')
    {
      यदि (स_बदलो ("/") < 0)
        {
          /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
        }
      त्रुटि_सं = ENOENT;

      /*
        The kernel prepends the string "(unreachable)" to the path
        when it is not reachable from the current mount namespace.
        Use it to give a better error message.
      */
#define UNREACHABLE "(unreachable)"
#define UNREACHABLE_LEN ((पूर्णाङ्क) परिमाण (UNREACHABLE) - 1)

      यदि ((ret >= UNREACHABLE_LEN) && (स_भेद (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "the working directory is not accessible from the current namespace");

      फर्काऊ crun_make_error (err, त्रुटि_सं, "the current working directory is not an absolute path");
    }
  फर्काऊ 0;
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
  यदि (UNLIKELY (pid < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "vfork");

  यदि (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      यदि (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = स_बदलो ("/");
      यदि (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, त्रुटि_सं, "chdir to `/`");
          _safe_exit (ret);
        }

      ret = callback (arg, err);
      /* On failure (ret < 0) the created error will be
         used by the parent proc */
      _safe_exit (ret);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  फर्काऊ get_process_exit_status (wait_status);
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
  अचर वर्ण *path = रिक्त;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  यदि (def->linux)
    लागि (i = 0; i < def->linux->namespaces_len; i++)
      यदि (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == रिक्त;
          रोक;
        }

  यदि (path == रिक्त)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      यदि (UNLIKELY (expected_fd < 0))
        फर्काऊ expected_fd;
    }
  अन्यथा
    {
      expected_fd = खोलो (path, O_RDONLY | O_CLOEXEC);
      यदि (UNLIKELY (expected_fd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  यदि (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      यदि (UNLIKELY (parent_fd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = ख_स्थिति (userns_fd, &st);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat user namespace");

  ret = ख_स्थिति (expected_fd, &expected_st);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat user namespace");

  यदि (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    फर्काऊ crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  फर्काऊ 0;
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
  यदि (UNLIKELY (mntns < 0))
    फर्काऊ mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (ret == 0)
    फर्काऊ crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  यदि (UNLIKELY (owner < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, पूर्णाङ्क (*callback) (शून्य *, libcrun_error_t *), शून्य *arg, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क mntns_fd = -1;
  cleanup_close पूर्णाङ्क owner_fd = -1;
  पूर्णाङ्क ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ run_callback_in_namespace (mntns_fd, callback, arg, err);
}

संरचना umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  माप_प्रकार len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
स्थिर पूर्णाङ्क
umount_nofollow (अचर वर्ण *destination, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क parentfd = -1;
  cleanup_close पूर्णाङ्क cwdfd = -1;
  वर्ण buffer[PATH_MAX];
  अचर वर्ण *parent, *name;
  वर्ण *last_slash;
  माप_प्रकार len;
  पूर्णाङ्क saved_errno;
  पूर्णाङ्क ret;

  यदि (is_empty_string (destination))
    फर्काऊ crun_make_error (err, 0, "empty destination for mount");

  len = म_माप (destination);
  यदि (UNLIKELY (len >= परिमाण (buffer)))
    फर्काऊ crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  स_नकल (buffer, destination, len + 1);

  जबसम्म (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = म_खोजप (buffer, '/');
  यदि (last_slash == रिक्त)
    {
      parent = ".";
      name = buffer;
    }
  अन्यथा यदि (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  अन्यथा
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  यदि (UNLIKELY (parentfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open parent directory of `%s`", destination);

  cwdfd = खोलो (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  यदि (UNLIKELY (cwdfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open current directory");

  ret = fchdir (parentfd);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = त्रुटि_सं;

  यदि (UNLIKELY (fchdir (cwdfd) < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fchdir to the previous working directory");

  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, saved_errno, "umount `%s`", destination);

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
do_umount_in_a_container (शून्य *arg, libcrun_error_t *err)
{
  संरचना umount_in_a_container_args *args = arg;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  लागि (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  फर्काऊ 0;
}

संरचना mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  संरचना libcrun_fd_map *fds;
  पूर्णाङ्क pidfd;
  पूर्णाङ्क owner_fd;
  माप_प्रकार len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
स्थिर पूर्णाङ्क
open_destination_nofollow (अचर वर्ण *destination, bool dir, libcrun_error_t *err)
{
  अचर uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  पूर्णाङ्क fd;
  पूर्णाङ्क ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  यदि (LIKELY (fd >= 0))
    फर्काऊ fd;

  यदि (त्रुटि_सं != ENOENT)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", destination);

  यदि (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  अन्यथा
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", destination);

  फर्काऊ fd;
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
  माप_प्रकार i;
  पूर्णाङ्क ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  यदि (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "setresuid to `0`");
    }
  अन्यथा यदि (त्रुटि_सं != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      फर्काऊ crun_make_error (err, त्रुटि_सं, "setns to the user namespace of the container");
    }

  लागि (i = 0; i < args->len; i++)
    {
      cleanup_close पूर्णाङ्क dest_fd = -1;
      संरचना stat st;

      ret = ख_स्थिति (args->fds->fds[i], &st);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      यदि (UNLIKELY (dest_fd < 0))
        फर्काऊ dest_fd;
    }

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
do_mount_in_a_container (शून्य *arg, libcrun_error_t *err)
{
  संरचना mount_in_a_container_args *args = arg;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  लागि (i = 0; i < args->len; i++)
    {
      cleanup_close पूर्णाङ्क dest_fd = -1;
      संरचना stat st;

      ret = ख_स्थिति (args->fds->fds[i], &st);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      यदि (UNLIKELY (dest_fd < 0))
        फर्काऊ dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "move mount to `%s`", args->mounts[i]->destination);
    }

  फर्काऊ 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
स्थिर पूर्णाङ्क
send_mounts_from_container_namespaces (libcrun_container_t *container, पूर्णाङ्क pidfd, runtime_spec_schema_defs_mount **mounts,
                                       संरचना libcrun_fd_map *fds, माप_प्रकार len, पूर्णाङ्क flags, पूर्णाङ्क owner_fd, पूर्णाङ्क sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  यदि (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "unshare `CLONE_NEWNS`");
    }
  अन्यथा यदि (त्रुटि_सं != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      फर्काऊ crun_make_error (err, त्रुटि_सं, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = विभाजन ();
  यदि (UNLIKELY (pid < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "fork");

  यदि (pid)
    {
      पूर्णाङ्क wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      यदि (ret > 0 && WIFSTOPPED (wait_status))
        {
          समाप्त (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, रिक्त, 0));
          फर्काऊ crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (निकास_सफल);
    }

  ret = set_id_init (container, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "prctl (PR_SET_DUMPABLE)");

  लागि (i = 0; i < len; i++)
    {
      cleanup_close पूर्णाङ्क fd = -1;

      यदि (fds->fds[i] >= 0)
        जारी;

      fd = open_mount_of_type (container, mounts[i], err);
      यदि (UNLIKELY (fd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  फर्काऊ 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
स्थिर पूर्णाङ्क
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, पूर्णाङ्क *flags, libcrun_error_t *err)
{
  स्थिर अचर संरचना
  {
    अचर वर्ण *name;
    पूर्णाङ्क flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  माप_प्रकार i;

  *flags = 0;

  लागि (i = 0; i < परिमाण (namespaces) / परिमाण (namespaces[0]); i++)
    {
      cleanup_close पूर्णाङ्क self_fd = -1;
      cleanup_close पूर्णाङ्क target_fd = -1;
      संरचना stat self_st, target_st;
      cleanup_free वर्ण *self_path = रिक्त;
      पूर्णाङ्क ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      यदि (UNLIKELY (self_fd < 0))
        फर्काऊ self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      यदि (UNLIKELY (target_fd < 0))
        फर्काऊ target_fd;

      ret = ख_स्थिति (self_fd, &self_st);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat `%s`", namespaces[i].name);

      ret = ख_स्थिति (target_fd, &target_st);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "fstat `%s`", namespaces[i].name);

      यदि (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  फर्काऊ 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
स्थिर पूर्णाङ्क
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, पूर्णाङ्क owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, संरचना libcrun_fd_map *fds, माप_प्रकार len,
                                     libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क sync_fd = -1;
  cleanup_close पूर्णाङ्क pidfd = -1;
  पूर्णाङ्क sync_socket_fd[2];
  bool needed = false;
  पूर्णाङ्क flags;
  pid_t pid;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  लागि (i = 0; i < len; i++)
    {
      यदि (fds->fds[i] >= 0)
        जारी;

      यदि (mounts[i]->type == रिक्त)
        फर्काऊ crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  यदि (! needed)
    फर्काऊ 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  यदि (UNLIKELY (pidfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (ret == 0)
    फर्काऊ crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = विभाजन ();
  यदि (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, त्रुटि_सं, "fork");
      TEMP_FAILURE_RETRY (बंद (sync_socket_fd[1]));
      फर्काऊ ret;
    }

  यदि (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      यदि (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (निकास_सफल);
    }

  TEMP_FAILURE_RETRY (बंद (sync_socket_fd[1]));

  ret = 0;
  लागि (i = 0; i < len; i++)
    {
      यदि (fds->fds[i] >= 0)
        जारी;

      ret = receive_fd_from_socket (sync_fd, err);
      यदि (UNLIKELY (ret < 0))
        {
          cleanup_free वर्ण *err_str = रिक्त;
          पूर्णाङ्क err_code = 0;

          यदि (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          अन्यथा
            ret = crun_error_wrap (err, "receive mount fd");
          रोक;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, रिक्त, 0));

  फर्काऊ ret;
}

स्थिर अचिन्हित दीर्घ
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free वर्ण *data = रिक्त;
  अचिन्हित दीर्घ extra_flags = 0;
  अचिन्हित दीर्घ flags = 0;
  माप_प्रकार j;

  यदि (mount->options == रिक्त)
    फर्काऊ get_default_flags (container, mount->destination, &data);

  लागि (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  फर्काऊ flags;
}

पूर्णाङ्क
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, माप_प्रकार len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  संरचना mount_in_a_container_args args;
  cleanup_close_map संरचना libcrun_fd_map *fds = रिक्त;
  cleanup_close पूर्णाङ्क pidfd = -1;
  cleanup_close पूर्णाङ्क mntns_fd = -1;
  cleanup_close पूर्णाङ्क owner_fd = -1;
  pid_t pid = status->pid;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  fds = make_libcrun_fd_map (len);

  लागि (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), रिक्त, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      यदि (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          अचिन्हित दीर्घ propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          यदि (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          यदि (UNLIKELY (fds->fds[i] < 0))
            फर्काऊ fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  लागि (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      अचिन्हित दीर्घ flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  यदि (UNLIKELY (ret > 0))
    फर्काऊ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

पूर्णाङ्क
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, माप_प्रकार len, libcrun_error_t *err)
{
  संरचना umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  फर्काऊ run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

संरचना restrict_mounts_in_a_container_args
{
  संरचना libcrun_mount_flags_s *mounts;
  माप_प्रकार len;
  पूर्णाङ्क rootfd;
};

स्थिर पूर्णाङ्क
do_restrict_mounts_in_a_container (शून्य *arg, libcrun_error_t *err)
{
  संरचना restrict_mounts_in_a_container_args *args = arg;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  यदि (LIKELY (ret == 0))
    ret = chroot (".");
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "chroot to the container root");

  लागि (i = 0; i < args->len; i++)
    {
      अचिन्हित दीर्घ flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      अचर वर्ण *target = consume_slashes (args->mounts[i].target);
      cleanup_close पूर्णाङ्क fd = -1;
      संरचना statfs sfs;
      अचिन्हित दीर्घ current_flags;

      यदि (flags == 0)
        जारी;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      यदि (UNLIKELY (fd < 0))
        {
          यदि (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              जारी;
            }
          फर्काऊ fd;
        }

      ret = fstatfs (fd, &sfs);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      यदि ((current_flags & flags) == flags)
        जारी;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      यदि (LIKELY (ret == 0))
        जारी;

      यदि (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        फर्काऊ ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (रिक्त, target, रिक्त, MS_REMOUNT | MS_BIND | flags | current_flags, रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "remount `/%s`", target);
    }

  फर्काऊ 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
पूर्णाङ्क
libcrun_restrict_mounts_in_container (pid_t pid, संरचना libcrun_mount_flags_s *mounts, माप_प्रकार len, libcrun_error_t *err)
{
  संरचना restrict_mounts_in_a_container_args args;
  cleanup_close पूर्णाङ्क mntns_fd = -1;
  cleanup_close पूर्णाङ्क rootfd = -1;
  वर्ण path[64];
  पूर्णाङ्क ret;

  snprintf (path, परिमाण (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = खोलो (path, O_RDONLY | O_CLOEXEC);
  यदि (UNLIKELY (mntns_fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", path);

  snprintf (path, परिमाण (path), "/proc/%d/root", pid);
  rootfd = खोलो (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  यदि (UNLIKELY (rootfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  यदि (UNLIKELY (ret > 0))
    फर्काऊ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  फर्काऊ ret;
}

पूर्णाङ्क
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णाङ्क netns_fd = -1;
  माप_प्रकार i;
  पूर्णाङ्क ret;

  यदि (def == रिक्त || def->linux == रिक्त || def->linux->net_devices == रिक्त)
    फर्काऊ 0;

  cleanup_free वर्ण *ns_path = रिक्त;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  यदि (UNLIKELY (netns_fd < 0))
    फर्काऊ netns_fd;

  लागि (i = 0; i < def->linux->net_devices->len; i++)
    {
      अचर वर्ण *new_name = def->linux->net_devices->मानहरू[i]->name ?: def->linux->net_devices->साँचोहरू[i];

      ret = move_network_device (def->linux->net_devices->साँचोहरू[i], new_name, netns_fd, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  फर्काऊ 0;
}
