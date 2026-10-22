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

#تعریف_کن GNU_منبع
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
#إذا !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#پایان_اگر
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#اگر_تعریف_نشده MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#پایان_اگر
صحيح memfd_create(ثابت حرف *name, بیعلامت صحيح flags)
{
#اگر_تعریف_شده SYS_memfd_create
	أرجع syscall(SYS_memfd_create, name, flags);
#وإلا
	errno = ENOSYS;
	أرجع -1;
#پایان_اگر
}


/* This comes directly from <linux/fcntl.h>. */
#اگر_تعریف_نشده F_LINUX_SPECIFIC_پایه
#  define F_LINUX_SPECIFIC_پایه 1024
#پایان_اگر
#اگر_تعریف_نشده F_جمع_کن_SEALS
#  define F_جمع_کن_SEALS (F_LINUX_SPECIFIC_پایه + 9)
#  define F_GET_SEALS (F_LINUX_SPECIFIC_پایه + 10)
#پایان_اگر
#اگر_تعریف_نشده F_SEAL_SEAL
#  define F_SEAL_SEAL   0x0001	/* prevent further seals from being set */
#  define F_SEAL_SHRINK 0x0002	/* prevent file from shrinking */
#  define F_SEAL_GROW   0x0004	/* prevent file from growing */
#  define F_SEAL_WRITE  0x0008	/* prevent writes */
#پایان_اگر

#تعریف_کن CLONED_دودویی_ENV "_LIBCONTAINER_CLONED_BINARY"
#تعریف_کن CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#تعریف_کن CRUN_MEMFD_SEALS \
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
	إذا (fd < 0)
		أرجع -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	إذا (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		اذهب بیرون;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	إذا (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		اذهب بیرون;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	إذا (!getenv(CLONED_دودویی_ENV)) {
		is_cloned = false;
		اذهب بیرون;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	إذا (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

بیرون:
	close(fd);
	أرجع is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
ساكن حرف *read_file(حرف *مسیر, size_t *درازی)
{
	صحيح fd;
	حرف buf[4096], *copy = عدم;

	إذا (!درازی)
		أرجع عدم;

	fd = open(مسیر, O_RDONLY | O_CLOEXEC);
	إذا (fd < 0)
		أرجع عدم;

	*درازی = 0;
	لكل (;;) {
		ssize_t n;

		n = اقرأ(fd, buf, حجم(buf));
		إذا (n < 0)
			اذهب error;
		إذا (!n)
			اكسر;

		copy = xrealloc(copy, (*درازی + n) * حجم(*copy));
		کپی_حافظه(copy + *درازی, buf, n);
		*درازی += n;
	}
	close(fd);
	أرجع copy;

error:
	close(fd);
	حرر(copy);
	أرجع عدم;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
ساكن صحيح parse_xargs(حرف *داده, صحيح داده_درازی, حرف ***output)
{
	صحيح رقم = 0;
	حرف *cur = داده;

	إذا (!داده || *output != عدم)
		أرجع -1;

	طالما (cur < داده + داده_درازی) {
		رقم++;
		*output = xrealloc(*output, (رقم + 1) * حجم(**output));
		(*output)[رقم - 1] = cur;
		cur += طول_رشته(cur) + 1;
	}
	(*output)[رقم] = عدم;
	أرجع رقم;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
ساكن صحيح fetchve(حرف ***argv)
{
	حرف *cmdline = عدم;
	size_t cmdline_اندازه;

	cmdline = read_file("/proc/self/cmdline", &cmdline_اندازه);
	إذا (!cmdline)
		اذهب error;

	إذا (parse_xargs(cmdline, cmdline_اندازه, argv) <= 0)
		اذهب error;

	أرجع 0;

error:
	حرر(cmdline);
	أرجع -EINVAL;
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
#اگر_تعریف_نشده O_TMPFILE
#  إذا defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#پایان_اگر

ساكن صحيح make_execfd(صحيح *fdtype)
{
	صحيح fd = -1;
	حرف قالب[PATH_MAX] = {0};
	حرف *پیشوند = getenv("_LIBCONTAINER_STATEDIR");

	إذا (!پیشوند || *پیشوند != '/')
		پیشوند = "/tmp";
	إذا (snprintf(قالب, حجم(قالب), "%s/crun.XXXXXX", پیشوند) < 0)
		أرجع -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	إذا (fd >= 0)
		أرجع fd;
	إذا (errno != ENOSYS && errno != EINVAL)
		اذهب error;

#اگر_تعریف_شده O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(پیشوند, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	إذا (fd >= 0) {
		بنية stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		إذا (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		إذا (working_otmpfile)
			أرجع fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	إذا (errno != EISDIR)
		اذهب error;
#پایان_اگر /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(قالب, O_CLOEXEC);
	إذا (fd >= 0) {
		إذا (unlink(قالب) >= 0)
			أرجع fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	أرجع -1;
}

ساكن صحيح seal_execfd(صحيح *fd, صحيح fdtype)
{
	بدل (fdtype) {
	حالة EFD_MEMFD:
		أرجع fcntl(*fd, F_جمع_کن_SEALS, CRUN_MEMFD_SEALS);
	حالة EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		صحيح newfd;
		حرف fdpath[PATH_MAX] = {0};

		إذا (fchmod(*fd, 0100) < 0)
			أرجع -1;

		إذا (snprintf(fdpath, حجم(fdpath), "/proc/self/fd/%d", *fd) < 0)
			أرجع -1;

		newfd = open(fdpath, O_مسیر | O_CLOEXEC);
		إذا (newfd < 0)
			أرجع -1;

		close(*fd);
		*fd = newfd;
		أرجع 0;
	}
	افتراضي:
	   اكسر;
	}
	أرجع -1;
}

ساكن صحيح try_bindfd_mount_api(فراغ)
{
	libcrun_error_t err;
	صحيح mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	إذا (mountfd < 0) {
		crun_error_release (&err);
		أرجع -1;
	}
	أرجع mountfd;
}

ساكن صحيح try_bindfd(فراغ)
{
	mode_t ماسک;
	صحيح fd, ret = -1;
	حرف قالب[PATH_MAX] = {0};
	حرف *پیشوند = getenv("_LIBCONTAINER_STATEDIR");

	إذا (!پیشوند || *پیشوند != '/')
		پیشوند = "/tmp";
	إذا (snprintf(قالب, حجم(قالب), "%s/crun.XXXXXX", پیشوند) < 0)
		أرجع ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	ماسک = umask(0700);
	fd = mkstemp(قالب);
	umask(ماسک);
	إذا (fd < 0)
		أرجع ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	إذا (mount("/proc/self/exe", قالب, "", MS_BIND, "") < 0)
		اذهب بیرون;
	إذا (mount("", قالب, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		اذهب بیرون_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(قالب, O_مسیر | O_CLOEXEC);

بیرون_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	إذا (umount2(قالب, MNT_DETACH) < 0) {
		إذا (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

بیرون:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(قالب);
	أرجع ret;
}

ساكن ssize_t fd_مقصد_fd(صحيح outfd, صحيح infd)
{
	ssize_t مجموع = 0;
	حرف میانگیر[4096];

	لكل (;;) {
		ssize_t nread, nwritten = 0;

		nread = اقرأ(infd, میانگیر, حجم(میانگیر));
		إذا (nread < 0)
			أرجع -1;
		إذا (!nread)
			اكسر;

		افعل {
			ssize_t n = اكتب(outfd, میانگیر + nwritten, nread - nwritten);
			إذا (n < 0)
				أرجع -1;
			nwritten += n;
		} طالما(nwritten < nread);

		مجموع += nwritten;
	}

	أرجع مجموع;
}

ساكن صحيح clone_دودویی(فراغ)
{
	cleanup_close صحيح execfd = -1;
	بنية stat statbuf = {};
	ssize_t فرستاده = 0;
	صحيح fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	إذا (execfd >= 0) {
		/* Transfer ownership to caller */
		صحيح ret_execfd = execfd;
		execfd = -1;
		أرجع ret_execfd;
	}
	execfd = try_bindfd();
	إذا (execfd >= 0) {
		/* Transfer ownership to caller */
		صحيح ret_execfd = execfd;
		execfd = -1;
		أرجع ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	إذا (execfd < 0 || fdtype == EFD_NONE)
		أرجع -ENOTRECOVERABLE;

	{
		cleanup_close صحيح binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		إذا (binfd < 0)
			اذهب error;

		إذا (fstat(binfd, &statbuf) < 0)
			اذهب error;

		طالما (فرستاده < statbuf.st_size) {
			صحيح n = sendfile(execfd, binfd, عدم, statbuf.st_size - فرستاده);
			إذا (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_مقصد_fd(execfd, binfd);
				إذا (n < 0)
					اذهب error;
			}
			فرستاده += n;
		}
	}
	إذا (فرستاده != statbuf.st_size)
		اذهب error;

	إذا (seal_execfd(&execfd, fdtype) < 0)
		اذهب error;

	{
		/* Transfer ownership to caller */
		صحيح ret_execfd = execfd;
		execfd = -1;
		أرجع ret_execfd;
	}
error:
	أرجع -EIO;
}

/* Get cheap access to the environment. */
خارجي حرف **environ;

صحيح ensure_cloned_دودویی(فراغ)
{
	cleanup_close صحيح execfd = -1;
	حرف **argv = عدم;

	/* Check that we're not self-cloned, and if we are then bail. */
	صحيح cloned = is_self_cloned();
	إذا (cloned > 0 || cloned == -ENOTRECOVERABLE)
		أرجع cloned;

	إذا (fetchve(&argv) < 0)
		أرجع -EINVAL;

	execfd = clone_دودویی();
	إذا (execfd < 0)
		أرجع -EIO;

	إذا (putenv(CLONED_دودویی_ENV "=1"))
		اذهب error;

	fexecve(execfd, argv, environ);
error:
	أرجع -ENOEXEC;
}
