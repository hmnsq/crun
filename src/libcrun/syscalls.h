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

#ifndef CRUN_SYSCALLS_H
#define CRUN_SYSCALLS_H

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <sys/syscall.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <stdint.h>

#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#যদি defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#endif

/* Constants for new mount API */
#ifndef FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#endif

#ifndef FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#endif

#ifndef FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#endif

#ifndef FSCONFIG_SET_STRING
#  define FSCONFIG_SET_STRING 1
#endif

/* Additional mount API constants */
#ifndef MOUNT_ATTR_RDONLY
#  define MOUNT_ATTR_RDONLY 0x00000001 /* Mount read-only */
#endif

#ifndef MOUNT_ATTR_NOSUID
#  define MOUNT_ATTR_NOSUID 0x00000002
#endif

#ifndef MOUNT_ATTR_NODEV
#  define MOUNT_ATTR_NODEV 0x00000004
#endif

#ifndef MOUNT_ATTR_NOEXEC
#  define MOUNT_ATTR_NOEXEC 0x00000008
#endif

#ifndef MOUNT_ATTR__ATIME
#  define MOUNT_ATTR__ATIME 0x00000070
#endif

#ifndef MOUNT_ATTR_RELATIME
#  define MOUNT_ATTR_RELATIME 0x00000000
#endif

#ifndef MOUNT_ATTR_NOATIME
#  define MOUNT_ATTR_NOATIME 0x00000010
#endif

#ifndef MOUNT_ATTR_STRICTATIME
#  define MOUNT_ATTR_STRICTATIME 0x00000020
#endif

#ifndef MOUNT_ATTR_NODIRATIME
#  define MOUNT_ATTR_NODIRATIME 0x00000080
#endif

#ifndef MOUNT_ATTR_IDMAP
#  define MOUNT_ATTR_IDMAP 0x00100000 /* Idmap mount to @userns_fd in struct mount_attr. */
#endif

#ifndef MOUNT_ATTR_NOSYMFOLLOW
#  define MOUNT_ATTR_NOSYMFOLLOW 0x00200000
#endif

#ifndef MS_NOSYMFOLLOW
#  define MS_NOSYMFOLLOW 256
#endif

/* close_range flags */
#ifndef CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#endif

/* openat2 resolve flags */
#ifndef RESOLVE_NO_MAGICLINKS
#  define RESOLVE_NO_MAGICLINKS 0x02
#endif
#ifndef RESOLVE_NO_SYMLINKS
#  define RESOLVE_NO_SYMLINKS 0x04
#endif
#ifndef RESOLVE_IN_মূল
#  define RESOLVE_IN_মূল 0x10
#endif

/* Structures for syscalls */
কাঠামো openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

কাঠামো mount_attr_s
{
  uint64_t attr_set;
  uint64_t attr_clr;
  uint64_t propagation;
  uint64_t userns_fd;
};

#ifndef __aligned_u64
#  define __aligned_u64 uint64_t __attribute__ ((aligned (8)))
#endif

#ifndef CLONE_INTO_CGROUP
#  define CLONE_INTO_CGROUP 0x200000000ULL
#endif

কাঠামো _clone3_args
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 স্তূপ;
  __aligned_u64 স্তূপ_আকার;
  __aligned_u64 tls;
  __aligned_u64 set_tid;
  __aligned_u64 set_tid_আকার;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
স্থির অন্তঃরেখ পূর্ণসংখ্যা
syscall_fsopen (ধ্রুবক অক্ষর *fs_name, unsigned পূর্ণসংখ্যা flags)
{
#যদি defined __NR_fsopen
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_fsopen, fs_name, flags);
#অন্যথা
  (শূন্য) fs_name;
  (শূন্য) flags;
  errno = ENOSYS;
  ফেরত -1;
#endif
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
syscall_fsmount (পূর্ণসংখ্যা fsfd, unsigned পূর্ণসংখ্যা flags, unsigned পূর্ণসংখ্যা attr_flags)
{
#যদি defined __NR_fsmount
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#অন্যথা
  (শূন্য) fsfd;
  (শূন্য) flags;
  (শূন্য) attr_flags;
  errno = ENOSYS;
  ফেরত -1;
#endif
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
syscall_fsconfig (পূর্ণসংখ্যা fsfd, unsigned পূর্ণসংখ্যা cmd, ধ্রুবক অক্ষর *চাবি, ধ্রুবক শূন্য *val, পূর্ণসংখ্যা aux)
{
#যদি defined __NR_fsconfig
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_fsconfig, fsfd, cmd, চাবি, val, aux);
#অন্যথা
  (শূন্য) fsfd;
  (শূন্য) cmd;
  (শূন্য) চাবি;
  (শূন্য) val;
  (শূন্য) aux;
  errno = ENOSYS;
  ফেরত -1;
#endif
}

/* File descriptor management syscalls */
স্থির অন্তঃরেখ পূর্ণসংখ্যা
syscall_close_range (unsigned পূর্ণসংখ্যা fd, unsigned পূর্ণসংখ্যা max_fd, unsigned পূর্ণসংখ্যা flags)
{
#ifdef __NR_close_range
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_close_range, fd, max_fd, flags);
#অন্যথা
  (শূন্য) fd;
  (শূন্য) max_fd;
  (শূন্য) flags;
  errno = ENOSYS;
  ফেরত -1;
#endif
}

/* Secure file operation syscalls */
স্থির অন্তঃরেখ পূর্ণসংখ্যা
syscall_openat2 (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  কাঠামো openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_openat2, dirfd, path, &how, আকার (how), 0);
#অন্যথা
  (শূন্য) dirfd;
  (শূন্য) path;
  (শূন্য) flags;
  (শূন্য) mode;
  (শূন্য) resolve;
  errno = ENOSYS;
  ফেরত -1;
#endif
}

/* Process management syscalls */
স্থির অন্তঃরেখ পূর্ণসংখ্যা
syscall_clone3 (কাঠামো _clone3_args *args)
{
#ifdef __NR_clone3
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_clone3, args, আকার (*args));
#অন্যথা
  (শূন্য) args;
  errno = ENOSYS;
  ফেরত -1;
#endif
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
syscall_getcwd (অক্ষর *path, size_t len)
{
#ifdef __NR_getcwd
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_getcwd, path, len);
#অন্যথা
  (শূন্য) path;
  (শূন্য) len;
  errno = ENOSYS;
  ফেরত -1;
#endif
}

/* Mount management syscalls */
স্থির অন্তঃরেখ পূর্ণসংখ্যা
syscall_move_mount (পূর্ণসংখ্যা from_dfd, ধ্রুবক অক্ষর *from_pathname, পূর্ণসংখ্যা to_dfd, ধ্রুবক অক্ষর *to_pathname, unsigned পূর্ণসংখ্যা flags)
{
#যদি defined __NR_move_mount
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#অন্যথা
  (শূন্য) from_dfd;
  (শূন্য) from_pathname;
  (শূন্য) to_dfd;
  (শূন্য) to_pathname;
  (শূন্য) flags;
  errno = ENOSYS;
  ফেরত -1;
#endif
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
syscall_open_বৃক্ষ (পূর্ণসংখ্যা dfd, ধ্রুবক অক্ষর *pathname, unsigned পূর্ণসংখ্যা flags)
{
#যদি defined __NR_open_tree
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_open_tree, dfd, pathname, flags);
#অন্যথা
  (শূন্য) dfd;
  (শূন্য) pathname;
  (শূন্য) flags;
  errno = ENOSYS;
  ফেরত -1;
#endif
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
syscall_mount_setattr (পূর্ণসংখ্যা dfd, ধ্রুবক অক্ষর *path, unsigned পূর্ণসংখ্যা flags, কাঠামো mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_mount_setattr, dfd, path, flags, attr, আকার (*attr));
#অন্যথা
  (শূন্য) dfd;
  (শূন্য) path;
  (শূন্য) flags;
  (শূন্য) attr;
  errno = ENOSYS;
  ফেরত -1;
#endif
}

#endif
