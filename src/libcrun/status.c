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
#شامل "status.h"
#شامل "utils.h"
#شامل "json_gen_utils.h"
#شامل <stdlib.h>
#شامل <unistd.h>
#شامل <fcntl.h>
#شامل <json-c/json.h>
#شامل <ocispec/json_common.h>
#شامل <sys/mount.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <dirent.h>
#شامل <signal.h>
#شامل <limits.h>

#تعریف_کرو STEAL_اشارہ_گر(x, y) \
  کرو                        \
    {                       \
      *x = y;               \
      y = تہی;             \
  } جب (0)

ساخت pid_stat
{
  حرف state;
  طویل عدد_دھاگے;
  غیرعلامتی طویل طویل starttime;
};

ساکن اندرونی bool
is_valid_id_char (حرف c)
{
  اگر ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    واپس true;
  واپس c == '_' || c == '+' || c == '.' || c == '-';
}

ساکن صحیح
validate_id (ثابت حرف *id, libcrun_error_t *err)
{
  اگر (id == تہی)
    واپس 0;

  اگر (id[0] == '\0')
    واپس crun_make_error (err, 0, "empty container ID");

  اگر (id[0] == '.')
    واپس crun_make_error (err, 0, "invalid container ID `%s`", id);

  برائے (size_t i = 0; id[i]; i++)
    {
      اگر (! is_valid_id_char (id[i]))
        واپس crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  واپس 0;
}

صحیح
get_تسلسل_directory (حرف **برآمد, ثابت حرف *state_جڑ, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free حرف *جڑ = تہی;

  اگر (state_جڑ)
    جڑ = xstrdup (state_جڑ);
  اگر (جڑ == تہی)
    {
      ثابت حرف *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      اگر (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&جڑ, err, runtime_dir, "crun", تہی);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }
  اگر (جڑ == تہی)
    جڑ = xstrdup ("/run/crun");

  ret = crun_ensure_directory (جڑ, 0700, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  STEAL_اشارہ_گر (برآمد, جڑ);

  واپس 0;
}

صحیح
get_shared_خالی_خانے_directory_راستہ (حرف **برآمد, ثابت حرف *state_جڑ, libcrun_error_t *err)
{
  cleanup_free حرف *تسلسل_dir = تہی;
  صحیح ret;

  ret = get_تسلسل_directory (&تسلسل_dir, state_جڑ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_paths (برآمد, err, تسلسل_dir, ".empty-directory", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*برآمد, 0555, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

صحیح
libcrun_get_state_directory (حرف **برآمد, ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free حرف *راستہ = تہی;
  cleanup_free حرف *جڑ = تہی;

  ret = validate_id (id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = get_تسلسل_directory (&جڑ, state_جڑ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_paths (&راستہ, err, جڑ, id, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  STEAL_اشارہ_گر (برآمد, راستہ);

  واپس 0;
}

ساکن صحیح
get_state_directory_حالت_file (حرف **برآمد, ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *dir = تہی;
  صحیح ret;

  ret = libcrun_get_state_directory (&dir, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس append_paths (برآمد, err, dir, "status", تہی);
}

ساکن صحیح
read_pid_stat (pid_t pid, ساخت pid_stat *st, libcrun_error_t *err)
{
  cleanup_free حرف *بفر = تہی;
  cleanup_close صحیح fd = -1;
  حرف pid_stat_file[64];
  حرف *it, *s;
  صحیح i, ret;

  ret = snprintf (pid_stat_file, حجم (pid_stat_file), "/proc/%d/stat", pid);
  اگر (UNLIKELY (ret >= (صحیح) حجم (pid_stat_file)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  اگر (fd < 0)
    {
      /* The process already exited.  */
      اگر (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, حجم (*st));
          واپس 0;
        }
      واپس crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &بفر, تہی, err);
  اگر (ret < 0)
    {
      st->starttime = 0;
      st->عدد_دھاگے = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      واپس 0;
    }

  s = تہی;

  /* Skip the first two arguments.  */
  برائے (it = بفر; it; it = strchr (it + 1, ')'))
    s = it + 1;

  اگر (s)
    جب (*s == ' ')
      s++;

  اگر (s == تہی || *s == '\0')
    واپس crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  برائے (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      اگر (it == تہی)
        واپس crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      اگر (i == 16)
        st->عدد_دھاگے = strtol (it, تہی, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, تہی, 10);
  اگر (errno != 0)
    واپس crun_make_error (err, errno, "parse process start time");

  واپس 0;
}

صحیح
libcrun_write_container_حالت (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_container_حالت_t *حالت_2,
                                libcrun_error_t *err)
{
  صحیح r, ret;
  cleanup_free حرف *file_tmp = تہی;
  cleanup_free حرف *file = تہی;
  size_t len;
  cleanup_close صحیح fd_write = -1;
  ثابت حرف *buf = تہی;
  ساخت pid_stat st;
  ثابت حرف *tmp;
  cleanup_json_gen json_gen_ctx *gen = تہی;

  ret = get_state_directory_حالت_file (&file, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = read_pid_stat (حالت_2->pid, &st, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  حالت_2->process_آغاز_وقت = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  اگر (UNLIKELY (fd_write < 0))
    واپس crun_make_error (err, errno, "cannot open status file");

  اگر (! json_gen_init (&gen, تہی))
    واپس crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_کلید (gen, "pid");
  GEN_OR_FAIL (map_int (gen, حالت_2->pid));

  GEN_کلید (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, حالت_2->process_آغاز_وقت));

  GEN_کلید (gen, "cgroup-path");
  tmp = حالت_2->cgroup_راستہ ? حالت_2->cgroup_راستہ : "";
  GEN_STR (gen, tmp);

  GEN_کلید (gen, "scope");
  tmp = حالت_2->scope ? حالت_2->scope : "";
  GEN_STR (gen, tmp);

  GEN_کلید (gen, "rootfs");
  GEN_STR (gen, حالت_2->rootfs);

  GEN_کلید (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, حالت_2->systemd_cgroup));

  GEN_کلید (gen, "bundle");
  GEN_STR (gen, حالت_2->bundle);

  GEN_کلید (gen, "created");
  GEN_STR (gen, حالت_2->created);

  اگر (حالت_2->owner)
    {
      GEN_کلید (gen, "owner");
      GEN_STR (gen, حالت_2->owner);
    }

  GEN_کلید (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, حالت_2->detached));

  GEN_کلید (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, حالت_2->no_new_keyring));

  اگر (حالت_2->external_descriptors)
    {
      GEN_کلید (gen, "external_descriptors");
      GEN_STR (gen, حالت_2->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = محفوظ_write (fd_write, "status file", buf, len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  close_and_reset (&fd_write);

  اگر (UNLIKELY (rename (file_tmp, file) < 0))
    واپس crun_make_error (err, errno, "cannot rename status file");

  واپس 0;

gen_error:
  واپس json_gen_error_مقصد_crun_error (r, err);
}

صحیح
libcrun_read_container_حالت (libcrun_container_حالت_t *حالت_2, ثابت حرف *state_جڑ, ثابت حرف *id,
                               libcrun_error_t *err)
{
  cleanup_free حرف *بفر = تہی;
  صحیح ret;
  cleanup_free حرف *file = تہی;
  json_object *doc = تہی;
  json_object *tmp;
  ثابت حرف *val;

  ret = get_state_directory_حالت_file (&file, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = read_all_file (file, &بفر, تہی, err);
  اگر (UNLIKELY (ret < 0))
    {

      اگر (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free حرف *statedir = تہی;
          libcrun_error_t tmp_err;
          صحیح tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_جڑ, id, &tmp_err);
          اگر (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          ورنہ
            {
              tmp_ret = crun_راستہ_exists (statedir, &tmp_err);
              اگر (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              ورنہ اگر (tmp_ret == 0)
                واپس crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      واپس ret;
    }

  ret = parse_json_file (&doc, بفر, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  {
    tmp = json_object_object_get (doc, "pid");
    اگر (UNLIKELY (tmp == تہی))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        جاؤ باہر_نکلو;
      }
    اگر (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        جاؤ باہر_نکلو;
      }
    حالت_2->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    اگر (UNLIKELY (tmp == تہی))
      حالت_2->process_آغاز_وقت = 0; /* backwards compatibility */
    ورنہ اگر (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        جاؤ باہر_نکلو;
      }
    ورنہ
      حالت_2->process_آغاز_وقت = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    اگر (UNLIKELY (tmp == تہی))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        جاؤ باہر_نکلو;
      }
    val = json_object_get_string (tmp);
    اگر (UNLIKELY (val == تہی))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        جاؤ باہر_نکلو;
      }
    حالت_2->cgroup_راستہ = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : تہی;
    حالت_2->scope = val ? xstrdup (val) : تہی;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    اگر (UNLIKELY (tmp == تہی))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        جاؤ باہر_نکلو;
      }
    val = json_object_get_string (tmp);
    اگر (UNLIKELY (val == تہی))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        جاؤ باہر_نکلو;
      }
    حالت_2->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    حالت_2->systemd_cgroup = tmp != تہی && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    اگر (UNLIKELY (tmp == تہی))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        جاؤ باہر_نکلو;
      }
    val = json_object_get_string (tmp);
    اگر (UNLIKELY (val == تہی))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        جاؤ باہر_نکلو;
      }
    حالت_2->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    اگر (UNLIKELY (tmp == تہی))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        جاؤ باہر_نکلو;
      }
    val = json_object_get_string (tmp);
    اگر (UNLIKELY (val == تہی))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        جاؤ باہر_نکلو;
      }
    حالت_2->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : تہی;
    حالت_2->owner = val ? xstrdup (val) : تہی;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    حالت_2->detached = tmp != تہی && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    حالت_2->no_new_keyring = tmp != تہی && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : تہی;
    حالت_2->external_descriptors = val ? xstrdup (val) : تہی;
  }
  ret = 0;

باہر_نکلو:
  json_object_put (doc);
  واپس ret;
}

صحیح
libcrun_حالت_check_directories (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *dir = تہی;
  صحیح ret;

  ret = libcrun_get_state_directory (&dir, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = crun_راستہ_exists (dir, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (ret)
    واپس crun_make_error (err, 0, "container `%s` already exists", id);

  اگر (UNLIKELY (mkdir (dir, 0700) < 0))
    واپس crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  واپس 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#تعریف_کرو RMDIRFD_MAX_چالیں (1U << 20)

ساکن صحیح
rmdirfd_move_مقصد (DIR **d, ثابت حرف *name, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  اگر (UNLIKELY (new_d == تہی))
    واپس crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  واپس 0;
}

ساکن صحیح
rmdirfd (ثابت حرف *namedir, صحیح fd, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_dir DIR *d = تہی;
  ساخت dirent *de;
  ساخت stat جڑ_st;
  bool is_جڑ = true;
  غیرعلامتی صحیح budget = RMDIRFD_MAX_چالیں;
  __attribute__ ((غیر_مستعمل)) cleanup_close صحیح fd_cleanup = fd;

  ret = fstat (fd, &جڑ_st);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  اگر (d == تہی)
    واپس crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  برائے (;;)
    {
      errno = 0;
      de = readdir (d);
      اگر (de == تہی)
        {
          ساخت stat st;

          اگر (UNLIKELY (errno != 0))
            واپس crun_make_error (err, errno, "cannot read directory");

          اگر (is_جڑ)
            توڑو;

          اگر (UNLIKELY (--budget == 0))
            واپس crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_مقصد (&d, "..", err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

          ret = fstat (dirfd (d), &st);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "stat `..`");

          is_جڑ = st.st_dev == جڑ_st.st_dev && st.st_ino == جڑ_st.st_ino;
          جاری;
        }

      اگر ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        جاری;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      اگر (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          اگر (ret < 0 && errno == EBUSY)
            {
              cleanup_close صحیح tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_راستہ | O_NOFOLLOW);
              اگر (tfd >= 0)
                {
                  proc_fd_راستہ_t procpath;

                  get_proc_self_fd_راستہ (procpath, tfd);
                  اگر (umount2 (procpath, MNT_DETACH) == 0)
                    جاؤ retry_unlink;
                }
              اگر (! is_جڑ)
                واپس crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          ورنہ اگر (ret < 0 && errno == ENOTEMPTY)
            {
              اگر (UNLIKELY (--budget == 0))
                واپس crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_مقصد (&d, de->d_name, err);
              اگر (UNLIKELY (ret < 0))
                واپس ret;

              is_جڑ = false;
            }
          ورنہ اگر (ret < 0 && errno != ENOENT && ! is_جڑ)
            {
              واپس crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  واپس 0;
}

صحیح
libcrun_container_delete_حالت (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_close صحیح rundir_dfd = -1;
  cleanup_close صحیح dfd = -1;
  cleanup_free حرف *dir = تہی;

  ret = validate_id (id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = get_تسلسل_directory (&dir, state_جڑ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  rundir_dfd = عارضی_FAILURE_RETRY (open (dir, O_DIRECTORY | O_راستہ | O_CLOEXEC));
  اگر (UNLIKELY (rundir_dfd < 0))
    واپس crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  اگر (UNLIKELY (dfd < 0))
    واپس crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  واپس 0;
}

خالی
libcrun_free_container_حالت (libcrun_container_حالت_t *حالت_2)
{
  اگر (حالت_2 == تہی)
    واپس;
  آزاد (حالت_2->cgroup_راستہ);
  آزاد (حالت_2->bundle);
  آزاد (حالت_2->rootfs);
  آزاد (حالت_2->external_descriptors);
  آزاد (حالت_2->created);
  آزاد (حالت_2->scope);
  آزاد (حالت_2->owner);
}

صحیح
libcrun_get_containers_list (libcrun_container_list_t **برآمد, ثابت حرف *state_جڑ, libcrun_error_t *err)
{
  ساخت dirent *اگلا;
  cleanup_container_list libcrun_container_list_t *tmp = تہی;
  cleanup_free حرف *جڑ = تہی;
  cleanup_dir DIR *dir = تہی;
  صحیح ret;

  *برآمد = تہی;

  ret = get_تسلسل_directory (&جڑ, state_جڑ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  dir = opendir (جڑ);
  اگر (UNLIKELY (dir == تہی))
    واپس crun_make_error (err, errno, "cannot opendir `%s`", جڑ);

  برائے (اگلا = readdir (dir); اگلا; اگلا = readdir (dir))
    {
      صحیح exists;
      cleanup_free حرف *حالت_file = تہی;

      libcrun_container_list_t *اگلا_container;

      اگر (اگلا->d_name[0] == '.')
        جاری;

      ret = append_paths (&حالت_file, err, جڑ, اگلا->d_name, "status", تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      exists = crun_راستہ_exists (حالت_file, err);
      اگر (exists < 0)
        {
          واپس exists;
        }

      اگر (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", حالت_file);
          جاری;
        }

      اگلا_container = xmalloc (حجم (libcrun_container_list_t));
      اگلا_container->name = xstrdup (اگلا->d_name);
      اگلا_container->اگلا = tmp;
      tmp = اگلا_container;
    }

  STEAL_اشارہ_گر (برآمد, tmp);

  واپس 0;
}

خالی
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *اگلا;
  جب (list)
    {
      اگلا = list->اگلا;
      آزاد (list->name);
      آزاد (list);
      list = اگلا;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
صحیح
libcrun_check_pid_valid (libcrun_container_حالت_t *حالت_2, libcrun_error_t *err)
{
  ساخت pid_stat st;
  صحیح ret;

  /* For backwards compatibility, check start time only if available. */
  اگر (! حالت_2->process_آغاز_وقت)
    واپس 1;

  ret = read_pid_stat (حالت_2->pid, &st, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (حالت_2->process_آغاز_وقت != st.starttime || st.state == 'X')
    واپس 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  اگر (st.state == 'Z' && st.عدد_دھاگے <= 1)
    واپس 0; /* stopped */

  واپس 1; /* running, created, or paused */
}

صحیح
libcrun_is_container_running (libcrun_container_حالت_t *حالت_2, libcrun_error_t *err)
{
  صحیح ret;

  ret = kill (حالت_2->pid, 0);
  اگر (UNLIKELY (ret < 0) && errno != ESRCH)
    واپس crun_make_error (err, errno, "kill");

  اگر (ret == 0)
    واپس libcrun_check_pid_valid (حالت_2, err);

  واپس 0; /* stopped */
}

ساکن صحیح
get_exec_fifo_راستہ (حرف **برآمد, ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *state_dir = تہی;
  صحیح ret;

  ret = libcrun_get_state_directory (&state_dir, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس append_paths (برآمد, err, state_dir, "exec.fifo", تہی);
}

صحیح
libcrun_حالت_create_exec_fifo (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *fifo_راستہ = تہی;
  صحیح ret, fd = -1;

  ret = get_exec_fifo_راستہ (&fifo_راستہ, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_راستہ);
  ret = mkfifo (fifo_راستہ, 0600);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_راستہ, O_NONBLOCK | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "cannot open pipe `%s`", fifo_راستہ);

  واپس fd;
}

صحیح
libcrun_حالت_write_exec_fifo (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *fifo_راستہ = تہی;
  حرف بفر[1] = {
    0,
  };
  cleanup_close صحیح fd = -1;
  صحیح ret;

  ret = get_exec_fifo_راستہ (&fifo_راستہ, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  fd = open (fifo_راستہ, O_WRONLY | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "cannot open `%s`", fifo_راستہ);

  ret = unlink (fifo_راستہ);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "unlink `%s`", fifo_راستہ);

  ret = عارضی_FAILURE_RETRY (اكتب (fd, بفر, 1));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "write to exec.fifo");

  واپس 0;
}

صحیح
libcrun_حالت_has_read_exec_fifo (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *fifo_راستہ = تہی;
  صحیح ret;

  ret = get_exec_fifo_راستہ (&fifo_راستہ, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس crun_راستہ_exists (fifo_راستہ, err);
}

ساخت libcrun_حالت_s
{
  libcrun_container_حالت_t حالت_2;
  حرف *id;
  حرف *state_جڑ;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  صحیح running;
};

صحیح
libcrun_container_حالت_بھرو (libcrun_context_t *context, ثابت حرف *id, libcrun_حالت_t **برآمد, libcrun_error_t *err)
{
  libcrun_حالت_t *st;
  صحیح ret;

  st = xmalloc0 (حجم (*st));

  ret = libcrun_read_container_حالت (&st->حالت_2, context->state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    {
      آزاد (st);
      واپس ret;
    }

  st->id = xstrdup (id);
  st->state_جڑ = xstrdup (context->state_جڑ);

  *برآمد = st;
  واپس 0;
}

خالی
libcrun_container_حالت_free (libcrun_حالت_t *st)
{
  اگر (st == تہی)
    واپس;
  libcrun_free_container_حالت (&st->حالت_2);
  آزاد (st->state_جڑ);
  آزاد (st->id);
  آزاد (st);
}

صحیح
libcrun_حالت_get_state (libcrun_حالت_t *st, libcrun_container_state_t *state, صحیح *running, libcrun_error_t *err)
{
  اگر (! st->state_resolved)
    {
      ثابت حرف *state_string = تہی;
      صحیح ret;

      ret = libcrun_get_container_state_string (st->id, &st->حالت_2, st->state_جڑ, &state_string, &st->running, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (state_string == تہی)
        st->state = LIBCRUN_CONTAINER_حالت_STOPPED;
      ورنہ اگر (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_حالت_CREATING;
      ورنہ اگر (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_حالت_CREATED;
      ورنہ اگر (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_حالت_RUNNING;
      ورنہ اگر (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_حالت_PAUSED;
      ورنہ
        st->state = LIBCRUN_CONTAINER_حالت_STOPPED;

      st->state_resolved = true;
    }

  اگر (state)
    *state = st->state;
  اگر (running)
    *running = st->running;
  واپس 0;
}

ثابت حرف *
libcrun_container_state_مقصد_string (libcrun_container_state_t state)
{
  انتخاب (state)
    {
    حالت LIBCRUN_CONTAINER_حالت_CREATING:
      واپس "creating";
    حالت LIBCRUN_CONTAINER_حالت_CREATED:
      واپس "created";
    حالت LIBCRUN_CONTAINER_حالت_RUNNING:
      واپس "running";
    حالت LIBCRUN_CONTAINER_حالت_PAUSED:
      واپس "paused";
    حالت LIBCRUN_CONTAINER_حالت_STOPPED:
      واپس "stopped";
    }
  واپس "unknown";
}

pid_t
libcrun_حالت_get_pid (libcrun_حالت_t *st)
{
  واپس st->حالت_2.pid;
}

ثابت حرف *
libcrun_حالت_get_bundle (libcrun_حالت_t *st)
{
  واپس st->حالت_2.bundle;
}

ثابت حرف *
libcrun_حالت_get_rootfs (libcrun_حالت_t *st)
{
  واپس st->حالت_2.rootfs;
}

ثابت حرف *
libcrun_حالت_get_created (libcrun_حالت_t *st)
{
  واپس st->حالت_2.created;
}

ثابت حرف *
libcrun_حالت_get_owner (libcrun_حالت_t *st)
{
  واپس st->حالت_2.owner;
}

ثابت حرف *
libcrun_حالت_get_scope (libcrun_حالت_t *st)
{
  واپس st->حالت_2.scope;
}

ثابت حرف *
libcrun_حالت_get_external_descriptors (libcrun_حالت_t *st)
{
  واپس st->حالت_2.external_descriptors;
}

صحیح
libcrun_container_get_state_string (libcrun_context_t *context, ثابت حرف *id, ثابت حرف **برآمد, صحیح *running,
                                    libcrun_error_t *err)
{
  cleanup_container_حالت libcrun_container_حالت_t حالت_2 = {};
  صحیح local_running;
  صحیح ret;

  ret = libcrun_read_container_حالت (&حالت_2, context->state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_get_container_state_string (id, &حالت_2, context->state_جڑ, برآمد, &local_running, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (running)
    *running = local_running;
  واپس 0;
}

صحیح
libcrun_get_state_dir (حرف **برآمد, ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err)
{
  واپس libcrun_get_state_directory (برآمد, state_جڑ, id, err);
}

صحیح
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **برآمد, libcrun_error_t *err)
{
  واپس libcrun_get_containers_list (برآمد, context->state_جڑ, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (حجم (*it));
  it->cur = l;
  واپس it;
}

bool
libcrun_container_iter_اگلا (libcrun_container_iter_t *it, ثابت حرف **id)
{
  اگر (it == تہی || it->cur == تہی)
    واپس false;

  اگر (id)
    *id = it->cur->name;
  it->cur = it->cur->اگلا;
  واپس true;
}

خالی
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  آزاد (it);
}

خالی
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
