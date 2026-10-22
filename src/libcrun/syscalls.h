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

#ضمن <config.h>
#ضمن <sys/syscall.h>
#ضمن <unistd.h>
#ضمن <errno.h>
#ضمن <stdint.h>

#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#𐤀𐤌 defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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
#ifndef RESOLVE_IN_ROOT
#  define RESOLVE_IN_ROOT 0x10
#endif

/* Structures for syscalls */
بنية openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

بنية mount_attr_s
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

بنية _clone3_args
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 stack;
  __aligned_u64 stack_size;
  __aligned_u64 tls;
  __aligned_u64 set_tid;
  __aligned_u64 set_tid_size;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
ساكن مضمن صحيح
syscall_fsopen (ثابت حرف *fs_name, unsigned صحيح flags)
{
#𐤀𐤌 defined __NR_fsopen
  أرجع (صحيح) syscall (__NR_fsopen, fs_name, flags);
#وإلا
  (فراغ) fs_name;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#endif
}

ساكن مضمن صحيح
syscall_fsmount (صحيح fsfd, unsigned صحيح flags, unsigned صحيح attr_flags)
{
#𐤀𐤌 defined __NR_fsmount
  أرجع (صحيح) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#وإلا
  (فراغ) fsfd;
  (فراغ) flags;
  (فراغ) attr_flags;
  errno = ENOSYS;
  أرجع -1;
#endif
}

ساكن مضمن صحيح
syscall_fsconfig (صحيح fsfd, unsigned صحيح cmd, ثابت حرف *key, ثابت فراغ *val, صحيح aux)
{
#𐤀𐤌 defined __NR_fsconfig
  أرجع (صحيح) syscall (__NR_fsconfig, fsfd, cmd, key, val, aux);
#وإلا
  (فراغ) fsfd;
  (فراغ) cmd;
  (فراغ) key;
  (فراغ) val;
  (فراغ) aux;
  errno = ENOSYS;
  أرجع -1;
#endif
}

/* File descriptor management syscalls */
ساكن مضمن صحيح
syscall_close_range (unsigned صحيح fd, unsigned صحيح max_fd, unsigned صحيح flags)
{
#ifdef __NR_close_range
  أرجع (صحيح) syscall (__NR_close_range, fd, max_fd, flags);
#وإلا
  (فراغ) fd;
  (فراغ) max_fd;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#endif
}

/* Secure file operation syscalls */
ساكن مضمن صحيح
syscall_openat2 (صحيح dirfd, ثابت حرف *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  بنية openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  أرجع (صحيح) syscall (__NR_openat2, dirfd, path, &how, حجم (how), 0);
#وإلا
  (فراغ) dirfd;
  (فراغ) path;
  (فراغ) flags;
  (فراغ) mode;
  (فراغ) resolve;
  errno = ENOSYS;
  أرجع -1;
#endif
}

/* Process management syscalls */
ساكن مضمن صحيح
syscall_clone3 (بنية _clone3_args *args)
{
#ifdef __NR_clone3
  أرجع (صحيح) syscall (__NR_clone3, args, حجم (*args));
#وإلا
  (فراغ) args;
  errno = ENOSYS;
  أرجع -1;
#endif
}

ساكن مضمن صحيح
syscall_getcwd (حرف *path, size_t len)
{
#ifdef __NR_getcwd
  أرجع (صحيح) syscall (__NR_getcwd, path, len);
#وإلا
  (فراغ) path;
  (فراغ) len;
  errno = ENOSYS;
  أرجع -1;
#endif
}

/* Mount management syscalls */
ساكن مضمن صحيح
syscall_move_mount (صحيح from_dfd, ثابت حرف *from_pathname, صحيح to_dfd, ثابت حرف *to_pathname, unsigned صحيح flags)
{
#𐤀𐤌 defined __NR_move_mount
  أرجع (صحيح) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#وإلا
  (فراغ) from_dfd;
  (فراغ) from_pathname;
  (فراغ) to_dfd;
  (فراغ) to_pathname;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#endif
}

ساكن مضمن صحيح
syscall_open_tree (صحيح dfd, ثابت حرف *pathname, unsigned صحيح flags)
{
#𐤀𐤌 defined __NR_open_tree
  أرجع (صحيح) syscall (__NR_open_tree, dfd, pathname, flags);
#وإلا
  (فراغ) dfd;
  (فراغ) pathname;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#endif
}

ساكن مضمن صحيح
syscall_mount_setattr (صحيح dfd, ثابت حرف *path, unsigned صحيح flags, بنية mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  أرجع (صحيح) syscall (__NR_mount_setattr, dfd, path, flags, attr, حجم (*attr));
#وإلا
  (فراغ) dfd;
  (فراغ) path;
  (فراغ) flags;
  (فراغ) attr;
  errno = ENOSYS;
  أرجع -1;
#endif
}

#endif
