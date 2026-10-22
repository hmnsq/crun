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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "cgroup.h"
#அந்தர்பாவ "cgroup-internal.h"
#அந்தர்பாவ "cgroup-systemd.h"
#அந்தர்பாவ "cgroup-resources.h"
#அந்தர்பாவ "cgroup-utils.h"
#அந்தர்பாவ "ebpf.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "status.h"
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <sys/vfs.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/vfs.h>
#அந்தர்பாவ <linux/magic.h>

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

நிலையான முழு
register_missing_property_from_message (மாறிலி எழுத்து *state_dir, மாறிலி எழுத்து *message, libcrun_error_t *err)
{
  cleanup_free எழுத்து *file_path = வெற்று;
  cleanup_free எழுத்து *dir_path = வெற்று;
  cleanup_free எழுத்து *property = வெற்று;
  எழுத்து *p;
  முழு ret;

  எனில் (! has_prefix (message, "Cannot set property "))
    திரும்பு 0;

  property = xstrdup (message + அளவு ("Cannot set property ") - 1);
  p = strchr (property, ',');
  எனில் (! p)
    திரும்பு 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_paths (&file_path, err, dir_path, property, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, வெற்று, 0, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 1;
}

நிலையான bool
property_missing_p (எழுத்து **missing_properties, மாறிலி எழுத்து *property)
{
  size_t i;

  க்கு (i = 0; missing_properties && missing_properties[i]; i++)
    எனில் (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        திரும்பு true;
      }

  திரும்பு false;
}

நிலையான வெறுமை
get_systemd_scope_and_slice (மாறிலி எழுத்து *id, bool user_slice, மாறிலி எழுத்து *cgroup_path,
                             எழுத்து **scope, எழுத்து **slice)
{
  மாறிலி எழுத்து *n;
  எழுத்து *p;

  எனில் (cgroup_path == வெற்று || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      திரும்பு;
    }

  n = strchr (cgroup_path, ':');
  எனில் (n == வெற்று)
    xasprintf (scope, "%s.scope", cgroup_path);
  இல்லையெனில்
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      எனில் (p)
        *p = '-';
    }
  எனில் (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      எனில் (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      எனில் (is_empty_string (*slice))
        {
          விடுவி (*slice);
          எனில் (user_slice)
            *slice = xstrdup ("user.slice");
          இல்லையெனில்
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
நிலையான முழு
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cgroup_path = வெற்று;
  cleanup_close முழு dirfd = -1;
  bool need_set_parent = true;
  எழுத்து fmt_buf[64];
  முழு len;
  முழு ret;

  எனில் (resources == வெற்று || resources->cpu == வெற்று)
    திரும்பு 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  எனில் (is_empty_string (path))
    திரும்பு crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  எனில் (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_வேர், "cpu", path, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (dirfd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", cgroup_path);

  எனில் (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      எனில் (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, அளவு (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      எனில் (UNLIKELY (len >= (முழு) அளவு (fmt_buf)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      எனில் (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு 0;
}

நிலையான முழு
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cgroup_path = வெற்று;
  முழு parent;
  முழு ret;

  எனில் (resources == வெற்று || resources->cpu == வெற்று)
    திரும்பு 0;

  எனில் (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    திரும்பு 0;

  க்கு (parent = 0; parent < 2; parent++)
    {
      cleanup_close முழு dirfd = -1;

      எனில் (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_வேர், path ? path : "", (parent ? ".." : வெற்று), வெற்று);
      இல்லையெனில்
        ret = append_paths (&cgroup_path, err, CGROUP_வேர், "/cpu", path ? path : "", (parent ? ".." : வெற்று), வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      எனில் (UNLIKELY (dirfd < 0))
        திரும்பு crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      எனில் (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }

  திரும்பு 0;
}

நிலையான முழு
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cgroup_path = வெற்று;
  முழு parent;
  முழு ret;
  முழு levels;

  ret = append_paths (&cgroup_path, err, CGROUP_வேர், "/cpuset", path ? path : "", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  க்கு (parent = 0; parent < levels; parent++)
    {
      cleanup_close முழு dirfd_cpuset = -1;
      cleanup_free எழுத்து *path_to_cpuset = வெற்று;

      ret = append_paths (&path_to_cpuset, err, CGROUP_வேர் "/cpuset", path, (parent ? ".." : வெற்று), வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      எனில் (UNLIKELY (dirfd_cpuset < 0))
        திரும்பு crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
நிலையான முழு
compute_finalized_path (எழுத்து **path, மாறிலி எழுத்து *from, மாறிலி எழுத்து *suffix, libcrun_error_t *err)
{
  எனில் (suffix == வெற்று)
    {
      *path = xstrdup (from);
      திரும்பு 0;
    }
  திரும்பு append_paths (path, err, from, suffix, வெற்று);
}

நிலையான முழு
systemd_finalize (கட்டமைப்பு libcrun_cgroup_args *args, எழுத்து **path_out,
                  முழு cgroup_mode, மாறிலி எழுத்து *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free எழுத்து *content = வெற்று;
  cleanup_free எழுத்து *path = வெற்று;
  pid_t pid = args->pid;
  முழு ret;
  எழுத்து *from, *to;
  எழுத்து *saveptr = வெற்று;

  cleanup_free எழுத்து *proc_path = வெற்று;
  cleanup_close முழு fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு fd;

  ret = read_all_fd (fd, "cgroup path", &content, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  தேர்வு (cgroup_mode)
    {
    நிலை CGROUP_MODE_HYBRID:
    நிலை CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      எனில் (LIKELY (from != வெற்று))
        from += 8;
      இல்லையெனில்
        {
          from = strstr (content, ":pids:");
          எனில் (UNLIKELY (from == வெற்று))
            திரும்பு crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      எனில் (UNLIKELY (to == வெற்று))
        திரும்பு crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      எனில் (geteuid ())
        திரும்பு 0;

      ret = setup_rt_runtime (resources, path, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      க்கு (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (வெற்று, "\n", &saveptr))
        {
          எழுத்து *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          எனில் (subsystem[0] == '\0')
            {
              எனில் (cgroup_mode == CGROUP_MODE_LEGACY)
                தொடர்;

              subsystem = "unified";
            }

          எனில் (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              எனில் (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  எனில் (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      தொடர்;
                    }
                  திரும்பு ret;
                }
            }
        }

      நிறுத்து;

    நிலை CGROUP_MODE_UNIFIED:
      {
        cleanup_free எழுத்து *dir = வெற்று;

        from = strstr (content, "0::");
        எனில் (UNLIKELY (from == வெற்று))
          திரும்பு crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        எனில் (UNLIKELY (to == வெற்று))
          திரும்பு crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        ret = append_paths (&dir, err, CGROUP_வேர், path, வெற்று);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        ret = move_process_to_cgroup (pid, வெற்று, path, err);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        ret = enable_controllers (path, err);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        எனில் (suffix)
          {
            ret = chown_cgroups (path, args->வேர்_uid, args->வேர்_gid, err);
            எனில் (UNLIKELY (ret < 0))
              திரும்பு ret;
          }
      }
      நிறுத்து;

    இயல்பு:
      திரும்பு crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  *path_out = path;
  path = வெற்று;

  திரும்பு 0;
}

கட்டமைப்பு systemd_job_removed_s
{
  மாறிலி எழுத்து *path;
  மாறிலி எழுத்து *op;
  முழு terminated;
  libcrun_error_t err;
};

நிலையான முழு
systemd_job_removed (sd_bus_message *m, வெறுமை *userdata, sd_bus_error *error arg_unused)
{
  மாறிலி எழுத்து *path, *unit, *result;
  uint32_t id;
  முழு ret;
  கட்டமைப்பு systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  எனில் (ret < 0)
    திரும்பு -1;

  எனில் (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      எனில் (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          திரும்பு -1;
        }
    }
  திரும்பு 0;
}

நிலையான முழு
systemd_check_job_status_setup (sd_bus *bus, கட்டமைப்பு systemd_job_removed_s *data, libcrun_error_t *err)
{
  முழு ret;

  ret = sd_bus_match_signal_async (bus, வெற்று, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, வெற்று, data);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, -ret, "sd-bus match signal");

  திரும்பு 0;
}

நிலையான முழு
systemd_check_job_status (sd_bus *bus, கட்டமைப்பு systemd_job_removed_s *data, மாறிலி எழுத்து *path, மாறிலி எழுத்து *op,
                          libcrun_error_t *err)
{
  முழு sd_err;

  data->path = path;
  data->op = op;
  போது (! data->terminated)
    {
      sd_err = sd_bus_process (bus, வெற்று);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus process");

      எனில் (sd_err != 0)
        தொடர்;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus wait");
    }

  எனில் (data->err != வெற்று)
    {
      *err = data->err;
      திரும்பு -1;
    }

  திரும்பு 0;
}

முழு
parse_sd_array (எழுத்து *s, எழுத்து **out, எழுத்து **next, libcrun_error_t *err)
{
  எழுத்து endchr;
  எழுத்து *it, *dest;
  bool escaped = false;

  *out = வெற்று;
  *next = வெற்று;

  போது (isspace (*s))
    s++;
  எனில் (*s == '\0')
    திரும்பு 0;
  இல்லையெனில் எனில் (*s != '\'' && *s != '"')
    திரும்பு crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  போது (1)
    {
      எனில் (*it == '\0')
        திரும்பு crun_make_error (err, 0, "invalid string `%s`", s);
      எனில் (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          போது (isspace (*it))
            it++;
          எனில் (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              திரும்பு 0;
            }

          எனில் (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              திரும்பு 0;
            }

          திரும்பு crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      எனில் (! escaped)
        *dest++ = *it;
      it++;
    }

  திரும்பு 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
நிலையான முழு
append_systemd_annotation (sd_bus_message *m, மாறிலி எழுத்து *name, size_t name_len, மாறிலி எழுத்து *மதிப்பு,
                           libcrun_error_t *err)
{
  cleanup_free எழுத்து *tmp_name = வெற்று;
  uint32_t factor = 1;
  மாறிலி எழுத்து *it;
  முழு sd_err;

  போது (*மதிப்பு == ' ')
    மதிப்பு++;

  it = மதிப்பு;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  எனில் (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  எனில் (factor != 1)
    {
      நிலையான மாறிலி எழுத்து *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", வெற்று };
      மாறிலி எழுத்து *இலக்கம் = it;
      எழுத்து *endptr = வெற்று;
      இரட்டை v;
      size_t i;

      க்கு (i = 0; types[i]; i++)
        எனில் (has_prefix (இலக்கம், types[i]))
          {
            இலக்கம் += strlen (types[i]);
            நிறுத்து;
          }

      errno = 0;
      v = strtod (இலக்கம், &endptr);
      எனில் (UNLIKELY (errno != 0 || endptr == இலக்கம் || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        திரும்பு crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, மதிப்பு);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      திரும்பு 0;
    }

  எனில் ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      திரும்பு 0;
    }
  இல்லையெனில் எனில் (*it == '\'')
    {
      cleanup_free எழுத்து *v_start = வெற்று;
      எழுத்து *end;

      it = v_start = xstrdup (மதிப்பு);

      end = strchr (v_start + 1, '\'');
      எனில் (end == வெற்று)
        திரும்பு crun_make_error (err, 0, "invalid variant `%s`", மதிப்பு);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      திரும்பு 0;
    }
  இல்லையெனில் எனில் (*it == '[')
    {
      cleanup_free எழுத்து *v_start = வெற்று;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free எழுத்து **parts = xmalloc (அளவு (எழுத்து *) * parts_size);
      எழுத்து *part;

      part = v_start = xstrdup (it + 1);
      போது (1)
        {
          எழுத்து *out = வெற்று, *next = வெற்று;
          முழு ret;

          ret = parse_sd_array (part, &out, &next, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          parts[n_parts++] = out;
          எனில் (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, அளவு (எழுத்து *) * parts_size);
            }
          parts[n_parts] = வெற்று;
          எனில் (next == வெற்று)
            நிறுத்து;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus close container");

      திரும்பு 0;
    }
  இல்லையெனில் எனில் (has_prefix (it, "uint64 "))
    {
      எழுத்து *endptr = வெற்று;
      uint64_t v;

      errno = 0;
      v = strtoull (it + அளவு ("uint64"), &endptr, 10);
      எனில் (UNLIKELY (errno != 0 || *endptr))
        திரும்பு crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      திரும்பு 0;
    }
  இல்லையெனில் எனில் (has_prefix (it, "int64 "))
    {
      எழுத்து *endptr = வெற்று;
      int64_t v;

      errno = 0;
      v = strtoll (it + அளவு ("int64"), &endptr, 10);
      எனில் (UNLIKELY (errno != 0 || *endptr))
        திரும்பு crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      திரும்பு 0;
    }
  இல்லையெனில் எனில் (has_prefix (it, "uint32 "))
    {
      எழுத்து *endptr = வெற்று;
      uint32_t v;

      errno = 0;
      v = strtoul (it + அளவு ("uint32"), &endptr, 10);
      எனில் (UNLIKELY (errno != 0 || *endptr))
        திரும்பு crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      திரும்பு 0;
    }
  இல்லையெனில் எனில் (has_prefix (it, "int32 ") || strchr (it, ' ') == வெற்று)
    {
      எழுத்து *endptr = வெற்று;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      எனில் (has_prefix (it, "int32 "))
        v = strtol (it + அளவு ("int32"), &endptr, 10);
      இல்லையெனில்
        v = strtol (it, &endptr, 10);
      எனில் (UNLIKELY (errno != 0 || *endptr))
        திரும்பு crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      திரும்பு 0;
    }

  திரும்பு crun_make_error (err, 0, "unknown type for `%s`", name);
}

நிலையான முழு
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  முழு rootless;
  முழு sd_err = 0;

  rootless = is_rootless (err);
  எனில் (UNLIKELY (rootless < 0))
    திரும்பு rootless;

  எனில் (rootless)
    sd_err = sd_bus_default_user (bus);
  எனில் (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  எனில் (sd_err < 0)
    திரும்பு crun_make_error (err, -sd_err, "cannot open sd-bus");

  திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, மாறிலி எழுத்து *name)
{
  size_t i;

  எனில் (resources == வெற்று || resources->unified == வெற்று)
    திரும்பு வெற்று;

  க்கு (i = 0; i < resources->unified->len; i++)
    எனில் (strcmp (resources->unified->சாவிகள்[i], name) == 0)
      திரும்பு resources->unified->மதிப்புகள்[i];
  திரும்பு வெற்று;
}

நிலையான முழு
get_மதிப்பு_from_unified_map (runtime_spec_schema_config_linux_resources *resources, மாறிலி எழுத்து *name,
                            uint64_t *மதிப்பு, libcrun_error_t *err)
{
  எழுத்து *endptr = வெற்று;
  size_t i;

  எனில் (resources == வெற்று || resources->unified == வெற்று)
    திரும்பு 0;

  க்கு (i = 0; i < resources->unified->len; i++)
    எனில் (strcmp (resources->unified->சாவிகள்[i], name) == 0)
      {
        எனில் (strcmp (resources->unified->மதிப்புகள்[i], "max") == 0)
          {
            *மதிப்பு = UINT64_MAX;
            திரும்பு 1;
          }

        errno = 0;
        *மதிப்பு = strtoull (resources->unified->மதிப்புகள்[i], &endptr, 10);
        எனில் (UNLIKELY (errno))
          திரும்பு crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->மதிப்புகள்[i]);
        எனில் (endptr && *endptr)
          திரும்பு crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->மதிப்புகள்[i]);
        திரும்பு 1;
      }
  திரும்பு 0;
}

நிலையான உள்வரி முழு
get_memory_கீழ் (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  எனில் (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      திரும்பு 1;
    }

  திரும்பு get_மதிப்பு_from_unified_map (resources, "memory.low", limit, err);
}

நிலையான உள்வரி முழு
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  எனில் (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      திரும்பு 1;
    }

  திரும்பு get_மதிப்பு_from_unified_map (resources, "pids.max", limit, err);
}

நிலையான உள்வரி முழு
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  எனில் (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      திரும்பு 1;
    }

  திரும்பு get_மதிப்பு_from_unified_map (resources, "memory.max", limit, err);
}

நிலையான உள்வரி முழு
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  எனில் (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      எனில் (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          திரும்பு 1;
        }
      /* 0 means unset.  */
      எனில் (memory->swap == 0)
        திரும்பு 0;
      எனில் (mem <= 0)
        திரும்பு crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      எனில் (memory->swap < mem)
        திரும்பு crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      திரும்பு 1;
    }

  திரும்பு get_மதிப்பு_from_unified_map (resources, "memory.swap.max", limit, err);
}

நிலையான உள்வரி முழு
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t மதிப்பு;
  முழு ret;

  ret = get_மதிப்பு_from_unified_map (resources, "cpu.idle", &மதிப்பு, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  எனில் (ret > 0 && மதிப்பு == 1)
    has_idle = true;
  எனில் (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_மதிப்பு_from_unified_map (resources, "cpu.weight", weight, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  எனில் (ret > 0)
    {
      எனில் (has_idle)
        திரும்பு crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      திரும்பு 1;
    }

  எனில் (resources->cpu && resources->cpu->shares_present)
    {
      எனில் (has_idle)
        திரும்பு crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      எனில் (resources->cpu->shares == 0)
        திரும்பு 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      திரும்பு 1;
    }
  எனில் (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      திரும்பு 1;
    }
  திரும்பு 0;
}

நிலையான முழு
read_uint64_மதிப்பு (மாறிலி எழுத்து *str, uint64_t *மதிப்பு, libcrun_error_t *err)
{
  எழுத்து *endptr = வெற்று;

  errno = 0;
  *மதிப்பு = strtoull (str, &endptr, 10);
  எனில் (UNLIKELY (errno))
    திரும்பு crun_make_error (err, errno, "invalid value: %s", str);
  எனில் (endptr && *endptr)
    திரும்பு crun_make_error (err, 0, "invalid value: %s", str);
  திரும்பு 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
நிலையான உள்வரி uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  திரும்பு bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

நிலையான முழு
append_io_weight (sd_bus_message *m, எழுத்து **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  முழு sd_err;
  size_t i;
  முழு ret;

#  define APPEND_IO_WEIGHT(மதிப்பு)                                                      \
    செய்                                                                                 \
      {                                                                                \
        எனில் (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            எனில் (UNLIKELY (sd_err < 0))                                                 \
              திரும்பு crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } போது (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, மதிப்பு)                                                      \
    செய்                                                                                                \
      {                                                                                               \
        எனில் (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            எனில் (UNLIKELY (sd_err < 0))                                                                \
              திரும்பு crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } போது (0)

  எனில் (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  க்கு (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      எழுத்து name[64];
      முழு len;

      len = snprintf (name, அளவு (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      எனில் (UNLIKELY (len >= (முழு) அளவு (name)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  க்கு (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free எழுத்து *மதிப்பு = வெற்று;
      bool need_conversion = false;
      எழுத்து *saveptr = வெற்று;
      எழுத்து *வரி;

      எனில் (strcmp (resources->unified->சாவிகள்[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      இல்லையெனில் எனில் (strcmp (resources->unified->சாவிகள்[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      இல்லையெனில்
        {
          /* Skip other keys.  */
          தொடர்;
        }

      மதிப்பு = xstrdup (resources->unified->மதிப்புகள்[i]);

      க்கு (வரி = strtok_r (மதிப்பு, "\n", &saveptr); வரி; வரி = strtok_r (வெற்று, "\n", &saveptr))
        {
          எழுத்து *sep;

          sep = strchr (வரி, ' ');
          போது (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_மதிப்பு (sep ? sep : வரி, &weight, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          எனில் (need_conversion)
            weight = IO_WEIGHT (weight);

          எனில் (sep == வெற்று || strcmp (வரி, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          இல்லையெனில்
            APPEND_IO_DEVICE_WEIGHT (வரி, weight);
        }
    }

  திரும்பு 0;
}

/* Adapted from systemd.  */
நிலையான முழு
bus_append_byte_array (sd_bus_message *m, மாறிலி எழுத்து *field, மாறிலி வெறுமை *buf, size_t n, libcrun_error_t *err)
{
  முழு ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  எனில் (ret < 0)
    திரும்பு crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  எனில் (ret < 0)
    திரும்பு crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  எனில் (ret < 0)
    திரும்பு crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  எனில் (ret < 0)
    திரும்பு crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  எனில் (ret < 0)
    திரும்பு crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  எனில் (ret < 0)
    திரும்பு crun_make_error (err, -ret, "sd_bus_message_close_container");

  திரும்பு 1;
}

நிலையான முழு
append_uint64_from_unified_map (sd_bus_message *m,
                                எழுத்து **missing_properties,
                                மாறிலி எழுத்து *attr,
                                மாறிலி எழுத்து *சாவி,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t மதிப்பு;
  முழு ret;

  எனில் (property_missing_p (missing_properties, attr))
    திரும்பு 0;

  ret = get_மதிப்பு_from_unified_map (resources, சாவி, &மதிப்பு, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  எனில் (ret)
    {
      முழு sd_err = sd_bus_message_append (m, "(sv)", attr, "t", மதிப்பு);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  திரும்பு 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
நிலையான முழு
append_device_allow (sd_bus_message *m,
                     மாறிலி எழுத்து type,
                     முழு major,
                     முழு minor,
                     மாறிலி எழுத்து *access,
                     libcrun_error_t *err)
{
  எழுத்து device[64];
  முழு sd_err;
  முழு len;

  எனில் (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      திரும்பு 0;
    }

  எனில் (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, அளவு (device), "%s-*", type == 'c' ? "char" : "block");
  இல்லையெனில் எனில் (IS_WILDCARD (minor))
    len = snprintf (device, அளவு (device), type == 'c' ? "char-%d" : "block-%d", major);
  இல்லையெனில்
    len = snprintf (device, அளவு (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  எனில் (UNLIKELY (len >= (முழு) அளவு (device)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  எனில் (UNLIKELY (sd_err < 0))
    திரும்பு crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  திரும்பு 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

நிலையான முழு
access_to_mode (மாறிலி எழுத்து *access, libcrun_error_t *err)
{
  முழு mode = 0;

  போது (access && *access)
    {
      முழு c = *access++;

      தேர்வு (c)
        {
        நிலை 'r':
          mode |= MODE_R;
          நிறுத்து;

        நிலை 'w':
          mode |= MODE_W;
          நிறுத்து;

        நிலை 'm':
          mode |= MODE_M;
          நிறுத்து;

        இயல்பு:
          திரும்பு crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  திரும்பு mode;
}

/* check if the type b is included in type a.  */
நிலையான bool
has_same_type (மாறிலி எழுத்து *a, மாறிலி எழுத்து *b)
{
  bool a_all = a == வெற்று || strcmp (a, "a") == 0;
  எனில் (b == வெற்று || strcmp (b, "a") == 0)
    திரும்பு a_all;

  திரும்பு a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
நிலையான முழு
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  முழு req_mode;

#  define CMP_DEV_இலக்கம்(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  எனில் (UNLIKELY (req_mode < 0))
    திரும்பு req_mode;

  க்கு (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      முழு mode;

      எனில் (! CMP_DEV_இலக்கம் (devices[n]->minor, devices[current_rule]->minor))
        தொடர்;
      எனில் (! CMP_DEV_இலக்கம் (devices[n]->major, devices[current_rule]->major))
        தொடர்;

      எனில் (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        தொடர்;

      mode = access_to_mode (devices[i]->access, err);
      எனில் (UNLIKELY (mode < 0))
        திரும்பு mode;

      எனில் (devices[current_rule]->allow && (mode & req_mode))
        திரும்பு 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      எனில் (req_mode == 0)
        திரும்பு 1;
    }

  திரும்பு 0;
#  undef CMP_DEV_இலக்கம்
}

நிலையான size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  எனில் (n == 0)
    திரும்பு 1;

  /* Find the first rule that is after the last "block all".  */
  க்கு (i = n - 1; i-- > 0;)
    {
      எனில் ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        திரும்பு i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  க்கு (i = 0; i < n; i++)
    எனில் (devices[i]->allow)
      திரும்பு i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  திரும்பு n + 1;
}

நிலையான bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  முழு remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  க்கு (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      எனில் (! devices[current_rule]->allow)
        திரும்பு false;

      எனில் (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          எனில் (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            திரும்பு true;

          எனில் (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          இல்லையெனில் எனில் (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          எனில் (remaining == 0)
            திரும்பு true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  திரும்பு false;
}

நிலையான முழு
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  கட்டமைப்பு default_dev_s *default_devices = get_default_devices ();
  முழு ret, sd_err;
  size_t i;

  எனில் (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      திரும்பு 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  எனில் (UNLIKELY (sd_err < 0))
    திரும்பு crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  க்கு (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  க்கு (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      எழுத்து type;

      எனில் (! d->allow)
        {
          முழு redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          எனில் (UNLIKELY (redundant < 0))
            திரும்பு redundant;

          எனில் (redundant)
            தொடர்;

          திரும்பு crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      எனில் (d->type == வெற்று || strcmp (d->type, "a") == 0)
        type = 'a';
      இல்லையெனில் எனில் (strcmp (d->type, "c") == 0)
        type = 'c';
      இல்லையெனில் எனில் (strcmp (d->type, "b") == 0)
        type = 'b';
      இல்லையெனில்
        திரும்பு crun_make_error (err, 0, "unknown device type `%s`", d->type);

      எனில் (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
      இல்லையெனில்
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }

  திரும்பு 0;
}

நிலையான bool
has_bpf_fs ()
{
  கட்டமைப்பு statfs stat;

  திரும்பு statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

நிலையான முழு
bpfprog_path_from_scope (எழுத்து **path, மாறிலி எழுத்து *scope, libcrun_error_t *err)
{
  cleanup_free எழுத்து *flat_scope = xstrdup (scope);
  எழுத்து *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  போது ((it = strchr (it, '.')) != வெற்று)
    *it = '_';

  திரும்பு append_paths (path, err, CRUN_BPF_DIR, flat_scope, வெற்று);
}

நிலையான முழு
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 எழுத்து **missing_properties,
                 மாறிலி எழுத்து *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  முழு ret = 0;
  முழு in_userns, sd_err;
  மாறிலி எழுத்து *property_name = "BPFProgram";
  cleanup_free கட்டமைப்பு bpf_program *program = வெற்று;
  cleanup_free எழுத்து *path = வெற்று;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  எனில் (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    திரும்பு 0;

  in_userns = check_running_in_user_namespace (err);
  எனில் (UNLIKELY (in_userns < 0))
    திரும்பு in_userns;

  எனில் (in_userns)
    திரும்பு 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  எனில் (UNLIKELY (program == வெற்று))
    {
      crun_error_release (err);
      திரும்பு 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  எனில் (UNLIKELY (ret < 0 && errno != EEXIST))
    திரும்பு crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  எனில் (is_update)
    {
      cleanup_free கட்டமைப்பு bpf_program *program_loaded = வெற்று;

      எனில் (resources->devices == வெற்று || resources->devices_len == 0)
        {
          *devices_set = true;
          திரும்பு 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      /* it is the same as the loaded program.  */
      எனில் (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          திரும்பு 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free கட்டமைப்பு bpf_program *program_new = வெற்று;
        எழுத்து tmp[64];
        முழு len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, அளவு (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        எனில் (UNLIKELY (len >= (முழு) அளவு (tmp)))
          திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        எனில் (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            திரும்பு 0;
          }
      }

      திரும்பு crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  எனில் (UNLIKELY (sd_err < 0))
    திரும்பு crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  திரும்பு 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
நிலையான முழு
append_cpu_quota (sd_bus_message *m, எழுத்து **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  மாறிலி எழுத்து *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  முழு sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  எனில் (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      முழு ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      எனில் (has_period)
        period = max_period;
    }

  எனில் (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  எனில் ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      எனில் (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          எனில் (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      எனில் (UNLIKELY (sd_err < 0))
        திரும்பு crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  திரும்பு 0;
}

நிலையான முழு
append_resources (sd_bus_message *m,
                  bool is_update,
                  மாறிலி எழுத்து *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  முழு cgroup_mode,
                  மாறிலி எழுத்து *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t மதிப்பு;
  முழு sd_err;
  முழு ret;
  cleanup_free எழுத்து *dir = வெற்று;
  cleanup_free எழுத்து **missing_properties = வெற்று;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  missing_properties = read_dir_entries (dir, err);
  எனில் (UNLIKELY (missing_properties == வெற்று))
    {
      எனில் (crun_error_get_errno (err) != ENOENT)
        திரும்பு -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_மதிப்பு(name, மதிப்பு)                                             \
    செய்                                                                                 \
      {                                                                                \
        எனில் (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", மதிப்பு);              \
            எனில் (UNLIKELY (sd_err < 0))                                                 \
              திரும்பு crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } போது (0)

#  define APPEND_UINT64(name, fn)              \
    செய்                                         \
      {                                        \
        ret = fn (resources, &மதிப்பு, err);     \
        எனில் (UNLIKELY (ret < 0))                \
          திரும்பு ret;                          \
        எனில் (ret)                               \
          {                                    \
            APPEND_UINT64_மதிப்பு (name, மதிப்பு); \
          }                                    \
    } போது (0)

  எனில் (resources == வெற்று)
    திரும்பு 0;

  APPEND_UINT64 ("MemoryLow", get_memory_கீழ்);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  தேர்வு (cgroup_mode)
    {
    நிலை CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;

        எனில் (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            மாறிலி எழுத்து *property_name = "AllowedCPUs";
            மாறிலி எழுத்து *மதிப்பு = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free எழுத்து *allowed_cpus = வெற்று;
            size_t allowed_cpus_len = 0;

            எனில் (மதிப்பு == வெற்று)
              மதிப்பு = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (மதிப்பு, &allowed_cpus, &allowed_cpus_len, err);
            எனில் (UNLIKELY (ret < 0))
              திரும்பு ret;

            எனில் (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                எனில் (UNLIKELY (ret < 0))
                  திரும்பு ret;
              }
          }

        எனில் (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            மாறிலி எழுத்து *property_name = "AllowedMemoryNodes";
            மாறிலி எழுத்து *மதிப்பு = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free எழுத்து *allowed_mems = வெற்று;
            size_t allowed_mems_len = 0;

            எனில் (மதிப்பு == வெற்று)
              மதிப்பு = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (மதிப்பு, &allowed_mems, &allowed_mems_len, err);
            எனில் (UNLIKELY (ret < 0))
              திரும்பு ret;

            எனில் (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                எனில் (UNLIKELY (ret < 0))
                  திரும்பு ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        எனில் (UNLIKELY (ret < 0))
          திரும்பு ret;
      }
      நிறுத்து;

    நிலை CGROUP_MODE_LEGACY:
    நிலை CGROUP_MODE_HYBRID:
      எனில் (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_மதிப்பு ("CPUShares", resources->cpu->shares);
      நிறுத்து;

    இயல்பு:
      திரும்பு crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_மதிப்பு

  எனில் (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு 0;
}

நிலையான முழு
reset_failed_unit (sd_bus *bus, மாறிலி எழுத்து *unit)
{
  முழு sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = வெற்று, *reply = வெற்று;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  எனில் (UNLIKELY (sd_err < 0))
    செல் exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  எனில் (UNLIKELY (sd_err < 0))
    செல் exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  எனில் (UNLIKELY (sd_err < 0))
    செல் exit;

  sd_err = 0;

exit:
  எனில் (m)
    sd_bus_message_unref (m);
  எனில் (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  திரும்பு sd_err;
}

நிலையான முழு
verify_ebpf_device_filter_installed (மாறிலி எழுத்து *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = வெற்று;
  cleanup_free எழுத்து *full_path = வெற்று;
  size_t n_progs = 0;
  முழு ret;

  ret = append_paths (&full_path, err, CGROUP_வேர், cgroup_path, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (n_progs == 0)
    திரும்பு crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  திரும்பு 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
நிலையான முழு
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, முழு ret)
{
  எனில் (bus)
    sd_bus_unref (bus);
  எனில் (m)
    sd_bus_message_unref (m);
  எனில் (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  திரும்பு ret;
}

நிலையான முழு
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            முழு cgroup_mode,
                            string_map *annotations,
                            மாறிலி எழுத்து *state_வேர்,
                            மாறிலி எழுத்து *scope, மாறிலி எழுத்து *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = வெற்று;
  sd_bus_message *m = வெற்று;
  sd_bus_message *reply = வெற்று;
  முழு sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  மாறிலி எழுத்து *object = வெற்று;
  கட்டமைப்பு systemd_job_removed_s job_data = {};
  முழு i;
  மாறிலி எழுத்து *boolean_opts[10];
  cleanup_free எழுத்து *state_dir = வெற்று;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_வேர், வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = வெற்று;

  ret = open_sd_bus_connection (&bus, err);
  எனில் (UNLIKELY (ret < 0))
    செல் exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  எனில் (UNLIKELY (ret < 0))
    செல் exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      செல் exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      செல் exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      செல் exit;
    }

  எனில் (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      எனில் (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          செல் exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      செல் exit;
    }

  எனில் (annotations)
    {
      size_t prefix_len = அளவு (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      க்கு (i = 0; i < annotations_len; i++)
        {
          மாறிலி எழுத்து *சாவி;
          மாறிலி எழுத்து *மதிப்பு;
          size_t len;

          ret = string_map_get_at (annotations, i, &சாவி, &மதிப்பு);
          எனில் (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              செல் exit;
            }

          எனில் (! has_prefix (சாவி, SYSTEMD_PROPERTY_PREFIX))
            தொடர்;

          len = strlen (சாவி);
          எனில் (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", சாவி);
              செல் exit;
            }

          ret = append_systemd_annotation (m, சாவி + prefix_len, len - prefix_len,
                                           மதிப்பு, err);
          எனில் (UNLIKELY (ret < 0))
            செல் exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      செல் exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      செல் exit;
    }

  க்கு (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      எனில் (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          செல் exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  எனில் (UNLIKELY (ret < 0))
    செல் exit;

  sd_err = sd_bus_message_close_container (m);
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      செல் exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      செல் exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  எனில் (UNLIKELY (sd_err < 0))
    {
      எனில் (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          எனில் (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = வெற்று;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      எனில் (sd_err < 0)
        {
          எனில் (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              எனில் (UNLIKELY (ret < 0))
                செல் exit;
              எனில் (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          செல் exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      செல் exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  திரும்பு cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

நிலையான முழு
libcrun_destroy_systemd_cgroup_scope (கட்டமைப்பு libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = வெற்று;
  sd_bus_message *m = வெற்று;
  sd_bus_message *reply = வெற்று;
  முழு ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  மாறிலி எழுத்து *object;
  மாறிலி எழுத்து *scope = cgroup_status->scope;
  கட்டமைப்பு systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  எனில் (UNLIKELY (ret < 0))
    செல் exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  எனில் (UNLIKELY (ret < 0))
    செல் exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  எனில் (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      செல் exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  எனில் (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      செல் exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  எனில் (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      செல் exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  எனில் (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      செல் exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  திரும்பு cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

நிலையான மாறிலி எழுத்து *
find_systemd_subgroup (string_map *annotations)
{
  மாறிலி எழுத்து *annotation;

  annotation = find_string_map_மதிப்பு (annotations, "run.oci.systemd.subgroup");
  எனில் (annotation)
    {
      எனில் (annotation[0] == '\0')
        திரும்பு வெற்று;
      திரும்பு annotation;
    }

  திரும்பு "container";
}

நிலையான முழு
libcrun_cgroup_enter_systemd (கட்டமைப்பு libcrun_cgroup_args *args,
                              கட்டமைப்பு libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  மாறிலி எழுத்து *cgroup_path = args->cgroup_path;
  cleanup_free எழுத்து *scope_path = வெற்று;
  cleanup_free எழுத்து *scope = வெற்று;
  cleanup_free எழுத்து *path = வெற்று;
  cleanup_free எழுத்து *slice = வெற்று;
  முழு retries_இடது = 32;
  மாறிலி எழுத்து *suffix;
  மாறிலி எழுத்து *id = args->id;
  pid_t pid = args->pid;
  முழு cgroup_mode;
  முழு rootless = 0;
  முழு ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      எனில் (UNLIKELY (rootless < 0))
        திரும்பு rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  க்கு (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_வேர்,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      எனில் (LIKELY (ret >= 0))
        நிறுத்து;

      எனில் (can_retry && retries_இடது-- > 0)
        {
          crun_error_release (err);
          தொடர்;
        }

      திரும்பு ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  எனில் (is_empty_string (suffix))
    scope_path = xstrdup (path);
  இல்லையெனில்
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      எனில் (UNLIKELY (scope_path == வெற்று))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  எனில் (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  out->path = path;
  path = வெற்று;

  out->scope = scope;
  scope = வெற்று;
  திரும்பு 0;
}

எழுத்து *
get_cgroup_scope_path (மாறிலி எழுத்து *cgroup_path, மாறிலி எழுத்து *scope)
{
  எழுத்து *path_to_scope = வெற்று;
  எழுத்து *cur;

  எனில் (is_empty_string (cgroup_path))
    திரும்பு வெற்று;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  போது (cur)
    {
      எழுத்து *next = strchr (cur + 1, '/');
      எனில் (next == வெற்று)
        நிறுத்து;

      *next = '\0';
      எனில் (strcmp (cur, scope) == 0)
        திரும்பு path_to_scope;
      *next = '/';

      cur = next;
      போது (*cur == '/')
        cur++;
    }

  திரும்பு path_to_scope;
}

நிலையான முழு
libcrun_destroy_cgroup_systemd (கட்டமைப்பு libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free எழுத்து *path_to_scope = வெற்று;
  cleanup_free எழுத்து *bpfprog = வெற்று;
  முழு mode;
  முழு ret;

  mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (mode < 0))
    திரும்பு mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  எனில் (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  எனில் (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  எனில் (UNLIKELY (ret < 0))
    crun_error_release (err);
  இல்லையெனில்
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  திரும்பு destroy_cgroup_path (path_to_scope, mode, err);
}

நிலையான முழு
libcrun_update_resources_systemd (கட்டமைப்பு libcrun_cgroup_status *cgroup_status,
                                  மாறிலி எழுத்து *state_வேர்,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  கட்டமைப்பு systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free எழுத்து *state_dir = வெற்று;
  sd_bus_message *reply = வெற்று;
  sd_bus_message *m = வெற்று;
  sd_bus *bus = வெற்று;
  முழு sd_err, ret;
  முழு cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_வேர், வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  எனில் (UNLIKELY (ret < 0))
    செல் exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      செல் exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      செல் exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      செல் exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  எனில் (UNLIKELY (ret < 0))
    செல் exit;

  sd_err = sd_bus_message_close_container (m);
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      செல் exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  எனில் (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      செல் exit;
    }

  எனில் (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      எனில் (UNLIKELY (ret < 0))
        செல் exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      எனில் (UNLIKELY (ret < 0))
        செல் exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  எனில் (UNLIKELY (ret < 0))
    செல் exit;

  ret = 0;

exit:
  திரும்பு cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
நிலையான முழு
libcrun_attach_process_systemd (கட்டமைப்பு libcrun_cgroup_status *cgroup_status,
                                மாறிலி எழுத்து *path, pid_t pid,
                                libcrun_error_t *err)
{
  மாறிலி எழுத்து *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free எழுத்து *scope_path = வெற்று;
  sd_bus_message *reply = வெற்று;
  மாறிலி எழுத்து *subcgroup;
  மாறிலி எழுத்து *last;
  sd_bus *bus = வெற்று;
  size_t len;
  முழு sd_err, ret;

  எனில் (is_empty_string (scope))
    திரும்பு crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  எனில் (scope_path == வெற்று)
    திரும்பு crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  எனில் (last == வெற்று || strcmp (last + 1, scope) != 0)
    திரும்பு crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  எனில் (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  திரும்பு cleanup_sd_bus_and_return (bus, வெற்று, reply, &error, ret);
}

#இல்லையெனில்
நிலையான முழு
libcrun_cgroup_enter_systemd (கட்டமைப்பு libcrun_cgroup_args *args,
                              கட்டமைப்பு libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (வெறுமை) args;
  (வெறுமை) out;

  திரும்பு crun_make_error (err, ENOTSUP, "systemd not supported");
}

நிலையான முழு
libcrun_destroy_cgroup_systemd (கட்டமைப்பு libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (வெறுமை) cgroup_status;

  திரும்பு crun_make_error (err, ENOTSUP, "systemd not supported");
}

நிலையான முழு
libcrun_update_resources_systemd (கட்டமைப்பு libcrun_cgroup_status *cgroup_status,
                                  மாறிலி எழுத்து *state_வேர்,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (வெறுமை) cgroup_status;
  (வெறுமை) state_வேர்;
  (வெறுமை) resources;

  திரும்பு crun_make_error (err, ENOTSUP, "systemd not supported");
}

நிலையான முழு
libcrun_attach_process_systemd (கட்டமைப்பு libcrun_cgroup_status *cgroup_status,
                                மாறிலி எழுத்து *path, pid_t pid,
                                libcrun_error_t *err)
{
  (வெறுமை) cgroup_status;
  (வெறுமை) path;
  (வெறுமை) pid;

  திரும்பு crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

கட்டமைப்பு libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = வெற்று,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
