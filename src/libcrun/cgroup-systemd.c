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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-systemd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-resources.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ebpf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/vfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/vfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/magic.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
#  include <systemd/sd-bus.h>
#  include <limits.h>

#  define SYSTEMD_PROPERTY_ਅਗੇਤਰ "org.systemd.property."

#  define CGROUP_BFQ_ਭਾਰ_MIN ((uint64_t) 1)
#  define CGROUP_BFQ_ਭਾਰ_DEFAULT ((uint64_t) 100)
#  define CGROUP_BFQ_ਭਾਰ_MAX ((uint64_t) 1000)

#  define CGROUP_ਭਾਰ_MIN ((uint64_t) 1)
#  define CGROUP_ਭਾਰ_DEFAULT ((uint64_t) 100)
#  define CGROUP_ਭਾਰ_MAX ((uint64_t) 10000)

/* The CPU quota period used by the kernel (and systemd) by default, in us.  */
#  define DEFAULT_CPU_QUOTA_PERIOD ((uint64_t) 100000)

#  define SYSTEMD_MISSING_PROPERTIES_DIR ".cache/systemd-missing-properties"

#  define IS_WILDCARD(x) (x <= 0)

ਸਥਿਰ ਪੂਰਨ
ਰਜਿਸਟਰ_missing_property_from_ਸੁਨੇਹਾ (ਅਚਲ ਅੱਖਰ *state_dir, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *file_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *dir_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *property = ਨਿਰਾ;
  ਅੱਖਰ *p;
  ਪੂਰਨ ret;

  ਜੇ (! has_ਅਗੇਤਰ (ਸੁਨੇਹਾ_2, "Cannot set property "))
    ਵਾਪਸ 0;

  property = xstrdup (ਸੁਨੇਹਾ_2 + ਆਕਾਰ ("Cannot set property ") - 1);
  p = strchr (property, ',');
  ਜੇ (! p)
    ਵਾਪਸ 0;
  *p = '\0';

  ret = append_paths (&dir_ਰਸਤਾ, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = crun_ensure_directory (dir_ਰਸਤਾ, 0755, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_paths (&file_ਰਸਤਾ, err, dir_ਰਸਤਾ, property, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_ਰਸਤਾ, ਨਿਰਾ, 0, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 1;
}

ਸਥਿਰ bool
property_missing_p (ਅੱਖਰ **missing_properties, ਅਚਲ ਅੱਖਰ *property)
{
  size_t i;

  ਲਈ (i = 0; missing_properties && missing_properties[i]; i++)
    ਜੇ (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        ਵਾਪਸ true;
      }

  ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ
get_systemd_scope_and_ਟੁਕੜਾ (ਅਚਲ ਅੱਖਰ *id, bool user_ਟੁਕੜਾ, ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ,
                             ਅੱਖਰ **scope, ਅੱਖਰ **ਟੁਕੜਾ)
{
  ਅਚਲ ਅੱਖਰ *n;
  ਅੱਖਰ *p;

  ਜੇ (cgroup_ਰਸਤਾ == ਨਿਰਾ || cgroup_ਰਸਤਾ[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      ਵਾਪਸ;
    }

  n = strchr (cgroup_ਰਸਤਾ, ':');
  ਜੇ (n == ਨਿਰਾ)
    xasprintf (scope, "%s.scope", cgroup_ਰਸਤਾ);
  ਨਹੀਂਤਾਂ
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      ਜੇ (p)
        *p = '-';
    }
  ਜੇ (ਟੁਕੜਾ)
    {
      *ਟੁਕੜਾ = xstrdup (cgroup_ਰਸਤਾ);
      p = strchr (*ਟੁਕੜਾ, ':');
      ਜੇ (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (*ਟੁਕੜਾ))
        {
          ਮੁਕਤ (*ਟੁਕੜਾ);
          ਜੇ (user_ਟੁਕੜਾ)
            *ਟੁਕੜਾ = xstrdup ("user.slice");
          ਨਹੀਂਤਾਂ
            *ਟੁਕੜਾ = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ dirfd = -1;
  bool need_ਸੈੱਟ_ਮਾਪਾ = true;
  ਅੱਖਰ fmt_buf[64];
  ਪੂਰਨ len;
  ਪੂਰਨ ret;

  ਜੇ (resources == ਨਿਰਾ || resources->cpu == ਨਿਰਾ)
    ਵਾਪਸ 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਰਸਤਾ))
    ਵਾਪਸ crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  ਜੇ (has_suffix (ਰਸਤਾ, ".scope"))
    need_ਸੈੱਟ_ਮਾਪਾ = false;

  ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, "cpu", ਰਸਤਾ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = crun_ensure_directory (cgroup_ਰਸਤਾ, 0755, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  dirfd = open (cgroup_ਰਸਤਾ, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
  ਜੇ (UNLIKELY (dirfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", cgroup_ਰਸਤਾ);

  ਜੇ (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ਜੇ (need_ਸੈੱਟ_ਮਾਪਾ)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, ਆਕਾਰ (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (fmt_buf)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ਜੇ (need_ਸੈੱਟ_ਮਾਪਾ)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ਮਾਪਾ_2;
  ਪੂਰਨ ret;

  ਜੇ (resources == ਨਿਰਾ || resources->cpu == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਜੇ (! resources->cpu->ਮਿਆਦ_present && ! (cgroup2 && resources->cpu->idle_present))
    ਵਾਪਸ 0;

  ਲਈ (ਮਾਪਾ_2 = 0; ਮਾਪਾ_2 < 2; ਮਾਪਾ_2++)
    {
      cleanup_close ਪੂਰਨ dirfd = -1;

      ਜੇ (cgroup2)
        ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ ? ਰਸਤਾ : "", (ਮਾਪਾ_2 ? ".." : ਨਿਰਾ), ਨਿਰਾ);
      ਨਹੀਂਤਾਂ
        ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, "/cpu", ਰਸਤਾ ? ਰਸਤਾ : "", (ਮਾਪਾ_2 ? ".." : ਨਿਰਾ), ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      dirfd = open (cgroup_ਰਸਤਾ, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
      ਜੇ (UNLIKELY (dirfd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", cgroup_ਰਸਤਾ);

      ret = write_cpu_ਮਿਆਦ (dirfd, cgroup2, resources->cpu, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      ਜੇ (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ਮਾਪਾ_2;
  ਪੂਰਨ ret;
  ਪੂਰਨ ਪੱਧਰ;

  ret = append_paths (&cgroup_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, "/cpuset", ਰਸਤਾ ? ਰਸਤਾ : "", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = crun_ensure_directory (cgroup_ਰਸਤਾ, 0755, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_ਰਸਤਾ, resources, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  ਪੱਧਰ = has_suffix (ਰਸਤਾ, ".scope") ? 1 : 2;

  ਲਈ (ਮਾਪਾ_2 = 0; ਮਾਪਾ_2 < ਪੱਧਰ; ਮਾਪਾ_2++)
    {
      cleanup_close ਪੂਰਨ dirfd_cpuset = -1;
      cleanup_free ਅੱਖਰ *ਰਸਤਾ_ਮੰਜ਼ਿਲ_cpuset = ਨਿਰਾ;

      ret = append_paths (&ਰਸਤਾ_ਮੰਜ਼ਿਲ_cpuset, err, CGROUP_ਜੜ੍ਹ "/cpuset", ਰਸਤਾ, (ਮਾਪਾ_2 ? ".." : ਨਿਰਾ), ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      dirfd_cpuset = open (ਰਸਤਾ_ਮੰਜ਼ਿਲ_cpuset, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC);
      ਜੇ (UNLIKELY (dirfd_cpuset < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", ਰਸਤਾ_ਮੰਜ਼ਿਲ_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
ਸਥਿਰ ਪੂਰਨ
compute_finalized_ਰਸਤਾ (ਅੱਖਰ **ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *from, ਅਚਲ ਅੱਖਰ *suffix, libcrun_error_t *err)
{
  ਜੇ (suffix == ਨਿਰਾ)
    {
      *ਰਸਤਾ = xstrdup (from);
      ਵਾਪਸ 0;
    }
  ਵਾਪਸ append_paths (ਰਸਤਾ, err, from, suffix, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ
systemd_finalize (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ, ਅੱਖਰ **ਰਸਤਾ_ਬਾਹਰਲਾ,
                  ਪੂਰਨ cgroup_ਬਹੁਲਕ, ਅਚਲ ਅੱਖਰ *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = ਦਲੀਲਾਂ->resources;
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  pid_t pid = ਦਲੀਲਾਂ->pid;
  ਪੂਰਨ ret;
  ਅੱਖਰ *from, *ਮੰਜ਼ਿਲ;
  ਅੱਖਰ *saveptr = ਨਿਰਾ;

  cleanup_free ਅੱਖਰ *proc_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ fd = -1;

  xasprintf (&proc_ਰਸਤਾ, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (ਦਲੀਲਾਂ->container, proc_ਰਸਤਾ, O_RDONLY, err);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ fd;

  ret = read_all_fd (fd, "cgroup path", &content, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਚੋਣ (cgroup_ਬਹੁਲਕ)
    {
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_HYBRID:
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_LEGACY:
      from = ਲੜੀ_ਖੋਜ (content, ":memory:");
      ਜੇ (LIKELY (from != ਨਿਰਾ))
        from += 8;
      ਨਹੀਂਤਾਂ
        {
          from = ਲੜੀ_ਖੋਜ (content, ":pids:");
          ਜੇ (UNLIKELY (from == ਨਿਰਾ))
            ਵਾਪਸ crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      ਮੰਜ਼ਿਲ = strchr (from, '\n');
      ਜੇ (UNLIKELY (ਮੰਜ਼ਿਲ == ਨਿਰਾ))
        ਵਾਪਸ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *ਮੰਜ਼ਿਲ = '\0';
      ret = compute_finalized_ਰਸਤਾ (&ਰਸਤਾ, from, suffix, err);
      *ਮੰਜ਼ਿਲ = '\n';
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (geteuid ())
        ਵਾਪਸ 0;

      ret = ਤਿਆਰੀ_rt_runtime (resources, ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = ਤਿਆਰੀ_cpuset_for_systemd_v1 (resources, ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਲਈ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ਨਿਰਾ, "\n", &saveptr))
        {
          ਅੱਖਰ *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          ਜੇ (subsystem[0] == '\0')
            {
              ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_LEGACY)
                ਜਾਰੀ;

              subsystem = "unified";
            }

          ਜੇ (strcmp (subpath, ਰਸਤਾ))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, ਰਸਤਾ, true, err);
              ਜੇ (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  ਜੇ (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      ਜਾਰੀ;
                    }
                  ਵਾਪਸ ret;
                }
            }
        }

      ਰੋਕੋ;

    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_UNIFIED:
      {
        cleanup_free ਅੱਖਰ *dir = ਨਿਰਾ;

        from = ਲੜੀ_ਖੋਜ (content, "0::");
        ਜੇ (UNLIKELY (from == ਨਿਰਾ))
          ਵਾਪਸ crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        ਮੰਜ਼ਿਲ = strchr (from, '\n');
        ਜੇ (UNLIKELY (ਮੰਜ਼ਿਲ == ਨਿਰਾ))
          ਵਾਪਸ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *ਮੰਜ਼ਿਲ = '\0';
        ret = compute_finalized_ਰਸਤਾ (&ਰਸਤਾ, from, suffix, err);
        *ਮੰਜ਼ਿਲ = '\n';
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        ret = append_paths (&dir, err, CGROUP_ਜੜ੍ਹ, ਰਸਤਾ, ਨਿਰਾ);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        ret = move_process_ਮੰਜ਼ਿਲ_cgroup (pid, ਨਿਰਾ, ਰਸਤਾ, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        ret = enable_controllers (ਰਸਤਾ, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        ਜੇ (suffix)
          {
            ret = chown_cgroups (ਰਸਤਾ, ਦਲੀਲਾਂ->ਜੜ੍ਹ_uid, ਦਲੀਲਾਂ->ਜੜ੍ਹ_gid, err);
            ਜੇ (UNLIKELY (ret < 0))
              ਵਾਪਸ ret;
          }
      }
      ਰੋਕੋ;

    ਮੂਲ:
      ਵਾਪਸ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_ਬਹੁਲਕ);
    }

  ret = ਤਿਆਰੀ_missing_cpu_options_for_systemd (resources, cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED, ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  *ਰਸਤਾ_ਬਾਹਰਲਾ = ਰਸਤਾ;
  ਰਸਤਾ = ਨਿਰਾ;

  ਵਾਪਸ 0;
}

ਬਣਤਰ systemd_ਕੰਮ_removed_s
{
  ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *op;
  ਪੂਰਨ terminated;
  libcrun_error_t err;
};

ਸਥਿਰ ਪੂਰਨ
systemd_ਕੰਮ_removed (sd_bus_ਸੁਨੇਹਾ *m, ਖਾਲੀ *userdata, sd_bus_error *error arg_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, *unit, *ਨਤੀਜਾ;
  uint32_t id;
  ਪੂਰਨ ret;
  ਬਣਤਰ systemd_ਕੰਮ_removed_s *d = userdata;

  ret = sd_bus_ਸੁਨੇਹਾ_read (m, "uoss", &id, &ਰਸਤਾ, &unit, &ਨਤੀਜਾ);
  ਜੇ (ret < 0)
    ਵਾਪਸ -1;

  ਜੇ (strcmp (d->ਰਸਤਾ, ਰਸਤਾ) == 0)
    {
      d->terminated = 1;
      ਜੇ (strcmp (ਨਤੀਜਾ, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, ਨਤੀਜਾ);
          ਵਾਪਸ -1;
        }
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
systemd_check_ਕੰਮ_ਸਥਿਤੀ_ਤਿਆਰੀ (sd_bus *bus, ਬਣਤਰ systemd_ਕੰਮ_removed_s *ਅੰਕੜੇ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = sd_bus_ਮੇਲ_signal_async (bus, ਨਿਰਾ, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_ਕੰਮ_removed, ਨਿਰਾ, ਅੰਕੜੇ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, -ret, "sd-bus match signal");

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
systemd_check_ਕੰਮ_ਸਥਿਤੀ (sd_bus *bus, ਬਣਤਰ systemd_ਕੰਮ_removed_s *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *op,
                          libcrun_error_t *err)
{
  ਪੂਰਨ sd_err;

  ਅੰਕੜੇ->ਰਸਤਾ = ਰਸਤਾ;
  ਅੰਕੜੇ->op = op;
  ਜਦੋਂ (! ਅੰਕੜੇ->terminated)
    {
      sd_err = sd_bus_process (bus, ਨਿਰਾ);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus process");

      ਜੇ (sd_err != 0)
        ਜਾਰੀ;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus wait");
    }

  ਜੇ (ਅੰਕੜੇ->err != ਨਿਰਾ)
    {
      *err = ਅੰਕੜੇ->err;
      ਵਾਪਸ -1;
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
parse_sd_array (ਅੱਖਰ *s, ਅੱਖਰ **ਬਾਹਰਲਾ, ਅੱਖਰ **ਅਗਲਾ, libcrun_error_t *err)
{
  ਅੱਖਰ endchr;
  ਅੱਖਰ *it, *dest;
  bool escaped = false;

  *ਬਾਹਰਲਾ = ਨਿਰਾ;
  *ਅਗਲਾ = ਨਿਰਾ;

  ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ (*s))
    s++;
  ਜੇ (*s == '\0')
    ਵਾਪਸ 0;
  ਨਹੀਂਤਾਂ ਜੇ (*s != '\'' && *s != '"')
    ਵਾਪਸ crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *ਬਾਹਰਲਾ = dest = it;

  ਜਦੋਂ (1)
    {
      ਜੇ (*it == '\0')
        ਵਾਪਸ crun_make_error (err, 0, "invalid string `%s`", s);
      ਜੇ (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ (*it))
            it++;
          ਜੇ (*it == ',')
            {
              *ਅਗਲਾ = ++it;
              *dest = '\0';
              ਵਾਪਸ 0;
            }

          ਜੇ (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              ਵਾਪਸ 0;
            }

          ਵਾਪਸ crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      ਜੇ (! escaped)
        *dest++ = *it;
      it++;
    }

  ਵਾਪਸ 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
ਸਥਿਰ ਪੂਰਨ
append_systemd_annotation (sd_bus_ਸੁਨੇਹਾ *m, ਅਚਲ ਅੱਖਰ *name, size_t name_len, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
                           libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *tmp_name = ਨਿਰਾ;
  uint32_t ਗੁਣਕ = 1;
  ਅਚਲ ਅੱਖਰ *it;
  ਪੂਰਨ sd_err;

  ਜਦੋਂ (*ਮੁੱਲ == ' ')
    ਮੁੱਲ++;

  it = ਮੁੱਲ;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  ਜੇ (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
      && name[name_len - 1] == 'c')
    {
      ਗੁਣਕ = 1000000;

      tmp_name = xmalloc (name_len + 2);
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (tmp_name, name, name_len - 3);
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (tmp_name + name_len - 3, "USec", 5);

      name = tmp_name;
    }

  /* A *USec property is always an uint64, so, like runc does, accept any
     number of seconds (possibly with a fractional part, and with an
     optional integer type), and convert it to uint64 microseconds.  */
  ਜੇ (ਗੁਣਕ != 1)
    {
      ਸਥਿਰ ਅਚਲ ਅੱਖਰ *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", ਨਿਰਾ };
      ਅਚਲ ਅੱਖਰ *ਅੰਕ = it;
      ਅੱਖਰ *endptr = ਨਿਰਾ;
      ਦੁੱਗਣਾ v;
      size_t i;

      ਲਈ (i = 0; types[i]; i++)
        ਜੇ (has_ਅਗੇਤਰ (ਅੰਕ, types[i]))
          {
            ਅੰਕ += ਲੜੀ_ਲੰਬਾਈ (types[i]);
            ਰੋਕੋ;
          }

      errno = 0;
      v = strtod (ਅੰਕ, &endptr);
      ਜੇ (UNLIKELY (errno != 0 || endptr == ਅੰਕ || *endptr || v < 0 || v * ਗੁਣਕ >= 18446744073709551615.0))
        ਵਾਪਸ crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, ਮੁੱਲ);

      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", name, "t", (uint64_t) (v * ਗੁਣਕ));
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ਵਾਪਸ 0;
    }

  ਜੇ ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", name, "b", b);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ਵਾਪਸ 0;
    }
  ਨਹੀਂਤਾਂ ਜੇ (*it == '\'')
    {
      cleanup_free ਅੱਖਰ *v_ਸ਼ੁਰੂ = ਨਿਰਾ;
      ਅੱਖਰ *ਸਿਰਾ;

      it = v_ਸ਼ੁਰੂ = xstrdup (ਮੁੱਲ);

      ਸਿਰਾ = strchr (v_ਸ਼ੁਰੂ + 1, '\'');
      ਜੇ (ਸਿਰਾ == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "invalid variant `%s`", ਮੁੱਲ);
      *ਸਿਰਾ = '\0';

      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", name, "s", it + 1);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ਵਾਪਸ 0;
    }
  ਨਹੀਂਤਾਂ ਜੇ (*it == '[')
    {
      cleanup_free ਅੱਖਰ *v_ਸ਼ੁਰੂ = ਨਿਰਾ;
      size_t n_parts = 0, parts_ਮਾਪ = 32;
      cleanup_free ਅੱਖਰ **parts = xmalloc (ਆਕਾਰ (ਅੱਖਰ *) * parts_ਮਾਪ);
      ਅੱਖਰ *part;

      part = v_ਸ਼ੁਰੂ = xstrdup (it + 1);
      ਜਦੋਂ (1)
        {
          ਅੱਖਰ *ਬਾਹਰਲਾ = ਨਿਰਾ, *ਅਗਲਾ = ਨਿਰਾ;
          ਪੂਰਨ ret;

          ret = parse_sd_array (part, &ਬਾਹਰਲਾ, &ਅਗਲਾ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          parts[n_parts++] = ਬਾਹਰਲਾ;
          ਜੇ (n_parts == parts_ਮਾਪ - 1)
            {
              parts_ਮਾਪ += 32;
              parts = xrealloc (parts, ਆਕਾਰ (ਅੱਖਰ *) * parts_ਮਾਪ);
            }
          parts[n_parts] = ਨਿਰਾ;
          ਜੇ (ਅਗਲਾ == ਨਿਰਾ)
            ਰੋਕੋ;

          part = ਅਗਲਾ;
        }

      sd_err = sd_bus_ਸੁਨੇਹਾ_open_container (m, 'r', "sv");
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "s", name);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_ਸੁਨੇਹਾ_open_container (m, 'v', "as");
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_ਸੁਨੇਹਾ_append_strv (m, parts);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_ਸੁਨੇਹਾ_close_container (m);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_ਸੁਨੇਹਾ_close_container (m);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus close container");

      ਵਾਪਸ 0;
    }
  ਨਹੀਂਤਾਂ ਜੇ (has_ਅਗੇਤਰ (it, "uint64 "))
    {
      ਅੱਖਰ *endptr = ਨਿਰਾ;
      uint64_t v;

      errno = 0;
      v = strtoull (it + ਆਕਾਰ ("uint64"), &endptr, 10);
      ਜੇ (UNLIKELY (errno != 0 || *endptr))
        ਵਾਪਸ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", name, "t", (uint64_t) (v * ਗੁਣਕ));
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ਵਾਪਸ 0;
    }
  ਨਹੀਂਤਾਂ ਜੇ (has_ਅਗੇਤਰ (it, "int64 "))
    {
      ਅੱਖਰ *endptr = ਨਿਰਾ;
      int64_t v;

      errno = 0;
      v = strtoll (it + ਆਕਾਰ ("int64"), &endptr, 10);
      ਜੇ (UNLIKELY (errno != 0 || *endptr))
        ਵਾਪਸ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", name, "x", (int64_t) (v * ਗੁਣਕ));
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ਵਾਪਸ 0;
    }
  ਨਹੀਂਤਾਂ ਜੇ (has_ਅਗੇਤਰ (it, "uint32 "))
    {
      ਅੱਖਰ *endptr = ਨਿਰਾ;
      uint32_t v;

      errno = 0;
      v = strtoul (it + ਆਕਾਰ ("uint32"), &endptr, 10);
      ਜੇ (UNLIKELY (errno != 0 || *endptr))
        ਵਾਪਸ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", name, "u", (uint32_t) (v * ਗੁਣਕ));
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ਵਾਪਸ 0;
    }
  ਨਹੀਂਤਾਂ ਜੇ (has_ਅਗੇਤਰ (it, "int32 ") || strchr (it, ' ') == ਨਿਰਾ)
    {
      ਅੱਖਰ *endptr = ਨਿਰਾ;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      ਜੇ (has_ਅਗੇਤਰ (it, "int32 "))
        v = strtol (it + ਆਕਾਰ ("int32"), &endptr, 10);
      ਨਹੀਂਤਾਂ
        v = strtol (it, &endptr, 10);
      ਜੇ (UNLIKELY (errno != 0 || *endptr))
        ਵਾਪਸ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", name, "i", (int32_t) (v * ਗੁਣਕ));
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ਵਾਪਸ 0;
    }

  ਵਾਪਸ crun_make_error (err, 0, "unknown type for `%s`", name);
}

ਸਥਿਰ ਪੂਰਨ
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  ਪੂਰਨ rootless;
  ਪੂਰਨ sd_err = 0;

  rootless = is_rootless (err);
  ਜੇ (UNLIKELY (rootless < 0))
    ਵਾਪਸ rootless;

  ਜੇ (rootless)
    sd_err = sd_bus_default_user (bus);
  ਜੇ (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  ਜੇ (sd_err < 0)
    ਵਾਪਸ crun_make_error (err, -sd_err, "cannot open sd-bus");

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ਅਚਲ ਅੱਖਰ *name)
{
  size_t i;

  ਜੇ (resources == ਨਿਰਾ || resources->unified == ਨਿਰਾ)
    ਵਾਪਸ ਨਿਰਾ;

  ਲਈ (i = 0; i < resources->unified->len; i++)
    ਜੇ (strcmp (resources->unified->ਕੁੰਜੀਆਂ[i], name) == 0)
      ਵਾਪਸ resources->unified->ਮੁੱਲਾਂ[i];
  ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ
get_ਮੁੱਲ_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ਅਚਲ ਅੱਖਰ *name,
                            uint64_t *ਮੁੱਲ, libcrun_error_t *err)
{
  ਅੱਖਰ *endptr = ਨਿਰਾ;
  size_t i;

  ਜੇ (resources == ਨਿਰਾ || resources->unified == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਲਈ (i = 0; i < resources->unified->len; i++)
    ਜੇ (strcmp (resources->unified->ਕੁੰਜੀਆਂ[i], name) == 0)
      {
        ਜੇ (strcmp (resources->unified->ਮੁੱਲਾਂ[i], "max") == 0)
          {
            *ਮੁੱਲ = UINT64_MAX;
            ਵਾਪਸ 1;
          }

        errno = 0;
        *ਮੁੱਲ = strtoull (resources->unified->ਮੁੱਲਾਂ[i], &endptr, 10);
        ਜੇ (UNLIKELY (errno))
          ਵਾਪਸ crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->ਮੁੱਲਾਂ[i]);
        ਜੇ (endptr && *endptr)
          ਵਾਪਸ crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->ਮੁੱਲਾਂ[i]);
        ਵਾਪਸ 1;
      }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
get_ਸਮ੍ਰਿਤੀ_ਨੀਵਾਂ (runtime_spec_schema_config_linux_resources *resources, uint64_t *ਹੱਦਬੰਦੀ, libcrun_error_t *err)
{
  ਜੇ (resources->ਸਮ੍ਰਿਤੀ && resources->ਸਮ੍ਰਿਤੀ->reservation_present)
    {
      *ਹੱਦਬੰਦੀ = resources->ਸਮ੍ਰਿਤੀ->reservation;
      ਵਾਪਸ 1;
    }

  ਵਾਪਸ get_ਮੁੱਲ_from_unified_map (resources, "memory.low", ਹੱਦਬੰਦੀ, err);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *ਹੱਦਬੰਦੀ, libcrun_error_t *err)
{
  ਜੇ (resources->pids && resources->pids->ਹੱਦਬੰਦੀ)
    {
      *ਹੱਦਬੰਦੀ = resources->pids->ਹੱਦਬੰਦੀ;
      ਵਾਪਸ 1;
    }

  ਵਾਪਸ get_ਮੁੱਲ_from_unified_map (resources, "pids.max", ਹੱਦਬੰਦੀ, err);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
get_ਸਮ੍ਰਿਤੀ_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *ਹੱਦਬੰਦੀ, libcrun_error_t *err)
{
  ਜੇ (resources->ਸਮ੍ਰਿਤੀ && resources->ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ_present)
    {
      *ਹੱਦਬੰਦੀ = resources->ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ;
      ਵਾਪਸ 1;
    }

  ਵਾਪਸ get_ਮੁੱਲ_from_unified_map (resources, "memory.max", ਹੱਦਬੰਦੀ, err);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
get_ਸਮ੍ਰਿਤੀ_ਅਦਲਾ_ਬਦਲੀ_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *ਹੱਦਬੰਦੀ, libcrun_error_t *err)
{
  ਜੇ (resources->ਸਮ੍ਰਿਤੀ && resources->ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ_present)
    {
      runtime_spec_schema_config_linux_resources_ਸਮ੍ਰਿਤੀ *ਸਮ੍ਰਿਤੀ = resources->ਸਮ੍ਰਿਤੀ;
      int64_t mem = ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ_present ? ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      ਜੇ (ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ == -1 || (mem == -1 && ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ == 0))
        {
          *ਹੱਦਬੰਦੀ = UINT64_MAX;
          ਵਾਪਸ 1;
        }
      /* 0 means unset.  */
      ਜੇ (ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ == 0)
        ਵਾਪਸ 0;
      ਜੇ (mem <= 0)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ਜੇ (ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ < mem)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *ਹੱਦਬੰਦੀ = ਸਮ੍ਰਿਤੀ->ਅਦਲਾ_ਬਦਲੀ - mem;
      ਵਾਪਸ 1;
    }

  ਵਾਪਸ get_ਮੁੱਲ_from_unified_map (resources, "memory.swap.max", ਹੱਦਬੰਦੀ, err);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
get_cpu_ਭਾਰ (runtime_spec_schema_config_linux_resources *resources, uint64_t *ਭਾਰ, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t ਮੁੱਲ;
  ਪੂਰਨ ret;

  ret = get_ਮੁੱਲ_from_unified_map (resources, "cpu.idle", &ਮੁੱਲ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  ਜੇ (ret > 0 && ਮੁੱਲ == 1)
    has_idle = true;
  ਜੇ (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_ਮੁੱਲ_from_unified_map (resources, "cpu.weight", ਭਾਰ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  ਜੇ (ret > 0)
    {
      ਜੇ (has_idle)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      ਵਾਪਸ 1;
    }

  ਜੇ (resources->cpu && resources->cpu->shares_present)
    {
      ਜੇ (has_idle)
        ਵਾਪਸ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      ਜੇ (resources->cpu->shares == 0)
        ਵਾਪਸ 0;
      *ਭਾਰ = convert_shares_ਮੰਜ਼ਿਲ_ਭਾਰ (resources->cpu->shares);
      ਵਾਪਸ 1;
    }
  ਜੇ (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *ਭਾਰ = 0;
      ਵਾਪਸ 1;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
read_uint64_ਮੁੱਲ (ਅਚਲ ਅੱਖਰ *str, uint64_t *ਮੁੱਲ, libcrun_error_t *err)
{
  ਅੱਖਰ *endptr = ਨਿਰਾ;

  errno = 0;
  *ਮੁੱਲ = strtoull (str, &endptr, 10);
  ਜੇ (UNLIKELY (errno))
    ਵਾਪਸ crun_make_error (err, errno, "invalid value: %s", str);
  ਜੇ (endptr && *endptr)
    ਵਾਪਸ crun_make_error (err, 0, "invalid value: %s", str);
  ਵਾਪਸ 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ uint64_t
IO_ਭਾਰ (uint64_t bfq_ਭਾਰ)
{
  ਵਾਪਸ bfq_ਭਾਰ <= CGROUP_BFQ_ਭਾਰ_DEFAULT ? CGROUP_ਭਾਰ_DEFAULT - (CGROUP_BFQ_ਭਾਰ_DEFAULT - bfq_ਭਾਰ) * (CGROUP_ਭਾਰ_DEFAULT - CGROUP_ਭਾਰ_MIN) / (CGROUP_BFQ_ਭਾਰ_DEFAULT - CGROUP_BFQ_ਭਾਰ_MIN) : CGROUP_ਭਾਰ_DEFAULT + (bfq_ਭਾਰ - CGROUP_BFQ_ਭਾਰ_DEFAULT) * (CGROUP_ਭਾਰ_MAX - CGROUP_ਭਾਰ_DEFAULT) / (CGROUP_BFQ_ਭਾਰ_MAX - CGROUP_BFQ_ਭਾਰ_DEFAULT);
}

ਸਥਿਰ ਪੂਰਨ
append_io_ਭਾਰ (sd_bus_ਸੁਨੇਹਾ *m, ਅੱਖਰ **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t ਭਾਰ;
  ਪੂਰਨ sd_err;
  size_t i;
  ਪੂਰਨ ret;

#  define APPEND_IO_ਭਾਰ(ਮੁੱਲ)                                                      \
    ਕਰੋ                                                                                 \
      {                                                                                \
        ਜੇ (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "IOWeight", "t", ਭਾਰ);       \
            ਜੇ (UNLIKELY (sd_err < 0))                                                 \
              ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } ਜਦੋਂ (0)

#  define APPEND_IO_DEVICE_ਭਾਰ(device, ਮੁੱਲ)                                                      \
    ਕਰੋ                                                                                                \
      {                                                                                               \
        ਜੇ (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, ਭਾਰ); \
            ਜੇ (UNLIKELY (sd_err < 0))                                                                \
              ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } ਜਦੋਂ (0)

  ਜੇ (resources->ਖੰਡ_io && resources->ਖੰਡ_io->ਭਾਰ_present)
    {
      ਭਾਰ = IO_ਭਾਰ (resources->ਖੰਡ_io->ਭਾਰ);
      APPEND_IO_ਭਾਰ (ਭਾਰ);
    }

  ਲਈ (i = 0; resources->ਖੰਡ_io && i < resources->ਖੰਡ_io->ਭਾਰ_device_len; i++)
    {
      ਅੱਖਰ name[64];
      ਪੂਰਨ len;

      len = snprintf (name, ਆਕਾਰ (name), "%" PRIu64 ":%" PRIu64,
                      resources->ਖੰਡ_io->ਭਾਰ_device[i]->major,
                      resources->ਖੰਡ_io->ਭਾਰ_device[i]->minor);
      ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (name)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ਭਾਰ = IO_ਭਾਰ (resources->ਖੰਡ_io->ਭਾਰ_device[i]->ਭਾਰ);
      APPEND_IO_DEVICE_ਭਾਰ (name, ਭਾਰ);
    }

  ਲਈ (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free ਅੱਖਰ *ਮੁੱਲ = ਨਿਰਾ;
      bool need_conversion = false;
      ਅੱਖਰ *saveptr = ਨਿਰਾ;
      ਅੱਖਰ *ਸਤਰ;

      ਜੇ (strcmp (resources->unified->ਕੁੰਜੀਆਂ[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (resources->unified->ਕੁੰਜੀਆਂ[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      ਨਹੀਂਤਾਂ
        {
          /* Skip other keys.  */
          ਜਾਰੀ;
        }

      ਮੁੱਲ = xstrdup (resources->unified->ਮੁੱਲਾਂ[i]);

      ਲਈ (ਸਤਰ = strtok_r (ਮੁੱਲ, "\n", &saveptr); ਸਤਰ; ਸਤਰ = strtok_r (ਨਿਰਾ, "\n", &saveptr))
        {
          ਅੱਖਰ *sep;

          sep = strchr (ਸਤਰ, ' ');
          ਜਦੋਂ (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_ਮੁੱਲ (sep ? sep : ਸਤਰ, &ਭਾਰ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          ਜੇ (need_conversion)
            ਭਾਰ = IO_ਭਾਰ (ਭਾਰ);

          ਜੇ (sep == ਨਿਰਾ || strcmp (ਸਤਰ, "default") == 0)
            APPEND_IO_ਭਾਰ (ਭਾਰ);
          ਨਹੀਂਤਾਂ
            APPEND_IO_DEVICE_ਭਾਰ (ਸਤਰ, ਭਾਰ);
        }
    }

  ਵਾਪਸ 0;
}

/* Adapted from systemd.  */
ਸਥਿਰ ਪੂਰਨ
bus_append_byte_array (sd_bus_ਸੁਨੇਹਾ *m, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ, ਅਚਲ ਖਾਲੀ *buf, size_t n, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = sd_bus_ਸੁਨੇਹਾ_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  ਜੇ (ret < 0)
    ਵਾਪਸ crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_ਸੁਨੇਹਾ_append_basic (m, SD_BUS_TYPE_STRING, ਖੇਤਰ);
  ਜੇ (ret < 0)
    ਵਾਪਸ crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_ਸੁਨੇਹਾ_open_container (m, 'v', "ay");
  ਜੇ (ret < 0)
    ਵਾਪਸ crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_ਸੁਨੇਹਾ_append_array (m, 'y', buf, n);
  ਜੇ (ret < 0)
    ਵਾਪਸ crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_ਸੁਨੇਹਾ_close_container (m);
  ਜੇ (ret < 0)
    ਵਾਪਸ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_ਸੁਨੇਹਾ_close_container (m);
  ਜੇ (ret < 0)
    ਵਾਪਸ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ
append_uint64_from_unified_map (sd_bus_ਸੁਨੇਹਾ *m,
                                ਅੱਖਰ **missing_properties,
                                ਅਚਲ ਅੱਖਰ *attr,
                                ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t ਮੁੱਲ;
  ਪੂਰਨ ret;

  ਜੇ (property_missing_p (missing_properties, attr))
    ਵਾਪਸ 0;

  ret = get_ਮੁੱਲ_from_unified_map (resources, ਕੁੰਜੀ, &ਮੁੱਲ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  ਜੇ (ret)
    {
      ਪੂਰਨ sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", attr, "t", ਮੁੱਲ);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  ਵਾਪਸ 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
ਸਥਿਰ ਪੂਰਨ
append_device_allow (sd_bus_ਸੁਨੇਹਾ *m,
                     ਅਚਲ ਅੱਖਰ type,
                     ਪੂਰਨ major,
                     ਪੂਰਨ minor,
                     ਅਚਲ ਅੱਖਰ *access,
                     libcrun_error_t *err)
{
  ਅੱਖਰ device[64];
  ਪੂਰਨ sd_err;
  ਪੂਰਨ len;

  ਜੇ (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      ਵਾਪਸ 0;
    }

  ਜੇ (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, ਆਕਾਰ (device), "%s-*", type == 'c' ? "char" : "block");
  ਨਹੀਂਤਾਂ ਜੇ (IS_WILDCARD (minor))
    len = snprintf (device, ਆਕਾਰ (device), type == 'c' ? "char-%d" : "block-%d", major);
  ਨਹੀਂਤਾਂ
    len = snprintf (device, ਆਕਾਰ (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (device)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  ਜੇ (UNLIKELY (sd_err < 0))
    ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  ਵਾਪਸ 0;
}

#  define ਬਹੁਲਕ_R 1
#  define ਬਹੁਲਕ_W 2
#  define ਬਹੁਲਕ_M 4

ਸਥਿਰ ਪੂਰਨ
access_ਮੰਜ਼ਿਲ_ਬਹੁਲਕ (ਅਚਲ ਅੱਖਰ *access, libcrun_error_t *err)
{
  ਪੂਰਨ ਬਹੁਲਕ = 0;

  ਜਦੋਂ (access && *access)
    {
      ਪੂਰਨ c = *access++;

      ਚੋਣ (c)
        {
        ਹਾਲਤ 'r':
          ਬਹੁਲਕ |= ਬਹੁਲਕ_R;
          ਰੋਕੋ;

        ਹਾਲਤ 'w':
          ਬਹੁਲਕ |= ਬਹੁਲਕ_W;
          ਰੋਕੋ;

        ਹਾਲਤ 'm':
          ਬਹੁਲਕ |= ਬਹੁਲਕ_M;
          ਰੋਕੋ;

        ਮੂਲ:
          ਵਾਪਸ crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  ਵਾਪਸ ਬਹੁਲਕ;
}

/* check if the type b is included in type a.  */
ਸਥਿਰ bool
has_same_type (ਅਚਲ ਅੱਖਰ *a, ਅਚਲ ਅੱਖਰ *b)
{
  bool a_all = a == ਨਿਰਾ || strcmp (a, "a") == 0;
  ਜੇ (b == ਨਿਰਾ || strcmp (b, "a") == 0)
    ਵਾਪਸ a_all;

  ਵਾਪਸ a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
ਸਥਿਰ ਪੂਰਨ
is_deny_ਨਿਯਮ_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  ਪੂਰਨ req_ਬਹੁਲਕ;

#  define CMP_DEV_ਅੰਕ(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_ਬਹੁਲਕ = access_ਮੰਜ਼ਿਲ_ਬਹੁਲਕ (devices[n]->access, err);
  ਜੇ (UNLIKELY (req_ਬਹੁਲਕ < 0))
    ਵਾਪਸ req_ਬਹੁਲਕ;

  ਲਈ (i = n; i > 0; i--)
    {
      size_t current_ਨਿਯਮ = i - 1;
      ਪੂਰਨ ਬਹੁਲਕ;

      ਜੇ (! CMP_DEV_ਅੰਕ (devices[n]->minor, devices[current_ਨਿਯਮ]->minor))
        ਜਾਰੀ;
      ਜੇ (! CMP_DEV_ਅੰਕ (devices[n]->major, devices[current_ਨਿਯਮ]->major))
        ਜਾਰੀ;

      ਜੇ (! has_same_type (devices[n]->type, devices[current_ਨਿਯਮ]->type)
          && has_same_type ("a", devices[current_ਨਿਯਮ]->type))
        ਜਾਰੀ;

      ਬਹੁਲਕ = access_ਮੰਜ਼ਿਲ_ਬਹੁਲਕ (devices[i]->access, err);
      ਜੇ (UNLIKELY (ਬਹੁਲਕ < 0))
        ਵਾਪਸ ਬਹੁਲਕ;

      ਜੇ (devices[current_ਨਿਯਮ]->allow && (ਬਹੁਲਕ & req_ਬਹੁਲਕ))
        ਵਾਪਸ 0;

      req_ਬਹੁਲਕ &= ! ਬਹੁਲਕ;

      /* no more access bits to validate.  */
      ਜੇ (req_ਬਹੁਲਕ == 0)
        ਵਾਪਸ 1;
    }

  ਵਾਪਸ 0;
#  undef CMP_DEV_ਅੰਕ
}

ਸਥਿਰ size_t
ਲੱਭੋ_ਪਹਿਲਾ_ਨਿਯਮ_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  ਜੇ (n == 0)
    ਵਾਪਸ 1;

  /* Find the first rule that is after the last "block all".  */
  ਲਈ (i = n - 1; i-- > 0;)
    {
      ਜੇ ((is_ਖਾਲੀ_ਥਾਵਾਂ_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        ਵਾਪਸ i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  ਲਈ (i = 0; i < n; i++)
    ਜੇ (devices[i]->allow)
      ਵਾਪਸ i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  ਵਾਪਸ n + 1;
}

ਸਥਿਰ bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_ਖੰਡ 1
#  define DEV_TYPE_CHAR 2
  ਪੂਰਨ ਬਚਿਆ = DEV_TYPE_ਖੰਡ | DEV_TYPE_CHAR;
  size_t i;

  ਲਈ (i = 0; i < n; i++)
    {
      size_t current_ਨਿਯਮ = n - i - 1;
      ਜੇ (! devices[current_ਨਿਯਮ]->allow)
        ਵਾਪਸ false;

      ਜੇ (IS_WILDCARD (devices[current_ਨਿਯਮ]->major) && IS_WILDCARD (devices[current_ਨਿਯਮ]->minor))
        {
          ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (devices[current_ਨਿਯਮ]->type) || strcmp (devices[current_ਨਿਯਮ]->type, "a") == 0)
            ਵਾਪਸ true;

          ਜੇ (strcmp (devices[current_ਨਿਯਮ]->type, "c") == 0)
            ਬਚਿਆ &= ~DEV_TYPE_CHAR;
          ਨਹੀਂਤਾਂ ਜੇ (strcmp (devices[current_ਨਿਯਮ]->type, "b") == 0)
            ਬਚਿਆ &= ~DEV_TYPE_ਖੰਡ;

          ਜੇ (ਬਚਿਆ == 0)
            ਵਾਪਸ true;
        }
    }
#  undef DEV_TYPE_ਖੰਡ
#  undef DEV_TYPE_CHAR

  ਵਾਪਸ false;
}

ਸਥਿਰ ਪੂਰਨ
append_devices (sd_bus_ਸੁਨੇਹਾ *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  ਬਣਤਰ default_dev_s *default_devices = get_default_devices ();
  ਪੂਰਨ ret, sd_err;
  size_t i;

  ਜੇ (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "DevicePolicy", "s", "auto");
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      ਵਾਪਸ 0;
    }

  sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "DevicePolicy", "s", "strict");
  ਜੇ (UNLIKELY (sd_err < 0))
    ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  ਲਈ (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਲਈ (i = ਲੱਭੋ_ਪਹਿਲਾ_ਨਿਯਮ_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      ਅੱਖਰ type;

      ਜੇ (! d->allow)
        {
          ਪੂਰਨ redundant;

          redundant = is_deny_ਨਿਯਮ_redundant (resources->devices, i, err);
          ਜੇ (UNLIKELY (redundant < 0))
            ਵਾਪਸ redundant;

          ਜੇ (redundant)
            ਜਾਰੀ;

          ਵਾਪਸ crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      ਜੇ (d->type == ਨਿਰਾ || strcmp (d->type, "a") == 0)
        type = 'a';
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (d->type, "c") == 0)
        type = 'c';
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (d->type, "b") == 0)
        type = 'b';
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ crun_make_error (err, 0, "unknown device type `%s`", d->type);

      ਜੇ (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
      ਨਹੀਂਤਾਂ
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ bool
has_bpf_fs ()
{
  ਬਣਤਰ statfs stat;

  ਵਾਪਸ statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

ਸਥਿਰ ਪੂਰਨ
bpfprog_ਰਸਤਾ_from_scope (ਅੱਖਰ **ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *scope, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *flat_scope = xstrdup (scope);
  ਅੱਖਰ *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  ਜਦੋਂ ((it = strchr (it, '.')) != ਨਿਰਾ)
    *it = '_';

  ਵਾਪਸ append_paths (ਰਸਤਾ, err, CRUN_BPF_DIR, flat_scope, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ
ਜੋੜੋ_bpf_ਪ੍ਰੋਗਰਾਮ (sd_bus_ਸੁਨੇਹਾ *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 ਅੱਖਰ **missing_properties,
                 ਅਚਲ ਅੱਖਰ *scope,
                 bool *devices_ਸੈੱਟ,
                 libcrun_error_t *err)
{
  ਪੂਰਨ ret = 0;
  ਪੂਰਨ ਵਿੱਚ_userns, sd_err;
  ਅਚਲ ਅੱਖਰ *property_name = "BPFProgram";
  cleanup_free ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;

  *devices_ਸੈੱਟ = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  ਜੇ (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    ਵਾਪਸ 0;

  ਵਿੱਚ_userns = check_running_ਵਿੱਚ_user_namespace (err);
  ਜੇ (UNLIKELY (ਵਿੱਚ_userns < 0))
    ਵਾਪਸ ਵਿੱਚ_userns;

  ਜੇ (ਵਿੱਚ_userns)
    ਵਾਪਸ 0;

  ਪ੍ਰੋਗਰਾਮ = create_dev_bpf (resources->devices, resources->devices_len, err);
  ਜੇ (UNLIKELY (ਪ੍ਰੋਗਰਾਮ == ਨਿਰਾ))
    {
      crun_error_release (err);
      ਵਾਪਸ 0;
    }

  ret = bpfprog_ਰਸਤਾ_from_scope (&ਰਸਤਾ, scope, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  ਜੇ (UNLIKELY (ret < 0 && errno != EEXIST))
    ਵਾਪਸ crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  ਜੇ (is_update)
    {
      cleanup_free ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ_loaded = ਨਿਰਾ;

      ਜੇ (resources->devices == ਨਿਰਾ || resources->devices_len == 0)
        {
          *devices_ਸੈੱਟ = true;
          ਵਾਪਸ 0;
        }

      ret = libcrun_ebpf_read_ਪ੍ਰੋਗਰਾਮ (&ਪ੍ਰੋਗਰਾਮ_loaded, ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      /* it is the same as the loaded program.  */
      ਜੇ (libcrun_ebpf_cmp_programs (ਪ੍ਰੋਗਰਾਮ, ਪ੍ਰੋਗਰਾਮ_loaded))
        {
          *devices_ਸੈੱਟ = true;
          ਵਾਪਸ 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ_new = ਨਿਰਾ;
        ਅੱਖਰ tmp[64];
        ਪੂਰਨ len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, ਆਕਾਰ (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (tmp)))
          ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_ਲੱਦੋ (ਪ੍ਰੋਗਰਾਮ, -1, tmp, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        ret = libcrun_ebpf_read_ਪ੍ਰੋਗਰਾਮ (&ਪ੍ਰੋਗਰਾਮ_new, tmp, err);
        unlink (tmp);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        ਜੇ (libcrun_ebpf_cmp_programs (ਪ੍ਰੋਗਰਾਮ_new, ਪ੍ਰੋਗਰਾਮ_loaded))
          {
            *devices_ਸੈੱਟ = true;
            ਵਾਪਸ 0;
          }
      }

      ਵਾਪਸ crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_ਲੱਦੋ (ਪ੍ਰੋਗਰਾਮ, -1, ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", property_name, "a(ss)", 1, "device", ਰਸਤਾ);
  ਜੇ (UNLIKELY (sd_err < 0))
    ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_ਸੈੱਟ = true;

  ਵਾਪਸ 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
ਸਥਿਰ ਪੂਰਨ
append_cpu_quota (sd_bus_ਸੁਨੇਹਾ *m, ਅੱਖਰ **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ਅਚਲ ਅੱਖਰ *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  ਪੂਰਨ sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ਜੇ (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      ਪੂਰਨ ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      ਜੇ (has_period)
        period = max_period;
    }

  ਜੇ (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  ਜੇ ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_ਪ੍ਰਤੀ_sec = UINT64_MAX;

      ਜੇ (quota > 0)
        {
          quota_ਪ੍ਰਤੀ_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          ਜੇ (quota_ਪ੍ਰਤੀ_sec % 10000)
            quota_ਪ੍ਰਤੀ_sec = ((quota_ਪ੍ਰਤੀ_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_ਪ੍ਰਤੀ_sec);
      ਜੇ (UNLIKELY (sd_err < 0))
        ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
append_resources (sd_bus_ਸੁਨੇਹਾ *m,
                  bool is_update,
                  ਅਚਲ ਅੱਖਰ *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  ਪੂਰਨ cgroup_ਬਹੁਲਕ,
                  ਅਚਲ ਅੱਖਰ *scope,
                  bool *devices_ਸੈੱਟ,
                  libcrun_error_t *err)
{
  uint64_t ਮੁੱਲ;
  ਪੂਰਨ sd_err;
  ਪੂਰਨ ret;
  cleanup_free ਅੱਖਰ *dir = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ **missing_properties = ਨਿਰਾ;

  *devices_ਸੈੱਟ = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  missing_properties = read_dir_entries (dir, err);
  ਜੇ (UNLIKELY (missing_properties == ਨਿਰਾ))
    {
      ਜੇ (crun_error_get_errno (err) != ENOENT)
        ਵਾਪਸ -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_ਮੁੱਲ(name, ਮੁੱਲ)                                             \
    ਕਰੋ                                                                                 \
      {                                                                                \
        ਜੇ (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", name, "t", ਮੁੱਲ);              \
            ਜੇ (UNLIKELY (sd_err < 0))                                                 \
              ਵਾਪਸ crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } ਜਦੋਂ (0)

#  define APPEND_UINT64(name, fn)              \
    ਕਰੋ                                         \
      {                                        \
        ret = fn (resources, &ਮੁੱਲ, err);     \
        ਜੇ (UNLIKELY (ret < 0))                \
          ਵਾਪਸ ret;                          \
        ਜੇ (ret)                               \
          {                                    \
            APPEND_UINT64_ਮੁੱਲ (name, ਮੁੱਲ); \
          }                                    \
    } ਜਦੋਂ (0)

  ਜੇ (resources == ਨਿਰਾ)
    ਵਾਪਸ 0;

  APPEND_UINT64 ("MemoryLow", get_ਸਮ੍ਰਿਤੀ_ਨੀਵਾਂ);
  APPEND_UINT64 ("MemoryMax", get_ਸਮ੍ਰਿਤੀ_max);
  APPEND_UINT64 ("MemorySwapMax", get_ਸਮ੍ਰਿਤੀ_ਅਦਲਾ_ਬਦਲੀ_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਚੋਣ (cgroup_ਬਹੁਲਕ)
    {
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_UNIFIED:
      {
        ret = append_io_ਭਾਰ (m, missing_properties, resources, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_ਭਾਰ);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;

        ਜੇ (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ਅਚਲ ਅੱਖਰ *property_name = "AllowedCPUs";
            ਅਚਲ ਅੱਖਰ *ਮੁੱਲ = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free ਅੱਖਰ *allowed_cpus = ਨਿਰਾ;
            size_t allowed_cpus_len = 0;

            ਜੇ (ਮੁੱਲ == ਨਿਰਾ)
              ਮੁੱਲ = resources->cpu->cpus;

            ret = cpuset_string_ਮੰਜ਼ਿਲ_bitmask (ਮੁੱਲ, &allowed_cpus, &allowed_cpus_len, err);
            ਜੇ (UNLIKELY (ret < 0))
              ਵਾਪਸ ret;

            ਜੇ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                ਜੇ (UNLIKELY (ret < 0))
                  ਵਾਪਸ ret;
              }
          }

        ਜੇ (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ਅਚਲ ਅੱਖਰ *property_name = "AllowedMemoryNodes";
            ਅਚਲ ਅੱਖਰ *ਮੁੱਲ = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free ਅੱਖਰ *allowed_mems = ਨਿਰਾ;
            size_t allowed_mems_len = 0;

            ਜੇ (ਮੁੱਲ == ਨਿਰਾ)
              ਮੁੱਲ = resources->cpu->mems;

            ret = cpuset_string_ਮੰਜ਼ਿਲ_bitmask (ਮੁੱਲ, &allowed_mems, &allowed_mems_len, err);
            ਜੇ (UNLIKELY (ret < 0))
              ਵਾਪਸ ret;

            ਜੇ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                ਜੇ (UNLIKELY (ret < 0))
                  ਵਾਪਸ ret;
              }
          }

        ret = ਜੋੜੋ_bpf_ਪ੍ਰੋਗਰਾਮ (m, is_update, resources, missing_properties, scope, devices_ਸੈੱਟ, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਵਾਪਸ ret;
      }
      ਰੋਕੋ;

    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_LEGACY:
    ਹਾਲਤ CGROUP_ਬਹੁਲਕ_HYBRID:
      ਜੇ (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_ਮੁੱਲ ("CPUShares", resources->cpu->shares);
      ਰੋਕੋ;

    ਮੂਲ:
      ਵਾਪਸ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_ਬਹੁਲਕ);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_ਮੁੱਲ

  ਜੇ (! *devices_ਸੈੱਟ && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
reset_failed_unit (sd_bus *bus, ਅਚਲ ਅੱਖਰ *unit)
{
  ਪੂਰਨ sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_ਸੁਨੇਹਾ *m = ਨਿਰਾ, *ਜਵਾਬ = ਨਿਰਾ;

  sd_err = sd_bus_ਸੁਨੇਹਾ_new_ਵਿਧੀ_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  ਜੇ (UNLIKELY (sd_err < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "s", unit);
  ਜੇ (UNLIKELY (sd_err < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  sd_err = sd_bus_call (bus, m, 0, &error, &ਜਵਾਬ);
  ਜੇ (UNLIKELY (sd_err < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  sd_err = 0;

ਬਾਹਰ_ਨਿਕਲੋ:
  ਜੇ (m)
    sd_bus_ਸੁਨੇਹਾ_unref (m);
  ਜੇ (ਜਵਾਬ)
    sd_bus_ਸੁਨੇਹਾ_unref (ਜਵਾਬ);
  sd_bus_error_free (&error);

  ਵਾਪਸ sd_err;
}

ਸਥਿਰ ਪੂਰਨ
verify_ebpf_device_filter_installed (ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਭਰਿਆ_ਰਸਤਾ = ਨਿਰਾ;
  size_t n_progs = 0;
  ਪੂਰਨ ret;

  ret = append_paths (&ਭਰਿਆ_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, cgroup_ਰਸਤਾ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_ebpf_query_cgroup_progs (ਭਰਿਆ_ਰਸਤਾ, &progs, &n_progs, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (n_progs == 0)
    ਵਾਪਸ crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", ਭਰਿਆ_ਰਸਤਾ);

  ਵਾਪਸ 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
ਸਥਿਰ ਪੂਰਨ
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_ਸੁਨੇਹਾ *m, sd_bus_ਸੁਨੇਹਾ *ਜਵਾਬ,
                           sd_bus_error *error, ਪੂਰਨ ret)
{
  ਜੇ (bus)
    sd_bus_unref (bus);
  ਜੇ (m)
    sd_bus_ਸੁਨੇਹਾ_unref (m);
  ਜੇ (ਜਵਾਬ)
    sd_bus_ਸੁਨੇਹਾ_unref (ਜਵਾਬ);
  sd_bus_error_free (error);
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            ਪੂਰਨ cgroup_ਬਹੁਲਕ,
                            string_map *annotations,
                            ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ,
                            ਅਚਲ ਅੱਖਰ *scope, ਅਚਲ ਅੱਖਰ *ਟੁਕੜਾ,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_ਸੈੱਟ,
                            libcrun_error_t *err)
{
  sd_bus *bus = ਨਿਰਾ;
  sd_bus_ਸੁਨੇਹਾ *m = ਨਿਰਾ;
  sd_bus_ਸੁਨੇਹਾ *ਜਵਾਬ = ਨਿਰਾ;
  ਪੂਰਨ sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ਅਚਲ ਅੱਖਰ *object = ਨਿਰਾ;
  ਬਣਤਰ systemd_ਕੰਮ_removed_s ਕੰਮ_ਅੰਕੜੇ = {};
  ਪੂਰਨ i;
  ਅਚਲ ਅੱਖਰ *boolean_opts[10];
  cleanup_free ਅੱਖਰ *state_dir = ਨਿਰਾ;

  *can_retry = false;
  *devices_ਸੈੱਟ = false;

  ret = libcrun_get_state_directory (&state_dir, state_ਜੜ੍ਹ, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = ਨਿਰਾ;

  ret = open_sd_bus_connection (&bus, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  ret = systemd_check_ਕੰਮ_ਸਥਿਤੀ_ਤਿਆਰੀ (bus, &ਕੰਮ_ਅੰਕੜੇ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  sd_err = sd_bus_ਸੁਨੇਹਾ_new_ਵਿਧੀ_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "ss", scope, "fail");
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  sd_err = sd_bus_ਸੁਨੇਹਾ_open_container (m, 'a', "(sv)");
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ਜੇ (ਟੁਕੜਾ)
    {
      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "Slice", "s", ਟੁਕੜਾ);
      ਜੇ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
        }
    }

  sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "DefaultDependencies", "b", 0);
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ਜੇ (annotations)
    {
      size_t ਅਗੇਤਰ_len = ਆਕਾਰ (SYSTEMD_PROPERTY_ਅਗੇਤਰ) - 1;
      size_t annotations_len = string_map_ਮਾਪ (annotations);
      size_t i;

      ਲਈ (i = 0; i < annotations_len; i++)
        {
          ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ;
          ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;
          size_t len;

          ret = string_map_get_at (annotations, i, &ਕੁੰਜੀ, &ਮੁੱਲ);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
            }

          ਜੇ (! has_ਅਗੇਤਰ (ਕੁੰਜੀ, SYSTEMD_PROPERTY_ਅਗੇਤਰ))
            ਜਾਰੀ;

          len = ਲੜੀ_ਲੰਬਾਈ (ਕੁੰਜੀ);
          ਜੇ (len < ਅਗੇਤਰ_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", ਕੁੰਜੀ);
              ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
            }

          ret = append_systemd_annotation (m, ਕੁੰਜੀ + ਅਗੇਤਰ_len, len - ਅਗੇਤਰ_len,
                                           ਮੁੱਲ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
        }
    }

  sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "Description", "s", "libcrun container");
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", "PIDs", "au", 1, pid);
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ਲਈ (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "(sv)", boolean_opts[i], "b", 1);
      ਜੇ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_ਬਹੁਲਕ, scope, devices_ਸੈੱਟ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  sd_err = sd_bus_ਸੁਨੇਹਾ_close_container (m);
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "a(sa(sv))", 0);
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &ਜਵਾਬ);
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ਜੇ (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          ਜੇ (ਜਵਾਬ)
            sd_bus_ਸੁਨੇਹਾ_unref (ਜਵਾਬ);

          error = SD_BUS_ERROR_NULL;
          ਜਵਾਬ = ਨਿਰਾ;

          sd_err = sd_bus_call (bus, m, 0, &error, &ਜਵਾਬ);
        }
      ਜੇ (sd_err < 0)
        {
          ਜੇ (sd_err == -EROFS)
            {
              ret = ਰਜਿਸਟਰ_missing_property_from_ਸੁਨੇਹਾ (state_dir, error.ਸੁਨੇਹਾ_2, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
              ਜੇ (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.ਸੁਨੇਹਾ_2 ?: error.name);
          ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
        }
    }

  sd_err = sd_bus_ਸੁਨੇਹਾ_read (ਜਵਾਬ, "o", &object);
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ret = systemd_check_ਕੰਮ_ਸਥਿਤੀ (bus, &ਕੰਮ_ਅੰਕੜੇ, object, "creating", err);

ਬਾਹਰ_ਨਿਕਲੋ:
  ਵਾਪਸ cleanup_sd_bus_and_return (bus, m, ਜਵਾਬ, &error, ret);
}

ਸਥਿਰ ਪੂਰਨ
libcrun_destroy_systemd_cgroup_scope (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ,
                                      libcrun_error_t *err)
{
  sd_bus *bus = ਨਿਰਾ;
  sd_bus_ਸੁਨੇਹਾ *m = ਨਿਰਾ;
  sd_bus_ਸੁਨੇਹਾ *ਜਵਾਬ = ਨਿਰਾ;
  ਪੂਰਨ ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ਅਚਲ ਅੱਖਰ *object;
  ਅਚਲ ਅੱਖਰ *scope = cgroup_ਸਥਿਤੀ->scope;
  ਬਣਤਰ systemd_ਕੰਮ_removed_s ਕੰਮ_ਅੰਕੜੇ = {};

  ret = open_sd_bus_connection (&bus, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  ret = systemd_check_ਕੰਮ_ਸਥਿਤੀ_ਤਿਆਰੀ (bus, &ਕੰਮ_ਅੰਕੜੇ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  ret = sd_bus_ਸੁਨੇਹਾ_new_ਵਿਧੀ_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  ਜੇ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ret = sd_bus_ਸੁਨੇਹਾ_append (m, "ss", scope, "replace");
  ਜੇ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ret = sd_bus_call (bus, m, 0, &error, &ਜਵਾਬ);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.ਸੁਨੇਹਾ_2 ?: error.name);
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ret = sd_bus_ਸੁਨੇਹਾ_read (ਜਵਾਬ, "o", &object);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ret = systemd_check_ਕੰਮ_ਸਥਿਤੀ (bus, &ਕੰਮ_ਅੰਕੜੇ, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

ਬਾਹਰ_ਨਿਕਲੋ:
  ਵਾਪਸ cleanup_sd_bus_and_return (bus, m, ਜਵਾਬ, &error, ret);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
ਲੱਭੋ_systemd_subgroup (string_map *annotations)
{
  ਅਚਲ ਅੱਖਰ *annotation;

  annotation = ਲੱਭੋ_string_map_ਮੁੱਲ (annotations, "run.oci.systemd.subgroup");
  ਜੇ (annotation)
    {
      ਜੇ (annotation[0] == '\0')
        ਵਾਪਸ ਨਿਰਾ;
      ਵਾਪਸ annotation;
    }

  ਵਾਪਸ "container";
}

ਸਥਿਰ ਪੂਰਨ
libcrun_cgroup_enter_systemd (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ,
                              ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਬਾਹਰਲਾ,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = ਦਲੀਲਾਂ->resources;
  ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ = ਦਲੀਲਾਂ->cgroup_ਰਸਤਾ;
  cleanup_free ਅੱਖਰ *scope_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *scope = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਟੁਕੜਾ = ਨਿਰਾ;
  ਪੂਰਨ retries_ਖੱਬਾ = 32;
  ਅਚਲ ਅੱਖਰ *suffix;
  ਅਚਲ ਅੱਖਰ *id = ਦਲੀਲਾਂ->id;
  pid_t pid = ਦਲੀਲਾਂ->pid;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ rootless = 0;
  ਪੂਰਨ ret;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
    {
      rootless = is_rootless (err);
      ਜੇ (UNLIKELY (rootless < 0))
        ਵਾਪਸ rootless;
    }

  get_systemd_scope_and_ਟੁਕੜਾ (id, rootless == 1, cgroup_ਰਸਤਾ, &scope, &ਟੁਕੜਾ);

  ਲਈ (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_ਬਹੁਲਕ, ਦਲੀਲਾਂ->annotations, ਦਲੀਲਾਂ->state_ਜੜ੍ਹ,
                                        scope, ਟੁਕੜਾ, pid, &can_retry, &ਬਾਹਰਲਾ->bpf_dev_ਸੈੱਟ, err);
      ਜੇ (LIKELY (ret >= 0))
        ਰੋਕੋ;

      ਜੇ (can_retry && retries_ਖੱਬਾ-- > 0)
        {
          crun_error_release (err);
          ਜਾਰੀ;
        }

      ਵਾਪਸ ret;
    }

  suffix = ਲੱਭੋ_systemd_subgroup (ਦਲੀਲਾਂ->annotations);

  ret = systemd_finalize (ਦਲੀਲਾਂ, &ਰਸਤਾ, cgroup_ਬਹੁਲਕ, suffix, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (suffix))
    scope_ਰਸਤਾ = xstrdup (ਰਸਤਾ);
  ਨਹੀਂਤਾਂ
    {
      size_t ਰਸਤਾ_len = ਲੜੀ_ਲੰਬਾਈ (ਰਸਤਾ);
      size_t suffix_len = ਲੜੀ_ਲੰਬਾਈ (suffix);

      scope_ਰਸਤਾ = strndup (ਰਸਤਾ, ਰਸਤਾ_len - suffix_len - 1);
      ਜੇ (UNLIKELY (scope_ਰਸਤਾ == ਨਿਰਾ))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  ਜੇ (ਬਾਹਰਲਾ->bpf_dev_ਸੈੱਟ)
    {
      ret = verify_ebpf_device_filter_installed (scope_ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਬਾਹਰਲਾ->ਰਸਤਾ = ਰਸਤਾ;
  ਰਸਤਾ = ਨਿਰਾ;

  ਬਾਹਰਲਾ->scope = scope;
  scope = ਨਿਰਾ;
  ਵਾਪਸ 0;
}

ਅੱਖਰ *
get_cgroup_scope_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *scope)
{
  ਅੱਖਰ *ਰਸਤਾ_ਮੰਜ਼ਿਲ_scope = ਨਿਰਾ;
  ਅੱਖਰ *cur;

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (cgroup_ਰਸਤਾ))
    ਵਾਪਸ ਨਿਰਾ;

  ਰਸਤਾ_ਮੰਜ਼ਿਲ_scope = xstrdup (cgroup_ਰਸਤਾ);

  cur = strchr (ਰਸਤਾ_ਮੰਜ਼ਿਲ_scope, '/');
  ਜਦੋਂ (cur)
    {
      ਅੱਖਰ *ਅਗਲਾ = strchr (cur + 1, '/');
      ਜੇ (ਅਗਲਾ == ਨਿਰਾ)
        ਰੋਕੋ;

      *ਅਗਲਾ = '\0';
      ਜੇ (strcmp (cur, scope) == 0)
        ਵਾਪਸ ਰਸਤਾ_ਮੰਜ਼ਿਲ_scope;
      *ਅਗਲਾ = '/';

      cur = ਅਗਲਾ;
      ਜਦੋਂ (*cur == '/')
        cur++;
    }

  ਵਾਪਸ ਰਸਤਾ_ਮੰਜ਼ਿਲ_scope;
}

ਸਥਿਰ ਪੂਰਨ
libcrun_destroy_cgroup_systemd (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ,
                                libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਰਸਤਾ_ਮੰਜ਼ਿਲ_scope = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *bpfprog = ਨਿਰਾ;
  ਪੂਰਨ ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (ਬਹੁਲਕ < 0))
    ਵਾਪਸ ਬਹੁਲਕ;

  ret = cgroup_killall_ਰਸਤਾ (cgroup_ਸਥਿਤੀ->ਰਸਤਾ, SIGKILL, err);
  ਜੇ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_ਰਸਤਾ_from_scope (&bpfprog, cgroup_ਸਥਿਤੀ->scope, err);
  ਜੇ (UNLIKELY (ret < 0))
    crun_error_release (err);
  ਨਹੀਂਤਾਂ
    unlink (bpfprog); // Best effort.

  ਰਸਤਾ_ਮੰਜ਼ਿਲ_scope = get_cgroup_scope_ਰਸਤਾ (cgroup_ਸਥਿਤੀ->ਰਸਤਾ, cgroup_ਸਥਿਤੀ->scope);

  ਵਾਪਸ destroy_cgroup_ਰਸਤਾ (ਰਸਤਾ_ਮੰਜ਼ਿਲ_scope, ਬਹੁਲਕ, err);
}

ਸਥਿਰ ਪੂਰਨ
libcrun_update_resources_systemd (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ,
                                  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  ਬਣਤਰ systemd_ਕੰਮ_removed_s ਕੰਮ_ਅੰਕੜੇ = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free ਅੱਖਰ *state_dir = ਨਿਰਾ;
  sd_bus_ਸੁਨੇਹਾ *ਜਵਾਬ = ਨਿਰਾ;
  sd_bus_ਸੁਨੇਹਾ *m = ਨਿਰਾ;
  sd_bus *bus = ਨਿਰਾ;
  ਪੂਰਨ sd_err, ret;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;

  ret = libcrun_get_state_directory (&state_dir, state_ਜੜ੍ਹ, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ret = open_sd_bus_connection (&bus, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = systemd_check_ਕੰਮ_ਸਥਿਤੀ_ਤਿਆਰੀ (bus, &ਕੰਮ_ਅੰਕੜੇ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  sd_err = sd_bus_ਸੁਨੇਹਾ_new_ਵਿਧੀ_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  sd_err = sd_bus_ਸੁਨੇਹਾ_append (m, "sb", cgroup_ਸਥਿਤੀ->scope, 1);
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  sd_err = sd_bus_ਸੁਨੇਹਾ_open_container (m, 'a', "(sv)");
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_ਬਹੁਲਕ, cgroup_ਸਥਿਤੀ->scope, &cgroup_ਸਥਿਤੀ->bpf_dev_ਸੈੱਟ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  sd_err = sd_bus_ਸੁਨੇਹਾ_close_container (m);
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &ਜਵਾਬ);
  ਜੇ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.ਸੁਨੇਹਾ_2 ?: error.name);
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ਜੇ (cgroup_ਬਹੁਲਕ != CGROUP_ਬਹੁਲਕ_UNIFIED)
    {
      ret = ਤਿਆਰੀ_rt_runtime (resources, cgroup_ਸਥਿਤੀ->ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

      ret = ਤਿਆਰੀ_cpuset_for_systemd_v1 (resources, cgroup_ਸਥਿਤੀ->ਰਸਤਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  ret = ਤਿਆਰੀ_missing_cpu_options_for_systemd (resources, cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED, cgroup_ਸਥਿਤੀ->ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  ret = 0;

ਬਾਹਰ_ਨਿਕਲੋ:
  ਵਾਪਸ cleanup_sd_bus_and_return (bus, m, ਜਵਾਬ, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
ਸਥਿਰ ਪੂਰਨ
libcrun_attach_process_systemd (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ,
                                ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, pid_t pid,
                                libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *scope = cgroup_ਸਥਿਤੀ->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free ਅੱਖਰ *scope_ਰਸਤਾ = ਨਿਰਾ;
  sd_bus_ਸੁਨੇਹਾ *ਜਵਾਬ = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *subcgroup;
  ਅਚਲ ਅੱਖਰ *ਆਖਰੀ;
  sd_bus *bus = ਨਿਰਾ;
  size_t len;
  ਪੂਰਨ sd_err, ret;

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (scope))
    ਵਾਪਸ crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_ਰਸਤਾ = get_cgroup_scope_ਰਸਤਾ (ਰਸਤਾ, scope);
  ਜੇ (scope_ਰਸਤਾ == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  ਆਖਰੀ = strrchr (scope_ਰਸਤਾ, '/');
  ਜੇ (ਆਖਰੀ == ਨਿਰਾ || strcmp (ਆਖਰੀ + 1, scope) != 0)
    ਵਾਪਸ crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, ਰਸਤਾ);
  len = ਲੜੀ_ਲੰਬਾਈ (scope_ਰਸਤਾ);
  subcgroup = ਰਸਤਾ[len] ? ਰਸਤਾ + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  sd_err = sd_bus_call_ਵਿਧੀ (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &ਜਵਾਬ, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  ਜੇ (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.ਸੁਨੇਹਾ_2 ?: error.name);

  ਵਾਪਸ cleanup_sd_bus_and_return (bus, ਨਿਰਾ, ਜਵਾਬ, &error, ret);
}

#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਪੂਰਨ
libcrun_cgroup_enter_systemd (ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ,
                              ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *ਬਾਹਰਲਾ,
                              libcrun_error_t *err)
{
  (ਖਾਲੀ) ਦਲੀਲਾਂ;
  (ਖਾਲੀ) ਬਾਹਰਲਾ;

  ਵਾਪਸ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ਸਥਿਰ ਪੂਰਨ
libcrun_destroy_cgroup_systemd (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ,
                                libcrun_error_t *err)
{
  (ਖਾਲੀ) cgroup_ਸਥਿਤੀ;

  ਵਾਪਸ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ਸਥਿਰ ਪੂਰਨ
libcrun_update_resources_systemd (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ,
                                  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (ਖਾਲੀ) cgroup_ਸਥਿਤੀ;
  (ਖਾਲੀ) state_ਜੜ੍ਹ;
  (ਖਾਲੀ) resources;

  ਵਾਪਸ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ਸਥਿਰ ਪੂਰਨ
libcrun_attach_process_systemd (ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ,
                                ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, pid_t pid,
                                libcrun_error_t *err)
{
  (ਖਾਲੀ) cgroup_ਸਥਿਤੀ;
  (ਖਾਲੀ) ਰਸਤਾ;
  (ਖਾਲੀ) pid;

  ਵਾਪਸ crun_make_error (err, ENOTSUP, "systemd not supported");
}
#ਜੇ_ਅੰਤ

ਬਣਤਰ libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = ਨਿਰਾ,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
