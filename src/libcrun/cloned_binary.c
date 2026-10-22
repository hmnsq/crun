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
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <errno.h>

#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/statfs.h>
#અન્તર્ભાવ <sys/vfs.h>
#અન્તર્ભાવ <sys/mman.h>
#અન્તર્ભાવ <sys/mount.h>
#અન્તર્ભાવ <sys/sendfile.h>
#અન્તર્ભાવ <sys/syscall.h>

#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "linux.h"

/* Use our own wrapper for memfd_create. */
#જો !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
પૂર્ણાંક memfd_create(અચલ અક્ષર *name, unsigned પૂર્ણાંક flags)
{
#ifdef SYS_memfd_create
	પાછા syscall(SYS_memfd_create, name, flags);
#નહીંતર
	errno = ENOSYS;
	પાછા -1;
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

#define CLONED_દ્વિઅંકી_ENV "_LIBCONTAINER_CLONED_BINARY"
#define CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#define CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
સ્થિર પૂર્ણાંક is_self_cloned(શૂન્ય)
{
	પૂર્ણાંક fd, ret, is_cloned = 0;
	સંરચના stat statbuf = {};
	સંરચના statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	જો (fd < 0)
		પાછા -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	જો (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		જાઓ out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	જો (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		જાઓ out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	જો (!getenv(CLONED_દ્વિઅંકી_ENV)) {
		is_cloned = false;
		જાઓ out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	જો (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	close(fd);
	પાછા is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
સ્થિર અક્ષર *read_file(અક્ષર *path, size_t *length)
{
	પૂર્ણાંક fd;
	અક્ષર buf[4096], *copy = ખાલી;

	જો (!length)
		પાછા ખાલી;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	જો (fd < 0)
		પાછા ખાલી;

	*length = 0;
	માટે (;;) {
		ssize_t n;

		n = પઠ(fd, buf, કદ(buf));
		જો (n < 0)
			જાઓ error;
		જો (!n)
			તોડો;

		copy = xrealloc(copy, (*length + n) * કદ(*copy));
		memcpy(copy + *length, buf, n);
		*length += n;
	}
	close(fd);
	પાછા copy;

error:
	close(fd);
	મુક્ત(copy);
	પાછા ખાલી;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
સ્થિર પૂર્ણાંક parse_xargs(અક્ષર *data, પૂર્ણાંક data_length, અક્ષર ***output)
{
	પૂર્ણાંક અંક = 0;
	અક્ષર *cur = data;

	જો (!data || *output != ખાલી)
		પાછા -1;

	જ્યારે (cur < data + data_length) {
		અંક++;
		*output = xrealloc(*output, (અંક + 1) * કદ(**output));
		(*output)[અંક - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[અંક] = ખાલી;
	પાછા અંક;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
સ્થિર પૂર્ણાંક fetchve(અક્ષર ***argv)
{
	અક્ષર *cmdline = ખાલી;
	size_t cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	જો (!cmdline)
		જાઓ error;

	જો (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		જાઓ error;

	પાછા 0;

error:
	મુક્ત(cmdline);
	પાછા -EINVAL;
}

ગણના {
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
#  જો defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

સ્થિર પૂર્ણાંક make_execfd(પૂર્ણાંક *fdtype)
{
	પૂર્ણાંક fd = -1;
	અક્ષર template[PATH_MAX] = {0};
	અક્ષર *prefix = getenv("_LIBCONTAINER_STATEDIR");

	જો (!prefix || *prefix != '/')
		prefix = "/tmp";
	જો (snprintf(template, કદ(template), "%s/crun.XXXXXX", prefix) < 0)
		પાછા -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	જો (fd >= 0)
		પાછા fd;
	જો (errno != ENOSYS && errno != EINVAL)
		જાઓ error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	જો (fd >= 0) {
		સંરચના stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		જો (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		જો (working_otmpfile)
			પાછા fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	જો (errno != EISDIR)
		જાઓ error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	જો (fd >= 0) {
		જો (unlink(template) >= 0)
			પાછા fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	પાછા -1;
}

સ્થિર પૂર્ણાંક seal_execfd(પૂર્ણાંક *fd, પૂર્ણાંક fdtype)
{
	પસંદગી (fdtype) {
	સ્થિતિ EFD_MEMFD:
		પાછા fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	સ્થિતિ EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		પૂર્ણાંક newfd;
		અક્ષર fdpath[PATH_MAX] = {0};

		જો (fchmod(*fd, 0100) < 0)
			પાછા -1;

		જો (snprintf(fdpath, કદ(fdpath), "/proc/self/fd/%d", *fd) < 0)
			પાછા -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		જો (newfd < 0)
			પાછા -1;

		close(*fd);
		*fd = newfd;
		પાછા 0;
	}
	મૂળભૂત:
	   તોડો;
	}
	પાછા -1;
}

સ્થિર પૂર્ણાંક try_bindfd_mount_api(શૂન્ય)
{
	libcrun_error_t err;
	પૂર્ણાંક mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	જો (mountfd < 0) {
		crun_error_release (&err);
		પાછા -1;
	}
	પાછા mountfd;
}

સ્થિર પૂર્ણાંક try_bindfd(શૂન્ય)
{
	mode_t mask;
	પૂર્ણાંક fd, ret = -1;
	અક્ષર template[PATH_MAX] = {0};
	અક્ષર *prefix = getenv("_LIBCONTAINER_STATEDIR");

	જો (!prefix || *prefix != '/')
		prefix = "/tmp";
	જો (snprintf(template, કદ(template), "%s/crun.XXXXXX", prefix) < 0)
		પાછા ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	જો (fd < 0)
		પાછા ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	જો (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		જાઓ out;
	જો (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		જાઓ out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(template, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	જો (umount2(template, MNT_DETACH) < 0) {
		જો (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(template);
	પાછા ret;
}

સ્થિર ssize_t fd_to_fd(પૂર્ણાંક outfd, પૂર્ણાંક infd)
{
	ssize_t total = 0;
	અક્ષર buffer[4096];

	માટે (;;) {
		ssize_t nread, nwritten = 0;

		nread = પઠ(infd, buffer, કદ(buffer));
		જો (nread < 0)
			પાછા -1;
		જો (!nread)
			તોડો;

		કરો {
			ssize_t n = લિખ(outfd, buffer + nwritten, nread - nwritten);
			જો (n < 0)
				પાછા -1;
			nwritten += n;
		} જ્યારે(nwritten < nread);

		total += nwritten;
	}

	પાછા total;
}

સ્થિર પૂર્ણાંક clone_દ્વિઅંકી(શૂન્ય)
{
	cleanup_close પૂર્ણાંક execfd = -1;
	સંરચના stat statbuf = {};
	ssize_t sent = 0;
	પૂર્ણાંક fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	જો (execfd >= 0) {
		/* Transfer ownership to caller */
		પૂર્ણાંક ret_execfd = execfd;
		execfd = -1;
		પાછા ret_execfd;
	}
	execfd = try_bindfd();
	જો (execfd >= 0) {
		/* Transfer ownership to caller */
		પૂર્ણાંક ret_execfd = execfd;
		execfd = -1;
		પાછા ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	જો (execfd < 0 || fdtype == EFD_NONE)
		પાછા -ENOTRECOVERABLE;

	{
		cleanup_close પૂર્ણાંક binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		જો (binfd < 0)
			જાઓ error;

		જો (fstat(binfd, &statbuf) < 0)
			જાઓ error;

		જ્યારે (sent < statbuf.st_size) {
			પૂર્ણાંક n = sendfile(execfd, binfd, ખાલી, statbuf.st_size - sent);
			જો (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				જો (n < 0)
					જાઓ error;
			}
			sent += n;
		}
	}
	જો (sent != statbuf.st_size)
		જાઓ error;

	જો (seal_execfd(&execfd, fdtype) < 0)
		જાઓ error;

	{
		/* Transfer ownership to caller */
		પૂર્ણાંક ret_execfd = execfd;
		execfd = -1;
		પાછા ret_execfd;
	}
error:
	પાછા -EIO;
}

/* Get cheap access to the environment. */
બાહ્ય અક્ષર **environ;

પૂર્ણાંક ensure_cloned_દ્વિઅંકી(શૂન્ય)
{
	cleanup_close પૂર્ણાંક execfd = -1;
	અક્ષર **argv = ખાલી;

	/* Check that we're not self-cloned, and if we are then bail. */
	પૂર્ણાંક cloned = is_self_cloned();
	જો (cloned > 0 || cloned == -ENOTRECOVERABLE)
		પાછા cloned;

	જો (fetchve(&argv) < 0)
		પાછા -EINVAL;

	execfd = clone_દ્વિઅંકી();
	જો (execfd < 0)
		પાછા -EIO;

	જો (putenv(CLONED_દ્વિઅંકી_ENV "=1"))
		જાઓ error;

	fexecve(execfd, argv, environ);
error:
	પાછા -ENOEXEC;
}
