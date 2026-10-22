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

#شامل <config.h>
#شامل "cgroup.h"
#شامل "cgroup-internal.h"
#شامل "cgroup-systemd.h"
#شامل "cgroup-utils.h"
#شامل "cgroup-resources.h"
#شامل "ebpf.h"
#شامل "utils.h"
#شامل "status.h"
#شامل <string.h>
#شامل <sys/types.h>
#شامل <sys/vfs.h>
#شامل <inttypes.h>
#شامل <time.h>

#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <libgen.h>

ساختار default_dev_s default_devices[] = {
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
  { 0, 0, 0, پوچ }
};

ساختار default_dev_s *
get_default_devices ()
{
  بازگشت default_devices;
}

ایستا صحیح
write_cgroup_file_or_alias (صحیح dirfd, ثابت نویسه *name, ثابت نویسه *alias, ثابت تهی *data, size_t len, libcrun_error_t *err)
{
  صحیح ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  اگر (UNLIKELY (alias != پوچ && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  بازگشت ret;
}

ایستا درونخطی صحیح
openat_with_alias (صحیح dirfd, ثابت نویسه *name, ثابت نویسه *alias, ثابت نویسه **used_name, صحیح flags, libcrun_error_t *err)
{
  صحیح ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  اگر (UNLIKELY (ret < 0 && alias != پوچ && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", name);
  بازگشت ret;
}

ایستا صحیح
is_rwm (ثابت نویسه *str, libcrun_error_t *err)
{
  ثابت نویسه *it;
  bool r = false;
  bool w = false;
  bool m = false;

  اگر (str == پوچ)
    str = "";

  برای (it = str; *it; it++)
    انتخاب (*it)
      {
      حالت 'r':
        r = true;
        بشکن;

      حالت 'w':
        w = true;
        بشکن;

      حالت 'm':
        m = true;
        بشکن;

      پیشفرض:
        بازگشت crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  بازگشت r && w && m ? 1 : 0;
}

ایستا صحیح
check_cgroup_v2_controller_available_wrapper (صحیح ret, صحیح cgroup_dirfd, ثابت نویسه *name, libcrun_error_t *err)
{
  اگر (ret == 0 || err == پوچ)
    بازگشت 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  اگر (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free نویسه *controllers = پوچ;
      libcrun_error_t tmp_err = پوچ;
      cleanup_free نویسه *کلید = پوچ;
      نویسه *saveptr = پوچ;
      bool found = false;
      ثابت نویسه *token;
      نویسه *it;

      /* Check if the specified controller is enabled.  */
      کلید = xstrdup (name);

      it = strchr (کلید, '.');
      اگر (it == پوچ)
        {
          crun_error_release (err);
          بازگشت crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      اگر (strcmp (کلید, "cgroup") == 0)
        بازگشت ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      اگر (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, پوچ, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          بازگشت ret;
        }
      برای (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (پوچ, " \n", &saveptr))
        {
          اگر (strcmp (token, کلید) == 0)
            {
              found = true;
              بشکن;
            }
        }
      اگر (! found)
        {
          cleanup_free نویسه *absolute_path = پوچ;
          libcrun_error_t tmp_err = پوچ;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          اگر (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", کلید, absolute_path);
          وگرنه
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", کلید);
            }
        }
    }
  بازگشت ret;
}

ایستا صحیح
write_file_and_check_controllers_at (bool cgroup2, صحیح dirfd, ثابت نویسه *name, ثابت نویسه *name_alias,
                                     ثابت تهی *data, size_t len, libcrun_error_t *err)
{
  صحیح ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  اگر (cgroup2)
    بازگشت check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  بازگشت ret;
}

ایستا صحیح
open_file_and_check_controllers_at (bool cgroup2, صحیح dirfd, ثابت نویسه *name, صحیح flags, libcrun_error_t *err)
{
  صحیح ret;

  ret = openat (dirfd, name, flags);
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      اگر (cgroup2)
        بازگشت check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  بازگشت ret;
}

/* The parser generates different structs but they are really all the same.  */
تعریفنوع runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

ایستا صحیح
write_blkio_v1_resources_throttling (صحیح dirfd, ثابت نویسه *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  نویسه fmt_buf[128];
  size_t i;
  cleanup_close صحیح fd = -1;

  اگر (throttling == پوچ)
    بازگشت 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", name);

  برای (i = 0; i < throttling_len; i++)
    {
      صحیح ret;
      صحیح len;
      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "write `%s`", name);
    }
  بازگشت 0;
}

ایستا صحیح
write_blkio_v2_resources_throttling (صحیح fd, ثابت نویسه *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  نویسه fmt_buf[128];
  size_t i;

  اگر (throttling == پوچ)
    بازگشت 0;

  برای (i = 0; i < throttling_len; i++)
    {
      صحیح ret;
      صحیح len;
      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "write `%s`", name);
    }
  بازگشت 0;
}

ایستا صحیح
write_blkio_resources (صحیح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  نویسه fmt_buf[128];
  صحیح len;
  صحیح ret;

  اگر (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu32, val);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
      اگر (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
      وگرنه
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu32, val);
                  اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
                    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              اگر (UNLIKELY (ret < 0))
                بازگشت ret;
            }
        }
    }
  اگر (blkio->leaf_weight)
    {
      اگر (cgroup2)
        بازگشت crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, اندازه (fmt_buf), "%d", blkio->leaf_weight);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  اگر (blkio->weight_device_len)
    {
      اگر (cgroup2)
        {
          cleanup_close صحیح wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          اگر (UNLIKELY (wfd < 0))
            بازگشت crun_make_error (err, errno, "open `io.bfq.weight`");
          برای (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
                بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (اكتب (wfd, fmt_buf, len));
              اگر (UNLIKELY (ret < 0))
                بازگشت crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      وگرنه
        {
          ثابت نویسه *leaf_weight_device_file_name = پوچ;
          ثابت نویسه *weight_device_file_name = پوچ;
          cleanup_close صحیح w_leafdevice_fd = -1;
          cleanup_close صحیح w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          اگر (UNLIKELY (w_device_fd < 0))
            بازگشت w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          اگر (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          برای (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
                بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (اكتب (w_device_fd, fmt_buf, len));
              اگر (UNLIKELY (ret < 0))
                بازگشت crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              اگر (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
                    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (اكتب (w_leafdevice_fd, fmt_buf, len));
                  اگر (UNLIKELY (ret < 0))
                    بازگشت crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  اگر (cgroup2)
    {
      cleanup_close صحیح wfd = -1;
      ثابت نویسه *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      اگر (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          بازگشت check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت 0;
}

ایستا صحیح
write_network_resources (صحیح dirfd_netclass, صحیح dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  نویسه fmt_buf[128];
  صحیح len;
  صحیح ret;
  اگر (net->class_id)
    {
      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu32, net->class_id);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  اگر (net->priorities_len)
    {
      size_t i;
      cleanup_close صحیح fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      اگر (UNLIKELY (fd < 0))
        بازگشت crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      برای (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, اندازه (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
            بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  بازگشت 0;
}

ایستا صحیح
write_hugetlb_resources (صحیح dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  ثابت نویسه *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  نویسه fmt_buf[128];
  size_t i;
  برای (i = 0; i < htlb_len; i++)
    {
      cleanup_free نویسه *filename = پوچ;
      cleanup_free نویسه *rsvd_filename = پوچ;
      صحیح len;
      صحیح ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64, htlb[i]->limit);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, پوچ, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      اگر (skip_rsvd)
        ادامه;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (crun_error_get_errno (err) != ENOENT)
            بازگشت ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  بازگشت 0;
}

ایستا صحیح
write_devices_resources_v1 (صحیح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  صحیح len;
  صحیح ret;

  برای (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      نویسه fmt_buf[FMT_BUF_LEN];
      ثابت نویسه *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      اگر (devs[i]->type == پوچ || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      وگرنه
        {
          نویسه fmt_buf_major[16];
          نویسه fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  انجامبده                                                                                    \
    {                                                                                   \
      اگر (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, اندازه (b), "%" PRIi64, x);                                \
          اگر (UNLIKELY (len >= (صحیح) اندازه (b)))                                       \
            بازگشت crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      وگرنه                                                                              \
        strcpy (b, "*");                                                                \
  } وقتی (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          اگر (UNLIKELY (len >= FMT_BUF_LEN))
            بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  برای (i = 0; default_devices[i].type; i++)
    {
      نویسه fmt_buf_major[16];
      نویسه fmt_buf_minor[16];
      نویسه device[64];
      صحیح len;

#define FMT_DEV(x, b)                                                                   \
  انجامبده                                                                                    \
    {                                                                                   \
      اگر (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, اندازه (b), "%d", x);                                      \
          اگر (UNLIKELY (len >= (صحیح) اندازه (b)))                                       \
            بازگشت crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      وگرنه                                                                              \
        strcpy (b, "*");                                                                \
  } وقتی (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, اندازه (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      اگر (UNLIKELY (len >= (صحیح) اندازه (device)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

ساختار bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  صحیح i;
  ساختار bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  اگر (UNLIKELY (program == پوچ))
    بازگشت پوچ;

  برای (i = (اندازه (default_devices) / اندازه (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      اگر (UNLIKELY (program == پوچ))
        بازگشت پوچ;
    }
  برای (i = devs_len - 1; i >= 0; i--)
    {
      نویسه type = 'a';
      صحیح minor = -1, major = -1;
      اگر (devs[i]->type != پوچ)
        type = devs[i]->type[0];

      اگر (devs[i]->major_present)
        major = devs[i]->major;
      اگر (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      اگر (UNLIKELY (program == پوچ))
        بازگشت پوچ;
    }

  program = bpf_program_complete_dev (program, err);
  اگر (UNLIKELY (program == پوچ))
    بازگشت پوچ;

  بازگشت program;
}

ایستا صحیح
write_devices_resources_v2_internal (صحیح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free ساختار bpf_program *program = پوچ;

  program = create_dev_bpf (devs, devs_len, err);
  اگر (UNLIKELY (program == پوچ))
    بازگشت -1;

  بازگشت libcrun_ebpf_load (program, dirfd, پوچ, err);
}

ایستا صحیح
write_devices_resources_v2 (صحیح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  صحیح ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  اگر (LIKELY (ret == 0))
    بازگشت 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  برای (i = 0; i < devs_len; i++)
    {
      اگر (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          بشکن;
        }
    }

  اگر (! can_skip)
    {
      libcrun_error_t tmp_err = پوچ;
      صحیح rootless;

      rootless = is_rootless (&tmp_err);
      اگر (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          بازگشت ret;
        }
      اگر (rootless)
        can_skip = true;
    }

  اگر (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  بازگشت ret;
}

ایستا صحیح
write_devices_resources (صحیح dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  صحیح ret;

  اگر (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  وگرنه
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  اگر (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = پوچ;
      صحیح rootless;

      rootless = is_rootless (&tmp_err);
      اگر (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          بازگشت ret;
        }

      اگر (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  بازگشت ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
ایستا صحیح
cg_itoa (نویسه *buf, size_t size, int64_t مقدار, bool use_max, libcrun_error_t *err)
{
  صحیح len;

  اگر (use_max && مقدار < 0)
    len = snprintf (buf, size, "max");
  وگرنه
    len = snprintf (buf, size, "%" PRIi64, مقدار);

  اگر (UNLIKELY (len >= (صحیح) size))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

  بازگشت len;
}

ایستا صحیح
write_memory (صحیح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  نویسه limit_buf[32];
  صحیح limit_buf_len;

  اگر (! memory->limit_present)
    بازگشت 0;

  limit_buf_len = cg_itoa (limit_buf, اندازه (limit_buf), memory->limit, cgroup2, err);
  اگر (UNLIKELY (limit_buf_len < 0))
    بازگشت limit_buf_len;

  بازگشت write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

ایستا صحیح
write_memory_swap (صحیح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  صحیح ret;
  int64_t swap;
  نویسه swap_buf[32];
  صحیح len;
  ثابت نویسه *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  اگر (! memory->swap_present)
    بازگشت 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  اگر (cgroup2 && memory->swap > 0)
    {
      اگر (! memory->limit_present)
        بازگشت crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      اگر (memory->swap < memory->limit)
        بازگشت crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, اندازه (swap_buf), swap, cgroup2, err);
  اگر (UNLIKELY (len < 0))
    بازگشت len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  اگر (ret >= 0)
    بازگشت ret;

  /* If swap is not enabled, ignore the error.  */
  اگر (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      بازگشت 0;
    }

  بازگشت ret;
}

ایستا صحیح
write_memory_resources (صحیح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  صحیح len;
  صحیح ret;
  نویسه fmt_buf[32];
  bool memory_limits_written = false;

  اگر (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      اگر (ret >= 0)
        memory_limits_written = true;
      وگرنه
        {
          اگر (cgroup2 || crun_error_get_errno (err) != EINVAL)
            بازگشت ret;

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
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (memory->kernel_present)
    {
      اگر (cgroup2)
        بازگشت crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64, memory->kernel);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  اگر (memory->use_hierarchy_present)
    {
      اگر (cgroup2)
        بازگشت crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (memory->reservation_present)
    {
      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64, memory->reservation);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 پوچ, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  اگر (memory->disable_oom_killer)
    {
      اگر (cgroup2)
        بازگشت crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  اگر (memory->kernel_tcp_present)
    {
      اگر (cgroup2)
        بازگشت crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  اگر (memory->swappiness_present)
    {
      اگر (cgroup2)
        بازگشت crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64, memory->swappiness);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

صحیح
write_cpu_idle (صحیح cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  نویسه fmt_buf[32];
  صحیح len;

  اگر (! cpu->idle_present)
    بازگشت 0;

  len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIi64, cpu->idle);
  اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
  بازگشت write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

صحیح
write_cpu_burst (صحیح cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  نویسه fmt_buf[32];
  صحیح len;

  اگر (! cpu->burst_present)
    بازگشت 0;

  len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIi64, cpu->burst);
  اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
  بازگشت write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

ایستا صحیح
write_pids_resources (صحیح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  اگر (pids->limit)
    {
      نویسه fmt_buf[32];
      صحیح len;
      صحیح ret;

      len = cg_itoa (fmt_buf, اندازه (fmt_buf), pids->limit, true, err);
      اگر (UNLIKELY (len < 0))
        بازگشت len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", پوچ, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

ایستا صحیح
write_cpu_resources (صحیح dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  صحیح len, period_len = 0;
  صحیح ret;
  نویسه fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free نویسه *period_str = پوچ;

  اگر (cpu->shares)
    {
      uint32_t val = cpu->shares;

      اگر (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          اگر (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              اگر (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, اندازه (fmt_buf), "%u", val);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 پوچ, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  اگر (cpu->period)
    {
      اگر (cgroup2)
        period = cpu->period;
      وگرنه
        {
          len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64, cpu->period);
          اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
            بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          اگر (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              اگر (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                بازگشت ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  اگر (cpu->quota)
    {
      اگر (cgroup2)
        quota = cpu->quota;
      وگرنه
        {
          len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIi64, cpu->quota);
          اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
            بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          اگر (period_str != پوچ)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              اگر (UNLIKELY (ret < 0))
                بازگشت ret;
            }
        }
    }
  اگر (cpu->realtime_period)
    {
      اگر (cgroup2)
        بازگشت crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64, cpu->realtime_period);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  اگر (cpu->realtime_runtime)
    {
      اگر (cgroup2)
        بازگشت crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (cgroup2 && (quota > 0 || period > 0))
    {
      اگر (period < 0)
        period = 100000;
      اگر (quota < 0)
        len = snprintf (fmt_buf, اندازه (fmt_buf), "max %" PRIi64, period);
      وگرنه
        len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", پوچ, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

صحیح
write_cpuset_resources (صحیح dirfd_cpuset, صحیح cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  صحیح ret;

  اگر (cpu == پوچ)
    بازگشت 0;

  اگر (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  اگر (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت 0;
}

ایستا صحیح
open_cgroup_subsystem (ثابت نویسه *subsystem, ثابت نویسه *path, libcrun_error_t *err)
{
  cleanup_free نویسه *full_path = پوچ;
  صحیح ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_ریشه, subsystem, path, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (dirfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", full_path);

  بازگشت dirfd;
}

صحیح
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, ثابت نویسه *path, libcrun_error_t *err)
{
  صحیح ret;

  اگر (resources->block_io)
    {
      cleanup_close صحیح dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      اگر (UNLIKELY (dirfd_blkio < 0))
        بازگشت dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (resources->network)
    {
      cleanup_close صحیح dirfd_netclass = -1;
      cleanup_close صحیح dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      اگر (UNLIKELY (dirfd_netclass < 0))
        بازگشت dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      اگر (UNLIKELY (dirfd_netprio < 0))
        بازگشت dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (resources->hugepage_limits_len)
    {
      cleanup_close صحیح dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      اگر (UNLIKELY (dirfd_htlb < 0))
        بازگشت dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (resources->devices_len)
    {
      cleanup_close صحیح dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      اگر (UNLIKELY (dirfd_devs < 0))
        بازگشت dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (resources->memory)
    {
      cleanup_close صحیح dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      اگر (UNLIKELY (dirfd_mem < 0))
        بازگشت dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (resources->pids)
    {
      cleanup_close صحیح dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      اگر (UNLIKELY (dirfd_pid < 0))
        بازگشت dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (resources->cpu)
    {
      cleanup_close صحیح dirfd_cpu = -1;
      cleanup_close صحیح dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      اگر (UNLIKELY (dirfd_cpu < 0))
        بازگشت dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      اگر (resources->cpu->cpus == پوچ && resources->cpu->mems == پوچ)
        بازگشت 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      اگر (UNLIKELY (dirfd_cpuset < 0))
        بازگشت dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (resources->unified && resources->unified->len > 0)
    بازگشت crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  بازگشت 0;
}

ایستا صحیح
write_unified_resources (صحیح cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  صحیح ret;

  برای (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close صحیح fd = -1;
      cleanup_free نویسه *مقدار = پوچ;
      نویسه *saveptr = پوچ;
      نویسه *سطر;

      اگر (strchr (resources->unified->کلیدها[i], '/'))
        بازگشت crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->کلیدها[i]);

      اگر (is_empty_string (resources->unified->مقادیر[i]))
        ادامه;

      مقدار = xstrdup (resources->unified->مقادیر[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->کلیدها[i], O_WRONLY, err);
      اگر (UNLIKELY (fd < 0))
        بازگشت fd;

      برای (سطر = strtok_r (مقدار, "\n", &saveptr); سطر; سطر = strtok_r (پوچ, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (اكتب (fd, سطر, strlen (سطر)));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "write to `%s`", resources->unified->کلیدها[i]);
        }
    }

  بازگشت 0;
}

ایستا صحیح
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, ثابت نویسه *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free نویسه *cgroup_path = پوچ;
  cleanup_close صحیح cgroup_dirfd = -1;
  صحیح ret;

  اگر (resources->network)
    بازگشت crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_ریشه, path, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (cgroup_dirfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", cgroup_path);

  اگر (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  اگر (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  اگر (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  اگر (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  اگر (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
صحیح
check_memory_before_update (ثابت نویسه *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free نویسه *swap_current = پوچ;
  cleanup_free نویسه *cgroup_path = پوچ;
  cleanup_free نویسه *current = پوچ;
  cleanup_close صحیح dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  صحیح cgroup_mode;
  صحیح ret;

  اگر (path == پوچ || memory == پوچ || ! (memory->check_before_update_present && memory->check_before_update))
    بازگشت 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  اگر (cgroup_mode != CGROUP_MODE_UNIFIED)
    بازگشت 0;

  ret = append_paths (&cgroup_path, err, CGROUP_ریشه, path, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (dirfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  errno = 0;
  val = strtoll (current, پوچ, 10);
  اگر (UNLIKELY (errno))
    بازگشت crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, پوچ, 10);
  اگر (UNLIKELY (errno))
    بازگشت crun_make_error (err, errno, "parse memory.swap.current");

  اگر (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  اگر (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  اگر (limit <= val + val_swap)
    بازگشت crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  بازگشت 0;
}

صحیح
update_cgroup_resources (ثابت نویسه *path,
                         ثابت نویسه *state_ریشه,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  صحیح cgroup_mode;

  (تهی) state_ریشه;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  اگر (path == پوچ)
    {
      size_t i;

      اگر (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        بازگشت crun_make_error (err, 0, "cannot set limits without cgroups");

      برای (i = 0; i < resources->devices_len; i++)
        {
          صحیح rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          اگر (UNLIKELY (rwm < 0))
            بازگشت rwm;

          اگر (rwm == 0)
            بازگشت crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      بازگشت 0;
    }
  انتخاب (cgroup_mode)
    {
    حالت CGROUP_MODE_UNIFIED:
      بازگشت update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    حالت CGROUP_MODE_LEGACY:
    حالت CGROUP_MODE_HYBRID:
      بازگشت update_cgroup_v1_resources (resources, path, err);

    پیشفرض:
      بازگشت crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
