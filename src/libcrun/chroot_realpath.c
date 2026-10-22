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

#اگر_تعریف_شدہ HAVE_CONFIG_H
#شامل <config.h>
#اگر_ختم

#شامل <sys/types.h>
#شامل <unistd.h>
#شامل <stdio.h>
#شامل <string.h>
#شامل <strings.h>
#شامل <limits.h>				/* for PATH_MAX */
#شامل <sys/param.h>			/* for MAXPATHLEN */
#شامل <errno.h>
#اگر_غیر_تعریف_شدہ __set_errno
#تعریف_کرو __set_errno(val) ((errno) = (val))
#اگر_ختم

#شامل <sys/stat.h>			/* for S_IFLNK */

#اگر_غیر_تعریف_شدہ PATH_MAX
#تعریف_کرو PATH_MAX _POSIX_PATH_MAX
#اگر_ختم

#تعریف_کرو MAX_READLINKS 32

/* Resolve PATH as if CHROOT was the root directory, storing the result in
   RESOLVED_PATH (which must be at least PATH_MAX bytes long).

   On success the returned path is prefixed by CHROOT, except in the trivial
   case where CHROOT is NULL, empty, or "/", where PATH is returned unchanged.
   A component of PATH that does not exist is not an error: the part that
   could be resolved is returned with the rest of PATH appended to it.

   On failure NULL is returned and errno is set to either ENAMETOOLONG or
   ELOOP, or to any error readlink(2) can report except ENOENT and EINVAL. */
حرف *chroot_realpath(ثابت حرف *chroot, ثابت حرف *راستہ, حرف resolved_راستہ[])
{
	حرف copy_راستہ[PATH_MAX];
	حرف link_راستہ[PATH_MAX];
	حرف got_راستہ[PATH_MAX];
	حرف *got_راستہ_جڑ = got_راستہ;
	حرف *new_راستہ = got_راستہ;
	حرف *max_راستہ;
	صحیح readlinks = 0;
	صحیح n;
	صحیح chroot_len;
	صحیح آخری_component;

	/* Trivial case. */
	اگر (chroot == تہی || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		اگر (لڑی_لمبائی(راستہ) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			واپس تہی;
		}
		strcpy(resolved_راستہ, راستہ);
		واپس resolved_راستہ;
	}

	chroot_len = لڑی_لمبائی(chroot);

	اگر (chroot_len + لڑی_لمبائی(راستہ) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		واپس تہی;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_راستہ, راستہ);
	راستہ = copy_راستہ;
	max_راستہ = copy_راستہ + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_راستہ, chroot);
	new_راستہ += chroot_len;
	got_راستہ_جڑ = new_راستہ;
	*new_راستہ++ = '/';

	/* Expand each slash-separated pathname component. */
	جب (*راستہ != '\0') {
		/* Ignore stray "/". */
		اگر (*راستہ == '/') {
			راستہ++;
			جاری;
		}
		اگر (*راستہ == '.') {
			/* Ignore ".". */
			اگر (راستہ[1] == '\0' || راستہ[1] == '/') {
				راستہ++;
				جاری;
			}
			اگر (راستہ[1] == '.') {
				اگر (راستہ[2] == '\0' || راستہ[2] == '/') {
					راستہ += 2;
					/* Ignore ".." at root. */
					اگر (new_راستہ == got_راستہ_جڑ + 1)
						جاری;
					/* Handle ".." by backing up. */
					جب ((--new_راستہ)[-1] != '/');
					جاری;
				}
			}
		}
		/* Safely copy the next pathname component. */
		جب (*راستہ != '\0' && *راستہ != '/') {
			اگر (راستہ > max_راستہ || new_راستہ >= got_راستہ + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				واپس تہی;
			}
			*new_راستہ++ = *راستہ++;
		}

		آخری_component = (*راستہ == '\0');

#اگر_تعریف_شدہ S_IFLNK
		/* Protect against infinite loops. */
		اگر (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			واپس تہی;
		}
		/* See if latest pathname component is a symlink. */
		*new_راستہ = '\0';
		n = readlink(got_راستہ, link_راستہ, PATH_MAX - 1);
		اگر (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			اگر (errno == ENOENT) {
				صحیح ret = snprintf (resolved_راستہ, PATH_MAX, "%s%s%s", got_راستہ, راستہ[0] == '/' || راستہ[0] == '\0' ? "" : "/", راستہ);
				اگر (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					واپس تہی;
				}
				واپس resolved_راستہ;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			اگر (errno != EINVAL)
				واپس تہی;
		} ورنہ {
			/* Note: readlink doesn't add the null byte. */
			link_راستہ[n] = '\0';
			اگر (*link_راستہ == '/') {
				/* Start over for an absolute symlink. */
				new_راستہ = got_راستہ_جڑ;
				*new_راستہ++ = '/';
			}
			ورنہ {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				جب (*(--new_راستہ) != '/');
				new_راستہ++;
			}
			/* Safe sex check. */
			اگر (لڑی_لمبائی(راستہ) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				واپس تہی;
			}
			/* Insert symlink contents into path. */
			strcat(link_راستہ, راستہ);
			strcpy(copy_راستہ, link_راستہ);
			راستہ = copy_راستہ;
		}
#اگر_ختم							/* S_IFLNK */
		اگر (!آخری_component) {
			اگر (new_راستہ >= got_راستہ + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				واپس تہی;
			}
			*new_راستہ++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	اگر (new_راستہ != got_راستہ + 1 && new_راستہ[-1] == '/')
		new_راستہ--;
	/* Make sure it's null terminated. */
	*new_راستہ = '\0';
	strcpy(resolved_راستہ, got_راستہ);
	واپس resolved_راستہ;
}
