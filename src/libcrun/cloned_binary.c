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
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <errno.h>

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/statfs.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <sys/mman.h>
#अन्तर्भाव <sys/mount.h>
#अन्तर्भाव <sys/sendfile.h>
#अन्तर्भाव <sys/syscall.h>

#अन्तर्भाव "utils.h"
#अन्तर्भाव "linux.h"

/* Use our own wrapper for memfd_create. */
#सचे !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
पूर्णाङ्क memfd_create(ध्रुव वर्ण *name, unsigned पूर्णाङ्क flags)
{
#ifdef SYS_memfd_create
	पटिनिवत्त syscall(SYS_memfd_create, name, flags);
#अञ्ञथा
	errno = ENOSYS;
	पटिनिवत्त -1;
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

#define CLONED_द्विमान_ENV "_LIBCONTAINER_CLONED_BINARY"
#define CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#define CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
स्थिर पूर्णाङ्क is_self_cloned(शून्य)
{
	पूर्णाङ्क fd, ret, is_cloned = 0;
	संरचना stat statbuf = {};
	संरचना statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	सचे (fd < 0)
		पटिनिवत्त -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	सचे (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		गच्छ out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	सचे (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		गच्छ out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	सचे (!getenv(CLONED_द्विमान_ENV)) {
		is_cloned = false;
		गच्छ out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	सचे (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	close(fd);
	पटिनिवत्त is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
स्थिर वर्ण *read_file(वर्ण *path, size_t *length)
{
	पूर्णाङ्क fd;
	वर्ण buf[4096], *copy = रिक्त;

	सचे (!length)
		पटिनिवत्त रिक्त;

	fd = open(path, O_RDONLY | O_CLOEXEC);
	सचे (fd < 0)
		पटिनिवत्त रिक्त;

	*length = 0;
	अत्थाय (;;) {
		ssize_t n;

		n = पठ(fd, buf, परिमाण(buf));
		सचे (n < 0)
			गच्छ error;
		सचे (!n)
			भङ्ग;

		copy = xrealloc(copy, (*length + n) * परिमाण(*copy));
		memcpy(copy + *length, buf, n);
		*length += n;
	}
	close(fd);
	पटिनिवत्त copy;

error:
	close(fd);
	मुक्त(copy);
	पटिनिवत्त रिक्त;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
स्थिर पूर्णाङ्क parse_xargs(वर्ण *data, पूर्णाङ्क data_length, वर्ण ***output)
{
	पूर्णाङ्क अङ्कः = 0;
	वर्ण *cur = data;

	सचे (!data || *output != रिक्त)
		पटिनिवत्त -1;

	याव (cur < data + data_length) {
		अङ्कः++;
		*output = xrealloc(*output, (अङ्कः + 1) * परिमाण(**output));
		(*output)[अङ्कः - 1] = cur;
		cur += strlen(cur) + 1;
	}
	(*output)[अङ्कः] = रिक्त;
	पटिनिवत्त अङ्कः;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
स्थिर पूर्णाङ्क fetchve(वर्ण ***argv)
{
	वर्ण *cmdline = रिक्त;
	size_t cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	सचे (!cmdline)
		गच्छ error;

	सचे (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		गच्छ error;

	पटिनिवत्त 0;

error:
	मुक्त(cmdline);
	पटिनिवत्त -EINVAL;
}

गणना {
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
#  सचे defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

स्थिर पूर्णाङ्क make_execfd(पूर्णाङ्क *fdtype)
{
	पूर्णाङ्क fd = -1;
	वर्ण template[PATH_MAX] = {0};
	वर्ण *prefix = getenv("_LIBCONTAINER_STATEDIR");

	सचे (!prefix || *prefix != '/')
		prefix = "/tmp";
	सचे (snprintf(template, परिमाण(template), "%s/crun.XXXXXX", prefix) < 0)
		पटिनिवत्त -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	सचे (fd >= 0)
		पटिनिवत्त fd;
	सचे (errno != ENOSYS && errno != EINVAL)
		गच्छ error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	सचे (fd >= 0) {
		संरचना stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		सचे (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		सचे (working_otmpfile)
			पटिनिवत्त fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	सचे (errno != EISDIR)
		गच्छ error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(template, O_CLOEXEC);
	सचे (fd >= 0) {
		सचे (unlink(template) >= 0)
			पटिनिवत्त fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	पटिनिवत्त -1;
}

स्थिर पूर्णाङ्क seal_execfd(पूर्णाङ्क *fd, पूर्णाङ्क fdtype)
{
	विकल्प (fdtype) {
	पक्ष EFD_MEMFD:
		पटिनिवत्त fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	पक्ष EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		पूर्णाङ्क newfd;
		वर्ण fdpath[PATH_MAX] = {0};

		सचे (fchmod(*fd, 0100) < 0)
			पटिनिवत्त -1;

		सचे (snprintf(fdpath, परिमाण(fdpath), "/proc/self/fd/%d", *fd) < 0)
			पटिनिवत्त -1;

		newfd = open(fdpath, O_PATH | O_CLOEXEC);
		सचे (newfd < 0)
			पटिनिवत्त -1;

		close(*fd);
		*fd = newfd;
		पटिनिवत्त 0;
	}
	स्वभाव:
	   भङ्ग;
	}
	पटिनिवत्त -1;
}

स्थिर पूर्णाङ्क try_bindfd_mount_api(शून्य)
{
	libcrun_error_t err;
	पूर्णाङ्क mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	सचे (mountfd < 0) {
		crun_error_release (&err);
		पटिनिवत्त -1;
	}
	पटिनिवत्त mountfd;
}

स्थिर पूर्णाङ्क try_bindfd(शून्य)
{
	mode_t mask;
	पूर्णाङ्क fd, ret = -1;
	वर्ण template[PATH_MAX] = {0};
	वर्ण *prefix = getenv("_LIBCONTAINER_STATEDIR");

	सचे (!prefix || *prefix != '/')
		prefix = "/tmp";
	सचे (snprintf(template, परिमाण(template), "%s/crun.XXXXXX", prefix) < 0)
		पटिनिवत्त ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(template);
	umask(mask);
	सचे (fd < 0)
		पटिनिवत्त ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	सचे (mount("/proc/self/exe", template, "", MS_BIND, "") < 0)
		गच्छ out;
	सचे (mount("", template, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		गच्छ out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(template, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	सचे (umount2(template, MNT_DETACH) < 0) {
		सचे (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(template);
	पटिनिवत्त ret;
}

स्थिर ssize_t fd_to_fd(पूर्णाङ्क outfd, पूर्णाङ्क infd)
{
	ssize_t total = 0;
	वर्ण buffer[4096];

	अत्थाय (;;) {
		ssize_t nread, nwritten = 0;

		nread = पठ(infd, buffer, परिमाण(buffer));
		सचे (nread < 0)
			पटिनिवत्त -1;
		सचे (!nread)
			भङ्ग;

		करोहि {
			ssize_t n = लिख(outfd, buffer + nwritten, nread - nwritten);
			सचे (n < 0)
				पटिनिवत्त -1;
			nwritten += n;
		} याव(nwritten < nread);

		total += nwritten;
	}

	पटिनिवत्त total;
}

स्थिर पूर्णाङ्क clone_द्विमान(शून्य)
{
	cleanup_close पूर्णाङ्क execfd = -1;
	संरचना stat statbuf = {};
	ssize_t sent = 0;
	पूर्णाङ्क fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	सचे (execfd >= 0) {
		/* Transfer ownership to caller */
		पूर्णाङ्क ret_execfd = execfd;
		execfd = -1;
		पटिनिवत्त ret_execfd;
	}
	execfd = try_bindfd();
	सचे (execfd >= 0) {
		/* Transfer ownership to caller */
		पूर्णाङ्क ret_execfd = execfd;
		execfd = -1;
		पटिनिवत्त ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	सचे (execfd < 0 || fdtype == EFD_NONE)
		पटिनिवत्त -ENOTRECOVERABLE;

	{
		cleanup_close पूर्णाङ्क binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		सचे (binfd < 0)
			गच्छ error;

		सचे (fstat(binfd, &statbuf) < 0)
			गच्छ error;

		याव (sent < statbuf.st_size) {
			पूर्णाङ्क n = sendfile(execfd, binfd, रिक्त, statbuf.st_size - sent);
			सचे (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				सचे (n < 0)
					गच्छ error;
			}
			sent += n;
		}
	}
	सचे (sent != statbuf.st_size)
		गच्छ error;

	सचे (seal_execfd(&execfd, fdtype) < 0)
		गच्छ error;

	{
		/* Transfer ownership to caller */
		पूर्णाङ्क ret_execfd = execfd;
		execfd = -1;
		पटिनिवत्त ret_execfd;
	}
error:
	पटिनिवत्त -EIO;
}

/* Get cheap access to the environment. */
बाह्य वर्ण **environ;

पूर्णाङ्क ensure_cloned_द्विमान(शून्य)
{
	cleanup_close पूर्णाङ्क execfd = -1;
	वर्ण **argv = रिक्त;

	/* Check that we're not self-cloned, and if we are then bail. */
	पूर्णाङ्क cloned = is_self_cloned();
	सचे (cloned > 0 || cloned == -ENOTRECOVERABLE)
		पटिनिवत्त cloned;

	सचे (fetchve(&argv) < 0)
		पटिनिवत्त -EINVAL;

	execfd = clone_द्विमान();
	सचे (execfd < 0)
		पटिनिवत्त -EIO;

	सचे (putenv(CLONED_द्विमान_ENV "=1"))
		गच्छ error;

	fexecve(execfd, argv, environ);
error:
	पटिनिवत्त -ENOEXEC;
}
