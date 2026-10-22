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
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <errno.h>

#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/statfs.h>
#অন্তর্ভাব <sys/vfs.h>
#অন্তর্ভাব <sys/mman.h>
#অন্তর্ভাব <sys/mount.h>
#অন্তর্ভাব <sys/sendfile.h>
#অন্তর্ভাব <sys/syscall.h>

#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "linux.h"

/* Use our own wrapper for memfd_create. */
#যদি !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
পূর্ণসংখ্যা memfd_create(ধ্রুবক অক্ষর *name, unsigned পূর্ণসংখ্যা flags)
{
#ifdef SYS_memfd_create
	ফেরত syscall(SYS_memfd_create, name, flags);
#অন্যথা
	errno = ENOSYS;
	ফেরত -1;
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
স্থির পূর্ণসংখ্যা is_self_cloned(শূন্য)
{
	পূর্ণসংখ্যা fd, ret, is_cloned = 0;
	কাঠামো stat statbuf = {};
	কাঠামো statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	যদি (fd < 0)
		ফেরত -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	যদি (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		যাও out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	যদি (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		যাও out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	যদি (!getenv(CLONED_দ্বিমিক_ENV)) {
		is_cloned = false;
		যাও out;
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
	ফেরত is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
স্থির অক্ষর *read_file(অক্ষর *path, size_t *দৈর্ঘ্য)
{
	পূর্ণসংখ্যা fd;
	অক্ষর buf[4096], *copy = ফাঁকা;

	যদি (!দৈর্ঘ্য)
		ফেরত ফাঁকা;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	যদি (fd < 0)
		ফেরত ফাঁকা;

	*দৈর্ঘ্য = 0;
	জন্য (;;) {
		ssize_t n;

		n = পঠ(fd, buf, আকার(buf));
		যদি (n < 0)
			যাও error;
		যদি (!n)
			বিরতি;

		copy = xrealloc(copy, (*দৈর্ঘ্য + n) * আকার(*copy));
		memcpy(copy + *দৈর্ঘ্য, buf, n);
		*দৈর্ঘ্য += n;
	}
	close(fd);
	ফেরত copy;

error:
	close(fd);
	মুক্ত(copy);
	ফেরত ফাঁকা;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
স্থির পূর্ণসংখ্যা parse_xargs(অক্ষর *তথ্য, পূর্ণসংখ্যা তথ্য_দৈর্ঘ্য, অক্ষর ***output)
{
	পূর্ণসংখ্যা অঙ্ক = 0;
	অক্ষর *cur = তথ্য;

	যদি (!তথ্য || *output != ফাঁকা)
		ফেরত -1;

	যখন (cur < তথ্য + তথ্য_দৈর্ঘ্য) {
		অঙ্ক++;
		*output = xrealloc(*output, (অঙ্ক + 1) * আকার(**output));
		(*output)[অঙ্ক - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[অঙ্ক] = ফাঁকা;
	ফেরত অঙ্ক;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
স্থির পূর্ণসংখ্যা fetchve(অক্ষর ***argv)
{
	অক্ষর *cmdline = ফাঁকা;
	size_t cmdline_আকার;

	cmdline = read_file("/proc/self/cmdline", &cmdline_আকার);
	যদি (!cmdline)
		যাও error;

	যদি (parse_xargs(cmdline, cmdline_আকার, argv) <= 0)
		যাও error;

	ফেরত 0;

error:
	মুক্ত(cmdline);
	ফেরত -EINVAL;
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

স্থির পূর্ণসংখ্যা make_execfd(পূর্ণসংখ্যা *fdtype)
{
	পূর্ণসংখ্যা fd = -1;
	অক্ষর template[PATH_MAX] = {0};
	অক্ষর *prefix = getenv("_LIBCONTAINER_STATEDIR");

	যদি (!prefix || *prefix != '/')
		prefix = "/tmp";
	যদি (snprintf(template, আকার(template), "%s/crun.XXXXXX", prefix) < 0)
		ফেরত -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	যদি (fd >= 0)
		ফেরত fd;
	যদি (errno != ENOSYS && errno != EINVAL)
		যাও error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	যদি (fd >= 0) {
		কাঠামো stat statbuf = {};
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
			ফেরত fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	যদি (errno != EISDIR)
		যাও error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	যদি (fd >= 0) {
		যদি (unlink(template) >= 0)
			ফেরত fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	ফেরত -1;
}

স্থির পূর্ণসংখ্যা seal_execfd(পূর্ণসংখ্যা *fd, পূর্ণসংখ্যা fdtype)
{
	নির্বাচন (fdtype) {
	ক্ষেত্র EFD_MEMFD:
		ফেরত fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	ক্ষেত্র EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		পূর্ণসংখ্যা newfd;
		অক্ষর fdpath[PATH_MAX] = {0};

		যদি (fchmod(*fd, 0100) < 0)
			ফেরত -1;

		যদি (snprintf(fdpath, আকার(fdpath), "/proc/self/fd/%d", *fd) < 0)
			ফেরত -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		যদি (newfd < 0)
			ফেরত -1;

		close(*fd);
		*fd = newfd;
		ফেরত 0;
	}
	পূর্বনির্ধারিত:
	   বিরতি;
	}
	ফেরত -1;
}

স্থির পূর্ণসংখ্যা try_bindfd_mount_api(শূন্য)
{
	libcrun_error_t err;
	পূর্ণসংখ্যা mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	যদি (mountfd < 0) {
		crun_error_release (&err);
		ফেরত -1;
	}
	ফেরত mountfd;
}

স্থির পূর্ণসংখ্যা try_bindfd(শূন্য)
{
	mode_t mask;
	পূর্ণসংখ্যা fd, ret = -1;
	অক্ষর template[PATH_MAX] = {0};
	অক্ষর *prefix = getenv("_LIBCONTAINER_STATEDIR");

	যদি (!prefix || *prefix != '/')
		prefix = "/tmp";
	যদি (snprintf(template, আকার(template), "%s/crun.XXXXXX", prefix) < 0)
		ফেরত ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	যদি (fd < 0)
		ফেরত ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	যদি (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		যাও out;
	যদি (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		যাও out_umount;


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
	ফেরত ret;
}

স্থির ssize_t fd_to_fd(পূর্ণসংখ্যা outfd, পূর্ণসংখ্যা infd)
{
	ssize_t যোগ = 0;
	অক্ষর বাফার[4096];

	জন্য (;;) {
		ssize_t nread, nwritten = 0;

		nread = পঠ(infd, বাফার, আকার(বাফার));
		যদি (nread < 0)
			ফেরত -1;
		যদি (!nread)
			বিরতি;

		করো {
			ssize_t n = লিখ(outfd, বাফার + nwritten, nread - nwritten);
			যদি (n < 0)
				ফেরত -1;
			nwritten += n;
		} যখন(nwritten < nread);

		যোগ += nwritten;
	}

	ফেরত যোগ;
}

স্থির পূর্ণসংখ্যা clone_দ্বিমিক(শূন্য)
{
	cleanup_close পূর্ণসংখ্যা execfd = -1;
	কাঠামো stat statbuf = {};
	ssize_t sent = 0;
	পূর্ণসংখ্যা fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	যদি (execfd >= 0) {
		/* Transfer ownership to caller */
		পূর্ণসংখ্যা ret_execfd = execfd;
		execfd = -1;
		ফেরত ret_execfd;
	}
	execfd = try_bindfd();
	যদি (execfd >= 0) {
		/* Transfer ownership to caller */
		পূর্ণসংখ্যা ret_execfd = execfd;
		execfd = -1;
		ফেরত ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	যদি (execfd < 0 || fdtype == EFD_NONE)
		ফেরত -ENOTRECOVERABLE;

	{
		cleanup_close পূর্ণসংখ্যা binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		যদি (binfd < 0)
			যাও error;

		যদি (fstat(binfd, &statbuf) < 0)
			যাও error;

		যখন (sent < statbuf.st_size) {
			পূর্ণসংখ্যা n = sendfile(execfd, binfd, ফাঁকা, statbuf.st_size - sent);
			যদি (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				যদি (n < 0)
					যাও error;
			}
			sent += n;
		}
	}
	যদি (sent != statbuf.st_size)
		যাও error;

	যদি (seal_execfd(&execfd, fdtype) < 0)
		যাও error;

	{
		/* Transfer ownership to caller */
		পূর্ণসংখ্যা ret_execfd = execfd;
		execfd = -1;
		ফেরত ret_execfd;
	}
error:
	ফেরত -EIO;
}

/* Get cheap access to the environment. */
বাহ্যিক অক্ষর **environ;

পূর্ণসংখ্যা ensure_cloned_দ্বিমিক(শূন্য)
{
	cleanup_close পূর্ণসংখ্যা execfd = -1;
	অক্ষর **argv = ফাঁকা;

	/* Check that we're not self-cloned, and if we are then bail. */
	পূর্ণসংখ্যা cloned = is_self_cloned();
	যদি (cloned > 0 || cloned == -ENOTRECOVERABLE)
		ফেরত cloned;

	যদি (fetchve(&argv) < 0)
		ফেরত -EINVAL;

	execfd = clone_দ্বিমিক();
	যদি (execfd < 0)
		ফেরত -EIO;

	যদি (putenv(CLONED_দ্বিমিক_ENV "=1"))
		যাও error;

	fexecve(execfd, argv, environ);
error:
	ফেরত -ENOEXEC;
}
