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
  вернуть default_devices;
}

static целое
write_cgroup_file_or_alias (целое dirfd, const char *name, const char *alias, const void *data, size_t len, libcrun_error_t *err)
{
  целое ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  если (UNLIKELY (alias != NULL && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  вернуть ret;
}

static inline целое
openat_with_alias (целое dirfd, const char *name, const char *alias, const char **used_name, целое flags, libcrun_error_t *err)
{
  целое ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  если (UNLIKELY (ret < 0 && alias != NULL && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "open `%s`", name);
  вернуть ret;
}

static целое
is_rwm (const char *str, libcrun_error_t *err)
{
  const char *it;
  bool r = false;
  bool w = false;
  bool m = false;

  если (str == NULL)
    str = "";

  для (it = str; *it; it++)
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
        вернуть crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  вернуть r && w && m ? 1 : 0;
}

static целое
check_cgroup_v2_controller_available_wrapper (целое ret, целое cgroup_dirfd, const char *name, libcrun_error_t *err)
{
  если (ret == 0 || err == NULL)
    вернуть 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  если (errno == ENOENT || errno == EPERM || errno == EACCES)
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
      если (it == NULL)
        {
          crun_error_release (err);
          вернуть crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      если (strcmp (key, "cgroup") == 0)
        вернуть ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      если (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, NULL, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          вернуть ret;
        }
      для (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (NULL, " \n", &saveptr))
        {
          если (strcmp (token, key) == 0)
            {
              found = true;
              break;
            }
        }
      если (! found)
        {
          cleanup_free char *absolute_path = NULL;
          libcrun_error_t tmp_err = NULL;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          если (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", key, absolute_path);
          иначе
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", key);
            }
        }
    }
  вернуть ret;
}

static целое
write_file_and_check_controllers_at (bool cgroup2, целое dirfd, const char *name, const char *name_alias,
                                     const void *data, size_t len, libcrun_error_t *err)
{
  целое ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  если (cgroup2)
    вернуть check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  вернуть ret;
}

static целое
open_file_and_check_controllers_at (bool cgroup2, целое dirfd, const char *name, целое flags, libcrun_error_t *err)
{
  целое ret;

  ret = openat (dirfd, name, flags);
  если (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      если (cgroup2)
        вернуть check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  вернуть ret;
}

/* The parser generates different structs but they are really all the same.  */
typedef runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

static целое
write_blkio_v1_resources_throttling (целое dirfd, const char *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  char fmt_buf[128];
  size_t i;
  cleanup_close целое fd = -1;

  если (throttling == NULL)
    вернуть 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  если (UNLIKELY (fd < 0))
    вернуть crun_make_error (err, errno, "open `%s`", name);

  для (i = 0; i < throttling_len; i++)
    {
      целое ret;
      целое len;
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      если (UNLIKELY (ret < 0))
        вернуть crun_make_error (err, errno, "write `%s`", name);
    }
  вернуть 0;
}

static целое
write_blkio_v2_resources_throttling (целое fd, const char *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  char fmt_buf[128];
  size_t i;

  если (throttling == NULL)
    вернуть 0;

  для (i = 0; i < throttling_len; i++)
    {
      целое ret;
      целое len;
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
      если (UNLIKELY (ret < 0))
        вернуть crun_make_error (err, errno, "write `%s`", name);
    }
  вернуть 0;
}

static целое
write_blkio_resources (целое dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  char fmt_buf[128];
  целое len;
  целое ret;

  если (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu32, val);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");
      если (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
        }
      иначе
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          если (UNLIKELY (ret < 0))
            {
              если (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu32, val);
                  если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
                    вернуть crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              если (UNLIKELY (ret < 0))
                вернуть ret;
            }
        }
    }
  если (blkio->leaf_weight)
    {
      если (cgroup2)
        вернуть crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%d", blkio->leaf_weight);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  если (blkio->weight_device_len)
    {
      если (cgroup2)
        {
          cleanup_close целое wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          если (UNLIKELY (wfd < 0))
            вернуть crun_make_error (err, errno, "open `io.bfq.weight`");
          для (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
                вернуть crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (wfd, fmt_buf, len));
              если (UNLIKELY (ret < 0))
                вернуть crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      иначе
        {
          const char *leaf_weight_device_file_name = NULL;
          const char *weight_device_file_name = NULL;
          cleanup_close целое w_leafdevice_fd = -1;
          cleanup_close целое w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          если (UNLIKELY (w_device_fd < 0))
            вернуть w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          если (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          для (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
                вернуть crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (write (w_device_fd, fmt_buf, len));
              если (UNLIKELY (ret < 0))
                вернуть crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              если (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
                    вернуть crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (write (w_leafdevice_fd, fmt_buf, len));
                  если (UNLIKELY (ret < 0))
                    вернуть crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  если (cgroup2)
    {
      cleanup_close целое wfd = -1;
      const char *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      если (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          вернуть check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  иначе
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  вернуть 0;
}

static целое
write_network_resources (целое dirfd_netclass, целое dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  char fmt_buf[128];
  целое len;
  целое ret;
  если (net->class_id)
    {
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu32, net->class_id);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  если (net->priorities_len)
    {
      size_t i;
      cleanup_close целое fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      если (UNLIKELY (fd < 0))
        вернуть crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      для (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, sizeof (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
            вернуть crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (write (fd, fmt_buf, len));
          если (UNLIKELY (ret < 0))
            вернуть crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  вернуть 0;
}

static целое
write_hugetlb_resources (целое dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  const char *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  char fmt_buf[128];
  size_t i;
  для (i = 0; i < htlb_len; i++)
    {
      cleanup_free char *filename = NULL;
      cleanup_free char *rsvd_filename = NULL;
      целое len;
      целое ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, htlb[i]->limit);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, NULL, fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      если (skip_rsvd)
        continue;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        {
          если (crun_error_get_errno (err) != ENOENT)
            вернуть ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  вернуть 0;
}

static целое
write_devices_resources_v1 (целое dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  целое len;
  целое ret;

  для (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      char fmt_buf[FMT_BUF_LEN];
      const char *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      если (devs[i]->type == NULL || devs[i]->type[0] == 'a')
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
      если (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, sizeof (b), "%" PRIi64, x);                                \
          если (UNLIKELY (len >= (целое) sizeof (b)))                                       \
            вернуть crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      иначе                                                                              \
        strcpy (b, "*");                                                                \
  } пока (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          если (UNLIKELY (len >= FMT_BUF_LEN))
            вернуть crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  для (i = 0; default_devices[i].type; i++)
    {
      char fmt_buf_major[16];
      char fmt_buf_minor[16];
      char device[64];
      целое len;

#define FMT_DEV(x, b)                                                                   \
  do                                                                                    \
    {                                                                                   \
      если (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, sizeof (b), "%d", x);                                      \
          если (UNLIKELY (len >= (целое) sizeof (b)))                                       \
            вернуть crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      иначе                                                                              \
        strcpy (b, "*");                                                                \
  } пока (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, sizeof (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      если (UNLIKELY (len >= (целое) sizeof (device)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  вернуть 0;
}

struct bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  целое i;
  struct bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  если (UNLIKELY (program == NULL))
    вернуть NULL;

  для (i = (sizeof (default_devices) / sizeof (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      если (UNLIKELY (program == NULL))
        вернуть NULL;
    }
  для (i = devs_len - 1; i >= 0; i--)
    {
      char type = 'a';
      целое minor = -1, major = -1;
      если (devs[i]->type != NULL)
        type = devs[i]->type[0];

      если (devs[i]->major_present)
        major = devs[i]->major;
      если (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      если (UNLIKELY (program == NULL))
        вернуть NULL;
    }

  program = bpf_program_complete_dev (program, err);
  если (UNLIKELY (program == NULL))
    вернуть NULL;

  вернуть program;
}

static целое
write_devices_resources_v2_internal (целое dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free struct bpf_program *program = NULL;

  program = create_dev_bpf (devs, devs_len, err);
  если (UNLIKELY (program == NULL))
    вернуть -1;

  вернуть libcrun_ebpf_load (program, dirfd, NULL, err);
}

static целое
write_devices_resources_v2 (целое dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  целое ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  если (LIKELY (ret == 0))
    вернуть 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  для (i = 0; i < devs_len; i++)
    {
      если (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          break;
        }
    }

  если (! can_skip)
    {
      libcrun_error_t tmp_err = NULL;
      целое rootless;

      rootless = is_rootless (&tmp_err);
      если (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          вернуть ret;
        }
      если (rootless)
        can_skip = true;
    }

  если (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  вернуть ret;
}

static целое
write_devices_resources (целое dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  целое ret;

  если (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  иначе
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  если (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = NULL;
      целое rootless;

      rootless = is_rootless (&tmp_err);
      если (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          вернуть ret;
        }

      если (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  вернуть ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
static целое
cg_itoa (char *buf, size_t size, int64_t value, bool use_max, libcrun_error_t *err)
{
  целое len;

  если (use_max && value < 0)
    len = snprintf (buf, size, "max");
  иначе
    len = snprintf (buf, size, "%" PRIi64, value);

  если (UNLIKELY (len >= (целое) size))
    вернуть crun_make_error (err, 0, "internal error: static buffer too small");

  вернуть len;
}

static целое
write_memory (целое dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  char limit_buf[32];
  целое limit_buf_len;

  если (! memory->limit_present)
    вернуть 0;

  limit_buf_len = cg_itoa (limit_buf, sizeof (limit_buf), memory->limit, cgroup2, err);
  если (UNLIKELY (limit_buf_len < 0))
    вернуть limit_buf_len;

  вернуть write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

static целое
write_memory_swap (целое dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  целое ret;
  int64_t swap;
  char swap_buf[32];
  целое len;
  const char *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  если (! memory->swap_present)
    вернуть 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  если (cgroup2 && memory->swap > 0)
    {
      если (! memory->limit_present)
        вернуть crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      если (memory->swap < memory->limit)
        вернуть crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, sizeof (swap_buf), swap, cgroup2, err);
  если (UNLIKELY (len < 0))
    вернуть len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  если (ret >= 0)
    вернуть ret;

  /* If swap is not enabled, ignore the error.  */
  если (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      вернуть 0;
    }

  вернуть ret;
}

static целое
write_memory_resources (целое dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  целое len;
  целое ret;
  char fmt_buf[32];
  bool memory_limits_written = false;

  если (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      если (ret >= 0)
        memory_limits_written = true;
      иначе
        {
          если (cgroup2 || crun_error_get_errno (err) != EINVAL)
            вернуть ret;

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
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (memory->kernel_present)
    {
      если (cgroup2)
        вернуть crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->kernel);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  если (memory->use_hierarchy_present)
    {
      если (cgroup2)
        вернуть crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (memory->reservation_present)
    {
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->reservation);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 NULL, fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  если (memory->disable_oom_killer)
    {
      если (cgroup2)
        вернуть crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  если (memory->kernel_tcp_present)
    {
      если (cgroup2)
        вернуть crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  если (memory->swappiness_present)
    {
      если (cgroup2)
        вернуть crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, memory->swappiness);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  вернуть 0;
}

целое
write_cpu_idle (целое cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  char fmt_buf[32];
  целое len;

  если (! cpu->idle_present)
    вернуть 0;

  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64, cpu->idle);
  если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
    вернуть crun_make_error (err, 0, "internal error: static buffer too small");
  вернуть write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

целое
write_cpu_burst (целое cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  char fmt_buf[32];
  целое len;

  если (! cpu->burst_present)
    вернуть 0;

  len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64, cpu->burst);
  если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
    вернуть crun_make_error (err, 0, "internal error: static buffer too small");
  вернуть write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

static целое
write_pids_resources (целое dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  если (pids->limit)
    {
      char fmt_buf[32];
      целое len;
      целое ret;

      len = cg_itoa (fmt_buf, sizeof (fmt_buf), pids->limit, true, err);
      если (UNLIKELY (len < 0))
        вернуть len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", NULL, fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  вернуть 0;
}

static целое
write_cpu_resources (целое dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  целое len, period_len = 0;
  целое ret;
  char fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free char *period_str = NULL;

  если (cpu->shares)
    {
      uint32_t val = cpu->shares;

      если (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          если (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              если (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, sizeof (fmt_buf), "%u", val);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 NULL, fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  если (cpu->period)
    {
      если (cgroup2)
        period = cpu->period;
      иначе
        {
          len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, cpu->period);
          если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
            вернуть crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          если (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              если (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                вернуть ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  если (cpu->quota)
    {
      если (cgroup2)
        quota = cpu->quota;
      иначе
        {
          len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64, cpu->quota);
          если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
            вернуть crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
          если (period_str != NULL)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              если (UNLIKELY (ret < 0))
                вернуть ret;
            }
        }
    }
  если (cpu->realtime_period)
    {
      если (cgroup2)
        вернуть crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, cpu->realtime_period);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  если (cpu->realtime_runtime)
    {
      если (cgroup2)
        вернуть crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (cgroup2 && (quota > 0 || period > 0))
    {
      если (period < 0)
        period = 100000;
      если (quota < 0)
        len = snprintf (fmt_buf, sizeof (fmt_buf), "max %" PRIi64, period);
      иначе
        len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      если (UNLIKELY (len >= (целое) sizeof (fmt_buf)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", NULL, fmt_buf, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  вернуть write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

целое
write_cpuset_resources (целое dirfd_cpuset, целое cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  целое ret;

  если (cpu == NULL)
    вернуть 0;

  если (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  если (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  вернуть 0;
}

static целое
open_cgroup_subsystem (const char *subsystem, const char *path, libcrun_error_t *err)
{
  cleanup_free char *full_path = NULL;
  целое ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_ROOT, subsystem, path, NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  если (UNLIKELY (dirfd < 0))
    вернуть crun_make_error (err, errno, "open `%s`", full_path);

  вернуть dirfd;
}

целое
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, const char *path, libcrun_error_t *err)
{
  целое ret;

  если (resources->block_io)
    {
      cleanup_close целое dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      если (UNLIKELY (dirfd_blkio < 0))
        вернуть dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (resources->network)
    {
      cleanup_close целое dirfd_netclass = -1;
      cleanup_close целое dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      если (UNLIKELY (dirfd_netclass < 0))
        вернуть dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      если (UNLIKELY (dirfd_netprio < 0))
        вернуть dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (resources->hugepage_limits_len)
    {
      cleanup_close целое dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      если (UNLIKELY (dirfd_htlb < 0))
        вернуть dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (resources->devices_len)
    {
      cleanup_close целое dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      если (UNLIKELY (dirfd_devs < 0))
        вернуть dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (resources->memory)
    {
      cleanup_close целое dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      если (UNLIKELY (dirfd_mem < 0))
        вернуть dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (resources->pids)
    {
      cleanup_close целое dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      если (UNLIKELY (dirfd_pid < 0))
        вернуть dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (resources->cpu)
    {
      cleanup_close целое dirfd_cpu = -1;
      cleanup_close целое dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      если (UNLIKELY (dirfd_cpu < 0))
        вернуть dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      если (resources->cpu->cpus == NULL && resources->cpu->mems == NULL)
        вернуть 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      если (UNLIKELY (dirfd_cpuset < 0))
        вернуть dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (resources->unified && resources->unified->len > 0)
    вернуть crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  вернуть 0;
}

static целое
write_unified_resources (целое cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  целое ret;

  для (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close целое fd = -1;
      cleanup_free char *value = NULL;
      char *saveptr = NULL;
      char *line;

      если (strchr (resources->unified->keys[i], '/'))
        вернуть crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->keys[i]);

      если (is_empty_string (resources->unified->values[i]))
        continue;

      value = xstrdup (resources->unified->values[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->keys[i], O_WRONLY, err);
      если (UNLIKELY (fd < 0))
        вернуть fd;

      для (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (NULL, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (write (fd, line, strlen (line)));
          если (UNLIKELY (ret < 0))
            вернуть crun_make_error (err, errno, "write to `%s`", resources->unified->keys[i]);
        }
    }

  вернуть 0;
}

static целое
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, const char *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_close целое cgroup_dirfd = -1;
  целое ret;

  если (resources->network)
    вернуть crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  если (UNLIKELY (cgroup_dirfd < 0))
    вернуть crun_make_error (err, errno, "open `%s`", cgroup_path);

  если (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  если (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  если (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  если (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  если (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  вернуть 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
целое
check_memory_before_update (const char *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free char *swap_current = NULL;
  cleanup_free char *cgroup_path = NULL;
  cleanup_free char *current = NULL;
  cleanup_close целое dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  целое cgroup_mode;
  целое ret;

  если (path == NULL || memory == NULL || ! (memory->check_before_update_present && memory->check_before_update))
    вернуть 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  если (cgroup_mode != CGROUP_MODE_UNIFIED)
    вернуть 0;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path, NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  если (UNLIKELY (dirfd < 0))
    вернуть crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, NULL, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, NULL, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  errno = 0;
  val = strtoll (current, NULL, 10);
  если (UNLIKELY (errno))
    вернуть crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, NULL, 10);
  если (UNLIKELY (errno))
    вернуть crun_make_error (err, errno, "parse memory.swap.current");

  если (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  если (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  если (limit <= val + val_swap)
    вернуть crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  вернуть 0;
}

целое
update_cgroup_resources (const char *path,
                         const char *state_root,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  целое cgroup_mode;

  (void) state_root;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (cgroup_mode < 0))
    вернуть cgroup_mode;

  если (path == NULL)
    {
      size_t i;

      если (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        вернуть crun_make_error (err, 0, "cannot set limits without cgroups");

      для (i = 0; i < resources->devices_len; i++)
        {
          целое rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          если (UNLIKELY (rwm < 0))
            вернуть rwm;

          если (rwm == 0)
            вернуть crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      вернуть 0;
    }
  switch (cgroup_mode)
    {
    case CGROUP_MODE_UNIFIED:
      вернуть update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    case CGROUP_MODE_LEGACY:
    case CGROUP_MODE_HYBRID:
      вернуть update_cgroup_v1_resources (resources, path, err);

    default:
      вернуть crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
