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
#شامل "cgroup-cgroupfs.h"
#شامل "cgroup-internal.h"
#شامل "cgroup-systemd.h"
#شامل "cgroup-utils.h"
#شامل "cgroup-setup.h"
#شامل "cgroup-resources.h"
#شامل "ebpf.h"
#شامل "utils.h"
#شامل "status.h"
#شامل <string.h>
#شامل <sched.h>
#شامل <sys/types.h>
#شامل <signal.h>
#شامل <sys/vfs.h>
#شامل <inttypes.h>
#شامل <time.h>

#شامل <linux/magic.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <libgen.h>

ساکن صحیح
libcrun_cgroup_enter_disabled (ساخت libcrun_cgroup_دلائل *دلائل arg_غیر_مستعمل, ساخت libcrun_cgroup_حالت *برآمد, libcrun_error_t *err arg_غیر_مستعمل)
{
  برآمد->راستہ = تہی;
  واپس 0;
}

ساکن صحیح
libcrun_destroy_cgroup_disabled (ساخت libcrun_cgroup_حالت *cgroup_حالت arg_غیر_مستعمل,
                                 libcrun_error_t *err arg_غیر_مستعمل)
{
  واپس 0;
}

ساخت libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = تہی,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

ساکن صحیح
get_cgroup_manager (صحیح manager, ساخت libcrun_cgroup_manager **برآمد, libcrun_error_t *err)
{
  انتخاب (manager)
    {
    حالت CGROUP_MANAGER_DISABLED:
      *برآمد = &cgroup_manager_disabled;
      واپس 0;

    حالت CGROUP_MANAGER_SYSTEMD:
      *برآمد = &cgroup_manager_systemd;
      واپس 0;

    حالت CGROUP_MANAGER_CGROUPFS:
      *برآمد = &cgroup_manager_cgroupfs;
      واپس 0;
    }

  *برآمد = تہی;
  واپس crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

ساکن ثابت حرف *
ڈھونڈو_delegate_cgroup (string_map *annotations)
{
  ثابت حرف *annotation;

  annotation = ڈھونڈو_string_map_قدر (annotations, "run.oci.delegate-cgroup");
  اگر (annotation)
    {
      اگر (annotation[0] == '\0')
        واپس تہی;
      واپس annotation;
    }

  واپس تہی;
}

صحیح
libcrun_cgroup_pause_unpause (ساخت libcrun_cgroup_حالت *حالت_2, ثابت bool pause, libcrun_error_t *err)
{
  واپس libcrun_cgroup_pause_unpause_راستہ (حالت_2->راستہ, pause, err);
}

صحیح
libcrun_cgroup_is_container_paused (ساخت libcrun_cgroup_حالت *حالت_2, bool *paused, libcrun_error_t *err)
{
  ثابت حرف *cgroup_راستہ = حالت_2->راستہ;
  cleanup_free حرف *content = تہی;
  cleanup_free حرف *راستہ = تہی;
  ثابت حرف *state;
  صحیح cgroup_منوال;
  صحیح ret;

  اگر (cgroup_راستہ == تہی)
    واپس 0;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
    {
      state = "1";

      ret = append_paths (&راستہ, err, CGROUP_جڑ, cgroup_راستہ, "cgroup.freeze", تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      state = "FROZEN";

      ret = append_paths (&راستہ, err, CGROUP_جڑ "/freezer", cgroup_راستہ, "freezer.state", تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = read_all_file (راستہ, &content, تہی, err);
  اگر (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      اگر (errno != ENOENT || cgroup_منوال == CGROUP_منوال_UNIFIED)
        واپس ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      ساخت statfs freezer_stat;
      اگر (statfs (CGROUP_جڑ "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          واپس crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_جڑ "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      اگر (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        واپس ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      واپس 0;
    }

  *paused = لڑی_تلاش (content, state) != تہی;
  واپس 0;
}

صحیح
libcrun_cgroup_join_process (ساخت libcrun_cgroup_حالت *حالت_2, ثابت حرف *راستہ, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  ساخت libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = تہی;
  صحیح errno_;
  صحیح ret;

  ret = libcrun_move_process_مقصد_cgroup (pid, init_pid, راستہ, false, err);
  اگر (LIKELY (ret >= 0))
    واپس ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  اگر (errno_ != EACCES && errno_ != EPERM)
    واپس ret;

  اگر (get_cgroup_manager (حالت_2->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      واپس ret;
    }
  اگر (cgroup_manager->attach_process == تہی)
    واپس ret;

  اگر (UNLIKELY (cgroup_manager->attach_process (حالت_2, راستہ, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, راستہ, tmp_err->پیغام);
      crun_error_release (&tmp_err);
      واپس ret;
    }

  crun_error_release (err);
  واپس 0;
}

صحیح
libcrun_cgroup_read_pids (ساخت libcrun_cgroup_حالت *حالت_2, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  واپس libcrun_cgroup_read_pids_from_راستہ (حالت_2->راستہ, recurse, pids, err);
}

صحیح
libcrun_cgroup_killall (ساخت libcrun_cgroup_حالت *cgroup_حالت, صحیح اشارہ, libcrun_error_t *err)
{
  واپس cgroup_killall_راستہ (cgroup_حالت->راستہ, اشارہ, err);
}

صحیح
libcrun_cgroup_destroy (ساخت libcrun_cgroup_حالت *cgroup_حالت, libcrun_error_t *err)
{
  ساخت libcrun_cgroup_manager *cgroup_manager = تہی;
  صحیح ret;

  ret = get_cgroup_manager (cgroup_حالت->manager, &cgroup_manager, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس cgroup_manager->destroy_cgroup (cgroup_حالت, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
ساکن صحیح
complete_cpu_max (ثابت حرف *راستہ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free حرف *cpu_max_راستہ = تہی;
  cleanup_free حرف *content = تہی;
  bool has_period;
  uint64_t period;
  int64_t quota;
  صحیح cgroup_منوال;
  صحیح ret;

  اگر (resources == تہی || resources->cpu == تہی)
    واپس 0;

  cpu = resources->cpu;
  اگر (cpu->quota_present == cpu->period_present)
    واپس 0;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;
  اگر (cgroup_منوال != CGROUP_منوال_UNIFIED)
    واپس 0;

  ret = append_paths (&cpu_max_راستہ, err, CGROUP_جڑ, راستہ, "cpu.max", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = read_all_file (cpu_max_راستہ, &content, تہی, err);
  اگر (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      واپس 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  ورنہ اگر (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  واپس 0;
}

صحیح
libcrun_update_cgroup_resources (ساخت libcrun_cgroup_حالت *cgroup_حالت,
                                 ثابت حرف *state_جڑ,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  ساخت libcrun_cgroup_manager *cgroup_manager = تہی;
  صحیح ret;

  ret = get_cgroup_manager (cgroup_حالت->manager, &cgroup_manager, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = check_حافظہ_before_update (cgroup_حالت->راستہ, resources, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = complete_cpu_max (cgroup_حالت->راستہ, resources, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_حالت, state_جڑ, resources, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس update_cgroup_resources (cgroup_حالت->راستہ, state_جڑ, resources, ! cgroup_حالت->bpf_dev_سیٹ, err);
}

ساکن صحیح
can_ignore_cgroup_enter_errors (ساخت libcrun_cgroup_دلائل *دلائل, صحیح cgroup_منوال,
                                libcrun_error_t *err)
{
  صحیح manager = دلائل->manager;
  صحیح rootless;

  rootless = is_rootless (err);
  اگر (UNLIKELY (rootless < 0))
    واپس rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  اگر (! rootless)
    واپس 0;

  اگر (! is_خالی_خانے_string (دلائل->cgroup_راستہ))
    واپس 0;

  اگر (cgroup_منوال == CGROUP_منوال_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    واپس 0;

  واپس 1;
}

صحیح
libcrun_cgroup_preenter (ساخت libcrun_cgroup_دلائل *دلائل, صحیح *dirfd, libcrun_error_t *err)
{
  ساخت libcrun_cgroup_manager *cgroup_manager;
  صحیح cgroup_منوال;
  صحیح ret;

  *dirfd = -1;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  اگر (cgroup_منوال != CGROUP_منوال_UNIFIED)
    واپس 0;

  ret = get_cgroup_manager (دلائل->manager, &cgroup_manager, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (cgroup_manager->precreate_cgroup == تہی)
    واپس 0;

  واپس cgroup_manager->precreate_cgroup (دلائل, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
خالی
libcrun_cgroup_preenter_cleanup (ساخت libcrun_cgroup_دلائل *دلائل)
{
  اگر (دلائل->precreated_راستہ == تہی)
    واپس;

  (خالی) rmdir (دلائل->precreated_راستہ);
  آزاد (دلائل->precreated_راستہ);
  دلائل->precreated_راستہ = تہی;
}

صحیح
libcrun_cgroup_enter (ساخت libcrun_cgroup_دلائل *دلائل, ساخت libcrun_cgroup_حالت **برآمد, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *حالت_2 = xmalloc0 (حجم *حالت_2);
  ساخت libcrun_cgroup_manager *cgroup_manager;
  uid_t جڑ_uid = دلائل->جڑ_uid;
  uid_t جڑ_gid = دلائل->جڑ_gid;
  صحیح cgroup_منوال;
  صحیح ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  اگر (cgroup_منوال == CGROUP_منوال_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free حرف *بفر = تہی;

      ret = read_all_file (CGROUP_جڑ "/unified/cgroup.controllers", &بفر, &len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      اگر (len > 0)
        واپس crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (دلائل->manager, &cgroup_manager, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  حالت_2->manager = دلائل->manager;

  ret = cgroup_manager->create_cgroup (دلائل, حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = تہی;
      صحیح ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (دلائل, cgroup_منوال, &tmp_err);
      اگر (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          واپس ignore_cgroup_errors;
        }

      اگر (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          آزاد (حالت_2->راستہ);
          آزاد (حالت_2->scope);
          حالت_2->راستہ = تہی;
          حالت_2->scope = تہی;
          حالت_2->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          جاؤ success;
        }

      واپس ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  آزاد (دلائل->precreated_راستہ);
  دلائل->precreated_راستہ = تہی;

  اگر (حالت_2->راستہ)
    {
      bool need_chown;

      need_chown = جڑ_uid != (uid_t) -1 || جڑ_gid != (gid_t) -1;
      اگر (cgroup_منوال == CGROUP_منوال_UNIFIED && need_chown)
        {
          ret = chown_cgroups (حالت_2->راستہ, جڑ_uid, جڑ_gid, err);
          اگر (UNLIKELY (ret < 0))
            جاؤ fail_destroy;
        }

      اگر (دلائل->resources)
        {
          ret = update_cgroup_resources (حالت_2->راستہ, دلائل->state_جڑ, دلائل->resources, ! حالت_2->bpf_dev_سیٹ, err);
          اگر (UNLIKELY (ret < 0))
            جاؤ fail_destroy;
        }
    }
success:
  *برآمد = حالت_2;
  حالت_2 = تہی;
  واپس 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = تہی;

    اگر (UNLIKELY (cgroup_manager->destroy_cgroup (حالت_2, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  واپس ret;
}

صحیح
libcrun_cgroup_enter_finalize (ساخت libcrun_cgroup_دلائل *دلائل, ساخت libcrun_cgroup_حالت *cgroup_حالت arg_غیر_مستعمل, libcrun_error_t *err)
{
  cleanup_free حرف *ہدف_cgroup = تہی;
  cleanup_free حرف *content = تہی;
  ثابت حرف *delegate_cgroup;
  cleanup_free حرف *dir = تہی;
  حرف *current_cgroup, *مقصد;
  صحیح cgroup_منوال;
  صحیح ret;

  delegate_cgroup = ڈھونڈو_delegate_cgroup (دلائل->annotations);
  اگر (delegate_cgroup == تہی)
    واپس 0;

  اگر (راستہ_has_dot_dot_component (delegate_cgroup))
    واپس crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  اگر (cgroup_منوال != CGROUP_منوال_UNIFIED)
    واپس crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free حرف *proc_راستہ = تہی;
  cleanup_close صحیح fd = -1;

  xasprintf (&proc_راستہ, "%d/cgroup", دلائل->pid);
  fd = libcrun_open_proc_file (دلائل->container, proc_راستہ, O_RDONLY, err);
  اگر (UNLIKELY (fd < 0))
    واپس fd;

  ret = read_all_fd (fd, "cgroup path", &content, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  current_cgroup = لڑی_تلاش (content, "0::");
  اگر (UNLIKELY (current_cgroup == تہی))
    واپس crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  مقصد = strchr (current_cgroup, '\n');
  اگر (UNLIKELY (مقصد == تہی))
    واپس crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *مقصد = '\0';

  ret = append_paths (&ہدف_cgroup, err, current_cgroup, delegate_cgroup, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_paths (&dir, err, CGROUP_جڑ, ہدف_cgroup, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = move_process_مقصد_cgroup (دلائل->pid, تہی, ہدف_cgroup, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = enable_controllers (ہدف_cgroup, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = chown_cgroups (ہدف_cgroup, دلائل->جڑ_uid, دلائل->جڑ_gid, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

صحیح
libcrun_cgroup_has_oom (ساخت libcrun_cgroup_حالت *حالت_2, libcrun_error_t *err)
{
  cleanup_free حرف *content = تہی;
  ثابت حرف *راستہ = حالت_2->راستہ;
  ثابت حرف *سابقہ = تہی;
  size_t content_سائز = 0;
  صحیح cgroup_منوال;
  حرف *it;

  اگر (UNLIKELY (راستہ == تہی || راستہ[0] == '\0'))
    واپس 0;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  انتخاب (cgroup_منوال)
    {
    حالت CGROUP_منوال_UNIFIED:
      {
        cleanup_free حرف *events_راستہ = تہی;
        صحیح ret;

        ret = append_paths (&events_راستہ, err, CGROUP_جڑ, راستہ, "memory.events", تہی);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_راستہ, &content, &content_سائز, err);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        سابقہ = "oom ";
        توڑو;
      }
    حالت CGROUP_منوال_LEGACY:
    حالت CGROUP_منوال_HYBRID:
      {
        cleanup_free حرف *oom_control_راستہ = تہی;
        صحیح ret;

        ret = append_paths (&oom_control_راستہ, err, CGROUP_جڑ, "memory", راستہ, "memory.oom_control", تہی);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_راستہ, &content, &content_سائز, err);
        اگر (UNLIKELY (ret < 0))
          واپس ret;

        سابقہ = "oom_kill ";
        توڑو;
      }

    طےشدہ:
      واپس crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_منوال);
    }

  it = content;
  جب (it && *it)
    {
      اگر (has_سابقہ (it, سابقہ))
        {
          it += لڑی_لمبائی (سابقہ);
          جب (*it == ' ')
            it++;

          واپس *it != '0';
        }
      ورنہ
        {
          it = strchr (it, '\n');
          اگر (it == تہی)
            واپس 0;
          it++;
        }
    }

  واپس 0;
}

صحیح
libcrun_cgroup_get_حالت (ساخت libcrun_cgroup_حالت *cgroup_حالت,
                           libcrun_container_حالت_t *حالت_2,
                           libcrun_error_t *err arg_غیر_مستعمل)
{
  حالت_2->cgroup_راستہ = cgroup_حالت->راستہ;
  حالت_2->scope = cgroup_حالت->scope;
  واپس 0;
}

خالی
libcrun_cgroup_حالت_free (ساخت libcrun_cgroup_حالت *cgroup_حالت)
{
  اگر (cgroup_حالت == تہی)
    واپس;

  آزاد (cgroup_حالت->راستہ);
  آزاد (cgroup_حالت->scope);
  آزاد (cgroup_حالت);
}

ساخت libcrun_cgroup_حالت *
libcrun_cgroup_make_حالت (libcrun_container_حالت_t *حالت_2)
{
  ساخت libcrun_cgroup_حالت *ret;

  ret = xmalloc0 (حجم *ret);

  اگر (! is_خالی_خانے_string (حالت_2->cgroup_راستہ))
    ret->راستہ = xstrdup (حالت_2->cgroup_راستہ);

  اگر (! is_خالی_خانے_string (حالت_2->scope))
    ret->scope = xstrdup (حالت_2->scope);

  اگر (is_خالی_خانے_string (حالت_2->cgroup_راستہ) && is_خالی_خانے_string (حالت_2->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  ورنہ
    ret->manager = حالت_2->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  واپس ret;
}
