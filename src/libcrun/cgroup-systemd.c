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

  אם (! has_prefix (message, "Cannot set property "))
    החזר 0;

  property = xstrdup (message + حجم ("Cannot set property ") - 1);
  p = strchr (property, ',');
  אם (! p)
    החזר 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = append_paths (&file_path, err, dir_path, property, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, عدم, 0, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר 1;
}

ساكن bool
property_missing_p (حرف **missing_properties, ثابت حرف *property)
{
  size_t i;

  עבור (i = 0; missing_properties && missing_properties[i]; i++)
    אם (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        החזר true;
      }

  החזר false;
}

ساكن فراغ
get_systemd_scope_and_slice (ثابت حرف *id, bool user_slice, ثابت حرف *cgroup_path,
                             حرف **scope, حرف **slice)
{
  ثابت حرف *n;
  حرف *p;

  אם (cgroup_path == عدم || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      החזר;
    }

  n = strchr (cgroup_path, ':');
  אם (n == عدم)
    xasprintf (scope, "%s.scope", cgroup_path);
  אחרת
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      אם (p)
        *p = '-';
    }
  אם (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      אם (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      אם (is_empty_string (*slice))
        {
          حرر (*slice);
          אם (user_slice)
            *slice = xstrdup ("user.slice");
          אחרת
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

  אם (resources == عدم || resources->cpu == عدم)
    החזר 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  אם (is_empty_string (path))
    החזר crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  אם (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "cpu", path, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  אם (UNLIKELY (dirfd < 0))
    החזר crun_make_error (err, errno, "open `%s`", cgroup_path);

  אם (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");

      אם (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      אם (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");

      אם (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  החזר 0;
}

ساكن صحيح
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path = عدم;
  صحيح parent;
  صحيح ret;

  אם (resources == عدم || resources->cpu == عدم)
    החזר 0;

  אם (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    החזר 0;

  עבור (parent = 0; parent < 2; parent++)
    {
      cleanup_close صحيح dirfd = -1;

      אם (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path ? path : "", (parent ? ".." : عدم), عدم);
      אחרת
        ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "/cpu", path ? path : "", (parent ? ".." : عدم), عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      אם (UNLIKELY (dirfd < 0))
        החזר crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      אם (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }
    }

  החזר 0;
}

ساكن صحيح
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_path = عدم;
  صحيح parent;
  صحيح ret;
  صحيح levels;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "/cpuset", path ? path : "", عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  עבור (parent = 0; parent < levels; parent++)
    {
      cleanup_close صحيح dirfd_cpuset = -1;
      cleanup_free حرف *path_to_cpuset = عدم;

      ret = append_paths (&path_to_cpuset, err, CGROUP_ROOT "/cpuset", path, (parent ? ".." : عدم), عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      אם (UNLIKELY (dirfd_cpuset < 0))
        החזר crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  החזר 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
ساكن صحيح
compute_finalized_path (حرف **path, ثابت حرف *from, ثابت حرف *suffix, libcrun_error_t *err)
{
  אם (suffix == عدم)
    {
      *path = xstrdup (from);
      החזר 0;
    }
  החזר append_paths (path, err, from, suffix, عدم);
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
  אם (UNLIKELY (fd < 0))
    החזר fd;

  ret = read_all_fd (fd, "cgroup path", &content, عدم, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  بدل (cgroup_mode)
    {
    حالة CGROUP_MODE_HYBRID:
    حالة CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      אם (LIKELY (from != عدم))
        from += 8;
      אחרת
        {
          from = strstr (content, ":pids:");
          אם (UNLIKELY (from == عدم))
            החזר crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      אם (UNLIKELY (to == عدم))
        החזר crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      אם (UNLIKELY (ret < 0))
        החזר ret;

      אם (geteuid ())
        החזר 0;

      ret = setup_rt_runtime (resources, path, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      עבור (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (عدم, "\n", &saveptr))
        {
          حرف *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          אם (subsystem[0] == '\0')
            {
              אם (cgroup_mode == CGROUP_MODE_LEGACY)
                استمر;

              subsystem = "unified";
            }

          אם (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              אם (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  אם (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      استمر;
                    }
                  החזר ret;
                }
            }
        }

      اكسر;

    حالة CGROUP_MODE_UNIFIED:
      {
        cleanup_free حرف *dir = عدم;

        from = strstr (content, "0::");
        אם (UNLIKELY (from == عدم))
          החזר crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        אם (UNLIKELY (to == عدم))
          החזר crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        אם (UNLIKELY (ret < 0))
          החזר ret;

        ret = append_paths (&dir, err, CGROUP_ROOT, path, عدم);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        ret = move_process_to_cgroup (pid, عدم, path, err);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        ret = enable_controllers (path, err);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        אם (suffix)
          {
            ret = chown_cgroups (path, args->root_uid, args->root_gid, err);
            אם (UNLIKELY (ret < 0))
              החזר ret;
          }
      }
      اكسر;

    افتراضي:
      החזר crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  *path_out = path;
  path = عدم;

  החזר 0;
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
  אם (ret < 0)
    החזר -1;

  אם (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      אם (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          החזר -1;
        }
    }
  החזר 0;
}

ساكن صحيح
systemd_check_job_status_setup (sd_bus *bus, بنية systemd_job_removed_s *data, libcrun_error_t *err)
{
  صحيح ret;

  ret = sd_bus_match_signal_async (bus, عدم, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, عدم, data);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, -ret, "sd-bus match signal");

  החזר 0;
}

ساكن صحيح
systemd_check_job_status (sd_bus *bus, بنية systemd_job_removed_s *data, ثابت حرف *path, ثابت حرف *op,
                          libcrun_error_t *err)
{
  صحيح sd_err;

  data->path = path;
  data->op = op;
  כלעוד (! data->terminated)
    {
      sd_err = sd_bus_process (bus, عدم);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus process");

      אם (sd_err != 0)
        استمر;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus wait");
    }

  אם (data->err != عدم)
    {
      *err = data->err;
      החזר -1;
    }

  החזר 0;
}

صحيح
parse_sd_array (حرف *s, حرف **out, حرف **next, libcrun_error_t *err)
{
  حرف endchr;
  حرف *it, *dest;
  bool escaped = false;

  *out = عدم;
  *next = عدم;

  כלעוד (isspace (*s))
    s++;
  אם (*s == '\0')
    החזר 0;
  אחרת אם (*s != '\'' && *s != '"')
    החזר crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  כלעוד (1)
    {
      אם (*it == '\0')
        החזר crun_make_error (err, 0, "invalid string `%s`", s);
      אם (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          כלעוד (isspace (*it))
            it++;
          אם (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              החזר 0;
            }

          אם (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              החזר 0;
            }

          החזר crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      אם (! escaped)
        *dest++ = *it;
      it++;
    }

  החזר 0;
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

  כלעוד (*value == ' ')
    value++;

  it = value;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  אם (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  אם (factor != 1)
    {
      ساكن ثابت حرف *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", عدم };
      ثابت حرف *num = it;
      حرف *endptr = عدم;
      مزدوج v;
      size_t i;

      עבור (i = 0; types[i]; i++)
        אם (has_prefix (num, types[i]))
          {
            num += strlen (types[i]);
            اكسر;
          }

      errno = 0;
      v = strtod (num, &endptr);
      אם (UNLIKELY (errno != 0 || endptr == num || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        החזר crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, value);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      החזר 0;
    }

  אם ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      החזר 0;
    }
  אחרת אם (*it == '\'')
    {
      cleanup_free حرف *v_start = عدم;
      حرف *end;

      it = v_start = xstrdup (value);

      end = strchr (v_start + 1, '\'');
      אם (end == عدم)
        החזר crun_make_error (err, 0, "invalid variant `%s`", value);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      החזר 0;
    }
  אחרת אם (*it == '[')
    {
      cleanup_free حرف *v_start = عدم;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free حرف **parts = xmalloc (حجم (حرف *) * parts_size);
      حرف *part;

      part = v_start = xstrdup (it + 1);
      כלעוד (1)
        {
          حرف *out = عدم, *next = عدم;
          صحيح ret;

          ret = parse_sd_array (part, &out, &next, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;

          parts[n_parts++] = out;
          אם (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, حجم (حرف *) * parts_size);
            }
          parts[n_parts] = عدم;
          אם (next == عدم)
            اكسر;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus close container");

      החזר 0;
    }
  אחרת אם (has_prefix (it, "uint64 "))
    {
      حرف *endptr = عدم;
      uint64_t v;

      errno = 0;
      v = strtoull (it + حجم ("uint64"), &endptr, 10);
      אם (UNLIKELY (errno != 0 || *endptr))
        החזר crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      החזר 0;
    }
  אחרת אם (has_prefix (it, "int64 "))
    {
      حرف *endptr = عدم;
      int64_t v;

      errno = 0;
      v = strtoll (it + حجم ("int64"), &endptr, 10);
      אם (UNLIKELY (errno != 0 || *endptr))
        החזר crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      החזר 0;
    }
  אחרת אם (has_prefix (it, "uint32 "))
    {
      حرف *endptr = عدم;
      uint32_t v;

      errno = 0;
      v = strtoul (it + حجم ("uint32"), &endptr, 10);
      אם (UNLIKELY (errno != 0 || *endptr))
        החזר crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      החזר 0;
    }
  אחרת אם (has_prefix (it, "int32 ") || strchr (it, ' ') == عدم)
    {
      حرف *endptr = عدم;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      אם (has_prefix (it, "int32 "))
        v = strtol (it + حجم ("int32"), &endptr, 10);
      אחרת
        v = strtol (it, &endptr, 10);
      אם (UNLIKELY (errno != 0 || *endptr))
        החזר crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      החזר 0;
    }

  החזר crun_make_error (err, 0, "unknown type for `%s`", name);
}

ساكن صحيح
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  صحيح rootless;
  صحيح sd_err = 0;

  rootless = is_rootless (err);
  אם (UNLIKELY (rootless < 0))
    החזר rootless;

  אם (rootless)
    sd_err = sd_bus_default_user (bus);
  אם (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  אם (sd_err < 0)
    החזר crun_make_error (err, -sd_err, "cannot open sd-bus");

  החזר 0;
}

ساكن ثابت حرف *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *name)
{
  size_t i;

  אם (resources == عدم || resources->unified == عدم)
    החזר عدم;

  עבור (i = 0; i < resources->unified->len; i++)
    אם (strcmp (resources->unified->keys[i], name) == 0)
      החזר resources->unified->values[i];
  החזר عدم;
}

ساكن صحيح
get_value_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *name,
                            uint64_t *value, libcrun_error_t *err)
{
  حرف *endptr = عدم;
  size_t i;

  אם (resources == عدم || resources->unified == عدم)
    החזר 0;

  עבור (i = 0; i < resources->unified->len; i++)
    אם (strcmp (resources->unified->keys[i], name) == 0)
      {
        אם (strcmp (resources->unified->values[i], "max") == 0)
          {
            *value = UINT64_MAX;
            החזר 1;
          }

        errno = 0;
        *value = strtoull (resources->unified->values[i], &endptr, 10);
        אם (UNLIKELY (errno))
          החזר crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->values[i]);
        אם (endptr && *endptr)
          החזר crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->values[i]);
        החזר 1;
      }
  החזר 0;
}

ساكن مضمن صحيح
get_memory_low (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  אם (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      החזר 1;
    }

  החזר get_value_from_unified_map (resources, "memory.low", limit, err);
}

ساكن مضمن صحيح
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  אם (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      החזר 1;
    }

  החזר get_value_from_unified_map (resources, "pids.max", limit, err);
}

ساكن مضمن صحيح
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  אם (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      החזר 1;
    }

  החזר get_value_from_unified_map (resources, "memory.max", limit, err);
}

ساكن مضمن صحيح
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  אם (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      אם (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          החזר 1;
        }
      /* 0 means unset.  */
      אם (memory->swap == 0)
        החזר 0;
      אם (mem <= 0)
        החזר crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      אם (memory->swap < mem)
        החזר crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      החזר 1;
    }

  החזר get_value_from_unified_map (resources, "memory.swap.max", limit, err);
}

ساكن مضمن صحيح
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t value;
  صحيح ret;

  ret = get_value_from_unified_map (resources, "cpu.idle", &value, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;
  אם (ret > 0 && value == 1)
    has_idle = true;
  אם (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_value_from_unified_map (resources, "cpu.weight", weight, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;
  אם (ret > 0)
    {
      אם (has_idle)
        החזר crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      החזר 1;
    }

  אם (resources->cpu && resources->cpu->shares_present)
    {
      אם (has_idle)
        החזר crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      אם (resources->cpu->shares == 0)
        החזר 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      החזר 1;
    }
  אם (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      החזר 1;
    }
  החזר 0;
}

ساكن صحيح
read_uint64_value (ثابت حرف *str, uint64_t *value, libcrun_error_t *err)
{
  حرف *endptr = عدم;

  errno = 0;
  *value = strtoull (str, &endptr, 10);
  אם (UNLIKELY (errno))
    החזר crun_make_error (err, errno, "invalid value: %s", str);
  אם (endptr && *endptr)
    החזר crun_make_error (err, 0, "invalid value: %s", str);
  החזר 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
ساكن مضمن uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  החזר bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

ساكن صحيح
append_io_weight (sd_bus_message *m, حرف **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  صحيح sd_err;
  size_t i;
  صحيح ret;

#  define APPEND_IO_WEIGHT(value)                                                      \
    עשה                                                                                 \
      {                                                                                \
        אם (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            אם (UNLIKELY (sd_err < 0))                                                 \
              החזר crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } כלעוד (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, value)                                                      \
    עשה                                                                                                \
      {                                                                                               \
        אם (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            אם (UNLIKELY (sd_err < 0))                                                                \
              החזר crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } כלעוד (0)

  אם (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  עבור (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      حرف name[64];
      صحيح len;

      len = snprintf (name, حجم (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      אם (UNLIKELY (len >= (صحيح) حجم (name)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  עבור (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free حرف *value = عدم;
      bool need_conversion = false;
      حرف *saveptr = عدم;
      حرف *line;

      אם (strcmp (resources->unified->keys[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      אחרת אם (strcmp (resources->unified->keys[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      אחרת
        {
          /* Skip other keys.  */
          استمر;
        }

      value = xstrdup (resources->unified->values[i]);

      עבור (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (عدم, "\n", &saveptr))
        {
          حرف *sep;

          sep = strchr (line, ' ');
          כלעוד (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_value (sep ? sep : line, &weight, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
          אם (need_conversion)
            weight = IO_WEIGHT (weight);

          אם (sep == عدم || strcmp (line, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          אחרת
            APPEND_IO_DEVICE_WEIGHT (line, weight);
        }
    }

  החזר 0;
}

/* Adapted from systemd.  */
ساكن صحيح
bus_append_byte_array (sd_bus_message *m, ثابت حرف *field, ثابت فراغ *buf, size_t n, libcrun_error_t *err)
{
  صحيح ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  אם (ret < 0)
    החזר crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  אם (ret < 0)
    החזר crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  אם (ret < 0)
    החזר crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  אם (ret < 0)
    החזר crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  אם (ret < 0)
    החזר crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  אם (ret < 0)
    החזר crun_make_error (err, -ret, "sd_bus_message_close_container");

  החזר 1;
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

  אם (property_missing_p (missing_properties, attr))
    החזר 0;

  ret = get_value_from_unified_map (resources, key, &value, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;
  אם (ret)
    {
      صحيح sd_err = sd_bus_message_append (m, "(sv)", attr, "t", value);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  החזר 0;
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

  אם (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      החזר 0;
    }

  אם (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, حجم (device), "%s-*", type == 'c' ? "char" : "block");
  אחרת אם (IS_WILDCARD (minor))
    len = snprintf (device, حجم (device), type == 'c' ? "char-%d" : "block-%d", major);
  אחרת
    len = snprintf (device, حجم (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  אם (UNLIKELY (len >= (صحيح) حجم (device)))
    החזר crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  אם (UNLIKELY (sd_err < 0))
    החזר crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  החזר 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

ساكن صحيح
access_to_mode (ثابت حرف *access, libcrun_error_t *err)
{
  صحيح mode = 0;

  כלעוד (access && *access)
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
          החזר crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  החזר mode;
}

/* check if the type b is included in type a.  */
ساكن bool
has_same_type (ثابت حرف *a, ثابت حرف *b)
{
  bool a_all = a == عدم || strcmp (a, "a") == 0;
  אם (b == عدم || strcmp (b, "a") == 0)
    החזר a_all;

  החזר a_all || strcmp (a, b) == 0;
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
  אם (UNLIKELY (req_mode < 0))
    החזר req_mode;

  עבור (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      صحيح mode;

      אם (! CMP_DEV_NUM (devices[n]->minor, devices[current_rule]->minor))
        استمر;
      אם (! CMP_DEV_NUM (devices[n]->major, devices[current_rule]->major))
        استمر;

      אם (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        استمر;

      mode = access_to_mode (devices[i]->access, err);
      אם (UNLIKELY (mode < 0))
        החזר mode;

      אם (devices[current_rule]->allow && (mode & req_mode))
        החזר 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      אם (req_mode == 0)
        החזר 1;
    }

  החזר 0;
#  undef CMP_DEV_NUM
}

ساكن size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  אם (n == 0)
    החזר 1;

  /* Find the first rule that is after the last "block all".  */
  עבור (i = n - 1; i-- > 0;)
    {
      אם ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        החזר i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  עבור (i = 0; i < n; i++)
    אם (devices[i]->allow)
      החזר i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  החזר n + 1;
}

ساكن bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  صحيح remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  עבור (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      אם (! devices[current_rule]->allow)
        החזר false;

      אם (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          אם (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            החזר true;

          אם (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          אחרת אם (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          אם (remaining == 0)
            החזר true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  החזר false;
}

ساكن صحيح
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  بنية default_dev_s *default_devices = get_default_devices ();
  صحيح ret, sd_err;
  size_t i;

  אם (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      החזר 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  אם (UNLIKELY (sd_err < 0))
    החזר crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  עבור (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  עבור (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      حرف type;

      אם (! d->allow)
        {
          صحيح redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          אם (UNLIKELY (redundant < 0))
            החזר redundant;

          אם (redundant)
            استمر;

          החזר crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      אם (d->type == عدم || strcmp (d->type, "a") == 0)
        type = 'a';
      אחרת אם (strcmp (d->type, "c") == 0)
        type = 'c';
      אחרת אם (strcmp (d->type, "b") == 0)
        type = 'b';
      אחרת
        החזר crun_make_error (err, 0, "unknown device type `%s`", d->type);

      אם (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }
      אחרת
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }
    }

  החזר 0;
}

ساكن bool
has_bpf_fs ()
{
  بنية statfs stat;

  החזר statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

ساكن صحيح
bpfprog_path_from_scope (حرف **path, ثابت حرف *scope, libcrun_error_t *err)
{
  cleanup_free حرف *flat_scope = xstrdup (scope);
  حرف *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  כלעוד ((it = strchr (it, '.')) != عدم)
    *it = '_';

  החזר append_paths (path, err, CRUN_BPF_DIR, flat_scope, عدم);
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
  אם (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    החזר 0;

  in_userns = check_running_in_user_namespace (err);
  אם (UNLIKELY (in_userns < 0))
    החזר in_userns;

  אם (in_userns)
    החזר 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  אם (UNLIKELY (program == عدم))
    {
      crun_error_release (err);
      החזר 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  אם (UNLIKELY (ret < 0 && errno != EEXIST))
    החזר crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  אם (is_update)
    {
      cleanup_free بنية bpf_program *program_loaded = عدم;

      אם (resources->devices == عدم || resources->devices_len == 0)
        {
          *devices_set = true;
          החזר 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      /* it is the same as the loaded program.  */
      אם (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          החזר 0;
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
        אם (UNLIKELY (len >= (صحيح) حجم (tmp)))
          החזר crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        אם (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            החזר 0;
          }
      }

      החזר crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  אם (UNLIKELY (sd_err < 0))
    החזר crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  החזר 0;
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
  אם (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      صحيح ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
      אם (has_period)
        period = max_period;
    }

  אם (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  אם ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      אם (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          אם (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      אם (UNLIKELY (sd_err < 0))
        החזר crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  החזר 0;
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
  אם (UNLIKELY (ret < 0))
    החזר ret;

  missing_properties = read_dir_entries (dir, err);
  אם (UNLIKELY (missing_properties == عدم))
    {
      אם (crun_error_get_errno (err) != ENOENT)
        החזר -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_VALUE(name, value)                                             \
    עשה                                                                                 \
      {                                                                                \
        אם (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", value);              \
            אם (UNLIKELY (sd_err < 0))                                                 \
              החזר crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } כלעוד (0)

#  define APPEND_UINT64(name, fn)              \
    עשה                                         \
      {                                        \
        ret = fn (resources, &value, err);     \
        אם (UNLIKELY (ret < 0))                \
          החזר ret;                          \
        אם (ret)                               \
          {                                    \
            APPEND_UINT64_VALUE (name, value); \
          }                                    \
    } כלעוד (0)

  אם (resources == عدم)
    החזר 0;

  APPEND_UINT64 ("MemoryLow", get_memory_low);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  بدل (cgroup_mode)
    {
    حالة CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        אם (UNLIKELY (ret < 0))
          החזר ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        אם (UNLIKELY (ret < 0))
          החזר ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        אם (UNLIKELY (ret < 0))
          החזר ret;

        אם (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ثابت حرف *property_name = "AllowedCPUs";
            ثابت حرف *value = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free حرف *allowed_cpus = عدم;
            size_t allowed_cpus_len = 0;

            אם (value == عدم)
              value = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (value, &allowed_cpus, &allowed_cpus_len, err);
            אם (UNLIKELY (ret < 0))
              החזר ret;

            אם (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                אם (UNLIKELY (ret < 0))
                  החזר ret;
              }
          }

        אם (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ثابت حرف *property_name = "AllowedMemoryNodes";
            ثابت حرف *value = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free حرف *allowed_mems = عدم;
            size_t allowed_mems_len = 0;

            אם (value == عدم)
              value = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (value, &allowed_mems, &allowed_mems_len, err);
            אם (UNLIKELY (ret < 0))
              החזר ret;

            אם (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                אם (UNLIKELY (ret < 0))
                  החזר ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        אם (UNLIKELY (ret < 0))
          החזר ret;
      }
      اكسر;

    حالة CGROUP_MODE_LEGACY:
    حالة CGROUP_MODE_HYBRID:
      אם (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_VALUE ("CPUShares", resources->cpu->shares);
      اكسر;

    افتراضي:
      החזר crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_VALUE

  אם (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  החזר 0;
}

ساكن صحيح
reset_failed_unit (sd_bus *bus, ثابت حرف *unit)
{
  صحيح sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = عدم, *reply = عدم;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  אם (UNLIKELY (sd_err < 0))
    اذهب exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  אם (UNLIKELY (sd_err < 0))
    اذهب exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  אם (UNLIKELY (sd_err < 0))
    اذهب exit;

  sd_err = 0;

exit:
  אם (m)
    sd_bus_message_unref (m);
  אם (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  החזר sd_err;
}

ساكن صحيح
verify_ebpf_device_filter_installed (ثابت حرف *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = عدم;
  cleanup_free حرف *full_path = عدم;
  size_t n_progs = 0;
  صحيح ret;

  ret = append_paths (&full_path, err, CGROUP_ROOT, cgroup_path, عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (n_progs == 0)
    החזר crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  החזר 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
ساكن صحيح
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, صحيح ret)
{
  אם (bus)
    sd_bus_unref (bus);
  אם (m)
    sd_bus_message_unref (m);
  אם (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  החזר ret;
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
  אם (UNLIKELY (ret < 0))
    החזר ret;

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
  אם (UNLIKELY (ret < 0))
    اذهب exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  אם (UNLIKELY (ret < 0))
    اذهب exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      اذهب exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      اذهب exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      اذهب exit;
    }

  אם (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      אם (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          اذهب exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      اذهب exit;
    }

  אם (annotations)
    {
      size_t prefix_len = حجم (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      עבור (i = 0; i < annotations_len; i++)
        {
          ثابت حرف *key;
          ثابت حرف *value;
          size_t len;

          ret = string_map_get_at (annotations, i, &key, &value);
          אם (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              اذهب exit;
            }

          אם (! has_prefix (key, SYSTEMD_PROPERTY_PREFIX))
            استمر;

          len = strlen (key);
          אם (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", key);
              اذهب exit;
            }

          ret = append_systemd_annotation (m, key + prefix_len, len - prefix_len,
                                           value, err);
          אם (UNLIKELY (ret < 0))
            اذهب exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      اذهب exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      اذهب exit;
    }

  עבור (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      אם (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          اذهب exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  אם (UNLIKELY (ret < 0))
    اذهب exit;

  sd_err = sd_bus_message_close_container (m);
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      اذهب exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      اذهب exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  אם (UNLIKELY (sd_err < 0))
    {
      אם (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          אם (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = عدم;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      אם (sd_err < 0)
        {
          אם (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              אם (UNLIKELY (ret < 0))
                اذهب exit;
              אם (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          اذهب exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      اذهب exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  החזר cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
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
  אם (UNLIKELY (ret < 0))
    اذهب exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  אם (UNLIKELY (ret < 0))
    اذهب exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  אם (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      اذهب exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  אם (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      اذهب exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  אם (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      اذهب exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  אם (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      اذهب exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  החזר cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

ساكن ثابت حرف *
find_systemd_subgroup (string_map *annotations)
{
  ثابت حرف *annotation;

  annotation = find_string_map_value (annotations, "run.oci.systemd.subgroup");
  אם (annotation)
    {
      אם (annotation[0] == '\0')
        החזר عدم;
      החזר annotation;
    }

  החזר "container";
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
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  אם (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      אם (UNLIKELY (rootless < 0))
        החזר rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  עבור (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_root,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      אם (LIKELY (ret >= 0))
        اكسر;

      אם (can_retry && retries_left-- > 0)
        {
          crun_error_release (err);
          استمر;
        }

      החזר ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  אם (is_empty_string (suffix))
    scope_path = xstrdup (path);
  אחרת
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      אם (UNLIKELY (scope_path == عدم))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  אם (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  out->path = path;
  path = عدم;

  out->scope = scope;
  scope = عدم;
  החזר 0;
}

حرف *
get_cgroup_scope_path (ثابت حرف *cgroup_path, ثابت حرف *scope)
{
  حرف *path_to_scope = عدم;
  حرف *cur;

  אם (is_empty_string (cgroup_path))
    החזר عدم;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  כלעוד (cur)
    {
      حرف *next = strchr (cur + 1, '/');
      אם (next == عدم)
        اكسر;

      *next = '\0';
      אם (strcmp (cur, scope) == 0)
        החזר path_to_scope;
      *next = '/';

      cur = next;
      כלעוד (*cur == '/')
        cur++;
    }

  החזר path_to_scope;
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
  אם (UNLIKELY (mode < 0))
    החזר mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  אם (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  אם (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  אם (UNLIKELY (ret < 0))
    crun_error_release (err);
  אחרת
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  החזר destroy_cgroup_path (path_to_scope, mode, err);
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
  אם (UNLIKELY (ret < 0))
    החזר ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (cgroup_mode < 0))
    החזר cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  אם (UNLIKELY (ret < 0))
    اذهب exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      اذهب exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      اذهب exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      اذهب exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  אם (UNLIKELY (ret < 0))
    اذهب exit;

  sd_err = sd_bus_message_close_container (m);
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      اذهب exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  אם (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      اذهب exit;
    }

  אם (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      אם (UNLIKELY (ret < 0))
        اذهب exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      אם (UNLIKELY (ret < 0))
        اذهب exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  אם (UNLIKELY (ret < 0))
    اذهب exit;

  ret = 0;

exit:
  החזר cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
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

  אם (is_empty_string (scope))
    החזר crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  אם (scope_path == عدم)
    החזר crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  אם (last == عدم || strcmp (last + 1, scope) != 0)
    החזר crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  אם (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  החזר cleanup_sd_bus_and_return (bus, عدم, reply, &error, ret);
}

#אחרת
ساكن صحيح
libcrun_cgroup_enter_systemd (بنية libcrun_cgroup_args *args,
                              بنية libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (فراغ) args;
  (فراغ) out;

  החזר crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساكن صحيح
libcrun_destroy_cgroup_systemd (بنية libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (فراغ) cgroup_status;

  החזר crun_make_error (err, ENOTSUP, "systemd not supported");
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

  החזר crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساكن صحيح
libcrun_attach_process_systemd (بنية libcrun_cgroup_status *cgroup_status,
                                ثابت حرف *path, pid_t pid,
                                libcrun_error_t *err)
{
  (فراغ) cgroup_status;
  (فراغ) path;
  (فراغ) pid;

  החזר crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

بنية libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = عدم,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
