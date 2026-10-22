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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CRUN_SYSCALLS_H
#ವ್ಯಾಖ್ಯಾನಿಸು CRUN_SYSCALLS_H

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#ಆದರೆ_ಅಂತ್ಯ

/* Constants for new mount API */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FSCONFIG_SET_STRING
#  define FSCONFIG_SET_STRING 1
#ಆದರೆ_ಅಂತ್ಯ

/* Additional mount API constants */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_ATTR_RDONLY
#  define MOUNT_ATTR_RDONLY 0x00000001 /* Mount read-only */
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_ATTR_NOSUID
#  define MOUNT_ATTR_NOSUID 0x00000002
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_ATTR_NODEV
#  define MOUNT_ATTR_NODEV 0x00000004
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_ATTR_NOEXEC
#  define MOUNT_ATTR_NOEXEC 0x00000008
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_ATTR__ATIME
#  define MOUNT_ATTR__ATIME 0x00000070
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_ATTR_RELATIME
#  define MOUNT_ATTR_RELATIME 0x00000000
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_ATTR_NOATIME
#  define MOUNT_ATTR_NOATIME 0x00000010
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_ATTR_STRICTATIME
#  define MOUNT_ATTR_STRICTATIME 0x00000020
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_ATTR_NODIRATIME
#  define MOUNT_ATTR_NODIRATIME 0x00000080
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_ATTR_IDMAP
#  define MOUNT_ATTR_IDMAP 0x00100000 /* Idmap mount to @userns_fd in struct mount_attr. */
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_ATTR_NOSYMFOLLOW
#  define MOUNT_ATTR_NOSYMFOLLOW 0x00200000
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MS_NOSYMFOLLOW
#  define MS_NOSYMFOLLOW 256
#ಆದರೆ_ಅಂತ್ಯ

/* close_range flags */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#ಆದರೆ_ಅಂತ್ಯ

/* openat2 resolve flags */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ RESOLVE_NO_MAGICLINKS
#  define RESOLVE_NO_MAGICLINKS 0x02
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ RESOLVE_NO_SYMLINKS
#  define RESOLVE_NO_SYMLINKS 0x04
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ RESOLVE_ಒಳ_ಬೇರು
#  define RESOLVE_ಒಳ_ಬೇರು 0x10
#ಆದರೆ_ಅಂತ್ಯ

/* Structures for syscalls */
ರಚನೆ openat2_open_how
{
  uint64_t flags;
  uint64_t ಬಹುಲಕ;
  uint64_t resolve;
};

ರಚನೆ mount_attr_s
{
  uint64_t attr_set;
  uint64_t attr_clr;
  uint64_t ಪ್ರಸರಣ;
  uint64_t userns_fd;
};

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __aligned_u64
#  define __aligned_u64 uint64_t __attribute__ ((aligned (8)))
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CLONE_INTO_CGROUP
#  define CLONE_INTO_CGROUP 0x200000000ULL
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ clone3_ವಾದಗಳು
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 ಮಗು_tid;
  __aligned_u64 ಪೋಷಕ_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 ರಾಶಿ;
  __aligned_u64 ರಾಶಿ_ಗಾತ್ರ;
  __aligned_u64 tls;
  __aligned_u64 ಗಣ_tid;
  __aligned_u64 ಗಣ_tid_ಗಾತ್ರ;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
syscall_fsopen (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fs_name, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
#ಆದರೆ defined __NR_fsopen
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_fsopen, fs_name, flags);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) fs_name;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
syscall_fsmount (ಪೂರ್ಣಾಂಕ fsfd, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ attr_flags)
{
#ಆದರೆ defined __NR_fsmount
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) fsfd;
  (ಶೂನ್ಯ) flags;
  (ಶೂನ್ಯ) attr_flags;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
syscall_fsconfig (ಪೂರ್ಣಾಂಕ fsfd, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cmd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *val, ಪೂರ್ಣಾಂಕ aux)
{
#ಆದರೆ defined __NR_fsconfig
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_fsconfig, fsfd, cmd, ಕೀಲಿ, val, aux);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) fsfd;
  (ಶೂನ್ಯ) cmd;
  (ಶೂನ್ಯ) ಕೀಲಿ;
  (ಶೂನ್ಯ) val;
  (ಶೂನ್ಯ) aux;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

/* File descriptor management syscalls */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
syscall_close_range (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ fd, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ max_fd, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __NR_close_range
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_close_range, fd, max_fd, flags);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) fd;
  (ಶೂನ್ಯ) max_fd;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

/* Secure file operation syscalls */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
syscall_openat2 (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, uint64_t flags, uint64_t ಬಹುಲಕ, uint64_t resolve)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __NR_openat2
  ರಚನೆ openat2_open_how how = {
    .flags = flags,
    .ಬಹುಲಕ = ಬಹುಲಕ,
    .resolve = resolve,
  };
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_openat2, dirfd, ಮಾರ್ಗ, &how, ಗಾತ್ರ (how), 0);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) dirfd;
  (ಶೂನ್ಯ) ಮಾರ್ಗ;
  (ಶೂನ್ಯ) flags;
  (ಶೂನ್ಯ) ಬಹುಲಕ;
  (ಶೂನ್ಯ) resolve;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

/* Process management syscalls */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
syscall_clone3 (ರಚನೆ clone3_ವಾದಗಳು *ವಾದಗಳು)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __NR_clone3
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_clone3, ವಾದಗಳು, ಗಾತ್ರ (*ವಾದಗಳು));
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) ವಾದಗಳು;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
syscall_getcwd (ಅಕ್ಷರ *ಮಾರ್ಗ, size_t len)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __NR_getcwd
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_getcwd, ಮಾರ್ಗ, len);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) ಮಾರ್ಗ;
  (ಶೂನ್ಯ) len;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

/* Mount management syscalls */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
syscall_move_mount (ಪೂರ್ಣಾಂಕ from_dfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *from_pathname, ಪೂರ್ಣಾಂಕ ಗಮ್ಯ_dfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗಮ್ಯ_pathname, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
#ಆದರೆ defined __NR_move_mount
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_move_mount, from_dfd, from_pathname, ಗಮ್ಯ_dfd, ಗಮ್ಯ_pathname, flags);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) from_dfd;
  (ಶೂನ್ಯ) from_pathname;
  (ಶೂನ್ಯ) ಗಮ್ಯ_dfd;
  (ಶೂನ್ಯ) ಗಮ್ಯ_pathname;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
syscall_open_ವೃಕ್ಷ (ಪೂರ್ಣಾಂಕ dfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
#ಆದರೆ defined __NR_open_tree
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_open_tree, dfd, pathname, flags);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) dfd;
  (ಶೂನ್ಯ) pathname;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
syscall_mount_setattr (ಪೂರ್ಣಾಂಕ dfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags, ರಚನೆ mount_attr_s *attr)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __NR_mount_setattr
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_mount_setattr, dfd, ಮಾರ್ಗ, flags, attr, ಗಾತ್ರ (*attr));
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) dfd;
  (ಶೂನ್ಯ) ಮಾರ್ಗ;
  (ಶೂನ್ಯ) flags;
  (ಶೂನ್ಯ) attr;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

#ಆದರೆ_ಅಂತ್ಯ
