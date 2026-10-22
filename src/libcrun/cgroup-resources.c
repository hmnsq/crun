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

#define _GNU_SOURCE

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

ಸಂರಚನಾ default_dev_s default_devices[] = {
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
  { 0, 0, 0, ರಿಕ್ತ }
};

ಸಂರಚನಾ default_dev_s *
get_default_devices ()
{
  ಪ್ರತ್ಯಾಗಮ default_devices;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_cgroup_file_or_alias (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *alias, ಧ್ರುವ ಶೂನ್ಯ *data, size_t len, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  ಯದಿ (UNLIKELY (alias != ರಿಕ್ತ && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
openat_with_alias (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *alias, ಧ್ರುವ ವರ್ಣ **used_name, ಪೂರ್ಣಾಙ್ಕ flags, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  ಯದಿ (UNLIKELY (ret < 0 && alias != ರಿಕ್ತ && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", name);
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
is_rwm (ಧ್ರುವ ವರ್ಣ *str, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *it;
  bool r = false;
  bool w = false;
  bool m = false;

  ಯದಿ (str == ರಿಕ್ತ)
    str = "";

  ಪ್ರತಿ (it = str; *it; it++)
    ವಿಕಲ್ಪ (*it)
      {
      ಪಕ್ಷ 'r':
        r = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'w':
        w = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'm':
        m = true;
        ಭಙ್ಗ;

      ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  ಪ್ರತ್ಯಾಗಮ r && w && m ? 1 : 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
check_cgroup_v2_controller_available_wrapper (ಪೂರ್ಣಾಙ್ಕ ret, ಪೂರ್ಣಾಙ್ಕ cgroup_dirfd, ಧ್ರುವ ವರ್ಣ *name, libcrun_error_t *err)
{
  ಯದಿ (ret == 0 || err == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  ಯದಿ (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free ವರ್ಣ *controllers = ರಿಕ್ತ;
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      cleanup_free ವರ್ಣ *ಕೀಲಿ = ರಿಕ್ತ;
      ವರ್ಣ *saveptr = ರಿಕ್ತ;
      bool found = false;
      ಧ್ರುವ ವರ್ಣ *token;
      ವರ್ಣ *it;

      /* Check if the specified controller is enabled.  */
      ಕೀಲಿ = xstrdup (name);

      it = strchr (ಕೀಲಿ, '.');
      ಯದಿ (it == ರಿಕ್ತ)
        {
          crun_error_release (err);
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      ಯದಿ (strcmp (ಕೀಲಿ, "cgroup") == 0)
        ಪ್ರತ್ಯಾಗಮ ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      ಯದಿ (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, ರಿಕ್ತ, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಪ್ರತಿ (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (ರಿಕ್ತ, " \n", &saveptr))
        {
          ಯದಿ (strcmp (token, ಕೀಲಿ) == 0)
            {
              found = true;
              ಭಙ್ಗ;
            }
        }
      ಯದಿ (! found)
        {
          cleanup_free ವರ್ಣ *absolute_path = ರಿಕ್ತ;
          libcrun_error_t tmp_err = ರಿಕ್ತ;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          ಯದಿ (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", ಕೀಲಿ, absolute_path);
          ಅನ್ಯಥಾ
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", ಕೀಲಿ);
            }
        }
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_file_and_check_controllers_at (bool cgroup2, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *name_alias,
                                     ಧ್ರುವ ಶೂನ್ಯ *data, size_t len, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  ಯದಿ (cgroup2)
    ಪ್ರತ್ಯಾಗಮ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_file_and_check_controllers_at (bool cgroup2, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = openat (dirfd, name, flags);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      ಯದಿ (cgroup2)
        ಪ್ರತ್ಯಾಗಮ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

/* The parser generates different structs but they are really all the same.  */
ಪ್ರಕಾರನಿರ್ದೇಶ runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_blkio_v1_resources_throttling (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  ವರ್ಣ fmt_buf[128];
  size_t i;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

  ಯದಿ (throttling == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", name);

  ಪ್ರತಿ (i = 0; i < throttling_len; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ ret;
      ಪೂರ್ಣಾಙ್ಕ len;
      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (ಲಿಖ (fd, fmt_buf, len));
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write `%s`", name);
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_blkio_v2_resources_throttling (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  ವರ್ಣ fmt_buf[128];
  size_t i;

  ಯದಿ (throttling == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತಿ (i = 0; i < throttling_len; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ ret;
      ಪೂರ್ಣಾಙ್ಕ len;
      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (ಲಿಖ (fd, fmt_buf, len));
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write `%s`", name);
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_blkio_resources (ಪೂರ್ಣಾಙ್ಕ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  ವರ್ಣ fmt_buf[128];
  ಪೂರ್ಣಾಙ್ಕ len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu32, val);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
      ಯದಿ (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಅನ್ಯಥಾ
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ಯದಿ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu32, val);
                  ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
                    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;
            }
        }
    }
  ಯದಿ (blkio->leaf_weight)
    {
      ಯದಿ (cgroup2)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%d", blkio->leaf_weight);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (blkio->weight_device_len)
    {
      ಯದಿ (cgroup2)
        {
          cleanup_close ಪೂರ್ಣಾಙ್ಕ wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          ಯದಿ (UNLIKELY (wfd < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `io.bfq.weight`");
          ಪ್ರತಿ (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (ಲಿಖ (wfd, fmt_buf, len));
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      ಅನ್ಯಥಾ
        {
          ಧ್ರುವ ವರ್ಣ *leaf_weight_device_file_name = ರಿಕ್ತ;
          ಧ್ರುವ ವರ್ಣ *weight_device_file_name = ರಿಕ್ತ;
          cleanup_close ಪೂರ್ಣಾಙ್ಕ w_leafdevice_fd = -1;
          cleanup_close ಪೂರ್ಣಾಙ್ಕ w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ಯದಿ (UNLIKELY (w_device_fd < 0))
            ಪ್ರತ್ಯಾಗಮ w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ಯದಿ (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          ಪ್ರತಿ (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (ಲಿಖ (w_device_fd, fmt_buf, len));
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              ಯದಿ (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
                    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (ಲಿಖ (w_leafdevice_fd, fmt_buf, len));
                  ಯದಿ (UNLIKELY (ret < 0))
                    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  ಯದಿ (cgroup2)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ wfd = -1;
      ಧ್ರುವ ವರ್ಣ *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ಯದಿ (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          ಪ್ರತ್ಯಾಗಮ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_network_resources (ಪೂರ್ಣಾಙ್ಕ dirfd_netclass, ಪೂರ್ಣಾಙ್ಕ dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  ವರ್ಣ fmt_buf[128];
  ಪೂರ್ಣಾಙ್ಕ len;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಯದಿ (net->class_id)
    {
      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu32, net->class_id);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (net->priorities_len)
    {
      size_t i;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      ಯದಿ (UNLIKELY (fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      ಪ್ರತಿ (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (ಲಿಖ (fd, fmt_buf, len));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_hugetlb_resources (ಪೂರ್ಣಾಙ್ಕ dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  ವರ್ಣ fmt_buf[128];
  size_t i;
  ಪ್ರತಿ (i = 0; i < htlb_len; i++)
    {
      cleanup_free ವರ್ಣ *filename = ರಿಕ್ತ;
      cleanup_free ವರ್ಣ *rsvd_filename = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ len;
      ಪೂರ್ಣಾಙ್ಕ ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64, htlb[i]->limit);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, ರಿಕ್ತ, fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (skip_rsvd)
        ಅನುವರ್ತ;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (crun_error_get_errno (err) != ENOENT)
            ಪ್ರತ್ಯಾಗಮ ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_devices_resources_v1 (ಪೂರ್ಣಾಙ್ಕ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಪ್ರತಿ (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      ವರ್ಣ fmt_buf[FMT_BUF_LEN];
      ಧ್ರುವ ವರ್ಣ *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      ಯದಿ (devs[i]->type == ರಿಕ್ತ || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      ಅನ್ಯಥಾ
        {
          ವರ್ಣ fmt_buf_major[16];
          ವರ್ಣ fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  ಕುರು                                                                                    \
    {                                                                                   \
      ಯದಿ (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, ಪರಿಮಾಣ (b), "%" PRIi64, x);                                \
          ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (b)))                                       \
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ಅನ್ಯಥಾ                                                                              \
        strcpy (b, "*");                                                                \
  } ಯಾವತ್ (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          ಯದಿ (UNLIKELY (len >= FMT_BUF_LEN))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತಿ (i = 0; default_devices[i].type; i++)
    {
      ವರ್ಣ fmt_buf_major[16];
      ವರ್ಣ fmt_buf_minor[16];
      ವರ್ಣ device[64];
      ಪೂರ್ಣಾಙ್ಕ len;

#define FMT_DEV(x, b)                                                                   \
  ಕುರು                                                                                    \
    {                                                                                   \
      ಯದಿ (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, ಪರಿಮಾಣ (b), "%d", x);                                      \
          ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (b)))                                       \
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ಅನ್ಯಥಾ                                                                              \
        strcpy (b, "*");                                                                \
  } ಯಾವತ್ (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, ಪರಿಮಾಣ (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (device)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ i;
  ಸಂರಚನಾ bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  ಯದಿ (UNLIKELY (program == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಪ್ರತಿ (i = (ಪರಿಮಾಣ (default_devices) / ಪರಿಮಾಣ (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      ಯದಿ (UNLIKELY (program == ರಿಕ್ತ))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
  ಪ್ರತಿ (i = devs_len - 1; i >= 0; i--)
    {
      ವರ್ಣ type = 'a';
      ಪೂರ್ಣಾಙ್ಕ minor = -1, major = -1;
      ಯದಿ (devs[i]->type != ರಿಕ್ತ)
        type = devs[i]->type[0];

      ಯದಿ (devs[i]->major_present)
        major = devs[i]->major;
      ಯದಿ (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      ಯದಿ (UNLIKELY (program == ರಿಕ್ತ))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

  program = bpf_program_complete_dev (program, err);
  ಯದಿ (UNLIKELY (program == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ program;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_devices_resources_v2_internal (ಪೂರ್ಣಾಙ್ಕ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free ಸಂರಚನಾ bpf_program *program = ರಿಕ್ತ;

  program = create_dev_bpf (devs, devs_len, err);
  ಯದಿ (UNLIKELY (program == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ -1;

  ಪ್ರತ್ಯಾಗಮ libcrun_ebpf_load (program, dirfd, ರಿಕ್ತ, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_devices_resources_v2 (ಪೂರ್ಣಾಙ್ಕ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  ಯದಿ (LIKELY (ret == 0))
    ಪ್ರತ್ಯಾಗಮ 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  ಪ್ರತಿ (i = 0; i < devs_len; i++)
    {
      ಯದಿ (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          ಭಙ್ಗ;
        }
    }

  ಯದಿ (! can_skip)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ rootless;

      rootless = is_rootless (&tmp_err);
      ಯದಿ (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಯದಿ (rootless)
        can_skip = true;
    }

  ಯದಿ (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_devices_resources (ಪೂರ್ಣಾಙ್ಕ dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  ಅನ್ಯಥಾ
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ rootless;

      rootless = is_rootless (&tmp_err);
      ಯದಿ (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          ಪ್ರತ್ಯಾಗಮ ret;
        }

      ಯದಿ (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cg_itoa (ವರ್ಣ *buf, size_t size, int64_t ಮೌಲ್ಯ, bool use_max, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ len;

  ಯದಿ (use_max && ಮೌಲ್ಯ < 0)
    len = snprintf (buf, size, "max");
  ಅನ್ಯಥಾ
    len = snprintf (buf, size, "%" PRIi64, ಮೌಲ್ಯ);

  ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) size))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

  ಪ್ರತ್ಯಾಗಮ len;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_memory (ಪೂರ್ಣಾಙ್ಕ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  ವರ್ಣ limit_buf[32];
  ಪೂರ್ಣಾಙ್ಕ limit_buf_len;

  ಯದಿ (! memory->limit_present)
    ಪ್ರತ್ಯಾಗಮ 0;

  limit_buf_len = cg_itoa (limit_buf, ಪರಿಮಾಣ (limit_buf), memory->limit, cgroup2, err);
  ಯದಿ (UNLIKELY (limit_buf_len < 0))
    ಪ್ರತ್ಯಾಗಮ limit_buf_len;

  ಪ್ರತ್ಯಾಗಮ write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_memory_swap (ಪೂರ್ಣಾಙ್ಕ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  int64_t swap;
  ವರ್ಣ swap_buf[32];
  ಪೂರ್ಣಾಙ್ಕ len;
  ಧ್ರುವ ವರ್ಣ *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  ಯದಿ (! memory->swap_present)
    ಪ್ರತ್ಯಾಗಮ 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  ಯದಿ (cgroup2 && memory->swap > 0)
    {
      ಯದಿ (! memory->limit_present)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ಯದಿ (memory->swap < memory->limit)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, ಪರಿಮಾಣ (swap_buf), swap, cgroup2, err);
  ಯದಿ (UNLIKELY (len < 0))
    ಪ್ರತ್ಯಾಗಮ len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  ಯದಿ (ret >= 0)
    ಪ್ರತ್ಯಾಗಮ ret;

  /* If swap is not enabled, ignore the error.  */
  ಯದಿ (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_memory_resources (ಪೂರ್ಣಾಙ್ಕ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ len;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ವರ್ಣ fmt_buf[32];
  bool memory_limits_written = false;

  ಯದಿ (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      ಯದಿ (ret >= 0)
        memory_limits_written = true;
      ಅನ್ಯಥಾ
        {
          ಯದಿ (cgroup2 || crun_error_get_errno (err) != EINVAL)
            ಪ್ರತ್ಯಾಗಮ ret;

          /*
            If we get an EINVAL error on cgroup v1 we reverse
            the order we write the memory limit and the swap.
            Attempt to write again the memory limit once the memory
            swap is written.
          */
          crun_error_release (err);
        }
    }

  ret = write_memory_swap (dirfd, cgroup2, memory, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (memory->kernel_present)
    {
      ಯದಿ (cgroup2)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64, memory->kernel);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  ಯದಿ (memory->use_hierarchy_present)
    {
      ಯದಿ (cgroup2)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (memory->reservation_present)
    {
      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64, memory->reservation);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 ರಿಕ್ತ, fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (memory->disable_oom_killer)
    {
      ಯದಿ (cgroup2)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (memory->kernel_tcp_present)
    {
      ಯದಿ (cgroup2)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (memory->swappiness_present)
    {
      ಯದಿ (cgroup2)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64, memory->swappiness);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
write_cpu_idle (ಪೂರ್ಣಾಙ್ಕ cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  ವರ್ಣ fmt_buf[32];
  ಪೂರ್ಣಾಙ್ಕ len;

  ಯದಿ (! cpu->idle_present)
    ಪ್ರತ್ಯಾಗಮ 0;

  len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIi64, cpu->idle);
  ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
  ಪ್ರತ್ಯಾಗಮ write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

ಪೂರ್ಣಾಙ್ಕ
write_cpu_burst (ಪೂರ್ಣಾಙ್ಕ cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  ವರ್ಣ fmt_buf[32];
  ಪೂರ್ಣಾಙ್ಕ len;

  ಯದಿ (! cpu->burst_present)
    ಪ್ರತ್ಯಾಗಮ 0;

  len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIi64, cpu->burst);
  ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
  ಪ್ರತ್ಯಾಗಮ write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_pids_resources (ಪೂರ್ಣಾಙ್ಕ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  ಯದಿ (pids->limit)
    {
      ವರ್ಣ fmt_buf[32];
      ಪೂರ್ಣಾಙ್ಕ len;
      ಪೂರ್ಣಾಙ್ಕ ret;

      len = cg_itoa (fmt_buf, ಪರಿಮಾಣ (fmt_buf), pids->limit, true, err);
      ಯದಿ (UNLIKELY (len < 0))
        ಪ್ರತ್ಯಾಗಮ len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", ರಿಕ್ತ, fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_cpu_resources (ಪೂರ್ಣಾಙ್ಕ dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ len, period_len = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ವರ್ಣ fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free ವರ್ಣ *period_str = ರಿಕ್ತ;

  ಯದಿ (cpu->shares)
    {
      uint32_t val = cpu->shares;

      ಯದಿ (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          ಯದಿ (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              ಯದಿ (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%u", val);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 ರಿಕ್ತ, fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (cpu->period)
    {
      ಯದಿ (cgroup2)
        period = cpu->period;
      ಅನ್ಯಥಾ
        {
          len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64, cpu->period);
          ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              ಯದಿ (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                ಪ್ರತ್ಯಾಗಮ ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  ಯದಿ (cpu->quota)
    {
      ಯದಿ (cgroup2)
        quota = cpu->quota;
      ಅನ್ಯಥಾ
        {
          len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIi64, cpu->quota);
          ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          ಯದಿ (period_str != ರಿಕ್ತ)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;
            }
        }
    }
  ಯದಿ (cpu->realtime_period)
    {
      ಯದಿ (cgroup2)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64, cpu->realtime_period);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (cpu->realtime_runtime)
    {
      ಯದಿ (cgroup2)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (cgroup2 && (quota > 0 || period > 0))
    {
      ಯದಿ (period < 0)
        period = 100000;
      ಯದಿ (quota < 0)
        len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "max %" PRIi64, period);
      ಅನ್ಯಥಾ
        len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", ರಿಕ್ತ, fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

ಪೂರ್ಣಾಙ್ಕ
write_cpuset_resources (ಪೂರ್ಣಾಙ್ಕ dirfd_cpuset, ಪೂರ್ಣಾಙ್ಕ cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (cpu == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_cgroup_subsystem (ಧ್ರುವ ವರ್ಣ *subsystem, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *full_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_ಬೇರು, subsystem, path, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", full_path);

  ಪ್ರತ್ಯಾಗಮ dirfd;
}

ಪೂರ್ಣಾಙ್ಕ
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (resources->block_io)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      ಯದಿ (UNLIKELY (dirfd_blkio < 0))
        ಪ್ರತ್ಯಾಗಮ dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (resources->network)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd_netclass = -1;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      ಯದಿ (UNLIKELY (dirfd_netclass < 0))
        ಪ್ರತ್ಯಾಗಮ dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      ಯದಿ (UNLIKELY (dirfd_netprio < 0))
        ಪ್ರತ್ಯಾಗಮ dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (resources->hugepage_limits_len)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      ಯದಿ (UNLIKELY (dirfd_htlb < 0))
        ಪ್ರತ್ಯಾಗಮ dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (resources->devices_len)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      ಯದಿ (UNLIKELY (dirfd_devs < 0))
        ಪ್ರತ್ಯಾಗಮ dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (resources->memory)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      ಯದಿ (UNLIKELY (dirfd_mem < 0))
        ಪ್ರತ್ಯಾಗಮ dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (resources->pids)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      ಯದಿ (UNLIKELY (dirfd_pid < 0))
        ಪ್ರತ್ಯಾಗಮ dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (resources->cpu)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd_cpu = -1;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      ಯದಿ (UNLIKELY (dirfd_cpu < 0))
        ಪ್ರತ್ಯಾಗಮ dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (resources->cpu->cpus == ರಿಕ್ತ && resources->cpu->mems == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      ಯದಿ (UNLIKELY (dirfd_cpuset < 0))
        ಪ್ರತ್ಯಾಗಮ dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (resources->unified && resources->unified->len > 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_unified_resources (ಪೂರ್ಣಾಙ್ಕ cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಪ್ರತಿ (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
      cleanup_free ವರ್ಣ *ಮೌಲ್ಯ = ರಿಕ್ತ;
      ವರ್ಣ *saveptr = ರಿಕ್ತ;
      ವರ್ಣ *ಸಾಲು;

      ಯದಿ (strchr (resources->unified->ಕೀಲಿಗಳು[i], '/'))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->ಕೀಲಿಗಳು[i]);

      ಯದಿ (is_empty_string (resources->unified->ಮೌಲ್ಯಗಳು[i]))
        ಅನುವರ್ತ;

      ಮೌಲ್ಯ = xstrdup (resources->unified->ಮೌಲ್ಯಗಳು[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->ಕೀಲಿಗಳು[i], O_WRONLY, err);
      ಯದಿ (UNLIKELY (fd < 0))
        ಪ್ರತ್ಯಾಗಮ fd;

      ಪ್ರತಿ (ಸಾಲು = strtok_r (ಮೌಲ್ಯ, "\n", &saveptr); ಸಾಲು; ಸಾಲು = strtok_r (ರಿಕ್ತ, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (ಲಿಖ (fd, ಸಾಲು, strlen (ಸಾಲು)));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to `%s`", resources->unified->ಕೀಲಿಗಳು[i]);
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, ಧ್ರುವ ವರ್ಣ *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ cgroup_dirfd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (resources->network)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, path, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (cgroup_dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ಯದಿ (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಯದಿ (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  ಯದಿ (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
ಪೂರ್ಣಾಙ್ಕ
check_memory_before_update (ಧ್ರುವ ವರ್ಣ *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free ವರ್ಣ *swap_current = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *current = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (path == ರಿಕ್ತ || memory == ರಿಕ್ತ || ! (memory->check_before_update_present && memory->check_before_update))
    ಪ್ರತ್ಯಾಗಮ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (cgroup_mode != CGROUP_MODE_UNIFIED)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, path, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  errno = 0;
  val = strtoll (current, ರಿಕ್ತ, 10);
  ಯದಿ (UNLIKELY (errno))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, ರಿಕ್ತ, 10);
  ಯದಿ (UNLIKELY (errno))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "parse memory.swap.current");

  ಯದಿ (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  ಯದಿ (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  ಯದಿ (limit <= val + val_swap)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
update_cgroup_resources (ಧ್ರುವ ವರ್ಣ *path,
                         ಧ್ರುವ ವರ್ಣ *state_ಬೇರು,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;

  (ಶೂನ್ಯ) state_ಬೇರು;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (path == ರಿಕ್ತ)
    {
      size_t i;

      ಯದಿ (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set limits without cgroups");

      ಪ್ರತಿ (i = 0; i < resources->devices_len; i++)
        {
          ಪೂರ್ಣಾಙ್ಕ rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          ಯದಿ (UNLIKELY (rwm < 0))
            ಪ್ರತ್ಯಾಗಮ rwm;

          ಯದಿ (rwm == 0)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ವಿಕಲ್ಪ (cgroup_mode)
    {
    ಪಕ್ಷ CGROUP_MODE_UNIFIED:
      ಪ್ರತ್ಯಾಗಮ update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    ಪಕ್ಷ CGROUP_MODE_LEGACY:
    ಪಕ್ಷ CGROUP_MODE_HYBRID:
      ಪ್ರತ್ಯಾಗಮ update_cgroup_v1_resources (resources, path, err);

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
