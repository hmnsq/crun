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
#अन्तर्भाव "cgroup-setup.h"
#अन्तर्भाव "cgroup-utils.h"
#अन्तर्भाव "cgroup-cgroupfs.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <libgen.h>

स्थिर वर्ण *
make_cgroup_मार्गः (ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *id)
{
  वर्ण *ret;

  यदि (मार्गः == रिक्त)
    xasprintf (&ret, "/%s", id);
  अन्यथा यदि (मार्गः[0] == '/')
    ret = xstrdup (मार्गः);
  अन्यथा
    xasprintf (&ret, "/%s", मार्गः);

  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
libcrun_precreate_cgroup_cgroupfs (संरचना libcrun_cgroup_तर्काः *तर्काः, पूर्णाङ्क *dirfd, libcrun_error_t *err)
{
  cleanup_free वर्ण *sub_मार्गः = make_cgroup_मार्गः (तर्काः->cgroup_मार्गः, तर्काः->id);
  cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
  cleanup_free वर्ण *parent_मार्गः = रिक्त;
  पूर्णाङ्क ret;

  /* No need to check the mode since this feature is supported only on cgroup v2, and
     libcrun_cgroup_preenter already performs this check.  */

  *dirfd = -1;

  /* The container process is created directly in the cgroup, so check it
     now: once it is there, it is too late.  */
  ret = libcrun_cgroup_ensure_not_frozen (sub_मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, sub_मार्गः, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  parent_मार्गः = xstrdup (cgroup_मार्गः);
  ret = crun_ensure_directory (dirname (parent_मार्गः), 0755, true, err);
  यदि (UNLIKELY (ret < 0))
    {
      libcrun_error_release (err);
      प्रत्यागम 0;
    }

  /* Remember the cgroup created here, so it can be removed if the container
     creation fails before there is a status for it.  A pre-existing cgroup is
     left alone.  */
  यदि (mkdir (cgroup_मार्गः, 0755) == 0)
    तर्काः->precreated_मार्गः = xstrdup (cgroup_मार्गः);
  अन्यथा यदि (errno != EEXIST)
    प्रत्यागम 0;

  ret = enable_controllers (sub_मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  *dirfd = उद्घाटय (cgroup_मार्गः, O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY | O_मार्गः);
  यदि (UNLIKELY (*dirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_मार्गः);

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
make_new_sibling_cgroup (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *current_cgroup = रिक्त;
  वर्ण *dir;
  पूर्णाङ्क ret;

  ret = libcrun_get_cgroup_process (0, &current_cgroup, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dir = dirname (current_cgroup);

  append_paths (बहिर्गमनम्, err, dir, id, रिक्त);
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libcrun_cgroup_enter_cgroupfs (संरचना libcrun_cgroup_तर्काः *तर्काः, संरचना libcrun_cgroup_स्थितिः *बहिर्गमनम्, libcrun_error_t *err)
{
  pid_t pid = तर्काः->pid;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क ret;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  बहिर्गमनम्->मार्गः = make_cgroup_मार्गः (तर्काः->cgroup_मार्गः, तर्काः->id);

  /* If the cgroup was already joined via CLONE_INTO_CGROUP, the controllers
     were already enabled by precreate_cgroup.  */
  यदि (तर्काः->joined)
    प्रत्यागम 0;

  यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
    {
      ret = enable_controllers (बहिर्गमनम्->मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        {
          /* If the generated path cannot be used attempt to create the new cgroup
             as a sibling of the current one.  */
          यदि (तर्काः->cgroup_मार्गः == रिक्त)
            {
              libcrun_error_t tmp_err = रिक्त;
              पूर्णाङ्क tmp_ret;

              मुक्त (बहिर्गमनम्->मार्गः);
              बहिर्गमनम्->मार्गः = रिक्त;

              tmp_ret = make_new_sibling_cgroup (&बहिर्गमनम्->मार्गः, तर्काः->id, &tmp_err);
              यदि (UNLIKELY (tmp_ret < 0))
                {
                  crun_error_release (&tmp_err);
                  प्रत्यागम ret;
                }

              crun_error_release (err);
              ret = enable_controllers (बहिर्गमनम्->मार्गः, err);
            }
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }

  ret = libcrun_cgroup_ensure_not_frozen (बहिर्गमनम्->मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम enter_cgroup (cgroup_बहुलकम्, pid, 0, बहिर्गमनम्->मार्गः, true, err);
}

स्थिर पूर्णाङ्क
libcrun_destroy_cgroup_cgroupfs (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः,
                                 libcrun_error_t *err)
{
  पूर्णाङ्क बहुलकम्;
  पूर्णाङ्क ret;

  बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (बहुलकम् < 0))
    प्रत्यागम बहुलकम्;

  ret = cgroup_killall_मार्गः (cgroup_स्थितिः->मार्गः, SIGKILL, err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (err);

  प्रत्यागम destroy_cgroup_मार्गः (cgroup_स्थितिः->मार्गः, बहुलकम्, err);
}

संरचना libcrun_cgroup_manager cgroup_manager_cgroupfs = {
  .precreate_cgroup = libcrun_precreate_cgroup_cgroupfs,
  .create_cgroup = libcrun_cgroup_enter_cgroupfs,
  .destroy_cgroup = libcrun_destroy_cgroup_cgroupfs,
};
