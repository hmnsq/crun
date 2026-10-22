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

#جيڪڏهن_وصف_ٿيل HAVE_CONFIG_H
#ضمن <config.h>
#جيڪڏهن_پورو

#ضمن <sys/types.h>
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <strings.h>
#ضمن <limits.h>				/* for PATH_MAX */
#ضمن <sys/param.h>			/* for MAXPATHLEN */
#ضمن <errno.h>
#جيڪڏهن_وصف_نه_ٿيل __set_errno
#وصف_ڪريو __set_errno(val) ((errno) = (val))
#جيڪڏهن_پورو

#ضمن <sys/stat.h>			/* for S_IFLNK */

#جيڪڏهن_وصف_نه_ٿيل PATH_MAX
#وصف_ڪريو PATH_MAX _POSIX_PATH_MAX
#جيڪڏهن_پورو

#وصف_ڪريو MAX_READLINKS 32

/* Resolve PATH as if CHROOT was the root directory, storing the result in
   RESOLVED_PATH (which must be at least PATH_MAX bytes long).

   On success the returned path is prefixed by CHROOT, except in the trivial
   case where CHROOT is NULL, empty, or "/", where PATH is returned unchanged.
   A component of PATH that does not exist is not an error: the part that
   could be resolved is returned with the rest of PATH appended to it.

   On failure NULL is returned and errno is set to either ENAMETOOLONG or
   ELOOP, or to any error readlink(2) can report except ENOENT and EINVAL. */
حرف *chroot_realpath(ثابت حرف *chroot, ثابت حرف *رستو, حرف resolved_رستو[])
{
	حرف copy_رستو[PATH_MAX];
	حرف link_رستو[PATH_MAX];
	حرف got_رستو[PATH_MAX];
	حرف *got_رستو_پاڙ = got_رستو;
	حرف *new_رستو = got_رستو;
	حرف *max_رستو;
	صحيح readlinks = 0;
	صحيح n;
	صحيح chroot_len;
	صحيح آخري_component;

	/* Trivial case. */
	إذا (chroot == عدم || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		إذا (لڙي_ڊيگهه(رستو) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			أرجع عدم;
		}
		strcpy(resolved_رستو, رستو);
		أرجع resolved_رستو;
	}

	chroot_len = لڙي_ڊيگهه(chroot);

	إذا (chroot_len + لڙي_ڊيگهه(رستو) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		أرجع عدم;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_رستو, رستو);
	رستو = copy_رستو;
	max_رستو = copy_رستو + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_رستو, chroot);
	new_رستو += chroot_len;
	got_رستو_پاڙ = new_رستو;
	*new_رستو++ = '/';

	/* Expand each slash-separated pathname component. */
	طالما (*رستو != '\0') {
		/* Ignore stray "/". */
		إذا (*رستو == '/') {
			رستو++;
			استمر;
		}
		إذا (*رستو == '.') {
			/* Ignore ".". */
			إذا (رستو[1] == '\0' || رستو[1] == '/') {
				رستو++;
				استمر;
			}
			إذا (رستو[1] == '.') {
				إذا (رستو[2] == '\0' || رستو[2] == '/') {
					رستو += 2;
					/* Ignore ".." at root. */
					إذا (new_رستو == got_رستو_پاڙ + 1)
						استمر;
					/* Handle ".." by backing up. */
					طالما ((--new_رستو)[-1] != '/');
					استمر;
				}
			}
		}
		/* Safely copy the next pathname component. */
		طالما (*رستو != '\0' && *رستو != '/') {
			إذا (رستو > max_رستو || new_رستو >= got_رستو + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			*new_رستو++ = *رستو++;
		}

		آخري_component = (*رستو == '\0');

#جيڪڏهن_وصف_ٿيل S_IFLNK
		/* Protect against infinite loops. */
		إذا (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			أرجع عدم;
		}
		/* See if latest pathname component is a symlink. */
		*new_رستو = '\0';
		n = readlink(got_رستو, link_رستو, PATH_MAX - 1);
		إذا (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			إذا (errno == ENOENT) {
				صحيح ret = snprintf (resolved_رستو, PATH_MAX, "%s%s%s", got_رستو, رستو[0] == '/' || رستو[0] == '\0' ? "" : "/", رستو);
				إذا (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					أرجع عدم;
				}
				أرجع resolved_رستو;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			إذا (errno != EINVAL)
				أرجع عدم;
		} وإلا {
			/* Note: readlink doesn't add the null byte. */
			link_رستو[n] = '\0';
			إذا (*link_رستو == '/') {
				/* Start over for an absolute symlink. */
				new_رستو = got_رستو_پاڙ;
				*new_رستو++ = '/';
			}
			وإلا {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				طالما (*(--new_رستو) != '/');
				new_رستو++;
			}
			/* Safe sex check. */
			إذا (لڙي_ڊيگهه(رستو) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			/* Insert symlink contents into path. */
			strcat(link_رستو, رستو);
			strcpy(copy_رستو, link_رستو);
			رستو = copy_رستو;
		}
#جيڪڏهن_پورو							/* S_IFLNK */
		إذا (!آخري_component) {
			إذا (new_رستو >= got_رستو + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			*new_رستو++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	إذا (new_رستو != got_رستو + 1 && new_رستو[-1] == '/')
		new_رستو--;
	/* Make sure it's null terminated. */
	*new_رستو = '\0';
	strcpy(resolved_رستو, got_رستو);
	أرجع resolved_رستو;
}
