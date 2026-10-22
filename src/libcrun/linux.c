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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "linux.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "status.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ <sched.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/mount.h>
#నిర్వచితమైతే HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#అయితే_ముగింపు
#ఒకవేళ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#అయితే_ముగింపు

#అన్తర్భావ <sys/prctl.h>
#నిర్వచితమైతే HAVE_CAP
#  include <sys/capability.h>
#అయితే_ముగింపు
#అన్తర్భావ <sys/time.h>
#అన్తర్భావ <sys/resource.h>
#అన్తర్భావ <sys/sysmacros.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <grp.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ "terminal.h"
#అన్తర్భావ "cgroup.h"
#అన్తర్భావ "cgroup-utils.h"
#అన్తర్భావ "status.h"
#అన్తర్భావ "criu.h"
#అన్తర్భావ "scheduler.h"
#అన్తర్భావ "intelrdt.h"
#అన్తర్భావ "io_priority.h"
#అన్తర్భావ "net_device.h"

#అన్తర్భావ <sys/socket.h>
#అన్తర్భావ <libgen.h>
#అన్తర్భావ <sys/wait.h>
#అన్తర్భావ <sys/vfs.h>
#అన్తర్భావ <sys/statvfs.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ <sys/personality.h>
#అన్తర్భావ <net/if.h>
#అన్తర్భావ <sys/xattr.h>
#అన్తర్భావ <linux/netlink.h>
#అన్తర్భావ <linux/rtnetlink.h>
#అన్తర్భావ <sched.h>
#అన్తర్భావ <linux/sched.h>
#అన్తర్భావ <linux/magic.h>
#అన్తర్భావ <linux/nsfs.h>

#అన్తర్భావ <json-c/json.h>
#అన్తర్భావ <ocispec/json_common.h>

#అన్తర్భావ "mount_flags.h"
#అన్తర్భావ "syscalls.h"

#నిర్వచితం_కాకపోతే RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే OPEN_వృక్షం_NAMESPACE
#  define OPEN_వృక్షం_NAMESPACE 2
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOVE_MOUNT_F_EMPTY_PATH
#  define MOVE_MOUNT_F_EMPTY_PATH 0x00000004
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOVE_MOUNT_T_EMPTY_PATH
#  define MOVE_MOUNT_T_EMPTY_PATH 0x00000040
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#అయితే_ముగింపు

#నిర్వచించు ALL_PROPAGATIONS_NO_REC (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE)
#నిర్వచించు ALL_PROPAGATIONS (MS_REC | ALL_PROPAGATIONS_NO_REC)

/* The per-mount flags a remount clears unless they are explicitly set.  */
#నిర్వచించు REMOUNT_CLEAR_FLAGS (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW)

#నిర్వచితం_కాకపోతే ST_NOSYMFOLLOW
#  define ST_NOSYMFOLLOW 0x2000
#అయితే_ముగింపు

#నిర్వచించు MOUNT_ATIME_FLAGS (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME)

/* Convert the ST_ flags reported by statfs(2) to the MS_ flags of mount(2).  */
స్థిర గుర్తులేని దీర్ఘ
mount_flags_from_statfs (గుర్తులేని దీర్ఘ f_flags)
{
  గుర్తులేని దీర్ఘ flags = 0;

  ఒకవేళ (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  ఒకవేళ (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  ఒకవేళ (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  ఒకవేళ (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  ఒకవేళ (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  ఒకవేళ (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  ఒకవేళ (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  ఒకవేళ (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  ఒకవేళ (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  తిరిగి flags;
}

నిర్మాణం remount_s
{
  నిర్మాణం remount_s *తదుపరి;
  అక్షరం *లక్ష్యం;
  పూర్ణాంకం targetfd;
  గుర్తులేని దీర్ఘ flags;
  అక్షరం *దత్తాంశం;
};

నిర్మాణం వ్యక్తిగత_దత్తాంశం_s
{
  నిర్మాణం remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  పూర్ణాంకం unshare_flags;
  పూర్ణాంకం unshare_cgroupns;

  అక్షరం *host_notify_socket_మార్గం;
  అక్షరం *container_notify_socket_మార్గం;
  అక్షరం *unified_cgroup_మార్గం;

  bool mount_dev_from_host;
  గుర్తులేని దీర్ఘ rootfs_ప్రసరణ;
  bool deny_setgroups;

  పూర్ణాంకం procfd;

  స్థిరరాశి అక్షరం *rootfs;
  పూర్ణాంకం rootfsfd;

  పూర్ణాంకం notify_socket_వృక్షం_fd;
  పూర్ణాంకం old_మూలం_fd;

  నిర్మాణం libcrun_fd_map *mount_fds;
  నిర్మాణం libcrun_fd_map *dev_fds;
  నిర్మాణం libcrun_fd_map *needed_devs_fds;

  అక్షరం **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  అక్షరం *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  పూర్ణాంకం maskdir_fd;
  అక్షరం *maskdir_proc_మార్గం;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_ఇరుసు;
  bool no_ఇరుసు;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_మూలం_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  అక్షరం *host_rootfs;
};

నిర్మాణం linux_namespace_s
{
  స్థిరరాశి అక్షరం *name;
  స్థిరరాశి అక్షరం *ns_file;
  పూర్ణాంకం విలువ;
};

స్థిర శూన్య
cleanup_వ్యక్తిగత_దత్తాంశం (శూన్య *వ్యక్తిగత_దత్తాంశం)
{
  నిర్మాణం వ్యక్తిగత_దత్తాంశం_s *p = వ్యక్తిగత_దత్తాంశం;

  ఒకవేళ (p->rootfsfd >= 0)
    తాత్కాలిక_FAILURE_RETRY (close (p->rootfsfd));
  ఒకవేళ (p->procfd >= 0)
    తాత్కాలిక_FAILURE_RETRY (close (p->procfd));
  ఒకవేళ (p->maskdir_fd >= 0)
    తాత్కాలిక_FAILURE_RETRY (close (p->maskdir_fd));
  ఒకవేళ (p->old_మూలం_fd >= 0)
    తాత్కాలిక_FAILURE_RETRY (close (p->old_మూలం_fd));
  ఒకవేళ (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  ఒకవేళ (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  ఒకవేళ (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  ఒకవేళ (p->copy_symlink_targets)
    {
      size_t i;
      కొరకు (i = 0; i < p->n_copy_symlink_targets; i++)
        విడుదల (p->copy_symlink_targets[i]);
      విడుదల (p->copy_symlink_targets);
    }
  విడుదల (p->unified_cgroup_మార్గం);
  విడుదల (p->host_notify_socket_మార్గం);
  విడుదల (p->container_notify_socket_మార్గం);
  విడుదల (p->external_descriptors);
  విడుదల (p->maskdir_proc_మార్గం);
  విడుదల (p->host_rootfs);
  విడుదల (p);
}

స్థిర నిర్మాణం వ్యక్తిగత_దత్తాంశం_s *
get_వ్యక్తిగత_దత్తాంశం (నిర్మాణం libcrun_container_s *container)
{
  ఒకవేళ (container->వ్యక్తిగత_దత్తాంశం == ఖాళీ)
    {
      నిర్మాణం వ్యక్తిగత_దత్తాంశం_s *p = xmalloc0 (పరిమాణం (*p));
      container->వ్యక్తిగత_దత్తాంశం = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_వృక్షం_fd = -1;
      p->old_మూలం_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_వ్యక్తిగత_దత్తాంశం = cleanup_వ్యక్తిగత_దత్తాంశం;
    }
  తిరిగి container->వ్యక్తిగత_దత్తాంశం;
}

#నిర్వచితం_కాకపోతే CLONE_NEWTIME
#  define CLONE_NEWTIME 0x00000080
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CLONE_NEWCGROUP
#  define CLONE_NEWCGROUP 0x02000000
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే AT_RECURSIVE
#  define AT_RECURSIVE 0x8000
#అయితే_ముగింపు

స్థిర నిర్మాణం linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { ఖాళీ, ఖాళీ, 0 } };

స్థిర పూర్ణాంకం
get_and_reset (పూర్ణాంకం *old)
{
  పూర్ణాంకం tmp = *old;
  *old = -1;
  తిరిగి tmp;
}

పూర్ణాంకం
libcrun_కనుగొను_namespace (స్థిరరాశి అక్షరం *name)
{
  నిర్మాణం linux_namespace_s *it;
  కొరకు (it = namespaces; it->name; it++)
    ఒకవేళ (strcmp (it->name, name) == 0)
      తిరిగి it->విలువ;
  తిరిగి -1;
}

స్థిర పూర్ణాంకం
syscall_keyctl_join (స్థిరరాశి అక్షరం *name)
{
#నిర్వచించు KEYCTL_JOIN_SESSION_KEYRING 0x1
  తిరిగి (పూర్ణాంకం) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

స్థిర పూర్ణాంకం
syscall_pidfd_open (pid_t pid, గుర్తులేని పూర్ణాంకం flags)
{
#ఒకవేళ defined __NR_pidfd_open
  తిరిగి (పూర్ణాంకం) syscall (__NR_pidfd_open, pid, flags);
#లేకపోతే
  (శూన్య) pid;
  (శూన్య) flags;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

స్థిర పూర్ణాంకం
syscall_pidfd_send_signal (పూర్ణాంకం pidfd, పూర్ణాంకం sig, siginfo_t *info, గుర్తులేని పూర్ణాంకం flags)
{
#ఒకవేళ defined __NR_pidfd_send_signal
  తిరిగి (పూర్ణాంకం) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#లేకపోతే
  (శూన్య) pidfd;
  (శూన్య) sig;
  (శూన్య) info;
  (శూన్య) flags;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
స్థిర uint64_t
ms_flags_గమ్యం_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  ఒకవేళ (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  ఒకవేళ (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  ఒకవేళ (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  ఒకవేళ (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  ఒకవేళ (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  ఒకవేళ (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  ఒకవేళ (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  ఒకవేళ (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  తిరిగి attr;
}

స్థిర పూర్ణాంకం
do_mount_setattr (bool recursive, స్థిరరాశి అక్షరం *లక్ష్యం, పూర్ణాంకం targetfd, uint64_t clear, uint64_t సమితి, libcrun_error_t *err)
{
  నిర్మాణం mount_attr_s attr = {
    0,
  };
  పూర్ణాంకం ret;

  సమితి &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.ప్రసరణ = సమితి & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_గమ్యం_mount_attr (సమితి & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_గమ్యం_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  ఒకవేళ (సమితి & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      ఒకవేళ (సమితి & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      లేకపోతే ఒకవేళ (సమితి & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      లేకపోతే
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      ఒకవేళ (సమితి & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  లేకపోతే ఒకవేళ (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_ఖాళీలు_మార్గం, &attr);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "mount_setattr `/%s`", లక్ష్యం);

  తిరిగి 0;
}

పూర్ణాంకం
get_bind_mount (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *src, bool recursive, bool rdonly, bool nofollow, గుర్తులేని దీర్ఘ ప్రసరణ, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం open_వృక్షం_fd = -1;
  నిర్మాణం mount_attr_s attr = {
    0,
  };
  పూర్ణాంకం recursive_flag = (recursive ? AT_RECURSIVE : 0);
  పూర్ణాంకం ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  ఒకవేళ (UNLIKELY (src == ఖాళీ))
    తిరిగి crun_make_error (err, EINVAL, "mount source is not specified");

  ఒకవేళ (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.ప్రసరణ = ప్రసరణ;

  ఒకవేళ (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_వృక్షం_fd = syscall_open_వృక్షం (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_ఖాళీలు_మార్గం : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  ఒకవేళ (UNLIKELY (open_వృక్షం_fd < 0))
    తిరిగి crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_వృక్షం_fd, "", AT_ఖాళీలు_మార్గం | recursive_flag, &attr);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "mount_setattr `%s`", src);

  తిరిగి get_and_reset (&open_వృక్షం_fd);
}

పూర్ణాంకం
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, అక్షరం *option, అక్షరం **బహిర్గతం, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free అక్షరం *mappings = ఖాళీ;
  స్థిరరాశి అక్షరం *it;
  పూర్ణాంకం ret;

  mappings = xmalloc (allocated);

  కొరకు (it = option; *it;)
    {
      bool relative = false;
      దీర్ఘ విలువ[3];
      size_t i;

      ఒకవేళ (*it == '\0')
        ఆపు;

      ఒకవేళ (*it == '#')
        it++;

      ఒకవేళ (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      కొరకు (i = 0; i < 3; i++)
        {
          అక్షరం *endptr = ఖాళీ;

          ఒకవేళ (i > 0 && *it == '-')
            it++;

          ఒకవేళ (*it == '\0')
            తిరిగి crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          విలువ[i] = strtol (it, &endptr, 10);
          ఒకవేళ (errno || endptr == it)
            తిరిగి crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      ఒకవేళ (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          ఒకవేళ (def == ఖాళీ
              || def->linux == ఖాళీ
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            తిరిగి crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          కొరకు (i = 0; i < mappings_len; i++)
            ఒకవేళ (విలువ[1] >= (int64_t) mappings[i]->container_id
                && విలువ[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->కొలత)
              ఆపు;

          ఒకవేళ (i == mappings_len)
            తిరిగి crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          విలువ[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      ఒకవేళ (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", విలువ[0], విలువ[1], విలువ[2]);
      ఒకవేళ (UNLIKELY (ret >= (పూర్ణాంకం) (allocated - written)))
        తిరిగి crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *బహిర్గతం = mappings;
  mappings = ఖాళీ;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
format_mount_mappings (అక్షరం **బహిర్గతం,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  స్థిరరాశి size_t max_len_mapping = 64;
  cleanup_free అక్షరం *ret = ఖాళీ;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  కొరకు (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->కొలత);
      ఒకవేళ (UNLIKELY (len >= max_len_mapping))
        తిరిగి crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *బహిర్గతం = ret;
  ret = ఖాళీ;
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
format_mount_mapping (అక్షరం **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t కొలత, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .కొలత = కొలత,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    ఖాళీ,
  };

  తిరిగి format_mount_mappings (ret, mappings, 1, written, err);
}

స్థిర bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  ఒకవేళ (def->linux == ఖాళీ)
    తిరిగి mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  ఒకవేళ (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    తిరిగి false;

  ఒకవేళ (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    తిరిగి false;

  కొరకు (s = 0; s < mnt->uid_mappings_len; s++)
    {
      ఒకవేళ (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        తిరిగి false;
      ఒకవేళ (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        తిరిగి false;
      ఒకవేళ (mnt->uid_mappings[s]->కొలత != def->linux->uid_mappings[s]->కొలత)
        తిరిగి false;
    }

  కొరకు (s = 0; s < mnt->gid_mappings_len; s++)
    {
      ఒకవేళ (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        తిరిగి false;
      ఒకవేళ (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        తిరిగి false;
      ఒకవేళ (mnt->gid_mappings[s]->కొలత != def->linux->gid_mappings[s]->కొలత)
        తిరిగి false;
    }

  తిరిగి true;
}

స్థిర pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        స్థిరరాశి అక్షరం *options, pid_t *pid_బహిర్గతం,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != ఖాళీ;

  ఒకవేళ (! need_new_userns)
    తిరిగి 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, ఖాళీ);
  ఒకవేళ (UNLIKELY (pid < 0))
    తిరిగి crun_make_error (err, errno, "clone");

  ఒకవేళ (pid == 0)
    {
      prctl (PR_సమితి_PDEATHSIG, SIGKILL);
      అయితే (1)
        pause ();
      సురక్షిత_exit (EXIT_SUCCESS);
    }

  ఒకవేళ (mnt->uid_mappings_len)
    {
      cleanup_free అక్షరం *uid_map = ఖాళీ;
      cleanup_free అక్షరం *gid_map = ఖాళీ;
      size_t written = 0;
      పూర్ణాంకం ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      cleanup_close పూర్ణాంకం fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      ఒకవేళ (UNLIKELY (fd < 0))
        తిరిగి fd;

      ret = సురక్షిత_write (fd, "uid_map", uid_map, written, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      ఒకవేళ (UNLIKELY (fd < 0))
        తిరిగి fd;

      ret = సురక్షిత_write (fd, "gid_map", gid_map, written, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      cleanup_free అక్షరం *dup_options = ఖాళీ;
      అక్షరం *option, *saveptr = ఖాళీ;

      ఒకవేళ (! options)
        తిరిగి crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      కొరకు (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (ఖాళీ, ";", &saveptr))
        {
          cleanup_free అక్షరం *mappings = ఖాళీ;
          cleanup_close పూర్ణాంకం map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          పూర్ణాంకం ret;

          ఒకవేళ (has_ఉపసర్గ (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              ఒకవేళ (UNLIKELY (map_fd < 0))
                తిరిగి map_fd;
            }
          లేకపోతే ఒకవేళ (has_ఉపసర్గ (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              ఒకవేళ (UNLIKELY (map_fd < 0))
                తిరిగి map_fd;
            }
          లేకపోతే
            తిరిగి crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          ret = సురక్షిత_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }

  *pid_బహిర్గతం = pid;
  pid = -1;
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_create_keyring (libcrun_container_t *container, స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *label, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం labelfd = -1;
  bool label_సమితి = false;
  పూర్ణాంకం ret;

  ఒకవేళ (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      ఒకవేళ (UNLIKELY (labelfd < 0))
        {
          ఒకవేళ (crun_error_get_errno (err) != ENOENT)
            తిరిగి labelfd;

          crun_error_release (err);
        }

      ఒకవేళ (labelfd >= 0)
        {
          ret = లిఖ (labelfd, label, అక్షరశ్రేణి_పొడవు (label));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_సమితి = true;
        }
    }

  ret = syscall_keyctl_join (name);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          వెళ్ళు బహిర్గతం;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      వెళ్ళు బహిర్గతం;
    }

బహిర్గతం:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  ఒకవేళ (label_సమితి && లిఖ (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  తిరిగి ret;
}

స్థిర శూన్య
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  ఒకవేళ (def->process && def->process->user)
    {
      ఒకవేళ (def->process->user->uid)
        *uid = def->process->user->uid;
      ఒకవేళ (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

స్థిర గుర్తులేని దీర్ఘ
get_mount_flags (స్థిరరాశి అక్షరం *name, పూర్ణాంకం current_flags, పూర్ణాంకం *దొరికింది, గుర్తులేని దీర్ఘ *extra_flags, uint64_t *rec_clear, uint64_t *rec_సమితి)
{
  స్థిరరాశి నిర్మాణం ప్రసరణ_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  ఒకవేళ (దొరికింది)
    *దొరికింది = prop ? 1 : 0;

  ఒకవేళ (prop == ఖాళీ)
    తిరిగి 0;

  ఒకవేళ (prop->extra_flags & OPTION_RECURSIVE)
    {
      ఒకవేళ (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      ఒకవేళ (rec_సమితి && ! prop->clear)
        *rec_సమితి |= prop->flags;
    }

  ఒకవేళ (extra_flags)
    *extra_flags |= prop->extra_flags;

  ఒకవేళ (prop->clear)
    తిరిగి current_flags & ~prop->flags;

  తిరిగి current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
స్థిర bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  కొరకు (i = 0; i < mount->options_len; i++)
    {
      స్థిరరాశి నిర్మాణం ప్రసరణ_flags_s *prop;

      ఒకవేళ (mount->options[i] == ఖాళీ)
        కొనసాగు;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ఒకవేళ (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        తిరిగి true;
    }

  తిరిగి false;
}

స్థిర గుర్తులేని దీర్ఘ
get_mount_flags_or_option (స్థిరరాశి అక్షరం *name, పూర్ణాంకం current_flags, గుర్తులేని దీర్ఘ *extra_flags, అక్షరం **option, uint64_t *rec_clear, uint64_t *rec_సమితి)
{
  పూర్ణాంకం దొరికింది;
  __attribute__ ((వాడని)) cleanup_free అక్షరం *prev = ఖాళీ;
  గుర్తులేని దీర్ఘ flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  ఒకవేళ (name == ఖాళీ)
    తిరిగి 0;

  flags = get_mount_flags (name, current_flags, &దొరికింది, extra_flags, rec_clear, rec_సమితి);
  ఒకవేళ (దొరికింది)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      స్థిరరాశి అక్షరం *దత్తాంశం_flag = ఖాళీ;
      ఒకవేళ ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        దత్తాంశం_flag = "sync";
      లేకపోతే ఒకవేళ ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        దత్తాంశం_flag = "dirsync";

      ఒకవేళ (దత్తాంశం_flag)
        {
          prev = *option;
          ఒకవేళ (*option && **option)
            xasprintf (option, "%s,%s", *option, దత్తాంశం_flag);
          లేకపోతే
            *option = xstrdup (దత్తాంశం_flag);
        }
      తిరిగి flags;
    }

  prev = *option;
  ఒకవేళ (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  లేకపోతే
    *option = xstrdup (name);

  తిరిగి 0;
}

పూర్ణాంకం
ఇరుసు_మూలం (స్థిరరాశి అక్షరం *new_మూలం, స్థిరరాశి అక్షరం *put_old)
{
  తిరిగి syscall (__NR_pivot_root, new_మూలం, put_old);
}

స్థిర శూన్య
free_remount (నిర్మాణం remount_s *r)
{
  ఒకవేళ (r == ఖాళీ)
    తిరిగి;
  ఒకవేళ (r->targetfd >= 0)
    close (r->targetfd);
  విడుదల (r->దత్తాంశం);
  విడుదల (r->లక్ష్యం);
  విడుదల (r);
}

స్థిర నిర్మాణం remount_s *
make_remount (పూర్ణాంకం targetfd, స్థిరరాశి అక్షరం *లక్ష్యం, గుర్తులేని దీర్ఘ flags, స్థిరరాశి అక్షరం *దత్తాంశం, నిర్మాణం remount_s *తదుపరి)
{
  నిర్మాణం remount_s *ret = xmalloc (పరిమాణం (*ret));
  ret->లక్ష్యం = xstrdup (లక్ష్యం);
  ret->flags = flags;
  ret->దత్తాంశం = దత్తాంశం ? xstrdup (దత్తాంశం) : ఖాళీ;
  ret->తదుపరి = తదుపరి;
  ret->targetfd = targetfd;
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
do_remount (పూర్ణాంకం targetfd, స్థిరరాశి అక్షరం *లక్ష్యం, గుర్తులేని దీర్ఘ flags, స్థిరరాశి అక్షరం *దత్తాంశం, libcrun_error_t *err)
{
  proc_fd_మార్గం_t లక్ష్యం_బఫరు;
  పూర్ణాంకం ret;
  స్థిరరాశి అక్షరం *real_లక్ష్యం = లక్ష్యం;

  ఒకవేళ (targetfd >= 0)
    {
      గుర్తులేని దీర్ఘ సమితి_flags = flags & ~(MS_REMOUNT | MS_BIND);
      గుర్తులేని దీర్ఘ clear_flags = REMOUNT_CLEAR_FLAGS & ~సమితి_flags;
      ret = do_mount_setattr (false, లక్ష్యం, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      ఒకవేళ (LIKELY (ret == 0))
        తిరిగి 0;
      crun_error_release (err);

      get_proc_self_fd_మార్గం (లక్ష్యం_బఫరు, targetfd);
      real_లక్ష్యం = లక్ష్యం_బఫరు;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  ఒకవేళ (flags & (MS_REMOUNT | MS_RDONLY))
    దత్తాంశం = ఖాళీ;

  ret = mount (ఖాళీ, real_లక్ష్యం, ఖాళీ, flags, దత్తాంశం);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      గుర్తులేని దీర్ఘ current_flags, remount_flags;
      నిర్మాణం statfs sfs;

      ret = statfs (real_లక్ష్యం, &sfs);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "statfs `%s`", real_లక్ష్యం);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      ఒకవేళ (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      ఒకవేళ ((flags | remount_flags) != flags)
        {
          ret = mount (ఖాళీ, real_లక్ష్యం, ఖాళీ, flags | remount_flags, దత్తాంశం);
          ఒకవేళ (LIKELY (ret == 0))
            తిరిగి 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          ఒకవేళ (current_flags & MS_RDONLY)
            ret = mount (ఖాళీ, real_లక్ష్యం, ఖాళీ, flags | remount_flags | MS_RDONLY, దత్తాంశం);
        }
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "remount `%s`", లక్ష్యం);
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  పూర్ణాంకం ret = 0;
  నిర్మాణం remount_s *r = get_వ్యక్తిగత_దత్తాంశం (container)->remounts;
  అయితే (r)
    {
      నిర్మాణం remount_s *తదుపరి = r->తదుపరి;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      ఒకవేళ (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          గుర్తులేని దీర్ఘ సమితి_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          గుర్తులేని దీర్ఘ clear_flags = REMOUNT_CLEAR_FLAGS & ~సమితి_flags;

          ret = do_mount_setattr (false, r->లక్ష్యం, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          ఒకవేళ (LIKELY (ret == 0))
            {
              free_remount (r);
              r = తదుపరి;
              కొనసాగు;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->లక్ష్యం, r->flags, r->దత్తాంశం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు cleanup;

      free_remount (r);
      r = తదుపరి;
    }

cleanup:
  అయితే (r)
    {
      నిర్మాణం remount_s *తదుపరి = r->తదుపరి;
      free_remount (r);
      r = తదుపరి;
    }

  get_వ్యక్తిగత_దత్తాంశం (container)->remounts = ఖాళీ;
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
open_mount_లక్ష్యం (libcrun_container_t *container, స్థిరరాశి అక్షరం *లక్ష్యం_rel, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *rootfs = get_వ్యక్తిగత_దత్తాంశం (container)->rootfs;
  పూర్ణాంకం rootfsfd = get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd;

  ఒకవేళ (rootfsfd < 0)
    తిరిగి crun_make_error (err, 0, "invalid rootfs state");

  తిరిగి సురక్షిత_openat (rootfsfd, rootfs, లక్ష్యం_rel, O_మార్గం | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
స్థిర పూర్ణాంకం
fsopen_mount (స్థిరరాశి అక్షరం *type, స్థిరరాశి అక్షరం *ఆధారం_name, స్థిరరాశి అక్షరం *labeltype, స్థిరరాశి అక్షరం *label, స్థిరరాశి అక్షరం *దత్తాంశం)
{
#నిర్వచితమైతే HAVE_NEW_MOUNT_API
  cleanup_close పూర్ణాంకం fsfd = -1;
  పూర్ణాంకం ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ఒకవేళ (UNLIKELY (fsfd < 0))
    తిరిగి fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (శూన్య) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", ఆధారం_name ? ఆధారం_name : type, 0);

  ఒకవేళ (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (దత్తాంశం && దత్తాంశం[0])
    {
      cleanup_free అక్షరం *దత్తాంశం_copy = xstrdup (దత్తాంశం);
      అక్షరం *saveptr = ఖాళీ;
      అక్షరం *token;

      కొరకు (token = strtok_r (దత్తాంశం_copy, ",", &saveptr); token; token = strtok_r (ఖాళీ, ",", &saveptr))
        {
          అక్షరం *eq = strchr (token, '=');
          ఒకవేళ (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          లేకపోతే
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, ఖాళీ, 0);

          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ఖాళీ, ఖాళీ, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#లేకపోతే
  (శూన్య) type;
  (శూన్య) ఆధారం_name;
  (శూన్య) దత్తాంశం;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

స్థిర పూర్ణాంకం
get_procfd (నిర్మాణం వ్యక్తిగత_దత్తాంశం_s *దత్తాంశం, libcrun_error_t *err)
{
  పూర్ణాంకం fd;

  ఒకవేళ (దత్తాంశం->procfd >= 0)
    తిరిగి దత్తాంశం->procfd;

  fd = fsopen_mount ("proc", ఖాళీ, ఖాళీ, ఖాళీ, ఖాళీ);
  ఒకవేళ (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "open `/proc`");

  దత్తాంశం->procfd = fd;
  తిరిగి fd;
}

స్థిర పూర్ణాంకం
get_old_మూలం_fd (నిర్మాణం వ్యక్తిగత_దత్తాంశం_s *దత్తాంశం)
{
  ఒకవేళ (దత్తాంశం->old_మూలం_fd >= 0)
    తిరిగి దత్తాంశం->old_మూలం_fd;

  దత్తాంశం->old_మూలం_fd = open ("/", O_మార్గం | O_CLOEXEC);
  తిరిగి దత్తాంశం->old_మూలం_fd;
}

స్థిర పూర్ణాంకం
fs_move_mount_గమ్యం (పూర్ణాంకం fd, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *name)
{
#నిర్వచితమైతే HAVE_NEW_MOUNT_API
  ఒకవేళ (name)
    తిరిగి syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  తిరిగి syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#లేకపోతే
  (శూన్య) fd;
  (శూన్య) dirfd;
  (శూన్య) name;
  (శూన్య) syscall_move_mount;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

గణన
{
  /* Do not apply any label to the mount.  */
  LABEL_NONE = 0,
  /* Apply the label as a mount option.  */
  LABEL_MOUNT = 1,
  /* Apply the label using setxattr.  */
  LABEL_XATTR = 2,

  LABEL_ముసుగు = 0x0f,

  /* Do not defer a readonly remount — apply MS_RDONLY directly.  */
  MOUNT_NO_DEFERRED_REMOUNT = 0x10,

  /* Do not remount — keep the flags the mount already has.  */
  MOUNT_NO_REMOUNT = 0x20,
};

స్థిర పూర్ణాంకం do_mount (libcrun_container_t *container, స్థిరరాశి అక్షరం *ఆధారం, పూర్ణాంకం targetfd,
                     స్థిరరాశి అక్షరం *లక్ష్యం, స్థిరరాశి అక్షరం *fstype, గుర్తులేని దీర్ఘ mountflags,
                     స్థిరరాశి శూన్య *దత్తాంశం, పూర్ణాంకం extra_flags, libcrun_error_t *err);

స్థిర bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, స్థిరరాశి అక్షరం *option)
{
  size_t i;
  కొరకు (i = 0; i < mnt->options_len; i++)
    ఒకవేళ (mnt->options[i] != ఖాళీ && strcmp (mnt->options[i], option) == 0)
      తిరిగి true;
  తిరిగి false;
}

స్థిర runtime_spec_schema_defs_mount *
కనుగొను_mount_for (libcrun_container_t *container, స్థిరరాశి అక్షరం *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  కొరకు (i = 0; i < def->mounts_len; i++)
    {
      ఒకవేళ (strcmp (def->mounts[i]->destination, destination) == 0)
        తిరిగి def->mounts[i];
    }
  తిరిగి ఖాళీ;
}

స్థిర శూన్య
warn_tmpfs_fallback_once (నిర్మాణం వ్యక్తిగత_దత్తాంశం_s *వ్యక్తిగత_దత్తాంశం, స్థిరరాశి అక్షరం *reason)
{
  ఒకవేళ (! వ్యక్తిగత_దత్తాంశం->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      వ్యక్తిగత_దత్తాంశం->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
స్థిర పూర్ణాంకం
get_shared_ఖాళీలు_dir_cached (libcrun_container_t *container, అక్షరం **proc_fd_మార్గం, libcrun_error_t *err)
{
  నిర్మాణం వ్యక్తిగత_దత్తాంశం_s *వ్యక్తిగత_దత్తాంశం = get_వ్యక్తిగత_దత్తాంశం (container);
  cleanup_close పూర్ణాంకం fd = -1;
  cleanup_free అక్షరం *ఖాళీలు_dir_మార్గం = ఖాళీ;
  proc_fd_మార్గం_t fd_మార్గం;
  పూర్ణాంకం ret;

  /* Fast path: return cached proc fd path if already set up */
  ఒకవేళ (వ్యక్తిగత_దత్తాంశం->maskdir_proc_మార్గం != ఖాళీ)
    {
      ఒకవేళ (proc_fd_మార్గం)
        *proc_fd_మార్గం = వ్యక్తిగత_దత్తాంశం->maskdir_proc_మార్గం;
      తిరిగి 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_ఖాళీలు_directory_మార్గం (&ఖాళీలు_dir_మార్గం, container->context->state_మూలం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, ఖాళీలు_dir_మార్గం, true, true, false, MS_PRIVATE, err);
  ఒకవేళ (fd < 0)
    తిరిగి fd;

  get_self_fd_మార్గం (fd_మార్గం, fd);
  వ్యక్తిగత_దత్తాంశం->maskdir_proc_మార్గం = xstrdup (fd_మార్గం);

  వ్యక్తిగత_దత్తాంశం->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  ఒకవేళ (proc_fd_మార్గం)
    *proc_fd_మార్గం = వ్యక్తిగత_దత్తాంశం->maskdir_proc_మార్గం;
  తిరిగి 0;
}

స్థిర bool
is_dev_null (స్థిరరాశి నిర్మాణం stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  తిరిగి S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
స్థిర పూర్ణాంకం
ensure_dev_null (పూర్ణాంకం fd, libcrun_error_t *err)
{
  నిర్మాణం stat st;

  ఒకవేళ (UNLIKELY (fstat (fd, &st) < 0))
    తిరిగి crun_make_error (err, errno, "stat `/dev/null`");

  ఒకవేళ (UNLIKELY (! is_dev_null (&st)))
    తిరిగి crun_make_error (err, 0, "`/dev/null` is not the null device");

  తిరిగి 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
స్థిర bool
is_ఖాళీలు_ro_tmpfs (పూర్ణాంకం fd)
{
  నిర్మాణం statfs sfs;

  ఒకవేళ (fstatfs (fd, &sfs) < 0)
    తిరిగి false;

  తిరిగి sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

స్థిర పూర్ణాంకం
mount_masked_dir (libcrun_container_t *container, పూర్ణాంకం pathfd, స్థిరరాశి అక్షరం *rel_మార్గం, libcrun_error_t *err)
{
  నిర్మాణం వ్యక్తిగత_దత్తాంశం_s *వ్యక్తిగత_దత్తాంశం = get_వ్యక్తిగత_దత్తాంశం (container);
  cleanup_close పూర్ణాంకం newfd = -1;
  libcrun_error_t tmp_err = ఖాళీ;
  పూర్ణాంకం ret;

  ఒకవేళ (వ్యక్తిగత_దత్తాంశం->maskdir_fd < 0)
    {
      ఒకవేళ (వ్యక్తిగత_దత్తాంశం->maskdir_bind_failed)
        వెళ్ళు fallback_గమ్యం_tmpfs;

      ret = get_shared_ఖాళీలు_dir_cached (container, ఖాళీ, &tmp_err);
      ఒకవేళ (ret < 0)
        {
          వ్యక్తిగత_దత్తాంశం->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (వ్యక్తిగత_దత్తాంశం, tmp_err->సందేశం);
          crun_error_release (&tmp_err);
          వెళ్ళు fallback_గమ్యం_tmpfs;
        }
    }

  ఒకవేళ (వ్యక్తిగత_దత్తాంశం->maskdir_fd >= 0)
    {
      పూర్ణాంకం rootfsfd = వ్యక్తిగత_దత్తాంశం->rootfsfd;
      నిర్మాణం stat before, after;

      ret = fstat (వ్యక్తిగత_దత్తాంశం->maskdir_fd, &before);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "fstat masked dir `%s`", rel_మార్గం);

      ret = fs_move_mount_గమ్యం (వ్యక్తిగత_దత్తాంశం->maskdir_fd, pathfd, ఖాళీ);
      ఒకవేళ (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_మార్గం, true, true, false, MS_PRIVATE, err);
          ఒకవేళ (UNLIKELY (newfd < 0))
            తిరిగి newfd;

          ret = fstat (newfd, &after);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "fstat masked dir `%s`", rel_మార్గం);

          ఒకవేళ (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            తిరిగి crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_మార్గం);

          తాత్కాలిక_FAILURE_RETRY (close (వ్యక్తిగత_దత్తాంశం->maskdir_fd));
          వ్యక్తిగత_దత్తాంశం->maskdir_fd = newfd;
          newfd = -1;

          తిరిగి 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_మార్గం);

      తాత్కాలిక_FAILURE_RETRY (close (వ్యక్తిగత_దత్తాంశం->maskdir_fd));
      వ్యక్తిగత_దత్తాంశం->maskdir_fd = -1;
      వ్యక్తిగత_దత్తాంశం->maskdir_move_failed = true;
    }

  వ్యక్తిగత_దత్తాంశం->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (వ్యక్తిగత_దత్తాంశం, tmp_err ? tmp_err->సందేశం : "unknown error");
  crun_error_release (&tmp_err);

fallback_గమ్యం_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_మార్గం);
  ret = do_mount (container, "tmpfs", pathfd, rel_మార్గం, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  ఒకవేళ (! వ్యక్తిగత_దత్తాంశం->maskdir_move_failed)
    {
      ret = get_bind_mount (వ్యక్తిగత_దత్తాంశం->rootfsfd, rel_మార్గం, true, true, false, MS_PRIVATE, &tmp_err);
      ఒకవేళ (ret >= 0 && is_ఖాళీలు_ro_tmpfs (ret))
        వ్యక్తిగత_దత్తాంశం->maskdir_fd = ret;
      లేకపోతే
        {
          ఒకవేళ (ret >= 0)
            తాత్కాలిక_FAILURE_RETRY (close (ret));
          వ్యక్తిగత_దత్తాంశం->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
do_masked_or_readonly_మార్గం (libcrun_container_t *container, స్థిరరాశి అక్షరం *rel_మార్గం, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  గుర్తులేని దీర్ఘ mount_flags = 0;
  స్థిరరాశి అక్షరం *rootfs = get_వ్యక్తిగత_దత్తాంశం (container)->rootfs;
  cleanup_close పూర్ణాంకం pathfd = -1;
  నిర్మాణం statfs sfs;
  పూర్ణాంకం ret;
  mode_t బహుళకం;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  ఒకవేళ (UNLIKELY (rel_మార్గం == ఖాళీ))
    తిరిగి crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  ఒకవేళ (rel_మార్గం[0] == '/')
    rel_మార్గం++;

  pathfd = సురక్షిత_openat (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, rel_మార్గం, O_మార్గం | O_CLOEXEC, 0, err);
  ఒకవేళ (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      ఒకవేళ (errno != ENOENT && errno != EACCES)
        తిరిగి pathfd;

      ఒకవేళ (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_మార్గం);
      crun_error_release (err);
      తిరిగి 0;
    }

  ఒకవేళ (readonly)
    {
      పూర్ణాంకం procfd = get_procfd (get_వ్యక్తిగత_దత్తాంశం (container), err);
      proc_fd_మార్గం_t ఆధారం_బఫరు;
      cleanup_close పూర్ణాంకం mountfd = -1;

      ఒకవేళ (UNLIKELY (procfd < 0))
        తిరిగి procfd;

      get_self_fd_మార్గం (ఆధారం_బఫరు, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      ఒకవేళ (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, ఆధారం_బఫరు, true, true, false, MS_PRIVATE, err);
          ఒకవేళ (mountfd >= 0)
            ret = fs_move_mount_గమ్యం (mountfd, pathfd, ఖాళీ);
          లేకపోతే
            ret = -1;

          ఒకవేళ (ret < 0)
            crun_error_release (err);
        }

      ఒకవేళ (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          ఒకవేళ (keep_flags)
            {
              cleanup_close పూర్ణాంకం sfd = openat (procfd, ఆధారం_బఫరు, O_మార్గం | O_CLOEXEC);
              ఒకవేళ (UNLIKELY (sfd < 0))
                తిరిగి crun_make_error (err, errno, "openat procfd for `%s`", rel_మార్గం);

              ret = fstatfs (sfd, &sfs);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి crun_make_error (err, errno, "fstatfs `%s`", rel_మార్గం);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, ఆధారం_బఫరు, true, true, false, MS_PRIVATE, err);
          ఒకవేళ (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_మార్గం, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;

              ret = fs_move_mount_గమ్యం (mountfd, pathfd, ఖాళీ);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి crun_make_error (err, errno, "move mount for readonly path `%s`", rel_మార్గం);
            }
          లేకపోతే
            {
              proc_fd_మార్గం_t abs_ఆధారం;

              crun_error_release (err);
              get_proc_self_fd_మార్గం (abs_ఆధారం, pathfd);
              ret = do_mount (container, abs_ఆధారం, pathfd, rel_మార్గం, ఖాళీ, mount_flags, ఖాళీ,
                              LABEL_NONE, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;
            }
        }
    }
  లేకపోతే
    {
      ret = get_file_type_fd (pathfd, &బహుళకం);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "cannot stat `%s`", rel_మార్గం);

      ఒకవేళ ((బహుళకం & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_మార్గం, err);
      లేకపోతే
        {
          cleanup_close పూర్ణాంకం nullfd = -1;
          cleanup_close పూర్ణాంకం mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_మార్గం | O_CLOEXEC);
          ఒకవేళ (UNLIKELY (nullfd < 0))
            తిరిగి crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          ఒకవేళ (mountfd >= 0)
            ret = fs_move_mount_గమ్యం (mountfd, pathfd, ఖాళీ);

          ఒకవేళ (mountfd < 0 || ret < 0)
            {
              proc_fd_మార్గం_t null_మార్గం;

              crun_error_release (err);
              get_proc_self_fd_మార్గం (null_మార్గం, nullfd);
              ret = do_mount (container, null_మార్గం, pathfd, rel_మార్గం, ఖాళీ, MS_BIND | MS_RDONLY, ఖాళీ,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *context_type;

  context_type = కనుగొను_annotation (container, "run.oci.mount_context_type");
  ఒకవేళ (context_type)
    {
      ఒకవేళ (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          తిరిగి ఖాళీ;
        }
      తిరిగి context_type;
    }

  తిరిగి "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
స్థిర పూర్ణాంకం
diagnose_mount_failure (పూర్ణాంకం ret, libcrun_error_t *err, libcrun_container_t *container,
                        స్థిరరాశి అక్షరం *fstype)
{
  ఒకవేళ (fstype && strcmp (fstype, "cgroup2") == 0 && get_వ్యక్తిగత_దత్తాంశం (container)->unified_cgroup_మార్గం)
    {
      ఒకవేళ (access (get_వ్యక్తిగత_దత్తాంశం (container)->unified_cgroup_మార్గం, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          తిరిగి ret;
        }
    }
  తిరిగి ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
స్థిర పూర్ణాంకం
sysfs_userns_fallback (libcrun_container_t *container, స్థిరరాశి అక్షరం *లక్ష్యం,
                       స్థిరరాశి అక్షరం *real_లక్ష్యం, పూర్ణాంకం targetfd,
                       గుర్తులేని దీర్ఘ mountflags, స్థిరరాశి అక్షరం *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close పూర్ణాంకం mountfd = -1;
  cleanup_close పూర్ణాంకం sysfd = -1;
  పూర్ణాంకం sys_old_మూలం_fd = -1;
  పూర్ణాంకం ret;

  ఒకవేళ (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    తిరిగి 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_లోపల_user_namespace (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (ret == 0)
    తిరిగి 1;

  cgroup_mount = కనుగొను_mount_for (container, "/sys/fs/cgroup");
  sys_old_మూలం_fd = get_old_మూలం_fd (get_వ్యక్తిగత_దత్తాంశం (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  ఒకవేళ (sys_old_మూలం_fd >= 0)
    mountfd = get_bind_mount (sys_old_మూలం_fd, "sys", true, true, false, MS_PRIVATE, err);
  ఒకవేళ (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  ఒకవేళ (cgroup_mount == ఖాళీ)
    {
      ఒకవేళ (mountfd >= 0)
        {
          ret = fs_move_mount_గమ్యం (mountfd, targetfd, ఖాళీ);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "move mount `/sys` to `%s`", real_లక్ష్యం);
        }
      లేకపోతే
        {
          crun_error_release (err);
          ఒకవేళ (sys_old_మూలం_fd >= 0)
            {
              cleanup_free అక్షరం *sys_మార్గం = ఖాళీ;
              proc_fd_మార్గం_t sys_మూలం_మార్గం;

              get_proc_self_fd_మార్గం (sys_మూలం_మార్గం, sys_old_మూలం_fd);
              xasprintf (&sys_మార్గం, "%s/sys", sys_మూలం_మార్గం);
              ret = mount (sys_మార్గం, real_లక్ష్యం, ఖాళీ, MS_BIND | MS_REC, ఖాళీ);
            }
          లేకపోతే
            ret = mount ("/sys", real_లక్ష్యం, ఖాళీ, MS_BIND | MS_REC, ఖాళీ);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_లక్ష్యం (container, లక్ష్యం, err);
      ఒకవేళ (UNLIKELY (sysfd < 0))
        తిరిగి sysfd;

      ret = do_remount (sysfd, లక్ష్యం,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        ఖాళీ, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      తిరిగి do_masked_or_readonly_మార్గం (container, "/sys/fs/cgroup", true, false, err);
    }

  ఒకవేళ (UNLIKELY (mountfd < 0))
    తిరిగి mountfd;

  ret = fs_move_mount_గమ్యం (mountfd, targetfd, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "move mount to `%s`", real_లక్ష్యం);

  sysfd = open_mount_లక్ష్యం (container, లక్ష్యం, err);
  ఒకవేళ (UNLIKELY (sysfd < 0))
    తిరిగి sysfd;

  ret = do_remount (sysfd, లక్ష్యం,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_మార్గం (container, "/sys/fs/cgroup", true, false, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

#నిర్వచితమైతే HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
స్థిర శూన్య
apply_selinux_xattr (libcrun_container_t *container, పూర్ణాంకం fd, పూర్ణాంకం extra_flags, స్థిరరాశి అక్షరం *label)
{
  libcrun_error_t tmp_err = ఖాళీ;
  పూర్ణాంకం procfd;

  ఒకవేళ ((extra_flags & LABEL_ముసుగు) != LABEL_XATTR)
    తిరిగి;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_వ్యక్తిగత_దత్తాంశం (container), &tmp_err);
  ఒకవేళ (procfd >= 0)
    {
      proc_fd_మార్గం_t proc_self_మార్గం;
      cleanup_close పూర్ణాంకం xfd = -1;

      get_self_fd_మార్గం (proc_self_మార్గం, fd);
      xfd = openat (procfd, proc_self_మార్గం, O_RDONLY | O_CLOEXEC);
      ఒకవేళ (xfd >= 0)
        (శూన్య) fsetxattr (xfd, "security.selinux", label, అక్షరశ్రేణి_పొడవు (label), 0);
    }
  crun_error_release (&tmp_err);
}
#అయితే_ముగింపు

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
స్థిర పూర్ణాంకం
do_mount_replace_rootfs (libcrun_container_t *container, పూర్ణాంకం *fd, libcrun_error_t *err)
{
  పూర్ణాంకం procfd = get_procfd (get_వ్యక్తిగత_దత్తాంశం (container), err);
  పూర్ణాంకం tmp;
  పూర్ణాంకం ret;

  ఒకవేళ (UNLIKELY (procfd < 0))
    తిరిగి procfd;

  {
    cleanup_close పూర్ణాంకం nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    ఒకవేళ (UNLIKELY (nsfd < 0))
      తిరిగి crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    ఒకవేళ (UNLIKELY (ret < 0))
      తిరిగి crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_వ్యక్తిగత_దత్తాంశం (container)->rootfs, O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (*fd < 0))
    తిరిగి crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  ఒకవేళ (UNLIKELY (tmp < 0))
    తిరిగి crun_make_error (err, errno, "dup");

  తాత్కాలిక_FAILURE_RETRY (close (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd));
  get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd = tmp;

  తిరిగి 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
స్థిర పూర్ణాంకం
apply_ప్రసరణ_flags (స్థిరరాశి అక్షరం *లక్ష్యం, పూర్ణాంకం targetfd, స్థిరరాశి అక్షరం *real_లక్ష్యం,
                         గుర్తులేని దీర్ఘ mountflags, libcrun_error_t *err)
{
  bool ప్రసరణ_పూర్తి = false;
  పూర్ణాంకం ret;

  ఒకవేళ (targetfd >= 0)
    {
      libcrun_error_t tmp_err = ఖాళీ;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, లక్ష్యం, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      ఒకవేళ (LIKELY (ret == 0))
        ప్రసరణ_పూర్తి = true;
      లేకపోతే
        crun_error_release (&tmp_err);
    }

  ఒకవేళ (! ప్రసరణ_పూర్తి)
    {
      ret = mount (ఖాళీ, real_లక్ష్యం, ఖాళీ, mountflags & ALL_PROPAGATIONS, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "set propagation for `%s`", లక్ష్యం);
    }

  తిరిగి 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
స్థిర పూర్ణాంకం
కాలపట్టిక_or_do_remount (libcrun_container_t *container, పూర్ణాంకం *fd, పూర్ణాంకం targetfd,
                        స్థిరరాశి అక్షరం *లక్ష్యం, స్థిరరాశి అక్షరం *real_లక్ష్యం,
                        bool single_instance, గుర్తులేని దీర్ఘ mountflags,
                        స్థిరరాశి శూన్య *దత్తాంశం, పూర్ణాంకం extra_flags, libcrun_error_t *err)
{
  గుర్తులేని దీర్ఘ remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  నిర్మాణం remount_s *r;

  ఒకవేళ ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    తిరిగి do_remount (*fd >= 0 ? *fd : targetfd, real_లక్ష్యం, remount_flags, దత్తాంశం, err);

  ఒకవేళ (*fd < 0)
    {
      *fd = dup (targetfd);
      ఒకవేళ (UNLIKELY (*fd < 0))
        తిరిగి crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), లక్ష్యం, remount_flags, దత్తాంశం, get_వ్యక్తిగత_దత్తాంశం (container)->remounts);
  get_వ్యక్తిగత_దత్తాంశం (container)->remounts = r;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
do_mount (libcrun_container_t *container, స్థిరరాశి అక్షరం *ఆధారం, పూర్ణాంకం targetfd,
          స్థిరరాశి అక్షరం *లక్ష్యం, స్థిరరాశి అక్షరం *fstype, గుర్తులేని దీర్ఘ mountflags, స్థిరరాశి శూన్య *దత్తాంశం,
          పూర్ణాంకం extra_flags, libcrun_error_t *err)
{
  cleanup_free అక్షరం *దత్తాంశం_with_label = ఖాళీ;
  స్థిరరాశి అక్షరం *దత్తాంశం_without_label = దత్తాంశం;
  proc_fd_మార్గం_t లక్ష్యం_బఫరు;
  స్థిరరాశి అక్షరం *context_type = ఖాళీ;
  cleanup_close పూర్ణాంకం ms_move_fd = -1;
  స్థిరరాశి అక్షరం *real_లక్ష్యం = లక్ష్యం;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close పూర్ణాంకం fd = -1;
  స్థిరరాశి అక్షరం *label = ఖాళీ;
  పూర్ణాంకం ret = 0;

  ఒకవేళ (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  లేకపోతే
    extra_flags = (extra_flags & ~LABEL_ముసుగు) | LABEL_NONE;

  ఒకవేళ (targetfd >= 0)
    {
      get_proc_self_fd_మార్గం (లక్ష్యం_బఫరు, targetfd);
      real_లక్ష్యం = లక్ష్యం_బఫరు;
      needs_remount = true;
    }

  ఒకవేళ ((extra_flags & LABEL_ముసుగు) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      ఒకవేళ (UNLIKELY (context_type == ఖాళీ))
        తిరిగి -1;

      ret = కూడు_selinux_mount_label (&దత్తాంశం_with_label, దత్తాంశం, label, context_type, err);
      ఒకవేళ (ret < 0)
        తిరిగి ret;
      దత్తాంశం = దత్తాంశం_with_label;
    }

  ఒకవేళ (mountflags & MS_MOVE)
    {
      ఒకవేళ ((mountflags & MS_BIND) || fstype)
        తిరిగి crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (ఆధారం, real_లక్ష్యం, ఖాళీ, MS_MOVE, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "move mount `%s` to `%s`", ఆధారం, లక్ష్యం);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_లక్ష్యం (container, లక్ష్యం, err);
      ఒకవేళ (UNLIKELY (ms_move_fd < 0))
        తిరిగి ms_move_fd;
      targetfd = ms_move_fd;
    }

  ఒకవేళ ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      గుర్తులేని దీర్ఘ flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      ఒకవేళ (targetfd >= 0)
        {
          ఒకవేళ (mountflags & MS_BIND)
            {
              cleanup_close పూర్ణాంకం bindfd = -1;
              libcrun_error_t tmp_err = ఖాళీ;

              bindfd = get_bind_mount (AT_FDCWD, ఆధారం,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              ఒకవేళ (bindfd < 0)
                {
                  పూర్ణాంకం old_మూలం_fd = get_old_మూలం_fd (get_వ్యక్తిగత_దత్తాంశం (container));

                  ఒకవేళ (old_మూలం_fd >= 0 && ఆధారం && ఆధారం[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_మూలం_fd, (ఆధారం[1] == '\0') ? "." : ఆధారం + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              ఒకవేళ (bindfd >= 0)
                ret = fs_move_mount_గమ్యం (bindfd, targetfd, ఖాళీ);
              ఒకవేళ (ret < 0)
                crun_error_release (&tmp_err);
            }
          లేకపోతే
            {
              cleanup_close పూర్ణాంకం newfs = fsopen_mount (fstype, ఆధారం, context_type, label, దత్తాంశం_without_label);
              ఒకవేళ (newfs >= 0)
                ret = fs_move_mount_గమ్యం (newfs, targetfd, ఖాళీ);
            }
        }

      ఒకవేళ (ret < 0)
        {
          ret = mount (ఆధారం, real_లక్ష్యం, fstype, flags, దత్తాంశం);
          ఒకవేళ (ret < 0 && (mountflags & MS_BIND) && ఆధారం && ఆధారం[0] == '/')
            {
              పూర్ణాంకం old_మూలం_fd = get_old_మూలం_fd (get_వ్యక్తిగత_దత్తాంశం (container));

              ఒకవేళ (old_మూలం_fd >= 0)
                {
                  cleanup_free అక్షరం *proc_ఆధారం_buf = ఖాళీ;
                  proc_fd_మార్గం_t old_మూలం_మార్గం;

                  get_proc_self_fd_మార్గం (old_మూలం_మార్గం, old_మూలం_fd);
                  xasprintf (&proc_ఆధారం_buf, "%s/%s", old_మూలం_మార్గం, ఆధారం + 1);
                  ret = mount (proc_ఆధారం_buf, real_లక్ష్యం, fstype, flags, దత్తాంశం);
                }
            }
        }
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          పూర్ణాంకం saved_errno = errno;

          ret = sysfs_userns_fallback (container, లక్ష్యం, real_లక్ష్యం, targetfd,
                                       mountflags, fstype, err);
          ఒకవేళ (ret == 0)
            తిరిగి 0;
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", ఆధారం, లక్ష్యం);

          తిరిగి diagnose_mount_failure (ret, err, container, fstype);
        }

      ఒకవేళ (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_లక్ష్యం (container, లక్ష్యం, err);
          ఒకవేళ (UNLIKELY (fd < 0))
            తిరిగి fd;

          /* We are replacing the rootfs, reopen it.  */
          ఒకవేళ (is_ఖాళీలు_string (లక్ష్యం))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;
            }

#నిర్వచితమైతే HAVE_FGETXATTR
          apply_selinux_xattr (container, fd, extra_flags, label);
#అయితే_ముగింపు

          targetfd = fd;

          /* The bind mount shadowed the original targetfd, so refresh
             real_target to point to the reopened mountpoint.  Otherwise the
             classic mount(2) fallbacks below (used when mount_setattr is not
             available, e.g. kernels < 5.12) operate on the shadowed path and
             fail with EINVAL.  */
          get_proc_self_fd_మార్గం (లక్ష్యం_బఫరు, targetfd);
          real_లక్ష్యం = లక్ష్యం_బఫరు;
        }
    }

  ఒకవేళ (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_ప్రసరణ_flags (లక్ష్యం, targetfd, real_లక్ష్యం, mountflags, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  ఒకవేళ (దత్తాంశం && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  ఒకవేళ (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = కాలపట్టిక_or_do_remount (container, &fd, targetfd, లక్ష్యం, real_లక్ష్యం,
                                    single_instance, mountflags, దత్తాంశం, extra_flags, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి ret;
}

స్థిర శూన్య
try_umount (పూర్ణాంకం targetfd, స్థిరరాశి అక్షరం *లక్ష్యం)
{
  proc_fd_మార్గం_t లక్ష్యం_బఫరు;
  స్థిరరాశి అక్షరం *real_లక్ష్యం = లక్ష్యం;

  ఒకవేళ (targetfd >= 0)
    {
      get_proc_self_fd_మార్గం (లక్ష్యం_బఫరు, targetfd);
      real_లక్ష్యం = లక్ష్యం_బఫరు;
    }

  umount2 (real_లక్ష్యం, MNT_DETACH);
}

స్థిర bool
container_has_cgroupns (libcrun_container_t *container)
{
  తిరిగి get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWCGROUP;
}

స్థిర పూర్ణాంకం
do_mount_cgroup_v2 (libcrun_container_t *container, పూర్ణాంకం targetfd, స్థిరరాశి అక్షరం *లక్ష్యం,
                    గుర్తులేని దీర్ఘ mountflags, స్థిరరాశి అక్షరం *దత్తాంశం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  పూర్ణాంకం cgroup_బహుళకం;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ret = do_mount (container, "cgroup2", targetfd, లక్ష్యం, "cgroup2", mountflags, దత్తాంశం, LABEL_NONE, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      పూర్ణాంకం saved_errno = crun_error_get_errno (err);
      ఒకవేళ (saved_errno == EPERM || saved_errno == EBUSY)
        {
          స్థిరరాశి అక్షరం *unified_cgroup_మార్గం;
          స్థిరరాశి అక్షరం *src_cgroup;

          crun_error_release (err);

          ఒకవేళ (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, లక్ష్యం, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              ఒకవేళ (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, లక్ష్యం, "cgroup2", mountflags, దత్తాంశం, LABEL_NONE, err);
                  ఒకవేళ (LIKELY (ret == 0))
                    తిరిగి ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, లక్ష్యం);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_మార్గం = get_వ్యక్తిగత_దత్తాంశం (container)->unified_cgroup_మార్గం;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_మార్గం && container_has_cgroupns (container) ? unified_cgroup_మార్గం : CGROUP_మూలం;
          తిరిగి do_mount (container, src_cgroup, targetfd, లక్ష్యం, ఖాళీ,
                           MS_BIND | mountflags, ఖాళీ, LABEL_NONE, err);
        }

      తిరిగి ret;
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, స్థిరరాశి అక్షరం *ఆధారం, పూర్ణాంకం targetfd, స్థిరరాశి అక్షరం *లక్ష్యం,
                            గుర్తులేని దీర్ఘ mountflags, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_close పూర్ణాంకం fd = -1;
  స్థిరరాశి అక్షరం *subsystem = "systemd";
  cleanup_free అక్షరం *subsystem_మార్గం = ఖాళీ;
  cleanup_close పూర్ణాంకం tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, ఆధారం, targetfd, లక్ష్యం, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_లక్ష్యం (container, లక్ష్యం, err);
  ఒకవేళ (UNLIKELY (tmpfsdirfd < 0))
    తిరిగి tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_మార్గం, err, లక్ష్యం, subsystem, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_మార్గం | O_DIRECTORY | O_NOFOLLOW);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "openat `%s`", subsystem_మార్గం);

  తిరిగి do_mount (container, "cgroup", fd, subsystem_మార్గం, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

స్థిర పూర్ణాంకం
do_mount_cgroup_v1 (libcrun_container_t *container, స్థిరరాశి అక్షరం *ఆధారం, పూర్ణాంకం targetfd, స్థిరరాశి అక్షరం *లక్ష్యం,
                    గుర్తులేని దీర్ఘ mountflags, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_free అక్షరం *content = ఖాళీ;
  అక్షరం *from;
  cleanup_close పూర్ణాంకం tmpfsdirfd = -1;
  అక్షరం *saveptr = ఖాళీ;

  ret = do_mount (container, ఆధారం, targetfd, లక్ష్యం, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  tmpfsdirfd = open_mount_లక్ష్యం (container, లక్ష్యం, err);
  ఒకవేళ (UNLIKELY (tmpfsdirfd < 0))
    తిరిగి tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    పూర్ణాంకం procfd = get_procfd (get_వ్యక్తిగత_దత్తాంశం (container), err);
    ఒకవేళ (UNLIKELY (procfd < 0))
      తిరిగి procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, ఖాళీ, err);
  }
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (UNLIKELY (content == ఖాళీ || content[0] == '\0'))
    తిరిగి crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  కొరకు (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ఖాళీ, "\n", &saveptr))
    {
      cleanup_free అక్షరం *ఆధారం_మార్గం = ఖాళీ;
      cleanup_free అక్షరం *ఆధారం_subsystem = ఖాళీ;
      cleanup_free అక్షరం *subsystem_మార్గం = ఖాళీ;
      అక్షరం *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close పూర్ణాంకం subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      ఒకవేళ (subsystem[0] == '\0')
        కొనసాగు;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = అక్షరశ్రేణి_శోధన (subsystem, "name=");
      ఒకవేళ (it)
        subsystem = it + 5;

      ఒకవేళ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ఒకవేళ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&ఆధారం_subsystem, err, CGROUP_మూలం, subsystem, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      /* if there is already a mount specified, do not add a default one.  */
      ఒకవేళ (కనుగొను_mount_for (container, ఆధారం_subsystem) != ఖాళీ)
        కొనసాగు;

      ret = append_paths (&ఆధారం_మార్గం, err, ఆధారం_subsystem, subpath, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = append_paths (&subsystem_మార్గం, err, లక్ష్యం, subsystem, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "mkdirat `%s`", subsystem_మార్గం);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_మార్గం | O_DIRECTORY | O_NOFOLLOW);
      ఒకవేళ (UNLIKELY (subsystemfd < 0))
        తిరిగి crun_make_error (err, errno, "open `%s`", subsystem_మార్గం);

      ఒకవేళ (container_has_cgroupns (container))
        {
          ret = do_mount (container, ఆధారం_మార్గం, subsystemfd, subsystem_మార్గం, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ఒకవేళ (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  కొనసాగు;
                }
              తిరిగి ret;
            }
        }
      లేకపోతే
        {
          ret = do_mount (container, ఆధారం_మార్గం, subsystemfd, subsystem_మార్గం, ఖాళీ, MS_BIND | mountflags, ఖాళీ,
                          LABEL_NONE, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ఒకవేళ (crun_error_get_errno (err) != ENOENT)
                తిరిగి ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, ఆధారం_subsystem, subsystemfd, subsystem_మార్గం, ఖాళీ, MS_BIND | mountflags,
                              ఖాళీ, LABEL_NONE, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  ఒకవేళ (crun_error_get_errno (err) != ENOENT)
                    తిరిగి ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
do_mount_cgroup (libcrun_container_t *container, స్థిరరాశి అక్షరం *ఆధారం, పూర్ణాంకం targetfd, స్థిరరాశి అక్షరం *లక్ష్యం,
                 గుర్తులేని దీర్ఘ mountflags, స్థిరరాశి అక్షరం *దత్తాంశం, libcrun_error_t *err)
{
  పూర్ణాంకం cgroup_బహుళకం;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఎంపిక (cgroup_బహుళకం)
    {
    స్థితి CGROUP_బహుళకం_UNIFIED:
      తిరిగి do_mount_cgroup_v2 (container, targetfd, లక్ష్యం, mountflags, దత్తాంశం, err);
    స్థితి CGROUP_బహుళకం_LEGACY:
    స్థితి CGROUP_బహుళకం_HYBRID:
      తిరిగి do_mount_cgroup_v1 (container, ఆధారం, targetfd, లక్ష్యం, mountflags, err);
    }

  తిరిగి crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_బహుళకం);
}

నిర్మాణం device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
స్థిర శూన్య
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  నిర్మాణం వ్యక్తిగత_దత్తాంశం_s *వ్యక్తిగత_దత్తాంశం = get_వ్యక్తిగత_దత్తాంశం (container);
  size_t i;

  ఒకవేళ (వ్యక్తిగత_దత్తాంశం->needed_devs_fds)
    {
      నిర్మాణం libcrun_fd_map *dev_fds = వ్యక్తిగత_దత్తాంశం->needed_devs_fds;

      కొరకు (i = 0; needed_devs[i].మార్గం; i++)
        {
          ఒకవేళ (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            కొనసాగు;

          పూర్ణాంకం fd = syscall_open_వృక్షం (AT_FDCWD, needed_devs[i].మార్గం,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          ఒకవేళ (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          లేకపోతే ఒకవేళ (fd >= 0)
            close (fd);
        }
    }

  ఒకవేళ (వ్యక్తిగత_దత్తాంశం->notify_socket_వృక్షం_fd < 0
      && వ్యక్తిగత_దత్తాంశం->host_notify_socket_మార్గం)
    {
      పూర్ణాంకం fd = syscall_open_వృక్షం (AT_FDCWD, వ్యక్తిగత_దత్తాంశం->host_notify_socket_మార్గం,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      ఒకవేళ (fd >= 0)
        వ్యక్తిగత_దత్తాంశం->notify_socket_వృక్షం_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
స్థిర స్థిరరాశి అక్షరం *
relative_మార్గం_under_dev (స్థిరరాశి అక్షరం *మార్గం)
{
  ఒకవేళ (మార్గం[0] != '/')
    తిరిగి మార్గం;
  ఒకవేళ (అక్షరశ్రేణి_పోలిక (మార్గం, "/dev/", 5) == 0)
    {
      ఒకవేళ (strchr (మార్గం + 5, '/'))
        తిరిగి ఖాళీ;
      తిరిగి మార్గం + 5;
    }
  తిరిగి ఖాళీ;
}

స్థిర పూర్ణాంకం
mknod_and_సమితి_attrs (libcrun_container_t *container, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *name, నిర్మాణం device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం fd = -1;
  proc_fd_మార్గం_t fd_మార్గం;
  స్థిరరాశి అక్షరం *what = "new";
  bool exists = false;
  నిర్మాణం stat st;
  పూర్ణాంకం procfd;
  పూర్ణాంకం ret;

  ret = mknodat (dirfd, name, device->బహుళకం | type, dev);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno != EEXIST)
        తిరిగి crun_make_error (err, errno, "mknodat `%s`", device->మార్గం);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_మార్గం | O_NOFOLLOW | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "open %s device `%s`", what, device->మార్గం);

  ret = fstat (fd, &st);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fstat %s device `%s`", what, device->మార్గం);

  ఒకవేళ (UNLIKELY ((st.st_mode & S_IFMT) != type))
    తిరిగి crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->మార్గం, st.st_mode & S_IFMT, type);

  ఒకవేళ (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    తిరిగి crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->మార్గం, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  ఒకవేళ (exists)
    తిరిగి 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_వ్యక్తిగత_దత్తాంశం (container), err);
  ఒకవేళ (UNLIKELY (procfd < 0))
    తిరిగి procfd;

  get_self_fd_మార్గం (fd_మార్గం, fd);
  ret = fchmodat (procfd, fd_మార్గం, device->బహుళకం, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fchmod `%s`", device->మార్గం);

  ret = fchownat (fd, "", device->uid, device->gid, AT_ఖాళీలు_మార్గం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fchown `%s`", device->మార్గం);

  తిరిగి 1;
}

పూర్ణాంకం
libcrun_create_dev (libcrun_container_t *container, పూర్ణాంకం devfd, పూర్ణాంకం srcfd,
                    నిర్మాణం device_s *device, bool binds, bool ensure_మాతృ_dir,
                    libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  స్థిరరాశి అక్షరం *fullname = device->మార్గం;
  స్థిరరాశి అక్షరం *rootfs = get_వ్యక్తిగత_దత్తాంశం (container)->rootfs;
  ఒకవేళ (is_ఖాళీలు_string (fullname))
    తిరిగి crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free అక్షరం *normalized_మార్గం = xstrdup (fullname);
  consume_trailing_slashes (normalized_మార్గం);
  ఒకవేళ (normalized_మార్గం[0] == '\0')
    strcpy (normalized_మార్గం, "/");
  స్థిరరాశి అక్షరం *rel_dev = relative_మార్గం_under_dev (normalized_మార్గం);

  ఒకవేళ (binds)
    {
      cleanup_close పూర్ణాంకం fd = -1;

      ఒకవేళ (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_మార్గం | O_NONBLOCK);
          ఒకవేళ (UNLIKELY (fd < 0))
            {
              ఒకవేళ (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              ఒకవేళ (UNLIKELY (fd < 0))
                తిరిగి crun_make_error (err, errno, "openat `%s`", device->మార్గం);
            }
        }
      లేకపోతే
        {
          స్థిరరాశి అక్షరం *rel_మార్గం = consume_slashes (normalized_మార్గం);

          fd = crun_సురక్షిత_create_and_open_ref_at (false, get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, rel_మార్గం, 0755, err);
          ఒకవేళ (UNLIKELY (fd < 0))
            తిరిగి fd;
        }

      ఒకవేళ (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_మార్గం, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          ఒకవేళ (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              ఒకవేళ (LIKELY (ret >= 0))
                తిరిగి 0;
            }
          లేకపోతే
            crun_error_release (err);
        }

      {
        cleanup_close పూర్ణాంకం mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        ఒకవేళ (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_మార్గం, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            ఒకవేళ (LIKELY (ret == 0))
              {
                ret = fs_move_mount_గమ్యం (mountfd, fd, ఖాళీ);
                ఒకవేళ (LIKELY (ret == 0))
                  తిరిగి 0;
              }
            లేకపోతే
              crun_error_release (err);
          }
        లేకపోతే
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_మార్గం, ఖాళీ, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, ఖాళీ, LABEL_MOUNT, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      ఒకవేళ (rel_dev)
        {
          ret = mknod_and_సమితి_attrs (container, devfd, rel_dev, device, type, dev, err);
          ఒకవేళ (ret <= 0)
            తిరిగి ret;
        }
      లేకపోతే
        {
          cleanup_close పూర్ణాంకం dirfd = -1;
          cleanup_free అక్షరం *dirname = ఖాళీ;
          అక్షరం *basename, *దొరికింది;

          dirname = xstrdup (normalized_మార్గం);

          దొరికింది = strrchr (dirname, '/');
          ఒకవేళ (దొరికింది)
            *దొరికింది = '\0';

          basename = దొరికింది ? దొరికింది + 1 : dirname;

          ఒకవేళ (dirname[0] == '\0')
            {
              dirfd = dup (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd);
              ఒకవేళ (UNLIKELY (dirfd < 0))
                తిరిగి crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          లేకపోతే
            {
              dirfd = సురక్షిత_openat (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_మార్గం | O_CLOEXEC, 0, err);
              ఒకవేళ (dirfd < 0 && ensure_మాతృ_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_సురక్షిత_create_and_open_ref_at (true, get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              ఒకవేళ (UNLIKELY (dirfd < 0))
                తిరిగి dirfd;
            }

          ret = mknod_and_సమితి_attrs (container, dirfd, basename, device, type, dev, err);
          ఒకవేళ (ret <= 0)
            తిరిగి ret;
        }
    }
  తిరిగి 0;
}

నిర్మాణం symlink_s
{
  స్థిరరాశి అక్షరం *మార్గం;
  స్థిరరాశి అక్షరం *లక్ష్యం;
  bool force;
};

స్థిర నిర్మాణం symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { ఖాళీ, ఖాళీ, false } };

స్థిర పూర్ణాంకం
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  size_t i;
  నిర్మాణం device_s *it;
  cleanup_close పూర్ణాంకం devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  స్థిరరాశి అక్షరం *rootfs = get_వ్యక్తిగత_దత్తాంశం (container)->rootfs;
  cleanup_close_map నిర్మాణం libcrun_fd_map *dev_fds = ఖాళీ;
  cleanup_close_map నిర్మాణం libcrun_fd_map *needed_fds = ఖాళీ;

  dev_fds = get_వ్యక్తిగత_దత్తాంశం (container)->dev_fds;
  get_వ్యక్తిగత_దత్తాంశం (container)->dev_fds = ఖాళీ;

  needed_fds = get_వ్యక్తిగత_దత్తాంశం (container)->needed_devs_fds;
  get_వ్యక్తిగత_దత్తాంశం (container)->needed_devs_fds = ఖాళీ;

  ఒకవేళ (! def || ! def->linux)
    తిరిగి 0;

  devfd = సురక్షిత_openat (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_మార్గం | O_DIRECTORY, 0, err);
  ఒకవేళ (UNLIKELY (devfd < 0))
    తిరిగి devfd;

  కొరకు (i = 0; i < def->linux->devices_len; i++)
    {
      నిర్మాణం device_s device = {
        def->linux->devices[i]->మార్గం,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_బహుళకం,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      ఒకవేళ (! def->linux->devices[i]->file_బహుళకం_present)
        device.బహుళకం = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  కొరకు (it = needed_devs, i = 0; it->మార్గం; it++, i++)
    {
      cleanup_close పూర్ణాంకం srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      ఒకవేళ (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  కొరకు (i = 0; symlinks[i].లక్ష్యం; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].మార్గం, devfd, symlinks[i].లక్ష్యం);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          పూర్ణాంకం saved_errno = errno;

          ఒకవేళ (errno == EEXIST && ! symlinks[i].force)
            కొనసాగు;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          ఒకవేళ (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].లక్ష్యం, 0);
              ఒకవేళ (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].లక్ష్యం, AT_REMOVEDIR);
              ఒకవేళ (ret < 0 && errno == EBUSY)
                {
                  cleanup_close పూర్ణాంకం tfd = openat (devfd, symlinks[i].లక్ష్యం, O_CLOEXEC | O_మార్గం | O_NOFOLLOW);
                  ఒకవేళ (tfd >= 0)
                    {
                      proc_fd_మార్గం_t procpath;

                      get_proc_self_fd_మార్గం (procpath, tfd);
                      ఒకవేళ (umount2 (procpath, MNT_DETACH) == 0)
                        వెళ్ళు retry_unlink;
                    }
                }
              ఒకవేళ (ret == 0)
                వెళ్ళు retry_symlink;
            }
          తిరిగి crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].లక్ష్యం);
        }
    }

  ఒకవేళ (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
స్థిర స్థిరరాశి అక్షరం *
తదుపరి_మార్గం_component (స్థిరరాశి అక్షరం *p, size_t *len)
{
  కొరకు (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      ఒకవేళ (*len != 1 || p[0] != '.')
        తిరిగి p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
స్థిర bool
same_మార్గం (స్థిరరాశి అక్షరం *a, స్థిరరాశి అక్షరం *b)
{
  size_t la, lb;

  కొరకు (;;)
    {
      a = తదుపరి_మార్గం_component (a, &la);
      b = తదుపరి_మార్గం_component (b, &lb);
      ఒకవేళ (la != lb || memcmp (a, b, la) != 0)
        తిరిగి false;
      ఒకవేళ (la == 0)
        తిరిగి true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
స్థిర bool
is_duplicate_మార్గం (అక్షరం **paths, size_t i)
{
  size_t j;

  ఒకవేళ (paths[i] == ఖాళీ)
    తిరిగి false;

  కొరకు (j = 0; j < i; j++)
    ఒకవేళ (paths[j] && same_మార్గం (paths[j], paths[i]))
      తిరిగి true;

  తిరిగి false;
}

స్థిర పూర్ణాంకం
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  పూర్ణాంకం ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  ఒకవేళ (! def || ! def->linux)
    తిరిగి 0;

  కొరకు (i = 0; i < def->linux->masked_paths_len; i++)
    {
      ఒకవేళ (is_duplicate_మార్గం (def->linux->masked_paths, i))
        కొనసాగు;

      ret = do_masked_or_readonly_మార్గం (container, def->linux->masked_paths[i], false, false, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  కొరకు (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_మార్గం (container, def->linux->readonly_paths[i], true, true, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
do_ఇరుసు (libcrun_container_t *container, స్థిరరాశి అక్షరం *rootfs, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం oldrootfd = -1;
  cleanup_close పూర్ణాంకం newrootfd = -1;
  పూర్ణాంకం ret;

  oldrootfd = open ("/", O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (oldrootfd < 0))
    తిరిగి crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (newrootfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = ఇరుసు_మూలం (".", ".");
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, ఖాళీ, -1, ".", ఖాళీ, MS_REC | MS_PRIVATE, ఖాళీ, LABEL_MOUNT, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = umount2 (".", MNT_DETACH);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "umount2 oldroot");

  చేయి
    {
      ret = umount2 (".", MNT_DETACH);
      ఒకవేళ (ret < 0 && errno == EINVAL)
        ఆపు;
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "umount2 oldroot");
  } అయితే (ret == 0);

  ret = chdir ("/");
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "chdir to newroot");

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
append_tmpfs_బహుళకం_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, అక్షరం **దత్తాంశం, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *rootfs = get_వ్యక్తిగత_దత్తాంశం (container)->rootfs;
  bool ఖాళీలు_దత్తాంశం = is_ఖాళీలు_string (*దత్తాంశం);
  cleanup_close పూర్ణాంకం fd = -1;
  నిర్మాణం stat st;
  పూర్ణాంకం ret;

  ఒకవేళ (*దత్తాంశం != ఖాళీ && అక్షరశ్రేణి_శోధన (*దత్తాంశం, "mode="))
    తిరిగి 0;

  fd = సురక్షిత_openat (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_మార్గం, 0, err);
  ఒకవేళ (fd < 0)
    {
      ఒకవేళ (crun_error_get_errno (err) != ENOENT)
        తిరిగి fd;

      crun_error_release (err);
      తిరిగి 0;
    }
  ret = fstat (fd, &st);
  ఒకవేళ (ret < 0)
    తిరిగి crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (దత్తాంశం, "%s%smode=%o", ఖాళీలు_దత్తాంశం ? "" : *దత్తాంశం, ఖాళీలు_దత్తాంశం ? "" : ",", st.st_mode & 07777);
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
get_default_flags (libcrun_container_t *container, స్థిరరాశి అక్షరం *destination, అక్షరం **దత్తాంశం)
{
  ఒకవేళ (strcmp (destination, "/proc") == 0)
    తిరిగి 0;
  ఒకవేళ (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *దత్తాంశం = xstrdup ("none,name=");
      తిరిగి MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  ఒకవేళ (strcmp (destination, "/dev") == 0)
    {
      *దత్తాంశం = xstrdup ("mode=755");
      తిరిగి MS_NOEXEC | MS_STRICTATIME;
    }
  ఒకవేళ (strcmp (destination, "/dev/shm") == 0)
    {
      *దత్తాంశం = xstrdup ("mode=1777,size=65536k");
      తిరిగి MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  ఒకవేళ (strcmp (destination, "/dev/mqueue") == 0)
    తిరిగి MS_NOEXEC | MS_NOSUID | MS_NODEV;
  ఒకవేళ (strcmp (destination, "/dev/pts") == 0)
    {
      ఒకవేళ (container->host_uid == 0)
        *దత్తాంశం = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      లేకపోతే
        *దత్తాంశం = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      తిరిగి MS_NOEXEC | MS_NOSUID;
    }
  ఒకవేళ (strcmp (destination, "/sys") == 0)
    తిరిగి MS_NOEXEC | MS_NOSUID | MS_NODEV;

  తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  తిరిగి కనుగొను_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

స్థిర పూర్ణాంకం
check_valid_no_follow_file_system (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *destination, libcrun_error_t *err)
{
  నిర్మాణం statfs sfs;

  పూర్ణాంకం ret = fstatfs (fd, &sfs);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "statfs `%s`", destination);

  ఎంపిక (sfs.f_type)
    {
    స్థితి PROC_SUPER_MAGIC:
    స్థితి SYSFS_MAGIC:
    స్థితి DEVPTS_SUPER_MAGIC:
      తిరిగి crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
సురక్షిత_create_symlink (పూర్ణాంకం rootfsfd, స్థిరరాశి అక్షరం *rootfs, స్థిరరాశి అక్షరం *లక్ష్యం, స్థిరరాశి అక్షరం *destination, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం మాతృ_dir_fd = -1;
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  అక్షరం *part;
  పూర్ణాంకం ret;

  ఒకవేళ (is_ఖాళీలు_string (destination))
    తిరిగి crun_make_error (err, 0, "empty destination for symlink `%s`", లక్ష్యం);

  బఫరు = xstrdup (destination);
  part = dirname (బఫరు);

  మాతృ_dir_fd = crun_సురక్షిత_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  ఒకవేళ (UNLIKELY (మాతృ_dir_fd < 0))
    తిరిగి మాతృ_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (బఫరు, destination);
  part = basename (బఫరు);

  ret = symlinkat (లక్ష్యం, మాతృ_dir_fd, part);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      ఒకవేళ (errno == EEXIST)
        {
          cleanup_free అక్షరం *link = ఖాళీ;
          ssize_t len;

          len = సురక్షిత_readlinkat (మాతృ_dir_fd, part, &link, 0, err);
          ఒకవేళ (UNLIKELY (len < 0))
            తిరిగి len;

          ఒకవేళ ((((size_t) len) == అక్షరశ్రేణి_పొడవు (లక్ష్యం)) && అక్షరశ్రేణి_పోలిక (link, లక్ష్యం, len) == 0)
            తిరిగి 0;

          తిరిగి crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      తిరిగి crun_make_error (err, errno, "symlinkat `%s`", లక్ష్యం);
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
handle_copy_symlink (libcrun_container_t *container, స్థిరరాశి అక్షరం *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     స్థిరరాశి అక్షరం *link_లక్ష్యం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *లక్ష్యం = ఖాళీ;

  ఒకవేళ (link_లక్ష్యం == ఖాళీ)
    {
      పూర్ణాంకం old_మూలం_fd = get_old_మూలం_fd (get_వ్యక్తిగత_దత్తాంశం (container));
      స్థిరరాశి అక్షరం *ఆధారం = mount->ఆధారం;
      ssize_t len;

      ఒకవేళ (old_మూలం_fd >= 0 && ఆధారం[0] == '/')
        len = సురక్షిత_readlinkat (old_మూలం_fd, ఆధారం + 1, &లక్ష్యం, 0, err);
      లేకపోతే
        len = సురక్షిత_readlinkat (AT_FDCWD, ఆధారం, &లక్ష్యం, 0, err);
      ఒకవేళ (UNLIKELY (len < 0))
        తిరిగి len;
      link_లక్ష్యం = లక్ష్యం;
    }

  తిరిగి సురక్షిత_create_symlink (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs,
                              link_లక్ష్యం, mount->destination, err);
}

స్థిర పూర్ణాంకం
prepare_sysfs_or_proc_mount (libcrun_container_t *container, స్థిరరాశి అక్షరం *లక్ష్యం,
                             పూర్ణాంకం *targetfd, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, లక్ష్యం,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno == ENOENT)
        {
          ఒకవేళ (strchr (లక్ష్యం, '/'))
            తిరిగి crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", లక్ష్యం);

          ret = mkdirat (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, లక్ష్యం, 0755);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "mkdirat `%s`", లక్ష్యం);

          /* Try opening it again.  */
          ret = openat (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, లక్ష్యం,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      లేకపోతే ఒకవేళ (errno == ENOTDIR)
        తిరిగి crun_make_error (err, errno, "the target `/%s` is invalid", లక్ష్యం);

      ఒకవేళ (ret < 0)
        తిరిగి crun_make_error (err, errno, "open `%s`", లక్ష్యం);
    }

  *targetfd = ret;
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
handle_tmpcopyup (libcrun_container_t *container, స్థిరరాశి అక్షరం *rootfs, స్థిరరాశి అక్షరం *లక్ష్యం,
                  పూర్ణాంకం copy_from_fd, libcrun_error_t *err)
{
  పూర్ణాంకం destfd, ret;
  cleanup_close పూర్ణాంకం tmpfd = copy_from_fd;

  లక్ష్యం = consume_slashes (లక్ష్యం);

  ఒకవేళ (is_ఖాళీలు_string (లక్ష్యం))
    తిరిగి crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = సురక్షిత_openat (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, లక్ష్యం,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  ఒకవేళ (UNLIKELY (destfd < 0))
    తిరిగి crun_error_wrap (err, "open `%s` to write for tmpcopyup", లక్ష్యం);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_గమ్యం_fd (tmpfd, destfd, లక్ష్యం, లక్ష్యం, err);
  tmpfd = -1;
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
get_mount_label_how (స్థిరరాశి అక్షరం *type, bool is_sysfs_or_proc)
{
  ఒకవేళ (is_sysfs_or_proc)
    తిరిగి LABEL_NONE;

  ఒకవేళ (strcmp (type, "mqueue") == 0)
    తిరిగి LABEL_XATTR;

  తిరిగి LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
స్థిర గుర్తులేని దీర్ఘ
get_bind_mount_locked_flags (పూర్ణాంకం ఆధారం_mountfd, స్థిరరాశి అక్షరం *ఆధారం, runtime_spec_schema_defs_mount *mount)
{
  గుర్తులేని దీర్ఘ cleared = 0;
  నిర్మాణం statfs sfs;
  size_t i;
  పూర్ణాంకం ret;

  ఒకవేళ (ఆధారం_mountfd >= 0)
    ret = fstatfs (ఆధారం_mountfd, &sfs);
  లేకపోతే
    ret = statfs (ఆధారం, &sfs);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి 0;

  కొరకు (i = 0; i < mount->options_len; i++)
    {
      స్థిరరాశి నిర్మాణం ప్రసరణ_flags_s *prop;

      ఒకవేళ (mount->options[i] == ఖాళీ)
        కొనసాగు;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ఒకవేళ (prop == ఖాళీ || (prop->extra_flags & OPTION_RECURSIVE))
        కొనసాగు;

      ఒకవేళ (prop->clear)
        cleared |= prop->flags;
      లేకపోతే
        cleared &= ~prop->flags;
    }

  తిరిగి sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
స్థిర స్థిరరాశి అక్షరం *
get_మార్గం_లోపల_rootfs (స్థిరరాశి అక్షరం *rootfs, స్థిరరాశి అక్షరం *మార్గం)
{
  size_t len;

  ఒకవేళ (rootfs == ఖాళీ || మార్గం == ఖాళీ)
    తిరిగి ఖాళీ;

  len = అక్షరశ్రేణి_పొడవు (rootfs);
  అయితే (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  ఒకవేళ (len == 0)
    తిరిగి ఖాళీ;

  ఒకవేళ (అక్షరశ్రేణి_పోలిక (మార్గం, rootfs, len) != 0 || (మార్గం[len] != '/' && మార్గం[len] != '\0'))
    తిరిగి ఖాళీ;

  తిరిగి consume_slashes (మార్గం + len);
}

స్థిర పూర్ణాంకం
process_single_mount (libcrun_container_t *container, స్థిరరాశి అక్షరం *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      నిర్మాణం libcrun_fd_map *mount_fds, size_t mount_సూచీ,
                      స్థిరరాశి అక్షరం *systemd_cgroup_v1, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *లక్ష్యం = consume_slashes (mount->destination);
  cleanup_close పూర్ణాంకం ఆధారం_mountfd = -1;
  cleanup_free అక్షరం *దత్తాంశం = ఖాళీ;
  అక్షరం *type;
  అక్షరం *ఆధారం;
  గుర్తులేని దీర్ఘ flags = 0;
  గుర్తులేని దీర్ఘ extra_flags = 0;
  mode_t src_బహుళకం = S_IFDIR;
  cleanup_close పూర్ణాంకం copy_from_fd = -1;
  cleanup_close పూర్ణాంకం targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_సమితి = 0;
  పూర్ణాంకం no_remount = 0;
  పూర్ణాంకం ret;

  ఒకవేళ (mount_fds)
    ఆధారం_mountfd = get_and_reset (&(mount_fds->fds[mount_సూచీ]));

  type = mount->type;

  ఒకవేళ (mount->options == ఖాళీ)
    flags = get_default_flags (container, mount->destination, &దత్తాంశం);
  లేకపోతే
    {
      size_t j;

      కొరకు (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &దత్తాంశం, &rec_clear, &rec_సమితి);
    }

  ఒకవేళ (type == ఖాళీ && (flags & MS_BIND) == 0)
    తిరిగి crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  ఒకవేళ ((flags & MS_BIND) && ఆధారం_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      స్థిరరాశి అక్షరం *rel_ఆధారం = get_మార్గం_లోపల_rootfs (get_వ్యక్తిగత_దత్తాంశం (container)->host_rootfs, mount->ఆధారం);

      ఒకవేళ (rel_ఆధారం)
        {
          గుర్తులేని దీర్ఘ ప్రసరణ = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          ఆధారం_mountfd = get_bind_mount (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rel_ఆధారం, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, ప్రసరణ, err);
          ఒకవేళ (UNLIKELY (ఆధారం_mountfd < 0))
            తిరిగి crun_error_wrap (err, "open mount source `%s`", mount->ఆధారం);
        }
    }

  ఒకవేళ ((flags & MS_BIND) && mount->ఆధారం && has_mount_flag_options (mount)
      && (get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (ఆధారం_mountfd, mount->ఆధారం, mount);

  ఒకవేళ (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      ఒకవేళ (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      ఒకవేళ (మార్గం_is_slash_dev (mount->destination))
        get_వ్యక్తిగత_దత్తాంశం (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  ఒకవేళ (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_బహుళకం_if_missing (container, mount, &దత్తాంశం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (mount->ఆధారం && (flags & MS_BIND))
    {
      proc_fd_మార్గం_t proc_buf;
      స్థిరరాశి అక్షరం *మార్గం = mount->ఆధారం;

      ఒకవేళ ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        తిరిగి crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_వ్యక్తిగత_దత్తాంశం (container)->copy_symlink_targets
                               && mount_సూచీ < get_వ్యక్తిగత_దత్తాంశం (container)->n_copy_symlink_targets
                               && get_వ్యక్తిగత_దత్తాంశం (container)->copy_symlink_targets[mount_సూచీ];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      ఒకవేళ (ఆధారం_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_మార్గం (proc_buf, ఆధారం_mountfd);
          మార్గం = proc_buf;

          ret = get_file_type_at (ఆధారం_mountfd, &src_బహుళకం, true, ఖాళీ);
        }
      లేకపోతే ఒకవేళ (is_cached_symlink)
        {
          src_బహుళకం = S_IFLNK;
          ret = 0;
        }
      లేకపోతే ఒకవేళ (మార్గం[0] == '/')
        {
          పూర్ణాంకం old_మూలం_fd = get_old_మూలం_fd (get_వ్యక్తిగత_దత్తాంశం (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          ఒకవేళ (old_మూలం_fd >= 0)
            ret = get_file_type_at (old_మూలం_fd, &src_బహుళకం, nofollow, (మార్గం[1] == '\0') ? ఖాళీ : మార్గం + 1);
          లేకపోతే
            ret = get_file_type_at (AT_FDCWD, &src_బహుళకం, nofollow, మార్గం);
        }
      లేకపోతే
        ret = get_file_type (&src_బహుళకం, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, మార్గం);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "cannot stat `%s`", మార్గం);

      ఒకవేళ (S_ISLNK (src_బహుళకం) && (extra_flags & OPTION_DEST_NOFOLLOW) && ఆధారం_mountfd < 0)
        {
          పూర్ణాంకం bind_dirfd = AT_FDCWD;
          స్థిరరాశి అక్షరం *bind_ఆధారం = mount->ఆధారం;
          పూర్ణాంకం old_మూలం_fd = get_old_మూలం_fd (get_వ్యక్తిగత_దత్తాంశం (container));

          ఒకవేళ (old_మూలం_fd >= 0 && mount->ఆధారం && mount->ఆధారం[0] == '/')
            {
              bind_dirfd = old_మూలం_fd;
              bind_ఆధారం = (mount->ఆధారం[1] == '\0') ? "." : mount->ఆధారం + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_ఆధారం, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          ఆధారం_mountfd = ret;
        }
    }

  ఒకవేళ (S_ISLNK (src_బహుళకం) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      స్థిరరాశి అక్షరం *cached_లక్ష్యం = ఖాళీ;

      ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->copy_symlink_targets
          && mount_సూచీ < get_వ్యక్తిగత_దత్తాంశం (container)->n_copy_symlink_targets)
        cached_లక్ష్యం = get_వ్యక్తిగత_దత్తాంశం (container)->copy_symlink_targets[mount_సూచీ];

      ret = handle_copy_symlink (container, rootfs, mount, cached_లక్ష్యం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      mounted = true;
    }
  లేకపోతే ఒకవేళ (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, లక్ష్యం, &targetfd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      bool is_dir = S_ISDIR (src_బహుళకం);

      ఒకవేళ (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = సురక్షిత_openat (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, లక్ష్యం, O_మార్గం | O_NOFOLLOW | O_CLOEXEC, 0, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, లక్ష్యం, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
      లేకపోతే
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_సురక్షిత_create_and_open_ref_at (is_dir, get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, లక్ష్యం, 0755, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          targetfd = ret;
        }
    }

  ఒకవేళ (extra_flags & OPTION_TMPCOPYUP)
    {
      ఒకవేళ (strcmp (type, "tmpfs") != 0)
        తిరిగి crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      ఒకవేళ (UNLIKELY (copy_from_fd < 0))
        {
          ఒకవేళ (errno != ENOTDIR)
            తిరిగి crun_make_error (err, errno, "cannot reopen `%s`", లక్ష్యం);
        }
    }

  ఆధారం = mount->ఆధారం ? mount->ఆధారం : type;

  /* Check if there is already a mount for the requested file system.  */
  ఒకవేళ (! mounted && ఆధారం_mountfd >= 0)
    {
      ret = fs_move_mount_గమ్యం (ఆధారం_mountfd, targetfd, ఖాళీ);
      ఒకవేళ (LIKELY (ret == 0))
        {
          గుర్తులేని దీర్ఘ మిగిలిన_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          ఒకవేళ (మిగిలిన_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, ఖాళీ, ఆధారం_mountfd, లక్ష్యం, ఖాళీ, మిగిలిన_flags, దత్తాంశం, LABEL_NONE | no_remount, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;
            }

          mounted = true;

          ఒకవేళ (is_ఖాళీలు_string (లక్ష్యం))
            {
              పూర్ణాంకం new_rootfsfd = open (rootfs, O_మార్గం | O_CLOEXEC);
              ఒకవేళ (UNLIKELY (new_rootfsfd < 0))
                తిరిగి crun_make_error (err, errno, "reopen rootfs after mount on /");

              తాత్కాలిక_FAILURE_RETRY (close (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd));
              get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd = new_rootfsfd;
            }
        }
      లేకపోతే ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          తిరిగి crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", లక్ష్యం);
        }
      లేకపోతే ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->host_మూలం_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          తిరిగి crun_make_error (err, errno, "move_mount `%s` to `%s`", ఆధారం, లక్ష్యం);
        }
      లేకపోతే ఒకవేళ (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           ఆధారం, లక్ష్యం, strerror (errno));
        }
    }

  ఒకవేళ (! mounted)
    {
      ఒకవేళ (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, ఆధారం, targetfd, లక్ష్యం, flags, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
      లేకపోతే ఒకవేళ (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, ఆధారం, targetfd, లక్ష్యం, flags, దత్తాంశం, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
      లేకపోతే
        {
          పూర్ణాంకం extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, ఆధారం, targetfd, లక్ష్యం, type, flags, దత్తాంశం, extra_flags, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }

  ఒకవేళ (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, లక్ష్యం, copy_from_fd, err);
      copy_from_fd = -1;
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (rec_clear || rec_సమితి)
    {
      స్థిరరాశి bool is_dir = S_ISDIR (src_బహుళకం);
      cleanup_close పూర్ణాంకం dfd = -1;

      dfd = సురక్షిత_openat (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, లక్ష్యం, O_RDONLY | O_మార్గం | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      ఒకవేళ (UNLIKELY (dfd < 0))
        తిరిగి crun_error_wrap (err, "open mount target `/%s`", లక్ష్యం);

      ret = do_mount_setattr (true, లక్ష్యం, dfd, rec_clear, rec_సమితి, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
do_mounts (libcrun_container_t *container, స్థిరరాశి అక్షరం *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  స్థిరరాశి అక్షరం *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map నిర్మాణం libcrun_fd_map *mount_fds = ఖాళీ;
  size_t i;
  పూర్ణాంకం ret;

  mount_fds = get_వ్యక్తిగత_దత్తాంశం (container)->mount_fds;
  get_వ్యక్తిగత_దత్తాంశం (container)->mount_fds = ఖాళీ;

  కొరకు (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
పూర్ణాంకం
libcrun_container_do_bind_mount (libcrun_container_t *container, అక్షరం *mount_ఆధారం, అక్షరం *mount_destination, అక్షరం **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *లక్ష్యం = consume_slashes (mount_destination);
  cleanup_free అక్షరం *దత్తాంశం = ఖాళీ;
  గుర్తులేని దీర్ఘ flags = 0;
  గుర్తులేని దీర్ఘ extra_flags = 0;
  cleanup_close పూర్ణాంకం targetfd = -1;
  పూర్ణాంకం is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_సమితి = 0;
  స్థిరరాశి అక్షరం *rootfs = get_వ్యక్తిగత_దత్తాంశం (container)->rootfs;
  పూర్ణాంకం ret;

  ఒకవేళ ((get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd < 0) || (rootfs == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  ఒకవేళ (mount_options == ఖాళీ)
    flags = get_default_flags (container, mount_destination, &దత్తాంశం);
  లేకపోతే
    {
      size_t j;

      కొరకు (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &దత్తాంశం, &rec_clear, &rec_సమితి);
    }

  ఒకవేళ (మార్గం_is_slash_dev (mount_destination))
    get_వ్యక్తిగత_దత్తాంశం (container)->mount_dev_from_host = true;

  ఒకవేళ (mount_ఆధారం && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_ఆధారం, false, err);
      ఒకవేళ (UNLIKELY (is_dir < 0))
        తిరిగి is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_సురక్షిత_create_and_open_ref_at (is_dir, get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, లక్ష్యం, 0755, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  targetfd = ret;

  ret = do_mount (container, mount_ఆధారం, targetfd, లక్ష్యం, "bind", flags, దత్తాంశం, LABEL_MOUNT, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
పూర్ణాంకం
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, పూర్ణాంకం *notify_socket_బహిర్గతం, libcrun_error_t *err)
{
#నిర్వచితమైతే HAVE_SYSTEMD
  cleanup_close పూర్ణాంకం notify_fd = -1;
  cleanup_free అక్షరం *host_notify_socket_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *state_dir = ఖాళీ;
  అక్షరం *host_మార్గం = ఖాళీ;
  పూర్ణాంకం ret;

  ఒకవేళ (container && get_వ్యక్తిగత_దత్తాంశం (container)->host_notify_socket_మార్గం)
    {
      స్థిరరాశి అక్షరం *మాతృ_dir;

      మాతృ_dir = get_వ్యక్తిగత_దత్తాంశం (container)->host_notify_socket_మార్గం;

      ret = append_paths (&host_notify_socket_మార్గం, err, మాతృ_dir, "notify", ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      host_మార్గం = host_notify_socket_మార్గం;
    }

  *notify_socket_బహిర్గతం = -1;

  ఒకవేళ (host_మార్గం == ఖాళీ)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_మూలం, context->id, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = append_paths (&host_notify_socket_మార్గం, err, state_dir, "notify/notify", ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      host_మార్గం = host_notify_socket_మార్గం;
    }

  notify_fd = open_unix_domain_socket (host_మార్గం, 1, err);
  ఒకవేళ (UNLIKELY (notify_fd < 0))
    తిరిగి notify_fd;

  ఒకవేళ (UNLIKELY (chmod (host_మార్గం, 0777) < 0))
    తిరిగి crun_make_error (err, errno, "chmod `%s`", host_మార్గం);

#  ifdef HAVE_FGETXATTR
  ఒకవేళ (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (శూన్య) setxattr (host_మార్గం, "security.selinux", container->container_def->linux->mount_label,
                       అక్షరశ్రేణి_పొడవు (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_బహిర్గతం = get_and_reset (&notify_fd);
  తిరిగి 1;
#లేకపోతే
  (శూన్య) context;
  (శూన్య) container;
  (శూన్య) err;
  *notify_socket_బహిర్గతం = -1;
  తిరిగి 0;
#అయితే_ముగింపు
}

#నిర్వచితమైతే HAVE_SYSTEMD
స్థిర పూర్ణాంకం
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  స్థిరరాశి అక్షరం *notify_socket = container->context->notify_socket;
  cleanup_free అక్షరం *host_notify_socket_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *container_notify_socket_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *state_dir = ఖాళీ;
  uid_t container_మూలం_uid = -1;
  gid_t container_మూలం_gid = -1;
  పూర్ణాంకం notify_socket_వృక్షం_fd;

  ఒకవేళ (notify_socket == ఖాళీ)
    తిరిగి 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_మూలం : ఖాళీ),
                                     (container->context ? container->context->id : ఖాళీ), err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_paths (&container_notify_socket_మార్గం, err, "/", notify_socket, "notify", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_paths (&host_notify_socket_మార్గం, err, state_dir, "notify", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = mkdir (host_notify_socket_మార్గం, 0700);
  ఒకవేళ (ret < 0)
    తిరిగి crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_మార్గం);

  ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_మూలం_లోపల_the_userns (container->container_def, 0, 0, &container_మూలం_uid, &container_మూలం_gid);
      ఒకవేళ (container_మూలం_uid != ((uid_t) -1) && container_మూలం_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_మార్గం, container_మూలం_uid, container_మూలం_gid);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_మార్గం, container_మూలం_uid, container_మూలం_gid);
        }
    }

  notify_socket_వృక్షం_fd = syscall_open_వృక్షం (AT_FDCWD, host_notify_socket_మార్గం, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  ఒకవేళ (notify_socket_వృక్షం_fd >= 0)
    /* open_tree worked */
    get_వ్యక్తిగత_దత్తాంశం (container)->notify_socket_వృక్షం_fd = notify_socket_వృక్షం_fd;
  లేకపోతే ఒకవేళ (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  లేకపోతే ఒకవేళ (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  లేకపోతే
    /* some other error */
    తిరిగి crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_మార్గం);

  get_వ్యక్తిగత_దత్తాంశం (container)->host_notify_socket_మార్గం = host_notify_socket_మార్గం;
  get_వ్యక్తిగత_దత్తాంశం (container)->container_notify_socket_మార్గం = container_notify_socket_మార్గం;
  host_notify_socket_మార్గం = container_notify_socket_మార్గం = ఖాళీ;
  తిరిగి 0;
}
#అయితే_ముగింపు

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
స్థిర పూర్ణాంకం
do_finalize_notify_socket (libcrun_container_t *container, స్థిరరాశి అక్షరం *rootfs, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_free అక్షరం *host_notify_socket_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *container_notify_socket_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *container_notify_socket_మార్గం_dir_alloc = ఖాళీ;
  స్థిరరాశి అక్షరం *container_notify_socket_మార్గం_dir = ఖాళీ;
  cleanup_close పూర్ణాంకం notify_socket_వృక్షం_fd = -1;
  cleanup_close పూర్ణాంకం targetfd = -1;
  పూర్ణాంకం did_mount_with_move_mount = 0;

  host_notify_socket_మార్గం = get_వ్యక్తిగత_దత్తాంశం (container)->host_notify_socket_మార్గం;
  get_వ్యక్తిగత_దత్తాంశం (container)->host_notify_socket_మార్గం = ఖాళీ;

  container_notify_socket_మార్గం = get_వ్యక్తిగత_దత్తాంశం (container)->container_notify_socket_మార్గం;
  get_వ్యక్తిగత_దత్తాంశం (container)->container_notify_socket_మార్గం = ఖాళీ;

  ఒకవేళ (host_notify_socket_మార్గం == ఖాళీ || container_notify_socket_మార్గం == ఖాళీ)
    తిరిగి 0;

  ఒకవేళ (rootfs == ఖాళీ)
    rootfs = "/";

  container_notify_socket_మార్గం_dir_alloc = xstrdup (container_notify_socket_మార్గం);
  container_notify_socket_మార్గం_dir = consume_slashes (dirname (container_notify_socket_మార్గం_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_మార్గం | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "open `%s`", rootfs);

      get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd = ret;
      get_వ్యక్తిగత_దత్తాంశం (container)->rootfs = rootfs;
    }

  ret = crun_సురక్షిత_create_and_open_ref_at (true, get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs,
                                          container_notify_socket_మార్గం_dir, 0755, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  targetfd = ret;

  notify_socket_వృక్షం_fd = get_వ్యక్తిగత_దత్తాంశం (container)->notify_socket_వృక్షం_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_వ్యక్తిగత_దత్తాంశం (container)->notify_socket_వృక్షం_fd = -1;

  ఒకవేళ (notify_socket_వృక్షం_fd >= 0)
    {
      ret = fs_move_mount_గమ్యం (notify_socket_వృక్షం_fd, targetfd, ఖాళీ);
      ఒకవేళ (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_లక్ష్యం (container, container_notify_socket_మార్గం_dir, err);
          ఒకవేళ (UNLIKELY (targetfd < 0))
            తిరిగి targetfd;

          ret = do_mount (container, ఖాళీ, targetfd, container_notify_socket_మార్గం_dir, ఖాళీ,
                          MS_REC | MS_PRIVATE, ఖాళీ, LABEL_NONE, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
      లేకపోతే ఒకవేళ (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      లేకపోతే
        తిరిగి crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_వృక్షం_fd,
                                container_notify_socket_మార్గం_dir);
    }

  ఒకవేళ (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_మార్గం, targetfd, container_notify_socket_మార్గం_dir, ఖాళీ,
                      MS_BIND | MS_REC | MS_PRIVATE, ఖాళీ, LABEL_MOUNT, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
make_మాతృ_mount_వ్యక్తిగత (స్థిరరాశి అక్షరం *rootfs, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం rootfsfd = -1;
  నిర్మాణం stat prev_st;
  proc_fd_మార్గం_t proc_మార్గం;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (rootfsfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", rootfs);

  ఒకవేళ (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    తిరిగి crun_make_error (err, errno, "fstat `%s`", rootfs);

  కొరకు (;;)
    {
      నిర్మాణం stat cur_st;
      libcrun_error_t tmp_err = ఖాళీ;
      పూర్ణాంకం ret;
      cleanup_close పూర్ణాంకం parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      ఒకవేళ (ret == 0)
        తిరిగి 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_మార్గం (proc_మార్గం, rootfsfd);
      ret = mount (ఖాళీ, proc_మార్గం, ఖాళీ, MS_PRIVATE, ఖాళీ);
      ఒకవేళ (ret == 0)
        తిరిగి 0;

      parentfd = openat (rootfsfd, "..", O_మార్గం | O_CLOEXEC);
      ఒకవేళ (parentfd < 0)
        {
          పూర్ణాంకం saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          ఒకవేళ (ret != 0)
            తిరిగి crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          తిరిగి crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      ఒకవేళ (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        తిరిగి crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      ఒకవేళ (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        ఆపు;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  తిరిగి crun_make_error (err, 0, "make `%s` private", rootfs);
}

పూర్ణాంకం
libcrun_సమితి_mounts (నిర్మాణం container_entrypoint_s *entrypoint_వాదనలు, libcrun_container_t *container, స్థిరరాశి అక్షరం *rootfs, సమితి_mounts_cb_t cb, శూన్య *cb_దత్తాంశం, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  పూర్ణాంకం is_user_ns = 0;
  పూర్ణాంకం ret = 0;

  ఒకవేళ (rootfs == ఖాళీ || def->mounts == ఖాళీ)
    తిరిగి 0;

  ret = open (rootfs, O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", rootfs);

  get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd = ret;
  get_వ్యక్తిగత_దత్తాంశం (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_వాదనలు, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  ఒకవేళ (def->మూలం->readonly)
    {
      నిర్మాణం remount_s *r;
      గుర్తులేని దీర్ఘ remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      పూర్ణాంకం fd;

      fd = dup (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd);
      ఒకవేళ (UNLIKELY (fd < 0))
        తిరిగి crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, ఖాళీ, get_వ్యక్తిగత_దత్తాంశం (container)->remounts);
      get_వ్యక్తిగత_దత్తాంశం (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWUSER);
  ఒకవేళ (! is_user_ns)
    {
      is_user_ns = check_running_లోపల_user_namespace (err);
      ఒకవేళ (UNLIKELY (is_user_ns < 0))
        తిరిగి is_user_ns;
    }

  ఒకవేళ (! get_వ్యక్తిగత_దత్తాంశం (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  ఒకవేళ (cb)
    {
      ret = cb (cb_దత్తాంశం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = ఖాళీ;
      స్థిరరాశి అక్షరం *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (శూన్య) crun_సురక్షిత_ensure_directory_at (get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
umount_or_hide (స్థిరరాశి అక్షరం *లక్ష్యం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = umount2 (లక్ష్యం, MNT_DETACH);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      పూర్ణాంకం saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      ఒకవేళ (errno == EINVAL)
        {
          ret = mount (ఖాళీ, లక్ష్యం, "tmpfs", 0, "size=0k");
          ఒకవేళ (LIKELY (ret == 0))
            తిరిగి 0;
        }

      తిరిగి crun_make_error (err, saved_errno, "umount `%s`", లక్ష్యం);
    }

  తిరిగి ret;
}

స్థిర పూర్ణాంకం
move_మూలం (స్థిరరాశి అక్షరం *rootfs, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = chdir (rootfs);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = umount_or_hide ("/proc", err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "chdir to `%s`", rootfs);

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_finalize_mounts (నిర్మాణం container_entrypoint_s *entrypoint_వాదనలు, libcrun_container_t *container, స్థిరరాశి అక్షరం *rootfs,
                         సమితి_mounts_cb_t after_ఇరుసు_cb, శూన్య *cb_దత్తాంశం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_వాదనలు, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  close_and_reset (&(get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_వ్యక్తిగత_దత్తాంశం (container)->old_మూలం_fd));

  ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->needs_ఇరుసు)
    {
      get_వ్యక్తిగత_దత్తాంశం (container)->needs_ఇరుసు = false;

      ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->no_ఇరుసు)
        {
          ret = move_మూలం (rootfs, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
      లేకపోతే
        {
          ret = do_ఇరుసు (container, rootfs, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ret = do_mount (container, ఖాళీ, -1, "/", ఖాళీ,
                      get_వ్యక్తిగత_దత్తాంశం (container)->rootfs_ప్రసరణ,
                      ఖాళీ, LABEL_MOUNT, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = chdir ("/");
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "chdir to `/`");
    }

  ఒకవేళ (after_ఇరుసు_cb)
    {
      ret = after_ఇరుసు_cb (cb_దత్తాంశం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = finalize_mounts (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
maybe_open_వృక్షం_namespace (స్థిరరాశి అక్షరం *rootfs, పూర్ణాంకం *బహిర్గతం_fd, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం rootfs_fd = -1;
  పూర్ణాంకం వృక్షం_fd;

  *బహిర్గతం_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (rootfs_fd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", rootfs);

  వృక్షం_fd = syscall_open_వృక్షం (rootfs_fd, "",
                               OPEN_వృక్షం_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_ఖాళీలు_మార్గం | AT_RECURSIVE);
  ఒకవేళ (వృక్షం_fd < 0)
    {
      ఒకవేళ (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        తిరిగి 0;
      తిరిగి crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *బహిర్గతం_fd = వృక్షం_fd;
  తిరిగి 0;
}

స్థిర నిర్మాణం libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  నిర్మాణం libcrun_fd_map *mount_fds = get_వ్యక్తిగత_దత్తాంశం (container)->mount_fds;

  ఒకవేళ (mount_fds == ఖాళీ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_వ్యక్తిగత_దత్తాంశం (container)->mount_fds = mount_fds;
    }
  తిరిగి mount_fds;
}

స్థిర పూర్ణాంకం
open_mount_యొక్క_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free అక్షరం *mnt_దత్తాంశం = ఖాళీ;
  గుర్తులేని దీర్ఘ mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_సమితి = 0;
  స్థిరరాశి అక్షరం *label_type = ఖాళీ;
  స్థిరరాశి అక్షరం *label_val = ఖాళీ;
  స్థిరరాశి అక్షరం *fstype;
  పూర్ణాంకం mnt_fd;
  size_t j;

  ఒకవేళ (mount->type == ఖాళీ)
    తిరిగి -1;

  fstype = mount->type;

  కొరకు (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_దత్తాంశం,
                                            &mnt_rec_clear, &mnt_rec_సమితి);

  ఒకవేళ (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      పూర్ణాంకం extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      ఒకవేళ (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          ఒకవేళ (label_type == ఖాళీ)
            crun_error_release (err);
          లేకపోతే
            label_val = def->linux->mount_label;
        }
    }

  ఒకవేళ (strcmp (fstype, "cgroup") == 0)
    {
      పూర్ణాంకం cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
      ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->ఆధారం, label_type, label_val, mnt_దత్తాంశం);
  ఒకవేళ (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->ఆధారం, ఖాళీ, ఖాళీ, mnt_దత్తాంశం);

  తిరిగి mnt_fd;
}

#నిర్వచితం_కాకపోతే LSMT_ROOT
#  define LSMT_ROOT 0xffffffffffffffffULL
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే STATMOUNT_FS_TYPE
#  define STATMOUNT_FS_TYPE 0x00000020U
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే STATMOUNT_MNT_ROOT
#  define STATMOUNT_MNT_ROOT 0x00000040U
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే STATMOUNT_MNT_POINT
#  define STATMOUNT_MNT_POINT 0x00000080U
#అయితే_ముగింపు

/* Declared here so that the build does not depend on the UAPI headers being
   recent enough; the layout is stable.  */
నిర్మాణం mnt_id_req_s
{
  uint32_t కొలత;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

నిర్మాణం statmount_s
{
  uint32_t కొలత;
  uint32_t mnt_opts;
  uint64_t ముసుగు;
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
  అక్షరం str[];
};

స్థిర పూర్ణాంకం
syscall_listmount (నిర్మాణం mnt_id_req_s *req, uint64_t *ids, size_t nr, గుర్తులేని పూర్ణాంకం flags)
{
#నిర్వచితమైతే __NR_listmount
  తిరిగి (పూర్ణాంకం) syscall (__NR_listmount, req, ids, nr, flags);
#లేకపోతే
  (శూన్య) req;
  (శూన్య) ids;
  (శూన్య) nr;
  (శూన్య) flags;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

స్థిర పూర్ణాంకం
syscall_statmount (నిర్మాణం mnt_id_req_s *req, నిర్మాణం statmount_s *buf, size_t కొలత, గుర్తులేని పూర్ణాంకం flags)
{
#నిర్వచితమైతే __NR_statmount
  తిరిగి (పూర్ణాంకం) syscall (__NR_statmount, req, buf, కొలత, flags);
#లేకపోతే
  (శూన్య) req;
  (శూన్య) buf;
  (శూన్య) కొలత;
  (శూన్య) flags;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
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
స్థిర bool
mounts_తగిలింది_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = ఖాళీ;
  size_t i, n_ids = 0, సామర్థ్యం = 128;
  bool has_absolute_bind = false;
  uint64_t చివరి = 0;

  కొరకు (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      ఒకవేళ (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->ఆధారం != ఖాళీ && def->mounts[i]->ఆధారం[0] == '/')
        {
          has_absolute_bind = true;
          ఆపు;
        }
    }
  ఒకవేళ (! has_absolute_bind)
    తిరిగి false;

  ids = xmalloc (సామర్థ్యం * పరిమాణం (uint64_t));
  కొరకు (;;)
    {
      నిర్మాణం mnt_id_req_s req = {
        .కొలత = పరిమాణం (req),
        .mnt_id = LSMT_ROOT,
        .param = చివరి,
      };
      పూర్ణాంకం ret;

      ఒకవేళ (n_ids == సామర్థ్యం)
        {
          సామర్థ్యం *= 2;
          ids = xrealloc (ids, సామర్థ్యం * పరిమాణం (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, సామర్థ్యం - n_ids, 0);
      ఒకవేళ (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        తిరిగి true;
      ఒకవేళ (ret == 0)
        ఆపు;

      n_ids += (size_t) ret;
      చివరి = ids[n_ids - 1];
    }

  కొరకు (i = 0; i < n_ids; i++)
    {
      అక్షరం బఫరు[4096] __attribute__ ((aligned (8)));
      నిర్మాణం statmount_s *sm = (నిర్మాణం statmount_s *) బఫరు;
      నిర్మాణం mnt_id_req_s req = {
        .కొలత = పరిమాణం (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      స్థిరరాశి అక్షరం *మూలం, *బిందువు;
      size_t j;

      ఒకవేళ (syscall_statmount (&req, sm, పరిమాణం (బఫరు), 0) < 0)
        కొనసాగు;
      ఒకవేళ ((sm->ముసుగు & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        కొనసాగు;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      మూలం = sm->str + sm->mnt_root;
      ఒకవేళ (! has_ఉపసర్గ (మూలం, "mnt:["))
        కొనసాగు;

      బిందువు = sm->str + sm->mnt_point;

      కొరకు (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          స్థిరరాశి అక్షరం *ఆధారం = def->mounts[j]->ఆధారం;
          size_t ఆధారం_len;

          ఒకవేళ (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            కొనసాగు;
          ఒకవేళ (ఆధారం == ఖాళీ || ఆధారం[0] != '/')
            కొనసాగు;

          ఆధారం_len = అక్షరశ్రేణి_పొడవు (ఆధారం);
          అయితే (ఆధారం_len > 1 && ఆధారం[ఆధారం_len - 1] == '/')
            ఆధారం_len--;

          ఒకవేళ (అక్షరశ్రేణి_పోలిక (బిందువు, ఆధారం, ఆధారం_len) != 0)
            కొనసాగు;

          /* The source itself is a pinned mount namespace.  */
          ఒకవేళ (బిందువు[ఆధారం_len] == '\0')
            తిరిగి true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          ఒకవేళ (recursive && (ఆధారం_len == 1 || బిందువు[ఆధారం_len] == '/'))
            తిరిగి true;
        }
    }

  తిరిగి false;
}

స్థిర bool
can_use_open_వృక్షం_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  నిర్మాణం libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  ఒకవేళ (has_hooks || has_userns)
    తిరిగి false;

  ఒకవేళ (mounts_తగిలింది_pinned_mount_namespace (def))
    తిరిగి false;

  ఒకవేళ (container->context->no_open_వృక్షం_namespace)
    తిరిగి false;

  {
    libcrun_error_t tmp_err = ఖాళీ;
    పూర్ణాంకం లోపల_userns = check_running_లోపల_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    ఒకవేళ (లోపల_userns != 0)
      తిరిగి false;
  }

  mount_fds = get_fd_map (container);
  కొరకు (i = 0; i < def->mounts_len; i++)
    {
      ఒకవేళ (mount_fds->fds[i] < 0)
        {
          bool has_cached_లక్ష్యం = get_వ్యక్తిగత_దత్తాంశం (container)->copy_symlink_targets
                                   && i < get_వ్యక్తిగత_దత్తాంశం (container)->n_copy_symlink_targets
                                   && get_వ్యక్తిగత_దత్తాంశం (container)->copy_symlink_targets[i];
          ఒకవేళ (! has_cached_లక్ష్యం)
            తిరిగి false;
        }
    }

  తిరిగి true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
స్థిర bool
mount_inherits_mountpoint_బహుళకం (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ఒకవేళ (mount->type == ఖాళీ || strcmp (mount->type, "tmpfs") != 0)
    తిరిగి false;

  కొరకు (i = 0; i < mount->options_len; i++)
    ఒకవేళ (mount->options[i] != ఖాళీ && has_ఉపసర్గ (mount->options[i], "mode="))
      తిరిగి false;

  తిరిగి true;
}

స్థిర పూర్ణాంకం
అమరిక_mount_namespace (libcrun_container_t *container, bool no_ఇరుసు, అక్షరం **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  గుర్తులేని దీర్ఘ rootfs_ప్రసరణ = 0;
  cleanup_close పూర్ణాంకం rootfsfd = -1;
  cleanup_close పూర్ణాంకం వృక్షం_fd = -1;
  libcrun_error_t tmp_err = ఖాళీ;
  bool use_open_వృక్షం = false;
  size_t i;
  పూర్ణాంకం ret;

  ఒకవేళ (def->linux && def->linux->rootfs_ప్రసరణ)
    rootfs_ప్రసరణ = get_mount_flags (def->linux->rootfs_ప్రసరణ, 0, ఖాళీ, ఖాళీ, ఖాళీ, ఖాళీ);

  ఒకవేళ ((rootfs_ప్రసరణ & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_ప్రసరణ = MS_REC | MS_PRIVATE;

  get_వ్యక్తిగత_దత్తాంశం (container)->rootfs_ప్రసరణ = rootfs_ప్రసరణ;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  విడుదల (get_వ్యక్తిగత_దత్తాంశం (container)->host_rootfs);
  get_వ్యక్తిగత_దత్తాంశం (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  కొరకు (i = 0; i < def->mounts_len; i++)
    {
      నిర్మాణం libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close పూర్ణాంకం mnt_fd = -1;
      bool recursive_bind, nofollow;
      గుర్తులేని దీర్ఘ mnt_flags = 0;

      ఒకవేళ (mount_fds && mount_fds->fds[i] >= 0)
        కొనసాగు;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      ఒకవేళ (def->mounts[i]->ఆధారం != ఖాళీ
          && def->mounts[i]->ఆధారం[0] == '/')
        {
          cleanup_free అక్షరం *mnt_దత్తాంశం = ఖాళీ;
          గుర్తులేని దీర్ఘ mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_సమితి = 0;
          size_t j;

          కొరకు (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_దత్తాంశం,
                                                    &mnt_rec_clear, &mnt_rec_సమితి);

          ఒకవేళ (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free అక్షరం *లక్ష్యం = ఖాళీ;
              libcrun_error_t tmp_err = ఖాళీ;
              ssize_t len;

              len = సురక్షిత_readlinkat (AT_FDCWD, def->mounts[i]->ఆధారం, &లక్ష్యం, 0, &tmp_err);
              ఒకవేళ (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->copy_symlink_targets == ఖాళీ)
                {
                  get_వ్యక్తిగత_దత్తాంశం (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * పరిమాణం (అక్షరం *));
                  get_వ్యక్తిగత_దత్తాంశం (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_వ్యక్తిగత_దత్తాంశం (container)->copy_symlink_targets[i] = లక్ష్యం;
              లక్ష్యం = ఖాళీ;
            }
        }

      ఒకవేళ (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_లక్ష్యం = get_వ్యక్తిగత_దత్తాంశం (container)->copy_symlink_targets
                                   && i < get_వ్యక్తిగత_దత్తాంశం (container)->n_copy_symlink_targets
                                   && get_వ్యక్తిగత_దత్తాంశం (container)->copy_symlink_targets[i];

          ఒకవేళ (mount_fds && def->mounts[i]->ఆధారం != ఖాళీ && ! has_cached_లక్ష్యం
              && get_మార్గం_లోపల_rootfs (*rootfs, def->mounts[i]->ఆధారం) == ఖాళీ)
            {
              libcrun_error_t tmp_err = ఖాళీ;
              గుర్తులేని దీర్ఘ ప్రసరణ = MS_PRIVATE;

              ఒకవేళ (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                ప్రసరణ = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->ఆధారం,
                                                  recursive_bind, false, nofollow,
                                                  ప్రసరణ, &tmp_err);
              ఒకవేళ (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          కొనసాగు;
        }

      mnt_fd = open_mount_యొక్క_type (container, def->mounts[i], err);
      ఒకవేళ (mnt_fd < 0)
        {
          crun_error_release (err);
          కొనసాగు;
        }

      ఒకవేళ (mount_inherits_mountpoint_బహుళకం (def->mounts[i]))
        {
          cleanup_close పూర్ణాంకం destfd = -1;
          నిర్మాణం stat st;

          ఒకవేళ (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_మార్గం | O_CLOEXEC);
              ఒకవేళ (UNLIKELY (rootfsfd < 0))
                తిరిగి crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = సురక్షిత_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_మార్గం | O_CLOEXEC, 0, &tmp_err);
          ఒకవేళ (destfd < 0)
            crun_error_release (&tmp_err);
          లేకపోతే ఒకవేళ (fstat (destfd, &st) == 0)
            {
              proc_fd_మార్గం_t fd_మార్గం;
              పూర్ణాంకం procfd = get_procfd (get_వ్యక్తిగత_దత్తాంశం (container), err);
              ఒకవేళ (UNLIKELY (procfd < 0))
                తిరిగి procfd;

              get_self_fd_మార్గం (fd_మార్గం, mnt_fd);
              ret = fchmodat (procfd, fd_మార్గం, st.st_mode & 07777, 0);
              (శూన్య) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_ఖాళీలు_dir_cached (container, ఖాళీ, &tmp_err);
  ఒకవేళ (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_వ్యక్తిగత_దత్తాంశం (container)->maskdir_bind_failed = true;
    }

  ఒకవేళ (! get_వ్యక్తిగత_దత్తాంశం (container)->joined_mount_ns
      && can_use_open_వృక్షం_namespace (container))
    use_open_వృక్షం = true;

  ఒకవేళ (use_open_వృక్షం)
    {
      ret = maybe_open_వృక్షం_namespace (*rootfs, &వృక్షం_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  ఒకవేళ (వృక్షం_fd >= 0)
    preopen_needed_devs_and_notify (container);

  ఒకవేళ (వృక్షం_fd >= 0)
    {
      ret = setns (వృక్షం_fd, CLONE_NEWNS);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (ఖాళీ, "/", ఖాళీ, MS_REMOUNT | MS_BIND, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, ఖాళీ, -1, "/", ఖాళీ, MS_REC | MS_PRIVATE, ఖాళీ, LABEL_MOUNT, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = do_mount (container, ఖాళీ, -1, "/", ఖాళీ, rootfs_ప్రసరణ, ఖాళీ, LABEL_MOUNT, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      get_వ్యక్తిగత_దత్తాంశం (container)->needs_ఇరుసు = false;
      get_వ్యక్తిగత_దత్తాంశం (container)->no_ఇరుసు = no_ఇరుసు;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_వ్యక్తిగత_దత్తాంశం (container)->host_మూలం_switched = true;
      విడుదల (*rootfs);
      *rootfs = xstrdup ("/");
    }
  లేకపోతే
    {
      ఒకవేళ (! get_వ్యక్తిగత_దత్తాంశం (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, ఖాళీ, -1, "/", ఖాళీ, rootfs_ప్రసరణ, ఖాళీ, LABEL_MOUNT, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = make_మాతృ_mount_వ్యక్తిగత (*rootfs, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, ఖాళీ, MS_BIND | MS_REC | MS_PRIVATE, ఖాళీ, LABEL_MOUNT, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      get_వ్యక్తిగత_దత్తాంశం (container)->needs_ఇరుసు = true;
      get_వ్యక్తిగత_దత్తాంశం (container)->no_ఇరుసు = no_ఇరుసు;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", *rootfs);
  get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd = ret;
  get_వ్యక్తిగత_దత్తాంశం (container)->rootfs = *rootfs;

  ఒకవేళ (! container->context->force_no_cgroup)
    {
      పూర్ణాంకం cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
      ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
        తిరిగి cgroup_బహుళకం;

      ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
        {
          అక్షరం *unified_cgroup_మార్గం = ఖాళీ;
          పూర్ణాంకం procfd = get_procfd (get_వ్యక్తిగత_దత్తాంశం (container), err);
          ఒకవేళ (UNLIKELY (procfd < 0))
            తిరిగి procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_మార్గం, true, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          get_వ్యక్తిగత_దత్తాంశం (container)->unified_cgroup_మార్గం = unified_cgroup_మార్గం;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd);
      get_వ్యక్తిగత_దత్తాంశం (container)->rootfs = ఖాళీ;
      తిరిగి ret;
    }

  close_and_reset (&get_వ్యక్తిగత_దత్తాంశం (container)->rootfsfd);
  get_వ్యక్తిగత_దత్తాంశం (container)->rootfs = ఖాళీ;

  ఒకవేళ (వృక్షం_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_వ్యక్తిగత_దత్తాంశం (container), &tmp_err);
  ఒకవేళ (ret < 0)
    crun_error_release (&tmp_err);

  check_running_లోపల_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_do_ఇరుసు_మూలం (libcrun_container_t *container, bool no_ఇరుసు, అక్షరం **rootfs, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ఒకవేళ (*rootfs == ఖాళీ)
    తిరిగి 0;

  ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = అమరిక_mount_namespace (container, no_ఇరుసు, rootfs, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      ఒకవేళ (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "chdir to `/`");
        }
    }
  లేకపోతే
    {
      ret = chroot (*rootfs);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "chdir to `/`");

      విడుదల (*rootfs);
      *rootfs = xstrdup ("/");
    }

  తిరిగి 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
పూర్ణాంకం
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  నిర్మాణం stat statbuf;
  cleanup_close పూర్ణాంకం fd;
  పూర్ణాంకం ret;
  పూర్ణాంకం i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  ఒకవేళ (UNLIKELY (fd == -1))
    తిరిగి crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  కొరకు (i = 0; i <= 2; i++)
    {
      ఒకవేళ (UNLIKELY (fstat (i, &statbuf) == -1))
        తిరిగి crun_make_error (err, errno, "stat fd `%d`", i);
      ఒకవేళ (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          ఒకవేళ (UNLIKELY (dup2 (fd, i) == -1))
            తిరిగి crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
uidgidmap_helper (అక్షరం *helper, pid_t pid, స్థిరరాశి అక్షరం *map_file, libcrun_error_t *err)
{
#నిర్వచించు MAX_వాదనలు 20
  అక్షరం pid_fmt[16];
  అక్షరం *వాదనలు[MAX_వాదనలు + 1];
  అక్షరం *తదుపరి;
  cleanup_free అక్షరం *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  పూర్ణాంకం ret;

  వాదనలు[nargs++] = helper;

  ret = snprintf (pid_fmt, పరిమాణం (pid_fmt), "%d", pid);
  ఒకవేళ (UNLIKELY (ret >= (పూర్ణాంకం) పరిమాణం (pid_fmt)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

  వాదనలు[nargs++] = pid_fmt;
  తదుపరి = map_file_copy;
  అయితే (nargs < MAX_వాదనలు)
    {
      అక్షరం *p = strsep (&తదుపరి, " \n");
      ఒకవేళ (తదుపరి == ఖాళీ)
        ఆపు;
      వాదనలు[nargs++] = p;
    }
  వాదనలు[nargs++] = ఖాళీ;

  ret = నిరంతరం_process (వాదనలు, err);
  ఒకవేళ (ret < 0)
    తిరిగి ret;
  ఒకవేళ (ret > 0)
    తిరిగి crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
newgidmap (pid_t pid, స్థిరరాశి అక్షరం *map_file, libcrun_error_t *err)
{
  తిరిగి uidgidmap_helper ("newgidmap", pid, map_file, err);
}

స్థిర పూర్ణాంకం
newuidmap (pid_t pid, స్థిరరాశి అక్షరం *map_file, libcrun_error_t *err)
{
  తిరిగి uidgidmap_helper ("newuidmap", pid, map_file, err);
}

స్థిర పూర్ణాంకం
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_close పూర్ణాంకం fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి fd;

  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, "deny", 4));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_వ్యక్తిగత_దత్తాంశం (container)->deny_setgroups = true;
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_free అక్షరం *content = ఖాళీ;

  ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->deny_setgroups)
    తిరిగి 0;

  ఒకవేళ (container->container_def->annotations)
    {
      స్థిరరాశి అక్షరం *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = కనుగొను_annotation (container, "run.oci.keep_original_groups");
      ఒకవేళ (annotation)
        తిరిగి strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close పూర్ణాంకం fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    ఒకవేళ (fd < 0)
      ret = fd;
    లేకపోతే
      ret = read_all_fd (fd, "self/setgroups", &content, ఖాళీ, err);
  }
  ఒకవేళ (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      ఒకవేళ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          తిరిగి 1;
        }
      తిరిగి ret;
    }

  తిరిగి అక్షరశ్రేణి_పోలిక (content, "deny", 4) == 0 ? 0 : 1;
}

పూర్ణాంకం
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = ఖాళీ;
  size_t additional_gids_len = 0;
  పూర్ణాంకం can_do_setgroups;
  పూర్ణాంకం ret;

  ఒకవేళ (process != ఖాళీ && process->user != ఖాళీ)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  ఒకవేళ (UNLIKELY (can_do_setgroups < 0))
    తిరిగి can_do_setgroups;

  ఒకవేళ (can_do_setgroups == 0)
    {
      స్థిరరాశి అక్షరం *annotation;
      size_t i;

      annotation = కనుగొను_annotation (container, "run.oci.keep_original_groups");
      ఒకవేళ (annotation && strcmp (annotation, "0") != 0)
        తిరిగి 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      కొరకు (i = 0; i < additional_gids_len; i++)
        ఒకవేళ (additional_gids[i] != process->user->gid)
          తిరిగి crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (పూర్ణాంకం) additional_gids[i]);
      తిరిగి 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "setgroups");

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->unshare_cgroupns)
    {
      పూర్ణాంకం ret = unshare (CLONE_NEWCGROUP);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (errno != EINVAL)
            తిరిగి crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  తిరిగి 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
పూర్ణాంకం
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  ఒకవేళ (len != 1)
    తిరిగి 0;

  ఒకవేళ (mappings[0]->కొలత != 1)
    తిరిగి 0;

  ఒకవేళ (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    తిరిగి 0;

  తిరిగి 1;
}

స్థిర పూర్ణాంకం
write_id_map (libcrun_container_t *container, pid_t pid, స్థిరరాశి అక్షరం *map_name,
              పూర్ణాంకం (*idmap_helper) (pid_t, స్థిరరాశి అక్షరం *, libcrun_error_t *), స్థిరరాశి అక్షరం *helper_name,
              bool warn_on_failure, స్థిరరాశి అక్షరం *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  పూర్ణాంకం ret = 0;

  ఒకవేళ (container->host_uid)
    ret = idmap_helper (pid, map, err);
  ఒకవేళ (container->host_uid == 0 || ret < 0)
    {
      cleanup_close పూర్ణాంకం fd = -1;

      ఒకవేళ (ret < 0)
        {
          ఒకవేళ (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      ఒకవేళ (UNLIKELY (fd < 0))
        తిరిగి fd;

      ret = సురక్షిత_write (fd, map_name, map, map_len, err);
      ఒకవేళ (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free అక్షరం *single_mapping = ఖాళీ;
          crun_error_release (err);

          ఒకవేళ (! skip_if_setgroups_denied || ! get_వ్యక్తిగత_దత్తాంశం (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          ఒకవేళ (UNLIKELY (fd < 0))
            తిరిగి fd;

          ret = సురక్షిత_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  తిరిగి ret;
}

పూర్ణాంకం
libcrun_సమితి_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free అక్షరం *uid_map = ఖాళీ;
  cleanup_free అక్షరం *gid_map = ఖాళీ;
  size_t uid_map_len = 0, gid_map_len = 0;
  పూర్ణాంకం ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ఒకవేళ ((get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWUSER) == 0)
    తిరిగి 0;

  ఒకవేళ (! def || ! def->linux)
    తిరిగి 0;

  ఒకవేళ (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      uid_map_len = (size_t) ret;
      ఒకవేళ (uid_map == ఖాళీ)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }

  ఒకవేళ (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      gid_map_len = (size_t) ret;
      ఒకవేళ (gid_map == ఖాళీ)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

#నిర్వచించు CAP_గమ్యం_ముసుగు_0(x) (1L << ((x) & 31))
#నిర్వచించు CAP_గమ్యం_ముసుగు_1(x) CAP_గమ్యం_ముసుగు_0 (x - 32)

నిర్మాణం all_caps_s
{
  గుర్తులేని దీర్ఘ effective[2];
  గుర్తులేని దీర్ఘ permitted[2];
  గుర్తులేని దీర్ఘ inheritable[2];
  గుర్తులేని దీర్ఘ ambient[2];
  గుర్తులేని దీర్ఘ bounding[2];
};

స్థిర పూర్ణాంకం
has_cap_on (పూర్ణాంకం cap, దీర్ఘ గుర్తులేని *caps)
{
  ఒకవేళ (cap < 32)
    తిరిగి CAP_గమ్యం_ముసుగు_0 (cap) & caps[0];
  తిరిగి (CAP_గమ్యం_ముసుగు_1 (cap) & caps[1]);
}

స్థిర గుర్తులేని దీర్ఘ cap_చివరి_cap;

పూర్ణాంకం
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం fd = -1;
  పూర్ణాంకం ret;
  అక్షరం బఫరు[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  ఒకవేళ (fd < 0)
    తిరిగి fd;
  ret = తాత్కాలిక_FAILURE_RETRY (పఠ (fd, బఫరు, పరిమాణం (బఫరు) - 1));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  బఫరు[ret] = '\0';

  errno = 0;
  cap_చివరి_cap = strtoul (బఫరు, ఖాళీ, 10);
  ఒకవేళ (errno != 0)
    తిరిగి crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", బఫరు);
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
సమితి_required_caps (నిర్మాణం all_caps_s *caps, uid_t uid, gid_t gid, పూర్ణాంకం no_new_privs, bool no_exec, libcrun_error_t *err)
{
#నిర్వచితమైతే HAVE_CAP
  గుర్తులేని దీర్ఘ cap;
  పూర్ణాంకం ret;
  నిర్మాణం __user_cap_header_struct hdr = { LINUX_CAPABILITY_సంస్కరణ_3, 0 };
  నిర్మాణం __user_cap_data_struct దత్తాంశం[2] = { { 0 } };

  ఒకవేళ (cap_చివరి_cap == 0)
    తిరిగి crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  కొరకు (cap = 0; cap <= cap_చివరి_cap; cap++)
    ఒకవేళ (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        ఒకవేళ (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          తిరిగి crun_make_error (err, errno, "prctl drop bounding");
      }

  దత్తాంశం[0].effective = caps->effective[0];
  దత్తాంశం[1].effective = caps->effective[1];
  దత్తాంశం[0].inheritable = caps->inheritable[0];
  దత్తాంశం[1].inheritable = caps->inheritable[1];
  దత్తాంశం[0].permitted = caps->permitted[0];
  దత్తాంశం[1].permitted = caps->permitted[1];

  ret = prctl (PR_సమితి_KEEPCAPS, 1, 0, 0, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  ఒకవేళ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    తిరిగి crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, దత్తాంశం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  ఒకవేళ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    తిరిగి crun_make_error (err, errno, "prctl reset ambient");

  కొరకు (cap = 0; cap <= cap_చివరి_cap; cap++)
    ఒకవేళ (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        ఒకవేళ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          తిరిగి crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  ఒకవేళ (no_exec)
    {
      ret = prctl (PR_సమితి_KEEPCAPS, 0, 0, 0, 0);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "prctl reset KEEPCAPS");

      ఒకవేళ (uid != 0)
        {
          దత్తాంశం[0].permitted = దత్తాంశం[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          కొరకు (cap = 0; cap <= cap_చివరి_cap; cap++)
            ఒకవేళ (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_సమితి, cap, 0, 0) > 0)
              దత్తాంశం[cap / 32].permitted |= CAP_గమ్యం_ముసుగు_0 (cap);
#  endif
          దత్తాంశం[0].effective = దత్తాంశం[0].permitted;
          దత్తాంశం[1].effective = దత్తాంశం[1].permitted;

          ret = capset (&hdr, దత్తాంశం);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "capset");
        }
    }
#అయితే_ముగింపు

  ఒకవేళ (no_new_privs)
    ఒకవేళ (UNLIKELY (prctl (PR_సమితి_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      తిరిగి crun_make_error (err, errno, "prctl set no new privs");

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
read_caps (గుర్తులేని దీర్ఘ caps[2], అక్షరం **విలువలు, size_t len, libcrun_error_t *err)
{
#నిర్వచితమైతే HAVE_CAP
  size_t i;
  కొరకు (i = 0; i < len; i++)
    {
      cap_విలువ_t cap;
      ఒకవేళ (cap_from_name (విలువలు[i], &cap) < 0)
        తిరిగి crun_make_error (err, 0, "unknown capability `%s`", విలువలు[i]);

      ఒకవేళ (cap < 32)
        caps[0] |= CAP_గమ్యం_ముసుగు_0 (cap);
      లేకపోతే
        caps[1] |= CAP_గమ్యం_ముసుగు_1 (cap);
    }
#లేకపోతే
  caps[0] = 0;
  caps[1] = 0;
#అయితే_ముగింపు
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_సమితి_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ఒకవేళ (proc->selinux_label)
    తిరిగి సమితి_selinux_label (container, proc->selinux_label, now, err);

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_సమితి_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ఒకవేళ (proc->apparmor_profile)
    తిరిగి సమితి_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_సమితి_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  పూర్ణాంకం no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  నిర్మాణం all_caps_s caps = {};
  పూర్ణాంకం ret;

  ఒకవేళ (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి సమితి_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

నిర్మాణం rlimit_s
{
  స్థిరరాశి అక్షరం *name;
  పూర్ణాంకం విలువ;
};

నిర్మాణం rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
                              { "RLIMIT_CORE", RLIMIT_CORE },
                              { "RLIMIT_CPU", RLIMIT_CPU },
                              { "RLIMIT_DATA", RLIMIT_దత్తాంశం },
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
                              { "RLIMIT_STACK", RLIMIT_దొంతర },
                              { ఖాళీ, 0 } };

స్థిర పూర్ణాంకం
get_rlimit_resource (స్థిరరాశి అక్షరం *name)
{
  నిర్మాణం rlimit_s *it;
  కొరకు (it = rlimits; it->name; it++)
    ఒకవేళ (strcmp (it->name, name) == 0)
      తిరిగి it->విలువ;
  తిరిగి -1;
}

పూర్ణాంకం
libcrun_సమితి_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  కొరకు (i = 0; i < len; i++)
    {
      కొరకు (j = i + 1; j < len; j++)
        {
          ఒకవేళ (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            తిరిగి crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  కొరకు (i = 0; i < len; i++)
    {
      నిర్మాణం rlimit హద్దు;
      అక్షరం *type = new_rlimits[i]->type;
      పూర్ణాంకం resource = get_rlimit_resource (type);
      ఒకవేళ (UNLIKELY (resource < 0))
        తిరిగి crun_make_error (err, 0, "invalid rlimit `%s`", type);
      హద్దు.rlim_cur = new_rlimits[i]->soft;
      హద్దు.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (గుర్తులేని దీర్ఘ దీర్ఘ) హద్దు.rlim_cur,
                     (గుర్తులేని దీర్ఘ దీర్ఘ) హద్దు.rlim_max);
      ఒకవేళ (UNLIKELY (setrlimit (resource, &హద్దు) < 0))
        తిరిగి crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_సమితి_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  పూర్ణాంకం has_uts = get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWUTS;
  పూర్ణాంకం ret;
  ఒకవేళ (def->hostname == ఖాళీ || def->hostname[0] == '\0')
    తిరిగి 0;
  ఒకవేళ (! has_uts)
    తిరిగి crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, అక్షరశ్రేణి_పొడవు (def->hostname));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "sethostname");
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_సమితి_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  పూర్ణాంకం has_uts = get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWUTS;
  పూర్ణాంకం ret;
  ఒకవేళ (is_ఖాళీలు_string (def->domainname))
    తిరిగి 0;
  ఒకవేళ (! has_uts)
    తిరిగి crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, అక్షరశ్రేణి_పొడవు (def->domainname));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "setdomainname");
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_సమితి_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close పూర్ణాంకం fd = -1;
  పూర్ణాంకం ret;
  అక్షరం oom_బఫరు[16];
  ఒకవేళ (def->process == ఖాళీ || ! def->process->oom_score_adj_present)
    తిరిగి 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_బఫరు, పరిమాణం (oom_బఫరు), "%i", def->process->oom_score_adj);
  ఒకవేళ (UNLIKELY (ret >= (పూర్ణాంకం) పరిమాణం (oom_బఫరు)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  ఒకవేళ (fd < 0)
    తిరిగి fd;
  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, oom_బఫరు, అక్షరశ్రేణి_పొడవు (oom_బఫరు)));
  ఒకవేళ (ret < 0)
    తిరిగి crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  తిరిగి 0;
}

స్థిరరాశి అక్షరం *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  ఖాళీ
};

స్థిర పూర్ణాంకం
validate_sysctl (స్థిరరాశి అక్షరం *original_కీలకం, స్థిరరాశి అక్షరం *original_విలువ, స్థిరరాశి అక్షరం *name, గుర్తులేని దీర్ఘ namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  ఒకవేళ (name[0] == '/' || మార్గం_has_dot_dot_component (name))
    తిరిగి crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_కీలకం);

  ఒకవేళ (has_ఉపసర్గ (name, "fs/mqueue/"))
    {
      ఒకవేళ (namespaces_created & CLONE_NEWIPC)
        తిరిగి 0;

      namespace = "IPC";
      వెళ్ళు fail;
    }

  ఒకవేళ (has_ఉపసర్గ (name, "kernel/"))
    {
      size_t i;

      కొరకు (i = 0; sysctlRequiringIPC[i]; i++)
        ఒకవేళ (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            ఒకవేళ (namespaces_created & CLONE_NEWIPC)
              తిరిగి 0;

            namespace = "IPC";
            వెళ్ళు fail;
          }

      ఒకవేళ (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          ఒకవేళ (! is_ఖాళీలు_string (def->domainname) && (strcmp (original_విలువ, def->domainname) != 0))
            తిరిగి crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_కీలకం);

          ఒకవేళ (namespaces_created & CLONE_NEWUTS)
            తిరిగి 0;

          namespace = "UTS";
          వెళ్ళు fail;
        }

      ఒకవేళ (strcmp (name, "kernel/hostname") == 0)
        తిరిగి crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_కీలకం);
    }
  ఒకవేళ (has_ఉపసర్గ (name, "net/"))
    {
      ఒకవేళ (namespaces_created & CLONE_NEWNET)
        తిరిగి 0;

      namespace = "network";
      వెళ్ళు fail;
    }

  తిరిగి crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_కీలకం);

fail:
  తిరిగి crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_కీలకం, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
స్థిర అక్షరం *
sysctl_error_reason (స్థిరరాశి అక్షరం *name, పూర్ణాంకం namespaces_created, పూర్ణాంకం errno_)
{

  ఒకవేళ (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    తిరిగి xstrdup ("are all the IDs mapped in the user namespace?");

  తిరిగి ఖాళీ;
}

పూర్ణాంకం
libcrun_సమితి_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close పూర్ణాంకం dirfd = -1;
  గుర్తులేని దీర్ఘ namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ఒకవేళ (def->linux == ఖాళీ || def->linux->sysctl == ఖాళీ || def->linux->sysctl->len == 0)
    తిరిగి 0;

  కొరకు (i = 0; i < def->linux->namespaces_len; i++)
    {
      పూర్ణాంకం విలువ;

      విలువ = libcrun_కనుగొను_namespace (def->linux->namespaces[i]->type);
      ఒకవేళ (UNLIKELY (విలువ < 0))
        తిరిగి crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= విలువ;
    }

  get_వ్యక్తిగత_దత్తాంశం (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_మార్గం, err);
  ఒకవేళ (UNLIKELY (dirfd < 0))
    తిరిగి dirfd;

  కొరకు (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free అక్షరం *name = ఖాళీ;
      cleanup_close పూర్ణాంకం fd = -1;
      పూర్ణాంకం ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      ఒకవేళ (def->linux->sysctl->కీలకాలు[i] == ఖాళీ || def->linux->sysctl->విలువలు[i] == ఖాళీ)
        తిరిగి crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_కీలకం_గమ్యం_proc_మార్గం (def->linux->sysctl->కీలకాలు[i]);

      ret = validate_sysctl (def->linux->sysctl->కీలకాలు[i], def->linux->sysctl->విలువలు[i], name, namespaces_created, def, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (fd < 0))
        తిరిగి crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, def->linux->sysctl->విలువలు[i], అక్షరశ్రేణి_పొడవు (def->linux->sysctl->విలువలు[i])));
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          పూర్ణాంకం saved_errno = errno;
          cleanup_free అక్షరం *reason = ఖాళీ;

          reason = sysctl_error_reason (def->linux->sysctl->కీలకాలు[i], namespaces_created, saved_errno);
          తిరిగి crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
open_terminal (అక్షరం **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం fd = -1;
  uid_t uid = 0;
  పూర్ణాంకం ret;

  ఒకవేళ (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి fd;

  ret = libcrun_సమితి_stdio (fd, *pty, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  తిరిగి ret;
}

అక్షరం *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  తిరిగి get_వ్యక్తిగత_దత్తాంశం (container)->external_descriptors;
}

పూర్ణాంకం
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *buf = ఖాళీ;
  json_gen_ctx *gen = ఖాళీ;
  size_t buf_len;
  పూర్ణాంకం ret;
  పూర్ణాంకం i;

  ఒకవేళ (! json_gen_init (&gen, ఖాళీ))
    తిరిగి crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  ఒకవేళ (UNLIKELY (ret != json_gen_స్థితి_ok))
    వెళ్ళు json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  కొరకు (i = 0; i < 3; i++)
    {
      proc_fd_మార్గం_t fd_మార్గం;
      అక్షరం link_మార్గం[PATH_MAX];

      get_proc_self_fd_మార్గం (fd_మార్గం, i);

      ret = readlink (fd_మార్గం, link_మార్గం, PATH_MAX - 1);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          ఒకవేళ (errno == ENOENT)
            {
              strcpy (link_మార్గం, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          లేకపోతే
            {
              json_gen_free (gen);
              తిరిగి crun_make_error (err, errno, "readlink `%s`", fd_మార్గం);
            }
        }
      link_మార్గం[ret] = 0;

      ret = json_gen_string (gen, link_మార్గం, ret);
      ఒకవేళ (UNLIKELY (ret != json_gen_స్థితి_ok))
        వెళ్ళు json_error;
    }

  ret = json_gen_array_close (gen);
  ఒకవేళ (UNLIKELY (ret != json_gen_స్థితి_ok))
    వెళ్ళు json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  ఒకవేళ (UNLIKELY (ret != json_gen_స్థితి_ok))
    వెళ్ళు json_error;

  ఒకవేళ (buf)
    {
      అక్షరం *b = xmalloc (buf_len + 1);
      జ్ఞాపకం_నకలు (b, buf, buf_len);
      b[buf_len] = '\0';
      get_వ్యక్తిగత_దత్తాంశం (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  తిరిగి 0;

json_error:
  ఒకవేళ (gen)
    json_gen_free (gen);
  తిరిగి json_gen_error_గమ్యం_crun_error (ret, err);
}

పూర్ణాంకం
libcrun_సమితి_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free అక్షరం *pty = ఖాళీ;
  cleanup_close పూర్ణాంకం fd = -1;
  పూర్ణాంకం ret;

  ఒకవేళ (def->process == ఖాళీ || ! def->process->terminal)
    తిరిగి 0;

  fd = open_terminal (&pty, def->process, err);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి fd;

  ఒకవేళ (def->process->console_కొలత)
    {
      ret = libcrun_terminal_అమరిక_కొలత (0, def->process->console_కొలత->ఎత్తు, def->process->console_కొలత->వెడల్పు, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", ఖాళీ, MS_BIND, ఖాళీ, LABEL_MOUNT, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      ret = unlink ("/dev/console");
      ఒకవేళ (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          ఒకవేళ (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", ఖాళీ, MS_BIND, ఖాళీ, LABEL_MOUNT, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;

              తిరిగి get_and_reset (&fd);
            }
          తిరిగి crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  తిరిగి get_and_reset (&fd);
}

స్థిర bool
read_error_from_sync_socket (పూర్ణాంకం sync_socket_fd, పూర్ణాంకం *error, అక్షరం **str)
{
  cleanup_free అక్షరం *b = ఖాళీ;
  size_t కొలత;
  పూర్ణాంకం సంకేతం_2 = 0;
  పూర్ణాంకం ret;

  ఒకవేళ (*error == 0)
    {
      ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_socket_fd, &సంకేతం_2, పరిమాణం (సంకేతం_2)));
      ఒకవేళ (UNLIKELY (ret != (పూర్ణాంకం) పరిమాణం (సంకేతం_2)))
        తిరిగి false;
      *error = సంకేతం_2;
    }

  ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_socket_fd, &కొలత, పరిమాణం (కొలత)));
  ఒకవేళ (UNLIKELY (ret != (పూర్ణాంకం) పరిమాణం (కొలత)))
    తిరిగి false;

  ఒకవేళ (కొలత == 0)
    తిరిగి false;

  ఒకవేళ (కొలత > 1024)
    కొలత = 1024;

  b = xmalloc (కొలత + 1);
  ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_socket_fd, b, కొలత));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి false;

  b[ret] = '\0';

  *str = b;
  b = ఖాళీ;
  తిరిగి true;
}

స్థిర bool
send_error_గమ్యం_sync_socket (పూర్ణాంకం sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t కొలత;
  పూర్ణాంకం ret;
  పూర్ణాంకం సంకేతం_2;
  అక్షరం *సందేశం;

  ఒకవేళ (err == ఖాళీ || *err == ఖాళీ)
    తిరిగి false;

  సంకేతం_2 = crun_error_get_errno (err);

  ఒకవేళ (has_fd)
    {
      /* dummy terminal fd.  */
      ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket_fd, "1", 1));
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి false;
    }

  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket_fd, &సంకేతం_2, పరిమాణం (సంకేతం_2)));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి false;

  సందేశం = (*err)->సందేశం;
  కొలత = అక్షరశ్రేణి_పొడవు (సందేశం) + 1;
  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket_fd, &కొలత, పరిమాణం (కొలత)));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి false;

  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket_fd, సందేశం, కొలత));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి false;

  తిరిగి true;
}

స్థిర అంతఃవరుస పూర్ణాంకం
send_success_గమ్యం_sync_socket (పూర్ణాంకం sync_socket, libcrun_error_t *err)
{
  స్థిరరాశి పూర్ణాంకం success = 0;
  పూర్ణాంకం ret;

  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket, &success, పరిమాణం (success)));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "write to sync socket");

  తిరిగి 0;
}

స్థిర __attribute__ ((noreturn)) శూన్య
send_error_గమ్యం_sync_socket_and_die (పూర్ణాంకం sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  అక్షరం *సందేశం;

  ఒకవేళ (err == ఖాళీ || *err == ఖాళీ)
    సురక్షిత_exit (EXIT_FAILURE);

  ఒకవేళ (! send_error_గమ్యం_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      సందేశం = (*err)->సందేశం;
      libcrun_fail_with_error (errno, "%s", సందేశం);
    }
  సురక్షిత_exit (EXIT_FAILURE);
}

స్థిర పూర్ణాంకం
expect_success_from_sync_socket (పూర్ణాంకం sync_fd, libcrun_error_t *err)
{
  cleanup_free అక్షరం *err_str = ఖాళీ;
  పూర్ణాంకం res = 1;
  పూర్ణాంకం ret;

  ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_fd, &res, పరిమాణం (res)));
  ఒకవేళ (UNLIKELY (ret != పరిమాణం (res)))
    తిరిగి crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  ఒకవేళ (res == 0)
    తిరిగి 0;

  ఒకవేళ (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      ఒకవేళ (! is_ఖాళీలు_string (err_str))
        తిరిగి crun_make_error (err, res, "%s", err_str);
    }

  తిరిగి crun_make_error (err, res, "read from sync socket");
}

స్థిర పూర్ణాంకం
join_namespaces (runtime_spec_schema_config_schema *def, పూర్ణాంకం *namespaces_గమ్యం_join, పూర్ణాంకం n_namespaces_గమ్యం_join,
                 పూర్ణాంకం *namespaces_గమ్యం_join_సూచీ, bool ignore_join_errors, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  పూర్ణాంకం i;

  కొరకు (i = 0; i < n_namespaces_గమ్యం_join; i++)
    {
      cleanup_free అక్షరం *cwd = ఖాళీ;
      పూర్ణాంకం orig_సూచీ = namespaces_గమ్యం_join_సూచీ[i];
      పూర్ణాంకం విలువ;

      ఒకవేళ (namespaces_గమ్యం_join[i] < 0)
        కొనసాగు;

      /* Skip the user namespace.  */
      విలువ = libcrun_కనుగొను_namespace (def->linux->namespaces[orig_సూచీ]->type);
      ఒకవేళ (విలువ == CLONE_NEWUSER)
        కొనసాగు;

      ఒకవేళ (విలువ == CLONE_NEWNS)
        {
          cwd = getcwd (ఖాళీ, 0);
          ఒకవేళ (UNLIKELY (cwd == ఖాళీ))
            తిరిగి crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_సూచీ]->type, def->linux->namespaces[orig_సూచీ]->మార్గం);
      ret = setns (namespaces_గమ్యం_join[i], విలువ);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (ignore_join_errors)
            కొనసాగు;
          తిరిగి crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_సూచీ]->మార్గం);
        }

      close_and_reset (&namespaces_గమ్యం_join[i]);

      ఒకవేళ (విలువ == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  తిరిగి 0;
}

#నిర్వచించు MAX_NAMESPACES 10

నిర్మాణం init_స్థితి_s
{
  /* fd to the namespace to join.  */
  పూర్ణాంకం fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  పూర్ణాంకం సూచీ[MAX_NAMESPACES];
  /* CLONE_* value.  */
  పూర్ణాంకం విలువ[MAX_NAMESPACES];
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
  పూర్ణాంకం userns_సూచీ;
  /* def->linux->namespaces userns.  */
  పూర్ణాంకం userns_సూచీ_origin;

  /* All namespaces created/joined by the container.  */
  పూర్ణాంకం all_namespaces;

  /* What namespaces are still missing to be created.  */
  పూర్ణాంకం namespaces_గమ్యం_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  పూర్ణాంకం idx_pidns_గమ్యం_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  పూర్ణాంకం idx_timens_గమ్యం_join_immediately;
};

శూన్య
cleanup_free_init_statusp (నిర్మాణం init_స్థితి_s *ns)
{
  size_t i;

  ఒకవేళ (ns == ఖాళీ)
    తిరిగి;

  కొరకు (i = 0; i < ns->fd_len; i++)
    తాత్కాలిక_FAILURE_RETRY (close (ns->fd[i]));
}

స్థిర పూర్ణాంకం
configure_init_స్థితి (నిర్మాణం init_స్థితి_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  కొరకు (i = 0; i < MAX_NAMESPACES + 1; i++)
    ns->fd[i] = -1;

  ns->fd_len = 0;
  ns->all_namespaces = 0;
  ns->namespaces_గమ్యం_unshare = 0;
  ns->join_pidns = false;
  ns->join_ipcns = false;
  ns->must_fork = false;
  ns->can_delay_pidns_fork = false;
  ns->delayed_userns_create = false;
  ns->userns_సూచీ = -1;
  ns->userns_సూచీ_origin = -1;
  ns->idx_pidns_గమ్యం_join_immediately = -1;
  ns->idx_timens_గమ్యం_join_immediately = -1;

  కొరకు (i = 0; i < def->linux->namespaces_len; i++)
    {
      పూర్ణాంకం విలువ = libcrun_కనుగొను_namespace (def->linux->namespaces[i]->type);
      ఒకవేళ (UNLIKELY (విలువ < 0))
        తిరిగి crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ఒకవేళ (ns->all_namespaces & విలువ)
        తిరిగి crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= విలువ;

      ఒకవేళ (def->linux->namespaces[i]->మార్గం == ఖాళీ)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_గమ్యం_unshare |= విలువ;
        }
      లేకపోతే
        {
          పూర్ణాంకం fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->మార్గం);

          ఒకవేళ (ns->fd_len >= MAX_NAMESPACES)
            తిరిగి crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->మార్గం, O_RDONLY | O_CLOEXEC);
          ఒకవేళ (UNLIKELY (fd < 0))
            తిరిగి crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->మార్గం);

          ఒకవేళ (విలువ == CLONE_NEWUSER)
            {
              ns->userns_సూచీ = ns->fd_len;
              ns->userns_సూచీ_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->సూచీ[ns->fd_len] = i;
          ns->విలువ[ns->fd_len] = విలువ;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  ఒకవేళ (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_గమ్యం_unshare |= CLONE_NEWUSER;
    }

  తిరిగి 0;
}

/* Detect if root is available in the container.  */
స్థిర bool
మూలం_mapped_లోపల_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  కొరకు (i = 0; i < len; i++)
    ఒకవేళ (mappings[i]->container_id == 0)
      తిరిగి true;

  తిరిగి false;
}

స్థిర నిర్మాణం libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  నిర్మాణం libcrun_fd_map *dev_fds = get_వ్యక్తిగత_దత్తాంశం (container)->dev_fds;

  ఒకవేళ (dev_fds == ఖాళీ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_వ్యక్తిగత_దత్తాంశం (container)->dev_fds = dev_fds;
    }
  తిరిగి dev_fds;
}

#నిర్వచించు అంకె_NEEDED_DEVS (పరిమాణం (needed_devs) / పరిమాణం (needed_devs[0]) - 1)

స్థిర నిర్మాణం libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  నిర్మాణం libcrun_fd_map *fds = get_వ్యక్తిగత_దత్తాంశం (container)->needed_devs_fds;

  ఒకవేళ (fds == ఖాళీ)
    {
      fds = make_libcrun_fd_map (అంకె_NEEDED_DEVS);
      get_వ్యక్తిగత_దత్తాంశం (container)->needed_devs_fds = fds;
    }
  తిరిగి fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  ఒకవేళ (recursive)
    *recursive = false;

  ఒకవేళ (src_nofollow != ఖాళీ)
    *src_nofollow = false;

  కొరకు (i = 0; i < mnt->options_len; i++)
    {
      ఒకవేళ (mnt->options[i] == ఖాళీ)
        కొనసాగు;

      ఒకవేళ (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ఒకవేళ (src_nofollow == ఖాళీ || *src_nofollow)
            ఆపు;
        }
      ఒకవేళ (strcmp (mnt->options[i], "rbind") == 0)
        {
          ఒకవేళ (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ఒకవేళ (src_nofollow == ఖాళీ || *src_nofollow)
            ఆపు;
        }
      ఒకవేళ (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  తిరిగి ret;
}

స్థిర అక్షరం *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  కొరకు (i = 0; i < mnt->options_len; i++)
    {
      ఒకవేళ (mnt->options[i] == ఖాళీ)
        కొనసాగు;

      ఒకవేళ (has_ఉపసర్గ (mnt->options[i], "idmap"))
        {
          *recursive = false;
          తిరిగి mnt->options[i];
        }
      ఒకవేళ (has_ఉపసర్గ (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          తిరిగి mnt->options[i];
        }
    }
  తిరిగి ఖాళీ;
}

స్థిర పూర్ణాంకం
open_mount_type (స్థిరరాశి అక్షరం *type, పూర్ణాంకం *బహిర్గతం_fd, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం fsopen_fd = -1;
  cleanup_close పూర్ణాంకం newfs_fd = -1;
  పూర్ణాంకం ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ఒకవేళ (UNLIKELY (fsopen_fd < 0))
    తిరిగి crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, ఖాళీ, ఖాళీ, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  ఒకవేళ (UNLIKELY (newfs_fd < 0))
    తిరిగి crun_make_error (err, errno, "fsmount `%s`", type);

  *బహిర్గతం_fd = get_and_reset (&newfs_fd);
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, పూర్ణాంకం *బహిర్గతం_fd, bool *has_mappings_బహిర్గతం, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  నిర్మాణం mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close పూర్ణాంకం fd = -1;
  స్థిరరాశి అక్షరం *idmap_option;
  bool recursive = false;
  స్థిరరాశి అక్షరం *options = ఖాళీ;
  bool has_mappings;
  పూర్ణాంకం ret;
  అక్షరం *extra_సందేశం = "";
  bool nofollow = false;

  *బహిర్గతం_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != ఖాళీ);
  ఒకవేళ (has_mappings_బహిర్గతం)
    *has_mappings_బహిర్గతం = has_mappings;
  ఒకవేళ (! has_mappings)
    తిరిగి 0;

  ఒకవేళ ((mnt->uid_mappings == ఖాళీ) != (mnt->gid_mappings == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  ఒకవేళ (idmap_option)
    {
      options = strchr (idmap_option, '=');
      ఒకవేళ (options)
        {
          /* Skip the '=' itself.  */
          options++;
          ఒకవేళ (options[0] == '\0')
            options = ఖాళీ;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  ఒకవేళ (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి fd;

  ఒకవేళ (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_వృక్షం (AT_FDCWD, mnt->ఆధారం, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      ఒకవేళ (UNLIKELY (newfs_fd < 0))
        తిరిగి crun_make_error (err, errno, "open `%s`", mnt->ఆధారం);
    }
  లేకపోతే
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_ఖాళీలు_మార్గం | (recursive ? AT_RECURSIVE : 0), &attr);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno == EINVAL)
        {
          extra_సందేశం = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      తిరిగి crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_సందేశం);
    }

  *బహిర్గతం_fd = get_and_reset (&newfs_fd);
  తిరిగి 0;
}

స్థిర uid_t
get_id_లోపల_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  ఒకవేళ (! def || ! def->linux)
    వెళ్ళు నిష్క్రమించు;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  ఒకవేళ (! mappings)
    వెళ్ళు నిష్క్రమించు;

  కొరకు (i = 0; i < len; i++)
    {
      ఒకవేళ (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->కొలత)
        తిరిగి id - mappings[i]->container_id + mappings[i]->host_id;
    }
నిష్క్రమించు:
  తిరిగి is_uid ? get_అధికప్రవాహం_uid () : get_అధికప్రవాహం_gid ();
}

స్థిర పూర్ణాంకం
precreate_device (libcrun_container_t *container, పూర్ణాంకం devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  అక్షరం name[64];
  mode_t type;
  dev_t dev;
  పూర్ణాంకం ret;

  ret = snprintf (name, పరిమాణం (name), "%zu", i);
  ఒకవేళ (UNLIKELY (ret >= (పూర్ణాంకం) పరిమాణం (name)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_బహుళకం | type, dev);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "mknod `%s`", device->మార్గం);

  uid = get_id_లోపల_user_namespace (device->uid, true, def);
  gid = get_id_లోపల_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "chown `%s`", device->మార్గం);

  తిరిగి get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

స్థిర పూర్ణాంకం
send_mounts (పూర్ణాంకం sync_socket_host, నిర్మాణం libcrun_fd_map *fds, size_t how_many, size_t మొత్తం, libcrun_error_t *err)
{
  size_t i;
  పూర్ణాంకం ret;

  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket_host, &how_many, పరిమాణం (how_many)));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "write to sync socket");

  కొరకు (i = 0; i < మొత్తం; i++)
    {
      ఒకవేళ (fds->fds[i] >= 0)
        {
          ret = send_fd_గమ్యం_socket_with_payload (sync_socket_host, fds->fds[i], (అక్షరం *) &i, పరిమాణం (i), err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, పూర్ణాంకం sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map నిర్మాణం libcrun_fd_map *mount_fds = ఖాళీ;
  bool has_userns = (get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free అక్షరం *rootfs = ఖాళీ;
  size_t how_many = 0;
  size_t i;
  పూర్ణాంకం ret;

  ఒకవేళ (def->mounts_len == 0)
    తిరిగి 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  ఒకవేళ (def->మూలం && def->మూలం->మార్గం)
    rootfs = realpath (def->మూలం->మార్గం, ఖాళీ);

  ఒకవేళ (! has_userns)
    {
      పూర్ణాంకం is_లోపల_userns = check_running_లోపల_user_namespace (err);
      ఒకవేళ (UNLIKELY (is_లోపల_userns < 0))
        తిరిగి is_లోపల_userns;

      has_userns = is_లోపల_userns > 0;
    }

  కొరకు (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      పూర్ణాంకం mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      ఒకవేళ (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_మార్గం_లోపల_rootfs (rootfs, def->mounts[i]->ఆధారం) == ఖాళీ)
        {
          గుర్తులేని దీర్ఘ ప్రసరణ = 0;

          ఒకవేళ (def->linux && def->linux->rootfs_ప్రసరణ)
            ప్రసరణ = get_mount_flags (def->linux->rootfs_ప్రసరణ, 0, ఖాళీ, ఖాళీ, ఖాళీ, ఖాళీ) & ALL_PROPAGATIONS_NO_REC;
          ఒకవేళ (ప్రసరణ == 0)
            ప్రసరణ = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->ఆధారం, recursive, false, nofollow, ప్రసరణ, err);
          ఒకవేళ (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      ఒకవేళ (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  తిరిగి send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

స్థిర పూర్ణాంకం
prepare_and_send_dev_mounts (libcrun_container_t *container, పూర్ణాంకం sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map నిర్మాణం libcrun_fd_map *dev_fds = ఖాళీ;
  bool has_userns = (get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close పూర్ణాంకం current_mountns = -1;
  cleanup_free అక్షరం *state_dir = ఖాళీ;
  cleanup_free అక్షరం *devs_మార్గం = ఖాళీ;
  cleanup_close పూర్ణాంకం devs_mountfd = -1;
  cleanup_close పూర్ణాంకం targetfd = -1;
  స్థిరరాశి అక్షరం *context_type = ఖాళీ;
  స్థిరరాశి అక్షరం *label = ఖాళీ;
  size_t how_many = 0;
  size_t i;
  పూర్ణాంకం ret;
  // To track whether the namespace has been changed.
  bool ns_మారింది = false;

  ఒకవేళ (def->linux == ఖాళీ || def->linux->devices_len == 0)
    తిరిగి 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  ఒకవేళ (! has_userns || is_ఖాళీలు_string (container->context->id) || geteuid () > 0)
    తిరిగి send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_మూలం : ఖాళీ),
                                     (container->context ? container->context->id : ఖాళీ), err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_paths (&devs_మార్గం, err, state_dir, "devs", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = mkdir (devs_మార్గం, 0700);
  ఒకవేళ (UNLIKELY (ret < 0 && errno != EEXIST))
    తిరిగి crun_make_error (err, errno, "mkdir `%s`", devs_మార్గం);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  ఒకవేళ (UNLIKELY (current_mountns < 0))
    తిరిగి current_mountns;

  ret = unshare (CLONE_NEWNS);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_మారింది = true;

  ret = mount (ఖాళీ, "/", ఖాళీ, MS_REC | MS_PRIVATE, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      వెళ్ళు restore_mountns;
    }

  ఒకవేళ (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      ఒకవేళ (UNLIKELY (context_type == ఖాళీ))
        {
          ret = -1;
          వెళ్ళు restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", ఖాళీ, context_type, label, ఖాళీ);
  ఒకవేళ (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      వెళ్ళు restore_mountns;
    }

  targetfd = open (devs_మార్గం, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_మార్గం);
      వెళ్ళు restore_mountns;
    }

  ret = fs_move_mount_గమ్యం (devs_mountfd, targetfd, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_మార్గం);
      వెళ్ళు restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_మార్గం);
      వెళ్ళు restore_mountns;
    }

  కొరకు (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          కొనసాగు;
        }

      dev_fds->fds[i] = ret;

      ఒకవేళ (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  ఒకవేళ (ns_మారింది && current_mountns >= 0)
    {
      పూర్ణాంకం setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      ఒకవేళ (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          తిరిగి crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  తిరిగి ret;
}

స్థిర పూర్ణాంకం
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, పూర్ణాంకం sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map నిర్మాణం libcrun_fd_map *fds = ఖాళీ;
  bool has_userns = (get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (అంకె_NEEDED_DEVS);

  ఒకవేళ (! has_userns || geteuid () > 0)
    తిరిగి send_mounts (sync_socket_host, fds, 0, అంకె_NEEDED_DEVS, err);

  కొరకు (i = 0; i < అంకె_NEEDED_DEVS && needed_devs[i].మార్గం; i++)
    {
      పూర్ణాంకం fd = syscall_open_వృక్షం (AT_FDCWD, needed_devs[i].మార్గం,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      ఒకవేళ (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  తిరిగి send_mounts (sync_socket_host, fds, how_many, అంకె_NEEDED_DEVS, err);
}

స్థిర పూర్ణాంకం
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, పూర్ణాంకం sync_socket_host, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
receive_mounts (నిర్మాణం libcrun_fd_map *fds, పూర్ణాంకం sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  పూర్ణాంకం ret;

  ఒకవేళ (fds->nfds == 0)
    తిరిగి 0;

  ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_socket_container, &how_many, పరిమాణం (how_many)));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "read from sync socket");

  కొరకు (i = 0; i < how_many; i++)
    {
      size_t సూచీ;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (అక్షరం *) &సూచీ, పరిమాణం (సూచీ), err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      ఒకవేళ (సూచీ >= fds->nfds)
        తిరిగి crun_make_error (err, 0, "invalid mount data received");

      ఒకవేళ (fds->fds[సూచీ] >= 0)
        తాత్కాలిక_FAILURE_RETRY (close (fds->fds[సూచీ]));

      fds->fds[సూచీ] = ret;
    }

  తిరిగి 0;
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
స్థిర పూర్ణాంకం
సమితి_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ఒకవేళ (! container->host_has_cap_sys_ptrace)
    తిరిగి 0;

  ret = prctl (PR_సమితి_DUMPABLE, 0, 0, 0, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
సమితి_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  పూర్ణాంకం ret;

  ఒకవేళ (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool మూలం_mapped = false;

      ఒకవేళ (def->linux->uid_mappings_len != 0)
        {
          మూలం_mapped = మూలం_mapped_లోపల_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          ఒకవేళ (! మూలం_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      ఒకవేళ (def->linux->gid_mappings_len != 0)
        {
          మూలం_mapped = మూలం_mapped_లోపల_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          ఒకవేళ (! మూలం_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  ఒకవేళ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    తిరిగి crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "setresgid to `%d`", gid);

  తిరిగి 0;
}

స్థిర bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  తిరిగి mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, ఖాళీ, ఖాళీ);
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
స్థిర పూర్ణాంకం
open_proc_mounts_లోపల_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  నిర్మాణం libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close పూర్ణాంకం socket_మాతృ = -1;
  cleanup_close పూర్ణాంకం socket_బిడ్డ = -1;
  cleanup_close పూర్ణాంకం మూలం_fd = -1;
  cleanup_close పూర్ణాంకం cwd_fd = -1;
  size_t i, missing = 0;
  పూర్ణాంకం saved_errno;
  పూర్ణాంకం sockets[2];
  pid_t pid;
  పూర్ణాంకం ret;

  ఒకవేళ (def->మూలం == ఖాళీ || def->మూలం->మార్గం == ఖాళీ)
    తిరిగి 0;

  కొరకు (i = 0; i < def->mounts_len; i++)
    ఒకవేళ (is_proc_mount (def->mounts[i]))
      missing++;
  ఒకవేళ (missing == 0)
    తిరిగి 1;

  మూలం_fd = open ("/", O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (మూలం_fd < 0))
    తిరిగి crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (cwd_fd < 0))
    తిరిగి crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "socketpair");
  socket_మాతృ = sockets[0];
  socket_బిడ్డ = sockets[1];

  ret = chroot (def->మూలం->మార్గం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి 0;

  pid = fork ();
  saved_errno = errno;
  ఒకవేళ (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      ఒకవేళ (get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags & CLONE_NEWUSER)
        (శూన్య) unshare (CLONE_NEWNS);

      కొరకు (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close పూర్ణాంకం fd = -1;

          ఒకవేళ (! is_proc_mount (def->mounts[i]))
            కొనసాగు;

          fd = open_mount_యొక్క_type (container, def->mounts[i], err);
          ఒకవేళ (fd < 0)
            సురక్షిత_exit (EXIT_FAILURE);

          ret = send_fd_గమ్యం_socket_with_payload (socket_బిడ్డ, fd, (అక్షరం *) &i, పరిమాణం (i), err);
          ఒకవేళ (UNLIKELY (ret < 0))
            సురక్షిత_exit (EXIT_FAILURE);
        }
      సురక్షిత_exit (EXIT_SUCCESS);
    }

  ఒకవేళ (UNLIKELY (fchdir (మూలం_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    తిరిగి crun_make_error (err, errno, "restore the root directory");

  ఒకవేళ (UNLIKELY (pid < 0))
    తిరిగి crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_బిడ్డ);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  అయితే (missing > 0)
    {
      libcrun_error_t tmp_err = ఖాళీ;

      ret = receive_fd_from_socket_with_payload (socket_మాతృ, (అక్షరం *) &i, పరిమాణం (i), &tmp_err);
      ఒకవేళ (ret < 0)
        {
          crun_error_release (&tmp_err);
          ఆపు;
        }
      ఒకవేళ (i >= mount_fds->nfds)
        {
          తాత్కాలిక_FAILURE_RETRY (close (ret));
          ఆపు;
        }

      ఒకవేళ (mount_fds->fds[i] >= 0)
        తాత్కాలిక_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, ఖాళీ, 0);

  తిరిగి missing == 0 ? 1 : 0;
}

స్థిర పూర్ణాంకం
init_container (libcrun_container_t *container, పూర్ణాంకం sync_socket_container, నిర్మాణం init_స్థితి_s *init_స్థితి,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  నిర్మాణం libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close పూర్ణాంకం timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  పూర్ణాంకం ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = సమితి_init_non_dumpable (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (init_స్థితి->idx_pidns_గమ్యం_join_immediately >= 0 || init_స్థితి->idx_timens_గమ్యం_join_immediately >= 0)
    {
      pid_t new_pid;

      ఒకవేళ (init_స్థితి->idx_pidns_గమ్యం_join_immediately >= 0)
        {
          ret = setns (init_స్థితి->fd[init_స్థితి->idx_pidns_గమ్యం_join_immediately], CLONE_NEWPID);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_స్థితి->fd[init_స్థితి->idx_pidns_గమ్యం_join_immediately]);
        }

      ఒకవేళ (init_స్థితి->idx_timens_గమ్యం_join_immediately >= 0)
        {
          ret = setns (init_స్థితి->fd[init_స్థితి->idx_timens_గమ్యం_join_immediately], CLONE_NEWTIME);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_స్థితి->fd[init_స్థితి->idx_timens_గమ్యం_join_immediately]);
        }

      ఒకవేళ (init_స్థితి->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_లోపల_pidns (container, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          delayed_pidns_fork = ret > 0;
        }

      ఒకవేళ (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_గమ్యం_sync_socket (sync_socket_container, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket_container, &new_pid, పరిమాణం (new_pid)));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "write to sync socket");
        }
      లేకపోతే
        {
          new_pid = fork ();
          ఒకవేళ (UNLIKELY (new_pid < 0))
            తిరిగి crun_make_error (err, errno, "fork");
        }

      ఒకవేళ (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_గమ్యం_sync_socket (sync_socket_container, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket_container, &new_pid, పరిమాణం (new_pid)));
          ఒకవేళ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          సురక్షిత_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = libcrun_సమితి_oom (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (init_స్థితి->fd_len > 0)
    {
      ret = join_namespaces (def, init_స్థితి->fd, init_స్థితి->fd_len, init_స్థితి->సూచీ, true, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  ఒకవేళ ((init_స్థితి->all_namespaces & CLONE_NEWUSER) && (init_స్థితి->join_pidns || init_స్థితి->join_ipcns))
    {
      కొరకు (i = 0; i < def->mounts_len; i++)
        {
          పూర్ణాంకం fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          ఒకవేళ (def->mounts[i]->type == ఖాళీ)
            కొనసాగు;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          ఒకవేళ (init_స్థితి->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->ఆధారం, ఖాళీ, ఖాళీ, ఖాళీ);
          ఒకవేళ (init_స్థితి->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->ఆధారం, ఖాళీ, ఖాళీ, ఖాళీ);

          ఒకవేళ (fd >= 0)
            {
              ఒకవేళ (mount_fds->fds[i] >= 0)
                తాత్కాలిక_FAILURE_RETRY (close (mount_fds->fds[i]));
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
  ఒకవేళ (def->linux->సమయం_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      ఒకవేళ (UNLIKELY (timens_offsets_fd < 0))
        తిరిగి timens_offsets_fd;
    }

  ఒకవేళ (init_స్థితి->all_namespaces & CLONE_NEWUSER)
    {
      ఒకవేళ (init_స్థితి->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "unshare user namespace");

          init_స్థితి->namespaces_గమ్యం_unshare &= ~CLONE_NEWUSER;

          ret = send_success_గమ్యం_sync_socket (sync_socket_container, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ఒకవేళ (init_స్థితి->userns_సూచీ < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
      లేకపోతే
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_స్థితి->fd[init_స్థితి->userns_సూచీ], CLONE_NEWUSER);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_స్థితి->userns_సూచీ_origin]->మార్గం);
        }

      ret = సమితి_id_init (container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = సమితి_init_non_dumpable (container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = join_namespaces (def, init_స్థితి->fd, init_స్థితి->fd_len, init_స్థితి->సూచీ, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (init_స్థితి->namespaces_గమ్యం_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_స్థితి->namespaces_గమ్యం_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "unshare");
    }

  ఒకవేళ (def->linux->సమయం_offsets)
    {
      అక్షరం fmt_బఫరు[128];
      పూర్ణాంకం fd = timens_offsets_fd;

      ఒకవేళ (def->linux->సమయం_offsets->boottime)
        {
          ret = snprintf (fmt_బఫరు, పరిమాణం (fmt_బఫరు), "boottime %" PRIi64 " %" PRIu32, def->linux->సమయం_offsets->boottime->secs, def->linux->సమయం_offsets->boottime->nanosecs);
          ఒకవేళ (UNLIKELY (ret >= (పూర్ణాంకం) పరిమాణం (fmt_బఫరు)))
            తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (దీర్ఘ దీర్ఘ పూర్ణాంకం) def->linux->సమయం_offsets->boottime->secs, def->linux->సమయం_offsets->boottime->nanosecs);
          ret = లిఖ (fd, fmt_బఫరు, అక్షరశ్రేణి_పొడవు (fmt_బఫరు));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      ఒకవేళ (def->linux->సమయం_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (దీర్ఘ దీర్ఘ పూర్ణాంకం) def->linux->సమయం_offsets->monotonic->secs, def->linux->సమయం_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_బఫరు, పరిమాణం (fmt_బఫరు), "monotonic %" PRIi64 " %" PRIu32, def->linux->సమయం_offsets->monotonic->secs, def->linux->సమయం_offsets->monotonic->nanosecs);
          ఒకవేళ (UNLIKELY (ret >= (పూర్ణాంకం) పరిమాణం (fmt_బఫరు)))
            తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

          ret = లిఖ (fd, fmt_బఫరు, అక్షరశ్రేణి_పొడవు (fmt_బఫరు));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  ఒకవేళ (init_స్థితి->must_fork)
    {
      ఒకవేళ (init_స్థితి->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_లోపల_pidns (container, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          delayed_pidns_fork = ret > 0;
        }

      ఒకవేళ (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          ఒకవేళ (UNLIKELY (pid_container < 0))
            తిరిగి crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      ఒకవేళ (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_గమ్యం_sync_socket (sync_socket_container, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket_container, &pid_container, పరిమాణం (pid_container)));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "write to sync socket");

          ఒకవేళ (pid_container)
            సురక్షిత_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  get_వ్యక్తిగత_దత్తాంశం (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_గమ్యం_sync_socket (sync_socket_container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  తిరిగి get_వ్యక్తిగత_దత్తాంశం (container)->delayed_pidns_fork;
}

పూర్ణాంకం
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం client_fd = -1;
  cleanup_close పూర్ణాంకం pidfd = -1;
  స్థిరరాశి అక్షరం *v;

  v = కనుగొను_annotation (container, "run.oci.pidfd_receiver");
  ఒకవేళ (v == ఖాళీ)
    తిరిగి 0;

  pidfd = syscall_pidfd_open (pid, 0);
  ఒకవేళ (UNLIKELY (pidfd < 0))
    తిరిగి crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  ఒకవేళ (UNLIKELY (client_fd < 0))
    తిరిగి client_fd;

  తిరిగి send_fd_గమ్యం_socket (client_fd, pidfd, err);
}

స్థిర bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  ఒకవేళ (process == ఖాళీ || process->exec_cpu_affinity == ఖాళీ)
    తిరిగి false;
  తిరిగి (! is_ఖాళీలు_string (process->exec_cpu_affinity->initial))
         || (! is_ఖాళీలు_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_నిరంతరం_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, శూన్య *వాదనలు,
                             పూర్ణాంకం *sync_socket_బహిర్గతం, నిర్మాణం libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) నిర్మాణం init_స్థితి_s init_స్థితి;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close పూర్ణాంకం sync_socket_container = -1;
  అక్షరం *notify_socket_env = ఖాళీ;
  cleanup_close పూర్ణాంకం sync_socket_host = -1;
  __attribute__ ((వాడని)) cleanup_close పూర్ణాంకం restore_pidns = -1;
  పూర్ణాంకం మొదటి_clone_వాదనలు = 0;
  పూర్ణాంకం sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  పూర్ణాంకం ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_స్థితి (&init_స్థితి, container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  get_వ్యక్తిగత_దత్తాంశం (container)->unshare_flags = init_స్థితి.all_namespaces;
  get_వ్యక్తిగత_దత్తాంశం (container)->joined_mount_ns
      = (init_స్థితి.all_namespaces & CLONE_NEWNS)
        && ! (init_స్థితి.namespaces_గమ్యం_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_స్థితి.all_namespaces &= ~CLONE_NEWCGROUP;
  get_వ్యక్తిగత_దత్తాంశం (container)->unshare_cgroupns = init_స్థితి.namespaces_గమ్యం_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#నిర్వచితమైతే HAVE_SYSTEMD
  ఒకవేళ (def->మూలం)
    {
      ret = do_notify_socket (container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
#అయితే_ముగింపు

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  ఒకవేళ (def->process)
    {
      ret = libcrun_సమితి_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = libcrun_సమితి_oom (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_container_notify_handler (వాదనలు, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->మూలం ? container->container_def->మూలం->మార్గం : ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_స్థితి.delayed_userns_create
      = (init_స్థితి.all_namespaces & CLONE_NEWUSER) && init_స్థితి.userns_సూచీ < 0 && init_స్థితి.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_స్థితి.all_namespaces & CLONE_NEWUSER) && init_స్థితి.userns_సూచీ < 0;
  కొరకు (i = 0; i < init_స్థితి.fd_len; i++)
    {
      ఎంపిక (init_స్థితి.విలువ[i])
        {
        స్థితి CLONE_NEWIPC:
          ఒకవేళ (init_స్థితి.all_namespaces & CLONE_NEWUSER)
            init_స్థితి.join_ipcns = true;
          ఆపు;

        స్థితి CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_స్థితి.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          ఒకవేళ (! new_userns)
            init_స్థితి.must_fork = true;
          లేకపోతే
            {
              init_స్థితి.join_pidns = true;
              init_స్థితి.idx_pidns_గమ్యం_join_immediately = i;
              init_స్థితి.namespaces_గమ్యం_unshare &= ~CLONE_NEWPID;
            }
          ఆపు;

        స్థితి CLONE_NEWTIME:
          ఒకవేళ (! new_userns)
            init_స్థితి.must_fork = true;
          లేకపోతే
            {
              init_స్థితి.idx_timens_గమ్యం_join_immediately = i;
              init_స్థితి.namespaces_గమ్యం_unshare &= ~CLONE_NEWTIME;
            }
          ఆపు;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  ఒకవేళ ((init_స్థితి.namespaces_గమ్యం_unshare & CLONE_NEWUSER) && init_స్థితి.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      మొదటి_clone_వాదనలు = CLONE_NEWUSER;
    }
  లేకపోతే ఒకవేళ ((init_స్థితి.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      మొదటి_clone_వాదనలు = init_స్థితి.namespaces_గమ్యం_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_స్థితి.namespaces_గమ్యం_unshare &= ~మొదటి_clone_వాదనలు;

  /* Check if there are still namespaces that require a fork().  */
  ఒకవేళ (init_స్థితి.namespaces_గమ్యం_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      మొదటి_clone_వాదనలు &= ~CLONE_NEWPID;
      init_స్థితి.namespaces_గమ్యం_unshare |= CLONE_NEWPID;
      init_స్థితి.must_fork = true;
      init_స్థితి.can_delay_pidns_fork = false;
    }

  pid = -1;
  ఒకవేళ (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      నిర్మాణం clone3_వాదనలు clone3_వాదనలు_2;
      memset (&clone3_వాదనలు_2, 0, పరిమాణం (clone3_వాదనలు_2));
      clone3_వాదనలు_2.exit_signal = SIGCHLD;
      clone3_వాదనలు_2.flags = మొదటి_clone_వాదనలు;

      clone3_వాదనలు_2.flags |= CLONE_INTO_CGROUP;
      clone3_వాదనలు_2.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_వాదనలు_2);
      ఒకవేళ (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  ఒకవేళ (pid < 0)
    {
      pid = syscall_clone (మొదటి_clone_వాదనలు | SIGCHLD, ఖాళీ);
      ఒకవేళ (UNLIKELY (pid < 0))
        తిరిగి crun_make_error (err, errno, "clone");
    }

  ఒకవేళ (pid)
    {
      __attribute__ ((వాడని)) cleanup_pid pid_t pid_గమ్యం_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = close_and_reset (&sync_socket_container);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_వ్యక్తిగత_దత్తాంశం (container)->notify_socket_వృక్షం_fd));

      ఒకవేళ (init_స్థితి.idx_pidns_గమ్యం_join_immediately >= 0 || init_స్థితి.idx_timens_గమ్యం_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_socket_host, &new_pid, పరిమాణం (new_pid)));
          ఒకవేళ (UNLIKELY (ret != పరిమాణం (new_pid)))
            తిరిగి crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          ఒకవేళ (new_pid == 0)
            get_వ్యక్తిగత_దత్తాంశం (container)->delayed_pidns_fork = true;
          లేకపోతే
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, ఖాళీ, 0);

              pid_గమ్యం_clean = pid = new_pid;
            }

          ret = send_success_గమ్యం_sync_socket (sync_socket_host, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ఒకవేళ (init_స్థితి.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ఒకవేళ ((init_స్థితి.all_namespaces & CLONE_NEWUSER) && init_స్థితి.userns_సూచీ < 0)
        {
          ret = libcrun_సమితి_usernamespace (container, pid, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          ret = send_success_గమ్యం_sync_socket (sync_socket_host, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ఒకవేళ (init_స్థితి.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_socket_host, &grandchild, పరిమాణం (grandchild)));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_గమ్యం_sync_socket (sync_socket_host, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          ఒకవేళ (grandchild == 0)
            get_వ్యక్తిగత_దత్తాంశం (container)->delayed_pidns_fork = true;
          లేకపోతే
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, ఖాళీ, 0);

              pid_గమ్యం_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      *sync_socket_బహిర్గతం = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      ఒకవేళ (! get_వ్యక్తిగత_దత్తాంశం (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      pid_గమ్యం_clean = 0;
      తిరిగి pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  ఒకవేళ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_స్థితి, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    send_error_గమ్యం_sync_socket_and_die (sync_socket_container, false, err);
  లేకపోతే
    {
      ret = send_success_గమ్యం_sync_socket (sync_socket_container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->సందేశం);
    }

  /* Jump into the specified entrypoint.  */
  ఒకవేళ (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (వాదనలు, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  ఒకవేళ (*err)
    libcrun_fail_with_error ((*err)->స్థితి_2, "%s", (*err)->సందేశం);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  ఒకవేళ (ret == 0)
    సురక్షిత_exit (EXIT_SUCCESS);

  సురక్షిత_exit (EXIT_FAILURE);
}

స్థిర పూర్ణాంకం
join_process_మాతృ_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t బిడ్డ_pid, పూర్ణాంకం sync_socket_fd,
                            libcrun_container_స్థితి_t *స్థితి_2,
                            bool need_move_గమ్యం_cgroup, స్థిరరాశి అక్షరం *sub_cgroup,
                            పూర్ణాంకం *terminal_fd, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  అక్షరం res;
  pid_t pid;
  cleanup_close పూర్ణాంకం sync_fd = sync_socket_fd;

  ఒకవేళ (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_fd, &res, 1));
  ఒకవేళ (UNLIKELY (ret != 1))
    తిరిగి crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  ఒకవేళ (res != '0')
    తిరిగి crun_make_error (err, 0, "fail startup");

  ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_fd, &pid, పరిమాణం (pid)));
  ఒకవేళ (UNLIKELY (ret != పరిమాణం (pid)))
    తిరిగి crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (బిడ్డ_pid, ఖాళీ, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "waitpid for exec child pid");

  ఒకవేళ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_సమితి_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_ముసుగు (pid, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (need_move_గమ్యం_cgroup)
    {
      cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి = libcrun_cgroup_make_స్థితి (స్థితి_2);
      cleanup_free అక్షరం *final_cgroup = ఖాళీ;

      ఒకవేళ (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, స్థితి_2->cgroup_మార్గం, sub_cgroup, ఖాళీ);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_స్థితి, final_cgroup ?: స్థితి_2->cgroup_మార్గం, pid, స్థితి_2->pid, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_సమితి_scheduler (pid, process, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_సమితి_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_సమితి_io_ప్రాధాన్యత (pid, process, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_fd, &ret, పరిమాణం (ret)));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "write to sync socket");

  ఒకవేళ (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          పూర్ణాంకం err_సంకేతం = 0;
          cleanup_free అక్షరం *err_str = ఖాళీ;

          ఒకవేళ (read_error_from_sync_socket (sync_fd, &err_సంకేతం, &err_str))
            {
              ఒకవేళ (! is_ఖాళీలు_string (err_str))
                {
                  crun_error_release (err);
                  తిరిగి crun_make_error (err, err_సంకేతం, "%s", err_str);
                }
            }

          తిరిగి crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  తిరిగి pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
స్థిర పూర్ణాంకం
try_setns_with_pidfd (pid_t pid_గమ్యం_join, libcrun_container_t *container, libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close పూర్ణాంకం pidfd_pid_గమ్యం_join = -1;
  పూర్ణాంకం all_flags = 0;
  size_t i;
  పూర్ణాంకం ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  ఒకవేళ (def->linux && def->linux->namespaces)
    {
      కొరకు (i = 0; i < def->linux->namespaces_len; i++)
        ఒకవేళ (! is_ఖాళీలు_string (def->linux->namespaces[i]->మార్గం))
          తిరిగి 0;
    }

  pidfd_pid_గమ్యం_join = syscall_pidfd_open (pid_గమ్యం_join, 0);
  ఒకవేళ (UNLIKELY (pidfd_pid_గమ్యం_join < 0))
    తిరిగి 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  ఒకవేళ (ret == 0)
    తిరిగి crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  కొరకు (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].విలువ;

  ret = setns (pidfd_pid_గమ్యం_join, all_flags);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి 0;

  తిరిగి 1;
}

స్థిర పూర్ణాంకం
join_process_namespaces (libcrun_container_t *container, pid_t pid_గమ్యం_join, libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  పూర్ణాంకం fds_joined[MAX_NAMESPACES] = {
    0,
  };
  పూర్ణాంకం fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  పూర్ణాంకం ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_గమ్యం_join, container, స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  /* Nothing left to do if the namespaces were joined.  */
  ఒకవేళ (LIKELY (ret > 0))
    తిరిగి 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  ఒకవేళ (def->linux->namespaces_len >= MAX_NAMESPACES)
    తిరిగి crun_make_error (err, 0, "invalid configuration");

  కొరకు (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free అక్షరం *ns_మార్గం = ఖాళీ;

      xasprintf (&ns_మార్గం, "%d/ns/%s", pid_గమ్యం_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_మార్గం, O_RDONLY, err);
      ఒకవేళ (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          ఒకవేళ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              కొనసాగు;
            }

          ret = fds[i];
          వెళ్ళు నిష్క్రమించు;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు నిష్క్రమించు;
  ఒకవేళ (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      వెళ్ళు నిష్క్రమించు;
    }

  కొరకు (i = 0; namespaces[i].ns_file; i++)
    {
      ఒకవేళ (namespaces[i].విలువ == CLONE_NEWUSER)
        కొనసాగు;

      ret = setns (fds[i], 0);
      ఒకవేళ (ret == 0)
        fds_joined[i] = 1;
    }
  కొరకు (i = 0; namespaces[i].ns_file; i++)
    {
      ఒకవేళ (fds_joined[i])
        కొనసాగు;
      ret = setns (fds[i], 0);
      ఒకవేళ (ret == 0)
        fds_joined[i] = 1;
    }
  కొరకు (i = 0; namespaces[i].ns_file; i++)
    {
      ఒకవేళ (fds_joined[i])
        కొనసాగు;
      ret = setns (fds[i], 0);
      ఒకవేళ (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool దొరికింది = false;

          కొరకు (j = 0; j < def->linux->namespaces_len; j++)
            {
              ఒకవేళ (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  దొరికింది = true;
                  ఆపు;
                }
            }
          ఒకవేళ (! దొరికింది)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              కొనసాగు;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          వెళ్ళు నిష్క్రమించు;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

నిష్క్రమించు:
  కొరకు (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  తిరిగి ret;
}

పూర్ణాంకం
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_గమ్యం_join,
                      libcrun_container_స్థితి_t *స్థితి_2,
                      స్థిరరాశి అక్షరం *sub_cgroup,
                      పూర్ణాంకం detach,
                      runtime_spec_schema_config_schema_process *process,
                      పూర్ణాంకం *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  పూర్ణాంకం ret;
  పూర్ణాంకం sync_socket_fd[2];
  cleanup_close పూర్ణాంకం cgroup_dirfd = -1;
  cleanup_close పూర్ణాంకం sync_fd = -1;
  నిర్మాణం clone3_వాదనలు clone3_వాదనలు_2;
  bool need_move_గమ్యం_cgroup;

  ఒకవేళ (! detach)
    {
      ret = prctl (PR_సమితి_బిడ్డ_SUBREAPER, 1, 0, 0, 0);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "set child subreaper");
    }

  ఒకవేళ (మార్గం_has_dot_dot_component (sub_cgroup))
    తిరిగి crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  ఒకవేళ (sub_cgroup && ! is_ఖాళీలు_string (స్థితి_2->cgroup_మార్గం))
    {
      cleanup_free అక్షరం *final_cgroup = ఖాళీ;

      ret = append_paths (&final_cgroup, err, స్థితి_2->cgroup_మార్గం, sub_cgroup, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి = ఖాళీ;

    cgroup_స్థితి = libcrun_cgroup_make_స్థితి (స్థితి_2);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    ఒకవేళ (get_force_cgroup_v1_annotation (container) == ఖాళీ)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_స్థితి, sub_cgroup, err);
        ఒకవేళ (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_వాదనలు_2, 0, పరిమాణం (clone3_వాదనలు_2));
  clone3_వాదనలు_2.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  ఒకవేళ (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_గమ్యం_cgroup = true;
  లేకపోతే
    {
      need_move_గమ్యం_cgroup = false;
      clone3_వాదనలు_2.flags |= CLONE_INTO_CGROUP;
      clone3_వాదనలు_2.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_వాదనలు_2);

  ఒకవేళ (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_సమితి_scheduler (pid, process, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  /* On errors, fall back to fork().  */
  ఒకవేళ (pid < 0)
    {
      need_move_గమ్యం_cgroup = true;

      pid = fork ();
      ఒకవేళ (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          వెళ్ళు నిష్క్రమించు;
        }
    }

  ఒకవేళ (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      తిరిగి join_process_మాతృ_helper (context, container,
                                         process, pid, sync_fd,
                                         స్థితి_2, need_move_గమ్యం_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_గమ్యం_join, స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->స్థితి_2, "%s", (*err)->సందేశం);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ఒకవేళ (UNLIKELY (pid < 0))
    {
      పూర్ణాంకం saved_errno = errno;
      తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  ఒకవేళ (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_fd, "0", 1));
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          సురక్షిత_exit (EXIT_FAILURE);
        }

      ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_fd, &pid, పరిమాణం (pid)));
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          సురక్షిత_exit (EXIT_FAILURE);
        }

      సురక్షిత_exit (EXIT_SUCCESS);
    }
  లేకపోతే
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free అక్షరం *pty = ఖాళీ;
      పూర్ణాంకం r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_fd, &r, పరిమాణం (r)));
      ఒకవేళ (UNLIKELY (ret != పరిమాణం (r) || r < 0))
        సురక్షిత_exit (EXIT_FAILURE);

      ret = setsid ();
      ఒకవేళ (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_గమ్యం_sync_socket_and_die (sync_fd, true, err);
        }

      ఒకవేళ (terminal_fd)
        {
          cleanup_close పూర్ణాంకం ptmx_fd = -1;

          ret = సమితి_id_init (container, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            send_error_గమ్యం_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          ఒకవేళ (UNLIKELY (ptmx_fd < 0))
            send_error_గమ్యం_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_గమ్యం_socket (sync_fd, ptmx_fd, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            send_error_గమ్యం_sync_socket_and_die (sync_fd, true, err);
        }
    }

  తిరిగి 0;

నిష్క్రమించు:
  ఒకవేళ (sync_socket_fd[0] >= 0)
    తాత్కాలిక_FAILURE_RETRY (close (sync_socket_fd[0]));
  ఒకవేళ (sync_socket_fd[1] >= 0)
    తాత్కాలిక_FAILURE_RETRY (close (sync_socket_fd[1]));
  తిరిగి ret;
}

పూర్ణాంకం
libcrun_linux_container_update (libcrun_container_స్థితి_t *స్థితి_2, స్థిరరాశి అక్షరం *state_మూలం, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి = ఖాళీ;

  cgroup_స్థితి = libcrun_cgroup_make_స్థితి (స్థితి_2);

  తిరిగి libcrun_update_cgroup_resources (cgroup_స్థితి, state_మూలం, resources, err);
}

స్థిర పూర్ణాంకం
libcrun_container_pause_unpause_linux (libcrun_container_స్థితి_t *స్థితి_2, స్థిరరాశి bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి = ఖాళీ;

  cgroup_స్థితి = libcrun_cgroup_make_స్థితి (స్థితి_2);

  తిరిగి libcrun_cgroup_pause_unpause (cgroup_స్థితి, pause, err);
}

పూర్ణాంకం
libcrun_container_pause_linux (libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err)
{
  తిరిగి libcrun_container_pause_unpause_linux (స్థితి_2, true, err);
}

పూర్ణాంకం
libcrun_container_unpause_linux (libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err)
{
  తిరిగి libcrun_container_pause_unpause_linux (స్థితి_2, false, err);
}

పూర్ణాంకం
libcrun_సమితి_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  గుర్తులేని దీర్ఘ persona = 0;
  పూర్ణాంకం ret;

  ఒకవేళ (strcmp (p->domain, "LINUX") == 0)
    persona = చొప్పున_LINUX;
  లేకపోతే ఒకవేళ (strcmp (p->domain, "LINUX32") == 0)
    persona = చొప్పున_LINUX32;
  లేకపోతే
    తిరిగి crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, 0, "personality to `%s`", p->domain);

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_configure_జాలం (libcrun_container_t *container, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  size_t i;
  bool configure_జాలం = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close పూర్ణాంకం sockfd = -1;

  కొరకు (i = 0; i < def->linux->namespaces_len; i++)
    {
      పూర్ణాంకం విలువ = libcrun_కనుగొను_namespace (def->linux->namespaces[i]->type);
      ఒకవేళ (UNLIKELY (విలువ < 0))
        తిరిగి crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ఒకవేళ (విలువ == CLONE_NEWNET && def->linux->namespaces[i]->మార్గం == ఖాళీ)
        {
          configure_జాలం = true;
          ఆపు;
        }
    }

  ఒకవేళ (! configure_జాలం)
    తిరిగి 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  ఒకవేళ (LIKELY (sockfd >= 0))
    {
      నిర్మాణం ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_పైకి | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  లేకపోతే
    {
      నిర్మాణం nlmsghdr *hdr_recv;
      అక్షరం buf[పరిమాణం (నిర్మాణం nlmsghdr) + పరిమాణం (నిర్మాణం nlmsgerr)];
      నిర్మాణం sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      నిర్మాణం sockaddr_nl addr_గమ్యం = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      నిర్మాణం nlmsghdr hdr = {
        .nlmsg_len = పరిమాణం (నిర్మాణం nlmsghdr) + పరిమాణం (నిర్మాణం ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      నిర్మాణం ifinfomsg సందేశం = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_సూచీ = 1,
        .ifi_flags = IFF_పైకి,
        .ifi_change = IFF_పైకి,
      };

      *((నిర్మాణం nlmsghdr *) buf) = hdr;
      *((నిర్మాణం ifinfomsg *) (buf + పరిమాణం (నిర్మాణం nlmsghdr))) = సందేశం;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      ఒకవేళ (UNLIKELY (sockfd < 0))
        తిరిగి crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (నిర్మాణం sockaddr *) &addr, పరిమాణం (addr));
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, పరిమాణం (buf), 0, (నిర్మాణం sockaddr *) &addr_గమ్యం, పరిమాణం (addr_గమ్యం));
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, పరిమాణం (buf), 0, ఖాళీ, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (నిర్మాణం nlmsghdr *) buf;

      ఒకవేళ (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          నిర్మాణం nlmsgerr *err_దత్తాంశం = (నిర్మాణం nlmsgerr *) NLMSG_దత్తాంశం (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          ఒకవేళ (err_దత్తాంశం->error < 0)
            తిరిగి crun_make_error (err, -err_దత్తాంశం->error, "netlink error while configuring network");
        }
    }

  తిరిగి 0;
}

/* Protection for attacks like CVE-2019-5736.  */
పూర్ణాంకం ensure_cloned_ద్విమాన ();
__attribute__ ((constructor)) స్థిర శూన్య
libcrun_rexec (శూన్య)
{
  ఒకవేళ (ensure_cloned_ద్విమాన () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      సురక్షిత_exit (EXIT_FAILURE);
    }
}

పూర్ణాంకం
libcrun_container_checkpoint_linux (libcrun_container_స్థితి_t *స్థితి_2, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  తిరిగి libcrun_container_checkpoint_linux_criu (స్థితి_2, container, cr_options, err);
}

పూర్ణాంకం
libcrun_container_restore_linux (libcrun_container_స్థితి_t *స్థితి_2, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  ret = libcrun_container_restore_linux_criu (స్థితి_2, container, cr_options, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
స్థిర పూర్ణాంకం
libcrun_kill_linux_no_pidfd (libcrun_container_స్థితి_t *స్థితి_2, bool check_pid, పూర్ణాంకం సంకేతం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  ఒకవేళ (check_pid)
    {
      ret = libcrun_check_pid_valid (స్థితి_2, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      /* The pid is not valid anymore, return an error.  */
      ఒకవేళ (ret == 0)
        {
          errno = ESRCH;
          తిరిగి crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (స్థితి_2->pid, సంకేతం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "kill container");
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_kill_linux (libcrun_container_స్థితి_t *స్థితి_2, పూర్ణాంకం సంకేతం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_close పూర్ణాంకం pidfd = -1;

  pidfd = syscall_pidfd_open (స్థితి_2->pid, 0);
  ఒకవేళ (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      ఒకవేళ (errno == ENOSYS)
        తిరిగి libcrun_kill_linux_no_pidfd (స్థితి_2, true, సంకేతం, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      ఒకవేళ (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        తిరిగి crun_make_error (err, ESRCH, "process not running");
      తిరిగి crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* The pid is not valid anymore, return an error.  */
  ఒకవేళ (ret == 0)
    {
      errno = ESRCH;
      తిరిగి crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, సంకేతం, ఖాళీ, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      ఒకవేళ (errno == ENOSYS)
        తిరిగి libcrun_kill_linux_no_pidfd (స్థితి_2, false, సంకేతం, err);
      తిరిగి crun_make_error (err, errno, "send signal to pidfd");
    }

  తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *
libcrun_get_intelrdt_name (స్థిరరాశి అక్షరం *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  ఒకవేళ (def == ఖాళీ || def->linux == ఖాళీ || def->linux->intel_rdt == ఖాళీ || def->linux->intel_rdt->clos_id == ఖాళీ)
    {
      ఒకవేళ (explicit)
        *explicit = false;
      తిరిగి ctr_name;
    }

  ఒకవేళ (explicit)
    *explicit = true;
  తిరిగి def->linux->intel_rdt->clos_id;
}

పూర్ణాంకం
libcrun_apply_intelrdt (స్థిరరాశి అక్షరం *ctr_name, libcrun_container_t *container, pid_t pid, పూర్ణాంకం actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ఖాళీ;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  స్థిరరాశి అక్షరం *name;
  పూర్ణాంకం ret;

  ఒకవేళ (container)
    def = container->container_def;

  ఒకవేళ (def == ఖాళీ || def->linux == ఖాళీ || def->linux->intel_rdt == ఖాళీ)
    తిరిగి 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  ఒకవేళ (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_సమూహం (name, ctr_name, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            వెళ్ళు fail;

          has_monitoring = true;
        }
    }

  ఒకవేళ (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు fail;
    }

  ఒకవేళ (actions & LIBCRUN_INTELRDT_MOVE)
    {
      స్థిరరాశి అక్షరం *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : ఖాళీ;
      ret = resctl_move_task_గమ్యం (name, monitoring_name, pid, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు fail;
    }

  తిరిగి 0;

fail:
  ఒకవేళ (has_monitoring)
    {
      libcrun_error_t tmp_err = ఖాళీ;
      పూర్ణాంకం tmp_ret;

      tmp_ret = resctl_destroy_monitoring_సమూహం (name, ctr_name, &tmp_err);
      ఒకవేళ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  ఒకవేళ (created)
    {
      libcrun_error_t tmp_err = ఖాళీ;
      పూర్ణాంకం tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      ఒకవేళ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  తిరిగి ret;
}

పూర్ణాంకం
libcrun_destroy_intelrdt (స్థిరరాశి అక్షరం *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  స్థిరరాశి అక్షరం *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  ఒకవేళ (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      పూర్ణాంకం ret;

      ret = resctl_destroy_monitoring_సమూహం (clos_id, container_id, err);
      ఒకవేళ (ret < 0)
        తిరిగి ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  ఒకవేళ (explicit)
    తిరిగి 0;

  తిరిగి resctl_destroy (clos_id, err);
}

పూర్ణాంకం
libcrun_update_intel_rdt (స్థిరరాశి అక్షరం *ctr_name, libcrun_container_t *container, స్థిరరాశి అక్షరం *l3_cache_schema, స్థిరరాశి అక్షరం *mem_bw_schema, అక్షరం *స్థిరరాశి *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ఖాళీ;
  స్థిరరాశి అక్షరం *name;

  ఒకవేళ (container)
    def = container->container_def;

  ఒకవేళ (def == ఖాళీ || def->linux == ఖాళీ || def->linux->intel_rdt == ఖాళీ)
    తిరిగి 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, ఖాళీ);

  తిరిగి resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
పూర్ణాంకం
libcrun_సురక్షిత_chdir (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  పూర్ణాంకం ret;

  ret = chdir (మార్గం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "chdir to `%s`", మార్గం);

  బఫరు = xmalloc (PATH_MAX);
  ret = syscall_getcwd (బఫరు, PATH_MAX);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  ఒకవేళ (ret == 0 || బఫరు[0] != '/')
    {
      ఒకవేళ (chdir ("/") < 0)
        {
          /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
        }
      errno = ENOENT;

      /*
        The kernel prepends the string "(unreachable)" to the path
        when it is not reachable from the current mount namespace.
        Use it to give a better error message.
      */
#నిర్వచించు UNREACHABLE "(unreachable)"
#నిర్వచించు UNREACHABLE_LEN ((పూర్ణాంకం) పరిమాణం (UNREACHABLE) - 1)

      ఒకవేళ ((ret >= UNREACHABLE_LEN) && (memcmp (బఫరు, UNREACHABLE, UNREACHABLE_LEN) == 0))
        తిరిగి crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      తిరిగి crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  తిరిగి 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
స్థిర పూర్ణాంకం
నిరంతరం_callback_లోపల_namespace (పూర్ణాంకం mntns_fd, పూర్ణాంకం (*callback) (శూన్య *, libcrun_error_t *), శూన్య *arg, libcrun_error_t *err)
{
  పూర్ణాంకం wait_స్థితి = 0;
  pid_t pid;
  పూర్ణాంకం ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ఒకవేళ (UNLIKELY (pid < 0))
    తిరిగి crun_make_error (err, errno, "vfork");

  ఒకవేళ (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          సురక్షిత_exit (ret);
        }
      ret = chdir ("/");
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, errno, "chdir to `/`");
          సురక్షిత_exit (ret);
        }

      ret = callback (arg, err);
      /* On failure (ret < 0) the created error will be
         used by the parent proc */
      సురక్షిత_exit (ret);
    }

  ret = waitpid_ignore_stopped (pid, &wait_స్థితి, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  తిరిగి get_process_exit_స్థితి (wait_స్థితి);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
స్థిర పూర్ణాంకం
check_container_userns (libcrun_container_t *container, పూర్ణాంకం userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close పూర్ణాంకం expected_fd = -1;
  cleanup_close పూర్ణాంకం మాతృ_fd = -1;
  నిర్మాణం stat st, expected_st;
  స్థిరరాశి అక్షరం *మార్గం = ఖాళీ;
  size_t i;
  పూర్ణాంకం ret;

  ఒకవేళ (def->linux)
    కొరకు (i = 0; i < def->linux->namespaces_len; i++)
      ఒకవేళ (libcrun_కనుగొను_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          మార్గం = def->linux->namespaces[i]->మార్గం;
          new_userns = మార్గం == ఖాళీ;
          ఆపు;
        }

  ఒకవేళ (మార్గం == ఖాళీ)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      ఒకవేళ (UNLIKELY (expected_fd < 0))
        తిరిగి expected_fd;
    }
  లేకపోతే
    {
      expected_fd = open (మార్గం, O_RDONLY | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (expected_fd < 0))
        తిరిగి crun_make_error (err, errno, "open `%s`", మార్గం);
    }

  /* A user namespace created for the container is a child of the current one.  */
  ఒకవేళ (new_userns)
    {
      మాతృ_fd = ioctl (userns_fd, NS_GET_మాతృ);
      ఒకవేళ (UNLIKELY (మాతృ_fd < 0))
        తిరిగి crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = మాతృ_fd;
    }

  ret = fstat (userns_fd, &st);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fstat user namespace");

  ఒకవేళ (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    తిరిగి crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  తిరిగి 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
స్థిర పూర్ణాంకం
open_container_mntns (libcrun_container_t *container, libcrun_container_స్థితి_t *స్థితి_2, పూర్ణాంకం *mntns_fd, పూర్ణాంకం *owner_fd, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం mntns = -1;
  cleanup_close పూర్ణాంకం owner = -1;
  పూర్ణాంకం ret;

  mntns = libcrun_open_proc_pid_file (container, స్థితి_2->pid, "ns/mnt", O_RDONLY, err);
  ఒకవేళ (UNLIKELY (mntns < 0))
    తిరిగి mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (ret == 0)
    తిరిగి crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  ఒకవేళ (UNLIKELY (owner < 0))
    తిరిగి crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
నిరంతరం_లోపల_container_namespace (libcrun_container_t *container, libcrun_container_స్థితి_t *స్థితి_2, పూర్ణాంకం (*callback) (శూన్య *, libcrun_error_t *), శూన్య *arg, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం mntns_fd = -1;
  cleanup_close పూర్ణాంకం owner_fd = -1;
  పూర్ణాంకం ret;

  ret = open_container_mntns (container, స్థితి_2, &mntns_fd, &owner_fd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి నిరంతరం_callback_లోపల_namespace (mntns_fd, callback, arg, err);
}

నిర్మాణం umount_లోపల_a_container_వాదనలు
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
స్థిర పూర్ణాంకం
umount_nofollow (స్థిరరాశి అక్షరం *destination, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం parentfd = -1;
  cleanup_close పూర్ణాంకం cwdfd = -1;
  అక్షరం బఫరు[PATH_MAX];
  స్థిరరాశి అక్షరం *మాతృ_2, *name;
  అక్షరం *చివరి_slash;
  size_t len;
  పూర్ణాంకం saved_errno;
  పూర్ణాంకం ret;

  ఒకవేళ (is_ఖాళీలు_string (destination))
    తిరిగి crun_make_error (err, 0, "empty destination for mount");

  len = అక్షరశ్రేణి_పొడవు (destination);
  ఒకవేళ (UNLIKELY (len >= పరిమాణం (బఫరు)))
    తిరిగి crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  జ్ఞాపకం_నకలు (బఫరు, destination, len + 1);

  అయితే (len > 1 && బఫరు[len - 1] == '/')
    బఫరు[--len] = '\0';

  చివరి_slash = strrchr (బఫరు, '/');
  ఒకవేళ (చివరి_slash == ఖాళీ)
    {
      మాతృ_2 = ".";
      name = బఫరు;
    }
  లేకపోతే ఒకవేళ (చివరి_slash == బఫరు)
    {
      మాతృ_2 = "/";
      name = బఫరు[1] ? బఫరు + 1 : ".";
    }
  లేకపోతే
    {
      *చివరి_slash = '\0';
      మాతృ_2 = బఫరు;
      name = చివరి_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, మాతృ_2, O_మార్గం | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  ఒకవేళ (UNLIKELY (parentfd < 0))
    తిరిగి crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_మార్గం | O_DIRECTORY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (cwdfd < 0))
    తిరిగి crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  ఒకవేళ (UNLIKELY (fchdir (cwdfd) < 0))
    తిరిగి crun_make_error (err, errno, "fchdir to the previous working directory");

  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, saved_errno, "umount `%s`", destination);

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
do_umount_లోపల_a_container (శూన్య *arg, libcrun_error_t *err)
{
  నిర్మాణం umount_లోపల_a_container_వాదనలు *వాదనలు = arg;
  size_t i;
  పూర్ణాంకం ret;

  కొరకు (i = 0; i < వాదనలు->len; i++)
    {
      ret = umount_nofollow (వాదనలు->mounts[i]->destination, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

నిర్మాణం mount_లోపల_a_container_వాదనలు
{
  runtime_spec_schema_defs_mount **mounts;
  నిర్మాణం libcrun_fd_map *fds;
  పూర్ణాంకం pidfd;
  పూర్ణాంకం owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
స్థిర పూర్ణాంకం
open_destination_nofollow (స్థిరరాశి అక్షరం *destination, bool dir, libcrun_error_t *err)
{
  స్థిరరాశి uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  పూర్ణాంకం fd;
  పూర్ణాంకం ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_మార్గం | O_CLOEXEC, 0, resolve);
  ఒకవేళ (LIKELY (fd >= 0))
    తిరిగి fd;

  ఒకవేళ (errno != ENOENT)
    తిరిగి crun_make_error (err, errno, "open `%s`", destination);

  ఒకవేళ (dir)
    ret = crun_సురక్షిత_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  లేకపోతే
    ret = crun_సురక్షిత_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_మార్గం | O_CLOEXEC, 0, resolve);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", destination);

  తిరిగి fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
స్థిర పూర్ణాంకం
create_destinations_లోపల_a_container (శూన్య *arg, libcrun_error_t *err)
{
  నిర్మాణం mount_లోపల_a_container_వాదనలు *వాదనలు = arg;
  size_t i;
  పూర్ణాంకం ret;

  ret = setns (వాదనలు->owner_fd, CLONE_NEWUSER);
  ఒకవేళ (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "setresuid to `0`");
    }
  లేకపోతే ఒకవేళ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      తిరిగి crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  కొరకు (i = 0; i < వాదనలు->len; i++)
    {
      cleanup_close పూర్ణాంకం dest_fd = -1;
      నిర్మాణం stat st;

      ret = fstat (వాదనలు->fds->fds[i], &st);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (వాదనలు->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ఒకవేళ (UNLIKELY (dest_fd < 0))
        తిరిగి dest_fd;
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
do_mount_లోపల_a_container (శూన్య *arg, libcrun_error_t *err)
{
  నిర్మాణం mount_లోపల_a_container_వాదనలు *వాదనలు = arg;
  size_t i;
  పూర్ణాంకం ret;

  కొరకు (i = 0; i < వాదనలు->len; i++)
    {
      cleanup_close పూర్ణాంకం dest_fd = -1;
      నిర్మాణం stat st;

      ret = fstat (వాదనలు->fds->fds[i], &st);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (వాదనలు->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ఒకవేళ (UNLIKELY (dest_fd < 0))
        తిరిగి dest_fd;

      ret = fs_move_mount_గమ్యం (వాదనలు->fds->fds[i], dest_fd, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "move mount to `%s`", వాదనలు->mounts[i]->destination);
    }

  తిరిగి 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
స్థిర పూర్ణాంకం
send_mounts_from_container_namespaces (libcrun_container_t *container, పూర్ణాంకం pidfd, runtime_spec_schema_defs_mount **mounts,
                                       నిర్మాణం libcrun_fd_map *fds, size_t len, పూర్ణాంకం flags, పూర్ణాంకం owner_fd, పూర్ణాంకం sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  పూర్ణాంకం ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_సమితి_DUMPABLE, 0, 0, 0, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  ఒకవేళ (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  లేకపోతే ఒకవేళ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      తిరిగి crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ఒకవేళ (UNLIKELY (pid < 0))
    తిరిగి crun_make_error (err, errno, "fork");

  ఒకవేళ (pid)
    {
      పూర్ణాంకం wait_స్థితి = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = తాత్కాలిక_FAILURE_RETRY (waitpid (pid, &wait_స్థితి, WUNTRACED));
      ఒకవేళ (ret > 0 && WIFSTOPPED (wait_స్థితి))
        {
          kill (pid, SIGKILL);
          తాత్కాలిక_FAILURE_RETRY (waitpid (pid, ఖాళీ, 0));
          తిరిగి crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      సురక్షిత_exit (EXIT_SUCCESS);
    }

  ret = సమితి_id_init (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = prctl (PR_సమితి_DUMPABLE, 0, 0, 0, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  కొరకు (i = 0; i < len; i++)
    {
      cleanup_close పూర్ణాంకం fd = -1;

      ఒకవేళ (fds->fds[i] >= 0)
        కొనసాగు;

      fd = open_mount_యొక్క_type (container, mounts[i], err);
      ఒకవేళ (UNLIKELY (fd < 0))
        తిరిగి crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_గమ్యం_socket (sync_fd, fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
స్థిర పూర్ణాంకం
get_namespaces_గమ్యం_join (libcrun_container_t *container, pid_t pid, పూర్ణాంకం *flags, libcrun_error_t *err)
{
  స్థిర స్థిరరాశి నిర్మాణం
  {
    స్థిరరాశి అక్షరం *name;
    పూర్ణాంకం flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  కొరకు (i = 0; i < పరిమాణం (namespaces) / పరిమాణం (namespaces[0]); i++)
    {
      cleanup_close పూర్ణాంకం self_fd = -1;
      cleanup_close పూర్ణాంకం లక్ష్యం_fd = -1;
      నిర్మాణం stat self_st, లక్ష్యం_st;
      cleanup_free అక్షరం *self_మార్గం = ఖాళీ;
      పూర్ణాంకం ret;

      xasprintf (&self_మార్గం, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_మార్గం, O_RDONLY, err);
      ఒకవేళ (UNLIKELY (self_fd < 0))
        తిరిగి self_fd;

      లక్ష్యం_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      ఒకవేళ (UNLIKELY (లక్ష్యం_fd < 0))
        తిరిగి లక్ష్యం_fd;

      ret = fstat (self_fd, &self_st);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (లక్ష్యం_fd, &లక్ష్యం_st);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ఒకవేళ (self_st.st_dev != లక్ష్యం_st.st_dev || self_st.st_ino != లక్ష్యం_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  తిరిగి 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
స్థిర పూర్ణాంకం
open_mounts_లోపల_container_namespaces (libcrun_container_t *container, libcrun_container_స్థితి_t *స్థితి_2, పూర్ణాంకం owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, నిర్మాణం libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం sync_fd = -1;
  cleanup_close పూర్ణాంకం pidfd = -1;
  పూర్ణాంకం sync_socket_fd[2];
  bool needed = false;
  పూర్ణాంకం flags;
  pid_t pid;
  size_t i;
  పూర్ణాంకం ret;

  కొరకు (i = 0; i < len; i++)
    {
      ఒకవేళ (fds->fds[i] >= 0)
        కొనసాగు;

      ఒకవేళ (mounts[i]->type == ఖాళీ)
        తిరిగి crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  ఒకవేళ (! needed)
    తిరిగి 0;

  pidfd = syscall_pidfd_open (స్థితి_2->pid, 0);
  ఒకవేళ (UNLIKELY (pidfd < 0))
    తిరిగి crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (ret == 0)
    తిరిగి crun_make_error (err, 0, "container not running");

  ret = get_namespaces_గమ్యం_join (container, స్థితి_2->pid, &flags, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  ఒకవేళ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      తాత్కాలిక_FAILURE_RETRY (close (sync_socket_fd[1]));
      తిరిగి ret;
    }

  ఒకవేళ (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      ఒకవేళ (UNLIKELY (ret < 0))
        send_error_గమ్యం_sync_socket_and_die (sync_socket_fd[1], true, err);

      సురక్షిత_exit (EXIT_SUCCESS);
    }

  తాత్కాలిక_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  కొరకు (i = 0; i < len; i++)
    {
      ఒకవేళ (fds->fds[i] >= 0)
        కొనసాగు;

      ret = receive_fd_from_socket (sync_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          cleanup_free అక్షరం *err_str = ఖాళీ;
          పూర్ణాంకం err_సంకేతం = 0;

          ఒకవేళ (read_error_from_sync_socket (sync_fd, &err_సంకేతం, &err_str) && ! is_ఖాళీలు_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_సంకేతం, "%s", err_str);
            }
          లేకపోతే
            ret = crun_error_wrap (err, "receive mount fd");
          ఆపు;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  తాత్కాలిక_FAILURE_RETRY (waitpid (pid, ఖాళీ, 0));

  తిరిగి ret;
}

స్థిర గుర్తులేని దీర్ఘ
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_సమితి)
{
  cleanup_free అక్షరం *దత్తాంశం = ఖాళీ;
  గుర్తులేని దీర్ఘ extra_flags = 0;
  గుర్తులేని దీర్ఘ flags = 0;
  size_t j;

  ఒకవేళ (mount->options == ఖాళీ)
    తిరిగి get_default_flags (container, mount->destination, &దత్తాంశం);

  కొరకు (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &దత్తాంశం, rec_clear, rec_సమితి);

  తిరిగి flags;
}

పూర్ణాంకం
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_స్థితి_t *స్థితి_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  నిర్మాణం mount_లోపల_a_container_వాదనలు వాదనలు;
  cleanup_close_map నిర్మాణం libcrun_fd_map *fds = ఖాళీ;
  cleanup_close పూర్ణాంకం pidfd = -1;
  cleanup_close పూర్ణాంకం mntns_fd = -1;
  cleanup_close పూర్ణాంకం owner_fd = -1;
  pid_t pid = స్థితి_2->pid;
  size_t i;
  పూర్ణాంకం ret;

  ret = open_container_mntns (container, స్థితి_2, &mntns_fd, &owner_fd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  fds = make_libcrun_fd_map (len);

  కొరకు (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), ఖాళీ, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_సమితి = 0;
          గుర్తులేని దీర్ఘ ప్రసరణ;

          ప్రసరణ = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_సమితి) & ALL_PROPAGATIONS_NO_REC;
          ఒకవేళ (ప్రసరణ == 0)
            ప్రసరణ = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->ఆధారం, recursive, false, nofollow, ప్రసరణ, err);
          ఒకవేళ (UNLIKELY (fds->fds[i] < 0))
            తిరిగి fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_లోపల_container_namespaces (container, స్థితి_2, owner_fd, mounts, fds, len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  కొరకు (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_సమితి = 0;
      గుర్తులేని దీర్ఘ flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_సమితి);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_సమితి, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  వాదనలు.mounts = mounts;
  వాదనలు.fds = fds;
  వాదనలు.pidfd = pidfd;
  వాదనలు.owner_fd = owner_fd;
  వాదనలు.len = len;

  ret = నిరంతరం_callback_లోపల_namespace (mntns_fd, create_destinations_లోపల_a_container, &వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret > 0))
    తిరిగి *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి నిరంతరం_callback_లోపల_namespace (mntns_fd, do_mount_లోపల_a_container, &వాదనలు, err);
}

పూర్ణాంకం
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_స్థితి_t *స్థితి_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  నిర్మాణం umount_లోపల_a_container_వాదనలు వాదనలు;

  వాదనలు.mounts = mounts;
  వాదనలు.len = len;

  తిరిగి నిరంతరం_లోపల_container_namespace (container, స్థితి_2, do_umount_లోపల_a_container, &వాదనలు, err);
}

నిర్మాణం restrict_mounts_లోపల_a_container_వాదనలు
{
  నిర్మాణం libcrun_mount_flags_s *mounts;
  size_t len;
  పూర్ణాంకం rootfd;
};

స్థిర పూర్ణాంకం
do_restrict_mounts_లోపల_a_container (శూన్య *arg, libcrun_error_t *err)
{
  నిర్మాణం restrict_mounts_లోపల_a_container_వాదనలు *వాదనలు = arg;
  size_t i;
  పూర్ణాంకం ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (వాదనలు->rootfd);
  ఒకవేళ (LIKELY (ret == 0))
    ret = chroot (".");
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "chroot to the container root");

  కొరకు (i = 0; i < వాదనలు->len; i++)
    {
      గుర్తులేని దీర్ఘ flags = వాదనలు->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      స్థిరరాశి అక్షరం *లక్ష్యం = consume_slashes (వాదనలు->mounts[i].లక్ష్యం);
      cleanup_close పూర్ణాంకం fd = -1;
      నిర్మాణం statfs sfs;
      గుర్తులేని దీర్ఘ current_flags;

      ఒకవేళ (flags == 0)
        కొనసాగు;

      fd = సురక్షిత_openat (AT_FDCWD, "/", లక్ష్యం, O_మార్గం | O_CLOEXEC, 0, err);
      ఒకవేళ (UNLIKELY (fd < 0))
        {
          ఒకవేళ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              కొనసాగు;
            }
          తిరిగి fd;
        }

      ret = fstatfs (fd, &sfs);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "statfs `/%s`", లక్ష్యం);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      ఒకవేళ ((current_flags & flags) == flags)
        కొనసాగు;

      ret = do_mount_setattr (false, లక్ష్యం, fd, 0, flags, err);
      ఒకవేళ (LIKELY (ret == 0))
        కొనసాగు;

      ఒకవేళ (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        తిరిగి ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (ఖాళీ, లక్ష్యం, ఖాళీ, MS_REMOUNT | MS_BIND | flags | current_flags, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "remount `/%s`", లక్ష్యం);
    }

  తిరిగి 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
పూర్ణాంకం
libcrun_restrict_mounts_లోపల_container (pid_t pid, నిర్మాణం libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  నిర్మాణం restrict_mounts_లోపల_a_container_వాదనలు వాదనలు;
  cleanup_close పూర్ణాంకం mntns_fd = -1;
  cleanup_close పూర్ణాంకం rootfd = -1;
  అక్షరం మార్గం[64];
  పూర్ణాంకం ret;

  snprintf (మార్గం, పరిమాణం (మార్గం), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (మార్గం, O_RDONLY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (mntns_fd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", మార్గం);

  snprintf (మార్గం, పరిమాణం (మార్గం), "/proc/%d/root", pid);
  rootfd = open (మార్గం, O_మార్గం | O_DIRECTORY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (rootfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", మార్గం);

  వాదనలు.mounts = mounts;
  వాదనలు.len = len;
  వాదనలు.rootfd = rootfd;

  ret = నిరంతరం_callback_లోపల_namespace (mntns_fd, do_restrict_mounts_లోపల_a_container, &వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret > 0))
    తిరిగి *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  తిరిగి ret;
}

పూర్ణాంకం
libcrun_move_జాలం_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close పూర్ణాంకం netns_fd = -1;
  size_t i;
  పూర్ణాంకం ret;

  ఒకవేళ (def == ఖాళీ || def->linux == ఖాళీ || def->linux->net_devices == ఖాళీ)
    తిరిగి 0;

  cleanup_free అక్షరం *ns_మార్గం = ఖాళీ;

  xasprintf (&ns_మార్గం, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_మార్గం, O_RDONLY, err);
  ఒకవేళ (UNLIKELY (netns_fd < 0))
    తిరిగి netns_fd;

  కొరకు (i = 0; i < def->linux->net_devices->len; i++)
    {
      స్థిరరాశి అక్షరం *new_name = def->linux->net_devices->విలువలు[i]->name ?: def->linux->net_devices->కీలకాలు[i];

      ret = move_జాలం_device (def->linux->net_devices->కీలకాలు[i], new_name, netns_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}
