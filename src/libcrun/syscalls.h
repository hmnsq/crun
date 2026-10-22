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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <sys/syscall.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <stdint.h>

#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#જો defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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
#ifndef RESOLVE_IN_મૂળ
#  define RESOLVE_IN_મૂળ 0x10
#endif

/* Structures for syscalls */
સંરચના openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

સંરચના mount_attr_s
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

સંરચના _clone3_args
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 ઢગલો;
  __aligned_u64 ઢગલો_size;
  __aligned_u64 tls;
  __aligned_u64 set_tid;
  __aligned_u64 set_tid_size;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
સ્થિર અંતઃરેખ પૂર્ણાંક
syscall_fsopen (અચલ અક્ષર *fs_name, unsigned પૂર્ણાંક flags)
{
#જો defined __NR_fsopen
  પાછા (પૂર્ણાંક) syscall (__NR_fsopen, fs_name, flags);
#નહીંતર
  (શૂન્ય) fs_name;
  (શૂન્ય) flags;
  errno = ENOSYS;
  પાછા -1;
#endif
}

સ્થિર અંતઃરેખ પૂર્ણાંક
syscall_fsmount (પૂર્ણાંક fsfd, unsigned પૂર્ણાંક flags, unsigned પૂર્ણાંક attr_flags)
{
#જો defined __NR_fsmount
  પાછા (પૂર્ણાંક) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#નહીંતર
  (શૂન્ય) fsfd;
  (શૂન્ય) flags;
  (શૂન્ય) attr_flags;
  errno = ENOSYS;
  પાછા -1;
#endif
}

સ્થિર અંતઃરેખ પૂર્ણાંક
syscall_fsconfig (પૂર્ણાંક fsfd, unsigned પૂર્ણાંક cmd, અચલ અક્ષર *ચાવી, અચલ શૂન્ય *val, પૂર્ણાંક aux)
{
#જો defined __NR_fsconfig
  પાછા (પૂર્ણાંક) syscall (__NR_fsconfig, fsfd, cmd, ચાવી, val, aux);
#નહીંતર
  (શૂન્ય) fsfd;
  (શૂન્ય) cmd;
  (શૂન્ય) ચાવી;
  (શૂન્ય) val;
  (શૂન્ય) aux;
  errno = ENOSYS;
  પાછા -1;
#endif
}

/* File descriptor management syscalls */
સ્થિર અંતઃરેખ પૂર્ણાંક
syscall_close_range (unsigned પૂર્ણાંક fd, unsigned પૂર્ણાંક max_fd, unsigned પૂર્ણાંક flags)
{
#ifdef __NR_close_range
  પાછા (પૂર્ણાંક) syscall (__NR_close_range, fd, max_fd, flags);
#નહીંતર
  (શૂન્ય) fd;
  (શૂન્ય) max_fd;
  (શૂન્ય) flags;
  errno = ENOSYS;
  પાછા -1;
#endif
}

/* Secure file operation syscalls */
સ્થિર અંતઃરેખ પૂર્ણાંક
syscall_openat2 (પૂર્ણાંક dirfd, અચલ અક્ષર *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  સંરચના openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  પાછા (પૂર્ણાંક) syscall (__NR_openat2, dirfd, path, &how, કદ (how), 0);
#નહીંતર
  (શૂન્ય) dirfd;
  (શૂન્ય) path;
  (શૂન્ય) flags;
  (શૂન્ય) mode;
  (શૂન્ય) resolve;
  errno = ENOSYS;
  પાછા -1;
#endif
}

/* Process management syscalls */
સ્થિર અંતઃરેખ પૂર્ણાંક
syscall_clone3 (સંરચના _clone3_args *args)
{
#ifdef __NR_clone3
  પાછા (પૂર્ણાંક) syscall (__NR_clone3, args, કદ (*args));
#નહીંતર
  (શૂન્ય) args;
  errno = ENOSYS;
  પાછા -1;
#endif
}

સ્થિર અંતઃરેખ પૂર્ણાંક
syscall_getcwd (અક્ષર *path, size_t len)
{
#ifdef __NR_getcwd
  પાછા (પૂર્ણાંક) syscall (__NR_getcwd, path, len);
#નહીંતર
  (શૂન્ય) path;
  (શૂન્ય) len;
  errno = ENOSYS;
  પાછા -1;
#endif
}

/* Mount management syscalls */
સ્થિર અંતઃરેખ પૂર્ણાંક
syscall_move_mount (પૂર્ણાંક from_dfd, અચલ અક્ષર *from_pathname, પૂર્ણાંક to_dfd, અચલ અક્ષર *to_pathname, unsigned પૂર્ણાંક flags)
{
#જો defined __NR_move_mount
  પાછા (પૂર્ણાંક) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#નહીંતર
  (શૂન્ય) from_dfd;
  (શૂન્ય) from_pathname;
  (શૂન્ય) to_dfd;
  (શૂન્ય) to_pathname;
  (શૂન્ય) flags;
  errno = ENOSYS;
  પાછા -1;
#endif
}

સ્થિર અંતઃરેખ પૂર્ણાંક
syscall_open_વૃક્ષ (પૂર્ણાંક dfd, અચલ અક્ષર *pathname, unsigned પૂર્ણાંક flags)
{
#જો defined __NR_open_tree
  પાછા (પૂર્ણાંક) syscall (__NR_open_tree, dfd, pathname, flags);
#નહીંતર
  (શૂન્ય) dfd;
  (શૂન્ય) pathname;
  (શૂન્ય) flags;
  errno = ENOSYS;
  પાછા -1;
#endif
}

સ્થિર અંતઃરેખ પૂર્ણાંક
syscall_mount_setattr (પૂર્ણાંક dfd, અચલ અક્ષર *path, unsigned પૂર્ણાંક flags, સંરચના mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  પાછા (પૂર્ણાંક) syscall (__NR_mount_setattr, dfd, path, flags, attr, કદ (*attr));
#નહીંતર
  (શૂન્ય) dfd;
  (શૂન્ય) path;
  (શૂન્ય) flags;
  (શૂન્ય) attr;
  errno = ENOSYS;
  પાછા -1;
#endif
}

#endif
