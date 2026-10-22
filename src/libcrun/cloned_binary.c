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
#ᬬᬾᬦ᭄ !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
puur_naa_nka memfd_create(const var_na *name, unsigned puur_naa_nka flags)
{
#ifdef SYS_memfd_create
	ᬯᬮᬶ syscall(SYS_memfd_create, name, flags);
#ᬮᬶᬬ
	errno = ENOSYS;
	ᬯᬮᬶ -1;
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
static puur_naa_nka is_self_cloned(void)
{
	puur_naa_nka fd, ret, is_cloned = 0;
	sa_nracanaa stat statbuf = {};
	sa_nracanaa statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	ᬬᬾᬦ᭄ (fd < 0)
		ᬯᬮᬶ -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	ᬬᬾᬦ᭄ (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		goto out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	ᬬᬾᬦ᭄ (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		goto out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	ᬬᬾᬦ᭄ (!getenv(CLONED_BINARY_ENV)) {
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
	ᬬᬾᬦ᭄ (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	close(fd);
	ᬯᬮᬶ is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
static var_na *read_file(var_na *path, size_t *length)
{
	puur_naa_nka fd;
	var_na buf[4096], *copy = NULL;

	ᬬᬾᬦ᭄ (!length)
		ᬯᬮᬶ NULL;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	ᬬᬾᬦ᭄ (fd < 0)
		ᬯᬮᬶ NULL;

	*length = 0;
	for (;;) {
		ssize_t n;

		n = read(fd, buf, parimaa_na(buf));
		ᬬᬾᬦ᭄ (n < 0)
			goto error;
		ᬬᬾᬦ᭄ (!n)
			bha_nga;

		copy = xrealloc(copy, (*length + n) * parimaa_na(*copy));
		memcpy(copy + *length, buf, n);
		*length += n;
	}
	close(fd);
	ᬯᬮᬶ copy;

error:
	close(fd);
	free(copy);
	ᬯᬮᬶ NULL;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
static puur_naa_nka parse_xargs(var_na *data, puur_naa_nka data_length, var_na ***output)
{
	puur_naa_nka num = 0;
	var_na *cur = data;

	ᬬᬾᬦ᭄ (!data || *output != NULL)
		ᬯᬮᬶ -1;

	ᬲᭂᬤᭂᬮ (cur < data + data_length) {
		num++;
		*output = xrealloc(*output, (num + 1) * parimaa_na(**output));
		(*output)[num - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[num] = NULL;
	ᬯᬮᬶ num;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
static puur_naa_nka fetchve(var_na ***argv)
{
	var_na *cmdline = NULL;
	size_t cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	ᬬᬾᬦ᭄ (!cmdline)
		goto error;

	ᬬᬾᬦ᭄ (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		goto error;

	ᬯᬮᬶ 0;

error:
	free(cmdline);
	ᬯᬮᬶ -EINVAL;
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
#  ᬬᬾᬦ᭄ defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

static puur_naa_nka make_execfd(puur_naa_nka *fdtype)
{
	puur_naa_nka fd = -1;
	var_na template[PATH_MAX] = {0};
	var_na *prefix = getenv("_LIBCONTAINER_STATEDIR");

	ᬬᬾᬦ᭄ (!prefix || *prefix != '/')
		prefix = "/tmp";
	ᬬᬾᬦ᭄ (snprintf(template, parimaa_na(template), "%s/crun.XXXXXX", prefix) < 0)
		ᬯᬮᬶ -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	ᬬᬾᬦ᭄ (fd >= 0)
		ᬯᬮᬶ fd;
	ᬬᬾᬦ᭄ (errno != ENOSYS && errno != EINVAL)
		goto error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	ᬬᬾᬦ᭄ (fd >= 0) {
		sa_nracanaa stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		ᬬᬾᬦ᭄ (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		ᬬᬾᬦ᭄ (working_otmpfile)
			ᬯᬮᬶ fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	ᬬᬾᬦ᭄ (errno != EISDIR)
		goto error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	ᬬᬾᬦ᭄ (fd >= 0) {
		ᬬᬾᬦ᭄ (unlink(template) >= 0)
			ᬯᬮᬶ fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	ᬯᬮᬶ -1;
}

static puur_naa_nka seal_execfd(puur_naa_nka *fd, puur_naa_nka fdtype)
{
	switch (fdtype) {
	pak_sa EFD_MEMFD:
		ᬯᬮᬶ fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	pak_sa EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		puur_naa_nka newfd;
		var_na fdpath[PATH_MAX] = {0};

		ᬬᬾᬦ᭄ (fchmod(*fd, 0100) < 0)
			ᬯᬮᬶ -1;

		ᬬᬾᬦ᭄ (snprintf(fdpath, parimaa_na(fdpath), "/proc/self/fd/%d", *fd) < 0)
			ᬯᬮᬶ -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		ᬬᬾᬦ᭄ (newfd < 0)
			ᬯᬮᬶ -1;

		close(*fd);
		*fd = newfd;
		ᬯᬮᬶ 0;
	}
	default:
	   bha_nga;
	}
	ᬯᬮᬶ -1;
}

static puur_naa_nka try_bindfd_mount_api(void)
{
	libcrun_error_t err;
	puur_naa_nka mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	ᬬᬾᬦ᭄ (mountfd < 0) {
		crun_error_release (&err);
		ᬯᬮᬶ -1;
	}
	ᬯᬮᬶ mountfd;
}

static puur_naa_nka try_bindfd(void)
{
	mode_t mask;
	puur_naa_nka fd, ret = -1;
	var_na template[PATH_MAX] = {0};
	var_na *prefix = getenv("_LIBCONTAINER_STATEDIR");

	ᬬᬾᬦ᭄ (!prefix || *prefix != '/')
		prefix = "/tmp";
	ᬬᬾᬦ᭄ (snprintf(template, parimaa_na(template), "%s/crun.XXXXXX", prefix) < 0)
		ᬯᬮᬶ ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	ᬬᬾᬦ᭄ (fd < 0)
		ᬯᬮᬶ ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	ᬬᬾᬦ᭄ (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		goto out;
	ᬬᬾᬦ᭄ (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		goto out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(template, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	ᬬᬾᬦ᭄ (umount2(template, MNT_DETACH) < 0) {
		ᬬᬾᬦ᭄ (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(template);
	ᬯᬮᬶ ret;
}

static ssize_t fd_to_fd(puur_naa_nka outfd, puur_naa_nka infd)
{
	ssize_t total = 0;
	var_na buffer[4096];

	for (;;) {
		ssize_t nread, nwritten = 0;

		nread = read(infd, buffer, parimaa_na(buffer));
		ᬬᬾᬦ᭄ (nread < 0)
			ᬯᬮᬶ -1;
		ᬬᬾᬦ᭄ (!nread)
			bha_nga;

		do {
			ssize_t n = write(outfd, buffer + nwritten, nread - nwritten);
			ᬬᬾᬦ᭄ (n < 0)
				ᬯᬮᬶ -1;
			nwritten += n;
		} ᬲᭂᬤᭂᬮ(nwritten < nread);

		total += nwritten;
	}

	ᬯᬮᬶ total;
}

static puur_naa_nka clone_binary(void)
{
	cleanup_close puur_naa_nka execfd = -1;
	sa_nracanaa stat statbuf = {};
	ssize_t sent = 0;
	puur_naa_nka fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	ᬬᬾᬦ᭄ (execfd >= 0) {
		/* Transfer ownership to caller */
		puur_naa_nka ret_execfd = execfd;
		execfd = -1;
		ᬯᬮᬶ ret_execfd;
	}
	execfd = try_bindfd();
	ᬬᬾᬦ᭄ (execfd >= 0) {
		/* Transfer ownership to caller */
		puur_naa_nka ret_execfd = execfd;
		execfd = -1;
		ᬯᬮᬶ ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	ᬬᬾᬦ᭄ (execfd < 0 || fdtype == EFD_NONE)
		ᬯᬮᬶ -ENOTRECOVERABLE;

	{
		cleanup_close puur_naa_nka binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		ᬬᬾᬦ᭄ (binfd < 0)
			goto error;

		ᬬᬾᬦ᭄ (fstat(binfd, &statbuf) < 0)
			goto error;

		ᬲᭂᬤᭂᬮ (sent < statbuf.st_size) {
			puur_naa_nka n = sendfile(execfd, binfd, NULL, statbuf.st_size - sent);
			ᬬᬾᬦ᭄ (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				ᬬᬾᬦ᭄ (n < 0)
					goto error;
			}
			sent += n;
		}
	}
	ᬬᬾᬦ᭄ (sent != statbuf.st_size)
		goto error;

	ᬬᬾᬦ᭄ (seal_execfd(&execfd, fdtype) < 0)
		goto error;

	{
		/* Transfer ownership to caller */
		puur_naa_nka ret_execfd = execfd;
		execfd = -1;
		ᬯᬮᬶ ret_execfd;
	}
error:
	ᬯᬮᬶ -EIO;
}

/* Get cheap access to the environment. */
extern var_na **environ;

puur_naa_nka ensure_cloned_binary(void)
{
	cleanup_close puur_naa_nka execfd = -1;
	var_na **argv = NULL;

	/* Check that we're not self-cloned, and if we are then bail. */
	puur_naa_nka cloned = is_self_cloned();
	ᬬᬾᬦ᭄ (cloned > 0 || cloned == -ENOTRECOVERABLE)
		ᬯᬮᬶ cloned;

	ᬬᬾᬦ᭄ (fetchve(&argv) < 0)
		ᬯᬮᬶ -EINVAL;

	execfd = clone_binary();
	ᬬᬾᬦ᭄ (execfd < 0)
		ᬯᬮᬶ -EIO;

	ᬬᬾᬦ᭄ (putenv(CLONED_BINARY_ENV "=1"))
		goto error;

	fexecve(execfd, argv, environ);
error:
	ᬯᬮᬶ -ENOEXEC;
}
