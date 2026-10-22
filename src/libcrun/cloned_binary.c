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

#घोषणा _GNU_SOURCE
#समावेश <unistd.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <stdbool.h>
#समावेश <string.h>
#समावेश <limits.h>
#समावेश <fcntl.h>
#समावेश <errno.h>

#समावेश <sys/types.h>
#समावेश <sys/stat.h>
#समावेश <sys/statfs.h>
#समावेश <sys/vfs.h>
#समावेश <sys/mman.h>
#समावेश <sys/mount.h>
#समावेश <sys/sendfile.h>
#समावेश <sys/syscall.h>

#समावेश "utils.h"
#समावेश "linux.h"

/* Use our own wrapper for memfd_create. */
#अगर !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#पूर्ण_अगर
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#अगर_अघोषित MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#पूर्ण_अगर
पूर्णांक memfd_create(स्थिर अक्षर *name, अचिन्हित पूर्णांक flags)
{
#अगर_घोषित SYS_memfd_create
	वापस syscall(SYS_memfd_create, name, flags);
#अन्यथा
	त्रुटि_सं = ENOSYS;
	वापस -1;
#पूर्ण_अगर
}


/* This comes directly from <linux/fcntl.h>. */
#अगर_अघोषित F_LINUX_SPECIFIC_BASE
#  define F_LINUX_SPECIFIC_BASE 1024
#पूर्ण_अगर
#अगर_अघोषित F_ADD_SEALS
#  define F_ADD_SEALS (F_LINUX_SPECIFIC_BASE + 9)
#  define F_GET_SEALS (F_LINUX_SPECIFIC_BASE + 10)
#पूर्ण_अगर
#अगर_अघोषित F_SEAL_SEAL
#  define F_SEAL_SEAL   0x0001	/* prevent further seals from being set */
#  define F_SEAL_SHRINK 0x0002	/* prevent file from shrinking */
#  define F_SEAL_GROW   0x0004	/* prevent file from growing */
#  define F_SEAL_WRITE  0x0008	/* prevent writes */
#पूर्ण_अगर

#घोषणा CLONED_द्विआधारी_ENV "_LIBCONTAINER_CLONED_BINARY"
#घोषणा CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#घोषणा CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
अटल पूर्णांक is_self_cloned(व्योम)
{
	पूर्णांक fd, ret, is_cloned = 0;
	काष्ठा stat statbuf = {};
	काष्ठा statfs fsbuf = {};

	fd = खोलो("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	अगर (fd < 0)
		वापस -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	अगर (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		जाओ out;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	अगर (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		जाओ out;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	अगर (!दो_पर्या(CLONED_द्विआधारी_ENV)) {
		is_cloned = false;
		जाओ out;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	अगर (ख_स्थिति(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

out:
	बंद(fd);
	वापस is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
अटल अक्षर *read_file(अक्षर *path, माप_प्रकार *लंबाई)
{
	पूर्णांक fd;
	अक्षर buf[4096], *copy = शून्य;

	अगर (!लंबाई)
		वापस शून्य;

	fd = खोलो(path, O_RDONLY | O_CLOEXEC);
	अगर (fd < 0)
		वापस शून्य;

	*लंबाई = 0;
	क्रम (;;) {
		ssize_t n;

		n = पढ़ो(fd, buf, माप(buf));
		अगर (n < 0)
			जाओ error;
		अगर (!n)
			अवरोध;

		copy = xrealloc(copy, (*लंबाई + n) * माप(*copy));
		स_नकल(copy + *लंबाई, buf, n);
		*लंबाई += n;
	}
	बंद(fd);
	वापस copy;

error:
	बंद(fd);
	मुक्त(copy);
	वापस शून्य;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
अटल पूर्णांक parse_xargs(अक्षर *तथ्य, पूर्णांक तथ्य_लंबाई, अक्षर ***output)
{
	पूर्णांक अंक = 0;
	अक्षर *cur = तथ्य;

	अगर (!तथ्य || *output != शून्य)
		वापस -1;

	जबतक (cur < तथ्य + तथ्य_लंबाई) {
		अंक++;
		*output = xrealloc(*output, (अंक + 1) * माप(**output));
		(*output)[अंक - 1] = cur;
		cur += म_माप(cur) + 1;
	}
	(*output)[अंक] = शून्य;
	वापस अंक;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
अटल पूर्णांक fetchve(अक्षर ***argv)
{
	अक्षर *cmdline = शून्य;
	माप_प्रकार cmdline_आकार;

	cmdline = read_file("/proc/self/cmdline", &cmdline_आकार);
	अगर (!cmdline)
		जाओ error;

	अगर (parse_xargs(cmdline, cmdline_आकार, argv) <= 0)
		जाओ error;

	वापस 0;

error:
	मुक्त(cmdline);
	वापस -EINVAL;
}

क्रमागत {
	EFD_NONE = 0,
	EFD_MEMFD,
	EFD_FILE,
};

/*
 * This comes from <linux/fcntl.h>. We can't hard-code __O_TMPFILE because it
 * changes depending on the architecture. If we don't have O_TMPFILE we always
 * have the mkostemp(3) fallback.
 */
#अगर_अघोषित O_TMPFILE
#  अगर defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#पूर्ण_अगर

अटल पूर्णांक make_execfd(पूर्णांक *fdtype)
{
	पूर्णांक fd = -1;
	अक्षर ढाँचा[PATH_MAX] = {0};
	अक्षर *prefix = दो_पर्या("_LIBCONTAINER_STATEDIR");

	अगर (!prefix || *prefix != '/')
		prefix = "/tmp";
	अगर (snprintf(ढाँचा, माप(ढाँचा), "%s/crun.XXXXXX", prefix) < 0)
		वापस -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	अगर (fd >= 0)
		वापस fd;
	अगर (त्रुटि_सं != ENOSYS && त्रुटि_सं != EINVAL)
		जाओ error;

#अगर_घोषित O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = खोलो(prefix, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	अगर (fd >= 0) {
		काष्ठा stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		अगर (ख_स्थिति(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		अगर (working_otmpfile)
			वापस fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		बंद(fd);
		त्रुटि_सं = EISDIR;
	}
	अगर (त्रुटि_सं != EISDIR)
		जाओ error;
#पूर्ण_अगर /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(ढाँचा, O_CLOEXEC);
	अगर (fd >= 0) {
		अगर (unlink(ढाँचा) >= 0)
			वापस fd;
		बंद(fd);
	}

error:
	*fdtype = EFD_NONE;
	वापस -1;
}

अटल पूर्णांक seal_execfd(पूर्णांक *fd, पूर्णांक fdtype)
{
	चयन (fdtype) {
	हाल EFD_MEMFD:
		वापस fcntl(*fd, F_ADD_SEALS, CRUN_MEMFD_SEALS);
	हाल EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		पूर्णांक newfd;
		अक्षर fdpath[PATH_MAX] = {0};

		अगर (fchmod(*fd, 0100) < 0)
			वापस -1;

		अगर (snprintf(fdpath, माप(fdpath), "/proc/self/fd/%d", *fd) < 0)
			वापस -1;

		newfd = खोलो(fdpath, O_PATH | O_CLOEXEC);
		अगर (newfd < 0)
			वापस -1;

		बंद(*fd);
		*fd = newfd;
		वापस 0;
	}
	शेष:
	   अवरोध;
	}
	वापस -1;
}

अटल पूर्णांक try_bindfd_mount_api(व्योम)
{
	libcrun_error_t err;
	पूर्णांक mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	अगर (mountfd < 0) {
		crun_error_release (&err);
		वापस -1;
	}
	वापस mountfd;
}

अटल पूर्णांक try_bindfd(व्योम)
{
	mode_t mask;
	पूर्णांक fd, ret = -1;
	अक्षर ढाँचा[PATH_MAX] = {0};
	अक्षर *prefix = दो_पर्या("_LIBCONTAINER_STATEDIR");

	अगर (!prefix || *prefix != '/')
		prefix = "/tmp";
	अगर (snprintf(ढाँचा, माप(ढाँचा), "%s/crun.XXXXXX", prefix) < 0)
		वापस ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	mask = umask(0700);
	fd = mkstemp(ढाँचा);
	umask(mask);
	अगर (fd < 0)
		वापस ret;
	बंद(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	अगर (mount("/proc/self/exe", ढाँचा, "", MS_BIND, "") < 0)
		जाओ out;
	अगर (mount("", ढाँचा, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		जाओ out_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = खोलो(ढाँचा, O_PATH | O_CLOEXEC);

out_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	अगर (umount2(ढाँचा, MNT_DETACH) < 0) {
		अगर (ret >= 0)
			बंद(ret);
		ret = -ENOTRECOVERABLE;
	}

out:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(ढाँचा);
	वापस ret;
}

अटल ssize_t fd_to_fd(पूर्णांक outfd, पूर्णांक infd)
{
	ssize_t योग = 0;
	अक्षर बफर[4096];

	क्रम (;;) {
		ssize_t nread, nwritten = 0;

		nread = पढ़ो(infd, बफर, माप(बफर));
		अगर (nread < 0)
			वापस -1;
		अगर (!nread)
			अवरोध;

		करो {
			ssize_t n = ग_लिखो(outfd, बफर + nwritten, nread - nwritten);
			अगर (n < 0)
				वापस -1;
			nwritten += n;
		} जबतक(nwritten < nread);

		योग += nwritten;
	}

	वापस योग;
}

अटल पूर्णांक clone_द्विआधारी(व्योम)
{
	cleanup_close पूर्णांक execfd = -1;
	काष्ठा stat statbuf = {};
	ssize_t sent = 0;
	पूर्णांक fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	अगर (execfd >= 0) {
		/* Transfer ownership to caller */
		पूर्णांक ret_execfd = execfd;
		execfd = -1;
		वापस ret_execfd;
	}
	execfd = try_bindfd();
	अगर (execfd >= 0) {
		/* Transfer ownership to caller */
		पूर्णांक ret_execfd = execfd;
		execfd = -1;
		वापस ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	अगर (execfd < 0 || fdtype == EFD_NONE)
		वापस -ENOTRECOVERABLE;

	{
		cleanup_close पूर्णांक binfd = -1;
		binfd = खोलो("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		अगर (binfd < 0)
			जाओ error;

		अगर (ख_स्थिति(binfd, &statbuf) < 0)
			जाओ error;

		जबतक (sent < statbuf.st_size) {
			पूर्णांक n = sendfile(execfd, binfd, शून्य, statbuf.st_size - sent);
			अगर (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_to_fd(execfd, binfd);
				अगर (n < 0)
					जाओ error;
			}
			sent += n;
		}
	}
	अगर (sent != statbuf.st_size)
		जाओ error;

	अगर (seal_execfd(&execfd, fdtype) < 0)
		जाओ error;

	{
		/* Transfer ownership to caller */
		पूर्णांक ret_execfd = execfd;
		execfd = -1;
		वापस ret_execfd;
	}
error:
	वापस -EIO;
}

/* Get cheap access to the environment. */
बाह्य अक्षर **environ;

पूर्णांक ensure_cloned_द्विआधारी(व्योम)
{
	cleanup_close पूर्णांक execfd = -1;
	अक्षर **argv = शून्य;

	/* Check that we're not self-cloned, and if we are then bail. */
	पूर्णांक cloned = is_self_cloned();
	अगर (cloned > 0 || cloned == -ENOTRECOVERABLE)
		वापस cloned;

	अगर (fetchve(&argv) < 0)
		वापस -EINVAL;

	execfd = clone_द्विआधारी();
	अगर (execfd < 0)
		वापस -EIO;

	अगर (putenv(CLONED_द्विआधारी_ENV "=1"))
		जाओ error;

	fexecve(execfd, argv, environ);
error:
	वापस -ENOEXEC;
}
