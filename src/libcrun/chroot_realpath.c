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
#அந்தர்பாவ <config.h>
#endif

#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <strings.h>
#அந்தர்பாவ <limits.h>				/* for PATH_MAX */
#அந்தர்பாவ <sys/param.h>			/* for MAXPATHLEN */
#அந்தர்பாவ <errno.h>
#ifndef __set_errno
#define __set_errno(val) ((errno) = (val))
#endif

#அந்தர்பாவ <sys/stat.h>			/* for S_IFLNK */

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
எழுத்து *chroot_realpath(மாறிலி எழுத்து *chroot, மாறிலி எழுத்து *path, எழுத்து resolved_path[])
{
	எழுத்து copy_path[PATH_MAX];
	எழுத்து link_path[PATH_MAX];
	எழுத்து got_path[PATH_MAX];
	எழுத்து *got_path_வேர் = got_path;
	எழுத்து *new_path = got_path;
	எழுத்து *max_path;
	முழு readlinks = 0;
	முழு n;
	முழு chroot_len;
	முழு last_component;

	/* Trivial case. */
	எனில் (chroot == வெற்று || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		எனில் (strlen(path) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			திரும்பு வெற்று;
		}
		strcpy(resolved_path, path);
		திரும்பு resolved_path;
	}

	chroot_len = strlen(chroot);

	எனில் (chroot_len + strlen(path) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		திரும்பு வெற்று;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_path, path);
	path = copy_path;
	max_path = copy_path + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_path, chroot);
	new_path += chroot_len;
	got_path_வேர் = new_path;
	*new_path++ = '/';

	/* Expand each slash-separated pathname component. */
	போது (*path != '\0') {
		/* Ignore stray "/". */
		எனில் (*path == '/') {
			path++;
			தொடர்;
		}
		எனில் (*path == '.') {
			/* Ignore ".". */
			எனில் (path[1] == '\0' || path[1] == '/') {
				path++;
				தொடர்;
			}
			எனில் (path[1] == '.') {
				எனில் (path[2] == '\0' || path[2] == '/') {
					path += 2;
					/* Ignore ".." at root. */
					எனில் (new_path == got_path_வேர் + 1)
						தொடர்;
					/* Handle ".." by backing up. */
					போது ((--new_path)[-1] != '/');
					தொடர்;
				}
			}
		}
		/* Safely copy the next pathname component. */
		போது (*path != '\0' && *path != '/') {
			எனில் (path > max_path || new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				திரும்பு வெற்று;
			}
			*new_path++ = *path++;
		}

		last_component = (*path == '\0');

#ifdef S_IFLNK
		/* Protect against infinite loops. */
		எனில் (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			திரும்பு வெற்று;
		}
		/* See if latest pathname component is a symlink. */
		*new_path = '\0';
		n = readlink(got_path, link_path, PATH_MAX - 1);
		எனில் (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			எனில் (errno == ENOENT) {
				முழு ret = snprintf (resolved_path, PATH_MAX, "%s%s%s", got_path, path[0] == '/' || path[0] == '\0' ? "" : "/", path);
				எனில் (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					திரும்பு வெற்று;
				}
				திரும்பு resolved_path;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			எனில் (errno != EINVAL)
				திரும்பு வெற்று;
		} இல்லையெனில் {
			/* Note: readlink doesn't add the null byte. */
			link_path[n] = '\0';
			எனில் (*link_path == '/') {
				/* Start over for an absolute symlink. */
				new_path = got_path_வேர்;
				*new_path++ = '/';
			}
			இல்லையெனில் {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				போது (*(--new_path) != '/');
				new_path++;
			}
			/* Safe sex check. */
			எனில் (strlen(path) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				திரும்பு வெற்று;
			}
			/* Insert symlink contents into path. */
			strcat(link_path, path);
			strcpy(copy_path, link_path);
			path = copy_path;
		}
#endif							/* S_IFLNK */
		எனில் (!last_component) {
			எனில் (new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				திரும்பு வெற்று;
			}
			*new_path++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	எனில் (new_path != got_path + 1 && new_path[-1] == '/')
		new_path--;
	/* Make sure it's null terminated. */
	*new_path = '\0';
	strcpy(resolved_path, got_path);
	திரும்பு resolved_path;
}
