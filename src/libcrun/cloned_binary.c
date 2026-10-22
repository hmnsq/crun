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
#जर !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#endif
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#ifndef MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#endif
पूर्णांक memfd_create(स्थिरांक अक्षर *name, अचिन्हित पूर्णांक flags)
{
#ifdef SYS_memfd_create
	परत syscall(SYS_memfd_create, name, flags);
#अन्यथा
	त्रुटि_सं = ENOSYS;
	परत -1;
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
स्थिर पूर्णांक is_self_cloned(शून्य)
{
	पूर्णांक fd, ret, is_cloned = 0;
	संरचना stat statbuf = {};
	संरचना statfs fsbuf = {};

	fd = खोलो("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	जर (fd < 0)
		परत -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	जर (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		जा out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	जर (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		जा out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	जर (!दो_पर्या(CLONED_द्विमान_ENV)) {
		is_cloned = false;
		जा out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	जर (ख_स्थिति(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	बंद(fd);
	परत is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
स्थिर अक्षर *read_file(अक्षर *path, माप_प्रकार *length)
{
	पूर्णांक fd;
	अक्षर buf[4096], *copy = रिक्त;

	जर (!length)
		परत रिक्त;

	fd = खोलो(path, O_RDONLY | O_CLOEXEC);
	जर (fd < 0)
		परत रिक्त;

	*length = 0;
	साठी (;;) {
		ssize_t n;

		n = पठ(fd, buf, आकार(buf));
		जर (n < 0)
			जा error;
		जर (!n)
			थांबा;

		copy = xrealloc(copy, (*length + n) * आकार(*copy));
		स_नकल(copy + *length, buf, n);
		*length += n;
	}
	बंद(fd);
	परत copy;

error:
	बंद(fd);
	मुक्त(copy);
	परत रिक्त;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
स्थिर पूर्णांक parse_xargs(अक्षर *data, पूर्णांक data_length, अक्षर ***output)
{
	पूर्णांक अंक = 0;
	अक्षर *cur = data;

	जर (!data || *output != रिक्त)
		परत -1;

	पर्यंत (cur < data + data_length) {
		अंक++;
		*output = xrealloc(*output, (अंक + 1) * आकार(**output));
		(*output)[अंक - 1] = cur;
		cur += म_माप(cur) + 1;
	}
	(*output)[अंक] = रिक्त;
	परत अंक;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
स्थिर पूर्णांक fetchve(अक्षर ***argv)
{
	अक्षर *cmdline = रिक्त;
	माप_प्रकार cmdline_size;

	cmdline = read_file("/proc/self/cmdline", &cmdline_size);
	जर (!cmdline)
		जा error;

	जर (parse_xargs(cmdline, cmdline_size, argv) <= 0)
		जा error;

	परत 0;

error:
	मुक्त(cmdline);
	परत -EINVAL;
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
#  जर defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#endif

स्थिर पूर्णांक make_execfd(पूर्णांक *fdtype)
{
	पूर्णांक fd = -1;
	अक्षर ढाँचा[PATH_MAX] = {0};
	अक्षर *prefix = दो_पर्या("_LIBCONTAINER_STATEDIR");

	जर (!prefix || *prefix != '/')
		prefix = "/tmp";
	जर (snprintf(ढाँचा, आकार(ढाँचा), "%s/crun.XXXXXX", prefix) < 0)
		परत -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	जर (fd >= 0)
		परत fd;
	जर (त्रुटि_सं != ENOSYS && त्रुटि_सं != EINVAL)
		जा error;

#ifdef O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = खोलो(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	जर (fd >= 0) {
		संरचना stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		जर (ख_स्थिति(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		जर (working_otmpfile)
			परत fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		बंद(fd);
		त्रुटि_सं = EISDIR;
	}
	जर (त्रुटि_सं != EISDIR)
		जा error;
#endif /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(ढाँचा, O_CLOEXEC);
	जर (fd >= 0) {
		जर (unlink(ढाँचा) >= 0)
			परत fd;
		बंद(fd);
	}

error:
	*fdtype = EFD_NONE;
	परत -1;
}

स्थिर पूर्णांक seal_execfd(पूर्णांक *fd, पूर्णांक fdtype)
{
	निवड (fdtype) {
	स्थिती EFD_MEMFD:
		परत fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	स्थिती EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		पूर्णांक newfd;
		अक्षर fdpath[PATH_MAX] = {0};

		जर (fchmod(*fd, 0100) < 0)
			परत -1;

		जर (snprintf(fdpath, आकार(fdpath), "/proc/self/fd/%d", *fd) < 0)
			परत -1;

		newfd = खोलो(fdpath, O_PATH | O_CLOEXEC);
		जर (newfd < 0)
			परत -1;

		बंद(*fd);
		*fd = newfd;
		परत 0;
	}
	पूर्वनिर्धारित:
	   थांबा;
	}
	परत -1;
}

स्थिर पूर्णांक try_bindfd_mount_api(शून्य)
{
	libcrun_error_t err;
	पूर्णांक mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	जर (mountfd < 0) {
		crun_error_release (&err);
		परत -1;
	}
	परत mountfd;
}

स्थिर पूर्णांक try_bindfd(शून्य)
{
	mode_t mask;
	पूर्णांक fd, ret = -1;
	अक्षर ढाँचा[PATH_MAX] = {0};
	अक्षर *prefix = दो_पर्या("_LIBCONTAINER_STATEDIR");

	जर (!prefix || *prefix != '/')
		prefix = "/tmp";
	जर (snprintf(ढाँचा, आकार(ढाँचा), "%s/crun.XXXXXX", prefix) < 0)
		परत ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(ढाँचा);
	umask(mask);
	जर (fd < 0)
		परत ret;
	बंद(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	जर (mount("/proc/self/exe", ढाँचा, "", MS_BIND, "") < 0)
		जा out;
	जर (mount("", ढाँचा, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		जा out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = खोलो(ढाँचा, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	जर (umount2(ढाँचा, MNT_DETACH) < 0) {
		जर (ret >= 0)
			बंद(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(ढाँचा);
	परत ret;
}

स्थिर ssize_t fd_to_fd(पूर्णांक outfd, पूर्णांक infd)
{
	ssize_t total = 0;
	अक्षर buffer[4096];

	साठी (;;) {
		ssize_t nread, nwritten = 0;

		nread = पठ(infd, buffer, आकार(buffer));
		जर (nread < 0)
			परत -1;
		जर (!nread)
			थांबा;

		करा {
			ssize_t n = लिख(outfd, buffer + nwritten, nread - nwritten);
			जर (n < 0)
				परत -1;
			nwritten += n;
		} पर्यंत(nwritten < nread);

		total += nwritten;
	}

	परत total;
}

स्थिर पूर्णांक clone_द्विमान(शून्य)
{
	cleanup_close पूर्णांक execfd = -1;
	संरचना stat statbuf = {};
	ssize_t sent = 0;
	पूर्णांक fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	जर (execfd >= 0) {
		/* Transfer ownership to caller */
		पूर्णांक ret_execfd = execfd;
		execfd = -1;
		परत ret_execfd;
	}
	execfd = try_bindfd();
	जर (execfd >= 0) {
		/* Transfer ownership to caller */
		पूर्णांक ret_execfd = execfd;
		execfd = -1;
		परत ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	जर (execfd < 0 || fdtype == EFD_NONE)
		परत -ENOTRECOVERABLE;

	{
		cleanup_close पूर्णांक binfd = -1;
		binfd = खोलो("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		जर (binfd < 0)
			जा error;

		जर (ख_स्थिति(binfd, &statbuf) < 0)
			जा error;

		पर्यंत (sent < statbuf.st_size) {
			पूर्णांक n = sendfile(execfd, binfd, रिक्त, statbuf.st_size - sent);
			जर (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				जर (n < 0)
					जा error;
			}
			sent += n;
		}
	}
	जर (sent != statbuf.st_size)
		जा error;

	जर (seal_execfd(&execfd, fdtype) < 0)
		जा error;

	{
		/* Transfer ownership to caller */
		पूर्णांक ret_execfd = execfd;
		execfd = -1;
		परत ret_execfd;
	}
error:
	परत -EIO;
}

/* Get cheap access to the environment. */
बाह्य अक्षर **environ;

पूर्णांक ensure_cloned_द्विमान(शून्य)
{
	cleanup_close पूर्णांक execfd = -1;
	अक्षर **argv = रिक्त;

	/* Check that we're not self-cloned, and if we are then bail. */
	पूर्णांक cloned = is_self_cloned();
	जर (cloned > 0 || cloned == -ENOTRECOVERABLE)
		परत cloned;

	जर (fetchve(&argv) < 0)
		परत -EINVAL;

	execfd = clone_द्विमान();
	जर (execfd < 0)
		परत -EIO;

	जर (putenv(CLONED_द्विमान_ENV "=1"))
		जा error;

	fexecve(execfd, argv, environ);
error:
	परत -ENOEXEC;
}
