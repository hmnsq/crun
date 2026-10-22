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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "cgroup.h"
#அந்தர்பாவ "cgroup-internal.h"
#அந்தர்பாவ "cgroup-systemd.h"
#அந்தர்பாவ "cgroup-utils.h"
#அந்தர்பாவ "cgroup-resources.h"
#அந்தர்பாவ "ebpf.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "status.h"
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/vfs.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ <time.h>

#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <libgen.h>

கட்டமைப்பு default_dev_s default_devices[] = {
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
  { 0, 0, 0, வெற்று }
};

கட்டமைப்பு default_dev_s *
get_default_devices ()
{
  திரும்பு default_devices;
}

நிலையான முழு
write_cgroup_file_or_alias (முழு dirfd, மாறிலி எழுத்து *name, மாறிலி எழுத்து *alias, மாறிலி வெறுமை *data, size_t len, libcrun_error_t *err)
{
  முழு ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  எனில் (UNLIKELY (alias != வெற்று && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  திரும்பு ret;
}

நிலையான உள்வரி முழு
openat_with_alias (முழு dirfd, மாறிலி எழுத்து *name, மாறிலி எழுத்து *alias, மாறிலி எழுத்து **used_name, முழு flags, libcrun_error_t *err)
{
  முழு ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  எனில் (UNLIKELY (ret < 0 && alias != வெற்று && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", name);
  திரும்பு ret;
}

நிலையான முழு
is_rwm (மாறிலி எழுத்து *str, libcrun_error_t *err)
{
  மாறிலி எழுத்து *it;
  bool r = false;
  bool w = false;
  bool m = false;

  எனில் (str == வெற்று)
    str = "";

  க்கு (it = str; *it; it++)
    தேர்வு (*it)
      {
      நிலை 'r':
        r = true;
        நிறுத்து;

      நிலை 'w':
        w = true;
        நிறுத்து;

      நிலை 'm':
        m = true;
        நிறுத்து;

      இயல்பு:
        திரும்பு crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  திரும்பு r && w && m ? 1 : 0;
}

நிலையான முழு
check_cgroup_v2_controller_available_wrapper (முழு ret, முழு cgroup_dirfd, மாறிலி எழுத்து *name, libcrun_error_t *err)
{
  எனில் (ret == 0 || err == வெற்று)
    திரும்பு 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  எனில் (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free எழுத்து *controllers = வெற்று;
      libcrun_error_t tmp_err = வெற்று;
      cleanup_free எழுத்து *சாவி = வெற்று;
      எழுத்து *saveptr = வெற்று;
      bool found = false;
      மாறிலி எழுத்து *token;
      எழுத்து *it;

      /* Check if the specified controller is enabled.  */
      சாவி = xstrdup (name);

      it = strchr (சாவி, '.');
      எனில் (it == வெற்று)
        {
          crun_error_release (err);
          திரும்பு crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      எனில் (strcmp (சாவி, "cgroup") == 0)
        திரும்பு ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      எனில் (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, வெற்று, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          திரும்பு ret;
        }
      க்கு (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (வெற்று, " \n", &saveptr))
        {
          எனில் (strcmp (token, சாவி) == 0)
            {
              found = true;
              நிறுத்து;
            }
        }
      எனில் (! found)
        {
          cleanup_free எழுத்து *absolute_path = வெற்று;
          libcrun_error_t tmp_err = வெற்று;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          எனில் (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", சாவி, absolute_path);
          இல்லையெனில்
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", சாவி);
            }
        }
    }
  திரும்பு ret;
}

நிலையான முழு
write_file_and_check_controllers_at (bool cgroup2, முழு dirfd, மாறிலி எழுத்து *name, மாறிலி எழுத்து *name_alias,
                                     மாறிலி வெறுமை *data, size_t len, libcrun_error_t *err)
{
  முழு ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  எனில் (cgroup2)
    திரும்பு check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  திரும்பு ret;
}

நிலையான முழு
open_file_and_check_controllers_at (bool cgroup2, முழு dirfd, மாறிலி எழுத்து *name, முழு flags, libcrun_error_t *err)
{
  முழு ret;

  ret = openat (dirfd, name, flags);
  எனில் (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      எனில் (cgroup2)
        திரும்பு check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  திரும்பு ret;
}

/* The parser generates different structs but they are really all the same.  */
வகைவரை runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

நிலையான முழு
write_blkio_v1_resources_throttling (முழு dirfd, மாறிலி எழுத்து *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  எழுத்து fmt_buf[128];
  size_t i;
  cleanup_close முழு fd = -1;

  எனில் (throttling == வெற்று)
    திரும்பு 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", name);

  க்கு (i = 0; i < throttling_len; i++)
    {
      முழு ret;
      முழு len;
      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (லிக (fd, fmt_buf, len));
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "write `%s`", name);
    }
  திரும்பு 0;
}

நிலையான முழு
write_blkio_v2_resources_throttling (முழு fd, மாறிலி எழுத்து *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  எழுத்து fmt_buf[128];
  size_t i;

  எனில் (throttling == வெற்று)
    திரும்பு 0;

  க்கு (i = 0; i < throttling_len; i++)
    {
      முழு ret;
      முழு len;
      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (லிக (fd, fmt_buf, len));
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "write `%s`", name);
    }
  திரும்பு 0;
}

நிலையான முழு
write_blkio_resources (முழு dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  எழுத்து fmt_buf[128];
  முழு len;
  முழு ret;

  எனில் (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu32, val);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
      எனில் (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
      இல்லையெனில்
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          எனில் (UNLIKELY (ret < 0))
            {
              எனில் (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu32, val);
                  எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
                    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;
            }
        }
    }
  எனில் (blkio->leaf_weight)
    {
      எனில் (cgroup2)
        திரும்பு crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, அளவு (fmt_buf), "%d", blkio->leaf_weight);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  எனில் (blkio->weight_device_len)
    {
      எனில் (cgroup2)
        {
          cleanup_close முழு wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          எனில் (UNLIKELY (wfd < 0))
            திரும்பு crun_make_error (err, errno, "open `io.bfq.weight`");
          க்கு (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
                திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (லிக (wfd, fmt_buf, len));
              எனில் (UNLIKELY (ret < 0))
                திரும்பு crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      இல்லையெனில்
        {
          மாறிலி எழுத்து *leaf_weight_device_file_name = வெற்று;
          மாறிலி எழுத்து *weight_device_file_name = வெற்று;
          cleanup_close முழு w_leafdevice_fd = -1;
          cleanup_close முழு w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          எனில் (UNLIKELY (w_device_fd < 0))
            திரும்பு w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          எனில் (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          க்கு (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
                திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (லிக (w_device_fd, fmt_buf, len));
              எனில் (UNLIKELY (ret < 0))
                திரும்பு crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              எனில் (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
                    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (லிக (w_leafdevice_fd, fmt_buf, len));
                  எனில் (UNLIKELY (ret < 0))
                    திரும்பு crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  எனில் (cgroup2)
    {
      cleanup_close முழு wfd = -1;
      மாறிலி எழுத்து *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      எனில் (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          திரும்பு check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு 0;
}

நிலையான முழு
write_network_resources (முழு dirfd_netclass, முழு dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  எழுத்து fmt_buf[128];
  முழு len;
  முழு ret;
  எனில் (net->class_id)
    {
      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu32, net->class_id);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  எனில் (net->priorities_len)
    {
      size_t i;
      cleanup_close முழு fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      எனில் (UNLIKELY (fd < 0))
        திரும்பு crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      க்கு (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, அளவு (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
            திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (லிக (fd, fmt_buf, len));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  திரும்பு 0;
}

நிலையான முழு
write_hugetlb_resources (முழு dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  மாறிலி எழுத்து *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  எழுத்து fmt_buf[128];
  size_t i;
  க்கு (i = 0; i < htlb_len; i++)
    {
      cleanup_free எழுத்து *filename = வெற்று;
      cleanup_free எழுத்து *rsvd_filename = வெற்று;
      முழு len;
      முழு ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64, htlb[i]->limit);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, வெற்று, fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      எனில் (skip_rsvd)
        தொடர்;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (crun_error_get_errno (err) != ENOENT)
            திரும்பு ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  திரும்பு 0;
}

நிலையான முழு
write_devices_resources_v1 (முழு dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  முழு len;
  முழு ret;

  க்கு (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      எழுத்து fmt_buf[FMT_BUF_LEN];
      மாறிலி எழுத்து *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      எனில் (devs[i]->type == வெற்று || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      இல்லையெனில்
        {
          எழுத்து fmt_buf_major[16];
          எழுத்து fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  செய்                                                                                    \
    {                                                                                   \
      எனில் (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, அளவு (b), "%" PRIi64, x);                                \
          எனில் (UNLIKELY (len >= (முழு) அளவு (b)))                                       \
            திரும்பு crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      இல்லையெனில்                                                                              \
        strcpy (b, "*");                                                                \
  } போது (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          எனில் (UNLIKELY (len >= FMT_BUF_LEN))
            திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  க்கு (i = 0; default_devices[i].type; i++)
    {
      எழுத்து fmt_buf_major[16];
      எழுத்து fmt_buf_minor[16];
      எழுத்து device[64];
      முழு len;

#define FMT_DEV(x, b)                                                                   \
  செய்                                                                                    \
    {                                                                                   \
      எனில் (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, அளவு (b), "%d", x);                                      \
          எனில் (UNLIKELY (len >= (முழு) அளவு (b)))                                       \
            திரும்பு crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      இல்லையெனில்                                                                              \
        strcpy (b, "*");                                                                \
  } போது (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, அளவு (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      எனில் (UNLIKELY (len >= (முழு) அளவு (device)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

கட்டமைப்பு bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  முழு i;
  கட்டமைப்பு bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  எனில் (UNLIKELY (program == வெற்று))
    திரும்பு வெற்று;

  க்கு (i = (அளவு (default_devices) / அளவு (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      எனில் (UNLIKELY (program == வெற்று))
        திரும்பு வெற்று;
    }
  க்கு (i = devs_len - 1; i >= 0; i--)
    {
      எழுத்து type = 'a';
      முழு minor = -1, major = -1;
      எனில் (devs[i]->type != வெற்று)
        type = devs[i]->type[0];

      எனில் (devs[i]->major_present)
        major = devs[i]->major;
      எனில் (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      எனில் (UNLIKELY (program == வெற்று))
        திரும்பு வெற்று;
    }

  program = bpf_program_complete_dev (program, err);
  எனில் (UNLIKELY (program == வெற்று))
    திரும்பு வெற்று;

  திரும்பு program;
}

நிலையான முழு
write_devices_resources_v2_internal (முழு dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free கட்டமைப்பு bpf_program *program = வெற்று;

  program = create_dev_bpf (devs, devs_len, err);
  எனில் (UNLIKELY (program == வெற்று))
    திரும்பு -1;

  திரும்பு libcrun_ebpf_load (program, dirfd, வெற்று, err);
}

நிலையான முழு
write_devices_resources_v2 (முழு dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  முழு ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  எனில் (LIKELY (ret == 0))
    திரும்பு 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  க்கு (i = 0; i < devs_len; i++)
    {
      எனில் (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          நிறுத்து;
        }
    }

  எனில் (! can_skip)
    {
      libcrun_error_t tmp_err = வெற்று;
      முழு rootless;

      rootless = is_rootless (&tmp_err);
      எனில் (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          திரும்பு ret;
        }
      எனில் (rootless)
        can_skip = true;
    }

  எனில் (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  திரும்பு ret;
}

நிலையான முழு
write_devices_resources (முழு dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  முழு ret;

  எனில் (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  இல்லையெனில்
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  எனில் (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = வெற்று;
      முழு rootless;

      rootless = is_rootless (&tmp_err);
      எனில் (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          திரும்பு ret;
        }

      எனில் (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  திரும்பு ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
நிலையான முழு
cg_itoa (எழுத்து *buf, size_t size, int64_t மதிப்பு, bool use_max, libcrun_error_t *err)
{
  முழு len;

  எனில் (use_max && மதிப்பு < 0)
    len = snprintf (buf, size, "max");
  இல்லையெனில்
    len = snprintf (buf, size, "%" PRIi64, மதிப்பு);

  எனில் (UNLIKELY (len >= (முழு) size))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

  திரும்பு len;
}

நிலையான முழு
write_memory (முழு dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  எழுத்து limit_buf[32];
  முழு limit_buf_len;

  எனில் (! memory->limit_present)
    திரும்பு 0;

  limit_buf_len = cg_itoa (limit_buf, அளவு (limit_buf), memory->limit, cgroup2, err);
  எனில் (UNLIKELY (limit_buf_len < 0))
    திரும்பு limit_buf_len;

  திரும்பு write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

நிலையான முழு
write_memory_swap (முழு dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  முழு ret;
  int64_t swap;
  எழுத்து swap_buf[32];
  முழு len;
  மாறிலி எழுத்து *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  எனில் (! memory->swap_present)
    திரும்பு 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  எனில் (cgroup2 && memory->swap > 0)
    {
      எனில் (! memory->limit_present)
        திரும்பு crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      எனில் (memory->swap < memory->limit)
        திரும்பு crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, அளவு (swap_buf), swap, cgroup2, err);
  எனில் (UNLIKELY (len < 0))
    திரும்பு len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  எனில் (ret >= 0)
    திரும்பு ret;

  /* If swap is not enabled, ignore the error.  */
  எனில் (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      திரும்பு 0;
    }

  திரும்பு ret;
}

நிலையான முழு
write_memory_resources (முழு dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  முழு len;
  முழு ret;
  எழுத்து fmt_buf[32];
  bool memory_limits_written = false;

  எனில் (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      எனில் (ret >= 0)
        memory_limits_written = true;
      இல்லையெனில்
        {
          எனில் (cgroup2 || crun_error_get_errno (err) != EINVAL)
            திரும்பு ret;

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
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (memory->kernel_present)
    {
      எனில் (cgroup2)
        திரும்பு crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64, memory->kernel);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  எனில் (memory->use_hierarchy_present)
    {
      எனில் (cgroup2)
        திரும்பு crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (memory->reservation_present)
    {
      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64, memory->reservation);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 வெற்று, fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  எனில் (memory->disable_oom_killer)
    {
      எனில் (cgroup2)
        திரும்பு crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  எனில் (memory->kernel_tcp_present)
    {
      எனில் (cgroup2)
        திரும்பு crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  எனில் (memory->swappiness_present)
    {
      எனில் (cgroup2)
        திரும்பு crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64, memory->swappiness);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

முழு
write_cpu_idle (முழு cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  எழுத்து fmt_buf[32];
  முழு len;

  எனில் (! cpu->idle_present)
    திரும்பு 0;

  len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIi64, cpu->idle);
  எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
  திரும்பு write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

முழு
write_cpu_burst (முழு cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  எழுத்து fmt_buf[32];
  முழு len;

  எனில் (! cpu->burst_present)
    திரும்பு 0;

  len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIi64, cpu->burst);
  எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
  திரும்பு write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

நிலையான முழு
write_pids_resources (முழு dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  எனில் (pids->limit)
    {
      எழுத்து fmt_buf[32];
      முழு len;
      முழு ret;

      len = cg_itoa (fmt_buf, அளவு (fmt_buf), pids->limit, true, err);
      எனில் (UNLIKELY (len < 0))
        திரும்பு len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", வெற்று, fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

நிலையான முழு
write_cpu_resources (முழு dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  முழு len, period_len = 0;
  முழு ret;
  எழுத்து fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free எழுத்து *period_str = வெற்று;

  எனில் (cpu->shares)
    {
      uint32_t val = cpu->shares;

      எனில் (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          எனில் (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              எனில் (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, அளவு (fmt_buf), "%u", val);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 வெற்று, fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  எனில் (cpu->period)
    {
      எனில் (cgroup2)
        period = cpu->period;
      இல்லையெனில்
        {
          len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64, cpu->period);
          எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
            திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          எனில் (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              எனில் (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                திரும்பு ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  எனில் (cpu->quota)
    {
      எனில் (cgroup2)
        quota = cpu->quota;
      இல்லையெனில்
        {
          len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIi64, cpu->quota);
          எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
            திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          எனில் (period_str != வெற்று)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;
            }
        }
    }
  எனில் (cpu->realtime_period)
    {
      எனில் (cgroup2)
        திரும்பு crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64, cpu->realtime_period);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  எனில் (cpu->realtime_runtime)
    {
      எனில் (cgroup2)
        திரும்பு crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (cgroup2 && (quota > 0 || period > 0))
    {
      எனில் (period < 0)
        period = 100000;
      எனில் (quota < 0)
        len = snprintf (fmt_buf, அளவு (fmt_buf), "max %" PRIi64, period);
      இல்லையெனில்
        len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", வெற்று, fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

முழு
write_cpuset_resources (முழு dirfd_cpuset, முழு cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  முழு ret;

  எனில் (cpu == வெற்று)
    திரும்பு 0;

  எனில் (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  எனில் (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு 0;
}

நிலையான முழு
open_cgroup_subsystem (மாறிலி எழுத்து *subsystem, மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *full_path = வெற்று;
  முழு ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_வேர், subsystem, path, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (dirfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", full_path);

  திரும்பு dirfd;
}

முழு
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  முழு ret;

  எனில் (resources->block_io)
    {
      cleanup_close முழு dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      எனில் (UNLIKELY (dirfd_blkio < 0))
        திரும்பு dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (resources->network)
    {
      cleanup_close முழு dirfd_netclass = -1;
      cleanup_close முழு dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      எனில் (UNLIKELY (dirfd_netclass < 0))
        திரும்பு dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      எனில் (UNLIKELY (dirfd_netprio < 0))
        திரும்பு dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (resources->hugepage_limits_len)
    {
      cleanup_close முழு dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      எனில் (UNLIKELY (dirfd_htlb < 0))
        திரும்பு dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (resources->devices_len)
    {
      cleanup_close முழு dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      எனில் (UNLIKELY (dirfd_devs < 0))
        திரும்பு dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (resources->memory)
    {
      cleanup_close முழு dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      எனில் (UNLIKELY (dirfd_mem < 0))
        திரும்பு dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (resources->pids)
    {
      cleanup_close முழு dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      எனில் (UNLIKELY (dirfd_pid < 0))
        திரும்பு dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (resources->cpu)
    {
      cleanup_close முழு dirfd_cpu = -1;
      cleanup_close முழு dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      எனில் (UNLIKELY (dirfd_cpu < 0))
        திரும்பு dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      எனில் (resources->cpu->cpus == வெற்று && resources->cpu->mems == வெற்று)
        திரும்பு 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      எனில் (UNLIKELY (dirfd_cpuset < 0))
        திரும்பு dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (resources->unified && resources->unified->len > 0)
    திரும்பு crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  திரும்பு 0;
}

நிலையான முழு
write_unified_resources (முழு cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  முழு ret;

  க்கு (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close முழு fd = -1;
      cleanup_free எழுத்து *மதிப்பு = வெற்று;
      எழுத்து *saveptr = வெற்று;
      எழுத்து *வரி;

      எனில் (strchr (resources->unified->சாவிகள்[i], '/'))
        திரும்பு crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->சாவிகள்[i]);

      எனில் (is_empty_string (resources->unified->மதிப்புகள்[i]))
        தொடர்;

      மதிப்பு = xstrdup (resources->unified->மதிப்புகள்[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->சாவிகள்[i], O_WRONLY, err);
      எனில் (UNLIKELY (fd < 0))
        திரும்பு fd;

      க்கு (வரி = strtok_r (மதிப்பு, "\n", &saveptr); வரி; வரி = strtok_r (வெற்று, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (லிக (fd, வரி, strlen (வரி)));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "write to `%s`", resources->unified->சாவிகள்[i]);
        }
    }

  திரும்பு 0;
}

நிலையான முழு
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, மாறிலி எழுத்து *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cgroup_path = வெற்று;
  cleanup_close முழு cgroup_dirfd = -1;
  முழு ret;

  எனில் (resources->network)
    திரும்பு crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_வேர், path, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (cgroup_dirfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", cgroup_path);

  எனில் (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  எனில் (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  எனில் (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  எனில் (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  எனில் (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
முழு
check_memory_before_update (மாறிலி எழுத்து *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free எழுத்து *swap_current = வெற்று;
  cleanup_free எழுத்து *cgroup_path = வெற்று;
  cleanup_free எழுத்து *current = வெற்று;
  cleanup_close முழு dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  முழு cgroup_mode;
  முழு ret;

  எனில் (path == வெற்று || memory == வெற்று || ! (memory->check_before_update_present && memory->check_before_update))
    திரும்பு 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  எனில் (cgroup_mode != CGROUP_MODE_UNIFIED)
    திரும்பு 0;

  ret = append_paths (&cgroup_path, err, CGROUP_வேர், path, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (dirfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  errno = 0;
  val = strtoll (current, வெற்று, 10);
  எனில் (UNLIKELY (errno))
    திரும்பு crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, வெற்று, 10);
  எனில் (UNLIKELY (errno))
    திரும்பு crun_make_error (err, errno, "parse memory.swap.current");

  எனில் (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  எனில் (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  எனில் (limit <= val + val_swap)
    திரும்பு crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  திரும்பு 0;
}

முழு
update_cgroup_resources (மாறிலி எழுத்து *path,
                         மாறிலி எழுத்து *state_வேர்,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  முழு cgroup_mode;

  (வெறுமை) state_வேர்;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  எனில் (path == வெற்று)
    {
      size_t i;

      எனில் (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        திரும்பு crun_make_error (err, 0, "cannot set limits without cgroups");

      க்கு (i = 0; i < resources->devices_len; i++)
        {
          முழு rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          எனில் (UNLIKELY (rwm < 0))
            திரும்பு rwm;

          எனில் (rwm == 0)
            திரும்பு crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      திரும்பு 0;
    }
  தேர்வு (cgroup_mode)
    {
    நிலை CGROUP_MODE_UNIFIED:
      திரும்பு update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    நிலை CGROUP_MODE_LEGACY:
    நிலை CGROUP_MODE_HYBRID:
      திரும்பு update_cgroup_v1_resources (resources, path, err);

    இயல்பு:
      திரும்பு crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
