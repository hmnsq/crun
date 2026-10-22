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
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>

#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/statfs.h>
#ଅନ୍ତର୍ଭାଵ <sys/vfs.h>
#ଅନ୍ତର୍ଭାଵ <sys/mman.h>
#ଅନ୍ତର୍ଭାଵ <sys/mount.h>
#ଅନ୍ତର୍ଭାଵ <sys/sendfile.h>
#ଅନ୍ତର୍ଭାଵ <sys/syscall.h>

#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "linux.h"

/* Use our own wrapper for memfd_create. */
#ଯଦି !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
ପୂର୍ଣ୍ଣସଂଖ୍ୟା memfd_create(ଅଚଳ ଅକ୍ଷର *name, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
#ifdef SYS_memfd_create
	ଫେରନ୍ତୁ syscall(SYS_memfd_create, name, flags);
#ଅନ୍ୟଥା
	errno = ENOSYS;
	ଫେରନ୍ତୁ -1;
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

#define CLONED_ଦ୍ୱିମିକ_ENV "_LIBCONTAINER_CLONED_BINARY"
#define CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#define CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_self_cloned(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ret, is_cloned = 0;
	ସଂରଚନା stat statbuf = {};
	ସଂରଚନା statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	ଯଦି (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		ଯାଅ out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	ଯଦି (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		ଯାଅ out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	ଯଦି (!getenv(CLONED_ଦ୍ୱିମିକ_ENV)) {
		is_cloned = false;
		ଯାଅ out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	ଯଦି (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	close(fd);
	ଫେରନ୍ତୁ is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
ସ୍ଥିର ଅକ୍ଷର *read_file(ଅକ୍ଷର *path, size_t *length)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	ଅକ୍ଷର buf[4096], *copy = ଖାଲି;

	ଯଦି (!length)
		ଫେରନ୍ତୁ ଖାଲି;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ ଖାଲି;

	*length = 0;
	ପାଇଁ (;;) {
		ssize_t n;

		n = ପଠ(fd, buf, ଆକାର(buf));
		ଯଦି (n < 0)
			ଯାଅ error;
		ଯଦି (!n)
			ଭାଙ୍ଗ;

		copy = xrealloc(copy, (*length + n) * ଆକାର(*copy));
		memcpy(copy + *length, buf, n);
		*length += n;
	}
	close(fd);
	ଫେରନ୍ତୁ copy;

error:
	close(fd);
	ମୁକ୍ତ(copy);
	ଫେରନ୍ତୁ ଖାଲି;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_xargs(ଅକ୍ଷର *data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା data_length, ଅକ୍ଷର ***output)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ = 0;
	ଅକ୍ଷର *cur = data;

	ଯଦି (!data || *output != ଖାଲି)
		ଫେରନ୍ତୁ -1;

	ଯେତେବେଳେ (cur < data + data_length) {
		ଅଙ୍କ++;
		*output = xrealloc(*output, (ଅଙ୍କ + 1) * ଆକାର(**output));
		(*output)[ଅଙ୍କ - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[ଅଙ୍କ] = ଖାଲି;
	ଫେରନ୍ତୁ ଅଙ୍କ;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetchve(ଅକ୍ଷର ***argv)
{
	ଅକ୍ଷର *cmdline = ଖାଲି;
	size_t cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	ଯଦି (!cmdline)
		ଯାଅ error;

	ଯଦି (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		ଯାଅ error;

	ଫେରନ୍ତୁ 0;

error:
	ମୁକ୍ତ(cmdline);
	ଫେରନ୍ତୁ -EINVAL;
}

ଗଣନା {
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
#  ଯଦି defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା make_execfd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *fdtype)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
	ଅକ୍ଷର template[PATH_MAX] = {0};
	ଅକ୍ଷର *prefix = getenv("_LIBCONTAINER_STATEDIR");

	ଯଦି (!prefix || *prefix != '/')
		prefix = "/tmp";
	ଯଦି (snprintf(template, ଆକାର(template), "%s/crun.XXXXXX", prefix) < 0)
		ଫେରନ୍ତୁ -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	ଯଦି (fd >= 0)
		ଫେରନ୍ତୁ fd;
	ଯଦି (errno != ENOSYS && errno != EINVAL)
		ଯାଅ error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	ଯଦି (fd >= 0) {
		ସଂରଚନା stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		ଯଦି (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		ଯଦି (working_otmpfile)
			ଫେରନ୍ତୁ fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	ଯଦି (errno != EISDIR)
		ଯାଅ error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	ଯଦି (fd >= 0) {
		ଯଦି (unlink(template) >= 0)
			ଫେରନ୍ତୁ fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା seal_execfd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdtype)
{
	ବାଛ (fdtype) {
	ସ୍ଥିତି EFD_MEMFD:
		ଫେରନ୍ତୁ fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	ସ୍ଥିତି EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା newfd;
		ଅକ୍ଷର fdpath[PATH_MAX] = {0};

		ଯଦି (fchmod(*fd, 0100) < 0)
			ଫେରନ୍ତୁ -1;

		ଯଦି (snprintf(fdpath, ଆକାର(fdpath), "/proc/self/fd/%d", *fd) < 0)
			ଫେରନ୍ତୁ -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		ଯଦି (newfd < 0)
			ଫେରନ୍ତୁ -1;

		close(*fd);
		*fd = newfd;
		ଫେରନ୍ତୁ 0;
	}
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	   ଭାଙ୍ଗ;
	}
	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_bindfd_mount_api(ଶୂନ୍ୟ)
{
	libcrun_error_t err;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	ଯଦି (mountfd < 0) {
		crun_error_release (&err);
		ଫେରନ୍ତୁ -1;
	}
	ଫେରନ୍ତୁ mountfd;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_bindfd(ଶୂନ୍ୟ)
{
	mode_t mask;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ret = -1;
	ଅକ୍ଷର template[PATH_MAX] = {0};
	ଅକ୍ଷର *prefix = getenv("_LIBCONTAINER_STATEDIR");

	ଯଦି (!prefix || *prefix != '/')
		prefix = "/tmp";
	ଯଦି (snprintf(template, ଆକାର(template), "%s/crun.XXXXXX", prefix) < 0)
		ଫେରନ୍ତୁ ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	ଯଦି (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		ଯାଅ out;
	ଯଦି (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		ଯାଅ out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(template, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	ଯଦି (umount2(template, MNT_DETACH) < 0) {
		ଯଦି (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(template);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ssize_t fd_to_fd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା outfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା infd)
{
	ssize_t total = 0;
	ଅକ୍ଷର buffer[4096];

	ପାଇଁ (;;) {
		ssize_t nread, nwritten = 0;

		nread = ପଠ(infd, buffer, ଆକାର(buffer));
		ଯଦି (nread < 0)
			ଫେରନ୍ତୁ -1;
		ଯଦି (!nread)
			ଭାଙ୍ଗ;

		କର {
			ssize_t n = ଲିଖ(outfd, buffer + nwritten, nread - nwritten);
			ଯଦି (n < 0)
				ଫେରନ୍ତୁ -1;
			nwritten += n;
		} ଯେତେବେଳେ(nwritten < nread);

		total += nwritten;
	}

	ଫେରନ୍ତୁ total;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା clone_ଦ୍ୱିମିକ(ଶୂନ୍ୟ)
{
	cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା execfd = -1;
	ସଂରଚନା stat statbuf = {};
	ssize_t sent = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	ଯଦି (execfd >= 0) {
		/* Transfer ownership to caller */
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret_execfd = execfd;
		execfd = -1;
		ଫେରନ୍ତୁ ret_execfd;
	}
	execfd = try_bindfd();
	ଯଦି (execfd >= 0) {
		/* Transfer ownership to caller */
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret_execfd = execfd;
		execfd = -1;
		ଫେରନ୍ତୁ ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	ଯଦି (execfd < 0 || fdtype == EFD_NONE)
		ଫେରନ୍ତୁ -ENOTRECOVERABLE;

	{
		cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		ଯଦି (binfd < 0)
			ଯାଅ error;

		ଯଦି (fstat(binfd, &statbuf) < 0)
			ଯାଅ error;

		ଯେତେବେଳେ (sent < statbuf.st_size) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = sendfile(execfd, binfd, ଖାଲି, statbuf.st_size - sent);
			ଯଦି (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				ଯଦି (n < 0)
					ଯାଅ error;
			}
			sent += n;
		}
	}
	ଯଦି (sent != statbuf.st_size)
		ଯାଅ error;

	ଯଦି (seal_execfd(&execfd, fdtype) < 0)
		ଯାଅ error;

	{
		/* Transfer ownership to caller */
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret_execfd = execfd;
		execfd = -1;
		ଫେରନ୍ତୁ ret_execfd;
	}
error:
	ଫେରନ୍ତୁ -EIO;
}

/* Get cheap access to the environment. */
ବାହ୍ୟ ଅକ୍ଷର **environ;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ensure_cloned_ଦ୍ୱିମିକ(ଶୂନ୍ୟ)
{
	cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା execfd = -1;
	ଅକ୍ଷର **argv = ଖାଲି;

	/* Check that we're not self-cloned, and if we are then bail. */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cloned = is_self_cloned();
	ଯଦି (cloned > 0 || cloned == -ENOTRECOVERABLE)
		ଫେରନ୍ତୁ cloned;

	ଯଦି (fetchve(&argv) < 0)
		ଫେରନ୍ତୁ -EINVAL;

	execfd = clone_ଦ୍ୱିମିକ();
	ଯଦି (execfd < 0)
		ଫେରନ୍ତୁ -EIO;

	ଯଦି (putenv(CLONED_ଦ୍ୱିମିକ_ENV "=1"))
		ଯାଅ error;

	fexecve(execfd, argv, environ);
error:
	ଫେରନ୍ତୁ -ENOEXEC;
}
