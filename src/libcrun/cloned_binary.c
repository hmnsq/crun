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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ
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
#ಆದರೆ !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#ಆದರೆ_ಅಂತ್ಯ
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#ಆದರೆ_ಅಂತ್ಯ
ಪೂರ್ಣಾಂಕ memfd_create(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SYS_memfd_create
	ಹಿಂತಿರುಗಿ syscall(SYS_memfd_create, name, flags);
#ಇಲ್ಲದಿದ್ದರೆ
	errno = ENOSYS;
	ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}


/* This comes directly from <linux/fcntl.h>. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ F_LINUX_SPECIFIC_ತಳ
#  define F_LINUX_SPECIFIC_ತಳ 1024
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ F_ಕೂಡಿಸು_SEALS
#  define F_ಕೂಡಿಸು_SEALS (F_LINUX_SPECIFIC_ತಳ + 9)
#  define F_GET_SEALS (F_LINUX_SPECIFIC_ತಳ + 10)
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ F_SEAL_SEAL
#  define F_SEAL_SEAL   0x0001	/* prevent further seals from being set */
#  define F_SEAL_SHRINK 0x0002	/* prevent file from shrinking */
#  define F_SEAL_GROW   0x0004	/* prevent file from growing */
#  define F_SEAL_WRITE  0x0008	/* prevent writes */
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು CLONED_ದ್ವಿಮಾನ_ENV "_LIBCONTAINER_CLONED_BINARY"
#ವ್ಯಾಖ್ಯಾನಿಸು CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#ವ್ಯಾಖ್ಯಾನಿಸು CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_self_cloned(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ fd, ret, is_cloned = 0;
	ರಚನೆ stat statbuf = {};
	ರಚನೆ statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	ಆದರೆ (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	ಆದರೆ (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	ಆದರೆ (!getenv(CLONED_ದ್ವಿಮಾನ_ENV)) {
		is_cloned = false;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	ಆದರೆ (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

ಹೊರಹೋಗು:
	close(fd);
	ಹಿಂತಿರುಗಿ is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
ಸ್ಥಿರ ಅಕ್ಷರ *read_file(ಅಕ್ಷರ *ಮಾರ್ಗ, size_t *ಉದ್ದಳತೆ)
{
	ಪೂರ್ಣಾಂಕ fd;
	ಅಕ್ಷರ buf[4096], *copy = ಖಾಲಿ;

	ಆದರೆ (!ಉದ್ದಳತೆ)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	fd = open(ಮಾರ್ಗ, O_RDONLY | O_CLOEXEC);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	*ಉದ್ದಳತೆ = 0;
	ಗಾಗಿ (;;) {
		ssize_t n;

		n = ಪಠ(fd, buf, ಗಾತ್ರ(buf));
		ಆದರೆ (n < 0)
			ಹೋಗು error;
		ಆದರೆ (!n)
			ನಿಲ್ಲಿಸು;

		copy = xrealloc(copy, (*ಉದ್ದಳತೆ + n) * ಗಾತ್ರ(*copy));
		ಸ್ಮರಣೆ_ನಕಲು(copy + *ಉದ್ದಳತೆ, buf, n);
		*ಉದ್ದಳತೆ += n;
	}
	close(fd);
	ಹಿಂತಿರುಗಿ copy;

error:
	close(fd);
	ಬಿಡುಗಡೆ(copy);
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_xargs(ಅಕ್ಷರ *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ ದತ್ತಾಂಶ_ಉದ್ದಳತೆ, ಅಕ್ಷರ ***output)
{
	ಪೂರ್ಣಾಂಕ ಅಂಕೆ = 0;
	ಅಕ್ಷರ *cur = ದತ್ತಾಂಶ;

	ಆದರೆ (!ದತ್ತಾಂಶ || *output != ಖಾಲಿ)
		ಹಿಂತಿರುಗಿ -1;

	ಆಗ (cur < ದತ್ತಾಂಶ + ದತ್ತಾಂಶ_ಉದ್ದಳತೆ) {
		ಅಂಕೆ++;
		*output = xrealloc(*output, (ಅಂಕೆ + 1) * ಗಾತ್ರ(**output));
		(*output)[ಅಂಕೆ - 1] = cur;
		cur += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(cur) + 1;
	}
	(*output)[ಅಂಕೆ] = ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ ಅಂಕೆ;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fetchve(ಅಕ್ಷರ ***argv)
{
	ಅಕ್ಷರ *cmdline = ಖಾಲಿ;
	size_t cmdline_ಗಾತ್ರ;

	cmdline = read_file("/proc/self/cmdline", &cmdline_ಗಾತ್ರ);
	ಆದರೆ (!cmdline)
		ಹೋಗು error;

	ಆದರೆ (parse_xargs(cmdline, cmdline_ಗಾತ್ರ, argv) <= 0)
		ಹೋಗು error;

	ಹಿಂತಿರುಗಿ 0;

error:
	ಬಿಡುಗಡೆ(cmdline);
	ಹಿಂತಿರುಗಿ -EINVAL;
}

ಎಣಿಕೆ {
	EFD_NONE = 0,
	EFD_MEMFD,
	EFD_FILE,
};

/*
 * This comes from <linux/fcntl.h>. We can't hard-code __O_TMPFILE because it
 * changes depending on the architecture. If we don't have O_TMPFILE we always
 * have the mkostemp(3) fallback.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ O_TMPFILE
#  ಆದರೆ defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ make_execfd(ಪೂರ್ಣಾಂಕ *fdtype)
{
	ಪೂರ್ಣಾಂಕ fd = -1;
	ಅಕ್ಷರ ಮಾದರಿ[PATH_MAX] = {0};
	ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = getenv("_LIBCONTAINER_STATEDIR");

	ಆದರೆ (!ಪೂರ್ವಪ್ರತ್ಯಯ || *ಪೂರ್ವಪ್ರತ್ಯಯ != '/')
		ಪೂರ್ವಪ್ರತ್ಯಯ = "/tmp";
	ಆದರೆ (snprintf(ಮಾದರಿ, ಗಾತ್ರ(ಮಾದರಿ), "%s/crun.XXXXXX", ಪೂರ್ವಪ್ರತ್ಯಯ) < 0)
		ಹಿಂತಿರುಗಿ -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	ಆದರೆ (fd >= 0)
		ಹಿಂತಿರುಗಿ fd;
	ಆದರೆ (errno != ENOSYS && errno != EINVAL)
		ಹೋಗು error;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(ಪೂರ್ವಪ್ರತ್ಯಯ, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	ಆದರೆ (fd >= 0) {
		ರಚನೆ stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		ಆದರೆ (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		ಆದರೆ (working_otmpfile)
			ಹಿಂತಿರುಗಿ fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	ಆದರೆ (errno != EISDIR)
		ಹೋಗು error;
#ಆದರೆ_ಅಂತ್ಯ /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(ಮಾದರಿ, O_CLOEXEC);
	ಆದರೆ (fd >= 0) {
		ಆದರೆ (unlink(ಮಾದರಿ) >= 0)
			ಹಿಂತಿರುಗಿ fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ seal_execfd(ಪೂರ್ಣಾಂಕ *fd, ಪೂರ್ಣಾಂಕ fdtype)
{
	ಆಯ್ಕೆ (fdtype) {
	ಸ್ಥಿತಿ EFD_MEMFD:
		ಹಿಂತಿರುಗಿ fcntl(*fd, F_ಕೂಡಿಸು_SEALS, CRUN_MEMFD_SEALS);
	ಸ್ಥಿತಿ EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		ಪೂರ್ಣಾಂಕ newfd;
		ಅಕ್ಷರ fdpath[PATH_MAX] = {0};

		ಆದರೆ (fchmod(*fd, 0100) < 0)
			ಹಿಂತಿರುಗಿ -1;

		ಆದರೆ (snprintf(fdpath, ಗಾತ್ರ(fdpath), "/proc/self/fd/%d", *fd) < 0)
			ಹಿಂತಿರುಗಿ -1;

		newfd = open(fdpath, O_ಮಾರ್ಗ | O_CLOEXEC);
		ಆದರೆ (newfd < 0)
			ಹಿಂತಿರುಗಿ -1;

		close(*fd);
		*fd = newfd;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಪೂರ್ವನಿಯೋಜಿತ:
	   ನಿಲ್ಲಿಸು;
	}
	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_bindfd_mount_api(ಶೂನ್ಯ)
{
	libcrun_error_t err;
	ಪೂರ್ಣಾಂಕ mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	ಆದರೆ (mountfd < 0) {
		crun_error_release (&err);
		ಹಿಂತಿರುಗಿ -1;
	}
	ಹಿಂತಿರುಗಿ mountfd;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_bindfd(ಶೂನ್ಯ)
{
	mode_t ಮುಖವಾಡ;
	ಪೂರ್ಣಾಂಕ fd, ret = -1;
	ಅಕ್ಷರ ಮಾದರಿ[PATH_MAX] = {0};
	ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = getenv("_LIBCONTAINER_STATEDIR");

	ಆದರೆ (!ಪೂರ್ವಪ್ರತ್ಯಯ || *ಪೂರ್ವಪ್ರತ್ಯಯ != '/')
		ಪೂರ್ವಪ್ರತ್ಯಯ = "/tmp";
	ಆದರೆ (snprintf(ಮಾದರಿ, ಗಾತ್ರ(ಮಾದರಿ), "%s/crun.XXXXXX", ಪೂರ್ವಪ್ರತ್ಯಯ) < 0)
		ಹಿಂತಿರುಗಿ ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	ಮುಖವಾಡ = umask(0700);
	fd = mkstemp(ಮಾದರಿ);
	umask(ಮುಖವಾಡ);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	ಆದರೆ (mount("/proc/self/exe", ಮಾದರಿ, "", MS_BIND, "") < 0)
		ಹೋಗು ಹೊರಹೋಗು;
	ಆದರೆ (mount("", ಮಾದರಿ, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		ಹೋಗು ಹೊರಹೋಗು_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(ಮಾದರಿ, O_ಮಾರ್ಗ | O_CLOEXEC);

ಹೊರಹೋಗು_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	ಆದರೆ (umount2(ಮಾದರಿ, MNT_DETACH) < 0) {
		ಆದರೆ (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

ಹೊರಹೋಗು:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(ಮಾದರಿ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ssize_t fd_ಗಮ್ಯ_fd(ಪೂರ್ಣಾಂಕ outfd, ಪೂರ್ಣಾಂಕ infd)
{
	ssize_t ಒಟ್ಟು = 0;
	ಅಕ್ಷರ ಬಫರ್[4096];

	ಗಾಗಿ (;;) {
		ssize_t nread, nwritten = 0;

		nread = ಪಠ(infd, ಬಫರ್, ಗಾತ್ರ(ಬಫರ್));
		ಆದರೆ (nread < 0)
			ಹಿಂತಿರುಗಿ -1;
		ಆದರೆ (!nread)
			ನಿಲ್ಲಿಸು;

		ಮಾಡು {
			ssize_t n = ಲಿಖ(outfd, ಬಫರ್ + nwritten, nread - nwritten);
			ಆದರೆ (n < 0)
				ಹಿಂತಿರುಗಿ -1;
			nwritten += n;
		} ಆಗ(nwritten < nread);

		ಒಟ್ಟು += nwritten;
	}

	ಹಿಂತಿರುಗಿ ಒಟ್ಟು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ clone_ದ್ವಿಮಾನ(ಶೂನ್ಯ)
{
	cleanup_close ಪೂರ್ಣಾಂಕ execfd = -1;
	ರಚನೆ stat statbuf = {};
	ssize_t ಕಳುಹಿಸಿದ = 0;
	ಪೂರ್ಣಾಂಕ fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	ಆದರೆ (execfd >= 0) {
		/* Transfer ownership to caller */
		ಪೂರ್ಣಾಂಕ ret_execfd = execfd;
		execfd = -1;
		ಹಿಂತಿರುಗಿ ret_execfd;
	}
	execfd = try_bindfd();
	ಆದರೆ (execfd >= 0) {
		/* Transfer ownership to caller */
		ಪೂರ್ಣಾಂಕ ret_execfd = execfd;
		execfd = -1;
		ಹಿಂತಿರುಗಿ ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	ಆದರೆ (execfd < 0 || fdtype == EFD_NONE)
		ಹಿಂತಿರುಗಿ -ENOTRECOVERABLE;

	{
		cleanup_close ಪೂರ್ಣಾಂಕ binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		ಆದರೆ (binfd < 0)
			ಹೋಗು error;

		ಆದರೆ (fstat(binfd, &statbuf) < 0)
			ಹೋಗು error;

		ಆಗ (ಕಳುಹಿಸಿದ < statbuf.st_size) {
			ಪೂರ್ಣಾಂಕ n = sendfile(execfd, binfd, ಖಾಲಿ, statbuf.st_size - ಕಳುಹಿಸಿದ);
			ಆದರೆ (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_ಗಮ್ಯ_fd(execfd, binfd);
				ಆದರೆ (n < 0)
					ಹೋಗು error;
			}
			ಕಳುಹಿಸಿದ += n;
		}
	}
	ಆದರೆ (ಕಳುಹಿಸಿದ != statbuf.st_size)
		ಹೋಗು error;

	ಆದರೆ (seal_execfd(&execfd, fdtype) < 0)
		ಹೋಗು error;

	{
		/* Transfer ownership to caller */
		ಪೂರ್ಣಾಂಕ ret_execfd = execfd;
		execfd = -1;
		ಹಿಂತಿರುಗಿ ret_execfd;
	}
error:
	ಹಿಂತಿರುಗಿ -EIO;
}

/* Get cheap access to the environment. */
ಬಾಹ್ಯ ಅಕ್ಷರ **environ;

ಪೂರ್ಣಾಂಕ ensure_cloned_ದ್ವಿಮಾನ(ಶೂನ್ಯ)
{
	cleanup_close ಪೂರ್ಣಾಂಕ execfd = -1;
	ಅಕ್ಷರ **argv = ಖಾಲಿ;

	/* Check that we're not self-cloned, and if we are then bail. */
	ಪೂರ್ಣಾಂಕ cloned = is_self_cloned();
	ಆದರೆ (cloned > 0 || cloned == -ENOTRECOVERABLE)
		ಹಿಂತಿರುಗಿ cloned;

	ಆದರೆ (fetchve(&argv) < 0)
		ಹಿಂತಿರುಗಿ -EINVAL;

	execfd = clone_ದ್ವಿಮಾನ();
	ಆದರೆ (execfd < 0)
		ಹಿಂತಿರುಗಿ -EIO;

	ಆದರೆ (putenv(CLONED_ದ್ವಿಮಾನ_ENV "=1"))
		ಹೋಗು error;

	fexecve(execfd, argv, environ);
error:
	ಹಿಂತಿರುಗಿ -ENOEXEC;
}
