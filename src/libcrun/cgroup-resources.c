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
  החזר default_devices;
}

ساكن صحيح
write_cgroup_file_or_alias (صحيح dirfd, ثابت حرف *name, ثابت حرف *alias, ثابت فراغ *data, size_t len, libcrun_error_t *err)
{
  صحيح ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  אם (UNLIKELY (alias != عدم && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  החזר ret;
}

ساكن مضمن صحيح
openat_with_alias (صحيح dirfd, ثابت حرف *name, ثابت حرف *alias, ثابت حرف **used_name, صحيح flags, libcrun_error_t *err)
{
  صحيح ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  אם (UNLIKELY (ret < 0 && alias != عدم && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "open `%s`", name);
  החזר ret;
}

ساكن صحيح
is_rwm (ثابت حرف *str, libcrun_error_t *err)
{
  ثابت حرف *it;
  bool r = false;
  bool w = false;
  bool m = false;

  אם (str == عدم)
    str = "";

  עבור (it = str; *it; it++)
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
        החזר crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  החזר r && w && m ? 1 : 0;
}

ساكن صحيح
check_cgroup_v2_controller_available_wrapper (صحيح ret, صحيح cgroup_dirfd, ثابت حرف *name, libcrun_error_t *err)
{
  אם (ret == 0 || err == عدم)
    החזר 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  אם (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free حرف *controllers = عدم;
      libcrun_error_t tmp_err = عدم;
      cleanup_free حرف *key = عدم;
      حرف *saveptr = عدم;
      bool found = false;
      ثابت حرف *token;
      حرف *it;

      /* Check if the specified controller is enabled.  */
      key = xstrdup (name);

      it = strchr (key, '.');
      אם (it == عدم)
        {
          crun_error_release (err);
          החזר crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      אם (strcmp (key, "cgroup") == 0)
        החזר ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      אם (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, عدم, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          החזר ret;
        }
      עבור (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (عدم, " \n", &saveptr))
        {
          אם (strcmp (token, key) == 0)
            {
              found = true;
              اكسر;
            }
        }
      אם (! found)
        {
          cleanup_free حرف *absolute_path = عدم;
          libcrun_error_t tmp_err = عدم;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          אם (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", key, absolute_path);
          אחרת
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", key);
            }
        }
    }
  החזר ret;
}

ساكن صحيح
write_file_and_check_controllers_at (bool cgroup2, صحيح dirfd, ثابت حرف *name, ثابت حرف *name_alias,
                                     ثابت فراغ *data, size_t len, libcrun_error_t *err)
{
  صحيح ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  אם (cgroup2)
    החזר check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  החזר ret;
}

ساكن صحيح
open_file_and_check_controllers_at (bool cgroup2, صحيح dirfd, ثابت حرف *name, صحيح flags, libcrun_error_t *err)
{
  صحيح ret;

  ret = openat (dirfd, name, flags);
  אם (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      אם (cgroup2)
        החזר check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  החזר ret;
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

  אם (throttling == عدم)
    החזר 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  אם (UNLIKELY (fd < 0))
    החזר crun_make_error (err, errno, "open `%s`", name);

  עבור (i = 0; i < throttling_len; i++)
    {
      صحيح ret;
      صحيح len;
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
      אם (UNLIKELY (ret < 0))
        החזר crun_make_error (err, errno, "write `%s`", name);
    }
  החזר 0;
}

ساكن صحيح
write_blkio_v2_resources_throttling (صحيح fd, ثابت حرف *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  حرف fmt_buf[128];
  size_t i;

  אם (throttling == عدم)
    החזר 0;

  עבור (i = 0; i < throttling_len; i++)
    {
      صحيح ret;
      صحيح len;
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
      אם (UNLIKELY (ret < 0))
        החזר crun_make_error (err, errno, "write `%s`", name);
    }
  החזר 0;
}

ساكن صحيح
write_blkio_resources (صحيح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  حرف fmt_buf[128];
  صحيح len;
  صحيح ret;

  אם (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu32, val);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");
      אם (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }
      אחרת
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          אם (UNLIKELY (ret < 0))
            {
              אם (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu32, val);
                  אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
                    החזר crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              אם (UNLIKELY (ret < 0))
                החזר ret;
            }
        }
    }
  אם (blkio->leaf_weight)
    {
      אם (cgroup2)
        החזר crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%d", blkio->leaf_weight);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אם (blkio->weight_device_len)
    {
      אם (cgroup2)
        {
          cleanup_close صحيح wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          אם (UNLIKELY (wfd < 0))
            החזר crun_make_error (err, errno, "open `io.bfq.weight`");
          עבור (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
                החזר crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (اكتب (wfd, fmt_buf, len));
              אם (UNLIKELY (ret < 0))
                החזר crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      אחרת
        {
          ثابت حرف *leaf_weight_device_file_name = عدم;
          ثابت حرف *weight_device_file_name = عدم;
          cleanup_close صحيح w_leafdevice_fd = -1;
          cleanup_close صحيح w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          אם (UNLIKELY (w_device_fd < 0))
            החזר w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          אם (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          עבור (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
                החזר crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (اكتب (w_device_fd, fmt_buf, len));
              אם (UNLIKELY (ret < 0))
                החזר crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              אם (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
                    החזר crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (اكتب (w_leafdevice_fd, fmt_buf, len));
                  אם (UNLIKELY (ret < 0))
                    החזר crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  אם (cgroup2)
    {
      cleanup_close صحيح wfd = -1;
      ثابت حرف *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      אם (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          החזר check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אחרת
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  החזר 0;
}

ساكن صحيح
write_network_resources (صحيح dirfd_netclass, صحيح dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  حرف fmt_buf[128];
  صحيح len;
  صحيح ret;
  אם (net->class_id)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu32, net->class_id);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אם (net->priorities_len)
    {
      size_t i;
      cleanup_close صحيح fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      אם (UNLIKELY (fd < 0))
        החזר crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      עבור (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, حجم (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
            החזר crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (اكتب (fd, fmt_buf, len));
          אם (UNLIKELY (ret < 0))
            החזר crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  החזר 0;
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
  עבור (i = 0; i < htlb_len; i++)
    {
      cleanup_free حرف *filename = عدم;
      cleanup_free حرف *rsvd_filename = عدم;
      صحيح len;
      صحيح ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, htlb[i]->limit);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, عدم, fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      אם (skip_rsvd)
        استمر;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        {
          אם (crun_error_get_errno (err) != ENOENT)
            החזר ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  החזר 0;
}

ساكن صحيح
write_devices_resources_v1 (صحيح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  صحيح len;
  صحيح ret;

  עבור (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      حرف fmt_buf[FMT_BUF_LEN];
      ثابت حرف *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      אם (devs[i]->type == عدم || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      אחרת
        {
          حرف fmt_buf_major[16];
          حرف fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  עשה                                                                                    \
    {                                                                                   \
      אם (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, حجم (b), "%" PRIi64, x);                                \
          אם (UNLIKELY (len >= (صحيح) حجم (b)))                                       \
            החזר crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      אחרת                                                                              \
        strcpy (b, "*");                                                                \
  } כלעוד (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          אם (UNLIKELY (len >= FMT_BUF_LEN))
            החזר crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  עבור (i = 0; default_devices[i].type; i++)
    {
      حرف fmt_buf_major[16];
      حرف fmt_buf_minor[16];
      حرف device[64];
      صحيح len;

#define FMT_DEV(x, b)                                                                   \
  עשה                                                                                    \
    {                                                                                   \
      אם (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, حجم (b), "%d", x);                                      \
          אם (UNLIKELY (len >= (صحيح) حجم (b)))                                       \
            החזר crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      אחרת                                                                              \
        strcpy (b, "*");                                                                \
  } כלעוד (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, حجم (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      אם (UNLIKELY (len >= (صحيح) حجم (device)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  החזר 0;
}

بنية bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  صحيح i;
  بنية bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  אם (UNLIKELY (program == عدم))
    החזר عدم;

  עבור (i = (حجم (default_devices) / حجم (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      אם (UNLIKELY (program == عدم))
        החזר عدم;
    }
  עבור (i = devs_len - 1; i >= 0; i--)
    {
      حرف type = 'a';
      صحيح minor = -1, major = -1;
      אם (devs[i]->type != عدم)
        type = devs[i]->type[0];

      אם (devs[i]->major_present)
        major = devs[i]->major;
      אם (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      אם (UNLIKELY (program == عدم))
        החזר عدم;
    }

  program = bpf_program_complete_dev (program, err);
  אם (UNLIKELY (program == عدم))
    החזר عدم;

  החזר program;
}

ساكن صحيح
write_devices_resources_v2_internal (صحيح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free بنية bpf_program *program = عدم;

  program = create_dev_bpf (devs, devs_len, err);
  אם (UNLIKELY (program == عدم))
    החזר -1;

  החזר libcrun_ebpf_load (program, dirfd, عدم, err);
}

ساكن صحيح
write_devices_resources_v2 (صحيح dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  صحيح ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  אם (LIKELY (ret == 0))
    החזר 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  עבור (i = 0; i < devs_len; i++)
    {
      אם (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          اكسر;
        }
    }

  אם (! can_skip)
    {
      libcrun_error_t tmp_err = عدم;
      صحيح rootless;

      rootless = is_rootless (&tmp_err);
      אם (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          החזר ret;
        }
      אם (rootless)
        can_skip = true;
    }

  אם (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  החזר ret;
}

ساكن صحيح
write_devices_resources (صحيح dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  صحيح ret;

  אם (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  אחרת
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  אם (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = عدم;
      صحيح rootless;

      rootless = is_rootless (&tmp_err);
      אם (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          החזר ret;
        }

      אם (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  החזר ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
ساكن صحيح
cg_itoa (حرف *buf, size_t size, int64_t value, bool use_max, libcrun_error_t *err)
{
  صحيح len;

  אם (use_max && value < 0)
    len = snprintf (buf, size, "max");
  אחרת
    len = snprintf (buf, size, "%" PRIi64, value);

  אם (UNLIKELY (len >= (صحيح) size))
    החזר crun_make_error (err, 0, "internal error: static buffer too small");

  החזר len;
}

ساكن صحيح
write_memory (صحيح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  حرف limit_buf[32];
  صحيح limit_buf_len;

  אם (! memory->limit_present)
    החזר 0;

  limit_buf_len = cg_itoa (limit_buf, حجم (limit_buf), memory->limit, cgroup2, err);
  אם (UNLIKELY (limit_buf_len < 0))
    החזר limit_buf_len;

  החזר write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
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

  אם (! memory->swap_present)
    החזר 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  אם (cgroup2 && memory->swap > 0)
    {
      אם (! memory->limit_present)
        החזר crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      אם (memory->swap < memory->limit)
        החזר crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, حجم (swap_buf), swap, cgroup2, err);
  אם (UNLIKELY (len < 0))
    החזר len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  אם (ret >= 0)
    החזר ret;

  /* If swap is not enabled, ignore the error.  */
  אם (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      החזר 0;
    }

  החזר ret;
}

ساكن صحيح
write_memory_resources (صحيح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  صحيح len;
  صحيح ret;
  حرف fmt_buf[32];
  bool memory_limits_written = false;

  אם (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      אם (ret >= 0)
        memory_limits_written = true;
      אחרת
        {
          אם (cgroup2 || crun_error_get_errno (err) != EINVAL)
            החזר ret;

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
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (memory->kernel_present)
    {
      אם (cgroup2)
        החזר crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, memory->kernel);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  אם (memory->use_hierarchy_present)
    {
      אם (cgroup2)
        החזר crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (memory->reservation_present)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, memory->reservation);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 عدم, fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אם (memory->disable_oom_killer)
    {
      אם (cgroup2)
        החזר crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אם (memory->kernel_tcp_present)
    {
      אם (cgroup2)
        החזר crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אם (memory->swappiness_present)
    {
      אם (cgroup2)
        החזר crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, memory->swappiness);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  החזר 0;
}

صحيح
write_cpu_idle (صحيح cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  حرف fmt_buf[32];
  صحيح len;

  אם (! cpu->idle_present)
    החזר 0;

  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64, cpu->idle);
  אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
    החזר crun_make_error (err, 0, "internal error: static buffer too small");
  החזר write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

صحيح
write_cpu_burst (صحيح cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  حرف fmt_buf[32];
  صحيح len;

  אם (! cpu->burst_present)
    החזר 0;

  len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64, cpu->burst);
  אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
    החזר crun_make_error (err, 0, "internal error: static buffer too small");
  החזר write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

ساكن صحيح
write_pids_resources (صحيح dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  אם (pids->limit)
    {
      حرف fmt_buf[32];
      صحيح len;
      صحيح ret;

      len = cg_itoa (fmt_buf, حجم (fmt_buf), pids->limit, true, err);
      אם (UNLIKELY (len < 0))
        החזר len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", عدم, fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  החזר 0;
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

  אם (cpu->shares)
    {
      uint32_t val = cpu->shares;

      אם (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          אם (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              אם (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, حجم (fmt_buf), "%u", val);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 عدم, fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אם (cpu->period)
    {
      אם (cgroup2)
        period = cpu->period;
      אחרת
        {
          len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, cpu->period);
          אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
            החזר crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          אם (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              אם (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                החזר ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  אם (cpu->quota)
    {
      אם (cgroup2)
        quota = cpu->quota;
      אחרת
        {
          len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64, cpu->quota);
          אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
            החזר crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
          אם (period_str != عدم)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              אם (UNLIKELY (ret < 0))
                החזר ret;
            }
        }
    }
  אם (cpu->realtime_period)
    {
      אם (cgroup2)
        החזר crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, cpu->realtime_period);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אם (cpu->realtime_runtime)
    {
      אם (cgroup2)
        החזר crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (cgroup2 && (quota > 0 || period > 0))
    {
      אם (period < 0)
        period = 100000;
      אם (quota < 0)
        len = snprintf (fmt_buf, حجم (fmt_buf), "max %" PRIi64, period);
      אחרת
        len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", عدم, fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  החזר write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

صحيح
write_cpuset_resources (صحيح dirfd_cpuset, صحيح cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  صحيح ret;

  אם (cpu == عدم)
    החזר 0;

  אם (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אם (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  החזר 0;
}

ساكن صحيح
open_cgroup_subsystem (ثابت حرف *subsystem, ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *full_path = عدم;
  صحيح ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_ROOT, subsystem, path, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  אם (UNLIKELY (dirfd < 0))
    החזר crun_make_error (err, errno, "open `%s`", full_path);

  החזר dirfd;
}

صحيح
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *path, libcrun_error_t *err)
{
  صحيح ret;

  אם (resources->block_io)
    {
      cleanup_close صحيح dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      אם (UNLIKELY (dirfd_blkio < 0))
        החזר dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (resources->network)
    {
      cleanup_close صحيح dirfd_netclass = -1;
      cleanup_close صحيح dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      אם (UNLIKELY (dirfd_netclass < 0))
        החזר dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      אם (UNLIKELY (dirfd_netprio < 0))
        החזר dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (resources->hugepage_limits_len)
    {
      cleanup_close صحيح dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      אם (UNLIKELY (dirfd_htlb < 0))
        החזר dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (resources->devices_len)
    {
      cleanup_close صحيح dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      אם (UNLIKELY (dirfd_devs < 0))
        החזר dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (resources->memory)
    {
      cleanup_close صحيح dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      אם (UNLIKELY (dirfd_mem < 0))
        החזר dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (resources->pids)
    {
      cleanup_close صحيح dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      אם (UNLIKELY (dirfd_pid < 0))
        החזר dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (resources->cpu)
    {
      cleanup_close صحيح dirfd_cpu = -1;
      cleanup_close صحيح dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      אם (UNLIKELY (dirfd_cpu < 0))
        החזר dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      אם (resources->cpu->cpus == عدم && resources->cpu->mems == عدم)
        החזר 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      אם (UNLIKELY (dirfd_cpuset < 0))
        החזר dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (resources->unified && resources->unified->len > 0)
    החזר crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  החזר 0;
}

ساكن صحيح
write_unified_resources (صحيح cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  صحيح ret;

  עבור (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close صحيح fd = -1;
      cleanup_free حرف *value = عدم;
      حرف *saveptr = عدم;
      حرف *line;

      אם (strchr (resources->unified->keys[i], '/'))
        החזר crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->keys[i]);

      אם (is_empty_string (resources->unified->values[i]))
        استمر;

      value = xstrdup (resources->unified->values[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->keys[i], O_WRONLY, err);
      אם (UNLIKELY (fd < 0))
        החזר fd;

      עבור (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (عدم, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (اكتب (fd, line, strlen (line)));
          אם (UNLIKELY (ret < 0))
            החזר crun_make_error (err, errno, "write to `%s`", resources->unified->keys[i]);
        }
    }

  החזר 0;
}

ساكن صحيح
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path = عدم;
  cleanup_close صحيح cgroup_dirfd = -1;
  صحيح ret;

  אם (resources->network)
    החזר crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  אם (UNLIKELY (cgroup_dirfd < 0))
    החזר crun_make_error (err, errno, "open `%s`", cgroup_path);

  אם (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אם (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אם (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אם (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  אם (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  החזר 0;
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

  אם (path == عدم || memory == عدم || ! (memory->check_before_update_present && memory->check_before_update))
    החזר 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  אם (cgroup_mode != CGROUP_MODE_UNIFIED)
    החזר 0;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  אם (UNLIKELY (dirfd < 0))
    החזר crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, عدم, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, عدم, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  errno = 0;
  val = strtoll (current, عدم, 10);
  אם (UNLIKELY (errno))
    החזר crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, عدم, 10);
  אם (UNLIKELY (errno))
    החזר crun_make_error (err, errno, "parse memory.swap.current");

  אם (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  אם (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  אם (limit <= val + val_swap)
    החזר crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  החזר 0;
}

صحيح
update_cgroup_resources (ثابت حرف *path,
                         ثابت حرف *state_root,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  صحيح cgroup_mode;

  (فراغ) state_root;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  אם (path == عدم)
    {
      size_t i;

      אם (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        החזר crun_make_error (err, 0, "cannot set limits without cgroups");

      עבור (i = 0; i < resources->devices_len; i++)
        {
          صحيح rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          אם (UNLIKELY (rwm < 0))
            החזר rwm;

          אם (rwm == 0)
            החזר crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      החזר 0;
    }
  بدل (cgroup_mode)
    {
    حالة CGROUP_MODE_UNIFIED:
      החזר update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    حالة CGROUP_MODE_LEGACY:
    حالة CGROUP_MODE_HYBRID:
      החזר update_cgroup_v1_resources (resources, path, err);

    افتراضي:
      החזר crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
