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

#अगर_अघोषित CRUN_SYSCALLS_H
#घोषणा CRUN_SYSCALLS_H

#समावेश <config.h>
#समावेश <sys/syscall.h>
#समावेश <unistd.h>
#समावेश <errno.h>
#समावेश <stdint.h>

#अगर_घोषित HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#पूर्ण_अगर
#अगर defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#पूर्ण_अगर

/* Constants for new mount API */
#अगर_अघोषित FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#पूर्ण_अगर

#अगर_अघोषित FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#पूर्ण_अगर

#अगर_अघोषित FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#पूर्ण_अगर

#अगर_अघोषित FSCONFIG_SET_STRING
#  define FSCONFIG_SET_STRING 1
#पूर्ण_अगर

/* Additional mount API constants */
#अगर_अघोषित MOUNT_ATTR_RDONLY
#  define MOUNT_ATTR_RDONLY 0x00000001 /* Mount read-only */
#पूर्ण_अगर

#अगर_अघोषित MOUNT_ATTR_NOSUID
#  define MOUNT_ATTR_NOSUID 0x00000002
#पूर्ण_अगर

#अगर_अघोषित MOUNT_ATTR_NODEV
#  define MOUNT_ATTR_NODEV 0x00000004
#पूर्ण_अगर

#अगर_अघोषित MOUNT_ATTR_NOEXEC
#  define MOUNT_ATTR_NOEXEC 0x00000008
#पूर्ण_अगर

#अगर_अघोषित MOUNT_ATTR__ATIME
#  define MOUNT_ATTR__ATIME 0x00000070
#पूर्ण_अगर

#अगर_अघोषित MOUNT_ATTR_RELATIME
#  define MOUNT_ATTR_RELATIME 0x00000000
#पूर्ण_अगर

#अगर_अघोषित MOUNT_ATTR_NOATIME
#  define MOUNT_ATTR_NOATIME 0x00000010
#पूर्ण_अगर

#अगर_अघोषित MOUNT_ATTR_STRICTATIME
#  define MOUNT_ATTR_STRICTATIME 0x00000020
#पूर्ण_अगर

#अगर_अघोषित MOUNT_ATTR_NODIRATIME
#  define MOUNT_ATTR_NODIRATIME 0x00000080
#पूर्ण_अगर

#अगर_अघोषित MOUNT_ATTR_IDMAP
#  define MOUNT_ATTR_IDMAP 0x00100000 /* Idmap mount to @userns_fd in struct mount_attr. */
#पूर्ण_अगर

#अगर_अघोषित MOUNT_ATTR_NOSYMFOLLOW
#  define MOUNT_ATTR_NOSYMFOLLOW 0x00200000
#पूर्ण_अगर

#अगर_अघोषित MS_NOSYMFOLLOW
#  define MS_NOSYMFOLLOW 256
#पूर्ण_अगर

/* close_range flags */
#अगर_अघोषित CLOSE_RANGE_CLOEXEC
#  define CLOSE_RANGE_CLOEXEC (1U << 2)
#पूर्ण_अगर

/* openat2 resolve flags */
#अगर_अघोषित RESOLVE_NO_MAGICLINKS
#  define RESOLVE_NO_MAGICLINKS 0x02
#पूर्ण_अगर
#अगर_अघोषित RESOLVE_NO_SYMLINKS
#  define RESOLVE_NO_SYMLINKS 0x04
#पूर्ण_अगर
#अगर_अघोषित RESOLVE_IN_मूल
#  define RESOLVE_IN_मूल 0x10
#पूर्ण_अगर

/* Structures for syscalls */
काष्ठा openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

काष्ठा mount_attr_s
{
  uint64_t attr_set;
  uint64_t attr_clr;
  uint64_t propagation;
  uint64_t userns_fd;
};

#अगर_अघोषित __aligned_u64
#  define __aligned_u64 uint64_t __attribute__ ((aligned (8)))
#पूर्ण_अगर

#अगर_अघोषित CLONE_INTO_CGROUP
#  define CLONE_INTO_CGROUP 0x200000000ULL
#पूर्ण_अगर

काष्ठा _clone3_args
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 ढेर;
  __aligned_u64 ढेर_आकार;
  __aligned_u64 tls;
  __aligned_u64 set_tid;
  __aligned_u64 set_tid_आकार;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
अटल अंतरभूत पूर्णांक
syscall_fsopen (स्थिर अक्षर *fs_name, अचिन्हित पूर्णांक flags)
{
#अगर defined __NR_fsopen
  वापस (पूर्णांक) syscall (__NR_fsopen, fs_name, flags);
#अन्यथा
  (व्योम) fs_name;
  (व्योम) flags;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

अटल अंतरभूत पूर्णांक
syscall_fsmount (पूर्णांक fsfd, अचिन्हित पूर्णांक flags, अचिन्हित पूर्णांक attr_flags)
{
#अगर defined __NR_fsmount
  वापस (पूर्णांक) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#अन्यथा
  (व्योम) fsfd;
  (व्योम) flags;
  (व्योम) attr_flags;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

अटल अंतरभूत पूर्णांक
syscall_fsconfig (पूर्णांक fsfd, अचिन्हित पूर्णांक cmd, स्थिर अक्षर *कुंजी, स्थिर व्योम *val, पूर्णांक aux)
{
#अगर defined __NR_fsconfig
  वापस (पूर्णांक) syscall (__NR_fsconfig, fsfd, cmd, कुंजी, val, aux);
#अन्यथा
  (व्योम) fsfd;
  (व्योम) cmd;
  (व्योम) कुंजी;
  (व्योम) val;
  (व्योम) aux;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

/* File descriptor management syscalls */
अटल अंतरभूत पूर्णांक
syscall_close_range (अचिन्हित पूर्णांक fd, अचिन्हित पूर्णांक max_fd, अचिन्हित पूर्णांक flags)
{
#अगर_घोषित __NR_close_range
  वापस (पूर्णांक) syscall (__NR_close_range, fd, max_fd, flags);
#अन्यथा
  (व्योम) fd;
  (व्योम) max_fd;
  (व्योम) flags;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

/* Secure file operation syscalls */
अटल अंतरभूत पूर्णांक
syscall_openat2 (पूर्णांक dirfd, स्थिर अक्षर *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#अगर_घोषित __NR_openat2
  काष्ठा openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  वापस (पूर्णांक) syscall (__NR_openat2, dirfd, path, &how, माप (how), 0);
#अन्यथा
  (व्योम) dirfd;
  (व्योम) path;
  (व्योम) flags;
  (व्योम) mode;
  (व्योम) resolve;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

/* Process management syscalls */
अटल अंतरभूत पूर्णांक
syscall_clone3 (काष्ठा _clone3_args *args)
{
#अगर_घोषित __NR_clone3
  वापस (पूर्णांक) syscall (__NR_clone3, args, माप (*args));
#अन्यथा
  (व्योम) args;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

अटल अंतरभूत पूर्णांक
syscall_getcwd (अक्षर *path, माप_प्रकार len)
{
#अगर_घोषित __NR_getcwd
  वापस (पूर्णांक) syscall (__NR_getcwd, path, len);
#अन्यथा
  (व्योम) path;
  (व्योम) len;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

/* Mount management syscalls */
अटल अंतरभूत पूर्णांक
syscall_move_mount (पूर्णांक from_dfd, स्थिर अक्षर *from_pathname, पूर्णांक to_dfd, स्थिर अक्षर *to_pathname, अचिन्हित पूर्णांक flags)
{
#अगर defined __NR_move_mount
  वापस (पूर्णांक) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#अन्यथा
  (व्योम) from_dfd;
  (व्योम) from_pathname;
  (व्योम) to_dfd;
  (व्योम) to_pathname;
  (व्योम) flags;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

अटल अंतरभूत पूर्णांक
syscall_open_वृक्ष (पूर्णांक dfd, स्थिर अक्षर *pathname, अचिन्हित पूर्णांक flags)
{
#अगर defined __NR_open_tree
  वापस (पूर्णांक) syscall (__NR_open_tree, dfd, pathname, flags);
#अन्यथा
  (व्योम) dfd;
  (व्योम) pathname;
  (व्योम) flags;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

अटल अंतरभूत पूर्णांक
syscall_mount_setattr (पूर्णांक dfd, स्थिर अक्षर *path, अचिन्हित पूर्णांक flags, काष्ठा mount_attr_s *attr)
{
#अगर_घोषित __NR_mount_setattr
  वापस (पूर्णांक) syscall (__NR_mount_setattr, dfd, path, flags, attr, माप (*attr));
#अन्यथा
  (व्योम) dfd;
  (व्योम) path;
  (व्योम) flags;
  (व्योम) attr;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

#पूर्ण_अगर
