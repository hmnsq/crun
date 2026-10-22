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

#include <config.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#nếu defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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
struct openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

struct mount_attr_s
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

struct _clone3_args
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
static inline số_nguyên
syscall_fsopen (const char *fs_name, unsigned số_nguyên flags)
{
#nếu defined __NR_fsopen
  trả_về (số_nguyên) syscall (__NR_fsopen, fs_name, flags);
#không_thì
  (void) fs_name;
  (void) flags;
  errno = ENOSYS;
  trả_về -1;
#endif
}

static inline số_nguyên
syscall_fsmount (số_nguyên fsfd, unsigned số_nguyên flags, unsigned số_nguyên attr_flags)
{
#nếu defined __NR_fsmount
  trả_về (số_nguyên) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#không_thì
  (void) fsfd;
  (void) flags;
  (void) attr_flags;
  errno = ENOSYS;
  trả_về -1;
#endif
}

static inline số_nguyên
syscall_fsconfig (số_nguyên fsfd, unsigned số_nguyên cmd, const char *key, const void *val, số_nguyên aux)
{
#nếu defined __NR_fsconfig
  trả_về (số_nguyên) syscall (__NR_fsconfig, fsfd, cmd, key, val, aux);
#không_thì
  (void) fsfd;
  (void) cmd;
  (void) key;
  (void) val;
  (void) aux;
  errno = ENOSYS;
  trả_về -1;
#endif
}

/* File descriptor management syscalls */
static inline số_nguyên
syscall_close_range (unsigned số_nguyên fd, unsigned số_nguyên max_fd, unsigned số_nguyên flags)
{
#ifdef __NR_close_range
  trả_về (số_nguyên) syscall (__NR_close_range, fd, max_fd, flags);
#không_thì
  (void) fd;
  (void) max_fd;
  (void) flags;
  errno = ENOSYS;
  trả_về -1;
#endif
}

/* Secure file operation syscalls */
static inline số_nguyên
syscall_openat2 (số_nguyên dirfd, const char *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  struct openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  trả_về (số_nguyên) syscall (__NR_openat2, dirfd, path, &how, sizeof (how), 0);
#không_thì
  (void) dirfd;
  (void) path;
  (void) flags;
  (void) mode;
  (void) resolve;
  errno = ENOSYS;
  trả_về -1;
#endif
}

/* Process management syscalls */
static inline số_nguyên
syscall_clone3 (struct _clone3_args *args)
{
#ifdef __NR_clone3
  trả_về (số_nguyên) syscall (__NR_clone3, args, sizeof (*args));
#không_thì
  (void) args;
  errno = ENOSYS;
  trả_về -1;
#endif
}

static inline số_nguyên
syscall_getcwd (char *path, size_t len)
{
#ifdef __NR_getcwd
  trả_về (số_nguyên) syscall (__NR_getcwd, path, len);
#không_thì
  (void) path;
  (void) len;
  errno = ENOSYS;
  trả_về -1;
#endif
}

/* Mount management syscalls */
static inline số_nguyên
syscall_move_mount (số_nguyên from_dfd, const char *from_pathname, số_nguyên to_dfd, const char *to_pathname, unsigned số_nguyên flags)
{
#nếu defined __NR_move_mount
  trả_về (số_nguyên) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#không_thì
  (void) from_dfd;
  (void) from_pathname;
  (void) to_dfd;
  (void) to_pathname;
  (void) flags;
  errno = ENOSYS;
  trả_về -1;
#endif
}

static inline số_nguyên
syscall_open_tree (số_nguyên dfd, const char *pathname, unsigned số_nguyên flags)
{
#nếu defined __NR_open_tree
  trả_về (số_nguyên) syscall (__NR_open_tree, dfd, pathname, flags);
#không_thì
  (void) dfd;
  (void) pathname;
  (void) flags;
  errno = ENOSYS;
  trả_về -1;
#endif
}

static inline số_nguyên
syscall_mount_setattr (số_nguyên dfd, const char *path, unsigned số_nguyên flags, struct mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  trả_về (số_nguyên) syscall (__NR_mount_setattr, dfd, path, flags, attr, sizeof (*attr));
#không_thì
  (void) dfd;
  (void) path;
  (void) flags;
  (void) attr;
  errno = ENOSYS;
  trả_về -1;
#endif
}

#endif
