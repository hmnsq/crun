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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CONFIG_H
#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <strings.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>				/* for PATH_MAX */
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/param.h>			/* for MAXPATHLEN */
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __set_errno
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __set_errno(val) ((errno) = (val))
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>			/* for S_IFLNK */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PATH_MAX
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PATH_MAX _POSIX_PATH_MAX
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_READLINKS 32

/* Resolve PATH as if CHROOT was the root directory, storing the result in
   RESOLVED_PATH (which must be at least PATH_MAX bytes long).

   On success the returned path is prefixed by CHROOT, except in the trivial
   case where CHROOT is NULL, empty, or "/", where PATH is returned unchanged.
   A component of PATH that does not exist is not an error: the part that
   could be resolved is returned with the rest of PATH appended to it.

   On failure NULL is returned and errno is set to either ENAMETOOLONG or
   ELOOP, or to any error readlink(2) can report except ENOENT and EINVAL. */
ਅੱਖਰ *chroot_realpath(ਅਚਲ ਅੱਖਰ *chroot, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ resolved_ਰਸਤਾ[])
{
	ਅੱਖਰ copy_ਰਸਤਾ[PATH_MAX];
	ਅੱਖਰ link_ਰਸਤਾ[PATH_MAX];
	ਅੱਖਰ got_ਰਸਤਾ[PATH_MAX];
	ਅੱਖਰ *got_ਰਸਤਾ_ਜੜ੍ਹ = got_ਰਸਤਾ;
	ਅੱਖਰ *new_ਰਸਤਾ = got_ਰਸਤਾ;
	ਅੱਖਰ *max_ਰਸਤਾ;
	ਪੂਰਨ readlinks = 0;
	ਪੂਰਨ n;
	ਪੂਰਨ chroot_len;
	ਪੂਰਨ ਆਖਰੀ_component;

	/* Trivial case. */
	ਜੇ (chroot == ਨਿਰਾ || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		ਜੇ (ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			ਵਾਪਸ ਨਿਰਾ;
		}
		strcpy(resolved_ਰਸਤਾ, ਰਸਤਾ);
		ਵਾਪਸ resolved_ਰਸਤਾ;
	}

	chroot_len = ਲੜੀ_ਲੰਬਾਈ(chroot);

	ਜੇ (chroot_len + ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		ਵਾਪਸ ਨਿਰਾ;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_ਰਸਤਾ, ਰਸਤਾ);
	ਰਸਤਾ = copy_ਰਸਤਾ;
	max_ਰਸਤਾ = copy_ਰਸਤਾ + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_ਰਸਤਾ, chroot);
	new_ਰਸਤਾ += chroot_len;
	got_ਰਸਤਾ_ਜੜ੍ਹ = new_ਰਸਤਾ;
	*new_ਰਸਤਾ++ = '/';

	/* Expand each slash-separated pathname component. */
	ਜਦੋਂ (*ਰਸਤਾ != '\0') {
		/* Ignore stray "/". */
		ਜੇ (*ਰਸਤਾ == '/') {
			ਰਸਤਾ++;
			ਜਾਰੀ;
		}
		ਜੇ (*ਰਸਤਾ == '.') {
			/* Ignore ".". */
			ਜੇ (ਰਸਤਾ[1] == '\0' || ਰਸਤਾ[1] == '/') {
				ਰਸਤਾ++;
				ਜਾਰੀ;
			}
			ਜੇ (ਰਸਤਾ[1] == '.') {
				ਜੇ (ਰਸਤਾ[2] == '\0' || ਰਸਤਾ[2] == '/') {
					ਰਸਤਾ += 2;
					/* Ignore ".." at root. */
					ਜੇ (new_ਰਸਤਾ == got_ਰਸਤਾ_ਜੜ੍ਹ + 1)
						ਜਾਰੀ;
					/* Handle ".." by backing up. */
					ਜਦੋਂ ((--new_ਰਸਤਾ)[-1] != '/');
					ਜਾਰੀ;
				}
			}
		}
		/* Safely copy the next pathname component. */
		ਜਦੋਂ (*ਰਸਤਾ != '\0' && *ਰਸਤਾ != '/') {
			ਜੇ (ਰਸਤਾ > max_ਰਸਤਾ || new_ਰਸਤਾ >= got_ਰਸਤਾ + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				ਵਾਪਸ ਨਿਰਾ;
			}
			*new_ਰਸਤਾ++ = *ਰਸਤਾ++;
		}

		ਆਖਰੀ_component = (*ਰਸਤਾ == '\0');

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ S_IFLNK
		/* Protect against infinite loops. */
		ਜੇ (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			ਵਾਪਸ ਨਿਰਾ;
		}
		/* See if latest pathname component is a symlink. */
		*new_ਰਸਤਾ = '\0';
		n = readlink(got_ਰਸਤਾ, link_ਰਸਤਾ, PATH_MAX - 1);
		ਜੇ (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			ਜੇ (errno == ENOENT) {
				ਪੂਰਨ ret = snprintf (resolved_ਰਸਤਾ, PATH_MAX, "%s%s%s", got_ਰਸਤਾ, ਰਸਤਾ[0] == '/' || ਰਸਤਾ[0] == '\0' ? "" : "/", ਰਸਤਾ);
				ਜੇ (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					ਵਾਪਸ ਨਿਰਾ;
				}
				ਵਾਪਸ resolved_ਰਸਤਾ;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			ਜੇ (errno != EINVAL)
				ਵਾਪਸ ਨਿਰਾ;
		} ਨਹੀਂਤਾਂ {
			/* Note: readlink doesn't add the null byte. */
			link_ਰਸਤਾ[n] = '\0';
			ਜੇ (*link_ਰਸਤਾ == '/') {
				/* Start over for an absolute symlink. */
				new_ਰਸਤਾ = got_ਰਸਤਾ_ਜੜ੍ਹ;
				*new_ਰਸਤਾ++ = '/';
			}
			ਨਹੀਂਤਾਂ {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				ਜਦੋਂ (*(--new_ਰਸਤਾ) != '/');
				new_ਰਸਤਾ++;
			}
			/* Safe sex check. */
			ਜੇ (ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				ਵਾਪਸ ਨਿਰਾ;
			}
			/* Insert symlink contents into path. */
			strcat(link_ਰਸਤਾ, ਰਸਤਾ);
			strcpy(copy_ਰਸਤਾ, link_ਰਸਤਾ);
			ਰਸਤਾ = copy_ਰਸਤਾ;
		}
#ਜੇ_ਅੰਤ							/* S_IFLNK */
		ਜੇ (!ਆਖਰੀ_component) {
			ਜੇ (new_ਰਸਤਾ >= got_ਰਸਤਾ + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				ਵਾਪਸ ਨਿਰਾ;
			}
			*new_ਰਸਤਾ++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	ਜੇ (new_ਰਸਤਾ != got_ਰਸਤਾ + 1 && new_ਰਸਤਾ[-1] == '/')
		new_ਰਸਤਾ--;
	/* Make sure it's null terminated. */
	*new_ਰਸਤਾ = '\0';
	strcpy(resolved_ਰਸਤਾ, got_ਰਸਤਾ);
	ਵਾਪਸ resolved_ਰਸਤਾ;
}
