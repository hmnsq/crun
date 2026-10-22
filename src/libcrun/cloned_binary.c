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
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <stdbool.h>
#ضمن <string.h>
#ضمن <limits.h>
#ضمن <fcntl.h>
#ضمن <errno.h>

#ضمن <sys/types.h>
#ضمن <sys/stat.h>
#ضمن <sys/statfs.h>
#ضمن <sys/vfs.h>
#ضمن <sys/mman.h>
#ضمن <sys/mount.h>
#ضمن <sys/sendfile.h>
#ضمن <sys/syscall.h>

#ضمن "utils.h"
#ضمن "linux.h"

/* Use our own wrapper for memfd_create. */
#ܐܢ !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
صحيح memfd_create(ثابت حرف *name, unsigned صحيح flags)
{
#ifdef SYS_memfd_create
	ܦܢܐ syscall(SYS_memfd_create, name, flags);
#ܐܚܪܢܐ
	errno = ENOSYS;
	ܦܢܐ -1;
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
ساكن صحيح is_self_cloned(فراغ)
{
	صحيح fd, ret, is_cloned = 0;
	بنية stat statbuf = {};
	بنية statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	ܐܢ (fd < 0)
		ܦܢܐ -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	ܐܢ (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		اذهب out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	ܐܢ (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		اذهب out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	ܐܢ (!getenv(CLONED_BINARY_ENV)) {
		is_cloned = false;
		اذهب out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	ܐܢ (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	close(fd);
	ܦܢܐ is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
ساكن حرف *read_file(حرف *path, size_t *length)
{
	صحيح fd;
	حرف buf[4096], *copy = عدم;

	ܐܢ (!length)
		ܦܢܐ عدم;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	ܐܢ (fd < 0)
		ܦܢܐ عدم;

	*length = 0;
	لكل (;;) {
		ssize_t n;

		n = اقرأ(fd, buf, حجم(buf));
		ܐܢ (n < 0)
			اذهب error;
		ܐܢ (!n)
			اكسر;

		copy = xrealloc(copy, (*length + n) * حجم(*copy));
		memcpy(copy + *length, buf, n);
		*length += n;
	}
	close(fd);
	ܦܢܐ copy;

error:
	close(fd);
	حرر(copy);
	ܦܢܐ عدم;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
ساكن صحيح parse_xargs(حرف *data, صحيح data_length, حرف ***output)
{
	صحيح num = 0;
	حرف *cur = data;

	ܐܢ (!data || *output != عدم)
		ܦܢܐ -1;

	ܟܡܐ (cur < data + data_length) {
		num++;
		*output = xrealloc(*output, (num + 1) * حجم(**output));
		(*output)[num - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[num] = عدم;
	ܦܢܐ num;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
ساكن صحيح fetchve(حرف ***argv)
{
	حرف *cmdline = عدم;
	size_t cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	ܐܢ (!cmdline)
		اذهب error;

	ܐܢ (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		اذهب error;

	ܦܢܐ 0;

error:
	حرر(cmdline);
	ܦܢܐ -EINVAL;
}

تعداد {
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
#  ܐܢ defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

ساكن صحيح make_execfd(صحيح *fdtype)
{
	صحيح fd = -1;
	حرف template[PATH_MAX] = {0};
	حرف *prefix = getenv("_LIBCONTAINER_STATEDIR");

	ܐܢ (!prefix || *prefix != '/')
		prefix = "/tmp";
	ܐܢ (snprintf(template, حجم(template), "%s/crun.XXXXXX", prefix) < 0)
		ܦܢܐ -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	ܐܢ (fd >= 0)
		ܦܢܐ fd;
	ܐܢ (errno != ENOSYS && errno != EINVAL)
		اذهب error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	ܐܢ (fd >= 0) {
		بنية stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		ܐܢ (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		ܐܢ (working_otmpfile)
			ܦܢܐ fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	ܐܢ (errno != EISDIR)
		اذهب error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	ܐܢ (fd >= 0) {
		ܐܢ (unlink(template) >= 0)
			ܦܢܐ fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	ܦܢܐ -1;
}

ساكن صحيح seal_execfd(صحيح *fd, صحيح fdtype)
{
	بدل (fdtype) {
	حالة EFD_MEMFD:
		ܦܢܐ fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	حالة EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		صحيح newfd;
		حرف fdpath[PATH_MAX] = {0};

		ܐܢ (fchmod(*fd, 0100) < 0)
			ܦܢܐ -1;

		ܐܢ (snprintf(fdpath, حجم(fdpath), "/proc/self/fd/%d", *fd) < 0)
			ܦܢܐ -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		ܐܢ (newfd < 0)
			ܦܢܐ -1;

		close(*fd);
		*fd = newfd;
		ܦܢܐ 0;
	}
	افتراضي:
	   اكسر;
	}
	ܦܢܐ -1;
}

ساكن صحيح try_bindfd_mount_api(فراغ)
{
	libcrun_error_t err;
	صحيح mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	ܐܢ (mountfd < 0) {
		crun_error_release (&err);
		ܦܢܐ -1;
	}
	ܦܢܐ mountfd;
}

ساكن صحيح try_bindfd(فراغ)
{
	mode_t mask;
	صحيح fd, ret = -1;
	حرف template[PATH_MAX] = {0};
	حرف *prefix = getenv("_LIBCONTAINER_STATEDIR");

	ܐܢ (!prefix || *prefix != '/')
		prefix = "/tmp";
	ܐܢ (snprintf(template, حجم(template), "%s/crun.XXXXXX", prefix) < 0)
		ܦܢܐ ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	ܐܢ (fd < 0)
		ܦܢܐ ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	ܐܢ (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		اذهب out;
	ܐܢ (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		اذهب out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(template, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	ܐܢ (umount2(template, MNT_DETACH) < 0) {
		ܐܢ (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(template);
	ܦܢܐ ret;
}

ساكن ssize_t fd_to_fd(صحيح outfd, صحيح infd)
{
	ssize_t total = 0;
	حرف buffer[4096];

	لكل (;;) {
		ssize_t nread, nwritten = 0;

		nread = اقرأ(infd, buffer, حجم(buffer));
		ܐܢ (nread < 0)
			ܦܢܐ -1;
		ܐܢ (!nread)
			اكسر;

		ܥܒܕ {
			ssize_t n = اكتب(outfd, buffer + nwritten, nread - nwritten);
			ܐܢ (n < 0)
				ܦܢܐ -1;
			nwritten += n;
		} ܟܡܐ(nwritten < nread);

		total += nwritten;
	}

	ܦܢܐ total;
}

ساكن صحيح clone_binary(فراغ)
{
	cleanup_close صحيح execfd = -1;
	بنية stat statbuf = {};
	ssize_t sent = 0;
	صحيح fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	ܐܢ (execfd >= 0) {
		/* Transfer ownership to caller */
		صحيح ret_execfd = execfd;
		execfd = -1;
		ܦܢܐ ret_execfd;
	}
	execfd = try_bindfd();
	ܐܢ (execfd >= 0) {
		/* Transfer ownership to caller */
		صحيح ret_execfd = execfd;
		execfd = -1;
		ܦܢܐ ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	ܐܢ (execfd < 0 || fdtype == EFD_NONE)
		ܦܢܐ -ENOTRECOVERABLE;

	{
		cleanup_close صحيح binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		ܐܢ (binfd < 0)
			اذهب error;

		ܐܢ (fstat(binfd, &statbuf) < 0)
			اذهب error;

		ܟܡܐ (sent < statbuf.st_size) {
			صحيح n = sendfile(execfd, binfd, عدم, statbuf.st_size - sent);
			ܐܢ (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				ܐܢ (n < 0)
					اذهب error;
			}
			sent += n;
		}
	}
	ܐܢ (sent != statbuf.st_size)
		اذهب error;

	ܐܢ (seal_execfd(&execfd, fdtype) < 0)
		اذهب error;

	{
		/* Transfer ownership to caller */
		صحيح ret_execfd = execfd;
		execfd = -1;
		ܦܢܐ ret_execfd;
	}
error:
	ܦܢܐ -EIO;
}

/* Get cheap access to the environment. */
خارجي حرف **environ;

صحيح ensure_cloned_binary(فراغ)
{
	cleanup_close صحيح execfd = -1;
	حرف **argv = عدم;

	/* Check that we're not self-cloned, and if we are then bail. */
	صحيح cloned = is_self_cloned();
	ܐܢ (cloned > 0 || cloned == -ENOTRECOVERABLE)
		ܦܢܐ cloned;

	ܐܢ (fetchve(&argv) < 0)
		ܦܢܐ -EINVAL;

	execfd = clone_binary();
	ܐܢ (execfd < 0)
		ܦܢܐ -EIO;

	ܐܢ (putenv(CLONED_BINARY_ENV "=1"))
		اذهب error;

	fexecve(execfd, argv, environ);
error:
	ܦܢܐ -ENOEXEC;
}
