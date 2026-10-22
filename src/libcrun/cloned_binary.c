// SPDX-License-Identifier: Apache-2.0 OR LGPL-2.1-or-later
/*
 * Copyright (C) 2019 Aleksa Sarai <cyphar@cyphar.com>
 * Copyright (C) 2019 SUSE LLC
 *
 * This work is dual licensed under the following licenses. You may use,
 * redistribute, and/or modify the work under the conditions of either (or
 * both) licenses.
 *
 * === Apache-2.0 ===
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * === LGPL-2.1-or-later ===
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <https://www.gnu.org/licenses/>.
 *
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/vfs.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/sendfile.h>
#include <sys/syscall.h>

#include "utils.h"
#include "linux.h"

/* Use our own wrapper for memfd_create. */
#အကယ်၍ !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
ကိန်းပြည့် memfd_create(const var_na *name, unsigned ကိန်းပြည့် flags)
{
#ifdef SYS_memfd_create
	ပြန်ပေး syscall(SYS_memfd_create, name, flags);
#မဟုတ်ပါက
	errno = ENOSYS;
	ပြန်ပေး -1;
#endif
}


/* This comes directly from <linux/fcntl.h>. */
#ifndef F_LINUX_SPECIFIC_BASE
#  define F_LINUX_SPECIFIC_BASE 1024
#endif
#ifndef F_ADD_SEALS
#  define F_ADD_SEALS (F_LINUX_SPECIFIC_BASE + 9)
#  define F_GET_SEALS (F_LINUX_SPECIFIC_BASE + 10)
#endif
#ifndef F_SEAL_SEAL
#  define F_SEAL_SEAL   0x0001	/* prevent further seals from being set */
#  define F_SEAL_SHRINK 0x0002	/* prevent file from shrinking */
#  define F_SEAL_GROW   0x0004	/* prevent file from growing */
#  define F_SEAL_WRITE  0x0008	/* prevent writes */
#endif

#define CLONED_BINARY_ENV "_LIBCONTAINER_CLONED_BINARY"
#define CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#define CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
static ကိန်းပြည့် is_self_cloned(void)
{
	ကိန်းပြည့် fd, ret, is_cloned = 0;
	sa_nracanaa stat statbuf = {};
	sa_nracanaa statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	အကယ်၍ (fd < 0)
		ပြန်ပေး -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	အကယ်၍ (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		goto out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	အကယ်၍ (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		goto out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	အကယ်၍ (!getenv(CLONED_BINARY_ENV)) {
		is_cloned = false;
		goto out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	အကယ်၍ (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	close(fd);
	ပြန်ပေး is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
static var_na *read_file(var_na *path, size_t *length)
{
	ကိန်းပြည့် fd;
	var_na buf[4096], *copy = NULL;

	အကယ်၍ (!length)
		ပြန်ပေး NULL;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	အကယ်၍ (fd < 0)
		ပြန်ပေး NULL;

	*length = 0;
	အတွက် (;;) {
		ssize_t n;

		n = read(fd, buf, parimaa_na(buf));
		အကယ်၍ (n < 0)
			goto error;
		အကယ်၍ (!n)
			bha_nga;

		copy = xrealloc(copy, (*length + n) * parimaa_na(*copy));
		memcpy(copy + *length, buf, n);
		*length += n;
	}
	close(fd);
	ပြန်ပေး copy;

error:
	close(fd);
	free(copy);
	ပြန်ပေး NULL;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
static ကိန်းပြည့် parse_xargs(var_na *data, ကိန်းပြည့် data_length, var_na ***output)
{
	ကိန်းပြည့် num = 0;
	var_na *cur = data;

	အကယ်၍ (!data || *output != NULL)
		ပြန်ပေး -1;

	နေစဉ် (cur < data + data_length) {
		num++;
		*output = xrealloc(*output, (num + 1) * parimaa_na(**output));
		(*output)[num - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[num] = NULL;
	ပြန်ပေး num;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
static ကိန်းပြည့် fetchve(var_na ***argv)
{
	var_na *cmdline = NULL;
	size_t cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	အကယ်၍ (!cmdline)
		goto error;

	အကယ်၍ (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		goto error;

	ပြန်ပေး 0;

error:
	free(cmdline);
	ပြန်ပေး -EINVAL;
}

ga_nanaa {
	EFD_NONE = 0,
	EFD_MEMFD,
	EFD_FILE,
};

/*
 * This comes from <linux/fcntl.h>. We can't hard-code __O_TMPFILE because it
 * changes depending on the architecture. If we don't have O_TMPFILE we always
 * have the mkostemp(3) fallback.
 */
#ifndef O_TMPFILE
#  အကယ်၍ defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

static ကိန်းပြည့် make_execfd(ကိန်းပြည့် *fdtype)
{
	ကိန်းပြည့် fd = -1;
	var_na template[PATH_MAX] = {0};
	var_na *prefix = getenv("_LIBCONTAINER_STATEDIR");

	အကယ်၍ (!prefix || *prefix != '/')
		prefix = "/tmp";
	အကယ်၍ (snprintf(template, parimaa_na(template), "%s/crun.XXXXXX", prefix) < 0)
		ပြန်ပေး -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	အကယ်၍ (fd >= 0)
		ပြန်ပေး fd;
	အကယ်၍ (errno != ENOSYS && errno != EINVAL)
		goto error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	အကယ်၍ (fd >= 0) {
		sa_nracanaa stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		အကယ်၍ (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		အကယ်၍ (working_otmpfile)
			ပြန်ပေး fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	အကယ်၍ (errno != EISDIR)
		goto error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	အကယ်၍ (fd >= 0) {
		အကယ်၍ (unlink(template) >= 0)
			ပြန်ပေး fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	ပြန်ပေး -1;
}

static ကိန်းပြည့် seal_execfd(ကိန်းပြည့် *fd, ကိန်းပြည့် fdtype)
{
	switch (fdtype) {
	pak_sa EFD_MEMFD:
		ပြန်ပေး fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	pak_sa EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		ကိန်းပြည့် newfd;
		var_na fdpath[PATH_MAX] = {0};

		အကယ်၍ (fchmod(*fd, 0100) < 0)
			ပြန်ပေး -1;

		အကယ်၍ (snprintf(fdpath, parimaa_na(fdpath), "/proc/self/fd/%d", *fd) < 0)
			ပြန်ပေး -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		အကယ်၍ (newfd < 0)
			ပြန်ပေး -1;

		close(*fd);
		*fd = newfd;
		ပြန်ပေး 0;
	}
	default:
	   bha_nga;
	}
	ပြန်ပေး -1;
}

static ကိန်းပြည့် try_bindfd_mount_api(void)
{
	libcrun_error_t err;
	ကိန်းပြည့် mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	အကယ်၍ (mountfd < 0) {
		crun_error_release (&err);
		ပြန်ပေး -1;
	}
	ပြန်ပေး mountfd;
}

static ကိန်းပြည့် try_bindfd(void)
{
	mode_t mask;
	ကိန်းပြည့် fd, ret = -1;
	var_na template[PATH_MAX] = {0};
	var_na *prefix = getenv("_LIBCONTAINER_STATEDIR");

	အကယ်၍ (!prefix || *prefix != '/')
		prefix = "/tmp";
	အကယ်၍ (snprintf(template, parimaa_na(template), "%s/crun.XXXXXX", prefix) < 0)
		ပြန်ပေး ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	အကယ်၍ (fd < 0)
		ပြန်ပေး ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	အကယ်၍ (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		goto out;
	အကယ်၍ (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		goto out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(template, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	အကယ်၍ (umount2(template, MNT_DETACH) < 0) {
		အကယ်၍ (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(template);
	ပြန်ပေး ret;
}

static ssize_t fd_to_fd(ကိန်းပြည့် outfd, ကိန်းပြည့် infd)
{
	ssize_t total = 0;
	var_na buffer[4096];

	အတွက် (;;) {
		ssize_t nread, nwritten = 0;

		nread = read(infd, buffer, parimaa_na(buffer));
		အကယ်၍ (nread < 0)
			ပြန်ပေး -1;
		အကယ်၍ (!nread)
			bha_nga;

		do {
			ssize_t n = write(outfd, buffer + nwritten, nread - nwritten);
			အကယ်၍ (n < 0)
				ပြန်ပေး -1;
			nwritten += n;
		} နေစဉ်(nwritten < nread);

		total += nwritten;
	}

	ပြန်ပေး total;
}

static ကိန်းပြည့် clone_binary(void)
{
	cleanup_close ကိန်းပြည့် execfd = -1;
	sa_nracanaa stat statbuf = {};
	ssize_t sent = 0;
	ကိန်းပြည့် fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	အကယ်၍ (execfd >= 0) {
		/* Transfer ownership to caller */
		ကိန်းပြည့် ret_execfd = execfd;
		execfd = -1;
		ပြန်ပေး ret_execfd;
	}
	execfd = try_bindfd();
	အကယ်၍ (execfd >= 0) {
		/* Transfer ownership to caller */
		ကိန်းပြည့် ret_execfd = execfd;
		execfd = -1;
		ပြန်ပေး ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	အကယ်၍ (execfd < 0 || fdtype == EFD_NONE)
		ပြန်ပေး -ENOTRECOVERABLE;

	{
		cleanup_close ကိန်းပြည့် binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		အကယ်၍ (binfd < 0)
			goto error;

		အကယ်၍ (fstat(binfd, &statbuf) < 0)
			goto error;

		နေစဉ် (sent < statbuf.st_size) {
			ကိန်းပြည့် n = sendfile(execfd, binfd, NULL, statbuf.st_size - sent);
			အကယ်၍ (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				အကယ်၍ (n < 0)
					goto error;
			}
			sent += n;
		}
	}
	အကယ်၍ (sent != statbuf.st_size)
		goto error;

	အကယ်၍ (seal_execfd(&execfd, fdtype) < 0)
		goto error;

	{
		/* Transfer ownership to caller */
		ကိန်းပြည့် ret_execfd = execfd;
		execfd = -1;
		ပြန်ပေး ret_execfd;
	}
error:
	ပြန်ပေး -EIO;
}

/* Get cheap access to the environment. */
extern var_na **environ;

ကိန်းပြည့် ensure_cloned_binary(void)
{
	cleanup_close ကိန်းပြည့် execfd = -1;
	var_na **argv = NULL;

	/* Check that we're not self-cloned, and if we are then bail. */
	ကိန်းပြည့် cloned = is_self_cloned();
	အကယ်၍ (cloned > 0 || cloned == -ENOTRECOVERABLE)
		ပြန်ပေး cloned;

	အကယ်၍ (fetchve(&argv) < 0)
		ပြန်ပေး -EINVAL;

	execfd = clone_binary();
	အကယ်၍ (execfd < 0)
		ပြန်ပေး -EIO;

	အကယ်၍ (putenv(CLONED_BINARY_ENV "=1"))
		goto error;

	fexecve(execfd, argv, environ);
error:
	ပြန်ပေး -ENOEXEC;
}
