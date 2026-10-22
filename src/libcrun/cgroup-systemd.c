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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "cgroup.h"
#অন্তর্ভাব "cgroup-internal.h"
#অন্তর্ভাব "cgroup-systemd.h"
#অন্তর্ভাব "cgroup-resources.h"
#অন্তর্ভাব "cgroup-utils.h"
#অন্তর্ভাব "ebpf.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "status.h"
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <sys/vfs.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/vfs.h>
#অন্তর্ভাব <linux/magic.h>

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

স্থির পূর্ণাঙ্ক
register_missing_property_from_message (ধ্রুব বর্ণ *state_dir, ধ্রুব বর্ণ *message, libcrun_error_t *err)
{
  cleanup_free বর্ণ *file_path = রিক্ত;
  cleanup_free বর্ণ *dir_path = রিক্ত;
  cleanup_free বর্ণ *property = রিক্ত;
  বর্ণ *p;
  পূর্ণাঙ্ক ret;

  যদি (! has_prefix (message, "Cannot set property "))
    প্রত্যাগম 0;

  property = xstrdup (message + পরিমাণ ("Cannot set property ") - 1);
  p = strchr (property, ',');
  যদি (! p)
    প্রত্যাগম 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = append_paths (&file_path, err, dir_path, property, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, রিক্ত, 0, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম 1;
}

স্থির bool
property_missing_p (বর্ণ **missing_properties, ধ্রুব বর্ণ *property)
{
  size_t i;

  প্রতি (i = 0; missing_properties && missing_properties[i]; i++)
    যদি (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        প্রত্যাগম true;
      }

  প্রত্যাগম false;
}

স্থির শূন্য
get_systemd_scope_and_slice (ধ্রুব বর্ণ *id, bool user_slice, ধ্রুব বর্ণ *cgroup_path,
                             বর্ণ **scope, বর্ণ **slice)
{
  ধ্রুব বর্ণ *n;
  বর্ণ *p;

  যদি (cgroup_path == রিক্ত || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      প্রত্যাগম;
    }

  n = strchr (cgroup_path, ':');
  যদি (n == রিক্ত)
    xasprintf (scope, "%s.scope", cgroup_path);
  অন্যথা
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      যদি (p)
        *p = '-';
    }
  যদি (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      যদি (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      যদি (is_empty_string (*slice))
        {
          মুক্ত (*slice);
          যদি (user_slice)
            *slice = xstrdup ("user.slice");
          অন্যথা
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
স্থির পূর্ণাঙ্ক
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  cleanup_free বর্ণ *cgroup_path = রিক্ত;
  cleanup_close পূর্ণাঙ্ক dirfd = -1;
  bool need_set_parent = true;
  বর্ণ fmt_buf[64];
  পূর্ণাঙ্ক len;
  পূর্ণাঙ্ক ret;

  যদি (resources == রিক্ত || resources->cpu == রিক্ত)
    প্রত্যাগম 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  যদি (is_empty_string (path))
    প্রত্যাগম crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  যদি (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_মূল, "cpu", path, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (dirfd < 0))
    প্রত্যাগম crun_make_error (err, errno, "open `%s`", cgroup_path);

  যদি (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

      যদি (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, পরিমাণ (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (fmt_buf)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

      যদি (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  cleanup_free বর্ণ *cgroup_path = রিক্ত;
  পূর্ণাঙ্ক parent;
  পূর্ণাঙ্ক ret;

  যদি (resources == রিক্ত || resources->cpu == রিক্ত)
    প্রত্যাগম 0;

  যদি (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    প্রত্যাগম 0;

  প্রতি (parent = 0; parent < 2; parent++)
    {
      cleanup_close পূর্ণাঙ্ক dirfd = -1;

      যদি (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_মূল, path ? path : "", (parent ? ".." : রিক্ত), রিক্ত);
      অন্যথা
        ret = append_paths (&cgroup_path, err, CGROUP_মূল, "/cpu", path ? path : "", (parent ? ".." : রিক্ত), রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (dirfd < 0))
        প্রত্যাগম crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      যদি (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }
    }

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  cleanup_free বর্ণ *cgroup_path = রিক্ত;
  পূর্ণাঙ্ক parent;
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক levels;

  ret = append_paths (&cgroup_path, err, CGROUP_মূল, "/cpuset", path ? path : "", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  প্রতি (parent = 0; parent < levels; parent++)
    {
      cleanup_close পূর্ণাঙ্ক dirfd_cpuset = -1;
      cleanup_free বর্ণ *path_to_cpuset = রিক্ত;

      ret = append_paths (&path_to_cpuset, err, CGROUP_মূল "/cpuset", path, (parent ? ".." : রিক্ত), রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (dirfd_cpuset < 0))
        প্রত্যাগম crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  প্রত্যাগম 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
স্থির পূর্ণাঙ্ক
compute_finalized_path (বর্ণ **path, ধ্রুব বর্ণ *from, ধ্রুব বর্ণ *suffix, libcrun_error_t *err)
{
  যদি (suffix == রিক্ত)
    {
      *path = xstrdup (from);
      প্রত্যাগম 0;
    }
  প্রত্যাগম append_paths (path, err, from, suffix, রিক্ত);
}

স্থির পূর্ণাঙ্ক
systemd_finalize (সংরচনা libcrun_cgroup_args *args, বর্ণ **path_out,
                  পূর্ণাঙ্ক cgroup_mode, ধ্রুব বর্ণ *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free বর্ণ *content = রিক্ত;
  cleanup_free বর্ণ *path = রিক্ত;
  pid_t pid = args->pid;
  পূর্ণাঙ্ক ret;
  বর্ণ *from, *to;
  বর্ণ *saveptr = রিক্ত;

  cleanup_free বর্ণ *proc_path = রিক্ত;
  cleanup_close পূর্ণাঙ্ক fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম fd;

  ret = read_all_fd (fd, "cgroup path", &content, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  বিকল্প (cgroup_mode)
    {
    পক্ষ CGROUP_MODE_HYBRID:
    পক্ষ CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      যদি (LIKELY (from != রিক্ত))
        from += 8;
      অন্যথা
        {
          from = strstr (content, ":pids:");
          যদি (UNLIKELY (from == রিক্ত))
            প্রত্যাগম crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      যদি (UNLIKELY (to == রিক্ত))
        প্রত্যাগম crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      যদি (geteuid ())
        প্রত্যাগম 0;

      ret = setup_rt_runtime (resources, path, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      প্রতি (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (রিক্ত, "\n", &saveptr))
        {
          বর্ণ *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          যদি (subsystem[0] == '\0')
            {
              যদি (cgroup_mode == CGROUP_MODE_LEGACY)
                অনুবর্ত;

              subsystem = "unified";
            }

          যদি (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              যদি (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  যদি (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      অনুবর্ত;
                    }
                  প্রত্যাগম ret;
                }
            }
        }

      ভঙ্গ;

    পক্ষ CGROUP_MODE_UNIFIED:
      {
        cleanup_free বর্ণ *dir = রিক্ত;

        from = strstr (content, "0::");
        যদি (UNLIKELY (from == রিক্ত))
          প্রত্যাগম crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        যদি (UNLIKELY (to == রিক্ত))
          প্রত্যাগম crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        ret = append_paths (&dir, err, CGROUP_মূল, path, রিক্ত);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        ret = move_process_to_cgroup (pid, রিক্ত, path, err);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        ret = enable_controllers (path, err);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        যদি (suffix)
          {
            ret = chown_cgroups (path, args->মূল_uid, args->মূল_gid, err);
            যদি (UNLIKELY (ret < 0))
              প্রত্যাগম ret;
          }
      }
      ভঙ্গ;

    স্বভাব:
      প্রত্যাগম crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  *path_out = path;
  path = রিক্ত;

  প্রত্যাগম 0;
}

সংরচনা systemd_job_removed_s
{
  ধ্রুব বর্ণ *path;
  ধ্রুব বর্ণ *op;
  পূর্ণাঙ্ক terminated;
  libcrun_error_t err;
};

স্থির পূর্ণাঙ্ক
systemd_job_removed (sd_bus_message *m, শূন্য *userdata, sd_bus_error *error arg_unused)
{
  ধ্রুব বর্ণ *path, *unit, *result;
  uint32_t id;
  পূর্ণাঙ্ক ret;
  সংরচনা systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  যদি (ret < 0)
    প্রত্যাগম -1;

  যদি (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      যদি (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          প্রত্যাগম -1;
        }
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
systemd_check_job_status_setup (sd_bus *bus, সংরচনা systemd_job_removed_s *data, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = sd_bus_match_signal_async (bus, রিক্ত, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, রিক্ত, data);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, -ret, "sd-bus match signal");

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
systemd_check_job_status (sd_bus *bus, সংরচনা systemd_job_removed_s *data, ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *op,
                          libcrun_error_t *err)
{
  পূর্ণাঙ্ক sd_err;

  data->path = path;
  data->op = op;
  যাবত্ (! data->terminated)
    {
      sd_err = sd_bus_process (bus, রিক্ত);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus process");

      যদি (sd_err != 0)
        অনুবর্ত;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus wait");
    }

  যদি (data->err != রিক্ত)
    {
      *err = data->err;
      প্রত্যাগম -1;
    }

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
parse_sd_array (বর্ণ *s, বর্ণ **out, বর্ণ **next, libcrun_error_t *err)
{
  বর্ণ endchr;
  বর্ণ *it, *dest;
  bool escaped = false;

  *out = রিক্ত;
  *next = রিক্ত;

  যাবত্ (isspace (*s))
    s++;
  যদি (*s == '\0')
    প্রত্যাগম 0;
  অন্যথা যদি (*s != '\'' && *s != '"')
    প্রত্যাগম crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  যাবত্ (1)
    {
      যদি (*it == '\0')
        প্রত্যাগম crun_make_error (err, 0, "invalid string `%s`", s);
      যদি (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          যাবত্ (isspace (*it))
            it++;
          যদি (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              প্রত্যাগম 0;
            }

          যদি (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              প্রত্যাগম 0;
            }

          প্রত্যাগম crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      যদি (! escaped)
        *dest++ = *it;
      it++;
    }

  প্রত্যাগম 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
স্থির পূর্ণাঙ্ক
append_systemd_annotation (sd_bus_message *m, ধ্রুব বর্ণ *name, size_t name_len, ধ্রুব বর্ণ *মান,
                           libcrun_error_t *err)
{
  cleanup_free বর্ণ *tmp_name = রিক্ত;
  uint32_t factor = 1;
  ধ্রুব বর্ণ *it;
  পূর্ণাঙ্ক sd_err;

  যাবত্ (*মান == ' ')
    মান++;

  it = মান;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  যদি (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  যদি (factor != 1)
    {
      স্থির ধ্রুব বর্ণ *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", রিক্ত };
      ধ্রুব বর্ণ *অঙ্ক = it;
      বর্ণ *endptr = রিক্ত;
      দ্বিগুণ v;
      size_t i;

      প্রতি (i = 0; types[i]; i++)
        যদি (has_prefix (অঙ্ক, types[i]))
          {
            অঙ্ক += strlen (types[i]);
            ভঙ্গ;
          }

      errno = 0;
      v = strtod (অঙ্ক, &endptr);
      যদি (UNLIKELY (errno != 0 || endptr == অঙ্ক || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        প্রত্যাগম crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, মান);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      প্রত্যাগম 0;
    }

  যদি ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      প্রত্যাগম 0;
    }
  অন্যথা যদি (*it == '\'')
    {
      cleanup_free বর্ণ *v_start = রিক্ত;
      বর্ণ *end;

      it = v_start = xstrdup (মান);

      end = strchr (v_start + 1, '\'');
      যদি (end == রিক্ত)
        প্রত্যাগম crun_make_error (err, 0, "invalid variant `%s`", মান);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      প্রত্যাগম 0;
    }
  অন্যথা যদি (*it == '[')
    {
      cleanup_free বর্ণ *v_start = রিক্ত;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free বর্ণ **parts = xmalloc (পরিমাণ (বর্ণ *) * parts_size);
      বর্ণ *part;

      part = v_start = xstrdup (it + 1);
      যাবত্ (1)
        {
          বর্ণ *out = রিক্ত, *next = রিক্ত;
          পূর্ণাঙ্ক ret;

          ret = parse_sd_array (part, &out, &next, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;

          parts[n_parts++] = out;
          যদি (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, পরিমাণ (বর্ণ *) * parts_size);
            }
          parts[n_parts] = রিক্ত;
          যদি (next == রিক্ত)
            ভঙ্গ;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus close container");

      প্রত্যাগম 0;
    }
  অন্যথা যদি (has_prefix (it, "uint64 "))
    {
      বর্ণ *endptr = রিক্ত;
      uint64_t v;

      errno = 0;
      v = strtoull (it + পরিমাণ ("uint64"), &endptr, 10);
      যদি (UNLIKELY (errno != 0 || *endptr))
        প্রত্যাগম crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      প্রত্যাগম 0;
    }
  অন্যথা যদি (has_prefix (it, "int64 "))
    {
      বর্ণ *endptr = রিক্ত;
      int64_t v;

      errno = 0;
      v = strtoll (it + পরিমাণ ("int64"), &endptr, 10);
      যদি (UNLIKELY (errno != 0 || *endptr))
        প্রত্যাগম crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      প্রত্যাগম 0;
    }
  অন্যথা যদি (has_prefix (it, "uint32 "))
    {
      বর্ণ *endptr = রিক্ত;
      uint32_t v;

      errno = 0;
      v = strtoul (it + পরিমাণ ("uint32"), &endptr, 10);
      যদি (UNLIKELY (errno != 0 || *endptr))
        প্রত্যাগম crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      প্রত্যাগম 0;
    }
  অন্যথা যদি (has_prefix (it, "int32 ") || strchr (it, ' ') == রিক্ত)
    {
      বর্ণ *endptr = রিক্ত;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      যদি (has_prefix (it, "int32 "))
        v = strtol (it + পরিমাণ ("int32"), &endptr, 10);
      অন্যথা
        v = strtol (it, &endptr, 10);
      যদি (UNLIKELY (errno != 0 || *endptr))
        প্রত্যাগম crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      প্রত্যাগম 0;
    }

  প্রত্যাগম crun_make_error (err, 0, "unknown type for `%s`", name);
}

স্থির পূর্ণাঙ্ক
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  পূর্ণাঙ্ক rootless;
  পূর্ণাঙ্ক sd_err = 0;

  rootless = is_rootless (err);
  যদি (UNLIKELY (rootless < 0))
    প্রত্যাগম rootless;

  যদি (rootless)
    sd_err = sd_bus_default_user (bus);
  যদি (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  যদি (sd_err < 0)
    প্রত্যাগম crun_make_error (err, -sd_err, "cannot open sd-bus");

  প্রত্যাগম 0;
}

স্থির ধ্রুব বর্ণ *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ধ্রুব বর্ণ *name)
{
  size_t i;

  যদি (resources == রিক্ত || resources->unified == রিক্ত)
    প্রত্যাগম রিক্ত;

  প্রতি (i = 0; i < resources->unified->len; i++)
    যদি (strcmp (resources->unified->চাবিগুলি[i], name) == 0)
      প্রত্যাগম resources->unified->মানসমূহ[i];
  প্রত্যাগম রিক্ত;
}

স্থির পূর্ণাঙ্ক
get_মান_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ধ্রুব বর্ণ *name,
                            uint64_t *মান, libcrun_error_t *err)
{
  বর্ণ *endptr = রিক্ত;
  size_t i;

  যদি (resources == রিক্ত || resources->unified == রিক্ত)
    প্রত্যাগম 0;

  প্রতি (i = 0; i < resources->unified->len; i++)
    যদি (strcmp (resources->unified->চাবিগুলি[i], name) == 0)
      {
        যদি (strcmp (resources->unified->মানসমূহ[i], "max") == 0)
          {
            *মান = UINT64_MAX;
            প্রত্যাগম 1;
          }

        errno = 0;
        *মান = strtoull (resources->unified->মানসমূহ[i], &endptr, 10);
        যদি (UNLIKELY (errno))
          প্রত্যাগম crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->মানসমূহ[i]);
        যদি (endptr && *endptr)
          প্রত্যাগম crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->মানসমূহ[i]);
        প্রত্যাগম 1;
      }
  প্রত্যাগম 0;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
get_memory_নিচের (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  যদি (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      প্রত্যাগম 1;
    }

  প্রত্যাগম get_মান_from_unified_map (resources, "memory.low", limit, err);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  যদি (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      প্রত্যাগম 1;
    }

  প্রত্যাগম get_মান_from_unified_map (resources, "pids.max", limit, err);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  যদি (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      প্রত্যাগম 1;
    }

  প্রত্যাগম get_মান_from_unified_map (resources, "memory.max", limit, err);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  যদি (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      যদি (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          প্রত্যাগম 1;
        }
      /* 0 means unset.  */
      যদি (memory->swap == 0)
        প্রত্যাগম 0;
      যদি (mem <= 0)
        প্রত্যাগম crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      যদি (memory->swap < mem)
        প্রত্যাগম crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      প্রত্যাগম 1;
    }

  প্রত্যাগম get_মান_from_unified_map (resources, "memory.swap.max", limit, err);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t মান;
  পূর্ণাঙ্ক ret;

  ret = get_মান_from_unified_map (resources, "cpu.idle", &মান, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;
  যদি (ret > 0 && মান == 1)
    has_idle = true;
  যদি (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_মান_from_unified_map (resources, "cpu.weight", weight, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;
  যদি (ret > 0)
    {
      যদি (has_idle)
        প্রত্যাগম crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      প্রত্যাগম 1;
    }

  যদি (resources->cpu && resources->cpu->shares_present)
    {
      যদি (has_idle)
        প্রত্যাগম crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      যদি (resources->cpu->shares == 0)
        প্রত্যাগম 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      প্রত্যাগম 1;
    }
  যদি (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      প্রত্যাগম 1;
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
read_uint64_মান (ধ্রুব বর্ণ *str, uint64_t *মান, libcrun_error_t *err)
{
  বর্ণ *endptr = রিক্ত;

  errno = 0;
  *মান = strtoull (str, &endptr, 10);
  যদি (UNLIKELY (errno))
    প্রত্যাগম crun_make_error (err, errno, "invalid value: %s", str);
  যদি (endptr && *endptr)
    প্রত্যাগম crun_make_error (err, 0, "invalid value: %s", str);
  প্রত্যাগম 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
স্থির অন্তঃস্থ uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  প্রত্যাগম bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

স্থির পূর্ণাঙ্ক
append_io_weight (sd_bus_message *m, বর্ণ **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  পূর্ণাঙ্ক sd_err;
  size_t i;
  পূর্ণাঙ্ক ret;

#  define APPEND_IO_WEIGHT(মান)                                                      \
    কুরু                                                                                 \
      {                                                                                \
        যদি (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            যদি (UNLIKELY (sd_err < 0))                                                 \
              প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } যাবত্ (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, মান)                                                      \
    কুরু                                                                                                \
      {                                                                                               \
        যদি (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            যদি (UNLIKELY (sd_err < 0))                                                                \
              প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } যাবত্ (0)

  যদি (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  প্রতি (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      বর্ণ name[64];
      পূর্ণাঙ্ক len;

      len = snprintf (name, পরিমাণ (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (name)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  প্রতি (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free বর্ণ *মান = রিক্ত;
      bool need_conversion = false;
      বর্ণ *saveptr = রিক্ত;
      বর্ণ *সারি;

      যদি (strcmp (resources->unified->চাবিগুলি[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      অন্যথা যদি (strcmp (resources->unified->চাবিগুলি[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      অন্যথা
        {
          /* Skip other keys.  */
          অনুবর্ত;
        }

      মান = xstrdup (resources->unified->মানসমূহ[i]);

      প্রতি (সারি = strtok_r (মান, "\n", &saveptr); সারি; সারি = strtok_r (রিক্ত, "\n", &saveptr))
        {
          বর্ণ *sep;

          sep = strchr (সারি, ' ');
          যাবত্ (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_মান (sep ? sep : সারি, &weight, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          যদি (need_conversion)
            weight = IO_WEIGHT (weight);

          যদি (sep == রিক্ত || strcmp (সারি, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          অন্যথা
            APPEND_IO_DEVICE_WEIGHT (সারি, weight);
        }
    }

  প্রত্যাগম 0;
}

/* Adapted from systemd.  */
স্থির পূর্ণাঙ্ক
bus_append_byte_array (sd_bus_message *m, ধ্রুব বর্ণ *field, ধ্রুব শূন্য *buf, size_t n, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  যদি (ret < 0)
    প্রত্যাগম crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  যদি (ret < 0)
    প্রত্যাগম crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  যদি (ret < 0)
    প্রত্যাগম crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  যদি (ret < 0)
    প্রত্যাগম crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  যদি (ret < 0)
    প্রত্যাগম crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  যদি (ret < 0)
    প্রত্যাগম crun_make_error (err, -ret, "sd_bus_message_close_container");

  প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক
append_uint64_from_unified_map (sd_bus_message *m,
                                বর্ণ **missing_properties,
                                ধ্রুব বর্ণ *attr,
                                ধ্রুব বর্ণ *চাবি,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t মান;
  পূর্ণাঙ্ক ret;

  যদি (property_missing_p (missing_properties, attr))
    প্রত্যাগম 0;

  ret = get_মান_from_unified_map (resources, চাবি, &মান, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;
  যদি (ret)
    {
      পূর্ণাঙ্ক sd_err = sd_bus_message_append (m, "(sv)", attr, "t", মান);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  প্রত্যাগম 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
স্থির পূর্ণাঙ্ক
append_device_allow (sd_bus_message *m,
                     ধ্রুব বর্ণ type,
                     পূর্ণাঙ্ক major,
                     পূর্ণাঙ্ক minor,
                     ধ্রুব বর্ণ *access,
                     libcrun_error_t *err)
{
  বর্ণ device[64];
  পূর্ণাঙ্ক sd_err;
  পূর্ণাঙ্ক len;

  যদি (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      প্রত্যাগম 0;
    }

  যদি (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, পরিমাণ (device), "%s-*", type == 'c' ? "char" : "block");
  অন্যথা যদি (IS_WILDCARD (minor))
    len = snprintf (device, পরিমাণ (device), type == 'c' ? "char-%d" : "block-%d", major);
  অন্যথা
    len = snprintf (device, পরিমাণ (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (device)))
    প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  যদি (UNLIKELY (sd_err < 0))
    প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  প্রত্যাগম 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

স্থির পূর্ণাঙ্ক
access_to_mode (ধ্রুব বর্ণ *access, libcrun_error_t *err)
{
  পূর্ণাঙ্ক mode = 0;

  যাবত্ (access && *access)
    {
      পূর্ণাঙ্ক c = *access++;

      বিকল্প (c)
        {
        পক্ষ 'r':
          mode |= MODE_R;
          ভঙ্গ;

        পক্ষ 'w':
          mode |= MODE_W;
          ভঙ্গ;

        পক্ষ 'm':
          mode |= MODE_M;
          ভঙ্গ;

        স্বভাব:
          প্রত্যাগম crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  প্রত্যাগম mode;
}

/* check if the type b is included in type a.  */
স্থির bool
has_same_type (ধ্রুব বর্ণ *a, ধ্রুব বর্ণ *b)
{
  bool a_all = a == রিক্ত || strcmp (a, "a") == 0;
  যদি (b == রিক্ত || strcmp (b, "a") == 0)
    প্রত্যাগম a_all;

  প্রত্যাগম a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
স্থির পূর্ণাঙ্ক
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  পূর্ণাঙ্ক req_mode;

#  define CMP_DEV_অঙ্ক(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  যদি (UNLIKELY (req_mode < 0))
    প্রত্যাগম req_mode;

  প্রতি (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      পূর্ণাঙ্ক mode;

      যদি (! CMP_DEV_অঙ্ক (devices[n]->minor, devices[current_rule]->minor))
        অনুবর্ত;
      যদি (! CMP_DEV_অঙ্ক (devices[n]->major, devices[current_rule]->major))
        অনুবর্ত;

      যদি (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        অনুবর্ত;

      mode = access_to_mode (devices[i]->access, err);
      যদি (UNLIKELY (mode < 0))
        প্রত্যাগম mode;

      যদি (devices[current_rule]->allow && (mode & req_mode))
        প্রত্যাগম 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      যদি (req_mode == 0)
        প্রত্যাগম 1;
    }

  প্রত্যাগম 0;
#  undef CMP_DEV_অঙ্ক
}

স্থির size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  যদি (n == 0)
    প্রত্যাগম 1;

  /* Find the first rule that is after the last "block all".  */
  প্রতি (i = n - 1; i-- > 0;)
    {
      যদি ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        প্রত্যাগম i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  প্রতি (i = 0; i < n; i++)
    যদি (devices[i]->allow)
      প্রত্যাগম i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  প্রত্যাগম n + 1;
}

স্থির bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  পূর্ণাঙ্ক remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  প্রতি (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      যদি (! devices[current_rule]->allow)
        প্রত্যাগম false;

      যদি (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          যদি (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            প্রত্যাগম true;

          যদি (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          অন্যথা যদি (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          যদি (remaining == 0)
            প্রত্যাগম true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  প্রত্যাগম false;
}

স্থির পূর্ণাঙ্ক
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  সংরচনা default_dev_s *default_devices = get_default_devices ();
  পূর্ণাঙ্ক ret, sd_err;
  size_t i;

  যদি (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      প্রত্যাগম 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  যদি (UNLIKELY (sd_err < 0))
    প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  প্রতি (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  প্রতি (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      বর্ণ type;

      যদি (! d->allow)
        {
          পূর্ণাঙ্ক redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          যদি (UNLIKELY (redundant < 0))
            প্রত্যাগম redundant;

          যদি (redundant)
            অনুবর্ত;

          প্রত্যাগম crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      যদি (d->type == রিক্ত || strcmp (d->type, "a") == 0)
        type = 'a';
      অন্যথা যদি (strcmp (d->type, "c") == 0)
        type = 'c';
      অন্যথা যদি (strcmp (d->type, "b") == 0)
        type = 'b';
      অন্যথা
        প্রত্যাগম crun_make_error (err, 0, "unknown device type `%s`", d->type);

      যদি (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }
      অন্যথা
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }
    }

  প্রত্যাগম 0;
}

স্থির bool
has_bpf_fs ()
{
  সংরচনা statfs stat;

  প্রত্যাগম statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

স্থির পূর্ণাঙ্ক
bpfprog_path_from_scope (বর্ণ **path, ধ্রুব বর্ণ *scope, libcrun_error_t *err)
{
  cleanup_free বর্ণ *flat_scope = xstrdup (scope);
  বর্ণ *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  যাবত্ ((it = strchr (it, '.')) != রিক্ত)
    *it = '_';

  প্রত্যাগম append_paths (path, err, CRUN_BPF_DIR, flat_scope, রিক্ত);
}

স্থির পূর্ণাঙ্ক
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 বর্ণ **missing_properties,
                 ধ্রুব বর্ণ *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret = 0;
  পূর্ণাঙ্ক in_userns, sd_err;
  ধ্রুব বর্ণ *property_name = "BPFProgram";
  cleanup_free সংরচনা bpf_program *program = রিক্ত;
  cleanup_free বর্ণ *path = রিক্ত;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  যদি (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    প্রত্যাগম 0;

  in_userns = check_running_in_user_namespace (err);
  যদি (UNLIKELY (in_userns < 0))
    প্রত্যাগম in_userns;

  যদি (in_userns)
    প্রত্যাগম 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  যদি (UNLIKELY (program == রিক্ত))
    {
      crun_error_release (err);
      প্রত্যাগম 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  যদি (UNLIKELY (ret < 0 && errno != EEXIST))
    প্রত্যাগম crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  যদি (is_update)
    {
      cleanup_free সংরচনা bpf_program *program_loaded = রিক্ত;

      যদি (resources->devices == রিক্ত || resources->devices_len == 0)
        {
          *devices_set = true;
          প্রত্যাগম 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      /* it is the same as the loaded program.  */
      যদি (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          প্রত্যাগম 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free সংরচনা bpf_program *program_new = রিক্ত;
        বর্ণ tmp[64];
        পূর্ণাঙ্ক len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, পরিমাণ (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (tmp)))
          প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        যদি (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            প্রত্যাগম 0;
          }
      }

      প্রত্যাগম crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  যদি (UNLIKELY (sd_err < 0))
    প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  প্রত্যাগম 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
স্থির পূর্ণাঙ্ক
append_cpu_quota (sd_bus_message *m, বর্ণ **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ধ্রুব বর্ণ *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  পূর্ণাঙ্ক sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  যদি (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      পূর্ণাঙ্ক ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      যদি (has_period)
        period = max_period;
    }

  যদি (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  যদি ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      যদি (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          যদি (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      যদি (UNLIKELY (sd_err < 0))
        প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
append_resources (sd_bus_message *m,
                  bool is_update,
                  ধ্রুব বর্ণ *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  পূর্ণাঙ্ক cgroup_mode,
                  ধ্রুব বর্ণ *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t মান;
  পূর্ণাঙ্ক sd_err;
  পূর্ণাঙ্ক ret;
  cleanup_free বর্ণ *dir = রিক্ত;
  cleanup_free বর্ণ **missing_properties = রিক্ত;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  missing_properties = read_dir_entries (dir, err);
  যদি (UNLIKELY (missing_properties == রিক্ত))
    {
      যদি (crun_error_get_errno (err) != ENOENT)
        প্রত্যাগম -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_মান(name, মান)                                             \
    কুরু                                                                                 \
      {                                                                                \
        যদি (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", মান);              \
            যদি (UNLIKELY (sd_err < 0))                                                 \
              প্রত্যাগম crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } যাবত্ (0)

#  define APPEND_UINT64(name, fn)              \
    কুরু                                         \
      {                                        \
        ret = fn (resources, &মান, err);     \
        যদি (UNLIKELY (ret < 0))                \
          প্রত্যাগম ret;                          \
        যদি (ret)                               \
          {                                    \
            APPEND_UINT64_মান (name, মান); \
          }                                    \
    } যাবত্ (0)

  যদি (resources == রিক্ত)
    প্রত্যাগম 0;

  APPEND_UINT64 ("MemoryLow", get_memory_নিচের);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  বিকল্প (cgroup_mode)
    {
    পক্ষ CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;

        যদি (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ধ্রুব বর্ণ *property_name = "AllowedCPUs";
            ধ্রুব বর্ণ *মান = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free বর্ণ *allowed_cpus = রিক্ত;
            size_t allowed_cpus_len = 0;

            যদি (মান == রিক্ত)
              মান = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (মান, &allowed_cpus, &allowed_cpus_len, err);
            যদি (UNLIKELY (ret < 0))
              প্রত্যাগম ret;

            যদি (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                যদি (UNLIKELY (ret < 0))
                  প্রত্যাগম ret;
              }
          }

        যদি (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ধ্রুব বর্ণ *property_name = "AllowedMemoryNodes";
            ধ্রুব বর্ণ *মান = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free বর্ণ *allowed_mems = রিক্ত;
            size_t allowed_mems_len = 0;

            যদি (মান == রিক্ত)
              মান = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (মান, &allowed_mems, &allowed_mems_len, err);
            যদি (UNLIKELY (ret < 0))
              প্রত্যাগম ret;

            যদি (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                যদি (UNLIKELY (ret < 0))
                  প্রত্যাগম ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        যদি (UNLIKELY (ret < 0))
          প্রত্যাগম ret;
      }
      ভঙ্গ;

    পক্ষ CGROUP_MODE_LEGACY:
    পক্ষ CGROUP_MODE_HYBRID:
      যদি (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_মান ("CPUShares", resources->cpu->shares);
      ভঙ্গ;

    স্বভাব:
      প্রত্যাগম crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_মান

  যদি (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
reset_failed_unit (sd_bus *bus, ধ্রুব বর্ণ *unit)
{
  পূর্ণাঙ্ক sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = রিক্ত, *reply = রিক্ত;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  যদি (UNLIKELY (sd_err < 0))
    গচ্ছ exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  যদি (UNLIKELY (sd_err < 0))
    গচ্ছ exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  যদি (UNLIKELY (sd_err < 0))
    গচ্ছ exit;

  sd_err = 0;

exit:
  যদি (m)
    sd_bus_message_unref (m);
  যদি (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  প্রত্যাগম sd_err;
}

স্থির পূর্ণাঙ্ক
verify_ebpf_device_filter_installed (ধ্রুব বর্ণ *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = রিক্ত;
  cleanup_free বর্ণ *full_path = রিক্ত;
  size_t n_progs = 0;
  পূর্ণাঙ্ক ret;

  ret = append_paths (&full_path, err, CGROUP_মূল, cgroup_path, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (n_progs == 0)
    প্রত্যাগম crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  প্রত্যাগম 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
স্থির পূর্ণাঙ্ক
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, পূর্ণাঙ্ক ret)
{
  যদি (bus)
    sd_bus_unref (bus);
  যদি (m)
    sd_bus_message_unref (m);
  যদি (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            পূর্ণাঙ্ক cgroup_mode,
                            string_map *annotations,
                            ধ্রুব বর্ণ *state_মূল,
                            ধ্রুব বর্ণ *scope, ধ্রুব বর্ণ *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = রিক্ত;
  sd_bus_message *m = রিক্ত;
  sd_bus_message *reply = রিক্ত;
  পূর্ণাঙ্ক sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ধ্রুব বর্ণ *object = রিক্ত;
  সংরচনা systemd_job_removed_s job_data = {};
  পূর্ণাঙ্ক i;
  ধ্রুব বর্ণ *boolean_opts[10];
  cleanup_free বর্ণ *state_dir = রিক্ত;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_মূল, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = রিক্ত;

  ret = open_sd_bus_connection (&bus, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      গচ্ছ exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      গচ্ছ exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      গচ্ছ exit;
    }

  যদি (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      যদি (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          গচ্ছ exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      গচ্ছ exit;
    }

  যদি (annotations)
    {
      size_t prefix_len = পরিমাণ (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      প্রতি (i = 0; i < annotations_len; i++)
        {
          ধ্রুব বর্ণ *চাবি;
          ধ্রুব বর্ণ *মান;
          size_t len;

          ret = string_map_get_at (annotations, i, &চাবি, &মান);
          যদি (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              গচ্ছ exit;
            }

          যদি (! has_prefix (চাবি, SYSTEMD_PROPERTY_PREFIX))
            অনুবর্ত;

          len = strlen (চাবি);
          যদি (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", চাবি);
              গচ্ছ exit;
            }

          ret = append_systemd_annotation (m, চাবি + prefix_len, len - prefix_len,
                                           মান, err);
          যদি (UNLIKELY (ret < 0))
            গচ্ছ exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      গচ্ছ exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      গচ্ছ exit;
    }

  প্রতি (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      যদি (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          গচ্ছ exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ exit;

  sd_err = sd_bus_message_close_container (m);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      গচ্ছ exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      গচ্ছ exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  যদি (UNLIKELY (sd_err < 0))
    {
      যদি (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          যদি (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = রিক্ত;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      যদি (sd_err < 0)
        {
          যদি (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              যদি (UNLIKELY (ret < 0))
                গচ্ছ exit;
              যদি (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          গচ্ছ exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      গচ্ছ exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  প্রত্যাগম cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

স্থির পূর্ণাঙ্ক
libcrun_destroy_systemd_cgroup_scope (সংরচনা libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = রিক্ত;
  sd_bus_message *m = রিক্ত;
  sd_bus_message *reply = রিক্ত;
  পূর্ণাঙ্ক ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ধ্রুব বর্ণ *object;
  ধ্রুব বর্ণ *scope = cgroup_status->scope;
  সংরচনা systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      গচ্ছ exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      গচ্ছ exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      গচ্ছ exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      গচ্ছ exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  প্রত্যাগম cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

স্থির ধ্রুব বর্ণ *
find_systemd_subgroup (string_map *annotations)
{
  ধ্রুব বর্ণ *annotation;

  annotation = find_string_map_মান (annotations, "run.oci.systemd.subgroup");
  যদি (annotation)
    {
      যদি (annotation[0] == '\0')
        প্রত্যাগম রিক্ত;
      প্রত্যাগম annotation;
    }

  প্রত্যাগম "container";
}

স্থির পূর্ণাঙ্ক
libcrun_cgroup_enter_systemd (সংরচনা libcrun_cgroup_args *args,
                              সংরচনা libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  ধ্রুব বর্ণ *cgroup_path = args->cgroup_path;
  cleanup_free বর্ণ *scope_path = রিক্ত;
  cleanup_free বর্ণ *scope = রিক্ত;
  cleanup_free বর্ণ *path = রিক্ত;
  cleanup_free বর্ণ *slice = রিক্ত;
  পূর্ণাঙ্ক retries_বাম = 32;
  ধ্রুব বর্ণ *suffix;
  ধ্রুব বর্ণ *id = args->id;
  pid_t pid = args->pid;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক rootless = 0;
  পূর্ণাঙ্ক ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      যদি (UNLIKELY (rootless < 0))
        প্রত্যাগম rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  প্রতি (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_মূল,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      যদি (LIKELY (ret >= 0))
        ভঙ্গ;

      যদি (can_retry && retries_বাম-- > 0)
        {
          crun_error_release (err);
          অনুবর্ত;
        }

      প্রত্যাগম ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  যদি (is_empty_string (suffix))
    scope_path = xstrdup (path);
  অন্যথা
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      যদি (UNLIKELY (scope_path == রিক্ত))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  যদি (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  out->path = path;
  path = রিক্ত;

  out->scope = scope;
  scope = রিক্ত;
  প্রত্যাগম 0;
}

বর্ণ *
get_cgroup_scope_path (ধ্রুব বর্ণ *cgroup_path, ধ্রুব বর্ণ *scope)
{
  বর্ণ *path_to_scope = রিক্ত;
  বর্ণ *cur;

  যদি (is_empty_string (cgroup_path))
    প্রত্যাগম রিক্ত;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  যাবত্ (cur)
    {
      বর্ণ *next = strchr (cur + 1, '/');
      যদি (next == রিক্ত)
        ভঙ্গ;

      *next = '\0';
      যদি (strcmp (cur, scope) == 0)
        প্রত্যাগম path_to_scope;
      *next = '/';

      cur = next;
      যাবত্ (*cur == '/')
        cur++;
    }

  প্রত্যাগম path_to_scope;
}

স্থির পূর্ণাঙ্ক
libcrun_destroy_cgroup_systemd (সংরচনা libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free বর্ণ *path_to_scope = রিক্ত;
  cleanup_free বর্ণ *bpfprog = রিক্ত;
  পূর্ণাঙ্ক mode;
  পূর্ণাঙ্ক ret;

  mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (mode < 0))
    প্রত্যাগম mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  যদি (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  যদি (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  যদি (UNLIKELY (ret < 0))
    crun_error_release (err);
  অন্যথা
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  প্রত্যাগম destroy_cgroup_path (path_to_scope, mode, err);
}

স্থির পূর্ণাঙ্ক
libcrun_update_resources_systemd (সংরচনা libcrun_cgroup_status *cgroup_status,
                                  ধ্রুব বর্ণ *state_মূল,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  সংরচনা systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free বর্ণ *state_dir = রিক্ত;
  sd_bus_message *reply = রিক্ত;
  sd_bus_message *m = রিক্ত;
  sd_bus *bus = রিক্ত;
  পূর্ণাঙ্ক sd_err, ret;
  পূর্ণাঙ্ক cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_মূল, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    প্রত্যাগম cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      গচ্ছ exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      গচ্ছ exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      গচ্ছ exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ exit;

  sd_err = sd_bus_message_close_container (m);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      গচ্ছ exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      গচ্ছ exit;
    }

  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ exit;

  ret = 0;

exit:
  প্রত্যাগম cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
স্থির পূর্ণাঙ্ক
libcrun_attach_process_systemd (সংরচনা libcrun_cgroup_status *cgroup_status,
                                ধ্রুব বর্ণ *path, pid_t pid,
                                libcrun_error_t *err)
{
  ধ্রুব বর্ণ *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free বর্ণ *scope_path = রিক্ত;
  sd_bus_message *reply = রিক্ত;
  ধ্রুব বর্ণ *subcgroup;
  ধ্রুব বর্ণ *last;
  sd_bus *bus = রিক্ত;
  size_t len;
  পূর্ণাঙ্ক sd_err, ret;

  যদি (is_empty_string (scope))
    প্রত্যাগম crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  যদি (scope_path == রিক্ত)
    প্রত্যাগম crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  যদি (last == রিক্ত || strcmp (last + 1, scope) != 0)
    প্রত্যাগম crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  যদি (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  প্রত্যাগম cleanup_sd_bus_and_return (bus, রিক্ত, reply, &error, ret);
}

#অন্যথা
স্থির পূর্ণাঙ্ক
libcrun_cgroup_enter_systemd (সংরচনা libcrun_cgroup_args *args,
                              সংরচনা libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (শূন্য) args;
  (শূন্য) out;

  প্রত্যাগম crun_make_error (err, ENOTSUP, "systemd not supported");
}

স্থির পূর্ণাঙ্ক
libcrun_destroy_cgroup_systemd (সংরচনা libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (শূন্য) cgroup_status;

  প্রত্যাগম crun_make_error (err, ENOTSUP, "systemd not supported");
}

স্থির পূর্ণাঙ্ক
libcrun_update_resources_systemd (সংরচনা libcrun_cgroup_status *cgroup_status,
                                  ধ্রুব বর্ণ *state_মূল,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (শূন্য) cgroup_status;
  (শূন্য) state_মূল;
  (শূন্য) resources;

  প্রত্যাগম crun_make_error (err, ENOTSUP, "systemd not supported");
}

স্থির পূর্ণাঙ্ক
libcrun_attach_process_systemd (সংরচনা libcrun_cgroup_status *cgroup_status,
                                ধ্রুব বর্ণ *path, pid_t pid,
                                libcrun_error_t *err)
{
  (শূন্য) cgroup_status;
  (শূন্য) path;
  (শূন্য) pid;

  প্রত্যাগম crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

সংরচনা libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = রিক্ত,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
