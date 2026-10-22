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
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/statfs.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <sys/mman.h>
#ಅನ್ತರ್ಭಾವ <sys/mount.h>
#ಅನ್ತರ್ಭಾವ <sys/sendfile.h>
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>

#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "linux.h"

/* Use our own wrapper for memfd_create. */
#ಯದಿ !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
ಪೂರ್ಣಾಙ್ಕ memfd_create(ಧ್ರುವ ವರ್ಣ *name, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
#ifdef SYS_memfd_create
	ಪ್ರತ್ಯಾಗಮ syscall(SYS_memfd_create, name, flags);
#ಅನ್ಯಥಾ
	errno = ENOSYS;
	ಪ್ರತ್ಯಾಗಮ -1;
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

#define CLONED_ದ್ವಿಮಾನ_ENV "_LIBCONTAINER_CLONED_BINARY"
#define CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#define CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_self_cloned(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ fd, ret, is_cloned = 0;
	ಸಂರಚನಾ stat statbuf = {};
	ಸಂರಚನಾ statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	ಯದಿ (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		ಗಚ್ಛ out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	ಯದಿ (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		ಗಚ್ಛ out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	ಯದಿ (!getenv(CLONED_ದ್ವಿಮಾನ_ENV)) {
		is_cloned = false;
		ಗಚ್ಛ out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	ಯದಿ (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	close(fd);
	ಪ್ರತ್ಯಾಗಮ is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
ಸ್ಥಿರ ವರ್ಣ *read_file(ವರ್ಣ *path, size_t *length)
{
	ಪೂರ್ಣಾಙ್ಕ fd;
	ವರ್ಣ buf[4096], *copy = ರಿಕ್ತ;

	ಯದಿ (!length)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	*length = 0;
	ಪ್ರತಿ (;;) {
		ssize_t n;

		n = ಪಠ(fd, buf, ಪರಿಮಾಣ(buf));
		ಯದಿ (n < 0)
			ಗಚ್ಛ error;
		ಯದಿ (!n)
			ಭಙ್ಗ;

		copy = xrealloc(copy, (*length + n) * ಪರಿಮಾಣ(*copy));
		memcpy(copy + *length, buf, n);
		*length += n;
	}
	close(fd);
	ಪ್ರತ್ಯಾಗಮ copy;

error:
	close(fd);
	ಮುಕ್ತ(copy);
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_xargs(ವರ್ಣ *data, ಪೂರ್ಣಾಙ್ಕ data_length, ವರ್ಣ ***output)
{
	ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ = 0;
	ವರ್ಣ *cur = data;

	ಯದಿ (!data || *output != ರಿಕ್ತ)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯಾವತ್ (cur < data + data_length) {
		ಅಂಕೆ++;
		*output = xrealloc(*output, (ಅಂಕೆ + 1) * ಪರಿಮಾಣ(**output));
		(*output)[ಅಂಕೆ - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[ಅಂಕೆ] = ರಿಕ್ತ;
	ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fetchve(ವರ್ಣ ***argv)
{
	ವರ್ಣ *cmdline = ರಿಕ್ತ;
	size_t cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	ಯದಿ (!cmdline)
		ಗಚ್ಛ error;

	ಯದಿ (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		ಗಚ್ಛ error;

	ಪ್ರತ್ಯಾಗಮ 0;

error:
	ಮುಕ್ತ(cmdline);
	ಪ್ರತ್ಯಾಗಮ -EINVAL;
}

ಗಣನಾ {
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
#  ಯದಿ defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ make_execfd(ಪೂರ್ಣಾಙ್ಕ *fdtype)
{
	ಪೂರ್ಣಾಙ್ಕ fd = -1;
	ವರ್ಣ template[PATH_MAX] = {0};
	ವರ್ಣ *prefix = getenv("_LIBCONTAINER_STATEDIR");

	ಯದಿ (!prefix || *prefix != '/')
		prefix = "/tmp";
	ಯದಿ (snprintf(template, ಪರಿಮಾಣ(template), "%s/crun.XXXXXX", prefix) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	ಯದಿ (fd >= 0)
		ಪ್ರತ್ಯಾಗಮ fd;
	ಯದಿ (errno != ENOSYS && errno != EINVAL)
		ಗಚ್ಛ error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	ಯದಿ (fd >= 0) {
		ಸಂರಚನಾ stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		ಯದಿ (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		ಯದಿ (working_otmpfile)
			ಪ್ರತ್ಯಾಗಮ fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	ಯದಿ (errno != EISDIR)
		ಗಚ್ಛ error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	ಯದಿ (fd >= 0) {
		ಯದಿ (unlink(template) >= 0)
			ಪ್ರತ್ಯಾಗಮ fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ seal_execfd(ಪೂರ್ಣಾಙ್ಕ *fd, ಪೂರ್ಣಾಙ್ಕ fdtype)
{
	ವಿಕಲ್ಪ (fdtype) {
	ಪಕ್ಷ EFD_MEMFD:
		ಪ್ರತ್ಯಾಗಮ fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	ಪಕ್ಷ EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		ಪೂರ್ಣಾಙ್ಕ newfd;
		ವರ್ಣ fdpath[PATH_MAX] = {0};

		ಯದಿ (fchmod(*fd, 0100) < 0)
			ಪ್ರತ್ಯಾಗಮ -1;

		ಯದಿ (snprintf(fdpath, ಪರಿಮಾಣ(fdpath), "/proc/self/fd/%d", *fd) < 0)
			ಪ್ರತ್ಯಾಗಮ -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		ಯದಿ (newfd < 0)
			ಪ್ರತ್ಯಾಗಮ -1;

		close(*fd);
		*fd = newfd;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಸ್ವಭಾವ:
	   ಭಙ್ಗ;
	}
	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ try_bindfd_mount_api(ಶೂನ್ಯ)
{
	libcrun_error_t err;
	ಪೂರ್ಣಾಙ್ಕ mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	ಯದಿ (mountfd < 0) {
		crun_error_release (&err);
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	ಪ್ರತ್ಯಾಗಮ mountfd;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ try_bindfd(ಶೂನ್ಯ)
{
	mode_t mask;
	ಪೂರ್ಣಾಙ್ಕ fd, ret = -1;
	ವರ್ಣ template[PATH_MAX] = {0};
	ವರ್ಣ *prefix = getenv("_LIBCONTAINER_STATEDIR");

	ಯದಿ (!prefix || *prefix != '/')
		prefix = "/tmp";
	ಯದಿ (snprintf(template, ಪರಿಮಾಣ(template), "%s/crun.XXXXXX", prefix) < 0)
		ಪ್ರತ್ಯಾಗಮ ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	ಯದಿ (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		ಗಚ್ಛ out;
	ಯದಿ (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		ಗಚ್ಛ out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(template, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	ಯದಿ (umount2(template, MNT_DETACH) < 0) {
		ಯದಿ (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(template);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ssize_t fd_to_fd(ಪೂರ್ಣಾಙ್ಕ outfd, ಪೂರ್ಣಾಙ್ಕ infd)
{
	ssize_t total = 0;
	ವರ್ಣ buffer[4096];

	ಪ್ರತಿ (;;) {
		ssize_t nread, nwritten = 0;

		nread = ಪಠ(infd, buffer, ಪರಿಮಾಣ(buffer));
		ಯದಿ (nread < 0)
			ಪ್ರತ್ಯಾಗಮ -1;
		ಯದಿ (!nread)
			ಭಙ್ಗ;

		ಕುರು {
			ssize_t n = ಲಿಖ(outfd, buffer + nwritten, nread - nwritten);
			ಯದಿ (n < 0)
				ಪ್ರತ್ಯಾಗಮ -1;
			nwritten += n;
		} ಯಾವತ್(nwritten < nread);

		total += nwritten;
	}

	ಪ್ರತ್ಯಾಗಮ total;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ clone_ದ್ವಿಮಾನ(ಶೂನ್ಯ)
{
	cleanup_close ಪೂರ್ಣಾಙ್ಕ execfd = -1;
	ಸಂರಚನಾ stat statbuf = {};
	ssize_t sent = 0;
	ಪೂರ್ಣಾಙ್ಕ fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	ಯದಿ (execfd >= 0) {
		/* Transfer ownership to caller */
		ಪೂರ್ಣಾಙ್ಕ ret_execfd = execfd;
		execfd = -1;
		ಪ್ರತ್ಯಾಗಮ ret_execfd;
	}
	execfd = try_bindfd();
	ಯದಿ (execfd >= 0) {
		/* Transfer ownership to caller */
		ಪೂರ್ಣಾಙ್ಕ ret_execfd = execfd;
		execfd = -1;
		ಪ್ರತ್ಯಾಗಮ ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	ಯದಿ (execfd < 0 || fdtype == EFD_NONE)
		ಪ್ರತ್ಯಾಗಮ -ENOTRECOVERABLE;

	{
		cleanup_close ಪೂರ್ಣಾಙ್ಕ binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		ಯದಿ (binfd < 0)
			ಗಚ್ಛ error;

		ಯದಿ (fstat(binfd, &statbuf) < 0)
			ಗಚ್ಛ error;

		ಯಾವತ್ (sent < statbuf.st_size) {
			ಪೂರ್ಣಾಙ್ಕ n = sendfile(execfd, binfd, ರಿಕ್ತ, statbuf.st_size - sent);
			ಯದಿ (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				ಯದಿ (n < 0)
					ಗಚ್ಛ error;
			}
			sent += n;
		}
	}
	ಯದಿ (sent != statbuf.st_size)
		ಗಚ್ಛ error;

	ಯದಿ (seal_execfd(&execfd, fdtype) < 0)
		ಗಚ್ಛ error;

	{
		/* Transfer ownership to caller */
		ಪೂರ್ಣಾಙ್ಕ ret_execfd = execfd;
		execfd = -1;
		ಪ್ರತ್ಯಾಗಮ ret_execfd;
	}
error:
	ಪ್ರತ್ಯಾಗಮ -EIO;
}

/* Get cheap access to the environment. */
ಬಾಹ್ಯ ವರ್ಣ **environ;

ಪೂರ್ಣಾಙ್ಕ ensure_cloned_ದ್ವಿಮಾನ(ಶೂನ್ಯ)
{
	cleanup_close ಪೂರ್ಣಾಙ್ಕ execfd = -1;
	ವರ್ಣ **argv = ರಿಕ್ತ;

	/* Check that we're not self-cloned, and if we are then bail. */
	ಪೂರ್ಣಾಙ್ಕ cloned = is_self_cloned();
	ಯದಿ (cloned > 0 || cloned == -ENOTRECOVERABLE)
		ಪ್ರತ್ಯಾಗಮ cloned;

	ಯದಿ (fetchve(&argv) < 0)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	execfd = clone_ದ್ವಿಮಾನ();
	ಯದಿ (execfd < 0)
		ಪ್ರತ್ಯಾಗಮ -EIO;

	ಯದಿ (putenv(CLONED_ದ್ವಿಮಾನ_ENV "=1"))
		ಗಚ್ಛ error;

	fexecve(execfd, argv, environ);
error:
	ಪ್ರತ್ಯಾಗಮ -ENOEXEC;
}
