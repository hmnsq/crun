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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <sys/syscall.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <stdint.h>

#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#எனில் defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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
#ifndef RESOLVE_IN_வேர்
#  define RESOLVE_IN_வேர் 0x10
#endif

/* Structures for syscalls */
கட்டமைப்பு openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

கட்டமைப்பு mount_attr_s
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

கட்டமைப்பு _clone3_args
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 அடுக்கு;
  __aligned_u64 அடுக்கு_size;
  __aligned_u64 tls;
  __aligned_u64 set_tid;
  __aligned_u64 set_tid_size;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
நிலையான உள்வரி முழு
syscall_fsopen (மாறிலி எழுத்து *fs_name, unsigned முழு flags)
{
#எனில் defined __NR_fsopen
  திரும்பு (முழு) syscall (__NR_fsopen, fs_name, flags);
#இல்லையெனில்
  (வெறுமை) fs_name;
  (வெறுமை) flags;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

நிலையான உள்வரி முழு
syscall_fsmount (முழு fsfd, unsigned முழு flags, unsigned முழு attr_flags)
{
#எனில் defined __NR_fsmount
  திரும்பு (முழு) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#இல்லையெனில்
  (வெறுமை) fsfd;
  (வெறுமை) flags;
  (வெறுமை) attr_flags;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

நிலையான உள்வரி முழு
syscall_fsconfig (முழு fsfd, unsigned முழு cmd, மாறிலி எழுத்து *சாவி, மாறிலி வெறுமை *val, முழு aux)
{
#எனில் defined __NR_fsconfig
  திரும்பு (முழு) syscall (__NR_fsconfig, fsfd, cmd, சாவி, val, aux);
#இல்லையெனில்
  (வெறுமை) fsfd;
  (வெறுமை) cmd;
  (வெறுமை) சாவி;
  (வெறுமை) val;
  (வெறுமை) aux;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

/* File descriptor management syscalls */
நிலையான உள்வரி முழு
syscall_close_range (unsigned முழு fd, unsigned முழு max_fd, unsigned முழு flags)
{
#ifdef __NR_close_range
  திரும்பு (முழு) syscall (__NR_close_range, fd, max_fd, flags);
#இல்லையெனில்
  (வெறுமை) fd;
  (வெறுமை) max_fd;
  (வெறுமை) flags;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

/* Secure file operation syscalls */
நிலையான உள்வரி முழு
syscall_openat2 (முழு dirfd, மாறிலி எழுத்து *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  கட்டமைப்பு openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  திரும்பு (முழு) syscall (__NR_openat2, dirfd, path, &how, அளவு (how), 0);
#இல்லையெனில்
  (வெறுமை) dirfd;
  (வெறுமை) path;
  (வெறுமை) flags;
  (வெறுமை) mode;
  (வெறுமை) resolve;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

/* Process management syscalls */
நிலையான உள்வரி முழு
syscall_clone3 (கட்டமைப்பு _clone3_args *args)
{
#ifdef __NR_clone3
  திரும்பு (முழு) syscall (__NR_clone3, args, அளவு (*args));
#இல்லையெனில்
  (வெறுமை) args;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

நிலையான உள்வரி முழு
syscall_getcwd (எழுத்து *path, size_t len)
{
#ifdef __NR_getcwd
  திரும்பு (முழு) syscall (__NR_getcwd, path, len);
#இல்லையெனில்
  (வெறுமை) path;
  (வெறுமை) len;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

/* Mount management syscalls */
நிலையான உள்வரி முழு
syscall_move_mount (முழு from_dfd, மாறிலி எழுத்து *from_pathname, முழு to_dfd, மாறிலி எழுத்து *to_pathname, unsigned முழு flags)
{
#எனில் defined __NR_move_mount
  திரும்பு (முழு) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#இல்லையெனில்
  (வெறுமை) from_dfd;
  (வெறுமை) from_pathname;
  (வெறுமை) to_dfd;
  (வெறுமை) to_pathname;
  (வெறுமை) flags;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

நிலையான உள்வரி முழு
syscall_open_மரம் (முழு dfd, மாறிலி எழுத்து *pathname, unsigned முழு flags)
{
#எனில் defined __NR_open_tree
  திரும்பு (முழு) syscall (__NR_open_tree, dfd, pathname, flags);
#இல்லையெனில்
  (வெறுமை) dfd;
  (வெறுமை) pathname;
  (வெறுமை) flags;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

நிலையான உள்வரி முழு
syscall_mount_setattr (முழு dfd, மாறிலி எழுத்து *path, unsigned முழு flags, கட்டமைப்பு mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  திரும்பு (முழு) syscall (__NR_mount_setattr, dfd, path, flags, attr, அளவு (*attr));
#இல்லையெனில்
  (வெறுமை) dfd;
  (வெறுமை) path;
  (வெறுமை) flags;
  (வெறுமை) attr;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

#endif
