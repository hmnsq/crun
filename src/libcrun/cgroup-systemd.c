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
register_missing_property_from_message (अचर वर्ण *state_dir, अचर वर्ण *message, libcrun_error_t *err)
{
  cleanup_free वर्ण *file_path = रिक्त;
  cleanup_free वर्ण *dir_path = रिक्त;
  cleanup_free वर्ण *property = रिक्त;
  वर्ण *p;
  पूर्णाङ्क ret;

  यदि (! has_prefix (message, "Cannot set property "))
    फर्काऊ 0;

  property = xstrdup (message + परिमाण ("Cannot set property ") - 1);
  p = म_अक्षर (property, ',');
  यदि (! p)
    फर्काऊ 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = append_paths (&file_path, err, dir_path, property, रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, रिक्त, 0, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ 1;
}

स्थिर bool
property_missing_p (वर्ण **missing_properties, अचर वर्ण *property)
{
  माप_प्रकार i;

  लागि (i = 0; missing_properties && missing_properties[i]; i++)
    यदि (म_भेद (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        फर्काऊ true;
      }

  फर्काऊ false;
}

स्थिर शून्य
get_systemd_scope_and_slice (अचर वर्ण *id, bool user_slice, अचर वर्ण *cgroup_path,
                             वर्ण **scope, वर्ण **slice)
{
  अचर वर्ण *n;
  वर्ण *p;

  यदि (cgroup_path == रिक्त || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      फर्काऊ;
    }

  n = म_अक्षर (cgroup_path, ':');
  यदि (n == रिक्त)
    xasprintf (scope, "%s.scope", cgroup_path);
  अन्यथा
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = म_अक्षर (*scope, ':');
      यदि (p)
        *p = '-';
    }
  यदि (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = म_अक्षर (*slice, ':');
      यदि (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      यदि (is_empty_string (*slice))
        {
          मुक्त (*slice);
          यदि (user_slice)
            *slice = xstrdup ("user.slice");
          अन्यथा
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
स्थिर पूर्णाङ्क
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  अचर वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path = रिक्त;
  cleanup_close पूर्णाङ्क dirfd = -1;
  bool need_set_parent = true;
  वर्ण fmt_buf[64];
  पूर्णाङ्क len;
  पूर्णाङ्क ret;

  यदि (resources == रिक्त || resources->cpu == रिक्त)
    फर्काऊ 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  यदि (is_empty_string (path))
    फर्काऊ crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  यदि (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_जरा, "cpu", path, रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  यदि (UNLIKELY (dirfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  यदि (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

      यदि (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  यदि (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, परिमाण (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (fmt_buf)))
        फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

      यदि (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, अचर वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path = रिक्त;
  पूर्णाङ्क parent;
  पूर्णाङ्क ret;

  यदि (resources == रिक्त || resources->cpu == रिक्त)
    फर्काऊ 0;

  यदि (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    फर्काऊ 0;

  लागि (parent = 0; parent < 2; parent++)
    {
      cleanup_close पूर्णाङ्क dirfd = -1;

      यदि (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_जरा, path ? path : "", (parent ? ".." : रिक्त), रिक्त);
      अन्यथा
        ret = append_paths (&cgroup_path, err, CGROUP_जरा, "/cpu", path ? path : "", (parent ? ".." : रिक्त), रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      यदि (UNLIKELY (dirfd < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      यदि (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
    }

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, अचर वर्ण *path, libcrun_error_t *err)
{
  cleanup_free वर्ण *cgroup_path = रिक्त;
  पूर्णाङ्क parent;
  पूर्णाङ्क ret;
  पूर्णाङ्क levels;

  ret = append_paths (&cgroup_path, err, CGROUP_जरा, "/cpuset", path ? path : "", रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  लागि (parent = 0; parent < levels; parent++)
    {
      cleanup_close पूर्णाङ्क dirfd_cpuset = -1;
      cleanup_free वर्ण *path_to_cpuset = रिक्त;

      ret = append_paths (&path_to_cpuset, err, CGROUP_जरा "/cpuset", path, (parent ? ".." : रिक्त), रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      dirfd_cpuset = खोलो (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      यदि (UNLIKELY (dirfd_cpuset < 0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  फर्काऊ 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
स्थिर पूर्णाङ्क
compute_finalized_path (वर्ण **path, अचर वर्ण *from, अचर वर्ण *suffix, libcrun_error_t *err)
{
  यदि (suffix == रिक्त)
    {
      *path = xstrdup (from);
      फर्काऊ 0;
    }
  फर्काऊ append_paths (path, err, from, suffix, रिक्त);
}

स्थिर पूर्णाङ्क
systemd_finalize (संरचना libcrun_cgroup_args *args, वर्ण **path_out,
                  पूर्णाङ्क cgroup_mode, अचर वर्ण *suffix, libcrun_error_t *err)
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
  यदि (UNLIKELY (fd < 0))
    फर्काऊ fd;

  ret = read_all_fd (fd, "cgroup path", &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  विकल्प (cgroup_mode)
    {
    पक्ष CGROUP_MODE_HYBRID:
    पक्ष CGROUP_MODE_LEGACY:
      from = म_माला (content, ":memory:");
      यदि (LIKELY (from != रिक्त))
        from += 8;
      अन्यथा
        {
          from = म_माला (content, ":pids:");
          यदि (UNLIKELY (from == रिक्त))
            फर्काऊ crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = म_अक्षर (from, '\n');
      यदि (UNLIKELY (to == रिक्त))
        फर्काऊ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      यदि (geteuid ())
        फर्काऊ 0;

      ret = setup_rt_runtime (resources, path, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      लागि (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (रिक्त, "\n", &saveptr))
        {
          वर्ण *subpath, *subsystem;
          subsystem = म_अक्षर (from, ':') + 1;
          subpath = म_अक्षर (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          यदि (subsystem[0] == '\0')
            {
              यदि (cgroup_mode == CGROUP_MODE_LEGACY)
                जारी;

              subsystem = "unified";
            }

          यदि (म_भेद (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              यदि (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  यदि (म_अक्षर (subsystem, '='))
                    {
                      crun_error_release (err);
                      जारी;
                    }
                  फर्काऊ ret;
                }
            }
        }

      रोक;

    पक्ष CGROUP_MODE_UNIFIED:
      {
        cleanup_free वर्ण *dir = रिक्त;

        from = म_माला (content, "0::");
        यदि (UNLIKELY (from == रिक्त))
          फर्काऊ crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = म_अक्षर (from, '\n');
        यदि (UNLIKELY (to == रिक्त))
          फर्काऊ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        ret = append_paths (&dir, err, CGROUP_जरा, path, रिक्त);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        ret = move_process_to_cgroup (pid, रिक्त, path, err);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        ret = enable_controllers (path, err);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        यदि (suffix)
          {
            ret = chown_cgroups (path, args->जरा_uid, args->जरा_gid, err);
            यदि (UNLIKELY (ret < 0))
              फर्काऊ ret;
          }
      }
      रोक;

    स्वभाव:
      फर्काऊ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  *path_out = path;
  path = रिक्त;

  फर्काऊ 0;
}

संरचना systemd_job_removed_s
{
  अचर वर्ण *path;
  अचर वर्ण *op;
  पूर्णाङ्क terminated;
  libcrun_error_t err;
};

स्थिर पूर्णाङ्क
systemd_job_removed (sd_bus_message *m, शून्य *userdata, sd_bus_error *error arg_unused)
{
  अचर वर्ण *path, *unit, *result;
  uint32_t id;
  पूर्णाङ्क ret;
  संरचना systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  यदि (ret < 0)
    फर्काऊ -1;

  यदि (म_भेद (d->path, path) == 0)
    {
      d->terminated = 1;
      यदि (म_भेद (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          फर्काऊ -1;
        }
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
systemd_check_job_status_setup (sd_bus *bus, संरचना systemd_job_removed_s *data, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = sd_bus_match_signal_async (bus, रिक्त, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, रिक्त, data);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, -ret, "sd-bus match signal");

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
systemd_check_job_status (sd_bus *bus, संरचना systemd_job_removed_s *data, अचर वर्ण *path, अचर वर्ण *op,
                          libcrun_error_t *err)
{
  पूर्णाङ्क sd_err;

  data->path = path;
  data->op = op;
  जबसम्म (! data->terminated)
    {
      sd_err = sd_bus_process (bus, रिक्त);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus process");

      यदि (sd_err != 0)
        जारी;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus wait");
    }

  यदि (data->err != रिक्त)
    {
      *err = data->err;
      फर्काऊ -1;
    }

  फर्काऊ 0;
}

पूर्णाङ्क
parse_sd_array (वर्ण *s, वर्ण **out, वर्ण **next, libcrun_error_t *err)
{
  वर्ण endchr;
  वर्ण *it, *dest;
  bool escaped = false;

  *out = रिक्त;
  *next = रिक्त;

  जबसम्म (है_खाली (*s))
    s++;
  यदि (*s == '\0')
    फर्काऊ 0;
  अन्यथा यदि (*s != '\'' && *s != '"')
    फर्काऊ crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  जबसम्म (1)
    {
      यदि (*it == '\0')
        फर्काऊ crun_make_error (err, 0, "invalid string `%s`", s);
      यदि (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          जबसम्म (है_खाली (*it))
            it++;
          यदि (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              फर्काऊ 0;
            }

          यदि (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              फर्काऊ 0;
            }

          फर्काऊ crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      यदि (! escaped)
        *dest++ = *it;
      it++;
    }

  फर्काऊ 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
स्थिर पूर्णाङ्क
append_systemd_annotation (sd_bus_message *m, अचर वर्ण *name, माप_प्रकार name_len, अचर वर्ण *मान,
                           libcrun_error_t *err)
{
  cleanup_free वर्ण *tmp_name = रिक्त;
  uint32_t factor = 1;
  अचर वर्ण *it;
  पूर्णाङ्क sd_err;

  जबसम्म (*मान == ' ')
    मान++;

  it = मान;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  यदि (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  यदि (factor != 1)
    {
      स्थिर अचर वर्ण *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", रिक्त };
      अचर वर्ण *अङ्क = it;
      वर्ण *endptr = रिक्त;
      द्विगुण v;
      माप_प्रकार i;

      लागि (i = 0; types[i]; i++)
        यदि (has_prefix (अङ्क, types[i]))
          {
            अङ्क += म_माप (types[i]);
            रोक;
          }

      त्रुटि_सं = 0;
      v = म_से_भग्न (अङ्क, &endptr);
      यदि (UNLIKELY (त्रुटि_सं != 0 || endptr == अङ्क || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "invalid value for `%s`: `%s`", name, मान);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      फर्काऊ 0;
    }

  यदि ((म_भेद (it, "true") == 0) || (म_भेद (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      फर्काऊ 0;
    }
  अन्यथा यदि (*it == '\'')
    {
      cleanup_free वर्ण *v_start = रिक्त;
      वर्ण *end;

      it = v_start = xstrdup (मान);

      end = म_अक्षर (v_start + 1, '\'');
      यदि (end == रिक्त)
        फर्काऊ crun_make_error (err, 0, "invalid variant `%s`", मान);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      फर्काऊ 0;
    }
  अन्यथा यदि (*it == '[')
    {
      cleanup_free वर्ण *v_start = रिक्त;
      माप_प्रकार n_parts = 0, parts_size = 32;
      cleanup_free वर्ण **parts = xmalloc (परिमाण (वर्ण *) * parts_size);
      वर्ण *part;

      part = v_start = xstrdup (it + 1);
      जबसम्म (1)
        {
          वर्ण *out = रिक्त, *next = रिक्त;
          पूर्णाङ्क ret;

          ret = parse_sd_array (part, &out, &next, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          parts[n_parts++] = out;
          यदि (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, परिमाण (वर्ण *) * parts_size);
            }
          parts[n_parts] = रिक्त;
          यदि (next == रिक्त)
            रोक;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus close container");

      फर्काऊ 0;
    }
  अन्यथा यदि (has_prefix (it, "uint64 "))
    {
      वर्ण *endptr = रिक्त;
      uint64_t v;

      त्रुटि_सं = 0;
      v = strtoull (it + परिमाण ("uint64"), &endptr, 10);
      यदि (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      फर्काऊ 0;
    }
  अन्यथा यदि (has_prefix (it, "int64 "))
    {
      वर्ण *endptr = रिक्त;
      int64_t v;

      त्रुटि_सं = 0;
      v = strtoll (it + परिमाण ("int64"), &endptr, 10);
      यदि (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      फर्काऊ 0;
    }
  अन्यथा यदि (has_prefix (it, "uint32 "))
    {
      वर्ण *endptr = रिक्त;
      uint32_t v;

      त्रुटि_सं = 0;
      v = म_से_अदीर्घ (it + परिमाण ("uint32"), &endptr, 10);
      यदि (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      फर्काऊ 0;
    }
  अन्यथा यदि (has_prefix (it, "int32 ") || म_अक्षर (it, ' ') == रिक्त)
    {
      वर्ण *endptr = रिक्त;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      त्रुटि_सं = 0;
      यदि (has_prefix (it, "int32 "))
        v = म_से_दीर्घ (it + परिमाण ("int32"), &endptr, 10);
      अन्यथा
        v = म_से_दीर्घ (it, &endptr, 10);
      यदि (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      फर्काऊ 0;
    }

  फर्काऊ crun_make_error (err, 0, "unknown type for `%s`", name);
}

स्थिर पूर्णाङ्क
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  पूर्णाङ्क rootless;
  पूर्णाङ्क sd_err = 0;

  rootless = is_rootless (err);
  यदि (UNLIKELY (rootless < 0))
    फर्काऊ rootless;

  यदि (rootless)
    sd_err = sd_bus_default_user (bus);
  यदि (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  यदि (sd_err < 0)
    फर्काऊ crun_make_error (err, -sd_err, "cannot open sd-bus");

  फर्काऊ 0;
}

स्थिर अचर वर्ण *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, अचर वर्ण *name)
{
  माप_प्रकार i;

  यदि (resources == रिक्त || resources->unified == रिक्त)
    फर्काऊ रिक्त;

  लागि (i = 0; i < resources->unified->len; i++)
    यदि (म_भेद (resources->unified->साँचोहरू[i], name) == 0)
      फर्काऊ resources->unified->मानहरू[i];
  फर्काऊ रिक्त;
}

स्थिर पूर्णाङ्क
get_मान_from_unified_map (runtime_spec_schema_config_linux_resources *resources, अचर वर्ण *name,
                            uint64_t *मान, libcrun_error_t *err)
{
  वर्ण *endptr = रिक्त;
  माप_प्रकार i;

  यदि (resources == रिक्त || resources->unified == रिक्त)
    फर्काऊ 0;

  लागि (i = 0; i < resources->unified->len; i++)
    यदि (म_भेद (resources->unified->साँचोहरू[i], name) == 0)
      {
        यदि (म_भेद (resources->unified->मानहरू[i], "max") == 0)
          {
            *मान = UINT64_MAX;
            फर्काऊ 1;
          }

        त्रुटि_सं = 0;
        *मान = strtoull (resources->unified->मानहरू[i], &endptr, 10);
        यदि (UNLIKELY (त्रुटि_सं))
          फर्काऊ crun_make_error (err, त्रुटि_सं, "invalid value for `%s`: %s", name,
                                  resources->unified->मानहरू[i]);
        यदि (endptr && *endptr)
          फर्काऊ crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->मानहरू[i]);
        फर्काऊ 1;
      }
  फर्काऊ 0;
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_memory_तल्लो (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  यदि (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      फर्काऊ 1;
    }

  फर्काऊ get_मान_from_unified_map (resources, "memory.low", limit, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  यदि (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      फर्काऊ 1;
    }

  फर्काऊ get_मान_from_unified_map (resources, "pids.max", limit, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  यदि (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      फर्काऊ 1;
    }

  फर्काऊ get_मान_from_unified_map (resources, "memory.max", limit, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  यदि (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      यदि (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          फर्काऊ 1;
        }
      /* 0 means unset.  */
      यदि (memory->swap == 0)
        फर्काऊ 0;
      यदि (mem <= 0)
        फर्काऊ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      यदि (memory->swap < mem)
        फर्काऊ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      फर्काऊ 1;
    }

  फर्काऊ get_मान_from_unified_map (resources, "memory.swap.max", limit, err);
}

स्थिर अन्तःस्थ पूर्णाङ्क
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t मान;
  पूर्णाङ्क ret;

  ret = get_मान_from_unified_map (resources, "cpu.idle", &मान, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;
  यदि (ret > 0 && मान == 1)
    has_idle = true;
  यदि (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_मान_from_unified_map (resources, "cpu.weight", weight, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;
  यदि (ret > 0)
    {
      यदि (has_idle)
        फर्काऊ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      फर्काऊ 1;
    }

  यदि (resources->cpu && resources->cpu->shares_present)
    {
      यदि (has_idle)
        फर्काऊ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      यदि (resources->cpu->shares == 0)
        फर्काऊ 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      फर्काऊ 1;
    }
  यदि (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      फर्काऊ 1;
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
read_uint64_मान (अचर वर्ण *str, uint64_t *मान, libcrun_error_t *err)
{
  वर्ण *endptr = रिक्त;

  त्रुटि_सं = 0;
  *मान = strtoull (str, &endptr, 10);
  यदि (UNLIKELY (त्रुटि_सं))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "invalid value: %s", str);
  यदि (endptr && *endptr)
    फर्काऊ crun_make_error (err, 0, "invalid value: %s", str);
  फर्काऊ 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
स्थिर अन्तःस्थ uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  फर्काऊ bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

स्थिर पूर्णाङ्क
append_io_weight (sd_bus_message *m, वर्ण **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  पूर्णाङ्क sd_err;
  माप_प्रकार i;
  पूर्णाङ्क ret;

#  define APPEND_IO_WEIGHT(मान)                                                      \
    गर                                                                                 \
      {                                                                                \
        यदि (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            यदि (UNLIKELY (sd_err < 0))                                                 \
              फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } जबसम्म (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, मान)                                                      \
    गर                                                                                                \
      {                                                                                               \
        यदि (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            यदि (UNLIKELY (sd_err < 0))                                                                \
              फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } जबसम्म (0)

  यदि (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  लागि (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      वर्ण name[64];
      पूर्णाङ्क len;

      len = snprintf (name, परिमाण (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (name)))
        फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  लागि (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free वर्ण *मान = रिक्त;
      bool need_conversion = false;
      वर्ण *saveptr = रिक्त;
      वर्ण *पङ्क्ति;

      यदि (म_भेद (resources->unified->साँचोहरू[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      अन्यथा यदि (म_भेद (resources->unified->साँचोहरू[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      अन्यथा
        {
          /* Skip other keys.  */
          जारी;
        }

      मान = xstrdup (resources->unified->मानहरू[i]);

      लागि (पङ्क्ति = strtok_r (मान, "\n", &saveptr); पङ्क्ति; पङ्क्ति = strtok_r (रिक्त, "\n", &saveptr))
        {
          वर्ण *sep;

          sep = म_अक्षर (पङ्क्ति, ' ');
          जबसम्म (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_मान (sep ? sep : पङ्क्ति, &weight, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
          यदि (need_conversion)
            weight = IO_WEIGHT (weight);

          यदि (sep == रिक्त || म_भेद (पङ्क्ति, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          अन्यथा
            APPEND_IO_DEVICE_WEIGHT (पङ्क्ति, weight);
        }
    }

  फर्काऊ 0;
}

/* Adapted from systemd.  */
स्थिर पूर्णाङ्क
bus_append_byte_array (sd_bus_message *m, अचर वर्ण *field, अचर शून्य *buf, माप_प्रकार n, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  यदि (ret < 0)
    फर्काऊ crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  यदि (ret < 0)
    फर्काऊ crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  यदि (ret < 0)
    फर्काऊ crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  यदि (ret < 0)
    फर्काऊ crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  यदि (ret < 0)
    फर्काऊ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  यदि (ret < 0)
    फर्काऊ crun_make_error (err, -ret, "sd_bus_message_close_container");

  फर्काऊ 1;
}

स्थिर पूर्णाङ्क
append_uint64_from_unified_map (sd_bus_message *m,
                                वर्ण **missing_properties,
                                अचर वर्ण *attr,
                                अचर वर्ण *साँचो,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t मान;
  पूर्णाङ्क ret;

  यदि (property_missing_p (missing_properties, attr))
    फर्काऊ 0;

  ret = get_मान_from_unified_map (resources, साँचो, &मान, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;
  यदि (ret)
    {
      पूर्णाङ्क sd_err = sd_bus_message_append (m, "(sv)", attr, "t", मान);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  फर्काऊ 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
स्थिर पूर्णाङ्क
append_device_allow (sd_bus_message *m,
                     अचर वर्ण type,
                     पूर्णाङ्क major,
                     पूर्णाङ्क minor,
                     अचर वर्ण *access,
                     libcrun_error_t *err)
{
  वर्ण device[64];
  पूर्णाङ्क sd_err;
  पूर्णाङ्क len;

  यदि (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      फर्काऊ 0;
    }

  यदि (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, परिमाण (device), "%s-*", type == 'c' ? "char" : "block");
  अन्यथा यदि (IS_WILDCARD (minor))
    len = snprintf (device, परिमाण (device), type == 'c' ? "char-%d" : "block-%d", major);
  अन्यथा
    len = snprintf (device, परिमाण (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (device)))
    फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  यदि (UNLIKELY (sd_err < 0))
    फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  फर्काऊ 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

स्थिर पूर्णाङ्क
access_to_mode (अचर वर्ण *access, libcrun_error_t *err)
{
  पूर्णाङ्क mode = 0;

  जबसम्म (access && *access)
    {
      पूर्णाङ्क c = *access++;

      विकल्प (c)
        {
        पक्ष 'r':
          mode |= MODE_R;
          रोक;

        पक्ष 'w':
          mode |= MODE_W;
          रोक;

        पक्ष 'm':
          mode |= MODE_M;
          रोक;

        स्वभाव:
          फर्काऊ crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  फर्काऊ mode;
}

/* check if the type b is included in type a.  */
स्थिर bool
has_same_type (अचर वर्ण *a, अचर वर्ण *b)
{
  bool a_all = a == रिक्त || म_भेद (a, "a") == 0;
  यदि (b == रिक्त || म_भेद (b, "a") == 0)
    फर्काऊ a_all;

  फर्काऊ a_all || म_भेद (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
स्थिर पूर्णाङ्क
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, माप_प्रकार n, libcrun_error_t *err)
{
  माप_प्रकार i;
  पूर्णाङ्क req_mode;

#  define CMP_DEV_अङ्क(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  यदि (UNLIKELY (req_mode < 0))
    फर्काऊ req_mode;

  लागि (i = n; i > 0; i--)
    {
      माप_प्रकार current_rule = i - 1;
      पूर्णाङ्क mode;

      यदि (! CMP_DEV_अङ्क (devices[n]->minor, devices[current_rule]->minor))
        जारी;
      यदि (! CMP_DEV_अङ्क (devices[n]->major, devices[current_rule]->major))
        जारी;

      यदि (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        जारी;

      mode = access_to_mode (devices[i]->access, err);
      यदि (UNLIKELY (mode < 0))
        फर्काऊ mode;

      यदि (devices[current_rule]->allow && (mode & req_mode))
        फर्काऊ 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      यदि (req_mode == 0)
        फर्काऊ 1;
    }

  फर्काऊ 0;
#  undef CMP_DEV_अङ्क
}

स्थिर माप_प्रकार
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, माप_प्रकार n)
{
  माप_प्रकार i;

  यदि (n == 0)
    फर्काऊ 1;

  /* Find the first rule that is after the last "block all".  */
  लागि (i = n - 1; i-- > 0;)
    {
      यदि ((is_empty_string (devices[i]->type) || म_भेद (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        फर्काऊ i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  लागि (i = 0; i < n; i++)
    यदि (devices[i]->allow)
      फर्काऊ i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  फर्काऊ n + 1;
}

स्थिर bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, माप_प्रकार n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  पूर्णाङ्क remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  माप_प्रकार i;

  लागि (i = 0; i < n; i++)
    {
      माप_प्रकार current_rule = n - i - 1;
      यदि (! devices[current_rule]->allow)
        फर्काऊ false;

      यदि (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          यदि (is_empty_string (devices[current_rule]->type) || म_भेद (devices[current_rule]->type, "a") == 0)
            फर्काऊ true;

          यदि (म_भेद (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          अन्यथा यदि (म_भेद (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          यदि (remaining == 0)
            फर्काऊ true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  फर्काऊ false;
}

स्थिर पूर्णाङ्क
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  संरचना default_dev_s *default_devices = get_default_devices ();
  पूर्णाङ्क ret, sd_err;
  माप_प्रकार i;

  यदि (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      फर्काऊ 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  यदि (UNLIKELY (sd_err < 0))
    फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  लागि (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  लागि (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      वर्ण type;

      यदि (! d->allow)
        {
          पूर्णाङ्क redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          यदि (UNLIKELY (redundant < 0))
            फर्काऊ redundant;

          यदि (redundant)
            जारी;

          फर्काऊ crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      यदि (d->type == रिक्त || म_भेद (d->type, "a") == 0)
        type = 'a';
      अन्यथा यदि (म_भेद (d->type, "c") == 0)
        type = 'c';
      अन्यथा यदि (म_भेद (d->type, "b") == 0)
        type = 'b';
      अन्यथा
        फर्काऊ crun_make_error (err, 0, "unknown device type `%s`", d->type);

      यदि (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
      अन्यथा
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
    }

  फर्काऊ 0;
}

स्थिर bool
has_bpf_fs ()
{
  संरचना statfs stat;

  फर्काऊ statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

स्थिर पूर्णाङ्क
bpfprog_path_from_scope (वर्ण **path, अचर वर्ण *scope, libcrun_error_t *err)
{
  cleanup_free वर्ण *flat_scope = xstrdup (scope);
  वर्ण *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  जबसम्म ((it = म_अक्षर (it, '.')) != रिक्त)
    *it = '_';

  फर्काऊ append_paths (path, err, CRUN_BPF_DIR, flat_scope, रिक्त);
}

स्थिर पूर्णाङ्क
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 वर्ण **missing_properties,
                 अचर वर्ण *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  पूर्णाङ्क ret = 0;
  पूर्णाङ्क in_userns, sd_err;
  अचर वर्ण *property_name = "BPFProgram";
  cleanup_free संरचना bpf_program *program = रिक्त;
  cleanup_free वर्ण *path = रिक्त;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  यदि (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    फर्काऊ 0;

  in_userns = check_running_in_user_namespace (err);
  यदि (UNLIKELY (in_userns < 0))
    फर्काऊ in_userns;

  यदि (in_userns)
    फर्काऊ 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  यदि (UNLIKELY (program == रिक्त))
    {
      crun_error_release (err);
      फर्काऊ 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = सूची_गढ़ो (CRUN_BPF_DIR, 0700);
  यदि (UNLIKELY (ret < 0 && त्रुटि_सं != EEXIST))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "mkdir " CRUN_BPF_DIR);

  यदि (is_update)
    {
      cleanup_free संरचना bpf_program *program_loaded = रिक्त;

      यदि (resources->devices == रिक्त || resources->devices_len == 0)
        {
          *devices_set = true;
          फर्काऊ 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      /* it is the same as the loaded program.  */
      यदि (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          फर्काऊ 0;
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
        यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (tmp)))
          फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        यदि (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            फर्काऊ 0;
          }
      }

      फर्काऊ crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  यदि (UNLIKELY (sd_err < 0))
    फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  फर्काऊ 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
स्थिर पूर्णाङ्क
append_cpu_quota (sd_bus_message *m, वर्ण **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  अचर वर्ण *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  पूर्णाङ्क sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  यदि (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      पूर्णाङ्क ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
      यदि (has_period)
        period = max_period;
    }

  यदि (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  यदि ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      यदि (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          यदि (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      यदि (UNLIKELY (sd_err < 0))
        फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
append_resources (sd_bus_message *m,
                  bool is_update,
                  अचर वर्ण *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  पूर्णाङ्क cgroup_mode,
                  अचर वर्ण *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t मान;
  पूर्णाङ्क sd_err;
  पूर्णाङ्क ret;
  cleanup_free वर्ण *dir = रिक्त;
  cleanup_free वर्ण **missing_properties = रिक्त;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  missing_properties = read_dir_entries (dir, err);
  यदि (UNLIKELY (missing_properties == रिक्त))
    {
      यदि (crun_error_get_errno (err) != ENOENT)
        फर्काऊ -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_मान(name, मान)                                             \
    गर                                                                                 \
      {                                                                                \
        यदि (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", मान);              \
            यदि (UNLIKELY (sd_err < 0))                                                 \
              फर्काऊ crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } जबसम्म (0)

#  define APPEND_UINT64(name, fn)              \
    गर                                         \
      {                                        \
        ret = fn (resources, &मान, err);     \
        यदि (UNLIKELY (ret < 0))                \
          फर्काऊ ret;                          \
        यदि (ret)                               \
          {                                    \
            APPEND_UINT64_मान (name, मान); \
          }                                    \
    } जबसम्म (0)

  यदि (resources == रिक्त)
    फर्काऊ 0;

  APPEND_UINT64 ("MemoryLow", get_memory_तल्लो);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  विकल्प (cgroup_mode)
    {
    पक्ष CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;

        यदि (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            अचर वर्ण *property_name = "AllowedCPUs";
            अचर वर्ण *मान = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free वर्ण *allowed_cpus = रिक्त;
            माप_प्रकार allowed_cpus_len = 0;

            यदि (मान == रिक्त)
              मान = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (मान, &allowed_cpus, &allowed_cpus_len, err);
            यदि (UNLIKELY (ret < 0))
              फर्काऊ ret;

            यदि (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                यदि (UNLIKELY (ret < 0))
                  फर्काऊ ret;
              }
          }

        यदि (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            अचर वर्ण *property_name = "AllowedMemoryNodes";
            अचर वर्ण *मान = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free वर्ण *allowed_mems = रिक्त;
            माप_प्रकार allowed_mems_len = 0;

            यदि (मान == रिक्त)
              मान = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (मान, &allowed_mems, &allowed_mems_len, err);
            यदि (UNLIKELY (ret < 0))
              फर्काऊ ret;

            यदि (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                यदि (UNLIKELY (ret < 0))
                  फर्काऊ ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        यदि (UNLIKELY (ret < 0))
          फर्काऊ ret;
      }
      रोक;

    पक्ष CGROUP_MODE_LEGACY:
    पक्ष CGROUP_MODE_HYBRID:
      यदि (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_मान ("CPUShares", resources->cpu->shares);
      रोक;

    स्वभाव:
      फर्काऊ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_मान

  यदि (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
reset_failed_unit (sd_bus *bus, अचर वर्ण *unit)
{
  पूर्णाङ्क sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = रिक्त, *reply = रिक्त;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  यदि (UNLIKELY (sd_err < 0))
    गच्छ निकास;

  sd_err = sd_bus_message_append (m, "s", unit);
  यदि (UNLIKELY (sd_err < 0))
    गच्छ निकास;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  यदि (UNLIKELY (sd_err < 0))
    गच्छ निकास;

  sd_err = 0;

निकास:
  यदि (m)
    sd_bus_message_unref (m);
  यदि (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  फर्काऊ sd_err;
}

स्थिर पूर्णाङ्क
verify_ebpf_device_filter_installed (अचर वर्ण *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = रिक्त;
  cleanup_free वर्ण *full_path = रिक्त;
  माप_प्रकार n_progs = 0;
  पूर्णाङ्क ret;

  ret = append_paths (&full_path, err, CGROUP_जरा, cgroup_path, रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (n_progs == 0)
    फर्काऊ crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  फर्काऊ 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
स्थिर पूर्णाङ्क
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, पूर्णाङ्क ret)
{
  यदि (bus)
    sd_bus_unref (bus);
  यदि (m)
    sd_bus_message_unref (m);
  यदि (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  फर्काऊ ret;
}

स्थिर पूर्णाङ्क
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            पूर्णाङ्क cgroup_mode,
                            string_map *annotations,
                            अचर वर्ण *state_जरा,
                            अचर वर्ण *scope, अचर वर्ण *slice,
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
  अचर वर्ण *object = रिक्त;
  संरचना systemd_job_removed_s job_data = {};
  पूर्णाङ्क i;
  अचर वर्ण *boolean_opts[10];
  cleanup_free वर्ण *state_dir = रिक्त;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_जरा, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

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
    गच्छ निकास;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निकास;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      गच्छ निकास;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      गच्छ निकास;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      गच्छ निकास;
    }

  यदि (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      यदि (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          गच्छ निकास;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      गच्छ निकास;
    }

  यदि (annotations)
    {
      माप_प्रकार prefix_len = परिमाण (SYSTEMD_PROPERTY_PREFIX) - 1;
      माप_प्रकार annotations_len = string_map_size (annotations);
      माप_प्रकार i;

      लागि (i = 0; i < annotations_len; i++)
        {
          अचर वर्ण *साँचो;
          अचर वर्ण *मान;
          माप_प्रकार len;

          ret = string_map_get_at (annotations, i, &साँचो, &मान);
          यदि (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              गच्छ निकास;
            }

          यदि (! has_prefix (साँचो, SYSTEMD_PROPERTY_PREFIX))
            जारी;

          len = म_माप (साँचो);
          यदि (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", साँचो);
              गच्छ निकास;
            }

          ret = append_systemd_annotation (m, साँचो + prefix_len, len - prefix_len,
                                           मान, err);
          यदि (UNLIKELY (ret < 0))
            गच्छ निकास;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      गच्छ निकास;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      गच्छ निकास;
    }

  लागि (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      यदि (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          गच्छ निकास;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निकास;

  sd_err = sd_bus_message_close_container (m);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      गच्छ निकास;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      गच्छ निकास;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  यदि (UNLIKELY (sd_err < 0))
    {
      यदि (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          यदि (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = रिक्त;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      यदि (sd_err < 0)
        {
          यदि (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              यदि (UNLIKELY (ret < 0))
                गच्छ निकास;
              यदि (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          गच्छ निकास;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      गच्छ निकास;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

निकास:
  फर्काऊ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
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
  अचर वर्ण *object;
  अचर वर्ण *scope = cgroup_status->scope;
  संरचना systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निकास;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निकास;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      गच्छ निकास;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      गच्छ निकास;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      गच्छ निकास;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      गच्छ निकास;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

निकास:
  फर्काऊ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

स्थिर अचर वर्ण *
find_systemd_subgroup (string_map *annotations)
{
  अचर वर्ण *annotation;

  annotation = find_string_map_मान (annotations, "run.oci.systemd.subgroup");
  यदि (annotation)
    {
      यदि (annotation[0] == '\0')
        फर्काऊ रिक्त;
      फर्काऊ annotation;
    }

  फर्काऊ "container";
}

स्थिर पूर्णाङ्क
libcrun_cgroup_enter_systemd (संरचना libcrun_cgroup_args *args,
                              संरचना libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  अचर वर्ण *cgroup_path = args->cgroup_path;
  cleanup_free वर्ण *scope_path = रिक्त;
  cleanup_free वर्ण *scope = रिक्त;
  cleanup_free वर्ण *path = रिक्त;
  cleanup_free वर्ण *slice = रिक्त;
  पूर्णाङ्क retries_बायाँ = 32;
  अचर वर्ण *suffix;
  अचर वर्ण *id = args->id;
  pid_t pid = args->pid;
  पूर्णाङ्क cgroup_mode;
  पूर्णाङ्क rootless = 0;
  पूर्णाङ्क ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    फर्काऊ cgroup_mode;

  यदि (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      यदि (UNLIKELY (rootless < 0))
        फर्काऊ rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  लागि (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_जरा,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      यदि (LIKELY (ret >= 0))
        रोक;

      यदि (can_retry && retries_बायाँ-- > 0)
        {
          crun_error_release (err);
          जारी;
        }

      फर्काऊ ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  यदि (is_empty_string (suffix))
    scope_path = xstrdup (path);
  अन्यथा
    {
      माप_प्रकार path_len = म_माप (path);
      माप_प्रकार suffix_len = म_माप (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      यदि (UNLIKELY (scope_path == रिक्त))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  यदि (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;
    }

  out->path = path;
  path = रिक्त;

  out->scope = scope;
  scope = रिक्त;
  फर्काऊ 0;
}

वर्ण *
get_cgroup_scope_path (अचर वर्ण *cgroup_path, अचर वर्ण *scope)
{
  वर्ण *path_to_scope = रिक्त;
  वर्ण *cur;

  यदि (is_empty_string (cgroup_path))
    फर्काऊ रिक्त;

  path_to_scope = xstrdup (cgroup_path);

  cur = म_अक्षर (path_to_scope, '/');
  जबसम्म (cur)
    {
      वर्ण *next = म_अक्षर (cur + 1, '/');
      यदि (next == रिक्त)
        रोक;

      *next = '\0';
      यदि (म_भेद (cur, scope) == 0)
        फर्काऊ path_to_scope;
      *next = '/';

      cur = next;
      जबसम्म (*cur == '/')
        cur++;
    }

  फर्काऊ path_to_scope;
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
  यदि (UNLIKELY (mode < 0))
    फर्काऊ mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (err);
  अन्यथा
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  फर्काऊ destroy_cgroup_path (path_to_scope, mode, err);
}

स्थिर पूर्णाङ्क
libcrun_update_resources_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                  अचर वर्ण *state_जरा,
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

  ret = libcrun_get_state_directory (&state_dir, state_जरा, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  यदि (UNLIKELY (cgroup_mode < 0))
    फर्काऊ cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निकास;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      गच्छ निकास;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      गच्छ निकास;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      गच्छ निकास;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निकास;

  sd_err = sd_bus_message_close_container (m);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      गच्छ निकास;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  यदि (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      गच्छ निकास;
    }

  यदि (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ निकास;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ निकास;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निकास;

  ret = 0;

निकास:
  फर्काऊ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
स्थिर पूर्णाङ्क
libcrun_attach_process_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                अचर वर्ण *path, pid_t pid,
                                libcrun_error_t *err)
{
  अचर वर्ण *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free वर्ण *scope_path = रिक्त;
  sd_bus_message *reply = रिक्त;
  अचर वर्ण *subcgroup;
  अचर वर्ण *last;
  sd_bus *bus = रिक्त;
  माप_प्रकार len;
  पूर्णाङ्क sd_err, ret;

  यदि (is_empty_string (scope))
    फर्काऊ crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  यदि (scope_path == रिक्त)
    फर्काऊ crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = म_खोजप (scope_path, '/');
  यदि (last == रिक्त || म_भेद (last + 1, scope) != 0)
    फर्काऊ crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = म_माप (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  यदि (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  फर्काऊ cleanup_sd_bus_and_return (bus, रिक्त, reply, &error, ret);
}

#अन्यथा
स्थिर पूर्णाङ्क
libcrun_cgroup_enter_systemd (संरचना libcrun_cgroup_args *args,
                              संरचना libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (शून्य) args;
  (शून्य) out;

  फर्काऊ crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णाङ्क
libcrun_destroy_cgroup_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (शून्य) cgroup_status;

  फर्काऊ crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णाङ्क
libcrun_update_resources_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                  अचर वर्ण *state_जरा,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (शून्य) cgroup_status;
  (शून्य) state_जरा;
  (शून्य) resources;

  फर्काऊ crun_make_error (err, ENOTSUP, "systemd not supported");
}

स्थिर पूर्णाङ्क
libcrun_attach_process_systemd (संरचना libcrun_cgroup_status *cgroup_status,
                                अचर वर्ण *path, pid_t pid,
                                libcrun_error_t *err)
{
  (शून्य) cgroup_status;
  (शून्य) path;
  (शून्य) pid;

  फर्काऊ crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

संरचना libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = रिक्त,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
