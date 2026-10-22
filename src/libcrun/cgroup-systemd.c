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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-internal.h"
#ಅನ್ತರ್ಭಾವ "cgroup-systemd.h"
#ಅನ್ತರ್ಭಾವ "cgroup-resources.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"
#ಅನ್ತರ್ಭಾವ "ebpf.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <linux/magic.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-bus.h>
#  include <limits.h>

#  define SYSTEMD_PROPERTY_PREFIX "org.systemd.property."

#  define CGROUP_BFQ_WEIGHT_MIN ((uint64_t) 1)
#  define CGROUP_BFQ_WEIGHT_DEFAULT ((uint64_t) 100)
#  define CGROUP_BFQ_WEIGHT_MAX ((uint64_t) 1000)

#  define CGROUP_WEIGHT_MIN ((uint64_t) 1)
#  define CGROUP_WEIGHT_DEFAULT ((uint64_t) 100)
#  define CGROUP_WEIGHT_MAX ((uint64_t) 10000)

/* The CPU quota period used by the kernel (and systemd) by default, in us.  */
#  define DEFAULT_CPU_QUOTA_PERIOD ((uint64_t) 100000)

#  define SYSTEMD_MISSING_PROPERTIES_DIR ".cache/systemd-missing-properties"

#  define IS_WILDCARD(x) (x <= 0)

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
register_missing_property_from_message (ಧ್ರುವ ವರ್ಣ *state_dir, ಧ್ರುವ ವರ್ಣ *message, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *file_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *dir_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *property = ರಿಕ್ತ;
  ವರ್ಣ *p;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (! has_prefix (message, "Cannot set property "))
    ಪ್ರತ್ಯಾಗಮ 0;

  property = xstrdup (message + ಪರಿಮಾಣ ("Cannot set property ") - 1);
  p = strchr (property, ',');
  ಯದಿ (! p)
    ಪ್ರತ್ಯಾಗಮ 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_paths (&file_path, err, dir_path, property, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, ರಿಕ್ತ, 0, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ bool
property_missing_p (ವರ್ಣ **missing_properties, ಧ್ರುವ ವರ್ಣ *property)
{
  size_t i;

  ಪ್ರತಿ (i = 0; missing_properties && missing_properties[i]; i++)
    ಯದಿ (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        ಪ್ರತ್ಯಾಗಮ true;
      }

  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
get_systemd_scope_and_slice (ಧ್ರುವ ವರ್ಣ *id, bool user_slice, ಧ್ರುವ ವರ್ಣ *cgroup_path,
                             ವರ್ಣ **scope, ವರ್ಣ **slice)
{
  ಧ್ರುವ ವರ್ಣ *n;
  ವರ್ಣ *p;

  ಯದಿ (cgroup_path == ರಿಕ್ತ || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      ಪ್ರತ್ಯಾಗಮ;
    }

  n = strchr (cgroup_path, ':');
  ಯದಿ (n == ರಿಕ್ತ)
    xasprintf (scope, "%s.scope", cgroup_path);
  ಅನ್ಯಥಾ
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      ಯದಿ (p)
        *p = '-';
    }
  ಯದಿ (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      ಯದಿ (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      ಯದಿ (is_empty_string (*slice))
        {
          ಮುಕ್ತ (*slice);
          ಯದಿ (user_slice)
            *slice = xstrdup ("user.slice");
          ಅನ್ಯಥಾ
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd = -1;
  bool need_set_parent = true;
  ವರ್ಣ fmt_buf[64];
  ಪೂರ್ಣಾಙ್ಕ len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (resources == ರಿಕ್ತ || resources->cpu == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  ಯದಿ (is_empty_string (path))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  ಯದಿ (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, "cpu", path, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ಯದಿ (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      ಯದಿ (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, ಪರಿಮಾಣ (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buf)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      ಯದಿ (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ parent;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (resources == ರಿಕ್ತ || resources->cpu == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತಿ (parent = 0; parent < 2; parent++)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd = -1;

      ಯದಿ (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, path ? path : "", (parent ? ".." : ರಿಕ್ತ), ರಿಕ್ತ);
      ಅನ್ಯಥಾ
        ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, "/cpu", path ? path : "", (parent ? ".." : ರಿಕ್ತ), ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ಯದಿ (UNLIKELY (dirfd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      ಯದಿ (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ parent;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ levels;

  ret = append_paths (&cgroup_path, err, CGROUP_ಬೇರು, "/cpuset", path ? path : "", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  ಪ್ರತಿ (parent = 0; parent < levels; parent++)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd_cpuset = -1;
      cleanup_free ವರ್ಣ *path_to_cpuset = ರಿಕ್ತ;

      ret = append_paths (&path_to_cpuset, err, CGROUP_ಬೇರು "/cpuset", path, (parent ? ".." : ರಿಕ್ತ), ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ಯದಿ (UNLIKELY (dirfd_cpuset < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compute_finalized_path (ವರ್ಣ **path, ಧ್ರುವ ವರ್ಣ *from, ಧ್ರುವ ವರ್ಣ *suffix, libcrun_error_t *err)
{
  ಯದಿ (suffix == ರಿಕ್ತ)
    {
      *path = xstrdup (from);
      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ಪ್ರತ್ಯಾಗಮ append_paths (path, err, from, suffix, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
systemd_finalize (ಸಂರಚನಾ libcrun_cgroup_args *args, ವರ್ಣ **path_out,
                  ಪೂರ್ಣಾಙ್ಕ cgroup_mode, ಧ್ರುವ ವರ್ಣ *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  pid_t pid = args->pid;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ವರ್ಣ *from, *to;
  ವರ್ಣ *saveptr = ರಿಕ್ತ;

  cleanup_free ವರ್ಣ *proc_path = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ fd;

  ret = read_all_fd (fd, "cgroup path", &content, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ವಿಕಲ್ಪ (cgroup_mode)
    {
    ಪಕ್ಷ CGROUP_MODE_HYBRID:
    ಪಕ್ಷ CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      ಯದಿ (LIKELY (from != ರಿಕ್ತ))
        from += 8;
      ಅನ್ಯಥಾ
        {
          from = strstr (content, ":pids:");
          ಯದಿ (UNLIKELY (from == ರಿಕ್ತ))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      ಯದಿ (UNLIKELY (to == ರಿಕ್ತ))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (geteuid ())
        ಪ್ರತ್ಯಾಗಮ 0;

      ret = setup_rt_runtime (resources, path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಪ್ರತಿ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ರಿಕ್ತ, "\n", &saveptr))
        {
          ವರ್ಣ *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          ಯದಿ (subsystem[0] == '\0')
            {
              ಯದಿ (cgroup_mode == CGROUP_MODE_LEGACY)
                ಅನುವರ್ತ;

              subsystem = "unified";
            }

          ಯದಿ (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              ಯದಿ (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  ಯದಿ (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      ಅನುವರ್ತ;
                    }
                  ಪ್ರತ್ಯಾಗಮ ret;
                }
            }
        }

      ಭಙ್ಗ;

    ಪಕ್ಷ CGROUP_MODE_UNIFIED:
      {
        cleanup_free ವರ್ಣ *dir = ರಿಕ್ತ;

        from = strstr (content, "0::");
        ಯದಿ (UNLIKELY (from == ರಿಕ್ತ))
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        ಯದಿ (UNLIKELY (to == ರಿಕ್ತ))
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        ret = append_paths (&dir, err, CGROUP_ಬೇರು, path, ರಿಕ್ತ);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        ret = move_process_to_cgroup (pid, ರಿಕ್ತ, path, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        ret = enable_controllers (path, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        ಯದಿ (suffix)
          {
            ret = chown_cgroups (path, args->ಬೇರು_uid, args->ಬೇರು_gid, err);
            ಯದಿ (UNLIKELY (ret < 0))
              ಪ್ರತ್ಯಾಗಮ ret;
          }
      }
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  *path_out = path;
  path = ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ systemd_job_removed_s
{
  ಧ್ರುವ ವರ್ಣ *path;
  ಧ್ರುವ ವರ್ಣ *op;
  ಪೂರ್ಣಾಙ್ಕ terminated;
  libcrun_error_t err;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
systemd_job_removed (sd_bus_message *m, ಶೂನ್ಯ *userdata, sd_bus_error *error arg_unused)
{
  ಧ್ರುವ ವರ್ಣ *path, *unit, *result;
  uint32_t id;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಸಂರಚನಾ systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ -1;

  ಯದಿ (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      ಯದಿ (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          ಪ್ರತ್ಯಾಗಮ -1;
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
systemd_check_job_status_setup (sd_bus *bus, ಸಂರಚನಾ systemd_job_removed_s *data, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = sd_bus_match_signal_async (bus, ರಿಕ್ತ, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, ರಿಕ್ತ, data);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "sd-bus match signal");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
systemd_check_job_status (sd_bus *bus, ಸಂರಚನಾ systemd_job_removed_s *data, ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *op,
                          libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ sd_err;

  data->path = path;
  data->op = op;
  ಯಾವತ್ (! data->terminated)
    {
      sd_err = sd_bus_process (bus, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus process");

      ಯದಿ (sd_err != 0)
        ಅನುವರ್ತ;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus wait");
    }

  ಯದಿ (data->err != ರಿಕ್ತ)
    {
      *err = data->err;
      ಪ್ರತ್ಯಾಗಮ -1;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
parse_sd_array (ವರ್ಣ *s, ವರ್ಣ **out, ವರ್ಣ **next, libcrun_error_t *err)
{
  ವರ್ಣ endchr;
  ವರ್ಣ *it, *dest;
  bool escaped = false;

  *out = ರಿಕ್ತ;
  *next = ರಿಕ್ತ;

  ಯಾವತ್ (isspace (*s))
    s++;
  ಯದಿ (*s == '\0')
    ಪ್ರತ್ಯಾಗಮ 0;
  ಅನ್ಯಥಾ ಯದಿ (*s != '\'' && *s != '"')
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  ಯಾವತ್ (1)
    {
      ಯದಿ (*it == '\0')
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid string `%s`", s);
      ಯದಿ (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          ಯಾವತ್ (isspace (*it))
            it++;
          ಯದಿ (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              ಪ್ರತ್ಯಾಗಮ 0;
            }

          ಯದಿ (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              ಪ್ರತ್ಯಾಗಮ 0;
            }

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      ಯದಿ (! escaped)
        *dest++ = *it;
      it++;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
append_systemd_annotation (sd_bus_message *m, ಧ್ರುವ ವರ್ಣ *name, size_t name_len, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
                           libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *tmp_name = ರಿಕ್ತ;
  uint32_t factor = 1;
  ಧ್ರುವ ವರ್ಣ *it;
  ಪೂರ್ಣಾಙ್ಕ sd_err;

  ಯಾವತ್ (*ಮೌಲ್ಯ == ' ')
    ಮೌಲ್ಯ++;

  it = ಮೌಲ್ಯ;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  ಯದಿ (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
      && name[name_len - 1] == 'c')
    {
      factor = 1000000;

      tmp_name = xmalloc (name_len + 2);
      memcpy (tmp_name, name, name_len - 3);
      memcpy (tmp_name + name_len - 3, "USec", 5);

      name = tmp_name;
    }

  /* A *USec property is always an uint64, so, like runc does, accept any
     number of seconds (possibly with a fractional part, and with an
     optional integer type), and convert it to uint64 microseconds.  */
  ಯದಿ (factor != 1)
    {
      ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", ರಿಕ್ತ };
      ಧ್ರುವ ವರ್ಣ *ಅಂಕೆ = it;
      ವರ್ಣ *endptr = ರಿಕ್ತ;
      ದ್ವಿಗುಣ v;
      size_t i;

      ಪ್ರತಿ (i = 0; types[i]; i++)
        ಯದಿ (has_prefix (ಅಂಕೆ, types[i]))
          {
            ಅಂಕೆ += strlen (types[i]);
            ಭಙ್ಗ;
          }

      errno = 0;
      v = strtod (ಅಂಕೆ, &endptr);
      ಯದಿ (UNLIKELY (errno != 0 || endptr == ಅಂಕೆ || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, ಮೌಲ್ಯ);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಯದಿ ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ಅನ್ಯಥಾ ಯದಿ (*it == '\'')
    {
      cleanup_free ವರ್ಣ *v_start = ರಿಕ್ತ;
      ವರ್ಣ *end;

      it = v_start = xstrdup (ಮೌಲ್ಯ);

      end = strchr (v_start + 1, '\'');
      ಯದಿ (end == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid variant `%s`", ಮೌಲ್ಯ);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ಅನ್ಯಥಾ ಯದಿ (*it == '[')
    {
      cleanup_free ವರ್ಣ *v_start = ರಿಕ್ತ;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free ವರ್ಣ **parts = xmalloc (ಪರಿಮಾಣ (ವರ್ಣ *) * parts_size);
      ವರ್ಣ *part;

      part = v_start = xstrdup (it + 1);
      ಯಾವತ್ (1)
        {
          ವರ್ಣ *out = ರಿಕ್ತ, *next = ರಿಕ್ತ;
          ಪೂರ್ಣಾಙ್ಕ ret;

          ret = parse_sd_array (part, &out, &next, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          parts[n_parts++] = out;
          ಯದಿ (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, ಪರಿಮಾಣ (ವರ್ಣ *) * parts_size);
            }
          parts[n_parts] = ರಿಕ್ತ;
          ಯದಿ (next == ರಿಕ್ತ)
            ಭಙ್ಗ;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus close container");

      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ಅನ್ಯಥಾ ಯದಿ (has_prefix (it, "uint64 "))
    {
      ವರ್ಣ *endptr = ರಿಕ್ತ;
      uint64_t v;

      errno = 0;
      v = strtoull (it + ಪರಿಮಾಣ ("uint64"), &endptr, 10);
      ಯದಿ (UNLIKELY (errno != 0 || *endptr))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ಅನ್ಯಥಾ ಯದಿ (has_prefix (it, "int64 "))
    {
      ವರ್ಣ *endptr = ರಿಕ್ತ;
      int64_t v;

      errno = 0;
      v = strtoll (it + ಪರಿಮಾಣ ("int64"), &endptr, 10);
      ಯದಿ (UNLIKELY (errno != 0 || *endptr))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ಅನ್ಯಥಾ ಯದಿ (has_prefix (it, "uint32 "))
    {
      ವರ್ಣ *endptr = ರಿಕ್ತ;
      uint32_t v;

      errno = 0;
      v = strtoul (it + ಪರಿಮಾಣ ("uint32"), &endptr, 10);
      ಯದಿ (UNLIKELY (errno != 0 || *endptr))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ಅನ್ಯಥಾ ಯದಿ (has_prefix (it, "int32 ") || strchr (it, ' ') == ರಿಕ್ತ)
    {
      ವರ್ಣ *endptr = ರಿಕ್ತ;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      ಯದಿ (has_prefix (it, "int32 "))
        v = strtol (it + ಪರಿಮಾಣ ("int32"), &endptr, 10);
      ಅನ್ಯಥಾ
        v = strtol (it, &endptr, 10);
      ಯದಿ (UNLIKELY (errno != 0 || *endptr))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "unknown type for `%s`", name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ rootless;
  ಪೂರ್ಣಾಙ್ಕ sd_err = 0;

  rootless = is_rootless (err);
  ಯದಿ (UNLIKELY (rootless < 0))
    ಪ್ರತ್ಯಾಗಮ rootless;

  ಯದಿ (rootless)
    sd_err = sd_bus_default_user (bus);
  ಯದಿ (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  ಯದಿ (sd_err < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "cannot open sd-bus");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ಧ್ರುವ ವರ್ಣ *name)
{
  size_t i;

  ಯದಿ (resources == ರಿಕ್ತ || resources->unified == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಪ್ರತಿ (i = 0; i < resources->unified->len; i++)
    ಯದಿ (strcmp (resources->unified->ಕೀಲಿಗಳು[i], name) == 0)
      ಪ್ರತ್ಯಾಗಮ resources->unified->ಮೌಲ್ಯಗಳು[i];
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_ಮೌಲ್ಯ_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ಧ್ರುವ ವರ್ಣ *name,
                            uint64_t *ಮೌಲ್ಯ, libcrun_error_t *err)
{
  ವರ್ಣ *endptr = ರಿಕ್ತ;
  size_t i;

  ಯದಿ (resources == ರಿಕ್ತ || resources->unified == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತಿ (i = 0; i < resources->unified->len; i++)
    ಯದಿ (strcmp (resources->unified->ಕೀಲಿಗಳು[i], name) == 0)
      {
        ಯದಿ (strcmp (resources->unified->ಮೌಲ್ಯಗಳು[i], "max") == 0)
          {
            *ಮೌಲ್ಯ = UINT64_MAX;
            ಪ್ರತ್ಯಾಗಮ 1;
          }

        errno = 0;
        *ಮೌಲ್ಯ = strtoull (resources->unified->ಮೌಲ್ಯಗಳು[i], &endptr, 10);
        ಯದಿ (UNLIKELY (errno))
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->ಮೌಲ್ಯಗಳು[i]);
        ಯದಿ (endptr && *endptr)
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->ಮೌಲ್ಯಗಳು[i]);
        ಪ್ರತ್ಯಾಗಮ 1;
      }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
get_memory_ಕೆಳಗಿನ (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ಯದಿ (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      ಪ್ರತ್ಯಾಗಮ 1;
    }

  ಪ್ರತ್ಯಾಗಮ get_ಮೌಲ್ಯ_from_unified_map (resources, "memory.low", limit, err);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ಯದಿ (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      ಪ್ರತ್ಯಾಗಮ 1;
    }

  ಪ್ರತ್ಯಾಗಮ get_ಮೌಲ್ಯ_from_unified_map (resources, "pids.max", limit, err);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ಯದಿ (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      ಪ್ರತ್ಯಾಗಮ 1;
    }

  ಪ್ರತ್ಯಾಗಮ get_ಮೌಲ್ಯ_from_unified_map (resources, "memory.max", limit, err);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ಯದಿ (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      ಯದಿ (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          ಪ್ರತ್ಯಾಗಮ 1;
        }
      /* 0 means unset.  */
      ಯದಿ (memory->swap == 0)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಯದಿ (mem <= 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ಯದಿ (memory->swap < mem)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      ಪ್ರತ್ಯಾಗಮ 1;
    }

  ಪ್ರತ್ಯಾಗಮ get_ಮೌಲ್ಯ_from_unified_map (resources, "memory.swap.max", limit, err);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t ಮೌಲ್ಯ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = get_ಮೌಲ್ಯ_from_unified_map (resources, "cpu.idle", &ಮೌಲ್ಯ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  ಯದಿ (ret > 0 && ಮೌಲ್ಯ == 1)
    has_idle = true;
  ಯದಿ (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_ಮೌಲ್ಯ_from_unified_map (resources, "cpu.weight", weight, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  ಯದಿ (ret > 0)
    {
      ಯದಿ (has_idle)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      ಪ್ರತ್ಯಾಗಮ 1;
    }

  ಯದಿ (resources->cpu && resources->cpu->shares_present)
    {
      ಯದಿ (has_idle)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      ಯದಿ (resources->cpu->shares == 0)
        ಪ್ರತ್ಯಾಗಮ 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      ಪ್ರತ್ಯಾಗಮ 1;
    }
  ಯದಿ (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      ಪ್ರತ್ಯಾಗಮ 1;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_uint64_ಮೌಲ್ಯ (ಧ್ರುವ ವರ್ಣ *str, uint64_t *ಮೌಲ್ಯ, libcrun_error_t *err)
{
  ವರ್ಣ *endptr = ರಿಕ್ತ;

  errno = 0;
  *ಮೌಲ್ಯ = strtoull (str, &endptr, 10);
  ಯದಿ (UNLIKELY (errno))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "invalid value: %s", str);
  ಯದಿ (endptr && *endptr)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid value: %s", str);
  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  ಪ್ರತ್ಯಾಗಮ bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
append_io_weight (sd_bus_message *m, ವರ್ಣ **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  ಪೂರ್ಣಾಙ್ಕ sd_err;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

#  define APPEND_IO_WEIGHT(ಮೌಲ್ಯ)                                                      \
    ಕುರು                                                                                 \
      {                                                                                \
        ಯದಿ (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            ಯದಿ (UNLIKELY (sd_err < 0))                                                 \
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } ಯಾವತ್ (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, ಮೌಲ್ಯ)                                                      \
    ಕುರು                                                                                                \
      {                                                                                               \
        ಯದಿ (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            ಯದಿ (UNLIKELY (sd_err < 0))                                                                \
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } ಯಾವತ್ (0)

  ಯದಿ (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  ಪ್ರತಿ (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      ವರ್ಣ name[64];
      ಪೂರ್ಣಾಙ್ಕ len;

      len = snprintf (name, ಪರಿಮಾಣ (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (name)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  ಪ್ರತಿ (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free ವರ್ಣ *ಮೌಲ್ಯ = ರಿಕ್ತ;
      bool need_conversion = false;
      ವರ್ಣ *saveptr = ರಿಕ್ತ;
      ವರ್ಣ *ಸಾಲು;

      ಯದಿ (strcmp (resources->unified->ಕೀಲಿಗಳು[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      ಅನ್ಯಥಾ ಯದಿ (strcmp (resources->unified->ಕೀಲಿಗಳು[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      ಅನ್ಯಥಾ
        {
          /* Skip other keys.  */
          ಅನುವರ್ತ;
        }

      ಮೌಲ್ಯ = xstrdup (resources->unified->ಮೌಲ್ಯಗಳು[i]);

      ಪ್ರತಿ (ಸಾಲು = strtok_r (ಮೌಲ್ಯ, "\n", &saveptr); ಸಾಲು; ಸಾಲು = strtok_r (ರಿಕ್ತ, "\n", &saveptr))
        {
          ವರ್ಣ *sep;

          sep = strchr (ಸಾಲು, ' ');
          ಯಾವತ್ (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_ಮೌಲ್ಯ (sep ? sep : ಸಾಲು, &weight, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          ಯದಿ (need_conversion)
            weight = IO_WEIGHT (weight);

          ಯದಿ (sep == ರಿಕ್ತ || strcmp (ಸಾಲು, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          ಅನ್ಯಥಾ
            APPEND_IO_DEVICE_WEIGHT (ಸಾಲು, weight);
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Adapted from systemd.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
bus_append_byte_array (sd_bus_message *m, ಧ್ರುವ ವರ್ಣ *field, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t n, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
append_uint64_from_unified_map (sd_bus_message *m,
                                ವರ್ಣ **missing_properties,
                                ಧ್ರುವ ವರ್ಣ *attr,
                                ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t ಮೌಲ್ಯ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (property_missing_p (missing_properties, attr))
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = get_ಮೌಲ್ಯ_from_unified_map (resources, ಕೀಲಿ, &ಮೌಲ್ಯ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  ಯದಿ (ret)
    {
      ಪೂರ್ಣಾಙ್ಕ sd_err = sd_bus_message_append (m, "(sv)", attr, "t", ಮೌಲ್ಯ);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
append_device_allow (sd_bus_message *m,
                     ಧ್ರುವ ವರ್ಣ type,
                     ಪೂರ್ಣಾಙ್ಕ major,
                     ಪೂರ್ಣಾಙ್ಕ minor,
                     ಧ್ರುವ ವರ್ಣ *access,
                     libcrun_error_t *err)
{
  ವರ್ಣ device[64];
  ಪೂರ್ಣಾಙ್ಕ sd_err;
  ಪೂರ್ಣಾಙ್ಕ len;

  ಯದಿ (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಯದಿ (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, ಪರಿಮಾಣ (device), "%s-*", type == 'c' ? "char" : "block");
  ಅನ್ಯಥಾ ಯದಿ (IS_WILDCARD (minor))
    len = snprintf (device, ಪರಿಮಾಣ (device), type == 'c' ? "char-%d" : "block-%d", major);
  ಅನ್ಯಥಾ
    len = snprintf (device, ಪರಿಮಾಣ (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (device)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  ಯದಿ (UNLIKELY (sd_err < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  ಪ್ರತ್ಯಾಗಮ 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
access_to_mode (ಧ್ರುವ ವರ್ಣ *access, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ mode = 0;

  ಯಾವತ್ (access && *access)
    {
      ಪೂರ್ಣಾಙ್ಕ c = *access++;

      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'r':
          mode |= MODE_R;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'w':
          mode |= MODE_W;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'm':
          mode |= MODE_M;
          ಭಙ್ಗ;

        ಸ್ವಭಾವ:
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  ಪ್ರತ್ಯಾಗಮ mode;
}

/* check if the type b is included in type a.  */
ಸ್ಥಿರ bool
has_same_type (ಧ್ರುವ ವರ್ಣ *a, ಧ್ರುವ ವರ್ಣ *b)
{
  bool a_all = a == ರಿಕ್ತ || strcmp (a, "a") == 0;
  ಯದಿ (b == ರಿಕ್ತ || strcmp (b, "a") == 0)
    ಪ್ರತ್ಯಾಗಮ a_all;

  ಪ್ರತ್ಯಾಗಮ a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ req_mode;

#  define CMP_DEV_ಅಂಕೆ(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  ಯದಿ (UNLIKELY (req_mode < 0))
    ಪ್ರತ್ಯಾಗಮ req_mode;

  ಪ್ರತಿ (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      ಪೂರ್ಣಾಙ್ಕ mode;

      ಯದಿ (! CMP_DEV_ಅಂಕೆ (devices[n]->minor, devices[current_rule]->minor))
        ಅನುವರ್ತ;
      ಯದಿ (! CMP_DEV_ಅಂಕೆ (devices[n]->major, devices[current_rule]->major))
        ಅನುವರ್ತ;

      ಯದಿ (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        ಅನುವರ್ತ;

      mode = access_to_mode (devices[i]->access, err);
      ಯದಿ (UNLIKELY (mode < 0))
        ಪ್ರತ್ಯಾಗಮ mode;

      ಯದಿ (devices[current_rule]->allow && (mode & req_mode))
        ಪ್ರತ್ಯಾಗಮ 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      ಯದಿ (req_mode == 0)
        ಪ್ರತ್ಯಾಗಮ 1;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
#  undef CMP_DEV_ಅಂಕೆ
}

ಸ್ಥಿರ size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  ಯದಿ (n == 0)
    ಪ್ರತ್ಯಾಗಮ 1;

  /* Find the first rule that is after the last "block all".  */
  ಪ್ರತಿ (i = n - 1; i-- > 0;)
    {
      ಯದಿ ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        ಪ್ರತ್ಯಾಗಮ i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  ಪ್ರತಿ (i = 0; i < n; i++)
    ಯದಿ (devices[i]->allow)
      ಪ್ರತ್ಯಾಗಮ i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  ಪ್ರತ್ಯಾಗಮ n + 1;
}

ಸ್ಥಿರ bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  ಪೂರ್ಣಾಙ್ಕ remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  ಪ್ರತಿ (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      ಯದಿ (! devices[current_rule]->allow)
        ಪ್ರತ್ಯಾಗಮ false;

      ಯದಿ (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          ಯದಿ (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            ಪ್ರತ್ಯಾಗಮ true;

          ಯದಿ (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          ಅನ್ಯಥಾ ಯದಿ (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          ಯದಿ (remaining == 0)
            ಪ್ರತ್ಯಾಗಮ true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  ಸಂರಚನಾ default_dev_s *default_devices = get_default_devices ();
  ಪೂರ್ಣಾಙ್ಕ ret, sd_err;
  size_t i;

  ಯದಿ (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      ಪ್ರತ್ಯಾಗಮ 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  ಯದಿ (UNLIKELY (sd_err < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  ಪ್ರತಿ (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತಿ (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      ವರ್ಣ type;

      ಯದಿ (! d->allow)
        {
          ಪೂರ್ಣಾಙ್ಕ redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          ಯದಿ (UNLIKELY (redundant < 0))
            ಪ್ರತ್ಯಾಗಮ redundant;

          ಯದಿ (redundant)
            ಅನುವರ್ತ;

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      ಯದಿ (d->type == ರಿಕ್ತ || strcmp (d->type, "a") == 0)
        type = 'a';
      ಅನ್ಯಥಾ ಯದಿ (strcmp (d->type, "c") == 0)
        type = 'c';
      ಅನ್ಯಥಾ ಯದಿ (strcmp (d->type, "b") == 0)
        type = 'b';
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "unknown device type `%s`", d->type);

      ಯದಿ (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಅನ್ಯಥಾ
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ bool
has_bpf_fs ()
{
  ಸಂರಚನಾ statfs stat;

  ಪ್ರತ್ಯಾಗಮ statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
bpfprog_path_from_scope (ವರ್ಣ **path, ಧ್ರುವ ವರ್ಣ *scope, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *flat_scope = xstrdup (scope);
  ವರ್ಣ *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  ಯಾವತ್ ((it = strchr (it, '.')) != ರಿಕ್ತ)
    *it = '_';

  ಪ್ರತ್ಯಾಗಮ append_paths (path, err, CRUN_BPF_DIR, flat_scope, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 ವರ್ಣ **missing_properties,
                 ಧ್ರುವ ವರ್ಣ *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret = 0;
  ಪೂರ್ಣಾಙ್ಕ in_userns, sd_err;
  ಧ್ರುವ ವರ್ಣ *property_name = "BPFProgram";
  cleanup_free ಸಂರಚನಾ bpf_program *program = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  ಯದಿ (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    ಪ್ರತ್ಯಾಗಮ 0;

  in_userns = check_running_in_user_namespace (err);
  ಯದಿ (UNLIKELY (in_userns < 0))
    ಪ್ರತ್ಯಾಗಮ in_userns;

  ಯದಿ (in_userns)
    ಪ್ರತ್ಯಾಗಮ 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  ಯದಿ (UNLIKELY (program == ರಿಕ್ತ))
    {
      crun_error_release (err);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  ಯದಿ (UNLIKELY (ret < 0 && errno != EEXIST))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  ಯದಿ (is_update)
    {
      cleanup_free ಸಂರಚನಾ bpf_program *program_loaded = ರಿಕ್ತ;

      ಯದಿ (resources->devices == ರಿಕ್ತ || resources->devices_len == 0)
        {
          *devices_set = true;
          ಪ್ರತ್ಯಾಗಮ 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      /* it is the same as the loaded program.  */
      ಯದಿ (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          ಪ್ರತ್ಯಾಗಮ 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free ಸಂರಚನಾ bpf_program *program_new = ರಿಕ್ತ;
        ವರ್ಣ tmp[64];
        ಪೂರ್ಣಾಙ್ಕ len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, ಪರಿಮಾಣ (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (tmp)))
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        ಯದಿ (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            ಪ್ರತ್ಯಾಗಮ 0;
          }
      }

      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  ಯದಿ (UNLIKELY (sd_err < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
append_cpu_quota (sd_bus_message *m, ವರ್ಣ **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ಧ್ರುವ ವರ್ಣ *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  ಪೂರ್ಣಾಙ್ಕ sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ಯದಿ (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      ಪೂರ್ಣಾಙ್ಕ ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      ಯದಿ (has_period)
        period = max_period;
    }

  ಯದಿ (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  ಯದಿ ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      ಯದಿ (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          ಯದಿ (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      ಯದಿ (UNLIKELY (sd_err < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
append_resources (sd_bus_message *m,
                  bool is_update,
                  ಧ್ರುವ ವರ್ಣ *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  ಪೂರ್ಣಾಙ್ಕ cgroup_mode,
                  ಧ್ರುವ ವರ್ಣ *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t ಮೌಲ್ಯ;
  ಪೂರ್ಣಾಙ್ಕ sd_err;
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free ವರ್ಣ *dir = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ **missing_properties = ರಿಕ್ತ;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  missing_properties = read_dir_entries (dir, err);
  ಯದಿ (UNLIKELY (missing_properties == ರಿಕ್ತ))
    {
      ಯದಿ (crun_error_get_errno (err) != ENOENT)
        ಪ್ರತ್ಯಾಗಮ -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_ಮೌಲ್ಯ(name, ಮೌಲ್ಯ)                                             \
    ಕುರು                                                                                 \
      {                                                                                \
        ಯದಿ (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", ಮೌಲ್ಯ);              \
            ಯದಿ (UNLIKELY (sd_err < 0))                                                 \
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } ಯಾವತ್ (0)

#  define APPEND_UINT64(name, fn)              \
    ಕುರು                                         \
      {                                        \
        ret = fn (resources, &ಮೌಲ್ಯ, err);     \
        ಯದಿ (UNLIKELY (ret < 0))                \
          ಪ್ರತ್ಯಾಗಮ ret;                          \
        ಯದಿ (ret)                               \
          {                                    \
            APPEND_UINT64_ಮೌಲ್ಯ (name, ಮೌಲ್ಯ); \
          }                                    \
    } ಯಾವತ್ (0)

  ಯದಿ (resources == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  APPEND_UINT64 ("MemoryLow", get_memory_ಕೆಳಗಿನ);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ವಿಕಲ್ಪ (cgroup_mode)
    {
    ಪಕ್ಷ CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;

        ಯದಿ (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ಧ್ರುವ ವರ್ಣ *property_name = "AllowedCPUs";
            ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free ವರ್ಣ *allowed_cpus = ರಿಕ್ತ;
            size_t allowed_cpus_len = 0;

            ಯದಿ (ಮೌಲ್ಯ == ರಿಕ್ತ)
              ಮೌಲ್ಯ = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (ಮೌಲ್ಯ, &allowed_cpus, &allowed_cpus_len, err);
            ಯದಿ (UNLIKELY (ret < 0))
              ಪ್ರತ್ಯಾಗಮ ret;

            ಯದಿ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                ಯದಿ (UNLIKELY (ret < 0))
                  ಪ್ರತ್ಯಾಗಮ ret;
              }
          }

        ಯದಿ (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ಧ್ರುವ ವರ್ಣ *property_name = "AllowedMemoryNodes";
            ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free ವರ್ಣ *allowed_mems = ರಿಕ್ತ;
            size_t allowed_mems_len = 0;

            ಯದಿ (ಮೌಲ್ಯ == ರಿಕ್ತ)
              ಮೌಲ್ಯ = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (ಮೌಲ್ಯ, &allowed_mems, &allowed_mems_len, err);
            ಯದಿ (UNLIKELY (ret < 0))
              ಪ್ರತ್ಯಾಗಮ ret;

            ಯದಿ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                ಯದಿ (UNLIKELY (ret < 0))
                  ಪ್ರತ್ಯಾಗಮ ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಪ್ರತ್ಯಾಗಮ ret;
      }
      ಭಙ್ಗ;

    ಪಕ್ಷ CGROUP_MODE_LEGACY:
    ಪಕ್ಷ CGROUP_MODE_HYBRID:
      ಯದಿ (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_ಮೌಲ್ಯ ("CPUShares", resources->cpu->shares);
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_ಮೌಲ್ಯ

  ಯದಿ (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
reset_failed_unit (sd_bus *bus, ಧ್ರುವ ವರ್ಣ *unit)
{
  ಪೂರ್ಣಾಙ್ಕ sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = ರಿಕ್ತ, *reply = ರಿಕ್ತ;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  ಯದಿ (UNLIKELY (sd_err < 0))
    ಗಚ್ಛ exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  ಯದಿ (UNLIKELY (sd_err < 0))
    ಗಚ್ಛ exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  ಯದಿ (UNLIKELY (sd_err < 0))
    ಗಚ್ಛ exit;

  sd_err = 0;

exit:
  ಯದಿ (m)
    sd_bus_message_unref (m);
  ಯದಿ (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  ಪ್ರತ್ಯಾಗಮ sd_err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
verify_ebpf_device_filter_installed (ಧ್ರುವ ವರ್ಣ *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *full_path = ರಿಕ್ತ;
  size_t n_progs = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = append_paths (&full_path, err, CGROUP_ಬೇರು, cgroup_path, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (n_progs == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, ಪೂರ್ಣಾಙ್ಕ ret)
{
  ಯದಿ (bus)
    sd_bus_unref (bus);
  ಯದಿ (m)
    sd_bus_message_unref (m);
  ಯದಿ (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            ಪೂರ್ಣಾಙ್ಕ cgroup_mode,
                            string_map *annotations,
                            ಧ್ರುವ ವರ್ಣ *state_ಬೇರು,
                            ಧ್ರುವ ವರ್ಣ *scope, ಧ್ರುವ ವರ್ಣ *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = ರಿಕ್ತ;
  sd_bus_message *m = ರಿಕ್ತ;
  sd_bus_message *reply = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ಧ್ರುವ ವರ್ಣ *object = ರಿಕ್ತ;
  ಸಂರಚನಾ systemd_job_removed_s job_data = {};
  ಪೂರ್ಣಾಙ್ಕ i;
  ಧ್ರುವ ವರ್ಣ *boolean_opts[10];
  cleanup_free ವರ್ಣ *state_dir = ರಿಕ್ತ;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_ಬೇರು, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = ರಿಕ್ತ;

  ret = open_sd_bus_connection (&bus, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      ಗಚ್ಛ exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      ಗಚ್ಛ exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      ಗಚ್ಛ exit;
    }

  ಯದಿ (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      ಯದಿ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          ಗಚ್ಛ exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      ಗಚ್ಛ exit;
    }

  ಯದಿ (annotations)
    {
      size_t prefix_len = ಪರಿಮಾಣ (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      ಪ್ರತಿ (i = 0; i < annotations_len; i++)
        {
          ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ;
          ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ;
          size_t len;

          ret = string_map_get_at (annotations, i, &ಕೀಲಿ, &ಮೌಲ್ಯ);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              ಗಚ್ಛ exit;
            }

          ಯದಿ (! has_prefix (ಕೀಲಿ, SYSTEMD_PROPERTY_PREFIX))
            ಅನುವರ್ತ;

          len = strlen (ಕೀಲಿ);
          ಯದಿ (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", ಕೀಲಿ);
              ಗಚ್ಛ exit;
            }

          ret = append_systemd_annotation (m, ಕೀಲಿ + prefix_len, len - prefix_len,
                                           ಮೌಲ್ಯ, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಗಚ್ಛ exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      ಗಚ್ಛ exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      ಗಚ್ಛ exit;
    }

  ಪ್ರತಿ (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      ಯದಿ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          ಗಚ್ಛ exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ exit;

  sd_err = sd_bus_message_close_container (m);
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      ಗಚ್ಛ exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      ಗಚ್ಛ exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ಯದಿ (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          ಯದಿ (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = ರಿಕ್ತ;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      ಯದಿ (sd_err < 0)
        {
          ಯದಿ (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಗಚ್ಛ exit;
              ಯದಿ (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          ಗಚ್ಛ exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      ಗಚ್ಛ exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  ಪ್ರತ್ಯಾಗಮ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_destroy_systemd_cgroup_scope (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = ರಿಕ್ತ;
  sd_bus_message *m = ರಿಕ್ತ;
  sd_bus_message *reply = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ಧ್ರುವ ವರ್ಣ *object;
  ಧ್ರುವ ವರ್ಣ *scope = cgroup_status->scope;
  ಸಂರಚನಾ systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      ಗಚ್ಛ exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      ಗಚ್ಛ exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      ಗಚ್ಛ exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      ಗಚ್ಛ exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  ಪ್ರತ್ಯಾಗಮ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
find_systemd_subgroup (string_map *annotations)
{
  ಧ್ರುವ ವರ್ಣ *annotation;

  annotation = find_string_map_ಮೌಲ್ಯ (annotations, "run.oci.systemd.subgroup");
  ಯದಿ (annotation)
    {
      ಯದಿ (annotation[0] == '\0')
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
      ಪ್ರತ್ಯಾಗಮ annotation;
    }

  ಪ್ರತ್ಯಾಗಮ "container";
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_enter_systemd (ಸಂರಚನಾ libcrun_cgroup_args *args,
                              ಸಂರಚನಾ libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  ಧ್ರುವ ವರ್ಣ *cgroup_path = args->cgroup_path;
  cleanup_free ವರ್ಣ *scope_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *scope = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *slice = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ retries_ಎಡ = 32;
  ಧ್ರುವ ವರ್ಣ *suffix;
  ಧ್ರುವ ವರ್ಣ *id = args->id;
  pid_t pid = args->pid;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ rootless = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      ಯದಿ (UNLIKELY (rootless < 0))
        ಪ್ರತ್ಯಾಗಮ rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  ಪ್ರತಿ (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_ಬೇರು,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      ಯದಿ (LIKELY (ret >= 0))
        ಭಙ್ಗ;

      ಯದಿ (can_retry && retries_ಎಡ-- > 0)
        {
          crun_error_release (err);
          ಅನುವರ್ತ;
        }

      ಪ್ರತ್ಯಾಗಮ ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  ಯದಿ (is_empty_string (suffix))
    scope_path = xstrdup (path);
  ಅನ್ಯಥಾ
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      ಯದಿ (UNLIKELY (scope_path == ರಿಕ್ತ))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  ಯದಿ (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  out->path = path;
  path = ರಿಕ್ತ;

  out->scope = scope;
  scope = ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ವರ್ಣ *
get_cgroup_scope_path (ಧ್ರುವ ವರ್ಣ *cgroup_path, ಧ್ರುವ ವರ್ಣ *scope)
{
  ವರ್ಣ *path_to_scope = ರಿಕ್ತ;
  ವರ್ಣ *cur;

  ಯದಿ (is_empty_string (cgroup_path))
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  ಯಾವತ್ (cur)
    {
      ವರ್ಣ *next = strchr (cur + 1, '/');
      ಯದಿ (next == ರಿಕ್ತ)
        ಭಙ್ಗ;

      *next = '\0';
      ಯದಿ (strcmp (cur, scope) == 0)
        ಪ್ರತ್ಯಾಗಮ path_to_scope;
      *next = '/';

      cur = next;
      ಯಾವತ್ (*cur == '/')
        cur++;
    }

  ಪ್ರತ್ಯಾಗಮ path_to_scope;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_destroy_cgroup_systemd (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *path_to_scope = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *bpfprog = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (mode < 0))
    ಪ್ರತ್ಯಾಗಮ mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  ಯದಿ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  ಯದಿ (UNLIKELY (ret < 0))
    crun_error_release (err);
  ಅನ್ಯಥಾ
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  ಪ್ರತ್ಯಾಗಮ destroy_cgroup_path (path_to_scope, mode, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_update_resources_systemd (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status,
                                  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  ಸಂರಚನಾ systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free ವರ್ಣ *state_dir = ರಿಕ್ತ;
  sd_bus_message *reply = ರಿಕ್ತ;
  sd_bus_message *m = ರಿಕ್ತ;
  sd_bus *bus = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ sd_err, ret;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_ಬೇರು, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      ಗಚ್ಛ exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      ಗಚ್ಛ exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      ಗಚ್ಛ exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ exit;

  sd_err = sd_bus_message_close_container (m);
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      ಗಚ್ಛ exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  ಯದಿ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      ಗಚ್ಛ exit;
    }

  ಯದಿ (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ exit;

  ret = 0;

exit:
  ಪ್ರತ್ಯಾಗಮ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_attach_process_systemd (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status,
                                ಧ್ರುವ ವರ್ಣ *path, pid_t pid,
                                libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free ವರ್ಣ *scope_path = ರಿಕ್ತ;
  sd_bus_message *reply = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *subcgroup;
  ಧ್ರುವ ವರ್ಣ *last;
  sd_bus *bus = ರಿಕ್ತ;
  size_t len;
  ಪೂರ್ಣಾಙ್ಕ sd_err, ret;

  ಯದಿ (is_empty_string (scope))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  ಯದಿ (scope_path == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  ಯದಿ (last == ರಿಕ್ತ || strcmp (last + 1, scope) != 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  ಯದಿ (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  ಪ್ರತ್ಯಾಗಮ cleanup_sd_bus_and_return (bus, ರಿಕ್ತ, reply, &error, ret);
}

#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_cgroup_enter_systemd (ಸಂರಚನಾ libcrun_cgroup_args *args,
                              ಸಂರಚನಾ libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (ಶೂನ್ಯ) args;
  (ಶೂನ್ಯ) out;

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_destroy_cgroup_systemd (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (ಶೂನ್ಯ) cgroup_status;

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_update_resources_systemd (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status,
                                  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (ಶೂನ್ಯ) cgroup_status;
  (ಶೂನ್ಯ) state_ಬೇರು;
  (ಶೂನ್ಯ) resources;

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_attach_process_systemd (ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status,
                                ಧ್ರುವ ವರ್ಣ *path, pid_t pid,
                                libcrun_error_t *err)
{
  (ಶೂನ್ಯ) cgroup_status;
  (ಶೂನ್ಯ) path;
  (ಶೂನ್ಯ) pid;

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

ಸಂರಚನಾ libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = ರಿಕ್ತ,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
