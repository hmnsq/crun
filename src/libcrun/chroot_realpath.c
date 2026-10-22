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
#അന്തര്ഭാവ <config.h>
#endif

#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <strings.h>
#അന്തര്ഭാവ <limits.h>				/* for PATH_MAX */
#അന്തര്ഭാവ <sys/param.h>			/* for MAXPATHLEN */
#അന്തര്ഭാവ <errno.h>
#ifndef __set_errno
#define __set_errno(val) ((errno) = (val))
#endif

#അന്തര്ഭാവ <sys/stat.h>			/* for S_IFLNK */

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
അക്ഷരം *chroot_realpath(സ്ഥിരം അക്ഷരം *chroot, സ്ഥിരം അക്ഷരം *path, അക്ഷരം resolved_path[])
{
	അക്ഷരം copy_path[PATH_MAX];
	അക്ഷരം link_path[PATH_MAX];
	അക്ഷരം got_path[PATH_MAX];
	അക്ഷരം *got_path_വേര് = got_path;
	അക്ഷരം *new_path = got_path;
	അക്ഷരം *max_path;
	പൂർണ്ണസംഖ്യ readlinks = 0;
	പൂർണ്ണസംഖ്യ n;
	പൂർണ്ണസംഖ്യ chroot_len;
	പൂർണ്ണസംഖ്യ last_component;

	/* Trivial case. */
	എങ്കിൽ (chroot == ശൂന്യ || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		എങ്കിൽ (strlen(path) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			തിരികെ ശൂന്യ;
		}
		strcpy(resolved_path, path);
		തിരികെ resolved_path;
	}

	chroot_len = strlen(chroot);

	എങ്കിൽ (chroot_len + strlen(path) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		തിരികെ ശൂന്യ;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_path, path);
	path = copy_path;
	max_path = copy_path + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_path, chroot);
	new_path += chroot_len;
	got_path_വേര് = new_path;
	*new_path++ = '/';

	/* Expand each slash-separated pathname component. */
	ആകുമ്പോൾ (*path != '\0') {
		/* Ignore stray "/". */
		എങ്കിൽ (*path == '/') {
			path++;
			തുടരുക;
		}
		എങ്കിൽ (*path == '.') {
			/* Ignore ".". */
			എങ്കിൽ (path[1] == '\0' || path[1] == '/') {
				path++;
				തുടരുക;
			}
			എങ്കിൽ (path[1] == '.') {
				എങ്കിൽ (path[2] == '\0' || path[2] == '/') {
					path += 2;
					/* Ignore ".." at root. */
					എങ്കിൽ (new_path == got_path_വേര് + 1)
						തുടരുക;
					/* Handle ".." by backing up. */
					ആകുമ്പോൾ ((--new_path)[-1] != '/');
					തുടരുക;
				}
			}
		}
		/* Safely copy the next pathname component. */
		ആകുമ്പോൾ (*path != '\0' && *path != '/') {
			എങ്കിൽ (path > max_path || new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				തിരികെ ശൂന്യ;
			}
			*new_path++ = *path++;
		}

		last_component = (*path == '\0');

#ifdef S_IFLNK
		/* Protect against infinite loops. */
		എങ്കിൽ (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			തിരികെ ശൂന്യ;
		}
		/* See if latest pathname component is a symlink. */
		*new_path = '\0';
		n = readlink(got_path, link_path, PATH_MAX - 1);
		എങ്കിൽ (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			എങ്കിൽ (errno == ENOENT) {
				പൂർണ്ണസംഖ്യ ret = snprintf (resolved_path, PATH_MAX, "%s%s%s", got_path, path[0] == '/' || path[0] == '\0' ? "" : "/", path);
				എങ്കിൽ (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					തിരികെ ശൂന്യ;
				}
				തിരികെ resolved_path;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			എങ്കിൽ (errno != EINVAL)
				തിരികെ ശൂന്യ;
		} അല്ലെങ്കിൽ {
			/* Note: readlink doesn't add the null byte. */
			link_path[n] = '\0';
			എങ്കിൽ (*link_path == '/') {
				/* Start over for an absolute symlink. */
				new_path = got_path_വേര്;
				*new_path++ = '/';
			}
			അല്ലെങ്കിൽ {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				ആകുമ്പോൾ (*(--new_path) != '/');
				new_path++;
			}
			/* Safe sex check. */
			എങ്കിൽ (strlen(path) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				തിരികെ ശൂന്യ;
			}
			/* Insert symlink contents into path. */
			strcat(link_path, path);
			strcpy(copy_path, link_path);
			path = copy_path;
		}
#endif							/* S_IFLNK */
		എങ്കിൽ (!last_component) {
			എങ്കിൽ (new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				തിരികെ ശൂന്യ;
			}
			*new_path++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	എങ്കിൽ (new_path != got_path + 1 && new_path[-1] == '/')
		new_path--;
	/* Make sure it's null terminated. */
	*new_path = '\0';
	strcpy(resolved_path, got_path);
	തിരികെ resolved_path;
}
