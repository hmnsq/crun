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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "cgroup.h"
#അന്തര്ഭാവ "cgroup-internal.h"
#അന്തര്ഭാവ "cgroup-systemd.h"
#അന്തര്ഭാവ "cgroup-utils.h"
#അന്തര്ഭാവ "cgroup-resources.h"
#അന്തര്ഭാവ "ebpf.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "status.h"
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/vfs.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <libgen.h>

ഘടന default_dev_s default_devices[] = {
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
  { 0, 0, 0, ശൂന്യ }
};

ഘടന default_dev_s *
get_default_devices ()
{
  തിരികെ default_devices;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_cgroup_file_or_alias (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *alias, സ്ഥിരം ശൂന്യം *data, size_t len, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  എങ്കിൽ (UNLIKELY (alias != ശൂന്യ && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  തിരികെ ret;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
openat_with_alias (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *alias, സ്ഥിരം അക്ഷരം **used_name, പൂർണ്ണസംഖ്യ flags, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  എങ്കിൽ (UNLIKELY (ret < 0 && alias != ശൂന്യ && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", name);
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
is_rwm (സ്ഥിരം അക്ഷരം *str, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *it;
  bool r = false;
  bool w = false;
  bool m = false;

  എങ്കിൽ (str == ശൂന്യ)
    str = "";

  വേണ്ടി (it = str; *it; it++)
    തിരഞ്ഞെടുപ്പ് (*it)
      {
      അവസ്ഥ 'r':
        r = true;
        നിർത്തുക;

      അവസ്ഥ 'w':
        w = true;
        നിർത്തുക;

      അവസ്ഥ 'm':
        m = true;
        നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
        തിരികെ crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  തിരികെ r && w && m ? 1 : 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
check_cgroup_v2_controller_available_wrapper (പൂർണ്ണസംഖ്യ ret, പൂർണ്ണസംഖ്യ cgroup_dirfd, സ്ഥിരം അക്ഷരം *name, libcrun_error_t *err)
{
  എങ്കിൽ (ret == 0 || err == ശൂന്യ)
    തിരികെ 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  എങ്കിൽ (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free അക്ഷരം *controllers = ശൂന്യ;
      libcrun_error_t tmp_err = ശൂന്യ;
      cleanup_free അക്ഷരം *താക്കോൽ = ശൂന്യ;
      അക്ഷരം *saveptr = ശൂന്യ;
      bool found = false;
      സ്ഥിരം അക്ഷരം *token;
      അക്ഷരം *it;

      /* Check if the specified controller is enabled.  */
      താക്കോൽ = xstrdup (name);

      it = strchr (താക്കോൽ, '.');
      എങ്കിൽ (it == ശൂന്യ)
        {
          crun_error_release (err);
          തിരികെ crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      എങ്കിൽ (strcmp (താക്കോൽ, "cgroup") == 0)
        തിരികെ ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      എങ്കിൽ (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, ശൂന്യ, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          തിരികെ ret;
        }
      വേണ്ടി (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (ശൂന്യ, " \n", &saveptr))
        {
          എങ്കിൽ (strcmp (token, താക്കോൽ) == 0)
            {
              found = true;
              നിർത്തുക;
            }
        }
      എങ്കിൽ (! found)
        {
          cleanup_free അക്ഷരം *absolute_path = ശൂന്യ;
          libcrun_error_t tmp_err = ശൂന്യ;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          എങ്കിൽ (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", താക്കോൽ, absolute_path);
          അല്ലെങ്കിൽ
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", താക്കോൽ);
            }
        }
    }
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_file_and_check_controllers_at (bool cgroup2, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *name_alias,
                                     സ്ഥിരം ശൂന്യം *data, size_t len, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  എങ്കിൽ (cgroup2)
    തിരികെ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
open_file_and_check_controllers_at (bool cgroup2, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = openat (dirfd, name, flags);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      എങ്കിൽ (cgroup2)
        തിരികെ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  തിരികെ ret;
}

/* The parser generates different structs but they are really all the same.  */
തരനിർവചനം runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_blkio_v1_resources_throttling (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  അക്ഷരം fmt_buf[128];
  size_t i;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

  എങ്കിൽ (throttling == ശൂന്യ)
    തിരികെ 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", name);

  വേണ്ടി (i = 0; i < throttling_len; i++)
    {
      പൂർണ്ണസംഖ്യ ret;
      പൂർണ്ണസംഖ്യ len;
      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (ലിഖ (fd, fmt_buf, len));
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "write `%s`", name);
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_blkio_v2_resources_throttling (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  അക്ഷരം fmt_buf[128];
  size_t i;

  എങ്കിൽ (throttling == ശൂന്യ)
    തിരികെ 0;

  വേണ്ടി (i = 0; i < throttling_len; i++)
    {
      പൂർണ്ണസംഖ്യ ret;
      പൂർണ്ണസംഖ്യ len;
      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (ലിഖ (fd, fmt_buf, len));
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "write `%s`", name);
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_blkio_resources (പൂർണ്ണസംഖ്യ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  അക്ഷരം fmt_buf[128];
  പൂർണ്ണസംഖ്യ len;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu32, val);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
      എങ്കിൽ (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
      അല്ലെങ്കിൽ
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu32, val);
                  എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
                    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;
            }
        }
    }
  എങ്കിൽ (blkio->leaf_weight)
    {
      എങ്കിൽ (cgroup2)
        തിരികെ crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%d", blkio->leaf_weight);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  എങ്കിൽ (blkio->weight_device_len)
    {
      എങ്കിൽ (cgroup2)
        {
          cleanup_close പൂർണ്ണസംഖ്യ wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          എങ്കിൽ (UNLIKELY (wfd < 0))
            തിരികെ crun_make_error (err, errno, "open `io.bfq.weight`");
          വേണ്ടി (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
                തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (ലിഖ (wfd, fmt_buf, len));
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      അല്ലെങ്കിൽ
        {
          സ്ഥിരം അക്ഷരം *leaf_weight_device_file_name = ശൂന്യ;
          സ്ഥിരം അക്ഷരം *weight_device_file_name = ശൂന്യ;
          cleanup_close പൂർണ്ണസംഖ്യ w_leafdevice_fd = -1;
          cleanup_close പൂർണ്ണസംഖ്യ w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          എങ്കിൽ (UNLIKELY (w_device_fd < 0))
            തിരികെ w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          എങ്കിൽ (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          വേണ്ടി (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
                തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (ലിഖ (w_device_fd, fmt_buf, len));
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              എങ്കിൽ (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
                    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (ലിഖ (w_leafdevice_fd, fmt_buf, len));
                  എങ്കിൽ (UNLIKELY (ret < 0))
                    തിരികെ crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  എങ്കിൽ (cgroup2)
    {
      cleanup_close പൂർണ്ണസംഖ്യ wfd = -1;
      സ്ഥിരം അക്ഷരം *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          തിരികെ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_network_resources (പൂർണ്ണസംഖ്യ dirfd_netclass, പൂർണ്ണസംഖ്യ dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  അക്ഷരം fmt_buf[128];
  പൂർണ്ണസംഖ്യ len;
  പൂർണ്ണസംഖ്യ ret;
  എങ്കിൽ (net->class_id)
    {
      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu32, net->class_id);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  എങ്കിൽ (net->priorities_len)
    {
      size_t i;
      cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (fd < 0))
        തിരികെ crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      വേണ്ടി (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
            തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (ലിഖ (fd, fmt_buf, len));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_hugetlb_resources (പൂർണ്ണസംഖ്യ dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  അക്ഷരം fmt_buf[128];
  size_t i;
  വേണ്ടി (i = 0; i < htlb_len; i++)
    {
      cleanup_free അക്ഷരം *filename = ശൂന്യ;
      cleanup_free അക്ഷരം *rsvd_filename = ശൂന്യ;
      പൂർണ്ണസംഖ്യ len;
      പൂർണ്ണസംഖ്യ ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64, htlb[i]->limit);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, ശൂന്യ, fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      എങ്കിൽ (skip_rsvd)
        തുടരുക;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (crun_error_get_errno (err) != ENOENT)
            തിരികെ ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_devices_resources_v1 (പൂർണ്ണസംഖ്യ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  പൂർണ്ണസംഖ്യ len;
  പൂർണ്ണസംഖ്യ ret;

  വേണ്ടി (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      അക്ഷരം fmt_buf[FMT_BUF_LEN];
      സ്ഥിരം അക്ഷരം *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      എങ്കിൽ (devs[i]->type == ശൂന്യ || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      അല്ലെങ്കിൽ
        {
          അക്ഷരം fmt_buf_major[16];
          അക്ഷരം fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  ചെയ്യുക                                                                                    \
    {                                                                                   \
      എങ്കിൽ (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, വലുപ്പം (b), "%" PRIi64, x);                                \
          എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (b)))                                       \
            തിരികെ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      അല്ലെങ്കിൽ                                                                              \
        strcpy (b, "*");                                                                \
  } ആകുമ്പോൾ (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          എങ്കിൽ (UNLIKELY (len >= FMT_BUF_LEN))
            തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  വേണ്ടി (i = 0; default_devices[i].type; i++)
    {
      അക്ഷരം fmt_buf_major[16];
      അക്ഷരം fmt_buf_minor[16];
      അക്ഷരം device[64];
      പൂർണ്ണസംഖ്യ len;

#define FMT_DEV(x, b)                                                                   \
  ചെയ്യുക                                                                                    \
    {                                                                                   \
      എങ്കിൽ (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, വലുപ്പം (b), "%d", x);                                      \
          എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (b)))                                       \
            തിരികെ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      അല്ലെങ്കിൽ                                                                              \
        strcpy (b, "*");                                                                \
  } ആകുമ്പോൾ (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, വലുപ്പം (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (device)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

ഘടന bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ i;
  ഘടന bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  എങ്കിൽ (UNLIKELY (program == ശൂന്യ))
    തിരികെ ശൂന്യ;

  വേണ്ടി (i = (വലുപ്പം (default_devices) / വലുപ്പം (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      എങ്കിൽ (UNLIKELY (program == ശൂന്യ))
        തിരികെ ശൂന്യ;
    }
  വേണ്ടി (i = devs_len - 1; i >= 0; i--)
    {
      അക്ഷരം type = 'a';
      പൂർണ്ണസംഖ്യ minor = -1, major = -1;
      എങ്കിൽ (devs[i]->type != ശൂന്യ)
        type = devs[i]->type[0];

      എങ്കിൽ (devs[i]->major_present)
        major = devs[i]->major;
      എങ്കിൽ (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      എങ്കിൽ (UNLIKELY (program == ശൂന്യ))
        തിരികെ ശൂന്യ;
    }

  program = bpf_program_complete_dev (program, err);
  എങ്കിൽ (UNLIKELY (program == ശൂന്യ))
    തിരികെ ശൂന്യ;

  തിരികെ program;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_devices_resources_v2_internal (പൂർണ്ണസംഖ്യ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free ഘടന bpf_program *program = ശൂന്യ;

  program = create_dev_bpf (devs, devs_len, err);
  എങ്കിൽ (UNLIKELY (program == ശൂന്യ))
    തിരികെ -1;

  തിരികെ libcrun_ebpf_load (program, dirfd, ശൂന്യ, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_devices_resources_v2 (പൂർണ്ണസംഖ്യ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  എങ്കിൽ (LIKELY (ret == 0))
    തിരികെ 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  വേണ്ടി (i = 0; i < devs_len; i++)
    {
      എങ്കിൽ (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          നിർത്തുക;
        }
    }

  എങ്കിൽ (! can_skip)
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      പൂർണ്ണസംഖ്യ rootless;

      rootless = is_rootless (&tmp_err);
      എങ്കിൽ (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          തിരികെ ret;
        }
      എങ്കിൽ (rootless)
        can_skip = true;
    }

  എങ്കിൽ (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_devices_resources (പൂർണ്ണസംഖ്യ dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  അല്ലെങ്കിൽ
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      പൂർണ്ണസംഖ്യ rootless;

      rootless = is_rootless (&tmp_err);
      എങ്കിൽ (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          തിരികെ ret;
        }

      എങ്കിൽ (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  തിരികെ ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
സ്ഥിതം പൂർണ്ണസംഖ്യ
cg_itoa (അക്ഷരം *buf, size_t size, int64_t മൂല്യം, bool use_max, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ len;

  എങ്കിൽ (use_max && മൂല്യം < 0)
    len = snprintf (buf, size, "max");
  അല്ലെങ്കിൽ
    len = snprintf (buf, size, "%" PRIi64, മൂല്യം);

  എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) size))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

  തിരികെ len;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_memory (പൂർണ്ണസംഖ്യ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  അക്ഷരം limit_buf[32];
  പൂർണ്ണസംഖ്യ limit_buf_len;

  എങ്കിൽ (! memory->limit_present)
    തിരികെ 0;

  limit_buf_len = cg_itoa (limit_buf, വലുപ്പം (limit_buf), memory->limit, cgroup2, err);
  എങ്കിൽ (UNLIKELY (limit_buf_len < 0))
    തിരികെ limit_buf_len;

  തിരികെ write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_memory_swap (പൂർണ്ണസംഖ്യ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  int64_t swap;
  അക്ഷരം swap_buf[32];
  പൂർണ്ണസംഖ്യ len;
  സ്ഥിരം അക്ഷരം *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  എങ്കിൽ (! memory->swap_present)
    തിരികെ 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  എങ്കിൽ (cgroup2 && memory->swap > 0)
    {
      എങ്കിൽ (! memory->limit_present)
        തിരികെ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      എങ്കിൽ (memory->swap < memory->limit)
        തിരികെ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, വലുപ്പം (swap_buf), swap, cgroup2, err);
  എങ്കിൽ (UNLIKELY (len < 0))
    തിരികെ len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  എങ്കിൽ (ret >= 0)
    തിരികെ ret;

  /* If swap is not enabled, ignore the error.  */
  എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      തിരികെ 0;
    }

  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_memory_resources (പൂർണ്ണസംഖ്യ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ len;
  പൂർണ്ണസംഖ്യ ret;
  അക്ഷരം fmt_buf[32];
  bool memory_limits_written = false;

  എങ്കിൽ (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      എങ്കിൽ (ret >= 0)
        memory_limits_written = true;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (cgroup2 || crun_error_get_errno (err) != EINVAL)
            തിരികെ ret;

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
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (memory->kernel_present)
    {
      എങ്കിൽ (cgroup2)
        തിരികെ crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64, memory->kernel);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  എങ്കിൽ (memory->use_hierarchy_present)
    {
      എങ്കിൽ (cgroup2)
        തിരികെ crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (memory->reservation_present)
    {
      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64, memory->reservation);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 ശൂന്യ, fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  എങ്കിൽ (memory->disable_oom_killer)
    {
      എങ്കിൽ (cgroup2)
        തിരികെ crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  എങ്കിൽ (memory->kernel_tcp_present)
    {
      എങ്കിൽ (cgroup2)
        തിരികെ crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  എങ്കിൽ (memory->swappiness_present)
    {
      എങ്കിൽ (cgroup2)
        തിരികെ crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64, memory->swappiness);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
write_cpu_idle (പൂർണ്ണസംഖ്യ cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  അക്ഷരം fmt_buf[32];
  പൂർണ്ണസംഖ്യ len;

  എങ്കിൽ (! cpu->idle_present)
    തിരികെ 0;

  len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIi64, cpu->idle);
  എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
  തിരികെ write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

പൂർണ്ണസംഖ്യ
write_cpu_burst (പൂർണ്ണസംഖ്യ cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  അക്ഷരം fmt_buf[32];
  പൂർണ്ണസംഖ്യ len;

  എങ്കിൽ (! cpu->burst_present)
    തിരികെ 0;

  len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIi64, cpu->burst);
  എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
  തിരികെ write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_pids_resources (പൂർണ്ണസംഖ്യ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  എങ്കിൽ (pids->limit)
    {
      അക്ഷരം fmt_buf[32];
      പൂർണ്ണസംഖ്യ len;
      പൂർണ്ണസംഖ്യ ret;

      len = cg_itoa (fmt_buf, വലുപ്പം (fmt_buf), pids->limit, true, err);
      എങ്കിൽ (UNLIKELY (len < 0))
        തിരികെ len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", ശൂന്യ, fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_cpu_resources (പൂർണ്ണസംഖ്യ dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ len, period_len = 0;
  പൂർണ്ണസംഖ്യ ret;
  അക്ഷരം fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free അക്ഷരം *period_str = ശൂന്യ;

  എങ്കിൽ (cpu->shares)
    {
      uint32_t val = cpu->shares;

      എങ്കിൽ (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          എങ്കിൽ (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%u", val);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 ശൂന്യ, fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  എങ്കിൽ (cpu->period)
    {
      എങ്കിൽ (cgroup2)
        period = cpu->period;
      അല്ലെങ്കിൽ
        {
          len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64, cpu->period);
          എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
            തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              എങ്കിൽ (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                തിരികെ ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  എങ്കിൽ (cpu->quota)
    {
      എങ്കിൽ (cgroup2)
        quota = cpu->quota;
      അല്ലെങ്കിൽ
        {
          len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIi64, cpu->quota);
          എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
            തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          എങ്കിൽ (period_str != ശൂന്യ)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;
            }
        }
    }
  എങ്കിൽ (cpu->realtime_period)
    {
      എങ്കിൽ (cgroup2)
        തിരികെ crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64, cpu->realtime_period);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  എങ്കിൽ (cpu->realtime_runtime)
    {
      എങ്കിൽ (cgroup2)
        തിരികെ crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (cgroup2 && (quota > 0 || period > 0))
    {
      എങ്കിൽ (period < 0)
        period = 100000;
      എങ്കിൽ (quota < 0)
        len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "max %" PRIi64, period);
      അല്ലെങ്കിൽ
        len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", ശൂന്യ, fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

പൂർണ്ണസംഖ്യ
write_cpuset_resources (പൂർണ്ണസംഖ്യ dirfd_cpuset, പൂർണ്ണസംഖ്യ cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (cpu == ശൂന്യ)
    തിരികെ 0;

  എങ്കിൽ (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  എങ്കിൽ (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
open_cgroup_subsystem (സ്ഥിരം അക്ഷരം *subsystem, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *full_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_വേര്, subsystem, path, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (dirfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", full_path);

  തിരികെ dirfd;
}

പൂർണ്ണസംഖ്യ
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (resources->block_io)
    {
      cleanup_close പൂർണ്ണസംഖ്യ dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      എങ്കിൽ (UNLIKELY (dirfd_blkio < 0))
        തിരികെ dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (resources->network)
    {
      cleanup_close പൂർണ്ണസംഖ്യ dirfd_netclass = -1;
      cleanup_close പൂർണ്ണസംഖ്യ dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      എങ്കിൽ (UNLIKELY (dirfd_netclass < 0))
        തിരികെ dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      എങ്കിൽ (UNLIKELY (dirfd_netprio < 0))
        തിരികെ dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (resources->hugepage_limits_len)
    {
      cleanup_close പൂർണ്ണസംഖ്യ dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      എങ്കിൽ (UNLIKELY (dirfd_htlb < 0))
        തിരികെ dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (resources->devices_len)
    {
      cleanup_close പൂർണ്ണസംഖ്യ dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      എങ്കിൽ (UNLIKELY (dirfd_devs < 0))
        തിരികെ dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (resources->memory)
    {
      cleanup_close പൂർണ്ണസംഖ്യ dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      എങ്കിൽ (UNLIKELY (dirfd_mem < 0))
        തിരികെ dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (resources->pids)
    {
      cleanup_close പൂർണ്ണസംഖ്യ dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      എങ്കിൽ (UNLIKELY (dirfd_pid < 0))
        തിരികെ dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (resources->cpu)
    {
      cleanup_close പൂർണ്ണസംഖ്യ dirfd_cpu = -1;
      cleanup_close പൂർണ്ണസംഖ്യ dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      എങ്കിൽ (UNLIKELY (dirfd_cpu < 0))
        തിരികെ dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      എങ്കിൽ (resources->cpu->cpus == ശൂന്യ && resources->cpu->mems == ശൂന്യ)
        തിരികെ 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      എങ്കിൽ (UNLIKELY (dirfd_cpuset < 0))
        തിരികെ dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (resources->unified && resources->unified->len > 0)
    തിരികെ crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_unified_resources (പൂർണ്ണസംഖ്യ cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  വേണ്ടി (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
      cleanup_free അക്ഷരം *മൂല്യം = ശൂന്യ;
      അക്ഷരം *saveptr = ശൂന്യ;
      അക്ഷരം *വരി;

      എങ്കിൽ (strchr (resources->unified->താക്കോലുകൾ[i], '/'))
        തിരികെ crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->താക്കോലുകൾ[i]);

      എങ്കിൽ (is_empty_string (resources->unified->മൂല്യങ്ങൾ[i]))
        തുടരുക;

      മൂല്യം = xstrdup (resources->unified->മൂല്യങ്ങൾ[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->താക്കോലുകൾ[i], O_WRONLY, err);
      എങ്കിൽ (UNLIKELY (fd < 0))
        തിരികെ fd;

      വേണ്ടി (വരി = strtok_r (മൂല്യം, "\n", &saveptr); വരി; വരി = strtok_r (ശൂന്യ, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (ലിഖ (fd, വരി, strlen (വരി)));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "write to `%s`", resources->unified->താക്കോലുകൾ[i]);
        }
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, സ്ഥിരം അക്ഷരം *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ cgroup_dirfd = -1;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (resources->network)
    തിരികെ crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_വേര്, path, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (cgroup_dirfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", cgroup_path);

  എങ്കിൽ (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  എങ്കിൽ (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  എങ്കിൽ (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  എങ്കിൽ (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  എങ്കിൽ (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
പൂർണ്ണസംഖ്യ
check_memory_before_update (സ്ഥിരം അക്ഷരം *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free അക്ഷരം *swap_current = ശൂന്യ;
  cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
  cleanup_free അക്ഷരം *current = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (path == ശൂന്യ || memory == ശൂന്യ || ! (memory->check_before_update_present && memory->check_before_update))
    തിരികെ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  എങ്കിൽ (cgroup_mode != CGROUP_MODE_UNIFIED)
    തിരികെ 0;

  ret = append_paths (&cgroup_path, err, CGROUP_വേര്, path, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (dirfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  errno = 0;
  val = strtoll (current, ശൂന്യ, 10);
  എങ്കിൽ (UNLIKELY (errno))
    തിരികെ crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, ശൂന്യ, 10);
  എങ്കിൽ (UNLIKELY (errno))
    തിരികെ crun_make_error (err, errno, "parse memory.swap.current");

  എങ്കിൽ (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  എങ്കിൽ (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  എങ്കിൽ (limit <= val + val_swap)
    തിരികെ crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
update_cgroup_resources (സ്ഥിരം അക്ഷരം *path,
                         സ്ഥിരം അക്ഷരം *state_വേര്,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ cgroup_mode;

  (ശൂന്യം) state_വേര്;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  എങ്കിൽ (path == ശൂന്യ)
    {
      size_t i;

      എങ്കിൽ (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        തിരികെ crun_make_error (err, 0, "cannot set limits without cgroups");

      വേണ്ടി (i = 0; i < resources->devices_len; i++)
        {
          പൂർണ്ണസംഖ്യ rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          എങ്കിൽ (UNLIKELY (rwm < 0))
            തിരികെ rwm;

          എങ്കിൽ (rwm == 0)
            തിരികെ crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      തിരികെ 0;
    }
  തിരഞ്ഞെടുപ്പ് (cgroup_mode)
    {
    അവസ്ഥ CGROUP_MODE_UNIFIED:
      തിരികെ update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    അവസ്ഥ CGROUP_MODE_LEGACY:
    അവസ്ഥ CGROUP_MODE_HYBRID:
      തിരികെ update_cgroup_v1_resources (resources, path, err);

    സ്ഥിരസ്ഥിതി:
      തിരികെ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
