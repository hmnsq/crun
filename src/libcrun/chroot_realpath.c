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
#ضمن <config.h>
#endif

#ضمن <sys/types.h>
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <strings.h>
#ضمن <limits.h>				/* for PATH_MAX */
#ضمن <sys/param.h>			/* for MAXPATHLEN */
#ضمن <errno.h>
#ifndef __set_errno
#define __set_errno(val) ((errno) = (val))
#endif

#ضمن <sys/stat.h>			/* for S_IFLNK */

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
حرف *chroot_realpath(ثابت حرف *chroot, ثابت حرف *path, حرف resolved_path[])
{
	حرف copy_path[PATH_MAX];
	حرف link_path[PATH_MAX];
	حرف got_path[PATH_MAX];
	حرف *got_path_ڕەگ = got_path;
	حرف *new_path = got_path;
	حرف *max_path;
	صحيح readlinks = 0;
	صحيح n;
	صحيح chroot_len;
	صحيح last_component;

	/* Trivial case. */
	إذا (chroot == عدم || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		إذا (strlen(path) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			أرجع عدم;
		}
		strcpy(resolved_path, path);
		أرجع resolved_path;
	}

	chroot_len = strlen(chroot);

	إذا (chroot_len + strlen(path) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		أرجع عدم;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_path, path);
	path = copy_path;
	max_path = copy_path + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_path, chroot);
	new_path += chroot_len;
	got_path_ڕەگ = new_path;
	*new_path++ = '/';

	/* Expand each slash-separated pathname component. */
	طالما (*path != '\0') {
		/* Ignore stray "/". */
		إذا (*path == '/') {
			path++;
			استمر;
		}
		إذا (*path == '.') {
			/* Ignore ".". */
			إذا (path[1] == '\0' || path[1] == '/') {
				path++;
				استمر;
			}
			إذا (path[1] == '.') {
				إذا (path[2] == '\0' || path[2] == '/') {
					path += 2;
					/* Ignore ".." at root. */
					إذا (new_path == got_path_ڕەگ + 1)
						استمر;
					/* Handle ".." by backing up. */
					طالما ((--new_path)[-1] != '/');
					استمر;
				}
			}
		}
		/* Safely copy the next pathname component. */
		طالما (*path != '\0' && *path != '/') {
			إذا (path > max_path || new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			*new_path++ = *path++;
		}

		last_component = (*path == '\0');

#ifdef S_IFLNK
		/* Protect against infinite loops. */
		إذا (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			أرجع عدم;
		}
		/* See if latest pathname component is a symlink. */
		*new_path = '\0';
		n = readlink(got_path, link_path, PATH_MAX - 1);
		إذا (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			إذا (errno == ENOENT) {
				صحيح ret = snprintf (resolved_path, PATH_MAX, "%s%s%s", got_path, path[0] == '/' || path[0] == '\0' ? "" : "/", path);
				إذا (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					أرجع عدم;
				}
				أرجع resolved_path;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			إذا (errno != EINVAL)
				أرجع عدم;
		} وإلا {
			/* Note: readlink doesn't add the null byte. */
			link_path[n] = '\0';
			إذا (*link_path == '/') {
				/* Start over for an absolute symlink. */
				new_path = got_path_ڕەگ;
				*new_path++ = '/';
			}
			وإلا {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				طالما (*(--new_path) != '/');
				new_path++;
			}
			/* Safe sex check. */
			إذا (strlen(path) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			/* Insert symlink contents into path. */
			strcat(link_path, path);
			strcpy(copy_path, link_path);
			path = copy_path;
		}
#endif							/* S_IFLNK */
		إذا (!last_component) {
			إذا (new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			*new_path++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	إذا (new_path != got_path + 1 && new_path[-1] == '/')
		new_path--;
	/* Make sure it's null terminated. */
	*new_path = '\0';
	strcpy(resolved_path, got_path);
	أرجع resolved_path;
}
