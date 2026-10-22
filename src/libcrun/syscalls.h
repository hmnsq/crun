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

#यदि_अपरिभाषितम् CRUN_SYSCALLS_H
#परिभाषय CRUN_SYSCALLS_H

#अन्तर्भाव <config.h>
#अन्तर्भाव <sys/syscall.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <stdint.h>

#यदि_परिभाषितम् HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#यदि_अन्तः
#यदि defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#यदि_अन्तः

/* Constants for new mount API */
#यदि_अपरिभाषितम् FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#यदि_अन्तः

#यदि_अपरिभाषितम् FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#यदि_अन्तः

#यदि_अपरिभाषितम् FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#यदि_अन्तः

#यदि_अपरिभाषितम् FSCONFIG_SET_STRING
#  define FSCONFIG_SET_STRING 1
#यदि_अन्तः

/* Additional mount API constants */
#यदि_अपरिभाषितम् MOUNT_ATTR_RDONLY
#  define MOUNT_ATTR_RDONLY 0x00000001 /* Mount read-only */
#यदि_अन्तः

#यदि_अपरिभाषितम् MOUNT_ATTR_NOSUID
#  define MOUNT_ATTR_NOSUID 0x00000002
#यदि_अन्तः

#यदि_अपरिभाषितम् MOUNT_ATTR_NODEV
#  define MOUNT_ATTR_NODEV 0x00000004
#यदि_अन्तः

#यदि_अपरिभाषितम् MOUNT_ATTR_NOEXEC
#  define MOUNT_ATTR_NOEXEC 0x00000008
#यदि_अन्तः

#यदि_अपरिभाषितम् MOUNT_ATTR__ATIME
#  define MOUNT_ATTR__ATIME 0x00000070
#यदि_अन्तः

#यदि_अपरिभाषितम् MOUNT_ATTR_RELATIME
#  define MOUNT_ATTR_RELATIME 0x00000000
#यदि_अन्तः

#यदि_अपरिभाषितम् MOUNT_ATTR_NOATIME
#  define MOUNT_ATTR_NOATIME 0x00000010
#यदि_अन्तः

#यदि_अपरिभाषितम् MOUNT_ATTR_STRICTATIME
#  define MOUNT_ATTR_STRICTATIME 0x00000020
#यदि_अन्तः

#यदि_अपरिभाषितम् MOUNT_ATTR_NODIRATIME
#  define MOUNT_ATTR_NODIRATIME 0x00000080
#यदि_अन्तः

#यदि_अपरिभाषितम् MOUNT_ATTR_IDMAP
#  define MOUNT_ATTR_IDMAP 0x00100000 /* Idmap mount to @userns_fd in struct mount_attr. */
#यदि_अन्तः

#यदि_अपरिभाषितम् MOUNT_ATTR_NOSYMFOLLOW
#  define MOUNT_ATTR_NOSYMFOLLOW 0x00200000
#यदि_अन्तः

#यदि_अपरिभाषितम् MS_NOSYMFOLLOW
#  define MS_NOSYMFOLLOW 256
#यदि_अन्तः

/* close_range flags */
#यदि_अपरिभाषितम् CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#यदि_अन्तः

/* openat2 resolve flags */
#यदि_अपरिभाषितम् RESOLVE_NO_MAGICLINKS
#  define RESOLVE_NO_MAGICLINKS 0x02
#यदि_अन्तः
#यदि_अपरिभाषितम् RESOLVE_NO_SYMLINKS
#  define RESOLVE_NO_SYMLINKS 0x04
#यदि_अन्तः
#यदि_अपरिभाषितम् RESOLVE_अन्तर्_मूलम्
#  define RESOLVE_अन्तर्_मूलम् 0x10
#यदि_अन्तः

/* Structures for syscalls */
संरचना openat2_open_how
{
  uint64_t flags;
  uint64_t बहुलकम्;
  uint64_t resolve;
};

संरचना mount_attr_s
{
  uint64_t attr_set;
  uint64_t attr_clr;
  uint64_t सञ्चरणम्;
  uint64_t userns_fd;
};

#यदि_अपरिभाषितम् __aligned_u64
#  define __aligned_u64 uint64_t __attribute__ ((aligned (8)))
#यदि_अन्तः

#यदि_अपरिभाषितम् CLONE_INTO_CGROUP
#  define CLONE_INTO_CGROUP 0x200000000ULL
#यदि_अन्तः

संरचना clone3_तर्काः
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 राशिः;
  __aligned_u64 राशिः_आकार;
  __aligned_u64 tls;
  __aligned_u64 समुच्चयः_tid;
  __aligned_u64 समुच्चयः_tid_आकार;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
स्थिर अन्तःस्थ पूर्णाङ्क
syscall_fsopen (ध्रुव वर्ण *fs_name, अचिह्नित पूर्णाङ्क flags)
{
#यदि defined __NR_fsopen
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_fsopen, fs_name, flags);
#अन्यथा
  (शून्य) fs_name;
  (शून्य) flags;
  errno = ENOSYS;
  प्रत्यागम -1;
#यदि_अन्तः
}

स्थिर अन्तःस्थ पूर्णाङ्क
syscall_fsmount (पूर्णाङ्क fsfd, अचिह्नित पूर्णाङ्क flags, अचिह्नित पूर्णाङ्क attr_flags)
{
#यदि defined __NR_fsmount
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#अन्यथा
  (शून्य) fsfd;
  (शून्य) flags;
  (शून्य) attr_flags;
  errno = ENOSYS;
  प्रत्यागम -1;
#यदि_अन्तः
}

स्थिर अन्तःस्थ पूर्णाङ्क
syscall_fsconfig (पूर्णाङ्क fsfd, अचिह्नित पूर्णाङ्क cmd, ध्रुव वर्ण *कुञ्चिका, ध्रुव शून्य *val, पूर्णाङ्क aux)
{
#यदि defined __NR_fsconfig
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_fsconfig, fsfd, cmd, कुञ्चिका, val, aux);
#अन्यथा
  (शून्य) fsfd;
  (शून्य) cmd;
  (शून्य) कुञ्चिका;
  (शून्य) val;
  (शून्य) aux;
  errno = ENOSYS;
  प्रत्यागम -1;
#यदि_अन्तः
}

/* File descriptor management syscalls */
स्थिर अन्तःस्थ पूर्णाङ्क
syscall_close_range (अचिह्नित पूर्णाङ्क fd, अचिह्नित पूर्णाङ्क max_fd, अचिह्नित पूर्णाङ्क flags)
{
#यदि_परिभाषितम् __NR_close_range
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_close_range, fd, max_fd, flags);
#अन्यथा
  (शून्य) fd;
  (शून्य) max_fd;
  (शून्य) flags;
  errno = ENOSYS;
  प्रत्यागम -1;
#यदि_अन्तः
}

/* Secure file operation syscalls */
स्थिर अन्तःस्थ पूर्णाङ्क
syscall_openat2 (पूर्णाङ्क dirfd, ध्रुव वर्ण *मार्गः, uint64_t flags, uint64_t बहुलकम्, uint64_t resolve)
{
#यदि_परिभाषितम् __NR_openat2
  संरचना openat2_open_how how = {
    .flags = flags,
    .बहुलकम् = बहुलकम्,
    .resolve = resolve,
  };
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_openat2, dirfd, मार्गः, &how, परिमाण (how), 0);
#अन्यथा
  (शून्य) dirfd;
  (शून्य) मार्गः;
  (शून्य) flags;
  (शून्य) बहुलकम्;
  (शून्य) resolve;
  errno = ENOSYS;
  प्रत्यागम -1;
#यदि_अन्तः
}

/* Process management syscalls */
स्थिर अन्तःस्थ पूर्णाङ्क
syscall_clone3 (संरचना clone3_तर्काः *तर्काः)
{
#यदि_परिभाषितम् __NR_clone3
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_clone3, तर्काः, परिमाण (*तर्काः));
#अन्यथा
  (शून्य) तर्काः;
  errno = ENOSYS;
  प्रत्यागम -1;
#यदि_अन्तः
}

स्थिर अन्तःस्थ पूर्णाङ्क
syscall_getcwd (वर्ण *मार्गः, size_t len)
{
#यदि_परिभाषितम् __NR_getcwd
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_getcwd, मार्गः, len);
#अन्यथा
  (शून्य) मार्गः;
  (शून्य) len;
  errno = ENOSYS;
  प्रत्यागम -1;
#यदि_अन्तः
}

/* Mount management syscalls */
स्थिर अन्तःस्थ पूर्णाङ्क
syscall_move_mount (पूर्णाङ्क from_dfd, ध्रुव वर्ण *from_pathname, पूर्णाङ्क गन्तव्यम्_dfd, ध्रुव वर्ण *गन्तव्यम्_pathname, अचिह्नित पूर्णाङ्क flags)
{
#यदि defined __NR_move_mount
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_move_mount, from_dfd, from_pathname, गन्तव्यम्_dfd, गन्तव्यम्_pathname, flags);
#अन्यथा
  (शून्य) from_dfd;
  (शून्य) from_pathname;
  (शून्य) गन्तव्यम्_dfd;
  (शून्य) गन्तव्यम्_pathname;
  (शून्य) flags;
  errno = ENOSYS;
  प्रत्यागम -1;
#यदि_अन्तः
}

स्थिर अन्तःस्थ पूर्णाङ्क
syscall_open_वृक्षः (पूर्णाङ्क dfd, ध्रुव वर्ण *pathname, अचिह्नित पूर्णाङ्क flags)
{
#यदि defined __NR_open_tree
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_open_tree, dfd, pathname, flags);
#अन्यथा
  (शून्य) dfd;
  (शून्य) pathname;
  (शून्य) flags;
  errno = ENOSYS;
  प्रत्यागम -1;
#यदि_अन्तः
}

स्थिर अन्तःस्थ पूर्णाङ्क
syscall_mount_setattr (पूर्णाङ्क dfd, ध्रुव वर्ण *मार्गः, अचिह्नित पूर्णाङ्क flags, संरचना mount_attr_s *attr)
{
#यदि_परिभाषितम् __NR_mount_setattr
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_mount_setattr, dfd, मार्गः, flags, attr, परिमाण (*attr));
#अन्यथा
  (शून्य) dfd;
  (शून्य) मार्गः;
  (शून्य) flags;
  (शून्य) attr;
  errno = ENOSYS;
  प्रत्यागम -1;
#यदि_अन्तः
}

#यदि_अन्तः
