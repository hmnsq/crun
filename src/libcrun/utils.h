/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ UTILS_H
#ವ್ಯಾಖ್ಯಾನಿಸು UTILS_H

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ <dirent.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ <sys/wait.h>
#ಅನ್ತರ್ಭಾವ "container.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY
#  define ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY(ಅಭಿವ್ಯಕ್ತಿ_2)      \
    (__extension__ ({                         \
      ದೀರ್ಘ ಪೂರ್ಣಾಂಕ __result;                      \
      ಮಾಡು                                      \
        __result = (ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) (ಅಭಿವ್ಯಕ್ತಿ_2);   \
      ಆಗ (__result < 0 && errno == EINTR); \
      __result;                               \
    }))
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_close __attribute__ ((cleanup (cleanup_closep)))
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_close_vec __attribute__ ((cleanup (cleanup_close_vecp)))
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_close_map __attribute__ ((cleanup (cleanup_close_mapp)))
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_dir __attribute__ ((cleanup (cleanup_dirp)))
#ವ್ಯಾಖ್ಯಾನಿಸು arg_ಬಳಸದ __attribute__ ((ಬಳಸದ))
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_pid __attribute__ ((cleanup (cleanup_pidp)))
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_mmap __attribute__ ((cleanup (cleanup_mmapp)))

#ವ್ಯಾಖ್ಯಾನಿಸು LIKELY(x) __builtin_expect ((x), 1)
#ವ್ಯಾಖ್ಯಾನಿಸು UNLIKELY(x) __builtin_expect ((x), 0)

#ವ್ಯಾಖ್ಯಾನಿಸು WRITE_FILE_DEFAULT_FLAGS (O_CLOEXEC | O_CREAT | O_TRUNC | O_WRONLY)

__attribute__ ((ಸ್ಮರಣೆ_ಕೊಡು)) ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ *
xmalloc (size_t ಗಾತ್ರ_2)
{
  ಶೂನ್ಯ *res = ಸ್ಮರಣೆ_ಕೊಡು (ಗಾತ್ರ_2);
  ಆದರೆ (UNLIKELY (res == ಖಾಲಿ))
    OOM ();
  ಹಿಂತಿರುಗಿ res;
}

__attribute__ ((ಸ್ಮರಣೆ_ಕೊಡು)) ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ *
xmalloc0 (size_t ಗಾತ್ರ_2)
{
  ಶೂನ್ಯ *res = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು (1, ಗಾತ್ರ_2);
  ಆದರೆ (UNLIKELY (res == ಖಾಲಿ))
    OOM ();
  ಹಿಂತಿರುಗಿ res;
}

__attribute__ ((ಸ್ಮರಣೆ_ಕೊಡು)) ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ *
xrealloc (ಶೂನ್ಯ *ptr, size_t ಗಾತ್ರ_2)
{
  ಶೂನ್ಯ *res = realloc (ptr, ಗಾತ್ರ_2);
  ಆದರೆ (UNLIKELY (res == ಖಾಲಿ))
    OOM ();
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_freep (ಶೂನ್ಯ *p)
{
  ಶೂನ್ಯ **pp = (ಶೂನ್ಯ **) p;
  ಬಿಡುಗಡೆ (*pp);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  ಆದರೆ (file)
    (ಶೂನ್ಯ) fclose (file);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_closep (ಶೂನ್ಯ *p)
{
  ಪೂರ್ಣಾಂಕ *pp = (ಪೂರ್ಣಾಂಕ *) p;
  ಆದರೆ (*pp >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (*pp));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_pidp (ಶೂನ್ಯ *p)
{
  pid_t *pp = (pid_t *) p;
  ಆದರೆ (*pp > 0)
    {
      ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (kill (*pp, SIGKILL));
      ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (waitpid (*pp, ಖಾಲಿ, 0));
    }
}

ರಚನೆ libcrun_mmap_s
{
  ಶೂನ್ಯ *addr;
  size_t ಉದ್ದಳತೆ;
};

ಪೂರ್ಣಾಂಕ libcrun_mmap (ರಚನೆ libcrun_mmap_s **ret, ಶೂನ್ಯ *addr, size_t ಉದ್ದಳತೆ,
                  ಪೂರ್ಣಾಂಕ prot, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ fd, off_t ಸ್ಥಾನಾಂತರ,
                  libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_munmap (ರಚನೆ libcrun_mmap_s *mmap, libcrun_error_t *err);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_mmapp (ಶೂನ್ಯ *p)
{
  ಪೂರ್ಣಾಂಕ ret;
  libcrun_error_t tmp_err = ಖಾಲಿ;
  ರಚನೆ libcrun_mmap_s **mm;

  mm = (ರಚನೆ libcrun_mmap_s **) p;
  ಆದರೆ (*mm == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ret = libcrun_munmap (*mm, &tmp_err);
  ಆದರೆ (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

ರಚನೆ libcrun_fd_map
{
  size_t nfds;
  ಪೂರ್ಣಾಂಕ fds[];
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  ರಚನೆ libcrun_fd_map *ret;
  size_t i;

  ret = (ರಚನೆ libcrun_fd_map *) xmalloc (ಗಾತ್ರ (*ret) + ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ) * len);
  ret->nfds = len;
  ಗಾಗಿ (i = 0; i < len; i++)
    ret->fds[i] = -1;

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_close_mapp (ರಚನೆ libcrun_fd_map **p)
{
  ರಚನೆ libcrun_fd_map *m = *p;
  size_t i;

  ಆದರೆ (m == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ಗಾಗಿ (i = 0; i < m->nfds; i++)
    ಆದರೆ (m->fds[i] >= 0)
      ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (m->fds[i]));

  ಬಿಡುಗಡೆ (m);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_close_vecp (ಪೂರ್ಣಾಂಕ **p)
{
  ಪೂರ್ಣಾಂಕ *pp = *p;
  ಪೂರ್ಣಾಂಕ i;

  ಆದರೆ (pp == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ಗಾಗಿ (i = 0; pp[i] >= 0; i++)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (pp[i]));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  ಆದರೆ (dir)
    closedir (dir);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
close_and_reset (ಪೂರ್ಣಾಂಕ *fd)
{
  ಪೂರ್ಣಾಂಕ ret = 0;
  ಆದರೆ (*fd >= 0)
    {
      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (*fd));
      ಆದರೆ (LIKELY (ret == 0))
        *fd = -1;
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ *
xstrdup (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
  ಅಕ್ಷರ *ret;
  ಆದರೆ (str == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ret = strdup (str);
  ಆದರೆ (ret == ಖಾಲಿ)
    OOM ();

  ಹಿಂತಿರುಗಿ ret;
}

ಶೂನ್ಯ consume_trailing_slashes (ಅಕ್ಷರ *ಮಾರ್ಗ);

#ವ್ಯಾಖ್ಯಾನಿಸು consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    ಆಗ (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ಮಾರ್ಗ_is_slash_dev (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
  ಮಾರ್ಗ = consume_slashes (ಮಾರ್ಗ);

  ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (ಮಾರ್ಗ, "dev", 3))
    ಹಿಂತಿರುಗಿ false;

  ಮಾರ್ಗ += 3;

  /* Check there are only '/' left.  */
  ಗಾಗಿ (; *ಮಾರ್ಗ; ಮಾರ್ಗ++)
    ಆದರೆ (*ಮಾರ್ಗ != '/')
      ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ true;
}

ಪೂರ್ಣಾಂಕ xasprintf (ಅಕ್ಷರ **str, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...) __attribute__ ((format (ಬರೆ, 2, 3)));

ಪೂರ್ಣಾಂಕ crun_ಮಾರ್ಗ_exists (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ write_file_at_with_flags (ಪೂರ್ಣಾಂಕ dirfd, ಪೂರ್ಣಾಂಕ flags, mode_t ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, size_t len, libcrun_error_t *err);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
write_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, size_t len, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, ದತ್ತಾಂಶ, len, err);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
write_file_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, size_t len, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, ದತ್ತಾಂಶ, len, err);
}

ಪೂರ್ಣಾಂಕ crun_ensure_directory (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, bool nofollow, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ crun_ensure_directory_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, bool nofollow, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ crun_ಸುರಕ್ಷಿತ_create_and_open_ref_at (bool dir, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dirpath, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ crun_ಸುರಕ್ಷಿತ_ensure_directory_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dirpath, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
                                   libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ crun_ಸುರಕ್ಷಿತ_ensure_file_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dirpath, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
                              libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ crun_dir_p (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool nofollow, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ crun_dir_p_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool nofollow, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ detach_process ();

ಪೂರ್ಣಾಂಕ create_file_if_missing_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, mode_t ಬಹುಲಕ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ check_running_ಒಳ_user_namespace (libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ ಗಣ_selinux_label (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label, bool now, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_selinux_mount_label (ಅಕ್ಷರ **ret, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *context_type, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ ಗಣ_apparmor_profile (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ read_all_fd_with_ಗಾತ್ರ_hint (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *description, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *len, size_t hint, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ read_all_fd_with_max_ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *description, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *len, size_t max_ಗಾತ್ರ, libcrun_error_t *err);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
read_all_fd (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *description, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *len, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ read_all_fd_with_ಗಾತ್ರ_hint (fd, description, ಹೊರಹೋಗು, len, 0, err);
}

ಪೂರ್ಣಾಂಕ get_realpath_ಗಮ್ಯ_file (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ_name, ಅಕ್ಷರ **absolute_ಮಾರ್ಗ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ read_all_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *len, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ read_all_file_at (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *len, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ open_unix_domain_client_socket (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ dgram, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ open_unix_domain_socket (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ dgram, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ send_fd_ಗಮ್ಯ_socket (ಪೂರ್ಣಾಂಕ server, ಪೂರ್ಣಾಂಕ fd, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ send_fd_ಗಮ್ಯ_socket_with_payload (ಪೂರ್ಣಾಂಕ server, ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *payload, size_t payload_len, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ create_socket_pair (ಪೂರ್ಣಾಂಕ *pair, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ receive_fd_from_socket (ಪೂರ್ಣಾಂಕ from, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ receive_fd_from_socket_with_payload (ಪೂರ್ಣಾಂಕ from, ಅಕ್ಷರ *payload, size_t payload_len, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ create_signalfd (sigset_t *ಮುಖವಾಡ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ epoll_helper (ಪೂರ್ಣಾಂಕ *ಒಳ_fds, ಪೂರ್ಣಾಂಕ *ಒಳ_levelfds, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_fds, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_levelfds, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ copy_from_fd_ಗಮ್ಯ_fd (ಪೂರ್ಣಾಂಕ src, ಪೂರ್ಣಾಂಕ dst, ಪೂರ್ಣಾಂಕ consume, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ ಸತತ_process (ಅಕ್ಷರ **ವಾದಗಳು, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ format_default_id_mapping (ಅಕ್ಷರ **ಹೊರಹೋಗು, uid_t container_id, uid_t host_uid, uid_t host_id, ಪೂರ್ಣಾಂಕ is_uid, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ ಸತತ_process_with_stdin_timeout_envp (ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ **ವಾದಗಳು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cwd, ಪೂರ್ಣಾಂಕ timeout, ಅಕ್ಷರ **envp,
                                         ಅಕ್ಷರ *stdin, size_t stdin_len, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_fd, ಪೂರ್ಣಾಂಕ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ mark_or_close_fds_ge_than (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ n, bool close_now, libcrun_error_t *err);

ಶೂನ್ಯ get_current_timestamp (ಅಕ್ಷರ *ಹೊರಹೋಗು, size_t len);

ಪೂರ್ಣಾಂಕ ಗಣ_blocking_fd (ಪೂರ್ಣಾಂಕ fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ parse_json_file (json_object **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *jsondata, ರಚನೆ parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
has_ಪೂರ್ವಪ್ರತ್ಯಯ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len;

  ಆದರೆ (! str || ! ಪೂರ್ವಪ್ರತ್ಯಯ)
    ಹಿಂತಿರುಗಿ 0;

  ಪೂರ್ವಪ್ರತ್ಯಯ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಪೂರ್ವಪ್ರತ್ಯಯ);
  ಹಿಂತಿರುಗಿ ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (str) >= ಪೂರ್ವಪ್ರತ್ಯಯ_len && memcmp (str, ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
has_dir_ಪೂರ್ವಪ್ರತ್ಯಯ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len;

  ಆದರೆ (! str || ! ಪೂರ್ವಪ್ರತ್ಯಯ)
    ಹಿಂತಿರುಗಿ 0;

  ಪೂರ್ವಪ್ರತ್ಯಯ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಪೂರ್ವಪ್ರತ್ಯಯ);
  ಹಿಂತಿರುಗಿ ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (str) > ಪೂರ್ವಪ್ರತ್ಯಯ_len && memcmp (str, ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ_len) == 0 && str[ಪೂರ್ವಪ್ರತ್ಯಯ_len] == '/';
}

ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_executable (ಅಕ್ಷರ **exec_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *executable_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cwd, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ copy_recursive_fd_ಗಮ್ಯ_fd (ಪೂರ್ಣಾಂಕ srcfd, ಪೂರ್ಣಾಂಕ destfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *srcname, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *destname, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ ಗಣ_home_env (uid_t uid);

ಪೂರ್ಣಾಂಕ libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_initialize_apparmor (libcrun_error_t *err);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕಂಡುಹಿಡಿ_annotation (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);

ಪೂರ್ಣಾಂಕ get_file_type_at (ಪೂರ್ಣಾಂಕ dirfd, mode_t *ಬಹುಲಕ, bool nofollow, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ);

ಪೂರ್ಣಾಂಕ get_file_type (mode_t *ಬಹುಲಕ, bool nofollow, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ);

ಪೂರ್ಣಾಂಕ get_file_type_fd (ಪೂರ್ಣಾಂಕ fd, mode_t *ಬಹುಲಕ);

ಅಕ್ಷರ *get_user_name (uid_t uid);

ಪೂರ್ಣಾಂಕ ಸುರಕ್ಷಿತ_openat (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
                 libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ ಸುರಕ್ಷಿತ_write (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fname, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ_2, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ append_paths (ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err, ...) __attribute__ ((sentinel));

ಪೂರ್ಣಾಂಕ str2sig (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);

ಪೂರ್ಣಾಂಕ base64_decode (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *iptr, size_t isize, ಅಕ್ಷರ *optr, size_t osize, size_t *nbytes);
ಪೂರ್ಣಾಂಕ has_suffix (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix);
ಅಕ್ಷರ *libcrun_sysctl_ಕೀಲಿ_ಗಮ್ಯ_proc_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ);
ಅಕ್ಷರ *str_join_array (ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ, size_t ಗಾತ್ರ_2, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ array[], ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *joint);

ssize_t ಸುರಕ್ಷಿತ_readlinkat (ಪೂರ್ಣಾಂಕ dfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಅಕ್ಷರ **ಬಫರ್, ssize_t hint, libcrun_error_t *err);

ಅಕ್ಷರ **read_dir_entries (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_ಖಾಲಿಗಳು_string (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
  ಹಿಂತಿರುಗಿ s == ಖಾಲಿ || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ size_t
ಸುರಕ್ಷಿತ_strlen (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
  ಹಿಂತಿರುಗಿ s == ಖಾಲಿ ? 0 : ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (s);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ಮಾರ್ಗ_has_dot_dot_component (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
  ಆದರೆ (ಮಾರ್ಗ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;
  cleanup_free ಅಕ್ಷರ *npath = xstrdup (ಮಾರ್ಗ);
  ಅಕ್ಷರ *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  ಆಗ (cur)
    {
      ಆದರೆ (it)
        *it = '\0';

      ಆದರೆ (strcmp (cur, "..") == 0)
        ಹಿಂತಿರುಗಿ true;

      ಆದರೆ (it == ಖಾಲಿ)
        ನಿಲ್ಲಿಸು;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
waitpid_ignore_stopped (pid_t pid, ಪೂರ್ಣಾಂಕ *ಸ್ಥಿತಿ_2, ಪೂರ್ಣಾಂಕ options)
{
  ಪೂರ್ಣಾಂಕ ret, s;
  ಮಾಡು
    {
      s = 0;
      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (waitpid (pid, &s, options));
      ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ ret;
  } ಆಗ (WIFSTOPPED (s) || WIFCONTINUED (s));

  ಆದರೆ (ಸ್ಥಿತಿ_2)
    *ಸ್ಥಿತಿ_2 = s;

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
get_process_exit_ಸ್ಥಿತಿ (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (WIFEXITED (ಸ್ಥಿತಿ_2))
    ಹಿಂತಿರುಗಿ WEXITSTATUS (ಸ್ಥಿತಿ_2);
  ಆದರೆ (WIFSIGNALED (ಸ್ಥಿತಿ_2))
    ಹಿಂತಿರುಗಿ 128 + WTERMSIG (ಸ್ಥಿತಿ_2);

  ಹಿಂತಿರುಗಿ -1;
}

uid_t get_ಅತಿಪ್ರವಾಹ_uid (ಶೂನ್ಯ);
gid_t get_ಅತಿಪ್ರವಾಹ_gid (ಶೂನ್ಯ);

/* Adapted from systemd.  Include space for the NUL byte.  */
#ವ್ಯಾಖ್ಯಾನಿಸು DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (ಗಾತ್ರ (type) <= 1 ? 3U : ಗಾತ್ರ (type) <= 2 ? 5U  \
                                        : ಗಾತ್ರ (type) <= 4   ? 10U \
                                        : ಗಾತ್ರ (type) <= 8   ? 20U \
                                                               : ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ[-2 * (ಗಾತ್ರ (type) > 8)])))

#ವ್ಯಾಖ್ಯಾನಿಸು _STRLEN(s) (ಗಾತ್ರ (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#ವ್ಯಾಖ್ಯಾನಿಸು PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (ಪೂರ್ಣಾಂಕ))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಅಕ್ಷರ proc_fd_ಮಾರ್ಗ_t[PROC_PID_FD_STRLEN];

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು _STRLEN

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
get_proc_fd_ಮಾರ್ಗ (proc_fd_ಮಾರ್ಗ_t ಮಾರ್ಗ, pid_t pid, ಪೂರ್ಣಾಂಕ fd)
{
  ಸ್ಥಿರಾಂಕ size_t max_len = ಗಾತ್ರ (proc_fd_ಮಾರ್ಗ_t);
  size_t n;

  ಆದರೆ (pid)
    n = snprintf (ಮಾರ್ಗ, max_len, "/proc/%d/fd/%d", pid, fd);
  ಇಲ್ಲದಿದ್ದರೆ
    n = snprintf (ಮಾರ್ಗ, max_len, "/proc/self/fd/%d", fd);

  ಆದರೆ (UNLIKELY (n >= max_len))
    abort ();
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
get_proc_self_fd_ಮಾರ್ಗ (proc_fd_ಮಾರ್ಗ_t ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ fd)
{
  get_proc_fd_ಮಾರ್ಗ (ಮಾರ್ಗ, 0, fd);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
get_self_fd_ಮಾರ್ಗ (proc_fd_ಮಾರ್ಗ_t ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ fd)
{
  snprintf (ಮಾರ್ಗ, ಗಾತ್ರ (proc_fd_ಮಾರ್ಗ_t), "self/fd/%d", fd);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
validate_options (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ specified_options, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ supported_options, libcrun_error_t *err)
{
  ಆದರೆ (! ! (~supported_options & specified_options))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  ಹಿಂತಿರುಗಿ 0;
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ cpuset_string_ಗಮ್ಯ_bitmask (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *ಹೊರಹೋಗು_ಗಾತ್ರ, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
ರಚನೆ channel_fd_pair;

ರಚನೆ channel_fd_pair *channel_fd_pair_new (ಪೂರ್ಣಾಂಕ ಒಳ_fd, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_fd, size_t ಗಾತ್ರ_2);

ಶೂನ್ಯ channel_fd_pair_free (ರಚನೆ channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
ಪೂರ್ಣಾಂಕ channel_fd_pair_process (ರಚನೆ channel_fd_pair *channel, ಪೂರ್ಣಾಂಕ epollfd, libcrun_error_t *err);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_channel_fd_pairp (ಶೂನ್ಯ *p)
{
  ರಚನೆ channel_fd_pair **pp = (ರಚನೆ channel_fd_pair **) p;
  ಆದರೆ (*pp == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  channel_fd_pair_free (*pp);
}
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

ಪೂರ್ಣಾಂಕ libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_open_proc_file (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ flags, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ flags, libcrun_error_t *err);

#ಆದರೆ_ಅಂತ್ಯ
