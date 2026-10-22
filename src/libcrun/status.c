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

#include <config.h>
#include "status.h"
#include "utils.h"
#include "json_gen_utils.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <json-c/json.h>
#include <ocispec/json_common.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <limits.h>

#define STEAL_POINTER(x, y) \
  do                        \
    {                       \
      *x = y;               \
      y = NULL;             \
  } ꦱꦼꦢꦼꦭ (0)

sa_nracanaa pid_stat
{
  var_na state;
  long num_threads;
  unsigned long long starttime;
};

static anta_hstha bool
is_valid_id_char (var_na c)
{
  ꦪꦼꦤ꧀ ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    ꦮꦭꦶ true;
  ꦮꦭꦶ c == '_' || c == '+' || c == '.' || c == '-';
}

static puur_naa_nka
validate_id (const var_na *id, libcrun_error_t *err)
{
  ꦪꦼꦤ꧀ (id == NULL)
    ꦮꦭꦶ 0;

  ꦪꦼꦤ꧀ (id[0] == '\0')
    ꦮꦭꦶ crun_make_error (err, 0, "empty container ID");

  ꦪꦼꦤ꧀ (id[0] == '.')
    ꦮꦭꦶ crun_make_error (err, 0, "invalid container ID `%s`", id);

  ꦏꦁꦒꦺꦴ (size_t i = 0; id[i]; i++)
    {
      ꦪꦼꦤ꧀ (! is_valid_id_char (id[i]))
        ꦮꦭꦶ crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  ꦮꦭꦶ 0;
}

puur_naa_nka
get_run_directory (var_na **out, const var_na *state_root, libcrun_error_t *err)
{
  puur_naa_nka ret;
  cleanup_free var_na *root = NULL;

  ꦪꦼꦤ꧀ (state_root)
    root = xstrdup (state_root);
  ꦪꦼꦤ꧀ (root == NULL)
    {
      const var_na *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      ꦪꦼꦤ꧀ (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&root, err, runtime_dir, "crun", NULL);
          ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
            ꦮꦭꦶ ret;
        }
    }
  ꦪꦼꦤ꧀ (root == NULL)
    root = xstrdup ("/run/crun");

  ret = crun_ensure_directory (root, 0700, false, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  STEAL_POINTER (out, root);

  ꦮꦭꦶ 0;
}

puur_naa_nka
get_shared_empty_directory_path (var_na **out, const var_na *state_root, libcrun_error_t *err)
{
  cleanup_free var_na *run_dir = NULL;
  puur_naa_nka ret;

  ret = get_run_directory (&run_dir, state_root, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", NULL);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ꦮꦭꦶ 0;
}

puur_naa_nka
libcrun_get_state_directory (var_na **out, const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  puur_naa_nka ret;
  cleanup_free var_na *path = NULL;
  cleanup_free var_na *root = NULL;

  ret = validate_id (id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ret = get_run_directory (&root, state_root, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ret = append_paths (&path, err, root, id, NULL);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  STEAL_POINTER (out, path);

  ꦮꦭꦶ 0;
}

static puur_naa_nka
get_state_directory_status_file (var_na **out, const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *dir = NULL;
  puur_naa_nka ret;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ꦮꦭꦶ append_paths (out, err, dir, "status", NULL);
}

static puur_naa_nka
read_pid_stat (pid_t pid, sa_nracanaa pid_stat *st, libcrun_error_t *err)
{
  cleanup_free var_na *buffer = NULL;
  cleanup_close puur_naa_nka fd = -1;
  var_na pid_stat_file[64];
  var_na *it, *s;
  puur_naa_nka i, ret;

  ret = snprintf (pid_stat_file, parimaa_na (pid_stat_file), "/proc/%d/stat", pid);
  ꦪꦼꦤ꧀ (UNLIKELY (ret >= (puur_naa_nka) parimaa_na (pid_stat_file)))
    ꦮꦭꦶ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  ꦪꦼꦤ꧀ (fd < 0)
    {
      /* The process already exited.  */
      ꦪꦼꦤ꧀ (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, parimaa_na (*st));
          ꦮꦭꦶ 0;
        }
      ꦮꦭꦶ crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, NULL, err);
  ꦪꦼꦤ꧀ (ret < 0)
    {
      st->starttime = 0;
      st->num_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      ꦮꦭꦶ 0;
    }

  s = NULL;

  /* Skip the first two arguments.  */
  ꦏꦁꦒꦺꦴ (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  ꦪꦼꦤ꧀ (s)
    ꦱꦼꦢꦼꦭ (*s == ' ')
      s++;

  ꦪꦼꦤ꧀ (s == NULL || *s == '\0')
    ꦮꦭꦶ crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  ꦏꦁꦒꦺꦴ (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      ꦪꦼꦤ꧀ (it == NULL)
        ꦮꦭꦶ crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      ꦪꦼꦤ꧀ (i == 16)
        st->num_threads = strtol (it, NULL, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, NULL, 10);
  ꦪꦼꦤ꧀ (errno != 0)
    ꦮꦭꦶ crun_make_error (err, errno, "parse process start time");

  ꦮꦭꦶ 0;
}

puur_naa_nka
libcrun_write_container_status (const var_na *state_root, const var_na *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  puur_naa_nka r, ret;
  cleanup_free var_na *file_tmp = NULL;
  cleanup_free var_na *file = NULL;
  size_t len;
  cleanup_close puur_naa_nka fd_write = -1;
  const var_na *buf = NULL;
  sa_nracanaa pid_stat st;
  const var_na *tmp;
  cleanup_json_gen json_gen_ctx *gen = NULL;

  ret = get_state_directory_status_file (&file, state_root, id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ret = read_pid_stat (status->pid, &st, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  ꦪꦼꦤ꧀ (UNLIKELY (fd_write < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "cannot open status file");

  ꦪꦼꦤ꧀ (! json_gen_init (&gen, NULL))
    ꦮꦭꦶ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_KEY (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_KEY (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_KEY (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_KEY (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_KEY (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_KEY (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_KEY (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_KEY (gen, "created");
  GEN_STR (gen, status->created);

  ꦪꦼꦤ꧀ (status->owner)
    {
      GEN_KEY (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_KEY (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_KEY (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  ꦪꦼꦤ꧀ (status->external_descriptors)
    {
      GEN_KEY (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  close_and_reset (&fd_write);

  ꦪꦼꦤ꧀ (UNLIKELY (rename (file_tmp, file) < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "cannot rename status file");

  ꦮꦭꦶ 0;

gen_error:
  ꦮꦭꦶ json_gen_error_to_crun_error (r, err);
}

puur_naa_nka
libcrun_read_container_status (libcrun_container_status_t *status, const var_na *state_root, const var_na *id,
                               libcrun_error_t *err)
{
  cleanup_free var_na *buffer = NULL;
  puur_naa_nka ret;
  cleanup_free var_na *file = NULL;
  json_object *doc = NULL;
  json_object *tmp;
  const var_na *val;

  ret = get_state_directory_status_file (&file, state_root, id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ret = read_all_file (file, &buffer, NULL, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    {

      ꦪꦼꦤ꧀ (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free var_na *statedir = NULL;
          libcrun_error_t tmp_err;
          puur_naa_nka tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_root, id, &tmp_err);
          ꦪꦼꦤ꧀ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          ꦭꦶꦪ
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              ꦪꦼꦤ꧀ (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              ꦭꦶꦪ ꦪꦼꦤ꧀ (tmp_ret == 0)
                ꦮꦭꦶ crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      ꦮꦭꦶ ret;
    }

  ret = parse_json_file (&doc, buffer, NULL, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  {
    tmp = json_object_object_get (doc, "pid");
    ꦪꦼꦤ꧀ (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        goto exit;
      }
    ꦪꦼꦤ꧀ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        goto exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    ꦪꦼꦤ꧀ (UNLIKELY (tmp == NULL))
      status->process_start_time = 0; /* backwards compatibility */
    ꦭꦶꦪ ꦪꦼꦤ꧀ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        goto exit;
      }
    ꦭꦶꦪ
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    ꦪꦼꦤ꧀ (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ꦪꦼꦤ꧀ (UNLIKELY (val == NULL))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        goto exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : NULL;
    status->scope = val ? xstrdup (val) : NULL;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    ꦪꦼꦤ꧀ (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ꦪꦼꦤ꧀ (UNLIKELY (val == NULL))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        goto exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != NULL && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    ꦪꦼꦤ꧀ (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ꦪꦼꦤ꧀ (UNLIKELY (val == NULL))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        goto exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    ꦪꦼꦤ꧀ (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ꦪꦼꦤ꧀ (UNLIKELY (val == NULL))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        goto exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : NULL;
    status->owner = val ? xstrdup (val) : NULL;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != NULL && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != NULL && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : NULL;
    status->external_descriptors = val ? xstrdup (val) : NULL;
  }
  ret = 0;

exit:
  json_object_put (doc);
  ꦮꦭꦶ ret;
}

puur_naa_nka
libcrun_status_check_directories (const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *dir = NULL;
  puur_naa_nka ret;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ret = crun_path_exists (dir, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ꦪꦼꦤ꧀ (ret)
    ꦮꦭꦶ crun_make_error (err, 0, "container `%s` already exists", id);

  ꦪꦼꦤ꧀ (UNLIKELY (mkdir (dir, 0700) < 0))
    ꦮꦭꦶ crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  ꦮꦭꦶ 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

static puur_naa_nka
rmdirfd_move_to (DIR **d, const var_na *name, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  ꦪꦼꦤ꧀ (UNLIKELY (fd < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  ꦪꦼꦤ꧀ (UNLIKELY (new_d == NULL))
    ꦮꦭꦶ crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  ꦮꦭꦶ 0;
}

static puur_naa_nka
rmdirfd (const var_na *namedir, puur_naa_nka fd, libcrun_error_t *err)
{
  puur_naa_nka ret;
  cleanup_dir DIR *d = NULL;
  sa_nracanaa dirent *de;
  sa_nracanaa stat root_st;
  bool is_root = true;
  unsigned puur_naa_nka budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close puur_naa_nka fd_cleanup = fd;

  ret = fstat (fd, &root_st);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  ꦪꦼꦤ꧀ (d == NULL)
    ꦮꦭꦶ crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  ꦏꦁꦒꦺꦴ (;;)
    {
      errno = 0;
      de = readdir (d);
      ꦪꦼꦤ꧀ (de == NULL)
        {
          sa_nracanaa stat st;

          ꦪꦼꦤ꧀ (UNLIKELY (errno != 0))
            ꦮꦭꦶ crun_make_error (err, errno, "cannot read directory");

          ꦪꦼꦤ꧀ (is_root)
            bha_nga;

          ꦪꦼꦤ꧀ (UNLIKELY (--budget == 0))
            ꦮꦭꦶ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
            ꦮꦭꦶ ret;

          ret = fstat (dirfd (d), &st);
          ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
            ꦮꦭꦶ crun_make_error (err, errno, "stat `..`");

          is_root = st.st_dev == root_st.st_dev && st.st_ino == root_st.st_ino;
          continue;
        }

      ꦪꦼꦤ꧀ ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        continue;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      ꦪꦼꦤ꧀ (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          ꦪꦼꦤ꧀ (ret < 0 && errno == EBUSY)
            {
              cleanup_close puur_naa_nka tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              ꦪꦼꦤ꧀ (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  ꦪꦼꦤ꧀ (umount2 (procpath, MNT_DETACH) == 0)
                    goto retry_unlink;
                }
              ꦪꦼꦤ꧀ (! is_root)
                ꦮꦭꦶ crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          ꦭꦶꦪ ꦪꦼꦤ꧀ (ret < 0 && errno == ENOTEMPTY)
            {
              ꦪꦼꦤ꧀ (UNLIKELY (--budget == 0))
                ꦮꦭꦶ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
                ꦮꦭꦶ ret;

              is_root = false;
            }
          ꦭꦶꦪ ꦪꦼꦤ꧀ (ret < 0 && errno != ENOENT && ! is_root)
            {
              ꦮꦭꦶ crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  ꦮꦭꦶ 0;
}

puur_naa_nka
libcrun_container_delete_status (const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  puur_naa_nka ret;
  cleanup_close puur_naa_nka rundir_dfd = -1;
  cleanup_close puur_naa_nka dfd = -1;
  cleanup_free var_na *dir = NULL;

  ret = validate_id (id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ret = get_run_directory (&dir, state_root, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  ꦪꦼꦤ꧀ (UNLIKELY (rundir_dfd < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  ꦪꦼꦤ꧀ (UNLIKELY (dfd < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  ꦮꦭꦶ 0;
}

void
libcrun_free_container_status (libcrun_container_status_t *status)
{
  ꦪꦼꦤ꧀ (status == NULL)
    ꦮꦭꦶ;
  free (status->cgroup_path);
  free (status->bundle);
  free (status->rootfs);
  free (status->external_descriptors);
  free (status->created);
  free (status->scope);
  free (status->owner);
}

puur_naa_nka
libcrun_get_containers_list (libcrun_container_list_t **out, const var_na *state_root, libcrun_error_t *err)
{
  sa_nracanaa dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = NULL;
  cleanup_free var_na *root = NULL;
  cleanup_dir DIR *dir = NULL;
  puur_naa_nka ret;

  *out = NULL;

  ret = get_run_directory (&root, state_root, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  dir = opendir (root);
  ꦪꦼꦤ꧀ (UNLIKELY (dir == NULL))
    ꦮꦭꦶ crun_make_error (err, errno, "cannot opendir `%s`", root);

  ꦏꦁꦒꦺꦴ (next = readdir (dir); next; next = readdir (dir))
    {
      puur_naa_nka exists;
      cleanup_free var_na *status_file = NULL;

      libcrun_container_list_t *next_container;

      ꦪꦼꦤ꧀ (next->d_name[0] == '.')
        continue;

      ret = append_paths (&status_file, err, root, next->d_name, "status", NULL);
      ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
        ꦮꦭꦶ ret;

      exists = crun_path_exists (status_file, err);
      ꦪꦼꦤ꧀ (exists < 0)
        {
          ꦮꦭꦶ exists;
        }

      ꦪꦼꦤ꧀ (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          continue;
        }

      next_container = xmalloc (parimaa_na (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  ꦮꦭꦶ 0;
}

void
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  ꦱꦼꦢꦼꦭ (list)
    {
      next = list->next;
      free (list->name);
      free (list);
      list = next;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
puur_naa_nka
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  sa_nracanaa pid_stat st;
  puur_naa_nka ret;

  /* For backwards compatibility, check start time only if available. */
  ꦪꦼꦤ꧀ (! status->process_start_time)
    ꦮꦭꦶ 1;

  ret = read_pid_stat (status->pid, &st, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ꦪꦼꦤ꧀ (status->process_start_time != st.starttime || st.state == 'X')
    ꦮꦭꦶ 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  ꦪꦼꦤ꧀ (st.state == 'Z' && st.num_threads <= 1)
    ꦮꦭꦶ 0; /* stopped */

  ꦮꦭꦶ 1; /* running, created, or paused */
}

puur_naa_nka
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = kill (status->pid, 0);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0) && errno != ESRCH)
    ꦮꦭꦶ crun_make_error (err, errno, "kill");

  ꦪꦼꦤ꧀ (ret == 0)
    ꦮꦭꦶ libcrun_check_pid_valid (status, err);

  ꦮꦭꦶ 0; /* stopped */
}

static puur_naa_nka
get_exec_fifo_path (var_na **out, const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *state_dir = NULL;
  puur_naa_nka ret;

  ret = libcrun_get_state_directory (&state_dir, state_root, id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ꦮꦭꦶ append_paths (out, err, state_dir, "exec.fifo", NULL);
}

puur_naa_nka
libcrun_status_create_exec_fifo (const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *fifo_path = NULL;
  puur_naa_nka ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  ꦪꦼꦤ꧀ (UNLIKELY (fd < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  ꦮꦭꦶ fd;
}

puur_naa_nka
libcrun_status_write_exec_fifo (const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *fifo_path = NULL;
  var_na buffer[1] = {
    0,
  };
  cleanup_close puur_naa_nka fd = -1;
  puur_naa_nka ret;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  ꦪꦼꦤ꧀ (UNLIKELY (fd < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (write (fd, buffer, 1));
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ crun_make_error (err, errno, "write to exec.fifo");

  ꦮꦭꦶ 0;
}

puur_naa_nka
libcrun_status_has_read_exec_fifo (const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *fifo_path = NULL;
  puur_naa_nka ret;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ꦮꦭꦶ crun_path_exists (fifo_path, err);
}

sa_nracanaa libcrun_status_s
{
  libcrun_container_status_t status;
  var_na *id;
  var_na *state_root;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  puur_naa_nka running;
};

puur_naa_nka
libcrun_container_status_load (libcrun_context_t *context, const var_na *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  puur_naa_nka ret;

  st = xmalloc0 (parimaa_na (*st));

  ret = libcrun_read_container_status (&st->status, context->state_root, id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    {
      free (st);
      ꦮꦭꦶ ret;
    }

  st->id = xstrdup (id);
  st->state_root = xstrdup (context->state_root);

  *out = st;
  ꦮꦭꦶ 0;
}

void
libcrun_container_status_free (libcrun_status_t *st)
{
  ꦪꦼꦤ꧀ (st == NULL)
    ꦮꦭꦶ;
  libcrun_free_container_status (&st->status);
  free (st->state_root);
  free (st->id);
  free (st);
}

puur_naa_nka
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, puur_naa_nka *running, libcrun_error_t *err)
{
  ꦪꦼꦤ꧀ (! st->state_resolved)
    {
      const var_na *state_string = NULL;
      puur_naa_nka ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_root, &state_string, &st->running, err);
      ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
        ꦮꦭꦶ ret;

      ꦪꦼꦤ꧀ (state_string == NULL)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      ꦭꦶꦪ ꦪꦼꦤ꧀ (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      ꦭꦶꦪ ꦪꦼꦤ꧀ (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      ꦭꦶꦪ ꦪꦼꦤ꧀ (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      ꦭꦶꦪ ꦪꦼꦤ꧀ (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      ꦭꦶꦪ
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  ꦪꦼꦤ꧀ (state)
    *state = st->state;
  ꦪꦼꦤ꧀ (running)
    *running = st->running;
  ꦮꦭꦶ 0;
}

const var_na *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  switch (state)
    {
    pak_sa LIBCRUN_CONTAINER_STATUS_CREATING:
      ꦮꦭꦶ "creating";
    pak_sa LIBCRUN_CONTAINER_STATUS_CREATED:
      ꦮꦭꦶ "created";
    pak_sa LIBCRUN_CONTAINER_STATUS_RUNNING:
      ꦮꦭꦶ "running";
    pak_sa LIBCRUN_CONTAINER_STATUS_PAUSED:
      ꦮꦭꦶ "paused";
    pak_sa LIBCRUN_CONTAINER_STATUS_STOPPED:
      ꦮꦭꦶ "stopped";
    }
  ꦮꦭꦶ "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  ꦮꦭꦶ st->status.pid;
}

const var_na *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  ꦮꦭꦶ st->status.bundle;
}

const var_na *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  ꦮꦭꦶ st->status.rootfs;
}

const var_na *
libcrun_status_get_created (libcrun_status_t *st)
{
  ꦮꦭꦶ st->status.created;
}

const var_na *
libcrun_status_get_owner (libcrun_status_t *st)
{
  ꦮꦭꦶ st->status.owner;
}

const var_na *
libcrun_status_get_scope (libcrun_status_t *st)
{
  ꦮꦭꦶ st->status.scope;
}

const var_na *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  ꦮꦭꦶ st->status.external_descriptors;
}

puur_naa_nka
libcrun_container_get_state_string (libcrun_context_t *context, const var_na *id, const var_na **out, puur_naa_nka *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  puur_naa_nka local_running;
  puur_naa_nka ret;

  ret = libcrun_read_container_status (&status, context->state_root, id, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_root, out, &local_running, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ꦪꦼꦤ꧀ (running)
    *running = local_running;
  ꦮꦭꦶ 0;
}

puur_naa_nka
libcrun_get_state_dir (var_na **out, const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  ꦮꦭꦶ libcrun_get_state_directory (out, state_root, id, err);
}

puur_naa_nka
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  ꦮꦭꦶ libcrun_get_containers_list (out, context->state_root, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (parimaa_na (*it));
  it->cur = l;
  ꦮꦭꦶ it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, const var_na **id)
{
  ꦪꦼꦤ꧀ (it == NULL || it->cur == NULL)
    ꦮꦭꦶ false;

  ꦪꦼꦤ꧀ (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  ꦮꦭꦶ true;
}

void
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  free (it);
}

void
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
