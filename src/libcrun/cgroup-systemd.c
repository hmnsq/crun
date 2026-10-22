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

#تعریف_کن GNU_منبع

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

#اگر_تعریف_شده HAVE_SYSTEMD
#  include <systemd/sd-bus.h>
#  include <limits.h>

#  define SYSTEMD_PROPERTY_پیشوند "org.systemd.property."

#  define CGROUP_BFQ_وزن_MIN ((uint64_t) 1)
#  define CGROUP_BFQ_وزن_DEFAULT ((uint64_t) 100)
#  define CGROUP_BFQ_وزن_MAX ((uint64_t) 1000)

#  define CGROUP_وزن_MIN ((uint64_t) 1)
#  define CGROUP_وزن_DEFAULT ((uint64_t) 100)
#  define CGROUP_وزن_MAX ((uint64_t) 10000)

/* The CPU quota period used by the kernel (and systemd) by default, in us.  */
#  define DEFAULT_CPU_QUOTA_PERIOD ((uint64_t) 100000)

#  define SYSTEMD_MISSING_PROPERTIES_DIR ".cache/systemd-missing-properties"

#  define IS_WILDCARD(x) (x <= 0)

ساكن صحيح
ثبات_missing_property_from_پیام (ثابت حرف *state_dir, ثابت حرف *پیام_2, libcrun_error_t *err)
{
  cleanup_free حرف *file_مسیر = عدم;
  cleanup_free حرف *dir_مسیر = عدم;
  cleanup_free حرف *property = عدم;
  حرف *p;
  صحيح ret;

  إذا (! has_پیشوند (پیام_2, "Cannot set property "))
    أرجع 0;

  property = xstrdup (پیام_2 + حجم ("Cannot set property ") - 1);
  p = strchr (property, ',');
  إذا (! p)
    أرجع 0;
  *p = '\0';

  ret = append_paths (&dir_مسیر, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (dir_مسیر, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&file_مسیر, err, dir_مسیر, property, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_مسیر, عدم, 0, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 1;
}

ساكن bool
property_missing_p (حرف **missing_properties, ثابت حرف *property)
{
  size_t i;

  لكل (i = 0; missing_properties && missing_properties[i]; i++)
    إذا (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        أرجع true;
      }

  أرجع false;
}

ساكن فراغ
get_systemd_scope_and_برش (ثابت حرف *id, bool user_برش, ثابت حرف *cgroup_مسیر,
                             حرف **scope, حرف **برش)
{
  ثابت حرف *n;
  حرف *p;

  إذا (cgroup_مسیر == عدم || cgroup_مسیر[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      أرجع;
    }

  n = strchr (cgroup_مسیر, ':');
  إذا (n == عدم)
    xasprintf (scope, "%s.scope", cgroup_مسیر);
  وإلا
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      إذا (p)
        *p = '-';
    }
  إذا (برش)
    {
      *برش = xstrdup (cgroup_مسیر);
      p = strchr (*برش, ':');
      إذا (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      إذا (is_خالی_string (*برش))
        {
          حرر (*برش);
          إذا (user_برش)
            *برش = xstrdup ("user.slice");
          وإلا
            *برش = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
ساكن صحيح
راهاندازی_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ثابت حرف *مسیر, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_مسیر = عدم;
  cleanup_close صحيح dirfd = -1;
  bool need_مجموعه_والد = true;
  حرف fmt_buf[64];
  صحيح len;
  صحيح ret;

  إذا (resources == عدم || resources->cpu == عدم)
    أرجع 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  إذا (is_خالی_string (مسیر))
    أرجع crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  إذا (has_suffix (مسیر, ".scope"))
    need_مجموعه_والد = false;

  ret = append_paths (&cgroup_مسیر, err, CGROUP_ریشه, "cpu", مسیر, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (cgroup_مسیر, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dirfd = open (cgroup_مسیر, O_DIRECTORY | O_مسیر | O_CLOEXEC);
  إذا (UNLIKELY (dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_مسیر);

  إذا (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      إذا (need_مجموعه_والد)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      إذا (need_مجموعه_والد)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
راهاندازی_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ثابت حرف *مسیر, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_مسیر = عدم;
  صحيح والد_2;
  صحيح ret;

  إذا (resources == عدم || resources->cpu == عدم)
    أرجع 0;

  إذا (! resources->cpu->مدت_present && ! (cgroup2 && resources->cpu->idle_present))
    أرجع 0;

  لكل (والد_2 = 0; والد_2 < 2; والد_2++)
    {
      cleanup_close صحيح dirfd = -1;

      إذا (cgroup2)
        ret = append_paths (&cgroup_مسیر, err, CGROUP_ریشه, مسیر ? مسیر : "", (والد_2 ? ".." : عدم), عدم);
      وإلا
        ret = append_paths (&cgroup_مسیر, err, CGROUP_ریشه, "/cpu", مسیر ? مسیر : "", (والد_2 ? ".." : عدم), عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      dirfd = open (cgroup_مسیر, O_DIRECTORY | O_مسیر | O_CLOEXEC);
      إذا (UNLIKELY (dirfd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", cgroup_مسیر);

      ret = write_cpu_مدت (dirfd, cgroup2, resources->cpu, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      إذا (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  أرجع 0;
}

ساكن صحيح
راهاندازی_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *مسیر, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_مسیر = عدم;
  صحيح والد_2;
  صحيح ret;
  صحيح سطوح;

  ret = append_paths (&cgroup_مسیر, err, CGROUP_ریشه, "/cpuset", مسیر ? مسیر : "", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (cgroup_مسیر, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_مسیر, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  سطوح = has_suffix (مسیر, ".scope") ? 1 : 2;

  لكل (والد_2 = 0; والد_2 < سطوح; والد_2++)
    {
      cleanup_close صحيح dirfd_cpuset = -1;
      cleanup_free حرف *مسیر_مقصد_cpuset = عدم;

      ret = append_paths (&مسیر_مقصد_cpuset, err, CGROUP_ریشه "/cpuset", مسیر, (والد_2 ? ".." : عدم), عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      dirfd_cpuset = open (مسیر_مقصد_cpuset, O_DIRECTORY | O_مسیر | O_CLOEXEC);
      إذا (UNLIKELY (dirfd_cpuset < 0))
        أرجع crun_make_error (err, errno, "open `%s`", مسیر_مقصد_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
ساكن صحيح
compute_finalized_مسیر (حرف **مسیر, ثابت حرف *from, ثابت حرف *suffix, libcrun_error_t *err)
{
  إذا (suffix == عدم)
    {
      *مسیر = xstrdup (from);
      أرجع 0;
    }
  أرجع append_paths (مسیر, err, from, suffix, عدم);
}

ساكن صحيح
systemd_finalize (بنية libcrun_cgroup_آرگومانها *آرگومانها, حرف **مسیر_بیرون,
                  صحيح cgroup_نما, ثابت حرف *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = آرگومانها->resources;
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *مسیر = عدم;
  pid_t pid = آرگومانها->pid;
  صحيح ret;
  حرف *from, *مقصد;
  حرف *saveptr = عدم;

  cleanup_free حرف *proc_مسیر = عدم;
  cleanup_close صحيح fd = -1;

  xasprintf (&proc_مسیر, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (آرگومانها->container, proc_مسیر, O_RDONLY, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = read_all_fd (fd, "cgroup path", &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  بدل (cgroup_نما)
    {
    حالة CGROUP_نما_HYBRID:
    حالة CGROUP_نما_LEGACY:
      from = جستجوی_رشته (content, ":memory:");
      إذا (LIKELY (from != عدم))
        from += 8;
      وإلا
        {
          from = جستجوی_رشته (content, ":pids:");
          إذا (UNLIKELY (from == عدم))
            أرجع crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      مقصد = strchr (from, '\n');
      إذا (UNLIKELY (مقصد == عدم))
        أرجع crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *مقصد = '\0';
      ret = compute_finalized_مسیر (&مسیر, from, suffix, err);
      *مقصد = '\n';
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (geteuid ())
        أرجع 0;

      ret = راهاندازی_rt_runtime (resources, مسیر, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = راهاندازی_cpuset_for_systemd_v1 (resources, مسیر, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      لكل (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (عدم, "\n", &saveptr))
        {
          حرف *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          إذا (subsystem[0] == '\0')
            {
              إذا (cgroup_نما == CGROUP_نما_LEGACY)
                استمر;

              subsystem = "unified";
            }

          إذا (strcmp (subpath, مسیر))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, مسیر, true, err);
              إذا (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  إذا (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      استمر;
                    }
                  أرجع ret;
                }
            }
        }

      اكسر;

    حالة CGROUP_نما_UNIFIED:
      {
        cleanup_free حرف *dir = عدم;

        from = جستجوی_رشته (content, "0::");
        إذا (UNLIKELY (from == عدم))
          أرجع crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        مقصد = strchr (from, '\n');
        إذا (UNLIKELY (مقصد == عدم))
          أرجع crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *مقصد = '\0';
        ret = compute_finalized_مسیر (&مسیر, from, suffix, err);
        *مقصد = '\n';
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        ret = append_paths (&dir, err, CGROUP_ریشه, مسیر, عدم);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        ret = move_process_مقصد_cgroup (pid, عدم, مسیر, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        ret = enable_controllers (مسیر, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        إذا (suffix)
          {
            ret = chown_cgroups (مسیر, آرگومانها->ریشه_uid, آرگومانها->ریشه_gid, err);
            إذا (UNLIKELY (ret < 0))
              أرجع ret;
          }
      }
      اكسر;

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_نما);
    }

  ret = راهاندازی_missing_cpu_options_for_systemd (resources, cgroup_نما == CGROUP_نما_UNIFIED, مسیر, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  *مسیر_بیرون = مسیر;
  مسیر = عدم;

  أرجع 0;
}

بنية systemd_کار_removed_s
{
  ثابت حرف *مسیر;
  ثابت حرف *op;
  صحيح terminated;
  libcrun_error_t err;
};

ساكن صحيح
systemd_کار_removed (sd_bus_پیام *m, فراغ *userdata, sd_bus_error *error arg_استفادهنشده)
{
  ثابت حرف *مسیر, *unit, *نتیجه;
  uint32_t id;
  صحيح ret;
  بنية systemd_کار_removed_s *d = userdata;

  ret = sd_bus_پیام_read (m, "uoss", &id, &مسیر, &unit, &نتیجه);
  إذا (ret < 0)
    أرجع -1;

  إذا (strcmp (d->مسیر, مسیر) == 0)
    {
      d->terminated = 1;
      إذا (strcmp (نتیجه, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, نتیجه);
          أرجع -1;
        }
    }
  أرجع 0;
}

ساكن صحيح
systemd_check_کار_وضعیت_راهاندازی (sd_bus *bus, بنية systemd_کار_removed_s *داده, libcrun_error_t *err)
{
  صحيح ret;

  ret = sd_bus_تطابق_signal_async (bus, عدم, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_کار_removed, عدم, داده);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "sd-bus match signal");

  أرجع 0;
}

ساكن صحيح
systemd_check_کار_وضعیت (sd_bus *bus, بنية systemd_کار_removed_s *داده, ثابت حرف *مسیر, ثابت حرف *op,
                          libcrun_error_t *err)
{
  صحيح sd_err;

  داده->مسیر = مسیر;
  داده->op = op;
  طالما (! داده->terminated)
    {
      sd_err = sd_bus_process (bus, عدم);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus process");

      إذا (sd_err != 0)
        استمر;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus wait");
    }

  إذا (داده->err != عدم)
    {
      *err = داده->err;
      أرجع -1;
    }

  أرجع 0;
}

صحيح
parse_sd_array (حرف *s, حرف **بیرون, حرف **بعدی, libcrun_error_t *err)
{
  حرف endchr;
  حرف *it, *dest;
  bool escaped = false;

  *بیرون = عدم;
  *بعدی = عدم;

  طالما (آیا_فاصله (*s))
    s++;
  إذا (*s == '\0')
    أرجع 0;
  وإلا إذا (*s != '\'' && *s != '"')
    أرجع crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *بیرون = dest = it;

  طالما (1)
    {
      إذا (*it == '\0')
        أرجع crun_make_error (err, 0, "invalid string `%s`", s);
      إذا (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          طالما (آیا_فاصله (*it))
            it++;
          إذا (*it == ',')
            {
              *بعدی = ++it;
              *dest = '\0';
              أرجع 0;
            }

          إذا (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              أرجع 0;
            }

          أرجع crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      إذا (! escaped)
        *dest++ = *it;
      it++;
    }

  أرجع 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
ساكن صحيح
append_systemd_annotation (sd_bus_پیام *m, ثابت حرف *name, size_t name_len, ثابت حرف *مقدار,
                           libcrun_error_t *err)
{
  cleanup_free حرف *tmp_name = عدم;
  uint32_t ضریب = 1;
  ثابت حرف *it;
  صحيح sd_err;

  طالما (*مقدار == ' ')
    مقدار++;

  it = مقدار;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  إذا (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
      && name[name_len - 1] == 'c')
    {
      ضریب = 1000000;

      tmp_name = xmalloc (name_len + 2);
      کپی_حافظه (tmp_name, name, name_len - 3);
      کپی_حافظه (tmp_name + name_len - 3, "USec", 5);

      name = tmp_name;
    }

  /* A *USec property is always an uint64, so, like runc does, accept any
     number of seconds (possibly with a fractional part, and with an
     optional integer type), and convert it to uint64 microseconds.  */
  إذا (ضریب != 1)
    {
      ساكن ثابت حرف *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", عدم };
      ثابت حرف *رقم = it;
      حرف *endptr = عدم;
      مزدوج v;
      size_t i;

      لكل (i = 0; types[i]; i++)
        إذا (has_پیشوند (رقم, types[i]))
          {
            رقم += طول_رشته (types[i]);
            اكسر;
          }

      errno = 0;
      v = strtod (رقم, &endptr);
      إذا (UNLIKELY (errno != 0 || endptr == رقم || *endptr || v < 0 || v * ضریب >= 18446744073709551615.0))
        أرجع crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, مقدار);

      sd_err = sd_bus_پیام_append (m, "(sv)", name, "t", (uint64_t) (v * ضریب));
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }

  إذا ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_پیام_append (m, "(sv)", name, "b", b);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا إذا (*it == '\'')
    {
      cleanup_free حرف *v_شروع = عدم;
      حرف *انتها;

      it = v_شروع = xstrdup (مقدار);

      انتها = strchr (v_شروع + 1, '\'');
      إذا (انتها == عدم)
        أرجع crun_make_error (err, 0, "invalid variant `%s`", مقدار);
      *انتها = '\0';

      sd_err = sd_bus_پیام_append (m, "(sv)", name, "s", it + 1);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا إذا (*it == '[')
    {
      cleanup_free حرف *v_شروع = عدم;
      size_t n_parts = 0, parts_اندازه = 32;
      cleanup_free حرف **parts = xmalloc (حجم (حرف *) * parts_اندازه);
      حرف *part;

      part = v_شروع = xstrdup (it + 1);
      طالما (1)
        {
          حرف *بیرون = عدم, *بعدی = عدم;
          صحيح ret;

          ret = parse_sd_array (part, &بیرون, &بعدی, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          parts[n_parts++] = بیرون;
          إذا (n_parts == parts_اندازه - 1)
            {
              parts_اندازه += 32;
              parts = xrealloc (parts, حجم (حرف *) * parts_اندازه);
            }
          parts[n_parts] = عدم;
          إذا (بعدی == عدم)
            اكسر;

          part = بعدی;
        }

      sd_err = sd_bus_پیام_open_container (m, 'r', "sv");
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_پیام_append (m, "s", name);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_پیام_open_container (m, 'v', "as");
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_پیام_append_strv (m, parts);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_پیام_close_container (m);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_پیام_close_container (m);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus close container");

      أرجع 0;
    }
  وإلا إذا (has_پیشوند (it, "uint64 "))
    {
      حرف *endptr = عدم;
      uint64_t v;

      errno = 0;
      v = strtoull (it + حجم ("uint64"), &endptr, 10);
      إذا (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پیام_append (m, "(sv)", name, "t", (uint64_t) (v * ضریب));
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا إذا (has_پیشوند (it, "int64 "))
    {
      حرف *endptr = عدم;
      int64_t v;

      errno = 0;
      v = strtoll (it + حجم ("int64"), &endptr, 10);
      إذا (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پیام_append (m, "(sv)", name, "x", (int64_t) (v * ضریب));
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا إذا (has_پیشوند (it, "uint32 "))
    {
      حرف *endptr = عدم;
      uint32_t v;

      errno = 0;
      v = strtoul (it + حجم ("uint32"), &endptr, 10);
      إذا (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پیام_append (m, "(sv)", name, "u", (uint32_t) (v * ضریب));
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا إذا (has_پیشوند (it, "int32 ") || strchr (it, ' ') == عدم)
    {
      حرف *endptr = عدم;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      إذا (has_پیشوند (it, "int32 "))
        v = strtol (it + حجم ("int32"), &endptr, 10);
      وإلا
        v = strtol (it, &endptr, 10);
      إذا (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پیام_append (m, "(sv)", name, "i", (int32_t) (v * ضریب));
      إذا (UNLIKELY (sd_err < 0))
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
  إذا (UNLIKELY (rootless < 0))
    أرجع rootless;

  إذا (rootless)
    sd_err = sd_bus_default_user (bus);
  إذا (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  إذا (sd_err < 0)
    أرجع crun_make_error (err, -sd_err, "cannot open sd-bus");

  أرجع 0;
}

ساكن ثابت حرف *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *name)
{
  size_t i;

  إذا (resources == عدم || resources->unified == عدم)
    أرجع عدم;

  لكل (i = 0; i < resources->unified->len; i++)
    إذا (strcmp (resources->unified->کلیدها[i], name) == 0)
      أرجع resources->unified->مقادیر[i];
  أرجع عدم;
}

ساكن صحيح
get_مقدار_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *name,
                            uint64_t *مقدار, libcrun_error_t *err)
{
  حرف *endptr = عدم;
  size_t i;

  إذا (resources == عدم || resources->unified == عدم)
    أرجع 0;

  لكل (i = 0; i < resources->unified->len; i++)
    إذا (strcmp (resources->unified->کلیدها[i], name) == 0)
      {
        إذا (strcmp (resources->unified->مقادیر[i], "max") == 0)
          {
            *مقدار = UINT64_MAX;
            أرجع 1;
          }

        errno = 0;
        *مقدار = strtoull (resources->unified->مقادیر[i], &endptr, 10);
        إذا (UNLIKELY (errno))
          أرجع crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->مقادیر[i]);
        إذا (endptr && *endptr)
          أرجع crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->مقادیر[i]);
        أرجع 1;
      }
  أرجع 0;
}

ساكن مضمن صحيح
get_حافظه_کم (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد, libcrun_error_t *err)
{
  إذا (resources->حافظه && resources->حافظه->reservation_present)
    {
      *حد = resources->حافظه->reservation;
      أرجع 1;
    }

  أرجع get_مقدار_from_unified_map (resources, "memory.low", حد, err);
}

ساكن مضمن صحيح
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد, libcrun_error_t *err)
{
  إذا (resources->pids && resources->pids->حد)
    {
      *حد = resources->pids->حد;
      أرجع 1;
    }

  أرجع get_مقدار_from_unified_map (resources, "pids.max", حد, err);
}

ساكن مضمن صحيح
get_حافظه_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد, libcrun_error_t *err)
{
  إذا (resources->حافظه && resources->حافظه->حد_present)
    {
      *حد = resources->حافظه->حد;
      أرجع 1;
    }

  أرجع get_مقدار_from_unified_map (resources, "memory.max", حد, err);
}

ساكن مضمن صحيح
get_حافظه_جابجا_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد, libcrun_error_t *err)
{
  إذا (resources->حافظه && resources->حافظه->جابجا_present)
    {
      runtime_spec_schema_config_linux_resources_حافظه *حافظه = resources->حافظه;
      int64_t mem = حافظه->حد_present ? حافظه->حد : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      إذا (حافظه->جابجا == -1 || (mem == -1 && حافظه->جابجا == 0))
        {
          *حد = UINT64_MAX;
          أرجع 1;
        }
      /* 0 means unset.  */
      إذا (حافظه->جابجا == 0)
        أرجع 0;
      إذا (mem <= 0)
        أرجع crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      إذا (حافظه->جابجا < mem)
        أرجع crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *حد = حافظه->جابجا - mem;
      أرجع 1;
    }

  أرجع get_مقدار_from_unified_map (resources, "memory.swap.max", حد, err);
}

ساكن مضمن صحيح
get_cpu_وزن (runtime_spec_schema_config_linux_resources *resources, uint64_t *وزن, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t مقدار;
  صحيح ret;

  ret = get_مقدار_from_unified_map (resources, "cpu.idle", &مقدار, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  إذا (ret > 0 && مقدار == 1)
    has_idle = true;
  إذا (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_مقدار_from_unified_map (resources, "cpu.weight", وزن, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  إذا (ret > 0)
    {
      إذا (has_idle)
        أرجع crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      أرجع 1;
    }

  إذا (resources->cpu && resources->cpu->shares_present)
    {
      إذا (has_idle)
        أرجع crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      إذا (resources->cpu->shares == 0)
        أرجع 0;
      *وزن = convert_shares_مقصد_وزن (resources->cpu->shares);
      أرجع 1;
    }
  إذا (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *وزن = 0;
      أرجع 1;
    }
  أرجع 0;
}

ساكن صحيح
read_uint64_مقدار (ثابت حرف *str, uint64_t *مقدار, libcrun_error_t *err)
{
  حرف *endptr = عدم;

  errno = 0;
  *مقدار = strtoull (str, &endptr, 10);
  إذا (UNLIKELY (errno))
    أرجع crun_make_error (err, errno, "invalid value: %s", str);
  إذا (endptr && *endptr)
    أرجع crun_make_error (err, 0, "invalid value: %s", str);
  أرجع 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
ساكن مضمن uint64_t
IO_وزن (uint64_t bfq_وزن)
{
  أرجع bfq_وزن <= CGROUP_BFQ_وزن_DEFAULT ? CGROUP_وزن_DEFAULT - (CGROUP_BFQ_وزن_DEFAULT - bfq_وزن) * (CGROUP_وزن_DEFAULT - CGROUP_وزن_MIN) / (CGROUP_BFQ_وزن_DEFAULT - CGROUP_BFQ_وزن_MIN) : CGROUP_وزن_DEFAULT + (bfq_وزن - CGROUP_BFQ_وزن_DEFAULT) * (CGROUP_وزن_MAX - CGROUP_وزن_DEFAULT) / (CGROUP_BFQ_وزن_MAX - CGROUP_BFQ_وزن_DEFAULT);
}

ساكن صحيح
append_io_وزن (sd_bus_پیام *m, حرف **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t وزن;
  صحيح sd_err;
  size_t i;
  صحيح ret;

#  define APPEND_IO_وزن(مقدار)                                                      \
    افعل                                                                                 \
      {                                                                                \
        إذا (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_پیام_append (m, "(sv)", "IOWeight", "t", وزن);       \
            إذا (UNLIKELY (sd_err < 0))                                                 \
              أرجع crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } طالما (0)

#  define APPEND_IO_DEVICE_وزن(device, مقدار)                                                      \
    افعل                                                                                                \
      {                                                                                               \
        إذا (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_پیام_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, وزن); \
            إذا (UNLIKELY (sd_err < 0))                                                                \
              أرجع crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } طالما (0)

  إذا (resources->بلوک_io && resources->بلوک_io->وزن_present)
    {
      وزن = IO_وزن (resources->بلوک_io->وزن);
      APPEND_IO_وزن (وزن);
    }

  لكل (i = 0; resources->بلوک_io && i < resources->بلوک_io->وزن_device_len; i++)
    {
      حرف name[64];
      صحيح len;

      len = snprintf (name, حجم (name), "%" PRIu64 ":%" PRIu64,
                      resources->بلوک_io->وزن_device[i]->major,
                      resources->بلوک_io->وزن_device[i]->minor);
      إذا (UNLIKELY (len >= (صحيح) حجم (name)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      وزن = IO_وزن (resources->بلوک_io->وزن_device[i]->وزن);
      APPEND_IO_DEVICE_وزن (name, وزن);
    }

  لكل (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free حرف *مقدار = عدم;
      bool need_conversion = false;
      حرف *saveptr = عدم;
      حرف *سطر;

      إذا (strcmp (resources->unified->کلیدها[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      وإلا إذا (strcmp (resources->unified->کلیدها[i], "io.weight") == 0)
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

      مقدار = xstrdup (resources->unified->مقادیر[i]);

      لكل (سطر = strtok_r (مقدار, "\n", &saveptr); سطر; سطر = strtok_r (عدم, "\n", &saveptr))
        {
          حرف *sep;

          sep = strchr (سطر, ' ');
          طالما (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_مقدار (sep ? sep : سطر, &وزن, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          إذا (need_conversion)
            وزن = IO_وزن (وزن);

          إذا (sep == عدم || strcmp (سطر, "default") == 0)
            APPEND_IO_وزن (وزن);
          وإلا
            APPEND_IO_DEVICE_وزن (سطر, وزن);
        }
    }

  أرجع 0;
}

/* Adapted from systemd.  */
ساكن صحيح
bus_append_byte_array (sd_bus_پیام *m, ثابت حرف *فیلد, ثابت فراغ *buf, size_t n, libcrun_error_t *err)
{
  صحيح ret;

  ret = sd_bus_پیام_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_پیام_append_basic (m, SD_BUS_TYPE_STRING, فیلد);
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_پیام_open_container (m, 'v', "ay");
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_پیام_append_array (m, 'y', buf, n);
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_پیام_close_container (m);
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_پیام_close_container (m);
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_close_container");

  أرجع 1;
}

ساكن صحيح
append_uint64_from_unified_map (sd_bus_پیام *m,
                                حرف **missing_properties,
                                ثابت حرف *attr,
                                ثابت حرف *کلید,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t مقدار;
  صحيح ret;

  إذا (property_missing_p (missing_properties, attr))
    أرجع 0;

  ret = get_مقدار_from_unified_map (resources, کلید, &مقدار, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  إذا (ret)
    {
      صحيح sd_err = sd_bus_پیام_append (m, "(sv)", attr, "t", مقدار);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  أرجع 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
ساكن صحيح
append_device_allow (sd_bus_پیام *m,
                     ثابت حرف type,
                     صحيح major,
                     صحيح minor,
                     ثابت حرف *access,
                     libcrun_error_t *err)
{
  حرف device[64];
  صحيح sd_err;
  صحيح len;

  إذا (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      أرجع 0;
    }

  إذا (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, حجم (device), "%s-*", type == 'c' ? "char" : "block");
  وإلا إذا (IS_WILDCARD (minor))
    len = snprintf (device, حجم (device), type == 'c' ? "char-%d" : "block-%d", major);
  وإلا
    len = snprintf (device, حجم (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  إذا (UNLIKELY (len >= (صحيح) حجم (device)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_پیام_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  إذا (UNLIKELY (sd_err < 0))
    أرجع crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  أرجع 0;
}

#  define نما_R 1
#  define نما_W 2
#  define نما_M 4

ساكن صحيح
access_مقصد_نما (ثابت حرف *access, libcrun_error_t *err)
{
  صحيح نما = 0;

  طالما (access && *access)
    {
      صحيح c = *access++;

      بدل (c)
        {
        حالة 'r':
          نما |= نما_R;
          اكسر;

        حالة 'w':
          نما |= نما_W;
          اكسر;

        حالة 'm':
          نما |= نما_M;
          اكسر;

        افتراضي:
          أرجع crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  أرجع نما;
}

/* check if the type b is included in type a.  */
ساكن bool
has_same_type (ثابت حرف *a, ثابت حرف *b)
{
  bool a_all = a == عدم || strcmp (a, "a") == 0;
  إذا (b == عدم || strcmp (b, "a") == 0)
    أرجع a_all;

  أرجع a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
ساكن صحيح
is_deny_قاعده_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  صحيح req_نما;

#  define CMP_DEV_رقم(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_نما = access_مقصد_نما (devices[n]->access, err);
  إذا (UNLIKELY (req_نما < 0))
    أرجع req_نما;

  لكل (i = n; i > 0; i--)
    {
      size_t current_قاعده = i - 1;
      صحيح نما;

      إذا (! CMP_DEV_رقم (devices[n]->minor, devices[current_قاعده]->minor))
        استمر;
      إذا (! CMP_DEV_رقم (devices[n]->major, devices[current_قاعده]->major))
        استمر;

      إذا (! has_same_type (devices[n]->type, devices[current_قاعده]->type)
          && has_same_type ("a", devices[current_قاعده]->type))
        استمر;

      نما = access_مقصد_نما (devices[i]->access, err);
      إذا (UNLIKELY (نما < 0))
        أرجع نما;

      إذا (devices[current_قاعده]->allow && (نما & req_نما))
        أرجع 0;

      req_نما &= ! نما;

      /* no more access bits to validate.  */
      إذا (req_نما == 0)
        أرجع 1;
    }

  أرجع 0;
#  undef CMP_DEV_رقم
}

ساكن size_t
پیدا_کن_اولی_قاعده_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  إذا (n == 0)
    أرجع 1;

  /* Find the first rule that is after the last "block all".  */
  لكل (i = n - 1; i-- > 0;)
    {
      إذا ((is_خالی_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        أرجع i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  لكل (i = 0; i < n; i++)
    إذا (devices[i]->allow)
      أرجع i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  أرجع n + 1;
}

ساكن bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_بلوک 1
#  define DEV_TYPE_CHAR 2
  صحيح مانده = DEV_TYPE_بلوک | DEV_TYPE_CHAR;
  size_t i;

  لكل (i = 0; i < n; i++)
    {
      size_t current_قاعده = n - i - 1;
      إذا (! devices[current_قاعده]->allow)
        أرجع false;

      إذا (IS_WILDCARD (devices[current_قاعده]->major) && IS_WILDCARD (devices[current_قاعده]->minor))
        {
          إذا (is_خالی_string (devices[current_قاعده]->type) || strcmp (devices[current_قاعده]->type, "a") == 0)
            أرجع true;

          إذا (strcmp (devices[current_قاعده]->type, "c") == 0)
            مانده &= ~DEV_TYPE_CHAR;
          وإلا إذا (strcmp (devices[current_قاعده]->type, "b") == 0)
            مانده &= ~DEV_TYPE_بلوک;

          إذا (مانده == 0)
            أرجع true;
        }
    }
#  undef DEV_TYPE_بلوک
#  undef DEV_TYPE_CHAR

  أرجع false;
}

ساكن صحيح
append_devices (sd_bus_پیام *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  بنية default_dev_s *default_devices = get_default_devices ();
  صحيح ret, sd_err;
  size_t i;

  إذا (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_پیام_append (m, "(sv)", "DevicePolicy", "s", "auto");
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_پیام_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      أرجع 0;
    }

  sd_err = sd_bus_پیام_append (m, "(sv)", "DevicePolicy", "s", "strict");
  إذا (UNLIKELY (sd_err < 0))
    أرجع crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  لكل (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  لكل (i = پیدا_کن_اولی_قاعده_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      حرف type;

      إذا (! d->allow)
        {
          صحيح redundant;

          redundant = is_deny_قاعده_redundant (resources->devices, i, err);
          إذا (UNLIKELY (redundant < 0))
            أرجع redundant;

          إذا (redundant)
            استمر;

          أرجع crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      إذا (d->type == عدم || strcmp (d->type, "a") == 0)
        type = 'a';
      وإلا إذا (strcmp (d->type, "c") == 0)
        type = 'c';
      وإلا إذا (strcmp (d->type, "b") == 0)
        type = 'b';
      وإلا
        أرجع crun_make_error (err, 0, "unknown device type `%s`", d->type);

      إذا (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          إذا (UNLIKELY (ret < 0))
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
bpfprog_مسیر_from_scope (حرف **مسیر, ثابت حرف *scope, libcrun_error_t *err)
{
  cleanup_free حرف *flat_scope = xstrdup (scope);
  حرف *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  طالما ((it = strchr (it, '.')) != عدم)
    *it = '_';

  أرجع append_paths (مسیر, err, CRUN_BPF_DIR, flat_scope, عدم);
}

ساكن صحيح
جمع_کن_bpf_برنامه (sd_bus_پیام *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 حرف **missing_properties,
                 ثابت حرف *scope,
                 bool *devices_مجموعه,
                 libcrun_error_t *err)
{
  صحيح ret = 0;
  صحيح درون_userns, sd_err;
  ثابت حرف *property_name = "BPFProgram";
  cleanup_free بنية bpf_برنامه *برنامه = عدم;
  cleanup_free حرف *مسیر = عدم;

  *devices_مجموعه = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  إذا (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    أرجع 0;

  درون_userns = check_running_درون_user_namespace (err);
  إذا (UNLIKELY (درون_userns < 0))
    أرجع درون_userns;

  إذا (درون_userns)
    أرجع 0;

  برنامه = create_dev_bpf (resources->devices, resources->devices_len, err);
  إذا (UNLIKELY (برنامه == عدم))
    {
      crun_error_release (err);
      أرجع 0;
    }

  ret = bpfprog_مسیر_from_scope (&مسیر, scope, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  إذا (UNLIKELY (ret < 0 && errno != EEXIST))
    أرجع crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  إذا (is_update)
    {
      cleanup_free بنية bpf_برنامه *برنامه_loaded = عدم;

      إذا (resources->devices == عدم || resources->devices_len == 0)
        {
          *devices_مجموعه = true;
          أرجع 0;
        }

      ret = libcrun_ebpf_read_برنامه (&برنامه_loaded, مسیر, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      /* it is the same as the loaded program.  */
      إذا (libcrun_ebpf_cmp_programs (برنامه, برنامه_loaded))
        {
          *devices_مجموعه = true;
          أرجع 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free بنية bpf_برنامه *برنامه_new = عدم;
        حرف tmp[64];
        صحيح len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, حجم (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        إذا (UNLIKELY (len >= (صحيح) حجم (tmp)))
          أرجع crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_بارگیری (برنامه, -1, tmp, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        ret = libcrun_ebpf_read_برنامه (&برنامه_new, tmp, err);
        unlink (tmp);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        إذا (libcrun_ebpf_cmp_programs (برنامه_new, برنامه_loaded))
          {
            *devices_مجموعه = true;
            أرجع 0;
          }
      }

      أرجع crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_بارگیری (برنامه, -1, مسیر, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  sd_err = sd_bus_پیام_append (m, "(sv)", property_name, "a(ss)", 1, "device", مسیر);
  إذا (UNLIKELY (sd_err < 0))
    أرجع crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_مجموعه = true;

  أرجع 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
ساكن صحيح
append_cpu_quota (sd_bus_پیام *m, حرف **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ثابت حرف *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  صحيح sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  إذا (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      صحيح ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (has_period)
        period = max_period;
    }

  إذا (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_پیام_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  إذا ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_در_هر_sec = UINT64_MAX;

      إذا (quota > 0)
        {
          quota_در_هر_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          إذا (quota_در_هر_sec % 10000)
            quota_در_هر_sec = ((quota_در_هر_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_پیام_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_در_هر_sec);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  أرجع 0;
}

ساكن صحيح
append_resources (sd_bus_پیام *m,
                  bool is_update,
                  ثابت حرف *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  صحيح cgroup_نما,
                  ثابت حرف *scope,
                  bool *devices_مجموعه,
                  libcrun_error_t *err)
{
  uint64_t مقدار;
  صحيح sd_err;
  صحيح ret;
  cleanup_free حرف *dir = عدم;
  cleanup_free حرف **missing_properties = عدم;

  *devices_مجموعه = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  missing_properties = read_dir_entries (dir, err);
  إذا (UNLIKELY (missing_properties == عدم))
    {
      إذا (crun_error_get_errno (err) != ENOENT)
        أرجع -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_مقدار(name, مقدار)                                             \
    افعل                                                                                 \
      {                                                                                \
        إذا (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_پیام_append (m, "(sv)", name, "t", مقدار);              \
            إذا (UNLIKELY (sd_err < 0))                                                 \
              أرجع crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } طالما (0)

#  define APPEND_UINT64(name, fn)              \
    افعل                                         \
      {                                        \
        ret = fn (resources, &مقدار, err);     \
        إذا (UNLIKELY (ret < 0))                \
          أرجع ret;                          \
        إذا (ret)                               \
          {                                    \
            APPEND_UINT64_مقدار (name, مقدار); \
          }                                    \
    } طالما (0)

  إذا (resources == عدم)
    أرجع 0;

  APPEND_UINT64 ("MemoryLow", get_حافظه_کم);
  APPEND_UINT64 ("MemoryMax", get_حافظه_max);
  APPEND_UINT64 ("MemorySwapMax", get_حافظه_جابجا_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  بدل (cgroup_نما)
    {
    حالة CGROUP_نما_UNIFIED:
      {
        ret = append_io_وزن (m, missing_properties, resources, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_وزن);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        إذا (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ثابت حرف *property_name = "AllowedCPUs";
            ثابت حرف *مقدار = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free حرف *allowed_cpus = عدم;
            size_t allowed_cpus_len = 0;

            إذا (مقدار == عدم)
              مقدار = resources->cpu->cpus;

            ret = cpuset_string_مقصد_bitmask (مقدار, &allowed_cpus, &allowed_cpus_len, err);
            إذا (UNLIKELY (ret < 0))
              أرجع ret;

            إذا (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                إذا (UNLIKELY (ret < 0))
                  أرجع ret;
              }
          }

        إذا (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ثابت حرف *property_name = "AllowedMemoryNodes";
            ثابت حرف *مقدار = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free حرف *allowed_mems = عدم;
            size_t allowed_mems_len = 0;

            إذا (مقدار == عدم)
              مقدار = resources->cpu->mems;

            ret = cpuset_string_مقصد_bitmask (مقدار, &allowed_mems, &allowed_mems_len, err);
            إذا (UNLIKELY (ret < 0))
              أرجع ret;

            إذا (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                إذا (UNLIKELY (ret < 0))
                  أرجع ret;
              }
          }

        ret = جمع_کن_bpf_برنامه (m, is_update, resources, missing_properties, scope, devices_مجموعه, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;
      }
      اكسر;

    حالة CGROUP_نما_LEGACY:
    حالة CGROUP_نما_HYBRID:
      إذا (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_مقدار ("CPUShares", resources->cpu->shares);
      اكسر;

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_نما);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_مقدار

  إذا (! *devices_مجموعه && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
reset_failed_unit (sd_bus *bus, ثابت حرف *unit)
{
  صحيح sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_پیام *m = عدم, *پاسخ = عدم;

  sd_err = sd_bus_پیام_new_روش_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  إذا (UNLIKELY (sd_err < 0))
    اذهب خروج;

  sd_err = sd_bus_پیام_append (m, "s", unit);
  إذا (UNLIKELY (sd_err < 0))
    اذهب خروج;

  sd_err = sd_bus_call (bus, m, 0, &error, &پاسخ);
  إذا (UNLIKELY (sd_err < 0))
    اذهب خروج;

  sd_err = 0;

خروج:
  إذا (m)
    sd_bus_پیام_unref (m);
  إذا (پاسخ)
    sd_bus_پیام_unref (پاسخ);
  sd_bus_error_free (&error);

  أرجع sd_err;
}

ساكن صحيح
verify_ebpf_device_filter_installed (ثابت حرف *cgroup_مسیر, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = عدم;
  cleanup_free حرف *پر_مسیر = عدم;
  size_t n_progs = 0;
  صحيح ret;

  ret = append_paths (&پر_مسیر, err, CGROUP_ریشه, cgroup_مسیر, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_ebpf_query_cgroup_progs (پر_مسیر, &progs, &n_progs, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (n_progs == 0)
    أرجع crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", پر_مسیر);

  أرجع 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
ساكن صحيح
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_پیام *m, sd_bus_پیام *پاسخ,
                           sd_bus_error *error, صحيح ret)
{
  إذا (bus)
    sd_bus_unref (bus);
  إذا (m)
    sd_bus_پیام_unref (m);
  إذا (پاسخ)
    sd_bus_پیام_unref (پاسخ);
  sd_bus_error_free (error);
  أرجع ret;
}

ساكن صحيح
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            صحيح cgroup_نما,
                            string_map *annotations,
                            ثابت حرف *state_ریشه,
                            ثابت حرف *scope, ثابت حرف *برش,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_مجموعه,
                            libcrun_error_t *err)
{
  sd_bus *bus = عدم;
  sd_bus_پیام *m = عدم;
  sd_bus_پیام *پاسخ = عدم;
  صحيح sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ثابت حرف *object = عدم;
  بنية systemd_کار_removed_s کار_داده = {};
  صحيح i;
  ثابت حرف *boolean_opts[10];
  cleanup_free حرف *state_dir = عدم;

  *can_retry = false;
  *devices_مجموعه = false;

  ret = libcrun_get_state_directory (&state_dir, state_ریشه, عدم, err);
  إذا (UNLIKELY (ret < 0))
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
  إذا (UNLIKELY (ret < 0))
    اذهب خروج;

  ret = systemd_check_کار_وضعیت_راهاندازی (bus, &کار_داده, err);
  إذا (UNLIKELY (ret < 0))
    اذهب خروج;

  sd_err = sd_bus_پیام_new_روش_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      اذهب خروج;
    }

  sd_err = sd_bus_پیام_append (m, "ss", scope, "fail");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      اذهب خروج;
    }

  sd_err = sd_bus_پیام_open_container (m, 'a', "(sv)");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      اذهب خروج;
    }

  إذا (برش)
    {
      sd_err = sd_bus_پیام_append (m, "(sv)", "Slice", "s", برش);
      إذا (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          اذهب خروج;
        }
    }

  sd_err = sd_bus_پیام_append (m, "(sv)", "DefaultDependencies", "b", 0);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      اذهب خروج;
    }

  إذا (annotations)
    {
      size_t پیشوند_len = حجم (SYSTEMD_PROPERTY_پیشوند) - 1;
      size_t annotations_len = string_map_اندازه (annotations);
      size_t i;

      لكل (i = 0; i < annotations_len; i++)
        {
          ثابت حرف *کلید;
          ثابت حرف *مقدار;
          size_t len;

          ret = string_map_get_at (annotations, i, &کلید, &مقدار);
          إذا (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              اذهب خروج;
            }

          إذا (! has_پیشوند (کلید, SYSTEMD_PROPERTY_پیشوند))
            استمر;

          len = طول_رشته (کلید);
          إذا (len < پیشوند_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", کلید);
              اذهب خروج;
            }

          ret = append_systemd_annotation (m, کلید + پیشوند_len, len - پیشوند_len,
                                           مقدار, err);
          إذا (UNLIKELY (ret < 0))
            اذهب خروج;
        }
    }

  sd_err = sd_bus_پیام_append (m, "(sv)", "Description", "s", "libcrun container");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      اذهب خروج;
    }

  sd_err = sd_bus_پیام_append (m, "(sv)", "PIDs", "au", 1, pid);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      اذهب خروج;
    }

  لكل (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_پیام_append (m, "(sv)", boolean_opts[i], "b", 1);
      إذا (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          اذهب خروج;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_نما, scope, devices_مجموعه, err);
  إذا (UNLIKELY (ret < 0))
    اذهب خروج;

  sd_err = sd_bus_پیام_close_container (m);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      اذهب خروج;
    }

  sd_err = sd_bus_پیام_append (m, "a(sa(sv))", 0);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      اذهب خروج;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &پاسخ);
  إذا (UNLIKELY (sd_err < 0))
    {
      إذا (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          إذا (پاسخ)
            sd_bus_پیام_unref (پاسخ);

          error = SD_BUS_ERROR_NULL;
          پاسخ = عدم;

          sd_err = sd_bus_call (bus, m, 0, &error, &پاسخ);
        }
      إذا (sd_err < 0)
        {
          إذا (sd_err == -EROFS)
            {
              ret = ثبات_missing_property_from_پیام (state_dir, error.پیام_2, err);
              إذا (UNLIKELY (ret < 0))
                اذهب خروج;
              إذا (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.پیام_2 ?: error.name);
          اذهب خروج;
        }
    }

  sd_err = sd_bus_پیام_read (پاسخ, "o", &object);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      اذهب خروج;
    }

  ret = systemd_check_کار_وضعیت (bus, &کار_داده, object, "creating", err);

خروج:
  أرجع cleanup_sd_bus_and_return (bus, m, پاسخ, &error, ret);
}

ساكن صحيح
libcrun_destroy_systemd_cgroup_scope (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت,
                                      libcrun_error_t *err)
{
  sd_bus *bus = عدم;
  sd_bus_پیام *m = عدم;
  sd_bus_پیام *پاسخ = عدم;
  صحيح ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ثابت حرف *object;
  ثابت حرف *scope = cgroup_وضعیت->scope;
  بنية systemd_کار_removed_s کار_داده = {};

  ret = open_sd_bus_connection (&bus, err);
  إذا (UNLIKELY (ret < 0))
    اذهب خروج;

  ret = systemd_check_کار_وضعیت_راهاندازی (bus, &کار_داده, err);
  إذا (UNLIKELY (ret < 0))
    اذهب خروج;

  ret = sd_bus_پیام_new_روش_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      اذهب خروج;
    }

  ret = sd_bus_پیام_append (m, "ss", scope, "replace");
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      اذهب خروج;
    }

  ret = sd_bus_call (bus, m, 0, &error, &پاسخ);
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.پیام_2 ?: error.name);
      اذهب خروج;
    }

  ret = sd_bus_پیام_read (پاسخ, "o", &object);
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      اذهب خروج;
    }

  ret = systemd_check_کار_وضعیت (bus, &کار_داده, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

خروج:
  أرجع cleanup_sd_bus_and_return (bus, m, پاسخ, &error, ret);
}

ساكن ثابت حرف *
پیدا_کن_systemd_subgroup (string_map *annotations)
{
  ثابت حرف *annotation;

  annotation = پیدا_کن_string_map_مقدار (annotations, "run.oci.systemd.subgroup");
  إذا (annotation)
    {
      إذا (annotation[0] == '\0')
        أرجع عدم;
      أرجع annotation;
    }

  أرجع "container";
}

ساكن صحيح
libcrun_cgroup_enter_systemd (بنية libcrun_cgroup_آرگومانها *آرگومانها,
                              بنية libcrun_cgroup_وضعیت *بیرون,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = آرگومانها->resources;
  ثابت حرف *cgroup_مسیر = آرگومانها->cgroup_مسیر;
  cleanup_free حرف *scope_مسیر = عدم;
  cleanup_free حرف *scope = عدم;
  cleanup_free حرف *مسیر = عدم;
  cleanup_free حرف *برش = عدم;
  صحيح retries_چپ = 32;
  ثابت حرف *suffix;
  ثابت حرف *id = آرگومانها->id;
  pid_t pid = آرگومانها->pid;
  صحيح cgroup_نما;
  صحيح rootless = 0;
  صحيح ret;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  إذا (cgroup_نما == CGROUP_نما_UNIFIED)
    {
      rootless = is_rootless (err);
      إذا (UNLIKELY (rootless < 0))
        أرجع rootless;
    }

  get_systemd_scope_and_برش (id, rootless == 1, cgroup_مسیر, &scope, &برش);

  لكل (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_نما, آرگومانها->annotations, آرگومانها->state_ریشه,
                                        scope, برش, pid, &can_retry, &بیرون->bpf_dev_مجموعه, err);
      إذا (LIKELY (ret >= 0))
        اكسر;

      إذا (can_retry && retries_چپ-- > 0)
        {
          crun_error_release (err);
          استمر;
        }

      أرجع ret;
    }

  suffix = پیدا_کن_systemd_subgroup (آرگومانها->annotations);

  ret = systemd_finalize (آرگومانها, &مسیر, cgroup_نما, suffix, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  إذا (is_خالی_string (suffix))
    scope_مسیر = xstrdup (مسیر);
  وإلا
    {
      size_t مسیر_len = طول_رشته (مسیر);
      size_t suffix_len = طول_رشته (suffix);

      scope_مسیر = strndup (مسیر, مسیر_len - suffix_len - 1);
      إذا (UNLIKELY (scope_مسیر == عدم))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_مسیر, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  إذا (بیرون->bpf_dev_مجموعه)
    {
      ret = verify_ebpf_device_filter_installed (scope_مسیر, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  بیرون->مسیر = مسیر;
  مسیر = عدم;

  بیرون->scope = scope;
  scope = عدم;
  أرجع 0;
}

حرف *
get_cgroup_scope_مسیر (ثابت حرف *cgroup_مسیر, ثابت حرف *scope)
{
  حرف *مسیر_مقصد_scope = عدم;
  حرف *cur;

  إذا (is_خالی_string (cgroup_مسیر))
    أرجع عدم;

  مسیر_مقصد_scope = xstrdup (cgroup_مسیر);

  cur = strchr (مسیر_مقصد_scope, '/');
  طالما (cur)
    {
      حرف *بعدی = strchr (cur + 1, '/');
      إذا (بعدی == عدم)
        اكسر;

      *بعدی = '\0';
      إذا (strcmp (cur, scope) == 0)
        أرجع مسیر_مقصد_scope;
      *بعدی = '/';

      cur = بعدی;
      طالما (*cur == '/')
        cur++;
    }

  أرجع مسیر_مقصد_scope;
}

ساكن صحيح
libcrun_destroy_cgroup_systemd (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت,
                                libcrun_error_t *err)
{
  cleanup_free حرف *مسیر_مقصد_scope = عدم;
  cleanup_free حرف *bpfprog = عدم;
  صحيح نما;
  صحيح ret;

  نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (نما < 0))
    أرجع نما;

  ret = cgroup_killall_مسیر (cgroup_وضعیت->مسیر, SIGKILL, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_مسیر_from_scope (&bpfprog, cgroup_وضعیت->scope, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);
  وإلا
    unlink (bpfprog); // Best effort.

  مسیر_مقصد_scope = get_cgroup_scope_مسیر (cgroup_وضعیت->مسیر, cgroup_وضعیت->scope);

  أرجع destroy_cgroup_مسیر (مسیر_مقصد_scope, نما, err);
}

ساكن صحيح
libcrun_update_resources_systemd (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت,
                                  ثابت حرف *state_ریشه,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  بنية systemd_کار_removed_s کار_داده = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free حرف *state_dir = عدم;
  sd_bus_پیام *پاسخ = عدم;
  sd_bus_پیام *m = عدم;
  sd_bus *bus = عدم;
  صحيح sd_err, ret;
  صحيح cgroup_نما;

  ret = libcrun_get_state_directory (&state_dir, state_ریشه, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  ret = open_sd_bus_connection (&bus, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = systemd_check_کار_وضعیت_راهاندازی (bus, &کار_داده, err);
  إذا (UNLIKELY (ret < 0))
    اذهب خروج;

  sd_err = sd_bus_پیام_new_روش_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      اذهب خروج;
    }

  sd_err = sd_bus_پیام_append (m, "sb", cgroup_وضعیت->scope, 1);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      اذهب خروج;
    }

  sd_err = sd_bus_پیام_open_container (m, 'a', "(sv)");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      اذهب خروج;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_نما, cgroup_وضعیت->scope, &cgroup_وضعیت->bpf_dev_مجموعه, err);
  إذا (UNLIKELY (ret < 0))
    اذهب خروج;

  sd_err = sd_bus_پیام_close_container (m);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      اذهب خروج;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &پاسخ);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.پیام_2 ?: error.name);
      اذهب خروج;
    }

  إذا (cgroup_نما != CGROUP_نما_UNIFIED)
    {
      ret = راهاندازی_rt_runtime (resources, cgroup_وضعیت->مسیر, err);
      إذا (UNLIKELY (ret < 0))
        اذهب خروج;

      ret = راهاندازی_cpuset_for_systemd_v1 (resources, cgroup_وضعیت->مسیر, err);
      إذا (UNLIKELY (ret < 0))
        اذهب خروج;
    }

  ret = راهاندازی_missing_cpu_options_for_systemd (resources, cgroup_نما == CGROUP_نما_UNIFIED, cgroup_وضعیت->مسیر, err);
  إذا (UNLIKELY (ret < 0))
    اذهب خروج;

  ret = 0;

خروج:
  أرجع cleanup_sd_bus_and_return (bus, m, پاسخ, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
ساكن صحيح
libcrun_attach_process_systemd (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت,
                                ثابت حرف *مسیر, pid_t pid,
                                libcrun_error_t *err)
{
  ثابت حرف *scope = cgroup_وضعیت->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free حرف *scope_مسیر = عدم;
  sd_bus_پیام *پاسخ = عدم;
  ثابت حرف *subcgroup;
  ثابت حرف *آخرین;
  sd_bus *bus = عدم;
  size_t len;
  صحيح sd_err, ret;

  إذا (is_خالی_string (scope))
    أرجع crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_مسیر = get_cgroup_scope_مسیر (مسیر, scope);
  إذا (scope_مسیر == عدم)
    أرجع crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  آخرین = strrchr (scope_مسیر, '/');
  إذا (آخرین == عدم || strcmp (آخرین + 1, scope) != 0)
    أرجع crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, مسیر);
  len = طول_رشته (scope_مسیر);
  subcgroup = مسیر[len] ? مسیر + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  sd_err = sd_bus_call_روش (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &پاسخ, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  إذا (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.پیام_2 ?: error.name);

  أرجع cleanup_sd_bus_and_return (bus, عدم, پاسخ, &error, ret);
}

#وإلا
ساكن صحيح
libcrun_cgroup_enter_systemd (بنية libcrun_cgroup_آرگومانها *آرگومانها,
                              بنية libcrun_cgroup_وضعیت *بیرون,
                              libcrun_error_t *err)
{
  (فراغ) آرگومانها;
  (فراغ) بیرون;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساكن صحيح
libcrun_destroy_cgroup_systemd (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت,
                                libcrun_error_t *err)
{
  (فراغ) cgroup_وضعیت;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساكن صحيح
libcrun_update_resources_systemd (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت,
                                  ثابت حرف *state_ریشه,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (فراغ) cgroup_وضعیت;
  (فراغ) state_ریشه;
  (فراغ) resources;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساكن صحيح
libcrun_attach_process_systemd (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت,
                                ثابت حرف *مسیر, pid_t pid,
                                libcrun_error_t *err)
{
  (فراغ) cgroup_وضعیت;
  (فراغ) مسیر;
  (فراغ) pid;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}
#پایان_اگر

بنية libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = عدم,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
