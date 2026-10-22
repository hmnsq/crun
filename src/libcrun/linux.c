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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "linux.h"
#समावेश "utils.h"
#समावेश "status.h"
#समावेश <string.h>
#समावेश <sched.h>
#समावेश <fcntl.h>
#समावेश <unistd.h>
#समावेश <sys/mount.h>
#अगर_घोषित HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#पूर्ण_अगर
#अगर defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#पूर्ण_अगर

#समावेश <sys/prctl.h>
#अगर_घोषित HAVE_CAP
#  include <sys/capability.h>
#पूर्ण_अगर
#समावेश <sys/time.h>
#समावेश <sys/resource.h>
#समावेश <sys/sysmacros.h>
#समावेश <sys/types.h>
#समावेश <sys/stat.h>
#समावेश <grp.h>
#समावेश <signal.h>
#समावेश "terminal.h"
#समावेश "cgroup.h"
#समावेश "cgroup-utils.h"
#समावेश "status.h"
#समावेश "criu.h"
#समावेश "scheduler.h"
#समावेश "intelrdt.h"
#समावेश "io_priority.h"
#समावेश "net_device.h"

#समावेश <sys/socket.h>
#समावेश <libgen.h>
#समावेश <sys/wait.h>
#समावेश <sys/vfs.h>
#समावेश <sys/statvfs.h>
#समावेश <limits.h>
#समावेश <inttypes.h>
#समावेश <sys/personality.h>
#समावेश <net/if.h>
#समावेश <sys/xattr.h>
#समावेश <linux/netlink.h>
#समावेश <linux/rtnetlink.h>
#समावेश <sched.h>
#समावेश <linux/sched.h>
#समावेश <linux/magic.h>
#समावेश <linux/nsfs.h>

#समावेश <json-c/json.h>
#समावेश <ocispec/json_common.h>

#समावेश "mount_flags.h"
#समावेश "syscalls.h"

#अगर_अघोषित RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#पूर्ण_अगर

#अगर_अघोषित OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#पूर्ण_अगर

#अगर_अघोषित OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#पूर्ण_अगर

#अगर_अघोषित OPEN_वृक्ष_NAMESPACE
#  define OPEN_वृक्ष_NAMESPACE 2
#पूर्ण_अगर

#अगर_अघोषित MOVE_MOUNT_F_EMPTY_PATH
#  define MOVE_MOUNT_F_EMPTY_PATH 0x00000004
#पूर्ण_अगर

#अगर_अघोषित MOVE_MOUNT_T_EMPTY_PATH
#  define MOVE_MOUNT_T_EMPTY_PATH 0x00000040
#पूर्ण_अगर

#अगर_अघोषित FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#पूर्ण_अगर

#अगर_अघोषित FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#पूर्ण_अगर

#अगर_अघोषित FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#पूर्ण_अगर

#घोषणा ALL_PROPAGATIONS_NO_REC (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE)
#घोषणा ALL_PROPAGATIONS (MS_REC | ALL_PROPAGATIONS_NO_REC)

/* The per-mount flags a remount clears unless they are explicitly set.  */
#घोषणा REMOUNT_CLEAR_FLAGS (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW)

#अगर_अघोषित ST_NOSYMFOLLOW
#  define ST_NOSYMFOLLOW 0x2000
#पूर्ण_अगर

#घोषणा MOUNT_ATIME_FLAGS (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME)

/* Convert the ST_ flags reported by statfs(2) to the MS_ flags of mount(2).  */
अटल अचिन्हित दीर्घ
mount_flags_from_statfs (अचिन्हित दीर्घ f_flags)
{
  अचिन्हित दीर्घ flags = 0;

  अगर (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  अगर (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  अगर (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  अगर (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  अगर (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  अगर (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  अगर (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  अगर (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  अगर (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  वापस flags;
}

काष्ठा remount_s
{
  काष्ठा remount_s *अग्र;
  अक्षर *target;
  पूर्णांक targetfd;
  अचिन्हित दीर्घ flags;
  अक्षर *तथ्य;
};

काष्ठा private_तथ्य_s
{
  काष्ठा remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  पूर्णांक unshare_flags;
  पूर्णांक unshare_cgroupns;

  अक्षर *host_notify_socket_path;
  अक्षर *container_notify_socket_path;
  अक्षर *unified_cgroup_path;

  bool mount_dev_from_host;
  अचिन्हित दीर्घ rootfs_propagation;
  bool deny_setgroups;

  पूर्णांक procfd;

  स्थिर अक्षर *rootfs;
  पूर्णांक rootfsfd;

  पूर्णांक notify_socket_वृक्ष_fd;
  पूर्णांक old_मूल_fd;

  काष्ठा libcrun_fd_map *mount_fds;
  काष्ठा libcrun_fd_map *dev_fds;
  काष्ठा libcrun_fd_map *needed_devs_fds;

  अक्षर **copy_symlink_targets;
  माप_प्रकार n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  अक्षर *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  पूर्णांक maskdir_fd;
  अक्षर *maskdir_proc_path;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_pivot;
  bool no_pivot;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_मूल_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  अक्षर *host_rootfs;
};

काष्ठा linux_namespace_s
{
  स्थिर अक्षर *name;
  स्थिर अक्षर *ns_file;
  पूर्णांक मूल्य;
};

अटल व्योम
cleanup_private_तथ्य (व्योम *private_तथ्य)
{
  काष्ठा private_तथ्य_s *p = private_तथ्य;

  अगर (p->rootfsfd >= 0)
    अस्थायी_FAILURE_RETRY (बंद (p->rootfsfd));
  अगर (p->procfd >= 0)
    अस्थायी_FAILURE_RETRY (बंद (p->procfd));
  अगर (p->maskdir_fd >= 0)
    अस्थायी_FAILURE_RETRY (बंद (p->maskdir_fd));
  अगर (p->old_मूल_fd >= 0)
    अस्थायी_FAILURE_RETRY (बंद (p->old_मूल_fd));
  अगर (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  अगर (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  अगर (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  अगर (p->copy_symlink_targets)
    {
      माप_प्रकार i;
      क्रम (i = 0; i < p->n_copy_symlink_targets; i++)
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

अटल काष्ठा private_तथ्य_s *
get_private_तथ्य (काष्ठा libcrun_container_s *container)
{
  अगर (container->private_तथ्य == शून्य)
    {
      काष्ठा private_तथ्य_s *p = xmalloc0 (माप (*p));
      container->private_तथ्य = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_वृक्ष_fd = -1;
      p->old_मूल_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_तथ्य = cleanup_private_तथ्य;
    }
  वापस container->private_तथ्य;
}

#अगर_अघोषित CLONE_NEWTIME
#  define CLONE_NEWTIME 0x00000080
#पूर्ण_अगर
#अगर_अघोषित CLONE_NEWCGROUP
#  define CLONE_NEWCGROUP 0x02000000
#पूर्ण_अगर
#अगर_अघोषित AT_RECURSIVE
#  define AT_RECURSIVE 0x8000
#पूर्ण_अगर

अटल काष्ठा linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { शून्य, शून्य, 0 } };

अटल पूर्णांक
get_and_reset (पूर्णांक *old)
{
  पूर्णांक tmp = *old;
  *old = -1;
  वापस tmp;
}

पूर्णांक
libcrun_find_namespace (स्थिर अक्षर *name)
{
  काष्ठा linux_namespace_s *it;
  क्रम (it = namespaces; it->name; it++)
    अगर (म_भेद (it->name, name) == 0)
      वापस it->मूल्य;
  वापस -1;
}

अटल पूर्णांक
syscall_keyctl_join (स्थिर अक्षर *name)
{
#घोषणा KEYCTL_JOIN_SESSION_KEYRING 0x1
  वापस (पूर्णांक) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

अटल पूर्णांक
syscall_pidfd_open (pid_t pid, अचिन्हित पूर्णांक flags)
{
#अगर defined __NR_pidfd_open
  वापस (पूर्णांक) syscall (__NR_pidfd_open, pid, flags);
#अन्यथा
  (व्योम) pid;
  (व्योम) flags;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

अटल पूर्णांक
syscall_pidfd_send_signal (पूर्णांक pidfd, पूर्णांक sig, siginfo_t *info, अचिन्हित पूर्णांक flags)
{
#अगर defined __NR_pidfd_send_signal
  वापस (पूर्णांक) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#अन्यथा
  (व्योम) pidfd;
  (व्योम) sig;
  (व्योम) info;
  (व्योम) flags;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
अटल uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  अगर (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  अगर (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  अगर (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  अगर (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  अगर (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  अगर (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  अगर (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  अगर (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  वापस attr;
}

अटल पूर्णांक
do_mount_setattr (bool recursive, स्थिर अक्षर *target, पूर्णांक targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  काष्ठा mount_attr_s attr = {
    0,
  };
  पूर्णांक ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  अगर (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      अगर (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      अन्यथा अगर (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      अन्यथा
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      अगर (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  अन्यथा अगर (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "mount_setattr `/%s`", target);

  वापस 0;
}

पूर्णांक
get_bind_mount (पूर्णांक dirfd, स्थिर अक्षर *src, bool recursive, bool rdonly, bool nofollow, अचिन्हित दीर्घ propagation, libcrun_error_t *err)
{
  cleanup_close पूर्णांक open_वृक्ष_fd = -1;
  काष्ठा mount_attr_s attr = {
    0,
  };
  पूर्णांक recursive_flag = (recursive ? AT_RECURSIVE : 0);
  पूर्णांक ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  अगर (UNLIKELY (src == शून्य))
    वापस crun_make_error (err, EINVAL, "mount source is not specified");

  अगर (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  अगर (dirfd < 0)
    dirfd = AT_FDCWD;

  त्रुटि_सं = 0;
  open_वृक्ष_fd = syscall_open_वृक्ष (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  अगर (UNLIKELY (open_वृक्ष_fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_वृक्ष_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "mount_setattr `%s`", src);

  वापस get_and_reset (&open_वृक्ष_fd);
}

पूर्णांक
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, अक्षर *option, अक्षर **out, माप_प्रकार *len, libcrun_error_t *err)
{
  माप_प्रकार written = 0, allocated = 256;
  cleanup_free अक्षर *mappings = शून्य;
  स्थिर अक्षर *it;
  पूर्णांक ret;

  mappings = xmalloc (allocated);

  क्रम (it = option; *it;)
    {
      bool relative = false;
      दीर्घ मूल्य[3];
      माप_प्रकार i;

      अगर (*it == '\0')
        अवरोध;

      अगर (*it == '#')
        it++;

      अगर (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      क्रम (i = 0; i < 3; i++)
        {
          अक्षर *endptr = शून्य;

          अगर (i > 0 && *it == '-')
            it++;

          अगर (*it == '\0')
            वापस crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          त्रुटि_सं = 0;
          मूल्य[i] = म_से_दीर्घ (it, &endptr, 10);
          अगर (त्रुटि_सं || endptr == it)
            वापस crun_make_error (err, त्रुटि_सं, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      अगर (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          माप_प्रकार mappings_len;

          अगर (def == शून्य
              || def->linux == शून्य
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            वापस crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          क्रम (i = 0; i < mappings_len; i++)
            अगर (मूल्य[1] >= (int64_t) mappings[i]->container_id
                && मूल्य[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->आकार)
              अवरोध;

          अगर (i == mappings_len)
            वापस crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          मूल्य[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      अगर (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", मूल्य[0], मूल्य[1], मूल्य[2]);
      अगर (UNLIKELY (ret >= (पूर्णांक) (allocated - written)))
        वापस crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = शून्य;

  वापस 0;
}

अटल पूर्णांक
format_mount_mappings (अक्षर **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       माप_प्रकार mappings_len,
                       माप_प्रकार *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  स्थिर माप_प्रकार max_len_mapping = 64;
  cleanup_free अक्षर *ret = शून्य;
  माप_प्रकार s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  क्रम (s = 0; s < mappings_len; s++)
    {
      माप_प्रकार len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->आकार);
      अगर (UNLIKELY (len >= max_len_mapping))
        वापस crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = शून्य;
  वापस 0;
}

अटल पूर्णांक
format_mount_mapping (अक्षर **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t आकार, माप_प्रकार *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .आकार = आकार,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    शून्य,
  };

  वापस format_mount_mappings (ret, mappings, 1, written, err);
}

अटल bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  माप_प्रकार s;

  अगर (def->linux == शून्य)
    वापस mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  अगर (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    वापस false;

  अगर (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    वापस false;

  क्रम (s = 0; s < mnt->uid_mappings_len; s++)
    {
      अगर (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        वापस false;
      अगर (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        वापस false;
      अगर (mnt->uid_mappings[s]->आकार != def->linux->uid_mappings[s]->आकार)
        वापस false;
    }

  क्रम (s = 0; s < mnt->gid_mappings_len; s++)
    {
      अगर (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        वापस false;
      अगर (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        वापस false;
      अगर (mnt->gid_mappings[s]->आकार != def->linux->gid_mappings[s]->आकार)
        वापस false;
    }

  वापस true;
}

अटल pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        स्थिर अक्षर *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != शून्य;

  अगर (! need_new_userns)
    वापस 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, शून्य);
  अगर (UNLIKELY (pid < 0))
    वापस crun_make_error (err, त्रुटि_सं, "clone");

  अगर (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      जबतक (1)
        छोड़ो ();
      _safe_exit (निकास_सफल);
    }

  अगर (mnt->uid_mappings_len)
    {
      cleanup_free अक्षर *uid_map = शून्य;
      cleanup_free अक्षर *gid_map = शून्य;
      माप_प्रकार written = 0;
      पूर्णांक ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      cleanup_close पूर्णांक fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      अगर (UNLIKELY (fd < 0))
        वापस fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      अगर (UNLIKELY (fd < 0))
        वापस fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      cleanup_free अक्षर *dup_options = शून्य;
      अक्षर *option, *saveptr = शून्य;

      अगर (! options)
        वापस crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      क्रम (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (शून्य, ";", &saveptr))
        {
          cleanup_free अक्षर *mappings = शून्य;
          cleanup_close पूर्णांक map_fd = -1;
          bool is_uids = false;
          माप_प्रकार len = 0;
          पूर्णांक ret;

          अगर (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              अगर (UNLIKELY (map_fd < 0))
                वापस map_fd;
            }
          अन्यथा अगर (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              अगर (UNLIKELY (map_fd < 0))
                वापस map_fd;
            }
          अन्यथा
            वापस crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  वापस 0;
}

पूर्णांक
libcrun_create_keyring (libcrun_container_t *container, स्थिर अक्षर *name, स्थिर अक्षर *label, libcrun_error_t *err)
{
  cleanup_close पूर्णांक labelfd = -1;
  bool label_set = false;
  पूर्णांक ret;

  अगर (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      अगर (UNLIKELY (labelfd < 0))
        {
          अगर (crun_error_get_errno (err) != ENOENT)
            वापस labelfd;

          crun_error_release (err);
        }

      अगर (labelfd >= 0)
        {
          ret = ग_लिखो (labelfd, label, म_माप (label));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          जाओ out;
        }
      ret = crun_make_error (err, त्रुटि_सं, "join keyctl `%s`", name ? name : "");
      जाओ out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  अगर (label_set && ग_लिखो (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  वापस ret;
}

अटल व्योम
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  अगर (def->process && def->process->user)
    {
      अगर (def->process->user->uid)
        *uid = def->process->user->uid;
      अगर (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

अटल अचिन्हित दीर्घ
get_mount_flags (स्थिर अक्षर *name, पूर्णांक current_flags, पूर्णांक *found, अचिन्हित दीर्घ *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  स्थिर काष्ठा propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  अगर (found)
    *found = prop ? 1 : 0;

  अगर (prop == शून्य)
    वापस 0;

  अगर (prop->extra_flags & OPTION_RECURSIVE)
    {
      अगर (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      अगर (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  अगर (extra_flags)
    *extra_flags |= prop->extra_flags;

  अगर (prop->clear)
    वापस current_flags & ~prop->flags;

  वापस current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
अटल bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  माप_प्रकार i;

  क्रम (i = 0; i < mount->options_len; i++)
    {
      स्थिर काष्ठा propagation_flags_s *prop;

      अगर (mount->options[i] == शून्य)
        जारी;

      prop = libcrun_str2mount_flags (mount->options[i]);
      अगर (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        वापस true;
    }

  वापस false;
}

अटल अचिन्हित दीर्घ
get_mount_flags_or_option (स्थिर अक्षर *name, पूर्णांक current_flags, अचिन्हित दीर्घ *extra_flags, अक्षर **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  पूर्णांक found;
  __attribute__ ((unused)) cleanup_free अक्षर *पूर्व = शून्य;
  अचिन्हित दीर्घ flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  अगर (name == शून्य)
    वापस 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  अगर (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      स्थिर अक्षर *तथ्य_flag = शून्य;
      अगर ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        तथ्य_flag = "sync";
      अन्यथा अगर ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        तथ्य_flag = "dirsync";

      अगर (तथ्य_flag)
        {
          पूर्व = *option;
          अगर (*option && **option)
            xasprintf (option, "%s,%s", *option, तथ्य_flag);
          अन्यथा
            *option = xstrdup (तथ्य_flag);
        }
      वापस flags;
    }

  पूर्व = *option;
  अगर (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  अन्यथा
    *option = xstrdup (name);

  वापस 0;
}

पूर्णांक
pivot_मूल (स्थिर अक्षर *new_मूल, स्थिर अक्षर *put_old)
{
  वापस syscall (__NR_pivot_root, new_मूल, put_old);
}

अटल व्योम
free_remount (काष्ठा remount_s *r)
{
  अगर (r == शून्य)
    वापस;
  अगर (r->targetfd >= 0)
    बंद (r->targetfd);
  मुक्त (r->तथ्य);
  मुक्त (r->target);
  मुक्त (r);
}

अटल काष्ठा remount_s *
make_remount (पूर्णांक targetfd, स्थिर अक्षर *target, अचिन्हित दीर्घ flags, स्थिर अक्षर *तथ्य, काष्ठा remount_s *अग्र)
{
  काष्ठा remount_s *ret = xmalloc (माप (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->तथ्य = तथ्य ? xstrdup (तथ्य) : शून्य;
  ret->अग्र = अग्र;
  ret->targetfd = targetfd;
  वापस ret;
}

अटल पूर्णांक
do_remount (पूर्णांक targetfd, स्थिर अक्षर *target, अचिन्हित दीर्घ flags, स्थिर अक्षर *तथ्य, libcrun_error_t *err)
{
  proc_fd_path_t target_बफर;
  पूर्णांक ret;
  स्थिर अक्षर *real_target = target;

  अगर (targetfd >= 0)
    {
      अचिन्हित दीर्घ set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      अचिन्हित दीर्घ clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      अगर (LIKELY (ret == 0))
        वापस 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_बफर, targetfd);
      real_target = target_बफर;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  अगर (flags & (MS_REMOUNT | MS_RDONLY))
    तथ्य = शून्य;

  ret = mount (शून्य, real_target, शून्य, flags, तथ्य);
  अगर (UNLIKELY (ret < 0))
    {
      अचिन्हित दीर्घ current_flags, remount_flags;
      काष्ठा statfs sfs;

      ret = statfs (real_target, &sfs);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      अगर (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      अगर ((flags | remount_flags) != flags)
        {
          ret = mount (शून्य, real_target, शून्य, flags | remount_flags, तथ्य);
          अगर (LIKELY (ret == 0))
            वापस 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          अगर (current_flags & MS_RDONLY)
            ret = mount (शून्य, real_target, शून्य, flags | remount_flags | MS_RDONLY, तथ्य);
        }
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "remount `%s`", target);
    }
  वापस 0;
}

अटल पूर्णांक
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret = 0;
  काष्ठा remount_s *r = get_private_तथ्य (container)->remounts;
  जबतक (r)
    {
      काष्ठा remount_s *अग्र = r->अग्र;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      अगर (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          अचिन्हित दीर्घ set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          अचिन्हित दीर्घ clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          अगर (LIKELY (ret == 0))
            {
              free_remount (r);
              r = अग्र;
              जारी;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->तथ्य, err);
      अगर (UNLIKELY (ret < 0))
        जाओ cleanup;

      free_remount (r);
      r = अग्र;
    }

cleanup:
  जबतक (r)
    {
      काष्ठा remount_s *अग्र = r->अग्र;
      free_remount (r);
      r = अग्र;
    }

  get_private_तथ्य (container)->remounts = शून्य;
  वापस ret;
}

अटल पूर्णांक
open_mount_target (libcrun_container_t *container, स्थिर अक्षर *target_rel, libcrun_error_t *err)
{
  स्थिर अक्षर *rootfs = get_private_तथ्य (container)->rootfs;
  पूर्णांक rootfsfd = get_private_तथ्य (container)->rootfsfd;

  अगर (rootfsfd < 0)
    वापस crun_make_error (err, 0, "invalid rootfs state");

  वापस safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
अटल पूर्णांक
fsopen_mount (स्थिर अक्षर *type, स्थिर अक्षर *source_name, स्थिर अक्षर *labeltype, स्थिर अक्षर *label, स्थिर अक्षर *तथ्य)
{
#अगर_घोषित HAVE_NEW_MOUNT_API
  cleanup_close पूर्णांक fsfd = -1;
  पूर्णांक ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  अगर (UNLIKELY (fsfd < 0))
    वापस fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (व्योम) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  अगर (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (तथ्य && तथ्य[0])
    {
      cleanup_free अक्षर *तथ्य_copy = xstrdup (तथ्य);
      अक्षर *saveptr = शून्य;
      अक्षर *token;

      क्रम (token = strtok_r (तथ्य_copy, ",", &saveptr); token; token = strtok_r (शून्य, ",", &saveptr))
        {
          अक्षर *eq = म_अक्षर (token, '=');
          अगर (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          अन्यथा
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, शून्य, 0);

          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, शून्य, शून्य, 0);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#अन्यथा
  (व्योम) type;
  (व्योम) source_name;
  (व्योम) तथ्य;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

अटल पूर्णांक
get_procfd (काष्ठा private_तथ्य_s *तथ्य, libcrun_error_t *err)
{
  पूर्णांक fd;

  अगर (तथ्य->procfd >= 0)
    वापस तथ्य->procfd;

  fd = fsopen_mount ("proc", शून्य, शून्य, शून्य, शून्य);
  अगर (fd < 0)
    fd = खोलो ("/proc", O_DIRECTORY | O_CLOEXEC);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `/proc`");

  तथ्य->procfd = fd;
  वापस fd;
}

अटल पूर्णांक
get_old_मूल_fd (काष्ठा private_तथ्य_s *तथ्य)
{
  अगर (तथ्य->old_मूल_fd >= 0)
    वापस तथ्य->old_मूल_fd;

  तथ्य->old_मूल_fd = खोलो ("/", O_PATH | O_CLOEXEC);
  वापस तथ्य->old_मूल_fd;
}

अटल पूर्णांक
fs_move_mount_to (पूर्णांक fd, पूर्णांक dirfd, स्थिर अक्षर *name)
{
#अगर_घोषित HAVE_NEW_MOUNT_API
  अगर (name)
    वापस syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  वापस syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#अन्यथा
  (व्योम) fd;
  (व्योम) dirfd;
  (व्योम) name;
  (व्योम) syscall_move_mount;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

क्रमागत
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

अटल पूर्णांक do_mount (libcrun_container_t *container, स्थिर अक्षर *source, पूर्णांक targetfd,
                     स्थिर अक्षर *target, स्थिर अक्षर *fstype, अचिन्हित दीर्घ mountflags,
                     स्थिर व्योम *तथ्य, पूर्णांक extra_flags, libcrun_error_t *err);

अटल bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, स्थिर अक्षर *option)
{
  माप_प्रकार i;
  क्रम (i = 0; i < mnt->options_len; i++)
    अगर (mnt->options[i] != शून्य && म_भेद (mnt->options[i], option) == 0)
      वापस true;
  वापस false;
}

अटल runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, स्थिर अक्षर *destination)
{
  माप_प्रकार i;
  runtime_spec_schema_config_schema *def = container->container_def;

  क्रम (i = 0; i < def->mounts_len; i++)
    {
      अगर (म_भेद (def->mounts[i]->destination, destination) == 0)
        वापस def->mounts[i];
    }
  वापस शून्य;
}

अटल व्योम
warn_tmpfs_fallback_once (काष्ठा private_तथ्य_s *private_तथ्य, स्थिर अक्षर *reason)
{
  अगर (! private_तथ्य->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_तथ्य->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
अटल पूर्णांक
get_shared_empty_dir_cached (libcrun_container_t *container, अक्षर **proc_fd_path, libcrun_error_t *err)
{
  काष्ठा private_तथ्य_s *private_तथ्य = get_private_तथ्य (container);
  cleanup_close पूर्णांक fd = -1;
  cleanup_free अक्षर *empty_dir_path = शून्य;
  proc_fd_path_t fd_path;
  पूर्णांक ret;

  /* Fast path: return cached proc fd path if already set up */
  अगर (private_तथ्य->maskdir_proc_path != शून्य)
    {
      अगर (proc_fd_path)
        *proc_fd_path = private_तथ्य->maskdir_proc_path;
      वापस 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_मूल, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  अगर (fd < 0)
    वापस fd;

  get_self_fd_path (fd_path, fd);
  private_तथ्य->maskdir_proc_path = xstrdup (fd_path);

  private_तथ्य->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  अगर (proc_fd_path)
    *proc_fd_path = private_तथ्य->maskdir_proc_path;
  वापस 0;
}

अटल bool
is_dev_null (स्थिर काष्ठा stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  वापस S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
अटल पूर्णांक
ensure_dev_null (पूर्णांक fd, libcrun_error_t *err)
{
  काष्ठा stat st;

  अगर (UNLIKELY (ख_स्थिति (fd, &st) < 0))
    वापस crun_make_error (err, त्रुटि_सं, "stat `/dev/null`");

  अगर (UNLIKELY (! is_dev_null (&st)))
    वापस crun_make_error (err, 0, "`/dev/null` is not the null device");

  वापस 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
अटल bool
is_empty_ro_tmpfs (पूर्णांक fd)
{
  काष्ठा statfs sfs;

  अगर (fstatfs (fd, &sfs) < 0)
    वापस false;

  वापस sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

अटल पूर्णांक
mount_masked_dir (libcrun_container_t *container, पूर्णांक pathfd, स्थिर अक्षर *rel_path, libcrun_error_t *err)
{
  काष्ठा private_तथ्य_s *private_तथ्य = get_private_तथ्य (container);
  cleanup_close पूर्णांक newfd = -1;
  libcrun_error_t tmp_err = शून्य;
  पूर्णांक ret;

  अगर (private_तथ्य->maskdir_fd < 0)
    {
      अगर (private_तथ्य->maskdir_bind_failed)
        जाओ fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, शून्य, &tmp_err);
      अगर (ret < 0)
        {
          private_तथ्य->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_तथ्य, tmp_err->संदेश);
          crun_error_release (&tmp_err);
          जाओ fallback_to_tmpfs;
        }
    }

  अगर (private_तथ्य->maskdir_fd >= 0)
    {
      पूर्णांक rootfsfd = private_तथ्य->rootfsfd;
      काष्ठा stat before, after;

      ret = ख_स्थिति (private_तथ्य->maskdir_fd, &before);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_तथ्य->maskdir_fd, pathfd, शून्य);
      अगर (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          अगर (UNLIKELY (newfd < 0))
            वापस newfd;

          ret = ख_स्थिति (newfd, &after);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "fstat masked dir `%s`", rel_path);

          अगर (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            वापस crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          अस्थायी_FAILURE_RETRY (बंद (private_तथ्य->maskdir_fd));
          private_तथ्य->maskdir_fd = newfd;
          newfd = -1;

          वापस 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, त्रुटि_सं, "move mount masked dir `%s`", rel_path);

      अस्थायी_FAILURE_RETRY (बंद (private_तथ्य->maskdir_fd));
      private_तथ्य->maskdir_fd = -1;
      private_तथ्य->maskdir_move_failed = true;
    }

  private_तथ्य->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_तथ्य, tmp_err ? tmp_err->संदेश : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  अगर (! private_तथ्य->maskdir_move_failed)
    {
      ret = get_bind_mount (private_तथ्य->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      अगर (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_तथ्य->maskdir_fd = ret;
      अन्यथा
        {
          अगर (ret >= 0)
            अस्थायी_FAILURE_RETRY (बंद (ret));
          private_तथ्य->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  वापस 0;
}

अटल पूर्णांक
do_masked_or_readonly_path (libcrun_container_t *container, स्थिर अक्षर *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  अचिन्हित दीर्घ mount_flags = 0;
  स्थिर अक्षर *rootfs = get_private_तथ्य (container)->rootfs;
  cleanup_close पूर्णांक pathfd = -1;
  काष्ठा statfs sfs;
  पूर्णांक ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  अगर (UNLIKELY (rel_path == शून्य))
    वापस crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  अगर (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_तथ्य (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  अगर (UNLIKELY (pathfd < 0))
    {
      त्रुटि_सं = crun_error_get_errno (err);
      अगर (त्रुटि_सं != ENOENT && त्रुटि_सं != EACCES)
        वापस pathfd;

      अगर (त्रुटि_सं == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      वापस 0;
    }

  अगर (readonly)
    {
      पूर्णांक procfd = get_procfd (get_private_तथ्य (container), err);
      proc_fd_path_t source_बफर;
      cleanup_close पूर्णांक mountfd = -1;

      अगर (UNLIKELY (procfd < 0))
        वापस procfd;

      get_self_fd_path (source_बफर, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      अगर (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_बफर, true, true, false, MS_PRIVATE, err);
          अगर (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, शून्य);
          अन्यथा
            ret = -1;

          अगर (ret < 0)
            crun_error_release (err);
        }

      अगर (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          अगर (keep_flags)
            {
              cleanup_close पूर्णांक sfd = openat (procfd, source_बफर, O_PATH | O_CLOEXEC);
              अगर (UNLIKELY (sfd < 0))
                वापस crun_make_error (err, त्रुटि_सं, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              अगर (UNLIKELY (ret < 0))
                वापस crun_make_error (err, त्रुटि_सं, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_बफर, true, true, false, MS_PRIVATE, err);
          अगर (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              अगर (UNLIKELY (ret < 0))
                वापस ret;

              ret = fs_move_mount_to (mountfd, pathfd, शून्य);
              अगर (UNLIKELY (ret < 0))
                वापस crun_make_error (err, त्रुटि_सं, "move mount for readonly path `%s`", rel_path);
            }
          अन्यथा
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, शून्य, mount_flags, शून्य,
                              LABEL_NONE, err);
              अगर (UNLIKELY (ret < 0))
                वापस ret;
            }
        }
    }
  अन्यथा
    {
      ret = get_file_type_fd (pathfd, &mode);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "cannot stat `%s`", rel_path);

      अगर ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      अन्यथा
        {
          cleanup_close पूर्णांक nullfd = -1;
          cleanup_close पूर्णांक mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = खोलो ("/dev/null", O_PATH | O_CLOEXEC);
          अगर (UNLIKELY (nullfd < 0))
            वापस crun_make_error (err, त्रुटि_सं, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          अगर (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, शून्य);

          अगर (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, शून्य, MS_BIND | MS_RDONLY, शून्य,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस 0;
}

अटल स्थिर अक्षर *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  स्थिर अक्षर *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  अगर (context_type)
    {
      अगर (म_भेद (context_type, "context") != 0
          && म_भेद (context_type, "fscontext") != 0
          && म_भेद (context_type, "defcontext") != 0
          && म_भेद (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          वापस शून्य;
        }
      वापस context_type;
    }

  वापस "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
अटल पूर्णांक
diagnose_mount_failure (पूर्णांक ret, libcrun_error_t *err, libcrun_container_t *container,
                        स्थिर अक्षर *fstype)
{
  अगर (fstype && म_भेद (fstype, "cgroup2") == 0 && get_private_तथ्य (container)->unified_cgroup_path)
    {
      अगर (access (get_private_तथ्य (container)->unified_cgroup_path, R_OK) < 0 && त्रुटि_सं == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          वापस ret;
        }
    }
  वापस ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
अटल पूर्णांक
sysfs_userns_fallback (libcrun_container_t *container, स्थिर अक्षर *target,
                       स्थिर अक्षर *real_target, पूर्णांक targetfd,
                       अचिन्हित दीर्घ mountflags, स्थिर अक्षर *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close पूर्णांक mountfd = -1;
  cleanup_close पूर्णांक sysfd = -1;
  पूर्णांक sys_old_मूल_fd = -1;
  पूर्णांक ret;

  अगर (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && म_भेद (fstype, "sysfs") == 0))
    वापस 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (ret == 0)
    वापस 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_मूल_fd = get_old_मूल_fd (get_private_तथ्य (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  अगर (sys_old_मूल_fd >= 0)
    mountfd = get_bind_mount (sys_old_मूल_fd, "sys", true, true, false, MS_PRIVATE, err);
  अगर (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  अगर (cgroup_mount == शून्य)
    {
      अगर (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, शून्य);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "move mount `/sys` to `%s`", real_target);
        }
      अन्यथा
        {
          crun_error_release (err);
          अगर (sys_old_मूल_fd >= 0)
            {
              cleanup_free अक्षर *sys_path = शून्य;
              proc_fd_path_t sys_मूल_path;

              get_proc_self_fd_path (sys_मूल_path, sys_old_मूल_fd);
              xasprintf (&sys_path, "%s/sys", sys_मूल_path);
              ret = mount (sys_path, real_target, शून्य, MS_BIND | MS_REC, शून्य);
            }
          अन्यथा
            ret = mount ("/sys", real_target, शून्य, MS_BIND | MS_REC, शून्य);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      अगर (UNLIKELY (sysfd < 0))
        वापस sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        शून्य, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      वापस do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  अगर (UNLIKELY (mountfd < 0))
    वापस mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  अगर (UNLIKELY (sysfd < 0))
    वापस sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

#अगर_घोषित HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
अटल व्योम
apply_selinux_xattr (libcrun_container_t *container, पूर्णांक fd, पूर्णांक extra_flags, स्थिर अक्षर *label)
{
  libcrun_error_t tmp_err = शून्य;
  पूर्णांक procfd;

  अगर ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    वापस;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_तथ्य (container), &tmp_err);
  अगर (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close पूर्णांक xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      अगर (xfd >= 0)
        (व्योम) fsetxattr (xfd, "security.selinux", label, म_माप (label), 0);
    }
  crun_error_release (&tmp_err);
}
#पूर्ण_अगर

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
अटल पूर्णांक
do_mount_replace_rootfs (libcrun_container_t *container, पूर्णांक *fd, libcrun_error_t *err)
{
  पूर्णांक procfd = get_procfd (get_private_तथ्य (container), err);
  पूर्णांक tmp;
  पूर्णांक ret;

  अगर (UNLIKELY (procfd < 0))
    वापस procfd;

  {
    cleanup_close पूर्णांक nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    अगर (UNLIKELY (nsfd < 0))
      वापस crun_make_error (err, त्रुटि_सं, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    अगर (UNLIKELY (ret < 0))
      वापस crun_make_error (err, त्रुटि_सं, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = खोलो (get_private_तथ्य (container)->rootfs, O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (*fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  अगर (UNLIKELY (tmp < 0))
    वापस crun_make_error (err, त्रुटि_सं, "dup");

  अस्थायी_FAILURE_RETRY (बंद (get_private_तथ्य (container)->rootfsfd));
  get_private_तथ्य (container)->rootfsfd = tmp;

  वापस 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
अटल पूर्णांक
apply_propagation_flags (स्थिर अक्षर *target, पूर्णांक targetfd, स्थिर अक्षर *real_target,
                         अचिन्हित दीर्घ mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  पूर्णांक ret;

  अगर (targetfd >= 0)
    {
      libcrun_error_t tmp_err = शून्य;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      अगर (LIKELY (ret == 0))
        propagation_done = true;
      अन्यथा
        crun_error_release (&tmp_err);
    }

  अगर (! propagation_done)
    {
      ret = mount (शून्य, real_target, शून्य, mountflags & ALL_PROPAGATIONS, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "set propagation for `%s`", target);
    }

  वापस 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
अटल पूर्णांक
schedule_or_do_remount (libcrun_container_t *container, पूर्णांक *fd, पूर्णांक targetfd,
                        स्थिर अक्षर *target, स्थिर अक्षर *real_target,
                        bool single_instance, अचिन्हित दीर्घ mountflags,
                        स्थिर व्योम *तथ्य, पूर्णांक extra_flags, libcrun_error_t *err)
{
  अचिन्हित दीर्घ remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  काष्ठा remount_s *r;

  अगर ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    वापस do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, तथ्य, err);

  अगर (*fd < 0)
    {
      *fd = dup (targetfd);
      अगर (UNLIKELY (*fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, तथ्य, get_private_तथ्य (container)->remounts);
  get_private_तथ्य (container)->remounts = r;

  वापस 0;
}

अटल पूर्णांक
do_mount (libcrun_container_t *container, स्थिर अक्षर *source, पूर्णांक targetfd,
          स्थिर अक्षर *target, स्थिर अक्षर *fstype, अचिन्हित दीर्घ mountflags, स्थिर व्योम *तथ्य,
          पूर्णांक extra_flags, libcrun_error_t *err)
{
  cleanup_free अक्षर *तथ्य_with_label = शून्य;
  स्थिर अक्षर *तथ्य_without_label = तथ्य;
  proc_fd_path_t target_बफर;
  स्थिर अक्षर *context_type = शून्य;
  cleanup_close पूर्णांक ms_move_fd = -1;
  स्थिर अक्षर *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close पूर्णांक fd = -1;
  स्थिर अक्षर *label = शून्य;
  पूर्णांक ret = 0;

  अगर (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  अन्यथा
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  अगर (targetfd >= 0)
    {
      get_proc_self_fd_path (target_बफर, targetfd);
      real_target = target_बफर;
      needs_remount = true;
    }

  अगर ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      अगर (UNLIKELY (context_type == शून्य))
        वापस -1;

      ret = add_selinux_mount_label (&तथ्य_with_label, तथ्य, label, context_type, err);
      अगर (ret < 0)
        वापस ret;
      तथ्य = तथ्य_with_label;
    }

  अगर (mountflags & MS_MOVE)
    {
      अगर ((mountflags & MS_BIND) || fstype)
        वापस crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, शून्य, MS_MOVE, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      अगर (UNLIKELY (ms_move_fd < 0))
        वापस ms_move_fd;
      targetfd = ms_move_fd;
    }

  अगर ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      अचिन्हित दीर्घ flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      अगर (targetfd >= 0)
        {
          अगर (mountflags & MS_BIND)
            {
              cleanup_close पूर्णांक bindfd = -1;
              libcrun_error_t tmp_err = शून्य;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              अगर (bindfd < 0)
                {
                  पूर्णांक old_मूल_fd = get_old_मूल_fd (get_private_तथ्य (container));

                  अगर (old_मूल_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_मूल_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              अगर (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, शून्य);
              अगर (ret < 0)
                crun_error_release (&tmp_err);
            }
          अन्यथा
            {
              cleanup_close पूर्णांक newfs = fsopen_mount (fstype, source, context_type, label, तथ्य_without_label);
              अगर (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, शून्य);
            }
        }

      अगर (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, तथ्य);
          अगर (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              पूर्णांक old_मूल_fd = get_old_मूल_fd (get_private_तथ्य (container));

              अगर (old_मूल_fd >= 0)
                {
                  cleanup_free अक्षर *proc_source_buf = शून्य;
                  proc_fd_path_t old_मूल_path;

                  get_proc_self_fd_path (old_मूल_path, old_मूल_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_मूल_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, तथ्य);
                }
            }
        }
      अगर (UNLIKELY (ret < 0))
        {
          पूर्णांक saved_errno = त्रुटि_सं;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          अगर (ret == 0)
            वापस 0;
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          वापस diagnose_mount_failure (ret, err, container, fstype);
        }

      अगर (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          अगर (UNLIKELY (fd < 0))
            वापस fd;

          /* We are replacing the rootfs, reopen it.  */
          अगर (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              अगर (UNLIKELY (ret < 0))
                वापस ret;
            }

#अगर_घोषित HAVE_FGETXATTR
          apply_selinux_xattr (container, fd, extra_flags, label);
#पूर्ण_अगर

          targetfd = fd;

          /* The bind mount shadowed the original targetfd, so refresh
             real_target to point to the reopened mountpoint.  Otherwise the
             classic mount(2) fallbacks below (used when mount_setattr is not
             available, e.g. kernels < 5.12) operate on the shadowed path and
             fail with EINVAL.  */
          get_proc_self_fd_path (target_बफर, targetfd);
          real_target = target_बफर;
        }
    }

  अगर (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  अगर (तथ्य && fstype && म_भेद (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  अगर (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, तथ्य, extra_flags, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस ret;
}

अटल व्योम
try_umount (पूर्णांक targetfd, स्थिर अक्षर *target)
{
  proc_fd_path_t target_बफर;
  स्थिर अक्षर *real_target = target;

  अगर (targetfd >= 0)
    {
      get_proc_self_fd_path (target_बफर, targetfd);
      real_target = target_बफर;
    }

  umount2 (real_target, MNT_DETACH);
}

अटल bool
container_has_cgroupns (libcrun_container_t *container)
{
  वापस get_private_तथ्य (container)->unshare_flags & CLONE_NEWCGROUP;
}

अटल पूर्णांक
do_mount_cgroup_v2 (libcrun_container_t *container, पूर्णांक targetfd, स्थिर अक्षर *target,
                    अचिन्हित दीर्घ mountflags, स्थिर अक्षर *तथ्य, libcrun_error_t *err)
{
  पूर्णांक ret;
  पूर्णांक cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, तथ्य, LABEL_NONE, err);
  अगर (UNLIKELY (ret < 0))
    {
      पूर्णांक saved_errno = crun_error_get_errno (err);
      अगर (saved_errno == EPERM || saved_errno == EBUSY)
        {
          स्थिर अक्षर *unified_cgroup_path;
          स्थिर अक्षर *src_cgroup;

          crun_error_release (err);

          अगर (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              अगर (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, तथ्य, LABEL_NONE, err);
                  अगर (LIKELY (ret == 0))
                    वापस ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_तथ्य (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_मूल;
          वापस do_mount (container, src_cgroup, targetfd, target, शून्य,
                           MS_BIND | mountflags, शून्य, LABEL_NONE, err);
        }

      वापस ret;
    }

  वापस 0;
}

अटल पूर्णांक
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, स्थिर अक्षर *source, पूर्णांक targetfd, स्थिर अक्षर *target,
                            अचिन्हित दीर्घ mountflags, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_close पूर्णांक fd = -1;
  स्थिर अक्षर *subsystem = "systemd";
  cleanup_free अक्षर *subsystem_path = शून्य;
  cleanup_close पूर्णांक tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  अगर (UNLIKELY (tmpfsdirfd < 0))
    वापस tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "openat `%s`", subsystem_path);

  वापस do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

अटल पूर्णांक
do_mount_cgroup_v1 (libcrun_container_t *container, स्थिर अक्षर *source, पूर्णांक targetfd, स्थिर अक्षर *target,
                    अचिन्हित दीर्घ mountflags, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *content = शून्य;
  अक्षर *from;
  cleanup_close पूर्णांक tmpfsdirfd = -1;
  अक्षर *saveptr = शून्य;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  अगर (UNLIKELY (tmpfsdirfd < 0))
    वापस tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    पूर्णांक procfd = get_procfd (get_private_तथ्य (container), err);
    अगर (UNLIKELY (procfd < 0))
      वापस procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, शून्य, err);
  }
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (UNLIKELY (content == शून्य || content[0] == '\0'))
    वापस crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  क्रम (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (शून्य, "\n", &saveptr))
    {
      cleanup_free अक्षर *source_path = शून्य;
      cleanup_free अक्षर *source_subsystem = शून्य;
      cleanup_free अक्षर *subsystem_path = शून्य;
      अक्षर *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close पूर्णांक subsystemfd = -1;
      subsystem = म_अक्षर (from, ':') + 1;
      subpath = म_अक्षर (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      अगर (subsystem[0] == '\0')
        जारी;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = म_माला (subsystem, "name=");
      अगर (it)
        subsystem = it + 5;

      अगर (म_भेद (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      अगर (म_भेद (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_मूल, subsystem, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      /* if there is already a mount specified, do not add a default one.  */
      अगर (find_mount_for (container, source_subsystem) != शून्य)
        जारी;

      ret = append_paths (&source_path, err, source_subsystem, subpath, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      अगर (UNLIKELY (subsystemfd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", subsystem_path);

      अगर (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          अगर (UNLIKELY (ret < 0))
            {
              अगर (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  जारी;
                }
              वापस ret;
            }
        }
      अन्यथा
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, शून्य, MS_BIND | mountflags, शून्य,
                          LABEL_NONE, err);
          अगर (UNLIKELY (ret < 0))
            {
              अगर (crun_error_get_errno (err) != ENOENT)
                वापस ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, शून्य, MS_BIND | mountflags,
                              शून्य, LABEL_NONE, err);
              अगर (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  अगर (crun_error_get_errno (err) != ENOENT)
                    वापस ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

अटल पूर्णांक
do_mount_cgroup (libcrun_container_t *container, स्थिर अक्षर *source, पूर्णांक targetfd, स्थिर अक्षर *target,
                 अचिन्हित दीर्घ mountflags, स्थिर अक्षर *तथ्य, libcrun_error_t *err)
{
  पूर्णांक cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  चयन (cgroup_mode)
    {
    हाल CGROUP_MODE_UNIFIED:
      वापस do_mount_cgroup_v2 (container, targetfd, target, mountflags, तथ्य, err);
    हाल CGROUP_MODE_LEGACY:
    हाल CGROUP_MODE_HYBRID:
      वापस do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  वापस crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

काष्ठा device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
अटल व्योम
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  काष्ठा private_तथ्य_s *private_तथ्य = get_private_तथ्य (container);
  माप_प्रकार i;

  अगर (private_तथ्य->needed_devs_fds)
    {
      काष्ठा libcrun_fd_map *dev_fds = private_तथ्य->needed_devs_fds;

      क्रम (i = 0; needed_devs[i].path; i++)
        {
          अगर (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            जारी;

          पूर्णांक fd = syscall_open_वृक्ष (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          अगर (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          अन्यथा अगर (fd >= 0)
            बंद (fd);
        }
    }

  अगर (private_तथ्य->notify_socket_वृक्ष_fd < 0
      && private_तथ्य->host_notify_socket_path)
    {
      पूर्णांक fd = syscall_open_वृक्ष (AT_FDCWD, private_तथ्य->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      अगर (fd >= 0)
        private_तथ्य->notify_socket_वृक्ष_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
अटल स्थिर अक्षर *
relative_path_under_dev (स्थिर अक्षर *path)
{
  अगर (path[0] != '/')
    वापस path;
  अगर (म_भेदन (path, "/dev/", 5) == 0)
    {
      अगर (म_अक्षर (path + 5, '/'))
        वापस शून्य;
      वापस path + 5;
    }
  वापस शून्य;
}

अटल पूर्णांक
mknod_and_set_attrs (libcrun_container_t *container, पूर्णांक dirfd, स्थिर अक्षर *name, काष्ठा device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  proc_fd_path_t fd_path;
  स्थिर अक्षर *what = "new";
  bool exists = false;
  काष्ठा stat st;
  पूर्णांक procfd;
  पूर्णांक ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं != EEXIST)
        वापस crun_make_error (err, त्रुटि_सं, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open %s device `%s`", what, device->path);

  ret = ख_स्थिति (fd, &st);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fstat %s device `%s`", what, device->path);

  अगर (UNLIKELY ((st.st_mode & S_IFMT) != type))
    वापस crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  अगर (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    वापस crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  अगर (exists)
    वापस 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_तथ्य (container), err);
  अगर (UNLIKELY (procfd < 0))
    वापस procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fchown `%s`", device->path);

  वापस 1;
}

पूर्णांक
libcrun_create_dev (libcrun_container_t *container, पूर्णांक devfd, पूर्णांक srcfd,
                    काष्ठा device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  पूर्णांक ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  स्थिर अक्षर *fullname = device->path;
  स्थिर अक्षर *rootfs = get_private_तथ्य (container)->rootfs;
  अगर (is_empty_string (fullname))
    वापस crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free अक्षर *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  अगर (normalized_path[0] == '\0')
    म_नकल (normalized_path, "/");
  स्थिर अक्षर *rel_dev = relative_path_under_dev (normalized_path);

  अगर (binds)
    {
      cleanup_close पूर्णांक fd = -1;

      अगर (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          अगर (UNLIKELY (fd < 0))
            {
              अगर (त्रुटि_सं == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              अगर (UNLIKELY (fd < 0))
                वापस crun_make_error (err, त्रुटि_सं, "openat `%s`", device->path);
            }
        }
      अन्यथा
        {
          स्थिर अक्षर *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_तथ्य (container)->rootfsfd, rootfs, rel_path, 0755, err);
          अगर (UNLIKELY (fd < 0))
            वापस fd;
        }

      अगर (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          अगर (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              अगर (LIKELY (ret >= 0))
                वापस 0;
            }
          अन्यथा
            crun_error_release (err);
        }

      {
        cleanup_close पूर्णांक mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        अगर (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            अगर (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, शून्य);
                अगर (LIKELY (ret == 0))
                  वापस 0;
              }
            अन्यथा
              crun_error_release (err);
          }
        अन्यथा
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, शून्य, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, शून्य, LABEL_MOUNT, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      अगर (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          अगर (ret <= 0)
            वापस ret;
        }
      अन्यथा
        {
          cleanup_close पूर्णांक dirfd = -1;
          cleanup_free अक्षर *स_नाम = शून्य;
          अक्षर *basename, *found;

          स_नाम = xstrdup (normalized_path);

          found = म_खोजप (स_नाम, '/');
          अगर (found)
            *found = '\0';

          basename = found ? found + 1 : स_नाम;

          अगर (स_नाम[0] == '\0')
            {
              dirfd = dup (get_private_तथ्य (container)->rootfsfd);
              अगर (UNLIKELY (dirfd < 0))
                वापस crun_make_error (err, त्रुटि_सं, "dup fd for `%s`", rootfs);
            }
          अन्यथा
            {
              dirfd = safe_openat (get_private_तथ्य (container)->rootfsfd, rootfs, स_नाम, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              अगर (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_तथ्य (container)->rootfsfd, rootfs, स_नाम, 0755, err);
                }
              अगर (UNLIKELY (dirfd < 0))
                वापस dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          अगर (ret <= 0)
            वापस ret;
        }
    }
  वापस 0;
}

काष्ठा symlink_s
{
  स्थिर अक्षर *path;
  स्थिर अक्षर *target;
  bool force;
};

अटल काष्ठा symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { शून्य, शून्य, false } };

अटल पूर्णांक
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  पूर्णांक ret;
  माप_प्रकार i;
  काष्ठा device_s *it;
  cleanup_close पूर्णांक devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  स्थिर अक्षर *rootfs = get_private_तथ्य (container)->rootfs;
  cleanup_close_map काष्ठा libcrun_fd_map *dev_fds = शून्य;
  cleanup_close_map काष्ठा libcrun_fd_map *needed_fds = शून्य;

  dev_fds = get_private_तथ्य (container)->dev_fds;
  get_private_तथ्य (container)->dev_fds = शून्य;

  needed_fds = get_private_तथ्य (container)->needed_devs_fds;
  get_private_तथ्य (container)->needed_devs_fds = शून्य;

  अगर (! def || ! def->linux)
    वापस 0;

  devfd = safe_openat (get_private_तथ्य (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  अगर (UNLIKELY (devfd < 0))
    वापस devfd;

  क्रम (i = 0; i < def->linux->devices_len; i++)
    {
      काष्ठा device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      अगर (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  क्रम (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close पूर्णांक srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      अगर (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  क्रम (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      अगर (UNLIKELY (ret < 0))
        {
          पूर्णांक saved_errno = त्रुटि_सं;

          अगर (त्रुटि_सं == EEXIST && ! symlinks[i].force)
            जारी;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          अगर (त्रुटि_सं == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              अगर (ret < 0 && त्रुटि_सं == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              अगर (ret < 0 && त्रुटि_सं == EBUSY)
                {
                  cleanup_close पूर्णांक tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  अगर (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      अगर (umount2 (procpath, MNT_DETACH) == 0)
                        जाओ retry_unlink;
                    }
                }
              अगर (ret == 0)
                जाओ retry_symlink;
            }
          वापस crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  अगर (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
अटल स्थिर अक्षर *
अग्र_path_component (स्थिर अक्षर *p, माप_प्रकार *len)
{
  क्रम (;;)
    {
      p = consume_slashes (p);
      *len = म_खोज (p, "/");
      अगर (*len != 1 || p[0] != '.')
        वापस p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
अटल bool
same_path (स्थिर अक्षर *a, स्थिर अक्षर *b)
{
  माप_प्रकार la, lb;

  क्रम (;;)
    {
      a = अग्र_path_component (a, &la);
      b = अग्र_path_component (b, &lb);
      अगर (la != lb || स_भेद (a, b, la) != 0)
        वापस false;
      अगर (la == 0)
        वापस true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
अटल bool
is_duplicate_path (अक्षर **paths, माप_प्रकार i)
{
  माप_प्रकार j;

  अगर (paths[i] == शून्य)
    वापस false;

  क्रम (j = 0; j < i; j++)
    अगर (paths[j] && same_path (paths[j], paths[i]))
      वापस true;

  वापस false;
}

अटल पूर्णांक
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  माप_प्रकार i;
  पूर्णांक ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  अगर (! def || ! def->linux)
    वापस 0;

  क्रम (i = 0; i < def->linux->masked_paths_len; i++)
    {
      अगर (is_duplicate_path (def->linux->masked_paths, i))
        जारी;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  क्रम (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस 0;
}

अटल पूर्णांक
do_pivot (libcrun_container_t *container, स्थिर अक्षर *rootfs, libcrun_error_t *err)
{
  cleanup_close पूर्णांक oldrootfd = -1;
  cleanup_close पूर्णांक newrootfd = -1;
  पूर्णांक ret;

  oldrootfd = खोलो ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (oldrootfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `/`");

  newrootfd = खोलो (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (newrootfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fchdir `%s`", rootfs);

  ret = pivot_मूल (".", ".");
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "pivot_root");

  ret = fchdir (oldrootfd);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fchdir `%s`", rootfs);

  ret = do_mount (container, शून्य, -1, ".", शून्य, MS_REC | MS_PRIVATE, शून्य, LABEL_MOUNT, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = umount2 (".", MNT_DETACH);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "umount2 oldroot");

  करो
    {
      ret = umount2 (".", MNT_DETACH);
      अगर (ret < 0 && त्रुटि_सं == EINVAL)
        अवरोध;
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "umount2 oldroot");
  } जबतक (ret == 0);

  ret = स_बदलो ("/");
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "chdir to newroot");

  वापस 0;
}

अटल पूर्णांक
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, अक्षर **तथ्य, libcrun_error_t *err)
{
  स्थिर अक्षर *rootfs = get_private_तथ्य (container)->rootfs;
  bool empty_तथ्य = is_empty_string (*तथ्य);
  cleanup_close पूर्णांक fd = -1;
  काष्ठा stat st;
  पूर्णांक ret;

  अगर (*तथ्य != शून्य && म_माला (*तथ्य, "mode="))
    वापस 0;

  fd = safe_openat (get_private_तथ्य (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  अगर (fd < 0)
    {
      अगर (crun_error_get_errno (err) != ENOENT)
        वापस fd;

      crun_error_release (err);
      वापस 0;
    }
  ret = ख_स्थिति (fd, &st);
  अगर (ret < 0)
    वापस crun_make_error (err, त्रुटि_सं, "fstat `%s`", mount->destination);

  xasprintf (तथ्य, "%s%smode=%o", empty_तथ्य ? "" : *तथ्य, empty_तथ्य ? "" : ",", st.st_mode & 07777);
  वापस 0;
}

अटल पूर्णांक
get_default_flags (libcrun_container_t *container, स्थिर अक्षर *destination, अक्षर **तथ्य)
{
  अगर (म_भेद (destination, "/proc") == 0)
    वापस 0;
  अगर (म_भेद (destination, "/dev/cgroup") == 0 || म_भेद (destination, "/sys/fs/cgroup") == 0)
    {
      *तथ्य = xstrdup ("none,name=");
      वापस MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  अगर (म_भेद (destination, "/dev") == 0)
    {
      *तथ्य = xstrdup ("mode=755");
      वापस MS_NOEXEC | MS_STRICTATIME;
    }
  अगर (म_भेद (destination, "/dev/shm") == 0)
    {
      *तथ्य = xstrdup ("mode=1777,size=65536k");
      वापस MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  अगर (म_भेद (destination, "/dev/mqueue") == 0)
    वापस MS_NOEXEC | MS_NOSUID | MS_NODEV;
  अगर (म_भेद (destination, "/dev/pts") == 0)
    {
      अगर (container->host_uid == 0)
        *तथ्य = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      अन्यथा
        *तथ्य = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      वापस MS_NOEXEC | MS_NOSUID;
    }
  अगर (म_भेद (destination, "/sys") == 0)
    वापस MS_NOEXEC | MS_NOSUID | MS_NODEV;

  वापस 0;
}

अटल स्थिर अक्षर *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  वापस find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

अटल पूर्णांक
check_valid_no_follow_file_system (पूर्णांक fd, स्थिर अक्षर *destination, libcrun_error_t *err)
{
  काष्ठा statfs sfs;

  पूर्णांक ret = fstatfs (fd, &sfs);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "statfs `%s`", destination);

  चयन (sfs.f_type)
    {
    हाल PROC_SUPER_MAGIC:
    हाल SYSFS_MAGIC:
    हाल DEVPTS_SUPER_MAGIC:
      वापस crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  वापस 0;
}

अटल पूर्णांक
safe_create_symlink (पूर्णांक rootfsfd, स्थिर अक्षर *rootfs, स्थिर अक्षर *target, स्थिर अक्षर *destination, libcrun_error_t *err)
{
  cleanup_close पूर्णांक parent_dir_fd = -1;
  cleanup_free अक्षर *बफर = शून्य;
  अक्षर *part;
  पूर्णांक ret;

  अगर (is_empty_string (destination))
    वापस crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  बफर = xstrdup (destination);
  part = स_नाम (बफर);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  अगर (UNLIKELY (parent_dir_fd < 0))
    वापस parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  म_नकल (बफर, destination);
  part = basename (बफर);

  ret = symlinkat (target, parent_dir_fd, part);
  अगर (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      अगर (त्रुटि_सं == EEXIST)
        {
          cleanup_free अक्षर *link = शून्य;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          अगर (UNLIKELY (len < 0))
            वापस len;

          अगर ((((माप_प्रकार) len) == म_माप (target)) && म_भेदन (link, target, len) == 0)
            वापस 0;

          वापस crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      वापस crun_make_error (err, त्रुटि_सं, "symlinkat `%s`", target);
    }

  वापस 0;
}

अटल पूर्णांक
handle_copy_symlink (libcrun_container_t *container, स्थिर अक्षर *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     स्थिर अक्षर *link_target, libcrun_error_t *err)
{
  cleanup_free अक्षर *target = शून्य;

  अगर (link_target == शून्य)
    {
      पूर्णांक old_मूल_fd = get_old_मूल_fd (get_private_तथ्य (container));
      स्थिर अक्षर *source = mount->source;
      ssize_t len;

      अगर (old_मूल_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_मूल_fd, source + 1, &target, 0, err);
      अन्यथा
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      अगर (UNLIKELY (len < 0))
        वापस len;
      link_target = target;
    }

  वापस safe_create_symlink (get_private_तथ्य (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

अटल पूर्णांक
prepare_sysfs_or_proc_mount (libcrun_container_t *container, स्थिर अक्षर *target,
                             पूर्णांक *targetfd, libcrun_error_t *err)
{
  पूर्णांक ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_तथ्य (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं == ENOENT)
        {
          अगर (म_अक्षर (target, '/'))
            वापस crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_तथ्य (container)->rootfsfd, target, 0755);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_तथ्य (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      अन्यथा अगर (त्रुटि_सं == ENOTDIR)
        वापस crun_make_error (err, त्रुटि_सं, "the target `/%s` is invalid", target);

      अगर (ret < 0)
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", target);
    }

  *targetfd = ret;
  वापस 0;
}

अटल पूर्णांक
handle_tmpcopyup (libcrun_container_t *container, स्थिर अक्षर *rootfs, स्थिर अक्षर *target,
                  पूर्णांक copy_from_fd, libcrun_error_t *err)
{
  पूर्णांक destfd, ret;
  cleanup_close पूर्णांक tmpfd = copy_from_fd;

  target = consume_slashes (target);

  अगर (is_empty_string (target))
    वापस crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_तथ्य (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  अगर (UNLIKELY (destfd < 0))
    वापस crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  वापस ret;
}

अटल पूर्णांक
get_mount_label_how (स्थिर अक्षर *type, bool is_sysfs_or_proc)
{
  अगर (is_sysfs_or_proc)
    वापस LABEL_NONE;

  अगर (म_भेद (type, "mqueue") == 0)
    वापस LABEL_XATTR;

  वापस LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
अटल अचिन्हित दीर्घ
get_bind_mount_locked_flags (पूर्णांक source_mountfd, स्थिर अक्षर *source, runtime_spec_schema_defs_mount *mount)
{
  अचिन्हित दीर्घ cleared = 0;
  काष्ठा statfs sfs;
  माप_प्रकार i;
  पूर्णांक ret;

  अगर (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  अन्यथा
    ret = statfs (source, &sfs);
  अगर (UNLIKELY (ret < 0))
    वापस 0;

  क्रम (i = 0; i < mount->options_len; i++)
    {
      स्थिर काष्ठा propagation_flags_s *prop;

      अगर (mount->options[i] == शून्य)
        जारी;

      prop = libcrun_str2mount_flags (mount->options[i]);
      अगर (prop == शून्य || (prop->extra_flags & OPTION_RECURSIVE))
        जारी;

      अगर (prop->clear)
        cleared |= prop->flags;
      अन्यथा
        cleared &= ~prop->flags;
    }

  वापस sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
अटल स्थिर अक्षर *
get_path_in_rootfs (स्थिर अक्षर *rootfs, स्थिर अक्षर *path)
{
  माप_प्रकार len;

  अगर (rootfs == शून्य || path == शून्य)
    वापस शून्य;

  len = म_माप (rootfs);
  जबतक (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  अगर (len == 0)
    वापस शून्य;

  अगर (म_भेदन (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    वापस शून्य;

  वापस consume_slashes (path + len);
}

अटल पूर्णांक
process_single_mount (libcrun_container_t *container, स्थिर अक्षर *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      काष्ठा libcrun_fd_map *mount_fds, माप_प्रकार mount_सूचकांक,
                      स्थिर अक्षर *systemd_cgroup_v1, libcrun_error_t *err)
{
  स्थिर अक्षर *target = consume_slashes (mount->destination);
  cleanup_close पूर्णांक source_mountfd = -1;
  cleanup_free अक्षर *तथ्य = शून्य;
  अक्षर *type;
  अक्षर *source;
  अचिन्हित दीर्घ flags = 0;
  अचिन्हित दीर्घ extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close पूर्णांक copy_from_fd = -1;
  cleanup_close पूर्णांक targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  पूर्णांक no_remount = 0;
  पूर्णांक ret;

  अगर (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_सूचकांक]));

  type = mount->type;

  अगर (mount->options == शून्य)
    flags = get_default_flags (container, mount->destination, &तथ्य);
  अन्यथा
    {
      माप_प्रकार j;

      क्रम (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &तथ्य, &rec_clear, &rec_set);
    }

  अगर (type == शून्य && (flags & MS_BIND) == 0)
    वापस crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  अगर ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      स्थिर अक्षर *rel_source = get_path_in_rootfs (get_private_तथ्य (container)->host_rootfs, mount->source);

      अगर (rel_source)
        {
          अचिन्हित दीर्घ propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_तथ्य (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          अगर (UNLIKELY (source_mountfd < 0))
            वापस crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  अगर ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_तथ्य (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  अगर (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      अगर (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      अगर (path_is_slash_dev (mount->destination))
        get_private_तथ्य (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = म_भेद (type, "sysfs") == 0 || म_भेद (type, "proc") == 0;

  अगर (म_भेद (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &तथ्य, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      स्थिर अक्षर *path = mount->source;

      अगर ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        वापस crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_तथ्य (container)->copy_symlink_targets
                               && mount_सूचकांक < get_private_तथ्य (container)->n_copy_symlink_targets
                               && get_private_तथ्य (container)->copy_symlink_targets[mount_सूचकांक];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      अगर (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, शून्य);
        }
      अन्यथा अगर (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      अन्यथा अगर (path[0] == '/')
        {
          पूर्णांक old_मूल_fd = get_old_मूल_fd (get_private_तथ्य (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          अगर (old_मूल_fd >= 0)
            ret = get_file_type_at (old_मूल_fd, &src_mode, nofollow, (path[1] == '\0') ? शून्य : path + 1);
          अन्यथा
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      अन्यथा
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "cannot stat `%s`", path);

      अगर (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          पूर्णांक bind_dirfd = AT_FDCWD;
          स्थिर अक्षर *bind_source = mount->source;
          पूर्णांक old_मूल_fd = get_old_मूल_fd (get_private_तथ्य (container));

          अगर (old_मूल_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_मूल_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          source_mountfd = ret;
        }
    }

  अगर (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      स्थिर अक्षर *cached_target = शून्य;

      अगर (get_private_तथ्य (container)->copy_symlink_targets
          && mount_सूचकांक < get_private_तथ्य (container)->n_copy_symlink_targets)
        cached_target = get_private_तथ्य (container)->copy_symlink_targets[mount_सूचकांक];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      mounted = true;
    }
  अन्यथा अगर (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      bool is_dir = S_ISDIR (src_mode);

      अगर (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_तथ्य (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
      अन्यथा
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_तथ्य (container)->rootfsfd, rootfs, target, 0755, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          targetfd = ret;
        }
    }

  अगर (extra_flags & OPTION_TMPCOPYUP)
    {
      अगर (म_भेद (type, "tmpfs") != 0)
        वापस crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      अगर (UNLIKELY (copy_from_fd < 0))
        {
          अगर (त्रुटि_सं != ENOTDIR)
            वापस crun_make_error (err, त्रुटि_सं, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  अगर (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, शून्य);
      अगर (LIKELY (ret == 0))
        {
          अचिन्हित दीर्घ remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          अगर (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, शून्य, source_mountfd, target, शून्य, remaining_flags, तथ्य, LABEL_NONE | no_remount, err);
              अगर (UNLIKELY (ret < 0))
                वापस ret;
            }

          mounted = true;

          अगर (is_empty_string (target))
            {
              पूर्णांक new_rootfsfd = खोलो (rootfs, O_PATH | O_CLOEXEC);
              अगर (UNLIKELY (new_rootfsfd < 0))
                वापस crun_make_error (err, त्रुटि_सं, "reopen rootfs after mount on /");

              अस्थायी_FAILURE_RETRY (बंद (get_private_तथ्य (container)->rootfsfd));
              get_private_तथ्य (container)->rootfsfd = new_rootfsfd;
            }
        }
      अन्यथा अगर (get_private_तथ्य (container)->delayed_pidns_fork && म_भेद (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          वापस crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      अन्यथा अगर (get_private_तथ्य (container)->host_मूल_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          वापस crun_make_error (err, त्रुटि_सं, "move_mount `%s` to `%s`", source, target);
        }
      अन्यथा अगर (त्रुटि_सं != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, म_त्रुटि (त्रुटि_सं));
        }
    }

  अगर (! mounted)
    {
      अगर (systemd_cgroup_v1 && म_भेद (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
      अन्यथा अगर (म_भेद (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, तथ्य, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
      अन्यथा
        {
          पूर्णांक extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, तथ्य, extra_flags, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }

  अगर (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (rec_clear || rec_set)
    {
      स्थिर bool is_dir = S_ISDIR (src_mode);
      cleanup_close पूर्णांक dfd = -1;

      dfd = safe_openat (get_private_तथ्य (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      अगर (UNLIKELY (dfd < 0))
        वापस crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

अटल पूर्णांक
do_mounts (libcrun_container_t *container, स्थिर अक्षर *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  स्थिर अक्षर *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map काष्ठा libcrun_fd_map *mount_fds = शून्य;
  माप_प्रकार i;
  पूर्णांक ret;

  mount_fds = get_private_तथ्य (container)->mount_fds;
  get_private_तथ्य (container)->mount_fds = शून्य;

  क्रम (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
पूर्णांक
libcrun_container_do_bind_mount (libcrun_container_t *container, अक्षर *mount_source, अक्षर *mount_destination, अक्षर **mount_options, माप_प्रकार mount_options_len, libcrun_error_t *err)
{
  स्थिर अक्षर *target = consume_slashes (mount_destination);
  cleanup_free अक्षर *तथ्य = शून्य;
  अचिन्हित दीर्घ flags = 0;
  अचिन्हित दीर्घ extra_flags = 0;
  cleanup_close पूर्णांक targetfd = -1;
  पूर्णांक is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  स्थिर अक्षर *rootfs = get_private_तथ्य (container)->rootfs;
  पूर्णांक ret;

  अगर ((get_private_तथ्य (container)->rootfsfd < 0) || (rootfs == शून्य))
    वापस crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  अगर (mount_options == शून्य)
    flags = get_default_flags (container, mount_destination, &तथ्य);
  अन्यथा
    {
      माप_प्रकार j;

      क्रम (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &तथ्य, &rec_clear, &rec_set);
    }

  अगर (path_is_slash_dev (mount_destination))
    get_private_तथ्य (container)->mount_dev_from_host = true;

  अगर (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      अगर (UNLIKELY (is_dir < 0))
        वापस is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_तथ्य (container)->rootfsfd, rootfs, target, 0755, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, तथ्य, LABEL_MOUNT, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
पूर्णांक
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, पूर्णांक *notify_socket_out, libcrun_error_t *err)
{
#अगर_घोषित HAVE_SYSTEMD
  cleanup_close पूर्णांक notify_fd = -1;
  cleanup_free अक्षर *host_notify_socket_path = शून्य;
  cleanup_free अक्षर *state_dir = शून्य;
  अक्षर *host_path = शून्य;
  पूर्णांक ret;

  अगर (container && get_private_तथ्य (container)->host_notify_socket_path)
    {
      स्थिर अक्षर *parent_dir;

      parent_dir = get_private_तथ्य (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  अगर (host_path == शून्य)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_मूल, context->id, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  अगर (UNLIKELY (notify_fd < 0))
    वापस notify_fd;

  अगर (UNLIKELY (chmod (host_path, 0777) < 0))
    वापस crun_make_error (err, त्रुटि_सं, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  अगर (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (व्योम) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       म_माप (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  वापस 1;
#अन्यथा
  (व्योम) context;
  (व्योम) container;
  (व्योम) err;
  *notify_socket_out = -1;
  वापस 0;
#पूर्ण_अगर
}

#अगर_घोषित HAVE_SYSTEMD
अटल पूर्णांक
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret;
  स्थिर अक्षर *notify_socket = container->context->notify_socket;
  cleanup_free अक्षर *host_notify_socket_path = शून्य;
  cleanup_free अक्षर *container_notify_socket_path = शून्य;
  cleanup_free अक्षर *state_dir = शून्य;
  uid_t container_मूल_uid = -1;
  gid_t container_मूल_gid = -1;
  पूर्णांक notify_socket_वृक्ष_fd;

  अगर (notify_socket == शून्य)
    वापस 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_मूल : शून्य),
                                     (container->context ? container->context->id : शून्य), err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = सूची_गढ़ो (host_notify_socket_path, 0700);
  अगर (ret < 0)
    वापस crun_make_error (err, त्रुटि_सं, "mkdir `%s`", host_notify_socket_path);

  अगर (get_private_तथ्य (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_मूल_in_the_userns (container->container_def, 0, 0, &container_मूल_uid, &container_मूल_gid);
      अगर (container_मूल_uid != ((uid_t) -1) && container_मूल_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_मूल_uid, container_मूल_gid);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "chown `%s` to `%d:%d`", host_notify_socket_path, container_मूल_uid, container_मूल_gid);
        }
    }

  notify_socket_वृक्ष_fd = syscall_open_वृक्ष (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  अगर (notify_socket_वृक्ष_fd >= 0)
    /* open_tree worked */
    get_private_तथ्य (container)->notify_socket_वृक्ष_fd = notify_socket_वृक्ष_fd;
  अन्यथा अगर (त्रुटि_सं == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  अन्यथा अगर (त्रुटि_सं == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  अन्यथा
    /* some other error */
    वापस crun_make_error (err, त्रुटि_सं, "open_tree `%s`", host_notify_socket_path);

  get_private_तथ्य (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_तथ्य (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = शून्य;
  वापस 0;
}
#पूर्ण_अगर

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
अटल पूर्णांक
do_finalize_notify_socket (libcrun_container_t *container, स्थिर अक्षर *rootfs, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *host_notify_socket_path = शून्य;
  cleanup_free अक्षर *container_notify_socket_path = शून्य;
  cleanup_free अक्षर *container_notify_socket_path_dir_alloc = शून्य;
  स्थिर अक्षर *container_notify_socket_path_dir = शून्य;
  cleanup_close पूर्णांक notify_socket_वृक्ष_fd = -1;
  cleanup_close पूर्णांक targetfd = -1;
  पूर्णांक did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_तथ्य (container)->host_notify_socket_path;
  get_private_तथ्य (container)->host_notify_socket_path = शून्य;

  container_notify_socket_path = get_private_तथ्य (container)->container_notify_socket_path;
  get_private_तथ्य (container)->container_notify_socket_path = शून्य;

  अगर (host_notify_socket_path == शून्य || container_notify_socket_path == शून्य)
    वापस 0;

  अगर (rootfs == शून्य)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (स_नाम (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  अगर (get_private_तथ्य (container)->rootfsfd < 0)
    {
      ret = खोलो (rootfs, O_PATH | O_CLOEXEC);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

      get_private_तथ्य (container)->rootfsfd = ret;
      get_private_तथ्य (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_तथ्य (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  targetfd = ret;

  notify_socket_वृक्ष_fd = get_private_तथ्य (container)->notify_socket_वृक्ष_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_तथ्य (container)->notify_socket_वृक्ष_fd = -1;

  अगर (notify_socket_वृक्ष_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_वृक्ष_fd, targetfd, शून्य);
      अगर (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          अगर (UNLIKELY (targetfd < 0))
            वापस targetfd;

          ret = do_mount (container, शून्य, targetfd, container_notify_socket_path_dir, शून्य,
                          MS_REC | MS_PRIVATE, शून्य, LABEL_NONE, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
      अन्यथा अगर (त्रुटि_सं == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      अन्यथा
        वापस crun_make_error (err, त्रुटि_सं, "move_mount `%d` to `%s`", notify_socket_वृक्ष_fd,
                                container_notify_socket_path_dir);
    }

  अगर (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, शून्य,
                      MS_BIND | MS_REC | MS_PRIVATE, शून्य, LABEL_MOUNT, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

अटल पूर्णांक
make_parent_mount_private (स्थिर अक्षर *rootfs, libcrun_error_t *err)
{
  cleanup_close पूर्णांक rootfsfd = -1;
  काष्ठा stat पूर्व_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = खोलो (rootfs, O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (rootfsfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

  अगर (UNLIKELY (ख_स्थिति (rootfsfd, &पूर्व_st) < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fstat `%s`", rootfs);

  क्रम (;;)
    {
      काष्ठा stat cur_st;
      libcrun_error_t tmp_err = शून्य;
      पूर्णांक ret;
      cleanup_close पूर्णांक parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      अगर (ret == 0)
        वापस 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (शून्य, proc_path, शून्य, MS_PRIVATE, शून्य);
      अगर (ret == 0)
        वापस 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      अगर (parentfd < 0)
        {
          पूर्णांक saved_errno = त्रुटि_सं;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          अगर (ret != 0)
            वापस crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          वापस crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      अगर (UNLIKELY (ख_स्थिति (parentfd, &cur_st) < 0))
        वापस crun_make_error (err, त्रुटि_सं, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      अगर (cur_st.st_dev == पूर्व_st.st_dev && cur_st.st_ino == पूर्व_st.st_ino)
        अवरोध;

      पूर्व_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  वापस crun_make_error (err, 0, "make `%s` private", rootfs);
}

पूर्णांक
libcrun_set_mounts (काष्ठा container_entrypoint_s *entrypoint_args, libcrun_container_t *container, स्थिर अक्षर *rootfs, set_mounts_cb_t cb, व्योम *cb_तथ्य, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक is_user_ns = 0;
  पूर्णांक ret = 0;

  अगर (rootfs == शून्य || def->mounts == शून्य)
    वापस 0;

  ret = खोलो (rootfs, O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

  get_private_तथ्य (container)->rootfsfd = ret;
  get_private_तथ्य (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  अगर (UNLIKELY (ret < 0))
    वापस crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  अगर (def->मूल->readonly)
    {
      काष्ठा remount_s *r;
      अचिन्हित दीर्घ remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      पूर्णांक fd;

      fd = dup (get_private_तथ्य (container)->rootfsfd);
      अगर (UNLIKELY (fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, शून्य, get_private_तथ्य (container)->remounts);
      get_private_तथ्य (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_तथ्य (container)->unshare_flags & CLONE_NEWUSER);
  अगर (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      अगर (UNLIKELY (is_user_ns < 0))
        वापस is_user_ns;
    }

  अगर (! get_private_तथ्य (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  अगर (cb)
    {
      ret = cb (cb_तथ्य, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = शून्य;
      स्थिर अक्षर *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (व्योम) crun_safe_ensure_directory_at (get_private_तथ्य (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

अटल पूर्णांक
umount_or_hide (स्थिर अक्षर *target, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = umount2 (target, MNT_DETACH);
  अगर (UNLIKELY (ret < 0))
    {
      पूर्णांक saved_errno = त्रुटि_सं;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      अगर (त्रुटि_सं == EINVAL)
        {
          ret = mount (शून्य, target, "tmpfs", 0, "size=0k");
          अगर (LIKELY (ret == 0))
            वापस 0;
        }

      वापस crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  वापस ret;
}

अटल पूर्णांक
move_मूल (स्थिर अक्षर *rootfs, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = स_बदलो (rootfs);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = umount_or_hide ("/proc", err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "mount MS_MOVE to `/`");

  ret = chroot (".");
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "chroot to `%s`", rootfs);

  ret = स_बदलो ("/");
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "chdir to `%s`", rootfs);

  वापस 0;
}

पूर्णांक
libcrun_finalize_mounts (काष्ठा container_entrypoint_s *entrypoint_args, libcrun_container_t *container, स्थिर अक्षर *rootfs,
                         set_mounts_cb_t after_pivot_cb, व्योम *cb_तथ्य, libcrun_error_t *err)
{
  पूर्णांक ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  अगर (UNLIKELY (ret < 0))
    वापस crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  close_and_reset (&(get_private_तथ्य (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_तथ्य (container)->old_मूल_fd));

  अगर (get_private_तथ्य (container)->needs_pivot)
    {
      get_private_तथ्य (container)->needs_pivot = false;

      अगर (get_private_तथ्य (container)->no_pivot)
        {
          ret = move_मूल (rootfs, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
      अन्यथा
        {
          ret = do_pivot (container, rootfs, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      ret = do_mount (container, शून्य, -1, "/", शून्य,
                      get_private_तथ्य (container)->rootfs_propagation,
                      शून्य, LABEL_MOUNT, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = स_बदलो ("/");
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "chdir to `/`");
    }

  अगर (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_तथ्य, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = finalize_mounts (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

अटल पूर्णांक
maybe_open_वृक्ष_namespace (स्थिर अक्षर *rootfs, पूर्णांक *out_fd, libcrun_error_t *err)
{
  cleanup_close पूर्णांक rootfs_fd = -1;
  पूर्णांक वृक्ष_fd;

  *out_fd = -1;

  rootfs_fd = खोलो (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (rootfs_fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);

  वृक्ष_fd = syscall_open_वृक्ष (rootfs_fd, "",
                               OPEN_वृक्ष_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  अगर (वृक्ष_fd < 0)
    {
      अगर (त्रुटि_सं == EINVAL || त्रुटि_सं == ENOSYS || त्रुटि_सं == EPERM)
        वापस 0;
      वापस crun_make_error (err, त्रुटि_सं, "open_tree `%s`", rootfs);
    }

  *out_fd = वृक्ष_fd;
  वापस 0;
}

अटल काष्ठा libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  काष्ठा libcrun_fd_map *mount_fds = get_private_तथ्य (container)->mount_fds;

  अगर (mount_fds == शून्य)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_तथ्य (container)->mount_fds = mount_fds;
    }
  वापस mount_fds;
}

अटल पूर्णांक
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free अक्षर *mnt_तथ्य = शून्य;
  अचिन्हित दीर्घ mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  स्थिर अक्षर *label_type = शून्य;
  स्थिर अक्षर *label_val = शून्य;
  स्थिर अक्षर *fstype;
  पूर्णांक mnt_fd;
  माप_प्रकार j;

  अगर (mount->type == शून्य)
    वापस -1;

  fstype = mount->type;

  क्रम (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_तथ्य,
                                            &mnt_rec_clear, &mnt_rec_set);

  अगर (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = म_भेद (fstype, "sysfs") == 0
                              || म_भेद (fstype, "proc") == 0;
      पूर्णांक extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      अगर (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          अगर (label_type == शून्य)
            crun_error_release (err);
          अन्यथा
            label_val = def->linux->mount_label;
        }
    }

  अगर (म_भेद (fstype, "cgroup") == 0)
    {
      पूर्णांक cgroup_mode = libcrun_get_cgroup_mode (err);
      अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_तथ्य);
  अगर (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, शून्य, शून्य, mnt_तथ्य);

  वापस mnt_fd;
}

#अगर_अघोषित LSMT_ROOT
#  define LSMT_ROOT 0xffffffffffffffffULL
#पूर्ण_अगर
#अगर_अघोषित STATMOUNT_FS_TYPE
#  define STATMOUNT_FS_TYPE 0x00000020U
#पूर्ण_अगर
#अगर_अघोषित STATMOUNT_MNT_ROOT
#  define STATMOUNT_MNT_ROOT 0x00000040U
#पूर्ण_अगर
#अगर_अघोषित STATMOUNT_MNT_POINT
#  define STATMOUNT_MNT_POINT 0x00000080U
#पूर्ण_अगर

/* Declared here so that the build does not depend on the UAPI headers being
   recent enough; the layout is stable.  */
काष्ठा mnt_id_req_s
{
  uint32_t आकार;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

काष्ठा statmount_s
{
  uint32_t आकार;
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
  अक्षर str[];
};

अटल पूर्णांक
syscall_listmount (काष्ठा mnt_id_req_s *req, uint64_t *ids, माप_प्रकार nr, अचिन्हित पूर्णांक flags)
{
#अगर_घोषित __NR_listmount
  वापस (पूर्णांक) syscall (__NR_listmount, req, ids, nr, flags);
#अन्यथा
  (व्योम) req;
  (व्योम) ids;
  (व्योम) nr;
  (व्योम) flags;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

अटल पूर्णांक
syscall_statmount (काष्ठा mnt_id_req_s *req, काष्ठा statmount_s *buf, माप_प्रकार आकार, अचिन्हित पूर्णांक flags)
{
#अगर_घोषित __NR_statmount
  वापस (पूर्णांक) syscall (__NR_statmount, req, buf, आकार, flags);
#अन्यथा
  (व्योम) req;
  (व्योम) buf;
  (व्योम) आकार;
  (व्योम) flags;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
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
अटल bool
mounts_hit_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = शून्य;
  माप_प्रकार i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  क्रम (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      अगर (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != शून्य && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          अवरोध;
        }
    }
  अगर (! has_absolute_bind)
    वापस false;

  ids = xmalloc (capacity * माप (uint64_t));
  क्रम (;;)
    {
      काष्ठा mnt_id_req_s req = {
        .आकार = माप (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      पूर्णांक ret;

      अगर (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * माप (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      अगर (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        वापस true;
      अगर (ret == 0)
        अवरोध;

      n_ids += (माप_प्रकार) ret;
      last = ids[n_ids - 1];
    }

  क्रम (i = 0; i < n_ids; i++)
    {
      अक्षर बफर[4096] __attribute__ ((aligned (8)));
      काष्ठा statmount_s *sm = (काष्ठा statmount_s *) बफर;
      काष्ठा mnt_id_req_s req = {
        .आकार = माप (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      स्थिर अक्षर *मूल, *point;
      माप_प्रकार j;

      अगर (syscall_statmount (&req, sm, माप (बफर), 0) < 0)
        जारी;
      अगर ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        जारी;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      मूल = sm->str + sm->mnt_root;
      अगर (! has_prefix (मूल, "mnt:["))
        जारी;

      point = sm->str + sm->mnt_point;

      क्रम (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          स्थिर अक्षर *source = def->mounts[j]->source;
          माप_प्रकार source_len;

          अगर (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            जारी;
          अगर (source == शून्य || source[0] != '/')
            जारी;

          source_len = म_माप (source);
          जबतक (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          अगर (म_भेदन (point, source, source_len) != 0)
            जारी;

          /* The source itself is a pinned mount namespace.  */
          अगर (point[source_len] == '\0')
            वापस true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          अगर (recursive && (source_len == 1 || point[source_len] == '/'))
            वापस true;
        }
    }

  वापस false;
}

अटल bool
can_use_open_वृक्ष_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  काष्ठा libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_तथ्य (container)->unshare_flags & CLONE_NEWUSER;
  माप_प्रकार i;

  अगर (has_hooks || has_userns)
    वापस false;

  अगर (mounts_hit_pinned_mount_namespace (def))
    वापस false;

  अगर (container->context->no_open_वृक्ष_namespace)
    वापस false;

  {
    libcrun_error_t tmp_err = शून्य;
    पूर्णांक in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    अगर (in_userns != 0)
      वापस false;
  }

  mount_fds = get_fd_map (container);
  क्रम (i = 0; i < def->mounts_len; i++)
    {
      अगर (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_तथ्य (container)->copy_symlink_targets
                                   && i < get_private_तथ्य (container)->n_copy_symlink_targets
                                   && get_private_तथ्य (container)->copy_symlink_targets[i];
          अगर (! has_cached_target)
            वापस false;
        }
    }

  वापस true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
अटल bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  माप_प्रकार i;

  अगर (mount->type == शून्य || म_भेद (mount->type, "tmpfs") != 0)
    वापस false;

  क्रम (i = 0; i < mount->options_len; i++)
    अगर (mount->options[i] != शून्य && has_prefix (mount->options[i], "mode="))
      वापस false;

  वापस true;
}

अटल पूर्णांक
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, अक्षर **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  अचिन्हित दीर्घ rootfs_propagation = 0;
  cleanup_close पूर्णांक rootfsfd = -1;
  cleanup_close पूर्णांक वृक्ष_fd = -1;
  libcrun_error_t tmp_err = शून्य;
  bool use_open_वृक्ष = false;
  माप_प्रकार i;
  पूर्णांक ret;

  अगर (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, शून्य, शून्य, शून्य, शून्य);

  अगर ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_तथ्य (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  मुक्त (get_private_तथ्य (container)->host_rootfs);
  get_private_तथ्य (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  क्रम (i = 0; i < def->mounts_len; i++)
    {
      काष्ठा libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close पूर्णांक mnt_fd = -1;
      bool recursive_bind, nofollow;
      अचिन्हित दीर्घ mnt_flags = 0;

      अगर (mount_fds && mount_fds->fds[i] >= 0)
        जारी;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      अगर (def->mounts[i]->source != शून्य
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free अक्षर *mnt_तथ्य = शून्य;
          अचिन्हित दीर्घ mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          माप_प्रकार j;

          क्रम (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_तथ्य,
                                                    &mnt_rec_clear, &mnt_rec_set);

          अगर (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free अक्षर *target = शून्य;
              libcrun_error_t tmp_err = शून्य;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              अगर (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              अगर (get_private_तथ्य (container)->copy_symlink_targets == शून्य)
                {
                  get_private_तथ्य (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * माप (अक्षर *));
                  get_private_तथ्य (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_तथ्य (container)->copy_symlink_targets[i] = target;
              target = शून्य;
            }
        }

      अगर (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_तथ्य (container)->copy_symlink_targets
                                   && i < get_private_तथ्य (container)->n_copy_symlink_targets
                                   && get_private_तथ्य (container)->copy_symlink_targets[i];

          अगर (mount_fds && def->mounts[i]->source != शून्य && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == शून्य)
            {
              libcrun_error_t tmp_err = शून्य;
              अचिन्हित दीर्घ propagation = MS_PRIVATE;

              अगर (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              अगर (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          जारी;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      अगर (mnt_fd < 0)
        {
          crun_error_release (err);
          जारी;
        }

      अगर (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close पूर्णांक destfd = -1;
          काष्ठा stat st;

          अगर (rootfsfd < 0)
            {
              rootfsfd = खोलो (*rootfs, O_PATH | O_CLOEXEC);
              अगर (UNLIKELY (rootfsfd < 0))
                वापस crun_make_error (err, त्रुटि_सं, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          अगर (destfd < 0)
            crun_error_release (&tmp_err);
          अन्यथा अगर (ख_स्थिति (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              पूर्णांक procfd = get_procfd (get_private_तथ्य (container), err);
              अगर (UNLIKELY (procfd < 0))
                वापस procfd;

              get_self_fd_path (fd_path, mnt_fd);
              ret = fchmodat (procfd, fd_path, st.st_mode & 07777, 0);
              (व्योम) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_empty_dir_cached (container, शून्य, &tmp_err);
  अगर (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_तथ्य (container)->maskdir_bind_failed = true;
    }

  अगर (! get_private_तथ्य (container)->joined_mount_ns
      && can_use_open_वृक्ष_namespace (container))
    use_open_वृक्ष = true;

  अगर (use_open_वृक्ष)
    {
      ret = maybe_open_वृक्ष_namespace (*rootfs, &वृक्ष_fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  अगर (वृक्ष_fd >= 0)
    preopen_needed_devs_and_notify (container);

  अगर (वृक्ष_fd >= 0)
    {
      ret = setns (वृक्ष_fd, CLONE_NEWNS);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "setns `CLONE_NEWNS`");

      ret = mount (शून्य, "/", शून्य, MS_REMOUNT | MS_BIND, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "remount `/`");

      ret = do_mount (container, शून्य, -1, "/", शून्य, MS_REC | MS_PRIVATE, शून्य, LABEL_MOUNT, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = do_mount (container, शून्य, -1, "/", शून्य, rootfs_propagation, शून्य, LABEL_MOUNT, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      get_private_तथ्य (container)->needs_pivot = false;
      get_private_तथ्य (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_तथ्य (container)->host_मूल_switched = true;
      मुक्त (*rootfs);
      *rootfs = xstrdup ("/");
    }
  अन्यथा
    {
      अगर (! get_private_तथ्य (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, शून्य, -1, "/", शून्य, rootfs_propagation, शून्य, LABEL_MOUNT, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = make_parent_mount_private (*rootfs, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, शून्य, MS_BIND | MS_REC | MS_PRIVATE, शून्य, LABEL_MOUNT, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      get_private_तथ्य (container)->needs_pivot = true;
      get_private_तथ्य (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = खोलो (*rootfs, O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", *rootfs);
  get_private_तथ्य (container)->rootfsfd = ret;
  get_private_तथ्य (container)->rootfs = *rootfs;

  अगर (! container->context->force_no_cgroup)
    {
      पूर्णांक cgroup_mode = libcrun_get_cgroup_mode (err);
      अगर (UNLIKELY (cgroup_mode < 0))
        वापस cgroup_mode;

      अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          अक्षर *unified_cgroup_path = शून्य;
          पूर्णांक procfd = get_procfd (get_private_तथ्य (container), err);
          अगर (UNLIKELY (procfd < 0))
            वापस procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          get_private_तथ्य (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  अगर (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_तथ्य (container)->rootfsfd);
      get_private_तथ्य (container)->rootfs = शून्य;
      वापस ret;
    }

  close_and_reset (&get_private_तथ्य (container)->rootfsfd);
  get_private_तथ्य (container)->rootfs = शून्य;

  अगर (वृक्ष_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_तथ्य (container), &tmp_err);
  अगर (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  वापस 0;
}

पूर्णांक
libcrun_do_pivot_मूल (libcrun_container_t *container, bool no_pivot, अक्षर **rootfs, libcrun_error_t *err)
{
  पूर्णांक ret;

  अगर (*rootfs == शून्य)
    वापस 0;

  अगर (get_private_तथ्य (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      अगर (म_भेद (*rootfs, "/") == 0)
        {
          ret = स_बदलो ("/");
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "chdir to `/`");
        }
    }
  अन्यथा
    {
      ret = chroot (*rootfs);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "chroot to `%s`", *rootfs);

      ret = स_बदलो ("/");
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "chdir to `/`");

      मुक्त (*rootfs);
      *rootfs = xstrdup ("/");
    }

  वापस 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
पूर्णांक
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  काष्ठा stat statbuf;
  cleanup_close पूर्णांक fd;
  पूर्णांक ret;
  पूर्णांक i;

  /* Open /dev/null inside of the container. */
  fd = खोलो ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  अगर (UNLIKELY (fd == -1))
    वापस crun_make_error (err, त्रुटि_सं, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  क्रम (i = 0; i <= 2; i++)
    {
      अगर (UNLIKELY (ख_स्थिति (i, &statbuf) == -1))
        वापस crun_make_error (err, त्रुटि_सं, "stat fd `%d`", i);
      अगर (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          अगर (UNLIKELY (dup2 (fd, i) == -1))
            वापस crun_make_error (err, त्रुटि_सं, "dup2 `%d`", i);
        }
    }
  वापस 0;
}

अटल पूर्णांक
uidgidmap_helper (अक्षर *helper, pid_t pid, स्थिर अक्षर *map_file, libcrun_error_t *err)
{
#घोषणा MAX_ARGS 20
  अक्षर pid_fmt[16];
  अक्षर *args[MAX_ARGS + 1];
  अक्षर *अग्र;
  cleanup_free अक्षर *map_file_copy = xstrdup (map_file);
  माप_प्रकार nargs = 0;
  पूर्णांक ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, माप (pid_fmt), "%d", pid);
  अगर (UNLIKELY (ret >= (पूर्णांक) माप (pid_fmt)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  अग्र = map_file_copy;
  जबतक (nargs < MAX_ARGS)
    {
      अक्षर *p = strsep (&अग्र, " \n");
      अगर (अग्र == शून्य)
        अवरोध;
      args[nargs++] = p;
    }
  args[nargs++] = शून्य;

  ret = run_process (args, err);
  अगर (ret < 0)
    वापस ret;
  अगर (ret > 0)
    वापस crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  वापस 0;
}

अटल पूर्णांक
newgidmap (pid_t pid, स्थिर अक्षर *map_file, libcrun_error_t *err)
{
  वापस uidgidmap_helper ("newgidmap", pid, map_file, err);
}

अटल पूर्णांक
newuidmap (pid_t pid, स्थिर अक्षर *map_file, libcrun_error_t *err)
{
  वापस uidgidmap_helper ("newuidmap", pid, map_file, err);
}

अटल पूर्णांक
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_close पूर्णांक fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  अगर (UNLIKELY (fd < 0))
    वापस fd;

  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (fd, "deny", 4));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "write to `/proc/%d/setgroups`", pid);

  get_private_तथ्य (container)->deny_setgroups = true;
  वापस 0;
}

अटल पूर्णांक
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *content = शून्य;

  अगर (get_private_तथ्य (container)->deny_setgroups)
    वापस 0;

  अगर (container->container_def->annotations)
    {
      स्थिर अक्षर *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      अगर (annotation)
        वापस म_भेद (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close पूर्णांक fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    अगर (fd < 0)
      ret = fd;
    अन्यथा
      ret = read_all_fd (fd, "self/setgroups", &content, शून्य, err);
  }
  अगर (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      अगर (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          वापस 1;
        }
      वापस ret;
    }

  वापस म_भेदन (content, "deny", 4) == 0 ? 0 : 1;
}

पूर्णांक
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = शून्य;
  माप_प्रकार additional_gids_len = 0;
  पूर्णांक can_do_setgroups;
  पूर्णांक ret;

  अगर (process != शून्य && process->user != शून्य)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  अगर (UNLIKELY (can_do_setgroups < 0))
    वापस can_do_setgroups;

  अगर (can_do_setgroups == 0)
    {
      स्थिर अक्षर *annotation;
      माप_प्रकार i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      अगर (annotation && म_भेद (annotation, "0") != 0)
        वापस 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      क्रम (i = 0; i < additional_gids_len; i++)
        अगर (additional_gids[i] != process->user->gid)
          वापस crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (पूर्णांक) additional_gids[i]);
      वापस 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "setgroups");

  वापस 0;
}

पूर्णांक
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  अगर (get_private_तथ्य (container)->unshare_cgroupns)
    {
      पूर्णांक ret = unshare (CLONE_NEWCGROUP);
      अगर (UNLIKELY (ret < 0))
        {
          अगर (त्रुटि_सं != EINVAL)
            वापस crun_make_error (err, त्रुटि_सं, "unshare (CLONE_NEWCGROUP)");
        }
    }
  वापस 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
पूर्णांक
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, माप_प्रकार len,
                   uint32_t host_id, uint32_t container_id)
{
  अगर (len != 1)
    वापस 0;

  अगर (mappings[0]->आकार != 1)
    वापस 0;

  अगर (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    वापस 0;

  वापस 1;
}

अटल पूर्णांक
write_id_map (libcrun_container_t *container, pid_t pid, स्थिर अक्षर *map_name,
              पूर्णांक (*idmap_helper) (pid_t, स्थिर अक्षर *, libcrun_error_t *), स्थिर अक्षर *helper_name,
              bool warn_on_failure, स्थिर अक्षर *map, माप_प्रकार map_len,
              runtime_spec_schema_defs_id_mapping **mappings, माप_प्रकार mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  पूर्णांक ret = 0;

  अगर (container->host_uid)
    ret = idmap_helper (pid, map, err);
  अगर (container->host_uid == 0 || ret < 0)
    {
      cleanup_close पूर्णांक fd = -1;

      अगर (ret < 0)
        {
          अगर (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      अगर (UNLIKELY (fd < 0))
        वापस fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      अगर (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          माप_प्रकार single_mapping_len;
          cleanup_free अक्षर *single_mapping = शून्य;
          crun_error_release (err);

          अगर (! skip_if_setgroups_denied || ! get_private_तथ्य (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              अगर (UNLIKELY (ret < 0))
                वापस ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          अगर (UNLIKELY (fd < 0))
            वापस fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  वापस ret;
}

पूर्णांक
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free अक्षर *uid_map = शून्य;
  cleanup_free अक्षर *gid_map = शून्य;
  माप_प्रकार uid_map_len = 0, gid_map_len = 0;
  पूर्णांक ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  अगर ((get_private_तथ्य (container)->unshare_flags & CLONE_NEWUSER) == 0)
    वापस 0;

  अगर (! def || ! def->linux)
    वापस 0;

  अगर (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      uid_map_len = (माप_प्रकार) ret;
      अगर (uid_map == शून्य)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }

  अगर (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      gid_map_len = (माप_प्रकार) ret;
      अगर (gid_map == शून्य)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

#घोषणा CAP_TO_MASK_0(x) (1L << ((x) & 31))
#घोषणा CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

काष्ठा all_caps_s
{
  अचिन्हित दीर्घ effective[2];
  अचिन्हित दीर्घ permitted[2];
  अचिन्हित दीर्घ inheritable[2];
  अचिन्हित दीर्घ ambient[2];
  अचिन्हित दीर्घ bounding[2];
};

अटल पूर्णांक
has_cap_on (पूर्णांक cap, दीर्घ अचिन्हित *caps)
{
  अगर (cap < 32)
    वापस CAP_TO_MASK_0 (cap) & caps[0];
  वापस (CAP_TO_MASK_1 (cap) & caps[1]);
}

अटल अचिन्हित दीर्घ cap_last_cap;

पूर्णांक
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret;
  अक्षर बफर[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  अगर (fd < 0)
    वापस fd;
  ret = अस्थायी_FAILURE_RETRY (पढ़ो (fd, बफर, माप (बफर) - 1));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "read from `/proc/sys/kernel/cap_last_cap`");
  बफर[ret] = '\0';

  त्रुटि_सं = 0;
  cap_last_cap = म_से_अदीर्घ (बफर, शून्य, 10);
  अगर (त्रुटि_सं != 0)
    वापस crun_make_error (err, त्रुटि_सं, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", बफर);
  वापस 0;
}

अटल पूर्णांक
set_required_caps (काष्ठा all_caps_s *caps, uid_t uid, gid_t gid, पूर्णांक no_new_privs, bool no_exec, libcrun_error_t *err)
{
#अगर_घोषित HAVE_CAP
  अचिन्हित दीर्घ cap;
  पूर्णांक ret;
  काष्ठा __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  काष्ठा __user_cap_data_struct तथ्य[2] = { { 0 } };

  अगर (cap_last_cap == 0)
    वापस crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  क्रम (cap = 0; cap <= cap_last_cap; cap++)
    अगर (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        अगर (UNLIKELY (ret < 0 && ! (त्रुटि_सं == EINVAL)))
          वापस crun_make_error (err, त्रुटि_सं, "prctl drop bounding");
      }

  तथ्य[0].effective = caps->effective[0];
  तथ्य[1].effective = caps->effective[1];
  तथ्य[0].inheritable = caps->inheritable[0];
  तथ्य[1].inheritable = caps->inheritable[1];
  तथ्य[0].permitted = caps->permitted[0];
  तथ्य[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  अगर (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    वापस crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "setresuid to `%d`", uid);

  ret = capset (&hdr, तथ्य);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  अगर (UNLIKELY (ret < 0 && ! (त्रुटि_सं == EINVAL || त्रुटि_सं == EPERM)))
    वापस crun_make_error (err, त्रुटि_सं, "prctl reset ambient");

  क्रम (cap = 0; cap <= cap_last_cap; cap++)
    अगर (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        अगर (UNLIKELY (ret < 0 && ! (त्रुटि_सं == EINVAL || त्रुटि_सं == EPERM)))
          वापस crun_make_error (err, त्रुटि_सं, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  अगर (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "prctl reset KEEPCAPS");

      अगर (uid != 0)
        {
          तथ्य[0].permitted = तथ्य[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          क्रम (cap = 0; cap <= cap_last_cap; cap++)
            अगर (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              तथ्य[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          तथ्य[0].effective = तथ्य[0].permitted;
          तथ्य[1].effective = तथ्य[1].permitted;

          ret = capset (&hdr, तथ्य);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "capset");
        }
    }
#पूर्ण_अगर

  अगर (no_new_privs)
    अगर (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      वापस crun_make_error (err, त्रुटि_सं, "prctl set no new privs");

  वापस 0;
}

अटल पूर्णांक
read_caps (अचिन्हित दीर्घ caps[2], अक्षर **मूल्यावली, माप_प्रकार len, libcrun_error_t *err)
{
#अगर_घोषित HAVE_CAP
  माप_प्रकार i;
  क्रम (i = 0; i < len; i++)
    {
      cap_मूल्य_t cap;
      अगर (cap_from_name (मूल्यावली[i], &cap) < 0)
        वापस crun_make_error (err, 0, "unknown capability `%s`", मूल्यावली[i]);

      अगर (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      अन्यथा
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#अन्यथा
  caps[0] = 0;
  caps[1] = 0;
#पूर्ण_अगर
  वापस 0;
}

पूर्णांक
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  अगर (proc->selinux_label)
    वापस set_selinux_label (container, proc->selinux_label, now, err);

  वापस 0;
}

पूर्णांक
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  अगर (proc->apparmor_profile)
    वापस set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  वापस 0;
}

पूर्णांक
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  पूर्णांक no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  काष्ठा all_caps_s caps = {};
  पूर्णांक ret;

  अगर (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

काष्ठा rlimit_s
{
  स्थिर अक्षर *name;
  पूर्णांक मूल्य;
};

काष्ठा rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
                              { "RLIMIT_CORE", RLIMIT_CORE },
                              { "RLIMIT_CPU", RLIMIT_CPU },
                              { "RLIMIT_DATA", RLIMIT_तथ्य },
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
                              { "RLIMIT_STACK", RLIMIT_ढेर },
                              { शून्य, 0 } };

अटल पूर्णांक
get_rlimit_resource (स्थिर अक्षर *name)
{
  काष्ठा rlimit_s *it;
  क्रम (it = rlimits; it->name; it++)
    अगर (म_भेद (it->name, name) == 0)
      वापस it->मूल्य;
  वापस -1;
}

पूर्णांक
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, माप_प्रकार len,
                     libcrun_error_t *err)
{
  माप_प्रकार i, j;
  /* Check for duplicate rlimit types.  */
  क्रम (i = 0; i < len; i++)
    {
      क्रम (j = i + 1; j < len; j++)
        {
          अगर (म_भेद (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            वापस crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  क्रम (i = 0; i < len; i++)
    {
      काष्ठा rlimit limit;
      अक्षर *type = new_rlimits[i]->type;
      पूर्णांक resource = get_rlimit_resource (type);
      अगर (UNLIKELY (resource < 0))
        वापस crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (अचिन्हित दीर्घ दीर्घ) limit.rlim_cur,
                     (अचिन्हित दीर्घ दीर्घ) limit.rlim_max);
      अगर (UNLIKELY (setrlimit (resource, &limit) < 0))
        वापस crun_make_error (err, त्रुटि_सं, "setrlimit `%s`", type);
    }
  वापस 0;
}

पूर्णांक
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक has_uts = get_private_तथ्य (container)->unshare_flags & CLONE_NEWUTS;
  पूर्णांक ret;
  अगर (def->hostname == शून्य || def->hostname[0] == '\0')
    वापस 0;
  अगर (! has_uts)
    वापस crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, म_माप (def->hostname));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "sethostname");
  वापस 0;
}

पूर्णांक
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक has_uts = get_private_तथ्य (container)->unshare_flags & CLONE_NEWUTS;
  पूर्णांक ret;
  अगर (is_empty_string (def->domainname))
    वापस 0;
  अगर (! has_uts)
    वापस crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, म_माप (def->domainname));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "setdomainname");
  वापस 0;
}

पूर्णांक
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret;
  अक्षर oom_बफर[16];
  अगर (def->process == शून्य || ! def->process->oom_score_adj_present)
    वापस 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_बफर, माप (oom_बफर), "%i", def->process->oom_score_adj);
  अगर (UNLIKELY (ret >= (पूर्णांक) माप (oom_बफर)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  अगर (fd < 0)
    वापस fd;
  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (fd, oom_बफर, म_माप (oom_बफर)));
  अगर (ret < 0)
    वापस crun_make_error (err, त्रुटि_सं, "write to `/proc/self/oom_score_adj`");
  वापस 0;
}

स्थिर अक्षर *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  शून्य
};

अटल पूर्णांक
validate_sysctl (स्थिर अक्षर *original_कुंजी, स्थिर अक्षर *original_मूल्य, स्थिर अक्षर *name, अचिन्हित दीर्घ namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  स्थिर अक्षर *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  अगर (name[0] == '/' || path_has_dot_dot_component (name))
    वापस crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_कुंजी);

  अगर (has_prefix (name, "fs/mqueue/"))
    {
      अगर (namespaces_created & CLONE_NEWIPC)
        वापस 0;

      namespace = "IPC";
      जाओ fail;
    }

  अगर (has_prefix (name, "kernel/"))
    {
      माप_प्रकार i;

      क्रम (i = 0; sysctlRequiringIPC[i]; i++)
        अगर (म_भेद (sysctlRequiringIPC[i], name) == 0)
          {
            अगर (namespaces_created & CLONE_NEWIPC)
              वापस 0;

            namespace = "IPC";
            जाओ fail;
          }

      अगर (म_भेद (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          अगर (! is_empty_string (def->domainname) && (म_भेद (original_मूल्य, def->domainname) != 0))
            वापस crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_कुंजी);

          अगर (namespaces_created & CLONE_NEWUTS)
            वापस 0;

          namespace = "UTS";
          जाओ fail;
        }

      अगर (म_भेद (name, "kernel/hostname") == 0)
        वापस crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_कुंजी);
    }
  अगर (has_prefix (name, "net/"))
    {
      अगर (namespaces_created & CLONE_NEWNET)
        वापस 0;

      namespace = "network";
      जाओ fail;
    }

  वापस crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_कुंजी);

fail:
  वापस crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_कुंजी, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
अटल अक्षर *
sysctl_error_reason (स्थिर अक्षर *name, पूर्णांक namespaces_created, पूर्णांक errno_)
{

  अगर (म_भेद (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    वापस xstrdup ("are all the IDs mapped in the user namespace?");

  वापस शून्य;
}

पूर्णांक
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  माप_प्रकार i;
  cleanup_close पूर्णांक dirfd = -1;
  अचिन्हित दीर्घ namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  अगर (def->linux == शून्य || def->linux->sysctl == शून्य || def->linux->sysctl->len == 0)
    वापस 0;

  क्रम (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णांक मूल्य;

      मूल्य = libcrun_find_namespace (def->linux->namespaces[i]->type);
      अगर (UNLIKELY (मूल्य < 0))
        वापस crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= मूल्य;
    }

  get_private_तथ्य (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  अगर (UNLIKELY (dirfd < 0))
    वापस dirfd;

  क्रम (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free अक्षर *name = शून्य;
      cleanup_close पूर्णांक fd = -1;
      पूर्णांक ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      अगर (def->linux->sysctl->कुंजियाँ[i] == शून्य || def->linux->sysctl->मूल्यावली[i] == शून्य)
        वापस crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_कुंजी_to_proc_path (def->linux->sysctl->कुंजियाँ[i]);

      ret = validate_sysctl (def->linux->sysctl->कुंजियाँ[i], def->linux->sysctl->मूल्यावली[i], name, namespaces_created, def, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      अगर (UNLIKELY (fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `/proc/sys/%s`", name);

      ret = अस्थायी_FAILURE_RETRY (ग_लिखो (fd, def->linux->sysctl->मूल्यावली[i], म_माप (def->linux->sysctl->मूल्यावली[i])));
      अगर (UNLIKELY (ret < 0))
        {
          पूर्णांक saved_errno = त्रुटि_सं;
          cleanup_free अक्षर *reason = शून्य;

          reason = sysctl_error_reason (def->linux->sysctl->कुंजियाँ[i], namespaces_created, saved_errno);
          वापस crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  वापस 0;
}

अटल पूर्णांक
open_terminal (अक्षर **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  uid_t uid = 0;
  पूर्णांक ret;

  अगर (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  अगर (UNLIKELY (fd < 0))
    वापस fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  वापस ret;
}

अक्षर *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  वापस get_private_तथ्य (container)->external_descriptors;
}

पूर्णांक
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  स्थिर अक्षर *buf = शून्य;
  json_gen_ctx *gen = शून्य;
  माप_प्रकार buf_len;
  पूर्णांक ret;
  पूर्णांक i;

  अगर (! json_gen_init (&gen, शून्य))
    वापस crun_make_error (err, त्रुटि_सं, "json_gen_init");

  ret = json_gen_array_open (gen);
  अगर (UNLIKELY (ret != json_gen_status_ok))
    जाओ json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  क्रम (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      अक्षर link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      अगर (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          अगर (त्रुटि_सं == ENOENT)
            {
              म_नकल (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          अन्यथा
            {
              json_gen_free (gen);
              वापस crun_make_error (err, त्रुटि_सं, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      अगर (UNLIKELY (ret != json_gen_status_ok))
        जाओ json_error;
    }

  ret = json_gen_array_close (gen);
  अगर (UNLIKELY (ret != json_gen_status_ok))
    जाओ json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  अगर (UNLIKELY (ret != json_gen_status_ok))
    जाओ json_error;

  अगर (buf)
    {
      अक्षर *b = xmalloc (buf_len + 1);
      स_नकल (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_तथ्य (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  वापस 0;

json_error:
  अगर (gen)
    json_gen_free (gen);
  वापस json_gen_error_to_crun_error (ret, err);
}

पूर्णांक
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free अक्षर *pty = शून्य;
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret;

  अगर (def->process == शून्य || ! def->process->terminal)
    वापस 0;

  fd = open_terminal (&pty, def->process, err);
  अगर (UNLIKELY (fd < 0))
    वापस fd;

  अगर (def->process->console_आकार)
    {
      ret = libcrun_terminal_setup_आकार (0, def->process->console_आकार->height, def->process->console_आकार->width, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (get_private_तथ्य (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", शून्य, MS_BIND, शून्य, LABEL_MOUNT, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      ret = unlink ("/dev/console");
      अगर (UNLIKELY (ret < 0 && त्रुटि_सं != ENOENT))
        {
          अगर (त्रुटि_सं == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", शून्य, MS_BIND, शून्य, LABEL_MOUNT, err);
              अगर (UNLIKELY (ret < 0))
                वापस ret;

              वापस get_and_reset (&fd);
            }
          वापस crun_make_error (err, त्रुटि_सं, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "symlink `/dev/console` -> `%s`", pty);
    }

  वापस get_and_reset (&fd);
}

अटल bool
read_error_from_sync_socket (पूर्णांक sync_socket_fd, पूर्णांक *error, अक्षर **str)
{
  cleanup_free अक्षर *b = शून्य;
  माप_प्रकार आकार;
  पूर्णांक code = 0;
  पूर्णांक ret;

  अगर (*error == 0)
    {
      ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_socket_fd, &code, माप (code)));
      अगर (UNLIKELY (ret != (पूर्णांक) माप (code)))
        वापस false;
      *error = code;
    }

  ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_socket_fd, &आकार, माप (आकार)));
  अगर (UNLIKELY (ret != (पूर्णांक) माप (आकार)))
    वापस false;

  अगर (आकार == 0)
    वापस false;

  अगर (आकार > 1024)
    आकार = 1024;

  b = xmalloc (आकार + 1);
  ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_socket_fd, b, आकार));
  अगर (UNLIKELY (ret < 0))
    वापस false;

  b[ret] = '\0';

  *str = b;
  b = शून्य;
  वापस true;
}

अटल bool
send_error_to_sync_socket (पूर्णांक sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  माप_प्रकार आकार;
  पूर्णांक ret;
  पूर्णांक code;
  अक्षर *संदेश;

  अगर (err == शून्य || *err == शून्य)
    वापस false;

  code = crun_error_get_errno (err);

  अगर (has_fd)
    {
      /* dummy terminal fd.  */
      ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket_fd, "1", 1));
      अगर (UNLIKELY (ret < 0))
        वापस false;
    }

  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket_fd, &code, माप (code)));
  अगर (UNLIKELY (ret < 0))
    वापस false;

  संदेश = (*err)->संदेश;
  आकार = म_माप (संदेश) + 1;
  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket_fd, &आकार, माप (आकार)));
  अगर (UNLIKELY (ret < 0))
    वापस false;

  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket_fd, संदेश, आकार));
  अगर (UNLIKELY (ret < 0))
    वापस false;

  वापस true;
}

अटल अंतरभूत पूर्णांक
send_success_to_sync_socket (पूर्णांक sync_socket, libcrun_error_t *err)
{
  स्थिर पूर्णांक success = 0;
  पूर्णांक ret;

  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket, &success, माप (success)));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "write to sync socket");

  वापस 0;
}

अटल __attribute__ ((noreturn)) व्योम
send_error_to_sync_socket_and_die (पूर्णांक sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  अक्षर *संदेश;

  अगर (err == शून्य || *err == शून्य)
    _safe_exit (निकास_त्रुटि);

  अगर (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      त्रुटि_सं = crun_error_get_errno (err);
      संदेश = (*err)->संदेश;
      libcrun_fail_with_error (त्रुटि_सं, "%s", संदेश);
    }
  _safe_exit (निकास_त्रुटि);
}

अटल पूर्णांक
expect_success_from_sync_socket (पूर्णांक sync_fd, libcrun_error_t *err)
{
  cleanup_free अक्षर *err_str = शून्य;
  पूर्णांक res = 1;
  पूर्णांक ret;

  ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_fd, &res, माप (res)));
  अगर (UNLIKELY (ret != माप (res)))
    वापस crun_make_error (err, ret < 0 ? त्रुटि_सं : 0, "read status from sync socket");

  अगर (res == 0)
    वापस 0;

  अगर (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      अगर (! is_empty_string (err_str))
        वापस crun_make_error (err, res, "%s", err_str);
    }

  वापस crun_make_error (err, res, "read from sync socket");
}

अटल पूर्णांक
join_namespaces (runtime_spec_schema_config_schema *def, पूर्णांक *namespaces_to_join, पूर्णांक n_namespaces_to_join,
                 पूर्णांक *namespaces_to_join_सूचकांक, bool ignore_join_errors, libcrun_error_t *err)
{
  पूर्णांक ret;
  पूर्णांक i;

  क्रम (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free अक्षर *cwd = शून्य;
      पूर्णांक orig_सूचकांक = namespaces_to_join_सूचकांक[i];
      पूर्णांक मूल्य;

      अगर (namespaces_to_join[i] < 0)
        जारी;

      /* Skip the user namespace.  */
      मूल्य = libcrun_find_namespace (def->linux->namespaces[orig_सूचकांक]->type);
      अगर (मूल्य == CLONE_NEWUSER)
        जारी;

      अगर (मूल्य == CLONE_NEWNS)
        {
          cwd = getcwd (शून्य, 0);
          अगर (UNLIKELY (cwd == शून्य))
            वापस crun_make_error (err, त्रुटि_सं, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_सूचकांक]->type, def->linux->namespaces[orig_सूचकांक]->path);
      ret = setns (namespaces_to_join[i], मूल्य);
      अगर (UNLIKELY (ret < 0))
        {
          अगर (ignore_join_errors)
            जारी;
          वापस crun_make_error (err, त्रुटि_सं, "setns `%s`", def->linux->namespaces[orig_सूचकांक]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      अगर (मूल्य == CLONE_NEWNS)
        {
          ret = स_बदलो (cwd);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "chdir `%s`", cwd);
        }
    }
  वापस 0;
}

#घोषणा MAX_NAMESPACES 10

काष्ठा init_status_s
{
  /* fd to the namespace to join.  */
  पूर्णांक fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  पूर्णांक सूचकांक[MAX_NAMESPACES];
  /* CLONE_* value.  */
  पूर्णांक मूल्य[MAX_NAMESPACES];
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
  पूर्णांक userns_सूचकांक;
  /* def->linux->namespaces userns.  */
  पूर्णांक userns_सूचकांक_origin;

  /* All namespaces created/joined by the container.  */
  पूर्णांक all_namespaces;

  /* What namespaces are still missing to be created.  */
  पूर्णांक namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  पूर्णांक idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  पूर्णांक idx_timens_to_join_immediately;
};

व्योम
cleanup_free_init_statusp (काष्ठा init_status_s *ns)
{
  माप_प्रकार i;

  अगर (ns == शून्य)
    वापस;

  क्रम (i = 0; i < ns->fd_len; i++)
    अस्थायी_FAILURE_RETRY (बंद (ns->fd[i]));
}

अटल पूर्णांक
configure_init_status (काष्ठा init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  माप_प्रकार i;

  क्रम (i = 0; i < MAX_NAMESPACES + 1; i++)
    ns->fd[i] = -1;

  ns->fd_len = 0;
  ns->all_namespaces = 0;
  ns->namespaces_to_unshare = 0;
  ns->join_pidns = false;
  ns->join_ipcns = false;
  ns->must_fork = false;
  ns->can_delay_pidns_fork = false;
  ns->delayed_userns_create = false;
  ns->userns_सूचकांक = -1;
  ns->userns_सूचकांक_origin = -1;
  ns->idx_pidns_to_join_immediately = -1;
  ns->idx_timens_to_join_immediately = -1;

  क्रम (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णांक मूल्य = libcrun_find_namespace (def->linux->namespaces[i]->type);
      अगर (UNLIKELY (मूल्य < 0))
        वापस crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      अगर (ns->all_namespaces & मूल्य)
        वापस crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= मूल्य;

      अगर (def->linux->namespaces[i]->path == शून्य)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= मूल्य;
        }
      अन्यथा
        {
          पूर्णांक fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          अगर (ns->fd_len >= MAX_NAMESPACES)
            वापस crun_make_error (err, 0, "too many namespaces to join");

          fd = खोलो (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          अगर (UNLIKELY (fd < 0))
            वापस crun_make_error (err, त्रुटि_सं, "open `%s`", def->linux->namespaces[i]->path);

          अगर (मूल्य == CLONE_NEWUSER)
            {
              ns->userns_सूचकांक = ns->fd_len;
              ns->userns_सूचकांक_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->सूचकांक[ns->fd_len] = i;
          ns->मूल्य[ns->fd_len] = मूल्य;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  अगर (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  वापस 0;
}

/* Detect if root is available in the container.  */
अटल bool
मूल_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, माप_प्रकार len)
{
  माप_प्रकार i;

  क्रम (i = 0; i < len; i++)
    अगर (mappings[i]->container_id == 0)
      वापस true;

  वापस false;
}

अटल काष्ठा libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  काष्ठा libcrun_fd_map *dev_fds = get_private_तथ्य (container)->dev_fds;

  अगर (dev_fds == शून्य)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      माप_प्रकार len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_तथ्य (container)->dev_fds = dev_fds;
    }
  वापस dev_fds;
}

#घोषणा अंक_NEEDED_DEVS (माप (needed_devs) / माप (needed_devs[0]) - 1)

अटल काष्ठा libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  काष्ठा libcrun_fd_map *fds = get_private_तथ्य (container)->needed_devs_fds;

  अगर (fds == शून्य)
    {
      fds = make_libcrun_fd_map (अंक_NEEDED_DEVS);
      get_private_तथ्य (container)->needed_devs_fds = fds;
    }
  वापस fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  माप_प्रकार i;

  अगर (recursive)
    *recursive = false;

  अगर (src_nofollow != शून्य)
    *src_nofollow = false;

  क्रम (i = 0; i < mnt->options_len; i++)
    {
      अगर (mnt->options[i] == शून्य)
        जारी;

      अगर (म_भेद (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          अगर (src_nofollow == शून्य || *src_nofollow)
            अवरोध;
        }
      अगर (म_भेद (mnt->options[i], "rbind") == 0)
        {
          अगर (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          अगर (src_nofollow == शून्य || *src_nofollow)
            अवरोध;
        }
      अगर (src_nofollow && म_भेद (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  वापस ret;
}

अटल अक्षर *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  माप_प्रकार i;

  क्रम (i = 0; i < mnt->options_len; i++)
    {
      अगर (mnt->options[i] == शून्य)
        जारी;

      अगर (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          वापस mnt->options[i];
        }
      अगर (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          वापस mnt->options[i];
        }
    }
  वापस शून्य;
}

अटल पूर्णांक
open_mount_type (स्थिर अक्षर *type, पूर्णांक *out_fd, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fsopen_fd = -1;
  cleanup_close पूर्णांक newfs_fd = -1;
  पूर्णांक ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  अगर (UNLIKELY (fsopen_fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, शून्य, शून्य, 0);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  अगर (UNLIKELY (newfs_fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  वापस 0;
}

अटल पूर्णांक
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, पूर्णांक *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close पूर्णांक newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  काष्ठा mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close पूर्णांक fd = -1;
  स्थिर अक्षर *idmap_option;
  bool recursive = false;
  स्थिर अक्षर *options = शून्य;
  bool has_mappings;
  पूर्णांक ret;
  अक्षर *extra_संदेश = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != शून्य);
  अगर (has_mappings_out)
    *has_mappings_out = has_mappings;
  अगर (! has_mappings)
    वापस 0;

  अगर ((mnt->uid_mappings == शून्य) != (mnt->gid_mappings == शून्य))
    वापस crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  अगर (idmap_option)
    {
      options = म_अक्षर (idmap_option, '=');
      अगर (options)
        {
          /* Skip the '=' itself.  */
          options++;
          अगर (options[0] == '\0')
            options = शून्य;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  अगर (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  अगर (UNLIKELY (fd < 0))
    वापस fd;

  अगर (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_वृक्ष (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      अगर (UNLIKELY (newfs_fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", mnt->source);
    }
  अन्यथा
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं == EINVAL)
        {
          extra_संदेश = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      वापस crun_make_error (err, त्रुटि_सं, "mount_setattr `%s`%s", mnt->destination, extra_संदेश);
    }

  *out_fd = get_and_reset (&newfs_fd);
  वापस 0;
}

अटल uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  माप_प्रकार len;
  माप_प्रकार i;

  अगर (! def || ! def->linux)
    जाओ निकास;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  अगर (! mappings)
    जाओ निकास;

  क्रम (i = 0; i < len; i++)
    {
      अगर (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->आकार)
        वापस id - mappings[i]->container_id + mappings[i]->host_id;
    }
निकास:
  वापस is_uid ? get_overflow_uid () : get_overflow_gid ();
}

अटल पूर्णांक
precreate_device (libcrun_container_t *container, पूर्णांक devs_dirfd, माप_प्रकार i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  अक्षर name[64];
  mode_t type;
  dev_t dev;
  पूर्णांक ret;

  ret = snprintf (name, माप (name), "%zu", i);
  अगर (UNLIKELY (ret >= (पूर्णांक) माप (name)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "chown `%s`", device->path);

  वापस get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

अटल पूर्णांक
send_mounts (पूर्णांक sync_socket_host, काष्ठा libcrun_fd_map *fds, माप_प्रकार how_many, माप_प्रकार योग, libcrun_error_t *err)
{
  माप_प्रकार i;
  पूर्णांक ret;

  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket_host, &how_many, माप (how_many)));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "write to sync socket");

  क्रम (i = 0; i < योग; i++)
    {
      अगर (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (अक्षर *) &i, माप (i), err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }
  वापस 0;
}

अटल पूर्णांक
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, पूर्णांक sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map काष्ठा libcrun_fd_map *mount_fds = शून्य;
  bool has_userns = (get_private_तथ्य (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free अक्षर *rootfs = शून्य;
  माप_प्रकार how_many = 0;
  माप_प्रकार i;
  पूर्णांक ret;

  अगर (def->mounts_len == 0)
    वापस 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  अगर (def->मूल && def->मूल->path)
    rootfs = realpath (def->मूल->path, शून्य);

  अगर (! has_userns)
    {
      पूर्णांक is_in_userns = check_running_in_user_namespace (err);
      अगर (UNLIKELY (is_in_userns < 0))
        वापस is_in_userns;

      has_userns = is_in_userns > 0;
    }

  क्रम (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      पूर्णांक mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      अगर (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == शून्य)
        {
          अचिन्हित दीर्घ propagation = 0;

          अगर (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, शून्य, शून्य, शून्य, शून्य) & ALL_PROPAGATIONS_NO_REC;
          अगर (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          अगर (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      अगर (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  वापस send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

अटल पूर्णांक
prepare_and_send_dev_mounts (libcrun_container_t *container, पूर्णांक sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map काष्ठा libcrun_fd_map *dev_fds = शून्य;
  bool has_userns = (get_private_तथ्य (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close पूर्णांक current_mountns = -1;
  cleanup_free अक्षर *state_dir = शून्य;
  cleanup_free अक्षर *devs_path = शून्य;
  cleanup_close पूर्णांक devs_mountfd = -1;
  cleanup_close पूर्णांक targetfd = -1;
  स्थिर अक्षर *context_type = शून्य;
  स्थिर अक्षर *label = शून्य;
  माप_प्रकार how_many = 0;
  माप_प्रकार i;
  पूर्णांक ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  अगर (def->linux == शून्य || def->linux->devices_len == 0)
    वापस 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  अगर (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    वापस send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_मूल : शून्य),
                                     (container->context ? container->context->id : शून्य), err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = सूची_गढ़ो (devs_path, 0700);
  अगर (UNLIKELY (ret < 0 && त्रुटि_सं != EEXIST))
    वापस crun_make_error (err, त्रुटि_सं, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  अगर (UNLIKELY (current_mountns < 0))
    वापस current_mountns;

  ret = unshare (CLONE_NEWNS);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (शून्य, "/", शून्य, MS_REC | MS_PRIVATE, शून्य);
  अगर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, त्रुटि_सं, "mount `MS_REC | MS_PRIVATE`");
      जाओ restore_mountns;
    }

  अगर (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      अगर (UNLIKELY (context_type == शून्य))
        {
          ret = -1;
          जाओ restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", शून्य, context_type, label, शून्य);
  अगर (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      जाओ restore_mountns;
    }

  targetfd = खोलो (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (targetfd < 0)
    {
      ret = crun_make_error (err, त्रुटि_सं, "open `%s`", devs_path);
      जाओ restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, शून्य);
  अगर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, त्रुटि_सं, "fs_move_mount_to `%s`", devs_path);
      जाओ restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (targetfd < 0)
    {
      ret = crun_make_error (err, त्रुटि_सं, "open `%s`", devs_path);
      जाओ restore_mountns;
    }

  क्रम (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      अगर (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          जारी;
        }

      dev_fds->fds[i] = ret;

      अगर (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  अगर (ns_changed && current_mountns >= 0)
    {
      पूर्णांक setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      अगर (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          वापस crun_make_error (err, त्रुटि_सं, "setns `CLONE_NEWNS`");
        }
    }

  वापस ret;
}

अटल पूर्णांक
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, पूर्णांक sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map काष्ठा libcrun_fd_map *fds = शून्य;
  bool has_userns = (get_private_तथ्य (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  माप_प्रकार how_many = 0;
  माप_प्रकार i;

  fds = make_libcrun_fd_map (अंक_NEEDED_DEVS);

  अगर (! has_userns || geteuid () > 0)
    वापस send_mounts (sync_socket_host, fds, 0, अंक_NEEDED_DEVS, err);

  क्रम (i = 0; i < अंक_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      पूर्णांक fd = syscall_open_वृक्ष (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      अगर (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  वापस send_mounts (sync_socket_host, fds, how_many, अंक_NEEDED_DEVS, err);
}

अटल पूर्णांक
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, पूर्णांक sync_socket_host, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

अटल पूर्णांक
receive_mounts (काष्ठा libcrun_fd_map *fds, पूर्णांक sync_socket_container, libcrun_error_t *err)
{
  माप_प्रकार i, how_many = 0;
  पूर्णांक ret;

  अगर (fds->nfds == 0)
    वापस 0;

  ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_socket_container, &how_many, माप (how_many)));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "read from sync socket");

  क्रम (i = 0; i < how_many; i++)
    {
      माप_प्रकार सूचकांक;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (अक्षर *) &सूचकांक, माप (सूचकांक), err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      अगर (सूचकांक >= fds->nfds)
        वापस crun_make_error (err, 0, "invalid mount data received");

      अगर (fds->fds[सूचकांक] >= 0)
        अस्थायी_FAILURE_RETRY (बंद (fds->fds[सूचकांक]));

      fds->fds[सूचकांक] = ret;
    }

  वापस 0;
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
अटल पूर्णांक
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret;

  अगर (! container->host_has_cap_sys_ptrace)
    वापस 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "prctl (PR_SET_DUMPABLE)");

  वापस 0;
}

अटल पूर्णांक
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  पूर्णांक ret;

  अगर (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool मूल_mapped = false;

      अगर (def->linux->uid_mappings_len != 0)
        {
          मूल_mapped = मूल_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          अगर (! मूल_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      अगर (def->linux->gid_mappings_len != 0)
        {
          मूल_mapped = मूल_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          अगर (! मूल_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  अगर (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    वापस crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "setresgid to `%d`", gid);

  वापस 0;
}

अटल bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  वापस mnt->type && म_भेद (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, शून्य, शून्य);
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
अटल पूर्णांक
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  काष्ठा libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close पूर्णांक socket_parent = -1;
  cleanup_close पूर्णांक socket_child = -1;
  cleanup_close पूर्णांक मूल_fd = -1;
  cleanup_close पूर्णांक cwd_fd = -1;
  माप_प्रकार i, missing = 0;
  पूर्णांक saved_errno;
  पूर्णांक sockets[2];
  pid_t pid;
  पूर्णांक ret;

  अगर (def->मूल == शून्य || def->मूल->path == शून्य)
    वापस 0;

  क्रम (i = 0; i < def->mounts_len; i++)
    अगर (is_proc_mount (def->mounts[i]))
      missing++;
  अगर (missing == 0)
    वापस 1;

  मूल_fd = खोलो ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (मूल_fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `/`");

  cwd_fd = खोलो (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (cwd_fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->मूल->path);
  अगर (UNLIKELY (ret < 0))
    वापस 0;

  pid = विभाजन ();
  saved_errno = त्रुटि_सं;
  अगर (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      अगर (get_private_तथ्य (container)->unshare_flags & CLONE_NEWUSER)
        (व्योम) unshare (CLONE_NEWNS);

      क्रम (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close पूर्णांक fd = -1;

          अगर (! is_proc_mount (def->mounts[i]))
            जारी;

          fd = open_mount_of_type (container, def->mounts[i], err);
          अगर (fd < 0)
            _safe_exit (निकास_त्रुटि);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (अक्षर *) &i, माप (i), err);
          अगर (UNLIKELY (ret < 0))
            _safe_exit (निकास_त्रुटि);
        }
      _safe_exit (निकास_सफल);
    }

  अगर (UNLIKELY (fchdir (मूल_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    वापस crun_make_error (err, त्रुटि_सं, "restore the root directory");

  अगर (UNLIKELY (pid < 0))
    वापस crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  जबतक (missing > 0)
    {
      libcrun_error_t tmp_err = शून्य;

      ret = receive_fd_from_socket_with_payload (socket_parent, (अक्षर *) &i, माप (i), &tmp_err);
      अगर (ret < 0)
        {
          crun_error_release (&tmp_err);
          अवरोध;
        }
      अगर (i >= mount_fds->nfds)
        {
          अस्थायी_FAILURE_RETRY (बंद (ret));
          अवरोध;
        }

      अगर (mount_fds->fds[i] >= 0)
        अस्थायी_FAILURE_RETRY (बंद (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, शून्य, 0);

  वापस missing == 0 ? 1 : 0;
}

अटल पूर्णांक
init_container (libcrun_container_t *container, पूर्णांक sync_socket_container, काष्ठा init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  काष्ठा libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close पूर्णांक timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  माप_प्रकार i;
  पूर्णांक ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      अगर (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      अगर (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      अगर (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          delayed_pidns_fork = ret > 0;
        }

      अगर (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket_container, &new_pid, माप (new_pid)));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "write to sync socket");
        }
      अन्यथा
        {
          new_pid = विभाजन ();
          अगर (UNLIKELY (new_pid < 0))
            वापस crun_make_error (err, त्रुटि_सं, "fork");
        }

      अगर (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          अगर (UNLIKELY (ret < 0))
            समाप्त (new_pid, SIGKILL);

          ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket_container, &new_pid, माप (new_pid)));
          अगर (UNLIKELY (ret < 0))
            समाप्त (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = libcrun_set_oom (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->सूचकांक, true, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  अगर ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      क्रम (i = 0; i < def->mounts_len; i++)
        {
          पूर्णांक fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          अगर (def->mounts[i]->type == शून्य)
            जारी;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          अगर (init_status->join_pidns && ! delayed_pidns_fork && म_भेद (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, शून्य, शून्य, शून्य);
          अगर (init_status->join_ipcns && म_भेद (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, शून्य, शून्य, शून्य);

          अगर (fd >= 0)
            {
              अगर (mount_fds->fds[i] >= 0)
                अस्थायी_FAILURE_RETRY (बंद (mount_fds->fds[i]));
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
  अगर (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      अगर (UNLIKELY (timens_offsets_fd < 0))
        वापस timens_offsets_fd;
    }

  अगर (init_status->all_namespaces & CLONE_NEWUSER)
    {
      अगर (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      अगर (init_status->userns_सूचकांक < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
      अन्यथा
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_सूचकांक], CLONE_NEWUSER);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_सूचकांक_origin]->path);
        }

      ret = set_id_init (container, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->सूचकांक, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "unshare");
    }

  अगर (def->linux->time_offsets)
    {
      अक्षर fmt_बफर[128];
      पूर्णांक fd = timens_offsets_fd;

      अगर (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_बफर, माप (fmt_बफर), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          अगर (UNLIKELY (ret >= (पूर्णांक) माप (fmt_बफर)))
            वापस crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (दीर्घ दीर्घ पूर्णांक) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = ग_लिखो (fd, fmt_बफर, म_माप (fmt_बफर));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "write `/proc/self/timens_offsets`");
        }
      अगर (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (दीर्घ दीर्घ पूर्णांक) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_बफर, माप (fmt_बफर), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          अगर (UNLIKELY (ret >= (पूर्णांक) माप (fmt_बफर)))
            वापस crun_make_error (err, 0, "internal error: static buffer too small");

          ret = ग_लिखो (fd, fmt_बफर, म_माप (fmt_बफर));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "write `/proc/self/timens_offsets`");
        }
    }

  अगर (init_status->must_fork)
    {
      अगर (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          delayed_pidns_fork = ret > 0;
        }

      अगर (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = विभाजन ();
          अगर (UNLIKELY (pid_container < 0))
            वापस crun_make_error (err, त्रुटि_सं, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      अगर (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket_container, &pid_container, माप (pid_container)));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "write to sync socket");

          अगर (pid_container)
            _safe_exit (निकास_सफल);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  get_private_तथ्य (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  वापस get_private_तथ्य (container)->delayed_pidns_fork;
}

पूर्णांक
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close पूर्णांक client_fd = -1;
  cleanup_close पूर्णांक pidfd = -1;
  स्थिर अक्षर *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  अगर (v == शून्य)
    वापस 0;

  pidfd = syscall_pidfd_open (pid, 0);
  अगर (UNLIKELY (pidfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  अगर (UNLIKELY (client_fd < 0))
    वापस client_fd;

  वापस send_fd_to_socket (client_fd, pidfd, err);
}

अटल bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  अगर (process == शून्य || process->exec_cpu_affinity == शून्य)
    वापस false;
  वापस (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, व्योम *args,
                             पूर्णांक *sync_socket_out, काष्ठा libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) काष्ठा init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णांक sync_socket_container = -1;
  अक्षर *notify_socket_env = शून्य;
  cleanup_close पूर्णांक sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close पूर्णांक restore_pidns = -1;
  पूर्णांक first_clone_args = 0;
  पूर्णांक sync_socket[2];
  pid_t pid;
  bool new_userns;
  माप_प्रकार i;
  पूर्णांक ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  get_private_तथ्य (container)->unshare_flags = init_status.all_namespaces;
  get_private_तथ्य (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_तथ्य (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#अगर_घोषित HAVE_SYSTEMD
  अगर (def->मूल)
    {
      ret = do_notify_socket (container, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
#पूर्ण_अगर

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  अगर (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = libcrun_set_oom (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->मूल ? container->container_def->मूल->path : शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_सूचकांक < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_सूचकांक < 0;
  क्रम (i = 0; i < init_status.fd_len; i++)
    {
      चयन (init_status.मूल्य[i])
        {
        हाल CLONE_NEWIPC:
          अगर (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          अवरोध;

        हाल CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          अगर (! new_userns)
            init_status.must_fork = true;
          अन्यथा
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          अवरोध;

        हाल CLONE_NEWTIME:
          अगर (! new_userns)
            init_status.must_fork = true;
          अन्यथा
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          अवरोध;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  अगर ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  अन्यथा अगर ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  अगर (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  अगर (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      काष्ठा _clone3_args clone3_args;
      स_रखो (&clone3_args, 0, माप (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      अगर (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  अगर (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, शून्य);
      अगर (UNLIKELY (pid < 0))
        वापस crun_make_error (err, त्रुटि_सं, "clone");
    }

  अगर (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = close_and_reset (&sync_socket_container);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_तथ्य (container)->notify_socket_वृक्ष_fd));

      अगर (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_socket_host, &new_pid, माप (new_pid)));
          अगर (UNLIKELY (ret != माप (new_pid)))
            वापस crun_make_error (err, ret < 0 ? त्रुटि_सं : 0, "read pid from sync socket");

          अगर (new_pid == 0)
            get_private_तथ्य (container)->delayed_pidns_fork = true;
          अन्यथा
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, शून्य, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      अगर (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      अगर ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_सूचकांक < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      अगर (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_socket_host, &grandchild, माप (grandchild)));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          अगर (grandchild == 0)
            get_private_तथ्य (container)->delayed_pidns_fork = true;
          अन्यथा
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, शून्य, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      अगर (! get_private_तथ्य (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      pid_to_clean = 0;
      वापस pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  अगर (UNLIKELY (ret < 0))
    libcrun_fail_with_error (त्रुटि_सं, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  अगर (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  अन्यथा
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      अगर (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->संदेश);
    }

  /* Jump into the specified entrypoint.  */
  अगर (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  अगर (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->संदेश);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  अगर (ret == 0)
    _safe_exit (निकास_सफल);

  _safe_exit (निकास_त्रुटि);
}

अटल पूर्णांक
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, पूर्णांक sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, स्थिर अक्षर *sub_cgroup,
                            पूर्णांक *terminal_fd, libcrun_error_t *err)
{
  पूर्णांक ret;
  अक्षर res;
  pid_t pid;
  cleanup_close पूर्णांक sync_fd = sync_socket_fd;

  अगर (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_fd, &res, 1));
  अगर (UNLIKELY (ret != 1))
    वापस crun_make_error (err, ret < 0 ? त्रुटि_सं : 0, "read from sync socket");

  अगर (res != '0')
    वापस crun_make_error (err, 0, "fail startup");

  ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_fd, &pid, माप (pid)));
  अगर (UNLIKELY (ret != माप (pid)))
    वापस crun_make_error (err, ret < 0 ? त्रुटि_सं : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, शून्य, 0);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "waitpid for exec child pid");

  अगर (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (need_move_to_cgroup)
    {
      cleanup_cgroup_status काष्ठा libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free अक्षर *final_cgroup = शून्य;

      अगर (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, शून्य);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_set_io_priority (pid, process, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_fd, &ret, माप (ret)));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "write to sync socket");

  अगर (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      अगर (UNLIKELY (ret < 0))
        {
          पूर्णांक err_code = 0;
          cleanup_free अक्षर *err_str = शून्य;

          अगर (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              अगर (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  वापस crun_make_error (err, err_code, "%s", err_str);
                }
            }

          वापस crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  वापस pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
अटल पूर्णांक
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णांक pidfd_pid_to_join = -1;
  पूर्णांक all_flags = 0;
  माप_प्रकार i;
  पूर्णांक ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  अगर (def->linux && def->linux->namespaces)
    {
      क्रम (i = 0; i < def->linux->namespaces_len; i++)
        अगर (! is_empty_string (def->linux->namespaces[i]->path))
          वापस 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  अगर (UNLIKELY (pidfd_pid_to_join < 0))
    वापस 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  अगर (ret == 0)
    वापस crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  क्रम (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].मूल्य;

  ret = setns (pidfd_pid_to_join, all_flags);
  अगर (UNLIKELY (ret < 0))
    वापस 0;

  वापस 1;
}

अटल पूर्णांक
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक fds_joined[MAX_NAMESPACES] = {
    0,
  };
  पूर्णांक fds[MAX_NAMESPACES] = {
    -1,
  };
  माप_प्रकार i;
  पूर्णांक ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  /* Nothing left to do if the namespaces were joined.  */
  अगर (LIKELY (ret > 0))
    वापस 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  अगर (def->linux->namespaces_len >= MAX_NAMESPACES)
    वापस crun_make_error (err, 0, "invalid configuration");

  क्रम (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free अक्षर *ns_path = शून्य;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      अगर (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          अगर (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              जारी;
            }

          ret = fds[i];
          जाओ निकास;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  अगर (UNLIKELY (ret < 0))
    जाओ निकास;
  अगर (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      जाओ निकास;
    }

  क्रम (i = 0; namespaces[i].ns_file; i++)
    {
      अगर (namespaces[i].मूल्य == CLONE_NEWUSER)
        जारी;

      ret = setns (fds[i], 0);
      अगर (ret == 0)
        fds_joined[i] = 1;
    }
  क्रम (i = 0; namespaces[i].ns_file; i++)
    {
      अगर (fds_joined[i])
        जारी;
      ret = setns (fds[i], 0);
      अगर (ret == 0)
        fds_joined[i] = 1;
    }
  क्रम (i = 0; namespaces[i].ns_file; i++)
    {
      अगर (fds_joined[i])
        जारी;
      ret = setns (fds[i], 0);
      अगर (UNLIKELY (ret < 0 && त्रुटि_सं != EINVAL))
        {
          माप_प्रकार j;
          bool found = false;

          क्रम (j = 0; j < def->linux->namespaces_len; j++)
            {
              अगर (म_भेद (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  अवरोध;
                }
            }
          अगर (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              जारी;
            }

          ret = crun_make_error (err, त्रुटि_सं, "setns `%s`", namespaces[i].ns_file);
          जाओ निकास;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

निकास:
  क्रम (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  वापस ret;
}

पूर्णांक
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      स्थिर अक्षर *sub_cgroup,
                      पूर्णांक detach,
                      runtime_spec_schema_config_schema_process *process,
                      पूर्णांक *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  पूर्णांक ret;
  पूर्णांक sync_socket_fd[2];
  cleanup_close पूर्णांक cgroup_dirfd = -1;
  cleanup_close पूर्णांक sync_fd = -1;
  काष्ठा _clone3_args clone3_args;
  bool need_move_to_cgroup;

  अगर (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "set child subreaper");
    }

  अगर (path_has_dot_dot_component (sub_cgroup))
    वापस crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  अगर (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free अक्षर *final_cgroup = शून्य;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "error creating socketpair");

  {
    cleanup_cgroup_status काष्ठा libcrun_cgroup_status *cgroup_status = शून्य;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    अगर (get_force_cgroup_v1_annotation (container) == शून्य)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        अगर (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  स_रखो (&clone3_args, 0, माप (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  अगर (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  अन्यथा
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  अगर (pid > 0 && ! (process->scheduler && process->scheduler->policy && म_भेद (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  /* On errors, fall back to fork().  */
  अगर (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = विभाजन ();
      अगर (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, त्रुटि_सं, "fork");
          जाओ निकास;
        }
    }

  अगर (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      वापस join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  अगर (UNLIKELY (ret < 0))
    {
      अस्थायी_FAILURE_RETRY (ग_लिखो (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->संदेश);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = विभाजन ();
  अगर (UNLIKELY (pid < 0))
    {
      पूर्णांक saved_errno = त्रुटि_सं;
      अस्थायी_FAILURE_RETRY (ग_लिखो (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  अगर (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_fd, "0", 1));
      अगर (UNLIKELY (ret < 0))
        {
          समाप्त (pid, SIGKILL);
          _safe_exit (निकास_त्रुटि);
        }

      ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_fd, &pid, माप (pid)));
      अगर (UNLIKELY (ret < 0))
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
      cleanup_free अक्षर *pty = शून्य;
      पूर्णांक r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_fd, &r, माप (r)));
      अगर (UNLIKELY (ret != माप (r) || r < 0))
        _safe_exit (निकास_त्रुटि);

      ret = setsid ();
      अगर (ret < 0)
        {
          crun_make_error (err, त्रुटि_सं, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      अगर (terminal_fd)
        {
          cleanup_close पूर्णांक ptmx_fd = -1;

          ret = set_id_init (container, err);
          अगर (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          अगर (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          अगर (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  वापस 0;

निकास:
  अगर (sync_socket_fd[0] >= 0)
    अस्थायी_FAILURE_RETRY (बंद (sync_socket_fd[0]));
  अगर (sync_socket_fd[1] >= 0)
    अस्थायी_FAILURE_RETRY (बंद (sync_socket_fd[1]));
  वापस ret;
}

पूर्णांक
libcrun_linux_container_update (libcrun_container_status_t *status, स्थिर अक्षर *state_मूल, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status काष्ठा libcrun_cgroup_status *cgroup_status = शून्य;

  cgroup_status = libcrun_cgroup_make_status (status);

  वापस libcrun_update_cgroup_resources (cgroup_status, state_मूल, resources, err);
}

अटल पूर्णांक
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, स्थिर bool छोड़ो, libcrun_error_t *err)
{
  cleanup_cgroup_status काष्ठा libcrun_cgroup_status *cgroup_status = शून्य;

  cgroup_status = libcrun_cgroup_make_status (status);

  वापस libcrun_cgroup_pause_unpause (cgroup_status, छोड़ो, err);
}

पूर्णांक
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  वापस libcrun_container_pause_unpause_linux (status, true, err);
}

पूर्णांक
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  वापस libcrun_container_pause_unpause_linux (status, false, err);
}

पूर्णांक
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  अचिन्हित दीर्घ persona = 0;
  पूर्णांक ret;

  अगर (म_भेद (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  अन्यथा अगर (म_भेद (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  अन्यथा
    वापस crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, 0, "personality to `%s`", p->domain);

  वापस 0;
}

पूर्णांक
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret;
  माप_प्रकार i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णांक sockfd = -1;

  क्रम (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णांक मूल्य = libcrun_find_namespace (def->linux->namespaces[i]->type);
      अगर (UNLIKELY (मूल्य < 0))
        वापस crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      अगर (मूल्य == CLONE_NEWNET && def->linux->namespaces[i]->path == शून्य)
        {
          configure_network = true;
          अवरोध;
        }
    }

  अगर (! configure_network)
    वापस 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  अगर (LIKELY (sockfd >= 0))
    {
      काष्ठा ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "ioctl SIOCSIFFLAGS");
    }
  अन्यथा
    {
      काष्ठा nlmsghdr *hdr_recv;
      अक्षर buf[माप (काष्ठा nlmsghdr) + माप (काष्ठा nlmsgerr)];
      काष्ठा sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      काष्ठा sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      काष्ठा nlmsghdr hdr = {
        .nlmsg_len = माप (काष्ठा nlmsghdr) + माप (काष्ठा ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      काष्ठा ifinfomsg संदेश = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_सूचकांक = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((काष्ठा nlmsghdr *) buf) = hdr;
      *((काष्ठा ifinfomsg *) (buf + माप (काष्ठा nlmsghdr))) = संदेश;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      अगर (UNLIKELY (sockfd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "socket PF_NETLINK");

      ret = bind (sockfd, (काष्ठा sockaddr *) &addr, माप (addr));
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, माप (buf), 0, (काष्ठा sockaddr *) &addr_to, माप (addr_to));
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, माप (buf), 0, शून्य, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "recvfrom PF_NETLINK");

      hdr_recv = (काष्ठा nlmsghdr *) buf;

      अगर (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          काष्ठा nlmsgerr *err_तथ्य = (काष्ठा nlmsgerr *) NLMSG_तथ्य (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          अगर (err_तथ्य->error < 0)
            वापस crun_make_error (err, -err_तथ्य->error, "netlink error while configuring network");
        }
    }

  वापस 0;
}

/* Protection for attacks like CVE-2019-5736.  */
पूर्णांक ensure_cloned_द्विआधारी ();
__attribute__ ((constructor)) अटल व्योम
libcrun_rexec (व्योम)
{
  अगर (ensure_cloned_द्विआधारी () < 0)
    {
      ख_लिखो (मानक_त्रुटि, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (निकास_त्रुटि);
    }
}

पूर्णांक
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  वापस libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

पूर्णांक
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  पूर्णांक ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
अटल पूर्णांक
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, पूर्णांक संकेत, libcrun_error_t *err)
{
  पूर्णांक ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  अगर (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      /* The pid is not valid anymore, return an error.  */
      अगर (ret == 0)
        {
          त्रुटि_सं = ESRCH;
          वापस crun_make_error (err, त्रुटि_सं, "kill container");
        }
    }

  ret = समाप्त (status->pid, संकेत);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "kill container");
  वापस 0;
}

पूर्णांक
libcrun_kill_linux (libcrun_container_status_t *status, पूर्णांक संकेत, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_close पूर्णांक pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  अगर (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      अगर (त्रुटि_सं == ENOSYS)
        वापस libcrun_kill_linux_no_pidfd (status, true, संकेत, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      अगर (त्रुटि_सं == ESRCH || त्रुटि_सं == ENOENT || त्रुटि_सं == EINVAL)
        वापस crun_make_error (err, ESRCH, "process not running");
      वापस crun_make_error (err, त्रुटि_सं, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* The pid is not valid anymore, return an error.  */
  अगर (ret == 0)
    {
      त्रुटि_सं = ESRCH;
      वापस crun_make_error (err, त्रुटि_सं, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, संकेत, शून्य, 0);
  अगर (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      अगर (त्रुटि_सं == ENOSYS)
        वापस libcrun_kill_linux_no_pidfd (status, false, संकेत, err);
      वापस crun_make_error (err, त्रुटि_सं, "send signal to pidfd");
    }

  वापस 0;
}

अटल स्थिर अक्षर *
libcrun_get_intelrdt_name (स्थिर अक्षर *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  अगर (def == शून्य || def->linux == शून्य || def->linux->intel_rdt == शून्य || def->linux->intel_rdt->clos_id == शून्य)
    {
      अगर (explicit)
        *explicit = false;
      वापस ctr_name;
    }

  अगर (explicit)
    *explicit = true;
  वापस def->linux->intel_rdt->clos_id;
}

पूर्णांक
libcrun_apply_intelrdt (स्थिर अक्षर *ctr_name, libcrun_container_t *container, pid_t pid, पूर्णांक actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = शून्य;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  स्थिर अक्षर *name;
  पूर्णांक ret;

  अगर (container)
    def = container->container_def;

  अगर (def == शून्य || def->linux == शून्य || def->linux->intel_rdt == शून्य)
    वापस 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  अगर (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      अगर (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          अगर (UNLIKELY (ret < 0))
            जाओ fail;

          has_monitoring = true;
        }
    }

  अगर (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      अगर (UNLIKELY (ret < 0))
        जाओ fail;
    }

  अगर (actions & LIBCRUN_INTELRDT_MOVE)
    {
      स्थिर अक्षर *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : शून्य;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      अगर (UNLIKELY (ret < 0))
        जाओ fail;
    }

  वापस 0;

fail:
  अगर (has_monitoring)
    {
      libcrun_error_t tmp_err = शून्य;
      पूर्णांक tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      अगर (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  अगर (created)
    {
      libcrun_error_t tmp_err = शून्य;
      पूर्णांक tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      अगर (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  वापस ret;
}

पूर्णांक
libcrun_destroy_intelrdt (स्थिर अक्षर *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  स्थिर अक्षर *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  अगर (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      पूर्णांक ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      अगर (ret < 0)
        वापस ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  अगर (explicit)
    वापस 0;

  वापस resctl_destroy (clos_id, err);
}

पूर्णांक
libcrun_update_intel_rdt (स्थिर अक्षर *ctr_name, libcrun_container_t *container, स्थिर अक्षर *l3_cache_schema, स्थिर अक्षर *mem_bw_schema, अक्षर *स्थिर *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = शून्य;
  स्थिर अक्षर *name;

  अगर (container)
    def = container->container_def;

  अगर (def == शून्य || def->linux == शून्य || def->linux->intel_rdt == शून्य)
    वापस 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, शून्य);

  वापस resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
पूर्णांक
libcrun_safe_chdir (स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *बफर = शून्य;
  पूर्णांक ret;

  ret = स_बदलो (path);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "chdir to `%s`", path);

  बफर = xmalloc (PATH_MAX);
  ret = syscall_getcwd (बफर, PATH_MAX);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  अगर (ret == 0 || बफर[0] != '/')
    {
      अगर (स_बदलो ("/") < 0)
        {
          /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
        }
      त्रुटि_सं = ENOENT;

      /*
        The kernel prepends the string "(unreachable)" to the path
        when it is not reachable from the current mount namespace.
        Use it to give a better error message.
      */
#घोषणा UNREACHABLE "(unreachable)"
#घोषणा UNREACHABLE_LEN ((पूर्णांक) माप (UNREACHABLE) - 1)

      अगर ((ret >= UNREACHABLE_LEN) && (स_भेद (बफर, UNREACHABLE, UNREACHABLE_LEN) == 0))
        वापस crun_make_error (err, त्रुटि_सं, "the working directory is not accessible from the current namespace");

      वापस crun_make_error (err, त्रुटि_सं, "the current working directory is not an absolute path");
    }
  वापस 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
अटल पूर्णांक
run_callback_in_namespace (पूर्णांक mntns_fd, पूर्णांक (*callback) (व्योम *, libcrun_error_t *), व्योम *arg, libcrun_error_t *err)
{
  पूर्णांक wait_status = 0;
  pid_t pid;
  पूर्णांक ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  अगर (UNLIKELY (pid < 0))
    वापस crun_make_error (err, त्रुटि_सं, "vfork");

  अगर (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      अगर (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = स_बदलो ("/");
      अगर (UNLIKELY (ret < 0))
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
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  वापस get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
अटल पूर्णांक
check_container_userns (libcrun_container_t *container, पूर्णांक userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close पूर्णांक expected_fd = -1;
  cleanup_close पूर्णांक parent_fd = -1;
  काष्ठा stat st, expected_st;
  स्थिर अक्षर *path = शून्य;
  माप_प्रकार i;
  पूर्णांक ret;

  अगर (def->linux)
    क्रम (i = 0; i < def->linux->namespaces_len; i++)
      अगर (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == शून्य;
          अवरोध;
        }

  अगर (path == शून्य)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      अगर (UNLIKELY (expected_fd < 0))
        वापस expected_fd;
    }
  अन्यथा
    {
      expected_fd = खोलो (path, O_RDONLY | O_CLOEXEC);
      अगर (UNLIKELY (expected_fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  अगर (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      अगर (UNLIKELY (parent_fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = ख_स्थिति (userns_fd, &st);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fstat user namespace");

  ret = ख_स्थिति (expected_fd, &expected_st);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fstat user namespace");

  अगर (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    वापस crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  वापस 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
अटल पूर्णांक
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, पूर्णांक *mntns_fd, पूर्णांक *owner_fd, libcrun_error_t *err)
{
  cleanup_close पूर्णांक mntns = -1;
  cleanup_close पूर्णांक owner = -1;
  पूर्णांक ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  अगर (UNLIKELY (mntns < 0))
    वापस mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (ret == 0)
    वापस crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  अगर (UNLIKELY (owner < 0))
    वापस crun_make_error (err, त्रुटि_सं, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  वापस 0;
}

अटल पूर्णांक
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, पूर्णांक (*callback) (व्योम *, libcrun_error_t *), व्योम *arg, libcrun_error_t *err)
{
  cleanup_close पूर्णांक mntns_fd = -1;
  cleanup_close पूर्णांक owner_fd = -1;
  पूर्णांक ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस run_callback_in_namespace (mntns_fd, callback, arg, err);
}

काष्ठा umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  माप_प्रकार len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
अटल पूर्णांक
umount_nofollow (स्थिर अक्षर *destination, libcrun_error_t *err)
{
  cleanup_close पूर्णांक parentfd = -1;
  cleanup_close पूर्णांक cwdfd = -1;
  अक्षर बफर[PATH_MAX];
  स्थिर अक्षर *parent, *name;
  अक्षर *last_slash;
  माप_प्रकार len;
  पूर्णांक saved_errno;
  पूर्णांक ret;

  अगर (is_empty_string (destination))
    वापस crun_make_error (err, 0, "empty destination for mount");

  len = म_माप (destination);
  अगर (UNLIKELY (len >= माप (बफर)))
    वापस crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  स_नकल (बफर, destination, len + 1);

  जबतक (len > 1 && बफर[len - 1] == '/')
    बफर[--len] = '\0';

  last_slash = म_खोजप (बफर, '/');
  अगर (last_slash == शून्य)
    {
      parent = ".";
      name = बफर;
    }
  अन्यथा अगर (last_slash == बफर)
    {
      parent = "/";
      name = बफर[1] ? बफर + 1 : ".";
    }
  अन्यथा
    {
      *last_slash = '\0';
      parent = बफर;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  अगर (UNLIKELY (parentfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open parent directory of `%s`", destination);

  cwdfd = खोलो (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  अगर (UNLIKELY (cwdfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open current directory");

  ret = fchdir (parentfd);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = त्रुटि_सं;

  अगर (UNLIKELY (fchdir (cwdfd) < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fchdir to the previous working directory");

  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, saved_errno, "umount `%s`", destination);

  वापस 0;
}

अटल पूर्णांक
do_umount_in_a_container (व्योम *arg, libcrun_error_t *err)
{
  काष्ठा umount_in_a_container_args *args = arg;
  माप_प्रकार i;
  पूर्णांक ret;

  क्रम (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

काष्ठा mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  काष्ठा libcrun_fd_map *fds;
  पूर्णांक pidfd;
  पूर्णांक owner_fd;
  माप_प्रकार len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
अटल पूर्णांक
open_destination_nofollow (स्थिर अक्षर *destination, bool dir, libcrun_error_t *err)
{
  स्थिर uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  पूर्णांक fd;
  पूर्णांक ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  अगर (LIKELY (fd >= 0))
    वापस fd;

  अगर (त्रुटि_सं != ENOENT)
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", destination);

  अगर (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  अन्यथा
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", destination);

  वापस fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
अटल पूर्णांक
create_destinations_in_a_container (व्योम *arg, libcrun_error_t *err)
{
  काष्ठा mount_in_a_container_args *args = arg;
  माप_प्रकार i;
  पूर्णांक ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  अगर (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "setresuid to `0`");
    }
  अन्यथा अगर (त्रुटि_सं != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      वापस crun_make_error (err, त्रुटि_सं, "setns to the user namespace of the container");
    }

  क्रम (i = 0; i < args->len; i++)
    {
      cleanup_close पूर्णांक dest_fd = -1;
      काष्ठा stat st;

      ret = ख_स्थिति (args->fds->fds[i], &st);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      अगर (UNLIKELY (dest_fd < 0))
        वापस dest_fd;
    }

  वापस 0;
}

अटल पूर्णांक
do_mount_in_a_container (व्योम *arg, libcrun_error_t *err)
{
  काष्ठा mount_in_a_container_args *args = arg;
  माप_प्रकार i;
  पूर्णांक ret;

  क्रम (i = 0; i < args->len; i++)
    {
      cleanup_close पूर्णांक dest_fd = -1;
      काष्ठा stat st;

      ret = ख_स्थिति (args->fds->fds[i], &st);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      अगर (UNLIKELY (dest_fd < 0))
        वापस dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "move mount to `%s`", args->mounts[i]->destination);
    }

  वापस 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
अटल पूर्णांक
send_mounts_from_container_namespaces (libcrun_container_t *container, पूर्णांक pidfd, runtime_spec_schema_defs_mount **mounts,
                                       काष्ठा libcrun_fd_map *fds, माप_प्रकार len, पूर्णांक flags, पूर्णांक owner_fd, पूर्णांक sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  माप_प्रकार i;
  पूर्णांक ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  अगर (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "unshare `CLONE_NEWNS`");
    }
  अन्यथा अगर (त्रुटि_सं != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      वापस crun_make_error (err, त्रुटि_सं, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = विभाजन ();
  अगर (UNLIKELY (pid < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fork");

  अगर (pid)
    {
      पूर्णांक wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = अस्थायी_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      अगर (ret > 0 && WIFSTOPPED (wait_status))
        {
          समाप्त (pid, SIGKILL);
          अस्थायी_FAILURE_RETRY (waitpid (pid, शून्य, 0));
          वापस crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (निकास_सफल);
    }

  ret = set_id_init (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "prctl (PR_SET_DUMPABLE)");

  क्रम (i = 0; i < len; i++)
    {
      cleanup_close पूर्णांक fd = -1;

      अगर (fds->fds[i] >= 0)
        जारी;

      fd = open_mount_of_type (container, mounts[i], err);
      अगर (UNLIKELY (fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
अटल पूर्णांक
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, पूर्णांक *flags, libcrun_error_t *err)
{
  अटल स्थिर काष्ठा
  {
    स्थिर अक्षर *name;
    पूर्णांक flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  माप_प्रकार i;

  *flags = 0;

  क्रम (i = 0; i < माप (namespaces) / माप (namespaces[0]); i++)
    {
      cleanup_close पूर्णांक self_fd = -1;
      cleanup_close पूर्णांक target_fd = -1;
      काष्ठा stat self_st, target_st;
      cleanup_free अक्षर *self_path = शून्य;
      पूर्णांक ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      अगर (UNLIKELY (self_fd < 0))
        वापस self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      अगर (UNLIKELY (target_fd < 0))
        वापस target_fd;

      ret = ख_स्थिति (self_fd, &self_st);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "fstat `%s`", namespaces[i].name);

      ret = ख_स्थिति (target_fd, &target_st);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "fstat `%s`", namespaces[i].name);

      अगर (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  वापस 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
अटल पूर्णांक
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, पूर्णांक owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, काष्ठा libcrun_fd_map *fds, माप_प्रकार len,
                                     libcrun_error_t *err)
{
  cleanup_close पूर्णांक sync_fd = -1;
  cleanup_close पूर्णांक pidfd = -1;
  पूर्णांक sync_socket_fd[2];
  bool needed = false;
  पूर्णांक flags;
  pid_t pid;
  माप_प्रकार i;
  पूर्णांक ret;

  क्रम (i = 0; i < len; i++)
    {
      अगर (fds->fds[i] >= 0)
        जारी;

      अगर (mounts[i]->type == शून्य)
        वापस crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  अगर (! needed)
    वापस 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  अगर (UNLIKELY (pidfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (ret == 0)
    वापस crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = विभाजन ();
  अगर (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, त्रुटि_सं, "fork");
      अस्थायी_FAILURE_RETRY (बंद (sync_socket_fd[1]));
      वापस ret;
    }

  अगर (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      अगर (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (निकास_सफल);
    }

  अस्थायी_FAILURE_RETRY (बंद (sync_socket_fd[1]));

  ret = 0;
  क्रम (i = 0; i < len; i++)
    {
      अगर (fds->fds[i] >= 0)
        जारी;

      ret = receive_fd_from_socket (sync_fd, err);
      अगर (UNLIKELY (ret < 0))
        {
          cleanup_free अक्षर *err_str = शून्य;
          पूर्णांक err_code = 0;

          अगर (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          अन्यथा
            ret = crun_error_wrap (err, "receive mount fd");
          अवरोध;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  अस्थायी_FAILURE_RETRY (waitpid (pid, शून्य, 0));

  वापस ret;
}

अटल अचिन्हित दीर्घ
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free अक्षर *तथ्य = शून्य;
  अचिन्हित दीर्घ extra_flags = 0;
  अचिन्हित दीर्घ flags = 0;
  माप_प्रकार j;

  अगर (mount->options == शून्य)
    वापस get_default_flags (container, mount->destination, &तथ्य);

  क्रम (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &तथ्य, rec_clear, rec_set);

  वापस flags;
}

पूर्णांक
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, माप_प्रकार len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  काष्ठा mount_in_a_container_args args;
  cleanup_close_map काष्ठा libcrun_fd_map *fds = शून्य;
  cleanup_close पूर्णांक pidfd = -1;
  cleanup_close पूर्णांक mntns_fd = -1;
  cleanup_close पूर्णांक owner_fd = -1;
  pid_t pid = status->pid;
  माप_प्रकार i;
  पूर्णांक ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  fds = make_libcrun_fd_map (len);

  क्रम (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), शून्य, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      अगर (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          अचिन्हित दीर्घ propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          अगर (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          अगर (UNLIKELY (fds->fds[i] < 0))
            वापस fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  क्रम (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      अचिन्हित दीर्घ flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  अगर (UNLIKELY (ret > 0))
    वापस *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

पूर्णांक
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, माप_प्रकार len, libcrun_error_t *err)
{
  काष्ठा umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  वापस run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

काष्ठा restrict_mounts_in_a_container_args
{
  काष्ठा libcrun_mount_flags_s *mounts;
  माप_प्रकार len;
  पूर्णांक rootfd;
};

अटल पूर्णांक
do_restrict_mounts_in_a_container (व्योम *arg, libcrun_error_t *err)
{
  काष्ठा restrict_mounts_in_a_container_args *args = arg;
  माप_प्रकार i;
  पूर्णांक ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  अगर (LIKELY (ret == 0))
    ret = chroot (".");
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "chroot to the container root");

  क्रम (i = 0; i < args->len; i++)
    {
      अचिन्हित दीर्घ flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      स्थिर अक्षर *target = consume_slashes (args->mounts[i].target);
      cleanup_close पूर्णांक fd = -1;
      काष्ठा statfs sfs;
      अचिन्हित दीर्घ current_flags;

      अगर (flags == 0)
        जारी;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      अगर (UNLIKELY (fd < 0))
        {
          अगर (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              जारी;
            }
          वापस fd;
        }

      ret = fstatfs (fd, &sfs);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      अगर ((current_flags & flags) == flags)
        जारी;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      अगर (LIKELY (ret == 0))
        जारी;

      अगर (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        वापस ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (शून्य, target, शून्य, MS_REMOUNT | MS_BIND | flags | current_flags, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "remount `/%s`", target);
    }

  वापस 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
पूर्णांक
libcrun_restrict_mounts_in_container (pid_t pid, काष्ठा libcrun_mount_flags_s *mounts, माप_प्रकार len, libcrun_error_t *err)
{
  काष्ठा restrict_mounts_in_a_container_args args;
  cleanup_close पूर्णांक mntns_fd = -1;
  cleanup_close पूर्णांक rootfd = -1;
  अक्षर path[64];
  पूर्णांक ret;

  snprintf (path, माप (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = खोलो (path, O_RDONLY | O_CLOEXEC);
  अगर (UNLIKELY (mntns_fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", path);

  snprintf (path, माप (path), "/proc/%d/root", pid);
  rootfd = खोलो (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  अगर (UNLIKELY (rootfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  अगर (UNLIKELY (ret > 0))
    वापस *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  वापस ret;
}

पूर्णांक
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close पूर्णांक netns_fd = -1;
  माप_प्रकार i;
  पूर्णांक ret;

  अगर (def == शून्य || def->linux == शून्य || def->linux->net_devices == शून्य)
    वापस 0;

  cleanup_free अक्षर *ns_path = शून्य;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  अगर (UNLIKELY (netns_fd < 0))
    वापस netns_fd;

  क्रम (i = 0; i < def->linux->net_devices->len; i++)
    {
      स्थिर अक्षर *new_name = def->linux->net_devices->मूल्यावली[i]->name ?: def->linux->net_devices->कुंजियाँ[i];

      ret = move_network_device (def->linux->net_devices->कुंजियाँ[i], new_name, netns_fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}
