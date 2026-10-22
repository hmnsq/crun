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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "cgroup.h"
#समावेश "cgroup-internal.h"
#समावेश "cgroup-systemd.h"
#समावेश "cgroup-resources.h"
#समावेश "cgroup-utils.h"
#समावेश "ebpf.h"
#समावेश "utils.h"
#समावेश "status.h"
#समावेश <string.h>
#समावेश <sys/types.h>
#समावेश <signal.h>
#समावेश <sys/vfs.h>
#समावेश <inttypes.h>
#समावेश <fcntl.h>
#समावेश <time.h>
#समावेश <sys/stat.h>
#समावेश <sys/vfs.h>
#समावेश <linux/magic.h>

#अगर_घोषित HAVE_SYSTEMD
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

अटल पूर्णांक
register_missing_property_from_message (स्थिर अक्षर *state_dir, स्थिर अक्षर *message, libcrun_error_t *err)
{
  cleanup_free अक्षर *file_path = शून्य;
  cleanup_free अक्षर *dir_path = शून्य;
  cleanup_free अक्षर *property = शून्य;
  अक्षर *p;
  पूर्णांक ret;

  अगर (! has_prefix (message, "Cannot set property "))
    वापस 0;

  property = xstrdup (message + माप ("Cannot set property ") - 1);
  p = म_अक्षर (property, ',');
  अगर (! p)
    वापस 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_paths (&file_path, err, dir_path, property, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, शून्य, 0, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 1;
}

अटल bool
property_missing_p (अक्षर **missing_properties, स्थिर अक्षर *property)
{
  माप_प्रकार i;

  क्रम (i = 0; missing_properties && missing_properties[i]; i++)
    अगर (म_भेद (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        वापस true;
      }

  वापस false;
}

अटल व्योम
get_systemd_scope_and_slice (स्थिर अक्षर *id, bool user_slice, स्थिर अक्षर *cgroup_path,
                             अक्षर **scope, अक्षर **slice)
{
  स्थिर अक्षर *n;
  अक्षर *p;

  अगर (cgroup_path == शून्य || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      वापस;
    }

  n = म_अक्षर (cgroup_path, ':');
  अगर (n == शून्य)
    xasprintf (scope, "%s.scope", cgroup_path);
  अन्यथा
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = म_अक्षर (*scope, ':');
      अगर (p)
        *p = '-';
    }
  अगर (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = म_अक्षर (*slice, ':');
      अगर (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      अगर (is_empty_string (*slice))
        {
          मुक्त (*slice);
          अगर (user_slice)
            *slice = xstrdup ("user.slice");
          अन्यथा
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
अटल पूर्णांक
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = शून्य;
  cleanup_close पूर्णांक dirfd = -1;
  bool need_set_parent = true;
  अक्षर fmt_buf[64];
  पूर्णांक len;
  पूर्णांक ret;

  अगर (resources == शून्य || resources->cpu == शून्य)
    वापस 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  अगर (is_empty_string (path))
    वापस crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  अगर (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_मूल, "cpu", path, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  अगर (UNLIKELY (dirfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

  अगर (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      अगर (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, माप (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (fmt_buf)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      अगर (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस 0;
}

अटल पूर्णांक
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = शून्य;
  पूर्णांक parent;
  पूर्णांक ret;

  अगर (resources == शून्य || resources->cpu == शून्य)
    वापस 0;

  अगर (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    वापस 0;

  क्रम (parent = 0; parent < 2; parent++)
    {
      cleanup_close पूर्णांक dirfd = -1;

      अगर (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_मूल, path ? path : "", (parent ? ".." : शून्य), शून्य);
      अन्यथा
        ret = append_paths (&cgroup_path, err, CGROUP_मूल, "/cpu", path ? path : "", (parent ? ".." : शून्य), शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      dirfd = खोलो (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      अगर (UNLIKELY (dirfd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      अगर (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }

  वापस 0;
}

अटल पूर्णांक
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_free अक्षर *cgroup_path = शून्य;
  पूर्णांक parent;
  पूर्णांक ret;
  पूर्णांक levels;

  ret = append_paths (&cgroup_path, err, CGROUP_मूल, "/cpuset", path ? path : "", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  क्रम (parent = 0; parent < levels; parent++)
    {
      cleanup_close पूर्णांक dirfd_cpuset = -1;
      cleanup_free अक्षर *path_to_cpuset = शून्य;

      ret = append_paths (&path_to_cpuset, err, CGROUP_मूल "/cpuset", path, (parent ? ".." : शून्य), शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      dirfd_cpuset = खोलो (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      अगर (UNLIKELY (dirfd_cpuset < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
अटल पूर्णांक
compute_finalized_path (अक्षर **path, स्थिर अक्षर *from, स्थिर अक्षर *suffix, libcrun_error_t *err)
{
  अगर (suffix == शून्य)
    {
      *path = xstrdup (from);
      वापस 0;
    }
  वापस append_paths (path, err, from, suffix, शून्य);
}

अटल पूर्णांक
systemd_finalize (काष्ठा libcrun_cgroup_args *args, अक्षर **path_out,
                  पूर्णांक cgroup_mode, स्थिर अक्षर *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free अक्षर *content = शून्य;
  cleanup_free अक्षर *path = शून्य;
  pid_t pid = args->pid;
  पूर्णांक ret;
  अक्षर *from, *to;
  अक्षर *saveptr = शून्य;

  cleanup_free अक्षर *proc_path = शून्य;
  cleanup_close पूर्णांक fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  अगर (UNLIKELY (fd < 0))
    वापस fd;

  ret = read_all_fd (fd, "cgroup path", &content, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  चयन (cgroup_mode)
    {
    हाल CGROUP_MODE_HYBRID:
    हाल CGROUP_MODE_LEGACY:
      from = म_माला (content, ":memory:");
      अगर (LIKELY (from != शून्य))
        from += 8;
      अन्यथा
        {
          from = म_माला (content, ":pids:");
          अगर (UNLIKELY (from == शून्य))
            वापस crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = म_अक्षर (from, '\n');
      अगर (UNLIKELY (to == शून्य))
        वापस crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      अगर (geteuid ())
        वापस 0;

      ret = setup_rt_runtime (resources, path, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      क्रम (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (शून्य, "\n", &saveptr))
        {
          अक्षर *subpath, *subsystem;
          subsystem = म_अक्षर (from, ':') + 1;
          subpath = म_अक्षर (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          अगर (subsystem[0] == '\0')
            {
              अगर (cgroup_mode == CGROUP_MODE_LEGACY)
                जारी;

              subsystem = "unified";
            }

          अगर (म_भेद (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              अगर (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  अगर (म_अक्षर (subsystem, '='))
                    {
                      crun_error_release (err);
                      जारी;
                    }
                  वापस ret;
                }
            }
        }

      अवरोध;

    हाल CGROUP_MODE_UNIFIED:
      {
        cleanup_free अक्षर *dir = शून्य;

        from = म_माला (content, "0::");
        अगर (UNLIKELY (from == शून्य))
          वापस crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = म_अक्षर (from, '\n');
        अगर (UNLIKELY (to == शून्य))
          वापस crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        ret = append_paths (&dir, err, CGROUP_मूल, path, शून्य);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        ret = move_process_to_cgroup (pid, शून्य, path, err);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        ret = enable_controllers (path, err);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        अगर (suffix)
          {
            ret = chown_cgroups (path, args->मूल_uid, args->मूल_gid, err);
            अगर (UNLIKELY (ret < 0))
              वापस ret;
          }
      }
      अवरोध;

    शेष:
      वापस crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  *path_out = path;
  path = शून्य;

  वापस 0;
}

काष्ठा systemd_job_removed_s
{
  स्थिर अक्षर *path;
  स्थिर अक्षर *op;
  पूर्णांक terminated;
  libcrun_error_t err;
};

अटल पूर्णांक
systemd_job_removed (sd_bus_message *m, व्योम *userdata, sd_bus_error *error arg_unused)
{
  स्थिर अक्षर *path, *unit, *परिणाम;
  uint32_t id;
  पूर्णांक ret;
  काष्ठा systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &परिणाम);
  अगर (ret < 0)
    वापस -1;

  अगर (म_भेद (d->path, path) == 0)
    {
      d->terminated = 1;
      अगर (म_भेद (परिणाम, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, परिणाम);
          वापस -1;
        }
    }
  वापस 0;
}

अटल पूर्णांक
systemd_check_job_status_setup (sd_bus *bus, काष्ठा systemd_job_removed_s *तथ्य, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = sd_bus_match_signal_async (bus, शून्य, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, शून्य, तथ्य);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, -ret, "sd-bus match signal");

  वापस 0;
}

अटल पूर्णांक
systemd_check_job_status (sd_bus *bus, काष्ठा systemd_job_removed_s *तथ्य, स्थिर अक्षर *path, स्थिर अक्षर *op,
                          libcrun_error_t *err)
{
  पूर्णांक sd_err;

  तथ्य->path = path;
  तथ्य->op = op;
  जबतक (! तथ्य->terminated)
    {
      sd_err = sd_bus_process (bus, शून्य);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus process");

      अगर (sd_err != 0)
        जारी;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus wait");
    }

  अगर (तथ्य->err != शून्य)
    {
      *err = तथ्य->err;
      वापस -1;
    }

  वापस 0;
}

पूर्णांक
parse_sd_array (अक्षर *s, अक्षर **out, अक्षर **अग्र, libcrun_error_t *err)
{
  अक्षर endchr;
  अक्षर *it, *dest;
  bool escaped = false;

  *out = शून्य;
  *अग्र = शून्य;

  जबतक (है_खाली (*s))
    s++;
  अगर (*s == '\0')
    वापस 0;
  अन्यथा अगर (*s != '\'' && *s != '"')
    वापस crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  जबतक (1)
    {
      अगर (*it == '\0')
        वापस crun_make_error (err, 0, "invalid string `%s`", s);
      अगर (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          जबतक (है_खाली (*it))
            it++;
          अगर (*it == ',')
            {
              *अग्र = ++it;
              *dest = '\0';
              वापस 0;
            }

          अगर (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              वापस 0;
            }

          वापस crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      अगर (! escaped)
        *dest++ = *it;
      it++;
    }

  वापस 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
अटल पूर्णांक
append_systemd_annotation (sd_bus_message *m, स्थिर अक्षर *name, माप_प्रकार name_len, स्थिर अक्षर *मूल्य,
                           libcrun_error_t *err)
{
  cleanup_free अक्षर *tmp_name = शून्य;
  uint32_t factor = 1;
  स्थिर अक्षर *it;
  पूर्णांक sd_err;

  जबतक (*मूल्य == ' ')
    मूल्य++;

  it = मूल्य;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  अगर (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  अगर (factor != 1)
    {
      अटल स्थिर अक्षर *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", शून्य };
      स्थिर अक्षर *अंक = it;
      अक्षर *endptr = शून्य;
      द्विगुन v;
      माप_प्रकार i;

      क्रम (i = 0; types[i]; i++)
        अगर (has_prefix (अंक, types[i]))
          {
            अंक += म_माप (types[i]);
            अवरोध;
          }

      त्रुटि_सं = 0;
      v = म_से_भग्न (अंक, &endptr);
      अगर (UNLIKELY (त्रुटि_सं != 0 || endptr == अंक || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        वापस crun_make_error (err, त्रुटि_सं, "invalid value for `%s`: `%s`", name, मूल्य);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      वापस 0;
    }

  अगर ((म_भेद (it, "true") == 0) || (म_भेद (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      वापस 0;
    }
  अन्यथा अगर (*it == '\'')
    {
      cleanup_free अक्षर *v_start = शून्य;
      अक्षर *end;

      it = v_start = xstrdup (मूल्य);

      end = म_अक्षर (v_start + 1, '\'');
      अगर (end == शून्य)
        वापस crun_make_error (err, 0, "invalid variant `%s`", मूल्य);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      वापस 0;
    }
  अन्यथा अगर (*it == '[')
    {
      cleanup_free अक्षर *v_start = शून्य;
      माप_प्रकार n_parts = 0, parts_आकार = 32;
      cleanup_free अक्षर **parts = xmalloc (माप (अक्षर *) * parts_आकार);
      अक्षर *part;

      part = v_start = xstrdup (it + 1);
      जबतक (1)
        {
          अक्षर *out = शून्य, *अग्र = शून्य;
          पूर्णांक ret;

          ret = parse_sd_array (part, &out, &अग्र, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          parts[n_parts++] = out;
          अगर (n_parts == parts_आकार - 1)
            {
              parts_आकार += 32;
              parts = xrealloc (parts, माप (अक्षर *) * parts_आकार);
            }
          parts[n_parts] = शून्य;
          अगर (अग्र == शून्य)
            अवरोध;

          part = अग्र;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus close container");

      वापस 0;
    }
  अन्यथा अगर (has_prefix (it, "uint64 "))
    {
      अक्षर *endptr = शून्य;
      uint64_t v;

      त्रुटि_सं = 0;
      v = strtoull (it + माप ("uint64"), &endptr, 10);
      अगर (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        वापस crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      वापस 0;
    }
  अन्यथा अगर (has_prefix (it, "int64 "))
    {
      अक्षर *endptr = शून्य;
      int64_t v;

      त्रुटि_सं = 0;
      v = strtoll (it + माप ("int64"), &endptr, 10);
      अगर (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        वापस crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      वापस 0;
    }
  अन्यथा अगर (has_prefix (it, "uint32 "))
    {
      अक्षर *endptr = शून्य;
      uint32_t v;

      त्रुटि_सं = 0;
      v = म_से_अदीर्घ (it + माप ("uint32"), &endptr, 10);
      अगर (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        वापस crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      वापस 0;
    }
  अन्यथा अगर (has_prefix (it, "int32 ") || म_अक्षर (it, ' ') == शून्य)
    {
      अक्षर *endptr = शून्य;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      त्रुटि_सं = 0;
      अगर (has_prefix (it, "int32 "))
        v = म_से_दीर्घ (it + माप ("int32"), &endptr, 10);
      अन्यथा
        v = म_से_दीर्घ (it, &endptr, 10);
      अगर (UNLIKELY (त्रुटि_सं != 0 || *endptr))
        वापस crun_make_error (err, त्रुटि_सं, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      वापस 0;
    }

  वापस crun_make_error (err, 0, "unknown type for `%s`", name);
}

अटल पूर्णांक
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  पूर्णांक rootless;
  पूर्णांक sd_err = 0;

  rootless = is_rootless (err);
  अगर (UNLIKELY (rootless < 0))
    वापस rootless;

  अगर (rootless)
    sd_err = sd_bus_default_user (bus);
  अगर (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  अगर (sd_err < 0)
    वापस crun_make_error (err, -sd_err, "cannot open sd-bus");

  वापस 0;
}

अटल स्थिर अक्षर *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, स्थिर अक्षर *name)
{
  माप_प्रकार i;

  अगर (resources == शून्य || resources->unified == शून्य)
    वापस शून्य;

  क्रम (i = 0; i < resources->unified->len; i++)
    अगर (म_भेद (resources->unified->कुंजियाँ[i], name) == 0)
      वापस resources->unified->मूल्यावली[i];
  वापस शून्य;
}

अटल पूर्णांक
get_मूल्य_from_unified_map (runtime_spec_schema_config_linux_resources *resources, स्थिर अक्षर *name,
                            uint64_t *मूल्य, libcrun_error_t *err)
{
  अक्षर *endptr = शून्य;
  माप_प्रकार i;

  अगर (resources == शून्य || resources->unified == शून्य)
    वापस 0;

  क्रम (i = 0; i < resources->unified->len; i++)
    अगर (म_भेद (resources->unified->कुंजियाँ[i], name) == 0)
      {
        अगर (म_भेद (resources->unified->मूल्यावली[i], "max") == 0)
          {
            *मूल्य = UINT64_MAX;
            वापस 1;
          }

        त्रुटि_सं = 0;
        *मूल्य = strtoull (resources->unified->मूल्यावली[i], &endptr, 10);
        अगर (UNLIKELY (त्रुटि_सं))
          वापस crun_make_error (err, त्रुटि_सं, "invalid value for `%s`: %s", name,
                                  resources->unified->मूल्यावली[i]);
        अगर (endptr && *endptr)
          वापस crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->मूल्यावली[i]);
        वापस 1;
      }
  वापस 0;
}

अटल अंतरभूत पूर्णांक
get_memory_निचला (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  अगर (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      वापस 1;
    }

  वापस get_मूल्य_from_unified_map (resources, "memory.low", limit, err);
}

अटल अंतरभूत पूर्णांक
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  अगर (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      वापस 1;
    }

  वापस get_मूल्य_from_unified_map (resources, "pids.max", limit, err);
}

अटल अंतरभूत पूर्णांक
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  अगर (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      वापस 1;
    }

  वापस get_मूल्य_from_unified_map (resources, "memory.max", limit, err);
}

अटल अंतरभूत पूर्णांक
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  अगर (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      अगर (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          वापस 1;
        }
      /* 0 means unset.  */
      अगर (memory->swap == 0)
        वापस 0;
      अगर (mem <= 0)
        वापस crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      अगर (memory->swap < mem)
        वापस crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      वापस 1;
    }

  वापस get_मूल्य_from_unified_map (resources, "memory.swap.max", limit, err);
}

अटल अंतरभूत पूर्णांक
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t मूल्य;
  पूर्णांक ret;

  ret = get_मूल्य_from_unified_map (resources, "cpu.idle", &मूल्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  अगर (ret > 0 && मूल्य == 1)
    has_idle = true;
  अगर (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_मूल्य_from_unified_map (resources, "cpu.weight", weight, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  अगर (ret > 0)
    {
      अगर (has_idle)
        वापस crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      वापस 1;
    }

  अगर (resources->cpu && resources->cpu->shares_present)
    {
      अगर (has_idle)
        वापस crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      अगर (resources->cpu->shares == 0)
        वापस 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      वापस 1;
    }
  अगर (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      वापस 1;
    }
  वापस 0;
}

अटल पूर्णांक
read_uint64_मूल्य (स्थिर अक्षर *str, uint64_t *मूल्य, libcrun_error_t *err)
{
  अक्षर *endptr = शून्य;

  त्रुटि_सं = 0;
  *मूल्य = strtoull (str, &endptr, 10);
  अगर (UNLIKELY (त्रुटि_सं))
    वापस crun_make_error (err, त्रुटि_सं, "invalid value: %s", str);
  अगर (endptr && *endptr)
    वापस crun_make_error (err, 0, "invalid value: %s", str);
  वापस 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
अटल अंतरभूत uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  वापस bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

अटल पूर्णांक
append_io_weight (sd_bus_message *m, अक्षर **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  पूर्णांक sd_err;
  माप_प्रकार i;
  पूर्णांक ret;

#  define APPEND_IO_WEIGHT(मूल्य)                                                      \
    करो                                                                                 \
      {                                                                                \
        अगर (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            अगर (UNLIKELY (sd_err < 0))                                                 \
              वापस crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } जबतक (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, मूल्य)                                                      \
    करो                                                                                                \
      {                                                                                               \
        अगर (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            अगर (UNLIKELY (sd_err < 0))                                                                \
              वापस crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } जबतक (0)

  अगर (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  क्रम (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      अक्षर name[64];
      पूर्णांक len;

      len = snprintf (name, माप (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      अगर (UNLIKELY (len >= (पूर्णांक) माप (name)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  क्रम (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free अक्षर *मूल्य = शून्य;
      bool need_conversion = false;
      अक्षर *saveptr = शून्य;
      अक्षर *पंक्ति;

      अगर (म_भेद (resources->unified->कुंजियाँ[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      अन्यथा अगर (म_भेद (resources->unified->कुंजियाँ[i], "io.weight") == 0)
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

      मूल्य = xstrdup (resources->unified->मूल्यावली[i]);

      क्रम (पंक्ति = strtok_r (मूल्य, "\n", &saveptr); पंक्ति; पंक्ति = strtok_r (शून्य, "\n", &saveptr))
        {
          अक्षर *sep;

          sep = म_अक्षर (पंक्ति, ' ');
          जबतक (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_मूल्य (sep ? sep : पंक्ति, &weight, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          अगर (need_conversion)
            weight = IO_WEIGHT (weight);

          अगर (sep == शून्य || म_भेद (पंक्ति, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          अन्यथा
            APPEND_IO_DEVICE_WEIGHT (पंक्ति, weight);
        }
    }

  वापस 0;
}

/* Adapted from systemd.  */
अटल पूर्णांक
bus_append_byte_array (sd_bus_message *m, स्थिर अक्षर *field, स्थिर व्योम *buf, माप_प्रकार n, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  अगर (ret < 0)
    वापस crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  अगर (ret < 0)
    वापस crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  अगर (ret < 0)
    वापस crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  अगर (ret < 0)
    वापस crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  अगर (ret < 0)
    वापस crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  अगर (ret < 0)
    वापस crun_make_error (err, -ret, "sd_bus_message_close_container");

  वापस 1;
}

अटल पूर्णांक
append_uint64_from_unified_map (sd_bus_message *m,
                                अक्षर **missing_properties,
                                स्थिर अक्षर *attr,
                                स्थिर अक्षर *कुंजी,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t मूल्य;
  पूर्णांक ret;

  अगर (property_missing_p (missing_properties, attr))
    वापस 0;

  ret = get_मूल्य_from_unified_map (resources, कुंजी, &मूल्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  अगर (ret)
    {
      पूर्णांक sd_err = sd_bus_message_append (m, "(sv)", attr, "t", मूल्य);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  वापस 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
अटल पूर्णांक
append_device_allow (sd_bus_message *m,
                     स्थिर अक्षर type,
                     पूर्णांक major,
                     पूर्णांक minor,
                     स्थिर अक्षर *access,
                     libcrun_error_t *err)
{
  अक्षर device[64];
  पूर्णांक sd_err;
  पूर्णांक len;

  अगर (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      वापस 0;
    }

  अगर (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, माप (device), "%s-*", type == 'c' ? "char" : "block");
  अन्यथा अगर (IS_WILDCARD (minor))
    len = snprintf (device, माप (device), type == 'c' ? "char-%d" : "block-%d", major);
  अन्यथा
    len = snprintf (device, माप (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  अगर (UNLIKELY (len >= (पूर्णांक) माप (device)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  अगर (UNLIKELY (sd_err < 0))
    वापस crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  वापस 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

अटल पूर्णांक
access_to_mode (स्थिर अक्षर *access, libcrun_error_t *err)
{
  पूर्णांक mode = 0;

  जबतक (access && *access)
    {
      पूर्णांक c = *access++;

      चयन (c)
        {
        हाल 'r':
          mode |= MODE_R;
          अवरोध;

        हाल 'w':
          mode |= MODE_W;
          अवरोध;

        हाल 'm':
          mode |= MODE_M;
          अवरोध;

        शेष:
          वापस crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  वापस mode;
}

/* check if the type b is included in type a.  */
अटल bool
has_same_type (स्थिर अक्षर *a, स्थिर अक्षर *b)
{
  bool a_all = a == शून्य || म_भेद (a, "a") == 0;
  अगर (b == शून्य || म_भेद (b, "a") == 0)
    वापस a_all;

  वापस a_all || म_भेद (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
अटल पूर्णांक
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, माप_प्रकार n, libcrun_error_t *err)
{
  माप_प्रकार i;
  पूर्णांक req_mode;

#  define CMP_DEV_अंक(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  अगर (UNLIKELY (req_mode < 0))
    वापस req_mode;

  क्रम (i = n; i > 0; i--)
    {
      माप_प्रकार current_rule = i - 1;
      पूर्णांक mode;

      अगर (! CMP_DEV_अंक (devices[n]->minor, devices[current_rule]->minor))
        जारी;
      अगर (! CMP_DEV_अंक (devices[n]->major, devices[current_rule]->major))
        जारी;

      अगर (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        जारी;

      mode = access_to_mode (devices[i]->access, err);
      अगर (UNLIKELY (mode < 0))
        वापस mode;

      अगर (devices[current_rule]->allow && (mode & req_mode))
        वापस 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      अगर (req_mode == 0)
        वापस 1;
    }

  वापस 0;
#  undef CMP_DEV_अंक
}

अटल माप_प्रकार
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, माप_प्रकार n)
{
  माप_प्रकार i;

  अगर (n == 0)
    वापस 1;

  /* Find the first rule that is after the last "block all".  */
  क्रम (i = n - 1; i-- > 0;)
    {
      अगर ((is_empty_string (devices[i]->type) || म_भेद (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        वापस i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  क्रम (i = 0; i < n; i++)
    अगर (devices[i]->allow)
      वापस i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  वापस n + 1;
}

अटल bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, माप_प्रकार n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  पूर्णांक remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  माप_प्रकार i;

  क्रम (i = 0; i < n; i++)
    {
      माप_प्रकार current_rule = n - i - 1;
      अगर (! devices[current_rule]->allow)
        वापस false;

      अगर (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          अगर (is_empty_string (devices[current_rule]->type) || म_भेद (devices[current_rule]->type, "a") == 0)
            वापस true;

          अगर (म_भेद (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          अन्यथा अगर (म_भेद (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          अगर (remaining == 0)
            वापस true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  वापस false;
}

अटल पूर्णांक
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  काष्ठा default_dev_s *default_devices = get_default_devices ();
  पूर्णांक ret, sd_err;
  माप_प्रकार i;

  अगर (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      वापस 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  अगर (UNLIKELY (sd_err < 0))
    वापस crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  क्रम (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  क्रम (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      अक्षर type;

      अगर (! d->allow)
        {
          पूर्णांक redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          अगर (UNLIKELY (redundant < 0))
            वापस redundant;

          अगर (redundant)
            जारी;

          वापस crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      अगर (d->type == शून्य || म_भेद (d->type, "a") == 0)
        type = 'a';
      अन्यथा अगर (म_भेद (d->type, "c") == 0)
        type = 'c';
      अन्यथा अगर (म_भेद (d->type, "b") == 0)
        type = 'b';
      अन्यथा
        वापस crun_make_error (err, 0, "unknown device type `%s`", d->type);

      अगर (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
      अन्यथा
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }

  वापस 0;
}

अटल bool
has_bpf_fs ()
{
  काष्ठा statfs stat;

  वापस statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

अटल पूर्णांक
bpfprog_path_from_scope (अक्षर **path, स्थिर अक्षर *scope, libcrun_error_t *err)
{
  cleanup_free अक्षर *flat_scope = xstrdup (scope);
  अक्षर *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  जबतक ((it = म_अक्षर (it, '.')) != शून्य)
    *it = '_';

  वापस append_paths (path, err, CRUN_BPF_DIR, flat_scope, शून्य);
}

अटल पूर्णांक
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 अक्षर **missing_properties,
                 स्थिर अक्षर *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  पूर्णांक ret = 0;
  पूर्णांक in_userns, sd_err;
  स्थिर अक्षर *property_name = "BPFProgram";
  cleanup_free काष्ठा bpf_program *program = शून्य;
  cleanup_free अक्षर *path = शून्य;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  अगर (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    वापस 0;

  in_userns = check_running_in_user_namespace (err);
  अगर (UNLIKELY (in_userns < 0))
    वापस in_userns;

  अगर (in_userns)
    वापस 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  अगर (UNLIKELY (program == शून्य))
    {
      crun_error_release (err);
      वापस 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = सूची_गढ़ो (CRUN_BPF_DIR, 0700);
  अगर (UNLIKELY (ret < 0 && त्रुटि_सं != EEXIST))
    वापस crun_make_error (err, त्रुटि_सं, "mkdir " CRUN_BPF_DIR);

  अगर (is_update)
    {
      cleanup_free काष्ठा bpf_program *program_loaded = शून्य;

      अगर (resources->devices == शून्य || resources->devices_len == 0)
        {
          *devices_set = true;
          वापस 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      /* it is the same as the loaded program.  */
      अगर (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          वापस 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free काष्ठा bpf_program *program_new = शून्य;
        अक्षर tmp[64];
        पूर्णांक len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, माप (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        अगर (UNLIKELY (len >= (पूर्णांक) माप (tmp)))
          वापस crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        अगर (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            वापस 0;
          }
      }

      वापस crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  अगर (UNLIKELY (sd_err < 0))
    वापस crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  वापस 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
अटल पूर्णांक
append_cpu_quota (sd_bus_message *m, अक्षर **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  स्थिर अक्षर *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  पूर्णांक sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  अगर (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      पूर्णांक ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      अगर (has_period)
        period = max_period;
    }

  अगर (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  अगर ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      अगर (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          अगर (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      अगर (UNLIKELY (sd_err < 0))
        वापस crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  वापस 0;
}

अटल पूर्णांक
append_resources (sd_bus_message *m,
                  bool is_update,
                  स्थिर अक्षर *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  पूर्णांक cgroup_mode,
                  स्थिर अक्षर *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t मूल्य;
  पूर्णांक sd_err;
  पूर्णांक ret;
  cleanup_free अक्षर *dir = शून्य;
  cleanup_free अक्षर **missing_properties = शून्य;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  missing_properties = read_dir_entries (dir, err);
  अगर (UNLIKELY (missing_properties == शून्य))
    {
      अगर (crun_error_get_errno (err) != ENOENT)
        वापस -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_मूल्य(name, मूल्य)                                             \
    करो                                                                                 \
      {                                                                                \
        अगर (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", मूल्य);              \
            अगर (UNLIKELY (sd_err < 0))                                                 \
              वापस crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } जबतक (0)

#  define APPEND_UINT64(name, fn)              \
    करो                                         \
      {                                        \
        ret = fn (resources, &मूल्य, err);     \
        अगर (UNLIKELY (ret < 0))                \
          वापस ret;                          \
        अगर (ret)                               \
          {                                    \
            APPEND_UINT64_मूल्य (name, मूल्य); \
          }                                    \
    } जबतक (0)

  अगर (resources == शून्य)
    वापस 0;

  APPEND_UINT64 ("MemoryLow", get_memory_निचला);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  चयन (cgroup_mode)
    {
    हाल CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        अगर (UNLIKELY (ret < 0))
          वापस ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        अगर (UNLIKELY (ret < 0))
          वापस ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        अगर (UNLIKELY (ret < 0))
          वापस ret;

        अगर (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            स्थिर अक्षर *property_name = "AllowedCPUs";
            स्थिर अक्षर *मूल्य = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free अक्षर *allowed_cpus = शून्य;
            माप_प्रकार allowed_cpus_len = 0;

            अगर (मूल्य == शून्य)
              मूल्य = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (मूल्य, &allowed_cpus, &allowed_cpus_len, err);
            अगर (UNLIKELY (ret < 0))
              वापस ret;

            अगर (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                अगर (UNLIKELY (ret < 0))
                  वापस ret;
              }
          }

        अगर (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            स्थिर अक्षर *property_name = "AllowedMemoryNodes";
            स्थिर अक्षर *मूल्य = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free अक्षर *allowed_mems = शून्य;
            माप_प्रकार allowed_mems_len = 0;

            अगर (मूल्य == शून्य)
              मूल्य = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (मूल्य, &allowed_mems, &allowed_mems_len, err);
            अगर (UNLIKELY (ret < 0))
              वापस ret;

            अगर (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                अगर (UNLIKELY (ret < 0))
                  वापस ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        अगर (UNLIKELY (ret < 0))
          वापस ret;
      }
      अवरोध;

    हाल CGROUP_MODE_LEGACY:
    हाल CGROUP_MODE_HYBRID:
      अगर (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_मूल्य ("CPUShares", resources->cpu->shares);
      अवरोध;

    शेष:
      वापस crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_मूल्य

  अगर (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस 0;
}

अटल पूर्णांक
reset_failed_unit (sd_bus *bus, स्थिर अक्षर *unit)
{
  पूर्णांक sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = शून्य, *reply = शून्य;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  अगर (UNLIKELY (sd_err < 0))
    जाओ निकास;

  sd_err = sd_bus_message_append (m, "s", unit);
  अगर (UNLIKELY (sd_err < 0))
    जाओ निकास;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  अगर (UNLIKELY (sd_err < 0))
    जाओ निकास;

  sd_err = 0;

निकास:
  अगर (m)
    sd_bus_message_unref (m);
  अगर (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  वापस sd_err;
}

अटल पूर्णांक
verify_ebpf_device_filter_installed (स्थिर अक्षर *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = शून्य;
  cleanup_free अक्षर *full_path = शून्य;
  माप_प्रकार n_progs = 0;
  पूर्णांक ret;

  ret = append_paths (&full_path, err, CGROUP_मूल, cgroup_path, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (n_progs == 0)
    वापस crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  वापस 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
अटल पूर्णांक
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, पूर्णांक ret)
{
  अगर (bus)
    sd_bus_unref (bus);
  अगर (m)
    sd_bus_message_unref (m);
  अगर (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  वापस ret;
}

अटल पूर्णांक
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            पूर्णांक cgroup_mode,
                            string_map *annotations,
                            स्थिर अक्षर *state_मूल,
                            स्थिर अक्षर *scope, स्थिर अक्षर *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = शून्य;
  sd_bus_message *m = शून्य;
  sd_bus_message *reply = शून्य;
  पूर्णांक sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  स्थिर अक्षर *object = शून्य;
  काष्ठा systemd_job_removed_s job_तथ्य = {};
  पूर्णांक i;
  स्थिर अक्षर *boolean_opts[10];
  cleanup_free अक्षर *state_dir = शून्य;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_मूल, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = शून्य;

  ret = open_sd_bus_connection (&bus, err);
  अगर (UNLIKELY (ret < 0))
    जाओ निकास;

  ret = systemd_check_job_status_setup (bus, &job_तथ्य, err);
  अगर (UNLIKELY (ret < 0))
    जाओ निकास;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      जाओ निकास;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      जाओ निकास;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      जाओ निकास;
    }

  अगर (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      अगर (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          जाओ निकास;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      जाओ निकास;
    }

  अगर (annotations)
    {
      माप_प्रकार prefix_len = माप (SYSTEMD_PROPERTY_PREFIX) - 1;
      माप_प्रकार annotations_len = string_map_आकार (annotations);
      माप_प्रकार i;

      क्रम (i = 0; i < annotations_len; i++)
        {
          स्थिर अक्षर *कुंजी;
          स्थिर अक्षर *मूल्य;
          माप_प्रकार len;

          ret = string_map_get_at (annotations, i, &कुंजी, &मूल्य);
          अगर (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              जाओ निकास;
            }

          अगर (! has_prefix (कुंजी, SYSTEMD_PROPERTY_PREFIX))
            जारी;

          len = म_माप (कुंजी);
          अगर (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", कुंजी);
              जाओ निकास;
            }

          ret = append_systemd_annotation (m, कुंजी + prefix_len, len - prefix_len,
                                           मूल्य, err);
          अगर (UNLIKELY (ret < 0))
            जाओ निकास;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      जाओ निकास;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      जाओ निकास;
    }

  क्रम (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      अगर (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          जाओ निकास;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  अगर (UNLIKELY (ret < 0))
    जाओ निकास;

  sd_err = sd_bus_message_close_container (m);
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      जाओ निकास;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      जाओ निकास;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  अगर (UNLIKELY (sd_err < 0))
    {
      अगर (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          अगर (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = शून्य;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      अगर (sd_err < 0)
        {
          अगर (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              अगर (UNLIKELY (ret < 0))
                जाओ निकास;
              अगर (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          जाओ निकास;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      जाओ निकास;
    }

  ret = systemd_check_job_status (bus, &job_तथ्य, object, "creating", err);

निकास:
  वापस cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

अटल पूर्णांक
libcrun_destroy_systemd_cgroup_scope (काष्ठा libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = शून्य;
  sd_bus_message *m = शून्य;
  sd_bus_message *reply = शून्य;
  पूर्णांक ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  स्थिर अक्षर *object;
  स्थिर अक्षर *scope = cgroup_status->scope;
  काष्ठा systemd_job_removed_s job_तथ्य = {};

  ret = open_sd_bus_connection (&bus, err);
  अगर (UNLIKELY (ret < 0))
    जाओ निकास;

  ret = systemd_check_job_status_setup (bus, &job_तथ्य, err);
  अगर (UNLIKELY (ret < 0))
    जाओ निकास;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  अगर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      जाओ निकास;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  अगर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      जाओ निकास;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  अगर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      जाओ निकास;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  अगर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      जाओ निकास;
    }

  ret = systemd_check_job_status (bus, &job_तथ्य, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

निकास:
  वापस cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

अटल स्थिर अक्षर *
find_systemd_subgroup (string_map *annotations)
{
  स्थिर अक्षर *annotation;

  annotation = find_string_map_मूल्य (annotations, "run.oci.systemd.subgroup");
  अगर (annotation)
    {
      अगर (annotation[0] == '\0')
        वापस शून्य;
      वापस annotation;
    }

  वापस "container";
}

अटल पूर्णांक
libcrun_cgroup_enter_systemd (काष्ठा libcrun_cgroup_args *args,
                              काष्ठा libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  स्थिर अक्षर *cgroup_path = args->cgroup_path;
  cleanup_free अक्षर *scope_path = शून्य;
  cleanup_free अक्षर *scope = शून्य;
  cleanup_free अक्षर *path = शून्य;
  cleanup_free अक्षर *slice = शून्य;
  पूर्णांक retries_बायाँ = 32;
  स्थिर अक्षर *suffix;
  स्थिर अक्षर *id = args->id;
  pid_t pid = args->pid;
  पूर्णांक cgroup_mode;
  पूर्णांक rootless = 0;
  पूर्णांक ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      अगर (UNLIKELY (rootless < 0))
        वापस rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  क्रम (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_मूल,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      अगर (LIKELY (ret >= 0))
        अवरोध;

      अगर (can_retry && retries_बायाँ-- > 0)
        {
          crun_error_release (err);
          जारी;
        }

      वापस ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  अगर (is_empty_string (suffix))
    scope_path = xstrdup (path);
  अन्यथा
    {
      माप_प्रकार path_len = म_माप (path);
      माप_प्रकार suffix_len = म_माप (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      अगर (UNLIKELY (scope_path == शून्य))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  अगर (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  out->path = path;
  path = शून्य;

  out->scope = scope;
  scope = शून्य;
  वापस 0;
}

अक्षर *
get_cgroup_scope_path (स्थिर अक्षर *cgroup_path, स्थिर अक्षर *scope)
{
  अक्षर *path_to_scope = शून्य;
  अक्षर *cur;

  अगर (is_empty_string (cgroup_path))
    वापस शून्य;

  path_to_scope = xstrdup (cgroup_path);

  cur = म_अक्षर (path_to_scope, '/');
  जबतक (cur)
    {
      अक्षर *अग्र = म_अक्षर (cur + 1, '/');
      अगर (अग्र == शून्य)
        अवरोध;

      *अग्र = '\0';
      अगर (म_भेद (cur, scope) == 0)
        वापस path_to_scope;
      *अग्र = '/';

      cur = अग्र;
      जबतक (*cur == '/')
        cur++;
    }

  वापस path_to_scope;
}

अटल पूर्णांक
libcrun_destroy_cgroup_systemd (काष्ठा libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free अक्षर *path_to_scope = शून्य;
  cleanup_free अक्षर *bpfprog = शून्य;
  पूर्णांक mode;
  पूर्णांक ret;

  mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (mode < 0))
    वापस mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  अगर (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  अगर (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  अगर (UNLIKELY (ret < 0))
    crun_error_release (err);
  अन्यथा
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  वापस destroy_cgroup_path (path_to_scope, mode, err);
}

अटल पूर्णांक
libcrun_update_resources_systemd (काष्ठा libcrun_cgroup_status *cgroup_status,
                                  स्थिर अक्षर *state_मूल,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  काष्ठा systemd_job_removed_s job_तथ्य = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free अक्षर *state_dir = शून्य;
  sd_bus_message *reply = शून्य;
  sd_bus_message *m = शून्य;
  sd_bus *bus = शून्य;
  पूर्णांक sd_err, ret;
  पूर्णांक cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_मूल, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = systemd_check_job_status_setup (bus, &job_तथ्य, err);
  अगर (UNLIKELY (ret < 0))
    जाओ निकास;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      जाओ निकास;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      जाओ निकास;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      जाओ निकास;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  अगर (UNLIKELY (ret < 0))
    जाओ निकास;

  sd_err = sd_bus_message_close_container (m);
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      जाओ निकास;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  अगर (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      जाओ निकास;
    }

  अगर (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      अगर (UNLIKELY (ret < 0))
        जाओ निकास;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      अगर (UNLIKELY (ret < 0))
        जाओ निकास;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  अगर (UNLIKELY (ret < 0))
    जाओ निकास;

  ret = 0;

निकास:
  वापस cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
अटल पूर्णांक
libcrun_attach_process_systemd (काष्ठा libcrun_cgroup_status *cgroup_status,
                                स्थिर अक्षर *path, pid_t pid,
                                libcrun_error_t *err)
{
  स्थिर अक्षर *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free अक्षर *scope_path = शून्य;
  sd_bus_message *reply = शून्य;
  स्थिर अक्षर *subcgroup;
  स्थिर अक्षर *last;
  sd_bus *bus = शून्य;
  माप_प्रकार len;
  पूर्णांक sd_err, ret;

  अगर (is_empty_string (scope))
    वापस crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  अगर (scope_path == शून्य)
    वापस crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = म_खोजप (scope_path, '/');
  अगर (last == शून्य || म_भेद (last + 1, scope) != 0)
    वापस crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = म_माप (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  अगर (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  वापस cleanup_sd_bus_and_return (bus, शून्य, reply, &error, ret);
}

#अन्यथा
अटल पूर्णांक
libcrun_cgroup_enter_systemd (काष्ठा libcrun_cgroup_args *args,
                              काष्ठा libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (व्योम) args;
  (व्योम) out;

  वापस crun_make_error (err, ENOTSUP, "systemd not supported");
}

अटल पूर्णांक
libcrun_destroy_cgroup_systemd (काष्ठा libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (व्योम) cgroup_status;

  वापस crun_make_error (err, ENOTSUP, "systemd not supported");
}

अटल पूर्णांक
libcrun_update_resources_systemd (काष्ठा libcrun_cgroup_status *cgroup_status,
                                  स्थिर अक्षर *state_मूल,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (व्योम) cgroup_status;
  (व्योम) state_मूल;
  (व्योम) resources;

  वापस crun_make_error (err, ENOTSUP, "systemd not supported");
}

अटल पूर्णांक
libcrun_attach_process_systemd (काष्ठा libcrun_cgroup_status *cgroup_status,
                                स्थिर अक्षर *path, pid_t pid,
                                libcrun_error_t *err)
{
  (व्योम) cgroup_status;
  (व्योम) path;
  (व्योम) pid;

  वापस crun_make_error (err, ENOTSUP, "systemd not supported");
}
#पूर्ण_अगर

काष्ठा libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = शून्य,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
