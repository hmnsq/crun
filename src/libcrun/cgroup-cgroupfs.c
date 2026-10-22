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
#ଅନ୍ତର୍ଭାଵ "cgroup-setup.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-utils.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-cgroupfs.h"
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <sys/vfs.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <libgen.h>

ସ୍ଥିର ଅକ୍ଷର *
make_cgroup_path (ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *id)
{
  ଅକ୍ଷର *ret;

  ଯଦି (path == ଖାଲି)
    xasprintf (&ret, "/%s", id);
  ଅନ୍ୟଥା ଯଦି (path[0] == '/')
    ret = xstrdup (path);
  ଅନ୍ୟଥା
    xasprintf (&ret, "/%s", path);

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_precreate_cgroup_cgroupfs (ସଂରଚନା libcrun_cgroup_args *args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *dirfd, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *sub_path = make_cgroup_path (args->cgroup_path, args->id);
  cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *parent_path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, sub_path, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  parent_path = xstrdup (cgroup_path);
  ret = crun_ensure_directory (dirname (parent_path), 0755, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      ଫେରନ୍ତୁ 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  ଯଦି (mkdir (cgroup_path, 0755) == 0)
    args->precreated_path = xstrdup (cgroup_path);
  ଅନ୍ୟଥା ଯଦି (errno != EEXIST)
    ଫେରନ୍ତୁ 0;

  ret = enable_controllers (sub_path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  *dirfd = open (cgroup_path, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_PATH);
  ଯଦି (UNLIKELY (*dirfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
make_new_sibling_cgroup (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *current_cgroup = ଖାଲି;
  ଅକ୍ଷର *dir;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  dir = dirname (current_cgroup);

  append_paths (out, err, dir, id, ଖାଲି);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_enter_cgroupfs (ସଂରଚନା libcrun_cgroup_args *args, ସଂରଚନା libcrun_cgroup_status *out, libcrun_error_t *err)
{
  pid_t pid = args->pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  out->path = make_cgroup_path (args->cgroup_path, args->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  ଯଦି (args->joined)
    ଫେରନ୍ତୁ 0;

  ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      ret = enable_controllers (out->path, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          ଯଦି (args->cgroup_path == ଖାଲି)
            {
              libcrun_error_t tmp_err = ଖାଲି;
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmp_ret;

              ମୁକ୍ତ (out->path);
              out->path = ଖାଲି;

              tmp_ret = make_new_sibling_cgroup (&out->path, args->id, &tmp_err);
              ଯଦି (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  ଫେରନ୍ତୁ ret;
                }

              crun_error_release (err);
              ret = enable_controllers (out->path, err);
            }
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (out->path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ enter_cgroup (cgroup_mode, pid, 0, out->path, true, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_destroy_cgroup_cgroupfs (ସଂରଚନା libcrun_cgroup_status *cgroup_status,
                                 libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (mode < 0))
    ଫେରନ୍ତୁ mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  ଯଦି (UNLIKELY (ret < 0))
    crun_error_release (err);

  ଫେରନ୍ତୁ destroy_cgroup_path (cgroup_status->path, mode, err);
}

ସଂରଚନା libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
