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
  ᬯᬮᬶ default_devices;
}

static puur_naa_nka
write_cgroup_file_or_alias (puur_naa_nka dirfd, const var_na *name, const var_na *alias, const void *data, size_t len, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  ᬬᬾᬦ᭄ (UNLIKELY (alias != NULL && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  ᬯᬮᬶ ret;
}

static anta_hstha puur_naa_nka
openat_with_alias (puur_naa_nka dirfd, const var_na *name, const var_na *alias, const var_na **used_name, puur_naa_nka flags, libcrun_error_t *err)
{
  puur_naa_nka ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0 && alias != NULL && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "open `%s`", name);
  ᬯᬮᬶ ret;
}

static puur_naa_nka
is_rwm (const var_na *str, libcrun_error_t *err)
{
  const var_na *it;
  bool r = false;
  bool w = false;
  bool m = false;

  ᬬᬾᬦ᭄ (str == NULL)
    str = "";

  for (it = str; *it; it++)
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
        ᬯᬮᬶ crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  ᬯᬮᬶ r && w && m ? 1 : 0;
}

static puur_naa_nka
check_cgroup_v2_controller_available_wrapper (puur_naa_nka ret, puur_naa_nka cgroup_dirfd, const var_na *name, libcrun_error_t *err)
{
  ᬬᬾᬦ᭄ (ret == 0 || err == NULL)
    ᬯᬮᬶ 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  ᬬᬾᬦ᭄ (errno == ENOENT || errno == EPERM || errno == EACCES)
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
      ᬬᬾᬦ᭄ (it == NULL)
        {
          crun_error_release (err);
          ᬯᬮᬶ crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      ᬬᬾᬦ᭄ (strcmp (key, "cgroup") == 0)
        ᬯᬮᬶ ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      ᬬᬾᬦ᭄ (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, NULL, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          ᬯᬮᬶ ret;
        }
      for (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
        {
          ᬬᬾᬦ᭄ (strcmp (token, key) == 0)
            {
              found = true;
              bha_nga;
            }
        }
      ᬬᬾᬦ᭄ (! found)
        {
          cleanup_free var_na *absolute_path = NULL;
          libcrun_error_t tmp_err = NULL;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          ᬬᬾᬦ᭄ (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", key, absolute_path);
          ᬮᬶᬬ
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", key);
            }
        }
    }
  ᬯᬮᬶ ret;
}

static puur_naa_nka
write_file_and_check_controllers_at (bool cgroup2, puur_naa_nka dirfd, const var_na *name, const var_na *name_alias,
                                     const void *data, size_t len, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  ᬬᬾᬦ᭄ (cgroup2)
    ᬯᬮᬶ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  ᬯᬮᬶ ret;
}

static puur_naa_nka
open_file_and_check_controllers_at (bool cgroup2, puur_naa_nka dirfd, const var_na *name, puur_naa_nka flags, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = openat (dirfd, name, flags);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      ᬬᬾᬦ᭄ (cgroup2)
        ᬯᬮᬶ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  ᬯᬮᬶ ret;
}

/* The parser generates different structs but they are really all the same.  */
typedef runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

static puur_naa_nka
write_blkio_v1_resources_throttling (puur_naa_nka dirfd, const var_na *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  var_na fmt_buf[128];
  size_t i;
  cleanup_close puur_naa_nka fd = -1;

  ᬬᬾᬦ᭄ (throttling == NULL)
    ᬯᬮᬶ 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "open `%s`", name);

  for (i = 0; i < throttling_len; i++)
    {
      puur_naa_nka ret;
      puur_naa_nka len;
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "write `%s`", name);
    }
  ᬯᬮᬶ 0;
}

static puur_naa_nka
write_blkio_v2_resources_throttling (puur_naa_nka fd, const var_na *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  var_na fmt_buf[128];
  size_t i;

  ᬬᬾᬦ᭄ (throttling == NULL)
    ᬯᬮᬶ 0;

  for (i = 0; i < throttling_len; i++)
    {
      puur_naa_nka ret;
      puur_naa_nka len;
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "write `%s`", name);
    }
  ᬯᬮᬶ 0;
}

static puur_naa_nka
write_blkio_resources (puur_naa_nka dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  var_na fmt_buf[128];
  puur_naa_nka len;
  puur_naa_nka ret;

  ᬬᬾᬦ᭄ (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu32, val);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
      ᬬᬾᬦ᭄ (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;
        }
      ᬮᬶᬬ
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            {
              ᬬᬾᬦ᭄ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu32, val);
                  ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
                    ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
                ᬯᬮᬶ ret;
            }
        }
    }
  ᬬᬾᬦ᭄ (blkio->leaf_weight)
    {
      ᬬᬾᬦ᭄ (cgroup2)
        ᬯᬮᬶ crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%d", blkio->leaf_weight);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬬᬾᬦ᭄ (blkio->weight_device_len)
    {
      ᬬᬾᬦ᭄ (cgroup2)
        {
          cleanup_close puur_naa_nka wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          ᬬᬾᬦ᭄ (UNLIKELY (wfd < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "open `io.bfq.weight`");
          for (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
                ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (wfd, fmt_buf, len));
              ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
                ᬯᬮᬶ crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      ᬮᬶᬬ
        {
          const var_na *leaf_weight_device_file_name = NULL;
          const var_na *weight_device_file_name = NULL;
          cleanup_close puur_naa_nka w_leafdevice_fd = -1;
          cleanup_close puur_naa_nka w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ᬬᬾᬦ᭄ (UNLIKELY (w_device_fd < 0))
            ᬯᬮᬶ w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ᬬᬾᬦ᭄ (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          for (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
                ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (w_device_fd, fmt_buf, len));
              ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
                ᬯᬮᬶ crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              ᬬᬾᬦ᭄ (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
                    ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (write (w_leafdevice_fd, fmt_buf, len));
                  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
                    ᬯᬮᬶ crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  ᬬᬾᬦ᭄ (cgroup2)
    {
      cleanup_close puur_naa_nka wfd = -1;
      const var_na *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ᬬᬾᬦ᭄ (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          ᬯᬮᬶ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬮᬶᬬ
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬯᬮᬶ 0;
}

static puur_naa_nka
write_network_resources (puur_naa_nka dirfd_netclass, puur_naa_nka dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  var_na fmt_buf[128];
  puur_naa_nka len;
  puur_naa_nka ret;
  ᬬᬾᬦ᭄ (net->class_id)
    {
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu32, net->class_id);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬬᬾᬦ᭄ (net->priorities_len)
    {
      size_t i;
      cleanup_close puur_naa_nka fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
        ᬯᬮᬶ crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      for (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
            ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  ᬯᬮᬶ 0;
}

static puur_naa_nka
write_hugetlb_resources (puur_naa_nka dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  const var_na *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  var_na fmt_buf[128];
  size_t i;
  for (i = 0; i < htlb_len; i++)
    {
      cleanup_free var_na *filename = NULL;
      cleanup_free var_na *rsvd_filename = NULL;
      puur_naa_nka len;
      puur_naa_nka ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, htlb[i]->limit);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, NULL, fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;

      ᬬᬾᬦ᭄ (skip_rsvd)
        continue;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        {
          ᬬᬾᬦ᭄ (crun_error_get_errno (err) != ENOENT)
            ᬯᬮᬶ ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  ᬯᬮᬶ 0;
}

static puur_naa_nka
write_devices_resources_v1 (puur_naa_nka dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  puur_naa_nka len;
  puur_naa_nka ret;

  for (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      var_na fmt_buf[FMT_BUF_LEN];
      const var_na *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      ᬬᬾᬦ᭄ (devs[i]->type == NULL || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      ᬮᬶᬬ
        {
          var_na fmt_buf_major[16];
          var_na fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  do                                                                                    \
    {                                                                                   \
      ᬬᬾᬦ᭄ (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, parimaa_na (b), "%" PRIi64, x);                                \
          ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (b)))                                       \
            ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ᬮᬶᬬ                                                                              \
        strcpy (b, "*");                                                                \
  } ᬲᭂᬤᭂᬮ (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          ᬬᬾᬦ᭄ (UNLIKELY (len >= FMT_BUF_LEN))
            ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  for (i = 0; default_devices[i].type; i++)
    {
      var_na fmt_buf_major[16];
      var_na fmt_buf_minor[16];
      var_na device[64];
      puur_naa_nka len;

#define FMT_DEV(x, b)                                                                   \
  do                                                                                    \
    {                                                                                   \
      ᬬᬾᬦ᭄ (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, parimaa_na (b), "%d", x);                                      \
          ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (b)))                                       \
            ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ᬮᬶᬬ                                                                              \
        strcpy (b, "*");                                                                \
  } ᬲᭂᬤᭂᬮ (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, parimaa_na (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (device)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬯᬮᬶ 0;
}

sa_nracanaa bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  puur_naa_nka i;
  sa_nracanaa bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  ᬬᬾᬦ᭄ (UNLIKELY (program == NULL))
    ᬯᬮᬶ NULL;

  for (i = (parimaa_na (default_devices) / parimaa_na (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      ᬬᬾᬦ᭄ (UNLIKELY (program == NULL))
        ᬯᬮᬶ NULL;
    }
  for (i = devs_len - 1; i >= 0; i--)
    {
      var_na type = 'a';
      puur_naa_nka minor = -1, major = -1;
      ᬬᬾᬦ᭄ (devs[i]->type != NULL)
        type = devs[i]->type[0];

      ᬬᬾᬦ᭄ (devs[i]->major_present)
        major = devs[i]->major;
      ᬬᬾᬦ᭄ (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      ᬬᬾᬦ᭄ (UNLIKELY (program == NULL))
        ᬯᬮᬶ NULL;
    }

  program = bpf_program_complete_dev (program, err);
  ᬬᬾᬦ᭄ (UNLIKELY (program == NULL))
    ᬯᬮᬶ NULL;

  ᬯᬮᬶ program;
}

static puur_naa_nka
write_devices_resources_v2_internal (puur_naa_nka dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free sa_nracanaa bpf_program *program = NULL;

  program = create_dev_bpf (devs, devs_len, err);
  ᬬᬾᬦ᭄ (UNLIKELY (program == NULL))
    ᬯᬮᬶ -1;

  ᬯᬮᬶ libcrun_ebpf_load (program, dirfd, NULL, err);
}

static puur_naa_nka
write_devices_resources_v2 (puur_naa_nka dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  puur_naa_nka ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  ᬬᬾᬦ᭄ (LIKELY (ret == 0))
    ᬯᬮᬶ 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  for (i = 0; i < devs_len; i++)
    {
      ᬬᬾᬦ᭄ (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          bha_nga;
        }
    }

  ᬬᬾᬦ᭄ (! can_skip)
    {
      libcrun_error_t tmp_err = NULL;
      puur_naa_nka rootless;

      rootless = is_rootless (&tmp_err);
      ᬬᬾᬦ᭄ (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ᬯᬮᬶ ret;
        }
      ᬬᬾᬦ᭄ (rootless)
        can_skip = true;
    }

  ᬬᬾᬦ᭄ (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  ᬯᬮᬶ ret;
}

static puur_naa_nka
write_devices_resources (puur_naa_nka dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  puur_naa_nka ret;

  ᬬᬾᬦ᭄ (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  ᬮᬶᬬ
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = NULL;
      puur_naa_nka rootless;

      rootless = is_rootless (&tmp_err);
      ᬬᬾᬦ᭄ (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          ᬯᬮᬶ ret;
        }

      ᬬᬾᬦ᭄ (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  ᬯᬮᬶ ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
static puur_naa_nka
cg_itoa (var_na *buf, size_t size, int64_t value, bool use_max, libcrun_error_t *err)
{
  puur_naa_nka len;

  ᬬᬾᬦ᭄ (use_max && value < 0)
    len = snprintf (buf, size, "max");
  ᬮᬶᬬ
    len = snprintf (buf, size, "%" PRIi64, value);

  ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) size))
    ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

  ᬯᬮᬶ len;
}

static puur_naa_nka
write_memory (puur_naa_nka dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  var_na limit_buf[32];
  puur_naa_nka limit_buf_len;

  ᬬᬾᬦ᭄ (! memory->limit_present)
    ᬯᬮᬶ 0;

  limit_buf_len = cg_itoa (limit_buf, parimaa_na (limit_buf), memory->limit, cgroup2, err);
  ᬬᬾᬦ᭄ (UNLIKELY (limit_buf_len < 0))
    ᬯᬮᬶ limit_buf_len;

  ᬯᬮᬶ write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

static puur_naa_nka
write_memory_swap (puur_naa_nka dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  puur_naa_nka ret;
  int64_t swap;
  var_na swap_buf[32];
  puur_naa_nka len;
  const var_na *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  ᬬᬾᬦ᭄ (! memory->swap_present)
    ᬯᬮᬶ 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  ᬬᬾᬦ᭄ (cgroup2 && memory->swap > 0)
    {
      ᬬᬾᬦ᭄ (! memory->limit_present)
        ᬯᬮᬶ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ᬬᬾᬦ᭄ (memory->swap < memory->limit)
        ᬯᬮᬶ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, parimaa_na (swap_buf), swap, cgroup2, err);
  ᬬᬾᬦ᭄ (UNLIKELY (len < 0))
    ᬯᬮᬶ len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  ᬬᬾᬦ᭄ (ret >= 0)
    ᬯᬮᬶ ret;

  /* If swap is not enabled, ignore the error.  */
  ᬬᬾᬦ᭄ (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      ᬯᬮᬶ 0;
    }

  ᬯᬮᬶ ret;
}

static puur_naa_nka
write_memory_resources (puur_naa_nka dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  puur_naa_nka len;
  puur_naa_nka ret;
  var_na fmt_buf[32];
  bool memory_limits_written = false;

  ᬬᬾᬦ᭄ (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      ᬬᬾᬦ᭄ (ret >= 0)
        memory_limits_written = true;
      ᬮᬶᬬ
        {
          ᬬᬾᬦ᭄ (cgroup2 || crun_error_get_errno (err) != EINVAL)
            ᬯᬮᬶ ret;

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
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ᬬᬾᬦ᭄ (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (memory->kernel_present)
    {
      ᬬᬾᬦ᭄ (cgroup2)
        ᬯᬮᬶ crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->kernel);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  ᬬᬾᬦ᭄ (memory->use_hierarchy_present)
    {
      ᬬᬾᬦ᭄ (cgroup2)
        ᬯᬮᬶ crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (memory->reservation_present)
    {
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->reservation);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 NULL, fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬬᬾᬦ᭄ (memory->disable_oom_killer)
    {
      ᬬᬾᬦ᭄ (cgroup2)
        ᬯᬮᬶ crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬬᬾᬦ᭄ (memory->kernel_tcp_present)
    {
      ᬬᬾᬦ᭄ (cgroup2)
        ᬯᬮᬶ crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬬᬾᬦ᭄ (memory->swappiness_present)
    {
      ᬬᬾᬦ᭄ (cgroup2)
        ᬯᬮᬶ crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, memory->swappiness);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬯᬮᬶ 0;
}

puur_naa_nka
write_cpu_idle (puur_naa_nka cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  var_na fmt_buf[32];
  puur_naa_nka len;

  ᬬᬾᬦ᭄ (! cpu->idle_present)
    ᬯᬮᬶ 0;

  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64, cpu->idle);
  ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
    ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
  ᬯᬮᬶ write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

puur_naa_nka
write_cpu_burst (puur_naa_nka cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  var_na fmt_buf[32];
  puur_naa_nka len;

  ᬬᬾᬦ᭄ (! cpu->burst_present)
    ᬯᬮᬶ 0;

  len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64, cpu->burst);
  ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
    ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
  ᬯᬮᬶ write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

static puur_naa_nka
write_pids_resources (puur_naa_nka dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  ᬬᬾᬦ᭄ (pids->limit)
    {
      var_na fmt_buf[32];
      puur_naa_nka len;
      puur_naa_nka ret;

      len = cg_itoa (fmt_buf, parimaa_na (fmt_buf), pids->limit, true, err);
      ᬬᬾᬦ᭄ (UNLIKELY (len < 0))
        ᬯᬮᬶ len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", NULL, fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬯᬮᬶ 0;
}

static puur_naa_nka
write_cpu_resources (puur_naa_nka dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  puur_naa_nka len, period_len = 0;
  puur_naa_nka ret;
  var_na fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free var_na *period_str = NULL;

  ᬬᬾᬦ᭄ (cpu->shares)
    {
      uint32_t val = cpu->shares;

      ᬬᬾᬦ᭄ (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          ᬬᬾᬦ᭄ (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%u", val);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 NULL, fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬬᬾᬦ᭄ (cpu->period)
    {
      ᬬᬾᬦ᭄ (cgroup2)
        period = cpu->period;
      ᬮᬶᬬ
        {
          len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, cpu->period);
          ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
            ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              ᬬᬾᬦ᭄ (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                ᬯᬮᬶ ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  ᬬᬾᬦ᭄ (cpu->quota)
    {
      ᬬᬾᬦ᭄ (cgroup2)
        quota = cpu->quota;
      ᬮᬶᬬ
        {
          len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64, cpu->quota);
          ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
            ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ ret;
          ᬬᬾᬦ᭄ (period_str != NULL)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
                ᬯᬮᬶ ret;
            }
        }
    }
  ᬬᬾᬦ᭄ (cpu->realtime_period)
    {
      ᬬᬾᬦ᭄ (cgroup2)
        ᬯᬮᬶ crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, cpu->realtime_period);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬬᬾᬦ᭄ (cpu->realtime_runtime)
    {
      ᬬᬾᬦ᭄ (cgroup2)
        ᬯᬮᬶ crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ᬬᬾᬦ᭄ (cgroup2 && (quota > 0 || period > 0))
    {
      ᬬᬾᬦ᭄ (period < 0)
        period = 100000;
      ᬬᬾᬦ᭄ (quota < 0)
        len = snprintf (fmt_buf, parimaa_na (fmt_buf), "max %" PRIi64, period);
      ᬮᬶᬬ
        len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      ᬬᬾᬦ᭄ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᬯᬮᬶ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", NULL, fmt_buf, len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬯᬮᬶ write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

puur_naa_nka
write_cpuset_resources (puur_naa_nka dirfd_cpuset, puur_naa_nka cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  puur_naa_nka ret;

  ᬬᬾᬦ᭄ (cpu == NULL)
    ᬯᬮᬶ 0;

  ᬬᬾᬦ᭄ (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬬᬾᬦ᭄ (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬯᬮᬶ 0;
}

static puur_naa_nka
open_cgroup_subsystem (const var_na *subsystem, const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *full_path = NULL;
  puur_naa_nka ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_ROOT, subsystem, path, NULL);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ᬬᬾᬦ᭄ (UNLIKELY (dirfd < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "open `%s`", full_path);

  ᬯᬮᬶ dirfd;
}

puur_naa_nka
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, const var_na *path, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ᬬᬾᬦ᭄ (resources->block_io)
    {
      cleanup_close puur_naa_nka dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      ᬬᬾᬦ᭄ (UNLIKELY (dirfd_blkio < 0))
        ᬯᬮᬶ dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (resources->network)
    {
      cleanup_close puur_naa_nka dirfd_netclass = -1;
      cleanup_close puur_naa_nka dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      ᬬᬾᬦ᭄ (UNLIKELY (dirfd_netclass < 0))
        ᬯᬮᬶ dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      ᬬᬾᬦ᭄ (UNLIKELY (dirfd_netprio < 0))
        ᬯᬮᬶ dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (resources->hugepage_limits_len)
    {
      cleanup_close puur_naa_nka dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      ᬬᬾᬦ᭄ (UNLIKELY (dirfd_htlb < 0))
        ᬯᬮᬶ dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (resources->devices_len)
    {
      cleanup_close puur_naa_nka dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      ᬬᬾᬦ᭄ (UNLIKELY (dirfd_devs < 0))
        ᬯᬮᬶ dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (resources->memory)
    {
      cleanup_close puur_naa_nka dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      ᬬᬾᬦ᭄ (UNLIKELY (dirfd_mem < 0))
        ᬯᬮᬶ dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (resources->pids)
    {
      cleanup_close puur_naa_nka dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      ᬬᬾᬦ᭄ (UNLIKELY (dirfd_pid < 0))
        ᬯᬮᬶ dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (resources->cpu)
    {
      cleanup_close puur_naa_nka dirfd_cpu = -1;
      cleanup_close puur_naa_nka dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      ᬬᬾᬦ᭄ (UNLIKELY (dirfd_cpu < 0))
        ᬯᬮᬶ dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;

      ᬬᬾᬦ᭄ (resources->cpu->cpus == NULL && resources->cpu->mems == NULL)
        ᬯᬮᬶ 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      ᬬᬾᬦ᭄ (UNLIKELY (dirfd_cpuset < 0))
        ᬯᬮᬶ dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (resources->unified && resources->unified->len > 0)
    ᬯᬮᬶ crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  ᬯᬮᬶ 0;
}

static puur_naa_nka
write_unified_resources (puur_naa_nka cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  puur_naa_nka ret;

  for (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close puur_naa_nka fd = -1;
      cleanup_free var_na *value = NULL;
      var_na *saveptr = NULL;
      var_na *line;

      ᬬᬾᬦ᭄ (strchr (resources->unified->keys[i], '/'))
        ᬯᬮᬶ crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->keys[i]);

      ᬬᬾᬦ᭄ (is_empty_string (resources->unified->values[i]))
        continue;

      value = xstrdup (resources->unified->values[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->keys[i], O_WRONLY, err);
      ᬬᬾᬦ᭄ (UNLIKELY (fd < 0))
        ᬯᬮᬶ fd;

      for (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (NULL, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (write (fd, line, strlen (line)));
          ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
            ᬯᬮᬶ crun_make_error (err, errno, "write to `%s`", resources->unified->keys[i]);
        }
    }

  ᬯᬮᬶ 0;
}

static puur_naa_nka
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, const var_na *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_close puur_naa_nka cgroup_dirfd = -1;
  puur_naa_nka ret;

  ᬬᬾᬦ᭄ (resources->network)
    ᬯᬮᬶ crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ᬬᬾᬦ᭄ (UNLIKELY (cgroup_dirfd < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ᬬᬾᬦ᭄ (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬬᬾᬦ᭄ (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬬᬾᬦ᭄ (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }
  ᬬᬾᬦ᭄ (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬬᬾᬦ᭄ (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  ᬬᬾᬦ᭄ (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
        ᬯᬮᬶ ret;
    }

  ᬯᬮᬶ 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
puur_naa_nka
check_memory_before_update (const var_na *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free var_na *swap_current = NULL;
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_free var_na *current = NULL;
  cleanup_close puur_naa_nka dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  puur_naa_nka cgroup_mode;
  puur_naa_nka ret;

  ᬬᬾᬦ᭄ (path == NULL || memory == NULL || ! (memory->check_before_update_present && memory->check_before_update))
    ᬯᬮᬶ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ᬬᬾᬦ᭄ (UNLIKELY (cgroup_mode < 0))
    ᬯᬮᬶ cgroup_mode;

  ᬬᬾᬦ᭄ (cgroup_mode != CGROUP_MODE_UNIFIED)
    ᬯᬮᬶ 0;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ᬬᬾᬦ᭄ (UNLIKELY (dirfd < 0))
    ᬯᬮᬶ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, NULL, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, NULL, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  errno = 0;
  val = strtoll (current, NULL, 10);
  ᬬᬾᬦ᭄ (UNLIKELY (errno))
    ᬯᬮᬶ crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, NULL, 10);
  ᬬᬾᬦ᭄ (UNLIKELY (errno))
    ᬯᬮᬶ crun_make_error (err, errno, "parse memory.swap.current");

  ᬬᬾᬦ᭄ (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  ᬬᬾᬦ᭄ (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  ᬬᬾᬦ᭄ (limit <= val + val_swap)
    ᬯᬮᬶ crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  ᬯᬮᬶ 0;
}

puur_naa_nka
update_cgroup_resources (const var_na *path,
                         const var_na *state_root,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  puur_naa_nka cgroup_mode;

  (void) state_root;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ᬬᬾᬦ᭄ (UNLIKELY (cgroup_mode < 0))
    ᬯᬮᬶ cgroup_mode;

  ᬬᬾᬦ᭄ (path == NULL)
    {
      size_t i;

      ᬬᬾᬦ᭄ (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        ᬯᬮᬶ crun_make_error (err, 0, "cannot set limits without cgroups");

      for (i = 0; i < resources->devices_len; i++)
        {
          puur_naa_nka rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          ᬬᬾᬦ᭄ (UNLIKELY (rwm < 0))
            ᬯᬮᬶ rwm;

          ᬬᬾᬦ᭄ (rwm == 0)
            ᬯᬮᬶ crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      ᬯᬮᬶ 0;
    }
  switch (cgroup_mode)
    {
    pak_sa CGROUP_MODE_UNIFIED:
      ᬯᬮᬶ update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    pak_sa CGROUP_MODE_LEGACY:
    pak_sa CGROUP_MODE_HYBRID:
      ᬯᬮᬶ update_cgroup_v1_resources (resources, path, err);

    default:
      ᬯᬮᬶ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
