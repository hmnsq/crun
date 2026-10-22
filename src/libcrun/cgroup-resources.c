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

#تعریف_کرو GNU_ماخذ

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

ساخت default_dev_s default_devices[] = {
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
  { 0, 0, 0, تہی }
};

ساخت default_dev_s *
get_default_devices ()
{
  واپس default_devices;
}

ساکن صحیح
write_cgroup_file_or_alias (صحیح dirfd, ثابت حرف *name, ثابت حرف *alias, ثابت خالی *ڈیٹا, size_t len, libcrun_error_t *err)
{
  صحیح ret;

  ret = write_cgroup_file (dirfd, name, ڈیٹا, len, err);
  اگر (UNLIKELY (alias != تہی && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, ڈیٹا, len, err);
    }
  واپس ret;
}

ساکن اندرونی صحیح
openat_with_alias (صحیح dirfd, ثابت حرف *name, ثابت حرف *alias, ثابت حرف **مستعمل_name, صحیح flags, libcrun_error_t *err)
{
  صحیح ret;

  *مستعمل_name = name;

  ret = openat (dirfd, name, flags);
  اگر (UNLIKELY (ret < 0 && alias != تہی && errno == ENOENT))
    {
      *مستعمل_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "open `%s`", name);
  واپس ret;
}

ساکن صحیح
is_rwm (ثابت حرف *str, libcrun_error_t *err)
{
  ثابت حرف *it;
  bool r = false;
  bool w = false;
  bool m = false;

  اگر (str == تہی)
    str = "";

  برائے (it = str; *it; it++)
    انتخاب (*it)
      {
      حالت 'r':
        r = true;
        توڑو;

      حالت 'w':
        w = true;
        توڑو;

      حالت 'm':
        m = true;
        توڑو;

      طےشدہ:
        واپس crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  واپس r && w && m ? 1 : 0;
}

ساکن صحیح
check_cgroup_v2_controller_دستیاب_wrapper (صحیح ret, صحیح cgroup_dirfd, ثابت حرف *name, libcrun_error_t *err)
{
  اگر (ret == 0 || err == تہی)
    واپس 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  اگر (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free حرف *controllers = تہی;
      libcrun_error_t tmp_err = تہی;
      cleanup_free حرف *کلید = تہی;
      حرف *saveptr = تہی;
      bool ملا = false;
      ثابت حرف *token;
      حرف *it;

      /* Check if the specified controller is enabled.  */
      کلید = xstrdup (name);

      it = strchr (کلید, '.');
      اگر (it == تہی)
        {
          crun_error_release (err);
          واپس crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      اگر (strcmp (کلید, "cgroup") == 0)
        واپس ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      اگر (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, تہی, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          واپس ret;
        }
      برائے (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (تہی, " \n", &saveptr))
        {
          اگر (strcmp (token, کلید) == 0)
            {
              ملا = true;
              توڑو;
            }
        }
      اگر (! ملا)
        {
          cleanup_free حرف *absolute_راستہ = تہی;
          libcrun_error_t tmp_err = تہی;

          crun_error_release (err);
          ret = get_realpath_مقصد_file (cgroup_dirfd, "cgroup.controllers", &absolute_راستہ, &tmp_err);
          اگر (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", کلید, absolute_راستہ);
          ورنہ
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", کلید);
            }
        }
    }
  واپس ret;
}

ساکن صحیح
write_file_and_check_controllers_at (bool cgroup2, صحیح dirfd, ثابت حرف *name, ثابت حرف *name_alias,
                                     ثابت خالی *ڈیٹا, size_t len, libcrun_error_t *err)
{
  صحیح ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, ڈیٹا, len, err);
  اگر (cgroup2)
    واپس check_cgroup_v2_controller_دستیاب_wrapper (ret, dirfd, name, err);
  واپس ret;
}

ساکن صحیح
open_file_and_check_controllers_at (bool cgroup2, صحیح dirfd, ثابت حرف *name, صحیح flags, libcrun_error_t *err)
{
  صحیح ret;

  ret = openat (dirfd, name, flags);
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      اگر (cgroup2)
        واپس check_cgroup_v2_controller_دستیاب_wrapper (ret, dirfd, name, err);
    }
  واپس ret;
}

/* The parser generates different structs but they are really all the same.  */
قسمتعریف runtime_spec_schema_defs_linux_بلاک_io_device_throttle throttling_s;

ساکن صحیح
write_blkio_v1_resources_throttling (صحیح dirfd, ثابت حرف *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  حرف fmt_buf[128];
  size_t i;
  cleanup_close صحیح fd = -1;

  اگر (throttling == تہی)
    واپس 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "open `%s`", name);

  برائے (i = 0; i < throttling_len; i++)
    {
      صحیح ret;
      صحیح len;
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->شرح);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      ret = عارضی_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "write `%s`", name);
    }
  واپس 0;
}

ساکن صحیح
write_blkio_v2_resources_throttling (صحیح fd, ثابت حرف *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  حرف fmt_buf[128];
  size_t i;

  اگر (throttling == تہی)
    واپس 0;

  برائے (i = 0; i < throttling_len; i++)
    {
      صحیح ret;
      صحیح len;
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->شرح);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      ret = عارضی_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "write `%s`", name);
    }
  واپس 0;
}

ساکن صحیح
write_blkio_resources (صحیح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_بلاک_io *blkio,
                       libcrun_error_t *err)
{
  حرف fmt_buf[128];
  صحیح len;
  صحیح ret;

  اگر (blkio->وزن)
    {
      uint32_t val = blkio->وزن;

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu32, val);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");
      اگر (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
      ورنہ
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu32, val);
                  اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
                    واپس crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              اگر (UNLIKELY (ret < 0))
                واپس ret;
            }
        }
    }
  اگر (blkio->برگ_وزن)
    {
      اگر (cgroup2)
        واپس crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%d", blkio->برگ_وزن);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  اگر (blkio->وزن_device_len)
    {
      اگر (cgroup2)
        {
          cleanup_close صحیح wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          اگر (UNLIKELY (wfd < 0))
            واپس crun_make_error (err, errno, "open `io.bfq.weight`");
          برائے (i = 0; i < blkio->وزن_device_len; i++)
            {
              uint32_t w = blkio->وزن_device[i]->وزن;

              len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->وزن_device[i]->major,
                              blkio->وزن_device[i]->minor, w);
              اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
                واپس crun_make_error (err, 0, "internal error: static buffer too small");

              ret = عارضی_FAILURE_RETRY (اكتب (wfd, fmt_buf, len));
              اگر (UNLIKELY (ret < 0))
                واپس crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      ورنہ
        {
          ثابت حرف *برگ_وزن_device_file_name = تہی;
          ثابت حرف *وزن_device_file_name = تہی;
          cleanup_close صحیح w_leafdevice_fd = -1;
          cleanup_close صحیح w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &وزن_device_file_name, O_WRONLY | O_CLOEXEC, err);
          اگر (UNLIKELY (w_device_fd < 0))
            واپس w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &برگ_وزن_device_file_name, O_WRONLY | O_CLOEXEC, err);
          اگر (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          برائے (i = 0; i < blkio->وزن_device_len; i++)
            {
              len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->وزن_device[i]->major,
                              blkio->وزن_device[i]->minor, blkio->وزن_device[i]->وزن);
              اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
                واپس crun_make_error (err, 0, "internal error: static buffer too small");

              ret = عارضی_FAILURE_RETRY (اكتب (w_device_fd, fmt_buf, len));
              اگر (UNLIKELY (ret < 0))
                واپس crun_make_error (err, errno, "write `%s`", وزن_device_file_name);

              اگر (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->وزن_device[i]->major,
                                  blkio->وزن_device[i]->minor, blkio->وزن_device[i]->برگ_وزن);
                  اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
                    واپس crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = عارضی_FAILURE_RETRY (اكتب (w_leafdevice_fd, fmt_buf, len));
                  اگر (UNLIKELY (ret < 0))
                    واپس crun_make_error (err, errno, "write `%s`", برگ_وزن_device_file_name);
                }
            }
        }
    }
  اگر (cgroup2)
    {
      cleanup_close صحیح wfd = -1;
      ثابت حرف *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      اگر (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          واپس check_cgroup_v2_controller_دستیاب_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس 0;
}

ساکن صحیح
write_جال_resources (صحیح dirfd_netclass, صحیح dirfd_netprio, runtime_spec_schema_config_linux_resources_جال *net,
                         libcrun_error_t *err)
{
  حرف fmt_buf[128];
  صحیح len;
  صحیح ret;
  اگر (net->class_id)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu32, net->class_id);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  اگر (net->priorities_len)
    {
      size_t i;
      cleanup_close صحیح fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      اگر (UNLIKELY (fd < 0))
        واپس crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      برائے (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, حجم (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->ترجیح);
          اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
            واپس crun_make_error (err, 0, "internal error: static buffer too small");

          ret = عارضی_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  واپس 0;
}

ساکن صحیح
write_hugetlb_resources (صحیح dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  ثابت حرف *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  حرف fmt_buf[128];
  size_t i;
  برائے (i = 0; i < htlb_len; i++)
    {
      cleanup_free حرف *filename = تہی;
      cleanup_free حرف *rsvd_filename = تہی;
      صحیح len;
      صحیح ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->صفحہ_سائز, suffix);

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, htlb[i]->حد_بندی);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, تہی, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (skip_rsvd)
        جاری;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->صفحہ_سائز, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (crun_error_get_errno (err) != ENOENT)
            واپس ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  واپس 0;
}

ساکن صحیح
write_devices_resources_v1 (صحیح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  صحیح len;
  صحیح ret;

  برائے (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#تعریف_کرو FMT_BUF_LEN 64
      حرف fmt_buf[FMT_BUF_LEN];
      ثابت حرف *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      اگر (devs[i]->type == تہی || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      ورنہ
        {
          حرف fmt_buf_major[16];
          حرف fmt_buf_minor[16];

#تعریف_کرو FMT_DEV(x, b)                                                                   \
  کرو                                                                                    \
    {                                                                                   \
      اگر (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, حجم (b), "%" PRIi64, x);                                \
          اگر (UNLIKELY (len >= (صحیح) حجم (b)))                                       \
            واپس crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ورنہ                                                                              \
        strcpy (b, "*");                                                                \
  } جب (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#تعریف_منسوخ FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          اگر (UNLIKELY (len >= FMT_BUF_LEN))
            واپس crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  برائے (i = 0; default_devices[i].type; i++)
    {
      حرف fmt_buf_major[16];
      حرف fmt_buf_minor[16];
      حرف device[64];
      صحیح len;

#تعریف_کرو FMT_DEV(x, b)                                                                   \
  کرو                                                                                    \
    {                                                                                   \
      اگر (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, حجم (b), "%d", x);                                      \
          اگر (UNLIKELY (len >= (صحیح) حجم (b)))                                       \
            واپس crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ورنہ                                                                              \
        strcpy (b, "*");                                                                \
  } جب (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#تعریف_منسوخ FMT_DEV

      len = snprintf (device, حجم (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      اگر (UNLIKELY (len >= (صحیح) حجم (device)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, لڑی_لمبائی (device), err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

ساخت bpf_پروگرام *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  صحیح i;
  ساخت bpf_پروگرام *پروگرام;

  پروگرام = bpf_پروگرام_new (2048);

  پروگرام = bpf_پروگرام_init_dev (پروگرام, err);
  اگر (UNLIKELY (پروگرام == تہی))
    واپس تہی;

  برائے (i = (حجم (default_devices) / حجم (default_devices[0])) - 1; i >= 0; i--)
    {
      پروگرام = bpf_پروگرام_append_dev (پروگرام, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      اگر (UNLIKELY (پروگرام == تہی))
        واپس تہی;
    }
  برائے (i = devs_len - 1; i >= 0; i--)
    {
      حرف type = 'a';
      صحیح minor = -1, major = -1;
      اگر (devs[i]->type != تہی)
        type = devs[i]->type[0];

      اگر (devs[i]->major_present)
        major = devs[i]->major;
      اگر (devs[i]->minor_present)
        minor = devs[i]->minor;

      پروگرام = bpf_پروگرام_append_dev (پروگرام, devs[i]->access, type, major, minor, devs[i]->allow, err);
      اگر (UNLIKELY (پروگرام == تہی))
        واپس تہی;
    }

  پروگرام = bpf_پروگرام_complete_dev (پروگرام, err);
  اگر (UNLIKELY (پروگرام == تہی))
    واپس تہی;

  واپس پروگرام;
}

ساکن صحیح
write_devices_resources_v2_اندرونی (صحیح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free ساخت bpf_پروگرام *پروگرام = تہی;

  پروگرام = create_dev_bpf (devs, devs_len, err);
  اگر (UNLIKELY (پروگرام == تہی))
    واپس -1;

  واپس libcrun_ebpf_بھرو (پروگرام, dirfd, تہی, err);
}

ساکن صحیح
write_devices_resources_v2 (صحیح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  صحیح ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_اندرونی (dirfd, devs, devs_len, err);
  اگر (LIKELY (ret == 0))
    واپس 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  برائے (i = 0; i < devs_len; i++)
    {
      اگر (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          توڑو;
        }
    }

  اگر (! can_skip)
    {
      libcrun_error_t tmp_err = تہی;
      صحیح rootless;

      rootless = is_rootless (&tmp_err);
      اگر (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          واپس ret;
        }
      اگر (rootless)
        can_skip = true;
    }

  اگر (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  واپس ret;
}

ساکن صحیح
write_devices_resources (صحیح dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  صحیح ret;

  اگر (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  ورنہ
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  اگر (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = تہی;
      صحیح rootless;

      rootless = is_rootless (&tmp_err);
      اگر (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          واپس ret;
        }

      اگر (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  واپس ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
ساکن صحیح
cg_itoa (حرف *buf, size_t سائز, int64_t قدر, bool use_max, libcrun_error_t *err)
{
  صحیح len;

  اگر (use_max && قدر < 0)
    len = snprintf (buf, سائز, "max");
  ورنہ
    len = snprintf (buf, سائز, "%" PRIi64, قدر);

  اگر (UNLIKELY (len >= (صحیح) سائز))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");

  واپس len;
}

ساکن صحیح
write_حافظہ (صحیح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_حافظہ *حافظہ, libcrun_error_t *err)
{
  حرف حد_بندی_buf[32];
  صحیح حد_بندی_buf_len;

  اگر (! حافظہ->حد_بندی_present)
    واپس 0;

  حد_بندی_buf_len = cg_itoa (حد_بندی_buf, حجم (حد_بندی_buf), حافظہ->حد_بندی, cgroup2, err);
  اگر (UNLIKELY (حد_بندی_buf_len < 0))
    واپس حد_بندی_buf_len;

  واپس write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", حد_بندی_buf, حد_بندی_buf_len, err);
}

ساکن صحیح
write_حافظہ_تبادلہ (صحیح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_حافظہ *حافظہ,
                   libcrun_error_t *err)
{
  صحیح ret;
  int64_t تبادلہ;
  حرف تبادلہ_buf[32];
  صحیح len;
  ثابت حرف *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  اگر (! حافظہ->تبادلہ_present)
    واپس 0;

  تبادلہ = حافظہ->تبادلہ;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  اگر (cgroup2 && حافظہ->تبادلہ > 0)
    {
      اگر (! حافظہ->حد_بندی_present)
        واپس crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      اگر (حافظہ->تبادلہ < حافظہ->حد_بندی)
        واپس crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      تبادلہ -= حافظہ->حد_بندی;
    }

  len = cg_itoa (تبادلہ_buf, حجم (تبادلہ_buf), تبادلہ, cgroup2, err);
  اگر (UNLIKELY (len < 0))
    واپس len;

  ret = write_cgroup_file (dirfd, fname, تبادلہ_buf, len, err);
  اگر (ret >= 0)
    واپس ret;

  /* If swap is not enabled, ignore the error.  */
  اگر (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      واپس 0;
    }

  واپس ret;
}

ساکن صحیح
write_حافظہ_resources (صحیح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_حافظہ *حافظہ,
                        libcrun_error_t *err)
{
  صحیح len;
  صحیح ret;
  حرف fmt_buf[32];
  bool حافظہ_limits_written = false;

  اگر (حافظہ->حد_بندی_present)
    {
      ret = write_حافظہ (dirfd, cgroup2, حافظہ, err);
      اگر (ret >= 0)
        حافظہ_limits_written = true;
      ورنہ
        {
          اگر (cgroup2 || crun_error_get_errno (err) != EINVAL)
            واپس ret;

          /*
            If we get an EINVAL error on cgroup v1 we reverse
            the order we write the memory limit and the swap.
            Attempt to write again the memory limit once the memory
            swap is written.
          */
          crun_error_release (err);
        }
    }

  ret = write_حافظہ_تبادلہ (dirfd, cgroup2, حافظہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (حافظہ->حد_بندی_present && ! حافظہ_limits_written)
    {
      ret = write_حافظہ (dirfd, cgroup2, حافظہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (حافظہ->kernel_present)
    {
      اگر (cgroup2)
        واپس crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, حافظہ->kernel);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  اگر (حافظہ->use_hierarchy_present)
    {
      اگر (cgroup2)
        واپس crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (حافظہ->use_hierarchy) ? "1" : "0", 1, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (حافظہ->reservation_present)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, حافظہ->reservation);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 تہی, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  اگر (حافظہ->disable_oom_killer)
    {
      اگر (cgroup2)
        واپس crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  اگر (حافظہ->kernel_tcp_present)
    {
      اگر (cgroup2)
        واپس crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, حافظہ->kernel_tcp);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  اگر (حافظہ->swappiness_present)
    {
      اگر (cgroup2)
        واپس crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, حافظہ->swappiness);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

صحیح
write_cpu_idle (صحیح cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  حرف fmt_buf[32];
  صحیح len;

  اگر (! cpu->idle_present)
    واپس 0;

  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64, cpu->idle);
  اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");
  واپس write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

صحیح
write_cpu_مدت (صحیح cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  حرف fmt_buf[32];
  صحیح len;

  اگر (! cpu->مدت_present)
    واپس 0;

  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64, cpu->مدت);
  اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");
  واپس write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

ساکن صحیح
write_pids_resources (صحیح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  اگر (pids->حد_بندی)
    {
      حرف fmt_buf[32];
      صحیح len;
      صحیح ret;

      len = cg_itoa (fmt_buf, حجم (fmt_buf), pids->حد_بندی, true, err);
      اگر (UNLIKELY (len < 0))
        واپس len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", تہی, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

ساکن صحیح
write_cpu_resources (صحیح dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  صحیح len, period_len = 0;
  صحیح ret;
  حرف fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free حرف *period_str = تہی;

  اگر (cpu->shares)
    {
      uint32_t val = cpu->shares;

      اگر (cgroup2)
        {
          val = convert_shares_مقصد_وزن (val);

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

      len = snprintf (fmt_buf, حجم (fmt_buf), "%u", val);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 تہی, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  اگر (cpu->period)
    {
      اگر (cgroup2)
        period = cpu->period;
      ورنہ
        {
          len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, cpu->period);
          اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
            واپس crun_make_error (err, 0, "internal error: static buffer too small");
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
                واپس ret;

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
      ورنہ
        {
          len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64, cpu->quota);
          اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
            واپس crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          اگر (period_str != تہی)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              اگر (UNLIKELY (ret < 0))
                واپس ret;
            }
        }
    }
  اگر (cpu->realtime_period)
    {
      اگر (cgroup2)
        واپس crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, cpu->realtime_period);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  اگر (cpu->realtime_runtime)
    {
      اگر (cgroup2)
        واپس crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (cgroup2 && (quota > 0 || period > 0))
    {
      اگر (period < 0)
        period = 100000;
      اگر (quota < 0)
        len = snprintf (fmt_buf, حجم (fmt_buf), "max %" PRIi64, period);
      ورنہ
        len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", تہی, fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس write_cpu_مدت (dirfd_cpu, cgroup2, cpu, err);
}

صحیح
write_cpuset_resources (صحیح dirfd_cpuset, صحیح cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  صحیح ret;

  اگر (cpu == تہی)
    واپس 0;

  اگر (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, لڑی_لمبائی (cpu->cpus), err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  اگر (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, لڑی_لمبائی (cpu->mems),
                                                 err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس 0;
}

ساکن صحیح
open_cgroup_subsystem (ثابت حرف *subsystem, ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *بھرا_راستہ = تہی;
  صحیح ret, dirfd;

  ret = append_paths (&بھرا_راستہ, err, CGROUP_جڑ, subsystem, راستہ, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  dirfd = open (بھرا_راستہ, O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (dirfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", بھرا_راستہ);

  واپس dirfd;
}

صحیح
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *راستہ, libcrun_error_t *err)
{
  صحیح ret;

  اگر (resources->بلاک_io)
    {
      cleanup_close صحیح dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_بلاک_io *blkio = resources->بلاک_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", راستہ, err);
      اگر (UNLIKELY (dirfd_blkio < 0))
        واپس dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (resources->جال)
    {
      cleanup_close صحیح dirfd_netclass = -1;
      cleanup_close صحیح dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_جال *جال = resources->جال;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", راستہ, err);
      اگر (UNLIKELY (dirfd_netclass < 0))
        واپس dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", راستہ, err);
      اگر (UNLIKELY (dirfd_netprio < 0))
        واپس dirfd_netprio;

      ret = write_جال_resources (dirfd_netclass, dirfd_netprio, جال, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (resources->hugepage_limits_len)
    {
      cleanup_close صحیح dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", راستہ, err);
      اگر (UNLIKELY (dirfd_htlb < 0))
        واپس dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (resources->devices_len)
    {
      cleanup_close صحیح dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", راستہ, err);
      اگر (UNLIKELY (dirfd_devs < 0))
        واپس dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (resources->حافظہ)
    {
      cleanup_close صحیح dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", راستہ, err);
      اگر (UNLIKELY (dirfd_mem < 0))
        واپس dirfd_mem;

      ret = write_حافظہ_resources (dirfd_mem, false, resources->حافظہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (resources->pids)
    {
      cleanup_close صحیح dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", راستہ, err);
      اگر (UNLIKELY (dirfd_pid < 0))
        واپس dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (resources->cpu)
    {
      cleanup_close صحیح dirfd_cpu = -1;
      cleanup_close صحیح dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", راستہ, err);
      اگر (UNLIKELY (dirfd_cpu < 0))
        واپس dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (resources->cpu->cpus == تہی && resources->cpu->mems == تہی)
        واپس 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", راستہ, err);
      اگر (UNLIKELY (dirfd_cpuset < 0))
        واپس dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (resources->unified && resources->unified->len > 0)
    واپس crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  واپس 0;
}

ساکن صحیح
write_unified_resources (صحیح cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  صحیح ret;

  برائے (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close صحیح fd = -1;
      cleanup_free حرف *قدر = تہی;
      حرف *saveptr = تہی;
      حرف *سطر;

      اگر (strchr (resources->unified->کلیدیں[i], '/'))
        واپس crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->کلیدیں[i]);

      اگر (is_خالی_خانے_string (resources->unified->اقدار[i]))
        جاری;

      قدر = xstrdup (resources->unified->اقدار[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->کلیدیں[i], O_WRONLY, err);
      اگر (UNLIKELY (fd < 0))
        واپس fd;

      برائے (سطر = strtok_r (قدر, "\n", &saveptr); سطر; سطر = strtok_r (تہی, "\n", &saveptr))
        {
          ret = عارضی_FAILURE_RETRY (اكتب (fd, سطر, لڑی_لمبائی (سطر)));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "write to `%s`", resources->unified->کلیدیں[i]);
        }
    }

  واپس 0;
}

ساکن صحیح
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *راستہ, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_راستہ = تہی;
  cleanup_close صحیح cgroup_dirfd = -1;
  صحیح ret;

  اگر (resources->جال)
    واپس crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, راستہ, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  cgroup_dirfd = open (cgroup_راستہ, O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (cgroup_dirfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", cgroup_راستہ);

  اگر (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (resources->حافظہ)
    {
      ret = write_حافظہ_resources (cgroup_dirfd, true, resources->حافظہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  اگر (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  اگر (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  اگر (resources->بلاک_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->بلاک_io, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  اگر (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
صحیح
check_حافظہ_before_update (ثابت حرف *راستہ, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_حافظہ *حافظہ = resources->حافظہ;
  cleanup_free حرف *تبادلہ_current = تہی;
  cleanup_free حرف *cgroup_راستہ = تہی;
  cleanup_free حرف *current = تہی;
  cleanup_close صحیح dirfd = -1;
  uint64_t حد_بندی = 0;
  uint64_t val, val_تبادلہ;
  صحیح cgroup_منوال;
  صحیح ret;

  اگر (راستہ == تہی || حافظہ == تہی || ! (حافظہ->check_before_update_present && حافظہ->check_before_update))
    واپس 0;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  اگر (cgroup_منوال != CGROUP_منوال_UNIFIED)
    واپس 0;

  ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, راستہ, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  dirfd = open (cgroup_راستہ, O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (dirfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", cgroup_راستہ);

  ret = read_all_file_at (dirfd, "memory.current", &current, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &تبادلہ_current, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  errno = 0;
  val = strtoll (current, تہی, 10);
  اگر (UNLIKELY (errno))
    واپس crun_make_error (err, errno, "parse memory.current");

  val_تبادلہ = strtoll (تبادلہ_current, تہی, 10);
  اگر (UNLIKELY (errno))
    واپس crun_make_error (err, errno, "parse memory.swap.current");

  اگر (حافظہ->حد_بندی_present && حافظہ->حد_بندی >= 0)
    حد_بندی = حافظہ->حد_بندی;
  اگر (حافظہ->تبادلہ_present && حافظہ->تبادلہ >= 0)
    حد_بندی += حافظہ->تبادلہ;

  اگر (حد_بندی <= val + val_تبادلہ)
    واپس crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  واپس 0;
}

صحیح
update_cgroup_resources (ثابت حرف *راستہ,
                         ثابت حرف *state_جڑ,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  صحیح cgroup_منوال;

  (خالی) state_جڑ;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  اگر (راستہ == تہی)
    {
      size_t i;

      اگر (resources->بلاک_io || resources->جال || resources->hugepage_limits_len || resources->حافظہ
          || resources->pids || resources->cpu)
        واپس crun_make_error (err, 0, "cannot set limits without cgroups");

      برائے (i = 0; i < resources->devices_len; i++)
        {
          صحیح rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          اگر (UNLIKELY (rwm < 0))
            واپس rwm;

          اگر (rwm == 0)
            واپس crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      واپس 0;
    }
  انتخاب (cgroup_منوال)
    {
    حالت CGROUP_منوال_UNIFIED:
      واپس update_cgroup_v2_resources (resources, راستہ, need_bpf_dev, err);

    حالت CGROUP_منوال_LEGACY:
    حالت CGROUP_منوال_HYBRID:
      واپس update_cgroup_v1_resources (resources, راستہ, err);

    طےشدہ:
      واپس crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_منوال);
    }
}
