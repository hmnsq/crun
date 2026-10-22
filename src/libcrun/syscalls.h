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

#اگر_تعریف_نشده CRUN_SYSCALLS_H
#تعریف_کن CRUN_SYSCALLS_H

#ضمن <config.h>
#ضمن <sys/syscall.h>
#ضمن <unistd.h>
#ضمن <errno.h>
#ضمن <stdint.h>

#اگر_تعریف_شده HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#پایان_اگر
#إذا defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#پایان_اگر

/* Constants for new mount API */
#اگر_تعریف_نشده FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#پایان_اگر

#اگر_تعریف_نشده FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#پایان_اگر

#اگر_تعریف_نشده FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#پایان_اگر

#اگر_تعریف_نشده FSCONFIG_SET_STRING
#  define FSCONFIG_SET_STRING 1
#پایان_اگر

/* Additional mount API constants */
#اگر_تعریف_نشده MOUNT_ATTR_RDONLY
#  define MOUNT_ATTR_RDONLY 0x00000001 /* Mount read-only */
#پایان_اگر

#اگر_تعریف_نشده MOUNT_ATTR_NOSUID
#  define MOUNT_ATTR_NOSUID 0x00000002
#پایان_اگر

#اگر_تعریف_نشده MOUNT_ATTR_NODEV
#  define MOUNT_ATTR_NODEV 0x00000004
#پایان_اگر

#اگر_تعریف_نشده MOUNT_ATTR_NOEXEC
#  define MOUNT_ATTR_NOEXEC 0x00000008
#پایان_اگر

#اگر_تعریف_نشده MOUNT_ATTR__ATIME
#  define MOUNT_ATTR__ATIME 0x00000070
#پایان_اگر

#اگر_تعریف_نشده MOUNT_ATTR_RELATIME
#  define MOUNT_ATTR_RELATIME 0x00000000
#پایان_اگر

#اگر_تعریف_نشده MOUNT_ATTR_NOATIME
#  define MOUNT_ATTR_NOATIME 0x00000010
#پایان_اگر

#اگر_تعریف_نشده MOUNT_ATTR_STRICTATIME
#  define MOUNT_ATTR_STRICTATIME 0x00000020
#پایان_اگر

#اگر_تعریف_نشده MOUNT_ATTR_NODIRATIME
#  define MOUNT_ATTR_NODIRATIME 0x00000080
#پایان_اگر

#اگر_تعریف_نشده MOUNT_ATTR_IDMAP
#  define MOUNT_ATTR_IDMAP 0x00100000 /* Idmap mount to @userns_fd in struct mount_attr. */
#پایان_اگر

#اگر_تعریف_نشده MOUNT_ATTR_NOSYMFOLLOW
#  define MOUNT_ATTR_NOSYMFOLLOW 0x00200000
#پایان_اگر

#اگر_تعریف_نشده MS_NOSYMFOLLOW
#  define MS_NOSYMFOLLOW 256
#پایان_اگر

/* close_range flags */
#اگر_تعریف_نشده CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#پایان_اگر

/* openat2 resolve flags */
#اگر_تعریف_نشده RESOLVE_NO_MAGICLINKS
#  define RESOLVE_NO_MAGICLINKS 0x02
#پایان_اگر
#اگر_تعریف_نشده RESOLVE_NO_SYMLINKS
#  define RESOLVE_NO_SYMLINKS 0x04
#پایان_اگر
#اگر_تعریف_نشده RESOLVE_درون_ریشه
#  define RESOLVE_درون_ریشه 0x10
#پایان_اگر

/* Structures for syscalls */
بنية openat2_open_how
{
  uint64_t flags;
  uint64_t نما;
  uint64_t resolve;
};

بنية mount_attr_s
{
  uint64_t attr_set;
  uint64_t attr_clr;
  uint64_t انتشار;
  uint64_t userns_fd;
};

#اگر_تعریف_نشده __aligned_u64
#  define __aligned_u64 uint64_t __attribute__ ((aligned (8)))
#پایان_اگر

#اگر_تعریف_نشده CLONE_INTO_CGROUP
#  define CLONE_INTO_CGROUP 0x200000000ULL
#پایان_اگر

بنية clone3_آرگومانها
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 فرزند_tid;
  __aligned_u64 والد_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 پشته;
  __aligned_u64 پشته_اندازه;
  __aligned_u64 tls;
  __aligned_u64 مجموعه_tid;
  __aligned_u64 مجموعه_tid_اندازه;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
ساكن مضمن صحيح
syscall_fsopen (ثابت حرف *fs_name, بیعلامت صحيح flags)
{
#إذا defined __NR_fsopen
  أرجع (صحيح) syscall (__NR_fsopen, fs_name, flags);
#وإلا
  (فراغ) fs_name;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#پایان_اگر
}

ساكن مضمن صحيح
syscall_fsmount (صحيح fsfd, بیعلامت صحيح flags, بیعلامت صحيح attr_flags)
{
#إذا defined __NR_fsmount
  أرجع (صحيح) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#وإلا
  (فراغ) fsfd;
  (فراغ) flags;
  (فراغ) attr_flags;
  errno = ENOSYS;
  أرجع -1;
#پایان_اگر
}

ساكن مضمن صحيح
syscall_fsconfig (صحيح fsfd, بیعلامت صحيح cmd, ثابت حرف *کلید, ثابت فراغ *val, صحيح aux)
{
#إذا defined __NR_fsconfig
  أرجع (صحيح) syscall (__NR_fsconfig, fsfd, cmd, کلید, val, aux);
#وإلا
  (فراغ) fsfd;
  (فراغ) cmd;
  (فراغ) کلید;
  (فراغ) val;
  (فراغ) aux;
  errno = ENOSYS;
  أرجع -1;
#پایان_اگر
}

/* File descriptor management syscalls */
ساكن مضمن صحيح
syscall_close_range (بیعلامت صحيح fd, بیعلامت صحيح max_fd, بیعلامت صحيح flags)
{
#اگر_تعریف_شده __NR_close_range
  أرجع (صحيح) syscall (__NR_close_range, fd, max_fd, flags);
#وإلا
  (فراغ) fd;
  (فراغ) max_fd;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#پایان_اگر
}

/* Secure file operation syscalls */
ساكن مضمن صحيح
syscall_openat2 (صحيح dirfd, ثابت حرف *مسیر, uint64_t flags, uint64_t نما, uint64_t resolve)
{
#اگر_تعریف_شده __NR_openat2
  بنية openat2_open_how how = {
    .flags = flags,
    .نما = نما,
    .resolve = resolve,
  };
  أرجع (صحيح) syscall (__NR_openat2, dirfd, مسیر, &how, حجم (how), 0);
#وإلا
  (فراغ) dirfd;
  (فراغ) مسیر;
  (فراغ) flags;
  (فراغ) نما;
  (فراغ) resolve;
  errno = ENOSYS;
  أرجع -1;
#پایان_اگر
}

/* Process management syscalls */
ساكن مضمن صحيح
syscall_clone3 (بنية clone3_آرگومانها *آرگومانها)
{
#اگر_تعریف_شده __NR_clone3
  أرجع (صحيح) syscall (__NR_clone3, آرگومانها, حجم (*آرگومانها));
#وإلا
  (فراغ) آرگومانها;
  errno = ENOSYS;
  أرجع -1;
#پایان_اگر
}

ساكن مضمن صحيح
syscall_getcwd (حرف *مسیر, size_t len)
{
#اگر_تعریف_شده __NR_getcwd
  أرجع (صحيح) syscall (__NR_getcwd, مسیر, len);
#وإلا
  (فراغ) مسیر;
  (فراغ) len;
  errno = ENOSYS;
  أرجع -1;
#پایان_اگر
}

/* Mount management syscalls */
ساكن مضمن صحيح
syscall_move_mount (صحيح from_dfd, ثابت حرف *from_pathname, صحيح مقصد_dfd, ثابت حرف *مقصد_pathname, بیعلامت صحيح flags)
{
#إذا defined __NR_move_mount
  أرجع (صحيح) syscall (__NR_move_mount, from_dfd, from_pathname, مقصد_dfd, مقصد_pathname, flags);
#وإلا
  (فراغ) from_dfd;
  (فراغ) from_pathname;
  (فراغ) مقصد_dfd;
  (فراغ) مقصد_pathname;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#پایان_اگر
}

ساكن مضمن صحيح
syscall_open_درخت (صحيح dfd, ثابت حرف *pathname, بیعلامت صحيح flags)
{
#إذا defined __NR_open_tree
  أرجع (صحيح) syscall (__NR_open_tree, dfd, pathname, flags);
#وإلا
  (فراغ) dfd;
  (فراغ) pathname;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#پایان_اگر
}

ساكن مضمن صحيح
syscall_mount_setattr (صحيح dfd, ثابت حرف *مسیر, بیعلامت صحيح flags, بنية mount_attr_s *attr)
{
#اگر_تعریف_شده __NR_mount_setattr
  أرجع (صحيح) syscall (__NR_mount_setattr, dfd, مسیر, flags, attr, حجم (*attr));
#وإلا
  (فراغ) dfd;
  (فراغ) مسیر;
  (فراغ) flags;
  (فراغ) attr;
  errno = ENOSYS;
  أرجع -1;
#پایان_اگر
}

#پایان_اگر
