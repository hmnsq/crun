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
#अन्तर्भाव "cgroup-resources.h"
#अन्तर्भाव "cgroup-utils.h"
#अन्तर्भाव "ebpf.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "status.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <linux/magic.h>

#यदि_परिभाषितम् HAVE_SYSTEMD
#  include <systemd/sd-bus.h>
#  include <limits.h>

#  define SYSTEMD_PROPERTY_उपसर्गः "org.systemd.property."

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

स्थिर पूर्णाङ्क
पञ्जिकाकोशः_missing_property_from_सन्देशः (ध्रुव वर्ण *state_dir, ध्रुव वर्ण *सन्देशः_2, libcrun_error_t *err)
{
  cleanup_free वर्ण *file_मार्गः = रिक्त;
  cleanup_free वर्ण *dir_मार्गः = रिक्त;
  cleanup_free वर्ण *property = रिक्त;
  वर्ण *p;
  पूर्णाङ्क ret;

  यदि (! has_उपसर्गः (सन्देशः_2, "Cannot set property "))
    प्रत्यागम 0;

  property = xstrdup (सन्देशः_2 + परिमाण ("Cannot set property ") - 1);
  p = strchr (property, ',');
  यदि (! p)
    प्रत्यागम 0;
  *p = '\0';

  ret = append_paths (&dir_मार्गः, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = crun_ensure_directory (dir_मार्गः, 0755, true, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (&file_मार्गः, err, dir_मार्गः, property, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_मार्गः, रिक्त, 0, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 1;
}

स्थिर bool
property_missing_p (वर्ण **missing_properties, ध्रुव वर्ण *property)
{
  size_t i;

  प्रति (i = 0; missing_properties && missing_properties[i]; i++)
    यदि (वाक्यं_तुलय (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        प्रत्यागम true;
      }

  प्रत्यागम false;
}

स्थिर शून्य
get_systemd_scope_and_खण्डिका (ध्रुव वर्ण *id, bool user_खण्डिका, ध्रुव वर्ण *cgroup_मार्गः,
                             वर्ण **scope, वर्ण **खण्डिका)
{
  ध्रुव वर्ण *n;
  वर्ण *p;

  यदि (cgroup_मार्गः == रिक्त || cgroup_मार्गः[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      प्रत्यागम;
    }

  n = strchr (cgroup_मार्गः, ':');
  यदि (n == रिक्त)
    xasprintf (scope, "%s.scope", cgroup_मार्गः);
  अन्यथा
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      यदि (p)
        *p = '-';
    }
  यदि (खण्डिका)
    {
      *खण्डिका = xstrdup (cgroup_मार्गः);
      p = strchr (*खण्डिका, ':');
      यदि (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      यदि (is_रिक्तानि_string (*खण्डिका))
        {
          मुक्त (*खण्डिका);
          यदि (user_खण्डिका)
            *खण्डिका = xstrdup ("user.slice");
          अन्यथा
            *खण्डिका = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
स्थिर पूर्णाङ्क
सज्जता_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
  cleanup_close पूर्णाङ्क dirfd = -1;
  bool need_समुच्चयः_parent = true;
  वर्ण fmt_buf[64];
  पूर्णाङ्क len;
  पूर्णाङ्क ret;

  यदि (resources == रिक्त || resources->cpu == रिक्त)
    प्रत्यागम 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  यदि (is_रिक्तानि_string (मार्गः))
    प्रत्यागम crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  यदि (has_suffix (मार्गः, ".scope"))
    need_समुच्चयः_parent = false;

  ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, "cpu", मार्गः, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = crun_ensure_directory (cgroup_मार्गः, 0755, true, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dirfd = उद्घाटय (cgroup_मार्गः, O_DIRECTORY | O_मार्गः | O_CLOEXEC);
  यदि (UNLIKELY (dirfd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_मार्गः);

  यदि (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      यदि (need_समुच्चयः_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      यदि (need_समुच्चयः_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
सज्जता_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
  पूर्णाङ्क parent;
  पूर्णाङ्क ret;

  यदि (resources == रिक्त || resources->cpu == रिक्त)
    प्रत्यागम 0;

  यदि (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    प्रत्यागम 0;

  प्रति (parent = 0; parent < 2; parent++)
    {
      cleanup_close पूर्णाङ्क dirfd = -1;

      यदि (cgroup2)
        ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, मार्गः ? मार्गः : "", (parent ? ".." : रिक्त), रिक्त);
      अन्यथा
        ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, "/cpu", मार्गः ? मार्गः : "", (parent ? ".." : रिक्त), रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      dirfd = उद्घाटय (cgroup_मार्गः, O_DIRECTORY | O_मार्गः | O_CLOEXEC);
      यदि (UNLIKELY (dirfd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s`", cgroup_मार्गः);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      यदि (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
सज्जता_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_मार्गः = रिक्त;
  पूर्णाङ्क parent;
  पूर्णाङ्क ret;
  पूर्णाङ्क स्तराः;

  ret = append_paths (&cgroup_मार्गः, err, CGROUP_मूलम्, "/cpuset", मार्गः ? मार्गः : "", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = crun_ensure_directory (cgroup_मार्गः, 0755, true, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_मार्गः, resources, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  स्तराः = has_suffix (मार्गः, ".scope") ? 1 : 2;

  प्रति (parent = 0; parent < स्तराः; parent++)
    {
      cleanup_close पूर्णाङ्क dirfd_cpuset = -1;
      cleanup_free वर्ण *मार्गः_गन्तव्यम्_cpuset = रिक्त;

      ret = append_paths (&मार्गः_गन्तव्यम्_cpuset, err, CGROUP_मूलम् "/cpuset", मार्गः, (parent ? ".." : रिक्त), रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      dirfd_cpuset = उद्घाटय (मार्गः_गन्तव्यम्_cpuset, O_DIRECTORY | O_मार्गः | O_CLOEXEC);
      यदि (UNLIKELY (dirfd_cpuset < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s`", मार्गः_गन्तव्यम्_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
स्थिर पूर्णाङ्क
compute_finalized_मार्गः (वर्ण **मार्गः, ध्रुव वर्ण *from, ध्रुव वर्ण *suffix, libcrun_error_t *err)
{
  यदि (suffix == रिक्त)
    {
      *मार्गः = xstrdup (from);
      प्रत्यागम 0;
    }
  प्रत्यागम append_paths (मार्गः, err, from, suffix, रिक्त);
}

स्थिर पूर्णाङ्क
systemd_finalize (संरचना libcrun_cgroup_तर्काः *तर्काः, वर्ण **मार्गः_बहिर्गमनम्,
                  पूर्णाङ्क cgroup_बहुलकम्, ध्रुव वर्ण *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = तर्काः->resources;
  cleanup_free वर्ण *content = रिक्त;
  cleanup_free वर्ण *मार्गः = रिक्त;
  pid_t pid = तर्काः->pid;
  पूर्णाङ्क ret;
  वर्ण *from, *गन्तव्यम्;
  वर्ण *saveptr = रिक्त;

  cleanup_free वर्ण *proc_मार्गः = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;

  xasprintf (&proc_मार्गः, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (तर्काः->container, proc_मार्गः, O_RDONLY, err);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम fd;

  ret = read_all_fd (fd, "cgroup path", &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  विकल्प (cgroup_बहुलकम्)
    {
    पक्ष CGROUP_बहुलकम्_HYBRID:
    पक्ष CGROUP_बहुलकम्_LEGACY:
      from = वाक्ये_अन्विष (content, ":memory:");
      यदि (LIKELY (from != रिक्त))
        from += 8;
      अन्यथा
        {
          from = वाक्ये_अन्विष (content, ":pids:");
          यदि (UNLIKELY (from == रिक्त))
            प्रत्यागम crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      गन्तव्यम् = strchr (from, '\n');
      यदि (UNLIKELY (गन्तव्यम् == रिक्त))
        प्रत्यागम crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *गन्तव्यम् = '\0';
      ret = compute_finalized_मार्गः (&मार्गः, from, suffix, err);
      *गन्तव्यम् = '\n';
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      यदि (geteuid ())
        प्रत्यागम 0;

      ret = सज्जता_rt_runtime (resources, मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = सज्जता_cpuset_for_systemd_v1 (resources, मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      प्रति (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (रिक्त, "\n", &saveptr))
        {
          वर्ण *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          यदि (subsystem[0] == '\0')
            {
              यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_LEGACY)
                अनुवर्त;

              subsystem = "unified";
            }

          यदि (वाक्यं_तुलय (subpath, मार्गः))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, मार्गः, true, err);
              यदि (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  यदि (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      अनुवर्त;
                    }
                  प्रत्यागम ret;
                }
            }
        }

      भङ्ग;

    पक्ष CGROUP_बहुलकम्_UNIFIED:
      {
        cleanup_free वर्ण *dir = रिक्त;

        from = वाक्ये_अन्विष (content, "0::");
        यदि (UNLIKELY (from == रिक्त))
          प्रत्यागम crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        गन्तव्यम् = strchr (from, '\n');
        यदि (UNLIKELY (गन्तव्यम् == रिक्त))
          प्रत्यागम crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *गन्तव्यम् = '\0';
        ret = compute_finalized_मार्गः (&मार्गः, from, suffix, err);
        *गन्तव्यम् = '\n';
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        ret = append_paths (&dir, err, CGROUP_मूलम्, मार्गः, रिक्त);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        ret = move_process_गन्तव्यम्_cgroup (pid, रिक्त, मार्गः, err);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        ret = enable_controllers (मार्गः, err);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        यदि (suffix)
          {
            ret = chown_cgroups (मार्गः, तर्काः->मूलम्_uid, तर्काः->मूलम्_gid, err);
            यदि (UNLIKELY (ret < 0))
              प्रत्यागम ret;
          }
      }
      भङ्ग;

    स्वभाव:
      प्रत्यागम crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_बहुलकम्);
    }

  ret = सज्जता_missing_cpu_options_for_systemd (resources, cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED, मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  *मार्गः_बहिर्गमनम् = मार्गः;
  मार्गः = रिक्त;

  प्रत्यागम 0;
}

संरचना systemd_job_removed_s
{
  ध्रुव वर्ण *मार्गः;
  ध्रुव वर्ण *op;
  पूर्णाङ्क terminated;
  libcrun_error_t err;
};

स्थिर पूर्णाङ्क
systemd_job_removed (sd_bus_सन्देशः *m, शून्य *userdata, sd_bus_error *error arg_अप्रयुक्तम्)
{
  ध्रुव वर्ण *मार्गः, *unit, *परिणाम;
  uint32_t id;
  पूर्णाङ्क ret;
  संरचना systemd_job_removed_s *d = userdata;

  ret = sd_bus_सन्देशः_read (m, "uoss", &id, &मार्गः, &unit, &परिणाम);
  यदि (ret < 0)
    प्रत्यागम -1;

  यदि (वाक्यं_तुलय (d->मार्गः, मार्गः) == 0)
    {
      d->terminated = 1;
      यदि (वाक्यं_तुलय (परिणाम, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, परिणाम);
          प्रत्यागम -1;
        }
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
systemd_check_job_स्थितिः_सज्जता (sd_bus *bus, संरचना systemd_job_removed_s *दत्त, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = sd_bus_मेलनम्_signal_async (bus, रिक्त, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, रिक्त, दत्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, -ret, "sd-bus match signal");

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
systemd_check_job_स्थितिः (sd_bus *bus, संरचना systemd_job_removed_s *दत्त, ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *op,
                          libcrun_error_t *err)
{
  पूर्णाङ्क sd_err;

  दत्त->मार्गः = मार्गः;
  दत्त->op = op;
  यावत् (! दत्त->terminated)
    {
      sd_err = sd_bus_process (bus, रिक्त);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus process");

      यदि (sd_err != 0)
        अनुवर्त;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus wait");
    }

  यदि (दत्त->err != रिक्त)
    {
      *err = दत्त->err;
      प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
parse_sd_array (वर्ण *s, वर्ण **बहिर्गमनम्, वर्ण **अग्र, libcrun_error_t *err)
{
  वर्ण endchr;
  वर्ण *it, *dest;
  bool escaped = false;

  *बहिर्गमनम् = रिक्त;
  *अग्र = रिक्त;

  यावत् (रिक्तम्_वा (*s))
    s++;
  यदि (*s == '\0')
    प्रत्यागम 0;
  अन्यथा यदि (*s != '\'' && *s != '"')
    प्रत्यागम crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *बहिर्गमनम् = dest = it;

  यावत् (1)
    {
      यदि (*it == '\0')
        प्रत्यागम crun_make_error (err, 0, "invalid string `%s`", s);
      यदि (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          यावत् (रिक्तम्_वा (*it))
            it++;
          यदि (*it == ',')
            {
              *अग्र = ++it;
              *dest = '\0';
              प्रत्यागम 0;
            }

          यदि (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              प्रत्यागम 0;
            }

          प्रत्यागम crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      यदि (! escaped)
        *dest++ = *it;
      it++;
    }

  प्रत्यागम 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
स्थिर पूर्णाङ्क
append_systemd_annotation (sd_bus_सन्देशः *m, ध्रुव वर्ण *name, size_t name_len, ध्रुव वर्ण *मानम्,
                           libcrun_error_t *err)
{
  cleanup_free वर्ण *tmp_name = रिक्त;
  uint32_t गुणकः = 1;
  ध्रुव वर्ण *it;
  पूर्णाङ्क sd_err;

  यावत् (*मानम् == ' ')
    मानम्++;

  it = मानम्;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  यदि (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
      && name[name_len - 1] == 'c')
    {
      गुणकः = 1000000;

      tmp_name = xmalloc (name_len + 2);
      स्मृतिं_प्रतिलिख (tmp_name, name, name_len - 3);
      स्मृतिं_प्रतिलिख (tmp_name + name_len - 3, "USec", 5);

      name = tmp_name;
    }

  /* A *USec property is always an uint64, so, like runc does, accept any
     number of seconds (possibly with a fractional part, and with an
     optional integer type), and convert it to uint64 microseconds.  */
  यदि (गुणकः != 1)
    {
      स्थिर ध्रुव वर्ण *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", रिक्त };
      ध्रुव वर्ण *अङ्कः = it;
      वर्ण *endptr = रिक्त;
      द्विगुण v;
      size_t i;

      प्रति (i = 0; types[i]; i++)
        यदि (has_उपसर्गः (अङ्कः, types[i]))
          {
            अङ्कः += वाक्यदैर्घ्यम् (types[i]);
            भङ्ग;
          }

      errno = 0;
      v = strtod (अङ्कः, &endptr);
      यदि (UNLIKELY (errno != 0 || endptr == अङ्कः || *endptr || v < 0 || v * गुणकः >= 18446744073709551615.0))
        प्रत्यागम crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, मानम्);

      sd_err = sd_bus_सन्देशः_append (m, "(sv)", name, "t", (uint64_t) (v * गुणकः));
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      प्रत्यागम 0;
    }

  यदि ((वाक्यं_तुलय (it, "true") == 0) || (वाक्यं_तुलय (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_सन्देशः_append (m, "(sv)", name, "b", b);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      प्रत्यागम 0;
    }
  अन्यथा यदि (*it == '\'')
    {
      cleanup_free वर्ण *v_start = रिक्त;
      वर्ण *अन्तः;

      it = v_start = xstrdup (मानम्);

      अन्तः = strchr (v_start + 1, '\'');
      यदि (अन्तः == रिक्त)
        प्रत्यागम crun_make_error (err, 0, "invalid variant `%s`", मानम्);
      *अन्तः = '\0';

      sd_err = sd_bus_सन्देशः_append (m, "(sv)", name, "s", it + 1);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      प्रत्यागम 0;
    }
  अन्यथा यदि (*it == '[')
    {
      cleanup_free वर्ण *v_start = रिक्त;
      size_t n_parts = 0, parts_आकार = 32;
      cleanup_free वर्ण **parts = xmalloc (परिमाण (वर्ण *) * parts_आकार);
      वर्ण *part;

      part = v_start = xstrdup (it + 1);
      यावत् (1)
        {
          वर्ण *बहिर्गमनम् = रिक्त, *अग्र = रिक्त;
          पूर्णाङ्क ret;

          ret = parse_sd_array (part, &बहिर्गमनम्, &अग्र, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          parts[n_parts++] = बहिर्गमनम्;
          यदि (n_parts == parts_आकार - 1)
            {
              parts_आकार += 32;
              parts = xrealloc (parts, परिमाण (वर्ण *) * parts_आकार);
            }
          parts[n_parts] = रिक्त;
          यदि (अग्र == रिक्त)
            भङ्ग;

          part = अग्र;
        }

      sd_err = sd_bus_सन्देशः_open_container (m, 'r', "sv");
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_सन्देशः_append (m, "s", name);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_सन्देशः_open_container (m, 'v', "as");
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_सन्देशः_append_strv (m, parts);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_सन्देशः_close_container (m);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_सन्देशः_close_container (m);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus close container");

      प्रत्यागम 0;
    }
  अन्यथा यदि (has_उपसर्गः (it, "uint64 "))
    {
      वर्ण *endptr = रिक्त;
      uint64_t v;

      errno = 0;
      v = strtoull (it + परिमाण ("uint64"), &endptr, 10);
      यदि (UNLIKELY (errno != 0 || *endptr))
        प्रत्यागम crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_सन्देशः_append (m, "(sv)", name, "t", (uint64_t) (v * गुणकः));
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      प्रत्यागम 0;
    }
  अन्यथा यदि (has_उपसर्गः (it, "int64 "))
    {
      वर्ण *endptr = रिक्त;
      int64_t v;

      errno = 0;
      v = strtoll (it + परिमाण ("int64"), &endptr, 10);
      यदि (UNLIKELY (errno != 0 || *endptr))
        प्रत्यागम crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_सन्देशः_append (m, "(sv)", name, "x", (int64_t) (v * गुणकः));
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      प्रत्यागम 0;
    }
  अन्यथा यदि (has_उपसर्गः (it, "uint32 "))
    {
      वर्ण *endptr = रिक्त;
      uint32_t v;

      errno = 0;
      v = strtoul (it + परिमाण ("uint32"), &endptr, 10);
      यदि (UNLIKELY (errno != 0 || *endptr))
        प्रत्यागम crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_सन्देशः_append (m, "(sv)", name, "u", (uint32_t) (v * गुणकः));
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      प्रत्यागम 0;
    }
  अन्यथा यदि (has_उपसर्गः (it, "int32 ") || strchr (it, ' ') == रिक्त)
    {
      वर्ण *endptr = रिक्त;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      यदि (has_उपसर्गः (it, "int32 "))
        v = strtol (it + परिमाण ("int32"), &endptr, 10);
      अन्यथा
        v = strtol (it, &endptr, 10);
      यदि (UNLIKELY (errno != 0 || *endptr))
        प्रत्यागम crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_सन्देशः_append (m, "(sv)", name, "i", (int32_t) (v * गुणकः));
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      प्रत्यागम 0;
    }

  प्रत्यागम crun_make_error (err, 0, "unknown type for `%s`", name);
}

स्थिर पूर्णाङ्क
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  पूर्णाङ्क rootless;
  पूर्णाङ्क sd_err = 0;

  rootless = is_rootless (err);
  यदि (UNLIKELY (rootless < 0))
    प्रत्यागम rootless;

  यदि (rootless)
    sd_err = sd_bus_default_user (bus);
  यदि (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  यदि (sd_err < 0)
    प्रत्यागम crun_make_error (err, -sd_err, "cannot open sd-bus");

  प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ध्रुव वर्ण *name)
{
  size_t i;

  यदि (resources == रिक्त || resources->unified == रिक्त)
    प्रत्यागम रिक्त;

  प्रति (i = 0; i < resources->unified->len; i++)
    यदि (वाक्यं_तुलय (resources->unified->कुञ्चिकाः[i], name) == 0)
      प्रत्यागम resources->unified->मानानि[i];
  प्रत्यागम रिक्त;
}

स्थिर पूर्णाङ्क
get_मानम्_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ध्रुव वर्ण *name,
                            uint64_t *मानम्, libcrun_error_t *err)
{
  वर्ण *endptr = रिक्त;
  size_t i;

  यदि (resources == रिक्त || resources->unified == रिक्त)
    प्रत्यागम 0;

  प्रति (i = 0; i < resources->unified->len; i++)
    यदि (वाक्यं_तुलय (resources->unified->कुञ्चिकाः[i], name) == 0)
      {
        यदि (वाक्यं_तुलय (resources->unified->मानानि[i], "max") == 0)
          {
            *मानम् = UINT64_MAX;
            प्रत्यागम 1;
          }

        errno = 0;
        *मानम् = strtoull (resources->unified->मानानि[i], &endptr, 10);
        यदि (UNLIKELY (errno))
          प्रत्यागम crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->मानानि[i]);
        यदि (endptr && *endptr)
          प्रत्यागम crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->मानानि[i]);
        प्रत्यागम 1;
      }
  प्रत्यागम 0;
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_स्मृतिः_नीचः (runtime_spec_schema_config_linux_resources *resources, uint64_t *सीमा, libcrun_error_t *err)
{
  यदि (resources->स्मृतिः && resources->स्मृतिः->reservation_present)
    {
      *सीमा = resources->स्मृतिः->reservation;
      प्रत्यागम 1;
    }

  प्रत्यागम get_मानम्_from_unified_map (resources, "memory.low", सीमा, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *सीमा, libcrun_error_t *err)
{
  यदि (resources->pids && resources->pids->सीमा)
    {
      *सीमा = resources->pids->सीमा;
      प्रत्यागम 1;
    }

  प्रत्यागम get_मानम्_from_unified_map (resources, "pids.max", सीमा, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_स्मृतिः_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *सीमा, libcrun_error_t *err)
{
  यदि (resources->स्मृतिः && resources->स्मृतिः->सीमा_present)
    {
      *सीमा = resources->स्मृतिः->सीमा;
      प्रत्यागम 1;
    }

  प्रत्यागम get_मानम्_from_unified_map (resources, "memory.max", सीमा, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_स्मृतिः_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *सीमा, libcrun_error_t *err)
{
  यदि (resources->स्मृतिः && resources->स्मृतिः->swap_present)
    {
      runtime_spec_schema_config_linux_resources_स्मृतिः *स्मृतिः = resources->स्मृतिः;
      int64_t mem = स्मृतिः->सीमा_present ? स्मृतिः->सीमा : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      यदि (स्मृतिः->swap == -1 || (mem == -1 && स्मृतिः->swap == 0))
        {
          *सीमा = UINT64_MAX;
          प्रत्यागम 1;
        }
      /* 0 means unset.  */
      यदि (स्मृतिः->swap == 0)
        प्रत्यागम 0;
      यदि (mem <= 0)
        प्रत्यागम crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      यदि (स्मृतिः->swap < mem)
        प्रत्यागम crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *सीमा = स्मृतिः->swap - mem;
      प्रत्यागम 1;
    }

  प्रत्यागम get_मानम्_from_unified_map (resources, "memory.swap.max", सीमा, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t मानम्;
  पूर्णाङ्क ret;

  ret = get_मानम्_from_unified_map (resources, "cpu.idle", &मानम्, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  यदि (ret > 0 && मानम् == 1)
    has_idle = true;
  यदि (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_मानम्_from_unified_map (resources, "cpu.weight", weight, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  यदि (ret > 0)
    {
      यदि (has_idle)
        प्रत्यागम crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      प्रत्यागम 1;
    }

  यदि (resources->cpu && resources->cpu->shares_present)
    {
      यदि (has_idle)
        प्रत्यागम crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      यदि (resources->cpu->shares == 0)
        प्रत्यागम 0;
      *weight = convert_shares_गन्तव्यम्_weight (resources->cpu->shares);
      प्रत्यागम 1;
    }
  यदि (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      प्रत्यागम 1;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
read_uint64_मानम् (ध्रुव वर्ण *str, uint64_t *मानम्, libcrun_error_t *err)
{
  वर्ण *endptr = रिक्त;

  errno = 0;
  *मानम् = strtoull (str, &endptr, 10);
  यदि (UNLIKELY (errno))
    प्रत्यागम crun_make_error (err, errno, "invalid value: %s", str);
  यदि (endptr && *endptr)
    प्रत्यागम crun_make_error (err, 0, "invalid value: %s", str);
  प्रत्यागम 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
स्थिर अन्तःस्थ uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  प्रत्यागम bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

स्थिर पूर्णाङ्क
append_io_weight (sd_bus_सन्देशः *m, वर्ण **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  पूर्णाङ्क sd_err;
  size_t i;
  पूर्णाङ्क ret;

#  define APPEND_IO_WEIGHT(मानम्)                                                      \
    कुरु                                                                                 \
      {                                                                                \
        यदि (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_सन्देशः_append (m, "(sv)", "IOWeight", "t", weight);       \
            यदि (UNLIKELY (sd_err < 0))                                                 \
              प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } यावत् (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, मानम्)                                                      \
    कुरु                                                                                                \
      {                                                                                               \
        यदि (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_सन्देशः_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            यदि (UNLIKELY (sd_err < 0))                                                                \
              प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } यावत् (0)

  यदि (resources->खण्डम्_io && resources->खण्डम्_io->weight_present)
    {
      weight = IO_WEIGHT (resources->खण्डम्_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  प्रति (i = 0; resources->खण्डम्_io && i < resources->खण्डम्_io->weight_device_len; i++)
    {
      वर्ण name[64];
      पूर्णाङ्क len;

      len = snprintf (name, परिमाण (name), "%" PRIu64 ":%" PRIu64,
                      resources->खण्डम्_io->weight_device[i]->major,
                      resources->खण्डम्_io->weight_device[i]->minor);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (name)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->खण्डम्_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  प्रति (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free वर्ण *मानम् = रिक्त;
      bool need_conversion = false;
      वर्ण *saveptr = रिक्त;
      वर्ण *पङ्क्तिः;

      यदि (वाक्यं_तुलय (resources->unified->कुञ्चिकाः[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      अन्यथा यदि (वाक्यं_तुलय (resources->unified->कुञ्चिकाः[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      अन्यथा
        {
          /* Skip other keys.  */
          अनुवर्त;
        }

      मानम् = xstrdup (resources->unified->मानानि[i]);

      प्रति (पङ्क्तिः = strtok_r (मानम्, "\n", &saveptr); पङ्क्तिः; पङ्क्तिः = strtok_r (रिक्त, "\n", &saveptr))
        {
          वर्ण *sep;

          sep = strchr (पङ्क्तिः, ' ');
          यावत् (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_मानम् (sep ? sep : पङ्क्तिः, &weight, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          यदि (need_conversion)
            weight = IO_WEIGHT (weight);

          यदि (sep == रिक्त || वाक्यं_तुलय (पङ्क्तिः, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          अन्यथा
            APPEND_IO_DEVICE_WEIGHT (पङ्क्तिः, weight);
        }
    }

  प्रत्यागम 0;
}

/* Adapted from systemd.  */
स्थिर पूर्णाङ्क
bus_append_byte_array (sd_bus_सन्देशः *m, ध्रुव वर्ण *क्षेत्रम्, ध्रुव शून्य *buf, size_t n, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = sd_bus_सन्देशः_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  यदि (ret < 0)
    प्रत्यागम crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_सन्देशः_append_basic (m, SD_BUS_TYPE_STRING, क्षेत्रम्);
  यदि (ret < 0)
    प्रत्यागम crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_सन्देशः_open_container (m, 'v', "ay");
  यदि (ret < 0)
    प्रत्यागम crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_सन्देशः_append_array (m, 'y', buf, n);
  यदि (ret < 0)
    प्रत्यागम crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_सन्देशः_close_container (m);
  यदि (ret < 0)
    प्रत्यागम crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_सन्देशः_close_container (m);
  यदि (ret < 0)
    प्रत्यागम crun_make_error (err, -ret, "sd_bus_message_close_container");

  प्रत्यागम 1;
}

स्थिर पूर्णाङ्क
append_uint64_from_unified_map (sd_bus_सन्देशः *m,
                                वर्ण **missing_properties,
                                ध्रुव वर्ण *attr,
                                ध्रुव वर्ण *कुञ्चिका,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t मानम्;
  पूर्णाङ्क ret;

  यदि (property_missing_p (missing_properties, attr))
    प्रत्यागम 0;

  ret = get_मानम्_from_unified_map (resources, कुञ्चिका, &मानम्, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  यदि (ret)
    {
      पूर्णाङ्क sd_err = sd_bus_सन्देशः_append (m, "(sv)", attr, "t", मानम्);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  प्रत्यागम 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
स्थिर पूर्णाङ्क
append_device_allow (sd_bus_सन्देशः *m,
                     ध्रुव वर्ण type,
                     पूर्णाङ्क major,
                     पूर्णाङ्क minor,
                     ध्रुव वर्ण *access,
                     libcrun_error_t *err)
{
  वर्ण device[64];
  पूर्णाङ्क sd_err;
  पूर्णाङ्क len;

  यदि (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      प्रत्यागम 0;
    }

  यदि (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, परिमाण (device), "%s-*", type == 'c' ? "char" : "block");
  अन्यथा यदि (IS_WILDCARD (minor))
    len = snprintf (device, परिमाण (device), type == 'c' ? "char-%d" : "block-%d", major);
  अन्यथा
    len = snprintf (device, परिमाण (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (device)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_सन्देशः_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  यदि (UNLIKELY (sd_err < 0))
    प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  प्रत्यागम 0;
}

#  define बहुलकम्_R 1
#  define बहुलकम्_W 2
#  define बहुलकम्_M 4

स्थिर पूर्णाङ्क
access_गन्तव्यम्_बहुलकम् (ध्रुव वर्ण *access, libcrun_error_t *err)
{
  पूर्णाङ्क बहुलकम् = 0;

  यावत् (access && *access)
    {
      पूर्णाङ्क c = *access++;

      विकल्प (c)
        {
        पक्ष 'r':
          बहुलकम् |= बहुलकम्_R;
          भङ्ग;

        पक्ष 'w':
          बहुलकम् |= बहुलकम्_W;
          भङ्ग;

        पक्ष 'm':
          बहुलकम् |= बहुलकम्_M;
          भङ्ग;

        स्वभाव:
          प्रत्यागम crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  प्रत्यागम बहुलकम्;
}

/* check if the type b is included in type a.  */
स्थिर bool
has_same_type (ध्रुव वर्ण *a, ध्रुव वर्ण *b)
{
  bool a_all = a == रिक्त || वाक्यं_तुलय (a, "a") == 0;
  यदि (b == रिक्त || वाक्यं_तुलय (b, "a") == 0)
    प्रत्यागम a_all;

  प्रत्यागम a_all || वाक्यं_तुलय (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
स्थिर पूर्णाङ्क
is_deny_नियमः_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  पूर्णाङ्क req_बहुलकम्;

#  define CMP_DEV_अङ्कः(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_बहुलकम् = access_गन्तव्यम्_बहुलकम् (devices[n]->access, err);
  यदि (UNLIKELY (req_बहुलकम् < 0))
    प्रत्यागम req_बहुलकम्;

  प्रति (i = n; i > 0; i--)
    {
      size_t current_नियमः = i - 1;
      पूर्णाङ्क बहुलकम्;

      यदि (! CMP_DEV_अङ्कः (devices[n]->minor, devices[current_नियमः]->minor))
        अनुवर्त;
      यदि (! CMP_DEV_अङ्कः (devices[n]->major, devices[current_नियमः]->major))
        अनुवर्त;

      यदि (! has_same_type (devices[n]->type, devices[current_नियमः]->type)
          && has_same_type ("a", devices[current_नियमः]->type))
        अनुवर्त;

      बहुलकम् = access_गन्तव्यम्_बहुलकम् (devices[i]->access, err);
      यदि (UNLIKELY (बहुलकम् < 0))
        प्रत्यागम बहुलकम्;

      यदि (devices[current_नियमः]->allow && (बहुलकम् & req_बहुलकम्))
        प्रत्यागम 0;

      req_बहुलकम् &= ! बहुलकम्;

      /* no more access bits to validate.  */
      यदि (req_बहुलकम् == 0)
        प्रत्यागम 1;
    }

  प्रत्यागम 0;
#  undef CMP_DEV_अङ्कः
}

स्थिर size_t
find_first_नियमः_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  यदि (n == 0)
    प्रत्यागम 1;

  /* Find the first rule that is after the last "block all".  */
  प्रति (i = n - 1; i-- > 0;)
    {
      यदि ((is_रिक्तानि_string (devices[i]->type) || वाक्यं_तुलय (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        प्रत्यागम i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  प्रति (i = 0; i < n; i++)
    यदि (devices[i]->allow)
      प्रत्यागम i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  प्रत्यागम n + 1;
}

स्थिर bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_खण्डम् 1
#  define DEV_TYPE_CHAR 2
  पूर्णाङ्क remaining = DEV_TYPE_खण्डम् | DEV_TYPE_CHAR;
  size_t i;

  प्रति (i = 0; i < n; i++)
    {
      size_t current_नियमः = n - i - 1;
      यदि (! devices[current_नियमः]->allow)
        प्रत्यागम false;

      यदि (IS_WILDCARD (devices[current_नियमः]->major) && IS_WILDCARD (devices[current_नियमः]->minor))
        {
          यदि (is_रिक्तानि_string (devices[current_नियमः]->type) || वाक्यं_तुलय (devices[current_नियमः]->type, "a") == 0)
            प्रत्यागम true;

          यदि (वाक्यं_तुलय (devices[current_नियमः]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          अन्यथा यदि (वाक्यं_तुलय (devices[current_नियमः]->type, "b") == 0)
            remaining &= ~DEV_TYPE_खण्डम्;

          यदि (remaining == 0)
            प्रत्यागम true;
        }
    }
#  undef DEV_TYPE_खण्डम्
#  undef DEV_TYPE_CHAR

  प्रत्यागम false;
}

स्थिर पूर्णाङ्क
append_devices (sd_bus_सन्देशः *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  संरचना default_dev_s *default_devices = get_default_devices ();
  पूर्णाङ्क ret, sd_err;
  size_t i;

  यदि (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_सन्देशः_append (m, "(sv)", "DevicePolicy", "s", "auto");
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_सन्देशः_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      प्रत्यागम 0;
    }

  sd_err = sd_bus_सन्देशः_append (m, "(sv)", "DevicePolicy", "s", "strict");
  यदि (UNLIKELY (sd_err < 0))
    प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  प्रति (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रति (i = find_first_नियमः_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      वर्ण type;

      यदि (! d->allow)
        {
          पूर्णाङ्क redundant;

          redundant = is_deny_नियमः_redundant (resources->devices, i, err);
          यदि (UNLIKELY (redundant < 0))
            प्रत्यागम redundant;

          यदि (redundant)
            अनुवर्त;

          प्रत्यागम crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      यदि (d->type == रिक्त || वाक्यं_तुलय (d->type, "a") == 0)
        type = 'a';
      अन्यथा यदि (वाक्यं_तुलय (d->type, "c") == 0)
        type = 'c';
      अन्यथा यदि (वाक्यं_तुलय (d->type, "b") == 0)
        type = 'b';
      अन्यथा
        प्रत्यागम crun_make_error (err, 0, "unknown device type `%s`", d->type);

      यदि (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      अन्यथा
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }

  प्रत्यागम 0;
}

स्थिर bool
has_bpf_fs ()
{
  संरचना statfs stat;

  प्रत्यागम statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

स्थिर पूर्णाङ्क
bpfprog_मार्गः_from_scope (वर्ण **मार्गः, ध्रुव वर्ण *scope, libcrun_error_t *err)
{
  cleanup_free वर्ण *flat_scope = xstrdup (scope);
  वर्ण *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  यावत् ((it = strchr (it, '.')) != रिक्त)
    *it = '_';

  प्रत्यागम append_paths (मार्गः, err, CRUN_BPF_DIR, flat_scope, रिक्त);
}

स्थिर पूर्णाङ्क
सङ्कलय_bpf_क्रमादेशः (sd_bus_सन्देशः *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 वर्ण **missing_properties,
                 ध्रुव वर्ण *scope,
                 bool *devices_समुच्चयः,
                 libcrun_error_t *err)
{
  पूर्णाङ्क ret = 0;
  पूर्णाङ्क अन्तर्_userns, sd_err;
  ध्रुव वर्ण *property_name = "BPFProgram";
  cleanup_free संरचना bpf_क्रमादेशः *क्रमादेशः = रिक्त;
  cleanup_free वर्ण *मार्गः = रिक्त;

  *devices_समुच्चयः = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  यदि (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    प्रत्यागम 0;

  अन्तर्_userns = check_running_अन्तर्_user_namespace (err);
  यदि (UNLIKELY (अन्तर्_userns < 0))
    प्रत्यागम अन्तर्_userns;

  यदि (अन्तर्_userns)
    प्रत्यागम 0;

  क्रमादेशः = create_dev_bpf (resources->devices, resources->devices_len, err);
  यदि (UNLIKELY (क्रमादेशः == रिक्त))
    {
      crun_error_release (err);
      प्रत्यागम 0;
    }

  ret = bpfprog_मार्गः_from_scope (&मार्गः, scope, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  यदि (UNLIKELY (ret < 0 && errno != EEXIST))
    प्रत्यागम crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  यदि (is_update)
    {
      cleanup_free संरचना bpf_क्रमादेशः *क्रमादेशः_loaded = रिक्त;

      यदि (resources->devices == रिक्त || resources->devices_len == 0)
        {
          *devices_समुच्चयः = true;
          प्रत्यागम 0;
        }

      ret = libcrun_ebpf_read_क्रमादेशः (&क्रमादेशः_loaded, मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      /* it is the same as the loaded program.  */
      यदि (libcrun_ebpf_cmp_programs (क्रमादेशः, क्रमादेशः_loaded))
        {
          *devices_समुच्चयः = true;
          प्रत्यागम 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free संरचना bpf_क्रमादेशः *क्रमादेशः_new = रिक्त;
        वर्ण tmp[64];
        पूर्णाङ्क len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, परिमाण (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (tmp)))
          प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_आरोपय (क्रमादेशः, -1, tmp, err);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        ret = libcrun_ebpf_read_क्रमादेशः (&क्रमादेशः_new, tmp, err);
        unlink (tmp);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        यदि (libcrun_ebpf_cmp_programs (क्रमादेशः_new, क्रमादेशः_loaded))
          {
            *devices_समुच्चयः = true;
            प्रत्यागम 0;
          }
      }

      प्रत्यागम crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_आरोपय (क्रमादेशः, -1, मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  sd_err = sd_bus_सन्देशः_append (m, "(sv)", property_name, "a(ss)", 1, "device", मार्गः);
  यदि (UNLIKELY (sd_err < 0))
    प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_समुच्चयः = true;

  प्रत्यागम 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
स्थिर पूर्णाङ्क
append_cpu_quota (sd_bus_सन्देशः *m, वर्ण **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ध्रुव वर्ण *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  पूर्णाङ्क sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  यदि (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      पूर्णाङ्क ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      यदि (has_period)
        period = max_period;
    }

  यदि (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_सन्देशः_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  यदि ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_प्रत्येकम्_sec = UINT64_MAX;

      यदि (quota > 0)
        {
          quota_प्रत्येकम्_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          यदि (quota_प्रत्येकम्_sec % 10000)
            quota_प्रत्येकम्_sec = ((quota_प्रत्येकम्_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_सन्देशः_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_प्रत्येकम्_sec);
      यदि (UNLIKELY (sd_err < 0))
        प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
append_resources (sd_bus_सन्देशः *m,
                  bool is_update,
                  ध्रुव वर्ण *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  पूर्णाङ्क cgroup_बहुलकम्,
                  ध्रुव वर्ण *scope,
                  bool *devices_समुच्चयः,
                  libcrun_error_t *err)
{
  uint64_t मानम्;
  पूर्णाङ्क sd_err;
  पूर्णाङ्क ret;
  cleanup_free वर्ण *dir = रिक्त;
  cleanup_free वर्ण **missing_properties = रिक्त;

  *devices_समुच्चयः = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  missing_properties = read_dir_entries (dir, err);
  यदि (UNLIKELY (missing_properties == रिक्त))
    {
      यदि (crun_error_get_errno (err) != ENOENT)
        प्रत्यागम -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_मानम्(name, मानम्)                                             \
    कुरु                                                                                 \
      {                                                                                \
        यदि (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_सन्देशः_append (m, "(sv)", name, "t", मानम्);              \
            यदि (UNLIKELY (sd_err < 0))                                                 \
              प्रत्यागम crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } यावत् (0)

#  define APPEND_UINT64(name, fn)              \
    कुरु                                         \
      {                                        \
        ret = fn (resources, &मानम्, err);     \
        यदि (UNLIKELY (ret < 0))                \
          प्रत्यागम ret;                          \
        यदि (ret)                               \
          {                                    \
            APPEND_UINT64_मानम् (name, मानम्); \
          }                                    \
    } यावत् (0)

  यदि (resources == रिक्त)
    प्रत्यागम 0;

  APPEND_UINT64 ("MemoryLow", get_स्मृतिः_नीचः);
  APPEND_UINT64 ("MemoryMax", get_स्मृतिः_max);
  APPEND_UINT64 ("MemorySwapMax", get_स्मृतिः_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  विकल्प (cgroup_बहुलकम्)
    {
    पक्ष CGROUP_बहुलकम्_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;

        यदि (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ध्रुव वर्ण *property_name = "AllowedCPUs";
            ध्रुव वर्ण *मानम् = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free वर्ण *allowed_cpus = रिक्त;
            size_t allowed_cpus_len = 0;

            यदि (मानम् == रिक्त)
              मानम् = resources->cpu->cpus;

            ret = cpuset_string_गन्तव्यम्_bitmask (मानम्, &allowed_cpus, &allowed_cpus_len, err);
            यदि (UNLIKELY (ret < 0))
              प्रत्यागम ret;

            यदि (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                यदि (UNLIKELY (ret < 0))
                  प्रत्यागम ret;
              }
          }

        यदि (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ध्रुव वर्ण *property_name = "AllowedMemoryNodes";
            ध्रुव वर्ण *मानम् = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free वर्ण *allowed_mems = रिक्त;
            size_t allowed_mems_len = 0;

            यदि (मानम् == रिक्त)
              मानम् = resources->cpu->mems;

            ret = cpuset_string_गन्तव्यम्_bitmask (मानम्, &allowed_mems, &allowed_mems_len, err);
            यदि (UNLIKELY (ret < 0))
              प्रत्यागम ret;

            यदि (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                यदि (UNLIKELY (ret < 0))
                  प्रत्यागम ret;
              }
          }

        ret = सङ्कलय_bpf_क्रमादेशः (m, is_update, resources, missing_properties, scope, devices_समुच्चयः, err);
        यदि (UNLIKELY (ret < 0))
          प्रत्यागम ret;
      }
      भङ्ग;

    पक्ष CGROUP_बहुलकम्_LEGACY:
    पक्ष CGROUP_बहुलकम्_HYBRID:
      यदि (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_मानम् ("CPUShares", resources->cpu->shares);
      भङ्ग;

    स्वभाव:
      प्रत्यागम crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_बहुलकम्);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_मानम्

  यदि (! *devices_समुच्चयः && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
reset_failed_unit (sd_bus *bus, ध्रुव वर्ण *unit)
{
  पूर्णाङ्क sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_सन्देशः *m = रिक्त, *प्रत्युत्तरम् = रिक्त;

  sd_err = sd_bus_सन्देशः_new_विधानम्_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  यदि (UNLIKELY (sd_err < 0))
    गच्छ निर्गच्छ;

  sd_err = sd_bus_सन्देशः_append (m, "s", unit);
  यदि (UNLIKELY (sd_err < 0))
    गच्छ निर्गच्छ;

  sd_err = sd_bus_call (bus, m, 0, &error, &प्रत्युत्तरम्);
  यदि (UNLIKELY (sd_err < 0))
    गच्छ निर्गच्छ;

  sd_err = 0;

निर्गच्छ:
  यदि (m)
    sd_bus_सन्देशः_unref (m);
  यदि (प्रत्युत्तरम्)
    sd_bus_सन्देशः_unref (प्रत्युत्तरम्);
  sd_bus_error_free (&error);

  प्रत्यागम sd_err;
}

स्थिर पूर्णाङ्क
verify_ebpf_device_filter_installed (ध्रुव वर्ण *cgroup_मार्गः, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = रिक्त;
  cleanup_free वर्ण *पूर्णम्_मार्गः = रिक्त;
  size_t n_progs = 0;
  पूर्णाङ्क ret;

  ret = append_paths (&पूर्णम्_मार्गः, err, CGROUP_मूलम्, cgroup_मार्गः, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_ebpf_query_cgroup_progs (पूर्णम्_मार्गः, &progs, &n_progs, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (n_progs == 0)
    प्रत्यागम crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", पूर्णम्_मार्गः);

  प्रत्यागम 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
स्थिर पूर्णाङ्क
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_सन्देशः *m, sd_bus_सन्देशः *प्रत्युत्तरम्,
                           sd_bus_error *error, पूर्णाङ्क ret)
{
  यदि (bus)
    sd_bus_unref (bus);
  यदि (m)
    sd_bus_सन्देशः_unref (m);
  यदि (प्रत्युत्तरम्)
    sd_bus_सन्देशः_unref (प्रत्युत्तरम्);
  sd_bus_error_free (error);
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            पूर्णाङ्क cgroup_बहुलकम्,
                            string_map *annotations,
                            ध्रुव वर्ण *state_मूलम्,
                            ध्रुव वर्ण *scope, ध्रुव वर्ण *खण्डिका,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_समुच्चयः,
                            libcrun_error_t *err)
{
  sd_bus *bus = रिक्त;
  sd_bus_सन्देशः *m = रिक्त;
  sd_bus_सन्देशः *प्रत्युत्तरम् = रिक्त;
  पूर्णाङ्क sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ध्रुव वर्ण *object = रिक्त;
  संरचना systemd_job_removed_s job_दत्त = {};
  पूर्णाङ्क i;
  ध्रुव वर्ण *boolean_opts[10];
  cleanup_free वर्ण *state_dir = रिक्त;

  *can_retry = false;
  *devices_समुच्चयः = false;

  ret = libcrun_get_state_directory (&state_dir, state_मूलम्, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = रिक्त;

  ret = open_sd_bus_connection (&bus, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निर्गच्छ;

  ret = systemd_check_job_स्थितिः_सज्जता (bus, &job_दत्त, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निर्गच्छ;

  sd_err = sd_bus_सन्देशः_new_विधानम्_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      गच्छ निर्गच्छ;
    }

  sd_err = sd_bus_सन्देशः_append (m, "ss", scope, "fail");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      गच्छ निर्गच्छ;
    }

  sd_err = sd_bus_सन्देशः_open_container (m, 'a', "(sv)");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      गच्छ निर्गच्छ;
    }

  यदि (खण्डिका)
    {
      sd_err = sd_bus_सन्देशः_append (m, "(sv)", "Slice", "s", खण्डिका);
      यदि (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          गच्छ निर्गच्छ;
        }
    }

  sd_err = sd_bus_सन्देशः_append (m, "(sv)", "DefaultDependencies", "b", 0);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      गच्छ निर्गच्छ;
    }

  यदि (annotations)
    {
      size_t उपसर्गः_len = परिमाण (SYSTEMD_PROPERTY_उपसर्गः) - 1;
      size_t annotations_len = string_map_आकार (annotations);
      size_t i;

      प्रति (i = 0; i < annotations_len; i++)
        {
          ध्रुव वर्ण *कुञ्चिका;
          ध्रुव वर्ण *मानम्;
          size_t len;

          ret = string_map_get_at (annotations, i, &कुञ्चिका, &मानम्);
          यदि (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              गच्छ निर्गच्छ;
            }

          यदि (! has_उपसर्गः (कुञ्चिका, SYSTEMD_PROPERTY_उपसर्गः))
            अनुवर्त;

          len = वाक्यदैर्घ्यम् (कुञ्चिका);
          यदि (len < उपसर्गः_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", कुञ्चिका);
              गच्छ निर्गच्छ;
            }

          ret = append_systemd_annotation (m, कुञ्चिका + उपसर्गः_len, len - उपसर्गः_len,
                                           मानम्, err);
          यदि (UNLIKELY (ret < 0))
            गच्छ निर्गच्छ;
        }
    }

  sd_err = sd_bus_सन्देशः_append (m, "(sv)", "Description", "s", "libcrun container");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      गच्छ निर्गच्छ;
    }

  sd_err = sd_bus_सन्देशः_append (m, "(sv)", "PIDs", "au", 1, pid);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      गच्छ निर्गच्छ;
    }

  प्रति (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_सन्देशः_append (m, "(sv)", boolean_opts[i], "b", 1);
      यदि (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          गच्छ निर्गच्छ;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_बहुलकम्, scope, devices_समुच्चयः, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निर्गच्छ;

  sd_err = sd_bus_सन्देशः_close_container (m);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      गच्छ निर्गच्छ;
    }

  sd_err = sd_bus_सन्देशः_append (m, "a(sa(sv))", 0);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      गच्छ निर्गच्छ;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &प्रत्युत्तरम्);
  यदि (UNLIKELY (sd_err < 0))
    {
      यदि (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          यदि (प्रत्युत्तरम्)
            sd_bus_सन्देशः_unref (प्रत्युत्तरम्);

          error = SD_BUS_ERROR_NULL;
          प्रत्युत्तरम् = रिक्त;

          sd_err = sd_bus_call (bus, m, 0, &error, &प्रत्युत्तरम्);
        }
      यदि (sd_err < 0)
        {
          यदि (sd_err == -EROFS)
            {
              ret = पञ्जिकाकोशः_missing_property_from_सन्देशः (state_dir, error.सन्देशः_2, err);
              यदि (UNLIKELY (ret < 0))
                गच्छ निर्गच्छ;
              यदि (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.सन्देशः_2 ?: error.name);
          गच्छ निर्गच्छ;
        }
    }

  sd_err = sd_bus_सन्देशः_read (प्रत्युत्तरम्, "o", &object);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      गच्छ निर्गच्छ;
    }

  ret = systemd_check_job_स्थितिः (bus, &job_दत्त, object, "creating", err);

निर्गच्छ:
  प्रत्यागम cleanup_sd_bus_and_return (bus, m, प्रत्युत्तरम्, &error, ret);
}

स्थिर पूर्णाङ्क
libcrun_destroy_systemd_cgroup_scope (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः,
                                      libcrun_error_t *err)
{
  sd_bus *bus = रिक्त;
  sd_bus_सन्देशः *m = रिक्त;
  sd_bus_सन्देशः *प्रत्युत्तरम् = रिक्त;
  पूर्णाङ्क ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ध्रुव वर्ण *object;
  ध्रुव वर्ण *scope = cgroup_स्थितिः->scope;
  संरचना systemd_job_removed_s job_दत्त = {};

  ret = open_sd_bus_connection (&bus, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निर्गच्छ;

  ret = systemd_check_job_स्थितिः_सज्जता (bus, &job_दत्त, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निर्गच्छ;

  ret = sd_bus_सन्देशः_new_विधानम्_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      गच्छ निर्गच्छ;
    }

  ret = sd_bus_सन्देशः_append (m, "ss", scope, "replace");
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      गच्छ निर्गच्छ;
    }

  ret = sd_bus_call (bus, m, 0, &error, &प्रत्युत्तरम्);
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.सन्देशः_2 ?: error.name);
      गच्छ निर्गच्छ;
    }

  ret = sd_bus_सन्देशः_read (प्रत्युत्तरम्, "o", &object);
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      गच्छ निर्गच्छ;
    }

  ret = systemd_check_job_स्थितिः (bus, &job_दत्त, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

निर्गच्छ:
  प्रत्यागम cleanup_sd_bus_and_return (bus, m, प्रत्युत्तरम्, &error, ret);
}

स्थिर ध्रुव वर्ण *
find_systemd_subgroup (string_map *annotations)
{
  ध्रुव वर्ण *annotation;

  annotation = find_string_map_मानम् (annotations, "run.oci.systemd.subgroup");
  यदि (annotation)
    {
      यदि (annotation[0] == '\0')
        प्रत्यागम रिक्त;
      प्रत्यागम annotation;
    }

  प्रत्यागम "container";
}

स्थिर पूर्णाङ्क
libcrun_cgroup_enter_systemd (संरचना libcrun_cgroup_तर्काः *तर्काः,
                              संरचना libcrun_cgroup_स्थितिः *बहिर्गमनम्,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = तर्काः->resources;
  ध्रुव वर्ण *cgroup_मार्गः = तर्काः->cgroup_मार्गः;
  cleanup_free वर्ण *scope_मार्गः = रिक्त;
  cleanup_free वर्ण *scope = रिक्त;
  cleanup_free वर्ण *मार्गः = रिक्त;
  cleanup_free वर्ण *खण्डिका = रिक्त;
  पूर्णाङ्क retries_वामः = 32;
  ध्रुव वर्ण *suffix;
  ध्रुव वर्ण *id = तर्काः->id;
  pid_t pid = तर्काः->pid;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क rootless = 0;
  पूर्णाङ्क ret;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
    {
      rootless = is_rootless (err);
      यदि (UNLIKELY (rootless < 0))
        प्रत्यागम rootless;
    }

  get_systemd_scope_and_खण्डिका (id, rootless == 1, cgroup_मार्गः, &scope, &खण्डिका);

  प्रति (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_बहुलकम्, तर्काः->annotations, तर्काः->state_मूलम्,
                                        scope, खण्डिका, pid, &can_retry, &बहिर्गमनम्->bpf_dev_समुच्चयः, err);
      यदि (LIKELY (ret >= 0))
        भङ्ग;

      यदि (can_retry && retries_वामः-- > 0)
        {
          crun_error_release (err);
          अनुवर्त;
        }

      प्रत्यागम ret;
    }

  suffix = find_systemd_subgroup (तर्काः->annotations);

  ret = systemd_finalize (तर्काः, &मार्गः, cgroup_बहुलकम्, suffix, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  यदि (is_रिक्तानि_string (suffix))
    scope_मार्गः = xstrdup (मार्गः);
  अन्यथा
    {
      size_t मार्गः_len = वाक्यदैर्घ्यम् (मार्गः);
      size_t suffix_len = वाक्यदैर्घ्यम् (suffix);

      scope_मार्गः = strndup (मार्गः, मार्गः_len - suffix_len - 1);
      यदि (UNLIKELY (scope_मार्गः == रिक्त))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  यदि (बहिर्गमनम्->bpf_dev_समुच्चयः)
    {
      ret = verify_ebpf_device_filter_installed (scope_मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  बहिर्गमनम्->मार्गः = मार्गः;
  मार्गः = रिक्त;

  बहिर्गमनम्->scope = scope;
  scope = रिक्त;
  प्रत्यागम 0;
}

वर्ण *
get_cgroup_scope_मार्गः (ध्रुव वर्ण *cgroup_मार्गः, ध्रुव वर्ण *scope)
{
  वर्ण *मार्गः_गन्तव्यम्_scope = रिक्त;
  वर्ण *cur;

  यदि (is_रिक्तानि_string (cgroup_मार्गः))
    प्रत्यागम रिक्त;

  मार्गः_गन्तव्यम्_scope = xstrdup (cgroup_मार्गः);

  cur = strchr (मार्गः_गन्तव्यम्_scope, '/');
  यावत् (cur)
    {
      वर्ण *अग्र = strchr (cur + 1, '/');
      यदि (अग्र == रिक्त)
        भङ्ग;

      *अग्र = '\0';
      यदि (वाक्यं_तुलय (cur, scope) == 0)
        प्रत्यागम मार्गः_गन्तव्यम्_scope;
      *अग्र = '/';

      cur = अग्र;
      यावत् (*cur == '/')
        cur++;
    }

  प्रत्यागम मार्गः_गन्तव्यम्_scope;
}

स्थिर पूर्णाङ्क
libcrun_destroy_cgroup_systemd (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः,
                                libcrun_error_t *err)
{
  cleanup_free वर्ण *मार्गः_गन्तव्यम्_scope = रिक्त;
  cleanup_free वर्ण *bpfprog = रिक्त;
  पूर्णाङ्क बहुलकम्;
  पूर्णाङ्क ret;

  बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (बहुलकम् < 0))
    प्रत्यागम बहुलकम्;

  ret = cgroup_killall_मार्गः (cgroup_स्थितिः->मार्गः, SIGKILL, err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_मार्गः_from_scope (&bpfprog, cgroup_स्थितिः->scope, err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (err);
  अन्यथा
    unlink (bpfprog); // Best effort.

  मार्गः_गन्तव्यम्_scope = get_cgroup_scope_मार्गः (cgroup_स्थितिः->मार्गः, cgroup_स्थितिः->scope);

  प्रत्यागम destroy_cgroup_मार्गः (मार्गः_गन्तव्यम्_scope, बहुलकम्, err);
}

स्थिर पूर्णाङ्क
libcrun_update_resources_systemd (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः,
                                  ध्रुव वर्ण *state_मूलम्,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  संरचना systemd_job_removed_s job_दत्त = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free वर्ण *state_dir = रिक्त;
  sd_bus_सन्देशः *प्रत्युत्तरम् = रिक्त;
  sd_bus_सन्देशः *m = रिक्त;
  sd_bus *bus = रिक्त;
  पूर्णाङ्क sd_err, ret;
  पूर्णाङ्क cgroup_बहुलकम्;

  ret = libcrun_get_state_directory (&state_dir, state_मूलम्, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  ret = open_sd_bus_connection (&bus, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = systemd_check_job_स्थितिः_सज्जता (bus, &job_दत्त, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निर्गच्छ;

  sd_err = sd_bus_सन्देशः_new_विधानम्_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      गच्छ निर्गच्छ;
    }

  sd_err = sd_bus_सन्देशः_append (m, "sb", cgroup_स्थितिः->scope, 1);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      गच्छ निर्गच्छ;
    }

  sd_err = sd_bus_सन्देशः_open_container (m, 'a', "(sv)");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      गच्छ निर्गच्छ;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_बहुलकम्, cgroup_स्थितिः->scope, &cgroup_स्थितिः->bpf_dev_समुच्चयः, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निर्गच्छ;

  sd_err = sd_bus_सन्देशः_close_container (m);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      गच्छ निर्गच्छ;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &प्रत्युत्तरम्);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.सन्देशः_2 ?: error.name);
      गच्छ निर्गच्छ;
    }

  यदि (cgroup_बहुलकम् != CGROUP_बहुलकम्_UNIFIED)
    {
      ret = सज्जता_rt_runtime (resources, cgroup_स्थितिः->मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ निर्गच्छ;

      ret = सज्जता_cpuset_for_systemd_v1 (resources, cgroup_स्थितिः->मार्गः, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ निर्गच्छ;
    }

  ret = सज्जता_missing_cpu_options_for_systemd (resources, cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED, cgroup_स्थितिः->मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निर्गच्छ;

  ret = 0;

निर्गच्छ:
  प्रत्यागम cleanup_sd_bus_and_return (bus, m, प्रत्युत्तरम्, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
स्थिर पूर्णाङ्क
libcrun_attach_process_systemd (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः,
                                ध्रुव वर्ण *मार्गः, pid_t pid,
                                libcrun_error_t *err)
{
  ध्रुव वर्ण *scope = cgroup_स्थितिः->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free वर्ण *scope_मार्गः = रिक्त;
  sd_bus_सन्देशः *प्रत्युत्तरम् = रिक्त;
  ध्रुव वर्ण *subcgroup;
  ध्रुव वर्ण *चरमः;
  sd_bus *bus = रिक्त;
  size_t len;
  पूर्णाङ्क sd_err, ret;

  यदि (is_रिक्तानि_string (scope))
    प्रत्यागम crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_मार्गः = get_cgroup_scope_मार्गः (मार्गः, scope);
  यदि (scope_मार्गः == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  चरमः = strrchr (scope_मार्गः, '/');
  यदि (चरमः == रिक्त || वाक्यं_तुलय (चरमः + 1, scope) != 0)
    प्रत्यागम crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, मार्गः);
  len = वाक्यदैर्घ्यम् (scope_मार्गः);
  subcgroup = मार्गः[len] ? मार्गः + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  sd_err = sd_bus_call_विधानम् (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &प्रत्युत्तरम्, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  यदि (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.सन्देशः_2 ?: error.name);

  प्रत्यागम cleanup_sd_bus_and_return (bus, रिक्त, प्रत्युत्तरम्, &error, ret);
}

#अन्यथा
स्थिर पूर्णाङ्क
libcrun_cgroup_enter_systemd (संरचना libcrun_cgroup_तर्काः *तर्काः,
                              संरचना libcrun_cgroup_स्थितिः *बहिर्गमनम्,
                              libcrun_error_t *err)
{
  (शून्य) तर्काः;
  (शून्य) बहिर्गमनम्;

  प्रत्यागम crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णाङ्क
libcrun_destroy_cgroup_systemd (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः,
                                libcrun_error_t *err)
{
  (शून्य) cgroup_स्थितिः;

  प्रत्यागम crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णाङ्क
libcrun_update_resources_systemd (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः,
                                  ध्रुव वर्ण *state_मूलम्,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (शून्य) cgroup_स्थितिः;
  (शून्य) state_मूलम्;
  (शून्य) resources;

  प्रत्यागम crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णाङ्क
libcrun_attach_process_systemd (संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः,
                                ध्रुव वर्ण *मार्गः, pid_t pid,
                                libcrun_error_t *err)
{
  (शून्य) cgroup_स्थितिः;
  (शून्य) मार्गः;
  (शून्य) pid;

  प्रत्यागम crun_make_error (err, ENOTSUP, "systemd not supported");
}
#यदि_अन्तः

संरचना libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = रिक्त,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
