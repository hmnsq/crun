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

#अन्तर्भाव <config.h>
#अन्तर्भाव <sys/syscall.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <stdint.h>

#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#जर defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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
#ifndef RESOLVE_IN_मूळ
#  define RESOLVE_IN_मूळ 0x10
#endif

/* Structures for syscalls */
संरचना openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

संरचना mount_attr_s
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

संरचना _clone3_args
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 चळत;
  __aligned_u64 चळत_size;
  __aligned_u64 tls;
  __aligned_u64 set_tid;
  __aligned_u64 set_tid_size;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
स्थिर ओळीत पूर्णांक
syscall_fsopen (स्थिरांक अक्षर *fs_name, अचिन्हित पूर्णांक flags)
{
#जर defined __NR_fsopen
  परत (पूर्णांक) syscall (__NR_fsopen, fs_name, flags);
#अन्यथा
  (शून्य) fs_name;
  (शून्य) flags;
  त्रुटि_सं = ENOSYS;
  परत -1;
#endif
}

स्थिर ओळीत पूर्णांक
syscall_fsmount (पूर्णांक fsfd, अचिन्हित पूर्णांक flags, अचिन्हित पूर्णांक attr_flags)
{
#जर defined __NR_fsmount
  परत (पूर्णांक) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#अन्यथा
  (शून्य) fsfd;
  (शून्य) flags;
  (शून्य) attr_flags;
  त्रुटि_सं = ENOSYS;
  परत -1;
#endif
}

स्थिर ओळीत पूर्णांक
syscall_fsconfig (पूर्णांक fsfd, अचिन्हित पूर्णांक cmd, स्थिरांक अक्षर *किल्ली, स्थिरांक शून्य *val, पूर्णांक aux)
{
#जर defined __NR_fsconfig
  परत (पूर्णांक) syscall (__NR_fsconfig, fsfd, cmd, किल्ली, val, aux);
#अन्यथा
  (शून्य) fsfd;
  (शून्य) cmd;
  (शून्य) किल्ली;
  (शून्य) val;
  (शून्य) aux;
  त्रुटि_सं = ENOSYS;
  परत -1;
#endif
}

/* File descriptor management syscalls */
स्थिर ओळीत पूर्णांक
syscall_close_range (अचिन्हित पूर्णांक fd, अचिन्हित पूर्णांक max_fd, अचिन्हित पूर्णांक flags)
{
#ifdef __NR_close_range
  परत (पूर्णांक) syscall (__NR_close_range, fd, max_fd, flags);
#अन्यथा
  (शून्य) fd;
  (शून्य) max_fd;
  (शून्य) flags;
  त्रुटि_सं = ENOSYS;
  परत -1;
#endif
}

/* Secure file operation syscalls */
स्थिर ओळीत पूर्णांक
syscall_openat2 (पूर्णांक dirfd, स्थिरांक अक्षर *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  संरचना openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  परत (पूर्णांक) syscall (__NR_openat2, dirfd, path, &how, आकार (how), 0);
#अन्यथा
  (शून्य) dirfd;
  (शून्य) path;
  (शून्य) flags;
  (शून्य) mode;
  (शून्य) resolve;
  त्रुटि_सं = ENOSYS;
  परत -1;
#endif
}

/* Process management syscalls */
स्थिर ओळीत पूर्णांक
syscall_clone3 (संरचना _clone3_args *args)
{
#ifdef __NR_clone3
  परत (पूर्णांक) syscall (__NR_clone3, args, आकार (*args));
#अन्यथा
  (शून्य) args;
  त्रुटि_सं = ENOSYS;
  परत -1;
#endif
}

स्थिर ओळीत पूर्णांक
syscall_getcwd (अक्षर *path, माप_प्रकार len)
{
#ifdef __NR_getcwd
  परत (पूर्णांक) syscall (__NR_getcwd, path, len);
#अन्यथा
  (शून्य) path;
  (शून्य) len;
  त्रुटि_सं = ENOSYS;
  परत -1;
#endif
}

/* Mount management syscalls */
स्थिर ओळीत पूर्णांक
syscall_move_mount (पूर्णांक from_dfd, स्थिरांक अक्षर *from_pathname, पूर्णांक to_dfd, स्थिरांक अक्षर *to_pathname, अचिन्हित पूर्णांक flags)
{
#जर defined __NR_move_mount
  परत (पूर्णांक) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#अन्यथा
  (शून्य) from_dfd;
  (शून्य) from_pathname;
  (शून्य) to_dfd;
  (शून्य) to_pathname;
  (शून्य) flags;
  त्रुटि_सं = ENOSYS;
  परत -1;
#endif
}

स्थिर ओळीत पूर्णांक
syscall_open_वृक्ष (पूर्णांक dfd, स्थिरांक अक्षर *pathname, अचिन्हित पूर्णांक flags)
{
#जर defined __NR_open_tree
  परत (पूर्णांक) syscall (__NR_open_tree, dfd, pathname, flags);
#अन्यथा
  (शून्य) dfd;
  (शून्य) pathname;
  (शून्य) flags;
  त्रुटि_सं = ENOSYS;
  परत -1;
#endif
}

स्थिर ओळीत पूर्णांक
syscall_mount_setattr (पूर्णांक dfd, स्थिरांक अक्षर *path, अचिन्हित पूर्णांक flags, संरचना mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  परत (पूर्णांक) syscall (__NR_mount_setattr, dfd, path, flags, attr, आकार (*attr));
#अन्यथा
  (शून्य) dfd;
  (शून्य) path;
  (शून्य) flags;
  (शून्य) attr;
  त्रुटि_सं = ENOSYS;
  परत -1;
#endif
}

#endif
