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
  परत default_devices;
}

स्थिर पूर्णांक
write_cgroup_file_or_alias (पूर्णांक dirfd, स्थिरांक अक्षर *name, स्थिरांक अक्षर *alias, स्थिरांक शून्य *data, माप_प्रकार len, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  जर (UNLIKELY (alias != रिक्त && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  परत ret;
}

स्थिर ओळीत पूर्णांक
openat_with_alias (पूर्णांक dirfd, स्थिरांक अक्षर *name, स्थिरांक अक्षर *alias, स्थिरांक अक्षर **used_name, पूर्णांक flags, libcrun_error_t *err)
{
  पूर्णांक ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  जर (UNLIKELY (ret < 0 && alias != रिक्त && त्रुटि_सं == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", name);
  परत ret;
}

स्थिर पूर्णांक
is_rwm (स्थिरांक अक्षर *str, libcrun_error_t *err)
{
  स्थिरांक अक्षर *it;
  bool r = false;
  bool w = false;
  bool m = false;

  जर (str == रिक्त)
    str = "";

  साठी (it = str; *it; it++)
    निवड (*it)
      {
      स्थिती 'r':
        r = true;
        थांबा;

      स्थिती 'w':
        w = true;
        थांबा;

      स्थिती 'm':
        m = true;
        थांबा;

      पूर्वनिर्धारित:
        परत crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  परत r && w && m ? 1 : 0;
}

स्थिर पूर्णांक
check_cgroup_v2_controller_available_wrapper (पूर्णांक ret, पूर्णांक cgroup_dirfd, स्थिरांक अक्षर *name, libcrun_error_t *err)
{
  जर (ret == 0 || err == रिक्त)
    परत 0;

  त्रुटि_सं = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  जर (त्रुटि_सं == ENOENT || त्रुटि_सं == EPERM || त्रुटि_सं == EACCES)
    {
      cleanup_free अक्षर *controllers = रिक्त;
      libcrun_error_t tmp_err = रिक्त;
      cleanup_free अक्षर *किल्ली = रिक्त;
      अक्षर *saveptr = रिक्त;
      bool found = false;
      स्थिरांक अक्षर *token;
      अक्षर *it;

      /* Check if the specified controller is enabled.  */
      किल्ली = xstrdup (name);

      it = म_अक्षर (किल्ली, '.');
      जर (it == रिक्त)
        {
          crun_error_release (err);
          परत crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      जर (म_भेद (किल्ली, "cgroup") == 0)
        परत ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      जर (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, रिक्त, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          परत ret;
        }
      साठी (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (रिक्त, " \n", &saveptr))
        {
          जर (म_भेद (token, किल्ली) == 0)
            {
              found = true;
              थांबा;
            }
        }
      जर (! found)
        {
          cleanup_free अक्षर *absolute_path = रिक्त;
          libcrun_error_t tmp_err = रिक्त;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          जर (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", किल्ली, absolute_path);
          अन्यथा
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", किल्ली);
            }
        }
    }
  परत ret;
}

स्थिर पूर्णांक
write_file_and_check_controllers_at (bool cgroup2, पूर्णांक dirfd, स्थिरांक अक्षर *name, स्थिरांक अक्षर *name_alias,
                                     स्थिरांक शून्य *data, माप_प्रकार len, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  जर (cgroup2)
    परत check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  परत ret;
}

स्थिर पूर्णांक
open_file_and_check_controllers_at (bool cgroup2, पूर्णांक dirfd, स्थिरांक अक्षर *name, पूर्णांक flags, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = openat (dirfd, name, flags);
  जर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, त्रुटि_सं, "open `%s`", name);
      जर (cgroup2)
        परत check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  परत ret;
}

/* The parser generates different structs but they are really all the same.  */
प्रकारव्याख्या runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

स्थिर पूर्णांक
write_blkio_v1_resources_throttling (पूर्णांक dirfd, स्थिरांक अक्षर *name, throttling_s **throttling, माप_प्रकार throttling_len,
                                     libcrun_error_t *err)
{
  अक्षर fmt_buf[128];
  माप_प्रकार i;
  cleanup_close पूर्णांक fd = -1;

  जर (throttling == रिक्त)
    परत 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  जर (UNLIKELY (fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", name);

  साठी (i = 0; i < throttling_len; i++)
    {
      पूर्णांक ret;
      पूर्णांक len;
      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (लिख (fd, fmt_buf, len));
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "write `%s`", name);
    }
  परत 0;
}

स्थिर पूर्णांक
write_blkio_v2_resources_throttling (पूर्णांक fd, स्थिरांक अक्षर *name, throttling_s **throttling, माप_प्रकार throttling_len,
                                     libcrun_error_t *err)
{
  अक्षर fmt_buf[128];
  माप_प्रकार i;

  जर (throttling == रिक्त)
    परत 0;

  साठी (i = 0; i < throttling_len; i++)
    {
      पूर्णांक ret;
      पूर्णांक len;
      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (लिख (fd, fmt_buf, len));
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "write `%s`", name);
    }
  परत 0;
}

स्थिर पूर्णांक
write_blkio_resources (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  अक्षर fmt_buf[128];
  पूर्णांक len;
  पूर्णांक ret;

  जर (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu32, val);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");
      जर (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }
      अन्यथा
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          जर (UNLIKELY (ret < 0))
            {
              जर (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu32, val);
                  जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
                    परत crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              जर (UNLIKELY (ret < 0))
                परत ret;
            }
        }
    }
  जर (blkio->leaf_weight)
    {
      जर (cgroup2)
        परत crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, आकार (fmt_buf), "%d", blkio->leaf_weight);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  जर (blkio->weight_device_len)
    {
      जर (cgroup2)
        {
          cleanup_close पूर्णांक wfd = -1;
          माप_प्रकार i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          जर (UNLIKELY (wfd < 0))
            परत crun_make_error (err, त्रुटि_सं, "open `io.bfq.weight`");
          साठी (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
                परत crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (लिख (wfd, fmt_buf, len));
              जर (UNLIKELY (ret < 0))
                परत crun_make_error (err, त्रुटि_सं, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      अन्यथा
        {
          स्थिरांक अक्षर *leaf_weight_device_file_name = रिक्त;
          स्थिरांक अक्षर *weight_device_file_name = रिक्त;
          cleanup_close पूर्णांक w_leafdevice_fd = -1;
          cleanup_close पूर्णांक w_device_fd = -1;
          माप_प्रकार i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          जर (UNLIKELY (w_device_fd < 0))
            परत w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          जर (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          साठी (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
                परत crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (लिख (w_device_fd, fmt_buf, len));
              जर (UNLIKELY (ret < 0))
                परत crun_make_error (err, त्रुटि_सं, "write `%s`", weight_device_file_name);

              जर (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
                    परत crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (लिख (w_leafdevice_fd, fmt_buf, len));
                  जर (UNLIKELY (ret < 0))
                    परत crun_make_error (err, त्रुटि_सं, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  जर (cgroup2)
    {
      cleanup_close पूर्णांक wfd = -1;
      स्थिरांक अक्षर *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      जर (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, त्रुटि_सं, "open `%s`", name);
          परत check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  अन्यथा
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  परत 0;
}

स्थिर पूर्णांक
write_network_resources (पूर्णांक dirfd_netclass, पूर्णांक dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  अक्षर fmt_buf[128];
  पूर्णांक len;
  पूर्णांक ret;
  जर (net->class_id)
    {
      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu32, net->class_id);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  जर (net->priorities_len)
    {
      माप_प्रकार i;
      cleanup_close पूर्णांक fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      जर (UNLIKELY (fd < 0))
        परत crun_make_error (err, त्रुटि_सं, "open `net_prio.ifpriomap`");

      साठी (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, आकार (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
            परत crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (लिख (fd, fmt_buf, len));
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "write `net_prio.ifpriomap`");
        }
    }

  परत 0;
}

स्थिर पूर्णांक
write_hugetlb_resources (पूर्णांक dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, माप_प्रकार htlb_len,
                         libcrun_error_t *err)
{
  स्थिरांक अक्षर *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  अक्षर fmt_buf[128];
  माप_प्रकार i;
  साठी (i = 0; i < htlb_len; i++)
    {
      cleanup_free अक्षर *filename = रिक्त;
      cleanup_free अक्षर *rsvd_filename = रिक्त;
      पूर्णांक len;
      पूर्णांक ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64, htlb[i]->limit);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, रिक्त, fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      जर (skip_rsvd)
        सुरूठेवा;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        {
          जर (crun_error_get_errno (err) != ENOENT)
            परत ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  परत 0;
}

स्थिर पूर्णांक
write_devices_resources_v1 (पूर्णांक dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                            libcrun_error_t *err)
{
  माप_प्रकार i;
  पूर्णांक len;
  पूर्णांक ret;

  साठी (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      अक्षर fmt_buf[FMT_BUF_LEN];
      स्थिरांक अक्षर *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      जर (devs[i]->type == रिक्त || devs[i]->type[0] == 'a')
        {
          म_नकल (fmt_buf, "a");
          len = 1;
        }
      अन्यथा
        {
          अक्षर fmt_buf_major[16];
          अक्षर fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  करा                                                                                    \
    {                                                                                   \
      जर (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, आकार (b), "%" PRIi64, x);                                \
          जर (UNLIKELY (len >= (पूर्णांक) आकार (b)))                                       \
            परत crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      अन्यथा                                                                              \
        म_नकल (b, "*");                                                                \
  } पर्यंत (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          जर (UNLIKELY (len >= FMT_BUF_LEN))
            परत crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  साठी (i = 0; default_devices[i].type; i++)
    {
      अक्षर fmt_buf_major[16];
      अक्षर fmt_buf_minor[16];
      अक्षर device[64];
      पूर्णांक len;

#define FMT_DEV(x, b)                                                                   \
  करा                                                                                    \
    {                                                                                   \
      जर (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, आकार (b), "%d", x);                                      \
          जर (UNLIKELY (len >= (पूर्णांक) आकार (b)))                                       \
            परत crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      अन्यथा                                                                              \
        म_नकल (b, "*");                                                                \
  } पर्यंत (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, आकार (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (device)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, म_माप (device), err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  परत 0;
}

संरचना bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                libcrun_error_t *err)
{
  पूर्णांक i;
  संरचना bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  जर (UNLIKELY (program == रिक्त))
    परत रिक्त;

  साठी (i = (आकार (default_devices) / आकार (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      जर (UNLIKELY (program == रिक्त))
        परत रिक्त;
    }
  साठी (i = devs_len - 1; i >= 0; i--)
    {
      अक्षर type = 'a';
      पूर्णांक minor = -1, major = -1;
      जर (devs[i]->type != रिक्त)
        type = devs[i]->type[0];

      जर (devs[i]->major_present)
        major = devs[i]->major;
      जर (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      जर (UNLIKELY (program == रिक्त))
        परत रिक्त;
    }

  program = bpf_program_complete_dev (program, err);
  जर (UNLIKELY (program == रिक्त))
    परत रिक्त;

  परत program;
}

स्थिर पूर्णांक
write_devices_resources_v2_internal (पूर्णांक dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free संरचना bpf_program *program = रिक्त;

  program = create_dev_bpf (devs, devs_len, err);
  जर (UNLIKELY (program == रिक्त))
    परत -1;

  परत libcrun_ebpf_load (program, dirfd, रिक्त, err);
}

स्थिर पूर्णांक
write_devices_resources_v2 (पूर्णांक dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                            libcrun_error_t *err)
{
  पूर्णांक ret;
  माप_प्रकार i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  जर (LIKELY (ret == 0))
    परत 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  साठी (i = 0; i < devs_len; i++)
    {
      जर (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          थांबा;
        }
    }

  जर (! can_skip)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णांक rootless;

      rootless = is_rootless (&tmp_err);
      जर (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          परत ret;
        }
      जर (rootless)
        can_skip = true;
    }

  जर (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  परत ret;
}

स्थिर पूर्णांक
write_devices_resources (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                         libcrun_error_t *err)
{
  पूर्णांक ret;

  जर (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  अन्यथा
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  जर (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णांक rootless;

      rootless = is_rootless (&tmp_err);
      जर (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          परत ret;
        }

      जर (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  परत ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
स्थिर पूर्णांक
cg_itoa (अक्षर *buf, माप_प्रकार size, int64_t मूल्य, bool use_max, libcrun_error_t *err)
{
  पूर्णांक len;

  जर (use_max && मूल्य < 0)
    len = snprintf (buf, size, "max");
  अन्यथा
    len = snprintf (buf, size, "%" PRIi64, मूल्य);

  जर (UNLIKELY (len >= (पूर्णांक) size))
    परत crun_make_error (err, 0, "internal error: static buffer too small");

  परत len;
}

स्थिर पूर्णांक
write_memory (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  अक्षर limit_buf[32];
  पूर्णांक limit_buf_len;

  जर (! memory->limit_present)
    परत 0;

  limit_buf_len = cg_itoa (limit_buf, आकार (limit_buf), memory->limit, cgroup2, err);
  जर (UNLIKELY (limit_buf_len < 0))
    परत limit_buf_len;

  परत write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

स्थिर पूर्णांक
write_memory_swap (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  पूर्णांक ret;
  int64_t swap;
  अक्षर swap_buf[32];
  पूर्णांक len;
  स्थिरांक अक्षर *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  जर (! memory->swap_present)
    परत 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  जर (cgroup2 && memory->swap > 0)
    {
      जर (! memory->limit_present)
        परत crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      जर (memory->swap < memory->limit)
        परत crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, आकार (swap_buf), swap, cgroup2, err);
  जर (UNLIKELY (len < 0))
    परत len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  जर (ret >= 0)
    परत ret;

  /* If swap is not enabled, ignore the error.  */
  जर (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      परत 0;
    }

  परत ret;
}

स्थिर पूर्णांक
write_memory_resources (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  पूर्णांक len;
  पूर्णांक ret;
  अक्षर fmt_buf[32];
  bool memory_limits_written = false;

  जर (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      जर (ret >= 0)
        memory_limits_written = true;
      अन्यथा
        {
          जर (cgroup2 || crun_error_get_errno (err) != EINVAL)
            परत ret;

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
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (memory->kernel_present)
    {
      जर (cgroup2)
        परत crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64, memory->kernel);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  जर (memory->use_hierarchy_present)
    {
      जर (cgroup2)
        परत crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (memory->reservation_present)
    {
      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64, memory->reservation);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 रिक्त, fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  जर (memory->disable_oom_killer)
    {
      जर (cgroup2)
        परत crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  जर (memory->kernel_tcp_present)
    {
      जर (cgroup2)
        परत crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  जर (memory->swappiness_present)
    {
      जर (cgroup2)
        परत crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64, memory->swappiness);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  परत 0;
}

पूर्णांक
write_cpu_idle (पूर्णांक cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  अक्षर fmt_buf[32];
  पूर्णांक len;

  जर (! cpu->idle_present)
    परत 0;

  len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIi64, cpu->idle);
  जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
    परत crun_make_error (err, 0, "internal error: static buffer too small");
  परत write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

पूर्णांक
write_cpu_burst (पूर्णांक cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  अक्षर fmt_buf[32];
  पूर्णांक len;

  जर (! cpu->burst_present)
    परत 0;

  len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIi64, cpu->burst);
  जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
    परत crun_make_error (err, 0, "internal error: static buffer too small");
  परत write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

स्थिर पूर्णांक
write_pids_resources (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  जर (pids->limit)
    {
      अक्षर fmt_buf[32];
      पूर्णांक len;
      पूर्णांक ret;

      len = cg_itoa (fmt_buf, आकार (fmt_buf), pids->limit, true, err);
      जर (UNLIKELY (len < 0))
        परत len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", रिक्त, fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  परत 0;
}

स्थिर पूर्णांक
write_cpu_resources (पूर्णांक dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  पूर्णांक len, period_len = 0;
  पूर्णांक ret;
  अक्षर fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free अक्षर *period_str = रिक्त;

  जर (cpu->shares)
    {
      uint32_t val = cpu->shares;

      जर (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          जर (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              जर (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, आकार (fmt_buf), "%u", val);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 रिक्त, fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  जर (cpu->period)
    {
      जर (cgroup2)
        period = cpu->period;
      अन्यथा
        {
          len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64, cpu->period);
          जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
            परत crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          जर (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              जर (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                परत ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  जर (cpu->quota)
    {
      जर (cgroup2)
        quota = cpu->quota;
      अन्यथा
        {
          len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIi64, cpu->quota);
          जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
            परत crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          जर (period_str != रिक्त)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              जर (UNLIKELY (ret < 0))
                परत ret;
            }
        }
    }
  जर (cpu->realtime_period)
    {
      जर (cgroup2)
        परत crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64, cpu->realtime_period);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  जर (cpu->realtime_runtime)
    {
      जर (cgroup2)
        परत crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (cgroup2 && (quota > 0 || period > 0))
    {
      जर (period < 0)
        period = 100000;
      जर (quota < 0)
        len = snprintf (fmt_buf, आकार (fmt_buf), "max %" PRIi64, period);
      अन्यथा
        len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", रिक्त, fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  परत write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

पूर्णांक
write_cpuset_resources (पूर्णांक dirfd_cpuset, पूर्णांक cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  पूर्णांक ret;

  जर (cpu == रिक्त)
    परत 0;

  जर (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, म_माप (cpu->cpus), err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  जर (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, म_माप (cpu->mems),
                                                 err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  परत 0;
}

स्थिर पूर्णांक
open_cgroup_subsystem (स्थिरांक अक्षर *subsystem, स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *full_path = रिक्त;
  पूर्णांक ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_मूळ, subsystem, path, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  dirfd = खोलो (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  जर (UNLIKELY (dirfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", full_path);

  परत dirfd;
}

पूर्णांक
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  पूर्णांक ret;

  जर (resources->block_io)
    {
      cleanup_close पूर्णांक dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      जर (UNLIKELY (dirfd_blkio < 0))
        परत dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (resources->network)
    {
      cleanup_close पूर्णांक dirfd_netclass = -1;
      cleanup_close पूर्णांक dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      जर (UNLIKELY (dirfd_netclass < 0))
        परत dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      जर (UNLIKELY (dirfd_netprio < 0))
        परत dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (resources->hugepage_limits_len)
    {
      cleanup_close पूर्णांक dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      जर (UNLIKELY (dirfd_htlb < 0))
        परत dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (resources->devices_len)
    {
      cleanup_close पूर्णांक dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      जर (UNLIKELY (dirfd_devs < 0))
        परत dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (resources->memory)
    {
      cleanup_close पूर्णांक dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      जर (UNLIKELY (dirfd_mem < 0))
        परत dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (resources->pids)
    {
      cleanup_close पूर्णांक dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      जर (UNLIKELY (dirfd_pid < 0))
        परत dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (resources->cpu)
    {
      cleanup_close पूर्णांक dirfd_cpu = -1;
      cleanup_close पूर्णांक dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      जर (UNLIKELY (dirfd_cpu < 0))
        परत dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      जर (resources->cpu->cpus == रिक्त && resources->cpu->mems == रिक्त)
        परत 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      जर (UNLIKELY (dirfd_cpuset < 0))
        परत dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (resources->unified && resources->unified->len > 0)
    परत crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  परत 0;
}

स्थिर पूर्णांक
write_unified_resources (पूर्णांक cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  माप_प्रकार i;
  पूर्णांक ret;

  साठी (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close पूर्णांक fd = -1;
      cleanup_free अक्षर *मूल्य = रिक्त;
      अक्षर *saveptr = रिक्त;
      अक्षर *ओळ;

      जर (म_अक्षर (resources->unified->किल्ल्या[i], '/'))
        परत crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->किल्ल्या[i]);

      जर (is_empty_string (resources->unified->मूल्ये[i]))
        सुरूठेवा;

      मूल्य = xstrdup (resources->unified->मूल्ये[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->किल्ल्या[i], O_WRONLY, err);
      जर (UNLIKELY (fd < 0))
        परत fd;

      साठी (ओळ = strtok_r (मूल्य, "\n", &saveptr); ओळ; ओळ = strtok_r (रिक्त, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (लिख (fd, ओळ, म_माप (ओळ)));
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "write to `%s`", resources->unified->किल्ल्या[i]);
        }
    }

  परत 0;
}

स्थिर पूर्णांक
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, स्थिरांक अक्षर *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = रिक्त;
  cleanup_close पूर्णांक cgroup_dirfd = -1;
  पूर्णांक ret;

  जर (resources->network)
    परत crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_मूळ, path, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  cgroup_dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  जर (UNLIKELY (cgroup_dirfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  जर (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  जर (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  जर (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  जर (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  जर (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  परत 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
पूर्णांक
check_memory_before_update (स्थिरांक अक्षर *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free अक्षर *swap_current = रिक्त;
  cleanup_free अक्षर *cgroup_path = रिक्त;
  cleanup_free अक्षर *current = रिक्त;
  cleanup_close पूर्णांक dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  जर (path == रिक्त || memory == रिक्त || ! (memory->check_before_update_present && memory->check_before_update))
    परत 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (cgroup_mode < 0))
    परत cgroup_mode;

  जर (cgroup_mode != CGROUP_MODE_UNIFIED)
    परत 0;

  ret = append_paths (&cgroup_path, err, CGROUP_मूळ, path, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  जर (UNLIKELY (dirfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  त्रुटि_सं = 0;
  val = strtoll (current, रिक्त, 10);
  जर (UNLIKELY (त्रुटि_सं))
    परत crun_make_error (err, त्रुटि_सं, "parse memory.current");

  val_swap = strtoll (swap_current, रिक्त, 10);
  जर (UNLIKELY (त्रुटि_सं))
    परत crun_make_error (err, त्रुटि_सं, "parse memory.swap.current");

  जर (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  जर (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  जर (limit <= val + val_swap)
    परत crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  परत 0;
}

पूर्णांक
update_cgroup_resources (स्थिरांक अक्षर *path,
                         स्थिरांक अक्षर *state_मूळ,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  पूर्णांक cgroup_mode;

  (शून्य) state_मूळ;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (cgroup_mode < 0))
    परत cgroup_mode;

  जर (path == रिक्त)
    {
      माप_प्रकार i;

      जर (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        परत crun_make_error (err, 0, "cannot set limits without cgroups");

      साठी (i = 0; i < resources->devices_len; i++)
        {
          पूर्णांक rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          जर (UNLIKELY (rwm < 0))
            परत rwm;

          जर (rwm == 0)
            परत crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      परत 0;
    }
  निवड (cgroup_mode)
    {
    स्थिती CGROUP_MODE_UNIFIED:
      परत update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    स्थिती CGROUP_MODE_LEGACY:
    स्थिती CGROUP_MODE_HYBRID:
      परत update_cgroup_v1_resources (resources, path, err);

    पूर्वनिर्धारित:
      परत crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
