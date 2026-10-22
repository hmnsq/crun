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

#ضمن <config.h>
#ضمن "cgroup.h"
#ضمن "cgroup-internal.h"
#ضمن "cgroup-systemd.h"
#ضمن "cgroup-utils.h"
#ضمن "cgroup-resources.h"
#ضمن "ebpf.h"
#ضمن "utils.h"
#ضمن "status.h"
#ضمن <string.h>
#ضمن <sys/types.h>
#ضمن <sys/vfs.h>
#ضمن <inttypes.h>
#ضمن <time.h>

#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <libgen.h>

بنية default_dev_s default_devices[] = {
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
  { 0, 0, 0, عدم }
};

بنية default_dev_s *
get_default_devices ()
{
  أرجع default_devices;
}

ساكن صحيح
write_cgroup_file_or_alias (صحيح dirfd, ثابت حرف *name, ثابت حرف *alias, ثابت فراغ *data, size_t len, libcrun_error_t *err)
{
  صحيح ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  إذا (UNLIKELY (alias != عدم && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  أرجع ret;
}

ساكن مضمن صحيح
openat_with_alias (صحيح dirfd, ثابت حرف *name, ثابت حرف *alias, ثابت حرف **used_name, صحيح flags, libcrun_error_t *err)
{
  صحيح ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  إذا (UNLIKELY (ret < 0 && alias != عدم && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "open `%s`", name);
  أرجع ret;
}

ساكن صحيح
is_rwm (ثابت حرف *str, libcrun_error_t *err)
{
  ثابت حرف *it;
  bool r = false;
  bool w = false;
  bool m = false;

  إذا (str == عدم)
    str = "";

  لكل (it = str; *it; it++)
    بدل (*it)
      {
      حالة 'r':
        r = true;
        اكسر;

      حالة 'w':
        w = true;
        اكسر;

      حالة 'm':
        m = true;
        اكسر;

      افتراضي:
        أرجع crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  أرجع r && w && m ? 1 : 0;
}

ساكن صحيح
check_cgroup_v2_controller_available_wrapper (صحيح ret, صحيح cgroup_dirfd, ثابت حرف *name, libcrun_error_t *err)
{
  إذا (ret == 0 || err == عدم)
    أرجع 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  إذا (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free حرف *controllers = عدم;
      libcrun_error_t tmp_err = عدم;
      cleanup_free حرف *ئاچقۇچ = عدم;
      حرف *saveptr = عدم;
      bool found = false;
      ثابت حرف *token;
      حرف *it;

      /* Check if the specified controller is enabled.  */
      ئاچقۇچ = xstrdup (name);

      it = strchr (ئاچقۇچ, '.');
      إذا (it == عدم)
        {
          crun_error_release (err);
          أرجع crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      إذا (strcmp (ئاچقۇچ, "cgroup") == 0)
        أرجع ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      إذا (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, عدم, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          أرجع ret;
        }
      لكل (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (عدم, " \n", &saveptr))
        {
          إذا (strcmp (token, ئاچقۇچ) == 0)
            {
              found = true;
              اكسر;
            }
        }
      إذا (! found)
        {
          cleanup_free حرف *absolute_path = عدم;
          libcrun_error_t tmp_err = عدم;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          إذا (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", ئاچقۇچ, absolute_path);
          وإلا
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", ئاچقۇچ);
            }
        }
    }
  أرجع ret;
}

ساكن صحيح
write_file_and_check_controllers_at (bool cgroup2, صحيح dirfd, ثابت حرف *name, ثابت حرف *name_alias,
                                     ثابت فراغ *data, size_t len, libcrun_error_t *err)
{
  صحيح ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  إذا (cgroup2)
    أرجع check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  أرجع ret;
}

ساكن صحيح
open_file_and_check_controllers_at (bool cgroup2, صحيح dirfd, ثابت حرف *name, صحيح flags, libcrun_error_t *err)
{
  صحيح ret;

  ret = openat (dirfd, name, flags);
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      إذا (cgroup2)
        أرجع check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  أرجع ret;
}

/* The parser generates different structs but they are really all the same.  */
تعريفنوع runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

ساكن صحيح
write_blkio_v1_resources_throttling (صحيح dirfd, ثابت حرف *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  حرف fmt_buf[128];
  size_t i;
  cleanup_close صحيح fd = -1;

  إذا (throttling == عدم)
    أرجع 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", name);

  لكل (i = 0; i < throttling_len; i++)
    {
      صحيح ret;
      صحيح len;
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "write `%s`", name);
    }
  أرجع 0;
}

ساكن صحيح
write_blkio_v2_resources_throttling (صحيح fd, ثابت حرف *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  حرف fmt_buf[128];
  size_t i;

  إذا (throttling == عدم)
    أرجع 0;

  لكل (i = 0; i < throttling_len; i++)
    {
      صحيح ret;
      صحيح len;
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "write `%s`", name);
    }
  أرجع 0;
}

ساكن صحيح
write_blkio_resources (صحيح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  حرف fmt_buf[128];
  صحيح len;
  صحيح ret;

  إذا (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu32, val);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");
      إذا (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          إذا (UNLIKELY (ret < 0))
            {
              إذا (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu32, val);
                  إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
                    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              إذا (UNLIKELY (ret < 0))
                أرجع ret;
            }
        }
    }
  إذا (blkio->leaf_weight)
    {
      إذا (cgroup2)
        أرجع crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%d", blkio->leaf_weight);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  إذا (blkio->weight_device_len)
    {
      إذا (cgroup2)
        {
          cleanup_close صحيح wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          إذا (UNLIKELY (wfd < 0))
            أرجع crun_make_error (err, errno, "open `io.bfq.weight`");
          لكل (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
                أرجع crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (اكتب (wfd, fmt_buf, len));
              إذا (UNLIKELY (ret < 0))
                أرجع crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      وإلا
        {
          ثابت حرف *leaf_weight_device_file_name = عدم;
          ثابت حرف *weight_device_file_name = عدم;
          cleanup_close صحيح w_leafdevice_fd = -1;
          cleanup_close صحيح w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          إذا (UNLIKELY (w_device_fd < 0))
            أرجع w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          إذا (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          لكل (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
                أرجع crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (اكتب (w_device_fd, fmt_buf, len));
              إذا (UNLIKELY (ret < 0))
                أرجع crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              إذا (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
                    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (اكتب (w_leafdevice_fd, fmt_buf, len));
                  إذا (UNLIKELY (ret < 0))
                    أرجع crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  إذا (cgroup2)
    {
      cleanup_close صحيح wfd = -1;
      ثابت حرف *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      إذا (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          أرجع check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
write_network_resources (صحيح dirfd_netclass, صحيح dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  حرف fmt_buf[128];
  صحيح len;
  صحيح ret;
  إذا (net->class_id)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu32, net->class_id);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  إذا (net->priorities_len)
    {
      size_t i;
      cleanup_close صحيح fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      إذا (UNLIKELY (fd < 0))
        أرجع crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      لكل (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, حجم (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
            أرجع crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  أرجع 0;
}

ساكن صحيح
write_hugetlb_resources (صحيح dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  ثابت حرف *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  حرف fmt_buf[128];
  size_t i;
  لكل (i = 0; i < htlb_len; i++)
    {
      cleanup_free حرف *filename = عدم;
      cleanup_free حرف *rsvd_filename = عدم;
      صحيح len;
      صحيح ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, htlb[i]->limit);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, عدم, fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (skip_rsvd)
        استمر;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (crun_error_get_errno (err) != ENOENT)
            أرجع ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  أرجع 0;
}

ساكن صحيح
write_devices_resources_v1 (صحيح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  صحيح len;
  صحيح ret;

  لكل (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      حرف fmt_buf[FMT_BUF_LEN];
      ثابت حرف *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      إذا (devs[i]->type == عدم || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      وإلا
        {
          حرف fmt_buf_major[16];
          حرف fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  افعل                                                                                    \
    {                                                                                   \
      إذا (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, حجم (b), "%" PRIi64, x);                                \
          إذا (UNLIKELY (len >= (صحيح) حجم (b)))                                       \
            أرجع crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      وإلا                                                                              \
        strcpy (b, "*");                                                                \
  } طالما (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          إذا (UNLIKELY (len >= FMT_BUF_LEN))
            أرجع crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  لكل (i = 0; default_devices[i].type; i++)
    {
      حرف fmt_buf_major[16];
      حرف fmt_buf_minor[16];
      حرف device[64];
      صحيح len;

#define FMT_DEV(x, b)                                                                   \
  افعل                                                                                    \
    {                                                                                   \
      إذا (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, حجم (b), "%d", x);                                      \
          إذا (UNLIKELY (len >= (صحيح) حجم (b)))                                       \
            أرجع crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      وإلا                                                                              \
        strcpy (b, "*");                                                                \
  } طالما (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, حجم (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      إذا (UNLIKELY (len >= (صحيح) حجم (device)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

بنية bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  صحيح i;
  بنية bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  إذا (UNLIKELY (program == عدم))
    أرجع عدم;

  لكل (i = (حجم (default_devices) / حجم (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      إذا (UNLIKELY (program == عدم))
        أرجع عدم;
    }
  لكل (i = devs_len - 1; i >= 0; i--)
    {
      حرف type = 'a';
      صحيح minor = -1, major = -1;
      إذا (devs[i]->type != عدم)
        type = devs[i]->type[0];

      إذا (devs[i]->major_present)
        major = devs[i]->major;
      إذا (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      إذا (UNLIKELY (program == عدم))
        أرجع عدم;
    }

  program = bpf_program_complete_dev (program, err);
  إذا (UNLIKELY (program == عدم))
    أرجع عدم;

  أرجع program;
}

ساكن صحيح
write_devices_resources_v2_internal (صحيح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free بنية bpf_program *program = عدم;

  program = create_dev_bpf (devs, devs_len, err);
  إذا (UNLIKELY (program == عدم))
    أرجع -1;

  أرجع libcrun_ebpf_load (program, dirfd, عدم, err);
}

ساكن صحيح
write_devices_resources_v2 (صحيح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  صحيح ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  إذا (LIKELY (ret == 0))
    أرجع 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  لكل (i = 0; i < devs_len; i++)
    {
      إذا (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          اكسر;
        }
    }

  إذا (! can_skip)
    {
      libcrun_error_t tmp_err = عدم;
      صحيح rootless;

      rootless = is_rootless (&tmp_err);
      إذا (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          أرجع ret;
        }
      إذا (rootless)
        can_skip = true;
    }

  إذا (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  أرجع ret;
}

ساكن صحيح
write_devices_resources (صحيح dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  صحيح ret;

  إذا (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  وإلا
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  إذا (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = عدم;
      صحيح rootless;

      rootless = is_rootless (&tmp_err);
      إذا (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          أرجع ret;
        }

      إذا (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  أرجع ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
ساكن صحيح
cg_itoa (حرف *buf, size_t size, int64_t قىممەت, bool use_max, libcrun_error_t *err)
{
  صحيح len;

  إذا (use_max && قىممەت < 0)
    len = snprintf (buf, size, "max");
  وإلا
    len = snprintf (buf, size, "%" PRIi64, قىممەت);

  إذا (UNLIKELY (len >= (صحيح) size))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  أرجع len;
}

ساكن صحيح
write_memory (صحيح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  حرف limit_buf[32];
  صحيح limit_buf_len;

  إذا (! memory->limit_present)
    أرجع 0;

  limit_buf_len = cg_itoa (limit_buf, حجم (limit_buf), memory->limit, cgroup2, err);
  إذا (UNLIKELY (limit_buf_len < 0))
    أرجع limit_buf_len;

  أرجع write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

ساكن صحيح
write_memory_swap (صحيح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  صحيح ret;
  int64_t swap;
  حرف swap_buf[32];
  صحيح len;
  ثابت حرف *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  إذا (! memory->swap_present)
    أرجع 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  إذا (cgroup2 && memory->swap > 0)
    {
      إذا (! memory->limit_present)
        أرجع crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      إذا (memory->swap < memory->limit)
        أرجع crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, حجم (swap_buf), swap, cgroup2, err);
  إذا (UNLIKELY (len < 0))
    أرجع len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  إذا (ret >= 0)
    أرجع ret;

  /* If swap is not enabled, ignore the error.  */
  إذا (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      أرجع 0;
    }

  أرجع ret;
}

ساكن صحيح
write_memory_resources (صحيح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  صحيح len;
  صحيح ret;
  حرف fmt_buf[32];
  bool memory_limits_written = false;

  إذا (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      إذا (ret >= 0)
        memory_limits_written = true;
      وإلا
        {
          إذا (cgroup2 || crun_error_get_errno (err) != EINVAL)
            أرجع ret;

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
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (memory->kernel_present)
    {
      إذا (cgroup2)
        أرجع crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, memory->kernel);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  إذا (memory->use_hierarchy_present)
    {
      إذا (cgroup2)
        أرجع crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (memory->reservation_present)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, memory->reservation);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 عدم, fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  إذا (memory->disable_oom_killer)
    {
      إذا (cgroup2)
        أرجع crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  إذا (memory->kernel_tcp_present)
    {
      إذا (cgroup2)
        أرجع crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  إذا (memory->swappiness_present)
    {
      إذا (cgroup2)
        أرجع crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, memory->swappiness);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

صحيح
write_cpu_idle (صحيح cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  حرف fmt_buf[32];
  صحيح len;

  إذا (! cpu->idle_present)
    أرجع 0;

  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64, cpu->idle);
  إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");
  أرجع write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

صحيح
write_cpu_burst (صحيح cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  حرف fmt_buf[32];
  صحيح len;

  إذا (! cpu->burst_present)
    أرجع 0;

  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64, cpu->burst);
  إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");
  أرجع write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

ساكن صحيح
write_pids_resources (صحيح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  إذا (pids->limit)
    {
      حرف fmt_buf[32];
      صحيح len;
      صحيح ret;

      len = cg_itoa (fmt_buf, حجم (fmt_buf), pids->limit, true, err);
      إذا (UNLIKELY (len < 0))
        أرجع len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", عدم, fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

ساكن صحيح
write_cpu_resources (صحيح dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  صحيح len, period_len = 0;
  صحيح ret;
  حرف fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free حرف *period_str = عدم;

  إذا (cpu->shares)
    {
      uint32_t val = cpu->shares;

      إذا (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          إذا (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              إذا (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, حجم (fmt_buf), "%u", val);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 عدم, fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  إذا (cpu->period)
    {
      إذا (cgroup2)
        period = cpu->period;
      وإلا
        {
          len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, cpu->period);
          إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
            أرجع crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          إذا (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              إذا (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                أرجع ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  إذا (cpu->quota)
    {
      إذا (cgroup2)
        quota = cpu->quota;
      وإلا
        {
          len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64, cpu->quota);
          إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
            أرجع crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          إذا (period_str != عدم)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;
            }
        }
    }
  إذا (cpu->realtime_period)
    {
      إذا (cgroup2)
        أرجع crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, cpu->realtime_period);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  إذا (cpu->realtime_runtime)
    {
      إذا (cgroup2)
        أرجع crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cgroup2 && (quota > 0 || period > 0))
    {
      إذا (period < 0)
        period = 100000;
      إذا (quota < 0)
        len = snprintf (fmt_buf, حجم (fmt_buf), "max %" PRIi64, period);
      وإلا
        len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", عدم, fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

صحيح
write_cpuset_resources (صحيح dirfd_cpuset, صحيح cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  صحيح ret;

  إذا (cpu == عدم)
    أرجع 0;

  إذا (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  إذا (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
open_cgroup_subsystem (ثابت حرف *subsystem, ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *full_path = عدم;
  صحيح ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_يىلتىز, subsystem, path, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", full_path);

  أرجع dirfd;
}

صحيح
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *path, libcrun_error_t *err)
{
  صحيح ret;

  إذا (resources->block_io)
    {
      cleanup_close صحيح dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      إذا (UNLIKELY (dirfd_blkio < 0))
        أرجع dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (resources->network)
    {
      cleanup_close صحيح dirfd_netclass = -1;
      cleanup_close صحيح dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      إذا (UNLIKELY (dirfd_netclass < 0))
        أرجع dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      إذا (UNLIKELY (dirfd_netprio < 0))
        أرجع dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (resources->hugepage_limits_len)
    {
      cleanup_close صحيح dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      إذا (UNLIKELY (dirfd_htlb < 0))
        أرجع dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (resources->devices_len)
    {
      cleanup_close صحيح dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      إذا (UNLIKELY (dirfd_devs < 0))
        أرجع dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (resources->memory)
    {
      cleanup_close صحيح dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      إذا (UNLIKELY (dirfd_mem < 0))
        أرجع dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (resources->pids)
    {
      cleanup_close صحيح dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      إذا (UNLIKELY (dirfd_pid < 0))
        أرجع dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (resources->cpu)
    {
      cleanup_close صحيح dirfd_cpu = -1;
      cleanup_close صحيح dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      إذا (UNLIKELY (dirfd_cpu < 0))
        أرجع dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (resources->cpu->cpus == عدم && resources->cpu->mems == عدم)
        أرجع 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      إذا (UNLIKELY (dirfd_cpuset < 0))
        أرجع dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (resources->unified && resources->unified->len > 0)
    أرجع crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  أرجع 0;
}

ساكن صحيح
write_unified_resources (صحيح cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  صحيح ret;

  لكل (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close صحيح fd = -1;
      cleanup_free حرف *قىممەت = عدم;
      حرف *saveptr = عدم;
      حرف *قۇر;

      إذا (strchr (resources->unified->ئاچقۇچلار[i], '/'))
        أرجع crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->ئاچقۇچلار[i]);

      إذا (is_empty_string (resources->unified->قىممەتلەر[i]))
        استمر;

      قىممەت = xstrdup (resources->unified->قىممەتلەر[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->ئاچقۇچلار[i], O_WRONLY, err);
      إذا (UNLIKELY (fd < 0))
        أرجع fd;

      لكل (قۇر = strtok_r (قىممەت, "\n", &saveptr); قۇر; قۇر = strtok_r (عدم, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (اكتب (fd, قۇر, strlen (قۇر)));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "write to `%s`", resources->unified->ئاچقۇچلار[i]);
        }
    }

  أرجع 0;
}

ساكن صحيح
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path = عدم;
  cleanup_close صحيح cgroup_dirfd = -1;
  صحيح ret;

  إذا (resources->network)
    أرجع crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_يىلتىز, path, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (cgroup_dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_path);

  إذا (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  إذا (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  إذا (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  إذا (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  إذا (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
صحيح
check_memory_before_update (ثابت حرف *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free حرف *swap_current = عدم;
  cleanup_free حرف *cgroup_path = عدم;
  cleanup_free حرف *current = عدم;
  cleanup_close صحيح dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  صحيح cgroup_mode;
  صحيح ret;

  إذا (path == عدم || memory == عدم || ! (memory->check_before_update_present && memory->check_before_update))
    أرجع 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  إذا (UNLIKELY (cgroup_mode < 0))
    أرجع cgroup_mode;

  إذا (cgroup_mode != CGROUP_MODE_UNIFIED)
    أرجع 0;

  ret = append_paths (&cgroup_path, err, CGROUP_يىلتىز, path, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  errno = 0;
  val = strtoll (current, عدم, 10);
  إذا (UNLIKELY (errno))
    أرجع crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, عدم, 10);
  إذا (UNLIKELY (errno))
    أرجع crun_make_error (err, errno, "parse memory.swap.current");

  إذا (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  إذا (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  إذا (limit <= val + val_swap)
    أرجع crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  أرجع 0;
}

صحيح
update_cgroup_resources (ثابت حرف *path,
                         ثابت حرف *state_يىلتىز,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  صحيح cgroup_mode;

  (فراغ) state_يىلتىز;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  إذا (UNLIKELY (cgroup_mode < 0))
    أرجع cgroup_mode;

  إذا (path == عدم)
    {
      size_t i;

      إذا (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        أرجع crun_make_error (err, 0, "cannot set limits without cgroups");

      لكل (i = 0; i < resources->devices_len; i++)
        {
          صحيح rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          إذا (UNLIKELY (rwm < 0))
            أرجع rwm;

          إذا (rwm == 0)
            أرجع crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      أرجع 0;
    }
  بدل (cgroup_mode)
    {
    حالة CGROUP_MODE_UNIFIED:
      أرجع update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    حالة CGROUP_MODE_LEGACY:
    حالة CGROUP_MODE_HYBRID:
      أرجع update_cgroup_v1_resources (resources, path, err);

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
