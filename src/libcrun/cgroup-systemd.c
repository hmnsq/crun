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

#شامل <config.h>
#شامل "cgroup.h"
#شامل "cgroup-internal.h"
#شامل "cgroup-systemd.h"
#شامل "cgroup-resources.h"
#شامل "cgroup-utils.h"
#شامل "ebpf.h"
#شامل "utils.h"
#شامل "status.h"
#شامل <string.h>
#شامل <sys/types.h>
#شامل <signal.h>
#شامل <sys/vfs.h>
#شامل <inttypes.h>
#شامل <fcntl.h>
#شامل <time.h>
#شامل <sys/stat.h>
#شامل <sys/vfs.h>
#شامل <linux/magic.h>

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

ایستا صحیح
register_missing_property_from_message (ثابت نویسه *state_dir, ثابت نویسه *message, libcrun_error_t *err)
{
  cleanup_free نویسه *file_path = پوچ;
  cleanup_free نویسه *dir_path = پوچ;
  cleanup_free نویسه *property = پوچ;
  نویسه *p;
  صحیح ret;

  اگر (! has_prefix (message, "Cannot set property "))
    بازگشت 0;

  property = xstrdup (message + اندازه ("Cannot set property ") - 1);
  p = strchr (property, ',');
  اگر (! p)
    بازگشت 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_paths (&file_path, err, dir_path, property, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, پوچ, 0, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 1;
}

ایستا bool
property_missing_p (نویسه **missing_properties, ثابت نویسه *property)
{
  size_t i;

  برای (i = 0; missing_properties && missing_properties[i]; i++)
    اگر (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        بازگشت true;
      }

  بازگشت false;
}

ایستا تهی
get_systemd_scope_and_slice (ثابت نویسه *id, bool user_slice, ثابت نویسه *cgroup_path,
                             نویسه **scope, نویسه **slice)
{
  ثابت نویسه *n;
  نویسه *p;

  اگر (cgroup_path == پوچ || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      بازگشت;
    }

  n = strchr (cgroup_path, ':');
  اگر (n == پوچ)
    xasprintf (scope, "%s.scope", cgroup_path);
  وگرنه
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      اگر (p)
        *p = '-';
    }
  اگر (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      اگر (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      اگر (is_empty_string (*slice))
        {
          آزاد (*slice);
          اگر (user_slice)
            *slice = xstrdup ("user.slice");
          وگرنه
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
ایستا صحیح
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ثابت نویسه *path, libcrun_error_t *err)
{
  cleanup_free نویسه *cgroup_path = پوچ;
  cleanup_close صحیح dirfd = -1;
  bool need_set_parent = true;
  نویسه fmt_buf[64];
  صحیح len;
  صحیح ret;

  اگر (resources == پوچ || resources->cpu == پوچ)
    بازگشت 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  اگر (is_empty_string (path))
    بازگشت crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  اگر (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_ریشه, "cpu", path, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  اگر (UNLIKELY (dirfd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", cgroup_path);

  اگر (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      اگر (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, اندازه (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      اگر (UNLIKELY (len >= (صحیح) اندازه (fmt_buf)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      اگر (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت 0;
}

ایستا صحیح
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ثابت نویسه *path, libcrun_error_t *err)
{
  cleanup_free نویسه *cgroup_path = پوچ;
  صحیح parent;
  صحیح ret;

  اگر (resources == پوچ || resources->cpu == پوچ)
    بازگشت 0;

  اگر (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    بازگشت 0;

  برای (parent = 0; parent < 2; parent++)
    {
      cleanup_close صحیح dirfd = -1;

      اگر (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_ریشه, path ? path : "", (parent ? ".." : پوچ), پوچ);
      وگرنه
        ret = append_paths (&cgroup_path, err, CGROUP_ریشه, "/cpu", path ? path : "", (parent ? ".." : پوچ), پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      اگر (UNLIKELY (dirfd < 0))
        بازگشت crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      اگر (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }

  بازگشت 0;
}

ایستا صحیح
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ثابت نویسه *path, libcrun_error_t *err)
{
  cleanup_free نویسه *cgroup_path = پوچ;
  صحیح parent;
  صحیح ret;
  صحیح levels;

  ret = append_paths (&cgroup_path, err, CGROUP_ریشه, "/cpuset", path ? path : "", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  برای (parent = 0; parent < levels; parent++)
    {
      cleanup_close صحیح dirfd_cpuset = -1;
      cleanup_free نویسه *path_to_cpuset = پوچ;

      ret = append_paths (&path_to_cpuset, err, CGROUP_ریشه "/cpuset", path, (parent ? ".." : پوچ), پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      اگر (UNLIKELY (dirfd_cpuset < 0))
        بازگشت crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
ایستا صحیح
compute_finalized_path (نویسه **path, ثابت نویسه *from, ثابت نویسه *suffix, libcrun_error_t *err)
{
  اگر (suffix == پوچ)
    {
      *path = xstrdup (from);
      بازگشت 0;
    }
  بازگشت append_paths (path, err, from, suffix, پوچ);
}

ایستا صحیح
systemd_finalize (ساختار libcrun_cgroup_args *args, نویسه **path_out,
                  صحیح cgroup_mode, ثابت نویسه *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free نویسه *content = پوچ;
  cleanup_free نویسه *path = پوچ;
  pid_t pid = args->pid;
  صحیح ret;
  نویسه *from, *to;
  نویسه *saveptr = پوچ;

  cleanup_free نویسه *proc_path = پوچ;
  cleanup_close صحیح fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  اگر (UNLIKELY (fd < 0))
    بازگشت fd;

  ret = read_all_fd (fd, "cgroup path", &content, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  انتخاب (cgroup_mode)
    {
    حالت CGROUP_MODE_HYBRID:
    حالت CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      اگر (LIKELY (from != پوچ))
        from += 8;
      وگرنه
        {
          from = strstr (content, ":pids:");
          اگر (UNLIKELY (from == پوچ))
            بازگشت crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      اگر (UNLIKELY (to == پوچ))
        بازگشت crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      اگر (geteuid ())
        بازگشت 0;

      ret = setup_rt_runtime (resources, path, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      برای (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (پوچ, "\n", &saveptr))
        {
          نویسه *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          اگر (subsystem[0] == '\0')
            {
              اگر (cgroup_mode == CGROUP_MODE_LEGACY)
                ادامه;

              subsystem = "unified";
            }

          اگر (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              اگر (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  اگر (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      ادامه;
                    }
                  بازگشت ret;
                }
            }
        }

      بشکن;

    حالت CGROUP_MODE_UNIFIED:
      {
        cleanup_free نویسه *dir = پوچ;

        from = strstr (content, "0::");
        اگر (UNLIKELY (from == پوچ))
          بازگشت crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        اگر (UNLIKELY (to == پوچ))
          بازگشت crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        ret = append_paths (&dir, err, CGROUP_ریشه, path, پوچ);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        ret = move_process_to_cgroup (pid, پوچ, path, err);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        ret = enable_controllers (path, err);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        اگر (suffix)
          {
            ret = chown_cgroups (path, args->ریشه_uid, args->ریشه_gid, err);
            اگر (UNLIKELY (ret < 0))
              بازگشت ret;
          }
      }
      بشکن;

    پیشفرض:
      بازگشت crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  *path_out = path;
  path = پوچ;

  بازگشت 0;
}

ساختار systemd_job_removed_s
{
  ثابت نویسه *path;
  ثابت نویسه *op;
  صحیح terminated;
  libcrun_error_t err;
};

ایستا صحیح
systemd_job_removed (sd_bus_message *m, تهی *userdata, sd_bus_error *error arg_unused)
{
  ثابت نویسه *path, *unit, *result;
  uint32_t id;
  صحیح ret;
  ساختار systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  اگر (ret < 0)
    بازگشت -1;

  اگر (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      اگر (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          بازگشت -1;
        }
    }
  بازگشت 0;
}

ایستا صحیح
systemd_check_job_status_setup (sd_bus *bus, ساختار systemd_job_removed_s *data, libcrun_error_t *err)
{
  صحیح ret;

  ret = sd_bus_match_signal_async (bus, پوچ, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, پوچ, data);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, -ret, "sd-bus match signal");

  بازگشت 0;
}

ایستا صحیح
systemd_check_job_status (sd_bus *bus, ساختار systemd_job_removed_s *data, ثابت نویسه *path, ثابت نویسه *op,
                          libcrun_error_t *err)
{
  صحیح sd_err;

  data->path = path;
  data->op = op;
  وقتی (! data->terminated)
    {
      sd_err = sd_bus_process (bus, پوچ);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus process");

      اگر (sd_err != 0)
        ادامه;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus wait");
    }

  اگر (data->err != پوچ)
    {
      *err = data->err;
      بازگشت -1;
    }

  بازگشت 0;
}

صحیح
parse_sd_array (نویسه *s, نویسه **out, نویسه **next, libcrun_error_t *err)
{
  نویسه endchr;
  نویسه *it, *dest;
  bool escaped = false;

  *out = پوچ;
  *next = پوچ;

  وقتی (isspace (*s))
    s++;
  اگر (*s == '\0')
    بازگشت 0;
  وگرنه اگر (*s != '\'' && *s != '"')
    بازگشت crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  وقتی (1)
    {
      اگر (*it == '\0')
        بازگشت crun_make_error (err, 0, "invalid string `%s`", s);
      اگر (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          وقتی (isspace (*it))
            it++;
          اگر (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              بازگشت 0;
            }

          اگر (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              بازگشت 0;
            }

          بازگشت crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      اگر (! escaped)
        *dest++ = *it;
      it++;
    }

  بازگشت 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
ایستا صحیح
append_systemd_annotation (sd_bus_message *m, ثابت نویسه *name, size_t name_len, ثابت نویسه *مقدار,
                           libcrun_error_t *err)
{
  cleanup_free نویسه *tmp_name = پوچ;
  uint32_t factor = 1;
  ثابت نویسه *it;
  صحیح sd_err;

  وقتی (*مقدار == ' ')
    مقدار++;

  it = مقدار;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  اگر (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  اگر (factor != 1)
    {
      ایستا ثابت نویسه *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", پوچ };
      ثابت نویسه *رقم = it;
      نویسه *endptr = پوچ;
      دوگانه v;
      size_t i;

      برای (i = 0; types[i]; i++)
        اگر (has_prefix (رقم, types[i]))
          {
            رقم += strlen (types[i]);
            بشکن;
          }

      errno = 0;
      v = strtod (رقم, &endptr);
      اگر (UNLIKELY (errno != 0 || endptr == رقم || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        بازگشت crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, مقدار);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      بازگشت 0;
    }

  اگر ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      بازگشت 0;
    }
  وگرنه اگر (*it == '\'')
    {
      cleanup_free نویسه *v_start = پوچ;
      نویسه *end;

      it = v_start = xstrdup (مقدار);

      end = strchr (v_start + 1, '\'');
      اگر (end == پوچ)
        بازگشت crun_make_error (err, 0, "invalid variant `%s`", مقدار);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      بازگشت 0;
    }
  وگرنه اگر (*it == '[')
    {
      cleanup_free نویسه *v_start = پوچ;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free نویسه **parts = xmalloc (اندازه (نویسه *) * parts_size);
      نویسه *part;

      part = v_start = xstrdup (it + 1);
      وقتی (1)
        {
          نویسه *out = پوچ, *next = پوچ;
          صحیح ret;

          ret = parse_sd_array (part, &out, &next, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          parts[n_parts++] = out;
          اگر (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, اندازه (نویسه *) * parts_size);
            }
          parts[n_parts] = پوچ;
          اگر (next == پوچ)
            بشکن;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus close container");

      بازگشت 0;
    }
  وگرنه اگر (has_prefix (it, "uint64 "))
    {
      نویسه *endptr = پوچ;
      uint64_t v;

      errno = 0;
      v = strtoull (it + اندازه ("uint64"), &endptr, 10);
      اگر (UNLIKELY (errno != 0 || *endptr))
        بازگشت crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      بازگشت 0;
    }
  وگرنه اگر (has_prefix (it, "int64 "))
    {
      نویسه *endptr = پوچ;
      int64_t v;

      errno = 0;
      v = strtoll (it + اندازه ("int64"), &endptr, 10);
      اگر (UNLIKELY (errno != 0 || *endptr))
        بازگشت crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      بازگشت 0;
    }
  وگرنه اگر (has_prefix (it, "uint32 "))
    {
      نویسه *endptr = پوچ;
      uint32_t v;

      errno = 0;
      v = strtoul (it + اندازه ("uint32"), &endptr, 10);
      اگر (UNLIKELY (errno != 0 || *endptr))
        بازگشت crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      بازگشت 0;
    }
  وگرنه اگر (has_prefix (it, "int32 ") || strchr (it, ' ') == پوچ)
    {
      نویسه *endptr = پوچ;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      اگر (has_prefix (it, "int32 "))
        v = strtol (it + اندازه ("int32"), &endptr, 10);
      وگرنه
        v = strtol (it, &endptr, 10);
      اگر (UNLIKELY (errno != 0 || *endptr))
        بازگشت crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      بازگشت 0;
    }

  بازگشت crun_make_error (err, 0, "unknown type for `%s`", name);
}

ایستا صحیح
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  صحیح rootless;
  صحیح sd_err = 0;

  rootless = is_rootless (err);
  اگر (UNLIKELY (rootless < 0))
    بازگشت rootless;

  اگر (rootless)
    sd_err = sd_bus_default_user (bus);
  اگر (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  اگر (sd_err < 0)
    بازگشت crun_make_error (err, -sd_err, "cannot open sd-bus");

  بازگشت 0;
}

ایستا ثابت نویسه *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت نویسه *name)
{
  size_t i;

  اگر (resources == پوچ || resources->unified == پوچ)
    بازگشت پوچ;

  برای (i = 0; i < resources->unified->len; i++)
    اگر (strcmp (resources->unified->کلیدها[i], name) == 0)
      بازگشت resources->unified->مقادیر[i];
  بازگشت پوچ;
}

ایستا صحیح
get_مقدار_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت نویسه *name,
                            uint64_t *مقدار, libcrun_error_t *err)
{
  نویسه *endptr = پوچ;
  size_t i;

  اگر (resources == پوچ || resources->unified == پوچ)
    بازگشت 0;

  برای (i = 0; i < resources->unified->len; i++)
    اگر (strcmp (resources->unified->کلیدها[i], name) == 0)
      {
        اگر (strcmp (resources->unified->مقادیر[i], "max") == 0)
          {
            *مقدار = UINT64_MAX;
            بازگشت 1;
          }

        errno = 0;
        *مقدار = strtoull (resources->unified->مقادیر[i], &endptr, 10);
        اگر (UNLIKELY (errno))
          بازگشت crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->مقادیر[i]);
        اگر (endptr && *endptr)
          بازگشت crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->مقادیر[i]);
        بازگشت 1;
      }
  بازگشت 0;
}

ایستا درونخطی صحیح
get_memory_کم (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  اگر (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      بازگشت 1;
    }

  بازگشت get_مقدار_from_unified_map (resources, "memory.low", limit, err);
}

ایستا درونخطی صحیح
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  اگر (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      بازگشت 1;
    }

  بازگشت get_مقدار_from_unified_map (resources, "pids.max", limit, err);
}

ایستا درونخطی صحیح
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  اگر (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      بازگشت 1;
    }

  بازگشت get_مقدار_from_unified_map (resources, "memory.max", limit, err);
}

ایستا درونخطی صحیح
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  اگر (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      اگر (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          بازگشت 1;
        }
      /* 0 means unset.  */
      اگر (memory->swap == 0)
        بازگشت 0;
      اگر (mem <= 0)
        بازگشت crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      اگر (memory->swap < mem)
        بازگشت crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      بازگشت 1;
    }

  بازگشت get_مقدار_from_unified_map (resources, "memory.swap.max", limit, err);
}

ایستا درونخطی صحیح
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t مقدار;
  صحیح ret;

  ret = get_مقدار_from_unified_map (resources, "cpu.idle", &مقدار, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  اگر (ret > 0 && مقدار == 1)
    has_idle = true;
  اگر (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_مقدار_from_unified_map (resources, "cpu.weight", weight, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  اگر (ret > 0)
    {
      اگر (has_idle)
        بازگشت crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      بازگشت 1;
    }

  اگر (resources->cpu && resources->cpu->shares_present)
    {
      اگر (has_idle)
        بازگشت crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      اگر (resources->cpu->shares == 0)
        بازگشت 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      بازگشت 1;
    }
  اگر (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      بازگشت 1;
    }
  بازگشت 0;
}

ایستا صحیح
read_uint64_مقدار (ثابت نویسه *str, uint64_t *مقدار, libcrun_error_t *err)
{
  نویسه *endptr = پوچ;

  errno = 0;
  *مقدار = strtoull (str, &endptr, 10);
  اگر (UNLIKELY (errno))
    بازگشت crun_make_error (err, errno, "invalid value: %s", str);
  اگر (endptr && *endptr)
    بازگشت crun_make_error (err, 0, "invalid value: %s", str);
  بازگشت 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
ایستا درونخطی uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  بازگشت bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

ایستا صحیح
append_io_weight (sd_bus_message *m, نویسه **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  صحیح sd_err;
  size_t i;
  صحیح ret;

#  define APPEND_IO_WEIGHT(مقدار)                                                      \
    انجامبده                                                                                 \
      {                                                                                \
        اگر (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            اگر (UNLIKELY (sd_err < 0))                                                 \
              بازگشت crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } وقتی (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, مقدار)                                                      \
    انجامبده                                                                                                \
      {                                                                                               \
        اگر (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            اگر (UNLIKELY (sd_err < 0))                                                                \
              بازگشت crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } وقتی (0)

  اگر (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  برای (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      نویسه name[64];
      صحیح len;

      len = snprintf (name, اندازه (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      اگر (UNLIKELY (len >= (صحیح) اندازه (name)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  برای (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free نویسه *مقدار = پوچ;
      bool need_conversion = false;
      نویسه *saveptr = پوچ;
      نویسه *سطر;

      اگر (strcmp (resources->unified->کلیدها[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      وگرنه اگر (strcmp (resources->unified->کلیدها[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      وگرنه
        {
          /* Skip other keys.  */
          ادامه;
        }

      مقدار = xstrdup (resources->unified->مقادیر[i]);

      برای (سطر = strtok_r (مقدار, "\n", &saveptr); سطر; سطر = strtok_r (پوچ, "\n", &saveptr))
        {
          نویسه *sep;

          sep = strchr (سطر, ' ');
          وقتی (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_مقدار (sep ? sep : سطر, &weight, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          اگر (need_conversion)
            weight = IO_WEIGHT (weight);

          اگر (sep == پوچ || strcmp (سطر, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          وگرنه
            APPEND_IO_DEVICE_WEIGHT (سطر, weight);
        }
    }

  بازگشت 0;
}

/* Adapted from systemd.  */
ایستا صحیح
bus_append_byte_array (sd_bus_message *m, ثابت نویسه *field, ثابت تهی *buf, size_t n, libcrun_error_t *err)
{
  صحیح ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  اگر (ret < 0)
    بازگشت crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  اگر (ret < 0)
    بازگشت crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  اگر (ret < 0)
    بازگشت crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  اگر (ret < 0)
    بازگشت crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  اگر (ret < 0)
    بازگشت crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  اگر (ret < 0)
    بازگشت crun_make_error (err, -ret, "sd_bus_message_close_container");

  بازگشت 1;
}

ایستا صحیح
append_uint64_from_unified_map (sd_bus_message *m,
                                نویسه **missing_properties,
                                ثابت نویسه *attr,
                                ثابت نویسه *کلید,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t مقدار;
  صحیح ret;

  اگر (property_missing_p (missing_properties, attr))
    بازگشت 0;

  ret = get_مقدار_from_unified_map (resources, کلید, &مقدار, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  اگر (ret)
    {
      صحیح sd_err = sd_bus_message_append (m, "(sv)", attr, "t", مقدار);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  بازگشت 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
ایستا صحیح
append_device_allow (sd_bus_message *m,
                     ثابت نویسه type,
                     صحیح major,
                     صحیح minor,
                     ثابت نویسه *access,
                     libcrun_error_t *err)
{
  نویسه device[64];
  صحیح sd_err;
  صحیح len;

  اگر (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      بازگشت 0;
    }

  اگر (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, اندازه (device), "%s-*", type == 'c' ? "char" : "block");
  وگرنه اگر (IS_WILDCARD (minor))
    len = snprintf (device, اندازه (device), type == 'c' ? "char-%d" : "block-%d", major);
  وگرنه
    len = snprintf (device, اندازه (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  اگر (UNLIKELY (len >= (صحیح) اندازه (device)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  اگر (UNLIKELY (sd_err < 0))
    بازگشت crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  بازگشت 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

ایستا صحیح
access_to_mode (ثابت نویسه *access, libcrun_error_t *err)
{
  صحیح mode = 0;

  وقتی (access && *access)
    {
      صحیح c = *access++;

      انتخاب (c)
        {
        حالت 'r':
          mode |= MODE_R;
          بشکن;

        حالت 'w':
          mode |= MODE_W;
          بشکن;

        حالت 'm':
          mode |= MODE_M;
          بشکن;

        پیشفرض:
          بازگشت crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  بازگشت mode;
}

/* check if the type b is included in type a.  */
ایستا bool
has_same_type (ثابت نویسه *a, ثابت نویسه *b)
{
  bool a_all = a == پوچ || strcmp (a, "a") == 0;
  اگر (b == پوچ || strcmp (b, "a") == 0)
    بازگشت a_all;

  بازگشت a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
ایستا صحیح
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  صحیح req_mode;

#  define CMP_DEV_رقم(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  اگر (UNLIKELY (req_mode < 0))
    بازگشت req_mode;

  برای (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      صحیح mode;

      اگر (! CMP_DEV_رقم (devices[n]->minor, devices[current_rule]->minor))
        ادامه;
      اگر (! CMP_DEV_رقم (devices[n]->major, devices[current_rule]->major))
        ادامه;

      اگر (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        ادامه;

      mode = access_to_mode (devices[i]->access, err);
      اگر (UNLIKELY (mode < 0))
        بازگشت mode;

      اگر (devices[current_rule]->allow && (mode & req_mode))
        بازگشت 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      اگر (req_mode == 0)
        بازگشت 1;
    }

  بازگشت 0;
#  undef CMP_DEV_رقم
}

ایستا size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  اگر (n == 0)
    بازگشت 1;

  /* Find the first rule that is after the last "block all".  */
  برای (i = n - 1; i-- > 0;)
    {
      اگر ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        بازگشت i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  برای (i = 0; i < n; i++)
    اگر (devices[i]->allow)
      بازگشت i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  بازگشت n + 1;
}

ایستا bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  صحیح remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  برای (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      اگر (! devices[current_rule]->allow)
        بازگشت false;

      اگر (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          اگر (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            بازگشت true;

          اگر (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          وگرنه اگر (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          اگر (remaining == 0)
            بازگشت true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  بازگشت false;
}

ایستا صحیح
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  ساختار default_dev_s *default_devices = get_default_devices ();
  صحیح ret, sd_err;
  size_t i;

  اگر (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      بازگشت 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  اگر (UNLIKELY (sd_err < 0))
    بازگشت crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  برای (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  برای (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      نویسه type;

      اگر (! d->allow)
        {
          صحیح redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          اگر (UNLIKELY (redundant < 0))
            بازگشت redundant;

          اگر (redundant)
            ادامه;

          بازگشت crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      اگر (d->type == پوچ || strcmp (d->type, "a") == 0)
        type = 'a';
      وگرنه اگر (strcmp (d->type, "c") == 0)
        type = 'c';
      وگرنه اگر (strcmp (d->type, "b") == 0)
        type = 'b';
      وگرنه
        بازگشت crun_make_error (err, 0, "unknown device type `%s`", d->type);

      اگر (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
      وگرنه
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }

  بازگشت 0;
}

ایستا bool
has_bpf_fs ()
{
  ساختار statfs stat;

  بازگشت statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

ایستا صحیح
bpfprog_path_from_scope (نویسه **path, ثابت نویسه *scope, libcrun_error_t *err)
{
  cleanup_free نویسه *flat_scope = xstrdup (scope);
  نویسه *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  وقتی ((it = strchr (it, '.')) != پوچ)
    *it = '_';

  بازگشت append_paths (path, err, CRUN_BPF_DIR, flat_scope, پوچ);
}

ایستا صحیح
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 نویسه **missing_properties,
                 ثابت نویسه *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  صحیح ret = 0;
  صحیح in_userns, sd_err;
  ثابت نویسه *property_name = "BPFProgram";
  cleanup_free ساختار bpf_program *program = پوچ;
  cleanup_free نویسه *path = پوچ;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  اگر (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    بازگشت 0;

  in_userns = check_running_in_user_namespace (err);
  اگر (UNLIKELY (in_userns < 0))
    بازگشت in_userns;

  اگر (in_userns)
    بازگشت 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  اگر (UNLIKELY (program == پوچ))
    {
      crun_error_release (err);
      بازگشت 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  اگر (UNLIKELY (ret < 0 && errno != EEXIST))
    بازگشت crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  اگر (is_update)
    {
      cleanup_free ساختار bpf_program *program_loaded = پوچ;

      اگر (resources->devices == پوچ || resources->devices_len == 0)
        {
          *devices_set = true;
          بازگشت 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      /* it is the same as the loaded program.  */
      اگر (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          بازگشت 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free ساختار bpf_program *program_new = پوچ;
        نویسه tmp[64];
        صحیح len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, اندازه (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        اگر (UNLIKELY (len >= (صحیح) اندازه (tmp)))
          بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        اگر (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            بازگشت 0;
          }
      }

      بازگشت crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  اگر (UNLIKELY (sd_err < 0))
    بازگشت crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  بازگشت 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
ایستا صحیح
append_cpu_quota (sd_bus_message *m, نویسه **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ثابت نویسه *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  صحیح sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  اگر (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      صحیح ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      اگر (has_period)
        period = max_period;
    }

  اگر (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  اگر ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      اگر (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          اگر (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      اگر (UNLIKELY (sd_err < 0))
        بازگشت crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  بازگشت 0;
}

ایستا صحیح
append_resources (sd_bus_message *m,
                  bool is_update,
                  ثابت نویسه *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  صحیح cgroup_mode,
                  ثابت نویسه *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t مقدار;
  صحیح sd_err;
  صحیح ret;
  cleanup_free نویسه *dir = پوچ;
  cleanup_free نویسه **missing_properties = پوچ;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  missing_properties = read_dir_entries (dir, err);
  اگر (UNLIKELY (missing_properties == پوچ))
    {
      اگر (crun_error_get_errno (err) != ENOENT)
        بازگشت -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_مقدار(name, مقدار)                                             \
    انجامبده                                                                                 \
      {                                                                                \
        اگر (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", مقدار);              \
            اگر (UNLIKELY (sd_err < 0))                                                 \
              بازگشت crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } وقتی (0)

#  define APPEND_UINT64(name, fn)              \
    انجامبده                                         \
      {                                        \
        ret = fn (resources, &مقدار, err);     \
        اگر (UNLIKELY (ret < 0))                \
          بازگشت ret;                          \
        اگر (ret)                               \
          {                                    \
            APPEND_UINT64_مقدار (name, مقدار); \
          }                                    \
    } وقتی (0)

  اگر (resources == پوچ)
    بازگشت 0;

  APPEND_UINT64 ("MemoryLow", get_memory_کم);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  انتخاب (cgroup_mode)
    {
    حالت CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;

        اگر (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ثابت نویسه *property_name = "AllowedCPUs";
            ثابت نویسه *مقدار = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free نویسه *allowed_cpus = پوچ;
            size_t allowed_cpus_len = 0;

            اگر (مقدار == پوچ)
              مقدار = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (مقدار, &allowed_cpus, &allowed_cpus_len, err);
            اگر (UNLIKELY (ret < 0))
              بازگشت ret;

            اگر (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                اگر (UNLIKELY (ret < 0))
                  بازگشت ret;
              }
          }

        اگر (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ثابت نویسه *property_name = "AllowedMemoryNodes";
            ثابت نویسه *مقدار = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free نویسه *allowed_mems = پوچ;
            size_t allowed_mems_len = 0;

            اگر (مقدار == پوچ)
              مقدار = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (مقدار, &allowed_mems, &allowed_mems_len, err);
            اگر (UNLIKELY (ret < 0))
              بازگشت ret;

            اگر (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                اگر (UNLIKELY (ret < 0))
                  بازگشت ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        اگر (UNLIKELY (ret < 0))
          بازگشت ret;
      }
      بشکن;

    حالت CGROUP_MODE_LEGACY:
    حالت CGROUP_MODE_HYBRID:
      اگر (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_مقدار ("CPUShares", resources->cpu->shares);
      بشکن;

    پیشفرض:
      بازگشت crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_مقدار

  اگر (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت 0;
}

ایستا صحیح
reset_failed_unit (sd_bus *bus, ثابت نویسه *unit)
{
  صحیح sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = پوچ, *reply = پوچ;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  اگر (UNLIKELY (sd_err < 0))
    برو exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  اگر (UNLIKELY (sd_err < 0))
    برو exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  اگر (UNLIKELY (sd_err < 0))
    برو exit;

  sd_err = 0;

exit:
  اگر (m)
    sd_bus_message_unref (m);
  اگر (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  بازگشت sd_err;
}

ایستا صحیح
verify_ebpf_device_filter_installed (ثابت نویسه *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = پوچ;
  cleanup_free نویسه *full_path = پوچ;
  size_t n_progs = 0;
  صحیح ret;

  ret = append_paths (&full_path, err, CGROUP_ریشه, cgroup_path, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (n_progs == 0)
    بازگشت crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  بازگشت 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
ایستا صحیح
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, صحیح ret)
{
  اگر (bus)
    sd_bus_unref (bus);
  اگر (m)
    sd_bus_message_unref (m);
  اگر (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  بازگشت ret;
}

ایستا صحیح
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            صحیح cgroup_mode,
                            string_map *annotations,
                            ثابت نویسه *state_ریشه,
                            ثابت نویسه *scope, ثابت نویسه *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = پوچ;
  sd_bus_message *m = پوچ;
  sd_bus_message *reply = پوچ;
  صحیح sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ثابت نویسه *object = پوچ;
  ساختار systemd_job_removed_s job_data = {};
  صحیح i;
  ثابت نویسه *boolean_opts[10];
  cleanup_free نویسه *state_dir = پوچ;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_ریشه, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = پوچ;

  ret = open_sd_bus_connection (&bus, err);
  اگر (UNLIKELY (ret < 0))
    برو exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  اگر (UNLIKELY (ret < 0))
    برو exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      برو exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      برو exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      برو exit;
    }

  اگر (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      اگر (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          برو exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      برو exit;
    }

  اگر (annotations)
    {
      size_t prefix_len = اندازه (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      برای (i = 0; i < annotations_len; i++)
        {
          ثابت نویسه *کلید;
          ثابت نویسه *مقدار;
          size_t len;

          ret = string_map_get_at (annotations, i, &کلید, &مقدار);
          اگر (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              برو exit;
            }

          اگر (! has_prefix (کلید, SYSTEMD_PROPERTY_PREFIX))
            ادامه;

          len = strlen (کلید);
          اگر (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", کلید);
              برو exit;
            }

          ret = append_systemd_annotation (m, کلید + prefix_len, len - prefix_len,
                                           مقدار, err);
          اگر (UNLIKELY (ret < 0))
            برو exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      برو exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      برو exit;
    }

  برای (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      اگر (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          برو exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  اگر (UNLIKELY (ret < 0))
    برو exit;

  sd_err = sd_bus_message_close_container (m);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      برو exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      برو exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  اگر (UNLIKELY (sd_err < 0))
    {
      اگر (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          اگر (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = پوچ;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      اگر (sd_err < 0)
        {
          اگر (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              اگر (UNLIKELY (ret < 0))
                برو exit;
              اگر (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          برو exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      برو exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  بازگشت cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

ایستا صحیح
libcrun_destroy_systemd_cgroup_scope (ساختار libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = پوچ;
  sd_bus_message *m = پوچ;
  sd_bus_message *reply = پوچ;
  صحیح ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ثابت نویسه *object;
  ثابت نویسه *scope = cgroup_status->scope;
  ساختار systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  اگر (UNLIKELY (ret < 0))
    برو exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  اگر (UNLIKELY (ret < 0))
    برو exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      برو exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      برو exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      برو exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      برو exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  بازگشت cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

ایستا ثابت نویسه *
find_systemd_subgroup (string_map *annotations)
{
  ثابت نویسه *annotation;

  annotation = find_string_map_مقدار (annotations, "run.oci.systemd.subgroup");
  اگر (annotation)
    {
      اگر (annotation[0] == '\0')
        بازگشت پوچ;
      بازگشت annotation;
    }

  بازگشت "container";
}

ایستا صحیح
libcrun_cgroup_enter_systemd (ساختار libcrun_cgroup_args *args,
                              ساختار libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  ثابت نویسه *cgroup_path = args->cgroup_path;
  cleanup_free نویسه *scope_path = پوچ;
  cleanup_free نویسه *scope = پوچ;
  cleanup_free نویسه *path = پوچ;
  cleanup_free نویسه *slice = پوچ;
  صحیح retries_چپ = 32;
  ثابت نویسه *suffix;
  ثابت نویسه *id = args->id;
  pid_t pid = args->pid;
  صحیح cgroup_mode;
  صحیح rootless = 0;
  صحیح ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  اگر (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      اگر (UNLIKELY (rootless < 0))
        بازگشت rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  برای (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_ریشه,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      اگر (LIKELY (ret >= 0))
        بشکن;

      اگر (can_retry && retries_چپ-- > 0)
        {
          crun_error_release (err);
          ادامه;
        }

      بازگشت ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  اگر (is_empty_string (suffix))
    scope_path = xstrdup (path);
  وگرنه
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      اگر (UNLIKELY (scope_path == پوچ))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  اگر (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  out->path = path;
  path = پوچ;

  out->scope = scope;
  scope = پوچ;
  بازگشت 0;
}

نویسه *
get_cgroup_scope_path (ثابت نویسه *cgroup_path, ثابت نویسه *scope)
{
  نویسه *path_to_scope = پوچ;
  نویسه *cur;

  اگر (is_empty_string (cgroup_path))
    بازگشت پوچ;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  وقتی (cur)
    {
      نویسه *next = strchr (cur + 1, '/');
      اگر (next == پوچ)
        بشکن;

      *next = '\0';
      اگر (strcmp (cur, scope) == 0)
        بازگشت path_to_scope;
      *next = '/';

      cur = next;
      وقتی (*cur == '/')
        cur++;
    }

  بازگشت path_to_scope;
}

ایستا صحیح
libcrun_destroy_cgroup_systemd (ساختار libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free نویسه *path_to_scope = پوچ;
  cleanup_free نویسه *bpfprog = پوچ;
  صحیح mode;
  صحیح ret;

  mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (mode < 0))
    بازگشت mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);
  وگرنه
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  بازگشت destroy_cgroup_path (path_to_scope, mode, err);
}

ایستا صحیح
libcrun_update_resources_systemd (ساختار libcrun_cgroup_status *cgroup_status,
                                  ثابت نویسه *state_ریشه,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  ساختار systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free نویسه *state_dir = پوچ;
  sd_bus_message *reply = پوچ;
  sd_bus_message *m = پوچ;
  sd_bus *bus = پوچ;
  صحیح sd_err, ret;
  صحیح cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_ریشه, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (cgroup_mode < 0))
    بازگشت cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  اگر (UNLIKELY (ret < 0))
    برو exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      برو exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      برو exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      برو exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  اگر (UNLIKELY (ret < 0))
    برو exit;

  sd_err = sd_bus_message_close_container (m);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      برو exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      برو exit;
    }

  اگر (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      اگر (UNLIKELY (ret < 0))
        برو exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      اگر (UNLIKELY (ret < 0))
        برو exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  اگر (UNLIKELY (ret < 0))
    برو exit;

  ret = 0;

exit:
  بازگشت cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
ایستا صحیح
libcrun_attach_process_systemd (ساختار libcrun_cgroup_status *cgroup_status,
                                ثابت نویسه *path, pid_t pid,
                                libcrun_error_t *err)
{
  ثابت نویسه *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free نویسه *scope_path = پوچ;
  sd_bus_message *reply = پوچ;
  ثابت نویسه *subcgroup;
  ثابت نویسه *last;
  sd_bus *bus = پوچ;
  size_t len;
  صحیح sd_err, ret;

  اگر (is_empty_string (scope))
    بازگشت crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  اگر (scope_path == پوچ)
    بازگشت crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  اگر (last == پوچ || strcmp (last + 1, scope) != 0)
    بازگشت crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  اگر (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  بازگشت cleanup_sd_bus_and_return (bus, پوچ, reply, &error, ret);
}

#وگرنه
ایستا صحیح
libcrun_cgroup_enter_systemd (ساختار libcrun_cgroup_args *args,
                              ساختار libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (تهی) args;
  (تهی) out;

  بازگشت crun_make_error (err, ENOTSUP, "systemd not supported");
}

ایستا صحیح
libcrun_destroy_cgroup_systemd (ساختار libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (تهی) cgroup_status;

  بازگشت crun_make_error (err, ENOTSUP, "systemd not supported");
}

ایستا صحیح
libcrun_update_resources_systemd (ساختار libcrun_cgroup_status *cgroup_status,
                                  ثابت نویسه *state_ریشه,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (تهی) cgroup_status;
  (تهی) state_ریشه;
  (تهی) resources;

  بازگشت crun_make_error (err, ENOTSUP, "systemd not supported");
}

ایستا صحیح
libcrun_attach_process_systemd (ساختار libcrun_cgroup_status *cgroup_status,
                                ثابت نویسه *path, pid_t pid,
                                libcrun_error_t *err)
{
  (تهی) cgroup_status;
  (تهی) path;
  (تهی) pid;

  بازگشت crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

ساختار libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = پوچ,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
