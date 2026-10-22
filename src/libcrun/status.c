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

#ضمن <config.h>
#ضمن "status.h"
#ضمن "utils.h"
#ضمن "json_gen_utils.h"
#ضمن <stdlib.h>
#ضمن <unistd.h>
#ضمن <fcntl.h>
#ضمن <json-c/json.h>
#ضمن <ocispec/json_common.h>
#ضمن <sys/mount.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <dirent.h>
#ضمن <signal.h>
#ضمن <limits.h>

#define STEAL_POINTER(x, y) \
  افعل                        \
    {                       \
      *x = y;               \
      y = عدم;             \
  } طالما (0)

بنية pid_stat
{
  حرف state;
  long رەقەم_threads;
  unsigned long long starttime;
};

ساكن مضمن bool
is_valid_id_char (حرف c)
{
  إذا ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    أرجع true;
  أرجع c == '_' || c == '+' || c == '.' || c == '-';
}

ساكن صحيح
validate_id (ثابت حرف *id, libcrun_error_t *err)
{
  إذا (id == عدم)
    أرجع 0;

  إذا (id[0] == '\0')
    أرجع crun_make_error (err, 0, "empty container ID");

  إذا (id[0] == '.')
    أرجع crun_make_error (err, 0, "invalid container ID `%s`", id);

  لكل (size_t i = 0; id[i]; i++)
    {
      إذا (! is_valid_id_char (id[i]))
        أرجع crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  أرجع 0;
}

صحيح
get_run_directory (حرف **out, ثابت حرف *state_يىلتىز, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *يىلتىز = عدم;

  إذا (state_يىلتىز)
    يىلتىز = xstrdup (state_يىلتىز);
  إذا (يىلتىز == عدم)
    {
      ثابت حرف *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      إذا (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&يىلتىز, err, runtime_dir, "crun", عدم);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }
  إذا (يىلتىز == عدم)
    يىلتىز = xstrdup ("/run/crun");

  ret = crun_ensure_directory (يىلتىز, 0700, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  STEAL_POINTER (out, يىلتىز);

  أرجع 0;
}

صحيح
get_shared_empty_directory_path (حرف **out, ثابت حرف *state_يىلتىز, libcrun_error_t *err)
{
  cleanup_free حرف *run_dir = عدم;
  صحيح ret;

  ret = get_run_directory (&run_dir, state_يىلتىز, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

صحيح
libcrun_get_state_directory (حرف **out, ثابت حرف *state_يىلتىز, ثابت حرف *id, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *path = عدم;
  cleanup_free حرف *يىلتىز = عدم;

  ret = validate_id (id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = get_run_directory (&يىلتىز, state_يىلتىز, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&path, err, يىلتىز, id, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  STEAL_POINTER (out, path);

  أرجع 0;
}

ساكن صحيح
get_state_directory_status_file (حرف **out, ثابت حرف *state_يىلتىز, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *dir = عدم;
  صحيح ret;

  ret = libcrun_get_state_directory (&dir, state_يىلتىز, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع append_paths (out, err, dir, "status", عدم);
}

ساكن صحيح
read_pid_stat (pid_t pid, بنية pid_stat *st, libcrun_error_t *err)
{
  cleanup_free حرف *buffer = عدم;
  cleanup_close صحيح fd = -1;
  حرف pid_stat_file[64];
  حرف *it, *s;
  صحيح i, ret;

  ret = snprintf (pid_stat_file, حجم (pid_stat_file), "/proc/%d/stat", pid);
  إذا (UNLIKELY (ret >= (صحيح) حجم (pid_stat_file)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  إذا (fd < 0)
    {
      /* The process already exited.  */
      إذا (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, حجم (*st));
          أرجع 0;
        }
      أرجع crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, عدم, err);
  إذا (ret < 0)
    {
      st->starttime = 0;
      st->رەقەم_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      أرجع 0;
    }

  s = عدم;

  /* Skip the first two arguments.  */
  لكل (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  إذا (s)
    طالما (*s == ' ')
      s++;

  إذا (s == عدم || *s == '\0')
    أرجع crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  لكل (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      إذا (it == عدم)
        أرجع crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      إذا (i == 16)
        st->رەقەم_threads = strtol (it, عدم, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, عدم, 10);
  إذا (errno != 0)
    أرجع crun_make_error (err, errno, "parse process start time");

  أرجع 0;
}

صحيح
libcrun_write_container_status (ثابت حرف *state_يىلتىز, ثابت حرف *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  صحيح r, ret;
  cleanup_free حرف *file_tmp = عدم;
  cleanup_free حرف *file = عدم;
  size_t len;
  cleanup_close صحيح fd_write = -1;
  ثابت حرف *buf = عدم;
  بنية pid_stat st;
  ثابت حرف *tmp;
  cleanup_json_gen json_gen_ctx *gen = عدم;

  ret = get_state_directory_status_file (&file, state_يىلتىز, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_pid_stat (status->pid, &st, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  إذا (UNLIKELY (fd_write < 0))
    أرجع crun_make_error (err, errno, "cannot open status file");

  إذا (! json_gen_init (&gen, عدم))
    أرجع crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ئاچقۇچ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_ئاچقۇچ (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_ئاچقۇچ (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_ئاچقۇچ (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_ئاچقۇچ (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_ئاچقۇچ (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_ئاچقۇچ (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_ئاچقۇچ (gen, "created");
  GEN_STR (gen, status->created);

  إذا (status->owner)
    {
      GEN_ئاچقۇچ (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_ئاچقۇچ (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_ئاچقۇچ (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  إذا (status->external_descriptors)
    {
      GEN_ئاچقۇچ (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  close_and_reset (&fd_write);

  إذا (UNLIKELY (rename (file_tmp, file) < 0))
    أرجع crun_make_error (err, errno, "cannot rename status file");

  أرجع 0;

gen_error:
  أرجع json_gen_error_to_crun_error (r, err);
}

صحيح
libcrun_read_container_status (libcrun_container_status_t *status, ثابت حرف *state_يىلتىز, ثابت حرف *id,
                               libcrun_error_t *err)
{
  cleanup_free حرف *buffer = عدم;
  صحيح ret;
  cleanup_free حرف *file = عدم;
  json_object *doc = عدم;
  json_object *tmp;
  ثابت حرف *val;

  ret = get_state_directory_status_file (&file, state_يىلتىز, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_all_file (file, &buffer, عدم, err);
  إذا (UNLIKELY (ret < 0))
    {

      إذا (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free حرف *statedir = عدم;
          libcrun_error_t tmp_err;
          صحيح tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_يىلتىز, id, &tmp_err);
          إذا (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          وإلا
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              إذا (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              وإلا إذا (tmp_ret == 0)
                أرجع crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      أرجع ret;
    }

  ret = parse_json_file (&doc, buffer, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  {
    tmp = json_object_object_get (doc, "pid");
    إذا (UNLIKELY (tmp == عدم))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        اذهب exit;
      }
    إذا (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        اذهب exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    إذا (UNLIKELY (tmp == عدم))
      status->process_start_time = 0; /* backwards compatibility */
    وإلا إذا (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        اذهب exit;
      }
    وإلا
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    إذا (UNLIKELY (tmp == عدم))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        اذهب exit;
      }
    val = json_object_get_string (tmp);
    إذا (UNLIKELY (val == عدم))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        اذهب exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : عدم;
    status->scope = val ? xstrdup (val) : عدم;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    إذا (UNLIKELY (tmp == عدم))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        اذهب exit;
      }
    val = json_object_get_string (tmp);
    إذا (UNLIKELY (val == عدم))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        اذهب exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != عدم && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    إذا (UNLIKELY (tmp == عدم))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        اذهب exit;
      }
    val = json_object_get_string (tmp);
    إذا (UNLIKELY (val == عدم))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        اذهب exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    إذا (UNLIKELY (tmp == عدم))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        اذهب exit;
      }
    val = json_object_get_string (tmp);
    إذا (UNLIKELY (val == عدم))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        اذهب exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : عدم;
    status->owner = val ? xstrdup (val) : عدم;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != عدم && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != عدم && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : عدم;
    status->external_descriptors = val ? xstrdup (val) : عدم;
  }
  ret = 0;

exit:
  json_object_put (doc);
  أرجع ret;
}

صحيح
libcrun_status_check_directories (ثابت حرف *state_يىلتىز, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *dir = عدم;
  صحيح ret;

  ret = libcrun_get_state_directory (&dir, state_يىلتىز, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_path_exists (dir, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret)
    أرجع crun_make_error (err, 0, "container `%s` already exists", id);

  إذا (UNLIKELY (mkdir (dir, 0700) < 0))
    أرجع crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  أرجع 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

ساكن صحيح
rmdirfd_move_to (DIR **d, ثابت حرف *name, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  إذا (UNLIKELY (new_d == عدم))
    أرجع crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  أرجع 0;
}

ساكن صحيح
rmdirfd (ثابت حرف *namedir, صحيح fd, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_dir DIR *d = عدم;
  بنية dirent *de;
  بنية stat يىلتىز_st;
  bool is_يىلتىز = true;
  unsigned صحيح budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close صحيح fd_cleanup = fd;

  ret = fstat (fd, &يىلتىز_st);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  إذا (d == عدم)
    أرجع crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  لكل (;;)
    {
      errno = 0;
      de = readdir (d);
      إذا (de == عدم)
        {
          بنية stat st;

          إذا (UNLIKELY (errno != 0))
            أرجع crun_make_error (err, errno, "cannot read directory");

          إذا (is_يىلتىز)
            اكسر;

          إذا (UNLIKELY (--budget == 0))
            أرجع crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          ret = fstat (dirfd (d), &st);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "stat `..`");

          is_يىلتىز = st.st_dev == يىلتىز_st.st_dev && st.st_ino == يىلتىز_st.st_ino;
          استمر;
        }

      إذا ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        استمر;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      إذا (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          إذا (ret < 0 && errno == EBUSY)
            {
              cleanup_close صحيح tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              إذا (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  إذا (umount2 (procpath, MNT_DETACH) == 0)
                    اذهب retry_unlink;
                }
              إذا (! is_يىلتىز)
                أرجع crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          وإلا إذا (ret < 0 && errno == ENOTEMPTY)
            {
              إذا (UNLIKELY (--budget == 0))
                أرجع crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;

              is_يىلتىز = false;
            }
          وإلا إذا (ret < 0 && errno != ENOENT && ! is_يىلتىز)
            {
              أرجع crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  أرجع 0;
}

صحيح
libcrun_container_delete_status (ثابت حرف *state_يىلتىز, ثابت حرف *id, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_close صحيح rundir_dfd = -1;
  cleanup_close صحيح dfd = -1;
  cleanup_free حرف *dir = عدم;

  ret = validate_id (id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = get_run_directory (&dir, state_يىلتىز, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  إذا (UNLIKELY (rundir_dfd < 0))
    أرجع crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  إذا (UNLIKELY (dfd < 0))
    أرجع crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  أرجع 0;
}

فراغ
libcrun_free_container_status (libcrun_container_status_t *status)
{
  إذا (status == عدم)
    أرجع;
  حرر (status->cgroup_path);
  حرر (status->bundle);
  حرر (status->rootfs);
  حرر (status->external_descriptors);
  حرر (status->created);
  حرر (status->scope);
  حرر (status->owner);
}

صحيح
libcrun_get_containers_list (libcrun_container_list_t **out, ثابت حرف *state_يىلتىز, libcrun_error_t *err)
{
  بنية dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = عدم;
  cleanup_free حرف *يىلتىز = عدم;
  cleanup_dir DIR *dir = عدم;
  صحيح ret;

  *out = عدم;

  ret = get_run_directory (&يىلتىز, state_يىلتىز, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dir = opendir (يىلتىز);
  إذا (UNLIKELY (dir == عدم))
    أرجع crun_make_error (err, errno, "cannot opendir `%s`", يىلتىز);

  لكل (next = readdir (dir); next; next = readdir (dir))
    {
      صحيح exists;
      cleanup_free حرف *status_file = عدم;

      libcrun_container_list_t *next_container;

      إذا (next->d_name[0] == '.')
        استمر;

      ret = append_paths (&status_file, err, يىلتىز, next->d_name, "status", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      exists = crun_path_exists (status_file, err);
      إذا (exists < 0)
        {
          أرجع exists;
        }

      إذا (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          استمر;
        }

      next_container = xmalloc (حجم (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  أرجع 0;
}

فراغ
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  طالما (list)
    {
      next = list->next;
      حرر (list->name);
      حرر (list);
      list = next;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
صحيح
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  بنية pid_stat st;
  صحيح ret;

  /* For backwards compatibility, check start time only if available. */
  إذا (! status->process_start_time)
    أرجع 1;

  ret = read_pid_stat (status->pid, &st, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (status->process_start_time != st.starttime || st.state == 'X')
    أرجع 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  إذا (st.state == 'Z' && st.رەقەم_threads <= 1)
    أرجع 0; /* stopped */

  أرجع 1; /* running, created, or paused */
}

صحيح
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  صحيح ret;

  ret = kill (status->pid, 0);
  إذا (UNLIKELY (ret < 0) && errno != ESRCH)
    أرجع crun_make_error (err, errno, "kill");

  إذا (ret == 0)
    أرجع libcrun_check_pid_valid (status, err);

  أرجع 0; /* stopped */
}

ساكن صحيح
get_exec_fifo_path (حرف **out, ثابت حرف *state_يىلتىز, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *state_dir = عدم;
  صحيح ret;

  ret = libcrun_get_state_directory (&state_dir, state_يىلتىز, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع append_paths (out, err, state_dir, "exec.fifo", عدم);
}

صحيح
libcrun_status_create_exec_fifo (ثابت حرف *state_يىلتىز, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *fifo_path = عدم;
  صحيح ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_يىلتىز, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  أرجع fd;
}

صحيح
libcrun_status_write_exec_fifo (ثابت حرف *state_يىلتىز, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *fifo_path = عدم;
  حرف buffer[1] = {
    0,
  };
  cleanup_close صحيح fd = -1;
  صحيح ret;

  ret = get_exec_fifo_path (&fifo_path, state_يىلتىز, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (اكتب (fd, buffer, 1));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "write to exec.fifo");

  أرجع 0;
}

صحيح
libcrun_status_has_read_exec_fifo (ثابت حرف *state_يىلتىز, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *fifo_path = عدم;
  صحيح ret;

  ret = get_exec_fifo_path (&fifo_path, state_يىلتىز, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع crun_path_exists (fifo_path, err);
}

بنية libcrun_status_s
{
  libcrun_container_status_t status;
  حرف *id;
  حرف *state_يىلتىز;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  صحيح running;
};

صحيح
libcrun_container_status_load (libcrun_context_t *context, ثابت حرف *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  صحيح ret;

  st = xmalloc0 (حجم (*st));

  ret = libcrun_read_container_status (&st->status, context->state_يىلتىز, id, err);
  إذا (UNLIKELY (ret < 0))
    {
      حرر (st);
      أرجع ret;
    }

  st->id = xstrdup (id);
  st->state_يىلتىز = xstrdup (context->state_يىلتىز);

  *out = st;
  أرجع 0;
}

فراغ
libcrun_container_status_free (libcrun_status_t *st)
{
  إذا (st == عدم)
    أرجع;
  libcrun_free_container_status (&st->status);
  حرر (st->state_يىلتىز);
  حرر (st->id);
  حرر (st);
}

صحيح
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, صحيح *running, libcrun_error_t *err)
{
  إذا (! st->state_resolved)
    {
      ثابت حرف *state_string = عدم;
      صحيح ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_يىلتىز, &state_string, &st->running, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (state_string == عدم)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      وإلا إذا (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      وإلا إذا (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      وإلا إذا (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      وإلا إذا (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      وإلا
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  إذا (state)
    *state = st->state;
  إذا (running)
    *running = st->running;
  أرجع 0;
}

ثابت حرف *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  بدل (state)
    {
    حالة LIBCRUN_CONTAINER_STATUS_CREATING:
      أرجع "creating";
    حالة LIBCRUN_CONTAINER_STATUS_CREATED:
      أرجع "created";
    حالة LIBCRUN_CONTAINER_STATUS_RUNNING:
      أرجع "running";
    حالة LIBCRUN_CONTAINER_STATUS_PAUSED:
      أرجع "paused";
    حالة LIBCRUN_CONTAINER_STATUS_STOPPED:
      أرجع "stopped";
    }
  أرجع "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  أرجع st->status.pid;
}

ثابت حرف *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  أرجع st->status.bundle;
}

ثابت حرف *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  أرجع st->status.rootfs;
}

ثابت حرف *
libcrun_status_get_created (libcrun_status_t *st)
{
  أرجع st->status.created;
}

ثابت حرف *
libcrun_status_get_owner (libcrun_status_t *st)
{
  أرجع st->status.owner;
}

ثابت حرف *
libcrun_status_get_scope (libcrun_status_t *st)
{
  أرجع st->status.scope;
}

ثابت حرف *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  أرجع st->status.external_descriptors;
}

صحيح
libcrun_container_get_state_string (libcrun_context_t *context, ثابت حرف *id, ثابت حرف **out, صحيح *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  صحيح local_running;
  صحيح ret;

  ret = libcrun_read_container_status (&status, context->state_يىلتىز, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_يىلتىز, out, &local_running, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (running)
    *running = local_running;
  أرجع 0;
}

صحيح
libcrun_get_state_dir (حرف **out, ثابت حرف *state_يىلتىز, ثابت حرف *id, libcrun_error_t *err)
{
  أرجع libcrun_get_state_directory (out, state_يىلتىز, id, err);
}

صحيح
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  أرجع libcrun_get_containers_list (out, context->state_يىلتىز, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (حجم (*it));
  it->cur = l;
  أرجع it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, ثابت حرف **id)
{
  إذا (it == عدم || it->cur == عدم)
    أرجع false;

  إذا (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  أرجع true;
}

فراغ
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  حرر (it);
}

فراغ
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
