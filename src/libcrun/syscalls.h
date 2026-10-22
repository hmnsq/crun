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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <sys/syscall.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>

#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#ଯଦି defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
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
#ifndef RESOLVE_IN_ମୂଳ
#  define RESOLVE_IN_ମୂଳ 0x10
#endif

/* Structures for syscalls */
ସଂରଚନା openat2_open_how
{
  uint64_t flags;
  uint64_t mode;
  uint64_t resolve;
};

ସଂରଚନା mount_attr_s
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

ସଂରଚନା _clone3_args
{
  __aligned_u64 flags;
  __aligned_u64 pidfd;
  __aligned_u64 child_tid;
  __aligned_u64 parent_tid;
  __aligned_u64 exit_signal;
  __aligned_u64 ଗଦା;
  __aligned_u64 ଗଦା_size;
  __aligned_u64 tls;
  __aligned_u64 set_tid;
  __aligned_u64 set_tid_size;
  __aligned_u64 cgroup;
};

/* Mount API syscall wrappers */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_fsopen (ଅଚଳ ଅକ୍ଷର *fs_name, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
#ଯଦି defined __NR_fsopen
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_fsopen, fs_name, flags);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) fs_name;
  (ଶୂନ୍ୟ) flags;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_fsmount (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsfd, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା attr_flags)
{
#ଯଦି defined __NR_fsmount
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_fsmount, fsfd, flags, attr_flags);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) fsfd;
  (ଶୂନ୍ୟ) flags;
  (ଶୂନ୍ୟ) attr_flags;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_fsconfig (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fsfd, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd, ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଶୂନ୍ୟ *val, ପୂର୍ଣ୍ଣସଂଖ୍ୟା aux)
{
#ଯଦି defined __NR_fsconfig
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_fsconfig, fsfd, cmd, ଚାବି, val, aux);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) fsfd;
  (ଶୂନ୍ୟ) cmd;
  (ଶୂନ୍ୟ) ଚାବି;
  (ଶୂନ୍ୟ) val;
  (ଶୂନ୍ୟ) aux;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

/* File descriptor management syscalls */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_close_range (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_fd, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
#ifdef __NR_close_range
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_close_range, fd, max_fd, flags);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) fd;
  (ଶୂନ୍ୟ) max_fd;
  (ଶୂନ୍ୟ) flags;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

/* Secure file operation syscalls */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_openat2 (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *path, uint64_t flags, uint64_t mode, uint64_t resolve)
{
#ifdef __NR_openat2
  ସଂରଚନା openat2_open_how how = {
    .flags = flags,
    .mode = mode,
    .resolve = resolve,
  };
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_openat2, dirfd, path, &how, ଆକାର (how), 0);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) dirfd;
  (ଶୂନ୍ୟ) path;
  (ଶୂନ୍ୟ) flags;
  (ଶୂନ୍ୟ) mode;
  (ଶୂନ୍ୟ) resolve;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

/* Process management syscalls */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_clone3 (ସଂରଚନା _clone3_args *args)
{
#ifdef __NR_clone3
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_clone3, args, ଆକାର (*args));
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) args;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_getcwd (ଅକ୍ଷର *path, size_t len)
{
#ifdef __NR_getcwd
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_getcwd, path, len);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) path;
  (ଶୂନ୍ୟ) len;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

/* Mount management syscalls */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_move_mount (ପୂର୍ଣ୍ଣସଂଖ୍ୟା from_dfd, ଅଚଳ ଅକ୍ଷର *from_pathname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_dfd, ଅଚଳ ଅକ୍ଷର *to_pathname, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
#ଯଦି defined __NR_move_mount
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_move_mount, from_dfd, from_pathname, to_dfd, to_pathname, flags);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) from_dfd;
  (ଶୂନ୍ୟ) from_pathname;
  (ଶୂନ୍ୟ) to_dfd;
  (ଶୂନ୍ୟ) to_pathname;
  (ଶୂନ୍ୟ) flags;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_open_ବୃକ୍ଷ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd, ଅଚଳ ଅକ୍ଷର *pathname, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
#ଯଦି defined __NR_open_tree
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_open_tree, dfd, pathname, flags);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) dfd;
  (ଶୂନ୍ୟ) pathname;
  (ଶୂନ୍ୟ) flags;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_mount_setattr (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd, ଅଚଳ ଅକ୍ଷର *path, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ସଂରଚନା mount_attr_s *attr)
{
#ifdef __NR_mount_setattr
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_mount_setattr, dfd, path, flags, attr, ଆକାର (*attr));
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) dfd;
  (ଶୂନ୍ୟ) path;
  (ଶୂନ୍ୟ) flags;
  (ଶୂନ୍ୟ) attr;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

#endif
