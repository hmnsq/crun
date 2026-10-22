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

#परिभाषय _GNU_SOURCE
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
#यदि !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#यदि_अन्तः
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#यदि_अपरिभाषितम् MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#यदि_अन्तः
पूर्णाङ्क memfd_create(ध्रुव वर्ण *name, अचिह्नित पूर्णाङ्क flags)
{
#यदि_परिभाषितम् SYS_memfd_create
	प्रत्यागम syscall(SYS_memfd_create, name, flags);
#अन्यथा
	errno = ENOSYS;
	प्रत्यागम -1;
#यदि_अन्तः
}


/* This comes directly from <linux/fcntl.h>. */
#यदि_अपरिभाषितम् F_LINUX_SPECIFIC_आधारः
#  define F_LINUX_SPECIFIC_आधारः 1024
#यदि_अन्तः
#यदि_अपरिभाषितम् F_सङ्कलय_SEALS
#  define F_सङ्कलय_SEALS (F_LINUX_SPECIFIC_आधारः + 9)
#  define F_GET_SEALS (F_LINUX_SPECIFIC_आधारः + 10)
#यदि_अन्तः
#यदि_अपरिभाषितम् F_SEAL_SEAL
#  define F_SEAL_SEAL   0x0001	/* prevent further seals from being set */
#  define F_SEAL_SHRINK 0x0002	/* prevent file from shrinking */
#  define F_SEAL_GROW   0x0004	/* prevent file from growing */
#  define F_SEAL_WRITE  0x0008	/* prevent writes */
#यदि_अन्तः

#परिभाषय CLONED_द्विमान_ENV "_LIBCONTAINER_CLONED_BINARY"
#परिभाषय CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#परिभाषय CRUN_MEMFD_SEALS \
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

	fd = उद्घाटय("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	यदि (fd < 0)
		प्रत्यागम -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	यदि (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		गच्छ बहिर्गमनम्;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	यदि (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		गच्छ बहिर्गमनम्;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	यदि (!getenv(CLONED_द्विमान_ENV)) {
		is_cloned = false;
		गच्छ बहिर्गमनम्;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	यदि (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

बहिर्गमनम्:
	close(fd);
	प्रत्यागम is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
स्थिर वर्ण *read_file(वर्ण *मार्गः, size_t *दीर्घता)
{
	पूर्णाङ्क fd;
	वर्ण buf[4096], *copy = रिक्त;

	यदि (!दीर्घता)
		प्रत्यागम रिक्त;

	fd = उद्घाटय(मार्गः, O_RDONLY | O_CLOEXEC);
	यदि (fd < 0)
		प्रत्यागम रिक्त;

	*दीर्घता = 0;
	प्रति (;;) {
		ssize_t n;

		n = पठ(fd, buf, परिमाण(buf));
		यदि (n < 0)
			गच्छ error;
		यदि (!n)
			भङ्ग;

		copy = xrealloc(copy, (*दीर्घता + n) * परिमाण(*copy));
		स्मृतिं_प्रतिलिख(copy + *दीर्घता, buf, n);
		*दीर्घता += n;
	}
	close(fd);
	प्रत्यागम copy;

error:
	close(fd);
	मुक्त(copy);
	प्रत्यागम रिक्त;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
स्थिर पूर्णाङ्क parse_xargs(वर्ण *दत्त, पूर्णाङ्क दत्त_दीर्घता, वर्ण ***output)
{
	पूर्णाङ्क अङ्कः = 0;
	वर्ण *cur = दत्त;

	यदि (!दत्त || *output != रिक्त)
		प्रत्यागम -1;

	यावत् (cur < दत्त + दत्त_दीर्घता) {
		अङ्कः++;
		*output = xrealloc(*output, (अङ्कः + 1) * परिमाण(**output));
		(*output)[अङ्कः - 1] = cur;
		cur += वाक्यदैर्घ्यम्(cur) + 1;
	}
	(*output)[अङ्कः] = रिक्त;
	प्रत्यागम अङ्कः;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
स्थिर पूर्णाङ्क fetchve(वर्ण ***argv)
{
	वर्ण *cmdline = रिक्त;
	size_t cmdline_आकार;

	cmdline = read_file("/proc/self/cmdline", &cmdline_आकार);
	यदि (!cmdline)
		गच्छ error;

	यदि (parse_xargs(cmdline, cmdline_आकार, argv) <= 0)
		गच्छ error;

	प्रत्यागम 0;

error:
	मुक्त(cmdline);
	प्रत्यागम -EINVAL;
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
#यदि_अपरिभाषितम् O_TMPFILE
#  यदि defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#यदि_अन्तः

स्थिर पूर्णाङ्क make_execfd(पूर्णाङ्क *fdtype)
{
	पूर्णाङ्क fd = -1;
	वर्ण प्रतिरूपम्[PATH_MAX] = {0};
	वर्ण *उपसर्गः = getenv("_LIBCONTAINER_STATEDIR");

	यदि (!उपसर्गः || *उपसर्गः != '/')
		उपसर्गः = "/tmp";
	यदि (snprintf(प्रतिरूपम्, परिमाण(प्रतिरूपम्), "%s/crun.XXXXXX", उपसर्गः) < 0)
		प्रत्यागम -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	यदि (fd >= 0)
		प्रत्यागम fd;
	यदि (errno != ENOSYS && errno != EINVAL)
		गच्छ error;

#यदि_परिभाषितम् O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = उद्घाटय(उपसर्गः, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	यदि (fd >= 0) {
		संरचना stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		यदि (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		यदि (working_otmpfile)
			प्रत्यागम fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	यदि (errno != EISDIR)
		गच्छ error;
#यदि_अन्तः /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(प्रतिरूपम्, O_CLOEXEC);
	यदि (fd >= 0) {
		यदि (unlink(प्रतिरूपम्) >= 0)
			प्रत्यागम fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	प्रत्यागम -1;
}

स्थिर पूर्णाङ्क seal_execfd(पूर्णाङ्क *fd, पूर्णाङ्क fdtype)
{
	विकल्प (fdtype) {
	पक्ष EFD_MEMFD:
		प्रत्यागम fcntl(*fd, F_सङ्कलय_SEALS, CRUN_MEMFD_SEALS);
	पक्ष EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		पूर्णाङ्क newfd;
		वर्ण fdpath[PATH_MAX] = {0};

		यदि (fchmod(*fd, 0100) < 0)
			प्रत्यागम -1;

		यदि (snprintf(fdpath, परिमाण(fdpath), "/proc/self/fd/%d", *fd) < 0)
			प्रत्यागम -1;

		newfd = उद्घाटय(fdpath, O_मार्गः | O_CLOEXEC);
		यदि (newfd < 0)
			प्रत्यागम -1;

		close(*fd);
		*fd = newfd;
		प्रत्यागम 0;
	}
	स्वभाव:
	   भङ्ग;
	}
	प्रत्यागम -1;
}

स्थिर पूर्णाङ्क try_bindfd_mount_api(शून्य)
{
	libcrun_error_t err;
	पूर्णाङ्क mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	यदि (mountfd < 0) {
		crun_error_release (&err);
		प्रत्यागम -1;
	}
	प्रत्यागम mountfd;
}

स्थिर पूर्णाङ्क try_bindfd(शून्य)
{
	mode_t आवरणम्;
	पूर्णाङ्क fd, ret = -1;
	वर्ण प्रतिरूपम्[PATH_MAX] = {0};
	वर्ण *उपसर्गः = getenv("_LIBCONTAINER_STATEDIR");

	यदि (!उपसर्गः || *उपसर्गः != '/')
		उपसर्गः = "/tmp";
	यदि (snprintf(प्रतिरूपम्, परिमाण(प्रतिरूपम्), "%s/crun.XXXXXX", उपसर्गः) < 0)
		प्रत्यागम ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	आवरणम् = umask(0700);
	fd = mkstemp(प्रतिरूपम्);
	umask(आवरणम्);
	यदि (fd < 0)
		प्रत्यागम ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	यदि (mount("/proc/self/exe", प्रतिरूपम्, "", MS_BIND, "") < 0)
		गच्छ बहिर्गमनम्;
	यदि (mount("", प्रतिरूपम्, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		गच्छ बहिर्गमनम्_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = उद्घाटय(प्रतिरूपम्, O_मार्गः | O_CLOEXEC);

बहिर्गमनम्_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	यदि (umount2(प्रतिरूपम्, MNT_DETACH) < 0) {
		यदि (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

बहिर्गमनम्:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(प्रतिरूपम्);
	प्रत्यागम ret;
}

स्थिर ssize_t fd_गन्तव्यम्_fd(पूर्णाङ्क outfd, पूर्णाङ्क infd)
{
	ssize_t योग = 0;
	वर्ण बफर[4096];

	प्रति (;;) {
		ssize_t nread, nwritten = 0;

		nread = पठ(infd, बफर, परिमाण(बफर));
		यदि (nread < 0)
			प्रत्यागम -1;
		यदि (!nread)
			भङ्ग;

		कुरु {
			ssize_t n = लिख(outfd, बफर + nwritten, nread - nwritten);
			यदि (n < 0)
				प्रत्यागम -1;
			nwritten += n;
		} यावत्(nwritten < nread);

		योग += nwritten;
	}

	प्रत्यागम योग;
}

स्थिर पूर्णाङ्क clone_द्विमान(शून्य)
{
	cleanup_close पूर्णाङ्क execfd = -1;
	संरचना stat statbuf = {};
	ssize_t प्रेषितम् = 0;
	पूर्णाङ्क fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	यदि (execfd >= 0) {
		/* Transfer ownership to caller */
		पूर्णाङ्क ret_execfd = execfd;
		execfd = -1;
		प्रत्यागम ret_execfd;
	}
	execfd = try_bindfd();
	यदि (execfd >= 0) {
		/* Transfer ownership to caller */
		पूर्णाङ्क ret_execfd = execfd;
		execfd = -1;
		प्रत्यागम ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	यदि (execfd < 0 || fdtype == EFD_NONE)
		प्रत्यागम -ENOTRECOVERABLE;

	{
		cleanup_close पूर्णाङ्क binfd = -1;
		binfd = उद्घाटय("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		यदि (binfd < 0)
			गच्छ error;

		यदि (fstat(binfd, &statbuf) < 0)
			गच्छ error;

		यावत् (प्रेषितम् < statbuf.st_size) {
			पूर्णाङ्क n = sendfile(execfd, binfd, रिक्त, statbuf.st_size - प्रेषितम्);
			यदि (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_गन्तव्यम्_fd(execfd, binfd);
				यदि (n < 0)
					गच्छ error;
			}
			प्रेषितम् += n;
		}
	}
	यदि (प्रेषितम् != statbuf.st_size)
		गच्छ error;

	यदि (seal_execfd(&execfd, fdtype) < 0)
		गच्छ error;

	{
		/* Transfer ownership to caller */
		पूर्णाङ्क ret_execfd = execfd;
		execfd = -1;
		प्रत्यागम ret_execfd;
	}
error:
	प्रत्यागम -EIO;
}

/* Get cheap access to the environment. */
बाह्य वर्ण **environ;

पूर्णाङ्क ensure_cloned_द्विमान(शून्य)
{
	cleanup_close पूर्णाङ्क execfd = -1;
	वर्ण **argv = रिक्त;

	/* Check that we're not self-cloned, and if we are then bail. */
	पूर्णाङ्क cloned = is_self_cloned();
	यदि (cloned > 0 || cloned == -ENOTRECOVERABLE)
		प्रत्यागम cloned;

	यदि (fetchve(&argv) < 0)
		प्रत्यागम -EINVAL;

	execfd = clone_द्विमान();
	यदि (execfd < 0)
		प्रत्यागम -EIO;

	यदि (putenv(CLONED_द्विमान_ENV "=1"))
		गच्छ error;

	fexecve(execfd, argv, environ);
error:
	प्रत्यागम -ENOEXEC;
}
