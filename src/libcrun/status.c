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
  गर                        \
    {                       \
      *x = y;               \
      y = रिक्त;             \
  } जबसम्म (0)

संरचना pid_stat
{
  वर्ण state;
  दीर्घ अङ्क_threads;
  अचिन्हित दीर्घ दीर्घ starttime;
};

स्थिर अन्तःस्थ bool
is_valid_id_char (वर्ण c)
{
  यदि ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    फर्काऊ true;
  फर्काऊ c == '_' || c == '+' || c == '.' || c == '-';
}

स्थिर पूर्णाङ्क
validate_id (अचर वर्ण *id, libcrun_error_t *err)
{
  यदि (id == रिक्त)
    फर्काऊ 0;

  यदि (id[0] == '\0')
    फर्काऊ crun_make_error (err, 0, "empty container ID");

  यदि (id[0] == '.')
    फर्काऊ crun_make_error (err, 0, "invalid container ID `%s`", id);

  लागि (माप_प्रकार i = 0; id[i]; i++)
    {
      यदि (! is_valid_id_char (id[i]))
        फर्काऊ crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  फर्काऊ 0;
}

पूर्णाङ्क
get_run_directory (वर्ण **out, अचर वर्ण *state_जरा, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *जरा = रिक्त;

  यदि (state_जरा)
    जरा = xstrdup (state_जरा);
  यदि (जरा == रिक्त)
    {
      अचर वर्ण *runtime_dir = दो_पर्या ("XDG_RUNTIME_DIR");
      यदि (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&जरा, err, runtime_dir, "crun", रिक्त);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;
        }
    }
  यदि (जरा == रिक्त)
    जरा = xstrdup ("/run/crun");

  ret = crun_ensure_directory (जरा, 0700, false, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  STEAL_POINTER (out, जरा);

  फर्काऊ 0;
}

पूर्णाङ्क
get_shared_empty_directory_path (वर्ण **out, अचर वर्ण *state_जरा, libcrun_error_t *err)
{
  cleanup_free वर्ण *run_dir = रिक्त;
  पूर्णाङ्क ret;

  ret = get_run_directory (&run_dir, state_जरा, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_get_state_directory (वर्ण **out, अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *path = रिक्त;
  cleanup_free वर्ण *जरा = रिक्त;

  ret = validate_id (id, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = get_run_directory (&जरा, state_जरा, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = append_paths (&path, err, जरा, id, रिक्त);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  STEAL_POINTER (out, path);

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
get_state_directory_status_file (वर्ण **out, अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *dir = रिक्त;
  पूर्णाङ्क ret;

  ret = libcrun_get_state_directory (&dir, state_जरा, id, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ append_paths (out, err, dir, "status", रिक्त);
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
    फर्काऊ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = खोलो (pid_stat_file, O_RDONLY | O_CLOEXEC);
  यदि (fd < 0)
    {
      /* The process already exited.  */
      यदि (त्रुटि_सं == ENOENT || त्रुटि_सं == ESRCH)
        {
          स_रखो (st, 0, परिमाण (*st));
          फर्काऊ 0;
        }
      फर्काऊ crun_make_error (err, त्रुटि_सं, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, रिक्त, err);
  यदि (ret < 0)
    {
      st->starttime = 0;
      st->अङ्क_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      फर्काऊ 0;
    }

  s = रिक्त;

  /* Skip the first two arguments.  */
  लागि (it = buffer; it; it = म_अक्षर (it + 1, ')'))
    s = it + 1;

  यदि (s)
    जबसम्म (*s == ' ')
      s++;

  यदि (s == रिक्त || *s == '\0')
    फर्काऊ crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  लागि (it = s + 1, i = 0; i < 19; i++)
    {
      it = म_अक्षर (it, ' ');
      यदि (it == रिक्त)
        फर्काऊ crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      यदि (i == 16)
        st->अङ्क_threads = म_से_दीर्घ (it, रिक्त, 10);
    }

  त्रुटि_सं = 0;
  st->starttime = strtoull (it, रिक्त, 10);
  यदि (त्रुटि_सं != 0)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "parse process start time");

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_write_container_status (अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  पूर्णाङ्क r, ret;
  cleanup_free वर्ण *file_tmp = रिक्त;
  cleanup_free वर्ण *file = रिक्त;
  माप_प्रकार len;
  cleanup_close पूर्णाङ्क fd_write = -1;
  अचर वर्ण *buf = रिक्त;
  संरचना pid_stat st;
  अचर वर्ण *tmp;
  cleanup_json_gen json_gen_ctx *gen = रिक्त;

  ret = get_state_directory_status_file (&file, state_जरा, id, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = read_pid_stat (status->pid, &st, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = खोलो (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  यदि (UNLIKELY (fd_write < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot open status file");

  यदि (! json_gen_init (&gen, रिक्त))
    फर्काऊ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_साँचो (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_साँचो (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_साँचो (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_साँचो (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_साँचो (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_साँचो (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_साँचो (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_साँचो (gen, "created");
  GEN_STR (gen, status->created);

  यदि (status->owner)
    {
      GEN_साँचो (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_साँचो (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_साँचो (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  यदि (status->external_descriptors)
    {
      GEN_साँचो (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  close_and_reset (&fd_write);

  यदि (UNLIKELY (नाम (file_tmp, file) < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot rename status file");

  फर्काऊ 0;

gen_error:
  फर्काऊ json_gen_error_to_crun_error (r, err);
}

पूर्णाङ्क
libcrun_read_container_status (libcrun_container_status_t *status, अचर वर्ण *state_जरा, अचर वर्ण *id,
                               libcrun_error_t *err)
{
  cleanup_free वर्ण *buffer = रिक्त;
  पूर्णाङ्क ret;
  cleanup_free वर्ण *file = रिक्त;
  json_object *doc = रिक्त;
  json_object *tmp;
  अचर वर्ण *val;

  ret = get_state_directory_status_file (&file, state_जरा, id, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = read_all_file (file, &buffer, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    {

      यदि (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free वर्ण *statedir = रिक्त;
          libcrun_error_t tmp_err;
          पूर्णाङ्क tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_जरा, id, &tmp_err);
          यदि (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          अन्यथा
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              यदि (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              अन्यथा यदि (tmp_ret == 0)
                फर्काऊ crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      फर्काऊ ret;
    }

  ret = parse_json_file (&doc, buffer, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  {
    tmp = json_object_object_get (doc, "pid");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        गच्छ निकास;
      }
    यदि (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        गच्छ निकास;
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
        गच्छ निकास;
      }
    अन्यथा
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        गच्छ निकास;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        गच्छ निकास;
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
        गच्छ निकास;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        गच्छ निकास;
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
        गच्छ निकास;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        गच्छ निकास;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        गच्छ निकास;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        गच्छ निकास;
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
  फर्काऊ ret;
}

पूर्णाङ्क
libcrun_status_check_directories (अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *dir = रिक्त;
  पूर्णाङ्क ret;

  ret = libcrun_get_state_directory (&dir, state_जरा, id, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = crun_path_exists (dir, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (ret)
    फर्काऊ crun_make_error (err, 0, "container `%s` already exists", id);

  यदि (UNLIKELY (सूची_गढ़ो (dir, 0700) < 0))
    फर्काऊ crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  फर्काऊ 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

स्थिर पूर्णाङ्क
rmdirfd_move_to (सूची **d, अचर वर्ण *name, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क fd = -1;
  सूची *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  यदि (UNLIKELY (new_d == रिक्त))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot open directory `%s`", name);

  fd = -1;

  बंद_सूची (*d);
  *d = new_d;
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
rmdirfd (अचर वर्ण *namedir, पूर्णाङ्क fd, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_dir सूची *d = रिक्त;
  संरचना dirent *de;
  संरचना stat जरा_st;
  bool is_जरा = true;
  अचिन्हित पूर्णाङ्क budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close पूर्णाङ्क fd_cleanup = fd;

  ret = ख_स्थिति (fd, &जरा_st);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "stat `%s`", namedir);

  d = fdopendir (fd);
  यदि (d == रिक्त)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  लागि (;;)
    {
      त्रुटि_सं = 0;
      de = सूची_पढ़ो (d);
      यदि (de == रिक्त)
        {
          संरचना stat st;

          यदि (UNLIKELY (त्रुटि_सं != 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot read directory");

          यदि (is_जरा)
            रोक;

          यदि (UNLIKELY (--budget == 0))
            फर्काऊ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ ret;

          ret = ख_स्थिति (dirfd (d), &st);
          यदि (UNLIKELY (ret < 0))
            फर्काऊ crun_make_error (err, त्रुटि_सं, "stat `..`");

          is_जरा = st.st_dev == जरा_st.st_dev && st.st_ino == जरा_st.st_ino;
          जारी;
        }

      यदि ((म_भेद (de->d_name, ".") == 0) || (म_भेद (de->d_name, "..") == 0))
        जारी;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      यदि (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          यदि (ret < 0 && त्रुटि_सं == EBUSY)
            {
              cleanup_close पूर्णाङ्क tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              यदि (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  यदि (umount2 (procpath, MNT_DETACH) == 0)
                    गच्छ retry_unlink;
                }
              यदि (! is_जरा)
                फर्काऊ crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          अन्यथा यदि (ret < 0 && त्रुटि_सं == ENOTEMPTY)
            {
              यदि (UNLIKELY (--budget == 0))
                फर्काऊ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              यदि (UNLIKELY (ret < 0))
                फर्काऊ ret;

              is_जरा = false;
            }
          अन्यथा यदि (ret < 0 && त्रुटि_सं != ENOENT && ! is_जरा)
            {
              फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot rm `%s`", de->d_name);
            }
        }
    }

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_container_delete_status (अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क rundir_dfd = -1;
  cleanup_close पूर्णाङ्क dfd = -1;
  cleanup_free वर्ण *dir = रिक्त;

  ret = validate_id (id, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = get_run_directory (&dir, state_जरा, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  rundir_dfd = TEMP_FAILURE_RETRY (खोलो (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  यदि (UNLIKELY (rundir_dfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  यदि (UNLIKELY (dfd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot rm state directory `%s/%s`", dir, id);

  फर्काऊ 0;
}

शून्य
libcrun_free_container_status (libcrun_container_status_t *status)
{
  यदि (status == रिक्त)
    फर्काऊ;
  मुक्त (status->cgroup_path);
  मुक्त (status->bundle);
  मुक्त (status->rootfs);
  मुक्त (status->external_descriptors);
  मुक्त (status->created);
  मुक्त (status->scope);
  मुक्त (status->owner);
}

पूर्णाङ्क
libcrun_get_containers_list (libcrun_container_list_t **out, अचर वर्ण *state_जरा, libcrun_error_t *err)
{
  संरचना dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = रिक्त;
  cleanup_free वर्ण *जरा = रिक्त;
  cleanup_dir सूची *dir = रिक्त;
  पूर्णाङ्क ret;

  *out = रिक्त;

  ret = get_run_directory (&जरा, state_जरा, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  dir = सूची_खोलो (जरा);
  यदि (UNLIKELY (dir == रिक्त))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot opendir `%s`", जरा);

  लागि (next = सूची_पढ़ो (dir); next; next = सूची_पढ़ो (dir))
    {
      पूर्णाङ्क exists;
      cleanup_free वर्ण *status_file = रिक्त;

      libcrun_container_list_t *next_container;

      यदि (next->d_name[0] == '.')
        जारी;

      ret = append_paths (&status_file, err, जरा, next->d_name, "status", रिक्त);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      exists = crun_path_exists (status_file, err);
      यदि (exists < 0)
        {
          फर्काऊ exists;
        }

      यदि (! exists)
        {
          libcrun_error (त्रुटि_सं, "error opening file `%s`", status_file);
          जारी;
        }

      next_container = xmalloc (परिमाण (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  फर्काऊ 0;
}

शून्य
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  जबसम्म (list)
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
    फर्काऊ 1;

  ret = read_pid_stat (status->pid, &st, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (status->process_start_time != st.starttime || st.state == 'X')
    फर्काऊ 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  यदि (st.state == 'Z' && st.अङ्क_threads <= 1)
    फर्काऊ 0; /* stopped */

  फर्काऊ 1; /* running, created, or paused */
}

पूर्णाङ्क
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = समाप्त (status->pid, 0);
  यदि (UNLIKELY (ret < 0) && त्रुटि_सं != ESRCH)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "kill");

  यदि (ret == 0)
    फर्काऊ libcrun_check_pid_valid (status, err);

  फर्काऊ 0; /* stopped */
}

स्थिर पूर्णाङ्क
get_exec_fifo_path (वर्ण **out, अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *state_dir = रिक्त;
  पूर्णाङ्क ret;

  ret = libcrun_get_state_directory (&state_dir, state_जरा, id, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ append_paths (out, err, state_dir, "exec.fifo", रिक्त);
}

पूर्णाङ्क
libcrun_status_create_exec_fifo (अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *fifo_path = रिक्त;
  पूर्णाङ्क ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_जरा, id, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "mkfifo");

  fd = खोलो (fifo_path, O_NONBLOCK | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot open pipe `%s`", fifo_path);

  फर्काऊ fd;
}

पूर्णाङ्क
libcrun_status_write_exec_fifo (अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *fifo_path = रिक्त;
  वर्ण buffer[1] = {
    0,
  };
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;

  ret = get_exec_fifo_path (&fifo_path, state_जरा, id, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  fd = खोलो (fifo_path, O_WRONLY | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (लिख (fd, buffer, 1));
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "write to exec.fifo");

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_status_has_read_exec_fifo (अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *fifo_path = रिक्त;
  पूर्णाङ्क ret;

  ret = get_exec_fifo_path (&fifo_path, state_जरा, id, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  फर्काऊ crun_path_exists (fifo_path, err);
}

संरचना libcrun_status_s
{
  libcrun_container_status_t status;
  वर्ण *id;
  वर्ण *state_जरा;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  पूर्णाङ्क running;
};

पूर्णाङ्क
libcrun_container_status_load (libcrun_context_t *context, अचर वर्ण *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  पूर्णाङ्क ret;

  st = xmalloc0 (परिमाण (*st));

  ret = libcrun_read_container_status (&st->status, context->state_जरा, id, err);
  यदि (UNLIKELY (ret < 0))
    {
      मुक्त (st);
      फर्काऊ ret;
    }

  st->id = xstrdup (id);
  st->state_जरा = xstrdup (context->state_जरा);

  *out = st;
  फर्काऊ 0;
}

शून्य
libcrun_container_status_free (libcrun_status_t *st)
{
  यदि (st == रिक्त)
    फर्काऊ;
  libcrun_free_container_status (&st->status);
  मुक्त (st->state_जरा);
  मुक्त (st->id);
  मुक्त (st);
}

पूर्णाङ्क
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, पूर्णाङ्क *running, libcrun_error_t *err)
{
  यदि (! st->state_resolved)
    {
      अचर वर्ण *state_string = रिक्त;
      पूर्णाङ्क ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_जरा, &state_string, &st->running, err);
      यदि (UNLIKELY (ret < 0))
        फर्काऊ ret;

      यदि (state_string == रिक्त)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      अन्यथा यदि (म_भेद (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      अन्यथा यदि (म_भेद (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      अन्यथा यदि (म_भेद (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      अन्यथा यदि (म_भेद (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      अन्यथा
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  यदि (state)
    *state = st->state;
  यदि (running)
    *running = st->running;
  फर्काऊ 0;
}

अचर वर्ण *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  विकल्प (state)
    {
    पक्ष LIBCRUN_CONTAINER_STATUS_CREATING:
      फर्काऊ "creating";
    पक्ष LIBCRUN_CONTAINER_STATUS_CREATED:
      फर्काऊ "created";
    पक्ष LIBCRUN_CONTAINER_STATUS_RUNNING:
      फर्काऊ "running";
    पक्ष LIBCRUN_CONTAINER_STATUS_PAUSED:
      फर्काऊ "paused";
    पक्ष LIBCRUN_CONTAINER_STATUS_STOPPED:
      फर्काऊ "stopped";
    }
  फर्काऊ "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  फर्काऊ st->status.pid;
}

अचर वर्ण *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  फर्काऊ st->status.bundle;
}

अचर वर्ण *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  फर्काऊ st->status.rootfs;
}

अचर वर्ण *
libcrun_status_get_created (libcrun_status_t *st)
{
  फर्काऊ st->status.created;
}

अचर वर्ण *
libcrun_status_get_owner (libcrun_status_t *st)
{
  फर्काऊ st->status.owner;
}

अचर वर्ण *
libcrun_status_get_scope (libcrun_status_t *st)
{
  फर्काऊ st->status.scope;
}

अचर वर्ण *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  फर्काऊ st->status.external_descriptors;
}

पूर्णाङ्क
libcrun_container_get_state_string (libcrun_context_t *context, अचर वर्ण *id, अचर वर्ण **out, पूर्णाङ्क *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  पूर्णाङ्क local_running;
  पूर्णाङ्क ret;

  ret = libcrun_read_container_status (&status, context->state_जरा, id, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_जरा, out, &local_running, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (running)
    *running = local_running;
  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_get_state_dir (वर्ण **out, अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err)
{
  फर्काऊ libcrun_get_state_directory (out, state_जरा, id, err);
}

पूर्णाङ्क
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  फर्काऊ libcrun_get_containers_list (out, context->state_जरा, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (परिमाण (*it));
  it->cur = l;
  फर्काऊ it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, अचर वर्ण **id)
{
  यदि (it == रिक्त || it->cur == रिक्त)
    फर्काऊ false;

  यदि (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  फर्काऊ true;
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
