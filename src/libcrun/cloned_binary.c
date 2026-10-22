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
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <errno.h>

#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/statfs.h>
#അന്തര്ഭാവ <sys/vfs.h>
#അന്തര്ഭാവ <sys/mman.h>
#അന്തര്ഭാവ <sys/mount.h>
#അന്തര്ഭാവ <sys/sendfile.h>
#അന്തര്ഭാവ <sys/syscall.h>

#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "linux.h"

/* Use our own wrapper for memfd_create. */
#എങ്കിൽ !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
പൂർണ്ണസംഖ്യ memfd_create(സ്ഥിരം അക്ഷരം *name, unsigned പൂർണ്ണസംഖ്യ flags)
{
#ifdef SYS_memfd_create
	തിരികെ syscall(SYS_memfd_create, name, flags);
#അല്ലെങ്കിൽ
	errno = ENOSYS;
	തിരികെ -1;
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

#define CLONED_ദ്വിമാന_ENV "_LIBCONTAINER_CLONED_BINARY"
#define CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#define CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ is_self_cloned(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ fd, ret, is_cloned = 0;
	ഘടന stat statbuf = {};
	ഘടന statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	എങ്കിൽ (fd < 0)
		തിരികെ -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	എങ്കിൽ (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		പോകുക out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	എങ്കിൽ (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		പോകുക out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	എങ്കിൽ (!getenv(CLONED_ദ്വിമാന_ENV)) {
		is_cloned = false;
		പോകുക out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	എങ്കിൽ (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	close(fd);
	തിരികെ is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
സ്ഥിതം അക്ഷരം *read_file(അക്ഷരം *path, size_t *length)
{
	പൂർണ്ണസംഖ്യ fd;
	അക്ഷരം buf[4096], *copy = ശൂന്യ;

	എങ്കിൽ (!length)
		തിരികെ ശൂന്യ;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	എങ്കിൽ (fd < 0)
		തിരികെ ശൂന്യ;

	*length = 0;
	വേണ്ടി (;;) {
		ssize_t n;

		n = പഠ(fd, buf, വലുപ്പം(buf));
		എങ്കിൽ (n < 0)
			പോകുക error;
		എങ്കിൽ (!n)
			നിർത്തുക;

		copy = xrealloc(copy, (*length + n) * വലുപ്പം(*copy));
		memcpy(copy + *length, buf, n);
		*length += n;
	}
	close(fd);
	തിരികെ copy;

error:
	close(fd);
	സ്വതന്ത്രം(copy);
	തിരികെ ശൂന്യ;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ parse_xargs(അക്ഷരം *data, പൂർണ്ണസംഖ്യ data_length, അക്ഷരം ***output)
{
	പൂർണ്ണസംഖ്യ അക്കം = 0;
	അക്ഷരം *cur = data;

	എങ്കിൽ (!data || *output != ശൂന്യ)
		തിരികെ -1;

	ആകുമ്പോൾ (cur < data + data_length) {
		അക്കം++;
		*output = xrealloc(*output, (അക്കം + 1) * വലുപ്പം(**output));
		(*output)[അക്കം - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[അക്കം] = ശൂന്യ;
	തിരികെ അക്കം;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ fetchve(അക്ഷരം ***argv)
{
	അക്ഷരം *cmdline = ശൂന്യ;
	size_t cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	എങ്കിൽ (!cmdline)
		പോകുക error;

	എങ്കിൽ (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		പോകുക error;

	തിരികെ 0;

error:
	സ്വതന്ത്രം(cmdline);
	തിരികെ -EINVAL;
}

എണ്ണൽ {
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
#  എങ്കിൽ defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

സ്ഥിതം പൂർണ്ണസംഖ്യ make_execfd(പൂർണ്ണസംഖ്യ *fdtype)
{
	പൂർണ്ണസംഖ്യ fd = -1;
	അക്ഷരം template[PATH_MAX] = {0};
	അക്ഷരം *prefix = getenv("_LIBCONTAINER_STATEDIR");

	എങ്കിൽ (!prefix || *prefix != '/')
		prefix = "/tmp";
	എങ്കിൽ (snprintf(template, വലുപ്പം(template), "%s/crun.XXXXXX", prefix) < 0)
		തിരികെ -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	എങ്കിൽ (fd >= 0)
		തിരികെ fd;
	എങ്കിൽ (errno != ENOSYS && errno != EINVAL)
		പോകുക error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	എങ്കിൽ (fd >= 0) {
		ഘടന stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		എങ്കിൽ (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		എങ്കിൽ (working_otmpfile)
			തിരികെ fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	എങ്കിൽ (errno != EISDIR)
		പോകുക error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	എങ്കിൽ (fd >= 0) {
		എങ്കിൽ (unlink(template) >= 0)
			തിരികെ fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	തിരികെ -1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ seal_execfd(പൂർണ്ണസംഖ്യ *fd, പൂർണ്ണസംഖ്യ fdtype)
{
	തിരഞ്ഞെടുപ്പ് (fdtype) {
	അവസ്ഥ EFD_MEMFD:
		തിരികെ fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	അവസ്ഥ EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		പൂർണ്ണസംഖ്യ newfd;
		അക്ഷരം fdpath[PATH_MAX] = {0};

		എങ്കിൽ (fchmod(*fd, 0100) < 0)
			തിരികെ -1;

		എങ്കിൽ (snprintf(fdpath, വലുപ്പം(fdpath), "/proc/self/fd/%d", *fd) < 0)
			തിരികെ -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		എങ്കിൽ (newfd < 0)
			തിരികെ -1;

		close(*fd);
		*fd = newfd;
		തിരികെ 0;
	}
	സ്ഥിരസ്ഥിതി:
	   നിർത്തുക;
	}
	തിരികെ -1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ try_bindfd_mount_api(ശൂന്യം)
{
	libcrun_error_t err;
	പൂർണ്ണസംഖ്യ mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	എങ്കിൽ (mountfd < 0) {
		crun_error_release (&err);
		തിരികെ -1;
	}
	തിരികെ mountfd;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ try_bindfd(ശൂന്യം)
{
	mode_t mask;
	പൂർണ്ണസംഖ്യ fd, ret = -1;
	അക്ഷരം template[PATH_MAX] = {0};
	അക്ഷരം *prefix = getenv("_LIBCONTAINER_STATEDIR");

	എങ്കിൽ (!prefix || *prefix != '/')
		prefix = "/tmp";
	എങ്കിൽ (snprintf(template, വലുപ്പം(template), "%s/crun.XXXXXX", prefix) < 0)
		തിരികെ ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	എങ്കിൽ (fd < 0)
		തിരികെ ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	എങ്കിൽ (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		പോകുക out;
	എങ്കിൽ (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		പോകുക out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(template, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	എങ്കിൽ (umount2(template, MNT_DETACH) < 0) {
		എങ്കിൽ (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(template);
	തിരികെ ret;
}

സ്ഥിതം ssize_t fd_to_fd(പൂർണ്ണസംഖ്യ outfd, പൂർണ്ണസംഖ്യ infd)
{
	ssize_t total = 0;
	അക്ഷരം buffer[4096];

	വേണ്ടി (;;) {
		ssize_t nread, nwritten = 0;

		nread = പഠ(infd, buffer, വലുപ്പം(buffer));
		എങ്കിൽ (nread < 0)
			തിരികെ -1;
		എങ്കിൽ (!nread)
			നിർത്തുക;

		ചെയ്യുക {
			ssize_t n = ലിഖ(outfd, buffer + nwritten, nread - nwritten);
			എങ്കിൽ (n < 0)
				തിരികെ -1;
			nwritten += n;
		} ആകുമ്പോൾ(nwritten < nread);

		total += nwritten;
	}

	തിരികെ total;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ clone_ദ്വിമാന(ശൂന്യം)
{
	cleanup_close പൂർണ്ണസംഖ്യ execfd = -1;
	ഘടന stat statbuf = {};
	ssize_t sent = 0;
	പൂർണ്ണസംഖ്യ fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	എങ്കിൽ (execfd >= 0) {
		/* Transfer ownership to caller */
		പൂർണ്ണസംഖ്യ ret_execfd = execfd;
		execfd = -1;
		തിരികെ ret_execfd;
	}
	execfd = try_bindfd();
	എങ്കിൽ (execfd >= 0) {
		/* Transfer ownership to caller */
		പൂർണ്ണസംഖ്യ ret_execfd = execfd;
		execfd = -1;
		തിരികെ ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	എങ്കിൽ (execfd < 0 || fdtype == EFD_NONE)
		തിരികെ -ENOTRECOVERABLE;

	{
		cleanup_close പൂർണ്ണസംഖ്യ binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		എങ്കിൽ (binfd < 0)
			പോകുക error;

		എങ്കിൽ (fstat(binfd, &statbuf) < 0)
			പോകുക error;

		ആകുമ്പോൾ (sent < statbuf.st_size) {
			പൂർണ്ണസംഖ്യ n = sendfile(execfd, binfd, ശൂന്യ, statbuf.st_size - sent);
			എങ്കിൽ (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				എങ്കിൽ (n < 0)
					പോകുക error;
			}
			sent += n;
		}
	}
	എങ്കിൽ (sent != statbuf.st_size)
		പോകുക error;

	എങ്കിൽ (seal_execfd(&execfd, fdtype) < 0)
		പോകുക error;

	{
		/* Transfer ownership to caller */
		പൂർണ്ണസംഖ്യ ret_execfd = execfd;
		execfd = -1;
		തിരികെ ret_execfd;
	}
error:
	തിരികെ -EIO;
}

/* Get cheap access to the environment. */
ബാഹ്യ അക്ഷരം **environ;

പൂർണ്ണസംഖ്യ ensure_cloned_ദ്വിമാന(ശൂന്യം)
{
	cleanup_close പൂർണ്ണസംഖ്യ execfd = -1;
	അക്ഷരം **argv = ശൂന്യ;

	/* Check that we're not self-cloned, and if we are then bail. */
	പൂർണ്ണസംഖ്യ cloned = is_self_cloned();
	എങ്കിൽ (cloned > 0 || cloned == -ENOTRECOVERABLE)
		തിരികെ cloned;

	എങ്കിൽ (fetchve(&argv) < 0)
		തിരികെ -EINVAL;

	execfd = clone_ദ്വിമാന();
	എങ്കിൽ (execfd < 0)
		തിരികെ -EIO;

	എങ്കിൽ (putenv(CLONED_ദ്വിമാന_ENV "=1"))
		പോകുക error;

	fexecve(execfd, argv, environ);
error:
	തിരികെ -ENOEXEC;
}
