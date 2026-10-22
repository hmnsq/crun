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

#ضمن <config.h>
#ضمن "cgroup.h"
#ضمن "cgroup-internal.h"
#ضمن "cgroup-systemd.h"
#ضمن "cgroup-resources.h"
#ضمن "cgroup-utils.h"
#ضمن "ebpf.h"
#ضمن "utils.h"
#ضمن "status.h"
#ضمن <string.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <sys/vfs.h>
#ضمن <inttypes.h>
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/vfs.h>
#ضمن <linux/magic.h>

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

ساكن صحيح
register_missing_property_from_message (ثابت حرف *state_dir, ثابت حرف *message, libcrun_error_t *err)
{
  cleanup_free حرف *file_path = عدم;
  cleanup_free حرف *dir_path = عدم;
  cleanup_free حرف *property = عدم;
  حرف *p;
  صحيح ret;

  𐡄𐡍 (! has_prefix (message, "Cannot set property "))
    أرجع 0;

  property = xstrdup (message + حجم ("Cannot set property ") - 1);
  p = strchr (property, ',');
  𐡄𐡍 (! p)
    أرجع 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, عدم);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&file_path, err, dir_path, property, عدم);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, عدم, 0, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 1;
}

ساكن bool
property_missing_p (حرف **missing_properties, ثابت حرف *property)
{
  size_t i;

  لكل (i = 0; missing_properties && missing_properties[i]; i++)
    𐡄𐡍 (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        أرجع true;
      }

  أرجع false;
}

ساكن فراغ
get_systemd_scope_and_slice (ثابت حرف *id, bool user_slice, ثابت حرف *cgroup_path,
                             حرف **scope, حرف **slice)
{
  ثابت حرف *n;
  حرف *p;

  𐡄𐡍 (cgroup_path == عدم || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      أرجع;
    }

  n = strchr (cgroup_path, ':');
  𐡄𐡍 (n == عدم)
    xasprintf (scope, "%s.scope", cgroup_path);
  وإلا
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      𐡄𐡍 (p)
        *p = '-';
    }
  𐡄𐡍 (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      𐡄𐡍 (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      𐡄𐡍 (is_empty_string (*slice))
        {
          حرر (*slice);
          𐡄𐡍 (user_slice)
            *slice = xstrdup ("user.slice");
          وإلا
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
ساكن صحيح
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path = عدم;
  cleanup_close صحيح dirfd = -1;
  bool need_set_parent = true;
  حرف fmt_buf[64];
  صحيح len;
  صحيح ret;

  𐡄𐡍 (resources == عدم || resources->cpu == عدم)
    أرجع 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  𐡄𐡍 (is_empty_string (path))
    أرجع crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  𐡄𐡍 (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "cpu", path, عدم);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  𐡄𐡍 (UNLIKELY (dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_path);

  𐡄𐡍 (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      𐡄𐡍 (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      𐡄𐡍 (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;
    }

  𐡄𐡍 (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      𐡄𐡍 (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      𐡄𐡍 (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path = عدم;
  صحيح parent;
  صحيح ret;

  𐡄𐡍 (resources == عدم || resources->cpu == عدم)
    أرجع 0;

  𐡄𐡍 (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    أرجع 0;

  لكل (parent = 0; parent < 2; parent++)
    {
      cleanup_close صحيح dirfd = -1;

      𐡄𐡍 (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path ? path : "", (parent ? ".." : عدم), عدم);
      وإلا
        ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "/cpu", path ? path : "", (parent ? ".." : عدم), عدم);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      𐡄𐡍 (UNLIKELY (dirfd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      𐡄𐡍 (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  أرجع 0;
}

ساكن صحيح
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path = عدم;
  صحيح parent;
  صحيح ret;
  صحيح levels;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "/cpuset", path ? path : "", عدم);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  لكل (parent = 0; parent < levels; parent++)
    {
      cleanup_close صحيح dirfd_cpuset = -1;
      cleanup_free حرف *path_to_cpuset = عدم;

      ret = append_paths (&path_to_cpuset, err, CGROUP_ROOT "/cpuset", path, (parent ? ".." : عدم), عدم);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      𐡄𐡍 (UNLIKELY (dirfd_cpuset < 0))
        أرجع crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
ساكن صحيح
compute_finalized_path (حرف **path, ثابت حرف *from, ثابت حرف *suffix, libcrun_error_t *err)
{
  𐡄𐡍 (suffix == عدم)
    {
      *path = xstrdup (from);
      أرجع 0;
    }
  أرجع append_paths (path, err, from, suffix, عدم);
}

ساكن صحيح
systemd_finalize (بنية libcrun_cgroup_args *args, حرف **path_out,
                  صحيح cgroup_mode, ثابت حرف *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *path = عدم;
  pid_t pid = args->pid;
  صحيح ret;
  حرف *from, *to;
  حرف *saveptr = عدم;

  cleanup_free حرف *proc_path = عدم;
  cleanup_close صحيح fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  𐡄𐡍 (UNLIKELY (fd < 0))
    أرجع fd;

  ret = read_all_fd (fd, "cgroup path", &content, عدم, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  بدل (cgroup_mode)
    {
    حالة CGROUP_MODE_HYBRID:
    حالة CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      𐡄𐡍 (LIKELY (from != عدم))
        from += 8;
      وإلا
        {
          from = strstr (content, ":pids:");
          𐡄𐡍 (UNLIKELY (from == عدم))
            أرجع crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      𐡄𐡍 (UNLIKELY (to == عدم))
        أرجع crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      𐡄𐡍 (geteuid ())
        أرجع 0;

      ret = setup_rt_runtime (resources, path, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      لكل (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (عدم, "\n", &saveptr))
        {
          حرف *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          𐡄𐡍 (subsystem[0] == '\0')
            {
              𐡄𐡍 (cgroup_mode == CGROUP_MODE_LEGACY)
                استمر;

              subsystem = "unified";
            }

          𐡄𐡍 (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              𐡄𐡍 (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  𐡄𐡍 (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      استمر;
                    }
                  أرجع ret;
                }
            }
        }

      اكسر;

    حالة CGROUP_MODE_UNIFIED:
      {
        cleanup_free حرف *dir = عدم;

        from = strstr (content, "0::");
        𐡄𐡍 (UNLIKELY (from == عدم))
          أرجع crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        𐡄𐡍 (UNLIKELY (to == عدم))
          أرجع crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;

        ret = append_paths (&dir, err, CGROUP_ROOT, path, عدم);
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;

        ret = move_process_to_cgroup (pid, عدم, path, err);
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;

        ret = enable_controllers (path, err);
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;

        𐡄𐡍 (suffix)
          {
            ret = chown_cgroups (path, args->root_uid, args->root_gid, err);
            𐡄𐡍 (UNLIKELY (ret < 0))
              أرجع ret;
          }
      }
      اكسر;

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  *path_out = path;
  path = عدم;

  أرجع 0;
}

بنية systemd_job_removed_s
{
  ثابت حرف *path;
  ثابت حرف *op;
  صحيح terminated;
  libcrun_error_t err;
};

ساكن صحيح
systemd_job_removed (sd_bus_message *m, فراغ *userdata, sd_bus_error *error arg_unused)
{
  ثابت حرف *path, *unit, *result;
  uint32_t id;
  صحيح ret;
  بنية systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  𐡄𐡍 (ret < 0)
    أرجع -1;

  𐡄𐡍 (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      𐡄𐡍 (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          أرجع -1;
        }
    }
  أرجع 0;
}

ساكن صحيح
systemd_check_job_status_setup (sd_bus *bus, بنية systemd_job_removed_s *data, libcrun_error_t *err)
{
  صحيح ret;

  ret = sd_bus_match_signal_async (bus, عدم, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, عدم, data);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "sd-bus match signal");

  أرجع 0;
}

ساكن صحيح
systemd_check_job_status (sd_bus *bus, بنية systemd_job_removed_s *data, ثابت حرف *path, ثابت حرف *op,
                          libcrun_error_t *err)
{
  صحيح sd_err;

  data->path = path;
  data->op = op;
  طالما (! data->terminated)
    {
      sd_err = sd_bus_process (bus, عدم);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus process");

      𐡄𐡍 (sd_err != 0)
        استمر;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus wait");
    }

  𐡄𐡍 (data->err != عدم)
    {
      *err = data->err;
      أرجع -1;
    }

  أرجع 0;
}

صحيح
parse_sd_array (حرف *s, حرف **out, حرف **next, libcrun_error_t *err)
{
  حرف endchr;
  حرف *it, *dest;
  bool escaped = false;

  *out = عدم;
  *next = عدم;

  طالما (isspace (*s))
    s++;
  𐡄𐡍 (*s == '\0')
    أرجع 0;
  وإلا 𐡄𐡍 (*s != '\'' && *s != '"')
    أرجع crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  طالما (1)
    {
      𐡄𐡍 (*it == '\0')
        أرجع crun_make_error (err, 0, "invalid string `%s`", s);
      𐡄𐡍 (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          طالما (isspace (*it))
            it++;
          𐡄𐡍 (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              أرجع 0;
            }

          𐡄𐡍 (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              أرجع 0;
            }

          أرجع crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      𐡄𐡍 (! escaped)
        *dest++ = *it;
      it++;
    }

  أرجع 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
ساكن صحيح
append_systemd_annotation (sd_bus_message *m, ثابت حرف *name, size_t name_len, ثابت حرف *value,
                           libcrun_error_t *err)
{
  cleanup_free حرف *tmp_name = عدم;
  uint32_t factor = 1;
  ثابت حرف *it;
  صحيح sd_err;

  طالما (*value == ' ')
    value++;

  it = value;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  𐡄𐡍 (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  𐡄𐡍 (factor != 1)
    {
      ساكن ثابت حرف *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", عدم };
      ثابت حرف *num = it;
      حرف *endptr = عدم;
      مزدوج v;
      size_t i;

      لكل (i = 0; types[i]; i++)
        𐡄𐡍 (has_prefix (num, types[i]))
          {
            num += strlen (types[i]);
            اكسر;
          }

      errno = 0;
      v = strtod (num, &endptr);
      𐡄𐡍 (UNLIKELY (errno != 0 || endptr == num || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        أرجع crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, value);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }

  𐡄𐡍 ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا 𐡄𐡍 (*it == '\'')
    {
      cleanup_free حرف *v_start = عدم;
      حرف *end;

      it = v_start = xstrdup (value);

      end = strchr (v_start + 1, '\'');
      𐡄𐡍 (end == عدم)
        أرجع crun_make_error (err, 0, "invalid variant `%s`", value);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا 𐡄𐡍 (*it == '[')
    {
      cleanup_free حرف *v_start = عدم;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free حرف **parts = xmalloc (حجم (حرف *) * parts_size);
      حرف *part;

      part = v_start = xstrdup (it + 1);
      طالما (1)
        {
          حرف *out = عدم, *next = عدم;
          صحيح ret;

          ret = parse_sd_array (part, &out, &next, err);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع ret;

          parts[n_parts++] = out;
          𐡄𐡍 (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, حجم (حرف *) * parts_size);
            }
          parts[n_parts] = عدم;
          𐡄𐡍 (next == عدم)
            اكسر;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus close container");

      أرجع 0;
    }
  وإلا 𐡄𐡍 (has_prefix (it, "uint64 "))
    {
      حرف *endptr = عدم;
      uint64_t v;

      errno = 0;
      v = strtoull (it + حجم ("uint64"), &endptr, 10);
      𐡄𐡍 (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا 𐡄𐡍 (has_prefix (it, "int64 "))
    {
      حرف *endptr = عدم;
      int64_t v;

      errno = 0;
      v = strtoll (it + حجم ("int64"), &endptr, 10);
      𐡄𐡍 (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا 𐡄𐡍 (has_prefix (it, "uint32 "))
    {
      حرف *endptr = عدم;
      uint32_t v;

      errno = 0;
      v = strtoul (it + حجم ("uint32"), &endptr, 10);
      𐡄𐡍 (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا 𐡄𐡍 (has_prefix (it, "int32 ") || strchr (it, ' ') == عدم)
    {
      حرف *endptr = عدم;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      𐡄𐡍 (has_prefix (it, "int32 "))
        v = strtol (it + حجم ("int32"), &endptr, 10);
      وإلا
        v = strtol (it, &endptr, 10);
      𐡄𐡍 (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }

  أرجع crun_make_error (err, 0, "unknown type for `%s`", name);
}

ساكن صحيح
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  صحيح rootless;
  صحيح sd_err = 0;

  rootless = is_rootless (err);
  𐡄𐡍 (UNLIKELY (rootless < 0))
    أرجع rootless;

  𐡄𐡍 (rootless)
    sd_err = sd_bus_default_user (bus);
  𐡄𐡍 (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  𐡄𐡍 (sd_err < 0)
    أرجع crun_make_error (err, -sd_err, "cannot open sd-bus");

  أرجع 0;
}

ساكن ثابت حرف *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *name)
{
  size_t i;

  𐡄𐡍 (resources == عدم || resources->unified == عدم)
    أرجع عدم;

  لكل (i = 0; i < resources->unified->len; i++)
    𐡄𐡍 (strcmp (resources->unified->keys[i], name) == 0)
      أرجع resources->unified->values[i];
  أرجع عدم;
}

ساكن صحيح
get_value_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *name,
                            uint64_t *value, libcrun_error_t *err)
{
  حرف *endptr = عدم;
  size_t i;

  𐡄𐡍 (resources == عدم || resources->unified == عدم)
    أرجع 0;

  لكل (i = 0; i < resources->unified->len; i++)
    𐡄𐡍 (strcmp (resources->unified->keys[i], name) == 0)
      {
        𐡄𐡍 (strcmp (resources->unified->values[i], "max") == 0)
          {
            *value = UINT64_MAX;
            أرجع 1;
          }

        errno = 0;
        *value = strtoull (resources->unified->values[i], &endptr, 10);
        𐡄𐡍 (UNLIKELY (errno))
          أرجع crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->values[i]);
        𐡄𐡍 (endptr && *endptr)
          أرجع crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->values[i]);
        أرجع 1;
      }
  أرجع 0;
}

ساكن مضمن صحيح
get_memory_low (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  𐡄𐡍 (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      أرجع 1;
    }

  أرجع get_value_from_unified_map (resources, "memory.low", limit, err);
}

ساكن مضمن صحيح
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  𐡄𐡍 (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      أرجع 1;
    }

  أرجع get_value_from_unified_map (resources, "pids.max", limit, err);
}

ساكن مضمن صحيح
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  𐡄𐡍 (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      أرجع 1;
    }

  أرجع get_value_from_unified_map (resources, "memory.max", limit, err);
}

ساكن مضمن صحيح
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  𐡄𐡍 (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      𐡄𐡍 (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          أرجع 1;
        }
      /* 0 means unset.  */
      𐡄𐡍 (memory->swap == 0)
        أرجع 0;
      𐡄𐡍 (mem <= 0)
        أرجع crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      𐡄𐡍 (memory->swap < mem)
        أرجع crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      أرجع 1;
    }

  أرجع get_value_from_unified_map (resources, "memory.swap.max", limit, err);
}

ساكن مضمن صحيح
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t value;
  صحيح ret;

  ret = get_value_from_unified_map (resources, "cpu.idle", &value, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;
  𐡄𐡍 (ret > 0 && value == 1)
    has_idle = true;
  𐡄𐡍 (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_value_from_unified_map (resources, "cpu.weight", weight, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;
  𐡄𐡍 (ret > 0)
    {
      𐡄𐡍 (has_idle)
        أرجع crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      أرجع 1;
    }

  𐡄𐡍 (resources->cpu && resources->cpu->shares_present)
    {
      𐡄𐡍 (has_idle)
        أرجع crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      𐡄𐡍 (resources->cpu->shares == 0)
        أرجع 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      أرجع 1;
    }
  𐡄𐡍 (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      أرجع 1;
    }
  أرجع 0;
}

ساكن صحيح
read_uint64_value (ثابت حرف *str, uint64_t *value, libcrun_error_t *err)
{
  حرف *endptr = عدم;

  errno = 0;
  *value = strtoull (str, &endptr, 10);
  𐡄𐡍 (UNLIKELY (errno))
    أرجع crun_make_error (err, errno, "invalid value: %s", str);
  𐡄𐡍 (endptr && *endptr)
    أرجع crun_make_error (err, 0, "invalid value: %s", str);
  أرجع 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
ساكن مضمن uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  أرجع bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

ساكن صحيح
append_io_weight (sd_bus_message *m, حرف **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  صحيح sd_err;
  size_t i;
  صحيح ret;

#  define APPEND_IO_WEIGHT(value)                                                      \
    𐡏𐡁𐡃                                                                                 \
      {                                                                                \
        𐡄𐡍 (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            𐡄𐡍 (UNLIKELY (sd_err < 0))                                                 \
              أرجع crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } طالما (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, value)                                                      \
    𐡏𐡁𐡃                                                                                                \
      {                                                                                               \
        𐡄𐡍 (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            𐡄𐡍 (UNLIKELY (sd_err < 0))                                                                \
              أرجع crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } طالما (0)

  𐡄𐡍 (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  لكل (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      حرف name[64];
      صحيح len;

      len = snprintf (name, حجم (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      𐡄𐡍 (UNLIKELY (len >= (صحيح) حجم (name)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  لكل (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free حرف *value = عدم;
      bool need_conversion = false;
      حرف *saveptr = عدم;
      حرف *line;

      𐡄𐡍 (strcmp (resources->unified->keys[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      وإلا 𐡄𐡍 (strcmp (resources->unified->keys[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      وإلا
        {
          /* Skip other keys.  */
          استمر;
        }

      value = xstrdup (resources->unified->values[i]);

      لكل (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (عدم, "\n", &saveptr))
        {
          حرف *sep;

          sep = strchr (line, ' ');
          طالما (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_value (sep ? sep : line, &weight, err);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع ret;
          𐡄𐡍 (need_conversion)
            weight = IO_WEIGHT (weight);

          𐡄𐡍 (sep == عدم || strcmp (line, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          وإلا
            APPEND_IO_DEVICE_WEIGHT (line, weight);
        }
    }

  أرجع 0;
}

/* Adapted from systemd.  */
ساكن صحيح
bus_append_byte_array (sd_bus_message *m, ثابت حرف *field, ثابت فراغ *buf, size_t n, libcrun_error_t *err)
{
  صحيح ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  𐡄𐡍 (ret < 0)
    أرجع crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  𐡄𐡍 (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  𐡄𐡍 (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  𐡄𐡍 (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  𐡄𐡍 (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  𐡄𐡍 (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_close_container");

  أرجع 1;
}

ساكن صحيح
append_uint64_from_unified_map (sd_bus_message *m,
                                حرف **missing_properties,
                                ثابت حرف *attr,
                                ثابت حرف *key,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t value;
  صحيح ret;

  𐡄𐡍 (property_missing_p (missing_properties, attr))
    أرجع 0;

  ret = get_value_from_unified_map (resources, key, &value, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;
  𐡄𐡍 (ret)
    {
      صحيح sd_err = sd_bus_message_append (m, "(sv)", attr, "t", value);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  أرجع 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
ساكن صحيح
append_device_allow (sd_bus_message *m,
                     ثابت حرف type,
                     صحيح major,
                     صحيح minor,
                     ثابت حرف *access,
                     libcrun_error_t *err)
{
  حرف device[64];
  صحيح sd_err;
  صحيح len;

  𐡄𐡍 (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      أرجع 0;
    }

  𐡄𐡍 (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, حجم (device), "%s-*", type == 'c' ? "char" : "block");
  وإلا 𐡄𐡍 (IS_WILDCARD (minor))
    len = snprintf (device, حجم (device), type == 'c' ? "char-%d" : "block-%d", major);
  وإلا
    len = snprintf (device, حجم (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  𐡄𐡍 (UNLIKELY (len >= (صحيح) حجم (device)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    أرجع crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  أرجع 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

ساكن صحيح
access_to_mode (ثابت حرف *access, libcrun_error_t *err)
{
  صحيح mode = 0;

  طالما (access && *access)
    {
      صحيح c = *access++;

      بدل (c)
        {
        حالة 'r':
          mode |= MODE_R;
          اكسر;

        حالة 'w':
          mode |= MODE_W;
          اكسر;

        حالة 'm':
          mode |= MODE_M;
          اكسر;

        افتراضي:
          أرجع crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  أرجع mode;
}

/* check if the type b is included in type a.  */
ساكن bool
has_same_type (ثابت حرف *a, ثابت حرف *b)
{
  bool a_all = a == عدم || strcmp (a, "a") == 0;
  𐡄𐡍 (b == عدم || strcmp (b, "a") == 0)
    أرجع a_all;

  أرجع a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
ساكن صحيح
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  صحيح req_mode;

#  define CMP_DEV_NUM(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  𐡄𐡍 (UNLIKELY (req_mode < 0))
    أرجع req_mode;

  لكل (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      صحيح mode;

      𐡄𐡍 (! CMP_DEV_NUM (devices[n]->minor, devices[current_rule]->minor))
        استمر;
      𐡄𐡍 (! CMP_DEV_NUM (devices[n]->major, devices[current_rule]->major))
        استمر;

      𐡄𐡍 (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        استمر;

      mode = access_to_mode (devices[i]->access, err);
      𐡄𐡍 (UNLIKELY (mode < 0))
        أرجع mode;

      𐡄𐡍 (devices[current_rule]->allow && (mode & req_mode))
        أرجع 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      𐡄𐡍 (req_mode == 0)
        أرجع 1;
    }

  أرجع 0;
#  undef CMP_DEV_NUM
}

ساكن size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  𐡄𐡍 (n == 0)
    أرجع 1;

  /* Find the first rule that is after the last "block all".  */
  لكل (i = n - 1; i-- > 0;)
    {
      𐡄𐡍 ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        أرجع i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  لكل (i = 0; i < n; i++)
    𐡄𐡍 (devices[i]->allow)
      أرجع i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  أرجع n + 1;
}

ساكن bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  صحيح remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  لكل (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      𐡄𐡍 (! devices[current_rule]->allow)
        أرجع false;

      𐡄𐡍 (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          𐡄𐡍 (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            أرجع true;

          𐡄𐡍 (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          وإلا 𐡄𐡍 (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          𐡄𐡍 (remaining == 0)
            أرجع true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  أرجع false;
}

ساكن صحيح
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  بنية default_dev_s *default_devices = get_default_devices ();
  صحيح ret, sd_err;
  size_t i;

  𐡄𐡍 (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      أرجع 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    أرجع crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  لكل (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;
    }

  لكل (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      حرف type;

      𐡄𐡍 (! d->allow)
        {
          صحيح redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          𐡄𐡍 (UNLIKELY (redundant < 0))
            أرجع redundant;

          𐡄𐡍 (redundant)
            استمر;

          أرجع crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      𐡄𐡍 (d->type == عدم || strcmp (d->type, "a") == 0)
        type = 'a';
      وإلا 𐡄𐡍 (strcmp (d->type, "c") == 0)
        type = 'c';
      وإلا 𐡄𐡍 (strcmp (d->type, "b") == 0)
        type = 'b';
      وإلا
        أرجع crun_make_error (err, 0, "unknown device type `%s`", d->type);

      𐡄𐡍 (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  أرجع 0;
}

ساكن bool
has_bpf_fs ()
{
  بنية statfs stat;

  أرجع statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

ساكن صحيح
bpfprog_path_from_scope (حرف **path, ثابت حرف *scope, libcrun_error_t *err)
{
  cleanup_free حرف *flat_scope = xstrdup (scope);
  حرف *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  طالما ((it = strchr (it, '.')) != عدم)
    *it = '_';

  أرجع append_paths (path, err, CRUN_BPF_DIR, flat_scope, عدم);
}

ساكن صحيح
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 حرف **missing_properties,
                 ثابت حرف *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  صحيح ret = 0;
  صحيح in_userns, sd_err;
  ثابت حرف *property_name = "BPFProgram";
  cleanup_free بنية bpf_program *program = عدم;
  cleanup_free حرف *path = عدم;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  𐡄𐡍 (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    أرجع 0;

  in_userns = check_running_in_user_namespace (err);
  𐡄𐡍 (UNLIKELY (in_userns < 0))
    أرجع in_userns;

  𐡄𐡍 (in_userns)
    أرجع 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  𐡄𐡍 (UNLIKELY (program == عدم))
    {
      crun_error_release (err);
      أرجع 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  𐡄𐡍 (UNLIKELY (ret < 0 && errno != EEXIST))
    أرجع crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  𐡄𐡍 (is_update)
    {
      cleanup_free بنية bpf_program *program_loaded = عدم;

      𐡄𐡍 (resources->devices == عدم || resources->devices_len == 0)
        {
          *devices_set = true;
          أرجع 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      /* it is the same as the loaded program.  */
      𐡄𐡍 (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          أرجع 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free بنية bpf_program *program_new = عدم;
        حرف tmp[64];
        صحيح len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, حجم (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        𐡄𐡍 (UNLIKELY (len >= (صحيح) حجم (tmp)))
          أرجع crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;

        𐡄𐡍 (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            أرجع 0;
          }
      }

      أرجع crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    أرجع crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  أرجع 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
ساكن صحيح
append_cpu_quota (sd_bus_message *m, حرف **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ثابت حرف *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  صحيح sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  𐡄𐡍 (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      صحيح ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;
      𐡄𐡍 (has_period)
        period = max_period;
    }

  𐡄𐡍 (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  𐡄𐡍 ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      𐡄𐡍 (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          𐡄𐡍 (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  أرجع 0;
}

ساكن صحيح
append_resources (sd_bus_message *m,
                  bool is_update,
                  ثابت حرف *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  صحيح cgroup_mode,
                  ثابت حرف *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t value;
  صحيح sd_err;
  صحيح ret;
  cleanup_free حرف *dir = عدم;
  cleanup_free حرف **missing_properties = عدم;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, عدم);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  missing_properties = read_dir_entries (dir, err);
  𐡄𐡍 (UNLIKELY (missing_properties == عدم))
    {
      𐡄𐡍 (crun_error_get_errno (err) != ENOENT)
        أرجع -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_VALUE(name, value)                                             \
    𐡏𐡁𐡃                                                                                 \
      {                                                                                \
        𐡄𐡍 (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", value);              \
            𐡄𐡍 (UNLIKELY (sd_err < 0))                                                 \
              أرجع crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } طالما (0)

#  define APPEND_UINT64(name, fn)              \
    𐡏𐡁𐡃                                         \
      {                                        \
        ret = fn (resources, &value, err);     \
        𐡄𐡍 (UNLIKELY (ret < 0))                \
          أرجع ret;                          \
        𐡄𐡍 (ret)                               \
          {                                    \
            APPEND_UINT64_VALUE (name, value); \
          }                                    \
    } طالما (0)

  𐡄𐡍 (resources == عدم)
    أرجع 0;

  APPEND_UINT64 ("MemoryLow", get_memory_low);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  بدل (cgroup_mode)
    {
    حالة CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;

        𐡄𐡍 (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ثابت حرف *property_name = "AllowedCPUs";
            ثابت حرف *value = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free حرف *allowed_cpus = عدم;
            size_t allowed_cpus_len = 0;

            𐡄𐡍 (value == عدم)
              value = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (value, &allowed_cpus, &allowed_cpus_len, err);
            𐡄𐡍 (UNLIKELY (ret < 0))
              أرجع ret;

            𐡄𐡍 (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                𐡄𐡍 (UNLIKELY (ret < 0))
                  أرجع ret;
              }
          }

        𐡄𐡍 (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ثابت حرف *property_name = "AllowedMemoryNodes";
            ثابت حرف *value = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free حرف *allowed_mems = عدم;
            size_t allowed_mems_len = 0;

            𐡄𐡍 (value == عدم)
              value = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (value, &allowed_mems, &allowed_mems_len, err);
            𐡄𐡍 (UNLIKELY (ret < 0))
              أرجع ret;

            𐡄𐡍 (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                𐡄𐡍 (UNLIKELY (ret < 0))
                  أرجع ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        𐡄𐡍 (UNLIKELY (ret < 0))
          أرجع ret;
      }
      اكسر;

    حالة CGROUP_MODE_LEGACY:
    حالة CGROUP_MODE_HYBRID:
      𐡄𐡍 (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_VALUE ("CPUShares", resources->cpu->shares);
      اكسر;

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_VALUE

  𐡄𐡍 (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
reset_failed_unit (sd_bus *bus, ثابت حرف *unit)
{
  صحيح sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = عدم, *reply = عدم;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    اذهب exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    اذهب exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    اذهب exit;

  sd_err = 0;

exit:
  𐡄𐡍 (m)
    sd_bus_message_unref (m);
  𐡄𐡍 (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  أرجع sd_err;
}

ساكن صحيح
verify_ebpf_device_filter_installed (ثابت حرف *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = عدم;
  cleanup_free حرف *full_path = عدم;
  size_t n_progs = 0;
  صحيح ret;

  ret = append_paths (&full_path, err, CGROUP_ROOT, cgroup_path, عدم);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  𐡄𐡍 (n_progs == 0)
    أرجع crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  أرجع 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
ساكن صحيح
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, صحيح ret)
{
  𐡄𐡍 (bus)
    sd_bus_unref (bus);
  𐡄𐡍 (m)
    sd_bus_message_unref (m);
  𐡄𐡍 (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  أرجع ret;
}

ساكن صحيح
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            صحيح cgroup_mode,
                            string_map *annotations,
                            ثابت حرف *state_root,
                            ثابت حرف *scope, ثابت حرف *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = عدم;
  sd_bus_message *m = عدم;
  sd_bus_message *reply = عدم;
  صحيح sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ثابت حرف *object = عدم;
  بنية systemd_job_removed_s job_data = {};
  صحيح i;
  ثابت حرف *boolean_opts[10];
  cleanup_free حرف *state_dir = عدم;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_root, عدم, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = عدم;

  ret = open_sd_bus_connection (&bus, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    اذهب exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    اذهب exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      اذهب exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      اذهب exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      اذهب exit;
    }

  𐡄𐡍 (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          اذهب exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      اذهب exit;
    }

  𐡄𐡍 (annotations)
    {
      size_t prefix_len = حجم (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      لكل (i = 0; i < annotations_len; i++)
        {
          ثابت حرف *key;
          ثابت حرف *value;
          size_t len;

          ret = string_map_get_at (annotations, i, &key, &value);
          𐡄𐡍 (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              اذهب exit;
            }

          𐡄𐡍 (! has_prefix (key, SYSTEMD_PROPERTY_PREFIX))
            استمر;

          len = strlen (key);
          𐡄𐡍 (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", key);
              اذهب exit;
            }

          ret = append_systemd_annotation (m, key + prefix_len, len - prefix_len,
                                           value, err);
          𐡄𐡍 (UNLIKELY (ret < 0))
            اذهب exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      اذهب exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      اذهب exit;
    }

  لكل (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      𐡄𐡍 (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          اذهب exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    اذهب exit;

  sd_err = sd_bus_message_close_container (m);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      اذهب exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      اذهب exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      𐡄𐡍 (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          𐡄𐡍 (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = عدم;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      𐡄𐡍 (sd_err < 0)
        {
          𐡄𐡍 (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              𐡄𐡍 (UNLIKELY (ret < 0))
                اذهب exit;
              𐡄𐡍 (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          اذهب exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      اذهب exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  أرجع cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

ساكن صحيح
libcrun_destroy_systemd_cgroup_scope (بنية libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = عدم;
  sd_bus_message *m = عدم;
  sd_bus_message *reply = عدم;
  صحيح ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ثابت حرف *object;
  ثابت حرف *scope = cgroup_status->scope;
  بنية systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    اذهب exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    اذهب exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  𐡄𐡍 (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      اذهب exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  𐡄𐡍 (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      اذهب exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  𐡄𐡍 (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      اذهب exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  𐡄𐡍 (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      اذهب exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  أرجع cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

ساكن ثابت حرف *
find_systemd_subgroup (string_map *annotations)
{
  ثابت حرف *annotation;

  annotation = find_string_map_value (annotations, "run.oci.systemd.subgroup");
  𐡄𐡍 (annotation)
    {
      𐡄𐡍 (annotation[0] == '\0')
        أرجع عدم;
      أرجع annotation;
    }

  أرجع "container";
}

ساكن صحيح
libcrun_cgroup_enter_systemd (بنية libcrun_cgroup_args *args,
                              بنية libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  ثابت حرف *cgroup_path = args->cgroup_path;
  cleanup_free حرف *scope_path = عدم;
  cleanup_free حرف *scope = عدم;
  cleanup_free حرف *path = عدم;
  cleanup_free حرف *slice = عدم;
  صحيح retries_left = 32;
  ثابت حرف *suffix;
  ثابت حرف *id = args->id;
  pid_t pid = args->pid;
  صحيح cgroup_mode;
  صحيح rootless = 0;
  صحيح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  𐡄𐡍 (UNLIKELY (cgroup_mode < 0))
    أرجع cgroup_mode;

  𐡄𐡍 (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      𐡄𐡍 (UNLIKELY (rootless < 0))
        أرجع rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  لكل (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_root,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      𐡄𐡍 (LIKELY (ret >= 0))
        اكسر;

      𐡄𐡍 (can_retry && retries_left-- > 0)
        {
          crun_error_release (err);
          استمر;
        }

      أرجع ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  𐡄𐡍 (is_empty_string (suffix))
    scope_path = xstrdup (path);
  وإلا
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      𐡄𐡍 (UNLIKELY (scope_path == عدم))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  𐡄𐡍 (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;
    }

  out->path = path;
  path = عدم;

  out->scope = scope;
  scope = عدم;
  أرجع 0;
}

حرف *
get_cgroup_scope_path (ثابت حرف *cgroup_path, ثابت حرف *scope)
{
  حرف *path_to_scope = عدم;
  حرف *cur;

  𐡄𐡍 (is_empty_string (cgroup_path))
    أرجع عدم;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  طالما (cur)
    {
      حرف *next = strchr (cur + 1, '/');
      𐡄𐡍 (next == عدم)
        اكسر;

      *next = '\0';
      𐡄𐡍 (strcmp (cur, scope) == 0)
        أرجع path_to_scope;
      *next = '/';

      cur = next;
      طالما (*cur == '/')
        cur++;
    }

  أرجع path_to_scope;
}

ساكن صحيح
libcrun_destroy_cgroup_systemd (بنية libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free حرف *path_to_scope = عدم;
  cleanup_free حرف *bpfprog = عدم;
  صحيح mode;
  صحيح ret;

  mode = libcrun_get_cgroup_mode (err);
  𐡄𐡍 (UNLIKELY (mode < 0))
    أرجع mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    crun_error_release (err);
  وإلا
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  أرجع destroy_cgroup_path (path_to_scope, mode, err);
}

ساكن صحيح
libcrun_update_resources_systemd (بنية libcrun_cgroup_status *cgroup_status,
                                  ثابت حرف *state_root,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  بنية systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free حرف *state_dir = عدم;
  sd_bus_message *reply = عدم;
  sd_bus_message *m = عدم;
  sd_bus *bus = عدم;
  صحيح sd_err, ret;
  صحيح cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_root, عدم, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  𐡄𐡍 (UNLIKELY (cgroup_mode < 0))
    أرجع cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    اذهب exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      اذهب exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      اذهب exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      اذهب exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    اذهب exit;

  sd_err = sd_bus_message_close_container (m);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      اذهب exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      اذهب exit;
    }

  𐡄𐡍 (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        اذهب exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        اذهب exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    اذهب exit;

  ret = 0;

exit:
  أرجع cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
ساكن صحيح
libcrun_attach_process_systemd (بنية libcrun_cgroup_status *cgroup_status,
                                ثابت حرف *path, pid_t pid,
                                libcrun_error_t *err)
{
  ثابت حرف *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free حرف *scope_path = عدم;
  sd_bus_message *reply = عدم;
  ثابت حرف *subcgroup;
  ثابت حرف *last;
  sd_bus *bus = عدم;
  size_t len;
  صحيح sd_err, ret;

  𐡄𐡍 (is_empty_string (scope))
    أرجع crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  𐡄𐡍 (scope_path == عدم)
    أرجع crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  𐡄𐡍 (last == عدم || strcmp (last + 1, scope) != 0)
    أرجع crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  𐡄𐡍 (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  أرجع cleanup_sd_bus_and_return (bus, عدم, reply, &error, ret);
}

#وإلا
ساكن صحيح
libcrun_cgroup_enter_systemd (بنية libcrun_cgroup_args *args,
                              بنية libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (فراغ) args;
  (فراغ) out;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساكن صحيح
libcrun_destroy_cgroup_systemd (بنية libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (فراغ) cgroup_status;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساكن صحيح
libcrun_update_resources_systemd (بنية libcrun_cgroup_status *cgroup_status,
                                  ثابت حرف *state_root,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (فراغ) cgroup_status;
  (فراغ) state_root;
  (فراغ) resources;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساكن صحيح
libcrun_attach_process_systemd (بنية libcrun_cgroup_status *cgroup_status,
                                ثابت حرف *path, pid_t pid,
                                libcrun_error_t *err)
{
  (فراغ) cgroup_status;
  (فراغ) path;
  (فراغ) pid;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

بنية libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = عدم,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
