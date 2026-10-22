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
  戻る default_devices;
}

static 整数
write_cgroup_file_or_alias (整数 dirfd, const char *name, const char *alias, const void *data, size_t len, libcrun_error_t *err)
{
  整数 ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  もし (UNLIKELY (alias != NULL && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  戻る ret;
}

static inline 整数
openat_with_alias (整数 dirfd, const char *name, const char *alias, const char **used_name, 整数 flags, libcrun_error_t *err)
{
  整数 ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  もし (UNLIKELY (ret < 0 && alias != NULL && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "open `%s`", name);
  戻る ret;
}

static 整数
is_rwm (const char *str, libcrun_error_t *err)
{
  const char *it;
  bool r = false;
  bool w = false;
  bool m = false;

  もし (str == NULL)
    str = "";

  ために (it = str; *it; it++)
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
        戻る crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  戻る r && w && m ? 1 : 0;
}

static 整数
check_cgroup_v2_controller_available_wrapper (整数 ret, 整数 cgroup_dirfd, const char *name, libcrun_error_t *err)
{
  もし (ret == 0 || err == NULL)
    戻る 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  もし (errno == ENOENT || errno == EPERM || errno == EACCES)
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
      もし (it == NULL)
        {
          crun_error_release (err);
          戻る crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      もし (strcmp (key, "cgroup") == 0)
        戻る ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      もし (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, NULL, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          戻る ret;
        }
      ために (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
        {
          もし (strcmp (token, key) == 0)
            {
              found = true;
              break;
            }
        }
      もし (! found)
        {
          cleanup_free char *absolute_path = NULL;
          libcrun_error_t tmp_err = NULL;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          もし (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", key, absolute_path);
          否则
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", key);
            }
        }
    }
  戻る ret;
}

static 整数
write_file_and_check_controllers_at (bool cgroup2, 整数 dirfd, const char *name, const char *name_alias,
                                     const void *data, size_t len, libcrun_error_t *err)
{
  整数 ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  もし (cgroup2)
    戻る check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  戻る ret;
}

static 整数
open_file_and_check_controllers_at (bool cgroup2, 整数 dirfd, const char *name, 整数 flags, libcrun_error_t *err)
{
  整数 ret;

  ret = openat (dirfd, name, flags);
  もし (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      もし (cgroup2)
        戻る check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  戻る ret;
}

/* The parser generates different structs but they are really all the same.  */
typedef runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

static 整数
write_blkio_v1_resources_throttling (整数 dirfd, const char *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  char fmt_buf[128];
  size_t i;
  cleanup_close 整数 fd = -1;

  もし (throttling == NULL)
    戻る 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  もし (UNLIKELY (fd < 0))
    戻る crun_make_error (err, errno, "open `%s`", name);

  ために (i = 0; i < throttling_len; i++)
    {
      整数 ret;
      整数 len;
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "write `%s`", name);
    }
  戻る 0;
}

static 整数
write_blkio_v2_resources_throttling (整数 fd, const char *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  char fmt_buf[128];
  size_t i;

  もし (throttling == NULL)
    戻る 0;

  ために (i = 0; i < throttling_len; i++)
    {
      整数 ret;
      整数 len;
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      もし (UNLIKELY (ret < 0))
        戻る crun_make_error (err, errno, "write `%s`", name);
    }
  戻る 0;
}

static 整数
write_blkio_resources (整数 dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  char fmt_buf[128];
  整数 len;
  整数 ret;

  もし (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu32, val);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");
      もし (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
        }
      否则
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          もし (UNLIKELY (ret < 0))
            {
              もし (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu32, val);
                  もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
                    戻る crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              もし (UNLIKELY (ret < 0))
                戻る ret;
            }
        }
    }
  もし (blkio->leaf_weight)
    {
      もし (cgroup2)
        戻る crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%d", blkio->leaf_weight);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  もし (blkio->weight_device_len)
    {
      もし (cgroup2)
        {
          cleanup_close 整数 wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          もし (UNLIKELY (wfd < 0))
            戻る crun_make_error (err, errno, "open `io.bfq.weight`");
          ために (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
                戻る crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (wfd, fmt_buf, len));
              もし (UNLIKELY (ret < 0))
                戻る crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      否则
        {
          const char *leaf_weight_device_file_name = NULL;
          const char *weight_device_file_name = NULL;
          cleanup_close 整数 w_leafdevice_fd = -1;
          cleanup_close 整数 w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          もし (UNLIKELY (w_device_fd < 0))
            戻る w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          もし (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          ために (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
                戻る crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (w_device_fd, fmt_buf, len));
              もし (UNLIKELY (ret < 0))
                戻る crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              もし (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
                    戻る crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (write (w_leafdevice_fd, fmt_buf, len));
                  もし (UNLIKELY (ret < 0))
                    戻る crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  もし (cgroup2)
    {
      cleanup_close 整数 wfd = -1;
      const char *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      もし (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          戻る check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  否则
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  戻る 0;
}

static 整数
write_network_resources (整数 dirfd_netclass, 整数 dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  char fmt_buf[128];
  整数 len;
  整数 ret;
  もし (net->class_id)
    {
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu32, net->class_id);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  もし (net->priorities_len)
    {
      size_t i;
      cleanup_close 整数 fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      もし (UNLIKELY (fd < 0))
        戻る crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      ために (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, sizeof (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
            戻る crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  戻る 0;
}

static 整数
write_hugetlb_resources (整数 dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  const char *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  char fmt_buf[128];
  size_t i;
  ために (i = 0; i < htlb_len; i++)
    {
      cleanup_free char *filename = NULL;
      cleanup_free char *rsvd_filename = NULL;
      整数 len;
      整数 ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, htlb[i]->limit);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, NULL, fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      もし (skip_rsvd)
        continue;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        {
          もし (crun_error_get_errno (err) != ENOENT)
            戻る ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  戻る 0;
}

static 整数
write_devices_resources_v1 (整数 dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  整数 len;
  整数 ret;

  ために (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      char fmt_buf[FMT_BUF_LEN];
      const char *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      もし (devs[i]->type == NULL || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      否则
        {
          char fmt_buf_major[16];
          char fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  do                                                                                    \
    {                                                                                   \
      もし (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, sizeof (b), "%" PRIi64, x);                                \
          もし (UNLIKELY (len >= (整数) sizeof (b)))                                       \
            戻る crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      否则                                                                              \
        strcpy (b, "*");                                                                \
  } ながら (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          もし (UNLIKELY (len >= FMT_BUF_LEN))
            戻る crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  ために (i = 0; default_devices[i].type; i++)
    {
      char fmt_buf_major[16];
      char fmt_buf_minor[16];
      char device[64];
      整数 len;

#define FMT_DEV(x, b)                                                                   \
  do                                                                                    \
    {                                                                                   \
      もし (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, sizeof (b), "%d", x);                                      \
          もし (UNLIKELY (len >= (整数) sizeof (b)))                                       \
            戻る crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      否则                                                                              \
        strcpy (b, "*");                                                                \
  } ながら (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, sizeof (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      もし (UNLIKELY (len >= (整数) sizeof (device)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}

struct bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  整数 i;
  struct bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  もし (UNLIKELY (program == NULL))
    戻る NULL;

  ために (i = (sizeof (default_devices) / sizeof (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      もし (UNLIKELY (program == NULL))
        戻る NULL;
    }
  ために (i = devs_len - 1; i >= 0; i--)
    {
      char type = 'a';
      整数 minor = -1, major = -1;
      もし (devs[i]->type != NULL)
        type = devs[i]->type[0];

      もし (devs[i]->major_present)
        major = devs[i]->major;
      もし (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      もし (UNLIKELY (program == NULL))
        戻る NULL;
    }

  program = bpf_program_complete_dev (program, err);
  もし (UNLIKELY (program == NULL))
    戻る NULL;

  戻る program;
}

static 整数
write_devices_resources_v2_internal (整数 dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free struct bpf_program *program = NULL;

  program = create_dev_bpf (devs, devs_len, err);
  もし (UNLIKELY (program == NULL))
    戻る -1;

  戻る libcrun_ebpf_load (program, dirfd, NULL, err);
}

static 整数
write_devices_resources_v2 (整数 dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  整数 ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  もし (LIKELY (ret == 0))
    戻る 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  ために (i = 0; i < devs_len; i++)
    {
      もし (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          break;
        }
    }

  もし (! can_skip)
    {
      libcrun_error_t tmp_err = NULL;
      整数 rootless;

      rootless = is_rootless (&tmp_err);
      もし (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          戻る ret;
        }
      もし (rootless)
        can_skip = true;
    }

  もし (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  戻る ret;
}

static 整数
write_devices_resources (整数 dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  整数 ret;

  もし (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  否则
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  もし (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = NULL;
      整数 rootless;

      rootless = is_rootless (&tmp_err);
      もし (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          戻る ret;
        }

      もし (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  戻る ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
static 整数
cg_itoa (char *buf, size_t size, int64_t value, bool use_max, libcrun_error_t *err)
{
  整数 len;

  もし (use_max && value < 0)
    len = snprintf (buf, size, "max");
  否则
    len = snprintf (buf, size, "%" PRIi64, value);

  もし (UNLIKELY (len >= (整数) size))
    戻る crun_make_error (err, 0, "internal error: static buffer too small");

  戻る len;
}

static 整数
write_memory (整数 dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  char limit_buf[32];
  整数 limit_buf_len;

  もし (! memory->limit_present)
    戻る 0;

  limit_buf_len = cg_itoa (limit_buf, sizeof (limit_buf), memory->limit, cgroup2, err);
  もし (UNLIKELY (limit_buf_len < 0))
    戻る limit_buf_len;

  戻る write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

static 整数
write_memory_swap (整数 dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  整数 ret;
  int64_t swap;
  char swap_buf[32];
  整数 len;
  const char *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  もし (! memory->swap_present)
    戻る 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  もし (cgroup2 && memory->swap > 0)
    {
      もし (! memory->limit_present)
        戻る crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      もし (memory->swap < memory->limit)
        戻る crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, sizeof (swap_buf), swap, cgroup2, err);
  もし (UNLIKELY (len < 0))
    戻る len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  もし (ret >= 0)
    戻る ret;

  /* If swap is not enabled, ignore the error.  */
  もし (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      戻る 0;
    }

  戻る ret;
}

static 整数
write_memory_resources (整数 dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  整数 len;
  整数 ret;
  char fmt_buf[32];
  bool memory_limits_written = false;

  もし (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      もし (ret >= 0)
        memory_limits_written = true;
      否则
        {
          もし (cgroup2 || crun_error_get_errno (err) != EINVAL)
            戻る ret;

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
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (memory->kernel_present)
    {
      もし (cgroup2)
        戻る crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->kernel);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  もし (memory->use_hierarchy_present)
    {
      もし (cgroup2)
        戻る crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (memory->reservation_present)
    {
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->reservation);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 NULL, fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  もし (memory->disable_oom_killer)
    {
      もし (cgroup2)
        戻る crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  もし (memory->kernel_tcp_present)
    {
      もし (cgroup2)
        戻る crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  もし (memory->swappiness_present)
    {
      もし (cgroup2)
        戻る crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->swappiness);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}

整数
write_cpu_idle (整数 cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  char fmt_buf[32];
  整数 len;

  もし (! cpu->idle_present)
    戻る 0;

  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64, cpu->idle);
  もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
    戻る crun_make_error (err, 0, "internal error: static buffer too small");
  戻る write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

整数
write_cpu_burst (整数 cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  char fmt_buf[32];
  整数 len;

  もし (! cpu->burst_present)
    戻る 0;

  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64, cpu->burst);
  もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
    戻る crun_make_error (err, 0, "internal error: static buffer too small");
  戻る write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

static 整数
write_pids_resources (整数 dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  もし (pids->limit)
    {
      char fmt_buf[32];
      整数 len;
      整数 ret;

      len = cg_itoa (fmt_buf, sizeof (fmt_buf), pids->limit, true, err);
      もし (UNLIKELY (len < 0))
        戻る len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", NULL, fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}

static 整数
write_cpu_resources (整数 dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  整数 len, period_len = 0;
  整数 ret;
  char fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free char *period_str = NULL;

  もし (cpu->shares)
    {
      uint32_t val = cpu->shares;

      もし (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          もし (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              もし (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%u", val);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 NULL, fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  もし (cpu->period)
    {
      もし (cgroup2)
        period = cpu->period;
      否则
        {
          len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, cpu->period);
          もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
            戻る crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          もし (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              もし (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                戻る ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  もし (cpu->quota)
    {
      もし (cgroup2)
        quota = cpu->quota;
      否则
        {
          len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64, cpu->quota);
          もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
            戻る crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          もし (UNLIKELY (ret < 0))
            戻る ret;
          もし (period_str != NULL)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              もし (UNLIKELY (ret < 0))
                戻る ret;
            }
        }
    }
  もし (cpu->realtime_period)
    {
      もし (cgroup2)
        戻る crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, cpu->realtime_period);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  もし (cpu->realtime_runtime)
    {
      もし (cgroup2)
        戻る crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (cgroup2 && (quota > 0 || period > 0))
    {
      もし (period < 0)
        period = 100000;
      もし (quota < 0)
        len = snprintf (fmt_buf, sizeof (fmt_buf), "max %" PRIi64, period);
      否则
        len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      もし (UNLIKELY (len >= (整数) sizeof (fmt_buf)))
        戻る crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", NULL, fmt_buf, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

整数
write_cpuset_resources (整数 dirfd_cpuset, 整数 cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  整数 ret;

  もし (cpu == NULL)
    戻る 0;

  もし (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  もし (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  戻る 0;
}

static 整数
open_cgroup_subsystem (const char *subsystem, const char *path, libcrun_error_t *err)
{
  cleanup_free char *full_path = NULL;
  整数 ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_ROOT, subsystem, path, NULL);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  もし (UNLIKELY (dirfd < 0))
    戻る crun_make_error (err, errno, "open `%s`", full_path);

  戻る dirfd;
}

整数
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, const char *path, libcrun_error_t *err)
{
  整数 ret;

  もし (resources->block_io)
    {
      cleanup_close 整数 dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      もし (UNLIKELY (dirfd_blkio < 0))
        戻る dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (resources->network)
    {
      cleanup_close 整数 dirfd_netclass = -1;
      cleanup_close 整数 dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      もし (UNLIKELY (dirfd_netclass < 0))
        戻る dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      もし (UNLIKELY (dirfd_netprio < 0))
        戻る dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (resources->hugepage_limits_len)
    {
      cleanup_close 整数 dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      もし (UNLIKELY (dirfd_htlb < 0))
        戻る dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (resources->devices_len)
    {
      cleanup_close 整数 dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      もし (UNLIKELY (dirfd_devs < 0))
        戻る dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (resources->memory)
    {
      cleanup_close 整数 dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      もし (UNLIKELY (dirfd_mem < 0))
        戻る dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (resources->pids)
    {
      cleanup_close 整数 dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      もし (UNLIKELY (dirfd_pid < 0))
        戻る dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (resources->cpu)
    {
      cleanup_close 整数 dirfd_cpu = -1;
      cleanup_close 整数 dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      もし (UNLIKELY (dirfd_cpu < 0))
        戻る dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      もし (resources->cpu->cpus == NULL && resources->cpu->mems == NULL)
        戻る 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      もし (UNLIKELY (dirfd_cpuset < 0))
        戻る dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (resources->unified && resources->unified->len > 0)
    戻る crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  戻る 0;
}

static 整数
write_unified_resources (整数 cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  整数 ret;

  ために (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close 整数 fd = -1;
      cleanup_free char *value = NULL;
      char *saveptr = NULL;
      char *line;

      もし (strchr (resources->unified->keys[i], '/'))
        戻る crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->keys[i]);

      もし (is_empty_string (resources->unified->values[i]))
        continue;

      value = xstrdup (resources->unified->values[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->keys[i], O_WRONLY, err);
      もし (UNLIKELY (fd < 0))
        戻る fd;

      ために (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (NULL, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (write (fd, line, strlen (line)));
          もし (UNLIKELY (ret < 0))
            戻る crun_make_error (err, errno, "write to `%s`", resources->unified->keys[i]);
        }
    }

  戻る 0;
}

static 整数
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, const char *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_close 整数 cgroup_dirfd = -1;
  整数 ret;

  もし (resources->network)
    戻る crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  もし (UNLIKELY (cgroup_dirfd < 0))
    戻る crun_make_error (err, errno, "open `%s`", cgroup_path);

  もし (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  もし (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  もし (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }
  もし (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  もし (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  もし (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
整数
check_memory_before_update (const char *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free char *swap_current = NULL;
  cleanup_free char *cgroup_path = NULL;
  cleanup_free char *current = NULL;
  cleanup_close 整数 dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  整数 cgroup_mode;
  整数 ret;

  もし (path == NULL || memory == NULL || ! (memory->check_before_update_present && memory->check_before_update))
    戻る 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  もし (UNLIKELY (cgroup_mode < 0))
    戻る cgroup_mode;

  もし (cgroup_mode != CGROUP_MODE_UNIFIED)
    戻る 0;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  もし (UNLIKELY (dirfd < 0))
    戻る crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, NULL, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, NULL, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  errno = 0;
  val = strtoll (current, NULL, 10);
  もし (UNLIKELY (errno))
    戻る crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, NULL, 10);
  もし (UNLIKELY (errno))
    戻る crun_make_error (err, errno, "parse memory.swap.current");

  もし (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  もし (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  もし (limit <= val + val_swap)
    戻る crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  戻る 0;
}

整数
update_cgroup_resources (const char *path,
                         const char *state_root,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  整数 cgroup_mode;

  (void) state_root;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  もし (UNLIKELY (cgroup_mode < 0))
    戻る cgroup_mode;

  もし (path == NULL)
    {
      size_t i;

      もし (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        戻る crun_make_error (err, 0, "cannot set limits without cgroups");

      ために (i = 0; i < resources->devices_len; i++)
        {
          整数 rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          もし (UNLIKELY (rwm < 0))
            戻る rwm;

          もし (rwm == 0)
            戻る crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      戻る 0;
    }
  switch (cgroup_mode)
    {
    case CGROUP_MODE_UNIFIED:
      戻る update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    case CGROUP_MODE_LEGACY:
    case CGROUP_MODE_HYBRID:
      戻る update_cgroup_v1_resources (resources, path, err);

    default:
      戻る crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
