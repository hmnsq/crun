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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "cgroup.h"
#અન્તર્ભાવ "cgroup-internal.h"
#અન્તર્ભાવ "cgroup-systemd.h"
#અન્તર્ભાવ "cgroup-utils.h"
#અન્તર્ભાવ "cgroup-resources.h"
#અન્તર્ભાવ "ebpf.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "status.h"
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/vfs.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <libgen.h>

સંરચના default_dev_s default_devices[] = {
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
  { 0, 0, 0, ખાલી }
};

સંરચના default_dev_s *
get_default_devices ()
{
  પાછા default_devices;
}

સ્થિર પૂર્ણાંક
write_cgroup_file_or_alias (પૂર્ણાંક dirfd, અચલ અક્ષર *name, અચલ અક્ષર *alias, અચલ શૂન્ય *data, size_t len, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  જો (UNLIKELY (alias != ખાલી && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  પાછા ret;
}

સ્થિર અંતઃરેખ પૂર્ણાંક
openat_with_alias (પૂર્ણાંક dirfd, અચલ અક્ષર *name, અચલ અક્ષર *alias, અચલ અક્ષર **used_name, પૂર્ણાંક flags, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  જો (UNLIKELY (ret < 0 && alias != ખાલી && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "open `%s`", name);
  પાછા ret;
}

સ્થિર પૂર્ણાંક
is_rwm (અચલ અક્ષર *str, libcrun_error_t *err)
{
  અચલ અક્ષર *it;
  bool r = false;
  bool w = false;
  bool m = false;

  જો (str == ખાલી)
    str = "";

  માટે (it = str; *it; it++)
    પસંદગી (*it)
      {
      સ્થિતિ 'r':
        r = true;
        તોડો;

      સ્થિતિ 'w':
        w = true;
        તોડો;

      સ્થિતિ 'm':
        m = true;
        તોડો;

      મૂળભૂત:
        પાછા crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  પાછા r && w && m ? 1 : 0;
}

સ્થિર પૂર્ણાંક
check_cgroup_v2_controller_available_wrapper (પૂર્ણાંક ret, પૂર્ણાંક cgroup_dirfd, અચલ અક્ષર *name, libcrun_error_t *err)
{
  જો (ret == 0 || err == ખાલી)
    પાછા 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  જો (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free અક્ષર *controllers = ખાલી;
      libcrun_error_t tmp_err = ખાલી;
      cleanup_free અક્ષર *ચાવી = ખાલી;
      અક્ષર *saveptr = ખાલી;
      bool found = false;
      અચલ અક્ષર *token;
      અક્ષર *it;

      /* Check if the specified controller is enabled.  */
      ચાવી = xstrdup (name);

      it = strchr (ચાવી, '.');
      જો (it == ખાલી)
        {
          crun_error_release (err);
          પાછા crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      જો (strcmp (ચાવી, "cgroup") == 0)
        પાછા ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      જો (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, ખાલી, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          પાછા ret;
        }
      માટે (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (ખાલી, " \n", &saveptr))
        {
          જો (strcmp (token, ચાવી) == 0)
            {
              found = true;
              તોડો;
            }
        }
      જો (! found)
        {
          cleanup_free અક્ષર *absolute_path = ખાલી;
          libcrun_error_t tmp_err = ખાલી;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          જો (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", ચાવી, absolute_path);
          નહીંતર
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", ચાવી);
            }
        }
    }
  પાછા ret;
}

સ્થિર પૂર્ણાંક
write_file_and_check_controllers_at (bool cgroup2, પૂર્ણાંક dirfd, અચલ અક્ષર *name, અચલ અક્ષર *name_alias,
                                     અચલ શૂન્ય *data, size_t len, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  જો (cgroup2)
    પાછા check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  પાછા ret;
}

સ્થિર પૂર્ણાંક
open_file_and_check_controllers_at (bool cgroup2, પૂર્ણાંક dirfd, અચલ અક્ષર *name, પૂર્ણાંક flags, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = openat (dirfd, name, flags);
  જો (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      જો (cgroup2)
        પાછા check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  પાછા ret;
}

/* The parser generates different structs but they are really all the same.  */
પ્રકારવ્યાખ્યા runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

સ્થિર પૂર્ણાંક
write_blkio_v1_resources_throttling (પૂર્ણાંક dirfd, અચલ અક્ષર *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  અક્ષર fmt_buf[128];
  size_t i;
  cleanup_close પૂર્ણાંક fd = -1;

  જો (throttling == ખાલી)
    પાછા 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", name);

  માટે (i = 0; i < throttling_len; i++)
    {
      પૂર્ણાંક ret;
      પૂર્ણાંક len;
      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (લિખ (fd, fmt_buf, len));
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "write `%s`", name);
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
write_blkio_v2_resources_throttling (પૂર્ણાંક fd, અચલ અક્ષર *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  અક્ષર fmt_buf[128];
  size_t i;

  જો (throttling == ખાલી)
    પાછા 0;

  માટે (i = 0; i < throttling_len; i++)
    {
      પૂર્ણાંક ret;
      પૂર્ણાંક len;
      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (લિખ (fd, fmt_buf, len));
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "write `%s`", name);
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
write_blkio_resources (પૂર્ણાંક dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  અક્ષર fmt_buf[128];
  પૂર્ણાંક len;
  પૂર્ણાંક ret;

  જો (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu32, val);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");
      જો (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
      નહીંતર
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          જો (UNLIKELY (ret < 0))
            {
              જો (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu32, val);
                  જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
                    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              જો (UNLIKELY (ret < 0))
                પાછા ret;
            }
        }
    }
  જો (blkio->leaf_weight)
    {
      જો (cgroup2)
        પાછા crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, કદ (fmt_buf), "%d", blkio->leaf_weight);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  જો (blkio->weight_device_len)
    {
      જો (cgroup2)
        {
          cleanup_close પૂર્ણાંક wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          જો (UNLIKELY (wfd < 0))
            પાછા crun_make_error (err, errno, "open `io.bfq.weight`");
          માટે (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
                પાછા crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (લિખ (wfd, fmt_buf, len));
              જો (UNLIKELY (ret < 0))
                પાછા crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      નહીંતર
        {
          અચલ અક્ષર *leaf_weight_device_file_name = ખાલી;
          અચલ અક્ષર *weight_device_file_name = ખાલી;
          cleanup_close પૂર્ણાંક w_leafdevice_fd = -1;
          cleanup_close પૂર્ણાંક w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          જો (UNLIKELY (w_device_fd < 0))
            પાછા w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          જો (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          માટે (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
                પાછા crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (લિખ (w_device_fd, fmt_buf, len));
              જો (UNLIKELY (ret < 0))
                પાછા crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              જો (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
                    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (લિખ (w_leafdevice_fd, fmt_buf, len));
                  જો (UNLIKELY (ret < 0))
                    પાછા crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  જો (cgroup2)
    {
      cleanup_close પૂર્ણાંક wfd = -1;
      અચલ અક્ષર *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      જો (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          પાછા check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
write_network_resources (પૂર્ણાંક dirfd_netclass, પૂર્ણાંક dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  અક્ષર fmt_buf[128];
  પૂર્ણાંક len;
  પૂર્ણાંક ret;
  જો (net->class_id)
    {
      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu32, net->class_id);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  જો (net->priorities_len)
    {
      size_t i;
      cleanup_close પૂર્ણાંક fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      જો (UNLIKELY (fd < 0))
        પાછા crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      માટે (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, કદ (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
            પાછા crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (લિખ (fd, fmt_buf, len));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
write_hugetlb_resources (પૂર્ણાંક dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  અચલ અક્ષર *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  અક્ષર fmt_buf[128];
  size_t i;
  માટે (i = 0; i < htlb_len; i++)
    {
      cleanup_free અક્ષર *filename = ખાલી;
      cleanup_free અક્ષર *rsvd_filename = ખાલી;
      પૂર્ણાંક len;
      પૂર્ણાંક ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64, htlb[i]->limit);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, ખાલી, fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      જો (skip_rsvd)
        ચાલુ;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        {
          જો (crun_error_get_errno (err) != ENOENT)
            પાછા ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
write_devices_resources_v1 (પૂર્ણાંક dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  પૂર્ણાંક len;
  પૂર્ણાંક ret;

  માટે (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      અક્ષર fmt_buf[FMT_BUF_LEN];
      અચલ અક્ષર *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      જો (devs[i]->type == ખાલી || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      નહીંતર
        {
          અક્ષર fmt_buf_major[16];
          અક્ષર fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  કરો                                                                                    \
    {                                                                                   \
      જો (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, કદ (b), "%" PRIi64, x);                                \
          જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (b)))                                       \
            પાછા crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      નહીંતર                                                                              \
        strcpy (b, "*");                                                                \
  } જ્યારે (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          જો (UNLIKELY (len >= FMT_BUF_LEN))
            પાછા crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  માટે (i = 0; default_devices[i].type; i++)
    {
      અક્ષર fmt_buf_major[16];
      અક્ષર fmt_buf_minor[16];
      અક્ષર device[64];
      પૂર્ણાંક len;

#define FMT_DEV(x, b)                                                                   \
  કરો                                                                                    \
    {                                                                                   \
      જો (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, કદ (b), "%d", x);                                      \
          જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (b)))                                       \
            પાછા crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      નહીંતર                                                                              \
        strcpy (b, "*");                                                                \
  } જ્યારે (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, કદ (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (device)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

સંરચના bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  પૂર્ણાંક i;
  સંરચના bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  જો (UNLIKELY (program == ખાલી))
    પાછા ખાલી;

  માટે (i = (કદ (default_devices) / કદ (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      જો (UNLIKELY (program == ખાલી))
        પાછા ખાલી;
    }
  માટે (i = devs_len - 1; i >= 0; i--)
    {
      અક્ષર type = 'a';
      પૂર્ણાંક minor = -1, major = -1;
      જો (devs[i]->type != ખાલી)
        type = devs[i]->type[0];

      જો (devs[i]->major_present)
        major = devs[i]->major;
      જો (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      જો (UNLIKELY (program == ખાલી))
        પાછા ખાલી;
    }

  program = bpf_program_complete_dev (program, err);
  જો (UNLIKELY (program == ખાલી))
    પાછા ખાલી;

  પાછા program;
}

સ્થિર પૂર્ણાંક
write_devices_resources_v2_internal (પૂર્ણાંક dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free સંરચના bpf_program *program = ખાલી;

  program = create_dev_bpf (devs, devs_len, err);
  જો (UNLIKELY (program == ખાલી))
    પાછા -1;

  પાછા libcrun_ebpf_load (program, dirfd, ખાલી, err);
}

સ્થિર પૂર્ણાંક
write_devices_resources_v2 (પૂર્ણાંક dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  જો (LIKELY (ret == 0))
    પાછા 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  માટે (i = 0; i < devs_len; i++)
    {
      જો (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          તોડો;
        }
    }

  જો (! can_skip)
    {
      libcrun_error_t tmp_err = ખાલી;
      પૂર્ણાંક rootless;

      rootless = is_rootless (&tmp_err);
      જો (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          પાછા ret;
        }
      જો (rootless)
        can_skip = true;
    }

  જો (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  પાછા ret;
}

સ્થિર પૂર્ણાંક
write_devices_resources (પૂર્ણાંક dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  જો (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  નહીંતર
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  જો (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ખાલી;
      પૂર્ણાંક rootless;

      rootless = is_rootless (&tmp_err);
      જો (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          પાછા ret;
        }

      જો (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  પાછા ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
સ્થિર પૂર્ણાંક
cg_itoa (અક્ષર *buf, size_t size, int64_t કિંમત, bool use_max, libcrun_error_t *err)
{
  પૂર્ણાંક len;

  જો (use_max && કિંમત < 0)
    len = snprintf (buf, size, "max");
  નહીંતર
    len = snprintf (buf, size, "%" PRIi64, કિંમત);

  જો (UNLIKELY (len >= (પૂર્ણાંક) size))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

  પાછા len;
}

સ્થિર પૂર્ણાંક
write_memory (પૂર્ણાંક dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  અક્ષર limit_buf[32];
  પૂર્ણાંક limit_buf_len;

  જો (! memory->limit_present)
    પાછા 0;

  limit_buf_len = cg_itoa (limit_buf, કદ (limit_buf), memory->limit, cgroup2, err);
  જો (UNLIKELY (limit_buf_len < 0))
    પાછા limit_buf_len;

  પાછા write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

સ્થિર પૂર્ણાંક
write_memory_swap (પૂર્ણાંક dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  int64_t swap;
  અક્ષર swap_buf[32];
  પૂર્ણાંક len;
  અચલ અક્ષર *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  જો (! memory->swap_present)
    પાછા 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  જો (cgroup2 && memory->swap > 0)
    {
      જો (! memory->limit_present)
        પાછા crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      જો (memory->swap < memory->limit)
        પાછા crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, કદ (swap_buf), swap, cgroup2, err);
  જો (UNLIKELY (len < 0))
    પાછા len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  જો (ret >= 0)
    પાછા ret;

  /* If swap is not enabled, ignore the error.  */
  જો (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      પાછા 0;
    }

  પાછા ret;
}

સ્થિર પૂર્ણાંક
write_memory_resources (પૂર્ણાંક dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  પૂર્ણાંક len;
  પૂર્ણાંક ret;
  અક્ષર fmt_buf[32];
  bool memory_limits_written = false;

  જો (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      જો (ret >= 0)
        memory_limits_written = true;
      નહીંતર
        {
          જો (cgroup2 || crun_error_get_errno (err) != EINVAL)
            પાછા ret;

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
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (memory->kernel_present)
    {
      જો (cgroup2)
        પાછા crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64, memory->kernel);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  જો (memory->use_hierarchy_present)
    {
      જો (cgroup2)
        પાછા crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (memory->reservation_present)
    {
      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64, memory->reservation);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 ખાલી, fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  જો (memory->disable_oom_killer)
    {
      જો (cgroup2)
        પાછા crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  જો (memory->kernel_tcp_present)
    {
      જો (cgroup2)
        પાછા crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  જો (memory->swappiness_present)
    {
      જો (cgroup2)
        પાછા crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64, memory->swappiness);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

પૂર્ણાંક
write_cpu_idle (પૂર્ણાંક cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  અક્ષર fmt_buf[32];
  પૂર્ણાંક len;

  જો (! cpu->idle_present)
    પાછા 0;

  len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIi64, cpu->idle);
  જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");
  પાછા write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

પૂર્ણાંક
write_cpu_burst (પૂર્ણાંક cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  અક્ષર fmt_buf[32];
  પૂર્ણાંક len;

  જો (! cpu->burst_present)
    પાછા 0;

  len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIi64, cpu->burst);
  જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");
  પાછા write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

સ્થિર પૂર્ણાંક
write_pids_resources (પૂર્ણાંક dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  જો (pids->limit)
    {
      અક્ષર fmt_buf[32];
      પૂર્ણાંક len;
      પૂર્ણાંક ret;

      len = cg_itoa (fmt_buf, કદ (fmt_buf), pids->limit, true, err);
      જો (UNLIKELY (len < 0))
        પાછા len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", ખાલી, fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
write_cpu_resources (પૂર્ણાંક dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  પૂર્ણાંક len, period_len = 0;
  પૂર્ણાંક ret;
  અક્ષર fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free અક્ષર *period_str = ખાલી;

  જો (cpu->shares)
    {
      uint32_t val = cpu->shares;

      જો (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          જો (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              જો (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, કદ (fmt_buf), "%u", val);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 ખાલી, fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  જો (cpu->period)
    {
      જો (cgroup2)
        period = cpu->period;
      નહીંતર
        {
          len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64, cpu->period);
          જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
            પાછા crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          જો (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              જો (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                પાછા ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  જો (cpu->quota)
    {
      જો (cgroup2)
        quota = cpu->quota;
      નહીંતર
        {
          len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIi64, cpu->quota);
          જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
            પાછા crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          જો (period_str != ખાલી)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              જો (UNLIKELY (ret < 0))
                પાછા ret;
            }
        }
    }
  જો (cpu->realtime_period)
    {
      જો (cgroup2)
        પાછા crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64, cpu->realtime_period);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  જો (cpu->realtime_runtime)
    {
      જો (cgroup2)
        પાછા crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (cgroup2 && (quota > 0 || period > 0))
    {
      જો (period < 0)
        period = 100000;
      જો (quota < 0)
        len = snprintf (fmt_buf, કદ (fmt_buf), "max %" PRIi64, period);
      નહીંતર
        len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", ખાલી, fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

પૂર્ણાંક
write_cpuset_resources (પૂર્ણાંક dirfd_cpuset, પૂર્ણાંક cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  જો (cpu == ખાલી)
    પાછા 0;

  જો (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  જો (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
open_cgroup_subsystem (અચલ અક્ષર *subsystem, અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *full_path = ખાલી;
  પૂર્ણાંક ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_મૂળ, subsystem, path, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (dirfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", full_path);

  પાછા dirfd;
}

પૂર્ણાંક
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, અચલ અક્ષર *path, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  જો (resources->block_io)
    {
      cleanup_close પૂર્ણાંક dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      જો (UNLIKELY (dirfd_blkio < 0))
        પાછા dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (resources->network)
    {
      cleanup_close પૂર્ણાંક dirfd_netclass = -1;
      cleanup_close પૂર્ણાંક dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      જો (UNLIKELY (dirfd_netclass < 0))
        પાછા dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      જો (UNLIKELY (dirfd_netprio < 0))
        પાછા dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (resources->hugepage_limits_len)
    {
      cleanup_close પૂર્ણાંક dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      જો (UNLIKELY (dirfd_htlb < 0))
        પાછા dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (resources->devices_len)
    {
      cleanup_close પૂર્ણાંક dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      જો (UNLIKELY (dirfd_devs < 0))
        પાછા dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (resources->memory)
    {
      cleanup_close પૂર્ણાંક dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      જો (UNLIKELY (dirfd_mem < 0))
        પાછા dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (resources->pids)
    {
      cleanup_close પૂર્ણાંક dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      જો (UNLIKELY (dirfd_pid < 0))
        પાછા dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (resources->cpu)
    {
      cleanup_close પૂર્ણાંક dirfd_cpu = -1;
      cleanup_close પૂર્ણાંક dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      જો (UNLIKELY (dirfd_cpu < 0))
        પાછા dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      જો (resources->cpu->cpus == ખાલી && resources->cpu->mems == ખાલી)
        પાછા 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      જો (UNLIKELY (dirfd_cpuset < 0))
        પાછા dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (resources->unified && resources->unified->len > 0)
    પાછા crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  પાછા 0;
}

સ્થિર પૂર્ણાંક
write_unified_resources (પૂર્ણાંક cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  પૂર્ણાંક ret;

  માટે (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close પૂર્ણાંક fd = -1;
      cleanup_free અક્ષર *કિંમત = ખાલી;
      અક્ષર *saveptr = ખાલી;
      અક્ષર *લીટી;

      જો (strchr (resources->unified->ચાવીઓ[i], '/'))
        પાછા crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->ચાવીઓ[i]);

      જો (is_empty_string (resources->unified->કિંમતો[i]))
        ચાલુ;

      કિંમત = xstrdup (resources->unified->કિંમતો[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->ચાવીઓ[i], O_WRONLY, err);
      જો (UNLIKELY (fd < 0))
        પાછા fd;

      માટે (લીટી = strtok_r (કિંમત, "\n", &saveptr); લીટી; લીટી = strtok_r (ખાલી, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (લિખ (fd, લીટી, strlen (લીટી)));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "write to `%s`", resources->unified->ચાવીઓ[i]);
        }
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, અચલ અક્ષર *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cgroup_path = ખાલી;
  cleanup_close પૂર્ણાંક cgroup_dirfd = -1;
  પૂર્ણાંક ret;

  જો (resources->network)
    પાછા crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, path, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (cgroup_dirfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", cgroup_path);

  જો (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  જો (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  જો (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  જો (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  જો (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
પૂર્ણાંક
check_memory_before_update (અચલ અક્ષર *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free અક્ષર *swap_current = ખાલી;
  cleanup_free અક્ષર *cgroup_path = ખાલી;
  cleanup_free અક્ષર *current = ખાલી;
  cleanup_close પૂર્ણાંક dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક ret;

  જો (path == ખાલી || memory == ખાલી || ! (memory->check_before_update_present && memory->check_before_update))
    પાછા 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  જો (cgroup_mode != CGROUP_MODE_UNIFIED)
    પાછા 0;

  ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, path, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (dirfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  errno = 0;
  val = strtoll (current, ખાલી, 10);
  જો (UNLIKELY (errno))
    પાછા crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, ખાલી, 10);
  જો (UNLIKELY (errno))
    પાછા crun_make_error (err, errno, "parse memory.swap.current");

  જો (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  જો (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  જો (limit <= val + val_swap)
    પાછા crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  પાછા 0;
}

પૂર્ણાંક
update_cgroup_resources (અચલ અક્ષર *path,
                         અચલ અક્ષર *state_મૂળ,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  પૂર્ણાંક cgroup_mode;

  (શૂન્ય) state_મૂળ;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  જો (path == ખાલી)
    {
      size_t i;

      જો (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        પાછા crun_make_error (err, 0, "cannot set limits without cgroups");

      માટે (i = 0; i < resources->devices_len; i++)
        {
          પૂર્ણાંક rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          જો (UNLIKELY (rwm < 0))
            પાછા rwm;

          જો (rwm == 0)
            પાછા crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      પાછા 0;
    }
  પસંદગી (cgroup_mode)
    {
    સ્થિતિ CGROUP_MODE_UNIFIED:
      પાછા update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    સ્થિતિ CGROUP_MODE_LEGACY:
    સ્થિતિ CGROUP_MODE_HYBRID:
      પાછા update_cgroup_v1_resources (resources, path, err);

    મૂળભૂત:
      પાછા crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
