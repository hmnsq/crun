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

#include <config.h>
#include "cgroup.h"
#include "cgroup-internal.h"
#include "cgroup-systemd.h"
#include "cgroup-utils.h"
#include "cgroup-resources.h"
#include "ebpf.h"
#include "utils.h"
#include "status.h"
#include <string.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <inttypes.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <libgen.h>

sa_nracanaa default_dev_s default_devices[] = {
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
  { 0, 0, 0, NULL }
};

sa_nracanaa default_dev_s *
get_default_devices ()
{
  ආපසු default_devices;
}

static පූර්ණ_සංඛ්‍යා
write_cgroup_file_or_alias (පූර්ණ_සංඛ්‍යා dirfd, const var_na *name, const var_na *alias, const void *data, size_t len, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  නම් (UNLIKELY (alias != NULL && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  ආපසු ret;
}

static anta_hstha පූර්ණ_සංඛ්‍යා
openat_with_alias (පූර්ණ_සංඛ්‍යා dirfd, const var_na *name, const var_na *alias, const var_na **used_name, පූර්ණ_සංඛ්‍යා flags, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  නම් (UNLIKELY (ret < 0 && alias != NULL && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  නම් (UNLIKELY (ret < 0))
    ආපසු crun_make_error (err, errno, "open `%s`", name);
  ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා
is_rwm (const var_na *str, libcrun_error_t *err)
{
  const var_na *it;
  bool r = false;
  bool w = false;
  bool m = false;

  නම් (str == NULL)
    str = "";

  සඳහා (it = str; *it; it++)
    switch (*it)
      {
      pak_sa 'r':
        r = true;
        bha_nga;

      pak_sa 'w':
        w = true;
        bha_nga;

      pak_sa 'm':
        m = true;
        bha_nga;

      default:
        ආපසු crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  ආපසු r && w && m ? 1 : 0;
}

static පූර්ණ_සංඛ්‍යා
check_cgroup_v2_controller_available_wrapper (පූර්ණ_සංඛ්‍යා ret, පූර්ණ_සංඛ්‍යා cgroup_dirfd, const var_na *name, libcrun_error_t *err)
{
  නම් (ret == 0 || err == NULL)
    ආපසු 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  නම් (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free var_na *controllers = NULL;
      libcrun_error_t tmp_err = NULL;
      cleanup_free var_na *key = NULL;
      var_na *saveptr = NULL;
      bool found = false;
      const var_na *token;
      var_na *it;

      /* Check if the specified controller is enabled.  */
      key = xstrdup (name);

      it = strchr (key, '.');
      නම් (it == NULL)
        {
          crun_error_release (err);
          ආපසු crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      නම් (strcmp (key, "cgroup") == 0)
        ආපසු ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      නම් (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, NULL, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          ආපසු ret;
        }
      සඳහා (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
        {
          නම් (strcmp (token, key) == 0)
            {
              found = true;
              bha_nga;
            }
        }
      නම් (! found)
        {
          cleanup_free var_na *absolute_path = NULL;
          libcrun_error_t tmp_err = NULL;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          නම් (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", key, absolute_path);
          නැතහොත්
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", key);
            }
        }
    }
  ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා
write_file_and_check_controllers_at (bool cgroup2, පූර්ණ_සංඛ්‍යා dirfd, const var_na *name, const var_na *name_alias,
                                     const void *data, size_t len, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  නම් (cgroup2)
    ආපසු check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා
open_file_and_check_controllers_at (bool cgroup2, පූර්ණ_සංඛ්‍යා dirfd, const var_na *name, පූර්ණ_සංඛ්‍යා flags, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;

  ret = openat (dirfd, name, flags);
  නම් (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      නම් (cgroup2)
        ආපසු check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  ආපසු ret;
}

/* The parser generates different structs but they are really all the same.  */
typedef runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

static පූර්ණ_සංඛ්‍යා
write_blkio_v1_resources_throttling (පූර්ණ_සංඛ්‍යා dirfd, const var_na *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  var_na fmt_buf[128];
  size_t i;
  cleanup_close පූර්ණ_සංඛ්‍යා fd = -1;

  නම් (throttling == NULL)
    ආපසු 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  නම් (UNLIKELY (fd < 0))
    ආපසු crun_make_error (err, errno, "open `%s`", name);

  සඳහා (i = 0; i < throttling_len; i++)
    {
      පූර්ණ_සංඛ්‍යා ret;
      පූර්ණ_සංඛ්‍යා len;
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      නම් (UNLIKELY (ret < 0))
        ආපසු crun_make_error (err, errno, "write `%s`", name);
    }
  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
write_blkio_v2_resources_throttling (පූර්ණ_සංඛ්‍යා fd, const var_na *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  var_na fmt_buf[128];
  size_t i;

  නම් (throttling == NULL)
    ආපසු 0;

  සඳහා (i = 0; i < throttling_len; i++)
    {
      පූර්ණ_සංඛ්‍යා ret;
      පූර්ණ_සංඛ්‍යා len;
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      නම් (UNLIKELY (ret < 0))
        ආපසු crun_make_error (err, errno, "write `%s`", name);
    }
  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
write_blkio_resources (පූර්ණ_සංඛ්‍යා dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  var_na fmt_buf[128];
  පූර්ණ_සංඛ්‍යා len;
  පූර්ණ_සංඛ්‍යා ret;

  නම් (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu32, val);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
      නම් (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          නම් (UNLIKELY (ret < 0))
            ආපසු ret;
        }
      නැතහොත්
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          නම් (UNLIKELY (ret < 0))
            {
              නම් (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu32, val);
                  නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
                    ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              නම් (UNLIKELY (ret < 0))
                ආපසු ret;
            }
        }
    }
  නම් (blkio->leaf_weight)
    {
      නම් (cgroup2)
        ආපසු crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%d", blkio->leaf_weight);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නම් (blkio->weight_device_len)
    {
      නම් (cgroup2)
        {
          cleanup_close පූර්ණ_සංඛ්‍යා wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          නම් (UNLIKELY (wfd < 0))
            ආපසු crun_make_error (err, errno, "open `io.bfq.weight`");
          සඳහා (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
                ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (wfd, fmt_buf, len));
              නම් (UNLIKELY (ret < 0))
                ආපසු crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      නැතහොත්
        {
          const var_na *leaf_weight_device_file_name = NULL;
          const var_na *weight_device_file_name = NULL;
          cleanup_close පූර්ණ_සංඛ්‍යා w_leafdevice_fd = -1;
          cleanup_close පූර්ණ_සංඛ්‍යා w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          නම් (UNLIKELY (w_device_fd < 0))
            ආපසු w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          නම් (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          සඳහා (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
                ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (w_device_fd, fmt_buf, len));
              නම් (UNLIKELY (ret < 0))
                ආපසු crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              නම් (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
                    ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (write (w_leafdevice_fd, fmt_buf, len));
                  නම් (UNLIKELY (ret < 0))
                    ආපසු crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  නම් (cgroup2)
    {
      cleanup_close පූර්ණ_සංඛ්‍යා wfd = -1;
      const var_na *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      නම් (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          ආපසු check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නැතහොත්
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
write_network_resources (පූර්ණ_සංඛ්‍යා dirfd_netclass, පූර්ණ_සංඛ්‍යා dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  var_na fmt_buf[128];
  පූර්ණ_සංඛ්‍යා len;
  පූර්ණ_සංඛ්‍යා ret;
  නම් (net->class_id)
    {
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu32, net->class_id);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නම් (net->priorities_len)
    {
      size_t i;
      cleanup_close පූර්ණ_සංඛ්‍යා fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      නම් (UNLIKELY (fd < 0))
        ආපසු crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      සඳහා (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
            ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
          නම් (UNLIKELY (ret < 0))
            ආපසු crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
write_hugetlb_resources (පූර්ණ_සංඛ්‍යා dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  const var_na *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  var_na fmt_buf[128];
  size_t i;
  සඳහා (i = 0; i < htlb_len; i++)
    {
      cleanup_free var_na *filename = NULL;
      cleanup_free var_na *rsvd_filename = NULL;
      පූර්ණ_සංඛ්‍යා len;
      පූර්ණ_සංඛ්‍යා ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, htlb[i]->limit);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, NULL, fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      නම් (skip_rsvd)
        continue;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        {
          නම් (crun_error_get_errno (err) != ENOENT)
            ආපසු ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
write_devices_resources_v1 (පූර්ණ_සංඛ්‍යා dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  පූර්ණ_සංඛ්‍යා len;
  පූර්ණ_සංඛ්‍යා ret;

  සඳහා (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      var_na fmt_buf[FMT_BUF_LEN];
      const var_na *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      නම් (devs[i]->type == NULL || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      නැතහොත්
        {
          var_na fmt_buf_major[16];
          var_na fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  do                                                                                    \
    {                                                                                   \
      නම් (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, parimaa_na (b), "%" PRIi64, x);                                \
          නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (b)))                                       \
            ආපසු crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      නැතහොත්                                                                              \
        strcpy (b, "*");                                                                \
  } අතරතුර (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          නම් (UNLIKELY (len >= FMT_BUF_LEN))
            ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  සඳහා (i = 0; default_devices[i].type; i++)
    {
      var_na fmt_buf_major[16];
      var_na fmt_buf_minor[16];
      var_na device[64];
      පූර්ණ_සංඛ්‍යා len;

#define FMT_DEV(x, b)                                                                   \
  do                                                                                    \
    {                                                                                   \
      නම් (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, parimaa_na (b), "%d", x);                                      \
          නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (b)))                                       \
            ආපසු crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      නැතහොත්                                                                              \
        strcpy (b, "*");                                                                \
  } අතරතුර (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, parimaa_na (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (device)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  ආපසු 0;
}

sa_nracanaa bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා i;
  sa_nracanaa bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  නම් (UNLIKELY (program == NULL))
    ආපසු NULL;

  සඳහා (i = (parimaa_na (default_devices) / parimaa_na (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      නම් (UNLIKELY (program == NULL))
        ආපසු NULL;
    }
  සඳහා (i = devs_len - 1; i >= 0; i--)
    {
      var_na type = 'a';
      පූර්ණ_සංඛ්‍යා minor = -1, major = -1;
      නම් (devs[i]->type != NULL)
        type = devs[i]->type[0];

      නම් (devs[i]->major_present)
        major = devs[i]->major;
      නම් (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      නම් (UNLIKELY (program == NULL))
        ආපසු NULL;
    }

  program = bpf_program_complete_dev (program, err);
  නම් (UNLIKELY (program == NULL))
    ආපසු NULL;

  ආපසු program;
}

static පූර්ණ_සංඛ්‍යා
write_devices_resources_v2_internal (පූර්ණ_සංඛ්‍යා dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free sa_nracanaa bpf_program *program = NULL;

  program = create_dev_bpf (devs, devs_len, err);
  නම් (UNLIKELY (program == NULL))
    ආපසු -1;

  ආපසු libcrun_ebpf_load (program, dirfd, NULL, err);
}

static පූර්ණ_සංඛ්‍යා
write_devices_resources_v2 (පූර්ණ_සංඛ්‍යා dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  නම් (LIKELY (ret == 0))
    ආපසු 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  සඳහා (i = 0; i < devs_len; i++)
    {
      නම් (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          bha_nga;
        }
    }

  නම් (! can_skip)
    {
      libcrun_error_t tmp_err = NULL;
      පූර්ණ_සංඛ්‍යා rootless;

      rootless = is_rootless (&tmp_err);
      නම් (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ආපසු ret;
        }
      නම් (rootless)
        can_skip = true;
    }

  නම් (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා
write_devices_resources (පූර්ණ_සංඛ්‍යා dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;

  නම් (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  නැතහොත්
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  නම් (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = NULL;
      පූර්ණ_සංඛ්‍යා rootless;

      rootless = is_rootless (&tmp_err);
      නම් (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          ආපසු ret;
        }

      නම් (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  ආපසු ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
static පූර්ණ_සංඛ්‍යා
cg_itoa (var_na *buf, size_t size, int64_t value, bool use_max, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා len;

  නම් (use_max && value < 0)
    len = snprintf (buf, size, "max");
  නැතහොත්
    len = snprintf (buf, size, "%" PRIi64, value);

  නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) size))
    ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

  ආපසු len;
}

static පූර්ණ_සංඛ්‍යා
write_memory (පූර්ණ_සංඛ්‍යා dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  var_na limit_buf[32];
  පූර්ණ_සංඛ්‍යා limit_buf_len;

  නම් (! memory->limit_present)
    ආපසු 0;

  limit_buf_len = cg_itoa (limit_buf, parimaa_na (limit_buf), memory->limit, cgroup2, err);
  නම් (UNLIKELY (limit_buf_len < 0))
    ආපසු limit_buf_len;

  ආපසු write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

static පූර්ණ_සංඛ්‍යා
write_memory_swap (පූර්ණ_සංඛ්‍යා dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;
  int64_t swap;
  var_na swap_buf[32];
  පූර්ණ_සංඛ්‍යා len;
  const var_na *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  නම් (! memory->swap_present)
    ආපසු 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  නම් (cgroup2 && memory->swap > 0)
    {
      නම් (! memory->limit_present)
        ආපසු crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      නම් (memory->swap < memory->limit)
        ආපසු crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, parimaa_na (swap_buf), swap, cgroup2, err);
  නම් (UNLIKELY (len < 0))
    ආපසු len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  නම් (ret >= 0)
    ආපසු ret;

  /* If swap is not enabled, ignore the error.  */
  නම් (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      ආපසු 0;
    }

  ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා
write_memory_resources (පූර්ණ_සංඛ්‍යා dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා len;
  පූර්ණ_සංඛ්‍යා ret;
  var_na fmt_buf[32];
  bool memory_limits_written = false;

  නම් (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      නම් (ret >= 0)
        memory_limits_written = true;
      නැතහොත්
        {
          නම් (cgroup2 || crun_error_get_errno (err) != EINVAL)
            ආපසු ret;

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
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  නම් (memory->kernel_present)
    {
      නම් (cgroup2)
        ආපසු crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->kernel);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  නම් (memory->use_hierarchy_present)
    {
      නම් (cgroup2)
        ආපසු crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  නම් (memory->reservation_present)
    {
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->reservation);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 NULL, fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නම් (memory->disable_oom_killer)
    {
      නම් (cgroup2)
        ආපසු crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නම් (memory->kernel_tcp_present)
    {
      නම් (cgroup2)
        ආපසු crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නම් (memory->swappiness_present)
    {
      නම් (cgroup2)
        ආපසු crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->swappiness);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  ආපසු 0;
}

පූර්ණ_සංඛ්‍යා
write_cpu_idle (පූර්ණ_සංඛ්‍යා cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  var_na fmt_buf[32];
  පූර්ණ_සංඛ්‍යා len;

  නම් (! cpu->idle_present)
    ආපසු 0;

  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64, cpu->idle);
  නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
    ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
  ආපසු write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

පූර්ණ_සංඛ්‍යා
write_cpu_burst (පූර්ණ_සංඛ්‍යා cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  var_na fmt_buf[32];
  පූර්ණ_සංඛ්‍යා len;

  නම් (! cpu->burst_present)
    ආපසු 0;

  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64, cpu->burst);
  නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
    ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
  ආපසු write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

static පූර්ණ_සංඛ්‍යා
write_pids_resources (පූර්ණ_සංඛ්‍යා dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  නම් (pids->limit)
    {
      var_na fmt_buf[32];
      පූර්ණ_සංඛ්‍යා len;
      පූර්ණ_සංඛ්‍යා ret;

      len = cg_itoa (fmt_buf, parimaa_na (fmt_buf), pids->limit, true, err);
      නම් (UNLIKELY (len < 0))
        ආපසු len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", NULL, fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
write_cpu_resources (පූර්ණ_සංඛ්‍යා dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා len, period_len = 0;
  පූර්ණ_සංඛ්‍යා ret;
  var_na fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free var_na *period_str = NULL;

  නම් (cpu->shares)
    {
      uint32_t val = cpu->shares;

      නම් (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          නම් (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              නම් (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%u", val);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 NULL, fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නම් (cpu->period)
    {
      නම් (cgroup2)
        period = cpu->period;
      නැතහොත්
        {
          len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, cpu->period);
          නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
            ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          නම් (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              නම් (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                ආපසු ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  නම් (cpu->quota)
    {
      නම් (cgroup2)
        quota = cpu->quota;
      නැතහොත්
        {
          len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64, cpu->quota);
          නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
            ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          නම් (UNLIKELY (ret < 0))
            ආපසු ret;
          නම් (period_str != NULL)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              නම් (UNLIKELY (ret < 0))
                ආපසු ret;
            }
        }
    }
  නම් (cpu->realtime_period)
    {
      නම් (cgroup2)
        ආපසු crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, cpu->realtime_period);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නම් (cpu->realtime_runtime)
    {
      නම් (cgroup2)
        ආපසු crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (cgroup2 && (quota > 0 || period > 0))
    {
      නම් (period < 0)
        period = 100000;
      නම් (quota < 0)
        len = snprintf (fmt_buf, parimaa_na (fmt_buf), "max %" PRIi64, period);
      නැතහොත්
        len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      නම් (UNLIKELY (len >= (පූර්ණ_සංඛ්‍යා) parimaa_na (fmt_buf)))
        ආපසු crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", NULL, fmt_buf, len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  ආපසු write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

පූර්ණ_සංඛ්‍යා
write_cpuset_resources (පූර්ණ_සංඛ්‍යා dirfd_cpuset, පූර්ණ_සංඛ්‍යා cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;

  නම් (cpu == NULL)
    ආපසු 0;

  නම් (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නම් (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
open_cgroup_subsystem (const var_na *subsystem, const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *full_path = NULL;
  පූර්ණ_සංඛ්‍යා ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_ROOT, subsystem, path, NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  නම් (UNLIKELY (dirfd < 0))
    ආපසු crun_make_error (err, errno, "open `%s`", full_path);

  ආපසු dirfd;
}

පූර්ණ_සංඛ්‍යා
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, const var_na *path, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;

  නම් (resources->block_io)
    {
      cleanup_close පූර්ණ_සංඛ්‍යා dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      නම් (UNLIKELY (dirfd_blkio < 0))
        ආපසු dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  නම් (resources->network)
    {
      cleanup_close පූර්ණ_සංඛ්‍යා dirfd_netclass = -1;
      cleanup_close පූර්ණ_සංඛ්‍යා dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      නම් (UNLIKELY (dirfd_netclass < 0))
        ආපසු dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      නම් (UNLIKELY (dirfd_netprio < 0))
        ආපසු dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  නම් (resources->hugepage_limits_len)
    {
      cleanup_close පූර්ණ_සංඛ්‍යා dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      නම් (UNLIKELY (dirfd_htlb < 0))
        ආපසු dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  නම් (resources->devices_len)
    {
      cleanup_close පූර්ණ_සංඛ්‍යා dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      නම් (UNLIKELY (dirfd_devs < 0))
        ආපසු dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  නම් (resources->memory)
    {
      cleanup_close පූර්ණ_සංඛ්‍යා dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      නම් (UNLIKELY (dirfd_mem < 0))
        ආපසු dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  නම් (resources->pids)
    {
      cleanup_close පූර්ණ_සංඛ්‍යා dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      නම් (UNLIKELY (dirfd_pid < 0))
        ආපසු dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  නම් (resources->cpu)
    {
      cleanup_close පූර්ණ_සංඛ්‍යා dirfd_cpu = -1;
      cleanup_close පූර්ණ_සංඛ්‍යා dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      නම් (UNLIKELY (dirfd_cpu < 0))
        ආපසු dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      නම් (resources->cpu->cpus == NULL && resources->cpu->mems == NULL)
        ආපසු 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      නම් (UNLIKELY (dirfd_cpuset < 0))
        ආපසු dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  නම් (resources->unified && resources->unified->len > 0)
    ආපසු crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
write_unified_resources (පූර්ණ_සංඛ්‍යා cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  පූර්ණ_සංඛ්‍යා ret;

  සඳහා (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close පූර්ණ_සංඛ්‍යා fd = -1;
      cleanup_free var_na *value = NULL;
      var_na *saveptr = NULL;
      var_na *line;

      නම් (strchr (resources->unified->keys[i], '/'))
        ආපසු crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->keys[i]);

      නම් (is_empty_string (resources->unified->values[i]))
        continue;

      value = xstrdup (resources->unified->values[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->keys[i], O_WRONLY, err);
      නම් (UNLIKELY (fd < 0))
        ආපසු fd;

      සඳහා (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (NULL, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (write (fd, line, strlen (line)));
          නම් (UNLIKELY (ret < 0))
            ආපසු crun_make_error (err, errno, "write to `%s`", resources->unified->keys[i]);
        }
    }

  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, const var_na *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_close පූර්ණ_සංඛ්‍යා cgroup_dirfd = -1;
  පූර්ණ_සංඛ්‍යා ret;

  නම් (resources->network)
    ආපසු crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  නම් (UNLIKELY (cgroup_dirfd < 0))
    ආපසු crun_make_error (err, errno, "open `%s`", cgroup_path);

  නම් (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  නම් (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නම් (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නම් (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නම් (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  නම් (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  නම් (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  ආපසු 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
පූර්ණ_සංඛ්‍යා
check_memory_before_update (const var_na *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free var_na *swap_current = NULL;
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_free var_na *current = NULL;
  cleanup_close පූර්ණ_සංඛ්‍යා dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  පූර්ණ_සංඛ්‍යා cgroup_mode;
  පූර්ණ_සංඛ්‍යා ret;

  නම් (path == NULL || memory == NULL || ! (memory->check_before_update_present && memory->check_before_update))
    ආපසු 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (cgroup_mode < 0))
    ආපසු cgroup_mode;

  නම් (cgroup_mode != CGROUP_MODE_UNIFIED)
    ආපසු 0;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  නම් (UNLIKELY (dirfd < 0))
    ආපසු crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, NULL, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, NULL, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  errno = 0;
  val = strtoll (current, NULL, 10);
  නම් (UNLIKELY (errno))
    ආපසු crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, NULL, 10);
  නම් (UNLIKELY (errno))
    ආපසු crun_make_error (err, errno, "parse memory.swap.current");

  නම් (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  නම් (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  නම් (limit <= val + val_swap)
    ආපසු crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  ආපසු 0;
}

පූර්ණ_සංඛ්‍යා
update_cgroup_resources (const var_na *path,
                         const var_na *state_root,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා cgroup_mode;

  (void) state_root;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (cgroup_mode < 0))
    ආපසු cgroup_mode;

  නම් (path == NULL)
    {
      size_t i;

      නම් (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        ආපසු crun_make_error (err, 0, "cannot set limits without cgroups");

      සඳහා (i = 0; i < resources->devices_len; i++)
        {
          පූර්ණ_සංඛ්‍යා rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          නම් (UNLIKELY (rwm < 0))
            ආපසු rwm;

          නම් (rwm == 0)
            ආපසු crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      ආපසු 0;
    }
  switch (cgroup_mode)
    {
    pak_sa CGROUP_MODE_UNIFIED:
      ආපසු update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    pak_sa CGROUP_MODE_LEGACY:
    pak_sa CGROUP_MODE_HYBRID:
      ආපසු update_cgroup_v1_resources (resources, path, err);

    default:
      ආපසු crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
