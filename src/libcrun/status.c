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
  কুরু                        \
    {                       \
      *x = y;               \
      y = রিক্ত;             \
  } যাবত্ (0)

সংরচনা pid_stat
{
  বর্ণ state;
  long অঙ্ক_threads;
  unsigned long long starttime;
};

স্থির অন্তঃস্থ bool
is_valid_id_char (বর্ণ c)
{
  যদি ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    প্রত্যাগম true;
  প্রত্যাগম c == '_' || c == '+' || c == '.' || c == '-';
}

স্থির পূর্ণাঙ্ক
validate_id (ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  যদি (id == রিক্ত)
    প্রত্যাগম 0;

  যদি (id[0] == '\0')
    প্রত্যাগম crun_make_error (err, 0, "empty container ID");

  যদি (id[0] == '.')
    প্রত্যাগম crun_make_error (err, 0, "invalid container ID `%s`", id);

  প্রতি (size_t i = 0; id[i]; i++)
    {
      যদি (! is_valid_id_char (id[i]))
        প্রত্যাগম crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
get_run_directory (বর্ণ **out, ধ্রুব বর্ণ *state_মূল, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_free বর্ণ *মূল = রিক্ত;

  যদি (state_মূল)
    মূল = xstrdup (state_মূল);
  যদি (মূল == রিক্ত)
    {
      ধ্রুব বর্ণ *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      যদি (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&মূল, err, runtime_dir, "crun", রিক্ত);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }
    }
  যদি (মূল == রিক্ত)
    মূল = xstrdup ("/run/crun");

  ret = crun_ensure_directory (মূল, 0700, false, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  STEAL_POINTER (out, মূল);

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
get_shared_empty_directory_path (বর্ণ **out, ধ্রুব বর্ণ *state_মূল, libcrun_error_t *err)
{
  cleanup_free বর্ণ *run_dir = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = get_run_directory (&run_dir, state_মূল, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_get_state_directory (বর্ণ **out, ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_free বর্ণ *path = রিক্ত;
  cleanup_free বর্ণ *মূল = রিক্ত;

  ret = validate_id (id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = get_run_directory (&মূল, state_মূল, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = append_paths (&path, err, মূল, id, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  STEAL_POINTER (out, path);

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
get_state_directory_status_file (বর্ণ **out, ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  cleanup_free বর্ণ *dir = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = libcrun_get_state_directory (&dir, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম append_paths (out, err, dir, "status", রিক্ত);
}

স্থির পূর্ণাঙ্ক
read_pid_stat (pid_t pid, সংরচনা pid_stat *st, libcrun_error_t *err)
{
  cleanup_free বর্ণ *buffer = রিক্ত;
  cleanup_close পূর্ণাঙ্ক fd = -1;
  বর্ণ pid_stat_file[64];
  বর্ণ *it, *s;
  পূর্ণাঙ্ক i, ret;

  ret = snprintf (pid_stat_file, পরিমাণ (pid_stat_file), "/proc/%d/stat", pid);
  যদি (UNLIKELY (ret >= (পূর্ণাঙ্ক) পরিমাণ (pid_stat_file)))
    প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  যদি (fd < 0)
    {
      /* The process already exited.  */
      যদি (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, পরিমাণ (*st));
          প্রত্যাগম 0;
        }
      প্রত্যাগম crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, রিক্ত, err);
  যদি (ret < 0)
    {
      st->starttime = 0;
      st->অঙ্ক_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      প্রত্যাগম 0;
    }

  s = রিক্ত;

  /* Skip the first two arguments.  */
  প্রতি (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  যদি (s)
    যাবত্ (*s == ' ')
      s++;

  যদি (s == রিক্ত || *s == '\0')
    প্রত্যাগম crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  প্রতি (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      যদি (it == রিক্ত)
        প্রত্যাগম crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      যদি (i == 16)
        st->অঙ্ক_threads = strtol (it, রিক্ত, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, রিক্ত, 10);
  যদি (errno != 0)
    প্রত্যাগম crun_make_error (err, errno, "parse process start time");

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_write_container_status (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  পূর্ণাঙ্ক r, ret;
  cleanup_free বর্ণ *file_tmp = রিক্ত;
  cleanup_free বর্ণ *file = রিক্ত;
  size_t len;
  cleanup_close পূর্ণাঙ্ক fd_write = -1;
  ধ্রুব বর্ণ *buf = রিক্ত;
  সংরচনা pid_stat st;
  ধ্রুব বর্ণ *tmp;
  cleanup_json_gen json_gen_ctx *gen = রিক্ত;

  ret = get_state_directory_status_file (&file, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = read_pid_stat (status->pid, &st, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

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
    প্রত্যাগম crun_make_error (err, errno, "cannot open status file");

  যদি (! json_gen_init (&gen, রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "json_gen_init failed");

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
    প্রত্যাগম ret;

  close_and_reset (&fd_write);

  যদি (UNLIKELY (rename (file_tmp, file) < 0))
    প্রত্যাগম crun_make_error (err, errno, "cannot rename status file");

  প্রত্যাগম 0;

gen_error:
  প্রত্যাগম json_gen_error_to_crun_error (r, err);
}

পূর্ণাঙ্ক
libcrun_read_container_status (libcrun_container_status_t *status, ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id,
                               libcrun_error_t *err)
{
  cleanup_free বর্ণ *buffer = রিক্ত;
  পূর্ণাঙ্ক ret;
  cleanup_free বর্ণ *file = রিক্ত;
  json_object *doc = রিক্ত;
  json_object *tmp;
  ধ্রুব বর্ণ *val;

  ret = get_state_directory_status_file (&file, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = read_all_file (file, &buffer, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    {

      যদি (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free বর্ণ *statedir = রিক্ত;
          libcrun_error_t tmp_err;
          পূর্ণাঙ্ক tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_মূল, id, &tmp_err);
          যদি (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          অন্যথা
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              যদি (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              অন্যথা যদি (tmp_ret == 0)
                প্রত্যাগম crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      প্রত্যাগম ret;
    }

  ret = parse_json_file (&doc, buffer, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  {
    tmp = json_object_object_get (doc, "pid");
    যদি (UNLIKELY (tmp == রিক্ত))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        গচ্ছ exit;
      }
    যদি (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        গচ্ছ exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    যদি (UNLIKELY (tmp == রিক্ত))
      status->process_start_time = 0; /* backwards compatibility */
    অন্যথা যদি (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        গচ্ছ exit;
      }
    অন্যথা
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    যদি (UNLIKELY (tmp == রিক্ত))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        গচ্ছ exit;
      }
    val = json_object_get_string (tmp);
    যদি (UNLIKELY (val == রিক্ত))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        গচ্ছ exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : রিক্ত;
    status->scope = val ? xstrdup (val) : রিক্ত;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    যদি (UNLIKELY (tmp == রিক্ত))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        গচ্ছ exit;
      }
    val = json_object_get_string (tmp);
    যদি (UNLIKELY (val == রিক্ত))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        গচ্ছ exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != রিক্ত && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    যদি (UNLIKELY (tmp == রিক্ত))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        গচ্ছ exit;
      }
    val = json_object_get_string (tmp);
    যদি (UNLIKELY (val == রিক্ত))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        গচ্ছ exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    যদি (UNLIKELY (tmp == রিক্ত))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        গচ্ছ exit;
      }
    val = json_object_get_string (tmp);
    যদি (UNLIKELY (val == রিক্ত))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        গচ্ছ exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : রিক্ত;
    status->owner = val ? xstrdup (val) : রিক্ত;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != রিক্ত && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != রিক্ত && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : রিক্ত;
    status->external_descriptors = val ? xstrdup (val) : রিক্ত;
  }
  ret = 0;

exit:
  json_object_put (doc);
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
libcrun_status_check_directories (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  cleanup_free বর্ণ *dir = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = libcrun_get_state_directory (&dir, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = crun_path_exists (dir, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (ret)
    প্রত্যাগম crun_make_error (err, 0, "container `%s` already exists", id);

  যদি (UNLIKELY (mkdir (dir, 0700) < 0))
    প্রত্যাগম crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  প্রত্যাগম 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

স্থির পূর্ণাঙ্ক
rmdirfd_move_to (DIR **d, ধ্রুব বর্ণ *name, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  যদি (UNLIKELY (new_d == রিক্ত))
    প্রত্যাগম crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
rmdirfd (ধ্রুব বর্ণ *namedir, পূর্ণাঙ্ক fd, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_dir DIR *d = রিক্ত;
  সংরচনা dirent *de;
  সংরচনা stat মূল_st;
  bool is_মূল = true;
  unsigned পূর্ণাঙ্ক budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close পূর্ণাঙ্ক fd_cleanup = fd;

  ret = fstat (fd, &মূল_st);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  যদি (d == রিক্ত)
    প্রত্যাগম crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  প্রতি (;;)
    {
      errno = 0;
      de = readdir (d);
      যদি (de == রিক্ত)
        {
          সংরচনা stat st;

          যদি (UNLIKELY (errno != 0))
            প্রত্যাগম crun_make_error (err, errno, "cannot read directory");

          যদি (is_মূল)
            ভঙ্গ;

          যদি (UNLIKELY (--budget == 0))
            প্রত্যাগম crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;

          ret = fstat (dirfd (d), &st);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "stat `..`");

          is_মূল = st.st_dev == মূল_st.st_dev && st.st_ino == মূল_st.st_ino;
          অনুবর্ত;
        }

      যদি ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        অনুবর্ত;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      যদি (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          যদি (ret < 0 && errno == EBUSY)
            {
              cleanup_close পূর্ণাঙ্ক tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              যদি (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  যদি (umount2 (procpath, MNT_DETACH) == 0)
                    গচ্ছ retry_unlink;
                }
              যদি (! is_মূল)
                প্রত্যাগম crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          অন্যথা যদি (ret < 0 && errno == ENOTEMPTY)
            {
              যদি (UNLIKELY (--budget == 0))
                প্রত্যাগম crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              যদি (UNLIKELY (ret < 0))
                প্রত্যাগম ret;

              is_মূল = false;
            }
          অন্যথা যদি (ret < 0 && errno != ENOENT && ! is_মূল)
            {
              প্রত্যাগম crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_container_delete_status (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_close পূর্ণাঙ্ক rundir_dfd = -1;
  cleanup_close পূর্ণাঙ্ক dfd = -1;
  cleanup_free বর্ণ *dir = রিক্ত;

  ret = validate_id (id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = get_run_directory (&dir, state_মূল, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  যদি (UNLIKELY (rundir_dfd < 0))
    প্রত্যাগম crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  যদি (UNLIKELY (dfd < 0))
    প্রত্যাগম crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  প্রত্যাগম 0;
}

শূন্য
libcrun_free_container_status (libcrun_container_status_t *status)
{
  যদি (status == রিক্ত)
    প্রত্যাগম;
  মুক্ত (status->cgroup_path);
  মুক্ত (status->bundle);
  মুক্ত (status->rootfs);
  মুক্ত (status->external_descriptors);
  মুক্ত (status->created);
  মুক্ত (status->scope);
  মুক্ত (status->owner);
}

পূর্ণাঙ্ক
libcrun_get_containers_list (libcrun_container_list_t **out, ধ্রুব বর্ণ *state_মূল, libcrun_error_t *err)
{
  সংরচনা dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = রিক্ত;
  cleanup_free বর্ণ *মূল = রিক্ত;
  cleanup_dir DIR *dir = রিক্ত;
  পূর্ণাঙ্ক ret;

  *out = রিক্ত;

  ret = get_run_directory (&মূল, state_মূল, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  dir = opendir (মূল);
  যদি (UNLIKELY (dir == রিক্ত))
    প্রত্যাগম crun_make_error (err, errno, "cannot opendir `%s`", মূল);

  প্রতি (next = readdir (dir); next; next = readdir (dir))
    {
      পূর্ণাঙ্ক exists;
      cleanup_free বর্ণ *status_file = রিক্ত;

      libcrun_container_list_t *next_container;

      যদি (next->d_name[0] == '.')
        অনুবর্ত;

      ret = append_paths (&status_file, err, মূল, next->d_name, "status", রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      exists = crun_path_exists (status_file, err);
      যদি (exists < 0)
        {
          প্রত্যাগম exists;
        }

      যদি (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          অনুবর্ত;
        }

      next_container = xmalloc (পরিমাণ (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  প্রত্যাগম 0;
}

শূন্য
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  যাবত্ (list)
    {
      next = list->next;
      মুক্ত (list->name);
      মুক্ত (list);
      list = next;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
পূর্ণাঙ্ক
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  সংরচনা pid_stat st;
  পূর্ণাঙ্ক ret;

  /* For backwards compatibility, check start time only if available. */
  যদি (! status->process_start_time)
    প্রত্যাগম 1;

  ret = read_pid_stat (status->pid, &st, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (status->process_start_time != st.starttime || st.state == 'X')
    প্রত্যাগম 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  যদি (st.state == 'Z' && st.অঙ্ক_threads <= 1)
    প্রত্যাগম 0; /* stopped */

  প্রত্যাগম 1; /* running, created, or paused */
}

পূর্ণাঙ্ক
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = kill (status->pid, 0);
  যদি (UNLIKELY (ret < 0) && errno != ESRCH)
    প্রত্যাগম crun_make_error (err, errno, "kill");

  যদি (ret == 0)
    প্রত্যাগম libcrun_check_pid_valid (status, err);

  প্রত্যাগম 0; /* stopped */
}

স্থির পূর্ণাঙ্ক
get_exec_fifo_path (বর্ণ **out, ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  cleanup_free বর্ণ *state_dir = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = libcrun_get_state_directory (&state_dir, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম append_paths (out, err, state_dir, "exec.fifo", রিক্ত);
}

পূর্ণাঙ্ক
libcrun_status_create_exec_fifo (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  cleanup_free বর্ণ *fifo_path = রিক্ত;
  পূর্ণাঙ্ক ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  প্রত্যাগম fd;
}

পূর্ণাঙ্ক
libcrun_status_write_exec_fifo (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  cleanup_free বর্ণ *fifo_path = রিক্ত;
  বর্ণ buffer[1] = {
    0,
  };
  cleanup_close পূর্ণাঙ্ক fd = -1;
  পূর্ণাঙ্ক ret;

  ret = get_exec_fifo_path (&fifo_path, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    প্রত্যাগম crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (লিখ (fd, buffer, 1));
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "write to exec.fifo");

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_status_has_read_exec_fifo (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  cleanup_free বর্ণ *fifo_path = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = get_exec_fifo_path (&fifo_path, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম crun_path_exists (fifo_path, err);
}

সংরচনা libcrun_status_s
{
  libcrun_container_status_t status;
  বর্ণ *id;
  বর্ণ *state_মূল;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  পূর্ণাঙ্ক running;
};

পূর্ণাঙ্ক
libcrun_container_status_load (libcrun_context_t *context, ধ্রুব বর্ণ *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  পূর্ণাঙ্ক ret;

  st = xmalloc0 (পরিমাণ (*st));

  ret = libcrun_read_container_status (&st->status, context->state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    {
      মুক্ত (st);
      প্রত্যাগম ret;
    }

  st->id = xstrdup (id);
  st->state_মূল = xstrdup (context->state_মূল);

  *out = st;
  প্রত্যাগম 0;
}

শূন্য
libcrun_container_status_free (libcrun_status_t *st)
{
  যদি (st == রিক্ত)
    প্রত্যাগম;
  libcrun_free_container_status (&st->status);
  মুক্ত (st->state_মূল);
  মুক্ত (st->id);
  মুক্ত (st);
}

পূর্ণাঙ্ক
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, পূর্ণাঙ্ক *running, libcrun_error_t *err)
{
  যদি (! st->state_resolved)
    {
      ধ্রুব বর্ণ *state_string = রিক্ত;
      পূর্ণাঙ্ক ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_মূল, &state_string, &st->running, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      যদি (state_string == রিক্ত)
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
  প্রত্যাগম 0;
}

ধ্রুব বর্ণ *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  বিকল্প (state)
    {
    পক্ষ LIBCRUN_CONTAINER_STATUS_CREATING:
      প্রত্যাগম "creating";
    পক্ষ LIBCRUN_CONTAINER_STATUS_CREATED:
      প্রত্যাগম "created";
    পক্ষ LIBCRUN_CONTAINER_STATUS_RUNNING:
      প্রত্যাগম "running";
    পক্ষ LIBCRUN_CONTAINER_STATUS_PAUSED:
      প্রত্যাগম "paused";
    পক্ষ LIBCRUN_CONTAINER_STATUS_STOPPED:
      প্রত্যাগম "stopped";
    }
  প্রত্যাগম "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  প্রত্যাগম st->status.pid;
}

ধ্রুব বর্ণ *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  প্রত্যাগম st->status.bundle;
}

ধ্রুব বর্ণ *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  প্রত্যাগম st->status.rootfs;
}

ধ্রুব বর্ণ *
libcrun_status_get_created (libcrun_status_t *st)
{
  প্রত্যাগম st->status.created;
}

ধ্রুব বর্ণ *
libcrun_status_get_owner (libcrun_status_t *st)
{
  প্রত্যাগম st->status.owner;
}

ধ্রুব বর্ণ *
libcrun_status_get_scope (libcrun_status_t *st)
{
  প্রত্যাগম st->status.scope;
}

ধ্রুব বর্ণ *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  প্রত্যাগম st->status.external_descriptors;
}

পূর্ণাঙ্ক
libcrun_container_get_state_string (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ **out, পূর্ণাঙ্ক *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  পূর্ণাঙ্ক local_running;
  পূর্ণাঙ্ক ret;

  ret = libcrun_read_container_status (&status, context->state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_মূল, out, &local_running, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (running)
    *running = local_running;
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_get_state_dir (বর্ণ **out, ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  প্রত্যাগম libcrun_get_state_directory (out, state_মূল, id, err);
}

পূর্ণাঙ্ক
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  প্রত্যাগম libcrun_get_containers_list (out, context->state_মূল, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (পরিমাণ (*it));
  it->cur = l;
  প্রত্যাগম it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, ধ্রুব বর্ণ **id)
{
  যদি (it == রিক্ত || it->cur == রিক্ত)
    প্রত্যাগম false;

  যদি (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  প্রত্যাগম true;
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
