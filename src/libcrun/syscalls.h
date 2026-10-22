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

#شامل <config.h>
#شامل <sys/syscall.h>
#شامل <unistd.h>
#شامل <errno.h>
#شامل <stdint.h>

#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#اگر defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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
#ifndef RESOLVE_IN_ریشه
#  define RESOLVE_IN_ریشه 0x10
#endif

/* Structures for syscalls */
ساختار openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

ساختار mount_attr_s
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

ساختار _clone3_args
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 پشته;
  __aligned_u64 پشته_size;
  __aligned_u64 tls;
  __aligned_u64 set_tid;
  __aligned_u64 set_tid_size;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
ایستا درونخطی صحیح
syscall_fsopen (ثابت نویسه *fs_name, unsigned صحیح flags)
{
#اگر defined __NR_fsopen
  بازگشت (صحیح) syscall (__NR_fsopen, fs_name, flags);
#وگرنه
  (تهی) fs_name;
  (تهی) flags;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

ایستا درونخطی صحیح
syscall_fsmount (صحیح fsfd, unsigned صحیح flags, unsigned صحیح attr_flags)
{
#اگر defined __NR_fsmount
  بازگشت (صحیح) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#وگرنه
  (تهی) fsfd;
  (تهی) flags;
  (تهی) attr_flags;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

ایستا درونخطی صحیح
syscall_fsconfig (صحیح fsfd, unsigned صحیح cmd, ثابت نویسه *کلید, ثابت تهی *val, صحیح aux)
{
#اگر defined __NR_fsconfig
  بازگشت (صحیح) syscall (__NR_fsconfig, fsfd, cmd, کلید, val, aux);
#وگرنه
  (تهی) fsfd;
  (تهی) cmd;
  (تهی) کلید;
  (تهی) val;
  (تهی) aux;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

/* File descriptor management syscalls */
ایستا درونخطی صحیح
syscall_close_range (unsigned صحیح fd, unsigned صحیح max_fd, unsigned صحیح flags)
{
#ifdef __NR_close_range
  بازگشت (صحیح) syscall (__NR_close_range, fd, max_fd, flags);
#وگرنه
  (تهی) fd;
  (تهی) max_fd;
  (تهی) flags;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

/* Secure file operation syscalls */
ایستا درونخطی صحیح
syscall_openat2 (صحیح dirfd, ثابت نویسه *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  ساختار openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  بازگشت (صحیح) syscall (__NR_openat2, dirfd, path, &how, اندازه (how), 0);
#وگرنه
  (تهی) dirfd;
  (تهی) path;
  (تهی) flags;
  (تهی) mode;
  (تهی) resolve;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

/* Process management syscalls */
ایستا درونخطی صحیح
syscall_clone3 (ساختار _clone3_args *args)
{
#ifdef __NR_clone3
  بازگشت (صحیح) syscall (__NR_clone3, args, اندازه (*args));
#وگرنه
  (تهی) args;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

ایستا درونخطی صحیح
syscall_getcwd (نویسه *path, size_t len)
{
#ifdef __NR_getcwd
  بازگشت (صحیح) syscall (__NR_getcwd, path, len);
#وگرنه
  (تهی) path;
  (تهی) len;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

/* Mount management syscalls */
ایستا درونخطی صحیح
syscall_move_mount (صحیح from_dfd, ثابت نویسه *from_pathname, صحیح to_dfd, ثابت نویسه *to_pathname, unsigned صحیح flags)
{
#اگر defined __NR_move_mount
  بازگشت (صحیح) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#وگرنه
  (تهی) from_dfd;
  (تهی) from_pathname;
  (تهی) to_dfd;
  (تهی) to_pathname;
  (تهی) flags;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

ایستا درونخطی صحیح
syscall_open_درخت (صحیح dfd, ثابت نویسه *pathname, unsigned صحیح flags)
{
#اگر defined __NR_open_tree
  بازگشت (صحیح) syscall (__NR_open_tree, dfd, pathname, flags);
#وگرنه
  (تهی) dfd;
  (تهی) pathname;
  (تهی) flags;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

ایستا درونخطی صحیح
syscall_mount_setattr (صحیح dfd, ثابت نویسه *path, unsigned صحیح flags, ساختار mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  بازگشت (صحیح) syscall (__NR_mount_setattr, dfd, path, flags, attr, اندازه (*attr));
#وگرنه
  (تهی) dfd;
  (تهی) path;
  (تهی) flags;
  (تهی) attr;
  errno = ENOSYS;
  بازگشت -1;
#endif
}

#endif
