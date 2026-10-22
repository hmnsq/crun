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

#که_تعریف_شوی HAVE_CONFIG_H
#ضمن <config.h>
#د_که_پای

#ضمن <sys/types.h>
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <strings.h>
#ضمن <limits.h>				/* for PATH_MAX */
#ضمن <sys/param.h>			/* for MAXPATHLEN */
#ضمن <errno.h>
#که_نه_تعریف_شوی __set_errno
#تعریف_کړه __set_errno(val) ((errno) = (val))
#د_که_پای

#ضمن <sys/stat.h>			/* for S_IFLNK */

#که_نه_تعریف_شوی PATH_MAX
#تعریف_کړه PATH_MAX _POSIX_PATH_MAX
#د_که_پای

#تعریف_کړه MAX_READLINKS 32

/* Resolve PATH as if CHROOT was the root directory, storing the result in
   RESOLVED_PATH (which must be at least PATH_MAX bytes long).

   On success the returned path is prefixed by CHROOT, except in the trivial
   case where CHROOT is NULL, empty, or "/", where PATH is returned unchanged.
   A component of PATH that does not exist is not an error: the part that
   could be resolved is returned with the rest of PATH appended to it.

   On failure NULL is returned and errno is set to either ENAMETOOLONG or
   ELOOP, or to any error readlink(2) can report except ENOENT and EINVAL. */
حرف *chroot_realpath(ثابت حرف *chroot, ثابت حرف *لار, حرف resolved_لار[])
{
	حرف copy_لار[PATH_MAX];
	حرف link_لار[PATH_MAX];
	حرف got_لار[PATH_MAX];
	حرف *got_لار_ریښه = got_لار;
	حرف *new_لار = got_لار;
	حرف *max_لار;
	صحيح readlinks = 0;
	صحيح n;
	صحيح chroot_len;
	صحيح وروستی_component;

	/* Trivial case. */
	إذا (chroot == عدم || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		إذا (مزي_اوږدوالی(لار) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			أرجع عدم;
		}
		strcpy(resolved_لار, لار);
		أرجع resolved_لار;
	}

	chroot_len = مزي_اوږدوالی(chroot);

	إذا (chroot_len + مزي_اوږدوالی(لار) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		أرجع عدم;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_لار, لار);
	لار = copy_لار;
	max_لار = copy_لار + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_لار, chroot);
	new_لار += chroot_len;
	got_لار_ریښه = new_لار;
	*new_لار++ = '/';

	/* Expand each slash-separated pathname component. */
	طالما (*لار != '\0') {
		/* Ignore stray "/". */
		إذا (*لار == '/') {
			لار++;
			استمر;
		}
		إذا (*لار == '.') {
			/* Ignore ".". */
			إذا (لار[1] == '\0' || لار[1] == '/') {
				لار++;
				استمر;
			}
			إذا (لار[1] == '.') {
				إذا (لار[2] == '\0' || لار[2] == '/') {
					لار += 2;
					/* Ignore ".." at root. */
					إذا (new_لار == got_لار_ریښه + 1)
						استمر;
					/* Handle ".." by backing up. */
					طالما ((--new_لار)[-1] != '/');
					استمر;
				}
			}
		}
		/* Safely copy the next pathname component. */
		طالما (*لار != '\0' && *لار != '/') {
			إذا (لار > max_لار || new_لار >= got_لار + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			*new_لار++ = *لار++;
		}

		وروستی_component = (*لار == '\0');

#که_تعریف_شوی S_IFLNK
		/* Protect against infinite loops. */
		إذا (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			أرجع عدم;
		}
		/* See if latest pathname component is a symlink. */
		*new_لار = '\0';
		n = readlink(got_لار, link_لار, PATH_MAX - 1);
		إذا (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			إذا (errno == ENOENT) {
				صحيح ret = snprintf (resolved_لار, PATH_MAX, "%s%s%s", got_لار, لار[0] == '/' || لار[0] == '\0' ? "" : "/", لار);
				إذا (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					أرجع عدم;
				}
				أرجع resolved_لار;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			إذا (errno != EINVAL)
				أرجع عدم;
		} وإلا {
			/* Note: readlink doesn't add the null byte. */
			link_لار[n] = '\0';
			إذا (*link_لار == '/') {
				/* Start over for an absolute symlink. */
				new_لار = got_لار_ریښه;
				*new_لار++ = '/';
			}
			وإلا {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				طالما (*(--new_لار) != '/');
				new_لار++;
			}
			/* Safe sex check. */
			إذا (مزي_اوږدوالی(لار) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			/* Insert symlink contents into path. */
			strcat(link_لار, لار);
			strcpy(copy_لار, link_لار);
			لار = copy_لار;
		}
#د_که_پای							/* S_IFLNK */
		إذا (!وروستی_component) {
			إذا (new_لار >= got_لار + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			*new_لار++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	إذا (new_لار != got_لار + 1 && new_لار[-1] == '/')
		new_لار--;
	/* Make sure it's null terminated. */
	*new_لار = '\0';
	strcpy(resolved_لار, got_لار);
	أرجع resolved_لار;
}
