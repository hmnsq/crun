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

#جيڪڏهن_وصف_نه_ٿيل CRUN_SYSCALLS_H
#وصف_ڪريو CRUN_SYSCALLS_H

#ضمن <config.h>
#ضمن <sys/syscall.h>
#ضمن <unistd.h>
#ضمن <errno.h>
#ضمن <stdint.h>

#جيڪڏهن_وصف_ٿيل HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#جيڪڏهن_پورو
#إذا defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#جيڪڏهن_پورو

/* Constants for new mount API */
#جيڪڏهن_وصف_نه_ٿيل FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل FSCONFIG_SET_STRING
#  define FSCONFIG_SET_STRING 1
#جيڪڏهن_پورو

/* Additional mount API constants */
#جيڪڏهن_وصف_نه_ٿيل MOUNT_ATTR_RDONLY
#  define MOUNT_ATTR_RDONLY 0x00000001 /* Mount read-only */
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MOUNT_ATTR_NOSUID
#  define MOUNT_ATTR_NOSUID 0x00000002
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MOUNT_ATTR_NODEV
#  define MOUNT_ATTR_NODEV 0x00000004
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MOUNT_ATTR_NOEXEC
#  define MOUNT_ATTR_NOEXEC 0x00000008
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MOUNT_ATTR__ATIME
#  define MOUNT_ATTR__ATIME 0x00000070
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MOUNT_ATTR_RELATIME
#  define MOUNT_ATTR_RELATIME 0x00000000
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MOUNT_ATTR_NOATIME
#  define MOUNT_ATTR_NOATIME 0x00000010
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MOUNT_ATTR_STRICTATIME
#  define MOUNT_ATTR_STRICTATIME 0x00000020
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MOUNT_ATTR_NODIRATIME
#  define MOUNT_ATTR_NODIRATIME 0x00000080
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MOUNT_ATTR_IDMAP
#  define MOUNT_ATTR_IDMAP 0x00100000 /* Idmap mount to @userns_fd in struct mount_attr. */
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MOUNT_ATTR_NOSYMFOLLOW
#  define MOUNT_ATTR_NOSYMFOLLOW 0x00200000
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MS_NOSYMFOLLOW
#  define MS_NOSYMFOLLOW 256
#جيڪڏهن_پورو

/* close_range flags */
#جيڪڏهن_وصف_نه_ٿيل CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#جيڪڏهن_پورو

/* openat2 resolve flags */
#جيڪڏهن_وصف_نه_ٿيل RESOLVE_NO_MAGICLINKS
#  define RESOLVE_NO_MAGICLINKS 0x02
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل RESOLVE_NO_SYMLINKS
#  define RESOLVE_NO_SYMLINKS 0x04
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل RESOLVE_اندريون_پاڙ
#  define RESOLVE_اندريون_پاڙ 0x10
#جيڪڏهن_پورو

/* Structures for syscalls */
بنية openat2_open_how
{
  uint64_t flags;
  uint64_t گهڻائي;
  uint64_t resolve;
};

بنية mount_attr_s
{
  uint64_t attr_set;
  uint64_t attr_clr;
  uint64_t ڦهلاء;
  uint64_t userns_fd;
};

#جيڪڏهن_وصف_نه_ٿيل __aligned_u64
#  define __aligned_u64 uint64_t __attribute__ ((aligned (8)))
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل CLONE_INTO_CGROUP
#  define CLONE_INTO_CGROUP 0x200000000ULL
#جيڪڏهن_پورو

بنية clone3_دليلون
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 ٻار_tid;
  __aligned_u64 والد_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 ڍير;
  __aligned_u64 ڍير_سائيز;
  __aligned_u64 tls;
  __aligned_u64 سيٽ_tid;
  __aligned_u64 سيٽ_tid_سائيز;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
ساكن مضمن صحيح
syscall_fsopen (ثابت حرف *fs_name, بنانشان صحيح flags)
{
#إذا defined __NR_fsopen
  أرجع (صحيح) syscall (__NR_fsopen, fs_name, flags);
#وإلا
  (فراغ) fs_name;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#جيڪڏهن_پورو
}

ساكن مضمن صحيح
syscall_fsmount (صحيح fsfd, بنانشان صحيح flags, بنانشان صحيح attr_flags)
{
#إذا defined __NR_fsmount
  أرجع (صحيح) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#وإلا
  (فراغ) fsfd;
  (فراغ) flags;
  (فراغ) attr_flags;
  errno = ENOSYS;
  أرجع -1;
#جيڪڏهن_پورو
}

ساكن مضمن صحيح
syscall_fsconfig (صحيح fsfd, بنانشان صحيح cmd, ثابت حرف *ڪنجي, ثابت فراغ *val, صحيح aux)
{
#إذا defined __NR_fsconfig
  أرجع (صحيح) syscall (__NR_fsconfig, fsfd, cmd, ڪنجي, val, aux);
#وإلا
  (فراغ) fsfd;
  (فراغ) cmd;
  (فراغ) ڪنجي;
  (فراغ) val;
  (فراغ) aux;
  errno = ENOSYS;
  أرجع -1;
#جيڪڏهن_پورو
}

/* File descriptor management syscalls */
ساكن مضمن صحيح
syscall_close_range (بنانشان صحيح fd, بنانشان صحيح max_fd, بنانشان صحيح flags)
{
#جيڪڏهن_وصف_ٿيل __NR_close_range
  أرجع (صحيح) syscall (__NR_close_range, fd, max_fd, flags);
#وإلا
  (فراغ) fd;
  (فراغ) max_fd;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#جيڪڏهن_پورو
}

/* Secure file operation syscalls */
ساكن مضمن صحيح
syscall_openat2 (صحيح dirfd, ثابت حرف *رستو, uint64_t flags, uint64_t گهڻائي, uint64_t resolve)
{
#جيڪڏهن_وصف_ٿيل __NR_openat2
  بنية openat2_open_how how = {
    .flags = flags,
    .گهڻائي = گهڻائي,
    .resolve = resolve,
  };
  أرجع (صحيح) syscall (__NR_openat2, dirfd, رستو, &how, حجم (how), 0);
#وإلا
  (فراغ) dirfd;
  (فراغ) رستو;
  (فراغ) flags;
  (فراغ) گهڻائي;
  (فراغ) resolve;
  errno = ENOSYS;
  أرجع -1;
#جيڪڏهن_پورو
}

/* Process management syscalls */
ساكن مضمن صحيح
syscall_clone3 (بنية clone3_دليلون *دليلون)
{
#جيڪڏهن_وصف_ٿيل __NR_clone3
  أرجع (صحيح) syscall (__NR_clone3, دليلون, حجم (*دليلون));
#وإلا
  (فراغ) دليلون;
  errno = ENOSYS;
  أرجع -1;
#جيڪڏهن_پورو
}

ساكن مضمن صحيح
syscall_getcwd (حرف *رستو, size_t len)
{
#جيڪڏهن_وصف_ٿيل __NR_getcwd
  أرجع (صحيح) syscall (__NR_getcwd, رستو, len);
#وإلا
  (فراغ) رستو;
  (فراغ) len;
  errno = ENOSYS;
  أرجع -1;
#جيڪڏهن_پورو
}

/* Mount management syscalls */
ساكن مضمن صحيح
syscall_move_mount (صحيح from_dfd, ثابت حرف *from_pathname, صحيح منزل_dfd, ثابت حرف *منزل_pathname, بنانشان صحيح flags)
{
#إذا defined __NR_move_mount
  أرجع (صحيح) syscall (__NR_move_mount, from_dfd, from_pathname, منزل_dfd, منزل_pathname, flags);
#وإلا
  (فراغ) from_dfd;
  (فراغ) from_pathname;
  (فراغ) منزل_dfd;
  (فراغ) منزل_pathname;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#جيڪڏهن_پورو
}

ساكن مضمن صحيح
syscall_open_وڻ (صحيح dfd, ثابت حرف *pathname, بنانشان صحيح flags)
{
#إذا defined __NR_open_tree
  أرجع (صحيح) syscall (__NR_open_tree, dfd, pathname, flags);
#وإلا
  (فراغ) dfd;
  (فراغ) pathname;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#جيڪڏهن_پورو
}

ساكن مضمن صحيح
syscall_mount_setattr (صحيح dfd, ثابت حرف *رستو, بنانشان صحيح flags, بنية mount_attr_s *attr)
{
#جيڪڏهن_وصف_ٿيل __NR_mount_setattr
  أرجع (صحيح) syscall (__NR_mount_setattr, dfd, رستو, flags, attr, حجم (*attr));
#وإلا
  (فراغ) dfd;
  (فراغ) رستو;
  (فراغ) flags;
  (فراغ) attr;
  errno = ENOSYS;
  أرجع -1;
#جيڪڏهن_پورو
}

#جيڪڏهن_پورو
