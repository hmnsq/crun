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
#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "status.h"
#समावेश "utils.h"
#समावेश "json_gen_utils.h"
#समावेश <stdlib.h>
#समावेश <unistd.h>
#समावेश <fcntl.h>
#समावेश <json-c/json.h>
#समावेश <ocispec/json_common.h>
#समावेश <sys/mount.h>
#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#समावेश <dirent.h>
#समावेश <signal.h>
#समावेश <limits.h>

#घोषणा STEAL_POINTER(x, y) \
  करो                        \
    {                       \
      *x = y;               \
      y = शून्य;             \
  } जबतक (0)

काष्ठा pid_stat
{
  अक्षर state;
  दीर्घ अंक_threads;
  अचिन्हित दीर्घ दीर्घ starttime;
};

अटल अंतरभूत bool
is_valid_id_char (अक्षर c)
{
  अगर ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    वापस true;
  वापस c == '_' || c == '+' || c == '.' || c == '-';
}

अटल पूर्णांक
validate_id (स्थिर अक्षर *id, libcrun_error_t *err)
{
  अगर (id == शून्य)
    वापस 0;

  अगर (id[0] == '\0')
    वापस crun_make_error (err, 0, "empty container ID");

  अगर (id[0] == '.')
    वापस crun_make_error (err, 0, "invalid container ID `%s`", id);

  क्रम (माप_प्रकार i = 0; id[i]; i++)
    {
      अगर (! is_valid_id_char (id[i]))
        वापस crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  वापस 0;
}

पूर्णांक
get_run_directory (अक्षर **out, स्थिर अक्षर *state_मूल, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *मूल = शून्य;

  अगर (state_मूल)
    मूल = xstrdup (state_मूल);
  अगर (मूल == शून्य)
    {
      स्थिर अक्षर *runtime_dir = दो_पर्या ("XDG_RUNTIME_DIR");
      अगर (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&मूल, err, runtime_dir, "crun", शून्य);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }
  अगर (मूल == शून्य)
    मूल = xstrdup ("/run/crun");

  ret = crun_ensure_directory (मूल, 0700, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  STEAL_POINTER (out, मूल);

  वापस 0;
}

पूर्णांक
get_shared_empty_directory_path (अक्षर **out, स्थिर अक्षर *state_मूल, libcrun_error_t *err)
{
  cleanup_free अक्षर *run_dir = शून्य;
  पूर्णांक ret;

  ret = get_run_directory (&run_dir, state_मूल, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

पूर्णांक
libcrun_get_state_directory (अक्षर **out, स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *path = शून्य;
  cleanup_free अक्षर *मूल = शून्य;

  ret = validate_id (id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = get_run_directory (&मूल, state_मूल, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_paths (&path, err, मूल, id, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  STEAL_POINTER (out, path);

  वापस 0;
}

अटल पूर्णांक
get_state_directory_status_file (अक्षर **out, स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *dir = शून्य;
  पूर्णांक ret;

  ret = libcrun_get_state_directory (&dir, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस append_paths (out, err, dir, "status", शून्य);
}

अटल पूर्णांक
read_pid_stat (pid_t pid, काष्ठा pid_stat *st, libcrun_error_t *err)
{
  cleanup_free अक्षर *बफर = शून्य;
  cleanup_close पूर्णांक fd = -1;
  अक्षर pid_stat_file[64];
  अक्षर *it, *s;
  पूर्णांक i, ret;

  ret = snprintf (pid_stat_file, माप (pid_stat_file), "/proc/%d/stat", pid);
  अगर (UNLIKELY (ret >= (पूर्णांक) माप (pid_stat_file)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");

  fd = खोलो (pid_stat_file, O_RDONLY | O_CLOEXEC);
  अगर (fd < 0)
    {
      /* The process already exited.  */
      अगर (त्रुटि_सं == ENOENT || त्रुटि_सं == ESRCH)
        {
          स_रखो (st, 0, माप (*st));
          वापस 0;
        }
      वापस crun_make_error (err, त्रुटि_सं, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &बफर, शून्य, err);
  अगर (ret < 0)
    {
      st->starttime = 0;
      st->अंक_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      वापस 0;
    }

  s = शून्य;

  /* Skip the first two arguments.  */
  क्रम (it = बफर; it; it = म_अक्षर (it + 1, ')'))
    s = it + 1;

  अगर (s)
    जबतक (*s == ' ')
      s++;

  अगर (s == शून्य || *s == '\0')
    वापस crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  क्रम (it = s + 1, i = 0; i < 19; i++)
    {
      it = म_अक्षर (it, ' ');
      अगर (it == शून्य)
        वापस crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      अगर (i == 16)
        st->अंक_threads = म_से_दीर्घ (it, शून्य, 10);
    }

  त्रुटि_सं = 0;
  st->starttime = strtoull (it, शून्य, 10);
  अगर (त्रुटि_सं != 0)
    वापस crun_make_error (err, त्रुटि_सं, "parse process start time");

  वापस 0;
}

पूर्णांक
libcrun_write_container_status (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  पूर्णांक r, ret;
  cleanup_free अक्षर *file_tmp = शून्य;
  cleanup_free अक्षर *file = शून्य;
  माप_प्रकार len;
  cleanup_close पूर्णांक fd_write = -1;
  स्थिर अक्षर *buf = शून्य;
  काष्ठा pid_stat st;
  स्थिर अक्षर *tmp;
  cleanup_json_gen json_gen_ctx *gen = शून्य;

  ret = get_state_directory_status_file (&file, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = read_pid_stat (status->pid, &st, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = खोलो (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  अगर (UNLIKELY (fd_write < 0))
    वापस crun_make_error (err, त्रुटि_सं, "cannot open status file");

  अगर (! json_gen_init (&gen, शून्य))
    वापस crun_make_error (err, 0, "json_gen_init failed");

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

  अगर (status->owner)
    {
      GEN_कुंजी (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_कुंजी (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_कुंजी (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  अगर (status->external_descriptors)
    {
      GEN_कुंजी (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  close_and_reset (&fd_write);

  अगर (UNLIKELY (नाम (file_tmp, file) < 0))
    वापस crun_make_error (err, त्रुटि_सं, "cannot rename status file");

  वापस 0;

gen_error:
  वापस json_gen_error_to_crun_error (r, err);
}

पूर्णांक
libcrun_read_container_status (libcrun_container_status_t *status, स्थिर अक्षर *state_मूल, स्थिर अक्षर *id,
                               libcrun_error_t *err)
{
  cleanup_free अक्षर *बफर = शून्य;
  पूर्णांक ret;
  cleanup_free अक्षर *file = शून्य;
  json_object *doc = शून्य;
  json_object *tmp;
  स्थिर अक्षर *val;

  ret = get_state_directory_status_file (&file, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = read_all_file (file, &बफर, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    {

      अगर (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free अक्षर *statedir = शून्य;
          libcrun_error_t tmp_err;
          पूर्णांक tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_मूल, id, &tmp_err);
          अगर (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          अन्यथा
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              अगर (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              अन्यथा अगर (tmp_ret == 0)
                वापस crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      वापस ret;
    }

  ret = parse_json_file (&doc, बफर, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  {
    tmp = json_object_object_get (doc, "pid");
    अगर (UNLIKELY (tmp == शून्य))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        जाओ निकास;
      }
    अगर (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        जाओ निकास;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    अगर (UNLIKELY (tmp == शून्य))
      status->process_start_time = 0; /* backwards compatibility */
    अन्यथा अगर (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        जाओ निकास;
      }
    अन्यथा
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    अगर (UNLIKELY (tmp == शून्य))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        जाओ निकास;
      }
    val = json_object_get_string (tmp);
    अगर (UNLIKELY (val == शून्य))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        जाओ निकास;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : शून्य;
    status->scope = val ? xstrdup (val) : शून्य;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    अगर (UNLIKELY (tmp == शून्य))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        जाओ निकास;
      }
    val = json_object_get_string (tmp);
    अगर (UNLIKELY (val == शून्य))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        जाओ निकास;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != शून्य && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    अगर (UNLIKELY (tmp == शून्य))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        जाओ निकास;
      }
    val = json_object_get_string (tmp);
    अगर (UNLIKELY (val == शून्य))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        जाओ निकास;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    अगर (UNLIKELY (tmp == शून्य))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        जाओ निकास;
      }
    val = json_object_get_string (tmp);
    अगर (UNLIKELY (val == शून्य))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        जाओ निकास;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : शून्य;
    status->owner = val ? xstrdup (val) : शून्य;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != शून्य && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != शून्य && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : शून्य;
    status->external_descriptors = val ? xstrdup (val) : शून्य;
  }
  ret = 0;

निकास:
  json_object_put (doc);
  वापस ret;
}

पूर्णांक
libcrun_status_check_directories (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *dir = शून्य;
  पूर्णांक ret;

  ret = libcrun_get_state_directory (&dir, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = crun_path_exists (dir, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (ret)
    वापस crun_make_error (err, 0, "container `%s` already exists", id);

  अगर (UNLIKELY (सूची_गढ़ो (dir, 0700) < 0))
    वापस crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  वापस 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#घोषणा RMDIRFD_MAX_MOVES (1U << 20)

अटल पूर्णांक
rmdirfd_move_to (सूची **d, स्थिर अक्षर *name, libcrun_error_t *err)
{
  cleanup_close पूर्णांक fd = -1;
  सूची *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  अगर (UNLIKELY (new_d == शून्य))
    वापस crun_make_error (err, त्रुटि_सं, "cannot open directory `%s`", name);

  fd = -1;

  बंद_सूची (*d);
  *d = new_d;
  वापस 0;
}

अटल पूर्णांक
rmdirfd (स्थिर अक्षर *namedir, पूर्णांक fd, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_dir सूची *d = शून्य;
  काष्ठा dirent *de;
  काष्ठा stat मूल_st;
  bool is_मूल = true;
  अचिन्हित पूर्णांक budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close पूर्णांक fd_cleanup = fd;

  ret = ख_स्थिति (fd, &मूल_st);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "stat `%s`", namedir);

  d = fdopendir (fd);
  अगर (d == शून्य)
    वापस crun_make_error (err, त्रुटि_सं, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  क्रम (;;)
    {
      त्रुटि_सं = 0;
      de = सूची_पढ़ो (d);
      अगर (de == शून्य)
        {
          काष्ठा stat st;

          अगर (UNLIKELY (त्रुटि_सं != 0))
            वापस crun_make_error (err, त्रुटि_सं, "cannot read directory");

          अगर (is_मूल)
            अवरोध;

          अगर (UNLIKELY (--budget == 0))
            वापस crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          ret = ख_स्थिति (dirfd (d), &st);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "stat `..`");

          is_मूल = st.st_dev == मूल_st.st_dev && st.st_ino == मूल_st.st_ino;
          जारी;
        }

      अगर ((म_भेद (de->d_name, ".") == 0) || (म_भेद (de->d_name, "..") == 0))
        जारी;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      अगर (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          अगर (ret < 0 && त्रुटि_सं == EBUSY)
            {
              cleanup_close पूर्णांक tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              अगर (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  अगर (umount2 (procpath, MNT_DETACH) == 0)
                    जाओ retry_unlink;
                }
              अगर (! is_मूल)
                वापस crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          अन्यथा अगर (ret < 0 && त्रुटि_सं == ENOTEMPTY)
            {
              अगर (UNLIKELY (--budget == 0))
                वापस crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              अगर (UNLIKELY (ret < 0))
                वापस ret;

              is_मूल = false;
            }
          अन्यथा अगर (ret < 0 && त्रुटि_सं != ENOENT && ! is_मूल)
            {
              वापस crun_make_error (err, त्रुटि_सं, "cannot rm `%s`", de->d_name);
            }
        }
    }

  वापस 0;
}

पूर्णांक
libcrun_container_delete_status (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_close पूर्णांक rundir_dfd = -1;
  cleanup_close पूर्णांक dfd = -1;
  cleanup_free अक्षर *dir = शून्य;

  ret = validate_id (id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = get_run_directory (&dir, state_मूल, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  rundir_dfd = अस्थायी_FAILURE_RETRY (खोलो (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  अगर (UNLIKELY (rundir_dfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  अगर (UNLIKELY (dfd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "cannot rm state directory `%s/%s`", dir, id);

  वापस 0;
}

व्योम
libcrun_free_container_status (libcrun_container_status_t *status)
{
  अगर (status == शून्य)
    वापस;
  मुक्त (status->cgroup_path);
  मुक्त (status->bundle);
  मुक्त (status->rootfs);
  मुक्त (status->external_descriptors);
  मुक्त (status->created);
  मुक्त (status->scope);
  मुक्त (status->owner);
}

पूर्णांक
libcrun_get_containers_list (libcrun_container_list_t **out, स्थिर अक्षर *state_मूल, libcrun_error_t *err)
{
  काष्ठा dirent *अग्र;
  cleanup_container_list libcrun_container_list_t *tmp = शून्य;
  cleanup_free अक्षर *मूल = शून्य;
  cleanup_dir सूची *dir = शून्य;
  पूर्णांक ret;

  *out = शून्य;

  ret = get_run_directory (&मूल, state_मूल, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  dir = सूची_खोलो (मूल);
  अगर (UNLIKELY (dir == शून्य))
    वापस crun_make_error (err, त्रुटि_सं, "cannot opendir `%s`", मूल);

  क्रम (अग्र = सूची_पढ़ो (dir); अग्र; अग्र = सूची_पढ़ो (dir))
    {
      पूर्णांक exists;
      cleanup_free अक्षर *status_file = शून्य;

      libcrun_container_list_t *अग्र_container;

      अगर (अग्र->d_name[0] == '.')
        जारी;

      ret = append_paths (&status_file, err, मूल, अग्र->d_name, "status", शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      exists = crun_path_exists (status_file, err);
      अगर (exists < 0)
        {
          वापस exists;
        }

      अगर (! exists)
        {
          libcrun_error (त्रुटि_सं, "error opening file `%s`", status_file);
          जारी;
        }

      अग्र_container = xmalloc (माप (libcrun_container_list_t));
      अग्र_container->name = xstrdup (अग्र->d_name);
      अग्र_container->अग्र = tmp;
      tmp = अग्र_container;
    }

  STEAL_POINTER (out, tmp);

  वापस 0;
}

व्योम
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *अग्र;
  जबतक (list)
    {
      अग्र = list->अग्र;
      मुक्त (list->name);
      मुक्त (list);
      list = अग्र;
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
  काष्ठा pid_stat st;
  पूर्णांक ret;

  /* For backwards compatibility, check start time only if available. */
  अगर (! status->process_start_time)
    वापस 1;

  ret = read_pid_stat (status->pid, &st, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (status->process_start_time != st.starttime || st.state == 'X')
    वापस 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  अगर (st.state == 'Z' && st.अंक_threads <= 1)
    वापस 0; /* stopped */

  वापस 1; /* running, created, or paused */
}

पूर्णांक
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = समाप्त (status->pid, 0);
  अगर (UNLIKELY (ret < 0) && त्रुटि_सं != ESRCH)
    वापस crun_make_error (err, त्रुटि_सं, "kill");

  अगर (ret == 0)
    वापस libcrun_check_pid_valid (status, err);

  वापस 0; /* stopped */
}

अटल पूर्णांक
get_exec_fifo_path (अक्षर **out, स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *state_dir = शून्य;
  पूर्णांक ret;

  ret = libcrun_get_state_directory (&state_dir, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस append_paths (out, err, state_dir, "exec.fifo", शून्य);
}

पूर्णांक
libcrun_status_create_exec_fifo (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *fifo_path = शून्य;
  पूर्णांक ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "mkfifo");

  fd = खोलो (fifo_path, O_NONBLOCK | O_CLOEXEC);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "cannot open pipe `%s`", fifo_path);

  वापस fd;
}

पूर्णांक
libcrun_status_write_exec_fifo (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *fifo_path = शून्य;
  अक्षर बफर[1] = {
    0,
  };
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret;

  ret = get_exec_fifo_path (&fifo_path, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  fd = खोलो (fifo_path, O_WRONLY | O_CLOEXEC);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "unlink `%s`", fifo_path);

  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (fd, बफर, 1));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "write to exec.fifo");

  वापस 0;
}

पूर्णांक
libcrun_status_has_read_exec_fifo (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err)
{
  cleanup_free अक्षर *fifo_path = शून्य;
  पूर्णांक ret;

  ret = get_exec_fifo_path (&fifo_path, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस crun_path_exists (fifo_path, err);
}

काष्ठा libcrun_status_s
{
  libcrun_container_status_t status;
  अक्षर *id;
  अक्षर *state_मूल;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  पूर्णांक running;
};

पूर्णांक
libcrun_container_status_load (libcrun_context_t *context, स्थिर अक्षर *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  पूर्णांक ret;

  st = xmalloc0 (माप (*st));

  ret = libcrun_read_container_status (&st->status, context->state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    {
      मुक्त (st);
      वापस ret;
    }

  st->id = xstrdup (id);
  st->state_मूल = xstrdup (context->state_मूल);

  *out = st;
  वापस 0;
}

व्योम
libcrun_container_status_free (libcrun_status_t *st)
{
  अगर (st == शून्य)
    वापस;
  libcrun_free_container_status (&st->status);
  मुक्त (st->state_मूल);
  मुक्त (st->id);
  मुक्त (st);
}

पूर्णांक
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, पूर्णांक *running, libcrun_error_t *err)
{
  अगर (! st->state_resolved)
    {
      स्थिर अक्षर *state_string = शून्य;
      पूर्णांक ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_मूल, &state_string, &st->running, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      अगर (state_string == शून्य)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      अन्यथा अगर (म_भेद (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      अन्यथा अगर (म_भेद (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      अन्यथा अगर (म_भेद (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      अन्यथा अगर (म_भेद (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      अन्यथा
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  अगर (state)
    *state = st->state;
  अगर (running)
    *running = st->running;
  वापस 0;
}

स्थिर अक्षर *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  चयन (state)
    {
    हाल LIBCRUN_CONTAINER_STATUS_CREATING:
      वापस "creating";
    हाल LIBCRUN_CONTAINER_STATUS_CREATED:
      वापस "created";
    हाल LIBCRUN_CONTAINER_STATUS_RUNNING:
      वापस "running";
    हाल LIBCRUN_CONTAINER_STATUS_PAUSED:
      वापस "paused";
    हाल LIBCRUN_CONTAINER_STATUS_STOPPED:
      वापस "stopped";
    }
  वापस "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  वापस st->status.pid;
}

स्थिर अक्षर *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  वापस st->status.bundle;
}

स्थिर अक्षर *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  वापस st->status.rootfs;
}

स्थिर अक्षर *
libcrun_status_get_created (libcrun_status_t *st)
{
  वापस st->status.created;
}

स्थिर अक्षर *
libcrun_status_get_owner (libcrun_status_t *st)
{
  वापस st->status.owner;
}

स्थिर अक्षर *
libcrun_status_get_scope (libcrun_status_t *st)
{
  वापस st->status.scope;
}

स्थिर अक्षर *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  वापस st->status.external_descriptors;
}

पूर्णांक
libcrun_container_get_state_string (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर **out, पूर्णांक *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  पूर्णांक local_running;
  पूर्णांक ret;

  ret = libcrun_read_container_status (&status, context->state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_मूल, out, &local_running, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (running)
    *running = local_running;
  वापस 0;
}

पूर्णांक
libcrun_get_state_dir (अक्षर **out, स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err)
{
  वापस libcrun_get_state_directory (out, state_मूल, id, err);
}

पूर्णांक
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  वापस libcrun_get_containers_list (out, context->state_मूल, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (माप (*it));
  it->cur = l;
  वापस it;
}

bool
libcrun_container_iter_अग्र (libcrun_container_iter_t *it, स्थिर अक्षर **id)
{
  अगर (it == शून्य || it->cur == शून्य)
    वापस false;

  अगर (id)
    *id = it->cur->name;
  it->cur = it->cur->अग्र;
  वापस true;
}

व्योम
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  मुक्त (it);
}

व्योम
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
