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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-internal.h"
#ಅನ್ತರ್ಭಾವ "cgroup-systemd.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"
#ಅನ್ತರ್ಭಾವ "cgroup-resources.h"
#ಅನ್ತರ್ಭಾವ "ebpf.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>

ರಚನೆ default_dev_s default_devices[] = {
  { 'c', -1, -1, "m" },
  { 'b', -1, -1, "m" },
  { 'c', 1, 3, "rwm" },
  { 'c', 1, 8, "rwm" },
  { 'c', 1, 7, "rwm" },
  { 'c', 5, 0, "rwm" },
  { 'c', 1, 5, "rwm" },
  { 'c', 1, 9, "rwm" },
  { 'c', 5, 1, "rwm" },
  { 'c', 136, -1, "rwm" },
  { 'c', 5, 2, "rwm" },
  { 'c', 10, 200, "rwm" },
  { 0, 0, 0, ಖಾಲಿ }
};

ರಚನೆ default_dev_s *
get_default_devices ()
{
  ಹಿಂತಿರುಗಿ default_devices;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_cgroup_file_or_alias (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *alias, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, size_t len, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = write_cgroup_file (dirfd, name, ದತ್ತಾಂಶ, len, err);
  ಆದರೆ (UNLIKELY (alias != ಖಾಲಿ && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, ದತ್ತಾಂಶ, len, err);
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
openat_with_alias (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *alias, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಬಳಸಿದ_name, ಪೂರ್ಣಾಂಕ flags, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  *ಬಳಸಿದ_name = name;

  ret = openat (dirfd, name, flags);
  ಆದರೆ (UNLIKELY (ret < 0 && alias != ಖಾಲಿ && errno == ENOENT))
    {
      *ಬಳಸಿದ_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", name);
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
is_rwm (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *it;
  bool r = false;
  bool w = false;
  bool m = false;

  ಆದರೆ (str == ಖಾಲಿ)
    str = "";

  ಗಾಗಿ (it = str; *it; it++)
    ಆಯ್ಕೆ (*it)
      {
      ಸ್ಥಿತಿ 'r':
        r = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'w':
        w = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'm':
        m = true;
        ನಿಲ್ಲಿಸು;

      ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  ಹಿಂತಿರುಗಿ r && w && m ? 1 : 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
check_cgroup_v2_controller_ಲಭ್ಯ_wrapper (ಪೂರ್ಣಾಂಕ ret, ಪೂರ್ಣಾಂಕ cgroup_dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, libcrun_error_t *err)
{
  ಆದರೆ (ret == 0 || err == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  ಆದರೆ (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free ಅಕ್ಷರ *controllers = ಖಾಲಿ;
      libcrun_error_t tmp_err = ಖಾಲಿ;
      cleanup_free ಅಕ್ಷರ *ಕೀಲಿ = ಖಾಲಿ;
      ಅಕ್ಷರ *saveptr = ಖಾಲಿ;
      bool ಸಿಕ್ಕಿತು = false;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *token;
      ಅಕ್ಷರ *it;

      /* Check if the specified controller is enabled.  */
      ಕೀಲಿ = xstrdup (name);

      it = strchr (ಕೀಲಿ, '.');
      ಆದರೆ (it == ಖಾಲಿ)
        {
          crun_error_release (err);
          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      ಆದರೆ (strcmp (ಕೀಲಿ, "cgroup") == 0)
        ಹಿಂತಿರುಗಿ ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      ಆದರೆ (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, ಖಾಲಿ, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          ಹಿಂತಿರುಗಿ ret;
        }
      ಗಾಗಿ (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (ಖಾಲಿ, " \n", &saveptr))
        {
          ಆದರೆ (strcmp (token, ಕೀಲಿ) == 0)
            {
              ಸಿಕ್ಕಿತು = true;
              ನಿಲ್ಲಿಸು;
            }
        }
      ಆದರೆ (! ಸಿಕ್ಕಿತು)
        {
          cleanup_free ಅಕ್ಷರ *absolute_ಮಾರ್ಗ = ಖಾಲಿ;
          libcrun_error_t tmp_err = ಖಾಲಿ;

          crun_error_release (err);
          ret = get_realpath_ಗಮ್ಯ_file (cgroup_dirfd, "cgroup.controllers", &absolute_ಮಾರ್ಗ, &tmp_err);
          ಆದರೆ (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", ಕೀಲಿ, absolute_ಮಾರ್ಗ);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", ಕೀಲಿ);
            }
        }
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_file_and_check_controllers_at (bool cgroup2, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name_alias,
                                     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, size_t len, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, ದತ್ತಾಂಶ, len, err);
  ಆದರೆ (cgroup2)
    ಹಿಂತಿರುಗಿ check_cgroup_v2_controller_ಲಭ್ಯ_wrapper (ret, dirfd, name, err);
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_file_and_check_controllers_at (bool cgroup2, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = openat (dirfd, name, flags);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      ಆದರೆ (cgroup2)
        ಹಿಂತಿರುಗಿ check_cgroup_v2_controller_ಲಭ್ಯ_wrapper (ret, dirfd, name, err);
    }
  ಹಿಂತಿರುಗಿ ret;
}

/* The parser generates different structs but they are really all the same.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ runtime_spec_schema_defs_linux_ಖಂಡ_io_device_throttle throttling_s;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_blkio_v1_resources_throttling (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  ಅಕ್ಷರ fmt_buf[128];
  size_t i;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

  ಆದರೆ (throttling == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", name);

  ಗಾಗಿ (i = 0; i < throttling_len; i++)
    {
      ಪೂರ್ಣಾಂಕ ret;
      ಪೂರ್ಣಾಂಕ len;
      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->ದರ);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, fmt_buf, len));
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write `%s`", name);
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_blkio_v2_resources_throttling (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  ಅಕ್ಷರ fmt_buf[128];
  size_t i;

  ಆದರೆ (throttling == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಗಾಗಿ (i = 0; i < throttling_len; i++)
    {
      ಪೂರ್ಣಾಂಕ ret;
      ಪೂರ್ಣಾಂಕ len;
      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->ದರ);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, fmt_buf, len));
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write `%s`", name);
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_blkio_resources (ಪೂರ್ಣಾಂಕ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_ಖಂಡ_io *blkio,
                       libcrun_error_t *err)
{
  ಅಕ್ಷರ fmt_buf[128];
  ಪೂರ್ಣಾಂಕ len;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (blkio->ತೂಕ)
    {
      uint32_t val = blkio->ತೂಕ;

      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu32, val);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
      ಆದರೆ (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ಆದರೆ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu32, val);
                  ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
                    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;
            }
        }
    }
  ಆದರೆ (blkio->ಎಲೆ_ತೂಕ)
    {
      ಆದರೆ (cgroup2)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%d", blkio->ಎಲೆ_ತೂಕ);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (blkio->ತೂಕ_device_len)
    {
      ಆದರೆ (cgroup2)
        {
          cleanup_close ಪೂರ್ಣಾಂಕ wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          ಆದರೆ (UNLIKELY (wfd < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `io.bfq.weight`");
          ಗಾಗಿ (i = 0; i < blkio->ತೂಕ_device_len; i++)
            {
              uint32_t w = blkio->ತೂಕ_device[i]->ತೂಕ;

              len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->ತೂಕ_device[i]->major,
                              blkio->ತೂಕ_device[i]->minor, w);
              ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
                ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (wfd, fmt_buf, len));
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಎಲೆ_ತೂಕ_device_file_name = ಖಾಲಿ;
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತೂಕ_device_file_name = ಖಾಲಿ;
          cleanup_close ಪೂರ್ಣಾಂಕ w_leafdevice_fd = -1;
          cleanup_close ಪೂರ್ಣಾಂಕ w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &ತೂಕ_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ಆದರೆ (UNLIKELY (w_device_fd < 0))
            ಹಿಂತಿರುಗಿ w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &ಎಲೆ_ತೂಕ_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ಆದರೆ (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          ಗಾಗಿ (i = 0; i < blkio->ತೂಕ_device_len; i++)
            {
              len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->ತೂಕ_device[i]->major,
                              blkio->ತೂಕ_device[i]->minor, blkio->ತೂಕ_device[i]->ತೂಕ);
              ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
                ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (w_device_fd, fmt_buf, len));
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write `%s`", ತೂಕ_device_file_name);

              ಆದರೆ (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->ತೂಕ_device[i]->major,
                                  blkio->ತೂಕ_device[i]->minor, blkio->ತೂಕ_device[i]->ಎಲೆ_ತೂಕ);
                  ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
                    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (w_leafdevice_fd, fmt_buf, len));
                  ಆದರೆ (UNLIKELY (ret < 0))
                    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write `%s`", ಎಲೆ_ತೂಕ_device_file_name);
                }
            }
        }
    }
  ಆದರೆ (cgroup2)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ wfd = -1;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          ಹಿಂತಿರುಗಿ check_cgroup_v2_controller_ಲಭ್ಯ_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_ಜಾಲ_resources (ಪೂರ್ಣಾಂಕ dirfd_netclass, ಪೂರ್ಣಾಂಕ dirfd_netprio, runtime_spec_schema_config_linux_resources_ಜಾಲ *net,
                         libcrun_error_t *err)
{
  ಅಕ್ಷರ fmt_buf[128];
  ಪೂರ್ಣಾಂಕ len;
  ಪೂರ್ಣಾಂಕ ret;
  ಆದರೆ (net->class_id)
    {
      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu32, net->class_id);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (net->priorities_len)
    {
      size_t i;
      cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      ಗಾಗಿ (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->ಆದ್ಯತೆ);
          ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, fmt_buf, len));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_hugetlb_resources (ಪೂರ್ಣಾಂಕ dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  ಅಕ್ಷರ fmt_buf[128];
  size_t i;
  ಗಾಗಿ (i = 0; i < htlb_len; i++)
    {
      cleanup_free ಅಕ್ಷರ *filename = ಖಾಲಿ;
      cleanup_free ಅಕ್ಷರ *rsvd_filename = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ len;
      ಪೂರ್ಣಾಂಕ ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->ಪುಟ_ಗಾತ್ರ, suffix);

      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64, htlb[i]->ಎಲ್ಲೆ);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, ಖಾಲಿ, fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (skip_rsvd)
        ಮುಂದುವರಿ;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->ಪುಟ_ಗಾತ್ರ, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (crun_error_get_errno (err) != ENOENT)
            ಹಿಂತಿರುಗಿ ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_devices_resources_v1 (ಪೂರ್ಣಾಂಕ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  ಪೂರ್ಣಾಂಕ len;
  ಪೂರ್ಣಾಂಕ ret;

  ಗಾಗಿ (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#ವ್ಯಾಖ್ಯಾನಿಸು FMT_BUF_LEN 64
      ಅಕ್ಷರ fmt_buf[FMT_BUF_LEN];
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      ಆದರೆ (devs[i]->type == ಖಾಲಿ || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಅಕ್ಷರ fmt_buf_major[16];
          ಅಕ್ಷರ fmt_buf_minor[16];

#ವ್ಯಾಖ್ಯಾನಿಸು FMT_DEV(x, b)                                                                   \
  ಮಾಡು                                                                                    \
    {                                                                                   \
      ಆದರೆ (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, ಗಾತ್ರ (b), "%" PRIi64, x);                                \
          ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (b)))                                       \
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ಇಲ್ಲದಿದ್ದರೆ                                                                              \
        strcpy (b, "*");                                                                \
  } ಆಗ (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          ಆದರೆ (UNLIKELY (len >= FMT_BUF_LEN))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಗಾಗಿ (i = 0; default_devices[i].type; i++)
    {
      ಅಕ್ಷರ fmt_buf_major[16];
      ಅಕ್ಷರ fmt_buf_minor[16];
      ಅಕ್ಷರ device[64];
      ಪೂರ್ಣಾಂಕ len;

#ವ್ಯಾಖ್ಯಾನಿಸು FMT_DEV(x, b)                                                                   \
  ಮಾಡು                                                                                    \
    {                                                                                   \
      ಆದರೆ (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, ಗಾತ್ರ (b), "%d", x);                                      \
          ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (b)))                                       \
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ಇಲ್ಲದಿದ್ದರೆ                                                                              \
        strcpy (b, "*");                                                                \
  } ಆಗ (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು FMT_DEV

      len = snprintf (device, ಗಾತ್ರ (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (device)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (device), err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ i;
  ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ;

  ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_new (2048);

  ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_init_dev (ಕಾರ್ಯಕ್ರಮ, err);
  ಆದರೆ (UNLIKELY (ಕಾರ್ಯಕ್ರಮ == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಗಾಗಿ (i = (ಗಾತ್ರ (default_devices) / ಗಾತ್ರ (default_devices[0])) - 1; i >= 0; i--)
    {
      ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_append_dev (ಕಾರ್ಯಕ್ರಮ, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      ಆದರೆ (UNLIKELY (ಕಾರ್ಯಕ್ರಮ == ಖಾಲಿ))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
  ಗಾಗಿ (i = devs_len - 1; i >= 0; i--)
    {
      ಅಕ್ಷರ type = 'a';
      ಪೂರ್ಣಾಂಕ minor = -1, major = -1;
      ಆದರೆ (devs[i]->type != ಖಾಲಿ)
        type = devs[i]->type[0];

      ಆದರೆ (devs[i]->major_present)
        major = devs[i]->major;
      ಆದರೆ (devs[i]->minor_present)
        minor = devs[i]->minor;

      ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_append_dev (ಕಾರ್ಯಕ್ರಮ, devs[i]->access, type, major, minor, devs[i]->allow, err);
      ಆದರೆ (UNLIKELY (ಕಾರ್ಯಕ್ರಮ == ಖಾಲಿ))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

  ಕಾರ್ಯಕ್ರಮ = bpf_ಕಾರ್ಯಕ್ರಮ_complete_dev (ಕಾರ್ಯಕ್ರಮ, err);
  ಆದರೆ (UNLIKELY (ಕಾರ್ಯಕ್ರಮ == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ ಕಾರ್ಯಕ್ರಮ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_devices_resources_v2_ಆಂತರಿಕ (ಪೂರ್ಣಾಂಕ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ = ಖಾಲಿ;

  ಕಾರ್ಯಕ್ರಮ = create_dev_bpf (devs, devs_len, err);
  ಆದರೆ (UNLIKELY (ಕಾರ್ಯಕ್ರಮ == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ -1;

  ಹಿಂತಿರುಗಿ libcrun_ebpf_ತುಂಬು (ಕಾರ್ಯಕ್ರಮ, dirfd, ಖಾಲಿ, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_devices_resources_v2 (ಪೂರ್ಣಾಂಕ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_ಆಂತರಿಕ (dirfd, devs, devs_len, err);
  ಆದರೆ (LIKELY (ret == 0))
    ಹಿಂತಿರುಗಿ 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  ಗಾಗಿ (i = 0; i < devs_len; i++)
    {
      ಆದರೆ (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (! can_skip)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ rootless;

      rootless = is_rootless (&tmp_err);
      ಆದರೆ (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ಹಿಂತಿರುಗಿ ret;
        }
      ಆದರೆ (rootless)
        can_skip = true;
    }

  ಆದರೆ (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_devices_resources (ಪೂರ್ಣಾಂಕ dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  ಇಲ್ಲದಿದ್ದರೆ
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ rootless;

      rootless = is_rootless (&tmp_err);
      ಆದರೆ (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          ಹಿಂತಿರುಗಿ ret;
        }

      ಆದರೆ (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  ಹಿಂತಿರುಗಿ ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cg_itoa (ಅಕ್ಷರ *buf, size_t ಗಾತ್ರ_2, int64_t ಮೌಲ್ಯ, bool use_max, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ len;

  ಆದರೆ (use_max && ಮೌಲ್ಯ < 0)
    len = snprintf (buf, ಗಾತ್ರ_2, "max");
  ಇಲ್ಲದಿದ್ದರೆ
    len = snprintf (buf, ಗಾತ್ರ_2, "%" PRIi64, ಮೌಲ್ಯ);

  ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ_2))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

  ಹಿಂತಿರುಗಿ len;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_ಸ್ಮರಣೆ (ಪೂರ್ಣಾಂಕ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_ಸ್ಮರಣೆ *ಸ್ಮರಣೆ, libcrun_error_t *err)
{
  ಅಕ್ಷರ ಎಲ್ಲೆ_buf[32];
  ಪೂರ್ಣಾಂಕ ಎಲ್ಲೆ_buf_len;

  ಆದರೆ (! ಸ್ಮರಣೆ->ಎಲ್ಲೆ_present)
    ಹಿಂತಿರುಗಿ 0;

  ಎಲ್ಲೆ_buf_len = cg_itoa (ಎಲ್ಲೆ_buf, ಗಾತ್ರ (ಎಲ್ಲೆ_buf), ಸ್ಮರಣೆ->ಎಲ್ಲೆ, cgroup2, err);
  ಆದರೆ (UNLIKELY (ಎಲ್ಲೆ_buf_len < 0))
    ಹಿಂತಿರುಗಿ ಎಲ್ಲೆ_buf_len;

  ಹಿಂತಿರುಗಿ write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", ಎಲ್ಲೆ_buf, ಎಲ್ಲೆ_buf_len, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_ಸ್ಮರಣೆ_ಅದಲುಬದಲು (ಪೂರ್ಣಾಂಕ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_ಸ್ಮರಣೆ *ಸ್ಮರಣೆ,
                   libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  int64_t ಅದಲುಬದಲು;
  ಅಕ್ಷರ ಅದಲುಬದಲು_buf[32];
  ಪೂರ್ಣಾಂಕ len;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  ಆದರೆ (! ಸ್ಮರಣೆ->ಅದಲುಬದಲು_present)
    ಹಿಂತಿರುಗಿ 0;

  ಅದಲುಬದಲು = ಸ್ಮರಣೆ->ಅದಲುಬದಲು;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  ಆದರೆ (cgroup2 && ಸ್ಮರಣೆ->ಅದಲುಬದಲು > 0)
    {
      ಆದರೆ (! ಸ್ಮರಣೆ->ಎಲ್ಲೆ_present)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ಆದರೆ (ಸ್ಮರಣೆ->ಅದಲುಬದಲು < ಸ್ಮರಣೆ->ಎಲ್ಲೆ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      ಅದಲುಬದಲು -= ಸ್ಮರಣೆ->ಎಲ್ಲೆ;
    }

  len = cg_itoa (ಅದಲುಬದಲು_buf, ಗಾತ್ರ (ಅದಲುಬದಲು_buf), ಅದಲುಬದಲು, cgroup2, err);
  ಆದರೆ (UNLIKELY (len < 0))
    ಹಿಂತಿರುಗಿ len;

  ret = write_cgroup_file (dirfd, fname, ಅದಲುಬದಲು_buf, len, err);
  ಆದರೆ (ret >= 0)
    ಹಿಂತಿರುಗಿ ret;

  /* If swap is not enabled, ignore the error.  */
  ಆದರೆ (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      ಹಿಂತಿರುಗಿ 0;
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_ಸ್ಮರಣೆ_resources (ಪೂರ್ಣಾಂಕ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_ಸ್ಮರಣೆ *ಸ್ಮರಣೆ,
                        libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ len;
  ಪೂರ್ಣಾಂಕ ret;
  ಅಕ್ಷರ fmt_buf[32];
  bool ಸ್ಮರಣೆ_limits_written = false;

  ಆದರೆ (ಸ್ಮರಣೆ->ಎಲ್ಲೆ_present)
    {
      ret = write_ಸ್ಮರಣೆ (dirfd, cgroup2, ಸ್ಮರಣೆ, err);
      ಆದರೆ (ret >= 0)
        ಸ್ಮರಣೆ_limits_written = true;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (cgroup2 || crun_error_get_errno (err) != EINVAL)
            ಹಿಂತಿರುಗಿ ret;

          /*
            If we get an EINVAL error on cgroup v1 we reverse
            the order we write the memory limit and the swap.
            Attempt to write again the memory limit once the memory
            swap is written.
          */
          crun_error_release (err);
        }
    }

  ret = write_ಸ್ಮರಣೆ_ಅದಲುಬದಲು (dirfd, cgroup2, ಸ್ಮರಣೆ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ಸ್ಮರಣೆ->ಎಲ್ಲೆ_present && ! ಸ್ಮರಣೆ_limits_written)
    {
      ret = write_ಸ್ಮರಣೆ (dirfd, cgroup2, ಸ್ಮರಣೆ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (ಸ್ಮರಣೆ->kernel_present)
    {
      ಆದರೆ (cgroup2)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64, ಸ್ಮರಣೆ->kernel);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  ಆದರೆ (ಸ್ಮರಣೆ->use_hierarchy_present)
    {
      ಆದರೆ (cgroup2)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (ಸ್ಮರಣೆ->use_hierarchy) ? "1" : "0", 1, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (ಸ್ಮರಣೆ->reservation_present)
    {
      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64, ಸ್ಮರಣೆ->reservation);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 ಖಾಲಿ, fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (ಸ್ಮರಣೆ->disable_oom_killer)
    {
      ಆದರೆ (cgroup2)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (ಸ್ಮರಣೆ->kernel_tcp_present)
    {
      ಆದರೆ (cgroup2)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64, ಸ್ಮರಣೆ->kernel_tcp);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (ಸ್ಮರಣೆ->swappiness_present)
    {
      ಆದರೆ (cgroup2)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64, ಸ್ಮರಣೆ->swappiness);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
write_cpu_idle (ಪೂರ್ಣಾಂಕ cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  ಅಕ್ಷರ fmt_buf[32];
  ಪೂರ್ಣಾಂಕ len;

  ಆದರೆ (! cpu->idle_present)
    ಹಿಂತಿರುಗಿ 0;

  len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIi64, cpu->idle);
  ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
  ಹಿಂತಿರುಗಿ write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

ಪೂರ್ಣಾಂಕ
write_cpu_ಅವಧಿ (ಪೂರ್ಣಾಂಕ cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  ಅಕ್ಷರ fmt_buf[32];
  ಪೂರ್ಣಾಂಕ len;

  ಆದರೆ (! cpu->ಅವಧಿ_present)
    ಹಿಂತಿರುಗಿ 0;

  len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIi64, cpu->ಅವಧಿ);
  ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
  ಹಿಂತಿರುಗಿ write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_pids_resources (ಪೂರ್ಣಾಂಕ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  ಆದರೆ (pids->ಎಲ್ಲೆ)
    {
      ಅಕ್ಷರ fmt_buf[32];
      ಪೂರ್ಣಾಂಕ len;
      ಪೂರ್ಣಾಂಕ ret;

      len = cg_itoa (fmt_buf, ಗಾತ್ರ (fmt_buf), pids->ಎಲ್ಲೆ, true, err);
      ಆದರೆ (UNLIKELY (len < 0))
        ಹಿಂತಿರುಗಿ len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", ಖಾಲಿ, fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_cpu_resources (ಪೂರ್ಣಾಂಕ dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ len, period_len = 0;
  ಪೂರ್ಣಾಂಕ ret;
  ಅಕ್ಷರ fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free ಅಕ್ಷರ *period_str = ಖಾಲಿ;

  ಆದರೆ (cpu->shares)
    {
      uint32_t val = cpu->shares;

      ಆದರೆ (cgroup2)
        {
          val = convert_shares_ಗಮ್ಯ_ತೂಕ (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          ಆದರೆ (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%u", val);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 ಖಾಲಿ, fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (cpu->period)
    {
      ಆದರೆ (cgroup2)
        period = cpu->period;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64, cpu->period);
          ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              ಆದರೆ (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                ಹಿಂತಿರುಗಿ ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  ಆದರೆ (cpu->quota)
    {
      ಆದರೆ (cgroup2)
        quota = cpu->quota;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIi64, cpu->quota);
          ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          ಆದರೆ (period_str != ಖಾಲಿ)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;
            }
        }
    }
  ಆದರೆ (cpu->realtime_period)
    {
      ಆದರೆ (cgroup2)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64, cpu->realtime_period);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (cpu->realtime_runtime)
    {
      ಆದರೆ (cgroup2)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (cgroup2 && (quota > 0 || period > 0))
    {
      ಆದರೆ (period < 0)
        period = 100000;
      ಆದರೆ (quota < 0)
        len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "max %" PRIi64, period);
      ಇಲ್ಲದಿದ್ದರೆ
        len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", ಖಾಲಿ, fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ write_cpu_ಅವಧಿ (dirfd_cpu, cgroup2, cpu, err);
}

ಪೂರ್ಣಾಂಕ
write_cpuset_resources (ಪೂರ್ಣಾಂಕ dirfd_cpuset, ಪೂರ್ಣಾಂಕ cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (cpu == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (cpu->cpus), err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (cpu->mems),
                                                 err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_cgroup_subsystem (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subsystem, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ತುಂಬಿದ_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret, dirfd;

  ret = append_paths (&ತುಂಬಿದ_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, subsystem, ಮಾರ್ಗ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  dirfd = open (ತುಂಬಿದ_ಮಾರ್ಗ, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (dirfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ತುಂಬಿದ_ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ dirfd;
}

ಪೂರ್ಣಾಂಕ
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (resources->ಖಂಡ_io)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_ಖಂಡ_io *blkio = resources->ಖಂಡ_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (dirfd_blkio < 0))
        ಹಿಂತಿರುಗಿ dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (resources->ಜಾಲ)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ dirfd_netclass = -1;
      cleanup_close ಪೂರ್ಣಾಂಕ dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_ಜಾಲ *ಜಾಲ = resources->ಜಾಲ;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (dirfd_netclass < 0))
        ಹಿಂತಿರುಗಿ dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (dirfd_netprio < 0))
        ಹಿಂತಿರುಗಿ dirfd_netprio;

      ret = write_ಜಾಲ_resources (dirfd_netclass, dirfd_netprio, ಜಾಲ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (resources->hugepage_limits_len)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (dirfd_htlb < 0))
        ಹಿಂತಿರುಗಿ dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (resources->devices_len)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (dirfd_devs < 0))
        ಹಿಂತಿರುಗಿ dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (resources->ಸ್ಮರಣೆ)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (dirfd_mem < 0))
        ಹಿಂತಿರುಗಿ dirfd_mem;

      ret = write_ಸ್ಮರಣೆ_resources (dirfd_mem, false, resources->ಸ್ಮರಣೆ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (resources->pids)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (dirfd_pid < 0))
        ಹಿಂತಿರುಗಿ dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (resources->cpu)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ dirfd_cpu = -1;
      cleanup_close ಪೂರ್ಣಾಂಕ dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (dirfd_cpu < 0))
        ಹಿಂತಿರುಗಿ dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (resources->cpu->cpus == ಖಾಲಿ && resources->cpu->mems == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (dirfd_cpuset < 0))
        ಹಿಂತಿರುಗಿ dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (resources->unified && resources->unified->len > 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_unified_resources (ಪೂರ್ಣಾಂಕ cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ಗಾಗಿ (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
      cleanup_free ಅಕ್ಷರ *ಮೌಲ್ಯ = ಖಾಲಿ;
      ಅಕ್ಷರ *saveptr = ಖಾಲಿ;
      ಅಕ್ಷರ *ಸಾಲು;

      ಆದರೆ (strchr (resources->unified->ಕೀಲಿಗಳು[i], '/'))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->ಕೀಲಿಗಳು[i]);

      ಆದರೆ (is_ಖಾಲಿಗಳು_string (resources->unified->ಮೌಲ್ಯಗಳು[i]))
        ಮುಂದುವರಿ;

      ಮೌಲ್ಯ = xstrdup (resources->unified->ಮೌಲ್ಯಗಳು[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->ಕೀಲಿಗಳು[i], O_WRONLY, err);
      ಆದರೆ (UNLIKELY (fd < 0))
        ಹಿಂತಿರುಗಿ fd;

      ಗಾಗಿ (ಸಾಲು = strtok_r (ಮೌಲ್ಯ, "\n", &saveptr); ಸಾಲು; ಸಾಲು = strtok_r (ಖಾಲಿ, "\n", &saveptr))
        {
          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, ಸಾಲು, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಾಲು)));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to `%s`", resources->unified->ಕೀಲಿಗಳು[i]);
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ cgroup_dirfd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (resources->ಜಾಲ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  cgroup_dirfd = open (cgroup_ಮಾರ್ಗ, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (cgroup_dirfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", cgroup_ಮಾರ್ಗ);

  ಆದರೆ (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (resources->ಸ್ಮರಣೆ)
    {
      ret = write_ಸ್ಮರಣೆ_resources (cgroup_dirfd, true, resources->ಸ್ಮರಣೆ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಆದರೆ (resources->ಖಂಡ_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->ಖಂಡ_io, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  ಆದರೆ (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
ಪೂರ್ಣಾಂಕ
check_ಸ್ಮರಣೆ_before_update (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_ಸ್ಮರಣೆ *ಸ್ಮರಣೆ = resources->ಸ್ಮರಣೆ;
  cleanup_free ಅಕ್ಷರ *ಅದಲುಬದಲು_current = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *current = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ dirfd = -1;
  uint64_t ಎಲ್ಲೆ = 0;
  uint64_t val, val_ಅದಲುಬದಲು;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (ಮಾರ್ಗ == ಖಾಲಿ || ಸ್ಮರಣೆ == ಖಾಲಿ || ! (ಸ್ಮರಣೆ->check_before_update_present && ಸ್ಮರಣೆ->check_before_update))
    ಹಿಂತಿರುಗಿ 0;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (cgroup_ಬಹುಲಕ != CGROUP_ಬಹುಲಕ_UNIFIED)
    ಹಿಂತಿರುಗಿ 0;

  ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  dirfd = open (cgroup_ಮಾರ್ಗ, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (dirfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", cgroup_ಮಾರ್ಗ);

  ret = read_all_file_at (dirfd, "memory.current", &current, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &ಅದಲುಬದಲು_current, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  errno = 0;
  val = strtoll (current, ಖಾಲಿ, 10);
  ಆದರೆ (UNLIKELY (errno))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "parse memory.current");

  val_ಅದಲುಬದಲು = strtoll (ಅದಲುಬದಲು_current, ಖಾಲಿ, 10);
  ಆದರೆ (UNLIKELY (errno))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "parse memory.swap.current");

  ಆದರೆ (ಸ್ಮರಣೆ->ಎಲ್ಲೆ_present && ಸ್ಮರಣೆ->ಎಲ್ಲೆ >= 0)
    ಎಲ್ಲೆ = ಸ್ಮರಣೆ->ಎಲ್ಲೆ;
  ಆದರೆ (ಸ್ಮರಣೆ->ಅದಲುಬದಲು_present && ಸ್ಮರಣೆ->ಅದಲುಬದಲು >= 0)
    ಎಲ್ಲೆ += ಸ್ಮರಣೆ->ಅದಲುಬದಲು;

  ಆದರೆ (ಎಲ್ಲೆ <= val + val_ಅದಲುಬದಲು)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
update_cgroup_resources (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;

  (ಶೂನ್ಯ) state_ಬೇರು;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (ಮಾರ್ಗ == ಖಾಲಿ)
    {
      size_t i;

      ಆದರೆ (resources->ಖಂಡ_io || resources->ಜಾಲ || resources->hugepage_limits_len || resources->ಸ್ಮರಣೆ
          || resources->pids || resources->cpu)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set limits without cgroups");

      ಗಾಗಿ (i = 0; i < resources->devices_len; i++)
        {
          ಪೂರ್ಣಾಂಕ rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          ಆದರೆ (UNLIKELY (rwm < 0))
            ಹಿಂತಿರುಗಿ rwm;

          ಆದರೆ (rwm == 0)
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      ಹಿಂತಿರುಗಿ 0;
    }
  ಆಯ್ಕೆ (cgroup_ಬಹುಲಕ)
    {
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_UNIFIED:
      ಹಿಂತಿರುಗಿ update_cgroup_v2_resources (resources, ಮಾರ್ಗ, need_bpf_dev, err);

    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_LEGACY:
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_HYBRID:
      ಹಿಂತಿರುಗಿ update_cgroup_v1_resources (resources, ಮಾರ್ಗ, err);

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_ಬಹುಲಕ);
    }
}
