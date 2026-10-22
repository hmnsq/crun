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
#شامل <unistd.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <stdbool.h>
#شامل <string.h>
#شامل <limits.h>
#شامل <fcntl.h>
#شامل <errno.h>

#شامل <sys/types.h>
#شامل <sys/stat.h>
#شامل <sys/statfs.h>
#شامل <sys/vfs.h>
#شامل <sys/mman.h>
#شامل <sys/mount.h>
#شامل <sys/sendfile.h>
#شامل <sys/syscall.h>

#شامل "utils.h"
#شامل "linux.h"

/* Use our own wrapper for memfd_create. */
#اگر !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
صحیح memfd_create(ثابت نویسه *name, unsigned صحیح flags)
{
#ifdef SYS_memfd_create
	بازگشت syscall(SYS_memfd_create, name, flags);
#وگرنه
	errno = ENOSYS;
	بازگشت -1;
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

#define CLONED_دودویی_ENV "_LIBCONTAINER_CLONED_BINARY"
#define CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#define CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
ایستا صحیح is_self_cloned(تهی)
{
	صحیح fd, ret, is_cloned = 0;
	ساختار stat statbuf = {};
	ساختار statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	اگر (fd < 0)
		بازگشت -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	اگر (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		برو out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	اگر (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		برو out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	اگر (!getenv(CLONED_دودویی_ENV)) {
		is_cloned = false;
		برو out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	اگر (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	close(fd);
	بازگشت is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
ایستا نویسه *read_file(نویسه *path, size_t *length)
{
	صحیح fd;
	نویسه buf[4096], *copy = پوچ;

	اگر (!length)
		بازگشت پوچ;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	اگر (fd < 0)
		بازگشت پوچ;

	*length = 0;
	برای (;;) {
		ssize_t n;

		n = اقرأ(fd, buf, اندازه(buf));
		اگر (n < 0)
			برو error;
		اگر (!n)
			بشکن;

		copy = xrealloc(copy, (*length + n) * اندازه(*copy));
		memcpy(copy + *length, buf, n);
		*length += n;
	}
	close(fd);
	بازگشت copy;

error:
	close(fd);
	آزاد(copy);
	بازگشت پوچ;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
ایستا صحیح parse_xargs(نویسه *data, صحیح data_length, نویسه ***output)
{
	صحیح رقم = 0;
	نویسه *cur = data;

	اگر (!data || *output != پوچ)
		بازگشت -1;

	وقتی (cur < data + data_length) {
		رقم++;
		*output = xrealloc(*output, (رقم + 1) * اندازه(**output));
		(*output)[رقم - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[رقم] = پوچ;
	بازگشت رقم;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
ایستا صحیح fetchve(نویسه ***argv)
{
	نویسه *cmdline = پوچ;
	size_t cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	اگر (!cmdline)
		برو error;

	اگر (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		برو error;

	بازگشت 0;

error:
	آزاد(cmdline);
	بازگشت -EINVAL;
}

شمارش {
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
#  اگر defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

ایستا صحیح make_execfd(صحیح *fdtype)
{
	صحیح fd = -1;
	نویسه template[PATH_MAX] = {0};
	نویسه *prefix = getenv("_LIBCONTAINER_STATEDIR");

	اگر (!prefix || *prefix != '/')
		prefix = "/tmp";
	اگر (snprintf(template, اندازه(template), "%s/crun.XXXXXX", prefix) < 0)
		بازگشت -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	اگر (fd >= 0)
		بازگشت fd;
	اگر (errno != ENOSYS && errno != EINVAL)
		برو error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	اگر (fd >= 0) {
		ساختار stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		اگر (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		اگر (working_otmpfile)
			بازگشت fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	اگر (errno != EISDIR)
		برو error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	اگر (fd >= 0) {
		اگر (unlink(template) >= 0)
			بازگشت fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	بازگشت -1;
}

ایستا صحیح seal_execfd(صحیح *fd, صحیح fdtype)
{
	انتخاب (fdtype) {
	حالت EFD_MEMFD:
		بازگشت fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	حالت EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		صحیح newfd;
		نویسه fdpath[PATH_MAX] = {0};

		اگر (fchmod(*fd, 0100) < 0)
			بازگشت -1;

		اگر (snprintf(fdpath, اندازه(fdpath), "/proc/self/fd/%d", *fd) < 0)
			بازگشت -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		اگر (newfd < 0)
			بازگشت -1;

		close(*fd);
		*fd = newfd;
		بازگشت 0;
	}
	پیشفرض:
	   بشکن;
	}
	بازگشت -1;
}

ایستا صحیح try_bindfd_mount_api(تهی)
{
	libcrun_error_t err;
	صحیح mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	اگر (mountfd < 0) {
		crun_error_release (&err);
		بازگشت -1;
	}
	بازگشت mountfd;
}

ایستا صحیح try_bindfd(تهی)
{
	mode_t mask;
	صحیح fd, ret = -1;
	نویسه template[PATH_MAX] = {0};
	نویسه *prefix = getenv("_LIBCONTAINER_STATEDIR");

	اگر (!prefix || *prefix != '/')
		prefix = "/tmp";
	اگر (snprintf(template, اندازه(template), "%s/crun.XXXXXX", prefix) < 0)
		بازگشت ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	اگر (fd < 0)
		بازگشت ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	اگر (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		برو out;
	اگر (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		برو out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(template, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	اگر (umount2(template, MNT_DETACH) < 0) {
		اگر (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(template);
	بازگشت ret;
}

ایستا ssize_t fd_to_fd(صحیح outfd, صحیح infd)
{
	ssize_t total = 0;
	نویسه buffer[4096];

	برای (;;) {
		ssize_t nread, nwritten = 0;

		nread = اقرأ(infd, buffer, اندازه(buffer));
		اگر (nread < 0)
			بازگشت -1;
		اگر (!nread)
			بشکن;

		انجامبده {
			ssize_t n = اكتب(outfd, buffer + nwritten, nread - nwritten);
			اگر (n < 0)
				بازگشت -1;
			nwritten += n;
		} وقتی(nwritten < nread);

		total += nwritten;
	}

	بازگشت total;
}

ایستا صحیح clone_دودویی(تهی)
{
	cleanup_close صحیح execfd = -1;
	ساختار stat statbuf = {};
	ssize_t sent = 0;
	صحیح fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	اگر (execfd >= 0) {
		/* Transfer ownership to caller */
		صحیح ret_execfd = execfd;
		execfd = -1;
		بازگشت ret_execfd;
	}
	execfd = try_bindfd();
	اگر (execfd >= 0) {
		/* Transfer ownership to caller */
		صحیح ret_execfd = execfd;
		execfd = -1;
		بازگشت ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	اگر (execfd < 0 || fdtype == EFD_NONE)
		بازگشت -ENOTRECOVERABLE;

	{
		cleanup_close صحیح binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		اگر (binfd < 0)
			برو error;

		اگر (fstat(binfd, &statbuf) < 0)
			برو error;

		وقتی (sent < statbuf.st_size) {
			صحیح n = sendfile(execfd, binfd, پوچ, statbuf.st_size - sent);
			اگر (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				اگر (n < 0)
					برو error;
			}
			sent += n;
		}
	}
	اگر (sent != statbuf.st_size)
		برو error;

	اگر (seal_execfd(&execfd, fdtype) < 0)
		برو error;

	{
		/* Transfer ownership to caller */
		صحیح ret_execfd = execfd;
		execfd = -1;
		بازگشت ret_execfd;
	}
error:
	بازگشت -EIO;
}

/* Get cheap access to the environment. */
بیرونی نویسه **environ;

صحیح ensure_cloned_دودویی(تهی)
{
	cleanup_close صحیح execfd = -1;
	نویسه **argv = پوچ;

	/* Check that we're not self-cloned, and if we are then bail. */
	صحیح cloned = is_self_cloned();
	اگر (cloned > 0 || cloned == -ENOTRECOVERABLE)
		بازگشت cloned;

	اگر (fetchve(&argv) < 0)
		بازگشت -EINVAL;

	execfd = clone_دودویی();
	اگر (execfd < 0)
		بازگشت -EIO;

	اگر (putenv(CLONED_دودویی_ENV "=1"))
		برو error;

	fexecve(execfd, argv, environ);
error:
	بازگشت -ENOEXEC;
}
