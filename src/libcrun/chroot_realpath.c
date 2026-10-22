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

#अगर_घोषित HAVE_CONFIG_H
#समावेश <config.h>
#पूर्ण_अगर

#समावेश <sys/types.h>
#समावेश <unistd.h>
#समावेश <stdio.h>
#समावेश <string.h>
#समावेश <strings.h>
#समावेश <limits.h>				/* for PATH_MAX */
#समावेश <sys/param.h>			/* for MAXPATHLEN */
#समावेश <errno.h>
#अगर_अघोषित __set_errno
#घोषणा __set_errno(val) ((त्रुटि_सं) = (val))
#पूर्ण_अगर

#समावेश <sys/stat.h>			/* for S_IFLNK */

#अगर_अघोषित PATH_MAX
#घोषणा PATH_MAX _POSIX_PATH_MAX
#पूर्ण_अगर

#घोषणा MAX_READLINKS 32

/* Resolve PATH as if CHROOT was the root directory, storing the result in
   RESOLVED_PATH (which must be at least PATH_MAX bytes long).

   On success the returned path is prefixed by CHROOT, except in the trivial
   case where CHROOT is NULL, empty, or "/", where PATH is returned unchanged.
   A component of PATH that does not exist is not an error: the part that
   could be resolved is returned with the rest of PATH appended to it.

   On failure NULL is returned and errno is set to either ENAMETOOLONG or
   ELOOP, or to any error readlink(2) can report except ENOENT and EINVAL. */
अक्षर *chroot_realpath(स्थिर अक्षर *chroot, स्थिर अक्षर *path, अक्षर resolved_path[])
{
	अक्षर copy_path[PATH_MAX];
	अक्षर link_path[PATH_MAX];
	अक्षर got_path[PATH_MAX];
	अक्षर *got_path_मूल = got_path;
	अक्षर *new_path = got_path;
	अक्षर *max_path;
	पूर्णांक readlinks = 0;
	पूर्णांक n;
	पूर्णांक chroot_len;
	पूर्णांक last_component;

	/* Trivial case. */
	अगर (chroot == शून्य || *chroot == '\0' ||
	    (*chroot == '/' && chroot[1] == '\0')) {
		अगर (म_माप(path) >= PATH_MAX) {
			__set_errno(ENAMETOOLONG);
			वापस शून्य;
		}
		म_नकल(resolved_path, path);
		वापस resolved_path;
	}

	chroot_len = म_माप(chroot);

	अगर (chroot_len + म_माप(path) >= PATH_MAX - 3) {
		__set_errno(ENAMETOOLONG);
		वापस शून्य;
	}

	/* Make a copy of the source path since we may need to modify it. */
	म_नकल(copy_path, path);
	path = copy_path;
	max_path = copy_path + PATH_MAX - chroot_len - 3;

	/* Start with the chroot path.  It is copied verbatim so that the
	   result is always prefixed by CHROOT and callers can strip it. */
	म_नकल(new_path, chroot);
	new_path += chroot_len;
	got_path_मूल = new_path;
	*new_path++ = '/';

	/* Expand each slash-separated pathname component. */
	जबतक (*path != '\0') {
		/* Ignore stray "/". */
		अगर (*path == '/') {
			path++;
			जारी;
		}
		अगर (*path == '.') {
			/* Ignore ".". */
			अगर (path[1] == '\0' || path[1] == '/') {
				path++;
				जारी;
			}
			अगर (path[1] == '.') {
				अगर (path[2] == '\0' || path[2] == '/') {
					path += 2;
					/* Ignore ".." at root. */
					अगर (new_path == got_path_मूल + 1)
						जारी;
					/* Handle ".." by backing up. */
					जबतक ((--new_path)[-1] != '/');
					जारी;
				}
			}
		}
		/* Safely copy the next pathname component. */
		जबतक (*path != '\0' && *path != '/') {
			अगर (path > max_path || new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				वापस शून्य;
			}
			*new_path++ = *path++;
		}

		last_component = (*path == '\0');

#अगर_घोषित S_IFLNK
		/* Protect against infinite loops. */
		अगर (readlinks++ > MAX_READLINKS) {
			__set_errno(ELOOP);
			वापस शून्य;
		}
		/* See if latest pathname component is a symlink. */
		*new_path = '\0';
		n = readlink(got_path, link_path, PATH_MAX - 1);
		अगर (n < 0) {
			/* If a component doesn't exist, then return what we could translate. */
			अगर (त्रुटि_सं == ENOENT) {
				पूर्णांक ret = snprintf (resolved_path, PATH_MAX, "%s%s%s", got_path, path[0] == '/' || path[0] == '\0' ? "" : "/", path);
				अगर (ret >= PATH_MAX) {
					__set_errno(ENAMETOOLONG);
					वापस शून्य;
				}
				वापस resolved_path;
			}
			/* EINVAL means the file exists but isn't a symlink. */
			अगर (त्रुटि_सं != EINVAL)
				वापस शून्य;
		} अन्यथा {
			/* Note: readlink doesn't add the null byte. */
			link_path[n] = '\0';
			अगर (*link_path == '/') {
				/* Start over for an absolute symlink. */
				new_path = got_path_मूल;
				*new_path++ = '/';
			}
			अन्यथा {
				/* Otherwise back up over this component, keeping the
				   separator, so that the expanded symlink is not
				   concatenated to the parent directory name. */
				जबतक (*(--new_path) != '/');
				new_path++;
			}
			/* Safe sex check. */
			अगर (म_माप(path) + n >= PATH_MAX - 2) {
				__set_errno(ENAMETOOLONG);
				वापस शून्य;
			}
			/* Insert symlink contents into path. */
			म_जोड़ो(link_path, path);
			म_नकल(copy_path, link_path);
			path = copy_path;
		}
#पूर्ण_अगर							/* S_IFLNK */
		अगर (!last_component) {
			अगर (new_path >= got_path + PATH_MAX - 1) {
				__set_errno(ENAMETOOLONG);
				वापस शून्य;
			}
			*new_path++ = '/';
		}
	}
	/* Delete trailing slash but don't whomp a lone slash. */
	अगर (new_path != got_path + 1 && new_path[-1] == '/')
		new_path--;
	/* Make sure it's null terminated. */
	*new_path = '\0';
	म_नकल(resolved_path, got_path);
	वापस resolved_path;
}
