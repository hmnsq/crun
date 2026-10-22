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

স্থির পূর্ণসংখ্যা
register_missing_property_from_message (ধ্রুবক অক্ষর *state_dir, ধ্রুবক অক্ষর *message, libcrun_error_t *err)
{
  cleanup_free অক্ষর *file_path = ফাঁকা;
  cleanup_free অক্ষর *dir_path = ফাঁকা;
  cleanup_free অক্ষর *property = ফাঁকা;
  অক্ষর *p;
  পূর্ণসংখ্যা ret;

  যদি (! has_prefix (message, "Cannot set property "))
    ফেরত 0;

  property = xstrdup (message + আকার ("Cannot set property ") - 1);
  p = strchr (property, ',');
  যদি (! p)
    ফেরত 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = append_paths (&file_path, err, dir_path, property, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, ফাঁকা, 0, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত 1;
}

স্থির bool
property_missing_p (অক্ষর **missing_properties, ধ্রুবক অক্ষর *property)
{
  size_t i;

  জন্য (i = 0; missing_properties && missing_properties[i]; i++)
    যদি (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        ফেরত true;
      }

  ফেরত false;
}

স্থির শূন্য
get_systemd_scope_and_slice (ধ্রুবক অক্ষর *id, bool user_slice, ধ্রুবক অক্ষর *cgroup_path,
                             অক্ষর **scope, অক্ষর **slice)
{
  ধ্রুবক অক্ষর *n;
  অক্ষর *p;

  যদি (cgroup_path == ফাঁকা || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      ফেরত;
    }

  n = strchr (cgroup_path, ':');
  যদি (n == ফাঁকা)
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
স্থির পূর্ণসংখ্যা
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
  cleanup_free অক্ষর *cgroup_path = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা dirfd = -1;
  bool need_set_parent = true;
  অক্ষর fmt_buf[64];
  পূর্ণসংখ্যা len;
  পূর্ণসংখ্যা ret;

  যদি (resources == ফাঁকা || resources->cpu == ফাঁকা)
    ফেরত 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  যদি (is_empty_string (path))
    ফেরত crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  যদি (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_মূল, "cpu", path, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  যদি (UNLIKELY (dirfd < 0))
    ফেরত crun_make_error (err, errno, "open `%s`", cgroup_path);

  যদি (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, আকার (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      যদি (UNLIKELY (len >= (পূর্ণসংখ্যা) আকার (fmt_buf)))
        ফেরত crun_make_error (err, 0, "internal error: static buffer too small");

      যদি (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  যদি (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, আকার (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      যদি (UNLIKELY (len >= (পূর্ণসংখ্যা) আকার (fmt_buf)))
        ফেরত crun_make_error (err, 0, "internal error: static buffer too small");

      যদি (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
  cleanup_free অক্ষর *cgroup_path = ফাঁকা;
  পূর্ণসংখ্যা parent;
  পূর্ণসংখ্যা ret;

  যদি (resources == ফাঁকা || resources->cpu == ফাঁকা)
    ফেরত 0;

  যদি (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    ফেরত 0;

  জন্য (parent = 0; parent < 2; parent++)
    {
      cleanup_close পূর্ণসংখ্যা dirfd = -1;

      যদি (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_মূল, path ? path : "", (parent ? ".." : ফাঁকা), ফাঁকা);
      অন্যথা
        ret = append_paths (&cgroup_path, err, CGROUP_মূল, "/cpu", path ? path : "", (parent ? ".." : ফাঁকা), ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (dirfd < 0))
        ফেরত crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      যদি (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
        }
    }

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
  cleanup_free অক্ষর *cgroup_path = ফাঁকা;
  পূর্ণসংখ্যা parent;
  পূর্ণসংখ্যা ret;
  পূর্ণসংখ্যা levels;

  ret = append_paths (&cgroup_path, err, CGROUP_মূল, "/cpuset", path ? path : "", ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  জন্য (parent = 0; parent < levels; parent++)
    {
      cleanup_close পূর্ণসংখ্যা dirfd_cpuset = -1;
      cleanup_free অক্ষর *path_to_cpuset = ফাঁকা;

      ret = append_paths (&path_to_cpuset, err, CGROUP_মূল "/cpuset", path, (parent ? ".." : ফাঁকা), ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (dirfd_cpuset < 0))
        ফেরত crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  ফেরত 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
স্থির পূর্ণসংখ্যা
compute_finalized_path (অক্ষর **path, ধ্রুবক অক্ষর *from, ধ্রুবক অক্ষর *suffix, libcrun_error_t *err)
{
  যদি (suffix == ফাঁকা)
    {
      *path = xstrdup (from);
      ফেরত 0;
    }
  ফেরত append_paths (path, err, from, suffix, ফাঁকা);
}

স্থির পূর্ণসংখ্যা
systemd_finalize (কাঠামো libcrun_cgroup_args *args, অক্ষর **path_out,
                  পূর্ণসংখ্যা cgroup_mode, ধ্রুবক অক্ষর *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free অক্ষর *content = ফাঁকা;
  cleanup_free অক্ষর *path = ফাঁকা;
  pid_t pid = args->pid;
  পূর্ণসংখ্যা ret;
  অক্ষর *from, *to;
  অক্ষর *saveptr = ফাঁকা;

  cleanup_free অক্ষর *proc_path = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  যদি (UNLIKELY (fd < 0))
    ফেরত fd;

  ret = read_all_fd (fd, "cgroup path", &content, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  নির্বাচন (cgroup_mode)
    {
    ক্ষেত্র CGROUP_MODE_HYBRID:
    ক্ষেত্র CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      যদি (LIKELY (from != ফাঁকা))
        from += 8;
      অন্যথা
        {
          from = strstr (content, ":pids:");
          যদি (UNLIKELY (from == ফাঁকা))
            ফেরত crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      যদি (UNLIKELY (to == ফাঁকা))
        ফেরত crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      যদি (geteuid ())
        ফেরত 0;

      ret = setup_rt_runtime (resources, path, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      জন্য (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ফাঁকা, "\n", &saveptr))
        {
          অক্ষর *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          যদি (subsystem[0] == '\0')
            {
              যদি (cgroup_mode == CGROUP_MODE_LEGACY)
                চালু;

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
                      চালু;
                    }
                  ফেরত ret;
                }
            }
        }

      বিরতি;

    ক্ষেত্র CGROUP_MODE_UNIFIED:
      {
        cleanup_free অক্ষর *dir = ফাঁকা;

        from = strstr (content, "0::");
        যদি (UNLIKELY (from == ফাঁকা))
          ফেরত crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        যদি (UNLIKELY (to == ফাঁকা))
          ফেরত crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        ret = append_paths (&dir, err, CGROUP_মূল, path, ফাঁকা);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        ret = move_process_to_cgroup (pid, ফাঁকা, path, err);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        ret = enable_controllers (path, err);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        যদি (suffix)
          {
            ret = chown_cgroups (path, args->মূল_uid, args->মূল_gid, err);
            যদি (UNLIKELY (ret < 0))
              ফেরত ret;
          }
      }
      বিরতি;

    পূর্বনির্ধারিত:
      ফেরত crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  *path_out = path;
  path = ফাঁকা;

  ফেরত 0;
}

কাঠামো systemd_job_removed_s
{
  ধ্রুবক অক্ষর *path;
  ধ্রুবক অক্ষর *op;
  পূর্ণসংখ্যা terminated;
  libcrun_error_t err;
};

স্থির পূর্ণসংখ্যা
systemd_job_removed (sd_bus_message *m, শূন্য *userdata, sd_bus_error *error arg_unused)
{
  ধ্রুবক অক্ষর *path, *unit, *ফলাফল;
  uint32_t id;
  পূর্ণসংখ্যা ret;
  কাঠামো systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &ফলাফল);
  যদি (ret < 0)
    ফেরত -1;

  যদি (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      যদি (strcmp (ফলাফল, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, ফলাফল);
          ফেরত -1;
        }
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
systemd_check_job_status_setup (sd_bus *bus, কাঠামো systemd_job_removed_s *তথ্য, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  ret = sd_bus_match_signal_async (bus, ফাঁকা, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, ফাঁকা, তথ্য);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, -ret, "sd-bus match signal");

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
systemd_check_job_status (sd_bus *bus, কাঠামো systemd_job_removed_s *তথ্য, ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *op,
                          libcrun_error_t *err)
{
  পূর্ণসংখ্যা sd_err;

  তথ্য->path = path;
  তথ্য->op = op;
  যখন (! তথ্য->terminated)
    {
      sd_err = sd_bus_process (bus, ফাঁকা);
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus process");

      যদি (sd_err != 0)
        চালু;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus wait");
    }

  যদি (তথ্য->err != ফাঁকা)
    {
      *err = তথ্য->err;
      ফেরত -1;
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
parse_sd_array (অক্ষর *s, অক্ষর **out, অক্ষর **পরবর্তী, libcrun_error_t *err)
{
  অক্ষর endchr;
  অক্ষর *it, *dest;
  bool escaped = false;

  *out = ফাঁকা;
  *পরবর্তী = ফাঁকা;

  যখন (isspace (*s))
    s++;
  যদি (*s == '\0')
    ফেরত 0;
  অন্যথা যদি (*s != '\'' && *s != '"')
    ফেরত crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  যখন (1)
    {
      যদি (*it == '\0')
        ফেরত crun_make_error (err, 0, "invalid string `%s`", s);
      যদি (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          যখন (isspace (*it))
            it++;
          যদি (*it == ',')
            {
              *পরবর্তী = ++it;
              *dest = '\0';
              ফেরত 0;
            }

          যদি (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              ফেরত 0;
            }

          ফেরত crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      যদি (! escaped)
        *dest++ = *it;
      it++;
    }

  ফেরত 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
স্থির পূর্ণসংখ্যা
append_systemd_annotation (sd_bus_message *m, ধ্রুবক অক্ষর *name, size_t name_len, ধ্রুবক অক্ষর *মান,
                           libcrun_error_t *err)
{
  cleanup_free অক্ষর *tmp_name = ফাঁকা;
  uint32_t factor = 1;
  ধ্রুবক অক্ষর *it;
  পূর্ণসংখ্যা sd_err;

  যখন (*মান == ' ')
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
      স্থির ধ্রুবক অক্ষর *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", ফাঁকা };
      ধ্রুবক অক্ষর *অঙ্ক = it;
      অক্ষর *endptr = ফাঁকা;
      দ্বিগুণ v;
      size_t i;

      জন্য (i = 0; types[i]; i++)
        যদি (has_prefix (অঙ্ক, types[i]))
          {
            অঙ্ক += strlen (types[i]);
            বিরতি;
          }

      errno = 0;
      v = strtod (অঙ্ক, &endptr);
      যদি (UNLIKELY (errno != 0 || endptr == অঙ্ক || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        ফেরত crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, মান);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ফেরত 0;
    }

  যদি ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ফেরত 0;
    }
  অন্যথা যদি (*it == '\'')
    {
      cleanup_free অক্ষর *v_start = ফাঁকা;
      অক্ষর *end;

      it = v_start = xstrdup (মান);

      end = strchr (v_start + 1, '\'');
      যদি (end == ফাঁকা)
        ফেরত crun_make_error (err, 0, "invalid variant `%s`", মান);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ফেরত 0;
    }
  অন্যথা যদি (*it == '[')
    {
      cleanup_free অক্ষর *v_start = ফাঁকা;
      size_t n_parts = 0, parts_আকার = 32;
      cleanup_free অক্ষর **parts = xmalloc (আকার (অক্ষর *) * parts_আকার);
      অক্ষর *part;

      part = v_start = xstrdup (it + 1);
      যখন (1)
        {
          অক্ষর *out = ফাঁকা, *পরবর্তী = ফাঁকা;
          পূর্ণসংখ্যা ret;

          ret = parse_sd_array (part, &out, &পরবর্তী, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;

          parts[n_parts++] = out;
          যদি (n_parts == parts_আকার - 1)
            {
              parts_আকার += 32;
              parts = xrealloc (parts, আকার (অক্ষর *) * parts_আকার);
            }
          parts[n_parts] = ফাঁকা;
          যদি (পরবর্তী == ফাঁকা)
            বিরতি;

          part = পরবর্তী;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus close container");

      ফেরত 0;
    }
  অন্যথা যদি (has_prefix (it, "uint64 "))
    {
      অক্ষর *endptr = ফাঁকা;
      uint64_t v;

      errno = 0;
      v = strtoull (it + আকার ("uint64"), &endptr, 10);
      যদি (UNLIKELY (errno != 0 || *endptr))
        ফেরত crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ফেরত 0;
    }
  অন্যথা যদি (has_prefix (it, "int64 "))
    {
      অক্ষর *endptr = ফাঁকা;
      int64_t v;

      errno = 0;
      v = strtoll (it + আকার ("int64"), &endptr, 10);
      যদি (UNLIKELY (errno != 0 || *endptr))
        ফেরত crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ফেরত 0;
    }
  অন্যথা যদি (has_prefix (it, "uint32 "))
    {
      অক্ষর *endptr = ফাঁকা;
      uint32_t v;

      errno = 0;
      v = strtoul (it + আকার ("uint32"), &endptr, 10);
      যদি (UNLIKELY (errno != 0 || *endptr))
        ফেরত crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ফেরত 0;
    }
  অন্যথা যদি (has_prefix (it, "int32 ") || strchr (it, ' ') == ফাঁকা)
    {
      অক্ষর *endptr = ফাঁকা;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      যদি (has_prefix (it, "int32 "))
        v = strtol (it + আকার ("int32"), &endptr, 10);
      অন্যথা
        v = strtol (it, &endptr, 10);
      যদি (UNLIKELY (errno != 0 || *endptr))
        ফেরত crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ফেরত 0;
    }

  ফেরত crun_make_error (err, 0, "unknown type for `%s`", name);
}

স্থির পূর্ণসংখ্যা
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  পূর্ণসংখ্যা rootless;
  পূর্ণসংখ্যা sd_err = 0;

  rootless = is_rootless (err);
  যদি (UNLIKELY (rootless < 0))
    ফেরত rootless;

  যদি (rootless)
    sd_err = sd_bus_default_user (bus);
  যদি (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  যদি (sd_err < 0)
    ফেরত crun_make_error (err, -sd_err, "cannot open sd-bus");

  ফেরত 0;
}

স্থির ধ্রুবক অক্ষর *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ধ্রুবক অক্ষর *name)
{
  size_t i;

  যদি (resources == ফাঁকা || resources->unified == ফাঁকা)
    ফেরত ফাঁকা;

  জন্য (i = 0; i < resources->unified->len; i++)
    যদি (strcmp (resources->unified->চাবিগুলি[i], name) == 0)
      ফেরত resources->unified->মানসমূহ[i];
  ফেরত ফাঁকা;
}

স্থির পূর্ণসংখ্যা
get_মান_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ধ্রুবক অক্ষর *name,
                            uint64_t *মান, libcrun_error_t *err)
{
  অক্ষর *endptr = ফাঁকা;
  size_t i;

  যদি (resources == ফাঁকা || resources->unified == ফাঁকা)
    ফেরত 0;

  জন্য (i = 0; i < resources->unified->len; i++)
    যদি (strcmp (resources->unified->চাবিগুলি[i], name) == 0)
      {
        যদি (strcmp (resources->unified->মানসমূহ[i], "max") == 0)
          {
            *মান = UINT64_MAX;
            ফেরত 1;
          }

        errno = 0;
        *মান = strtoull (resources->unified->মানসমূহ[i], &endptr, 10);
        যদি (UNLIKELY (errno))
          ফেরত crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->মানসমূহ[i]);
        যদি (endptr && *endptr)
          ফেরত crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->মানসমূহ[i]);
        ফেরত 1;
      }
  ফেরত 0;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
get_memory_নিচের (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  যদি (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      ফেরত 1;
    }

  ফেরত get_মান_from_unified_map (resources, "memory.low", limit, err);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  যদি (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      ফেরত 1;
    }

  ফেরত get_মান_from_unified_map (resources, "pids.max", limit, err);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  যদি (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      ফেরত 1;
    }

  ফেরত get_মান_from_unified_map (resources, "memory.max", limit, err);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
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
          ফেরত 1;
        }
      /* 0 means unset.  */
      যদি (memory->swap == 0)
        ফেরত 0;
      যদি (mem <= 0)
        ফেরত crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      যদি (memory->swap < mem)
        ফেরত crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      ফেরত 1;
    }

  ফেরত get_মান_from_unified_map (resources, "memory.swap.max", limit, err);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t মান;
  পূর্ণসংখ্যা ret;

  ret = get_মান_from_unified_map (resources, "cpu.idle", &মান, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;
  যদি (ret > 0 && মান == 1)
    has_idle = true;
  যদি (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_মান_from_unified_map (resources, "cpu.weight", weight, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;
  যদি (ret > 0)
    {
      যদি (has_idle)
        ফেরত crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      ফেরত 1;
    }

  যদি (resources->cpu && resources->cpu->shares_present)
    {
      যদি (has_idle)
        ফেরত crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      যদি (resources->cpu->shares == 0)
        ফেরত 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      ফেরত 1;
    }
  যদি (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      ফেরত 1;
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
read_uint64_মান (ধ্রুবক অক্ষর *str, uint64_t *মান, libcrun_error_t *err)
{
  অক্ষর *endptr = ফাঁকা;

  errno = 0;
  *মান = strtoull (str, &endptr, 10);
  যদি (UNLIKELY (errno))
    ফেরত crun_make_error (err, errno, "invalid value: %s", str);
  যদি (endptr && *endptr)
    ফেরত crun_make_error (err, 0, "invalid value: %s", str);
  ফেরত 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
স্থির অন্তঃরেখ uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  ফেরত bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

স্থির পূর্ণসংখ্যা
append_io_weight (sd_bus_message *m, অক্ষর **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  পূর্ণসংখ্যা sd_err;
  size_t i;
  পূর্ণসংখ্যা ret;

#  define APPEND_IO_WEIGHT(মান)                                                      \
    করো                                                                                 \
      {                                                                                \
        যদি (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            যদি (UNLIKELY (sd_err < 0))                                                 \
              ফেরত crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } যখন (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, মান)                                                      \
    করো                                                                                                \
      {                                                                                               \
        যদি (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            যদি (UNLIKELY (sd_err < 0))                                                                \
              ফেরত crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } যখন (0)

  যদি (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  জন্য (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      অক্ষর name[64];
      পূর্ণসংখ্যা len;

      len = snprintf (name, আকার (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      যদি (UNLIKELY (len >= (পূর্ণসংখ্যা) আকার (name)))
        ফেরত crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  জন্য (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free অক্ষর *মান = ফাঁকা;
      bool need_conversion = false;
      অক্ষর *saveptr = ফাঁকা;
      অক্ষর *সারি;

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
          চালু;
        }

      মান = xstrdup (resources->unified->মানসমূহ[i]);

      জন্য (সারি = strtok_r (মান, "\n", &saveptr); সারি; সারি = strtok_r (ফাঁকা, "\n", &saveptr))
        {
          অক্ষর *sep;

          sep = strchr (সারি, ' ');
          যখন (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_মান (sep ? sep : সারি, &weight, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          যদি (need_conversion)
            weight = IO_WEIGHT (weight);

          যদি (sep == ফাঁকা || strcmp (সারি, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          অন্যথা
            APPEND_IO_DEVICE_WEIGHT (সারি, weight);
        }
    }

  ফেরত 0;
}

/* Adapted from systemd.  */
স্থির পূর্ণসংখ্যা
bus_append_byte_array (sd_bus_message *m, ধ্রুবক অক্ষর *field, ধ্রুবক শূন্য *buf, size_t n, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  যদি (ret < 0)
    ফেরত crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  যদি (ret < 0)
    ফেরত crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  যদি (ret < 0)
    ফেরত crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  যদি (ret < 0)
    ফেরত crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  যদি (ret < 0)
    ফেরত crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  যদি (ret < 0)
    ফেরত crun_make_error (err, -ret, "sd_bus_message_close_container");

  ফেরত 1;
}

স্থির পূর্ণসংখ্যা
append_uint64_from_unified_map (sd_bus_message *m,
                                অক্ষর **missing_properties,
                                ধ্রুবক অক্ষর *attr,
                                ধ্রুবক অক্ষর *চাবি,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t মান;
  পূর্ণসংখ্যা ret;

  যদি (property_missing_p (missing_properties, attr))
    ফেরত 0;

  ret = get_মান_from_unified_map (resources, চাবি, &মান, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;
  যদি (ret)
    {
      পূর্ণসংখ্যা sd_err = sd_bus_message_append (m, "(sv)", attr, "t", মান);
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  ফেরত 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
স্থির পূর্ণসংখ্যা
append_device_allow (sd_bus_message *m,
                     ধ্রুবক অক্ষর type,
                     পূর্ণসংখ্যা major,
                     পূর্ণসংখ্যা minor,
                     ধ্রুবক অক্ষর *access,
                     libcrun_error_t *err)
{
  অক্ষর device[64];
  পূর্ণসংখ্যা sd_err;
  পূর্ণসংখ্যা len;

  যদি (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      ফেরত 0;
    }

  যদি (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, আকার (device), "%s-*", type == 'c' ? "char" : "block");
  অন্যথা যদি (IS_WILDCARD (minor))
    len = snprintf (device, আকার (device), type == 'c' ? "char-%d" : "block-%d", major);
  অন্যথা
    len = snprintf (device, আকার (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  যদি (UNLIKELY (len >= (পূর্ণসংখ্যা) আকার (device)))
    ফেরত crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  যদি (UNLIKELY (sd_err < 0))
    ফেরত crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  ফেরত 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

স্থির পূর্ণসংখ্যা
access_to_mode (ধ্রুবক অক্ষর *access, libcrun_error_t *err)
{
  পূর্ণসংখ্যা mode = 0;

  যখন (access && *access)
    {
      পূর্ণসংখ্যা c = *access++;

      নির্বাচন (c)
        {
        ক্ষেত্র 'r':
          mode |= MODE_R;
          বিরতি;

        ক্ষেত্র 'w':
          mode |= MODE_W;
          বিরতি;

        ক্ষেত্র 'm':
          mode |= MODE_M;
          বিরতি;

        পূর্বনির্ধারিত:
          ফেরত crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  ফেরত mode;
}

/* check if the type b is included in type a.  */
স্থির bool
has_same_type (ধ্রুবক অক্ষর *a, ধ্রুবক অক্ষর *b)
{
  bool a_all = a == ফাঁকা || strcmp (a, "a") == 0;
  যদি (b == ফাঁকা || strcmp (b, "a") == 0)
    ফেরত a_all;

  ফেরত a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
স্থির পূর্ণসংখ্যা
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  পূর্ণসংখ্যা req_mode;

#  define CMP_DEV_অঙ্ক(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  যদি (UNLIKELY (req_mode < 0))
    ফেরত req_mode;

  জন্য (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      পূর্ণসংখ্যা mode;

      যদি (! CMP_DEV_অঙ্ক (devices[n]->minor, devices[current_rule]->minor))
        চালু;
      যদি (! CMP_DEV_অঙ্ক (devices[n]->major, devices[current_rule]->major))
        চালু;

      যদি (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        চালু;

      mode = access_to_mode (devices[i]->access, err);
      যদি (UNLIKELY (mode < 0))
        ফেরত mode;

      যদি (devices[current_rule]->allow && (mode & req_mode))
        ফেরত 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      যদি (req_mode == 0)
        ফেরত 1;
    }

  ফেরত 0;
#  undef CMP_DEV_অঙ্ক
}

স্থির size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  যদি (n == 0)
    ফেরত 1;

  /* Find the first rule that is after the last "block all".  */
  জন্য (i = n - 1; i-- > 0;)
    {
      যদি ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        ফেরত i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  জন্য (i = 0; i < n; i++)
    যদি (devices[i]->allow)
      ফেরত i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  ফেরত n + 1;
}

স্থির bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  পূর্ণসংখ্যা remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  জন্য (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      যদি (! devices[current_rule]->allow)
        ফেরত false;

      যদি (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          যদি (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            ফেরত true;

          যদি (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          অন্যথা যদি (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          যদি (remaining == 0)
            ফেরত true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  ফেরত false;
}

স্থির পূর্ণসংখ্যা
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  কাঠামো default_dev_s *default_devices = get_default_devices ();
  পূর্ণসংখ্যা ret, sd_err;
  size_t i;

  যদি (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      ফেরত 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  যদি (UNLIKELY (sd_err < 0))
    ফেরত crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  জন্য (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  জন্য (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      অক্ষর type;

      যদি (! d->allow)
        {
          পূর্ণসংখ্যা redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          যদি (UNLIKELY (redundant < 0))
            ফেরত redundant;

          যদি (redundant)
            চালু;

          ফেরত crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      যদি (d->type == ফাঁকা || strcmp (d->type, "a") == 0)
        type = 'a';
      অন্যথা যদি (strcmp (d->type, "c") == 0)
        type = 'c';
      অন্যথা যদি (strcmp (d->type, "b") == 0)
        type = 'b';
      অন্যথা
        ফেরত crun_make_error (err, 0, "unknown device type `%s`", d->type);

      যদি (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
        }
      অন্যথা
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
        }
    }

  ফেরত 0;
}

স্থির bool
has_bpf_fs ()
{
  কাঠামো statfs stat;

  ফেরত statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

স্থির পূর্ণসংখ্যা
bpfprog_path_from_scope (অক্ষর **path, ধ্রুবক অক্ষর *scope, libcrun_error_t *err)
{
  cleanup_free অক্ষর *flat_scope = xstrdup (scope);
  অক্ষর *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  যখন ((it = strchr (it, '.')) != ফাঁকা)
    *it = '_';

  ফেরত append_paths (path, err, CRUN_BPF_DIR, flat_scope, ফাঁকা);
}

স্থির পূর্ণসংখ্যা
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 অক্ষর **missing_properties,
                 ধ্রুবক অক্ষর *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret = 0;
  পূর্ণসংখ্যা in_userns, sd_err;
  ধ্রুবক অক্ষর *property_name = "BPFProgram";
  cleanup_free কাঠামো bpf_program *program = ফাঁকা;
  cleanup_free অক্ষর *path = ফাঁকা;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  যদি (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    ফেরত 0;

  in_userns = check_running_in_user_namespace (err);
  যদি (UNLIKELY (in_userns < 0))
    ফেরত in_userns;

  যদি (in_userns)
    ফেরত 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  যদি (UNLIKELY (program == ফাঁকা))
    {
      crun_error_release (err);
      ফেরত 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  যদি (UNLIKELY (ret < 0 && errno != EEXIST))
    ফেরত crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  যদি (is_update)
    {
      cleanup_free কাঠামো bpf_program *program_loaded = ফাঁকা;

      যদি (resources->devices == ফাঁকা || resources->devices_len == 0)
        {
          *devices_set = true;
          ফেরত 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      /* it is the same as the loaded program.  */
      যদি (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          ফেরত 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free কাঠামো bpf_program *program_new = ফাঁকা;
        অক্ষর tmp[64];
        পূর্ণসংখ্যা len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, আকার (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        যদি (UNLIKELY (len >= (পূর্ণসংখ্যা) আকার (tmp)))
          ফেরত crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        যদি (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            ফেরত 0;
          }
      }

      ফেরত crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  যদি (UNLIKELY (sd_err < 0))
    ফেরত crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  ফেরত 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
স্থির পূর্ণসংখ্যা
append_cpu_quota (sd_bus_message *m, অক্ষর **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ধ্রুবক অক্ষর *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  পূর্ণসংখ্যা sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  যদি (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      পূর্ণসংখ্যা ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
      যদি (has_period)
        period = max_period;
    }

  যদি (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      যদি (UNLIKELY (sd_err < 0))
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
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
        ফেরত crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
append_resources (sd_bus_message *m,
                  bool is_update,
                  ধ্রুবক অক্ষর *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  পূর্ণসংখ্যা cgroup_mode,
                  ধ্রুবক অক্ষর *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t মান;
  পূর্ণসংখ্যা sd_err;
  পূর্ণসংখ্যা ret;
  cleanup_free অক্ষর *dir = ফাঁকা;
  cleanup_free অক্ষর **missing_properties = ফাঁকা;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  missing_properties = read_dir_entries (dir, err);
  যদি (UNLIKELY (missing_properties == ফাঁকা))
    {
      যদি (crun_error_get_errno (err) != ENOENT)
        ফেরত -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_মান(name, মান)                                             \
    করো                                                                                 \
      {                                                                                \
        যদি (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", মান);              \
            যদি (UNLIKELY (sd_err < 0))                                                 \
              ফেরত crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } যখন (0)

#  define APPEND_UINT64(name, fn)              \
    করো                                         \
      {                                        \
        ret = fn (resources, &মান, err);     \
        যদি (UNLIKELY (ret < 0))                \
          ফেরত ret;                          \
        যদি (ret)                               \
          {                                    \
            APPEND_UINT64_মান (name, মান); \
          }                                    \
    } যখন (0)

  যদি (resources == ফাঁকা)
    ফেরত 0;

  APPEND_UINT64 ("MemoryLow", get_memory_নিচের);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  নির্বাচন (cgroup_mode)
    {
    ক্ষেত্র CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;

        যদি (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ধ্রুবক অক্ষর *property_name = "AllowedCPUs";
            ধ্রুবক অক্ষর *মান = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free অক্ষর *allowed_cpus = ফাঁকা;
            size_t allowed_cpus_len = 0;

            যদি (মান == ফাঁকা)
              মান = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (মান, &allowed_cpus, &allowed_cpus_len, err);
            যদি (UNLIKELY (ret < 0))
              ফেরত ret;

            যদি (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                যদি (UNLIKELY (ret < 0))
                  ফেরত ret;
              }
          }

        যদি (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ধ্রুবক অক্ষর *property_name = "AllowedMemoryNodes";
            ধ্রুবক অক্ষর *মান = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free অক্ষর *allowed_mems = ফাঁকা;
            size_t allowed_mems_len = 0;

            যদি (মান == ফাঁকা)
              মান = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (মান, &allowed_mems, &allowed_mems_len, err);
            যদি (UNLIKELY (ret < 0))
              ফেরত ret;

            যদি (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                যদি (UNLIKELY (ret < 0))
                  ফেরত ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        যদি (UNLIKELY (ret < 0))
          ফেরত ret;
      }
      বিরতি;

    ক্ষেত্র CGROUP_MODE_LEGACY:
    ক্ষেত্র CGROUP_MODE_HYBRID:
      যদি (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_মান ("CPUShares", resources->cpu->shares);
      বিরতি;

    পূর্বনির্ধারিত:
      ফেরত crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_মান

  যদি (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
reset_failed_unit (sd_bus *bus, ধ্রুবক অক্ষর *unit)
{
  পূর্ণসংখ্যা sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = ফাঁকা, *reply = ফাঁকা;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  যদি (UNLIKELY (sd_err < 0))
    যাও exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  যদি (UNLIKELY (sd_err < 0))
    যাও exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  যদি (UNLIKELY (sd_err < 0))
    যাও exit;

  sd_err = 0;

exit:
  যদি (m)
    sd_bus_message_unref (m);
  যদি (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  ফেরত sd_err;
}

স্থির পূর্ণসংখ্যা
verify_ebpf_device_filter_installed (ধ্রুবক অক্ষর *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = ফাঁকা;
  cleanup_free অক্ষর *full_path = ফাঁকা;
  size_t n_progs = 0;
  পূর্ণসংখ্যা ret;

  ret = append_paths (&full_path, err, CGROUP_মূল, cgroup_path, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (n_progs == 0)
    ফেরত crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  ফেরত 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
স্থির পূর্ণসংখ্যা
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, পূর্ণসংখ্যা ret)
{
  যদি (bus)
    sd_bus_unref (bus);
  যদি (m)
    sd_bus_message_unref (m);
  যদি (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  ফেরত ret;
}

স্থির পূর্ণসংখ্যা
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            পূর্ণসংখ্যা cgroup_mode,
                            string_map *annotations,
                            ধ্রুবক অক্ষর *state_মূল,
                            ধ্রুবক অক্ষর *scope, ধ্রুবক অক্ষর *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = ফাঁকা;
  sd_bus_message *m = ফাঁকা;
  sd_bus_message *reply = ফাঁকা;
  পূর্ণসংখ্যা sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ধ্রুবক অক্ষর *object = ফাঁকা;
  কাঠামো systemd_job_removed_s job_তথ্য = {};
  পূর্ণসংখ্যা i;
  ধ্রুবক অক্ষর *boolean_opts[10];
  cleanup_free অক্ষর *state_dir = ফাঁকা;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_মূল, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = ফাঁকা;

  ret = open_sd_bus_connection (&bus, err);
  যদি (UNLIKELY (ret < 0))
    যাও exit;

  ret = systemd_check_job_status_setup (bus, &job_তথ্য, err);
  যদি (UNLIKELY (ret < 0))
    যাও exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      যাও exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      যাও exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      যাও exit;
    }

  যদি (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      যদি (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          যাও exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      যাও exit;
    }

  যদি (annotations)
    {
      size_t prefix_len = আকার (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_আকার (annotations);
      size_t i;

      জন্য (i = 0; i < annotations_len; i++)
        {
          ধ্রুবক অক্ষর *চাবি;
          ধ্রুবক অক্ষর *মান;
          size_t len;

          ret = string_map_get_at (annotations, i, &চাবি, &মান);
          যদি (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              যাও exit;
            }

          যদি (! has_prefix (চাবি, SYSTEMD_PROPERTY_PREFIX))
            চালু;

          len = strlen (চাবি);
          যদি (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", চাবি);
              যাও exit;
            }

          ret = append_systemd_annotation (m, চাবি + prefix_len, len - prefix_len,
                                           মান, err);
          যদি (UNLIKELY (ret < 0))
            যাও exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      যাও exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      যাও exit;
    }

  জন্য (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      যদি (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          যাও exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  যদি (UNLIKELY (ret < 0))
    যাও exit;

  sd_err = sd_bus_message_close_container (m);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      যাও exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      যাও exit;
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
          reply = ফাঁকা;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      যদি (sd_err < 0)
        {
          যদি (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              যদি (UNLIKELY (ret < 0))
                যাও exit;
              যদি (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          যাও exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      যাও exit;
    }

  ret = systemd_check_job_status (bus, &job_তথ্য, object, "creating", err);

exit:
  ফেরত cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

স্থির পূর্ণসংখ্যা
libcrun_destroy_systemd_cgroup_scope (কাঠামো libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = ফাঁকা;
  sd_bus_message *m = ফাঁকা;
  sd_bus_message *reply = ফাঁকা;
  পূর্ণসংখ্যা ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ধ্রুবক অক্ষর *object;
  ধ্রুবক অক্ষর *scope = cgroup_status->scope;
  কাঠামো systemd_job_removed_s job_তথ্য = {};

  ret = open_sd_bus_connection (&bus, err);
  যদি (UNLIKELY (ret < 0))
    যাও exit;

  ret = systemd_check_job_status_setup (bus, &job_তথ্য, err);
  যদি (UNLIKELY (ret < 0))
    যাও exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      যাও exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      যাও exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      যাও exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      যাও exit;
    }

  ret = systemd_check_job_status (bus, &job_তথ্য, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  ফেরত cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

স্থির ধ্রুবক অক্ষর *
find_systemd_subgroup (string_map *annotations)
{
  ধ্রুবক অক্ষর *annotation;

  annotation = find_string_map_মান (annotations, "run.oci.systemd.subgroup");
  যদি (annotation)
    {
      যদি (annotation[0] == '\0')
        ফেরত ফাঁকা;
      ফেরত annotation;
    }

  ফেরত "container";
}

স্থির পূর্ণসংখ্যা
libcrun_cgroup_enter_systemd (কাঠামো libcrun_cgroup_args *args,
                              কাঠামো libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  ধ্রুবক অক্ষর *cgroup_path = args->cgroup_path;
  cleanup_free অক্ষর *scope_path = ফাঁকা;
  cleanup_free অক্ষর *scope = ফাঁকা;
  cleanup_free অক্ষর *path = ফাঁকা;
  cleanup_free অক্ষর *slice = ফাঁকা;
  পূর্ণসংখ্যা retries_বাম = 32;
  ধ্রুবক অক্ষর *suffix;
  ধ্রুবক অক্ষর *id = args->id;
  pid_t pid = args->pid;
  পূর্ণসংখ্যা cgroup_mode;
  পূর্ণসংখ্যা rootless = 0;
  পূর্ণসংখ্যা ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      যদি (UNLIKELY (rootless < 0))
        ফেরত rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  জন্য (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_মূল,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      যদি (LIKELY (ret >= 0))
        বিরতি;

      যদি (can_retry && retries_বাম-- > 0)
        {
          crun_error_release (err);
          চালু;
        }

      ফেরত ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  যদি (is_empty_string (suffix))
    scope_path = xstrdup (path);
  অন্যথা
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      যদি (UNLIKELY (scope_path == ফাঁকা))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  যদি (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  out->path = path;
  path = ফাঁকা;

  out->scope = scope;
  scope = ফাঁকা;
  ফেরত 0;
}

অক্ষর *
get_cgroup_scope_path (ধ্রুবক অক্ষর *cgroup_path, ধ্রুবক অক্ষর *scope)
{
  অক্ষর *path_to_scope = ফাঁকা;
  অক্ষর *cur;

  যদি (is_empty_string (cgroup_path))
    ফেরত ফাঁকা;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  যখন (cur)
    {
      অক্ষর *পরবর্তী = strchr (cur + 1, '/');
      যদি (পরবর্তী == ফাঁকা)
        বিরতি;

      *পরবর্তী = '\0';
      যদি (strcmp (cur, scope) == 0)
        ফেরত path_to_scope;
      *পরবর্তী = '/';

      cur = পরবর্তী;
      যখন (*cur == '/')
        cur++;
    }

  ফেরত path_to_scope;
}

স্থির পূর্ণসংখ্যা
libcrun_destroy_cgroup_systemd (কাঠামো libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free অক্ষর *path_to_scope = ফাঁকা;
  cleanup_free অক্ষর *bpfprog = ফাঁকা;
  পূর্ণসংখ্যা mode;
  পূর্ণসংখ্যা ret;

  mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (mode < 0))
    ফেরত mode;

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

  ফেরত destroy_cgroup_path (path_to_scope, mode, err);
}

স্থির পূর্ণসংখ্যা
libcrun_update_resources_systemd (কাঠামো libcrun_cgroup_status *cgroup_status,
                                  ধ্রুবক অক্ষর *state_মূল,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  কাঠামো systemd_job_removed_s job_তথ্য = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free অক্ষর *state_dir = ফাঁকা;
  sd_bus_message *reply = ফাঁকা;
  sd_bus_message *m = ফাঁকা;
  sd_bus *bus = ফাঁকা;
  পূর্ণসংখ্যা sd_err, ret;
  পূর্ণসংখ্যা cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_মূল, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ফেরত cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = systemd_check_job_status_setup (bus, &job_তথ্য, err);
  যদি (UNLIKELY (ret < 0))
    যাও exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      যাও exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      যাও exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      যাও exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  যদি (UNLIKELY (ret < 0))
    যাও exit;

  sd_err = sd_bus_message_close_container (m);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      যাও exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  যদি (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      যাও exit;
    }

  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      যদি (UNLIKELY (ret < 0))
        যাও exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      যদি (UNLIKELY (ret < 0))
        যাও exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  যদি (UNLIKELY (ret < 0))
    যাও exit;

  ret = 0;

exit:
  ফেরত cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
স্থির পূর্ণসংখ্যা
libcrun_attach_process_systemd (কাঠামো libcrun_cgroup_status *cgroup_status,
                                ধ্রুবক অক্ষর *path, pid_t pid,
                                libcrun_error_t *err)
{
  ধ্রুবক অক্ষর *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free অক্ষর *scope_path = ফাঁকা;
  sd_bus_message *reply = ফাঁকা;
  ধ্রুবক অক্ষর *subcgroup;
  ধ্রুবক অক্ষর *last;
  sd_bus *bus = ফাঁকা;
  size_t len;
  পূর্ণসংখ্যা sd_err, ret;

  যদি (is_empty_string (scope))
    ফেরত crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  যদি (scope_path == ফাঁকা)
    ফেরত crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  যদি (last == ফাঁকা || strcmp (last + 1, scope) != 0)
    ফেরত crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  যদি (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  ফেরত cleanup_sd_bus_and_return (bus, ফাঁকা, reply, &error, ret);
}

#অন্যথা
স্থির পূর্ণসংখ্যা
libcrun_cgroup_enter_systemd (কাঠামো libcrun_cgroup_args *args,
                              কাঠামো libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (শূন্য) args;
  (শূন্য) out;

  ফেরত crun_make_error (err, ENOTSUP, "systemd not supported");
}

স্থির পূর্ণসংখ্যা
libcrun_destroy_cgroup_systemd (কাঠামো libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (শূন্য) cgroup_status;

  ফেরত crun_make_error (err, ENOTSUP, "systemd not supported");
}

স্থির পূর্ণসংখ্যা
libcrun_update_resources_systemd (কাঠামো libcrun_cgroup_status *cgroup_status,
                                  ধ্রুবক অক্ষর *state_মূল,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (শূন্য) cgroup_status;
  (শূন্য) state_মূল;
  (শূন্য) resources;

  ফেরত crun_make_error (err, ENOTSUP, "systemd not supported");
}

স্থির পূর্ণসংখ্যা
libcrun_attach_process_systemd (কাঠামো libcrun_cgroup_status *cgroup_status,
                                ধ্রুবক অক্ষর *path, pid_t pid,
                                libcrun_error_t *err)
{
  (শূন্য) cgroup_status;
  (শূন্য) path;
  (শূন্য) pid;

  ফেরত crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

কাঠামো libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = ফাঁকা,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
