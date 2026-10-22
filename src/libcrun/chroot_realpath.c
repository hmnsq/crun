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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CONFIG_H
#ಅನ್ತರ್ಭಾವ <config.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <strings.h>
#ಅನ್ತರ್ಭಾವ <limits.h>				/* for PATH_MAX */
#ಅನ್ತರ್ಭಾವ <sys/param.h>			/* for MAXPATHLEN */
#ಅನ್ತರ್ಭಾವ <errno.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __set_errno
#ವ್ಯಾಖ್ಯಾನಿಸು __set_errno(val) ((errno) = (val))
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <sys/stat.h>			/* for S_IFLNK */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PATH_MAX
#ವ್ಯಾಖ್ಯಾನಿಸು PATH_MAX _POSIX_PATH_MAX
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು MAX_READLINKS 32

/* Resolve PATH as if CHROOT was the root directory, storing the result in
   RESOLVED_PATH (which must be at least PATH_MAX bytes long).

   On success the returned path is prefixed by CHROOT, except in the trivial
   case where CHROOT is NULL, empty, or "/", where PATH is returned unchanged.
   A component of PATH that does not exist is not an error: the part that
   could be resolved is returned with the rest of PATH appended to it.

   On failure NULL is returned and errno is set to either ENAMETOOLONG or
   ELOOP, or to any error readlink(2) can report except ENOENT and EINVAL. */
ಅಕ್ಷರ *chroot_realpath(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *chroot, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ resolved_ಮಾರ್ಗ[])
{
	ಅಕ್ಷರ copy_ಮಾರ್ಗ[PATH_MAX];
	ಅಕ್ಷರ link_ಮಾರ್ಗ[PATH_MAX];
	ಅಕ್ಷರ got_ಮಾರ್ಗ[PATH_MAX];
	ಅಕ್ಷರ *got_ಮಾರ್ಗ_ಬೇರು = got_ಮಾರ್ಗ;
	ಅಕ್ಷರ *new_ಮಾರ್ಗ = got_ಮಾರ್ಗ;
	ಅಕ್ಷರ *max_ಮಾರ್ಗ;
	ಪೂರ್ಣಾಂಕ readlinks = 0;
	ಪೂರ್ಣಾಂಕ n;
	ಪೂರ್ಣಾಂಕ chroot_len;
	ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_component;

	/* Trivial case. */
	ಆದರೆ (chroot == ಖಾಲಿ || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		}
		strcpy(resolved_ಮಾರ್ಗ, ಮಾರ್ಗ);
		ಹಿಂತಿರುಗಿ resolved_ಮಾರ್ಗ;
	}

	chroot_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(chroot);

	ಆದರೆ (chroot_len + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_ಮಾರ್ಗ, ಮಾರ್ಗ);
	ಮಾರ್ಗ = copy_ಮಾರ್ಗ;
	max_ಮಾರ್ಗ = copy_ಮಾರ್ಗ + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_ಮಾರ್ಗ, chroot);
	new_ಮಾರ್ಗ += chroot_len;
	got_ಮಾರ್ಗ_ಬೇರು = new_ಮಾರ್ಗ;
	*new_ಮಾರ್ಗ++ = '/';

	/* Expand each slash-separated pathname component. */
	ಆಗ (*ಮಾರ್ಗ != '\0') {
		/* Ignore stray "/". */
		ಆದರೆ (*ಮಾರ್ಗ == '/') {
			ಮಾರ್ಗ++;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (*ಮಾರ್ಗ == '.') {
			/* Ignore ".". */
			ಆದರೆ (ಮಾರ್ಗ[1] == '\0' || ಮಾರ್ಗ[1] == '/') {
				ಮಾರ್ಗ++;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (ಮಾರ್ಗ[1] == '.') {
				ಆದರೆ (ಮಾರ್ಗ[2] == '\0' || ಮಾರ್ಗ[2] == '/') {
					ಮಾರ್ಗ += 2;
					/* Ignore ".." at root. */
					ಆದರೆ (new_ಮಾರ್ಗ == got_ಮಾರ್ಗ_ಬೇರು + 1)
						ಮುಂದುವರಿ;
					/* Handle ".." by backing up. */
					ಆಗ ((--new_ಮಾರ್ಗ)[-1] != '/');
					ಮುಂದುವರಿ;
				}
			}
		}
		/* Safely copy the next pathname component. */
		ಆಗ (*ಮಾರ್ಗ != '\0' && *ಮಾರ್ಗ != '/') {
			ಆದರೆ (ಮಾರ್ಗ > max_ಮಾರ್ಗ || new_ಮಾರ್ಗ >= got_ಮಾರ್ಗ + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				ಹಿಂತಿರುಗಿ ಖಾಲಿ;
			}
			*new_ಮಾರ್ಗ++ = *ಮಾರ್ಗ++;
		}

		ಕೊನೆಯ_component = (*ಮಾರ್ಗ == '\0');

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ S_IFLNK
		/* Protect against infinite loops. */
		ಆದರೆ (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		}
		/* See if latest pathname component is a symlink. */
		*new_ಮಾರ್ಗ = '\0';
		n = readlink(got_ಮಾರ್ಗ, link_ಮಾರ್ಗ, PATH_MAX - 1);
		ಆದರೆ (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			ಆದರೆ (errno == ENOENT) {
				ಪೂರ್ಣಾಂಕ ret = snprintf (resolved_ಮಾರ್ಗ, PATH_MAX, "%s%s%s", got_ಮಾರ್ಗ, ಮಾರ್ಗ[0] == '/' || ಮಾರ್ಗ[0] == '\0' ? "" : "/", ಮಾರ್ಗ);
				ಆದರೆ (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					ಹಿಂತಿರುಗಿ ಖಾಲಿ;
				}
				ಹಿಂತಿರುಗಿ resolved_ಮಾರ್ಗ;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			ಆದರೆ (errno != EINVAL)
				ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* Note: readlink doesn't add the null byte. */
			link_ಮಾರ್ಗ[n] = '\0';
			ಆದರೆ (*link_ಮಾರ್ಗ == '/') {
				/* Start over for an absolute symlink. */
				new_ಮಾರ್ಗ = got_ಮಾರ್ಗ_ಬೇರು;
				*new_ಮಾರ್ಗ++ = '/';
			}
			ಇಲ್ಲದಿದ್ದರೆ {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				ಆಗ (*(--new_ಮಾರ್ಗ) != '/');
				new_ಮಾರ್ಗ++;
			}
			/* Safe sex check. */
			ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				ಹಿಂತಿರುಗಿ ಖಾಲಿ;
			}
			/* Insert symlink contents into path. */
			strcat(link_ಮಾರ್ಗ, ಮಾರ್ಗ);
			strcpy(copy_ಮಾರ್ಗ, link_ಮಾರ್ಗ);
			ಮಾರ್ಗ = copy_ಮಾರ್ಗ;
		}
#ಆದರೆ_ಅಂತ್ಯ							/* S_IFLNK */
		ಆದರೆ (!ಕೊನೆಯ_component) {
			ಆದರೆ (new_ಮಾರ್ಗ >= got_ಮಾರ್ಗ + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				ಹಿಂತಿರುಗಿ ಖಾಲಿ;
			}
			*new_ಮಾರ್ಗ++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	ಆದರೆ (new_ಮಾರ್ಗ != got_ಮಾರ್ಗ + 1 && new_ಮಾರ್ಗ[-1] == '/')
		new_ಮಾರ್ಗ--;
	/* Make sure it's null terminated. */
	*new_ಮಾರ್ಗ = '\0';
	strcpy(resolved_ಮಾರ್ಗ, got_ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ resolved_ಮಾರ್ಗ;
}
