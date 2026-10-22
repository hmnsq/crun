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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/statfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/vfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mman.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mount.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sendfile.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "linux.h"

/* Use our own wrapper for memfd_create. */
#ਜੇ !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#ਜੇ_ਅੰਤ
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#ਜੇ_ਅੰਤ
ਪੂਰਨ memfd_create(ਅਚਲ ਅੱਖਰ *name, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SYS_memfd_create
	ਵਾਪਸ syscall(SYS_memfd_create, name, flags);
#ਨਹੀਂਤਾਂ
	errno = ENOSYS;
	ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}


/* This comes directly from <linux/fcntl.h>. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ F_LINUX_SPECIFIC_ਅਧਾਰ
#  define F_LINUX_SPECIFIC_ਅਧਾਰ 1024
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ F_ਜੋੜੋ_SEALS
#  define F_ਜੋੜੋ_SEALS (F_LINUX_SPECIFIC_ਅਧਾਰ + 9)
#  define F_GET_SEALS (F_LINUX_SPECIFIC_ਅਧਾਰ + 10)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ F_SEAL_SEAL
#  define F_SEAL_SEAL   0x0001	/* prevent further seals from being set */
#  define F_SEAL_SHRINK 0x0002	/* prevent file from shrinking */
#  define F_SEAL_GROW   0x0004	/* prevent file from growing */
#  define F_SEAL_WRITE  0x0008	/* prevent writes */
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CLONED_ਦੁਆਧਾਰੀ_ENV "_LIBCONTAINER_CLONED_BINARY"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
ਸਥਿਰ ਪੂਰਨ is_self_cloned(ਖਾਲੀ)
{
	ਪੂਰਨ fd, ret, is_cloned = 0;
	ਬਣਤਰ stat statbuf = {};
	ਬਣਤਰ statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	ਜੇ (fd < 0)
		ਵਾਪਸ -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	ਜੇ (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	ਜੇ (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	ਜੇ (!getenv(CLONED_ਦੁਆਧਾਰੀ_ENV)) {
		is_cloned = false;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	ਜੇ (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

ਬਾਹਰਲਾ:
	close(fd);
	ਵਾਪਸ is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
ਸਥਿਰ ਅੱਖਰ *read_file(ਅੱਖਰ *ਰਸਤਾ, size_t *ਲੰਮਾਈ)
{
	ਪੂਰਨ fd;
	ਅੱਖਰ buf[4096], *copy = ਨਿਰਾ;

	ਜੇ (!ਲੰਮਾਈ)
		ਵਾਪਸ ਨਿਰਾ;

	fd = open(ਰਸਤਾ, O_RDONLY | O_CLOEXEC);
	ਜੇ (fd < 0)
		ਵਾਪਸ ਨਿਰਾ;

	*ਲੰਮਾਈ = 0;
	ਲਈ (;;) {
		ssize_t n;

		n = ਪਠ(fd, buf, ਆਕਾਰ(buf));
		ਜੇ (n < 0)
			ਜਾਓ error;
		ਜੇ (!n)
			ਰੋਕੋ;

		copy = xrealloc(copy, (*ਲੰਮਾਈ + n) * ਆਕਾਰ(*copy));
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(copy + *ਲੰਮਾਈ, buf, n);
		*ਲੰਮਾਈ += n;
	}
	close(fd);
	ਵਾਪਸ copy;

error:
	close(fd);
	ਮੁਕਤ(copy);
	ਵਾਪਸ ਨਿਰਾ;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
ਸਥਿਰ ਪੂਰਨ parse_xargs(ਅੱਖਰ *ਅੰਕੜੇ, ਪੂਰਨ ਅੰਕੜੇ_ਲੰਮਾਈ, ਅੱਖਰ ***output)
{
	ਪੂਰਨ ਅੰਕ = 0;
	ਅੱਖਰ *cur = ਅੰਕੜੇ;

	ਜੇ (!ਅੰਕੜੇ || *output != ਨਿਰਾ)
		ਵਾਪਸ -1;

	ਜਦੋਂ (cur < ਅੰਕੜੇ + ਅੰਕੜੇ_ਲੰਮਾਈ) {
		ਅੰਕ++;
		*output = xrealloc(*output, (ਅੰਕ + 1) * ਆਕਾਰ(**output));
		(*output)[ਅੰਕ - 1] = cur;
		cur += ਲੜੀ_ਲੰਬਾਈ(cur) + 1;
	}
	(*output)[ਅੰਕ] = ਨਿਰਾ;
	ਵਾਪਸ ਅੰਕ;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
ਸਥਿਰ ਪੂਰਨ fetchve(ਅੱਖਰ ***argv)
{
	ਅੱਖਰ *cmdline = ਨਿਰਾ;
	size_t cmdline_ਮਾਪ;

	cmdline = read_file("/proc/self/cmdline", &cmdline_ਮਾਪ);
	ਜੇ (!cmdline)
		ਜਾਓ error;

	ਜੇ (parse_xargs(cmdline, cmdline_ਮਾਪ, argv) <= 0)
		ਜਾਓ error;

	ਵਾਪਸ 0;

error:
	ਮੁਕਤ(cmdline);
	ਵਾਪਸ -EINVAL;
}

ਗਿਣਤੀ {
	EFD_NONE = 0,
	EFD_MEMFD,
	EFD_FILE,
};

/*
 * This comes from <linux/fcntl.h>. We can't hard-code __O_TMPFILE because it
 * changes depending on the architecture. If we don't have O_TMPFILE we always
 * have the mkostemp(3) fallback.
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ O_TMPFILE
#  ਜੇ defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਪੂਰਨ make_execfd(ਪੂਰਨ *fdtype)
{
	ਪੂਰਨ fd = -1;
	ਅੱਖਰ ਸਾਂਚਾ[PATH_MAX] = {0};
	ਅੱਖਰ *ਅਗੇਤਰ = getenv("_LIBCONTAINER_STATEDIR");

	ਜੇ (!ਅਗੇਤਰ || *ਅਗੇਤਰ != '/')
		ਅਗੇਤਰ = "/tmp";
	ਜੇ (snprintf(ਸਾਂਚਾ, ਆਕਾਰ(ਸਾਂਚਾ), "%s/crun.XXXXXX", ਅਗੇਤਰ) < 0)
		ਵਾਪਸ -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	ਜੇ (fd >= 0)
		ਵਾਪਸ fd;
	ਜੇ (errno != ENOSYS && errno != EINVAL)
		ਜਾਓ error;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(ਅਗੇਤਰ, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	ਜੇ (fd >= 0) {
		ਬਣਤਰ stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		ਜੇ (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		ਜੇ (working_otmpfile)
			ਵਾਪਸ fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	ਜੇ (errno != EISDIR)
		ਜਾਓ error;
#ਜੇ_ਅੰਤ /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(ਸਾਂਚਾ, O_CLOEXEC);
	ਜੇ (fd >= 0) {
		ਜੇ (unlink(ਸਾਂਚਾ) >= 0)
			ਵਾਪਸ fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	ਵਾਪਸ -1;
}

ਸਥਿਰ ਪੂਰਨ seal_execfd(ਪੂਰਨ *fd, ਪੂਰਨ fdtype)
{
	ਚੋਣ (fdtype) {
	ਹਾਲਤ EFD_MEMFD:
		ਵਾਪਸ fcntl(*fd, F_ਜੋੜੋ_SEALS, CRUN_MEMFD_SEALS);
	ਹਾਲਤ EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		ਪੂਰਨ newfd;
		ਅੱਖਰ fdpath[PATH_MAX] = {0};

		ਜੇ (fchmod(*fd, 0100) < 0)
			ਵਾਪਸ -1;

		ਜੇ (snprintf(fdpath, ਆਕਾਰ(fdpath), "/proc/self/fd/%d", *fd) < 0)
			ਵਾਪਸ -1;

		newfd = open(fdpath, O_ਰਸਤਾ | O_CLOEXEC);
		ਜੇ (newfd < 0)
			ਵਾਪਸ -1;

		close(*fd);
		*fd = newfd;
		ਵਾਪਸ 0;
	}
	ਮੂਲ:
	   ਰੋਕੋ;
	}
	ਵਾਪਸ -1;
}

ਸਥਿਰ ਪੂਰਨ try_bindfd_mount_api(ਖਾਲੀ)
{
	libcrun_error_t err;
	ਪੂਰਨ mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	ਜੇ (mountfd < 0) {
		crun_error_release (&err);
		ਵਾਪਸ -1;
	}
	ਵਾਪਸ mountfd;
}

ਸਥਿਰ ਪੂਰਨ try_bindfd(ਖਾਲੀ)
{
	mode_t ਨਕਾਬ;
	ਪੂਰਨ fd, ret = -1;
	ਅੱਖਰ ਸਾਂਚਾ[PATH_MAX] = {0};
	ਅੱਖਰ *ਅਗੇਤਰ = getenv("_LIBCONTAINER_STATEDIR");

	ਜੇ (!ਅਗੇਤਰ || *ਅਗੇਤਰ != '/')
		ਅਗੇਤਰ = "/tmp";
	ਜੇ (snprintf(ਸਾਂਚਾ, ਆਕਾਰ(ਸਾਂਚਾ), "%s/crun.XXXXXX", ਅਗੇਤਰ) < 0)
		ਵਾਪਸ ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	ਨਕਾਬ = umask(0700);
	fd = mkstemp(ਸਾਂਚਾ);
	umask(ਨਕਾਬ);
	ਜੇ (fd < 0)
		ਵਾਪਸ ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	ਜੇ (mount("/proc/self/exe", ਸਾਂਚਾ, "", MS_BIND, "") < 0)
		ਜਾਓ ਬਾਹਰਲਾ;
	ਜੇ (mount("", ਸਾਂਚਾ, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		ਜਾਓ ਬਾਹਰਲਾ_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(ਸਾਂਚਾ, O_ਰਸਤਾ | O_CLOEXEC);

ਬਾਹਰਲਾ_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	ਜੇ (umount2(ਸਾਂਚਾ, MNT_DETACH) < 0) {
		ਜੇ (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

ਬਾਹਰਲਾ:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(ਸਾਂਚਾ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ssize_t fd_ਮੰਜ਼ਿਲ_fd(ਪੂਰਨ outfd, ਪੂਰਨ infd)
{
	ssize_t ਕੁੱਲ = 0;
	ਅੱਖਰ ਬਫਰ[4096];

	ਲਈ (;;) {
		ssize_t nread, nwritten = 0;

		nread = ਪਠ(infd, ਬਫਰ, ਆਕਾਰ(ਬਫਰ));
		ਜੇ (nread < 0)
			ਵਾਪਸ -1;
		ਜੇ (!nread)
			ਰੋਕੋ;

		ਕਰੋ {
			ssize_t n = ਲਿਖ(outfd, ਬਫਰ + nwritten, nread - nwritten);
			ਜੇ (n < 0)
				ਵਾਪਸ -1;
			nwritten += n;
		} ਜਦੋਂ(nwritten < nread);

		ਕੁੱਲ += nwritten;
	}

	ਵਾਪਸ ਕੁੱਲ;
}

ਸਥਿਰ ਪੂਰਨ clone_ਦੁਆਧਾਰੀ(ਖਾਲੀ)
{
	cleanup_close ਪੂਰਨ execfd = -1;
	ਬਣਤਰ stat statbuf = {};
	ssize_t ਭੇਜਿਆ = 0;
	ਪੂਰਨ fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	ਜੇ (execfd >= 0) {
		/* Transfer ownership to caller */
		ਪੂਰਨ ret_execfd = execfd;
		execfd = -1;
		ਵਾਪਸ ret_execfd;
	}
	execfd = try_bindfd();
	ਜੇ (execfd >= 0) {
		/* Transfer ownership to caller */
		ਪੂਰਨ ret_execfd = execfd;
		execfd = -1;
		ਵਾਪਸ ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	ਜੇ (execfd < 0 || fdtype == EFD_NONE)
		ਵਾਪਸ -ENOTRECOVERABLE;

	{
		cleanup_close ਪੂਰਨ binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		ਜੇ (binfd < 0)
			ਜਾਓ error;

		ਜੇ (fstat(binfd, &statbuf) < 0)
			ਜਾਓ error;

		ਜਦੋਂ (ਭੇਜਿਆ < statbuf.st_size) {
			ਪੂਰਨ n = sendfile(execfd, binfd, ਨਿਰਾ, statbuf.st_size - ਭੇਜਿਆ);
			ਜੇ (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_ਮੰਜ਼ਿਲ_fd(execfd, binfd);
				ਜੇ (n < 0)
					ਜਾਓ error;
			}
			ਭੇਜਿਆ += n;
		}
	}
	ਜੇ (ਭੇਜਿਆ != statbuf.st_size)
		ਜਾਓ error;

	ਜੇ (seal_execfd(&execfd, fdtype) < 0)
		ਜਾਓ error;

	{
		/* Transfer ownership to caller */
		ਪੂਰਨ ret_execfd = execfd;
		execfd = -1;
		ਵਾਪਸ ret_execfd;
	}
error:
	ਵਾਪਸ -EIO;
}

/* Get cheap access to the environment. */
ਬਾਹਰੀ ਅੱਖਰ **environ;

ਪੂਰਨ ensure_cloned_ਦੁਆਧਾਰੀ(ਖਾਲੀ)
{
	cleanup_close ਪੂਰਨ execfd = -1;
	ਅੱਖਰ **argv = ਨਿਰਾ;

	/* Check that we're not self-cloned, and if we are then bail. */
	ਪੂਰਨ cloned = is_self_cloned();
	ਜੇ (cloned > 0 || cloned == -ENOTRECOVERABLE)
		ਵਾਪਸ cloned;

	ਜੇ (fetchve(&argv) < 0)
		ਵਾਪਸ -EINVAL;

	execfd = clone_ਦੁਆਧਾਰੀ();
	ਜੇ (execfd < 0)
		ਵਾਪਸ -EIO;

	ਜੇ (putenv(CLONED_ਦੁਆਧਾਰੀ_ENV "=1"))
		ਜਾਓ error;

	fexecve(execfd, argv, environ);
error:
	ਵਾਪਸ -ENOEXEC;
}
