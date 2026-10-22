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

#جے_تعریف_شدہ HAVE_CONFIG_H
#ضمن <config.h>
#جے_انت

#ضمن <sys/types.h>
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <strings.h>
#ضمن <limits.h>				/* for PATH_MAX */
#ضمن <sys/param.h>			/* for MAXPATHLEN */
#ضمن <errno.h>
#جے_بنا_تعریف __set_errno
#تعریف_کرو __set_errno(val) ((errno) = (val))
#جے_انت

#ضمن <sys/stat.h>			/* for S_IFLNK */

#جے_بنا_تعریف PATH_MAX
#تعریف_کرو PATH_MAX _POSIX_PATH_MAX
#جے_انت

#تعریف_کرو MAX_READLINKS 32

/* Resolve PATH as if CHROOT was the root directory, storing the result in
   RESOLVED_PATH (which must be at least PATH_MAX bytes long).

   On success the returned path is prefixed by CHROOT, except in the trivial
   case where CHROOT is NULL, empty, or "/", where PATH is returned unchanged.
   A component of PATH that does not exist is not an error: the part that
   could be resolved is returned with the rest of PATH appended to it.

   On failure NULL is returned and errno is set to either ENAMETOOLONG or
   ELOOP, or to any error readlink(2) can report except ENOENT and EINVAL. */
حرف *chroot_realpath(ثابت حرف *chroot, ثابت حرف *رستہ, حرف resolved_رستہ[])
{
	حرف copy_رستہ[PATH_MAX];
	حرف link_رستہ[PATH_MAX];
	حرف got_رستہ[PATH_MAX];
	حرف *got_رستہ_جڑ = got_رستہ;
	حرف *new_رستہ = got_رستہ;
	حرف *max_رستہ;
	صحيح readlinks = 0;
	صحيح n;
	صحيح chroot_len;
	صحيح آخری_component;

	/* Trivial case. */
	إذا (chroot == عدم || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		إذا (لڑی_لمبائی(رستہ) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			أرجع عدم;
		}
		strcpy(resolved_رستہ, رستہ);
		أرجع resolved_رستہ;
	}

	chroot_len = لڑی_لمبائی(chroot);

	إذا (chroot_len + لڑی_لمبائی(رستہ) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		أرجع عدم;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_رستہ, رستہ);
	رستہ = copy_رستہ;
	max_رستہ = copy_رستہ + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_رستہ, chroot);
	new_رستہ += chroot_len;
	got_رستہ_جڑ = new_رستہ;
	*new_رستہ++ = '/';

	/* Expand each slash-separated pathname component. */
	طالما (*رستہ != '\0') {
		/* Ignore stray "/". */
		إذا (*رستہ == '/') {
			رستہ++;
			استمر;
		}
		إذا (*رستہ == '.') {
			/* Ignore ".". */
			إذا (رستہ[1] == '\0' || رستہ[1] == '/') {
				رستہ++;
				استمر;
			}
			إذا (رستہ[1] == '.') {
				إذا (رستہ[2] == '\0' || رستہ[2] == '/') {
					رستہ += 2;
					/* Ignore ".." at root. */
					إذا (new_رستہ == got_رستہ_جڑ + 1)
						استمر;
					/* Handle ".." by backing up. */
					طالما ((--new_رستہ)[-1] != '/');
					استمر;
				}
			}
		}
		/* Safely copy the next pathname component. */
		طالما (*رستہ != '\0' && *رستہ != '/') {
			إذا (رستہ > max_رستہ || new_رستہ >= got_رستہ + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			*new_رستہ++ = *رستہ++;
		}

		آخری_component = (*رستہ == '\0');

#جے_تعریف_شدہ S_IFLNK
		/* Protect against infinite loops. */
		إذا (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			أرجع عدم;
		}
		/* See if latest pathname component is a symlink. */
		*new_رستہ = '\0';
		n = readlink(got_رستہ, link_رستہ, PATH_MAX - 1);
		إذا (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			إذا (errno == ENOENT) {
				صحيح ret = snprintf (resolved_رستہ, PATH_MAX, "%s%s%s", got_رستہ, رستہ[0] == '/' || رستہ[0] == '\0' ? "" : "/", رستہ);
				إذا (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					أرجع عدم;
				}
				أرجع resolved_رستہ;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			إذا (errno != EINVAL)
				أرجع عدم;
		} وإلا {
			/* Note: readlink doesn't add the null byte. */
			link_رستہ[n] = '\0';
			إذا (*link_رستہ == '/') {
				/* Start over for an absolute symlink. */
				new_رستہ = got_رستہ_جڑ;
				*new_رستہ++ = '/';
			}
			وإلا {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				طالما (*(--new_رستہ) != '/');
				new_رستہ++;
			}
			/* Safe sex check. */
			إذا (لڑی_لمبائی(رستہ) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			/* Insert symlink contents into path. */
			strcat(link_رستہ, رستہ);
			strcpy(copy_رستہ, link_رستہ);
			رستہ = copy_رستہ;
		}
#جے_انت							/* S_IFLNK */
		إذا (!آخری_component) {
			إذا (new_رستہ >= got_رستہ + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				أرجع عدم;
			}
			*new_رستہ++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	إذا (new_رستہ != got_رستہ + 1 && new_رستہ[-1] == '/')
		new_رستہ--;
	/* Make sure it's null terminated. */
	*new_رستہ = '\0';
	strcpy(resolved_رستہ, got_رستہ);
	أرجع resolved_رستہ;
}
