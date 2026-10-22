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
#ضمن "cgroup-cgroupfs.h"
#ضمن "cgroup-internal.h"
#ضمن "cgroup-systemd.h"
#ضمن "cgroup-utils.h"
#ضمن "cgroup-setup.h"
#ضمن "cgroup-resources.h"
#ضمن "ebpf.h"
#ضمن "utils.h"
#ضمن "status.h"
#ضمن <string.h>
#ضمن <sched.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <sys/vfs.h>
#ضمن <inttypes.h>
#ضمن <time.h>

#ضمن <linux/magic.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <libgen.h>

ساكن صحيح
libcrun_cgroup_enter_disabled (بنية libcrun_cgroup_آرگومانها *آرگومانها arg_استفادهنشده, بنية libcrun_cgroup_وضعیت *بیرون, libcrun_error_t *err arg_استفادهنشده)
{
  بیرون->مسیر = عدم;
  أرجع 0;
}

ساكن صحيح
libcrun_destroy_cgroup_disabled (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت arg_استفادهنشده,
                                 libcrun_error_t *err arg_استفادهنشده)
{
  أرجع 0;
}

بنية libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = عدم,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

ساكن صحيح
get_cgroup_manager (صحيح manager, بنية libcrun_cgroup_manager **بیرون, libcrun_error_t *err)
{
  بدل (manager)
    {
    حالة CGROUP_MANAGER_DISABLED:
      *بیرون = &cgroup_manager_disabled;
      أرجع 0;

    حالة CGROUP_MANAGER_SYSTEMD:
      *بیرون = &cgroup_manager_systemd;
      أرجع 0;

    حالة CGROUP_MANAGER_CGROUPFS:
      *بیرون = &cgroup_manager_cgroupfs;
      أرجع 0;
    }

  *بیرون = عدم;
  أرجع crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

ساكن ثابت حرف *
پیدا_کن_delegate_cgroup (string_map *annotations)
{
  ثابت حرف *annotation;

  annotation = پیدا_کن_string_map_مقدار (annotations, "run.oci.delegate-cgroup");
  إذا (annotation)
    {
      إذا (annotation[0] == '\0')
        أرجع عدم;
      أرجع annotation;
    }

  أرجع عدم;
}

صحيح
libcrun_cgroup_pause_unpause (بنية libcrun_cgroup_وضعیت *وضعیت, ثابت bool pause, libcrun_error_t *err)
{
  أرجع libcrun_cgroup_pause_unpause_مسیر (وضعیت->مسیر, pause, err);
}

صحيح
libcrun_cgroup_is_container_paused (بنية libcrun_cgroup_وضعیت *وضعیت, bool *paused, libcrun_error_t *err)
{
  ثابت حرف *cgroup_مسیر = وضعیت->مسیر;
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *مسیر = عدم;
  ثابت حرف *state;
  صحيح cgroup_نما;
  صحيح ret;

  إذا (cgroup_مسیر == عدم)
    أرجع 0;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  إذا (cgroup_نما == CGROUP_نما_UNIFIED)
    {
      state = "1";

      ret = append_paths (&مسیر, err, CGROUP_ریشه, cgroup_مسیر, "cgroup.freeze", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      state = "FROZEN";

      ret = append_paths (&مسیر, err, CGROUP_ریشه "/freezer", cgroup_مسیر, "freezer.state", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = read_all_file (مسیر, &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      إذا (errno != ENOENT || cgroup_نما == CGROUP_نما_UNIFIED)
        أرجع ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      بنية statfs freezer_stat;
      إذا (statfs (CGROUP_ریشه "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          أرجع crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ریشه "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      إذا (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        أرجع ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      أرجع 0;
    }

  *paused = جستجوی_رشته (content, state) != عدم;
  أرجع 0;
}

صحيح
libcrun_cgroup_join_process (بنية libcrun_cgroup_وضعیت *وضعیت, ثابت حرف *مسیر, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = عدم;
  صحيح errno_;
  صحيح ret;

  ret = libcrun_move_process_مقصد_cgroup (pid, init_pid, مسیر, false, err);
  إذا (LIKELY (ret >= 0))
    أرجع ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  إذا (errno_ != EACCES && errno_ != EPERM)
    أرجع ret;

  إذا (get_cgroup_manager (وضعیت->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      أرجع ret;
    }
  إذا (cgroup_manager->attach_process == عدم)
    أرجع ret;

  إذا (UNLIKELY (cgroup_manager->attach_process (وضعیت, مسیر, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, مسیر, tmp_err->پیام);
      crun_error_release (&tmp_err);
      أرجع ret;
    }

  crun_error_release (err);
  أرجع 0;
}

صحيح
libcrun_cgroup_read_pids (بنية libcrun_cgroup_وضعیت *وضعیت, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  أرجع libcrun_cgroup_read_pids_from_مسیر (وضعیت->مسیر, recurse, pids, err);
}

صحيح
libcrun_cgroup_killall (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت, صحيح إشارة, libcrun_error_t *err)
{
  أرجع cgroup_killall_مسیر (cgroup_وضعیت->مسیر, إشارة, err);
}

صحيح
libcrun_cgroup_destroy (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت, libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager = عدم;
  صحيح ret;

  ret = get_cgroup_manager (cgroup_وضعیت->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع cgroup_manager->destroy_cgroup (cgroup_وضعیت, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
ساكن صحيح
complete_cpu_max (ثابت حرف *مسیر, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free حرف *cpu_max_مسیر = عدم;
  cleanup_free حرف *content = عدم;
  bool has_period;
  uint64_t period;
  int64_t quota;
  صحيح cgroup_نما;
  صحيح ret;

  إذا (resources == عدم || resources->cpu == عدم)
    أرجع 0;

  cpu = resources->cpu;
  إذا (cpu->quota_present == cpu->period_present)
    أرجع 0;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;
  إذا (cgroup_نما != CGROUP_نما_UNIFIED)
    أرجع 0;

  ret = append_paths (&cpu_max_مسیر, err, CGROUP_ریشه, مسیر, "cpu.max", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_all_file (cpu_max_مسیر, &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      أرجع 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  وإلا إذا (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  أرجع 0;
}

صحيح
libcrun_update_cgroup_resources (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت,
                                 ثابت حرف *state_ریشه,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager = عدم;
  صحيح ret;

  ret = get_cgroup_manager (cgroup_وضعیت->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = check_حافظه_before_update (cgroup_وضعیت->مسیر, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = complete_cpu_max (cgroup_وضعیت->مسیر, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_وضعیت, state_ریشه, resources, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع update_cgroup_resources (cgroup_وضعیت->مسیر, state_ریشه, resources, ! cgroup_وضعیت->bpf_dev_مجموعه, err);
}

ساكن صحيح
can_ignore_cgroup_enter_errors (بنية libcrun_cgroup_آرگومانها *آرگومانها, صحيح cgroup_نما,
                                libcrun_error_t *err)
{
  صحيح manager = آرگومانها->manager;
  صحيح rootless;

  rootless = is_rootless (err);
  إذا (UNLIKELY (rootless < 0))
    أرجع rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  إذا (! rootless)
    أرجع 0;

  إذا (! is_خالی_string (آرگومانها->cgroup_مسیر))
    أرجع 0;

  إذا (cgroup_نما == CGROUP_نما_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    أرجع 0;

  أرجع 1;
}

صحيح
libcrun_cgroup_preenter (بنية libcrun_cgroup_آرگومانها *آرگومانها, صحيح *dirfd, libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager;
  صحيح cgroup_نما;
  صحيح ret;

  *dirfd = -1;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  إذا (cgroup_نما != CGROUP_نما_UNIFIED)
    أرجع 0;

  ret = get_cgroup_manager (آرگومانها->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cgroup_manager->precreate_cgroup == عدم)
    أرجع 0;

  أرجع cgroup_manager->precreate_cgroup (آرگومانها, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
فراغ
libcrun_cgroup_preenter_cleanup (بنية libcrun_cgroup_آرگومانها *آرگومانها)
{
  إذا (آرگومانها->precreated_مسیر == عدم)
    أرجع;

  (فراغ) rmdir (آرگومانها->precreated_مسیر);
  حرر (آرگومانها->precreated_مسیر);
  آرگومانها->precreated_مسیر = عدم;
}

صحيح
libcrun_cgroup_enter (بنية libcrun_cgroup_آرگومانها *آرگومانها, بنية libcrun_cgroup_وضعیت **بیرون, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_وضعیت بنية libcrun_cgroup_وضعیت *وضعیت = xmalloc0 (حجم *وضعیت);
  بنية libcrun_cgroup_manager *cgroup_manager;
  uid_t ریشه_uid = آرگومانها->ریشه_uid;
  uid_t ریشه_gid = آرگومانها->ریشه_gid;
  صحيح cgroup_نما;
  صحيح ret;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  إذا (cgroup_نما == CGROUP_نما_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free حرف *میانگیر = عدم;

      ret = read_all_file (CGROUP_ریشه "/unified/cgroup.controllers", &میانگیر, &len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (len > 0)
        أرجع crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (آرگومانها->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  وضعیت->manager = آرگومانها->manager;

  ret = cgroup_manager->create_cgroup (آرگومانها, وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = عدم;
      صحيح ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (آرگومانها, cgroup_نما, &tmp_err);
      إذا (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          أرجع ignore_cgroup_errors;
        }

      إذا (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          حرر (وضعیت->مسیر);
          حرر (وضعیت->scope);
          وضعیت->مسیر = عدم;
          وضعیت->scope = عدم;
          وضعیت->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          اذهب success;
        }

      أرجع ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  حرر (آرگومانها->precreated_مسیر);
  آرگومانها->precreated_مسیر = عدم;

  إذا (وضعیت->مسیر)
    {
      bool need_chown;

      need_chown = ریشه_uid != (uid_t) -1 || ریشه_gid != (gid_t) -1;
      إذا (cgroup_نما == CGROUP_نما_UNIFIED && need_chown)
        {
          ret = chown_cgroups (وضعیت->مسیر, ریشه_uid, ریشه_gid, err);
          إذا (UNLIKELY (ret < 0))
            اذهب fail_destroy;
        }

      إذا (آرگومانها->resources)
        {
          ret = update_cgroup_resources (وضعیت->مسیر, آرگومانها->state_ریشه, آرگومانها->resources, ! وضعیت->bpf_dev_مجموعه, err);
          إذا (UNLIKELY (ret < 0))
            اذهب fail_destroy;
        }
    }
success:
  *بیرون = وضعیت;
  وضعیت = عدم;
  أرجع 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = عدم;

    إذا (UNLIKELY (cgroup_manager->destroy_cgroup (وضعیت, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  أرجع ret;
}

صحيح
libcrun_cgroup_enter_finalize (بنية libcrun_cgroup_آرگومانها *آرگومانها, بنية libcrun_cgroup_وضعیت *cgroup_وضعیت arg_استفادهنشده, libcrun_error_t *err)
{
  cleanup_free حرف *هدف_cgroup = عدم;
  cleanup_free حرف *content = عدم;
  ثابت حرف *delegate_cgroup;
  cleanup_free حرف *dir = عدم;
  حرف *current_cgroup, *مقصد;
  صحيح cgroup_نما;
  صحيح ret;

  delegate_cgroup = پیدا_کن_delegate_cgroup (آرگومانها->annotations);
  إذا (delegate_cgroup == عدم)
    أرجع 0;

  إذا (مسیر_has_dot_dot_component (delegate_cgroup))
    أرجع crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  إذا (cgroup_نما != CGROUP_نما_UNIFIED)
    أرجع crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free حرف *proc_مسیر = عدم;
  cleanup_close صحيح fd = -1;

  xasprintf (&proc_مسیر, "%d/cgroup", آرگومانها->pid);
  fd = libcrun_open_proc_file (آرگومانها->container, proc_مسیر, O_RDONLY, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = read_all_fd (fd, "cgroup path", &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  current_cgroup = جستجوی_رشته (content, "0::");
  إذا (UNLIKELY (current_cgroup == عدم))
    أرجع crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  مقصد = strchr (current_cgroup, '\n');
  إذا (UNLIKELY (مقصد == عدم))
    أرجع crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *مقصد = '\0';

  ret = append_paths (&هدف_cgroup, err, current_cgroup, delegate_cgroup, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&dir, err, CGROUP_ریشه, هدف_cgroup, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = move_process_مقصد_cgroup (آرگومانها->pid, عدم, هدف_cgroup, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = enable_controllers (هدف_cgroup, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = chown_cgroups (هدف_cgroup, آرگومانها->ریشه_uid, آرگومانها->ریشه_gid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

صحيح
libcrun_cgroup_has_oom (بنية libcrun_cgroup_وضعیت *وضعیت, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  ثابت حرف *مسیر = وضعیت->مسیر;
  ثابت حرف *پیشوند = عدم;
  size_t content_اندازه = 0;
  صحيح cgroup_نما;
  حرف *it;

  إذا (UNLIKELY (مسیر == عدم || مسیر[0] == '\0'))
    أرجع 0;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  بدل (cgroup_نما)
    {
    حالة CGROUP_نما_UNIFIED:
      {
        cleanup_free حرف *events_مسیر = عدم;
        صحيح ret;

        ret = append_paths (&events_مسیر, err, CGROUP_ریشه, مسیر, "memory.events", عدم);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_مسیر, &content, &content_اندازه, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        پیشوند = "oom ";
        اكسر;
      }
    حالة CGROUP_نما_LEGACY:
    حالة CGROUP_نما_HYBRID:
      {
        cleanup_free حرف *oom_control_مسیر = عدم;
        صحيح ret;

        ret = append_paths (&oom_control_مسیر, err, CGROUP_ریشه, "memory", مسیر, "memory.oom_control", عدم);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_مسیر, &content, &content_اندازه, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        پیشوند = "oom_kill ";
        اكسر;
      }

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_نما);
    }

  it = content;
  طالما (it && *it)
    {
      إذا (has_پیشوند (it, پیشوند))
        {
          it += طول_رشته (پیشوند);
          طالما (*it == ' ')
            it++;

          أرجع *it != '0';
        }
      وإلا
        {
          it = strchr (it, '\n');
          إذا (it == عدم)
            أرجع 0;
          it++;
        }
    }

  أرجع 0;
}

صحيح
libcrun_cgroup_get_وضعیت (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت,
                           libcrun_container_وضعیت_t *وضعیت,
                           libcrun_error_t *err arg_استفادهنشده)
{
  وضعیت->cgroup_مسیر = cgroup_وضعیت->مسیر;
  وضعیت->scope = cgroup_وضعیت->scope;
  أرجع 0;
}

فراغ
libcrun_cgroup_وضعیت_free (بنية libcrun_cgroup_وضعیت *cgroup_وضعیت)
{
  إذا (cgroup_وضعیت == عدم)
    أرجع;

  حرر (cgroup_وضعیت->مسیر);
  حرر (cgroup_وضعیت->scope);
  حرر (cgroup_وضعیت);
}

بنية libcrun_cgroup_وضعیت *
libcrun_cgroup_make_وضعیت (libcrun_container_وضعیت_t *وضعیت)
{
  بنية libcrun_cgroup_وضعیت *ret;

  ret = xmalloc0 (حجم *ret);

  إذا (! is_خالی_string (وضعیت->cgroup_مسیر))
    ret->مسیر = xstrdup (وضعیت->cgroup_مسیر);

  إذا (! is_خالی_string (وضعیت->scope))
    ret->scope = xstrdup (وضعیت->scope);

  إذا (is_خالی_string (وضعیت->cgroup_مسیر) && is_خالی_string (وضعیت->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  وإلا
    ret->manager = وضعیت->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  أرجع ret;
}
