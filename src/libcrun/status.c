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
#অন্তর্ভাব "status.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "json_gen_utils.h"
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <json-c/json.h>
#অন্তর্ভাব <ocispec/json_common.h>
#অন্তর্ভাব <sys/mount.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <dirent.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <limits.h>

#define STEAL_POINTER(x, y) \
  করো                        \
    {                       \
      *x = y;               \
      y = ফাঁকা;             \
  } যখন (0)

কাঠামো pid_stat
{
  অক্ষর state;
  long অঙ্ক_threads;
  unsigned long long starttime;
};

স্থির অন্তঃরেখ bool
is_valid_id_char (অক্ষর c)
{
  যদি ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    ফেরত true;
  ফেরত c == '_' || c == '+' || c == '.' || c == '-';
}

স্থির পূর্ণসংখ্যা
validate_id (ধ্রুবক অক্ষর *id, libcrun_error_t *err)
{
  যদি (id == ফাঁকা)
    ফেরত 0;

  যদি (id[0] == '\0')
    ফেরত crun_make_error (err, 0, "empty container ID");

  যদি (id[0] == '.')
    ফেরত crun_make_error (err, 0, "invalid container ID `%s`", id);

  জন্য (size_t i = 0; id[i]; i++)
    {
      যদি (! is_valid_id_char (id[i]))
        ফেরত crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
get_run_directory (অক্ষর **out, ধ্রুবক অক্ষর *state_মূল, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  cleanup_free অক্ষর *মূল = ফাঁকা;

  যদি (state_মূল)
    মূল = xstrdup (state_মূল);
  যদি (মূল == ফাঁকা)
    {
      ধ্রুবক অক্ষর *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      যদি (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&মূল, err, runtime_dir, "crun", ফাঁকা);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
        }
    }
  যদি (মূল == ফাঁকা)
    মূল = xstrdup ("/run/crun");

  ret = crun_ensure_directory (মূল, 0700, false, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  STEAL_POINTER (out, মূল);

  ফেরত 0;
}

পূর্ণসংখ্যা
get_shared_empty_directory_path (অক্ষর **out, ধ্রুবক অক্ষর *state_মূল, libcrun_error_t *err)
{
  cleanup_free অক্ষর *run_dir = ফাঁকা;
  পূর্ণসংখ্যা ret;

  ret = get_run_directory (&run_dir, state_মূল, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_get_state_directory (অক্ষর **out, ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  cleanup_free অক্ষর *path = ফাঁকা;
  cleanup_free অক্ষর *মূল = ফাঁকা;

  ret = validate_id (id, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = get_run_directory (&মূল, state_মূল, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = append_paths (&path, err, মূল, id, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  STEAL_POINTER (out, path);

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
get_state_directory_status_file (অক্ষর **out, ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err)
{
  cleanup_free অক্ষর *dir = ফাঁকা;
  পূর্ণসংখ্যা ret;

  ret = libcrun_get_state_directory (&dir, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত append_paths (out, err, dir, "status", ফাঁকা);
}

স্থির পূর্ণসংখ্যা
read_pid_stat (pid_t pid, কাঠামো pid_stat *st, libcrun_error_t *err)
{
  cleanup_free অক্ষর *বাফার = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা fd = -1;
  অক্ষর pid_stat_file[64];
  অক্ষর *it, *s;
  পূর্ণসংখ্যা i, ret;

  ret = snprintf (pid_stat_file, আকার (pid_stat_file), "/proc/%d/stat", pid);
  যদি (UNLIKELY (ret >= (পূর্ণসংখ্যা) আকার (pid_stat_file)))
    ফেরত crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  যদি (fd < 0)
    {
      /* The process already exited.  */
      যদি (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, আকার (*st));
          ফেরত 0;
        }
      ফেরত crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &বাফার, ফাঁকা, err);
  যদি (ret < 0)
    {
      st->starttime = 0;
      st->অঙ্ক_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      ফেরত 0;
    }

  s = ফাঁকা;

  /* Skip the first two arguments.  */
  জন্য (it = বাফার; it; it = strchr (it + 1, ')'))
    s = it + 1;

  যদি (s)
    যখন (*s == ' ')
      s++;

  যদি (s == ফাঁকা || *s == '\0')
    ফেরত crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  জন্য (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      যদি (it == ফাঁকা)
        ফেরত crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      যদি (i == 16)
        st->অঙ্ক_threads = strtol (it, ফাঁকা, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, ফাঁকা, 10);
  যদি (errno != 0)
    ফেরত crun_make_error (err, errno, "parse process start time");

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_write_container_status (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  পূর্ণসংখ্যা r, ret;
  cleanup_free অক্ষর *file_tmp = ফাঁকা;
  cleanup_free অক্ষর *file = ফাঁকা;
  size_t len;
  cleanup_close পূর্ণসংখ্যা fd_write = -1;
  ধ্রুবক অক্ষর *buf = ফাঁকা;
  কাঠামো pid_stat st;
  ধ্রুবক অক্ষর *tmp;
  cleanup_json_gen json_gen_ctx *gen = ফাঁকা;

  ret = get_state_directory_status_file (&file, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = read_pid_stat (status->pid, &st, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  যদি (UNLIKELY (fd_write < 0))
    ফেরত crun_make_error (err, errno, "cannot open status file");

  যদি (! json_gen_init (&gen, ফাঁকা))
    ফেরত crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_চাবি (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_চাবি (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_চাবি (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_চাবি (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_চাবি (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_চাবি (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_চাবি (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_চাবি (gen, "created");
  GEN_STR (gen, status->created);

  যদি (status->owner)
    {
      GEN_চাবি (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_চাবি (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_চাবি (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  যদি (status->external_descriptors)
    {
      GEN_চাবি (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  close_and_reset (&fd_write);

  যদি (UNLIKELY (rename (file_tmp, file) < 0))
    ফেরত crun_make_error (err, errno, "cannot rename status file");

  ফেরত 0;

gen_error:
  ফেরত json_gen_error_to_crun_error (r, err);
}

পূর্ণসংখ্যা
libcrun_read_container_status (libcrun_container_status_t *status, ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id,
                               libcrun_error_t *err)
{
  cleanup_free অক্ষর *বাফার = ফাঁকা;
  পূর্ণসংখ্যা ret;
  cleanup_free অক্ষর *file = ফাঁকা;
  json_object *doc = ফাঁকা;
  json_object *tmp;
  ধ্রুবক অক্ষর *val;

  ret = get_state_directory_status_file (&file, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = read_all_file (file, &বাফার, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    {

      যদি (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free অক্ষর *statedir = ফাঁকা;
          libcrun_error_t tmp_err;
          পূর্ণসংখ্যা tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_মূল, id, &tmp_err);
          যদি (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          অন্যথা
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              যদি (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              অন্যথা যদি (tmp_ret == 0)
                ফেরত crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      ফেরত ret;
    }

  ret = parse_json_file (&doc, বাফার, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  {
    tmp = json_object_object_get (doc, "pid");
    যদি (UNLIKELY (tmp == ফাঁকা))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        যাও exit;
      }
    যদি (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        যাও exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    যদি (UNLIKELY (tmp == ফাঁকা))
      status->process_start_time = 0; /* backwards compatibility */
    অন্যথা যদি (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        যাও exit;
      }
    অন্যথা
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    যদি (UNLIKELY (tmp == ফাঁকা))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        যাও exit;
      }
    val = json_object_get_string (tmp);
    যদি (UNLIKELY (val == ফাঁকা))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        যাও exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : ফাঁকা;
    status->scope = val ? xstrdup (val) : ফাঁকা;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    যদি (UNLIKELY (tmp == ফাঁকা))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        যাও exit;
      }
    val = json_object_get_string (tmp);
    যদি (UNLIKELY (val == ফাঁকা))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        যাও exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != ফাঁকা && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    যদি (UNLIKELY (tmp == ফাঁকা))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        যাও exit;
      }
    val = json_object_get_string (tmp);
    যদি (UNLIKELY (val == ফাঁকা))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        যাও exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    যদি (UNLIKELY (tmp == ফাঁকা))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        যাও exit;
      }
    val = json_object_get_string (tmp);
    যদি (UNLIKELY (val == ফাঁকা))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        যাও exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : ফাঁকা;
    status->owner = val ? xstrdup (val) : ফাঁকা;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != ফাঁকা && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != ফাঁকা && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : ফাঁকা;
    status->external_descriptors = val ? xstrdup (val) : ফাঁকা;
  }
  ret = 0;

exit:
  json_object_put (doc);
  ফেরত ret;
}

পূর্ণসংখ্যা
libcrun_status_check_directories (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err)
{
  cleanup_free অক্ষর *dir = ফাঁকা;
  পূর্ণসংখ্যা ret;

  ret = libcrun_get_state_directory (&dir, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = crun_path_exists (dir, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (ret)
    ফেরত crun_make_error (err, 0, "container `%s` already exists", id);

  যদি (UNLIKELY (mkdir (dir, 0700) < 0))
    ফেরত crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  ফেরত 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

স্থির পূর্ণসংখ্যা
rmdirfd_move_to (DIR **d, ধ্রুবক অক্ষর *name, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  যদি (UNLIKELY (new_d == ফাঁকা))
    ফেরত crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
rmdirfd (ধ্রুবক অক্ষর *namedir, পূর্ণসংখ্যা fd, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  cleanup_dir DIR *d = ফাঁকা;
  কাঠামো dirent *de;
  কাঠামো stat মূল_st;
  bool is_মূল = true;
  unsigned পূর্ণসংখ্যা budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close পূর্ণসংখ্যা fd_cleanup = fd;

  ret = fstat (fd, &মূল_st);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  যদি (d == ফাঁকা)
    ফেরত crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  জন্য (;;)
    {
      errno = 0;
      de = readdir (d);
      যদি (de == ফাঁকা)
        {
          কাঠামো stat st;

          যদি (UNLIKELY (errno != 0))
            ফেরত crun_make_error (err, errno, "cannot read directory");

          যদি (is_মূল)
            বিরতি;

          যদি (UNLIKELY (--budget == 0))
            ফেরত crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;

          ret = fstat (dirfd (d), &st);
          যদি (UNLIKELY (ret < 0))
            ফেরত crun_make_error (err, errno, "stat `..`");

          is_মূল = st.st_dev == মূল_st.st_dev && st.st_ino == মূল_st.st_ino;
          চালু;
        }

      যদি ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        চালু;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      যদি (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          যদি (ret < 0 && errno == EBUSY)
            {
              cleanup_close পূর্ণসংখ্যা tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              যদি (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  যদি (umount2 (procpath, MNT_DETACH) == 0)
                    যাও retry_unlink;
                }
              যদি (! is_মূল)
                ফেরত crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          অন্যথা যদি (ret < 0 && errno == ENOTEMPTY)
            {
              যদি (UNLIKELY (--budget == 0))
                ফেরত crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              যদি (UNLIKELY (ret < 0))
                ফেরত ret;

              is_মূল = false;
            }
          অন্যথা যদি (ret < 0 && errno != ENOENT && ! is_মূল)
            {
              ফেরত crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_container_delete_status (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  cleanup_close পূর্ণসংখ্যা rundir_dfd = -1;
  cleanup_close পূর্ণসংখ্যা dfd = -1;
  cleanup_free অক্ষর *dir = ফাঁকা;

  ret = validate_id (id, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = get_run_directory (&dir, state_মূল, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  rundir_dfd = অস্থায়ী_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  যদি (UNLIKELY (rundir_dfd < 0))
    ফেরত crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  যদি (UNLIKELY (dfd < 0))
    ফেরত crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  ফেরত 0;
}

শূন্য
libcrun_free_container_status (libcrun_container_status_t *status)
{
  যদি (status == ফাঁকা)
    ফেরত;
  মুক্ত (status->cgroup_path);
  মুক্ত (status->bundle);
  মুক্ত (status->rootfs);
  মুক্ত (status->external_descriptors);
  মুক্ত (status->created);
  মুক্ত (status->scope);
  মুক্ত (status->owner);
}

পূর্ণসংখ্যা
libcrun_get_containers_list (libcrun_container_list_t **out, ধ্রুবক অক্ষর *state_মূল, libcrun_error_t *err)
{
  কাঠামো dirent *পরবর্তী;
  cleanup_container_list libcrun_container_list_t *tmp = ফাঁকা;
  cleanup_free অক্ষর *মূল = ফাঁকা;
  cleanup_dir DIR *dir = ফাঁকা;
  পূর্ণসংখ্যা ret;

  *out = ফাঁকা;

  ret = get_run_directory (&মূল, state_মূল, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  dir = opendir (মূল);
  যদি (UNLIKELY (dir == ফাঁকা))
    ফেরত crun_make_error (err, errno, "cannot opendir `%s`", মূল);

  জন্য (পরবর্তী = readdir (dir); পরবর্তী; পরবর্তী = readdir (dir))
    {
      পূর্ণসংখ্যা exists;
      cleanup_free অক্ষর *status_file = ফাঁকা;

      libcrun_container_list_t *পরবর্তী_container;

      যদি (পরবর্তী->d_name[0] == '.')
        চালু;

      ret = append_paths (&status_file, err, মূল, পরবর্তী->d_name, "status", ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      exists = crun_path_exists (status_file, err);
      যদি (exists < 0)
        {
          ফেরত exists;
        }

      যদি (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          চালু;
        }

      পরবর্তী_container = xmalloc (আকার (libcrun_container_list_t));
      পরবর্তী_container->name = xstrdup (পরবর্তী->d_name);
      পরবর্তী_container->পরবর্তী = tmp;
      tmp = পরবর্তী_container;
    }

  STEAL_POINTER (out, tmp);

  ফেরত 0;
}

শূন্য
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *পরবর্তী;
  যখন (list)
    {
      পরবর্তী = list->পরবর্তী;
      মুক্ত (list->name);
      মুক্ত (list);
      list = পরবর্তী;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
পূর্ণসংখ্যা
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  কাঠামো pid_stat st;
  পূর্ণসংখ্যা ret;

  /* For backwards compatibility, check start time only if available. */
  যদি (! status->process_start_time)
    ফেরত 1;

  ret = read_pid_stat (status->pid, &st, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (status->process_start_time != st.starttime || st.state == 'X')
    ফেরত 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  যদি (st.state == 'Z' && st.অঙ্ক_threads <= 1)
    ফেরত 0; /* stopped */

  ফেরত 1; /* running, created, or paused */
}

পূর্ণসংখ্যা
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  ret = kill (status->pid, 0);
  যদি (UNLIKELY (ret < 0) && errno != ESRCH)
    ফেরত crun_make_error (err, errno, "kill");

  যদি (ret == 0)
    ফেরত libcrun_check_pid_valid (status, err);

  ফেরত 0; /* stopped */
}

স্থির পূর্ণসংখ্যা
get_exec_fifo_path (অক্ষর **out, ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err)
{
  cleanup_free অক্ষর *state_dir = ফাঁকা;
  পূর্ণসংখ্যা ret;

  ret = libcrun_get_state_directory (&state_dir, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত append_paths (out, err, state_dir, "exec.fifo", ফাঁকা);
}

পূর্ণসংখ্যা
libcrun_status_create_exec_fifo (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err)
{
  cleanup_free অক্ষর *fifo_path = ফাঁকা;
  পূর্ণসংখ্যা ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  ফেরত fd;
}

পূর্ণসংখ্যা
libcrun_status_write_exec_fifo (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err)
{
  cleanup_free অক্ষর *fifo_path = ফাঁকা;
  অক্ষর বাফার[1] = {
    0,
  };
  cleanup_close পূর্ণসংখ্যা fd = -1;
  পূর্ণসংখ্যা ret;

  ret = get_exec_fifo_path (&fifo_path, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = অস্থায়ী_FAILURE_RETRY (লিখ (fd, বাফার, 1));
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "write to exec.fifo");

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_status_has_read_exec_fifo (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err)
{
  cleanup_free অক্ষর *fifo_path = ফাঁকা;
  পূর্ণসংখ্যা ret;

  ret = get_exec_fifo_path (&fifo_path, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত crun_path_exists (fifo_path, err);
}

কাঠামো libcrun_status_s
{
  libcrun_container_status_t status;
  অক্ষর *id;
  অক্ষর *state_মূল;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  পূর্ণসংখ্যা running;
};

পূর্ণসংখ্যা
libcrun_container_status_load (libcrun_context_t *context, ধ্রুবক অক্ষর *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  পূর্ণসংখ্যা ret;

  st = xmalloc0 (আকার (*st));

  ret = libcrun_read_container_status (&st->status, context->state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    {
      মুক্ত (st);
      ফেরত ret;
    }

  st->id = xstrdup (id);
  st->state_মূল = xstrdup (context->state_মূল);

  *out = st;
  ফেরত 0;
}

শূন্য
libcrun_container_status_free (libcrun_status_t *st)
{
  যদি (st == ফাঁকা)
    ফেরত;
  libcrun_free_container_status (&st->status);
  মুক্ত (st->state_মূল);
  মুক্ত (st->id);
  মুক্ত (st);
}

পূর্ণসংখ্যা
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, পূর্ণসংখ্যা *running, libcrun_error_t *err)
{
  যদি (! st->state_resolved)
    {
      ধ্রুবক অক্ষর *state_string = ফাঁকা;
      পূর্ণসংখ্যা ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_মূল, &state_string, &st->running, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      যদি (state_string == ফাঁকা)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      অন্যথা যদি (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      অন্যথা যদি (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      অন্যথা যদি (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      অন্যথা যদি (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      অন্যথা
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  যদি (state)
    *state = st->state;
  যদি (running)
    *running = st->running;
  ফেরত 0;
}

ধ্রুবক অক্ষর *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  নির্বাচন (state)
    {
    ক্ষেত্র LIBCRUN_CONTAINER_STATUS_CREATING:
      ফেরত "creating";
    ক্ষেত্র LIBCRUN_CONTAINER_STATUS_CREATED:
      ফেরত "created";
    ক্ষেত্র LIBCRUN_CONTAINER_STATUS_RUNNING:
      ফেরত "running";
    ক্ষেত্র LIBCRUN_CONTAINER_STATUS_PAUSED:
      ফেরত "paused";
    ক্ষেত্র LIBCRUN_CONTAINER_STATUS_STOPPED:
      ফেরত "stopped";
    }
  ফেরত "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  ফেরত st->status.pid;
}

ধ্রুবক অক্ষর *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  ফেরত st->status.bundle;
}

ধ্রুবক অক্ষর *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  ফেরত st->status.rootfs;
}

ধ্রুবক অক্ষর *
libcrun_status_get_created (libcrun_status_t *st)
{
  ফেরত st->status.created;
}

ধ্রুবক অক্ষর *
libcrun_status_get_owner (libcrun_status_t *st)
{
  ফেরত st->status.owner;
}

ধ্রুবক অক্ষর *
libcrun_status_get_scope (libcrun_status_t *st)
{
  ফেরত st->status.scope;
}

ধ্রুবক অক্ষর *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  ফেরত st->status.external_descriptors;
}

পূর্ণসংখ্যা
libcrun_container_get_state_string (libcrun_context_t *context, ধ্রুবক অক্ষর *id, ধ্রুবক অক্ষর **out, পূর্ণসংখ্যা *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  পূর্ণসংখ্যা local_running;
  পূর্ণসংখ্যা ret;

  ret = libcrun_read_container_status (&status, context->state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_মূল, out, &local_running, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (running)
    *running = local_running;
  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_get_state_dir (অক্ষর **out, ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err)
{
  ফেরত libcrun_get_state_directory (out, state_মূল, id, err);
}

পূর্ণসংখ্যা
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  ফেরত libcrun_get_containers_list (out, context->state_মূল, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (আকার (*it));
  it->cur = l;
  ফেরত it;
}

bool
libcrun_container_iter_পরবর্তী (libcrun_container_iter_t *it, ধ্রুবক অক্ষর **id)
{
  যদি (it == ফাঁকা || it->cur == ফাঁকা)
    ফেরত false;

  যদি (id)
    *id = it->cur->name;
  it->cur = it->cur->পরবর্তী;
  ফেরত true;
}

শূন্য
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  মুক্ত (it);
}

শূন্য
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
