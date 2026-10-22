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
সংরচনা openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

সংরচনা mount_attr_s
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

সংরচনা _clone3_args
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 স্তূপ;
  __aligned_u64 স্তূপ_size;
  __aligned_u64 tls;
  __aligned_u64 set_tid;
  __aligned_u64 set_tid_size;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
স্থির অন্তঃস্থ পূর্ণাঙ্ক
syscall_fsopen (ধ্রুব বর্ণ *fs_name, unsigned পূর্ণাঙ্ক flags)
{
#যদি defined __NR_fsopen
  প্রত্যাগম (পূর্ণাঙ্ক) syscall (__NR_fsopen, fs_name, flags);
#অন্যথা
  (শূন্য) fs_name;
  (শূন্য) flags;
  errno = ENOSYS;
  প্রত্যাগম -1;
#endif
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
syscall_fsmount (পূর্ণাঙ্ক fsfd, unsigned পূর্ণাঙ্ক flags, unsigned পূর্ণাঙ্ক attr_flags)
{
#যদি defined __NR_fsmount
  প্রত্যাগম (পূর্ণাঙ্ক) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#অন্যথা
  (শূন্য) fsfd;
  (শূন্য) flags;
  (শূন্য) attr_flags;
  errno = ENOSYS;
  প্রত্যাগম -1;
#endif
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
syscall_fsconfig (পূর্ণাঙ্ক fsfd, unsigned পূর্ণাঙ্ক cmd, ধ্রুব বর্ণ *চাবি, ধ্রুব শূন্য *val, পূর্ণাঙ্ক aux)
{
#যদি defined __NR_fsconfig
  প্রত্যাগম (পূর্ণাঙ্ক) syscall (__NR_fsconfig, fsfd, cmd, চাবি, val, aux);
#অন্যথা
  (শূন্য) fsfd;
  (শূন্য) cmd;
  (শূন্য) চাবি;
  (শূন্য) val;
  (শূন্য) aux;
  errno = ENOSYS;
  প্রত্যাগম -1;
#endif
}

/* File descriptor management syscalls */
স্থির অন্তঃস্থ পূর্ণাঙ্ক
syscall_close_range (unsigned পূর্ণাঙ্ক fd, unsigned পূর্ণাঙ্ক max_fd, unsigned পূর্ণাঙ্ক flags)
{
#ifdef __NR_close_range
  প্রত্যাগম (পূর্ণাঙ্ক) syscall (__NR_close_range, fd, max_fd, flags);
#অন্যথা
  (শূন্য) fd;
  (শূন্য) max_fd;
  (শূন্য) flags;
  errno = ENOSYS;
  প্রত্যাগম -1;
#endif
}

/* Secure file operation syscalls */
স্থির অন্তঃস্থ পূর্ণাঙ্ক
syscall_openat2 (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  সংরচনা openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  প্রত্যাগম (পূর্ণাঙ্ক) syscall (__NR_openat2, dirfd, path, &how, পরিমাণ (how), 0);
#অন্যথা
  (শূন্য) dirfd;
  (শূন্য) path;
  (শূন্য) flags;
  (শূন্য) mode;
  (শূন্য) resolve;
  errno = ENOSYS;
  প্রত্যাগম -1;
#endif
}

/* Process management syscalls */
স্থির অন্তঃস্থ পূর্ণাঙ্ক
syscall_clone3 (সংরচনা _clone3_args *args)
{
#ifdef __NR_clone3
  প্রত্যাগম (পূর্ণাঙ্ক) syscall (__NR_clone3, args, পরিমাণ (*args));
#অন্যথা
  (শূন্য) args;
  errno = ENOSYS;
  প্রত্যাগম -1;
#endif
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
syscall_getcwd (বর্ণ *path, size_t len)
{
#ifdef __NR_getcwd
  প্রত্যাগম (পূর্ণাঙ্ক) syscall (__NR_getcwd, path, len);
#অন্যথা
  (শূন্য) path;
  (শূন্য) len;
  errno = ENOSYS;
  প্রত্যাগম -1;
#endif
}

/* Mount management syscalls */
স্থির অন্তঃস্থ পূর্ণাঙ্ক
syscall_move_mount (পূর্ণাঙ্ক from_dfd, ধ্রুব বর্ণ *from_pathname, পূর্ণাঙ্ক to_dfd, ধ্রুব বর্ণ *to_pathname, unsigned পূর্ণাঙ্ক flags)
{
#যদি defined __NR_move_mount
  প্রত্যাগম (পূর্ণাঙ্ক) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#অন্যথা
  (শূন্য) from_dfd;
  (শূন্য) from_pathname;
  (শূন্য) to_dfd;
  (শূন্য) to_pathname;
  (শূন্য) flags;
  errno = ENOSYS;
  প্রত্যাগম -1;
#endif
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
syscall_open_বৃক্ষ (পূর্ণাঙ্ক dfd, ধ্রুব বর্ণ *pathname, unsigned পূর্ণাঙ্ক flags)
{
#যদি defined __NR_open_tree
  প্রত্যাগম (পূর্ণাঙ্ক) syscall (__NR_open_tree, dfd, pathname, flags);
#অন্যথা
  (শূন্য) dfd;
  (শূন্য) pathname;
  (শূন্য) flags;
  errno = ENOSYS;
  প্রত্যাগম -1;
#endif
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
syscall_mount_setattr (পূর্ণাঙ্ক dfd, ধ্রুব বর্ণ *path, unsigned পূর্ণাঙ্ক flags, সংরচনা mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  প্রত্যাগম (পূর্ণাঙ্ক) syscall (__NR_mount_setattr, dfd, path, flags, attr, পরিমাণ (*attr));
#অন্যথা
  (শূন্য) dfd;
  (শূন্য) path;
  (শূন্য) flags;
  (শূন্য) attr;
  errno = ENOSYS;
  প্রত্যাগম -1;
#endif
}

#endif
