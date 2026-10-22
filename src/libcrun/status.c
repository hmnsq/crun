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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "status.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "json_gen_utils.h"
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <json-c/json.h>
#અન્તર્ભાવ <ocispec/json_common.h>
#અન્તર્ભાવ <sys/mount.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <dirent.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <limits.h>

#define STEAL_POINTER(x, y) \
  કરો                        \
    {                       \
      *x = y;               \
      y = ખાલી;             \
  } જ્યારે (0)

સંરચના pid_stat
{
  અક્ષર state;
  long અંક_threads;
  unsigned long long starttime;
};

સ્થિર અંતઃરેખ bool
is_valid_id_char (અક્ષર c)
{
  જો ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    પાછા true;
  પાછા c == '_' || c == '+' || c == '.' || c == '-';
}

સ્થિર પૂર્ણાંક
validate_id (અચલ અક્ષર *id, libcrun_error_t *err)
{
  જો (id == ખાલી)
    પાછા 0;

  જો (id[0] == '\0')
    પાછા crun_make_error (err, 0, "empty container ID");

  જો (id[0] == '.')
    પાછા crun_make_error (err, 0, "invalid container ID `%s`", id);

  માટે (size_t i = 0; id[i]; i++)
    {
      જો (! is_valid_id_char (id[i]))
        પાછા crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  પાછા 0;
}

પૂર્ણાંક
get_run_directory (અક્ષર **out, અચલ અક્ષર *state_મૂળ, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_free અક્ષર *મૂળ = ખાલી;

  જો (state_મૂળ)
    મૂળ = xstrdup (state_મૂળ);
  જો (મૂળ == ખાલી)
    {
      અચલ અક્ષર *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      જો (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&મૂળ, err, runtime_dir, "crun", ખાલી);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }
  જો (મૂળ == ખાલી)
    મૂળ = xstrdup ("/run/crun");

  ret = crun_ensure_directory (મૂળ, 0700, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  STEAL_POINTER (out, મૂળ);

  પાછા 0;
}

પૂર્ણાંક
get_shared_empty_directory_path (અક્ષર **out, અચલ અક્ષર *state_મૂળ, libcrun_error_t *err)
{
  cleanup_free અક્ષર *run_dir = ખાલી;
  પૂર્ણાંક ret;

  ret = get_run_directory (&run_dir, state_મૂળ, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

પૂર્ણાંક
libcrun_get_state_directory (અક્ષર **out, અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_free અક્ષર *path = ખાલી;
  cleanup_free અક્ષર *મૂળ = ખાલી;

  ret = validate_id (id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = get_run_directory (&મૂળ, state_મૂળ, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_paths (&path, err, મૂળ, id, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  STEAL_POINTER (out, path);

  પાછા 0;
}

સ્થિર પૂર્ણાંક
get_state_directory_status_file (અક્ષર **out, અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err)
{
  cleanup_free અક્ષર *dir = ખાલી;
  પૂર્ણાંક ret;

  ret = libcrun_get_state_directory (&dir, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા append_paths (out, err, dir, "status", ખાલી);
}

સ્થિર પૂર્ણાંક
read_pid_stat (pid_t pid, સંરચના pid_stat *st, libcrun_error_t *err)
{
  cleanup_free અક્ષર *buffer = ખાલી;
  cleanup_close પૂર્ણાંક fd = -1;
  અક્ષર pid_stat_file[64];
  અક્ષર *it, *s;
  પૂર્ણાંક i, ret;

  ret = snprintf (pid_stat_file, કદ (pid_stat_file), "/proc/%d/stat", pid);
  જો (UNLIKELY (ret >= (પૂર્ણાંક) કદ (pid_stat_file)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  જો (fd < 0)
    {
      /* The process already exited.  */
      જો (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, કદ (*st));
          પાછા 0;
        }
      પાછા crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, ખાલી, err);
  જો (ret < 0)
    {
      st->starttime = 0;
      st->અંક_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      પાછા 0;
    }

  s = ખાલી;

  /* Skip the first two arguments.  */
  માટે (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  જો (s)
    જ્યારે (*s == ' ')
      s++;

  જો (s == ખાલી || *s == '\0')
    પાછા crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  માટે (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      જો (it == ખાલી)
        પાછા crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      જો (i == 16)
        st->અંક_threads = strtol (it, ખાલી, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, ખાલી, 10);
  જો (errno != 0)
    પાછા crun_make_error (err, errno, "parse process start time");

  પાછા 0;
}

પૂર્ણાંક
libcrun_write_container_status (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  પૂર્ણાંક r, ret;
  cleanup_free અક્ષર *file_tmp = ખાલી;
  cleanup_free અક્ષર *file = ખાલી;
  size_t len;
  cleanup_close પૂર્ણાંક fd_write = -1;
  અચલ અક્ષર *buf = ખાલી;
  સંરચના pid_stat st;
  અચલ અક્ષર *tmp;
  cleanup_json_gen json_gen_ctx *gen = ખાલી;

  ret = get_state_directory_status_file (&file, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = read_pid_stat (status->pid, &st, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  જો (UNLIKELY (fd_write < 0))
    પાછા crun_make_error (err, errno, "cannot open status file");

  જો (! json_gen_init (&gen, ખાલી))
    પાછા crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ચાવી (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_ચાવી (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_ચાવી (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_ચાવી (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_ચાવી (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_ચાવી (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_ચાવી (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_ચાવી (gen, "created");
  GEN_STR (gen, status->created);

  જો (status->owner)
    {
      GEN_ચાવી (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_ચાવી (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_ચાવી (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  જો (status->external_descriptors)
    {
      GEN_ચાવી (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  close_and_reset (&fd_write);

  જો (UNLIKELY (rename (file_tmp, file) < 0))
    પાછા crun_make_error (err, errno, "cannot rename status file");

  પાછા 0;

gen_error:
  પાછા json_gen_error_to_crun_error (r, err);
}

પૂર્ણાંક
libcrun_read_container_status (libcrun_container_status_t *status, અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id,
                               libcrun_error_t *err)
{
  cleanup_free અક્ષર *buffer = ખાલી;
  પૂર્ણાંક ret;
  cleanup_free અક્ષર *file = ખાલી;
  json_object *doc = ખાલી;
  json_object *tmp;
  અચલ અક્ષર *val;

  ret = get_state_directory_status_file (&file, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = read_all_file (file, &buffer, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    {

      જો (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free અક્ષર *statedir = ખાલી;
          libcrun_error_t tmp_err;
          પૂર્ણાંક tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_મૂળ, id, &tmp_err);
          જો (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          નહીંતર
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              જો (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              નહીંતર જો (tmp_ret == 0)
                પાછા crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      પાછા ret;
    }

  ret = parse_json_file (&doc, buffer, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  {
    tmp = json_object_object_get (doc, "pid");
    જો (UNLIKELY (tmp == ખાલી))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        જાઓ exit;
      }
    જો (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        જાઓ exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    જો (UNLIKELY (tmp == ખાલી))
      status->process_start_time = 0; /* backwards compatibility */
    નહીંતર જો (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        જાઓ exit;
      }
    નહીંતર
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    જો (UNLIKELY (tmp == ખાલી))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        જાઓ exit;
      }
    val = json_object_get_string (tmp);
    જો (UNLIKELY (val == ખાલી))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        જાઓ exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : ખાલી;
    status->scope = val ? xstrdup (val) : ખાલી;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    જો (UNLIKELY (tmp == ખાલી))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        જાઓ exit;
      }
    val = json_object_get_string (tmp);
    જો (UNLIKELY (val == ખાલી))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        જાઓ exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != ખાલી && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    જો (UNLIKELY (tmp == ખાલી))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        જાઓ exit;
      }
    val = json_object_get_string (tmp);
    જો (UNLIKELY (val == ખાલી))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        જાઓ exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    જો (UNLIKELY (tmp == ખાલી))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        જાઓ exit;
      }
    val = json_object_get_string (tmp);
    જો (UNLIKELY (val == ખાલી))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        જાઓ exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : ખાલી;
    status->owner = val ? xstrdup (val) : ખાલી;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != ખાલી && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != ખાલી && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : ખાલી;
    status->external_descriptors = val ? xstrdup (val) : ખાલી;
  }
  ret = 0;

exit:
  json_object_put (doc);
  પાછા ret;
}

પૂર્ણાંક
libcrun_status_check_directories (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err)
{
  cleanup_free અક્ષર *dir = ખાલી;
  પૂર્ણાંક ret;

  ret = libcrun_get_state_directory (&dir, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = crun_path_exists (dir, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (ret)
    પાછા crun_make_error (err, 0, "container `%s` already exists", id);

  જો (UNLIKELY (mkdir (dir, 0700) < 0))
    પાછા crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  પાછા 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

સ્થિર પૂર્ણાંક
rmdirfd_move_to (DIR **d, અચલ અક્ષર *name, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  જો (UNLIKELY (new_d == ખાલી))
    પાછા crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  પાછા 0;
}

સ્થિર પૂર્ણાંક
rmdirfd (અચલ અક્ષર *namedir, પૂર્ણાંક fd, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_dir DIR *d = ખાલી;
  સંરચના dirent *de;
  સંરચના stat મૂળ_st;
  bool is_મૂળ = true;
  unsigned પૂર્ણાંક budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close પૂર્ણાંક fd_cleanup = fd;

  ret = fstat (fd, &મૂળ_st);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  જો (d == ખાલી)
    પાછા crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  માટે (;;)
    {
      errno = 0;
      de = readdir (d);
      જો (de == ખાલી)
        {
          સંરચના stat st;

          જો (UNLIKELY (errno != 0))
            પાછા crun_make_error (err, errno, "cannot read directory");

          જો (is_મૂળ)
            તોડો;

          જો (UNLIKELY (--budget == 0))
            પાછા crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          ret = fstat (dirfd (d), &st);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "stat `..`");

          is_મૂળ = st.st_dev == મૂળ_st.st_dev && st.st_ino == મૂળ_st.st_ino;
          ચાલુ;
        }

      જો ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        ચાલુ;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      જો (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          જો (ret < 0 && errno == EBUSY)
            {
              cleanup_close પૂર્ણાંક tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              જો (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  જો (umount2 (procpath, MNT_DETACH) == 0)
                    જાઓ retry_unlink;
                }
              જો (! is_મૂળ)
                પાછા crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          નહીંતર જો (ret < 0 && errno == ENOTEMPTY)
            {
              જો (UNLIKELY (--budget == 0))
                પાછા crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              જો (UNLIKELY (ret < 0))
                પાછા ret;

              is_મૂળ = false;
            }
          નહીંતર જો (ret < 0 && errno != ENOENT && ! is_મૂળ)
            {
              પાછા crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  પાછા 0;
}

પૂર્ણાંક
libcrun_container_delete_status (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_close પૂર્ણાંક rundir_dfd = -1;
  cleanup_close પૂર્ણાંક dfd = -1;
  cleanup_free અક્ષર *dir = ખાલી;

  ret = validate_id (id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = get_run_directory (&dir, state_મૂળ, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  જો (UNLIKELY (rundir_dfd < 0))
    પાછા crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  જો (UNLIKELY (dfd < 0))
    પાછા crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  પાછા 0;
}

શૂન્ય
libcrun_free_container_status (libcrun_container_status_t *status)
{
  જો (status == ખાલી)
    પાછા;
  મુક્ત (status->cgroup_path);
  મુક્ત (status->bundle);
  મુક્ત (status->rootfs);
  મુક્ત (status->external_descriptors);
  મુક્ત (status->created);
  મુક્ત (status->scope);
  મુક્ત (status->owner);
}

પૂર્ણાંક
libcrun_get_containers_list (libcrun_container_list_t **out, અચલ અક્ષર *state_મૂળ, libcrun_error_t *err)
{
  સંરચના dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = ખાલી;
  cleanup_free અક્ષર *મૂળ = ખાલી;
  cleanup_dir DIR *dir = ખાલી;
  પૂર્ણાંક ret;

  *out = ખાલી;

  ret = get_run_directory (&મૂળ, state_મૂળ, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  dir = opendir (મૂળ);
  જો (UNLIKELY (dir == ખાલી))
    પાછા crun_make_error (err, errno, "cannot opendir `%s`", મૂળ);

  માટે (next = readdir (dir); next; next = readdir (dir))
    {
      પૂર્ણાંક exists;
      cleanup_free અક્ષર *status_file = ખાલી;

      libcrun_container_list_t *next_container;

      જો (next->d_name[0] == '.')
        ચાલુ;

      ret = append_paths (&status_file, err, મૂળ, next->d_name, "status", ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      exists = crun_path_exists (status_file, err);
      જો (exists < 0)
        {
          પાછા exists;
        }

      જો (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          ચાલુ;
        }

      next_container = xmalloc (કદ (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  પાછા 0;
}

શૂન્ય
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  જ્યારે (list)
    {
      next = list->next;
      મુક્ત (list->name);
      મુક્ત (list);
      list = next;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
પૂર્ણાંક
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  સંરચના pid_stat st;
  પૂર્ણાંક ret;

  /* For backwards compatibility, check start time only if available. */
  જો (! status->process_start_time)
    પાછા 1;

  ret = read_pid_stat (status->pid, &st, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (status->process_start_time != st.starttime || st.state == 'X')
    પાછા 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  જો (st.state == 'Z' && st.અંક_threads <= 1)
    પાછા 0; /* stopped */

  પાછા 1; /* running, created, or paused */
}

પૂર્ણાંક
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = kill (status->pid, 0);
  જો (UNLIKELY (ret < 0) && errno != ESRCH)
    પાછા crun_make_error (err, errno, "kill");

  જો (ret == 0)
    પાછા libcrun_check_pid_valid (status, err);

  પાછા 0; /* stopped */
}

સ્થિર પૂર્ણાંક
get_exec_fifo_path (અક્ષર **out, અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err)
{
  cleanup_free અક્ષર *state_dir = ખાલી;
  પૂર્ણાંક ret;

  ret = libcrun_get_state_directory (&state_dir, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા append_paths (out, err, state_dir, "exec.fifo", ખાલી);
}

પૂર્ણાંક
libcrun_status_create_exec_fifo (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err)
{
  cleanup_free અક્ષર *fifo_path = ખાલી;
  પૂર્ણાંક ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  પાછા fd;
}

પૂર્ણાંક
libcrun_status_write_exec_fifo (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err)
{
  cleanup_free અક્ષર *fifo_path = ખાલી;
  અક્ષર buffer[1] = {
    0,
  };
  cleanup_close પૂર્ણાંક fd = -1;
  પૂર્ણાંક ret;

  ret = get_exec_fifo_path (&fifo_path, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (લિખ (fd, buffer, 1));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "write to exec.fifo");

  પાછા 0;
}

પૂર્ણાંક
libcrun_status_has_read_exec_fifo (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err)
{
  cleanup_free અક્ષર *fifo_path = ખાલી;
  પૂર્ણાંક ret;

  ret = get_exec_fifo_path (&fifo_path, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા crun_path_exists (fifo_path, err);
}

સંરચના libcrun_status_s
{
  libcrun_container_status_t status;
  અક્ષર *id;
  અક્ષર *state_મૂળ;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  પૂર્ણાંક running;
};

પૂર્ણાંક
libcrun_container_status_load (libcrun_context_t *context, અચલ અક્ષર *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  પૂર્ણાંક ret;

  st = xmalloc0 (કદ (*st));

  ret = libcrun_read_container_status (&st->status, context->state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    {
      મુક્ત (st);
      પાછા ret;
    }

  st->id = xstrdup (id);
  st->state_મૂળ = xstrdup (context->state_મૂળ);

  *out = st;
  પાછા 0;
}

શૂન્ય
libcrun_container_status_free (libcrun_status_t *st)
{
  જો (st == ખાલી)
    પાછા;
  libcrun_free_container_status (&st->status);
  મુક્ત (st->state_મૂળ);
  મુક્ત (st->id);
  મુક્ત (st);
}

પૂર્ણાંક
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, પૂર્ણાંક *running, libcrun_error_t *err)
{
  જો (! st->state_resolved)
    {
      અચલ અક્ષર *state_string = ખાલી;
      પૂર્ણાંક ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_મૂળ, &state_string, &st->running, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      જો (state_string == ખાલી)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      નહીંતર જો (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      નહીંતર જો (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      નહીંતર જો (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      નહીંતર જો (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      નહીંતર
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  જો (state)
    *state = st->state;
  જો (running)
    *running = st->running;
  પાછા 0;
}

અચલ અક્ષર *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  પસંદગી (state)
    {
    સ્થિતિ LIBCRUN_CONTAINER_STATUS_CREATING:
      પાછા "creating";
    સ્થિતિ LIBCRUN_CONTAINER_STATUS_CREATED:
      પાછા "created";
    સ્થિતિ LIBCRUN_CONTAINER_STATUS_RUNNING:
      પાછા "running";
    સ્થિતિ LIBCRUN_CONTAINER_STATUS_PAUSED:
      પાછા "paused";
    સ્થિતિ LIBCRUN_CONTAINER_STATUS_STOPPED:
      પાછા "stopped";
    }
  પાછા "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  પાછા st->status.pid;
}

અચલ અક્ષર *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  પાછા st->status.bundle;
}

અચલ અક્ષર *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  પાછા st->status.rootfs;
}

અચલ અક્ષર *
libcrun_status_get_created (libcrun_status_t *st)
{
  પાછા st->status.created;
}

અચલ અક્ષર *
libcrun_status_get_owner (libcrun_status_t *st)
{
  પાછા st->status.owner;
}

અચલ અક્ષર *
libcrun_status_get_scope (libcrun_status_t *st)
{
  પાછા st->status.scope;
}

અચલ અક્ષર *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  પાછા st->status.external_descriptors;
}

પૂર્ણાંક
libcrun_container_get_state_string (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર **out, પૂર્ણાંક *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  પૂર્ણાંક local_running;
  પૂર્ણાંક ret;

  ret = libcrun_read_container_status (&status, context->state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_મૂળ, out, &local_running, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (running)
    *running = local_running;
  પાછા 0;
}

પૂર્ણાંક
libcrun_get_state_dir (અક્ષર **out, અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err)
{
  પાછા libcrun_get_state_directory (out, state_મૂળ, id, err);
}

પૂર્ણાંક
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  પાછા libcrun_get_containers_list (out, context->state_મૂળ, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (કદ (*it));
  it->cur = l;
  પાછા it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, અચલ અક્ષર **id)
{
  જો (it == ખાલી || it->cur == ખાલી)
    પાછા false;

  જો (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  પાછા true;
}

શૂન્ય
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  મુક્ત (it);
}

શૂન્ય
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
