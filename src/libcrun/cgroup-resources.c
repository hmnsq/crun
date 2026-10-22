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
  ལོག default_devices;
}

static ཧྲིལ
write_cgroup_file_or_alias (ཧྲིལ dirfd, const var_na *name, const var_na *alias, const void *data, size_t len, libcrun_error_t *err)
{
  ཧྲིལ ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  གལ་སྲིད (UNLIKELY (alias != NULL && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  ལོག ret;
}

static anta_hstha ཧྲིལ
openat_with_alias (ཧྲིལ dirfd, const var_na *name, const var_na *alias, const var_na **used_name, ཧྲིལ flags, libcrun_error_t *err)
{
  ཧྲིལ ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  གལ་སྲིད (UNLIKELY (ret < 0 && alias != NULL && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  གལ་སྲིད (UNLIKELY (ret < 0))
    ལོག crun_make_error (err, errno, "open `%s`", name);
  ལོག ret;
}

static ཧྲིལ
is_rwm (const var_na *str, libcrun_error_t *err)
{
  const var_na *it;
  bool r = false;
  bool w = false;
  bool m = false;

  གལ་སྲིད (str == NULL)
    str = "";

  དོན་ལུ (it = str; *it; it++)
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
        ལོག crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  ལོག r && w && m ? 1 : 0;
}

static ཧྲིལ
check_cgroup_v2_controller_available_wrapper (ཧྲིལ ret, ཧྲིལ cgroup_dirfd, const var_na *name, libcrun_error_t *err)
{
  གལ་སྲིད (ret == 0 || err == NULL)
    ལོག 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  གལ་སྲིད (errno == ENOENT || errno == EPERM || errno == EACCES)
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
      གལ་སྲིད (it == NULL)
        {
          crun_error_release (err);
          ལོག crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      གལ་སྲིད (strcmp (key, "cgroup") == 0)
        ལོག ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      གལ་སྲིད (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, NULL, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          ལོག ret;
        }
      དོན་ལུ (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
        {
          གལ་སྲིད (strcmp (token, key) == 0)
            {
              found = true;
              bha_nga;
            }
        }
      གལ་སྲིད (! found)
        {
          cleanup_free var_na *absolute_path = NULL;
          libcrun_error_t tmp_err = NULL;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          གལ་སྲིད (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", key, absolute_path);
          དེ་མིན
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", key);
            }
        }
    }
  ལོག ret;
}

static ཧྲིལ
write_file_and_check_controllers_at (bool cgroup2, ཧྲིལ dirfd, const var_na *name, const var_na *name_alias,
                                     const void *data, size_t len, libcrun_error_t *err)
{
  ཧྲིལ ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  གལ་སྲིད (cgroup2)
    ལོག check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  ལོག ret;
}

static ཧྲིལ
open_file_and_check_controllers_at (bool cgroup2, ཧྲིལ dirfd, const var_na *name, ཧྲིལ flags, libcrun_error_t *err)
{
  ཧྲིལ ret;

  ret = openat (dirfd, name, flags);
  གལ་སྲིད (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      གལ་སྲིད (cgroup2)
        ལོག check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  ལོག ret;
}

/* The parser generates different structs but they are really all the same.  */
typedef runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

static ཧྲིལ
write_blkio_v1_resources_throttling (ཧྲིལ dirfd, const var_na *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  var_na fmt_buf[128];
  size_t i;
  cleanup_close ཧྲིལ fd = -1;

  གལ་སྲིད (throttling == NULL)
    ལོག 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  གལ་སྲིད (UNLIKELY (fd < 0))
    ལོག crun_make_error (err, errno, "open `%s`", name);

  དོན་ལུ (i = 0; i < throttling_len; i++)
    {
      ཧྲིལ ret;
      ཧྲིལ len;
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག crun_make_error (err, errno, "write `%s`", name);
    }
  ལོག 0;
}

static ཧྲིལ
write_blkio_v2_resources_throttling (ཧྲིལ fd, const var_na *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  var_na fmt_buf[128];
  size_t i;

  གལ་སྲིད (throttling == NULL)
    ལོག 0;

  དོན་ལུ (i = 0; i < throttling_len; i++)
    {
      ཧྲིལ ret;
      ཧྲིལ len;
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག crun_make_error (err, errno, "write `%s`", name);
    }
  ལོག 0;
}

static ཧྲིལ
write_blkio_resources (ཧྲིལ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  var_na fmt_buf[128];
  ཧྲིལ len;
  ཧྲིལ ret;

  གལ་སྲིད (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu32, val);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");
      གལ་སྲིད (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          གལ་སྲིད (UNLIKELY (ret < 0))
            ལོག ret;
        }
      དེ་མིན
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          གལ་སྲིད (UNLIKELY (ret < 0))
            {
              གལ་སྲིད (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu32, val);
                  གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
                    ལོག crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              གལ་སྲིད (UNLIKELY (ret < 0))
                ལོག ret;
            }
        }
    }
  གལ་སྲིད (blkio->leaf_weight)
    {
      གལ་སྲིད (cgroup2)
        ལོག crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%d", blkio->leaf_weight);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  གལ་སྲིད (blkio->weight_device_len)
    {
      གལ་སྲིད (cgroup2)
        {
          cleanup_close ཧྲིལ wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          གལ་སྲིད (UNLIKELY (wfd < 0))
            ལོག crun_make_error (err, errno, "open `io.bfq.weight`");
          དོན་ལུ (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
                ལོག crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (wfd, fmt_buf, len));
              གལ་སྲིད (UNLIKELY (ret < 0))
                ལོག crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      དེ་མིན
        {
          const var_na *leaf_weight_device_file_name = NULL;
          const var_na *weight_device_file_name = NULL;
          cleanup_close ཧྲིལ w_leafdevice_fd = -1;
          cleanup_close ཧྲིལ w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          གལ་སྲིད (UNLIKELY (w_device_fd < 0))
            ལོག w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          གལ་སྲིད (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          དོན་ལུ (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
                ལོག crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (w_device_fd, fmt_buf, len));
              གལ་སྲིད (UNLIKELY (ret < 0))
                ལོག crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              གལ་སྲིད (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
                    ལོག crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (write (w_leafdevice_fd, fmt_buf, len));
                  གལ་སྲིད (UNLIKELY (ret < 0))
                    ལོག crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  གལ་སྲིད (cgroup2)
    {
      cleanup_close ཧྲིལ wfd = -1;
      const var_na *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      གལ་སྲིད (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          ལོག check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  དེ་མིན
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  ལོག 0;
}

static ཧྲིལ
write_network_resources (ཧྲིལ dirfd_netclass, ཧྲིལ dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  var_na fmt_buf[128];
  ཧྲིལ len;
  ཧྲིལ ret;
  གལ་སྲིད (net->class_id)
    {
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu32, net->class_id);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  གལ་སྲིད (net->priorities_len)
    {
      size_t i;
      cleanup_close ཧྲིལ fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      གལ་སྲིད (UNLIKELY (fd < 0))
        ལོག crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      དོན་ལུ (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
            ལོག crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
          གལ་སྲིད (UNLIKELY (ret < 0))
            ལོག crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  ལོག 0;
}

static ཧྲིལ
write_hugetlb_resources (ཧྲིལ dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  const var_na *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  var_na fmt_buf[128];
  size_t i;
  དོན་ལུ (i = 0; i < htlb_len; i++)
    {
      cleanup_free var_na *filename = NULL;
      cleanup_free var_na *rsvd_filename = NULL;
      ཧྲིལ len;
      ཧྲིལ ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, htlb[i]->limit);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, NULL, fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;

      གལ་སྲིད (skip_rsvd)
        continue;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        {
          གལ་སྲིད (crun_error_get_errno (err) != ENOENT)
            ལོག ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  ལོག 0;
}

static ཧྲིལ
write_devices_resources_v1 (ཧྲིལ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  ཧྲིལ len;
  ཧྲིལ ret;

  དོན་ལུ (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      var_na fmt_buf[FMT_BUF_LEN];
      const var_na *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      གལ་སྲིད (devs[i]->type == NULL || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      དེ་མིན
        {
          var_na fmt_buf_major[16];
          var_na fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  do                                                                                    \
    {                                                                                   \
      གལ་སྲིད (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, parimaa_na (b), "%" PRIi64, x);                                \
          གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (b)))                                       \
            ལོག crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      དེ་མིན                                                                              \
        strcpy (b, "*");                                                                \
  } སྐབས (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          གལ་སྲིད (UNLIKELY (len >= FMT_BUF_LEN))
            ལོག crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  དོན་ལུ (i = 0; default_devices[i].type; i++)
    {
      var_na fmt_buf_major[16];
      var_na fmt_buf_minor[16];
      var_na device[64];
      ཧྲིལ len;

#define FMT_DEV(x, b)                                                                   \
  do                                                                                    \
    {                                                                                   \
      གལ་སྲིད (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, parimaa_na (b), "%d", x);                                      \
          གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (b)))                                       \
            ལོག crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      དེ་མིན                                                                              \
        strcpy (b, "*");                                                                \
  } སྐབས (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, parimaa_na (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (device)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  ལོག 0;
}

sa_nracanaa bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  ཧྲིལ i;
  sa_nracanaa bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  གལ་སྲིད (UNLIKELY (program == NULL))
    ལོག NULL;

  དོན་ལུ (i = (parimaa_na (default_devices) / parimaa_na (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      གལ་སྲིད (UNLIKELY (program == NULL))
        ལོག NULL;
    }
  དོན་ལུ (i = devs_len - 1; i >= 0; i--)
    {
      var_na type = 'a';
      ཧྲིལ minor = -1, major = -1;
      གལ་སྲིད (devs[i]->type != NULL)
        type = devs[i]->type[0];

      གལ་སྲིད (devs[i]->major_present)
        major = devs[i]->major;
      གལ་སྲིད (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      གལ་སྲིད (UNLIKELY (program == NULL))
        ལོག NULL;
    }

  program = bpf_program_complete_dev (program, err);
  གལ་སྲིད (UNLIKELY (program == NULL))
    ལོག NULL;

  ལོག program;
}

static ཧྲིལ
write_devices_resources_v2_internal (ཧྲིལ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free sa_nracanaa bpf_program *program = NULL;

  program = create_dev_bpf (devs, devs_len, err);
  གལ་སྲིད (UNLIKELY (program == NULL))
    ལོག -1;

  ལོག libcrun_ebpf_load (program, dirfd, NULL, err);
}

static ཧྲིལ
write_devices_resources_v2 (ཧྲིལ dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  ཧྲིལ ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  གལ་སྲིད (LIKELY (ret == 0))
    ལོག 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  དོན་ལུ (i = 0; i < devs_len; i++)
    {
      གལ་སྲིད (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          bha_nga;
        }
    }

  གལ་སྲིད (! can_skip)
    {
      libcrun_error_t tmp_err = NULL;
      ཧྲིལ rootless;

      rootless = is_rootless (&tmp_err);
      གལ་སྲིད (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ལོག ret;
        }
      གལ་སྲིད (rootless)
        can_skip = true;
    }

  གལ་སྲིད (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  ལོག ret;
}

static ཧྲིལ
write_devices_resources (ཧྲིལ dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  ཧྲིལ ret;

  གལ་སྲིད (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  དེ་མིན
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  གལ་སྲིད (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = NULL;
      ཧྲིལ rootless;

      rootless = is_rootless (&tmp_err);
      གལ་སྲིད (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          ལོག ret;
        }

      གལ་སྲིད (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  ལོག ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
static ཧྲིལ
cg_itoa (var_na *buf, size_t size, int64_t value, bool use_max, libcrun_error_t *err)
{
  ཧྲིལ len;

  གལ་སྲིད (use_max && value < 0)
    len = snprintf (buf, size, "max");
  དེ་མིན
    len = snprintf (buf, size, "%" PRIi64, value);

  གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) size))
    ལོག crun_make_error (err, 0, "internal error: static buffer too small");

  ལོག len;
}

static ཧྲིལ
write_memory (ཧྲིལ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  var_na limit_buf[32];
  ཧྲིལ limit_buf_len;

  གལ་སྲིད (! memory->limit_present)
    ལོག 0;

  limit_buf_len = cg_itoa (limit_buf, parimaa_na (limit_buf), memory->limit, cgroup2, err);
  གལ་སྲིད (UNLIKELY (limit_buf_len < 0))
    ལོག limit_buf_len;

  ལོག write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

static ཧྲིལ
write_memory_swap (ཧྲིལ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  ཧྲིལ ret;
  int64_t swap;
  var_na swap_buf[32];
  ཧྲིལ len;
  const var_na *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  གལ་སྲིད (! memory->swap_present)
    ལོག 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  གལ་སྲིད (cgroup2 && memory->swap > 0)
    {
      གལ་སྲིད (! memory->limit_present)
        ལོག crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      གལ་སྲིད (memory->swap < memory->limit)
        ལོག crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, parimaa_na (swap_buf), swap, cgroup2, err);
  གལ་སྲིད (UNLIKELY (len < 0))
    ལོག len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  གལ་སྲིད (ret >= 0)
    ལོག ret;

  /* If swap is not enabled, ignore the error.  */
  གལ་སྲིད (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      ལོག 0;
    }

  ལོག ret;
}

static ཧྲིལ
write_memory_resources (ཧྲིལ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  ཧྲིལ len;
  ཧྲིལ ret;
  var_na fmt_buf[32];
  bool memory_limits_written = false;

  གལ་སྲིད (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      གལ་སྲིད (ret >= 0)
        memory_limits_written = true;
      དེ་མིན
        {
          གལ་སྲིད (cgroup2 || crun_error_get_errno (err) != EINVAL)
            ལོག ret;

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
  གལ་སྲིད (UNLIKELY (ret < 0))
    ལོག ret;

  གལ་སྲིད (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  གལ་སྲིད (memory->kernel_present)
    {
      གལ་སྲིད (cgroup2)
        ལོག crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->kernel);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  གལ་སྲིད (memory->use_hierarchy_present)
    {
      གལ་སྲིད (cgroup2)
        ལོག crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  གལ་སྲིད (memory->reservation_present)
    {
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->reservation);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 NULL, fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  གལ་སྲིད (memory->disable_oom_killer)
    {
      གལ་སྲིད (cgroup2)
        ལོག crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  གལ་སྲིད (memory->kernel_tcp_present)
    {
      གལ་སྲིད (cgroup2)
        ལོག crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  གལ་སྲིད (memory->swappiness_present)
    {
      གལ་སྲིད (cgroup2)
        ལོག crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->swappiness);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  ལོག 0;
}

ཧྲིལ
write_cpu_idle (ཧྲིལ cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  var_na fmt_buf[32];
  ཧྲིལ len;

  གལ་སྲིད (! cpu->idle_present)
    ལོག 0;

  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64, cpu->idle);
  གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
    ལོག crun_make_error (err, 0, "internal error: static buffer too small");
  ལོག write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

ཧྲིལ
write_cpu_burst (ཧྲིལ cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  var_na fmt_buf[32];
  ཧྲིལ len;

  གལ་སྲིད (! cpu->burst_present)
    ལོག 0;

  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64, cpu->burst);
  གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
    ལོག crun_make_error (err, 0, "internal error: static buffer too small");
  ལོག write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

static ཧྲིལ
write_pids_resources (ཧྲིལ dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  གལ་སྲིད (pids->limit)
    {
      var_na fmt_buf[32];
      ཧྲིལ len;
      ཧྲིལ ret;

      len = cg_itoa (fmt_buf, parimaa_na (fmt_buf), pids->limit, true, err);
      གལ་སྲིད (UNLIKELY (len < 0))
        ལོག len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", NULL, fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  ལོག 0;
}

static ཧྲིལ
write_cpu_resources (ཧྲིལ dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  ཧྲིལ len, period_len = 0;
  ཧྲིལ ret;
  var_na fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free var_na *period_str = NULL;

  གལ་སྲིད (cpu->shares)
    {
      uint32_t val = cpu->shares;

      གལ་སྲིད (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          གལ་སྲིད (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              གལ་སྲིད (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%u", val);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 NULL, fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  གལ་སྲིད (cpu->period)
    {
      གལ་སྲིད (cgroup2)
        period = cpu->period;
      དེ་མིན
        {
          len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, cpu->period);
          གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
            ལོག crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          གལ་སྲིད (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              གལ་སྲིད (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                ལོག ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  གལ་སྲིད (cpu->quota)
    {
      གལ་སྲིད (cgroup2)
        quota = cpu->quota;
      དེ་མིན
        {
          len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64, cpu->quota);
          གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
            ལོག crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          གལ་སྲིད (UNLIKELY (ret < 0))
            ལོག ret;
          གལ་སྲིད (period_str != NULL)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              གལ་སྲིད (UNLIKELY (ret < 0))
                ལོག ret;
            }
        }
    }
  གལ་སྲིད (cpu->realtime_period)
    {
      གལ་སྲིད (cgroup2)
        ལོག crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, cpu->realtime_period);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  གལ་སྲིད (cpu->realtime_runtime)
    {
      གལ་སྲིད (cgroup2)
        ལོག crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  གལ་སྲིད (UNLIKELY (ret < 0))
    ལོག ret;

  གལ་སྲིད (cgroup2 && (quota > 0 || period > 0))
    {
      གལ་སྲིད (period < 0)
        period = 100000;
      གལ་སྲིད (quota < 0)
        len = snprintf (fmt_buf, parimaa_na (fmt_buf), "max %" PRIi64, period);
      དེ་མིན
        len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      གལ་སྲིད (UNLIKELY (len >= (ཧྲིལ) parimaa_na (fmt_buf)))
        ལོག crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", NULL, fmt_buf, len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  ལོག write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

ཧྲིལ
write_cpuset_resources (ཧྲིལ dirfd_cpuset, ཧྲིལ cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  ཧྲིལ ret;

  གལ་སྲིད (cpu == NULL)
    ལོག 0;

  གལ་སྲིད (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  གལ་སྲིད (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  ལོག 0;
}

static ཧྲིལ
open_cgroup_subsystem (const var_na *subsystem, const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *full_path = NULL;
  ཧྲིལ ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_ROOT, subsystem, path, NULL);
  གལ་སྲིད (UNLIKELY (ret < 0))
    ལོག ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  གལ་སྲིད (UNLIKELY (dirfd < 0))
    ལོག crun_make_error (err, errno, "open `%s`", full_path);

  ལོག dirfd;
}

ཧྲིལ
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, const var_na *path, libcrun_error_t *err)
{
  ཧྲིལ ret;

  གལ་སྲིད (resources->block_io)
    {
      cleanup_close ཧྲིལ dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      གལ་སྲིད (UNLIKELY (dirfd_blkio < 0))
        ལོག dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  གལ་སྲིད (resources->network)
    {
      cleanup_close ཧྲིལ dirfd_netclass = -1;
      cleanup_close ཧྲིལ dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      གལ་སྲིད (UNLIKELY (dirfd_netclass < 0))
        ལོག dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      གལ་སྲིད (UNLIKELY (dirfd_netprio < 0))
        ལོག dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  གལ་སྲིད (resources->hugepage_limits_len)
    {
      cleanup_close ཧྲིལ dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      གལ་སྲིད (UNLIKELY (dirfd_htlb < 0))
        ལོག dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  གལ་སྲིད (resources->devices_len)
    {
      cleanup_close ཧྲིལ dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      གལ་སྲིད (UNLIKELY (dirfd_devs < 0))
        ལོག dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  གལ་སྲིད (resources->memory)
    {
      cleanup_close ཧྲིལ dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      གལ་སྲིད (UNLIKELY (dirfd_mem < 0))
        ལོག dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  གལ་སྲིད (resources->pids)
    {
      cleanup_close ཧྲིལ dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      གལ་སྲིད (UNLIKELY (dirfd_pid < 0))
        ལོག dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  གལ་སྲིད (resources->cpu)
    {
      cleanup_close ཧྲིལ dirfd_cpu = -1;
      cleanup_close ཧྲིལ dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      གལ་སྲིད (UNLIKELY (dirfd_cpu < 0))
        ལོག dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;

      གལ་སྲིད (resources->cpu->cpus == NULL && resources->cpu->mems == NULL)
        ལོག 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      གལ་སྲིད (UNLIKELY (dirfd_cpuset < 0))
        ལོག dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  གལ་སྲིད (resources->unified && resources->unified->len > 0)
    ལོག crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  ལོག 0;
}

static ཧྲིལ
write_unified_resources (ཧྲིལ cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  ཧྲིལ ret;

  དོན་ལུ (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close ཧྲིལ fd = -1;
      cleanup_free var_na *value = NULL;
      var_na *saveptr = NULL;
      var_na *line;

      གལ་སྲིད (strchr (resources->unified->keys[i], '/'))
        ལོག crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->keys[i]);

      གལ་སྲིད (is_empty_string (resources->unified->values[i]))
        continue;

      value = xstrdup (resources->unified->values[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->keys[i], O_WRONLY, err);
      གལ་སྲིད (UNLIKELY (fd < 0))
        ལོག fd;

      དོན་ལུ (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (NULL, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (write (fd, line, strlen (line)));
          གལ་སྲིད (UNLIKELY (ret < 0))
            ལོག crun_make_error (err, errno, "write to `%s`", resources->unified->keys[i]);
        }
    }

  ལོག 0;
}

static ཧྲིལ
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, const var_na *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_close ཧྲིལ cgroup_dirfd = -1;
  ཧྲིལ ret;

  གལ་སྲིད (resources->network)
    ལོག crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  གལ་སྲིད (UNLIKELY (ret < 0))
    ལོག ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  གལ་སྲིད (UNLIKELY (cgroup_dirfd < 0))
    ལོག crun_make_error (err, errno, "open `%s`", cgroup_path);

  གལ་སྲིད (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  གལ་སྲིད (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  གལ་སྲིད (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  གལ་སྲིད (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }
  གལ་སྲིད (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  གལ་སྲིད (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  གལ་སྲིད (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      གལ་སྲིད (UNLIKELY (ret < 0))
        ལོག ret;
    }

  ལོག 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
ཧྲིལ
check_memory_before_update (const var_na *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free var_na *swap_current = NULL;
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_free var_na *current = NULL;
  cleanup_close ཧྲིལ dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  ཧྲིལ cgroup_mode;
  ཧྲིལ ret;

  གལ་སྲིད (path == NULL || memory == NULL || ! (memory->check_before_update_present && memory->check_before_update))
    ལོག 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  གལ་སྲིད (UNLIKELY (cgroup_mode < 0))
    ལོག cgroup_mode;

  གལ་སྲིད (cgroup_mode != CGROUP_MODE_UNIFIED)
    ལོག 0;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  གལ་སྲིད (UNLIKELY (ret < 0))
    ལོག ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  གལ་སྲིད (UNLIKELY (dirfd < 0))
    ལོག crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, NULL, err);
  གལ་སྲིད (UNLIKELY (ret < 0))
    ལོག ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, NULL, err);
  གལ་སྲིད (UNLIKELY (ret < 0))
    ལོག ret;

  errno = 0;
  val = strtoll (current, NULL, 10);
  གལ་སྲིད (UNLIKELY (errno))
    ལོག crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, NULL, 10);
  གལ་སྲིད (UNLIKELY (errno))
    ལོག crun_make_error (err, errno, "parse memory.swap.current");

  གལ་སྲིད (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  གལ་སྲིད (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  གལ་སྲིད (limit <= val + val_swap)
    ལོག crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  ལོག 0;
}

ཧྲིལ
update_cgroup_resources (const var_na *path,
                         const var_na *state_root,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  ཧྲིལ cgroup_mode;

  (void) state_root;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  གལ་སྲིད (UNLIKELY (cgroup_mode < 0))
    ལོག cgroup_mode;

  གལ་སྲིད (path == NULL)
    {
      size_t i;

      གལ་སྲིད (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        ལོག crun_make_error (err, 0, "cannot set limits without cgroups");

      དོན་ལུ (i = 0; i < resources->devices_len; i++)
        {
          ཧྲིལ rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          གལ་སྲིད (UNLIKELY (rwm < 0))
            ལོག rwm;

          གལ་སྲིད (rwm == 0)
            ལོག crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      ལོག 0;
    }
  switch (cgroup_mode)
    {
    pak_sa CGROUP_MODE_UNIFIED:
      ལོག update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    pak_sa CGROUP_MODE_LEGACY:
    pak_sa CGROUP_MODE_HYBRID:
      ལོག update_cgroup_v1_resources (resources, path, err);

    default:
      ལོག crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
