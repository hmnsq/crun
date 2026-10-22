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
#परिभाषय _GNU_SOURCE

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

#परिभाषय STEAL_POINTER(x, y) \
  कुरु                        \
    {                       \
      *x = y;               \
      y = रिक्त;             \
  } यावत् (0)

संरचना pid_stat
{
  वर्ण state;
  दीर्घ अङ्कः_तन्तवः;
  अचिह्नित दीर्घ दीर्घ starttime;
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
get_सततम्_directory (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *state_मूलम्, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *मूलम् = रिक्त;

  यदि (state_मूलम्)
    मूलम् = xstrdup (state_मूलम्);
  यदि (मूलम् == रिक्त)
    {
      ध्रुव वर्ण *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      यदि (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&मूलम्, err, runtime_dir, "crun", रिक्त);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }
  यदि (मूलम् == रिक्त)
    मूलम् = xstrdup ("/run/crun");

  ret = crun_ensure_directory (मूलम्, 0700, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  STEAL_POINTER (बहिर्गमनम्, मूलम्);

  प्रत्यागम 0;
}

पूर्णाङ्क
get_shared_रिक्तानि_directory_मार्गः (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *state_मूलम्, libcrun_error_t *err)
{
  cleanup_free वर्ण *सततम्_dir = रिक्त;
  पूर्णाङ्क ret;

  ret = get_सततम्_directory (&सततम्_dir, state_मूलम्, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (बहिर्गमनम्, err, सततम्_dir, ".empty-directory", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*बहिर्गमनम्, 0555, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_get_state_directory (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *मार्गः = रिक्त;
  cleanup_free वर्ण *मूलम् = रिक्त;

  ret = validate_id (id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = get_सततम्_directory (&मूलम्, state_मूलम्, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (&मार्गः, err, मूलम्, id, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  STEAL_POINTER (बहिर्गमनम्, मार्गः);

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
get_state_directory_स्थितिः_file (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *dir = रिक्त;
  पूर्णाङ्क ret;

  ret = libcrun_get_state_directory (&dir, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम append_paths (बहिर्गमनम्, err, dir, "status", रिक्त);
}

स्थिर पूर्णाङ्क
read_pid_stat (pid_t pid, संरचना pid_stat *st, libcrun_error_t *err)
{
  cleanup_free वर्ण *बफर = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  वर्ण pid_stat_file[64];
  वर्ण *it, *s;
  पूर्णाङ्क i, ret;

  ret = snprintf (pid_stat_file, परिमाण (pid_stat_file), "/proc/%d/stat", pid);
  यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (pid_stat_file)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  fd = उद्घाटय (pid_stat_file, O_RDONLY | O_CLOEXEC);
  यदि (fd < 0)
    {
      /* The process already exited.  */
      यदि (errno == ENOENT || errno == ESRCH)
        {
          स्मृतिं_पूरय (st, 0, परिमाण (*st));
          प्रत्यागम 0;
        }
      प्रत्यागम crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &बफर, रिक्त, err);
  यदि (ret < 0)
    {
      st->starttime = 0;
      st->अङ्कः_तन्तवः = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      प्रत्यागम 0;
    }

  s = रिक्त;

  /* Skip the first two arguments.  */
  प्रति (it = बफर; it; it = strchr (it + 1, ')'))
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
        st->अङ्कः_तन्तवः = strtol (it, रिक्त, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, रिक्त, 10);
  यदि (errno != 0)
    प्रत्यागम crun_make_error (err, errno, "parse process start time");

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_write_container_स्थितिः (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_container_स्थितिः_t *स्थितिः,
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

  ret = get_state_directory_स्थितिः_file (&file, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_pid_stat (स्थितिः->pid, &st, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  स्थितिः->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = उद्घाटय (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  यदि (UNLIKELY (fd_write < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot open status file");

  यदि (! json_gen_init (&gen, रिक्त))
    प्रत्यागम crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_कुञ्चिका (gen, "pid");
  GEN_OR_FAIL (map_int (gen, स्थितिः->pid));

  GEN_कुञ्चिका (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, स्थितिः->process_start_time));

  GEN_कुञ्चिका (gen, "cgroup-path");
  tmp = स्थितिः->cgroup_मार्गः ? स्थितिः->cgroup_मार्गः : "";
  GEN_STR (gen, tmp);

  GEN_कुञ्चिका (gen, "scope");
  tmp = स्थितिः->scope ? स्थितिः->scope : "";
  GEN_STR (gen, tmp);

  GEN_कुञ्चिका (gen, "rootfs");
  GEN_STR (gen, स्थितिः->rootfs);

  GEN_कुञ्चिका (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, स्थितिः->systemd_cgroup));

  GEN_कुञ्चिका (gen, "bundle");
  GEN_STR (gen, स्थितिः->bundle);

  GEN_कुञ्चिका (gen, "created");
  GEN_STR (gen, स्थितिः->created);

  यदि (स्थितिः->owner)
    {
      GEN_कुञ्चिका (gen, "owner");
      GEN_STR (gen, स्थितिः->owner);
    }

  GEN_कुञ्चिका (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, स्थितिः->detached));

  GEN_कुञ्चिका (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, स्थितिः->no_new_keyring));

  यदि (स्थितिः->external_descriptors)
    {
      GEN_कुञ्चिका (gen, "external_descriptors");
      GEN_STR (gen, स्थितिः->external_descriptors);
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
  प्रत्यागम json_gen_error_गन्तव्यम्_crun_error (r, err);
}

पूर्णाङ्क
libcrun_read_container_स्थितिः (libcrun_container_स्थितिः_t *स्थितिः, ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id,
                               libcrun_error_t *err)
{
  cleanup_free वर्ण *बफर = रिक्त;
  पूर्णाङ्क ret;
  cleanup_free वर्ण *file = रिक्त;
  json_object *doc = रिक्त;
  json_object *tmp;
  ध्रुव वर्ण *val;

  ret = get_state_directory_स्थितिः_file (&file, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file (file, &बफर, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    {

      यदि (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free वर्ण *statedir = रिक्त;
          libcrun_error_t tmp_err;
          पूर्णाङ्क tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_मूलम्, id, &tmp_err);
          यदि (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          अन्यथा
            {
              tmp_ret = crun_मार्गः_exists (statedir, &tmp_err);
              यदि (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              अन्यथा यदि (tmp_ret == 0)
                प्रत्यागम crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      प्रत्यागम ret;
    }

  ret = parse_json_file (&doc, बफर, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  {
    tmp = json_object_object_get (doc, "pid");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        गच्छ निर्गच्छ;
      }
    यदि (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        गच्छ निर्गच्छ;
      }
    स्थितिः->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    यदि (UNLIKELY (tmp == रिक्त))
      स्थितिः->process_start_time = 0; /* backwards compatibility */
    अन्यथा यदि (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        गच्छ निर्गच्छ;
      }
    अन्यथा
      स्थितिः->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        गच्छ निर्गच्छ;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        गच्छ निर्गच्छ;
      }
    स्थितिः->cgroup_मार्गः = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : रिक्त;
    स्थितिः->scope = val ? xstrdup (val) : रिक्त;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        गच्छ निर्गच्छ;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        गच्छ निर्गच्छ;
      }
    स्थितिः->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    स्थितिः->systemd_cgroup = tmp != रिक्त && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        गच्छ निर्गच्छ;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        गच्छ निर्गच्छ;
      }
    स्थितिः->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    यदि (UNLIKELY (tmp == रिक्त))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        गच्छ निर्गच्छ;
      }
    val = json_object_get_string (tmp);
    यदि (UNLIKELY (val == रिक्त))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        गच्छ निर्गच्छ;
      }
    स्थितिः->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : रिक्त;
    स्थितिः->owner = val ? xstrdup (val) : रिक्त;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    स्थितिः->detached = tmp != रिक्त && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    स्थितिः->no_new_keyring = tmp != रिक्त && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : रिक्त;
    स्थितिः->external_descriptors = val ? xstrdup (val) : रिक्त;
  }
  ret = 0;

निर्गच्छ:
  json_object_put (doc);
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_स्थितिः_check_directories (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *dir = रिक्त;
  पूर्णाङ्क ret;

  ret = libcrun_get_state_directory (&dir, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = crun_मार्गः_exists (dir, err);
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
#परिभाषय RMDIRFD_MAX_MOVES (1U << 20)

स्थिर पूर्णाङ्क
rmdirfd_move_गन्तव्यम् (DIR **d, ध्रुव वर्ण *name, libcrun_error_t *err)
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
  संरचना stat मूलम्_st;
  bool is_मूलम् = true;
  अचिह्नित पूर्णाङ्क budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((अप्रयुक्तम्)) cleanup_close पूर्णाङ्क fd_cleanup = fd;

  ret = fstat (fd, &मूलम्_st);
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

          यदि (is_मूलम्)
            भङ्ग;

          यदि (UNLIKELY (--budget == 0))
            प्रत्यागम crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_गन्तव्यम् (&d, "..", err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          ret = fstat (dirfd (d), &st);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "stat `..`");

          is_मूलम् = st.st_dev == मूलम्_st.st_dev && st.st_ino == मूलम्_st.st_ino;
          अनुवर्त;
        }

      यदि ((वाक्यं_तुलय (de->d_name, ".") == 0) || (वाक्यं_तुलय (de->d_name, "..") == 0))
        अनुवर्त;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      यदि (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          यदि (ret < 0 && errno == EBUSY)
            {
              cleanup_close पूर्णाङ्क tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_मार्गः | O_NOFOLLOW);
              यदि (tfd >= 0)
                {
                  proc_fd_मार्गः_t procpath;

                  get_proc_self_fd_मार्गः (procpath, tfd);
                  यदि (umount2 (procpath, MNT_DETACH) == 0)
                    गच्छ retry_unlink;
                }
              यदि (! is_मूलम्)
                प्रत्यागम crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          अन्यथा यदि (ret < 0 && errno == ENOTEMPTY)
            {
              यदि (UNLIKELY (--budget == 0))
                प्रत्यागम crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_गन्तव्यम् (&d, de->d_name, err);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम ret;

              is_मूलम् = false;
            }
          अन्यथा यदि (ret < 0 && errno != ENOENT && ! is_मूलम्)
            {
              प्रत्यागम crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_container_delete_स्थितिः (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_close पूर्णाङ्क rundir_dfd = -1;
  cleanup_close पूर्णाङ्क dfd = -1;
  cleanup_free वर्ण *dir = रिक्त;

  ret = validate_id (id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = get_सततम्_directory (&dir, state_मूलम्, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  rundir_dfd = अस्थायी_FAILURE_RETRY (उद्घाटय (dir, O_DIRECTORY | O_मार्गः | O_CLOEXEC));
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
libcrun_free_container_स्थितिः (libcrun_container_स्थितिः_t *स्थितिः)
{
  यदि (स्थितिः == रिक्त)
    प्रत्यागम;
  मुक्त (स्थितिः->cgroup_मार्गः);
  मुक्त (स्थितिः->bundle);
  मुक्त (स्थितिः->rootfs);
  मुक्त (स्थितिः->external_descriptors);
  मुक्त (स्थितिः->created);
  मुक्त (स्थितिः->scope);
  मुक्त (स्थितिः->owner);
}

पूर्णाङ्क
libcrun_get_containers_list (libcrun_container_list_t **बहिर्गमनम्, ध्रुव वर्ण *state_मूलम्, libcrun_error_t *err)
{
  संरचना dirent *अग्र;
  cleanup_container_list libcrun_container_list_t *tmp = रिक्त;
  cleanup_free वर्ण *मूलम् = रिक्त;
  cleanup_dir DIR *dir = रिक्त;
  पूर्णाङ्क ret;

  *बहिर्गमनम् = रिक्त;

  ret = get_सततम्_directory (&मूलम्, state_मूलम्, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  dir = opendir (मूलम्);
  यदि (UNLIKELY (dir == रिक्त))
    प्रत्यागम crun_make_error (err, errno, "cannot opendir `%s`", मूलम्);

  प्रति (अग्र = readdir (dir); अग्र; अग्र = readdir (dir))
    {
      पूर्णाङ्क exists;
      cleanup_free वर्ण *स्थितिः_file = रिक्त;

      libcrun_container_list_t *अग्र_container;

      यदि (अग्र->d_name[0] == '.')
        अनुवर्त;

      ret = append_paths (&स्थितिः_file, err, मूलम्, अग्र->d_name, "status", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      exists = crun_मार्गः_exists (स्थितिः_file, err);
      यदि (exists < 0)
        {
          प्रत्यागम exists;
        }

      यदि (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", स्थितिः_file);
          अनुवर्त;
        }

      अग्र_container = xmalloc (परिमाण (libcrun_container_list_t));
      अग्र_container->name = xstrdup (अग्र->d_name);
      अग्र_container->अग्र = tmp;
      tmp = अग्र_container;
    }

  STEAL_POINTER (बहिर्गमनम्, tmp);

  प्रत्यागम 0;
}

शून्य
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *अग्र;
  यावत् (list)
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
पूर्णाङ्क
libcrun_check_pid_valid (libcrun_container_स्थितिः_t *स्थितिः, libcrun_error_t *err)
{
  संरचना pid_stat st;
  पूर्णाङ्क ret;

  /* For backwards compatibility, check start time only if available. */
  यदि (! स्थितिः->process_start_time)
    प्रत्यागम 1;

  ret = read_pid_stat (स्थितिः->pid, &st, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (स्थितिः->process_start_time != st.starttime || st.state == 'X')
    प्रत्यागम 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  यदि (st.state == 'Z' && st.अङ्कः_तन्तवः <= 1)
    प्रत्यागम 0; /* stopped */

  प्रत्यागम 1; /* running, created, or paused */
}

पूर्णाङ्क
libcrun_is_container_running (libcrun_container_स्थितिः_t *स्थितिः, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = kill (स्थितिः->pid, 0);
  यदि (UNLIKELY (ret < 0) && errno != ESRCH)
    प्रत्यागम crun_make_error (err, errno, "kill");

  यदि (ret == 0)
    प्रत्यागम libcrun_check_pid_valid (स्थितिः, err);

  प्रत्यागम 0; /* stopped */
}

स्थिर पूर्णाङ्क
get_exec_fifo_मार्गः (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *state_dir = रिक्त;
  पूर्णाङ्क ret;

  ret = libcrun_get_state_directory (&state_dir, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम append_paths (बहिर्गमनम्, err, state_dir, "exec.fifo", रिक्त);
}

पूर्णाङ्क
libcrun_स्थितिः_create_exec_fifo (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *fifo_मार्गः = रिक्त;
  पूर्णाङ्क ret, fd = -1;

  ret = get_exec_fifo_मार्गः (&fifo_मार्गः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_मार्गः);
  ret = mkfifo (fifo_मार्गः, 0600);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "mkfifo");

  fd = उद्घाटय (fifo_मार्गः, O_NONBLOCK | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot open pipe `%s`", fifo_मार्गः);

  प्रत्यागम fd;
}

पूर्णाङ्क
libcrun_स्थितिः_write_exec_fifo (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *fifo_मार्गः = रिक्त;
  वर्ण बफर[1] = {
    0,
  };
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;

  ret = get_exec_fifo_मार्गः (&fifo_मार्गः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  fd = उद्घाटय (fifo_मार्गः, O_WRONLY | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "cannot open `%s`", fifo_मार्गः);

  ret = unlink (fifo_मार्गः);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "unlink `%s`", fifo_मार्गः);

  ret = अस्थायी_FAILURE_RETRY (लिख (fd, बफर, 1));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "write to exec.fifo");

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_स्थितिः_has_read_exec_fifo (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_free वर्ण *fifo_मार्गः = रिक्त;
  पूर्णाङ्क ret;

  ret = get_exec_fifo_मार्गः (&fifo_मार्गः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम crun_मार्गः_exists (fifo_मार्गः, err);
}

संरचना libcrun_स्थितिः_s
{
  libcrun_container_स्थितिः_t स्थितिः;
  वर्ण *id;
  वर्ण *state_मूलम्;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  पूर्णाङ्क running;
};

पूर्णाङ्क
libcrun_container_स्थितिः_आरोपय (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_स्थितिः_t **बहिर्गमनम्, libcrun_error_t *err)
{
  libcrun_स्थितिः_t *st;
  पूर्णाङ्क ret;

  st = xmalloc0 (परिमाण (*st));

  ret = libcrun_read_container_स्थितिः (&st->स्थितिः, context->state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    {
      मुक्त (st);
      प्रत्यागम ret;
    }

  st->id = xstrdup (id);
  st->state_मूलम् = xstrdup (context->state_मूलम्);

  *बहिर्गमनम् = st;
  प्रत्यागम 0;
}

शून्य
libcrun_container_स्थितिः_free (libcrun_स्थितिः_t *st)
{
  यदि (st == रिक्त)
    प्रत्यागम;
  libcrun_free_container_स्थितिः (&st->स्थितिः);
  मुक्त (st->state_मूलम्);
  मुक्त (st->id);
  मुक्त (st);
}

पूर्णाङ्क
libcrun_स्थितिः_get_state (libcrun_स्थितिः_t *st, libcrun_container_state_t *state, पूर्णाङ्क *running, libcrun_error_t *err)
{
  यदि (! st->state_resolved)
    {
      ध्रुव वर्ण *state_string = रिक्त;
      पूर्णाङ्क ret;

      ret = libcrun_get_container_state_string (st->id, &st->स्थितिः, st->state_मूलम्, &state_string, &st->running, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      यदि (state_string == रिक्त)
        st->state = LIBCRUN_CONTAINER_स्थितिः_STOPPED;
      अन्यथा यदि (वाक्यं_तुलय (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_स्थितिः_CREATING;
      अन्यथा यदि (वाक्यं_तुलय (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_स्थितिः_CREATED;
      अन्यथा यदि (वाक्यं_तुलय (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_स्थितिः_RUNNING;
      अन्यथा यदि (वाक्यं_तुलय (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_स्थितिः_PAUSED;
      अन्यथा
        st->state = LIBCRUN_CONTAINER_स्थितिः_STOPPED;

      st->state_resolved = true;
    }

  यदि (state)
    *state = st->state;
  यदि (running)
    *running = st->running;
  प्रत्यागम 0;
}

ध्रुव वर्ण *
libcrun_container_state_गन्तव्यम्_string (libcrun_container_state_t state)
{
  विकल्प (state)
    {
    पक्ष LIBCRUN_CONTAINER_स्थितिः_CREATING:
      प्रत्यागम "creating";
    पक्ष LIBCRUN_CONTAINER_स्थितिः_CREATED:
      प्रत्यागम "created";
    पक्ष LIBCRUN_CONTAINER_स्थितिः_RUNNING:
      प्रत्यागम "running";
    पक्ष LIBCRUN_CONTAINER_स्थितिः_PAUSED:
      प्रत्यागम "paused";
    पक्ष LIBCRUN_CONTAINER_स्थितिः_STOPPED:
      प्रत्यागम "stopped";
    }
  प्रत्यागम "unknown";
}

pid_t
libcrun_स्थितिः_get_pid (libcrun_स्थितिः_t *st)
{
  प्रत्यागम st->स्थितिः.pid;
}

ध्रुव वर्ण *
libcrun_स्थितिः_get_bundle (libcrun_स्थितिः_t *st)
{
  प्रत्यागम st->स्थितिः.bundle;
}

ध्रुव वर्ण *
libcrun_स्थितिः_get_rootfs (libcrun_स्थितिः_t *st)
{
  प्रत्यागम st->स्थितिः.rootfs;
}

ध्रुव वर्ण *
libcrun_स्थितिः_get_created (libcrun_स्थितिः_t *st)
{
  प्रत्यागम st->स्थितिः.created;
}

ध्रुव वर्ण *
libcrun_स्थितिः_get_owner (libcrun_स्थितिः_t *st)
{
  प्रत्यागम st->स्थितिः.owner;
}

ध्रुव वर्ण *
libcrun_स्थितिः_get_scope (libcrun_स्थितिः_t *st)
{
  प्रत्यागम st->स्थितिः.scope;
}

ध्रुव वर्ण *
libcrun_स्थितिः_get_external_descriptors (libcrun_स्थितिः_t *st)
{
  प्रत्यागम st->स्थितिः.external_descriptors;
}

पूर्णाङ्क
libcrun_container_get_state_string (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण **बहिर्गमनम्, पूर्णाङ्क *running,
                                    libcrun_error_t *err)
{
  cleanup_container_स्थितिः libcrun_container_स्थितिः_t स्थितिः = {};
  पूर्णाङ्क local_running;
  पूर्णाङ्क ret;

  ret = libcrun_read_container_स्थितिः (&स्थितिः, context->state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_get_container_state_string (id, &स्थितिः, context->state_मूलम्, बहिर्गमनम्, &local_running, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (running)
    *running = local_running;
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_get_state_dir (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  प्रत्यागम libcrun_get_state_directory (बहिर्गमनम्, state_मूलम्, id, err);
}

पूर्णाङ्क
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **बहिर्गमनम्, libcrun_error_t *err)
{
  प्रत्यागम libcrun_get_containers_list (बहिर्गमनम्, context->state_मूलम्, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (परिमाण (*it));
  it->cur = l;
  प्रत्यागम it;
}

bool
libcrun_container_iter_अग्र (libcrun_container_iter_t *it, ध्रुव वर्ण **id)
{
  यदि (it == रिक्त || it->cur == रिक्त)
    प्रत्यागम false;

  यदि (id)
    *id = it->cur->name;
  it->cur = it->cur->अग्र;
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
