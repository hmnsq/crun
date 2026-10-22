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
#अन्तर्भाव <config.h>
#endif

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <strings.h>
#अन्तर्भाव <limits.h>				/* for PATH_MAX */
#अन्तर्भाव <sys/param.h>			/* for MAXPATHLEN */
#अन्तर्भाव <errno.h>
#ifndef __set_errno
#define __set_errno(val) ((errno) = (val))
#endif

#अन्तर्भाव <sys/stat.h>			/* for S_IFLNK */

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
वर्ण *chroot_realpath(ध्रुव वर्ण *chroot, ध्रुव वर्ण *path, वर्ण resolved_path[])
{
	वर्ण copy_path[PATH_MAX];
	वर्ण link_path[PATH_MAX];
	वर्ण got_path[PATH_MAX];
	वर्ण *got_path_मूलम् = got_path;
	वर्ण *new_path = got_path;
	वर्ण *max_path;
	पूर्णाङ्क readlinks = 0;
	पूर्णाङ्क n;
	पूर्णाङ्क chroot_len;
	पूर्णाङ्क last_component;

	/* Trivial case. */
	जइ (chroot == रिक्त || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		जइ (strlen(path) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			प्रत्यागम रिक्त;
		}
		strcpy(resolved_path, path);
		प्रत्यागम resolved_path;
	}

	chroot_len = strlen(chroot);

	जइ (chroot_len + strlen(path) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		प्रत्यागम रिक्त;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_path, path);
	path = copy_path;
	max_path = copy_path + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_path, chroot);
	new_path += chroot_len;
	got_path_मूलम् = new_path;
	*new_path++ = '/';

	/* Expand each slash-separated pathname component. */
	जाव (*path != '\0') {
		/* Ignore stray "/". */
		जइ (*path == '/') {
			path++;
			अनुवर्त;
		}
		जइ (*path == '.') {
			/* Ignore ".". */
			जइ (path[1] == '\0' || path[1] == '/') {
				path++;
				अनुवर्त;
			}
			जइ (path[1] == '.') {
				जइ (path[2] == '\0' || path[2] == '/') {
					path += 2;
					/* Ignore ".." at root. */
					जइ (new_path == got_path_मूलम् + 1)
						अनुवर्त;
					/* Handle ".." by backing up. */
					जाव ((--new_path)[-1] != '/');
					अनुवर्त;
				}
			}
		}
		/* Safely copy the next pathname component. */
		जाव (*path != '\0' && *path != '/') {
			जइ (path > max_path || new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				प्रत्यागम रिक्त;
			}
			*new_path++ = *path++;
		}

		last_component = (*path == '\0');

#ifdef S_IFLNK
		/* Protect against infinite loops. */
		जइ (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			प्रत्यागम रिक्त;
		}
		/* See if latest pathname component is a symlink. */
		*new_path = '\0';
		n = readlink(got_path, link_path, PATH_MAX - 1);
		जइ (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			जइ (errno == ENOENT) {
				पूर्णाङ्क ret = snprintf (resolved_path, PATH_MAX, "%s%s%s", got_path, path[0] == '/' || path[0] == '\0' ? "" : "/", path);
				जइ (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					प्रत्यागम रिक्त;
				}
				प्रत्यागम resolved_path;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			जइ (errno != EINVAL)
				प्रत्यागम रिक्त;
		} अण्णहा {
			/* Note: readlink doesn't add the null byte. */
			link_path[n] = '\0';
			जइ (*link_path == '/') {
				/* Start over for an absolute symlink. */
				new_path = got_path_मूलम्;
				*new_path++ = '/';
			}
			अण्णहा {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				जाव (*(--new_path) != '/');
				new_path++;
			}
			/* Safe sex check. */
			जइ (strlen(path) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				प्रत्यागम रिक्त;
			}
			/* Insert symlink contents into path. */
			strcat(link_path, path);
			strcpy(copy_path, link_path);
			path = copy_path;
		}
#endif							/* S_IFLNK */
		जइ (!last_component) {
			जइ (new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				प्रत्यागम रिक्त;
			}
			*new_path++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	जइ (new_path != got_path + 1 && new_path[-1] == '/')
		new_path--;
	/* Make sure it's null terminated. */
	*new_path = '\0';
	strcpy(resolved_path, got_path);
	प्रत्यागम resolved_path;
}
