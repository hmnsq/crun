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

#अन्तर्भाव <config.h>
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव "cgroup-internal.h"
#अन्तर्भाव "cgroup-systemd.h"
#अन्तर्भाव "cgroup-utils.h"
#अन्तर्भाव "cgroup-resources.h"
#अन्तर्भाव "ebpf.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "status.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <libgen.h>

संरचना default_dev_s default_devices[] = {
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
  { 0, 0, 0, रिक्त }
};

संरचना default_dev_s *
get_default_devices ()
{
  प्रत्यागम default_devices;
}

स्थिर पूर्णाङ्क
write_cgroup_file_or_alias (पूर्णाङ्क dirfd, ध्रुव वर्ण *name, ध्रुव वर्ण *alias, ध्रुव शून्य *data, size_t len, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  जइ (UNLIKELY (alias != रिक्त && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  प्रत्यागम ret;
}

स्थिर अन्तःस्थ पूर्णाङ्क
openat_with_alias (पूर्णाङ्क dirfd, ध्रुव वर्ण *name, ध्रुव वर्ण *alias, ध्रुव वर्ण **used_name, पूर्णाङ्क flags, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  जइ (UNLIKELY (ret < 0 && alias != रिक्त && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", name);
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
is_rwm (ध्रुव वर्ण *str, libcrun_error_t *err)
{
  ध्रुव वर्ण *it;
  bool r = false;
  bool w = false;
  bool m = false;

  जइ (str == रिक्त)
    str = "";

  प्रति (it = str; *it; it++)
    विकल्प (*it)
      {
      पक्ष 'r':
        r = true;
        भङ्ग;

      पक्ष 'w':
        w = true;
        भङ्ग;

      पक्ष 'm':
        m = true;
        भङ्ग;

      स्वभाव:
        प्रत्यागम crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  प्रत्यागम r && w && m ? 1 : 0;
}

स्थिर पूर्णाङ्क
check_cgroup_v2_controller_available_wrapper (पूर्णाङ्क ret, पूर्णाङ्क cgroup_dirfd, ध्रुव वर्ण *name, libcrun_error_t *err)
{
  जइ (ret == 0 || err == रिक्त)
    प्रत्यागम 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  जइ (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free वर्ण *controllers = रिक्त;
      libcrun_error_t tmp_err = रिक्त;
      cleanup_free वर्ण *कुञ्चिका = रिक्त;
      वर्ण *saveptr = रिक्त;
      bool found = false;
      ध्रुव वर्ण *token;
      वर्ण *it;

      /* Check if the specified controller is enabled.  */
      कुञ्चिका = xstrdup (name);

      it = strchr (कुञ्चिका, '.');
      जइ (it == रिक्त)
        {
          crun_error_release (err);
          प्रत्यागम crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      जइ (strcmp (कुञ्चिका, "cgroup") == 0)
        प्रत्यागम ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      जइ (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, रिक्त, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          प्रत्यागम ret;
        }
      प्रति (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (रिक्त, " \n", &saveptr))
        {
          जइ (strcmp (token, कुञ्चिका) == 0)
            {
              found = true;
              भङ्ग;
            }
        }
      जइ (! found)
        {
          cleanup_free वर्ण *absolute_path = रिक्त;
          libcrun_error_t tmp_err = रिक्त;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          जइ (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", कुञ्चिका, absolute_path);
          अण्णहा
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", कुञ्चिका);
            }
        }
    }
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
write_file_and_check_controllers_at (bool cgroup2, पूर्णाङ्क dirfd, ध्रुव वर्ण *name, ध्रुव वर्ण *name_alias,
                                     ध्रुव शून्य *data, size_t len, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  जइ (cgroup2)
    प्रत्यागम check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
open_file_and_check_controllers_at (bool cgroup2, पूर्णाङ्क dirfd, ध्रुव वर्ण *name, पूर्णाङ्क flags, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = openat (dirfd, name, flags);
  जइ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      जइ (cgroup2)
        प्रत्यागम check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  प्रत्यागम ret;
}

/* The parser generates different structs but they are really all the same.  */
प्रकारनिर्देश runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

स्थिर पूर्णाङ्क
write_blkio_v1_resources_throttling (पूर्णाङ्क dirfd, ध्रुव वर्ण *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  वर्ण fmt_buf[128];
  size_t i;
  cleanup_close पूर्णाङ्क fd = -1;

  जइ (throttling == रिक्त)
    प्रत्यागम 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  जइ (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", name);

  प्रति (i = 0; i < throttling_len; i++)
    {
      पूर्णाङ्क ret;
      पूर्णाङ्क len;
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (लिख (fd, fmt_buf, len));
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "write `%s`", name);
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
write_blkio_v2_resources_throttling (पूर्णाङ्क fd, ध्रुव वर्ण *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  वर्ण fmt_buf[128];
  size_t i;

  जइ (throttling == रिक्त)
    प्रत्यागम 0;

  प्रति (i = 0; i < throttling_len; i++)
    {
      पूर्णाङ्क ret;
      पूर्णाङ्क len;
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (लिख (fd, fmt_buf, len));
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "write `%s`", name);
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
write_blkio_resources (पूर्णाङ्क dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  वर्ण fmt_buf[128];
  पूर्णाङ्क len;
  पूर्णाङ्क ret;

  जइ (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu32, val);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      जइ (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      अण्णहा
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          जइ (UNLIKELY (ret < 0))
            {
              जइ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu32, val);
                  जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
                    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              जइ (UNLIKELY (ret < 0))
                प्रत्यागम ret;
            }
        }
    }
  जइ (blkio->leaf_weight)
    {
      जइ (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%d", blkio->leaf_weight);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  जइ (blkio->weight_device_len)
    {
      जइ (cgroup2)
        {
          cleanup_close पूर्णाङ्क wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          जइ (UNLIKELY (wfd < 0))
            प्रत्यागम crun_make_error (err, errno, "open `io.bfq.weight`");
          प्रति (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
                प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (लिख (wfd, fmt_buf, len));
              जइ (UNLIKELY (ret < 0))
                प्रत्यागम crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      अण्णहा
        {
          ध्रुव वर्ण *leaf_weight_device_file_name = रिक्त;
          ध्रुव वर्ण *weight_device_file_name = रिक्त;
          cleanup_close पूर्णाङ्क w_leafdevice_fd = -1;
          cleanup_close पूर्णाङ्क w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          जइ (UNLIKELY (w_device_fd < 0))
            प्रत्यागम w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          जइ (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          प्रति (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
                प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (लिख (w_device_fd, fmt_buf, len));
              जइ (UNLIKELY (ret < 0))
                प्रत्यागम crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              जइ (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
                    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (लिख (w_leafdevice_fd, fmt_buf, len));
                  जइ (UNLIKELY (ret < 0))
                    प्रत्यागम crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  जइ (cgroup2)
    {
      cleanup_close पूर्णाङ्क wfd = -1;
      ध्रुव वर्ण *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      जइ (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          प्रत्यागम check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अण्णहा
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
write_network_resources (पूर्णाङ्क dirfd_netclass, पूर्णाङ्क dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  वर्ण fmt_buf[128];
  पूर्णाङ्क len;
  पूर्णाङ्क ret;
  जइ (net->class_id)
    {
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu32, net->class_id);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  जइ (net->priorities_len)
    {
      size_t i;
      cleanup_close पूर्णाङ्क fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      जइ (UNLIKELY (fd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      प्रति (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, परिमाण (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (लिख (fd, fmt_buf, len));
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
write_hugetlb_resources (पूर्णाङ्क dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  ध्रुव वर्ण *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  वर्ण fmt_buf[128];
  size_t i;
  प्रति (i = 0; i < htlb_len; i++)
    {
      cleanup_free वर्ण *filename = रिक्त;
      cleanup_free वर्ण *rsvd_filename = रिक्त;
      पूर्णाङ्क len;
      पूर्णाङ्क ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, htlb[i]->limit);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, रिक्त, fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      जइ (skip_rsvd)
        अनुवर्त;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        {
          जइ (crun_error_get_errno (err) != ENOENT)
            प्रत्यागम ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
write_devices_resources_v1 (पूर्णाङ्क dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  पूर्णाङ्क len;
  पूर्णाङ्क ret;

  प्रति (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      वर्ण fmt_buf[FMT_BUF_LEN];
      ध्रुव वर्ण *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      जइ (devs[i]->type == रिक्त || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      अण्णहा
        {
          वर्ण fmt_buf_major[16];
          वर्ण fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  करेहि                                                                                    \
    {                                                                                   \
      जइ (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, परिमाण (b), "%" PRIi64, x);                                \
          जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (b)))                                       \
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      अण्णहा                                                                              \
        strcpy (b, "*");                                                                \
  } जाव (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          जइ (UNLIKELY (len >= FMT_BUF_LEN))
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रति (i = 0; default_devices[i].type; i++)
    {
      वर्ण fmt_buf_major[16];
      वर्ण fmt_buf_minor[16];
      वर्ण device[64];
      पूर्णाङ्क len;

#define FMT_DEV(x, b)                                                                   \
  करेहि                                                                                    \
    {                                                                                   \
      जइ (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, परिमाण (b), "%d", x);                                      \
          जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (b)))                                       \
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      अण्णहा                                                                              \
        strcpy (b, "*");                                                                \
  } जाव (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, परिमाण (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (device)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

संरचना bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  पूर्णाङ्क i;
  संरचना bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  जइ (UNLIKELY (program == रिक्त))
    प्रत्यागम रिक्त;

  प्रति (i = (परिमाण (default_devices) / परिमाण (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      जइ (UNLIKELY (program == रिक्त))
        प्रत्यागम रिक्त;
    }
  प्रति (i = devs_len - 1; i >= 0; i--)
    {
      वर्ण type = 'a';
      पूर्णाङ्क minor = -1, major = -1;
      जइ (devs[i]->type != रिक्त)
        type = devs[i]->type[0];

      जइ (devs[i]->major_present)
        major = devs[i]->major;
      जइ (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      जइ (UNLIKELY (program == रिक्त))
        प्रत्यागम रिक्त;
    }

  program = bpf_program_complete_dev (program, err);
  जइ (UNLIKELY (program == रिक्त))
    प्रत्यागम रिक्त;

  प्रत्यागम program;
}

स्थिर पूर्णाङ्क
write_devices_resources_v2_internal (पूर्णाङ्क dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free संरचना bpf_program *program = रिक्त;

  program = create_dev_bpf (devs, devs_len, err);
  जइ (UNLIKELY (program == रिक्त))
    प्रत्यागम -1;

  प्रत्यागम libcrun_ebpf_load (program, dirfd, रिक्त, err);
}

स्थिर पूर्णाङ्क
write_devices_resources_v2 (पूर्णाङ्क dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  जइ (LIKELY (ret == 0))
    प्रत्यागम 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  प्रति (i = 0; i < devs_len; i++)
    {
      जइ (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          भङ्ग;
        }
    }

  जइ (! can_skip)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क rootless;

      rootless = is_rootless (&tmp_err);
      जइ (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          प्रत्यागम ret;
        }
      जइ (rootless)
        can_skip = true;
    }

  जइ (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
write_devices_resources (पूर्णाङ्क dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  जइ (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  अण्णहा
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  जइ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क rootless;

      rootless = is_rootless (&tmp_err);
      जइ (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          प्रत्यागम ret;
        }

      जइ (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  प्रत्यागम ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
स्थिर पूर्णाङ्क
cg_itoa (वर्ण *buf, size_t size, int64_t मानम्, bool use_max, libcrun_error_t *err)
{
  पूर्णाङ्क len;

  जइ (use_max && मानम् < 0)
    len = snprintf (buf, size, "max");
  अण्णहा
    len = snprintf (buf, size, "%" PRIi64, मानम्);

  जइ (UNLIKELY (len >= (पूर्णाङ्क) size))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  प्रत्यागम len;
}

स्थिर पूर्णाङ्क
write_memory (पूर्णाङ्क dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  वर्ण limit_buf[32];
  पूर्णाङ्क limit_buf_len;

  जइ (! memory->limit_present)
    प्रत्यागम 0;

  limit_buf_len = cg_itoa (limit_buf, परिमाण (limit_buf), memory->limit, cgroup2, err);
  जइ (UNLIKELY (limit_buf_len < 0))
    प्रत्यागम limit_buf_len;

  प्रत्यागम write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

स्थिर पूर्णाङ्क
write_memory_swap (पूर्णाङ्क dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  int64_t swap;
  वर्ण swap_buf[32];
  पूर्णाङ्क len;
  ध्रुव वर्ण *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  जइ (! memory->swap_present)
    प्रत्यागम 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  जइ (cgroup2 && memory->swap > 0)
    {
      जइ (! memory->limit_present)
        प्रत्यागम crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      जइ (memory->swap < memory->limit)
        प्रत्यागम crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, परिमाण (swap_buf), swap, cgroup2, err);
  जइ (UNLIKELY (len < 0))
    प्रत्यागम len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  जइ (ret >= 0)
    प्रत्यागम ret;

  /* If swap is not enabled, ignore the error.  */
  जइ (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      प्रत्यागम 0;
    }

  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
write_memory_resources (पूर्णाङ्क dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  पूर्णाङ्क len;
  पूर्णाङ्क ret;
  वर्ण fmt_buf[32];
  bool memory_limits_written = false;

  जइ (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      जइ (ret >= 0)
        memory_limits_written = true;
      अण्णहा
        {
          जइ (cgroup2 || crun_error_get_errno (err) != EINVAL)
            प्रत्यागम ret;

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
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (memory->kernel_present)
    {
      जइ (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, memory->kernel);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  जइ (memory->use_hierarchy_present)
    {
      जइ (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (memory->reservation_present)
    {
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, memory->reservation);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 रिक्त, fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  जइ (memory->disable_oom_killer)
    {
      जइ (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  जइ (memory->kernel_tcp_present)
    {
      जइ (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  जइ (memory->swappiness_present)
    {
      जइ (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, memory->swappiness);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
write_cpu_idle (पूर्णाङ्क cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  वर्ण fmt_buf[32];
  पूर्णाङ्क len;

  जइ (! cpu->idle_present)
    प्रत्यागम 0;

  len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIi64, cpu->idle);
  जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
  प्रत्यागम write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

पूर्णाङ्क
write_cpu_burst (पूर्णाङ्क cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  वर्ण fmt_buf[32];
  पूर्णाङ्क len;

  जइ (! cpu->burst_present)
    प्रत्यागम 0;

  len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIi64, cpu->burst);
  जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
  प्रत्यागम write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

स्थिर पूर्णाङ्क
write_pids_resources (पूर्णाङ्क dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  जइ (pids->limit)
    {
      वर्ण fmt_buf[32];
      पूर्णाङ्क len;
      पूर्णाङ्क ret;

      len = cg_itoa (fmt_buf, परिमाण (fmt_buf), pids->limit, true, err);
      जइ (UNLIKELY (len < 0))
        प्रत्यागम len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", रिक्त, fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
write_cpu_resources (पूर्णाङ्क dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  पूर्णाङ्क len, period_len = 0;
  पूर्णाङ्क ret;
  वर्ण fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free वर्ण *period_str = रिक्त;

  जइ (cpu->shares)
    {
      uint32_t val = cpu->shares;

      जइ (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          जइ (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              जइ (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%u", val);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 रिक्त, fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  जइ (cpu->period)
    {
      जइ (cgroup2)
        period = cpu->period;
      अण्णहा
        {
          len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, cpu->period);
          जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          जइ (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              जइ (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                प्रत्यागम ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  जइ (cpu->quota)
    {
      जइ (cgroup2)
        quota = cpu->quota;
      अण्णहा
        {
          len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIi64, cpu->quota);
          जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          जइ (period_str != रिक्त)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              जइ (UNLIKELY (ret < 0))
                प्रत्यागम ret;
            }
        }
    }
  जइ (cpu->realtime_period)
    {
      जइ (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, cpu->realtime_period);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  जइ (cpu->realtime_runtime)
    {
      जइ (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (cgroup2 && (quota > 0 || period > 0))
    {
      जइ (period < 0)
        period = 100000;
      जइ (quota < 0)
        len = snprintf (fmt_buf, परिमाण (fmt_buf), "max %" PRIi64, period);
      अण्णहा
        len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", रिक्त, fmt_buf, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

पूर्णाङ्क
write_cpuset_resources (पूर्णाङ्क dirfd_cpuset, पूर्णाङ्क cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  जइ (cpu == रिक्त)
    प्रत्यागम 0;

  जइ (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  जइ (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
open_cgroup_subsystem (ध्रुव वर्ण *subsystem, ध्रुव वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *full_path = रिक्त;
  पूर्णाङ्क ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_मूलम्, subsystem, path, रिक्त);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (dirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", full_path);

  प्रत्यागम dirfd;
}

पूर्णाङ्क
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, ध्रुव वर्ण *path, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  जइ (resources->block_io)
    {
      cleanup_close पूर्णाङ्क dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      जइ (UNLIKELY (dirfd_blkio < 0))
        प्रत्यागम dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (resources->network)
    {
      cleanup_close पूर्णाङ्क dirfd_netclass = -1;
      cleanup_close पूर्णाङ्क dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      जइ (UNLIKELY (dirfd_netclass < 0))
        प्रत्यागम dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      जइ (UNLIKELY (dirfd_netprio < 0))
        प्रत्यागम dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (resources->hugepage_limits_len)
    {
      cleanup_close पूर्णाङ्क dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      जइ (UNLIKELY (dirfd_htlb < 0))
        प्रत्यागम dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (resources->devices_len)
    {
      cleanup_close पूर्णाङ्क dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      जइ (UNLIKELY (dirfd_devs < 0))
        प्रत्यागम dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (resources->memory)
    {
      cleanup_close पूर्णाङ्क dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      जइ (UNLIKELY (dirfd_mem < 0))
        प्रत्यागम dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (resources->pids)
    {
      cleanup_close पूर्णाङ्क dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      जइ (UNLIKELY (dirfd_pid < 0))
        प्रत्यागम dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (resources->cpu)
    {
      cleanup_close पूर्णाङ्क dirfd_cpu = -1;
      cleanup_close पूर्णाङ्क dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      जइ (UNLIKELY (dirfd_cpu < 0))
        प्रत्यागम dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      जइ (resources->cpu->cpus == रिक्त && resources->cpu->mems == रिक्त)
        प्रत्यागम 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      जइ (UNLIKELY (dirfd_cpuset < 0))
        प्रत्यागम dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (resources->unified && resources->unified->len > 0)
    प्रत्यागम crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
write_unified_resources (पूर्णाङ्क cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  पूर्णाङ्क ret;

  प्रति (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close पूर्णाङ्क fd = -1;
      cleanup_free वर्ण *मानम् = रिक्त;
      वर्ण *saveptr = रिक्त;
      वर्ण *पङ्क्तिः;

      जइ (strchr (resources->unified->कुञ्चिकाः[i], '/'))
        प्रत्यागम crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->कुञ्चिकाः[i]);

      जइ (is_empty_string (resources->unified->मानानि[i]))
        अनुवर्त;

      मानम् = xstrdup (resources->unified->मानानि[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->कुञ्चिकाः[i], O_WRONLY, err);
      जइ (UNLIKELY (fd < 0))
        प्रत्यागम fd;

      प्रति (पङ्क्तिः = strtok_r (मानम्, "\n", &saveptr); पङ्क्तिः; पङ्क्तिः = strtok_r (रिक्त, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (लिख (fd, पङ्क्तिः, strlen (पङ्क्तिः)));
          जइ (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "write to `%s`", resources->unified->कुञ्चिकाः[i]);
        }
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, ध्रुव वर्ण *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path = रिक्त;
  cleanup_close पूर्णाङ्क cgroup_dirfd = -1;
  पूर्णाङ्क ret;

  जइ (resources->network)
    प्रत्यागम crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_मूलम्, path, रिक्त);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (cgroup_dirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_path);

  जइ (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  जइ (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  जइ (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  जइ (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  जइ (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  जइ (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
पूर्णाङ्क
check_memory_before_update (ध्रुव वर्ण *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free वर्ण *swap_current = रिक्त;
  cleanup_free वर्ण *cgroup_path = रिक्त;
  cleanup_free वर्ण *current = रिक्त;
  cleanup_close पूर्णाङ्क dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क ret;

  जइ (path == रिक्त || memory == रिक्त || ! (memory->check_before_update_present && memory->check_before_update))
    प्रत्यागम 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जइ (UNLIKELY (cgroup_mode < 0))
    प्रत्यागम cgroup_mode;

  जइ (cgroup_mode != CGROUP_MODE_UNIFIED)
    प्रत्यागम 0;

  ret = append_paths (&cgroup_path, err, CGROUP_मूलम्, path, रिक्त);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  जइ (UNLIKELY (dirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, रिक्त, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, रिक्त, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  errno = 0;
  val = strtoll (current, रिक्त, 10);
  जइ (UNLIKELY (errno))
    प्रत्यागम crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, रिक्त, 10);
  जइ (UNLIKELY (errno))
    प्रत्यागम crun_make_error (err, errno, "parse memory.swap.current");

  जइ (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  जइ (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  जइ (limit <= val + val_swap)
    प्रत्यागम crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  प्रत्यागम 0;
}

पूर्णाङ्क
update_cgroup_resources (ध्रुव वर्ण *path,
                         ध्रुव वर्ण *state_मूलम्,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  पूर्णाङ्क cgroup_mode;

  (शून्य) state_मूलम्;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जइ (UNLIKELY (cgroup_mode < 0))
    प्रत्यागम cgroup_mode;

  जइ (path == रिक्त)
    {
      size_t i;

      जइ (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        प्रत्यागम crun_make_error (err, 0, "cannot set limits without cgroups");

      प्रति (i = 0; i < resources->devices_len; i++)
        {
          पूर्णाङ्क rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          जइ (UNLIKELY (rwm < 0))
            प्रत्यागम rwm;

          जइ (rwm == 0)
            प्रत्यागम crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      प्रत्यागम 0;
    }
  विकल्प (cgroup_mode)
    {
    पक्ष CGROUP_MODE_UNIFIED:
      प्रत्यागम update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    पक्ष CGROUP_MODE_LEGACY:
    पक्ष CGROUP_MODE_HYBRID:
      प्रत्यागम update_cgroup_v1_resources (resources, path, err);

    स्वभाव:
      प्रत्यागम crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
