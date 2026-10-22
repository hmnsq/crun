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

#నిర్వచితం_కాకపోతే CRUN_SYSCALLS_H
#నిర్వచించు CRUN_SYSCALLS_H

#అన్తర్భావ <config.h>
#అన్తర్భావ <sys/syscall.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <stdint.h>

#నిర్వచితమైతే HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#అయితే_ముగింపు
#ఒకవేళ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#అయితే_ముగింపు

/* Constants for new mount API */
#నిర్వచితం_కాకపోతే FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే FSCONFIG_SET_STRING
#  define FSCONFIG_SET_STRING 1
#అయితే_ముగింపు

/* Additional mount API constants */
#నిర్వచితం_కాకపోతే MOUNT_ATTR_RDONLY
#  define MOUNT_ATTR_RDONLY 0x00000001 /* Mount read-only */
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOUNT_ATTR_NOSUID
#  define MOUNT_ATTR_NOSUID 0x00000002
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOUNT_ATTR_NODEV
#  define MOUNT_ATTR_NODEV 0x00000004
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOUNT_ATTR_NOEXEC
#  define MOUNT_ATTR_NOEXEC 0x00000008
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOUNT_ATTR__ATIME
#  define MOUNT_ATTR__ATIME 0x00000070
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOUNT_ATTR_RELATIME
#  define MOUNT_ATTR_RELATIME 0x00000000
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOUNT_ATTR_NOATIME
#  define MOUNT_ATTR_NOATIME 0x00000010
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOUNT_ATTR_STRICTATIME
#  define MOUNT_ATTR_STRICTATIME 0x00000020
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOUNT_ATTR_NODIRATIME
#  define MOUNT_ATTR_NODIRATIME 0x00000080
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOUNT_ATTR_IDMAP
#  define MOUNT_ATTR_IDMAP 0x00100000 /* Idmap mount to @userns_fd in struct mount_attr. */
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MOUNT_ATTR_NOSYMFOLLOW
#  define MOUNT_ATTR_NOSYMFOLLOW 0x00200000
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MS_NOSYMFOLLOW
#  define MS_NOSYMFOLLOW 256
#అయితే_ముగింపు

/* close_range flags */
#నిర్వచితం_కాకపోతే CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#అయితే_ముగింపు

/* openat2 resolve flags */
#నిర్వచితం_కాకపోతే RESOLVE_NO_MAGICLINKS
#  define RESOLVE_NO_MAGICLINKS 0x02
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే RESOLVE_NO_SYMLINKS
#  define RESOLVE_NO_SYMLINKS 0x04
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే RESOLVE_లోపల_మూలం
#  define RESOLVE_లోపల_మూలం 0x10
#అయితే_ముగింపు

/* Structures for syscalls */
నిర్మాణం openat2_open_how
{
  uint64_t flags;
  uint64_t బహుళకం;
  uint64_t resolve;
};

నిర్మాణం mount_attr_s
{
  uint64_t attr_set;
  uint64_t attr_clr;
  uint64_t ప్రసరణ;
  uint64_t userns_fd;
};

#నిర్వచితం_కాకపోతే __aligned_u64
#  define __aligned_u64 uint64_t __attribute__ ((aligned (8)))
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే CLONE_INTO_CGROUP
#  define CLONE_INTO_CGROUP 0x200000000ULL
#అయితే_ముగింపు

నిర్మాణం clone3_వాదనలు
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 బిడ్డ_tid;
  __aligned_u64 మాతృ_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 దొంతర;
  __aligned_u64 దొంతర_కొలత;
  __aligned_u64 tls;
  __aligned_u64 సమితి_tid;
  __aligned_u64 సమితి_tid_కొలత;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
స్థిర అంతఃవరుస పూర్ణాంకం
syscall_fsopen (స్థిరరాశి అక్షరం *fs_name, గుర్తులేని పూర్ణాంకం flags)
{
#ఒకవేళ defined __NR_fsopen
  తిరిగి (పూర్ణాంకం) syscall (__NR_fsopen, fs_name, flags);
#లేకపోతే
  (శూన్య) fs_name;
  (శూన్య) flags;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస పూర్ణాంకం
syscall_fsmount (పూర్ణాంకం fsfd, గుర్తులేని పూర్ణాంకం flags, గుర్తులేని పూర్ణాంకం attr_flags)
{
#ఒకవేళ defined __NR_fsmount
  తిరిగి (పూర్ణాంకం) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#లేకపోతే
  (శూన్య) fsfd;
  (శూన్య) flags;
  (శూన్య) attr_flags;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస పూర్ణాంకం
syscall_fsconfig (పూర్ణాంకం fsfd, గుర్తులేని పూర్ణాంకం cmd, స్థిరరాశి అక్షరం *కీలకం, స్థిరరాశి శూన్య *val, పూర్ణాంకం aux)
{
#ఒకవేళ defined __NR_fsconfig
  తిరిగి (పూర్ణాంకం) syscall (__NR_fsconfig, fsfd, cmd, కీలకం, val, aux);
#లేకపోతే
  (శూన్య) fsfd;
  (శూన్య) cmd;
  (శూన్య) కీలకం;
  (శూన్య) val;
  (శూన్య) aux;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

/* File descriptor management syscalls */
స్థిర అంతఃవరుస పూర్ణాంకం
syscall_close_range (గుర్తులేని పూర్ణాంకం fd, గుర్తులేని పూర్ణాంకం max_fd, గుర్తులేని పూర్ణాంకం flags)
{
#నిర్వచితమైతే __NR_close_range
  తిరిగి (పూర్ణాంకం) syscall (__NR_close_range, fd, max_fd, flags);
#లేకపోతే
  (శూన్య) fd;
  (శూన్య) max_fd;
  (శూన్య) flags;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

/* Secure file operation syscalls */
స్థిర అంతఃవరుస పూర్ణాంకం
syscall_openat2 (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *మార్గం, uint64_t flags, uint64_t బహుళకం, uint64_t resolve)
{
#నిర్వచితమైతే __NR_openat2
  నిర్మాణం openat2_open_how how = {
    .flags = flags,
    .బహుళకం = బహుళకం,
    .resolve = resolve,
  };
  తిరిగి (పూర్ణాంకం) syscall (__NR_openat2, dirfd, మార్గం, &how, పరిమాణం (how), 0);
#లేకపోతే
  (శూన్య) dirfd;
  (శూన్య) మార్గం;
  (శూన్య) flags;
  (శూన్య) బహుళకం;
  (శూన్య) resolve;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

/* Process management syscalls */
స్థిర అంతఃవరుస పూర్ణాంకం
syscall_clone3 (నిర్మాణం clone3_వాదనలు *వాదనలు)
{
#నిర్వచితమైతే __NR_clone3
  తిరిగి (పూర్ణాంకం) syscall (__NR_clone3, వాదనలు, పరిమాణం (*వాదనలు));
#లేకపోతే
  (శూన్య) వాదనలు;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస పూర్ణాంకం
syscall_getcwd (అక్షరం *మార్గం, size_t len)
{
#నిర్వచితమైతే __NR_getcwd
  తిరిగి (పూర్ణాంకం) syscall (__NR_getcwd, మార్గం, len);
#లేకపోతే
  (శూన్య) మార్గం;
  (శూన్య) len;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

/* Mount management syscalls */
స్థిర అంతఃవరుస పూర్ణాంకం
syscall_move_mount (పూర్ణాంకం from_dfd, స్థిరరాశి అక్షరం *from_pathname, పూర్ణాంకం గమ్యం_dfd, స్థిరరాశి అక్షరం *గమ్యం_pathname, గుర్తులేని పూర్ణాంకం flags)
{
#ఒకవేళ defined __NR_move_mount
  తిరిగి (పూర్ణాంకం) syscall (__NR_move_mount, from_dfd, from_pathname, గమ్యం_dfd, గమ్యం_pathname, flags);
#లేకపోతే
  (శూన్య) from_dfd;
  (శూన్య) from_pathname;
  (శూన్య) గమ్యం_dfd;
  (శూన్య) గమ్యం_pathname;
  (శూన్య) flags;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస పూర్ణాంకం
syscall_open_వృక్షం (పూర్ణాంకం dfd, స్థిరరాశి అక్షరం *pathname, గుర్తులేని పూర్ణాంకం flags)
{
#ఒకవేళ defined __NR_open_tree
  తిరిగి (పూర్ణాంకం) syscall (__NR_open_tree, dfd, pathname, flags);
#లేకపోతే
  (శూన్య) dfd;
  (శూన్య) pathname;
  (శూన్య) flags;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస పూర్ణాంకం
syscall_mount_setattr (పూర్ణాంకం dfd, స్థిరరాశి అక్షరం *మార్గం, గుర్తులేని పూర్ణాంకం flags, నిర్మాణం mount_attr_s *attr)
{
#నిర్వచితమైతే __NR_mount_setattr
  తిరిగి (పూర్ణాంకం) syscall (__NR_mount_setattr, dfd, మార్గం, flags, attr, పరిమాణం (*attr));
#లేకపోతే
  (శూన్య) dfd;
  (శూన్య) మార్గం;
  (శూన్య) flags;
  (శూన్య) attr;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

#అయితే_ముగింపు
