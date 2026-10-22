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

#నిర్వచించు GNU_ఆధారం
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <errno.h>

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/statfs.h>
#అన్తర్భావ <sys/vfs.h>
#అన్తర్భావ <sys/mman.h>
#అన్తర్భావ <sys/mount.h>
#అన్తర్భావ <sys/sendfile.h>
#అన్తర్భావ <sys/syscall.h>

#అన్తర్భావ "utils.h"
#అన్తర్భావ "linux.h"

/* Use our own wrapper for memfd_create. */
#ఒకవేళ !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#అయితే_ముగింపు
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#నిర్వచితం_కాకపోతే MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#అయితే_ముగింపు
పూర్ణాంకం memfd_create(స్థిరరాశి అక్షరం *name, గుర్తులేని పూర్ణాంకం flags)
{
#నిర్వచితమైతే SYS_memfd_create
	తిరిగి syscall(SYS_memfd_create, name, flags);
#లేకపోతే
	errno = ENOSYS;
	తిరిగి -1;
#అయితే_ముగింపు
}


/* This comes directly from <linux/fcntl.h>. */
#నిర్వచితం_కాకపోతే F_LINUX_SPECIFIC_పునాది
#  define F_LINUX_SPECIFIC_పునాది 1024
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే F_కూడు_SEALS
#  define F_కూడు_SEALS (F_LINUX_SPECIFIC_పునాది + 9)
#  define F_GET_SEALS (F_LINUX_SPECIFIC_పునాది + 10)
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే F_SEAL_SEAL
#  define F_SEAL_SEAL   0x0001	/* prevent further seals from being set */
#  define F_SEAL_SHRINK 0x0002	/* prevent file from shrinking */
#  define F_SEAL_GROW   0x0004	/* prevent file from growing */
#  define F_SEAL_WRITE  0x0008	/* prevent writes */
#అయితే_ముగింపు

#నిర్వచించు CLONED_ద్విమాన_ENV "_LIBCONTAINER_CLONED_BINARY"
#నిర్వచించు CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#నిర్వచించు CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
స్థిర పూర్ణాంకం is_self_cloned(శూన్య)
{
	పూర్ణాంకం fd, ret, is_cloned = 0;
	నిర్మాణం stat statbuf = {};
	నిర్మాణం statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	ఒకవేళ (fd < 0)
		తిరిగి -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	ఒకవేళ (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		వెళ్ళు బహిర్గతం;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	ఒకవేళ (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	ఒకవేళ (!getenv(CLONED_ద్విమాన_ENV)) {
		is_cloned = false;
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	ఒకవేళ (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

బహిర్గతం:
	close(fd);
	తిరిగి is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
స్థిర అక్షరం *read_file(అక్షరం *మార్గం, size_t *నిడివి)
{
	పూర్ణాంకం fd;
	అక్షరం buf[4096], *copy = ఖాళీ;

	ఒకవేళ (!నిడివి)
		తిరిగి ఖాళీ;

	fd = open(మార్గం, O_RDONLY | O_CLOEXEC);
	ఒకవేళ (fd < 0)
		తిరిగి ఖాళీ;

	*నిడివి = 0;
	కొరకు (;;) {
		ssize_t n;

		n = పఠ(fd, buf, పరిమాణం(buf));
		ఒకవేళ (n < 0)
			వెళ్ళు error;
		ఒకవేళ (!n)
			ఆపు;

		copy = xrealloc(copy, (*నిడివి + n) * పరిమాణం(*copy));
		జ్ఞాపకం_నకలు(copy + *నిడివి, buf, n);
		*నిడివి += n;
	}
	close(fd);
	తిరిగి copy;

error:
	close(fd);
	విడుదల(copy);
	తిరిగి ఖాళీ;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
స్థిర పూర్ణాంకం parse_xargs(అక్షరం *దత్తాంశం, పూర్ణాంకం దత్తాంశం_నిడివి, అక్షరం ***output)
{
	పూర్ణాంకం అంకె = 0;
	అక్షరం *cur = దత్తాంశం;

	ఒకవేళ (!దత్తాంశం || *output != ఖాళీ)
		తిరిగి -1;

	అయితే (cur < దత్తాంశం + దత్తాంశం_నిడివి) {
		అంకె++;
		*output = xrealloc(*output, (అంకె + 1) * పరిమాణం(**output));
		(*output)[అంకె - 1] = cur;
		cur += అక్షరశ్రేణి_పొడవు(cur) + 1;
	}
	(*output)[అంకె] = ఖాళీ;
	తిరిగి అంకె;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
స్థిర పూర్ణాంకం fetchve(అక్షరం ***argv)
{
	అక్షరం *cmdline = ఖాళీ;
	size_t cmdline_కొలత;

	cmdline = read_file("/proc/self/cmdline", &cmdline_కొలత);
	ఒకవేళ (!cmdline)
		వెళ్ళు error;

	ఒకవేళ (parse_xargs(cmdline, cmdline_కొలత, argv) <= 0)
		వెళ్ళు error;

	తిరిగి 0;

error:
	విడుదల(cmdline);
	తిరిగి -EINVAL;
}

గణన {
	EFD_NONE = 0,
	EFD_MEMFD,
	EFD_FILE,
};

/*
 * This comes from <linux/fcntl.h>. We can't hard-code __O_TMPFILE because it
 * changes depending on the architecture. If we don't have O_TMPFILE we always
 * have the mkostemp(3) fallback.
 */
#నిర్వచితం_కాకపోతే O_TMPFILE
#  ఒకవేళ defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#అయితే_ముగింపు

స్థిర పూర్ణాంకం make_execfd(పూర్ణాంకం *fdtype)
{
	పూర్ణాంకం fd = -1;
	అక్షరం మూస[PATH_MAX] = {0};
	అక్షరం *ఉపసర్గ = getenv("_LIBCONTAINER_STATEDIR");

	ఒకవేళ (!ఉపసర్గ || *ఉపసర్గ != '/')
		ఉపసర్గ = "/tmp";
	ఒకవేళ (snprintf(మూస, పరిమాణం(మూస), "%s/crun.XXXXXX", ఉపసర్గ) < 0)
		తిరిగి -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	ఒకవేళ (fd >= 0)
		తిరిగి fd;
	ఒకవేళ (errno != ENOSYS && errno != EINVAL)
		వెళ్ళు error;

#నిర్వచితమైతే O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(ఉపసర్గ, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	ఒకవేళ (fd >= 0) {
		నిర్మాణం stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		ఒకవేళ (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		ఒకవేళ (working_otmpfile)
			తిరిగి fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	ఒకవేళ (errno != EISDIR)
		వెళ్ళు error;
#అయితే_ముగింపు /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(మూస, O_CLOEXEC);
	ఒకవేళ (fd >= 0) {
		ఒకవేళ (unlink(మూస) >= 0)
			తిరిగి fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	తిరిగి -1;
}

స్థిర పూర్ణాంకం seal_execfd(పూర్ణాంకం *fd, పూర్ణాంకం fdtype)
{
	ఎంపిక (fdtype) {
	స్థితి EFD_MEMFD:
		తిరిగి fcntl(*fd, F_కూడు_SEALS, CRUN_MEMFD_SEALS);
	స్థితి EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		పూర్ణాంకం newfd;
		అక్షరం fdpath[PATH_MAX] = {0};

		ఒకవేళ (fchmod(*fd, 0100) < 0)
			తిరిగి -1;

		ఒకవేళ (snprintf(fdpath, పరిమాణం(fdpath), "/proc/self/fd/%d", *fd) < 0)
			తిరిగి -1;

		newfd = open(fdpath, O_మార్గం | O_CLOEXEC);
		ఒకవేళ (newfd < 0)
			తిరిగి -1;

		close(*fd);
		*fd = newfd;
		తిరిగి 0;
	}
	అప్రమేయ:
	   ఆపు;
	}
	తిరిగి -1;
}

స్థిర పూర్ణాంకం try_bindfd_mount_api(శూన్య)
{
	libcrun_error_t err;
	పూర్ణాంకం mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	ఒకవేళ (mountfd < 0) {
		crun_error_release (&err);
		తిరిగి -1;
	}
	తిరిగి mountfd;
}

స్థిర పూర్ణాంకం try_bindfd(శూన్య)
{
	mode_t ముసుగు;
	పూర్ణాంకం fd, ret = -1;
	అక్షరం మూస[PATH_MAX] = {0};
	అక్షరం *ఉపసర్గ = getenv("_LIBCONTAINER_STATEDIR");

	ఒకవేళ (!ఉపసర్గ || *ఉపసర్గ != '/')
		ఉపసర్గ = "/tmp";
	ఒకవేళ (snprintf(మూస, పరిమాణం(మూస), "%s/crun.XXXXXX", ఉపసర్గ) < 0)
		తిరిగి ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	ముసుగు = umask(0700);
	fd = mkstemp(మూస);
	umask(ముసుగు);
	ఒకవేళ (fd < 0)
		తిరిగి ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	ఒకవేళ (mount("/proc/self/exe", మూస, "", MS_BIND, "") < 0)
		వెళ్ళు బహిర్గతం;
	ఒకవేళ (mount("", మూస, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		వెళ్ళు బహిర్గతం_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(మూస, O_మార్గం | O_CLOEXEC);

బహిర్గతం_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	ఒకవేళ (umount2(మూస, MNT_DETACH) < 0) {
		ఒకవేళ (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

బహిర్గతం:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(మూస);
	తిరిగి ret;
}

స్థిర ssize_t fd_గమ్యం_fd(పూర్ణాంకం outfd, పూర్ణాంకం infd)
{
	ssize_t మొత్తం = 0;
	అక్షరం బఫరు[4096];

	కొరకు (;;) {
		ssize_t nread, nwritten = 0;

		nread = పఠ(infd, బఫరు, పరిమాణం(బఫరు));
		ఒకవేళ (nread < 0)
			తిరిగి -1;
		ఒకవేళ (!nread)
			ఆపు;

		చేయి {
			ssize_t n = లిఖ(outfd, బఫరు + nwritten, nread - nwritten);
			ఒకవేళ (n < 0)
				తిరిగి -1;
			nwritten += n;
		} అయితే(nwritten < nread);

		మొత్తం += nwritten;
	}

	తిరిగి మొత్తం;
}

స్థిర పూర్ణాంకం clone_ద్విమాన(శూన్య)
{
	cleanup_close పూర్ణాంకం execfd = -1;
	నిర్మాణం stat statbuf = {};
	ssize_t పంపిన = 0;
	పూర్ణాంకం fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	ఒకవేళ (execfd >= 0) {
		/* Transfer ownership to caller */
		పూర్ణాంకం ret_execfd = execfd;
		execfd = -1;
		తిరిగి ret_execfd;
	}
	execfd = try_bindfd();
	ఒకవేళ (execfd >= 0) {
		/* Transfer ownership to caller */
		పూర్ణాంకం ret_execfd = execfd;
		execfd = -1;
		తిరిగి ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	ఒకవేళ (execfd < 0 || fdtype == EFD_NONE)
		తిరిగి -ENOTRECOVERABLE;

	{
		cleanup_close పూర్ణాంకం binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		ఒకవేళ (binfd < 0)
			వెళ్ళు error;

		ఒకవేళ (fstat(binfd, &statbuf) < 0)
			వెళ్ళు error;

		అయితే (పంపిన < statbuf.st_size) {
			పూర్ణాంకం n = sendfile(execfd, binfd, ఖాళీ, statbuf.st_size - పంపిన);
			ఒకవేళ (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_గమ్యం_fd(execfd, binfd);
				ఒకవేళ (n < 0)
					వెళ్ళు error;
			}
			పంపిన += n;
		}
	}
	ఒకవేళ (పంపిన != statbuf.st_size)
		వెళ్ళు error;

	ఒకవేళ (seal_execfd(&execfd, fdtype) < 0)
		వెళ్ళు error;

	{
		/* Transfer ownership to caller */
		పూర్ణాంకం ret_execfd = execfd;
		execfd = -1;
		తిరిగి ret_execfd;
	}
error:
	తిరిగి -EIO;
}

/* Get cheap access to the environment. */
బాహ్య అక్షరం **environ;

పూర్ణాంకం ensure_cloned_ద్విమాన(శూన్య)
{
	cleanup_close పూర్ణాంకం execfd = -1;
	అక్షరం **argv = ఖాళీ;

	/* Check that we're not self-cloned, and if we are then bail. */
	పూర్ణాంకం cloned = is_self_cloned();
	ఒకవేళ (cloned > 0 || cloned == -ENOTRECOVERABLE)
		తిరిగి cloned;

	ఒకవేళ (fetchve(&argv) < 0)
		తిరిగి -EINVAL;

	execfd = clone_ద్విమాన();
	ఒకవేళ (execfd < 0)
		తిరిగి -EIO;

	ఒకవేళ (putenv(CLONED_ద్విమాన_ENV "=1"))
		వెళ్ళు error;

	fexecve(execfd, argv, environ);
error:
	తిరిగి -ENOEXEC;
}
