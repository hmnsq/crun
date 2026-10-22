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

#define STEAL_POINTER(x, y) \
  انجامبده                        \
    {                       \
      *x = y;               \
      y = پوچ;             \
  } وقتی (0)

ساختار pid_stat
{
  نویسه state;
  long رقم_threads;
  unsigned long long starttime;
};

ایستا درونخطی bool
is_valid_id_char (نویسه c)
{
  اگر ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    بازگشت true;
  بازگشت c == '_' || c == '+' || c == '.' || c == '-';
}

ایستا صحیح
validate_id (ثابت نویسه *id, libcrun_error_t *err)
{
  اگر (id == پوچ)
    بازگشت 0;

  اگر (id[0] == '\0')
    بازگشت crun_make_error (err, 0, "empty container ID");

  اگر (id[0] == '.')
    بازگشت crun_make_error (err, 0, "invalid container ID `%s`", id);

  برای (size_t i = 0; id[i]; i++)
    {
      اگر (! is_valid_id_char (id[i]))
        بازگشت crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  بازگشت 0;
}

صحیح
get_run_directory (نویسه **out, ثابت نویسه *state_ریشه, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free نویسه *ریشه = پوچ;

  اگر (state_ریشه)
    ریشه = xstrdup (state_ریشه);
  اگر (ریشه == پوچ)
    {
      ثابت نویسه *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      اگر (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&ریشه, err, runtime_dir, "crun", پوچ);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }
  اگر (ریشه == پوچ)
    ریشه = xstrdup ("/run/crun");

  ret = crun_ensure_directory (ریشه, 0700, false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  STEAL_POINTER (out, ریشه);

  بازگشت 0;
}

صحیح
get_shared_empty_directory_path (نویسه **out, ثابت نویسه *state_ریشه, libcrun_error_t *err)
{
  cleanup_free نویسه *run_dir = پوچ;
  صحیح ret;

  ret = get_run_directory (&run_dir, state_ریشه, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

صحیح
libcrun_get_state_directory (نویسه **out, ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free نویسه *path = پوچ;
  cleanup_free نویسه *ریشه = پوچ;

  ret = validate_id (id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = get_run_directory (&ریشه, state_ریشه, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_paths (&path, err, ریشه, id, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  STEAL_POINTER (out, path);

  بازگشت 0;
}

ایستا صحیح
get_state_directory_status_file (نویسه **out, ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err)
{
  cleanup_free نویسه *dir = پوچ;
  صحیح ret;

  ret = libcrun_get_state_directory (&dir, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت append_paths (out, err, dir, "status", پوچ);
}

ایستا صحیح
read_pid_stat (pid_t pid, ساختار pid_stat *st, libcrun_error_t *err)
{
  cleanup_free نویسه *buffer = پوچ;
  cleanup_close صحیح fd = -1;
  نویسه pid_stat_file[64];
  نویسه *it, *s;
  صحیح i, ret;

  ret = snprintf (pid_stat_file, اندازه (pid_stat_file), "/proc/%d/stat", pid);
  اگر (UNLIKELY (ret >= (صحیح) اندازه (pid_stat_file)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  اگر (fd < 0)
    {
      /* The process already exited.  */
      اگر (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, اندازه (*st));
          بازگشت 0;
        }
      بازگشت crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, پوچ, err);
  اگر (ret < 0)
    {
      st->starttime = 0;
      st->رقم_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      بازگشت 0;
    }

  s = پوچ;

  /* Skip the first two arguments.  */
  برای (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  اگر (s)
    وقتی (*s == ' ')
      s++;

  اگر (s == پوچ || *s == '\0')
    بازگشت crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  برای (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      اگر (it == پوچ)
        بازگشت crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      اگر (i == 16)
        st->رقم_threads = strtol (it, پوچ, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, پوچ, 10);
  اگر (errno != 0)
    بازگشت crun_make_error (err, errno, "parse process start time");

  بازگشت 0;
}

صحیح
libcrun_write_container_status (ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  صحیح r, ret;
  cleanup_free نویسه *file_tmp = پوچ;
  cleanup_free نویسه *file = پوچ;
  size_t len;
  cleanup_close صحیح fd_write = -1;
  ثابت نویسه *buf = پوچ;
  ساختار pid_stat st;
  ثابت نویسه *tmp;
  cleanup_json_gen json_gen_ctx *gen = پوچ;

  ret = get_state_directory_status_file (&file, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = read_pid_stat (status->pid, &st, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  اگر (UNLIKELY (fd_write < 0))
    بازگشت crun_make_error (err, errno, "cannot open status file");

  اگر (! json_gen_init (&gen, پوچ))
    بازگشت crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_کلید (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_کلید (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_کلید (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_کلید (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_کلید (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_کلید (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_کلید (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_کلید (gen, "created");
  GEN_STR (gen, status->created);

  اگر (status->owner)
    {
      GEN_کلید (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_کلید (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_کلید (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  اگر (status->external_descriptors)
    {
      GEN_کلید (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  close_and_reset (&fd_write);

  اگر (UNLIKELY (rename (file_tmp, file) < 0))
    بازگشت crun_make_error (err, errno, "cannot rename status file");

  بازگشت 0;

gen_error:
  بازگشت json_gen_error_to_crun_error (r, err);
}

صحیح
libcrun_read_container_status (libcrun_container_status_t *status, ثابت نویسه *state_ریشه, ثابت نویسه *id,
                               libcrun_error_t *err)
{
  cleanup_free نویسه *buffer = پوچ;
  صحیح ret;
  cleanup_free نویسه *file = پوچ;
  json_object *doc = پوچ;
  json_object *tmp;
  ثابت نویسه *val;

  ret = get_state_directory_status_file (&file, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = read_all_file (file, &buffer, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    {

      اگر (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free نویسه *statedir = پوچ;
          libcrun_error_t tmp_err;
          صحیح tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_ریشه, id, &tmp_err);
          اگر (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          وگرنه
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              اگر (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              وگرنه اگر (tmp_ret == 0)
                بازگشت crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      بازگشت ret;
    }

  ret = parse_json_file (&doc, buffer, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  {
    tmp = json_object_object_get (doc, "pid");
    اگر (UNLIKELY (tmp == پوچ))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        برو exit;
      }
    اگر (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        برو exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    اگر (UNLIKELY (tmp == پوچ))
      status->process_start_time = 0; /* backwards compatibility */
    وگرنه اگر (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        برو exit;
      }
    وگرنه
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    اگر (UNLIKELY (tmp == پوچ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        برو exit;
      }
    val = json_object_get_string (tmp);
    اگر (UNLIKELY (val == پوچ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        برو exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : پوچ;
    status->scope = val ? xstrdup (val) : پوچ;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    اگر (UNLIKELY (tmp == پوچ))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        برو exit;
      }
    val = json_object_get_string (tmp);
    اگر (UNLIKELY (val == پوچ))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        برو exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != پوچ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    اگر (UNLIKELY (tmp == پوچ))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        برو exit;
      }
    val = json_object_get_string (tmp);
    اگر (UNLIKELY (val == پوچ))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        برو exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    اگر (UNLIKELY (tmp == پوچ))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        برو exit;
      }
    val = json_object_get_string (tmp);
    اگر (UNLIKELY (val == پوچ))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        برو exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : پوچ;
    status->owner = val ? xstrdup (val) : پوچ;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != پوچ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != پوچ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : پوچ;
    status->external_descriptors = val ? xstrdup (val) : پوچ;
  }
  ret = 0;

exit:
  json_object_put (doc);
  بازگشت ret;
}

صحیح
libcrun_status_check_directories (ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err)
{
  cleanup_free نویسه *dir = پوچ;
  صحیح ret;

  ret = libcrun_get_state_directory (&dir, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = crun_path_exists (dir, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (ret)
    بازگشت crun_make_error (err, 0, "container `%s` already exists", id);

  اگر (UNLIKELY (mkdir (dir, 0700) < 0))
    بازگشت crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  بازگشت 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

ایستا صحیح
rmdirfd_move_to (DIR **d, ثابت نویسه *name, libcrun_error_t *err)
{
  cleanup_close صحیح fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  اگر (UNLIKELY (new_d == پوچ))
    بازگشت crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  بازگشت 0;
}

ایستا صحیح
rmdirfd (ثابت نویسه *namedir, صحیح fd, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_dir DIR *d = پوچ;
  ساختار dirent *de;
  ساختار stat ریشه_st;
  bool is_ریشه = true;
  unsigned صحیح budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close صحیح fd_cleanup = fd;

  ret = fstat (fd, &ریشه_st);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  اگر (d == پوچ)
    بازگشت crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  برای (;;)
    {
      errno = 0;
      de = readdir (d);
      اگر (de == پوچ)
        {
          ساختار stat st;

          اگر (UNLIKELY (errno != 0))
            بازگشت crun_make_error (err, errno, "cannot read directory");

          اگر (is_ریشه)
            بشکن;

          اگر (UNLIKELY (--budget == 0))
            بازگشت crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          ret = fstat (dirfd (d), &st);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "stat `..`");

          is_ریشه = st.st_dev == ریشه_st.st_dev && st.st_ino == ریشه_st.st_ino;
          ادامه;
        }

      اگر ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        ادامه;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      اگر (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          اگر (ret < 0 && errno == EBUSY)
            {
              cleanup_close صحیح tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              اگر (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  اگر (umount2 (procpath, MNT_DETACH) == 0)
                    برو retry_unlink;
                }
              اگر (! is_ریشه)
                بازگشت crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          وگرنه اگر (ret < 0 && errno == ENOTEMPTY)
            {
              اگر (UNLIKELY (--budget == 0))
                بازگشت crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              اگر (UNLIKELY (ret < 0))
                بازگشت ret;

              is_ریشه = false;
            }
          وگرنه اگر (ret < 0 && errno != ENOENT && ! is_ریشه)
            {
              بازگشت crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  بازگشت 0;
}

صحیح
libcrun_container_delete_status (ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_close صحیح rundir_dfd = -1;
  cleanup_close صحیح dfd = -1;
  cleanup_free نویسه *dir = پوچ;

  ret = validate_id (id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = get_run_directory (&dir, state_ریشه, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  اگر (UNLIKELY (rundir_dfd < 0))
    بازگشت crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  اگر (UNLIKELY (dfd < 0))
    بازگشت crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  بازگشت 0;
}

تهی
libcrun_free_container_status (libcrun_container_status_t *status)
{
  اگر (status == پوچ)
    بازگشت;
  آزاد (status->cgroup_path);
  آزاد (status->bundle);
  آزاد (status->rootfs);
  آزاد (status->external_descriptors);
  آزاد (status->created);
  آزاد (status->scope);
  آزاد (status->owner);
}

صحیح
libcrun_get_containers_list (libcrun_container_list_t **out, ثابت نویسه *state_ریشه, libcrun_error_t *err)
{
  ساختار dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = پوچ;
  cleanup_free نویسه *ریشه = پوچ;
  cleanup_dir DIR *dir = پوچ;
  صحیح ret;

  *out = پوچ;

  ret = get_run_directory (&ریشه, state_ریشه, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  dir = opendir (ریشه);
  اگر (UNLIKELY (dir == پوچ))
    بازگشت crun_make_error (err, errno, "cannot opendir `%s`", ریشه);

  برای (next = readdir (dir); next; next = readdir (dir))
    {
      صحیح exists;
      cleanup_free نویسه *status_file = پوچ;

      libcrun_container_list_t *next_container;

      اگر (next->d_name[0] == '.')
        ادامه;

      ret = append_paths (&status_file, err, ریشه, next->d_name, "status", پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      exists = crun_path_exists (status_file, err);
      اگر (exists < 0)
        {
          بازگشت exists;
        }

      اگر (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          ادامه;
        }

      next_container = xmalloc (اندازه (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  بازگشت 0;
}

تهی
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  وقتی (list)
    {
      next = list->next;
      آزاد (list->name);
      آزاد (list);
      list = next;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
صحیح
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ساختار pid_stat st;
  صحیح ret;

  /* For backwards compatibility, check start time only if available. */
  اگر (! status->process_start_time)
    بازگشت 1;

  ret = read_pid_stat (status->pid, &st, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (status->process_start_time != st.starttime || st.state == 'X')
    بازگشت 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  اگر (st.state == 'Z' && st.رقم_threads <= 1)
    بازگشت 0; /* stopped */

  بازگشت 1; /* running, created, or paused */
}

صحیح
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  صحیح ret;

  ret = kill (status->pid, 0);
  اگر (UNLIKELY (ret < 0) && errno != ESRCH)
    بازگشت crun_make_error (err, errno, "kill");

  اگر (ret == 0)
    بازگشت libcrun_check_pid_valid (status, err);

  بازگشت 0; /* stopped */
}

ایستا صحیح
get_exec_fifo_path (نویسه **out, ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err)
{
  cleanup_free نویسه *state_dir = پوچ;
  صحیح ret;

  ret = libcrun_get_state_directory (&state_dir, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت append_paths (out, err, state_dir, "exec.fifo", پوچ);
}

صحیح
libcrun_status_create_exec_fifo (ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err)
{
  cleanup_free نویسه *fifo_path = پوچ;
  صحیح ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  بازگشت fd;
}

صحیح
libcrun_status_write_exec_fifo (ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err)
{
  cleanup_free نویسه *fifo_path = پوچ;
  نویسه buffer[1] = {
    0,
  };
  cleanup_close صحیح fd = -1;
  صحیح ret;

  ret = get_exec_fifo_path (&fifo_path, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (اكتب (fd, buffer, 1));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "write to exec.fifo");

  بازگشت 0;
}

صحیح
libcrun_status_has_read_exec_fifo (ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err)
{
  cleanup_free نویسه *fifo_path = پوچ;
  صحیح ret;

  ret = get_exec_fifo_path (&fifo_path, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت crun_path_exists (fifo_path, err);
}

ساختار libcrun_status_s
{
  libcrun_container_status_t status;
  نویسه *id;
  نویسه *state_ریشه;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  صحیح running;
};

صحیح
libcrun_container_status_load (libcrun_context_t *context, ثابت نویسه *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  صحیح ret;

  st = xmalloc0 (اندازه (*st));

  ret = libcrun_read_container_status (&st->status, context->state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    {
      آزاد (st);
      بازگشت ret;
    }

  st->id = xstrdup (id);
  st->state_ریشه = xstrdup (context->state_ریشه);

  *out = st;
  بازگشت 0;
}

تهی
libcrun_container_status_free (libcrun_status_t *st)
{
  اگر (st == پوچ)
    بازگشت;
  libcrun_free_container_status (&st->status);
  آزاد (st->state_ریشه);
  آزاد (st->id);
  آزاد (st);
}

صحیح
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, صحیح *running, libcrun_error_t *err)
{
  اگر (! st->state_resolved)
    {
      ثابت نویسه *state_string = پوچ;
      صحیح ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_ریشه, &state_string, &st->running, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      اگر (state_string == پوچ)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      وگرنه اگر (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      وگرنه اگر (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      وگرنه اگر (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      وگرنه اگر (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      وگرنه
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  اگر (state)
    *state = st->state;
  اگر (running)
    *running = st->running;
  بازگشت 0;
}

ثابت نویسه *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  انتخاب (state)
    {
    حالت LIBCRUN_CONTAINER_STATUS_CREATING:
      بازگشت "creating";
    حالت LIBCRUN_CONTAINER_STATUS_CREATED:
      بازگشت "created";
    حالت LIBCRUN_CONTAINER_STATUS_RUNNING:
      بازگشت "running";
    حالت LIBCRUN_CONTAINER_STATUS_PAUSED:
      بازگشت "paused";
    حالت LIBCRUN_CONTAINER_STATUS_STOPPED:
      بازگشت "stopped";
    }
  بازگشت "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  بازگشت st->status.pid;
}

ثابت نویسه *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  بازگشت st->status.bundle;
}

ثابت نویسه *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  بازگشت st->status.rootfs;
}

ثابت نویسه *
libcrun_status_get_created (libcrun_status_t *st)
{
  بازگشت st->status.created;
}

ثابت نویسه *
libcrun_status_get_owner (libcrun_status_t *st)
{
  بازگشت st->status.owner;
}

ثابت نویسه *
libcrun_status_get_scope (libcrun_status_t *st)
{
  بازگشت st->status.scope;
}

ثابت نویسه *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  بازگشت st->status.external_descriptors;
}

صحیح
libcrun_container_get_state_string (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه **out, صحیح *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  صحیح local_running;
  صحیح ret;

  ret = libcrun_read_container_status (&status, context->state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_ریشه, out, &local_running, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (running)
    *running = local_running;
  بازگشت 0;
}

صحیح
libcrun_get_state_dir (نویسه **out, ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err)
{
  بازگشت libcrun_get_state_directory (out, state_ریشه, id, err);
}

صحیح
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  بازگشت libcrun_get_containers_list (out, context->state_ریشه, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (اندازه (*it));
  it->cur = l;
  بازگشت it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, ثابت نویسه **id)
{
  اگر (it == پوچ || it->cur == پوچ)
    بازگشت false;

  اگر (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  بازگشت true;
}

تهی
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  آزاد (it);
}

تهی
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
