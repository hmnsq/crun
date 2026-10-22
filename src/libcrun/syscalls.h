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

#که_نه_تعریف_شوی CRUN_SYSCALLS_H
#تعریف_کړه CRUN_SYSCALLS_H

#ضمن <config.h>
#ضمن <sys/syscall.h>
#ضمن <unistd.h>
#ضمن <errno.h>
#ضمن <stdint.h>

#که_تعریف_شوی HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#د_که_پای
#إذا defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#د_که_پای

/* Constants for new mount API */
#که_نه_تعریف_شوی FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#د_که_پای

#که_نه_تعریف_شوی FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#د_که_پای

#که_نه_تعریف_شوی FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#د_که_پای

#که_نه_تعریف_شوی FSCONFIG_SET_STRING
#  define FSCONFIG_SET_STRING 1
#د_که_پای

/* Additional mount API constants */
#که_نه_تعریف_شوی MOUNT_ATTR_RDONLY
#  define MOUNT_ATTR_RDONLY 0x00000001 /* Mount read-only */
#د_که_پای

#که_نه_تعریف_شوی MOUNT_ATTR_NOSUID
#  define MOUNT_ATTR_NOSUID 0x00000002
#د_که_پای

#که_نه_تعریف_شوی MOUNT_ATTR_NODEV
#  define MOUNT_ATTR_NODEV 0x00000004
#د_که_پای

#که_نه_تعریف_شوی MOUNT_ATTR_NOEXEC
#  define MOUNT_ATTR_NOEXEC 0x00000008
#د_که_پای

#که_نه_تعریف_شوی MOUNT_ATTR__ATIME
#  define MOUNT_ATTR__ATIME 0x00000070
#د_که_پای

#که_نه_تعریف_شوی MOUNT_ATTR_RELATIME
#  define MOUNT_ATTR_RELATIME 0x00000000
#د_که_پای

#که_نه_تعریف_شوی MOUNT_ATTR_NOATIME
#  define MOUNT_ATTR_NOATIME 0x00000010
#د_که_پای

#که_نه_تعریف_شوی MOUNT_ATTR_STRICTATIME
#  define MOUNT_ATTR_STRICTATIME 0x00000020
#د_که_پای

#که_نه_تعریف_شوی MOUNT_ATTR_NODIRATIME
#  define MOUNT_ATTR_NODIRATIME 0x00000080
#د_که_پای

#که_نه_تعریف_شوی MOUNT_ATTR_IDMAP
#  define MOUNT_ATTR_IDMAP 0x00100000 /* Idmap mount to @userns_fd in struct mount_attr. */
#د_که_پای

#که_نه_تعریف_شوی MOUNT_ATTR_NOSYMFOLLOW
#  define MOUNT_ATTR_NOSYMFOLLOW 0x00200000
#د_که_پای

#که_نه_تعریف_شوی MS_NOSYMFOLLOW
#  define MS_NOSYMFOLLOW 256
#د_که_پای

/* close_range flags */
#که_نه_تعریف_شوی CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#د_که_پای

/* openat2 resolve flags */
#که_نه_تعریف_شوی RESOLVE_NO_MAGICLINKS
#  define RESOLVE_NO_MAGICLINKS 0x02
#د_که_پای
#که_نه_تعریف_شوی RESOLVE_NO_SYMLINKS
#  define RESOLVE_NO_SYMLINKS 0x04
#د_که_پای
#که_نه_تعریف_شوی RESOLVE_دننه_ریښه
#  define RESOLVE_دننه_ریښه 0x10
#د_که_پای

/* Structures for syscalls */
بنية openat2_open_how
{
  uint64_t flags;
  uint64_t ډېرپېښ;
  uint64_t resolve;
};

بنية mount_attr_s
{
  uint64_t attr_set;
  uint64_t attr_clr;
  uint64_t انتشار;
  uint64_t userns_fd;
};

#که_نه_تعریف_شوی __aligned_u64
#  define __aligned_u64 uint64_t __attribute__ ((aligned (8)))
#د_که_پای

#که_نه_تعریف_شوی CLONE_INTO_CGROUP
#  define CLONE_INTO_CGROUP 0x200000000ULL
#د_که_پای

بنية clone3_دلیلونه
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 ماشوم_tid;
  __aligned_u64 پلار_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 ډېری;
  __aligned_u64 ډېری_اندازه;
  __aligned_u64 tls;
  __aligned_u64 سېټ_tid;
  __aligned_u64 سېټ_tid_اندازه;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
ساكن مضمن صحيح
syscall_fsopen (ثابت حرف *fs_name, بینښه صحيح flags)
{
#إذا defined __NR_fsopen
  أرجع (صحيح) syscall (__NR_fsopen, fs_name, flags);
#وإلا
  (فراغ) fs_name;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#د_که_پای
}

ساكن مضمن صحيح
syscall_fsmount (صحيح fsfd, بینښه صحيح flags, بینښه صحيح attr_flags)
{
#إذا defined __NR_fsmount
  أرجع (صحيح) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#وإلا
  (فراغ) fsfd;
  (فراغ) flags;
  (فراغ) attr_flags;
  errno = ENOSYS;
  أرجع -1;
#د_که_پای
}

ساكن مضمن صحيح
syscall_fsconfig (صحيح fsfd, بینښه صحيح cmd, ثابت حرف *کلی, ثابت فراغ *val, صحيح aux)
{
#إذا defined __NR_fsconfig
  أرجع (صحيح) syscall (__NR_fsconfig, fsfd, cmd, کلی, val, aux);
#وإلا
  (فراغ) fsfd;
  (فراغ) cmd;
  (فراغ) کلی;
  (فراغ) val;
  (فراغ) aux;
  errno = ENOSYS;
  أرجع -1;
#د_که_پای
}

/* File descriptor management syscalls */
ساكن مضمن صحيح
syscall_close_range (بینښه صحيح fd, بینښه صحيح max_fd, بینښه صحيح flags)
{
#که_تعریف_شوی __NR_close_range
  أرجع (صحيح) syscall (__NR_close_range, fd, max_fd, flags);
#وإلا
  (فراغ) fd;
  (فراغ) max_fd;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#د_که_پای
}

/* Secure file operation syscalls */
ساكن مضمن صحيح
syscall_openat2 (صحيح dirfd, ثابت حرف *لار, uint64_t flags, uint64_t ډېرپېښ, uint64_t resolve)
{
#که_تعریف_شوی __NR_openat2
  بنية openat2_open_how how = {
    .flags = flags,
    .ډېرپېښ = ډېرپېښ,
    .resolve = resolve,
  };
  أرجع (صحيح) syscall (__NR_openat2, dirfd, لار, &how, حجم (how), 0);
#وإلا
  (فراغ) dirfd;
  (فراغ) لار;
  (فراغ) flags;
  (فراغ) ډېرپېښ;
  (فراغ) resolve;
  errno = ENOSYS;
  أرجع -1;
#د_که_پای
}

/* Process management syscalls */
ساكن مضمن صحيح
syscall_clone3 (بنية clone3_دلیلونه *دلیلونه)
{
#که_تعریف_شوی __NR_clone3
  أرجع (صحيح) syscall (__NR_clone3, دلیلونه, حجم (*دلیلونه));
#وإلا
  (فراغ) دلیلونه;
  errno = ENOSYS;
  أرجع -1;
#د_که_پای
}

ساكن مضمن صحيح
syscall_getcwd (حرف *لار, size_t len)
{
#که_تعریف_شوی __NR_getcwd
  أرجع (صحيح) syscall (__NR_getcwd, لار, len);
#وإلا
  (فراغ) لار;
  (فراغ) len;
  errno = ENOSYS;
  أرجع -1;
#د_که_پای
}

/* Mount management syscalls */
ساكن مضمن صحيح
syscall_move_mount (صحيح from_dfd, ثابت حرف *from_pathname, صحيح منزل_dfd, ثابت حرف *منزل_pathname, بینښه صحيح flags)
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
#د_که_پای
}

ساكن مضمن صحيح
syscall_open_ونه (صحيح dfd, ثابت حرف *pathname, بینښه صحيح flags)
{
#إذا defined __NR_open_tree
  أرجع (صحيح) syscall (__NR_open_tree, dfd, pathname, flags);
#وإلا
  (فراغ) dfd;
  (فراغ) pathname;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#د_که_پای
}

ساكن مضمن صحيح
syscall_mount_setattr (صحيح dfd, ثابت حرف *لار, بینښه صحيح flags, بنية mount_attr_s *attr)
{
#که_تعریف_شوی __NR_mount_setattr
  أرجع (صحيح) syscall (__NR_mount_setattr, dfd, لار, flags, attr, حجم (*attr));
#وإلا
  (فراغ) dfd;
  (فراغ) لار;
  (فراغ) flags;
  (فراغ) attr;
  errno = ENOSYS;
  أرجع -1;
#د_که_پای
}

#د_که_پای
