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

#تعریف_کرو GNU_ماخذ

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

#اگر_تعریف_شدہ HAVE_SYSTEMD
#  include <systemd/sd-bus.h>
#  include <limits.h>

#  define SYSTEMD_PROPERTY_سابقہ "org.systemd.property."

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

ساکن صحیح
رجسٹر_missing_property_from_پیغام (ثابت حرف *state_dir, ثابت حرف *پیغام_2, libcrun_error_t *err)
{
  cleanup_free حرف *file_راستہ = تہی;
  cleanup_free حرف *dir_راستہ = تہی;
  cleanup_free حرف *property = تہی;
  حرف *p;
  صحیح ret;

  اگر (! has_سابقہ (پیغام_2, "Cannot set property "))
    واپس 0;

  property = xstrdup (پیغام_2 + حجم ("Cannot set property ") - 1);
  p = strchr (property, ',');
  اگر (! p)
    واپس 0;
  *p = '\0';

  ret = append_paths (&dir_راستہ, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = crun_ensure_directory (dir_راستہ, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_paths (&file_راستہ, err, dir_راستہ, property, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_راستہ, تہی, 0, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 1;
}

ساکن bool
property_missing_p (حرف **missing_properties, ثابت حرف *property)
{
  size_t i;

  برائے (i = 0; missing_properties && missing_properties[i]; i++)
    اگر (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        واپس true;
      }

  واپس false;
}

ساکن خالی
get_systemd_scope_and_قاش (ثابت حرف *id, bool user_قاش, ثابت حرف *cgroup_راستہ,
                             حرف **scope, حرف **قاش)
{
  ثابت حرف *n;
  حرف *p;

  اگر (cgroup_راستہ == تہی || cgroup_راستہ[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      واپس;
    }

  n = strchr (cgroup_راستہ, ':');
  اگر (n == تہی)
    xasprintf (scope, "%s.scope", cgroup_راستہ);
  ورنہ
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      اگر (p)
        *p = '-';
    }
  اگر (قاش)
    {
      *قاش = xstrdup (cgroup_راستہ);
      p = strchr (*قاش, ':');
      اگر (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      اگر (is_خالی_خانے_string (*قاش))
        {
          آزاد (*قاش);
          اگر (user_قاش)
            *قاش = xstrdup ("user.slice");
          ورنہ
            *قاش = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
ساکن صحیح
تیاری_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_راستہ = تہی;
  cleanup_close صحیح dirfd = -1;
  bool need_سیٹ_والد = true;
  حرف fmt_buf[64];
  صحیح len;
  صحیح ret;

  اگر (resources == تہی || resources->cpu == تہی)
    واپس 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  اگر (is_خالی_خانے_string (راستہ))
    واپس crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  اگر (has_suffix (راستہ, ".scope"))
    need_سیٹ_والد = false;

  ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, "cpu", راستہ, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = crun_ensure_directory (cgroup_راستہ, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  dirfd = open (cgroup_راستہ, O_DIRECTORY | O_راستہ | O_CLOEXEC);
  اگر (UNLIKELY (dirfd < 0))
    واپس crun_make_error (err, errno, "open `%s`", cgroup_راستہ);

  اگر (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      اگر (need_سیٹ_والد)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      اگر (UNLIKELY (len >= (صحیح) حجم (fmt_buf)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      اگر (need_سیٹ_والد)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس 0;
}

ساکن صحیح
تیاری_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_راستہ = تہی;
  صحیح والد_2;
  صحیح ret;

  اگر (resources == تہی || resources->cpu == تہی)
    واپس 0;

  اگر (! resources->cpu->مدت_present && ! (cgroup2 && resources->cpu->idle_present))
    واپس 0;

  برائے (والد_2 = 0; والد_2 < 2; والد_2++)
    {
      cleanup_close صحیح dirfd = -1;

      اگر (cgroup2)
        ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, راستہ ? راستہ : "", (والد_2 ? ".." : تہی), تہی);
      ورنہ
        ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, "/cpu", راستہ ? راستہ : "", (والد_2 ? ".." : تہی), تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      dirfd = open (cgroup_راستہ, O_DIRECTORY | O_راستہ | O_CLOEXEC);
      اگر (UNLIKELY (dirfd < 0))
        واپس crun_make_error (err, errno, "open `%s`", cgroup_راستہ);

      ret = write_cpu_مدت (dirfd, cgroup2, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      اگر (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }

  واپس 0;
}

ساکن صحیح
تیاری_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_راستہ = تہی;
  صحیح والد_2;
  صحیح ret;
  صحیح سطحیں;

  ret = append_paths (&cgroup_راستہ, err, CGROUP_جڑ, "/cpuset", راستہ ? راستہ : "", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = crun_ensure_directory (cgroup_راستہ, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_راستہ, resources, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  سطحیں = has_suffix (راستہ, ".scope") ? 1 : 2;

  برائے (والد_2 = 0; والد_2 < سطحیں; والد_2++)
    {
      cleanup_close صحیح dirfd_cpuset = -1;
      cleanup_free حرف *راستہ_مقصد_cpuset = تہی;

      ret = append_paths (&راستہ_مقصد_cpuset, err, CGROUP_جڑ "/cpuset", راستہ, (والد_2 ? ".." : تہی), تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      dirfd_cpuset = open (راستہ_مقصد_cpuset, O_DIRECTORY | O_راستہ | O_CLOEXEC);
      اگر (UNLIKELY (dirfd_cpuset < 0))
        واپس crun_make_error (err, errno, "open `%s`", راستہ_مقصد_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
ساکن صحیح
compute_finalized_راستہ (حرف **راستہ, ثابت حرف *from, ثابت حرف *suffix, libcrun_error_t *err)
{
  اگر (suffix == تہی)
    {
      *راستہ = xstrdup (from);
      واپس 0;
    }
  واپس append_paths (راستہ, err, from, suffix, تہی);
}

ساکن صحیح
systemd_finalize (ساخت libcrun_cgroup_دلائل *دلائل, حرف **راستہ_برآمد,
                  صحیح cgroup_منوال, ثابت حرف *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = دلائل->resources;
  cleanup_free حرف *content = تہی;
  cleanup_free حرف *راستہ = تہی;
  pid_t pid = دلائل->pid;
  صحیح ret;
  حرف *from, *مقصد;
  حرف *saveptr = تہی;

  cleanup_free حرف *proc_راستہ = تہی;
  cleanup_close صحیح fd = -1;

  xasprintf (&proc_راستہ, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (دلائل->container, proc_راستہ, O_RDONLY, err);
  اگر (UNLIKELY (fd < 0))
    واپس fd;

  ret = read_all_fd (fd, "cgroup path", &content, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  انتخاب (cgroup_منوال)
    {
    حالت CGROUP_منوال_HYBRID:
    حالت CGROUP_منوال_LEGACY:
      from = لڑی_تلاش (content, ":memory:");
      اگر (LIKELY (from != تہی))
        from += 8;
      ورنہ
        {
          from = لڑی_تلاش (content, ":pids:");
          اگر (UNLIKELY (from == تہی))
            واپس crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      مقصد = strchr (from, '\n');
      اگر (UNLIKELY (مقصد == تہی))
        واپس crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *مقصد = '\0';
      ret = compute_finalized_راستہ (&راستہ, from, suffix, err);
      *مقصد = '\n';
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (geteuid ())
        واپس 0;

      ret = تیاری_rt_runtime (resources, راستہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = تیاری_cpuset_for_systemd_v1 (resources, راستہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      برائے (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (تہی, "\n", &saveptr))
        {
          حرف *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          اگر (subsystem[0] == '\0')
            {
              اگر (cgroup_منوال == CGROUP_منوال_LEGACY)
                جاری;

              subsystem = "unified";
            }

          اگر (strcmp (subpath, راستہ))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, راستہ, true, err);
              اگر (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  اگر (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      جاری;
                    }
                  واپس ret;
                }
            }
        }

      توڑو;

    حالت CGROUP_منوال_UNIFIED:
      {
        cleanup_free حرف *dir = تہی;

        from = لڑی_تلاش (content, "0::");
        اگر (UNLIKELY (from == تہی))
          واپس crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        مقصد = strchr (from, '\n');
        اگر (UNLIKELY (مقصد == تہی))
          واپس crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *مقصد = '\0';
        ret = compute_finalized_راستہ (&راستہ, from, suffix, err);
        *مقصد = '\n';
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        ret = append_paths (&dir, err, CGROUP_جڑ, راستہ, تہی);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        ret = move_process_مقصد_cgroup (pid, تہی, راستہ, err);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        ret = enable_controllers (راستہ, err);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        اگر (suffix)
          {
            ret = chown_cgroups (راستہ, دلائل->جڑ_uid, دلائل->جڑ_gid, err);
            اگر (UNLIKELY (ret < 0))
              واپس ret;
          }
      }
      توڑو;

    طےشدہ:
      واپس crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_منوال);
    }

  ret = تیاری_missing_cpu_options_for_systemd (resources, cgroup_منوال == CGROUP_منوال_UNIFIED, راستہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  *راستہ_برآمد = راستہ;
  راستہ = تہی;

  واپس 0;
}

ساخت systemd_کام_removed_s
{
  ثابت حرف *راستہ;
  ثابت حرف *op;
  صحیح terminated;
  libcrun_error_t err;
};

ساکن صحیح
systemd_کام_removed (sd_bus_پیغام *m, خالی *userdata, sd_bus_error *error arg_غیر_مستعمل)
{
  ثابت حرف *راستہ, *unit, *نتیجہ;
  uint32_t id;
  صحیح ret;
  ساخت systemd_کام_removed_s *d = userdata;

  ret = sd_bus_پیغام_read (m, "uoss", &id, &راستہ, &unit, &نتیجہ);
  اگر (ret < 0)
    واپس -1;

  اگر (strcmp (d->راستہ, راستہ) == 0)
    {
      d->terminated = 1;
      اگر (strcmp (نتیجہ, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, نتیجہ);
          واپس -1;
        }
    }
  واپس 0;
}

ساکن صحیح
systemd_check_کام_حالت_تیاری (sd_bus *bus, ساخت systemd_کام_removed_s *ڈیٹا, libcrun_error_t *err)
{
  صحیح ret;

  ret = sd_bus_مطابقت_signal_async (bus, تہی, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_کام_removed, تہی, ڈیٹا);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, -ret, "sd-bus match signal");

  واپس 0;
}

ساکن صحیح
systemd_check_کام_حالت (sd_bus *bus, ساخت systemd_کام_removed_s *ڈیٹا, ثابت حرف *راستہ, ثابت حرف *op,
                          libcrun_error_t *err)
{
  صحیح sd_err;

  ڈیٹا->راستہ = راستہ;
  ڈیٹا->op = op;
  جب (! ڈیٹا->terminated)
    {
      sd_err = sd_bus_process (bus, تہی);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus process");

      اگر (sd_err != 0)
        جاری;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus wait");
    }

  اگر (ڈیٹا->err != تہی)
    {
      *err = ڈیٹا->err;
      واپس -1;
    }

  واپس 0;
}

صحیح
parse_sd_array (حرف *s, حرف **برآمد, حرف **اگلا, libcrun_error_t *err)
{
  حرف endchr;
  حرف *it, *dest;
  bool escaped = false;

  *برآمد = تہی;
  *اگلا = تہی;

  جب (خلا_ہے (*s))
    s++;
  اگر (*s == '\0')
    واپس 0;
  ورنہ اگر (*s != '\'' && *s != '"')
    واپس crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *برآمد = dest = it;

  جب (1)
    {
      اگر (*it == '\0')
        واپس crun_make_error (err, 0, "invalid string `%s`", s);
      اگر (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          جب (خلا_ہے (*it))
            it++;
          اگر (*it == ',')
            {
              *اگلا = ++it;
              *dest = '\0';
              واپس 0;
            }

          اگر (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              واپس 0;
            }

          واپس crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      اگر (! escaped)
        *dest++ = *it;
      it++;
    }

  واپس 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
ساکن صحیح
append_systemd_annotation (sd_bus_پیغام *m, ثابت حرف *name, size_t name_len, ثابت حرف *قدر,
                           libcrun_error_t *err)
{
  cleanup_free حرف *tmp_name = تہی;
  uint32_t ضربی_عدد = 1;
  ثابت حرف *it;
  صحیح sd_err;

  جب (*قدر == ' ')
    قدر++;

  it = قدر;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  اگر (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
      && name[name_len - 1] == 'c')
    {
      ضربی_عدد = 1000000;

      tmp_name = xmalloc (name_len + 2);
      حافظہ_نقل (tmp_name, name, name_len - 3);
      حافظہ_نقل (tmp_name + name_len - 3, "USec", 5);

      name = tmp_name;
    }

  /* A *USec property is always an uint64, so, like runc does, accept any
     number of seconds (possibly with a fractional part, and with an
     optional integer type), and convert it to uint64 microseconds.  */
  اگر (ضربی_عدد != 1)
    {
      ساکن ثابت حرف *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", تہی };
      ثابت حرف *عدد = it;
      حرف *endptr = تہی;
      دوہرا v;
      size_t i;

      برائے (i = 0; types[i]; i++)
        اگر (has_سابقہ (عدد, types[i]))
          {
            عدد += لڑی_لمبائی (types[i]);
            توڑو;
          }

      errno = 0;
      v = strtod (عدد, &endptr);
      اگر (UNLIKELY (errno != 0 || endptr == عدد || *endptr || v < 0 || v * ضربی_عدد >= 18446744073709551615.0))
        واپس crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, قدر);

      sd_err = sd_bus_پیغام_append (m, "(sv)", name, "t", (uint64_t) (v * ضربی_عدد));
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      واپس 0;
    }

  اگر ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_پیغام_append (m, "(sv)", name, "b", b);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      واپس 0;
    }
  ورنہ اگر (*it == '\'')
    {
      cleanup_free حرف *v_آغاز = تہی;
      حرف *انتہا;

      it = v_آغاز = xstrdup (قدر);

      انتہا = strchr (v_آغاز + 1, '\'');
      اگر (انتہا == تہی)
        واپس crun_make_error (err, 0, "invalid variant `%s`", قدر);
      *انتہا = '\0';

      sd_err = sd_bus_پیغام_append (m, "(sv)", name, "s", it + 1);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      واپس 0;
    }
  ورنہ اگر (*it == '[')
    {
      cleanup_free حرف *v_آغاز = تہی;
      size_t n_parts = 0, parts_سائز = 32;
      cleanup_free حرف **parts = xmalloc (حجم (حرف *) * parts_سائز);
      حرف *part;

      part = v_آغاز = xstrdup (it + 1);
      جب (1)
        {
          حرف *برآمد = تہی, *اگلا = تہی;
          صحیح ret;

          ret = parse_sd_array (part, &برآمد, &اگلا, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          parts[n_parts++] = برآمد;
          اگر (n_parts == parts_سائز - 1)
            {
              parts_سائز += 32;
              parts = xrealloc (parts, حجم (حرف *) * parts_سائز);
            }
          parts[n_parts] = تہی;
          اگر (اگلا == تہی)
            توڑو;

          part = اگلا;
        }

      sd_err = sd_bus_پیغام_open_container (m, 'r', "sv");
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_پیغام_append (m, "s", name);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_پیغام_open_container (m, 'v', "as");
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_پیغام_append_strv (m, parts);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_پیغام_close_container (m);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_پیغام_close_container (m);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus close container");

      واپس 0;
    }
  ورنہ اگر (has_سابقہ (it, "uint64 "))
    {
      حرف *endptr = تہی;
      uint64_t v;

      errno = 0;
      v = strtoull (it + حجم ("uint64"), &endptr, 10);
      اگر (UNLIKELY (errno != 0 || *endptr))
        واپس crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پیغام_append (m, "(sv)", name, "t", (uint64_t) (v * ضربی_عدد));
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      واپس 0;
    }
  ورنہ اگر (has_سابقہ (it, "int64 "))
    {
      حرف *endptr = تہی;
      int64_t v;

      errno = 0;
      v = strtoll (it + حجم ("int64"), &endptr, 10);
      اگر (UNLIKELY (errno != 0 || *endptr))
        واپس crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پیغام_append (m, "(sv)", name, "x", (int64_t) (v * ضربی_عدد));
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      واپس 0;
    }
  ورنہ اگر (has_سابقہ (it, "uint32 "))
    {
      حرف *endptr = تہی;
      uint32_t v;

      errno = 0;
      v = strtoul (it + حجم ("uint32"), &endptr, 10);
      اگر (UNLIKELY (errno != 0 || *endptr))
        واپس crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پیغام_append (m, "(sv)", name, "u", (uint32_t) (v * ضربی_عدد));
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      واپس 0;
    }
  ورنہ اگر (has_سابقہ (it, "int32 ") || strchr (it, ' ') == تہی)
    {
      حرف *endptr = تہی;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      اگر (has_سابقہ (it, "int32 "))
        v = strtol (it + حجم ("int32"), &endptr, 10);
      ورنہ
        v = strtol (it, &endptr, 10);
      اگر (UNLIKELY (errno != 0 || *endptr))
        واپس crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پیغام_append (m, "(sv)", name, "i", (int32_t) (v * ضربی_عدد));
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      واپس 0;
    }

  واپس crun_make_error (err, 0, "unknown type for `%s`", name);
}

ساکن صحیح
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  صحیح rootless;
  صحیح sd_err = 0;

  rootless = is_rootless (err);
  اگر (UNLIKELY (rootless < 0))
    واپس rootless;

  اگر (rootless)
    sd_err = sd_bus_default_user (bus);
  اگر (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  اگر (sd_err < 0)
    واپس crun_make_error (err, -sd_err, "cannot open sd-bus");

  واپس 0;
}

ساکن ثابت حرف *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *name)
{
  size_t i;

  اگر (resources == تہی || resources->unified == تہی)
    واپس تہی;

  برائے (i = 0; i < resources->unified->len; i++)
    اگر (strcmp (resources->unified->کلیدیں[i], name) == 0)
      واپس resources->unified->اقدار[i];
  واپس تہی;
}

ساکن صحیح
get_قدر_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *name,
                            uint64_t *قدر, libcrun_error_t *err)
{
  حرف *endptr = تہی;
  size_t i;

  اگر (resources == تہی || resources->unified == تہی)
    واپس 0;

  برائے (i = 0; i < resources->unified->len; i++)
    اگر (strcmp (resources->unified->کلیدیں[i], name) == 0)
      {
        اگر (strcmp (resources->unified->اقدار[i], "max") == 0)
          {
            *قدر = UINT64_MAX;
            واپس 1;
          }

        errno = 0;
        *قدر = strtoull (resources->unified->اقدار[i], &endptr, 10);
        اگر (UNLIKELY (errno))
          واپس crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->اقدار[i]);
        اگر (endptr && *endptr)
          واپس crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->اقدار[i]);
        واپس 1;
      }
  واپس 0;
}

ساکن اندرونی صحیح
get_حافظہ_کم (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد_بندی, libcrun_error_t *err)
{
  اگر (resources->حافظہ && resources->حافظہ->reservation_present)
    {
      *حد_بندی = resources->حافظہ->reservation;
      واپس 1;
    }

  واپس get_قدر_from_unified_map (resources, "memory.low", حد_بندی, err);
}

ساکن اندرونی صحیح
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد_بندی, libcrun_error_t *err)
{
  اگر (resources->pids && resources->pids->حد_بندی)
    {
      *حد_بندی = resources->pids->حد_بندی;
      واپس 1;
    }

  واپس get_قدر_from_unified_map (resources, "pids.max", حد_بندی, err);
}

ساکن اندرونی صحیح
get_حافظہ_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد_بندی, libcrun_error_t *err)
{
  اگر (resources->حافظہ && resources->حافظہ->حد_بندی_present)
    {
      *حد_بندی = resources->حافظہ->حد_بندی;
      واپس 1;
    }

  واپس get_قدر_from_unified_map (resources, "memory.max", حد_بندی, err);
}

ساکن اندرونی صحیح
get_حافظہ_تبادلہ_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد_بندی, libcrun_error_t *err)
{
  اگر (resources->حافظہ && resources->حافظہ->تبادلہ_present)
    {
      runtime_spec_schema_config_linux_resources_حافظہ *حافظہ = resources->حافظہ;
      int64_t mem = حافظہ->حد_بندی_present ? حافظہ->حد_بندی : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      اگر (حافظہ->تبادلہ == -1 || (mem == -1 && حافظہ->تبادلہ == 0))
        {
          *حد_بندی = UINT64_MAX;
          واپس 1;
        }
      /* 0 means unset.  */
      اگر (حافظہ->تبادلہ == 0)
        واپس 0;
      اگر (mem <= 0)
        واپس crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      اگر (حافظہ->تبادلہ < mem)
        واپس crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *حد_بندی = حافظہ->تبادلہ - mem;
      واپس 1;
    }

  واپس get_قدر_from_unified_map (resources, "memory.swap.max", حد_بندی, err);
}

ساکن اندرونی صحیح
get_cpu_وزن (runtime_spec_schema_config_linux_resources *resources, uint64_t *وزن, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t قدر;
  صحیح ret;

  ret = get_قدر_from_unified_map (resources, "cpu.idle", &قدر, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  اگر (ret > 0 && قدر == 1)
    has_idle = true;
  اگر (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_قدر_from_unified_map (resources, "cpu.weight", وزن, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  اگر (ret > 0)
    {
      اگر (has_idle)
        واپس crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      واپس 1;
    }

  اگر (resources->cpu && resources->cpu->shares_present)
    {
      اگر (has_idle)
        واپس crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      اگر (resources->cpu->shares == 0)
        واپس 0;
      *وزن = convert_shares_مقصد_وزن (resources->cpu->shares);
      واپس 1;
    }
  اگر (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *وزن = 0;
      واپس 1;
    }
  واپس 0;
}

ساکن صحیح
read_uint64_قدر (ثابت حرف *str, uint64_t *قدر, libcrun_error_t *err)
{
  حرف *endptr = تہی;

  errno = 0;
  *قدر = strtoull (str, &endptr, 10);
  اگر (UNLIKELY (errno))
    واپس crun_make_error (err, errno, "invalid value: %s", str);
  اگر (endptr && *endptr)
    واپس crun_make_error (err, 0, "invalid value: %s", str);
  واپس 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
ساکن اندرونی uint64_t
IO_وزن (uint64_t bfq_وزن)
{
  واپس bfq_وزن <= CGROUP_BFQ_وزن_DEFAULT ? CGROUP_وزن_DEFAULT - (CGROUP_BFQ_وزن_DEFAULT - bfq_وزن) * (CGROUP_وزن_DEFAULT - CGROUP_وزن_MIN) / (CGROUP_BFQ_وزن_DEFAULT - CGROUP_BFQ_وزن_MIN) : CGROUP_وزن_DEFAULT + (bfq_وزن - CGROUP_BFQ_وزن_DEFAULT) * (CGROUP_وزن_MAX - CGROUP_وزن_DEFAULT) / (CGROUP_BFQ_وزن_MAX - CGROUP_BFQ_وزن_DEFAULT);
}

ساکن صحیح
append_io_وزن (sd_bus_پیغام *m, حرف **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t وزن;
  صحیح sd_err;
  size_t i;
  صحیح ret;

#  define APPEND_IO_وزن(قدر)                                                      \
    کرو                                                                                 \
      {                                                                                \
        اگر (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_پیغام_append (m, "(sv)", "IOWeight", "t", وزن);       \
            اگر (UNLIKELY (sd_err < 0))                                                 \
              واپس crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } جب (0)

#  define APPEND_IO_DEVICE_وزن(device, قدر)                                                      \
    کرو                                                                                                \
      {                                                                                               \
        اگر (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_پیغام_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, وزن); \
            اگر (UNLIKELY (sd_err < 0))                                                                \
              واپس crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } جب (0)

  اگر (resources->بلاک_io && resources->بلاک_io->وزن_present)
    {
      وزن = IO_وزن (resources->بلاک_io->وزن);
      APPEND_IO_وزن (وزن);
    }

  برائے (i = 0; resources->بلاک_io && i < resources->بلاک_io->وزن_device_len; i++)
    {
      حرف name[64];
      صحیح len;

      len = snprintf (name, حجم (name), "%" PRIu64 ":%" PRIu64,
                      resources->بلاک_io->وزن_device[i]->major,
                      resources->بلاک_io->وزن_device[i]->minor);
      اگر (UNLIKELY (len >= (صحیح) حجم (name)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      وزن = IO_وزن (resources->بلاک_io->وزن_device[i]->وزن);
      APPEND_IO_DEVICE_وزن (name, وزن);
    }

  برائے (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free حرف *قدر = تہی;
      bool need_conversion = false;
      حرف *saveptr = تہی;
      حرف *سطر;

      اگر (strcmp (resources->unified->کلیدیں[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      ورنہ اگر (strcmp (resources->unified->کلیدیں[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      ورنہ
        {
          /* Skip other keys.  */
          جاری;
        }

      قدر = xstrdup (resources->unified->اقدار[i]);

      برائے (سطر = strtok_r (قدر, "\n", &saveptr); سطر; سطر = strtok_r (تہی, "\n", &saveptr))
        {
          حرف *sep;

          sep = strchr (سطر, ' ');
          جب (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_قدر (sep ? sep : سطر, &وزن, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          اگر (need_conversion)
            وزن = IO_وزن (وزن);

          اگر (sep == تہی || strcmp (سطر, "default") == 0)
            APPEND_IO_وزن (وزن);
          ورنہ
            APPEND_IO_DEVICE_وزن (سطر, وزن);
        }
    }

  واپس 0;
}

/* Adapted from systemd.  */
ساکن صحیح
bus_append_byte_array (sd_bus_پیغام *m, ثابت حرف *میدان, ثابت خالی *buf, size_t n, libcrun_error_t *err)
{
  صحیح ret;

  ret = sd_bus_پیغام_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  اگر (ret < 0)
    واپس crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_پیغام_append_basic (m, SD_BUS_TYPE_STRING, میدان);
  اگر (ret < 0)
    واپس crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_پیغام_open_container (m, 'v', "ay");
  اگر (ret < 0)
    واپس crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_پیغام_append_array (m, 'y', buf, n);
  اگر (ret < 0)
    واپس crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_پیغام_close_container (m);
  اگر (ret < 0)
    واپس crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_پیغام_close_container (m);
  اگر (ret < 0)
    واپس crun_make_error (err, -ret, "sd_bus_message_close_container");

  واپس 1;
}

ساکن صحیح
append_uint64_from_unified_map (sd_bus_پیغام *m,
                                حرف **missing_properties,
                                ثابت حرف *attr,
                                ثابت حرف *کلید,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t قدر;
  صحیح ret;

  اگر (property_missing_p (missing_properties, attr))
    واپس 0;

  ret = get_قدر_from_unified_map (resources, کلید, &قدر, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  اگر (ret)
    {
      صحیح sd_err = sd_bus_پیغام_append (m, "(sv)", attr, "t", قدر);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  واپس 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
ساکن صحیح
append_device_allow (sd_bus_پیغام *m,
                     ثابت حرف type,
                     صحیح major,
                     صحیح minor,
                     ثابت حرف *access,
                     libcrun_error_t *err)
{
  حرف device[64];
  صحیح sd_err;
  صحیح len;

  اگر (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      واپس 0;
    }

  اگر (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, حجم (device), "%s-*", type == 'c' ? "char" : "block");
  ورنہ اگر (IS_WILDCARD (minor))
    len = snprintf (device, حجم (device), type == 'c' ? "char-%d" : "block-%d", major);
  ورنہ
    len = snprintf (device, حجم (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  اگر (UNLIKELY (len >= (صحیح) حجم (device)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_پیغام_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  اگر (UNLIKELY (sd_err < 0))
    واپس crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  واپس 0;
}

#  define منوال_R 1
#  define منوال_W 2
#  define منوال_M 4

ساکن صحیح
access_مقصد_منوال (ثابت حرف *access, libcrun_error_t *err)
{
  صحیح منوال = 0;

  جب (access && *access)
    {
      صحیح c = *access++;

      انتخاب (c)
        {
        حالت 'r':
          منوال |= منوال_R;
          توڑو;

        حالت 'w':
          منوال |= منوال_W;
          توڑو;

        حالت 'm':
          منوال |= منوال_M;
          توڑو;

        طےشدہ:
          واپس crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  واپس منوال;
}

/* check if the type b is included in type a.  */
ساکن bool
has_same_type (ثابت حرف *a, ثابت حرف *b)
{
  bool a_all = a == تہی || strcmp (a, "a") == 0;
  اگر (b == تہی || strcmp (b, "a") == 0)
    واپس a_all;

  واپس a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
ساکن صحیح
is_deny_قاعدہ_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  صحیح req_منوال;

#  define CMP_DEV_عدد(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_منوال = access_مقصد_منوال (devices[n]->access, err);
  اگر (UNLIKELY (req_منوال < 0))
    واپس req_منوال;

  برائے (i = n; i > 0; i--)
    {
      size_t current_قاعدہ = i - 1;
      صحیح منوال;

      اگر (! CMP_DEV_عدد (devices[n]->minor, devices[current_قاعدہ]->minor))
        جاری;
      اگر (! CMP_DEV_عدد (devices[n]->major, devices[current_قاعدہ]->major))
        جاری;

      اگر (! has_same_type (devices[n]->type, devices[current_قاعدہ]->type)
          && has_same_type ("a", devices[current_قاعدہ]->type))
        جاری;

      منوال = access_مقصد_منوال (devices[i]->access, err);
      اگر (UNLIKELY (منوال < 0))
        واپس منوال;

      اگر (devices[current_قاعدہ]->allow && (منوال & req_منوال))
        واپس 0;

      req_منوال &= ! منوال;

      /* no more access bits to validate.  */
      اگر (req_منوال == 0)
        واپس 1;
    }

  واپس 0;
#  undef CMP_DEV_عدد
}

ساکن size_t
ڈھونڈو_پہلا_قاعدہ_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  اگر (n == 0)
    واپس 1;

  /* Find the first rule that is after the last "block all".  */
  برائے (i = n - 1; i-- > 0;)
    {
      اگر ((is_خالی_خانے_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        واپس i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  برائے (i = 0; i < n; i++)
    اگر (devices[i]->allow)
      واپس i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  واپس n + 1;
}

ساکن bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_بلاک 1
#  define DEV_TYPE_CHAR 2
  صحیح بقایا = DEV_TYPE_بلاک | DEV_TYPE_CHAR;
  size_t i;

  برائے (i = 0; i < n; i++)
    {
      size_t current_قاعدہ = n - i - 1;
      اگر (! devices[current_قاعدہ]->allow)
        واپس false;

      اگر (IS_WILDCARD (devices[current_قاعدہ]->major) && IS_WILDCARD (devices[current_قاعدہ]->minor))
        {
          اگر (is_خالی_خانے_string (devices[current_قاعدہ]->type) || strcmp (devices[current_قاعدہ]->type, "a") == 0)
            واپس true;

          اگر (strcmp (devices[current_قاعدہ]->type, "c") == 0)
            بقایا &= ~DEV_TYPE_CHAR;
          ورنہ اگر (strcmp (devices[current_قاعدہ]->type, "b") == 0)
            بقایا &= ~DEV_TYPE_بلاک;

          اگر (بقایا == 0)
            واپس true;
        }
    }
#  undef DEV_TYPE_بلاک
#  undef DEV_TYPE_CHAR

  واپس false;
}

ساکن صحیح
append_devices (sd_bus_پیغام *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  ساخت default_dev_s *default_devices = get_default_devices ();
  صحیح ret, sd_err;
  size_t i;

  اگر (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_پیغام_append (m, "(sv)", "DevicePolicy", "s", "auto");
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_پیغام_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      واپس 0;
    }

  sd_err = sd_bus_پیغام_append (m, "(sv)", "DevicePolicy", "s", "strict");
  اگر (UNLIKELY (sd_err < 0))
    واپس crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  برائے (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  برائے (i = ڈھونڈو_پہلا_قاعدہ_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      حرف type;

      اگر (! d->allow)
        {
          صحیح redundant;

          redundant = is_deny_قاعدہ_redundant (resources->devices, i, err);
          اگر (UNLIKELY (redundant < 0))
            واپس redundant;

          اگر (redundant)
            جاری;

          واپس crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      اگر (d->type == تہی || strcmp (d->type, "a") == 0)
        type = 'a';
      ورنہ اگر (strcmp (d->type, "c") == 0)
        type = 'c';
      ورنہ اگر (strcmp (d->type, "b") == 0)
        type = 'b';
      ورنہ
        واپس crun_make_error (err, 0, "unknown device type `%s`", d->type);

      اگر (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
      ورنہ
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }

  واپس 0;
}

ساکن bool
has_bpf_fs ()
{
  ساخت statfs stat;

  واپس statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

ساکن صحیح
bpfprog_راستہ_from_scope (حرف **راستہ, ثابت حرف *scope, libcrun_error_t *err)
{
  cleanup_free حرف *flat_scope = xstrdup (scope);
  حرف *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  جب ((it = strchr (it, '.')) != تہی)
    *it = '_';

  واپس append_paths (راستہ, err, CRUN_BPF_DIR, flat_scope, تہی);
}

ساکن صحیح
جوڑو_bpf_پروگرام (sd_bus_پیغام *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 حرف **missing_properties,
                 ثابت حرف *scope,
                 bool *devices_سیٹ,
                 libcrun_error_t *err)
{
  صحیح ret = 0;
  صحیح اندر_userns, sd_err;
  ثابت حرف *property_name = "BPFProgram";
  cleanup_free ساخت bpf_پروگرام *پروگرام = تہی;
  cleanup_free حرف *راستہ = تہی;

  *devices_سیٹ = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  اگر (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    واپس 0;

  اندر_userns = check_running_اندر_user_namespace (err);
  اگر (UNLIKELY (اندر_userns < 0))
    واپس اندر_userns;

  اگر (اندر_userns)
    واپس 0;

  پروگرام = create_dev_bpf (resources->devices, resources->devices_len, err);
  اگر (UNLIKELY (پروگرام == تہی))
    {
      crun_error_release (err);
      واپس 0;
    }

  ret = bpfprog_راستہ_from_scope (&راستہ, scope, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  اگر (UNLIKELY (ret < 0 && errno != EEXIST))
    واپس crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  اگر (is_update)
    {
      cleanup_free ساخت bpf_پروگرام *پروگرام_loaded = تہی;

      اگر (resources->devices == تہی || resources->devices_len == 0)
        {
          *devices_سیٹ = true;
          واپس 0;
        }

      ret = libcrun_ebpf_read_پروگرام (&پروگرام_loaded, راستہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      /* it is the same as the loaded program.  */
      اگر (libcrun_ebpf_cmp_programs (پروگرام, پروگرام_loaded))
        {
          *devices_سیٹ = true;
          واپس 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free ساخت bpf_پروگرام *پروگرام_new = تہی;
        حرف tmp[64];
        صحیح len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, حجم (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        اگر (UNLIKELY (len >= (صحیح) حجم (tmp)))
          واپس crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_بھرو (پروگرام, -1, tmp, err);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        ret = libcrun_ebpf_read_پروگرام (&پروگرام_new, tmp, err);
        unlink (tmp);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        اگر (libcrun_ebpf_cmp_programs (پروگرام_new, پروگرام_loaded))
          {
            *devices_سیٹ = true;
            واپس 0;
          }
      }

      واپس crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_بھرو (پروگرام, -1, راستہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  sd_err = sd_bus_پیغام_append (m, "(sv)", property_name, "a(ss)", 1, "device", راستہ);
  اگر (UNLIKELY (sd_err < 0))
    واپس crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_سیٹ = true;

  واپس 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
ساکن صحیح
append_cpu_quota (sd_bus_پیغام *m, حرف **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ثابت حرف *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  صحیح sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  اگر (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      صحیح ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      اگر (has_period)
        period = max_period;
    }

  اگر (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_پیغام_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  اگر ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_فی_sec = UINT64_MAX;

      اگر (quota > 0)
        {
          quota_فی_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          اگر (quota_فی_sec % 10000)
            quota_فی_sec = ((quota_فی_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_پیغام_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_فی_sec);
      اگر (UNLIKELY (sd_err < 0))
        واپس crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  واپس 0;
}

ساکن صحیح
append_resources (sd_bus_پیغام *m,
                  bool is_update,
                  ثابت حرف *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  صحیح cgroup_منوال,
                  ثابت حرف *scope,
                  bool *devices_سیٹ,
                  libcrun_error_t *err)
{
  uint64_t قدر;
  صحیح sd_err;
  صحیح ret;
  cleanup_free حرف *dir = تہی;
  cleanup_free حرف **missing_properties = تہی;

  *devices_سیٹ = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  missing_properties = read_dir_entries (dir, err);
  اگر (UNLIKELY (missing_properties == تہی))
    {
      اگر (crun_error_get_errno (err) != ENOENT)
        واپس -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_قدر(name, قدر)                                             \
    کرو                                                                                 \
      {                                                                                \
        اگر (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_پیغام_append (m, "(sv)", name, "t", قدر);              \
            اگر (UNLIKELY (sd_err < 0))                                                 \
              واپس crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } جب (0)

#  define APPEND_UINT64(name, fn)              \
    کرو                                         \
      {                                        \
        ret = fn (resources, &قدر, err);     \
        اگر (UNLIKELY (ret < 0))                \
          واپس ret;                          \
        اگر (ret)                               \
          {                                    \
            APPEND_UINT64_قدر (name, قدر); \
          }                                    \
    } جب (0)

  اگر (resources == تہی)
    واپس 0;

  APPEND_UINT64 ("MemoryLow", get_حافظہ_کم);
  APPEND_UINT64 ("MemoryMax", get_حافظہ_max);
  APPEND_UINT64 ("MemorySwapMax", get_حافظہ_تبادلہ_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  انتخاب (cgroup_منوال)
    {
    حالت CGROUP_منوال_UNIFIED:
      {
        ret = append_io_وزن (m, missing_properties, resources, err);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_وزن);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        اگر (UNLIKELY (ret < 0))
          واپس ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        اگر (UNLIKELY (ret < 0))
          واپس ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        اگر (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ثابت حرف *property_name = "AllowedCPUs";
            ثابت حرف *قدر = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free حرف *allowed_cpus = تہی;
            size_t allowed_cpus_len = 0;

            اگر (قدر == تہی)
              قدر = resources->cpu->cpus;

            ret = cpuset_string_مقصد_bitmask (قدر, &allowed_cpus, &allowed_cpus_len, err);
            اگر (UNLIKELY (ret < 0))
              واپس ret;

            اگر (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                اگر (UNLIKELY (ret < 0))
                  واپس ret;
              }
          }

        اگر (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ثابت حرف *property_name = "AllowedMemoryNodes";
            ثابت حرف *قدر = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free حرف *allowed_mems = تہی;
            size_t allowed_mems_len = 0;

            اگر (قدر == تہی)
              قدر = resources->cpu->mems;

            ret = cpuset_string_مقصد_bitmask (قدر, &allowed_mems, &allowed_mems_len, err);
            اگر (UNLIKELY (ret < 0))
              واپس ret;

            اگر (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                اگر (UNLIKELY (ret < 0))
                  واپس ret;
              }
          }

        ret = جوڑو_bpf_پروگرام (m, is_update, resources, missing_properties, scope, devices_سیٹ, err);
        اگر (UNLIKELY (ret < 0))
          واپس ret;
      }
      توڑو;

    حالت CGROUP_منوال_LEGACY:
    حالت CGROUP_منوال_HYBRID:
      اگر (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_قدر ("CPUShares", resources->cpu->shares);
      توڑو;

    طےشدہ:
      واپس crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_منوال);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_قدر

  اگر (! *devices_سیٹ && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس 0;
}

ساکن صحیح
reset_failed_unit (sd_bus *bus, ثابت حرف *unit)
{
  صحیح sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_پیغام *m = تہی, *جواب = تہی;

  sd_err = sd_bus_پیغام_new_طریقہ_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  اگر (UNLIKELY (sd_err < 0))
    جاؤ باہر_نکلو;

  sd_err = sd_bus_پیغام_append (m, "s", unit);
  اگر (UNLIKELY (sd_err < 0))
    جاؤ باہر_نکلو;

  sd_err = sd_bus_call (bus, m, 0, &error, &جواب);
  اگر (UNLIKELY (sd_err < 0))
    جاؤ باہر_نکلو;

  sd_err = 0;

باہر_نکلو:
  اگر (m)
    sd_bus_پیغام_unref (m);
  اگر (جواب)
    sd_bus_پیغام_unref (جواب);
  sd_bus_error_free (&error);

  واپس sd_err;
}

ساکن صحیح
verify_ebpf_device_filter_installed (ثابت حرف *cgroup_راستہ, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = تہی;
  cleanup_free حرف *بھرا_راستہ = تہی;
  size_t n_progs = 0;
  صحیح ret;

  ret = append_paths (&بھرا_راستہ, err, CGROUP_جڑ, cgroup_راستہ, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_ebpf_query_cgroup_progs (بھرا_راستہ, &progs, &n_progs, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (n_progs == 0)
    واپس crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", بھرا_راستہ);

  واپس 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
ساکن صحیح
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_پیغام *m, sd_bus_پیغام *جواب,
                           sd_bus_error *error, صحیح ret)
{
  اگر (bus)
    sd_bus_unref (bus);
  اگر (m)
    sd_bus_پیغام_unref (m);
  اگر (جواب)
    sd_bus_پیغام_unref (جواب);
  sd_bus_error_free (error);
  واپس ret;
}

ساکن صحیح
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            صحیح cgroup_منوال,
                            string_map *annotations,
                            ثابت حرف *state_جڑ,
                            ثابت حرف *scope, ثابت حرف *قاش,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_سیٹ,
                            libcrun_error_t *err)
{
  sd_bus *bus = تہی;
  sd_bus_پیغام *m = تہی;
  sd_bus_پیغام *جواب = تہی;
  صحیح sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ثابت حرف *object = تہی;
  ساخت systemd_کام_removed_s کام_ڈیٹا = {};
  صحیح i;
  ثابت حرف *boolean_opts[10];
  cleanup_free حرف *state_dir = تہی;

  *can_retry = false;
  *devices_سیٹ = false;

  ret = libcrun_get_state_directory (&state_dir, state_جڑ, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = تہی;

  ret = open_sd_bus_connection (&bus, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ باہر_نکلو;

  ret = systemd_check_کام_حالت_تیاری (bus, &کام_ڈیٹا, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ باہر_نکلو;

  sd_err = sd_bus_پیغام_new_طریقہ_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      جاؤ باہر_نکلو;
    }

  sd_err = sd_bus_پیغام_append (m, "ss", scope, "fail");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      جاؤ باہر_نکلو;
    }

  sd_err = sd_bus_پیغام_open_container (m, 'a', "(sv)");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      جاؤ باہر_نکلو;
    }

  اگر (قاش)
    {
      sd_err = sd_bus_پیغام_append (m, "(sv)", "Slice", "s", قاش);
      اگر (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          جاؤ باہر_نکلو;
        }
    }

  sd_err = sd_bus_پیغام_append (m, "(sv)", "DefaultDependencies", "b", 0);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      جاؤ باہر_نکلو;
    }

  اگر (annotations)
    {
      size_t سابقہ_len = حجم (SYSTEMD_PROPERTY_سابقہ) - 1;
      size_t annotations_len = string_map_سائز (annotations);
      size_t i;

      برائے (i = 0; i < annotations_len; i++)
        {
          ثابت حرف *کلید;
          ثابت حرف *قدر;
          size_t len;

          ret = string_map_get_at (annotations, i, &کلید, &قدر);
          اگر (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              جاؤ باہر_نکلو;
            }

          اگر (! has_سابقہ (کلید, SYSTEMD_PROPERTY_سابقہ))
            جاری;

          len = لڑی_لمبائی (کلید);
          اگر (len < سابقہ_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", کلید);
              جاؤ باہر_نکلو;
            }

          ret = append_systemd_annotation (m, کلید + سابقہ_len, len - سابقہ_len,
                                           قدر, err);
          اگر (UNLIKELY (ret < 0))
            جاؤ باہر_نکلو;
        }
    }

  sd_err = sd_bus_پیغام_append (m, "(sv)", "Description", "s", "libcrun container");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      جاؤ باہر_نکلو;
    }

  sd_err = sd_bus_پیغام_append (m, "(sv)", "PIDs", "au", 1, pid);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      جاؤ باہر_نکلو;
    }

  برائے (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_پیغام_append (m, "(sv)", boolean_opts[i], "b", 1);
      اگر (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          جاؤ باہر_نکلو;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_منوال, scope, devices_سیٹ, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ باہر_نکلو;

  sd_err = sd_bus_پیغام_close_container (m);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      جاؤ باہر_نکلو;
    }

  sd_err = sd_bus_پیغام_append (m, "a(sa(sv))", 0);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      جاؤ باہر_نکلو;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &جواب);
  اگر (UNLIKELY (sd_err < 0))
    {
      اگر (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          اگر (جواب)
            sd_bus_پیغام_unref (جواب);

          error = SD_BUS_ERROR_NULL;
          جواب = تہی;

          sd_err = sd_bus_call (bus, m, 0, &error, &جواب);
        }
      اگر (sd_err < 0)
        {
          اگر (sd_err == -EROFS)
            {
              ret = رجسٹر_missing_property_from_پیغام (state_dir, error.پیغام_2, err);
              اگر (UNLIKELY (ret < 0))
                جاؤ باہر_نکلو;
              اگر (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.پیغام_2 ?: error.name);
          جاؤ باہر_نکلو;
        }
    }

  sd_err = sd_bus_پیغام_read (جواب, "o", &object);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      جاؤ باہر_نکلو;
    }

  ret = systemd_check_کام_حالت (bus, &کام_ڈیٹا, object, "creating", err);

باہر_نکلو:
  واپس cleanup_sd_bus_and_return (bus, m, جواب, &error, ret);
}

ساکن صحیح
libcrun_destroy_systemd_cgroup_scope (ساخت libcrun_cgroup_حالت *cgroup_حالت,
                                      libcrun_error_t *err)
{
  sd_bus *bus = تہی;
  sd_bus_پیغام *m = تہی;
  sd_bus_پیغام *جواب = تہی;
  صحیح ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ثابت حرف *object;
  ثابت حرف *scope = cgroup_حالت->scope;
  ساخت systemd_کام_removed_s کام_ڈیٹا = {};

  ret = open_sd_bus_connection (&bus, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ باہر_نکلو;

  ret = systemd_check_کام_حالت_تیاری (bus, &کام_ڈیٹا, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ باہر_نکلو;

  ret = sd_bus_پیغام_new_طریقہ_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      جاؤ باہر_نکلو;
    }

  ret = sd_bus_پیغام_append (m, "ss", scope, "replace");
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      جاؤ باہر_نکلو;
    }

  ret = sd_bus_call (bus, m, 0, &error, &جواب);
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.پیغام_2 ?: error.name);
      جاؤ باہر_نکلو;
    }

  ret = sd_bus_پیغام_read (جواب, "o", &object);
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      جاؤ باہر_نکلو;
    }

  ret = systemd_check_کام_حالت (bus, &کام_ڈیٹا, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

باہر_نکلو:
  واپس cleanup_sd_bus_and_return (bus, m, جواب, &error, ret);
}

ساکن ثابت حرف *
ڈھونڈو_systemd_subgroup (string_map *annotations)
{
  ثابت حرف *annotation;

  annotation = ڈھونڈو_string_map_قدر (annotations, "run.oci.systemd.subgroup");
  اگر (annotation)
    {
      اگر (annotation[0] == '\0')
        واپس تہی;
      واپس annotation;
    }

  واپس "container";
}

ساکن صحیح
libcrun_cgroup_enter_systemd (ساخت libcrun_cgroup_دلائل *دلائل,
                              ساخت libcrun_cgroup_حالت *برآمد,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = دلائل->resources;
  ثابت حرف *cgroup_راستہ = دلائل->cgroup_راستہ;
  cleanup_free حرف *scope_راستہ = تہی;
  cleanup_free حرف *scope = تہی;
  cleanup_free حرف *راستہ = تہی;
  cleanup_free حرف *قاش = تہی;
  صحیح retries_بایاں = 32;
  ثابت حرف *suffix;
  ثابت حرف *id = دلائل->id;
  pid_t pid = دلائل->pid;
  صحیح cgroup_منوال;
  صحیح rootless = 0;
  صحیح ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
    {
      rootless = is_rootless (err);
      اگر (UNLIKELY (rootless < 0))
        واپس rootless;
    }

  get_systemd_scope_and_قاش (id, rootless == 1, cgroup_راستہ, &scope, &قاش);

  برائے (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_منوال, دلائل->annotations, دلائل->state_جڑ,
                                        scope, قاش, pid, &can_retry, &برآمد->bpf_dev_سیٹ, err);
      اگر (LIKELY (ret >= 0))
        توڑو;

      اگر (can_retry && retries_بایاں-- > 0)
        {
          crun_error_release (err);
          جاری;
        }

      واپس ret;
    }

  suffix = ڈھونڈو_systemd_subgroup (دلائل->annotations);

  ret = systemd_finalize (دلائل, &راستہ, cgroup_منوال, suffix, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  اگر (is_خالی_خانے_string (suffix))
    scope_راستہ = xstrdup (راستہ);
  ورنہ
    {
      size_t راستہ_len = لڑی_لمبائی (راستہ);
      size_t suffix_len = لڑی_لمبائی (suffix);

      scope_راستہ = strndup (راستہ, راستہ_len - suffix_len - 1);
      اگر (UNLIKELY (scope_راستہ == تہی))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_راستہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  اگر (برآمد->bpf_dev_سیٹ)
    {
      ret = verify_ebpf_device_filter_installed (scope_راستہ, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  برآمد->راستہ = راستہ;
  راستہ = تہی;

  برآمد->scope = scope;
  scope = تہی;
  واپس 0;
}

حرف *
get_cgroup_scope_راستہ (ثابت حرف *cgroup_راستہ, ثابت حرف *scope)
{
  حرف *راستہ_مقصد_scope = تہی;
  حرف *cur;

  اگر (is_خالی_خانے_string (cgroup_راستہ))
    واپس تہی;

  راستہ_مقصد_scope = xstrdup (cgroup_راستہ);

  cur = strchr (راستہ_مقصد_scope, '/');
  جب (cur)
    {
      حرف *اگلا = strchr (cur + 1, '/');
      اگر (اگلا == تہی)
        توڑو;

      *اگلا = '\0';
      اگر (strcmp (cur, scope) == 0)
        واپس راستہ_مقصد_scope;
      *اگلا = '/';

      cur = اگلا;
      جب (*cur == '/')
        cur++;
    }

  واپس راستہ_مقصد_scope;
}

ساکن صحیح
libcrun_destroy_cgroup_systemd (ساخت libcrun_cgroup_حالت *cgroup_حالت,
                                libcrun_error_t *err)
{
  cleanup_free حرف *راستہ_مقصد_scope = تہی;
  cleanup_free حرف *bpfprog = تہی;
  صحیح منوال;
  صحیح ret;

  منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (منوال < 0))
    واپس منوال;

  ret = cgroup_killall_راستہ (cgroup_حالت->راستہ, SIGKILL, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_حالت, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_راستہ_from_scope (&bpfprog, cgroup_حالت->scope, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);
  ورنہ
    unlink (bpfprog); // Best effort.

  راستہ_مقصد_scope = get_cgroup_scope_راستہ (cgroup_حالت->راستہ, cgroup_حالت->scope);

  واپس destroy_cgroup_راستہ (راستہ_مقصد_scope, منوال, err);
}

ساکن صحیح
libcrun_update_resources_systemd (ساخت libcrun_cgroup_حالت *cgroup_حالت,
                                  ثابت حرف *state_جڑ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  ساخت systemd_کام_removed_s کام_ڈیٹا = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free حرف *state_dir = تہی;
  sd_bus_پیغام *جواب = تہی;
  sd_bus_پیغام *m = تہی;
  sd_bus *bus = تہی;
  صحیح sd_err, ret;
  صحیح cgroup_منوال;

  ret = libcrun_get_state_directory (&state_dir, state_جڑ, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  ret = open_sd_bus_connection (&bus, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = systemd_check_کام_حالت_تیاری (bus, &کام_ڈیٹا, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ باہر_نکلو;

  sd_err = sd_bus_پیغام_new_طریقہ_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      جاؤ باہر_نکلو;
    }

  sd_err = sd_bus_پیغام_append (m, "sb", cgroup_حالت->scope, 1);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      جاؤ باہر_نکلو;
    }

  sd_err = sd_bus_پیغام_open_container (m, 'a', "(sv)");
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      جاؤ باہر_نکلو;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_منوال, cgroup_حالت->scope, &cgroup_حالت->bpf_dev_سیٹ, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ باہر_نکلو;

  sd_err = sd_bus_پیغام_close_container (m);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      جاؤ باہر_نکلو;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &جواب);
  اگر (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.پیغام_2 ?: error.name);
      جاؤ باہر_نکلو;
    }

  اگر (cgroup_منوال != CGROUP_منوال_UNIFIED)
    {
      ret = تیاری_rt_runtime (resources, cgroup_حالت->راستہ, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ باہر_نکلو;

      ret = تیاری_cpuset_for_systemd_v1 (resources, cgroup_حالت->راستہ, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ باہر_نکلو;
    }

  ret = تیاری_missing_cpu_options_for_systemd (resources, cgroup_منوال == CGROUP_منوال_UNIFIED, cgroup_حالت->راستہ, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ باہر_نکلو;

  ret = 0;

باہر_نکلو:
  واپس cleanup_sd_bus_and_return (bus, m, جواب, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
ساکن صحیح
libcrun_attach_process_systemd (ساخت libcrun_cgroup_حالت *cgroup_حالت,
                                ثابت حرف *راستہ, pid_t pid,
                                libcrun_error_t *err)
{
  ثابت حرف *scope = cgroup_حالت->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free حرف *scope_راستہ = تہی;
  sd_bus_پیغام *جواب = تہی;
  ثابت حرف *subcgroup;
  ثابت حرف *آخری;
  sd_bus *bus = تہی;
  size_t len;
  صحیح sd_err, ret;

  اگر (is_خالی_خانے_string (scope))
    واپس crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_راستہ = get_cgroup_scope_راستہ (راستہ, scope);
  اگر (scope_راستہ == تہی)
    واپس crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  آخری = strrchr (scope_راستہ, '/');
  اگر (آخری == تہی || strcmp (آخری + 1, scope) != 0)
    واپس crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, راستہ);
  len = لڑی_لمبائی (scope_راستہ);
  subcgroup = راستہ[len] ? راستہ + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  sd_err = sd_bus_call_طریقہ (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &جواب, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  اگر (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.پیغام_2 ?: error.name);

  واپس cleanup_sd_bus_and_return (bus, تہی, جواب, &error, ret);
}

#ورنہ
ساکن صحیح
libcrun_cgroup_enter_systemd (ساخت libcrun_cgroup_دلائل *دلائل,
                              ساخت libcrun_cgroup_حالت *برآمد,
                              libcrun_error_t *err)
{
  (خالی) دلائل;
  (خالی) برآمد;

  واپس crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساکن صحیح
libcrun_destroy_cgroup_systemd (ساخت libcrun_cgroup_حالت *cgroup_حالت,
                                libcrun_error_t *err)
{
  (خالی) cgroup_حالت;

  واپس crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساکن صحیح
libcrun_update_resources_systemd (ساخت libcrun_cgroup_حالت *cgroup_حالت,
                                  ثابت حرف *state_جڑ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (خالی) cgroup_حالت;
  (خالی) state_جڑ;
  (خالی) resources;

  واپس crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساکن صحیح
libcrun_attach_process_systemd (ساخت libcrun_cgroup_حالت *cgroup_حالت,
                                ثابت حرف *راستہ, pid_t pid,
                                libcrun_error_t *err)
{
  (خالی) cgroup_حالت;
  (خالی) راستہ;
  (خالی) pid;

  واپس crun_make_error (err, ENOTSUP, "systemd not supported");
}
#اگر_ختم

ساخت libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = تہی,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
