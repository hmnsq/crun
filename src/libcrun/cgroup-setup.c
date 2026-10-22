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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-internal.h"
#ಅನ್ತರ್ಭಾವ "cgroup-systemd.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"
#ಅನ್ತರ್ಭಾವ "ebpf.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
initialize_cpuset_subsystem_rec (ಅಕ್ಷರ *ಮಾರ್ಗ, size_t ಮಾರ್ಗ_len, ಅಕ್ಷರ *cpus, ಅಕ್ಷರ *mems, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *allocated_cpus = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *allocated_mems = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ dirfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ mems_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ cpus_fd = -1;
  bool has_cpus = false, has_mems = false;
  ಪೂರ್ಣಾಂಕ b_len;

  dirfd = open (ಮಾರ್ಗ, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (dirfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ಮಾರ್ಗ);

  /* If we don't yet have a cpu/mem value to base the child off of, we attempt to read it.
   * Either, it's the newly created cgroup (and thus is still empty), or it's a parent that's been
   * prepopulated. */
  ಆದರೆ (cpus[0] == '\0')
    {
      cpus_fd = openat (dirfd, "cpuset.cpus", O_RDWR | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (cpus_fd < 0 && errno == ENOENT))
        cpus_fd = openat (dirfd, "cpus", O_RDWR | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (cpus_fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s/%s`", ಮಾರ್ಗ, "cpuset.cpus");

      b_len = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (cpus_fd, cpus, 256));
      ಆದರೆ (UNLIKELY (b_len < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from `cpuset.cpus`");
      cpus[b_len] = '\0';
      ಆದರೆ (cpus[0] == '\n')
        cpus[0] = '\0';
      ಆದರೆ (cpus[0] != '\0')
        has_cpus = true;
    }

  ಆದರೆ (mems[0] == '\0')
    {
      mems_fd = openat (dirfd, "cpuset.mems", O_RDWR | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (mems_fd < 0 && errno == ENOENT))
        mems_fd = openat (dirfd, "mems", O_RDWR | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (mems_fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s/%s`", ಮಾರ್ಗ, "cpuset.mems");

      b_len = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (mems_fd, mems, 256));
      ಆದರೆ (UNLIKELY (b_len < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from `cpuset.mems`");
      mems[b_len] = '\0';
      ಆದರೆ (mems[0] == '\n')
        mems[0] = '\0';
      ಆದರೆ (mems[0] != '\0')
        has_mems = true;
    }

  /* If we fail to find one, we should continue searching up until we find one */
  ಆದರೆ (cpus[0] == '\0' || mems[0] == '\0')
    {
      size_t ಪೋಷಕ_ಮಾರ್ಗ_len;
      ಪೂರ್ಣಾಂಕ ret;

      ಗಾಗಿ (ಪೋಷಕ_ಮಾರ್ಗ_len = ಮಾರ್ಗ_len - 1; ಪೋಷಕ_ಮಾರ್ಗ_len > 1 && ಮಾರ್ಗ[ಪೋಷಕ_ಮಾರ್ಗ_len] != '/'; ಪೋಷಕ_ಮಾರ್ಗ_len--)
        ;
      ಆದರೆ (ಪೋಷಕ_ಮಾರ್ಗ_len == 1)
        ಹಿಂತಿರುಗಿ 0;

      ಮಾರ್ಗ[ಪೋಷಕ_ಮಾರ್ಗ_len] = '\0';
      ret = initialize_cpuset_subsystem_rec (ಮಾರ್ಗ, ಪೋಷಕ_ಮಾರ್ಗ_len, cpus, mems, resources, err);
      ಮಾರ್ಗ[ಪೋಷಕ_ಮಾರ್ಗ_len] = '/';
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /* Ignore errors here and try to write the configuration we want later on.  */
          crun_error_release (err);
        }
    }

  /* If we know the resources, use them, instead of initializing with the full set, only to revert it later.
   * Only do so if we didn't read the cpus and mems we have from this cgroup.
   * Otherwise, we'll clobber existing values, which is problematic when there are multiple containers in a cgroup. */
  ಆದರೆ (resources && resources->cpu)
    {
      ಆದರೆ (resources->cpu->cpus && ! has_cpus)
        cpus = allocated_cpus = xstrdup (resources->cpu->cpus);
      ಆದರೆ (resources->cpu->mems && ! has_mems)
        mems = allocated_mems = xstrdup (resources->cpu->mems);
    }

  /* Finally, if we have a fd to populate, write the value chosen. If we have a value from the resources struct to base it off of,
   * use that, otherwise use the parent's. */
  ಆದರೆ (cpus_fd >= 0)
    {
      b_len = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (cpus_fd, cpus, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (cpus)));
      ಆದರೆ (UNLIKELY (b_len < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write `cpuset.cpus`");
    }

  ಆದರೆ (mems_fd >= 0)
    {
      b_len = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (mems_fd, mems, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (mems)));
      ಆದರೆ (UNLIKELY (b_len < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write `cpuset.mems`");
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
initialize_cpuset_subsystem (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *tmp_ಮಾರ್ಗ = xstrdup (ಮಾರ್ಗ);
  ಅಕ್ಷರ cpus_buf[257];
  ಅಕ್ಷರ mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ಹಿಂತಿರುಗಿ initialize_cpuset_subsystem_rec (tmp_ಮಾರ್ಗ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (tmp_ಮಾರ್ಗ), cpus_buf, mems_buf, ಖಾಲಿ, err);
}

ಪೂರ್ಣಾಂಕ
initialize_cpuset_subsystem_resources (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *tmp_ಮಾರ್ಗ = xstrdup (ಮಾರ್ಗ);
  ಅಕ್ಷರ cpus_buf[257];
  ಅಕ್ಷರ mems_buf[257];

  cpus_buf[0] = mems_buf[0] = '\0';
  ಹಿಂತಿರುಗಿ initialize_cpuset_subsystem_rec (tmp_ಮಾರ್ಗ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (tmp_ಮಾರ್ಗ), cpus_buf, mems_buf, resources, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
initialize_ಸ್ಮರಣೆ_subsystem (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ files[]
      = { "memory.limit_in_bytes", "memory.kmem.limit_in_bytes", "memory.memsw.limit_in_bytes", ಖಾಲಿ };
  cleanup_close ಪೂರ್ಣಾಂಕ dirfd = -1;
  ಪೂರ್ಣಾಂಕ i;

  dirfd = open (ಮಾರ್ಗ, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (dirfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ಮಾರ್ಗ);

  ಗಾಗಿ (i = 0; files[i]; i++)
    {
      ಪೂರ್ಣಾಂಕ ret;

      ret = write_cgroup_file (dirfd, files[i], "-1", 2, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /* Ignore any error here.  */
          crun_error_release (err);
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
enter_cgroup_v1_subsystem (pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subsystem, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool create_if_missing,
                           libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, subsystem, ಮಾರ್ಗ ? ಮಾರ್ಗ : "", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_ಮಾರ್ಗ, 0755, false, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (crun_error_get_errno (err) != EROFS)
            ಹಿಂತಿರುಗಿ crun_error_wrap (err, "creating cgroup directory `%s`", cgroup_ಮಾರ್ಗ);

          crun_error_release (err);
          ಹಿಂತಿರುಗಿ 0;
        }

      ಆದರೆ (strcmp (subsystem, "cpuset") == 0)
        {
          ret = initialize_cpuset_subsystem (cgroup_ಮಾರ್ಗ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
      ಆದರೆ (strcmp (subsystem, "memory") == 0)
        {
          ret = initialize_ಸ್ಮರಣೆ_subsystem (cgroup_ಮಾರ್ಗ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = crun_ಮಾರ್ಗ_exists (cgroup_ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      ಆದರೆ (ret == 0)
        ಹಿಂತಿರುಗಿ 0;
    }

  ಹಿಂತಿರುಗಿ move_process_ಗಮ್ಯ_cgroup (pid, subsystem, ಮಾರ್ಗ, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_file_owner (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, uid_t *uid, gid_t *gid)
{
  ರಚನೆ stat st;
  ಪೂರ್ಣಾಂಕ ret;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_STATX
  ರಚನೆ statx stx;

  ret = statx (AT_FDCWD, ಮಾರ್ಗ, AT_STATX_DONT_SYNC, STATX_UID | STATX_GID, &stx);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno == ENOSYS || errno == EINVAL)
        ಹೋಗು fallback;

      ಹಿಂತಿರುಗಿ ret;
    }
  *uid = stx.stx_uid;
  *gid = stx.stx_gid;
  ಹಿಂತಿರುಗಿ ret;

fallback:
#ಆದರೆ_ಅಂತ್ಯ
  ret = stat (ಮಾರ್ಗ, &st);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  *uid = st.st_uid;
  *gid = st.st_gid;
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
copy_owner (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *from, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗಮ್ಯ, libcrun_error_t *err)
{
  uid_t uid = 0;
  gid_t gid = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ret = get_file_owner (from, &uid, &gid);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot get file owner for `%s`", from);

  ಆದರೆ (uid == 0 && gid == 0)
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ chown_cgroups (ಗಮ್ಯ, uid, gid, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_unified_cgroup_pid (pid_t pid, ಅಕ್ಷರ **ಮಾರ್ಗ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಅಕ್ಷರ cgroup_ಮಾರ್ಗ[32];
  ಅಕ್ಷರ *from, *ಗಮ್ಯ;
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ len;

  len = snprintf (cgroup_ಮಾರ್ಗ, ಗಾತ್ರ (cgroup_ಮಾರ್ಗ), "/proc/%d/cgroup", pid);
  ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (cgroup_ಮಾರ್ಗ)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

  cgroup_ಮಾರ್ಗ[len] = '\0';

  ret = read_all_file (cgroup_ಮಾರ್ಗ, &content, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  from = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (content, "0::");
  ಆದರೆ (UNLIKELY (from == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot find cgroup2 for the process `%d`", pid);

  from += 3;

  ಗಮ್ಯ = strchr (from, '\n');
  ಆದರೆ (UNLIKELY (ಗಮ್ಯ == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot parse `%s`", cgroup_ಮಾರ್ಗ);
  *ಗಮ್ಯ = '\0';

  *ಮಾರ್ಗ = xstrdup (from);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
enter_cgroup_v1 (pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  bool entered_any = false;
  size_t content_ಗಾತ್ರ;
  ಅಕ್ಷರ *controller;
  ಅಕ್ಷರ *saveptr;
  bool has_ದತ್ತಾಂಶ;
  ಪೂರ್ಣಾಂಕ rootless;
  ಪೂರ್ಣಾಂಕ ret;

  rootless = is_rootless (err);
  ಆದರೆ (UNLIKELY (rootless < 0))
    ಹಿಂತಿರುಗಿ rootless;

  ret = read_all_file (PROC_SELF_CGROUP, &content, &content_ಗಾತ್ರ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಹಿಂತಿರುಗಿ 0;
        }
      ಹಿಂತಿರುಗಿ ret;
    }

  ಗಾಗಿ (has_ದತ್ತಾಂಶ = read_proc_cgroup (content, &saveptr, ಖಾಲಿ, &controller, ಖಾಲಿ);
       has_ದತ್ತಾಂಶ;
       has_ದತ್ತಾಂಶ = read_proc_cgroup (ಖಾಲಿ, &saveptr, ಖಾಲಿ, &controller, ಖಾಲಿ))
    {
      ಅಕ್ಷರ subsystem_ಮಾರ್ಗ[64];
      ಅಕ್ಷರ *subsystem;
      size_t len;

      ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (controller, "name="))
        controller += 5;

      subsystem = controller[0] == '\0' ? "unified" : controller;

      ಆದರೆ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ಆದರೆ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      len = snprintf (subsystem_ಮಾರ್ಗ, ಗಾತ್ರ (subsystem_ಮಾರ್ಗ), CGROUP_ಬೇರು "/%s", subsystem);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (subsystem_ಮಾರ್ಗ)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = crun_ಮಾರ್ಗ_exists (subsystem_ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      ಆದರೆ (ret == 0)
        ಮುಂದುವರಿ;

      entered_any = true;
      ret = enter_cgroup_v1_subsystem (pid, subsystem, ಮಾರ್ಗ, create_if_missing, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಂಕ errcode = crun_error_get_errno (err);
          ಆದರೆ (rootless && (errcode == EACCES || errcode == EPERM))
            {
              crun_error_release (err);
              ಮುಂದುವರಿ;
            }
          ಹಿಂತಿರುಗಿ ret;
        }
    }

  ಆದರೆ (entered_any)
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not join cgroup");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
enter_cgroup_v2 (pid_t pid, pid_t init_pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, bool create_if_missing, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ_procs = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
  ಅಕ್ಷರ pid_str[16];
  ಪೂರ್ಣಾಂಕ repeat;
  ಪೂರ್ಣಾಂಕ len;
  ಪೂರ್ಣಾಂಕ ret;

  len = snprintf (pid_str, ಗಾತ್ರ (pid_str), "%d", pid);
  ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (pid_str)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (create_if_missing)
    {
      ret = crun_ensure_directory (cgroup_ಮಾರ್ಗ, 0755, false, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = append_paths (&cgroup_ಮಾರ್ಗ_procs, err, cgroup_ಮಾರ್ಗ, "cgroup.procs", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = write_cgroup_file (AT_FDCWD, cgroup_ಮಾರ್ಗ_procs, pid_str, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (pid_str), err);
  ಆದರೆ (LIKELY (ret >= 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (UNLIKELY (crun_error_get_errno (err) == EOPNOTSUPP))
    {
      crun_error_release (err);

      ret = maybe_make_cgroup_threaded (ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_ಮಾರ್ಗ_procs, pid_str, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (pid_str), err);
      ಆದರೆ (LIKELY (ret >= 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (create_if_missing || crun_error_get_errno (err) != EBUSY)
    ಹಿಂತಿರುಗಿ ret;

  crun_error_release (err);

  /* There are subdirectories so it is not possible to join the initial
     cgroup.  Create a subdirectory and use that.
     It can still fail if the container creates a subdirectory under
     /sys/fs/cgroup/../crun-exec/  */
  ಗಾಗಿ (repeat = 0;; repeat++)
    {
      cleanup_free ಅಕ್ಷರ *cgroup_crun_exec_ಮಾರ್ಗ = ಖಾಲಿ;
      cleanup_free ಅಕ್ಷರ *cgroup_sub_ಮಾರ್ಗ_procs = ಖಾಲಿ;

      /* There is an init pid, try to join its cgroup.  */
      ಆದರೆ (init_pid > 0)
        {
          ret = read_unified_cgroup_pid (init_pid, &cgroup_crun_exec_ಮಾರ್ಗ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          /* Make sure the cgroup is below the initial cgroup specified for the container.  */
          ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (ಮಾರ್ಗ, cgroup_crun_exec_ಮಾರ್ಗ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮಾರ್ಗ)))
            {
              ಬಿಡುಗಡೆ (cgroup_crun_exec_ಮಾರ್ಗ);
              cgroup_crun_exec_ಮಾರ್ಗ = ಖಾಲಿ;
            }
        }

      /* There is no init_pid to lookup, try a static path.  */
      ಆದರೆ (cgroup_crun_exec_ಮಾರ್ಗ == ಖಾಲಿ)
        xasprintf (&cgroup_crun_exec_ಮಾರ್ಗ, "%s/crun-exec", ಮಾರ್ಗ);

      /* The sub-cgroup is managed by the container, make sure it is not
         frozen or the process would never run once moved there.  */
      ret = libcrun_cgroup_ensure_not_frozen (cgroup_crun_exec_ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = append_paths (&cgroup_sub_ಮಾರ್ಗ_procs, err, CGROUP_ಬೇರು, cgroup_crun_exec_ಮಾರ್ಗ, "cgroup.procs", ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = write_cgroup_file (AT_FDCWD, cgroup_sub_ಮಾರ್ಗ_procs, pid_str, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (pid_str), err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /* The init process might have moved to a different cgroup, try again.  */
          ಆದರೆ (crun_error_get_errno (err) == EBUSY && init_pid && repeat < 20)
            {
              crun_error_release (err);
              ಮುಂದುವರಿ;
            }
          ಹಿಂತಿರುಗಿ ret;
        }
      ಹಿಂತಿರುಗಿ copy_owner (cgroup_ಮಾರ್ಗ_procs, cgroup_crun_exec_ಮಾರ್ಗ, err);
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
enter_cgroup (ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ, pid_t pid, pid_t init_pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
              bool create_if_missing, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
    {
      ret = enter_cgroup_v2 (pid, init_pid, ಮಾರ್ಗ, create_if_missing, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = enter_cgroup_v1 (pid, ಮಾರ್ಗ, create_if_missing, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ 0;
}
