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
#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "status.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "json_gen_utils.h"
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <json-c/json.h>
#అన్తర్భావ <ocispec/json_common.h>
#అన్తర్భావ <sys/mount.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <dirent.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <limits.h>

#నిర్వచించు STEAL_సూచిక(x, y) \
  చేయి                        \
    {                       \
      *x = y;               \
      y = ఖాళీ;             \
  } అయితే (0)

నిర్మాణం pid_stat
{
  అక్షరం state;
  దీర్ఘ అంకె_దారాలు;
  గుర్తులేని దీర్ఘ దీర్ఘ starttime;
};

స్థిర అంతఃవరుస bool
is_valid_id_char (అక్షరం c)
{
  ఒకవేళ ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    తిరిగి true;
  తిరిగి c == '_' || c == '+' || c == '.' || c == '-';
}

స్థిర పూర్ణాంకం
validate_id (స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  ఒకవేళ (id == ఖాళీ)
    తిరిగి 0;

  ఒకవేళ (id[0] == '\0')
    తిరిగి crun_make_error (err, 0, "empty container ID");

  ఒకవేళ (id[0] == '.')
    తిరిగి crun_make_error (err, 0, "invalid container ID `%s`", id);

  కొరకు (size_t i = 0; id[i]; i++)
    {
      ఒకవేళ (! is_valid_id_char (id[i]))
        తిరిగి crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  తిరిగి 0;
}

పూర్ణాంకం
get_నిరంతరం_directory (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_free అక్షరం *మూలం = ఖాళీ;

  ఒకవేళ (state_మూలం)
    మూలం = xstrdup (state_మూలం);
  ఒకవేళ (మూలం == ఖాళీ)
    {
      స్థిరరాశి అక్షరం *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      ఒకవేళ (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&మూలం, err, runtime_dir, "crun", ఖాళీ);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }
  ఒకవేళ (మూలం == ఖాళీ)
    మూలం = xstrdup ("/run/crun");

  ret = crun_ensure_directory (మూలం, 0700, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  STEAL_సూచిక (బహిర్గతం, మూలం);

  తిరిగి 0;
}

పూర్ణాంకం
get_shared_ఖాళీలు_directory_మార్గం (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *నిరంతరం_dir = ఖాళీ;
  పూర్ణాంకం ret;

  ret = get_నిరంతరం_directory (&నిరంతరం_dir, state_మూలం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_paths (బహిర్గతం, err, నిరంతరం_dir, ".empty-directory", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*బహిర్గతం, 0555, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_get_state_directory (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  cleanup_free అక్షరం *మూలం = ఖాళీ;

  ret = validate_id (id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = get_నిరంతరం_directory (&మూలం, state_మూలం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_paths (&మార్గం, err, మూలం, id, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  STEAL_సూచిక (బహిర్గతం, మార్గం);

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
get_state_directory_స్థితి_file (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  cleanup_free అక్షరం *dir = ఖాళీ;
  పూర్ణాంకం ret;

  ret = libcrun_get_state_directory (&dir, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి append_paths (బహిర్గతం, err, dir, "status", ఖాళీ);
}

స్థిర పూర్ణాంకం
read_pid_stat (pid_t pid, నిర్మాణం pid_stat *st, libcrun_error_t *err)
{
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  cleanup_close పూర్ణాంకం fd = -1;
  అక్షరం pid_stat_file[64];
  అక్షరం *it, *s;
  పూర్ణాంకం i, ret;

  ret = snprintf (pid_stat_file, పరిమాణం (pid_stat_file), "/proc/%d/stat", pid);
  ఒకవేళ (UNLIKELY (ret >= (పూర్ణాంకం) పరిమాణం (pid_stat_file)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  ఒకవేళ (fd < 0)
    {
      /* The process already exited.  */
      ఒకవేళ (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, పరిమాణం (*st));
          తిరిగి 0;
        }
      తిరిగి crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &బఫరు, ఖాళీ, err);
  ఒకవేళ (ret < 0)
    {
      st->starttime = 0;
      st->అంకె_దారాలు = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      తిరిగి 0;
    }

  s = ఖాళీ;

  /* Skip the first two arguments.  */
  కొరకు (it = బఫరు; it; it = strchr (it + 1, ')'))
    s = it + 1;

  ఒకవేళ (s)
    అయితే (*s == ' ')
      s++;

  ఒకవేళ (s == ఖాళీ || *s == '\0')
    తిరిగి crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  కొరకు (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      ఒకవేళ (it == ఖాళీ)
        తిరిగి crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      ఒకవేళ (i == 16)
        st->అంకె_దారాలు = strtol (it, ఖాళీ, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, ఖాళీ, 10);
  ఒకవేళ (errno != 0)
    తిరిగి crun_make_error (err, errno, "parse process start time");

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_write_container_స్థితి (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_container_స్థితి_t *స్థితి_2,
                                libcrun_error_t *err)
{
  పూర్ణాంకం r, ret;
  cleanup_free అక్షరం *file_tmp = ఖాళీ;
  cleanup_free అక్షరం *file = ఖాళీ;
  size_t len;
  cleanup_close పూర్ణాంకం fd_write = -1;
  స్థిరరాశి అక్షరం *buf = ఖాళీ;
  నిర్మాణం pid_stat st;
  స్థిరరాశి అక్షరం *tmp;
  cleanup_json_gen json_gen_ctx *gen = ఖాళీ;

  ret = get_state_directory_స్థితి_file (&file, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = read_pid_stat (స్థితి_2->pid, &st, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  స్థితి_2->process_ప్రారంభం_సమయం = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  ఒకవేళ (UNLIKELY (fd_write < 0))
    తిరిగి crun_make_error (err, errno, "cannot open status file");

  ఒకవేళ (! json_gen_init (&gen, ఖాళీ))
    తిరిగి crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_కీలకం (gen, "pid");
  GEN_OR_FAIL (map_int (gen, స్థితి_2->pid));

  GEN_కీలకం (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, స్థితి_2->process_ప్రారంభం_సమయం));

  GEN_కీలకం (gen, "cgroup-path");
  tmp = స్థితి_2->cgroup_మార్గం ? స్థితి_2->cgroup_మార్గం : "";
  GEN_STR (gen, tmp);

  GEN_కీలకం (gen, "scope");
  tmp = స్థితి_2->scope ? స్థితి_2->scope : "";
  GEN_STR (gen, tmp);

  GEN_కీలకం (gen, "rootfs");
  GEN_STR (gen, స్థితి_2->rootfs);

  GEN_కీలకం (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, స్థితి_2->systemd_cgroup));

  GEN_కీలకం (gen, "bundle");
  GEN_STR (gen, స్థితి_2->bundle);

  GEN_కీలకం (gen, "created");
  GEN_STR (gen, స్థితి_2->created);

  ఒకవేళ (స్థితి_2->owner)
    {
      GEN_కీలకం (gen, "owner");
      GEN_STR (gen, స్థితి_2->owner);
    }

  GEN_కీలకం (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, స్థితి_2->detached));

  GEN_కీలకం (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, స్థితి_2->no_new_keyring));

  ఒకవేళ (స్థితి_2->external_descriptors)
    {
      GEN_కీలకం (gen, "external_descriptors");
      GEN_STR (gen, స్థితి_2->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = సురక్షిత_write (fd_write, "status file", buf, len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  close_and_reset (&fd_write);

  ఒకవేళ (UNLIKELY (rename (file_tmp, file) < 0))
    తిరిగి crun_make_error (err, errno, "cannot rename status file");

  తిరిగి 0;

gen_error:
  తిరిగి json_gen_error_గమ్యం_crun_error (r, err);
}

పూర్ణాంకం
libcrun_read_container_స్థితి (libcrun_container_స్థితి_t *స్థితి_2, స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id,
                               libcrun_error_t *err)
{
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  పూర్ణాంకం ret;
  cleanup_free అక్షరం *file = ఖాళీ;
  json_object *doc = ఖాళీ;
  json_object *tmp;
  స్థిరరాశి అక్షరం *val;

  ret = get_state_directory_స్థితి_file (&file, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = read_all_file (file, &బఫరు, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {

      ఒకవేళ (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free అక్షరం *statedir = ఖాళీ;
          libcrun_error_t tmp_err;
          పూర్ణాంకం tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_మూలం, id, &tmp_err);
          ఒకవేళ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          లేకపోతే
            {
              tmp_ret = crun_మార్గం_exists (statedir, &tmp_err);
              ఒకవేళ (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              లేకపోతే ఒకవేళ (tmp_ret == 0)
                తిరిగి crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      తిరిగి ret;
    }

  ret = parse_json_file (&doc, బఫరు, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  {
    tmp = json_object_object_get (doc, "pid");
    ఒకవేళ (UNLIKELY (tmp == ఖాళీ))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        వెళ్ళు నిష్క్రమించు;
      }
    ఒకవేళ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        వెళ్ళు నిష్క్రమించు;
      }
    స్థితి_2->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    ఒకవేళ (UNLIKELY (tmp == ఖాళీ))
      స్థితి_2->process_ప్రారంభం_సమయం = 0; /* backwards compatibility */
    లేకపోతే ఒకవేళ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        వెళ్ళు నిష్క్రమించు;
      }
    లేకపోతే
      స్థితి_2->process_ప్రారంభం_సమయం = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    ఒకవేళ (UNLIKELY (tmp == ఖాళీ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        వెళ్ళు నిష్క్రమించు;
      }
    val = json_object_get_string (tmp);
    ఒకవేళ (UNLIKELY (val == ఖాళీ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        వెళ్ళు నిష్క్రమించు;
      }
    స్థితి_2->cgroup_మార్గం = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : ఖాళీ;
    స్థితి_2->scope = val ? xstrdup (val) : ఖాళీ;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    ఒకవేళ (UNLIKELY (tmp == ఖాళీ))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        వెళ్ళు నిష్క్రమించు;
      }
    val = json_object_get_string (tmp);
    ఒకవేళ (UNLIKELY (val == ఖాళీ))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        వెళ్ళు నిష్క్రమించు;
      }
    స్థితి_2->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    స్థితి_2->systemd_cgroup = tmp != ఖాళీ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    ఒకవేళ (UNLIKELY (tmp == ఖాళీ))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        వెళ్ళు నిష్క్రమించు;
      }
    val = json_object_get_string (tmp);
    ఒకవేళ (UNLIKELY (val == ఖాళీ))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        వెళ్ళు నిష్క్రమించు;
      }
    స్థితి_2->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    ఒకవేళ (UNLIKELY (tmp == ఖాళీ))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        వెళ్ళు నిష్క్రమించు;
      }
    val = json_object_get_string (tmp);
    ఒకవేళ (UNLIKELY (val == ఖాళీ))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        వెళ్ళు నిష్క్రమించు;
      }
    స్థితి_2->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : ఖాళీ;
    స్థితి_2->owner = val ? xstrdup (val) : ఖాళీ;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    స్థితి_2->detached = tmp != ఖాళీ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    స్థితి_2->no_new_keyring = tmp != ఖాళీ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : ఖాళీ;
    స్థితి_2->external_descriptors = val ? xstrdup (val) : ఖాళీ;
  }
  ret = 0;

నిష్క్రమించు:
  json_object_put (doc);
  తిరిగి ret;
}

పూర్ణాంకం
libcrun_స్థితి_check_directories (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  cleanup_free అక్షరం *dir = ఖాళీ;
  పూర్ణాంకం ret;

  ret = libcrun_get_state_directory (&dir, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = crun_మార్గం_exists (dir, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (ret)
    తిరిగి crun_make_error (err, 0, "container `%s` already exists", id);

  ఒకవేళ (UNLIKELY (mkdir (dir, 0700) < 0))
    తిరిగి crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  తిరిగి 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#నిర్వచించు RMDIRFD_MAX_కదలికలు (1U << 20)

స్థిర పూర్ణాంకం
rmdirfd_move_గమ్యం (DIR **d, స్థిరరాశి అక్షరం *name, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  ఒకవేళ (UNLIKELY (new_d == ఖాళీ))
    తిరిగి crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
rmdirfd (స్థిరరాశి అక్షరం *namedir, పూర్ణాంకం fd, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_dir DIR *d = ఖాళీ;
  నిర్మాణం dirent *de;
  నిర్మాణం stat మూలం_st;
  bool is_మూలం = true;
  గుర్తులేని పూర్ణాంకం budget = RMDIRFD_MAX_కదలికలు;
  __attribute__ ((వాడని)) cleanup_close పూర్ణాంకం fd_cleanup = fd;

  ret = fstat (fd, &మూలం_st);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  ఒకవేళ (d == ఖాళీ)
    తిరిగి crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  కొరకు (;;)
    {
      errno = 0;
      de = readdir (d);
      ఒకవేళ (de == ఖాళీ)
        {
          నిర్మాణం stat st;

          ఒకవేళ (UNLIKELY (errno != 0))
            తిరిగి crun_make_error (err, errno, "cannot read directory");

          ఒకవేళ (is_మూలం)
            ఆపు;

          ఒకవేళ (UNLIKELY (--budget == 0))
            తిరిగి crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_గమ్యం (&d, "..", err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          ret = fstat (dirfd (d), &st);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "stat `..`");

          is_మూలం = st.st_dev == మూలం_st.st_dev && st.st_ino == మూలం_st.st_ino;
          కొనసాగు;
        }

      ఒకవేళ ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        కొనసాగు;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      ఒకవేళ (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          ఒకవేళ (ret < 0 && errno == EBUSY)
            {
              cleanup_close పూర్ణాంకం tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_మార్గం | O_NOFOLLOW);
              ఒకవేళ (tfd >= 0)
                {
                  proc_fd_మార్గం_t procpath;

                  get_proc_self_fd_మార్గం (procpath, tfd);
                  ఒకవేళ (umount2 (procpath, MNT_DETACH) == 0)
                    వెళ్ళు retry_unlink;
                }
              ఒకవేళ (! is_మూలం)
                తిరిగి crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          లేకపోతే ఒకవేళ (ret < 0 && errno == ENOTEMPTY)
            {
              ఒకవేళ (UNLIKELY (--budget == 0))
                తిరిగి crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_గమ్యం (&d, de->d_name, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;

              is_మూలం = false;
            }
          లేకపోతే ఒకవేళ (ret < 0 && errno != ENOENT && ! is_మూలం)
            {
              తిరిగి crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_container_delete_స్థితి (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_close పూర్ణాంకం rundir_dfd = -1;
  cleanup_close పూర్ణాంకం dfd = -1;
  cleanup_free అక్షరం *dir = ఖాళీ;

  ret = validate_id (id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = get_నిరంతరం_directory (&dir, state_మూలం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  rundir_dfd = తాత్కాలిక_FAILURE_RETRY (open (dir, O_DIRECTORY | O_మార్గం | O_CLOEXEC));
  ఒకవేళ (UNLIKELY (rundir_dfd < 0))
    తిరిగి crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (dfd < 0))
    తిరిగి crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  తిరిగి 0;
}

శూన్య
libcrun_free_container_స్థితి (libcrun_container_స్థితి_t *స్థితి_2)
{
  ఒకవేళ (స్థితి_2 == ఖాళీ)
    తిరిగి;
  విడుదల (స్థితి_2->cgroup_మార్గం);
  విడుదల (స్థితి_2->bundle);
  విడుదల (స్థితి_2->rootfs);
  విడుదల (స్థితి_2->external_descriptors);
  విడుదల (స్థితి_2->created);
  విడుదల (స్థితి_2->scope);
  విడుదల (స్థితి_2->owner);
}

పూర్ణాంకం
libcrun_get_containers_list (libcrun_container_list_t **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, libcrun_error_t *err)
{
  నిర్మాణం dirent *తదుపరి;
  cleanup_container_list libcrun_container_list_t *tmp = ఖాళీ;
  cleanup_free అక్షరం *మూలం = ఖాళీ;
  cleanup_dir DIR *dir = ఖాళీ;
  పూర్ణాంకం ret;

  *బహిర్గతం = ఖాళీ;

  ret = get_నిరంతరం_directory (&మూలం, state_మూలం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  dir = opendir (మూలం);
  ఒకవేళ (UNLIKELY (dir == ఖాళీ))
    తిరిగి crun_make_error (err, errno, "cannot opendir `%s`", మూలం);

  కొరకు (తదుపరి = readdir (dir); తదుపరి; తదుపరి = readdir (dir))
    {
      పూర్ణాంకం exists;
      cleanup_free అక్షరం *స్థితి_file = ఖాళీ;

      libcrun_container_list_t *తదుపరి_container;

      ఒకవేళ (తదుపరి->d_name[0] == '.')
        కొనసాగు;

      ret = append_paths (&స్థితి_file, err, మూలం, తదుపరి->d_name, "status", ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      exists = crun_మార్గం_exists (స్థితి_file, err);
      ఒకవేళ (exists < 0)
        {
          తిరిగి exists;
        }

      ఒకవేళ (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", స్థితి_file);
          కొనసాగు;
        }

      తదుపరి_container = xmalloc (పరిమాణం (libcrun_container_list_t));
      తదుపరి_container->name = xstrdup (తదుపరి->d_name);
      తదుపరి_container->తదుపరి = tmp;
      tmp = తదుపరి_container;
    }

  STEAL_సూచిక (బహిర్గతం, tmp);

  తిరిగి 0;
}

శూన్య
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *తదుపరి;
  అయితే (list)
    {
      తదుపరి = list->తదుపరి;
      విడుదల (list->name);
      విడుదల (list);
      list = తదుపరి;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
పూర్ణాంకం
libcrun_check_pid_valid (libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err)
{
  నిర్మాణం pid_stat st;
  పూర్ణాంకం ret;

  /* For backwards compatibility, check start time only if available. */
  ఒకవేళ (! స్థితి_2->process_ప్రారంభం_సమయం)
    తిరిగి 1;

  ret = read_pid_stat (స్థితి_2->pid, &st, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (స్థితి_2->process_ప్రారంభం_సమయం != st.starttime || st.state == 'X')
    తిరిగి 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  ఒకవేళ (st.state == 'Z' && st.అంకె_దారాలు <= 1)
    తిరిగి 0; /* stopped */

  తిరిగి 1; /* running, created, or paused */
}

పూర్ణాంకం
libcrun_is_container_running (libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = kill (స్థితి_2->pid, 0);
  ఒకవేళ (UNLIKELY (ret < 0) && errno != ESRCH)
    తిరిగి crun_make_error (err, errno, "kill");

  ఒకవేళ (ret == 0)
    తిరిగి libcrun_check_pid_valid (స్థితి_2, err);

  తిరిగి 0; /* stopped */
}

స్థిర పూర్ణాంకం
get_exec_fifo_మార్గం (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  cleanup_free అక్షరం *state_dir = ఖాళీ;
  పూర్ణాంకం ret;

  ret = libcrun_get_state_directory (&state_dir, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి append_paths (బహిర్గతం, err, state_dir, "exec.fifo", ఖాళీ);
}

పూర్ణాంకం
libcrun_స్థితి_create_exec_fifo (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  cleanup_free అక్షరం *fifo_మార్గం = ఖాళీ;
  పూర్ణాంకం ret, fd = -1;

  ret = get_exec_fifo_మార్గం (&fifo_మార్గం, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_మార్గం);
  ret = mkfifo (fifo_మార్గం, 0600);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_మార్గం, O_NONBLOCK | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "cannot open pipe `%s`", fifo_మార్గం);

  తిరిగి fd;
}

పూర్ణాంకం
libcrun_స్థితి_write_exec_fifo (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  cleanup_free అక్షరం *fifo_మార్గం = ఖాళీ;
  అక్షరం బఫరు[1] = {
    0,
  };
  cleanup_close పూర్ణాంకం fd = -1;
  పూర్ణాంకం ret;

  ret = get_exec_fifo_మార్గం (&fifo_మార్గం, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  fd = open (fifo_మార్గం, O_WRONLY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "cannot open `%s`", fifo_మార్గం);

  ret = unlink (fifo_మార్గం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "unlink `%s`", fifo_మార్గం);

  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, బఫరు, 1));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "write to exec.fifo");

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_స్థితి_has_read_exec_fifo (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  cleanup_free అక్షరం *fifo_మార్గం = ఖాళీ;
  పూర్ణాంకం ret;

  ret = get_exec_fifo_మార్గం (&fifo_మార్గం, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి crun_మార్గం_exists (fifo_మార్గం, err);
}

నిర్మాణం libcrun_స్థితి_s
{
  libcrun_container_స్థితి_t స్థితి_2;
  అక్షరం *id;
  అక్షరం *state_మూలం;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  పూర్ణాంకం running;
};

పూర్ణాంకం
libcrun_container_స్థితి_ఎక్కించు (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, libcrun_స్థితి_t **బహిర్గతం, libcrun_error_t *err)
{
  libcrun_స్థితి_t *st;
  పూర్ణాంకం ret;

  st = xmalloc0 (పరిమాణం (*st));

  ret = libcrun_read_container_స్థితి (&st->స్థితి_2, context->state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      విడుదల (st);
      తిరిగి ret;
    }

  st->id = xstrdup (id);
  st->state_మూలం = xstrdup (context->state_మూలం);

  *బహిర్గతం = st;
  తిరిగి 0;
}

శూన్య
libcrun_container_స్థితి_free (libcrun_స్థితి_t *st)
{
  ఒకవేళ (st == ఖాళీ)
    తిరిగి;
  libcrun_free_container_స్థితి (&st->స్థితి_2);
  విడుదల (st->state_మూలం);
  విడుదల (st->id);
  విడుదల (st);
}

పూర్ణాంకం
libcrun_స్థితి_get_state (libcrun_స్థితి_t *st, libcrun_container_state_t *state, పూర్ణాంకం *running, libcrun_error_t *err)
{
  ఒకవేళ (! st->state_resolved)
    {
      స్థిరరాశి అక్షరం *state_string = ఖాళీ;
      పూర్ణాంకం ret;

      ret = libcrun_get_container_state_string (st->id, &st->స్థితి_2, st->state_మూలం, &state_string, &st->running, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (state_string == ఖాళీ)
        st->state = LIBCRUN_CONTAINER_స్థితి_STOPPED;
      లేకపోతే ఒకవేళ (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_స్థితి_CREATING;
      లేకపోతే ఒకవేళ (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_స్థితి_CREATED;
      లేకపోతే ఒకవేళ (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_స్థితి_RUNNING;
      లేకపోతే ఒకవేళ (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_స్థితి_PAUSED;
      లేకపోతే
        st->state = LIBCRUN_CONTAINER_స్థితి_STOPPED;

      st->state_resolved = true;
    }

  ఒకవేళ (state)
    *state = st->state;
  ఒకవేళ (running)
    *running = st->running;
  తిరిగి 0;
}

స్థిరరాశి అక్షరం *
libcrun_container_state_గమ్యం_string (libcrun_container_state_t state)
{
  ఎంపిక (state)
    {
    స్థితి LIBCRUN_CONTAINER_స్థితి_CREATING:
      తిరిగి "creating";
    స్థితి LIBCRUN_CONTAINER_స్థితి_CREATED:
      తిరిగి "created";
    స్థితి LIBCRUN_CONTAINER_స్థితి_RUNNING:
      తిరిగి "running";
    స్థితి LIBCRUN_CONTAINER_స్థితి_PAUSED:
      తిరిగి "paused";
    స్థితి LIBCRUN_CONTAINER_స్థితి_STOPPED:
      తిరిగి "stopped";
    }
  తిరిగి "unknown";
}

pid_t
libcrun_స్థితి_get_pid (libcrun_స్థితి_t *st)
{
  తిరిగి st->స్థితి_2.pid;
}

స్థిరరాశి అక్షరం *
libcrun_స్థితి_get_bundle (libcrun_స్థితి_t *st)
{
  తిరిగి st->స్థితి_2.bundle;
}

స్థిరరాశి అక్షరం *
libcrun_స్థితి_get_rootfs (libcrun_స్థితి_t *st)
{
  తిరిగి st->స్థితి_2.rootfs;
}

స్థిరరాశి అక్షరం *
libcrun_స్థితి_get_created (libcrun_స్థితి_t *st)
{
  తిరిగి st->స్థితి_2.created;
}

స్థిరరాశి అక్షరం *
libcrun_స్థితి_get_owner (libcrun_స్థితి_t *st)
{
  తిరిగి st->స్థితి_2.owner;
}

స్థిరరాశి అక్షరం *
libcrun_స్థితి_get_scope (libcrun_స్థితి_t *st)
{
  తిరిగి st->స్థితి_2.scope;
}

స్థిరరాశి అక్షరం *
libcrun_స్థితి_get_external_descriptors (libcrun_స్థితి_t *st)
{
  తిరిగి st->స్థితి_2.external_descriptors;
}

పూర్ణాంకం
libcrun_container_get_state_string (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం **బహిర్గతం, పూర్ణాంకం *running,
                                    libcrun_error_t *err)
{
  cleanup_container_స్థితి libcrun_container_స్థితి_t స్థితి_2 = {};
  పూర్ణాంకం local_running;
  పూర్ణాంకం ret;

  ret = libcrun_read_container_స్థితి (&స్థితి_2, context->state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_get_container_state_string (id, &స్థితి_2, context->state_మూలం, బహిర్గతం, &local_running, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (running)
    *running = local_running;
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_get_state_dir (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  తిరిగి libcrun_get_state_directory (బహిర్గతం, state_మూలం, id, err);
}

పూర్ణాంకం
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **బహిర్గతం, libcrun_error_t *err)
{
  తిరిగి libcrun_get_containers_list (బహిర్గతం, context->state_మూలం, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (పరిమాణం (*it));
  it->cur = l;
  తిరిగి it;
}

bool
libcrun_container_iter_తదుపరి (libcrun_container_iter_t *it, స్థిరరాశి అక్షరం **id)
{
  ఒకవేళ (it == ఖాళీ || it->cur == ఖాళీ)
    తిరిగి false;

  ఒకవేళ (id)
    *id = it->cur->name;
  it->cur = it->cur->తదుపరి;
  తిరిగి true;
}

శూన్య
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  విడుదల (it);
}

శూన్య
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
