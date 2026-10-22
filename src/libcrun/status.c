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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "status.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "json_gen_utils.h"
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <json-c/json.h>
#അന്തര്ഭാവ <ocispec/json_common.h>
#അന്തര്ഭാവ <sys/mount.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <dirent.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <limits.h>

#define STEAL_POINTER(x, y) \
  ചെയ്യുക                        \
    {                       \
      *x = y;               \
      y = ശൂന്യ;             \
  } ആകുമ്പോൾ (0)

ഘടന pid_stat
{
  അക്ഷരം state;
  long അക്കം_threads;
  unsigned long long starttime;
};

സ്ഥിതം വരിയിൽ bool
is_valid_id_char (അക്ഷരം c)
{
  എങ്കിൽ ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    തിരികെ true;
  തിരികെ c == '_' || c == '+' || c == '.' || c == '-';
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
validate_id (സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  എങ്കിൽ (id == ശൂന്യ)
    തിരികെ 0;

  എങ്കിൽ (id[0] == '\0')
    തിരികെ crun_make_error (err, 0, "empty container ID");

  എങ്കിൽ (id[0] == '.')
    തിരികെ crun_make_error (err, 0, "invalid container ID `%s`", id);

  വേണ്ടി (size_t i = 0; id[i]; i++)
    {
      എങ്കിൽ (! is_valid_id_char (id[i]))
        തിരികെ crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
get_run_directory (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *state_വേര്, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free അക്ഷരം *വേര് = ശൂന്യ;

  എങ്കിൽ (state_വേര്)
    വേര് = xstrdup (state_വേര്);
  എങ്കിൽ (വേര് == ശൂന്യ)
    {
      സ്ഥിരം അക്ഷരം *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      എങ്കിൽ (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&വേര്, err, runtime_dir, "crun", ശൂന്യ);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }
  എങ്കിൽ (വേര് == ശൂന്യ)
    വേര് = xstrdup ("/run/crun");

  ret = crun_ensure_directory (വേര്, 0700, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  STEAL_POINTER (out, വേര്);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
get_shared_empty_directory_path (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *state_വേര്, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *run_dir = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = get_run_directory (&run_dir, state_വേര്, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_get_state_directory (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free അക്ഷരം *path = ശൂന്യ;
  cleanup_free അക്ഷരം *വേര് = ശൂന്യ;

  ret = validate_id (id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = get_run_directory (&വേര്, state_വേര്, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_paths (&path, err, വേര്, id, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  STEAL_POINTER (out, path);

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_state_directory_status_file (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *dir = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_get_state_directory (&dir, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ append_paths (out, err, dir, "status", ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_pid_stat (pid_t pid, ഘടന pid_stat *st, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *buffer = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  അക്ഷരം pid_stat_file[64];
  അക്ഷരം *it, *s;
  പൂർണ്ണസംഖ്യ i, ret;

  ret = snprintf (pid_stat_file, വലുപ്പം (pid_stat_file), "/proc/%d/stat", pid);
  എങ്കിൽ (UNLIKELY (ret >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (pid_stat_file)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  എങ്കിൽ (fd < 0)
    {
      /* The process already exited.  */
      എങ്കിൽ (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, വലുപ്പം (*st));
          തിരികെ 0;
        }
      തിരികെ crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, ശൂന്യ, err);
  എങ്കിൽ (ret < 0)
    {
      st->starttime = 0;
      st->അക്കം_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      തിരികെ 0;
    }

  s = ശൂന്യ;

  /* Skip the first two arguments.  */
  വേണ്ടി (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  എങ്കിൽ (s)
    ആകുമ്പോൾ (*s == ' ')
      s++;

  എങ്കിൽ (s == ശൂന്യ || *s == '\0')
    തിരികെ crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  വേണ്ടി (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      എങ്കിൽ (it == ശൂന്യ)
        തിരികെ crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      എങ്കിൽ (i == 16)
        st->അക്കം_threads = strtol (it, ശൂന്യ, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, ശൂന്യ, 10);
  എങ്കിൽ (errno != 0)
    തിരികെ crun_make_error (err, errno, "parse process start time");

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_write_container_status (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ r, ret;
  cleanup_free അക്ഷരം *file_tmp = ശൂന്യ;
  cleanup_free അക്ഷരം *file = ശൂന്യ;
  size_t len;
  cleanup_close പൂർണ്ണസംഖ്യ fd_write = -1;
  സ്ഥിരം അക്ഷരം *buf = ശൂന്യ;
  ഘടന pid_stat st;
  സ്ഥിരം അക്ഷരം *tmp;
  cleanup_json_gen json_gen_ctx *gen = ശൂന്യ;

  ret = get_state_directory_status_file (&file, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = read_pid_stat (status->pid, &st, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  എങ്കിൽ (UNLIKELY (fd_write < 0))
    തിരികെ crun_make_error (err, errno, "cannot open status file");

  എങ്കിൽ (! json_gen_init (&gen, ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_താക്കോൽ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_താക്കോൽ (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_താക്കോൽ (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_താക്കോൽ (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_താക്കോൽ (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_താക്കോൽ (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_താക്കോൽ (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_താക്കോൽ (gen, "created");
  GEN_STR (gen, status->created);

  എങ്കിൽ (status->owner)
    {
      GEN_താക്കോൽ (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_താക്കോൽ (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_താക്കോൽ (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  എങ്കിൽ (status->external_descriptors)
    {
      GEN_താക്കോൽ (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  close_and_reset (&fd_write);

  എങ്കിൽ (UNLIKELY (rename (file_tmp, file) < 0))
    തിരികെ crun_make_error (err, errno, "cannot rename status file");

  തിരികെ 0;

gen_error:
  തിരികെ json_gen_error_to_crun_error (r, err);
}

പൂർണ്ണസംഖ്യ
libcrun_read_container_status (libcrun_container_status_t *status, സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id,
                               libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *buffer = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free അക്ഷരം *file = ശൂന്യ;
  json_object *doc = ശൂന്യ;
  json_object *tmp;
  സ്ഥിരം അക്ഷരം *val;

  ret = get_state_directory_status_file (&file, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = read_all_file (file, &buffer, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {

      എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free അക്ഷരം *statedir = ശൂന്യ;
          libcrun_error_t tmp_err;
          പൂർണ്ണസംഖ്യ tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_വേര്, id, &tmp_err);
          എങ്കിൽ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          അല്ലെങ്കിൽ
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              എങ്കിൽ (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              അല്ലെങ്കിൽ എങ്കിൽ (tmp_ret == 0)
                തിരികെ crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      തിരികെ ret;
    }

  ret = parse_json_file (&doc, buffer, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  {
    tmp = json_object_object_get (doc, "pid");
    എങ്കിൽ (UNLIKELY (tmp == ശൂന്യ))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        പോകുക exit;
      }
    എങ്കിൽ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        പോകുക exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    എങ്കിൽ (UNLIKELY (tmp == ശൂന്യ))
      status->process_start_time = 0; /* backwards compatibility */
    അല്ലെങ്കിൽ എങ്കിൽ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        പോകുക exit;
      }
    അല്ലെങ്കിൽ
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    എങ്കിൽ (UNLIKELY (tmp == ശൂന്യ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        പോകുക exit;
      }
    val = json_object_get_string (tmp);
    എങ്കിൽ (UNLIKELY (val == ശൂന്യ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        പോകുക exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : ശൂന്യ;
    status->scope = val ? xstrdup (val) : ശൂന്യ;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    എങ്കിൽ (UNLIKELY (tmp == ശൂന്യ))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        പോകുക exit;
      }
    val = json_object_get_string (tmp);
    എങ്കിൽ (UNLIKELY (val == ശൂന്യ))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        പോകുക exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != ശൂന്യ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    എങ്കിൽ (UNLIKELY (tmp == ശൂന്യ))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        പോകുക exit;
      }
    val = json_object_get_string (tmp);
    എങ്കിൽ (UNLIKELY (val == ശൂന്യ))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        പോകുക exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    എങ്കിൽ (UNLIKELY (tmp == ശൂന്യ))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        പോകുക exit;
      }
    val = json_object_get_string (tmp);
    എങ്കിൽ (UNLIKELY (val == ശൂന്യ))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        പോകുക exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : ശൂന്യ;
    status->owner = val ? xstrdup (val) : ശൂന്യ;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != ശൂന്യ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != ശൂന്യ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : ശൂന്യ;
    status->external_descriptors = val ? xstrdup (val) : ശൂന്യ;
  }
  ret = 0;

exit:
  json_object_put (doc);
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_status_check_directories (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *dir = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_get_state_directory (&dir, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = crun_path_exists (dir, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (ret)
    തിരികെ crun_make_error (err, 0, "container `%s` already exists", id);

  എങ്കിൽ (UNLIKELY (mkdir (dir, 0700) < 0))
    തിരികെ crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  തിരികെ 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

സ്ഥിതം പൂർണ്ണസംഖ്യ
rmdirfd_move_to (DIR **d, സ്ഥിരം അക്ഷരം *name, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  എങ്കിൽ (UNLIKELY (new_d == ശൂന്യ))
    തിരികെ crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
rmdirfd (സ്ഥിരം അക്ഷരം *namedir, പൂർണ്ണസംഖ്യ fd, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_dir DIR *d = ശൂന്യ;
  ഘടന dirent *de;
  ഘടന stat വേര്_st;
  bool is_വേര് = true;
  unsigned പൂർണ്ണസംഖ്യ budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close പൂർണ്ണസംഖ്യ fd_cleanup = fd;

  ret = fstat (fd, &വേര്_st);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  എങ്കിൽ (d == ശൂന്യ)
    തിരികെ crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  വേണ്ടി (;;)
    {
      errno = 0;
      de = readdir (d);
      എങ്കിൽ (de == ശൂന്യ)
        {
          ഘടന stat st;

          എങ്കിൽ (UNLIKELY (errno != 0))
            തിരികെ crun_make_error (err, errno, "cannot read directory");

          എങ്കിൽ (is_വേര്)
            നിർത്തുക;

          എങ്കിൽ (UNLIKELY (--budget == 0))
            തിരികെ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          ret = fstat (dirfd (d), &st);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "stat `..`");

          is_വേര് = st.st_dev == വേര്_st.st_dev && st.st_ino == വേര്_st.st_ino;
          തുടരുക;
        }

      എങ്കിൽ ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        തുടരുക;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      എങ്കിൽ (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          എങ്കിൽ (ret < 0 && errno == EBUSY)
            {
              cleanup_close പൂർണ്ണസംഖ്യ tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              എങ്കിൽ (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  എങ്കിൽ (umount2 (procpath, MNT_DETACH) == 0)
                    പോകുക retry_unlink;
                }
              എങ്കിൽ (! is_വേര്)
                തിരികെ crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          അല്ലെങ്കിൽ എങ്കിൽ (ret < 0 && errno == ENOTEMPTY)
            {
              എങ്കിൽ (UNLIKELY (--budget == 0))
                തിരികെ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;

              is_വേര് = false;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (ret < 0 && errno != ENOENT && ! is_വേര്)
            {
              തിരികെ crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_container_delete_status (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_close പൂർണ്ണസംഖ്യ rundir_dfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ dfd = -1;
  cleanup_free അക്ഷരം *dir = ശൂന്യ;

  ret = validate_id (id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = get_run_directory (&dir, state_വേര്, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  എങ്കിൽ (UNLIKELY (rundir_dfd < 0))
    തിരികെ crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (dfd < 0))
    തിരികെ crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  തിരികെ 0;
}

ശൂന്യം
libcrun_free_container_status (libcrun_container_status_t *status)
{
  എങ്കിൽ (status == ശൂന്യ)
    തിരികെ;
  സ്വതന്ത്രം (status->cgroup_path);
  സ്വതന്ത്രം (status->bundle);
  സ്വതന്ത്രം (status->rootfs);
  സ്വതന്ത്രം (status->external_descriptors);
  സ്വതന്ത്രം (status->created);
  സ്വതന്ത്രം (status->scope);
  സ്വതന്ത്രം (status->owner);
}

പൂർണ്ണസംഖ്യ
libcrun_get_containers_list (libcrun_container_list_t **out, സ്ഥിരം അക്ഷരം *state_വേര്, libcrun_error_t *err)
{
  ഘടന dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = ശൂന്യ;
  cleanup_free അക്ഷരം *വേര് = ശൂന്യ;
  cleanup_dir DIR *dir = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  *out = ശൂന്യ;

  ret = get_run_directory (&വേര്, state_വേര്, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  dir = opendir (വേര്);
  എങ്കിൽ (UNLIKELY (dir == ശൂന്യ))
    തിരികെ crun_make_error (err, errno, "cannot opendir `%s`", വേര്);

  വേണ്ടി (next = readdir (dir); next; next = readdir (dir))
    {
      പൂർണ്ണസംഖ്യ exists;
      cleanup_free അക്ഷരം *status_file = ശൂന്യ;

      libcrun_container_list_t *next_container;

      എങ്കിൽ (next->d_name[0] == '.')
        തുടരുക;

      ret = append_paths (&status_file, err, വേര്, next->d_name, "status", ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      exists = crun_path_exists (status_file, err);
      എങ്കിൽ (exists < 0)
        {
          തിരികെ exists;
        }

      എങ്കിൽ (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          തുടരുക;
        }

      next_container = xmalloc (വലുപ്പം (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  തിരികെ 0;
}

ശൂന്യം
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  ആകുമ്പോൾ (list)
    {
      next = list->next;
      സ്വതന്ത്രം (list->name);
      സ്വതന്ത്രം (list);
      list = next;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
പൂർണ്ണസംഖ്യ
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ഘടന pid_stat st;
  പൂർണ്ണസംഖ്യ ret;

  /* For backwards compatibility, check start time only if available. */
  എങ്കിൽ (! status->process_start_time)
    തിരികെ 1;

  ret = read_pid_stat (status->pid, &st, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (status->process_start_time != st.starttime || st.state == 'X')
    തിരികെ 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  എങ്കിൽ (st.state == 'Z' && st.അക്കം_threads <= 1)
    തിരികെ 0; /* stopped */

  തിരികെ 1; /* running, created, or paused */
}

പൂർണ്ണസംഖ്യ
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = kill (status->pid, 0);
  എങ്കിൽ (UNLIKELY (ret < 0) && errno != ESRCH)
    തിരികെ crun_make_error (err, errno, "kill");

  എങ്കിൽ (ret == 0)
    തിരികെ libcrun_check_pid_valid (status, err);

  തിരികെ 0; /* stopped */
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_exec_fifo_path (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *state_dir = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_get_state_directory (&state_dir, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ append_paths (out, err, state_dir, "exec.fifo", ശൂന്യ);
}

പൂർണ്ണസംഖ്യ
libcrun_status_create_exec_fifo (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *fifo_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  തിരികെ fd;
}

പൂർണ്ണസംഖ്യ
libcrun_status_write_exec_fifo (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *fifo_path = ശൂന്യ;
  അക്ഷരം buffer[1] = {
    0,
  };
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  പൂർണ്ണസംഖ്യ ret;

  ret = get_exec_fifo_path (&fifo_path, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (ലിഖ (fd, buffer, 1));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "write to exec.fifo");

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_status_has_read_exec_fifo (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *fifo_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = get_exec_fifo_path (&fifo_path, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ crun_path_exists (fifo_path, err);
}

ഘടന libcrun_status_s
{
  libcrun_container_status_t status;
  അക്ഷരം *id;
  അക്ഷരം *state_വേര്;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  പൂർണ്ണസംഖ്യ running;
};

പൂർണ്ണസംഖ്യ
libcrun_container_status_load (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  പൂർണ്ണസംഖ്യ ret;

  st = xmalloc0 (വലുപ്പം (*st));

  ret = libcrun_read_container_status (&st->status, context->state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      സ്വതന്ത്രം (st);
      തിരികെ ret;
    }

  st->id = xstrdup (id);
  st->state_വേര് = xstrdup (context->state_വേര്);

  *out = st;
  തിരികെ 0;
}

ശൂന്യം
libcrun_container_status_free (libcrun_status_t *st)
{
  എങ്കിൽ (st == ശൂന്യ)
    തിരികെ;
  libcrun_free_container_status (&st->status);
  സ്വതന്ത്രം (st->state_വേര്);
  സ്വതന്ത്രം (st->id);
  സ്വതന്ത്രം (st);
}

പൂർണ്ണസംഖ്യ
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, പൂർണ്ണസംഖ്യ *running, libcrun_error_t *err)
{
  എങ്കിൽ (! st->state_resolved)
    {
      സ്ഥിരം അക്ഷരം *state_string = ശൂന്യ;
      പൂർണ്ണസംഖ്യ ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_വേര്, &state_string, &st->running, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      എങ്കിൽ (state_string == ശൂന്യ)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      അല്ലെങ്കിൽ
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  എങ്കിൽ (state)
    *state = st->state;
  എങ്കിൽ (running)
    *running = st->running;
  തിരികെ 0;
}

സ്ഥിരം അക്ഷരം *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  തിരഞ്ഞെടുപ്പ് (state)
    {
    അവസ്ഥ LIBCRUN_CONTAINER_STATUS_CREATING:
      തിരികെ "creating";
    അവസ്ഥ LIBCRUN_CONTAINER_STATUS_CREATED:
      തിരികെ "created";
    അവസ്ഥ LIBCRUN_CONTAINER_STATUS_RUNNING:
      തിരികെ "running";
    അവസ്ഥ LIBCRUN_CONTAINER_STATUS_PAUSED:
      തിരികെ "paused";
    അവസ്ഥ LIBCRUN_CONTAINER_STATUS_STOPPED:
      തിരികെ "stopped";
    }
  തിരികെ "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  തിരികെ st->status.pid;
}

സ്ഥിരം അക്ഷരം *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  തിരികെ st->status.bundle;
}

സ്ഥിരം അക്ഷരം *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  തിരികെ st->status.rootfs;
}

സ്ഥിരം അക്ഷരം *
libcrun_status_get_created (libcrun_status_t *st)
{
  തിരികെ st->status.created;
}

സ്ഥിരം അക്ഷരം *
libcrun_status_get_owner (libcrun_status_t *st)
{
  തിരികെ st->status.owner;
}

സ്ഥിരം അക്ഷരം *
libcrun_status_get_scope (libcrun_status_t *st)
{
  തിരികെ st->status.scope;
}

സ്ഥിരം അക്ഷരം *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  തിരികെ st->status.external_descriptors;
}

പൂർണ്ണസംഖ്യ
libcrun_container_get_state_string (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം **out, പൂർണ്ണസംഖ്യ *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  പൂർണ്ണസംഖ്യ local_running;
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_read_container_status (&status, context->state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_വേര്, out, &local_running, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (running)
    *running = local_running;
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_get_state_dir (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  തിരികെ libcrun_get_state_directory (out, state_വേര്, id, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  തിരികെ libcrun_get_containers_list (out, context->state_വേര്, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (വലുപ്പം (*it));
  it->cur = l;
  തിരികെ it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, സ്ഥിരം അക്ഷരം **id)
{
  എങ്കിൽ (it == ശൂന്യ || it->cur == ശൂന്യ)
    തിരികെ false;

  എങ്കിൽ (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  തിരികെ true;
}

ശൂന്യം
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  സ്വതന്ത്രം (it);
}

ശൂന്യം
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
