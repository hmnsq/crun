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

#నిర్వచితమైతే HAVE_CONFIG_H
#అన్తర్భావ <config.h>
#అయితే_ముగింపు

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <strings.h>
#అన్తర్భావ <limits.h>				/* for PATH_MAX */
#అన్తర్భావ <sys/param.h>			/* for MAXPATHLEN */
#అన్తర్భావ <errno.h>
#నిర్వచితం_కాకపోతే __set_errno
#నిర్వచించు __set_errno(val) ((errno) = (val))
#అయితే_ముగింపు

#అన్తర్భావ <sys/stat.h>			/* for S_IFLNK */

#నిర్వచితం_కాకపోతే PATH_MAX
#నిర్వచించు PATH_MAX _POSIX_PATH_MAX
#అయితే_ముగింపు

#నిర్వచించు MAX_READLINKS 32

/* Resolve PATH as if CHROOT was the root directory, storing the result in
   RESOLVED_PATH (which must be at least PATH_MAX bytes long).

   On success the returned path is prefixed by CHROOT, except in the trivial
   case where CHROOT is NULL, empty, or "/", where PATH is returned unchanged.
   A component of PATH that does not exist is not an error: the part that
   could be resolved is returned with the rest of PATH appended to it.

   On failure NULL is returned and errno is set to either ENAMETOOLONG or
   ELOOP, or to any error readlink(2) can report except ENOENT and EINVAL. */
అక్షరం *chroot_realpath(స్థిరరాశి అక్షరం *chroot, స్థిరరాశి అక్షరం *మార్గం, అక్షరం resolved_మార్గం[])
{
	అక్షరం copy_మార్గం[PATH_MAX];
	అక్షరం link_మార్గం[PATH_MAX];
	అక్షరం got_మార్గం[PATH_MAX];
	అక్షరం *got_మార్గం_మూలం = got_మార్గం;
	అక్షరం *new_మార్గం = got_మార్గం;
	అక్షరం *max_మార్గం;
	పూర్ణాంకం readlinks = 0;
	పూర్ణాంకం n;
	పూర్ణాంకం chroot_len;
	పూర్ణాంకం చివరి_component;

	/* Trivial case. */
	ఒకవేళ (chroot == ఖాళీ || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		ఒకవేళ (అక్షరశ్రేణి_పొడవు(మార్గం) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			తిరిగి ఖాళీ;
		}
		strcpy(resolved_మార్గం, మార్గం);
		తిరిగి resolved_మార్గం;
	}

	chroot_len = అక్షరశ్రేణి_పొడవు(chroot);

	ఒకవేళ (chroot_len + అక్షరశ్రేణి_పొడవు(మార్గం) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		తిరిగి ఖాళీ;
	}

	/* Make a copy of the source path since we may need to modify it. */
	strcpy(copy_మార్గం, మార్గం);
	మార్గం = copy_మార్గం;
	max_మార్గం = copy_మార్గం + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	strcpy(new_మార్గం, chroot);
	new_మార్గం += chroot_len;
	got_మార్గం_మూలం = new_మార్గం;
	*new_మార్గం++ = '/';

	/* Expand each slash-separated pathname component. */
	అయితే (*మార్గం != '\0') {
		/* Ignore stray "/". */
		ఒకవేళ (*మార్గం == '/') {
			మార్గం++;
			కొనసాగు;
		}
		ఒకవేళ (*మార్గం == '.') {
			/* Ignore ".". */
			ఒకవేళ (మార్గం[1] == '\0' || మార్గం[1] == '/') {
				మార్గం++;
				కొనసాగు;
			}
			ఒకవేళ (మార్గం[1] == '.') {
				ఒకవేళ (మార్గం[2] == '\0' || మార్గం[2] == '/') {
					మార్గం += 2;
					/* Ignore ".." at root. */
					ఒకవేళ (new_మార్గం == got_మార్గం_మూలం + 1)
						కొనసాగు;
					/* Handle ".." by backing up. */
					అయితే ((--new_మార్గం)[-1] != '/');
					కొనసాగు;
				}
			}
		}
		/* Safely copy the next pathname component. */
		అయితే (*మార్గం != '\0' && *మార్గం != '/') {
			ఒకవేళ (మార్గం > max_మార్గం || new_మార్గం >= got_మార్గం + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				తిరిగి ఖాళీ;
			}
			*new_మార్గం++ = *మార్గం++;
		}

		చివరి_component = (*మార్గం == '\0');

#నిర్వచితమైతే S_IFLNK
		/* Protect against infinite loops. */
		ఒకవేళ (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			తిరిగి ఖాళీ;
		}
		/* See if latest pathname component is a symlink. */
		*new_మార్గం = '\0';
		n = readlink(got_మార్గం, link_మార్గం, PATH_MAX - 1);
		ఒకవేళ (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			ఒకవేళ (errno == ENOENT) {
				పూర్ణాంకం ret = snprintf (resolved_మార్గం, PATH_MAX, "%s%s%s", got_మార్గం, మార్గం[0] == '/' || మార్గం[0] == '\0' ? "" : "/", మార్గం);
				ఒకవేళ (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					తిరిగి ఖాళీ;
				}
				తిరిగి resolved_మార్గం;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			ఒకవేళ (errno != EINVAL)
				తిరిగి ఖాళీ;
		} లేకపోతే {
			/* Note: readlink doesn't add the null byte. */
			link_మార్గం[n] = '\0';
			ఒకవేళ (*link_మార్గం == '/') {
				/* Start over for an absolute symlink. */
				new_మార్గం = got_మార్గం_మూలం;
				*new_మార్గం++ = '/';
			}
			లేకపోతే {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				అయితే (*(--new_మార్గం) != '/');
				new_మార్గం++;
			}
			/* Safe sex check. */
			ఒకవేళ (అక్షరశ్రేణి_పొడవు(మార్గం) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				తిరిగి ఖాళీ;
			}
			/* Insert symlink contents into path. */
			strcat(link_మార్గం, మార్గం);
			strcpy(copy_మార్గం, link_మార్గం);
			మార్గం = copy_మార్గం;
		}
#అయితే_ముగింపు							/* S_IFLNK */
		ఒకవేళ (!చివరి_component) {
			ఒకవేళ (new_మార్గం >= got_మార్గం + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				తిరిగి ఖాళీ;
			}
			*new_మార్గం++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	ఒకవేళ (new_మార్గం != got_మార్గం + 1 && new_మార్గం[-1] == '/')
		new_మార్గం--;
	/* Make sure it's null terminated. */
	*new_మార్గం = '\0';
	strcpy(resolved_మార్గం, got_మార్గం);
	తిరిగి resolved_మార్గం;
}
