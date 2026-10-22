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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "status.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "json_gen_utils.h"
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <json-c/json.h>
#அந்தர்பாவ <ocispec/json_common.h>
#அந்தர்பாவ <sys/mount.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <dirent.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <limits.h>

#define STEAL_POINTER(x, y) \
  செய்                        \
    {                       \
      *x = y;               \
      y = வெற்று;             \
  } போது (0)

கட்டமைப்பு pid_stat
{
  எழுத்து state;
  long இலக்கம்_threads;
  unsigned long long starttime;
};

நிலையான உள்வரி bool
is_valid_id_char (எழுத்து c)
{
  எனில் ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    திரும்பு true;
  திரும்பு c == '_' || c == '+' || c == '.' || c == '-';
}

நிலையான முழு
validate_id (மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  எனில் (id == வெற்று)
    திரும்பு 0;

  எனில் (id[0] == '\0')
    திரும்பு crun_make_error (err, 0, "empty container ID");

  எனில் (id[0] == '.')
    திரும்பு crun_make_error (err, 0, "invalid container ID `%s`", id);

  க்கு (size_t i = 0; id[i]; i++)
    {
      எனில் (! is_valid_id_char (id[i]))
        திரும்பு crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  திரும்பு 0;
}

முழு
get_run_directory (எழுத்து **out, மாறிலி எழுத்து *state_வேர், libcrun_error_t *err)
{
  முழு ret;
  cleanup_free எழுத்து *வேர் = வெற்று;

  எனில் (state_வேர்)
    வேர் = xstrdup (state_வேர்);
  எனில் (வேர் == வெற்று)
    {
      மாறிலி எழுத்து *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      எனில் (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&வேர், err, runtime_dir, "crun", வெற்று);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }
  எனில் (வேர் == வெற்று)
    வேர் = xstrdup ("/run/crun");

  ret = crun_ensure_directory (வேர், 0700, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  STEAL_POINTER (out, வேர்);

  திரும்பு 0;
}

முழு
get_shared_empty_directory_path (எழுத்து **out, மாறிலி எழுத்து *state_வேர், libcrun_error_t *err)
{
  cleanup_free எழுத்து *run_dir = வெற்று;
  முழு ret;

  ret = get_run_directory (&run_dir, state_வேர், err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

முழு
libcrun_get_state_directory (எழுத்து **out, மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  முழு ret;
  cleanup_free எழுத்து *path = வெற்று;
  cleanup_free எழுத்து *வேர் = வெற்று;

  ret = validate_id (id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = get_run_directory (&வேர், state_வேர், err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_paths (&path, err, வேர், id, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  STEAL_POINTER (out, path);

  திரும்பு 0;
}

நிலையான முழு
get_state_directory_status_file (எழுத்து **out, மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  cleanup_free எழுத்து *dir = வெற்று;
  முழு ret;

  ret = libcrun_get_state_directory (&dir, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு append_paths (out, err, dir, "status", வெற்று);
}

நிலையான முழு
read_pid_stat (pid_t pid, கட்டமைப்பு pid_stat *st, libcrun_error_t *err)
{
  cleanup_free எழுத்து *buffer = வெற்று;
  cleanup_close முழு fd = -1;
  எழுத்து pid_stat_file[64];
  எழுத்து *it, *s;
  முழு i, ret;

  ret = snprintf (pid_stat_file, அளவு (pid_stat_file), "/proc/%d/stat", pid);
  எனில் (UNLIKELY (ret >= (முழு) அளவு (pid_stat_file)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  எனில் (fd < 0)
    {
      /* The process already exited.  */
      எனில் (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, அளவு (*st));
          திரும்பு 0;
        }
      திரும்பு crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, வெற்று, err);
  எனில் (ret < 0)
    {
      st->starttime = 0;
      st->இலக்கம்_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      திரும்பு 0;
    }

  s = வெற்று;

  /* Skip the first two arguments.  */
  க்கு (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  எனில் (s)
    போது (*s == ' ')
      s++;

  எனில் (s == வெற்று || *s == '\0')
    திரும்பு crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  க்கு (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      எனில் (it == வெற்று)
        திரும்பு crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      எனில் (i == 16)
        st->இலக்கம்_threads = strtol (it, வெற்று, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, வெற்று, 10);
  எனில் (errno != 0)
    திரும்பு crun_make_error (err, errno, "parse process start time");

  திரும்பு 0;
}

முழு
libcrun_write_container_status (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  முழு r, ret;
  cleanup_free எழுத்து *file_tmp = வெற்று;
  cleanup_free எழுத்து *file = வெற்று;
  size_t len;
  cleanup_close முழு fd_write = -1;
  மாறிலி எழுத்து *buf = வெற்று;
  கட்டமைப்பு pid_stat st;
  மாறிலி எழுத்து *tmp;
  cleanup_json_gen json_gen_ctx *gen = வெற்று;

  ret = get_state_directory_status_file (&file, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = read_pid_stat (status->pid, &st, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  எனில் (UNLIKELY (fd_write < 0))
    திரும்பு crun_make_error (err, errno, "cannot open status file");

  எனில் (! json_gen_init (&gen, வெற்று))
    திரும்பு crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_சாவி (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_சாவி (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_சாவி (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_சாவி (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_சாவி (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_சாவி (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_சாவி (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_சாவி (gen, "created");
  GEN_STR (gen, status->created);

  எனில் (status->owner)
    {
      GEN_சாவி (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_சாவி (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_சாவி (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  எனில் (status->external_descriptors)
    {
      GEN_சாவி (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  close_and_reset (&fd_write);

  எனில் (UNLIKELY (rename (file_tmp, file) < 0))
    திரும்பு crun_make_error (err, errno, "cannot rename status file");

  திரும்பு 0;

gen_error:
  திரும்பு json_gen_error_to_crun_error (r, err);
}

முழு
libcrun_read_container_status (libcrun_container_status_t *status, மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id,
                               libcrun_error_t *err)
{
  cleanup_free எழுத்து *buffer = வெற்று;
  முழு ret;
  cleanup_free எழுத்து *file = வெற்று;
  json_object *doc = வெற்று;
  json_object *tmp;
  மாறிலி எழுத்து *val;

  ret = get_state_directory_status_file (&file, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = read_all_file (file, &buffer, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    {

      எனில் (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free எழுத்து *statedir = வெற்று;
          libcrun_error_t tmp_err;
          முழு tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_வேர், id, &tmp_err);
          எனில் (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          இல்லையெனில்
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              எனில் (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              இல்லையெனில் எனில் (tmp_ret == 0)
                திரும்பு crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      திரும்பு ret;
    }

  ret = parse_json_file (&doc, buffer, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  {
    tmp = json_object_object_get (doc, "pid");
    எனில் (UNLIKELY (tmp == வெற்று))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        செல் exit;
      }
    எனில் (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        செல் exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    எனில் (UNLIKELY (tmp == வெற்று))
      status->process_start_time = 0; /* backwards compatibility */
    இல்லையெனில் எனில் (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        செல் exit;
      }
    இல்லையெனில்
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    எனில் (UNLIKELY (tmp == வெற்று))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        செல் exit;
      }
    val = json_object_get_string (tmp);
    எனில் (UNLIKELY (val == வெற்று))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        செல் exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : வெற்று;
    status->scope = val ? xstrdup (val) : வெற்று;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    எனில் (UNLIKELY (tmp == வெற்று))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        செல் exit;
      }
    val = json_object_get_string (tmp);
    எனில் (UNLIKELY (val == வெற்று))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        செல் exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != வெற்று && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    எனில் (UNLIKELY (tmp == வெற்று))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        செல் exit;
      }
    val = json_object_get_string (tmp);
    எனில் (UNLIKELY (val == வெற்று))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        செல் exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    எனில் (UNLIKELY (tmp == வெற்று))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        செல் exit;
      }
    val = json_object_get_string (tmp);
    எனில் (UNLIKELY (val == வெற்று))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        செல் exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : வெற்று;
    status->owner = val ? xstrdup (val) : வெற்று;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != வெற்று && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != வெற்று && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : வெற்று;
    status->external_descriptors = val ? xstrdup (val) : வெற்று;
  }
  ret = 0;

exit:
  json_object_put (doc);
  திரும்பு ret;
}

முழு
libcrun_status_check_directories (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  cleanup_free எழுத்து *dir = வெற்று;
  முழு ret;

  ret = libcrun_get_state_directory (&dir, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = crun_path_exists (dir, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (ret)
    திரும்பு crun_make_error (err, 0, "container `%s` already exists", id);

  எனில் (UNLIKELY (mkdir (dir, 0700) < 0))
    திரும்பு crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  திரும்பு 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

நிலையான முழு
rmdirfd_move_to (DIR **d, மாறிலி எழுத்து *name, libcrun_error_t *err)
{
  cleanup_close முழு fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  எனில் (UNLIKELY (new_d == வெற்று))
    திரும்பு crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  திரும்பு 0;
}

நிலையான முழு
rmdirfd (மாறிலி எழுத்து *namedir, முழு fd, libcrun_error_t *err)
{
  முழு ret;
  cleanup_dir DIR *d = வெற்று;
  கட்டமைப்பு dirent *de;
  கட்டமைப்பு stat வேர்_st;
  bool is_வேர் = true;
  unsigned முழு budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close முழு fd_cleanup = fd;

  ret = fstat (fd, &வேர்_st);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  எனில் (d == வெற்று)
    திரும்பு crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  க்கு (;;)
    {
      errno = 0;
      de = readdir (d);
      எனில் (de == வெற்று)
        {
          கட்டமைப்பு stat st;

          எனில் (UNLIKELY (errno != 0))
            திரும்பு crun_make_error (err, errno, "cannot read directory");

          எனில் (is_வேர்)
            நிறுத்து;

          எனில் (UNLIKELY (--budget == 0))
            திரும்பு crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          ret = fstat (dirfd (d), &st);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "stat `..`");

          is_வேர் = st.st_dev == வேர்_st.st_dev && st.st_ino == வேர்_st.st_ino;
          தொடர்;
        }

      எனில் ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        தொடர்;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      எனில் (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          எனில் (ret < 0 && errno == EBUSY)
            {
              cleanup_close முழு tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              எனில் (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  எனில் (umount2 (procpath, MNT_DETACH) == 0)
                    செல் retry_unlink;
                }
              எனில் (! is_வேர்)
                திரும்பு crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          இல்லையெனில் எனில் (ret < 0 && errno == ENOTEMPTY)
            {
              எனில் (UNLIKELY (--budget == 0))
                திரும்பு crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;

              is_வேர் = false;
            }
          இல்லையெனில் எனில் (ret < 0 && errno != ENOENT && ! is_வேர்)
            {
              திரும்பு crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  திரும்பு 0;
}

முழு
libcrun_container_delete_status (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  முழு ret;
  cleanup_close முழு rundir_dfd = -1;
  cleanup_close முழு dfd = -1;
  cleanup_free எழுத்து *dir = வெற்று;

  ret = validate_id (id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = get_run_directory (&dir, state_வேர், err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  எனில் (UNLIKELY (rundir_dfd < 0))
    திரும்பு crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  எனில் (UNLIKELY (dfd < 0))
    திரும்பு crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  திரும்பு 0;
}

வெறுமை
libcrun_free_container_status (libcrun_container_status_t *status)
{
  எனில் (status == வெற்று)
    திரும்பு;
  விடுவி (status->cgroup_path);
  விடுவி (status->bundle);
  விடுவி (status->rootfs);
  விடுவி (status->external_descriptors);
  விடுவி (status->created);
  விடுவி (status->scope);
  விடுவி (status->owner);
}

முழு
libcrun_get_containers_list (libcrun_container_list_t **out, மாறிலி எழுத்து *state_வேர், libcrun_error_t *err)
{
  கட்டமைப்பு dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = வெற்று;
  cleanup_free எழுத்து *வேர் = வெற்று;
  cleanup_dir DIR *dir = வெற்று;
  முழு ret;

  *out = வெற்று;

  ret = get_run_directory (&வேர், state_வேர், err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  dir = opendir (வேர்);
  எனில் (UNLIKELY (dir == வெற்று))
    திரும்பு crun_make_error (err, errno, "cannot opendir `%s`", வேர்);

  க்கு (next = readdir (dir); next; next = readdir (dir))
    {
      முழு exists;
      cleanup_free எழுத்து *status_file = வெற்று;

      libcrun_container_list_t *next_container;

      எனில் (next->d_name[0] == '.')
        தொடர்;

      ret = append_paths (&status_file, err, வேர், next->d_name, "status", வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      exists = crun_path_exists (status_file, err);
      எனில் (exists < 0)
        {
          திரும்பு exists;
        }

      எனில் (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          தொடர்;
        }

      next_container = xmalloc (அளவு (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  திரும்பு 0;
}

வெறுமை
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  போது (list)
    {
      next = list->next;
      விடுவி (list->name);
      விடுவி (list);
      list = next;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
முழு
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  கட்டமைப்பு pid_stat st;
  முழு ret;

  /* For backwards compatibility, check start time only if available. */
  எனில் (! status->process_start_time)
    திரும்பு 1;

  ret = read_pid_stat (status->pid, &st, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (status->process_start_time != st.starttime || st.state == 'X')
    திரும்பு 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  எனில் (st.state == 'Z' && st.இலக்கம்_threads <= 1)
    திரும்பு 0; /* stopped */

  திரும்பு 1; /* running, created, or paused */
}

முழு
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  முழு ret;

  ret = kill (status->pid, 0);
  எனில் (UNLIKELY (ret < 0) && errno != ESRCH)
    திரும்பு crun_make_error (err, errno, "kill");

  எனில் (ret == 0)
    திரும்பு libcrun_check_pid_valid (status, err);

  திரும்பு 0; /* stopped */
}

நிலையான முழு
get_exec_fifo_path (எழுத்து **out, மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  cleanup_free எழுத்து *state_dir = வெற்று;
  முழு ret;

  ret = libcrun_get_state_directory (&state_dir, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு append_paths (out, err, state_dir, "exec.fifo", வெற்று);
}

முழு
libcrun_status_create_exec_fifo (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  cleanup_free எழுத்து *fifo_path = வெற்று;
  முழு ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  திரும்பு fd;
}

முழு
libcrun_status_write_exec_fifo (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  cleanup_free எழுத்து *fifo_path = வெற்று;
  எழுத்து buffer[1] = {
    0,
  };
  cleanup_close முழு fd = -1;
  முழு ret;

  ret = get_exec_fifo_path (&fifo_path, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (லிக (fd, buffer, 1));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "write to exec.fifo");

  திரும்பு 0;
}

முழு
libcrun_status_has_read_exec_fifo (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  cleanup_free எழுத்து *fifo_path = வெற்று;
  முழு ret;

  ret = get_exec_fifo_path (&fifo_path, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு crun_path_exists (fifo_path, err);
}

கட்டமைப்பு libcrun_status_s
{
  libcrun_container_status_t status;
  எழுத்து *id;
  எழுத்து *state_வேர்;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  முழு running;
};

முழு
libcrun_container_status_load (libcrun_context_t *context, மாறிலி எழுத்து *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  முழு ret;

  st = xmalloc0 (அளவு (*st));

  ret = libcrun_read_container_status (&st->status, context->state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    {
      விடுவி (st);
      திரும்பு ret;
    }

  st->id = xstrdup (id);
  st->state_வேர் = xstrdup (context->state_வேர்);

  *out = st;
  திரும்பு 0;
}

வெறுமை
libcrun_container_status_free (libcrun_status_t *st)
{
  எனில் (st == வெற்று)
    திரும்பு;
  libcrun_free_container_status (&st->status);
  விடுவி (st->state_வேர்);
  விடுவி (st->id);
  விடுவி (st);
}

முழு
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, முழு *running, libcrun_error_t *err)
{
  எனில் (! st->state_resolved)
    {
      மாறிலி எழுத்து *state_string = வெற்று;
      முழு ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_வேர், &state_string, &st->running, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      எனில் (state_string == வெற்று)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      இல்லையெனில் எனில் (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      இல்லையெனில் எனில் (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      இல்லையெனில் எனில் (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      இல்லையெனில் எனில் (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      இல்லையெனில்
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  எனில் (state)
    *state = st->state;
  எனில் (running)
    *running = st->running;
  திரும்பு 0;
}

மாறிலி எழுத்து *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  தேர்வு (state)
    {
    நிலை LIBCRUN_CONTAINER_STATUS_CREATING:
      திரும்பு "creating";
    நிலை LIBCRUN_CONTAINER_STATUS_CREATED:
      திரும்பு "created";
    நிலை LIBCRUN_CONTAINER_STATUS_RUNNING:
      திரும்பு "running";
    நிலை LIBCRUN_CONTAINER_STATUS_PAUSED:
      திரும்பு "paused";
    நிலை LIBCRUN_CONTAINER_STATUS_STOPPED:
      திரும்பு "stopped";
    }
  திரும்பு "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  திரும்பு st->status.pid;
}

மாறிலி எழுத்து *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  திரும்பு st->status.bundle;
}

மாறிலி எழுத்து *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  திரும்பு st->status.rootfs;
}

மாறிலி எழுத்து *
libcrun_status_get_created (libcrun_status_t *st)
{
  திரும்பு st->status.created;
}

மாறிலி எழுத்து *
libcrun_status_get_owner (libcrun_status_t *st)
{
  திரும்பு st->status.owner;
}

மாறிலி எழுத்து *
libcrun_status_get_scope (libcrun_status_t *st)
{
  திரும்பு st->status.scope;
}

மாறிலி எழுத்து *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  திரும்பு st->status.external_descriptors;
}

முழு
libcrun_container_get_state_string (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து **out, முழு *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  முழு local_running;
  முழு ret;

  ret = libcrun_read_container_status (&status, context->state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_வேர், out, &local_running, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (running)
    *running = local_running;
  திரும்பு 0;
}

முழு
libcrun_get_state_dir (எழுத்து **out, மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  திரும்பு libcrun_get_state_directory (out, state_வேர், id, err);
}

முழு
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  திரும்பு libcrun_get_containers_list (out, context->state_வேர், err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (அளவு (*it));
  it->cur = l;
  திரும்பு it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, மாறிலி எழுத்து **id)
{
  எனில் (it == வெற்று || it->cur == வெற்று)
    திரும்பு false;

  எனில் (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  திரும்பு true;
}

வெறுமை
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  விடுவி (it);
}

வெறுமை
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
