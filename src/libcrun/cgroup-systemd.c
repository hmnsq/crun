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

स्थिर पूर्णाङ्क
register_missing_property_from_message (ध्रुव वर्ण *state_dir, ध्रुव वर्ण *message, libcrun_error_t *err)
{
  cleanup_free वर्ण *file_path = रिक्त;
  cleanup_free वर्ण *dir_path = रिक्त;
  cleanup_free वर्ण *property = रिक्त;
  वर्ण *p;
  पूर्णाङ्क ret;

  सचे (! has_prefix (message, "Cannot set property "))
    पटिनिवत्त 0;

  property = xstrdup (message + परिमाण ("Cannot set property ") - 1);
  p = strchr (property, ',');
  सचे (! p)
    पटिनिवत्त 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = append_paths (&file_path, err, dir_path, property, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, रिक्त, 0, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 1;
}

स्थिर bool
property_missing_p (वर्ण **missing_properties, ध्रुव वर्ण *property)
{
  size_t i;

  अत्थाय (i = 0; missing_properties && missing_properties[i]; i++)
    सचे (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        पटिनिवत्त true;
      }

  पटिनिवत्त false;
}

स्थिर शून्य
get_systemd_scope_and_slice (ध्रुव वर्ण *id, bool user_slice, ध्रुव वर्ण *cgroup_path,
                             वर्ण **scope, वर्ण **slice)
{
  ध्रुव वर्ण *n;
  वर्ण *p;

  सचे (cgroup_path == रिक्त || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      पटिनिवत्त;
    }

  n = strchr (cgroup_path, ':');
  सचे (n == रिक्त)
    xasprintf (scope, "%s.scope", cgroup_path);
  अञ्ञथा
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      सचे (p)
        *p = '-';
    }
  सचे (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      सचे (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      सचे (is_empty_string (*slice))
        {
          मुक्त (*slice);
          सचे (user_slice)
            *slice = xstrdup ("user.slice");
          अञ्ञथा
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
स्थिर पूर्णाङ्क
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ध्रुव वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path = रिक्त;
  cleanup_close पूर्णाङ्क dirfd = -1;
  bool need_set_parent = true;
  वर्ण fmt_buf[64];
  पूर्णाङ्क len;
  पूर्णाङ्क ret;

  सचे (resources == रिक्त || resources->cpu == रिक्त)
    पटिनिवत्त 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  सचे (is_empty_string (path))
    पटिनिवत्त crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  सचे (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_मूलम्, "cpu", path, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  सचे (UNLIKELY (dirfd < 0))
    पटिनिवत्त crun_make_error (err, errno, "open `%s`", cgroup_path);

  सचे (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      सचे (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");

      सचे (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      सचे (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");

      सचे (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ध्रुव वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path = रिक्त;
  पूर्णाङ्क parent;
  पूर्णाङ्क ret;

  सचे (resources == रिक्त || resources->cpu == रिक्त)
    पटिनिवत्त 0;

  सचे (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    पटिनिवत्त 0;

  अत्थाय (parent = 0; parent < 2; parent++)
    {
      cleanup_close पूर्णाङ्क dirfd = -1;

      सचे (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_मूलम्, path ? path : "", (parent ? ".." : रिक्त), रिक्त);
      अञ्ञथा
        ret = append_paths (&cgroup_path, err, CGROUP_मूलम्, "/cpu", path ? path : "", (parent ? ".." : रिक्त), रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      सचे (UNLIKELY (dirfd < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      सचे (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
    }

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ध्रुव वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path = रिक्त;
  पूर्णाङ्क parent;
  पूर्णाङ्क ret;
  पूर्णाङ्क levels;

  ret = append_paths (&cgroup_path, err, CGROUP_मूलम्, "/cpuset", path ? path : "", रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  अत्थाय (parent = 0; parent < levels; parent++)
    {
      cleanup_close पूर्णाङ्क dirfd_cpuset = -1;
      cleanup_free वर्ण *path_to_cpuset = रिक्त;

      ret = append_paths (&path_to_cpuset, err, CGROUP_मूलम् "/cpuset", path, (parent ? ".." : रिक्त), रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      सचे (UNLIKELY (dirfd_cpuset < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
स्थिर पूर्णाङ्क
compute_finalized_path (वर्ण **path, ध्रुव वर्ण *from, ध्रुव वर्ण *suffix, libcrun_error_t *err)
{
  सचे (suffix == रिक्त)
    {
      *path = xstrdup (from);
      पटिनिवत्त 0;
    }
  पटिनिवत्त append_paths (path, err, from, suffix, रिक्त);
}

स्थिर पूर्णाङ्क
systemd_finalize (संरचना libcrun_cgroup_args *args, वर्ण **path_out,
                  पूर्णाङ्क cgroup_mode, ध्रुव वर्ण *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free वर्ण *content = रिक्त;
  cleanup_free वर्ण *path = रिक्त;
  pid_t pid = args->pid;
  पूर्णाङ्क ret;
  वर्ण *from, *to;
  वर्ण *saveptr = रिक्त;

  cleanup_free वर्ण *proc_path = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  सचे (UNLIKELY (fd < 0))
    पटिनिवत्त fd;

  ret = read_all_fd (fd, "cgroup path", &content, रिक्त, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  विकल्प (cgroup_mode)
    {
    पक्ष CGROUP_MODE_HYBRID:
    पक्ष CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      सचे (LIKELY (from != रिक्त))
        from += 8;
      अञ्ञथा
        {
          from = strstr (content, ":pids:");
          सचे (UNLIKELY (from == रिक्त))
            पटिनिवत्त crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      सचे (UNLIKELY (to == रिक्त))
        पटिनिवत्त crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      सचे (geteuid ())
        पटिनिवत्त 0;

      ret = setup_rt_runtime (resources, path, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      अत्थाय (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (रिक्त, "\n", &saveptr))
        {
          वर्ण *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          सचे (subsystem[0] == '\0')
            {
              सचे (cgroup_mode == CGROUP_MODE_LEGACY)
                अनुवर्त;

              subsystem = "unified";
            }

          सचे (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              सचे (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  सचे (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      अनुवर्त;
                    }
                  पटिनिवत्त ret;
                }
            }
        }

      भङ्ग;

    पक्ष CGROUP_MODE_UNIFIED:
      {
        cleanup_free वर्ण *dir = रिक्त;

        from = strstr (content, "0::");
        सचे (UNLIKELY (from == रिक्त))
          पटिनिवत्त crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        सचे (UNLIKELY (to == रिक्त))
          पटिनिवत्त crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;

        ret = append_paths (&dir, err, CGROUP_मूलम्, path, रिक्त);
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;

        ret = move_process_to_cgroup (pid, रिक्त, path, err);
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;

        ret = enable_controllers (path, err);
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;

        सचे (suffix)
          {
            ret = chown_cgroups (path, args->मूलम्_uid, args->मूलम्_gid, err);
            सचे (UNLIKELY (ret < 0))
              पटिनिवत्त ret;
          }
      }
      भङ्ग;

    स्वभाव:
      पटिनिवत्त crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  *path_out = path;
  path = रिक्त;

  पटिनिवत्त 0;
}

संरचना systemd_job_removed_s
{
  ध्रुव वर्ण *path;
  ध्रुव वर्ण *op;
  पूर्णाङ्क terminated;
  libcrun_error_t err;
};

स्थिर पूर्णाङ्क
systemd_job_removed (sd_bus_message *m, शून्य *userdata, sd_bus_error *error arg_unused)
{
  ध्रुव वर्ण *path, *unit, *result;
  uint32_t id;
  पूर्णाङ्क ret;
  संरचना systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  सचे (ret < 0)
    पटिनिवत्त -1;

  सचे (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      सचे (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          पटिनिवत्त -1;
        }
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
systemd_check_job_status_setup (sd_bus *bus, संरचना systemd_job_removed_s *data, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = sd_bus_match_signal_async (bus, रिक्त, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, रिक्त, data);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, -ret, "sd-bus match signal");

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
systemd_check_job_status (sd_bus *bus, संरचना systemd_job_removed_s *data, ध्रुव वर्ण *path, ध्रुव वर्ण *op,
                          libcrun_error_t *err)
{
  पूर्णाङ्क sd_err;

  data->path = path;
  data->op = op;
  याव (! data->terminated)
    {
      sd_err = sd_bus_process (bus, रिक्त);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus process");

      सचे (sd_err != 0)
        अनुवर्त;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus wait");
    }

  सचे (data->err != रिक्त)
    {
      *err = data->err;
      पटिनिवत्त -1;
    }

  पटिनिवत्त 0;
}

पूर्णाङ्क
parse_sd_array (वर्ण *s, वर्ण **out, वर्ण **next, libcrun_error_t *err)
{
  वर्ण endchr;
  वर्ण *it, *dest;
  bool escaped = false;

  *out = रिक्त;
  *next = रिक्त;

  याव (isspace (*s))
    s++;
  सचे (*s == '\0')
    पटिनिवत्त 0;
  अञ्ञथा सचे (*s != '\'' && *s != '"')
    पटिनिवत्त crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  याव (1)
    {
      सचे (*it == '\0')
        पटिनिवत्त crun_make_error (err, 0, "invalid string `%s`", s);
      सचे (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          याव (isspace (*it))
            it++;
          सचे (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              पटिनिवत्त 0;
            }

          सचे (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              पटिनिवत्त 0;
            }

          पटिनिवत्त crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      सचे (! escaped)
        *dest++ = *it;
      it++;
    }

  पटिनिवत्त 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
स्थिर पूर्णाङ्क
append_systemd_annotation (sd_bus_message *m, ध्रुव वर्ण *name, size_t name_len, ध्रुव वर्ण *मानम्,
                           libcrun_error_t *err)
{
  cleanup_free वर्ण *tmp_name = रिक्त;
  uint32_t factor = 1;
  ध्रुव वर्ण *it;
  पूर्णाङ्क sd_err;

  याव (*मानम् == ' ')
    मानम्++;

  it = मानम्;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  सचे (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  सचे (factor != 1)
    {
      स्थिर ध्रुव वर्ण *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", रिक्त };
      ध्रुव वर्ण *अङ्कः = it;
      वर्ण *endptr = रिक्त;
      द्विगुण v;
      size_t i;

      अत्थाय (i = 0; types[i]; i++)
        सचे (has_prefix (अङ्कः, types[i]))
          {
            अङ्कः += strlen (types[i]);
            भङ्ग;
          }

      errno = 0;
      v = strtod (अङ्कः, &endptr);
      सचे (UNLIKELY (errno != 0 || endptr == अङ्कः || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        पटिनिवत्त crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, मानम्);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      पटिनिवत्त 0;
    }

  सचे ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      पटिनिवत्त 0;
    }
  अञ्ञथा सचे (*it == '\'')
    {
      cleanup_free वर्ण *v_start = रिक्त;
      वर्ण *end;

      it = v_start = xstrdup (मानम्);

      end = strchr (v_start + 1, '\'');
      सचे (end == रिक्त)
        पटिनिवत्त crun_make_error (err, 0, "invalid variant `%s`", मानम्);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      पटिनिवत्त 0;
    }
  अञ्ञथा सचे (*it == '[')
    {
      cleanup_free वर्ण *v_start = रिक्त;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free वर्ण **parts = xmalloc (परिमाण (वर्ण *) * parts_size);
      वर्ण *part;

      part = v_start = xstrdup (it + 1);
      याव (1)
        {
          वर्ण *out = रिक्त, *next = रिक्त;
          पूर्णाङ्क ret;

          ret = parse_sd_array (part, &out, &next, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          parts[n_parts++] = out;
          सचे (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, परिमाण (वर्ण *) * parts_size);
            }
          parts[n_parts] = रिक्त;
          सचे (next == रिक्त)
            भङ्ग;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus close container");

      पटिनिवत्त 0;
    }
  अञ्ञथा सचे (has_prefix (it, "uint64 "))
    {
      वर्ण *endptr = रिक्त;
      uint64_t v;

      errno = 0;
      v = strtoull (it + परिमाण ("uint64"), &endptr, 10);
      सचे (UNLIKELY (errno != 0 || *endptr))
        पटिनिवत्त crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      पटिनिवत्त 0;
    }
  अञ्ञथा सचे (has_prefix (it, "int64 "))
    {
      वर्ण *endptr = रिक्त;
      int64_t v;

      errno = 0;
      v = strtoll (it + परिमाण ("int64"), &endptr, 10);
      सचे (UNLIKELY (errno != 0 || *endptr))
        पटिनिवत्त crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      पटिनिवत्त 0;
    }
  अञ्ञथा सचे (has_prefix (it, "uint32 "))
    {
      वर्ण *endptr = रिक्त;
      uint32_t v;

      errno = 0;
      v = strtoul (it + परिमाण ("uint32"), &endptr, 10);
      सचे (UNLIKELY (errno != 0 || *endptr))
        पटिनिवत्त crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      पटिनिवत्त 0;
    }
  अञ्ञथा सचे (has_prefix (it, "int32 ") || strchr (it, ' ') == रिक्त)
    {
      वर्ण *endptr = रिक्त;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      सचे (has_prefix (it, "int32 "))
        v = strtol (it + परिमाण ("int32"), &endptr, 10);
      अञ्ञथा
        v = strtol (it, &endptr, 10);
      सचे (UNLIKELY (errno != 0 || *endptr))
        पटिनिवत्त crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      पटिनिवत्त 0;
    }

  पटिनिवत्त crun_make_error (err, 0, "unknown type for `%s`", name);
}

स्थिर पूर्णाङ्क
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  पूर्णाङ्क rootless;
  पूर्णाङ्क sd_err = 0;

  rootless = is_rootless (err);
  सचे (UNLIKELY (rootless < 0))
    पटिनिवत्त rootless;

  सचे (rootless)
    sd_err = sd_bus_default_user (bus);
  सचे (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  सचे (sd_err < 0)
    पटिनिवत्त crun_make_error (err, -sd_err, "cannot open sd-bus");

  पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ध्रुव वर्ण *name)
{
  size_t i;

  सचे (resources == रिक्त || resources->unified == रिक्त)
    पटिनिवत्त रिक्त;

  अत्थाय (i = 0; i < resources->unified->len; i++)
    सचे (strcmp (resources->unified->कुञ्चिकाः[i], name) == 0)
      पटिनिवत्त resources->unified->मानानि[i];
  पटिनिवत्त रिक्त;
}

स्थिर पूर्णाङ्क
get_मानम्_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ध्रुव वर्ण *name,
                            uint64_t *मानम्, libcrun_error_t *err)
{
  वर्ण *endptr = रिक्त;
  size_t i;

  सचे (resources == रिक्त || resources->unified == रिक्त)
    पटिनिवत्त 0;

  अत्थाय (i = 0; i < resources->unified->len; i++)
    सचे (strcmp (resources->unified->कुञ्चिकाः[i], name) == 0)
      {
        सचे (strcmp (resources->unified->मानानि[i], "max") == 0)
          {
            *मानम् = UINT64_MAX;
            पटिनिवत्त 1;
          }

        errno = 0;
        *मानम् = strtoull (resources->unified->मानानि[i], &endptr, 10);
        सचे (UNLIKELY (errno))
          पटिनिवत्त crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->मानानि[i]);
        सचे (endptr && *endptr)
          पटिनिवत्त crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->मानानि[i]);
        पटिनिवत्त 1;
      }
  पटिनिवत्त 0;
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_memory_नीचः (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  सचे (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      पटिनिवत्त 1;
    }

  पटिनिवत्त get_मानम्_from_unified_map (resources, "memory.low", limit, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  सचे (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      पटिनिवत्त 1;
    }

  पटिनिवत्त get_मानम्_from_unified_map (resources, "pids.max", limit, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  सचे (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      पटिनिवत्त 1;
    }

  पटिनिवत्त get_मानम्_from_unified_map (resources, "memory.max", limit, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  सचे (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      सचे (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          पटिनिवत्त 1;
        }
      /* 0 means unset.  */
      सचे (memory->swap == 0)
        पटिनिवत्त 0;
      सचे (mem <= 0)
        पटिनिवत्त crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      सचे (memory->swap < mem)
        पटिनिवत्त crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      पटिनिवत्त 1;
    }

  पटिनिवत्त get_मानम्_from_unified_map (resources, "memory.swap.max", limit, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t मानम्;
  पूर्णाङ्क ret;

  ret = get_मानम्_from_unified_map (resources, "cpu.idle", &मानम्, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;
  सचे (ret > 0 && मानम् == 1)
    has_idle = true;
  सचे (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_मानम्_from_unified_map (resources, "cpu.weight", weight, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;
  सचे (ret > 0)
    {
      सचे (has_idle)
        पटिनिवत्त crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      पटिनिवत्त 1;
    }

  सचे (resources->cpu && resources->cpu->shares_present)
    {
      सचे (has_idle)
        पटिनिवत्त crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      सचे (resources->cpu->shares == 0)
        पटिनिवत्त 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      पटिनिवत्त 1;
    }
  सचे (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      पटिनिवत्त 1;
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
read_uint64_मानम् (ध्रुव वर्ण *str, uint64_t *मानम्, libcrun_error_t *err)
{
  वर्ण *endptr = रिक्त;

  errno = 0;
  *मानम् = strtoull (str, &endptr, 10);
  सचे (UNLIKELY (errno))
    पटिनिवत्त crun_make_error (err, errno, "invalid value: %s", str);
  सचे (endptr && *endptr)
    पटिनिवत्त crun_make_error (err, 0, "invalid value: %s", str);
  पटिनिवत्त 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
स्थिर अन्तःस्थ uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  पटिनिवत्त bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

स्थिर पूर्णाङ्क
append_io_weight (sd_bus_message *m, वर्ण **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  पूर्णाङ्क sd_err;
  size_t i;
  पूर्णाङ्क ret;

#  define APPEND_IO_WEIGHT(मानम्)                                                      \
    करोहि                                                                                 \
      {                                                                                \
        सचे (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            सचे (UNLIKELY (sd_err < 0))                                                 \
              पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } याव (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, मानम्)                                                      \
    करोहि                                                                                                \
      {                                                                                               \
        सचे (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            सचे (UNLIKELY (sd_err < 0))                                                                \
              पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } याव (0)

  सचे (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  अत्थाय (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      वर्ण name[64];
      पूर्णाङ्क len;

      len = snprintf (name, परिमाण (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      सचे (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (name)))
        पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  अत्थाय (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free वर्ण *मानम् = रिक्त;
      bool need_conversion = false;
      वर्ण *saveptr = रिक्त;
      वर्ण *पङ्क्तिः;

      सचे (strcmp (resources->unified->कुञ्चिकाः[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      अञ्ञथा सचे (strcmp (resources->unified->कुञ्चिकाः[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      अञ्ञथा
        {
          /* Skip other keys.  */
          अनुवर्त;
        }

      मानम् = xstrdup (resources->unified->मानानि[i]);

      अत्थाय (पङ्क्तिः = strtok_r (मानम्, "\n", &saveptr); पङ्क्तिः; पङ्क्तिः = strtok_r (रिक्त, "\n", &saveptr))
        {
          वर्ण *sep;

          sep = strchr (पङ्क्तिः, ' ');
          याव (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_मानम् (sep ? sep : पङ्क्तिः, &weight, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          सचे (need_conversion)
            weight = IO_WEIGHT (weight);

          सचे (sep == रिक्त || strcmp (पङ्क्तिः, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          अञ्ञथा
            APPEND_IO_DEVICE_WEIGHT (पङ्क्तिः, weight);
        }
    }

  पटिनिवत्त 0;
}

/* Adapted from systemd.  */
स्थिर पूर्णाङ्क
bus_append_byte_array (sd_bus_message *m, ध्रुव वर्ण *field, ध्रुव शून्य *buf, size_t n, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  सचे (ret < 0)
    पटिनिवत्त crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  सचे (ret < 0)
    पटिनिवत्त crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  सचे (ret < 0)
    पटिनिवत्त crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  सचे (ret < 0)
    पटिनिवत्त crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  सचे (ret < 0)
    पटिनिवत्त crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  सचे (ret < 0)
    पटिनिवत्त crun_make_error (err, -ret, "sd_bus_message_close_container");

  पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क
append_uint64_from_unified_map (sd_bus_message *m,
                                वर्ण **missing_properties,
                                ध्रुव वर्ण *attr,
                                ध्रुव वर्ण *कुञ्चिका,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t मानम्;
  पूर्णाङ्क ret;

  सचे (property_missing_p (missing_properties, attr))
    पटिनिवत्त 0;

  ret = get_मानम्_from_unified_map (resources, कुञ्चिका, &मानम्, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;
  सचे (ret)
    {
      पूर्णाङ्क sd_err = sd_bus_message_append (m, "(sv)", attr, "t", मानम्);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  पटिनिवत्त 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
स्थिर पूर्णाङ्क
append_device_allow (sd_bus_message *m,
                     ध्रुव वर्ण type,
                     पूर्णाङ्क major,
                     पूर्णाङ्क minor,
                     ध्रुव वर्ण *access,
                     libcrun_error_t *err)
{
  वर्ण device[64];
  पूर्णाङ्क sd_err;
  पूर्णाङ्क len;

  सचे (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      पटिनिवत्त 0;
    }

  सचे (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, परिमाण (device), "%s-*", type == 'c' ? "char" : "block");
  अञ्ञथा सचे (IS_WILDCARD (minor))
    len = snprintf (device, परिमाण (device), type == 'c' ? "char-%d" : "block-%d", major);
  अञ्ञथा
    len = snprintf (device, परिमाण (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  सचे (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (device)))
    पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  सचे (UNLIKELY (sd_err < 0))
    पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  पटिनिवत्त 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

स्थिर पूर्णाङ्क
access_to_mode (ध्रुव वर्ण *access, libcrun_error_t *err)
{
  पूर्णाङ्क mode = 0;

  याव (access && *access)
    {
      पूर्णाङ्क c = *access++;

      विकल्प (c)
        {
        पक्ष 'r':
          mode |= MODE_R;
          भङ्ग;

        पक्ष 'w':
          mode |= MODE_W;
          भङ्ग;

        पक्ष 'm':
          mode |= MODE_M;
          भङ्ग;

        स्वभाव:
          पटिनिवत्त crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  पटिनिवत्त mode;
}

/* check if the type b is included in type a.  */
स्थिर bool
has_same_type (ध्रुव वर्ण *a, ध्रुव वर्ण *b)
{
  bool a_all = a == रिक्त || strcmp (a, "a") == 0;
  सचे (b == रिक्त || strcmp (b, "a") == 0)
    पटिनिवत्त a_all;

  पटिनिवत्त a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
स्थिर पूर्णाङ्क
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  पूर्णाङ्क req_mode;

#  define CMP_DEV_अङ्कः(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  सचे (UNLIKELY (req_mode < 0))
    पटिनिवत्त req_mode;

  अत्थाय (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      पूर्णाङ्क mode;

      सचे (! CMP_DEV_अङ्कः (devices[n]->minor, devices[current_rule]->minor))
        अनुवर्त;
      सचे (! CMP_DEV_अङ्कः (devices[n]->major, devices[current_rule]->major))
        अनुवर्त;

      सचे (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        अनुवर्त;

      mode = access_to_mode (devices[i]->access, err);
      सचे (UNLIKELY (mode < 0))
        पटिनिवत्त mode;

      सचे (devices[current_rule]->allow && (mode & req_mode))
        पटिनिवत्त 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      सचे (req_mode == 0)
        पटिनिवत्त 1;
    }

  पटिनिवत्त 0;
#  undef CMP_DEV_अङ्कः
}

स्थिर size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  सचे (n == 0)
    पटिनिवत्त 1;

  /* Find the first rule that is after the last "block all".  */
  अत्थाय (i = n - 1; i-- > 0;)
    {
      सचे ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        पटिनिवत्त i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  अत्थाय (i = 0; i < n; i++)
    सचे (devices[i]->allow)
      पटिनिवत्त i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  पटिनिवत्त n + 1;
}

स्थिर bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  पूर्णाङ्क remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  अत्थाय (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      सचे (! devices[current_rule]->allow)
        पटिनिवत्त false;

      सचे (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          सचे (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            पटिनिवत्त true;

          सचे (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          अञ्ञथा सचे (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          सचे (remaining == 0)
            पटिनिवत्त true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  पटिनिवत्त false;
}

स्थिर पूर्णाङ्क
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  संरचना default_dev_s *default_devices = get_default_devices ();
  पूर्णाङ्क ret, sd_err;
  size_t i;

  सचे (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      पटिनिवत्त 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  सचे (UNLIKELY (sd_err < 0))
    पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  अत्थाय (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  अत्थाय (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      वर्ण type;

      सचे (! d->allow)
        {
          पूर्णाङ्क redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          सचे (UNLIKELY (redundant < 0))
            पटिनिवत्त redundant;

          सचे (redundant)
            अनुवर्त;

          पटिनिवत्त crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      सचे (d->type == रिक्त || strcmp (d->type, "a") == 0)
        type = 'a';
      अञ्ञथा सचे (strcmp (d->type, "c") == 0)
        type = 'c';
      अञ्ञथा सचे (strcmp (d->type, "b") == 0)
        type = 'b';
      अञ्ञथा
        पटिनिवत्त crun_make_error (err, 0, "unknown device type `%s`", d->type);

      सचे (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
      अञ्ञथा
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
    }

  पटिनिवत्त 0;
}

स्थिर bool
has_bpf_fs ()
{
  संरचना statfs stat;

  पटिनिवत्त statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

स्थिर पूर्णाङ्क
bpfprog_path_from_scope (वर्ण **path, ध्रुव वर्ण *scope, libcrun_error_t *err)
{
  cleanup_free वर्ण *flat_scope = xstrdup (scope);
  वर्ण *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  याव ((it = strchr (it, '.')) != रिक्त)
    *it = '_';

  पटिनिवत्त append_paths (path, err, CRUN_BPF_DIR, flat_scope, रिक्त);
}

स्थिर पूर्णाङ्क
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 वर्ण **missing_properties,
                 ध्रुव वर्ण *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  पूर्णाङ्क ret = 0;
  पूर्णाङ्क in_userns, sd_err;
  ध्रुव वर्ण *property_name = "BPFProgram";
  cleanup_free संरचना bpf_program *program = रिक्त;
  cleanup_free वर्ण *path = रिक्त;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  सचे (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    पटिनिवत्त 0;

  in_userns = check_running_in_user_namespace (err);
  सचे (UNLIKELY (in_userns < 0))
    पटिनिवत्त in_userns;

  सचे (in_userns)
    पटिनिवत्त 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  सचे (UNLIKELY (program == रिक्त))
    {
      crun_error_release (err);
      पटिनिवत्त 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  सचे (UNLIKELY (ret < 0 && errno != EEXIST))
    पटिनिवत्त crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  सचे (is_update)
    {
      cleanup_free संरचना bpf_program *program_loaded = रिक्त;

      सचे (resources->devices == रिक्त || resources->devices_len == 0)
        {
          *devices_set = true;
          पटिनिवत्त 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      /* it is the same as the loaded program.  */
      सचे (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          पटिनिवत्त 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free संरचना bpf_program *program_new = रिक्त;
        वर्ण tmp[64];
        पूर्णाङ्क len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, परिमाण (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        सचे (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (tmp)))
          पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;

        सचे (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            पटिनिवत्त 0;
          }
      }

      पटिनिवत्त crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  सचे (UNLIKELY (sd_err < 0))
    पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  पटिनिवत्त 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
स्थिर पूर्णाङ्क
append_cpu_quota (sd_bus_message *m, वर्ण **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ध्रुव वर्ण *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  पूर्णाङ्क sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  सचे (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      पूर्णाङ्क ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
      सचे (has_period)
        period = max_period;
    }

  सचे (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  सचे ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      सचे (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          सचे (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      सचे (UNLIKELY (sd_err < 0))
        पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
append_resources (sd_bus_message *m,
                  bool is_update,
                  ध्रुव वर्ण *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  पूर्णाङ्क cgroup_mode,
                  ध्रुव वर्ण *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t मानम्;
  पूर्णाङ्क sd_err;
  पूर्णाङ्क ret;
  cleanup_free वर्ण *dir = रिक्त;
  cleanup_free वर्ण **missing_properties = रिक्त;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  missing_properties = read_dir_entries (dir, err);
  सचे (UNLIKELY (missing_properties == रिक्त))
    {
      सचे (crun_error_get_errno (err) != ENOENT)
        पटिनिवत्त -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_मानम्(name, मानम्)                                             \
    करोहि                                                                                 \
      {                                                                                \
        सचे (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", मानम्);              \
            सचे (UNLIKELY (sd_err < 0))                                                 \
              पटिनिवत्त crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } याव (0)

#  define APPEND_UINT64(name, fn)              \
    करोहि                                         \
      {                                        \
        ret = fn (resources, &मानम्, err);     \
        सचे (UNLIKELY (ret < 0))                \
          पटिनिवत्त ret;                          \
        सचे (ret)                               \
          {                                    \
            APPEND_UINT64_मानम् (name, मानम्); \
          }                                    \
    } याव (0)

  सचे (resources == रिक्त)
    पटिनिवत्त 0;

  APPEND_UINT64 ("MemoryLow", get_memory_नीचः);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  विकल्प (cgroup_mode)
    {
    पक्ष CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;

        सचे (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ध्रुव वर्ण *property_name = "AllowedCPUs";
            ध्रुव वर्ण *मानम् = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free वर्ण *allowed_cpus = रिक्त;
            size_t allowed_cpus_len = 0;

            सचे (मानम् == रिक्त)
              मानम् = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (मानम्, &allowed_cpus, &allowed_cpus_len, err);
            सचे (UNLIKELY (ret < 0))
              पटिनिवत्त ret;

            सचे (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                सचे (UNLIKELY (ret < 0))
                  पटिनिवत्त ret;
              }
          }

        सचे (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ध्रुव वर्ण *property_name = "AllowedMemoryNodes";
            ध्रुव वर्ण *मानम् = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free वर्ण *allowed_mems = रिक्त;
            size_t allowed_mems_len = 0;

            सचे (मानम् == रिक्त)
              मानम् = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (मानम्, &allowed_mems, &allowed_mems_len, err);
            सचे (UNLIKELY (ret < 0))
              पटिनिवत्त ret;

            सचे (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                सचे (UNLIKELY (ret < 0))
                  पटिनिवत्त ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        सचे (UNLIKELY (ret < 0))
          पटिनिवत्त ret;
      }
      भङ्ग;

    पक्ष CGROUP_MODE_LEGACY:
    पक्ष CGROUP_MODE_HYBRID:
      सचे (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_मानम् ("CPUShares", resources->cpu->shares);
      भङ्ग;

    स्वभाव:
      पटिनिवत्त crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_मानम्

  सचे (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
reset_failed_unit (sd_bus *bus, ध्रुव वर्ण *unit)
{
  पूर्णाङ्क sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = रिक्त, *reply = रिक्त;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  सचे (UNLIKELY (sd_err < 0))
    गच्छ exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  सचे (UNLIKELY (sd_err < 0))
    गच्छ exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  सचे (UNLIKELY (sd_err < 0))
    गच्छ exit;

  sd_err = 0;

exit:
  सचे (m)
    sd_bus_message_unref (m);
  सचे (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  पटिनिवत्त sd_err;
}

स्थिर पूर्णाङ्क
verify_ebpf_device_filter_installed (ध्रुव वर्ण *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = रिक्त;
  cleanup_free वर्ण *full_path = रिक्त;
  size_t n_progs = 0;
  पूर्णाङ्क ret;

  ret = append_paths (&full_path, err, CGROUP_मूलम्, cgroup_path, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (n_progs == 0)
    पटिनिवत्त crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  पटिनिवत्त 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
स्थिर पूर्णाङ्क
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, पूर्णाङ्क ret)
{
  सचे (bus)
    sd_bus_unref (bus);
  सचे (m)
    sd_bus_message_unref (m);
  सचे (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            पूर्णाङ्क cgroup_mode,
                            string_map *annotations,
                            ध्रुव वर्ण *state_मूलम्,
                            ध्रुव वर्ण *scope, ध्रुव वर्ण *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = रिक्त;
  sd_bus_message *m = रिक्त;
  sd_bus_message *reply = रिक्त;
  पूर्णाङ्क sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ध्रुव वर्ण *object = रिक्त;
  संरचना systemd_job_removed_s job_data = {};
  पूर्णाङ्क i;
  ध्रुव वर्ण *boolean_opts[10];
  cleanup_free वर्ण *state_dir = रिक्त;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_मूलम्, रिक्त, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

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
  सचे (UNLIKELY (ret < 0))
    गच्छ exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      गच्छ exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      गच्छ exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      गच्छ exit;
    }

  सचे (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      सचे (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          गच्छ exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      गच्छ exit;
    }

  सचे (annotations)
    {
      size_t prefix_len = परिमाण (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      अत्थाय (i = 0; i < annotations_len; i++)
        {
          ध्रुव वर्ण *कुञ्चिका;
          ध्रुव वर्ण *मानम्;
          size_t len;

          ret = string_map_get_at (annotations, i, &कुञ्चिका, &मानम्);
          सचे (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              गच्छ exit;
            }

          सचे (! has_prefix (कुञ्चिका, SYSTEMD_PROPERTY_PREFIX))
            अनुवर्त;

          len = strlen (कुञ्चिका);
          सचे (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", कुञ्चिका);
              गच्छ exit;
            }

          ret = append_systemd_annotation (m, कुञ्चिका + prefix_len, len - prefix_len,
                                           मानम्, err);
          सचे (UNLIKELY (ret < 0))
            गच्छ exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      गच्छ exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      गच्छ exit;
    }

  अत्थाय (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      सचे (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          गच्छ exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ exit;

  sd_err = sd_bus_message_close_container (m);
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      गच्छ exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      गच्छ exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  सचे (UNLIKELY (sd_err < 0))
    {
      सचे (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          सचे (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = रिक्त;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      सचे (sd_err < 0)
        {
          सचे (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              सचे (UNLIKELY (ret < 0))
                गच्छ exit;
              सचे (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          गच्छ exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      गच्छ exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  पटिनिवत्त cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

स्थिर पूर्णाङ्क
libcrun_destroy_systemd_cgroup_scope (संरचना libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = रिक्त;
  sd_bus_message *m = रिक्त;
  sd_bus_message *reply = रिक्त;
  पूर्णाङ्क ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ध्रुव वर्ण *object;
  ध्रुव वर्ण *scope = cgroup_status->scope;
  संरचना systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  सचे (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      गच्छ exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  सचे (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      गच्छ exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  सचे (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      गच्छ exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  सचे (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      गच्छ exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  पटिनिवत्त cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

स्थिर ध्रुव वर्ण *
find_systemd_subgroup (string_map *annotations)
{
  ध्रुव वर्ण *annotation;

  annotation = find_string_map_मानम् (annotations, "run.oci.systemd.subgroup");
  सचे (annotation)
    {
      सचे (annotation[0] == '\0')
        पटिनिवत्त रिक्त;
      पटिनिवत्त annotation;
    }

  पटिनिवत्त "container";
}

स्थिर पूर्णाङ्क
libcrun_cgroup_enter_systemd (संरचना libcrun_cgroup_args *args,
                              संरचना libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  ध्रुव वर्ण *cgroup_path = args->cgroup_path;
  cleanup_free वर्ण *scope_path = रिक्त;
  cleanup_free वर्ण *scope = रिक्त;
  cleanup_free वर्ण *path = रिक्त;
  cleanup_free वर्ण *slice = रिक्त;
  पूर्णाङ्क retries_वामः = 32;
  ध्रुव वर्ण *suffix;
  ध्रुव वर्ण *id = args->id;
  pid_t pid = args->pid;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क rootless = 0;
  पूर्णाङ्क ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  सचे (UNLIKELY (cgroup_mode < 0))
    पटिनिवत्त cgroup_mode;

  सचे (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      सचे (UNLIKELY (rootless < 0))
        पटिनिवत्त rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  अत्थाय (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_मूलम्,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      सचे (LIKELY (ret >= 0))
        भङ्ग;

      सचे (can_retry && retries_वामः-- > 0)
        {
          crun_error_release (err);
          अनुवर्त;
        }

      पटिनिवत्त ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  सचे (is_empty_string (suffix))
    scope_path = xstrdup (path);
  अञ्ञथा
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      सचे (UNLIKELY (scope_path == रिक्त))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  सचे (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  out->path = path;
  path = रिक्त;

  out->scope = scope;
  scope = रिक्त;
  पटिनिवत्त 0;
}

वर्ण *
get_cgroup_scope_path (ध्रुव वर्ण *cgroup_path, ध्रुव वर्ण *scope)
{
  वर्ण *path_to_scope = रिक्त;
  वर्ण *cur;

  सचे (is_empty_string (cgroup_path))
    पटिनिवत्त रिक्त;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  याव (cur)
    {
      वर्ण *next = strchr (cur + 1, '/');
      सचे (next == रिक्त)
        भङ्ग;

      *next = '\0';
      सचे (strcmp (cur, scope) == 0)
        पटिनिवत्त path_to_scope;
      *next = '/';

      cur = next;
      याव (*cur == '/')
        cur++;
    }

  पटिनिवत्त path_to_scope;
}

स्थिर पूर्णाङ्क
libcrun_destroy_cgroup_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free वर्ण *path_to_scope = रिक्त;
  cleanup_free वर्ण *bpfprog = रिक्त;
  पूर्णाङ्क mode;
  पूर्णाङ्क ret;

  mode = libcrun_get_cgroup_mode (err);
  सचे (UNLIKELY (mode < 0))
    पटिनिवत्त mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  सचे (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  सचे (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  सचे (UNLIKELY (ret < 0))
    crun_error_release (err);
  अञ्ञथा
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  पटिनिवत्त destroy_cgroup_path (path_to_scope, mode, err);
}

स्थिर पूर्णाङ्क
libcrun_update_resources_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                  ध्रुव वर्ण *state_मूलम्,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  संरचना systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free वर्ण *state_dir = रिक्त;
  sd_bus_message *reply = रिक्त;
  sd_bus_message *m = रिक्त;
  sd_bus *bus = रिक्त;
  पूर्णाङ्क sd_err, ret;
  पूर्णाङ्क cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_मूलम्, रिक्त, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  सचे (UNLIKELY (cgroup_mode < 0))
    पटिनिवत्त cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      गच्छ exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      गच्छ exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      गच्छ exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ exit;

  sd_err = sd_bus_message_close_container (m);
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      गच्छ exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  सचे (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      गच्छ exit;
    }

  सचे (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ exit;

  ret = 0;

exit:
  पटिनिवत्त cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
स्थिर पूर्णाङ्क
libcrun_attach_process_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                ध्रुव वर्ण *path, pid_t pid,
                                libcrun_error_t *err)
{
  ध्रुव वर्ण *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free वर्ण *scope_path = रिक्त;
  sd_bus_message *reply = रिक्त;
  ध्रुव वर्ण *subcgroup;
  ध्रुव वर्ण *last;
  sd_bus *bus = रिक्त;
  size_t len;
  पूर्णाङ्क sd_err, ret;

  सचे (is_empty_string (scope))
    पटिनिवत्त crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  सचे (scope_path == रिक्त)
    पटिनिवत्त crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  सचे (last == रिक्त || strcmp (last + 1, scope) != 0)
    पटिनिवत्त crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  सचे (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  पटिनिवत्त cleanup_sd_bus_and_return (bus, रिक्त, reply, &error, ret);
}

#अञ्ञथा
स्थिर पूर्णाङ्क
libcrun_cgroup_enter_systemd (संरचना libcrun_cgroup_args *args,
                              संरचना libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (शून्य) args;
  (शून्य) out;

  पटिनिवत्त crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णाङ्क
libcrun_destroy_cgroup_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (शून्य) cgroup_status;

  पटिनिवत्त crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णाङ्क
libcrun_update_resources_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                  ध्रुव वर्ण *state_मूलम्,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (शून्य) cgroup_status;
  (शून्य) state_मूलम्;
  (शून्य) resources;

  पटिनिवत्त crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णाङ्क
libcrun_attach_process_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                ध्रुव वर्ण *path, pid_t pid,
                                libcrun_error_t *err)
{
  (शून्य) cgroup_status;
  (शून्य) path;
  (शून्य) pid;

  पटिनिवत्त crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

संरचना libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = रिक्त,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
