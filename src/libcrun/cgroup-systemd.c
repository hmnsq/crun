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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "cgroup.h"
#અન્તર્ભાવ "cgroup-internal.h"
#અન્તર્ભાવ "cgroup-systemd.h"
#અન્તર્ભાવ "cgroup-resources.h"
#અન્તર્ભાવ "cgroup-utils.h"
#અન્તર્ભાવ "ebpf.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "status.h"
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <sys/vfs.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/vfs.h>
#અન્તર્ભાવ <linux/magic.h>

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

સ્થિર પૂર્ણાંક
register_missing_property_from_message (અચલ અક્ષર *state_dir, અચલ અક્ષર *message, libcrun_error_t *err)
{
  cleanup_free અક્ષર *file_path = ખાલી;
  cleanup_free અક્ષર *dir_path = ખાલી;
  cleanup_free અક્ષર *property = ખાલી;
  અક્ષર *p;
  પૂર્ણાંક ret;

  જો (! has_prefix (message, "Cannot set property "))
    પાછા 0;

  property = xstrdup (message + કદ ("Cannot set property ") - 1);
  p = strchr (property, ',');
  જો (! p)
    પાછા 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_paths (&file_path, err, dir_path, property, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, ખાલી, 0, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 1;
}

સ્થિર bool
property_missing_p (અક્ષર **missing_properties, અચલ અક્ષર *property)
{
  size_t i;

  માટે (i = 0; missing_properties && missing_properties[i]; i++)
    જો (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        પાછા true;
      }

  પાછા false;
}

સ્થિર શૂન્ય
get_systemd_scope_and_slice (અચલ અક્ષર *id, bool user_slice, અચલ અક્ષર *cgroup_path,
                             અક્ષર **scope, અક્ષર **slice)
{
  અચલ અક્ષર *n;
  અક્ષર *p;

  જો (cgroup_path == ખાલી || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      પાછા;
    }

  n = strchr (cgroup_path, ':');
  જો (n == ખાલી)
    xasprintf (scope, "%s.scope", cgroup_path);
  નહીંતર
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      જો (p)
        *p = '-';
    }
  જો (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      જો (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      જો (is_empty_string (*slice))
        {
          મુક્ત (*slice);
          જો (user_slice)
            *slice = xstrdup ("user.slice");
          નહીંતર
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
સ્થિર પૂર્ણાંક
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cgroup_path = ખાલી;
  cleanup_close પૂર્ણાંક dirfd = -1;
  bool need_set_parent = true;
  અક્ષર fmt_buf[64];
  પૂર્ણાંક len;
  પૂર્ણાંક ret;

  જો (resources == ખાલી || resources->cpu == ખાલી)
    પાછા 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  જો (is_empty_string (path))
    પાછા crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  જો (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, "cpu", path, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  જો (UNLIKELY (dirfd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", cgroup_path);

  જો (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      જો (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, કદ (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (fmt_buf)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      જો (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cgroup_path = ખાલી;
  પૂર્ણાંક parent;
  પૂર્ણાંક ret;

  જો (resources == ખાલી || resources->cpu == ખાલી)
    પાછા 0;

  જો (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    પાછા 0;

  માટે (parent = 0; parent < 2; parent++)
    {
      cleanup_close પૂર્ણાંક dirfd = -1;

      જો (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, path ? path : "", (parent ? ".." : ખાલી), ખાલી);
      નહીંતર
        ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, "/cpu", path ? path : "", (parent ? ".." : ખાલી), ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      જો (UNLIKELY (dirfd < 0))
        પાછા crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      જો (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cgroup_path = ખાલી;
  પૂર્ણાંક parent;
  પૂર્ણાંક ret;
  પૂર્ણાંક levels;

  ret = append_paths (&cgroup_path, err, CGROUP_મૂળ, "/cpuset", path ? path : "", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  માટે (parent = 0; parent < levels; parent++)
    {
      cleanup_close પૂર્ણાંક dirfd_cpuset = -1;
      cleanup_free અક્ષર *path_to_cpuset = ખાલી;

      ret = append_paths (&path_to_cpuset, err, CGROUP_મૂળ "/cpuset", path, (parent ? ".." : ખાલી), ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      જો (UNLIKELY (dirfd_cpuset < 0))
        પાછા crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
સ્થિર પૂર્ણાંક
compute_finalized_path (અક્ષર **path, અચલ અક્ષર *from, અચલ અક્ષર *suffix, libcrun_error_t *err)
{
  જો (suffix == ખાલી)
    {
      *path = xstrdup (from);
      પાછા 0;
    }
  પાછા append_paths (path, err, from, suffix, ખાલી);
}

સ્થિર પૂર્ણાંક
systemd_finalize (સંરચના libcrun_cgroup_args *args, અક્ષર **path_out,
                  પૂર્ણાંક cgroup_mode, અચલ અક્ષર *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free અક્ષર *content = ખાલી;
  cleanup_free અક્ષર *path = ખાલી;
  pid_t pid = args->pid;
  પૂર્ણાંક ret;
  અક્ષર *from, *to;
  અક્ષર *saveptr = ખાલી;

  cleanup_free અક્ષર *proc_path = ખાલી;
  cleanup_close પૂર્ણાંક fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  જો (UNLIKELY (fd < 0))
    પાછા fd;

  ret = read_all_fd (fd, "cgroup path", &content, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પસંદગી (cgroup_mode)
    {
    સ્થિતિ CGROUP_MODE_HYBRID:
    સ્થિતિ CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      જો (LIKELY (from != ખાલી))
        from += 8;
      નહીંતર
        {
          from = strstr (content, ":pids:");
          જો (UNLIKELY (from == ખાલી))
            પાછા crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      જો (UNLIKELY (to == ખાલી))
        પાછા crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      જો (geteuid ())
        પાછા 0;

      ret = setup_rt_runtime (resources, path, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      માટે (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ખાલી, "\n", &saveptr))
        {
          અક્ષર *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          જો (subsystem[0] == '\0')
            {
              જો (cgroup_mode == CGROUP_MODE_LEGACY)
                ચાલુ;

              subsystem = "unified";
            }

          જો (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              જો (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  જો (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      ચાલુ;
                    }
                  પાછા ret;
                }
            }
        }

      તોડો;

    સ્થિતિ CGROUP_MODE_UNIFIED:
      {
        cleanup_free અક્ષર *dir = ખાલી;

        from = strstr (content, "0::");
        જો (UNLIKELY (from == ખાલી))
          પાછા crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        જો (UNLIKELY (to == ખાલી))
          પાછા crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        ret = append_paths (&dir, err, CGROUP_મૂળ, path, ખાલી);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        ret = move_process_to_cgroup (pid, ખાલી, path, err);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        ret = enable_controllers (path, err);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        જો (suffix)
          {
            ret = chown_cgroups (path, args->મૂળ_uid, args->મૂળ_gid, err);
            જો (UNLIKELY (ret < 0))
              પાછા ret;
          }
      }
      તોડો;

    મૂળભૂત:
      પાછા crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  *path_out = path;
  path = ખાલી;

  પાછા 0;
}

સંરચના systemd_job_removed_s
{
  અચલ અક્ષર *path;
  અચલ અક્ષર *op;
  પૂર્ણાંક terminated;
  libcrun_error_t err;
};

સ્થિર પૂર્ણાંક
systemd_job_removed (sd_bus_message *m, શૂન્ય *userdata, sd_bus_error *error arg_unused)
{
  અચલ અક્ષર *path, *unit, *result;
  uint32_t id;
  પૂર્ણાંક ret;
  સંરચના systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  જો (ret < 0)
    પાછા -1;

  જો (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      જો (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          પાછા -1;
        }
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
systemd_check_job_status_setup (sd_bus *bus, સંરચના systemd_job_removed_s *data, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = sd_bus_match_signal_async (bus, ખાલી, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, ખાલી, data);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, -ret, "sd-bus match signal");

  પાછા 0;
}

સ્થિર પૂર્ણાંક
systemd_check_job_status (sd_bus *bus, સંરચના systemd_job_removed_s *data, અચલ અક્ષર *path, અચલ અક્ષર *op,
                          libcrun_error_t *err)
{
  પૂર્ણાંક sd_err;

  data->path = path;
  data->op = op;
  જ્યારે (! data->terminated)
    {
      sd_err = sd_bus_process (bus, ખાલી);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus process");

      જો (sd_err != 0)
        ચાલુ;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus wait");
    }

  જો (data->err != ખાલી)
    {
      *err = data->err;
      પાછા -1;
    }

  પાછા 0;
}

પૂર્ણાંક
parse_sd_array (અક્ષર *s, અક્ષર **out, અક્ષર **next, libcrun_error_t *err)
{
  અક્ષર endchr;
  અક્ષર *it, *dest;
  bool escaped = false;

  *out = ખાલી;
  *next = ખાલી;

  જ્યારે (isspace (*s))
    s++;
  જો (*s == '\0')
    પાછા 0;
  નહીંતર જો (*s != '\'' && *s != '"')
    પાછા crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  જ્યારે (1)
    {
      જો (*it == '\0')
        પાછા crun_make_error (err, 0, "invalid string `%s`", s);
      જો (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          જ્યારે (isspace (*it))
            it++;
          જો (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              પાછા 0;
            }

          જો (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              પાછા 0;
            }

          પાછા crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      જો (! escaped)
        *dest++ = *it;
      it++;
    }

  પાછા 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
સ્થિર પૂર્ણાંક
append_systemd_annotation (sd_bus_message *m, અચલ અક્ષર *name, size_t name_len, અચલ અક્ષર *કિંમત,
                           libcrun_error_t *err)
{
  cleanup_free અક્ષર *tmp_name = ખાલી;
  uint32_t factor = 1;
  અચલ અક્ષર *it;
  પૂર્ણાંક sd_err;

  જ્યારે (*કિંમત == ' ')
    કિંમત++;

  it = કિંમત;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  જો (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  જો (factor != 1)
    {
      સ્થિર અચલ અક્ષર *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", ખાલી };
      અચલ અક્ષર *અંક = it;
      અક્ષર *endptr = ખાલી;
      દ્વિગુણ v;
      size_t i;

      માટે (i = 0; types[i]; i++)
        જો (has_prefix (અંક, types[i]))
          {
            અંક += strlen (types[i]);
            તોડો;
          }

      errno = 0;
      v = strtod (અંક, &endptr);
      જો (UNLIKELY (errno != 0 || endptr == અંક || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        પાછા crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, કિંમત);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      પાછા 0;
    }

  જો ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      પાછા 0;
    }
  નહીંતર જો (*it == '\'')
    {
      cleanup_free અક્ષર *v_start = ખાલી;
      અક્ષર *end;

      it = v_start = xstrdup (કિંમત);

      end = strchr (v_start + 1, '\'');
      જો (end == ખાલી)
        પાછા crun_make_error (err, 0, "invalid variant `%s`", કિંમત);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      પાછા 0;
    }
  નહીંતર જો (*it == '[')
    {
      cleanup_free અક્ષર *v_start = ખાલી;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free અક્ષર **parts = xmalloc (કદ (અક્ષર *) * parts_size);
      અક્ષર *part;

      part = v_start = xstrdup (it + 1);
      જ્યારે (1)
        {
          અક્ષર *out = ખાલી, *next = ખાલી;
          પૂર્ણાંક ret;

          ret = parse_sd_array (part, &out, &next, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          parts[n_parts++] = out;
          જો (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, કદ (અક્ષર *) * parts_size);
            }
          parts[n_parts] = ખાલી;
          જો (next == ખાલી)
            તોડો;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus close container");

      પાછા 0;
    }
  નહીંતર જો (has_prefix (it, "uint64 "))
    {
      અક્ષર *endptr = ખાલી;
      uint64_t v;

      errno = 0;
      v = strtoull (it + કદ ("uint64"), &endptr, 10);
      જો (UNLIKELY (errno != 0 || *endptr))
        પાછા crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      પાછા 0;
    }
  નહીંતર જો (has_prefix (it, "int64 "))
    {
      અક્ષર *endptr = ખાલી;
      int64_t v;

      errno = 0;
      v = strtoll (it + કદ ("int64"), &endptr, 10);
      જો (UNLIKELY (errno != 0 || *endptr))
        પાછા crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      પાછા 0;
    }
  નહીંતર જો (has_prefix (it, "uint32 "))
    {
      અક્ષર *endptr = ખાલી;
      uint32_t v;

      errno = 0;
      v = strtoul (it + કદ ("uint32"), &endptr, 10);
      જો (UNLIKELY (errno != 0 || *endptr))
        પાછા crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      પાછા 0;
    }
  નહીંતર જો (has_prefix (it, "int32 ") || strchr (it, ' ') == ખાલી)
    {
      અક્ષર *endptr = ખાલી;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      જો (has_prefix (it, "int32 "))
        v = strtol (it + કદ ("int32"), &endptr, 10);
      નહીંતર
        v = strtol (it, &endptr, 10);
      જો (UNLIKELY (errno != 0 || *endptr))
        પાછા crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      પાછા 0;
    }

  પાછા crun_make_error (err, 0, "unknown type for `%s`", name);
}

સ્થિર પૂર્ણાંક
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  પૂર્ણાંક rootless;
  પૂર્ણાંક sd_err = 0;

  rootless = is_rootless (err);
  જો (UNLIKELY (rootless < 0))
    પાછા rootless;

  જો (rootless)
    sd_err = sd_bus_default_user (bus);
  જો (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  જો (sd_err < 0)
    પાછા crun_make_error (err, -sd_err, "cannot open sd-bus");

  પાછા 0;
}

સ્થિર અચલ અક્ષર *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, અચલ અક્ષર *name)
{
  size_t i;

  જો (resources == ખાલી || resources->unified == ખાલી)
    પાછા ખાલી;

  માટે (i = 0; i < resources->unified->len; i++)
    જો (strcmp (resources->unified->ચાવીઓ[i], name) == 0)
      પાછા resources->unified->કિંમતો[i];
  પાછા ખાલી;
}

સ્થિર પૂર્ણાંક
get_કિંમત_from_unified_map (runtime_spec_schema_config_linux_resources *resources, અચલ અક્ષર *name,
                            uint64_t *કિંમત, libcrun_error_t *err)
{
  અક્ષર *endptr = ખાલી;
  size_t i;

  જો (resources == ખાલી || resources->unified == ખાલી)
    પાછા 0;

  માટે (i = 0; i < resources->unified->len; i++)
    જો (strcmp (resources->unified->ચાવીઓ[i], name) == 0)
      {
        જો (strcmp (resources->unified->કિંમતો[i], "max") == 0)
          {
            *કિંમત = UINT64_MAX;
            પાછા 1;
          }

        errno = 0;
        *કિંમત = strtoull (resources->unified->કિંમતો[i], &endptr, 10);
        જો (UNLIKELY (errno))
          પાછા crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->કિંમતો[i]);
        જો (endptr && *endptr)
          પાછા crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->કિંમતો[i]);
        પાછા 1;
      }
  પાછા 0;
}

સ્થિર અંતઃરેખ પૂર્ણાંક
get_memory_નીચલું (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  જો (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      પાછા 1;
    }

  પાછા get_કિંમત_from_unified_map (resources, "memory.low", limit, err);
}

સ્થિર અંતઃરેખ પૂર્ણાંક
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  જો (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      પાછા 1;
    }

  પાછા get_કિંમત_from_unified_map (resources, "pids.max", limit, err);
}

સ્થિર અંતઃરેખ પૂર્ણાંક
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  જો (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      પાછા 1;
    }

  પાછા get_કિંમત_from_unified_map (resources, "memory.max", limit, err);
}

સ્થિર અંતઃરેખ પૂર્ણાંક
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  જો (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      જો (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          પાછા 1;
        }
      /* 0 means unset.  */
      જો (memory->swap == 0)
        પાછા 0;
      જો (mem <= 0)
        પાછા crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      જો (memory->swap < mem)
        પાછા crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      પાછા 1;
    }

  પાછા get_કિંમત_from_unified_map (resources, "memory.swap.max", limit, err);
}

સ્થિર અંતઃરેખ પૂર્ણાંક
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t કિંમત;
  પૂર્ણાંક ret;

  ret = get_કિંમત_from_unified_map (resources, "cpu.idle", &કિંમત, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  જો (ret > 0 && કિંમત == 1)
    has_idle = true;
  જો (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_કિંમત_from_unified_map (resources, "cpu.weight", weight, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  જો (ret > 0)
    {
      જો (has_idle)
        પાછા crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      પાછા 1;
    }

  જો (resources->cpu && resources->cpu->shares_present)
    {
      જો (has_idle)
        પાછા crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      જો (resources->cpu->shares == 0)
        પાછા 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      પાછા 1;
    }
  જો (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      પાછા 1;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
read_uint64_કિંમત (અચલ અક્ષર *str, uint64_t *કિંમત, libcrun_error_t *err)
{
  અક્ષર *endptr = ખાલી;

  errno = 0;
  *કિંમત = strtoull (str, &endptr, 10);
  જો (UNLIKELY (errno))
    પાછા crun_make_error (err, errno, "invalid value: %s", str);
  જો (endptr && *endptr)
    પાછા crun_make_error (err, 0, "invalid value: %s", str);
  પાછા 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
સ્થિર અંતઃરેખ uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  પાછા bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

સ્થિર પૂર્ણાંક
append_io_weight (sd_bus_message *m, અક્ષર **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  પૂર્ણાંક sd_err;
  size_t i;
  પૂર્ણાંક ret;

#  define APPEND_IO_WEIGHT(કિંમત)                                                      \
    કરો                                                                                 \
      {                                                                                \
        જો (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            જો (UNLIKELY (sd_err < 0))                                                 \
              પાછા crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } જ્યારે (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, કિંમત)                                                      \
    કરો                                                                                                \
      {                                                                                               \
        જો (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            જો (UNLIKELY (sd_err < 0))                                                                \
              પાછા crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } જ્યારે (0)

  જો (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  માટે (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      અક્ષર name[64];
      પૂર્ણાંક len;

      len = snprintf (name, કદ (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (name)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  માટે (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free અક્ષર *કિંમત = ખાલી;
      bool need_conversion = false;
      અક્ષર *saveptr = ખાલી;
      અક્ષર *લીટી;

      જો (strcmp (resources->unified->ચાવીઓ[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      નહીંતર જો (strcmp (resources->unified->ચાવીઓ[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      નહીંતર
        {
          /* Skip other keys.  */
          ચાલુ;
        }

      કિંમત = xstrdup (resources->unified->કિંમતો[i]);

      માટે (લીટી = strtok_r (કિંમત, "\n", &saveptr); લીટી; લીટી = strtok_r (ખાલી, "\n", &saveptr))
        {
          અક્ષર *sep;

          sep = strchr (લીટી, ' ');
          જ્યારે (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_કિંમત (sep ? sep : લીટી, &weight, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          જો (need_conversion)
            weight = IO_WEIGHT (weight);

          જો (sep == ખાલી || strcmp (લીટી, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          નહીંતર
            APPEND_IO_DEVICE_WEIGHT (લીટી, weight);
        }
    }

  પાછા 0;
}

/* Adapted from systemd.  */
સ્થિર પૂર્ણાંક
bus_append_byte_array (sd_bus_message *m, અચલ અક્ષર *field, અચલ શૂન્ય *buf, size_t n, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  જો (ret < 0)
    પાછા crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  જો (ret < 0)
    પાછા crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  જો (ret < 0)
    પાછા crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  જો (ret < 0)
    પાછા crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  જો (ret < 0)
    પાછા crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  જો (ret < 0)
    પાછા crun_make_error (err, -ret, "sd_bus_message_close_container");

  પાછા 1;
}

સ્થિર પૂર્ણાંક
append_uint64_from_unified_map (sd_bus_message *m,
                                અક્ષર **missing_properties,
                                અચલ અક્ષર *attr,
                                અચલ અક્ષર *ચાવી,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t કિંમત;
  પૂર્ણાંક ret;

  જો (property_missing_p (missing_properties, attr))
    પાછા 0;

  ret = get_કિંમત_from_unified_map (resources, ચાવી, &કિંમત, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  જો (ret)
    {
      પૂર્ણાંક sd_err = sd_bus_message_append (m, "(sv)", attr, "t", કિંમત);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  પાછા 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
સ્થિર પૂર્ણાંક
append_device_allow (sd_bus_message *m,
                     અચલ અક્ષર type,
                     પૂર્ણાંક major,
                     પૂર્ણાંક minor,
                     અચલ અક્ષર *access,
                     libcrun_error_t *err)
{
  અક્ષર device[64];
  પૂર્ણાંક sd_err;
  પૂર્ણાંક len;

  જો (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      પાછા 0;
    }

  જો (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, કદ (device), "%s-*", type == 'c' ? "char" : "block");
  નહીંતર જો (IS_WILDCARD (minor))
    len = snprintf (device, કદ (device), type == 'c' ? "char-%d" : "block-%d", major);
  નહીંતર
    len = snprintf (device, કદ (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (device)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  જો (UNLIKELY (sd_err < 0))
    પાછા crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  પાછા 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

સ્થિર પૂર્ણાંક
access_to_mode (અચલ અક્ષર *access, libcrun_error_t *err)
{
  પૂર્ણાંક mode = 0;

  જ્યારે (access && *access)
    {
      પૂર્ણાંક c = *access++;

      પસંદગી (c)
        {
        સ્થિતિ 'r':
          mode |= MODE_R;
          તોડો;

        સ્થિતિ 'w':
          mode |= MODE_W;
          તોડો;

        સ્થિતિ 'm':
          mode |= MODE_M;
          તોડો;

        મૂળભૂત:
          પાછા crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  પાછા mode;
}

/* check if the type b is included in type a.  */
સ્થિર bool
has_same_type (અચલ અક્ષર *a, અચલ અક્ષર *b)
{
  bool a_all = a == ખાલી || strcmp (a, "a") == 0;
  જો (b == ખાલી || strcmp (b, "a") == 0)
    પાછા a_all;

  પાછા a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
સ્થિર પૂર્ણાંક
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  પૂર્ણાંક req_mode;

#  define CMP_DEV_અંક(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  જો (UNLIKELY (req_mode < 0))
    પાછા req_mode;

  માટે (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      પૂર્ણાંક mode;

      જો (! CMP_DEV_અંક (devices[n]->minor, devices[current_rule]->minor))
        ચાલુ;
      જો (! CMP_DEV_અંક (devices[n]->major, devices[current_rule]->major))
        ચાલુ;

      જો (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        ચાલુ;

      mode = access_to_mode (devices[i]->access, err);
      જો (UNLIKELY (mode < 0))
        પાછા mode;

      જો (devices[current_rule]->allow && (mode & req_mode))
        પાછા 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      જો (req_mode == 0)
        પાછા 1;
    }

  પાછા 0;
#  undef CMP_DEV_અંક
}

સ્થિર size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  જો (n == 0)
    પાછા 1;

  /* Find the first rule that is after the last "block all".  */
  માટે (i = n - 1; i-- > 0;)
    {
      જો ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        પાછા i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  માટે (i = 0; i < n; i++)
    જો (devices[i]->allow)
      પાછા i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  પાછા n + 1;
}

સ્થિર bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  પૂર્ણાંક remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  માટે (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      જો (! devices[current_rule]->allow)
        પાછા false;

      જો (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          જો (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            પાછા true;

          જો (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          નહીંતર જો (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          જો (remaining == 0)
            પાછા true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  પાછા false;
}

સ્થિર પૂર્ણાંક
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  સંરચના default_dev_s *default_devices = get_default_devices ();
  પૂર્ણાંક ret, sd_err;
  size_t i;

  જો (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      પાછા 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  જો (UNLIKELY (sd_err < 0))
    પાછા crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  માટે (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  માટે (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      અક્ષર type;

      જો (! d->allow)
        {
          પૂર્ણાંક redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          જો (UNLIKELY (redundant < 0))
            પાછા redundant;

          જો (redundant)
            ચાલુ;

          પાછા crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      જો (d->type == ખાલી || strcmp (d->type, "a") == 0)
        type = 'a';
      નહીંતર જો (strcmp (d->type, "c") == 0)
        type = 'c';
      નહીંતર જો (strcmp (d->type, "b") == 0)
        type = 'b';
      નહીંતર
        પાછા crun_make_error (err, 0, "unknown device type `%s`", d->type);

      જો (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
      નહીંતર
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }

  પાછા 0;
}

સ્થિર bool
has_bpf_fs ()
{
  સંરચના statfs stat;

  પાછા statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

સ્થિર પૂર્ણાંક
bpfprog_path_from_scope (અક્ષર **path, અચલ અક્ષર *scope, libcrun_error_t *err)
{
  cleanup_free અક્ષર *flat_scope = xstrdup (scope);
  અક્ષર *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  જ્યારે ((it = strchr (it, '.')) != ખાલી)
    *it = '_';

  પાછા append_paths (path, err, CRUN_BPF_DIR, flat_scope, ખાલી);
}

સ્થિર પૂર્ણાંક
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 અક્ષર **missing_properties,
                 અચલ અક્ષર *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  પૂર્ણાંક ret = 0;
  પૂર્ણાંક in_userns, sd_err;
  અચલ અક્ષર *property_name = "BPFProgram";
  cleanup_free સંરચના bpf_program *program = ખાલી;
  cleanup_free અક્ષર *path = ખાલી;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  જો (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    પાછા 0;

  in_userns = check_running_in_user_namespace (err);
  જો (UNLIKELY (in_userns < 0))
    પાછા in_userns;

  જો (in_userns)
    પાછા 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  જો (UNLIKELY (program == ખાલી))
    {
      crun_error_release (err);
      પાછા 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  જો (UNLIKELY (ret < 0 && errno != EEXIST))
    પાછા crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  જો (is_update)
    {
      cleanup_free સંરચના bpf_program *program_loaded = ખાલી;

      જો (resources->devices == ખાલી || resources->devices_len == 0)
        {
          *devices_set = true;
          પાછા 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      /* it is the same as the loaded program.  */
      જો (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          પાછા 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free સંરચના bpf_program *program_new = ખાલી;
        અક્ષર tmp[64];
        પૂર્ણાંક len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, કદ (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (tmp)))
          પાછા crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        જો (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            પાછા 0;
          }
      }

      પાછા crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  જો (UNLIKELY (sd_err < 0))
    પાછા crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  પાછા 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
સ્થિર પૂર્ણાંક
append_cpu_quota (sd_bus_message *m, અક્ષર **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  અચલ અક્ષર *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  પૂર્ણાંક sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  જો (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      પૂર્ણાંક ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      જો (has_period)
        period = max_period;
    }

  જો (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  જો ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      જો (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          જો (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      જો (UNLIKELY (sd_err < 0))
        પાછા crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
append_resources (sd_bus_message *m,
                  bool is_update,
                  અચલ અક્ષર *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  પૂર્ણાંક cgroup_mode,
                  અચલ અક્ષર *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t કિંમત;
  પૂર્ણાંક sd_err;
  પૂર્ણાંક ret;
  cleanup_free અક્ષર *dir = ખાલી;
  cleanup_free અક્ષર **missing_properties = ખાલી;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  missing_properties = read_dir_entries (dir, err);
  જો (UNLIKELY (missing_properties == ખાલી))
    {
      જો (crun_error_get_errno (err) != ENOENT)
        પાછા -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_કિંમત(name, કિંમત)                                             \
    કરો                                                                                 \
      {                                                                                \
        જો (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", કિંમત);              \
            જો (UNLIKELY (sd_err < 0))                                                 \
              પાછા crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } જ્યારે (0)

#  define APPEND_UINT64(name, fn)              \
    કરો                                         \
      {                                        \
        ret = fn (resources, &કિંમત, err);     \
        જો (UNLIKELY (ret < 0))                \
          પાછા ret;                          \
        જો (ret)                               \
          {                                    \
            APPEND_UINT64_કિંમત (name, કિંમત); \
          }                                    \
    } જ્યારે (0)

  જો (resources == ખાલી)
    પાછા 0;

  APPEND_UINT64 ("MemoryLow", get_memory_નીચલું);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પસંદગી (cgroup_mode)
    {
    સ્થિતિ CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        જો (UNLIKELY (ret < 0))
          પાછા ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        જો (UNLIKELY (ret < 0))
          પાછા ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        જો (UNLIKELY (ret < 0))
          પાછા ret;

        જો (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            અચલ અક્ષર *property_name = "AllowedCPUs";
            અચલ અક્ષર *કિંમત = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free અક્ષર *allowed_cpus = ખાલી;
            size_t allowed_cpus_len = 0;

            જો (કિંમત == ખાલી)
              કિંમત = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (કિંમત, &allowed_cpus, &allowed_cpus_len, err);
            જો (UNLIKELY (ret < 0))
              પાછા ret;

            જો (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                જો (UNLIKELY (ret < 0))
                  પાછા ret;
              }
          }

        જો (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            અચલ અક્ષર *property_name = "AllowedMemoryNodes";
            અચલ અક્ષર *કિંમત = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free અક્ષર *allowed_mems = ખાલી;
            size_t allowed_mems_len = 0;

            જો (કિંમત == ખાલી)
              કિંમત = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (કિંમત, &allowed_mems, &allowed_mems_len, err);
            જો (UNLIKELY (ret < 0))
              પાછા ret;

            જો (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                જો (UNLIKELY (ret < 0))
                  પાછા ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        જો (UNLIKELY (ret < 0))
          પાછા ret;
      }
      તોડો;

    સ્થિતિ CGROUP_MODE_LEGACY:
    સ્થિતિ CGROUP_MODE_HYBRID:
      જો (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_કિંમત ("CPUShares", resources->cpu->shares);
      તોડો;

    મૂળભૂત:
      પાછા crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_કિંમત

  જો (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
reset_failed_unit (sd_bus *bus, અચલ અક્ષર *unit)
{
  પૂર્ણાંક sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = ખાલી, *reply = ખાલી;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  જો (UNLIKELY (sd_err < 0))
    જાઓ exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  જો (UNLIKELY (sd_err < 0))
    જાઓ exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  જો (UNLIKELY (sd_err < 0))
    જાઓ exit;

  sd_err = 0;

exit:
  જો (m)
    sd_bus_message_unref (m);
  જો (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  પાછા sd_err;
}

સ્થિર પૂર્ણાંક
verify_ebpf_device_filter_installed (અચલ અક્ષર *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = ખાલી;
  cleanup_free અક્ષર *full_path = ખાલી;
  size_t n_progs = 0;
  પૂર્ણાંક ret;

  ret = append_paths (&full_path, err, CGROUP_મૂળ, cgroup_path, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (n_progs == 0)
    પાછા crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  પાછા 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
સ્થિર પૂર્ણાંક
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, પૂર્ણાંક ret)
{
  જો (bus)
    sd_bus_unref (bus);
  જો (m)
    sd_bus_message_unref (m);
  જો (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  પાછા ret;
}

સ્થિર પૂર્ણાંક
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            પૂર્ણાંક cgroup_mode,
                            string_map *annotations,
                            અચલ અક્ષર *state_મૂળ,
                            અચલ અક્ષર *scope, અચલ અક્ષર *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = ખાલી;
  sd_bus_message *m = ખાલી;
  sd_bus_message *reply = ખાલી;
  પૂર્ણાંક sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  અચલ અક્ષર *object = ખાલી;
  સંરચના systemd_job_removed_s job_data = {};
  પૂર્ણાંક i;
  અચલ અક્ષર *boolean_opts[10];
  cleanup_free અક્ષર *state_dir = ખાલી;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_મૂળ, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = ખાલી;

  ret = open_sd_bus_connection (&bus, err);
  જો (UNLIKELY (ret < 0))
    જાઓ exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  જો (UNLIKELY (ret < 0))
    જાઓ exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      જાઓ exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      જાઓ exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      જાઓ exit;
    }

  જો (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      જો (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          જાઓ exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      જાઓ exit;
    }

  જો (annotations)
    {
      size_t prefix_len = કદ (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      માટે (i = 0; i < annotations_len; i++)
        {
          અચલ અક્ષર *ચાવી;
          અચલ અક્ષર *કિંમત;
          size_t len;

          ret = string_map_get_at (annotations, i, &ચાવી, &કિંમત);
          જો (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              જાઓ exit;
            }

          જો (! has_prefix (ચાવી, SYSTEMD_PROPERTY_PREFIX))
            ચાલુ;

          len = strlen (ચાવી);
          જો (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", ચાવી);
              જાઓ exit;
            }

          ret = append_systemd_annotation (m, ચાવી + prefix_len, len - prefix_len,
                                           કિંમત, err);
          જો (UNLIKELY (ret < 0))
            જાઓ exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      જાઓ exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      જાઓ exit;
    }

  માટે (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      જો (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          જાઓ exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  જો (UNLIKELY (ret < 0))
    જાઓ exit;

  sd_err = sd_bus_message_close_container (m);
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      જાઓ exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      જાઓ exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  જો (UNLIKELY (sd_err < 0))
    {
      જો (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          જો (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = ખાલી;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      જો (sd_err < 0)
        {
          જો (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              જો (UNLIKELY (ret < 0))
                જાઓ exit;
              જો (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          જાઓ exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      જાઓ exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  પાછા cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

સ્થિર પૂર્ણાંક
libcrun_destroy_systemd_cgroup_scope (સંરચના libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = ખાલી;
  sd_bus_message *m = ખાલી;
  sd_bus_message *reply = ખાલી;
  પૂર્ણાંક ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  અચલ અક્ષર *object;
  અચલ અક્ષર *scope = cgroup_status->scope;
  સંરચના systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  જો (UNLIKELY (ret < 0))
    જાઓ exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  જો (UNLIKELY (ret < 0))
    જાઓ exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  જો (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      જાઓ exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  જો (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      જાઓ exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  જો (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      જાઓ exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  જો (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      જાઓ exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  પાછા cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

સ્થિર અચલ અક્ષર *
find_systemd_subgroup (string_map *annotations)
{
  અચલ અક્ષર *annotation;

  annotation = find_string_map_કિંમત (annotations, "run.oci.systemd.subgroup");
  જો (annotation)
    {
      જો (annotation[0] == '\0')
        પાછા ખાલી;
      પાછા annotation;
    }

  પાછા "container";
}

સ્થિર પૂર્ણાંક
libcrun_cgroup_enter_systemd (સંરચના libcrun_cgroup_args *args,
                              સંરચના libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  અચલ અક્ષર *cgroup_path = args->cgroup_path;
  cleanup_free અક્ષર *scope_path = ખાલી;
  cleanup_free અક્ષર *scope = ખાલી;
  cleanup_free અક્ષર *path = ખાલી;
  cleanup_free અક્ષર *slice = ખાલી;
  પૂર્ણાંક retries_ડાબું = 32;
  અચલ અક્ષર *suffix;
  અચલ અક્ષર *id = args->id;
  pid_t pid = args->pid;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક rootless = 0;
  પૂર્ણાંક ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  જો (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      જો (UNLIKELY (rootless < 0))
        પાછા rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  માટે (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_મૂળ,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      જો (LIKELY (ret >= 0))
        તોડો;

      જો (can_retry && retries_ડાબું-- > 0)
        {
          crun_error_release (err);
          ચાલુ;
        }

      પાછા ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  જો (is_empty_string (suffix))
    scope_path = xstrdup (path);
  નહીંતર
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      જો (UNLIKELY (scope_path == ખાલી))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  જો (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  out->path = path;
  path = ખાલી;

  out->scope = scope;
  scope = ખાલી;
  પાછા 0;
}

અક્ષર *
get_cgroup_scope_path (અચલ અક્ષર *cgroup_path, અચલ અક્ષર *scope)
{
  અક્ષર *path_to_scope = ખાલી;
  અક્ષર *cur;

  જો (is_empty_string (cgroup_path))
    પાછા ખાલી;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  જ્યારે (cur)
    {
      અક્ષર *next = strchr (cur + 1, '/');
      જો (next == ખાલી)
        તોડો;

      *next = '\0';
      જો (strcmp (cur, scope) == 0)
        પાછા path_to_scope;
      *next = '/';

      cur = next;
      જ્યારે (*cur == '/')
        cur++;
    }

  પાછા path_to_scope;
}

સ્થિર પૂર્ણાંક
libcrun_destroy_cgroup_systemd (સંરચના libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free અક્ષર *path_to_scope = ખાલી;
  cleanup_free અક્ષર *bpfprog = ખાલી;
  પૂર્ણાંક mode;
  પૂર્ણાંક ret;

  mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (mode < 0))
    પાછા mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  જો (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  જો (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  જો (UNLIKELY (ret < 0))
    crun_error_release (err);
  નહીંતર
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  પાછા destroy_cgroup_path (path_to_scope, mode, err);
}

સ્થિર પૂર્ણાંક
libcrun_update_resources_systemd (સંરચના libcrun_cgroup_status *cgroup_status,
                                  અચલ અક્ષર *state_મૂળ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  સંરચના systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free અક્ષર *state_dir = ખાલી;
  sd_bus_message *reply = ખાલી;
  sd_bus_message *m = ખાલી;
  sd_bus *bus = ખાલી;
  પૂર્ણાંક sd_err, ret;
  પૂર્ણાંક cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_મૂળ, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  જો (UNLIKELY (ret < 0))
    જાઓ exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      જાઓ exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      જાઓ exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      જાઓ exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  જો (UNLIKELY (ret < 0))
    જાઓ exit;

  sd_err = sd_bus_message_close_container (m);
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      જાઓ exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  જો (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      જાઓ exit;
    }

  જો (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      જો (UNLIKELY (ret < 0))
        જાઓ exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      જો (UNLIKELY (ret < 0))
        જાઓ exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  જો (UNLIKELY (ret < 0))
    જાઓ exit;

  ret = 0;

exit:
  પાછા cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
સ્થિર પૂર્ણાંક
libcrun_attach_process_systemd (સંરચના libcrun_cgroup_status *cgroup_status,
                                અચલ અક્ષર *path, pid_t pid,
                                libcrun_error_t *err)
{
  અચલ અક્ષર *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free અક્ષર *scope_path = ખાલી;
  sd_bus_message *reply = ખાલી;
  અચલ અક્ષર *subcgroup;
  અચલ અક્ષર *last;
  sd_bus *bus = ખાલી;
  size_t len;
  પૂર્ણાંક sd_err, ret;

  જો (is_empty_string (scope))
    પાછા crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  જો (scope_path == ખાલી)
    પાછા crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  જો (last == ખાલી || strcmp (last + 1, scope) != 0)
    પાછા crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  જો (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  પાછા cleanup_sd_bus_and_return (bus, ખાલી, reply, &error, ret);
}

#નહીંતર
સ્થિર પૂર્ણાંક
libcrun_cgroup_enter_systemd (સંરચના libcrun_cgroup_args *args,
                              સંરચના libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (શૂન્ય) args;
  (શૂન્ય) out;

  પાછા crun_make_error (err, ENOTSUP, "systemd not supported");
}

સ્થિર પૂર્ણાંક
libcrun_destroy_cgroup_systemd (સંરચના libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (શૂન્ય) cgroup_status;

  પાછા crun_make_error (err, ENOTSUP, "systemd not supported");
}

સ્થિર પૂર્ણાંક
libcrun_update_resources_systemd (સંરચના libcrun_cgroup_status *cgroup_status,
                                  અચલ અક્ષર *state_મૂળ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (શૂન્ય) cgroup_status;
  (શૂન્ય) state_મૂળ;
  (શૂન્ય) resources;

  પાછા crun_make_error (err, ENOTSUP, "systemd not supported");
}

સ્થિર પૂર્ણાંક
libcrun_attach_process_systemd (સંરચના libcrun_cgroup_status *cgroup_status,
                                અચલ અક્ષર *path, pid_t pid,
                                libcrun_error_t *err)
{
  (શૂન્ય) cgroup_status;
  (શૂન્ય) path;
  (શૂન્ય) pid;

  પાછા crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

સંરચના libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = ખાલી,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
