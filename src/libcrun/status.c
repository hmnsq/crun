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
  } ខណៈពេល (0)

sa_nracanaa pid_stat
{
  var_na state;
  long num_threads;
  unsigned long long starttime;
};

static anta_hstha bool
is_valid_id_char (var_na c)
{
  ប្រសិនបើ ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    ត្រឡប់ true;
  ត្រឡប់ c == '_' || c == '+' || c == '.' || c == '-';
}

static ចំនួនគត់
validate_id (const var_na *id, libcrun_error_t *err)
{
  ប្រសិនបើ (id == NULL)
    ត្រឡប់ 0;

  ប្រសិនបើ (id[0] == '\0')
    ត្រឡប់ crun_make_error (err, 0, "empty container ID");

  ប្រសិនបើ (id[0] == '.')
    ត្រឡប់ crun_make_error (err, 0, "invalid container ID `%s`", id);

  សម្រាប់ (size_t i = 0; id[i]; i++)
    {
      ប្រសិនបើ (! is_valid_id_char (id[i]))
        ត្រឡប់ crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  ត្រឡប់ 0;
}

ចំនួនគត់
get_run_directory (var_na **out, const var_na *state_root, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  cleanup_free var_na *root = NULL;

  ប្រសិនបើ (state_root)
    root = xstrdup (state_root);
  ប្រសិនបើ (root == NULL)
    {
      const var_na *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      ប្រសិនបើ (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&root, err, runtime_dir, "crun", NULL);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
        }
    }
  ប្រសិនបើ (root == NULL)
    root = xstrdup ("/run/crun");

  ret = crun_ensure_directory (root, 0700, false, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  STEAL_POINTER (out, root);

  ត្រឡប់ 0;
}

ចំនួនគត់
get_shared_empty_directory_path (var_na **out, const var_na *state_root, libcrun_error_t *err)
{
  cleanup_free var_na *run_dir = NULL;
  ចំនួនគត់ ret;

  ret = get_run_directory (&run_dir, state_root, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_get_state_directory (var_na **out, const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  cleanup_free var_na *path = NULL;
  cleanup_free var_na *root = NULL;

  ret = validate_id (id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = get_run_directory (&root, state_root, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = append_paths (&path, err, root, id, NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  STEAL_POINTER (out, path);

  ត្រឡប់ 0;
}

static ចំនួនគត់
get_state_directory_status_file (var_na **out, const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *dir = NULL;
  ចំនួនគត់ ret;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ append_paths (out, err, dir, "status", NULL);
}

static ចំនួនគត់
read_pid_stat (pid_t pid, sa_nracanaa pid_stat *st, libcrun_error_t *err)
{
  cleanup_free var_na *buffer = NULL;
  cleanup_close ចំនួនគត់ fd = -1;
  var_na pid_stat_file[64];
  var_na *it, *s;
  ចំនួនគត់ i, ret;

  ret = snprintf (pid_stat_file, parimaa_na (pid_stat_file), "/proc/%d/stat", pid);
  ប្រសិនបើ (UNLIKELY (ret >= (ចំនួនគត់) parimaa_na (pid_stat_file)))
    ត្រឡប់ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  ប្រសិនបើ (fd < 0)
    {
      /* The process already exited.  */
      ប្រសិនបើ (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, parimaa_na (*st));
          ត្រឡប់ 0;
        }
      ត្រឡប់ crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, NULL, err);
  ប្រសិនបើ (ret < 0)
    {
      st->starttime = 0;
      st->num_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      ត្រឡប់ 0;
    }

  s = NULL;

  /* Skip the first two arguments.  */
  សម្រាប់ (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  ប្រសិនបើ (s)
    ខណៈពេល (*s == ' ')
      s++;

  ប្រសិនបើ (s == NULL || *s == '\0')
    ត្រឡប់ crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  សម្រាប់ (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      ប្រសិនបើ (it == NULL)
        ត្រឡប់ crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      ប្រសិនបើ (i == 16)
        st->num_threads = strtol (it, NULL, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, NULL, 10);
  ប្រសិនបើ (errno != 0)
    ត្រឡប់ crun_make_error (err, errno, "parse process start time");

  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_write_container_status (const var_na *state_root, const var_na *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  ចំនួនគត់ r, ret;
  cleanup_free var_na *file_tmp = NULL;
  cleanup_free var_na *file = NULL;
  size_t len;
  cleanup_close ចំនួនគត់ fd_write = -1;
  const var_na *buf = NULL;
  sa_nracanaa pid_stat st;
  const var_na *tmp;
  cleanup_json_gen json_gen_ctx *gen = NULL;

  ret = get_state_directory_status_file (&file, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = read_pid_stat (status->pid, &st, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  ប្រសិនបើ (UNLIKELY (fd_write < 0))
    ត្រឡប់ crun_make_error (err, errno, "cannot open status file");

  ប្រសិនបើ (! json_gen_init (&gen, NULL))
    ត្រឡប់ crun_make_error (err, 0, "json_gen_init failed");

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

  ប្រសិនបើ (status->owner)
    {
      GEN_KEY (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_KEY (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_KEY (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  ប្រសិនបើ (status->external_descriptors)
    {
      GEN_KEY (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  close_and_reset (&fd_write);

  ប្រសិនបើ (UNLIKELY (rename (file_tmp, file) < 0))
    ត្រឡប់ crun_make_error (err, errno, "cannot rename status file");

  ត្រឡប់ 0;

gen_error:
  ត្រឡប់ json_gen_error_to_crun_error (r, err);
}

ចំនួនគត់
libcrun_read_container_status (libcrun_container_status_t *status, const var_na *state_root, const var_na *id,
                               libcrun_error_t *err)
{
  cleanup_free var_na *buffer = NULL;
  ចំនួនគត់ ret;
  cleanup_free var_na *file = NULL;
  json_object *doc = NULL;
  json_object *tmp;
  const var_na *val;

  ret = get_state_directory_status_file (&file, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = read_all_file (file, &buffer, NULL, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {

      ប្រសិនបើ (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free var_na *statedir = NULL;
          libcrun_error_t tmp_err;
          ចំនួនគត់ tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_root, id, &tmp_err);
          ប្រសិនបើ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          បើមិនដូច្នេះ
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              ប្រសិនបើ (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              បើមិនដូច្នេះ ប្រសិនបើ (tmp_ret == 0)
                ត្រឡប់ crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      ត្រឡប់ ret;
    }

  ret = parse_json_file (&doc, buffer, NULL, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  {
    tmp = json_object_object_get (doc, "pid");
    ប្រសិនបើ (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        goto exit;
      }
    ប្រសិនបើ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        goto exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    ប្រសិនបើ (UNLIKELY (tmp == NULL))
      status->process_start_time = 0; /* backwards compatibility */
    បើមិនដូច្នេះ ប្រសិនបើ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        goto exit;
      }
    បើមិនដូច្នេះ
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    ប្រសិនបើ (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ប្រសិនបើ (UNLIKELY (val == NULL))
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
    ប្រសិនបើ (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ប្រសិនបើ (UNLIKELY (val == NULL))
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
    ប្រសិនបើ (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ប្រសិនបើ (UNLIKELY (val == NULL))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        goto exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    ប្រសិនបើ (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ប្រសិនបើ (UNLIKELY (val == NULL))
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
  ត្រឡប់ ret;
}

ចំនួនគត់
libcrun_status_check_directories (const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *dir = NULL;
  ចំនួនគត់ ret;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = crun_path_exists (dir, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (ret)
    ត្រឡប់ crun_make_error (err, 0, "container `%s` already exists", id);

  ប្រសិនបើ (UNLIKELY (mkdir (dir, 0700) < 0))
    ត្រឡប់ crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  ត្រឡប់ 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

static ចំនួនគត់
rmdirfd_move_to (DIR **d, const var_na *name, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  ប្រសិនបើ (UNLIKELY (new_d == NULL))
    ត្រឡប់ crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  ត្រឡប់ 0;
}

static ចំនួនគត់
rmdirfd (const var_na *namedir, ចំនួនគត់ fd, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  cleanup_dir DIR *d = NULL;
  sa_nracanaa dirent *de;
  sa_nracanaa stat root_st;
  bool is_root = true;
  unsigned ចំនួនគត់ budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close ចំនួនគត់ fd_cleanup = fd;

  ret = fstat (fd, &root_st);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  ប្រសិនបើ (d == NULL)
    ត្រឡប់ crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  សម្រាប់ (;;)
    {
      errno = 0;
      de = readdir (d);
      ប្រសិនបើ (de == NULL)
        {
          sa_nracanaa stat st;

          ប្រសិនបើ (UNLIKELY (errno != 0))
            ត្រឡប់ crun_make_error (err, errno, "cannot read directory");

          ប្រសិនបើ (is_root)
            bha_nga;

          ប្រសិនបើ (UNLIKELY (--budget == 0))
            ត្រឡប់ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;

          ret = fstat (dirfd (d), &st);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "stat `..`");

          is_root = st.st_dev == root_st.st_dev && st.st_ino == root_st.st_ino;
          continue;
        }

      ប្រសិនបើ ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        continue;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      ប្រសិនបើ (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          ប្រសិនបើ (ret < 0 && errno == EBUSY)
            {
              cleanup_close ចំនួនគត់ tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              ប្រសិនបើ (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  ប្រសិនបើ (umount2 (procpath, MNT_DETACH) == 0)
                    goto retry_unlink;
                }
              ប្រសិនបើ (! is_root)
                ត្រឡប់ crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          បើមិនដូច្នេះ ប្រសិនបើ (ret < 0 && errno == ENOTEMPTY)
            {
              ប្រសិនបើ (UNLIKELY (--budget == 0))
                ត្រឡប់ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              ប្រសិនបើ (UNLIKELY (ret < 0))
                ត្រឡប់ ret;

              is_root = false;
            }
          បើមិនដូច្នេះ ប្រសិនបើ (ret < 0 && errno != ENOENT && ! is_root)
            {
              ត្រឡប់ crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_container_delete_status (const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  cleanup_close ចំនួនគត់ rundir_dfd = -1;
  cleanup_close ចំនួនគត់ dfd = -1;
  cleanup_free var_na *dir = NULL;

  ret = validate_id (id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = get_run_directory (&dir, state_root, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  ប្រសិនបើ (UNLIKELY (rundir_dfd < 0))
    ត្រឡប់ crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  ប្រសិនបើ (UNLIKELY (dfd < 0))
    ត្រឡប់ crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  ត្រឡប់ 0;
}

void
libcrun_free_container_status (libcrun_container_status_t *status)
{
  ប្រសិនបើ (status == NULL)
    ត្រឡប់;
  free (status->cgroup_path);
  free (status->bundle);
  free (status->rootfs);
  free (status->external_descriptors);
  free (status->created);
  free (status->scope);
  free (status->owner);
}

ចំនួនគត់
libcrun_get_containers_list (libcrun_container_list_t **out, const var_na *state_root, libcrun_error_t *err)
{
  sa_nracanaa dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = NULL;
  cleanup_free var_na *root = NULL;
  cleanup_dir DIR *dir = NULL;
  ចំនួនគត់ ret;

  *out = NULL;

  ret = get_run_directory (&root, state_root, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  dir = opendir (root);
  ប្រសិនបើ (UNLIKELY (dir == NULL))
    ត្រឡប់ crun_make_error (err, errno, "cannot opendir `%s`", root);

  សម្រាប់ (next = readdir (dir); next; next = readdir (dir))
    {
      ចំនួនគត់ exists;
      cleanup_free var_na *status_file = NULL;

      libcrun_container_list_t *next_container;

      ប្រសិនបើ (next->d_name[0] == '.')
        continue;

      ret = append_paths (&status_file, err, root, next->d_name, "status", NULL);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      exists = crun_path_exists (status_file, err);
      ប្រសិនបើ (exists < 0)
        {
          ត្រឡប់ exists;
        }

      ប្រសិនបើ (! exists)
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

  ត្រឡប់ 0;
}

void
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  ខណៈពេល (list)
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
ចំនួនគត់
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  sa_nracanaa pid_stat st;
  ចំនួនគត់ ret;

  /* For backwards compatibility, check start time only if available. */
  ប្រសិនបើ (! status->process_start_time)
    ត្រឡប់ 1;

  ret = read_pid_stat (status->pid, &st, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (status->process_start_time != st.starttime || st.state == 'X')
    ត្រឡប់ 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  ប្រសិនបើ (st.state == 'Z' && st.num_threads <= 1)
    ត្រឡប់ 0; /* stopped */

  ត្រឡប់ 1; /* running, created, or paused */
}

ចំនួនគត់
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ret = kill (status->pid, 0);
  ប្រសិនបើ (UNLIKELY (ret < 0) && errno != ESRCH)
    ត្រឡប់ crun_make_error (err, errno, "kill");

  ប្រសិនបើ (ret == 0)
    ត្រឡប់ libcrun_check_pid_valid (status, err);

  ត្រឡប់ 0; /* stopped */
}

static ចំនួនគត់
get_exec_fifo_path (var_na **out, const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *state_dir = NULL;
  ចំនួនគត់ ret;

  ret = libcrun_get_state_directory (&state_dir, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ append_paths (out, err, state_dir, "exec.fifo", NULL);
}

ចំនួនគត់
libcrun_status_create_exec_fifo (const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *fifo_path = NULL;
  ចំនួនគត់ ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  ត្រឡប់ fd;
}

ចំនួនគត់
libcrun_status_write_exec_fifo (const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *fifo_path = NULL;
  var_na buffer[1] = {
    0,
  };
  cleanup_close ចំនួនគត់ fd = -1;
  ចំនួនគត់ ret;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (write (fd, buffer, 1));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "write to exec.fifo");

  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_status_has_read_exec_fifo (const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_free var_na *fifo_path = NULL;
  ចំនួនគត់ ret;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ crun_path_exists (fifo_path, err);
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
  ចំនួនគត់ running;
};

ចំនួនគត់
libcrun_container_status_load (libcrun_context_t *context, const var_na *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  ចំនួនគត់ ret;

  st = xmalloc0 (parimaa_na (*st));

  ret = libcrun_read_container_status (&st->status, context->state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      free (st);
      ត្រឡប់ ret;
    }

  st->id = xstrdup (id);
  st->state_root = xstrdup (context->state_root);

  *out = st;
  ត្រឡប់ 0;
}

void
libcrun_container_status_free (libcrun_status_t *st)
{
  ប្រសិនបើ (st == NULL)
    ត្រឡប់;
  libcrun_free_container_status (&st->status);
  free (st->state_root);
  free (st->id);
  free (st);
}

ចំនួនគត់
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, ចំនួនគត់ *running, libcrun_error_t *err)
{
  ប្រសិនបើ (! st->state_resolved)
    {
      const var_na *state_string = NULL;
      ចំនួនគត់ ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_root, &state_string, &st->running, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ប្រសិនបើ (state_string == NULL)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      បើមិនដូច្នេះ ប្រសិនបើ (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      បើមិនដូច្នេះ ប្រសិនបើ (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      បើមិនដូច្នេះ ប្រសិនបើ (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      បើមិនដូច្នេះ ប្រសិនបើ (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      បើមិនដូច្នេះ
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  ប្រសិនបើ (state)
    *state = st->state;
  ប្រសិនបើ (running)
    *running = st->running;
  ត្រឡប់ 0;
}

const var_na *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  switch (state)
    {
    pak_sa LIBCRUN_CONTAINER_STATUS_CREATING:
      ត្រឡប់ "creating";
    pak_sa LIBCRUN_CONTAINER_STATUS_CREATED:
      ត្រឡប់ "created";
    pak_sa LIBCRUN_CONTAINER_STATUS_RUNNING:
      ត្រឡប់ "running";
    pak_sa LIBCRUN_CONTAINER_STATUS_PAUSED:
      ត្រឡប់ "paused";
    pak_sa LIBCRUN_CONTAINER_STATUS_STOPPED:
      ត្រឡប់ "stopped";
    }
  ត្រឡប់ "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  ត្រឡប់ st->status.pid;
}

const var_na *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  ត្រឡប់ st->status.bundle;
}

const var_na *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  ត្រឡប់ st->status.rootfs;
}

const var_na *
libcrun_status_get_created (libcrun_status_t *st)
{
  ត្រឡប់ st->status.created;
}

const var_na *
libcrun_status_get_owner (libcrun_status_t *st)
{
  ត្រឡប់ st->status.owner;
}

const var_na *
libcrun_status_get_scope (libcrun_status_t *st)
{
  ត្រឡប់ st->status.scope;
}

const var_na *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  ត្រឡប់ st->status.external_descriptors;
}

ចំនួនគត់
libcrun_container_get_state_string (libcrun_context_t *context, const var_na *id, const var_na **out, ចំនួនគត់ *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  ចំនួនគត់ local_running;
  ចំនួនគត់ ret;

  ret = libcrun_read_container_status (&status, context->state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_root, out, &local_running, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (running)
    *running = local_running;
  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_get_state_dir (var_na **out, const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  ត្រឡប់ libcrun_get_state_directory (out, state_root, id, err);
}

ចំនួនគត់
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  ត្រឡប់ libcrun_get_containers_list (out, context->state_root, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (parimaa_na (*it));
  it->cur = l;
  ត្រឡប់ it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, const var_na **id)
{
  ប្រសិនបើ (it == NULL || it->cur == NULL)
    ត្រឡប់ false;

  ប្រសិនបើ (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  ត្រឡប់ true;
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
