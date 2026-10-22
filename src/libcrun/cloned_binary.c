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
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <errno.h>

#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/statfs.h>
#அந்தர்பாவ <sys/vfs.h>
#அந்தர்பாவ <sys/mman.h>
#அந்தர்பாவ <sys/mount.h>
#அந்தர்பாவ <sys/sendfile.h>
#அந்தர்பாவ <sys/syscall.h>

#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "linux.h"

/* Use our own wrapper for memfd_create. */
#எனில் !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
முழு memfd_create(மாறிலி எழுத்து *name, unsigned முழு flags)
{
#ifdef SYS_memfd_create
	திரும்பு syscall(SYS_memfd_create, name, flags);
#இல்லையெனில்
	errno = ENOSYS;
	திரும்பு -1;
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

#define CLONED_இருமம்_ENV "_LIBCONTAINER_CLONED_BINARY"
#define CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#define CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
நிலையான முழு is_self_cloned(வெறுமை)
{
	முழு fd, ret, is_cloned = 0;
	கட்டமைப்பு stat statbuf = {};
	கட்டமைப்பு statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	எனில் (fd < 0)
		திரும்பு -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	எனில் (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		செல் out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	எனில் (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		செல் out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	எனில் (!getenv(CLONED_இருமம்_ENV)) {
		is_cloned = false;
		செல் out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	எனில் (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	close(fd);
	திரும்பு is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
நிலையான எழுத்து *read_file(எழுத்து *path, size_t *length)
{
	முழு fd;
	எழுத்து buf[4096], *copy = வெற்று;

	எனில் (!length)
		திரும்பு வெற்று;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	எனில் (fd < 0)
		திரும்பு வெற்று;

	*length = 0;
	க்கு (;;) {
		ssize_t n;

		n = பட(fd, buf, அளவு(buf));
		எனில் (n < 0)
			செல் error;
		எனில் (!n)
			நிறுத்து;

		copy = xrealloc(copy, (*length + n) * அளவு(*copy));
		memcpy(copy + *length, buf, n);
		*length += n;
	}
	close(fd);
	திரும்பு copy;

error:
	close(fd);
	விடுவி(copy);
	திரும்பு வெற்று;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
நிலையான முழு parse_xargs(எழுத்து *data, முழு data_length, எழுத்து ***output)
{
	முழு இலக்கம் = 0;
	எழுத்து *cur = data;

	எனில் (!data || *output != வெற்று)
		திரும்பு -1;

	போது (cur < data + data_length) {
		இலக்கம்++;
		*output = xrealloc(*output, (இலக்கம் + 1) * அளவு(**output));
		(*output)[இலக்கம் - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[இலக்கம்] = வெற்று;
	திரும்பு இலக்கம்;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
நிலையான முழு fetchve(எழுத்து ***argv)
{
	எழுத்து *cmdline = வெற்று;
	size_t cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	எனில் (!cmdline)
		செல் error;

	எனில் (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		செல் error;

	திரும்பு 0;

error:
	விடுவி(cmdline);
	திரும்பு -EINVAL;
}

எண்ணிக்கை {
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
#  எனில் defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

நிலையான முழு make_execfd(முழு *fdtype)
{
	முழு fd = -1;
	எழுத்து template[PATH_MAX] = {0};
	எழுத்து *prefix = getenv("_LIBCONTAINER_STATEDIR");

	எனில் (!prefix || *prefix != '/')
		prefix = "/tmp";
	எனில் (snprintf(template, அளவு(template), "%s/crun.XXXXXX", prefix) < 0)
		திரும்பு -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	எனில் (fd >= 0)
		திரும்பு fd;
	எனில் (errno != ENOSYS && errno != EINVAL)
		செல் error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	எனில் (fd >= 0) {
		கட்டமைப்பு stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		எனில் (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		எனில் (working_otmpfile)
			திரும்பு fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	எனில் (errno != EISDIR)
		செல் error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	எனில் (fd >= 0) {
		எனில் (unlink(template) >= 0)
			திரும்பு fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	திரும்பு -1;
}

நிலையான முழு seal_execfd(முழு *fd, முழு fdtype)
{
	தேர்வு (fdtype) {
	நிலை EFD_MEMFD:
		திரும்பு fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	நிலை EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		முழு newfd;
		எழுத்து fdpath[PATH_MAX] = {0};

		எனில் (fchmod(*fd, 0100) < 0)
			திரும்பு -1;

		எனில் (snprintf(fdpath, அளவு(fdpath), "/proc/self/fd/%d", *fd) < 0)
			திரும்பு -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		எனில் (newfd < 0)
			திரும்பு -1;

		close(*fd);
		*fd = newfd;
		திரும்பு 0;
	}
	இயல்பு:
	   நிறுத்து;
	}
	திரும்பு -1;
}

நிலையான முழு try_bindfd_mount_api(வெறுமை)
{
	libcrun_error_t err;
	முழு mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	எனில் (mountfd < 0) {
		crun_error_release (&err);
		திரும்பு -1;
	}
	திரும்பு mountfd;
}

நிலையான முழு try_bindfd(வெறுமை)
{
	mode_t mask;
	முழு fd, ret = -1;
	எழுத்து template[PATH_MAX] = {0};
	எழுத்து *prefix = getenv("_LIBCONTAINER_STATEDIR");

	எனில் (!prefix || *prefix != '/')
		prefix = "/tmp";
	எனில் (snprintf(template, அளவு(template), "%s/crun.XXXXXX", prefix) < 0)
		திரும்பு ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	எனில் (fd < 0)
		திரும்பு ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	எனில் (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		செல் out;
	எனில் (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		செல் out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(template, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	எனில் (umount2(template, MNT_DETACH) < 0) {
		எனில் (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(template);
	திரும்பு ret;
}

நிலையான ssize_t fd_to_fd(முழு outfd, முழு infd)
{
	ssize_t total = 0;
	எழுத்து buffer[4096];

	க்கு (;;) {
		ssize_t nread, nwritten = 0;

		nread = பட(infd, buffer, அளவு(buffer));
		எனில் (nread < 0)
			திரும்பு -1;
		எனில் (!nread)
			நிறுத்து;

		செய் {
			ssize_t n = லிக(outfd, buffer + nwritten, nread - nwritten);
			எனில் (n < 0)
				திரும்பு -1;
			nwritten += n;
		} போது(nwritten < nread);

		total += nwritten;
	}

	திரும்பு total;
}

நிலையான முழு clone_இருமம்(வெறுமை)
{
	cleanup_close முழு execfd = -1;
	கட்டமைப்பு stat statbuf = {};
	ssize_t sent = 0;
	முழு fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	எனில் (execfd >= 0) {
		/* Transfer ownership to caller */
		முழு ret_execfd = execfd;
		execfd = -1;
		திரும்பு ret_execfd;
	}
	execfd = try_bindfd();
	எனில் (execfd >= 0) {
		/* Transfer ownership to caller */
		முழு ret_execfd = execfd;
		execfd = -1;
		திரும்பு ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	எனில் (execfd < 0 || fdtype == EFD_NONE)
		திரும்பு -ENOTRECOVERABLE;

	{
		cleanup_close முழு binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		எனில் (binfd < 0)
			செல் error;

		எனில் (fstat(binfd, &statbuf) < 0)
			செல் error;

		போது (sent < statbuf.st_size) {
			முழு n = sendfile(execfd, binfd, வெற்று, statbuf.st_size - sent);
			எனில் (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				எனில் (n < 0)
					செல் error;
			}
			sent += n;
		}
	}
	எனில் (sent != statbuf.st_size)
		செல் error;

	எனில் (seal_execfd(&execfd, fdtype) < 0)
		செல் error;

	{
		/* Transfer ownership to caller */
		முழு ret_execfd = execfd;
		execfd = -1;
		திரும்பு ret_execfd;
	}
error:
	திரும்பு -EIO;
}

/* Get cheap access to the environment. */
வெளி எழுத்து **environ;

முழு ensure_cloned_இருமம்(வெறுமை)
{
	cleanup_close முழு execfd = -1;
	எழுத்து **argv = வெற்று;

	/* Check that we're not self-cloned, and if we are then bail. */
	முழு cloned = is_self_cloned();
	எனில் (cloned > 0 || cloned == -ENOTRECOVERABLE)
		திரும்பு cloned;

	எனில் (fetchve(&argv) < 0)
		திரும்பு -EINVAL;

	execfd = clone_இருமம்();
	எனில் (execfd < 0)
		திரும்பு -EIO;

	எனில் (putenv(CLONED_இருமம்_ENV "=1"))
		செல் error;

	fexecve(execfd, argv, environ);
error:
	திரும்பு -ENOEXEC;
}
