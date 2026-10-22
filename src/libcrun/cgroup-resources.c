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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "cgroup.h"
#అన్తర్భావ "cgroup-internal.h"
#అన్తర్భావ "cgroup-systemd.h"
#అన్తర్భావ "cgroup-utils.h"
#అన్తర్భావ "cgroup-resources.h"
#అన్తర్భావ "ebpf.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "status.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/vfs.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ <time.h>

#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <libgen.h>

నిర్మాణం default_dev_s default_devices[] = {
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
  { 0, 0, 0, ఖాళీ }
};

నిర్మాణం default_dev_s *
get_default_devices ()
{
  తిరిగి default_devices;
}

స్థిర పూర్ణాంకం
write_cgroup_file_or_alias (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *alias, స్థిరరాశి శూన్య *దత్తాంశం, size_t len, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = write_cgroup_file (dirfd, name, దత్తాంశం, len, err);
  ఒకవేళ (UNLIKELY (alias != ఖాళీ && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, దత్తాంశం, len, err);
    }
  తిరిగి ret;
}

స్థిర అంతఃవరుస పూర్ణాంకం
openat_with_alias (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *alias, స్థిరరాశి అక్షరం **వాడిన_name, పూర్ణాంకం flags, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  *వాడిన_name = name;

  ret = openat (dirfd, name, flags);
  ఒకవేళ (UNLIKELY (ret < 0 && alias != ఖాళీ && errno == ENOENT))
    {
      *వాడిన_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", name);
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
is_rwm (స్థిరరాశి అక్షరం *str, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *it;
  bool r = false;
  bool w = false;
  bool m = false;

  ఒకవేళ (str == ఖాళీ)
    str = "";

  కొరకు (it = str; *it; it++)
    ఎంపిక (*it)
      {
      స్థితి 'r':
        r = true;
        ఆపు;

      స్థితి 'w':
        w = true;
        ఆపు;

      స్థితి 'm':
        m = true;
        ఆపు;

      అప్రమేయ:
        తిరిగి crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  తిరిగి r && w && m ? 1 : 0;
}

స్థిర పూర్ణాంకం
check_cgroup_v2_controller_అందుబాటు_wrapper (పూర్ణాంకం ret, పూర్ణాంకం cgroup_dirfd, స్థిరరాశి అక్షరం *name, libcrun_error_t *err)
{
  ఒకవేళ (ret == 0 || err == ఖాళీ)
    తిరిగి 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  ఒకవేళ (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free అక్షరం *controllers = ఖాళీ;
      libcrun_error_t tmp_err = ఖాళీ;
      cleanup_free అక్షరం *కీలకం = ఖాళీ;
      అక్షరం *saveptr = ఖాళీ;
      bool దొరికింది = false;
      స్థిరరాశి అక్షరం *token;
      అక్షరం *it;

      /* Check if the specified controller is enabled.  */
      కీలకం = xstrdup (name);

      it = strchr (కీలకం, '.');
      ఒకవేళ (it == ఖాళీ)
        {
          crun_error_release (err);
          తిరిగి crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      ఒకవేళ (strcmp (కీలకం, "cgroup") == 0)
        తిరిగి ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      ఒకవేళ (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, ఖాళీ, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          తిరిగి ret;
        }
      కొరకు (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (ఖాళీ, " \n", &saveptr))
        {
          ఒకవేళ (strcmp (token, కీలకం) == 0)
            {
              దొరికింది = true;
              ఆపు;
            }
        }
      ఒకవేళ (! దొరికింది)
        {
          cleanup_free అక్షరం *absolute_మార్గం = ఖాళీ;
          libcrun_error_t tmp_err = ఖాళీ;

          crun_error_release (err);
          ret = get_realpath_గమ్యం_file (cgroup_dirfd, "cgroup.controllers", &absolute_మార్గం, &tmp_err);
          ఒకవేళ (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", కీలకం, absolute_మార్గం);
          లేకపోతే
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", కీలకం);
            }
        }
    }
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
write_file_and_check_controllers_at (bool cgroup2, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *name_alias,
                                     స్థిరరాశి శూన్య *దత్తాంశం, size_t len, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, దత్తాంశం, len, err);
  ఒకవేళ (cgroup2)
    తిరిగి check_cgroup_v2_controller_అందుబాటు_wrapper (ret, dirfd, name, err);
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
open_file_and_check_controllers_at (bool cgroup2, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = openat (dirfd, name, flags);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      ఒకవేళ (cgroup2)
        తిరిగి check_cgroup_v2_controller_అందుబాటు_wrapper (ret, dirfd, name, err);
    }
  తిరిగి ret;
}

/* The parser generates different structs but they are really all the same.  */
రకనిర్వచనం runtime_spec_schema_defs_linux_ఖండం_io_device_throttle throttling_s;

స్థిర పూర్ణాంకం
write_blkio_v1_resources_throttling (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  అక్షరం fmt_buf[128];
  size_t i;
  cleanup_close పూర్ణాంకం fd = -1;

  ఒకవేళ (throttling == ఖాళీ)
    తిరిగి 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", name);

  కొరకు (i = 0; i < throttling_len; i++)
    {
      పూర్ణాంకం ret;
      పూర్ణాంకం len;
      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->రేటు);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, fmt_buf, len));
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "write `%s`", name);
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
write_blkio_v2_resources_throttling (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  అక్షరం fmt_buf[128];
  size_t i;

  ఒకవేళ (throttling == ఖాళీ)
    తిరిగి 0;

  కొరకు (i = 0; i < throttling_len; i++)
    {
      పూర్ణాంకం ret;
      పూర్ణాంకం len;
      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->రేటు);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, fmt_buf, len));
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "write `%s`", name);
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
write_blkio_resources (పూర్ణాంకం dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_ఖండం_io *blkio,
                       libcrun_error_t *err)
{
  అక్షరం fmt_buf[128];
  పూర్ణాంకం len;
  పూర్ణాంకం ret;

  ఒకవేళ (blkio->బరువు)
    {
      uint32_t val = blkio->బరువు;

      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu32, val);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
      ఒకవేళ (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
      లేకపోతే
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ఒకవేళ (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu32, val);
                  ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
                    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;
            }
        }
    }
  ఒకవేళ (blkio->ఆకు_బరువు)
    {
      ఒకవేళ (cgroup2)
        తిరిగి crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%d", blkio->ఆకు_బరువు);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ఒకవేళ (blkio->బరువు_device_len)
    {
      ఒకవేళ (cgroup2)
        {
          cleanup_close పూర్ణాంకం wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          ఒకవేళ (UNLIKELY (wfd < 0))
            తిరిగి crun_make_error (err, errno, "open `io.bfq.weight`");
          కొరకు (i = 0; i < blkio->బరువు_device_len; i++)
            {
              uint32_t w = blkio->బరువు_device[i]->బరువు;

              len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->బరువు_device[i]->major,
                              blkio->బరువు_device[i]->minor, w);
              ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
                తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

              ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (wfd, fmt_buf, len));
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      లేకపోతే
        {
          స్థిరరాశి అక్షరం *ఆకు_బరువు_device_file_name = ఖాళీ;
          స్థిరరాశి అక్షరం *బరువు_device_file_name = ఖాళీ;
          cleanup_close పూర్ణాంకం w_leafdevice_fd = -1;
          cleanup_close పూర్ణాంకం w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &బరువు_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ఒకవేళ (UNLIKELY (w_device_fd < 0))
            తిరిగి w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &ఆకు_బరువు_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ఒకవేళ (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          కొరకు (i = 0; i < blkio->బరువు_device_len; i++)
            {
              len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->బరువు_device[i]->major,
                              blkio->బరువు_device[i]->minor, blkio->బరువు_device[i]->బరువు);
              ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
                తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

              ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (w_device_fd, fmt_buf, len));
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి crun_make_error (err, errno, "write `%s`", బరువు_device_file_name);

              ఒకవేళ (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->బరువు_device[i]->major,
                                  blkio->బరువు_device[i]->minor, blkio->బరువు_device[i]->ఆకు_బరువు);
                  ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
                    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (w_leafdevice_fd, fmt_buf, len));
                  ఒకవేళ (UNLIKELY (ret < 0))
                    తిరిగి crun_make_error (err, errno, "write `%s`", ఆకు_బరువు_device_file_name);
                }
            }
        }
    }
  ఒకవేళ (cgroup2)
    {
      cleanup_close పూర్ణాంకం wfd = -1;
      స్థిరరాశి అక్షరం *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          తిరిగి check_cgroup_v2_controller_అందుబాటు_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
write_జాలం_resources (పూర్ణాంకం dirfd_netclass, పూర్ణాంకం dirfd_netprio, runtime_spec_schema_config_linux_resources_జాలం *net,
                         libcrun_error_t *err)
{
  అక్షరం fmt_buf[128];
  పూర్ణాంకం len;
  పూర్ణాంకం ret;
  ఒకవేళ (net->class_id)
    {
      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu32, net->class_id);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ఒకవేళ (net->priorities_len)
    {
      size_t i;
      cleanup_close పూర్ణాంకం fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (fd < 0))
        తిరిగి crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      కొరకు (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->ప్రాధాన్యత);
          ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
            తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

          ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, fmt_buf, len));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
write_hugetlb_resources (పూర్ణాంకం dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  అక్షరం fmt_buf[128];
  size_t i;
  కొరకు (i = 0; i < htlb_len; i++)
    {
      cleanup_free అక్షరం *filename = ఖాళీ;
      cleanup_free అక్షరం *rsvd_filename = ఖాళీ;
      పూర్ణాంకం len;
      పూర్ణాంకం ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->పుట_కొలత, suffix);

      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64, htlb[i]->హద్దు);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, ఖాళీ, fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (skip_rsvd)
        కొనసాగు;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->పుట_కొలత, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (crun_error_get_errno (err) != ENOENT)
            తిరిగి ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
write_devices_resources_v1 (పూర్ణాంకం dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  పూర్ణాంకం len;
  పూర్ణాంకం ret;

  కొరకు (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#నిర్వచించు FMT_BUF_LEN 64
      అక్షరం fmt_buf[FMT_BUF_LEN];
      స్థిరరాశి అక్షరం *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      ఒకవేళ (devs[i]->type == ఖాళీ || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      లేకపోతే
        {
          అక్షరం fmt_buf_major[16];
          అక్షరం fmt_buf_minor[16];

#నిర్వచించు FMT_DEV(x, b)                                                                   \
  చేయి                                                                                    \
    {                                                                                   \
      ఒకవేళ (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, పరిమాణం (b), "%" PRIi64, x);                                \
          ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (b)))                                       \
            తిరిగి crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      లేకపోతే                                                                              \
        strcpy (b, "*");                                                                \
  } అయితే (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#నిర్వచనం_రద్దు FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          ఒకవేళ (UNLIKELY (len >= FMT_BUF_LEN))
            తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  కొరకు (i = 0; default_devices[i].type; i++)
    {
      అక్షరం fmt_buf_major[16];
      అక్షరం fmt_buf_minor[16];
      అక్షరం device[64];
      పూర్ణాంకం len;

#నిర్వచించు FMT_DEV(x, b)                                                                   \
  చేయి                                                                                    \
    {                                                                                   \
      ఒకవేళ (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, పరిమాణం (b), "%d", x);                                      \
          ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (b)))                                       \
            తిరిగి crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      లేకపోతే                                                                              \
        strcpy (b, "*");                                                                \
  } అయితే (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#నిర్వచనం_రద్దు FMT_DEV

      len = snprintf (device, పరిమాణం (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (device)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, అక్షరశ్రేణి_పొడవు (device), err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

నిర్మాణం bpf_కార్యక్రమం *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  పూర్ణాంకం i;
  నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం;

  కార్యక్రమం = bpf_కార్యక్రమం_new (2048);

  కార్యక్రమం = bpf_కార్యక్రమం_init_dev (కార్యక్రమం, err);
  ఒకవేళ (UNLIKELY (కార్యక్రమం == ఖాళీ))
    తిరిగి ఖాళీ;

  కొరకు (i = (పరిమాణం (default_devices) / పరిమాణం (default_devices[0])) - 1; i >= 0; i--)
    {
      కార్యక్రమం = bpf_కార్యక్రమం_append_dev (కార్యక్రమం, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      ఒకవేళ (UNLIKELY (కార్యక్రమం == ఖాళీ))
        తిరిగి ఖాళీ;
    }
  కొరకు (i = devs_len - 1; i >= 0; i--)
    {
      అక్షరం type = 'a';
      పూర్ణాంకం minor = -1, major = -1;
      ఒకవేళ (devs[i]->type != ఖాళీ)
        type = devs[i]->type[0];

      ఒకవేళ (devs[i]->major_present)
        major = devs[i]->major;
      ఒకవేళ (devs[i]->minor_present)
        minor = devs[i]->minor;

      కార్యక్రమం = bpf_కార్యక్రమం_append_dev (కార్యక్రమం, devs[i]->access, type, major, minor, devs[i]->allow, err);
      ఒకవేళ (UNLIKELY (కార్యక్రమం == ఖాళీ))
        తిరిగి ఖాళీ;
    }

  కార్యక్రమం = bpf_కార్యక్రమం_complete_dev (కార్యక్రమం, err);
  ఒకవేళ (UNLIKELY (కార్యక్రమం == ఖాళీ))
    తిరిగి ఖాళీ;

  తిరిగి కార్యక్రమం;
}

స్థిర పూర్ణాంకం
write_devices_resources_v2_అంతర్గత (పూర్ణాంకం dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం = ఖాళీ;

  కార్యక్రమం = create_dev_bpf (devs, devs_len, err);
  ఒకవేళ (UNLIKELY (కార్యక్రమం == ఖాళీ))
    తిరిగి -1;

  తిరిగి libcrun_ebpf_ఎక్కించు (కార్యక్రమం, dirfd, ఖాళీ, err);
}

స్థిర పూర్ణాంకం
write_devices_resources_v2 (పూర్ణాంకం dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_అంతర్గత (dirfd, devs, devs_len, err);
  ఒకవేళ (LIKELY (ret == 0))
    తిరిగి 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  కొరకు (i = 0; i < devs_len; i++)
    {
      ఒకవేళ (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          ఆపు;
        }
    }

  ఒకవేళ (! can_skip)
    {
      libcrun_error_t tmp_err = ఖాళీ;
      పూర్ణాంకం rootless;

      rootless = is_rootless (&tmp_err);
      ఒకవేళ (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          తిరిగి ret;
        }
      ఒకవేళ (rootless)
        can_skip = true;
    }

  ఒకవేళ (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  తిరిగి ret;
}

స్థిర పూర్ణాంకం
write_devices_resources (పూర్ణాంకం dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ఒకవేళ (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  లేకపోతే
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ఖాళీ;
      పూర్ణాంకం rootless;

      rootless = is_rootless (&tmp_err);
      ఒకవేళ (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          తిరిగి ret;
        }

      ఒకవేళ (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  తిరిగి ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
స్థిర పూర్ణాంకం
cg_itoa (అక్షరం *buf, size_t కొలత, int64_t విలువ, bool use_max, libcrun_error_t *err)
{
  పూర్ణాంకం len;

  ఒకవేళ (use_max && విలువ < 0)
    len = snprintf (buf, కొలత, "max");
  లేకపోతే
    len = snprintf (buf, కొలత, "%" PRIi64, విలువ);

  ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) కొలత))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

  తిరిగి len;
}

స్థిర పూర్ణాంకం
write_జ్ఞాపకం (పూర్ణాంకం dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_జ్ఞాపకం *జ్ఞాపకం, libcrun_error_t *err)
{
  అక్షరం హద్దు_buf[32];
  పూర్ణాంకం హద్దు_buf_len;

  ఒకవేళ (! జ్ఞాపకం->హద్దు_present)
    తిరిగి 0;

  హద్దు_buf_len = cg_itoa (హద్దు_buf, పరిమాణం (హద్దు_buf), జ్ఞాపకం->హద్దు, cgroup2, err);
  ఒకవేళ (UNLIKELY (హద్దు_buf_len < 0))
    తిరిగి హద్దు_buf_len;

  తిరిగి write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", హద్దు_buf, హద్దు_buf_len, err);
}

స్థిర పూర్ణాంకం
write_జ్ఞాపకం_మార్పిడి (పూర్ణాంకం dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_జ్ఞాపకం *జ్ఞాపకం,
                   libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  int64_t మార్పిడి;
  అక్షరం మార్పిడి_buf[32];
  పూర్ణాంకం len;
  స్థిరరాశి అక్షరం *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  ఒకవేళ (! జ్ఞాపకం->మార్పిడి_present)
    తిరిగి 0;

  మార్పిడి = జ్ఞాపకం->మార్పిడి;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  ఒకవేళ (cgroup2 && జ్ఞాపకం->మార్పిడి > 0)
    {
      ఒకవేళ (! జ్ఞాపకం->హద్దు_present)
        తిరిగి crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ఒకవేళ (జ్ఞాపకం->మార్పిడి < జ్ఞాపకం->హద్దు)
        తిరిగి crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      మార్పిడి -= జ్ఞాపకం->హద్దు;
    }

  len = cg_itoa (మార్పిడి_buf, పరిమాణం (మార్పిడి_buf), మార్పిడి, cgroup2, err);
  ఒకవేళ (UNLIKELY (len < 0))
    తిరిగి len;

  ret = write_cgroup_file (dirfd, fname, మార్పిడి_buf, len, err);
  ఒకవేళ (ret >= 0)
    తిరిగి ret;

  /* If swap is not enabled, ignore the error.  */
  ఒకవేళ (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      తిరిగి 0;
    }

  తిరిగి ret;
}

స్థిర పూర్ణాంకం
write_జ్ఞాపకం_resources (పూర్ణాంకం dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_జ్ఞాపకం *జ్ఞాపకం,
                        libcrun_error_t *err)
{
  పూర్ణాంకం len;
  పూర్ణాంకం ret;
  అక్షరం fmt_buf[32];
  bool జ్ఞాపకం_limits_written = false;

  ఒకవేళ (జ్ఞాపకం->హద్దు_present)
    {
      ret = write_జ్ఞాపకం (dirfd, cgroup2, జ్ఞాపకం, err);
      ఒకవేళ (ret >= 0)
        జ్ఞాపకం_limits_written = true;
      లేకపోతే
        {
          ఒకవేళ (cgroup2 || crun_error_get_errno (err) != EINVAL)
            తిరిగి ret;

          /*
            If we get an EINVAL error on cgroup v1 we reverse
            the order we write the memory limit and the swap.
            Attempt to write again the memory limit once the memory
            swap is written.
          */
          crun_error_release (err);
        }
    }

  ret = write_జ్ఞాపకం_మార్పిడి (dirfd, cgroup2, జ్ఞాపకం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (జ్ఞాపకం->హద్దు_present && ! జ్ఞాపకం_limits_written)
    {
      ret = write_జ్ఞాపకం (dirfd, cgroup2, జ్ఞాపకం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (జ్ఞాపకం->kernel_present)
    {
      ఒకవేళ (cgroup2)
        తిరిగి crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64, జ్ఞాపకం->kernel);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  ఒకవేళ (జ్ఞాపకం->use_hierarchy_present)
    {
      ఒకవేళ (cgroup2)
        తిరిగి crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (జ్ఞాపకం->use_hierarchy) ? "1" : "0", 1, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (జ్ఞాపకం->reservation_present)
    {
      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64, జ్ఞాపకం->reservation);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 ఖాళీ, fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ఒకవేళ (జ్ఞాపకం->disable_oom_killer)
    {
      ఒకవేళ (cgroup2)
        తిరిగి crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ఒకవేళ (జ్ఞాపకం->kernel_tcp_present)
    {
      ఒకవేళ (cgroup2)
        తిరిగి crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64, జ్ఞాపకం->kernel_tcp);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ఒకవేళ (జ్ఞాపకం->swappiness_present)
    {
      ఒకవేళ (cgroup2)
        తిరిగి crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64, జ్ఞాపకం->swappiness);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

పూర్ణాంకం
write_cpu_idle (పూర్ణాంకం cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  అక్షరం fmt_buf[32];
  పూర్ణాంకం len;

  ఒకవేళ (! cpu->idle_present)
    తిరిగి 0;

  len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIi64, cpu->idle);
  ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
  తిరిగి write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

పూర్ణాంకం
write_cpu_వ్యవధి (పూర్ణాంకం cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  అక్షరం fmt_buf[32];
  పూర్ణాంకం len;

  ఒకవేళ (! cpu->వ్యవధి_present)
    తిరిగి 0;

  len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIi64, cpu->వ్యవధి);
  ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
  తిరిగి write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

స్థిర పూర్ణాంకం
write_pids_resources (పూర్ణాంకం dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  ఒకవేళ (pids->హద్దు)
    {
      అక్షరం fmt_buf[32];
      పూర్ణాంకం len;
      పూర్ణాంకం ret;

      len = cg_itoa (fmt_buf, పరిమాణం (fmt_buf), pids->హద్దు, true, err);
      ఒకవేళ (UNLIKELY (len < 0))
        తిరిగి len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", ఖాళీ, fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
write_cpu_resources (పూర్ణాంకం dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  పూర్ణాంకం len, period_len = 0;
  పూర్ణాంకం ret;
  అక్షరం fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free అక్షరం *period_str = ఖాళీ;

  ఒకవేళ (cpu->shares)
    {
      uint32_t val = cpu->shares;

      ఒకవేళ (cgroup2)
        {
          val = convert_shares_గమ్యం_బరువు (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          ఒకవేళ (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%u", val);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 ఖాళీ, fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ఒకవేళ (cpu->period)
    {
      ఒకవేళ (cgroup2)
        period = cpu->period;
      లేకపోతే
        {
          len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64, cpu->period);
          ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
            తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              ఒకవేళ (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                తిరిగి ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  ఒకవేళ (cpu->quota)
    {
      ఒకవేళ (cgroup2)
        quota = cpu->quota;
      లేకపోతే
        {
          len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIi64, cpu->quota);
          ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
            తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          ఒకవేళ (period_str != ఖాళీ)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;
            }
        }
    }
  ఒకవేళ (cpu->realtime_period)
    {
      ఒకవేళ (cgroup2)
        తిరిగి crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64, cpu->realtime_period);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ఒకవేళ (cpu->realtime_runtime)
    {
      ఒకవేళ (cgroup2)
        తిరిగి crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (cgroup2 && (quota > 0 || period > 0))
    {
      ఒకవేళ (period < 0)
        period = 100000;
      ఒకవేళ (quota < 0)
        len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "max %" PRIi64, period);
      లేకపోతే
        len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", ఖాళీ, fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి write_cpu_వ్యవధి (dirfd_cpu, cgroup2, cpu, err);
}

పూర్ణాంకం
write_cpuset_resources (పూర్ణాంకం dirfd_cpuset, పూర్ణాంకం cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ఒకవేళ (cpu == ఖాళీ)
    తిరిగి 0;

  ఒకవేళ (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, అక్షరశ్రేణి_పొడవు (cpu->cpus), err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ఒకవేళ (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, అక్షరశ్రేణి_పొడవు (cpu->mems),
                                                 err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
open_cgroup_subsystem (స్థిరరాశి అక్షరం *subsystem, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *నిండిన_మార్గం = ఖాళీ;
  పూర్ణాంకం ret, dirfd;

  ret = append_paths (&నిండిన_మార్గం, err, CGROUP_మూలం, subsystem, మార్గం, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  dirfd = open (నిండిన_మార్గం, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (dirfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", నిండిన_మార్గం);

  తిరిగి dirfd;
}

పూర్ణాంకం
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ఒకవేళ (resources->ఖండం_io)
    {
      cleanup_close పూర్ణాంకం dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_ఖండం_io *blkio = resources->ఖండం_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", మార్గం, err);
      ఒకవేళ (UNLIKELY (dirfd_blkio < 0))
        తిరిగి dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (resources->జాలం)
    {
      cleanup_close పూర్ణాంకం dirfd_netclass = -1;
      cleanup_close పూర్ణాంకం dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_జాలం *జాలం = resources->జాలం;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", మార్గం, err);
      ఒకవేళ (UNLIKELY (dirfd_netclass < 0))
        తిరిగి dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", మార్గం, err);
      ఒకవేళ (UNLIKELY (dirfd_netprio < 0))
        తిరిగి dirfd_netprio;

      ret = write_జాలం_resources (dirfd_netclass, dirfd_netprio, జాలం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (resources->hugepage_limits_len)
    {
      cleanup_close పూర్ణాంకం dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", మార్గం, err);
      ఒకవేళ (UNLIKELY (dirfd_htlb < 0))
        తిరిగి dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (resources->devices_len)
    {
      cleanup_close పూర్ణాంకం dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", మార్గం, err);
      ఒకవేళ (UNLIKELY (dirfd_devs < 0))
        తిరిగి dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (resources->జ్ఞాపకం)
    {
      cleanup_close పూర్ణాంకం dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", మార్గం, err);
      ఒకవేళ (UNLIKELY (dirfd_mem < 0))
        తిరిగి dirfd_mem;

      ret = write_జ్ఞాపకం_resources (dirfd_mem, false, resources->జ్ఞాపకం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (resources->pids)
    {
      cleanup_close పూర్ణాంకం dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", మార్గం, err);
      ఒకవేళ (UNLIKELY (dirfd_pid < 0))
        తిరిగి dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (resources->cpu)
    {
      cleanup_close పూర్ణాంకం dirfd_cpu = -1;
      cleanup_close పూర్ణాంకం dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", మార్గం, err);
      ఒకవేళ (UNLIKELY (dirfd_cpu < 0))
        తిరిగి dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (resources->cpu->cpus == ఖాళీ && resources->cpu->mems == ఖాళీ)
        తిరిగి 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", మార్గం, err);
      ఒకవేళ (UNLIKELY (dirfd_cpuset < 0))
        తిరిగి dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (resources->unified && resources->unified->len > 0)
    తిరిగి crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
write_unified_resources (పూర్ణాంకం cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  పూర్ణాంకం ret;

  కొరకు (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close పూర్ణాంకం fd = -1;
      cleanup_free అక్షరం *విలువ = ఖాళీ;
      అక్షరం *saveptr = ఖాళీ;
      అక్షరం *పంక్తి;

      ఒకవేళ (strchr (resources->unified->కీలకాలు[i], '/'))
        తిరిగి crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->కీలకాలు[i]);

      ఒకవేళ (is_ఖాళీలు_string (resources->unified->విలువలు[i]))
        కొనసాగు;

      విలువ = xstrdup (resources->unified->విలువలు[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->కీలకాలు[i], O_WRONLY, err);
      ఒకవేళ (UNLIKELY (fd < 0))
        తిరిగి fd;

      కొరకు (పంక్తి = strtok_r (విలువ, "\n", &saveptr); పంక్తి; పంక్తి = strtok_r (ఖాళీ, "\n", &saveptr))
        {
          ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, పంక్తి, అక్షరశ్రేణి_పొడవు (పంక్తి)));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "write to `%s`", resources->unified->కీలకాలు[i]);
        }
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, స్థిరరాశి అక్షరం *మార్గం, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
  cleanup_close పూర్ణాంకం cgroup_dirfd = -1;
  పూర్ణాంకం ret;

  ఒకవేళ (resources->జాలం)
    తిరిగి crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, మార్గం, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  cgroup_dirfd = open (cgroup_మార్గం, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (cgroup_dirfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", cgroup_మార్గం);

  ఒకవేళ (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (resources->జ్ఞాపకం)
    {
      ret = write_జ్ఞాపకం_resources (cgroup_dirfd, true, resources->జ్ఞాపకం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ఒకవేళ (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ఒకవేళ (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  ఒకవేళ (resources->ఖండం_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->ఖండం_io, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  ఒకవేళ (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
పూర్ణాంకం
check_జ్ఞాపకం_before_update (స్థిరరాశి అక్షరం *మార్గం, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_జ్ఞాపకం *జ్ఞాపకం = resources->జ్ఞాపకం;
  cleanup_free అక్షరం *మార్పిడి_current = ఖాళీ;
  cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *current = ఖాళీ;
  cleanup_close పూర్ణాంకం dirfd = -1;
  uint64_t హద్దు = 0;
  uint64_t val, val_మార్పిడి;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం ret;

  ఒకవేళ (మార్గం == ఖాళీ || జ్ఞాపకం == ఖాళీ || ! (జ్ఞాపకం->check_before_update_present && జ్ఞాపకం->check_before_update))
    తిరిగి 0;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (cgroup_బహుళకం != CGROUP_బహుళకం_UNIFIED)
    తిరిగి 0;

  ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, మార్గం, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  dirfd = open (cgroup_మార్గం, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (dirfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", cgroup_మార్గం);

  ret = read_all_file_at (dirfd, "memory.current", &current, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &మార్పిడి_current, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  errno = 0;
  val = strtoll (current, ఖాళీ, 10);
  ఒకవేళ (UNLIKELY (errno))
    తిరిగి crun_make_error (err, errno, "parse memory.current");

  val_మార్పిడి = strtoll (మార్పిడి_current, ఖాళీ, 10);
  ఒకవేళ (UNLIKELY (errno))
    తిరిగి crun_make_error (err, errno, "parse memory.swap.current");

  ఒకవేళ (జ్ఞాపకం->హద్దు_present && జ్ఞాపకం->హద్దు >= 0)
    హద్దు = జ్ఞాపకం->హద్దు;
  ఒకవేళ (జ్ఞాపకం->మార్పిడి_present && జ్ఞాపకం->మార్పిడి >= 0)
    హద్దు += జ్ఞాపకం->మార్పిడి;

  ఒకవేళ (హద్దు <= val + val_మార్పిడి)
    తిరిగి crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  తిరిగి 0;
}

పూర్ణాంకం
update_cgroup_resources (స్థిరరాశి అక్షరం *మార్గం,
                         స్థిరరాశి అక్షరం *state_మూలం,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  పూర్ణాంకం cgroup_బహుళకం;

  (శూన్య) state_మూలం;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (మార్గం == ఖాళీ)
    {
      size_t i;

      ఒకవేళ (resources->ఖండం_io || resources->జాలం || resources->hugepage_limits_len || resources->జ్ఞాపకం
          || resources->pids || resources->cpu)
        తిరిగి crun_make_error (err, 0, "cannot set limits without cgroups");

      కొరకు (i = 0; i < resources->devices_len; i++)
        {
          పూర్ణాంకం rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          ఒకవేళ (UNLIKELY (rwm < 0))
            తిరిగి rwm;

          ఒకవేళ (rwm == 0)
            తిరిగి crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      తిరిగి 0;
    }
  ఎంపిక (cgroup_బహుళకం)
    {
    స్థితి CGROUP_బహుళకం_UNIFIED:
      తిరిగి update_cgroup_v2_resources (resources, మార్గం, need_bpf_dev, err);

    స్థితి CGROUP_బహుళకం_LEGACY:
    స్థితి CGROUP_బహుళకం_HYBRID:
      తిరిగి update_cgroup_v1_resources (resources, మార్గం, err);

    అప్రమేయ:
      తిరిగి crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_బహుళకం);
    }
}
