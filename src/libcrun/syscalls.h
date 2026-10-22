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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#ಯದಿ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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
#ifndef RESOLVE_IN_ಬೇರು
#  define RESOLVE_IN_ಬೇರು 0x10
#endif

/* Structures for syscalls */
ಸಂರಚನಾ openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

ಸಂರಚನಾ mount_attr_s
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

ಸಂರಚನಾ _clone3_args
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 ರಾಶಿ;
  __aligned_u64 ರಾಶಿ_size;
  __aligned_u64 tls;
  __aligned_u64 set_tid;
  __aligned_u64 set_tid_size;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
syscall_fsopen (ಧ್ರುವ ವರ್ಣ *fs_name, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
#ಯದಿ defined __NR_fsopen
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_fsopen, fs_name, flags);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) fs_name;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
syscall_fsmount (ಪೂರ್ಣಾಙ್ಕ fsfd, unsigned ಪೂರ್ಣಾಙ್ಕ flags, unsigned ಪೂರ್ಣಾಙ್ಕ attr_flags)
{
#ಯದಿ defined __NR_fsmount
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) fsfd;
  (ಶೂನ್ಯ) flags;
  (ಶೂನ್ಯ) attr_flags;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
syscall_fsconfig (ಪೂರ್ಣಾಙ್ಕ fsfd, unsigned ಪೂರ್ಣಾಙ್ಕ cmd, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ಶೂನ್ಯ *val, ಪೂರ್ಣಾಙ್ಕ aux)
{
#ಯದಿ defined __NR_fsconfig
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_fsconfig, fsfd, cmd, ಕೀಲಿ, val, aux);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) fsfd;
  (ಶೂನ್ಯ) cmd;
  (ಶೂನ್ಯ) ಕೀಲಿ;
  (ಶೂನ್ಯ) val;
  (ಶೂನ್ಯ) aux;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

/* File descriptor management syscalls */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
syscall_close_range (unsigned ಪೂರ್ಣಾಙ್ಕ fd, unsigned ಪೂರ್ಣಾಙ್ಕ max_fd, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
#ifdef __NR_close_range
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_close_range, fd, max_fd, flags);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) fd;
  (ಶೂನ್ಯ) max_fd;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

/* Secure file operation syscalls */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
syscall_openat2 (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  ಸಂರಚನಾ openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_openat2, dirfd, path, &how, ಪರಿಮಾಣ (how), 0);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) dirfd;
  (ಶೂನ್ಯ) path;
  (ಶೂನ್ಯ) flags;
  (ಶೂನ್ಯ) mode;
  (ಶೂನ್ಯ) resolve;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

/* Process management syscalls */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
syscall_clone3 (ಸಂರಚನಾ _clone3_args *args)
{
#ifdef __NR_clone3
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_clone3, args, ಪರಿಮಾಣ (*args));
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) args;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
syscall_getcwd (ವರ್ಣ *path, size_t len)
{
#ifdef __NR_getcwd
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_getcwd, path, len);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) path;
  (ಶೂನ್ಯ) len;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

/* Mount management syscalls */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
syscall_move_mount (ಪೂರ್ಣಾಙ್ಕ from_dfd, ಧ್ರುವ ವರ್ಣ *from_pathname, ಪೂರ್ಣಾಙ್ಕ to_dfd, ಧ್ರುವ ವರ್ಣ *to_pathname, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
#ಯದಿ defined __NR_move_mount
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) from_dfd;
  (ಶೂನ್ಯ) from_pathname;
  (ಶೂನ್ಯ) to_dfd;
  (ಶೂನ್ಯ) to_pathname;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
syscall_open_ವೃಕ್ಷ (ಪೂರ್ಣಾಙ್ಕ dfd, ಧ್ರುವ ವರ್ಣ *pathname, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
#ಯದಿ defined __NR_open_tree
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_open_tree, dfd, pathname, flags);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) dfd;
  (ಶೂನ್ಯ) pathname;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
syscall_mount_setattr (ಪೂರ್ಣಾಙ್ಕ dfd, ಧ್ರುವ ವರ್ಣ *path, unsigned ಪೂರ್ಣಾಙ್ಕ flags, ಸಂರಚನಾ mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_mount_setattr, dfd, path, flags, attr, ಪರಿಮಾಣ (*attr));
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) dfd;
  (ಶೂನ್ಯ) path;
  (ಶೂನ್ಯ) flags;
  (ಶೂನ್ಯ) attr;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

#endif
