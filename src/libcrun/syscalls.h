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
#အကယ်၍ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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
sa_nracanaa openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

sa_nracanaa mount_attr_s
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

sa_nracanaa _clone3_args
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
static anta_hstha ကိန်းပြည့်
syscall_fsopen (const var_na *fs_name, unsigned ကိန်းပြည့် flags)
{
#အကယ်၍ defined __NR_fsopen
  ပြန်ပေး (ကိန်းပြည့်) syscall (__NR_fsopen, fs_name, flags);
#မဟုတ်ပါက
  (void) fs_name;
  (void) flags;
  errno = ENOSYS;
  ပြန်ပေး -1;
#endif
}

static anta_hstha ကိန်းပြည့်
syscall_fsmount (ကိန်းပြည့် fsfd, unsigned ကိန်းပြည့် flags, unsigned ကိန်းပြည့် attr_flags)
{
#အကယ်၍ defined __NR_fsmount
  ပြန်ပေး (ကိန်းပြည့်) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#မဟုတ်ပါက
  (void) fsfd;
  (void) flags;
  (void) attr_flags;
  errno = ENOSYS;
  ပြန်ပေး -1;
#endif
}

static anta_hstha ကိန်းပြည့်
syscall_fsconfig (ကိန်းပြည့် fsfd, unsigned ကိန်းပြည့် cmd, const var_na *key, const void *val, ကိန်းပြည့် aux)
{
#အကယ်၍ defined __NR_fsconfig
  ပြန်ပေး (ကိန်းပြည့်) syscall (__NR_fsconfig, fsfd, cmd, key, val, aux);
#မဟုတ်ပါက
  (void) fsfd;
  (void) cmd;
  (void) key;
  (void) val;
  (void) aux;
  errno = ENOSYS;
  ပြန်ပေး -1;
#endif
}

/* File descriptor management syscalls */
static anta_hstha ကိန်းပြည့်
syscall_close_range (unsigned ကိန်းပြည့် fd, unsigned ကိန်းပြည့် max_fd, unsigned ကိန်းပြည့် flags)
{
#ifdef __NR_close_range
  ပြန်ပေး (ကိန်းပြည့်) syscall (__NR_close_range, fd, max_fd, flags);
#မဟုတ်ပါက
  (void) fd;
  (void) max_fd;
  (void) flags;
  errno = ENOSYS;
  ပြန်ပေး -1;
#endif
}

/* Secure file operation syscalls */
static anta_hstha ကိန်းပြည့်
syscall_openat2 (ကိန်းပြည့် dirfd, const var_na *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  sa_nracanaa openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  ပြန်ပေး (ကိန်းပြည့်) syscall (__NR_openat2, dirfd, path, &how, parimaa_na (how), 0);
#မဟုတ်ပါက
  (void) dirfd;
  (void) path;
  (void) flags;
  (void) mode;
  (void) resolve;
  errno = ENOSYS;
  ပြန်ပေး -1;
#endif
}

/* Process management syscalls */
static anta_hstha ကိန်းပြည့်
syscall_clone3 (sa_nracanaa _clone3_args *args)
{
#ifdef __NR_clone3
  ပြန်ပေး (ကိန်းပြည့်) syscall (__NR_clone3, args, parimaa_na (*args));
#မဟုတ်ပါက
  (void) args;
  errno = ENOSYS;
  ပြန်ပေး -1;
#endif
}

static anta_hstha ကိန်းပြည့်
syscall_getcwd (var_na *path, size_t len)
{
#ifdef __NR_getcwd
  ပြန်ပေး (ကိန်းပြည့်) syscall (__NR_getcwd, path, len);
#မဟုတ်ပါက
  (void) path;
  (void) len;
  errno = ENOSYS;
  ပြန်ပေး -1;
#endif
}

/* Mount management syscalls */
static anta_hstha ကိန်းပြည့်
syscall_move_mount (ကိန်းပြည့် from_dfd, const var_na *from_pathname, ကိန်းပြည့် to_dfd, const var_na *to_pathname, unsigned ကိန်းပြည့် flags)
{
#အကယ်၍ defined __NR_move_mount
  ပြန်ပေး (ကိန်းပြည့်) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#မဟုတ်ပါက
  (void) from_dfd;
  (void) from_pathname;
  (void) to_dfd;
  (void) to_pathname;
  (void) flags;
  errno = ENOSYS;
  ပြန်ပေး -1;
#endif
}

static anta_hstha ကိန်းပြည့်
syscall_open_tree (ကိန်းပြည့် dfd, const var_na *pathname, unsigned ကိန်းပြည့် flags)
{
#အကယ်၍ defined __NR_open_tree
  ပြန်ပေး (ကိန်းပြည့်) syscall (__NR_open_tree, dfd, pathname, flags);
#မဟုတ်ပါက
  (void) dfd;
  (void) pathname;
  (void) flags;
  errno = ENOSYS;
  ပြန်ပေး -1;
#endif
}

static anta_hstha ကိန်းပြည့်
syscall_mount_setattr (ကိန်းပြည့် dfd, const var_na *path, unsigned ကိန်းပြည့် flags, sa_nracanaa mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  ပြန်ပေး (ကိန်းပြည့်) syscall (__NR_mount_setattr, dfd, path, flags, attr, parimaa_na (*attr));
#မဟုတ်ပါက
  (void) dfd;
  (void) path;
  (void) flags;
  (void) attr;
  errno = ENOSYS;
  ပြန်ပေး -1;
#endif
}

#endif
