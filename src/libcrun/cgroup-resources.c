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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "cgroup.h"
#समावेश "cgroup-internal.h"
#समावेश "cgroup-systemd.h"
#समावेश "cgroup-utils.h"
#समावेश "cgroup-resources.h"
#समावेश "ebpf.h"
#समावेश "utils.h"
#समावेश "status.h"
#समावेश <string.h>
#समावेश <sys/types.h>
#समावेश <sys/vfs.h>
#समावेश <inttypes.h>
#समावेश <time.h>

#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#समावेश <fcntl.h>
#समावेश <libgen.h>

काष्ठा default_dev_s default_devices[] = {
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
  { 0, 0, 0, शून्य }
};

काष्ठा default_dev_s *
get_default_devices ()
{
  वापस default_devices;
}

अटल पूर्णांक
write_cgroup_file_or_alias (पूर्णांक dirfd, स्थिर अक्षर *name, स्थिर अक्षर *alias, स्थिर व्योम *तथ्य, माप_प्रकार len, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = write_cgroup_file (dirfd, name, तथ्य, len, err);
  अगर (UNLIKELY (alias != शून्य && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, तथ्य, len, err);
    }
  वापस ret;
}

अटल अंतरभूत पूर्णांक
openat_with_alias (पूर्णांक dirfd, स्थिर अक्षर *name, स्थिर अक्षर *alias, स्थिर अक्षर **used_name, पूर्णांक flags, libcrun_error_t *err)
{
  पूर्णांक ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  अगर (UNLIKELY (ret < 0 && alias != शून्य && त्रुटि_सं == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", name);
  वापस ret;
}

अटल पूर्णांक
is_rwm (स्थिर अक्षर *str, libcrun_error_t *err)
{
  स्थिर अक्षर *it;
  bool r = false;
  bool w = false;
  bool m = false;

  अगर (str == शून्य)
    str = "";

  क्रम (it = str; *it; it++)
    चयन (*it)
      {
      हाल 'r':
        r = true;
        अवरोध;

      हाल 'w':
        w = true;
        अवरोध;

      हाल 'm':
        m = true;
        अवरोध;

      शेष:
        वापस crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  वापस r && w && m ? 1 : 0;
}

अटल पूर्णांक
check_cgroup_v2_controller_available_wrapper (पूर्णांक ret, पूर्णांक cgroup_dirfd, स्थिर अक्षर *name, libcrun_error_t *err)
{
  अगर (ret == 0 || err == शून्य)
    वापस 0;

  त्रुटि_सं = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  अगर (त्रुटि_सं == ENOENT || त्रुटि_सं == EPERM || त्रुटि_सं == EACCES)
    {
      cleanup_free अक्षर *controllers = शून्य;
      libcrun_error_t tmp_err = शून्य;
      cleanup_free अक्षर *कुंजी = शून्य;
      अक्षर *saveptr = शून्य;
      bool found = false;
      स्थिर अक्षर *token;
      अक्षर *it;

      /* Check if the specified controller is enabled.  */
      कुंजी = xstrdup (name);

      it = म_अक्षर (कुंजी, '.');
      अगर (it == शून्य)
        {
          crun_error_release (err);
          वापस crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      अगर (म_भेद (कुंजी, "cgroup") == 0)
        वापस ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      अगर (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, शून्य, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          वापस ret;
        }
      क्रम (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (शून्य, " \n", &saveptr))
        {
          अगर (म_भेद (token, कुंजी) == 0)
            {
              found = true;
              अवरोध;
            }
        }
      अगर (! found)
        {
          cleanup_free अक्षर *absolute_path = शून्य;
          libcrun_error_t tmp_err = शून्य;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          अगर (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", कुंजी, absolute_path);
          अन्यथा
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", कुंजी);
            }
        }
    }
  वापस ret;
}

अटल पूर्णांक
write_file_and_check_controllers_at (bool cgroup2, पूर्णांक dirfd, स्थिर अक्षर *name, स्थिर अक्षर *name_alias,
                                     स्थिर व्योम *तथ्य, माप_प्रकार len, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, तथ्य, len, err);
  अगर (cgroup2)
    वापस check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  वापस ret;
}

अटल पूर्णांक
open_file_and_check_controllers_at (bool cgroup2, पूर्णांक dirfd, स्थिर अक्षर *name, पूर्णांक flags, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = openat (dirfd, name, flags);
  अगर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, त्रुटि_सं, "open `%s`", name);
      अगर (cgroup2)
        वापस check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  वापस ret;
}

/* The parser generates different structs but they are really all the same.  */
प्रकार runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

अटल पूर्णांक
write_blkio_v1_resources_throttling (पूर्णांक dirfd, स्थिर अक्षर *name, throttling_s **throttling, माप_प्रकार throttling_len,
                                     libcrun_error_t *err)
{
  अक्षर fmt_buf[128];
  माप_प्रकार i;
  cleanup_close पूर्णांक fd = -1;

  अगर (throttling == शून्य)
    वापस 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", name);

  क्रम (i = 0; i < throttling_len; i++)
    {
      पूर्णांक ret;
      पूर्णांक len;
      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      ret = अस्थायी_FAILURE_RETRY (ग_लिखो (fd, fmt_buf, len));
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "write `%s`", name);
    }
  वापस 0;
}

अटल पूर्णांक
write_blkio_v2_resources_throttling (पूर्णांक fd, स्थिर अक्षर *name, throttling_s **throttling, माप_प्रकार throttling_len,
                                     libcrun_error_t *err)
{
  अक्षर fmt_buf[128];
  माप_प्रकार i;

  अगर (throttling == शून्य)
    वापस 0;

  क्रम (i = 0; i < throttling_len; i++)
    {
      पूर्णांक ret;
      पूर्णांक len;
      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      ret = अस्थायी_FAILURE_RETRY (ग_लिखो (fd, fmt_buf, len));
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "write `%s`", name);
    }
  वापस 0;
}

अटल पूर्णांक
write_blkio_resources (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  अक्षर fmt_buf[128];
  पूर्णांक len;
  पूर्णांक ret;

  अगर (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu32, val);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");
      अगर (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
      अन्यथा
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          अगर (UNLIKELY (ret < 0))
            {
              अगर (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu32, val);
                  अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
                    वापस crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              अगर (UNLIKELY (ret < 0))
                वापस ret;
            }
        }
    }
  अगर (blkio->leaf_weight)
    {
      अगर (cgroup2)
        वापस crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, माप (fmt_buf), "%d", blkio->leaf_weight);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अगर (blkio->weight_device_len)
    {
      अगर (cgroup2)
        {
          cleanup_close पूर्णांक wfd = -1;
          माप_प्रकार i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          अगर (UNLIKELY (wfd < 0))
            वापस crun_make_error (err, त्रुटि_सं, "open `io.bfq.weight`");
          क्रम (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
                वापस crun_make_error (err, 0, "internal error: static buffer too small");

              ret = अस्थायी_FAILURE_RETRY (ग_लिखो (wfd, fmt_buf, len));
              अगर (UNLIKELY (ret < 0))
                वापस crun_make_error (err, त्रुटि_सं, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      अन्यथा
        {
          स्थिर अक्षर *leaf_weight_device_file_name = शून्य;
          स्थिर अक्षर *weight_device_file_name = शून्य;
          cleanup_close पूर्णांक w_leafdevice_fd = -1;
          cleanup_close पूर्णांक w_device_fd = -1;
          माप_प्रकार i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          अगर (UNLIKELY (w_device_fd < 0))
            वापस w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          अगर (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          क्रम (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
                वापस crun_make_error (err, 0, "internal error: static buffer too small");

              ret = अस्थायी_FAILURE_RETRY (ग_लिखो (w_device_fd, fmt_buf, len));
              अगर (UNLIKELY (ret < 0))
                वापस crun_make_error (err, त्रुटि_सं, "write `%s`", weight_device_file_name);

              अगर (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
                    वापस crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (w_leafdevice_fd, fmt_buf, len));
                  अगर (UNLIKELY (ret < 0))
                    वापस crun_make_error (err, त्रुटि_सं, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  अगर (cgroup2)
    {
      cleanup_close पूर्णांक wfd = -1;
      स्थिर अक्षर *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      अगर (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, त्रुटि_सं, "open `%s`", name);
          वापस check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस 0;
}

अटल पूर्णांक
write_network_resources (पूर्णांक dirfd_netclass, पूर्णांक dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  अक्षर fmt_buf[128];
  पूर्णांक len;
  पूर्णांक ret;
  अगर (net->class_id)
    {
      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu32, net->class_id);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अगर (net->priorities_len)
    {
      माप_प्रकार i;
      cleanup_close पूर्णांक fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      अगर (UNLIKELY (fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `net_prio.ifpriomap`");

      क्रम (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, माप (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
            वापस crun_make_error (err, 0, "internal error: static buffer too small");

          ret = अस्थायी_FAILURE_RETRY (ग_लिखो (fd, fmt_buf, len));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "write `net_prio.ifpriomap`");
        }
    }

  वापस 0;
}

अटल पूर्णांक
write_hugetlb_resources (पूर्णांक dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, माप_प्रकार htlb_len,
                         libcrun_error_t *err)
{
  स्थिर अक्षर *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  अक्षर fmt_buf[128];
  माप_प्रकार i;
  क्रम (i = 0; i < htlb_len; i++)
    {
      cleanup_free अक्षर *filename = शून्य;
      cleanup_free अक्षर *rsvd_filename = शून्य;
      पूर्णांक len;
      पूर्णांक ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_आकार, suffix);

      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64, htlb[i]->limit);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, शून्य, fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      अगर (skip_rsvd)
        जारी;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_आकार, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        {
          अगर (crun_error_get_errno (err) != ENOENT)
            वापस ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  वापस 0;
}

अटल पूर्णांक
write_devices_resources_v1 (पूर्णांक dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                            libcrun_error_t *err)
{
  माप_प्रकार i;
  पूर्णांक len;
  पूर्णांक ret;

  क्रम (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#घोषणा FMT_BUF_LEN 64
      अक्षर fmt_buf[FMT_BUF_LEN];
      स्थिर अक्षर *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      अगर (devs[i]->type == शून्य || devs[i]->type[0] == 'a')
        {
          म_नकल (fmt_buf, "a");
          len = 1;
        }
      अन्यथा
        {
          अक्षर fmt_buf_major[16];
          अक्षर fmt_buf_minor[16];

#घोषणा FMT_DEV(x, b)                                                                   \
  करो                                                                                    \
    {                                                                                   \
      अगर (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, माप (b), "%" PRIi64, x);                                \
          अगर (UNLIKELY (len >= (पूर्णांक) माप (b)))                                       \
            वापस crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      अन्यथा                                                                              \
        म_नकल (b, "*");                                                                \
  } जबतक (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#अघोषित FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          अगर (UNLIKELY (len >= FMT_BUF_LEN))
            वापस crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  क्रम (i = 0; default_devices[i].type; i++)
    {
      अक्षर fmt_buf_major[16];
      अक्षर fmt_buf_minor[16];
      अक्षर device[64];
      पूर्णांक len;

#घोषणा FMT_DEV(x, b)                                                                   \
  करो                                                                                    \
    {                                                                                   \
      अगर (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, माप (b), "%d", x);                                      \
          अगर (UNLIKELY (len >= (पूर्णांक) माप (b)))                                       \
            वापस crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      अन्यथा                                                                              \
        म_नकल (b, "*");                                                                \
  } जबतक (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#अघोषित FMT_DEV

      len = snprintf (device, माप (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (device)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, म_माप (device), err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

काष्ठा bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                libcrun_error_t *err)
{
  पूर्णांक i;
  काष्ठा bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  अगर (UNLIKELY (program == शून्य))
    वापस शून्य;

  क्रम (i = (माप (default_devices) / माप (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      अगर (UNLIKELY (program == शून्य))
        वापस शून्य;
    }
  क्रम (i = devs_len - 1; i >= 0; i--)
    {
      अक्षर type = 'a';
      पूर्णांक minor = -1, major = -1;
      अगर (devs[i]->type != शून्य)
        type = devs[i]->type[0];

      अगर (devs[i]->major_present)
        major = devs[i]->major;
      अगर (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      अगर (UNLIKELY (program == शून्य))
        वापस शून्य;
    }

  program = bpf_program_complete_dev (program, err);
  अगर (UNLIKELY (program == शून्य))
    वापस शून्य;

  वापस program;
}

अटल पूर्णांक
write_devices_resources_v2_internal (पूर्णांक dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free काष्ठा bpf_program *program = शून्य;

  program = create_dev_bpf (devs, devs_len, err);
  अगर (UNLIKELY (program == शून्य))
    वापस -1;

  वापस libcrun_ebpf_load (program, dirfd, शून्य, err);
}

अटल पूर्णांक
write_devices_resources_v2 (पूर्णांक dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                            libcrun_error_t *err)
{
  पूर्णांक ret;
  माप_प्रकार i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  अगर (LIKELY (ret == 0))
    वापस 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  क्रम (i = 0; i < devs_len; i++)
    {
      अगर (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          अवरोध;
        }
    }

  अगर (! can_skip)
    {
      libcrun_error_t tmp_err = शून्य;
      पूर्णांक rootless;

      rootless = is_rootless (&tmp_err);
      अगर (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          वापस ret;
        }
      अगर (rootless)
        can_skip = true;
    }

  अगर (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  वापस ret;
}

अटल पूर्णांक
write_devices_resources (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                         libcrun_error_t *err)
{
  पूर्णांक ret;

  अगर (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  अन्यथा
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  अगर (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = शून्य;
      पूर्णांक rootless;

      rootless = is_rootless (&tmp_err);
      अगर (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          वापस ret;
        }

      अगर (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  वापस ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
अटल पूर्णांक
cg_itoa (अक्षर *buf, माप_प्रकार आकार, int64_t मूल्य, bool use_max, libcrun_error_t *err)
{
  पूर्णांक len;

  अगर (use_max && मूल्य < 0)
    len = snprintf (buf, आकार, "max");
  अन्यथा
    len = snprintf (buf, आकार, "%" PRIi64, मूल्य);

  अगर (UNLIKELY (len >= (पूर्णांक) आकार))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");

  वापस len;
}

अटल पूर्णांक
write_memory (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  अक्षर limit_buf[32];
  पूर्णांक limit_buf_len;

  अगर (! memory->limit_present)
    वापस 0;

  limit_buf_len = cg_itoa (limit_buf, माप (limit_buf), memory->limit, cgroup2, err);
  अगर (UNLIKELY (limit_buf_len < 0))
    वापस limit_buf_len;

  वापस write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

अटल पूर्णांक
write_memory_swap (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  पूर्णांक ret;
  int64_t swap;
  अक्षर swap_buf[32];
  पूर्णांक len;
  स्थिर अक्षर *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  अगर (! memory->swap_present)
    वापस 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  अगर (cgroup2 && memory->swap > 0)
    {
      अगर (! memory->limit_present)
        वापस crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      अगर (memory->swap < memory->limit)
        वापस crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, माप (swap_buf), swap, cgroup2, err);
  अगर (UNLIKELY (len < 0))
    वापस len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  अगर (ret >= 0)
    वापस ret;

  /* If swap is not enabled, ignore the error.  */
  अगर (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      वापस 0;
    }

  वापस ret;
}

अटल पूर्णांक
write_memory_resources (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  पूर्णांक len;
  पूर्णांक ret;
  अक्षर fmt_buf[32];
  bool memory_limits_written = false;

  अगर (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      अगर (ret >= 0)
        memory_limits_written = true;
      अन्यथा
        {
          अगर (cgroup2 || crun_error_get_errno (err) != EINVAL)
            वापस ret;

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
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (memory->kernel_present)
    {
      अगर (cgroup2)
        वापस crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64, memory->kernel);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  अगर (memory->use_hierarchy_present)
    {
      अगर (cgroup2)
        वापस crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (memory->reservation_present)
    {
      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64, memory->reservation);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 शून्य, fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अगर (memory->disable_oom_killer)
    {
      अगर (cgroup2)
        वापस crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अगर (memory->kernel_tcp_present)
    {
      अगर (cgroup2)
        वापस crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अगर (memory->swappiness_present)
    {
      अगर (cgroup2)
        वापस crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64, memory->swappiness);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

पूर्णांक
write_cpu_idle (पूर्णांक cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  अक्षर fmt_buf[32];
  पूर्णांक len;

  अगर (! cpu->idle_present)
    वापस 0;

  len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIi64, cpu->idle);
  अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");
  वापस write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

पूर्णांक
write_cpu_burst (पूर्णांक cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  अक्षर fmt_buf[32];
  पूर्णांक len;

  अगर (! cpu->burst_present)
    वापस 0;

  len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIi64, cpu->burst);
  अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");
  वापस write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

अटल पूर्णांक
write_pids_resources (पूर्णांक dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  अगर (pids->limit)
    {
      अक्षर fmt_buf[32];
      पूर्णांक len;
      पूर्णांक ret;

      len = cg_itoa (fmt_buf, माप (fmt_buf), pids->limit, true, err);
      अगर (UNLIKELY (len < 0))
        वापस len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", शून्य, fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

अटल पूर्णांक
write_cpu_resources (पूर्णांक dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  पूर्णांक len, period_len = 0;
  पूर्णांक ret;
  अक्षर fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free अक्षर *period_str = शून्य;

  अगर (cpu->shares)
    {
      uint32_t val = cpu->shares;

      अगर (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          अगर (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              अगर (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, माप (fmt_buf), "%u", val);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 शून्य, fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अगर (cpu->period)
    {
      अगर (cgroup2)
        period = cpu->period;
      अन्यथा
        {
          len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64, cpu->period);
          अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
            वापस crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          अगर (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              अगर (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                वापस ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  अगर (cpu->quota)
    {
      अगर (cgroup2)
        quota = cpu->quota;
      अन्यथा
        {
          len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIi64, cpu->quota);
          अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
            वापस crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          अगर (period_str != शून्य)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              अगर (UNLIKELY (ret < 0))
                वापस ret;
            }
        }
    }
  अगर (cpu->realtime_period)
    {
      अगर (cgroup2)
        वापस crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64, cpu->realtime_period);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अगर (cpu->realtime_runtime)
    {
      अगर (cgroup2)
        वापस crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (cgroup2 && (quota > 0 || period > 0))
    {
      अगर (period < 0)
        period = 100000;
      अगर (quota < 0)
        len = snprintf (fmt_buf, माप (fmt_buf), "max %" PRIi64, period);
      अन्यथा
        len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", शून्य, fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

पूर्णांक
write_cpuset_resources (पूर्णांक dirfd_cpuset, पूर्णांक cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  पूर्णांक ret;

  अगर (cpu == शून्य)
    वापस 0;

  अगर (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, म_माप (cpu->cpus), err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अगर (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, म_माप (cpu->mems),
                                                 err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस 0;
}

अटल पूर्णांक
open_cgroup_subsystem (स्थिर अक्षर *subsystem, स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *full_path = शून्य;
  पूर्णांक ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_मूल, subsystem, path, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  dirfd = खोलो (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (dirfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", full_path);

  वापस dirfd;
}

पूर्णांक
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, स्थिर अक्षर *path, libcrun_error_t *err)
{
  पूर्णांक ret;

  अगर (resources->block_io)
    {
      cleanup_close पूर्णांक dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      अगर (UNLIKELY (dirfd_blkio < 0))
        वापस dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (resources->network)
    {
      cleanup_close पूर्णांक dirfd_netclass = -1;
      cleanup_close पूर्णांक dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      अगर (UNLIKELY (dirfd_netclass < 0))
        वापस dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      अगर (UNLIKELY (dirfd_netprio < 0))
        वापस dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (resources->hugepage_limits_len)
    {
      cleanup_close पूर्णांक dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      अगर (UNLIKELY (dirfd_htlb < 0))
        वापस dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (resources->devices_len)
    {
      cleanup_close पूर्णांक dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      अगर (UNLIKELY (dirfd_devs < 0))
        वापस dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (resources->memory)
    {
      cleanup_close पूर्णांक dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      अगर (UNLIKELY (dirfd_mem < 0))
        वापस dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (resources->pids)
    {
      cleanup_close पूर्णांक dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      अगर (UNLIKELY (dirfd_pid < 0))
        वापस dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (resources->cpu)
    {
      cleanup_close पूर्णांक dirfd_cpu = -1;
      cleanup_close पूर्णांक dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      अगर (UNLIKELY (dirfd_cpu < 0))
        वापस dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      अगर (resources->cpu->cpus == शून्य && resources->cpu->mems == शून्य)
        वापस 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      अगर (UNLIKELY (dirfd_cpuset < 0))
        वापस dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (resources->unified && resources->unified->len > 0)
    वापस crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  वापस 0;
}

अटल पूर्णांक
write_unified_resources (पूर्णांक cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  माप_प्रकार i;
  पूर्णांक ret;

  क्रम (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close पूर्णांक fd = -1;
      cleanup_free अक्षर *मूल्य = शून्य;
      अक्षर *saveptr = शून्य;
      अक्षर *पंक्ति;

      अगर (म_अक्षर (resources->unified->कुंजियाँ[i], '/'))
        वापस crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->कुंजियाँ[i]);

      अगर (is_empty_string (resources->unified->मूल्यावली[i]))
        जारी;

      मूल्य = xstrdup (resources->unified->मूल्यावली[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->कुंजियाँ[i], O_WRONLY, err);
      अगर (UNLIKELY (fd < 0))
        वापस fd;

      क्रम (पंक्ति = strtok_r (मूल्य, "\n", &saveptr); पंक्ति; पंक्ति = strtok_r (शून्य, "\n", &saveptr))
        {
          ret = अस्थायी_FAILURE_RETRY (ग_लिखो (fd, पंक्ति, म_माप (पंक्ति)));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "write to `%s`", resources->unified->कुंजियाँ[i]);
        }
    }

  वापस 0;
}

अटल पूर्णांक
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, स्थिर अक्षर *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = शून्य;
  cleanup_close पूर्णांक cgroup_dirfd = -1;
  पूर्णांक ret;

  अगर (resources->network)
    वापस crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_मूल, path, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  cgroup_dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (cgroup_dirfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  अगर (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अगर (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अगर (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अगर (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  अगर (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
पूर्णांक
check_memory_before_update (स्थिर अक्षर *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free अक्षर *swap_current = शून्य;
  cleanup_free अक्षर *cgroup_path = शून्य;
  cleanup_free अक्षर *current = शून्य;
  cleanup_close पूर्णांक dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  पूर्णांक cgroup_mode;
  पूर्णांक ret;

  अगर (path == शून्य || memory == शून्य || ! (memory->check_before_update_present && memory->check_before_update))
    वापस 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  अगर (cgroup_mode != CGROUP_MODE_UNIFIED)
    वापस 0;

  ret = append_paths (&cgroup_path, err, CGROUP_मूल, path, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (dirfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  त्रुटि_सं = 0;
  val = strtoll (current, शून्य, 10);
  अगर (UNLIKELY (त्रुटि_सं))
    वापस crun_make_error (err, त्रुटि_सं, "parse memory.current");

  val_swap = strtoll (swap_current, शून्य, 10);
  अगर (UNLIKELY (त्रुटि_सं))
    वापस crun_make_error (err, त्रुटि_सं, "parse memory.swap.current");

  अगर (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  अगर (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  अगर (limit <= val + val_swap)
    वापस crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  वापस 0;
}

पूर्णांक
update_cgroup_resources (स्थिर अक्षर *path,
                         स्थिर अक्षर *state_मूल,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  पूर्णांक cgroup_mode;

  (व्योम) state_मूल;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  अगर (path == शून्य)
    {
      माप_प्रकार i;

      अगर (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        वापस crun_make_error (err, 0, "cannot set limits without cgroups");

      क्रम (i = 0; i < resources->devices_len; i++)
        {
          पूर्णांक rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          अगर (UNLIKELY (rwm < 0))
            वापस rwm;

          अगर (rwm == 0)
            वापस crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      वापस 0;
    }
  चयन (cgroup_mode)
    {
    हाल CGROUP_MODE_UNIFIED:
      वापस update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    हाल CGROUP_MODE_LEGACY:
    हाल CGROUP_MODE_HYBRID:
      वापस update_cgroup_v1_resources (resources, path, err);

    शेष:
      वापस crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
