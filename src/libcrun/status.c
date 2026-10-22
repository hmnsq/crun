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

#अन्तर्भाव <config.h>
#अन्तर्भाव "status.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "json_gen_utils.h"
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <json-c/json.h>
#अन्तर्भाव <ocispec/json_common.h>
#अन्तर्भाव <sys/mount.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <dirent.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <limits.h>

#define STEAL_POINTER(x, y) \
  कुरु                        \
    {                       \
      *x = y;               \
      y = रिक्त;             \
  } यावत् (0)

संरचना pid_stat
{
  वर्ण state;
  long अंक_threads;
  unsigned long long starttime;
};

स्थिर अन्तःस्थ bool
is_valid_id_char (वर्ण c)
{
  यदि ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    प्रत्यागम true;
  प्रत्यागम c == '_' || c == '+' || c == '.' || c == '-';
}

स्थिर पूर्णाङ्क
validate_id (ध्रुव वर्ण *id, libcrun_error_t *err)
{
  यदि (id == रिक्त)
    प्रत्यागम 0;

  यदि (id[0] == '\0')
    प्रत्यागम crun_make_error (err, 0, "empty container ID");

  यदि (id[0] == '.')
    प्रत्यागम crun_make_error (err, 0, "invalid container ID `%s`", id);

  प्रति (size_t i = 0; id[i]; i++)
    {
      यदि (! is_valid_id_char (id[i]))
        प्रत्यागम crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
get_run_directory (वर्ण **out, ध्रुव वर्ण *state_मूल, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *मूल = रिक्त;

  यदि (state_मूल)
    मूल = xstrdup (state_मूल);
  यदि (मूल == रिक्त)
    {
      ध्रुव वर्ण *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      यदि (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&मूल, err, runtime_dir, "crun", रिक्त);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }
  यदि (मूल == रिक्त)
    मूल = xstrdup ("/run/crun");

  ret = crun_ensure_directory (मूल, 0700, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  STEAL_POINTER (out, मूल);

  प्रत्यागम 0;
}

पूर्णाङ्क
get_shared_empty_directory_path (वर्ण **out, ध्रुव वर्ण *state_मूल, libcrun_error_t *err)
{
  cleanup_free वर्ण *run_dir = रिक्त;
  पूर्णाङ्क ret;

  ret = get_run_directory (&run_dir, state_मूल, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_get_state_directory (वर्ण **out, ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *path = रिक्त;
  cleanup_free वर्ण *मूल = रिक्त;

  ret = validate_id (id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = get_run_directory (&मूल, state_मूल, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (&path, err, मूल, id, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  STEAL_POINTER (out, path);

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
get_state_directory_status_file (वर्ण **out, ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *dir = रिक्त;
  पूर्णाङ्क ret;

  ret = libcrun_get_state_directory (&dir, state_मूल, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम append_paths (out, err, dir, "status", रिक्त);
}

स्थिर पूर्णाङ्क
read_pid_stat (pid_t pid, संरचना pid_stat *st, libcrun_error_t *err)
{
  cleanup_free वर्ण *buffer = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  वर्ण pid_stat_file[64];
  वर्ण *it, *s;
  पूर्णाङ्क i, ret;

  ret = snprintf (pid_stat_file, परिमाण (pid_stat_file), "/proc/%d/stat", pid);
  यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (pid_stat_file)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  यदि (fd < 0)
    {
      /* The process already exited.  */
      यदि (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, परिमाण (*st));
          प्रत्यागम 0;
        }
      प्रत्यागम crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, रिक्त, err);
  यदि (ret < 0)
    {
      st->starttime = 0;
      st->अंक_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      प्रत्यागम 0;
    }

  s = रिक्त;

  /* Skip the first two arguments.  */
  प्रति (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  यदि (s)
    यावत् (*s == ' ')
      s++;

  यदि (s == रिक्त || *s == '\0')
    प्रत्यागम crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  प्रति (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      यदि (it == रिक्त)
        प्रत्यागम crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      यदि (i == 16)
        st->अंक_threads = strtol (it, रिक्त, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, रिक्त, 10);
  यदि (errno != 0)
    प्रत्यागम crun_make_error (err, errno, "parse process start time");

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_write_container_status (ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  पूर्णाङ्क r, ret;
  cleanup_free वर्ण *file_tmp = रिक्त;
  cleanup_free वर्ण *file = रिक्त;
  size_t len;
  cleanup_close पूर्णाङ्क fd_write = -1;
  ध्रुव वर्ण *buf = रिक्त;
  संरचना pid_stat st;
  ध्रुव वर्ण *tmp;
  cleanup_json_gen json_gen_ctx *gen = रिक्त;

  ret = get_state_directory_status_file (&file, state_मूल, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_pid_stat (status->pid, &st, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  यदि (UNLIKELY (fd_write < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot open status file");

  यदि (! json_gen_init (&gen, रिक्त))
    प्रत्यागम crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_कुंजी (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_कुंजी (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_कुंजी (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_कुंजी (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_कुंजी (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_कुंजी (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_कुंजी (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_कुंजी (gen, "created");
  GEN_STR (gen, status->created);

  यदि (status->owner)
    {
      GEN_कुंजी (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_कुंजी (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_कुंजी (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  यदि (status->external_descriptors)
    {
      GEN_कुंजी (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  close_and_reset (&fd_write);

  यदि (UNLIKELY (rename (file_tmp, file) < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot rename status file");

  प्रत्यागम 0;

gen_error:
  प्रत्यागम json_gen_error_to_crun_error (r, err);
}

पूर्णाङ्क
libcrun_read_container_status (libcrun_container_status_t *status, ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id,
                               libcrun_error_t *err)
{
  cleanup_free वर्ण *buffer = रिक्त;
  पूर्णाङ्क ret;
  cleanup_free वर्ण *file = रिक्त;
  json_object *doc = रिक्त;
  json_object *tmp;
  ध्रुव वर्ण *val;

  ret = get_state_directory_status_file (&file, state_मूल, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file (file, &buffer, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    {

      यदि (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free वर्ण *statedir = रिक्त;
          libcrun_error_t tmp_err;
          पूर्णाङ्क tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_मूल, id, &tmp_err);
          यदि (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          अन्यथा
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              यदि (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              अन्यथा यदि (tmp_ret == 0)
                प्रत्यागम crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      प्रत्यागम ret;
    }

  ret = parse_json_file (&doc, buffer, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  {
    tmp = json_object_object_get (doc, "pid");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        गच्छ exit;
      }
    यदि (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        गच्छ exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    यदि (UNLIKELY (tmp == रिक्त))
      status->process_start_time = 0; /* backwards compatibility */
    अन्यथा यदि (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        गच्छ exit;
      }
    अन्यथा
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        गच्छ exit;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        गच्छ exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : रिक्त;
    status->scope = val ? xstrdup (val) : रिक्त;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        गच्छ exit;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        गच्छ exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != रिक्त && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        गच्छ exit;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        गच्छ exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        गच्छ exit;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        गच्छ exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : रिक्त;
    status->owner = val ? xstrdup (val) : रिक्त;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != रिक्त && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != रिक्त && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : रिक्त;
    status->external_descriptors = val ? xstrdup (val) : रिक्त;
  }
  ret = 0;

exit:
  json_object_put (doc);
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_status_check_directories (ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *dir = रिक्त;
  पूर्णाङ्क ret;

  ret = libcrun_get_state_directory (&dir, state_मूल, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = crun_path_exists (dir, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (ret)
    प्रत्यागम crun_make_error (err, 0, "container `%s` already exists", id);

  यदि (UNLIKELY (mkdir (dir, 0700) < 0))
    प्रत्यागम crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  प्रत्यागम 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

स्थिर पूर्णाङ्क
rmdirfd_move_to (DIR **d, ध्रुव वर्ण *name, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  यदि (UNLIKELY (new_d == रिक्त))
    प्रत्यागम crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
rmdirfd (ध्रुव वर्ण *namedir, पूर्णाङ्क fd, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_dir DIR *d = रिक्त;
  संरचना dirent *de;
  संरचना stat मूल_st;
  bool is_मूल = true;
  unsigned पूर्णाङ्क budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close पूर्णाङ्क fd_cleanup = fd;

  ret = fstat (fd, &मूल_st);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  यदि (d == रिक्त)
    प्रत्यागम crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  प्रति (;;)
    {
      errno = 0;
      de = readdir (d);
      यदि (de == रिक्त)
        {
          संरचना stat st;

          यदि (UNLIKELY (errno != 0))
            प्रत्यागम crun_make_error (err, errno, "cannot read directory");

          यदि (is_मूल)
            भङ्ग;

          यदि (UNLIKELY (--budget == 0))
            प्रत्यागम crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          ret = fstat (dirfd (d), &st);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "stat `..`");

          is_मूल = st.st_dev == मूल_st.st_dev && st.st_ino == मूल_st.st_ino;
          अनुवर्त;
        }

      यदि ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        अनुवर्त;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      यदि (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          यदि (ret < 0 && errno == EBUSY)
            {
              cleanup_close पूर्णाङ्क tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              यदि (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  यदि (umount2 (procpath, MNT_DETACH) == 0)
                    गच्छ retry_unlink;
                }
              यदि (! is_मूल)
                प्रत्यागम crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          अन्यथा यदि (ret < 0 && errno == ENOTEMPTY)
            {
              यदि (UNLIKELY (--budget == 0))
                प्रत्यागम crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम ret;

              is_मूल = false;
            }
          अन्यथा यदि (ret < 0 && errno != ENOENT && ! is_मूल)
            {
              प्रत्यागम crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_container_delete_status (ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क rundir_dfd = -1;
  cleanup_close पूर्णाङ्क dfd = -1;
  cleanup_free वर्ण *dir = रिक्त;

  ret = validate_id (id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = get_run_directory (&dir, state_मूल, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  यदि (UNLIKELY (rundir_dfd < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  यदि (UNLIKELY (dfd < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  प्रत्यागम 0;
}

शून्य
libcrun_free_container_status (libcrun_container_status_t *status)
{
  यदि (status == रिक्त)
    प्रत्यागम;
  मुक्त (status->cgroup_path);
  मुक्त (status->bundle);
  मुक्त (status->rootfs);
  मुक्त (status->external_descriptors);
  मुक्त (status->created);
  मुक्त (status->scope);
  मुक्त (status->owner);
}

पूर्णाङ्क
libcrun_get_containers_list (libcrun_container_list_t **out, ध्रुव वर्ण *state_मूल, libcrun_error_t *err)
{
  संरचना dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = रिक्त;
  cleanup_free वर्ण *मूल = रिक्त;
  cleanup_dir DIR *dir = रिक्त;
  पूर्णाङ्क ret;

  *out = रिक्त;

  ret = get_run_directory (&मूल, state_मूल, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dir = opendir (मूल);
  यदि (UNLIKELY (dir == रिक्त))
    प्रत्यागम crun_make_error (err, errno, "cannot opendir `%s`", मूल);

  प्रति (next = readdir (dir); next; next = readdir (dir))
    {
      पूर्णाङ्क exists;
      cleanup_free वर्ण *status_file = रिक्त;

      libcrun_container_list_t *next_container;

      यदि (next->d_name[0] == '.')
        अनुवर्त;

      ret = append_paths (&status_file, err, मूल, next->d_name, "status", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      exists = crun_path_exists (status_file, err);
      यदि (exists < 0)
        {
          प्रत्यागम exists;
        }

      यदि (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          अनुवर्त;
        }

      next_container = xmalloc (परिमाण (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  प्रत्यागम 0;
}

शून्य
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  यावत् (list)
    {
      next = list->next;
      मुक्त (list->name);
      मुक्त (list);
      list = next;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
पूर्णाङ्क
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  संरचना pid_stat st;
  पूर्णाङ्क ret;

  /* For backwards compatibility, check start time only if available. */
  यदि (! status->process_start_time)
    प्रत्यागम 1;

  ret = read_pid_stat (status->pid, &st, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (status->process_start_time != st.starttime || st.state == 'X')
    प्रत्यागम 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  यदि (st.state == 'Z' && st.अंक_threads <= 1)
    प्रत्यागम 0; /* stopped */

  प्रत्यागम 1; /* running, created, or paused */
}

पूर्णाङ्क
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = kill (status->pid, 0);
  यदि (UNLIKELY (ret < 0) && errno != ESRCH)
    प्रत्यागम crun_make_error (err, errno, "kill");

  यदि (ret == 0)
    प्रत्यागम libcrun_check_pid_valid (status, err);

  प्रत्यागम 0; /* stopped */
}

स्थिर पूर्णाङ्क
get_exec_fifo_path (वर्ण **out, ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *state_dir = रिक्त;
  पूर्णाङ्क ret;

  ret = libcrun_get_state_directory (&state_dir, state_मूल, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम append_paths (out, err, state_dir, "exec.fifo", रिक्त);
}

पूर्णाङ्क
libcrun_status_create_exec_fifo (ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *fifo_path = रिक्त;
  पूर्णाङ्क ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_मूल, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  प्रत्यागम fd;
}

पूर्णाङ्क
libcrun_status_write_exec_fifo (ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *fifo_path = रिक्त;
  वर्ण buffer[1] = {
    0,
  };
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;

  ret = get_exec_fifo_path (&fifo_path, state_मूल, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (लिख (fd, buffer, 1));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "write to exec.fifo");

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_status_has_read_exec_fifo (ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *fifo_path = रिक्त;
  पूर्णाङ्क ret;

  ret = get_exec_fifo_path (&fifo_path, state_मूल, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम crun_path_exists (fifo_path, err);
}

संरचना libcrun_status_s
{
  libcrun_container_status_t status;
  वर्ण *id;
  वर्ण *state_मूल;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  पूर्णाङ्क running;
};

पूर्णाङ्क
libcrun_container_status_load (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  पूर्णाङ्क ret;

  st = xmalloc0 (परिमाण (*st));

  ret = libcrun_read_container_status (&st->status, context->state_मूल, id, err);
  यदि (UNLIKELY (ret < 0))
    {
      मुक्त (st);
      प्रत्यागम ret;
    }

  st->id = xstrdup (id);
  st->state_मूल = xstrdup (context->state_मूल);

  *out = st;
  प्रत्यागम 0;
}

शून्य
libcrun_container_status_free (libcrun_status_t *st)
{
  यदि (st == रिक्त)
    प्रत्यागम;
  libcrun_free_container_status (&st->status);
  मुक्त (st->state_मूल);
  मुक्त (st->id);
  मुक्त (st);
}

पूर्णाङ्क
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, पूर्णाङ्क *running, libcrun_error_t *err)
{
  यदि (! st->state_resolved)
    {
      ध्रुव वर्ण *state_string = रिक्त;
      पूर्णाङ्क ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_मूल, &state_string, &st->running, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      यदि (state_string == रिक्त)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      अन्यथा यदि (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      अन्यथा यदि (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      अन्यथा यदि (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      अन्यथा यदि (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      अन्यथा
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  यदि (state)
    *state = st->state;
  यदि (running)
    *running = st->running;
  प्रत्यागम 0;
}

ध्रुव वर्ण *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  विकल्प (state)
    {
    पक्ष LIBCRUN_CONTAINER_STATUS_CREATING:
      प्रत्यागम "creating";
    पक्ष LIBCRUN_CONTAINER_STATUS_CREATED:
      प्रत्यागम "created";
    पक्ष LIBCRUN_CONTAINER_STATUS_RUNNING:
      प्रत्यागम "running";
    पक्ष LIBCRUN_CONTAINER_STATUS_PAUSED:
      प्रत्यागम "paused";
    पक्ष LIBCRUN_CONTAINER_STATUS_STOPPED:
      प्रत्यागम "stopped";
    }
  प्रत्यागम "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  प्रत्यागम st->status.pid;
}

ध्रुव वर्ण *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  प्रत्यागम st->status.bundle;
}

ध्रुव वर्ण *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  प्रत्यागम st->status.rootfs;
}

ध्रुव वर्ण *
libcrun_status_get_created (libcrun_status_t *st)
{
  प्रत्यागम st->status.created;
}

ध्रुव वर्ण *
libcrun_status_get_owner (libcrun_status_t *st)
{
  प्रत्यागम st->status.owner;
}

ध्रुव वर्ण *
libcrun_status_get_scope (libcrun_status_t *st)
{
  प्रत्यागम st->status.scope;
}

ध्रुव वर्ण *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  प्रत्यागम st->status.external_descriptors;
}

पूर्णाङ्क
libcrun_container_get_state_string (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण **out, पूर्णाङ्क *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  पूर्णाङ्क local_running;
  पूर्णाङ्क ret;

  ret = libcrun_read_container_status (&status, context->state_मूल, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_मूल, out, &local_running, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (running)
    *running = local_running;
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_get_state_dir (वर्ण **out, ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  प्रत्यागम libcrun_get_state_directory (out, state_मूल, id, err);
}

पूर्णाङ्क
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  प्रत्यागम libcrun_get_containers_list (out, context->state_मूल, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (परिमाण (*it));
  it->cur = l;
  प्रत्यागम it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, ध्रुव वर्ण **id)
{
  यदि (it == रिक्त || it->cur == रिक्त)
    प्रत्यागम false;

  यदि (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  प्रत्यागम true;
}

शून्य
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  मुक्त (it);
}

शून्य
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
