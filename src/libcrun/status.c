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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "json_gen_utils.h"
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <json-c/json.h>
#ಅನ್ತರ್ಭಾವ <ocispec/json_common.h>
#ಅನ್ತರ್ಭಾವ <sys/mount.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <dirent.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <limits.h>

#define STEAL_POINTER(x, y) \
  ಕುರು                        \
    {                       \
      *x = y;               \
      y = ರಿಕ್ತ;             \
  } ಯಾವತ್ (0)

ಸಂರಚನಾ pid_stat
{
  ವರ್ಣ state;
  long ಅಂಕೆ_threads;
  unsigned long long starttime;
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_valid_id_char (ವರ್ಣ c)
{
  ಯದಿ ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    ಪ್ರತ್ಯಾಗಮ true;
  ಪ್ರತ್ಯಾಗಮ c == '_' || c == '+' || c == '.' || c == '-';
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
validate_id (ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  ಯದಿ (id == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (id[0] == '\0')
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "empty container ID");

  ಯದಿ (id[0] == '.')
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid container ID `%s`", id);

  ಪ್ರತಿ (size_t i = 0; id[i]; i++)
    {
      ಯದಿ (! is_valid_id_char (id[i]))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
get_run_directory (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free ವರ್ಣ *ಬೇರು = ರಿಕ್ತ;

  ಯದಿ (state_ಬೇರು)
    ಬೇರು = xstrdup (state_ಬೇರು);
  ಯದಿ (ಬೇರು == ರಿಕ್ತ)
    {
      ಧ್ರುವ ವರ್ಣ *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      ಯದಿ (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&ಬೇರು, err, runtime_dir, "crun", ರಿಕ್ತ);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }
  ಯದಿ (ಬೇರು == ರಿಕ್ತ)
    ಬೇರು = xstrdup ("/run/crun");

  ret = crun_ensure_directory (ಬೇರು, 0700, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  STEAL_POINTER (out, ಬೇರು);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
get_shared_empty_directory_path (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *run_dir = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = get_run_directory (&run_dir, state_ಬೇರು, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_get_state_directory (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *ಬೇರು = ರಿಕ್ತ;

  ret = validate_id (id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = get_run_directory (&ಬೇರು, state_ಬೇರು, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_paths (&path, err, ಬೇರು, id, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  STEAL_POINTER (out, path);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_state_directory_status_file (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *dir = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_get_state_directory (&dir, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ append_paths (out, err, dir, "status", ರಿಕ್ತ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_pid_stat (pid_t pid, ಸಂರಚನಾ pid_stat *st, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ವರ್ಣ pid_stat_file[64];
  ವರ್ಣ *it, *s;
  ಪೂರ್ಣಾಙ್ಕ i, ret;

  ret = snprintf (pid_stat_file, ಪರಿಮಾಣ (pid_stat_file), "/proc/%d/stat", pid);
  ಯದಿ (UNLIKELY (ret >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (pid_stat_file)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  ಯದಿ (fd < 0)
    {
      /* The process already exited.  */
      ಯದಿ (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, ಪರಿಮಾಣ (*st));
          ಪ್ರತ್ಯಾಗಮ 0;
        }
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, ರಿಕ್ತ, err);
  ಯದಿ (ret < 0)
    {
      st->starttime = 0;
      st->ಅಂಕೆ_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  s = ರಿಕ್ತ;

  /* Skip the first two arguments.  */
  ಪ್ರತಿ (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  ಯದಿ (s)
    ಯಾವತ್ (*s == ' ')
      s++;

  ಯದಿ (s == ರಿಕ್ತ || *s == '\0')
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  ಪ್ರತಿ (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      ಯದಿ (it == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      ಯದಿ (i == 16)
        st->ಅಂಕೆ_threads = strtol (it, ರಿಕ್ತ, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, ರಿಕ್ತ, 10);
  ಯದಿ (errno != 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "parse process start time");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_write_container_status (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ r, ret;
  cleanup_free ವರ್ಣ *file_tmp = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *file = ರಿಕ್ತ;
  size_t len;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd_write = -1;
  ಧ್ರುವ ವರ್ಣ *buf = ರಿಕ್ತ;
  ಸಂರಚನಾ pid_stat st;
  ಧ್ರುವ ವರ್ಣ *tmp;
  cleanup_json_gen json_gen_ctx *gen = ರಿಕ್ತ;

  ret = get_state_directory_status_file (&file, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = read_pid_stat (status->pid, &st, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  ಯದಿ (UNLIKELY (fd_write < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot open status file");

  ಯದಿ (! json_gen_init (&gen, ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ಕೀಲಿ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_ಕೀಲಿ (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_ಕೀಲಿ (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_ಕೀಲಿ (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_ಕೀಲಿ (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_ಕೀಲಿ (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_ಕೀಲಿ (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_ಕೀಲಿ (gen, "created");
  GEN_STR (gen, status->created);

  ಯದಿ (status->owner)
    {
      GEN_ಕೀಲಿ (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_ಕೀಲಿ (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_ಕೀಲಿ (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  ಯದಿ (status->external_descriptors)
    {
      GEN_ಕೀಲಿ (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  close_and_reset (&fd_write);

  ಯದಿ (UNLIKELY (rename (file_tmp, file) < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot rename status file");

  ಪ್ರತ್ಯಾಗಮ 0;

gen_error:
  ಪ್ರತ್ಯಾಗಮ json_gen_error_to_crun_error (r, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_read_container_status (libcrun_container_status_t *status, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id,
                               libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free ವರ್ಣ *file = ರಿಕ್ತ;
  json_object *doc = ರಿಕ್ತ;
  json_object *tmp;
  ಧ್ರುವ ವರ್ಣ *val;

  ret = get_state_directory_status_file (&file, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = read_all_file (file, &buffer, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {

      ಯದಿ (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free ವರ್ಣ *statedir = ರಿಕ್ತ;
          libcrun_error_t tmp_err;
          ಪೂರ್ಣಾಙ್ಕ tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_ಬೇರು, id, &tmp_err);
          ಯದಿ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          ಅನ್ಯಥಾ
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              ಯದಿ (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              ಅನ್ಯಥಾ ಯದಿ (tmp_ret == 0)
                ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = parse_json_file (&doc, buffer, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  {
    tmp = json_object_object_get (doc, "pid");
    ಯದಿ (UNLIKELY (tmp == ರಿಕ್ತ))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        ಗಚ್ಛ exit;
      }
    ಯದಿ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        ಗಚ್ಛ exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    ಯದಿ (UNLIKELY (tmp == ರಿಕ್ತ))
      status->process_start_time = 0; /* backwards compatibility */
    ಅನ್ಯಥಾ ಯದಿ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        ಗಚ್ಛ exit;
      }
    ಅನ್ಯಥಾ
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    ಯದಿ (UNLIKELY (tmp == ರಿಕ್ತ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        ಗಚ್ಛ exit;
      }
    val = json_object_get_string (tmp);
    ಯದಿ (UNLIKELY (val == ರಿಕ್ತ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        ಗಚ್ಛ exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : ರಿಕ್ತ;
    status->scope = val ? xstrdup (val) : ರಿಕ್ತ;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    ಯದಿ (UNLIKELY (tmp == ರಿಕ್ತ))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        ಗಚ್ಛ exit;
      }
    val = json_object_get_string (tmp);
    ಯದಿ (UNLIKELY (val == ರಿಕ್ತ))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        ಗಚ್ಛ exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != ರಿಕ್ತ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    ಯದಿ (UNLIKELY (tmp == ರಿಕ್ತ))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        ಗಚ್ಛ exit;
      }
    val = json_object_get_string (tmp);
    ಯದಿ (UNLIKELY (val == ರಿಕ್ತ))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        ಗಚ್ಛ exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    ಯದಿ (UNLIKELY (tmp == ರಿಕ್ತ))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        ಗಚ್ಛ exit;
      }
    val = json_object_get_string (tmp);
    ಯದಿ (UNLIKELY (val == ರಿಕ್ತ))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        ಗಚ್ಛ exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : ರಿಕ್ತ;
    status->owner = val ? xstrdup (val) : ರಿಕ್ತ;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != ರಿಕ್ತ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != ರಿಕ್ತ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : ರಿಕ್ತ;
    status->external_descriptors = val ? xstrdup (val) : ರಿಕ್ತ;
  }
  ret = 0;

exit:
  json_object_put (doc);
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_status_check_directories (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *dir = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_get_state_directory (&dir, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = crun_path_exists (dir, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (ret)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "container `%s` already exists", id);

  ಯದಿ (UNLIKELY (mkdir (dir, 0700) < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
rmdirfd_move_to (DIR **d, ಧ್ರುವ ವರ್ಣ *name, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  ಯದಿ (UNLIKELY (new_d == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
rmdirfd (ಧ್ರುವ ವರ್ಣ *namedir, ಪೂರ್ಣಾಙ್ಕ fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_dir DIR *d = ರಿಕ್ತ;
  ಸಂರಚನಾ dirent *de;
  ಸಂರಚನಾ stat ಬೇರು_st;
  bool is_ಬೇರು = true;
  unsigned ಪೂರ್ಣಾಙ್ಕ budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close ಪೂರ್ಣಾಙ್ಕ fd_cleanup = fd;

  ret = fstat (fd, &ಬೇರು_st);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  ಯದಿ (d == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  ಪ್ರತಿ (;;)
    {
      errno = 0;
      de = readdir (d);
      ಯದಿ (de == ರಿಕ್ತ)
        {
          ಸಂರಚನಾ stat st;

          ಯದಿ (UNLIKELY (errno != 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot read directory");

          ಯದಿ (is_ಬೇರು)
            ಭಙ್ಗ;

          ಯದಿ (UNLIKELY (--budget == 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          ret = fstat (dirfd (d), &st);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "stat `..`");

          is_ಬೇರು = st.st_dev == ಬೇರು_st.st_dev && st.st_ino == ಬೇರು_st.st_ino;
          ಅನುವರ್ತ;
        }

      ಯದಿ ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        ಅನುವರ್ತ;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      ಯದಿ (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          ಯದಿ (ret < 0 && errno == EBUSY)
            {
              cleanup_close ಪೂರ್ಣಾಙ್ಕ tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              ಯದಿ (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  ಯದಿ (umount2 (procpath, MNT_DETACH) == 0)
                    ಗಚ್ಛ retry_unlink;
                }
              ಯದಿ (! is_ಬೇರು)
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          ಅನ್ಯಥಾ ಯದಿ (ret < 0 && errno == ENOTEMPTY)
            {
              ಯದಿ (UNLIKELY (--budget == 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;

              is_ಬೇರು = false;
            }
          ಅನ್ಯಥಾ ಯದಿ (ret < 0 && errno != ENOENT && ! is_ಬೇರು)
            {
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_delete_status (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ rundir_dfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dfd = -1;
  cleanup_free ವರ್ಣ *dir = ರಿಕ್ತ;

  ret = validate_id (id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = get_run_directory (&dir, state_ಬೇರು, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  ಯದಿ (UNLIKELY (rundir_dfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (dfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ
libcrun_free_container_status (libcrun_container_status_t *status)
{
  ಯದಿ (status == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;
  ಮುಕ್ತ (status->cgroup_path);
  ಮುಕ್ತ (status->bundle);
  ಮುಕ್ತ (status->rootfs);
  ಮುಕ್ತ (status->external_descriptors);
  ಮುಕ್ತ (status->created);
  ಮುಕ್ತ (status->scope);
  ಮುಕ್ತ (status->owner);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_get_containers_list (libcrun_container_list_t **out, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, libcrun_error_t *err)
{
  ಸಂರಚನಾ dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *ಬೇರು = ರಿಕ್ತ;
  cleanup_dir DIR *dir = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  *out = ರಿಕ್ತ;

  ret = get_run_directory (&ಬೇರು, state_ಬೇರು, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  dir = opendir (ಬೇರು);
  ಯದಿ (UNLIKELY (dir == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot opendir `%s`", ಬೇರು);

  ಪ್ರತಿ (next = readdir (dir); next; next = readdir (dir))
    {
      ಪೂರ್ಣಾಙ್ಕ exists;
      cleanup_free ವರ್ಣ *status_file = ರಿಕ್ತ;

      libcrun_container_list_t *next_container;

      ಯದಿ (next->d_name[0] == '.')
        ಅನುವರ್ತ;

      ret = append_paths (&status_file, err, ಬೇರು, next->d_name, "status", ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      exists = crun_path_exists (status_file, err);
      ಯದಿ (exists < 0)
        {
          ಪ್ರತ್ಯಾಗಮ exists;
        }

      ಯದಿ (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          ಅನುವರ್ತ;
        }

      next_container = xmalloc (ಪರಿಮಾಣ (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  ಯಾವತ್ (list)
    {
      next = list->next;
      ಮುಕ್ತ (list->name);
      ಮುಕ್ತ (list);
      list = next;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
ಪೂರ್ಣಾಙ್ಕ
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ಸಂರಚನಾ pid_stat st;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* For backwards compatibility, check start time only if available. */
  ಯದಿ (! status->process_start_time)
    ಪ್ರತ್ಯಾಗಮ 1;

  ret = read_pid_stat (status->pid, &st, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (status->process_start_time != st.starttime || st.state == 'X')
    ಪ್ರತ್ಯಾಗಮ 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  ಯದಿ (st.state == 'Z' && st.ಅಂಕೆ_threads <= 1)
    ಪ್ರತ್ಯಾಗಮ 0; /* stopped */

  ಪ್ರತ್ಯಾಗಮ 1; /* running, created, or paused */
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = kill (status->pid, 0);
  ಯದಿ (UNLIKELY (ret < 0) && errno != ESRCH)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "kill");

  ಯದಿ (ret == 0)
    ಪ್ರತ್ಯಾಗಮ libcrun_check_pid_valid (status, err);

  ಪ್ರತ್ಯಾಗಮ 0; /* stopped */
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_exec_fifo_path (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *state_dir = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_get_state_directory (&state_dir, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ append_paths (out, err, state_dir, "exec.fifo", ರಿಕ್ತ);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_status_create_exec_fifo (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *fifo_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  ಪ್ರತ್ಯಾಗಮ fd;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_status_write_exec_fifo (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *fifo_path = ರಿಕ್ತ;
  ವರ್ಣ buffer[1] = {
    0,
  };
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = get_exec_fifo_path (&fifo_path, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (ಲಿಖ (fd, buffer, 1));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to exec.fifo");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_status_has_read_exec_fifo (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *fifo_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = get_exec_fifo_path (&fifo_path, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ crun_path_exists (fifo_path, err);
}

ಸಂರಚನಾ libcrun_status_s
{
  libcrun_container_status_t status;
  ವರ್ಣ *id;
  ವರ್ಣ *state_ಬೇರು;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  ಪೂರ್ಣಾಙ್ಕ running;
};

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_status_load (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  ಪೂರ್ಣಾಙ್ಕ ret;

  st = xmalloc0 (ಪರಿಮಾಣ (*st));

  ret = libcrun_read_container_status (&st->status, context->state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಮುಕ್ತ (st);
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  st->id = xstrdup (id);
  st->state_ಬೇರು = xstrdup (context->state_ಬೇರು);

  *out = st;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ
libcrun_container_status_free (libcrun_status_t *st)
{
  ಯದಿ (st == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;
  libcrun_free_container_status (&st->status);
  ಮುಕ್ತ (st->state_ಬೇರು);
  ಮುಕ್ತ (st->id);
  ಮುಕ್ತ (st);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, ಪೂರ್ಣಾಙ್ಕ *running, libcrun_error_t *err)
{
  ಯದಿ (! st->state_resolved)
    {
      ಧ್ರುವ ವರ್ಣ *state_string = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_ಬೇರು, &state_string, &st->running, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (state_string == ರಿಕ್ತ)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      ಅನ್ಯಥಾ
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  ಯದಿ (state)
    *state = st->state;
  ಯದಿ (running)
    *running = st->running;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಧ್ರುವ ವರ್ಣ *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  ವಿಕಲ್ಪ (state)
    {
    ಪಕ್ಷ LIBCRUN_CONTAINER_STATUS_CREATING:
      ಪ್ರತ್ಯಾಗಮ "creating";
    ಪಕ್ಷ LIBCRUN_CONTAINER_STATUS_CREATED:
      ಪ್ರತ್ಯಾಗಮ "created";
    ಪಕ್ಷ LIBCRUN_CONTAINER_STATUS_RUNNING:
      ಪ್ರತ್ಯಾಗಮ "running";
    ಪಕ್ಷ LIBCRUN_CONTAINER_STATUS_PAUSED:
      ಪ್ರತ್ಯಾಗಮ "paused";
    ಪಕ್ಷ LIBCRUN_CONTAINER_STATUS_STOPPED:
      ಪ್ರತ್ಯಾಗಮ "stopped";
    }
  ಪ್ರತ್ಯಾಗಮ "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  ಪ್ರತ್ಯಾಗಮ st->status.pid;
}

ಧ್ರುವ ವರ್ಣ *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  ಪ್ರತ್ಯಾಗಮ st->status.bundle;
}

ಧ್ರುವ ವರ್ಣ *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  ಪ್ರತ್ಯಾಗಮ st->status.rootfs;
}

ಧ್ರುವ ವರ್ಣ *
libcrun_status_get_created (libcrun_status_t *st)
{
  ಪ್ರತ್ಯಾಗಮ st->status.created;
}

ಧ್ರುವ ವರ್ಣ *
libcrun_status_get_owner (libcrun_status_t *st)
{
  ಪ್ರತ್ಯಾಗಮ st->status.owner;
}

ಧ್ರುವ ವರ್ಣ *
libcrun_status_get_scope (libcrun_status_t *st)
{
  ಪ್ರತ್ಯಾಗಮ st->status.scope;
}

ಧ್ರುವ ವರ್ಣ *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  ಪ್ರತ್ಯಾಗಮ st->status.external_descriptors;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_get_state_string (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ **out, ಪೂರ್ಣಾಙ್ಕ *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  ಪೂರ್ಣಾಙ್ಕ local_running;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_read_container_status (&status, context->state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_ಬೇರು, out, &local_running, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (running)
    *running = local_running;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_get_state_dir (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ libcrun_get_state_directory (out, state_ಬೇರು, id, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ libcrun_get_containers_list (out, context->state_ಬೇರು, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (ಪರಿಮಾಣ (*it));
  it->cur = l;
  ಪ್ರತ್ಯಾಗಮ it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, ಧ್ರುವ ವರ್ಣ **id)
{
  ಯದಿ (it == ರಿಕ್ತ || it->cur == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;

  ಯದಿ (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  ಪ್ರತ್ಯಾಗಮ true;
}

ಶೂನ್ಯ
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  ಮುಕ್ತ (it);
}

ಶೂನ್ಯ
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
