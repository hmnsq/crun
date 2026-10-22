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
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <stdbool.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <limits.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <errno.h>

#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <sys/statfs.h>
#অন্তর্ভাৱ <sys/vfs.h>
#অন্তর্ভাৱ <sys/mman.h>
#অন্তর্ভাৱ <sys/mount.h>
#অন্তর্ভাৱ <sys/sendfile.h>
#অন্তর্ভাৱ <sys/syscall.h>

#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "linux.h"

/* Use our own wrapper for memfd_create. */
#যদি !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
পূর্ণাঙ্ক memfd_create(ধ্রুৱ ৱর্ণ *name, unsigned পূর্ণাঙ্ক flags)
{
#ifdef SYS_memfd_create
	ঘূৰাই syscall(SYS_memfd_create, name, flags);
#অন্যথা
	errno = ENOSYS;
	ঘূৰাই -1;
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

#define CLONED_দ্বিমিক_ENV "_LIBCONTAINER_CLONED_BINARY"
#define CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#define CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
স্থির পূর্ণাঙ্ক is_self_cloned(শূন্য)
{
	পূর্ণাঙ্ক fd, ret, is_cloned = 0;
	সংরচনা stat statbuf = {};
	সংরচনা statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	যদি (fd < 0)
		ঘূৰাই -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	যদি (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		গচ্ছ out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	যদি (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		গচ্ছ out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	যদি (!getenv(CLONED_দ্বিমিক_ENV)) {
		is_cloned = false;
		গচ্ছ out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	যদি (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	close(fd);
	ঘূৰাই is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
স্থির ৱর্ণ *read_file(ৱর্ণ *path, size_t *length)
{
	পূর্ণাঙ্ক fd;
	ৱর্ণ buf[4096], *copy = রিক্ত;

	যদি (!length)
		ঘূৰাই রিক্ত;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	যদি (fd < 0)
		ঘূৰাই রিক্ত;

	*length = 0;
	বাবে (;;) {
		ssize_t n;

		n = পঠ(fd, buf, পরিমাণ(buf));
		যদি (n < 0)
			গচ্ছ error;
		যদি (!n)
			ভঙ্গ;

		copy = xrealloc(copy, (*length + n) * পরিমাণ(*copy));
		memcpy(copy + *length, buf, n);
		*length += n;
	}
	close(fd);
	ঘূৰাই copy;

error:
	close(fd);
	মুক্ত(copy);
	ঘূৰাই রিক্ত;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
স্থির পূর্ণাঙ্ক parse_xargs(ৱর্ণ *data, পূর্ণাঙ্ক data_length, ৱর্ণ ***output)
{
	পূর্ণাঙ্ক অঙ্ক = 0;
	ৱর্ণ *cur = data;

	যদি (!data || *output != রিক্ত)
		ঘূৰাই -1;

	যেতিয়ালৈ (cur < data + data_length) {
		অঙ্ক++;
		*output = xrealloc(*output, (অঙ্ক + 1) * পরিমাণ(**output));
		(*output)[অঙ্ক - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[অঙ্ক] = রিক্ত;
	ঘূৰাই অঙ্ক;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
স্থির পূর্ণাঙ্ক fetchve(ৱর্ণ ***argv)
{
	ৱর্ণ *cmdline = রিক্ত;
	size_t cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	যদি (!cmdline)
		গচ্ছ error;

	যদি (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		গচ্ছ error;

	ঘূৰাই 0;

error:
	মুক্ত(cmdline);
	ঘূৰাই -EINVAL;
}

গণনা {
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
#  যদি defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

স্থির পূর্ণাঙ্ক make_execfd(পূর্ণাঙ্ক *fdtype)
{
	পূর্ণাঙ্ক fd = -1;
	ৱর্ণ template[PATH_MAX] = {0};
	ৱর্ণ *prefix = getenv("_LIBCONTAINER_STATEDIR");

	যদি (!prefix || *prefix != '/')
		prefix = "/tmp";
	যদি (snprintf(template, পরিমাণ(template), "%s/crun.XXXXXX", prefix) < 0)
		ঘূৰাই -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	যদি (fd >= 0)
		ঘূৰাই fd;
	যদি (errno != ENOSYS && errno != EINVAL)
		গচ্ছ error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	যদি (fd >= 0) {
		সংরচনা stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		যদি (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		যদি (working_otmpfile)
			ঘূৰাই fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	যদি (errno != EISDIR)
		গচ্ছ error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	যদি (fd >= 0) {
		যদি (unlink(template) >= 0)
			ঘূৰাই fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	ঘূৰাই -1;
}

স্থির পূর্ণাঙ্ক seal_execfd(পূর্ণাঙ্ক *fd, পূর্ণাঙ্ক fdtype)
{
	ৱিকল্প (fdtype) {
	পক্ষ EFD_MEMFD:
		ঘূৰাই fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	পক্ষ EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		পূর্ণাঙ্ক newfd;
		ৱর্ণ fdpath[PATH_MAX] = {0};

		যদি (fchmod(*fd, 0100) < 0)
			ঘূৰাই -1;

		যদি (snprintf(fdpath, পরিমাণ(fdpath), "/proc/self/fd/%d", *fd) < 0)
			ঘূৰাই -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		যদি (newfd < 0)
			ঘূৰাই -1;

		close(*fd);
		*fd = newfd;
		ঘূৰাই 0;
	}
	স্ৱভাৱ:
	   ভঙ্গ;
	}
	ঘূৰাই -1;
}

স্থির পূর্ণাঙ্ক try_bindfd_mount_api(শূন্য)
{
	libcrun_error_t err;
	পূর্ণাঙ্ক mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	যদি (mountfd < 0) {
		crun_error_release (&err);
		ঘূৰাই -1;
	}
	ঘূৰাই mountfd;
}

স্থির পূর্ণাঙ্ক try_bindfd(শূন্য)
{
	mode_t mask;
	পূর্ণাঙ্ক fd, ret = -1;
	ৱর্ণ template[PATH_MAX] = {0};
	ৱর্ণ *prefix = getenv("_LIBCONTAINER_STATEDIR");

	যদি (!prefix || *prefix != '/')
		prefix = "/tmp";
	যদি (snprintf(template, পরিমাণ(template), "%s/crun.XXXXXX", prefix) < 0)
		ঘূৰাই ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	যদি (fd < 0)
		ঘূৰাই ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	যদি (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		গচ্ছ out;
	যদি (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		গচ্ছ out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(template, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	যদি (umount2(template, MNT_DETACH) < 0) {
		যদি (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(template);
	ঘূৰাই ret;
}

স্থির ssize_t fd_to_fd(পূর্ণাঙ্ক outfd, পূর্ণাঙ্ক infd)
{
	ssize_t total = 0;
	ৱর্ণ buffer[4096];

	বাবে (;;) {
		ssize_t nread, nwritten = 0;

		nread = পঠ(infd, buffer, পরিমাণ(buffer));
		যদি (nread < 0)
			ঘূৰাই -1;
		যদি (!nread)
			ভঙ্গ;

		কৰা {
			ssize_t n = লিখ(outfd, buffer + nwritten, nread - nwritten);
			যদি (n < 0)
				ঘূৰাই -1;
			nwritten += n;
		} যেতিয়ালৈ(nwritten < nread);

		total += nwritten;
	}

	ঘূৰাই total;
}

স্থির পূর্ণাঙ্ক clone_দ্বিমিক(শূন্য)
{
	cleanup_close পূর্ণাঙ্ক execfd = -1;
	সংরচনা stat statbuf = {};
	ssize_t sent = 0;
	পূর্ণাঙ্ক fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	যদি (execfd >= 0) {
		/* Transfer ownership to caller */
		পূর্ণাঙ্ক ret_execfd = execfd;
		execfd = -1;
		ঘূৰাই ret_execfd;
	}
	execfd = try_bindfd();
	যদি (execfd >= 0) {
		/* Transfer ownership to caller */
		পূর্ণাঙ্ক ret_execfd = execfd;
		execfd = -1;
		ঘূৰাই ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	যদি (execfd < 0 || fdtype == EFD_NONE)
		ঘূৰাই -ENOTRECOVERABLE;

	{
		cleanup_close পূর্ণাঙ্ক binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		যদি (binfd < 0)
			গচ্ছ error;

		যদি (fstat(binfd, &statbuf) < 0)
			গচ্ছ error;

		যেতিয়ালৈ (sent < statbuf.st_size) {
			পূর্ণাঙ্ক n = sendfile(execfd, binfd, রিক্ত, statbuf.st_size - sent);
			যদি (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				যদি (n < 0)
					গচ্ছ error;
			}
			sent += n;
		}
	}
	যদি (sent != statbuf.st_size)
		গচ্ছ error;

	যদি (seal_execfd(&execfd, fdtype) < 0)
		গচ্ছ error;

	{
		/* Transfer ownership to caller */
		পূর্ণাঙ্ক ret_execfd = execfd;
		execfd = -1;
		ঘূৰাই ret_execfd;
	}
error:
	ঘূৰাই -EIO;
}

/* Get cheap access to the environment. */
বাহ্য ৱর্ণ **environ;

পূর্ণাঙ্ক ensure_cloned_দ্বিমিক(শূন্য)
{
	cleanup_close পূর্ণাঙ্ক execfd = -1;
	ৱর্ণ **argv = রিক্ত;

	/* Check that we're not self-cloned, and if we are then bail. */
	পূর্ণাঙ্ক cloned = is_self_cloned();
	যদি (cloned > 0 || cloned == -ENOTRECOVERABLE)
		ঘূৰাই cloned;

	যদি (fetchve(&argv) < 0)
		ঘূৰাই -EINVAL;

	execfd = clone_দ্বিমিক();
	যদি (execfd < 0)
		ঘূৰাই -EIO;

	যদি (putenv(CLONED_দ্বিমিক_ENV "=1"))
		গচ্ছ error;

	fexecve(execfd, argv, environ);
error:
	ঘূৰাই -ENOEXEC;
}
