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
#అన్తర్భావ "cgroup-resources.h"
#అన్తర్భావ "cgroup-utils.h"
#అన్తర్భావ "ebpf.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "status.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <sys/vfs.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/vfs.h>
#అన్తర్భావ <linux/magic.h>

#నిర్వచితమైతే HAVE_SYSTEMD
#  include <systemd/sd-bus.h>
#  include <limits.h>

#  define SYSTEMD_PROPERTY_ఉపసర్గ "org.systemd.property."

#  define CGROUP_BFQ_బరువు_MIN ((uint64_t) 1)
#  define CGROUP_BFQ_బరువు_DEFAULT ((uint64_t) 100)
#  define CGROUP_BFQ_బరువు_MAX ((uint64_t) 1000)

#  define CGROUP_బరువు_MIN ((uint64_t) 1)
#  define CGROUP_బరువు_DEFAULT ((uint64_t) 100)
#  define CGROUP_బరువు_MAX ((uint64_t) 10000)

/* The CPU quota period used by the kernel (and systemd) by default, in us.  */
#  define DEFAULT_CPU_QUOTA_PERIOD ((uint64_t) 100000)

#  define SYSTEMD_MISSING_PROPERTIES_DIR ".cache/systemd-missing-properties"

#  define IS_WILDCARD(x) (x <= 0)

స్థిర పూర్ణాంకం
రిజిస్టరు_missing_property_from_సందేశం (స్థిరరాశి అక్షరం *state_dir, స్థిరరాశి అక్షరం *సందేశం_2, libcrun_error_t *err)
{
  cleanup_free అక్షరం *file_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *dir_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *property = ఖాళీ;
  అక్షరం *p;
  పూర్ణాంకం ret;

  ఒకవేళ (! has_ఉపసర్గ (సందేశం_2, "Cannot set property "))
    తిరిగి 0;

  property = xstrdup (సందేశం_2 + పరిమాణం ("Cannot set property ") - 1);
  p = strchr (property, ',');
  ఒకవేళ (! p)
    తిరిగి 0;
  *p = '\0';

  ret = append_paths (&dir_మార్గం, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = crun_ensure_directory (dir_మార్గం, 0755, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_paths (&file_మార్గం, err, dir_మార్గం, property, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_మార్గం, ఖాళీ, 0, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 1;
}

స్థిర bool
property_missing_p (అక్షరం **missing_properties, స్థిరరాశి అక్షరం *property)
{
  size_t i;

  కొరకు (i = 0; missing_properties && missing_properties[i]; i++)
    ఒకవేళ (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        తిరిగి true;
      }

  తిరిగి false;
}

స్థిర శూన్య
get_systemd_scope_and_ముక్క (స్థిరరాశి అక్షరం *id, bool user_ముక్క, స్థిరరాశి అక్షరం *cgroup_మార్గం,
                             అక్షరం **scope, అక్షరం **ముక్క)
{
  స్థిరరాశి అక్షరం *n;
  అక్షరం *p;

  ఒకవేళ (cgroup_మార్గం == ఖాళీ || cgroup_మార్గం[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      తిరిగి;
    }

  n = strchr (cgroup_మార్గం, ':');
  ఒకవేళ (n == ఖాళీ)
    xasprintf (scope, "%s.scope", cgroup_మార్గం);
  లేకపోతే
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      ఒకవేళ (p)
        *p = '-';
    }
  ఒకవేళ (ముక్క)
    {
      *ముక్క = xstrdup (cgroup_మార్గం);
      p = strchr (*ముక్క, ':');
      ఒకవేళ (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      ఒకవేళ (is_ఖాళీలు_string (*ముక్క))
        {
          విడుదల (*ముక్క);
          ఒకవేళ (user_ముక్క)
            *ముక్క = xstrdup ("user.slice");
          లేకపోతే
            *ముక్క = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
స్థిర పూర్ణాంకం
అమరిక_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
  cleanup_close పూర్ణాంకం dirfd = -1;
  bool need_సమితి_మాతృ = true;
  అక్షరం fmt_buf[64];
  పూర్ణాంకం len;
  పూర్ణాంకం ret;

  ఒకవేళ (resources == ఖాళీ || resources->cpu == ఖాళీ)
    తిరిగి 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  ఒకవేళ (is_ఖాళీలు_string (మార్గం))
    తిరిగి crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  ఒకవేళ (has_suffix (మార్గం, ".scope"))
    need_సమితి_మాతృ = false;

  ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, "cpu", మార్గం, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = crun_ensure_directory (cgroup_మార్గం, 0755, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  dirfd = open (cgroup_మార్గం, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (dirfd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", cgroup_మార్గం);

  ఒకవేళ (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      ఒకవేళ (need_సమితి_మాతృ)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, పరిమాణం (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (fmt_buf)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      ఒకవేళ (need_సమితి_మాతృ)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
అమరిక_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
  పూర్ణాంకం మాతృ_2;
  పూర్ణాంకం ret;

  ఒకవేళ (resources == ఖాళీ || resources->cpu == ఖాళీ)
    తిరిగి 0;

  ఒకవేళ (! resources->cpu->వ్యవధి_present && ! (cgroup2 && resources->cpu->idle_present))
    తిరిగి 0;

  కొరకు (మాతృ_2 = 0; మాతృ_2 < 2; మాతృ_2++)
    {
      cleanup_close పూర్ణాంకం dirfd = -1;

      ఒకవేళ (cgroup2)
        ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, మార్గం ? మార్గం : "", (మాతృ_2 ? ".." : ఖాళీ), ఖాళీ);
      లేకపోతే
        ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, "/cpu", మార్గం ? మార్గం : "", (మాతృ_2 ? ".." : ఖాళీ), ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      dirfd = open (cgroup_మార్గం, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (dirfd < 0))
        తిరిగి crun_make_error (err, errno, "open `%s`", cgroup_మార్గం);

      ret = write_cpu_వ్యవధి (dirfd, cgroup2, resources->cpu, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      ఒకవేళ (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
అమరిక_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;
  పూర్ణాంకం మాతృ_2;
  పూర్ణాంకం ret;
  పూర్ణాంకం స్థాయిలు;

  ret = append_paths (&cgroup_మార్గం, err, CGROUP_మూలం, "/cpuset", మార్గం ? మార్గం : "", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = crun_ensure_directory (cgroup_మార్గం, 0755, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_మార్గం, resources, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  స్థాయిలు = has_suffix (మార్గం, ".scope") ? 1 : 2;

  కొరకు (మాతృ_2 = 0; మాతృ_2 < స్థాయిలు; మాతృ_2++)
    {
      cleanup_close పూర్ణాంకం dirfd_cpuset = -1;
      cleanup_free అక్షరం *మార్గం_గమ్యం_cpuset = ఖాళీ;

      ret = append_paths (&మార్గం_గమ్యం_cpuset, err, CGROUP_మూలం "/cpuset", మార్గం, (మాతృ_2 ? ".." : ఖాళీ), ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      dirfd_cpuset = open (మార్గం_గమ్యం_cpuset, O_DIRECTORY | O_మార్గం | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (dirfd_cpuset < 0))
        తిరిగి crun_make_error (err, errno, "open `%s`", మార్గం_గమ్యం_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
స్థిర పూర్ణాంకం
compute_finalized_మార్గం (అక్షరం **మార్గం, స్థిరరాశి అక్షరం *from, స్థిరరాశి అక్షరం *suffix, libcrun_error_t *err)
{
  ఒకవేళ (suffix == ఖాళీ)
    {
      *మార్గం = xstrdup (from);
      తిరిగి 0;
    }
  తిరిగి append_paths (మార్గం, err, from, suffix, ఖాళీ);
}

స్థిర పూర్ణాంకం
systemd_finalize (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు, అక్షరం **మార్గం_బహిర్గతం,
                  పూర్ణాంకం cgroup_బహుళకం, స్థిరరాశి అక్షరం *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = వాదనలు->resources;
  cleanup_free అక్షరం *content = ఖాళీ;
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  pid_t pid = వాదనలు->pid;
  పూర్ణాంకం ret;
  అక్షరం *from, *గమ్యం;
  అక్షరం *saveptr = ఖాళీ;

  cleanup_free అక్షరం *proc_మార్గం = ఖాళీ;
  cleanup_close పూర్ణాంకం fd = -1;

  xasprintf (&proc_మార్గం, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (వాదనలు->container, proc_మార్గం, O_RDONLY, err);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి fd;

  ret = read_all_fd (fd, "cgroup path", &content, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఎంపిక (cgroup_బహుళకం)
    {
    స్థితి CGROUP_బహుళకం_HYBRID:
    స్థితి CGROUP_బహుళకం_LEGACY:
      from = అక్షరశ్రేణి_శోధన (content, ":memory:");
      ఒకవేళ (LIKELY (from != ఖాళీ))
        from += 8;
      లేకపోతే
        {
          from = అక్షరశ్రేణి_శోధన (content, ":pids:");
          ఒకవేళ (UNLIKELY (from == ఖాళీ))
            తిరిగి crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      గమ్యం = strchr (from, '\n');
      ఒకవేళ (UNLIKELY (గమ్యం == ఖాళీ))
        తిరిగి crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *గమ్యం = '\0';
      ret = compute_finalized_మార్గం (&మార్గం, from, suffix, err);
      *గమ్యం = '\n';
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (geteuid ())
        తిరిగి 0;

      ret = అమరిక_rt_runtime (resources, మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = అమరిక_cpuset_for_systemd_v1 (resources, మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      కొరకు (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ఖాళీ, "\n", &saveptr))
        {
          అక్షరం *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          ఒకవేళ (subsystem[0] == '\0')
            {
              ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_LEGACY)
                కొనసాగు;

              subsystem = "unified";
            }

          ఒకవేళ (strcmp (subpath, మార్గం))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, మార్గం, true, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  ఒకవేళ (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      కొనసాగు;
                    }
                  తిరిగి ret;
                }
            }
        }

      ఆపు;

    స్థితి CGROUP_బహుళకం_UNIFIED:
      {
        cleanup_free అక్షరం *dir = ఖాళీ;

        from = అక్షరశ్రేణి_శోధన (content, "0::");
        ఒకవేళ (UNLIKELY (from == ఖాళీ))
          తిరిగి crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        గమ్యం = strchr (from, '\n');
        ఒకవేళ (UNLIKELY (గమ్యం == ఖాళీ))
          తిరిగి crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *గమ్యం = '\0';
        ret = compute_finalized_మార్గం (&మార్గం, from, suffix, err);
        *గమ్యం = '\n';
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        ret = append_paths (&dir, err, CGROUP_మూలం, మార్గం, ఖాళీ);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        ret = move_process_గమ్యం_cgroup (pid, ఖాళీ, మార్గం, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        ret = enable_controllers (మార్గం, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        ఒకవేళ (suffix)
          {
            ret = chown_cgroups (మార్గం, వాదనలు->మూలం_uid, వాదనలు->మూలం_gid, err);
            ఒకవేళ (UNLIKELY (ret < 0))
              తిరిగి ret;
          }
      }
      ఆపు;

    అప్రమేయ:
      తిరిగి crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_బహుళకం);
    }

  ret = అమరిక_missing_cpu_options_for_systemd (resources, cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED, మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  *మార్గం_బహిర్గతం = మార్గం;
  మార్గం = ఖాళీ;

  తిరిగి 0;
}

నిర్మాణం systemd_పని_removed_s
{
  స్థిరరాశి అక్షరం *మార్గం;
  స్థిరరాశి అక్షరం *op;
  పూర్ణాంకం terminated;
  libcrun_error_t err;
};

స్థిర పూర్ణాంకం
systemd_పని_removed (sd_bus_సందేశం *m, శూన్య *userdata, sd_bus_error *error arg_వాడని)
{
  స్థిరరాశి అక్షరం *మార్గం, *unit, *ఫలితం;
  uint32_t id;
  పూర్ణాంకం ret;
  నిర్మాణం systemd_పని_removed_s *d = userdata;

  ret = sd_bus_సందేశం_read (m, "uoss", &id, &మార్గం, &unit, &ఫలితం);
  ఒకవేళ (ret < 0)
    తిరిగి -1;

  ఒకవేళ (strcmp (d->మార్గం, మార్గం) == 0)
    {
      d->terminated = 1;
      ఒకవేళ (strcmp (ఫలితం, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, ఫలితం);
          తిరిగి -1;
        }
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
systemd_check_పని_స్థితి_అమరిక (sd_bus *bus, నిర్మాణం systemd_పని_removed_s *దత్తాంశం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = sd_bus_సరిపోలిక_signal_async (bus, ఖాళీ, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_పని_removed, ఖాళీ, దత్తాంశం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, -ret, "sd-bus match signal");

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
systemd_check_పని_స్థితి (sd_bus *bus, నిర్మాణం systemd_పని_removed_s *దత్తాంశం, స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *op,
                          libcrun_error_t *err)
{
  పూర్ణాంకం sd_err;

  దత్తాంశం->మార్గం = మార్గం;
  దత్తాంశం->op = op;
  అయితే (! దత్తాంశం->terminated)
    {
      sd_err = sd_bus_process (bus, ఖాళీ);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus process");

      ఒకవేళ (sd_err != 0)
        కొనసాగు;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus wait");
    }

  ఒకవేళ (దత్తాంశం->err != ఖాళీ)
    {
      *err = దత్తాంశం->err;
      తిరిగి -1;
    }

  తిరిగి 0;
}

పూర్ణాంకం
parse_sd_array (అక్షరం *s, అక్షరం **బహిర్గతం, అక్షరం **తదుపరి, libcrun_error_t *err)
{
  అక్షరం endchr;
  అక్షరం *it, *dest;
  bool escaped = false;

  *బహిర్గతం = ఖాళీ;
  *తదుపరి = ఖాళీ;

  అయితే (ఖాళీనా (*s))
    s++;
  ఒకవేళ (*s == '\0')
    తిరిగి 0;
  లేకపోతే ఒకవేళ (*s != '\'' && *s != '"')
    తిరిగి crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *బహిర్గతం = dest = it;

  అయితే (1)
    {
      ఒకవేళ (*it == '\0')
        తిరిగి crun_make_error (err, 0, "invalid string `%s`", s);
      ఒకవేళ (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          అయితే (ఖాళీనా (*it))
            it++;
          ఒకవేళ (*it == ',')
            {
              *తదుపరి = ++it;
              *dest = '\0';
              తిరిగి 0;
            }

          ఒకవేళ (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              తిరిగి 0;
            }

          తిరిగి crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      ఒకవేళ (! escaped)
        *dest++ = *it;
      it++;
    }

  తిరిగి 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
స్థిర పూర్ణాంకం
append_systemd_annotation (sd_bus_సందేశం *m, స్థిరరాశి అక్షరం *name, size_t name_len, స్థిరరాశి అక్షరం *విలువ,
                           libcrun_error_t *err)
{
  cleanup_free అక్షరం *tmp_name = ఖాళీ;
  uint32_t గుణకం = 1;
  స్థిరరాశి అక్షరం *it;
  పూర్ణాంకం sd_err;

  అయితే (*విలువ == ' ')
    విలువ++;

  it = విలువ;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  ఒకవేళ (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
      && name[name_len - 1] == 'c')
    {
      గుణకం = 1000000;

      tmp_name = xmalloc (name_len + 2);
      జ్ఞాపకం_నకలు (tmp_name, name, name_len - 3);
      జ్ఞాపకం_నకలు (tmp_name + name_len - 3, "USec", 5);

      name = tmp_name;
    }

  /* A *USec property is always an uint64, so, like runc does, accept any
     number of seconds (possibly with a fractional part, and with an
     optional integer type), and convert it to uint64 microseconds.  */
  ఒకవేళ (గుణకం != 1)
    {
      స్థిర స్థిరరాశి అక్షరం *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", ఖాళీ };
      స్థిరరాశి అక్షరం *అంకె = it;
      అక్షరం *endptr = ఖాళీ;
      ద్విగుణ v;
      size_t i;

      కొరకు (i = 0; types[i]; i++)
        ఒకవేళ (has_ఉపసర్గ (అంకె, types[i]))
          {
            అంకె += అక్షరశ్రేణి_పొడవు (types[i]);
            ఆపు;
          }

      errno = 0;
      v = strtod (అంకె, &endptr);
      ఒకవేళ (UNLIKELY (errno != 0 || endptr == అంకె || *endptr || v < 0 || v * గుణకం >= 18446744073709551615.0))
        తిరిగి crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, విలువ);

      sd_err = sd_bus_సందేశం_append (m, "(sv)", name, "t", (uint64_t) (v * గుణకం));
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      తిరిగి 0;
    }

  ఒకవేళ ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_సందేశం_append (m, "(sv)", name, "b", b);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      తిరిగి 0;
    }
  లేకపోతే ఒకవేళ (*it == '\'')
    {
      cleanup_free అక్షరం *v_ప్రారంభం = ఖాళీ;
      అక్షరం *చివర;

      it = v_ప్రారంభం = xstrdup (విలువ);

      చివర = strchr (v_ప్రారంభం + 1, '\'');
      ఒకవేళ (చివర == ఖాళీ)
        తిరిగి crun_make_error (err, 0, "invalid variant `%s`", విలువ);
      *చివర = '\0';

      sd_err = sd_bus_సందేశం_append (m, "(sv)", name, "s", it + 1);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      తిరిగి 0;
    }
  లేకపోతే ఒకవేళ (*it == '[')
    {
      cleanup_free అక్షరం *v_ప్రారంభం = ఖాళీ;
      size_t n_parts = 0, parts_కొలత = 32;
      cleanup_free అక్షరం **parts = xmalloc (పరిమాణం (అక్షరం *) * parts_కొలత);
      అక్షరం *part;

      part = v_ప్రారంభం = xstrdup (it + 1);
      అయితే (1)
        {
          అక్షరం *బహిర్గతం = ఖాళీ, *తదుపరి = ఖాళీ;
          పూర్ణాంకం ret;

          ret = parse_sd_array (part, &బహిర్గతం, &తదుపరి, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          parts[n_parts++] = బహిర్గతం;
          ఒకవేళ (n_parts == parts_కొలత - 1)
            {
              parts_కొలత += 32;
              parts = xrealloc (parts, పరిమాణం (అక్షరం *) * parts_కొలత);
            }
          parts[n_parts] = ఖాళీ;
          ఒకవేళ (తదుపరి == ఖాళీ)
            ఆపు;

          part = తదుపరి;
        }

      sd_err = sd_bus_సందేశం_open_container (m, 'r', "sv");
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_సందేశం_append (m, "s", name);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_సందేశం_open_container (m, 'v', "as");
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_సందేశం_append_strv (m, parts);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_సందేశం_close_container (m);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_సందేశం_close_container (m);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus close container");

      తిరిగి 0;
    }
  లేకపోతే ఒకవేళ (has_ఉపసర్గ (it, "uint64 "))
    {
      అక్షరం *endptr = ఖాళీ;
      uint64_t v;

      errno = 0;
      v = strtoull (it + పరిమాణం ("uint64"), &endptr, 10);
      ఒకవేళ (UNLIKELY (errno != 0 || *endptr))
        తిరిగి crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_సందేశం_append (m, "(sv)", name, "t", (uint64_t) (v * గుణకం));
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      తిరిగి 0;
    }
  లేకపోతే ఒకవేళ (has_ఉపసర్గ (it, "int64 "))
    {
      అక్షరం *endptr = ఖాళీ;
      int64_t v;

      errno = 0;
      v = strtoll (it + పరిమాణం ("int64"), &endptr, 10);
      ఒకవేళ (UNLIKELY (errno != 0 || *endptr))
        తిరిగి crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_సందేశం_append (m, "(sv)", name, "x", (int64_t) (v * గుణకం));
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      తిరిగి 0;
    }
  లేకపోతే ఒకవేళ (has_ఉపసర్గ (it, "uint32 "))
    {
      అక్షరం *endptr = ఖాళీ;
      uint32_t v;

      errno = 0;
      v = strtoul (it + పరిమాణం ("uint32"), &endptr, 10);
      ఒకవేళ (UNLIKELY (errno != 0 || *endptr))
        తిరిగి crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_సందేశం_append (m, "(sv)", name, "u", (uint32_t) (v * గుణకం));
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      తిరిగి 0;
    }
  లేకపోతే ఒకవేళ (has_ఉపసర్గ (it, "int32 ") || strchr (it, ' ') == ఖాళీ)
    {
      అక్షరం *endptr = ఖాళీ;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      ఒకవేళ (has_ఉపసర్గ (it, "int32 "))
        v = strtol (it + పరిమాణం ("int32"), &endptr, 10);
      లేకపోతే
        v = strtol (it, &endptr, 10);
      ఒకవేళ (UNLIKELY (errno != 0 || *endptr))
        తిరిగి crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_సందేశం_append (m, "(sv)", name, "i", (int32_t) (v * గుణకం));
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      తిరిగి 0;
    }

  తిరిగి crun_make_error (err, 0, "unknown type for `%s`", name);
}

స్థిర పూర్ణాంకం
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  పూర్ణాంకం rootless;
  పూర్ణాంకం sd_err = 0;

  rootless = is_rootless (err);
  ఒకవేళ (UNLIKELY (rootless < 0))
    తిరిగి rootless;

  ఒకవేళ (rootless)
    sd_err = sd_bus_default_user (bus);
  ఒకవేళ (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  ఒకవేళ (sd_err < 0)
    తిరిగి crun_make_error (err, -sd_err, "cannot open sd-bus");

  తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, స్థిరరాశి అక్షరం *name)
{
  size_t i;

  ఒకవేళ (resources == ఖాళీ || resources->unified == ఖాళీ)
    తిరిగి ఖాళీ;

  కొరకు (i = 0; i < resources->unified->len; i++)
    ఒకవేళ (strcmp (resources->unified->కీలకాలు[i], name) == 0)
      తిరిగి resources->unified->విలువలు[i];
  తిరిగి ఖాళీ;
}

స్థిర పూర్ణాంకం
get_విలువ_from_unified_map (runtime_spec_schema_config_linux_resources *resources, స్థిరరాశి అక్షరం *name,
                            uint64_t *విలువ, libcrun_error_t *err)
{
  అక్షరం *endptr = ఖాళీ;
  size_t i;

  ఒకవేళ (resources == ఖాళీ || resources->unified == ఖాళీ)
    తిరిగి 0;

  కొరకు (i = 0; i < resources->unified->len; i++)
    ఒకవేళ (strcmp (resources->unified->కీలకాలు[i], name) == 0)
      {
        ఒకవేళ (strcmp (resources->unified->విలువలు[i], "max") == 0)
          {
            *విలువ = UINT64_MAX;
            తిరిగి 1;
          }

        errno = 0;
        *విలువ = strtoull (resources->unified->విలువలు[i], &endptr, 10);
        ఒకవేళ (UNLIKELY (errno))
          తిరిగి crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->విలువలు[i]);
        ఒకవేళ (endptr && *endptr)
          తిరిగి crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->విలువలు[i]);
        తిరిగి 1;
      }
  తిరిగి 0;
}

స్థిర అంతఃవరుస పూర్ణాంకం
get_జ్ఞాపకం_దిగువ (runtime_spec_schema_config_linux_resources *resources, uint64_t *హద్దు, libcrun_error_t *err)
{
  ఒకవేళ (resources->జ్ఞాపకం && resources->జ్ఞాపకం->reservation_present)
    {
      *హద్దు = resources->జ్ఞాపకం->reservation;
      తిరిగి 1;
    }

  తిరిగి get_విలువ_from_unified_map (resources, "memory.low", హద్దు, err);
}

స్థిర అంతఃవరుస పూర్ణాంకం
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *హద్దు, libcrun_error_t *err)
{
  ఒకవేళ (resources->pids && resources->pids->హద్దు)
    {
      *హద్దు = resources->pids->హద్దు;
      తిరిగి 1;
    }

  తిరిగి get_విలువ_from_unified_map (resources, "pids.max", హద్దు, err);
}

స్థిర అంతఃవరుస పూర్ణాంకం
get_జ్ఞాపకం_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *హద్దు, libcrun_error_t *err)
{
  ఒకవేళ (resources->జ్ఞాపకం && resources->జ్ఞాపకం->హద్దు_present)
    {
      *హద్దు = resources->జ్ఞాపకం->హద్దు;
      తిరిగి 1;
    }

  తిరిగి get_విలువ_from_unified_map (resources, "memory.max", హద్దు, err);
}

స్థిర అంతఃవరుస పూర్ణాంకం
get_జ్ఞాపకం_మార్పిడి_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *హద్దు, libcrun_error_t *err)
{
  ఒకవేళ (resources->జ్ఞాపకం && resources->జ్ఞాపకం->మార్పిడి_present)
    {
      runtime_spec_schema_config_linux_resources_జ్ఞాపకం *జ్ఞాపకం = resources->జ్ఞాపకం;
      int64_t mem = జ్ఞాపకం->హద్దు_present ? జ్ఞాపకం->హద్దు : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      ఒకవేళ (జ్ఞాపకం->మార్పిడి == -1 || (mem == -1 && జ్ఞాపకం->మార్పిడి == 0))
        {
          *హద్దు = UINT64_MAX;
          తిరిగి 1;
        }
      /* 0 means unset.  */
      ఒకవేళ (జ్ఞాపకం->మార్పిడి == 0)
        తిరిగి 0;
      ఒకవేళ (mem <= 0)
        తిరిగి crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ఒకవేళ (జ్ఞాపకం->మార్పిడి < mem)
        తిరిగి crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *హద్దు = జ్ఞాపకం->మార్పిడి - mem;
      తిరిగి 1;
    }

  తిరిగి get_విలువ_from_unified_map (resources, "memory.swap.max", హద్దు, err);
}

స్థిర అంతఃవరుస పూర్ణాంకం
get_cpu_బరువు (runtime_spec_schema_config_linux_resources *resources, uint64_t *బరువు, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t విలువ;
  పూర్ణాంకం ret;

  ret = get_విలువ_from_unified_map (resources, "cpu.idle", &విలువ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  ఒకవేళ (ret > 0 && విలువ == 1)
    has_idle = true;
  ఒకవేళ (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_విలువ_from_unified_map (resources, "cpu.weight", బరువు, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  ఒకవేళ (ret > 0)
    {
      ఒకవేళ (has_idle)
        తిరిగి crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      తిరిగి 1;
    }

  ఒకవేళ (resources->cpu && resources->cpu->shares_present)
    {
      ఒకవేళ (has_idle)
        తిరిగి crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      ఒకవేళ (resources->cpu->shares == 0)
        తిరిగి 0;
      *బరువు = convert_shares_గమ్యం_బరువు (resources->cpu->shares);
      తిరిగి 1;
    }
  ఒకవేళ (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *బరువు = 0;
      తిరిగి 1;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
read_uint64_విలువ (స్థిరరాశి అక్షరం *str, uint64_t *విలువ, libcrun_error_t *err)
{
  అక్షరం *endptr = ఖాళీ;

  errno = 0;
  *విలువ = strtoull (str, &endptr, 10);
  ఒకవేళ (UNLIKELY (errno))
    తిరిగి crun_make_error (err, errno, "invalid value: %s", str);
  ఒకవేళ (endptr && *endptr)
    తిరిగి crun_make_error (err, 0, "invalid value: %s", str);
  తిరిగి 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
స్థిర అంతఃవరుస uint64_t
IO_బరువు (uint64_t bfq_బరువు)
{
  తిరిగి bfq_బరువు <= CGROUP_BFQ_బరువు_DEFAULT ? CGROUP_బరువు_DEFAULT - (CGROUP_BFQ_బరువు_DEFAULT - bfq_బరువు) * (CGROUP_బరువు_DEFAULT - CGROUP_బరువు_MIN) / (CGROUP_BFQ_బరువు_DEFAULT - CGROUP_BFQ_బరువు_MIN) : CGROUP_బరువు_DEFAULT + (bfq_బరువు - CGROUP_BFQ_బరువు_DEFAULT) * (CGROUP_బరువు_MAX - CGROUP_బరువు_DEFAULT) / (CGROUP_BFQ_బరువు_MAX - CGROUP_BFQ_బరువు_DEFAULT);
}

స్థిర పూర్ణాంకం
append_io_బరువు (sd_bus_సందేశం *m, అక్షరం **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t బరువు;
  పూర్ణాంకం sd_err;
  size_t i;
  పూర్ణాంకం ret;

#  define APPEND_IO_బరువు(విలువ)                                                      \
    చేయి                                                                                 \
      {                                                                                \
        ఒకవేళ (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_సందేశం_append (m, "(sv)", "IOWeight", "t", బరువు);       \
            ఒకవేళ (UNLIKELY (sd_err < 0))                                                 \
              తిరిగి crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } అయితే (0)

#  define APPEND_IO_DEVICE_బరువు(device, విలువ)                                                      \
    చేయి                                                                                                \
      {                                                                                               \
        ఒకవేళ (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_సందేశం_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, బరువు); \
            ఒకవేళ (UNLIKELY (sd_err < 0))                                                                \
              తిరిగి crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } అయితే (0)

  ఒకవేళ (resources->ఖండం_io && resources->ఖండం_io->బరువు_present)
    {
      బరువు = IO_బరువు (resources->ఖండం_io->బరువు);
      APPEND_IO_బరువు (బరువు);
    }

  కొరకు (i = 0; resources->ఖండం_io && i < resources->ఖండం_io->బరువు_device_len; i++)
    {
      అక్షరం name[64];
      పూర్ణాంకం len;

      len = snprintf (name, పరిమాణం (name), "%" PRIu64 ":%" PRIu64,
                      resources->ఖండం_io->బరువు_device[i]->major,
                      resources->ఖండం_io->బరువు_device[i]->minor);
      ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (name)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      బరువు = IO_బరువు (resources->ఖండం_io->బరువు_device[i]->బరువు);
      APPEND_IO_DEVICE_బరువు (name, బరువు);
    }

  కొరకు (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free అక్షరం *విలువ = ఖాళీ;
      bool need_conversion = false;
      అక్షరం *saveptr = ఖాళీ;
      అక్షరం *పంక్తి;

      ఒకవేళ (strcmp (resources->unified->కీలకాలు[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      లేకపోతే ఒకవేళ (strcmp (resources->unified->కీలకాలు[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      లేకపోతే
        {
          /* Skip other keys.  */
          కొనసాగు;
        }

      విలువ = xstrdup (resources->unified->విలువలు[i]);

      కొరకు (పంక్తి = strtok_r (విలువ, "\n", &saveptr); పంక్తి; పంక్తి = strtok_r (ఖాళీ, "\n", &saveptr))
        {
          అక్షరం *sep;

          sep = strchr (పంక్తి, ' ');
          అయితే (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_విలువ (sep ? sep : పంక్తి, &బరువు, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          ఒకవేళ (need_conversion)
            బరువు = IO_బరువు (బరువు);

          ఒకవేళ (sep == ఖాళీ || strcmp (పంక్తి, "default") == 0)
            APPEND_IO_బరువు (బరువు);
          లేకపోతే
            APPEND_IO_DEVICE_బరువు (పంక్తి, బరువు);
        }
    }

  తిరిగి 0;
}

/* Adapted from systemd.  */
స్థిర పూర్ణాంకం
bus_append_byte_array (sd_bus_సందేశం *m, స్థిరరాశి అక్షరం *క్షేత్రం, స్థిరరాశి శూన్య *buf, size_t n, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = sd_bus_సందేశం_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  ఒకవేళ (ret < 0)
    తిరిగి crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_సందేశం_append_basic (m, SD_BUS_TYPE_STRING, క్షేత్రం);
  ఒకవేళ (ret < 0)
    తిరిగి crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_సందేశం_open_container (m, 'v', "ay");
  ఒకవేళ (ret < 0)
    తిరిగి crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_సందేశం_append_array (m, 'y', buf, n);
  ఒకవేళ (ret < 0)
    తిరిగి crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_సందేశం_close_container (m);
  ఒకవేళ (ret < 0)
    తిరిగి crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_సందేశం_close_container (m);
  ఒకవేళ (ret < 0)
    తిరిగి crun_make_error (err, -ret, "sd_bus_message_close_container");

  తిరిగి 1;
}

స్థిర పూర్ణాంకం
append_uint64_from_unified_map (sd_bus_సందేశం *m,
                                అక్షరం **missing_properties,
                                స్థిరరాశి అక్షరం *attr,
                                స్థిరరాశి అక్షరం *కీలకం,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t విలువ;
  పూర్ణాంకం ret;

  ఒకవేళ (property_missing_p (missing_properties, attr))
    తిరిగి 0;

  ret = get_విలువ_from_unified_map (resources, కీలకం, &విలువ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  ఒకవేళ (ret)
    {
      పూర్ణాంకం sd_err = sd_bus_సందేశం_append (m, "(sv)", attr, "t", విలువ);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  తిరిగి 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
స్థిర పూర్ణాంకం
append_device_allow (sd_bus_సందేశం *m,
                     స్థిరరాశి అక్షరం type,
                     పూర్ణాంకం major,
                     పూర్ణాంకం minor,
                     స్థిరరాశి అక్షరం *access,
                     libcrun_error_t *err)
{
  అక్షరం device[64];
  పూర్ణాంకం sd_err;
  పూర్ణాంకం len;

  ఒకవేళ (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      తిరిగి 0;
    }

  ఒకవేళ (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, పరిమాణం (device), "%s-*", type == 'c' ? "char" : "block");
  లేకపోతే ఒకవేళ (IS_WILDCARD (minor))
    len = snprintf (device, పరిమాణం (device), type == 'c' ? "char-%d" : "block-%d", major);
  లేకపోతే
    len = snprintf (device, పరిమాణం (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (device)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_సందేశం_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    తిరిగి crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  తిరిగి 0;
}

#  define బహుళకం_R 1
#  define బహుళకం_W 2
#  define బహుళకం_M 4

స్థిర పూర్ణాంకం
access_గమ్యం_బహుళకం (స్థిరరాశి అక్షరం *access, libcrun_error_t *err)
{
  పూర్ణాంకం బహుళకం = 0;

  అయితే (access && *access)
    {
      పూర్ణాంకం c = *access++;

      ఎంపిక (c)
        {
        స్థితి 'r':
          బహుళకం |= బహుళకం_R;
          ఆపు;

        స్థితి 'w':
          బహుళకం |= బహుళకం_W;
          ఆపు;

        స్థితి 'm':
          బహుళకం |= బహుళకం_M;
          ఆపు;

        అప్రమేయ:
          తిరిగి crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  తిరిగి బహుళకం;
}

/* check if the type b is included in type a.  */
స్థిర bool
has_same_type (స్థిరరాశి అక్షరం *a, స్థిరరాశి అక్షరం *b)
{
  bool a_all = a == ఖాళీ || strcmp (a, "a") == 0;
  ఒకవేళ (b == ఖాళీ || strcmp (b, "a") == 0)
    తిరిగి a_all;

  తిరిగి a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
స్థిర పూర్ణాంకం
is_deny_నియమం_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  పూర్ణాంకం req_బహుళకం;

#  define CMP_DEV_అంకె(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_బహుళకం = access_గమ్యం_బహుళకం (devices[n]->access, err);
  ఒకవేళ (UNLIKELY (req_బహుళకం < 0))
    తిరిగి req_బహుళకం;

  కొరకు (i = n; i > 0; i--)
    {
      size_t current_నియమం = i - 1;
      పూర్ణాంకం బహుళకం;

      ఒకవేళ (! CMP_DEV_అంకె (devices[n]->minor, devices[current_నియమం]->minor))
        కొనసాగు;
      ఒకవేళ (! CMP_DEV_అంకె (devices[n]->major, devices[current_నియమం]->major))
        కొనసాగు;

      ఒకవేళ (! has_same_type (devices[n]->type, devices[current_నియమం]->type)
          && has_same_type ("a", devices[current_నియమం]->type))
        కొనసాగు;

      బహుళకం = access_గమ్యం_బహుళకం (devices[i]->access, err);
      ఒకవేళ (UNLIKELY (బహుళకం < 0))
        తిరిగి బహుళకం;

      ఒకవేళ (devices[current_నియమం]->allow && (బహుళకం & req_బహుళకం))
        తిరిగి 0;

      req_బహుళకం &= ! బహుళకం;

      /* no more access bits to validate.  */
      ఒకవేళ (req_బహుళకం == 0)
        తిరిగి 1;
    }

  తిరిగి 0;
#  undef CMP_DEV_అంకె
}

స్థిర size_t
కనుగొను_మొదటి_నియమం_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  ఒకవేళ (n == 0)
    తిరిగి 1;

  /* Find the first rule that is after the last "block all".  */
  కొరకు (i = n - 1; i-- > 0;)
    {
      ఒకవేళ ((is_ఖాళీలు_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        తిరిగి i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  కొరకు (i = 0; i < n; i++)
    ఒకవేళ (devices[i]->allow)
      తిరిగి i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  తిరిగి n + 1;
}

స్థిర bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_ఖండం 1
#  define DEV_TYPE_CHAR 2
  పూర్ణాంకం మిగిలిన = DEV_TYPE_ఖండం | DEV_TYPE_CHAR;
  size_t i;

  కొరకు (i = 0; i < n; i++)
    {
      size_t current_నియమం = n - i - 1;
      ఒకవేళ (! devices[current_నియమం]->allow)
        తిరిగి false;

      ఒకవేళ (IS_WILDCARD (devices[current_నియమం]->major) && IS_WILDCARD (devices[current_నియమం]->minor))
        {
          ఒకవేళ (is_ఖాళీలు_string (devices[current_నియమం]->type) || strcmp (devices[current_నియమం]->type, "a") == 0)
            తిరిగి true;

          ఒకవేళ (strcmp (devices[current_నియమం]->type, "c") == 0)
            మిగిలిన &= ~DEV_TYPE_CHAR;
          లేకపోతే ఒకవేళ (strcmp (devices[current_నియమం]->type, "b") == 0)
            మిగిలిన &= ~DEV_TYPE_ఖండం;

          ఒకవేళ (మిగిలిన == 0)
            తిరిగి true;
        }
    }
#  undef DEV_TYPE_ఖండం
#  undef DEV_TYPE_CHAR

  తిరిగి false;
}

స్థిర పూర్ణాంకం
append_devices (sd_bus_సందేశం *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  నిర్మాణం default_dev_s *default_devices = get_default_devices ();
  పూర్ణాంకం ret, sd_err;
  size_t i;

  ఒకవేళ (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_సందేశం_append (m, "(sv)", "DevicePolicy", "s", "auto");
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_సందేశం_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      తిరిగి 0;
    }

  sd_err = sd_bus_సందేశం_append (m, "(sv)", "DevicePolicy", "s", "strict");
  ఒకవేళ (UNLIKELY (sd_err < 0))
    తిరిగి crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  కొరకు (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  కొరకు (i = కనుగొను_మొదటి_నియమం_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      అక్షరం type;

      ఒకవేళ (! d->allow)
        {
          పూర్ణాంకం redundant;

          redundant = is_deny_నియమం_redundant (resources->devices, i, err);
          ఒకవేళ (UNLIKELY (redundant < 0))
            తిరిగి redundant;

          ఒకవేళ (redundant)
            కొనసాగు;

          తిరిగి crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      ఒకవేళ (d->type == ఖాళీ || strcmp (d->type, "a") == 0)
        type = 'a';
      లేకపోతే ఒకవేళ (strcmp (d->type, "c") == 0)
        type = 'c';
      లేకపోతే ఒకవేళ (strcmp (d->type, "b") == 0)
        type = 'b';
      లేకపోతే
        తిరిగి crun_make_error (err, 0, "unknown device type `%s`", d->type);

      ఒకవేళ (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
      లేకపోతే
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }

  తిరిగి 0;
}

స్థిర bool
has_bpf_fs ()
{
  నిర్మాణం statfs stat;

  తిరిగి statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

స్థిర పూర్ణాంకం
bpfprog_మార్గం_from_scope (అక్షరం **మార్గం, స్థిరరాశి అక్షరం *scope, libcrun_error_t *err)
{
  cleanup_free అక్షరం *flat_scope = xstrdup (scope);
  అక్షరం *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  అయితే ((it = strchr (it, '.')) != ఖాళీ)
    *it = '_';

  తిరిగి append_paths (మార్గం, err, CRUN_BPF_DIR, flat_scope, ఖాళీ);
}

స్థిర పూర్ణాంకం
కూడు_bpf_కార్యక్రమం (sd_bus_సందేశం *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 అక్షరం **missing_properties,
                 స్థిరరాశి అక్షరం *scope,
                 bool *devices_సమితి,
                 libcrun_error_t *err)
{
  పూర్ణాంకం ret = 0;
  పూర్ణాంకం లోపల_userns, sd_err;
  స్థిరరాశి అక్షరం *property_name = "BPFProgram";
  cleanup_free నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం = ఖాళీ;
  cleanup_free అక్షరం *మార్గం = ఖాళీ;

  *devices_సమితి = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  ఒకవేళ (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    తిరిగి 0;

  లోపల_userns = check_running_లోపల_user_namespace (err);
  ఒకవేళ (UNLIKELY (లోపల_userns < 0))
    తిరిగి లోపల_userns;

  ఒకవేళ (లోపల_userns)
    తిరిగి 0;

  కార్యక్రమం = create_dev_bpf (resources->devices, resources->devices_len, err);
  ఒకవేళ (UNLIKELY (కార్యక్రమం == ఖాళీ))
    {
      crun_error_release (err);
      తిరిగి 0;
    }

  ret = bpfprog_మార్గం_from_scope (&మార్గం, scope, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  ఒకవేళ (UNLIKELY (ret < 0 && errno != EEXIST))
    తిరిగి crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  ఒకవేళ (is_update)
    {
      cleanup_free నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం_loaded = ఖాళీ;

      ఒకవేళ (resources->devices == ఖాళీ || resources->devices_len == 0)
        {
          *devices_సమితి = true;
          తిరిగి 0;
        }

      ret = libcrun_ebpf_read_కార్యక్రమం (&కార్యక్రమం_loaded, మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      /* it is the same as the loaded program.  */
      ఒకవేళ (libcrun_ebpf_cmp_programs (కార్యక్రమం, కార్యక్రమం_loaded))
        {
          *devices_సమితి = true;
          తిరిగి 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం_new = ఖాళీ;
        అక్షరం tmp[64];
        పూర్ణాంకం len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, పరిమాణం (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (tmp)))
          తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_ఎక్కించు (కార్యక్రమం, -1, tmp, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        ret = libcrun_ebpf_read_కార్యక్రమం (&కార్యక్రమం_new, tmp, err);
        unlink (tmp);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        ఒకవేళ (libcrun_ebpf_cmp_programs (కార్యక్రమం_new, కార్యక్రమం_loaded))
          {
            *devices_సమితి = true;
            తిరిగి 0;
          }
      }

      తిరిగి crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_ఎక్కించు (కార్యక్రమం, -1, మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  sd_err = sd_bus_సందేశం_append (m, "(sv)", property_name, "a(ss)", 1, "device", మార్గం);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    తిరిగి crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_సమితి = true;

  తిరిగి 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
స్థిర పూర్ణాంకం
append_cpu_quota (sd_bus_సందేశం *m, అక్షరం **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  స్థిరరాశి అక్షరం *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  పూర్ణాంకం sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ఒకవేళ (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      పూర్ణాంకం ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      ఒకవేళ (has_period)
        period = max_period;
    }

  ఒకవేళ (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_సందేశం_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  ఒకవేళ ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_చొప్పున_sec = UINT64_MAX;

      ఒకవేళ (quota > 0)
        {
          quota_చొప్పున_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          ఒకవేళ (quota_చొప్పున_sec % 10000)
            quota_చొప్పున_sec = ((quota_చొప్పున_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_సందేశం_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_చొప్పున_sec);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        తిరిగి crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
append_resources (sd_bus_సందేశం *m,
                  bool is_update,
                  స్థిరరాశి అక్షరం *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  పూర్ణాంకం cgroup_బహుళకం,
                  స్థిరరాశి అక్షరం *scope,
                  bool *devices_సమితి,
                  libcrun_error_t *err)
{
  uint64_t విలువ;
  పూర్ణాంకం sd_err;
  పూర్ణాంకం ret;
  cleanup_free అక్షరం *dir = ఖాళీ;
  cleanup_free అక్షరం **missing_properties = ఖాళీ;

  *devices_సమితి = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  missing_properties = read_dir_entries (dir, err);
  ఒకవేళ (UNLIKELY (missing_properties == ఖాళీ))
    {
      ఒకవేళ (crun_error_get_errno (err) != ENOENT)
        తిరిగి -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_విలువ(name, విలువ)                                             \
    చేయి                                                                                 \
      {                                                                                \
        ఒకవేళ (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_సందేశం_append (m, "(sv)", name, "t", విలువ);              \
            ఒకవేళ (UNLIKELY (sd_err < 0))                                                 \
              తిరిగి crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } అయితే (0)

#  define APPEND_UINT64(name, fn)              \
    చేయి                                         \
      {                                        \
        ret = fn (resources, &విలువ, err);     \
        ఒకవేళ (UNLIKELY (ret < 0))                \
          తిరిగి ret;                          \
        ఒకవేళ (ret)                               \
          {                                    \
            APPEND_UINT64_విలువ (name, విలువ); \
          }                                    \
    } అయితే (0)

  ఒకవేళ (resources == ఖాళీ)
    తిరిగి 0;

  APPEND_UINT64 ("MemoryLow", get_జ్ఞాపకం_దిగువ);
  APPEND_UINT64 ("MemoryMax", get_జ్ఞాపకం_max);
  APPEND_UINT64 ("MemorySwapMax", get_జ్ఞాపకం_మార్పిడి_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఎంపిక (cgroup_బహుళకం)
    {
    స్థితి CGROUP_బహుళకం_UNIFIED:
      {
        ret = append_io_బరువు (m, missing_properties, resources, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_బరువు);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;

        ఒకవేళ (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            స్థిరరాశి అక్షరం *property_name = "AllowedCPUs";
            స్థిరరాశి అక్షరం *విలువ = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free అక్షరం *allowed_cpus = ఖాళీ;
            size_t allowed_cpus_len = 0;

            ఒకవేళ (విలువ == ఖాళీ)
              విలువ = resources->cpu->cpus;

            ret = cpuset_string_గమ్యం_bitmask (విలువ, &allowed_cpus, &allowed_cpus_len, err);
            ఒకవేళ (UNLIKELY (ret < 0))
              తిరిగి ret;

            ఒకవేళ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                ఒకవేళ (UNLIKELY (ret < 0))
                  తిరిగి ret;
              }
          }

        ఒకవేళ (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            స్థిరరాశి అక్షరం *property_name = "AllowedMemoryNodes";
            స్థిరరాశి అక్షరం *విలువ = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free అక్షరం *allowed_mems = ఖాళీ;
            size_t allowed_mems_len = 0;

            ఒకవేళ (విలువ == ఖాళీ)
              విలువ = resources->cpu->mems;

            ret = cpuset_string_గమ్యం_bitmask (విలువ, &allowed_mems, &allowed_mems_len, err);
            ఒకవేళ (UNLIKELY (ret < 0))
              తిరిగి ret;

            ఒకవేళ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                ఒకవేళ (UNLIKELY (ret < 0))
                  తిరిగి ret;
              }
          }

        ret = కూడు_bpf_కార్యక్రమం (m, is_update, resources, missing_properties, scope, devices_సమితి, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          తిరిగి ret;
      }
      ఆపు;

    స్థితి CGROUP_బహుళకం_LEGACY:
    స్థితి CGROUP_బహుళకం_HYBRID:
      ఒకవేళ (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_విలువ ("CPUShares", resources->cpu->shares);
      ఆపు;

    అప్రమేయ:
      తిరిగి crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_బహుళకం);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_విలువ

  ఒకవేళ (! *devices_సమితి && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
reset_failed_unit (sd_bus *bus, స్థిరరాశి అక్షరం *unit)
{
  పూర్ణాంకం sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_సందేశం *m = ఖాళీ, *జవాబు = ఖాళీ;

  sd_err = sd_bus_సందేశం_new_పద్ధతి_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  ఒకవేళ (UNLIKELY (sd_err < 0))
    వెళ్ళు నిష్క్రమించు;

  sd_err = sd_bus_సందేశం_append (m, "s", unit);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    వెళ్ళు నిష్క్రమించు;

  sd_err = sd_bus_call (bus, m, 0, &error, &జవాబు);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    వెళ్ళు నిష్క్రమించు;

  sd_err = 0;

నిష్క్రమించు:
  ఒకవేళ (m)
    sd_bus_సందేశం_unref (m);
  ఒకవేళ (జవాబు)
    sd_bus_సందేశం_unref (జవాబు);
  sd_bus_error_free (&error);

  తిరిగి sd_err;
}

స్థిర పూర్ణాంకం
verify_ebpf_device_filter_installed (స్థిరరాశి అక్షరం *cgroup_మార్గం, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = ఖాళీ;
  cleanup_free అక్షరం *నిండిన_మార్గం = ఖాళీ;
  size_t n_progs = 0;
  పూర్ణాంకం ret;

  ret = append_paths (&నిండిన_మార్గం, err, CGROUP_మూలం, cgroup_మార్గం, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_ebpf_query_cgroup_progs (నిండిన_మార్గం, &progs, &n_progs, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (n_progs == 0)
    తిరిగి crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", నిండిన_మార్గం);

  తిరిగి 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
స్థిర పూర్ణాంకం
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_సందేశం *m, sd_bus_సందేశం *జవాబు,
                           sd_bus_error *error, పూర్ణాంకం ret)
{
  ఒకవేళ (bus)
    sd_bus_unref (bus);
  ఒకవేళ (m)
    sd_bus_సందేశం_unref (m);
  ఒకవేళ (జవాబు)
    sd_bus_సందేశం_unref (జవాబు);
  sd_bus_error_free (error);
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            పూర్ణాంకం cgroup_బహుళకం,
                            string_map *annotations,
                            స్థిరరాశి అక్షరం *state_మూలం,
                            స్థిరరాశి అక్షరం *scope, స్థిరరాశి అక్షరం *ముక్క,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_సమితి,
                            libcrun_error_t *err)
{
  sd_bus *bus = ఖాళీ;
  sd_bus_సందేశం *m = ఖాళీ;
  sd_bus_సందేశం *జవాబు = ఖాళీ;
  పూర్ణాంకం sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  స్థిరరాశి అక్షరం *object = ఖాళీ;
  నిర్మాణం systemd_పని_removed_s పని_దత్తాంశం = {};
  పూర్ణాంకం i;
  స్థిరరాశి అక్షరం *boolean_opts[10];
  cleanup_free అక్షరం *state_dir = ఖాళీ;

  *can_retry = false;
  *devices_సమితి = false;

  ret = libcrun_get_state_directory (&state_dir, state_మూలం, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = ఖాళీ;

  ret = open_sd_bus_connection (&bus, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు నిష్క్రమించు;

  ret = systemd_check_పని_స్థితి_అమరిక (bus, &పని_దత్తాంశం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు నిష్క్రమించు;

  sd_err = sd_bus_సందేశం_new_పద్ధతి_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      వెళ్ళు నిష్క్రమించు;
    }

  sd_err = sd_bus_సందేశం_append (m, "ss", scope, "fail");
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      వెళ్ళు నిష్క్రమించు;
    }

  sd_err = sd_bus_సందేశం_open_container (m, 'a', "(sv)");
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      వెళ్ళు నిష్క్రమించు;
    }

  ఒకవేళ (ముక్క)
    {
      sd_err = sd_bus_సందేశం_append (m, "(sv)", "Slice", "s", ముక్క);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          వెళ్ళు నిష్క్రమించు;
        }
    }

  sd_err = sd_bus_సందేశం_append (m, "(sv)", "DefaultDependencies", "b", 0);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      వెళ్ళు నిష్క్రమించు;
    }

  ఒకవేళ (annotations)
    {
      size_t ఉపసర్గ_len = పరిమాణం (SYSTEMD_PROPERTY_ఉపసర్గ) - 1;
      size_t annotations_len = string_map_కొలత (annotations);
      size_t i;

      కొరకు (i = 0; i < annotations_len; i++)
        {
          స్థిరరాశి అక్షరం *కీలకం;
          స్థిరరాశి అక్షరం *విలువ;
          size_t len;

          ret = string_map_get_at (annotations, i, &కీలకం, &విలువ);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              వెళ్ళు నిష్క్రమించు;
            }

          ఒకవేళ (! has_ఉపసర్గ (కీలకం, SYSTEMD_PROPERTY_ఉపసర్గ))
            కొనసాగు;

          len = అక్షరశ్రేణి_పొడవు (కీలకం);
          ఒకవేళ (len < ఉపసర్గ_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", కీలకం);
              వెళ్ళు నిష్క్రమించు;
            }

          ret = append_systemd_annotation (m, కీలకం + ఉపసర్గ_len, len - ఉపసర్గ_len,
                                           విలువ, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            వెళ్ళు నిష్క్రమించు;
        }
    }

  sd_err = sd_bus_సందేశం_append (m, "(sv)", "Description", "s", "libcrun container");
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      వెళ్ళు నిష్క్రమించు;
    }

  sd_err = sd_bus_సందేశం_append (m, "(sv)", "PIDs", "au", 1, pid);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      వెళ్ళు నిష్క్రమించు;
    }

  కొరకు (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_సందేశం_append (m, "(sv)", boolean_opts[i], "b", 1);
      ఒకవేళ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          వెళ్ళు నిష్క్రమించు;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_బహుళకం, scope, devices_సమితి, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు నిష్క్రమించు;

  sd_err = sd_bus_సందేశం_close_container (m);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      వెళ్ళు నిష్క్రమించు;
    }

  sd_err = sd_bus_సందేశం_append (m, "a(sa(sv))", 0);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      వెళ్ళు నిష్క్రమించు;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &జవాబు);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ఒకవేళ (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          ఒకవేళ (జవాబు)
            sd_bus_సందేశం_unref (జవాబు);

          error = SD_BUS_ERROR_NULL;
          జవాబు = ఖాళీ;

          sd_err = sd_bus_call (bus, m, 0, &error, &జవాబు);
        }
      ఒకవేళ (sd_err < 0)
        {
          ఒకవేళ (sd_err == -EROFS)
            {
              ret = రిజిస్టరు_missing_property_from_సందేశం (state_dir, error.సందేశం_2, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                వెళ్ళు నిష్క్రమించు;
              ఒకవేళ (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.సందేశం_2 ?: error.name);
          వెళ్ళు నిష్క్రమించు;
        }
    }

  sd_err = sd_bus_సందేశం_read (జవాబు, "o", &object);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      వెళ్ళు నిష్క్రమించు;
    }

  ret = systemd_check_పని_స్థితి (bus, &పని_దత్తాంశం, object, "creating", err);

నిష్క్రమించు:
  తిరిగి cleanup_sd_bus_and_return (bus, m, జవాబు, &error, ret);
}

స్థిర పూర్ణాంకం
libcrun_destroy_systemd_cgroup_scope (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి,
                                      libcrun_error_t *err)
{
  sd_bus *bus = ఖాళీ;
  sd_bus_సందేశం *m = ఖాళీ;
  sd_bus_సందేశం *జవాబు = ఖాళీ;
  పూర్ణాంకం ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  స్థిరరాశి అక్షరం *object;
  స్థిరరాశి అక్షరం *scope = cgroup_స్థితి->scope;
  నిర్మాణం systemd_పని_removed_s పని_దత్తాంశం = {};

  ret = open_sd_bus_connection (&bus, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు నిష్క్రమించు;

  ret = systemd_check_పని_స్థితి_అమరిక (bus, &పని_దత్తాంశం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు నిష్క్రమించు;

  ret = sd_bus_సందేశం_new_పద్ధతి_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      వెళ్ళు నిష్క్రమించు;
    }

  ret = sd_bus_సందేశం_append (m, "ss", scope, "replace");
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      వెళ్ళు నిష్క్రమించు;
    }

  ret = sd_bus_call (bus, m, 0, &error, &జవాబు);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.సందేశం_2 ?: error.name);
      వెళ్ళు నిష్క్రమించు;
    }

  ret = sd_bus_సందేశం_read (జవాబు, "o", &object);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      వెళ్ళు నిష్క్రమించు;
    }

  ret = systemd_check_పని_స్థితి (bus, &పని_దత్తాంశం, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

నిష్క్రమించు:
  తిరిగి cleanup_sd_bus_and_return (bus, m, జవాబు, &error, ret);
}

స్థిర స్థిరరాశి అక్షరం *
కనుగొను_systemd_subgroup (string_map *annotations)
{
  స్థిరరాశి అక్షరం *annotation;

  annotation = కనుగొను_string_map_విలువ (annotations, "run.oci.systemd.subgroup");
  ఒకవేళ (annotation)
    {
      ఒకవేళ (annotation[0] == '\0')
        తిరిగి ఖాళీ;
      తిరిగి annotation;
    }

  తిరిగి "container";
}

స్థిర పూర్ణాంకం
libcrun_cgroup_enter_systemd (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు,
                              నిర్మాణం libcrun_cgroup_స్థితి *బహిర్గతం,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = వాదనలు->resources;
  స్థిరరాశి అక్షరం *cgroup_మార్గం = వాదనలు->cgroup_మార్గం;
  cleanup_free అక్షరం *scope_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *scope = ఖాళీ;
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  cleanup_free అక్షరం *ముక్క = ఖాళీ;
  పూర్ణాంకం retries_ఎడమ = 32;
  స్థిరరాశి అక్షరం *suffix;
  స్థిరరాశి అక్షరం *id = వాదనలు->id;
  pid_t pid = వాదనలు->pid;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం rootless = 0;
  పూర్ణాంకం ret;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
    {
      rootless = is_rootless (err);
      ఒకవేళ (UNLIKELY (rootless < 0))
        తిరిగి rootless;
    }

  get_systemd_scope_and_ముక్క (id, rootless == 1, cgroup_మార్గం, &scope, &ముక్క);

  కొరకు (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_బహుళకం, వాదనలు->annotations, వాదనలు->state_మూలం,
                                        scope, ముక్క, pid, &can_retry, &బహిర్గతం->bpf_dev_సమితి, err);
      ఒకవేళ (LIKELY (ret >= 0))
        ఆపు;

      ఒకవేళ (can_retry && retries_ఎడమ-- > 0)
        {
          crun_error_release (err);
          కొనసాగు;
        }

      తిరిగి ret;
    }

  suffix = కనుగొను_systemd_subgroup (వాదనలు->annotations);

  ret = systemd_finalize (వాదనలు, &మార్గం, cgroup_బహుళకం, suffix, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  ఒకవేళ (is_ఖాళీలు_string (suffix))
    scope_మార్గం = xstrdup (మార్గం);
  లేకపోతే
    {
      size_t మార్గం_len = అక్షరశ్రేణి_పొడవు (మార్గం);
      size_t suffix_len = అక్షరశ్రేణి_పొడవు (suffix);

      scope_మార్గం = strndup (మార్గం, మార్గం_len - suffix_len - 1);
      ఒకవేళ (UNLIKELY (scope_మార్గం == ఖాళీ))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  ఒకవేళ (బహిర్గతం->bpf_dev_సమితి)
    {
      ret = verify_ebpf_device_filter_installed (scope_మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  బహిర్గతం->మార్గం = మార్గం;
  మార్గం = ఖాళీ;

  బహిర్గతం->scope = scope;
  scope = ఖాళీ;
  తిరిగి 0;
}

అక్షరం *
get_cgroup_scope_మార్గం (స్థిరరాశి అక్షరం *cgroup_మార్గం, స్థిరరాశి అక్షరం *scope)
{
  అక్షరం *మార్గం_గమ్యం_scope = ఖాళీ;
  అక్షరం *cur;

  ఒకవేళ (is_ఖాళీలు_string (cgroup_మార్గం))
    తిరిగి ఖాళీ;

  మార్గం_గమ్యం_scope = xstrdup (cgroup_మార్గం);

  cur = strchr (మార్గం_గమ్యం_scope, '/');
  అయితే (cur)
    {
      అక్షరం *తదుపరి = strchr (cur + 1, '/');
      ఒకవేళ (తదుపరి == ఖాళీ)
        ఆపు;

      *తదుపరి = '\0';
      ఒకవేళ (strcmp (cur, scope) == 0)
        తిరిగి మార్గం_గమ్యం_scope;
      *తదుపరి = '/';

      cur = తదుపరి;
      అయితే (*cur == '/')
        cur++;
    }

  తిరిగి మార్గం_గమ్యం_scope;
}

స్థిర పూర్ణాంకం
libcrun_destroy_cgroup_systemd (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి,
                                libcrun_error_t *err)
{
  cleanup_free అక్షరం *మార్గం_గమ్యం_scope = ఖాళీ;
  cleanup_free అక్షరం *bpfprog = ఖాళీ;
  పూర్ణాంకం బహుళకం;
  పూర్ణాంకం ret;

  బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (బహుళకం < 0))
    తిరిగి బహుళకం;

  ret = cgroup_killall_మార్గం (cgroup_స్థితి->మార్గం, SIGKILL, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_స్థితి, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_మార్గం_from_scope (&bpfprog, cgroup_స్థితి->scope, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    crun_error_release (err);
  లేకపోతే
    unlink (bpfprog); // Best effort.

  మార్గం_గమ్యం_scope = get_cgroup_scope_మార్గం (cgroup_స్థితి->మార్గం, cgroup_స్థితి->scope);

  తిరిగి destroy_cgroup_మార్గం (మార్గం_గమ్యం_scope, బహుళకం, err);
}

స్థిర పూర్ణాంకం
libcrun_update_resources_systemd (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి,
                                  స్థిరరాశి అక్షరం *state_మూలం,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  నిర్మాణం systemd_పని_removed_s పని_దత్తాంశం = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free అక్షరం *state_dir = ఖాళీ;
  sd_bus_సందేశం *జవాబు = ఖాళీ;
  sd_bus_సందేశం *m = ఖాళీ;
  sd_bus *bus = ఖాళీ;
  పూర్ణాంకం sd_err, ret;
  పూర్ణాంకం cgroup_బహుళకం;

  ret = libcrun_get_state_directory (&state_dir, state_మూలం, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ret = open_sd_bus_connection (&bus, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = systemd_check_పని_స్థితి_అమరిక (bus, &పని_దత్తాంశం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు నిష్క్రమించు;

  sd_err = sd_bus_సందేశం_new_పద్ధతి_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      వెళ్ళు నిష్క్రమించు;
    }

  sd_err = sd_bus_సందేశం_append (m, "sb", cgroup_స్థితి->scope, 1);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      వెళ్ళు నిష్క్రమించు;
    }

  sd_err = sd_bus_సందేశం_open_container (m, 'a', "(sv)");
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      వెళ్ళు నిష్క్రమించు;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_బహుళకం, cgroup_స్థితి->scope, &cgroup_స్థితి->bpf_dev_సమితి, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు నిష్క్రమించు;

  sd_err = sd_bus_సందేశం_close_container (m);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      వెళ్ళు నిష్క్రమించు;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &జవాబు);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.సందేశం_2 ?: error.name);
      వెళ్ళు నిష్క్రమించు;
    }

  ఒకవేళ (cgroup_బహుళకం != CGROUP_బహుళకం_UNIFIED)
    {
      ret = అమరిక_rt_runtime (resources, cgroup_స్థితి->మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు నిష్క్రమించు;

      ret = అమరిక_cpuset_for_systemd_v1 (resources, cgroup_స్థితి->మార్గం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు నిష్క్రమించు;
    }

  ret = అమరిక_missing_cpu_options_for_systemd (resources, cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED, cgroup_స్థితి->మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు నిష్క్రమించు;

  ret = 0;

నిష్క్రమించు:
  తిరిగి cleanup_sd_bus_and_return (bus, m, జవాబు, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
స్థిర పూర్ణాంకం
libcrun_attach_process_systemd (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి,
                                స్థిరరాశి అక్షరం *మార్గం, pid_t pid,
                                libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *scope = cgroup_స్థితి->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free అక్షరం *scope_మార్గం = ఖాళీ;
  sd_bus_సందేశం *జవాబు = ఖాళీ;
  స్థిరరాశి అక్షరం *subcgroup;
  స్థిరరాశి అక్షరం *చివరి;
  sd_bus *bus = ఖాళీ;
  size_t len;
  పూర్ణాంకం sd_err, ret;

  ఒకవేళ (is_ఖాళీలు_string (scope))
    తిరిగి crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_మార్గం = get_cgroup_scope_మార్గం (మార్గం, scope);
  ఒకవేళ (scope_మార్గం == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  చివరి = strrchr (scope_మార్గం, '/');
  ఒకవేళ (చివరి == ఖాళీ || strcmp (చివరి + 1, scope) != 0)
    తిరిగి crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, మార్గం);
  len = అక్షరశ్రేణి_పొడవు (scope_మార్గం);
  subcgroup = మార్గం[len] ? మార్గం + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  sd_err = sd_bus_call_పద్ధతి (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &జవాబు, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  ఒకవేళ (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.సందేశం_2 ?: error.name);

  తిరిగి cleanup_sd_bus_and_return (bus, ఖాళీ, జవాబు, &error, ret);
}

#లేకపోతే
స్థిర పూర్ణాంకం
libcrun_cgroup_enter_systemd (నిర్మాణం libcrun_cgroup_వాదనలు *వాదనలు,
                              నిర్మాణం libcrun_cgroup_స్థితి *బహిర్గతం,
                              libcrun_error_t *err)
{
  (శూన్య) వాదనలు;
  (శూన్య) బహిర్గతం;

  తిరిగి crun_make_error (err, ENOTSUP, "systemd not supported");
}

స్థిర పూర్ణాంకం
libcrun_destroy_cgroup_systemd (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి,
                                libcrun_error_t *err)
{
  (శూన్య) cgroup_స్థితి;

  తిరిగి crun_make_error (err, ENOTSUP, "systemd not supported");
}

స్థిర పూర్ణాంకం
libcrun_update_resources_systemd (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి,
                                  స్థిరరాశి అక్షరం *state_మూలం,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (శూన్య) cgroup_స్థితి;
  (శూన్య) state_మూలం;
  (శూన్య) resources;

  తిరిగి crun_make_error (err, ENOTSUP, "systemd not supported");
}

స్థిర పూర్ణాంకం
libcrun_attach_process_systemd (నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి,
                                స్థిరరాశి అక్షరం *మార్గం, pid_t pid,
                                libcrun_error_t *err)
{
  (శూన్య) cgroup_స్థితి;
  (శూన్య) మార్గం;
  (శూన్య) pid;

  తిరిగి crun_make_error (err, ENOTSUP, "systemd not supported");
}
#అయితే_ముగింపు

నిర్మాణం libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = ఖాళీ,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
