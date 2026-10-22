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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-systemd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-resources.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ebpf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/vfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <libgen.h>

ਬਣਤਰ default_dev_s default_devices[] = {
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
  { 0, 0, 0, ਨਿਰਾ }
};

ਬਣਤਰ default_dev_s *
get_default_devices ()
{
  ਵਾਪਸ default_devices;
}

ਸਥਿਰ ਪੂਰਨ
write_cgroup_file_or_alias (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *alias, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, size_t len, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = write_cgroup_file (dirfd, name, ਅੰਕੜੇ, len, err);
  ਜੇ (UNLIKELY (alias != ਨਿਰਾ && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, ਅੰਕੜੇ, len, err);
    }
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
openat_with_alias (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *alias, ਅਚਲ ਅੱਖਰ **ਵਰਤਿਆ_name, ਪੂਰਨ flags, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  *ਵਰਤਿਆ_name = name;

  ret = openat (dirfd, name, flags);
  ਜੇ (UNLIKELY (ret < 0 && alias != ਨਿਰਾ && errno == ENOENT))
    {
      *ਵਰਤਿਆ_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", name);
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
is_rwm (ਅਚਲ ਅੱਖਰ *str, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *it;
  bool r = false;
  bool w = false;
  bool m = false;

  ਜੇ (str == ਨਿਰਾ)
    str = "";

  ਲਈ (it = str; *it; it++)
    ਚੋਣ (*it)
      {
      ਹਾਲਤ 'r':
        r = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'w':
        w = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'm':
        m = true;
        ਰੋਕੋ;

      ਮੂਲ:
        ਵਾਪਸ crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  ਵਾਪਸ r && w && m ? 1 : 0;
}

ਸਥਿਰ ਪੂਰਨ
check_cgroup_v2_controller_ਉਪਲਬਧ_wrapper (ਪੂਰਨ ret, ਪੂਰਨ cgroup_dirfd, ਅਚਲ ਅੱਖਰ *name, libcrun_error_t *err)
{
  ਜੇ (ret == 0 || err == ਨਿਰਾ)
    ਵਾਪਸ 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  ਜੇ (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free ਅੱਖਰ *controllers = ਨਿਰਾ;
      libcrun_error_t tmp_err = ਨਿਰਾ;
      cleanup_free ਅੱਖਰ *ਕੁੰਜੀ = ਨਿਰਾ;
      ਅੱਖਰ *saveptr = ਨਿਰਾ;
      bool ਮਿਲਿਆ = false;
      ਅਚਲ ਅੱਖਰ *token;
      ਅੱਖਰ *it;

      /* Check if the specified controller is enabled.  */
      ਕੁੰਜੀ = xstrdup (name);

      it = strchr (ਕੁੰਜੀ, '.');
      ਜੇ (it == ਨਿਰਾ)
        {
          crun_error_release (err);
          ਵਾਪਸ crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      ਜੇ (strcmp (ਕੁੰਜੀ, "cgroup") == 0)
        ਵਾਪਸ ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      ਜੇ (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, ਨਿਰਾ, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          ਵਾਪਸ ret;
        }
      ਲਈ (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (ਨਿਰਾ, " \n", &saveptr))
        {
          ਜੇ (strcmp (token, ਕੁੰਜੀ) == 0)
            {
              ਮਿਲਿਆ = true;
              ਰੋਕੋ;
            }
        }
      ਜੇ (! ਮਿਲਿਆ)
        {
          cleanup_free ਅੱਖਰ *absolute_ਰਸਤਾ = ਨਿਰਾ;
          libcrun_error_t tmp_err = ਨਿਰਾ;

          crun_error_release (err);
          ret = get_realpath_ਮੰਜ਼ਿਲ_file (cgroup_dirfd, "cgroup.controllers", &absolute_ਰਸਤਾ, &tmp_err);
          ਜੇ (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", ਕੁੰਜੀ, absolute_ਰਸਤਾ);
          ਨਹੀਂਤਾਂ
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", ਕੁੰਜੀ);
            }
        }
    }
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
write_file_and_check_controllers_at (bool cgroup2, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *name_alias,
                                     ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, size_t len, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, ਅੰਕੜੇ, len, err);
  ਜੇ (cgroup2)
    ਵਾਪਸ check_cgroup_v2_controller_ਉਪਲਬਧ_wrapper (ret, dirfd, name, err);
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
open_file_and_check_controllers_at (bool cgroup2, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = openat (dirfd, name, flags);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      ਜੇ (cgroup2)
        ਵਾਪਸ check_cgroup_v2_controller_ਉਪਲਬਧ_wrapper (ret, dirfd, name, err);
    }
  ਵਾਪਸ ret;
}

/* The parser generates different structs but they are really all the same.  */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ runtime_spec_schema_defs_linux_ਖੰਡ_io_device_throttle throttling_s;

ਸਥਿਰ ਪੂਰਨ
write_blkio_v1_resources_throttling (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  ਅੱਖਰ fmt_buf[128];
  size_t i;
  cleanup_close ਪੂਰਨ fd = -1;

  ਜੇ (throttling == ਨਿਰਾ)
    ਵਾਪਸ 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", name);

  ਲਈ (i = 0; i < throttling_len; i++)
    {
      ਪੂਰਨ ret;
      ਪੂਰਨ len;
      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->ਦਰ);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, fmt_buf, len));
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "write `%s`", name);
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
write_blkio_v2_resources_throttling (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  ਅੱਖਰ fmt_buf[128];
  size_t i;

  ਜੇ (throttling == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਲਈ (i = 0; i < throttling_len; i++)
    {
      ਪੂਰਨ ret;
      ਪੂਰਨ len;
      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->ਦਰ);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, fmt_buf, len));
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "write `%s`", name);
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
write_blkio_resources (ਪੂਰਨ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_ਖੰਡ_io *blkio,
                       libcrun_error_t *err)
{
  ਅੱਖਰ fmt_buf[128];
  ਪੂਰਨ len;
  ਪੂਰਨ ret;

  ਜੇ (blkio->ਭਾਰ)
    {
      uint32_t val = blkio->ਭਾਰ;

      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu32, val);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
      ਜੇ (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
      ਨਹੀਂਤਾਂ
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ਜੇ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu32, val);
                  ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
                    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;
            }
        }
    }
  ਜੇ (blkio->ਪੱਤੀ_ਭਾਰ)
    {
      ਜੇ (cgroup2)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%d", blkio->ਪੱਤੀ_ਭਾਰ);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਜੇ (blkio->ਭਾਰ_device_len)
    {
      ਜੇ (cgroup2)
        {
          cleanup_close ਪੂਰਨ wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          ਜੇ (UNLIKELY (wfd < 0))
            ਵਾਪਸ crun_make_error (err, errno, "open `io.bfq.weight`");
          ਲਈ (i = 0; i < blkio->ਭਾਰ_device_len; i++)
            {
              uint32_t w = blkio->ਭਾਰ_device[i]->ਭਾਰ;

              len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->ਭਾਰ_device[i]->major,
                              blkio->ਭਾਰ_device[i]->minor, w);
              ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
                ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (wfd, fmt_buf, len));
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      ਨਹੀਂਤਾਂ
        {
          ਅਚਲ ਅੱਖਰ *ਪੱਤੀ_ਭਾਰ_device_file_name = ਨਿਰਾ;
          ਅਚਲ ਅੱਖਰ *ਭਾਰ_device_file_name = ਨਿਰਾ;
          cleanup_close ਪੂਰਨ w_leafdevice_fd = -1;
          cleanup_close ਪੂਰਨ w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &ਭਾਰ_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ਜੇ (UNLIKELY (w_device_fd < 0))
            ਵਾਪਸ w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &ਪੱਤੀ_ਭਾਰ_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ਜੇ (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          ਲਈ (i = 0; i < blkio->ਭਾਰ_device_len; i++)
            {
              len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->ਭਾਰ_device[i]->major,
                              blkio->ਭਾਰ_device[i]->minor, blkio->ਭਾਰ_device[i]->ਭਾਰ);
              ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
                ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (w_device_fd, fmt_buf, len));
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ crun_make_error (err, errno, "write `%s`", ਭਾਰ_device_file_name);

              ਜੇ (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->ਭਾਰ_device[i]->major,
                                  blkio->ਭਾਰ_device[i]->minor, blkio->ਭਾਰ_device[i]->ਪੱਤੀ_ਭਾਰ);
                  ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
                    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (w_leafdevice_fd, fmt_buf, len));
                  ਜੇ (UNLIKELY (ret < 0))
                    ਵਾਪਸ crun_make_error (err, errno, "write `%s`", ਪੱਤੀ_ਭਾਰ_device_file_name);
                }
            }
        }
    }
  ਜੇ (cgroup2)
    {
      cleanup_close ਪੂਰਨ wfd = -1;
      ਅਚਲ ਅੱਖਰ *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ਜੇ (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          ਵਾਪਸ check_cgroup_v2_controller_ਉਪਲਬਧ_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
write_ਜਾਲ_resources (ਪੂਰਨ dirfd_netclass, ਪੂਰਨ dirfd_netprio, runtime_spec_schema_config_linux_resources_ਜਾਲ *net,
                         libcrun_error_t *err)
{
  ਅੱਖਰ fmt_buf[128];
  ਪੂਰਨ len;
  ਪੂਰਨ ret;
  ਜੇ (net->class_id)
    {
      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu32, net->class_id);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਜੇ (net->priorities_len)
    {
      size_t i;
      cleanup_close ਪੂਰਨ fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      ਜੇ (UNLIKELY (fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      ਲਈ (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->ਤਰਜੀਹ);
          ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
            ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, fmt_buf, len));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
write_hugetlb_resources (ਪੂਰਨ dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  ਅੱਖਰ fmt_buf[128];
  size_t i;
  ਲਈ (i = 0; i < htlb_len; i++)
    {
      cleanup_free ਅੱਖਰ *filename = ਨਿਰਾ;
      cleanup_free ਅੱਖਰ *rsvd_filename = ਨਿਰਾ;
      ਪੂਰਨ len;
      ਪੂਰਨ ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->ਪੰਨਾ_ਮਾਪ, suffix);

      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64, htlb[i]->ਹੱਦਬੰਦੀ);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, ਨਿਰਾ, fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (skip_rsvd)
        ਜਾਰੀ;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->ਪੰਨਾ_ਮਾਪ, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (crun_error_get_errno (err) != ENOENT)
            ਵਾਪਸ ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
write_devices_resources_v1 (ਪੂਰਨ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  ਪੂਰਨ len;
  ਪੂਰਨ ret;

  ਲਈ (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FMT_BUF_LEN 64
      ਅੱਖਰ fmt_buf[FMT_BUF_LEN];
      ਅਚਲ ਅੱਖਰ *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      ਜੇ (devs[i]->type == ਨਿਰਾ || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      ਨਹੀਂਤਾਂ
        {
          ਅੱਖਰ fmt_buf_major[16];
          ਅੱਖਰ fmt_buf_minor[16];

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FMT_DEV(x, b)                                                                   \
  ਕਰੋ                                                                                    \
    {                                                                                   \
      ਜੇ (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, ਆਕਾਰ (b), "%" PRIi64, x);                                \
          ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (b)))                                       \
            ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ਨਹੀਂਤਾਂ                                                                              \
        strcpy (b, "*");                                                                \
  } ਜਦੋਂ (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          ਜੇ (UNLIKELY (len >= FMT_BUF_LEN))
            ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਲਈ (i = 0; default_devices[i].type; i++)
    {
      ਅੱਖਰ fmt_buf_major[16];
      ਅੱਖਰ fmt_buf_minor[16];
      ਅੱਖਰ device[64];
      ਪੂਰਨ len;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FMT_DEV(x, b)                                                                   \
  ਕਰੋ                                                                                    \
    {                                                                                   \
      ਜੇ (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, ਆਕਾਰ (b), "%d", x);                                      \
          ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (b)))                                       \
            ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ਨਹੀਂਤਾਂ                                                                              \
        strcpy (b, "*");                                                                \
  } ਜਦੋਂ (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ FMT_DEV

      len = snprintf (device, ਆਕਾਰ (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (device)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, ਲੜੀ_ਲੰਬਾਈ (device), err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  ਪੂਰਨ i;
  ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ;

  ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_new (2048);

  ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_init_dev (ਪ੍ਰੋਗਰਾਮ, err);
  ਜੇ (UNLIKELY (ਪ੍ਰੋਗਰਾਮ == ਨਿਰਾ))
    ਵਾਪਸ ਨਿਰਾ;

  ਲਈ (i = (ਆਕਾਰ (default_devices) / ਆਕਾਰ (default_devices[0])) - 1; i >= 0; i--)
    {
      ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_append_dev (ਪ੍ਰੋਗਰਾਮ, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      ਜੇ (UNLIKELY (ਪ੍ਰੋਗਰਾਮ == ਨਿਰਾ))
        ਵਾਪਸ ਨਿਰਾ;
    }
  ਲਈ (i = devs_len - 1; i >= 0; i--)
    {
      ਅੱਖਰ type = 'a';
      ਪੂਰਨ minor = -1, major = -1;
      ਜੇ (devs[i]->type != ਨਿਰਾ)
        type = devs[i]->type[0];

      ਜੇ (devs[i]->major_present)
        major = devs[i]->major;
      ਜੇ (devs[i]->minor_present)
        minor = devs[i]->minor;

      ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_append_dev (ਪ੍ਰੋਗਰਾਮ, devs[i]->access, type, major, minor, devs[i]->allow, err);
      ਜੇ (UNLIKELY (ਪ੍ਰੋਗਰਾਮ == ਨਿਰਾ))
        ਵਾਪਸ ਨਿਰਾ;
    }

  ਪ੍ਰੋਗਰਾਮ = bpf_ਪ੍ਰੋਗਰਾਮ_complete_dev (ਪ੍ਰੋਗਰਾਮ, err);
  ਜੇ (UNLIKELY (ਪ੍ਰੋਗਰਾਮ == ਨਿਰਾ))
    ਵਾਪਸ ਨਿਰਾ;

  ਵਾਪਸ ਪ੍ਰੋਗਰਾਮ;
}

ਸਥਿਰ ਪੂਰਨ
write_devices_resources_v2_ਅੰਦਰੂਨੀ (ਪੂਰਨ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ = ਨਿਰਾ;

  ਪ੍ਰੋਗਰਾਮ = create_dev_bpf (devs, devs_len, err);
  ਜੇ (UNLIKELY (ਪ੍ਰੋਗਰਾਮ == ਨਿਰਾ))
    ਵਾਪਸ -1;

  ਵਾਪਸ libcrun_ebpf_ਲੱਦੋ (ਪ੍ਰੋਗਰਾਮ, dirfd, ਨਿਰਾ, err);
}

ਸਥਿਰ ਪੂਰਨ
write_devices_resources_v2 (ਪੂਰਨ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_ਅੰਦਰੂਨੀ (dirfd, devs, devs_len, err);
  ਜੇ (LIKELY (ret == 0))
    ਵਾਪਸ 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  ਲਈ (i = 0; i < devs_len; i++)
    {
      ਜੇ (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          ਰੋਕੋ;
        }
    }

  ਜੇ (! can_skip)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਪੂਰਨ rootless;

      rootless = is_rootless (&tmp_err);
      ਜੇ (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ਵਾਪਸ ret;
        }
      ਜੇ (rootless)
        can_skip = true;
    }

  ਜੇ (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
write_devices_resources (ਪੂਰਨ dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ਜੇ (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  ਨਹੀਂਤਾਂ
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਪੂਰਨ rootless;

      rootless = is_rootless (&tmp_err);
      ਜੇ (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          ਵਾਪਸ ret;
        }

      ਜੇ (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  ਵਾਪਸ ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
ਸਥਿਰ ਪੂਰਨ
cg_itoa (ਅੱਖਰ *buf, size_t ਮਾਪ, int64_t ਮੁੱਲ, bool use_max, libcrun_error_t *err)
{
  ਪੂਰਨ len;

  ਜੇ (use_max && ਮੁੱਲ < 0)
    len = snprintf (buf, ਮਾਪ, "max");
  ਨਹੀਂਤਾਂ
    len = snprintf (buf, ਮਾਪ, "%" PRIi64, ਮੁੱਲ);

  ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਮਾਪ))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

  ਵਾਪਸ len;
}

ਸਥਿਰ ਪੂਰਨ
write_ਸਮ੍ਰਿਤੀ (ਪੂਰਨ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_ਸਮ੍ਰਿਤੀ *ਸਮ੍ਰਿਤੀ, libcrun_error_t *err)
{
  ਅੱਖਰ ਹੱਦਬੰਦੀ_buf[32];
  ਪੂਰਨ ਹੱਦਬੰਦੀ_buf_len;

  ਜੇ (! ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ_present)
    ਵਾਪਸ 0;

  ਹੱਦਬੰਦੀ_buf_len = cg_itoa (ਹੱਦਬੰਦੀ_buf, ਆਕਾਰ (ਹੱਦਬੰਦੀ_buf), ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ, cgroup2, err);
  ਜੇ (UNLIKELY (ਹੱਦਬੰਦੀ_buf_len < 0))
    ਵਾਪਸ ਹੱਦਬੰਦੀ_buf_len;

  ਵਾਪਸ write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", ਹੱਦਬੰਦੀ_buf, ਹੱਦਬੰਦੀ_buf_len, err);
}

ਸਥਿਰ ਪੂਰਨ
write_ਸਮ੍ਰਿਤੀ_ਅਦਲਾ_ਬਦਲੀ (ਪੂਰਨ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_ਸਮ੍ਰਿਤੀ *ਸਮ੍ਰਿਤੀ,
                   libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  int64_t ਅਦਲਾ_ਬਦਲੀ;
  ਅੱਖਰ ਅਦਲਾ_ਬਦਲੀ_buf[32];
  ਪੂਰਨ len;
  ਅਚਲ ਅੱਖਰ *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  ਜੇ (! ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ_present)
    ਵਾਪਸ 0;

  ਅਦਲਾ_ਬਦਲੀ = ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  ਜੇ (cgroup2 && ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ > 0)
    {
      ਜੇ (! ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ_present)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ਜੇ (ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ < ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      ਅਦਲਾ_ਬਦਲੀ -= ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ;
    }

  len = cg_itoa (ਅਦਲਾ_ਬਦਲੀ_buf, ਆਕਾਰ (ਅਦਲਾ_ਬਦਲੀ_buf), ਅਦਲਾ_ਬਦਲੀ, cgroup2, err);
  ਜੇ (UNLIKELY (len < 0))
    ਵਾਪਸ len;

  ret = write_cgroup_file (dirfd, fname, ਅਦਲਾ_ਬਦਲੀ_buf, len, err);
  ਜੇ (ret >= 0)
    ਵਾਪਸ ret;

  /* If swap is not enabled, ignore the error.  */
  ਜੇ (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      ਵਾਪਸ 0;
    }

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
write_ਸਮ੍ਰਿਤੀ_resources (ਪੂਰਨ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_ਸਮ੍ਰਿਤੀ *ਸਮ੍ਰਿਤੀ,
                        libcrun_error_t *err)
{
  ਪੂਰਨ len;
  ਪੂਰਨ ret;
  ਅੱਖਰ fmt_buf[32];
  bool ਸਮ੍ਰਿਤੀ_limits_written = false;

  ਜੇ (ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ_present)
    {
      ret = write_ਸਮ੍ਰਿਤੀ (dirfd, cgroup2, ਸਮ੍ਰਿਤੀ, err);
      ਜੇ (ret >= 0)
        ਸਮ੍ਰਿਤੀ_limits_written = true;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (cgroup2 || crun_error_get_errno (err) != EINVAL)
            ਵਾਪਸ ret;

          /*
            If we get an EINVAL error on cgroup v1 we reverse
            the order we write the memory limit and the swap.
            Attempt to write again the memory limit once the memory
            swap is written.
          */
          crun_error_release (err);
        }
    }

  ret = write_ਸਮ੍ਰਿਤੀ_ਅਦਲਾ_ਬਦਲੀ (dirfd, cgroup2, ਸਮ੍ਰਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ_present && ! ਸਮ੍ਰਿਤੀ_limits_written)
    {
      ret = write_ਸਮ੍ਰਿਤੀ (dirfd, cgroup2, ਸਮ੍ਰਿਤੀ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (ਸਮ੍ਰਿਤੀ->kernel_present)
    {
      ਜੇ (cgroup2)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64, ਸਮ੍ਰਿਤੀ->kernel);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  ਜੇ (ਸਮ੍ਰਿਤੀ->use_hierarchy_present)
    {
      ਜੇ (cgroup2)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (ਸਮ੍ਰਿਤੀ->use_hierarchy) ? "1" : "0", 1, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (ਸਮ੍ਰਿਤੀ->reservation_present)
    {
      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64, ਸਮ੍ਰਿਤੀ->reservation);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 ਨਿਰਾ, fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਜੇ (ਸਮ੍ਰਿਤੀ->disable_oom_killer)
    {
      ਜੇ (cgroup2)
        ਵਾਪਸ crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਜੇ (ਸਮ੍ਰਿਤੀ->kernel_tcp_present)
    {
      ਜੇ (cgroup2)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64, ਸਮ੍ਰਿਤੀ->kernel_tcp);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਜੇ (ਸਮ੍ਰਿਤੀ->swappiness_present)
    {
      ਜੇ (cgroup2)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64, ਸਮ੍ਰਿਤੀ->swappiness);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
write_cpu_idle (ਪੂਰਨ cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  ਅੱਖਰ fmt_buf[32];
  ਪੂਰਨ len;

  ਜੇ (! cpu->idle_present)
    ਵਾਪਸ 0;

  len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIi64, cpu->idle);
  ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
  ਵਾਪਸ write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

ਪੂਰਨ
write_cpu_ਮਿਆਦ (ਪੂਰਨ cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  ਅੱਖਰ fmt_buf[32];
  ਪੂਰਨ len;

  ਜੇ (! cpu->ਮਿਆਦ_present)
    ਵਾਪਸ 0;

  len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIi64, cpu->ਮਿਆਦ);
  ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
  ਵਾਪਸ write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

ਸਥਿਰ ਪੂਰਨ
write_pids_resources (ਪੂਰਨ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  ਜੇ (pids->ਹੱਦਬੰਦੀ)
    {
      ਅੱਖਰ fmt_buf[32];
      ਪੂਰਨ len;
      ਪੂਰਨ ret;

      len = cg_itoa (fmt_buf, ਆਕਾਰ (fmt_buf), pids->ਹੱਦਬੰਦੀ, true, err);
      ਜੇ (UNLIKELY (len < 0))
        ਵਾਪਸ len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", ਨਿਰਾ, fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
write_cpu_resources (ਪੂਰਨ dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  ਪੂਰਨ len, period_len = 0;
  ਪੂਰਨ ret;
  ਅੱਖਰ fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free ਅੱਖਰ *period_str = ਨਿਰਾ;

  ਜੇ (cpu->shares)
    {
      uint32_t val = cpu->shares;

      ਜੇ (cgroup2)
        {
          val = convert_shares_ਮੰਜ਼ਿਲ_ਭਾਰ (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          ਜੇ (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              ਜੇ (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%u", val);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 ਨਿਰਾ, fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਜੇ (cpu->period)
    {
      ਜੇ (cgroup2)
        period = cpu->period;
      ਨਹੀਂਤਾਂ
        {
          len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64, cpu->period);
          ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
            ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          ਜੇ (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              ਜੇ (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                ਵਾਪਸ ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  ਜੇ (cpu->quota)
    {
      ਜੇ (cgroup2)
        quota = cpu->quota;
      ਨਹੀਂਤਾਂ
        {
          len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIi64, cpu->quota);
          ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
            ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          ਜੇ (period_str != ਨਿਰਾ)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;
            }
        }
    }
  ਜੇ (cpu->realtime_period)
    {
      ਜੇ (cgroup2)
        ਵਾਪਸ crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64, cpu->realtime_period);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਜੇ (cpu->realtime_runtime)
    {
      ਜੇ (cgroup2)
        ਵਾਪਸ crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (cgroup2 && (quota > 0 || period > 0))
    {
      ਜੇ (period < 0)
        period = 100000;
      ਜੇ (quota < 0)
        len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "max %" PRIi64, period);
      ਨਹੀਂਤਾਂ
        len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", ਨਿਰਾ, fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ write_cpu_ਮਿਆਦ (dirfd_cpu, cgroup2, cpu, err);
}

ਪੂਰਨ
write_cpuset_resources (ਪੂਰਨ dirfd_cpuset, ਪੂਰਨ cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ਜੇ (cpu == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਜੇ (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, ਲੜੀ_ਲੰਬਾਈ (cpu->cpus), err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਜੇ (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, ਲੜੀ_ਲੰਬਾਈ (cpu->mems),
                                                 err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
open_cgroup_subsystem (ਅਚਲ ਅੱਖਰ *subsystem, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਭਰਿਆ_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ret, dirfd;

  ret = append_paths (&ਭਰਿਆ_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, subsystem, ਰਸਤਾ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  dirfd = open (ਭਰਿਆ_ਰਸਤਾ, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (dirfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਭਰਿਆ_ਰਸਤਾ);

  ਵਾਪਸ dirfd;
}

ਪੂਰਨ
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ਜੇ (resources->ਖੰਡ_io)
    {
      cleanup_close ਪੂਰਨ dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_ਖੰਡ_io *blkio = resources->ਖੰਡ_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (dirfd_blkio < 0))
        ਵਾਪਸ dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (resources->ਜਾਲ)
    {
      cleanup_close ਪੂਰਨ dirfd_netclass = -1;
      cleanup_close ਪੂਰਨ dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_ਜਾਲ *ਜਾਲ = resources->ਜਾਲ;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (dirfd_netclass < 0))
        ਵਾਪਸ dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (dirfd_netprio < 0))
        ਵਾਪਸ dirfd_netprio;

      ret = write_ਜਾਲ_resources (dirfd_netclass, dirfd_netprio, ਜਾਲ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (resources->hugepage_limits_len)
    {
      cleanup_close ਪੂਰਨ dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (dirfd_htlb < 0))
        ਵਾਪਸ dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (resources->devices_len)
    {
      cleanup_close ਪੂਰਨ dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (dirfd_devs < 0))
        ਵਾਪਸ dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (resources->ਸਮ੍ਰਿਤੀ)
    {
      cleanup_close ਪੂਰਨ dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (dirfd_mem < 0))
        ਵਾਪਸ dirfd_mem;

      ret = write_ਸਮ੍ਰਿਤੀ_resources (dirfd_mem, false, resources->ਸਮ੍ਰਿਤੀ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (resources->pids)
    {
      cleanup_close ਪੂਰਨ dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (dirfd_pid < 0))
        ਵਾਪਸ dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (resources->cpu)
    {
      cleanup_close ਪੂਰਨ dirfd_cpu = -1;
      cleanup_close ਪੂਰਨ dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (dirfd_cpu < 0))
        ਵਾਪਸ dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (resources->cpu->cpus == ਨਿਰਾ && resources->cpu->mems == ਨਿਰਾ)
        ਵਾਪਸ 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (dirfd_cpuset < 0))
        ਵਾਪਸ dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (resources->unified && resources->unified->len > 0)
    ਵਾਪਸ crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
write_unified_resources (ਪੂਰਨ cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  ਪੂਰਨ ret;

  ਲਈ (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close ਪੂਰਨ fd = -1;
      cleanup_free ਅੱਖਰ *ਮੁੱਲ = ਨਿਰਾ;
      ਅੱਖਰ *saveptr = ਨਿਰਾ;
      ਅੱਖਰ *ਸਤਰ;

      ਜੇ (strchr (resources->unified->ਕੁੰਜੀਆਂ[i], '/'))
        ਵਾਪਸ crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->ਕੁੰਜੀਆਂ[i]);

      ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (resources->unified->ਮੁੱਲਾਂ[i]))
        ਜਾਰੀ;

      ਮੁੱਲ = xstrdup (resources->unified->ਮੁੱਲਾਂ[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->ਕੁੰਜੀਆਂ[i], O_WRONLY, err);
      ਜੇ (UNLIKELY (fd < 0))
        ਵਾਪਸ fd;

      ਲਈ (ਸਤਰ = strtok_r (ਮੁੱਲ, "\n", &saveptr); ਸਤਰ; ਸਤਰ = strtok_r (ਨਿਰਾ, "\n", &saveptr))
        {
          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, ਸਤਰ, ਲੜੀ_ਲੰਬਾਈ (ਸਤਰ)));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "write to `%s`", resources->unified->ਕੁੰਜੀਆਂ[i]);
        }
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ cgroup_dirfd = -1;
  ਪੂਰਨ ret;

  ਜੇ (resources->ਜਾਲ)
    ਵਾਪਸ crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  cgroup_dirfd = open (cgroup_ਰਸਤਾ, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (cgroup_dirfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", cgroup_ਰਸਤਾ);

  ਜੇ (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (resources->ਸਮ੍ਰਿਤੀ)
    {
      ret = write_ਸਮ੍ਰਿਤੀ_resources (cgroup_dirfd, true, resources->ਸਮ੍ਰਿਤੀ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਜੇ (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਜੇ (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਜੇ (resources->ਖੰਡ_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->ਖੰਡ_io, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  ਜੇ (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
ਪੂਰਨ
check_ਸਮ੍ਰਿਤੀ_before_update (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_ਸਮ੍ਰਿਤੀ *ਸਮ੍ਰਿਤੀ = resources->ਸਮ੍ਰਿਤੀ;
  cleanup_free ਅੱਖਰ *ਅਦਲਾ_ਬਦਲੀ_current = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *current = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ dirfd = -1;
  uint64_t ਹੱਦਬੰਦੀ = 0;
  uint64_t val, val_ਅਦਲਾ_ਬਦਲੀ;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  ਜੇ (ਰਸਤਾ == ਨਿਰਾ || ਸਮ੍ਰਿਤੀ == ਨਿਰਾ || ! (ਸਮ੍ਰਿਤੀ->check_before_update_present && ਸਮ੍ਰਿਤੀ->check_before_update))
    ਵਾਪਸ 0;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (cgroup_ਬਹੁਲਕ != CGROUP_ਬਹੁਲਕ_UNIFIED)
    ਵਾਪਸ 0;

  ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  dirfd = open (cgroup_ਰਸਤਾ, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (dirfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", cgroup_ਰਸਤਾ);

  ret = read_all_file_at (dirfd, "memory.current", &current, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &ਅਦਲਾ_ਬਦਲੀ_current, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  errno = 0;
  val = strtoll (current, ਨਿਰਾ, 10);
  ਜੇ (UNLIKELY (errno))
    ਵਾਪਸ crun_make_error (err, errno, "parse memory.current");

  val_ਅਦਲਾ_ਬਦਲੀ = strtoll (ਅਦਲਾ_ਬਦਲੀ_current, ਨਿਰਾ, 10);
  ਜੇ (UNLIKELY (errno))
    ਵਾਪਸ crun_make_error (err, errno, "parse memory.swap.current");

  ਜੇ (ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ_present && ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ >= 0)
    ਹੱਦਬੰਦੀ = ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ;
  ਜੇ (ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ_present && ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ >= 0)
    ਹੱਦਬੰਦੀ += ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ;

  ਜੇ (ਹੱਦਬੰਦੀ <= val + val_ਅਦਲਾ_ਬਦਲੀ)
    ਵਾਪਸ crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  ਵਾਪਸ 0;
}

ਪੂਰਨ
update_cgroup_resources (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
                         ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;

  (ਖਾਲੀ) state_ਜੜ੍ਹ;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (ਰਸਤਾ == ਨਿਰਾ)
    {
      size_t i;

      ਜੇ (resources->ਖੰਡ_io || resources->ਜਾਲ || resources->hugepage_limits_len || resources->ਸਮ੍ਰਿਤੀ
          || resources->pids || resources->cpu)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set limits without cgroups");

      ਲਈ (i = 0; i < resources->devices_len; i++)
        {
          ਪੂਰਨ rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          ਜੇ (UNLIKELY (rwm < 0))
            ਵਾਪਸ rwm;

          ਜੇ (rwm == 0)
            ਵਾਪਸ crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      ਵਾਪਸ 0;
    }
  ਚੋਣ (cgroup_ਬਹੁਲਕ)
    {
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_UNIFIED:
      ਵਾਪਸ update_cgroup_v2_resources (resources, ਰਸਤਾ, need_bpf_dev, err);

    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_LEGACY:
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_HYBRID:
      ਵਾਪਸ update_cgroup_v1_resources (resources, ਰਸਤਾ, err);

    ਮੂਲ:
      ਵਾਪਸ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_ਬਹੁਲਕ);
    }
}
