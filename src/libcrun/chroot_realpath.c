/*
 * chroot_realpath.c -- resolve pathname as if inside chroot
 * Based on realpath.c Copyright (C) 1993 Rick Sladkey <jrs@world.std.com>
 *
 * This program is free software; you can redistribute it and/or
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
 * License along with this library; see the file COPYING.LIB.  If not,
 * write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * 2005/09/12: Dan Howell (modified from realpath.c to emulate chroot)
 * 2019/04/19: Giuseppe Scrivano (on ENOENT return the part that could be resolved)
 * 2019/09/30: Giuseppe Scrivano (follow symlinks for the last component)
 * 2020/02/02: Giuseppe Scrivano (don't lose terminal '/' if an absolute symlink is found)
 */

#ifdef HAVE_CONFIG_H
#অন্তর্ভাব <config.h>
#endif

#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <strings.h>
#অন্তর্ভাব <limits.h>				/* for PATH_MAX */
#অন্তর্ভাব <sys/param.h>			/* for MAXPATHLEN */
#অন্তর্ভাব <errno.h>
#ifndef __set_errno
#define __set_errno(val) ((errno) = (val))
#endif

#অন্তর্ভাব <sys/stat.h>			/* for S_IFLNK */

#ifndef PATH_MAX
#define PATH_MAX _POSIX_PATH_MAX
#endif

#define MAX_READLINKS 32

/* Resolve PATH as if CHROOT was the root directory, storing the result in
   RESOLVED_PATH (which must be at least PATH_MAX bytes long).

   On success the returned path is prefixed by CHROOT, except in the trivial
   case where CHROOT is NULL, empty, or "/", where PATH is returned unchanged.
   A component of PATH that does not exist is not an error: the part that
   could be resolved is returned with the rest of PATH appended to it.

   On failure NULL is returned and errno is set to either ENAMETOOLONG or
   ELOOP, or to any error readlink(2) can report except ENOENT and EINVAL. */
বর্ণ *chroot_realpath(ধ্রুব বর্ণ *chroot, ধ্রুব বর্ণ *path, বর্ণ resolved_path[])
{
	বর্ণ copy_path[PATH_MAX];
	বর্ণ link_path[PATH_MAX];
	বর্ণ got_path[PATH_MAX];
	বর্ণ *got_path_মূল = got_path;
	বর্ণ *new_path = got_path;
	বর্ণ *max_path;
	পূর্ণাঙ্ক readlinks = 0;
	পূর্ণাঙ্ক n;
	পূর্ণাঙ্ক chroot_len;
	পূর্ণাঙ্ক last_component;

	/* Trivial case. */
	যদি (chroot == রিক্ত || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		যদি (strlen(path) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			প্রত্যাগম রিক্ত;
		}
		strcpy(resolved_path, path);
		প্রত্যাগম resolved_path;
	}

	chroot_len = strlen(chroot);

	যদি (chroot_len + strlen(path) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		প্রত্যাগম রিক্ত;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_path, path);
	path = copy_path;
	max_path = copy_path + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_path, chroot);
	new_path += chroot_len;
	got_path_মূল = new_path;
	*new_path++ = '/';

	/* Expand each slash-separated pathname component. */
	যাবত্ (*path != '\0') {
		/* Ignore stray "/". */
		যদি (*path == '/') {
			path++;
			অনুবর্ত;
		}
		যদি (*path == '.') {
			/* Ignore ".". */
			যদি (path[1] == '\0' || path[1] == '/') {
				path++;
				অনুবর্ত;
			}
			যদি (path[1] == '.') {
				যদি (path[2] == '\0' || path[2] == '/') {
					path += 2;
					/* Ignore ".." at root. */
					যদি (new_path == got_path_মূল + 1)
						অনুবর্ত;
					/* Handle ".." by backing up. */
					যাবত্ ((--new_path)[-1] != '/');
					অনুবর্ত;
				}
			}
		}
		/* Safely copy the next pathname component. */
		যাবত্ (*path != '\0' && *path != '/') {
			যদি (path > max_path || new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				প্রত্যাগম রিক্ত;
			}
			*new_path++ = *path++;
		}

		last_component = (*path == '\0');

#ifdef S_IFLNK
		/* Protect against infinite loops. */
		যদি (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			প্রত্যাগম রিক্ত;
		}
		/* See if latest pathname component is a symlink. */
		*new_path = '\0';
		n = readlink(got_path, link_path, PATH_MAX - 1);
		যদি (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			যদি (errno == ENOENT) {
				পূর্ণাঙ্ক ret = snprintf (resolved_path, PATH_MAX, "%s%s%s", got_path, path[0] == '/' || path[0] == '\0' ? "" : "/", path);
				যদি (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					প্রত্যাগম রিক্ত;
				}
				প্রত্যাগম resolved_path;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			যদি (errno != EINVAL)
				প্রত্যাগম রিক্ত;
		} অন্যথা {
			/* Note: readlink doesn't add the null byte. */
			link_path[n] = '\0';
			যদি (*link_path == '/') {
				/* Start over for an absolute symlink. */
				new_path = got_path_মূল;
				*new_path++ = '/';
			}
			অন্যথা {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				যাবত্ (*(--new_path) != '/');
				new_path++;
			}
			/* Safe sex check. */
			যদি (strlen(path) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				প্রত্যাগম রিক্ত;
			}
			/* Insert symlink contents into path. */
			strcat(link_path, path);
			strcpy(copy_path, link_path);
			path = copy_path;
		}
#endif							/* S_IFLNK */
		যদি (!last_component) {
			যদি (new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				প্রত্যাগম রিক্ত;
			}
			*new_path++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	যদি (new_path != got_path + 1 && new_path[-1] == '/')
		new_path--;
	/* Make sure it's null terminated. */
	*new_path = '\0';
	strcpy(resolved_path, got_path);
	প্রত্যাগম resolved_path;
}
