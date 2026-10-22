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

#यदि_परिभाषितम् HAVE_CONFIG_H
#अन्तर्भाव <config.h>
#यदि_अन्तः

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <strings.h>
#अन्तर्भाव <limits.h>				/* for PATH_MAX */
#अन्तर्भाव <sys/param.h>			/* for MAXPATHLEN */
#अन्तर्भाव <errno.h>
#यदि_अपरिभाषितम् __set_errno
#परिभाषय __set_errno(val) ((errno) = (val))
#यदि_अन्तः

#अन्तर्भाव <sys/stat.h>			/* for S_IFLNK */

#यदि_अपरिभाषितम् PATH_MAX
#परिभाषय PATH_MAX _POSIX_PATH_MAX
#यदि_अन्तः

#परिभाषय MAX_READLINKS 32

/* Resolve PATH as if CHROOT was the root directory, storing the result in
   RESOLVED_PATH (which must be at least PATH_MAX bytes long).

   On success the returned path is prefixed by CHROOT, except in the trivial
   case where CHROOT is NULL, empty, or "/", where PATH is returned unchanged.
   A component of PATH that does not exist is not an error: the part that
   could be resolved is returned with the rest of PATH appended to it.

   On failure NULL is returned and errno is set to either ENAMETOOLONG or
   ELOOP, or to any error readlink(2) can report except ENOENT and EINVAL. */
वर्ण *chroot_realpath(ध्रुव वर्ण *chroot, ध्रुव वर्ण *मार्गः, वर्ण resolved_मार्गः[])
{
	वर्ण copy_मार्गः[PATH_MAX];
	वर्ण link_मार्गः[PATH_MAX];
	वर्ण got_मार्गः[PATH_MAX];
	वर्ण *got_मार्गः_मूलम् = got_मार्गः;
	वर्ण *new_मार्गः = got_मार्गः;
	वर्ण *max_मार्गः;
	पूर्णाङ्क readlinks = 0;
	पूर्णाङ्क n;
	पूर्णाङ्क chroot_len;
	पूर्णाङ्क चरमः_component;

	/* Trivial case. */
	यदि (chroot == रिक्त || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		यदि (वाक्यदैर्घ्यम्(मार्गः) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			प्रत्यागम रिक्त;
		}
		वाक्यं_प्रतिलिख(resolved_मार्गः, मार्गः);
		प्रत्यागम resolved_मार्गः;
	}

	chroot_len = वाक्यदैर्घ्यम्(chroot);

	यदि (chroot_len + वाक्यदैर्घ्यम्(मार्गः) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		प्रत्यागम रिक्त;
	}

	/* Make a copy of the source path since we may need to modify it. */
	वाक्यं_प्रतिलिख(copy_मार्गः, मार्गः);
	मार्गः = copy_मार्गः;
	max_मार्गः = copy_मार्गः + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	वाक्यं_प्रतिलिख(new_मार्गः, chroot);
	new_मार्गः += chroot_len;
	got_मार्गः_मूलम् = new_मार्गः;
	*new_मार्गः++ = '/';

	/* Expand each slash-separated pathname component. */
	यावत् (*मार्गः != '\0') {
		/* Ignore stray "/". */
		यदि (*मार्गः == '/') {
			मार्गः++;
			अनुवर्त;
		}
		यदि (*मार्गः == '.') {
			/* Ignore ".". */
			यदि (मार्गः[1] == '\0' || मार्गः[1] == '/') {
				मार्गः++;
				अनुवर्त;
			}
			यदि (मार्गः[1] == '.') {
				यदि (मार्गः[2] == '\0' || मार्गः[2] == '/') {
					मार्गः += 2;
					/* Ignore ".." at root. */
					यदि (new_मार्गः == got_मार्गः_मूलम् + 1)
						अनुवर्त;
					/* Handle ".." by backing up. */
					यावत् ((--new_मार्गः)[-1] != '/');
					अनुवर्त;
				}
			}
		}
		/* Safely copy the next pathname component. */
		यावत् (*मार्गः != '\0' && *मार्गः != '/') {
			यदि (मार्गः > max_मार्गः || new_मार्गः >= got_मार्गः + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				प्रत्यागम रिक्त;
			}
			*new_मार्गः++ = *मार्गः++;
		}

		चरमः_component = (*मार्गः == '\0');

#यदि_परिभाषितम् S_IFLNK
		/* Protect against infinite loops. */
		यदि (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			प्रत्यागम रिक्त;
		}
		/* See if latest pathname component is a symlink. */
		*new_मार्गः = '\0';
		n = readlink(got_मार्गः, link_मार्गः, PATH_MAX - 1);
		यदि (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			यदि (errno == ENOENT) {
				पूर्णाङ्क ret = snprintf (resolved_मार्गः, PATH_MAX, "%s%s%s", got_मार्गः, मार्गः[0] == '/' || मार्गः[0] == '\0' ? "" : "/", मार्गः);
				यदि (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					प्रत्यागम रिक्त;
				}
				प्रत्यागम resolved_मार्गः;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			यदि (errno != EINVAL)
				प्रत्यागम रिक्त;
		} अन्यथा {
			/* Note: readlink doesn't add the null byte. */
			link_मार्गः[n] = '\0';
			यदि (*link_मार्गः == '/') {
				/* Start over for an absolute symlink. */
				new_मार्गः = got_मार्गः_मूलम्;
				*new_मार्गः++ = '/';
			}
			अन्यथा {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				यावत् (*(--new_मार्गः) != '/');
				new_मार्गः++;
			}
			/* Safe sex check. */
			यदि (वाक्यदैर्घ्यम्(मार्गः) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				प्रत्यागम रिक्त;
			}
			/* Insert symlink contents into path. */
			वाक्यं_योजय(link_मार्गः, मार्गः);
			वाक्यं_प्रतिलिख(copy_मार्गः, link_मार्गः);
			मार्गः = copy_मार्गः;
		}
#यदि_अन्तः							/* S_IFLNK */
		यदि (!चरमः_component) {
			यदि (new_मार्गः >= got_मार्गः + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				प्रत्यागम रिक्त;
			}
			*new_मार्गः++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	यदि (new_मार्गः != got_मार्गः + 1 && new_मार्गः[-1] == '/')
		new_मार्गः--;
	/* Make sure it's null terminated. */
	*new_मार्गः = '\0';
	वाक्यं_प्रतिलिख(resolved_मार्गः, got_मार्गः);
	प्रत्यागम resolved_मार्गः;
}
