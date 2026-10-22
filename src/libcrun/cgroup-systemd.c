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

स्थिर पूर्णांक
register_missing_property_from_message (स्थिरांक अक्षर *state_dir, स्थिरांक अक्षर *message, libcrun_error_t *err)
{
  cleanup_free अक्षर *file_path = रिक्त;
  cleanup_free अक्षर *dir_path = रिक्त;
  cleanup_free अक्षर *property = रिक्त;
  अक्षर *p;
  पूर्णांक ret;

  जर (! has_prefix (message, "Cannot set property "))
    परत 0;

  property = xstrdup (message + आकार ("Cannot set property ") - 1);
  p = म_अक्षर (property, ',');
  जर (! p)
    परत 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = append_paths (&file_path, err, dir_path, property, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, रिक्त, 0, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत 1;
}

स्थिर bool
property_missing_p (अक्षर **missing_properties, स्थिरांक अक्षर *property)
{
  माप_प्रकार i;

  साठी (i = 0; missing_properties && missing_properties[i]; i++)
    जर (म_भेद (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        परत true;
      }

  परत false;
}

स्थिर शून्य
get_systemd_scope_and_slice (स्थिरांक अक्षर *id, bool user_slice, स्थिरांक अक्षर *cgroup_path,
                             अक्षर **scope, अक्षर **slice)
{
  स्थिरांक अक्षर *n;
  अक्षर *p;

  जर (cgroup_path == रिक्त || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      परत;
    }

  n = म_अक्षर (cgroup_path, ':');
  जर (n == रिक्त)
    xasprintf (scope, "%s.scope", cgroup_path);
  अन्यथा
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = म_अक्षर (*scope, ':');
      जर (p)
        *p = '-';
    }
  जर (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = म_अक्षर (*slice, ':');
      जर (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      जर (is_empty_string (*slice))
        {
          मुक्त (*slice);
          जर (user_slice)
            *slice = xstrdup ("user.slice");
          अन्यथा
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
स्थिर पूर्णांक
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = रिक्त;
  cleanup_close पूर्णांक dirfd = -1;
  bool need_set_parent = true;
  अक्षर fmt_buf[64];
  पूर्णांक len;
  पूर्णांक ret;

  जर (resources == रिक्त || resources->cpu == रिक्त)
    परत 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  जर (is_empty_string (path))
    परत crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  जर (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_मूळ, "cpu", path, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  जर (UNLIKELY (dirfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  जर (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");

      जर (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, आकार (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (fmt_buf)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");

      जर (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  परत 0;
}

स्थिर पूर्णांक
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = रिक्त;
  पूर्णांक parent;
  पूर्णांक ret;

  जर (resources == रिक्त || resources->cpu == रिक्त)
    परत 0;

  जर (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    परत 0;

  साठी (parent = 0; parent < 2; parent++)
    {
      cleanup_close पूर्णांक dirfd = -1;

      जर (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_मूळ, path ? path : "", (parent ? ".." : रिक्त), रिक्त);
      अन्यथा
        ret = append_paths (&cgroup_path, err, CGROUP_मूळ, "/cpu", path ? path : "", (parent ? ".." : रिक्त), रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;

      dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      जर (UNLIKELY (dirfd < 0))
        परत crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      जर (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }
    }

  परत 0;
}

स्थिर पूर्णांक
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = रिक्त;
  पूर्णांक parent;
  पूर्णांक ret;
  पूर्णांक levels;

  ret = append_paths (&cgroup_path, err, CGROUP_मूळ, "/cpuset", path ? path : "", रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  साठी (parent = 0; parent < levels; parent++)
    {
      cleanup_close पूर्णांक dirfd_cpuset = -1;
      cleanup_free अक्षर *path_to_cpuset = रिक्त;

      ret = append_paths (&path_to_cpuset, err, CGROUP_मूळ "/cpuset", path, (parent ? ".." : रिक्त), रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;

      dirfd_cpuset = खोलो (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      जर (UNLIKELY (dirfd_cpuset < 0))
        परत crun_make_error (err, त्रुटि_सं, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  परत 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
स्थिर पूर्णांक
compute_finalized_path (अक्षर **path, स्थिरांक अक्षर *from, स्थिरांक अक्षर *suffix, libcrun_error_t *err)
{
  जर (suffix == रिक्त)
    {
      *path = xstrdup (from);
      परत 0;
    }
  परत append_paths (path, err, from, suffix, रिक्त);
}

स्थिर पूर्णांक
systemd_finalize (संरचना libcrun_cgroup_args *args, अक्षर **path_out,
                  पूर्णांक cgroup_mode, स्थिरांक अक्षर *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free अक्षर *content = रिक्त;
  cleanup_free अक्षर *path = रिक्त;
  pid_t pid = args->pid;
  पूर्णांक ret;
  अक्षर *from, *to;
  अक्षर *saveptr = रिक्त;

  cleanup_free अक्षर *proc_path = रिक्त;
  cleanup_close पूर्णांक fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  जर (UNLIKELY (fd < 0))
    परत fd;

  ret = read_all_fd (fd, "cgroup path", &content, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  निवड (cgroup_mode)
    {
    स्थिती CGROUP_MODE_HYBRID:
    स्थिती CGROUP_MODE_LEGACY:
      from = म_माला (content, ":memory:");
      जर (LIKELY (from != रिक्त))
        from += 8;
      अन्यथा
        {
          from = म_माला (content, ":pids:");
          जर (UNLIKELY (from == रिक्त))
            परत crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = म_अक्षर (from, '\n');
      जर (UNLIKELY (to == रिक्त))
        परत crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      जर (UNLIKELY (ret < 0))
        परत ret;

      जर (geteuid ())
        परत 0;

      ret = setup_rt_runtime (resources, path, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      साठी (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (रिक्त, "\n", &saveptr))
        {
          अक्षर *subpath, *subsystem;
          subsystem = म_अक्षर (from, ':') + 1;
          subpath = म_अक्षर (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          जर (subsystem[0] == '\0')
            {
              जर (cgroup_mode == CGROUP_MODE_LEGACY)
                सुरूठेवा;

              subsystem = "unified";
            }

          जर (म_भेद (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              जर (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  जर (म_अक्षर (subsystem, '='))
                    {
                      crun_error_release (err);
                      सुरूठेवा;
                    }
                  परत ret;
                }
            }
        }

      थांबा;

    स्थिती CGROUP_MODE_UNIFIED:
      {
        cleanup_free अक्षर *dir = रिक्त;

        from = म_माला (content, "0::");
        जर (UNLIKELY (from == रिक्त))
          परत crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = म_अक्षर (from, '\n');
        जर (UNLIKELY (to == रिक्त))
          परत crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        जर (UNLIKELY (ret < 0))
          परत ret;

        ret = append_paths (&dir, err, CGROUP_मूळ, path, रिक्त);
        जर (UNLIKELY (ret < 0))
          परत ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        जर (UNLIKELY (ret < 0))
          परत ret;

        ret = move_process_to_cgroup (pid, रिक्त, path, err);
        जर (UNLIKELY (ret < 0))
          परत ret;

        ret = enable_controllers (path, err);
        जर (UNLIKELY (ret < 0))
          परत ret;

        जर (suffix)
          {
            ret = chown_cgroups (path, args->मूळ_uid, args->मूळ_gid, err);
            जर (UNLIKELY (ret < 0))
              परत ret;
          }
      }
      थांबा;

    पूर्वनिर्धारित:
      परत crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  *path_out = path;
  path = रिक्त;

  परत 0;
}

संरचना systemd_job_removed_s
{
  स्थिरांक अक्षर *path;
  स्थिरांक अक्षर *op;
  पूर्णांक terminated;
  libcrun_error_t err;
};

स्थिर पूर्णांक
systemd_job_removed (sd_bus_message *m, शून्य *userdata, sd_bus_error *error arg_unused)
{
  स्थिरांक अक्षर *path, *unit, *result;
  uint32_t id;
  पूर्णांक ret;
  संरचना systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  जर (ret < 0)
    परत -1;

  जर (म_भेद (d->path, path) == 0)
    {
      d->terminated = 1;
      जर (म_भेद (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          परत -1;
        }
    }
  परत 0;
}

स्थिर पूर्णांक
systemd_check_job_status_setup (sd_bus *bus, संरचना systemd_job_removed_s *data, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = sd_bus_match_signal_async (bus, रिक्त, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, रिक्त, data);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, -ret, "sd-bus match signal");

  परत 0;
}

स्थिर पूर्णांक
systemd_check_job_status (sd_bus *bus, संरचना systemd_job_removed_s *data, स्थिरांक अक्षर *path, स्थिरांक अक्षर *op,
                          libcrun_error_t *err)
{
  पूर्णांक sd_err;

  data->path = path;
  data->op = op;
  पर्यंत (! data->terminated)
    {
      sd_err = sd_bus_process (bus, रिक्त);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus process");

      जर (sd_err != 0)
        सुरूठेवा;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus wait");
    }

  जर (data->err != रिक्त)
    {
      *err = data->err;
      परत -1;
    }

  परत 0;
}

पूर्णांक
parse_sd_array (अक्षर *s, अक्षर **out, अक्षर **next, libcrun_error_t *err)
{
  अक्षर endchr;
  अक्षर *it, *dest;
  bool escaped = false;

  *out = रिक्त;
  *next = रिक्त;

  पर्यंत (है_खाली (*s))
    s++;
  जर (*s == '\0')
    परत 0;
  अन्यथा जर (*s != '\'' && *s != '"')
    परत crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  पर्यंत (1)
    {
      जर (*it == '\0')
        परत crun_make_error (err, 0, "invalid string `%s`", s);
      जर (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          पर्यंत (है_खाली (*it))
            it++;
          जर (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              परत 0;
            }

          जर (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              परत 0;
            }

          परत crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      जर (! escaped)
        *dest++ = *it;
      it++;
    }

  परत 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
स्थिर पूर्णांक
append_systemd_annotation (sd_bus_message *m, स्थिरांक अक्षर *name, माप_प्रकार name_len, स्थिरांक अक्षर *मूल्य,
                           libcrun_error_t *err)
{
  cleanup_free अक्षर *tmp_name = रिक्त;
  uint32_t factor = 1;
  स्थिरांक अक्षर *it;
  पूर्णांक sd_err;

  पर्यंत (*मूल्य == ' ')
    मूल्य++;

  it = मूल्य;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  जर (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
      && name[name_len - 1] == 'c')
    {
      factor = 1000000;

      tmp_name = xmalloc (name_len + 2);
      स_नकल (tmp_name, name, name_len - 3);
      स_नकल (tmp_name + name_len - 3, "USec", 5);

      name = tmp_name;
    }

  /* A *USec property is always an uint64, so, like runc does, accept any
     number of seconds (possibly with a fractional part, and with an
     optional integer type), and convert it to uint64 microseconds.  */
  जर (factor != 1)
    {
      स्थिर स्थिरांक अक्षर *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", रिक्त };
      स्थिरांक अक्षर *अंक = it;
      अक्षर *endptr = रिक्त;
      द्विगुण v;
      माप_प्रकार i;

      साठी (i = 0; types[i]; i++)
        जर (has_prefix (अंक, types[i]))
          {
            अंक += म_माप (types[i]);
            थांबा;
          }

      त्रुटि_सं = 0;
      v = म_से_भग्न (अंक, &endptr);
      जर (UNLIKELY (त्रुटि_सं != 0 || endptr == अंक || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        परत crun_make_error (err, त्रुटि_सं, "invalid value for `%s`: `%s`", name, मूल्य);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      परत 0;
    }

  जर ((म_भेद (it, "true") == 0) || (म_भेद (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      परत 0;
    }
  अन्यथा जर (*it == '\'')
    {
      cleanup_free अक्षर *v_start = रिक्त;
      अक्षर *end;

      it = v_start = xstrdup (मूल्य);

      end = म_अक्षर (v_start + 1, '\'');
      जर (end == रिक्त)
        परत crun_make_error (err, 0, "invalid variant `%s`", मूल्य);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      परत 0;
    }
  अन्यथा जर (*it == '[')
    {
      cleanup_free अक्षर *v_start = रिक्त;
      माप_प्रकार n_parts = 0, parts_size = 32;
      cleanup_free अक्षर **parts = xmalloc (आकार (अक्षर *) * parts_size);
      अक्षर *part;

      part = v_start = xstrdup (it + 1);
      पर्यंत (1)
        {
          अक्षर *out = रिक्त, *next = रिक्त;
          पूर्णांक ret;

          ret = parse_sd_array (part, &out, &next, err);
          जर (UNLIKELY (ret < 0))
            परत ret;

          parts[n_parts++] = out;
          जर (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, आकार (अक्षर *) * parts_size);
            }
          parts[n_parts] = रिक्त;
          जर (next == रिक्त)
            थांबा;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus close container");

      परत 0;
    }
  अन्यथा जर (has_prefix (it, "uint64 "))
    {
      अक्षर *endptr = रिक्त;
      uint64_t v;

      त्रुटि_सं = 0;
      v = strtoull (it + आकार ("uint64"), &endptr, 10);
      जर (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        परत crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      परत 0;
    }
  अन्यथा जर (has_prefix (it, "int64 "))
    {
      अक्षर *endptr = रिक्त;
      int64_t v;

      त्रुटि_सं = 0;
      v = strtoll (it + आकार ("int64"), &endptr, 10);
      जर (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        परत crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      परत 0;
    }
  अन्यथा जर (has_prefix (it, "uint32 "))
    {
      अक्षर *endptr = रिक्त;
      uint32_t v;

      त्रुटि_सं = 0;
      v = म_से_अदीर्घ (it + आकार ("uint32"), &endptr, 10);
      जर (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        परत crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      परत 0;
    }
  अन्यथा जर (has_prefix (it, "int32 ") || म_अक्षर (it, ' ') == रिक्त)
    {
      अक्षर *endptr = रिक्त;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      त्रुटि_सं = 0;
      जर (has_prefix (it, "int32 "))
        v = म_से_दीर्घ (it + आकार ("int32"), &endptr, 10);
      अन्यथा
        v = म_से_दीर्घ (it, &endptr, 10);
      जर (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        परत crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      परत 0;
    }

  परत crun_make_error (err, 0, "unknown type for `%s`", name);
}

स्थिर पूर्णांक
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  पूर्णांक rootless;
  पूर्णांक sd_err = 0;

  rootless = is_rootless (err);
  जर (UNLIKELY (rootless < 0))
    परत rootless;

  जर (rootless)
    sd_err = sd_bus_default_user (bus);
  जर (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  जर (sd_err < 0)
    परत crun_make_error (err, -sd_err, "cannot open sd-bus");

  परत 0;
}

स्थिर स्थिरांक अक्षर *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, स्थिरांक अक्षर *name)
{
  माप_प्रकार i;

  जर (resources == रिक्त || resources->unified == रिक्त)
    परत रिक्त;

  साठी (i = 0; i < resources->unified->len; i++)
    जर (म_भेद (resources->unified->किल्ल्या[i], name) == 0)
      परत resources->unified->मूल्ये[i];
  परत रिक्त;
}

स्थिर पूर्णांक
get_मूल्य_from_unified_map (runtime_spec_schema_config_linux_resources *resources, स्थिरांक अक्षर *name,
                            uint64_t *मूल्य, libcrun_error_t *err)
{
  अक्षर *endptr = रिक्त;
  माप_प्रकार i;

  जर (resources == रिक्त || resources->unified == रिक्त)
    परत 0;

  साठी (i = 0; i < resources->unified->len; i++)
    जर (म_भेद (resources->unified->किल्ल्या[i], name) == 0)
      {
        जर (म_भेद (resources->unified->मूल्ये[i], "max") == 0)
          {
            *मूल्य = UINT64_MAX;
            परत 1;
          }

        त्रुटि_सं = 0;
        *मूल्य = strtoull (resources->unified->मूल्ये[i], &endptr, 10);
        जर (UNLIKELY (त्रुटि_सं))
          परत crun_make_error (err, त्रुटि_सं, "invalid value for `%s`: %s", name,
                                  resources->unified->मूल्ये[i]);
        जर (endptr && *endptr)
          परत crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->मूल्ये[i]);
        परत 1;
      }
  परत 0;
}

स्थिर ओळीत पूर्णांक
get_memory_खालचा (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  जर (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      परत 1;
    }

  परत get_मूल्य_from_unified_map (resources, "memory.low", limit, err);
}

स्थिर ओळीत पूर्णांक
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  जर (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      परत 1;
    }

  परत get_मूल्य_from_unified_map (resources, "pids.max", limit, err);
}

स्थिर ओळीत पूर्णांक
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  जर (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      परत 1;
    }

  परत get_मूल्य_from_unified_map (resources, "memory.max", limit, err);
}

स्थिर ओळीत पूर्णांक
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  जर (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      जर (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          परत 1;
        }
      /* 0 means unset.  */
      जर (memory->swap == 0)
        परत 0;
      जर (mem <= 0)
        परत crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      जर (memory->swap < mem)
        परत crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      परत 1;
    }

  परत get_मूल्य_from_unified_map (resources, "memory.swap.max", limit, err);
}

स्थिर ओळीत पूर्णांक
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t मूल्य;
  पूर्णांक ret;

  ret = get_मूल्य_from_unified_map (resources, "cpu.idle", &मूल्य, err);
  जर (UNLIKELY (ret < 0))
    परत ret;
  जर (ret > 0 && मूल्य == 1)
    has_idle = true;
  जर (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_मूल्य_from_unified_map (resources, "cpu.weight", weight, err);
  जर (UNLIKELY (ret < 0))
    परत ret;
  जर (ret > 0)
    {
      जर (has_idle)
        परत crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      परत 1;
    }

  जर (resources->cpu && resources->cpu->shares_present)
    {
      जर (has_idle)
        परत crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      जर (resources->cpu->shares == 0)
        परत 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      परत 1;
    }
  जर (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      परत 1;
    }
  परत 0;
}

स्थिर पूर्णांक
read_uint64_मूल्य (स्थिरांक अक्षर *str, uint64_t *मूल्य, libcrun_error_t *err)
{
  अक्षर *endptr = रिक्त;

  त्रुटि_सं = 0;
  *मूल्य = strtoull (str, &endptr, 10);
  जर (UNLIKELY (त्रुटि_सं))
    परत crun_make_error (err, त्रुटि_सं, "invalid value: %s", str);
  जर (endptr && *endptr)
    परत crun_make_error (err, 0, "invalid value: %s", str);
  परत 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
स्थिर ओळीत uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  परत bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

स्थिर पूर्णांक
append_io_weight (sd_bus_message *m, अक्षर **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  पूर्णांक sd_err;
  माप_प्रकार i;
  पूर्णांक ret;

#  define APPEND_IO_WEIGHT(मूल्य)                                                      \
    करा                                                                                 \
      {                                                                                \
        जर (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            जर (UNLIKELY (sd_err < 0))                                                 \
              परत crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } पर्यंत (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, मूल्य)                                                      \
    करा                                                                                                \
      {                                                                                               \
        जर (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            जर (UNLIKELY (sd_err < 0))                                                                \
              परत crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } पर्यंत (0)

  जर (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  साठी (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      अक्षर name[64];
      पूर्णांक len;

      len = snprintf (name, आकार (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      जर (UNLIKELY (len >= (पूर्णांक) आकार (name)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  साठी (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free अक्षर *मूल्य = रिक्त;
      bool need_conversion = false;
      अक्षर *saveptr = रिक्त;
      अक्षर *ओळ;

      जर (म_भेद (resources->unified->किल्ल्या[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      अन्यथा जर (म_भेद (resources->unified->किल्ल्या[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      अन्यथा
        {
          /* Skip other keys.  */
          सुरूठेवा;
        }

      मूल्य = xstrdup (resources->unified->मूल्ये[i]);

      साठी (ओळ = strtok_r (मूल्य, "\n", &saveptr); ओळ; ओळ = strtok_r (रिक्त, "\n", &saveptr))
        {
          अक्षर *sep;

          sep = म_अक्षर (ओळ, ' ');
          पर्यंत (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_मूल्य (sep ? sep : ओळ, &weight, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          जर (need_conversion)
            weight = IO_WEIGHT (weight);

          जर (sep == रिक्त || म_भेद (ओळ, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          अन्यथा
            APPEND_IO_DEVICE_WEIGHT (ओळ, weight);
        }
    }

  परत 0;
}

/* Adapted from systemd.  */
स्थिर पूर्णांक
bus_append_byte_array (sd_bus_message *m, स्थिरांक अक्षर *field, स्थिरांक शून्य *buf, माप_प्रकार n, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  जर (ret < 0)
    परत crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  जर (ret < 0)
    परत crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  जर (ret < 0)
    परत crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  जर (ret < 0)
    परत crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  जर (ret < 0)
    परत crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  जर (ret < 0)
    परत crun_make_error (err, -ret, "sd_bus_message_close_container");

  परत 1;
}

स्थिर पूर्णांक
append_uint64_from_unified_map (sd_bus_message *m,
                                अक्षर **missing_properties,
                                स्थिरांक अक्षर *attr,
                                स्थिरांक अक्षर *किल्ली,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t मूल्य;
  पूर्णांक ret;

  जर (property_missing_p (missing_properties, attr))
    परत 0;

  ret = get_मूल्य_from_unified_map (resources, किल्ली, &मूल्य, err);
  जर (UNLIKELY (ret < 0))
    परत ret;
  जर (ret)
    {
      पूर्णांक sd_err = sd_bus_message_append (m, "(sv)", attr, "t", मूल्य);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  परत 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
स्थिर पूर्णांक
append_device_allow (sd_bus_message *m,
                     स्थिरांक अक्षर type,
                     पूर्णांक major,
                     पूर्णांक minor,
                     स्थिरांक अक्षर *access,
                     libcrun_error_t *err)
{
  अक्षर device[64];
  पूर्णांक sd_err;
  पूर्णांक len;

  जर (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      परत 0;
    }

  जर (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, आकार (device), "%s-*", type == 'c' ? "char" : "block");
  अन्यथा जर (IS_WILDCARD (minor))
    len = snprintf (device, आकार (device), type == 'c' ? "char-%d" : "block-%d", major);
  अन्यथा
    len = snprintf (device, आकार (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  जर (UNLIKELY (len >= (पूर्णांक) आकार (device)))
    परत crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  जर (UNLIKELY (sd_err < 0))
    परत crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  परत 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

स्थिर पूर्णांक
access_to_mode (स्थिरांक अक्षर *access, libcrun_error_t *err)
{
  पूर्णांक mode = 0;

  पर्यंत (access && *access)
    {
      पूर्णांक c = *access++;

      निवड (c)
        {
        स्थिती 'r':
          mode |= MODE_R;
          थांबा;

        स्थिती 'w':
          mode |= MODE_W;
          थांबा;

        स्थिती 'm':
          mode |= MODE_M;
          थांबा;

        पूर्वनिर्धारित:
          परत crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  परत mode;
}

/* check if the type b is included in type a.  */
स्थिर bool
has_same_type (स्थिरांक अक्षर *a, स्थिरांक अक्षर *b)
{
  bool a_all = a == रिक्त || म_भेद (a, "a") == 0;
  जर (b == रिक्त || म_भेद (b, "a") == 0)
    परत a_all;

  परत a_all || म_भेद (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
स्थिर पूर्णांक
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, माप_प्रकार n, libcrun_error_t *err)
{
  माप_प्रकार i;
  पूर्णांक req_mode;

#  define CMP_DEV_अंक(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  जर (UNLIKELY (req_mode < 0))
    परत req_mode;

  साठी (i = n; i > 0; i--)
    {
      माप_प्रकार current_rule = i - 1;
      पूर्णांक mode;

      जर (! CMP_DEV_अंक (devices[n]->minor, devices[current_rule]->minor))
        सुरूठेवा;
      जर (! CMP_DEV_अंक (devices[n]->major, devices[current_rule]->major))
        सुरूठेवा;

      जर (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        सुरूठेवा;

      mode = access_to_mode (devices[i]->access, err);
      जर (UNLIKELY (mode < 0))
        परत mode;

      जर (devices[current_rule]->allow && (mode & req_mode))
        परत 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      जर (req_mode == 0)
        परत 1;
    }

  परत 0;
#  undef CMP_DEV_अंक
}

स्थिर माप_प्रकार
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, माप_प्रकार n)
{
  माप_प्रकार i;

  जर (n == 0)
    परत 1;

  /* Find the first rule that is after the last "block all".  */
  साठी (i = n - 1; i-- > 0;)
    {
      जर ((is_empty_string (devices[i]->type) || म_भेद (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        परत i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  साठी (i = 0; i < n; i++)
    जर (devices[i]->allow)
      परत i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  परत n + 1;
}

स्थिर bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, माप_प्रकार n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  पूर्णांक remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  माप_प्रकार i;

  साठी (i = 0; i < n; i++)
    {
      माप_प्रकार current_rule = n - i - 1;
      जर (! devices[current_rule]->allow)
        परत false;

      जर (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          जर (is_empty_string (devices[current_rule]->type) || म_भेद (devices[current_rule]->type, "a") == 0)
            परत true;

          जर (म_भेद (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          अन्यथा जर (म_भेद (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          जर (remaining == 0)
            परत true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  परत false;
}

स्थिर पूर्णांक
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  संरचना default_dev_s *default_devices = get_default_devices ();
  पूर्णांक ret, sd_err;
  माप_प्रकार i;

  जर (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      परत 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  जर (UNLIKELY (sd_err < 0))
    परत crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  साठी (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  साठी (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      अक्षर type;

      जर (! d->allow)
        {
          पूर्णांक redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          जर (UNLIKELY (redundant < 0))
            परत redundant;

          जर (redundant)
            सुरूठेवा;

          परत crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      जर (d->type == रिक्त || म_भेद (d->type, "a") == 0)
        type = 'a';
      अन्यथा जर (म_भेद (d->type, "c") == 0)
        type = 'c';
      अन्यथा जर (म_भेद (d->type, "b") == 0)
        type = 'b';
      अन्यथा
        परत crun_make_error (err, 0, "unknown device type `%s`", d->type);

      जर (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }
      अन्यथा
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }
    }

  परत 0;
}

स्थिर bool
has_bpf_fs ()
{
  संरचना statfs stat;

  परत statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

स्थिर पूर्णांक
bpfprog_path_from_scope (अक्षर **path, स्थिरांक अक्षर *scope, libcrun_error_t *err)
{
  cleanup_free अक्षर *flat_scope = xstrdup (scope);
  अक्षर *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  पर्यंत ((it = म_अक्षर (it, '.')) != रिक्त)
    *it = '_';

  परत append_paths (path, err, CRUN_BPF_DIR, flat_scope, रिक्त);
}

स्थिर पूर्णांक
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 अक्षर **missing_properties,
                 स्थिरांक अक्षर *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  पूर्णांक ret = 0;
  पूर्णांक in_userns, sd_err;
  स्थिरांक अक्षर *property_name = "BPFProgram";
  cleanup_free संरचना bpf_program *program = रिक्त;
  cleanup_free अक्षर *path = रिक्त;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  जर (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    परत 0;

  in_userns = check_running_in_user_namespace (err);
  जर (UNLIKELY (in_userns < 0))
    परत in_userns;

  जर (in_userns)
    परत 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  जर (UNLIKELY (program == रिक्त))
    {
      crun_error_release (err);
      परत 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = सूची_गढ़ो (CRUN_BPF_DIR, 0700);
  जर (UNLIKELY (ret < 0 && त्रुटि_सं != EEXIST))
    परत crun_make_error (err, त्रुटि_सं, "mkdir " CRUN_BPF_DIR);

  जर (is_update)
    {
      cleanup_free संरचना bpf_program *program_loaded = रिक्त;

      जर (resources->devices == रिक्त || resources->devices_len == 0)
        {
          *devices_set = true;
          परत 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      /* it is the same as the loaded program.  */
      जर (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          परत 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free संरचना bpf_program *program_new = रिक्त;
        अक्षर tmp[64];
        पूर्णांक len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, आकार (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        जर (UNLIKELY (len >= (पूर्णांक) आकार (tmp)))
          परत crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        जर (UNLIKELY (ret < 0))
          परत ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        जर (UNLIKELY (ret < 0))
          परत ret;

        जर (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            परत 0;
          }
      }

      परत crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  जर (UNLIKELY (sd_err < 0))
    परत crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  परत 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
स्थिर पूर्णांक
append_cpu_quota (sd_bus_message *m, अक्षर **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  स्थिरांक अक्षर *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  पूर्णांक sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  जर (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      पूर्णांक ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
      जर (has_period)
        period = max_period;
    }

  जर (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  जर ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      जर (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          जर (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      जर (UNLIKELY (sd_err < 0))
        परत crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  परत 0;
}

स्थिर पूर्णांक
append_resources (sd_bus_message *m,
                  bool is_update,
                  स्थिरांक अक्षर *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  पूर्णांक cgroup_mode,
                  स्थिरांक अक्षर *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t मूल्य;
  पूर्णांक sd_err;
  पूर्णांक ret;
  cleanup_free अक्षर *dir = रिक्त;
  cleanup_free अक्षर **missing_properties = रिक्त;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  missing_properties = read_dir_entries (dir, err);
  जर (UNLIKELY (missing_properties == रिक्त))
    {
      जर (crun_error_get_errno (err) != ENOENT)
        परत -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_मूल्य(name, मूल्य)                                             \
    करा                                                                                 \
      {                                                                                \
        जर (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", मूल्य);              \
            जर (UNLIKELY (sd_err < 0))                                                 \
              परत crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } पर्यंत (0)

#  define APPEND_UINT64(name, fn)              \
    करा                                         \
      {                                        \
        ret = fn (resources, &मूल्य, err);     \
        जर (UNLIKELY (ret < 0))                \
          परत ret;                          \
        जर (ret)                               \
          {                                    \
            APPEND_UINT64_मूल्य (name, मूल्य); \
          }                                    \
    } पर्यंत (0)

  जर (resources == रिक्त)
    परत 0;

  APPEND_UINT64 ("MemoryLow", get_memory_खालचा);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  निवड (cgroup_mode)
    {
    स्थिती CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        जर (UNLIKELY (ret < 0))
          परत ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        जर (UNLIKELY (ret < 0))
          परत ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        जर (UNLIKELY (ret < 0))
          परत ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        जर (UNLIKELY (ret < 0))
          परत ret;

        जर (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            स्थिरांक अक्षर *property_name = "AllowedCPUs";
            स्थिरांक अक्षर *मूल्य = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free अक्षर *allowed_cpus = रिक्त;
            माप_प्रकार allowed_cpus_len = 0;

            जर (मूल्य == रिक्त)
              मूल्य = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (मूल्य, &allowed_cpus, &allowed_cpus_len, err);
            जर (UNLIKELY (ret < 0))
              परत ret;

            जर (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                जर (UNLIKELY (ret < 0))
                  परत ret;
              }
          }

        जर (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            स्थिरांक अक्षर *property_name = "AllowedMemoryNodes";
            स्थिरांक अक्षर *मूल्य = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free अक्षर *allowed_mems = रिक्त;
            माप_प्रकार allowed_mems_len = 0;

            जर (मूल्य == रिक्त)
              मूल्य = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (मूल्य, &allowed_mems, &allowed_mems_len, err);
            जर (UNLIKELY (ret < 0))
              परत ret;

            जर (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                जर (UNLIKELY (ret < 0))
                  परत ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        जर (UNLIKELY (ret < 0))
          परत ret;
      }
      थांबा;

    स्थिती CGROUP_MODE_LEGACY:
    स्थिती CGROUP_MODE_HYBRID:
      जर (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_मूल्य ("CPUShares", resources->cpu->shares);
      थांबा;

    पूर्वनिर्धारित:
      परत crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_मूल्य

  जर (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  परत 0;
}

स्थिर पूर्णांक
reset_failed_unit (sd_bus *bus, स्थिरांक अक्षर *unit)
{
  पूर्णांक sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = रिक्त, *reply = रिक्त;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  जर (UNLIKELY (sd_err < 0))
    जा निकास;

  sd_err = sd_bus_message_append (m, "s", unit);
  जर (UNLIKELY (sd_err < 0))
    जा निकास;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  जर (UNLIKELY (sd_err < 0))
    जा निकास;

  sd_err = 0;

निकास:
  जर (m)
    sd_bus_message_unref (m);
  जर (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  परत sd_err;
}

स्थिर पूर्णांक
verify_ebpf_device_filter_installed (स्थिरांक अक्षर *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = रिक्त;
  cleanup_free अक्षर *full_path = रिक्त;
  माप_प्रकार n_progs = 0;
  पूर्णांक ret;

  ret = append_paths (&full_path, err, CGROUP_मूळ, cgroup_path, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (n_progs == 0)
    परत crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  परत 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
स्थिर पूर्णांक
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, पूर्णांक ret)
{
  जर (bus)
    sd_bus_unref (bus);
  जर (m)
    sd_bus_message_unref (m);
  जर (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  परत ret;
}

स्थिर पूर्णांक
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            पूर्णांक cgroup_mode,
                            string_map *annotations,
                            स्थिरांक अक्षर *state_मूळ,
                            स्थिरांक अक्षर *scope, स्थिरांक अक्षर *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = रिक्त;
  sd_bus_message *m = रिक्त;
  sd_bus_message *reply = रिक्त;
  पूर्णांक sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  स्थिरांक अक्षर *object = रिक्त;
  संरचना systemd_job_removed_s job_data = {};
  पूर्णांक i;
  स्थिरांक अक्षर *boolean_opts[10];
  cleanup_free अक्षर *state_dir = रिक्त;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_मूळ, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

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
  जर (UNLIKELY (ret < 0))
    जा निकास;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  जर (UNLIKELY (ret < 0))
    जा निकास;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      जा निकास;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      जा निकास;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      जा निकास;
    }

  जर (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      जर (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          जा निकास;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      जा निकास;
    }

  जर (annotations)
    {
      माप_प्रकार prefix_len = आकार (SYSTEMD_PROPERTY_PREFIX) - 1;
      माप_प्रकार annotations_len = string_map_size (annotations);
      माप_प्रकार i;

      साठी (i = 0; i < annotations_len; i++)
        {
          स्थिरांक अक्षर *किल्ली;
          स्थिरांक अक्षर *मूल्य;
          माप_प्रकार len;

          ret = string_map_get_at (annotations, i, &किल्ली, &मूल्य);
          जर (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              जा निकास;
            }

          जर (! has_prefix (किल्ली, SYSTEMD_PROPERTY_PREFIX))
            सुरूठेवा;

          len = म_माप (किल्ली);
          जर (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", किल्ली);
              जा निकास;
            }

          ret = append_systemd_annotation (m, किल्ली + prefix_len, len - prefix_len,
                                           मूल्य, err);
          जर (UNLIKELY (ret < 0))
            जा निकास;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      जा निकास;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      जा निकास;
    }

  साठी (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      जर (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          जा निकास;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  जर (UNLIKELY (ret < 0))
    जा निकास;

  sd_err = sd_bus_message_close_container (m);
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      जा निकास;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      जा निकास;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  जर (UNLIKELY (sd_err < 0))
    {
      जर (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          जर (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = रिक्त;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      जर (sd_err < 0)
        {
          जर (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              जर (UNLIKELY (ret < 0))
                जा निकास;
              जर (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          जा निकास;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      जा निकास;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

निकास:
  परत cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

स्थिर पूर्णांक
libcrun_destroy_systemd_cgroup_scope (संरचना libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = रिक्त;
  sd_bus_message *m = रिक्त;
  sd_bus_message *reply = रिक्त;
  पूर्णांक ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  स्थिरांक अक्षर *object;
  स्थिरांक अक्षर *scope = cgroup_status->scope;
  संरचना systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  जर (UNLIKELY (ret < 0))
    जा निकास;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  जर (UNLIKELY (ret < 0))
    जा निकास;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  जर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      जा निकास;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  जर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      जा निकास;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  जर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      जा निकास;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  जर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      जा निकास;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

निकास:
  परत cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

स्थिर स्थिरांक अक्षर *
find_systemd_subgroup (string_map *annotations)
{
  स्थिरांक अक्षर *annotation;

  annotation = find_string_map_मूल्य (annotations, "run.oci.systemd.subgroup");
  जर (annotation)
    {
      जर (annotation[0] == '\0')
        परत रिक्त;
      परत annotation;
    }

  परत "container";
}

स्थिर पूर्णांक
libcrun_cgroup_enter_systemd (संरचना libcrun_cgroup_args *args,
                              संरचना libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  स्थिरांक अक्षर *cgroup_path = args->cgroup_path;
  cleanup_free अक्षर *scope_path = रिक्त;
  cleanup_free अक्षर *scope = रिक्त;
  cleanup_free अक्षर *path = रिक्त;
  cleanup_free अक्षर *slice = रिक्त;
  पूर्णांक retries_डावा = 32;
  स्थिरांक अक्षर *suffix;
  स्थिरांक अक्षर *id = args->id;
  pid_t pid = args->pid;
  पूर्णांक cgroup_mode;
  पूर्णांक rootless = 0;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (cgroup_mode < 0))
    परत cgroup_mode;

  जर (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      जर (UNLIKELY (rootless < 0))
        परत rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  साठी (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_मूळ,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      जर (LIKELY (ret >= 0))
        थांबा;

      जर (can_retry && retries_डावा-- > 0)
        {
          crun_error_release (err);
          सुरूठेवा;
        }

      परत ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  जर (is_empty_string (suffix))
    scope_path = xstrdup (path);
  अन्यथा
    {
      माप_प्रकार path_len = म_माप (path);
      माप_प्रकार suffix_len = म_माप (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      जर (UNLIKELY (scope_path == रिक्त))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  जर (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  out->path = path;
  path = रिक्त;

  out->scope = scope;
  scope = रिक्त;
  परत 0;
}

अक्षर *
get_cgroup_scope_path (स्थिरांक अक्षर *cgroup_path, स्थिरांक अक्षर *scope)
{
  अक्षर *path_to_scope = रिक्त;
  अक्षर *cur;

  जर (is_empty_string (cgroup_path))
    परत रिक्त;

  path_to_scope = xstrdup (cgroup_path);

  cur = म_अक्षर (path_to_scope, '/');
  पर्यंत (cur)
    {
      अक्षर *next = म_अक्षर (cur + 1, '/');
      जर (next == रिक्त)
        थांबा;

      *next = '\0';
      जर (म_भेद (cur, scope) == 0)
        परत path_to_scope;
      *next = '/';

      cur = next;
      पर्यंत (*cur == '/')
        cur++;
    }

  परत path_to_scope;
}

स्थिर पूर्णांक
libcrun_destroy_cgroup_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free अक्षर *path_to_scope = रिक्त;
  cleanup_free अक्षर *bpfprog = रिक्त;
  पूर्णांक mode;
  पूर्णांक ret;

  mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (mode < 0))
    परत mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  जर (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  जर (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  जर (UNLIKELY (ret < 0))
    crun_error_release (err);
  अन्यथा
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  परत destroy_cgroup_path (path_to_scope, mode, err);
}

स्थिर पूर्णांक
libcrun_update_resources_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                  स्थिरांक अक्षर *state_मूळ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  संरचना systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free अक्षर *state_dir = रिक्त;
  sd_bus_message *reply = रिक्त;
  sd_bus_message *m = रिक्त;
  sd_bus *bus = रिक्त;
  पूर्णांक sd_err, ret;
  पूर्णांक cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_मूळ, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (cgroup_mode < 0))
    परत cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  जर (UNLIKELY (ret < 0))
    जा निकास;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      जा निकास;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      जा निकास;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      जा निकास;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  जर (UNLIKELY (ret < 0))
    जा निकास;

  sd_err = sd_bus_message_close_container (m);
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      जा निकास;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  जर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      जा निकास;
    }

  जर (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      जर (UNLIKELY (ret < 0))
        जा निकास;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      जर (UNLIKELY (ret < 0))
        जा निकास;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  जर (UNLIKELY (ret < 0))
    जा निकास;

  ret = 0;

निकास:
  परत cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
स्थिर पूर्णांक
libcrun_attach_process_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                स्थिरांक अक्षर *path, pid_t pid,
                                libcrun_error_t *err)
{
  स्थिरांक अक्षर *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free अक्षर *scope_path = रिक्त;
  sd_bus_message *reply = रिक्त;
  स्थिरांक अक्षर *subcgroup;
  स्थिरांक अक्षर *last;
  sd_bus *bus = रिक्त;
  माप_प्रकार len;
  पूर्णांक sd_err, ret;

  जर (is_empty_string (scope))
    परत crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  जर (scope_path == रिक्त)
    परत crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = म_खोजप (scope_path, '/');
  जर (last == रिक्त || म_भेद (last + 1, scope) != 0)
    परत crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = म_माप (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  जर (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  परत cleanup_sd_bus_and_return (bus, रिक्त, reply, &error, ret);
}

#अन्यथा
स्थिर पूर्णांक
libcrun_cgroup_enter_systemd (संरचना libcrun_cgroup_args *args,
                              संरचना libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (शून्य) args;
  (शून्य) out;

  परत crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णांक
libcrun_destroy_cgroup_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (शून्य) cgroup_status;

  परत crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णांक
libcrun_update_resources_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                  स्थिरांक अक्षर *state_मूळ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (शून्य) cgroup_status;
  (शून्य) state_मूळ;
  (शून्य) resources;

  परत crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णांक
libcrun_attach_process_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                स्थिरांक अक्षर *path, pid_t pid,
                                libcrun_error_t *err)
{
  (शून्य) cgroup_status;
  (शून्य) path;
  (शून्य) pid;

  परत crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

संरचना libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = रिक्त,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
