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

#اگر_غیر_تعریف_شدہ CRUN_SYSCALLS_H
#تعریف_کرو CRUN_SYSCALLS_H

#شامل <config.h>
#شامل <sys/syscall.h>
#شامل <unistd.h>
#شامل <errno.h>
#شامل <stdint.h>

#اگر_تعریف_شدہ HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#اگر_ختم
#اگر defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#اگر_ختم

/* Constants for new mount API */
#اگر_غیر_تعریف_شدہ FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#اگر_ختم

#اگر_غیر_تعریف_شدہ FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#اگر_ختم

#اگر_غیر_تعریف_شدہ FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#اگر_ختم

#اگر_غیر_تعریف_شدہ FSCONFIG_SET_STRING
#  define FSCONFIG_SET_STRING 1
#اگر_ختم

/* Additional mount API constants */
#اگر_غیر_تعریف_شدہ MOUNT_ATTR_RDONLY
#  define MOUNT_ATTR_RDONLY 0x00000001 /* Mount read-only */
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOUNT_ATTR_NOSUID
#  define MOUNT_ATTR_NOSUID 0x00000002
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOUNT_ATTR_NODEV
#  define MOUNT_ATTR_NODEV 0x00000004
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOUNT_ATTR_NOEXEC
#  define MOUNT_ATTR_NOEXEC 0x00000008
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOUNT_ATTR__ATIME
#  define MOUNT_ATTR__ATIME 0x00000070
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOUNT_ATTR_RELATIME
#  define MOUNT_ATTR_RELATIME 0x00000000
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOUNT_ATTR_NOATIME
#  define MOUNT_ATTR_NOATIME 0x00000010
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOUNT_ATTR_STRICTATIME
#  define MOUNT_ATTR_STRICTATIME 0x00000020
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOUNT_ATTR_NODIRATIME
#  define MOUNT_ATTR_NODIRATIME 0x00000080
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOUNT_ATTR_IDMAP
#  define MOUNT_ATTR_IDMAP 0x00100000 /* Idmap mount to @userns_fd in struct mount_attr. */
#اگر_ختم

#اگر_غیر_تعریف_شدہ MOUNT_ATTR_NOSYMFOLLOW
#  define MOUNT_ATTR_NOSYMFOLLOW 0x00200000
#اگر_ختم

#اگر_غیر_تعریف_شدہ MS_NOSYMFOLLOW
#  define MS_NOSYMFOLLOW 256
#اگر_ختم

/* close_range flags */
#اگر_غیر_تعریف_شدہ CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#اگر_ختم

/* openat2 resolve flags */
#اگر_غیر_تعریف_شدہ RESOLVE_NO_MAGICLINKS
#  define RESOLVE_NO_MAGICLINKS 0x02
#اگر_ختم
#اگر_غیر_تعریف_شدہ RESOLVE_NO_SYMLINKS
#  define RESOLVE_NO_SYMLINKS 0x04
#اگر_ختم
#اگر_غیر_تعریف_شدہ RESOLVE_اندر_جڑ
#  define RESOLVE_اندر_جڑ 0x10
#اگر_ختم

/* Structures for syscalls */
ساخت openat2_open_how
{
  uint64_t flags;
  uint64_t منوال;
  uint64_t resolve;
};

ساخت mount_attr_s
{
  uint64_t attr_set;
  uint64_t attr_clr;
  uint64_t اشاعت;
  uint64_t userns_fd;
};

#اگر_غیر_تعریف_شدہ __aligned_u64
#  define __aligned_u64 uint64_t __attribute__ ((aligned (8)))
#اگر_ختم

#اگر_غیر_تعریف_شدہ CLONE_INTO_CGROUP
#  define CLONE_INTO_CGROUP 0x200000000ULL
#اگر_ختم

ساخت clone3_دلائل
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 بچہ_tid;
  __aligned_u64 والد_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 ڈھیر;
  __aligned_u64 ڈھیر_سائز;
  __aligned_u64 tls;
  __aligned_u64 سیٹ_tid;
  __aligned_u64 سیٹ_tid_سائز;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
ساکن اندرونی صحیح
syscall_fsopen (ثابت حرف *fs_name, غیرعلامتی صحیح flags)
{
#اگر defined __NR_fsopen
  واپس (صحیح) syscall (__NR_fsopen, fs_name, flags);
#ورنہ
  (خالی) fs_name;
  (خالی) flags;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

ساکن اندرونی صحیح
syscall_fsmount (صحیح fsfd, غیرعلامتی صحیح flags, غیرعلامتی صحیح attr_flags)
{
#اگر defined __NR_fsmount
  واپس (صحیح) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#ورنہ
  (خالی) fsfd;
  (خالی) flags;
  (خالی) attr_flags;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

ساکن اندرونی صحیح
syscall_fsconfig (صحیح fsfd, غیرعلامتی صحیح cmd, ثابت حرف *کلید, ثابت خالی *val, صحیح aux)
{
#اگر defined __NR_fsconfig
  واپس (صحیح) syscall (__NR_fsconfig, fsfd, cmd, کلید, val, aux);
#ورنہ
  (خالی) fsfd;
  (خالی) cmd;
  (خالی) کلید;
  (خالی) val;
  (خالی) aux;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

/* File descriptor management syscalls */
ساکن اندرونی صحیح
syscall_close_range (غیرعلامتی صحیح fd, غیرعلامتی صحیح max_fd, غیرعلامتی صحیح flags)
{
#اگر_تعریف_شدہ __NR_close_range
  واپس (صحیح) syscall (__NR_close_range, fd, max_fd, flags);
#ورنہ
  (خالی) fd;
  (خالی) max_fd;
  (خالی) flags;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

/* Secure file operation syscalls */
ساکن اندرونی صحیح
syscall_openat2 (صحیح dirfd, ثابت حرف *راستہ, uint64_t flags, uint64_t منوال, uint64_t resolve)
{
#اگر_تعریف_شدہ __NR_openat2
  ساخت openat2_open_how how = {
    .flags = flags,
    .منوال = منوال,
    .resolve = resolve,
  };
  واپس (صحیح) syscall (__NR_openat2, dirfd, راستہ, &how, حجم (how), 0);
#ورنہ
  (خالی) dirfd;
  (خالی) راستہ;
  (خالی) flags;
  (خالی) منوال;
  (خالی) resolve;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

/* Process management syscalls */
ساکن اندرونی صحیح
syscall_clone3 (ساخت clone3_دلائل *دلائل)
{
#اگر_تعریف_شدہ __NR_clone3
  واپس (صحیح) syscall (__NR_clone3, دلائل, حجم (*دلائل));
#ورنہ
  (خالی) دلائل;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

ساکن اندرونی صحیح
syscall_getcwd (حرف *راستہ, size_t len)
{
#اگر_تعریف_شدہ __NR_getcwd
  واپس (صحیح) syscall (__NR_getcwd, راستہ, len);
#ورنہ
  (خالی) راستہ;
  (خالی) len;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

/* Mount management syscalls */
ساکن اندرونی صحیح
syscall_move_mount (صحیح from_dfd, ثابت حرف *from_pathname, صحیح مقصد_dfd, ثابت حرف *مقصد_pathname, غیرعلامتی صحیح flags)
{
#اگر defined __NR_move_mount
  واپس (صحیح) syscall (__NR_move_mount, from_dfd, from_pathname, مقصد_dfd, مقصد_pathname, flags);
#ورنہ
  (خالی) from_dfd;
  (خالی) from_pathname;
  (خالی) مقصد_dfd;
  (خالی) مقصد_pathname;
  (خالی) flags;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

ساکن اندرونی صحیح
syscall_open_درخت (صحیح dfd, ثابت حرف *pathname, غیرعلامتی صحیح flags)
{
#اگر defined __NR_open_tree
  واپس (صحیح) syscall (__NR_open_tree, dfd, pathname, flags);
#ورنہ
  (خالی) dfd;
  (خالی) pathname;
  (خالی) flags;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

ساکن اندرونی صحیح
syscall_mount_setattr (صحیح dfd, ثابت حرف *راستہ, غیرعلامتی صحیح flags, ساخت mount_attr_s *attr)
{
#اگر_تعریف_شدہ __NR_mount_setattr
  واپس (صحیح) syscall (__NR_mount_setattr, dfd, راستہ, flags, attr, حجم (*attr));
#ورنہ
  (خالی) dfd;
  (خالی) راستہ;
  (خالی) flags;
  (خالی) attr;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

#اگر_ختم
