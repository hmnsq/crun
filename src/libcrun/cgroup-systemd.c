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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
#  include <systemd/sd-bus.h>
#  include <limits.h>

#  define SYSTEMD_PROPERTY_ಪೂರ್ವಪ್ರತ್ಯಯ "org.systemd.property."

#  define CGROUP_BFQ_ತೂಕ_MIN ((uint64_t) 1)
#  define CGROUP_BFQ_ತೂಕ_DEFAULT ((uint64_t) 100)
#  define CGROUP_BFQ_ತೂಕ_MAX ((uint64_t) 1000)

#  define CGROUP_ತೂಕ_MIN ((uint64_t) 1)
#  define CGROUP_ತೂಕ_DEFAULT ((uint64_t) 100)
#  define CGROUP_ತೂಕ_MAX ((uint64_t) 10000)

/* The CPU quota period used by the kernel (and systemd) by default, in us.  */
#  define DEFAULT_CPU_QUOTA_PERIOD ((uint64_t) 100000)

#  define SYSTEMD_MISSING_PROPERTIES_DIR ".cache/systemd-missing-properties"

#  define IS_WILDCARD(x) (x <= 0)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ರಿಜಿಸ್ಟರ್_missing_property_from_ಸಂದೇಶ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_dir, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *file_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *dir_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *property = ಖಾಲಿ;
  ಅಕ್ಷರ *p;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (! has_ಪೂರ್ವಪ್ರತ್ಯಯ (ಸಂದೇಶ_2, "Cannot set property "))
    ಹಿಂತಿರುಗಿ 0;

  property = xstrdup (ಸಂದೇಶ_2 + ಗಾತ್ರ ("Cannot set property ") - 1);
  p = strchr (property, ',');
  ಆದರೆ (! p)
    ಹಿಂತಿರುಗಿ 0;
  *p = '\0';

  ret = append_paths (&dir_ಮಾರ್ಗ, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = crun_ensure_directory (dir_ಮಾರ್ಗ, 0755, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_paths (&file_ಮಾರ್ಗ, err, dir_ಮಾರ್ಗ, property, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_ಮಾರ್ಗ, ಖಾಲಿ, 0, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ bool
property_missing_p (ಅಕ್ಷರ **missing_properties, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *property)
{
  size_t i;

  ಗಾಗಿ (i = 0; missing_properties && missing_properties[i]; i++)
    ಆದರೆ (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        ಹಿಂತಿರುಗಿ true;
      }

  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
get_systemd_scope_and_ತುಂಡು (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, bool user_ತುಂಡು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ,
                             ಅಕ್ಷರ **scope, ಅಕ್ಷರ **ತುಂಡು)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *n;
  ಅಕ್ಷರ *p;

  ಆದರೆ (cgroup_ಮಾರ್ಗ == ಖಾಲಿ || cgroup_ಮಾರ್ಗ[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      ಹಿಂತಿರುಗಿ;
    }

  n = strchr (cgroup_ಮಾರ್ಗ, ':');
  ಆದರೆ (n == ಖಾಲಿ)
    xasprintf (scope, "%s.scope", cgroup_ಮಾರ್ಗ);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      ಆದರೆ (p)
        *p = '-';
    }
  ಆದರೆ (ತುಂಡು)
    {
      *ತುಂಡು = xstrdup (cgroup_ಮಾರ್ಗ);
      p = strchr (*ತುಂಡು, ':');
      ಆದರೆ (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      ಆದರೆ (is_ಖಾಲಿಗಳು_string (*ತುಂಡು))
        {
          ಬಿಡುಗಡೆ (*ತುಂಡು);
          ಆದರೆ (user_ತುಂಡು)
            *ತುಂಡು = xstrdup ("user.slice");
          ಇಲ್ಲದಿದ್ದರೆ
            *ತುಂಡು = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ dirfd = -1;
  bool need_ಗಣ_ಪೋಷಕ = true;
  ಅಕ್ಷರ fmt_buf[64];
  ಪೂರ್ಣಾಂಕ len;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (resources == ಖಾಲಿ || resources->cpu == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  ಆದರೆ (is_ಖಾಲಿಗಳು_string (ಮಾರ್ಗ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  ಆದರೆ (has_suffix (ಮಾರ್ಗ, ".scope"))
    need_ಗಣ_ಪೋಷಕ = false;

  ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, "cpu", ಮಾರ್ಗ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = crun_ensure_directory (cgroup_ಮಾರ್ಗ, 0755, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  dirfd = open (cgroup_ಮಾರ್ಗ, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (dirfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", cgroup_ಮಾರ್ಗ);

  ಆದರೆ (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ಆದರೆ (need_ಗಣ_ಪೋಷಕ)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, ಗಾತ್ರ (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_buf)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ಆದರೆ (need_ಗಣ_ಪೋಷಕ)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ಪೋಷಕ_2;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (resources == ಖಾಲಿ || resources->cpu == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (! resources->cpu->ಅವಧಿ_present && ! (cgroup2 && resources->cpu->idle_present))
    ಹಿಂತಿರುಗಿ 0;

  ಗಾಗಿ (ಪೋಷಕ_2 = 0; ಪೋಷಕ_2 < 2; ಪೋಷಕ_2++)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ dirfd = -1;

      ಆದರೆ (cgroup2)
        ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, ಮಾರ್ಗ ? ಮಾರ್ಗ : "", (ಪೋಷಕ_2 ? ".." : ಖಾಲಿ), ಖಾಲಿ);
      ಇಲ್ಲದಿದ್ದರೆ
        ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, "/cpu", ಮಾರ್ಗ ? ಮಾರ್ಗ : "", (ಪೋಷಕ_2 ? ".." : ಖಾಲಿ), ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      dirfd = open (cgroup_ಮಾರ್ಗ, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (dirfd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", cgroup_ಮಾರ್ಗ);

      ret = write_cpu_ಅವಧಿ (dirfd, cgroup2, resources->cpu, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      ಆದರೆ (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ಪೋಷಕ_2;
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ ಮಟ್ಟಗಳು;

  ret = append_paths (&cgroup_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, "/cpuset", ಮಾರ್ಗ ? ಮಾರ್ಗ : "", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = crun_ensure_directory (cgroup_ಮಾರ್ಗ, 0755, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_ಮಾರ್ಗ, resources, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  ಮಟ್ಟಗಳು = has_suffix (ಮಾರ್ಗ, ".scope") ? 1 : 2;

  ಗಾಗಿ (ಪೋಷಕ_2 = 0; ಪೋಷಕ_2 < ಮಟ್ಟಗಳು; ಪೋಷಕ_2++)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ dirfd_cpuset = -1;
      cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ_ಗಮ್ಯ_cpuset = ಖಾಲಿ;

      ret = append_paths (&ಮಾರ್ಗ_ಗಮ್ಯ_cpuset, err, CGROUP_ಬೇರು "/cpuset", ಮಾರ್ಗ, (ಪೋಷಕ_2 ? ".." : ಖಾಲಿ), ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      dirfd_cpuset = open (ಮಾರ್ಗ_ಗಮ್ಯ_cpuset, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (dirfd_cpuset < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ಮಾರ್ಗ_ಗಮ್ಯ_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compute_finalized_ಮಾರ್ಗ (ಅಕ್ಷರ **ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *from, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix, libcrun_error_t *err)
{
  ಆದರೆ (suffix == ಖಾಲಿ)
    {
      *ಮಾರ್ಗ = xstrdup (from);
      ಹಿಂತಿರುಗಿ 0;
    }
  ಹಿಂತಿರುಗಿ append_paths (ಮಾರ್ಗ, err, from, suffix, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
systemd_finalize (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ಅಕ್ಷರ **ಮಾರ್ಗ_ಹೊರಹೋಗು,
                  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = ವಾದಗಳು->resources;
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  pid_t pid = ವಾದಗಳು->pid;
  ಪೂರ್ಣಾಂಕ ret;
  ಅಕ್ಷರ *from, *ಗಮ್ಯ;
  ಅಕ್ಷರ *saveptr = ಖಾಲಿ;

  cleanup_free ಅಕ್ಷರ *proc_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

  xasprintf (&proc_ಮಾರ್ಗ, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (ವಾದಗಳು->container, proc_ಮಾರ್ಗ, O_RDONLY, err);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ fd;

  ret = read_all_fd (fd, "cgroup path", &content, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆಯ್ಕೆ (cgroup_ಬಹುಲಕ)
    {
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_HYBRID:
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_LEGACY:
      from = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (content, ":memory:");
      ಆದರೆ (LIKELY (from != ಖಾಲಿ))
        from += 8;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          from = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (content, ":pids:");
          ಆದರೆ (UNLIKELY (from == ಖಾಲಿ))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      ಗಮ್ಯ = strchr (from, '\n');
      ಆದರೆ (UNLIKELY (ಗಮ್ಯ == ಖಾಲಿ))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *ಗಮ್ಯ = '\0';
      ret = compute_finalized_ಮಾರ್ಗ (&ಮಾರ್ಗ, from, suffix, err);
      *ಗಮ್ಯ = '\n';
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (geteuid ())
        ಹಿಂತಿರುಗಿ 0;

      ret = ಸಿದ್ಧತೆ_rt_runtime (resources, ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = ಸಿದ್ಧತೆ_cpuset_for_systemd_v1 (resources, ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಗಾಗಿ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ಖಾಲಿ, "\n", &saveptr))
        {
          ಅಕ್ಷರ *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          ಆದರೆ (subsystem[0] == '\0')
            {
              ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_LEGACY)
                ಮುಂದುವರಿ;

              subsystem = "unified";
            }

          ಆದರೆ (strcmp (subpath, ಮಾರ್ಗ))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, ಮಾರ್ಗ, true, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  ಆದರೆ (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      ಮುಂದುವರಿ;
                    }
                  ಹಿಂತಿರುಗಿ ret;
                }
            }
        }

      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_UNIFIED:
      {
        cleanup_free ಅಕ್ಷರ *dir = ಖಾಲಿ;

        from = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (content, "0::");
        ಆದರೆ (UNLIKELY (from == ಖಾಲಿ))
          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        ಗಮ್ಯ = strchr (from, '\n');
        ಆದರೆ (UNLIKELY (ಗಮ್ಯ == ಖಾಲಿ))
          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *ಗಮ್ಯ = '\0';
        ret = compute_finalized_ಮಾರ್ಗ (&ಮಾರ್ಗ, from, suffix, err);
        *ಗಮ್ಯ = '\n';
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        ret = append_paths (&dir, err, CGROUP_ಬೇರು, ಮಾರ್ಗ, ಖಾಲಿ);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        ret = move_process_ಗಮ್ಯ_cgroup (pid, ಖಾಲಿ, ಮಾರ್ಗ, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        ret = enable_controllers (ಮಾರ್ಗ, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        ಆದರೆ (suffix)
          {
            ret = chown_cgroups (ಮಾರ್ಗ, ವಾದಗಳು->ಬೇರು_uid, ವಾದಗಳು->ಬೇರು_gid, err);
            ಆದರೆ (UNLIKELY (ret < 0))
              ಹಿಂತಿರುಗಿ ret;
          }
      }
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_ಬಹುಲಕ);
    }

  ret = ಸಿದ್ಧತೆ_missing_cpu_options_for_systemd (resources, cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED, ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  *ಮಾರ್ಗ_ಹೊರಹೋಗು = ಮಾರ್ಗ;
  ಮಾರ್ಗ = ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ systemd_ಕೆಲಸ_removed_s
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *op;
  ಪೂರ್ಣಾಂಕ terminated;
  libcrun_error_t err;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
systemd_ಕೆಲಸ_removed (sd_bus_ಸಂದೇಶ *m, ಶೂನ್ಯ *userdata, sd_bus_error *error arg_ಬಳಸದ)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, *unit, *ಫಲಿತಾಂಶ;
  uint32_t id;
  ಪೂರ್ಣಾಂಕ ret;
  ರಚನೆ systemd_ಕೆಲಸ_removed_s *d = userdata;

  ret = sd_bus_ಸಂದೇಶ_read (m, "uoss", &id, &ಮಾರ್ಗ, &unit, &ಫಲಿತಾಂಶ);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ -1;

  ಆದರೆ (strcmp (d->ಮಾರ್ಗ, ಮಾರ್ಗ) == 0)
    {
      d->terminated = 1;
      ಆದರೆ (strcmp (ಫಲಿತಾಂಶ, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, ಫಲಿತಾಂಶ);
          ಹಿಂತಿರುಗಿ -1;
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
systemd_check_ಕೆಲಸ_ಸ್ಥಿತಿ_ಸಿದ್ಧತೆ (sd_bus *bus, ರಚನೆ systemd_ಕೆಲಸ_removed_s *ದತ್ತಾಂಶ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = sd_bus_ಹೊಂದಿಕೆ_signal_async (bus, ಖಾಲಿ, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_ಕೆಲಸ_removed, ಖಾಲಿ, ದತ್ತಾಂಶ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "sd-bus match signal");

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
systemd_check_ಕೆಲಸ_ಸ್ಥಿತಿ (sd_bus *bus, ರಚನೆ systemd_ಕೆಲಸ_removed_s *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *op,
                          libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ sd_err;

  ದತ್ತಾಂಶ->ಮಾರ್ಗ = ಮಾರ್ಗ;
  ದತ್ತಾಂಶ->op = op;
  ಆಗ (! ದತ್ತಾಂಶ->terminated)
    {
      sd_err = sd_bus_process (bus, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus process");

      ಆದರೆ (sd_err != 0)
        ಮುಂದುವರಿ;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus wait");
    }

  ಆದರೆ (ದತ್ತಾಂಶ->err != ಖಾಲಿ)
    {
      *err = ದತ್ತಾಂಶ->err;
      ಹಿಂತಿರುಗಿ -1;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
parse_sd_array (ಅಕ್ಷರ *s, ಅಕ್ಷರ **ಹೊರಹೋಗು, ಅಕ್ಷರ **ಮುಂದಿನ, libcrun_error_t *err)
{
  ಅಕ್ಷರ endchr;
  ಅಕ್ಷರ *it, *dest;
  bool escaped = false;

  *ಹೊರಹೋಗು = ಖಾಲಿ;
  *ಮುಂದಿನ = ಖಾಲಿ;

  ಆಗ (ಖಾಲಿಯೇ (*s))
    s++;
  ಆದರೆ (*s == '\0')
    ಹಿಂತಿರುಗಿ 0;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*s != '\'' && *s != '"')
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *ಹೊರಹೋಗು = dest = it;

  ಆಗ (1)
    {
      ಆದರೆ (*it == '\0')
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid string `%s`", s);
      ಆದರೆ (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          ಆಗ (ಖಾಲಿಯೇ (*it))
            it++;
          ಆದರೆ (*it == ',')
            {
              *ಮುಂದಿನ = ++it;
              *dest = '\0';
              ಹಿಂತಿರುಗಿ 0;
            }

          ಆದರೆ (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              ಹಿಂತಿರುಗಿ 0;
            }

          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      ಆದರೆ (! escaped)
        *dest++ = *it;
      it++;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
append_systemd_annotation (sd_bus_ಸಂದೇಶ *m, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, size_t name_len, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
                           libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *tmp_name = ಖಾಲಿ;
  uint32_t ಗುಣಕ = 1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *it;
  ಪೂರ್ಣಾಂಕ sd_err;

  ಆಗ (*ಮೌಲ್ಯ == ' ')
    ಮೌಲ್ಯ++;

  it = ಮೌಲ್ಯ;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  ಆದರೆ (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
      && name[name_len - 1] == 'c')
    {
      ಗುಣಕ = 1000000;

      tmp_name = xmalloc (name_len + 2);
      ಸ್ಮರಣೆ_ನಕಲು (tmp_name, name, name_len - 3);
      ಸ್ಮರಣೆ_ನಕಲು (tmp_name + name_len - 3, "USec", 5);

      name = tmp_name;
    }

  /* A *USec property is always an uint64, so, like runc does, accept any
     number of seconds (possibly with a fractional part, and with an
     optional integer type), and convert it to uint64 microseconds.  */
  ಆದರೆ (ಗುಣಕ != 1)
    {
      ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", ಖಾಲಿ };
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಅಂಕೆ = it;
      ಅಕ್ಷರ *endptr = ಖಾಲಿ;
      ದ್ವಿಗುಣ v;
      size_t i;

      ಗಾಗಿ (i = 0; types[i]; i++)
        ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (ಅಂಕೆ, types[i]))
          {
            ಅಂಕೆ += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (types[i]);
            ನಿಲ್ಲಿಸು;
          }

      errno = 0;
      v = strtod (ಅಂಕೆ, &endptr);
      ಆದರೆ (UNLIKELY (errno != 0 || endptr == ಅಂಕೆ || *endptr || v < 0 || v * ಗುಣಕ >= 18446744073709551615.0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, ಮೌಲ್ಯ);

      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", name, "t", (uint64_t) (v * ಗುಣಕ));
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಹಿಂತಿರುಗಿ 0;
    }

  ಆದರೆ ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", name, "b", b);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಹಿಂತಿರುಗಿ 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*it == '\'')
    {
      cleanup_free ಅಕ್ಷರ *v_ಆರಂಭ = ಖಾಲಿ;
      ಅಕ್ಷರ *ಕೊನೆ;

      it = v_ಆರಂಭ = xstrdup (ಮೌಲ್ಯ);

      ಕೊನೆ = strchr (v_ಆರಂಭ + 1, '\'');
      ಆದರೆ (ಕೊನೆ == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid variant `%s`", ಮೌಲ್ಯ);
      *ಕೊನೆ = '\0';

      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", name, "s", it + 1);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಹಿಂತಿರುಗಿ 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*it == '[')
    {
      cleanup_free ಅಕ್ಷರ *v_ಆರಂಭ = ಖಾಲಿ;
      size_t n_parts = 0, parts_ಗಾತ್ರ = 32;
      cleanup_free ಅಕ್ಷರ **parts = xmalloc (ಗಾತ್ರ (ಅಕ್ಷರ *) * parts_ಗಾತ್ರ);
      ಅಕ್ಷರ *part;

      part = v_ಆರಂಭ = xstrdup (it + 1);
      ಆಗ (1)
        {
          ಅಕ್ಷರ *ಹೊರಹೋಗು = ಖಾಲಿ, *ಮುಂದಿನ = ಖಾಲಿ;
          ಪೂರ್ಣಾಂಕ ret;

          ret = parse_sd_array (part, &ಹೊರಹೋಗು, &ಮುಂದಿನ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          parts[n_parts++] = ಹೊರಹೋಗು;
          ಆದರೆ (n_parts == parts_ಗಾತ್ರ - 1)
            {
              parts_ಗಾತ್ರ += 32;
              parts = xrealloc (parts, ಗಾತ್ರ (ಅಕ್ಷರ *) * parts_ಗಾತ್ರ);
            }
          parts[n_parts] = ಖಾಲಿ;
          ಆದರೆ (ಮುಂದಿನ == ಖಾಲಿ)
            ನಿಲ್ಲಿಸು;

          part = ಮುಂದಿನ;
        }

      sd_err = sd_bus_ಸಂದೇಶ_open_container (m, 'r', "sv");
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_ಸಂದೇಶ_append (m, "s", name);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_ಸಂದೇಶ_open_container (m, 'v', "as");
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_ಸಂದೇಶ_append_strv (m, parts);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_ಸಂದೇಶ_close_container (m);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_ಸಂದೇಶ_close_container (m);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus close container");

      ಹಿಂತಿರುಗಿ 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (it, "uint64 "))
    {
      ಅಕ್ಷರ *endptr = ಖಾಲಿ;
      uint64_t v;

      errno = 0;
      v = strtoull (it + ಗಾತ್ರ ("uint64"), &endptr, 10);
      ಆದರೆ (UNLIKELY (errno != 0 || *endptr))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", name, "t", (uint64_t) (v * ಗುಣಕ));
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಹಿಂತಿರುಗಿ 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (it, "int64 "))
    {
      ಅಕ್ಷರ *endptr = ಖಾಲಿ;
      int64_t v;

      errno = 0;
      v = strtoll (it + ಗಾತ್ರ ("int64"), &endptr, 10);
      ಆದರೆ (UNLIKELY (errno != 0 || *endptr))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", name, "x", (int64_t) (v * ಗುಣಕ));
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಹಿಂತಿರುಗಿ 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (it, "uint32 "))
    {
      ಅಕ್ಷರ *endptr = ಖಾಲಿ;
      uint32_t v;

      errno = 0;
      v = strtoul (it + ಗಾತ್ರ ("uint32"), &endptr, 10);
      ಆದರೆ (UNLIKELY (errno != 0 || *endptr))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", name, "u", (uint32_t) (v * ಗುಣಕ));
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಹಿಂತಿರುಗಿ 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (it, "int32 ") || strchr (it, ' ') == ಖಾಲಿ)
    {
      ಅಕ್ಷರ *endptr = ಖಾಲಿ;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (it, "int32 "))
        v = strtol (it + ಗಾತ್ರ ("int32"), &endptr, 10);
      ಇಲ್ಲದಿದ್ದರೆ
        v = strtol (it, &endptr, 10);
      ಆದರೆ (UNLIKELY (errno != 0 || *endptr))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", name, "i", (int32_t) (v * ಗುಣಕ));
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ಹಿಂತಿರುಗಿ 0;
    }

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "unknown type for `%s`", name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ rootless;
  ಪೂರ್ಣಾಂಕ sd_err = 0;

  rootless = is_rootless (err);
  ಆದರೆ (UNLIKELY (rootless < 0))
    ಹಿಂತಿರುಗಿ rootless;

  ಆದರೆ (rootless)
    sd_err = sd_bus_default_user (bus);
  ಆದರೆ (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  ಆದರೆ (sd_err < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "cannot open sd-bus");

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
  size_t i;

  ಆದರೆ (resources == ಖಾಲಿ || resources->unified == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಗಾಗಿ (i = 0; i < resources->unified->len; i++)
    ಆದರೆ (strcmp (resources->unified->ಕೀಲಿಗಳು[i], name) == 0)
      ಹಿಂತಿರುಗಿ resources->unified->ಮೌಲ್ಯಗಳು[i];
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_ಮೌಲ್ಯ_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
                            uint64_t *ಮೌಲ್ಯ, libcrun_error_t *err)
{
  ಅಕ್ಷರ *endptr = ಖಾಲಿ;
  size_t i;

  ಆದರೆ (resources == ಖಾಲಿ || resources->unified == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಗಾಗಿ (i = 0; i < resources->unified->len; i++)
    ಆದರೆ (strcmp (resources->unified->ಕೀಲಿಗಳು[i], name) == 0)
      {
        ಆದರೆ (strcmp (resources->unified->ಮೌಲ್ಯಗಳು[i], "max") == 0)
          {
            *ಮೌಲ್ಯ = UINT64_MAX;
            ಹಿಂತಿರುಗಿ 1;
          }

        errno = 0;
        *ಮೌಲ್ಯ = strtoull (resources->unified->ಮೌಲ್ಯಗಳು[i], &endptr, 10);
        ಆದರೆ (UNLIKELY (errno))
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->ಮೌಲ್ಯಗಳು[i]);
        ಆದರೆ (endptr && *endptr)
          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->ಮೌಲ್ಯಗಳು[i]);
        ಹಿಂತಿರುಗಿ 1;
      }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
get_ಸ್ಮರಣೆ_ಕೆಳಗಿನ (runtime_spec_schema_config_linux_resources *resources, uint64_t *ಎಲ್ಲೆ, libcrun_error_t *err)
{
  ಆದರೆ (resources->ಸ್ಮರಣೆ && resources->ಸ್ಮರಣೆ->reservation_present)
    {
      *ಎಲ್ಲೆ = resources->ಸ್ಮರಣೆ->reservation;
      ಹಿಂತಿರುಗಿ 1;
    }

  ಹಿಂತಿರುಗಿ get_ಮೌಲ್ಯ_from_unified_map (resources, "memory.low", ಎಲ್ಲೆ, err);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *ಎಲ್ಲೆ, libcrun_error_t *err)
{
  ಆದರೆ (resources->pids && resources->pids->ಎಲ್ಲೆ)
    {
      *ಎಲ್ಲೆ = resources->pids->ಎಲ್ಲೆ;
      ಹಿಂತಿರುಗಿ 1;
    }

  ಹಿಂತಿರುಗಿ get_ಮೌಲ್ಯ_from_unified_map (resources, "pids.max", ಎಲ್ಲೆ, err);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
get_ಸ್ಮರಣೆ_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *ಎಲ್ಲೆ, libcrun_error_t *err)
{
  ಆದರೆ (resources->ಸ್ಮರಣೆ && resources->ಸ್ಮರಣೆ->ಎಲ್ಲೆ_present)
    {
      *ಎಲ್ಲೆ = resources->ಸ್ಮರಣೆ->ಎಲ್ಲೆ;
      ಹಿಂತಿರುಗಿ 1;
    }

  ಹಿಂತಿರುಗಿ get_ಮೌಲ್ಯ_from_unified_map (resources, "memory.max", ಎಲ್ಲೆ, err);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
get_ಸ್ಮರಣೆ_ಅದಲುಬದಲು_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *ಎಲ್ಲೆ, libcrun_error_t *err)
{
  ಆದರೆ (resources->ಸ್ಮರಣೆ && resources->ಸ್ಮರಣೆ->ಅದಲುಬದಲು_present)
    {
      runtime_spec_schema_config_linux_resources_ಸ್ಮರಣೆ *ಸ್ಮರಣೆ = resources->ಸ್ಮರಣೆ;
      int64_t mem = ಸ್ಮರಣೆ->ಎಲ್ಲೆ_present ? ಸ್ಮರಣೆ->ಎಲ್ಲೆ : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      ಆದರೆ (ಸ್ಮರಣೆ->ಅದಲುಬದಲು == -1 || (mem == -1 && ಸ್ಮರಣೆ->ಅದಲುಬದಲು == 0))
        {
          *ಎಲ್ಲೆ = UINT64_MAX;
          ಹಿಂತಿರುಗಿ 1;
        }
      /* 0 means unset.  */
      ಆದರೆ (ಸ್ಮರಣೆ->ಅದಲುಬದಲು == 0)
        ಹಿಂತಿರುಗಿ 0;
      ಆದರೆ (mem <= 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ಆದರೆ (ಸ್ಮರಣೆ->ಅದಲುಬದಲು < mem)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *ಎಲ್ಲೆ = ಸ್ಮರಣೆ->ಅದಲುಬದಲು - mem;
      ಹಿಂತಿರುಗಿ 1;
    }

  ಹಿಂತಿರುಗಿ get_ಮೌಲ್ಯ_from_unified_map (resources, "memory.swap.max", ಎಲ್ಲೆ, err);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
get_cpu_ತೂಕ (runtime_spec_schema_config_linux_resources *resources, uint64_t *ತೂಕ, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t ಮೌಲ್ಯ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = get_ಮೌಲ್ಯ_from_unified_map (resources, "cpu.idle", &ಮೌಲ್ಯ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  ಆದರೆ (ret > 0 && ಮೌಲ್ಯ == 1)
    has_idle = true;
  ಆದರೆ (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_ಮೌಲ್ಯ_from_unified_map (resources, "cpu.weight", ತೂಕ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  ಆದರೆ (ret > 0)
    {
      ಆದರೆ (has_idle)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      ಹಿಂತಿರುಗಿ 1;
    }

  ಆದರೆ (resources->cpu && resources->cpu->shares_present)
    {
      ಆದರೆ (has_idle)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      ಆದರೆ (resources->cpu->shares == 0)
        ಹಿಂತಿರುಗಿ 0;
      *ತೂಕ = convert_shares_ಗಮ್ಯ_ತೂಕ (resources->cpu->shares);
      ಹಿಂತಿರುಗಿ 1;
    }
  ಆದರೆ (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *ತೂಕ = 0;
      ಹಿಂತಿರುಗಿ 1;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_uint64_ಮೌಲ್ಯ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, uint64_t *ಮೌಲ್ಯ, libcrun_error_t *err)
{
  ಅಕ್ಷರ *endptr = ಖಾಲಿ;

  errno = 0;
  *ಮೌಲ್ಯ = strtoull (str, &endptr, 10);
  ಆದರೆ (UNLIKELY (errno))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "invalid value: %s", str);
  ಆದರೆ (endptr && *endptr)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid value: %s", str);
  ಹಿಂತಿರುಗಿ 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ uint64_t
IO_ತೂಕ (uint64_t bfq_ತೂಕ)
{
  ಹಿಂತಿರುಗಿ bfq_ತೂಕ <= CGROUP_BFQ_ತೂಕ_DEFAULT ? CGROUP_ತೂಕ_DEFAULT - (CGROUP_BFQ_ತೂಕ_DEFAULT - bfq_ತೂಕ) * (CGROUP_ತೂಕ_DEFAULT - CGROUP_ತೂಕ_MIN) / (CGROUP_BFQ_ತೂಕ_DEFAULT - CGROUP_BFQ_ತೂಕ_MIN) : CGROUP_ತೂಕ_DEFAULT + (bfq_ತೂಕ - CGROUP_BFQ_ತೂಕ_DEFAULT) * (CGROUP_ತೂಕ_MAX - CGROUP_ತೂಕ_DEFAULT) / (CGROUP_BFQ_ತೂಕ_MAX - CGROUP_BFQ_ತೂಕ_DEFAULT);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
append_io_ತೂಕ (sd_bus_ಸಂದೇಶ *m, ಅಕ್ಷರ **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t ತೂಕ;
  ಪೂರ್ಣಾಂಕ sd_err;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

#  define APPEND_IO_ತೂಕ(ಮೌಲ್ಯ)                                                      \
    ಮಾಡು                                                                                 \
      {                                                                                \
        ಆದರೆ (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "IOWeight", "t", ತೂಕ);       \
            ಆದರೆ (UNLIKELY (sd_err < 0))                                                 \
              ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } ಆಗ (0)

#  define APPEND_IO_DEVICE_ತೂಕ(device, ಮೌಲ್ಯ)                                                      \
    ಮಾಡು                                                                                                \
      {                                                                                               \
        ಆದರೆ (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, ತೂಕ); \
            ಆದರೆ (UNLIKELY (sd_err < 0))                                                                \
              ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } ಆಗ (0)

  ಆದರೆ (resources->ಖಂಡ_io && resources->ಖಂಡ_io->ತೂಕ_present)
    {
      ತೂಕ = IO_ತೂಕ (resources->ಖಂಡ_io->ತೂಕ);
      APPEND_IO_ತೂಕ (ತೂಕ);
    }

  ಗಾಗಿ (i = 0; resources->ಖಂಡ_io && i < resources->ಖಂಡ_io->ತೂಕ_device_len; i++)
    {
      ಅಕ್ಷರ name[64];
      ಪೂರ್ಣಾಂಕ len;

      len = snprintf (name, ಗಾತ್ರ (name), "%" PRIu64 ":%" PRIu64,
                      resources->ಖಂಡ_io->ತೂಕ_device[i]->major,
                      resources->ಖಂಡ_io->ತೂಕ_device[i]->minor);
      ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (name)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ತೂಕ = IO_ತೂಕ (resources->ಖಂಡ_io->ತೂಕ_device[i]->ತೂಕ);
      APPEND_IO_DEVICE_ತೂಕ (name, ತೂಕ);
    }

  ಗಾಗಿ (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free ಅಕ್ಷರ *ಮೌಲ್ಯ = ಖಾಲಿ;
      bool need_conversion = false;
      ಅಕ್ಷರ *saveptr = ಖಾಲಿ;
      ಅಕ್ಷರ *ಸಾಲು;

      ಆದರೆ (strcmp (resources->unified->ಕೀಲಿಗಳು[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (resources->unified->ಕೀಲಿಗಳು[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Skip other keys.  */
          ಮುಂದುವರಿ;
        }

      ಮೌಲ್ಯ = xstrdup (resources->unified->ಮೌಲ್ಯಗಳು[i]);

      ಗಾಗಿ (ಸಾಲು = strtok_r (ಮೌಲ್ಯ, "\n", &saveptr); ಸಾಲು; ಸಾಲು = strtok_r (ಖಾಲಿ, "\n", &saveptr))
        {
          ಅಕ್ಷರ *sep;

          sep = strchr (ಸಾಲು, ' ');
          ಆಗ (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_ಮೌಲ್ಯ (sep ? sep : ಸಾಲು, &ತೂಕ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          ಆದರೆ (need_conversion)
            ತೂಕ = IO_ತೂಕ (ತೂಕ);

          ಆದರೆ (sep == ಖಾಲಿ || strcmp (ಸಾಲು, "default") == 0)
            APPEND_IO_ತೂಕ (ತೂಕ);
          ಇಲ್ಲದಿದ್ದರೆ
            APPEND_IO_DEVICE_ತೂಕ (ಸಾಲು, ತೂಕ);
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Adapted from systemd.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
bus_append_byte_array (sd_bus_ಸಂದೇಶ *m, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, size_t n, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = sd_bus_ಸಂದೇಶ_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_ಸಂದೇಶ_append_basic (m, SD_BUS_TYPE_STRING, ಕ್ಷೇತ್ರ);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_ಸಂದೇಶ_open_container (m, 'v', "ay");
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_ಸಂದೇಶ_append_array (m, 'y', buf, n);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_ಸಂದೇಶ_close_container (m);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_ಸಂದೇಶ_close_container (m);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
append_uint64_from_unified_map (sd_bus_ಸಂದೇಶ *m,
                                ಅಕ್ಷರ **missing_properties,
                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *attr,
                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t ಮೌಲ್ಯ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (property_missing_p (missing_properties, attr))
    ಹಿಂತಿರುಗಿ 0;

  ret = get_ಮೌಲ್ಯ_from_unified_map (resources, ಕೀಲಿ, &ಮೌಲ್ಯ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  ಆದರೆ (ret)
    {
      ಪೂರ್ಣಾಂಕ sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", attr, "t", ಮೌಲ್ಯ);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
append_device_allow (sd_bus_ಸಂದೇಶ *m,
                     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ type,
                     ಪೂರ್ಣಾಂಕ major,
                     ಪೂರ್ಣಾಂಕ minor,
                     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *access,
                     libcrun_error_t *err)
{
  ಅಕ್ಷರ device[64];
  ಪೂರ್ಣಾಂಕ sd_err;
  ಪೂರ್ಣಾಂಕ len;

  ಆದರೆ (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      ಹಿಂತಿರುಗಿ 0;
    }

  ಆದರೆ (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, ಗಾತ್ರ (device), "%s-*", type == 'c' ? "char" : "block");
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (IS_WILDCARD (minor))
    len = snprintf (device, ಗಾತ್ರ (device), type == 'c' ? "char-%d" : "block-%d", major);
  ಇಲ್ಲದಿದ್ದರೆ
    len = snprintf (device, ಗಾತ್ರ (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (device)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  ಹಿಂತಿರುಗಿ 0;
}

#  define ಬಹುಲಕ_R 1
#  define ಬಹುಲಕ_W 2
#  define ಬಹುಲಕ_M 4

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
access_ಗಮ್ಯ_ಬಹುಲಕ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *access, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ಬಹುಲಕ = 0;

  ಆಗ (access && *access)
    {
      ಪೂರ್ಣಾಂಕ c = *access++;

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'r':
          ಬಹುಲಕ |= ಬಹುಲಕ_R;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'w':
          ಬಹುಲಕ |= ಬಹುಲಕ_W;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'm':
          ಬಹುಲಕ |= ಬಹುಲಕ_M;
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  ಹಿಂತಿರುಗಿ ಬಹುಲಕ;
}

/* check if the type b is included in type a.  */
ಸ್ಥಿರ bool
has_same_type (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *b)
{
  bool a_all = a == ಖಾಲಿ || strcmp (a, "a") == 0;
  ಆದರೆ (b == ಖಾಲಿ || strcmp (b, "a") == 0)
    ಹಿಂತಿರುಗಿ a_all;

  ಹಿಂತಿರುಗಿ a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
is_deny_ನಿಯಮ_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  ಪೂರ್ಣಾಂಕ req_ಬಹುಲಕ;

#  define CMP_DEV_ಅಂಕೆ(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_ಬಹುಲಕ = access_ಗಮ್ಯ_ಬಹುಲಕ (devices[n]->access, err);
  ಆದರೆ (UNLIKELY (req_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ req_ಬಹುಲಕ;

  ಗಾಗಿ (i = n; i > 0; i--)
    {
      size_t current_ನಿಯಮ = i - 1;
      ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;

      ಆದರೆ (! CMP_DEV_ಅಂಕೆ (devices[n]->minor, devices[current_ನಿಯಮ]->minor))
        ಮುಂದುವರಿ;
      ಆದರೆ (! CMP_DEV_ಅಂಕೆ (devices[n]->major, devices[current_ನಿಯಮ]->major))
        ಮುಂದುವರಿ;

      ಆದರೆ (! has_same_type (devices[n]->type, devices[current_ನಿಯಮ]->type)
          && has_same_type ("a", devices[current_ನಿಯಮ]->type))
        ಮುಂದುವರಿ;

      ಬಹುಲಕ = access_ಗಮ್ಯ_ಬಹುಲಕ (devices[i]->access, err);
      ಆದರೆ (UNLIKELY (ಬಹುಲಕ < 0))
        ಹಿಂತಿರುಗಿ ಬಹುಲಕ;

      ಆದರೆ (devices[current_ನಿಯಮ]->allow && (ಬಹುಲಕ & req_ಬಹುಲಕ))
        ಹಿಂತಿರುಗಿ 0;

      req_ಬಹುಲಕ &= ! ಬಹುಲಕ;

      /* no more access bits to validate.  */
      ಆದರೆ (req_ಬಹುಲಕ == 0)
        ಹಿಂತಿರುಗಿ 1;
    }

  ಹಿಂತಿರುಗಿ 0;
#  undef CMP_DEV_ಅಂಕೆ
}

ಸ್ಥಿರ size_t
ಕಂಡುಹಿಡಿ_ಮೊದಲ_ನಿಯಮ_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  ಆದರೆ (n == 0)
    ಹಿಂತಿರುಗಿ 1;

  /* Find the first rule that is after the last "block all".  */
  ಗಾಗಿ (i = n - 1; i-- > 0;)
    {
      ಆದರೆ ((is_ಖಾಲಿಗಳು_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        ಹಿಂತಿರುಗಿ i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  ಗಾಗಿ (i = 0; i < n; i++)
    ಆದರೆ (devices[i]->allow)
      ಹಿಂತಿರುಗಿ i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  ಹಿಂತಿರುಗಿ n + 1;
}

ಸ್ಥಿರ bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_ಖಂಡ 1
#  define DEV_TYPE_CHAR 2
  ಪೂರ್ಣಾಂಕ ಉಳಿದ = DEV_TYPE_ಖಂಡ | DEV_TYPE_CHAR;
  size_t i;

  ಗಾಗಿ (i = 0; i < n; i++)
    {
      size_t current_ನಿಯಮ = n - i - 1;
      ಆದರೆ (! devices[current_ನಿಯಮ]->allow)
        ಹಿಂತಿರುಗಿ false;

      ಆದರೆ (IS_WILDCARD (devices[current_ನಿಯಮ]->major) && IS_WILDCARD (devices[current_ನಿಯಮ]->minor))
        {
          ಆದರೆ (is_ಖಾಲಿಗಳು_string (devices[current_ನಿಯಮ]->type) || strcmp (devices[current_ನಿಯಮ]->type, "a") == 0)
            ಹಿಂತಿರುಗಿ true;

          ಆದರೆ (strcmp (devices[current_ನಿಯಮ]->type, "c") == 0)
            ಉಳಿದ &= ~DEV_TYPE_CHAR;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (devices[current_ನಿಯಮ]->type, "b") == 0)
            ಉಳಿದ &= ~DEV_TYPE_ಖಂಡ;

          ಆದರೆ (ಉಳಿದ == 0)
            ಹಿಂತಿರುಗಿ true;
        }
    }
#  undef DEV_TYPE_ಖಂಡ
#  undef DEV_TYPE_CHAR

  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
append_devices (sd_bus_ಸಂದೇಶ *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  ರಚನೆ default_dev_s *default_devices = get_default_devices ();
  ಪೂರ್ಣಾಂಕ ret, sd_err;
  size_t i;

  ಆದರೆ (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "DevicePolicy", "s", "auto");
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      ಹಿಂತಿರುಗಿ 0;
    }

  sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "DevicePolicy", "s", "strict");
  ಆದರೆ (UNLIKELY (sd_err < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  ಗಾಗಿ (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಗಾಗಿ (i = ಕಂಡುಹಿಡಿ_ಮೊದಲ_ನಿಯಮ_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      ಅಕ್ಷರ type;

      ಆದರೆ (! d->allow)
        {
          ಪೂರ್ಣಾಂಕ redundant;

          redundant = is_deny_ನಿಯಮ_redundant (resources->devices, i, err);
          ಆದರೆ (UNLIKELY (redundant < 0))
            ಹಿಂತಿರುಗಿ redundant;

          ಆದರೆ (redundant)
            ಮುಂದುವರಿ;

          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      ಆದರೆ (d->type == ಖಾಲಿ || strcmp (d->type, "a") == 0)
        type = 'a';
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (d->type, "c") == 0)
        type = 'c';
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (d->type, "b") == 0)
        type = 'b';
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "unknown device type `%s`", d->type);

      ಆದರೆ (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ bool
has_bpf_fs ()
{
  ರಚನೆ statfs stat;

  ಹಿಂತಿರುಗಿ statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
bpfprog_ಮಾರ್ಗ_from_scope (ಅಕ್ಷರ **ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scope, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *flat_scope = xstrdup (scope);
  ಅಕ್ಷರ *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  ಆಗ ((it = strchr (it, '.')) != ಖಾಲಿ)
    *it = '_';

  ಹಿಂತಿರುಗಿ append_paths (ಮಾರ್ಗ, err, CRUN_BPF_DIR, flat_scope, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಕೂಡಿಸು_bpf_ಕಾರ್ಯಕ್ರಮ (sd_bus_ಸಂದೇಶ *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 ಅಕ್ಷರ **missing_properties,
                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scope,
                 bool *devices_ಗಣ,
                 libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret = 0;
  ಪೂರ್ಣಾಂಕ ಒಳ_userns, sd_err;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *property_name = "BPFProgram";
  cleanup_free ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;

  *devices_ಗಣ = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  ಆದರೆ (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    ಹಿಂತಿರುಗಿ 0;

  ಒಳ_userns = check_running_ಒಳ_user_namespace (err);
  ಆದರೆ (UNLIKELY (ಒಳ_userns < 0))
    ಹಿಂತಿರುಗಿ ಒಳ_userns;

  ಆದರೆ (ಒಳ_userns)
    ಹಿಂತಿರುಗಿ 0;

  ಕಾರ್ಯಕ್ರಮ = create_dev_bpf (resources->devices, resources->devices_len, err);
  ಆದರೆ (UNLIKELY (ಕಾರ್ಯಕ್ರಮ == ಖಾಲಿ))
    {
      crun_error_release (err);
      ಹಿಂತಿರುಗಿ 0;
    }

  ret = bpfprog_ಮಾರ್ಗ_from_scope (&ಮಾರ್ಗ, scope, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  ಆದರೆ (UNLIKELY (ret < 0 && errno != EEXIST))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  ಆದರೆ (is_update)
    {
      cleanup_free ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ_loaded = ಖಾಲಿ;

      ಆದರೆ (resources->devices == ಖಾಲಿ || resources->devices_len == 0)
        {
          *devices_ಗಣ = true;
          ಹಿಂತಿರುಗಿ 0;
        }

      ret = libcrun_ebpf_read_ಕಾರ್ಯಕ್ರಮ (&ಕಾರ್ಯಕ್ರಮ_loaded, ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      /* it is the same as the loaded program.  */
      ಆದರೆ (libcrun_ebpf_cmp_programs (ಕಾರ್ಯಕ್ರಮ, ಕಾರ್ಯಕ್ರಮ_loaded))
        {
          *devices_ಗಣ = true;
          ಹಿಂತಿರುಗಿ 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ_new = ಖಾಲಿ;
        ಅಕ್ಷರ tmp[64];
        ಪೂರ್ಣಾಂಕ len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, ಗಾತ್ರ (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (tmp)))
          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_ತುಂಬು (ಕಾರ್ಯಕ್ರಮ, -1, tmp, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        ret = libcrun_ebpf_read_ಕಾರ್ಯಕ್ರಮ (&ಕಾರ್ಯಕ್ರಮ_new, tmp, err);
        unlink (tmp);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        ಆದರೆ (libcrun_ebpf_cmp_programs (ಕಾರ್ಯಕ್ರಮ_new, ಕಾರ್ಯಕ್ರಮ_loaded))
          {
            *devices_ಗಣ = true;
            ಹಿಂತಿರುಗಿ 0;
          }
      }

      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_ತುಂಬು (ಕಾರ್ಯಕ್ರಮ, -1, ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", property_name, "a(ss)", 1, "device", ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_ಗಣ = true;

  ಹಿಂತಿರುಗಿ 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
append_cpu_quota (sd_bus_ಸಂದೇಶ *m, ಅಕ್ಷರ **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  ಪೂರ್ಣಾಂಕ sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ಆದರೆ (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      ಪೂರ್ಣಾಂಕ ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      ಆದರೆ (has_period)
        period = max_period;
    }

  ಆದರೆ (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  ಆದರೆ ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_ಪ್ರತಿ_sec = UINT64_MAX;

      ಆದರೆ (quota > 0)
        {
          quota_ಪ್ರತಿ_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          ಆದರೆ (quota_ಪ್ರತಿ_sec % 10000)
            quota_ಪ್ರತಿ_sec = ((quota_ಪ್ರತಿ_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_ಪ್ರತಿ_sec);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
append_resources (sd_bus_ಸಂದೇಶ *m,
                  bool is_update,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scope,
                  bool *devices_ಗಣ,
                  libcrun_error_t *err)
{
  uint64_t ಮೌಲ್ಯ;
  ಪೂರ್ಣಾಂಕ sd_err;
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free ಅಕ್ಷರ *dir = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ **missing_properties = ಖಾಲಿ;

  *devices_ಗಣ = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  missing_properties = read_dir_entries (dir, err);
  ಆದರೆ (UNLIKELY (missing_properties == ಖಾಲಿ))
    {
      ಆದರೆ (crun_error_get_errno (err) != ENOENT)
        ಹಿಂತಿರುಗಿ -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_ಮೌಲ್ಯ(name, ಮೌಲ್ಯ)                                             \
    ಮಾಡು                                                                                 \
      {                                                                                \
        ಆದರೆ (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", name, "t", ಮೌಲ್ಯ);              \
            ಆದರೆ (UNLIKELY (sd_err < 0))                                                 \
              ಹಿಂತಿರುಗಿ crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } ಆಗ (0)

#  define APPEND_UINT64(name, fn)              \
    ಮಾಡು                                         \
      {                                        \
        ret = fn (resources, &ಮೌಲ್ಯ, err);     \
        ಆದರೆ (UNLIKELY (ret < 0))                \
          ಹಿಂತಿರುಗಿ ret;                          \
        ಆದರೆ (ret)                               \
          {                                    \
            APPEND_UINT64_ಮೌಲ್ಯ (name, ಮೌಲ್ಯ); \
          }                                    \
    } ಆಗ (0)

  ಆದರೆ (resources == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  APPEND_UINT64 ("MemoryLow", get_ಸ್ಮರಣೆ_ಕೆಳಗಿನ);
  APPEND_UINT64 ("MemoryMax", get_ಸ್ಮರಣೆ_max);
  APPEND_UINT64 ("MemorySwapMax", get_ಸ್ಮರಣೆ_ಅದಲುಬದಲು_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆಯ್ಕೆ (cgroup_ಬಹುಲಕ)
    {
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_UNIFIED:
      {
        ret = append_io_ತೂಕ (m, missing_properties, resources, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_ತೂಕ);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;

        ಆದರೆ (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *property_name = "AllowedCPUs";
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free ಅಕ್ಷರ *allowed_cpus = ಖಾಲಿ;
            size_t allowed_cpus_len = 0;

            ಆದರೆ (ಮೌಲ್ಯ == ಖಾಲಿ)
              ಮೌಲ್ಯ = resources->cpu->cpus;

            ret = cpuset_string_ಗಮ್ಯ_bitmask (ಮೌಲ್ಯ, &allowed_cpus, &allowed_cpus_len, err);
            ಆದರೆ (UNLIKELY (ret < 0))
              ಹಿಂತಿರುಗಿ ret;

            ಆದರೆ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                ಆದರೆ (UNLIKELY (ret < 0))
                  ಹಿಂತಿರುಗಿ ret;
              }
          }

        ಆದರೆ (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *property_name = "AllowedMemoryNodes";
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free ಅಕ್ಷರ *allowed_mems = ಖಾಲಿ;
            size_t allowed_mems_len = 0;

            ಆದರೆ (ಮೌಲ್ಯ == ಖಾಲಿ)
              ಮೌಲ್ಯ = resources->cpu->mems;

            ret = cpuset_string_ಗಮ್ಯ_bitmask (ಮೌಲ್ಯ, &allowed_mems, &allowed_mems_len, err);
            ಆದರೆ (UNLIKELY (ret < 0))
              ಹಿಂತಿರುಗಿ ret;

            ಆದರೆ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                ಆದರೆ (UNLIKELY (ret < 0))
                  ಹಿಂತಿರುಗಿ ret;
              }
          }

        ret = ಕೂಡಿಸು_bpf_ಕಾರ್ಯಕ್ರಮ (m, is_update, resources, missing_properties, scope, devices_ಗಣ, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹಿಂತಿರುಗಿ ret;
      }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_LEGACY:
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_HYBRID:
      ಆದರೆ (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_ಮೌಲ್ಯ ("CPUShares", resources->cpu->shares);
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_ಬಹುಲಕ);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_ಮೌಲ್ಯ

  ಆದರೆ (! *devices_ಗಣ && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
reset_failed_unit (sd_bus *bus, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *unit)
{
  ಪೂರ್ಣಾಂಕ sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_ಸಂದೇಶ *m = ಖಾಲಿ, *ಉತ್ತರ = ಖಾಲಿ;

  sd_err = sd_bus_ಸಂದೇಶ_new_ವಿಧಾನ_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  ಆದರೆ (UNLIKELY (sd_err < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  sd_err = sd_bus_ಸಂದೇಶ_append (m, "s", unit);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  sd_err = sd_bus_call (bus, m, 0, &error, &ಉತ್ತರ);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  sd_err = 0;

ನಿರ್ಗಮಿಸು:
  ಆದರೆ (m)
    sd_bus_ಸಂದೇಶ_unref (m);
  ಆದರೆ (ಉತ್ತರ)
    sd_bus_ಸಂದೇಶ_unref (ಉತ್ತರ);
  sd_bus_error_free (&error);

  ಹಿಂತಿರುಗಿ sd_err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
verify_ebpf_device_filter_installed (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ತುಂಬಿದ_ಮಾರ್ಗ = ಖಾಲಿ;
  size_t n_progs = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ret = append_paths (&ತುಂಬಿದ_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, cgroup_ಮಾರ್ಗ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_ebpf_query_cgroup_progs (ತುಂಬಿದ_ಮಾರ್ಗ, &progs, &n_progs, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (n_progs == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", ತುಂಬಿದ_ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_ಸಂದೇಶ *m, sd_bus_ಸಂದೇಶ *ಉತ್ತರ,
                           sd_bus_error *error, ಪೂರ್ಣಾಂಕ ret)
{
  ಆದರೆ (bus)
    sd_bus_unref (bus);
  ಆದರೆ (m)
    sd_bus_ಸಂದೇಶ_unref (m);
  ಆದರೆ (ಉತ್ತರ)
    sd_bus_ಸಂದೇಶ_unref (ಉತ್ತರ);
  sd_bus_error_free (error);
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ,
                            string_map *annotations,
                            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು,
                            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scope, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತುಂಡು,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_ಗಣ,
                            libcrun_error_t *err)
{
  sd_bus *bus = ಖಾಲಿ;
  sd_bus_ಸಂದೇಶ *m = ಖಾಲಿ;
  sd_bus_ಸಂದೇಶ *ಉತ್ತರ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object = ಖಾಲಿ;
  ರಚನೆ systemd_ಕೆಲಸ_removed_s ಕೆಲಸ_ದತ್ತಾಂಶ = {};
  ಪೂರ್ಣಾಂಕ i;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *boolean_opts[10];
  cleanup_free ಅಕ್ಷರ *state_dir = ಖಾಲಿ;

  *can_retry = false;
  *devices_ಗಣ = false;

  ret = libcrun_get_state_directory (&state_dir, state_ಬೇರು, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = ಖಾಲಿ;

  ret = open_sd_bus_connection (&bus, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  ret = systemd_check_ಕೆಲಸ_ಸ್ಥಿತಿ_ಸಿದ್ಧತೆ (bus, &ಕೆಲಸ_ದತ್ತಾಂಶ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  sd_err = sd_bus_ಸಂದೇಶ_new_ವಿಧಾನ_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  sd_err = sd_bus_ಸಂದೇಶ_append (m, "ss", scope, "fail");
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  sd_err = sd_bus_ಸಂದೇಶ_open_container (m, 'a', "(sv)");
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ಆದರೆ (ತುಂಡು)
    {
      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "Slice", "s", ತುಂಡು);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          ಹೋಗು ನಿರ್ಗಮಿಸು;
        }
    }

  sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "DefaultDependencies", "b", 0);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ಆದರೆ (annotations)
    {
      size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len = ಗಾತ್ರ (SYSTEMD_PROPERTY_ಪೂರ್ವಪ್ರತ್ಯಯ) - 1;
      size_t annotations_len = string_map_ಗಾತ್ರ (annotations);
      size_t i;

      ಗಾಗಿ (i = 0; i < annotations_len; i++)
        {
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ;
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;
          size_t len;

          ret = string_map_get_at (annotations, i, &ಕೀಲಿ, &ಮೌಲ್ಯ);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              ಹೋಗು ನಿರ್ಗಮಿಸು;
            }

          ಆದರೆ (! has_ಪೂರ್ವಪ್ರತ್ಯಯ (ಕೀಲಿ, SYSTEMD_PROPERTY_ಪೂರ್ವಪ್ರತ್ಯಯ))
            ಮುಂದುವರಿ;

          len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಕೀಲಿ);
          ಆದರೆ (len < ಪೂರ್ವಪ್ರತ್ಯಯ_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", ಕೀಲಿ);
              ಹೋಗು ನಿರ್ಗಮಿಸು;
            }

          ret = append_systemd_annotation (m, ಕೀಲಿ + ಪೂರ್ವಪ್ರತ್ಯಯ_len, len - ಪೂರ್ವಪ್ರತ್ಯಯ_len,
                                           ಮೌಲ್ಯ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹೋಗು ನಿರ್ಗಮಿಸು;
        }
    }

  sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "Description", "s", "libcrun container");
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", "PIDs", "au", 1, pid);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ಗಾಗಿ (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_ಸಂದೇಶ_append (m, "(sv)", boolean_opts[i], "b", 1);
      ಆದರೆ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          ಹೋಗು ನಿರ್ಗಮಿಸು;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_ಬಹುಲಕ, scope, devices_ಗಣ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  sd_err = sd_bus_ಸಂದೇಶ_close_container (m);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  sd_err = sd_bus_ಸಂದೇಶ_append (m, "a(sa(sv))", 0);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &ಉತ್ತರ);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ಆದರೆ (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          ಆದರೆ (ಉತ್ತರ)
            sd_bus_ಸಂದೇಶ_unref (ಉತ್ತರ);

          error = SD_BUS_ERROR_NULL;
          ಉತ್ತರ = ಖಾಲಿ;

          sd_err = sd_bus_call (bus, m, 0, &error, &ಉತ್ತರ);
        }
      ಆದರೆ (sd_err < 0)
        {
          ಆದರೆ (sd_err == -EROFS)
            {
              ret = ರಿಜಿಸ್ಟರ್_missing_property_from_ಸಂದೇಶ (state_dir, error.ಸಂದೇಶ_2, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹೋಗು ನಿರ್ಗಮಿಸು;
              ಆದರೆ (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.ಸಂದೇಶ_2 ?: error.name);
          ಹೋಗು ನಿರ್ಗಮಿಸು;
        }
    }

  sd_err = sd_bus_ಸಂದೇಶ_read (ಉತ್ತರ, "o", &object);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ret = systemd_check_ಕೆಲಸ_ಸ್ಥಿತಿ (bus, &ಕೆಲಸ_ದತ್ತಾಂಶ, object, "creating", err);

ನಿರ್ಗಮಿಸು:
  ಹಿಂತಿರುಗಿ cleanup_sd_bus_and_return (bus, m, ಉತ್ತರ, &error, ret);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_destroy_systemd_cgroup_scope (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ,
                                      libcrun_error_t *err)
{
  sd_bus *bus = ಖಾಲಿ;
  sd_bus_ಸಂದೇಶ *m = ಖಾಲಿ;
  sd_bus_ಸಂದೇಶ *ಉತ್ತರ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scope = cgroup_ಸ್ಥಿತಿ->scope;
  ರಚನೆ systemd_ಕೆಲಸ_removed_s ಕೆಲಸ_ದತ್ತಾಂಶ = {};

  ret = open_sd_bus_connection (&bus, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  ret = systemd_check_ಕೆಲಸ_ಸ್ಥಿತಿ_ಸಿದ್ಧತೆ (bus, &ಕೆಲಸ_ದತ್ತಾಂಶ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  ret = sd_bus_ಸಂದೇಶ_new_ವಿಧಾನ_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ret = sd_bus_ಸಂದೇಶ_append (m, "ss", scope, "replace");
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ret = sd_bus_call (bus, m, 0, &error, &ಉತ್ತರ);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.ಸಂದೇಶ_2 ?: error.name);
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ret = sd_bus_ಸಂದೇಶ_read (ಉತ್ತರ, "o", &object);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ret = systemd_check_ಕೆಲಸ_ಸ್ಥಿತಿ (bus, &ಕೆಲಸ_ದತ್ತಾಂಶ, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

ನಿರ್ಗಮಿಸು:
  ಹಿಂತಿರುಗಿ cleanup_sd_bus_and_return (bus, m, ಉತ್ತರ, &error, ret);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
ಕಂಡುಹಿಡಿ_systemd_subgroup (string_map *annotations)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *annotation;

  annotation = ಕಂಡುಹಿಡಿ_string_map_ಮೌಲ್ಯ (annotations, "run.oci.systemd.subgroup");
  ಆದರೆ (annotation)
    {
      ಆದರೆ (annotation[0] == '\0')
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
      ಹಿಂತಿರುಗಿ annotation;
    }

  ಹಿಂತಿರುಗಿ "container";
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_cgroup_enter_systemd (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು,
                              ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಹೊರಹೋಗು,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = ವಾದಗಳು->resources;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ವಾದಗಳು->cgroup_ಮಾರ್ಗ;
  cleanup_free ಅಕ್ಷರ *scope_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *scope = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ತುಂಡು = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ retries_ಎಡ = 32;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id = ವಾದಗಳು->id;
  pid_t pid = ವಾದಗಳು->pid;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ rootless = 0;
  ಪೂರ್ಣಾಂಕ ret;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
    {
      rootless = is_rootless (err);
      ಆದರೆ (UNLIKELY (rootless < 0))
        ಹಿಂತಿರುಗಿ rootless;
    }

  get_systemd_scope_and_ತುಂಡು (id, rootless == 1, cgroup_ಮಾರ್ಗ, &scope, &ತುಂಡು);

  ಗಾಗಿ (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_ಬಹುಲಕ, ವಾದಗಳು->annotations, ವಾದಗಳು->state_ಬೇರು,
                                        scope, ತುಂಡು, pid, &can_retry, &ಹೊರಹೋಗು->bpf_dev_ಗಣ, err);
      ಆದರೆ (LIKELY (ret >= 0))
        ನಿಲ್ಲಿಸು;

      ಆದರೆ (can_retry && retries_ಎಡ-- > 0)
        {
          crun_error_release (err);
          ಮುಂದುವರಿ;
        }

      ಹಿಂತಿರುಗಿ ret;
    }

  suffix = ಕಂಡುಹಿಡಿ_systemd_subgroup (ವಾದಗಳು->annotations);

  ret = systemd_finalize (ವಾದಗಳು, &ಮಾರ್ಗ, cgroup_ಬಹುಲಕ, suffix, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  ಆದರೆ (is_ಖಾಲಿಗಳು_string (suffix))
    scope_ಮಾರ್ಗ = xstrdup (ಮಾರ್ಗ);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      size_t ಮಾರ್ಗ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮಾರ್ಗ);
      size_t suffix_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (suffix);

      scope_ಮಾರ್ಗ = strndup (ಮಾರ್ಗ, ಮಾರ್ಗ_len - suffix_len - 1);
      ಆದರೆ (UNLIKELY (scope_ಮಾರ್ಗ == ಖಾಲಿ))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  ಆದರೆ (ಹೊರಹೋಗು->bpf_dev_ಗಣ)
    {
      ret = verify_ebpf_device_filter_installed (scope_ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹೊರಹೋಗು->ಮಾರ್ಗ = ಮಾರ್ಗ;
  ಮಾರ್ಗ = ಖಾಲಿ;

  ಹೊರಹೋಗು->scope = scope;
  scope = ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ 0;
}

ಅಕ್ಷರ *
get_cgroup_scope_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scope)
{
  ಅಕ್ಷರ *ಮಾರ್ಗ_ಗಮ್ಯ_scope = ಖಾಲಿ;
  ಅಕ್ಷರ *cur;

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (cgroup_ಮಾರ್ಗ))
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಮಾರ್ಗ_ಗಮ್ಯ_scope = xstrdup (cgroup_ಮಾರ್ಗ);

  cur = strchr (ಮಾರ್ಗ_ಗಮ್ಯ_scope, '/');
  ಆಗ (cur)
    {
      ಅಕ್ಷರ *ಮುಂದಿನ = strchr (cur + 1, '/');
      ಆದರೆ (ಮುಂದಿನ == ಖಾಲಿ)
        ನಿಲ್ಲಿಸು;

      *ಮುಂದಿನ = '\0';
      ಆದರೆ (strcmp (cur, scope) == 0)
        ಹಿಂತಿರುಗಿ ಮಾರ್ಗ_ಗಮ್ಯ_scope;
      *ಮುಂದಿನ = '/';

      cur = ಮುಂದಿನ;
      ಆಗ (*cur == '/')
        cur++;
    }

  ಹಿಂತಿರುಗಿ ಮಾರ್ಗ_ಗಮ್ಯ_scope;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_destroy_cgroup_systemd (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ,
                                libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ_ಗಮ್ಯ_scope = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *bpfprog = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ ಬಹುಲಕ;

  ret = cgroup_killall_ಮಾರ್ಗ (cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ, SIGKILL, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_ಸ್ಥಿತಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_ಮಾರ್ಗ_from_scope (&bpfprog, cgroup_ಸ್ಥಿತಿ->scope, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    crun_error_release (err);
  ಇಲ್ಲದಿದ್ದರೆ
    unlink (bpfprog); // Best effort.

  ಮಾರ್ಗ_ಗಮ್ಯ_scope = get_cgroup_scope_ಮಾರ್ಗ (cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ, cgroup_ಸ್ಥಿತಿ->scope);

  ಹಿಂತಿರುಗಿ destroy_cgroup_ಮಾರ್ಗ (ಮಾರ್ಗ_ಗಮ್ಯ_scope, ಬಹುಲಕ, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_update_resources_systemd (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ,
                                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  ರಚನೆ systemd_ಕೆಲಸ_removed_s ಕೆಲಸ_ದತ್ತಾಂಶ = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free ಅಕ್ಷರ *state_dir = ಖಾಲಿ;
  sd_bus_ಸಂದೇಶ *ಉತ್ತರ = ಖಾಲಿ;
  sd_bus_ಸಂದೇಶ *m = ಖಾಲಿ;
  sd_bus *bus = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ sd_err, ret;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;

  ret = libcrun_get_state_directory (&state_dir, state_ಬೇರು, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ret = open_sd_bus_connection (&bus, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = systemd_check_ಕೆಲಸ_ಸ್ಥಿತಿ_ಸಿದ್ಧತೆ (bus, &ಕೆಲಸ_ದತ್ತಾಂಶ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  sd_err = sd_bus_ಸಂದೇಶ_new_ವಿಧಾನ_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  sd_err = sd_bus_ಸಂದೇಶ_append (m, "sb", cgroup_ಸ್ಥಿತಿ->scope, 1);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  sd_err = sd_bus_ಸಂದೇಶ_open_container (m, 'a', "(sv)");
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_ಬಹುಲಕ, cgroup_ಸ್ಥಿತಿ->scope, &cgroup_ಸ್ಥಿತಿ->bpf_dev_ಗಣ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  sd_err = sd_bus_ಸಂದೇಶ_close_container (m);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &ಉತ್ತರ);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.ಸಂದೇಶ_2 ?: error.name);
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ಆದರೆ (cgroup_ಬಹುಲಕ != CGROUP_ಬಹುಲಕ_UNIFIED)
    {
      ret = ಸಿದ್ಧತೆ_rt_runtime (resources, cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು ನಿರ್ಗಮಿಸು;

      ret = ಸಿದ್ಧತೆ_cpuset_for_systemd_v1 (resources, cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ret = ಸಿದ್ಧತೆ_missing_cpu_options_for_systemd (resources, cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED, cgroup_ಸ್ಥಿತಿ->ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  ret = 0;

ನಿರ್ಗಮಿಸು:
  ಹಿಂತಿರುಗಿ cleanup_sd_bus_and_return (bus, m, ಉತ್ತರ, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_attach_process_systemd (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ,
                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, pid_t pid,
                                libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scope = cgroup_ಸ್ಥಿತಿ->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free ಅಕ್ಷರ *scope_ಮಾರ್ಗ = ಖಾಲಿ;
  sd_bus_ಸಂದೇಶ *ಉತ್ತರ = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subcgroup;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆಯ;
  sd_bus *bus = ಖಾಲಿ;
  size_t len;
  ಪೂರ್ಣಾಂಕ sd_err, ret;

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (scope))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_ಮಾರ್ಗ = get_cgroup_scope_ಮಾರ್ಗ (ಮಾರ್ಗ, scope);
  ಆದರೆ (scope_ಮಾರ್ಗ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  ಕೊನೆಯ = strrchr (scope_ಮಾರ್ಗ, '/');
  ಆದರೆ (ಕೊನೆಯ == ಖಾಲಿ || strcmp (ಕೊನೆಯ + 1, scope) != 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, ಮಾರ್ಗ);
  len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (scope_ಮಾರ್ಗ);
  subcgroup = ಮಾರ್ಗ[len] ? ಮಾರ್ಗ + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  sd_err = sd_bus_call_ವಿಧಾನ (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &ಉತ್ತರ, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  ಆದರೆ (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.ಸಂದೇಶ_2 ?: error.name);

  ಹಿಂತಿರುಗಿ cleanup_sd_bus_and_return (bus, ಖಾಲಿ, ಉತ್ತರ, &error, ret);
}

#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_cgroup_enter_systemd (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು,
                              ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಹೊರಹೋಗು,
                              libcrun_error_t *err)
{
  (ಶೂನ್ಯ) ವಾದಗಳು;
  (ಶೂನ್ಯ) ಹೊರಹೋಗು;

  ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_destroy_cgroup_systemd (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ,
                                libcrun_error_t *err)
{
  (ಶೂನ್ಯ) cgroup_ಸ್ಥಿತಿ;

  ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_update_resources_systemd (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ,
                                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (ಶೂನ್ಯ) cgroup_ಸ್ಥಿತಿ;
  (ಶೂನ್ಯ) state_ಬೇರು;
  (ಶೂನ್ಯ) resources;

  ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_attach_process_systemd (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ,
                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, pid_t pid,
                                libcrun_error_t *err)
{
  (ಶೂನ್ಯ) cgroup_ಸ್ಥಿತಿ;
  (ಶೂನ್ಯ) ಮಾರ್ಗ;
  (ಶೂನ್ಯ) pid;

  ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTSUP, "systemd not supported");
}
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = ಖಾಲಿ,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
