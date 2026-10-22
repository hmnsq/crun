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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "cgroup.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-internal.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-systemd.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-utils.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-resources.h"
#ଅନ୍ତର୍ଭାଵ "ebpf.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "status.h"
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/vfs.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <libgen.h>

ସଂରଚନା default_dev_s default_devices[] = {
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
  { 0, 0, 0, ଖାଲି }
};

ସଂରଚନା default_dev_s *
get_default_devices ()
{
  ଫେରନ୍ତୁ default_devices;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_cgroup_file_or_alias (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *alias, ଅଚଳ ଶୂନ୍ୟ *data, size_t len, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = write_cgroup_file (dirfd, name, data, len, err);
  ଯଦି (UNLIKELY (alias != ଖାଲି && ret < 0 && crun_error_get_errno (err) == ENOENT))
    {
      crun_error_release (err);
      ret = write_cgroup_file (dirfd, alias, data, len, err);
    }
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
openat_with_alias (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *alias, ଅଚଳ ଅକ୍ଷର **used_name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  *used_name = name;

  ret = openat (dirfd, name, flags);
  ଯଦି (UNLIKELY (ret < 0 && alias != ଖାଲି && errno == ENOENT))
    {
      *used_name = alias;
      ret = openat (dirfd, alias, flags);
    }
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", name);
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
is_rwm (ଅଚଳ ଅକ୍ଷର *str, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *it;
  bool r = false;
  bool w = false;
  bool m = false;

  ଯଦି (str == ଖାଲି)
    str = "";

  ପାଇଁ (it = str; *it; it++)
    ବାଛ (*it)
      {
      ସ୍ଥିତି 'r':
        r = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'w':
        w = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'm':
        m = true;
        ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid mode specified `%s`", str);
      }

  ଫେରନ୍ତୁ r && w && m ? 1 : 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
check_cgroup_v2_controller_available_wrapper (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_dirfd, ଅଚଳ ଅକ୍ଷର *name, libcrun_error_t *err)
{
  ଯଦି (ret == 0 || err == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  errno = crun_error_get_errno (err);

  /* If the file is not found, try to give a more meaningful error message.  */
  ଯଦି (errno == ENOENT || errno == EPERM || errno == EACCES)
    {
      cleanup_free ଅକ୍ଷର *controllers = ଖାଲି;
      libcrun_error_t tmp_err = ଖାଲି;
      cleanup_free ଅକ୍ଷର *ଚାବି = ଖାଲି;
      ଅକ୍ଷର *saveptr = ଖାଲି;
      bool found = false;
      ଅଚଳ ଅକ୍ଷର *token;
      ଅକ୍ଷର *it;

      /* Check if the specified controller is enabled.  */
      ଚାବି = xstrdup (name);

      it = strchr (ଚାବି, '.');
      ଯଦି (it == ଖାଲି)
        {
          crun_error_release (err);
          ଫେରନ୍ତୁ crun_make_error (err, 0, "the specified key has not the form CONTROLLER.VALUE `%s`", name);
        }
      *it = '\0';

      /* cgroup. files are not part of a controller.  Return the original error.  */
      ଯଦି (strcmp (ଚାବି, "cgroup") == 0)
        ଫେରନ୍ତୁ ret;

      /* If the cgroup.controllers file cannot be read, return the original error.  */
      ଯଦି (read_all_file_at (cgroup_dirfd, "cgroup.controllers", &controllers, ଖାଲି, &tmp_err) < 0)
        {
          crun_error_release (&tmp_err);
          ଫେରନ୍ତୁ ret;
        }
      ପାଇଁ (token = strtok_r (controllers, " \n", &saveptr); token; token = strtok_r (ଖାଲି, " \n", &saveptr))
        {
          ଯଦି (strcmp (token, ଚାବି) == 0)
            {
              found = true;
              ଭାଙ୍ଗ;
            }
        }
      ଯଦି (! found)
        {
          cleanup_free ଅକ୍ଷର *absolute_path = ଖାଲି;
          libcrun_error_t tmp_err = ଖାଲି;

          crun_error_release (err);
          ret = get_realpath_to_file (cgroup_dirfd, "cgroup.controllers", &absolute_path, &tmp_err);
          ଯଦି (LIKELY (ret >= 0))
            ret = crun_make_error (err, 0, "controller `%s` is not available under %s", ଚାବି, absolute_path);
          ଅନ୍ୟଥା
            {
              crun_error_release (&tmp_err);
              ret = crun_make_error (err, 0, "the requested cgroup controller `%s` is not available", ଚାବି);
            }
        }
    }
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_file_and_check_controllers_at (bool cgroup2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *name_alias,
                                     ଅଚଳ ଶୂନ୍ୟ *data, size_t len, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = write_cgroup_file_or_alias (dirfd, name, name_alias, data, len, err);
  ଯଦି (cgroup2)
    ଫେରନ୍ତୁ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_file_and_check_controllers_at (bool cgroup2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = openat (dirfd, name, flags);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "open `%s`", name);
      ଯଦି (cgroup2)
        ଫେରନ୍ତୁ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
    }
  ଫେରନ୍ତୁ ret;
}

/* The parser generates different structs but they are really all the same.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା runtime_spec_schema_defs_linux_block_io_device_throttle throttling_s;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_blkio_v1_resources_throttling (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  ଅକ୍ଷର fmt_buf[128];
  size_t i;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

  ଯଦି (throttling == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", name);

  ପାଇଁ (i = 0; i < throttling_len; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      throttling[i]->rate);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (ଲିଖ (fd, fmt_buf, len));
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "write `%s`", name);
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_blkio_v2_resources_throttling (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *name, throttling_s **throttling, size_t throttling_len,
                                     libcrun_error_t *err)
{
  ଅକ୍ଷର fmt_buf[128];
  size_t i;

  ଯଦି (throttling == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ପାଇଁ (i = 0; i < throttling_len; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64 ":%" PRIu64 " %s=%" PRIu64 "\n", throttling[i]->major, throttling[i]->minor,
                      name, throttling[i]->rate);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = TEMP_FAILURE_RETRY (ଲିଖ (fd, fmt_buf, len));
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "write `%s`", name);
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_blkio_resources (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_block_io *blkio,
                       libcrun_error_t *err)
{
  ଅକ୍ଷର fmt_buf[128];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (blkio->weight)
    {
      uint32_t val = blkio->weight;

      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu32, val);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
      ଯଦି (! cgroup2)
        {
          ret = write_cgroup_file_or_alias (dirfd, "blkio.weight", "blkio.bfq.weight", fmt_buf, len, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
      ଅନ୍ୟଥା
        {
          ret = write_cgroup_file (dirfd, "io.bfq.weight", fmt_buf, len, err);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ଯଦି (crun_error_get_errno (err) == ENOENT)
                {
                  crun_error_release (err);

                  /* convert linearly from [10-1000] to [1-10000] */
                  val = 1 + (val - 10) * 9999 / 990;

                  len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu32, val);
                  ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
                    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = write_cgroup_file (dirfd, "io.weight", fmt_buf, len, err);
                }

              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;
            }
        }
    }
  ଯଦି (blkio->leaf_weight)
    {
      ଯଦି (cgroup2)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set leaf_weight with cgroupv2");

      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%d", blkio->leaf_weight);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "blkio.leaf_weight", fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଯଦି (blkio->weight_device_len)
    {
      ଯଦି (cgroup2)
        {
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା wfd = -1;
          size_t i;

          wfd = openat (dirfd, "io.bfq.weight", O_WRONLY | O_CLOEXEC);
          ଯଦି (UNLIKELY (wfd < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "open `io.bfq.weight`");
          ପାଇଁ (i = 0; i < blkio->weight_device_len; i++)
            {
              uint32_t w = blkio->weight_device[i]->weight;

              len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64 ":%" PRIu64 " %i\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, w);
              ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
                ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (ଲିଖ (wfd, fmt_buf, len));
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ crun_make_error (err, errno, "write `io.bfq.weight`");

              /* Ignore blkio->weight_device[i]->leaf_weight.  */
            }
        }
      ଅନ୍ୟଥା
        {
          ଅଚଳ ଅକ୍ଷର *leaf_weight_device_file_name = ଖାଲି;
          ଅଚଳ ଅକ୍ଷର *weight_device_file_name = ଖାଲି;
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା w_leafdevice_fd = -1;
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା w_device_fd = -1;
          size_t i;

          w_device_fd = openat_with_alias (dirfd, "blkio.weight_device", "blkio.bfq.weight_device",
                                           &weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ଯଦି (UNLIKELY (w_device_fd < 0))
            ଫେରନ୍ତୁ w_device_fd;

          w_leafdevice_fd = openat_with_alias (dirfd, "blkio.leaf_weight_device", "blkio.bfq.leaf_weight_device",
                                               &leaf_weight_device_file_name, O_WRONLY | O_CLOEXEC, err);
          ଯଦି (UNLIKELY (w_leafdevice_fd < 0))
            {
              /* If the .leaf_weight_device file is missing, just ignore it.  */
              crun_error_release (err);
            }

          ପାଇଁ (i = 0; i < blkio->weight_device_len; i++)
            {
              len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                              blkio->weight_device[i]->minor, blkio->weight_device[i]->weight);
              ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
                ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

              ret = TEMP_FAILURE_RETRY (ଲିଖ (w_device_fd, fmt_buf, len));
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ crun_make_error (err, errno, "write `%s`", weight_device_file_name);

              ଯଦି (w_leafdevice_fd >= 0)
                {
                  len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64 ":%" PRIu64 " %" PRIu16 "\n", blkio->weight_device[i]->major,
                                  blkio->weight_device[i]->minor, blkio->weight_device[i]->leaf_weight);
                  ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
                    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

                  ret = TEMP_FAILURE_RETRY (ଲିଖ (w_leafdevice_fd, fmt_buf, len));
                  ଯଦି (UNLIKELY (ret < 0))
                    ଫେରନ୍ତୁ crun_make_error (err, errno, "write `%s`", leaf_weight_device_file_name);
                }
            }
        }
    }
  ଯଦି (cgroup2)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା wfd = -1;
      ଅଚଳ ଅକ୍ଷର *name = "io.max";

      wfd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ଯଦି (UNLIKELY (wfd < 0))
        {
          ret = crun_make_error (err, errno, "open `%s`", name);
          ଫେରନ୍ତୁ check_cgroup_v2_controller_available_wrapper (ret, dirfd, name, err);
        }

      ret = write_blkio_v2_resources_throttling (wfd, "rbps", (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wbps", (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "riops", (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = write_blkio_v2_resources_throttling (wfd, "wiops", (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_bps_device",
                                                 (throttling_s **) blkio->throttle_read_bps_device,
                                                 blkio->throttle_read_bps_device_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_bps_device",
                                                 (throttling_s **) blkio->throttle_write_bps_device,
                                                 blkio->throttle_write_bps_device_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.read_iops_device",
                                                 (throttling_s **) blkio->throttle_read_iops_device,
                                                 blkio->throttle_read_iops_device_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = write_blkio_v1_resources_throttling (dirfd, "blkio.throttle.write_iops_device",
                                                 (throttling_s **) blkio->throttle_write_iops_device,
                                                 blkio->throttle_write_iops_device_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_network_resources (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_netclass, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_netprio, runtime_spec_schema_config_linux_resources_network *net,
                         libcrun_error_t *err)
{
  ଅକ୍ଷର fmt_buf[128];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଯଦି (net->class_id)
    {
      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu32, net->class_id);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd_netclass, "net_cls.classid", fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଯଦି (net->priorities_len)
    {
      size_t i;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
      fd = openat (dirfd_netprio, "net_prio.ifpriomap", O_WRONLY | O_CLOEXEC);
      ଯଦି (UNLIKELY (fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `net_prio.ifpriomap`");

      ପାଇଁ (i = 0; i < net->priorities_len; i++)
        {
          len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%s %" PRIu32 "\n", net->priorities[i]->name,
                          net->priorities[i]->priority);
          ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = TEMP_FAILURE_RETRY (ଲିଖ (fd, fmt_buf, len));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "write `net_prio.ifpriomap`");
        }
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_hugetlb_resources (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, bool cgroup2,
                         runtime_spec_schema_config_linux_resources_hugepage_limits_element **htlb, size_t htlb_len,
                         libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *suffix = cgroup2 ? "max" : "limit_in_bytes";
  bool skip_rsvd = false;
  ଅକ୍ଷର fmt_buf[128];
  size_t i;
  ପାଇଁ (i = 0; i < htlb_len; i++)
    {
      cleanup_free ଅକ୍ଷର *filename = ଖାଲି;
      cleanup_free ଅକ୍ଷର *rsvd_filename = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      xasprintf (&filename, "hugetlb.%s.%s", htlb[i]->page_size, suffix);

      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64, htlb[i]->limit);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, filename, ଖାଲି, fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଯଦି (skip_rsvd)
        ଜାରି;

      /* Reservation accounting (Linux >= 5.7) is used when available.  */
      xasprintf (&rsvd_filename, "hugetlb.%s.rsvd.%s", htlb[i]->page_size, suffix);
      ret = write_cgroup_file (dirfd, rsvd_filename, fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (crun_error_get_errno (err) != ENOENT)
            ଫେରନ୍ତୁ ret;
          crun_error_release (err);
          skip_rsvd = true;
        }
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_devices_resources_v1 (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ପାଇଁ (i = 0; i < devs_len; i++)
    {
      /* It is plenty of room for "TYPE MAJOR:MINOR ACCESS", where type is one char, and ACCESS is at most 3.   */
#define FMT_BUF_LEN 64
      ଅକ୍ଷର fmt_buf[FMT_BUF_LEN];
      ଅଚଳ ଅକ୍ଷର *file = devs[i]->allow ? "devices.allow" : "devices.deny";

      ଯଦି (devs[i]->type == ଖାଲି || devs[i]->type[0] == 'a')
        {
          strcpy (fmt_buf, "a");
          len = 1;
        }
      ଅନ୍ୟଥା
        {
          ଅକ୍ଷର fmt_buf_major[16];
          ଅକ୍ଷର fmt_buf_minor[16];

#define FMT_DEV(x, b)                                                                   \
  କର                                                                                    \
    {                                                                                   \
      ଯଦି (x##_present)                                                                  \
        {                                                                               \
          len = snprintf (b, ଆକାର (b), "%" PRIi64, x);                                \
          ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (b)))                                       \
            ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ଅନ୍ୟଥା                                                                              \
        strcpy (b, "*");                                                                \
  } ଯେତେବେଳେ (0)

          FMT_DEV (devs[i]->major, fmt_buf_major);
          FMT_DEV (devs[i]->minor, fmt_buf_minor);
#undef FMT_DEV

          /* `access` is optional, and libocispec leaves it NULL when it is not
             specified.  Printing a NULL pointer with %s is undefined.  */
          len = snprintf (fmt_buf, FMT_BUF_LEN, "%s %s:%s %s", devs[i]->type, fmt_buf_major, fmt_buf_minor,
                          devs[i]->access ? devs[i]->access : "");
          ଯଦି (UNLIKELY (len >= FMT_BUF_LEN))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
        }
      ret = write_cgroup_file (dirfd, file, fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ପାଇଁ (i = 0; default_devices[i].type; i++)
    {
      ଅକ୍ଷର fmt_buf_major[16];
      ଅକ୍ଷର fmt_buf_minor[16];
      ଅକ୍ଷର device[64];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

#define FMT_DEV(x, b)                                                                   \
  କର                                                                                    \
    {                                                                                   \
      ଯଦି (x != -1)                                                                      \
        {                                                                               \
          len = snprintf (b, ଆକାର (b), "%d", x);                                      \
          ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (b)))                                       \
            ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small"); \
        }                                                                               \
      ଅନ୍ୟଥା                                                                              \
        strcpy (b, "*");                                                                \
  } ଯେତେବେଳେ (0)

      FMT_DEV (default_devices[i].major, fmt_buf_major);
      FMT_DEV (default_devices[i].minor, fmt_buf_minor);

#undef FMT_DEV

      len = snprintf (device, ଆକାର (device), "%c %s:%s %s", default_devices[i].type, fmt_buf_major, fmt_buf_minor,
                      default_devices[i].access);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (device)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_cgroup_file (dirfd, "devices.allow", device, strlen (device), err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ସଂରଚନା bpf_program *
create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
  ସଂରଚନା bpf_program *program;

  program = bpf_program_new (2048);

  program = bpf_program_init_dev (program, err);
  ଯଦି (UNLIKELY (program == ଖାଲି))
    ଫେରନ୍ତୁ ଖାଲି;

  ପାଇଁ (i = (ଆକାର (default_devices) / ଆକାର (default_devices[0])) - 1; i >= 0; i--)
    {
      program = bpf_program_append_dev (program, default_devices[i].access, default_devices[i].type,
                                        default_devices[i].major, default_devices[i].minor, true, err);
      ଯଦି (UNLIKELY (program == ଖାଲି))
        ଫେରନ୍ତୁ ଖାଲି;
    }
  ପାଇଁ (i = devs_len - 1; i >= 0; i--)
    {
      ଅକ୍ଷର type = 'a';
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor = -1, major = -1;
      ଯଦି (devs[i]->type != ଖାଲି)
        type = devs[i]->type[0];

      ଯଦି (devs[i]->major_present)
        major = devs[i]->major;
      ଯଦି (devs[i]->minor_present)
        minor = devs[i]->minor;

      program = bpf_program_append_dev (program, devs[i]->access, type, major, minor, devs[i]->allow, err);
      ଯଦି (UNLIKELY (program == ଖାଲି))
        ଫେରନ୍ତୁ ଖାଲି;
    }

  program = bpf_program_complete_dev (program, err);
  ଯଦି (UNLIKELY (program == ଖାଲି))
    ଫେରନ୍ତୁ ଖାଲି;

  ଫେରନ୍ତୁ program;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_devices_resources_v2_internal (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                     libcrun_error_t *err)
{
  cleanup_free ସଂରଚନା bpf_program *program = ଖାଲି;

  program = create_dev_bpf (devs, devs_len, err);
  ଯଦି (UNLIKELY (program == ଖାଲି))
    ଫେରନ୍ତୁ -1;

  ଫେରନ୍ତୁ libcrun_ebpf_load (program, dirfd, ଖାଲି, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_devices_resources_v2 (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                            libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  size_t i;
  bool can_skip = true;

  ret = write_devices_resources_v2_internal (dirfd, devs, devs_len, err);
  ଯଦି (LIKELY (ret == 0))
    ଫେରନ୍ତୁ 0;

  /* If writing the resources ebpf failed, check if it is fine to ignore the error.  */
  ପାଇଁ (i = 0; i < devs_len; i++)
    {
      ଯଦି (devs[i]->allow_present && ! devs[i]->allow)
        {
          can_skip = false;
          ଭାଙ୍ଗ;
        }
    }

  ଯଦି (! can_skip)
    {
      libcrun_error_t tmp_err = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootless;

      rootless = is_rootless (&tmp_err);
      ଯଦି (UNLIKELY (rootless < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ଫେରନ୍ତୁ ret;
        }
      ଯଦି (rootless)
        can_skip = true;
    }

  ଯଦି (can_skip)
    {
      crun_error_release (err);
      ret = 0;
    }

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_devices_resources (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, bool cgroup2, runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                         libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (cgroup2)
    ret = write_devices_resources_v2 (dirfd, devs, devs_len, err);
  ଅନ୍ୟଥା
    ret = write_devices_resources_v1 (dirfd, devs, devs_len, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootless;

      rootless = is_rootless (&tmp_err);
      ଯଦି (UNLIKELY (rootless < 0))
        {
          crun_error_release (&tmp_err);
          ଫେରନ୍ତୁ ret;
        }

      ଯଦି (rootless)
        {
          crun_error_release (err);
          ret = 0;
        }
    }
  ଫେରନ୍ତୁ ret;
}

/* use for cgroupv2 files with .min, .max, .low, or .high suffix */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cg_itoa (ଅକ୍ଷର *buf, size_t size, int64_t ମୂଲ୍ୟ, bool use_max, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

  ଯଦି (use_max && ମୂଲ୍ୟ < 0)
    len = snprintf (buf, size, "max");
  ଅନ୍ୟଥା
    len = snprintf (buf, size, "%" PRIi64, ମୂଲ୍ୟ);

  ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) size))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

  ଫେରନ୍ତୁ len;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_memory (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory, libcrun_error_t *err)
{
  ଅକ୍ଷର limit_buf[32];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା limit_buf_len;

  ଯଦି (! memory->limit_present)
    ଫେରନ୍ତୁ 0;

  limit_buf_len = cg_itoa (limit_buf, ଆକାର (limit_buf), memory->limit, cgroup2, err);
  ଯଦି (UNLIKELY (limit_buf_len < 0))
    ଫେରନ୍ତୁ limit_buf_len;

  ଫେରନ୍ତୁ write_cgroup_file (dirfd, cgroup2 ? "memory.max" : "memory.limit_in_bytes", limit_buf, limit_buf_len, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_memory_swap (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                   libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  int64_t swap;
  ଅକ୍ଷର swap_buf[32];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
  ଅଚଳ ଅକ୍ଷର *fname = cgroup2 ? "memory.swap.max" : "memory.memsw.limit_in_bytes";

  ଯଦି (! memory->swap_present)
    ଫେରନ୍ତୁ 0;

  swap = memory->swap;
  // Cgroupv2 apply limit must check if swap > 0, since `0` and `-1` are special case
  // 0: This means process will not be able to use any swap space.
  // -1: This means that the process can use as much swap as it needs.
  ଯଦି (cgroup2 && memory->swap > 0)
    {
      ଯଦି (! memory->limit_present)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ଯଦି (memory->swap < memory->limit)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      swap -= memory->limit;
    }

  len = cg_itoa (swap_buf, ଆକାର (swap_buf), swap, cgroup2, err);
  ଯଦି (UNLIKELY (len < 0))
    ଫେରନ୍ତୁ len;

  ret = write_cgroup_file (dirfd, fname, swap_buf, len, err);
  ଯଦି (ret >= 0)
    ଫେରନ୍ତୁ ret;

  /* If swap is not enabled, ignore the error.  */
  ଯଦି (crun_error_get_errno (err) == ENOENT)
    {
      crun_error_release (err);
      ଫେରନ୍ତୁ 0;
    }

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_memory_resources (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_memory *memory,
                        libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅକ୍ଷର fmt_buf[32];
  bool memory_limits_written = false;

  ଯଦି (memory->limit_present)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      ଯଦି (ret >= 0)
        memory_limits_written = true;
      ଅନ୍ୟଥା
        {
          ଯଦି (cgroup2 || crun_error_get_errno (err) != EINVAL)
            ଫେରନ୍ତୁ ret;

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
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (memory->limit_present && ! memory_limits_written)
    {
      ret = write_memory (dirfd, cgroup2, memory, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (memory->kernel_present)
    {
      ଯଦି (cgroup2)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set kernel memory with cgroupv2");

      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64, memory->kernel);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.limit_in_bytes", fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  // allows users set use_hierarchy as 0 when defined as false in spec.
  // if use_hierarchy is not defined in spec value defaults to 1 (True).
  // Note: users can only toggle use_hierarchy if the parent cgroup has use_hierarchy configured as 0.
  ଯଦି (memory->use_hierarchy_present)
    {
      ଯଦି (cgroup2)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set useHierarchy memory with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.use_hierarchy", (memory->use_hierarchy) ? "1" : "0", 1, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (memory->reservation_present)
    {
      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64, memory->reservation);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_file_and_check_controllers_at (cgroup2, dirfd, cgroup2 ? "memory.low" : "memory.soft_limit_in_bytes",
                                                 ଖାଲି, fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଯଦି (memory->disable_oom_killer)
    {
      ଯଦି (cgroup2)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot disable OOM killer with cgroupv2");

      ret = write_cgroup_file (dirfd, "memory.oom_control", "1", 1, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଯଦି (memory->kernel_tcp_present)
    {
      ଯଦି (cgroup2)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set kernel TCP with cgroupv2");

      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64, memory->kernel_tcp);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.kmem.tcp.limit_in_bytes", fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଯଦି (memory->swappiness_present)
    {
      ଯଦି (cgroup2)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set memory swappiness with cgroupv2");

      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64, memory->swappiness);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd, "memory.swappiness", fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_cpu_idle (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_dirfd, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err)
{
  ଅକ୍ଷର fmt_buf[32];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

  ଯଦି (! cpu->idle_present)
    ଫେରନ୍ତୁ 0;

  len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIi64, cpu->idle);
  ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
  ଫେରନ୍ତୁ write_cgroup_file (cpu_dirfd, "cpu.idle", fmt_buf, len, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_cpu_burst (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                 libcrun_error_t *err)
{
  ଅକ୍ଷର fmt_buf[32];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

  ଯଦି (! cpu->burst_present)
    ଫେରନ୍ତୁ 0;

  len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIi64, cpu->burst);
  ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
  ଫେରନ୍ତୁ write_cgroup_file (cpu_dirfd, cgroup2 ? "cpu.max.burst" : "cpu.cfs_burst_us", fmt_buf, len, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_pids_resources (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_pids *pids,
                      libcrun_error_t *err)
{
  ଯଦି (pids->limit)
    {
      ଅକ୍ଷର fmt_buf[32];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      len = cg_itoa (fmt_buf, ଆକାର (fmt_buf), pids->limit, true, err);
      ଯଦି (UNLIKELY (len < 0))
        ଫେରନ୍ତୁ len;

      ret = write_file_and_check_controllers_at (cgroup2, dirfd, "pids.max", ଖାଲି, fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_cpu_resources (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_cpu, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                     libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, period_len = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅକ୍ଷର fmt_buf[64];
  int64_t period = -1;
  int64_t quota = -1;
  cleanup_free ଅକ୍ଷର *period_str = ଖାଲି;

  ଯଦି (cpu->shares)
    {
      uint32_t val = cpu->shares;

      ଯଦି (cgroup2)
        {
          val = convert_shares_to_weight (val);

          /* The weight cannot be changed while cpu.idle is set, and setting
             it means the cgroup is not idle (this is what systemd does, too),
             so reset cpu.idle unless it is also being set.  The file might
             not exist on older kernels, so ignore errors.  */
          ଯଦି (! cpu->idle_present)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.idle", "0", 1, err);
              ଯଦି (UNLIKELY (ret < 0))
                crun_error_release (err);
            }
        }

      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%u", val);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, cgroup2 ? "cpu.weight" : "cpu.shares",
                                                 ଖାଲି, fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଯଦି (cpu->period)
    {
      ଯଦି (cgroup2)
        period = cpu->period;
      ଅନ୍ୟଥା
        {
          len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64, cpu->period);
          ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", fmt_buf, len, err);
          ଯଦି (UNLIKELY (ret < 0))
            {
              /*
                Sometimes when the period to be set is smaller than the current one,
                it is rejected by the kernel (EINVAL) as old_quota/new_period exceeds
                the parent cgroup quota limit. If this happens and the quota is going
                to be set, ignore the error for now and retry after setting the quota.
               */
              ଯଦି (! cpu->quota || crun_error_get_errno (err) != EINVAL)
                ଫେରନ୍ତୁ ret;

              crun_error_release (err);
              period_str = xstrdup (fmt_buf);
              period_len = len;
            }
        }
    }
  ଯଦି (cpu->quota)
    {
      ଯଦି (cgroup2)
        quota = cpu->quota;
      ଅନ୍ୟଥା
        {
          len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIi64, cpu->quota);
          ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
          ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_quota_us", fmt_buf, len, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          ଯଦି (period_str != ଖାଲି)
            {
              ret = write_cgroup_file (dirfd_cpu, "cpu.cfs_period_us", period_str, period_len, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;
            }
        }
    }
  ଯଦି (cpu->realtime_period)
    {
      ଯଦି (cgroup2)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "realtime period not supported on cgroupv2");
      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64, cpu->realtime_period);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_period_us", fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଯଦି (cpu->realtime_runtime)
    {
      ଯଦି (cgroup2)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "realtime runtime not supported on cgroupv2");
      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64, cpu->realtime_runtime);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
      ret = write_cgroup_file (dirfd_cpu, "cpu.rt_runtime_us", fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ret = write_cpu_idle (dirfd_cpu, cpu, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (cgroup2 && (quota > 0 || period > 0))
    {
      ଯଦି (period < 0)
        period = 100000;
      ଯଦି (quota < 0)
        len = snprintf (fmt_buf, ଆକାର (fmt_buf), "max %" PRIi64, period);
      ଅନ୍ୟଥା
        len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIi64 " %" PRIi64, quota, period);

      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpu, "cpu.max", ଖାଲି, fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ write_cpu_burst (dirfd_cpu, cgroup2, cpu, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_cpuset_resources (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_cpuset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu,
                        libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (cpu == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ଯଦି (cpu->cpus)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.cpus", "cpus",
                                                 cpu->cpus, strlen (cpu->cpus), err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଯଦି (cpu->mems)
    {
      ret = write_file_and_check_controllers_at (cgroup2, dirfd_cpuset, "cpuset.mems", "mems", cpu->mems, strlen (cpu->mems),
                                                 err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_cgroup_subsystem (ଅଚଳ ଅକ୍ଷର *subsystem, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *full_path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, dirfd;

  ret = append_paths (&full_path, err, CGROUP_ମୂଳ, subsystem, path, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  dirfd = open (full_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (dirfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", full_path);

  ଫେରନ୍ତୁ dirfd;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
update_cgroup_v1_resources (runtime_spec_schema_config_linux_resources *resources, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (resources->block_io)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_blkio = -1;
      runtime_spec_schema_config_linux_resources_block_io *blkio = resources->block_io;

      dirfd_blkio = open_cgroup_subsystem ("blkio", path, err);
      ଯଦି (UNLIKELY (dirfd_blkio < 0))
        ଫେରନ୍ତୁ dirfd_blkio;

      ret = write_blkio_resources (dirfd_blkio, false, blkio, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (resources->network)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_netclass = -1;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_netprio = -1;
      runtime_spec_schema_config_linux_resources_network *network = resources->network;

      dirfd_netclass = open_cgroup_subsystem ("net_cls", path, err);
      ଯଦି (UNLIKELY (dirfd_netclass < 0))
        ଫେରନ୍ତୁ dirfd_netclass;

      dirfd_netprio = open_cgroup_subsystem ("net_prio", path, err);
      ଯଦି (UNLIKELY (dirfd_netprio < 0))
        ଫେରନ୍ତୁ dirfd_netprio;

      ret = write_network_resources (dirfd_netclass, dirfd_netprio, network, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (resources->hugepage_limits_len)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_htlb = -1;

      dirfd_htlb = open_cgroup_subsystem ("hugetlb", path, err);
      ଯଦି (UNLIKELY (dirfd_htlb < 0))
        ଫେରନ୍ତୁ dirfd_htlb;

      ret = write_hugetlb_resources (dirfd_htlb, false, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (resources->devices_len)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_devs = -1;

      dirfd_devs = open_cgroup_subsystem ("devices", path, err);
      ଯଦି (UNLIKELY (dirfd_devs < 0))
        ଫେରନ୍ତୁ dirfd_devs;

      ret = write_devices_resources (dirfd_devs, false, resources->devices, resources->devices_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (resources->memory)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_mem = -1;

      dirfd_mem = open_cgroup_subsystem ("memory", path, err);
      ଯଦି (UNLIKELY (dirfd_mem < 0))
        ଫେରନ୍ତୁ dirfd_mem;

      ret = write_memory_resources (dirfd_mem, false, resources->memory, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (resources->pids)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_pid = -1;

      dirfd_pid = open_cgroup_subsystem ("pids", path, err);
      ଯଦି (UNLIKELY (dirfd_pid < 0))
        ଫେରନ୍ତୁ dirfd_pid;

      ret = write_pids_resources (dirfd_pid, false, resources->pids, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (resources->cpu)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_cpu = -1;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_cpuset = -1;

      dirfd_cpu = open_cgroup_subsystem ("cpu", path, err);
      ଯଦି (UNLIKELY (dirfd_cpu < 0))
        ଫେରନ୍ତୁ dirfd_cpu;

      ret = write_cpu_resources (dirfd_cpu, false, resources->cpu, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଯଦି (resources->cpu->cpus == ଖାଲି && resources->cpu->mems == ଖାଲି)
        ଫେରନ୍ତୁ 0;

      dirfd_cpuset = open_cgroup_subsystem ("cpuset", path, err);
      ଯଦି (UNLIKELY (dirfd_cpuset < 0))
        ଫେରନ୍ତୁ dirfd_cpuset;

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (resources->unified && resources->unified->len > 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid configuration: cannot use unified on cgroup v1");

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_unified_resources (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_dirfd, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ପାଇଁ (i = 0; i < resources->unified->len; i++)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
      cleanup_free ଅକ୍ଷର *ମୂଲ୍ୟ = ଖାଲି;
      ଅକ୍ଷର *saveptr = ଖାଲି;
      ଅକ୍ଷର *ଧାଡ଼ି;

      ଯଦି (strchr (resources->unified->ଚାବିଗୁଡ଼ିକ[i], '/'))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "key `%s` must be a file name without any slash", resources->unified->ଚାବିଗୁଡ଼ିକ[i]);

      ଯଦି (is_empty_string (resources->unified->ମୂଲ୍ୟଗୁଡ଼ିକ[i]))
        ଜାରି;

      ମୂଲ୍ୟ = xstrdup (resources->unified->ମୂଲ୍ୟଗୁଡ଼ିକ[i]);

      fd = open_file_and_check_controllers_at (true, cgroup_dirfd, resources->unified->ଚାବିଗୁଡ଼ିକ[i], O_WRONLY, err);
      ଯଦି (UNLIKELY (fd < 0))
        ଫେରନ୍ତୁ fd;

      ପାଇଁ (ଧାଡ଼ି = strtok_r (ମୂଲ୍ୟ, "\n", &saveptr); ଧାଡ଼ି; ଧାଡ଼ି = strtok_r (ଖାଲି, "\n", &saveptr))
        {
          ret = TEMP_FAILURE_RETRY (ଲିଖ (fd, ଧାଡ଼ି, strlen (ଧାଡ଼ି)));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "write to `%s`", resources->unified->ଚାବିଗୁଡ଼ିକ[i]);
        }
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
update_cgroup_v2_resources (runtime_spec_schema_config_linux_resources *resources, ଅଚଳ ଅକ୍ଷର *path, bool need_bpf_dev, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_dirfd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (resources->network)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "network limits not supported on cgroupv2");

  ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, path, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  cgroup_dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (cgroup_dirfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ଯଦି (need_bpf_dev && resources->devices_len)
    {
      ret = write_devices_resources (cgroup_dirfd, true, resources->devices, resources->devices_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (resources->memory)
    {
      ret = write_memory_resources (cgroup_dirfd, true, resources->memory, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଯଦି (resources->pids)
    {
      ret = write_pids_resources (cgroup_dirfd, true, resources->pids, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଯଦି (resources->cpu)
    {
      ret = write_cpu_resources (cgroup_dirfd, true, resources->cpu, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = write_cpuset_resources (cgroup_dirfd, true, resources->cpu, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଯଦି (resources->block_io)
    {
      ret = write_blkio_resources (cgroup_dirfd, true, resources->block_io, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (resources->hugepage_limits_len)
    {
      ret = write_hugetlb_resources (cgroup_dirfd, true, resources->hugepage_limits, resources->hugepage_limits_len,
                                     err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  /* Write unified resources if any.  They have higher precedence and override any previous setting.  */
  ଯଦି (resources->unified)
    {
      ret = write_unified_resources (cgroup_dirfd, resources, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

/* With checkBeforeUpdate, refuse to set the memory limit lower than the
   current usage.  This must be done before any of the resources is updated,
   including by the cgroup manager (e.g. the systemd unit properties).  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
check_memory_before_update (ଅଚଳ ଅକ୍ଷର *path, runtime_spec_schema_config_linux_resources *resources,
                            libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
  cleanup_free ଅକ୍ଷର *swap_current = ଖାଲି;
  cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *current = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd = -1;
  uint64_t limit = 0;
  uint64_t val, val_swap;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (path == ଖାଲି || memory == ଖାଲି || ! (memory->check_before_update_present && memory->check_before_update))
    ଫେରନ୍ତୁ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (cgroup_mode != CGROUP_MODE_UNIFIED)
    ଫେରନ୍ତୁ 0;

  ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, path, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (dirfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ret = read_all_file_at (dirfd, "memory.current", &current, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = read_all_file_at (dirfd, "memory.swap.current", &swap_current, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  errno = 0;
  val = strtoll (current, ଖାଲି, 10);
  ଯଦି (UNLIKELY (errno))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "parse memory.current");

  val_swap = strtoll (swap_current, ଖାଲି, 10);
  ଯଦି (UNLIKELY (errno))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "parse memory.swap.current");

  ଯଦି (memory->limit_present && memory->limit >= 0)
    limit = memory->limit;
  ଯଦି (memory->swap_present && memory->swap >= 0)
    limit += memory->swap;

  ଯଦି (limit <= val + val_swap)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set the memory limit lower than its current usage");

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
update_cgroup_resources (ଅଚଳ ଅକ୍ଷର *path,
                         ଅଚଳ ଅକ୍ଷର *state_ମୂଳ,
                         runtime_spec_schema_config_linux_resources *resources,
                         bool need_bpf_dev,
                         libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;

  (ଶୂନ୍ୟ) state_ମୂଳ;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (path == ଖାଲି)
    {
      size_t i;

      ଯଦି (resources->block_io || resources->network || resources->hugepage_limits_len || resources->memory
          || resources->pids || resources->cpu)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set limits without cgroups");

      ପାଇଁ (i = 0; i < resources->devices_len; i++)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା rwm;

          rwm = is_rwm (resources->devices[i]->access, err);
          ଯଦି (UNLIKELY (rwm < 0))
            ଫେରନ୍ତୁ rwm;

          ଯଦି (rwm == 0)
            ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set limits without cgroups");
        }

      ଫେରନ୍ତୁ 0;
    }
  ବାଛ (cgroup_mode)
    {
    ସ୍ଥିତି CGROUP_MODE_UNIFIED:
      ଫେରନ୍ତୁ update_cgroup_v2_resources (resources, path, need_bpf_dev, err);

    ସ୍ଥିତି CGROUP_MODE_LEGACY:
    ସ୍ଥିତି CGROUP_MODE_HYBRID:
      ଫେରନ୍ତୁ update_cgroup_v1_resources (resources, path, err);

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }
}
