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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "cgroup.h"
#অন্তর্ভাব "cgroup-internal.h"
#অন্তর্ভাব "cgroup-systemd.h"
#অন্তর্ভাব "cgroup-utils.h"
#অন্তর্ভাব "cgroup-resources.h"
#অন্তর্ভাব "ebpf.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "status.h"
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/vfs.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব <time.h>

#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <libgen.h>

সংরচনা default_dev_s default_devices[] = {
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
  { 0, 0, 0, রিক্ত }
};

সংরচনা default_dev_s *
get_default_devices ()
{
  প্রত্যাগম default_devices;
}

স্থির পূর্ণাঙ্ক
write_cgroup_file_or_alias (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *alias, ধ্রুব শূন্য *data, size_t len, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  যদি (UNLIKELY (alias != রিক্ত && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  প্রত্যাগম ret;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
openat_with_alias (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *alias, ধ্রুব বর্ণ **used_name, পূর্ণাঙ্ক flags, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  যদি (UNLIKELY (ret < 0 && alias != রিক্ত && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", name);
  প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক
is_rwm (ধ্রুব বর্ণ *str, libcrun_error_t *err)
{
  ধ্রুব বর্ণ *it;
  bool r = false;
  bool w = false;
  bool m = false;

  যদি (str == রিক্ত)
    str = "";

  প্রতি (it = str; *it; it++)
    বিকল্প (*it)
      {
      পক্ষ 'r':
        r = true;
        ভঙ্গ;

      পক্ষ 'w':
        w = true;
        ভঙ্গ;

      পক্ষ 'm':
        m = true;
        ভঙ্গ;

      স্বভাব:
        প্রত্যাগম crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  প্রত্যাগম r && w && m ? 1 : 0;
}

স্থির পূর্ণাঙ্ক
check_cgroup_v2_controller_available_wrapper (পূর্ণাঙ্ক ret, পূর্ণাঙ্ক cgroup_dirfd, ধ্রুব বর্ণ *name, libcrun_error_t *err)
{
  যদি (ret == 0 || err == রিক্ত)
    প্রত্যাগম 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  যদি (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free বর্ণ *controllers = রিক্ত;
      libcrun_error_t tmp_err = রিক্ত;
      cleanup_free বর্ণ *চাবি = রিক্ত;
      বর্ণ *saveptr = রিক্ত;
      bool found = false;
      ধ্রুব বর্ণ *token;
      বর্ণ *it;

      /* Check if the specified controller is enabled.  */
      চাবি = xstrdup (name);

      it = strchr (চাবি, '.');
      যদি (it == রিক্ত)
        {
          crun_error_release (err);
          প্রত্যাগম crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      যদি (strcmp (চাবি, "cgroup") == 0)
        প্রত্যাগম ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      যদি (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, রিক্ত, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          প্রত্যাগম ret;
        }
      প্রতি (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (রিক্ত, " \n", &saveptr))
        {
          যদি (strcmp (token, চাবি) == 0)
            {
              found = true;
              ভঙ্গ;
            }
        }
      যদি (! found)
        {
          cleanup_free বর্ণ *absolute_path = রিক্ত;
          libcrun_error_t tmp_err = রিক্ত;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          যদি (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", চাবি, absolute_path);
          অন্যথা
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", চাবি);
            }
        }
    }
  প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক
write_file_and_check_controllers_at (bool cgroup2, পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *name_alias,
                                     ধ্রুব শূন্য *data, size_t len, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  যদি (cgroup2)
    প্রত্যাগম check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক
open_file_and_check_controllers_at (bool cgroup2, পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *name, পূর্ণাঙ্ক flags, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = openat (dirfd, name, flags);
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      যদি (cgroup2)
        প্রত্যাগম check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  প্রত্যাগম ret;
}

/* The parser generates different structs but they are really all the same.  */
প্রকারনির্দেশ runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

স্থির পূর্ণাঙ্ক
write_blkio_v1_resources_throttling (পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  বর্ণ fmt_buf[128];
  size_t i;
  cleanup_close পূর্ণাঙ্ক fd = -1;

  যদি (throttling == রিক্ত)
    প্রত্যাগম 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", name);

  প্রতি (i = 0; i < throttling_len; i++)
    {
      পূর্ণাঙ্ক ret;
      পূর্ণাঙ্ক len;
      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (লিখ (fd, fmt_buf, len));
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "write `%s`", name);
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
write_blkio_v2_resources_throttling (পূর্ণাঙ্ক fd, ধ্রুব বর্ণ *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  বর্ণ fmt_buf[128];
  size_t i;

  যদি (throttling == রিক্ত)
    প্রত্যাগম 0;

  প্রতি (i = 0; i < throttling_len; i++)
    {
      পূর্ণাঙ্ক ret;
      পূর্ণাঙ্ক len;
      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (লিখ (fd, fmt_buf, len));
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "write `%s`", name);
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
write_blkio_resources (পূর্ণাঙ্ক dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  বর্ণ fmt_buf[128];
  পূর্ণাঙ্ক len;
  পূর্ণাঙ্ক ret;

  যদি (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu32, val);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
      যদি (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }
      অন্যথা
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          যদি (UNLIKELY (ret < 0))
            {
              যদি (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu32, val);
                  যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
                    প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              যদি (UNLIKELY (ret < 0))
                প্রত্যাগম ret;
            }
        }
    }
  যদি (blkio->leaf_weight)
    {
      যদি (cgroup2)
        প্রত্যাগম crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%d", blkio->leaf_weight);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  যদি (blkio->weight_device_len)
    {
      যদি (cgroup2)
        {
          cleanup_close পূর্ণাঙ্ক wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          যদি (UNLIKELY (wfd < 0))
            প্রত্যাগম crun_make_error (err, errno, "open `io.bfq.weight`");
          প্রতি (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
                প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (লিখ (wfd, fmt_buf, len));
              যদি (UNLIKELY (ret < 0))
                প্রত্যাগম crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      অন্যথা
        {
          ধ্রুব বর্ণ *leaf_weight_device_file_name = রিক্ত;
          ধ্রুব বর্ণ *weight_device_file_name = রিক্ত;
          cleanup_close পূর্ণাঙ্ক w_leafdevice_fd = -1;
          cleanup_close পূর্ণাঙ্ক w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          যদি (UNLIKELY (w_device_fd < 0))
            প্রত্যাগম w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          যদি (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          প্রতি (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
                প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (লিখ (w_device_fd, fmt_buf, len));
              যদি (UNLIKELY (ret < 0))
                প্রত্যাগম crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              যদি (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
                    প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (লিখ (w_leafdevice_fd, fmt_buf, len));
                  যদি (UNLIKELY (ret < 0))
                    প্রত্যাগম crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  যদি (cgroup2)
    {
      cleanup_close পূর্ণাঙ্ক wfd = -1;
      ধ্রুব বর্ণ *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      যদি (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          প্রত্যাগম check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  অন্যথা
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
write_network_resources (পূর্ণাঙ্ক dirfd_netclass, পূর্ণাঙ্ক dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  বর্ণ fmt_buf[128];
  পূর্ণাঙ্ক len;
  পূর্ণাঙ্ক ret;
  যদি (net->class_id)
    {
      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu32, net->class_id);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  যদি (net->priorities_len)
    {
      size_t i;
      cleanup_close পূর্ণাঙ্ক fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      যদি (UNLIKELY (fd < 0))
        প্রত্যাগম crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      প্রতি (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
            প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (লিখ (fd, fmt_buf, len));
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
write_hugetlb_resources (পূর্ণাঙ্ক dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  ধ্রুব বর্ণ *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  বর্ণ fmt_buf[128];
  size_t i;
  প্রতি (i = 0; i < htlb_len; i++)
    {
      cleanup_free বর্ণ *filename = রিক্ত;
      cleanup_free বর্ণ *rsvd_filename = রিক্ত;
      পূর্ণাঙ্ক len;
      পূর্ণাঙ্ক ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64, htlb[i]->limit);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, রিক্ত, fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      যদি (skip_rsvd)
        অনুবর্ত;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (crun_error_get_errno (err) != ENOENT)
            প্রত্যাগম ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
write_devices_resources_v1 (পূর্ণাঙ্ক dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  পূর্ণাঙ্ক len;
  পূর্ণাঙ্ক ret;

  প্রতি (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      বর্ণ fmt_buf[FMT_BUF_LEN];
      ধ্রুব বর্ণ *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      যদি (devs[i]->type == রিক্ত || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      অন্যথা
        {
          বর্ণ fmt_buf_major[16];
          বর্ণ fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  কুরু                                                                                    \
    {                                                                                   \
      যদি (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, পরিমাণ (b), "%" PRIi64, x);                                \
          যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (b)))                                       \
            প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      অন্যথা                                                                              \
        strcpy (b, "*");                                                                \
  } যাবত্ (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          যদি (UNLIKELY (len >= FMT_BUF_LEN))
            প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  প্রতি (i = 0; default_devices[i].type; i++)
    {
      বর্ণ fmt_buf_major[16];
      বর্ণ fmt_buf_minor[16];
      বর্ণ device[64];
      পূর্ণাঙ্ক len;

#define FMT_DEV(x, b)                                                                   \
  কুরু                                                                                    \
    {                                                                                   \
      যদি (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, পরিমাণ (b), "%d", x);                                      \
          যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (b)))                                       \
            প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      অন্যথা                                                                              \
        strcpy (b, "*");                                                                \
  } যাবত্ (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, পরিমাণ (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (device)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  প্রত্যাগম 0;
}

সংরচনা bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  পূর্ণাঙ্ক i;
  সংরচনা bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  যদি (UNLIKELY (program == রিক্ত))
    প্রত্যাগম রিক্ত;

  প্রতি (i = (পরিমাণ (default_devices) / পরিমাণ (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      যদি (UNLIKELY (program == রিক্ত))
        প্রত্যাগম রিক্ত;
    }
  প্রতি (i = devs_len - 1; i >= 0; i--)
    {
      বর্ণ type = 'a';
      পূর্ণাঙ্ক minor = -1, major = -1;
      যদি (devs[i]->type != রিক্ত)
        type = devs[i]->type[0];

      যদি (devs[i]->major_present)
        major = devs[i]->major;
      যদি (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      যদি (UNLIKELY (program == রিক্ত))
        প্রত্যাগম রিক্ত;
    }

  program = bpf_program_complete_dev (program, err);
  যদি (UNLIKELY (program == রিক্ত))
    প্রত্যাগম রিক্ত;

  প্রত্যাগম program;
}

স্থির পূর্ণাঙ্ক
write_devices_resources_v2_internal (পূর্ণাঙ্ক dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free সংরচনা bpf_program *program = রিক্ত;

  program = create_dev_bpf (devs, devs_len, err);
  যদি (UNLIKELY (program == রিক্ত))
    প্রত্যাগম -1;

  প্রত্যাগম libcrun_ebpf_load (program, dirfd, রিক্ত, err);
}

স্থির পূর্ণাঙ্ক
write_devices_resources_v2 (পূর্ণাঙ্ক dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  যদি (LIKELY (ret == 0))
    প্রত্যাগম 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  প্রতি (i = 0; i < devs_len; i++)
    {
      যদি (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          ভঙ্গ;
        }
    }

  যদি (! can_skip)
    {
      libcrun_error_t tmp_err = রিক্ত;
      পূর্ণাঙ্ক rootless;

      rootless = is_rootless (&tmp_err);
      যদি (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          প্রত্যাগম ret;
        }
      যদি (rootless)
        can_skip = true;
    }

  যদি (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক
write_devices_resources (পূর্ণাঙ্ক dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  যদি (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  অন্যথা
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  যদি (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = রিক্ত;
      পূর্ণাঙ্ক rootless;

      rootless = is_rootless (&tmp_err);
      যদি (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          প্রত্যাগম ret;
        }

      যদি (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  প্রত্যাগম ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
স্থির পূর্ণাঙ্ক
cg_itoa (বর্ণ *buf, size_t size, int64_t মান, bool use_max, libcrun_error_t *err)
{
  পূর্ণাঙ্ক len;

  যদি (use_max && মান < 0)
    len = snprintf (buf, size, "max");
  অন্যথা
    len = snprintf (buf, size, "%" PRIi64, মান);

  যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) size))
    প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

  প্রত্যাগম len;
}

স্থির পূর্ণাঙ্ক
write_memory (পূর্ণাঙ্ক dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  বর্ণ limit_buf[32];
  পূর্ণাঙ্ক limit_buf_len;

  যদি (! memory->limit_present)
    প্রত্যাগম 0;

  limit_buf_len = cg_itoa (limit_buf, পরিমাণ (limit_buf), memory->limit, cgroup2, err);
  যদি (UNLIKELY (limit_buf_len < 0))
    প্রত্যাগম limit_buf_len;

  প্রত্যাগম write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

স্থির পূর্ণাঙ্ক
write_memory_swap (পূর্ণাঙ্ক dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  int64_t swap;
  বর্ণ swap_buf[32];
  পূর্ণাঙ্ক len;
  ধ্রুব বর্ণ *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  যদি (! memory->swap_present)
    প্রত্যাগম 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  যদি (cgroup2 && memory->swap > 0)
    {
      যদি (! memory->limit_present)
        প্রত্যাগম crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      যদি (memory->swap < memory->limit)
        প্রত্যাগম crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, পরিমাণ (swap_buf), swap, cgroup2, err);
  যদি (UNLIKELY (len < 0))
    প্রত্যাগম len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  যদি (ret >= 0)
    প্রত্যাগম ret;

  /* If swap is not enabled, ignore the error.  */
  যদি (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      প্রত্যাগম 0;
    }

  প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক
write_memory_resources (পূর্ণাঙ্ক dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  পূর্ণাঙ্ক len;
  পূর্ণাঙ্ক ret;
  বর্ণ fmt_buf[32];
  bool memory_limits_written = false;

  যদি (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      যদি (ret >= 0)
        memory_limits_written = true;
      অন্যথা
        {
          যদি (cgroup2 || crun_error_get_errno (err) != EINVAL)
            প্রত্যাগম ret;

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
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (memory->kernel_present)
    {
      যদি (cgroup2)
        প্রত্যাগম crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64, memory->kernel);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  যদি (memory->use_hierarchy_present)
    {
      যদি (cgroup2)
        প্রত্যাগম crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (memory->reservation_present)
    {
      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64, memory->reservation);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 রিক্ত, fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  যদি (memory->disable_oom_killer)
    {
      যদি (cgroup2)
        প্রত্যাগম crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  যদি (memory->kernel_tcp_present)
    {
      যদি (cgroup2)
        প্রত্যাগম crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  যদি (memory->swappiness_present)
    {
      যদি (cgroup2)
        প্রত্যাগম crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64, memory->swappiness);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
write_cpu_idle (পূর্ণাঙ্ক cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  বর্ণ fmt_buf[32];
  পূর্ণাঙ্ক len;

  যদি (! cpu->idle_present)
    প্রত্যাগম 0;

  len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIi64, cpu->idle);
  যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
    প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
  প্রত্যাগম write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

পূর্ণাঙ্ক
write_cpu_burst (পূর্ণাঙ্ক cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  বর্ণ fmt_buf[32];
  পূর্ণাঙ্ক len;

  যদি (! cpu->burst_present)
    প্রত্যাগম 0;

  len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIi64, cpu->burst);
  যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
    প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
  প্রত্যাগম write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

স্থির পূর্ণাঙ্ক
write_pids_resources (পূর্ণাঙ্ক dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  যদি (pids->limit)
    {
      বর্ণ fmt_buf[32];
      পূর্ণাঙ্ক len;
      পূর্ণাঙ্ক ret;

      len = cg_itoa (fmt_buf, পরিমাণ (fmt_buf), pids->limit, true, err);
      যদি (UNLIKELY (len < 0))
        প্রত্যাগম len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", রিক্ত, fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
write_cpu_resources (পূর্ণাঙ্ক dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  পূর্ণাঙ্ক len, period_len = 0;
  পূর্ণাঙ্ক ret;
  বর্ণ fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free বর্ণ *period_str = রিক্ত;

  যদি (cpu->shares)
    {
      uint32_t val = cpu->shares;

      যদি (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          যদি (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              যদি (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%u", val);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 রিক্ত, fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  যদি (cpu->period)
    {
      যদি (cgroup2)
        period = cpu->period;
      অন্যথা
        {
          len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64, cpu->period);
          যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
            প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          যদি (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              যদি (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                প্রত্যাগম ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  যদি (cpu->quota)
    {
      যদি (cgroup2)
        quota = cpu->quota;
      অন্যথা
        {
          len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIi64, cpu->quota);
          যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
            প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          যদি (period_str != রিক্ত)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              যদি (UNLIKELY (ret < 0))
                প্রত্যাগম ret;
            }
        }
    }
  যদি (cpu->realtime_period)
    {
      যদি (cgroup2)
        প্রত্যাগম crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64, cpu->realtime_period);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  যদি (cpu->realtime_runtime)
    {
      যদি (cgroup2)
        প্রত্যাগম crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (cgroup2 && (quota > 0 || period > 0))
    {
      যদি (period < 0)
        period = 100000;
      যদি (quota < 0)
        len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "max %" PRIi64, period);
      অন্যথা
        len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", রিক্ত, fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  প্রত্যাগম write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

পূর্ণাঙ্ক
write_cpuset_resources (পূর্ণাঙ্ক dirfd_cpuset, পূর্ণাঙ্ক cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  যদি (cpu == রিক্ত)
    প্রত্যাগম 0;

  যদি (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  যদি (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
open_cgroup_subsystem (ধ্রুব বর্ণ *subsystem, ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  cleanup_free বর্ণ *full_path = রিক্ত;
  পূর্ণাঙ্ক ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_মূল, subsystem, path, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (dirfd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", full_path);

  প্রত্যাগম dirfd;
}

পূর্ণাঙ্ক
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  যদি (resources->block_io)
    {
      cleanup_close পূর্ণাঙ্ক dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      যদি (UNLIKELY (dirfd_blkio < 0))
        প্রত্যাগম dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (resources->network)
    {
      cleanup_close পূর্ণাঙ্ক dirfd_netclass = -1;
      cleanup_close পূর্ণাঙ্ক dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      যদি (UNLIKELY (dirfd_netclass < 0))
        প্রত্যাগম dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      যদি (UNLIKELY (dirfd_netprio < 0))
        প্রত্যাগম dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (resources->hugepage_limits_len)
    {
      cleanup_close পূর্ণাঙ্ক dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      যদি (UNLIKELY (dirfd_htlb < 0))
        প্রত্যাগম dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (resources->devices_len)
    {
      cleanup_close পূর্ণাঙ্ক dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      যদি (UNLIKELY (dirfd_devs < 0))
        প্রত্যাগম dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (resources->memory)
    {
      cleanup_close পূর্ণাঙ্ক dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      যদি (UNLIKELY (dirfd_mem < 0))
        প্রত্যাগম dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (resources->pids)
    {
      cleanup_close পূর্ণাঙ্ক dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      যদি (UNLIKELY (dirfd_pid < 0))
        প্রত্যাগম dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (resources->cpu)
    {
      cleanup_close পূর্ণাঙ্ক dirfd_cpu = -1;
      cleanup_close পূর্ণাঙ্ক dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      যদি (UNLIKELY (dirfd_cpu < 0))
        প্রত্যাগম dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      যদি (resources->cpu->cpus == রিক্ত && resources->cpu->mems == রিক্ত)
        প্রত্যাগম 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      যদি (UNLIKELY (dirfd_cpuset < 0))
        প্রত্যাগম dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (resources->unified && resources->unified->len > 0)
    প্রত্যাগম crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
write_unified_resources (পূর্ণাঙ্ক cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  পূর্ণাঙ্ক ret;

  প্রতি (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close পূর্ণাঙ্ক fd = -1;
      cleanup_free বর্ণ *মান = রিক্ত;
      বর্ণ *saveptr = রিক্ত;
      বর্ণ *সারি;

      যদি (strchr (resources->unified->চাবিগুলি[i], '/'))
        প্রত্যাগম crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->চাবিগুলি[i]);

      যদি (is_empty_string (resources->unified->মানসমূহ[i]))
        অনুবর্ত;

      মান = xstrdup (resources->unified->মানসমূহ[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->চাবিগুলি[i], O_WRONLY, err);
      যদি (UNLIKELY (fd < 0))
        প্রত্যাগম fd;

      প্রতি (সারি = strtok_r (মান, "\n", &saveptr); সারি; সারি = strtok_r (রিক্ত, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (লিখ (fd, সারি, strlen (সারি)));
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "write to `%s`", resources->unified->চাবিগুলি[i]);
        }
    }

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, ধ্রুব বর্ণ *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free বর্ণ *cgroup_path = রিক্ত;
  cleanup_close পূর্ণাঙ্ক cgroup_dirfd = -1;
  পূর্ণাঙ্ক ret;

  যদি (resources->network)
    প্রত্যাগম crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_মূল, path, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (cgroup_dirfd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", cgroup_path);

  যদি (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  যদি (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  যদি (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  যদি (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  যদি (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  প্রত্যাগম 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
পূর্ণাঙ্ক
check_memory_before_update (ধ্রুব বর্ণ *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free বর্ণ *swap_current = রিক্ত;
  cleanup_free বর্ণ *cgroup_path = রিক্ত;
  cleanup_free বর্ণ *current = রিক্ত;
  cleanup_close পূর্ণাঙ্ক dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক ret;

  যদি (path == রিক্ত || memory == রিক্ত || ! (memory->check_before_update_present && memory->check_before_update))
    প্রত্যাগম 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    প্রত্যাগম 0;

  ret = append_paths (&cgroup_path, err, CGROUP_মূল, path, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (dirfd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  errno = 0;
  val = strtoll (current, রিক্ত, 10);
  যদি (UNLIKELY (errno))
    প্রত্যাগম crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, রিক্ত, 10);
  যদি (UNLIKELY (errno))
    প্রত্যাগম crun_make_error (err, errno, "parse memory.swap.current");

  যদি (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  যদি (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  যদি (limit <= val + val_swap)
    প্রত্যাগম crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
update_cgroup_resources (ধ্রুব বর্ণ *path,
                         ধ্রুব বর্ণ *state_মূল,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  পূর্ণাঙ্ক cgroup_mode;

  (শূন্য) state_মূল;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  যদি (path == রিক্ত)
    {
      size_t i;

      যদি (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        প্রত্যাগম crun_make_error (err, 0, "cannot set limits without cgroups");

      প্রতি (i = 0; i < resources->devices_len; i++)
        {
          পূর্ণাঙ্ক rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          যদি (UNLIKELY (rwm < 0))
            প্রত্যাগম rwm;

          যদি (rwm == 0)
            প্রত্যাগম crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      প্রত্যাগম 0;
    }
  বিকল্প (cgroup_mode)
    {
    পক্ষ CGROUP_MODE_UNIFIED:
      প্রত্যাগম update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    পক্ষ CGROUP_MODE_LEGACY:
    পক্ষ CGROUP_MODE_HYBRID:
      প্রত্যাগম update_cgroup_v1_resources (resources, path, err);

    স্বভাব:
      প্রত্যাগম crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
