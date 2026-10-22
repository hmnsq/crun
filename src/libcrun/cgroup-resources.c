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

struct default_dev_s default_devices[] = {
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

struct default_dev_s *
get_default_devices ()
{
  върни default_devices;
}

static цяло
write_cgroup_file_or_alias (цяло dirfd, const char *name, const char *alias, const void *data, size_t len, libcrun_error_t *err)
{
  цяло ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  ако (UNLIKELY (alias != NULL && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  върни ret;
}

static inline цяло
openat_with_alias (цяло dirfd, const char *name, const char *alias, const char **used_name, цяло flags, libcrun_error_t *err)
{
  цяло ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  ако (UNLIKELY (ret < 0 && alias != NULL && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  ако (UNLIKELY (ret < 0))
    върни crun_make_error (err, errno, "open `%s`", name);
  върни ret;
}

static цяло
is_rwm (const char *str, libcrun_error_t *err)
{
  const char *it;
  bool r = false;
  bool w = false;
  bool m = false;

  ако (str == NULL)
    str = "";

  за (it = str; *it; it++)
    switch (*it)
      {
      case 'r':
        r = true;
        break;

      case 'w':
        w = true;
        break;

      case 'm':
        m = true;
        break;

      default:
        върни crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  върни r && w && m ? 1 : 0;
}

static цяло
check_cgroup_v2_controller_available_wrapper (цяло ret, цяло cgroup_dirfd, const char *name, libcrun_error_t *err)
{
  ако (ret == 0 || err == NULL)
    върни 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  ако (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free char *controllers = NULL;
      libcrun_error_t tmp_err = NULL;
      cleanup_free char *key = NULL;
      char *saveptr = NULL;
      bool found = false;
      const char *token;
      char *it;

      /* Check if the specified controller is enabled.  */
      key = xstrdup (name);

      it = strchr (key, '.');
      ако (it == NULL)
        {
          crun_error_release (err);
          върни crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      ако (strcmp (key, "cgroup") == 0)
        върни ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      ако (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, NULL, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          върни ret;
        }
      за (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
        {
          ако (strcmp (token, key) == 0)
            {
              found = true;
              break;
            }
        }
      ако (! found)
        {
          cleanup_free char *absolute_path = NULL;
          libcrun_error_t tmp_err = NULL;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          ако (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", key, absolute_path);
          иначе
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", key);
            }
        }
    }
  върни ret;
}

static цяло
write_file_and_check_controllers_at (bool cgroup2, цяло dirfd, const char *name, const char *name_alias,
                                     const void *data, size_t len, libcrun_error_t *err)
{
  цяло ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  ако (cgroup2)
    върни check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  върни ret;
}

static цяло
open_file_and_check_controllers_at (bool cgroup2, цяло dirfd, const char *name, цяло flags, libcrun_error_t *err)
{
  цяло ret;

  ret = openat (dirfd, name, flags);
  ако (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      ако (cgroup2)
        върни check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  върни ret;
}

/* The parser generates different structs but they are really all the same.  */
typedef runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

static цяло
write_blkio_v1_resources_throttling (цяло dirfd, const char *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  char fmt_buf[128];
  size_t i;
  cleanup_close цяло fd = -1;

  ако (throttling == NULL)
    върни 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  ако (UNLIKELY (fd < 0))
    върни crun_make_error (err, errno, "open `%s`", name);

  за (i = 0; i < throttling_len; i++)
    {
      цяло ret;
      цяло len;
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      ако (UNLIKELY (ret < 0))
        върни crun_make_error (err, errno, "write `%s`", name);
    }
  върни 0;
}

static цяло
write_blkio_v2_resources_throttling (цяло fd, const char *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  char fmt_buf[128];
  size_t i;

  ако (throttling == NULL)
    върни 0;

  за (i = 0; i < throttling_len; i++)
    {
      цяло ret;
      цяло len;
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      ако (UNLIKELY (ret < 0))
        върни crun_make_error (err, errno, "write `%s`", name);
    }
  върни 0;
}

static цяло
write_blkio_resources (цяло dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  char fmt_buf[128];
  цяло len;
  цяло ret;

  ако (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu32, val);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");
      ако (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          ако (UNLIKELY (ret < 0))
            върни ret;
        }
      иначе
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          ако (UNLIKELY (ret < 0))
            {
              ако (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu32, val);
                  ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
                    върни crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              ако (UNLIKELY (ret < 0))
                върни ret;
            }
        }
    }
  ако (blkio->leaf_weight)
    {
      ако (cgroup2)
        върни crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%d", blkio->leaf_weight);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ако (blkio->weight_device_len)
    {
      ако (cgroup2)
        {
          cleanup_close цяло wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          ако (UNLIKELY (wfd < 0))
            върни crun_make_error (err, errno, "open `io.bfq.weight`");
          за (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
                върни crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (wfd, fmt_buf, len));
              ако (UNLIKELY (ret < 0))
                върни crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      иначе
        {
          const char *leaf_weight_device_file_name = NULL;
          const char *weight_device_file_name = NULL;
          cleanup_close цяло w_leafdevice_fd = -1;
          cleanup_close цяло w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ако (UNLIKELY (w_device_fd < 0))
            върни w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ако (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          за (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
                върни crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (w_device_fd, fmt_buf, len));
              ако (UNLIKELY (ret < 0))
                върни crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              ако (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
                    върни crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (write (w_leafdevice_fd, fmt_buf, len));
                  ако (UNLIKELY (ret < 0))
                    върни crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  ако (cgroup2)
    {
      cleanup_close цяло wfd = -1;
      const char *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ако (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          върни check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  иначе
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  върни 0;
}

static цяло
write_network_resources (цяло dirfd_netclass, цяло dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  char fmt_buf[128];
  цяло len;
  цяло ret;
  ако (net->class_id)
    {
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu32, net->class_id);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ако (net->priorities_len)
    {
      size_t i;
      cleanup_close цяло fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      ако (UNLIKELY (fd < 0))
        върни crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      за (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, sizeof (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
            върни crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
          ако (UNLIKELY (ret < 0))
            върни crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  върни 0;
}

static цяло
write_hugetlb_resources (цяло dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  const char *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  char fmt_buf[128];
  size_t i;
  за (i = 0; i < htlb_len; i++)
    {
      cleanup_free char *filename = NULL;
      cleanup_free char *rsvd_filename = NULL;
      цяло len;
      цяло ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, htlb[i]->limit);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, NULL, fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;

      ако (skip_rsvd)
        continue;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        {
          ако (crun_error_get_errno (err) != ENOENT)
            върни ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  върни 0;
}

static цяло
write_devices_resources_v1 (цяло dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  цяло len;
  цяло ret;

  за (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      char fmt_buf[FMT_BUF_LEN];
      const char *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      ако (devs[i]->type == NULL || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      иначе
        {
          char fmt_buf_major[16];
          char fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  do                                                                                    \
    {                                                                                   \
      ако (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, sizeof (b), "%" PRIi64, x);                                \
          ако (UNLIKELY (len >= (цяло) sizeof (b)))                                       \
            върни crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      иначе                                                                              \
        strcpy (b, "*");                                                                \
  } докато (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          ако (UNLIKELY (len >= FMT_BUF_LEN))
            върни crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  за (i = 0; default_devices[i].type; i++)
    {
      char fmt_buf_major[16];
      char fmt_buf_minor[16];
      char device[64];
      цяло len;

#define FMT_DEV(x, b)                                                                   \
  do                                                                                    \
    {                                                                                   \
      ако (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, sizeof (b), "%d", x);                                      \
          ако (UNLIKELY (len >= (цяло) sizeof (b)))                                       \
            върни crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      иначе                                                                              \
        strcpy (b, "*");                                                                \
  } докато (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, sizeof (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      ако (UNLIKELY (len >= (цяло) sizeof (device)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  върни 0;
}

struct bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  цяло i;
  struct bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  ако (UNLIKELY (program == NULL))
    върни NULL;

  за (i = (sizeof (default_devices) / sizeof (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      ако (UNLIKELY (program == NULL))
        върни NULL;
    }
  за (i = devs_len - 1; i >= 0; i--)
    {
      char type = 'a';
      цяло minor = -1, major = -1;
      ако (devs[i]->type != NULL)
        type = devs[i]->type[0];

      ако (devs[i]->major_present)
        major = devs[i]->major;
      ако (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      ако (UNLIKELY (program == NULL))
        върни NULL;
    }

  program = bpf_program_complete_dev (program, err);
  ако (UNLIKELY (program == NULL))
    върни NULL;

  върни program;
}

static цяло
write_devices_resources_v2_internal (цяло dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free struct bpf_program *program = NULL;

  program = create_dev_bpf (devs, devs_len, err);
  ако (UNLIKELY (program == NULL))
    върни -1;

  върни libcrun_ebpf_load (program, dirfd, NULL, err);
}

static цяло
write_devices_resources_v2 (цяло dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  цяло ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  ако (LIKELY (ret == 0))
    върни 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  за (i = 0; i < devs_len; i++)
    {
      ако (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          break;
        }
    }

  ако (! can_skip)
    {
      libcrun_error_t tmp_err = NULL;
      цяло rootless;

      rootless = is_rootless (&tmp_err);
      ако (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          върни ret;
        }
      ако (rootless)
        can_skip = true;
    }

  ако (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  върни ret;
}

static цяло
write_devices_resources (цяло dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  цяло ret;

  ако (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  иначе
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  ако (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = NULL;
      цяло rootless;

      rootless = is_rootless (&tmp_err);
      ако (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          върни ret;
        }

      ако (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  върни ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
static цяло
cg_itoa (char *buf, size_t size, int64_t value, bool use_max, libcrun_error_t *err)
{
  цяло len;

  ако (use_max && value < 0)
    len = snprintf (buf, size, "max");
  иначе
    len = snprintf (buf, size, "%" PRIi64, value);

  ако (UNLIKELY (len >= (цяло) size))
    върни crun_make_error (err, 0, "internal error: static buffer too small");

  върни len;
}

static цяло
write_memory (цяло dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  char limit_buf[32];
  цяло limit_buf_len;

  ако (! memory->limit_present)
    върни 0;

  limit_buf_len = cg_itoa (limit_buf, sizeof (limit_buf), memory->limit, cgroup2, err);
  ако (UNLIKELY (limit_buf_len < 0))
    върни limit_buf_len;

  върни write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

static цяло
write_memory_swap (цяло dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  цяло ret;
  int64_t swap;
  char swap_buf[32];
  цяло len;
  const char *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  ако (! memory->swap_present)
    върни 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  ако (cgroup2 && memory->swap > 0)
    {
      ако (! memory->limit_present)
        върни crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ако (memory->swap < memory->limit)
        върни crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, sizeof (swap_buf), swap, cgroup2, err);
  ако (UNLIKELY (len < 0))
    върни len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  ако (ret >= 0)
    върни ret;

  /* If swap is not enabled, ignore the error.  */
  ако (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      върни 0;
    }

  върни ret;
}

static цяло
write_memory_resources (цяло dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  цяло len;
  цяло ret;
  char fmt_buf[32];
  bool memory_limits_written = false;

  ако (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      ако (ret >= 0)
        memory_limits_written = true;
      иначе
        {
          ако (cgroup2 || crun_error_get_errno (err) != EINVAL)
            върни ret;

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
  ако (UNLIKELY (ret < 0))
    върни ret;

  ако (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ако (memory->kernel_present)
    {
      ако (cgroup2)
        върни crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->kernel);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  ако (memory->use_hierarchy_present)
    {
      ако (cgroup2)
        върни crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ако (memory->reservation_present)
    {
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->reservation);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 NULL, fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ако (memory->disable_oom_killer)
    {
      ако (cgroup2)
        върни crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ако (memory->kernel_tcp_present)
    {
      ако (cgroup2)
        върни crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ако (memory->swappiness_present)
    {
      ако (cgroup2)
        върни crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->swappiness);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  върни 0;
}

цяло
write_cpu_idle (цяло cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  char fmt_buf[32];
  цяло len;

  ако (! cpu->idle_present)
    върни 0;

  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64, cpu->idle);
  ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
    върни crun_make_error (err, 0, "internal error: static buffer too small");
  върни write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

цяло
write_cpu_burst (цяло cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  char fmt_buf[32];
  цяло len;

  ако (! cpu->burst_present)
    върни 0;

  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64, cpu->burst);
  ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
    върни crun_make_error (err, 0, "internal error: static buffer too small");
  върни write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

static цяло
write_pids_resources (цяло dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  ако (pids->limit)
    {
      char fmt_buf[32];
      цяло len;
      цяло ret;

      len = cg_itoa (fmt_buf, sizeof (fmt_buf), pids->limit, true, err);
      ако (UNLIKELY (len < 0))
        върни len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", NULL, fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  върни 0;
}

static цяло
write_cpu_resources (цяло dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  цяло len, period_len = 0;
  цяло ret;
  char fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free char *period_str = NULL;

  ако (cpu->shares)
    {
      uint32_t val = cpu->shares;

      ако (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          ако (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              ако (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%u", val);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 NULL, fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ако (cpu->period)
    {
      ако (cgroup2)
        period = cpu->period;
      иначе
        {
          len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, cpu->period);
          ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
            върни crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          ако (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              ако (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                върни ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  ако (cpu->quota)
    {
      ако (cgroup2)
        quota = cpu->quota;
      иначе
        {
          len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64, cpu->quota);
          ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
            върни crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          ако (UNLIKELY (ret < 0))
            върни ret;
          ако (period_str != NULL)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              ако (UNLIKELY (ret < 0))
                върни ret;
            }
        }
    }
  ако (cpu->realtime_period)
    {
      ако (cgroup2)
        върни crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, cpu->realtime_period);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ако (cpu->realtime_runtime)
    {
      ако (cgroup2)
        върни crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ако (cgroup2 && (quota > 0 || period > 0))
    {
      ако (period < 0)
        period = 100000;
      ако (quota < 0)
        len = snprintf (fmt_buf, sizeof (fmt_buf), "max %" PRIi64, period);
      иначе
        len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      ако (UNLIKELY (len >= (цяло) sizeof (fmt_buf)))
        върни crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", NULL, fmt_buf, len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  върни write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

цяло
write_cpuset_resources (цяло dirfd_cpuset, цяло cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  цяло ret;

  ако (cpu == NULL)
    върни 0;

  ако (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ако (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  върни 0;
}

static цяло
open_cgroup_subsystem (const char *subsystem, const char *path, libcrun_error_t *err)
{
  cleanup_free char *full_path = NULL;
  цяло ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_ROOT, subsystem, path, NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ако (UNLIKELY (dirfd < 0))
    върни crun_make_error (err, errno, "open `%s`", full_path);

  върни dirfd;
}

цяло
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, const char *path, libcrun_error_t *err)
{
  цяло ret;

  ако (resources->block_io)
    {
      cleanup_close цяло dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      ако (UNLIKELY (dirfd_blkio < 0))
        върни dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ако (resources->network)
    {
      cleanup_close цяло dirfd_netclass = -1;
      cleanup_close цяло dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      ако (UNLIKELY (dirfd_netclass < 0))
        върни dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      ако (UNLIKELY (dirfd_netprio < 0))
        върни dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ако (resources->hugepage_limits_len)
    {
      cleanup_close цяло dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      ако (UNLIKELY (dirfd_htlb < 0))
        върни dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ако (resources->devices_len)
    {
      cleanup_close цяло dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      ако (UNLIKELY (dirfd_devs < 0))
        върни dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ако (resources->memory)
    {
      cleanup_close цяло dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      ако (UNLIKELY (dirfd_mem < 0))
        върни dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ако (resources->pids)
    {
      cleanup_close цяло dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      ако (UNLIKELY (dirfd_pid < 0))
        върни dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ако (resources->cpu)
    {
      cleanup_close цяло dirfd_cpu = -1;
      cleanup_close цяло dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      ако (UNLIKELY (dirfd_cpu < 0))
        върни dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      ако (UNLIKELY (ret < 0))
        върни ret;

      ако (resources->cpu->cpus == NULL && resources->cpu->mems == NULL)
        върни 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      ако (UNLIKELY (dirfd_cpuset < 0))
        върни dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ако (resources->unified && resources->unified->len > 0)
    върни crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  върни 0;
}

static цяло
write_unified_resources (цяло cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  цяло ret;

  за (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close цяло fd = -1;
      cleanup_free char *value = NULL;
      char *saveptr = NULL;
      char *line;

      ако (strchr (resources->unified->keys[i], '/'))
        върни crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->keys[i]);

      ако (is_empty_string (resources->unified->values[i]))
        continue;

      value = xstrdup (resources->unified->values[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->keys[i], O_WRONLY, err);
      ако (UNLIKELY (fd < 0))
        върни fd;

      за (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (NULL, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (write (fd, line, strlen (line)));
          ако (UNLIKELY (ret < 0))
            върни crun_make_error (err, errno, "write to `%s`", resources->unified->keys[i]);
        }
    }

  върни 0;
}

static цяло
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, const char *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_close цяло cgroup_dirfd = -1;
  цяло ret;

  ако (resources->network)
    върни crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ако (UNLIKELY (cgroup_dirfd < 0))
    върни crun_make_error (err, errno, "open `%s`", cgroup_path);

  ако (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ако (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ако (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ако (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      ако (UNLIKELY (ret < 0))
        върни ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }
  ако (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  ако (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  ако (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      ако (UNLIKELY (ret < 0))
        върни ret;
    }

  върни 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
цяло
check_memory_before_update (const char *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free char *swap_current = NULL;
  cleanup_free char *cgroup_path = NULL;
  cleanup_free char *current = NULL;
  cleanup_close цяло dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  цяло cgroup_mode;
  цяло ret;

  ако (path == NULL || memory == NULL || ! (memory->check_before_update_present && memory->check_before_update))
    върни 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  ако (cgroup_mode != CGROUP_MODE_UNIFIED)
    върни 0;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ако (UNLIKELY (dirfd < 0))
    върни crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, NULL, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, NULL, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  errno = 0;
  val = strtoll (current, NULL, 10);
  ако (UNLIKELY (errno))
    върни crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, NULL, 10);
  ако (UNLIKELY (errno))
    върни crun_make_error (err, errno, "parse memory.swap.current");

  ако (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  ако (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  ако (limit <= val + val_swap)
    върни crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  върни 0;
}

цяло
update_cgroup_resources (const char *path,
                         const char *state_root,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  цяло cgroup_mode;

  (void) state_root;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ако (UNLIKELY (cgroup_mode < 0))
    върни cgroup_mode;

  ако (path == NULL)
    {
      size_t i;

      ако (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        върни crun_make_error (err, 0, "cannot set limits without cgroups");

      за (i = 0; i < resources->devices_len; i++)
        {
          цяло rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          ако (UNLIKELY (rwm < 0))
            върни rwm;

          ако (rwm == 0)
            върни crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      върни 0;
    }
  switch (cgroup_mode)
    {
    case CGROUP_MODE_UNIFIED:
      върни update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    case CGROUP_MODE_LEGACY:
    case CGROUP_MODE_HYBRID:
      върни update_cgroup_v1_resources (resources, path, err);

    default:
      върни crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
