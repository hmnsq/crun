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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CRUN_SYSCALLS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CRUN_SYSCALLS_H

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#ਜੇ_ਅੰਤ
#ਜੇ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#ਜੇ_ਅੰਤ

/* Constants for new mount API */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FSCONFIG_SET_STRING
#  define FSCONFIG_SET_STRING 1
#ਜੇ_ਅੰਤ

/* Additional mount API constants */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNT_ATTR_RDONLY
#  define MOUNT_ATTR_RDONLY 0x00000001 /* Mount read-only */
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNT_ATTR_NOSUID
#  define MOUNT_ATTR_NOSUID 0x00000002
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNT_ATTR_NODEV
#  define MOUNT_ATTR_NODEV 0x00000004
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNT_ATTR_NOEXEC
#  define MOUNT_ATTR_NOEXEC 0x00000008
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNT_ATTR__ATIME
#  define MOUNT_ATTR__ATIME 0x00000070
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNT_ATTR_RELATIME
#  define MOUNT_ATTR_RELATIME 0x00000000
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNT_ATTR_NOATIME
#  define MOUNT_ATTR_NOATIME 0x00000010
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNT_ATTR_STRICTATIME
#  define MOUNT_ATTR_STRICTATIME 0x00000020
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNT_ATTR_NODIRATIME
#  define MOUNT_ATTR_NODIRATIME 0x00000080
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNT_ATTR_IDMAP
#  define MOUNT_ATTR_IDMAP 0x00100000 /* Idmap mount to @userns_fd in struct mount_attr. */
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNT_ATTR_NOSYMFOLLOW
#  define MOUNT_ATTR_NOSYMFOLLOW 0x00200000
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MS_NOSYMFOLLOW
#  define MS_NOSYMFOLLOW 256
#ਜੇ_ਅੰਤ

/* close_range flags */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#ਜੇ_ਅੰਤ

/* openat2 resolve flags */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ RESOLVE_NO_MAGICLINKS
#  define RESOLVE_NO_MAGICLINKS 0x02
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ RESOLVE_NO_SYMLINKS
#  define RESOLVE_NO_SYMLINKS 0x04
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ RESOLVE_ਵਿੱਚ_ਜੜ੍ਹ
#  define RESOLVE_ਵਿੱਚ_ਜੜ੍ਹ 0x10
#ਜੇ_ਅੰਤ

/* Structures for syscalls */
ਬਣਤਰ openat2_open_how
{
  uint64_t flags;
  uint64_t ਬਹੁਲਕ;
  uint64_t resolve;
};

ਬਣਤਰ mount_attr_s
{
  uint64_t attr_set;
  uint64_t attr_clr;
  uint64_t ਪ੍ਰਸਾਰ;
  uint64_t userns_fd;
};

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __aligned_u64
#  define __aligned_u64 uint64_t __attribute__ ((aligned (8)))
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CLONE_INTO_CGROUP
#  define CLONE_INTO_CGROUP 0x200000000ULL
#ਜੇ_ਅੰਤ

ਬਣਤਰ clone3_ਦਲੀਲਾਂ
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 ਬੱਚਾ_tid;
  __aligned_u64 ਮਾਪਾ_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 ਢੇਰ;
  __aligned_u64 ਢੇਰ_ਮਾਪ;
  __aligned_u64 tls;
  __aligned_u64 ਸੈੱਟ_tid;
  __aligned_u64 ਸੈੱਟ_tid_ਮਾਪ;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
syscall_fsopen (ਅਚਲ ਅੱਖਰ *fs_name, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
#ਜੇ defined __NR_fsopen
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_fsopen, fs_name, flags);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) fs_name;
  (ਖਾਲੀ) flags;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
syscall_fsmount (ਪੂਰਨ fsfd, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ attr_flags)
{
#ਜੇ defined __NR_fsmount
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) fsfd;
  (ਖਾਲੀ) flags;
  (ਖਾਲੀ) attr_flags;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
syscall_fsconfig (ਪੂਰਨ fsfd, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cmd, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅਚਲ ਖਾਲੀ *val, ਪੂਰਨ aux)
{
#ਜੇ defined __NR_fsconfig
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_fsconfig, fsfd, cmd, ਕੁੰਜੀ, val, aux);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) fsfd;
  (ਖਾਲੀ) cmd;
  (ਖਾਲੀ) ਕੁੰਜੀ;
  (ਖਾਲੀ) val;
  (ਖਾਲੀ) aux;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

/* File descriptor management syscalls */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
syscall_close_range (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_fd, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __NR_close_range
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_close_range, fd, max_fd, flags);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) fd;
  (ਖਾਲੀ) max_fd;
  (ਖਾਲੀ) flags;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

/* Secure file operation syscalls */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
syscall_openat2 (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, uint64_t flags, uint64_t ਬਹੁਲਕ, uint64_t resolve)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __NR_openat2
  ਬਣਤਰ openat2_open_how how = {
    .flags = flags,
    .ਬਹੁਲਕ = ਬਹੁਲਕ,
    .resolve = resolve,
  };
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_openat2, dirfd, ਰਸਤਾ, &how, ਆਕਾਰ (how), 0);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) dirfd;
  (ਖਾਲੀ) ਰਸਤਾ;
  (ਖਾਲੀ) flags;
  (ਖਾਲੀ) ਬਹੁਲਕ;
  (ਖਾਲੀ) resolve;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

/* Process management syscalls */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
syscall_clone3 (ਬਣਤਰ clone3_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __NR_clone3
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_clone3, ਦਲੀਲਾਂ, ਆਕਾਰ (*ਦਲੀਲਾਂ));
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) ਦਲੀਲਾਂ;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
syscall_getcwd (ਅੱਖਰ *ਰਸਤਾ, size_t len)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __NR_getcwd
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_getcwd, ਰਸਤਾ, len);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) ਰਸਤਾ;
  (ਖਾਲੀ) len;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

/* Mount management syscalls */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
syscall_move_mount (ਪੂਰਨ from_dfd, ਅਚਲ ਅੱਖਰ *from_pathname, ਪੂਰਨ ਮੰਜ਼ਿਲ_dfd, ਅਚਲ ਅੱਖਰ *ਮੰਜ਼ਿਲ_pathname, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
#ਜੇ defined __NR_move_mount
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_move_mount, from_dfd, from_pathname, ਮੰਜ਼ਿਲ_dfd, ਮੰਜ਼ਿਲ_pathname, flags);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) from_dfd;
  (ਖਾਲੀ) from_pathname;
  (ਖਾਲੀ) ਮੰਜ਼ਿਲ_dfd;
  (ਖਾਲੀ) ਮੰਜ਼ਿਲ_pathname;
  (ਖਾਲੀ) flags;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
syscall_open_ਰੁੱਖ (ਪੂਰਨ dfd, ਅਚਲ ਅੱਖਰ *pathname, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
#ਜੇ defined __NR_open_tree
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_open_tree, dfd, pathname, flags);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) dfd;
  (ਖਾਲੀ) pathname;
  (ਖਾਲੀ) flags;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
syscall_mount_setattr (ਪੂਰਨ dfd, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags, ਬਣਤਰ mount_attr_s *attr)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __NR_mount_setattr
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_mount_setattr, dfd, ਰਸਤਾ, flags, attr, ਆਕਾਰ (*attr));
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) dfd;
  (ਖਾਲੀ) ਰਸਤਾ;
  (ਖਾਲੀ) flags;
  (ਖਾਲੀ) attr;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਅੰਤ
