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
  करा                        \
    {                       \
      *x = y;               \
      y = रिक्त;             \
  } पर्यंत (0)

संरचना pid_stat
{
  अक्षर state;
  दीर्घ अंक_threads;
  अचिन्हित दीर्घ दीर्घ starttime;
};

स्थिर ओळीत bool
is_valid_id_char (अक्षर c)
{
  जर ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    परत true;
  परत c == '_' || c == '+' || c == '.' || c == '-';
}

स्थिर पूर्णांक
validate_id (स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  जर (id == रिक्त)
    परत 0;

  जर (id[0] == '\0')
    परत crun_make_error (err, 0, "empty container ID");

  जर (id[0] == '.')
    परत crun_make_error (err, 0, "invalid container ID `%s`", id);

  साठी (माप_प्रकार i = 0; id[i]; i++)
    {
      जर (! is_valid_id_char (id[i]))
        परत crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  परत 0;
}

पूर्णांक
get_run_directory (अक्षर **out, स्थिरांक अक्षर *state_मूळ, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *मूळ = रिक्त;

  जर (state_मूळ)
    मूळ = xstrdup (state_मूळ);
  जर (मूळ == रिक्त)
    {
      स्थिरांक अक्षर *runtime_dir = दो_पर्या ("XDG_RUNTIME_DIR");
      जर (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&मूळ, err, runtime_dir, "crun", रिक्त);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }
    }
  जर (मूळ == रिक्त)
    मूळ = xstrdup ("/run/crun");

  ret = crun_ensure_directory (मूळ, 0700, false, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  STEAL_POINTER (out, मूळ);

  परत 0;
}

पूर्णांक
get_shared_empty_directory_path (अक्षर **out, स्थिरांक अक्षर *state_मूळ, libcrun_error_t *err)
{
  cleanup_free अक्षर *run_dir = रिक्त;
  पूर्णांक ret;

  ret = get_run_directory (&run_dir, state_मूळ, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत 0;
}

पूर्णांक
libcrun_get_state_directory (अक्षर **out, स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *path = रिक्त;
  cleanup_free अक्षर *मूळ = रिक्त;

  ret = validate_id (id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = get_run_directory (&मूळ, state_मूळ, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = append_paths (&path, err, मूळ, id, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  STEAL_POINTER (out, path);

  परत 0;
}

स्थिर पूर्णांक
get_state_directory_status_file (अक्षर **out, स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *dir = रिक्त;
  पूर्णांक ret;

  ret = libcrun_get_state_directory (&dir, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत append_paths (out, err, dir, "status", रिक्त);
}

स्थिर पूर्णांक
read_pid_stat (pid_t pid, संरचना pid_stat *st, libcrun_error_t *err)
{
  cleanup_free अक्षर *buffer = रिक्त;
  cleanup_close पूर्णांक fd = -1;
  अक्षर pid_stat_file[64];
  अक्षर *it, *s;
  पूर्णांक i, ret;

  ret = snprintf (pid_stat_file, आकार (pid_stat_file), "/proc/%d/stat", pid);
  जर (UNLIKELY (ret >= (पूर्णांक) आकार (pid_stat_file)))
    परत crun_make_error (err, 0, "internal error: static buffer too small");

  fd = खोलो (pid_stat_file, O_RDONLY | O_CLOEXEC);
  जर (fd < 0)
    {
      /* The process already exited.  */
      जर (त्रुटि_सं == ENOENT || त्रुटि_सं == ESRCH)
        {
          स_रखो (st, 0, आकार (*st));
          परत 0;
        }
      परत crun_make_error (err, त्रुटि_सं, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, रिक्त, err);
  जर (ret < 0)
    {
      st->starttime = 0;
      st->अंक_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      परत 0;
    }

  s = रिक्त;

  /* Skip the first two arguments.  */
  साठी (it = buffer; it; it = म_अक्षर (it + 1, ')'))
    s = it + 1;

  जर (s)
    पर्यंत (*s == ' ')
      s++;

  जर (s == रिक्त || *s == '\0')
    परत crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  साठी (it = s + 1, i = 0; i < 19; i++)
    {
      it = म_अक्षर (it, ' ');
      जर (it == रिक्त)
        परत crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      जर (i == 16)
        st->अंक_threads = म_से_दीर्घ (it, रिक्त, 10);
    }

  त्रुटि_सं = 0;
  st->starttime = strtoull (it, रिक्त, 10);
  जर (त्रुटि_सं != 0)
    परत crun_make_error (err, त्रुटि_सं, "parse process start time");

  परत 0;
}

पूर्णांक
libcrun_write_container_status (स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  पूर्णांक r, ret;
  cleanup_free अक्षर *file_tmp = रिक्त;
  cleanup_free अक्षर *file = रिक्त;
  माप_प्रकार len;
  cleanup_close पूर्णांक fd_write = -1;
  स्थिरांक अक्षर *buf = रिक्त;
  संरचना pid_stat st;
  स्थिरांक अक्षर *tmp;
  cleanup_json_gen json_gen_ctx *gen = रिक्त;

  ret = get_state_directory_status_file (&file, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = read_pid_stat (status->pid, &st, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = खोलो (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  जर (UNLIKELY (fd_write < 0))
    परत crun_make_error (err, त्रुटि_सं, "cannot open status file");

  जर (! json_gen_init (&gen, रिक्त))
    परत crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_किल्ली (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_किल्ली (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_किल्ली (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_किल्ली (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_किल्ली (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_किल्ली (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_किल्ली (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_किल्ली (gen, "created");
  GEN_STR (gen, status->created);

  जर (status->owner)
    {
      GEN_किल्ली (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_किल्ली (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_किल्ली (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  जर (status->external_descriptors)
    {
      GEN_किल्ली (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  close_and_reset (&fd_write);

  जर (UNLIKELY (नाम (file_tmp, file) < 0))
    परत crun_make_error (err, त्रुटि_सं, "cannot rename status file");

  परत 0;

gen_error:
  परत json_gen_error_to_crun_error (r, err);
}

पूर्णांक
libcrun_read_container_status (libcrun_container_status_t *status, स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id,
                               libcrun_error_t *err)
{
  cleanup_free अक्षर *buffer = रिक्त;
  पूर्णांक ret;
  cleanup_free अक्षर *file = रिक्त;
  json_object *doc = रिक्त;
  json_object *tmp;
  स्थिरांक अक्षर *val;

  ret = get_state_directory_status_file (&file, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = read_all_file (file, &buffer, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    {

      जर (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free अक्षर *statedir = रिक्त;
          libcrun_error_t tmp_err;
          पूर्णांक tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_मूळ, id, &tmp_err);
          जर (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          अन्यथा
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              जर (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              अन्यथा जर (tmp_ret == 0)
                परत crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      परत ret;
    }

  ret = parse_json_file (&doc, buffer, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  {
    tmp = json_object_object_get (doc, "pid");
    जर (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        जा निकास;
      }
    जर (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        जा निकास;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    जर (UNLIKELY (tmp == रिक्त))
      status->process_start_time = 0; /* backwards compatibility */
    अन्यथा जर (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        जा निकास;
      }
    अन्यथा
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    जर (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        जा निकास;
      }
    val = json_object_get_string (tmp);
    जर (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        जा निकास;
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
    जर (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        जा निकास;
      }
    val = json_object_get_string (tmp);
    जर (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        जा निकास;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != रिक्त && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    जर (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        जा निकास;
      }
    val = json_object_get_string (tmp);
    जर (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        जा निकास;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    जर (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        जा निकास;
      }
    val = json_object_get_string (tmp);
    जर (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        जा निकास;
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

निकास:
  json_object_put (doc);
  परत ret;
}

पूर्णांक
libcrun_status_check_directories (स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *dir = रिक्त;
  पूर्णांक ret;

  ret = libcrun_get_state_directory (&dir, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = crun_path_exists (dir, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (ret)
    परत crun_make_error (err, 0, "container `%s` already exists", id);

  जर (UNLIKELY (सूची_गढ़ो (dir, 0700) < 0))
    परत crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  परत 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

स्थिर पूर्णांक
rmdirfd_move_to (सूची **d, स्थिरांक अक्षर *name, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  सूची *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  जर (UNLIKELY (fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  जर (UNLIKELY (new_d == रिक्त))
    परत crun_make_error (err, त्रुटि_सं, "cannot open directory `%s`", name);

  fd = -1;

  बंद_सूची (*d);
  *d = new_d;
  परत 0;
}

स्थिर पूर्णांक
rmdirfd (स्थिरांक अक्षर *namedir, पूर्णांक fd, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_dir सूची *d = रिक्त;
  संरचना dirent *de;
  संरचना stat मूळ_st;
  bool is_मूळ = true;
  अचिन्हित पूर्णांक budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close पूर्णांक fd_cleanup = fd;

  ret = ख_स्थिति (fd, &मूळ_st);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "stat `%s`", namedir);

  d = fdopendir (fd);
  जर (d == रिक्त)
    परत crun_make_error (err, त्रुटि_सं, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  साठी (;;)
    {
      त्रुटि_सं = 0;
      de = सूची_पढ़ो (d);
      जर (de == रिक्त)
        {
          संरचना stat st;

          जर (UNLIKELY (त्रुटि_सं != 0))
            परत crun_make_error (err, त्रुटि_सं, "cannot read directory");

          जर (is_मूळ)
            थांबा;

          जर (UNLIKELY (--budget == 0))
            परत crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          जर (UNLIKELY (ret < 0))
            परत ret;

          ret = ख_स्थिति (dirfd (d), &st);
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "stat `..`");

          is_मूळ = st.st_dev == मूळ_st.st_dev && st.st_ino == मूळ_st.st_ino;
          सुरूठेवा;
        }

      जर ((म_भेद (de->d_name, ".") == 0) || (म_भेद (de->d_name, "..") == 0))
        सुरूठेवा;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      जर (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          जर (ret < 0 && त्रुटि_सं == EBUSY)
            {
              cleanup_close पूर्णांक tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              जर (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  जर (umount2 (procpath, MNT_DETACH) == 0)
                    जा retry_unlink;
                }
              जर (! is_मूळ)
                परत crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          अन्यथा जर (ret < 0 && त्रुटि_सं == ENOTEMPTY)
            {
              जर (UNLIKELY (--budget == 0))
                परत crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              जर (UNLIKELY (ret < 0))
                परत ret;

              is_मूळ = false;
            }
          अन्यथा जर (ret < 0 && त्रुटि_सं != ENOENT && ! is_मूळ)
            {
              परत crun_make_error (err, त्रुटि_सं, "cannot rm `%s`", de->d_name);
            }
        }
    }

  परत 0;
}

पूर्णांक
libcrun_container_delete_status (स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_close पूर्णांक rundir_dfd = -1;
  cleanup_close पूर्णांक dfd = -1;
  cleanup_free अक्षर *dir = रिक्त;

  ret = validate_id (id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = get_run_directory (&dir, state_मूळ, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  rundir_dfd = TEMP_FAILURE_RETRY (खोलो (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  जर (UNLIKELY (rundir_dfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  जर (UNLIKELY (dfd < 0))
    परत crun_make_error (err, त्रुटि_सं, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "cannot rm state directory `%s/%s`", dir, id);

  परत 0;
}

शून्य
libcrun_free_container_status (libcrun_container_status_t *status)
{
  जर (status == रिक्त)
    परत;
  मुक्त (status->cgroup_path);
  मुक्त (status->bundle);
  मुक्त (status->rootfs);
  मुक्त (status->external_descriptors);
  मुक्त (status->created);
  मुक्त (status->scope);
  मुक्त (status->owner);
}

पूर्णांक
libcrun_get_containers_list (libcrun_container_list_t **out, स्थिरांक अक्षर *state_मूळ, libcrun_error_t *err)
{
  संरचना dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = रिक्त;
  cleanup_free अक्षर *मूळ = रिक्त;
  cleanup_dir सूची *dir = रिक्त;
  पूर्णांक ret;

  *out = रिक्त;

  ret = get_run_directory (&मूळ, state_मूळ, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  dir = सूची_खोलो (मूळ);
  जर (UNLIKELY (dir == रिक्त))
    परत crun_make_error (err, त्रुटि_सं, "cannot opendir `%s`", मूळ);

  साठी (next = सूची_पढ़ो (dir); next; next = सूची_पढ़ो (dir))
    {
      पूर्णांक exists;
      cleanup_free अक्षर *status_file = रिक्त;

      libcrun_container_list_t *next_container;

      जर (next->d_name[0] == '.')
        सुरूठेवा;

      ret = append_paths (&status_file, err, मूळ, next->d_name, "status", रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;

      exists = crun_path_exists (status_file, err);
      जर (exists < 0)
        {
          परत exists;
        }

      जर (! exists)
        {
          libcrun_error (त्रुटि_सं, "error opening file `%s`", status_file);
          सुरूठेवा;
        }

      next_container = xmalloc (आकार (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  परत 0;
}

शून्य
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  पर्यंत (list)
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
पूर्णांक
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  संरचना pid_stat st;
  पूर्णांक ret;

  /* For backwards compatibility, check start time only if available. */
  जर (! status->process_start_time)
    परत 1;

  ret = read_pid_stat (status->pid, &st, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (status->process_start_time != st.starttime || st.state == 'X')
    परत 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  जर (st.state == 'Z' && st.अंक_threads <= 1)
    परत 0; /* stopped */

  परत 1; /* running, created, or paused */
}

पूर्णांक
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = समाप्त (status->pid, 0);
  जर (UNLIKELY (ret < 0) && त्रुटि_सं != ESRCH)
    परत crun_make_error (err, त्रुटि_सं, "kill");

  जर (ret == 0)
    परत libcrun_check_pid_valid (status, err);

  परत 0; /* stopped */
}

स्थिर पूर्णांक
get_exec_fifo_path (अक्षर **out, स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *state_dir = रिक्त;
  पूर्णांक ret;

  ret = libcrun_get_state_directory (&state_dir, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत append_paths (out, err, state_dir, "exec.fifo", रिक्त);
}

पूर्णांक
libcrun_status_create_exec_fifo (स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *fifo_path = रिक्त;
  पूर्णांक ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "mkfifo");

  fd = खोलो (fifo_path, O_NONBLOCK | O_CLOEXEC);
  जर (UNLIKELY (fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "cannot open pipe `%s`", fifo_path);

  परत fd;
}

पूर्णांक
libcrun_status_write_exec_fifo (स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *fifo_path = रिक्त;
  अक्षर buffer[1] = {
    0,
  };
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret;

  ret = get_exec_fifo_path (&fifo_path, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  fd = खोलो (fifo_path, O_WRONLY | O_CLOEXEC);
  जर (UNLIKELY (fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (लिख (fd, buffer, 1));
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "write to exec.fifo");

  परत 0;
}

पूर्णांक
libcrun_status_has_read_exec_fifo (स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *fifo_path = रिक्त;
  पूर्णांक ret;

  ret = get_exec_fifo_path (&fifo_path, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत crun_path_exists (fifo_path, err);
}

संरचना libcrun_status_s
{
  libcrun_container_status_t status;
  अक्षर *id;
  अक्षर *state_मूळ;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  पूर्णांक running;
};

पूर्णांक
libcrun_container_status_load (libcrun_context_t *context, स्थिरांक अक्षर *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  पूर्णांक ret;

  st = xmalloc0 (आकार (*st));

  ret = libcrun_read_container_status (&st->status, context->state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    {
      मुक्त (st);
      परत ret;
    }

  st->id = xstrdup (id);
  st->state_मूळ = xstrdup (context->state_मूळ);

  *out = st;
  परत 0;
}

शून्य
libcrun_container_status_free (libcrun_status_t *st)
{
  जर (st == रिक्त)
    परत;
  libcrun_free_container_status (&st->status);
  मुक्त (st->state_मूळ);
  मुक्त (st->id);
  मुक्त (st);
}

पूर्णांक
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, पूर्णांक *running, libcrun_error_t *err)
{
  जर (! st->state_resolved)
    {
      स्थिरांक अक्षर *state_string = रिक्त;
      पूर्णांक ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_मूळ, &state_string, &st->running, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      जर (state_string == रिक्त)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      अन्यथा जर (म_भेद (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      अन्यथा जर (म_भेद (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      अन्यथा जर (म_भेद (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      अन्यथा जर (म_भेद (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      अन्यथा
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  जर (state)
    *state = st->state;
  जर (running)
    *running = st->running;
  परत 0;
}

स्थिरांक अक्षर *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  निवड (state)
    {
    स्थिती LIBCRUN_CONTAINER_STATUS_CREATING:
      परत "creating";
    स्थिती LIBCRUN_CONTAINER_STATUS_CREATED:
      परत "created";
    स्थिती LIBCRUN_CONTAINER_STATUS_RUNNING:
      परत "running";
    स्थिती LIBCRUN_CONTAINER_STATUS_PAUSED:
      परत "paused";
    स्थिती LIBCRUN_CONTAINER_STATUS_STOPPED:
      परत "stopped";
    }
  परत "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  परत st->status.pid;
}

स्थिरांक अक्षर *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  परत st->status.bundle;
}

स्थिरांक अक्षर *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  परत st->status.rootfs;
}

स्थिरांक अक्षर *
libcrun_status_get_created (libcrun_status_t *st)
{
  परत st->status.created;
}

स्थिरांक अक्षर *
libcrun_status_get_owner (libcrun_status_t *st)
{
  परत st->status.owner;
}

स्थिरांक अक्षर *
libcrun_status_get_scope (libcrun_status_t *st)
{
  परत st->status.scope;
}

स्थिरांक अक्षर *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  परत st->status.external_descriptors;
}

पूर्णांक
libcrun_container_get_state_string (libcrun_context_t *context, स्थिरांक अक्षर *id, स्थिरांक अक्षर **out, पूर्णांक *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  पूर्णांक local_running;
  पूर्णांक ret;

  ret = libcrun_read_container_status (&status, context->state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_मूळ, out, &local_running, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (running)
    *running = local_running;
  परत 0;
}

पूर्णांक
libcrun_get_state_dir (अक्षर **out, स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  परत libcrun_get_state_directory (out, state_मूळ, id, err);
}

पूर्णांक
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  परत libcrun_get_containers_list (out, context->state_मूळ, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (आकार (*it));
  it->cur = l;
  परत it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, स्थिरांक अक्षर **id)
{
  जर (it == रिक्त || it->cur == रिक्त)
    परत false;

  जर (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  परत true;
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
