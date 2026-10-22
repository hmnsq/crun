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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <sys/syscall.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <stdint.h>

#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#എങ്കിൽ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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
#ifndef RESOLVE_IN_വേര്
#  define RESOLVE_IN_വേര് 0x10
#endif

/* Structures for syscalls */
ഘടന openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

ഘടന mount_attr_s
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

ഘടന _clone3_args
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 അട്ടി;
  __aligned_u64 അട്ടി_size;
  __aligned_u64 tls;
  __aligned_u64 set_tid;
  __aligned_u64 set_tid_size;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
syscall_fsopen (സ്ഥിരം അക്ഷരം *fs_name, unsigned പൂർണ്ണസംഖ്യ flags)
{
#എങ്കിൽ defined __NR_fsopen
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_fsopen, fs_name, flags);
#അല്ലെങ്കിൽ
  (ശൂന്യം) fs_name;
  (ശൂന്യം) flags;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
syscall_fsmount (പൂർണ്ണസംഖ്യ fsfd, unsigned പൂർണ്ണസംഖ്യ flags, unsigned പൂർണ്ണസംഖ്യ attr_flags)
{
#എങ്കിൽ defined __NR_fsmount
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#അല്ലെങ്കിൽ
  (ശൂന്യം) fsfd;
  (ശൂന്യം) flags;
  (ശൂന്യം) attr_flags;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
syscall_fsconfig (പൂർണ്ണസംഖ്യ fsfd, unsigned പൂർണ്ണസംഖ്യ cmd, സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം ശൂന്യം *val, പൂർണ്ണസംഖ്യ aux)
{
#എങ്കിൽ defined __NR_fsconfig
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_fsconfig, fsfd, cmd, താക്കോൽ, val, aux);
#അല്ലെങ്കിൽ
  (ശൂന്യം) fsfd;
  (ശൂന്യം) cmd;
  (ശൂന്യം) താക്കോൽ;
  (ശൂന്യം) val;
  (ശൂന്യം) aux;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

/* File descriptor management syscalls */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
syscall_close_range (unsigned പൂർണ്ണസംഖ്യ fd, unsigned പൂർണ്ണസംഖ്യ max_fd, unsigned പൂർണ്ണസംഖ്യ flags)
{
#ifdef __NR_close_range
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_close_range, fd, max_fd, flags);
#അല്ലെങ്കിൽ
  (ശൂന്യം) fd;
  (ശൂന്യം) max_fd;
  (ശൂന്യം) flags;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

/* Secure file operation syscalls */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
syscall_openat2 (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  ഘടന openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_openat2, dirfd, path, &how, വലുപ്പം (how), 0);
#അല്ലെങ്കിൽ
  (ശൂന്യം) dirfd;
  (ശൂന്യം) path;
  (ശൂന്യം) flags;
  (ശൂന്യം) mode;
  (ശൂന്യം) resolve;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

/* Process management syscalls */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
syscall_clone3 (ഘടന _clone3_args *args)
{
#ifdef __NR_clone3
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_clone3, args, വലുപ്പം (*args));
#അല്ലെങ്കിൽ
  (ശൂന്യം) args;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
syscall_getcwd (അക്ഷരം *path, size_t len)
{
#ifdef __NR_getcwd
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_getcwd, path, len);
#അല്ലെങ്കിൽ
  (ശൂന്യം) path;
  (ശൂന്യം) len;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

/* Mount management syscalls */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
syscall_move_mount (പൂർണ്ണസംഖ്യ from_dfd, സ്ഥിരം അക്ഷരം *from_pathname, പൂർണ്ണസംഖ്യ to_dfd, സ്ഥിരം അക്ഷരം *to_pathname, unsigned പൂർണ്ണസംഖ്യ flags)
{
#എങ്കിൽ defined __NR_move_mount
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#അല്ലെങ്കിൽ
  (ശൂന്യം) from_dfd;
  (ശൂന്യം) from_pathname;
  (ശൂന്യം) to_dfd;
  (ശൂന്യം) to_pathname;
  (ശൂന്യം) flags;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
syscall_open_വൃക്ഷം (പൂർണ്ണസംഖ്യ dfd, സ്ഥിരം അക്ഷരം *pathname, unsigned പൂർണ്ണസംഖ്യ flags)
{
#എങ്കിൽ defined __NR_open_tree
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_open_tree, dfd, pathname, flags);
#അല്ലെങ്കിൽ
  (ശൂന്യം) dfd;
  (ശൂന്യം) pathname;
  (ശൂന്യം) flags;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
syscall_mount_setattr (പൂർണ്ണസംഖ്യ dfd, സ്ഥിരം അക്ഷരം *path, unsigned പൂർണ്ണസംഖ്യ flags, ഘടന mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_mount_setattr, dfd, path, flags, attr, വലുപ്പം (*attr));
#അല്ലെങ്കിൽ
  (ശൂന്യം) dfd;
  (ശൂന്യം) path;
  (ശൂന്യം) flags;
  (ശൂന്യം) attr;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

#endif
