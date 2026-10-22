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

#تعریف_کرو GNU_ماخذ
#شامل <unistd.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <stdbool.h>
#شامل <string.h>
#شامل <limits.h>
#شامل <fcntl.h>
#شامل <errno.h>

#شامل <sys/types.h>
#شامل <sys/stat.h>
#شامل <sys/statfs.h>
#شامل <sys/vfs.h>
#شامل <sys/mman.h>
#شامل <sys/mount.h>
#شامل <sys/sendfile.h>
#شامل <sys/syscall.h>

#شامل "utils.h"
#شامل "linux.h"

/* Use our own wrapper for memfd_create. */
#اگر !defined(SYS_memfd_create) && defined(__NR_memfd_create)
#  define SYS_memfd_create __NR_memfd_create
#اگر_ختم
/* memfd_create(2) flags -- copied from <linux/memfd.h>. */
#اگر_غیر_تعریف_شدہ MFD_CLOEXEC
#  define MFD_CLOEXEC       0x0001U
#  define MFD_ALLOW_SEALING 0x0002U
#اگر_ختم
صحیح memfd_create(ثابت حرف *name, غیرعلامتی صحیح flags)
{
#اگر_تعریف_شدہ SYS_memfd_create
	واپس syscall(SYS_memfd_create, name, flags);
#ورنہ
	errno = ENOSYS;
	واپس -1;
#اگر_ختم
}


/* This comes directly from <linux/fcntl.h>. */
#اگر_غیر_تعریف_شدہ F_LINUX_SPECIFIC_بنیاد
#  define F_LINUX_SPECIFIC_بنیاد 1024
#اگر_ختم
#اگر_غیر_تعریف_شدہ F_جوڑو_SEALS
#  define F_جوڑو_SEALS (F_LINUX_SPECIFIC_بنیاد + 9)
#  define F_GET_SEALS (F_LINUX_SPECIFIC_بنیاد + 10)
#اگر_ختم
#اگر_غیر_تعریف_شدہ F_SEAL_SEAL
#  define F_SEAL_SEAL   0x0001	/* prevent further seals from being set */
#  define F_SEAL_SHRINK 0x0002	/* prevent file from shrinking */
#  define F_SEAL_GROW   0x0004	/* prevent file from growing */
#  define F_SEAL_WRITE  0x0008	/* prevent writes */
#اگر_ختم

#تعریف_کرو CLONED_ثنائی_ENV "_LIBCONTAINER_CLONED_BINARY"
#تعریف_کرو CRUN_MEMFD_COMMENT "crun_cloned:/proc/self/exe"
#تعریف_کرو CRUN_MEMFD_SEALS \
	(F_SEAL_SEAL | F_SEAL_SHRINK | F_SEAL_GROW | F_SEAL_WRITE)

/*
 * Verify whether we are currently in a self-cloned program (namely, is
 * /proc/self/exe a memfd). F_GET_SEALS will only succeed for memfds (or rather
 * for shmem files), and we want to be sure it's actually sealed.
 */
ساکن صحیح is_self_cloned(خالی)
{
	صحیح fd, ret, is_cloned = 0;
	ساخت stat statbuf = {};
	ساخت statfs fsbuf = {};

	fd = open("/proc/self/exe", O_RDONLY|O_CLOEXEC);
	اگر (fd < 0)
		واپس -ENOTRECOVERABLE;

	/*
	 * Is the binary a fully-sealed memfd? We don't need CLONED_BINARY_ENV for
	 * this, because you cannot write to a sealed memfd no matter what (so
	 * sharing it isn't a bad thing -- and an admin could bind-mount a sealed
	 * memfd to /usr/bin/crun to allow reuse).
	 */
	ret = fcntl(fd, F_GET_SEALS);
	اگر (ret >= 0) {
		is_cloned = (ret == CRUN_MEMFD_SEALS);
		جاؤ برآمد;
	}

	/* Is the binary on a read-only filesystem? We can't detect bind-mounts in
	 * particular (in-kernel they are identical to regular mounts) but we can
	 * at least be sure that it's read-only.  This occurs for multiple cases,
	 * such as truly read-only filesystems like squashfs/erofs as well as
	 * things like the ostree read-only bind mount.
	 */
	اگر (fstatfs(fd, &fsbuf) >= 0 && (fsbuf.f_flags & MS_RDONLY)) {
		is_cloned = true;
		جاؤ برآمد;
	}

	/*
	 * All other forms require CLONED_BINARY_ENV, since they are potentially
	 * writeable (or we can't tell if they're fully safe) and thus we must
	 * check the environment as an extra layer of defence.
	 */
	اگر (!getenv(CLONED_ثنائی_ENV)) {
		is_cloned = false;
		جاؤ برآمد;
	}

	/*
	 * Okay, we're a tmpfile -- or we're currently running on RHEL <=7.6
	 * which appears to have a borked backport of F_GET_SEALS. Either way,
	 * having a file which has no hardlinks indicates that we aren't using
	 * a host-side "crun" binary and this is something that a container
	 * cannot fake (because unlinking requires being able to resolve the
	 * path that you want to unlink).
	 */
	اگر (fstat(fd, &statbuf) >= 0)
		is_cloned |= (statbuf.st_nlink == 0);

برآمد:
	close(fd);
	واپس is_cloned;
}

/* Read a given file into a new buffer, and providing the length. */
ساکن حرف *read_file(حرف *راستہ, size_t *لمبائی)
{
	صحیح fd;
	حرف buf[4096], *copy = تہی;

	اگر (!لمبائی)
		واپس تہی;

	fd = open(راستہ, O_RDONLY | O_CLOEXEC);
	اگر (fd < 0)
		واپس تہی;

	*لمبائی = 0;
	برائے (;;) {
		ssize_t n;

		n = اقرأ(fd, buf, حجم(buf));
		اگر (n < 0)
			جاؤ error;
		اگر (!n)
			توڑو;

		copy = xrealloc(copy, (*لمبائی + n) * حجم(*copy));
		حافظہ_نقل(copy + *لمبائی, buf, n);
		*لمبائی += n;
	}
	close(fd);
	واپس copy;

error:
	close(fd);
	آزاد(copy);
	واپس تہی;
}

/*
 * A poor-man's version of "xargs -0". Basically parses a given block of
 * NUL-delimited data, within the given length and adds a pointer to each entry
 * to the array of pointers.
 */
ساکن صحیح parse_xargs(حرف *ڈیٹا, صحیح ڈیٹا_لمبائی, حرف ***output)
{
	صحیح عدد = 0;
	حرف *cur = ڈیٹا;

	اگر (!ڈیٹا || *output != تہی)
		واپس -1;

	جب (cur < ڈیٹا + ڈیٹا_لمبائی) {
		عدد++;
		*output = xrealloc(*output, (عدد + 1) * حجم(**output));
		(*output)[عدد - 1] = cur;
		cur += لڑی_لمبائی(cur) + 1;
	}
	(*output)[عدد] = تہی;
	واپس عدد;
}

/*
 * "Parse" out argv from /proc/self/cmdline.
 * This is necessary because we are running in a context where we don't have a
 * main() that we can just get the arguments from.
 */
ساکن صحیح fetchve(حرف ***argv)
{
	حرف *cmdline = تہی;
	size_t cmdline_سائز;

	cmdline = read_file("/proc/self/cmdline", &cmdline_سائز);
	اگر (!cmdline)
		جاؤ error;

	اگر (parse_xargs(cmdline, cmdline_سائز, argv) <= 0)
		جاؤ error;

	واپس 0;

error:
	آزاد(cmdline);
	واپس -EINVAL;
}

شمار {
	EFD_NONE = 0,
	EFD_MEMFD,
	EFD_FILE,
};

/*
 * This comes from <linux/fcntl.h>. We can't hard-code __O_TMPFILE because it
 * changes depending on the architecture. If we don't have O_TMPFILE we always
 * have the mkostemp(3) fallback.
 */
#اگر_غیر_تعریف_شدہ O_TMPFILE
#  اگر defined(__O_TMPFILE) && defined(O_DIRECTORY)
#    define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#  endif
#اگر_ختم

ساکن صحیح make_execfd(صحیح *fdtype)
{
	صحیح fd = -1;
	حرف سانچہ[PATH_MAX] = {0};
	حرف *سابقہ = getenv("_LIBCONTAINER_STATEDIR");

	اگر (!سابقہ || *سابقہ != '/')
		سابقہ = "/tmp";
	اگر (snprintf(سانچہ, حجم(سانچہ), "%s/crun.XXXXXX", سابقہ) < 0)
		واپس -1;

	/*
	 * Now try memfd, it's much nicer than actually creating a file in STATEDIR
	 * since it's easily detected thanks to sealing and also doesn't require
	 * assumptions about STATEDIR.
	 */
	*fdtype = EFD_MEMFD;
	fd = memfd_create(CRUN_MEMFD_COMMENT, MFD_CLOEXEC | MFD_ALLOW_SEALING);
	اگر (fd >= 0)
		واپس fd;
	اگر (errno != ENOSYS && errno != EINVAL)
		جاؤ error;

#اگر_تعریف_شدہ O_TMPFILE
	/*
	 * Try O_TMPFILE to avoid races where someone might snatch our file. Note
	 * that O_EXCL isn't actually a security measure here (since you can just
	 * fd re-open it and clear O_EXCL).
	 */
	*fdtype = EFD_FILE;
	fd = open(سابقہ, O_TMPFILE | O_EXCL | O_RDWR | O_CLOEXEC, 0700);
	اگر (fd >= 0) {
		ساخت stat statbuf = {};
		bool working_otmpfile = false;

		/*
		 * open(2) ignores unknown O_* flags -- yeah, I was surprised when I
		 * found this out too. As a result we can't check for EINVAL. However,
		 * if we get nlink != 0 (or EISDIR) then we know that this kernel
		 * doesn't support O_TMPFILE.
		 */
		اگر (fstat(fd, &statbuf) >= 0)
			working_otmpfile = (statbuf.st_nlink == 0);

		اگر (working_otmpfile)
			واپس fd;

		/* Pretend that we got EISDIR since O_TMPFILE failed. */
		close(fd);
		errno = EISDIR;
	}
	اگر (errno != EISDIR)
		جاؤ error;
#اگر_ختم /* defined(O_TMPFILE) */

	/*
	 * Our final option is to create a temporary file the old-school way, and
	 * then unlink it so that nothing else sees it by accident.
	 */
	*fdtype = EFD_FILE;
	fd = mkostemp(سانچہ, O_CLOEXEC);
	اگر (fd >= 0) {
		اگر (unlink(سانچہ) >= 0)
			واپس fd;
		close(fd);
	}

error:
	*fdtype = EFD_NONE;
	واپس -1;
}

ساکن صحیح seal_execfd(صحیح *fd, صحیح fdtype)
{
	انتخاب (fdtype) {
	حالت EFD_MEMFD:
		واپس fcntl(*fd, F_جوڑو_SEALS, CRUN_MEMFD_SEALS);
	حالت EFD_FILE: {
		/* Need to re-open our pseudo-memfd as an O_PATH to avoid execve(2) giving -ETXTBSY. */
		صحیح newfd;
		حرف fdpath[PATH_MAX] = {0};

		اگر (fchmod(*fd, 0100) < 0)
			واپس -1;

		اگر (snprintf(fdpath, حجم(fdpath), "/proc/self/fd/%d", *fd) < 0)
			واپس -1;

		newfd = open(fdpath, O_راستہ | O_CLOEXEC);
		اگر (newfd < 0)
			واپس -1;

		close(*fd);
		*fd = newfd;
		واپس 0;
	}
	طےشدہ:
	   توڑو;
	}
	واپس -1;
}

ساکن صحیح try_bindfd_mount_api(خالی)
{
	libcrun_error_t err;
	صحیح mountfd = get_bind_mount (-1, "/proc/self/exe", false, true, false, MS_PRIVATE, &err);
	اگر (mountfd < 0) {
		crun_error_release (&err);
		واپس -1;
	}
	واپس mountfd;
}

ساکن صحیح try_bindfd(خالی)
{
	mode_t نقاب;
	صحیح fd, ret = -1;
	حرف سانچہ[PATH_MAX] = {0};
	حرف *سابقہ = getenv("_LIBCONTAINER_STATEDIR");

	اگر (!سابقہ || *سابقہ != '/')
		سابقہ = "/tmp";
	اگر (snprintf(سانچہ, حجم(سانچہ), "%s/crun.XXXXXX", سابقہ) < 0)
		واپس ret;

	/*
	 * We need somewhere to mount it, mounting anything over /proc/self is a
	 * BAD idea on the host -- even if we do it temporarily.
	 */
	نقاب = umask(0700);
	fd = mkstemp(سانچہ);
	umask(نقاب);
	اگر (fd < 0)
		واپس ret;
	close(fd);

	/*
	 * For obvious reasons this won't work in rootless mode because we haven't
	 * created a userns+mntns -- but getting that to work will be a bit
	 * complicated and it's only worth doing if someone actually needs it.
	 */
	ret = -EPERM;
	اگر (mount("/proc/self/exe", سانچہ, "", MS_BIND, "") < 0)
		جاؤ برآمد;
	اگر (mount("", سانچہ, "", MS_REMOUNT | MS_BIND | MS_RDONLY, "") < 0)
		جاؤ برآمد_umount;


	/* Get read-only handle that we're sure can't be made read-write. */
	ret = open(سانچہ, O_راستہ | O_CLOEXEC);

برآمد_umount:
	/*
	 * Make sure the MNT_DETACH works, otherwise we could get remounted
	 * read-write and that would be quite bad (the fd would be made read-write
	 * too, invalidating the protection).
	 */
	اگر (umount2(سانچہ, MNT_DETACH) < 0) {
		اگر (ret >= 0)
			close(ret);
		ret = -ENOTRECOVERABLE;
	}

برآمد:
	/*
	 * We don't care about unlink errors, the worst that happens is that
	 * there's an empty file left around in STATEDIR.
	 */
	unlink(سانچہ);
	واپس ret;
}

ساکن ssize_t fd_مقصد_fd(صحیح outfd, صحیح infd)
{
	ssize_t کل = 0;
	حرف بفر[4096];

	برائے (;;) {
		ssize_t nread, nwritten = 0;

		nread = اقرأ(infd, بفر, حجم(بفر));
		اگر (nread < 0)
			واپس -1;
		اگر (!nread)
			توڑو;

		کرو {
			ssize_t n = اكتب(outfd, بفر + nwritten, nread - nwritten);
			اگر (n < 0)
				واپس -1;
			nwritten += n;
		} جب(nwritten < nread);

		کل += nwritten;
	}

	واپس کل;
}

ساکن صحیح clone_ثنائی(خالی)
{
	cleanup_close صحیح execfd = -1;
	ساخت stat statbuf = {};
	ssize_t بھیجا = 0;
	صحیح fdtype = EFD_NONE;

	/*
	 * Before we resort to copying, let's try creating an ro-binfd in one shot
	 * by getting a handle for a read-only bind-mount of the execfd.
	 */
	execfd = try_bindfd_mount_api();
	اگر (execfd >= 0) {
		/* Transfer ownership to caller */
		صحیح ret_execfd = execfd;
		execfd = -1;
		واپس ret_execfd;
	}
	execfd = try_bindfd();
	اگر (execfd >= 0) {
		/* Transfer ownership to caller */
		صحیح ret_execfd = execfd;
		execfd = -1;
		واپس ret_execfd;
	}

	/*
	 * Dammit, that didn't work -- time to copy the binary to a safe place we
	 * can seal the contents.
	 */
	execfd = make_execfd(&fdtype);
	اگر (execfd < 0 || fdtype == EFD_NONE)
		واپس -ENOTRECOVERABLE;

	{
		cleanup_close صحیح binfd = -1;
		binfd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
		اگر (binfd < 0)
			جاؤ error;

		اگر (fstat(binfd, &statbuf) < 0)
			جاؤ error;

		جب (بھیجا < statbuf.st_size) {
			صحیح n = sendfile(execfd, binfd, تہی, statbuf.st_size - بھیجا);
			اگر (n < 0) {
				/* sendfile can fail so we fallback to a dumb user-space copy. */
				n = fd_مقصد_fd(execfd, binfd);
				اگر (n < 0)
					جاؤ error;
			}
			بھیجا += n;
		}
	}
	اگر (بھیجا != statbuf.st_size)
		جاؤ error;

	اگر (seal_execfd(&execfd, fdtype) < 0)
		جاؤ error;

	{
		/* Transfer ownership to caller */
		صحیح ret_execfd = execfd;
		execfd = -1;
		واپس ret_execfd;
	}
error:
	واپس -EIO;
}

/* Get cheap access to the environment. */
خارجی حرف **environ;

صحیح ensure_cloned_ثنائی(خالی)
{
	cleanup_close صحیح execfd = -1;
	حرف **argv = تہی;

	/* Check that we're not self-cloned, and if we are then bail. */
	صحیح cloned = is_self_cloned();
	اگر (cloned > 0 || cloned == -ENOTRECOVERABLE)
		واپس cloned;

	اگر (fetchve(&argv) < 0)
		واپس -EINVAL;

	execfd = clone_ثنائی();
	اگر (execfd < 0)
		واپس -EIO;

	اگر (putenv(CLONED_ثنائی_ENV "=1"))
		جاؤ error;

	fexecve(execfd, argv, environ);
error:
	واپس -ENOEXEC;
}
