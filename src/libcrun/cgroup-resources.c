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

#परिभाषय _GNU_SOURCE

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
write_cgroup_file_or_alias (पूर्णाङ्क dirfd, ध्रुव वर्ण *name, ध्रुव वर्ण *alias, ध्रुव शून्य *दत्त, size_t len, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = write_cgroup_file (dirfd, name, दत्त, len, err);
  यदि (UNLIKELY (alias != रिक्त && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, दत्त, len, err);
    }
  प्रत्यागम ret;
}

स्थिर अन्तःस्थ पूर्णाङ्क
openat_with_alias (पूर्णाङ्क dirfd, ध्रुव वर्ण *name, ध्रुव वर्ण *alias, ध्रुव वर्ण **प्रयुक्तम्_name, पूर्णाङ्क flags, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  *प्रयुक्तम्_name = name;

  ret = openat (dirfd, name, flags);
  यदि (UNLIKELY (ret < 0 && alias != रिक्त && errno == ENOENT))
    {
      *प्रयुक्तम्_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  यदि (UNLIKELY (ret < 0))
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

  यदि (str == रिक्त)
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
check_cgroup_v2_controller_उपलब्धम्_wrapper (पूर्णाङ्क ret, पूर्णाङ्क cgroup_dirfd, ध्रुव वर्ण *name, libcrun_error_t *err)
{
  यदि (ret == 0 || err == रिक्त)
    प्रत्यागम 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  यदि (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free वर्ण *controllers = रिक्त;
      libcrun_error_t tmp_err = रिक्त;
      cleanup_free वर्ण *कुञ्चिका = रिक्त;
      वर्ण *saveptr = रिक्त;
      bool प्राप्तम् = false;
      ध्रुव वर्ण *token;
      वर्ण *it;

      /* Check if the specified controller is enabled.  */
      कुञ्चिका = xstrdup (name);

      it = strchr (कुञ्चिका, '.');
      यदि (it == रिक्त)
        {
          crun_error_release (err);
          प्रत्यागम crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      यदि (वाक्यं_तुलय (कुञ्चिका, "cgroup") == 0)
        प्रत्यागम ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      यदि (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, रिक्त, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          प्रत्यागम ret;
        }
      प्रति (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (रिक्त, " \n", &saveptr))
        {
          यदि (वाक्यं_तुलय (token, कुञ्चिका) == 0)
            {
              प्राप्तम् = true;
              भङ्ग;
            }
        }
      यदि (! प्राप्तम्)
        {
          cleanup_free वर्ण *absolute_मार्गः = रिक्त;
          libcrun_error_t tmp_err = रिक्त;

          crun_error_release (err);
          ret = get_realpath_गन्तव्यम्_file (cgroup_dirfd, "cgroup.controllers", &absolute_मार्गः, &tmp_err);
          यदि (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", कुञ्चिका, absolute_मार्गः);
          अन्यथा
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
                                     ध्रुव शून्य *दत्त, size_t len, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, दत्त, len, err);
  यदि (cgroup2)
    प्रत्यागम check_cgroup_v2_controller_उपलब्धम्_wrapper (ret, dirfd, name, err);
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
open_file_and_check_controllers_at (bool cgroup2, पूर्णाङ्क dirfd, ध्रुव वर्ण *name, पूर्णाङ्क flags, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = openat (dirfd, name, flags);
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      यदि (cgroup2)
        प्रत्यागम check_cgroup_v2_controller_उपलब्धम्_wrapper (ret, dirfd, name, err);
    }
  प्रत्यागम ret;
}

/* The parser generates different structs but they are really all the same.  */
प्रकारनिर्देश runtime_spec_schema_defs_linux_खण्डम्_io_device_throttle throttling_s;

स्थिर पूर्णाङ्क
write_blkio_v1_resources_throttling (पूर्णाङ्क dirfd, ध्रुव वर्ण *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  वर्ण fmt_buf[128];
  size_t i;
  cleanup_close पूर्णाङ्क fd = -1;

  यदि (throttling == रिक्त)
    प्रत्यागम 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", name);

  प्रति (i = 0; i < throttling_len; i++)
    {
      पूर्णाङ्क ret;
      पूर्णाङ्क len;
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->दरः);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = अस्थायी_FAILURE_RETRY (लिख (fd, fmt_buf, len));
      यदि (UNLIKELY (ret < 0))
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

  यदि (throttling == रिक्त)
    प्रत्यागम 0;

  प्रति (i = 0; i < throttling_len; i++)
    {
      पूर्णाङ्क ret;
      पूर्णाङ्क len;
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->दरः);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = अस्थायी_FAILURE_RETRY (लिख (fd, fmt_buf, len));
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "write `%s`", name);
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
write_blkio_resources (पूर्णाङ्क dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_खण्डम्_io *blkio,
                       libcrun_error_t *err)
{
  वर्ण fmt_buf[128];
  पूर्णाङ्क len;
  पूर्णाङ्क ret;

  यदि (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu32, val);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      यदि (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      अन्यथा
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          यदि (UNLIKELY (ret < 0))
            {
              यदि (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu32, val);
                  यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
                    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              यदि (UNLIKELY (ret < 0))
                प्रत्यागम ret;
            }
        }
    }
  यदि (blkio->पर्णम्_weight)
    {
      यदि (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%d", blkio->पर्णम्_weight);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  यदि (blkio->weight_device_len)
    {
      यदि (cgroup2)
        {
          cleanup_close पूर्णाङ्क wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          यदि (UNLIKELY (wfd < 0))
            प्रत्यागम crun_make_error (err, errno, "open `io.bfq.weight`");
          प्रति (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
                प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

              ret = अस्थायी_FAILURE_RETRY (लिख (wfd, fmt_buf, len));
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      अन्यथा
        {
          ध्रुव वर्ण *पर्णम्_weight_device_file_name = रिक्त;
          ध्रुव वर्ण *weight_device_file_name = रिक्त;
          cleanup_close पूर्णाङ्क w_leafdevice_fd = -1;
          cleanup_close पूर्णाङ्क w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          यदि (UNLIKELY (w_device_fd < 0))
            प्रत्यागम w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &पर्णम्_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          यदि (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          प्रति (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
                प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

              ret = अस्थायी_FAILURE_RETRY (लिख (w_device_fd, fmt_buf, len));
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              यदि (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->पर्णम्_weight);
                  यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
                    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = अस्थायी_FAILURE_RETRY (लिख (w_leafdevice_fd, fmt_buf, len));
                  यदि (UNLIKELY (ret < 0))
                    प्रत्यागम crun_make_error (err, errno, "write `%s`", पर्णम्_weight_device_file_name);
                }
            }
        }
    }
  यदि (cgroup2)
    {
      cleanup_close पूर्णाङ्क wfd = -1;
      ध्रुव वर्ण *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      यदि (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          प्रत्यागम check_cgroup_v2_controller_उपलब्धम्_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अन्यथा
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
write_जालम्_resources (पूर्णाङ्क dirfd_netclass, पूर्णाङ्क dirfd_netprio, runtime_spec_schema_config_linux_resources_जालम् *net,
                         libcrun_error_t *err)
{
  वर्ण fmt_buf[128];
  पूर्णाङ्क len;
  पूर्णाङ्क ret;
  यदि (net->class_id)
    {
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu32, net->class_id);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  यदि (net->priorities_len)
    {
      size_t i;
      cleanup_close पूर्णाङ्क fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      यदि (UNLIKELY (fd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      प्रति (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, परिमाण (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->प्राथमिकता);
          यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

          ret = अस्थायी_FAILURE_RETRY (लिख (fd, fmt_buf, len));
          यदि (UNLIKELY (ret < 0))
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

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_आकार, suffix);

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, htlb[i]->सीमा);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, रिक्त, fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      यदि (skip_rsvd)
        अनुवर्त;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_आकार, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (crun_error_get_errno (err) != ENOENT)
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
#परिभाषय FMT_BUF_LEN 64
      वर्ण fmt_buf[FMT_BUF_LEN];
      ध्रुव वर्ण *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      यदि (devs[i]->type == रिक्त || devs[i]->type[0] == 'a')
        {
          वाक्यं_प्रतिलिख (fmt_buf, "a");
          len = 1;
        }
      अन्यथा
        {
          वर्ण fmt_buf_major[16];
          वर्ण fmt_buf_minor[16];

#परिभाषय FMT_DEV(x, b)                                                                   \
  कुरु                                                                                    \
    {                                                                                   \
      यदि (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, परिमाण (b), "%" PRIi64, x);                                \
          यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (b)))                                       \
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      अन्यथा                                                                              \
        वाक्यं_प्रतिलिख (b, "*");                                                                \
  } यावत् (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#अपरिभाषय FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          यदि (UNLIKELY (len >= FMT_BUF_LEN))
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रति (i = 0; default_devices[i].type; i++)
    {
      वर्ण fmt_buf_major[16];
      वर्ण fmt_buf_minor[16];
      वर्ण device[64];
      पूर्णाङ्क len;

#परिभाषय FMT_DEV(x, b)                                                                   \
  कुरु                                                                                    \
    {                                                                                   \
      यदि (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, परिमाण (b), "%d", x);                                      \
          यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (b)))                                       \
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      अन्यथा                                                                              \
        वाक्यं_प्रतिलिख (b, "*");                                                                \
  } यावत् (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#अपरिभाषय FMT_DEV

      len = snprintf (device, परिमाण (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (device)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, वाक्यदैर्घ्यम् (device), err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

संरचना bpf_क्रमादेशः *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  पूर्णाङ्क i;
  संरचना bpf_क्रमादेशः *क्रमादेशः;

  क्रमादेशः = bpf_क्रमादेशः_new (2048);

  क्रमादेशः = bpf_क्रमादेशः_init_dev (क्रमादेशः, err);
  यदि (UNLIKELY (क्रमादेशः == रिक्त))
    प्रत्यागम रिक्त;

  प्रति (i = (परिमाण (default_devices) / परिमाण (default_devices[0])) - 1; i >= 0; i--)
    {
      क्रमादेशः = bpf_क्रमादेशः_append_dev (क्रमादेशः, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      यदि (UNLIKELY (क्रमादेशः == रिक्त))
        प्रत्यागम रिक्त;
    }
  प्रति (i = devs_len - 1; i >= 0; i--)
    {
      वर्ण type = 'a';
      पूर्णाङ्क minor = -1, major = -1;
      यदि (devs[i]->type != रिक्त)
        type = devs[i]->type[0];

      यदि (devs[i]->major_present)
        major = devs[i]->major;
      यदि (devs[i]->minor_present)
        minor = devs[i]->minor;

      क्रमादेशः = bpf_क्रमादेशः_append_dev (क्रमादेशः, devs[i]->access, type, major, minor, devs[i]->allow, err);
      यदि (UNLIKELY (क्रमादेशः == रिक्त))
        प्रत्यागम रिक्त;
    }

  क्रमादेशः = bpf_क्रमादेशः_complete_dev (क्रमादेशः, err);
  यदि (UNLIKELY (क्रमादेशः == रिक्त))
    प्रत्यागम रिक्त;

  प्रत्यागम क्रमादेशः;
}

स्थिर पूर्णाङ्क
write_devices_resources_v2_आन्तरिकम् (पूर्णाङ्क dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free संरचना bpf_क्रमादेशः *क्रमादेशः = रिक्त;

  क्रमादेशः = create_dev_bpf (devs, devs_len, err);
  यदि (UNLIKELY (क्रमादेशः == रिक्त))
    प्रत्यागम -1;

  प्रत्यागम libcrun_ebpf_आरोपय (क्रमादेशः, dirfd, रिक्त, err);
}

स्थिर पूर्णाङ्क
write_devices_resources_v2 (पूर्णाङ्क dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_आन्तरिकम् (dirfd, devs, devs_len, err);
  यदि (LIKELY (ret == 0))
    प्रत्यागम 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  प्रति (i = 0; i < devs_len; i++)
    {
      यदि (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          भङ्ग;
        }
    }

  यदि (! can_skip)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क rootless;

      rootless = is_rootless (&tmp_err);
      यदि (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          प्रत्यागम ret;
        }
      यदि (rootless)
        can_skip = true;
    }

  यदि (can_skip)
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

  यदि (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  अन्यथा
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  यदि (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क rootless;

      rootless = is_rootless (&tmp_err);
      यदि (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          प्रत्यागम ret;
        }

      यदि (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  प्रत्यागम ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
स्थिर पूर्णाङ्क
cg_itoa (वर्ण *buf, size_t आकार, int64_t मानम्, bool use_max, libcrun_error_t *err)
{
  पूर्णाङ्क len;

  यदि (use_max && मानम् < 0)
    len = snprintf (buf, आकार, "max");
  अन्यथा
    len = snprintf (buf, आकार, "%" PRIi64, मानम्);

  यदि (UNLIKELY (len >= (पूर्णाङ्क) आकार))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  प्रत्यागम len;
}

स्थिर पूर्णाङ्क
write_स्मृतिः (पूर्णाङ्क dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_स्मृतिः *स्मृतिः, libcrun_error_t *err)
{
  वर्ण सीमा_buf[32];
  पूर्णाङ्क सीमा_buf_len;

  यदि (! स्मृतिः->सीमा_present)
    प्रत्यागम 0;

  सीमा_buf_len = cg_itoa (सीमा_buf, परिमाण (सीमा_buf), स्मृतिः->सीमा, cgroup2, err);
  यदि (UNLIKELY (सीमा_buf_len < 0))
    प्रत्यागम सीमा_buf_len;

  प्रत्यागम write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", सीमा_buf, सीमा_buf_len, err);
}

स्थिर पूर्णाङ्क
write_स्मृतिः_swap (पूर्णाङ्क dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_स्मृतिः *स्मृतिः,
                   libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  int64_t swap;
  वर्ण swap_buf[32];
  पूर्णाङ्क len;
  ध्रुव वर्ण *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  यदि (! स्मृतिः->swap_present)
    प्रत्यागम 0;

  swap = स्मृतिः->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  यदि (cgroup2 && स्मृतिः->swap > 0)
    {
      यदि (! स्मृतिः->सीमा_present)
        प्रत्यागम crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      यदि (स्मृतिः->swap < स्मृतिः->सीमा)
        प्रत्यागम crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= स्मृतिः->सीमा;
    }

  len = cg_itoa (swap_buf, परिमाण (swap_buf), swap, cgroup2, err);
  यदि (UNLIKELY (len < 0))
    प्रत्यागम len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  यदि (ret >= 0)
    प्रत्यागम ret;

  /* If swap is not enabled, ignore the error.  */
  यदि (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      प्रत्यागम 0;
    }

  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
write_स्मृतिः_resources (पूर्णाङ्क dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_स्मृतिः *स्मृतिः,
                        libcrun_error_t *err)
{
  पूर्णाङ्क len;
  पूर्णाङ्क ret;
  वर्ण fmt_buf[32];
  bool स्मृतिः_limits_written = false;

  यदि (स्मृतिः->सीमा_present)
    {
      ret = write_स्मृतिः (dirfd, cgroup2, स्मृतिः, err);
      यदि (ret >= 0)
        स्मृतिः_limits_written = true;
      अन्यथा
        {
          यदि (cgroup2 || crun_error_get_errno (err) != EINVAL)
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

  ret = write_स्मृतिः_swap (dirfd, cgroup2, स्मृतिः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (स्मृतिः->सीमा_present && ! स्मृतिः_limits_written)
    {
      ret = write_स्मृतिः (dirfd, cgroup2, स्मृतिः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (स्मृतिः->kernel_present)
    {
      यदि (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, स्मृतिः->kernel);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  यदि (स्मृतिः->use_hierarchy_present)
    {
      यदि (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (स्मृतिः->use_hierarchy) ? "1" : "0", 1, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (स्मृतिः->reservation_present)
    {
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, स्मृतिः->reservation);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 रिक्त, fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  यदि (स्मृतिः->disable_oom_killer)
    {
      यदि (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  यदि (स्मृतिः->kernel_tcp_present)
    {
      यदि (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, स्मृतिः->kernel_tcp);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  यदि (स्मृतिः->swappiness_present)
    {
      यदि (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, स्मृतिः->swappiness);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
write_cpu_idle (पूर्णाङ्क cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  वर्ण fmt_buf[32];
  पूर्णाङ्क len;

  यदि (! cpu->idle_present)
    प्रत्यागम 0;

  len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIi64, cpu->idle);
  यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
  प्रत्यागम write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

पूर्णाङ्क
write_cpu_burst (पूर्णाङ्क cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  वर्ण fmt_buf[32];
  पूर्णाङ्क len;

  यदि (! cpu->burst_present)
    प्रत्यागम 0;

  len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIi64, cpu->burst);
  यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
  प्रत्यागम write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

स्थिर पूर्णाङ्क
write_pids_resources (पूर्णाङ्क dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  यदि (pids->सीमा)
    {
      वर्ण fmt_buf[32];
      पूर्णाङ्क len;
      पूर्णाङ्क ret;

      len = cg_itoa (fmt_buf, परिमाण (fmt_buf), pids->सीमा, true, err);
      यदि (UNLIKELY (len < 0))
        प्रत्यागम len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", रिक्त, fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
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

  यदि (cpu->shares)
    {
      uint32_t val = cpu->shares;

      यदि (cgroup2)
        {
          val = convert_shares_गन्तव्यम्_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          यदि (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              यदि (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%u", val);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 रिक्त, fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  यदि (cpu->period)
    {
      यदि (cgroup2)
        period = cpu->period;
      अन्यथा
        {
          len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, cpu->period);
          यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          यदि (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              यदि (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                प्रत्यागम ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  यदि (cpu->quota)
    {
      यदि (cgroup2)
        quota = cpu->quota;
      अन्यथा
        {
          len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIi64, cpu->quota);
          यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
            प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          यदि (period_str != रिक्त)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम ret;
            }
        }
    }
  यदि (cpu->realtime_period)
    {
      यदि (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, cpu->realtime_period);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  यदि (cpu->realtime_runtime)
    {
      यदि (cgroup2)
        प्रत्यागम crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (cgroup2 && (quota > 0 || period > 0))
    {
      यदि (period < 0)
        period = 100000;
      यदि (quota < 0)
        len = snprintf (fmt_buf, परिमाण (fmt_buf), "max %" PRIi64, period);
      अन्यथा
        len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", रिक्त, fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

पूर्णाङ्क
write_cpuset_resources (पूर्णाङ्क dirfd_cpuset, पूर्णाङ्क cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  यदि (cpu == रिक्त)
    प्रत्यागम 0;

  यदि (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, वाक्यदैर्घ्यम् (cpu->cpus), err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  यदि (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, वाक्यदैर्घ्यम् (cpu->mems),
                                                 err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
open_cgroup_subsystem (ध्रुव वर्ण *subsystem, ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  cleanup_free वर्ण *पूर्णम्_मार्गः = रिक्त;
  पूर्णाङ्क ret, dirfd;

  ret = append_paths (&पूर्णम्_मार्गः, err, CGROUP_मूलम्, subsystem, मार्गः, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dirfd = उद्घाटय (पूर्णम्_मार्गः, O_DIRECTORY | O_मार्गः | O_CLOEXEC);
  यदि (UNLIKELY (dirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", पूर्णम्_मार्गः);

  प्रत्यागम dirfd;
}

पूर्णाङ्क
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  यदि (resources->खण्डम्_io)
    {
      cleanup_close पूर्णाङ्क dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_खण्डम्_io *blkio = resources->खण्डम्_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", मार्गः, err);
      यदि (UNLIKELY (dirfd_blkio < 0))
        प्रत्यागम dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (resources->जालम्)
    {
      cleanup_close पूर्णाङ्क dirfd_netclass = -1;
      cleanup_close पूर्णाङ्क dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_जालम् *जालम् = resources->जालम्;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", मार्गः, err);
      यदि (UNLIKELY (dirfd_netclass < 0))
        प्रत्यागम dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", मार्गः, err);
      यदि (UNLIKELY (dirfd_netprio < 0))
        प्रत्यागम dirfd_netprio;

      ret = write_जालम्_resources (dirfd_netclass, dirfd_netprio, जालम्, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (resources->hugepage_limits_len)
    {
      cleanup_close पूर्णाङ्क dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", मार्गः, err);
      यदि (UNLIKELY (dirfd_htlb < 0))
        प्रत्यागम dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (resources->devices_len)
    {
      cleanup_close पूर्णाङ्क dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", मार्गः, err);
      यदि (UNLIKELY (dirfd_devs < 0))
        प्रत्यागम dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (resources->स्मृतिः)
    {
      cleanup_close पूर्णाङ्क dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", मार्गः, err);
      यदि (UNLIKELY (dirfd_mem < 0))
        प्रत्यागम dirfd_mem;

      ret = write_स्मृतिः_resources (dirfd_mem, false, resources->स्मृतिः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (resources->pids)
    {
      cleanup_close पूर्णाङ्क dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", मार्गः, err);
      यदि (UNLIKELY (dirfd_pid < 0))
        प्रत्यागम dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (resources->cpu)
    {
      cleanup_close पूर्णाङ्क dirfd_cpu = -1;
      cleanup_close पूर्णाङ्क dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", मार्गः, err);
      यदि (UNLIKELY (dirfd_cpu < 0))
        प्रत्यागम dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      यदि (resources->cpu->cpus == रिक्त && resources->cpu->mems == रिक्त)
        प्रत्यागम 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", मार्गः, err);
      यदि (UNLIKELY (dirfd_cpuset < 0))
        प्रत्यागम dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (resources->unified && resources->unified->len > 0)
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

      यदि (strchr (resources->unified->कुञ्चिकाः[i], '/'))
        प्रत्यागम crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->कुञ्चिकाः[i]);

      यदि (is_रिक्तानि_string (resources->unified->मानानि[i]))
        अनुवर्त;

      मानम् = xstrdup (resources->unified->मानानि[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->कुञ्चिकाः[i], O_WRONLY, err);
      यदि (UNLIKELY (fd < 0))
        प्रत्यागम fd;

      प्रति (पङ्क्तिः = strtok_r (मानम्, "\n", &saveptr); पङ्क्तिः; पङ्क्तिः = strtok_r (रिक्त, "\n", &saveptr))
        {
          ret = अस्थायी_FAILURE_RETRY (लिख (fd, पङ्क्तिः, वाक्यदैर्घ्यम् (पङ्क्तिः)));
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "write to `%s`", resources->unified->कुञ्चिकाः[i]);
        }
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, ध्रुव वर्ण *मार्गः, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
  cleanup_close पूर्णाङ्क cgroup_dirfd = -1;
  पूर्णाङ्क ret;

  यदि (resources->जालम्)
    प्रत्यागम crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, मार्गः, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  cgroup_dirfd = उद्घाटय (cgroup_मार्गः, O_DIRECTORY | O_मार्गः | O_CLOEXEC);
  यदि (UNLIKELY (cgroup_dirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_मार्गः);

  यदि (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (resources->स्मृतिः)
    {
      ret = write_स्मृतिः_resources (cgroup_dirfd, true, resources->स्मृतिः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  यदि (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  यदि (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  यदि (resources->खण्डम्_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->खण्डम्_io, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  यदि (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
पूर्णाङ्क
check_स्मृतिः_before_update (ध्रुव वर्ण *मार्गः, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_स्मृतिः *स्मृतिः = resources->स्मृतिः;
  cleanup_free वर्ण *swap_current = रिक्त;
  cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
  cleanup_free वर्ण *current = रिक्त;
  cleanup_close पूर्णाङ्क dirfd = -1;
  uint64_t सीमा = 0;
  uint64_t val, val_swap;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क ret;

  यदि (मार्गः == रिक्त || स्मृतिः == रिक्त || ! (स्मृतिः->check_before_update_present && स्मृतिः->check_before_update))
    प्रत्यागम 0;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  यदि (cgroup_बहुलकम् != CGROUP_बहुलकम्_UNIFIED)
    प्रत्यागम 0;

  ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, मार्गः, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dirfd = उद्घाटय (cgroup_मार्गः, O_DIRECTORY | O_मार्गः | O_CLOEXEC);
  यदि (UNLIKELY (dirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_मार्गः);

  ret = read_all_file_at (dirfd, "memory.current", &current, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  errno = 0;
  val = strtoll (current, रिक्त, 10);
  यदि (UNLIKELY (errno))
    प्रत्यागम crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, रिक्त, 10);
  यदि (UNLIKELY (errno))
    प्रत्यागम crun_make_error (err, errno, "parse memory.swap.current");

  यदि (स्मृतिः->सीमा_present && स्मृतिः->सीमा >= 0)
    सीमा = स्मृतिः->सीमा;
  यदि (स्मृतिः->swap_present && स्मृतिः->swap >= 0)
    सीमा += स्मृतिः->swap;

  यदि (सीमा <= val + val_swap)
    प्रत्यागम crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  प्रत्यागम 0;
}

पूर्णाङ्क
update_cgroup_resources (ध्रुव वर्ण *मार्गः,
                         ध्रुव वर्ण *state_मूलम्,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  पूर्णाङ्क cgroup_बहुलकम्;

  (शून्य) state_मूलम्;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  यदि (मार्गः == रिक्त)
    {
      size_t i;

      यदि (resources->खण्डम्_io || resources->जालम् || resources->hugepage_limits_len || resources->स्मृतिः
          || resources->pids || resources->cpu)
        प्रत्यागम crun_make_error (err, 0, "cannot set limits without cgroups");

      प्रति (i = 0; i < resources->devices_len; i++)
        {
          पूर्णाङ्क rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          यदि (UNLIKELY (rwm < 0))
            प्रत्यागम rwm;

          यदि (rwm == 0)
            प्रत्यागम crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      प्रत्यागम 0;
    }
  विकल्प (cgroup_बहुलकम्)
    {
    पक्ष CGROUP_बहुलकम्_UNIFIED:
      प्रत्यागम update_cgroup_v2_resources (resources, मार्गः, need_bpf_dev, err);

    पक्ष CGROUP_बहुलकम्_LEGACY:
    पक्ष CGROUP_बहुलकम्_HYBRID:
      प्रत्यागम update_cgroup_v1_resources (resources, मार्गः, err);

    स्वभाव:
      प्रत्यागम crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_बहुलकम्);
    }
}
